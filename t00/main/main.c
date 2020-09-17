#include "main.h"

static TaskHandle_t oled_tsk = NULL;
static xSemaphoreHandle xMutex;
//static TaskHandle_t photoresistor_tsk = NULL;


static void read_photoresistor(void *param)
{
    t_app *a = (t_app*)param;
    int res = 0;
    _Bool on = true;
    while(on)
    {
        if (xSemaphoreTake(xMutex, (TickType_t)0xffffffff) == 1)
        {
            if (oled_tsk != NULL)
            {
                xTaskNotify(oled_tsk, res, eSetValueWithOverwrite);
                res++;
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
        if (xSemaphoreTake(xMutex, (TickType_t)0xffffffff) == 1)
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
