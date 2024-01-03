/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:18:03 by yohlee            #+#    #+#             */
/*   Updated: 2024/01/03 17:26:07 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	worldMap[MAP_WIDTH][MAP_HEIGHT] =
						{
							{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
							{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
							{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
							{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
							{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
							{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
							{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
							{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
							{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
							{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
							{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
							{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
							{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
							{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
							{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
							{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
							{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
							{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
							{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
							{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
							{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
						};

void	draw(t_info *info)
{
	int	x;
	int	y;

	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
		{
			info->img.data[y * WIDTH + x] = info->buf[y][x];
		}
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}

void	floor_casting(t_info *info)
{
	float	ray_dir_x0;
	float	ray_dir_y0;
	float	ray_dir_x1;
	float	ray_dir_y1;
	int		p;
	float	pos_z;
	float	row_dist;
	float	floor_step_x;
	float	floor_step_y;
	float	floor_x;
	float	floor_y;
	float	cell_x;
	float	cell_y;
	int		floor_texture;
	int		ceiling_texture;
	int		color;
	int		tx;
	int		ty;
	int		y;
	int		x;

	y = -1;
	x = -1;
	while (++y < HEIGHT)
	{
		ray_dir_x0 = info->dir_x - info->plane_x;
		ray_dir_y0 = info->dir_y - info->plane_y;
		ray_dir_x1 = info->dir_x + info->plane_x;
		ray_dir_y1 = info->dir_y + info->plane_y;
		p = y - HEIGHT / 2;
		pos_z = 0.5 * HEIGHT;
		row_dist = pos_z / p;
		floor_step_x = row_dist * (ray_dir_x1 - ray_dir_x0) / WIDTH;
		floor_step_y = row_dist * (ray_dir_y1 - ray_dir_y0) / WIDTH;
		floor_x = info->pos_x + row_dist * ray_dir_x0;
		floor_y = info->pos_y + row_dist * ray_dir_y0;
		while (++x < WIDTH)
		{
			cell_x = (int)(floor_x);
			cell_y = (int)(floor_y);
			tx = (int)(TEX_WIDTH * (floor_x - cell_x)) & (TEX_WIDTH - 1);
			ty = (int)(TEX_HEIGHT * (floor_y - cell_y)) & (TEX_HEIGHT - 1);
			floor_x += floor_step_x;
			floor_y += floor_step_y;
			floor_texture = 3;
			ceiling_texture = 6;
			color = info->texture[floor_texture][TEX_WIDTH * ty + tx];
			color = (color >> 1) & 8355711;
			info->buf[y][x] = color;
			color = info->texture[ceiling_texture][TEX_WIDTH * ty + tx];
			color = (color >> 1) & 8355711;
			info->buf[HEIGHT - y - 1][x] = color;
		}
	}
}

void	wall_casting(t_info *info)
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_num;
	double	wall_x;
	int		tex_x;
	double	step;
	double	tex_pos;
	int		tex_y;
	int		color;
	double	floor_x_wall;
	double	floor_y_wall;
	double	dist_wall;
	double	dist_player;
	double	current_dist;
	double	weight;
	double	current_floor_x;
	double	current_floor_y;
	int		floor_tex_x;
	int		floor_tex_y;
	int		checker_board_pattern;
	int		floor_texture;
	int 	x;
	int		y;

	x = -1;
	y = -1;
	while (++x < WIDTH)
	{
		camera_x = 2 * x / (double) WIDTH - 1;
		ray_dir_x = info->dir_x + info->plane_x * camera_x;
		ray_dir_y = info->dir_y + info->plane_y * camera_x;
		map_x = (int)info->pos_x;
		map_y = (int)info->pos_y;
		delta_dist_x = fabs(1 / ray_dir_x);
		delta_dist_y = fabs(1 / ray_dir_y);
		hit = 0;
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (info->pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - info->pos_x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (info->pos_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - info->pos_y) * delta_dist_y;
		}
		while (hit == 0)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (worldMap[map_x][map_y] > 0)
				hit = 1;
		}
		if (side == 0)
			perp_wall_dist = (map_x - info->pos_x + (1 - step_x) / 2) / ray_dir_x;
		else
			perp_wall_dist = (map_y - info->pos_y + (1 - step_y) / 2) / ray_dir_y;
		line_height = (int)(HEIGHT / perp_wall_dist);
		draw_start = -line_height / 2 + HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = line_height / 2 + HEIGHT / 2;
		if (draw_end >= HEIGHT)
			draw_end = HEIGHT - 1;
		tex_num = worldMap[map_x][map_y];
		if (side == 0)
			wall_x = info->pos_y + perp_wall_dist * ray_dir_y;
		else
			wall_x = info->pos_x + perp_wall_dist * ray_dir_x;
		wall_x -= floor(wall_x);
		tex_x = (int)(wall_x * (double) TEX_WIDTH);
		if (side == 0 && ray_dir_x > 0)
			tex_x = TEX_WIDTH - tex_x - 1;
		if (side == 1 && ray_dir_y < 0)
			tex_x = TEX_WIDTH - tex_x - 1;
		step = 1.0 * TEX_HEIGHT / line_height;
		tex_pos = (draw_start - HEIGHT / 2 + line_height / 2) * step;
		y = draw_start - 1;
		while (++y < draw_end)
		{
			tex_y = (int) tex_pos & (TEX_HEIGHT - 1);
			tex_pos += step;
			color = info->texture[tex_num][TEX_HEIGHT * tex_y + tex_x];
			if (side == 1)
				color = (color >> 1) & 8355711;
			info->buf[y][x] = color;
		}
		if (side == 0 && ray_dir_x > 0)
		{
			floor_x_wall = map_x;
			floor_y_wall = map_y + wall_x;
		}
		else if (side == 0 && ray_dir_x < 0)
		{
			floor_x_wall = map_x + 1.0;
			floor_y_wall = map_y + wall_x;
		}
		else if (side == 1 && ray_dir_y > 0)
		{
			floor_x_wall = map_x + wall_x;
			floor_y_wall = map_y;
		}
		else
		{
			floor_x_wall = map_x + wall_x;
			floor_y_wall = map_y + 1.0;
		}
		dist_wall = perp_wall_dist;
		dist_player = 0.0;
		if (draw_end < 0)
			draw_end = HEIGHT;
		y = draw_end;
		while (++y < HEIGHT)
		{
			current_dist = HEIGHT / (2.0 * y - HEIGHT);
			weight = (current_dist - dist_player) / (dist_wall - dist_player);
			current_floor_x = weight * floor_x_wall + (1.0 - weight) * info->pos_x;
			current_floor_y = weight * floor_y_wall + (1.0 - weight) * info->pos_y;
			floor_tex_x = (int)(current_floor_x * TEX_WIDTH) % TEX_WIDTH;
			floor_tex_y = (int)(current_floor_y * TEX_HEIGHT) % TEX_HEIGHT;
			checker_board_pattern = ((int)(current_floor_x) + (int)(current_floor_y)) % 2;
			if (checker_board_pattern == 0)
				floor_texture = 3;
			else
				floor_texture = 4;
			info->buf[y][x] = (info->texture[floor_texture][TEX_WIDTH * floor_tex_y + floor_tex_x] >> 1) & 8355711;
			info->buf[HEIGHT - y][x] = info->texture[6][TEX_WIDTH * floor_tex_y + floor_tex_x];
		}
	}
}

void	calc(t_info *info)
{
	floor_casting(info);
	wall_casting(info);
}

int	main_loop(t_info *info)
{
	calc(info);
	draw(info);
	return (0);
}

int	key_press(int key, t_info *info)
{
	double old_dir_x;
	double old_plane_x;
	
	if (key == K_W)
	{
		if (!worldMap[(int)(info->pos_x + info->dir_x * info->move_speed)][(int)(info->pos_y)])
			info->pos_x += info->dir_x * info->move_speed;
		if (!worldMap[(int)(info->pos_x)][(int)(info->pos_y + info->dir_y * info->move_speed)])
			info->pos_y += info->dir_y * info->move_speed;
	}
	//move backwards if no wall behind you
	if (key == K_S)
	{
		if (!worldMap[(int)(info->pos_x - info->dir_x * info->move_speed)][(int)(info->pos_y)])
			info->pos_x -= info->dir_x * info->move_speed;
		if (!worldMap[(int)(info->pos_x)][(int)(info->pos_y - info->dir_y * info->move_speed)])
			info->pos_y -= info->dir_y * info->move_speed;
	}
	if (key == K_A)
	{
		if (!worldMap[(int)(info->pos_x - info->dir_y * info->move_speed)][(int)(info->pos_y)])
			info->pos_x -= info->dir_y * info->move_speed;
		if (!worldMap[(int)(info->pos_x)][(int)(info->pos_y + info->dir_x * info->move_speed)])
			info->pos_y += info->dir_x * info->move_speed;
	}
	if (key == K_D)
	{
		if (!worldMap[(int)(info->pos_x + info->dir_y * info->move_speed)][(int)(info->pos_y)])
			info->pos_x += info->dir_y * info->move_speed;
		if (!worldMap[(int)(info->pos_x)][(int)(info->pos_y - info->dir_x * info->move_speed)])
			info->pos_y -= info->dir_x * info->move_speed;
	}
	//rotate to the right
	if (key == K_AR_R)
	{
		//both camera direction and camera plane must be rotated
		old_dir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(-info->rot_speed) - info->dir_y * sin(-info->rot_speed);
		info->dir_y = old_dir_x * sin(-info->rot_speed) + info->dir_y * cos(-info->rot_speed);
		old_plane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(-info->rot_speed) - info->plane_y * sin(-info->rot_speed);
		info->plane_y = old_plane_x * sin(-info->rot_speed) + info->plane_y * cos(-info->rot_speed);
	}
	//rotate to the left
	if (key == K_AR_L)
	{
		//both camera direction and camera plane must be rotated
		old_dir_x = info->dir_x;
		info->dir_x = info->dir_x * cos(info->rot_speed) - info->dir_y * sin(info->rot_speed);
		info->dir_y = old_dir_x * sin(info->rot_speed) + info->dir_y * cos(info->rot_speed);
		old_plane_x = info->plane_x;
		info->plane_x = info->plane_x * cos(info->rot_speed) - info->plane_y * sin(info->rot_speed);
		info->plane_y = old_plane_x * sin(info->rot_speed) + info->plane_y * cos(info->rot_speed);
	}
	if (key == K_ESC)
		exit(0);
	mlx_clear_window(info->mlx, info->win);
	main_loop(info);
	return (0);
}

void	load_image(t_info *info, int *texture, char *path, t_img *img)
{
	img->img = mlx_xpm_file_to_image(info->mlx, path, &img->img_width, &img->img_height);
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);
	for (int y = 0; y < img->img_height; y++)
	{
		for (int x = 0; x < img->img_width; x++)
		{
			texture[img->img_width * y + x] = img->data[img->img_width * y + x];
		}
	}
	mlx_destroy_image(info->mlx, img->img);
}

void	load_texture(t_info *info)
{
	t_img	img;

	load_image(info, info->texture[0], "textures/eagle.xpm", &img);
	load_image(info, info->texture[1], "textures/redbrick.xpm", &img);
	load_image(info, info->texture[2], "textures/purplestone.xpm", &img);
	load_image(info, info->texture[3], "textures/greystone.xpm", &img);
	load_image(info, info->texture[4], "textures/bluestone.xpm", &img);
	load_image(info, info->texture[5], "textures/mossy.xpm", &img);
	load_image(info, info->texture[6], "textures/wood.xpm", &img);
	load_image(info, info->texture[7], "textures/colorstone.xpm", &img);
}

int	main(void)
{
	t_info info;

	info.mlx = mlx_init();
	info.pos_x = 22.0;
	info.pos_y = 11.5;
	info.dir_x = -1.0;
	info.dir_y = 0.0;
	info.plane_x = 0.0;
	info.plane_y = 0.66;
	info.re_buf = 0;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
			info.buf[i][j] = 0;
	}
	if (!(info.texture = (int **)malloc(sizeof(int *) * 8)))
		return (-1);
	for (int i = 0; i < 8; i++)
	{
		if (!(info.texture[i] = (int *)malloc(sizeof(int) * (TEX_HEIGHT * TEX_WIDTH))))
			return (-1);
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < TEX_HEIGHT * TEX_WIDTH; j++)
		{
			info.texture[i][j] = 0;
		}
	}

	load_texture(&info);

	info.move_speed = 0.1;
	info.rot_speed = 0.1;
	
	info.win = mlx_new_window(info.mlx, WIDTH, HEIGHT, "mlx");

	info.img.img = mlx_new_image(info.mlx, WIDTH, HEIGHT);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);

	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);

	mlx_loop(info.mlx);
}
