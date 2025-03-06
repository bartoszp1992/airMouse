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

#define INT_SUPPORT 1 //flag for DRDY in sensor struct


//dev addreses
#define LSM6DS_ADDR_SA0_H 0x6b<<1
#define LSM6DS_ADDR_SA0_L 0x6a<<1

#define LSM6DS_WHO_AM_I 0x6a
#define LSM6DS_REGISTER_ADDR_LENGTH 1

typedef enum {
	LSM6DS_STATE_OK = 0,
	LSM6DS_STATE_ERR = 1,
	LSM6DS_STATE_TIMEOUT = 2,
	LSM6DS_STATE_ERR_SERIAL = 3,
	LSM6DS_STATE_WRONG_WHOAMI = 4,
	LSM6DS_STATE_PIN_NOT_DEFINED = 5
} lsm6ds_state_t;

#if INT_SUPPORT == 1
typedef enum {
	LSM6DS_DATA_NREADY = 0,
	LSM6DS_DATA_READY = 1
}lsm6ds_drdy_t;
#endif

//hardware specific i2c structure
typedef struct {
	I2C_HandleTypeDef *i2c; //port
	uint32_t sendTimeout;
	uint32_t receiveTimeout;
} lsm6ds_serialInstance_t;

//sensor output struct

typedef enum {
	LSM6DS_AXIS_X = 0, LSM6DS_AXIS_Y = 1, LSM6DS_AXIS_Z = 2
} lsm6ds_axis_t;

//sensor struct
typedef struct {
	lsm6ds_serialInstance_t lsm6dsI2c;
	uint16_t deviceAddr;
//	lsm6ds_output_t outXL;
//	lsm6ds_output_t outGR;
	int16_t outputXL[3];
	int16_t outputGR[3];
	int16_t outTemperature; //temperature in centi Celsius degress - e.g. 2405 for 24,05 degress.

#if (INT_SUPPORT == 1)
	lsm6ds_drdy_t dataReady; //for use with interrupt. set to 1 when interrupt occurs
#endif

} lsm6ds_sensor_t;



lsm6ds_state_t lsm6ds_init(lsm6ds_sensor_t *sensor, uint16_t devAddr,
		I2C_HandleTypeDef *i2c, uint32_t txTimeout, uint32_t rxTimeout);
lsm6ds_state_t lsm6ds_setInt1Drdy(lsm6ds_sensor_t *sensor, uint8_t interrupt);
lsm6ds_state_t lsm6ds_setXLOutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr);
lsm6ds_state_t lsm6ds_setXLFullScale(lsm6ds_sensor_t *sensor, uint8_t fs);
lsm6ds_state_t lsm6ds_setGROutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr);
lsm6ds_state_t lsm6ds_setGRLowPass(lsm6ds_sensor_t *sensor, uint8_t filter);
lsm6ds_state_t lsm6ds_setGRFullScale(lsm6ds_sensor_t *sensor, uint8_t fs);
lsm6ds_state_t lsm6ds_reset(lsm6ds_sensor_t *sensor);
lsm6ds_state_t lsm6ds_sleepGR(lsm6ds_sensor_t *sensor, uint8_t sleep);
lsm6ds_state_t lsm6ds_newDataAvailableCheck(lsm6ds_sensor_t *sensor,
		uint8_t *newData);
lsm6ds_state_t lsm6ds_updateTemp(lsm6ds_sensor_t *sensor);
lsm6ds_state_t lsm6ds_updateXL(lsm6ds_sensor_t *sensor);
lsm6ds_state_t lsm6ds_updateGR(lsm6ds_sensor_t *sensor);

int16_t lsm6ds_readTemperature(lsm6ds_sensor_t *sensor);
int16_t lsm6ds_readXL(lsm6ds_sensor_t *sensor, lsm6ds_axis_t axis);
int16_t lsm6ds_readGR(lsm6ds_sensor_t *sensor, lsm6ds_axis_t axis);


#if INT_SUPPORT == 1
void lsm6ds_flagDataReadySet(lsm6ds_sensor_t *sensor);
lsm6ds_drdy_t lsm6ds_flagDataReadyRead(lsm6ds_sensor_t *sensor);
#endif

#endif /* LSM6DS_H_ */
