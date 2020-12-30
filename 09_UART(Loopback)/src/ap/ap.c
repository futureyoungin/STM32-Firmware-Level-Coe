/*
 * ap.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "ap.h"

uint32_t flash_addr = 0x08008000;
uint32_t flash_size = 96*1024;

bool runEraseEeprom(void);
bool runWriteEeprom(void);
bool runReadEeprom(void);

void led_isr(void *arg){
  ledToggle(0);
  //delay(500);
  //ledOff(0);
}


void apInit(void){
  uartOpen(_DEF_UART1, 115200);
  uartOpen(_DEF_UART2, 115200);
}


void apMain(void){
  while(1){
    if(uartAvailable(_DEF_UART1) > 0){
      uint8_t data;
      data = uartRead(_DEF_UART1);
      if (data == '1')
      {
          uartPrintf(_DEF_UART1, "uart ch1 trigger\n");
          uartPrintf(_DEF_UART2, "uart ch2 tx : 0x%X\n", data);
      }
    }

    if (uartAvailable(_DEF_UART2) > 0)
    {
      uint8_t data;
      data = uartRead(_DEF_UART2);
      uartPrintf(_DEF_UART1, "%c", data);
    }

  }

}









