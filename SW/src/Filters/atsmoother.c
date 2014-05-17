//
// Filename    : atsmoother.c
// Code module : Time smoothing for after touch filter
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2014, Lars Ole Pontoppidan (Larsp)
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
// Aftertouch Smoother has a state for each channel
//
// For each channel,
//
// Input:          NoteOn 100         ChanAt 50,   ChanAt 20,   ChanAt 100,    NoteOff
//
// Output:  Vol.100 NoteOn 100        Vol90, 80, 70, ..

// -------------------------------  INCLUDES  -----------------------------------

#include "../midiprocessing.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../errors.h"
#include "../util.h"
#include "../ui.h"
#include "atsmoother.h"
#include "../pgmstrings.h"
#include <avr/pgmspace.h>
#include "filteruids.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------



#define MESSAGE_TYPE_CHAN_AT     0
#define MESSAGE_TYPE_FIRST_UICC  1

#define START_RESERVED 0
#define START_N_ON     1
#define START_VALUE_0  2

#define SMOOTHE_MAX  100

#define TICK_DIVISION 3  // 100 Hz tick is divided with this

typedef struct
{
    uint8_t Start;
    uint8_t FadeIn;
    uint8_t FadeOut;
    uint8_t OutputCc;
} config_t;

// Filter state

typedef struct
{
    uint8_t NoteOnCount;
    uint16_t FilterState;     // Smooth filter for Key AfterTouch state
    uint8_t FilterTarget;     // Target value
    uint8_t LatestKeyAt;
} channel_state_t;

#define CHANNEL_MAX 9

typedef struct
{
    bool_t  InUse;
    uint8_t FilterStep;
    uint8_t TickDivider;
    channel_state_t ChannelState[CHANNEL_MAX];
} state_t;



static char const MenuTitle[]    PROGMEM = "AT. smoothing";

static char const MenuSetting1[] PROGMEM = "Start  : ";
static char const MenuSetting2[] PROGMEM = "FadeIn : %i";
static char const MenuSetting3[] PROGMEM = "FadeOut: %i";
static char const MenuSetting4[] PROGMEM = "Output : ";


// ------------------------------  PROTOTYPES  ----------------------------------

// Filter interface

static uint8_t atsmoother_Create(uint8_t filter_step);
static void    atsmoother_Destroy(uint8_t instance);
static void    atsmoother_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    atsmoother_LoadConfig(uint8_t instance, void* data);
static void    atsmoother_SaveConfig(uint8_t instance, void* data);
static uint8_t atsmoother_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    atsmoother_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    atsmoother_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// Channel management:

static void initialize(void);

static bool_t handleNoteOn(uint8_t channel, uint8_t vel);
static void handleNoteOff(uint8_t channel);
static void handleChanAt(uint8_t channel, uint8_t vel);
static void sendKeyAtIfChange(uint8_t channel);

static void makeOutputMsg(uint8_t channel, midiMsg_t *msg, uint8_t filter_out);

// Smooth filter

static void filterReset(uint8_t channel, uint8_t value);
static void filterNewTarget(uint8_t channel, uint8_t value);
static void filterUpdate(uint8_t channel);
static uint8_t filterGetOutput(uint8_t channel);


// -------------------------------  VARIABLES  ----------------------------------


const filterInterface_t atsmoother_Filter PROGMEM =
{
        atsmoother_Create,
        atsmoother_Destroy,
        atsmoother_SetFilterStep,
        atsmoother_LoadConfig,
        atsmoother_SaveConfig,
        atsmoother_ProcessMidiMsg,
        atsmoother_WriteMenuText,
        atsmoother_HandleUiEvent,

        sizeof(config_t),   // Number of bytes in configuration
        4,                  // Number of menu items (0 means only title, 1 is one item)
        9,                  // Cursor indentation in menu
        FILTER_MODE_PROCESSOR, // Filter operation mode
        MenuTitle,          // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_ATSMOOTHER_FILTER, 1)
};

static state_t  Instance;
static config_t Config;



// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void atsmoother_handleTick_ISR(void)
{
    if (Instance.TickDivider == 0)
    {
        if (Instance.InUse)
        {
            uint8_t i;

            for (i = 0; i < CHANNEL_MAX; i++)
            {
                if (Instance.ChannelState[i].NoteOnCount > 0)
                {
                    filterUpdate(i);
                    sendKeyAtIfChange(i);
                }
            }
        }

        Instance.TickDivider = TICK_DIVISION - 1;
    }
    else
    {
        Instance.TickDivider--;
    }
}



