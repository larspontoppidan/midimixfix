/*
 * atsmoother.h
 *
 *  Created on: May 6, 2014
 *      Author: lars
 */

#ifndef ATSMOOTHER_H_
#define ATSMOOTHER_H_



#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

extern const filterInterface_t PROGMEM atsmoother_Filter;


void atsmoother_handleTick_ISR(void);

#endif /* ATSMOOTHER_H_ */
