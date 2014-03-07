
//
// Filename    : routemsg.c
// Code module : Filter for routing MIDI messages
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
    bool_t  InUse;

    // Configuration
    uint8_t ConfigFrom;

    // Optimized checking of from:
    uint8_t FromStatus;
    uint8_t FromCC;

} instanceState_t;

#define MESSAGE_TYPE_NOTE_OFF    0
#define MESSAGE_TYPE_NOTE_ON     1
#define MESSAGE_TYPE_KEY_AT      2
#define MESSAGE_TYPE_CTRL_CHG    3
#define MESSAGE_TYPE_PROG_CHG    4
#define MESSAGE_TYPE_CHAN_AT     5
#define MESSAGE_TYPE_P_WHEEL     6
#define MESSAGE_TYPE_FIRST_UICC  7


#define MAP_TYPE_STATUS(x)  (((x)+8)<<4)


#define INSTANCE_MAX 10

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t routemsg_Create(uint8_t filter_step);
static void    routemsg_Destroy(uint8_t instance);
static void    routemsg_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    routemsg_LoadConfig(uint8_t instance, void* data);
static void    routemsg_SaveConfig(uint8_t instance, void* data);
static uint8_t routemsg_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    routemsg_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    routemsg_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);

static void writeMsgType(char *dest, uint8_t msg_type);
static void makeOptimizedFromCheck(instanceState_t *instance);

// -------------------------------  VARIABLES  ----------------------------------


static const char MenuTitle[]   PROGMEM = "Route message";
static const char MenuSetting[] PROGMEM = "Type : ";

const filterInterface_t routemsg_Filter PROGMEM =
{
        routemsg_Create,
        routemsg_Destroy,
        routemsg_SetFilterStep,
        routemsg_LoadConfig,
        routemsg_SaveConfig,
        routemsg_ProcessMidiMsg,
        routemsg_WriteMenuText,
        routemsg_HandleUiEvent,

        1,              // Number of bytes in configuration
        1,              // Number of menu items (0 means only title, 1 is one item)
        7,              // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        MenuTitle,      // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_ROUTEMSG_FILTER, 1)
};

// State
static instanceState_t Instances[INSTANCE_MAX];





// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

static void writeMsgType(char *dest, uint8_t msg_type)
{
    if (msg_type == MESSAGE_TYPE_CTRL_CHG)
    {
        util_copyString_P(dest, PSTR("All CC"));
    }
    else if (msg_type < MESSAGE_TYPE_FIRST_UICC)
    {
        midi_writeStatusName(dest, MAP_TYPE_STATUS(msg_type));
    }
    else
    {
        midi_writeUiccName(dest, msg_type - MESSAGE_TYPE_FIRST_UICC);
    }
}

static void makeOptimizedFromCheck(instanceState_t *instance)
{
    if (instance->ConfigFrom < MESSAGE_TYPE_FIRST_UICC)
    {
        instance->FromStatus = MAP_TYPE_STATUS(instance->ConfigFrom);
        instance->FromCC = 0xFFu;
    }
    else
    {
        instance->FromStatus = MIDI_STATUS_CTRL_CHANGE;
        instance->FromCC = midi_convertUiccToCc(instance->ConfigFrom - MESSAGE_TYPE_FIRST_UICC);
    }
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


static uint8_t routemsg_Create(uint8_t filter_step)
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
        Instances[i].ConfigFrom = MESSAGE_TYPE_FIRST_UICC;
        Instances[i].InUse = TRUE;

        makeOptimizedFromCheck(&(Instances[i]));

        // Return instance number
        ret = i;
    }

    return ret;
}

static void routemsg_Destroy(uint8_t instance)
{
    // Todo range checking
    Instances[instance].InUse = FALSE;
}

static void routemsg_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    (void)(filter_step);
}

static void routemsg_LoadConfig(uint8_t instance, void* data)
{
    Instances[instance].ConfigFrom = *((uint8_t*)data);

    makeOptimizedFromCheck(&(Instances[instance]));
}

static void routemsg_SaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data) = Instances[instance].ConfigFrom;
}

static uint8_t routemsg_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Check status
    if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == Instances[instance].FromStatus)
    {
        // If this is a CC, also check CC byte
        if ((Instances[instance].FromCC == 0xFFu) ||
                (msg->Data[MIDIMSG_DATA1] == Instances[instance].FromCC))
        {
            // This message should be routed!
            ret = FILTER_PROCESS_DID;
        }
    }

    return ret;
}


// Menu integration
static void routemsg_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        util_copyString_P(dest, MenuTitle);
    }
    else if (menu_item == 1)
    {
        dest = util_copyString_P(dest, MenuSetting);
        writeMsgType(dest, Instances[instance].ConfigFrom);
    }
}


static void routemsg_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].ConfigFrom =
                util_boundedAddUint8(Instances[instance].ConfigFrom, 0, MIDI_UICC_COUNT + MESSAGE_TYPE_FIRST_UICC - 1,
                        ui_eventToDelta(ui_event, 10));

        makeOptimizedFromCheck(&(Instances[instance]));
    }
}

