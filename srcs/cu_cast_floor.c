/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cu_cast_floor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeongsh <hyeongsh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 20:04:54 by hyeongsh          #+#    #+#             */
/*   Updated: 2024/01/05 13:57:13 by hyeongsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	cast_floor(t_info *info)
{
	t_dpos	floor_step;
	t_dpos	floor;
	t_ipos	tex;
	int		a;
	int		b;

	a = -1;
	while (++a < HEIGHT / 2)
	{
		set_floor_loop(info, a, &floor_step, &floor);
		b = -1;
		while (++b < WIDTH)
		{
			find_texture_index(&tex, &floor_step, &floor);
			enter_color_in_buf(info, &tex, a, b);
		}
	}
}

void	set_floor_loop(t_info *info, int a, t_dpos *floor_step, t_dpos *floor)
{
	t_dpos	ray_dir0;
	t_dpos	ray_dir1;
	double	pos_z;
	double	row_dist;
	int		p;

	ray_dir0.x = info->dir.x - info->plane.x;
	ray_dir0.y = info->dir.y - info->plane.y;
	ray_dir1.x = info->dir.x + info->plane.x;
	ray_dir1.y = info->dir.y + info->plane.y;
	p = a - HEIGHT / 2;
	pos_z = HEIGHT / 2;
	row_dist = pos_z / p;
	floor_step->x = row_dist * (ray_dir1.x - ray_dir0.x) / WIDTH;
	floor_step->y = row_dist * (ray_dir1.y - ray_dir0.y) / WIDTH;
	floor->x = info->pos.x + row_dist * ray_dir0.x;
	floor->y = info->pos.y + row_dist * ray_dir0.y;
}

void	find_texture_index(t_ipos *tex, t_dpos *floor_step, t_dpos *floor)
{
	t_dpos	cell;

	cell.x = (int)(floor->x);
	cell.y = (int)(floor->y);
	floor->x += floor_step->x;
	floor->y += floor_step->y;
	tex->x = (int)(TEX_WIDTH * (floor->x - cell.x)) & (TEX_WIDTH - 1);
	tex->y = (int)(TEX_HEIGHT * (floor->y - cell.y)) & (TEX_HEIGHT - 1);
}

void	enter_color_in_buf(t_info *info, t_ipos *tex, int a, int b)
{
	int		ceiling_color;
	int		floor_color;
	t_ipos	texture;

	texture.x = 3;
	texture.y = 6;
	ceiling_color = info->texture[texture.x][TEX_WIDTH * tex->y + tex->x];
	ceiling_color = (ceiling_color >> 1) & 8355711;
	info->buf[HEIGHT - a - 1][b] = ceiling_color;
	floor_color = info->texture[texture.y][TEX_WIDTH * tex->y + tex->x];
	floor_color = (floor_color >> 1) & 8355711;
	info->buf[a][b] = floor_color;
}
