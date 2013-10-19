/*
 * pedals.h
 *
 *  Created on: Oct 18, 2013
 *      Author: lars
 */

#ifndef PEDALS_H_
#define PEDALS_H_


#include "../common.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

// This module provides the pedal inputs as two filter types
//
// Pedal1
// Pedal2

// The user can setup:
//
// Pedal 1 (127)
//  Ctrl : SustainP.
//  Offs : 100
//  Gain : 40     / Digital


// The parsing results in calls to midiio message creation api


void pedals_initialize(void);
void pedals_handleTick_ISR(void);
void pedals_handleMainLoop(void);

extern const filterInterface_t PROGMEM pedals_In1;
extern const filterInterface_t PROGMEM pedals_In2;


#endif /* PEDALS_H_ */
