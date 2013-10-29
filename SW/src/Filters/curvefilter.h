/*
 * curvefilter.h
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */

#ifndef CURVEFILTER_H_
#define CURVEFILTER_H_


#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>


void curveflt_initialize(void);
void curveflt_handleMainLoop(void);

extern const filterInterface_t PROGMEM curvefilter_Filter;



#endif /* CURVEFILTER_H_ */
