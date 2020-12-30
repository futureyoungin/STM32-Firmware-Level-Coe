/*
 * hw_def.h
 *
 *  Created on: 2020. 12. 29.
 *      Author: MCES
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#endif /* SRC_HW_HW_DEF_H_ */

#include "def.h"
#include "bsp.h"


#define log_printf(fmt, ...)        printf(fmt, __VA_ARGS__)


#define FLASH_FW_SIZE               (96*1024)  // 96KB
#define FLASH_FW_ADDR_START         0x08008000
#define FLASH_FW_ADDR_END           (FLASH_FW_ADDR_START + FLASH_FW_SIZE)

#define FLASH_TX_BLOCK_LENGTH       (1024)
