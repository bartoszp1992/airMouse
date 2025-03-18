/*
 * blink.h
 *
 *  Created on: Mar 18, 2025
 *      Author: bartosz
 */

#ifndef LIB_BLINK_H_
#define LIB_BLINK_H_

#include <stdint.h>
#include "gpio.h"

#define BLINK_PATTERN_BLINK_X1 		0xffff0000
#define BLINK_PATTERN_BLINK_X2 		0xff00ff00
#define BLINK_PATTERN_BLINK_X4 		0xf0f0f0f0
#define BLINK_PATTERN_BLINK_X8 		0xcccccccc
#define BLINK_PATTERN_BLINK_X16 	0xaaaaaaaa
#define BLINK_PATTERN_SLOW_FAST		0xf83c38ca
#define BLINK_PATTERN_FAST_SLOW		0xace3c3e0
#define BLINK_PATTERN_ON 			0xffffffff

typedef enum{
	BLINK_MODE_CONTINOUS,
	BLINK_MODE_ONCE
}blink_mode_t;

typedef enum {
	BLINK_SLEEP_DIS,
	BLINK_SLEEP_EN
}blink_sleep_t;

typedef enum {
	BLINK_READY,
	BLINK_BUSY
}blink_busyFlag_t;

typedef struct{
	GPIO_TypeDef *port; //hardware specific
	uint32_t pin;		//hardware specific
	uint32_t prescaler;
	uint32_t pattern;
	blink_mode_t mode;
	uint8_t actualBit; //used also for busy recognision: 0- ready; any other value- busy
	uint32_t masterCounter; //increments every tick function call
	uint32_t counter;		//increments only if masterCounter % prescaler == 0
	blink_sleep_t sleep;
	blink_busyFlag_t busyFlag;

}blink_t;


void blink_init(blink_t *blink, GPIO_TypeDef* port, uint32_t pin, uint32_t prescaler);
void blink_enable(blink_t *blink, uint32_t pattern, blink_mode_t mode);
void blink_disable(blink_t *blink);

blink_busyFlag_t blink_checkBusy(blink_t *blink);

void blink_tick(blink_t *blink);

#endif /* LIB_BLINK_H_ */
