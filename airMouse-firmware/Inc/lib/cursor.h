/*
 * cursor.h
 *
 *  Created on: Mar 6, 2025
 *      Author: bartosz
 */

#ifndef LIB_CURSOR_H_
#define LIB_CURSOR_H_

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
	CURSOR_AXIS_X = 0, CURSOR_AXIS_Y = 1, CURSOR_AXIS_Z = 2
} cursor_axis_t;

typedef enum {
	CURSOR_NOT_REVERSED = 0, CURSOR_REVERSED = 1
} cursor_reversed_t;

typedef enum {
	CURSOR_ACCELERATION_POWER = 0, CURSOR_ACCELERATION_MULTIPLY = 1
} cursor_acceleraton_t;

typedef struct {

	int32_t coordinates[3]; //input from sensor
	int32_t output[3]; //output cursor movement
	cursor_reversed_t isReversed[3]; //stores information that axis is reversed
	cursor_acceleraton_t accelerationMethod;

	int32_t sensitivity;
	int32_t acceleration;
	int32_t maxVal;

} cursor_t;

void cursor_init(cursor_t *cursor);
void cursor_writeInput(cursor_t *cursor, int32_t value, cursor_axis_t axis);
void cursor_setSensitivity(cursor_t *cursor, int32_t sensitivity);
void cursor_setAcceleration(cursor_t *cursor, int32_t acceleration,
		cursor_acceleraton_t method);
void cursor_setMax(cursor_t *cursor, int32_t max);
void cursor_setReverse(cursor_t *cursor, cursor_axis_t axis,
		cursor_reversed_t reversed);
int32_t cursor_output(cursor_t *cursor, cursor_axis_t axis);

#endif /* LIB_CURSOR_H_ */
