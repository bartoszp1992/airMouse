/*
 * keyboard.c
 *
 *  Created on: Mar 1, 2025
 *      Author: bartosz
 *
 *      this library allows to create specific keyboard.
 *      You can read pressed buttons reading stateMatrix field from keyboard struct.
 *      Each element of stateMatrix array stores bit states of pressed keys in whole row
 *      One stateMatrix element->one row
 */

#include "keyboard.h"

kbd_pinState_t kbd_readPin(kbd_pin_t *pin) {
	if (HAL_GPIO_ReadPin(pin->port, pin->pin) == GPIO_PIN_RESET)
		return KBD_RESET;
	else
		return KBD_SET;
}

void kbd_writePin(kbd_pin_t *pin, kbd_pinState_t state) {
	if (state == KBD_RESET)
		HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_SET);
}

/*
 * init keyboard. before use create kbd_keyboard_t element
 * @param: keyboard
 * @param: columns count(no more than 32)
 * @param: rows count
 * @param: prescaler for scanning
 * @param: KBD_SET/RESET- logic level on actual scanning row, and logic level of pressed button
 *
 * @retval: status
 */
kbd_state_t kbd_init(kbd_keyboard_t *keyboard, uint8_t columns, uint8_t rows,
		uint32_t prescaler, kbd_state_t pressedState) {

	if (rows < 1 || columns < 1)
		return KBD_ROWS_OR_COLUMNS_NUMBER_CANT_BE_0;

	if (columns > 32)
		return KBD_TOO_MUCH_COLUMNS;

	//set layout output as spaces
	//memset(layout, ' ', KBD_MAX_PRESSED);

	keyboard->numberOfColumns = columns;
	keyboard->numberOfRows = rows;
	keyboard->numberOfKeys = keyboard->numberOfColumns * keyboard->numberOfRows;
	keyboard->prescaler = prescaler;
	keyboard->pressedState = pressedState;

	uint32_t pinStructSize = sizeof(kbd_pin_t);

	keyboard->columns = malloc(keyboard->numberOfColumns * pinStructSize);
	if (keyboard->columns == NULL)
		return KBD_NOT_ENOUGH_MEMORY;

	keyboard->rows = malloc(keyboard->numberOfRows * pinStructSize);
	if (keyboard->rows == NULL) {
		free(keyboard->columns);
		return KBD_NOT_ENOUGH_MEMORY;
	}

	keyboard->stateMatrix = malloc(rows * sizeof(uint32_t));
	if (keyboard->stateMatrix == NULL) {
		free(keyboard->columns);
		free(keyboard->rows);
		return KBD_NOT_ENOUGH_MEMORY;
	}

	keyboard->layout = malloc(keyboard->numberOfKeys * sizeof(char));
	if (keyboard->layout == NULL) {
		free(keyboard->columns);
		free(keyboard->rows);
		free(keyboard->stateMatrix);
		return KBD_NOT_ENOUGH_MEMORY;
	}

	memset(keyboard->stateMatrix, 0, rows * sizeof(uint32_t));

	keyboard->actualScannedRow = 0;
	return KBD_OK;

}

/*
 * set keyboard layout- assign one char to each key
 * using this function are optional
 * @param: keyboard
 * @param: charsr
 *
 * @retval: none
 */
void kbd_setLayout(kbd_keyboard_t *keyboard, ...) {
	va_list ap;
	va_start(ap, keyboard);

	for (uint8_t i = 0; i < keyboard->numberOfKeys; i++) {
		keyboard->layout[i] = (char) va_arg(ap, uint32_t);
	}

	va_end(ap);
}

/*
 * define ports and pins for columns(GPI- inputs)
 * @param: keybaord
 * @params: portStruct*, pinMask, portStruct*, pinMask...
 *
 * @retval: status
 */
kbd_state_t kbd_setColumns(kbd_keyboard_t *keyboard, ...) {

	va_list ap;
	va_start(ap, keyboard);

	for (uint8_t i = 0; i < keyboard->numberOfColumns; i++) {
		keyboard->columns[i].port = va_arg(ap, kbd_port_t*);
		keyboard->columns[i].pin = va_arg(ap, uint32_t);
	}

	va_end(ap);

	return KBD_OK;

}

