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
#define BLINK_PATTERN_SHORT			0x80000000
#define BLINK_PATTERN_ON 			0xffffffff

typedef enum{
	BLINK_MODE_CONTINOUS,
	BLINK_MODE_SINGLE
}blink_mode_t;

typedef enum {
	BLINK_STATUS_BUSY,
	BLINK_STATUS_READY
}blink_status_t;



typedef struct{
	GPIO_TypeDef *port; //hardware specific
	uint32_t pin;		//hardware specific
	uint32_t prescaler;
	uint32_t pattern;
	blink_mode_t mode;
	uint8_t actualBit;
	uint32_t masterCounter; //increments every tick function call
	uint32_t counter;		//increments only if masterCounter % prescaler == 0
	blink_status_t status;
}blink_t;


void blink_init(blink_t *blink, GPIO_TypeDef* port, uint32_t pin, uint32_t prescaler);
void blink_enable(blink_t *blink, uint32_t pattern, blink_mode_t mode);
void blink_disable(blink_t *blink);

blink_status_t blink_status(blink_t *blink);

void blink_tick(blink_t *blink);

#endif /* LIB_BLINK_H_ */
