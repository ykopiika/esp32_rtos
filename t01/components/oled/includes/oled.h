#ifndef OLED_H
#define OLED_H

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "error_functions.h"

#define OLED_ADDR   0x3C
#define I2C_PORT    I2C_NUM_0
#define OLED_EN     GPIO_NUM_32
#define SDA_PIN     GPIO_NUM_21
#define SCL_PIN     GPIO_NUM_22

#define LCDWIDTH    128
#define LCDHEIGHT   8

typedef struct      s_oled
{
    uint8_t         addr;
    i2c_port_t      port;
    uint8_t         pixels[LCDWIDTH * LCDHEIGHT];
}                   t_oled;

void display_pixels(t_oled *oled);
void put_pixel(uint8_t *pixels, int16_t x, uint16_t y, _Bool color);
void init_oled(t_oled *oled);
void str_to_oled(t_oled *oled, char *str);
void clear_pixels(t_oled *oled);

#endif
