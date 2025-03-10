/*
 * led.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef LIB_LED_H_
#define LIB_LED_H_

#include "main.h"

enum{
	LED_GREEN,
	LED_BLUE,
	LED_RED
};

void ledOn(uint8_t led);
void ledOff(uint8_t led);

#endif /* LIB_LED_H_ */
