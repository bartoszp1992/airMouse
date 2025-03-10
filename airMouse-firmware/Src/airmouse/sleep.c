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

		//radio sleep
		espAt_pwrOff(&bleRadio);

		//sensor sleep
//		lsm6ds_setGROutputDataRate(&mems, LSM6DS_ODR_G_PWR_DN);
		lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_DIS);

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

		//wake up sensor
//		lsm6ds_setGROutputDataRate(&mems, CONFIG_IMU_ODR);
//		lsm6ds_setGRFullScale(&mems, LSM6DS_FS_G_500DPS);
		lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);

	}

}

void sleep_timerInc(void) {
	sleepTimer++;
	if (sleepTimer > ONTIME)
		sleepFlag = 1;
}
