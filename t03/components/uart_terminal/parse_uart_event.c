#include "uart_terminal.h"

static void arrow_commands(t_buffer *buf, t_buffer *line)
{
    if (buf->data[2] == 65)
        printf("UP\n");
    else if (buf->data[2] == 66)
        printf("DOWN\n");
    else if (buf->data[2] == 67) // right
    {
        if (line->index < line->len)
        {
            uart_write_bytes(UART_NUM_1, (char *)&line->data[line->index], 1);
            line->index++;
        }
    }
    else if (buf->data[2] == 68) // left
    {
        if (line->index > 0)
        {
            line->index--;
            uart_write_bytes(UART_NUM_1, "\b", 1);
        }
    }
}

static void backspase_command(t_buffer *line)
{
    if (line->index > 0) {
        line->index--;
        line->data[line->index] = 0;
        uart_write_bytes(UART_NUM_1, "\b \b", 3);
        if (line->index != line->len)
        {
            for (int i = line->index; i < (line->len - 1); ++i) {
                line->data[i] = line->data[i+1];
                if (i == line->len - 2)
                    line->data[i+1] = 0;
            }
            uart_write_bytes(UART_NUM_1, (const char *)&line->data[line->index],
                             (line->len - (line->index + 1)));
            uart_write_bytes(UART_NUM_1, "\e[0J", sizeof("\e[0J"));
            for (int i = 0; i < (line->len - (line->index + 1)); ++i) {
                uart_write_bytes(UART_NUM_1, "\b", sizeof("\b"));
            }
        }
        line->len--;
    }
}

static void enter_command(t_buffer *line, t_data *d)
{
    if(line->index != line->len)
        uart_write_bytes(UART_NUM_1, (const char *)&line->data[line->index],
                         (line->len - line->index));
    uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
	parse_command_line(line, d);
	memset(line, 0, sizeof(*line));
}

void parse_uart_event(t_buffer *buf, t_buffer *line, t_data *d)
{
	if (!buf || !line)
		return;
	if (buf->len == 1)
	{
		if (buf->data[0] == 13) // enter was pressed
			enter_command(line, d);
		else if (buf->data[0] == 127) //backspace was pressed
			backspase_command(line);
	}
	else if (buf->len == 3 && buf->data[0] == 27 && buf->data[1] == 91)
		arrow_commands(buf, line);
}
