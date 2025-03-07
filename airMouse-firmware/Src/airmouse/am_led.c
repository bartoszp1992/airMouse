/*
 * led.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */


#include <am_led.h>

void ledOn(void) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 1);
}

void ledOff(void) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 0);
}
