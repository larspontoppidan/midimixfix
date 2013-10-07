
//
// Filename    : midiinput.c
// Code module : Midi input sources for MidiMixFix
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

#include "midiin.h"
#include "../midilog.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../midiprocessing.h"
#include "../errors.h"
#include "../util.h"
#include <avr/pgmspace.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------

typedef struct
{
    // --- Parsing state
    bool_t  Receiving;

    midiMsg_t Msg;
    uint8_t MsgIndex; // OBSOLETE For referring in midiio

    uint8_t DataCount; // OBSOLETE
    uint8_t ExpectedDataCount;

    // For implementation of running status
    uint8_t MidiStatus; // OBSOLETE
    bool_t  AllowRunningStatus;

    // --- Configuration of this particular input
    uint8_t NormalMsgRoute;   // Where goes normal messages
    uint8_t NormalMsgStep;    // Where goes normal messages
    uint8_t RealtimeMsgRoute; // Where goes realtime messages
    uint8_t RealtimeMsgStep;  // Where goes realtime messages
    uint8_t SysexMsgRoute;    // Where goes sysex messages
    uint8_t SysexMsgStep;     // Where goes sysex messages

} inputState_t;


typedef struct
{
    char Text[15];
} menuTitle_t;

// ----------------------------  LOCAL VARIABLES  -------------------------------

inputState_t Input1State;
inputState_t Input2State;

static menuTitle_t const MenuTitles[6] PROGMEM =
{
    {"MIDI In1"},
    {"MIDI In2"},
    {"MIDI In1 Realt"},
    {"MIDI In2 Realt"},
    {"MIDI In1 Sysex"},
    {"MIDI In2 Sysex"}
};


// ------------------------------  PROTOTYPES  ----------------------------------

static void setRouteStep(uint8_t filter_type, uint8_t route, uint8_t step);
static uint8_t getRoute(uint8_t filter_type);

// ---------------------------  PRIVATE FUNCTIONS  ------------------------------

static void setRouteStep(uint8_t filter_type, uint8_t route, uint8_t step)
{
    switch (filter_type)
    {
    case FILTER_MIDIIN1:
        Input1State.NormalMsgRoute = route;
        Input1State.NormalMsgStep = step;
        break;
    case FILTER_MIDIIN2:
        Input2State.NormalMsgRoute = route;
        Input2State.NormalMsgStep = step;
        break;
    case FILTER_MIDIIN1_REALT:
        Input1State.RealtimeMsgRoute = route;
        Input1State.RealtimeMsgStep = step;
        break;
    case FILTER_MIDIIN2_REALT:
        Input2State.RealtimeMsgRoute = route;
        Input2State.RealtimeMsgStep = step;
        break;
    case FILTER_MIDIIN1_SYSEX:
        Input1State.SysexMsgRoute = route;
        Input1State.SysexMsgStep = step;
        break;
    case FILTER_MIDIIN2_SYSEX:
        Input2State.SysexMsgRoute = route;
        Input2State.SysexMsgStep = step;
        break;
    default:
        err_raise_MAIN(ERR_MODULE_MIDIINPUT, __LINE__);
        break;
    }
}

