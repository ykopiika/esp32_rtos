#ifndef LIBFT_H
#define LIBFT_H

#include <stdlib.h>
#include <string.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len);
char	**ft_split_count(char const *s, char c, int *nb);
int		ft_array_len(void **arr);
void	ft_free_array_strings(char **str);

#endif
