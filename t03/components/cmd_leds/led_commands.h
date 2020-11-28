#ifndef LED_COMMANDS_H
# define LED_COMMANDS_H

#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "error_functions.h"
#include "structs.h"

int			led_on_off(void *ptr);
int			led_pulse(void *ptr);

#endif
