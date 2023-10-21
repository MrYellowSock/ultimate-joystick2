/*
 * button.h
 *
 *  Created on: Oct 4, 2023
 *      Author: ss
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"

#define BUTTON_MACRO 99
struct ButtonStateEvent {
	// joystick button number , mapped by pinToButtonMapping
	uint8_t button;
	uint8_t state;
}typedef ButtonStateEvent;
ButtonStateEvent onButtonPinINT(uint16_t GPIO_Pin);

#endif /* INC_BUTTON_H_ */
