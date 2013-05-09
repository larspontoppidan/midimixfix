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

void    SuperDly_initialize(void);

void    SuperDly_handleMidiMsg_ISR(midiMsg_t *msg);
void    SuperDly_handleTick_ISR(void);
void    SuperDly_handleMainLoop(void);

uint8_t SuperDly_MenuGetSubCount(void);
void    SuperDly_menuGetText(char *dest, uint8_t item);
uint8_t SuperDly_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t SuperDly_configGetSize(void);
void    SuperDly_configSave(uint8_t *dest);
void    SuperDly_configLoad(uint8_t *dest);


#endif /* SUPERDELAY_H_ */
