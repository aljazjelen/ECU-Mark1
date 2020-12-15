/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define TLE8888_CS_Pin GPIO_PIN_4
#define TLE8888_CS_GPIO_Port GPIOC
#define ECU_Term15_Pin GPIO_PIN_10
#define ECU_Term15_GPIO_Port GPIOE
#define ECU_Ignition_Pin GPIO_PIN_12
#define ECU_Ignition_GPIO_Port GPIOE
#define Led_Green_Pin GPIO_PIN_12
#define Led_Green_GPIO_Port GPIOD
#define Led_Orange_Pin GPIO_PIN_13
#define Led_Orange_GPIO_Port GPIOD
#define Led_Red_Pin GPIO_PIN_14
#define Led_Red_GPIO_Port GPIOD
#define Led_Blue_Pin GPIO_PIN_15
#define Led_Blue_GPIO_Port GPIOD
#define Injection_Cyl1_Pin GPIO_PIN_0
#define Injection_Cyl1_GPIO_Port GPIOD
#define Injection_Cyl2_Pin GPIO_PIN_1
#define Injection_Cyl2_GPIO_Port GPIOD
#define Injection_Cyl3_Pin GPIO_PIN_2
#define Injection_Cyl3_GPIO_Port GPIOD
#define Injection_Cyl4_Pin GPIO_PIN_3
#define Injection_Cyl4_GPIO_Port GPIOD
#define Ignition_Cyl4_Pin GPIO_PIN_4
#define Ignition_Cyl4_GPIO_Port GPIOD
#define Ignition_Cyl2_Pin GPIO_PIN_5
#define Ignition_Cyl2_GPIO_Port GPIOD
#define Ignition_Cyl3_Pin GPIO_PIN_6
#define Ignition_Cyl3_GPIO_Port GPIOD
#define Ignition_Cyl1_Pin GPIO_PIN_7
#define Ignition_Cyl1_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
