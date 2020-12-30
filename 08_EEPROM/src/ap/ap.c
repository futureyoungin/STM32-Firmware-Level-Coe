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
}


void apMain(void){
  while(1){
    if(uartAvailable(_DEF_UART1) > 0){
      uint8_t ch;
      ch = uartRead(_DEF_UART1);
      if(ch == 'h'){
        uartPrintf(_DEF_UART1, "h-> help\n");
        uartPrintf(_DEF_UART1, "w-> write eeprom\n");
        uartPrintf(_DEF_UART1, "r-> read  eeprom\n");
        uartPrintf(_DEF_UART1, "e-> erase eeprom\n");
        uartPrintf(_DEF_UART1, "\n");
      }
      else if(ch=='w'){
        runWriteEeprom();
      }
      else if(ch=='r'){
        runReadEeprom();
      }
      else if(ch=='e'){
        runEraseEeprom();
      }
    }
  }
}


bool runEraseEeprom(void){
  bool ret = true;
  uint8_t data[8] = {0,};

  uartPrintf(_DEF_UART1, "EraseEeprom..");
  if( eepromWrite(0,data,8) == true){
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else{
    uartPrintf(_DEF_UART1, "Fail\n");
    ret = false;
  }
  return ret;
}

bool runWriteEeprom(void){
  bool ret = true;
  uint8_t data;

  uartPrintf(_DEF_UART1, "WriteEeprom..");
  for(uint32_t i=0; i<8; i++){
    data = i;
    ret = eepromWrite(i, &data, 1);
    if(ret == false){
      break;
    }
  }
  if(ret == true){
    uartPrintf(_DEF_UART1, "OK\n");
  }
  else{
    uartPrintf(_DEF_UART1, "Fail\n");
  }
  return ret;
}

bool runReadEeprom(void)
{
  bool ret = true;
  uint8_t data;

  uartPrintf(_DEF_UART1, "ReadEeprom..\n");

  for (int i=0; i<8; i++)
  {
    eepromRead(i, &data, 1);
    uartPrintf(_DEF_UART1, "0x%02X : 0x%02X \n", i, data);
  }

  return ret;
}











