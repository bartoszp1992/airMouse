/*
 * am_keys.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 *
 *      this module is responsible for converting button presses into reports for AT+BLEHID
 *
 */

#include "am_keys.h"

uint8_t amKeys_reportMouseButton;
int8_t amKeys_reportWheel;
uint8_t amKeys_modifiers;
char amKeys_keys[6];

uint32_t test;


/*
 * read mouse keys and wheel
 * @param: none
 *
 * @retval: state change- 1 if previous buttons status was changed
 */
uint8_t amKeys_readMouse(void) {


	uint8_t lastReportBackup = amKeys_reportMouseButton;

	amKeys_reportMouseButton = 0;
	amKeys_reportWheel = 0;
	uint32_t rawKeyboardReading = kbd_readRow(&mouseButtons, 0);

	if (rawKeyboardReading & KEY_MOUSE_L)
		amKeys_reportMouseButton |= HID_MOUSE_MASK_L;
	if (rawKeyboardReading & KEY_MOUSE_R)
		amKeys_reportMouseButton |= HID_MOUSE_MASK_R;
	if (rawKeyboardReading & KEY_MOUSE_M)
		amKeys_reportMouseButton |= HID_MOUSE_MASK_M;
	if (rawKeyboardReading & KEY_MOUSE_FWD)
		amKeys_reportMouseButton |= HID_MOUSE_MASK_FWD;
	if (rawKeyboardReading & KEY_MOUSE_BCK)
		amKeys_reportMouseButton |= HID_MOUSE_MASK_BCK;

	if (rawKeyboardReading & KEY_MOUSE_WHEELUP)
		amKeys_reportWheel = 1;
	if (rawKeyboardReading & KEY_MOUSE_WHEELDN)
		amKeys_reportWheel = -1;

	if (amKeys_reportMouseButton != lastReportBackup)
		return 1;
	else
		return 0;

}

void amKeys_readWheel(void) {
//	if(HAL_GPIO_)
}
void amKeys_readModifiers(void) {
	amKeys_modifiers = 0;

}
