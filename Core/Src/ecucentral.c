/*
 * ecucentral.c
 *
 *  Created on: Dec 3, 2020
 *      Author: aljaz
 */


#include <ecucentral.h>
#include <systemconfig.h>


ECU_Info ECU;
ECU_Modes ECU_Mode = off;

void ECU_MainLoop(){
	// get all ADCs
	// get all CAN messages
	// get all SPIs (if needed)
	// run the loop

	switch (ECU_Mode){
	case off:
		ECU_Mode = initializing;
		break;
	case initializing:
		// RUN initialization sequence, whatever is needed
		ECU_InitLoop();
		// Transit to cranking only if error free and upon starter signal TODO make starter signal on Teensy

		// Transit to postdrive if contact taken
		// if ECU.KL15 == 0, ECU_StMachine.mode = postdrive
		break;
	case cranking:
		// here the cranking procedure comes, if needed
		ECU_CrankingLoop();
		// Transit to running when speed is above threshold (lets say 1000 RPM)
		// if speed >= 1000, ECU_StMachine.mode = running
		break;
	case running:
		// here will be the running loop, 99% of things happen here
		ECU_RunLoop();
		// Transit to postdrive if contact taken
		// if ECU.KL15 == 0, ECU_StMachine.mode = postdrive
		break;
	case postdrive:
		// here is post drive, like ventilators, pumps, lights and stuff
		// save to diagnostics to NVM if possible TODO
		ECU_PostDriveLoop();
		break;
	}
}

void ECU_CheckTransitConditions(){
	// here all transition conditions are checked. For example engine speed etc.
}

void ECU_InitLoop(){
	// Init sequence here
}
void ECU_CrankingLoop(){
	// Cranking sequence here
}
void ECU_RunLoop(){
	// Running sequence here
}
void ECU_PostDriveLoop(){
	// Post Drive sequence here
}

