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
void curvef_MessageIsrHook(mmsg_t *msg);
void curvef_MainLoopHook(void);

uint8_t curvef_GetSubMenuCount(void);
void    curvef_GetMenuText(char *dest, uint8_t item);
uint8_t curvef_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


#endif /* CURVEFILTER_H_ */
