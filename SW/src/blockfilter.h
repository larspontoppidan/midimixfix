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

void    BlockFlt_initialize(void);

void    BlockFlt_hookMidiMsg_ISR(midiMsg_t *msg);
void    BlockFlt_hookTick_ISR(void);
void    BlockFlt_hookMainLoop(void);

uint8_t BlockFlt_menuGetSubCount(void);
void    BlockFlt_menuGetText(char *dest, uint8_t item);
uint8_t BlockFlt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t BlockFlt_configGetSize(void);
void    BlockFlt_configSave(uint8_t *dest);
void    BlockFlt_configLoad(uint8_t *dest);

#endif /* BLOCKFILTER_H_ */
