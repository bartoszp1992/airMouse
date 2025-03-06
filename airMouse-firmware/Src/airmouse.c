/*
 * airmouse.c
 *
 *  Created on: Mar 3, 2025
 *      Author: bartosz
 *
 *      todo:
 *      DONE esp power and boot functions -> espat
 *      DONE dataReady flag -> lsm6ds
 *      receiving data from esp -> espat
 *      DONE on moving, not connected: HID reports sending only if connected(depending on the receiving data)
 *      accelerometer used to correct cursor movement(simple or vector)
 *      presicion move when the back of the hand is placed upwards
 *      sending keyboard data
 *      sending mouse button data
 *      leds handling
 *      low battery alert
 *      low power mode
 *      keyboard layout
 *
 *      name propositions:
 *      IMU		flow
 *      neuro	glide
 *      Aero	sync
 *      Warp	pointer
 *      Void	hover
 *      Omni	grip
 *      hyper	vortex
 *      hover

 *
 */

#include "airmouse.h"

extern void SystemClock_Config(void);

//PV
espat_radio_t bleRadio;
lsm6ds_sensor_t mems;
kbd_keyboard_t qwerty;
kbd_keyboard_t mouseButtons;
cursor_t cursor;

espat_state_t espStat;
lsm6ds_state_t sensorStat;

uint8_t mouseButtonState = 0;

volatile uint32_t ledCounter = 0;
volatile uint32_t onCounter = 0;

