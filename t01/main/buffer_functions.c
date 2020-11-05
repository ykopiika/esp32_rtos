#include "main.h"

_Bool buffer_parse(t_buffer *buf)
{
    if (!buf)
        return false;
    for (int i = 0; i < buf->len; ++i) {
        if (!isprint(buf->data[i]))
            return false;
    }
    return true;
}

void parse_command(t_buffer *buf, t_buffer *line)
{
    if (!buf || !line)
        return;
    if (buf->len == 1) // enter was pressed
    {
        if (buf->data[0] == 13)
        {
            uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
            printf(T_RED"\nparse && clear line\n"R);
            //                    parse_line(); //todo parse line
//                    clear_line(); //todo clear line
            memset(line, 0, sizeof(*line));
            printf(T_RED"line: %zu, *line: %zu\n"R, sizeof(line), sizeof(*line));
        }
        if (buf->data[0] == 127)
        {
            if (line->index > 0) {
                line->index--;
                line->data[line->index] = 0;
                uart_write_bytes(UART_NUM_1, "\b \b", 3);
                if (line->index != line->len)
                {
                    uart_write_bytes(UART_NUM_1, "\e[s", sizeof("\e[s"));
                    for (int i = line->index; i < (line->len - 1); ++i) {
                        line->data[i] = line->data[i+1];
                        if (i == line->len - 2)
                            line->data[i+1] = 0;
                    }
                    uart_write_bytes(UART_NUM_1, (const char *)&line->data[line->index],
                                     (line->len - (line->index + 1)));
//                    uart_write_bytes(UART_NUM_1, " ", 1);
                    uart_write_bytes(UART_NUM_1, "\e[0J", sizeof("\e[0J"));
                    uart_write_bytes(UART_NUM_1, "\e[u", sizeof("\e[u"));

                }
                line->len--;
            }
        }
    }
    else if (buf->len == 3 && buf->data[0] == 27 && buf->data[1] == 91)
    {
        if (buf->data[2] == 65)
            printf("UP ");
        else if (buf->data[2] == 66)
            printf("DOWN ");
        else if (buf->data[2] == 67)
        {
            printf("RIGHT ");
            if (line->index < line->len)
            {
                uart_write_bytes(UART_NUM_1, (char *)&line->data[line->index], 1);
                line->index++;
//                uart_write_bytes(UART_NUM_1, (const char*)buf->data, buf->len);
            }
        }
        else if (buf->data[2] == 68) {
            printf("LEFT ");
            if (line->index > 0)
            {
                line->index--;
//                uart_write_bytes(UART_NUM_1, (const char*) buf->data, buf->len);
                uart_write_bytes(UART_NUM_1, "\b", 1);
            }
        }
    }
}
