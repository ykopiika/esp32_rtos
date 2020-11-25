#include "led_commands.h"

static TaskHandle_t	led1_pulse = NULL;
static TaskHandle_t	led2_pulse = NULL;
static TaskHandle_t	led3_pulse = NULL;

static void hello_led(void *param)
{
	t_pwm	*pwm = (t_pwm*)param;
	t_pwm	cur_pwm = *pwm;
	int		leds[3] = {GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33};

	ESP_ERROR_CHECK(gpio_set_direction(leds[cur_pwm.led - 1], GPIO_MODE_OUTPUT));
	while (1)
	{
		vTaskDelay(cur_pwm.frq/portTICK_PERIOD_MS);
		ESP_ERROR_CHECK(gpio_set_level(leds[cur_pwm.led - 1], 1));
		vTaskDelay(cur_pwm.frq/portTICK_PERIOD_MS);
		ESP_ERROR_CHECK(gpio_set_level(leds[cur_pwm.led - 1], 0));
	}
}

static void	check_is_run_pulse(int	nbr)
{
	if (nbr == 1 && (led1_pulse != NULL))
	{
		vTaskDelete(led1_pulse);
		led1_pulse = NULL;
	}
	if (nbr == 2 && (led2_pulse != NULL))
	{
		vTaskDelete(led2_pulse);
		led2_pulse = NULL;
	}
	if (nbr == 3 && (led3_pulse != NULL))
	{
		vTaskDelete(led3_pulse);
		led3_pulse = NULL;
	}
}

static void	make_pulse(t_pwm *pwm)
{
	check_is_run_pulse(pwm->led);
	if (pwm->led == 1)
		xTaskCreate(hello_led, "led", 2048, (void *)pwm, 5, &led1_pulse);
	if (pwm->led == 2)
		xTaskCreate(hello_led, "led", 2048, (void *)pwm, 5, &led2_pulse);
	if (pwm->led == 3)
		xTaskCreate(hello_led, "led", 2048, (void *)pwm, 5, &led3_pulse);
}

int	led_on_off(void *ptr)
{
	t_param *p = (t_param*)ptr;
	_Bool	is_on = false;
	int		nbr = 0;
	int		leds[3] = {GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33};

	if (p->len < 3 || p->len > 5)
		return ST_NOT_VALID;
	if (!strcmp(p->str[1], "on"))
		is_on = true;
	for (int i = 2; i < p->len; ++i)
	{
		nbr = atoi(p->str[i]);
		if ((nbr < 1) || (nbr > 3))
			return ST_NOT_VALID;
		check_is_run_pulse(nbr);
		ESP_ERROR_CHECK(gpio_set_direction(leds[nbr - 1], GPIO_MODE_OUTPUT));
		ESP_ERROR_CHECK(gpio_set_level(leds[nbr - 1], is_on));
	}
	return ST_SUCCESS;
}

int	led_pulse(void *ptr)
{
	t_param *p = (t_param*)ptr;
	int		nbr = 0;
	double 	dbl = 0.0;

	if (p->len != 4)
		return ST_NOT_VALID;
	if (   (strlen(p->str[3]) < 3)
		   || (p->str[3][0] != 'f')
		   || (p->str[3][1] != ':'))
		return ST_NOT_VALID;
	dbl = atof(&(p->str[3][2]));
	if (dbl < 0.1 || dbl >= 2.0)
		return ST_NOT_VALID;
	nbr = atoi(p->str[2]);
	if ((nbr < 1) || (nbr > 3))
		return ST_NOT_VALID;
	p->pwm.led = nbr;
	p->pwm.frq = (int)rint(250.0 * dbl);
	make_pulse(&p->pwm);
	return ST_SUCCESS;
}
