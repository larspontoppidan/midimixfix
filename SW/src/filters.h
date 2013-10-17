/*
 * filters.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "common.h"
#include "filterinterface.h"
#include "midimessage.h"
#include <avr/pgmspace.h>

// The filters module defines a common interface to all filters in the system: filters_instance_t
//
// Some functions require full specification of filer type and instance number
// Some functions are "static" in that they don't care about the filter instance



// This struct is used to conveniently refer to instances of filters
typedef struct
{
    uint8_t FilterType;
    uint8_t Instance;

} filters_instance_t;

#define FILTERS_TYPE_COUNT 7

// Wrappers performing calls into the applicable filter type:

uint8_t filters_create(uint8_t filter_type, uint8_t filter_step);
void filters_destroy(filters_instance_t filter);
void filters_setFilterStep(filters_instance_t filter, uint8_t filter_step);

void filters_loadConfig(filters_instance_t filter, void* config_data);
void filters_saveConfig(filters_instance_t filter, void* config_data);

uint8_t filters_processMidiMsg(filters_instance_t filter, midiMsg_t *midimsg);
void filters_writeMenuText(filters_instance_t filter, uint8_t menu_item, uint8_t *dest);
void filters_handleUiEvent(filters_instance_t filter, uint8_t menu_item, uint8_t user_event);

uint8_t filters_getConfigSize(uint8_t filter_type);
uint8_t filters_getMenuItems(uint8_t filter_type);
uint8_t filters_getMenuIndent(uint8_t filter_type);
uint8_t filters_getFilterMode(uint8_t filter_type);
const char* filters_getFilterTitle(uint8_t filter_type);
uint32_t filters_getUniqueId(uint8_t filter_type);


uint8_t filters_findFilterType(uint32_t unique_id);

#define FILTERS_FILTER_NOT_FOUND 0xFF

#endif /* FILTERS_H_ */
