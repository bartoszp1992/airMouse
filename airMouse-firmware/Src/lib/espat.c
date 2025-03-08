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
 * wrapper for hardware specific uartSend/Receive
 */
espat_state_t uartSend(espat_uartInstance_t *uart, char *data, uint32_t size) {

	HAL_StatusTypeDef state = HAL_OK;

	state = HAL_UART_Transmit(uart->uart, (uint8_t*) data, size,
			uart->sendTimeout);

	if (state == HAL_OK)
		return ESPAT_STATE_OK;
	else if (state == HAL_TIMEOUT)
		return ESPAT_STATE_TIMEOUT;
	else if (state == HAL_BUSY)
		return ESPAT_STATE_BUSY;
	else
		return ESPAT_STATE_ERR;
}

//port
espat_state_t uartReceive(espat_uartInstance_t *uart, char *data, uint32_t size) {

	HAL_StatusTypeDef state;

	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 1); //debug

	state = HAL_UART_Receive(uart->uart, (uint8_t*) data, size,
			uart->receiveTimeout);

	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 0); //debug

	if (state == HAL_OK)
		return ESPAT_STATE_OK;
	else if (state == HAL_TIMEOUT)
		return ESPAT_STATE_TIMEOUT;
	else
		return ESPAT_STATE_ERR;
}

#if (BOOT_SUPPORT == 1) || (EN_SUPPORT == 1)
/*
 * read GPIO
 * @param: pin
 *
 * @retval: readed pin state
 */
espat_pinState_t espAt_readPin(espat_pin_t *pin) { //port
	if (HAL_GPIO_ReadPin(pin->port, pin->pin) == GPIO_PIN_RESET)
		return ESPAT_RESET;
	else
		return ESPAT_SET;
}

/*
 * write GPIO
 * @param: pin
 *
 * @retval: none
 */
void espAt_writePin(espat_pin_t *pin, espat_pinState_t state) { //port
	if (state == ESPAT_RESET)
		HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(pin->port, pin->pin, GPIO_PIN_SET);
}
#endif

/*
 * wait function
 * @param: time in ms
 *
 * @retval: none
 */
void espAt_delay(uint32_t delay) { //port
	HAL_Delay(delay);
}

//---------------------------PUBLIC FUNCTIONS---------------------------

/*
 * init radio
 * @param: radio struct
 * @param: uart MCU specific struct
 * @param: timeout for transmit
 * @param: timout for receive- 1500ms seems good for BLEHID application
 *
 * @retval: always ESPATHID_OK
 */
