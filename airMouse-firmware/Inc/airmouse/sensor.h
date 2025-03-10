/*
 * sensor.h
 *
 *  Created on: Mar 10, 2025
 *      Author: bartosz
 */

#include "lsm6ds.h"
#include "cursor.h"
#include "config.h"

extern lsm6ds_sensor_t mems;
extern lsm6ds_state_t sensorStat;
extern cursor_t cursor;

void sensor_init(void);
