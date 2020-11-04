#include "main.h"

static TaskHandle_t uart_tsk = NULL;
QueueHandle_t uart0_queue = NULL;

static void read_from_uart(void *param)
{
    t_app *a = (t_app*)param;
    _Bool on = true;
    _Bool is_printed_alarm = false;

    memset(&a->buf, 0, sizeof(a->buf));
    memset(&a->line, 0, sizeof(a->line));
    uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
    while (on)
    {
        if (xQueueReceive(uart0_queue, (void *)&a->event,
                         portMAX_DELAY)) {
            if (a->event.type == UART_DATA && a->event.size < MAX_LEN)
            {
                a->buf.len = a->event.size;
                uart_read_bytes(UART_NUM_1, a->buf.data, a->event.size, 1);
                if (buffer_parse(&a->buf))
                    add_buffer_to_line(&a->buf, &a->line); // todo add full line logic
                else
                    parse_command(&a->buf, &a->line);
                if (a->line.index == (MAX_LEN - 1) && !is_printed_alarm)
                {
                        is_printed_alarm = true;
                        char str[] = T_GRN"\r\n\n==>\tline is full\r\n\n$ "R;
                        uart_write_bytes(UART_NUM_1, str, sizeof(str));
                        uart_write_bytes(UART_NUM_1, (const char *) a->line.data, a->line.len);

                }
                else if (is_printed_alarm && a->line.index != (MAX_LEN - 1))
                    is_printed_alarm = false;
            }
            printf(T_YEL"buf len:%d\n"R, a->buf.len);
            printf(T_RED"line i:%d len:%d %s\n"R, a->line.index, a->line.len, (char*)a->line.data);
//            for (int i = 0; i < 10; ++i)
//                printf(T_GRN"%d "R, a->line.data[i]);
            printf(T_YEL"evn t:%d s:%d/ BUF: "R, a->event.type, a->event.size); //todo: clear debug
            for (int i = 0; i < 10; ++i)
                printf(T_PNK"%d "R, a->buf.data[i]);
            printf("\n"); //todo: clear debug
            memset(&a->buf, 0, sizeof(a->buf));
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