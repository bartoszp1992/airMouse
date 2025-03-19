/*
 * led.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#include <led.h>

blink_t ledRed;
blink_t ledGreen;
blink_t ledBlue;

void led_init(void){

	blink_init(&ledRed, LED_RED_GPIO_Port, LED_RED_Pin, 100);
	blink_init(&ledGreen, LED_GREEN_GPIO_Port, LED_GREEN_Pin, 100);
	blink_init(&ledBlue, LED_BLUE_GPIO_Port, LED_BLUE_Pin, 100);

	blink_enable(&ledGreen, BLINK_PATTERN_SLOW_FAST, BLINK_MODE_SINGLE);


}

