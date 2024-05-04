/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
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
volatile rtc_set_clock_flag_t rtc_set_flag = RTC_SET;
volatile flag_t process_flag = FLAG_STANDBY;
uint8_t uart_rx_buf[RX_SIZE], uart_tx_buf[TX_SIZE];
RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef date_time;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void display_rtc_to_serial(RTC_TimeTypeDef time, rtc_num_t type);
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (process_flag == FLAG_SHOW_CLOCK){
		  memset(uart_tx_buf, 0x0U, TX_SIZE);

		  HAL_RTC_WaitForSynchro(&hrtc);

		  if(HAL_RTC_GetDate(&hrtc, &date_time, RTC_FORMAT_BIN) != HAL_OK){
			  Error_Handler();
		  }
		  if(HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN) != HAL_OK){
			  Error_Handler();
		  }

		  HAL_RTC_WaitForSynchro(&hrtc);

		  display_rtc_to_serial(rtc_time, RTC_TYPE_DECIMAL);

		  clear_process_flag();
	  }

	  if (process_flag == FLAG_SET_CLOCK){
		  memset(uart_tx_buf, 0x0U, TX_SIZE);

		  rtc_set_flag += 1;

		  if (rtc_set_flag == RTC_SET_FLAG_MAX){
			  rtc_set_flag = RTC_SET;
		  }

		  if (rtc_set_flag == RTC_SET_HOUR){
			  sprintf((char *) uart_tx_buf, "==== SET HOUR ====\r\n");
		  }
		  else if (rtc_set_flag == RTC_SET_MIN){
			  sprintf((char *) uart_tx_buf, "==== SET MINUTES ====\r\n");
		  }
		  else if (rtc_set_flag == RTC_SET){
			  sprintf((char *) uart_tx_buf, "==== SETTING TIME ====\r\n");

			  HAL_RTC_WaitForSynchro(&hrtc);

			  if(HAL_RTC_SetDate(&hrtc, &date_time, RTC_FORMAT_BIN) != HAL_OK){
				  Error_Handler();
			  }
			  if(HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN) != HAL_OK){
				  Error_Handler();
			  }

			  HAL_RTC_WaitForSynchro(&hrtc);
		  }

		  HAL_UART_Transmit(&huart1, uart_tx_buf, strlen((char *) uart_tx_buf), HAL_MAX_DELAY);
		  clear_process_flag();

	  }

	  if (process_flag == FLAG_INC_TIME){
		  memset(uart_tx_buf, 0x0U, TX_SIZE);

		  if (rtc_set_flag == RTC_SET_HOUR){
			  rtc_time.Hours += 1;
			  if (rtc_time.Hours >= 24) rtc_time.Hours = 0;
			  HAL_Delay(10);
			  sprintf((char *)uart_tx_buf, "Hour: %d\r\n", rtc_time.Hours);
		  }

		  if (rtc_set_flag == RTC_SET_MIN){
			  rtc_time.Minutes += 1;
			  if (rtc_time.Minutes >= 60) rtc_time.Minutes = 0;
			  HAL_Delay(10);
			  sprintf((char *)uart_tx_buf, "Minutes: %d\r\n", rtc_time.Minutes);
		  }


		  HAL_UART_Transmit(&huart1, uart_tx_buf, strlen((char *) uart_tx_buf), HAL_MAX_DELAY);
		  clear_process_flag();

	  }

	  if (process_flag == FLAG_DEC_TIME){
		  if (rtc_set_flag == RTC_SET_HOUR){
			  rtc_time.Hours -= 1;
			  if (rtc_time.Hours >= 24) rtc_time.Hours = 23;
			  HAL_Delay(10);
			  sprintf((char *)uart_tx_buf, "Hour: %d\r\n", rtc_time.Hours);
		  }

		  if (rtc_set_flag == RTC_SET_MIN){
			  rtc_time.Minutes -= 1;
			  if (rtc_time.Minutes >= 60) rtc_time.Minutes = 59;
			  HAL_Delay(10);
			  sprintf((char *)uart_tx_buf, "Minutes: %d\r\n", rtc_time.Minutes);
		  }

		  HAL_UART_Transmit(&huart1, uart_tx_buf, strlen((char *) uart_tx_buf), HAL_MAX_DELAY);
		  clear_process_flag();

	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void display_rtc_to_serial(RTC_TimeTypeDef time, rtc_num_t type){
	if (type == RTC_TYPE_BCD){
		sprintf((char *)uart_tx_buf,
			  "Time: %d:%d:%d\n",
			  RTC_Bcd2ToByte(time.Hours),
			  RTC_Bcd2ToByte(time.Minutes),
			  RTC_Bcd2ToByte(time.Seconds)
	    );
	}
	else if (type == RTC_TYPE_DECIMAL){
		sprintf((char *)uart_tx_buf,
			  "Time: %d:%d:%d\n",
			  time.Hours,
			  time.Minutes,
			  time.Seconds
		);
	}

	HAL_UART_Transmit(&huart1, uart_tx_buf, strlen((char *)uart_tx_buf), HAL_MAX_DELAY);
}

//void increment_time(){
//
//}
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
  while (1)
  {
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
