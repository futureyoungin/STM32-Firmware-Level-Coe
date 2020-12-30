/*
 * ap.c
 *
 *  Created on: 2020. 12. 29.
 *      Author: MCES
 */


#include "ap.h"





void apInit(void)
{
  uartOpen(_DEF_UART1, 115200);
}

void apMain(int argc, char *argv[])
{
  bool uart_is_open = false;


  if (argc >= 3)
  {
    char *port_name;
    int baud;

    port_name = argv[1];
    baud      = (int)strtoul((const char * ) argv[2], (char **)NULL, (int) 0);
    uartSetPortName(_DEF_UART2, port_name);

    if (uartOpen(_DEF_UART2, baud) == true)
    {
      uart_is_open = true;
      printf("uart open : %s, %d\n", port_name, baud);
    }
    else
    {
      printf("uart open fail\n");
    }
  }
  else
  {
    printf(".exe com1 115200 \n");
    return;
  }


  if (uart_is_open == false)
  {
    return;
  }

  while(1)
  {
    if (uartAvailable(_DEF_UART1) > 0)
    {
      uint8_t ch;

      ch = uartRead(_DEF_UART1);

      uartPutch(_DEF_UART2, ch);
    }

    if (uartAvailable(_DEF_UART2) > 0)
    {
      uartPutch(_DEF_UART1, uartRead(_DEF_UART2));
    }

  }
}