// ------------------------  FILTER INTERFACE FUNCTIONS  ------------------------

static uint8_t atsmoother_Create(uint8_t filter_step)
{
    uint8_t ret;

    if (Instance.InUse == FALSE)
    {
        // Reset state
        initialize();

        // Setup
        Instance.FilterStep = filter_step;
        Instance.InUse = TRUE;

        // Return some instance number. Won't be used as there can be only one instance
        ret = 0;
    }
    else
    {
        ret = FILTER_CREATE_FAILED;
    }

    return ret;
}

static void atsmoother_Destroy(uint8_t instance)
{
    UNUSED(instance);
    Instance.InUse = FALSE;
}

static void atsmoother_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    UNUSED(instance);

    if (Instance.InUse)
    {
        Instance.FilterStep = filter_step;
    }
}

static void atsmoother_LoadConfig(uint8_t instance, void* data)
{
    UNUSED(instance);
    Config = *((config_t*)data);
}

static void atsmoother_SaveConfig(uint8_t instance, void* data)
{
    UNUSED(instance);
    *((config_t*)data) = Config;
}

static uint8_t atsmoother_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    UNUSED(instance);

    // Deal with this message if status is something we are interested in
    uint8_t chan = msg->Data[MIDIMSG_STATUS] & MIDI_CHANNEL_MASK;

    switch (msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK)
    {
    case MIDI_STATUS_NOTE_ON:
        // If this is first note on in channel, we might want to precede it with a
        // control value setting
        if (handleNoteOn(chan, msg->Data[MIDIMSG_DATA2]))
        {
            // Move this message to the future by posting it again:
            midiproc_addMessage_ISR(msg, Instance.FilterStep);

            // and put our control change message here
            uint8_t filter_out = filterGetOutput(chan);
            Instance.ChannelState[chan].LatestKeyAt = filter_out;
            makeOutputMsg(chan, msg, filter_out);

            ret = FILTER_PROCESS_DID;
        }
        break;

    case MIDI_STATUS_NOTE_OFF:
        handleNoteOff(chan);
        break;

    case MIDI_STATUS_CHAN_ATOUCH:
        handleChanAt(chan, msg->Data[MIDIMSG_DATA1]);
        ret = FILTER_PROCESS_DISCARD;
        break;
    }

    return ret;
}


// Menu integration
static void atsmoother_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    UNUSED(instance);

    switch (menu_item)
    {
    case 0:
        util_copyString_P(dest, MenuTitle);
        break;
    case 1:
        dest = util_copyString_P(dest, MenuSetting1);
        if (Config.Start == START_N_ON)
        {
            util_copyString_P(dest, PSTR("N.On"));
        }
        else if (Config.Start == START_RESERVED)
        {
            util_copyString_P(dest, PSTR("N/A"));
        }
        else
        {
            util_writeInt8LA(dest, Config.Start - START_VALUE_0);
        }
        break;
    case 2:
        util_writeFormat_P(dest, MenuSetting2, Config.FadeIn);
        break;
    case 3:
        util_writeFormat_P(dest, MenuSetting3, Config.FadeOut);
        break;
    case 4:
        dest = util_copyString_P(dest, MenuSetting4);
        if (Config.OutputCc == MESSAGE_TYPE_CHAN_AT)
        {
            midi_writeStatusName(dest, MIDI_STATUS_CHAN_ATOUCH);
        }
        else
        {
            midi_writeUiccName(dest, Config.OutputCc - MESSAGE_TYPE_FIRST_UICC);
        }
        break;
    }
}

static void atsmoother_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    int8_t delta;

    UNUSED(instance);

    delta = ui_eventToDelta(ui_event, 10);

    switch (menu_item)
    {
    case 1:
        Config.Start = util_boundedAddUint8(Config.Start, START_N_ON, START_VALUE_0 + 127, delta);
        break;
    case 2:
        Config.FadeIn = util_boundedAddUint8(Config.FadeIn, 0, SMOOTHE_MAX, delta);
        break;
    case 3:
        Config.FadeOut = util_boundedAddUint8(Config.FadeOut, 0, SMOOTHE_MAX, delta);
        break;
    case 4:
        Config.OutputCc = util_boundedAddUint8(Config.OutputCc, 0, MESSAGE_TYPE_FIRST_UICC + MIDI_UICC_COUNT, delta);
        break;
    }
}


