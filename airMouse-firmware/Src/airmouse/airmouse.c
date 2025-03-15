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
 *      -DONE keyboard buttons are send too frequently
 *      -FN functionality(backspace, dot, comma, etc)
 *      -back and forward buttons are not working
 *      -low polling rate
 *      -esp32 hid in separate library
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

uint32_t reportCounterKBD = 0;
uint32_t reportCounterMUS = 0;
uint32_t downloadCounter = 0;
espat_response_t responseMus = ESPAT_RESPONSE_ERROR;
uint32_t dummyCounter = 0;

void airMouseSetup(void) {

	//_________________________________________KEYS_________________________________________

	amhid_init();

	//_________________________________________RADIO_________________________________________

	radio_init();

	//_________________________________________IMU SENSOR & CURSOR___________________________

	sensor_init();

}
void airMouseProcess(void) {

	//_________________________________________MOUSE_________________________________________
	//read sensor
	amhid_readCursor();
	amhid_mouseReportButtonPrevious = amhid_mouseReportButton;
	amhid_readButtonsMouse();

	/*
	 * check with previous report.
	 * using for detection change from 1 to 0
	 */
	if (amhid_mouseReportButton != amhid_mouseReportButtonPrevious
			|| amhid_mouseReportWheel)
		amhid_mouseFlagSendReport = 1;

	//send mouse report
	if (amhid_mouseFlagSendReport) {
		sleepTimer = 0; //reset sleep timer
		amhid_mouseFlagSendReport = 0;
		//send mouse report
		prCounterMouse++;
		reportCounterMUS++;
		espAt_sendParams(&bleRadio, P_BHM, 4,   //
				amhid_mouseReportButton,        //
				amhid_mouseXmove,               //
				amhid_mouseYmove,               //
				amhid_mouseReportWheel          //
				);

//		espAt_downloadResponse(&bleRadio);
//		responseMus = espAt_returnResponseStatus(&bleRadio);
//		if(responseMus != ESPAT_RESPONSE_OK && responseMus != ESPAT_RESPONSE_PARSING_ERROR){
//			dummyCounter++;
//		}
	}

	//_________________________________________KEYBOARD_______________________________________


	memcpy(amhid_qwertyReportKeysPrevious, amhid_qwertyReportKeys,
			sizeof(amhid_qwertyReportKeys));
	amhid_qwertyReportModifiersPrevious = amhid_qwertyReportModifiers;

	//read buttons and keys

	amhid_readKeysQwerty();

	if (memcmp(amhid_qwertyReportKeys, amhid_qwertyReportKeysPrevious,
	KEYS_MAX_KEYS) != 0
			|| amhid_qwertyReportModifiers
					!= amhid_qwertyReportModifiersPrevious)
		amhid_qwertyFlagSendReport = 1;
	else
		amhid_qwertyFlagSendReport = 0;

	//send keyboard report
	if (amhid_qwertyFlagSendReport) {
		sleepTimer = 0;
		amhid_qwertyFlagSendReport = 0;
		prCounterKeyboard++;
		reportCounterKBD++;
		espAt_sendParams(&bleRadio, P_BHK, 7,   //
				amhid_qwertyReportModifiers,    //
				amhid_qwertyReportKeys[0],      //
				amhid_qwertyReportKeys[1],      //
				amhid_qwertyReportKeys[2],      //
				amhid_qwertyReportKeys[3],      //
				amhid_qwertyReportKeys[4],      //
				amhid_qwertyReportKeys[5]       //
				);
	}

	sleep_enterSleep();

}
