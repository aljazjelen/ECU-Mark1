/*
 * injection.c
 *
 *  Created on: Dec 1, 2020
 *      Author: aljaz
 */

#include "injection.h"
#include "generalfunctions.h"


// GLOBALS
Injection_InjectorCyl Injection_Cyl1;	// Global Variable for Cylinder 1, containing all Info regarding Injection
Injection_InjectorCyl Injection_Cyl2;	// Global Variable for Cylinder 2, containing all Info regarding Injection
Injection_InjectorCyl Injection_Cyl3;	// Global Variable for Cylinder 3, containing all Info regarding Injection
Injection_InjectorCyl Injection_Cyl4;	// Global Variable for Cylinder 4, containing all Info regarding Injection

Injection_TIM_Config Injection_InjectorTIM;	// TIM Struct for Injection branch Timers


int xx = 4;
int yy = 5;
float ax[4] = {0,1,2,3};
float ay[5] = {100,200,300,400,500};
float map[4][5]= {{0,1,2,3,4},{0,10,20,30,40},{0,100,200,300,400},{0,1000,2000,3000,4000}};


void Injection_Init_InjectorTIM(TIM_HandleTypeDef *Handler, uint8_t Channel, uint8_t ActiveChannel){
	Injection_InjectorTIM.Handler = Handler;
	Injection_InjectorTIM.Channel = Channel;
	Injection_InjectorTIM.ActiveChannel = ActiveChannel;
	HAL_TIM_Base_Start_IT(Handler);
	HAL_TIM_OC_Start_IT(Handler, Channel);
}

void Injection_Init_IOConfig(GPIO_TypeDef *port1,__IO uint16_t pin1,GPIO_TypeDef *port2,__IO uint16_t pin2,GPIO_TypeDef *port3,__IO uint16_t pin3,GPIO_TypeDef *port4,__IO uint16_t pin4){
	Injection_Cyl1.injectorport = port1;
	Injection_Cyl1.injectorpin = pin1;
	Injection_Cyl2.injectorport = port2;
	Injection_Cyl2.injectorpin = pin2;
	Injection_Cyl3.injectorport = port3;
	Injection_Cyl3.injectorpin = pin3;
	Injection_Cyl4.injectorport = port4;
	Injection_Cyl4.injectorpin = pin4;
}

void Injection_Init_InjectionCyl(){
	// Cylinder 1
	Injection_Cyl1.cylinderID = 1;
	Injection_Cyl1.injectorstate = IDLE_INJECT;
	Injection_Cyl1.injectionangle = 30;
	Injection_Cyl1.injectiontooth = 2;
	Injection_Cyl1.time2injection = 10;
	Injection_Cyl1.injectCamCycle = 1;
	Injection_Cyl1.injectiontimeus = 1000;
	// Cylinder 2
	Injection_Cyl2.cylinderID = 2;
	Injection_Cyl2.injectorstate = IDLE_INJECT;
	Injection_Cyl2.injectionangle = 110;
	Injection_Cyl2.injectiontooth = 2;
	Injection_Cyl2.time2injection = 10;
	Injection_Cyl2.injectCamCycle = 0;	// DO NOT CHANGE!
	Injection_Cyl2.injectiontimeus = 1000;
	// Cylinder 3
	Injection_Cyl3.cylinderID = 3;
	Injection_Cyl3.injectorstate = IDLE_INJECT;
	Injection_Cyl3.injectionangle = 200;
	Injection_Cyl3.injectiontooth = 10;
	Injection_Cyl3.time2injection = 10;
	Injection_Cyl3.injectCamCycle = 1;
	Injection_Cyl3.injectiontimeus = 1000;
	// Cylinder 4
	Injection_Cyl4.cylinderID = 4;
	Injection_Cyl4.injectorstate = IDLE_INJECT;
	Injection_Cyl4.injectionangle = 280;
	Injection_Cyl4.injectiontooth = 10;
	Injection_Cyl4.time2injection = 10;
	Injection_Cyl4.injectCamCycle = 0;	// DO NOT CHANGE!
	Injection_Cyl4.injectiontimeus = 1000;
}


// METHODS
void Injection_CalcFuelQty(){
	float fuel = 0;
	fuel = Common_GetFrom2Dmap(6,700,xx,yy,ax,ay,map);

}

// INECTOR DRIVER FUNCTIONS ========================================
void Injection_StartTimerInjectCylinder(Injection_InjectorCyl *Cylinder){
	Cylinder->injectorstate = TRIG_INJECT;
	//uint32_t injectionStartInUs = Cylinder->time2injection;
	uint32_t injectForUs = 1000;
	HAL_GPIO_WritePin(Cylinder->injectorport,Cylinder->injectorpin,GPIO_PIN_SET);
	// START IGNITION TIM OUTPUT
	Injection_InjectorTIM.Handler->Instance->ARR = injectForUs + 10;
	Injection_InjectorTIM.Handler->Instance->CCR1 = injectForUs;
	Injection_InjectorTIM.Handler->Instance->EGR = 1;
	Injection_InjectorTIM.Handler->Instance->CR1 |= TIM_CR1_CEN;
}


