#include "main.h"

static void	make_pulse(int led_num, double dbl)
{
	return;
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
	make_pulse(leds[nbr - 1], dbl);
	for (int i = 0; i < p->len; ++i)
	{
		printf("\t\t\t\t%s|\n", p->str[i]);
	}
	return ST_SUCCESS;
}

