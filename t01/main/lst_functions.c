#include "main.h"

t_lst	*add_new_lst(t_lst *head)
{
	t_lst *new = NULL;

	if (head && head->previous != NULL)
		return head;
	if (!(new = (t_lst*)malloc(sizeof(t_lst))))
		return head;
	bzero(new, sizeof(*new));
	if (!head)
		return new;
	new->next = head;
	new->previous = NULL;
	if (head->previous != NULL)
		new->previous = head->previous;
	head->previous = new;
	return new;
}

t_lst	*clean_last_lst(t_lst *head)
{
	while (head->next->previous != NULL)
		head = head->next;
	head->previous = NULL;
	head->next->previous = head;
	bzero(&head->line, sizeof(head->line));
}


