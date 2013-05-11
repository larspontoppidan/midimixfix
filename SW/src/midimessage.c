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


uint8_t midimsg_getLength(midiMsg_t *msg)
{
    uint8_t ret;

    ret = msg->DataLen;

    if ((msg->Flags & MIDIMSG_FLAG_RAW) == 0u)
    {
        ret++;
    }

    return ret;
}


uint8_t midimsg_getByte(midiMsg_t *msg, uint8_t index)
{
    uint8_t ret;

    if ((msg->Flags & MIDIMSG_FLAG_RAW) == 0u)
    {
        // Not a raw type message, index 0 is status:

        if (index == 0u)
        {
            return msg->MidiStatus;
        }
        else
        {
            index--;
        }
    }

    if (index < msg->DataLen)
    {
        ret = msg->Data[index];
    }
    else
    {
        ret = 0u;
        err_raise(ERR_MODULE_MMSG, __LINE__);;
    }

    return ret;
}

// Number of chars written is typically 7, maximum 10.
//
// Examples:
// "SS."        Only status
// "SS!AA"      One data byte (running status)
// "SS.AABB"    Two data bytes
// "AABBCCDDEE" Raw message with 5 bytes

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

        // Do we have a status?
        if (msg->Flags & MIDIMSG_FLAG_MSG_OK)
        {
            // Write it
            dest = util_writeHex(dest, msg->MidiStatus);

            // Did we use running status?
            if (msg->Flags & MIDIMSG_FLAG_RUNNING_STATUS)
            {
                *(dest++) = '!';
            }
            else
            {
                *(dest++) = '.';
            }
        }

        // Write all the data
        for (i = 0u; i < msg->DataLen; i++)
        {
            dest = util_writeHex(dest, msg->Data[i]);
        }
    }

    return dest;
}


char *midimsg_writeParsed(char *dest, midiMsg_t *msg)
{
    if (msg->Flags == 0u)
    {
        // Nothing here
        *(dest++) = '-';
    }
    else if (msg->Flags & MIDIMSG_FLAG_RAW)
    {
        dest = util_copyString_P(dest, PSTR("Raw data"));
    }
    else if ((msg->Flags & MIDIMSG_FLAG_MSG_OK) == 0u)
    {
        dest = util_copyString_P(dest, PSTR("Bad length"));
    }
    else
    {
        // We have well formed message, decode it
        uint8_t status = msg->MidiStatus;

        if (status >= 0xF0)
        {
            // Its a system status<
            dest = midi_writeStatusName(dest, status);

            if ((status == 0xF1) || (status == 0xF2) || (status == 0xF3))
            {
                // We have one parameter (or more)
                *(dest++) = ' ';
                dest = util_writeInt8LA(dest, msg->Data[0]);

                if (status == 0xF2)
                {
                    // We have two parameters
                    *(dest++) = ' ';
                    dest = util_writeInt8LA(dest, msg->Data[1]);
                }
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
                dest = midi_writeNoteName(dest, msg->Data[0]);
                *(dest++) = ' ';

                // Add velocity / pressure
                dest = util_writeInt8LA(dest, msg->Data[1]);
                break;

            case MIDI_STATUS_CTRL_CHANGE:
                // Write controller name=value
                dest = midi_writeControllerName(dest, msg->Data[0]);
                *(dest++) = '=';
                dest = util_writeInt8LA(dest, msg->Data[1]);
                break;

            case MIDI_STATUS_PROG_CHANGE:
                // Write new prog name
                dest = util_writeInt16LA(dest, msg->Data[0] + 1);
                break;

            case MIDI_STATUS_CHAN_ATOUCH:
                // Write value
                dest = util_writeInt8LA(dest, msg->Data[0]);
                break;

            case MIDI_STATUS_PITCH_WHEEL:
                // Write value
                sw  = msg->Data[0];       // LS 7 bits
                sw |= msg->Data[1] << 7u; // MS 7 bits
                sw -= 8192;

                dest = util_writeInt16(dest, sw);
                break;
            }
        }
    }

    return dest;
}
