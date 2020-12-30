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

#define BUTTON_MAX_CH 1

// Initialize the GPIO PIN 13 for the button.
void buttonInit(void);


// it the button get pressed, return true. or not return false.
bool buttonGetPressed(uint8_t ch);


/* addition for testing swTimer */
void buttonResetTime(uint8_t ch);
bool buttonGetPressedEvent(uint8_t ch);
uint32_t buttonGetPressedTime(uint8_t ch);
bool buttonGetReleased(uint8_t ch);
bool buttonGetReleasedEvent(uint8_t ch);
uint32_t buttonGetReleasedTime(uint8_t ch);


