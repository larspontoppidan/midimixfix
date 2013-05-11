/*
 * mainmenu.c
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */


#include "common.h"
#include "mainmenu.h"
#include "menu.h" // for MENU_ defines TODO could be avoided
#include "util.h"
#include "midimessage.h"
#include <avr/pgmspace.h>
#include "pgmstrings.h"
#include "errors.h"

#include "midiio.h"

static char TitleString0[] PROGMEM = "- MIDIMIXFIX ";
static char TitleString1[] PROGMEM = __DATE__;
static char TitleString2[] PROGMEM = "Error: ";

static char SetupString0[] PROGMEM = "IO setup ";
static char SetupString1[] PROGMEM = "In1       :";
static char SetupString2[] PROGMEM = "In1 rt.msg:";
static char SetupString3[] PROGMEM = "In2       :";
static char SetupString4[] PROGMEM = "In2 rt.msg:";
static char SetupString5[] PROGMEM = "Send runn.st.:";

static uint8_t CursorPositions[6] = {0, 11, 11, 11, 11, 14};


static char *ModeStrings[3] = {pstr_Discard, pstr_Through, pstr_Use};

static bool_t SetupMenuVisible = FALSE;

static bool_t TitleMenuVisible = FALSE;

uint8_t mainmenu_titleGetSubCount(void)
{
    if (TitleMenuVisible)
    {
        return err_getCount() + 1;
    }
    else
    {
        return 0;
    }
}

void mainmenu_titleGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        dest = util_copyString_P(dest, TitleString0);

        dest = util_writeInt8LA(dest, BUILD_VERSION_MAJOR);
        (*dest++) = '.';
        dest = util_writeInt8LA(dest, BUILD_VERSION_MINOR);
        (*dest++) = ' ';
        (*dest++) = '-';
    }
    else if (item == 1)
    {
        // Build date
        util_copyString_P(dest, TitleString1);
    }
    else
    {
        // Errors
        err_print(dest, item - 2);
    }
}

uint8_t mainmenu_titleHandleEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    UNUSED(edit_mode);
    UNUSED(item);
    UNUSED(knob_delta);

    if (user_event == MENU_EVENT_SELECT)
    {
        TitleMenuVisible = !TitleMenuVisible;
        ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
    }
    else if (user_event == MENU_EVENT_BACK)
    {
        TitleMenuVisible = FALSE;
        ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
    }

    return ret;
}


// The general setup options

uint8_t mainmenu_setupGetSubCount(void)
{
    return SetupMenuVisible ? 5 : 0;
}

void mainmenu_setupGetText(char *dest, uint8_t item)
{
    uint8_t i;

    switch (item)
    {
    case 0: // General setup
        util_copyString_P(dest, SetupString0);
        if (SetupMenuVisible)
        {
            dest = util_copyString_P(dest + 16, pstr_MinusParentheses);
        }
        else
        {
            dest = util_copyString_P(dest + 16, pstr_PlusParentheses);
        }
        break;

    case 1: // In1
        dest = util_copyString_P(dest, SetupString1);

        i = midiio_getMode(MIDIMSG_SOURCE_INPUT1, FALSE);
        dest = util_copyString_P(dest, ModeStrings[i]);

        break;

    case 2: // In1 realtime:
        dest = util_copyString_P(dest, SetupString2);

        i = midiio_getMode(MIDIMSG_SOURCE_INPUT1, TRUE);
        dest = util_copyString_P(dest, ModeStrings[i]);

        break;

    case 3: // In2
        dest = util_copyString_P(dest, SetupString3);

        i = midiio_getMode(MIDIMSG_SOURCE_INPUT2, FALSE);
        dest = util_copyString_P(dest, ModeStrings[i]);

        break;

    case 4: // In2 realtime:
        dest = util_copyString_P(dest, SetupString4);

        i = midiio_getMode(MIDIMSG_SOURCE_INPUT2, TRUE);
        dest = util_copyString_P(dest, ModeStrings[i]);

        break;

    case 5: // Send runn. status
        dest = util_copyString_P(dest, SetupString5);

        if (midiio_getRunStatusMode())
        {
            dest = util_copyString_P(dest, pstr_On);
        }
        else
        {
            dest = util_copyString_P(dest, pstr_Off);
        }
        break;
    }

}


uint8_t mainmenu_setupHandleEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    if (user_event == MENU_EVENT_SELECT)
    {
        if (edit_mode == 0)
        {
            if (item == 0)
            {
                // This toggles showing sub items
                SetupMenuVisible = !SetupMenuVisible;
                ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
            }
            else
            {
                // Report back desired cursor position
                ret = CursorPositions[item];
            }
        }
    }
    else if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
    {
        int8_t i;

        // Report back desired cursor position
        ret = CursorPositions[item];

        switch (item)
        {
        case 1: // In1
            // Get current mode
            i = midiio_getMode(MIDIMSG_SOURCE_INPUT1, FALSE);
            // The available modes are 0, 1, 2. Add the knob delta with bounds
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            // Set new mode
            midiio_setMode(i, MIDIMSG_SOURCE_INPUT1, FALSE);
            break;

        case 2: // In1 realtime
            i = midiio_getMode(MIDIMSG_SOURCE_INPUT1, TRUE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            midiio_setMode(i, MIDIMSG_SOURCE_INPUT1, TRUE);
            break;

        case 3: // In2
            i = midiio_getMode(MIDIMSG_SOURCE_INPUT2, FALSE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            midiio_setMode(i, MIDIMSG_SOURCE_INPUT2, FALSE);
            break;

        case 4: // In2 realtime
            i = midiio_getMode(MIDIMSG_SOURCE_INPUT2, TRUE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            midiio_setMode(i, MIDIMSG_SOURCE_INPUT2, TRUE);
            break;

        case 5:
            // Set the running status
            midiio_setRunStatusMode(knob_delta > 0 ? TRUE : FALSE);
            break;
        }
    }

    return ret;
}


// Configuration store and load implementation

uint8_t mainmenu_configGetSize(void)
{
    return 5;
}

void mainmenu_configSave(uint8_t *dest)
{
    *(dest++) = midiio_getMode(MIDIMSG_SOURCE_INPUT1, FALSE);
    *(dest++) = midiio_getMode(MIDIMSG_SOURCE_INPUT1, TRUE);
    *(dest++) = midiio_getMode(MIDIMSG_SOURCE_INPUT2, FALSE);
    *(dest++) = midiio_getMode(MIDIMSG_SOURCE_INPUT2, TRUE);
    *(dest++) = midiio_getRunStatusMode();
}

void mainmenu_configLoad(uint8_t *dest)
{
    midiio_setMode(*(dest++), MIDIMSG_SOURCE_INPUT1, FALSE);
    midiio_setMode(*(dest++), MIDIMSG_SOURCE_INPUT1, TRUE);
    midiio_setMode(*(dest++), MIDIMSG_SOURCE_INPUT2, FALSE);
    midiio_setMode(*(dest++), MIDIMSG_SOURCE_INPUT2, TRUE);
    midiio_setRunStatusMode(*(dest++));
}
