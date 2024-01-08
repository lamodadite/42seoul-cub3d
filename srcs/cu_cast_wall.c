/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cu_cast_wall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 11:16:37 by hyeongsh          #+#    #+#             */
/*   Updated: 2024/01/08 18:20:01 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	cast_wall(t_info *info)
{
	t_wall	wall;
	//double	dist_wall;
	//double	dist_player;
	//double	current_dist;
	//double	weight;
	//t_dpos	current_floor;
	//t_ipos	floor_tex;
	//int		checker_board_pattern;
	//int		floor_texture;

	wall.a = -1;
	wall.b = -1;
	while (++wall.b < WIDTH)
	{
		calc_wall_dist(info, &wall);
		find_draw_part(info, &wall);
		put_buf_wall_line(info, &wall);
		find_floor_pos(&wall);
		//dist_wall = wall.perp_wall_dist;
		//dist_player = 0.0;
		//wall.a = wall.draw_end;
		//while (++wall.a < HEIGHT)
		//{
		//	current_dist = HEIGHT / (2.0 * wall.a - HEIGHT);
		//	weight = (current_dist - dist_player) / (dist_wall - dist_player);
		//	current_floor.x = weight * wall.floor.x + (1.0 - weight) * info->pos.x;
		//	current_floor.y = weight * wall.floor.y + (1.0 - weight) * info->pos.y;
		//	floor_tex.x = (int)(current_floor.x * TEX_WIDTH) % TEX_WIDTH;
		//	floor_tex.y = (int)(current_floor.y * TEX_HEIGHT) % TEX_HEIGHT;
		//	checker_board_pattern = ((int)(current_floor.x) + (int)(current_floor.y)) % 2;
		//	if (checker_board_pattern == 0)
		//		floor_texture = 3;
		//	else
		//		floor_texture = 4;
		//	info->buf[wall.a][wall.b] = (info->texture[floor_texture][TEX_WIDTH * floor_tex.y + floor_tex.x] >> 1) & 8355711;
		//	info->buf[HEIGHT - wall.a][wall.b] = info->texture[6][TEX_WIDTH * floor_tex.y + floor_tex.x];
		//}
	}
}

void	calc_wall_dist(t_info *info, t_wall *wall)
{
	double	camera_x;

	camera_x = 2 * wall->b / (double) WIDTH - 1;
	wall->ray_dir.x = info->dir.x + info->plane.x * camera_x;
	wall->ray_dir.y = info->dir.y + info->plane.y * camera_x;
	wall->map.x = (int)info->pos.x;
	wall->map.y = (int)info->pos.y;
	wall->side = hit_wall(info, wall);
	if (wall->side == 0)
		wall->perp_wall_dist = (wall->map.x - info->pos.x
				+ (1 - wall->step.x) / 2) / wall->ray_dir.x;
	else
		wall->perp_wall_dist = (wall->map.y - info->pos.y
				+ (1 - wall->step.y) / 2) / wall->ray_dir.y;
}

int	hit_wall(t_info *info, t_wall *wall)
{
	t_dpos	delta_dist;
	t_dpos	side_dist;

	delta_dist.x = fabs(1 / wall->ray_dir.x);
	delta_dist.y = fabs(1 / wall->ray_dir.y);
	wall->step.x = 2 * (wall->ray_dir.x >= 0) - 1;
	wall->step.y = 2 * (wall->ray_dir.y >= 0) - 1;
	if (wall->ray_dir.x < 0)
		side_dist.x = (info->pos.x - wall->map.x) * delta_dist.x;
	else
		side_dist.x = (wall->map.x + 1.0 - info->pos.x) * delta_dist.x;
	if (wall->ray_dir.y < 0)
		side_dist.y = (info->pos.y - wall->map.y) * delta_dist.y;
	else
		side_dist.y = (wall->map.y + 1.0 - info->pos.y) * delta_dist.y;
	return (check_hit_wall(&side_dist, &delta_dist, wall, info));
}

int	check_hit_wall(t_dpos *side_dist, t_dpos *delta_dist, t_wall *wall, t_info *info)
{
	int	side;

	while (42)
	{
		if (side_dist->x < side_dist->y)
		{
			side_dist->x += delta_dist->x;
			wall->map.x += wall->step.x;
			side = 0;
		}
		else
		{
			side_dist->y += delta_dist->y;
			wall->map.y += wall->step.y;
			side = 1;
		}
		//printf("map.x -> %d, map.y -> %d\n", wall->map.x, wall->map.y);
		if (info->map->map[wall->map.x][wall->map.y] == '1')
			return (side);
	}
}

void	find_draw_part(t_info *info, t_wall *wall)
{
	wall->line_height = (int)(HEIGHT / wall->perp_wall_dist);
	wall->draw_start = -wall->line_height / 2 + HEIGHT / 2;
	if (wall->draw_start < 0)
		wall->draw_start = 0;
	wall->draw_end = wall->line_height / 2 + HEIGHT / 2;
	if (wall->draw_end >= HEIGHT)
		wall->draw_end = HEIGHT - 1;
	if (wall->side == 0)
		wall->ratio = info->pos.y + wall->perp_wall_dist * wall->ray_dir.y;
	else
		wall->ratio = info->pos.x + wall->perp_wall_dist * wall->ray_dir.x;
	wall->ratio -= floor(wall->ratio);
}

void	put_buf_wall_line(t_info *info, t_wall *wall)
{
	double	delta_step;
	double	tex_pos;
	int		color;

	wall->tex.x = (int)(wall->ratio * (double) TEX_WIDTH);
	if (wall->side == 0 && wall->ray_dir.x > 0)
		wall->tex.x = TEX_WIDTH - wall->tex.x - 1;
	if (wall->side == 1 && wall->ray_dir.y < 0)
		wall->tex.x = TEX_WIDTH - wall->tex.x - 1;
	delta_step = 1.0 * TEX_HEIGHT / wall->line_height;
	tex_pos = (wall->draw_start - HEIGHT / 2
			+ wall->line_height / 2) * delta_step;
	wall->a = wall->draw_start - 1;
	while (++wall->a < wall->draw_end)
	{
		wall->tex.y = (int) tex_pos & (TEX_HEIGHT - 1);
		if (wall->side == 0)
		{
			if (wall->ray_dir.x >= 0)
				color = info->texture[0][TEX_HEIGHT * wall->tex.y + wall->tex.x];
			else
				color = info->texture[1][TEX_HEIGHT * wall->tex.y + wall->tex.x];
		}
		else if (wall->side == 1)
		{
			if (wall->ray_dir.y >= 0)
				color = info->texture[2][TEX_HEIGHT * wall->tex.y + wall->tex.x];
			else
				color = info->texture[3][TEX_HEIGHT * wall->tex.y + wall->tex.x];
		}
		info->buf[wall->a][wall->b] = color;
		tex_pos += delta_step;
	}
}

void	find_floor_pos(t_wall *wall)
{
	if (wall->side == 0 && wall->ray_dir.x > 0)
	{
		wall->floor.x = wall->map.x;
		wall->floor.y = wall->map.y + wall->ratio;
	}
	else if (wall->side == 0 && wall->ray_dir.x < 0)
	{
		wall->floor.x = wall->map.x + 1.0;
		wall->floor.y = wall->map.y + wall->ratio;
	}
	else if (wall->side == 1 && wall->ray_dir.y > 0)
	{
		wall->floor.x = wall->map.x + wall->ratio;
		wall->floor.y = wall->map.y;
	}
	else
	{
		wall->floor.x = wall->map.x + wall->ratio;
		wall->floor.y = wall->map.y + 1.0;
	}
}
