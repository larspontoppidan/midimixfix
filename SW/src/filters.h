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


#define FILTER_INTERFACE_LIST  \
    &midiin_In1,           \
    &midiin_In2,           \
    &midiin_In1Realtime,   \
    &midiin_In2Realtime,   \
    &midiin_In1Sysex,      \
    &midiin_In2Sysex,      \
    &pedals_In1,           \
    &pedals_In2,           \
    &routemsg_Filter,      \
    &dupmsg_Filter,        \
    &setchan_Filter,       \
    &modifycc_Filter,      \
    &transfilter_Filter,   \
    &curvefilter_Filter,   \
    &genmsg_Filter,        \
    &polysplit_Filter,     \
    &atsmoother_Filter,    \
    &midiout_Filter


enum
{
    FILTER_TYPE_MIDIIN_IN1 = 0,
    FILTER_TYPE_MIDIIN_IN2,
    FILTER_TYPE_MIDIIN_IN1REALTIME,
    FILTER_TYPE_MIDIIN_IN2REALTIME,
    FILTER_TYPE_MIDIIN_IN1SYSEX,
    FILTER_TYPE_MIDIIN_IN2SYSEX,
    FILTER_TYPE_PEDALS_IN1,
    FILTER_TYPE_PEDALS_IN2,
    FILTER_TYPE_ROUTEMSG_FILTER,
    FILTER_TYPE_DUPLICATEMSG_FILTER,
    FILTER_TYPE_SETCHAN_FILTER,
    FILTER_TYPE_MODIFYCC_FILTER,
    FILTER_TYPE_TRANSFILTER_FILTER,
    FILTER_TYPE_CURVE_FILTER,
    FILTER_TYPE_GENMSG_FILTER,
    FILTER_TYPE_POLYSPLIT_FILTER,
    FILTER_TYPE_ATSMOOTHER_FILTER,
    FILTER_TYPE_MIDIOUT_FILTER,
    FILTERS_TYPE_COUNT
};


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
