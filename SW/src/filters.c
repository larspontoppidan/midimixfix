/*
 * filters.c
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#include "filters.h"
#include "errors.h"
#include "Filters/midiin.h"
#include "Filters/midiout.h"

bool_t filter_new(uint8_t filter_type, uint8_t *config, filter_t* instance)
{
    bool_t ret = FALSE;

    switch (instance->FilterType)
    {
    case FILTER_MIDIIN1:
    case FILTER_MIDIIN2:
    case FILTER_MIDIIN1_REALT:
    case FILTER_MIDIIN2_REALT:
    case FILTER_MIDIIN1_SYSEX:
    case FILTER_MIDIIN2_SYSEX:
        ret = midiin_new(filter_type, config, instance);
        break;
    case FILTER_MIDIOUT:
        ret = midiout_new(filter_type, config, instance);
        break;
    default:
        err_raise(ERR_MODULE_FILTERS, __LINE__);
        break;
    }

    return ret;
}


uint8_t filter_request(filter_t* instance, uint8_t request)
{
    uint8_t ret = 0;

    switch (instance->FilterType)
    {
    case FILTER_MIDIIN1:
    case FILTER_MIDIIN2:
    case FILTER_MIDIIN1_REALT:
    case FILTER_MIDIIN2_REALT:
    case FILTER_MIDIIN1_SYSEX:
    case FILTER_MIDIIN2_SYSEX:
        ret = midiin_request(instance, request);
        break;
    case FILTER_MIDIOUT:
        ret = midiout_request(instance, request);
        break;
    default:
        err_raise(ERR_MODULE_FILTERS, __LINE__);
        break;
    }

    return ret;
}

void filter_saveConfig(filter_t* instance, uint8_t *dest)
{
    switch (instance->FilterType)
    {
    case FILTER_MIDIOUT:
        midiout_saveConfig(instance, dest);
        break;
    default:
        break;
    }
}

// Process midi msg

void filter_processMidiMsg(filter_t* instance, midiMsg_t *msg)
{
    switch (instance->FilterType)
    {
    case FILTER_MIDIOUT:
        midiout_processMidiMsg(instance, msg);
        break;
    default:
        break;
    }
}

void filter_getTitle(uint8_t filter_type, char *dest)
{
    switch (filter_type)
    {
    case FILTER_MIDIIN1:
    case FILTER_MIDIIN2:
    case FILTER_MIDIIN1_REALT:
    case FILTER_MIDIIN2_REALT:
    case FILTER_MIDIIN1_SYSEX:
    case FILTER_MIDIIN2_SYSEX:
        midiin_getMenuTitle(filter_type, dest);
        break;
    case FILTER_MIDIOUT:
        midiout_getMenuTitle(filter_type, dest);
        break;
    default:
        err_raise(ERR_MODULE_FILTERS, __LINE__);
        break;
    }
}

// Menu integration
void filter_getMenuText(filter_t* instance, char *dest, uint8_t item)
{
    switch (instance->FilterType)
    {
    case FILTER_MIDIOUT:
        midiout_getMenuText(instance, dest, item);
        break;
    default:
        err_raise(ERR_MODULE_FILTERS, __LINE__);
        break;
    }
}

void filter_handleUiEvent(filter_t* instance, uint8_t item, uint8_t user_event)
{
    switch (instance->FilterType)
    {
    case FILTER_MIDIIN1:
    case FILTER_MIDIIN2:
    case FILTER_MIDIIN1_REALT:
    case FILTER_MIDIIN2_REALT:
    case FILTER_MIDIIN1_SYSEX:
    case FILTER_MIDIIN2_SYSEX:
        break;
    case FILTER_MIDIOUT:
        midiout_handleUiEvent(instance, item, user_event);
        break;
    default:
        err_raise(ERR_MODULE_FILTERS, __LINE__);
        break;
    }
}
