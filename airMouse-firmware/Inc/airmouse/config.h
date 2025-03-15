
/*
 * config.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "lsm6ds.h"

#define CONFIG_BAUDRATE_FAST 115200*12
#define CONFIG_BAUDRATE_DEFAULT 115200
#define CONFIG_TIMEOUT_RX_LONG 1500
#define CONFIG_TIMEOUT_RX_FAST 10

#define CONFIG_ONTIME 5 //in seconds

#define CONFIG_DPS LSM6DS_FS_G_500DPS

#define CONFIG_DEVICE_NAME "NEURO5"

#define CONFIG_IMU_ODR LSM6DS_ODR_G_104_HZ

#endif /* CONFIG_H_ */
