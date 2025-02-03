/*
 * espathid.h
 *
 *  Created on: Feb 3, 2025
 *      Author: bartosz
 */

#ifndef ESPAT_H_
#define ESPAT_H_

//hardware specific
#include "usart.h"

//others
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define AT_PREFIX "AT+"
#define AT_ENDING "\r\n"
#define AT_ASSIGNMENT "="
#define AT_QUESTION "?"
#define AT_BUFFER_SIZE 30

#define BLEHIDINIT "BLEHIDINIT"
#define BLEHIDNAME "BLEHIDNAME"


typedef enum{
	ESPAT_STATE_OK = 0,
	ESPAT_STATE_ERR = 1
}espat_state_t;

typedef struct{
	UART_HandleTypeDef *uart;
	uint32_t timeout;
}espat_uartInstance_t;

typedef struct{
	espat_uartInstance_t *uart;

}espat_radio_t;

espat_state_t espAt_Init(espat_radio_t *radio, UART_HandleTypeDef *uart, uint32_t timeout);
espat_state_t espAt_sendCommand(espat_radio_t *radio, char* command, uint16_t paramCount, ...);


#endif /* ESPAT_H_ */
