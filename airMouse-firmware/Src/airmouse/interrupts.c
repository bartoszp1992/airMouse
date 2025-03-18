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

extern volatile uint32_t prCounterMouse;
extern volatile uint32_t prCounterKeyboard;

extern volatile uint32_t prMouse;
extern volatile uint32_t prKeyboard;
extern volatile uint32_t prMasterCounter;

extern blink_t ledRed;
extern blink_t ledGreen;
extern blink_t ledBlue;


void systemTick(void) {

	kbd_scanning(&qwerty);
	kbd_scanning(&mouseButtons);
	sleep_timerInc();

	blink_tick(&ledRed);
	blink_tick(&ledGreen);
	blink_tick(&ledBlue);

	//polling rate counting
	prMasterCounter++;
	if(prMasterCounter % 1000 == 0){
		prKeyboard = prCounterKeyboard;
		prMouse = prCounterMouse;

		prCounterKeyboard = 0;
		prCounterMouse = 0;
	}

}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GYRO_INT_Pin) {
		lsm6ds_flagDataReadySet(&mems);
	}
}
