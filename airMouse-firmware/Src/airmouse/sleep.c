/*
 * lp.c
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#include <sleep.h>

extern espat_radio_t bleRadio;
extern lsm6ds_sensor_t mems;
extern void SystemClock_Config(void);

volatile uint32_t sleepTimer = 0;
volatile uint8_t sleepFlag = 0;

void sleep_enterSleep(void) {

	if (sleepFlag) {
		sleepFlag = 0;
		sleepTimer = 0;

		//radio sleep
		espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_LONG);
		espAt_sendParams(&bleRadio, P_BD, 1, 0);
		espAt_downloadResponse(&bleRadio);
		espAt_pwrOff(&bleRadio);

		//sensor sleep
		lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_DIS);

				lsm6ds_setGROutputDataRate(&mems, LSM6DS_ODR_G_PWR_DN);

		//led off
		ledOff(LED_GREEN);
		ledOff(LED_BLUE);
		ledOff(LED_RED);

		//MCU sleep
		HAL_SuspendTick();
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

		//MCU wkup
		HAL_ResumeTick();
		SystemClock_Config();

		//peripheral wkup

		HAL_UART_ChangeSpeed(&huart1, CONFIG_BAUDRATE_DEFAULT);

		//turn on
		espAt_pwrOn(&bleRadio);
		espAt_downloadResponse(&bleRadio);

		//change name
		espAt_sendString(&bleRadio, S_BHN, CONFIG_DEVICE_NAME);
		espAt_downloadResponse(&bleRadio);

		//turn on BLE
		espAt_sendParams(&bleRadio, P_BHI, 1, 1);
		espAt_downloadResponse(&bleRadio);

		//change baudrate
		espAt_sendParams(&bleRadio, P_UC, 5, CONFIG_BAUDRATE_FAST, 8, 1, 0, 0);
		HAL_Delay(200);
		HAL_UART_ChangeSpeed(&huart1, CONFIG_BAUDRATE_FAST);

		espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_FAST);

		//wake up sensor
		lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);

				lsm6ds_setGROutputDataRate(&mems, CONFIG_IMU_ODR);
				lsm6ds_setGRFullScale(&mems, CONFIG_DPS);

	}

}

void sleep_timerInc(void) {
	sleepTimer++;
	if (sleepTimer > CONFIG_ONTIME*1000)
		sleepFlag = 1;
}
