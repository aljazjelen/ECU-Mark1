/*
 * ignition.c
 *
 *  Created on: Sep 21, 2020
 *      Author: Aljaž Jelen
 */
#include "systemconfig.h"
#include "ignition.h"
#include "crankshaft.h"
#include "errorhandler.h"

Ignition_TIM_Config  Ignition_FireTIM;		// Global Configuration of Channel and TIM for Ignition Branch1
Ignition_TIM_Config  Ignition_DwellTIM;		// Global Configuration of Channel and TIM for Ignition Branch2

Ignition_CoilCyl Ignition_Cyl1;				// Global Variable for Cylinder 1, containing all Info regarding Ignition
Ignition_CoilCyl Ignition_Cyl2;				// Global Variable for Cylinder 2, containing all Info regarding Ignition
Ignition_CoilCyl Ignition_Cyl3;				// Global Variable for Cylinder 3, containing all Info regarding Ignition
Ignition_CoilCyl Ignition_Cyl4;				// Global Variable for Cylinder 4, containing all Info regarding Ignition

// INPUTS
float _Crank_ShaftFreqHz = 0;
//

// LOCALS
int Ignition_DwellTimeUs_P = 2000;	// Global Variable for Dwell Time of ignition coils, calculated from maps (1000us to 2000us)DC


// INIT FUNCTIONS  =================================================
/*
 *  Function to initialize timer related variables of first branch (cylinders 1 and 2)
 */
void Ignition_Init_FireTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel){
	Ignition_FireTIM.Handler = Handler;
	Ignition_FireTIM.Channel = Channel;
	Ignition_FireTIM.ActiveChannel = ActiveChannel;
	HAL_TIM_Base_Start_IT(Handler);
	HAL_TIM_OC_Start_IT(Handler, Channel);
}
/*
 *  Function to initialize timer related variables of second branch (cylinders 3 and 4)
 */
void Ignition_Init_DwellTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel){
	Ignition_DwellTIM.Handler = Handler;
	Ignition_DwellTIM.Channel = Channel;
	Ignition_DwellTIM.ActiveChannel = ActiveChannel;
	HAL_TIM_Base_Start_IT(Handler);
	HAL_TIM_OC_Start_IT(Handler, Channel);
}

void Ignition_Init_IOConfig(GPIO_TypeDef *coilport1,__IO uint16_t coilpin1,GPIO_TypeDef *coilport2,__IO uint16_t coilpin2,GPIO_TypeDef *coilport3,__IO uint16_t coilpin3,GPIO_TypeDef *coilport4,__IO uint16_t coilpin4)
{
	Ignition_Cyl1.coilport = coilport1;
	Ignition_Cyl1.coilpin = coilpin1;
	Ignition_Cyl2.coilport = coilport2;
	Ignition_Cyl2.coilpin = coilpin2;
	Ignition_Cyl3.coilport = coilport3;
	Ignition_Cyl3.coilpin = coilpin3;
	Ignition_Cyl4.coilport = coilport4;
	Ignition_Cyl4.coilpin = coilpin4;
}

void Ignition_Init_IgnitionCyl(){
	Ignition_Cyl1.cylinderID = 1;
	Ignition_Cyl1.firestate = FIRED;
	Ignition_Cyl1.fireangle = 30;
	Ignition_Cyl1.firetooth = 4;
	Ignition_Cyl1.time2fire = 10;
	Ignition_Cyl1.fireCamCycle = 0;
	Ignition_Cyl1.dwellstate = IDLE;
	Ignition_Cyl1.dwellangle = 160;
	Ignition_Cyl1.dwelltooth = 2;
	Ignition_Cyl1.time2dwell = 10;
	Ignition_Cyl1.dwellCamCycle = 0;

	Ignition_Cyl2.cylinderID = 2;
	Ignition_Cyl2.firestate = FIRED;
	Ignition_Cyl2.fireangle = 30;
	Ignition_Cyl2.firetooth = 12;
	Ignition_Cyl2.time2fire = 10;
	Ignition_Cyl2.fireCamCycle = 1;
	Ignition_Cyl2.dwellstate = IDLE;
	Ignition_Cyl2.dwellangle = 160;
	Ignition_Cyl2.dwelltooth = 9;
	Ignition_Cyl2.time2dwell = 10;
	Ignition_Cyl2.dwellCamCycle = 1;

	Ignition_Cyl3.cylinderID = 3;
	Ignition_Cyl3.firestate = FIRED;
	Ignition_Cyl3.fireangle = 250;
	Ignition_Cyl3.firetooth = 8;
	Ignition_Cyl3.time2fire = 10;
	Ignition_Cyl3.fireCamCycle = 0;
	Ignition_Cyl3.dwellstate = IDLE;
	Ignition_Cyl3.dwellangle = 280;
	Ignition_Cyl3.dwelltooth = 5;
	Ignition_Cyl3.time2dwell = 10;
	Ignition_Cyl3.dwellCamCycle = 0;

	Ignition_Cyl4.cylinderID = 4;
	Ignition_Cyl4.firestate = FIRED;
	Ignition_Cyl4.fireangle = 250;
	Ignition_Cyl4.firetooth = 16;
	Ignition_Cyl4.time2fire = 10;
	Ignition_Cyl4.fireCamCycle = 1;
	Ignition_Cyl4.dwellstate = IDLE;
	Ignition_Cyl4.dwellangle = 280;
	Ignition_Cyl4.dwelltooth = 10;
	Ignition_Cyl4.time2dwell = 10;
	Ignition_Cyl4.dwellCamCycle = 1;
}


