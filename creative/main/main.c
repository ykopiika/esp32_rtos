#include "main.h"

static t_command	*init_commands(void)
{
	t_command		*lst = NULL;

	lst = command_registration((t_fnxptr[]) {led_on_off, led_on_off, led_pulse},
							lst, "led", "on off pulse");
	lst = command_registration((t_fnxptr[]) {print_all_lists},
							lst, "dht", "log");
	lst = command_registration((t_fnxptr[]) {set_time, set_alarm},
							lst, "set", "time alarm");
	lst = command_registration((t_fnxptr[]) {sound_on_off, sound_on_off},
							lst,"sound","on off");
	return lst;
}

static void init_components(t_data *data)
{
	init_i2s_speaker();
	init_timer(data);
	init_uart(data);
	dht11_init();
	init_oled(&data->oled);
	data->cmds = init_commands();
}

void app_main(void)
{
	t_data			*data;

	if (!(data = (t_data*)malloc(sizeof(t_data))))
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
	bzero(data, sizeof(*data));

	init_components(data);
	xTaskCreate(dht_measuring_tsk, "dht_measuring",
				2048, (void *)data, 2, NULL);
    xTaskCreate(dht_write_to_lists_tsk, "dht_write_to_lists",
                2048, (void *)data, 10, NULL);
    xTaskCreate(read_from_uart, "read_uart",
                4096, (void *)data, 3, NULL);
	xTaskCreate(time_to_oled, "time_to_oled",
				2048, (void *) data, 1, &data->oled_time_task);
//	xTaskCreate(make_noise, "make_noise",
//			 	1024, NULL, 4, NULL);
}

