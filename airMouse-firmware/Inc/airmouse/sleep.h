/*
 * sleep.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef LIB_SLEEP_H_
#define LIB_SLEEP_H_

#include "espat.h"
#include "config.h"
#include "lsm6ds.h"
#include "led.h"

#define ONTIME 80000 //in seconds

extern volatile uint32_t sleepTimer;
extern volatile uint8_t sleepFlag;

void sleep_enterSleep(void);
void sleep_timerInc(void);


#endif /* LIB_LP_H_ */
