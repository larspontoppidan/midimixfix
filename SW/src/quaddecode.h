/*
 * quaddecode.h
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#ifndef QUADDECODE_H_
#define QUADDECODE_H_

#include "common.h"


void qd_initialize(void);

void qd_handleAchange_isr(bool_t b_value, bool_t pushed);
void qd_handleBchange_isr(bool_t a_value, bool_t pushed);

int8_t qd_getDelta(void);
int8_t qd_getPushedDelta(void);


#endif /* QUADDECODE_H_ */
