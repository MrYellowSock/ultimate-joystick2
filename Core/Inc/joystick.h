/*
 * joystick.h
 *
 *  Created on: Oct 6, 2023
 *      Author: ss
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_
#include "main.h"
#define adc_channel_count 5
// 5 because LDR is now included.

struct StickStateEvent{
	int16_t x,y,Rx,Ry;
	uint8_t isChanged;

} typedef StickStateEvent;

StickStateEvent onSticksChanged(uint32_t adc_dma_result[adc_channel_count]);


#endif /* INC_JOYSTICK_H_ */
