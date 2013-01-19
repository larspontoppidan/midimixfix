/*
 * midiparser.h
 *
 *  Created on: Dec 15, 2012
 *      Author: lars
 */

#ifndef MIDIPARSER_H_
#define MIDIPARSER_H_

#include "common.h"

// Midi parser module receives raw data bytes from midi inputs and implements
// a state machine for parsing the data.

// The parsing results in calls to midiio message creation api


void mparser_initialize(void);

void mparser_handleInput1Rx_Isr(uint8_t x);
void mparser_handleInput2Rx_Isr(uint8_t x);





#endif /* MIDIPARSER_H_ */
