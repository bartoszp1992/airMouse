/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "espat.h"
#include "lsm6ds.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
espat_radio_t bleRadio;
lsm6ds_sensor_t mems;
char rxBuffer[25];

volatile int32_t amx = 0;
volatile int32_t amy = 0;
volatile int32_t amz = 0;
volatile uint8_t flagDrdyG = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ledOn(void);
void ledOff(void);
void esp32download(void);
void esp32rst(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  while(1);


  //enter download mode or reset ESP
  HAL_Delay(200);
  if(HAL_GPIO_ReadPin(MUS_BCK_GPIO_Port, MUS_BCK_Pin) == GPIO_PIN_RESET)
	  esp32download();
  else
	  esp32rst();
  HAL_Delay(200);



	lsm6ds_state_t sensorStat = lsm6ds_init(&mems, LSM6DS_ADDR_SA0_L, &hi2c1,
			100, 100);

	espat_state_t espStat = espAt_init(&bleRadio, &huart1, 50, 2000);

	espStat = espAt_sendCommand(&bleRadio, G_RST);
	HAL_Delay(1000);

	espStat = espAt_sendString(&bleRadio, S_BHN, "bartsHID");
	ledOn();
	espStat = espAt_receive(&bleRadio, rxBuffer, sizeof(rxBuffer));
	ledOff();
	HAL_Delay(800);
	espStat = espAt_sendParams(&bleRadio, P_BHI, 1, 1);
	ledOn();
	espStat = espAt_receive(&bleRadio, rxBuffer, sizeof(rxBuffer));
	ledOff();
	HAL_Delay(200);

	//change baudrate
	espAt_sendParams(&bleRadio, P_UC, 5, 3000000, 8, 1, 0, 0);
	HAL_Delay(200);
	HAL_UART_ChangeSpeed(&huart1, 3000000);

	sensorStat = lsm6ds_reset(&mems);

//	sensorStat = lsm6ds_setXLOutputDataRate(&mems, LSM6DS_ODR_XL_12_5_HZ);
	//	sensorStat = lsm6ds_setXLFullScale(&mems, LSM6DS_FS_XL_16G);

	sensorStat = lsm6ds_setGRLowPass(&mems, LSM6DS_FTYPE_VHIGH);
	sensorStat = lsm6ds_setGROutputDataRate(&mems, LSM6DS_ODR_G_208_HZ);
	sensorStat = lsm6ds_setGRFullScale(&mems, LSM6DS_FS_G_2000DPS);
	sensorStat = lsm6ds_setInt1Drdy(&mems, LSM6DS_INT1_DRDY_G);

	int32_t sensitivity = 100; //10000 are 1
	int32_t acceleration = 140; //100- no acceleration

	uint8_t newDataAvailable = 0;
	uint8_t blueButtonState = 0;

	int32_t maxVal = 100;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {


		if (flagDrdyG) {
			flagDrdyG = 0;

			sensorStat = lsm6ds_updateGR(&mems);

			//sensitivity
			amx = ((int32_t) mems.outGR.x * sensitivity) / 10000;
			amz = ((int32_t) mems.outGR.z * sensitivity) / 10000;

			//acceleration

			uint8_t isNegative;

			if (amx < 0)
				isNegative = 1;
			else
				isNegative = 0;

			amx = pow((double) abs(amx), ((double) acceleration) / 100);
			if (isNegative)
				amx = -amx;

			if (amz < 0)
				isNegative = 1;
			else
				isNegative = 0;
			amz = pow((double) abs(amz), ((double) acceleration) / 100);
			if (isNegative)
				amz = -amz;

			//to high value secure
			if (amx > maxVal)
				amx = maxVal;
			if (amx < -maxVal)
				amx = -maxVal;

			if (amz > maxVal)
				amz = maxVal;
			if (amz < -maxVal)
				amz = -maxVal;

			//revert sign
			amz *= -1;
			amx *= -1;

			if (HAL_GPIO_ReadPin(MUS_DPI_GPIO_Port, MUS_DPI_Pin)
					== GPIO_PIN_RESET)
				blueButtonState = 1;
			else
				blueButtonState = 0;

			espAt_sendParams(&bleRadio, P_BHM, 4, blueButtonState, amz, amx, 0);


		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	}
	// no warning workarounds
	sensorStat++;
	amy++;
	newDataAvailable++;
	espStat++;
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GYRO_INT_Pin) {
		flagDrdyG = 1;
	}
}

void esp32download(void){
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP_BOOT_GPIO_Port, ESP_BOOT_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, 1);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP_BOOT_GPIO_Port, ESP_BOOT_Pin, 1);
	HAL_Delay(100);
}

void esp32rst(void){

	HAL_GPIO_WritePin(ESP_BOOT_GPIO_Port, ESP_BOOT_Pin, 1);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, 1);
	HAL_Delay(100);

}

void ledOn(void) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 1);
}

void ledOff(void) {
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 0);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
