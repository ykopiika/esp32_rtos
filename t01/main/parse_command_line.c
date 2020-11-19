#include "main.h"

void parse_command_line(t_buffer *line, t_command *cmds)
{
	char	**str = NULL;
	int		len = 0;
	int 	result = ST_OK;

	if (!line || !cmds)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	str = ft_split_count((char *)line->data, ' ', &len);
	printf("\t\t\t\t=============> LEN:  %d\n", len);
	result = search_command(str, len, cmds);
	if (result == ST_NOT_FOUND && len == 0)
		result = ST_OK;
	ft_free_array_strings(str);
	if (result == ST_NOT_VALID)
		printf(T_TRK"\t\t\t\t********** cmd NOT_VALID *********\n"R);
	if (result == ST_NOT_FOUND)
		printf(T_TRK"\t\t\t\t********** cmd not found *********\n"R);
	if (result == ST_SUCCESS)
		printf(T_TRK"\t\t\t\t********** SUCCESS *********\n"R);
}

