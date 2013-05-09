/*
 * quaddecode.h
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#ifndef QUADDECODE_H_
#define QUADDECODE_H_

#include "common.h"


void QuadDecode_initialize(void);

void QuadDecode_handleAChange_ISR(bool_t b_value, bool_t pushed);
void QuadDecode_handleBChange_ISR(bool_t a_value, bool_t pushed);

int8_t QuadDecode_getDelta_MAIN(void);
int8_t QuadDecode_getPushedDelta_MAIN(void);


#endif /* QUADDECODE_H_ */
