/*
 * radio.c
 *
 *  Created on: Mar 10, 2025
 *      Author: bartosz
 */

#include "radio.h"

espat_radio_t bleRadio;

espat_state_t espStat;
espat_response_t espResponse;

void radio_init(void) {
	espStat = espAt_init(&bleRadio, &huart1, 10, 1500);
	espStat = espAt_defineEn(&bleRadio, ESP_EN_GPIO_Port, ESP_EN_Pin);
	espStat = espAt_defineBoot(&bleRadio, ESP_BOOT_GPIO_Port, ESP_BOOT_Pin);
	//enter download mode or reset ESP
	if (HAL_GPIO_ReadPin(MUS_BCK_GPIO_Port, MUS_BCK_Pin) == GPIO_PIN_RESET)
		espStat = espAt_enterDownload(&bleRadio);

	//turn on
	espStat = espAt_pwrOn(&bleRadio);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponseStatus(&bleRadio);

	//change name
	espStat = espAt_sendString(&bleRadio, S_BHN, CONFIG_DEVICE_NAME);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponseStatus(&bleRadio);

	//turn on BLE
	espStat = espAt_sendParams(&bleRadio, P_BHI, 1, 1);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponseStatus(&bleRadio);

	//change baudrate
	espAt_sendParams(&bleRadio, P_UC, 5, CONFIG_BAUDRATE_FAST, 8, 1, 0, 0);
	HAL_Delay(200);
	HAL_UART_ChangeSpeed(&huart1, CONFIG_BAUDRATE_FAST);
}
