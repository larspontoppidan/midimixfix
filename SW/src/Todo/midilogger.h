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

void midilog_initialize(void);

void midilog_handleMidiMsgIn_ISR(midiMsg_t *msg);
void midilog_handleMidiMsgOut_ISR(midiMsg_t *msg);
void midilog_handleTick_ISR(void);
void midilog_handleMainLoop(void);

uint8_t midilog_menuGetSubCount(void);
void    midilog_menuGetText(char *dest, uint8_t item);
uint8_t midilog_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


#endif /* MIDILOGGER_H_ */
