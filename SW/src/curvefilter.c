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



#define FILTER_NOTE_ON        0
#define FILTER_NOTE_ON_BLACK  1
#define FILTER_NOTE_ON_WHITE  2
#define FILTER_NOTE_OFF       3
#define FILTER_KEY_AT         4
#define FILTER_CHAN_AT        5
#define FILTER_CTRL           6

#define FILTER_MODES          7

static char filterModeNames[FILTER_MODES][15] PROGMEM =
{
    {"N.On"},           // 0
    {"N.On BlackKeys"}, // 1
    {"N.On WhiteKeys"}, // 2
    {"N.Off"},          // 3
    {"Key AfterTch."},  // 4
    {"Chan.AfterTch."}, // 5
    {"CC:"},            // 6
};

static uint8_t filterModeStatusses[FILTER_MODES] PROGMEM =
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

    // Controller number, if midi ctl such:
    uint8_t ctrlNo;

    // The curve math
    cmath_curve_t curve;

} curveFilter_t;


// Variables

static char   titleString[]  PROGMEM = "Curve filter ";

#define FILTER_COUNT_MAX 8

static uint8_t filterCount;
static curveFilter_t filters[FILTER_COUNT_MAX];

// The midi_status to work with corresponding to mode
// Derived from cfCurve settings to optimize ISR implementation
static uint8_t filterMidiStatus[FILTER_COUNT_MAX];


void curvef_Initialize(void)
{
    uint8_t i;

    filterCount = 0;

    // Set default curves in all slots
    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        filters[i].source = 1;
        filters[i].ctrlNo = 0;
        filters[i].mode = FILTER_NOTE_ON;
        filterMidiStatus[i] = MIDI_STATUS_NOTE_ON;
        cmath_CurveReset(&(filters[i].curve));
    }

}

void curvef_HookMidiMsg_ISR(mmsg_t *msg)
{
    if (filterCount != 0)
    {
        // Check that OK flag is on, DISCARD flag is off, RAW flag is off
        if ((msg->flags & (MMSG_FLAG_MSG_OK | MMSG_FLAG_DISCARD | MMSG_FLAG_RAW)) == MMSG_FLAG_MSG_OK)
        {
            uint8_t i;

            for (i = 0; i < filterCount; i++)
            {
                // Check source is what we want
                if ((msg->flags & filters[i].source) != 0)
                {
                    // Does this curve apply to message?
                    if ((msg->midi_status & MIDI_STATUS_MASK) == filterMidiStatus[i])
                    {
                        // Apparently! Look into the finer details and set
                        // what byte of message to process

                        uint8_t apply_curve = 0;

                        switch (filters[i].mode)
                        {
                        case FILTER_NOTE_ON:
                        case FILTER_NOTE_OFF:
                        case FILTER_KEY_AT:
                            // Work on data byte 2
                            apply_curve = 2;
                            break;

                        case FILTER_NOTE_ON_BLACK:
                            // This curve is only for black keys
                            if (mmsg_IsKeyBlack(msg->midi_data[0]))
                            {
                                apply_curve = 2;
                            }
                            break;

                        case FILTER_NOTE_ON_WHITE:
                            // This curve is only for white keys
                            if (mmsg_IsKeyBlack(msg->midi_data[0]) == FALSE)
                            {
                                apply_curve = 2;
                            }
                            break;

                        case FILTER_CHAN_AT:
                            // Work on byte 1
                            apply_curve = 1;
                            break;

                        case FILTER_CTRL:
                            // This curve is for a certain controller
                            if (msg->midi_data[0] == filters[i].ctrlNo)
                            {
                                // And it is correct
                                apply_curve = 2;
                            }
                            break;
                        }

                        if (apply_curve != 0)
                        {
                            // We are still happy about this, then do it
                            msg->midi_data[apply_curve - 1] = cmath_CurveApply(
                                    msg->midi_data[apply_curve - 1],
                                    &(filters[i].curve));
                        }
                    }
                }
            }
        }
    }

}

