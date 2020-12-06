/*
 * errorhandler.c
 *
 *  Created on: Nov 22, 2020
 *      Author: aljaz
 */

#include "errorhandler.h"

ErrorHandler ErrorMemory[10];		// ErrorMemory with predefined allocate size (default 10). Expendable upon expending enum ErrorHandler_UniqueIDCollectibles

/*
Collection of all Errors to be tracked. Simply add one when needed
Use Public functions (ErrorHandler_Increase/Decrease), the rest is taken care here
*/
void ErrorHandler_Init(){
	// Initialize the Error Handler Memory
	for (int i = 0;i<10;i++){
		ErrorMemory[i].counter = 0;
		ErrorMemory[i].maxcounter = 100;
		ErrorMemory[i].faultflag = 0;
		ErrorMemory[i].uniqueID = i;
		ErrorMemory[i].description = "Abc";
	}
}

/* Init function which takes care of initializing ErrorHandler memory */
void ErrorHandler_Update(int errorID, int val){
	// This Error Handler function updates the given Error Handler under ID
	int counterValue = ErrorMemory[errorID].counter;
	int maxCounterValue = ErrorMemory[errorID].maxcounter;
	counterValue = counterValue + val;
	if (counterValue > maxCounterValue)
		ErrorMemory[errorID].faultflag = 1;
	else if (counterValue <= 0)
		ErrorMemory[errorID].counter = 0;
	else
		ErrorMemory[errorID].counter = counterValue;
}

/* Bouncing strategy. Called in case of failure event, increase by 2 (default) */
void ErrorHandler_Increase(int errorID){ErrorHandler_Update(errorID,2);}

/* Bouncing strategy. Called in case of non-failure event, decrease by 1 (default) */
void ErrorHandler_Decrease(int errorID){ErrorHandler_Update(errorID,-1);}
