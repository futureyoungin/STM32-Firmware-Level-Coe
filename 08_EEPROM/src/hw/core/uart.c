/*
 * uart.c
 *
 *  Created on: 2020. 11. 6.
 *      Author: MCES
 */


#include <stdarg.h>    /* Use this header file when we want to remove the limit of the number of the arguments of the function.*/
#include <stdio.h>

#include "uart.h"
#include "core/qbuffer.h"

#define UART_MODE_POLLING   0   // Polling Mode   : check the changes of the hardware on and on.
#define UART_MODE_INTERRUPT 1   // Interrupt Mode : When check the changes, stop what you were doing and deal with the changes.(IT)
#define UART_MODE_DMA       2   // Direct Memory Access Mode :

#define UART_RX_BUF_LENGTH  16
#define UART_RX_QBUF_LENGTH 256


typedef struct{
  bool     is_open;
  uint32_t baud;
  uint8_t  tx_mode;
  uint8_t  rx_mode;

  uint8_t  rx_buf[UART_RX_BUF_LENGTH];

  qbuffer_t qbuffer_rx;

  UART_HandleTypeDef handle;                        // There is the struct definition in the "stm32f1xx_hal_uart.h"
}uart_t;


uart_t  uart_tbl[UART_MAX_CH];                           /* UART Declaration. */
uint8_t uart_rx_qbuf[UART_MAX_CH][UART_RX_BUF_LENGTH];  /* UART RX Q Buffer Declaration. */


static void uartStartRx(uint8_t channel);
static void uartRxHandler(uint8_t channel);

void uartInit(void)                               //Initialize the UART.
{
  uint8_t i;


  for (i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].rx_mode = UART_MODE_POLLING;
    uart_tbl[i].tx_mode = UART_MODE_POLLING;
  }
}



bool uartOpen(uint8_t channel, uint32_t baud){  // Open the UART of the channel.
  bool ret = true;
  uart_t *p_uart;
  switch(channel){
    case _DEF_UART1 :
      p_uart = &uart_tbl[channel];

      p_uart->handle.Instance        = USART2;
      p_uart->handle.Init.BaudRate   = baud;
      p_uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
      p_uart->handle.Init.StopBits   = UART_STOPBITS_1;
      p_uart->handle.Init.Parity     = UART_PARITY_NONE;
      p_uart->handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
      p_uart->handle.Init.Mode       = UART_MODE_TX_RX;

      if(HAL_UART_DeInit(&p_uart->handle) != HAL_OK){
        ret = false;
        break;
      }
      if(HAL_UART_Init(&p_uart->handle) != HAL_OK){
        ret = false;
        break;
      }
      qbufferCreate(&p_uart->qbuffer_rx, uart_rx_qbuf[channel], UART_RX_BUF_LENGTH);

      p_uart->rx_mode = UART_MODE_INTERRUPT;
      p_uart->baud    = baud;
      p_uart->is_open = true;

      uartStartRx(channel);
      break;
  }
  return ret;
}

void uartStartRx(uint8_t channel){

  uart_t *p_uart =  &uart_tbl[channel];     // channel to uart.

  if(p_uart->rx_mode == UART_MODE_INTERRUPT){
    HAL_UART_Receive_IT(&p_uart->handle, p_uart->rx_buf, 1);
  }
}

bool uartClose(uint8_t channel){
  bool ret = false;
  if(channel >= UART_MAX_CH){
    return false;
  }

  if(uart_tbl[channel].is_open == true){                          // if the uart of the channel is open, we can close.
    if(HAL_UART_DeInit(&uart_tbl[channel].handle) == HAL_OK){     // if the uart handler is deinitialized, return HAL_OK
      ret = true;
    }
  }
  return ret;
}

uint32_t uartAvailable(uint8_t channel){
  if(channel >= UART_MAX_CH)
    return 0;
  return qbufferAvailable(&uart_tbl[channel].qbuffer_rx);
}

void uartFlush(uint8_t channel){
  if(uart_tbl[channel].rx_mode == UART_MODE_INTERRUPT){
    qbufferFlush(&uart_tbl[channel].qbuffer_rx);
  }
}


void uartPutch(uint8_t channel, uint8_t ch){
  uartWrite(channel, &ch, 1);
}



uint8_t uartGetch(uint8_t channel){
  uint8_t ret = 0;

  while(1){
    if(uartAvailable(channel) > 0){
      ret = uartRead(channel);
      break;
    }
  }
  return ret;
}


int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length){
  int32_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];

  if(p_uart->rx_mode == UART_MODE_INTERRUPT){
    if( HAL_UART_Transmit(&p_uart->handle, (uint8_t*)p_data, length, 1000) == HAL_OK ){
      ret = length;
    }
  }
  return ret;
}

uint8_t uartRead(uint8_t channel){
  uint8_t ret = 0;
  uart_t *p_uart = &uart_tbl[channel];

  if(p_uart->rx_mode == UART_MODE_INTERRUPT){
    qbufferRead(&p_uart->qbuffer_rx, &ret, 1);
  }
  return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...){
  int32_t ret = 0;
  va_list arg;
  va_start(arg, fmt);
  int32_t len;
  char print_buffer[256];

  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end(arg);

  ret = uartWrite(channel, (uint8_t *)print_buffer, len);

  return ret;
}

void uartRxHandler(uint8_t channel)
{
  uart_t *p_uart = &uart_tbl[channel];


  if(p_uart->rx_mode == UART_MODE_INTERRUPT)
  {
      qbufferWrite(&p_uart->qbuffer_rx, &p_uart->rx_buf[0], 1);

      __HAL_UNLOCK(&p_uart->handle);
      uartStartRx(channel);
  }
}

void uartErrHandler(uint8_t channel)
{
  uartFlush(channel);
  uartStartRx(channel);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if (UartHandle->Instance == USART2)
  {
    uartRxHandler(_DEF_UART1);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  HAL_UART_DeInit(UartHandle);
  HAL_UART_Init(UartHandle);

  if (UartHandle->Instance == USART2)
  {
    uartErrHandler(_DEF_UART1);
  }
}

//-- _DEF_UART1 Interrupt
//
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart_tbl[_DEF_UART1].handle);
}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;


  if (huart->Instance == USART2)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();


    /* Enable USARTx clock */
    __HAL_RCC_USART2_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*##-3- Configure the NVIC for UART ########################################*/
    /* NVIC for USART */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    /*##-1- Reset peripherals ##################################################*/
    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure UART Tx as alternate function  */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
    /* Configure UART Rx as alternate function  */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

    /*##-3- Disable the NVIC for UART ##########################################*/
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
}