void curvef_HookMainLoop(void)
{

}

uint8_t curvef_MenuGetSubCount(void)
{
    return filterCount * 2;
}

void curvef_MenuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        util_strCpy_P(dest, titleString);
        util_strWriteNumberParentheses(dest + 14, filterCount);
    }
    else
    {
        uint8_t c = (item - 1) >> 1;

        if ((item & 1) == 1)
        {
            // First line of a curve setup "InX ..."
            dest = pstr_writeInX(dest, filters[c].source);
            (*dest++) = ' ';

            dest = util_strCpy_P(dest, filterModeNames[filters[c].mode]);

            // If this is a controller, also write name of controller
            if (filters[c].mode == FILTER_CTRL)
            {
                dest = mmsg_WriteControllerName(dest, filters[c].ctrlNo);
            }
        }
        else
        {
            // Second line of a curve setup. Write the curve math specs
            // " -127 2.0 -127"
            (*dest++) = ' ';
            dest = cmath_WriteLow(dest, &(filters[c].curve));
            (*dest++) = ' ';
            dest = cmath_WriteType(dest, &(filters[c].curve));
            (*dest++) = ' ';
            dest = cmath_WriteHigh(dest, &(filters[c].curve));
        }
    }
}

uint8_t curvef_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
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
                ret = 17;
            }
        }
        else if (edit_mode == 1)
        {
            if (user_event == MENU_EVENT_TURN)
            {
                // Modify filter count
                filterCount = util_boundedAddInt8(filterCount, 0, FILTER_COUNT_MAX, knob_delta);

                // Keep cursor at position, and notify menu that this may alter our submenu
                ret = 17 | MENU_UPDATE_ALL;
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
                    if (filters[c].mode == FILTER_CTRL)
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
                    filters[c].source =
                            util_boundedAddInt8(filters[c].source, 1, 3, knob_delta);
                }
                else if (edit_mode == 2)
                {
                    ret = 4;
                    filters[c].mode =
                        util_boundedAddInt8(filters[c].mode, 0, (FILTER_MODES-1), knob_delta);
                    filterMidiStatus[c] = pgm_read_byte(&(filterModeStatusses[filters[c].mode]));
                }
                else if (edit_mode == 3)
                {
                    ret = 7;
                    filters[c].mode = FILTER_CTRL;
                    filterMidiStatus[c] = MIDI_STATUS_CTRL_CHANGE;
                    filters[c].ctrlNo = (filters[c].ctrlNo + knob_delta) & 127;
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
                    filters[c].curve.offset += knob_delta;
                }
                else if (edit_mode == 2)
                {
                    ret = 5;
                    filters[c].curve.type =
                        util_boundedAddInt8(filters[c].curve.type, 0, CURVEM_TYPES-1, knob_delta);
                }
                else if (edit_mode == 3)
                {
                    ret = 9;
                    filters[c].curve.gain += (int16_t)knob_delta;
                }
            }
        }
    }

    return ret;
}

// Configuration store and load implementation

uint8_t curvef_ConfigGetSize(void)
{
    return 1 + FILTER_COUNT_MAX * sizeof(curveFilter_t);
}

void curvef_ConfigSave(uint8_t *dest)
{
    *(dest++) = filterCount;

    memcpy(dest, &(filters[0]), FILTER_COUNT_MAX * sizeof(curveFilter_t));
}

void curvef_ConfigLoad(uint8_t *dest)
{
    bool_t new_filterCount;
    uint8_t i;

    // Disable filter while we load the new settings
    filterCount = 0;

    new_filterCount = *(dest++);

    memcpy(&(filters[0]), dest, FILTER_COUNT_MAX * sizeof(curveFilter_t));

    // Set correct midi status according to modes
    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        filterMidiStatus[i] = pgm_read_byte(&(filterModeStatusses[filters[i].mode]));
    }

    filterCount = new_filterCount;
}