// MAIN IGNITION LOOP ==============================================
void Ignition_GetInputs(){
	_Crank_ShaftFreqHz = Crank_ShaftFreqHz;
}

void Ignition_MainLoop(){
	// set for all cylinders
	Ignition_GetInputs();
	Ignition_GetDwellTime(250);
	Ignition_SetDwellTiming(&Ignition_Cyl1);
	Ignition_SetIgnitionTiming(&Ignition_Cyl1);
	Ignition_SetDwellTiming(&Ignition_Cyl2);
	Ignition_SetIgnitionTiming(&Ignition_Cyl2);
	Ignition_SetDwellTiming(&Ignition_Cyl3);
	Ignition_SetIgnitionTiming(&Ignition_Cyl3);
	Ignition_SetDwellTiming(&Ignition_Cyl4);
	Ignition_SetIgnitionTiming(&Ignition_Cyl4);
}


// FIRE FUNCTIONS ==================================================
void Ignition_DefineIgnitionTeeth(Ignition_CoilCyl *Cylinder){
	int CrankTeethNmbr = CrankCfg.Crank_TeethNmbr_P;
	int CrankMissingTeethNmbr = CrankCfg.Crank_MissingTeethNmbr_P;
	float fireangle = Cylinder->fireangle;
	int triggerTooth = 0;

	triggerTooth = (CrankTeethNmbr+CrankMissingTeethNmbr)*fireangle/360+1;
	Cylinder->firetooth = triggerTooth;
}

/**
  * @brief Ignition Function which will set the ignition timer and the tooth number at which timer has to start counting, in order to fire at exact angle.
  * @param Cylinder - Info about the cylinder chosen
  * @retval None
  */
void Ignition_SetIgnitionTiming(Ignition_CoilCyl *Cylinder){
	Ignition_DefineIgnitionTeeth(Cylinder);		// Define the closest previous tooth at given cylinder ignition angle

	float requestedAngle = Cylinder->fireangle;
	int triggerTooth = Cylinder->firetooth;
	float triggerAngle;
	float delayAngle;
	uint32_t Ignition_TimeToFire;

	// Perform correction if angle is after the missing teeth
	if (triggerTooth == CrankCfg.Crank_TeethNmbr_P+1){
		triggerTooth = 0;
		delayAngle = 0;
	}else{
		triggerAngle = (triggerTooth-1)*360/(CrankCfg.Crank_TeethNmbr_P+CrankCfg.Crank_MissingTeethNmbr_P);
		delayAngle = requestedAngle - triggerAngle;
		Ignition_TimeToFire = Ignition_AngleToUs(_Crank_ShaftFreqHz,delayAngle);
	}

	// correction in case CCR and ARR dont work with 0
	if (Ignition_TimeToFire == 0)
		Ignition_TimeToFire = 1;

	Cylinder->time2fire = Ignition_TimeToFire;
	Cylinder->firetooth = triggerTooth;
}


// DWELL FUNCTIONS =================================================
void Ignition_GetDwellTime(float AppliedTorque){
	Ignition_DwellTimeUs_P = 2000;
}

