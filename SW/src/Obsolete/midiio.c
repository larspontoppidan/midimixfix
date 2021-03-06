/*
 * Copyright (c) 2013, Lars Ole Pontoppidan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * midiin.c
 *
 *  Created on: Dec 13, 2012
 *      Author: lars
 */

#include "common.h"
#include "midiio.h"
#include "midimessage.h"
#include "errors.h"
#include "hal.h"
#include "componenthooks.h"
#include <util/delay.h>

// Ring buffer with midi messages:

#define MIDIIO_BUFFER_SIZE 16u

#define STATUS_EMPTY      0u
#define STATUS_RECEIVING  1u
#define STATUS_TRANSMIT   2u
#define STATUS_DISCARD    3u

static uint8_t   StatusBuffer[MIDIIO_BUFFER_SIZE];
static midiMsg_t MsgBuffer[MIDIIO_BUFFER_SIZE];

static uint8_t BufferHead;
static uint8_t BufferTail;

// Ring buffer principle:
//
// Two midi input channels and other sources can contribute to the
// FIFO midi message queue, stored in a ring buffer.
//
// The ring buffer records can have different states:
//     0  empty        Nothing here. The initial state
//     1  receiving    Record is currently being updated by receive ISR
//     2  transmit     It is decided to transmit this record
//     3  discard      Record must be discarded
//
// Invariants:
// - head points to a free to use element in buffer
// - tail points to the first to process element in buffer
// - if head==tail buffer is empty
//
//
// Empty buffer:
//  .... D/C  D/C   D/C  D/C   D/C ....
//                   ^
//                   |
//               tail head
//
//
// Message being received:
//  .... D/C  D/C    1    D/C   D/C ....
//                   ^     ^
//                   |     |
//                 tail   head
//
//
// Message being transmitted:
//  .... D/C  D/C    2    D/C   D/C ....
//                   ^     ^
//                   |     |
//                 tail   head
//
// Message done transmitting:
//  .... D/C  D/C    2    D/C   D/C ....
//                         ^
//                         |
//                     tail head
//




// New concept!

//
// Midiparser constructs the message itself, no individual adding of bytes here anymore.
//
// Midiio handles new midi messages. They are queued on the midi message queue.
//















// Currently transmitting status

static midiMsg_t *OutputMessage;
static uint8_t    OutputTransmitIndex;


// Configuration variables:

// Messages with ConfigDiscardFlags set will be discarded right away,
// with ConfigProcessFlags will be processed
static uint8_t ConfigDiscardFlags;
static uint8_t ConfigProcessFlags;

// Realtime messages with ConfigRtDiscardFlags set will be discarded right away,
// with ConfigRtProcessFlags will be processed
static uint8_t ConfigRtDiscardFlags;
static uint8_t ConfigRtProcessFlags;

// Transmit with running status
static bool_t  ConfigSendRunningStatus; // Todo: Implement this


void midiio_initialize(void)
{
    BufferHead = 0u;
    BufferTail = 0u;
    OutputMessage = NULL;
    OutputTransmitIndex = 0u;

    // Set default config
    ConfigDiscardFlags = 0u;
    ConfigProcessFlags = MIDIMSG_SOURCE_INPUT1 | MIDIMSG_SOURCE_INPUT2;

    ConfigRtDiscardFlags = MIDIMSG_SOURCE_INPUT1 | MIDIMSG_SOURCE_INPUT2;
    ConfigRtProcessFlags = 0u;

    ConfigSendRunningStatus = FALSE;
}

void midiio_setMode(uint8_t mode, uint8_t source, bool_t rt)
{
    // Transform the desired mode and source into ready to use
    // bits in ConfigFlags:

    bool_t discard = (mode == MIDIIO_MODE_DISCARD) ? source : 0;
    bool_t process = (mode == MIDIIO_MODE_PROCESS) ? source : 0;

    if (rt)
    {
        ConfigRtDiscardFlags = (ConfigRtDiscardFlags & ~source) | discard;
        ConfigRtProcessFlags = (ConfigRtProcessFlags & ~source) | process;
    }
    else
    {
        ConfigDiscardFlags = (ConfigDiscardFlags & ~source) | discard;
        ConfigProcessFlags = (ConfigProcessFlags & ~source) | process;
    }
}

