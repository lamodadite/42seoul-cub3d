#ifndef CUB_H
# define CUB_H

# include "keys.h"
# include "libft.h"
# include "get_next_line_bonus.h"
# include <fcntl.h>
# include <mlx.h>
# include <math.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

# define X_EVENT_KEY_PRESS	2
# define X_EVENT_KEY_EXIT	17
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
# define MAP_WIDTH 24
# define MAP_HEIGHT 24
# define WIDTH 640
# define HEIGHT 480

# define MAP_PATH "map/"
# define NO 1
# define SO 2
# define WE 3
# define EA 4
# define F 5
# define C 6

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_texture
{
	int		cnt;
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	char	*c_color;
	char	*f_color;
}	t_texture;

typedef struct s_map
{
	char		**map;
	int			height;
	int			width;
	t_texture	*texture;
	t_player	*player;
}	t_map;

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

/* cu_parse_texture.c */
void	print_error_and_exit(char *message);
void	init_map(t_map *map);
void	check_map_name(char *map_name);
int		is_map_element(char c);
int		is_texture_identifier(char *line);
int		is_texture_element(char *line);
void	is_duplicated_path(int identifier, char *path, t_map *map);
void	is_valid_path(int identifier, char *path, t_map *map);
void	set_texture_path(t_map *map, char *line);
void	get_texture(t_map *map, int fd);
void	load_file(char *map_path, t_map *map);

/* cu_parse_map.c */
void	get_map(t_map *map, int fd, char *map_path);
void	set_map_width_height(t_map *map, int fd);
char	*move_to_map_element(int fd);
void	set_map_width(t_map *map, char *line);
void	check_map_is_surrouned(t_map *map, int h, int w);
void	check_map_is_valid(t_map *map);

/* cu_utils.c */
void	free_2d_array(char **arr);

/* cu_debug.c */
void	print_texture_struct(t_texture *texture);
void	print_map_struct(t_map *map);
void	print_2d_arr(char **s, int arr_cnt);

#endif
