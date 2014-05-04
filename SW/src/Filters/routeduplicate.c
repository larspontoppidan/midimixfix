
//
// Filename    : duplicate.c
// Code module : Filter for setting MIDI channel of messages
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
    uint8_t FilterStep;
    bool_t  RouteNotDuplicate;

    // Filter config
    uint8_t MsgType;

    // Optimized filtering vars (to speed up midi processing)
    uint8_t StatusMask;
    uint8_t StatusCheck;
    uint8_t CCCheck;

} instanceState_t;

#define INSTANCE_MAX 10

#define MESSAGE_TYPE_ALL         0
#define MESSAGE_TYPE_CHAN1       1
#define MESSAGE_TYPE_CHAN16      16

#define MESSAGE_TYPE_NOTE_OFF    17
#define MESSAGE_TYPE_NOTE_ON     18
#define MESSAGE_TYPE_KEY_AT      19
#define MESSAGE_TYPE_CTRL_CHG    20
#define MESSAGE_TYPE_PROG_CHG    21
#define MESSAGE_TYPE_CHAN_AT     22
#define MESSAGE_TYPE_P_WHEEL     23
#define MESSAGE_TYPE_FIRST_UICC  24
#define MESSAGE_TYPE_LAST        (MIDI_UICC_COUNT + MESSAGE_TYPE_FIRST_UICC - 1)

#define MESSAGE_TYPE_DEFAULT     MESSAGE_TYPE_ALL


#define MAP_TYPE_STATUS(x)  (((x)-9)<<4)

#define NO_CC_CHECK         0xFF

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t filterCreate(uint8_t filter_step, bool_t routeNotDuplicate);

static void writeMsgType(char *dest, uint8_t msg_type);
static void makeOptimizedFromCheck(instanceState_t *instance);

// Filter implementation functionsf

static uint8_t filterCreateDuplicate(uint8_t filter_step);
static uint8_t filterCreateRoute(uint8_t filter_step);

static void    filterDestroy(uint8_t instance);
static void    filterSetStep(uint8_t instance, uint8_t filter_step);
static void    filterLoadConfig(uint8_t instance, void* data);
static void    filterSaveConfig(uint8_t instance, void* data);

static uint8_t filterProcessMidiMsg(uint8_t instance, midiMsg_t *msg);

static void    filterWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    filterHandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// -------------------------------  VARIABLES  ----------------------------------


static const char MenuTitleDuplicate[] PROGMEM = "Duplicate msg.";
static const char MenuTitleRoute[]     PROGMEM = "Route msg.";
static const char MenuSetting[]        PROGMEM = "Type : ";

const filterInterface_t dupmsg_Filter PROGMEM =
{
        filterCreateDuplicate,
        filterDestroy,
        filterSetStep,
        filterLoadConfig,
        filterSaveConfig,
        filterProcessMidiMsg,
        filterWriteMenuText,
        filterHandleUiEvent,

        1,                  // Number of bytes in configuration
        1,                  // Number of menu items (0 means only title, 1 is one item)
        7,                  // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        MenuTitleDuplicate, // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_DUPMSG_FILTER, 1)
};


const filterInterface_t routemsg_Filter PROGMEM =
{
        filterCreateRoute,
        filterDestroy,
        filterSetStep,
        filterLoadConfig,
        filterSaveConfig,
        filterProcessMidiMsg,
        filterWriteMenuText,
        filterHandleUiEvent,

        1,                  // Number of bytes in configuration
        1,                  // Number of menu items (0 means only title, 1 is one item)
        7,                  // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        MenuTitleRoute,     // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_ROUTEMSG_FILTER, 1)
};

// State
static instanceState_t Instances[INSTANCE_MAX];


// ---------------------------  PRIVATE FUNCTIONS  -------------------------------

