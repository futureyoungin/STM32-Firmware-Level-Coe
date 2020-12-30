/*
 * uart.c
 *
 *  Created on: 2020. 12. 29.
 *      Author: MCES
 */


#include "uart.h"
#include <conio.h>





typedef struct
{
  bool     is_open;
  bool     is_consol;
  uint32_t baud;

  HANDLE   serial_handle;
  char     port_name[256];
} uart_t;



uart_t   uart_tbl[UART_MAX_CH];




void uartInit(void)
{
  uint8_t i;


  for (i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open   = false;
    uart_tbl[i].is_consol = false;
  }
}

bool uartOpen(uint8_t channel, uint32_t baud)
{
  bool ret = false;
  uart_t *p_uart;

  switch(channel)
  {
    case _DEF_UART1:
      p_uart = &uart_tbl[channel];

      p_uart->baud      = baud;
      p_uart->is_open   = true;
      p_uart->is_consol = true;
      break;
  }

  return ret;
}

bool uartClose(uint8_t channel)
{
  bool ret = false;
  uart_t *p_uart = &uart_tbl[channel];


  if (channel >= UART_MAX_CH)
  {
    return false;
  }
  if (uart_tbl[channel].is_open == false)
  {
    return false;
  }

  if (p_uart->is_consol == true)
  {

  }
  else
  {
    CloseHandle(p_uart->serial_handle);
    p_uart->is_open = false;
  }


  return ret;
}

uint32_t uartAvailable(uint8_t channel)
{
  uint32_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];


  if (channel >= UART_MAX_CH)
  {
    return 0;
  }
  if (p_uart->is_open == false)
  {
    return 0;
  }


  if (p_uart->is_consol == true)
  {
    if(kbhit())
    {
      ret = 1;
    }
  }
  else
  {

  }

  return ret;
}

void uartFlush(uint8_t channel)
{
}

void uartPutch(uint8_t channel, uint8_t ch)
{
  uartWrite(channel, &ch, 1 );
}

uint8_t uartGetch(uint8_t channel)
{
  uint8_t ret = 0;


  if (channel >= UART_MAX_CH)
  {
    return 0;
  }
  if (uart_tbl[channel].is_open == false)
  {
    return 0;
  }


  while(1)
  {
    if (uartAvailable(channel) > 0)
    {
      ret = uartRead(channel);
      break;
    }
  }

  return ret;
}

int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
  int32_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];


  if (channel >= UART_MAX_CH)
  {
    return 0;
  }
  if (p_uart->is_open == false)
  {
    return 0;
  }


  if (p_uart->is_consol == true)
  {
    for (int i=0; i<length; i++)
    {
      putch(p_data[i]);
    }
  }
  else
  {

  }

  return ret;
}

uint8_t uartRead(uint8_t channel)
{
  uint8_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];


  if (channel >= UART_MAX_CH)
  {
    return 0;
  }
  if (p_uart->is_open == false)
  {
    return 0;
  }


  if (p_uart->is_consol == true)
  {
    ret = getch();
  }
  else
  {

  }

  return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  ret = uartWrite(channel, (uint8_t *)print_buffer, len);

  return ret;
}
