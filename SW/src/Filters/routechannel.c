
//
// Filename    : routechannel.c
// Code module : Filter for routing MIDI channels
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

#include "midiout.h"
#include "../midilog.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../errors.h"
#include "../util.h"
#include "../hal.h"
#include "../filters.h"
#include "../ui.h"
#include <avr/pgmspace.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------


typedef struct
{
    bool_t  InUse;
    uint8_t Channel;
} instanceState_t;

#define INSTANCE_MAX 10

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t routechan_Create(uint8_t filter_step);
static void    routechan_Destroy(uint8_t instance);
static void    routechan_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    routechan_LoadConfig(uint8_t instance, void* data);
static void    routechan_SaveConfig(uint8_t instance, void* data);
static uint8_t routechan_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    routechan_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    routechan_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// -------------------------------  VARIABLES  ----------------------------------


static const char MenuTitle[]   PROGMEM = "Route channel";
static const char MenuSetting[] PROGMEM = "Chan. : %i";

const filterInterface_t routechan_Filter PROGMEM =
{
        routechan_Create,
        routechan_Destroy,
        routechan_SetFilterStep,
        routechan_LoadConfig,
        routechan_SaveConfig,
        routechan_ProcessMidiMsg,
        routechan_WriteMenuText,
        routechan_HandleUiEvent,

        1,              // Number of bytes in configuration
        1,              // Number of menu items (0 means only title, 1 is one item)
        8,              // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        MenuTitle,      // Static filter title (this may be different from in-menu title)

        (FILTER_ID_AUTHOR  * 1ul) |
        (FILTER_ID_VERSION * 1ul) |
        (FILTER_ID_TYPE    * 2ul)
};

// State
instanceState_t Instances[INSTANCE_MAX];


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------



static uint8_t routechan_Create(uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;
    uint8_t i;

    (void)(filter_step);

    // Find free instance
    for (i = 0; i < INSTANCE_MAX; i++)
    {
        if (Instances[i].InUse == FALSE)
        {
            break;
        }
    }

    if (i < INSTANCE_MAX)
    {
        // Okay, lets do this

        // Set default config
        Instances[i].Channel = 0;
        Instances[i].InUse = TRUE;

        // Return instance number
        ret = i;
    }

    return ret;
}

static void routechan_Destroy(uint8_t instance)
{
    // Todo range checking
    Instances[instance].InUse = FALSE;
}

static void routechan_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    (void)(filter_step);
}

static void routechan_LoadConfig(uint8_t instance, void* data)
{
    Instances[instance].Channel = *((uint8_t*)data);
}

static void routechan_SaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data) = Instances[instance].Channel;
}

static uint8_t routechan_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    // Processing in this filter is pretty simple.
    // If midi message has the right channel, it is "taken" and routed
    // to the filter out route.
    //
    // The re-routing is handled by midiprocessing module, so all we have
    // to do here is to decide if we want to "take" the message or not

    uint8_t ret;

    if (((msg->Data[MIDIMSG_STATUS] & 0xF0) != 0xF0) &&    // Don't look at system messages
         (msg->Data[MIDIMSG_STATUS] & MIDI_CHANNEL_MASK) == Instances[instance].Channel)
    {
        ret = FILTER_PROCESS_DID;
    }
    else
    {
        ret = FILTER_PROCESS_NEUTRAL;
    }

    return ret;
}


// Menu integration
static void routechan_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        util_copyString_P(dest, MenuTitle);
    }
    else if (menu_item == 1)
    {
        util_writeFormat_P(dest, MenuSetting, (Instances[instance].Channel + 1));
    }
}


static void routechan_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].Channel =
                util_boundedAddInt8(Instances[instance].Channel, 0, 15,
                        ui_eventToDelta(ui_event, 10));
    }
}

