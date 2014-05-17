
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
#include "filteruids.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------


typedef struct
{
    bool_t  InUse;

    // Configuration
    uint8_t ConfigFrom;
    uint8_t ConfigTo;

    // Optimized checking of from:
    uint8_t FromStatus;
    uint8_t FromCC;

} instanceState_t;


#define MESSAGE_TYPE_KEY_AT      0
#define MESSAGE_TYPE_PROGCHG     1
#define MESSAGE_TYPE_CHAN_AT     2

#define MESSAGE_TYPE_FIRST_UICC  3

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

static void writeMsgType(char *dest, uint8_t msg_type);
static void makeOptimizedFromCheck(instanceState_t *instance);
static void processMsg(midiMsg_t *msg, instanceState_t *instance);

// -------------------------------  VARIABLES  ----------------------------------


static const char MenuTitle[]   PROGMEM = "Modify CC type";
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

        FILTER_UID(FILTER_ID_MODIFYCC_FILTER, 1)
};

// State
static instanceState_t Instances[INSTANCE_MAX];

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
        Instances[i].ConfigFrom = 3;
        Instances[i].ConfigTo = 3;
        Instances[i].InUse = TRUE;

        makeOptimizedFromCheck(&(Instances[i]));

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

    makeOptimizedFromCheck(&(Instances[instance]));
}

static void modifycc_SaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data) = Instances[instance].ConfigFrom;
    *((uint8_t*)(data)+1) = Instances[instance].ConfigTo;
}

static uint8_t modifycc_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Check status
    if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == Instances[instance].FromStatus)
    {
        // If this is a CC, also check CC byte
        if ((Instances[instance].FromCC == 0xFFu) ||
                (msg->Data[MIDIMSG_DATA1] == Instances[instance].FromCC))
        {
            // Process the message
            processMsg(msg, &(Instances[instance]));

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
        writeMsgType(dest, Instances[instance].ConfigFrom);
    }
    else if (menu_item == 2)
    {
        // ConfigTo
        dest = util_copyString_P(dest, MenuSetTo);
        writeMsgType(dest, Instances[instance].ConfigTo);
    }
}

static void modifycc_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].ConfigFrom =
                util_boundedAddUint8(Instances[instance].ConfigFrom, 0, MIDI_UICC_COUNT + MESSAGE_TYPE_FIRST_UICC - 1,
                        ui_eventToDelta(ui_event, 10));
    }
    else if (menu_item == 2)
    {
        // TO config starts at 1 because, we can't translate to Key Aftertouch
        Instances[instance].ConfigTo =
                util_boundedAddUint8(Instances[instance].ConfigTo, 1, MIDI_UICC_COUNT + MESSAGE_TYPE_FIRST_UICC - 1,
                        ui_eventToDelta(ui_event, 10));
    }

    makeOptimizedFromCheck(&(Instances[instance]));
}

static void writeMsgType(char *dest, uint8_t msg_type)
{
    switch (msg_type)
    {
    case MESSAGE_TYPE_KEY_AT:
        midi_writeStatusName(dest, MIDI_STATUS_KEY_ATOUCH);
        break;
    case MESSAGE_TYPE_PROGCHG:
        midi_writeStatusName(dest, MIDI_STATUS_PROG_CHANGE);
        break;
    case MESSAGE_TYPE_CHAN_AT:
        midi_writeStatusName(dest, MIDI_STATUS_CHAN_ATOUCH);
        break;
    default:
        midi_writeUiccName(dest, msg_type - MESSAGE_TYPE_FIRST_UICC);
        break;
    }
}

static void makeOptimizedFromCheck(instanceState_t *instance)
{
    switch (instance->ConfigFrom)
    {
    case MESSAGE_TYPE_KEY_AT:
        instance->FromStatus = MIDI_STATUS_KEY_ATOUCH;
        instance->FromCC = 0xFFu;
        break;
    case MESSAGE_TYPE_PROGCHG:
        instance->FromStatus = MIDI_STATUS_PROG_CHANGE;
        instance->FromCC = 0xFFu;
        break;
    case MESSAGE_TYPE_CHAN_AT:
        instance->FromStatus = MIDI_STATUS_CHAN_ATOUCH;
        instance->FromCC = 0xFFu;
        break;
    default:
        instance->FromStatus = MIDI_STATUS_CTRL_CHANGE;
        instance->FromCC = midi_convertUiccToCc(instance->ConfigFrom - MESSAGE_TYPE_FIRST_UICC);
        break;
    }
}

static void processMsg(midiMsg_t *msg, instanceState_t *instance)
{
    uint8_t data;
    uint8_t chan;

    // When we are here, the msg has been checked and found to match From message type

    // Pick out what we need from the From message:
    chan = msg->Data[MIDIMSG_STATUS] & MIDI_CHANNEL_MASK;

    if ((instance->ConfigFrom >= MESSAGE_TYPE_FIRST_UICC) ||
        (instance->ConfigFrom == MESSAGE_TYPE_KEY_AT))
    {
        data = msg->Data[MIDIMSG_DATA2];
    }
    else
    {
        data = msg->Data[MIDIMSG_DATA1];
    }

    // Construct To message:
    switch (instance->ConfigTo)
    {
    case MESSAGE_TYPE_PROGCHG:
        msg->DataLen = 2;
        msg->Data[MIDIMSG_STATUS] = MIDI_STATUS_PROG_CHANGE | chan;
        msg->Data[MIDIMSG_DATA1] = data;
        break;

    case MESSAGE_TYPE_CHAN_AT:
        msg->DataLen = 2;
        msg->Data[MIDIMSG_STATUS] = MIDI_STATUS_CHAN_ATOUCH | chan;
        msg->Data[MIDIMSG_DATA1] = data;
        break;

    default:
        msg->DataLen = 3;
        msg->Data[MIDIMSG_STATUS] = MIDI_STATUS_CTRL_CHANGE | chan;
        msg->Data[MIDIMSG_DATA1] = midi_convertUiccToCc(instance->ConfigTo - MESSAGE_TYPE_FIRST_UICC);
        msg->Data[MIDIMSG_DATA2] = data;
        break;
    }

}


