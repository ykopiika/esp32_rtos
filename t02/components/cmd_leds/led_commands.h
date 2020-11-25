#ifndef LED_COMMANDS_H
# define LED_COMMANDS_H

#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

enum			e_state
{
	ST_NOT_FOUND = 0,
	ST_NOT_VALID = 1,
	ST_SUCCESS = 2,
	ST_OK = 3,
};

typedef struct  s_pwm
{
	int			led;
	int 		frq;
}				t_pwm;

typedef struct  s_param
{
	char		**str;
	int			len;
	t_pwm		pwm;
}				t_param;

int			led_on_off(void *ptr);
int			led_pulse(void *ptr);

#endif
