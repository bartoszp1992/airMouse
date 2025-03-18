/*
 * blink.c
 *
 *  Created on: Mar 18, 2025
 *      Author: bartosz
 *
 *      library for automatic led handling
 */

#include "blink.h"

typedef enum {
	BLINK_RESET, BLINK_SET
} blink_state_t;

//_________________________________________PRIVATE

/*
 * wrapper function for hardware specific action
 */
void blink_write(blink_t *blink, blink_state_t state) {

	if (state == BLINK_SET)
		HAL_GPIO_WritePin(blink->port, blink->pin, GPIO_PIN_SET); //hardware specific
	else if (state == BLINK_RESET)
		HAL_GPIO_WritePin(blink->port, blink->pin, GPIO_PIN_RESET); //hardware specific

}

//_________________________________________PUBLIC

/*
 * Init blink pin
 * @param: blink struct
 * @param: port
 * @param: pin
 * @param: prescaler
 */
void blink_init(blink_t *blink, GPIO_TypeDef *port, uint32_t pin,
		uint32_t prescaler) {

	blink->port = port;
	blink->pin = pin;
	blink->prescaler = prescaler;
	blink->pattern = 0xaaaaaaaa;
	blink->mode = BLINK_MODE_CONTINOUS;
	blink->actualBit = 31;
	blink->masterCounter = 0;
	blink->sleep = BLINK_SLEEP_EN;

}

/*
 * enable blinking
 * @param: pin
 * @param: pattern
 * @param: BLINK_MODE_CONTINOUS/ONCE
 *
 * @retval: BLINK_BUSY/READY
 */
void blink_enable(blink_t *blink, uint32_t pattern,
		blink_mode_t mode) {

	blink->pattern = pattern;
	blink->mode = mode;
	blink->sleep = BLINK_SLEEP_DIS;

	blink->actualBit = 0;
}

/*
 * disable blinking
 * @param: pin
 */
void blink_disable(blink_t *blink) {

	blink->sleep = BLINK_SLEEP_EN;
	blink_write(blink, BLINK_RESET);
}

blink_busyFlag_t blink_checkBusy(blink_t *blink) {
	if (blink->actualBit != 0)
		return BLINK_BUSY;
	else {
		return BLINK_READY;
	}
}

/*
 * run this function in 1ms interrupt, separately for each blink pin
 * @param: blink
 *
 */
void blink_tick(blink_t *blink) {

	if (blink->sleep == BLINK_SLEEP_DIS) {
		blink->masterCounter++;
		if (blink->masterCounter % blink->prescaler == 0) {

			if ((blink->pattern >> (31 - blink->actualBit)) & 0x01)
				blink_write(blink, BLINK_SET);
			else
				blink_write(blink, BLINK_RESET);

			blink->actualBit++;
			if (blink->actualBit > 31) {
				if (blink->mode == BLINK_MODE_CONTINOUS)
					blink->actualBit = 0;
				else
					blink->actualBit = 31;
			}

		}
	} else if (blink->sleep == BLINK_SLEEP_EN) {
		blink->actualBit = 31;
		blink_write(blink, BLINK_RESET);
	}

}
