/*
 * crankshaft.h
 *
 *  Created on: Oct 4, 2020
 *      Author: aljaz
 */
#include "stm32f4xx_hal.h"

#ifndef INC_CRANKSHAFT_H_
#define INC_CRANKSHAFT_H_


typedef struct{
	TIM_HandleTypeDef *Handler;			// TIM Handler
	uint8_t Channel;					// TIM Config Channel
	uint8_t ActiveChannel;				// TIM Active Channel
}Crank_HallSensor_TIM_Config;

extern Crank_HallSensor_TIM_Config Crank_HallTIM;	// TIM Struct for Hall Sensor Config

extern float 	Crank_ShaftFreqHz;
extern float 	Crank_ShaftFreqHzRaw;
extern uint32_t Crank_PosDiff;			// Global Variable to track Crankshaft Tooth New timing difference
extern uint32_t Crank_PosDiffOld;		// Global Variable to track Crankshaft Tooth Old timing difference
extern uint8_t 	Crank_TeethCount; 		// Global Variable of exact Current Tooth Number
extern uint32_t Crank_RotDelta;
extern uint16_t Crank_Angle;
extern uint32_t Crank_DivAngle;			// Global Variable used to calculate angle between 2 rising edges of the tooth

extern uint8_t	Cam_CycleStart; // USED?

// LOCALS
uint32_t Crank_LastCapturedEdgeTime;
uint8_t 	Crank_bErrToothJump;

// PARAMETERS
extern uint8_t 	Crank_TeethNmbr_P;
extern uint8_t 	Crank_MissingTeethNmbr_P;

// METHODS
extern void Crank_Init(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel);
extern void Crank_HalGeberDriver();
extern void Crank_TeethCounterReset();
extern void Crank_AngleCalc();

#endif /* INC_CRANKSHAFT_H_ */
