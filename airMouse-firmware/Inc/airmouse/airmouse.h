/*
 * airmouse.h
 *
 *  Created on: Mar 3, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_H_
#define AIRMOUSE_H_

#include <key_codes.h>
#include <keys.h>
#include <led.h>
#include "espat.h"
#include "lsm6ds.h"
#include <math.h>
#include <sleep.h>
#include "keyboard.h"
#include <stdint.h>
#include "cursor.h"
#include "config.h"
#include "radio.h"
#include "sensor.h"



void airMouseSetup(void);
void airMouseProcess(void);



#endif /* AIRMOUSE_H_ */