uint8_t midiio_getMode(uint8_t source, bool_t rt)
{
    uint8_t ret = MIDIIO_MODE_THROUGH;

    if (rt)
    {
        if (ConfigRtDiscardFlags & source)
        {
            ret = MIDIIO_MODE_DISCARD;
        }
        else if (ConfigRtProcessFlags & source)
        {
            ret = MIDIIO_MODE_PROCESS;
        }
    }
    else
    {
        if (ConfigDiscardFlags & source)
        {
            ret = MIDIIO_MODE_DISCARD;
        }
        else if (ConfigProcessFlags & source)
        {
            ret = MIDIIO_MODE_PROCESS;
        }
    }

    return ret;
}

void midiio_setRunStatusMode(bool_t x)
{
    ConfigSendRunningStatus = x;
}

bool_t midiio_getRunStatusMode(void)
{
    return ConfigSendRunningStatus;
}

// Allocate new message, returns msg_index
uint8_t midiio_msgNew_ISR(uint8_t flags, uint8_t midi_status)
{
    uint8_t msg_index;

    // Lets use next free number in buffer
    msg_index = BufferHead;

    // Increment head variable
    BufferHead++;

    if (BufferHead >= MIDIIO_BUFFER_SIZE)
    {
        BufferHead = 0u;
    }

    if (BufferHead == BufferTail)
    {
        // Buffer overflow... Undefined behavior will follow...
        err_raise(ERR_MODULE_MIDIIO, __LINE__);
    }

    // Set status
    StatusBuffer[msg_index] = STATUS_RECEIVING;

    // Initialize msg
    MsgBuffer[msg_index].ReceivedTick = hal_tickCountGet_ISR();
    MsgBuffer[msg_index].Flags = flags;
    MsgBuffer[msg_index].MidiStatus = midi_status;
    MsgBuffer[msg_index].DataLen = 0;

    return msg_index;
}

// Allocate new message, returns msg_index
// This version of the function is intended to be called from
// main thread. It will BLOCK if buffer is full, and wait for a free
// spot before returning.
uint8_t midiio_msgNew_MAIN(uint8_t flags, uint8_t midi_status)
{
    uint8_t msg_index;
    bool_t got_msg_index;

    got_msg_index = FALSE;

    do
    {
        // Pull next free number in buffer with interrupts disabled

        hal_interruptsDisable();

        // Lets use next free number in buffer
        msg_index = BufferHead;

        // Increment head variable
        BufferHead++;

        if (BufferHead >= MIDIIO_BUFFER_SIZE)
        {
            BufferHead = 0u;
        }

        if (BufferHead == BufferTail)
        {
            // Buffer overflow... Undo what we just did
            BufferHead = msg_index;

            // Exit critical region and wait a short while before attempting
            // this again
            hal_interruptsEnable();
            _delay_ms(1);
        }
        else
        {
            // Buffer NOT overflow.
            // Set status for our new message and exit critical region
            StatusBuffer[msg_index] = STATUS_RECEIVING;
            hal_interruptsEnable();

            got_msg_index = TRUE;
        }

    } while (got_msg_index == FALSE);

    // Now we got the message with status receiving. This means it is currently
    // reserved for whoever is going to put data into it.

    // Initialize msg
    MsgBuffer[msg_index].ReceivedTick = hal_tickCountGet_MAIN();
    MsgBuffer[msg_index].Flags = flags;
    MsgBuffer[msg_index].MidiStatus = midi_status;
    MsgBuffer[msg_index].DataLen = 0;

    return msg_index;
}

midiMsg_t* midiio_msgGetPtr(uint8_t msg_index)
{
    return &(MsgBuffer[msg_index]);
}

// Add data to message
void midiio_msgAddData_ISR(uint8_t msg_index, uint8_t midi_data)
{
    uint8_t l;

    l = MsgBuffer[msg_index].DataLen;

    if (l < MIDIMSG_DATA_MAX)
    {
        MsgBuffer[msg_index].Data[l] = midi_data;
        MsgBuffer[msg_index].DataLen = l+1;
    }
    else
    {
        // No more room in this buffer... Discard the rest of the data for now...
        // TODO what about sysex
    }
}



