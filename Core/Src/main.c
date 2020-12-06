/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "systemconfig.h"
#include "crankshaft.h"
#include "ignition.h"
#include "camshaft.h"
#include "injection.h"
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
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim12;
TIM_HandleTypeDef htim14;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM11_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM12_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM14_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_TIM11_Init();
  MX_TIM3_Init();
  MX_TIM12_Init();
  MX_TIM10_Init();
  MX_TIM14_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  /* Put Custom INIT here, since peripherals need to be initialized first */
  // Scheduler_Init(); here the TIM channel is assigned to scheduler 10ms and initialized to run periodically
  ErrorHandler_Init();

  Crank_Init(&htim12,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);		// Initialize Crankshaft related sensing + (Hall Timer)
  Cam_Init(&htim12,TIM_CHANNEL_2,HAL_TIM_ACTIVE_CHANNEL_2);			// Initialize Camshaft related sensing + (Hall Timer)

  Ignition_Init_FireTIM(&htim10,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
  Ignition_Init_DwellTIM(&htim11,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
  Ignition_Init_IOConfig(GPIOD,Ignition_Cyl1_Pin,GPIOD,Ignition_Cyl2_Pin,GPIOD,Ignition_Cyl3_Pin,GPIOD,Ignition_Cyl4_Pin);
  Ignition_Init_IgnitionCyl();


  Injection_Init_InjectorTIM(&htim14,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
  Injection_Init_IOConfig(GPIOD,Injection_Cyl1_Pin,GPIOD,Injection_Cyl2_Pin,GPIOD,Injection_Cyl3_Pin,GPIOD,Injection_Cyl4_Pin);
  Injection_Init_InjectionCyl();


  HAL_TIM_Base_Start_IT(&htim3); // TODO move to Scheduler Init
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1); // TODO move to Scheduler Init




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(100);
	  //HAL_GPIO_TogglePin(GPIOD,Led_Red_Pin);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 252;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_TRG_COM_TIM11_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* TIM8_BRK_TIM12_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
  /* TIM1_UP_TIM10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  /* TIM8_TRG_COM_TIM14_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 60000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 167;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 10;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim10, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 167;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 10;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim11, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM12_Init(void)
{

  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 83;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 65535;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM12_Init 2 */

  /* USER CODE END TIM12_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 83;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 10;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim14, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Led_Green_Pin|Led_Orange_Pin|Led_Red_Pin|Led_Blue_Pin
                          |Injection_Cyl1_Pin|Injection_Cyl2_Pin|Injection_Cyl3_Pin|Injection_Cyl4_Pin
                          |Ignition_Cyl4_Pin|Ignition_Cyl2_Pin|Ignition_Cyl3_Pin|Ignition_Cyl1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Led_Green_Pin Led_Orange_Pin Led_Red_Pin Led_Blue_Pin
                           Injection_Cyl1_Pin Injection_Cyl2_Pin Injection_Cyl3_Pin Injection_Cyl4_Pin
                           Ignition_Cyl4_Pin Ignition_Cyl2_Pin Ignition_Cyl3_Pin Ignition_Cyl1_Pin */
  GPIO_InitStruct.Pin = Led_Green_Pin|Led_Orange_Pin|Led_Red_Pin|Led_Blue_Pin
                          |Injection_Cyl1_Pin|Injection_Cyl2_Pin|Injection_Cyl3_Pin|Injection_Cyl4_Pin
                          |Ignition_Cyl4_Pin|Ignition_Cyl2_Pin|Ignition_Cyl3_Pin|Ignition_Cyl1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void Scheduler_10ms(){
	// Call all 10ms related functions
	Ignition_MainLoop();
	Injection_CalcFuelQty();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim == &htim3){

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
			HAL_GPIO_TogglePin(GPIOD,Led_Blue_Pin);
		}else{
			HAL_GPIO_TogglePin(GPIOD,Led_Green_Pin);
			Scheduler_10ms(); // Call 10ms Scheduler
		}
	}else{
		__NOP();
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){



	if(htim == Injection_InjectorTIM.Handler){
		// Injector OPM TIM Channels
		if(htim->Channel == Injection_InjectorTIM.ActiveChannel){
			HAL_GPIO_WritePin(GPIOD,Injection_Cyl1_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,Injection_Cyl2_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,Injection_Cyl3_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,Injection_Cyl4_Pin,GPIO_PIN_RESET);
		}
	}

	if(htim == Ignition_FireTIM.Handler){
		// Ignition OPM TIM Channels
		if(htim->Channel == Ignition_FireTIM.ActiveChannel){
			// Cylinder 1
			if (Ignition_Cyl1.firestate == TRIG_FIRE){
				HAL_GPIO_WritePin(GPIOD,Led_Blue_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,Led_Orange_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl1_Pin,GPIO_PIN_RESET);
				Ignition_FireStarted(&Ignition_Cyl1);
			}
			// Cylinder 2
			if (Ignition_Cyl2.firestate == TRIG_FIRE){
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl2_Pin,GPIO_PIN_RESET);
				Ignition_FireStarted(&Ignition_Cyl2);}
			// Cylinder 3
			if (Ignition_Cyl3.firestate == TRIG_FIRE){
				HAL_GPIO_WritePin(GPIOD,Led_Blue_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,Led_Orange_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl3_Pin,GPIO_PIN_RESET);
				Ignition_FireStarted(&Ignition_Cyl3);
			}
			// Cylinder 4
			if (Ignition_Cyl4.firestate == TRIG_FIRE){
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl4_Pin,GPIO_PIN_RESET);
				Ignition_FireStarted(&Ignition_Cyl4);}
		}
	}

	if(htim == Ignition_DwellTIM.Handler){
		// Dwell OPM TIM Channels
		if(htim->Channel == Ignition_DwellTIM.ActiveChannel){
			// Cylinder 1
			if (Ignition_Cyl1.dwellstate == TRIG_DWELL){
				HAL_GPIO_WritePin(GPIOD,Led_Blue_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD,Led_Orange_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl1_Pin,GPIO_PIN_SET);
				Ignition_DwellStarted(&Ignition_Cyl1);
			}
			// Cylinder 2
			if (Ignition_Cyl2.dwellstate == TRIG_DWELL){
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl2_Pin,GPIO_PIN_SET);
				Ignition_DwellStarted(&Ignition_Cyl2);}
			// Cylinder 3
			if (Ignition_Cyl3.dwellstate == TRIG_DWELL){
				HAL_GPIO_WritePin(GPIOD,Led_Blue_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD,Led_Orange_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl3_Pin,GPIO_PIN_SET);
				Ignition_DwellStarted(&Ignition_Cyl3);
			}
			// Cylinder 4
			if (Ignition_Cyl4.dwellstate == TRIG_DWELL){
				HAL_GPIO_WritePin(GPIOD,Ignition_Cyl4_Pin,GPIO_PIN_SET);
				Ignition_DwellStarted(&Ignition_Cyl4);}
		}
	}
}

