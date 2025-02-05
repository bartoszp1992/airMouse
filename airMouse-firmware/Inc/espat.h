/*
 * espathid.h
 *
 *  Created on: Feb 3, 2025
 *      Author: bartosz
 */

#ifndef ESPAT_H_
#define ESPAT_H_

//port- hardware specific
#include "usart.h"

//others
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define AT_PREFIX "AT+"
#define AT_ENDING "\r\n"
#define AT_ASSIGNMENT "="
#define AT_QUESTION "?"
#define AT_BUFFER_SIZE 30
#define AT_STRING_QUOTE_MARK "\""

//general
#define G_RST "RST"

//UART
#define P_UC "UART_CUR" //change without saving (baudrate, data bits, stopbits, parity, flow control)
#define P_UD "UART_DEF" //change and save (baudrate, data bits, stopbits, parity, flow control)

//BLE HID
#define P_BHI "BLEHIDINIT" //(1/0)
#define P_BHM "BLEHIDMUS"	//send mouse data (keys, x, y, wheel)
#define S_BHN "BLEHIDNAME"





typedef enum {
	ESPAT_STATE_OK = 0, ESPAT_STATE_ERR = 1, ESPAT_STATE_TIMEOUT = 2
} espat_state_t;

//hardware specific uart structure
typedef struct {
	UART_HandleTypeDef *uart; //port
	uint32_t sendTimeout;
	uint32_t receiveTimeout;
} espat_uartInstance_t;


//radio structure
typedef struct {
	espat_uartInstance_t espUart;

} espat_radio_t;

espat_state_t espAt_init(espat_radio_t *radio, UART_HandleTypeDef *uart,
		uint32_t txTimeout, uint32_t rxTimeout);//port
espat_state_t espAt_sendCommand(espat_radio_t *radio, char *command);
espat_state_t espAt_sendParams(espat_radio_t *radio, char *command,
		uint16_t paramCount, ...);
espat_state_t espAt_sendString(espat_radio_t *radio, char *command,
		char *string);
espat_state_t espAt_receive(espat_radio_t *radio, char *response, uint16_t size);

#endif /* ESPAT_H_ */
