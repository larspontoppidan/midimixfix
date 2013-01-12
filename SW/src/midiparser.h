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


void mparse_Initialize(void);

void mparse_Input1Rx(uint8_t x);
void mparse_Input2Rx(uint8_t x);





#endif /* MIDIPARSER_H_ */
