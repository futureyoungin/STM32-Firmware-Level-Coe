/*
 * cmd.h
 *
 *  Created on: 2020. 12. 27.
 *      Author: MCES
 */

#ifndef SRC_COMMON_CMD_CMD_H_
#define SRC_COMMON_CMD_CMD_H_



#endif /* SRC_COMMON_CMD_CMD_H_ */

#include "def.h"

#define CMD_MAX_DATA_LENGTH   2048

typedef struct
{
  uint32_t index;
  uint8_t  cmd;
  uint8_t  option;
  uint8_t  error;
  uint16_t length;
  uint8_t  check_sum;
  uint8_t  check_sum_recv;
  uint8_t  data[CMD_MAX_DATA_LENGTH];
} cmd_packet_t;


typedef struct
{
  uint8_t  ch;
  uint8_t  init;
  uint8_t  state;
  uint32_t baud;
  uint32_t save_time[2];

  cmd_packet_t tx_packet;
  cmd_packet_t rx_packet;
} cmd_t;



void cmdInit(cmd_t *p_cmd);

bool cmdBegin(cmd_t *p_cmd, uint8_t ch, uint32_t baud);
bool cmdReceivePacket(cmd_t *p_cmd);
void cmdSendCmd(cmd_t *p_cmd, uint8_t cmd, uint8_t *p_data, uint32_t length);
void cmdSendResp(cmd_t *p_cmd, uint8_t err_code, uint8_t *p_data, uint32_t length);
bool cmdSendCmdRxResp(cmd_t *p_cmd, uint8_t cmd, uint8_t *p_data, uint32_t length, uint32_t timeout);


