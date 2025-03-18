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

extern blink_t ledRed;
extern blink_t ledGreen;
extern blink_t ledBlue;

void radio_init(void) {
	espStat = espAt_init(&bleRadio, &huart1, 50, CONFIG_TIMEOUT_RX_LONG);
	espStat = espAt_defineEn(&bleRadio, ESP_EN_GPIO_Port, ESP_EN_Pin);
	espStat = espAt_defineBoot(&bleRadio, ESP_BOOT_GPIO_Port, ESP_BOOT_Pin);

	HAL_Delay(200);
	//enter download mode ESP
	if (HAL_GPIO_ReadPin(MUS_BCK_GPIO_Port, MUS_BCK_Pin) == GPIO_PIN_RESET
			&& HAL_GPIO_ReadPin(MUS_FWD_GPIO_Port, MUS_FWD_Pin)
					== GPIO_PIN_SET) {
		espStat = espAt_enterDownload(&bleRadio);
		blink_enable(&ledGreen, BLINK_PATTERN_ON, BLINK_MODE_ONCE);
		while (1)
			;
	//persistent UART baudrate change
	} else if (HAL_GPIO_ReadPin(MUS_FWD_GPIO_Port, MUS_FWD_Pin)
					== GPIO_PIN_RESET) {
		espStat = espAt_pwrOn(&bleRadio);
		blink_enable(&ledRed, BLINK_PATTERN_ON, BLINK_MODE_ONCE);
		HAL_Delay(2000);
		HAL_UART_ChangeSpeed(&huart1, CONFIG_BAUDRATE_DEFAULT);
		espAt_sendParams(&bleRadio, P_UD, 5, CONFIG_BAUDRATE_FAST, 8, 1, 0, 0);
		espStat = espAt_downloadResponse(&bleRadio);
		espResponse = espAt_returnResponse(&bleRadio);
		if (espResponse == ESPAT_RESPONSE_OK) {
			blink_disable(&ledRed);
		}

		while(1);
	}

	//turn on
	espStat = espAt_pwrOn(&bleRadio);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponse(&bleRadio);

	espStat = espAt_echo(&bleRadio, ESPAT_ECHO_OFF);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponse(&bleRadio);

	//change name
	espStat = espAt_sendString(&bleRadio, S_BHN, CONFIG_DEVICE_NAME);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponse(&bleRadio);

	//turn on BLE
	espStat = espAt_sendParams(&bleRadio, P_BHI, 1, 1);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponse(&bleRadio);


//change timeout
	espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_FAST);
}
