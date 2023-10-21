/*
 * macro.c
 *
 *  Created on: Oct 17, 2023
 *      Author: ss
 */

// will use rgb to show state of the macro
#include "macro.h"

volatile Action buffer[MACRO_BUFFER_SIZE];

volatile uint8_t macroState = 0;
volatile uint16_t recordBufferIndex = 0;
volatile uint16_t playBufferIndex = 0;

uint8_t getMacroState() {
	return macroState;
}

void recordAdd(uint8_t cmd[COMMAND_SIZE]) {
	if (macroState == MACRO_STATE_RECORD) {
		if (recordBufferIndex < MACRO_BUFFER_SIZE) {
			memcpy(buffer[recordBufferIndex].command, cmd, COMMAND_SIZE);
			buffer[recordBufferIndex].timeSpan = HAL_GetTick();
			recordBufferIndex++;
		} else {
			macroState = MACRO_STATE_NONE;
		}
	}
}

void recordStart() {
	if (macroState == MACRO_STATE_NONE) {
		macroState = MACRO_STATE_RECORD;
		recordBufferIndex = 0;
	}
}

void recordStop() {
	if (macroState == MACRO_STATE_RECORD) {
		macroState = MACRO_STATE_NONE;
	}
}

void recordToggle() {
	// goes into recording mode.
	if (getMacroState() == MACRO_STATE_NONE)
		recordStart();
	else
		recordStop();
}

void playStart() {
	if (macroState == MACRO_STATE_NONE) {
		playBufferIndex = 0;
		macroState = MACRO_STATE_PLAY;
	}
}

// force stop
void playStop() {
	if (macroState == MACRO_STATE_PLAY) {
		macroState = MACRO_STATE_NONE;
	}
}

void playToggle() {
	// execute the macro
	if (getMacroState() == MACRO_STATE_NONE)
		playStart();
	else
		playStop();
}

Action nextAction() {
	if (macroState == MACRO_STATE_PLAY) {
		if (playBufferIndex < recordBufferIndex) {
			Action now = buffer[playBufferIndex];
			if (playBufferIndex == 0) {
				now.timeSpan = 0;
			} else {
				now.timeSpan -= buffer[playBufferIndex - 1].timeSpan;
			}
			playBufferIndex++;
			if(playBufferIndex >= recordBufferIndex)
				macroState = MACRO_STATE_NONE;
			return now;
		} else {
			macroState = MACRO_STATE_NONE;
		}
	}
}
