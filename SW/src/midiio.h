/*
 * midiio.h
 *
 *  Created on: Dec 13, 2012
 *      Author: lars
 */

#ifndef MIDIIO_H_
#define MIDIIO_H_

#include "common.h"
#include "midimessage.h"

// Midi IO module receives parsed midi data and takes care of:
//  - logging
//  - processing
//  - transmission of output, if required


void    midiio_initialize(void);


// Allocate new message, returns msg_index
uint8_t midiio_msgNew_ISR(uint8_t flags, uint8_t midi_status);
uint8_t midiio_msgNew_MAIN(uint8_t flags, uint8_t midi_status);

// Add data to message
void midiio_msgAddData_ISR(uint8_t msg_index, uint8_t midi_data);

// Direct access to a message
midiMsg_t* midiio_msgGetPtr(uint8_t msg_index);

// Indicate message is complete, and should be processed
void midiio_msgFinish_ISR(uint8_t msg_index, uint8_t flags);

// Add realtime message (F8 <= midi_status <= FF)
void midiio_realtimeMsg_ISR(uint8_t flags, uint8_t midi_status);

// Called by hal module at tx complete:
void midiio_outputTxComplete_ISR(void);


// IO setup

#define MIDIIO_MODE_DISCARD 0u
#define MIDIIO_MODE_THROUGH 1u
#define MIDIIO_MODE_PROCESS 2u

void    midiio_setMode(uint8_t mode, uint8_t source, bool_t rt);
uint8_t midiio_getMode(uint8_t source, bool_t rt);

void    midiio_setRunStatusMode(bool_t x);
bool_t  midiio_getRunStatusMode(void);


#endif /* MIDIIO_H_ */
