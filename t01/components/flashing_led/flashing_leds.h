#ifndef PWM_AND_DAC_MAIN_H
#define PWM_AND_DAC_MAIN_H

#include <driver/gpio.h>
#include <driver/dac.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DELAY                   15
#define LEVEL                   120
#define LEDC_TEST_DUTY          255
#define LEDC_TEST_FADE_TIME     1200

#if LEVEL != 120
#warning RECOMMENDED VALUE FOR LEVEL BETWEEN 120-180
#endif
#if LEDC_TEST_DUTY != 255
#warning "RECOMMENDED VALUE FOR LEDC_TEST_DUTY 255"
#endif

void data_init(ledc_timer_config_t *ledc_timer,
			   ledc_channel_config_t *ledc_channel,
			   int led);
void pwm_flashing(ledc_channel_config_t *ledc_channel, int frq);
void dac_flashing(void);

#endif
