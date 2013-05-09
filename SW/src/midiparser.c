/*
 * midiparser.c
 *
 *  Created on: Dec 15, 2012
 *      Author: lars
 */

#include "common.h"
#include "midiparser.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "midiio.h"

// The job of the parser is to "tokenize" data from the midi data stream
// into calls in midiio module.
//
// Example of normal parser operation: TODO update this table
//
//               Midi   msg calls     Parser state after processing
//               ----   -----------      ------------
//
// System clk    F8     New
//                      Finish
// Note on       81     New
// Data 1        54     AddData
// Data 2        02     AddData
//                      Finish
// Data 1 (r.s.) 55     New
//                      AddData
// Data 2        02     AddData
//                      Finish
// System sense  FE
// SYSEX start   F0
// data          xx
// data          xx
// (...)
// data          xx
// data          xx
// SYSEX end     F7


typedef struct
{
    bool_t  receiving;

    uint8_t msg_index; // For referring in midiio

    uint8_t data_count;
    uint8_t expected_data_count;

    // For implementation of running status
    uint8_t midi_status;
    bool_t  allow_running_status;

} mparse_State_t;

mparse_State_t Input1State;
mparse_State_t Input2State;

void mparser_initialize(void)
{
    Input1State.receiving = FALSE;
    Input1State.allow_running_status = FALSE;

    Input2State.receiving = FALSE;
    Input2State.allow_running_status = FALSE;
}


