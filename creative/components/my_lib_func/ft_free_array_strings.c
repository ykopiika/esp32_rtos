#include "libft.h"

void	ft_free_array_strings(char **str)
{
	if (str != NULL)
	{
		for (int i = 0; str[i] != NULL; ++i)
			free(str[i]);
		if (str != NULL)
			free(str);
	}
}

