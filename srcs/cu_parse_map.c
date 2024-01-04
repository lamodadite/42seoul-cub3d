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

char	*move_to_map_element(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (1)
	{
		if (line == NULL)
			return (line);
		if (is_map_element(line[0]) == 1 && is_texture_identifier(line) == 0)
			break;
		free(line);
		line = get_next_line(fd);
	}
	return (line);
}

void	set_map_width_height(t_map *map, int fd)
{
	char	*line;

	line = move_to_map_element(fd);
	while (1)
	{
		printf("%s\n", line);
		if (line == NULL)
			break ;
		else if (ft_strncmp(line, "\n", 1) == 1)
			print_error_and_exit("empty line between map element\n");
		else if (is_map_element(line[0]) == 0)
			print_error_and_exit("invalid map element2\n");
		else
		{
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
	map->map = (char **)ft_calloc(sizeof(char *), map->height);
	while (++i < map->height)
		map->map[i] = (char *)ft_calloc(sizeof(char), map->width);
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
			printf("height_cnt -> %d, width -> %d, line[width] -> %c\n", height_cnt, width, line[width]);
			if (*pos_flag == 1)
				print_error_and_exit("charater must be one\n");
			*pos_flag = 1;
		}
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
	printf("%s\n", line);
	pre_set_map(map);
	height_cnt = 0;
	while (1)
	{
		if (line == NULL)
			break ;
		else if (height_cnt == map->height)
			print_error_and_exit("invalid map element3\n");
		set_map_component(map, line, height_cnt, &pos_flag);
		free (line);
		line = get_next_line(fd);
		height_cnt++;
	}
	close(fd);
}

void	get_map(t_map *map, int fd, char *map_path)
{
	set_map_width_height(map, fd);
	set_map_elements(map, map_path);
}
