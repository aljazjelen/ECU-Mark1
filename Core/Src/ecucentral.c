/*
 * ecucentral.c
 *
 *  Created on: Dec 3, 2020
 *      Author: aljaz
 */

#include "main.h"
#include "ecucentral.h"
#include "systemconfig.h"
#include "crankshaft.h"
#include "camshaft.h"
#include "ignition.h"
#include "injection.h"
#include "errorhandler.h"
#include "generalfunctions.h"
#include "dio.h"
#include "adc.h"

ECU_Info ECU;
ECU_Modes ECU_Mode = sleep;

void ECU_MainLoop(){
	HAL_GPIO_TogglePin(GPIOD,Led_Green_Pin);
	// TODO, flip GPIO at the beginning and at the end, to see whether everything is done in 10ms or not. Goal is 5ms at 9000 RPM! (to make sure enough CPU is available)
	// get all ADCs
	// get all CAN messages
	// get all SPIs (if needed)
	// run the loop
	ECU_CommonFunctions();
	DIO_getAllPorts();



	switch (ECU_Mode){
	case sleep:
		ECU_Mode = initialize;
		break;
	case initialize:
		// Run initialization sequence, whatever is needed
		ECU_InitLoop();	//todo move all init sequence here if needed
		ECU_Mode = idle;
		// Transit to IDLE only if error free
		break;
	case idle:
		// Run any necessary checks before start. Wait for ECU_Term15
		// Transit to cranking only if error free and upon starter signal TODO make starter signal on Teensy

		if ((ECU_Ignition == 1)&&(ECU_Term15 == 1)){ECU_Mode = cranking;}
		//if (ECU_Term15 == 0){ECU_Mode = postdrive;} //TODO used or not?
		// Transit to postdrive if contact taken
		// if ECU.KL15 == 0, ECU_StMachine.mode = postdrive
		break;
	case cranking:
		// here the cranking procedure comes, if needed
		ECU_CrankingLoop();
		if (ECU_Term15 == 0){ECU_Mode = postdrive;}
		if (ECU_Ignition == 0){ECU_Mode = idle;}
		if (Crank_ShaftFreqHz >= 12){ECU_Mode = running;}
		// Transit to running when speed is above threshold (lets say 1000 RPM)
		// if speed >= 1000, ECU_StMachine.mode = running
		break;
	case running:
		// here will be the running loop, 99% of things happen here
		ECU_RunLoop();
		if (ECU_Term15 == 0){ECU_Mode = postdrive;}
		// Transit to postdrive if contact taken
		// if ECU.KL15 == 0, ECU_StMachine.mode = postdrive
		break;
	case postdrive:
		// here is post drive, like ventilators, pumps, lights and stuff
		// save to diagnostics to NVM if possible TODO
		ECU_PostDriveLoop();
		ECU_Mode = idle;	// TODO, maybe add a timer, or counter, how many times the post drive is executed.
		break;
	}
	// TODO, flip GPIO at the beginning and at the end, to see whether everything is done in 10ms or not. Goal is 5ms at 9000 RPM! (to make sure enough CPU is available)
}

/*
void ECU_GetADCs(){
	// Get ADC values
*/

void ECU_CommonFunctions(){
	// Execute all functions which are common in all states (like crankshaft/camshaft related) TODO, check if ignition driver can be disabled during Init sequence
	Ignition_MainLoop();
	Injection_CalcFuelQty();
}

void ECU_CheckTransitConditions(){
	// here all transition conditions are checked. For example engine speed etc.
}
void ECU_InitLoop(){
	// Init sequence here
	/*
	ErrorHandler_Init();
	Crank_Init(&htim12,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);		// Initialize Crankshaft related sensing + (Hall Timer)
	Cam_Init(&htim12,TIM_CHANNEL_2,HAL_TIM_ACTIVE_CHANNEL_2);			// Initialize Camshaft related sensing + (Hall Timer)


	Ignition_Init_FireTIM(&htim10,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
	Ignition_Init_DwellTIM(&htim11,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
	Ignition_Init_IOConfig(GPIOD,Ignition_Cyl1_Pin,GPIOD,Ignition_Cyl2_Pin,GPIOD,Ignition_Cyl3_Pin,GPIOD,Ignition_Cyl4_Pin);
	Ignition_Init_IgnitionCyl();

	// Injection Init Functions
	Injection_Init_InjectorTIM(&htim14,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
	Injection_Init_IOConfig(GPIOD,Injection_Cyl1_Pin,GPIOD,Injection_Cyl2_Pin,GPIOD,Injection_Cyl3_Pin,GPIOD,Injection_Cyl4_Pin);
	Injection_Init_InjectionCyl();

	ADC_Init_TriggerTIM(&htim3,TIM_CHANNEL_1,HAL_TIM_ACTIVE_CHANNEL_1);
	ADC_Init_ADC1(&hadc1);
	*/
}

void ECU_IdleLoop(){
	// Idle sequence here
}
void ECU_CrankingLoop(){
	// Cranking sequence here
	HAL_GPIO_TogglePin(GPIOD,Led_Blue_Pin);
	Ignition_MainLoop();
}
void ECU_RunLoop(){
	// Running sequence here
	HAL_GPIO_WritePin(GPIOD,Led_Blue_Pin,GPIO_PIN_SET);
	Ignition_MainLoop();
}
void ECU_PostDriveLoop(){
	// Post Drive sequence here
	HAL_GPIO_WritePin(GPIOD,Led_Blue_Pin,GPIO_PIN_RESET);
}

