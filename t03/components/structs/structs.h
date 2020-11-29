#ifndef STRUCTS_H
# define STRUCTS_H

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


typedef struct	s_data
{
	QueueHandle_t	dht_queue;
	t_dht_lst		*head;
	t_command		*cmds;
	t_oled			oled;
	t_cmd_tmp		tmp;
	t_led_tasks		leds;
}				t_data;

#endif
