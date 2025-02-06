/*
 * lsm6ds.c
 *
 *  Created on: Feb 5, 2025
 *      Author: bartosz
 */

#include "lsm6ds.h"

//---------------------------IMPLICIT FUNCTIONS---------------------------
/*
 * uart send/receive functions. fill it by hardware-specific uart send/receive functions
 * port
 */
lsm6ds_state_t i2cSend(lsm6ds_i2cInstance_t *i2c, uint16_t devAddr,
		uint16_t memAddr, uint8_t *data, uint32_t size) {

	HAL_StatusTypeDef state = HAL_I2C_Mem_Write(i2c->i2c, devAddr, memAddr,
	LSM6DS_REGISTER_ADDR_LENGTH, data, size, i2c->sendTimeout);

	if (state == HAL_OK)
		return LSM6DS_STATE_OK;
	else if (state == HAL_TIMEOUT)
		return LSM6DS_STATE_TIMEOUT;
	else
		return LSM6DS_STATE_ERR;
}

//port
lsm6ds_state_t i2cReceive(lsm6ds_i2cInstance_t *i2c, uint16_t devAddr,
		uint16_t memAddr, uint8_t *data, uint32_t size) {

	HAL_StatusTypeDef state = HAL_I2C_Mem_Read(i2c->i2c, devAddr, memAddr,
	LSM6DS_REGISTER_ADDR_LENGTH, data, size, i2c->receiveTimeout);

	if (state == HAL_OK)
		return LSM6DS_STATE_OK;
	else if (state == HAL_TIMEOUT)
		return LSM6DS_STATE_TIMEOUT;
	else
		return LSM6DS_STATE_ERR;
}

/*
 * read and modify reg by specified bits
 * @param: sensor
 * @param: register address
 * @param: bitmask
 * @param: value
 */
lsm6ds_state_t modifyReg(lsm6ds_sensor_t *sensor, uint16_t regAddr,
		uint8_t bitmask, uint16_t value) {

	uint8_t readedReg = 0;
	lsm6ds_state_t state = LSM6DS_STATE_ERR;

	//read original register
	state = i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr, regAddr,
			&readedReg, 1);

	if (state != LSM6DS_STATE_OK)
		return state;

	//clear bits corresponding to mask
	readedReg &= ~(bitmask);

	//modify register
	uint8_t regToWrite = readedReg | (bitmask & value);

	//write register
	return i2cSend(&sensor->lsm6dsI2c, sensor->deviceAddr, regAddr, &regToWrite,
			1);

}

//---------------------------PUBLIC FUNCTIONS---------------------------

/*
 * init sensor
 * @param: sensor struct
 * @param: i2c address(8 bit) LSM6DS_ADDR_SA0_H/L
 * @param: hardware specific i2c struct
 * @param: txTimeout
 * @param: rxTimeout
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_init(lsm6ds_sensor_t *sensor, uint16_t devAddr,
		I2C_HandleTypeDef *i2c, uint32_t txTimeout, uint32_t rxTimeout) { //port

	sensor->deviceAddr = devAddr;
	sensor->lsm6dsI2c.i2c = i2c;
	sensor->lsm6dsI2c.receiveTimeout = rxTimeout;
	sensor->lsm6dsI2c.sendTimeout = txTimeout;

	//check communication
	lsm6ds_state_t state = LSM6DS_STATE_ERR;
	uint8_t receivedWhoIAm = 0;
	state = i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr,
	LSM6DS_REG_WHO_AM_I, &receivedWhoIAm, 1);
	if (state == LSM6DS_STATE_ERR)
		return LSM6DS_STATE_ERR_I2C;
	else if (state == LSM6DS_STATE_TIMEOUT)
		return state;
	else {
		if (receivedWhoIAm == LSM6DS_WHO_AM_I)
			return LSM6DS_STATE_OK;
		else
			return LSM6DS_STATE_WRONG_WHOAMI;
	}

}

/*
 * set accelerometer output data rate
 * @param: sensor
 * @param: LSM6DS_ODR_XL_*
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setAccOutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr) {

	return modifyReg(sensor, LSM6DS_REG_CTRL1_XL, LSM6DS_MASK_ODX_XL, odr);

}

/*
 * set accelerometer full scale
 * @param: sensor
 * @param: LSM6DS_FS_XL_*
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setAccFullScale(lsm6ds_sensor_t *sensor, uint8_t fs) {

	return modifyReg(sensor, LSM6DS_REG_CTRL1_XL, LSM6DS_MASK_FS_XL, fs);

}
