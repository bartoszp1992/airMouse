/*
 * espathid.c
 *
 *  Created on: Feb 3, 2025
 *      Author: bartosz
 *
 *      Note: for port library, you need to redefine uartSend, uartReceive and
 *      espAt_init functions. If you need some data to point MCU specific uart data, use
 *      espat_uartInstance_t struct and espAt_init function.
 *
 *      command defines:
 *      G_ - general commands- for use with espAt_sendCommand()
 *      P_ - parameter comman- for use with espAt_sendParams()
 *      S_ - string command- for use with espAt_sendString()
 */

#include <stdint.h>
#include "espat.h"

//---------------------------IMPLICIT FUNCTIONS---------------------------
/*
 * uart send/receive functions. fill it by hardware-specific uart send/receive functions
 * port
 */
espat_state_t uartSend(espat_uartInstance_t *uart, char *data, uint32_t size) {


	HAL_StatusTypeDef state = HAL_UART_Transmit(uart->uart, (uint8_t*) data,
			size, uart->sendTimeout);

	if (state == HAL_OK)
		return ESPAT_STATE_OK;
	else if (state == HAL_TIMEOUT)
		return ESPAT_STATE_TIMEOUT;
	else
		return ESPAT_STATE_ERR;
}

//port
espat_state_t uartReceive(espat_uartInstance_t *uart, char *data, uint32_t size) {

//	uint16_t receivedBytes = 0;
//	if (HAL_UARTEx_ReceiveToIdle(uart->uart, (uint8_t*) data, size,
//			&receivedBytes, uart->timeout) == HAL_OK)
//		return ESPAT_STATE_OK;
//	else
//		return ESPAT_STATE_ERR;

	HAL_StatusTypeDef state = HAL_UART_Receive(uart->uart, (uint8_t*) data,
			size, uart->receiveTimeout);

	if (state == HAL_OK)
		return ESPAT_STATE_OK;
	else if(state == HAL_TIMEOUT)
		return ESPAT_STATE_TIMEOUT;
	else
		return ESPAT_STATE_ERR;
}

//---------------------------PUBLIC FUNCTIONS---------------------------

/*
 * init radio
 * @param: radio struct
 * @param: uart MCU specific struct
 * @param: timeout for transmit
 * @param: timout for receive
 *
 * @retval: always ESPATHID_OK
 */
espat_state_t espAt_init(espat_radio_t *radio, UART_HandleTypeDef *uart,
		uint32_t txTimeout, uint32_t rxTimeout) { //port

	radio->espUart.uart = uart;
	radio->espUart.sendTimeout = txTimeout;
	radio->espUart.receiveTimeout = rxTimeout;
	return ESPAT_STATE_OK;

}

/*
 * send simple AT command
 * @param: radio struct
 * @param: command(e.g. RST)
 *
 * @retval: ESPAT state- OK if sent succesfully.
 * 			!NOTE: This parameter does not say whether and what ESP returns
 * 			in response to the command!
 */
espat_state_t espAt_sendCommand(espat_radio_t *radio, char *command) {
	//check if command and string are valid
	if (command == NULL)
		return ESPAT_STATE_ERR;

	uint16_t prefixLength = strlen(AT_PREFIX);
	uint16_t commandLength = strlen(command);
	uint16_t endingLength = strlen(AT_ENDING);

	uint16_t bufferSize = prefixLength + commandLength + endingLength;

	//create and clear buffer
	char buffer[bufferSize];
	memset(buffer, ' ', bufferSize);

	//build command
	memcpy(&buffer[0], AT_PREFIX, prefixLength);
	memcpy(&buffer[prefixLength], command, commandLength);
	memcpy(&buffer[prefixLength + commandLength], AT_ENDING, endingLength);

	return uartSend(&radio->espUart, buffer, bufferSize);
}

/*
 * send AT command with parameters
 * @param: radio struct
 * @param: command (e.g. BHI for AT+BLEHIDNAME)
 * @param: AT PARAMETERS COUNT. E.G. AT+BLEHIDNAME need one parameter(0 or 1),
 * 			you should give 1
 * @params: give parameters in correct order. Dont give more or less parameters
 * 			than declared in previous param
 *
 * @retval: ESPAT state- OK if sent succesfully.
 * 			!NOTE: This parameter does not say whether and what ESP returns
 * 			in response to the command!
 */
espat_state_t espAt_sendParams(espat_radio_t *radio, char *command,
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
	uint16_t bufferSize = prefixLength + commandLength + assignmentLength
			+ parametersLength + endingLength;
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

	return uartSend(&radio->espUart, buffer, bufferSize);

}

/*
 * send AT command with string
 * @param: radio struct
 * @param: AT command intended for send strings(e.g. AT+BLEHIDNAME)
 * @param: string to send in quote marks
 *
 * @retval: ESPAT state- OK if sent succesfully.
 * 			!NOTE: This parameter does not say whether and what ESP returns
 * 			in response to the command!
 */
espat_state_t espAt_sendString(espat_radio_t *radio, char *command,
		char *string) {

	//check if command and string are valid
	if (command == NULL || string == NULL)
		return ESPAT_STATE_ERR;

	uint16_t prefixLength = strlen(AT_PREFIX);
	uint16_t commandLength = strlen(command);
	uint16_t assignmentLength = strlen(AT_ASSIGNMENT);
	uint16_t quoteLength = strlen(AT_STRING_QUOTE_MARK);
	uint16_t stringLength = strlen(string);
	uint16_t endingLength = strlen(AT_ENDING);

	uint16_t bufferSize = prefixLength + commandLength + assignmentLength
			+ (quoteLength * 2) + stringLength + endingLength;

	//create and clear buffer
	char buffer[bufferSize];
	memset(buffer, ' ', bufferSize);

	//build command
	memcpy(&buffer[0], AT_PREFIX, prefixLength);
	memcpy(&buffer[prefixLength], command, commandLength);
	memcpy(&buffer[prefixLength + commandLength], AT_ASSIGNMENT,
			assignmentLength);
	memcpy(&buffer[prefixLength + commandLength + assignmentLength],
	AT_STRING_QUOTE_MARK, quoteLength);
	memcpy(
			&buffer[prefixLength + commandLength + assignmentLength
					+ quoteLength], string, stringLength);
	memcpy(
			&buffer[prefixLength + commandLength + assignmentLength
					+ quoteLength + stringLength], AT_STRING_QUOTE_MARK,
			stringLength);
	memcpy(
			&buffer[prefixLength + commandLength + assignmentLength
					+ (quoteLength * 2) + stringLength], AT_ENDING,
			endingLength);

	return uartSend(&radio->espUart, buffer, bufferSize);

}

/*
 * receive raw data from ESP
 * @param: radio struct
 * @param: pointer to save response
 * @param: size to receive
 *
 * @retval: state
 */
espat_state_t espAt_receive(espat_radio_t *radio, char *response, uint16_t size) {

	memset(response, ' ', size);
	return uartReceive(&radio->espUart, response, size);

}
