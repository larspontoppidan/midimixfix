
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
#include "../common.h"
#include "../midimessage.h"
#include "../errors.h"
#include "../util.h"
#include "../hal.h"
#include "../filters.h"
#include "../ui.h"
#include <avr/pgmspace.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------

static const char MenuTitle[9]    PROGMEM = "MIDI Out";
static const char MenuSetting[18] PROGMEM = "Runn. status : %o";


// ----------------------------  LOCAL VARIABLES  -------------------------------

// Configuration
static bool_t ConfigUseRunningStatus;

// State
static uint8_t Instances;
static uint8_t LastStatus;

// ------------------------------  PROTOTYPES  ----------------------------------


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void midiout_initialize(void)
{
    Instances = 0;
    LastStatus = 0xFF;
}

bool_t midiout_new(uint8_t filter_type, uint8_t *config, filter_t* self)
{
    bool_t ret = FALSE;

    (void)(filter_type);

    if (Instances == 0)
    {
        // Okay, lets do this
        Instances = 1;

        if (config == NULL)
        {
            // Default config
            ConfigUseRunningStatus = FALSE;
        }
        else
        {
            // Load from config pointer
            ConfigUseRunningStatus = *config;
        }

        self->RouteOut = MIDIMSG_ROUTE_INACTIVE;
        ret = TRUE;
    }

    return ret;
}

uint8_t midiout_request(filter_t* self, uint8_t request)
{
    uint8_t ret = 0;

    switch (request)
    {
    case FILTER_REQ_DESTROY:
        Instances = 0;
        break;
    case FILTER_REQ_CONF_SIZE:
        ret = 1;
        break;
    case FILTER_REQ_MENU_ITEMS:
        ret = 1;
        break;
    case FILTER_REQ_MENU_INDENT:
        ret = 15;
        break;
    case FILTER_REQ_UPDATE_SELF:
        // Just enforce the route out setting
        self->RouteOut = MIDIMSG_ROUTE_INACTIVE;
        break;
    }

    return ret;
}

// Process midi msg
void midiout_processMidiMsg(filter_t* self, midiMsg_t *msg)
{
    uint8_t i;

    (void)(self);

    if (ConfigUseRunningStatus && (msg->Data[MIDIMSG_STATUS] == LastStatus))
    {
        // Do running status, which means not sending it
    }
    else
    {
        // Send status
        hal_midiTxEnqueue_ISR(msg->Data[MIDIMSG_STATUS]);
        LastStatus = msg->Data[MIDIMSG_STATUS];
    }

    for (i = 1; i < msg->DataLen; i++)
    {
        hal_midiTxEnqueue_ISR(msg->Data[i]);
    }

    // We are done with this msg, better discard it!
    msg->Route = MIDIMSG_ROUTE_INACTIVE;
}


void midiout_getMenuTitle(uint8_t filter_type, char *dest)
{
    (void)(filter_type);

    util_copyString_P(dest, MenuTitle);
}

// Menu integration
void midiout_getMenuText(filter_t* self, char *dest, uint8_t item)
{
    (void)(self);

    if (item == 0)
    {
        util_writeFormat_P(dest, MenuSetting, ConfigUseRunningStatus);
    }
}

void midiout_handleUiEvent(filter_t* self, uint8_t item, uint8_t user_event)
{
    (void)(self);

    if (item == 0)
    {
        // Handle up down moves on item 0
        switch (user_event & (~UI_MOVE_FAST_MASK))
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

void midiout_saveConfig(filter_t* self, uint8_t *dest)
{
    (void)(self);

    *dest = ConfigUseRunningStatus;
}
