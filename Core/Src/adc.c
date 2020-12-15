/*
 * adc.c
 *
 *  Created on: Dec 9, 2020
 *      Author: aljaz
 */


#include "adc.h"

uint32_t ADC1_Buffer[ADC1_CHANNELS];



ADC1_Pinout ADC1_Channels;

// GLOBALS
ADC_Trig_TIM_Config ADC_TriggerCfg;
ADC_Config ADC1_Cfg;
ADC_Config ADC2_Cfg;

// INITS =======================================================================================
void ADC_Init_TriggerTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel){
	HAL_TIM_Base_Start_IT(Handler);
	HAL_TIM_OC_Start_IT(Handler, Channel);
}

void ADC_Init_ADC1(ADC_HandleTypeDef *hadc){
	ADC1_Cfg.Handler = hadc;
	ADC1_Cfg.channels = ADC1_CHANNELS;
	ADC1_Cfg.id = 1;
	HAL_ADC_Start_DMA(hadc, ADC1_Buffer, ADC1_CHANNELS);
}

// METHODS =====================================================================================
extern uint16_t ADC1_getVal(ADC1_Pinout ADC1_Channel){return ADC1_Buffer[ADC1_Channel];}
extern uint16_t ADC2_getVal(ADC1_Pinout ADC2_Channel){return ADC1_Buffer[ADC2_Channel];}
extern uint16_t ADC3_getVal(ADC1_Pinout ADC3_Channel){return ADC1_Buffer[ADC3_Channel];}
