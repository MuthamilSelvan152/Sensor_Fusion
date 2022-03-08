/*
 * RCFilter.c
 *
 *  Created on: Mar 4, 2022
 *      Author: 123
 */
#include "RCFilter.h"

void RCFilter_Init(RCFilter * Filt,float cutoff_Freq, float sampleTime)
{

	/* Compute RC for cutOff_Freq */
	float RC = 1.0f / (2*PI*cutoff_Freq);

	/* Compute first order LPF coefficients */
	Filt->coeff[0] = sampleTime / (sampleTime + RC);
	Filt->coeff[1] = RC / (sampleTime + RC);

	/* Clear output buffer */
	Filt->out[0] = 0.0f;
	Filt->out[1] = 0.0f;
}

float RCFilter_Update(RCFilter * Filt, float input)
{
	/* Shift output sample */
	Filt->out[1] = Filt->out[0];

	/* Compute new output sample */
	return Filt->out[0] = Filt->coeff[0] * input + Filt->coeff[1] * Filt->out[1];

}


