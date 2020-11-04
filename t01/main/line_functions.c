#include "main.h"

void add_buffer_to_line(t_buffer *buf, t_buffer *line)
{
    if (!buf || !line)
        return;
    if (buf->len < (MAX_LEN - line->len))
    {
        if (line->index != line->len)
        {
            memmove(&line->data[line->index + buf->len], &line->data[line->index], line->len - line->index);
        }
        for (int i = 0; i < buf->len; ++i) {
            line->data[line->index + i] = buf->data[i];
        }
        uart_write_bytes(UART_NUM_1, (const char *)buf->data, buf->len);
        if (line->index != line->len)
        {
            uart_write_bytes(UART_NUM_1, (const char *)&line->data[line->index],
                             (line->len - line->index));
        }
        line->len += buf->len;
        line->index += buf->len;
    }
    //todo: check is full line -> call enter event
}

