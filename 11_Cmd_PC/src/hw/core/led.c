/*
 * led.c
 *
 *  Created on: 2020. 9. 16.
 *      Author: MCES
 */


#include "led.h"

void ledInit(void){ //Setting the GPIO Pin to use the LED
  GPIO_InitTypeDef  GPIO_InitStruct;

  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_5; //LED Pin Number 5
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ledOn(uint8_t ch)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

void ledOff(uint8_t ch)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void ledToggle(uint8_t ch)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