/*
 * define ports and pins for rows(GPO- outputs)
 * don't use if your keybaord have 1 row only. These pins are used only in scanning mode
 * @param: keybaord
 * @params: portStruct*, pinMask, portStruct*, pinMask...
 *
 * @retval: status
 */
kbd_state_t kbd_setRows(kbd_keyboard_t *keyboard, ...) {

	if (keyboard->numberOfRows <= 1)
		return KBD_NOT_IN_SCANNING_MODE;

	va_list ap;
	va_start(ap, keyboard);

	for (uint8_t i = 0; i < keyboard->numberOfRows; i++) {
		keyboard->rows[i].port = va_arg(ap, kbd_port_t*);
		keyboard->rows[i].pin = va_arg(ap, uint32_t);
	}

	va_end(ap);

	return KBD_OK;

}

/*
 * read keyboard row as one number
 * one bit are one key
 *
 * @param: keyboard
 * @param: row to read[0-3 for 4 row keyboard]
 *
 * @retval: row state
 */
uint32_t kbd_readRow(kbd_keyboard_t *keyboard, uint8_t row) {

	if (row >= keyboard->numberOfRows) // >= becouse if numberofRows are 3, max row can be 2(0, 1, 2)
		return KBD_ERR;
	return keyboard->stateMatrix[row];

}

/*
 * read KBD_MAX_PRESSED_BUTTONS pressed keys
 * @param: keyboard
 * @param: pointer to array for store pressed buttons.
 * Make sure array to have min. KBD_MAX_PRESSED_BUTTONS bytes!
 * array will be filled with chars assigned using kbd_setLayout()
 *
 *
 * @retval: KBD_OK/ANY_PRESSED
 */
kbd_state_t kbd_readFromLayout(kbd_keyboard_t *keyboard, char *buttonsArray) {

	uint8_t buttonsArrayFillCounter = 0;

	for (uint8_t rows = 0; rows < keyboard->numberOfRows; rows++) {
		for (uint8_t columns = 0; columns < keyboard->numberOfColumns;
				columns++) {
			if (keyboard->stateMatrix[rows] & 1 << columns) {
				if (buttonsArrayFillCounter < KBD_MAX_PRESSED_BUTTONS) {
					buttonsArray[buttonsArrayFillCounter] =
							keyboard->layout[rows * keyboard->numberOfColumns
									+ keyboard->numberOfColumns - 1 - columns];
					buttonsArrayFillCounter++;
				} else
					break;
			}
		}
	}

	//fill not pressed as spaces
	for(uint8_t i = buttonsArrayFillCounter; i<KBD_MAX_PRESSED_BUTTONS; i++){
		buttonsArray[i] = ' ';
	}

	if (buttonsArrayFillCounter == 0)
		return KBD_ANY_PRESSED;
	else
		return KBD_OK;
}

/*scanning function. run in synchronous interrupt(eg systick 1ms timer)
 * @param: keybaord
 *
 * @retval: none
 */

void kbd_scanning(kbd_keyboard_t *keyboard) {
	keyboard->masterTimer++;

	//run with prescaler
	if (keyboard->masterTimer % keyboard->prescaler == 0) {

		//save previous row to matrix
		//read current row
		for (uint8_t i = 0; i < keyboard->numberOfColumns; i++) {
			if (kbd_readPin(&keyboard->columns[i]) == keyboard->pressedState)
				keyboard->stateMatrix[keyboard->actualScannedRow] |= 1 << i;
			else
				keyboard->stateMatrix[keyboard->actualScannedRow] &= ~(1 << i);
		}

		//switch row only if there's more than one(Scanning mode)
		if (keyboard->numberOfRows > 1) {
			//switch to next row
			keyboard->actualScannedRow++;
			if (keyboard->actualScannedRow > keyboard->numberOfRows - 1)
				keyboard->actualScannedRow = 0;

			//reset all rows
			for (uint8_t i = 0; i < keyboard->numberOfRows; i++)
				kbd_writePin(&keyboard->rows[i], !keyboard->pressedState);

			//set next
			kbd_writePin(&keyboard->rows[keyboard->actualScannedRow],
					keyboard->pressedState);
		}

	}

}

