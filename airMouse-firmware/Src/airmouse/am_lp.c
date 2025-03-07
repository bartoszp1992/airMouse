/*
 * lp.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#include <am_lp.h>

extern espat_radio_t bleRadio;
extern void SystemClock_Config(void);

void sleep(void){
	//peripheral sleep
	espAt_pwrOff(&bleRadio);

	//MCU sleep
	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	//MCU wkup
	HAL_ResumeTick();
	SystemClock_Config();

	//peripheral wkup
	HAL_UART_ChangeSpeed(&huart1, BAUDRATE_DEFAULT);
	espAt_pwrOn(&bleRadio);
	espAt_getResponse(&bleRadio);
	espAt_sendString(&bleRadio, S_BHN, "neuroGlide");
	espAt_sendParams(&bleRadio, P_BHI, 1, 1);
	espAt_sendParams(&bleRadio, P_UC, 5, BAUDRATE_FAST, 8, 1, 0, 0);
	espAt_getResponse(&bleRadio);
	HAL_UART_ChangeSpeed(&huart1, BAUDRATE_FAST);
}
