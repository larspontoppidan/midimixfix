
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
#include "../filterinterface.h"
#include "../errors.h"
#include "../util.h"
#include <avr/pgmspace.h>
#include "filteruids.h"

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t genericCreate(uint8_t *step, uint8_t filter_step);

static void    midiin_ConfigNull(uint8_t instance, void* data);
static uint8_t midiin_ProcessMidiMsgNull(uint8_t instance, midiMsg_t *msg);
static void    midiin_HandleUiEventNull(uint8_t instance, uint8_t menu_item, uint8_t ui_event);

// Functions for the six filters:
static uint8_t midiin_In1Create(uint8_t filter_step);
static uint8_t midiin_In2Create(uint8_t filter_step);
static uint8_t midiin_In1RtCreate(uint8_t filter_step);
static uint8_t midiin_In2RtCreate(uint8_t filter_step);
static uint8_t midiin_In1SxCreate(uint8_t filter_step);
static uint8_t midiin_In2SxCreate(uint8_t filter_step);

static void midiin_In1Destroy(uint8_t instance);
static void midiin_In2Destroy(uint8_t instance);
static void midiin_In1RtDestroy(uint8_t instance);
static void midiin_In2RtDestroy(uint8_t instance);
static void midiin_In1SxDestroy(uint8_t instance);
static void midiin_In2SxDestroy(uint8_t instance);

static void midiin_In1SetFilterStep(uint8_t instance, uint8_t filter_step);
static void midiin_In2SetFilterStep(uint8_t instance, uint8_t filter_step);
static void midiin_In1RtSetFilterStep(uint8_t instance, uint8_t filter_step);
static void midiin_In2RtSetFilterStep(uint8_t instance, uint8_t filter_step);
static void midiin_In1SxSetFilterStep(uint8_t instance, uint8_t filter_step);
static void midiin_In2SxSetFilterStep(uint8_t instance, uint8_t filter_step);

static void midiin_In1WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void midiin_In2WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void midiin_In1RtWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void midiin_In2RtWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void midiin_In1SxWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void midiin_In2SxWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);


// --------------------------  TYPES AND CONSTANTS  -----------------------------

typedef struct
{
    // --- Parsing state
    bool_t Receiving;
    midiMsg_t Msg;
    uint8_t ExpectedDataCount;

    // For implementation of running status
    bool_t AllowRunningStatus;

    // --- Configuration of this particular input
    uint8_t NormalMsgStep;    // Where goes normal messages
    uint8_t RealtimeMsgStep;  // Where goes realtime messages
    uint8_t SysexMsgStep;     // Where goes sysex messages

} inputState_t;


// ------------------------------  VARIABLES  ---------------------------------

static inputState_t Input1State;
static inputState_t Input2State;

static const char MenuTitleIn1[] PROGMEM = "MIDI In1";
static const char MenuTitleIn2[] PROGMEM = "MIDI In2";
static const char MenuTitleIn1Rt[] PROGMEM = "MIDI In1 Realt";
static const char MenuTitleIn2Rt[] PROGMEM = "MIDI In2 Realt";
static const char MenuTitleIn1Sx[] PROGMEM = "MIDI In1 Sysex";
static const char MenuTitleIn2Sx[] PROGMEM = "MIDI In2 Sysex";

const filterInterface_t midiin_In1 PROGMEM =
{
        midiin_In1Create,
        midiin_In1Destroy,
        midiin_In1SetFilterStep,
        midiin_ConfigNull,
        midiin_ConfigNull,
        midiin_ProcessMidiMsgNull,
        midiin_In1WriteMenuText,
        midiin_HandleUiEventNull,

        0,               // Number of bytes in configuration
        0,               // Number of menu items (0 means only title, 1 is one item)
        0,               // Cursor indentation in menu
        FILTER_MODE_OUT, // Filter operation mode
        MenuTitleIn1,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_MIDIIN_IN1, 1)
};

const filterInterface_t midiin_In2 PROGMEM =
{
        midiin_In2Create,
        midiin_In2Destroy,
        midiin_In2SetFilterStep,
        midiin_ConfigNull,
        midiin_ConfigNull,
        midiin_ProcessMidiMsgNull,
        midiin_In2WriteMenuText,
        midiin_HandleUiEventNull,

        0,               // Number of bytes in configuration
        0,               // Number of menu items (0 means only title, 1 is one item)
        0,               // Cursor indentation in menu
        FILTER_MODE_OUT, // Filter operation mode
        MenuTitleIn2,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_MIDIIN_IN2, 1)
};

