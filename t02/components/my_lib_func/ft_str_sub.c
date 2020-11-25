#include "libft.h"

static void	*ft_memalloc(size_t size)
{
	void	*p;

	p = malloc(sizeof(void) * size);
	if (p == 0)
		return (NULL);
	memset(p, 0, size);
	return (p);
}

static char	*ft_strnew(size_t size)
{
	char	*s;

	s = ft_memalloc(sizeof(char) * (size + 1));
	if (!s)
		return (NULL);
	return (s);
}

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s || !*s)
		return (NULL);
	sub = ft_strnew(len);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[(size_t)start + i] != '\0')
	{
		sub[i] = s[(size_t)start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

