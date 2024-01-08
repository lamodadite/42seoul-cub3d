/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:18:03 by yohlee            #+#    #+#             */
/*   Updated: 2024/01/08 10:27:21 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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

int	main(int ac, char **av)
{
	t_map	map;
	t_info	info;

	if (ac != 2)
	{
		print_error_and_exit("need one map file\n");
		return (1);
	}
	check_map_name(av[1]);
	load_file(av[1], &map);
	init_info(&info, &map);
	load_texture(&info);
	info.win = mlx_new_window(info.mlx, WIDTH, HEIGHT, "mlx");
	info.img.img = mlx_new_image(info.mlx, WIDTH, HEIGHT);
	info.img.data = (int *)mlx_get_data_addr(info.img.img, &info.img.bpp, &info.img.size_l, &info.img.endian);
	//cast_floor(&info);
	tmp_cast_floor(&info);
	cast_wall(&info);
	draw(&info);
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &press_key, &info);
	mlx_loop(info.mlx);
}
