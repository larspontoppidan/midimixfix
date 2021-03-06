/*
 * midimessage.c
 *
 *  Created on: Dec 13, 2012
 *      Author: lars
 */


#include "common.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "errors.h"
#include "util.h"
#include <avr/pgmspace.h>


// Number of chars written is typically 7, maximum 11.
//
// Examples:
// "-"           Empty
// "SS."         Only status
// ".AA"         One data byte (running status)
// "SS.AABB"     Two data bytes
// "AABBCCDDEE"  Raw message with 5 bytes

char *midimsg_writeRaw(char *dest, midiMsg_t *msg)
{
    if (msg->Flags == 0u)
    {
        // Nothing here
        *(dest++) = '-';
    }
    else
    {
        uint8_t i;

        // Well formed message?
        if (msg->Flags & MIDIMSG_FLAG_PARSE_OK)
        {
            // Write status if we didn't use running status
            if ((msg->Flags & MIDIMSG_FLAG_RUNN_STATUS) == 0)
            {
                dest = util_writeHex(dest, msg->Data[MIDIMSG_STATUS]);
            }

            *(dest++) = '.';

            // Write all the data
            for (i = 1u; i < msg->DataLen; i++)
            {
                dest = util_writeHex(dest, msg->Data[i]);
            }
        }
        else
        {
            // Not well formed message (or part of sysex)

            // Just write raw data
            for (i = 0u; i < msg->DataLen; i++)
            {
                dest = util_writeHex(dest, msg->Data[i]);
            }
        }


    }

    return dest;
}


char *midimsg_writeParsed(char *dest, midiMsg_t *msg)
{
    if (msg->Flags & MIDIMSG_FLAG_NO_STATUS)
    {
        dest = util_copyString_P(dest, PSTR("BAD:No status"));
    }
    else if (msg->Flags & MIDIMSG_FLAG_TOO_SHORT)
    {
        dest = util_copyString_P(dest, PSTR("BAD:Too short"));
    }
    else if (msg->Flags & MIDIMSG_FLAG_RAW)
    {
        dest = util_copyString_P(dest, PSTR("Raw data?"));
    }
    else if (msg->Flags & MIDIMSG_FLAG_PARSE_OK)
    {
        // We have well formed message, decode it
        uint8_t status = msg->Data[MIDIMSG_STATUS];

        if (status >= 0xF0)
        {
            uint8_t i;

            // Its a system status
            dest = midi_writeStatusName(dest, status);

            // Write parameters as numbers
            for (i = 0; i < midi_getDataCount(status); i++)
            {
                *(dest++) = ' ';
                dest = util_writeInt8LA(dest, msg->Data[MIDIMSG_DATA1 + i]);
            }
        }
        else
        {
            int16_t sw;

            // Its a channel + status

            // Write channel number followed by midi status
            *(dest++) = 'C';
            *(dest++) = 'h';
            dest = util_writeInt8LA(dest, (status & 0x0F)+1);
            *(dest++) = ' ';
            dest = midi_writeStatusName(dest, status);

            switch (status & 0xF0)
            {
            case MIDI_STATUS_NOTE_ON:
            case MIDI_STATUS_NOTE_OFF:
            case MIDI_STATUS_KEY_ATOUCH:
                // Decode note number
                dest = midi_writeNoteName(dest, msg->Data[MIDIMSG_DATA1]);
                *(dest++) = ' ';

                // Add velocity / pressure
                dest = util_writeInt8LA(dest, msg->Data[MIDIMSG_DATA2]);
                break;

            case MIDI_STATUS_CTRL_CHANGE:
                // Write controller name=value
                dest = midi_writeControllerName(dest, msg->Data[MIDIMSG_DATA1]);
                *(dest++) = '=';
                dest = util_writeInt8LA(dest, msg->Data[MIDIMSG_DATA2]);
                break;

            case MIDI_STATUS_PROG_CHANGE:
                // Write new prog name
                dest = util_writeInt16LA(dest, msg->Data[MIDIMSG_DATA1] + 1);
                break;

            case MIDI_STATUS_CHAN_ATOUCH:
                // Write value
                dest = util_writeInt8LA(dest, msg->Data[MIDIMSG_DATA1]);
                break;

            case MIDI_STATUS_PITCH_WHEEL:
                // Write value
                sw  = msg->Data[MIDIMSG_DATA1];       // LS 7 bits
                sw |= (int16_t)(msg->Data[MIDIMSG_DATA2]) << 7u; // MS 7 bits
                sw -= 8192;

                dest = util_writeInt16(dest, sw);
                break;
            }
        }
    }
    else
    {
        // Nothing here
        *(dest++) = '-';
    }

    return dest;
}


