/*
 * mididisplay.h
 *
 *  Created on: Dec 14, 2012
 *      Author: lars
 */

#ifndef MIDILOGGER_H_
#define MIDILOGGER_H_

#include "common.h"
#include "midimessage.h"

void MidiLog_initialize(void);

void MidiLog_handleMidiMsgIn_ISR(midiMsg_t *msg);
void MidiLog_handleMidiMsgOut_ISR(midiMsg_t *msg);
void MidiLog_handleTick_ISR(void);
void MidiLog_handleMainLoop(void);

uint8_t MidiLog_menuGetSubCount(void);
void    MidiLog_menuGetText(char *dest, uint8_t item);
uint8_t MidiLog_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


#endif /* MIDILOGGER_H_ */
