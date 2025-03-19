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
 *      -DONE keyboard buttons are send too frequently
 *      -FN functionality(backspace, dot, comma, etc)
 *      -back and forward buttons are not working
 *      -DONE low polling rate
 *      -DONEfix busy check in led_blink library-
 *      -you can use BLEHIDINIT=0 to disconnect
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
 *      remember to run first with mouse FWD button to set esp32 baudrate to 1152000

 *
 */

#include "airmouse.h"

extern void SystemClock_Config(void);

airmouse_t airmouse;
uint32_t counterConn = 0;
uint32_t counterDisconn = 0;
volatile uint8_t disconn = 0;

void airMouseSetup(void) {

	airmouse.reportCounterKBD = 0;
	airmouse.reportCounterMUS = 0;
	airmouse.downloadCounter = 0;
	airmouse.responseMus = ESPAT_RESPONSE_ERROR;
	airmouse.responseKbd = ESPAT_RESPONSE_ERROR;
	airmouse.state = AIRMOUSE_STATE_DISCONNECTED;
	airmouse.message = ESPAT_MESSAGE_BLEHIDDISCONN;

	//_________________________________________LEDS
	led_init();

	//_________________________________________KEYS

	amhid_init();

	//_________________________________________RADIO

	radio_init();

	//_________________________________________IMU SENSOR & CURSOR

	sensor_init();

}
void airMouseProcess(void) {

	if (airmouse.state == AIRMOUSE_STATE_DISCONNECTED) {

		counterDisconn++;

		if (blink_status(&ledBlue) == BLINK_STATUS_READY)
			blink_enable(&ledBlue, BLINK_PATTERN_BLINK_X16,
					BLINK_MODE_SINGLE);

		espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_LONG);

		espAt_downloadMessage(&bleRadio);
		airmouse.message = espAt_returnMessage(&bleRadio);
		if (airmouse.message == ESPAT_MESSAGE_BLEHIDCONN) {
			airmouse.state = AIRMOUSE_STATE_CONNECTED;
		}

	} else {

		counterConn++;

		if(disconn == 1){
			disconn = 0;
			espAt_sendParams(&bleRadio, P_BD, 1, 0);
		}

		if (blink_status(&ledBlue) == BLINK_STATUS_READY)
			blink_enable(&ledBlue, BLINK_PATTERN_SHORT,
					BLINK_MODE_SINGLE);


		espAt_setRxTimeout(&bleRadio, CONFIG_TIMEOUT_RX_FAST);

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
			airmouse.reportCounterMUS++;
			espAt_sendParams(&bleRadio, P_BHM, 4,   //
					amhid_mouseReportButton,        //
					amhid_mouseXmove,               //
					amhid_mouseYmove,               //
					amhid_mouseReportWheel          //
					);

			espAt_downloadResponse(&bleRadio);
			airmouse.responseMus = espAt_returnResponse(&bleRadio);

			if (airmouse.responseMus != ESPAT_RESPONSE_OK
					&& airmouse.responseMus != ESPAT_RESPONSE_PARSING_ERROR) {
				airmouse.state = AIRMOUSE_STATE_DISCONNECTED;
			}

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
			airmouse.reportCounterKBD++;
			espAt_sendParams(&bleRadio, P_BHK, 7,   //
					amhid_qwertyReportModifiers,    //
					amhid_qwertyReportKeys[0],      //
					amhid_qwertyReportKeys[1],      //
					amhid_qwertyReportKeys[2],      //
					amhid_qwertyReportKeys[3],      //
					amhid_qwertyReportKeys[4],      //
					amhid_qwertyReportKeys[5]       //
					);
			espAt_downloadResponse(&bleRadio);
			airmouse.responseKbd = espAt_returnResponse(&bleRadio);

			if (airmouse.responseKbd != ESPAT_RESPONSE_OK
					&& airmouse.responseKbd != ESPAT_RESPONSE_PARSING_ERROR) {
				airmouse.state = AIRMOUSE_STATE_DISCONNECTED;
			}
		}
	}

	sleep_enterSleep();

}
