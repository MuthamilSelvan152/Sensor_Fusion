/*
 * FirstOrderIIRFilter.c
 *
 *  Created on: Mar 5, 2022
 *      Author: 123
 */
#include "FirstOrderIIRFilter.h"

void FirstOrderIIR_Init(FirstOrderIIR * filt, float alpha)
{
	if(alpha < 0.0f)
	{
		filt->alpha = 0.0f;
	}
	else if(alpha > 1.0f)
	{
		filt->alpha = 1.0f;
	}
	else
	{
		filt->alpha = alpha;
	}

	filt->out = 0;
}


float FirstOrderIIR_Update(FirstOrderIIR * filt, float in)
{
	/* Update the output */
	return filt->out = (1.0f - filt->alpha) * in + filt->alpha * filt->out;
}
