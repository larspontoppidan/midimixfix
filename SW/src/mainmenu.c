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

#include "midiio.h"

char mainmenu_Title[] PROGMEM = "- MIDIMIXFIX ";

char mainmenu_Setup[]  PROGMEM = "IO setup ";
char mainmenu_Setup1[] PROGMEM = "In1       :";
char mainmenu_Setup2[] PROGMEM = "In1 rt.msg:";
char mainmenu_Setup3[] PROGMEM = "In2       :";
char mainmenu_Setup4[] PROGMEM = "In2 rt.msg:";
char mainmenu_Setup5[] PROGMEM = "Send runn.st.:";

uint8_t mainmenu_Cursors[6] = {0, 11, 11, 11, 11, 14};


char *mainmenu_Modes[3] = {pstr_Discard, pstr_Through, pstr_Use};

bool_t mainSetupVisible = FALSE;

uint8_t mainmenu_TitleGetSubMenuCount(void)
{
    return 0;
}

void mainmenu_TitleGetMenuText(char *dest, uint8_t item)
{
    dest = util_StrCpy_P(dest, mainmenu_Title);

    dest = util_StrWriteInt8LA(dest, BUILD_VERSION_MAJOR);
    (*dest++) = '.';
    dest = util_StrWriteInt8LA(dest, BUILD_VERSION_MINOR);
    (*dest++) = ' ';
    (*dest++) = '-';
}

uint8_t mainmenu_TitleMenuEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta)
{
    // TODO Show about screen?
    return MENU_EDIT_MODE_UNAVAIL;
}


// The general setup options

uint8_t mainmenu_SetupGetSubMenuCount(void)
{
    return mainSetupVisible ? 5 : 0;
}

void mainmenu_SetupGetMenuText(char *dest, uint8_t item)
{
    uint8_t i;

    switch (item)
    {
    case 0: // General setup
        dest = util_StrCpy_P(dest, mainmenu_Setup);
        if (mainSetupVisible)
        {
            dest = util_StrCpy_P(dest, pstr_HideParentheses);
        }
        else
        {
            dest = util_StrCpy_P(dest, pstr_ShowParentheses);
        }
        break;

    case 1: // In1
        dest = util_StrCpy_P(dest, mainmenu_Setup1);

        i = midiio_GetMode(MMSG_SOURCE_INPUT1, FALSE);
        dest = util_StrCpy_P(dest, mainmenu_Modes[i]);

        break;

    case 2: // In1 realtime:
        dest = util_StrCpy_P(dest, mainmenu_Setup2);

        i = midiio_GetMode(MMSG_SOURCE_INPUT1, TRUE);
        dest = util_StrCpy_P(dest, mainmenu_Modes[i]);

        break;

    case 3: // In2
        dest = util_StrCpy_P(dest, mainmenu_Setup3);

        i = midiio_GetMode(MMSG_SOURCE_INPUT2, FALSE);
        dest = util_StrCpy_P(dest, mainmenu_Modes[i]);

        break;

    case 4: // In2 realtime:
        dest = util_StrCpy_P(dest, mainmenu_Setup4);

        i = midiio_GetMode(MMSG_SOURCE_INPUT2, TRUE);
        dest = util_StrCpy_P(dest, mainmenu_Modes[i]);

        break;

    case 5: // Send runn. status
        dest = util_StrCpy_P(dest, mainmenu_Setup5);

        if (midiio_GetSendRunStatus())
        {
            dest = util_StrCpy_P(dest, pstr_On);
        }
        else
        {
            dest = util_StrCpy_P(dest, pstr_Off);
        }
        break;
    }

}


uint8_t mainmenu_SetupMenuEvent(uint8_t item, uint8_t edit_mode,
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
                mainSetupVisible = !mainSetupVisible;
                ret = MENU_UPDATE_ALL | MENU_EDIT_MODE_UNAVAIL;
            }
            else
            {
                // Report back desired cursor position
                ret = mainmenu_Cursors[item];
            }
        }
    }
    else if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
    {
        int8_t i;

        // Report back desired cursor position
        ret = mainmenu_Cursors[item];

        switch (item)
        {
        case 1: // In1
            // Get current mode
            i = midiio_GetMode(MMSG_SOURCE_INPUT1, FALSE);
            // The available modes are 0, 1, 2. Add the knob delta with bounds
            i = util_BoundedAddInt8(i, 0, 2, knob_delta);
            // Set new mode
            midiio_SetMode(i, MMSG_SOURCE_INPUT1, FALSE);
            break;

        case 2: // In1 realtime
            i = midiio_GetMode(MMSG_SOURCE_INPUT1, TRUE);
            i = util_BoundedAddInt8(i, 0, 2, knob_delta);
            midiio_SetMode(i, MMSG_SOURCE_INPUT1, TRUE);
            break;

        case 3: // In2
            i = midiio_GetMode(MMSG_SOURCE_INPUT2, FALSE);
            i = util_BoundedAddInt8(i, 0, 2, knob_delta);
            midiio_SetMode(i, MMSG_SOURCE_INPUT2, FALSE);
            break;

        case 4: // In2 realtime
            i = midiio_GetMode(MMSG_SOURCE_INPUT2, TRUE);
            i = util_BoundedAddInt8(i, 0, 2, knob_delta);
            midiio_SetMode(i, MMSG_SOURCE_INPUT2, TRUE);
            break;

        case 5:
            // Set the running status
            midiio_SetSendRunStatus(knob_delta > 0 ? TRUE : FALSE);
            break;
        }
    }

    return ret;
}
