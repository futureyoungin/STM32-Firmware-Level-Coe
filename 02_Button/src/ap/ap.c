/*
 * ap.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "ap.h"

void apInit(void){

}

void apMain(void){
  while(1){

    if(buttonGetPressed(0) == true){
      ledOn(0);
    }
    else{
      ledOff(0);
    }

   // ledToggle(0);
   // delay(500);
  }
}