espat_state_t espAt_init(espat_radio_t *radio, UART_HandleTypeDef *uart,
		uint32_t txTimeout, uint32_t rxTimeout) { //port

	radio->espUart.uart = uart;
	radio->espUart.sendTimeout = txTimeout;
	radio->espUart.receiveTimeout = rxTimeout;
	memset(radio->rxBuffer, ' ', RX_BUFFER_SIZE);
	radio->response = ESPAT_RESPONSE_ERROR;

#if (EN_SUPPORT == 1)
	radio->pinEn.port = NULL;
#endif

#if (BOOT_SUPPORT == 1)
	radio->pinBoot.port = NULL;
#endif

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
	int32_t parameters[paramCount];

	//fill parameters array
	for (uint16_t i = 0; i < paramCount; i++) {
		parameters[i] = va_arg(ap, int32_t);
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
 * send question to ESP32 to get response with desired informations.
 * remember to run espAt_getResponse() to... get response :)
 * @param: radio
 * @param: command
 *
 * @retval: status
 */
espat_state_t espAt_sendQuestion(espat_radio_t *radio, char *command) {

	//check if command are valid
	if (command == NULL)
		return ESPAT_STATE_ERR;

	//count buffer length
	uint16_t prefixLength = strlen(AT_PREFIX);
	uint16_t commandLength = strlen(command);
	uint16_t questionLength = strlen(AT_QUESTION);
	uint16_t endingLength = strlen(AT_ENDING);

	//create master buffer and clear it
	uint16_t bufferSize = prefixLength + commandLength + questionLength
			+ endingLength;
	char buffer[bufferSize];
	memset(buffer, ' ', bufferSize);

	//build command
	memcpy(&buffer[0], AT_PREFIX, prefixLength);
	memcpy(&buffer[prefixLength], command, commandLength);
	memcpy(&buffer[prefixLength + commandLength], AT_QUESTION, questionLength);
	memcpy(&buffer[prefixLength + commandLength + questionLength], AT_ENDING,
			endingLength);

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
 * @output: ESPAT_RESPONSE_OK/ERROR/BUSY/PARSING_ERROR
 *
 * @retval: state
 */
espat_state_t espAt_downloadResponse(espat_radio_t *radio) {

	memset(radio->rxBuffer, ' ', RX_BUFFER_SIZE);
	espat_state_t state;

	for (uint32_t actualReceived = 0; actualReceived < RX_BUFFER_SIZE;
			actualReceived++) {

		state = uartReceive(&radio->espUart, &radio->rxBuffer[actualReceived],
				1);

		if (state != ESPAT_STATE_OK)
			return state;

		if (radio->rxBuffer[actualReceived] == '\n'
				&& radio->rxBuffer[actualReceived - 1] == '\r'
				&& actualReceived > 1) {

			uint8_t sizeOk = strlen(AT_OK);
			uint8_t sizeErr = strlen(AT_ERROR);
			uint8_t sizeBusy = strlen(AT_BUSY);
			uint8_t sizeReady = strlen(AT_READY);

			if (memcmp(&radio->rxBuffer[actualReceived - 1 - sizeOk], AT_OK,
					sizeOk) == 0) {
				radio->response = ESPAT_RESPONSE_OK;
				break;
			} else if (memcmp(&radio->rxBuffer[actualReceived - 1 - sizeErr],
			AT_ERROR, sizeErr) == 0) {
				radio->response = ESPAT_RESPONSE_ERROR;
				break;
			} else if (memcmp(&radio->rxBuffer[actualReceived - 1 - sizeBusy],
			AT_BUSY, sizeBusy) == 0) {
				radio->response = ESPAT_RESPONSE_BUSY;
				break;
			} else if (memcmp(&radio->rxBuffer[actualReceived - 1 - sizeReady],
			AT_READY, sizeReady) == 0) {
				radio->response = ESPAT_RESPONSE_READY;
				break;
			}
		} else {
			radio->response = ESPAT_RESPONSE_PARSING_ERROR;
		}
	}
	return state;

}

/*
 * return last ESP response
 */
espat_response_t espAt_returnResponseStatus(espat_radio_t *radio) {
	return radio->response;
}

#if (EN_SUPPORT == 1)
/*
 * define EN pin of ESP32.
 * @param: radio
 * @param: port
 * @param: pin
 *
 * @retval: status
 */
espat_state_t espAt_defineEn(espat_radio_t *radio, espat_port_t *port,
		uint32_t pin) {

	radio->pinEn.port = port;
	radio->pinEn.pin = pin;

	return ESPAT_STATE_OK;
}

/*
 * power on using EN pin
 * @param: radio
 *
 * @retval: status
 */
espat_state_t espAt_pwrOn(espat_radio_t *radio) {
	if (radio->pinEn.port != NULL) {
		espAt_writePin(&radio->pinEn, ESPAT_SET);
		return ESPAT_STATE_OK;
	} else
		return ESPAT_STATE_PIN_NOT_DEFINED;
}

/*
 * power off using EN pin
 * @param: radio
 *
 * @retval: status
 */
espat_state_t espAt_pwrOff(espat_radio_t *radio) {
	if (radio->pinEn.port != NULL) {
		espAt_writePin(&radio->pinEn, ESPAT_RESET);
		return ESPAT_STATE_OK;
	} else
		return ESPAT_STATE_PIN_NOT_DEFINED;
}

#endif

#if (BOOT_SUPPORT == 1)
/*
 * define port and pin connected to BOOT of ESP32.
 * @param: radio
 * @param: port
 * @param: pin
 *
 * @retval: status
 */
espat_state_t espAt_defineBoot(espat_radio_t *radio, espat_port_t *port,
		uint32_t pin) {

	radio->pinBoot.port = port;
	radio->pinBoot.pin = pin;

	return ESPAT_STATE_OK;

}
#endif

#if (BOOT_SUPPORT == 1) && (EN_SUPPORT == 1)
/*
 * reset in download mode
 * @param: radio
 *
 * @retval: status
 */
espat_state_t espAt_enterDownload(espat_radio_t *radio) {

	if (radio->pinBoot.port != NULL && radio->pinEn.port != NULL) {
		espAt_writePin(&radio->pinEn, ESPAT_RESET);
		espAt_delay(100);
		espAt_writePin(&radio->pinBoot, ESPAT_RESET);
		espAt_delay(100);
		espAt_writePin(&radio->pinEn, ESPAT_SET);
		espAt_delay(100);
		espAt_writePin(&radio->pinBoot, ESPAT_SET);
		espAt_delay(100);
		return ESPAT_STATE_OK;
	} else
		return ESPAT_STATE_PIN_NOT_DEFINED;

}

/*
 * reset in normal mode
 * @param: radio
 *
 * @retval: status
 */
espat_state_t espAt_rst(espat_radio_t *radio) {

	if (radio->pinBoot.port != NULL && radio->pinEn.port != NULL) {
		espAt_writePin(&radio->pinBoot, ESPAT_SET);
		espAt_delay(100);
		espAt_writePin(&radio->pinEn, ESPAT_RESET);
		espAt_delay(100);
		espAt_writePin(&radio->pinEn, ESPAT_SET);
		espAt_delay(100);
		return ESPAT_STATE_OK;
	} else
		return ESPAT_STATE_PIN_NOT_DEFINED;
}
#endif

/*
 UNUSED CODE


 espat_state_t espAt_getResponseDMA(espat_radio_t *radio) {

 if (!radio->flagDmaReceive) {
 memset(radio->rxBuffer, ' ', RX_BUFFER_SIZE);
 espat_state_t state;

 for (uint32_t actualReceived = 0; actualReceived < RX_BUFFER_SIZE;
 actualReceived++) {

 state = uartReceiveDMA(&radio->espUart,
 &radio->rxBuffer[actualReceived], 1);

 if (state != ESPAT_STATE_OK)
 return state;

 if (radio->rxBuffer[actualReceived] == '\n'
 && radio->rxBuffer[actualReceived - 1] == '\r'
 && actualReceived > 1) {

 uint8_t sizeOk = strlen(AT_OK);
 uint8_t sizeErr = strlen(AT_ERROR);
 uint8_t sizeBusy = strlen(AT_BUSY);
 uint8_t sizeReady = strlen(AT_READY);

 if (memcmp(&radio->rxBuffer[actualReceived - 1 - sizeOk], AT_OK,
 sizeOk) == 0) {
 radio->response = ESPAT_RESPONSE_OK;
 break;
 } else if (memcmp(
 &radio->rxBuffer[actualReceived - 1 - sizeErr],
 AT_ERROR, sizeErr) == 0) {
 radio->response = ESPAT_RESPONSE_ERROR;
 break;
 } else if (memcmp(
 &radio->rxBuffer[actualReceived - 1 - sizeBusy],
 AT_BUSY, sizeBusy) == 0) {
 radio->response = ESPAT_RESPONSE_BUSY;
 break;
 } else if (memcmp(
 &radio->rxBuffer[actualReceived - 1 - sizeReady],
 AT_READY, sizeReady) == 0) {
 radio->response = ESPAT_RESPONSE_READY;
 break;
 }
 } else {
 radio->response = ESPAT_RESPONSE_PARSING_ERROR;
 }
 }
 return state;
 } else
 return ESPAT_STATE_ERROR_DMA_RX_ACTIVE;
 }


 espat_state_t uartReceiveDMA(espat_uartInstance_t *uart, char *data,
 uint32_t size) {

 HAL_StatusTypeDef state;

 state = HAL_UART_Receive_DMA(uart->uart, (uint8_t*) data, size);

 if (state == HAL_OK)
 return ESPAT_STATE_OK;
 else if (state == HAL_TIMEOUT)
 return ESPAT_STATE_TIMEOUT;
 else
 return ESPAT_STATE_ERR;
 }


 */
