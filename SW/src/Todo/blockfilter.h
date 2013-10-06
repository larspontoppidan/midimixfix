/*
 * blockfilter.h
 *
 *  Created on: Dec 22, 2012
 *      Author: lars
 */

#ifndef BLOCKFILTER_H_
#define BLOCKFILTER_H_

#include "common.h"
#include "midimessage.h"

void    blockflt_initialize(void);

void    blockflt_handleMidiMsg_ISR(midiMsg_t *msg);
void    blockflt_handleTick_ISR(void);
void    blockflt_handleMainLoop(void);

uint8_t blockflt_menuGetSubCount(void);
void    blockflt_menuGetText(char *dest, uint8_t item);
uint8_t blockflt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t blockflt_configGetSize(void);
void    blockflt_configSave(uint8_t *dest);
void    blockflt_configLoad(uint8_t *dest);

#endif /* BLOCKFILTER_H_ */
