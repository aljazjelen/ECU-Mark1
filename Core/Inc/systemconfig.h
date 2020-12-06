/*
 * engineconfig.h
 *
 *  Created on: Dec 3, 2020
 *      Author: aljaz
 */

#ifndef INC_SYSTEMCONFIG_H_
#define INC_SYSTEMCONFIG_H_


typedef struct{
	int Crank_TeethNmbr_P;
	int Crank_MissingTeethNmbr_P;
}Crankshaft_Configuration;

extern Crankshaft_Configuration CrankCfg;


#endif /* INC_SYSTEMCONFIG_H_ */
