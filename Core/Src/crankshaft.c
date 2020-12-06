/*
 * Crankshaft.c
 *
 *  Created on: Oct 4, 2020
 *      Author: aljaz
 */


#include "systemconfig.h"
#include "crankshaft.h"
#include "errorhandler.h"


Crank_HallSensor_TIM_Config Crank_HallTIM; // Global Configuration of Channel and TIM for Hall Sensor

float Crank_ShaftFreqHz = 0;	// Global Variable of estimated Crankshaft frequency in Hz (filtered)
float Crank_ShaftFreqHzRaw = 0;	// Global Variable of estimated Crankshaft freq in Hz (non filtered, no missing tooth compensation)
uint32_t Crank_RotDelta = 0;	// Global Variable of exact delta time between 2 teeth
uint32_t Crank_PosDiff = 0;		// Global Variable to track Crankshaft Tooth New timing difference
uint32_t Crank_PosDiffOld = 0;	// Global Variable to track Crankshaft Tooth Old timing difference
uint8_t Crank_TeethCount = 0;	// Global Variable of exact Current Tooth Number
uint16_t Crank_Angle = 0;		// Global Variable of estimated Crankshaft Angle
uint32_t Crank_DivAngle = 0;	// Global Variable used to calculate angle between 2 rising edges of the tooth

// TODO move to own files (engine. where all central variables are)
uint16_t Engine_Angle = 0;		// Global Variable of estimated Engine System

// LOCALS
uint32_t Crank_LastCapturedEdgeTime = 0;	// Global Time Stamp for last detected edge from Hal Sensor
uint8_t Crank_bErrToothJump = 0;			// Global Error flag when counted teeth and total tooth number dont match

// PARAMETERS
// Declared in
// TODO add a parameter which varries threshold for missing tooth detection algorithm

/* Initialization of variables and TIM related start-up */
void Crank_Init(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel){
	CrankCfg.Crank_TeethNmbr_P = 17;
	CrankCfg.Crank_MissingTeethNmbr_P = 1;

	Crank_DivAngle = 360/CrankCfg.Crank_MissingTeethNmbr_P;
	Crank_HallTIM.Handler = Handler;
	Crank_HallTIM.Channel = Channel;
	Crank_HallTIM.ActiveChannel = ActiveChannel;
	HAL_TIM_Base_Start_IT(Handler);
	HAL_TIM_IC_Start_IT(Handler, Channel); // HALL Timer Initialization
}

/* Driver for calculating the speed and resetting the counter upon empty slot */
void Crank_HalGeberDriver(){
	uint32_t capturedValue;
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
	float clkFreq = HAL_RCC_GetPCLK1Freq();
	Crank_ShaftFreqHzRaw = 2*clkFreq/Crank_RotDelta/(CrankCfg.Crank_TeethNmbr_P+CrankCfg.Crank_MissingTeethNmbr_P);  // calculate frequency 2* because APB2 has clock multiplied by 2 for TIM!

	// Update the last captured value
	Crank_LastCapturedEdgeTime = capturedValue;
	Crank_PosDiff = Crank_RotDelta;

	// Check for the empty tooth - if the difference between timestamps is bigger than usually.
	if(((Crank_PosDiff-Crank_PosDiffOld) >= 0.7*Crank_PosDiffOld) && (Crank_PosDiff >= Crank_PosDiffOld)){
		Crank_PosDiffOld = Crank_PosDiff;

		Crank_ShaftFreqHz = Crank_ShaftFreqHz; // take the last valid value in case the tooth is missing
		if (Crank_TeethCount >= CrankCfg.Crank_TeethNmbr_P-1){
			ErrorHandler_Increase(Crank_ToothJump);
		}else{
			ErrorHandler_Decrease(Crank_ToothJump);
		}
		Crank_TeethCounterReset();					// reset the counter due to the larger space between teeth
	}
	else{
		Crank_PosDiffOld = Crank_PosDiff;
		Crank_ShaftFreqHz = 0.2*Crank_ShaftFreqHz + 0.8*Crank_ShaftFreqHzRaw; // filter frequency if the tooth isnt missing
	}
}

/* Helper funcion for Teeth Counter reset */
void Crank_TeethCounterReset(){
	Crank_TeethCount = 1;
}

/* Teeth Counter */
void Crank_TeethCounter(){
	if (Crank_TeethCount < CrankCfg.Crank_TeethNmbr_P)
		Crank_TeethCount = Crank_TeethCount + 1;
	else
		Crank_TeethCounterReset();
}

/* Calculation of Crankshaft angle */
void Crank_AngleCalc(){
	Crank_TeethCounter();
	Crank_Angle = 360*(Crank_TeethCount-1)/(CrankCfg.Crank_TeethNmbr_P+CrankCfg.Crank_MissingTeethNmbr_P);
}

/*
/* Used to synchronise angle calculation considering twice slower rotation of camshaft //
void Crank_CamPositionSync(){
	Engine_Angle = Crank_Angle + Cam_CycleStart*360;
}*/


