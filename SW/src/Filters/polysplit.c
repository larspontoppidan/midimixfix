//
// Filename    : polysplit.c
// Code module : Filter for splitting polyphonic aftertouch into different MIDI channels
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


// -------------------------------  INCLUDES  -----------------------------------

#include "../midiprocessing.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../errors.h"
#include "../util.h"
#include "../ui.h"
#include "../menus/filtermenu.h"
#include "../pgmstrings.h"
#include <avr/pgmspace.h>
#include "filteruids.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------


// When note state is 255, the key has just been pressed
// At key off, the state is initially 254 and then counts down to zero
#define NOTE_STATE_ON   255

#define CHANNEL_MAX 9

#define NOT_FOUND 0xFF

// Configuration

#define BROADCAST_CC      0x01
#define BROADCAST_PROGCHG 0x02
#define BROADCAST_P_WHEEL 0x04
#define BROADCAST_CHAN_AT 0x08

typedef struct
{
    uint8_t FirstChannel;
    uint8_t OutChannels;     // E.g., if 5, the channels 1,2,3,4,5 are used
    uint8_t BroadCastMode;
} config_t;

// Filter state

typedef struct
{
    uint8_t NoteOffTime;      // 255 is key on, 254 is key off and counting downwards
    uint8_t LatestKey;        // The key played.
} channel_state_t;

typedef struct
{
    bool_t  InUse;
    uint8_t FilterStep;
    uint8_t LatestNoteOnChan;
    uint8_t TickDivider;
    channel_state_t ChannelState[CHANNEL_MAX];
} state_t;



static char const MenuTitle[]    PROGMEM = "Polyphon. split";

static char const MenuSetting1[] PROGMEM = "Ch.range from: %i";
static char const MenuSetting2[] PROGMEM = "Ch.range to  : %i";
static char const MenuSetting4[] PROGMEM = "Broadcast CC.: %y";
static char const MenuSetting5[] PROGMEM = "Bcst. ProgChg: %y";
static char const MenuSetting6[] PROGMEM = "Bcst. P.Wheel: %y";
static char const MenuSetting7[] PROGMEM = "Bcst. Chan.AT: %y";


// ------------------------------  PROTOTYPES  ----------------------------------

// Filter interface

static uint8_t polysplit_Create(uint8_t filter_step);
static void    polysplit_Destroy(uint8_t instance);
static void    polysplit_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    polysplit_LoadConfig(uint8_t instance, void* data);
static void    polysplit_SaveConfig(uint8_t instance, void* data);
static uint8_t polysplit_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    polysplit_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    polysplit_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// Channel management:

static void initialize(void);
static uint8_t findActiveChannel(uint8_t key);
static uint8_t findFreeChannel(uint8_t key);


static bool_t handleNoteOn(midiMsg_t *msg);
static bool_t handleNoteOff(midiMsg_t *msg);
static bool_t handleKeyAT(midiMsg_t *msg);

static void broadcastMsg(midiMsg_t *msg);
static bool_t singlecastMsg(midiMsg_t *msg);


// -------------------------------  VARIABLES  ----------------------------------


const filterInterface_t polysplit_Filter PROGMEM =
{
        polysplit_Create,
        polysplit_Destroy,
        polysplit_SetFilterStep,
        polysplit_LoadConfig,
        polysplit_SaveConfig,
        polysplit_ProcessMidiMsg,
        polysplit_WriteMenuText,
        polysplit_HandleUiEvent,

        sizeof(config_t),   // Number of bytes in configuration
        6,                  // Number of menu items (0 means only title, 1 is one item)
        15,                 // Cursor indentation in menu
        FILTER_MODE_PROCESSOR, // Filter operation mode
        MenuTitle,          // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_POLYSPLIT_FILTER, 2)
};

static state_t  Instance;
static config_t Config;



// ---------------------------  PUBLIC FUNCTIONS  -------------------------------



// ------------------------  FILTER INTERFACE FUNCTIONS  ------------------------

static uint8_t polysplit_Create(uint8_t filter_step)
{
    uint8_t ret;

    if (Instance.InUse == FALSE)
    {
        Instance.FilterStep = filter_step;
        Instance.InUse = TRUE;

        // Initialize state
        initialize();

        // Return some instance number. Won't be used as there can be only one instance
        ret = 0;
    }
    else
    {
        ret = FILTER_CREATE_FAILED;
    }

    return ret;
}

static void polysplit_Destroy(uint8_t instance)
{
    UNUSED(instance);
    Instance.InUse = FALSE;
}

