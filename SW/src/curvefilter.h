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

void curvef_Initialize(void);

void curvef_HookMidiMsg_ISR(mmsg_t *msg);
void curvef_HookMainLoop(void);

uint8_t curvef_MenuGetSubCount(void);
void    curvef_MenuGetText(char *dest, uint8_t item);
uint8_t curvef_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t curvef_ConfigGetSize(void);
void    curvef_ConfigSave(uint8_t *dest);
void    curvef_ConfigLoad(uint8_t *dest);



#endif /* CURVEFILTER_H_ */
