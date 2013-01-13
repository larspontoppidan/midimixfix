/*
 * presets.h
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */

#ifndef PRESETS_H_
#define PRESETS_H_


#include "common.h"


// The presets module takes care of saving and loading configuration

void    presets_Initialize(void);

uint8_t presets_GetSubMenuCount(void);
void    presets_GetMenuText(char *dest, uint8_t item);
uint8_t presets_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);






#endif /* PRESETS_H_ */
