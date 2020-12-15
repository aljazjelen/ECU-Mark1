/*
 * dio.h
 *
 *  Created on: Dec 11, 2020
 *      Author: aljaz
 */

#ifndef INC_DIO_H_
#define INC_DIO_H_

#include "main.h"
#include "stm32f4xx_hal.h"


// ENUMS
typedef enum{
	PORTE_null0,
	PORTE_null1,
	PORTE_null2,
	PORTE_null3,
	PORTE_null4,
	PORTE_null5,
	PORTE_null6,
	PORTE_null7,
	PORTE_null8,
	PORTE_null9,
	PORTE_Term15,
	PORTE_null11,
	PORTE_Ignition
}PORTE_Pinout;

extern PORTE_Pinout a;

extern uint8_t ECU_Term15;
extern uint8_t ECU_Ignition;

extern void DIO_getAllPorts();
extern uint8_t PORTE_getVal(PORTE_Pinout PORTE_Pin);
#endif /* INC_DIO_H_ */
