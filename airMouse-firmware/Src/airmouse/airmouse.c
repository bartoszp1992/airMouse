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
espat_response_t espResponse;
lsm6ds_state_t sensorStat;

uint8_t flagMouseSendReport = 0;
int32_t x = 0;
int32_t z = 0;

void airMouseSetup(void) {

	//_________________________________________KEYS_________________________________________

	//init mouse buttons
	kbd_init(&mouseButtons, 10, 1, 10, KBD_RESET);
	kbd_setColumns(&mouseButtons,
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
	kbd_setColumns(&qwerty,
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
	kbd_setRows(&qwerty,
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
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponseStatus(&bleRadio);

	espStat = espAt_sendString(&bleRadio, S_BHN, "neuroGlide");
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponseStatus(&bleRadio);

	espStat = espAt_sendParams(&bleRadio, P_BHI, 1, 1);
	espStat = espAt_downloadResponse(&bleRadio);
	espResponse = espAt_returnResponseStatus(&bleRadio);

	//change baudrate
	espAt_sendParams(&bleRadio, P_UC, 5, BAUDRATE_FAST, 8, 1, 0, 0);
	HAL_Delay(200);
	HAL_UART_ChangeSpeed(&huart1, BAUDRATE_FAST);

	//_________________________________________IMU SENSOR_________________________________________

	sensorStat = lsm6ds_init(&mems, LSM6DS_ADDR_SA0_L, &hi2c1, 100, 100);
	sensorStat = lsm6ds_reset(&mems);

	//	sensorStat = lsm6ds_setXLOutputDataRate(&mems, LSM6DS_ODR_XL_12_5_HZ);
	//	sensorStat = lsm6ds_setXLFullScale(&mems, LSM6DS_FS_XL_16G);

	sensorStat = lsm6ds_setGRLowPass(&mems, LSM6DS_FTYPE_LOW);
	sensorStat = lsm6ds_setGROutputDataRate(&mems, LSM6DS_ODR_G_416_HZ);
	sensorStat = lsm6ds_setGRFullScale(&mems, LSM6DS_FS_G_500DPS);
	sensorStat = lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);

	//_________________________________________CUROSR_________________________________________
	cursor_init(&cursor);
	cursor_setReverse(&cursor, CURSOR_AXIS_X, CURSOR_REVERSED);
	cursor_setReverse(&cursor, CURSOR_AXIS_Y, CURSOR_NOT_REVERSED);
	cursor_setReverse(&cursor, CURSOR_AXIS_Z, CURSOR_NOT_REVERSED);

	cursor_setSensitivity(&cursor, 10);
	cursor_setAcceleration(&cursor, 140, CURSOR_ACCELERATION_MULTIPLY);
	cursor_setMax(&cursor, 40);

}
void airMouseProcess(void) {

	//read sensor
	if (lsm6ds_flagDataReadyRead(&mems) == LSM6DS_DATA_READY) {

		//update gyro data
		sensorStat = lsm6ds_updateGR(&mems);

		//send gyro data to cursor lib
		cursor_writeInput(&cursor, lsm6ds_readGR(&mems, LSM6DS_AXIS_X),
				CURSOR_AXIS_X);
		cursor_writeInput(&cursor, lsm6ds_readGR(&mems, LSM6DS_AXIS_Y),
				CURSOR_AXIS_Y);
		cursor_writeInput(&cursor, lsm6ds_readGR(&mems, LSM6DS_AXIS_Z),
				CURSOR_AXIS_Z);

		//read mouse movement from cursor lib
		x = cursor_output(&cursor, CURSOR_AXIS_X);
		z = cursor_output(&cursor, CURSOR_AXIS_Z);

		//set flag only if movement occurs
		if (abs(x) > 0 || abs(z) > 0)
			flagMouseSendReport = 1;

	}

	//read mouse buttons
	amKeys_readMouse();
	//set flag only if press occurs
	if (amKeys_reportMouseButton || amKeys_reportWheel)
		flagMouseSendReport = 1;

	//send report
	if (flagMouseSendReport) {
		flagMouseSendReport = 0;

		//send mouse report
		espAt_sendParams(&bleRadio, P_BHM, 4, amKeys_reportMouseButton, x, z,
				amKeys_reportWheel);
	}
}
/*
 UNUSED CODE

 kbd_defineLayout(&mouseButtons,
 HID_MOUSE_MASK_L,
 HID_MOUSE_MASK_R,
 HID_MOUSE_MASK_M,
 HID_MOUSE_MASK_FWD,
 HID_MOUSE_MASK_BCK, HID_SPECIAL_WHEELUP, HID_SPECIAL_WHEELDN,
 HID_SPECIAL_DPI, HID_SPECIAL_HOME, HID_SPECIAL_PRC);


 kbd_defineLayout(&qwerty,
 HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7,
 HID_KEY_8, HID_KEY_9, HID_KEY_0,
 HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T, HID_KEY_Y,
 HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P,
 HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G, HID_KEY_H,
 HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_ENTER,
 HID_MOD_MASK_LSHIFT, HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V,
 HID_KEY_B, HID_KEY_N, HID_KEY_M, HID_MOD_MASK_RSHIFT, HID_KEY_UP,
 HID_MOD_MASK_LCTRL, HID_SPECIAL_FN, HID_MOD_MASK_LGUI, HID_MOD_MASK_LALT,
 HID_KEY_SPACE, HID_MOD_MASK_RALT, HID_KEY_LEFT, HID_KEY_DOWN,
 HID_KEY_RIGHT);


 */

