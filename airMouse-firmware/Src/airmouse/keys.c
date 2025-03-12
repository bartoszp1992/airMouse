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

//keyboards structures
kbd_keyboard_t qwerty;
kbd_keyboard_t mouseButtons;

//mouse
kbd_key_t keys_mousePressedOn[KBD_MAX_PRESSED_BUTTONS];
uint8_t keys_mouseReportButton;
uint8_t keys_mouseReportButtonPrevious;
uint8_t keys_mouseFlagSendReport = 0;
uint8_t keys_mouseButtonsChanged = 0;
int8_t keys_mouseReportWheel;

//keyboard
kbd_key_t keys_qwertyPressedOn[KBD_MAX_PRESSED_BUTTONS];
uint8_t keys_qwertyReportModifiers;
uint8_t keys_qwertyReportModifiersPrevious;
int8_t keys_qwertyReportKeys[KEYS_MAX_KEYS];
int8_t keys_qwertyReportKeysPrevious[KEYS_MAX_KEYS];
uint8_t keys_qwertyFlagSendReport = 0;
uint8_t keys_qwertyKeysChanged = 0;

/*
 * initialize keyboard and mouse keys
 */
void keys_init(void) {
	//init mouse buttons
	kbd_init(&mouseButtons, 10, 1, 8, KBD_RESET, KBD_COLUMN_REVERSE_DIS);
	kbd_setColumns(&mouseButtons,                           //
			MUS_LB_GPIO_Port, MUS_LB_Pin,                           //
			MUS_RB_GPIO_Port, MUS_RB_Pin,                           //
			MUS_MB_GPIO_Port, MUS_MB_Pin,                           //
			MUS_FWD_GPIO_Port, MUS_FWD_Pin,                         //
			MUS_BCK_GPIO_Port, MUS_BCK_Pin,                         //
			MUS_UP_GPIO_Port, MUS_UP_Pin,                           //
			MUS_DN_GPIO_Port, MUS_DN_Pin,                           //
			MUS_DPI_GPIO_Port, MUS_DPI_Pin,                         //
			MUS_HOME_GPIO_Port, MUS_HOME_Pin,                       //
			MUS_PRC_GPIO_Port, MUS_PRC_Pin                          //
			);
	kbd_setLayout(&mouseButtons,                            //
			KBD_KEY_TYPE_BITSHIFT, HID_MOUSE_MASK_L,        //
			KBD_KEY_TYPE_BITSHIFT, HID_MOUSE_MASK_R,        //
			KBD_KEY_TYPE_BITSHIFT, HID_MOUSE_MASK_M,        //
			KBD_KEY_TYPE_BITSHIFT, HID_MOUSE_MASK_FWD,      //
			KBD_KEY_TYPE_BITSHIFT, HID_MOUSE_MASK_BCK,      //
			KBD_KEY_TYPE_WHEEL, HID_MOUSE_WHEELUP,          //
			KBD_KEY_TYPE_WHEEL, HID_MOUSE_WHEELDN,          //
			KBD_KEY_TYPE_INTERNAL, HID_INTERNAL_DPI,        //
			KBD_KEY_TYPE_INTERNAL, HID_INTERNAL_HOME,       //
			KBD_KEY_TYPE_INTERNAL, HID_INTERNAL_PRC         //
			);

	//init qwerty
	kbd_init(&qwerty, 10, 5, 8, KBD_RESET, KBD_COLUMN_REVERSE_EN);
	kbd_setColumns(&qwerty,               //
			KBD_COL1_GPIO_Port, KBD_COL1_Pin,     //
			KBD_COL2_GPIO_Port, KBD_COL2_Pin,     //
			KBD_COL3_GPIO_Port, KBD_COL3_Pin,     //
			KBD_COL4_GPIO_Port, KBD_COL4_Pin,     //
			KBD_COL5_GPIO_Port, KBD_COL5_Pin,     //
			KBD_COL6_GPIO_Port, KBD_COL6_Pin,     //
			KBD_COL7_GPIO_Port, KBD_COL7_Pin,     //
			KBD_COL8_GPIO_Port, KBD_COL8_Pin,     //
			KBD_COL9_GPIO_Port, KBD_COL9_Pin,     //
			KBD_COL10_GPIO_Port, KBD_COL10_Pin);  //
	kbd_setRows(&qwerty,                  //
			KBD_ROW1_GPIO_Port, KBD_ROW1_Pin,     //
			KBD_ROW2_GPIO_Port, KBD_ROW2_Pin,     //
			KBD_ROW3_GPIO_Port, KBD_ROW3_Pin,     //
			KBD_ROW4_GPIO_Port, KBD_ROW4_Pin,     //
			KBD_ROW5_GPIO_Port, KBD_ROW5_Pin);    //
	kbd_setLayout(&qwerty,                                      //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_1,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_2,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_3,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_4,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_5,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_6,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_7,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_8,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_9,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_0,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_Q,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_W,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_E,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_R,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_T,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_Y,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_U,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_I,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_O,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_P,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_A,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_S,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_D,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_F,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_G,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_H,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_J,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_K,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_L,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_ENTER,            //
			KBD_KEY_TYPE_BITSHIFT, HID_MOD_MASK_LSHIFT,     //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_Z,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_X,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_C,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_V,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_B,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_N,                //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_M,                //
			KBD_KEY_TYPE_BITSHIFT, HID_MOD_MASK_RSHIFT,     //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_UP,               //
			KBD_KEY_TYPE_BITSHIFT, HID_MOD_MASK_LCTRL,      //
			KBD_KEY_TYPE_INTERNAL, HID_INTERNAL_FN,         //
			KBD_KEY_TYPE_BITSHIFT, HID_MOD_MASK_LGUI,       //
			KBD_KEY_TYPE_BITSHIFT, HID_MOD_MASK_LALT,       //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_SPACE,            //
			KBD_KEY_TYPE_NONE, 0,                           //
			KBD_KEY_TYPE_BITSHIFT, HID_MOD_RALT,            //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_LEFT,             //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_RIGHT,            //
			KBD_KEY_TYPE_HIDCODE, HID_KEY_DOWN              //
			);
}

