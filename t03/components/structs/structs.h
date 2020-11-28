#ifndef STRUCTS_H
# define STRUCTS_H

#include "driver/i2c.h"

#define LCDWIDTH    128
#define LCDHEIGHT   8

typedef struct		s_oled
{
	uint8_t         addr;
	i2c_port_t      port;
	uint8_t         pixels[LCDWIDTH * LCDHEIGHT];
}					t_oled;

typedef struct	s_data
{
	t_oled		oled;
}				t_data;

#endif
