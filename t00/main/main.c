#include "main.h"

static TaskHandle_t oled_tsk = NULL;
static xSemaphoreHandle xMutex;

static void set_oled_brightness(t_oled *oled, uint8_t res)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd,
                       (oled->addr << 1U) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x81, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, res, true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(
            oled->port, cmd, 10 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

static void read_photoresistor(void *param)
{
    (void)param;
    _Bool on = true;
    while(on)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == 1)
        {
            if (oled_tsk != NULL)
            {
                int val = adc1_get_raw(ADC1_CHANNEL_0);
                val = MAX_VALUE - val;
                xTaskNotify(oled_tsk, val, eSetValueWithOverwrite);
            }
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void oled_brightness(void *param)
{
    t_app *a = (t_app*)param;
    uint32_t res = 0;
    _Bool on = true;
    while(on)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == 1)
        {
            if(xTaskNotifyWait(0, 0, &res, 1) == pdTRUE)
            {
                res = (uint32_t)((double)res *
                        (double)(255.0/(double)MAX_VALUE));
                printf("%d\n", res);
                set_oled_brightness(&a->oled, (uint8_t)res);
            }
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void init_app(t_app *app)
{
    memset(app, 0, sizeof(*app));
    init_oled(&app->oled);
    memset(&app->oled.pixels, 255, sizeof(app->oled.pixels));
    display_pixels(&app->oled);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));
    ESP_ERROR_CHECK(adc1_config_channel_atten(
                        ADC1_CHANNEL_0,ADC_ATTEN_DB_11));
}

void app_main(void)
{
    t_app app;
    init_app(&app);

    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(oled_brightness, "oled_brightness",
                2048, &app, 10, &oled_tsk);
    xTaskCreate(read_photoresistor, "photoresistor",
                2048, NULL, 10, NULL);
}
