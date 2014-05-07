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
// Generate message is a "filter" for generating midi messages based on various
// kinds of trigger messages:
//
// ...................
// Generate msg.    >2
//  Type : Patch chg.
//  Val. : 44
//
// -------------------------------  INCLUDES  -----------------------------------

#include "generatemsg.h"
#include "../midilog.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../midiprocessing.h"
#include "../errors.h"
#include "../hal.h"
#include "../util.h"
#include "../filters.h"
#include "../ui.h"
#include "../menus/filtermenu.h"  // TODO trim all these includes...
#include <avr/pgmspace.h>
#include "filteruids.h"

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t genmsg_Create(uint8_t filter_step);
static void    genmsg_Destroy(uint8_t instance);
static void    genmsg_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    genmsg_LoadConfig(uint8_t instance, void* data);
static void    genmsg_SaveConfig(uint8_t instance, void* data);
static uint8_t genmsg_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    genmsg_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    genmsg_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);

static void   makeMsg(uint8_t instance, midiMsg_t *msg, uint8_t channel);

// --------------------------  TYPES AND CONSTANTS  -----------------------------


typedef struct
{
    uint8_t  When;
    uint8_t  Type;
    int16_t  Value;
} message_setup_t;

typedef struct
{
    uint8_t FilterStep;
    bool_t  InUse;
} instance_t;

#define MESSAGES_MAX 10

// When options

#define WHEN_ON_MENU_CHG   0
#define WHEN_ON_LOAD       1
#define WHEN_BEFORE_KEYON  2
#define WHEN_AFTER_KEYOFF  3
#define WHEN_TRANS_ALL_MSG 4
#define WHEN_SETTINGS_COUNT 5


static char const WhenSettingsStr[WHEN_SETTINGS_COUNT][12] PROGMEM =
{
    "Val. change",
    "On load    ",
    "Before N.On",
    "After N.Off",
    "Translate  ",
};


// Message types

#define MESSAGE_TYPE_PROGCHG     0
#define MESSAGE_TYPE_CHAN_AT     1
#define MESSAGE_TYPE_PWHEEL      2
#define MESSAGE_TYPE_FIRST_UICC  3

#define MESSAGE_DEFAULT_CHANNEL  0


static char const StaticTitle[]   PROGMEM = "Generate msg.";
static char const SettingWhen[]   PROGMEM = "When : ";
static char const SettingType[]   PROGMEM = "Type : ";
static char const SettingValue[]  PROGMEM = "Val. : ";



const filterInterface_t genmsg_Filter PROGMEM =
{
        genmsg_Create,
        genmsg_Destroy,
        genmsg_SetFilterStep,
        genmsg_LoadConfig,
        genmsg_SaveConfig,
        genmsg_ProcessMidiMsg,
        genmsg_WriteMenuText,
        genmsg_HandleUiEvent,

        sizeof(message_setup_t), // Number of bytes in configuration
        3,              // Number of menu items (0 means only title, 1 is one item)
        7,              // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        StaticTitle,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_GENMSG_FILTER, 2)
};



// -------------------------------  VARIABLES  ----------------------------------

static message_setup_t Messages[MESSAGES_MAX];
static instance_t      Instances[MESSAGES_MAX];

// --------------------------  PRIVATE FUNCTIONS  ------------------------------

static void makeMsg(uint8_t instance, midiMsg_t *msg, uint8_t channel)
{
    if (Messages[instance].Type == MESSAGE_TYPE_PROGCHG)
    {
        midimsg_newProgramChange(msg, channel,
                (uint8_t)(Messages[instance].Value));
    }
    else if (Messages[instance].Type == MESSAGE_TYPE_CHAN_AT)
    {
        midimsg_newChanAfterTouch(msg, channel,
                (uint8_t)(Messages[instance].Value));
    }
    else if (Messages[instance].Type == MESSAGE_TYPE_PWHEEL)
    {
        midimsg_newPitchWheel(msg, channel, Messages[instance].Value);
    }
    else
    {
        midimsg_newContinuousCtrl(msg, channel,
                midi_convertUiccToCc(Messages[instance].Type - MESSAGE_TYPE_FIRST_UICC),
                (uint8_t)(Messages[instance].Value));
    }

}


// ---------------------------  PUBLIC FUNCTIONS  ------------------------------

void genmsg_initialize(void)
{
    uint8_t i;

    for (i = 0; i < MESSAGES_MAX; i++)
    {
        Messages[i].When = WHEN_ON_MENU_CHG;
        Messages[i].Type = MESSAGE_TYPE_PROGCHG;
        Messages[i].Value = 0;
        Instances[i].InUse = FALSE;
    }
}


// -----------------------  FILTER INTERFACE FUNCTIONS  -----------------------------

static uint8_t genmsg_Create(uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;
    uint8_t i;

    // Find free instance
    for (i = 0; i < MESSAGES_MAX; i++)
    {
        if (Instances[i].InUse == FALSE)
        {
            break;
        }
    }

    if (i < MESSAGES_MAX)
    {
        // Okay, lets do this

        // Set default config
        Instances[i].FilterStep = filter_step;
        Instances[i].InUse = TRUE;

        // Return instance number
        ret = i;
    }

    return ret;
}

static void genmsg_Destroy(uint8_t instance)
{
    Instances[instance].InUse = FALSE;
}

