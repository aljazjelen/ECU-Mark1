/*
 * injection.h
 *
 *  Created on: Dec 1, 2020
 *      Author: aljaz
 */
#include "stm32f4xx_hal.h"

#ifndef INC_INJECTION_H_
#define INC_INJECTION_H_

// ENUMS
typedef enum Injection_InjectorStates {
	IDLE_INJECT = 0,
    TRIG_INJECT = 1,
    INJECTED = 2
}InjectorStates;

// STRUCTS
typedef struct{
	TIM_HandleTypeDef *Handler;			// TIM Handler
	uint8_t Channel;					// TIM Config Channel
	uint8_t ActiveChannel;				// TIM Active Channel
}Injection_TIM_Config;

typedef struct{
	int cylinderID;						// ID of the Cylinder [1,2,3,4]
	InjectorStates injectorstate;		// Enum which tells the state will be upon the next injection tooth (used for pulse mode to optimize timing)
	GPIO_TypeDef *injectorport;			// Port of uC, where Injector of this cylinder is connected
	__IO uint16_t injectorpin;			// Pin of Port of where this Injector is connected
	float injectionangle;				// Angle at which the Injectors shall start with injection
	int injectiontooth;					// Tooth number at which Cylinder shall begin OPM extrapolation for Injection
	uint32_t injectiontimeus;			// Total time of open injectors in microseconds (us)
	uint32_t time2injection;			// Time used for extrapolation using OPM to trigger start the injection in advance
	int injectCamCycle;					// Identifier: If 0, Fire occurs in the first 360 degrees. If 1, Fire occurs in the 360-720 degrees.
}Injection_InjectorCyl;

// GLOBALS
extern Injection_InjectorCyl Injection_Cyl1;	// Global Variable for Cylinder 1, containing all Info regarding Injection
extern Injection_InjectorCyl Injection_Cyl2;	// Global Variable for Cylinder 2, containing all Info regarding Injection
extern Injection_InjectorCyl Injection_Cyl3;	// Global Variable for Cylinder 3, containing all Info regarding Injection
extern Injection_InjectorCyl Injection_Cyl4;	// Global Variable for Cylinder 4, containing all Info regarding Injection

extern Injection_TIM_Config Injection_InjectorTIM;	// TIM Struct for Injection branch Timers


// METHODS
void Injection_CalcFuelQty();

// INIT FUNCTIONS
extern void Injection_Init_InjectorTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel);
extern void Injection_Init_IOConfig(GPIO_TypeDef *port1,__IO uint16_t pin1,GPIO_TypeDef *port2,__IO uint16_t pin2,GPIO_TypeDef *port3,__IO uint16_t pin3,GPIO_TypeDef *port4,__IO uint16_t pin4);
extern void Injection_Init_InjectionCyl();

// INJECTOR DRIVER FUNCTIONS
extern void Injection_StartTimerInjectCylinder(Injection_InjectorCyl *Cylinder);


#endif /* INC_INJECTION_H_ */
