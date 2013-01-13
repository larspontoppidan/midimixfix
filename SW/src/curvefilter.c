/*
 * curvefilter.c
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */


// Curve filter

#include "common.h"
#include "curvefilter.h"
#include "midimessage.h"
#include "util.h"
#include "menu.h"
#include "pgmstrings.h"
#include "curvemath.h"
#include <string.h>

//
// Allows shaping 7-bit values according to curve settings
//
// The curve rules have the following structure:
//
// Input:    In1 In2 In*
// Param.:   Note-On All, Note-On White, Note-On Black, Note-Off
//           Key AT, Chan AT,
// Shape:    Low point, Curve type, High point
//
// Neutral:              0   1  127
// To lift up minimum:   30  1  127
// To advance minimum:   -30 1  127
//
// To use a non-linear curve:  0   0.8  127  (resembling gamma 0.8)
// To use a non-linear curve:  0   1.3  127
// To use a non-linear curve:  0   S    127  (having a more steep center)
// To use a non-linear curve:  0   N    127  (having a more flat center)
//
// The available non-linear curves could be:
//
// "0.5", "0.8", "1.3", "2  ", "S", "N"
//
// The filter algorithm to be used live would be like:
//
// 1. Look up value in non-linear curve table, or use directly
// 2. Multiply gain  (fixed point)
// 3. Add offset
// 4. Bounded conversion to non-fixed point, 0..127
//



#define CURVEF_NOTE_ON        0
#define CURVEF_NOTE_ON_BLACK  1
#define CURVEF_NOTE_ON_WHITE  2
#define CURVEF_NOTE_OFF       3
#define CURVEF_KEY_AT         4
#define CURVEF_CHAN_AT        5
#define CURVEF_CTRL           6

#define CURVEF_MODES          7

char Curvef_ModeName[CURVEF_MODES][15] PROGMEM =
{
    {"N.On"},           // 0
    {"N.On BlackKeys"}, // 1
    {"N.On WhiteKeys"}, // 2
    {"N.Off"},          // 3
    {"Key AfterTch."},  // 4
    {"Chan.AfterTch."}, // 5
    {"CC:"},            // 6
};

uint8_t Curvef_ModeMidiStatus[CURVEF_MODES] PROGMEM =
{
        MIDI_STATUS_NOTE_ON,
        MIDI_STATUS_NOTE_ON,
        MIDI_STATUS_NOTE_ON,
        MIDI_STATUS_NOTE_OFF,
        MIDI_STATUS_KEY_ATOUCH,
        MIDI_STATUS_CHAN_ATOUCH,
        MIDI_STATUS_CTRL_CHANGE
};


typedef struct
{
    // The source to filter
    int8_t source;

    // The parameter to work with
    int8_t mode;

    // Optimized for the ISR: The midi_status to work corresponding to mode
    uint8_t midi_status;

    // Controller number, if midi ctl such:
    uint8_t ctrl_no;

    // The curve math
    curvemath_t curve;

} curvef_t;


// Variables

bool_t cfEnabled;
char   cfTitle[]  PROGMEM = "Curve filter ";

uint8_t cfManageMode;

#define CF_CURVE_MAX 8

uint8_t cfCurves;
curvef_t cfCurve[CF_CURVE_MAX];


void curvef_Initialize(void)
{
    cfCurves = 0;
    cfEnabled = FALSE;
}

void curvef_MessageIsrHook(mmsg_t *msg)
{
    if (cfEnabled != FALSE)
    {
        // Check that OK flag is on, DISCARD flag is off, RAW flag is off
        if ((msg->flags & (MMSG_FLAG_MSG_OK | MMSG_FLAG_DISCARD | MMSG_FLAG_RAW)) == MMSG_FLAG_MSG_OK)
        {
            uint8_t i;

            for (i = 0; i < cfCurves; i++)
            {
                // Check source is what we want
                if ((msg->flags & cfCurve[i].source) != 0)
                {
                    // Does this curve apply to message?
                    if ((msg->midi_status & MIDI_STATUS_MASK) == cfCurve[i].midi_status)
                    {
                        // Apparently! Look into the finer details and set
                        // what byte of message to process

                        uint8_t apply_curve = 0;

                        switch (cfCurve[i].mode)
                        {
                        case CURVEF_NOTE_ON:
                        case CURVEF_NOTE_OFF:
                        case CURVEF_KEY_AT:
                            // Work on data byte 2
                            apply_curve = 2;
                            break;

                        case CURVEF_NOTE_ON_BLACK:
                            // This curve is only for black keys
                            if (mmsg_IsKeyBlack(msg->midi_data[0]))
                            {
                                apply_curve = 2;
                            }
                            break;

                        case CURVEF_NOTE_ON_WHITE:
                            // This curve is only for white keys
                            if (mmsg_IsKeyBlack(msg->midi_data[0]) == FALSE)
                            {
                                apply_curve = 2;
                            }
                            break;

                        case CURVEF_CHAN_AT:
                            // Work on byte 1
                            apply_curve = 1;
                            break;

                        case CURVEF_CTRL:
                            // This curve is for a certain controller
                            if (msg->midi_data[0] == cfCurve[i].ctrl_no)
                            {
                                // And it is correct
                                apply_curve = 2;
                            }
                            break;
                        }

                        if (apply_curve != 0)
                        {
                            // We are still happy about this, then do it
                            msg->midi_data[apply_curve - 1] = curvem_ApplyCurve(
                                    msg->midi_data[apply_curve - 1],
                                    &(cfCurve[i].curve));
                        }
                    }
                }
            }
        }
    }

}

