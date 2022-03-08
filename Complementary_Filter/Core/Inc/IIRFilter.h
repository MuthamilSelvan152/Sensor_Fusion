/*
 * IIRFilter.h
 *
 *  Created on: Mar 4, 2022
 *      Author: 123
 */

#ifndef INC_IIRFILTER_H_
#define INC_IIRFILTER_H_

#include <stdint.h>

typedef struct
{
	float alpha;
	float beta;
	float out;
}IIRFilter;

void IIRFilter_Init(IIRFilter * filt, float alpha, float beta);
float IIRFilter_Update(IIRFilter * filt, float input);

#endif /* INC_IIRFILTER_H_ */
