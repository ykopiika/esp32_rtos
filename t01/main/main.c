#include "main.h"

static TaskHandle_t uart_tsk = NULL;
QueueHandle_t uart0_queue = NULL;

void print_new_line(_Bool is_first_line)
{
    if (is_first_line)
        uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
    else
    {
        uart_write_bytes(UART_NUM_1, "\r\n", 2);
        uart_write_bytes(UART_NUM_1, "$ ", 2);
    }
}

static void read_from_uart(void *param)
{
    t_app *a = (t_app*)param;
    uint8_t buf[256];
    uart_event_t event;
    char test_str[30];
    uint32_t res = 0;
    _Bool on = true;
    int len = 0;
    print_new_line(true);
    memset(&buf, 0, sizeof(buf));
    while(on)
    {
        printf("  -  \n");
        if(xQueueReceive(uart0_queue, (void *)&event,
                         portMAX_DELAY)) {
            if(event.type == UART_DATA)
            {
                uart_read_bytes(UART_NUM_1, buf, event.size, 1);
                string_parse(event.size, buf);
            }
            printf("%d|%d| %d,%d,%d,%d,%d,%d,%d\n", event.type, event.size, buf[0], buf[1],
                   buf[2], buf[3], buf[4], buf[5], buf[6]); //todo: clear debug
            memset(&buf, 0, sizeof(buf));
            uart_flush(UART_NUM_1);
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