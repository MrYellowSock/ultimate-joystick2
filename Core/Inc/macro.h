/*
 * macro.h
 *
 *  Created on: Oct 17, 2023
 *      Author: ss
 */

#ifndef INC_MACRO_H_
#define INC_MACRO_H_

#include "main.h"
#define MACRO_BUFFER_SIZE 100
#define MACRO_MAX_WAIT 10000 //10 second of no input shall terminates

#define MACRO_STATE_NONE 0
#define MACRO_STATE_RECORD 1
#define MACRO_STATE_PLAY 2
struct Action{
	uint8_t command[COMMAND_SIZE];
	uint16_t timeSpan;
} typedef Action;


#endif /* INC_MACRO_H_ */
