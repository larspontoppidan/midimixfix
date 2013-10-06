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

void    superdly_initialize(void);

void    superdly_handleMidiMsg_ISR(midiMsg_t *msg);
void    superdly_handleTick_ISR(void);
void    superdly_handleMainLoop(void);

uint8_t superdly_menuGetSubCount(void);
void    superdly_menuGetText(char *dest, uint8_t item);
uint8_t superdly_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t superdly_configGetSize(void);
void    superdly_configSave(uint8_t *dest);
void    superdly_configLoad(uint8_t *dest);


#endif /* SUPERDELAY_H_ */
