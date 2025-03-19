/*
 * led.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef LIB_LED_H_
#define LIB_LED_H_

#include "blink.h"

extern blink_t ledRed;
extern blink_t ledGreen;
extern blink_t ledBlue;

void led_init(void);


#endif /* LIB_LED_H_ */
