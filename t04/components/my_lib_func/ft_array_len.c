#include "libft.h"

int	ft_array_len(void **arr)
{
	int len = 0;

	for (void **ptr = arr; ptr && (*ptr != NULL); ptr++)
		len++;
	return len;
}
