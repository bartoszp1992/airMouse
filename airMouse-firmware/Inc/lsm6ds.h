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

//sensor struct
typedef struct {
	lsm6ds_i2cInstance_t lsm6dsI2c;
	uint16_t deviceAddr;
} lsm6ds_sensor_t;

lsm6ds_state_t lsm6ds_init(lsm6ds_sensor_t *sensor, uint16_t devAddr,
		I2C_HandleTypeDef *i2c, uint32_t txTimeout, uint32_t rxTimeout);

#endif /* LSM6DS_H_ */
