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

void mlog_Initialize(void);

void mlog_handleMidiMsgIn_ISR(midiMsg_t *msg);
void mlog_handleMidiMsgOut_ISR(midiMsg_t *msg);
void mlog_handleTick_ISR(void);
void mlog_handleMainLoop(void);

uint8_t mlog_menuGetSubCount(void);
void    mlog_menuGetText(char *dest, uint8_t item);
uint8_t mlog_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


#endif /* MIDILOGGER_H_ */
