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


void    midiio_Initialize(void);


// Allocate new message, returns msg_index
uint8_t midiio_MsgNew_ISR(uint8_t flags, uint8_t midi_status);
uint8_t midiio_MsgNew_Main(uint8_t flags, uint8_t midi_status);

// Add data to message
void midiio_MsgAddData_ISR(uint8_t msg_index, uint8_t midi_data);

// Indicate message is complete, and should be processed
void midiio_MsgFinish_ISR(uint8_t msg_index, uint8_t flags);

// Add realtime message (F8 <= midi_status <= FF)
void midiio_RealtimeMsgAdd_ISR(uint8_t flags, uint8_t midi_status);

// Called by hal module at tx complete:
void midiio_OutputTxComplete_ISR(void);


// IO setup

#define MIDIIO_MODE_DISCARD 0u
#define MIDIIO_MODE_THROUGH 1u
#define MIDIIO_MODE_PROCESS 2u

void    midiio_ModeSet(uint8_t mode, uint8_t source, bool_t rt);
uint8_t midiio_ModeGet(uint8_t source, bool_t rt);

void    midiio_SendRunStatusSet(bool_t x);
bool_t  midiio_SendRunStatusGet(void);


#endif /* MIDIIO_H_ */
