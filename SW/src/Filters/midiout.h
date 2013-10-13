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

bool_t  midiout_new(uint8_t filter_type, uint8_t *config, filterInstance_t* self);
void    midiout_saveConfig(filterInstance_t* self, uint8_t *dest);
uint8_t midiout_request(filterInstance_t* self, uint8_t request);
void    midiout_processMidiMsg(filterInstance_t* self, midiMsg_t *msg);

void    midiout_getMenuTitle(uint8_t filter_type, char *dest);
void    midiout_getMenuText(filterInstance_t* self, char *dest, uint8_t item);
void    midiout_handleUiEvent(filterInstance_t* self, uint8_t item, uint8_t user_event);


#endif /* MIDIOUT_H_ */
