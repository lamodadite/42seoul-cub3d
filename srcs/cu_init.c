#include "cub.h"

void	init_map(t_map *map)
{
	map->map = NULL;
	map->height = 0;
	map->width = 0;
	map->pp.x = 0;
	map->pp.y = 0;
	map->texture.cnt = 0;
	map->texture.ea_path = NULL;
	map->texture.no_path = NULL;
	map->texture.so_path = NULL;
	map->texture.we_path = NULL;
	map->texture.f_color[0] = -1;
	map->texture.c_color[0] = -1;
}

void	init_dir(t_info *info, t_map *map)
{
	if (map->dir == 'N')
	{
		info->dir.x = -1.0;
		info->dir.y = 0.0;
	}
	if (map->dir == 'S')
	{
		info->dir.x = 1.0;
		info->dir.y = 0.0;
	}
	if (map->dir == 'E')
	{
		info->dir.x = 0.0;
		info->dir.y = -1.0;
	}
	if (map->dir == 'W')
	{
		info->dir.x = 0.0;
		info->dir.y = 1.0;
	}
}

void	init_info(t_info *info, t_map *map)
{
	int	i;

	info->pos.x = map->pp.x + 0.01;
	info->pos.y = map->pp.y + 0.51;
	init_dir(info, map);
	info->plane.x = 0.0;
	info->plane.y = 0.66;
	info->re_buf = 0;
	info->map = map;
	info->move_speed = 0.1;
	info->rot_speed = 0.1;
	info->mlx = mlx_init();
	i = 0;
	while (i < HEIGHT)
		ft_bzero(info->buf[i++], WIDTH);
	info->texture = (int **)ft_calloc(sizeof(int *), 4);
	i = 0;
	while (i < 4)
		info->texture[i++] = (int *)ft_calloc(sizeof(int), (TEX_HEIGHT * TEX_WIDTH));
	check_and_set_color(info, map, 'F');
	check_and_set_color(info, map, 'C');
}

void	check_and_set_color(t_info *info, t_map *map, char identifier)
{
	int		i;
	int		color;
	t_rgb	rgb;

	i = -1;
	while (++i < 3)
	{
		if (identifier == 'F')
			color = map->texture.f_color[i];
		else if (identifier == 'C')
			color = map->texture.c_color[i];
		if (!(color >= 0 && color <= 255))
			print_error_and_exit("color must in 255\n");
		if (i == 0)
			rgb.r = color;
		if (i == 1)
			rgb.g = color;
		if (i == 2)
			rgb.b = color;
	}
	if (identifier == 'F')
		info->floor_color = (rgb.r << 16 | rgb.g << 8 | rgb.b);
	else if (identifier == 'C')
		info->ceiling_color = (rgb.r << 16 | rgb.g << 8 | rgb.b);
}
