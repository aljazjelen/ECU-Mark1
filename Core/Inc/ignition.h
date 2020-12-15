/*
 * ignition.h
 *
 *  Created on: Sep 21, 2020
 *      Author: Aljaž Jelen
 */
#include "stm32f4xx_hal.h"

#ifndef INC_IGNITION_H_
#define INC_IGNITION_H_

// ENUMS
typedef enum Ignition_DwellCoilStates {
    IDLE = 0,
    TRIG_DWELL = 1,
    DWELLING = 2
}DwellCoilstate;

typedef enum Ignition_FireCoilStates {
    TRIG_FIRE = 0,
    FIRED = 1
}FireCoilstate;

// STRUCTS
typedef struct{
	TIM_HandleTypeDef *Handler;			// TIM Handler
	uint8_t Channel;					// TIM Config Channel
	uint8_t ActiveChannel;				// TIM Active Channel
}Ignition_TIM_Config;

typedef struct{
	int cylinderID;						// ID of the Cylinder [1,2,3,4]
	DwellCoilstate dwellstate;			// Enum which tells the state will be upon the next dwell tooth (used for pulse mode to optimize timing)
	FireCoilstate firestate;			// Enum which tells the state will be upon the next fire tooth (used for pulse mode to optimize timing)
	GPIO_TypeDef *coilport;				// Port of uC, where Coil of this cylinder is connected
	__IO uint16_t coilpin;				// Pin of Port of where this Coil is connected
	float fireangle;					// Angle at which the Cylinder shall enter fire mode
	float dwellangle;					// Angle at which the Cylinder shall enter dwell mode
	int firetooth;						// Tooth number at which Cylinder shall begin OPM extrapolation for Firing
	int dwelltooth;						// Tooth number at which Cylinder shall begin OPM extrapolation for Dwelling
	uint32_t time2fire;					// Time used for extrapolation using OPM to trigger start the ignition in advance
	uint32_t time2dwell;				// Time used for extrapolation using OPM to trigger start dwelling at some time in advance
	int fireCamCycle;					// Identifier: If 0, Fire occurs in the first 360 degrees. If 1, Fire occurs in the 360-720 degrees.
	int dwellCamCycle;					// Identifier: If 0, Dwell occurs in the first 360 degrees. If 1, dwell occurs in the 360-720 degrees.
}Ignition_CoilCyl;

// GLOBALS
extern Ignition_CoilCyl Ignition_Cyl1;				// Global Variable for Cylinder 1, containing all Info regarding Ignition
extern Ignition_CoilCyl Ignition_Cyl2;				// Global Variable for Cylinder 2, containing all Info regarding Ignition
extern Ignition_CoilCyl Ignition_Cyl3;				// Global Variable for Cylinder 3, containing all Info regarding Ignition
extern Ignition_CoilCyl Ignition_Cyl4;				// Global Variable for Cylinder 4, containing all Info regarding Ignition

extern Ignition_TIM_Config Ignition_FireTIM;		// TIM Struct for Fire branch Ignition Timers
extern Ignition_TIM_Config Ignition_DwellTIM;		// TIM Struct for Dwell branch Ignition Timers

// LOCALS
int Ignition_DwellTimeUs_P;

// INITS
extern void Ignition_Init_FireTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel);
extern void Ignition_Init_DwellTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel);
extern void Ignition_Init_IOConfig(GPIO_TypeDef *coilport1,__IO uint16_t coilpin1,GPIO_TypeDef *coilport2,__IO uint16_t coilpin2,GPIO_TypeDef *coilport3,__IO uint16_t coilpin3,GPIO_TypeDef *coilport4,__IO uint16_t coilpin4);
extern void Ignition_Init_IgnitionCyl();

// Main LOOP Functions
extern void Ignition_GetInputs();
extern void Ignition_MainLoop();

// METHODS
// Dwell Functions
extern void Ignition_SetDwellTiming(Ignition_CoilCyl *Cylinder);					// Function used to set the Timing parameters for Dwell functionality for specific cylinder
extern void Ignition_GetDwellTime(float AppliedTorque);								// Function used to get&set the required Dwell Time based on Applied Torque (TODO more inputs?
// Fire Functions
extern void Ignition_SetIgnitionTiming(Ignition_CoilCyl *Cylinder);					// Function used to set the Timing parameters for Fire functionality for specific cylinder
extern void Ignition_CorrectIgnitionAngle(Ignition_CoilCyl *Cylinder,int AngleOffs);// Function used to set the offset of angle (early/late ignition) for specific cylinder

// DRIVER
extern void Ignition_DriverLoop(int Cam_HalfCycle,uint8_t Crank_TeethCount);					// Ignition Driver Main Loop
extern void Ignition_StartTimerDwellCylinder(Ignition_CoilCyl *Cylinder);			// API called by driver to start the timer and start to Dwell upon finished period
extern void Ignition_DwellStarted(Ignition_CoilCyl *Cylinder);						// API called by driver to set the state to Dwelling
extern void Ignition_StartTimerFireCylinder(Ignition_CoilCyl *Cylinder);			// API called by driver to start the timer and Fire upon finished period
extern void Ignition_FireStarted(Ignition_CoilCyl *Cylinder);						// API called by driver to set the state to Fired

// Supporting Functions
extern uint32_t Ignition_AngleToUs(int CrankShaftHz, float AngleDegree);

#endif /* INC_IGNITION_H_ */
