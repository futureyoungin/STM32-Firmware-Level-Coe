/*
 * ap.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "ap.h"

void apInit(void){
  uartOpen(_DEF_UART1, 115200);
}

void apMain(void){
  while(1){

    if(buttonGetPressed(0) == true){
      ledOn(0);
    }
    else{
      ledOff(0);
    }
    if(uartAvailable(_DEF_UART1) > 0){
      uartPrintf(_DEF_UART1, "rx : 0x%X\n", uartRead(_DEF_UART1));
    }
  }
}

