/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_KEY2_Pin GPIO_PIN_0
#define USER_KEY2_GPIO_Port GPIOA
#define USER_KEY1_Pin GPIO_PIN_1
#define USER_KEY1_GPIO_Port GPIOA
#define TX2_Pin GPIO_PIN_2
#define TX2_GPIO_Port GPIOA
#define RX2_Pin GPIO_PIN_3
#define RX2_GPIO_Port GPIOA
#define ADC1_Pin GPIO_PIN_6
#define ADC1_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_0
#define OLED_RES_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOB
#define ECHO_Pin GPIO_PIN_10
#define ECHO_GPIO_Port GPIOB
#define KEY_Y0_Pin GPIO_PIN_12
#define KEY_Y0_GPIO_Port GPIOB
#define KEY_Y1_Pin GPIO_PIN_13
#define KEY_Y1_GPIO_Port GPIOB
#define KEY_Y2_Pin GPIO_PIN_14
#define KEY_Y2_GPIO_Port GPIOB
#define KEY_Y3_Pin GPIO_PIN_15
#define KEY_Y3_GPIO_Port GPIOB
#define BZ_Pin GPIO_PIN_8
#define BZ_GPIO_Port GPIOA
#define TX1_Pin GPIO_PIN_9
#define TX1_GPIO_Port GPIOA
#define RX1_Pin GPIO_PIN_10
#define RX1_GPIO_Port GPIOA
#define KEY_X3_Pin GPIO_PIN_15
#define KEY_X3_GPIO_Port GPIOA
#define KEY_X2_Pin GPIO_PIN_3
#define KEY_X2_GPIO_Port GPIOB
#define KEY_X1_Pin GPIO_PIN_4
#define KEY_X1_GPIO_Port GPIOB
#define KEY_X0_Pin GPIO_PIN_5
#define KEY_X0_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_6
#define LED_B_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_8
#define LED_R_GPIO_Port GPIOB
#define TRIG_Pin GPIO_PIN_9
#define TRIG_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
