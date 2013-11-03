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
#include "../menus/filtermenu.h"
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

static void   PrepareMsg(uint8_t instance, midiMsg_t *msg);

// --------------------------  TYPES AND CONSTANTS  -----------------------------


typedef struct
{
    uint8_t Type;
    uint8_t Value;
} message_setup_t;

typedef struct
{
    uint8_t FilterStep;
    bool_t  InUse;
} instance_t;

#define MESSAGES_MAX 10

// Message types

#define MESSAGE_TYPE_PROGCHG     0
#define MESSAGE_TYPE_FIRST_UICC  1

#define MESSAGE_CHANNEL 0

static char const StaticTitle[]   PROGMEM = "Generate msg.";

static char const SettingType[]   PROGMEM = "Type : ";
static char const SettingValue[]  PROGMEM = "Val. : %i";

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
        2,              // Number of menu items (0 means only title, 1 is one item)
        7,              // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        StaticTitle,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_GENMSG_FILTER, 1)
};



// -------------------------------  VARIABLES  ----------------------------------

static message_setup_t Messages[MESSAGES_MAX];
static instance_t      Instances[MESSAGES_MAX];

// --------------------------  PRIVATE FUNCTIONS  ------------------------------

static void PrepareMsg(uint8_t instance, midiMsg_t *msg)
{
    if (Messages[instance].Type == MESSAGE_TYPE_PROGCHG)
    {
        midimsg_newProgramChange(msg, MESSAGE_CHANNEL,
                        Messages[instance].Value);
    }
    else
    {
        midimsg_newContinuousCtrl(msg, MESSAGE_CHANNEL,
                midi_convertUiccToCc(Messages[instance].Type - MESSAGE_TYPE_FIRST_UICC),
                Messages[instance].Value);
    }

}


// ---------------------------  PUBLIC FUNCTIONS  ------------------------------

void genmsg_initialize(void)
{
    uint8_t i;

    for (i = 0; i < MESSAGES_MAX; i++)
    {
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

    // Send the message
    midiMsg_t msg;
    PrepareMsg(instance, &msg);
    midiproc_addMessage_MAIN(&msg, Instances[instance].FilterStep);
}

static void genmsg_SaveConfig(uint8_t instance, void* data)
{
    *((message_setup_t*)data) = Messages[instance];
}

static uint8_t genmsg_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    (void)(msg);

    // Whenever we get stuff to the input, we send our message
    midiMsg_t new_msg;
    PrepareMsg(instance, &new_msg);
    midiproc_addMessage_ISR(&new_msg, Instances[instance].FilterStep);

    return FILTER_PROCESS_NEUTRAL;
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
        // Message type
        dest = util_copyString_P(dest, SettingType);

        if (Messages[instance].Type == MESSAGE_TYPE_PROGCHG)
        {
            util_copyString_P(dest, PSTR("Prog chg."));
        }
        else
        {
            midi_writeUiccName(dest, Messages[instance].Type - MESSAGE_TYPE_FIRST_UICC);
        }
    }
    else if (menu_item == 2)
    {
        // Value. This is always a 0-127 number
        util_writeFormat_P(dest, SettingValue, Messages[instance].Value + 1);
    }
}

static void genmsg_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    int8_t delta = ui_eventToDelta(ui_event, 10);

    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Messages[instance].Type =
                util_boundedAddUint8(Messages[instance].Type, 0, MESSAGE_TYPE_FIRST_UICC + MIDI_UICC_COUNT,
                        delta);
    }
    else if (menu_item == 2)
    {
        Messages[instance].Value =
                util_boundedAddUint8(Messages[instance].Value, 0, 127,
                        ui_eventToDelta(ui_event, 10));

        midiMsg_t new_msg;
        PrepareMsg(instance, &new_msg);
        midiproc_addMessage_ISR(&new_msg, Instances[instance].FilterStep);

    }
}



