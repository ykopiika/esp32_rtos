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
#include "driver/gpio.h"
#include "stdlib.h"
#include "flashing_leds.h"
#include "math.h"

#define UART_TX_PIN 17
#define UART_RX_PIN 16
#define MAX_LEN 120
#define CMD_LEN 3

#define ERR_MALLOC "Fatal: malloc failed!"
#define ERR_VAL_NULL "Fatal: value is NULL!"

enum			e_state
{
	ST_NOT_FOUND = 0,
	ST_NOT_VALID = 1,
	ST_SUCCESS = 2,
	ST_OK = 3,
};

typedef struct  s_command	t_command;
typedef struct  s_subcmd	t_subcmd;
typedef	int		(*t_fnxptr)(void *);

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

typedef struct  s_buffer
{
    uint8_t     data[MAX_LEN];
    size_t      len;
    size_t      index;
}               t_buffer;

struct  s_subcmd
{
    char		*name;
	t_fnxptr	func;

	t_subcmd 	*next;
};

struct  s_command
{
    char		*name;

	t_command	*next;
	t_subcmd 	*down;
};

t_command	*command_registration(char *name, char *sub,
								t_fnxptr *fx_arr);
void parse_uart_event(t_buffer *buf, t_buffer *line, t_command *cmds);
_Bool		parse_uart_buffer(t_buffer *buf);
void		add_buffer_to_line(t_buffer *buf, t_buffer *line);
void		parse_command_line(t_buffer *line, t_command *cmds);
int			led_on_off(void *ptr);
int			led_pulse(void *ptr);

#endif
