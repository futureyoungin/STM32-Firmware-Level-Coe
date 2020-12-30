/*
 * ap.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "ap.h"
#include "cmd/cmd.h"

#define BOOT_CMD_READ_VERSION    0x00

void mcuLoop(void);
void pcLoop(void);

cmd_t cmd_mcu;
cmd_t cmd_pc;


void apInit(void){
  uartOpen(_DEF_UART1, 115200);

  cmdInit(&cmd_mcu);
  cmdBegin(&cmd_mcu, _DEF_UART2, 115200);

  cmdInit(&cmd_pc);
  cmdBegin(&cmd_pc, _DEF_UART2, 115200);

}


void apMain(void){
  while(1){
    mcuLoop();
    pcLoop();

  } //while

}


void mcuLoop(void)
{
  uint32_t addr;
  uint32_t data;


  if (cmdReceivePacket(&cmd_mcu) == true)
  {
    switch(cmd_mcu.rx_packet.cmd)
    {
      case BOOT_CMD_READ_VERSION:
        addr  = cmd_mcu.rx_packet.data[0]<<0;
        addr |= cmd_mcu.rx_packet.data[1]<<8;
        addr |= cmd_mcu.rx_packet.data[2]<<16;
        addr |= cmd_mcu.rx_packet.data[3]<<24;
        data  = cmd_mcu.rx_packet.data[4]<<0;
        data |= cmd_mcu.rx_packet.data[5]<<8;
        data |= cmd_mcu.rx_packet.data[6]<<16;
        data |= cmd_mcu.rx_packet.data[7]<<24;

        uartPrintf(_DEF_UART1, "mcu cmd received : addr %d, data %d\n", addr, data);
        break;
    }
  }
}

void pcLoop(void)
{
  static uint32_t addr = 1024;  //
  static uint32_t data = 0;


  if (uartAvailable(_DEF_UART1) > 0)
  {
    uint8_t ch;

    ch = uartRead(_DEF_UART1);

    if (ch == '0')
    {
      cmd_pc.tx_packet.data[0] = addr>>0;
      cmd_pc.tx_packet.data[1] = addr>>8;
      cmd_pc.tx_packet.data[2] = addr>>16;
      cmd_pc.tx_packet.data[3] = addr>>24;
      cmd_pc.tx_packet.data[4] = data>>0;
      cmd_pc.tx_packet.data[5] = data>>8;
      cmd_pc.tx_packet.data[6] = data>>16;
      cmd_pc.tx_packet.data[7] = data>>24;

      cmdSendCmd(&cmd_pc, BOOT_CMD_READ_VERSION, cmd_pc.tx_packet.data, 8);
      data++;
    }
  }
}
