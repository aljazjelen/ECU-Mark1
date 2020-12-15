/*
 * ecucentral.h
 *
 *  Created on: Dec 3, 2020
 *      Author: aljaz
 */

#ifndef INC_ECUCENTRAL_H_
#define INC_ECUCENTRAL_H_

typedef enum{
	sleep = 0,
	initialize = 1,
	idle = 2,
	cranking = 3,
	running = 4,
	postdrive = 5
}ECU_Modes;

typedef struct{
	ECU_Modes mode;
}ECU_Info;

extern ECU_Modes ECU_Mode;
extern ECU_Info ECU;

extern void ECU_MainLoop();
extern void ECU_InitLoop();

extern void ECU_CommonFunctions();
extern void ECU_CrankingLoop();
extern void ECU_RunLoop();
extern void ECU_PostDriveLoop();
extern void ECU_CheckTransitConditions();

#endif /* INC_ECUCENTRAL_H_ */
