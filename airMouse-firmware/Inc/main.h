/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MUS_PRC_Pin GPIO_PIN_11
#define MUS_PRC_GPIO_Port GPIOC
#define MUS_HOME_Pin GPIO_PIN_12
#define MUS_HOME_GPIO_Port GPIOC
#define MUS_DPI_Pin GPIO_PIN_13
#define MUS_DPI_GPIO_Port GPIOC
#define MUS_FWD_Pin GPIO_PIN_14
#define MUS_FWD_GPIO_Port GPIOC
#define MUS_BCK_Pin GPIO_PIN_15
#define MUS_BCK_GPIO_Port GPIOC
#define GYRO_INT_Pin GPIO_PIN_0
#define GYRO_INT_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_1
#define LED_BLUE_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_4
#define LED_RED_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOA
#define ESP_TX_Pin GPIO_PIN_4
#define ESP_TX_GPIO_Port GPIOC
#define ESP_RX_Pin GPIO_PIN_5
#define ESP_RX_GPIO_Port GPIOC
#define BAT__Pin GPIO_PIN_0
#define BAT__GPIO_Port GPIOB
#define KBD_COL1_Pin GPIO_PIN_1
#define KBD_COL1_GPIO_Port GPIOB
#define KBD_COL2_Pin GPIO_PIN_2
#define KBD_COL2_GPIO_Port GPIOB
#define KBD_COL10_Pin GPIO_PIN_10
#define KBD_COL10_GPIO_Port GPIOB
#define ESP_BOOT_Pin GPIO_PIN_14
#define ESP_BOOT_GPIO_Port GPIOB
#define ESP_EN_Pin GPIO_PIN_15
#define ESP_EN_GPIO_Port GPIOB
#define MUS_LB_Pin GPIO_PIN_6
#define MUS_LB_GPIO_Port GPIOC
#define MUS_MB_Pin GPIO_PIN_7
#define MUS_MB_GPIO_Port GPIOC
#define MUS_RB_Pin GPIO_PIN_8
#define MUS_RB_GPIO_Port GPIOC
#define MUS_UP_Pin GPIO_PIN_9
#define MUS_UP_GPIO_Port GPIOC
#define KBD_ROW1_Pin GPIO_PIN_1
#define KBD_ROW1_GPIO_Port GPIOD
#define KBD_ROW2_Pin GPIO_PIN_2
#define KBD_ROW2_GPIO_Port GPIOD
#define KBD_ROW3_Pin GPIO_PIN_3
#define KBD_ROW3_GPIO_Port GPIOD
#define KBD_ROW4_Pin GPIO_PIN_4
#define KBD_ROW4_GPIO_Port GPIOD
#define KBD_ROW5_Pin GPIO_PIN_5
#define KBD_ROW5_GPIO_Port GPIOD
#define KBD_COL3_Pin GPIO_PIN_3
#define KBD_COL3_GPIO_Port GPIOB
#define KBD_COL4_Pin GPIO_PIN_4
#define KBD_COL4_GPIO_Port GPIOB
#define KBD_COL5_Pin GPIO_PIN_5
#define KBD_COL5_GPIO_Port GPIOB
#define KBD_COL6_Pin GPIO_PIN_6
#define KBD_COL6_GPIO_Port GPIOB
#define KBD_COL7_Pin GPIO_PIN_7
#define KBD_COL7_GPIO_Port GPIOB
#define KBD_COL8_Pin GPIO_PIN_8
#define KBD_COL8_GPIO_Port GPIOB
#define KBD_COL9_Pin GPIO_PIN_9
#define KBD_COL9_GPIO_Port GPIOB
#define MUS_DN_Pin GPIO_PIN_10
#define MUS_DN_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
