/*
 * led.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#include <led.h>

void ledOn(uint8_t led) {
	if (led == LED_GREEN)
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	else if (led == LED_RED)
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	else if (led == LED_BLUE)
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
}

void ledOff(uint8_t led) {
	if (led == LED_GREEN)
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	else if (led == LED_RED)
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	else if (led == LED_BLUE)
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
}
