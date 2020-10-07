#include "oled.h"

static void set_part_of_commands(i2c_cmd_handle_t cmd)
{
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x80, true)); /// 0b10000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA8, true)); /// 0b10101000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xFF, true)); /// 0b11111111
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x8D, true)); /// 0b10001101
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x14, true)); /// 0b00010100
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x20, true)); /// 0b00100000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); /// 0b00000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x10, true)); /// 0b00010000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xB0, true)); /// 0b10110000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xC8, true)); /// 0b11001000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); /// 0b00000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x10, true)); /// 0b00010000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true)); /// 0b01000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA1, true)); /// 0b10100001
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA6, true)); /// 0b10100110
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x81, true)); /// 0b10000001
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xFF, true)); /// 0b11111111
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xAF, true)); /// 0b10101111
}

static void set_commands(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd,
                        (oled->addr << 1U) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xAE, true)); /// 0b10101110
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xD5, true)); /// 0b11010101
    set_part_of_commands(cmd);
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(
            oled->port, cmd, 10 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

static void init_i2c(void)
{
    i2c_config_t i2c_config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PIN,
            .scl_io_num = SCL_PIN,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = 1000000
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));
}

void init_oled(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    *oled = (t_oled){.addr = OLED_ADDR, .port = I2C_PORT, .pixels = {0}};
    ESP_ERROR_CHECK(gpio_set_direction(OLED_EN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(OLED_EN, 1));
    ets_delay_us(1000000);
    init_i2c();
    set_commands(oled);
    clear_pixels(oled);
    display_pixels(oled);
}
