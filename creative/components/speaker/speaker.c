#include "speaker.h"

void make_bip(void)
{
    for (int i = 0; i < 50; ++i)
    {
        for (int i = 0; i < 100; i += SPKR_STEP)
        {
            ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_1, i));
            ets_delay_us(SPKR_DELAY);
        }
        ets_delay_us(SPKR_DELAY);
        for (int i = 100; i > 0; i -= SPKR_STEP)
        {
            ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_1, i));
            ets_delay_us(SPKR_DELAY);
        }
        ets_delay_us(10);
    }
}

void make_noise(void *ptr)
{
	uint8_t sound_arr[20];
	size_t i2s_bytes_write = 0;

	sound_arr[0] = 0;
	for (int i = 1; i < 10; ++i)
		sound_arr[i] += 10;
	for (int i = 10; i < 20; ++i)
		sound_arr[i] -= 10;
	while (true)
	{
		ESP_ERROR_CHECK(i2s_write(0, sound_arr, sizeof(sound_arr), &i2s_bytes_write, 200));
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

int sound_on_off(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_cmd_tmp	*tmp = &d->tmp;
	_Bool		is_on = false;

	if (tmp->len != 2)
		return ST_NOT_VALID;
	is_on = !strcmp("on", tmp->str[1]);
	if (is_on)
		i2s_start(0);
	else
		i2s_stop(0);
	return ST_SUCCESS;
}

void init_i2s_speaker(void)
{
    ESP_ERROR_CHECK(dac_output_enable(DAC_CHANNEL_1));
    ESP_ERROR_CHECK(gpio_set_direction(AMP_POWER_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(AMP_POWER_PIN, 1));
	static const int i2s_num = 0;
	static const i2s_config_t i2s_config = {
			.mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
			.sample_rate = 16000,
			.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
			.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
			.communication_format = I2S_COMM_FORMAT_I2S_LSB,
			.intr_alloc_flags = 0,
			.dma_buf_count = 4,
			.dma_buf_len = 1024,
			.use_apll = false
	};
	ESP_ERROR_CHECK(i2s_driver_install(i2s_num, &i2s_config, 0, NULL));
	ESP_ERROR_CHECK(i2s_set_pin(i2s_num, 0));
	ESP_ERROR_CHECK(i2s_stop(0));
}
