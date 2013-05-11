/*
 * curvefilter.h
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */

#ifndef CURVEFILTER_H_
#define CURVEFILTER_H_

#include "common.h"
#include "midimessage.h"

void curveflt_initialize(void);

void curveflt_handleMidiMsg_ISR(midiMsg_t *msg);
void curveflt_handleMainLoop(void);

uint8_t curveflt_menuGetSubCount(void);
void    curveflt_menuGetText(char *dest, uint8_t item);
uint8_t curveflt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t curveflt_configGetSize(void);
void    curveflt_configSave(uint8_t *dest);
void    curveflt_configLoad(uint8_t *dest);



#endif /* CURVEFILTER_H_ */
