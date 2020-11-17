#include "main.h"

void	led_on(void *ptr)
{
	(void)ptr;
	printf(T_TRK"led_on called\n"R);
}

void	led_off(void *ptr)
{
	(void)ptr;
	printf(T_TRK"led_off called\n"R);
}

void	led_pulse(void *ptr)
{
	(void)ptr;
	printf(T_TRK"led_pulse called\n"R);
}

void parse_command_line(t_buffer *line, t_command *cmds)
{
	char **str = NULL;
	int len = 0;

	if (!line || !cmds)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	str = ft_split_count((char *)line->data, ' ', &len);
	printf("\t\t\t\t=============> LEN:  %d\n", len);
	for (int i = 0; i < len; ++i)
	{
		printf("\t\t\t\t%s|\n", str[i]);
	}
	if (str != NULL)
	{
		for (int i = 0; i < len; ++i)
		{
			if (str[i] != NULL)
				free(str[i]);
		}
		if (str != NULL)
			free(str);
	}
//	char *arr[] = {"str","","array"};
//	int size;
//	int size = ft_array_len((void **) arr);
//	printf("size **arr = %d\n", size);
	printf("size cmds->func = %zu %zu\n", sizeof(cmds->func), sizeof(*cmds->func));
	cmds->func[0](line);
	cmds->func[1](line);
	cmds->func[2](line);
	printf(T_RED"\nparse line\n"R);
}