const filterInterface_t midiin_In1Realtime PROGMEM =
{
        midiin_In1RtCreate,
        midiin_In1RtDestroy,
        midiin_In1RtSetFilterStep,
        midiin_ConfigNull,
        midiin_ConfigNull,
        midiin_ProcessMidiMsgNull,
        midiin_In1RtWriteMenuText,
        midiin_HandleUiEventNull,

        0,               // Number of bytes in configuration
        0,               // Number of menu items (0 means only title, 1 is one item)
        0,               // Cursor indentation in menu
        FILTER_MODE_OUT, // Filter operation mode
        MenuTitleIn1Rt,  // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_MIDIIN_IN1REALTIME, 1)
};

const filterInterface_t midiin_In2Realtime PROGMEM =
{
        midiin_In2RtCreate,
        midiin_In2RtDestroy,
        midiin_In2RtSetFilterStep,
        midiin_ConfigNull,
        midiin_ConfigNull,
        midiin_ProcessMidiMsgNull,
        midiin_In2RtWriteMenuText,
        midiin_HandleUiEventNull,

        0,               // Number of bytes in configuration
        0,               // Number of menu items (0 means only title, 1 is one item)
        0,               // Cursor indentation in menu
        FILTER_MODE_OUT, // Filter operation mode
        MenuTitleIn2Rt,  // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_MIDIIN_IN2REALTIME, 1)
};

const filterInterface_t midiin_In1Sysex PROGMEM =
{
        midiin_In1SxCreate,
        midiin_In1SxDestroy,
        midiin_In1SxSetFilterStep,
        midiin_ConfigNull,
        midiin_ConfigNull,
        midiin_ProcessMidiMsgNull,
        midiin_In1SxWriteMenuText,
        midiin_HandleUiEventNull,

        0,               // Number of bytes in configuration
        0,               // Number of menu items (0 means only title, 1 is one item)
        0,               // Cursor indentation in menu
        FILTER_MODE_OUT, // Filter operation mode
        MenuTitleIn1Sx,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_MIDIIN_IN1SYSEX, 1)
};

const filterInterface_t midiin_In2Sysex PROGMEM =
{
        midiin_In2SxCreate,
        midiin_In2SxDestroy,
        midiin_In2SxSetFilterStep,
        midiin_ConfigNull,
        midiin_ConfigNull,
        midiin_ProcessMidiMsgNull,
        midiin_In2SxWriteMenuText,
        midiin_HandleUiEventNull,

        0,               // Number of bytes in configuration
        0,               // Number of menu items (0 means only title, 1 is one item)
        0,               // Cursor indentation in menu
        FILTER_MODE_OUT, // Filter operation mode
        MenuTitleIn2Sx,    // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_MIDIIN_IN2SYSEX, 1)
};


// ---------------------------  PRIVATE FUNCTIONS  ------------------------------

static uint8_t genericCreate(uint8_t *step, uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;

    if (*step == FILTER_STEP_NA)
    {
        *step = filter_step;
        ret = 0;
    }

    return ret;
}

// ------------------------  MIDI FILTER INTERFACE FUNCTIONS  -------------------

static void midiin_ConfigNull(uint8_t instance, void* data)
{
    (void)(instance);
    (void)(data);
}

static uint8_t midiin_ProcessMidiMsgNull(uint8_t instance, midiMsg_t *msg)
{
    (void)(instance);
    (void)(msg);
    return 0;
}

static void midiin_HandleUiEventNull(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    (void)(instance);
    (void)(menu_item);
    (void)(ui_event);
}

static uint8_t midiin_In1Create(uint8_t filter_step)
{
    return genericCreate(&(Input1State.NormalMsgStep), filter_step);
}
static uint8_t midiin_In2Create(uint8_t filter_step)
{
    return genericCreate(&(Input2State.NormalMsgStep), filter_step);
}
static uint8_t midiin_In1RtCreate(uint8_t filter_step)
{
    return genericCreate(&(Input1State.RealtimeMsgStep), filter_step);
}
static uint8_t midiin_In2RtCreate(uint8_t filter_step)
{
    return genericCreate(&(Input2State.RealtimeMsgStep), filter_step);
}
static uint8_t midiin_In1SxCreate(uint8_t filter_step)
{
    return genericCreate(&(Input1State.SysexMsgStep), filter_step);
}
static uint8_t midiin_In2SxCreate(uint8_t filter_step)
{
    return genericCreate(&(Input2State.SysexMsgStep), filter_step);
}


