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
#define MAX_LEN 120

#define T_BLK "\x1b[30m" // 30 - text is 	black
#define T_RED "\x1b[31m" // 31 - text is 	red
#define T_GRN "\x1b[32m" // 32 - text is 	green
#define T_YEL "\x1b[33m" // 33 - text is 	yellow
#define T_BLU "\x1b[34m" // 34 - text is 	blue
#define T_PNK "\x1b[35m" // 35 - text is 	pink
#define T_TRK "\x1b[36m" // 36 - text is 	turquoise

#define M_BLK "\x1b[40m" // 40 - black 		marker for text
#define M_RED "\x1b[41m" // 41 - red 		marker for text
#define M_GRN "\x1b[42m" // 42 - green		marker for text
#define M_YEL "\x1b[43m" // 43 - yellow		marker for text
#define M_BLU "\x1b[44m" // 44 - blue		marker for text
#define M_PNK "\x1b[45m" // 45 - pink		marker for text
#define M_TRK "\x1b[46m" // 46 - turquoise	marker for text

#define T_BLD "\x1b[1m"  //  1  bold 		text
#define T_CRS "\x1b[3m"  //  3  cursive 	text
#define T_UNL "\x1b[4m"  //  4  underline 	text
#define T_SLV "\x1b[2m"  //  2  text is 	silver

#define R "\x1B[0m"		 // reset

typedef struct  s_buffer
{
    uint8_t     data[MAX_LEN];
    size_t      len;
    size_t      index;
}               t_buffer;
    
typedef struct  s_app
{
    t_buffer buf;
    t_buffer line;
    uart_event_t event;
    uart_config_t uart_config;
}               t_app;

_Bool   parse_uart_buffer(t_buffer *buf);
void    parse_uart_event(t_buffer *buf, t_buffer *line);
void    add_buffer_to_line(t_buffer *buf, t_buffer *line);
void    parse_command_line(t_buffer *line);
//void print_new_line(_Bool is_first_line);

#endif
