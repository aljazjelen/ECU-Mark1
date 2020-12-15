/*
 * adc.h
 *
 *  Created on: Dec 9, 2020
 *      Author: aljaz
 */

#include "stm32f4xx_hal.h"

#ifndef INC_ADC_H_
#define INC_ADC_H_

#define ADC1_CHANNELS 2
#define ADC2_CHANNELS 6

// ENUMS
typedef enum{
	ADC1_Empty0,
	ADC1_Temp1,
	ADC1_Temp2
}ADC1_Pinout;

// STRUCTS
typedef struct{
	TIM_HandleTypeDef *Handler;			// TIM Handler
	uint8_t Channel;					// TIM Config Channel
	uint8_t ActiveChannel;				// TIM Active Channel
}ADC_Trig_TIM_Config;

typedef struct{
	uint8_t id;
	ADC_HandleTypeDef *Handler;
	uint8_t channels;
}ADC_Config;

// GLOBALS
extern ADC1_Pinout ADC1_Channels;			// Global ADC1 pinout overview
extern ADC_Trig_TIM_Config ADC_TriggerCfg;	// Trigger for ADCs (TIM used)
extern ADC_Config ADC1_Cfg;					// Global Config for ADC1
extern ADC_Config ADC2_Cfg;					// Global Config for ADC2
extern uint32_t ADC1_Buffer[ADC1_CHANNELS];	// Buffer for ADC1
extern uint32_t ADC2_Buffer[ADC2_CHANNELS];	// Buffer for ADC2

// INITS
extern void ADC_Init_TriggerTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel);
extern void ADC_Init_ADC1();

// METHODS
extern uint16_t ADC1_getVal(ADC1_Pinout ADC1_Channel);
extern uint16_t ADC2_getVal(ADC1_Pinout ADC2_Channel);
extern uint16_t ADC3_getVal(ADC1_Pinout ADC3_Channel);

#endif /* INC_ADC_H_ */