static void midiin_In1Destroy(uint8_t instance)
{
    (void)(instance);
    Input1State.NormalMsgStep = FILTER_STEP_NA;
}
static void midiin_In2Destroy(uint8_t instance)
{
    (void)(instance);
    Input2State.NormalMsgStep = FILTER_STEP_NA;
}
static void midiin_In1RtDestroy(uint8_t instance)
{
    (void)(instance);
    Input1State.RealtimeMsgStep = FILTER_STEP_NA;
}
static void midiin_In2RtDestroy(uint8_t instance)
{
    (void)(instance);
    Input2State.RealtimeMsgStep = FILTER_STEP_NA;
}
static void midiin_In1SxDestroy(uint8_t instance)
{
    (void)(instance);
    Input1State.SysexMsgStep = FILTER_STEP_NA;
}
static void midiin_In2SxDestroy(uint8_t instance)
{
    (void)(instance);
    Input2State.SysexMsgStep = FILTER_STEP_NA;
}



static void midiin_In1SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    Input1State.NormalMsgStep = filter_step;
}
static void midiin_In2SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    Input2State.NormalMsgStep = filter_step;
}
static void midiin_In1RtSetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    Input1State.RealtimeMsgStep = filter_step;
}
static void midiin_In2RtSetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    Input2State.RealtimeMsgStep = filter_step;
}
static void midiin_In1SxSetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    Input1State.SysexMsgStep = filter_step;
}
static void midiin_In2SxSetFilterStep(uint8_t instance, uint8_t filter_step)
{
    (void)(instance);
    Input2State.SysexMsgStep = filter_step;
}


static void midiin_In1WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);
    (void)(menu_item);
    util_copyString_P(dest, MenuTitleIn1);
}
static void midiin_In2WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);
    (void)(menu_item);
    util_copyString_P(dest, MenuTitleIn2);
}
static void midiin_In1RtWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);
    (void)(menu_item);
    util_copyString_P(dest, MenuTitleIn1Rt);
}
static void midiin_In2RtWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);
    (void)(menu_item);
    util_copyString_P(dest, MenuTitleIn2Rt);
}
static void midiin_In1SxWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);
    (void)(menu_item);
    util_copyString_P(dest, MenuTitleIn1Sx);
}
static void midiin_In2SxWriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    (void)(instance);
    (void)(menu_item);
    util_copyString_P(dest, MenuTitleIn2Sx);
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void midiin_initialize(void)
{
    // Reset the inputs
    Input1State.Receiving = FALSE;
    Input1State.AllowRunningStatus = FALSE;
    Input1State.NormalMsgStep = FILTER_STEP_NA;
    Input1State.RealtimeMsgStep = FILTER_STEP_NA;
    Input1State.SysexMsgStep = FILTER_STEP_NA;

    Input2State.Receiving = FALSE;
    Input2State.AllowRunningStatus = FALSE;
    Input2State.NormalMsgStep = FILTER_STEP_NA;
    Input2State.RealtimeMsgStep = FILTER_STEP_NA;
    Input2State.SysexMsgStep = FILTER_STEP_NA;
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
        // Create a realtime msg on stack and post it for processing right away
        midiMsg_t rt_msg;

        rt_msg.Flags = MIDIMSG_FLAG_PARSE_OK;;
        rt_msg.DataLen = 1u;
        rt_msg.Data[MIDIMSG_STATUS] = x;

        midilog_handleMidiIn1RtSysx_ISR(&rt_msg);

        if (Input1State.RealtimeMsgStep != FILTER_STEP_NA)
        {
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

                    if (Input1State.NormalMsgStep != FILTER_STEP_NA)
                    {
                        midiproc_addMessage_ISR(&(Input1State.Msg), Input1State.NormalMsgStep);
                    }

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

                        if (Input1State.NormalMsgStep != FILTER_STEP_NA)
                        {
                            midiproc_addMessage_ISR(&(Input1State.Msg), Input1State.NormalMsgStep);
                        }

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
        // Create a realtime msg on stack and post it for processing right away
        midiMsg_t rt_msg;

        rt_msg.Flags = MIDIMSG_FLAG_PARSE_OK;
        rt_msg.DataLen = 1u;
        rt_msg.Data[MIDIMSG_STATUS] = x;

        midilog_handleMidiIn2RtSysx_ISR(&rt_msg);

        if (Input2State.RealtimeMsgStep != FILTER_STEP_NA)
        {
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

                    if (Input2State.NormalMsgStep != FILTER_STEP_NA)
                    {
                        midiproc_addMessage_ISR(&(Input2State.Msg), Input2State.NormalMsgStep);
                    }

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

                        if (Input2State.NormalMsgStep != FILTER_STEP_NA)
                        {
                            midiproc_addMessage_ISR(&(Input2State.Msg), Input2State.NormalMsgStep);
                        }

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

bool_t midiin_getIsReceivingStatus(void)
{
    return (Input1State.Receiving | Input2State.Receiving);
}
