#include "main.h"

static void init_timer(void)
{
	timer_config_t config_1 = {
			.alarm_en = TIMER_ALARM_DIS,
			.counter_en = TIMER_START,
			.intr_type = TIMER_INTR_LEVEL,
			.counter_dir = TIMER_COUNT_UP,
			.auto_reload = TIMER_AUTORELOAD_DIS,
			.divider = TIMER_DIVIDER,
	};
	ESP_ERROR_CHECK(timer_init(TIMER_GROUP_0, TIMER_0, &config_1));
}

static t_command	*init_commands(void)
{
	t_command		*lst = NULL;
	t_command		*ptr = NULL;
	t_fnxptr		fx_arr[] = {led_on_off, led_on_off, led_pulse};

	lst = command_registration("led", "on off pulse", fx_arr);
	if (!lst)
		return NULL;
	fx_arr[0] = print_all_lists;
	fx_arr[1] = NULL;
	fx_arr[2] = NULL;
	ptr = lst;
	ptr->next = command_registration("dht", "log", fx_arr);
//	ptr = ptr->next;
	return lst;
}

static void init_components(uart_config_t *uart_config, t_command **lst)
{
	init_timer();
	init_uart(uart_config);
	dht11_init();
	*lst = init_commands();
	if (!*lst)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
}

void app_main(void)
{
	uart_config_t   uart_config;
	t_command		*lst = NULL;

	init_components(&uart_config, &lst);

	xTaskCreate(dht_measuring_tsk, "dht_measuring",
				2048, NULL, 10, NULL);
    xTaskCreate(dht_write_to_lists_tsk, "dht_write_to_lists",
                2048, NULL, 10, NULL);
    xTaskCreate(read_from_uart, "read_uart",
                4096, (void *)lst, 5, NULL);
}