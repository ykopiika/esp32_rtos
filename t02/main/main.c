#include "main.h"

static t_command	*init_commands(void)
{
	t_command		*lst = NULL;
	t_fnxptr		fx_arr[] = {led_on_off, led_on_off, led_pulse};

	lst = command_registration("led", "on off pulse", fx_arr);
	if (!lst)
		return NULL;
	return lst;
}

static void init_components(uart_config_t *uart_config, t_command **lst)
{
	timer_config_t config = {
			.alarm_en = TIMER_ALARM_DIS,
			.counter_en = TIMER_START,
			.intr_type = TIMER_INTR_NONE,
			.counter_dir = TIMER_COUNT_UP,
			.auto_reload = TIMER_AUTORELOAD_DIS,
			.divider = TIMER_DIVIDER,
	}; // default clock source is APB
	timer_init(TIMER_GROUP_0, timer_idx, &config);

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
    xTaskCreate(read_from_uart, "read_uart",
                4096, (void *)lst, 5, NULL);
}