void mparser_handleInput1Rx_Isr(uint8_t x)
{
    uint8_t type;

    type = Midi_getDataType(x);

    // Real time messages gets special handling right away
    if (type == MIDI_TYPE_SYS_REALTIME)
    {
        midiio_RealtimeMsgAdd_ISR(MMSG_SOURCE_INPUT1 | MMSG_FLAG_MSG_OK, x);
    }
    else
    {
        bool_t x_used = FALSE;

        do
        {
            if (Input1State.receiving == FALSE)
            {
                // So, we are starting a new message.
                Input1State.data_count = 0u;
                Input1State.receiving = TRUE;

                // Did we get a status byte?
                if (type != MIDI_TYPE_DATA)
                {
                    // Yes, we'll use it for status, create new message
                    Input1State.msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_INPUT1, x);
                    Input1State.midi_status = x;
                    x_used = TRUE;

                    // Predict expected length
                    Input1State.expected_data_count = Midi_getDataCount(x);

                    // Can we use this for running status?
                    if ((type == MIDI_TYPE_CHAN_VOICE) ||
                        (type == MIDI_TYPE_CHAN_MODE))
                    {
                        Input1State.allow_running_status = TRUE;
                    }
                    else
                    {
                        Input1State.allow_running_status = FALSE;
                    }
                }
                else
                {
                    // No. Can we transfer status byte from last telegram?
                    if (Input1State.allow_running_status)
                    {
                        // Ok, this is normal running status, create message
                        Input1State.msg_index = midiio_MsgNew_ISR(
                                MMSG_SOURCE_INPUT1 | MMSG_FLAG_RUNNING_STATUS,
                                Input1State.midi_status);

                        // Predict expected length
                        Input1State.expected_data_count = Midi_getDataCount(Input1State.midi_status);
                    }
                    else
                    {
                        // We can't do running status, so we don't know what status is!
                        // Add data as raw
                        Input1State.msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_INPUT1 | MMSG_FLAG_RAW, 0);

                        Input1State.expected_data_count = 0xFF;
                    }

                    // Note that we haven't actually used x yet.
                    // Next iteration in loop will take care of that
                }

                // Are we done already?
                if (Input1State.expected_data_count == Input1State.data_count)
                {
                    // Yep
                    midiio_MsgFinish_ISR(Input1State.msg_index, MMSG_FLAG_MSG_OK);
                    Input1State.receiving = FALSE;
                }
            }
            else
            {
                // We are receiving a message, meaning x is supposed to be a data byte, is it?

                if (type == MIDI_TYPE_DATA)
                {
                    // Yes, store it
                    midiio_MsgAddData_ISR(Input1State.msg_index, x);
                    Input1State.data_count++;
                    x_used = TRUE;

                    // Are we done now?
                    if (Input1State.expected_data_count == Input1State.data_count)
                    {
                        // Yep
                        midiio_MsgFinish_ISR(Input1State.msg_index, MMSG_FLAG_MSG_OK);
                        Input1State.receiving = FALSE;
                    }
                }
                else
                {
                    // Its a status bit...

                    // We must finish current message. Do so without the OK flag, since it didn't
                    // have the expected length
                    midiio_MsgFinish_ISR(Input1State.msg_index, 0u);
                    Input1State.receiving = FALSE;

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
void mparser_handleInput2Rx_Isr(uint8_t x)
{
    uint8_t type;

    type = Midi_getDataType(x);

    // Real time messages gets special handling right away
    if (type == MIDI_TYPE_SYS_REALTIME)
    {
        midiio_RealtimeMsgAdd_ISR(MMSG_SOURCE_INPUT2 | MMSG_FLAG_MSG_OK, x);
    }
    else
    {
        bool_t x_used = FALSE;

        do
        {
            if (Input2State.receiving == FALSE)
            {
                // So, we are starting a new message.
                Input2State.data_count = 0u;
                Input2State.receiving = TRUE;

                // Did we get a status byte?
                if (type != MIDI_TYPE_DATA)
                {
                    // Yes, we'll use it for status, create new message
                    Input2State.msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_INPUT2, x);
                    Input2State.midi_status = x;
                    x_used = TRUE;

                    // Predict expected length
                    Input2State.expected_data_count = Midi_getDataCount(x);

                    // Can we use this for running status?
                    if ((type == MIDI_TYPE_CHAN_VOICE) ||
                        (type == MIDI_TYPE_CHAN_MODE))
                    {
                        Input2State.allow_running_status = TRUE;
                    }
                    else
                    {
                        Input2State.allow_running_status = FALSE;
                    }
                }
                else
                {
                    // No. Can we transfer status byte from last telegram?
                    if (Input2State.allow_running_status)
                    {
                        // Ok, this is normal running status, create message
                        Input2State.msg_index = midiio_MsgNew_ISR(
                                MMSG_SOURCE_INPUT2 | MMSG_FLAG_RUNNING_STATUS,
                                Input2State.midi_status);

                        // Predict expected length
                        Input2State.expected_data_count = Midi_getDataCount(Input2State.midi_status);
                    }
                    else
                    {
                        // We can't do running status, so we don't know what status is!
                        // Add data as raw
                        Input2State.msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_INPUT2 | MMSG_FLAG_RAW, 0);

                        Input2State.expected_data_count = 0xFF;
                    }

                    // Note that we haven't actually used x yet.
                    // Next iteration in loop will take care of that
                }

                // Are we done already?
                if (Input2State.expected_data_count == Input2State.data_count)
                {
                    // Yep
                    midiio_MsgFinish_ISR(Input2State.msg_index, MMSG_FLAG_MSG_OK);
                    Input2State.receiving = FALSE;
                }
            }
            else
            {
                // We are receiving a message, meaning x is supposed to be a data byte, is it?

                if (type == MIDI_TYPE_DATA)
                {
                    // Yes, store it
                    midiio_MsgAddData_ISR(Input2State.msg_index, x);
                    Input2State.data_count++;
                    x_used = TRUE;

                    // Are we done now?
                    if (Input2State.expected_data_count == Input2State.data_count)
                    {
                        // Yep
                        midiio_MsgFinish_ISR(Input2State.msg_index, MMSG_FLAG_MSG_OK);
                        Input2State.receiving = FALSE;
                    }
                }
                else
                {
                    // Its a status bit...

                    // We must finish current message. Do so without the OK flag, since it didn't
                    // have the expected length
                    midiio_MsgFinish_ISR(Input2State.msg_index, 0u);
                    Input2State.receiving = FALSE;

                    // We didn't use x at this point.
                    // The outer loop will iterate and so x is handled as status for a new message
                }
            }

            // We may require another pass to actually use the data byte
        } while (!x_used);
    }

}

