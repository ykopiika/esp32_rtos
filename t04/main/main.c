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
	dac_output_enable(DAC_CHANNEL_1);
	int i2s_num = 0;
	i2s_config_t i2s_config = {
			.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
			.sample_rate = 16000,
			.bits_per_sample = 16,
			.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//			.communication_format = I2S_COMM_FORMAT_STAND_MSB,
			.communication_format = I2S_COMM_FORMAT_STAND_I2S,
			.intr_alloc_flags = 0,
			.dma_buf_count = 2,
			.dma_buf_len = 1024,
			.use_apll = 1
	};
	i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
	i2s_set_pin(i2s_num, NULL);
	i2s_stop(0);

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
				2048, (void *)data, 3, NULL);
    xTaskCreate(dht_write_to_lists_tsk, "dht_write_to_lists",
                2048, (void *)data, 10, NULL);
    xTaskCreate(read_from_uart, "read_uart",
                4096, (void *)data, 5, NULL);
	xTaskCreate(time_to_oled, "time_to_oled",
				2048, (void *) data, 1, &data->oled_time_task);
}