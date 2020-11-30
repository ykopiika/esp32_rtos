#include "oled.h"
#include "font6x8.h"
#include "font8x16.h"

void clear_pixels(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    memset(oled->pixels, 0, sizeof(oled->pixels));
}

void display_pixels(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd,
                                          (oled->addr << 1) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x80, true)); // single command
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); // page number
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true)); // data stream
    ESP_ERROR_CHECK(i2c_master_write(cmd, oled->pixels,
                                     sizeof(oled->pixels), true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    print_error(i2c_master_cmd_begin(
            oled->port, cmd, 10 / portTICK_PERIOD_MS),
                __func__, __LINE__, "master_cmd_begin failed");
    i2c_cmd_link_delete(cmd);
}

void put_pixel(uint8_t *pixels, int16_t x, uint16_t y, _Bool color)
{
    if (!pixels)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    if (color == true)
        pixels[x + ((y >> 3U) * LCDWIDTH)] |=  (1U << (y & 7U));
    else
        pixels[x + ((y >> 3U) * LCDWIDTH)] &= ~(1U << (y & 7U));
}

void str_to_oled_6x8(t_oled *oled, char *str, int y)
{
    if (!oled || !str)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    int len = strlen(str);
    int calc = LCDWIDTH - (len * 6);
    int x = (calc > 0) ? (calc / 2) : 0;
    for (int i = 0; (i < len) && (i < 168); ++i) {
        int pixels_index = (y * LCDWIDTH) + x;
        int char_index = (str[i] - 32) * 6;
		for (int j = 0; j < 6; ++j, pixels_index++, char_index++)
			oled->pixels[pixels_index] = font6x8[char_index];
        x += 6;
        if (x >= 126)
        {
            x = 0;
            y++;
        }
    }
    display_pixels(oled);
}

static void print_chars_8x16(t_oled *oled, t_point p, char *str, int len, int shift)
{
	int pixels_index = 0;
	int char_index = 0;

	for (int i = 0; (i < len) && (i < 168); ++i) {
		pixels_index = (p.y * LCDWIDTH) + p.x;
		char_index = ((str[i] - 32) * 16) + shift;
		for (int j = 0; j < 8; ++j, pixels_index++, char_index++)
			oled->pixels[pixels_index] = font8x16[char_index];
		p.x += 8;
		if (p.x >= 126)
		{
			p.x = 0;
			p.y++;
		}
	}
}

void str_to_oled_8x16(t_oled *oled, char *str, int y)
{
    if (!oled || !str)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    int len = strlen(str);
    int calc = LCDWIDTH - (len * 8);
    t_point p = {0, y};
    p.x = (calc > 0) ? (calc / 2) : 0;
	print_chars_8x16(oled, p, str, len, 0);
//	p.x = (calc > 0) ? (calc / 2) : 0;
	p.y++;
	print_chars_8x16(oled, p, str, len, 8);
	display_pixels(oled);
}
