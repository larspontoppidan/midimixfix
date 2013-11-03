//
// Filename    : curvefilter.c
// Code module : Filter for transforming continuous controller message values
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2013, Lars Ole Pontoppidan (Larsp)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// *  Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// *  Neither the name of the original author (Lars Ole Pontoppidan) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//
// ------------------------------  DESCRIPTION  ---------------------------------
//
// Allows shaping 7-bit values according to curve settings
//
// ...................
// Curve 127->127 2>2
//  On   : N.On Wh.Key
//  Low  : 0
//  High : 127
//  Shape: Linear
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

// -------------------------------  INCLUDES  -----------------------------------

#include "midiout.h"
#include "../midilog.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../errors.h"
#include "../hal.h"
#include "../util.h"
#include "../filters.h"
#include "../ui.h"
#include "../menus/filtermenu.h"
#include "curvemath.h"
#include <avr/pgmspace.h>
#include "filteruids.h"

// ------------------------------  PROTOTYPES  ----------------------------------


static uint8_t Create(uint8_t filter_step);
static void    Destroy(uint8_t instance);
static void    SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    LoadConfig(uint8_t instance, void* data);
static void    SaveConfig(uint8_t instance, void* data);
static uint8_t ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);

static void DeriveFilterStatus(uint8_t instance);

// --------------------------  TYPES AND CONSTANTS  -----------------------------

// The filter mode has different meaning in different intervals.
// The first FILTER_NON_CC_MODES have special meanings:

#define FILTER_NOTE_ON        0
#define FILTER_NOTE_ON_BLACK  1
#define FILTER_NOTE_ON_WHITE  2
#define FILTER_NOTE_OFF       3
#define FILTER_KEY_AT         4
#define FILTER_CHAN_AT        5

#define FILTER_NON_CC_MODES   6

// After the special modes comes the UiCc controller range,
// thus the highest allowed filter mode is
#define FILTER_MODES    (FILTER_NON_CC_MODES + MIDI_UICC_COUNT)


static char const FilterNonCcModeNames[FILTER_NON_CC_MODES][12] PROGMEM =
{
    {"N.On"},         // 0
    {"N.On Bl.Key"},  // 1
    {"N.On Wh.Key"},  // 2
    {"N.Off"},        // 3
    {"Key AfterT."},  // 4
    {"Ch. AfterT."},  // 5
};

static uint8_t const FilterNonCcModeStatusses[FILTER_NON_CC_MODES] PROGMEM =
{
        MIDI_STATUS_NOTE_ON,
        MIDI_STATUS_NOTE_ON,
        MIDI_STATUS_NOTE_ON,
        MIDI_STATUS_NOTE_OFF,
        MIDI_STATUS_KEY_ATOUCH,
        MIDI_STATUS_CHAN_ATOUCH
};

typedef struct
{
    // The parameter to work with
    uint8_t Mode;

    // The curve math
    curveMath_t Curve;

} curveFilter_t;


typedef struct
{
    uint8_t  LatestIn;
    bool_t   LatestInChanged;
    bool_t   InUse;
} curveInstance_t;


static char const StaticTitle[]   PROGMEM = "Curve filter";
static char const DynamicTitle1[] PROGMEM = "Curve %U->";
static char const DynamicTitle2[] PROGMEM = "%U";

static char const SettingCtrl[]  PROGMEM = "On   : ";
static char const SettingLow[]   PROGMEM = "Low  : ";
static char const SettingHigh[]  PROGMEM = "High : ";
static char const SettingCurve[] PROGMEM = "Curve: ";

#define FILTER_COUNT_MAX 6

const filterInterface_t curvefilter_Filter PROGMEM =
{
        Create,
        Destroy,
        SetFilterStep,
        LoadConfig,
        SaveConfig,
        ProcessMidiMsg,
        WriteMenuText,
        HandleUiEvent,

        sizeof(curveFilter_t), // Number of bytes in configuration
        4,              // Number of menu items (0 means only title, 1 is one item)
        7,              // Cursor indentation in menu
        FILTER_MODE_PROCESSOR, // Filter operation mode
        StaticTitle,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_CURVE_FILTER, 1)
};

// -------------------------------  VARIABLES  ----------------------------------

static curveFilter_t   Filters[FILTER_COUNT_MAX];
static curveInstance_t Instances[FILTER_COUNT_MAX];
static uint8_t         FilterCount;

// The midi_status to work with corresponding to mode
// Derived from curveFilter_t settings to simplify ISR implementation
static uint8_t FilterMidiStatus[FILTER_COUNT_MAX];

// --------------------------  PRIVATE FUNCTIONS  ------------------------------


static void DeriveFilterStatus(uint8_t instance)
{
    if (Filters[instance].Mode >= FILTER_NON_CC_MODES)
    {
        FilterMidiStatus[instance] = MIDI_STATUS_CTRL_CHANGE;
    }
    else
    {
        FilterMidiStatus[instance] = pgm_read_byte(
                &(FilterNonCcModeStatusses[Filters[instance].Mode]));
    }
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void curveflt_initialize(void)
{
    uint8_t i;

    FilterCount = 0;

    // Set default curves in all slots
    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        Filters[i].Mode = FILTER_NOTE_ON;
        curvemath_reset(&(Filters[i].Curve));
        Instances[i].InUse = FALSE;
        DeriveFilterStatus(i);
    }
}

void curveflt_handleMainLoop(void)
{
    uint8_t i;
    filters_instance_t fi;
    fi.FilterType = FILTER_TYPE_CURVE_FILTER;

    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        if (Instances[i].LatestInChanged)
        {
            Instances[i].LatestInChanged = FALSE;

            fi.Instance = i;
            filtermenu_RequestUpdate(fi, 0);
        }
    }
}

