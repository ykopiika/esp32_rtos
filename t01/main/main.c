#include "main.h"

static TaskHandle_t uart_tsk = NULL;
QueueHandle_t uart0_queue = NULL;

static void read_from_uart(void *param)
{
    t_app *a = (t_app*)param;
    uint8_t dtmp[256];
    uart_event_t event;
    char test_str[30];
    uint32_t res = 0;
    _Bool on = true;
    int len = 0;

    memset(&dtmp, 0, sizeof(dtmp));
    while(on)
    {
        if(xQueueReceive(uart0_queue, (void * )&event,
                         portMAX_DELAY)) {
            len = uart_read_bytes(UART_NUM_1, dtmp, event.size, portMAX_DELAY);
            if (len == 1 && isprint(dtmp[0]))
            {
                uart_write_bytes(UART_NUM_1, (const char *) dtmp, event.size);
            }
            else if(len == 1 && (dtmp[0] == 13))
            {
                uart_write_bytes(UART_NUM_1, "\r\n", 2);
            }
            printf("%d|%d| %d,%d,%d,%d,%d,%d,%d\n", event.type,event.size, dtmp[0], dtmp[1],
                   dtmp[2], dtmp[3], dtmp[4], dtmp[5], dtmp[6]);
            memset(&dtmp, 0, sizeof(dtmp));
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void init_app(t_app *app)
{
    memset(app, 0, sizeof(*app));
    app->uart_config = (uart_config_t){
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
//    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &app->uart_config));
//    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, 17, 16,
//                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
//    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0));

        uart_driver_install(UART_NUM_1, 2048, 2048, 20, &uart0_queue, 0);
        uart_param_config(UART_NUM_1, &app->uart_config);
        uart_set_pin(UART_NUM_1, UART_TX_PIN, UART_RX_PIN,
                            UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        uart_pattern_queue_reset(UART_NUM_1, 20);
}

void app_main(void)
{
    t_app app;
    init_app(&app);


    xTaskCreate(read_from_uart, "read_uart",
                2048, &app, 10, NULL);
}










//            if (uart_write_bytes(UART_NUM_1,
//                                 (const char *) test_str, strlen(test_str)) == -1)
//                ESP_ERROR_CHECK(ESP_FAIL);
//TODO: set protect for all funct