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

uint8_t menue_GetSubCount(uint8_t entity)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = mmenu_TitleMenuGetSubCount();
        break;
    case ENTITY_PRESETS:
        ret = presets_menuGetSubCount();
        break;
    case ENTITY_MAINSETUP:
        ret = mmenu_SetupMenuGetSubCount();
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_menuGetSubCount();
        break;
    case ENTITY_MIDILOG:
        ret = mlog_menuGetSubCount();
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockf_MenuGetSubCount();
        break;
    case ENTITY_CURVEFILTER:
        ret = curvef_MenuGetSubCount();
        break;
    case ENTITY_SUPERDELAY:
        ret = sdelay_MenuGetSubCount();
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }

    return ret;
}

void menue_GetText(uint8_t entity, char *dest, uint8_t item)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        mmenu_TitleMenuGetText(dest, item);
        break;
    case ENTITY_PRESETS:
        presets_menuGetText(dest, item);
        break;
    case ENTITY_MAINSETUP:
        mmenu_SetupMenuGetText(dest, item);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_menuGetText(dest, item);
        break;
    case ENTITY_MIDILOG:
        mlog_menuGetText(dest, item);
        break;
    case ENTITY_BLOCKFILTER:
        blockf_MenuGetText(dest, item);
        break;
    case ENTITY_CURVEFILTER:
        curvef_MenuGetText(dest, item);
        break;
    case ENTITY_SUPERDELAY:
        sdelay_MenuGetText(dest, item);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}

uint8_t menue_HandleEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                       uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = mmenu_TitleMenuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_PRESETS:
        ret = presets_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MAINSETUP:
        ret = mmenu_SetupMenuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MIDILOG:
        ret = mlog_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockf_MenuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_CURVEFILTER:
        ret = curvef_MenuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_SUPERDELAY:
        ret = sdelay_MenuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }

    return ret;
}


uint8_t menue_ConfigGetSize(uint8_t entity)
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
        ret = mmenu_ConfigGetSize();
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_configGetSize();
        break;
    case ENTITY_MIDILOG:
        ret = 0;
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockf_ConfigGetSize();
        break;
    case ENTITY_CURVEFILTER:
        ret = curvef_ConfigGetSize();
        break;
    case ENTITY_SUPERDELAY:
        ret = sdelay_ConfigGetSize();
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }

    return ret;
}

void menue_ConfigSave(uint8_t entity, uint8_t *dest)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        break;
    case ENTITY_PRESETS:
        break;
    case ENTITY_MAINSETUP:
        mmenu_ConfigSave(dest);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_configSave(dest);
        break;
    case ENTITY_MIDILOG:
        break;
    case ENTITY_BLOCKFILTER:
        blockf_ConfigSave(dest);
        break;
    case ENTITY_CURVEFILTER:
        curvef_ConfigSave(dest);
        break;
    case ENTITY_SUPERDELAY:
        sdelay_ConfigSave(dest);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}

void menue_ConfigLoad(uint8_t entity, uint8_t *dest)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        break;
    case ENTITY_PRESETS:
        break;
    case ENTITY_MAINSETUP:
        mmenu_ConfigLoad(dest);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_configLoad(dest);
        break;
    case ENTITY_MIDILOG:
        break;
    case ENTITY_BLOCKFILTER:
        blockf_ConfigLoad(dest);
        break;
    case ENTITY_CURVEFILTER:
        curvef_ConfigLoad(dest);
        break;
    case ENTITY_SUPERDELAY:
        sdelay_ConfigLoad(dest);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}
