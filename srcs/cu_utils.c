#include "cub.h"

void	free_2d_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != 0)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	get_str_2d_len(char **s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != NULL)
		i++;
	return (i);
}
