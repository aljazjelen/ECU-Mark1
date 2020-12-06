/*
 * camshaft.h
 *
 *  Created on: Nov 22, 2020
 *      Author: aljaz
 */

#ifndef INC_CAMSHAFT_H_
#define INC_CAMSHAFT_H_

#include "stm32f4xx_hal.h"

typedef struct{
	TIM_HandleTypeDef *Handler;			// TIM Handler
	uint8_t Channel;					// TIM Config Channel
	uint8_t ActiveChannel;				// TIM Active Channel
}Cam_HallSensor_TIM_Config;
extern Cam_HallSensor_TIM_Config Cam_HallTIM;	// TIM Struct for Hall Sensor Config

// GLOBALS
extern int Cam_HalfCycle;				// Cam-Crank Cyle identifier. When Camshaft indicates a start of new cycle, ID is set to 0 [0-360]. ID can be set to 1 [360-720] when the crankshaft rotation restarts

extern void Cam_Init(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel);
extern void Cam_CrankPositionSync();	// Function which is called by Crankshaft to set the halfCylce to 0 or 1. Depending on the angle. When Toothcount resets, halfcycle is toggled.
extern void Cam_HalGeberDriver();

#endif /* INC_CAMSHAFT_H_ */
