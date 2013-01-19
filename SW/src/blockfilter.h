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

void    blockf_Initialize(void);

void    blockf_HookMidiMsg_ISR(mmsg_t *msg);
void    blockf_HookTick_ISR(void);
void    blockf_HookMainLoop(void);

uint8_t blockf_MenuGetSubCount(void);
void    blockf_MenuGetText(char *dest, uint8_t item);
uint8_t blockf_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t blockf_ConfigGetSize(void);
void    blockf_ConfigSave(uint8_t *dest);
void    blockf_ConfigLoad(uint8_t *dest);

#endif /* BLOCKFILTER_H_ */
