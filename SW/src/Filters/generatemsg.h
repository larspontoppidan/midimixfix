/*
 * generatemsg.h
 *
 *  Created on: Jan 17, 2013
 *      Author: lars
 */

#ifndef GENERATEMSG_H_
#define GENERATEMSG_H_


#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

void genmsg_initialize(void);

extern const filterInterface_t PROGMEM genmsg_Filter;

#endif /* GENERATEMSG_H_ */
