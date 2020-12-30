/*
 * ap.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "ap.h"

void led_isr(void *arg){
  ledToggle(0);
  //delay(500);
  //ledOff(0);
}

void apInit(void){
  uartOpen(_DEF_UART1, 115200);
  swtimer_handle_t h_led_timer;

  h_led_timer = swtimerGetHandle();
  swtimerSet(h_led_timer, 500, LOOP_TIME, led_isr, NULL);
  swtimerStart(h_led_timer);
}

void apMain(void){
  while(1){
    if(buttonGetPressedEvent(0) == true){
      uartPrintf(_DEF_UART1, "PressedEvent\r\n");

    }

    if(buttonGetPressed(0) == true){
      uartPrintf(_DEF_UART1, "PressedTime : %d\r\n", buttonGetPressedTime(0));

    }
    if(buttonGetReleasedEvent(0) == true){
      uartPrintf(_DEF_UART1, "ReleasedEvent\r\n");

    }
    if(uartAvailable(_DEF_UART1) > 0){
      uartPrintf(_DEF_UART1, "rx : 0x%X\n", uartRead(_DEF_UART1));
    }
  }
}

