#include "main.h"

static void print_one_symbol(size_t size, uint8_t *buf)
{
    if (isprint(buf[0])) //if isprint
    {
        uart_write_bytes(UART_NUM_1, (const char *)buf, size);
    }
    else if(buf[0] == 13) //enter
    {
        print_new_line(false);
    }
}

static void check_line(size_t size, uint8_t *buf)
{
    return; //todo make checking logic
}

void string_parse(size_t size, uint8_t *buf)
{
    if (size == 1)
        print_one_symbol(size, buf);
    else
        check_line(size, buf);
}
