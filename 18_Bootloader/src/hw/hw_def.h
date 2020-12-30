/*
 * hw_def.h
 *
 *  Created on: 2020. 9. 14.
 *      Author: MCES
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#endif /* SRC_HW_HW_DEF_H_ */

#include "def.h"
#include "bsp.h"

#define _USE_HW_EEPROM  /* We will use EEPROM Hardware */

#define FLASH_FW_SIZE                     (96*1024)  // 96KB
#define FLASH_FW_ADDR_START               0x08008000
#define FLASH_FW_ADDR_END                 (FLASH_FW_ADDR_START + FLASH_FW_SIZE)


/* The functions in the hw.h */
extern void delay(uint32_t time_ms);
extern uint32_t millis(void);
