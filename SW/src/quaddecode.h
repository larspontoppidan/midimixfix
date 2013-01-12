/*
 * quaddecode.h
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#ifndef QUADDECODE_H_
#define QUADDECODE_H_

#include "common.h"


void qd_Initialize(void);

void qd_AChangeIsr(bool_t b_value, bool_t pushed);

void qd_BChangeIsr(bool_t a_value, bool_t pushed);

int8_t qd_GetDelta(void);
int8_t qd_GetPushedDelta(void);


#endif /* QUADDECODE_H_ */
