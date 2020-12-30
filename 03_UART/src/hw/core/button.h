/*
 * button.h
 *
 *  Created on: 2020. 11. 5.
 *      Author: MCES
 */

#ifndef SRC_HW_CORE_BUTTON_H_
#define SRC_HW_CORE_BUTTON_H_



#endif /* SRC_HW_CORE_BUTTON_H_ */

#include "hw_def.h"


// Initialize the GPIO PIN 13 for the button.
void buttonInit(void);


// it the button get pressed, return true. or not return false.
bool buttonGetPressed(uint8_t ch);
