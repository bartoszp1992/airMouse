/*
 * lsm6ds.h
 *
 *  Created on: Feb 5, 2025
 *      Author: bartosz
 */

#ifndef LSM6DS_H_
#define LSM6DS_H_

//port- hardware specific
#include "i2c.h"

//others
#include <stdint.h>
#include <string.h>
#include "lsm6ds_reg.h"

//dev addreses
#define LSM6DS_ADDR_SA0_H 0x6b<<1
#define LSM6DS_ADDR_SA0_L 0x6a<<1

#define LSM6DS_WHO_AM_I 0x6a
#define LSM6DS_REGISTER_ADDR_LENGTH 1

typedef enum {
	LSM6DS_STATE_OK = 0,
	LSM6DS_STATE_ERR = 1,
	LSM6DS_STATE_TIMEOUT = 2,
	LSM6DS_STATE_ERR_I2C = 3,
	LSM6DS_STATE_WRONG_WHOAMI = 4
} lsm6ds_state_t;

//hardware specific i2c structure
typedef struct {
	I2C_HandleTypeDef *i2c; //port
	uint32_t sendTimeout;
	uint32_t receiveTimeout;
} lsm6ds_i2cInstance_t;

//sensor output struct
typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
} lsm6ds_output_t;

//sensor struct
typedef struct {
	lsm6ds_i2cInstance_t lsm6dsI2c;
	uint16_t deviceAddr;
	lsm6ds_output_t outXL;
	lsm6ds_output_t outGR;
	int16_t outTemperature; //temperature in centi Celsius degress - e.g. 2405 for 24,05 degress.
} lsm6ds_sensor_t;



lsm6ds_state_t lsm6ds_init(lsm6ds_sensor_t *sensor, uint16_t devAddr,
		I2C_HandleTypeDef *i2c, uint32_t txTimeout, uint32_t rxTimeout);
lsm6ds_state_t lsm6ds_setXLOutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr);
lsm6ds_state_t lsm6ds_setXLFullScale(lsm6ds_sensor_t *sensor, uint8_t fs);
lsm6ds_state_t lsm6ds_setGROutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr);
lsm6ds_state_t lsm6ds_setGRFullScale(lsm6ds_sensor_t *sensor, uint8_t fs);
lsm6ds_state_t lsm6ds_reset(lsm6ds_sensor_t *sensor);
lsm6ds_state_t lsm6ds_sleepGR(lsm6ds_sensor_t *sensor, uint8_t sleep);
lsm6ds_state_t lsm6ds_newDataAvailableCheck(lsm6ds_sensor_t *sensor,
		uint8_t *newData);
lsm6ds_state_t lsm6ds_updateTemp(lsm6ds_sensor_t *sensor);
lsm6ds_state_t lsm6ds_updateXL(lsm6ds_sensor_t *sensor);
lsm6ds_state_t lsm6ds_updateGR(lsm6ds_sensor_t *sensor);

#endif /* LSM6DS_H_ */
