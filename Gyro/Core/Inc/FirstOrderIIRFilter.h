/*
 * FirstOrderIIRFilter.h
 *
 *  Created on: Mar 5, 2022
 *      Author: 123
 */

#ifndef INC_FIRSTORDERIIRFILTER_H_
#define INC_FIRSTORDERIIRFILTER_H_

typedef struct
{
	float alpha;
	float out;

}FirstOrderIIR;

void FirstOrderIIR_Init(FirstOrderIIR * filt, float alpha);
float FirstOrderIIR_Update(FirstOrderIIR * filt, float in);

#endif /* INC_FIRSTORDERIIRFILTER_H_ */
