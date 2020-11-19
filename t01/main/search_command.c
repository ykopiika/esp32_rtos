#include "main.h"

static int	search_sub_command(t_subcmd *subcmd, char **str, int len)
{
	int	result = ST_NOT_FOUND;
	t_param	param = (t_param){str, len};

	while (subcmd)
	{
		if (str[1] && (strcmp(str[1], subcmd->name) == 0))
			result = subcmd->func((void *)&param);
		subcmd = subcmd->next;
	}
	return result;
}

int			search_command(char **str, int len, t_command *cmds)
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

