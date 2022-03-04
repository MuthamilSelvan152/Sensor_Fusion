/*
 * FIRFilter.c
 *
 *  Created on: Mar 4, 2022
 *      Author: 123
 */

#include "FIRFilter.h"

static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {-0.0032906f, -0.0052635f, -0.0068811f, 0.0000000f,0.0254209f,0.0724719f,0.1311260f,0.1805961f,0.2000000f,0.1805961f,0.1311260f,0.0724719f,0.0254209f,0.0000000f,-0.00688114, -0.0052635f};

void FIRFilter_Init(FIRFilter * fir)
{
	/* Clear the buffer */
	for(uint8_t n = 0; n < FIR_FILTER_LENGTH; n++ )
	{
		fir->buf[n] = 0.0f;
	}

	/* Reset Buffer index */
	fir->bufIndex = 0;

	/* Clear the output */
	fir->out = 0.0f;
}

float FIRFilter_Update(FIRFilter * fir, float input)
{

	/* store the latest input in the buffer */
	fir->buf[fir->bufIndex] = input;

	/* Increment the index and wrap around if necessary */
	fir->bufIndex++;

	if(fir->bufIndex == FIR_FILTER_LENGTH)
	{
		fir->bufIndex = 0;
	}

	/* Compute the new output sample (via convolution) */
	fir->out = 0.0f;

	uint8_t sumIndex = fir->bufIndex;

	for(uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
	{
		/* Decrement the index and wrap if necessary */
		if(sumIndex > 0)
		{
			sumIndex--;
		}
		else
		{
			sumIndex = FIR_FILTER_LENGTH - 1;
		}

		/* Multiply impulse response with shifted input sample and add to output */
		fir->out += FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
	}

	/* Return filtered output */
	return fir->out;
}


