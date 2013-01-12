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
#include "blockfilter.h"
#include "midilogger.h"

enum
{
    ENTITY_MAINTITLE = 0,
    ENTITY_MAINSETUP,
    ENTITY_MIDILOG,
    ENTITY_BLOCKFILTER,
};

uint8_t menue_GetSubMenuCount(uint8_t entity)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = mainmenu_TitleGetSubMenuCount();
        break;
    case ENTITY_MAINSETUP:
        ret = mainmenu_SetupGetSubMenuCount();
        break;
    case ENTITY_MIDILOG:
        ret = mlog_GetSubMenuCount();
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockf_GetSubMenuCount();
        break;
    default:
        err_Register(ERR_INVALID_MENU_ENTITY);
        break;
    }

    return ret;
}

void menue_GetMenuText(uint8_t entity, char *dest, uint8_t item)
{
    switch(entity)
    {
    case ENTITY_MAINTITLE:
        mainmenu_TitleGetMenuText(dest, item);
        break;
    case ENTITY_MAINSETUP:
        mainmenu_SetupGetMenuText(dest, item);
        break;
    case ENTITY_MIDILOG:
        mlog_GetMenuText(dest, item);
        break;
    case ENTITY_BLOCKFILTER:
        blockf_GetMenuText(dest, item);
        break;
    default:
        err_Register(ERR_INVALID_MENU_ENTITY);
        break;
    }
}

uint8_t menue_MenuEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                       uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = 0u;

    switch(entity)
    {
    case ENTITY_MAINTITLE:
        ret = mainmenu_TitleMenuEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MAINSETUP:
        ret = mainmenu_SetupMenuEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_MIDILOG:
        ret = mlog_MenuEvent(item, edit_mode, user_event, knob_delta);
        break;
    case ENTITY_BLOCKFILTER:
        ret = blockf_MenuEvent(item, edit_mode, user_event, knob_delta);
        break;
    default:
        err_Register(ERR_INVALID_MENU_ENTITY);
        break;
    }

    return ret;
}
