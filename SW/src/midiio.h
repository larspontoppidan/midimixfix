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
uint8_t midiio_MsgNew(uint8_t flags, uint8_t midi_status);

// Add data to message
void midiio_MsgAddData(uint8_t msg_index, uint8_t midi_data);

// Indicate message is complete, and should be processed
void midiio_MsgFinish(uint8_t msg_index, uint8_t flags);

// Add realtime message (F8 <= midi_status <= FF)
void midiio_RealtimeMsg(uint8_t flags, uint8_t midi_status);

// Called by hal module at tx complete:
void midiio_OutputTxCompleteIsr(void);


// IO setup

#define MIDIIO_MODE_DISCARD 0u
#define MIDIIO_MODE_THROUGH 1u
#define MIDIIO_MODE_PROCESS 2u

void    midiio_SetMode(uint8_t mode, uint8_t source, bool_t rt);
uint8_t midiio_GetMode(uint8_t source, bool_t rt);

void midiio_SetSendRunStatus(bool_t x);
bool_t midiio_GetSendRunStatus(void);


#endif /* MIDIIO_H_ */
