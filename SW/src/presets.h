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

void    presets_initialize(void);

uint8_t presets_menuGetSubCount(void);
void    presets_menuGetText(char *dest, uint8_t item);
uint8_t presets_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);


// In order to monitor changes and show modified in case of such, the
// preset module must be notified after a menu entity has been accessed and potentially modified
void presets_notifyConfigMayChange(uint8_t entity_index);



#endif /* PRESETS_H_ */