static uint8_t getRoute(uint8_t filter_type)
{
    uint8_t ret = 0;

    switch (filter_type)
    {
    case FILTER_MIDIIN1:
        ret = Input1State.NormalMsgRoute;
        break;
    case FILTER_MIDIIN2:
        ret = Input2State.NormalMsgRoute;
        break;
    case FILTER_MIDIIN1_REALT:
        ret = Input1State.RealtimeMsgRoute;
        break;
    case FILTER_MIDIIN2_REALT:
        ret = Input2State.RealtimeMsgRoute;
        break;
    case FILTER_MIDIIN1_SYSEX:
        ret = Input1State.SysexMsgRoute;
        break;
    case FILTER_MIDIIN2_SYSEX:
        ret = Input2State.SysexMsgRoute;
        break;
    default:
        err_raise_MAIN(ERR_MODULE_MIDIINPUT, __LINE__);
        break;
    }

    return ret;
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void midiin_initialize(void)
{
    // Reset the inputs
    Input1State.Receiving = FALSE;
    Input1State.AllowRunningStatus = FALSE;
    Input1State.NormalMsgRoute = MIDIMSG_ROUTE_INACTIVE;
    Input1State.RealtimeMsgRoute = MIDIMSG_ROUTE_INACTIVE;
    Input1State.SysexMsgRoute = MIDIMSG_ROUTE_INACTIVE;

    Input2State.Receiving = FALSE;
    Input2State.AllowRunningStatus = FALSE;
    Input2State.NormalMsgRoute = MIDIMSG_ROUTE_INACTIVE;
    Input2State.RealtimeMsgRoute = MIDIMSG_ROUTE_INACTIVE;
    Input2State.SysexMsgRoute = MIDIMSG_ROUTE_INACTIVE;
}

bool_t midiin_new(uint8_t filter_type, uint8_t *config, filter_t* self)
{
    bool_t success = FALSE;

    (void)(config);

    // Have we already created this particular filter type
    if (getRoute(filter_type) == MIDIMSG_ROUTE_INACTIVE)
    {
        // No, lets set this up
        self->FilterType = filter_type;
        self->Instance = 0;
        self->RouteIn = MIDIMSG_ROUTE_INACTIVE;
        setRouteStep(self->FilterType, self->RouteOut, self->StepNumber);

        success = TRUE;
    }

    return success;
}

uint8_t midiin_request(filter_t* self, uint8_t request)
{
    uint8_t ret = 0;

    switch (request)
    {
    case FILTER_REQ_DESTROY:
        // Release this instance of the filter
        setRouteStep(self->FilterType, MIDIMSG_ROUTE_INACTIVE, 0);
        break;

    case FILTER_REQ_UPDATE_SELF:
        // When the self data change, update accordingly:
        setRouteStep(self->FilterType, self->RouteOut, self->StepNumber);

        // and enforce what cannot change:
        self->Instance = 0;
        self->RouteIn = MIDIMSG_ROUTE_INACTIVE;
        break;
    }

    return ret;
}


// Menu integration.
// All we got is a menu title
void midiin_getMenuTitle(uint8_t filter_type, char *dest)
{
    util_copyString_P(dest, MenuTitles[filter_type].Text);
}


// MIDI raw data parsing engine
// ----------------------------

// TODO sysex is not handled

void midiin_handleInput1_ISR(uint8_t x)
{
    uint8_t type;

    type = midi_getDataType(x);

    // Real time messages gets special handling right away
    if (type == MIDI_TYPE_SYS_REALTIME)
    {
        if (Input1State.RealtimeMsgRoute != MIDIMSG_ROUTE_INACTIVE)
        {
            // Create a realtime msg on stack and post it for processing right away
            midiMsg_t rt_msg;

            midimsg_newSetStatus(&rt_msg, x);
            rt_msg.Route = Input1State.RealtimeMsgRoute;

            midilog_handleMidiIn1RtSysx_ISR(&rt_msg);
            midiproc_addMessage_ISR(&rt_msg, Input1State.RealtimeMsgStep);
        }
    }
    else
    {
        bool_t x_used = FALSE;

        do
        {
            if (Input1State.Receiving == FALSE)
            {
                // So, we are starting a new message.
                Input1State.Msg.DataLen = 0u;
                Input1State.Msg.Route = Input1State.NormalMsgRoute;
                Input1State.Msg.Flags = 0u;

                Input1State.Receiving = TRUE;

                // Did we get a status byte?
                if (type != MIDI_TYPE_DATA)
                {
                    // Yes, we'll use it for status, create new message
                    Input1State.Msg.DataLen = 1;
                    Input1State.Msg.Data[MIDIMSG_STATUS] = x;

                    x_used = TRUE;

                    // Predict expected length
                    Input1State.ExpectedDataCount = midi_getDataCount(x) + 1;

                    // Can we use this for running status?
                    if ((type == MIDI_TYPE_CHAN_VOICE) ||
                        (type == MIDI_TYPE_CHAN_MODE))
                    {
                        Input1State.AllowRunningStatus = TRUE;
                    }
                    else
                    {
                        Input1State.AllowRunningStatus = FALSE;
                    }
                }
                else
                {
                    // No. Can we transfer status byte from last telegram?
                    if (Input1State.AllowRunningStatus)
                    {
                        // Ok, this is normal running status, use midi status from last msg
                        Input1State.Msg.DataLen = 1;
                        Input1State.Msg.Flags |= MIDIMSG_FLAG_RUNN_STATUS;

                        // And keep expected length
                    }
                    else
                    {
                        // We can't do running status, so we don't know what status is!
                        Input1State.Msg.Flags |= MIDIMSG_FLAG_NO_STATUS;

                        // This kind of data may be routed as TODO SYSEX
                        // Input1State.Msg.Route = Input1State.SysexMsgRoute;

                        Input1State.ExpectedDataCount = 0xFF;
                    }

                    // Note that we haven't actually used x yet.
                    // Next iteration in loop will take care of that
                }

                // Are we done already?
                if (Input1State.ExpectedDataCount == Input1State.Msg.DataLen)
                {
                    // Yes, that means we have a well formed message
                    Input1State.Msg.Flags |= MIDIMSG_FLAG_PARSE_OK;

                    // Send for processing
                    midilog_handleMidiIn1_ISR(&(Input1State.Msg));
                    midiproc_addMessage_ISR(&(Input1State.Msg), Input1State.NormalMsgStep);

                    Input1State.Receiving = FALSE;
                }
            }
            else
            {
                // We are receiving a message, meaning x is supposed to be a data byte, is it?

                if (type == MIDI_TYPE_DATA)
                {
                    // Yes, store it
                    midimsg_addByte(&(Input1State.Msg), x);
                    x_used = TRUE;

                    // Are we done now?
                    if (Input1State.ExpectedDataCount == Input1State.Msg.DataLen)
                    {
                        // Yes, that means we have a well formed message
                        Input1State.Msg.Flags |= MIDIMSG_FLAG_PARSE_OK;

                        // Send for processing
                        midilog_handleMidiIn1_ISR(&(Input1State.Msg));
                        midiproc_addMessage_ISR(&(Input1State.Msg), Input1State.NormalMsgStep);

                        Input1State.Receiving = FALSE;
                    }
                }
                else
                {
                    // Its a status bit...

                    // We must finish current message. Do so without the OK flag, since it didn't
                    // have the expected length

                    Input1State.Msg.Flags |= MIDIMSG_FLAG_TOO_SHORT;

                    // We wont process this garbage,
                    // however the TODO MIDI log should show it for debugging purposes

                    Input1State.Receiving = FALSE;

                    // We didn't use x at this point.
                    // The outer loop will iterate and so x is handled as status for a new message
                }
            }

            // We may require another pass to actually use the data byte
        } while (!x_used);
    }
}

// The Input2Rx function is an exact copy of Input1Rx function, except that all occurrences of
// "Input1" is exchanged to "Input2" and "INPUT1" with "INPUT2"
//
// Note: Copying a function may not be elegant, but it allows the compiler to directly access the
//       input state struct members without indirection. Since these functions are called by ISR
//       and are always in the midi processing chain, they should be as lightweight as possible.
//

// TODO sysex is not handled

void midiin_handleInput2_ISR(uint8_t x)
{
    uint8_t type;

    type = midi_getDataType(x);

    // Real time messages gets special handling right away
    if (type == MIDI_TYPE_SYS_REALTIME)
    {
        if (Input2State.RealtimeMsgRoute != MIDIMSG_ROUTE_INACTIVE)
        {
            // Create a realtime msg on stack and post it for processing right away
            midiMsg_t rt_msg;

            midimsg_newSetStatus(&rt_msg, x);
            rt_msg.Route = Input2State.RealtimeMsgRoute;

            midilog_handleMidiIn2RtSysx_ISR(&rt_msg);
            midiproc_addMessage_ISR(&rt_msg, Input2State.RealtimeMsgStep);
        }
    }
    else
    {
        bool_t x_used = FALSE;

        do
        {
            if (Input2State.Receiving == FALSE)
            {
                // So, we are starting a new message.
                Input2State.Msg.DataLen = 0u;
                Input2State.Msg.Route = Input2State.NormalMsgRoute;
                Input2State.Msg.Flags = 0u;

                Input2State.Receiving = TRUE;

                // Did we get a status byte?
                if (type != MIDI_TYPE_DATA)
                {
                    // Yes, we'll use it for status, create new message
                    Input2State.Msg.DataLen = 1;
                    Input2State.Msg.Data[MIDIMSG_STATUS] = x;

                    x_used = TRUE;

                    // Predict expected length
                    Input2State.ExpectedDataCount = midi_getDataCount(x) + 1;

                    // Can we use this for running status?
                    if ((type == MIDI_TYPE_CHAN_VOICE) ||
                        (type == MIDI_TYPE_CHAN_MODE))
                    {
                        Input2State.AllowRunningStatus = TRUE;
                    }
                    else
                    {
                        Input2State.AllowRunningStatus = FALSE;
                    }
                }
                else
                {
                    // No. Can we transfer status byte from last telegram?
                    if (Input2State.AllowRunningStatus)
                    {
                        // Ok, this is normal running status, use midi status from last msg
                        Input2State.Msg.DataLen = 1;
                        Input2State.Msg.Flags |= MIDIMSG_FLAG_RUNN_STATUS;

                        // And keep expected length
                    }
                    else
                    {
                        // We can't do running status, so we don't know what status is!
                        Input2State.Msg.Flags |= MIDIMSG_FLAG_NO_STATUS;

                        // This kind of data may be routed as TODO SYSEX
                        // Input2State.Msg.Route = Input2State.SysexMsgRoute;

                        Input2State.ExpectedDataCount = 0xFF;
                    }

                    // Note that we haven't actually used x yet.
                    // Next iteration in loop will take care of that
                }

                // Are we done already?
                if (Input2State.ExpectedDataCount == Input2State.Msg.DataLen)
                {
                    // Yes, that means we have a well formed message
                    Input2State.Msg.Flags |= MIDIMSG_FLAG_PARSE_OK;

                    // Send for processing
                    midilog_handleMidiIn2_ISR(&(Input2State.Msg));
                    midiproc_addMessage_ISR(&(Input2State.Msg), Input2State.NormalMsgStep);

                    Input2State.Receiving = FALSE;
                }
            }
            else
            {
                // We are receiving a message, meaning x is supposed to be a data byte, is it?

                if (type == MIDI_TYPE_DATA)
                {
                    // Yes, store it
                    midimsg_addByte(&(Input2State.Msg), x);
                    x_used = TRUE;

                    // Are we done now?
                    if (Input2State.ExpectedDataCount == Input2State.Msg.DataLen)
                    {
                        // Yes, that means we have a well formed message
                        Input2State.Msg.Flags |= MIDIMSG_FLAG_PARSE_OK;

                        // Send for processing
                        midilog_handleMidiIn2_ISR(&(Input2State.Msg));
                        midiproc_addMessage_ISR(&(Input2State.Msg), Input2State.NormalMsgStep);

                        Input2State.Receiving = FALSE;
                    }
                }
                else
                {
                    // Its a status bit...

                    // We must finish current message. Do so without the OK flag, since it didn't
                    // have the expected length

                    Input2State.Msg.Flags |= MIDIMSG_FLAG_TOO_SHORT;

                    // We wont process this garbage,
                    // however the TODO MIDI log should show it for debugging purposes

                    Input2State.Receiving = FALSE;

                    // We didn't use x at this point.
                    // The outer loop will iterate and so x is handled as status for a new message
                }
            }

            // We may require another pass to actually use the data byte
        } while (!x_used);
    }
}
