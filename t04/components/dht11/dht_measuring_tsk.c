#include "dht_11.h"

void dht_measuring_tsk(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_dht dht;
	_Bool is_on = true;

	d->dht_queue = xQueueCreate(3, sizeof(t_dht));
	if(d->dht_queue == NULL)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);

	while (is_on)
	{
		dht = (t_dht){0,0, 0};
		get_value_dht11(&dht.tem, &dht.hum);
		timer_get_counter_value(TIMER_GROUP_1, TIMER_0, &dht.time);
		xQueueSend(d->dht_queue, (void*)&dht, (TickType_t)0);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}

int print_all_lists(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_dht_lst	*lst = d->head;
	uint64_t	time = 0;
	int			sec = 0;
	int			min = 0;
	char		str[80];

	if(!lst)
		return ST_CMD_FAILED;
	while (lst)
	{
		bzero(&str, sizeof(str));
		timer_get_counter_value(TIMER_GROUP_1, TIMER_0, &time);
		sec = (int)((time - lst->dht.time) / TIMER_SCALE);
		min = (int)(sec / 60);
		sec = (int)(sec % 60);
		if (lst->next && (lst->next->dht.hum == lst->dht.hum) && (lst->next->dht.tem == lst->dht.tem))
			sprintf(str, "Temperature %dC Humidity %d%%\r\n$ ",
		   			lst->dht.tem, lst->dht.hum);
		else
			sprintf(str, "Temperature %dC Humidity %d%% | %d min %d sec ago\r\n$ ",
					lst->dht.tem, lst->dht.hum, min, sec);
		uart_write_bytes(UART_NUM_1, str, sizeof(str));
		lst = lst->next;
	}
	return ST_SUCCESS;
}

void dht_write_to_lists_tsk(void *ptr)
{
	_Bool		is_on = true;
	t_data		*d = (t_data*)ptr;
	t_dht		dht;
	int			count = 0;

	while (is_on)
	{
		dht = (t_dht){0,0, 0};
		if (d->dht_queue && xQueueReceive(d->dht_queue, (void*)&dht, portMAX_DELAY))
		{
			if (count < MAX_DHT_LST)
			{
				d->head = add_dht_lst(d->head);
				count++;
			}
			else
				d->head = last_lst_to_first(d->head);
			d->head->dht = dht;
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

