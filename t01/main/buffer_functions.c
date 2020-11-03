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
    else if(buf->len == 3 && buf->data[0] == 27 && buf->data[1] == 91)
    {
        if (buf->data[2] == 65)
            printf("UP ");
        else if (buf->data[2] == 66)
            printf("DOWN ");
        else if (buf->data[2] == 67)
            printf("RIGHT ");
        else if (buf->data[2] == 68)
            printf("LEFT ");
    }
}
