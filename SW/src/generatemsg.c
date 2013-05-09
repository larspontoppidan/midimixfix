
// COPYRIGHT AND LICENSE
// ---------------------
// Copyright (c) 2013, Lars Ole Pontoppidan (Larsp)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the original author (Lars Ole Pontoppidan) nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
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
// FILE INFORMATION
// ----------------
// Filename:        generatemsg.c
// Code module:     Message Generator MidiMixFix component
// Public prefix:   genmsg
//
//
// DESCRIPTION
// -----------
// The Message Generator component enables the user to send midi messages defined
// through the Midimixfix menu system. The user specifies a number of midi messages
// and the corresponding number of submenus appear. Whenever a message is edited,
// it is sent through MidiOut.
//
// Implements:
// * Menu
// * Config save and load
//
// Implementation concept
// The messages are actually mmsg_t messages.
//
//
// CHANGE LOG
// ----------
// 2013-01-19: File created for MidiMixFix project by Larsp
//



// ------------
//   INCLUDES
// ------------

#include "common.h"
#include "util.h"
#include "pgmstrings.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "midiio.h"
#include "menu.h"
#include "generatemsg.h"
#include "avr/pgmspace.h"
#include "errors.h"
#include <string.h>

// -----------------------
//   TYPES AND CONSTANTS
// -----------------------

typedef struct
{
    uint8_t MidiStatus;
    uint8_t MidiParam[2];
} message_t;

#define MESSAGES_MAX 20

// Menu system defines
#define EDIT_MODE_ROOT    0
#define EDIT_MODE_CHANNEL 1
#define EDIT_MODE_TYPE    2
#define EDIT_MODE_VALUE   3


// -------------------
//   LOCAL VARIABLES
// -------------------

static uint8_t   MessageCount;
static message_t Messages[MESSAGES_MAX];

static char TitleString[] PROGMEM = "Generate msg. ";

// ---------------------
//   PRIVATE FUNCTIONS
// ---------------------

static void ResetMessage(uint8_t msg)
{
    // Default to program change on channel 1 patch 1
    Messages[msg].MidiStatus = MIDI_STATUS_PROG_CHANGE;
    Messages[msg].MidiParam[0] = 0;
}

static void EditChannel(uint8_t msg, int8_t delta)
{
    uint8_t chan;

    // Modify lower nibble of midi status with delta:

    chan = Messages[msg].MidiStatus & MIDI_CHANNEL_MASK;
    chan = util_boundedAddInt8(chan, 0, 15, delta);
    Messages[msg].MidiStatus = (Messages[msg].MidiStatus & (~MIDI_CHANNEL_MASK)) | chan;
}

static void EditType(uint8_t msg, int8_t delta)
{
    uint8_t status;

    status = Messages[msg].MidiStatus & MIDI_STATUS_MASK;

    while (delta > 0)
    {
        switch (status)
        {
        default:
        case MIDI_STATUS_PROG_CHANGE:
            status = MIDI_STATUS_CHAN_ATOUCH;
            Messages[msg].MidiParam[0] = 0;
            break;

        case MIDI_STATUS_CHAN_ATOUCH:
            status = MIDI_STATUS_CTRL_CHANGE;
            Messages[msg].MidiParam[0] = 0;
            Messages[msg].MidiParam[1] = 0;
            break;

        case MIDI_STATUS_CTRL_CHANGE:
            if (Messages[msg].MidiParam[0] < 127)
            {
                (Messages[msg].MidiParam[0])++;
            }
            break;
        }

        delta--;
    }

    while (delta < 0)
    {
        switch (status)
        {
        case MIDI_STATUS_CTRL_CHANGE:
            if (Messages[msg].MidiParam[0] > 0)
            {
                (Messages[msg].MidiParam[0])--;
            }
            else
            {
                status = MIDI_STATUS_CHAN_ATOUCH;
                Messages[msg].MidiParam[0] = 0;
            }
            break;

        default:
        case MIDI_STATUS_CHAN_ATOUCH:
            status = MIDI_STATUS_PROG_CHANGE;
            Messages[msg].MidiParam[0] = 0;
        }

        delta++;
    }

    Messages[msg].MidiStatus = (Messages[msg].MidiStatus & (~MIDI_STATUS_MASK)) | status;
}