// -----------------------  FILTER INTERFACE FUNCTIONS  -----------------------------


static uint8_t Create(uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;
    uint8_t i;

    (void)(filter_step);

    // Find free instance
    for (i = 0; i < FILTER_COUNT_MAX; i++)
    {
        if (Instances[i].InUse == FALSE)
        {
            break;
        }
    }

    if (i < FILTER_COUNT_MAX)
    {
        // Okay, lets do this
        Instances[i].LatestIn = 0;
        Instances[i].InUse = TRUE;

        // Return instance number
        ret = i;
    }

    return ret;
}

static void Destroy(uint8_t instance)
{
    Instances[instance].InUse = FALSE;
}

static void SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    (void)(filter_step);
}

static void LoadConfig(uint8_t instance, void* data)
{
    Filters[instance] = *((curveFilter_t *)data);
    DeriveFilterStatus(instance);
}

static void SaveConfig(uint8_t instance, void* data)
{
    *((curveFilter_t *)data) = Filters[instance];
}

static uint8_t ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Does this curve apply to message?
    if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == FilterMidiStatus[instance])
    {
        // Yes, look into the finer details and set
        // what byte of message to process

        uint8_t apply_curve = 0;

#if 0 == MIDIMSG_DATA1
#error This implementation assumes MIDIMSG_DATA1 is different from 0
#elif 0 == MIDIMSG_DATA2
#error This implementation assumes MIDIMSG_DATA2 is different from 0
#endif

        if (Filters[instance].Mode >= FILTER_NON_CC_MODES)
        {
            // This is a controller, is it the correct one?
            if (msg->Data[MIDIMSG_DATA1] == midi_convertUiccToCc(
                    Filters[instance].Mode - FILTER_NON_CC_MODES))
            {
                // And it is correct
                apply_curve = MIDIMSG_DATA2;
            }
        }
        else
        {
            switch (Filters[instance].Mode)
            {
            case FILTER_NOTE_ON:
            case FILTER_NOTE_OFF:
            case FILTER_KEY_AT:
                // Work on data byte 2
                apply_curve = MIDIMSG_DATA2;
                break;

            case FILTER_NOTE_ON_BLACK:
                // This curve is only for black keys
                if (midi_isKeyBlack(msg->Data[MIDIMSG_DATA1]))
                {
                    apply_curve = MIDIMSG_DATA2;
                }
                break;

            case FILTER_NOTE_ON_WHITE:
                // This curve is only for white keys
                if (midi_isKeyBlack(msg->Data[MIDIMSG_DATA1]) == FALSE)
                {
                    apply_curve = MIDIMSG_DATA2;
                }
                break;

            case FILTER_CHAN_AT:
                // Work on byte 1
                apply_curve = MIDIMSG_DATA1;
                break;
            }
        }

        if (apply_curve != 0)
        {
            // We are still happy about this, then do it
            uint8_t x = msg->Data[apply_curve];

            Instances[instance].LatestIn = x;
            Instances[instance].LatestInChanged = TRUE;
            x = curvemath_apply(x, &(Filters[instance].Curve));
            msg->Data[apply_curve] = x;

            ret = FILTER_PROCESS_DID;
        }
    }

    // (OBSOLETE) Profiling: Inactive filter cost: 2.84 us
    //                       Transforming black keys: 14 us  (worst case)

    return ret;
}

// Menu integration
static void WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    uint8_t x;

    switch (menu_item)
    {
    case 0: // Write dynamic title including the latest actual input and output of filter
        x = Instances[instance].LatestIn;
        dest = util_writeFormat_P(dest, DynamicTitle1, x);
        util_writeFormat_P(dest, DynamicTitle2, curvemath_apply(x, &(Filters[instance].Curve)));
        break;

    case 1: // Set controller menu item
        dest = util_copyString_P(dest, SettingCtrl);

        // If this is a controller, also write name of controller
        if (Filters[instance].Mode < FILTER_NON_CC_MODES)
        {
            dest = util_copyString_P(dest, FilterNonCcModeNames[Filters[instance].Mode]);
        }
        else
        {
            dest = midi_writeUiccName(dest, Filters[instance].Mode - FILTER_NON_CC_MODES);
        }
        break;

    case 2:
        dest = util_copyString_P(dest, SettingLow);
        dest = curvemath_writeLow(dest, &(Filters[instance].Curve));
        break;
    case 3:
        dest = util_copyString_P(dest, SettingHigh);
        dest = curvemath_writeHigh(dest, &(Filters[instance].Curve));
        break;
    case 4:
        dest = util_copyString_P(dest, SettingCurve);
        dest = curvemath_writeType(dest, &(Filters[instance].Curve));
        break;
    }
}


static void HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    int8_t delta = ui_eventToDelta(ui_event, 10);

    filters_instance_t fi;
    fi.FilterType = FILTER_TYPE_CURVE_FILTER;
    fi.Instance = instance;

    switch (menu_item)
    {
    case 1:
        Filters[instance].Mode =
             util_boundedAddUint8(Filters[instance].Mode, 0, FILTER_MODES - 1, delta);
        DeriveFilterStatus(instance);
        break;

    case 2:
        Filters[instance].Curve.Offset =
             util_boundedAddInt8(Filters[instance].Curve.Offset, -128, 127, delta);
        filtermenu_RequestUpdate(fi, 3);
        break;

    case 3:
        Filters[instance].Curve.Gain += delta;
        filtermenu_RequestUpdate(fi, 2);
        break;
    case 4:
        Filters[instance].Curve.Type =
            util_boundedAddUint8(Filters[instance].Curve.Type, 0, CURVEMATH_TYPES - 1, delta);
        break;
    }

    // Just update menu 0 in any case
    filtermenu_RequestUpdate(fi, 0);
}

