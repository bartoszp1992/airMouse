/*
 * sensor.c
 *
 *  Created on: Mar 10, 2025
 *      Author: bartosz
 */

#include "sensor.h"

lsm6ds_sensor_t mems;
lsm6ds_state_t sensorStat;

cursor_t cursor;

void sensor_init(void){

		sensorStat = lsm6ds_init(&mems, LSM6DS_ADDR_SA0_L, &hi2c1, 100, 100);
		sensorStat = lsm6ds_reset(&mems);

		//	sensorStat = lsm6ds_setXLOutputDataRate(&mems, LSM6DS_ODR_XL_12_5_HZ);
		//	sensorStat = lsm6ds_setXLFullScale(&mems, LSM6DS_FS_XL_16G);
		sensorStat = lsm6ds_setXLOutputDataRate(&mems, LSM6DS_ODR_XL_PWR_DN);

		sensorStat = lsm6ds_setGRLowPass(&mems, LSM6DS_FTYPE_VHIGH);
		sensorStat = lsm6ds_setGROutputDataRate(&mems, CONFIG_IMU_ODR);
		sensorStat = lsm6ds_setGRFullScale(&mems, LSM6DS_FS_G_500DPS);
		sensorStat = lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);

		cursor_init(&cursor);
		cursor_setReverse(&cursor, CURSOR_AXIS_X, CURSOR_REVERSED);
		cursor_setReverse(&cursor, CURSOR_AXIS_Y, CURSOR_NOT_REVERSED);
		cursor_setReverse(&cursor, CURSOR_AXIS_Z, CURSOR_NOT_REVERSED);

		cursor_setSensitivity(&cursor, 10);
		cursor_setAcceleration(&cursor, 140, CURSOR_ACCELERATION_MULTIPLY);
		cursor_setMax(&cursor, 40);
}
