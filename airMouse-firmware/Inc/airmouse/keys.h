/*
 * keys.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_KEYS_H_
#define AIRMOUSE_KEYS_H_

#include <keys_codes.h>
#include <stdint.h>
#include "main.h"
#include "hidcodes.h"
#include "keyboard.h"

extern uint8_t keys_reportMouseButton;
extern uint8_t keys_reportMouseButtonPrevious;

extern uint8_t keys_flagMouseSendReport;
extern uint8_t keys_buttonsChanged ;

extern int8_t keys_reportWheel;
extern uint8_t keys_modifiers;
extern uint8_t keys_keys[6];
extern kbd_keyboard_t mouseButtons;

void keys_init(void);

void keys_readMouse(void);
void keys_readKeyboard(void);


#endif /* AIRMOUSE_KEYS_H_ */
