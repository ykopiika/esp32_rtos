#include "main.h"

static t_subcmd 	*create_sub_node(char *name, t_fnxptr func)
{
	t_subcmd *lst = NULL;

	lst = (t_subcmd *)malloc(sizeof(t_subcmd));
	if (!lst)
		return NULL;
	lst->name = strdup(name);
	lst->func = func;
	lst->next = NULL;
	return lst;
}

static t_command	*create_cmd_node(char *name)
{
	t_command *lst = NULL;

	lst = (t_command *)malloc(sizeof(t_command));
	if (!lst)
		return NULL;
	lst->name = strdup(name);
	lst->next = NULL;
	lst->down = NULL;
	return lst;
}

t_command	*command_registration(char *name, char *sub, t_fnxptr *fx_arr)
{
	t_command	*lst = NULL;
	t_subcmd 	*sublst = NULL;
	char		**str = NULL;
	int			size = 0;

	lst = create_cmd_node(name);
	if (!lst)
		return NULL;
	str = ft_split_count(sub, ' ', &size);
	if (!str)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	for (int i = 0; i < size; ++i)
	{
		if (i == 0)
		{
			if (!(sublst = create_sub_node(str[i], fx_arr[i])))
				err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
			lst->down = sublst;
		}
		else
		{
			if (!(sublst->next = create_sub_node(str[i], fx_arr[i])))
				err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
			sublst = sublst->next;
		}
	}
	ft_free_array_strings(str);
	return lst;
}

