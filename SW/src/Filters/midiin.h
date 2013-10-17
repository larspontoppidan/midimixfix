/*
 * midiinput.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef MIDIINPUT_H_
#define MIDIINPUT_H_

#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

// Midi input module parses the raw data bytes from midi inputs into midi messages
// The messages are inserted into the processing chain as specified by filter setups

// This module provides as many as six individual filter types, each only available in one instance:
//
// Midi IN1
// Midi IN2
// Midi IN1 REALT
// Midi IN2 REALT
// Midi IN1 SYSEX
// Midi IN2 SYSEX


// The parsing results in calls to midiio message creation api


void midiin_initialize(void);

void midiin_handleInput1_ISR(uint8_t x);
void midiin_handleInput2_ISR(uint8_t x);

bool_t midiin_getIsReceivingStatus(void);

extern const filterInterface_t PROGMEM midiin_In1;
extern const filterInterface_t PROGMEM midiin_In2;
extern const filterInterface_t PROGMEM midiin_In1Realtime;
extern const filterInterface_t PROGMEM midiin_In2Realtime;
extern const filterInterface_t PROGMEM midiin_In1Sysex;
extern const filterInterface_t PROGMEM midiin_In2Sysex;



#endif /* MIDIINPUT_H_ */

