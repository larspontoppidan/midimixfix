/*
 * midimessage.c
 *
 *  Created on: Dec 13, 2012
 *      Author: lars
 */


#include "common.h"
#include "midimessage.h"
#include "errors.h"
#include "util.h"
#include <avr/pgmspace.h>

// Strings for decoding midi messages

char MidiCtrlNames1[32][10] PROGMEM =
{
        {"Bank.Sel"},  // 0
        {"Modulat."},  // 1
        {"Breath"},    // 2
        {"FootCtl"},   // 4
        {"Port.Time"}, // 5
        {"DataEntry"}, // 6
        {"Volume"},    // 7
        {"Balance"},   // 8
        {"Undef.9"},   // 9
        {"Pan"},       // 10
        {"Expr."},     // 11
        {"Eff.Ctl.1"}, // 12
        {"Eff.Ctl.2"}, // 13
        {"Undef.14"},  // 14
        {"Undef.15"},  // 15
        {"GP.Ctl.1"},  // 16
        {"GP.Ctl.2"},  // 17
        {"GP.Ctl.3"},  // 18
        {"GP.Ctl.4"},  // 19
        {"Undef.20"},  // 20
        {"Undef.21"},  // 21
        {"Undef.22"},  // 22
        {"Undef.23"},  // 23
        {"Undef.24"},  // 24
        {"Undef.25"},  // 25
        {"Undef.26"},  // 26
        {"Undef.27"},  // 27
        {"Undef.28"},  // 28
        {"Undef.29"},  // 29
        {"Undef.30"},  // 30
        {"Undef.31"}   // 31
};


char MidiCtrlNames2[64][10] PROGMEM =
{
        {"SustainP"},  // 64
        {"Portam.P"},  // 65
        {"Sosten.P"},  // 66
        {"SoftP"},     // 67
        {"LegatoP"},   // 68
        {"Hold2"},     // 69
        {"Variation"}, // 70
        {"Intensity"}, // 71
        {"Rel.Time"},  // 72
        {"Att.Time"},  // 73
        {"Brightns"},  // 74
        {"DecayTime"}, // 75
        {"VibRate"},   // 76
        {"VibDepth"},  // 77
        {"VibDelay"},  // 78
        {"Undef.79"},  // 79
        {"GP.Ctl.5"},  // 80
        {"GP.Ctl.6"},  // 81
        {"GP.Ctl.7"},  // 82
        {"GP.Ctl.8"},  // 83
        {"PortaCtl"},  // 84
        {"Undef.85"},  // 85
        {"Undef.86"},  // 86
        {"Undef.87"},  // 87
        {"HresVel"},   // 88
        {"Undef.89"},  // 89
        {"Undef.90"},  // 90
        {"Reverb"},    // 91
        {"Tremolo"},   // 92
        {"Chorus"},    // 93
        {"DetuDepth"}, // 94
        {"PhasDepth"}, // 95
        {"DataInc."},  // 96
        {"DataDec."},  // 97
        {"NrpnLSB"},   // 98
        {"NrpnMSB"},   // 99
        {"RpnLSB"},    // 100
        {"RpnMSB"},    // 101
        {"Undef.102"}, // 102
        {"Undef.103"}, // 103
        {"Undef.104"}, // 104
        {"Undef.105"}, // 105
        {"Undef.106"}, // 106
        {"Undef.107"}, // 107
        {"Undef.108"}, // 108
        {"Undef.109"}, // 109
        {"Undef.110"}, // 110
        {"Undef.111"}, // 111
        {"Undef.112"}, // 112
        {"Undef.113"}, // 113
        {"Undef.114"}, // 114
        {"Undef.115"}, // 115
        {"Undef.116"}, // 116
        {"Undef.117"}, // 117
        {"Undef.118"}, // 118
        {"Undef.119"}, // 119
        {"SoundOff"},  // 120
        {"ResetCtrl"}, // 121
        {"LocalCtrl"}, // 122
        {"NotesOff"},  // 123
        {"OmniOff"},   // 124
        {"OmniOn"},    // 125
        {"MonoOn"},    // 126
        {"PolyOn"}     // 127
};

