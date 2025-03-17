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
#define RX_BUFFER_SIZE 100 //max size for storing whole response received from ESP32

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
typedef enum {
	ESPAT_RESET = 0, ESPAT_SET = 1
} espat_pinState_t;
#endif



//______________________________________________COMMANDS______________________________________________
//P- PARAM- sending numbers
//S- STRING- sending string
//G- GENERIC- just command, without sending data, like reset.
//C- COMPLEX- mix of strings and numbers

//general
#define G_RST "RST"

//UART
#define P_UC "UART_CUR" //change without saving (baudrate, data bits, stopbits, parity, flow control)
#define P_UD "UART_DEF" //change and save (baudrate, data bits, stopbits, parity, flow control)

//BLE HID
#define P_BHI "BLEHIDINIT" //(1/0)
#define P_BHM "BLEHIDMUS"	//send mouse data (keys, x, y, wheel)
#define S_BHN "BLEHIDNAME"
#define P_BHK "BLEHIDKB" //send keyboard data(mod, k1, k2, k3, k4, k5, k6)

//BLE
#define P_BI "BLEINIT"
#define P_BCP "BLECONNPARAM"
#define P_BD "BLEDISCONN"


#define S_BN "BLENAME"
#define C_BAP "BLEADVPARAM"
#define C_BA "BLEADDR"

typedef enum {
	ESPAT_STATE_OK = 0,
	ESPAT_STATE_ERR = 1,
	ESPAT_STATE_TIMEOUT = 2,
	ESPAT_STATE_BUSY = 3,
	ESPAT_STATE_PIN_NOT_DEFINED = 4,
	ESPAT_STATE_ERROR_PARAM = 6,
	ESPAT_STATE_MAC_NOT_FOUND = 7
} espat_state_t;

typedef enum{
	ESPAT_ECHO_ON,
	ESPAT_ECHO_OFF
} espat_echo_t;

/*
 * be sure to implement every response in espAt_downloadResponse()
 */
typedef enum {
	ESPAT_RESPONSE_OK,
	ESPAT_RESPONSE_ERROR,
	ESPAT_RESPONSE_BUSY,
	ESPAT_RESPONSE_READY,
	ESPAT_RESPONSE_PARSING_ERROR

} espat_response_t;

/*
 * be sure to implement every message in espAt_downloadMessage()
 */
typedef enum {
	ESPAT_MESSAGE_BLECONN,
	ESPAT_MESSAGE_BLEDISCONN,
	ESPAT_MESSAGE_BLEHIDCONN,
	ESPAT_MESSAGE_BLEHIDDISCONN
} espat_message_t;


typedef enum{
	ESPAT_PARAM_TYPE_STRING = 0,
	ESPAT_PARAM_TYPE_NUMBER
} espat_param_t; //used in send complex

//hardware specific uart structure
typedef struct {
	UART_HandleTypeDef *uart; //port
	uint32_t sendTimeout;
	uint32_t receiveTimeout;
} espat_uartInstance_t;

//hardware specific pin struct //port
#if (BOOT_SUPPORT == 1) || (EN_SUPPORT == 1)

#define espat_port_t GPIO_TypeDef
typedef struct {
	espat_port_t *port;
	uint32_t pin;
} espat_pin_t;

#endif

//radio structure
typedef struct {
	espat_uartInstance_t espUart;
	char rxBuffer[RX_BUFFER_SIZE];
	espat_response_t response; //response for AT command
	espat_message_t message;

#if (BOOT_SUPPORT == 1)
	espat_pin_t pinBoot;
#endif

#if (EN_SUPPORT == 1)
	espat_pin_t pinEn;
#endif

} espat_radio_t;

espat_state_t espAt_init(espat_radio_t *radio, UART_HandleTypeDef *uart,
		uint32_t txTimeout, uint32_t rxTimeout); //port
void espAt_setRxTimeout(espat_radio_t *radio, uint32_t rxTimeout);
espat_state_t espAt_echo(espat_radio_t *radio, espat_echo_t echo);
espat_state_t espAt_sendCommand(espat_radio_t *radio, char *command);
espat_state_t espAt_sendParams(espat_radio_t *radio, char *command,
		uint16_t paramCount, ...);
espat_state_t espAt_sendQuery(espat_radio_t *radio, char *command);
espat_state_t espAt_sendString(espat_radio_t *radio, char *command,
		char *string);
espat_state_t espAt_sendComplex(espat_radio_t *radio, char *command,
		uint8_t paramCount, ...);
espat_state_t espAt_downloadResponse(espat_radio_t *radio);
espat_state_t espAt_downloadMessage(espat_radio_t *radio);
espat_response_t espAt_returnResponse(espat_radio_t *radio);
espat_message_t espAt_returnMessage(espat_radio_t *radio);
espat_state_t espAt_returnPhysicalAddress(espat_radio_t *radio, char *mac);

#if (EN_SUPPORT == 1)
espat_state_t espAt_defineEn(espat_radio_t *radio, espat_port_t *port,
		uint32_t pin);
espat_state_t espAt_pwrOn(espat_radio_t *radio);
espat_state_t espAt_pwrOff(espat_radio_t *radio);

#endif

#if (BOOT_SUPPORT == 1)
espat_state_t espAt_defineBoot(espat_radio_t *radio, espat_port_t *port,
		uint32_t pin);
#endif

#if (BOOT_SUPPORT == 1) && (EN_SUPPORT == 1)
espat_state_t espAt_enterDownload(espat_radio_t *radio);
espat_state_t espAt_rst(espat_radio_t *radio);
#endif

#endif /* ESPAT_H_ */
