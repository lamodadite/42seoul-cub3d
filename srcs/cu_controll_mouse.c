#include "cub.h"

int	controll_mouse(int x, int y, t_info *info)
{
	(void) y;
	int	dir;
	double tmp_rot_speed;

	tmp_rot_speed = info->rot_speed;
	dir = 0;
	if (x - info->pre_mouse_pos_x > 0)
		dir = K_AR_R;
	else if (x - info->pre_mouse_pos_x < 0)
		dir = K_AR_L;
	if (dir != 0)
		info->rot_speed /= 2;
	press_left_rotate_key(dir, info);
	press_right_rotate_key(dir, info);
	info->pre_mouse_pos_x = x;
	info->rot_speed = tmp_rot_speed;
	return (0);
}
