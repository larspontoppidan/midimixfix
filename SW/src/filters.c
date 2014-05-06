
//
// Filename    : midilogmenu.c
// Code module : Midilog menu implementation
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2013, Lars Ole Pontoppidan (Larsp)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// *  Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// *  Neither the name of the original author (Lars Ole Pontoppidan) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//
// ------------------------------  DESCRIPTION  ---------------------------------


// -------------------------------  INCLUDES  -----------------------------------

#include "filters.h"
#include "filterinterface.h"
#include "errors.h"

#include "Filters/midiin.h"
#include "Filters/midiout.h"
#include "Filters/pedals.h"
#include "Filters/routeduplicate.h"
#include "Filters/setchannel.h"
#include "Filters/transposefilter.h"
#include "Filters/modifycontroller.h"
#include "Filters/curvefilter.h"
#include "Filters/generatemsg.h"
#include "Filters/polysplit.h"
#include "Filters/atsmoother.h"


// ----------------------------  LOCAL VARIABLES  -------------------------------


static const filterInterface_t PROGMEM * const filters_Types[FILTERS_TYPE_COUNT] =
{
        FILTER_INTERFACE_LIST
};


// --------------------------  TYPES AND CONSTANTS  -----------------------------

// LOOKUP_FILTER_TYPE(x) reads from filters_Types array and returns address of
// the filter's filterInterface_t struct, located in PROGMEM!

#define LOOKUP_FILTER_TYPE(x) ((const filterInterface_t*) pgm_read_word( &(filters_Types[(x)]) ) )

// ------------------------------  PROTOTYPES  ----------------------------------


static bool_t validateFilterType(uint8_t filter_type);

// ---------------------------  PRIVATE FUNCTIONS  -------------------------------

static bool_t validateFilterType(uint8_t filter_type)
{
    return (filter_type < FILTERS_TYPE_COUNT);
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

uint8_t filters_create(uint8_t filter_type, uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;

    if (validateFilterType(filter_type))
    {
        fptrUint8Uint8_t create = (fptrUint8Uint8_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter_type)->create));

        ret = (create)(filter_step);
    }

    return ret;
}

void filters_destroy(filters_instance_t filter)
{
    if (validateFilterType(filter.FilterType))
    {
        fptrVoidUint8_t destroy = (fptrVoidUint8_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->destroy));

        (destroy)(filter.Instance);
    }
}

void filters_setFilterStep(filters_instance_t filter, uint8_t filter_step)
{
    if (validateFilterType(filter.FilterType))
    {
        fptrVoidUint8Uint8_t setFilterStep = (fptrVoidUint8Uint8_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->setFilterStep));

        (setFilterStep)(filter.Instance, filter_step);
    }
}

void filters_loadConfig(filters_instance_t filter, void* config_data)
{
    if (validateFilterType(filter.FilterType))
    {
        fptrVoidUint8Voidp_t loadConfig = (fptrVoidUint8Voidp_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->loadConfig));

        (loadConfig)(filter.Instance, config_data);
    }
}

void filters_saveConfig(filters_instance_t filter, void* config_data)
{
    if (validateFilterType(filter.FilterType))
    {
        fptrVoidUint8Voidp_t saveConfig = (fptrVoidUint8Voidp_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->saveConfig));

        (saveConfig)(filter.Instance, config_data);
    }
}

uint8_t filters_processMidiMsg(filters_instance_t filter, midiMsg_t *midimsg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    if (validateFilterType(filter.FilterType))
    {
        fptrProcessMidiMsg_t processMidiMsg = (fptrProcessMidiMsg_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->processMidiMsg));

        ret = (processMidiMsg)(filter.Instance, midimsg);
    }

    return ret;
}

void filters_writeMenuText(filters_instance_t filter, uint8_t menu_item, uint8_t *dest)
{
    if (validateFilterType(filter.FilterType))
    {
        fptrWriteMenuText_t writeMenuText = (fptrWriteMenuText_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->writeMenuText));

        (writeMenuText)(filter.Instance, menu_item, dest);
    }
}

void filters_handleUiEvent(filters_instance_t filter, uint8_t menu_item, uint8_t user_event)
{
    if (validateFilterType(filter.FilterType))
    {
        fptrHandleUserEvent_t handleUiEvent = (fptrHandleUserEvent_t)pgm_read_word(
                &(LOOKUP_FILTER_TYPE(filter.FilterType)->handleUiEvent));

        (handleUiEvent)(filter.Instance, menu_item, user_event);
    }
}

uint8_t filters_getConfigSize(uint8_t filter_type)
{
    return (uint8_t)pgm_read_byte( &(LOOKUP_FILTER_TYPE(filter_type)->ConfigSize) );
}

uint8_t filters_getMenuItems(uint8_t filter_type)
{
    return (uint8_t)pgm_read_byte( &(LOOKUP_FILTER_TYPE(filter_type)->MenuItems) );
}

uint8_t filters_getMenuIndent(uint8_t filter_type)
{
    return (uint8_t)pgm_read_byte( &(LOOKUP_FILTER_TYPE(filter_type)->MenuIndent) );
}

uint8_t filters_getFilterMode(uint8_t filter_type)
{
    return (uint8_t)pgm_read_byte( &(LOOKUP_FILTER_TYPE(filter_type)->FilterMode) );
}

const char* filters_getFilterTitle(uint8_t filter_type)
{
    return (const char*) pgm_read_word( &(LOOKUP_FILTER_TYPE(filter_type)->FilterTitle) );
}

uint32_t filters_getUniqueId(uint8_t filter_type)
{
    return (uint32_t)pgm_read_dword( &(LOOKUP_FILTER_TYPE(filter_type)->UniqueId) );
}

uint8_t filters_findFilterType(uint32_t unique_id)
{
    uint8_t i;
    uint8_t ret = FILTERS_FILTER_NOT_FOUND;

    for (i = 0; i < FILTERS_TYPE_COUNT; i++)
    {
        if (filters_getUniqueId(i) == unique_id)
        {
            ret = i;
            break;
        }
    }

    return ret;
}

