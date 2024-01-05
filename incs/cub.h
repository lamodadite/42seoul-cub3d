/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeongsh <hyeongsh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:55:32 by hyeongsh          #+#    #+#             */
/*   Updated: 2024/01/05 16:11:43 by hyeongsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include "keys.h"
# include <mlx.h>
# include <math.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

# define X_EVENT_KEY_PRESS	2
# define X_EVENT_KEY_EXIT	17
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
# define mapWidth 24
# define mapHeight 24
# define WIDTH 640
# define HEIGHT 480
// mlx image

int	worldMap[mapHeight][mapWidth];

typedef struct s_ipos
{
	int	x;
	int	y;
}	t_ipos;

typedef struct s_dpos
{
	double	x;
	double	y;
}	t_dpos;

typedef struct s_img
{
	void	*img;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
	int		img_width;
	int		img_height;
}	t_img;

typedef struct s_info
{
	t_dpos	pos;
	t_dpos	dir;
	t_dpos	plane;
	void	*mlx;
	void	*win;
	t_img	img;
	int		buf[HEIGHT][WIDTH];
	int		**texture;
	double	move_speed;
	double	rot_speed;
	int		re_buf;
}	t_info;

typedef struct s_wall
{
	t_ipos	step;
	t_dpos	ray_dir;
	t_ipos	map;
	double	perp_wall_dist;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_num;
	double	ratio;
	t_dpos	floor;
	t_ipos	tex;
	int		a;
	int		b;
}	t_wall;

/* cu_load_data */
void	load_image(t_info *info, int *texture, char *path, t_img *img);
void	load_texture(t_info *info);

/* cu_check_key */
int		press_key(int key, t_info *info);
void	press_front_back_key(int key, t_info *info);
void	press_left_right_key(int key, t_info *info);
void	press_left_rotate_key(int key, t_info *info);
void	press_right_rotate_key(int key, t_info *info);

/* cu_cast_floor */
void	cast_floor(t_info *info);
void	set_floor_loop(t_info *info, int a, t_dpos *floor_step, t_dpos *floor);
void	find_texture_index(t_ipos *tex, t_dpos *floor_step, t_dpos *floor);
void	enter_color_in_buf(t_info *info, t_ipos *tex, int a, int b);


void	draw(t_info *info);

void	cast_wall(t_info *info);
void	calc_wall_dist(t_info *info, t_wall *wall);
int		hit_wall(t_info *info, t_wall *wall);
int		check_hit_wall(t_dpos *side_dist, t_dpos *delta_dist, t_wall *wall);
void	find_draw_part(t_info *info, t_wall *wall);
void	put_buf_wall_line(t_info *info, t_wall *wall);
void	find_floor_pos(t_wall *wall);

#endif