void curvef_MainLoopHook(void)
{

}

uint8_t curvef_GetSubMenuCount(void)
{
    return cfEnabled ? ((cfCurves * 2) + 1) : 0;
}

char *curvef_RenderSubMenu(char *dest, uint8_t i)
{
    uint8_t c = i >> 1;

    if ((i & 1) == 0)
    {
        // First line of a curve setup "InX ..."
        dest = pstr_WriteInX(dest, cfCurve[c].source);
        (*dest++) = ' ';

        dest = util_StrCpy_P(dest, Curvef_ModeName[cfCurve[c].mode]);

        // If this is a controller, also write name of controller
        if (cfCurve[c].mode == CURVEF_CTRL)
        {
            dest = mmsg_WriteControllerName(dest, cfCurve[c].ctrl_no);
        }
    }
    else
    {
        // Second line of a curve setup. Write the curve math specs
        // " -127 2.0 -127"
        (*dest++) = ' ';
        dest = curvem_WriteLow(dest, &(cfCurve[c].curve));
        (*dest++) = ' ';
        dest = curvem_WriteType(dest, &(cfCurve[c].curve));
        (*dest++) = ' ';
        dest = curvem_WriteHigh(dest, &(cfCurve[c].curve));
    }

    return dest;
}

void curvef_AddCurve(void)
{
    // Add new curve.
    if (cfCurves < CF_CURVE_MAX)
    {
        // Set default values
        cfCurve[cfCurves].source = 1;
        cfCurve[cfCurves].ctrl_no = 0;
        cfCurve[cfCurves].mode = CURVEF_NOTE_ON;
        cfCurve[cfCurves].midi_status = MIDI_STATUS_NOTE_ON;
        curvem_ResetCurve(&(cfCurve[cfCurves].curve));
        cfCurves++;
    }
}

void curvef_RemoveCurve(void)
{
    if (cfCurves > 0)
    {
        cfCurves--;
    }
}

void curvef_GetMenuText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        dest = util_StrCpy_P(dest, cfTitle);

        if (cfEnabled)
        {
            util_StrCpy_P(dest, pstr_On);
        }
        else
        {
            util_StrCpy_P(dest, pstr_Off);
        }
    }
    else if (item == ((cfCurves * 2) + 1))
    {
        dest = util_StrCpy_P(dest, pstr_ManageEllipsis);

        if (cfManageMode == 1)
        {
            util_StrCpy_P(dest, pstr_Add);
        }
        else if (cfManageMode == 2)
        {
            util_StrCpy_P(dest, pstr_Remove);
        }
    }
    else
    {
        curvef_RenderSubMenu(dest, item - 1);
    }
}

