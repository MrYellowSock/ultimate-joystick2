#include "button.h"
#define debounce 20
#define EXTI_COUNT 16
volatile uint8_t pinToButtonMapping[EXTI_COUNT] = { 13, 12, 14, 15, 4, 5, 3, 8, 10,
		1, 9, 0, 2, 13, 11, BUTTON_MACRO };

volatile uint8_t buttonState[EXTI_COUNT] = {0};
//PG0 PG1 PE2 PE3 PC6  PF7 PE8 PC9 PC10 PC11 PC12 PD14 PF15
const GPIO_TypeDef **portMapping[EXTI_COUNT] = {
		GPIOG,   //0
		GPIOG,	//1
		GPIOE,	//2
		GPIOE,		//3
		NULL,		//4
		NULL,		//5
		GPIOC,		//6
		GPIOF,		//7
		GPIOE,		//8
		GPIOC,		//9
		GPIOC,		//10
		GPIOC,		//11
		GPIOC,		//12
		NULL,		//13
		GPIOD,		//14
		GPIOF,		//15
		};

uint8_t toPortNumber(uint16_t GPIO_Pin) {
	static const uint16_t possiblePins[EXTI_COUNT] = { GPIO_PIN_0, GPIO_PIN_1,
			GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
			GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,
			GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, };
	for (uint8_t i = 0; i < 16; i++)
		if (GPIO_Pin == possiblePins[i])
			return i;
	return 255;
}
volatile uint8_t pinLastTick[EXTI_COUNT] = { 0 };
// pullup configuration
//volatile uint8_t pinLastState[EXTI_COUNT] = { 0 };

// also debounce event -> may return null
ButtonStateEvent onButtonPinINT(uint16_t GPIO_Pin) {
	uint8_t portNumber = toPortNumber(GPIO_Pin);
	uint32_t currentTick = HAL_GetTick();
	uint8_t currentState = HAL_GPIO_ReadPin(portMapping[portNumber], GPIO_Pin);
	ButtonStateEvent event;
	event.button = 0xFF;
	if((currentTick - buttonState[portNumber] > debounce) && (currentState != buttonState[portNumber])){
		//allowed
		event.button = pinToButtonMapping[portNumber];
		event.state = currentState;
		buttonState[portNumber] = currentState;
		pinLastTick[portNumber] = currentTick;
	}
	return event;
}
