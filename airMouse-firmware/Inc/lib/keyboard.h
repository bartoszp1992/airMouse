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

typedef enum{
	KBD_OK = 0,
	KBD_ERR = 1,
	KBD_NOT_ENOUGH_MEMORY = 2,
	KBD_TOO_MUCH_COLUMNS = 3,
	KBD_NOT_IN_SCANNING_MODE = 4,
	KBD_ROWS_OR_COLUMNS_NUMBER_CANT_BE_0 = 5
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

	//for slow-down scanning due to filters
	uint32_t prescaler;
	uint32_t masterTimer;

	/*state of pressed button. If buttons are pulled up
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
kbd_state_t kbd_set_columns(kbd_keyboard_t *keyboard, ...);
kbd_state_t kbd_set_rows(kbd_keyboard_t *keyboard, ...);
void kbd_scanning(kbd_keyboard_t *keyboard);

#endif /* KEYBOARD_H_ */