char MidiStatusNames[8][9] PROGMEM =
{
        {"N.Off "},    // 0x80
        {"N.On  "},     // 0x90
        {"ATouch "},   // 0xA0
        {""},          // 0xB0   // Control change, don't write name of this status
        {"ProgChg "},  // 0xC0
        {"ATouch "},   // 0xD0
        {"PitchW "},   // 0xE0
        {"System "}    // 0xF0
};

char MidiSysStatusNames[16][13] PROGMEM =
{
        {"SysexStart"},   //  0xF0
        {"Sys.TimeCode"}, //  0xF1
        {"Sys.SongPos"},  //  0xF2
        {"Sys.SongSel"},  //  0xF3
        {"Sys.Undef.4"},  //  0xF4
        {"Sys.Undef.5"},  //  0xF5
        {"Sys.TuneReq"},  //  0xF6
        {"SysexEnd"},     //  0xF7
        {"Realt.Clock"},  //  0xF8
        {"Realt.Undef."}, //  0xF9
        {"Realt.Start"},  //  0xFA
        {"Realt.Cont."},  //  0xFB
        {"Realt.Stop"},   //  0xFC
        {"Realt.Undef."}, //  0xFD
        {"Realt.ASense"}, //  0xFE
        {"Realt.Reset"}   //  0xFF
};

const char NoteNames[12][3] PROGMEM =
{
        {"C"},   // Note number 0 = "C -5"
        {"C#"},
        {"D"},
        {"D#"},
        {"E"},
        {"F"},
        {"F#"},
        {"G"},
        {"G#"},
        {"A"},
        {"A#"},
        {"B"}
};


uint8_t mmsg_GetDataCount(uint8_t midi_status)
{
    uint8_t count = 0;

    if (midi_status >= 0xF0)
    {
        if ((midi_status == 0xF1) || (midi_status == 0xF3))
        {
            count = 1;
        }
        else if (midi_status == 0xF2)
        {
            count = 2;
        }
    }
    else if (midi_status >= 0x80)
    {
        midi_status &= 0xF0;

        if ((midi_status == 0xC0) || (midi_status == 0xD0))
        {
            count = 1;
        }
        else
        {
            count = 2;
        }
    }

    return count;
}


uint8_t mmsg_GetDataType(uint8_t x)
{
    uint8_t type;

    if ((x & MIDI_STATUS_BIT) == 0u)
    {
        // Its data
        type = MIDI_TYPE_DATA;
    }
    else if ((x & 0xF0) == 0xF0)
    {
        // Its system something

        if (x >= 0xF8)
        {
            type = MIDI_TYPE_SYS_REALTIME;
        }
        else if ((x == 0xF0) || (x == 0xF7))
        {
            type = MIDI_TYPE_SYS_EXCL;
        }
        else
        {
            type = MIDI_TYPE_SYS_COMMON;
        }
    }
    else if ((x & 0xF0) == 0xB0)
    {
        type = MIDI_TYPE_CHAN_MODE;
    }
    else
    {
        type = MIDI_TYPE_CHAN_VOICE;
    }

    return type;
}



uint8_t mmsg_GetMsgLength(mmsg_t *msg)
{
    uint8_t ret;

    ret = msg->midi_data_len;

    if ((msg->flags & MMSG_FLAG_RAW) == 0u)
    {
        ret++;
    }

    return ret;
}


uint8_t mmsg_GetMsgByte(mmsg_t *msg, uint8_t index)
{
    uint8_t ret;

    if ((msg->flags & MMSG_FLAG_RAW) == 0u)
    {
        // Not a raw type message, index 0 is status:

        if (index == 0u)
        {
            return msg->midi_status;
        }
        else
        {
            index--;
        }
    }

    if (index < msg->midi_data_len)
    {
        ret = msg->midi_data[index];
    }
    else
    {
        ret = 0u;
        err_Register(ERR_INTERNAL4);
    }

    return ret;
}


char *mmsg_WriteControllerName(char *dest, uint8_t ctrl)
{
    if (ctrl < 32)
    {
        // First range of controllers
        dest = util_StrCpy_P(dest, MidiCtrlNames1[ctrl]);
    }
    else if (ctrl < 64)
    {
        // LSB of first range
        dest = util_StrCpy_P(dest, MidiCtrlNames1[ctrl - 32]);
        dest = util_StrCpy_P(dest, PSTR("LSB"));
    }
    else
    {
        // Second range
        dest = util_StrCpy_P(dest, MidiCtrlNames2[ctrl - 64]);
    }

    return dest;
}

