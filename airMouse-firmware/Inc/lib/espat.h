/*
 * espathid.h
 *
 *  Created on: Feb 3, 2025
 *      Author: bartosz
 */

#ifndef ESPAT_H_
#define ESPAT_H_

#define EN_SUPPORT 1
#define BOOT_SUPPORT 1

//port- hardware specific
#include "usart.h"
#if (BOOT_SUPPORT == 1) || (EN_SUPPORT == 1)
#include "gpio.h"
#include "stm32g0xx_hal_gpio.h"
#endif

//others
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#if (BOOT_SUPPORT == 1) || (EN_SUPPORT == 1)
typedef enum{
	ESPAT_RESET = 0,
	ESPAT_SET = 1
}espat_pinState_t;
#endif

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
	ESPAT_STATE_OK = 0,
	ESPAT_STATE_ERR = 1,
	ESPAT_STATE_TIMEOUT = 2,
	ESPAT_STATE_BUSY = 3,
	ESPAT_STATE_PIN_NOT_DEFINED = 4
} espat_state_t;

//hardware specific uart structure
typedef struct {
	UART_HandleTypeDef *uart; //port
	uint32_t sendTimeout;
	uint32_t receiveTimeout;
} espat_uartInstance_t;

//hardware specific pin struct //port
#if (BOOT_SUPPORT == 1) || (EN_SUPPORT == 1)

#define espat_port_t GPIO_TypeDef
typedef struct{
	espat_port_t *port;
	uint32_t pin;
}espat_pin_t;

#endif

//radio structure
typedef struct {
	espat_uartInstance_t espUart;

#if (BOOT_SUPPORT == 1)
	espat_pin_t pinBoot;
#endif

#if (EN_SUPPORT == 1)
	espat_pin_t pinEn;
#endif

} espat_radio_t;

espat_state_t espAt_init(espat_radio_t *radio, UART_HandleTypeDef *uart,
		uint32_t txTimeout, uint32_t rxTimeout); //port
espat_state_t espAt_sendCommand(espat_radio_t *radio, char *command);
espat_state_t espAt_sendParams(espat_radio_t *radio, char *command,
		uint16_t paramCount, ...);
espat_state_t espAt_sendString(espat_radio_t *radio, char *command,
		char *string);
espat_state_t espAt_receive(espat_radio_t *radio, char *response, uint16_t size);

#if (EN_SUPPORT == 1)
espat_state_t espAt_defineEn(espat_radio_t *radio, espat_port_t *port, uint32_t pin);
espat_state_t espAt_pwrOn(espat_radio_t *radio);
espat_state_t espAt_pwrOff(espat_radio_t *radio);

#endif

#if (BOOT_SUPPORT == 1)
espat_state_t espAt_defineBoot(espat_radio_t *radio, espat_port_t *port, uint32_t pin);
#endif

#if (BOOT_SUPPORT == 1) && (EN_SUPPORT == 1)
espat_state_t espAt_enterDownload(espat_radio_t *radio);
espat_state_t esp32rst(espat_radio_t *radio);
#endif


#endif /* ESPAT_H_ */
