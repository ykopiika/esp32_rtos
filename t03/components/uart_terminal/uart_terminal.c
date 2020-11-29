#include "uart_terminal.h"

static void full_line_check(t_buffer line, 	_Bool *is_printed_alarm)
{
	if (line.len == (MAX_LEN - 1) && !*is_printed_alarm)
	{
		*is_printed_alarm = true;
		char str[] = T_TRK"\r\n\n==>\t"
					 " line is full\r\n\n"R
					 "$ ";
		uart_write_bytes(UART_NUM_1, str, sizeof(str));
		uart_write_bytes(UART_NUM_1, (const char *) line.data, line.len);
	}
	else if (*is_printed_alarm && line.len != (MAX_LEN - 1))
		*is_printed_alarm = false;
}

void read_from_uart(void *param)
{
	t_data			*d = (t_data*)param;
	t_buffer        buf;
	t_buffer        line;
	uart_event_t    event;
	_Bool on = true;
	_Bool is_printed_alarm = false;

	memset(&buf, 0, sizeof(buf));
	memset(&line, 0, sizeof(line));
	uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
	while (on)
	{
		if (xQueueReceive(d->uart0_queue, (void *)&event, portMAX_DELAY))
		{
			if (event.type == UART_DATA && event.size < MAX_LEN)
			{
				buf.len = event.size;
				uart_read_bytes(UART_NUM_1, buf.data, event.size, 0);
				if (parse_uart_buffer(&buf))
					add_buffer_to_line(&buf, &line);
				else
					parse_uart_event(&buf, &line, d);
				full_line_check(line, &is_printed_alarm);
			}
			memset(&buf, 0, sizeof(buf));
			ESP_ERROR_CHECK(uart_flush(UART_NUM_1));
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

void init_uart(t_data *data)
{
	uart_config_t	uart;

	memset(&uart, 0, sizeof(uart));
	uart = (uart_config_t){
			.baud_rate = 9600,
			.data_bits = UART_DATA_8_BITS,
			.parity = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	};
	ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, 2048, 2048, 20, &data->uart0_queue, 0));
	ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart));
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, UART_TX_PIN, UART_RX_PIN,
				 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	ESP_ERROR_CHECK(uart_pattern_queue_reset(UART_NUM_1, 20));
}
