#ifndef MAIN_H
#define MAIN_H

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


#define UART_TX_PIN 17
#define UART_RX_PIN 16
#define MAX_LEN 120
#define CMD_LEN 3

#define ERR_MALLOC "Fatal: malloc failed!"
#define ERR_VAL_NULL "Fatal: value is NULL!"


typedef	void	(*t_fnxptr)(void*);

typedef struct  s_cmd_config
{
	t_fnxptr	func[5];
	int 		int_arg_num;
	int 		dbl_arg_num;
}               t_cmd_config;

typedef struct  s_command
{
    char		*cmd_name;
    char		**cmd_sub_names;
	t_fnxptr	*func;
}               t_command;

typedef struct  s_buffer
{
    uint8_t     data[MAX_LEN];
    size_t      len;
    size_t      index;
}               t_buffer;
    
typedef struct  s_app
{
    t_buffer        buf;
    t_buffer        line;
    t_command       *cmd;
    uart_event_t    event;
    uart_config_t   uart_config;
}               t_app;

_Bool   parse_uart_buffer(t_buffer *buf);
void	parse_uart_event(t_buffer *buf, t_buffer *line, t_command *cmds);
void    add_buffer_to_line(t_buffer *buf, t_buffer *line);
void	parse_command_line(t_buffer *line, t_command *cmds);
//void print_new_line(_Bool is_first_line);
void	led_on(void *ptr);
void	led_off(void *ptr);
void	led_pulse(void *ptr);

#endif
