/*
 * espathid.c
 *
 *  Created on: Feb 3, 2025
 *      Author: bartosz
 */

#include <stdint.h>
#include "espat.h"

//---------------------------IMPLICIT FUNCTIONS---------------------------
/*
 * uart send/receive functions. fill it by hardware-specific uart send/receive functions
 */
espat_state_t uartSend(espat_uartInstance_t *uart, char *data, uint32_t size) {
	if (HAL_UART_Transmit(uart->uart, (uint8_t*) data, size, uart->timeout)
			== HAL_OK)
		return ESPAT_STATE_OK;
	else
		return ESPAT_STATE_ERR;
}

espat_state_t uartReceive(espat_uartInstance_t *uart, char *data, uint32_t size) {

	uint16_t receivedBytes = 0;

	if (HAL_UARTEx_ReceiveToIdle(uart->uart, (uint8_t*) data, size,
			&receivedBytes, uart->timeout) == HAL_OK)
		return ESPAT_STATE_OK;
	else
		return ESPAT_STATE_ERR;
}

//---------------------------PUBLIC FUNCTIONS---------------------------
/*
 * init radio
 * @param: radio struct
 * @param: uart MCU specific struct
 * @param: timeout
 *
 * @retval: always ESPATHID_OK
 */
espat_state_t espAt_Init(espat_radio_t *radio, UART_HandleTypeDef *uart,
		uint32_t timeout) {

	radio->uart->uart = uart;
	radio->uart->timeout = timeout;
	return ESPAT_STATE_OK;

}

espat_state_t espAt_sendCommand(espat_radio_t *radio, char *command,
		uint16_t paramCount, ...) {

	//check if command are valid
	if (command == NULL)
		return ESPAT_STATE_ERR;

	va_list ap;
	va_start(ap, paramCount);

	//make parameters array
	uint32_t parameters[paramCount];

	//fill parameters array
	for (uint16_t i = 0; i < paramCount; i++) {
		parameters[i] = va_arg(ap, uint32_t);
	}

	//make arguments list string
	uint16_t characterCounter = 0;
	char argumentsBuffer[AT_BUFFER_SIZE];
	memset(argumentsBuffer, '\0', AT_BUFFER_SIZE);

	for (uint16_t i = 0; i < paramCount; i++) {
		itoa(parameters[i], &argumentsBuffer[characterCounter], 10);
		characterCounter += strlen(&argumentsBuffer[characterCounter]) + 1;
		if (i < paramCount - 1)
			argumentsBuffer[characterCounter - 1] = ','; //add comma only if NOT last parameter
	}

	va_end(ap);

	//count buffer length
	uint16_t prefixLength = strlen(AT_PREFIX);
	uint16_t commandLength = strlen(command);
	uint16_t assignmentLength = strlen(AT_ASSIGNMENT);
	uint16_t parametersLength = --characterCounter;
	uint16_t endingLength = strlen(AT_ENDING);

	//create master buffer and clear it
	uint16_t bufferSize = prefixLength + commandLength + assignmentLength + parametersLength
			+ endingLength;
	char buffer[bufferSize];
	memset(buffer, ' ', bufferSize);

	//build command
	memcpy(&buffer[0], AT_PREFIX, prefixLength);
	memcpy(&buffer[prefixLength], command, commandLength);
	memcpy(&buffer[prefixLength + commandLength], AT_ASSIGNMENT,
			assignmentLength);
	memcpy(&buffer[prefixLength + commandLength + assignmentLength],
			argumentsBuffer, parametersLength);
	memcpy(
			&buffer[prefixLength + commandLength + assignmentLength
					+ parametersLength], AT_ENDING, endingLength);

	uartSend(radio->uart, buffer, bufferSize);

	return ESPAT_STATE_OK;

}
