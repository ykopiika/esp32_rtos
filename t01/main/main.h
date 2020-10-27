#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"


#define UART_TX_PIN 17
#define UART_RX_PIN 16


typedef struct  s_app
{
    uart_config_t uart_config;
}               t_app;

void string_parse(size_t size, uint8_t *buf);
void print_new_line(_Bool is_first_line);

#endif
