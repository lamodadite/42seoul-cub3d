#include "cub.h"

void	print_texture_struct(t_texture *texture)
{
	printf("====texture=====\n");
	printf("cnt -> %d\n", texture->cnt);
	printf("no_path -> %s\n", texture->no_path);
	printf("so_path -> %s\n", texture->so_path);
	printf("we_path -> %s\n", texture->we_path);
	printf("ea_path -> %s\n", texture->ea_path);
	printf("c_color -> %s\n", texture->c_color);
	printf("f_color -> %s\n", texture->f_color);
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
