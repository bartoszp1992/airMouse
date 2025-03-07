/*
 * am_keys.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_AM_KEYS_H_
#define AIRMOUSE_AM_KEYS_H_

#include <stdint.h>
#include "main.h"
#include "hidcodes.h"
#include "keyboard.h"

extern uint8_t amKeys_reportMouseButton;
extern int8_t amKeys_reportWheel;
extern uint8_t amKeys_modifiers;
extern char amKeys_keys[6];
extern kbd_keyboard_t mouseButtons;;

void amKeys_readMouse(void);


#endif /* AIRMOUSE_AM_KEYS_H_ */
