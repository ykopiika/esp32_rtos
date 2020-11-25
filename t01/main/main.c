#include "main.h"

static t_command	*init_commands(void)
{
	t_command		*lst = NULL;
//	t_command		*ptr = NULL;
	t_fnxptr		fx_arr[] = {led_on_off, led_on_off, led_pulse};

	lst = command_registration("led", "on off pulse", fx_arr);
	if (!lst)
		return NULL;
//	ptr = lst; //todo: 1
//	ptr->next = command_registration("led", "on off pulse", fx_arr);
	return lst;
}

void app_main(void)
{
	uart_config_t   uart_config;
	t_command		*lst = NULL;

	init_uart(&uart_config);
	lst = init_commands();
	if (!lst)
		err_print_exit(ERR_VAL_NULL, __FILE__, __func__, __LINE__);
    xTaskCreate(read_from_uart, "read_uart",
                4096, (void *)lst, 5, NULL);
}
//TODO: set protect for all funct