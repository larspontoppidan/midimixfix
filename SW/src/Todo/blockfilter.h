/*
 * blockfilter.h
 *
 *  Created on: Dec 22, 2012
 *      Author: lars
 */

#ifndef BLOCKFILTER_H_
#define BLOCKFILTER_H_

#include "../common.h"
#include "../midimessage.h"



bool_t  blockflt_new(uint8_t filter_type, uint8_t *config, filter_t* self);
void    blockflt_saveConfig(filter_t* self, uint8_t *dest);
uint8_t blockflt_request(filter_t* self, uint8_t request);
void    blockflt_processMidiMsg(filter_t* self, midiMsg_t *msg);
void    blockflt_getMenuTitle(uint8_t filter_type, char *dest);
void    blockflt_getMenuText(filter_t* self, char *dest, uint8_t item);
void    blockflt_handleUiEvent(filter_t* self, uint8_t item, uint8_t user_event);






void    blockflt_initialize(void);

void    blockflt_handleMidiMsg_ISR(midiMsg_t *msg);
void    blockflt_handleTick_ISR(void);
void    blockflt_handleMainLoop(void);

uint8_t blockflt_menuGetSubCount(void);
void    blockflt_menuGetText(char *dest, uint8_t item);
uint8_t blockflt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t blockflt_configGetSize(void);
void    blockflt_configSave(uint8_t *dest);
void    blockflt_configLoad(uint8_t *dest);

#endif /* BLOCKFILTER_H_ */
