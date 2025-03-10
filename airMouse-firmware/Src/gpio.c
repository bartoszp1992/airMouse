/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_BLUE_Pin|LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ESP_BOOT_GPIO_Port, ESP_BOOT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, KBD_ROW1_Pin|KBD_ROW2_Pin|KBD_ROW3_Pin|KBD_ROW4_Pin
                          |KBD_ROW5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : MUS_PRC_Pin MUS_HOME_Pin MUS_DPI_Pin MUS_FWD_Pin
                           MUS_BCK_Pin MUS_MB_Pin MUS_RB_Pin MUS_UP_Pin
                           MUS_DN_Pin */
  GPIO_InitStruct.Pin = MUS_PRC_Pin|MUS_HOME_Pin|MUS_DPI_Pin|MUS_FWD_Pin
                          |MUS_BCK_Pin|MUS_MB_Pin|MUS_RB_Pin|MUS_UP_Pin
                          |MUS_DN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : GYRO_INT_Pin */
  GPIO_InitStruct.Pin = GYRO_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GYRO_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_BLUE_Pin LED_RED_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_BLUE_Pin|LED_RED_Pin|LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : KBD_COL1_Pin KBD_COL2_Pin KBD_COL10_Pin KBD_COL3_Pin
                           KBD_COL4_Pin KBD_COL5_Pin KBD_COL6_Pin KBD_COL7_Pin
                           KBD_COL8_Pin KBD_COL9_Pin */
  GPIO_InitStruct.Pin = KBD_COL1_Pin|KBD_COL2_Pin|KBD_COL10_Pin|KBD_COL3_Pin
                          |KBD_COL4_Pin|KBD_COL5_Pin|KBD_COL6_Pin|KBD_COL7_Pin
                          |KBD_COL8_Pin|KBD_COL9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_BOOT_Pin */
  GPIO_InitStruct.Pin = ESP_BOOT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_BOOT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_EN_Pin */
  GPIO_InitStruct.Pin = ESP_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MUS_LB_Pin */
  GPIO_InitStruct.Pin = MUS_LB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MUS_LB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KBD_ROW1_Pin KBD_ROW2_Pin KBD_ROW3_Pin KBD_ROW4_Pin
                           KBD_ROW5_Pin */
  GPIO_InitStruct.Pin = KBD_ROW1_Pin|KBD_ROW2_Pin|KBD_ROW3_Pin|KBD_ROW4_Pin
                          |KBD_ROW5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
