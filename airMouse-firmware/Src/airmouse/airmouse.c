/*
 * airmouse.c
 *
 *  Created on: Mar 3, 2025
 *      Author: bartosz
 *
 *      todo:
 *      DONE -esp power and boot functions -> espat
 *      DONE -dataReady flag -> lsm6ds
 *      DONE -on moving, not connected: HID reports sending only if connected(depending on the receiving data)
 *      -accelerometer used to correct cursor movement(simple or vector)
 *      -presicion move when the back of the hand is placed upwards
 *      -leds handling
 *      -low battery alert
 *      -sleep mode- gyroscope cant awake
 *      -receiving data from ESP trough DMA.
 *      -keyboard buttons are send too frequently
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

	//backup previous reports
	keys_mouseReportButtonPrevious = keys_mouseReportButton;
	keys_qwertyReportModifiersPrevious = keys_qwertyReportModifiers;
	memcpy(keys_qwertyReportKeysPrevious, keys_qwertyReportKeys,
			sizeof(keys_qwertyReportKeys));

	//read buttons and keys
	keys_readMouse();
	keys_readQwerty();

	/*
	 * check with previous report.
	 * using for detection change from 1 to 0
	 */
	if (keys_mouseReportButton != keys_mouseReportButtonPrevious)
		keys_mouseButtonsChanged = 1;

	if (memcmp(keys_qwertyReportKeys, keys_qwertyReportKeysPrevious,KEYS_MAX_KEYS) != 0  //
			|| keys_qwertyReportModifiers != keys_qwertyReportModifiersPrevious          //
			)
		keys_qwertyKeysChanged = 1;

	//set flag only if press occurs
	if (keys_mouseReportButton
			|| keys_mouseReportWheel
			|| keys_mouseButtonsChanged
			)
		keys_mouseFlagSendReport = 1;

	if (keys_qwertyReportModifiers || keys_qwertyKeysChanged)
		keys_qwertyFlagSendReport = 1;

	//send mouse report
	if (keys_mouseFlagSendReport) {
		sleepTimer = 0; //reset sleep timer
		keys_mouseFlagSendReport = 0;
		//send mouse report
		espAt_sendParams(&bleRadio, P_BHM, 4,  //
				keys_mouseReportButton,        //
				x,                             //
				z,                             //
				keys_mouseReportWheel          //
				);
	}

	//send keyboard report
	if (keys_qwertyFlagSendReport) {
		sleepTimer = 0;
		keys_qwertyFlagSendReport = 0;
		espAt_sendParams(&bleRadio, P_BHK, 7,  //
				keys_qwertyReportModifiers,    //
				keys_qwertyReportKeys[0],      //
				keys_qwertyReportKeys[1],      //
				keys_qwertyReportKeys[2],      //
				keys_qwertyReportKeys[3],      //
				keys_qwertyReportKeys[4],      //
				keys_qwertyReportKeys[5]       //
				);
	}

	sleep_enterSleep();

}
