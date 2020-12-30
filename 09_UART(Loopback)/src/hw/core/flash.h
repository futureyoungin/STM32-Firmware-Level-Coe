/*
 * flash.h
 *
 *  Created on: 2020. 11. 26.
 *      Author: MCES
 */

#ifndef SRC_HW_CORE_FLASH_H_
#define SRC_HW_CORE_FLASH_H_

#endif /* SRC_HW_CORE_FLASH_H_ */

#include "hw_def.h"

void flashInit(void);

bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
