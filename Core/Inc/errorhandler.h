/*
 * errorhandler.h
 *
 *  Created on: Nov 22, 2020
 *      Author: aljaz
 */

#ifndef INC_ERRORHANDLER_H_
#define INC_ERRORHANDLER_H_

/*
Collection of all Errors to be tracked. Simply add one when needed
Use Public functions (ErrorHandler_Increase/Decrease), the rest is taken care here
*/
enum ErrorHandler_UniqueIDCollectibles {
    Crank_ToothJump = 0,
    Crank_AnotherFailure1 = 1,
	Crank_AnotherFailure2 = 2
};


typedef struct{
	int uniqueID;
	char description;
	int counter;
	int maxcounter;
	int faultflag;
}ErrorHandler;

// Memory of all faults
extern ErrorHandler ErrorMemory[10];

extern void ErrorHandler_Init();					// Init function which takes care of initializing ErrorHandler memory
void ErrorHandler_AlarmLight();						// Private function to update upon failure flags.
void ErrorHandler_Update(int errorID, int val);		// Private function to update upon failure flags. Called upon no fault or fault.

extern void ErrorHandler_Increase(int errorID);		// Bouncing strategy. Called in case of failure event, increase by 2 (default)
extern void ErrorHandler_Decrease(int errorID);		// Bouncing strategy. Called in case of non-failure event, decrease by 1 (default)

#endif /* INC_ERRORHANDLER_H_ */
