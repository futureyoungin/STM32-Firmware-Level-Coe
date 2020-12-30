/*
 * qbuffer.h
 *
 *  Created on: 2020. 11. 5.
 *      Author: MCES
 */

#ifndef SRC_HW_CORE_QBUFFER_H_
#define SRC_HW_CORE_QBUFFER_H_



#endif /* SRC_HW_CORE_QBUFFER_H_ */

#include "def.h" // ?hw_def.h?

typedef struct
{
  uint32_t   ptr_in;    // Insert index
  uint32_t   ptr_out;   // Out    index
  uint32_t   length;    // Length of the buffer
  uint8_t   *p_buf;     // Buffer : data type uint8_t
} qbuffer_t;            // Buffer to be used by Receiver, RX Interrupt.

void qbufferInit(void);

bool     qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length);
bool     qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
bool     qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);
uint32_t qbufferAvailable(qbuffer_t *p_node);
void     qbufferFlush(qbuffer_t *p_node);
