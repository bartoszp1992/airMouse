/*
 * keys.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_AMHID_H_
#define AIRMOUSE_AMHID_H_

#include <key_codes.h>
#include <stdint.h>
#include "main.h"
#include "keyboard.h"
#include "sensor.h"

#define KEYS_MAX_KEYS 6

//cursor
extern int32_t amhid_mouseXmove;
extern int32_t amhid_mouseYmove;

//mouse
//extern kbd_key_t keys_mousePressedOn[KBD_MAX_PRESSED_BUTTONS];
extern uint8_t amhid_mouseReportButton;
extern uint8_t amhid_mouseReportButtonPrevious;
extern uint8_t amhid_mouseFlagSendReport;
extern int8_t amhid_mouseReportWheel;
//
////keyboard
//extern kbd_key_t keys_qwertyPressedOn[KBD_MAX_PRESSED_BUTTONS];
extern uint8_t amhid_qwertyReportModifiers;
extern uint8_t amhid_qwertyReportModifiersPrevious;
extern int8_t amhid_qwertyReportKeys[6];
extern int8_t amhid_qwertyReportKeysPrevious[6];
extern uint8_t amhid_qwertyFlagSendReport;

extern volatile uint32_t prCounterMouse;
extern volatile uint32_t prCounterKeyboard;

void amhid_init(void);
void amhid_readCursor(void);
void amhid_readButtonsMouse(void);
void amhid_readKeysQwerty(void);


#endif /* AIRMOUSE_AMHID_H_ */