static void writeMsgType(char *dest, uint8_t msg_type)
{
    if (msg_type == MESSAGE_TYPE_ALL)
    {
        util_copyString_P(dest, PSTR("All"));
    }
    else if (msg_type <= MESSAGE_TYPE_CHAN16)
    {
        util_writeFormat_P(dest, PSTR("Chan %i"), msg_type);
    }
    else if (msg_type == MESSAGE_TYPE_CTRL_CHG)
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
    if (instance->MsgType == MESSAGE_TYPE_ALL)
    {
        // All pass rule:
        instance->StatusMask = 0;
        instance->StatusCheck = 0;
        instance->CCCheck = NO_CC_CHECK;
    }
    else if (instance->MsgType <= MESSAGE_TYPE_CHAN16)
    {
        // Channel pass rule:
        instance->StatusMask = 0x0F;
        instance->StatusCheck = instance->MsgType - 1;
        instance->CCCheck = NO_CC_CHECK;
    }
    else if (instance->MsgType < MESSAGE_TYPE_FIRST_UICC)
    {
        // Message status check rule
        instance->StatusMask = 0xF0;
        instance->StatusCheck = MAP_TYPE_STATUS(instance->MsgType);
        instance->CCCheck = NO_CC_CHECK;
    }
    else
    {
        // CC check rule
        instance->StatusMask = 0xF0;
        instance->StatusCheck = MIDI_STATUS_CTRL_CHANGE;
        instance->CCCheck = midi_convertUiccToCc(instance->MsgType - MESSAGE_TYPE_FIRST_UICC);
    }
}


// ---------------------------  FILTER FUNCTIONS  -------------------------------


static uint8_t filterCreate(uint8_t filter_step, bool_t routeNotDuplicate)
{
    uint8_t ret = FILTER_CREATE_FAILED;
    uint8_t i;

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
        Instances[i].FilterStep = filter_step;
        Instances[i].InUse = TRUE;
        Instances[i].RouteNotDuplicate = routeNotDuplicate;
        Instances[i].MsgType = MESSAGE_TYPE_DEFAULT;

        makeOptimizedFromCheck(&(Instances[i]));

        // Return instance number
        ret = i;
    }

    return ret;
}

static uint8_t filterCreateRoute(uint8_t filter_step)
{
    return filterCreate(filter_step, TRUE);
}

static uint8_t filterCreateDuplicate(uint8_t filter_step)
{
    return filterCreate(filter_step, FALSE);
}

static void filterDestroy(uint8_t instance)
{
    // Todo range checking
    Instances[instance].InUse = FALSE;
}

static void filterSetStep(uint8_t instance, uint8_t filter_step)
{
    Instances[instance].FilterStep = filter_step;
}

static void filterLoadConfig(uint8_t instance, void* data)
{
    Instances[instance].MsgType = *((uint8_t*)data);
    makeOptimizedFromCheck(&(Instances[instance]));
}

static void filterSaveConfig(uint8_t instance, void* data)
{
    *((uint8_t*)data) = Instances[instance].MsgType;
}

static uint8_t filterProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t ret = FILTER_PROCESS_NEUTRAL;

    // Check status
    if ((msg->Data[MIDIMSG_STATUS] & Instances[instance].StatusMask) == Instances[instance].StatusCheck)
    {
        // If this is a CC, also check CC byte
        if ((Instances[instance].CCCheck == NO_CC_CHECK) ||
                (msg->Data[MIDIMSG_DATA1] == Instances[instance].CCCheck))
        {
            // This message passes the input filter!

            // Route or duplicate?
            if (Instances[instance].RouteNotDuplicate)
            {
                // We reroute the message simply by indicating that we did infact process it
                ret = FILTER_PROCESS_DID;
            }
            else
            {
                // We duplicate the message by creating a new one, which is a copy, at our FilterStep
                midiproc_addMessage_ISR(msg, Instances[instance].FilterStep);
            }
        }
    }

    return ret;
}


// Menu integration
static void filterWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    if (menu_item == 0)
    {
        if (Instances[instance].RouteNotDuplicate)
        {
            util_copyString_P(dest, MenuTitleRoute);
        }
        else
        {
            util_copyString_P(dest, MenuTitleDuplicate);
        }
    }
    else if (menu_item == 1)
    {
        dest = util_copyString_P(dest, MenuSetting);
        writeMsgType(dest, Instances[instance].MsgType);
    }
}


static void filterHandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        Instances[instance].MsgType =
                util_boundedAddUint8(Instances[instance].MsgType, 0, MESSAGE_TYPE_LAST,
                        ui_eventToDelta(ui_event, 10));

        makeOptimizedFromCheck(&(Instances[instance]));
    }
}

