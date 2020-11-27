#include "dht_11.h"

t_dht_lst *add_dht_lst(t_dht_lst *head)
{
	t_dht_lst *lst = NULL;

	if (!(lst = (t_dht_lst*)malloc(sizeof(t_dht_lst))))
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	if (!head)
	{
		lst->dht = (t_dht){0,0,0};
		lst->next = NULL;
		return lst;
	}
	lst->dht = (t_dht){0,0,0};
	lst->next = head;
	return lst;
}

t_dht_lst *last_lst_to_first(t_dht_lst *head)
{
	t_dht_lst *lst = NULL;
	t_dht_lst *pre_lst = NULL;

	if (!head)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	lst = head;
	while (lst->next && lst->next->next)
		lst = lst->next;
	pre_lst = lst;
	lst = lst->next;
	lst->dht = (t_dht){0,0,0};
	lst->next = head;
	pre_lst->next = NULL;
	return lst;
}
