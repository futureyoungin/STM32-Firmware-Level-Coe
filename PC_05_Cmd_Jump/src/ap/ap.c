/*
 * ap.c
 *
 *  Created on: 2020. 12. 29.
 *      Author: MCES
 */


#include "ap.h"
#include "cmd/cmd.h"


#define BOOT_CMD_READ_VERSION           0x00
#define BOOT_CMD_JUMP_TO_FW             0x08
#define BOOT_CMD_LED_CONTROL            0x10


cmd_t cmd_pc;



void apInit(void)
{
  uartOpen(_DEF_UART1, 115200);

  cmdInit(&cmd_pc);
}

void apMain(int argc, char *argv[])
{
  bool uart_is_open = false;
  char *port_name;
  int baud;


  if (argc >= 3)
  {
    port_name = argv[1];
    baud      = (int)strtoul((const char * ) argv[2], (char **)NULL, (int) 0);
    uartSetPortName(_DEF_UART2, port_name);


    if (cmdBegin(&cmd_pc, _DEF_UART2, baud) == true)
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

      if (ch == 'm')
      {
        uartPrintf(_DEF_UART1, "1. ReadVersion\n");
        uartPrintf(_DEF_UART1, "2. Led On\n");
        uartPrintf(_DEF_UART1, "3. Led Off\n");
        uartPrintf(_DEF_UART1, "4. JumpToFW\n");
        uartPrintf(_DEF_UART1, "m. Menu\n");
      }

      if (ch == '1')
      {
        bool ret;

        ret = cmdSendCmdRxResp(&cmd_pc, BOOT_CMD_READ_VERSION, NULL, 0, 100);

        if (ret == true)
        {
          uartPrintf(_DEF_UART1, "fw_version : ");
          for (int i=0; i<cmd_pc.rx_packet.length; i++)
          {
            uartPrintf(_DEF_UART1, "%c", cmd_pc.rx_packet.data[i]);
          }
          uartPrintf(_DEF_UART1, "\n");
        }
        else
        {
          uartPrintf(_DEF_UART1, "err_code : %d\n", cmd_pc.rx_packet.error);
        }
      }
      if (ch == '2')
      {
        bool ret;

        cmd_pc.tx_packet.data[0] = 1;
        ret = cmdSendCmdRxResp(&cmd_pc, BOOT_CMD_LED_CONTROL, cmd_pc.tx_packet.data, 1, 100);

        if (ret == false)
        {
          uartPrintf(_DEF_UART1, "err_code : %d\n", cmd_pc.rx_packet.error);
        }
      }
      if (ch == '3')
      {
        bool ret;

        cmd_pc.tx_packet.data[0] = 0;
        ret = cmdSendCmdRxResp(&cmd_pc, BOOT_CMD_LED_CONTROL, cmd_pc.tx_packet.data, 1, 100);

        if (ret == false)
        {
          uartPrintf(_DEF_UART1, "err_code : %d\n", cmd_pc.rx_packet.error);
        }
      }
      if (ch == '4')
      {
        bool ret;

        ret = cmdSendCmdRxResp(&cmd_pc, BOOT_CMD_JUMP_TO_FW, NULL, 0, 100);

        if (ret == false)
        {
          uartPrintf(_DEF_UART1, "err_code : %d\n", cmd_pc.rx_packet.error);
        }
        else
        {
          uartClose(_DEF_UART2);
          delay(1500);
          cmdBegin(&cmd_pc, _DEF_UART2, baud);
        }
      }
    }
  }
}




