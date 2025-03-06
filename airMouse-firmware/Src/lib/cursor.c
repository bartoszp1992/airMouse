/*
 * cursor.c
 *
 *  Created on: Mar 6, 2025
 *      Author: bartosz
 *
 *      cursor movement processing
 *      sensitivity and acceleration support
 */

#include "cursor.h"

/*
 * initialize cursor. Made cursor struct first
 * @param: cursor
 *
 * @retval: none
 */
void cursor_init(cursor_t *cursor) {

	cursor->coordinates[CURSOR_AXIS_X] = 0;
	cursor->coordinates[CURSOR_AXIS_Y] = 0;
	cursor->coordinates[CURSOR_AXIS_Z] = 0;

	cursor->output[CURSOR_AXIS_X] = 0;
	cursor->output[CURSOR_AXIS_Y] = 0;
	cursor->output[CURSOR_AXIS_Z] = 0;

	cursor->sensitivity = 30; //10000 are 1
	cursor->acceleration = 150; //100- no acceleration
	cursor->maxVal = 100;

	cursor->accelerationMethod = CURSOR_ACCELERATION_MULTIPLY;
}

/*
 * write input readed from sensor into structure
 * @param: cursor
 * @param: sensor value reading
 * @param: CURSOR_AXIS_X/Y/Z
 */
void cursor_writeInput(cursor_t *cursor, int32_t value, cursor_axis_t axis) {
	cursor->coordinates[axis] = value;
}

/*
 * set sensitivity
 * @param: cursor
 * @param: sensitivity(10000 are 1:1)
 *
 * @retval: none
 */
void cursor_setSensitivity(cursor_t *cursor, int32_t sensitivity) {
	cursor->sensitivity = sensitivity;
}

/*
 * set acceleration
 * @param: cursor
 * @param: acceleration value(100 are without acceleration)
 * @param: acceleration method: CURSOR_ACCELERATION_POWER/MULTIPLY
 *
 * @retval: none
 */
void cursor_setAcceleration(cursor_t *cursor, int32_t acceleration,
		cursor_acceleraton_t method) {
	cursor->acceleration = acceleration;
	cursor->accelerationMethod = method;
}

/*
 * set maximum output value. If value will be counted as higher, will be cut to max
 * @param: cursor
 * @param: max value
 */
void cursor_setMax(cursor_t *cursor, int32_t max) {
	cursor->maxVal = max;
}

/*
 * set axis reversion
 * @param: cursror
 * @param: axis
 * @param: CURSOR_REVERSED/NOT_REVERSED
 */
void cursor_setReverse(cursor_t *cursor, cursor_axis_t axis,
		cursor_reversed_t reversed) {
	cursor->isReversed[axis] = reversed;
}

/*
 * read output
 * @param: cursor
 * @param: axis
 *
 * @retval: output value
 */
int32_t cursor_output(cursor_t *cursor, cursor_axis_t axis) {

	cursor->output[axis] = cursor->coordinates[axis];

	//sensitivity
	cursor->output[axis] =
			((int32_t) cursor->output[axis] * cursor->sensitivity) / 10000;

	//acceleration
	if (cursor->accelerationMethod == CURSOR_ACCELERATION_POWER) {

		cursor->output[axis] = pow((double) abs(cursor->output[axis]),
				((double) cursor->acceleration) / 100);

	} else if (cursor->accelerationMethod == CURSOR_ACCELERATION_MULTIPLY) {

		if (cursor->acceleration != 100) {
			cursor->output[axis] = cursor->output[axis]
					* (cursor->output[axis] * cursor->acceleration) / 100;
		}

	}
	//restore sign
	if (cursor->coordinates[axis] < 0 && cursor->output[axis] > 0)
		cursor->output[axis] = -cursor->output[axis];

	//to high value secure
	if (cursor->output[axis] > cursor->maxVal)
		cursor->output[axis] = cursor->maxVal;
	if (cursor->output[axis] < -cursor->maxVal)
		cursor->output[axis] = -cursor->maxVal;

	if (cursor->isReversed[axis] == CURSOR_REVERSED)
		cursor->output[axis] *= -1;

	return cursor->output[axis];
}
