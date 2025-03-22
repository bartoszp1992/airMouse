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
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

extern airmouse_t airmouse;

extern blink_t ledRed;
extern blink_t ledGreen;
extern blink_t ledBlue;

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
		espAt_sendParams(&bleRadio, P_BHI, 1, 0);
		espAt_downloadResponse(&bleRadio);
		espAt_pwrOff(&bleRadio);
		airmouse.state = AIRMOUSE_STATE_DISCONNECTED;

		//sensor sleep
		lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_DIS);
		lsm6ds_setGROutputDataRate(&mems, LSM6DS_ODR_G_PWR_DN);

		lsm6ds_setGRMode(&mems, LSM6DS_G_HM_MODE_HI_PERFORMANCE_DIS);
		lsm6ds_setXLMode(&mems, LSM6DS_XL_HM_MODE_HI_PERFORMANCE_DIS);
		lsm6ds_setGRSleep(&mems, LSM6DS_G_SLEEP_EN);

		//led off0
		blink_enable(&ledRed, BLINK_PATTERN_FAST_SLOW, BLINK_MODE_SINGLE);
		while(blink_returnStatus(&ledRed)==BLINK_STATUS_BUSY);
		blink_disable(&ledGreen);
		blink_disable(&ledBlue);


		HAL_UART_MspDeInit(&huart1);
		HAL_I2C_MspDeInit(&hi2c1);
		HAL_ADC_MspDeInit(&hadc1);

		//MCU sleep
		HAL_PWREx_EnableLowPowerRunMode();
		HAL_RCC_DeInit();
		HAL_SuspendTick();
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

		//MCU wkup
		HAL_ResumeTick();
		SystemClock_Config();
		HAL_PWREx_DisableLowPowerRunMode();

		HAL_UART_MspInit(&huart1);
		HAL_I2C_MspInit(&hi2c1);
		HAL_ADC_MspInit(&hadc1);

		//turn on
		espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_LONG);
		espAt_pwrOn(&bleRadio);
		espAt_downloadResponse(&bleRadio);

		//turn off echo
		espAt_echo(&bleRadio, ESPAT_ECHO_OFF);
		espAt_downloadResponse(&bleRadio);

		//change name
		espAt_sendString(&bleRadio, S_BHN, CONFIG_DEVICE_NAME);
		espAt_downloadResponse(&bleRadio);

		//turn on BLE
		espAt_sendParams(&bleRadio, P_BHI, 1, 1);
		espAt_downloadResponse(&bleRadio);

		espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_FAST);

		//wake up sensor

		lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);//lp problem?
		lsm6ds_setGROutputDataRate(&mems, CONFIG_IMU_ODR);
		lsm6ds_setGRFullScale(&mems, CONFIG_DPS);

		lsm6ds_setGRMode(&mems, LSM6DS_G_HM_MODE_HI_PERFORMANCE_EN);
		lsm6ds_setXLMode(&mems, LSM6DS_XL_HM_MODE_HI_PERFORMANCE_EN);
		lsm6ds_setGRSleep(&mems, LSM6DS_G_SLEEP_DIS);

	}

}

void sleep_timerInc(void) {
	sleepTimer++;
	if (sleepTimer > CONFIG_ONTIME * 1000)
		sleepFlag = 1;
}
