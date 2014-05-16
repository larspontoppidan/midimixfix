
//
// Filename    : controlnoteon.c
// Code module : Filter for modifying note on velocity
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
#include "filteruids.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------


typedef struct
{
    // Instance
    bool_t  InUse;
    uint8_t LatestValue;

    // Configuration
    uint8_t ConfigFrom;

} instanceState_t;


#define INSTANCE_MAX 5

#define DEFAULT_VELOCITY 100

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t ccnoteon_Create(uint8_t filter_step);
static void    ccnoteon_Destroy(uint8_t instance);
static void    ccnoteon_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    ccnoteon_LoadConfig(uint8_t instance, void* data);
static void    ccnoteon_SaveConfig(uint8_t instance, void* data);
static uint8_t ccnoteon_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    ccnoteon_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    ccnoteon_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// -------------------------------  VARIABLES  ----------------------------------

static const char MenuTitle[]   PROGMEM = "Set NoteOn vel.";
static const char MenuSetting[] PROGMEM = "Use : ";


const filterInterface_t ccnoteon_Filter PROGMEM =
{
        ccnoteon_Create,
        ccnoteon_Destroy,
        ccnoteon_SetFilterStep,
        ccnoteon_LoadConfig,
        ccnoteon_SaveConfig,
        ccnoteon_ProcessMidiMsg,
        ccnoteon_WriteMenuText,
        ccnoteon_HandleUiEvent,

        1,              // Number of bytes in configuration
        1,              // Number of menu items (0 means only title, 1 is one item)
        6,              // Cursor indentation in menu
        FILTER_MODE_PROCESSOR, // Filter operation mode
        MenuTitle,      // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_CCNOTEON_FILTER, 1)
};

// State
static instanceState_t Instances[INSTANCE_MAX];

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

static uint8_t ccnoteon_Create(uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;
    uint8_t i;

    UNUSED(filter_step);

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
        Instances[i].ConfigFrom = 0;
        Instances[i].LatestValue = DEFAULT_VELOCITY;
        Instances[i].InUse = TRUE;

        // Return instance number
        ret = i;
    }

    return ret;
}

static void ccnoteon_Destroy(uint8_t instance)
{
    // Todo range checking
    Instances[instance].InUse = FALSE;
}

static void ccnoteon_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    UNUSED(instance);
    UNUSED(filter_step);
}

static void ccnoteon_LoadConfig(uint8_t instance, void* data)
{
    Instances[instance].ConfigFrom = *((uint8_t*)data);
}

static void ccnoteon_SaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data) = Instances[instance].ConfigFrom;
}

static uint8_t ccnoteon_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Check status
    switch (msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK)
    {
    case MIDI_STATUS_CTRL_CHANGE:
        // This is a CC, the one we are looking for?
        if (msg->Data[MIDIMSG_DATA1] == midi_convertUiccToCc(Instances[instance].ConfigFrom))
        {
            // Store the controller value
            Instances[instance].LatestValue = msg->Data[MIDIMSG_DATA2];

            // and discard message
            ret = FILTER_PROCESS_DISCARD;
        }
        break;

    case MIDI_STATUS_NOTE_ON:
        // Modify the note on velocity
        msg->Data[MIDIMSG_DATA2] = Instances[instance].LatestValue;
        ret = FILTER_PROCESS_DID;
        break;
    }

    return ret;
}


// Menu integration
static void ccnoteon_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        util_copyString_P(dest, MenuTitle);
    }
    else if (menu_item == 1)
    {
        // ConfigFrom
        dest = util_copyString_P(dest, MenuSetting);

        midi_writeUiccName(dest, Instances[instance].ConfigFrom);
    }
}

static void ccnoteon_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].ConfigFrom =
                util_boundedAddUint8(Instances[instance].ConfigFrom, 0, MIDI_UICC_COUNT,
                        ui_eventToDelta(ui_event, 10));
    }
}


