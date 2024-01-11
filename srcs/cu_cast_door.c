/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cu_cast_door.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeongsh <hyeongsh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 16:25:25 by hyeongsh          #+#    #+#             */
/*   Updated: 2024/01/11 21:21:55 by hyeongsh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void		calc_door_dist(t_info *info, t_wall *door);
static void		open_door(t_wall *door, t_door *cur);
static void		close_door(t_info *info);
static int		hit_door(t_info *info, t_wall *door);
static int		check_hit_door(t_dpos *side_dist, t_dpos *delta_dist, t_wall *door, t_info *info);
static void		find_door_part(t_info *info, t_wall *door);
static void		put_buf_door_line(t_info *info, t_wall *door, t_door *cur);
static int		check_door_order(t_info *info);
static double	*sort_door_arr(t_info *info, int door_num);
static t_door	*find_i_door(t_info *info, double dist);

void	cast_door(t_info *info)
{
	t_wall		door;
	t_door		*cur;
	int			i;
	int			door_num;
	double		*arr;

	door.a = -1;
	door_num = check_door_order(info);
	arr = sort_door_arr(info, door_num);
	i = -1;
	while (++i < door_num)
	{
		cur = find_i_door(info, arr[i]);
		printf("%d %d\n", cur->map.x, cur->map.y);
		info->map->map[cur->map.x][cur->map.y] = 'D';
		door.b = -1;
		while (++door.b < WIDTH)
		{
			calc_door_dist(info, &door);
			if (info->map->map[door.map.x][door.map.y] == 'D')
				open_door(&door, cur);
			else
				continue ;
			find_door_part(info, &door);
			put_buf_door_line(info, &door, cur);
		}
		info->map->map[cur->map.x][cur->map.y] = '0';
	}
	free(arr);
	close_door(info);
}

static double	*sort_door_arr(t_info *info, int door_num)
{
	double	*arr;
	double	tmp;
	t_door	*cur;
	int		i;
	int		j;

	arr = (double *)ft_calloc(door_num, sizeof(double *));
	cur = info->head;
	i = 0;
	while (i < door_num)
	{
		arr[i++] = cur->dist;
		cur = cur->next;
	}
	i = -1;
	while (++i < door_num)
	{
		j = -1;
		while (++j < door_num - 1)
		{
			if (arr[j] < arr[j + 1])
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
	return (arr);
}

static t_door	*find_i_door(t_info *info, double dist)
{
	t_door	*cur;

	cur = info->head;
	while (cur != NULL)
	{
		if (cur->dist == dist)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

static int	check_door_order(t_info *info)
{
	t_door	*cur;
	int		i;

	cur = info->head;
	i = 0;
	while (cur != NULL)
	{
		cur->dist = pow(info->pos.x - cur->map.x, 2)
			+ pow(info->pos.y - cur->map.y, 2);
		cur = cur->next;
		i++;
	}
	return (i);
}

static void	open_door(t_wall *door, t_door *cur)
{
	if (cur->flag == 0)
	{
		if (door->perp_wall_dist <= 2
			&& cur->open < 800 / door->perp_wall_dist)
			cur->open += 25 / door->perp_wall_dist;
		if (door->perp_wall_dist <= 2
			&& cur->open >= 800 / door->perp_wall_dist)
			cur->open = 800 / door->perp_wall_dist;
		if (door->perp_wall_dist > 2 && cur->open > 0)
			cur->open -= 25 / door->perp_wall_dist;
		if (cur->open < 0)
			cur->open = 0;
		cur->flag = 1;
	}
}

static void	close_door(t_info *info)
{
	t_door	*cur;

	cur = info->head;
	while (cur != NULL)
	{
		if (cur->flag == 0)
			cur->open = 0;
		cur->flag = 0;
		cur = cur->next;
	}
}

static void	calc_door_dist(t_info *info, t_wall *door)
{
	double	camera_x;

	camera_x = 2 * door->b / (double) WIDTH - 1;
	door->ray_dir.x = info->dir.x + info->plane.x * camera_x;
	door->ray_dir.y = info->dir.y + info->plane.y * camera_x;
	door->map.x = (int)info->pos.x;
	door->map.y = (int)info->pos.y;
	door->side = hit_door(info, door);
	if (door->side == 0)
		door->perp_wall_dist = (door->map.x - info->pos.x
				+ (1 - door->step.x) / 2) / door->ray_dir.x;
	else
		door->perp_wall_dist = (door->map.y - info->pos.y
				+ (1 - door->step.y) / 2) / door->ray_dir.y;
}

static int	hit_door(t_info *info, t_wall *door)
{
	t_dpos	delta_dist;
	t_dpos	side_dist;

	delta_dist.x = fabs(1 / door->ray_dir.x);
	delta_dist.y = fabs(1 / door->ray_dir.y);
	door->step.x = 2 * (door->ray_dir.x >= 0) - 1;
	door->step.y = 2 * (door->ray_dir.y >= 0) - 1;
	if (door->ray_dir.x < 0)
		side_dist.x = (info->pos.x - door->map.x) * delta_dist.x;
	else
		side_dist.x = (door->map.x + 1.0 - info->pos.x) * delta_dist.x;
	if (door->ray_dir.y < 0)
		side_dist.y = (info->pos.y - door->map.y) * delta_dist.y;
	else
		side_dist.y = (door->map.y + 1.0 - info->pos.y) * delta_dist.y;
	return (check_hit_door(&side_dist, &delta_dist, door, info));
}

static int	check_hit_door(t_dpos *side_dist, t_dpos *delta_dist, t_wall *door, t_info *info)
{
	int	side;

	while (42)
	{
		if (side_dist->x < side_dist->y)
		{
			side_dist->x += delta_dist->x;
			door->map.x += door->step.x;
			side = 0;
		}
		else
		{
			side_dist->y += delta_dist->y;
			door->map.y += door->step.y;
			side = 1;
		}
		if (info->map->map[door->map.x][door->map.y] == '1'
			|| info->map->map[door->map.x][door->map.y] == 'D')
			return (side);
	}
}

static void	find_door_part(t_info *info, t_wall *door)
{
	door->line_height = (int)(HEIGHT / door->perp_wall_dist);
	door->draw_start = -door->line_height / 2 + HEIGHT / 2;
	if (door->draw_start < 0)
		door->draw_start = 0;
	door->draw_end = door->line_height / 2 + HEIGHT / 2;
	if (door->draw_end >= HEIGHT)
		door->draw_end = HEIGHT - 1;
	if (door->side == 0)
		door->ratio = info->pos.y + door->perp_wall_dist * door->ray_dir.y;
	else
		door->ratio = info->pos.x + door->perp_wall_dist * door->ray_dir.x;
	door->ratio -= floor(door->ratio);
}

static void	put_buf_door_line(t_info *info, t_wall *door, t_door *cur)
{
	double			delta_step;
	double			tex_pos;

	door->tex.x = (int)(door->ratio * (double) TEX_WIDTH);
	if (door->side == 0 && door->ray_dir.x > 0)
		door->tex.x = TEX_WIDTH - door->tex.x - 1;
	if (door->side == 1 && door->ray_dir.y < 0)
		door->tex.x = TEX_WIDTH - door->tex.x - 1;
	delta_step = 1.0 * TEX_HEIGHT / door->line_height;
	tex_pos = (door->draw_start - HEIGHT / 2
			+ door->line_height / 2) * delta_step;
	door->a = door->draw_start - 1;
	while (++door->a < door->draw_end)
	{
		door->tex.y = (int) tex_pos & (TEX_HEIGHT - 1);
		if (cur != NULL && door->a - cur->open > 0)
			info->buf[door->a - cur->open][door->b] = info->texture[4]
			[TEX_HEIGHT * door->tex.y + door->tex.x];
		tex_pos += delta_step;
	}
}
