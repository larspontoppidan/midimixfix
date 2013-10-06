/*
 * pedals.h
 *
 *  Created on: May 11, 2013
 *      Author: lars
 */

#ifndef PEDALS_H_
#define PEDALS_H_

#include "common.h"
#include "midimessage.h"

void    pedals_initialize(void);

void    pedals_handleTick_ISR(void);
void    pedals_handleMainLoop(void);

uint8_t pedals_menuGetSubCount(uint8_t pedal);
void    pedals_menuGetText(uint8_t pedal, char *dest, uint8_t item);
uint8_t pedals_menuHandleEvent(uint8_t pedal, uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t pedals_configGetSize(void);
void    pedals_configSave(uint8_t *dest);
void    pedals_configLoad(uint8_t *dest);


#endif /* PEDALS_H_ */
