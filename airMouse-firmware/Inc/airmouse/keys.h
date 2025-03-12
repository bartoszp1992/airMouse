/*
 * keys.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_KEYS_H_
#define AIRMOUSE_KEYS_H_

#include <key_codes.h>
#include <stdint.h>
#include "main.h"
#include "keyboard.h"

#define KEYS_MAX_KEYS 6

//mouse
extern kbd_key_t keys_mousePressedOn[KBD_MAX_PRESSED_BUTTONS];
extern uint8_t keys_mouseReportButton;
extern uint8_t keys_mouseReportButtonPrevious;
extern uint8_t keys_mouseFlagSendReport;
extern uint8_t keys_mouseButtonsChanged;
extern int8_t keys_mouseReportWheel;

//keyboard
extern kbd_key_t keys_qwertyPressedOn[KBD_MAX_PRESSED_BUTTONS];
extern uint8_t keys_qwertyReportModifiers;
extern uint8_t keys_qwertyReportModifiersPrevious;
extern uint8_t keys_qwertyReportKeys[6];
extern uint8_t keys_qwertyReportKeysPrevious[6];
extern uint8_t keys_qwertyFlagSendReport;
extern uint8_t keys_qwertyKeysChanged;

void keys_init(void);
void keys_readMouse(void);
void keys_readQwerty(void);


#endif /* AIRMOUSE_KEYS_H_ */
