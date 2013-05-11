/*
 * menuentities.c
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */


#include "common.h"
#include "menuentities.h"
#include "errors.h"

#include "mainmenu.h"
#include "midilogger.h"
#include "generatemsg.h"
#include "blockfilter.h"
#include "curvefilter.h"
#include "superdelay.h"
#include "presets.h"

enum
{
    ENTITY_MAINTITLE = 0,
    ENTITY_PRESETS,
    ENTITY_MAINSETUP,
    ENTITY_GENERATEMSG,
    ENTITY_BLOCKFILTER,
    ENTITY_CURVEFILTER,
    ENTITY_SUPERDELAY,
    ENTITY_MIDILOG
};

uint8_t menuents_getSubCount(uint8_t entity)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = mainmenu_titleGetSubCount();
        break;
    case ENTITY_PRESETS:
        ret = presets_menuGetSubCount();
        break;
    case ENTITY_MAINSETUP:
        ret = mainmenu_setupGetSubCount();
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_menuGetSubCount();
        break;
    case ENTITY_MIDILOG:
        ret = midilog_menuGetSubCount();
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockflt_menuGetSubCount();
        break;
    case ENTITY_CURVEFILTER:
        ret = curveflt_menuGetSubCount();
        break;
    case ENTITY_SUPERDELAY:
        ret = superdly_menuGetSubCount();
        break;
    default:
        err_raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }

    return ret;
}

void menuents_getText(uint8_t entity, char *dest, uint8_t item)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        mainmenu_titleGetText(dest, item);
        break;
    case ENTITY_PRESETS:
        presets_menuGetText(dest, item);
        break;
    case ENTITY_MAINSETUP:
        mainmenu_setupGetText(dest, item);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_menuGetText(dest, item);
        break;
    case ENTITY_MIDILOG:
        midilog_menuGetText(dest, item);
        break;
    case ENTITY_BLOCKFILTER:
        blockflt_menuGetText(dest, item);
        break;
    case ENTITY_CURVEFILTER:
        curveflt_menuGetText(dest, item);
        break;
    case ENTITY_SUPERDELAY:
        superdly_menuGetText(dest, item);
        break;
    default:
        err_raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}

uint8_t menuents_handleEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                       uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = mainmenu_titleHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_PRESETS:
        ret = presets_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MAINSETUP:
        ret = mainmenu_setupHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MIDILOG:
        ret = midilog_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockflt_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_CURVEFILTER:
        ret = curveflt_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_SUPERDELAY:
        ret = superdly_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    default:
        err_raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }

    return ret;
}


uint8_t menuents_configGetSize(uint8_t entity)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = 0;
        break;
    case ENTITY_PRESETS:
        ret = 0;
        break;
    case ENTITY_MAINSETUP:
        ret = mainmenu_configGetSize();
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_configGetSize();
        break;
    case ENTITY_MIDILOG:
        ret = 0;
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockflt_configGetSize();
        break;
    case ENTITY_CURVEFILTER:
        ret = curveflt_configGetSize();
        break;
    case ENTITY_SUPERDELAY:
        ret = superdly_configGetSize();
        break;
    default:
        err_raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }

    return ret;
}

void menuents_configSave(uint8_t entity, uint8_t *dest)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        break;
    case ENTITY_PRESETS:
        break;
    case ENTITY_MAINSETUP:
        mainmenu_configSave(dest);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_configSave(dest);
        break;
    case ENTITY_MIDILOG:
        break;
    case ENTITY_BLOCKFILTER:
        blockflt_configSave(dest);
        break;
    case ENTITY_CURVEFILTER:
        curveflt_configSave(dest);
        break;
    case ENTITY_SUPERDELAY:
        superdly_configSave(dest);
        break;
    default:
        err_raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}

void menuents_configLoad(uint8_t entity, uint8_t *dest)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        break;
    case ENTITY_PRESETS:
        break;
    case ENTITY_MAINSETUP:
        mainmenu_configLoad(dest);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_configLoad(dest);
        break;
    case ENTITY_MIDILOG:
        break;
    case ENTITY_BLOCKFILTER:
        blockflt_configLoad(dest);
        break;
    case ENTITY_CURVEFILTER:
        curveflt_configLoad(dest);
        break;
    case ENTITY_SUPERDELAY:
        superdly_configLoad(dest);
        break;
    default:
        err_raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}
