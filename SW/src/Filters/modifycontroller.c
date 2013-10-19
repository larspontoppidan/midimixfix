
//
// Filename    : modifycontroller.c
// Code module : Filter for modifying various controller messages
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

    // Configuration
    uint8_t ConfigFrom;
    uint8_t ConfigTo;

    // Destilled into easy-to-do actions
    uint8_t FilterMode;
    uint8_t FilterFrom;
    uint8_t FilterTo;
} instanceState_t;

#define FILTER_CC_TO_CC  0
#define FILTER_CAT_TO_CC  0
#define FILTER_CAT_TO_CC  0

#define INSTANCE_MAX 5

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t modifycc_Create(uint8_t filter_step);
static void    modifycc_Destroy(uint8_t instance);
static void    modifycc_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    modifycc_LoadConfig(uint8_t instance, void* data);
static void    modifycc_SaveConfig(uint8_t instance, void* data);
static uint8_t modifycc_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    modifycc_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    modifycc_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// -------------------------------  VARIABLES  ----------------------------------


static const char MenuTitle[]   PROGMEM = "Modify ctrl.";
static const char MenuSetFrom[] PROGMEM = "From : ";
static const char MenuSetTo[]   PROGMEM = "To   : ";

const filterInterface_t modifycc_Filter PROGMEM =
{
        modifycc_Create,
        modifycc_Destroy,
        modifycc_SetFilterStep,
        modifycc_LoadConfig,
        modifycc_SaveConfig,
        modifycc_ProcessMidiMsg,
        modifycc_WriteMenuText,
        modifycc_HandleUiEvent,

        2,              // Number of bytes in configuration
        2,              // Number of menu items (0 means only title, 1 is one item)
        7,              // Cursor indentation in menu
        FILTER_MODE_PROCESSOR, // Filter operation mode
        MenuTitle,      // Static filter title (this may be different from in-menu title)

        (FILTER_ID_AUTHOR  * 1ul) |
        (FILTER_ID_VERSION * 1ul) |
        (FILTER_ID_TYPE    * 5ul)
};

// State
instanceState_t Instances[INSTANCE_MAX];

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

static uint8_t modifycc_Create(uint8_t filter_step)
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
        Instances[i].ConfigFrom = 0;
        Instances[i].ConfigTo = 0;
        Instances[i].InUse = TRUE;

        // Return instance number
        ret = i;
    }

    return ret;
}

static void modifycc_Destroy(uint8_t instance)
{
    // Todo range checking
    Instances[instance].InUse = FALSE;
}

static void modifycc_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    (void)(filter_step);
}

static void modifycc_LoadConfig(uint8_t instance, void* data)
{
    Instances[instance].ConfigFrom = *((uint8_t*)data);
    Instances[instance].ConfigTo   = *((uint8_t*)(data)+1);
}

static void modifycc_SaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data) = Instances[instance].ConfigFrom;
    *((uint8_t*)(data)+1) = Instances[instance].ConfigTo;
}

static uint8_t modifycc_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Only deal with CC
    if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == MIDI_STATUS_CTRL_CHANGE)
    {
        // Is this a controller we should modify?
        if (msg->Data[MIDIMSG_DATA1] == midi_convertUiccToCc(Instances[instance].ConfigFrom))
        {
            // Then do it
            msg->Data[MIDIMSG_DATA1] = midi_convertUiccToCc(Instances[instance].ConfigTo);
            ret = FILTER_PROCESS_DID;
        }
    }

    return ret;
}


// Menu integration
static void modifycc_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        util_copyString_P(dest, MenuTitle);
    }
    else if (menu_item == 1)
    {
        // ConfigFrom
        dest = util_copyString_P(dest, MenuSetFrom);
        midi_writeUiccName(dest, Instances[instance].ConfigFrom);
    }
    else if (menu_item == 2)
    {
        // ConfigTo
        dest = util_copyString_P(dest, MenuSetTo);
        midi_writeUiccName(dest, Instances[instance].ConfigTo);
    }
}

static void modifycc_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].ConfigFrom =
                util_boundedAddUint8(Instances[instance].ConfigFrom, 0, MIDI_UICC_MAX-1,
                        ui_eventToDelta(ui_event, 10));
    }
    else if (menu_item == 2)
    {
        Instances[instance].ConfigTo =
                util_boundedAddUint8(Instances[instance].ConfigTo, 0, MIDI_UICC_MAX-1,
                        ui_eventToDelta(ui_event, 10));
    }
}

