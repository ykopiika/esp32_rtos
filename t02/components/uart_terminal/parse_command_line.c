#include "uart_terminal.h"

static int	search_sub_command(t_subcmd *subcmd, char **str, int len)
{
	int	result = ST_NOT_FOUND;
	t_param	param = (t_param){.str = str, .len = len};

	while (subcmd)
	{
		if (str[1] && (strcmp(str[1], subcmd->name) == 0))
			result = subcmd->func((void *)&param);
		subcmd = subcmd->next;
	}
	return result;
}

static int	search_command(char **str, int len, t_command *cmds)
{
	t_command	*ptr = cmds;
	int			result = ST_NOT_FOUND;

	if (len < 2 || !str || !*str || !cmds)
		return ST_NOT_FOUND;
	while (ptr)
	{
		if (str[0] && (strcmp(str[0], ptr->name) == 0))
			result = search_sub_command(ptr->down, str, len);
		ptr = ptr->next;
	}
	return result;
}

void		parse_command_line(t_buffer *line, t_command *cmds)
{
	char	**str = NULL;
	int		len = 0;
	int 	result = ST_OK;

	if (!line || !cmds)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	str = ft_split_count((char *)line->data, ' ', &len);
	result = search_command(str, len, cmds);
	if (result == ST_NOT_FOUND && len == 0)
		result = ST_OK;
	ft_free_array_strings(str);
	if (result == ST_NOT_VALID)
	{
		uart_write_bytes(UART_NUM_1, T_PNK"command not valid"R,
				   sizeof(T_PNK"command not valid"R));
		uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
	}
	if (result == ST_NOT_FOUND)
	{
		uart_write_bytes(UART_NUM_1, T_PNK"command not found"R,
						 sizeof(T_PNK"command not found"R));
		uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
	}
	if (result == ST_SUCCESS)
		printf(T_TRK"\t\t\t\t SUCCESS \n"R);
}

