/*
 * midilog.h
 *
 *  Created on: Dec 14, 2012
 *      Author: lars
 */

#ifndef MIDILOG_H_
#define MIDILOG_H_

#include "common.h"
#include "midimessage.h"
#include "ui.h"

void midilog_initialize(void);

// Getting midilog to do anything:
void midilog_configure(uint8_t source, bool_t filtered);

#define MIDILOG_SOURCE_IN1  1
#define MIDILOG_SOURCE_IN2  2
#define MIDILOG_SOURCE_OUT  3

void midilog_stop(void);
void midilog_handleUi(uint8_t uiEvent);

// These need to be hooked up in the system
void midilog_handleMidiIn1RtSysx_ISR(midiMsg_t *msg);
void midilog_handleMidiIn1_ISR(midiMsg_t *msg);
void midilog_handleMidiIn2RtSysx_ISR(midiMsg_t *msg);
void midilog_handleMidiIn2_ISR(midiMsg_t *msg);

void midilog_handleMidiOut_ISR(midiMsg_t *msg);
void midilog_handleMidiOutRtSysx_ISR(midiMsg_t *msg);

void midilog_handleMainLoop(void);

// Note that the actual display writing happens out of main loop




#endif /* MIDILOG_H_ */
