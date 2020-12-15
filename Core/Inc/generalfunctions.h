/*
 * generalfunctions.h
 *
 *  Created on: Dec 1, 2020
 *      Author: aljaz
 */

#ifndef INC_GENERALFUNCTIONS_H_
#define INC_GENERALFUNCTIONS_H_




// Function to get the interpolated value from 2D map, using x and y as Inputs
extern float Common_GetFrom2Dmap(float x, float y, int sizex, int sizey, float *arrayX, float *arrayY, float mapZ[][sizey-1]);

// Function to get the interpolated value from 1D map, using x as Input TODO do the test if it works! might be 1-fract in b and fract in a!
extern float Common_GetFrom1Dmap(float x, int sizex, float *arrayX);


#endif /* INC_GENERALFUNCTIONS_H_ */
