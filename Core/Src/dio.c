/*
 * dio.c
 *
 *  Created on: Dec 11, 2020
 *      Author: aljaz
 */


#include "dio.h"



uint8_t ECU_Term15;
uint8_t ECU_Ignition;

void DIO_getAllPorts(){
	// TODO in future if we want to sample all the ports
	ECU_Term15 = HAL_GPIO_ReadPin(ECU_Term15_GPIO_Port,ECU_Term15_Pin);
	ECU_Ignition = HAL_GPIO_ReadPin(ECU_Term15_GPIO_Port,ECU_Ignition_Pin);
	__IO uint32_t a = GPIOE->IDR;
}

uint8_t PORTE_getVal(PORTE_Pinout PORTE_Pin){return 0;}
