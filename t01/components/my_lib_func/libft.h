#ifndef SPLIT_COUNT_H
#define SPLIT_COUNT_H

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len);
char	**ft_split_count(char const *s, char c, int *nb);
int		ft_array_len(void **arr);
void	*ft_memdup(const void* src, size_t size);

#endif
