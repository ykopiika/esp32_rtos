#include "led_commands.h"

static TaskHandle_t	led1_pulse = NULL;
static TaskHandle_t	led2_pulse = NULL;
static TaskHandle_t	led3_pulse = NULL;

static void hello_led(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_cmd_tmp	*tmp = &d->tmp;
	int			leds[3] = {GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33};

	ESP_ERROR_CHECK(gpio_set_direction(leds[tmp->led - 1], GPIO_MODE_OUTPUT));
	while (1)
	{
		vTaskDelay(tmp->frq/portTICK_PERIOD_MS);
		ESP_ERROR_CHECK(gpio_set_level(leds[tmp->led - 1], 1));
		vTaskDelay(tmp->frq/portTICK_PERIOD_MS);
		ESP_ERROR_CHECK(gpio_set_level(leds[tmp->led - 1], 0));
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

static void	make_pulse(t_data *d)
{
	t_cmd_tmp	*tmp = &d->tmp;

	check_is_run_pulse(tmp->led);
	if (tmp->led == 1)
		xTaskCreate(hello_led, "led", 2048, (void *)d, 5, &led1_pulse);
	if (tmp->led == 2)
		xTaskCreate(hello_led, "led", 2048, (void *)d, 5, &led2_pulse);
	if (tmp->led == 3)
		xTaskCreate(hello_led, "led", 2048, (void *)d, 5, &led3_pulse);
}

int	led_on_off(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_cmd_tmp	*tmp = &d->tmp;
	_Bool		is_on = false;
	int			nbr = 0;
	int			leds[3] = {GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_33};

	if (tmp->len < 3 || tmp->len > 5)
		return ST_NOT_VALID;
	if (!strcmp(tmp->str[1], "on"))
		is_on = true;
	for (int i = 2; i < tmp->len; ++i)
	{
		nbr = atoi(tmp->str[i]);
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
	t_data		*d = (t_data*)ptr;
	t_cmd_tmp	*tmp = &d->tmp;
	int		nbr = 0;
	double 	dbl = 0.0;

	if (tmp->len != 4)
		return ST_NOT_VALID;
	if (   (strlen(tmp->str[3]) < 3)
		   || (tmp->str[3][0] != 'f')
		   || (tmp->str[3][1] != ':'))
		return ST_NOT_VALID;
	dbl = atof(&(tmp->str[3][2]));
	if (dbl < 0.1 || dbl >= 2.0)
		return ST_NOT_VALID;
	nbr = atoi(tmp->str[2]);
	if ((nbr < 1) || (nbr > 3))
		return ST_NOT_VALID;
	tmp->led = nbr;
	tmp->frq = (int)rint(250.0 * dbl);
	make_pulse(d);
	return ST_SUCCESS;
}
