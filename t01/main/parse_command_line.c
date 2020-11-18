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
	char	**str = NULL;
	int		len = 0;
	int 	result = ST_OK;

	if (!line || !cmds)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	str = ft_split_count((char *)line->data, ' ', &len);
	printf("\t\t\t\t=============> LEN:  %d\n", len);
	if (len == 0)
		result = ST_NOT_FOUND;
//	if (result == ST_OK)
//		result = search_command(str);
//	run_comm

	for (int i = 0; i < len; ++i)
	{
		printf("\t\t\t\t%s|\n", str[i]);
	}
//	command_compare(str)

	ft_free_array_strings(str);
	if (result == ST_NOT_FOUND)
		printf(T_TRK"\t\t\t\t********** cmd not found *********\n"R);
}

