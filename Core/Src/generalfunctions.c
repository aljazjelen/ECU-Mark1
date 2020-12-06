/*
 * generalfunctions.c
 *
 *  Created on: Dec 1, 2020
 *      Author: aljaz
 */


#include "generalfunctions.h"


float Common_GetFrom2Dmap(float x, float y, int sizex, int sizey, float *arrayX, float *arrayY, float mapZ[][sizey]){
	float val = 0;

	int xLowInd = 0;
	int xHighInd = 0;
	int yLowInd = 0;
	int yHighInd = 0;

	float xFractMin;
	float xFractMax;
	float yFractMin;
	float yFractMax;

	float xLimited;
	float yLimited;

	if (x < arrayX[0]){
		xLowInd = 0;
		xHighInd = 1;
		xLimited = arrayX[0];
	}else if (x >= arrayX[sizex-1]){
		xLowInd = sizex-2;
		xHighInd = sizex-1;
		xLimited = arrayX[sizex-1];
	}
	else{
		xLimited = x;
		for (int i = 0; i < sizex; i++){
			if (x > arrayX[i])
				xLowInd = i;
		}
		xHighInd = xLowInd+1;
	}
	xFractMin = arrayX[xLowInd];
	xFractMax = arrayX[xHighInd];


	if (y < arrayY[0]){
		yLowInd = 0;
		yHighInd = 1;
		yLimited = arrayY[0];
	}else if (y >= arrayY[sizey-1]){
		yLowInd = sizey-2;
		yHighInd = sizey-1;
		yLimited = arrayY[sizey-1];
	}
	else{
		yLimited = y;
		for (int j = 0; j < sizex; j++){
			if (y > arrayY[j])
				yLowInd = j;
		}
		yHighInd = yLowInd+1;
	}
	yFractMin = arrayY[yLowInd];
	yFractMax = arrayY[yHighInd];


	float xFract = (xLimited-xFractMin)/(xFractMax-xFractMin);
	float yFract = (yLimited-yFractMin)/(yFractMax-yFractMin);


	float a = mapZ[xLowInd][yLowInd];
	float b = mapZ[xLowInd][yHighInd];
	float c = mapZ[xHighInd][yLowInd];
	float d = mapZ[xHighInd][yHighInd];

	val = a*(1-xFract)*(1-yFract) + b*(1-xFract)*(yFract) + c*(xFract)*(1-yFract) + d*(xFract)*(yFract);

	return val;
}
