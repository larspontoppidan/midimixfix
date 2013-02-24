/*
 * superdelay.h
 *
 *  Created on: Feb 23, 2013
 *      Author: lars
 */

#ifndef SUPERDELAY_H_
#define SUPERDELAY_H_

#include "common.h"
#include "midimessage.h"

void    sdelay_Initialize(void);

void    sdelay_HookMidiMsg_ISR(mmsg_t *msg);
void    sdelay_HookTick_ISR(void);
void    sdelay_HookMainLoop(void);

uint8_t sdelay_MenuGetSubCount(void);
void    sdelay_MenuGetText(char *dest, uint8_t item);
uint8_t sdelay_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t sdelay_ConfigGetSize(void);
void    sdelay_ConfigSave(uint8_t *dest);
void    sdelay_ConfigLoad(uint8_t *dest);


#endif /* SUPERDELAY_H_ */
