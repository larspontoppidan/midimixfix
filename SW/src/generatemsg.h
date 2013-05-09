/*
 * generatemsg.h
 *
 *  Created on: Jan 17, 2013
 *      Author: lars
 */

#ifndef GENERATEMSG_H_
#define GENERATEMSG_H_


#include "common.h"

void    GenMsg_initialize(void);

uint8_t GenMsg_menuGetSubCount(void);
void    GenMsg_menuGetText(char *dest, uint8_t item);
uint8_t GenMsg_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t GenMsg_configGetSize(void);
void    GenMsg_configSave(uint8_t *dest);
void    GenMsg_configLoad(uint8_t *dest);


#endif /* GENERATEMSG_H_ */
