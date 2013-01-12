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

void mlog_InMessageIsrHook(mmsg_t *msg);
void mlog_OutMessageIsrHook(mmsg_t *msg);
void mlog_TickIsrHook(void);
void mlog_MainLoopHook(void);

uint8_t mlog_GetSubMenuCount(void);
void    mlog_GetMenuText(char *dest, uint8_t item);
uint8_t mlog_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


#endif /* MIDILOGGER_H_ */
