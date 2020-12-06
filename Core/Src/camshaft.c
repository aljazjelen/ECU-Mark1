/*
 * camshaft.c
 *
 *  Created on: Nov 22, 2020
 *      Author: aljaz
 */

#include "camshaft.h"

// GLOBALS
Cam_HallSensor_TIM_Config Cam_HallTIM; 	// Global Configuration of Channel and TIM for Hall Sensor
int Cam_HalfCycle = 0;					// Cam-Crank Cyle identifier. When Camshaft indicates a start of new cycle, ID is set to 0 [0-360]. ID can be set to 1 [360-720] when the crankshaft rotation restarts


/* Initialization of variables and TIM related start-up */
void Cam_Init(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel){
	Cam_HallTIM.Handler = Handler;
	Cam_HallTIM.Channel = Channel;
	Cam_HallTIM.ActiveChannel = ActiveChannel;
	HAL_TIM_Base_Start_IT(Handler);
	HAL_TIM_IC_Start_IT(Handler, Channel); // HALL Timer Initialization
}

void Cam_CrankPositionSync(){
	Cam_HalfCycle = 1-Cam_HalfCycle;
}

void Cam_HalGeberDriver(){

	// Check if the CamShaft indicates start of the new Cam cycle
	// If it does, reset the variable if it already int
	// TODO posssible Error check here. Similar to crankshaft.
	Cam_HalfCycle = 0; // This is definitely 0, if CAM is on the initial phase.
	/*
	uint32_t capturedValue;
	//int Crank_ShaftFreqHzRaw = 0;
	//channelId = PWM_IC_CHANNEL_FLOW_METER;

	// Get CCR register for the specific Timer and Channel
	capturedValue = HAL_TIM_ReadCapturedValue(Crank_HallTIM.Handler, Crank_HallTIM.Channel);

	if( capturedValue > Crank_LastCapturedEdgeTime )
		// Compute the input signal frequency
		Crank_RotDelta = capturedValue - Crank_LastCapturedEdgeTime;
	else
	// Timer counter overflow
		Crank_RotDelta = ( 0xFFFF - Crank_LastCapturedEdgeTime ) + capturedValue;

	// Compute the input signal frequency
	Crank_RotDelta = Crank_RotDelta*(1+Crank_HallTIM.Handler->Instance->PSC);
	Crank_ShaftFreqHzRaw = 2*HAL_RCC_GetPCLK1Freq()/Crank_RotDelta/(Crank_TeethNmbr_P+Crank_MissingTeethNmbr_P);  // calculate frequency 2* because APB2 has clock multiplied by 2 for TIM!

	// Update the last captured value
	Crank_LastCapturedEdgeTime = capturedValue;
	Crank_PosDiff = Crank_RotDelta;

	// Check for the empty tooth - if the difference between timestamps is bigger than usually.
	if(((Crank_PosDiff-Crank_PosDiffOld) >= 0.7*Crank_PosDiffOld) && (Crank_PosDiff >= Crank_PosDiffOld)){
		Crank_PosDiffOld = Crank_PosDiff;

		Crank_ShaftFreqHz = Crank_ShaftFreqHz; // take the last valid value in case the tooth is missing
		if (Crank_TeethCount >= Crank_TeethNmbr_P-1){
			ErrorHandler_Increase(Crank_ToothJump);
		}else{
			ErrorHandler_Decrease(Crank_ToothJump);
		}
		Crank_TeethCounterReset();					// reset the counter due to the larger space between teeth
	}
	else{
		Crank_PosDiffOld = Crank_PosDiff;
		Crank_ShaftFreqHz = 0.5*Crank_ShaftFreqHz + 0.5*Crank_ShaftFreqHzRaw; // filter frequency if the tooth isnt missing
		//Crank_TeethCount++;
	}
	*/
}
