#include "cub.h"

int	main(int argc, char **argv)
{
	t_map	map;

	if (argc != 2)
	{
		print_error_and_exit("need one map file\n");
		return (1);
	}
	check_map_name(argv[1]);
	load_file(argv[1], &map);
	print_texture_struct(map.texture);
	print_map_struct(&map);
	return (0);
}
