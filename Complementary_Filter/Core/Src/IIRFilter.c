/*
 * IIRFilter.c
 *
 *  Created on: Mar 4, 2022
 *      Author: 123
 */

#include "IIRFilter.h"

void IIRFilter_Init(IIRFilter * filt, float alpha, float beta)
{
	filt->alpha = alpha;
	filt->beta = beta;
	filt->out = 0;
}

float IIRFilter_Update(IIRFilter * filt, float input)
{
	/* y[n] = alpha*x[n] - beta*y[n-1] */
	return filt->out = filt->alpha*input - filt->beta * filt->out;
}
