/*
 * midiout.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef MIDIOUT_H_
#define MIDIOUT_H_

#include "../common.h"
#include "../filterinterface.h"
#include "../midimessage.h"
#include <avr/pgmspace.h>

void midiout_initialize(void);

extern const filterInterface_t PROGMEM midiout_Filter;


#endif /* MIDIOUT_H_ */
