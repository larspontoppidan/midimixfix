/*
 * generatemsg.h
 *
 *  Created on: Jan 17, 2013
 *      Author: lars
 */

#ifndef GENERATEMSG_H_
#define GENERATEMSG_H_


#include "common.h"

void    genmsg_initialize(void);

uint8_t genmsg_menuGetSubCount(void);
void    genmsg_menuGetText(char *dest, uint8_t item);
uint8_t genmsg_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t genmsg_configGetSize(void);
void    genmsg_configSave(uint8_t *dest);
void    genmsg_configLoad(uint8_t *dest);


#endif /* GENERATEMSG_H_ */
