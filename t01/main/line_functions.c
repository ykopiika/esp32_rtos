#include "main.h"

void add_buffer_to_line(t_buffer *buf, t_buffer *line)
{
    if (!buf || !line)
        return;
    if (buf->len < (MAX_LEN - line->len))
    {
        for (int i = 0; i < buf->len; ++i) {
            line->data[line->len + i] = buf->data[i];
        }
        line->len += buf->len;
        uart_write_bytes(UART_NUM_1, (const char *)buf->data, buf->len);
    }
    //todo: check is full line -> call enter event
}

