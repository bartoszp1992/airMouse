/*
 * am_keys.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#include "am_keys.h"

uint8_t amKeys_reportMouseButton;
int8_t amKeys_reportWheel;
uint8_t amKeys_modifiers;
char amKeys_keys[6];

uint32_t test;

void amKeys_readMouse(void) {
	amKeys_reportMouseButton = 0;
	amKeys_reportMouseButton = !HAL_GPIO_ReadPin(MUS_LB_GPIO_Port, MUS_LB_Pin) << HID_MOUSE_BITSHIFT_L
			| !HAL_GPIO_ReadPin(MUS_RB_GPIO_Port, MUS_RB_Pin) << HID_MOUSE_BITSHIFT_R
			| !HAL_GPIO_ReadPin(MUS_MB_GPIO_Port, MUS_MB_Pin) << HID_MOUSE_BITSHIFT_M
			| !HAL_GPIO_ReadPin(MUS_FWD_GPIO_Port, MUS_FWD_Pin) << HID_MOUSE_BITSHIFT_FWD
			| !HAL_GPIO_ReadPin(MUS_BCK_GPIO_Port, MUS_BCK_Pin) << HID_MOUSE_BITSHIFT_BCK;

	test = kbd_readRow(&mouseButtons, 1);
//	if(amKeys_reportMouseButton )
}

void amKeys_readWheel(void){
//	if(HAL_GPIO_)
}
void amKeys_readModifiers(void){
	amKeys_modifiers = 0;

}
