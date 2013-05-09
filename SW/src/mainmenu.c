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


static char *modeStrings[3] = {pstr_Discard, pstr_Through, pstr_Use};

static bool_t setupMenuVisible = FALSE;

static bool_t titleMenuVisible = FALSE;

uint8_t mmenu_TitleMenuGetSubCount(void)
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

void mmenu_TitleMenuGetText(char *dest, uint8_t item)
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

uint8_t mmenu_TitleMenuHandleEvent(uint8_t item, uint8_t edit_mode,
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

uint8_t mmenu_SetupMenuGetSubCount(void)
{
    return setupMenuVisible ? 5 : 0;
}

void mmenu_SetupMenuGetText(char *dest, uint8_t item)
{
    uint8_t i;

    switch (item)
    {
    case 0: // General setup
        util_strCpy_P(dest, setupString0);
        if (setupMenuVisible)
        {
            dest = util_strCpy_P(dest + 16, pstr_MinusParentheses);
        }
        else
        {
            dest = util_strCpy_P(dest + 16, pstr_PlusParentheses);
        }
        break;

    case 1: // In1
        dest = util_strCpy_P(dest, setupString1);

        i = midiio_ModeGet(MMSG_SOURCE_INPUT1, FALSE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 2: // In1 realtime:
        dest = util_strCpy_P(dest, setupString2);

        i = midiio_ModeGet(MMSG_SOURCE_INPUT1, TRUE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 3: // In2
        dest = util_strCpy_P(dest, setupString3);

        i = midiio_ModeGet(MMSG_SOURCE_INPUT2, FALSE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 4: // In2 realtime:
        dest = util_strCpy_P(dest, setupString4);

        i = midiio_ModeGet(MMSG_SOURCE_INPUT2, TRUE);
        dest = util_strCpy_P(dest, modeStrings[i]);

        break;

    case 5: // Send runn. status
        dest = util_strCpy_P(dest, setupString5);

        if (midiio_SendRunStatusGet())
        {
            dest = util_strCpy_P(dest, pstr_On);
        }
        else
        {
            dest = util_strCpy_P(dest, pstr_Off);
        }
        break;
    }

}


uint8_t mmenu_SetupMenuHandleEvent(uint8_t item, uint8_t edit_mode,
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
            i = midiio_ModeGet(MMSG_SOURCE_INPUT1, FALSE);
            // The available modes are 0, 1, 2. Add the knob delta with bounds
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            // Set new mode
            midiio_ModeSet(i, MMSG_SOURCE_INPUT1, FALSE);
            break;

        case 2: // In1 realtime
            i = midiio_ModeGet(MMSG_SOURCE_INPUT1, TRUE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            midiio_ModeSet(i, MMSG_SOURCE_INPUT1, TRUE);
            break;

        case 3: // In2
            i = midiio_ModeGet(MMSG_SOURCE_INPUT2, FALSE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            midiio_ModeSet(i, MMSG_SOURCE_INPUT2, FALSE);
            break;

        case 4: // In2 realtime
            i = midiio_ModeGet(MMSG_SOURCE_INPUT2, TRUE);
            i = util_boundedAddInt8(i, 0, 2, knob_delta);
            midiio_ModeSet(i, MMSG_SOURCE_INPUT2, TRUE);
            break;

        case 5:
            // Set the running status
            midiio_SendRunStatusSet(knob_delta > 0 ? TRUE : FALSE);
            break;
        }
    }

    return ret;
}


// Configuration store and load implementation

uint8_t mmenu_ConfigGetSize(void)
{
    return 5;
}

void mmenu_ConfigSave(uint8_t *dest)
{
    *(dest++) = midiio_ModeGet(MMSG_SOURCE_INPUT1, FALSE);
    *(dest++) = midiio_ModeGet(MMSG_SOURCE_INPUT1, TRUE);
    *(dest++) = midiio_ModeGet(MMSG_SOURCE_INPUT2, FALSE);
    *(dest++) = midiio_ModeGet(MMSG_SOURCE_INPUT2, TRUE);
    *(dest++) = midiio_SendRunStatusGet();
}

void mmenu_ConfigLoad(uint8_t *dest)
{
    midiio_ModeSet(*(dest++), MMSG_SOURCE_INPUT1, FALSE);
    midiio_ModeSet(*(dest++), MMSG_SOURCE_INPUT1, TRUE);
    midiio_ModeSet(*(dest++), MMSG_SOURCE_INPUT2, FALSE);
    midiio_ModeSet(*(dest++), MMSG_SOURCE_INPUT2, TRUE);
    midiio_SendRunStatusSet(*(dest++));
}
