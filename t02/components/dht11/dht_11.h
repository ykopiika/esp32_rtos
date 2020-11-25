#ifndef BUTTON_DHT_11_H
#define BUTTON_DHT_11_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "error_functions.h"

#define DH11_DATA_PIN 4
#define DH11_POWER_PIN 2

typedef struct	s_dht
{
	uint8_t tem;
	uint8_t hum;
}				t_dht;

void dht11_init(void);
void get_value_dht11(uint8_t *temperature, uint8_t *humidity);
void dht_measuring_tsk(void *param);

#endif