void HAL_TIM_IC_CaptureCallback( TIM_HandleTypeDef *htim ){

	if(htim == Cam_HallTIM.Handler){
		if(htim->Channel == Cam_HallTIM.ActiveChannel){
			Cam_HalGeberDriver(); // TODO implement real driver, for now its only deteting a singl rising signal.
		}
	}

	if(htim == Crank_HallTIM.Handler){
		if(htim->Channel == Crank_HallTIM.ActiveChannel){
			// Ignite before halgeber driver, if the tooth is fine. There is huge delay (30-40us) in high speed9:
			Crank_AngleCalc(); // roughly 10us computation time
			if (Crank_TeethCount == 1)
				Cam_CrankPositionSync();


			// INJECTOR PART
			if ((Cam_HalfCycle == Injection_Cyl1.injectCamCycle) && (Crank_TeethCount == Injection_Cyl1.injectiontooth))
				Injection_StartTimerInjectCylinder(&Injection_Cyl1);
			if ((Cam_HalfCycle == Injection_Cyl2.injectCamCycle) && (Crank_TeethCount == Injection_Cyl2.injectiontooth))
				Injection_StartTimerInjectCylinder(&Injection_Cyl2);
			if ((Cam_HalfCycle == Injection_Cyl3.injectCamCycle) && (Crank_TeethCount == Injection_Cyl3.injectiontooth))
				Injection_StartTimerInjectCylinder(&Injection_Cyl3);
			if ((Cam_HalfCycle == Injection_Cyl4.injectCamCycle) && (Crank_TeethCount == Injection_Cyl4.injectiontooth))
				Injection_StartTimerInjectCylinder(&Injection_Cyl4);

			// IGNITION PART
			// Cylinder 1
			if ((Cam_HalfCycle == Ignition_Cyl1.dwellCamCycle) && (Crank_TeethCount == Ignition_Cyl1.dwelltooth))
				Ignition_StartTimerDwellCylinder(&Ignition_Cyl1);

			if ((Cam_HalfCycle == Ignition_Cyl1.fireCamCycle) && (Crank_TeethCount == Ignition_Cyl1.firetooth))
				Ignition_StartTimerFireCylinder(&Ignition_Cyl1);

			// Cylinder 2
			if ((Cam_HalfCycle == Ignition_Cyl2.dwellCamCycle) && (Crank_TeethCount == Ignition_Cyl2.dwelltooth))
				Ignition_StartTimerDwellCylinder(&Ignition_Cyl2);

			if ((Cam_HalfCycle == Ignition_Cyl2.fireCamCycle) && (Crank_TeethCount == Ignition_Cyl2.firetooth))
				Ignition_StartTimerFireCylinder(&Ignition_Cyl2);

			// Cylinder 3
			if ((Cam_HalfCycle == Ignition_Cyl3.dwellCamCycle) && (Crank_TeethCount == Ignition_Cyl3.dwelltooth))
				Ignition_StartTimerDwellCylinder(&Ignition_Cyl3);

			if ((Cam_HalfCycle == Ignition_Cyl3.fireCamCycle) && (Crank_TeethCount == Ignition_Cyl3.firetooth))
				Ignition_StartTimerFireCylinder(&Ignition_Cyl3);

			// Cylinder 4
			if ((Cam_HalfCycle == Ignition_Cyl4.dwellCamCycle) && (Crank_TeethCount == Ignition_Cyl4.dwelltooth))
				Ignition_StartTimerDwellCylinder(&Ignition_Cyl4);

			if ((Cam_HalfCycle == Ignition_Cyl4.fireCamCycle) && (Crank_TeethCount == Ignition_Cyl4.firetooth))
				Ignition_StartTimerFireCylinder(&Ignition_Cyl4);

			// Call the Hall driver at the end. Since it takes most time and it can be called at the end, since the tooth counter is implemented at the beginning,
			// therefore it is not critical for performance.
			Crank_HalGeberDriver();
		}
	}else{
		__NOP();
	}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
