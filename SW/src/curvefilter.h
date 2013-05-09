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

void CurveFlt_initialize(void);

void CurveFlt_handleMidiMsg_ISR(midiMsg_t *msg);
void CurveFlt_handleMainLoop(void);

uint8_t CurveFlt_menuGetSubCount(void);
void    CurveFlt_menuGetText(char *dest, uint8_t item);
uint8_t CurveFlt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t CurveFlt_configGetSize(void);
void    CurveFlt_configSave(uint8_t *dest);
void    CurveFlt_configLoad(uint8_t *dest);



#endif /* CURVEFILTER_H_ */
