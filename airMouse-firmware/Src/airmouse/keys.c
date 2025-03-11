/*
 * am_keys.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 *
 *      this module is responsible for converting button presses into reports for AT+BLEHID
 *
 */

#include <keys.h>

kbd_keyboard_t qwerty;
kbd_keyboard_t mouseButtons;

uint8_t keys_reportMouseButton;
uint8_t keys_reportMouseButtonPrevious;

uint8_t keys_flagMouseSendReport = 0;
uint8_t keys_buttonsChanged = 0;

int8_t keys_reportWheel;

uint8_t keys_modifiers;
char keys_readQwertyKeys[7];

/*
 * initialize keyboard and mouse keys
 */
void keys_init(void) {
	//init mouse buttons
	kbd_init(&mouseButtons, 10, 1, 10, KBD_RESET);
	kbd_setColumns(&mouseButtons,
	MUS_LB_GPIO_Port, MUS_LB_Pin,
	MUS_RB_GPIO_Port, MUS_RB_Pin,
	MUS_MB_GPIO_Port, MUS_MB_Pin,
	MUS_FWD_GPIO_Port, MUS_FWD_Pin,
	MUS_BCK_GPIO_Port, MUS_BCK_Pin,
	MUS_UP_GPIO_Port, MUS_UP_Pin,
	MUS_DN_GPIO_Port, MUS_DN_Pin,
	MUS_DPI_GPIO_Port, MUS_DPI_Pin,
	MUS_HOME_GPIO_Port, MUS_HOME_Pin,
	MUS_PRC_GPIO_Port, MUS_PRC_Pin);

	//init qwerty
	kbd_init(&qwerty, 10, 5, 8, KBD_RESET);
	kbd_setColumns(&qwerty,
	KBD_COL1_GPIO_Port, KBD_COL1_Pin,
	KBD_COL2_GPIO_Port, KBD_COL2_Pin,
	KBD_COL3_GPIO_Port, KBD_COL3_Pin,
	KBD_COL4_GPIO_Port, KBD_COL4_Pin,
	KBD_COL5_GPIO_Port, KBD_COL5_Pin,
	KBD_COL6_GPIO_Port, KBD_COL6_Pin,
	KBD_COL7_GPIO_Port, KBD_COL7_Pin,
	KBD_COL8_GPIO_Port, KBD_COL8_Pin,
	KBD_COL9_GPIO_Port, KBD_COL9_Pin,
	KBD_COL10_GPIO_Port, KBD_COL10_Pin);
	kbd_setRows(&qwerty,
	KBD_ROW1_GPIO_Port, KBD_ROW1_Pin,
	KBD_ROW2_GPIO_Port, KBD_ROW2_Pin,
	KBD_ROW3_GPIO_Port, KBD_ROW3_Pin,
	KBD_ROW4_GPIO_Port, KBD_ROW4_Pin,
	KBD_ROW5_GPIO_Port, KBD_ROW5_Pin);

	//layout qwerty
	kbd_setLayout(&qwerty, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
			'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd',
			'f', 'g', 'h', 'j', 'k', 'l', KBD_MOD_ENTER,
			KBD_MOD_LSHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', KBD_MOD_RSHIFT,
			KBD_MOD_UP,
			KBD_MOD_LCTRL, KBD_MOD_FN, KBD_MOD_LGUI, KBD_MOD_LALT, ' ',
			KBD_MOD_RALT, KBD_MOD_LEFT, KBD_MOD_RIGHT, KBD_MOD_DOWN);
}

/*
 * read mouse keys and wheel
 * @param: none
 *
 * @retval: state change- 1 if previous buttons status was changed
 */
void keys_readMouse(void) {

	keys_reportMouseButton = 0;
	keys_reportWheel = 0;
	uint32_t rawKeyboardReading = kbd_readRow(&mouseButtons, 0);

	if (rawKeyboardReading & KEY_MOUSE_L)
		keys_reportMouseButton |= HID_MOUSE_MASK_L;
	if (rawKeyboardReading & KEY_MOUSE_R)
		keys_reportMouseButton |= HID_MOUSE_MASK_R;
	if (rawKeyboardReading & KEY_MOUSE_M)
		keys_reportMouseButton |= HID_MOUSE_MASK_M;
	if (rawKeyboardReading & KEY_MOUSE_FWD)
		keys_reportMouseButton |= HID_MOUSE_MASK_FWD;
	if (rawKeyboardReading & KEY_MOUSE_BCK)
		keys_reportMouseButton |= HID_MOUSE_MASK_BCK;

	if (rawKeyboardReading & KEY_MOUSE_WHEELUP)
		keys_reportWheel = 1;
	if (rawKeyboardReading & KEY_MOUSE_WHEELDN)
		keys_reportWheel = -1;

}

void keys_readModifiers(void) {
	keys_modifiers = 0;

}

void keys_readKeyboard(void) {

	//qwerty read test
	kbd_readFromLayout(&qwerty, keys_readQwertyKeys);

}
