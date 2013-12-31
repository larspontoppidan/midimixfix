/*
 * quaddecode.h
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#ifndef QUADDECODE_H_
#define QUADDECODE_H_

#include "common.h"


void quaddecode_initialize(void);

void quaddecode_handleAChange_ISR(bool_t b_value, bool_t pushed);
void quaddecode_handleBChange_ISR(bool_t a_value, bool_t pushed);

int8_t quaddecode_getDelta_MAIN(void);
int8_t quaddecode_getPushedDelta_MAIN(void);

void quaddecode_handleTickIsr_ISR(void);

#endif /* QUADDECODE_H_ */
