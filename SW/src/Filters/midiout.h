/*
 * midiout.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef MIDIOUT_H_
#define MIDIOUT_H_

#include "../common.h"
#include "../filters.h"
#include "../midimessage.h"

void midiout_initialize(void);


bool_t  midiout_new(uint8_t filter_type, uint8_t *config, filter_t* self);

void    midiout_saveConfig(filter_t* self, uint8_t *dest);

uint8_t midiout_request(filter_t* self, uint8_t request);

void    midiout_processMidiMsg(filter_t* self, midiMsg_t *msg);


void    midiout_getMenuTitle(uint8_t filter_type, char *dest);

void    midiout_getMenuText(filter_t* self, char *dest, uint8_t item);

void    midiout_handleUiEvent(filter_t* self, uint8_t item, uint8_t user_event);


#endif /* MIDIOUT_H_ */