void Ignition_DefineDwellTeeth(Ignition_CoilCyl *Cylinder){
	int CrankTeethNmbr = CrankCfg.Crank_TeethNmbr_P;
	int CrankMissingTeethNmbr = CrankCfg.Crank_MissingTeethNmbr_P;
	float fireangle = Cylinder->fireangle;
	int triggerTooth = 0;

	Cylinder->dwellCamCycle = Cylinder->fireCamCycle;

	float dwellangle = fireangle - Ignition_DwellTimeUs_P*_Crank_ShaftFreqHz*360/1000000;
	if (dwellangle < 0){
		dwellangle = dwellangle + 360;

		// Do a correction. Dwell has to occur in the previous Cam cycle already!
		if (Cylinder->fireCamCycle == 0)
			Cylinder->dwellCamCycle = 1;
		else
			Cylinder->dwellCamCycle = 0;
	}

	triggerTooth = (CrankTeethNmbr+CrankMissingTeethNmbr)*dwellangle/360+1;

	// Correction in case triggertooth is bound to be one of the missing teeth on he crankwheel
	if (triggerTooth >= CrankTeethNmbr+1)
			triggerTooth = CrankTeethNmbr;

	Cylinder->dwellangle = dwellangle;
	Cylinder->dwelltooth = triggerTooth;
}

void Ignition_SetDwellTiming(Ignition_CoilCyl *Cylinder){
	Ignition_DefineDwellTeeth(Cylinder);		// Define the closest previous tooth at given cylinder dwell angle

	float requestedAngle = Cylinder->dwellangle;
	int triggerTooth = Cylinder->dwelltooth;
	float triggerAngle;
	float delayAngle;
	uint32_t Ignition_TimeToDwell;

	// Perform correction if angle is after the missing teeth
	if (triggerTooth == CrankCfg.Crank_TeethNmbr_P+10){
		triggerTooth = CrankCfg.Crank_TeethNmbr_P;
		delayAngle = 360*CrankCfg.Crank_MissingTeethNmbr_P/CrankCfg.Crank_TeethNmbr_P;
	}else{
		triggerAngle = (triggerTooth-1)*360/(CrankCfg.Crank_TeethNmbr_P+CrankCfg.Crank_MissingTeethNmbr_P);
		delayAngle = requestedAngle - triggerAngle;
	}
	Ignition_TimeToDwell = Ignition_AngleToUs(_Crank_ShaftFreqHz,delayAngle);
	// correction in case CCR and ARR dont work with 0
	if (Ignition_TimeToDwell == 0)
		Ignition_TimeToDwell = 1;

	Cylinder->time2dwell = Ignition_TimeToDwell;
	Cylinder->dwelltooth = triggerTooth;
}


// HELPER FUNCTIONS ================================================
uint32_t Ignition_AngleToUs(int CrankShaftHz, float AngleDegree){
	return HAL_RCC_GetPCLK2Freq()*2*AngleDegree/360/_Crank_ShaftFreqHz/TIM10->PSC; // multiplied by 2, since the TIM module can run 2times faster than the clock
}

// FIRE DRIVER FUNCTIONS ===========================================
void Ignition_DriverLoop(int Cam_HalfCycle, uint8_t Crank_TeethCount){
	// Start ignition timers (either Fire or Dwell) based on Camshaft and Cranshaft position.

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
}

void Ignition_StartTimerFireCylinder(Ignition_CoilCyl *Cylinder){
	Cylinder->firestate = TRIG_FIRE;
	uint32_t fireStartInUs = Cylinder->time2fire;

	// START IGNITION TIM OUTPUT
	Ignition_FireTIM.Handler->Instance->ARR = fireStartInUs + 10;
	Ignition_FireTIM.Handler->Instance->CCR1 = fireStartInUs;
	Ignition_FireTIM.Handler->Instance->EGR = 1;
	Ignition_FireTIM.Handler->Instance->CR1 |= TIM_CR1_CEN;
}

void Ignition_FireStarted(Ignition_CoilCyl *Cylinder){Cylinder->firestate = FIRED;}


// DWELL DRIVER FUNCTIONS ==========================================

void Ignition_StartTimerDwellCylinder(Ignition_CoilCyl *Cylinder){
	Cylinder->dwellstate = TRIG_DWELL;
	uint32_t dwellStartInUs = Cylinder->time2dwell;

	// START DWELL TIM OUTPUT
	Ignition_DwellTIM.Handler->Instance->ARR = dwellStartInUs+10;
	Ignition_DwellTIM.Handler->Instance->CCR1 = dwellStartInUs;
	Ignition_DwellTIM.Handler->Instance->EGR = 1;
	Ignition_DwellTIM.Handler->Instance->CR1 |= TIM_CR1_CEN;
}

void Ignition_DwellStarted(Ignition_CoilCyl *Cylinder){Cylinder->dwellstate = DWELLING;}


