#ifndef STRUCTS_H
# define STRUCTS_H

//------------------------------------------------ for oled start
#include "driver/i2c.h"

#define LCDWIDTH    128
#define LCDHEIGHT   8

typedef struct		s_oled
{
	uint8_t         addr;
	i2c_port_t      port;
	uint8_t         pixels[LCDWIDTH * LCDHEIGHT];
}					t_oled;
//------------------------------------------------ for oled end

//------------------------------------------------ for uart start
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
//------------------------------------------------ for uart end

typedef struct	s_data
{
	t_command	*lst;
	t_oled		oled;
}				t_data;

#endif
