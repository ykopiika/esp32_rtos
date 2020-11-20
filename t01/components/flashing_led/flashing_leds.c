#include "flashing_leds.h"

void data_init(ledc_timer_config_t *ledc_timer,
               ledc_channel_config_t *ledc_channel,
               int led)
{
	(void )led;
    *ledc_timer = (ledc_timer_config_t){
            .duty_resolution = LEDC_TIMER_8_BIT,
            .freq_hz = 1000,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(ledc_timer));
    *ledc_channel = (ledc_channel_config_t){
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = GPIO_NUM_27,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
    };
    ESP_ERROR_CHECK(ledc_channel_config(ledc_channel));
    ESP_ERROR_CHECK(ledc_fade_func_install(0));
}

void dac_flashing(void)
{
    for (uint8_t i = LEVEL; i < 255; i++)
    {
        ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_2, i));
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
    vTaskDelay(30 / portTICK_PERIOD_MS);
    for (uint8_t i = 255; i > LEVEL; i--)
    {
        ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_2, i));
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
}

void pwm_flashing(ledc_channel_config_t *ledc_channel, int frq)
{
    ESP_ERROR_CHECK(ledc_set_fade_with_time(ledc_channel->speed_mode,
                       ledc_channel->channel, LEDC_TEST_DUTY, frq));
    ESP_ERROR_CHECK(ledc_fade_start(ledc_channel->speed_mode,
                    ledc_channel->channel, LEDC_FADE_WAIT_DONE));
    vTaskDelay(30 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(ledc_set_fade_with_time(ledc_channel->speed_mode,
                            ledc_channel->channel, 0, frq));
    ESP_ERROR_CHECK(ledc_fade_start(ledc_channel->speed_mode,
                    ledc_channel->channel, LEDC_FADE_WAIT_DONE));
}

