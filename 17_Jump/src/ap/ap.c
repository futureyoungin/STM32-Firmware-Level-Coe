/*
 * ap.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "ap.h"
#include "cmd/cmd.h"


#define BOOT_CMD_READ_VERSION           0x00
#define BOOT_CMD_JUMP_TO_FW             0x08
#define BOOT_CMD_LED_CONTROL            0x10



void mcuLoop(cmd_t *p_cmd);
void mcuCmdReadVersion(cmd_t *p_cmd);
void mcuCmdJump(cmd_t *p_cmd);


cmd_t cmd_mcu;



void apInit(void)
{
  cmdInit(&cmd_mcu);
  cmdBegin(&cmd_mcu, _DEF_UART1, 115200);
}

void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {
    mcuLoop(&cmd_mcu);

    if (millis()-pre_time >= 100)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }
  }
}

void mcuLoop(cmd_t *p_cmd)
{


  if (cmdReceivePacket(p_cmd) == true)
  {
    switch(p_cmd->rx_packet.cmd)
    {
      case BOOT_CMD_READ_VERSION:
        mcuCmdReadVersion(p_cmd);
        break;

      case BOOT_CMD_JUMP_TO_FW:
        mcuCmdJump(p_cmd);
        break;
    }
  }
}

void mcuCmdReadVersion(cmd_t *p_cmd)
{
  char *fw_version = "B180824R1";


  p_cmd->tx_packet.data[0] = fw_version[0];
  p_cmd->tx_packet.data[1] = fw_version[1];
  p_cmd->tx_packet.data[2] = fw_version[2];
  p_cmd->tx_packet.data[3] = fw_version[3];
  p_cmd->tx_packet.data[4] = fw_version[4];
  p_cmd->tx_packet.data[5] = fw_version[5];
  p_cmd->tx_packet.data[6] = fw_version[6];
  p_cmd->tx_packet.data[7] = fw_version[7];
  p_cmd->tx_packet.data[8] = fw_version[8];

  cmdSendResp(p_cmd, OK, p_cmd->tx_packet.data, 9);
}

void mcuCmdJump(cmd_t *p_cmd)
{
  void (**jump_func)(void) = (void (**)(void))(0x8008000 + 4);


  if ((uint32_t)(*jump_func) != 0xFFFFFFFF)
  {
    cmdSendResp(p_cmd, OK, NULL, 0);
    delay(100);

    bspDeInit();
    (*jump_func)();
  }
  else
  {
    cmdSendResp(p_cmd, ERR_INVALID_FW, NULL, 0);
  }
}


