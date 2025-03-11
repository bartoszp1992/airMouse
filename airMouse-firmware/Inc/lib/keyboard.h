/*
 * keyboard.h
 *
 *  Created on: Mar 1, 2025
 *      Author: bartosz
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "gpio.h"
#include "stm32g0xx_hal_gpio.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


//modifier codes- for layout set.
#define KBD_MOD_ENTER 	0x01
#define KBD_MOD_LSHIFT  0x02
#define KBD_MOD_RSHIFT  0x03
#define KBD_MOD_UP      0x04
#define KBD_MOD_LCTRL   0x05
#define KBD_MOD_FN      0x06
#define KBD_MOD_LGUI    0x07
#define KBD_MOD_LALT    0x08
#define KBD_MOD_RALT    0x09
#define KBD_MOD_LEFT    0x0a
#define KBD_MOD_DOWN    0x0b
#define KBD_MOD_RIGHT   0x0c

typedef enum{
	KBD_OK = 0,
	KBD_ERR = 1,
	KBD_NOT_ENOUGH_MEMORY = 2,
	KBD_TOO_MUCH_COLUMNS = 3,
	KBD_NOT_IN_SCANNING_MODE = 4,
	KBD_ROWS_OR_COLUMNS_NUMBER_CANT_BE_0 = 5,
	KBD_ANY_PRESSED = 6
}kbd_state_t;

typedef enum{
	KBD_RESET = 0,
	KBD_SET = 1
}kbd_pinState_t;

//level on actual scanner row. on other rows level will be opposite
//thats also level of pressed button checked on corresponding column pin
#define KBD_SCAN_ROW_STATE KBD_RESET


//hardware specific pin struct //port
#define kbd_port_t GPIO_TypeDef
typedef struct{
	kbd_port_t *port;
	uint32_t pin;
}kbd_pin_t;

typedef struct{

	//kbd pins arrays
	kbd_pin_t *columns; //GPI
	kbd_pin_t *rows; //GPO

	uint8_t numberOfColumns;
	uint8_t numberOfRows;
	uint8_t numberOfKeys;
	uint8_t actualScannedRow;

	/*
	 * stores keyboard layout as char 2-dimensional array
	 */
	char *layout;

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
	uint32_t stateMatrixSize;


}kbd_keyboard_t;

kbd_state_t kbd_init(kbd_keyboard_t *keyboard, uint8_t columns, uint8_t rows,
		uint32_t prescaler, kbd_state_t pressedState);
void kbd_setLayout(kbd_keyboard_t *keyboard, ...);
kbd_state_t kbd_setColumns(kbd_keyboard_t *keyboard, ...);
kbd_state_t kbd_setRows(kbd_keyboard_t *keyboard, ...);
uint32_t kbd_readRow(kbd_keyboard_t *keyboard, uint8_t row);
kbd_state_t kbd_readFromLayout(kbd_keyboard_t *keyboard, char *buttonsArray);
void kbd_scanning(kbd_keyboard_t *keyboard);

#endif /* KEYBOARD_H_ */
