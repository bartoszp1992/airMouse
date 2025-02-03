/*
 * espathid.c
 *
 *  Created on: Feb 3, 2025
 *      Author: bartosz
 */

#include <stdint.h>
#include "espat.h"s

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

espat_state_t uartReceive(espat_uartInstance_t *uart, char *data,
		uint32_t size) {

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

espat_state_t espAt_sendCommand(espathid_radio_t *radio, char* command, uint16_t paramCount, ...){


	if(ATcommand == NULL)
		return ESPAT_STATE_ERR;

	char buffer[AT_BUFFER_SIZE];
	memset(buffer, ' ', AT_BUFFER_SIZE);

	uint16_t commandLength = strlen(ATcommand);
	uint16_t prefixLength = strlen(AT_PREFIX);
	uint16_t endingLength = strlen(AT_ENDING);

	va_list ap;
	va_start(ap, paramCount);



	va_end(ap);


}
