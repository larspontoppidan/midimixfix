
//
// Filename    : midiout.c
// Code module : Midi output sink for MidiMixFix
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

static const char MenuTitle[9]    PROGMEM = "MIDI Out";
static const char MenuSetting[18] PROGMEM = "Runn. status : %o";


// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t midiout_Create(uint8_t filter_step);
static void    midiout_Destroy(uint8_t instance);
static void    midiout_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    midiout_LoadConfig(uint8_t instance, void* data);
static void    midiout_SaveConfig(uint8_t instance, void* data);
static uint8_t midiout_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    midiout_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    midiout_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);


// -------------------------------  VARIABLES  ----------------------------------


const filterInterface_t midiout_Filter PROGMEM =
{
        midiout_Create,
        midiout_Destroy,
        midiout_SetFilterStep,
        midiout_LoadConfig,
        midiout_SaveConfig,
        midiout_ProcessMidiMsg,
        midiout_WriteMenuText,
        midiout_HandleUiEvent,

        1,              // Number of bytes in configuration
        1,              // Number of menu items (0 means only title, 1 is one item)
        15,             // Cursor indentation in menu
        FILTER_MODE_IN, // Filter operation mode
        MenuTitle,      // Static filter title (this may be different from in-menu title)

        (FILTER_ID_AUTHOR  * 1ul) |
        (FILTER_ID_VERSION * 1ul) |
        (FILTER_ID_TYPE    * 10ul)
};


// Configuration
static bool_t ConfigUseRunningStatus;

// State
static uint8_t Instances;
static uint8_t LastStatus;

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void midiout_initialize(void)
{
    Instances = 0;
    LastStatus = 0xFF;
}


static uint8_t midiout_Create(uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;

    (void)(filter_step);

    if (Instances == 0)
    {
        // Okay, lets do this
        Instances = 1;

        // Default config
        ConfigUseRunningStatus = FALSE;

        // Return our instance number, which is 0
        ret = 0;
    }

    return ret;
}

static void midiout_Destroy(uint8_t instance)
{
    (void)(instance);

    // Release our only instance we may have
    Instances = 0;
}

static void midiout_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    // We are not posting messages to the processing chain, so we don't care about filter step
    (void)(instance);
    (void)(filter_step);
}

static void midiout_LoadConfig(uint8_t instance, void* data)
{
    (void)(instance);

    ConfigUseRunningStatus = *((uint8_t*)data);
}

static void midiout_SaveConfig(uint8_t instance, void* data)
{
    (void)(instance);

    *((uint8_t*)data) = ConfigUseRunningStatus;
}

static uint8_t midiout_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    uint8_t i;

    (void)(instance);

    if (midi_getDataType(msg->Data[MIDIMSG_STATUS]) == MIDI_TYPE_SYS_REALTIME)
    {
        // Handle this separately to avoid affecting running status
        for (i = 0; i < msg->DataLen; i++)
        {
            hal_midiTxEnqueue_ISR(msg->Data[i]);
        }

        midilog_handleMidiOutRtSysx_ISR(msg);
    }
    else
    {
        if (ConfigUseRunningStatus && (msg->Data[MIDIMSG_STATUS] == LastStatus))
        {
            // Do running status, which means not sending it
            msg->Flags |= MIDIMSG_FLAG_RUNN_STATUS;
        }
        else
        {
            // Send status
            msg->Flags &= ~MIDIMSG_FLAG_RUNN_STATUS;

            hal_midiTxEnqueue_ISR(msg->Data[MIDIMSG_STATUS]);
            LastStatus = msg->Data[MIDIMSG_STATUS];
        }

        for (i = 1; i < msg->DataLen; i++)
        {
            hal_midiTxEnqueue_ISR(msg->Data[i]);
        }

        midilog_handleMidiOut_ISR(msg);
    }


    return FILTER_PROCESS_DISCARD;
}


// Menu integration
static void midiout_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);

    if (menu_item == 0)
    {
        util_copyString_P(dest, MenuTitle);
    }
    else if (menu_item == 1)
    {
        util_writeFormat_P(dest, MenuSetting, ConfigUseRunningStatus);
    }
}


static void midiout_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    (void)(instance);

    if (menu_item == 1)
    {
        // Handle up down moves on menu_item 1
        switch (ui_event & (~UI_MOVE_FAST_MASK))
        {
        case UI_EVENT_MOVE_DOWN:
            ConfigUseRunningStatus = FALSE;
            break;
        case UI_EVENT_MOVE_UP:
            ConfigUseRunningStatus = TRUE;
            break;
        }
    }
}

