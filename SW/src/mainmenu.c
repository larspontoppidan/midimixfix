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

static char titleString0[] PROGMEM = "- MIDIMIXFIX ";
static char titleString1[] PROGMEM = __DATE__;
static char titleString2[] PROGMEM = "Error: ";

static char setupString0[]  PROGMEM = "IO setup ";
static char setupString1[] PROGMEM = "In1       :";
static char setupString2[] PROGMEM = "In1 rt.msg:";
static char setupString3[] PROGMEM = "In2       :";
static char setupString4[] PROGMEM = "In2 rt.msg:";
static char setupString5[] PROGMEM = "Send runn.st.:";

static uint8_t cursorPositions[6] = {0, 11, 11, 11, 11, 14};


static char *modeStrings[3] = {PStr_Discard, PStr_Through, PStr_Use};

static bool_t setupMenuVisible = FALSE;

static bool_t titleMenuVisible = FALSE;

uint8_t MainMenu_titleGetSubCount(void)
{
    if (titleMenuVisible)
    {
        return err_GetCount() + 1;
    }
    else
    {
        return 0;
    }
}

void MainMenu_titleGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        dest = util_strCpy_P(dest, titleString0);

        dest = util_strWriteInt8LA(dest, BUILD_VERSION_MAJOR);
        (*dest++) = '.';
        dest = util_strWriteInt8LA(dest, BUILD_VERSION_MINOR);
        (*dest++) = ' ';
        (*dest++) = '-';
    }
    else if (item == 1)
    {
        // Build date
        util_strCpy_P(dest, titleString1);
    }
    else
    {
        // Errors
        err_Print(dest, item - 2);
    }
}

uint8_t MainMenu_titleHandleEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    UNUSED(edit_mode);
    UNUSED(item);
    UNUSED(knob_delta);

    if (user_event == MENU_EVENT_SELECT)
    {
        titleMenuVisible = !titleMenuVisible;
        ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
    }
    else if (user_event == MENU_EVENT_BACK)
    {
        titleMenuVisible = FALSE;
        ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
    }

    return ret;
}


// The general setup options

uint8_t MainMenu_setupGetSubCount(void)
{
    return setupMenuVisible ? 5 : 0;
}

void MainMenu_setupGetText(char *dest, uint8_t item)
{
    uint8_t i;

    switch (item)
    {
    case 0: // General setup
        util_strCpy_P(dest, setupString0);
        if (setupMenuVisible)
        {
            dest = util_strCpy_P(dest + 16, PStr_MinusParentheses);
        }
        else
        {
            dest = util_strCpy_P(dest + 16, PStr_PlusParentheses);
        }
        break;

    case 1: // In1
        dest = util_strCpy_P(dest, setupString1);

        i = MidiIo_getMode(MMSG_SOURCE_INPUT1, FALSE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 2: // In1 realtime:
        dest = util_strCpy_P(dest, setupString2);

        i = MidiIo_getMode(MMSG_SOURCE_INPUT1, TRUE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 3: // In2
        dest = util_strCpy_P(dest, setupString3);

        i = MidiIo_getMode(MMSG_SOURCE_INPUT2, FALSE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 4: // In2 realtime:
        dest = util_strCpy_P(dest, setupString4);

        i = MidiIo_getMode(MMSG_SOURCE_INPUT2, TRUE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 5: // Send runn. status
        dest = util_strCpy_P(dest, setupString5);

        if (MidiIo_getRunStatusMode())
        {
            dest = util_strCpy_P(dest, PStr_On);
        }
        else
        {
            dest = util_strCpy_P(dest, PStr_Off);
        }
        break;
    }

}


uint8_t MainMenu_setupHandleEvent(uint8_t item, uint8_t edit_mode,
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
                setupMenuVisible = !setupMenuVisible;
                ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
            }
            else
            {
                // Report back desired cursor position
                ret = cursorPositions[item];
            }
        }
    }
    else if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
    {
        int8_t i;

        // Report back desired cursor position
        ret = cursorPositions[item];

        switch (item)
        {
        case 1: // In1
            // Get current mode
            i = MidiIo_getMode(MMSG_SOURCE_INPUT1, FALSE);
            // The available modes are 0, 1, 2. Add the knob delta with bounds
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            // Set new mode
            MidiIo_setMode(i, MMSG_SOURCE_INPUT1, FALSE);
            break;

        case 2: // In1 realtime
            i = MidiIo_getMode(MMSG_SOURCE_INPUT1, TRUE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            MidiIo_setMode(i, MMSG_SOURCE_INPUT1, TRUE);
            break;

        case 3: // In2
            i = MidiIo_getMode(MMSG_SOURCE_INPUT2, FALSE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            MidiIo_setMode(i, MMSG_SOURCE_INPUT2, FALSE);
            break;

        case 4: // In2 realtime
            i = MidiIo_getMode(MMSG_SOURCE_INPUT2, TRUE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            MidiIo_setMode(i, MMSG_SOURCE_INPUT2, TRUE);
            break;

        case 5:
            // Set the running status
            MidiIo_setRunStatusMode(knob_delta > 0 ? TRUE : FALSE);
            break;
        }
    }

    return ret;
}


// Configuration store and load implementation

uint8_t MainMenu_configGetSize(void)
{
    return 5;
}

void MainMenu_configSave(uint8_t *dest)
{
    *(dest++) = MidiIo_getMode(MMSG_SOURCE_INPUT1, FALSE);
    *(dest++) = MidiIo_getMode(MMSG_SOURCE_INPUT1, TRUE);
    *(dest++) = MidiIo_getMode(MMSG_SOURCE_INPUT2, FALSE);
    *(dest++) = MidiIo_getMode(MMSG_SOURCE_INPUT2, TRUE);
    *(dest++) = MidiIo_getRunStatusMode();
}

void MainMenu_configLoad(uint8_t *dest)
{
    MidiIo_setMode(*(dest++), MMSG_SOURCE_INPUT1, FALSE);
    MidiIo_setMode(*(dest++), MMSG_SOURCE_INPUT1, TRUE);
    MidiIo_setMode(*(dest++), MMSG_SOURCE_INPUT2, FALSE);
    MidiIo_setMode(*(dest++), MMSG_SOURCE_INPUT2, TRUE);
    MidiIo_setRunStatusMode(*(dest++));
}
