#include "cub.h"

void	set_map_width(t_map *map, char *line)
{
	int	i;

	i = -1;
	while (line[++i] != '\n' && line[i] != '\0')
	{
		if (is_map_element(line[i]) == 0)
			print_error_and_exit("invalid map element1\n");
	}
	if (i > map->width)
		map->width = i;
}

int	is_map_element_arr(char *line)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (line[0] == '\n')
		return (0);
	while (line[i] != '\0' && line[i] != '\n')
	{
		if (is_map_element(line[i]) == 0)
			return (0);
		if (line[i] != ' ')
			flag = 1;
		i++;
	}
	if (flag == 0)
		return (0);
	return (1);
}

char	*move_to_map_element(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (1)
	{
		if (line == NULL)
			return (line);
		//printf("in move_to_map_element -> %s\n", line);
		if (is_map_element_arr(line) == 1)
			break ;
		free(line);
		line = get_next_line(fd);
	}
	return (line);
}

void	set_map_width_height(t_map *map, int fd)
{
	char	*line;

	line = move_to_map_element(fd);
	//printf("first move to map -> %s\n", line);
	while (1)
	{
		if (line == NULL)
			break ;
		else if (ft_strncmp(line, "\n", 1) == 1)
			print_error_and_exit("empty line between map element\n");
		else if (is_map_element(line[0]) == 0)
			print_error_and_exit("invalid map element2\n");
		else
		{
			//printf("%s\n", line);
			map->height++;
			set_map_width(map, line);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	pre_set_map(t_map *map)
{
	int	i;
	int	j;

	i = -1;
	map->map = (char **)ft_calloc(sizeof(char *), map->height + 1);
	while (++i < map->height)
		map->map[i] = (char *)ft_calloc(sizeof(char), map->width + 1);
	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			map->map[i][j] = ' ';
		}
	}
}

void	set_map_component(t_map *map, char *line, int height_cnt, int *pos_flag)
{
	int	width;

	width = 0;
	while (line[width] != '\0' && line[width] != '\n')
	{
		if (line[width] != ' ' && line[width] != '0' && line[width] != '1')
		{
			if (*pos_flag == 1)
				print_error_and_exit("character must be one\n");
			*pos_flag = 1;
			map->pp.x = height_cnt;
			map->pp.y = width;
			map->dir = line[width];
			//printf("height_cnt -> %d, width -> %d\n", height_cnt, width);
		}
		//printf("height_cnt -> %d, width -> %d, line[width] -> %c, %d\n", height_cnt, width, line[width], line[width]);
		map->map[height_cnt][width] = line[width];
		width++;
	}
	return ;
}

void	set_map_elements(t_map *map, char *map_path)
{
	int		fd;
	int		pos_flag;
	int		height_cnt;
	char	*line;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		print_error_and_exit("file open error\n");
	line = move_to_map_element(fd);
	//printf("second move to map -> %s\n", line);
	pre_set_map(map);
	height_cnt = 0;
	while (1)
	{
		//printf("%s\n", line);
		if (line == NULL)
			break ;
		set_map_component(map, line, height_cnt, &pos_flag);
		free(line);
		line = get_next_line(fd);
		height_cnt++;
	}
	close(fd);
}

void	check_map_is_surrouned(t_map *map, int h, int w)
{
	if (w - 1 >= 0 && (map->map[h][w - 1] != ' ' && map->map[h][w - 1] != '1'))
		print_error_and_exit("map must be closed by wall3\n");
	if (w + 1 < map->width && map->map[h][w + 1] != ' ' && map->map[h][w + 1] != '1')
		print_error_and_exit("map must be closed by wall4\n");
	if (h + 1 < map->height && map->map[h + 1][w] != ' ' && map->map[h + 1][w] != '1')
		print_error_and_exit("map must be closed by wall5\n");
	if (h - 1 >= 0 && map->map[h - 1][w] != ' ' && map->map[h - 1][w] != '1')
		print_error_and_exit("map must be closed by wall6\n");
}

void	check_map_is_valid(t_map *map)
{
	int	x;
	int	y;

	x = -1;
	while (++x < map->height)
	{
		y = -1;
		while (++y < map->width)
		{
			//printf("x -> %d, y -> %d\n", x, y);
			if ((x == 0 || x == map->height - 1) &&
				(map->map[x][y] != ' ' && map->map[x][y] != '1'))
				print_error_and_exit("map must be closed by wall1\n");
			if ((y == 0 || y == map->width - 1) &&
				(map->map[x][y] != ' ' && map->map[x][y] != '1'))
				print_error_and_exit("map must be closed by wall2\n");
			if (map->map[x][y] == ' ')
				check_map_is_surrouned(map, x, y);
		}
	}
}

void	get_map(t_map *map, char *map_path)
{
	int	fd;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		print_error_and_exit("file open error\n");
	set_map_width_height(map, fd);
	//print_map_struct(map);
	set_map_elements(map, map_path);
	check_map_is_valid(map);
}