uint8_t curvef_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    // Generally, we want the pushed turns to be x10
    if (user_event == MENU_EVENT_TURN_PUSH)
    {
        user_event = MENU_EVENT_TURN;
        knob_delta *= 10;
    }

    if (item == 0)
    {
        if (edit_mode == 0)
        {
            if (user_event == MENU_EVENT_SELECT)
            {
                // We are at root level and user wants to edit title menu

                // Ok, move cursor to ON / OFF
                ret = 13;
            }
        }
        else if (edit_mode == 1)
        {
            if (user_event == MENU_EVENT_TURN)
            {
                // Toggle on / off status
                cfEnabled = util_BoundedAddInt8(cfEnabled, 0, 1, knob_delta);

                // Keep cursor at position, and notify menu that this may alter our submenu
                ret = 13 | MENU_UPDATE_ALL;
            }
        }
    }
    else if (item == ((cfCurves * 2) + 1))
    {
        if (edit_mode == 0)
        {
            if (user_event == MENU_EVENT_SELECT)
            {
                // User starts to enter manage mode
                ret = 10;

                // Our mode is initially Add
                cfManageMode = 1;
            }
        }
        else if (edit_mode == 1)
        {
            ret = 10;

            if (user_event == MENU_EVENT_TURN)
            {
                // User is turning knob in manage mode
                cfManageMode = util_BoundedAddInt8(cfManageMode, 1, 2, knob_delta);
            }
            else if (user_event == MENU_EVENT_SELECT)
            {
                // User is selecting current manage mode
                if (cfManageMode == 1)
                {
                    curvef_AddCurve();
                    cfManageMode = 0;
                    ret = MENU_EDIT_MODE_UNAVAIL | MENU_UPDATE_ALL;
                }
                else if (cfManageMode == 2)
                {
                    curvef_RemoveCurve();
                    cfManageMode = 0;
                    ret = MENU_EDIT_MODE_UNAVAIL | MENU_UPDATE_ALL;
                }
            }
        }
    }
    else
    {
        uint8_t c = (item - 1) >> 1;

        if ((item & 1) == 1)
        {
            // We are highlighting curve source / mode settings
            if (user_event == MENU_EVENT_SELECT)
            {
                if (edit_mode == 0)
                {
                    // User enters first edit mode
                    ret = 0;
                }
                else if (edit_mode == 1)
                {
                    // User enters second edit mode
                    ret = 4;
                }
                else if (edit_mode == 2)
                {
                    // Allow this if it is a controller curve
                    if (cfCurve[c].mode == CURVEF_CTRL)
                    {
                        ret = 7;
                    }
                }
            }
            else if (user_event == MENU_EVENT_TURN)
            {
                if (edit_mode == 1)
                {
                    ret = 0;
                    cfCurve[c].source =
                            util_BoundedAddInt8(cfCurve[c].source, 1, 3, knob_delta);
                }
                else if (edit_mode == 2)
                {
                    ret = 4;
                    cfCurve[c].mode =
                        util_BoundedAddInt8(cfCurve[c].mode, 0, (CURVEF_MODES-1), knob_delta);
                    cfCurve[c].midi_status = Curvef_ModeMidiStatus[cfCurve[c].mode];
                }
                else if (edit_mode == 3)
                {
                    ret = 7;
                    cfCurve[c].mode = CURVEF_CTRL;
                    cfCurve[c].midi_status = MIDI_STATUS_CTRL_CHANGE;
                    cfCurve[c].ctrl_no = (cfCurve[c].ctrl_no + knob_delta) & 127;
                }
            }
        }
        else
        {
            // We are highlighting curve specs settings. Always three modes here
            if (user_event == MENU_EVENT_SELECT)
            {
                if (edit_mode == 0)
                {
                    ret = 1;
                }
                else if (edit_mode == 1)
                {
                    ret = 5;
                }
                else if (edit_mode == 2)
                {
                    ret = 9;
                }
            }
            else if (user_event == MENU_EVENT_TURN)
            {
                if (edit_mode == 1)
                {
                    ret = 1;
                    cfCurve[c].curve.offset += knob_delta;
                }
                else if (edit_mode == 2)
                {
                    ret = 5;
                    cfCurve[c].curve.curve_type =
                        util_BoundedAddInt8(cfCurve[c].curve.curve_type, 0, CURVEM_TYPES-1, knob_delta);
                }
                else if (edit_mode == 3)
                {
                    ret = 9;
                    cfCurve[c].curve.gain += (int16_t)knob_delta;
                }
            }
        }
    }

    return ret;
}

// Configuration store and load implementation

uint8_t curvef_ConfigGetSize(void)
{
    return 2 + CF_CURVE_MAX * sizeof(curvef_t);
}

void curvef_ConfigSave(uint8_t *dest)
{
    *(dest++) = cfEnabled;
    *(dest++) = cfCurves;

    memcpy(dest, &(cfCurve[0]), CF_CURVE_MAX * sizeof(curvef_t));
}

void curvef_ConfigLoad(uint8_t *dest)
{
    cfEnabled = *(dest++);
    cfCurves = *(dest++);

    memcpy(&(cfCurve[0]), dest, CF_CURVE_MAX * sizeof(curvef_t));
}