static void genmsg_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    Instances[instance].FilterStep = filter_step;
}

static void genmsg_LoadConfig(uint8_t instance, void* data)
{
    Messages[instance] = *((message_setup_t*)data);

    // If configured, send the message already

    if (Messages[instance].When == WHEN_ON_LOAD)
    {
        midiMsg_t msg;
        makeMsg(instance, &msg, MESSAGE_DEFAULT_CHANNEL);
        midiproc_addMessage_MAIN(&msg, Instances[instance].FilterStep);
    }
}

static void genmsg_SaveConfig(uint8_t instance, void* data)
{
    *((message_setup_t*)data) = Messages[instance];
}

static uint8_t genmsg_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;
    uint8_t chan;

    chan = msg->Data[MIDIMSG_STATUS] & MIDI_CHANNEL_MASK;

    // When to send message?
    switch (Messages[instance].When)
    {
    case WHEN_TRANS_ALL_MSG:
        // All incoming messages gets translated into our message

        // TODO implement CC trigger
        makeMsg(instance, msg, chan);
        ret = FILTER_PROCESS_DID;
        break;

    case WHEN_BEFORE_KEYON:
        // In this case, duplicate the key on message, then translate the first one into our message
        if (((msg->Data[MIDIMSG_STATUS]) & MIDI_STATUS_MASK) == MIDI_STATUS_NOTE_ON)
        {
            midiproc_addMessage_ISR(msg, Instances[instance].FilterStep);
            makeMsg(instance, msg, chan);
            ret = FILTER_PROCESS_DID;
        }
        break;

    case WHEN_AFTER_KEYOFF:
        // In this case, post a new message
        if (((msg->Data[MIDIMSG_STATUS]) & MIDI_STATUS_MASK) == MIDI_STATUS_NOTE_OFF)
        {
            midiMsg_t new_msg;
            makeMsg(instance, &new_msg, chan);
            midiproc_addMessage_ISR(&new_msg, Instances[instance].FilterStep);

            ret = FILTER_PROCESS_DID;
        }
        break;

    }

    return ret;
}


// Menu integration
static void genmsg_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        util_copyString_P(dest, StaticTitle);
    }
    else if (menu_item == 1)
        {
            // When information
            dest = util_copyString_P(dest, SettingWhen);
            util_copyString_P(dest, &(WhenSettingsStr[Messages[instance].When][0]) );
        }
    else if (menu_item == 2)
    {
        // Message type
        dest = util_copyString_P(dest, SettingType);

        switch (Messages[instance].Type)
        {
        case MESSAGE_TYPE_PROGCHG:
            midi_writeStatusName(dest, MIDI_STATUS_PROG_CHANGE);
            break;
        case MESSAGE_TYPE_CHAN_AT:
            midi_writeStatusName(dest, MIDI_STATUS_CHAN_ATOUCH);
            break;
        case MESSAGE_TYPE_PWHEEL:
            midi_writeStatusName(dest, MIDI_STATUS_PITCH_WHEEL);
            break;
        default:
            midi_writeUiccName(dest, Messages[instance].Type - MESSAGE_TYPE_FIRST_UICC);
            break;
        }
    }
    else if (menu_item == 3)
    {
        // Value.
        dest = util_copyString_P(dest, SettingValue);

        if (Messages[instance].Type == MESSAGE_TYPE_PROGCHG)
        {
            util_writeInt16LA(dest, Messages[instance].Value + 1);
        }
        else
        {
            util_writeInt16LA(dest, Messages[instance].Value);
        }
    }
}

static void genmsg_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    int8_t delta = ui_eventToDelta(ui_event, 10);

    if (menu_item == 1)
    {
        // When options
        Messages[instance].When =
                util_boundedAddUint8(Messages[instance].When, 0, WHEN_SETTINGS_COUNT - 1, delta);
    }
    else if (menu_item == 2)
    {
        // Message type options
        Messages[instance].Type =
                util_boundedAddUint8(Messages[instance].Type, 0, MESSAGE_TYPE_FIRST_UICC + MIDI_UICC_COUNT,
                        delta);

        // Zero the value when changing type
        Messages[instance].Value = 0;

        // Update the value field also
        filters_instance_t inst;
        inst.FilterType = FILTER_TYPE_GENMSG_FILTER;
        inst.Instance = instance;

        filtermenu_RequestUpdate(inst, 3);
    }
    else if (menu_item == 3)
    {
        // Value options
        if (Messages[instance].Type == MESSAGE_TYPE_PWHEEL)
        {
            Messages[instance].Value =
                    util_boundLimitInt16(Messages[instance].Value + (int16_t)ui_eventToDelta(ui_event, 100),
                            MIDI_PITCH_WHEEL_MIN, MIDI_PITCH_WHEEL_MAX);

        }
        else
        {
            Messages[instance].Value =
                    util_boundedAddUint8((uint8_t)(Messages[instance].Value), 0, 127,
                            ui_eventToDelta(ui_event, 10));
        }

        if (Messages[instance].When == WHEN_ON_MENU_CHG)
        {
            midiMsg_t new_msg;
            makeMsg(instance, &new_msg, MESSAGE_DEFAULT_CHANNEL);
            midiproc_addMessage_MAIN(&new_msg, Instances[instance].FilterStep);
        }

    }
}



