#include "main.h"

static t_command	*init_commands(void)
{
	t_command		*lst = NULL;
	t_command		*ptr = NULL;
	t_fnxptr		fx_arr[] = {led_on_off, led_on_off, led_pulse};

	lst = command_registration("led", "on off pulse", fx_arr);
	if (!lst)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);

	fx_arr[0] = print_all_lists;
	fx_arr[1] = NULL;
	fx_arr[2] = NULL;
	ptr = lst;
	ptr->next = command_registration("dht", "log", fx_arr);
	if (!ptr->next)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	ptr = ptr->next;

	fx_arr[0] = set_time;
	fx_arr[1] = NULL;
	fx_arr[2] = NULL;
	ptr->next = command_registration("set", "time", fx_arr);
	if (!ptr->next)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
//	ptr = ptr->next;
	return lst;
}

static void init_components(t_data *data)
{
	init_timer(data);
	init_uart(data);
	dht11_init();
	init_oled(&data->oled);
	data->cmds = init_commands();
	str_to_oled_6x8(&data->oled, "HELLO__HELLo", 3);
	clear_pixels(&data->oled);
	str_to_oled_6x8(&data->oled, "WORLD", 3);
}

void app_main(void)
{
	t_data			*data;

	if (!(data = (t_data*)malloc(sizeof(t_data))))
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	bzero(data, sizeof(*data));

	init_components(data);
	xTaskCreate(dht_measuring_tsk, "dht_measuring",
				2048, (void *)data, 3, NULL);
    xTaskCreate(dht_write_to_lists_tsk, "dht_write_to_lists",
                2048, (void *)data, 10, NULL);
    xTaskCreate(read_from_uart, "read_uart",
                4096, (void *)data, 5, NULL);
	xTaskCreate(time_to_oled, "time_to_oled",
				4096, (void *) data, 1, &data->oled_time_task);
}