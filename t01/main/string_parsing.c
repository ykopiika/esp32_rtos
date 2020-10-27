#include "main.h"

void string_parse(uart_event_t event, uint8_t *dtmp)
{
  //  if (event.size == 1 && isprint(dtmp[0])) //if isprint
  //  {
        uart_write_bytes(UART_NUM_1, (const char *) dtmp, event.size);
//    }
//    if(event.size == 1 && (dtmp[0] == 13)) //enter
//    {
//        uart_write_bytes(UART_NUM_1, "\r\n", 2);
//        uart_write_bytes(UART_NUM_1, "$ ", 2);
//    }
}