// This function converts NoteOn vel. 0 messages to regular NoteOff.
// It's allowed to use NoteOn vel. 0 as a NoteOff and some instruments
// indeed do (Yamaha pianos). However, we don't want to handle
// this inconsistency in all filters, so fix it once and for all
//
void midimsg_convertSpecialNoteOff(midiMsg_t *msg)
{
    if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == MIDI_STATUS_NOTE_ON)
    {
        if (msg->Data[MIDIMSG_DATA2] == 0)
        {
            // We got a NoteOn vel. 0 here, transform to NoteOff (keep vel. 0)
            msg->Data[MIDIMSG_STATUS] = (msg->Data[MIDIMSG_STATUS] & MIDI_CHANNEL_MASK) | MIDI_STATUS_NOTE_OFF;
        }
    }
}

void midimsg_setChan(midiMsg_t *msg, uint8_t chan)
{
    uint8_t ms = msg->Data[MIDIMSG_STATUS];

    // And away current channel, and add desired channel
    ms = (ms & MIDI_STATUS_MASK) | (chan & MIDI_CHANNEL_MASK);

    msg->Data[MIDIMSG_STATUS] = ms;
}


void midimsg_newProgramChange(midiMsg_t *msg, uint8_t chan, uint8_t part)
{
    msg->DataLen = 2;
    msg->Flags = MIDIMSG_FLAG_PARSE_OK;
    msg->Data[0] = MIDI_STATUS_PROG_CHANGE | chan;
    msg->Data[1] = part;
}

void midimsg_newChanAfterTouch(midiMsg_t *msg, uint8_t chan, uint8_t pressure)
{
    msg->DataLen = 2;
    msg->Flags = MIDIMSG_FLAG_PARSE_OK;
    msg->Data[0] = MIDI_STATUS_CHAN_ATOUCH | chan;
    msg->Data[1] = pressure;
}


void midimsg_newPitchWheel(midiMsg_t *msg, uint8_t chan, int16_t pitch_wheel)
{
    msg->DataLen = 3;
    msg->Flags = MIDIMSG_FLAG_PARSE_OK;
    msg->Data[MIDIMSG_STATUS] = MIDI_STATUS_PITCH_WHEEL | chan;

    // Offset pwheel
    pitch_wheel += 8192;

    msg->Data[MIDIMSG_DATA1] = pitch_wheel & 0x7F; // LS 7 bit
    msg->Data[MIDIMSG_DATA2] = pitch_wheel >> 7u;  // MS 7 bit
}


void midimsg_newContinuousCtrl(midiMsg_t *msg, uint8_t chan, uint8_t cc, uint8_t value)
{
    msg->DataLen = 3;
    msg->Flags = MIDIMSG_FLAG_PARSE_OK;
    msg->Data[0] = MIDI_STATUS_CTRL_CHANGE | chan;
    msg->Data[1] = cc;
    msg->Data[2] = value;
}


void midimsg_newKeyAt(midiMsg_t *msg, uint8_t chan, uint8_t key, uint8_t value)
{
    msg->DataLen = 3;
    msg->Flags = MIDIMSG_FLAG_PARSE_OK;
    msg->Data[0] = MIDI_STATUS_KEY_ATOUCH | chan;
    msg->Data[1] = key;
    msg->Data[2] = value;
}

void midimsg_newSetStatus(midiMsg_t *msg, uint8_t status)
{
    msg->Flags = MIDIMSG_FLAG_PARSE_OK;
    msg->DataLen = 1;
    msg->Data[MIDIMSG_STATUS] = status;
}

void midimsg_addByte(midiMsg_t *msg, uint8_t value)
{
    uint8_t l;

    l = msg->DataLen;

    if (l < MIDIMSG_DATA_MAX)
    {
        msg->Data[l] = value;
        msg->DataLen = l+1;
    }
    else
    {
        // Overflow. TODO Handle this?
    }
}

