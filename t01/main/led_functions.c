#include "main.h"


void hello_led(void *param)
{
	t_pwm *pwm = (t_pwm*)param;
	ledc_timer_config_t ledc_timer;
	ledc_channel_config_t ledc_channel;
	data_init(&ledc_timer, &ledc_channel, GPIO_NUM_26);
	printf(T_TRK"\t\t\t\t\t\t\t\t===> f = %d    num = %d"R, pwm->led, pwm->frq);
	while (1)
	{
		pwm_flashing(&ledc_channel, 1200);
	}
}

//void hello_led(void *param)
//{
//	t_pwm *pwm = (t_pwm*)param;
//	ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
//	ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
//	ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
//	printf(T_TRK"\t\t\t\t\t\t\t\t===> f = %d    num = %d"R, pwm->led, pwm->frq);
//	while (1)
//	{
//		ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, 1));
//		vTaskDelay(2);
//		ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, 0));
//		vTaskDelay(2);
//	}
//}

static void	make_pulse(int led, int frq)
{
	t_pwm pwm = {led, frq};
	printf("\t\t\t\t\t\t\t\t===> f = %d    num = %d", led, frq);
//	xTaskNotify(leds_management, pwm, eSetValueWithOverwrite);
	xTaskCreate(hello_led, "led", 4096, (void *) &pwm, 5, NULL);
}

int	led_on_off(void *ptr)
{
	t_param *p = (t_param*)ptr;
	_Bool	is_on = false;
	int		nbr = 0;
	int		leds[3] = {GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33};

	if (p->len < 3 || p->len > 5)
		return ST_NOT_VALID;
	printf(T_TRK"led_on_off called\n"R);
	if (!strcmp(p->str[1], "on"))
		is_on = true;
	for (int i = 2; i < p->len; ++i)
	{
		nbr = atoi(p->str[i]);
		if ((nbr < 1) || (nbr > 3))
			return ST_NOT_VALID;
		ESP_ERROR_CHECK(gpio_set_direction(leds[nbr - 1], GPIO_MODE_OUTPUT));
		ESP_ERROR_CHECK(gpio_set_level(leds[nbr - 1], is_on));
	}
	if (is_on)
		printf(T_TRK"led_on called\n"R);
	else
		printf(T_TRK"led_off called\n"R);
	for (int i = 0; i < p->len; ++i)
	{
		printf("\t\t\t\t%s|\n", p->str[i]);
	}
	return ST_SUCCESS;
}

int	led_pulse(void *ptr)
{
	t_param *p = (t_param*)ptr;
	int		nbr = 0;
	double 	dbl = 0.0;
	int		leds[3] = {GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33};

	if (p->len != 4)
		return ST_NOT_VALID;
	if (   (strlen(p->str[3]) < 3)
		|| (p->str[3][0] != 'f')
		|| (p->str[3][1] != ':'))
		return ST_NOT_VALID;
	dbl = atof(&(p->str[3][2]));
	if (dbl <= 0.0 || dbl > 2.0)
		return ST_NOT_VALID;
	printf(T_TRK"led_pulse called\n"R);
	nbr = atoi(p->str[2]);
	if ((nbr < 1) || (nbr > 3))
		return ST_NOT_VALID;
	(void)dbl;//todo calculate frq
	make_pulse(leds[nbr - 1], 1000);
	for (int i = 0; i < p->len; ++i)
	{
		printf("\t\t\t\t%s|\n", p->str[i]);
	}
	return ST_SUCCESS;
}

