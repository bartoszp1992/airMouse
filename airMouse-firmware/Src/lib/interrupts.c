/*
 * interrupts.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */


#include "interrupts.h"

volatile uint32_t ledCounter = 0;
volatile uint32_t onCounter = 0;
extern kbd_keyboard_t qwerty;
extern kbd_keyboard_t mouseButtons;
extern lsm6ds_sensor_t mems;


void systemTick(void) {
	ledCounter++;
	onCounter++;
	if (ledCounter % 2000 == 0) {
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, 1);
		onCounter = 0;
	}

	if (onCounter > 5)
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, 0);

	kbd_scanning(&qwerty);
	kbd_scanning(&mouseButtons);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GYRO_INT_Pin) {
		lsm6ds_flagDataReadySet(&mems);
	}
}
