#include "main.h"

_Bool buffer_parse(t_buffer *buf)
{
    if(!buf)
        return false;
    for (int i = 0; i < buf->len; ++i) {
        if(!isprint(buf->data[i]))
            return false;
    }
    return true;
}

void parse_command(t_buffer *buf, t_buffer *line)
{
    if (!buf || !line)
        return;
    if(buf->len == 1 && buf->data[0] == 13) // enter was pressed
    {
        uart_write_bytes(UART_NUM_1, "\r\n$ ", 4);
//                    parse_line(); //todo parse line
//                    clear_line(); //todo clear line
    }
}