// Number of chars written is typically 7, maximum 10.
//
// Examples:
// "SS."        Only status
// "SS!AA"      One data byte (running status)
// "SS.AABB"    Two data bytes
// "AABBCCDDEE" Raw message with 5 bytes

char *mmsg_WriteMsgRaw(char *dest, mmsg_t *msg)
{
    if (msg->flags == 0u)
    {
        // Nothing here
        *(dest++) = '-';
    }
    else
    {
        uint8_t i;

        // Do we have a status?
        if (msg->flags & MMSG_FLAG_MSG_OK)
        {
            // Write it
            dest = util_StrWriteHex(dest, msg->midi_status);

            // Did we use running status?
            if (msg->flags & MMSG_FLAG_RUNNING_STATUS)
            {
                *(dest++) = '!';
            }
            else
            {
                *(dest++) = '.';
            }
        }

        // Write all the data
        for (i = 0u; i < msg->midi_data_len; i++)
        {
            dest = util_StrWriteHex(dest, msg->midi_data[i]);
        }
    }

    return dest;
}

char *mmsg_WriteMsgParsed(char *dest, mmsg_t *msg)
{
    if (msg->flags == 0u)
    {
        // Nothing here
        *(dest++) = '-';
    }
    else if (msg->flags & MMSG_FLAG_RAW)
    {
        dest = util_StrCpy_P(dest, PSTR("Raw data"));
    }
    else if ((msg->flags & MMSG_FLAG_MSG_OK) == 0u)
    {
        dest = util_StrCpy_P(dest, PSTR("Bad length"));
    }
    else
    {
        // We have well formed message, decode it
        uint8_t status = msg->midi_status;

        if (status >= 0xF0)
        {
            // Its a system status
            dest = util_StrCpy_P(dest, MidiSysStatusNames[status - 0xF0]);

            if ((status == 0xF1) || (status == 0xF2) || (status == 0xF3))
            {
                // We have one parameter (or more)
                *(dest++) = ' ';
                dest = util_StrWriteInt8LA(dest, msg->midi_data[0]);

                if (status == 0xF2)
                {
                    // We have two parameters
                    *(dest++) = ' ';
                    dest = util_StrWriteInt8LA(dest, msg->midi_data[1]);
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
            dest = util_StrWriteInt8LA(dest, (status & 0x0F)+1);
            *(dest++) = ' ';
            dest = util_StrCpy_P(dest, MidiStatusNames[(status >> 4u) - 8]);

            switch (status & 0xF0)
            {
            case MIDI_STATUS_NOTE_ON:
            case MIDI_STATUS_NOTE_OFF:
            case MIDI_STATUS_KEY_ATOUCH:
                // Decode note number
                dest = util_StrCpy_P(dest, NoteNames[(msg->midi_data[0] % 12)]);
                dest = util_StrWriteInt8LA(dest,
                        (int8_t)(msg->midi_data[0] / 12) - (int8_t)5);
                *(dest++) = ' ';

                // Add velocity / pressure
                dest = util_StrWriteInt8LA(dest, msg->midi_data[1]);
                break;

            case MIDI_STATUS_CTRL_CHANGE:
                // Write controller name=value
                dest = mmsg_WriteControllerName(dest, msg->midi_data[0]);
                *(dest++) = '=';
                dest = util_StrWriteInt8LA(dest, msg->midi_data[1]);
                break;

            case MIDI_STATUS_PROG_CHANGE:
                // Write new prog name
                dest = util_StrWriteInt8LA(dest, msg->midi_data[0]);
                break;

            case MIDI_STATUS_CHAN_ATOUCH:
                // Write value
                dest = util_StrWriteInt8LA(dest, msg->midi_data[0]);
                break;

            case MIDI_STATUS_PITCH_WHEEL:
                // Write value
                sw  = msg->midi_data[0];       // LS 7 bits
                sw |= msg->midi_data[1] << 7u; // MS 7 bits
                sw -= 8192;

                dest = util_StrWriteInt16(dest, sw);
                break;
            }
        }
    }

    return dest;
}

