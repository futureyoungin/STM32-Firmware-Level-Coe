/*
 * hw.c
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */


#include "hw.h"


void hwInit(void)
{
  bspInit();

  ledInit();
  buttonInit();
}

void delay(uint32_t time_ms)
{
  HAL_Delay(time_ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}
