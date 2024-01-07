#include "cub.h"

void	print_texture_struct(t_texture texture)
{
	printf("====texture=====\n");
	printf("cnt -> %d\n", texture.cnt);
	printf("no_path -> %s\n", texture.no_path);
	printf("so_path -> %s\n", texture.so_path);
	printf("we_path -> %s\n", texture.we_path);
	printf("ea_path -> %s\n", texture.ea_path);
	printf("c_color -> ");
	for (int i = 0; i < 3; i++)
		printf("%d ", texture.c_color[i]);
	printf("f_color -> ");
	for (int i = 0; i < 3; i++)
		printf("%d ", texture.f_color[i]);
	printf("\n");
}

void	print_map_struct(t_map *map)
{
	printf("width -> %d\n", map->width);
	printf("height -> %d\n", map->height);
	print_2d_arr(map->map, map->height);
}

void	print_2d_arr(char **s, int arr_cnt)
{
	for (int i = 0; i < arr_cnt; i++)
	{
		printf("%s\n", s[i]);
	}
}

void	print_2d_arr_d(char **s, int arr_cnt)
{
	for (int i = 0; i < arr_cnt; i++)
	{
		for (int j = 0; j < (int)ft_strlen(s[i]); j++)
		{
			printf("%d ", s[i][j]);
		}
		printf("\n");
	}
}