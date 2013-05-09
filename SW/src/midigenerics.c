/*
 * midigenerics.c
 *
 *  Created on: May 9, 2013
 *      Author: lars
 */



#include "common.h"
#include "midigenerics.h"
#include "util.h"
#include <avr/pgmspace.h>

// Strings for decoding midi messages

static char MidiCtrlNames1[32][10] PROGMEM =
{
        {"Bank.Sel"},  // 0
        {"Modulat."},  // 1
        {"Breath"},    // 2
        {"Undef.3"},   // 3
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


static char MidiCtrlNames2[64][10] PROGMEM =
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

static char MidiStatusNames[8][9] PROGMEM =
{
        {"N.Off "},    // 0x80
        {"N.On  "},     // 0x90
        {"KeyAT "},   // 0xA0
        {""},          // 0xB0   // Control change, don't write name of this status
        {"ProgChg "},  // 0xC0
        {"ChanAT "},   // 0xD0
        {"PitchW "},   // 0xE0
        {"System "}    // 0xF0
};

static char MidiSysStatusNames[16][13] PROGMEM =
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

static const char NoteNames[12][3] PROGMEM =
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


uint8_t Midi_getDataCount(uint8_t midi_status)
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


uint8_t Midi_getDataType(uint8_t x)
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




char *Midi_writeControllerName(char *dest, uint8_t ctrl)
{
    if (ctrl < 32)
    {
        // First range of controllers
        dest = Util_copyString_P(dest, MidiCtrlNames1[ctrl]);
    }
    else if (ctrl < 64)
    {
        // LSB of first range
        dest = Util_copyString_P(dest, MidiCtrlNames1[ctrl - 32]);
        dest = Util_copyString_P(dest, PSTR("LSB"));
    }
    else
    {
        // Second range
        dest = Util_copyString_P(dest, MidiCtrlNames2[ctrl - 64]);
    }

    return dest;
}



char *Midi_writeNoteName(char *dest, uint8_t note_number)
{
    dest = Util_copyString_P(dest, NoteNames[(note_number % 12)]);
    dest = Util_writeInt8LA(dest, (int8_t)(note_number / 12) - (int8_t)5);
    return dest;
}

char *Midi_writeStatusName(char *dest, uint8_t status)
{
    if (status >= 0xF0)
    {
        // Its a system status
        dest = Util_copyString_P(dest, MidiSysStatusNames[status - 0xF0]);
    }
    else
    {
        dest = Util_copyString_P(dest, MidiStatusNames[(status >> 4u) - 8]);
    }

    return dest;
}


bool_t Midi_isKeyBlack(uint8_t key)
{
    bool_t ret = FALSE;

    // Black keys are notes C# D# F# G# A#

    switch (key % 12)
    {
    case 1:  // C#
    case 3:  // D#
    case 6:  // F#
    case 8:  // G#
    case 10: // A#
        ret = TRUE;
        break;
    }

    return ret;
}
