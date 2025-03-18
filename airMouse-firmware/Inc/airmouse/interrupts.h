/*
 * interrupts.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef LIB_INTERRUPTS_H_
#define LIB_INTERRUPTS_H_

#include <sleep.h>
#include <stdint.h>
#include "main.h"
#include "keyboard.h"
#include "lsm6ds.h"
#include "blink.h"

void systemTick(void);

#endif /* LIB_INTERRUPTS_H_ */
