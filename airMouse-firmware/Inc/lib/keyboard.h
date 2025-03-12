/*
 * keyboard.h
 *
 *  Created on: Mar 1, 2025
 *      Author: bartosz
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <key_codes.h>
#include "gpio.h"
#include "stm32g0xx_hal_gpio.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>



#define KBD_MAX_PRESSED_BUTTONS 7

//hardware specific pin struct //port
#define kbd_port_t GPIO_TypeDef
typedef struct{
	kbd_port_t *port;
	uint32_t pin;
}kbd_pin_t;

//return status
typedef enum{
	KBD_OK = 0,
	KBD_ERR = 1,
	KBD_NOT_ENOUGH_MEMORY = 2,
	KBD_TOO_MUCH_COLUMNS = 3,
	KBD_NOT_IN_SCANNING_MODE = 4,
	KBD_ROWS_OR_COLUMNS_NUMBER_CANT_BE_0 = 5,
	KBD_ANY_PRESSED = 6
}kbd_state_t;

//logic level states
typedef enum{
	KBD_RESET = 0,
	KBD_SET = 1
}kbd_pinState_t;

//columns order
typedef enum{
	KBD_COLUMN_REVERSE_DIS,
	KBD_COLUMN_REVERSE_EN
}kbd_columnReverse_t;


/*
 * layout key types for layout[*].type
 * stores type of assigned key
 */
typedef enum{
	KBD_KEY_TYPE_RELEASED,
	KBD_KEY_TYPE_CHAR, //store characters, like 'b'
	KBD_KEY_TYPE_HIDCODE, //hidcodes, like 0x05
	KBD_KEY_TYPE_BITSHIFT, //bitshift's for mod keys/mouse buttons- becouse modkeys are sent as bit states in one byte
	KBD_KEY_TYPE_WHEEL,//for mouse encoder
	KBD_KEY_TYPE_INTERNAL //internal type keys, like sleep or something, not for send outside MCU, like FN key
}kbd_keyType_t;

//layout key entry
typedef struct{
	union{
		uint8_t bitshift;
		uint8_t internal;
		char character;
		int8_t code;
		int8_t wheel;
	};
	kbd_keyType_t type;
}kbd_key_t;

typedef struct{

	//kbd pins arrays
	kbd_pin_t *columns; //GPI
	kbd_pin_t *rows; //GPO

	uint8_t numberOfColumns;
	uint8_t numberOfRows;
	uint8_t numberOfKeys;
	uint8_t actualScannedRow;

	/*
	 * stores keyboard layout- one element in size of kbd_key_t for one key. Value/character and type
	 */
	kbd_key_t *layoutTable;


	//for slow-down scanning due to filters
	uint32_t prescaler;
	uint32_t masterTimer;

	/*state of pressed button. If buttons are pulled up:
	 *-> depressed button are KBD_SET
	 *-> pressed button are KBD_RESET
	 *-> actual scanning row will be set to KBD_RESET
	 *-> not scanned rows will be set to KBD_SET
	*/
	kbd_pinState_t pressedState;

	/*
	 * this field allows you to reverse column order
	 */
	kbd_columnReverse_t reverse;

	/*
	 * matrix for store actual keybaord status. Init function will malloc memory
	 * to assign each row as stateMatrix[x] element.
	 * each stateMatrix[] element are the binary representation of actual row state
	 * e.g. for 4*3 keyboard:
	 *
	 * stateMatrix[0] = 0b0000
	 * stateMatrix[1] = 0b0010 <<pressed SECOND button
	 * stateMatrix[2] = 0b0000
	 * stateMatrix[3] = 0b0000
	 */
	uint32_t *stateMatrix;


}kbd_keyboard_t;

kbd_state_t kbd_init(kbd_keyboard_t *keyboard, uint8_t columns, uint8_t rows,
		uint32_t prescaler, kbd_state_t pressedState, kbd_columnReverse_t reverse);
void kbd_setLayout(kbd_keyboard_t *keyboard, ...);
kbd_state_t kbd_setColumns(kbd_keyboard_t *keyboard, ...);
kbd_state_t kbd_setRows(kbd_keyboard_t *keyboard, ...);
uint32_t kbd_readRow(kbd_keyboard_t *keyboard, uint8_t row);
kbd_state_t kbd_readFromLayout(kbd_keyboard_t *keyboard, kbd_key_t* pressedKeys);
void kbd_scanning(kbd_keyboard_t *keyboard);

#endif /* KEYBOARD_H_ */
