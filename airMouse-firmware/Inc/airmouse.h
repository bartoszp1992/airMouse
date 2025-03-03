/*
 * airmouse.h
 *
 *  Created on: Mar 3, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_H_
#define AIRMOUSE_H_

#include "espat.h"
#include "lsm6ds.h"
#include <math.h>
#include "keyboard.h"
#include <stdint.h>


void airMouseSetup(void);
void airMouseProcess(void);
void airMouseTick(void);

void ledOn(void);
void ledOff(void);



#endif /* AIRMOUSE_H_ */