void airMouseSetup(void) {

	//_________________________________________KEYS_________________________________________

	//init mouse buttons
	kbd_init(&mouseButtons, 10, 1, 10, KBD_RESET);
	kbd_set_columns(&mouseButtons,
	MUS_LB_GPIO_Port, MUS_LB_Pin,
	MUS_RB_GPIO_Port, MUS_RB_Pin,
	MUS_MB_GPIO_Port, MUS_MB_Pin,
	MUS_FWD_GPIO_Port, MUS_FWD_Pin,
	MUS_BCK_GPIO_Port, MUS_BCK_Pin,
	MUS_UP_GPIO_Port, MUS_UP_Pin,
	MUS_DN_GPIO_Port, MUS_DN_Pin,
	MUS_DPI_GPIO_Port, MUS_DPI_Pin,
	MUS_HOME_GPIO_Port, MUS_HOME_Pin,
	MUS_PRC_GPIO_Port, MUS_PRC_Pin);

	//init qwerty
	kbd_init(&qwerty, 10, 5, 10, KBD_RESET);
	kbd_set_columns(&qwerty,
	KBD_COL1_GPIO_Port, KBD_COL1_Pin,
	KBD_COL2_GPIO_Port, KBD_COL2_Pin,
	KBD_COL3_GPIO_Port, KBD_COL3_Pin,
	KBD_COL4_GPIO_Port, KBD_COL4_Pin,
	KBD_COL5_GPIO_Port, KBD_COL5_Pin,
	KBD_COL6_GPIO_Port, KBD_COL6_Pin,
	KBD_COL7_GPIO_Port, KBD_COL7_Pin,
	KBD_COL8_GPIO_Port, KBD_COL8_Pin,
	KBD_COL9_GPIO_Port, KBD_COL9_Pin,
	KBD_COL10_GPIO_Port, KBD_COL10_Pin);
	kbd_set_rows(&qwerty,
	KBD_ROW1_GPIO_Port, KBD_ROW1_Pin,
	KBD_ROW2_GPIO_Port, KBD_ROW2_Pin,
	KBD_ROW3_GPIO_Port, KBD_ROW3_Pin,
	KBD_ROW4_GPIO_Port, KBD_ROW4_Pin,
	KBD_ROW5_GPIO_Port, KBD_ROW5_Pin);

	//_________________________________________RADIO_________________________________________

	espStat = espAt_init(&bleRadio, &huart1, 2, 1500);
	espStat = espAt_defineEn(&bleRadio, ESP_EN_GPIO_Port, ESP_EN_Pin);
	espStat = espAt_defineBoot(&bleRadio, ESP_BOOT_GPIO_Port, ESP_BOOT_Pin);
	//enter download mode or reset ESP
	if (HAL_GPIO_ReadPin(MUS_BCK_GPIO_Port, MUS_BCK_Pin) == GPIO_PIN_RESET)
		espStat = espAt_enterDownload(&bleRadio);

	//turn on
	espStat = espAt_pwrOn(&bleRadio);
	espStat = espAt_getResponse(&bleRadio);
	HAL_Delay(200);

//	espStat = espAt_sendCommand(&bleRadio, G_RST);
//	espStat = espAt_receive(&bleRadio, rxBuffer, sizeof(rxBuffer));
//	HAL_Delay(2000);

	espStat = espAt_sendString(&bleRadio, S_BHN, "neuroGlide");
	espStat = espAt_getResponse(&bleRadio);
	HAL_Delay(200);

	espStat = espAt_sendParams(&bleRadio, P_BHI, 1, 1);
	espStat = espAt_getResponse(&bleRadio);
	HAL_Delay(200);

	//change baudrate
	espAt_sendParams(&bleRadio, P_UC, 5, 2000000, 8, 1, 0, 0);
	HAL_Delay(200);
	HAL_UART_ChangeSpeed(&huart1, 2000000);

	//_________________________________________IMU SENSOR_________________________________________

	sensorStat = lsm6ds_init(&mems, LSM6DS_ADDR_SA0_L, &hi2c1, 100, 100);
	sensorStat = lsm6ds_reset(&mems);

	//	sensorStat = lsm6ds_setXLOutputDataRate(&mems, LSM6DS_ODR_XL_12_5_HZ);
	//	sensorStat = lsm6ds_setXLFullScale(&mems, LSM6DS_FS_XL_16G);

	sensorStat = lsm6ds_setGRLowPass(&mems, LSM6DS_FTYPE_LOW);
	sensorStat = lsm6ds_setGROutputDataRate(&mems, LSM6DS_ODR_G_416_HZ);
	sensorStat = lsm6ds_setGRFullScale(&mems, LSM6DS_FS_G_1000DPS);
	sensorStat = lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);

	//_________________________________________CUROSR_________________________________________
	cursor_init(&cursor);
	cursor_setReverse(&cursor, CURSOR_AXIS_X, CURSOR_REVERSED);
	cursor_setReverse(&cursor, CURSOR_AXIS_Y, CURSOR_NOT_REVERSED);
	cursor_setReverse(&cursor, CURSOR_AXIS_Z, CURSOR_NOT_REVERSED);

	cursor_setSensitivity(&cursor, 15);
	cursor_setAcceleration(&cursor, 130, CURSOR_ACCELERATION_MULTIPLY);
	cursor_setMax(&cursor, 40);

}
void airMouseProcess(void) {


	if (lsm6ds_flagDataReadyRead(&mems) == LSM6DS_DATA_READY) {

		sensorStat = lsm6ds_updateGR(&mems);

		cursor_writeInput(&cursor, lsm6ds_readGR(&mems, LSM6DS_AXIS_X), CURSOR_AXIS_X);
		cursor_writeInput(&cursor, lsm6ds_readGR(&mems, LSM6DS_AXIS_Y), CURSOR_AXIS_Y);
		cursor_writeInput(&cursor, lsm6ds_readGR(&mems, LSM6DS_AXIS_Z), CURSOR_AXIS_Z);

		mouseButtonState = 0;
		mouseButtonState = !HAL_GPIO_ReadPin(MUS_LB_GPIO_Port, MUS_LB_Pin) << 0
				| !HAL_GPIO_ReadPin(MUS_RB_GPIO_Port, MUS_RB_Pin) << 1
				| !HAL_GPIO_ReadPin(MUS_MB_GPIO_Port, MUS_MB_Pin) << 2
				| !HAL_GPIO_ReadPin(MUS_FWD_GPIO_Port, MUS_FWD_Pin) << 3
				| !HAL_GPIO_ReadPin(MUS_BCK_GPIO_Port, MUS_BCK_Pin) << 4;

		int32_t x = cursor_output(&cursor, CURSOR_AXIS_X);
//		int32_t y = cursor_output(&cursor, CURSOR_Y);
		int32_t z = cursor_output(&cursor, CURSOR_AXIS_Z);

		if (mouseButtonState != 0 || abs(x) > 0 || abs(z) > 0) {
			espAt_sendParams(&bleRadio, P_BHM, 4, mouseButtonState, x, z, 0);
			if (qwerty.stateMatrix[0]) {
				espAt_getResponse(&bleRadio);
				sensorStat++;
			}
		}

	}

}

void airMouseTick(void) {
	ledCounter++;
	onCounter++;
	if (ledCounter % 2000 == 0) {
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, 1);
		onCounter = 0;
	}

	if (onCounter > 5)
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, 0);

	kbd_scanning(&qwerty);
	kbd_scanning(&mouseButtons);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GYRO_INT_Pin) {
		lsm6ds_flagDataReadySet(&mems);
	}
}

void ledOn(void) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 1);
}

void ledOff(void) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 0);
}

void sleep(void){

	//enter sleep
	//peripheral


	//MCU
	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	//wake up
	HAL_ResumeTick();
	SystemClock_Config();

}

