/*
 * resetter.h
 *
 *  Created on: May 7, 2014
 *      Author: lars
 */

#ifndef RESETTER_H_
#define RESETTER_H_


#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

extern const filterInterface_t PROGMEM resetter_Filter;


void resetter_handleTick_ISR(void);


#endif /* RESETTER_H_ */
