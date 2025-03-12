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

extern uint8_t keys_mouseReportButton;
extern uint8_t keys_mouseReportButtonPrevious;

extern uint8_t keys_mouseFlagSendReport;
extern uint8_t keys_mouseButtonsChanged ;

extern int8_t keys_mouseReportWheel;
extern uint8_t keys_qwertyReportModifiers;
extern uint8_t keys_keys[6];
extern kbd_keyboard_t mouseButtons;

void keys_init(void);
void keys_readMouse(void);
void keys_readKeyboard(void);


#endif /* AIRMOUSE_KEYS_H_ */
