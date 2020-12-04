#ifndef STRUCTS_H
# define STRUCTS_H

#include <ctype.h>
#include "driver/i2s.h"

//------------------------------------------------ oled start
#include "driver/i2c.h"

#define LCDWIDTH    128
#define LCDHEIGHT   8

typedef struct		s_oled
{
	uint8_t         addr;
	i2c_port_t      port;
	uint8_t         pixels[LCDWIDTH * LCDHEIGHT];
}					t_oled;
//------------------------------------------------ oled end

//------------------------------------------------ uart start
typedef struct  s_command	t_command;
typedef struct  s_subcmd	t_subcmd;
typedef	int		(*t_fnxptr)(void *);

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

typedef struct		s_cmd_tmp
{
	char		**str;
	int			len;
	int			led;
	int 		frq;
}					t_cmd_tmp;
//------------------------------------------------ uart end

//------------------------------------------------ dht start
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
//------------------------------------------------ dht end

//------------------------------------------------ leds start
typedef struct	s_led_tasks
{
	TaskHandle_t	led1_pulse;
	TaskHandle_t	led2_pulse;
	TaskHandle_t	led3_pulse;
}				t_led_tasks;
//------------------------------------------------ leds end

//------------------------------------------------ timer start
#include "driver/timer.h"
#define TIMER_DIVIDER 80
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER)

typedef struct	s_hms_time
{
	uint8_t		hrs;
	uint8_t		min;
	uint8_t		sec;
}				t_hms_time;
//------------------------------------------------ timer end

#define MAX_LEN 40

typedef struct  s_buffer
{
	uint8_t     data[MAX_LEN];
	size_t      len;
	size_t      index;
}               t_buffer;

typedef struct	s_data
{
	TaskHandle_t	oled_time_task;
	QueueHandle_t	uart0_queue;
	QueueHandle_t	dht_queue;
	t_dht_lst		*head;
	t_command		*cmds;
	t_oled			oled;
	t_cmd_tmp		tmp;
	t_led_tasks		leds;
	char			dht_str[21];

	_Bool 			is_alarm_on;
	uint64_t		alarm_time;

	t_buffer		buf;
	t_buffer		line;
}				t_data;

#endif
