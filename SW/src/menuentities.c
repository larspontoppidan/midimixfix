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

uint8_t MenuEnts_getSubCount(uint8_t entity)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = MainMenu_titleGetSubCount();
        break;
    case ENTITY_PRESETS:
        ret = presets_menuGetSubCount();
        break;
    case ENTITY_MAINSETUP:
        ret = MainMenu_setupGetSubCount();
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_menuGetSubCount();
        break;
    case ENTITY_MIDILOG:
        ret = MidiLog_menuGetSubCount();
        break;
    case ENTITY_BLOCKFILTER:
        ret = BlockFlt_menuGetSubCount();
        break;
    case ENTITY_CURVEFILTER:
        ret = CurveFlt_menuGetSubCount();
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

void MenuEnts_getText(uint8_t entity, char *dest, uint8_t item)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        MainMenu_titleGetText(dest, item);
        break;
    case ENTITY_PRESETS:
        presets_menuGetText(dest, item);
        break;
    case ENTITY_MAINSETUP:
        MainMenu_setupGetText(dest, item);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_menuGetText(dest, item);
        break;
    case ENTITY_MIDILOG:
        MidiLog_menuGetText(dest, item);
        break;
    case ENTITY_BLOCKFILTER:
        BlockFlt_menuGetText(dest, item);
        break;
    case ENTITY_CURVEFILTER:
        CurveFlt_menuGetText(dest, item);
        break;
    case ENTITY_SUPERDELAY:
        sdelay_MenuGetText(dest, item);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}

uint8_t MenuEnts_handleEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                       uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = MainMenu_titleHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_PRESETS:
        ret = presets_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MAINSETUP:
        ret = MainMenu_setupHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MIDILOG:
        ret = MidiLog_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_BLOCKFILTER:
        ret = BlockFlt_menuHandleEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_CURVEFILTER:
        ret = CurveFlt_menuHandleEvent(item, edit_mode, user_event, knob_delta);
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


uint8_t MenuEnts_configGetSize(uint8_t entity)
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
        ret = MainMenu_configGetSize();
        break;
    case ENTITY_GENERATEMSG:
        ret = genmsg_configGetSize();
        break;
    case ENTITY_MIDILOG:
        ret = 0;
        break;
    case ENTITY_BLOCKFILTER:
        ret = BlockFlt_configGetSize();
        break;
    case ENTITY_CURVEFILTER:
        ret = CurveFlt_configGetSize();
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

void MenuEnts_configSave(uint8_t entity, uint8_t *dest)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        break;
    case ENTITY_PRESETS:
        break;
    case ENTITY_MAINSETUP:
        MainMenu_configSave(dest);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_configSave(dest);
        break;
    case ENTITY_MIDILOG:
        break;
    case ENTITY_BLOCKFILTER:
        BlockFlt_configSave(dest);
        break;
    case ENTITY_CURVEFILTER:
        CurveFlt_configSave(dest);
        break;
    case ENTITY_SUPERDELAY:
        sdelay_ConfigSave(dest);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}

void MenuEnts_configLoad(uint8_t entity, uint8_t *dest)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        break;
    case ENTITY_PRESETS:
        break;
    case ENTITY_MAINSETUP:
        MainMenu_configLoad(dest);
        break;
    case ENTITY_GENERATEMSG:
        genmsg_configLoad(dest);
        break;
    case ENTITY_MIDILOG:
        break;
    case ENTITY_BLOCKFILTER:
        BlockFlt_configLoad(dest);
        break;
    case ENTITY_CURVEFILTER:
        CurveFlt_configLoad(dest);
        break;
    case ENTITY_SUPERDELAY:
        sdelay_ConfigLoad(dest);
        break;
    default:
        err_Raise(ERR_MODULE_MENUE, __LINE__);
        break;
    }
}
