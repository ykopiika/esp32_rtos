#ifndef BUTTON_DHT_11_H
#define BUTTON_DHT_11_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include "error_functions.h"
#include "structs.h"

#define DH11_DATA_PIN 4
#define DH11_POWER_PIN 2

#define MAX_DHT_LST 60


void dht11_init(void);
void get_value_dht11(uint8_t *temperature, uint8_t *humidity);
void dht_write_to_lists_tsk(void *param);
void dht_measuring_tsk(void *param);
t_dht_lst *add_dht_lst(t_dht_lst *head);
t_dht_lst *last_lst_to_first(t_dht_lst *head);
int print_all_lists(void *ptr);

#endif
