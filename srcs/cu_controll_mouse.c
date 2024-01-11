#include "cub.h"

int	controll_mouse(int x, int y, t_info *info)
{
	(void) y;
	int	dir;

	dir = 0;
	if (x - info->pre_mouse_pos_x > 0)
		dir = K_AR_R;
	else if (x - info->pre_mouse_pos_x < 0)
		dir = K_AR_L;
	press_left_rotate_key(dir, info);
	press_right_rotate_key(dir, info);
	info->pre_mouse_pos_x = x;
	return (0);
}