// ---------------------------  PRIVATE FUNCTIONS  -----------------------------

static void initialize(void)
{
    // Set defaults
    Config.FadeIn = 8;
    Config.FadeOut = 8;
    Config.OutputCc = MESSAGE_TYPE_CHAN_AT;
    Config.Start = START_N_ON;

    Instance.InUse = FALSE;
    Instance.TickDivider = 0;

    // Init state of channels
    uint8_t i;

    for (i = 0; i < CHANNEL_MAX; i++)
    {
        Instance.ChannelState[i].NoteOnCount = 0;
        Instance.ChannelState[i].FilterState = 0;
        Instance.ChannelState[i].FilterTarget = 0;
    }
}

static void makeOutputMsg(uint8_t channel, midiMsg_t *msg, uint8_t filter_out)
{
    if (Config.OutputCc == MESSAGE_TYPE_CHAN_AT)
    {
        midimsg_newChanAfterTouch(msg, channel, filter_out);
    }
    else
    {
        midimsg_newContinuousCtrl(msg, channel,
                midi_convertUiccToCc(Config.OutputCc - MESSAGE_TYPE_FIRST_UICC), filter_out);
    }
}

static bool_t handleNoteOn(uint8_t channel, uint8_t vel)
{
    bool_t ret = FALSE;

    if (channel < CHANNEL_MAX)
    {
        Instance.ChannelState[channel].NoteOnCount++;

        if (Instance.ChannelState[channel].NoteOnCount == 1)
        {
            if (Config.Start == START_N_ON)
            {
                // Adopt note on directly
                filterReset(channel, vel);

                // We will send a filter CC before note on in this case
                ret = TRUE;
            }
            else
            {
                filterReset(channel, Config.Start - START_VALUE_0);
                ret = TRUE;
            }
        }
    }

    return ret;
}

static void handleNoteOff(uint8_t channel)
{
    if (channel < CHANNEL_MAX)
    {
        if (Instance.ChannelState[channel].NoteOnCount > 0)
        {
            Instance.ChannelState[channel].NoteOnCount--;
        }
    }
}

static void handleChanAt(uint8_t channel, uint8_t vel)
{
    if (channel < CHANNEL_MAX)
    {
        if (Instance.ChannelState[channel].NoteOnCount > 0)
        {
            filterNewTarget(channel, vel);
        }
    }
}



static void filterReset(uint8_t channel, uint8_t value)
{
    Instance.ChannelState[channel].FilterTarget = value;
    Instance.ChannelState[channel].FilterState = (uint16_t)value << 4;
}


static void filterNewTarget(uint8_t channel, uint8_t value)
{
    Instance.ChannelState[channel].FilterTarget = value;
}

static void filterUpdate(uint8_t channel)
{
    int16_t diff;
    int16_t limit;

    // Calculate diff in value << 4
    diff = (int16_t)(Instance.ChannelState[channel].FilterTarget << 4);
    diff -= Instance.ChannelState[channel].FilterState;

    diff >>= 1;

    // Fading in or out?
    if (diff > 0)
    {
        limit = (int16_t)(Config.FadeIn << 2);
        if (diff > limit)
        {
            diff = limit;
        }
    }
    else
    {
        limit = -(int16_t)(Config.FadeOut << 2);

        if (diff < limit)
        {
            diff = limit;
        }
    }

    Instance.ChannelState[channel].FilterState += diff;
}

static uint8_t filterGetOutput(uint8_t channel)
{
    return (uint8_t)(Instance.ChannelState[channel].FilterState >> 4);
}


static void sendKeyAtIfChange(uint8_t channel)
{
    uint8_t filter_out;

    filter_out = filterGetOutput(channel);

    if (Instance.ChannelState[channel].LatestKeyAt != filter_out)
    {
        // Latest key AT value is different from filter calculated output, send new KeyAT message
        midiMsg_t msg;

        makeOutputMsg(channel, &msg, filter_out);
        midiproc_addMessage_ISR(&msg, Instance.FilterStep);

        Instance.ChannelState[channel].LatestKeyAt = filter_out;
    }
}