/*
 * read mouse keys and wheel
 * @param: none
 *
 * @retval: state change- 1 if previous buttons status was changed
 */
void keys_readMouse(void) {

	keys_mouseReportButton = 0;
	keys_mouseReportWheel = 0;

	kbd_readFromLayout(&mouseButtons, keys_mousePressedOn);

	for (uint8_t i = 0; i < KBD_MAX_PRESSED_BUTTONS; i++) {

		if (keys_mousePressedOn[i].type == KBD_KEY_TYPE_BITSHIFT)
			keys_mouseReportButton |= keys_mousePressedOn[i].bitshift;

		else if (keys_mousePressedOn[i].type == KBD_KEY_TYPE_WHEEL)
			keys_mouseReportWheel = keys_mousePressedOn[i].wheel;
	}

}

void keys_readQwerty(void) {

	memset(keys_qwertyReportKeys, 0, sizeof(keys_qwertyReportKeys));
	keys_qwertyReportModifiers = 0;

	kbd_readFromLayout(&qwerty, keys_qwertyPressedOn);

	uint8_t keysCounter = 0;

	for (uint8_t i = 0; i < KBD_MAX_PRESSED_BUTTONS; i++) {

		if (keys_qwertyPressedOn[i].type == KBD_KEY_TYPE_BITSHIFT)
			keys_qwertyReportModifiers |= keys_qwertyPressedOn[i].bitshift;

		else if (keys_qwertyPressedOn[i].type == KBD_KEY_TYPE_HIDCODE) {
			if (keysCounter < KEYS_MAX_KEYS) {
				keys_qwertyReportKeys[keysCounter] =
						keys_qwertyPressedOn[i].code;
				keysCounter++;
			}

		}

	}

}
