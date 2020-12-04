#ifndef UART_TERMINAL_H
# define UART_TERMINAL_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"

#include "libft.h"
#include "error_functions.h"
#include "led_commands.h"
#include "structs.h"

#define UART_TX_PIN 17
#define UART_RX_PIN 16
//#define CMD_LEN 3

t_command *command_registration(t_fnxptr *fx_arr, t_command *lst, char *name, char *sub);
void		parse_uart_event(t_buffer *buf, t_buffer *line, t_data *d);
_Bool		parse_uart_buffer(t_buffer *buf);
void		add_buffer_to_line(t_buffer *buf, t_buffer *line);
void		parse_command_line(t_buffer *line,  t_data *d);
void		init_uart(t_data *data);
void		read_from_uart(void *param);

#endif
