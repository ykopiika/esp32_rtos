#include "main.h"

static TaskHandle_t oled_tsk = NULL;
static xSemaphoreHandle xMutex;

static void get_photoresistor_value()
{

}


static void read_photoresistor(void *param)
{
    t_app *a = (t_app*)param;
    int res = 0;
    _Bool on = true;
    while(on)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == 1)
        {
            if (oled_tsk != NULL)
            {
                int val = adc1_get_raw(ADC1_CHANNEL_0);
                xTaskNotify(oled_tsk, val, eSetValueWithOverwrite);
//                res++;
            }
            xSemaphoreGive(xMutex);
        }
        else
            vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void oled_brightness(void *param)
{
    uint32_t res = 0;
    _Bool on = true;
    while(on)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == 1)
        {
            if(xTaskNotifyWait(0, 0, &res, portMAX_DELAY) == pdTRUE)
            {
                printf("%d\n", res);
            }
            xSemaphoreGive(xMutex);
        }
        else
            vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void init_app(t_app *app)
{
    memset(app, 0, sizeof(*app));
    init_oled(&app->oled);
    memset(&app->oled.pixels, 255, sizeof(app->oled.pixels));
    display_pixels(&app->oled);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));//36 gpio get_raw
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_MAX);
}

void app_main(void)
{
    t_app app;
    init_app(&app);

    xMutex = xSemaphoreCreateMutex();

    xTaskCreate(read_photoresistor, "photoresistor",
                2048, &app, 10, NULL);
    xTaskCreate(oled_brightness, "oled_brightness",
                2048, &app, 10, &oled_tsk);
}
