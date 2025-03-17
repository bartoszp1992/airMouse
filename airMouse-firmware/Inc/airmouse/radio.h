/*
 * radio.h
 *
 *  Created on: Mar 10, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_RADIO_H_
#define AIRMOUSE_RADIO_H_

#include "espat.h"
#include "usart.h"
#include "config.h"
#include "led.h"

extern espat_radio_t bleRadio;

void radio_init(void);

#endif /* AIRMOUSE_RADIO_H_ */
