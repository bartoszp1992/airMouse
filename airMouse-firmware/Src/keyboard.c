/*
 * keyboard.c
 *
 *  Created on: Mar 1, 2025
 *      Author: bartosz
 */

#include "keyboard.h"

kbd_pinState_t readPin(kbd_pin_t *pin) {
	if (HAL_GPIO_ReadPin(pin->port, pin->pin) == GPIO_PIN_RESET)
		return KBD_RESET;
	else
		return KBD_SET;
}

void writePin(kbd_pin_t *pin, kbd_pinState_t state) {
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
 *
 * @retval: status
 */
kbd_state_t kbd_init(kbd_keyboard_t *keyboard, uint8_t columns, uint8_t rows, uint32_t prescaler){


	if(columns > 32)
		return KBD_TOO_MUCH_COLUMNS;

	keyboard->numberOfColumns = columns;
	keyboard->numberOfRows = rows;
	keyboard->numberOfKeys = keyboard->numberOfColumns*keyboard->numberOfRows;
	keyboard->prescaler = prescaler;

	uint32_t pinStructSize = sizeof(kbd_pin_t);

	keyboard->columns = malloc(keyboard->numberOfColumns*pinStructSize);
	if(keyboard->columns == NULL)
		return KBD_NOT_ENOUGH_MEMORY;

	keyboard->rows = malloc(keyboard->numberOfRows*pinStructSize);
	if(keyboard->rows == NULL){
		free(keyboard->columns);
		return KBD_NOT_ENOUGH_MEMORY;
	}

	keyboard->stateMatrix = malloc(rows*sizeof(uint32_t));
	if(keyboard->stateMatrix == NULL){
		free(keyboard->columns);
		free(keyboard->rows);
		return KBD_NOT_ENOUGH_MEMORY;
	}

	memset(keyboard->stateMatrix, 0, rows*sizeof(uint32_t));

	keyboard->actualScannedRow = 0;
	return KBD_OK;

}

/*
 * define ports and pins for columns(GPI- inputs)
 * @param: keybaord
 * @params: portStruct*, pinMask, portStruct*, pinMask...
 *
 * @retval: status
 */
kbd_state_t kbd_set_columns(kbd_keyboard_t *keyboard, ...){

	va_list ap;
	va_start(ap, keyboard);

	for(uint8_t i = 0; i<keyboard->numberOfColumns; i++){
		keyboard->columns[i].port = va_arg(ap, kbd_port_t*);
		keyboard->columns[i].pin = va_arg(ap, uint32_t);
	}

	va_end(ap);

	return KBD_OK;

}

/*
 * define ports and pins for rows(GPO- outputs)
 * @param: keybaord
 * @params: portStruct*, pinMask, portStruct*, pinMask...
 *
 * @retval: status
 */
kbd_state_t kbd_set_rows(kbd_keyboard_t *keyboard, ...){

	va_list ap;
	va_start(ap, keyboard);

	for(uint8_t i = 0; i<keyboard->numberOfRows; i++){
		keyboard->rows[i].port = va_arg(ap, kbd_port_t*);
		keyboard->rows[i].pin = va_arg(ap, uint32_t);
	}

	va_end(ap);

	return KBD_OK;

}

/*scanning function. run in synchronous interrupt(eg systick 1ms timer)
 * @param: keybaord
 *
 * @retval: none
 */

void kbd_scanning(kbd_keyboard_t *keyboard){
	keyboard->masterTimer++;

	//run with prescaler
	if(keyboard->masterTimer % keyboard->prescaler == 0){

		//save previous row to matrix
		//read current row
		for(uint8_t i = 0; i<keyboard->numberOfColumns; i++){
			if(readPin(&keyboard->columns[i]) == KBD_SCAN_ROW_STATE)
				keyboard->stateMatrix[keyboard->actualScannedRow] |= 1<<i;
			else
				keyboard->stateMatrix[keyboard->actualScannedRow] &= ~(1<<i);
		}

		//switch to next row
		keyboard->actualScannedRow++;
		if(keyboard->actualScannedRow > keyboard->numberOfRows -1)
			keyboard->actualScannedRow = 0;

		//reset all rows
		for(uint8_t i = 0; i< keyboard->numberOfRows; i++)
			writePin(&keyboard->rows[i], !KBD_SCAN_ROW_STATE);

		//set next
		writePin(&keyboard->rows[keyboard->actualScannedRow], KBD_SCAN_ROW_STATE);

	}

}
