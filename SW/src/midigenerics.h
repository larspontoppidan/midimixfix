/*
 * midigenerics.h
 *
 *  Created on: May 9, 2013
 *      Author: lars
 */

#ifndef MIDIGENERICS_H_
#define MIDIGENERICS_H_


// Translating aspects of a message:
uint8_t Midi_getDataType(uint8_t x);
char *Midi_writeNoteName(char *dest, uint8_t note_number);
char *Midi_writeControllerName(char *dest, uint8_t ctrl);
char *Midi_writeStatusName(char *dest, uint8_t status);

uint8_t Midi_getDataCount(uint8_t midi_status);

// Is a certain key white or black on the keyboard?
bool_t Midi_isKeyBlack(uint8_t key);

////////////////////// Midi Message definitions ///////////////////////////


#define MIDI_TYPE_DATA          0    //  High nibble: 0
#define MIDI_TYPE_CHAN_VOICE    1    //  High nibble: 80,90,A0,C0,D0,E0
#define MIDI_TYPE_CHAN_MODE     2    //  High nibble: B0
#define MIDI_TYPE_SYS_COMMON    3    //  F1, F2, F3, F4, F5, F6
#define MIDI_TYPE_SYS_REALTIME  4    //  F8, F9, FA, FB, FC, FD, FE, FF
#define MIDI_TYPE_SYS_EXCL      5    //  F0, F7


// STATUS
//////////

// Bytes with MS-bit high in midi messages are status bytes.

#define MIDI_STATUS_BIT           0x80
#define MIDI_STATUS_MASK          0xF0
#define MIDI_CHANNEL_MASK         0x0F

// Channel Voice Message
//
#define MIDI_STATUS_NOTE_OFF      0x80 // 7-bit key, 7-bit velocity
#define MIDI_STATUS_NOTE_ON       0x90 // 7-bit key, 7-bit velocity
#define MIDI_STATUS_KEY_ATOUCH    0xA0 // 7-bit key, 7-bit pressure
#define MIDI_STATUS_CTRL_CHANGE   0xB0 // 7-bit controller, 7-bit value
#define MIDI_STATUS_PROG_CHANGE   0xC0 // 7-bit patch
#define MIDI_STATUS_CHAN_ATOUCH   0xD0 // 7-bit pressure
#define MIDI_STATUS_PITCH_WHEEL   0xE0 // 7-bit LSB, 7-bit MSB of 14-bit pitch value
#define MIDI_STATUS_SYSTEM        0xF0 // (...) Sysex or system common message

// Channel mode controller messages
//
#define MIDI_CTRL_SOUND_OFF       120   // 7-bit value = 0:   All oscillators must turn off
#define MIDI_CTRL_RESET_CTRL      121   // 7-bit value = 0:   All controllers to default value
#define MIDI_CTRL_LOCAL_CTRL      122   // 7-bit value = 0:   Local control off
                                        // 7-bit value = 127: Local control on
#define MIDI_CTRL_NOTES_OFF       123   // 7-bit value = 0:   All notes must go off
#define MIDI_CTRL_OMNI_OFF        124
#define MIDI_CTRL_OMNI_ON         125
#define MIDI_CTRL_MONO_ON         126
#define MIDI_CTRL_POLY_MODE_ON    127

// Control change messages
//
#define MIDI_CTRL_BANK_SEL        0
#define MIDI_CTRL_MODULATION      1
#define MIDI_CTRL_BREATH          2
#define MIDI_CTRL_FOOT            4
#define MIDI_CTRL_PORTAMENTO_TIME 5
#define MIDI_CTRL_DATA_ENTRY_MSB  6
#define MIDI_CTRL_CHAN_VOLUME     7
#define MIDI_CTRL_BALANCE         8
#define MIDI_CTRL_PAN             10
#define MIDI_CTRL_EXPRESSION      11
#define MIDI_CTRL_EFFECT_1        12
#define MIDI_CTRL_EFFECT_2        13
#define MIDI_CTRL_GEN_PURPOSE_1   16
#define MIDI_CTRL_GEN_PURPOSE_2   17
#define MIDI_CTRL_GEN_PURPOSE_3   18
#define MIDI_CTRL_GEN_PURPOSE_4   19

#define MIDI_CTRL_LSB_OFFSET      32   // Add to access LSB of controllers 0 to 31

#define MIDI_CTRL_DAMPER_PEDAL    64   // (Sustain)
#define MIDI_CTRL_PORTAM_PEDAL    65
#define MIDI_CTRL_SUSTENUTO       66
#define MIDI_CTRL_SOFT_PEDAL      67
#define MIDI_CTRL_LEGATO_PEDAL    68
#define MIDI_CTRL_HOLD_2          69

#define MIDI_CTRL_SND_VARIATION   70
#define MIDI_CTRL_SND_INTENSITY   71
#define MIDI_CTRL_SND_REL_TIME    72
#define MIDI_CTRL_SND_ATT_TIME    73
#define MIDI_CTRL_SND_BRIGHTNESS  74
#define MIDI_CTRL_SND_DECAY_TIME  75
#define MIDI_CTRL_SND_VIB_RATE    76
#define MIDI_CTRL_SND_VIB_DEPTH   77
#define MIDI_CTRL_SND_VIB_DELAY   78
#define MIDI_CTRL_SND_UNDEF       79

#define MIDI_CTRL_GEN_PURPOSE_5   80
#define MIDI_CTRL_GEN_PURPOSE_6   81
#define MIDI_CTRL_GEN_PURPOSE_7   82
#define MIDI_CTRL_GEN_PURPOSE_8   83
#define MIDI_CTRL_PORTAMENTO      84

#define MIDI_CTRL_HIGH_RES_VEL    88


// System exclusive / common messages (by default data count = 0)
#define MIDI_SYSTEM_EX_START     0xF0 // variable data length
#define MIDI_SYSTEM_TIME_CODE    0xF1 // data count = 1
#define MIDI_SYSTEM_SONG_POS     0xF2 // data count = 2
#define MIDI_SYSTEM_SONG_SEL     0xF3 // data count = 1
#define MIDI_SYSTEM_TUNE_REQ     0xF6
#define MIDI_SYSTEM_EX_END       0xF7

// System real-time messages
#define MIDI_SYSTEM_TIMING_CLK   0xF8
#define MIDI_SYSTEM_START        0xFA
#define MIDI_SYSTEM_CONTINUE     0xFB
#define MIDI_SYSTEM_STOP         0xFC
#define MIDI_SYSTEM_ACT_SENSE    0xFE
#define MIDI_SYSTEM_RESET        0xFF


#endif /* MIDIGENERICS_H_ */
