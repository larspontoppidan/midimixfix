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
#include "midigenerics.h"
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

static char const FilterModeNames[FILTER_MODES][15] PROGMEM =
{
    {"N.On"},           // 0
    {"N.On BlackKeys"}, // 1
    {"N.On WhiteKeys"}, // 2
    {"N.Off"},          // 3
    {"Key AfterTch."},  // 4
    {"Chan.AfterTch."}, // 5
    {"CC:"},            // 6
};

static uint8_t const FilterModeStatusses[FILTER_MODES] PROGMEM =
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
    int8_t Source;

    // The parameter to work with
    int8_t Mode;

    // Controller number, if midi ctl such:
    uint8_t CtrlNo;

    // The curve math
    curveMath_t Curve;

} curveFilter_t;


// Variables

static char const TitleString[] PROGMEM = "Curve filter ";

#define FILTER_COUNT_MAX 8

static uint8_t FilterCount;
static curveFilter_t Filters[FILTER_COUNT_MAX];

// The midi_status to work with corresponding to mode
// Derived from cfCurve settings to optimize ISR implementation
static uint8_t FilterMidiStatus[FILTER_COUNT_MAX];


void curveflt_initialize(void)
{
    uint8_t i;

    FilterCount = 0;

    // Set default curves in all slots
    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        Filters[i].Source = 1;
        Filters[i].CtrlNo = 0;
        Filters[i].Mode = FILTER_NOTE_ON;
        FilterMidiStatus[i] = MIDI_STATUS_NOTE_ON;
        curvemath_reset(&(Filters[i].Curve));
    }

}

void curveflt_handleMidiMsg_ISR(midiMsg_t *msg)
{
    if (FilterCount != 0)
    {
        // Check that OK flag is on, DISCARD flag is off, RAW flag is off
        if ((msg->Flags & (MIDIMSG_FLAG_MSG_OK | MIDIMSG_FLAG_DISCARD | MIDIMSG_FLAG_RAW)) == MIDIMSG_FLAG_MSG_OK)
        {
            uint8_t i;

            for (i = 0; i < FilterCount; i++)
            {
                // Check source is what we want
                if ((msg->Flags & Filters[i].Source) != 0)
                {
                    // Does this curve apply to message?
                    if ((msg->MidiStatus & MIDI_STATUS_MASK) == FilterMidiStatus[i])
                    {
                        // Apparently! Look into the finer details and set
                        // what byte of message to process

                        uint8_t apply_curve = 0;

                        switch (Filters[i].Mode)
                        {
                        case FILTER_NOTE_ON:
                        case FILTER_NOTE_OFF:
                        case FILTER_KEY_AT:
                            // Work on data byte 2
                            apply_curve = 2;
                            break;

                        case FILTER_NOTE_ON_BLACK:
                            // This curve is only for black keys
                            if (midi_isKeyBlack(msg->Data[0]))
                            {
                                apply_curve = 2;
                            }
                            break;

                        case FILTER_NOTE_ON_WHITE:
                            // This curve is only for white keys
                            if (midi_isKeyBlack(msg->Data[0]) == FALSE)
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
                            if (msg->Data[0] == Filters[i].CtrlNo)
                            {
                                // And it is correct
                                apply_curve = 2;
                            }
                            break;
                        }

                        if (apply_curve != 0)
                        {
                            // We are still happy about this, then do it
                            msg->Data[apply_curve - 1] = curvemath_apply(
                                    msg->Data[apply_curve - 1],
                                    &(Filters[i].Curve));
                        }
                    }
                }
            }
        }
    }

    // Profiling: Inactive filter cost: 2.84 us
    //            Transforming black keys: 14 us  (worst case)
}

void curveflt_handleMainLoop(void)
{

}

uint8_t curveflt_menuGetSubCount(void)
{
    return FilterCount * 2;
}

void curveflt_menuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        util_copyString_P(dest, TitleString);
        util_writeNumberParentheses(dest + 14, FilterCount);
    }
    else
    {
        uint8_t c = (item - 1) >> 1;

        if ((item & 1) == 1)
        {
            // First line of a curve setup "InX ..."
            dest = pstr_writeInX(dest, Filters[c].Source);
            (*dest++) = ' ';

            dest = util_copyString_P(dest, FilterModeNames[Filters[c].Mode]);

            // If this is a controller, also write name of controller
            if (Filters[c].Mode == FILTER_CTRL)
            {
                dest = midi_writeControllerName(dest, Filters[c].CtrlNo);
            }
        }
        else
        {
            // Second line of a curve setup. Write the curve math specs
            // " -127 2.0 -127"
            (*dest++) = ' ';
            dest = curvemath_writeLow(dest, &(Filters[c].Curve));
            (*dest++) = ' ';
            dest = curvemath_writeType(dest, &(Filters[c].Curve));
            (*dest++) = ' ';
            dest = curvemath_writeHigh(dest, &(Filters[c].Curve));
        }
    }
}

uint8_t curveflt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
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
                FilterCount = util_boundedAddInt8(FilterCount, 0, FILTER_COUNT_MAX, knob_delta);

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
                    if (Filters[c].Mode == FILTER_CTRL)
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
                    Filters[c].Source =
                            util_boundedAddInt8(Filters[c].Source, 1, 3, knob_delta);
                }
                else if (edit_mode == 2)
                {
                    ret = 4;
                    Filters[c].Mode =
                        util_boundedAddInt8(Filters[c].Mode, 0, (FILTER_MODES-1), knob_delta);
                    FilterMidiStatus[c] = pgm_read_byte(&(FilterModeStatusses[Filters[c].Mode]));
                }
                else if (edit_mode == 3)
                {
                    ret = 7;
                    Filters[c].Mode = FILTER_CTRL;
                    FilterMidiStatus[c] = MIDI_STATUS_CTRL_CHANGE;
                    Filters[c].CtrlNo = (Filters[c].CtrlNo + knob_delta) & 127;
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
                    Filters[c].Curve.Offset += knob_delta;
                }
                else if (edit_mode == 2)
                {
                    ret = 5;
                    Filters[c].Curve.Type =
                        util_boundedAddInt8(Filters[c].Curve.Type, 0, CURVEMATH_TYPES-1, knob_delta);
                }
                else if (edit_mode == 3)
                {
                    ret = 9;
                    Filters[c].Curve.Gain += (int16_t)knob_delta;
                }
            }
        }
    }

    return ret;
}

// Configuration store and load implementation

uint8_t curveflt_configGetSize(void)
{
    return 1 + FILTER_COUNT_MAX * sizeof(curveFilter_t);
}

void curveflt_configSave(uint8_t *dest)
{
    *(dest++) = FilterCount;

    memcpy(dest, &(Filters[0]), FILTER_COUNT_MAX * sizeof(curveFilter_t));
}

void curveflt_configLoad(uint8_t *dest)
{
    bool_t new_filterCount;
    uint8_t i;

    // Disable filter while we load the new settings
    FilterCount = 0;

    new_filterCount = *(dest++);

    memcpy(&(Filters[0]), dest, FILTER_COUNT_MAX * sizeof(curveFilter_t));

    // Set correct midi status according to modes
    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        FilterMidiStatus[i] = pgm_read_byte(&(FilterModeStatusses[Filters[i].Mode]));
    }

    FilterCount = new_filterCount;
}
