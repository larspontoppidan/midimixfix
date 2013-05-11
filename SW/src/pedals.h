/*
 * pedals.h
 *
 *  Created on: May 11, 2013
 *      Author: lars
 */

#ifndef PEDALS_H_
#define PEDALS_H_

#include "common.h"
#include "midimessage.h"

void    pedals_initialize(void);

void    pedals_handleTick_ISR(void);
void    pedals_handleMainLoop(void);

uint8_t pedals_menuGetSubCount(void);
void    pedals_menuGetText(char *dest, uint8_t item);
uint8_t pedals_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);

uint8_t pedals_configGetSize(void);
void    pedals_configSave(uint8_t *dest);
void    pedals_configLoad(uint8_t *dest);

uint8_t ProcessAdcValue(int8_t offset, uint8_t gain, uint16_t adc_value);


#endif /* PEDALS_H_ */