static void EditValue(uint8_t msg, int8_t delta)
{
    uint8_t status;

    status = Messages[msg].MidiStatus & MIDI_STATUS_MASK;

    if (status == MIDI_STATUS_CTRL_CHANGE)
    {
        Messages[msg].MidiParam[1] = util_boundedAddInt8(Messages[msg].MidiParam[1], 0, 127, delta);
    }
    else
    {
        Messages[msg].MidiParam[0] = util_boundedAddInt8(Messages[msg].MidiParam[0], 0, 127, delta);
    }
}


// Actually send the message
static void SendMessage(uint8_t msg)
{
    uint8_t msg_index;
    uint8_t i;
    uint8_t len;

    // Reserve an index in the midiio buffer
    msg_index = MidiIo_msgNew_MAIN(MMSG_SOURCE_GENERATED | MMSG_FLAG_MSG_OK, Messages[msg].MidiStatus);

    // Ensure we use the correct length
    len = Midi_getDataCount(Messages[msg].MidiStatus);

    // Put in the data
    for (i = 0; i < len; i++)
    {
        MidiIo_msgAddData_ISR(msg_index, Messages[msg].MidiParam[i]);
    }

    // Finish so it will be sent
    MidiIo_msgFinish_ISR(msg_index, 0);
}


// --------------------
//   PUBLIC FUNCTIONS
// --------------------


void genmsg_initialize(void)
{
    uint8_t i;

    MessageCount = 0;

    for (i = 0; i < MESSAGES_MAX; i++)
    {
        ResetMessage(i);
    }
}

// =============================  PUBLIC FUNCTIONS  ===========================


uint8_t genmsg_menuGetSubCount(void)
{
    return MessageCount;
}


// -----------------------------  PUBLIC FUNCTIONS  ---------------------------

void genmsg_menuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        util_strCpy_P(dest, TitleString);
        util_strWriteNumberParentheses(&(dest[14]), MessageCount);
    }
    else
    {
        // Write the decoded message. Move from message to mmsg_t
        midiMsg_t msg;
        msg.Flags = MMSG_FLAG_MSG_OK;
        msg.MidiStatus = Messages[item-1].MidiStatus;
        msg.Data[0] = Messages[item-1].MidiParam[0];
        msg.Data[1] = Messages[item-1].MidiParam[1];
        msg.Data[2] = Messages[item-1].MidiParam[2];
        msg.DataLen = Midi_getDataCount(msg.MidiStatus);

        MidiMsg_writeParsed(dest, &msg);
    }
}

uint8_t genmsg_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

     if (item == 0)
     {
         if (edit_mode == 0)
         {
             if (user_event == MENU_EVENT_SELECT)
             {
                 // We are at root level and user wants to edit title menu
                 // Ok, move cursor to (X)
                 ret = 17;
             }
         }
         else if (edit_mode == 1)
         {
             if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
             {
                 // Toggle number of filters
                 MessageCount = util_boundedAddInt8(MessageCount, 0, MESSAGES_MAX, knob_delta);

                 // Keep cursor at position, and notify menu that this may alter our submenu
                 ret = 17 | MENU_UPDATE_ALL;
             }
         }
     }
     else
     {
         uint8_t msg = item - 1;

         if (user_event == MENU_EVENT_SELECT)
         {
             // User steps into next edit mode compared to what we
             // are at now:
             edit_mode++;
         }

         // If user turns pushed, speed things up by x10
         if (user_event == MENU_EVENT_TURN_PUSH)
         {
             knob_delta *= 10;
         }

         switch (edit_mode)
         {
         case EDIT_MODE_CHANNEL:
             ret = 2;
             EditChannel(msg, knob_delta);
             break;

         case EDIT_MODE_TYPE:
             ret = 4;
             EditType(msg, knob_delta);
             break;

         case EDIT_MODE_VALUE:
             ret = 12;
             if (knob_delta != 0)
             {
                 EditValue(msg, knob_delta);

                 // User turned the knob on a value, lets send the message
                 SendMessage(msg);
             }
             break;
         }

     }

     return ret;
}

uint8_t genmsg_configGetSize(void)
{
    return (MESSAGES_MAX * sizeof(message_t) + 1);
}

void genmsg_configSave(uint8_t *dest)
{
    *(dest++) = MessageCount;
    memcpy(dest, &(Messages[0]), MESSAGES_MAX * sizeof(message_t));
}

void genmsg_configLoad(uint8_t *dest)
{
    uint8_t i;

    MessageCount = *(dest++);
    memcpy(&(Messages[0]), dest, MESSAGES_MAX * sizeof(message_t));

    // Send all messages after preset load!
    for (i = 0; i < MessageCount; i++)
    {
        SendMessage(i);
    }
}
