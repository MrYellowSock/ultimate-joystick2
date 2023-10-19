/*
 * macro.c
 *
 *  Created on: Oct 17, 2023
 *      Author: ss
 */


// will use rgb to show state of the macro
#include "macro.h"

volatile Action buffer[MACRO_BUFFER_SIZE];

volatile uint8_t macroState  = 0;
volatile uint16_t recordBufferIndex = 0;
volatile uint16_t playBufferIndex = 0;

void recordAdd(uint8_t cmd[COMMAND_SIZE],uint16_t timespan){
	if(macroState == MACRO_STATE_RECORD){
		if(recordBufferIndex < MACRO_BUFFER_SIZE){
			memcpy(buffer[recordBufferIndex].command,cmd,COMMAND_SIZE);
			buffer[recordBufferIndex].timeSpan = timespan;
			recordBufferIndex++;
		}
		else{
			macroState = MACRO_STATE_NONE;
		}
	}
}

void recordStart(){
	if(macroState == MACRO_STATE_NONE){
		macroState = MACRO_STATE_RECORD;
		recordBufferIndex = 0;
	}
}

void recordStop(){
	if(macroState == MACRO_STATE_RECORD){
		macroState = MACRO_STATE_NONE;
	}
}



void playStart(){
	if(macroState == MACRO_STATE_NONE){
		playBufferIndex = 0;
		macroState = MACRO_STATE_PLAY;
	}
}

// force stop
void playStop(){
	if(macroState == MACRO_STATE_PLAY){
		macroState = MACRO_STATE_NONE;
	}
}

uint8_t getState(){
	return macroState;
}

Action nextAction(){
	if(macroState == MACRO_STATE_PLAY){
		if(playBufferIndex < MACRO_BUFFER_SIZE){
			return buffer[playBufferIndex++];
		}
		else{
			macroState = MACRO_STATE_NONE;
		}
	}
}
