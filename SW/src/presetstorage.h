/*
 * presetstorage.h
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */

#ifndef PRESETSTORAGE_H_
#define PRESETSTORAGE_H_


#include "common.h"


// The presets module takes care of saving and loading configuration

// Midiprocessing must be stopped before calling presets_load

bool_t  presets_validateEeprom(void);
void    presets_resetEeprom(void);

uint16_t presets_calcConfigSize(void);

uint8_t presets_load(uint8_t preset_slot, bool_t test);
uint8_t presets_save(uint8_t preset_slot);


#define PRESETS_SLOTS 10



// Return values for presets_load function
#define PRESET_OK             0
#define PRESET_EMPTY          1
#define PRESET_INVALID_UID    2
#define PRESET_INVALID_LENGTH 3
#define PRESET_LOAD_ERROR     4
#define PRESET_CHKSUM_ERROR   5

#define PRESET_SAVE_TOO_BIG   1


#endif /* PRESETSTORAGE_H_ */
