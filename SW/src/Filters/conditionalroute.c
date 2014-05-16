
//
// Filename    : conditionalroute.c
// Code module : Filter for conditional routing of MIDI messages
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
// Conditional routing of messages
//
//
// Cond. route     1>2
//  Mode : When >64
//  Mode : Toggle
//  Mode : Toggle >64
//  Use  : SustainP.

// -------------------------------  INCLUDES  -----------------------------------

#include "../common.h"
#include "../midimessage.h"
#include "../midiprocessing.h"
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
    // Filter management
    bool_t  InUse;
    bool_t  ReroutingNow;

    // Config
    uint8_t Mode;
    uint8_t Cc;

} instanceState_t;

#define INSTANCE_MAX 5

#define MODE_MOMENTARILY    0
#define MODE_TOGGLE_ALL     1
#define MODE_TOGGLE_HALF    2


// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t filterCreate(uint8_t filter_step);
static void    filterDestroy(uint8_t instance);
static void    filterSetStep(uint8_t instance, uint8_t filter_step);
static void    filterLoadConfig(uint8_t instance, void* data);
static void    filterSaveConfig(uint8_t instance, void* data);

static uint8_t filterProcessMidiMsg(uint8_t instance, midiMsg_t *msg);

static void    filterWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    filterHandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// -------------------------------  VARIABLES  ----------------------------------


static const char MenuTitle[]       PROGMEM = "Cond. route";


const filterInterface_t condroute_Filter PROGMEM =
{
        filterCreate,
        filterDestroy,
        filterSetStep,
        filterLoadConfig,
        filterSaveConfig,
        filterProcessMidiMsg,
        filterWriteMenuText,
        filterHandleUiEvent,

        2,                  // Number of bytes in configuration
        2,                  // Number of menu items (0 means only title, 1 is one item)
        7,                  // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        MenuTitle,          // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_CONDROUTE_FILTER, 1)
};



// State
static instanceState_t Instances[INSTANCE_MAX];


// ---------------------------  PRIVATE FUNCTIONS  -------------------------------


// ---------------------------  FILTER FUNCTIONS  -------------------------------


static uint8_t filterCreate(uint8_t filter_step)
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
        Instances[i].InUse = TRUE;
        Instances[i].ReroutingNow = FALSE;
        Instances[i].Mode = MODE_MOMENTARILY;
        Instances[i].Cc = 0;

        // Return instance number
        ret = i;
    }

    return ret;
}

static void filterDestroy(uint8_t instance)
{
    // Todo range checking
    Instances[instance].InUse = FALSE;
}

static void filterSetStep(uint8_t instance, uint8_t filter_step)
{
    UNUSED(instance);
    UNUSED(filter_step);
}

static void filterLoadConfig(uint8_t instance, void* data)
{
    Instances[instance].Mode = *((uint8_t*)data);
    Instances[instance].Cc =   *((uint8_t*)(data)+1);
}

static void filterSaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data)     = Instances[instance].Mode;
    *((uint8_t*)(data)+1) = Instances[instance].Cc;
}

static uint8_t filterProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Check status
    if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == MIDI_STATUS_CTRL_CHANGE)
    {
        if (msg->Data[MIDIMSG_DATA1] == midi_convertUiccToCc(Instances[instance].Cc))
        {
            // Ok, we got our control change to react to

            switch (Instances[instance].Mode)
            {
            case MODE_MOMENTARILY:
                Instances[instance].ReroutingNow = (msg->Data[MIDIMSG_DATA2] >= 64);
                break;
            case MODE_TOGGLE_HALF:
                if (msg->Data[MIDIMSG_DATA2] >= 64)
                {
                    Instances[instance].ReroutingNow = !Instances[instance].ReroutingNow;
                }
                break;
            case MODE_TOGGLE_ALL:
                Instances[instance].ReroutingNow = !Instances[instance].ReroutingNow;
                break;
            }

            ret = FILTER_PROCESS_DISCARD;
        }
        else
        {
            // Not our message, reroute?
            if (Instances[instance].ReroutingNow)
            {
                ret = FILTER_PROCESS_DID;
            }
        }
    }
    else
    {
        // Not our message, reroute?
        if (Instances[instance].ReroutingNow)
        {
            ret = FILTER_PROCESS_DID;
        }
    }

    return ret;
}


// Menu integration
static void filterWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        util_copyString_P(dest, MenuTitle);
    }
    else if (menu_item == 1)
    {
        dest = util_copyString_P(dest, PSTR("Mode : "));
        switch (Instances[instance].Mode)
        {
        case MODE_MOMENTARILY:
            util_copyString_P(dest, PSTR("When >64"));
            break;
        case MODE_TOGGLE_ALL:
            util_copyString_P(dest, PSTR("Toggle"));
            break;
        case MODE_TOGGLE_HALF:
            util_copyString_P(dest, PSTR("Toggle >64"));
            break;
        }
    }
    else if (menu_item == 2)
    {
        dest = util_copyString_P(dest, PSTR("Use  : "));
        midi_writeUiccName(dest, Instances[instance].Cc);
    }
}


static void filterHandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].Mode =
                util_boundedAddUint8(Instances[instance].Mode, 0, MODE_TOGGLE_HALF,
                        ui_eventToDelta(ui_event, 10));
    }
    else if (menu_item == 2)
    {
        Instances[instance].Cc =
                util_boundedAddUint8(Instances[instance].Cc, 0, MIDI_UICC_COUNT,
                        ui_eventToDelta(ui_event, 10));
    }

}

