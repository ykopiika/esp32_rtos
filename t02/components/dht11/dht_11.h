#ifndef BUTTON_DHT_11_H
#define BUTTON_DHT_11_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "driver/uart.h"

#include "error_functions.h"

#define DH11_DATA_PIN 4
#define DH11_POWER_PIN 2

#define MAX_DHT_LST 60
#define TIMER_DIVIDER 80
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER)

typedef struct	s_dht_lst t_dht_lst;

typedef struct	s_dht
{
	uint8_t		tem;
	uint8_t		hum;
	uint64_t	time;
}				t_dht;

struct	s_dht_lst
{
	t_dht	dht;

	t_dht_lst *next;
};

void dht11_init(void);
void get_value_dht11(uint8_t *temperature, uint8_t *humidity);
void dht_write_to_lists_tsk(void *param);
void dht_measuring_tsk(void *param);
t_dht_lst *add_dht_lst(t_dht_lst *head);
t_dht_lst *last_lst_to_first(t_dht_lst *head);
int print_all_lists(void *ptr);

#endif
