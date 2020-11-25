#include "uart_terminal.h"

QueueHandle_t uart0_queue = NULL;

void debug(t_buffer buf, t_buffer line, uart_event_t event)
{
	printf(T_YEL"buf len:%d\n"R, buf.len);
	printf(T_RED"line i:%d len:%d %s\n"R, line.index, line.len, (char*)line.data);
//            for (int i = 0; i < 10; ++i)
//                printf(T_GRN"%d "R, cmds->line.data[i]);
	printf(T_YEL"evn t:%d s:%d/ BUF: "R, event.type, event.size); //todo: clear debug
	for (int i = 0; i < 10; ++i)
		printf(T_PNK"%d "R, buf.data[i]);
	printf("\n"); //todo: clear debug
}

void read_from_uart(void *param)
{
	t_command		*cmds = (t_command*)param;
	t_buffer        buf;
	t_buffer        line;
	uart_event_t    event;
	_Bool on = true;
	_Bool is_printed_alarm = false;

	memset(&buf, 0, sizeof(buf));
	memset(&line, 0, sizeof(line));
	uart_write_bytes(UART_NUM_1, "\r\n$ ", 4); // todo
	while (on)
	{
		if (xQueueReceive(uart0_queue, (void *)&event,
						  portMAX_DELAY)) {
			if (event.type == UART_DATA && event.size < MAX_LEN)
			{
				buf.len = event.size;
				uart_read_bytes(UART_NUM_1, buf.data, event.size, 0);
				if (parse_uart_buffer(&buf))
					add_buffer_to_line(&buf, &line); // todo add full line logic
				else
					parse_uart_event(&buf, &line, cmds);
				if (line.len == (MAX_LEN - 1) && !is_printed_alarm)
				{
					is_printed_alarm = true;
					char str[] = T_GRN"\r\n\n==>\tline is full\r\n\n"R
								 "$ "
								 "\xE2\x9C\x85";
					uart_write_bytes(UART_NUM_1, str, sizeof(str));
					uart_write_bytes(UART_NUM_1, (const char *) line.data, line.len);
				}
				else if (is_printed_alarm && line.len != (MAX_LEN - 1))
					is_printed_alarm = false;
			}
//            debug(buf,line,event);
			memset(&buf, 0, sizeof(buf));
			uart_flush(UART_NUM_1);
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

void init_uart(uart_config_t *uart)
{
	memset(uart, 0, sizeof(*uart));
	*uart = (uart_config_t){
			.baud_rate = 9600,
			.data_bits = UART_DATA_8_BITS,
			.parity = UART_PARITY_DISABLE,
			.stop_bits = UART_STOP_BITS_1,
			.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	};
	uart_driver_install(UART_NUM_1, 2048, 2048, 20, &uart0_queue, 0);
	uart_param_config(UART_NUM_1, uart);
	uart_set_pin(UART_NUM_1, UART_TX_PIN, UART_RX_PIN,
				 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_pattern_queue_reset(UART_NUM_1, 20);
}