static void polysplit_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    UNUSED(instance);

    if (Instance.InUse)
    {
        Instance.FilterStep = filter_step;
    }
}

static void polysplit_LoadConfig(uint8_t instance, void* data)
{
    UNUSED(instance);
    Config = *((config_t*)data);
}

static void polysplit_SaveConfig(uint8_t instance, void* data)
{
    UNUSED(instance);
    *((config_t*)data) = Config;
}

static uint8_t polysplit_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    UNUSED(instance);

    // Deal with this message if status is something we are interested in
    switch (msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK)
    {
    case MIDI_STATUS_NOTE_ON:
        ret = handleNoteOn(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        break;

    case MIDI_STATUS_NOTE_OFF:
        ret = handleNoteOff(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        break;

    case MIDI_STATUS_KEY_ATOUCH:
        ret = handleKeyAT(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        break;

    case MIDI_STATUS_CTRL_CHANGE:
        if (Config.BroadCastMode & BROADCAST_CC)
        {
            broadcastMsg(msg);
            ret = FILTER_PROCESS_DISCARD;
        }
        else
        {
            ret = singlecastMsg(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        }
        break;

    case MIDI_STATUS_PROG_CHANGE:
        if (Config.BroadCastMode & BROADCAST_PROGCHG)
        {
            broadcastMsg(msg);
            ret = FILTER_PROCESS_DISCARD;
        }
        else
        {
            ret = singlecastMsg(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        }
        break;

    case MIDI_STATUS_PITCH_WHEEL:
        if (Config.BroadCastMode & BROADCAST_P_WHEEL)
        {
            broadcastMsg(msg);
            ret = FILTER_PROCESS_DISCARD;
        }
        else
        {
            ret = singlecastMsg(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        }
        break;

    case MIDI_STATUS_CHAN_ATOUCH:
        if (Config.BroadCastMode & BROADCAST_CHAN_AT)
        {
            broadcastMsg(msg);
            ret = FILTER_PROCESS_DISCARD;
        }
        else
        {
            ret = singlecastMsg(msg) ? FILTER_PROCESS_DID : FILTER_PROCESS_DISCARD;
        }
        break;
    }

    return ret;
}


// Menu integration
static void polysplit_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    UNUSED(instance);

    switch (menu_item)
    {
    case 0:
        util_copyString_P(dest, MenuTitle);
        break;
    case 1:
        util_writeFormat_P(dest, MenuSetting1, Config.FirstChannel + 1);
        break;
    case 2:
        util_writeFormat_P(dest, MenuSetting2, Config.FirstChannel + Config.OutChannels);
        break;
    case 3:
        util_writeFormat_P(dest, MenuSetting4, Config.BroadCastMode & BROADCAST_CC);
        break;
    case 4:
        util_writeFormat_P(dest, MenuSetting5, Config.BroadCastMode & BROADCAST_PROGCHG);
        break;
    case 5:
        util_writeFormat_P(dest, MenuSetting6, Config.BroadCastMode & BROADCAST_P_WHEEL);
        break;
    case 6:
        util_writeFormat_P(dest, MenuSetting7, Config.BroadCastMode & BROADCAST_CHAN_AT);
        break;
    }
}

static void polysplit_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    filters_instance_t f_instance;
    int8_t delta;

    f_instance.FilterType = FILTER_TYPE_POLYSPLIT_FILTER;
    f_instance.Instance = instance;

    delta = ui_eventToDelta(ui_event, 5);

    switch (menu_item)
    {
    case 1:
        Config.FirstChannel = util_boundedAddUint8(Config.FirstChannel, 0, 15, delta);
        filtermenu_RequestUpdate(f_instance, 2);
        break;
    case 2:
        Config.OutChannels =  util_boundedAddUint8(Config.OutChannels, 1, CHANNEL_MAX, delta);
        break;
    case 3:
        Config.BroadCastMode = ui_eventToSetClearBit(ui_event, Config.BroadCastMode, BROADCAST_CC);
        break;
    case 4:
        Config.BroadCastMode = ui_eventToSetClearBit(ui_event, Config.BroadCastMode, BROADCAST_PROGCHG);
        break;
    case 5:
        Config.BroadCastMode = ui_eventToSetClearBit(ui_event, Config.BroadCastMode, BROADCAST_P_WHEEL);
        break;
    case 6:
        Config.BroadCastMode = ui_eventToSetClearBit(ui_event, Config.BroadCastMode, BROADCAST_CHAN_AT);
        break;
    }
}


// ---------------------------  PRIVATE FUNCTIONS  -----------------------------

static void initialize(void)
{
    // Set defaults
    Config.FirstChannel = 1;
    Config.OutChannels = 7;
    Config.BroadCastMode = BROADCAST_CC | BROADCAST_CHAN_AT | BROADCAST_PROGCHG | BROADCAST_P_WHEEL;
    Instance.LatestNoteOnChan = NOT_FOUND;
    Instance.TickDivider = 0;

    // Init state of channels
    uint8_t i;

    for (i = 0; i < Config.OutChannels; i++)
    {
        Instance.ChannelState[i].LatestKey = 0;
        Instance.ChannelState[i].NoteOffTime = 0;
    }
}

// Find channel that currently has this key pressed
static uint8_t findActiveChannel(uint8_t key)
{
    uint8_t i;
    uint8_t ret = NOT_FOUND;

    for (i = 0; i < Config.OutChannels; i++)
    {
        if ((Instance.ChannelState[i].NoteOffTime == NOTE_STATE_ON) &&
                (Instance.ChannelState[i].LatestKey == key))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

// Find a free channel that either played the key last time
// or secondarily, the channel not used for the longest time
static uint8_t findFreeChannel(uint8_t key)
{
    uint8_t i;
    uint8_t ret = NOT_FOUND;
    uint8_t best_free = NOTE_STATE_ON;

    for (i = 0; i < Config.OutChannels; i++)
    {
        if ((Instance.ChannelState[i].NoteOffTime != NOTE_STATE_ON) &&
                (Instance.ChannelState[i].LatestKey == key))
        {
            // We got a hit right here
            ret = i;
            break;
        }

        if (Instance.ChannelState[i].NoteOffTime < best_free)
        {
            best_free = Instance.ChannelState[i].NoteOffTime;
            ret = i;
        }
    }

    return ret;
}


static bool_t handleNoteOn(midiMsg_t *msg)
{
    // Find a channel for this based on key
    uint8_t c = findFreeChannel(msg->Data[MIDIMSG_DATA1]);

    if (c != NOT_FOUND)
    {
        midimsg_setChan(msg, c + Config.FirstChannel);
        Instance.LatestNoteOnChan = c + Config.FirstChannel;

        Instance.ChannelState[c].LatestKey = msg->Data[MIDIMSG_DATA1];
        Instance.ChannelState[c].NoteOffTime = NOTE_STATE_ON;
    }

    return (c != NOT_FOUND);
}

static bool_t handleNoteOff(midiMsg_t *msg)
{
    // Find correct channel for this
    uint8_t c = findActiveChannel(msg->Data[MIDIMSG_DATA1]);

    if (c != NOT_FOUND)
    {
        midimsg_setChan(msg, c + Config.FirstChannel);

        Instance.ChannelState[c].NoteOffTime = NOTE_STATE_ON - 1;

        // Decrement all off channel's NoteOffvalue
        uint8_t i;

        for (i = 0; i < Config.OutChannels; i++)
        {
            if ((Instance.ChannelState[i].NoteOffTime != NOTE_STATE_ON) &&
                (Instance.ChannelState[i].NoteOffTime != 0))
            {
                Instance.ChannelState[i].NoteOffTime--;
            }
        }

    }

    return (c != NOT_FOUND);
}

static bool_t handleKeyAT(midiMsg_t *msg)
{
    bool_t ret = FALSE;

    // Find correct channel for this
    uint8_t c = findActiveChannel(msg->Data[MIDIMSG_DATA1]);

    if (c != NOT_FOUND)
    {
        // Transform the key at. into channel at.
        midimsg_newChanAfterTouch(msg, c + Config.FirstChannel, msg->Data[MIDIMSG_DATA2]);

        ret = TRUE;
    }

    return ret;
}


static bool_t singlecastMsg(midiMsg_t *msg)
{
    // When not broadcasting, send message to channel having received the latest note one

    if (Instance.LatestNoteOnChan != NOT_FOUND)
    {
        // Modify message channel and submit it
        midimsg_setChan(msg, Instance.LatestNoteOnChan);
    }

    return (Instance.LatestNoteOnChan != NOT_FOUND);
}

static void broadcastMsg(midiMsg_t *msg)
{
    uint8_t c;

    // Broadcast messages to all channels
    for (c = 0; c < Config.OutChannels; c++)
    {
        // Modify message channel and submit it
        midimsg_setChan(msg, c + Config.FirstChannel);
        midiproc_addMessage_ISR(msg, Instance.FilterStep);
    }
}
