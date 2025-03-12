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
 *      sleep mode- gyroscope cant awake
 *      receiving data from ESP trough DMA.
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


int32_t x = 0;
int32_t z = 0;

void airMouseSetup(void) {

	//_________________________________________KEYS_________________________________________

	keys_init();

	//_________________________________________RADIO_________________________________________

	radio_init();

	//_________________________________________IMU SENSOR & CURSOR___________________________

	sensor_init();


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

		if (abs(x) > 0 || abs(z) > 0)
			keys_mouseFlagSendReport = 1;

	}

	//backup previous report
	keys_mouseReportButtonPrevious = keys_mouseReportButton;

	//read mouse buttons
	keys_readMouse();

	//read qwerty buttons
	keys_readKeyboard();

	/*
	 * check with previous report.
	 * using for detection change from 1 to 0
	 */
	if(keys_mouseReportButton != keys_mouseReportButtonPrevious)
		keys_mouseButtonsChanged = 1;


	//set flag only if press occurs
	if (keys_mouseReportButton || keys_mouseReportWheel || keys_mouseButtonsChanged)
		keys_mouseFlagSendReport = 1;

	//send report
	if (keys_mouseFlagSendReport) {
		sleepTimer = 0;//reset sleep timer
		keys_mouseFlagSendReport = 0;
		//send mouse report
		espAt_sendParams(&bleRadio, P_BHM, 4, keys_mouseReportButton, x, z,
				keys_mouseReportWheel);
	}

	sleep_enterSleep();

}
