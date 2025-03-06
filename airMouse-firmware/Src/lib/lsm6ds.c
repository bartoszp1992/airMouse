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
lsm6ds_state_t i2cSend(lsm6ds_serialInstance_t *i2c, uint16_t devAddr,
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
lsm6ds_state_t i2cReceive(lsm6ds_serialInstance_t *i2c, uint16_t devAddr,
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

	memset(&sensor->outputGR, 0, sizeof(sensor->outputGR));
	memset(&sensor->outputXL, 0, sizeof(sensor->outputXL));
	sensor->outTemperature = 0;

#if INT_SUPPORT == 1
	sensor->dataReady = LSM6DS_DATA_NREADY;
#endif

	//check communication
	lsm6ds_state_t state = LSM6DS_STATE_ERR;
	uint8_t receivedWhoIAm = 0;
	state = i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr,
	LSM6DS_REG_WHO_AM_I, &receivedWhoIAm, 1);
	if (state == LSM6DS_STATE_ERR)
		return LSM6DS_STATE_ERR_SERIAL;
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
 * set DRDY of gyro/accelerometer on INT1 pin
 * @param: sensor
 * @param: LSM6DS_INT1_DRDY_G/XL/DIS
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setInt1Drdy(lsm6ds_sensor_t *sensor, uint8_t interrupt){

	return modifyReg(sensor, LSM6DS_REG_INT1_CTRL, LSM6DS_MASK_INT1_DRDY_G_XL, interrupt);

}

/*
 * set accelerometer output data rate
 * @param: sensor
 * @param: LSM6DS_ODR_XL_*
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setXLOutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr) {

	return modifyReg(sensor, LSM6DS_REG_CTRL1_XL, LSM6DS_MASK_ODR_XL, odr);

}

/*
 * set accelerometer full scale
 * @param: sensor
 * @param: LSM6DS_FS_XL_*
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setXLFullScale(lsm6ds_sensor_t *sensor, uint8_t fs) {

	return modifyReg(sensor, LSM6DS_REG_CTRL1_XL, LSM6DS_MASK_FS_XL, fs);

}

/*
 * set gyroscope output data rate
 * @param: sensor
 * @param: LSM6DS_ODR_G_*
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setGROutputDataRate(lsm6ds_sensor_t *sensor, uint8_t odr) {
	return modifyReg(sensor, LSM6DS_REG_CTRL2_G, LSM6DS_MASK_ODR_G, odr);

}

/*
 * set low pass filter for gyroscope
 * @param: sensor
 * @param: LSM6DS_FTYPE_
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setGRLowPass(lsm6ds_sensor_t *sensor, uint8_t filter){

	lsm6ds_state_t status = LSM6DS_STATE_ERR;

	status = modifyReg(sensor, LSM6DS_REG_CTRL4_C, LSM6DS_MASK_LPF1_SEL_G, LSM6DS_LPF_SEL_G_EN);
	if(status != LSM6DS_STATE_OK)
		return status;

	status =  modifyReg(sensor, LSM6DS_REG_CTRL6_C, LSM6DS_MASK_FTYPE, filter);
	return status;

}

/*
 * set gyroscope full scale
 * @param: sensor
 * @param: LSM6DS_FS_G_*
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_setGRFullScale(lsm6ds_sensor_t *sensor, uint8_t fs) {

	lsm6ds_state_t stat = LSM6DS_STATE_ERR;

	if (fs == LSM6DS_FS_G_125DPS) {
		stat = modifyReg(sensor, LSM6DS_REG_CTRL2_G, LSM6DS_MASK_FS_G,
		LSM6DS_FS_G_245DPS);
		if (stat != LSM6DS_STATE_OK)
			return stat;
		stat = modifyReg(sensor, LSM6DS_REG_CTRL2_G, LSM6DS_MASK_FS_125,
		LSM6DS_FS_G_125DPS);
		if (stat != LSM6DS_STATE_OK)
			return stat;

	} else {

		stat = modifyReg(sensor, LSM6DS_REG_CTRL2_G, LSM6DS_MASK_FS_125, 0);
		if (stat != LSM6DS_STATE_OK)
			return stat;
		stat = modifyReg(sensor, LSM6DS_REG_CTRL2_G, LSM6DS_MASK_FS_G, fs);
		if (stat != LSM6DS_STATE_OK)
			return stat;

	}

	return stat;

}

/*
 * software reset
 * @param: sensor
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_reset(lsm6ds_sensor_t *sensor) {
	return modifyReg(sensor, LSM6DS_REG_CTRL3_C, LSM6DS_MASK_SW_RESET,
	LSM6DS_SW_RESET);
}

/*
 * set gyroscope sleep
 * @param: sensor
 * @param: LSM6DS_WKUP/SLEEP
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_sleepGR(lsm6ds_sensor_t *sensor, uint8_t sleep) {
	return modifyReg(sensor, LSM6DS_REG_CTRL4_C, LSM6DS_MASK_SLEEP, sleep);
}

/*
 * check new data available
 * @param: sensor
 * @output: check output with LSM6DS_TDA/GDA/XLDA
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_newDataAvailableCheck(lsm6ds_sensor_t *sensor,
		uint8_t *newData) {

	return i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr,
			LSM6DS_REG_STATUS_REG, newData, 1);

}


/*
 * updates temperature in sensor struct
 * @param: sensor
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_updateTemp(lsm6ds_sensor_t *sensor){

	lsm6ds_state_t status = LSM6DS_STATE_ERR;

	//array to value converter
	union {
		uint8_t array[2];
		int16_t value;
	}outputData;

	status = i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr, LSM6DS_REG_OUT_TEMP_L, outputData.array, 2);

	if(status != LSM6DS_STATE_OK)
		return status;

	sensor->outTemperature = 2500+(((int32_t)outputData.value*100)/256);

	return status;

}

/*
 * updates accelerometer data in sensor struct
 * @param: sensor
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_updateXL(lsm6ds_sensor_t *sensor){

	lsm6ds_state_t status = LSM6DS_STATE_ERR;

	//array to value converter
	union{
		uint8_t raw[6];
		int16_t combined[3];
	}outputData;

	memset(&outputData, 0, sizeof(outputData));

	status = i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr, LSM6DS_REG_OUTX_L_XL, outputData.raw, 6);

	if(status != LSM6DS_STATE_OK)
		return status;

	sensor->outputXL[LSM6DS_AXIS_X] = outputData.combined[0];
	sensor->outputXL[LSM6DS_AXIS_Y] = outputData.combined[1];
	sensor->outputXL[LSM6DS_AXIS_Z] = outputData.combined[2];

	return status;

}

/*
 * updates gyro data in sensor struct
 * @param: sensor
 *
 * @retval: status
 */
lsm6ds_state_t lsm6ds_updateGR(lsm6ds_sensor_t *sensor){

	lsm6ds_state_t status = LSM6DS_STATE_ERR;

	//array to value converter
	union{
		uint8_t raw[6];
		int16_t combined[3];
	}outputData;

	memset(&outputData, 0, sizeof(outputData));

	status = i2cReceive(&sensor->lsm6dsI2c, sensor->deviceAddr, LSM6DS_REG_OUTX_L_G, outputData.raw, 6);

	if(status != LSM6DS_STATE_OK)
		return status;

	sensor->outputGR[LSM6DS_AXIS_X] = outputData.combined[0];
	sensor->outputGR[LSM6DS_AXIS_Y] = outputData.combined[1];
	sensor->outputGR[LSM6DS_AXIS_Z] = outputData.combined[2];

	return status;
}

/*
 * read temperature data. remember to run lsm6ds_updateTemp() first!
 * @param: sensor
 *
 * @retval: temperature data in centi Celsius degress - e.g. 2405 for 24,05 degress.
 */
int16_t lsm6ds_readTemperature(lsm6ds_sensor_t *sensor){
	return sensor->outTemperature;
}

/*
 * read accelerometer data. remember to run lsm6ds_updateXL() first!
 * @param: sensor
 * @param: axis- LSM6DS_AXIS_X/Y/Z
 *
 * @retval: accelerometer data
 */
int16_t lsm6ds_readXL(lsm6ds_sensor_t *sensor, lsm6ds_axis_t axis){
	return sensor->outputXL[axis];
}

/*
 * read gyroscope data. remember to run lsm6ds_updateGR() first!
 * @param: sensor
 * @param: axis- LSM6DS_AXIS_X/Y/Z
 *
 * @retval: gyroscope data
 */
int16_t lsm6ds_readGR(lsm6ds_sensor_t *sensor, lsm6ds_axis_t axis){
	return sensor->outputGR[axis];
}

#if INT_SUPPORT == 1

/*
 * run this function when EXTI occurs
 * @param: sensor
 *
 * @retval: none
 */
void lsm6ds_flagDataReadySet(lsm6ds_sensor_t *sensor){
	sensor->dataReady = LSM6DS_DATA_READY;
}

/*
 * check if dataready flag is set by EXTI
 * @param: sensor
 *
 * @retval: LSM6DS_DATA_READY/NREADY
 */
lsm6ds_drdy_t lsm6ds_flagDataReadyRead(lsm6ds_sensor_t *sensor){
	if(sensor->dataReady == LSM6DS_DATA_READY){
		sensor->dataReady = LSM6DS_DATA_NREADY;
		return LSM6DS_DATA_READY;
	}else
		return LSM6DS_DATA_NREADY;
}


#endif