// This function is called when message is complete and should be processed
void midiio_msgFinish_ISR(uint8_t msg_index, uint8_t flags)
{
    midiMsg_t *msg = &(MsgBuffer[msg_index]);

    // Or the last flags into message
    msg->Flags |= flags;

    if ((msg->Flags) & ConfigDiscardFlags)
    {
        // OK, we discard the message directly
        StatusBuffer[msg_index] = STATUS_DISCARD;
    }
    else
    {
        // Message survives so far
        if ((msg->Flags) & ConfigProcessFlags)
        {
            // Let components deal with message
            COMP_HOOKS_MIDI_MSG_IN_ISR(msg);
        }

        // Do we still want to send the message
        if ((msg->Flags) & MIDIMSG_FLAG_DISCARD)
        {
            StatusBuffer[msg_index] = STATUS_DISCARD;
        }
        else
        {
            // OK, send the message
            StatusBuffer[msg_index] = STATUS_TRANSMIT;
        }
    }

    // Make sure the transmit interrupt system is running in any case
    hal_midiTxEnableIsr_ISR(TRUE);
}


// Add realtime message (F8 <= midi_status <= FF)
void midiio_realtimeMsg_ISR(uint8_t flags, uint8_t midi_status)
{
    if (flags & ConfigRtDiscardFlags)
    {
        // Just ignore the message
    }
    else
    {
        // We need to deal with this message, add it
        uint8_t msg_index = midiio_msgNew_ISR(flags, midi_status);

        if (flags & ConfigRtProcessFlags)
        {
            // Let components deal with it
            COMP_HOOKS_RT_MIDI_MSG_ISR(&(MsgBuffer[msg_index]));
        }

        // Do we still want to send the message
        if (MsgBuffer[msg_index].Flags & MIDIMSG_FLAG_DISCARD)
        {
            StatusBuffer[msg_index] = STATUS_DISCARD;
        }
        else
        {
            // OK, send the message
            StatusBuffer[msg_index] = STATUS_TRANSMIT;
        }

        // Make sure the transmit interrupt system is running in any case
        hal_midiTxEnableIsr_ISR(TRUE);
    }
}


void midiio_outputTxComplete_ISR(void)
{
    // Is uart active right now?
    if (hal_midiTxGetActive_ISR() == FALSE)
    {
        // Are we currently transmitting a message?

        if (OutputMessage != NULL)
        {
            // Yes, send next byte
            // TODO implement running status if configured to do such
            hal_midiTxSend_ISR(midimsg_getRawByte(OutputMessage, OutputTransmitIndex));
            OutputTransmitIndex++;

            // More bytes left of this message?
            if (OutputTransmitIndex >= midimsg_getRawLength(OutputMessage))
            {
                // No, clear message pointer
                OutputMessage = NULL;

                // Increment BufferTail
                BufferTail++;

                if (BufferTail >= MIDIIO_BUFFER_SIZE)
                {
                    BufferTail = 0u;
                }
            }
        }
        else
        {
            // No, search for next message to send, if any

            while (BufferTail != BufferHead)
            {
                if (StatusBuffer[BufferTail] == STATUS_TRANSMIT)
                {
                    // We found a message to transmit
                    OutputMessage = &(MsgBuffer[BufferTail]);
                    hal_midiTxSend_ISR(midimsg_getRawByte(OutputMessage, 0));
                    OutputTransmitIndex = 1;

                    // Let output loggers peek at the message
                    COMP_HOOKS_MIDI_MSG_OUT_ISR(OutputMessage);

                    // More bytes left of this message?
                    if (OutputTransmitIndex >= midimsg_getRawLength(OutputMessage))
                    {
                        // No, clear message pointer
                        OutputMessage = NULL;

                        // Increment BufferTail
                        BufferTail++;

                        if (BufferTail >= MIDIIO_BUFFER_SIZE)
                        {
                            BufferTail = 0u;
                        }
                    }

                    // Break out of while
                    break;
                }
                else if (StatusBuffer[BufferTail] == STATUS_RECEIVING)
                {
                    // This message is being received. Stop searching,
                    // we will get started when it's done
                    hal_midiTxEnableIsr_ISR(FALSE);
                    break;
                }
                else
                {
                    // Increment BufferTail
                    BufferTail++;

                    if (BufferTail >= MIDIIO_BUFFER_SIZE)
                    {
                        BufferTail = 0u;
                    }
                }
            }

            if (BufferTail == BufferHead)
            {
                // We didn't find anything to send, or are already finished
                // Turn off this interrupt
                hal_midiTxEnableIsr_ISR(FALSE);
            }

        }
    }

}


