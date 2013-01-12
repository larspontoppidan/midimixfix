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

void blockf_Initialize(void);

void blockf_MessageIsrHook(mmsg_t *msg);
void blockf_TickIsrHook(void);
void blockf_MainLoopHook(void);

uint8_t blockf_GetSubMenuCount(void);
void    blockf_GetMenuText(char *dest, uint8_t item);
uint8_t blockf_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


#endif /* BLOCKFILTER_H_ */
