/*
 * polysplit.h
 *
 *  Created on: Jul 14, 2013
 *      Author: lars
 */

#ifndef POLYSPLIT_H_
#define POLYSPLIT_H_


#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

extern const filterInterface_t PROGMEM polysplit_Filter;


void polysplit_handleTick_ISR(void);

#endif /* POLYSPLIT_H_ */
