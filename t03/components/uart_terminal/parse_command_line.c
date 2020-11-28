#include "uart_terminal.h"

static int	search_sub_command(t_data *d, t_subcmd *subcmd)
{
	t_cmd_tmp	*tmp = &d->tmp;
	int			result = ST_NOT_FOUND;

	if (!d || !subcmd)
		return ST_NOT_FOUND;
	while (subcmd)
	{
		if (tmp->str[1] && (strcmp(tmp->str[1], subcmd->name) == 0))
			result = subcmd->func((void *)d);
		subcmd = subcmd->next;
	}
	return result;
}

static int	search_command(t_data *d)
{
	t_command	*ptr = d->cmds;
	t_cmd_tmp	*tmp = &d->tmp;
	int			result = ST_NOT_FOUND;

	if (tmp->len < 2 || !tmp->str || !*tmp->str || !d || !d->cmds)
		return ST_NOT_FOUND;
	while (ptr)
	{
		if (tmp->str[0] && (strcmp(tmp->str[0], ptr->name) == 0))
			result = search_sub_command(d, ptr->down);
		ptr = ptr->next;
	}
	return result;
}

void		parse_command_line(t_buffer *line, t_data *d)
{
	t_cmd_tmp	*tmp = &d->tmp;
	int			result = ST_OK;

	if (!line || !d || !d->cmds)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	tmp->str = NULL;
	tmp->len = 0;
	tmp->str = ft_split_count((char *)line->data, ' ', &tmp->len);
	result = search_command(d);
	if (result == ST_NOT_FOUND && tmp->len == 0)
		result = ST_OK;
	ft_free_array_strings(tmp->str);
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
	if (result == ST_CMD_FAILED)
		printf(T_TRK"\t\t\t\t CMD_FAILED \n"R);
}

