/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cu_parse_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeongsh <hyeongsh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:11:37 by hyeongsh          #+#    #+#             */
/*   Updated: 2024/01/11 22:12:10 by hyeongsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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
		|| c == 'S' || c == 'E' || c == 'W' || c == 'D')
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
		if (map->texture.no_path != NULL && ft_strncmp(path, map->texture.no_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated no_path error\n");
	}
	if (i++ != identifier)
	{
		if (map->texture.so_path != NULL && ft_strncmp(path, map->texture.so_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated so_path error\n");
	}
	if (i++ != identifier)
	{
		if (map->texture.we_path != NULL && ft_strncmp(path, map->texture.we_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated we_path error\n");
	}
	if (i != identifier)
	{
		if (map->texture.ea_path != NULL && ft_strncmp(path, map->texture.ea_path, ft_strlen(path)) == 0)
			print_error_and_exit("duplicated ea_path error\n");
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
			path[ft_strlen(path) - 1] = '\0';
		fd = open(path, O_RDONLY);
		if (fd < 0)
			print_error_and_exit("file open error\n");
		close(fd);
	}
}

void	check_color_is_valid(t_map *map, char *line, char identifier)
{
	char	**splited_line;
	int		i;
	int		j;

	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	splited_line = ft_split(line, ',');
	if (get_str_2d_len(splited_line) != 3)
		print_error_and_exit("invalid color\n");
	i = -1;
	while (++i < 3)
	{
		if (ft_strlen(splited_line[i]) > 3)
			print_error_and_exit("color value is over 255\n");
		j = -1;
		while (++j < (int)ft_strlen(splited_line[i]))
		{
			if (ft_isdigit(splited_line[i][j]) == 0)
				print_error_and_exit("color value isn't numeric\n");
		}
		if (identifier == 'F')
			map->texture.f_color[i] = ft_atoi(splited_line[i]);
		else if (identifier == 'C')
			map->texture.c_color[i] = ft_atoi(splited_line[i]);
	}
}

void	set_texture_path(t_map *map, char *line)
{
	char		**splited_line;
	int			identifier;

	splited_line = ft_split(line, ' ');
	identifier = is_texture_identifier(splited_line[0]);
	if (identifier == NO && map->texture.no_path == NULL)
		map->texture.no_path = ft_strdup(splited_line[1]);
	else if (identifier == SO && map->texture.so_path == NULL)
		map->texture.so_path = ft_strdup(splited_line[1]);
	else if (identifier == WE && map->texture.we_path == NULL)
		map->texture.we_path = ft_strdup(splited_line[1]);
	else if (identifier == EA && map->texture.ea_path == NULL)
		map->texture.ea_path = ft_strdup(splited_line[1]);
	else if (identifier == F && map->texture.f_color[0] == -1)
		check_color_is_valid(map, splited_line[1], 'F');
	else if (identifier == C && map->texture.c_color[0] == -1)
		check_color_is_valid(map, splited_line[1], 'C');
	else
		print_error_and_exit("identifier is duplicated\n");
	is_valid_path(identifier, splited_line[1], map);
	free_2d_array(splited_line);
	map->texture.cnt++;
}

void	trim_textures(t_texture *texture)
{
	if (texture->no_path[ft_strlen(texture->no_path) - 1] == '\n')
		texture->no_path[ft_strlen(texture->no_path) - 1] = '\0';
	if (texture->so_path[ft_strlen(texture->so_path) - 1] == '\n')
		texture->so_path[ft_strlen(texture->so_path) - 1] = '\0';
	if (texture->we_path[ft_strlen(texture->we_path) - 1] == '\n')
		texture->we_path[ft_strlen(texture->we_path) - 1] = '\0';
	if (texture->ea_path[ft_strlen(texture->ea_path) - 1] == '\n')
		texture->ea_path[ft_strlen(texture->ea_path) - 1] = '\0';
}

void	get_texture(t_map *map, int fd)
{
	char	*line;

	while (42)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (is_map_element_arr(line) == 1)
			break ;
		else if (ft_strncmp(line, "\n", 2) == 0)
			continue ;
		else if (is_texture_element(line) == 0)
			set_texture_path(map, line);
		else
			print_error_and_exit("invalid element\n");
		free(line);
	}
	if (map->texture.cnt != 6)
		print_error_and_exit("number of texture is wrong\n");
	trim_textures(&map->texture);
}

void	load_file(char *map_path, t_map *map)
{
	int	fd;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		print_error_and_exit("file open error\n");
	init_map(map);
	get_texture(map, fd);
	get_map(map, map_path);
	close(fd);
}
