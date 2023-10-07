/*
 * rgb.h
 *
 *  Created on: Oct 7, 2023
 *      Author: ss
 */

#ifndef INC_RGB_H_
#define INC_RGB_H_
#include "main.h"
struct Color {
	int red, green, blue;
}typedef Color;
Color mkColor_trueHSV(int angle) ;
Color mkColor_powerHSV(int angle);
Color mkColor_sineLED(int angle);

#endif /* INC_RGB_H_ */
