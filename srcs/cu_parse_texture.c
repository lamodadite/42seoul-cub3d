#include "cub.h"

void	print_error_and_exit(char *message)
{
	write(2, "\033[0;35mError\n", ft_strlen("\033[0;35mError\n"));
	write(2, message, ft_strlen(message));
	write(2, "\033[0;0m", ft_strlen("\033[0;0m"));
	exit(1);
}

void	init_map(t_map *map)
{
	map->map = NULL;
	map->height = 0;
	map->width = 0;
	map->texture = (t_texture *)ft_calloc(sizeof(t_texture *), 1);
	map->texture->cnt = 0;
	map->texture->no_path = NULL;
	map->texture->so_path = NULL;
	map->texture->we_path = NULL;
	map->texture->ea_path = NULL;
	map->texture->c_color = NULL;
	map->texture->f_color = NULL;
	map->player = (t_player *)ft_calloc(sizeof(t_player *), 1);
	map->player->pos_x = 0;
	map->player->pos_y = 0;
	map->player->dir_x = 0;
	map->player->dir_y = 0;
	map->player->plane_x = 0;
	map->player->plane_y = 0;
}

void	check_map_name(char *map_name)
{
	int	len;

	len = ft_strlen(map_name);
	if (len < 4)
		print_error_and_exit("check map name error\n");
	if (map_name[len - 1] != 'b' || map_name[len - 2] != 'u'
		|| map_name[len - 3] != 'c' || map_name[len - 4] != '.')
		print_error_and_exit("check map name error\n");
}

int	is_map_element(char c)
{
	int	result;

	result = 0;
	if (c == ' ' || c == '1' || c == '0' || c == 'N'
		|| c == 'S' || c == 'E' || c == 'W')
		result = 1;
	return (result);
}

int	is_texture_identifier(char *line)
{
	int	result;

	result = 0;
	if (ft_strncmp(line, "NO", 3) == 0)
		result = 1;
	else if (ft_strncmp(line, "SO", 3) == 0)
		result = 2;
	else if (ft_strncmp(line, "EA", 3) == 0)
		result = 3;
	else if (ft_strncmp(line, "WE", 3) == 0)
		result = 4;
	else if (ft_strncmp(line, "F", 2) == 0)
		result = 5;
	else if (ft_strncmp(line, "C", 2) == 0)
		result = 6;
	return (result);
}

int	is_texture_element(char *line)
{
	char	**splited_line;
	int		result;

	splited_line = ft_split(line, ' ');
	result = 0;
	if (is_texture_identifier(splited_line[0]) == 0)
		result = 1;
	else if (splited_line[1] == NULL)
		result = 1;
	else if (splited_line[2] != NULL)
		result = 1;
	free_2d_array(splited_line);
	return (result);
}

void	is_duplicated_path(int identifier, char *path, t_map *map)
{
	int	i;

	i = 1;
	if (i++ != identifier)
	{
		if (map->texture->no_path != NULL && ft_strncmp(path, map->texture->no_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated path error\n");
	}
	if (i++ != identifier)
	{
		if (map->texture->so_path != NULL && ft_strncmp(path, map->texture->so_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated path error\n");
	}
	if (i++ != identifier)
	{
		if (map->texture->we_path != NULL && ft_strncmp(path, map->texture->we_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated path error\n");
	}
	if (i != identifier)
	{
		if (map->texture->ea_path != NULL && ft_strncmp(path, map->texture->ea_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated path error\n");
	}
}

void	is_valid_path(int identifier, char *path, t_map *map)
{
	int		fd;

	if (identifier == NO || identifier == SO
		|| identifier == WE || identifier == EA)
	{
		is_duplicated_path(identifier, path, map);
		if (path[ft_strlen(path) - 1] == '\n')
			path = ft_substr(path, 0, ft_strlen(path) - 1);
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			printf("%s\n", path);
			print_error_and_exit("file open error\n");
		}
		close(fd);
		free(path);
	}
}

void	set_texture_path(t_map *map, char *line)
{
	char		**splited_line;
	int			identifier;

	splited_line = ft_split(line, ' ');
	identifier = is_texture_identifier(splited_line[0]);
	if (identifier == NO && map->texture->no_path == NULL)
		map->texture->no_path = ft_strdup(splited_line[1]);
	else if (identifier == SO && map->texture->so_path == NULL)
		map->texture->so_path = ft_strdup(splited_line[1]);
	else if (identifier == WE && map->texture->we_path == NULL)
		map->texture->we_path = ft_strdup(splited_line[1]);
	else if (identifier == EA && map->texture->ea_path == NULL)
		map->texture->ea_path = ft_strdup(splited_line[1]);
	else if (identifier == F && map->texture->f_color == NULL)
		map->texture->f_color = ft_strdup(splited_line[1]);
	else if (identifier == C && map->texture->c_color == NULL)
		map->texture->c_color = ft_strdup(splited_line[1]);
	else
		print_error_and_exit("identifer is duplicated\n");
	is_valid_path(identifier, splited_line[1], map);
	free_2d_array(splited_line);
	map->texture->cnt++;
}

void	get_texture(t_map *map, int fd)
{
	char	*line;

	while (map->texture->cnt < 6)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		else if (ft_strncmp(line, "\n", 2) == 0)
			continue ;
		else if (is_texture_element(line) == 0)
			set_texture_path(map, line);
		else
			print_error_and_exit("invalid element\n");
		free(line);
	}
	if (map->texture->cnt != 6)
		print_error_and_exit("check texture element\n");
}

void	load_file(char *map_path, t_map *map)
{
	int	fd;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		print_error_and_exit("file open error\n");
	init_map(map);
	get_texture(map, fd);
	get_map(map, fd, map_path);
	close(fd);
}
