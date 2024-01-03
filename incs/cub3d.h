#ifndef CUB3D_H
# define CUB3D_H

#include "keys.h"
#include <mlx.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define X_EVENT_KEY_PRESS	2
#define X_EVENT_KEY_EXIT	17
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define mapWidth 24
#define mapHeight 24
#define WIDTH 640
#define HEIGHT 480

typedef struct	s_img
{
	void	*img;
	int		*data;
	int		size_l;
	int		bpp;
	int		endian;
	int		img_width;
	int		img_height;
}				t_img;

typedef struct	s_info
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	void	*mlx;
	void	*win;
	t_img	img;
	int		buf[HEIGHT][WIDTH];
	int		**texture;
	double	move_speed;
	double	rot_speed;
	int		re_buf;
}				t_info;

#endif
