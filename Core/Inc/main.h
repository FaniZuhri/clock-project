/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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
#define LEFT_SHIFT(N) (1 << N)

#define RX_SIZE 256
#define TX_SIZE 256
typedef enum flag_e {
	FLAG_STANDBY,
	FLAG_SHOW_CLOCK,
	FLAG_SET_CLOCK,
	FLAG_INC_TIME,
	FLAG_DEC_TIME,
} flag_t;
typedef enum rtc_num_type_e {
	RTC_TYPE_BCD,
	RTC_TYPE_DECIMAL,
} rtc_num_t;

typedef enum rtc_set_clock_flag_e {
	RTC_SET,
	RTC_SET_HOUR,
	RTC_SET_MIN,
	RTC_SET_FLAG_MAX,
} rtc_set_clock_flag_t;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
extern volatile flag_t process_flag;
extern volatile rtc_set_clock_flag_t rtc_set_flag;
extern uint8_t uart_rx_buf[RX_SIZE], uart_tx_buf[TX_SIZE];

__STATIC_INLINE flag_t get_process_flag(void){
	return process_flag;
}

__STATIC_INLINE void set_process_flag(flag_t flag){
	process_flag = flag;
}

__STATIC_INLINE void clear_process_flag(void){
	process_flag = FLAG_STANDBY;
}
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
