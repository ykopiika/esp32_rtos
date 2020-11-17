#include "libft.h"

void* ft_memdup(const void* src, size_t size)
{
	void *dst = malloc(size);

	if (dst != NULL)
		memcpy(dst, src, size);
	return dst;
}

