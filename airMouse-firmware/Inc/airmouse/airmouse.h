/*
 * airmouse.h
 *
 *  Created on: Mar 3, 2025
 *      Author: bartosz
 */

#ifndef AIRMOUSE_H_
#define AIRMOUSE_H_

#include <am_lp.h>
#include "espat.h"
#include "lsm6ds.h"
#include <math.h>
#include "keyboard.h"
#include <stdint.h>
#include "cursor.h"
#include "hidcodes.h"
#include "config.h"
#include "am_keys.h"


void airMouseSetup(void);
void airMouseProcess(void);



#endif /* AIRMOUSE_H_ */
