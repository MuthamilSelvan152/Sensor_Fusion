/*
 * RCFilter.h
 *
 *  Created on: Mar 4, 2022
 *      Author: 123
 */

#ifndef INC_RCFILTER_H_
#define INC_RCFILTER_H_

typedef struct {
	float coeff[2];
	float out[2];
}RCFilter;

#define PI	(22/7)

void RCFilter_Init(RCFilter * Filt,float cutoff_Freq, float sampleTime);

float RCFilter_Update(RCFilter * Filt, float input);



#endif /* INC_RCFILTER_H_ */
