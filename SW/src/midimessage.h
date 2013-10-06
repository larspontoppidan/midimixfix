/*
 * mididefines.h
 *
 *  Created on: Dec 13, 2012
 *      Author: lars
 */

#ifndef MIDIMESSAGE_H_
#define MIDIMESSAGE_H_

#include "common.h"

// Midi message storage format definition

#define MIDIMSG_DATA_MAX 5

// Some routes have predefined meanings:
#define MIDIMSG_ROUTE_INACTIVE   0   // Used for non existing or deleted messages
#define MIDIMSG_ROUTE_ALL        255 // The wildcard of routes

// The flags give information on how parsing went:
#define MIDIMSG_FLAG_PARSE_OK    1   // OK message
#define MIDIMSG_FLAG_RUNN_STATUS 2   // Indicates running status was used (which is ok)
#define MIDIMSG_FLAG_NO_STATUS   4   // No status to use
#define MIDIMSG_FLAG_TOO_SHORT   8   // All required bytes didn't arrive

// Data locations in a well formed MIDI message:
// (May not be true for SYSEX or invalid messages)
#define MIDIMSG_STATUS 0
#define MIDIMSG_DATA1  1
#define MIDIMSG_DATA2  2



typedef struct
{
    uint16_t ReceivedTick;
    uint8_t  Route;
    uint8_t  Flags;
    uint8_t  DataLen; // 0 to data_max
    uint8_t  Data[MIDIMSG_DATA_MAX];
} midiMsg_t;


// Converting message to text
char *midimsg_writeRaw(char *dest, midiMsg_t *msg);
char *midimsg_writeParsed(char *dest, midiMsg_t *msg);


// TODO consider this nicely encapsulated interface to the midi message:
//
//// Accessing features of a message
//uint8_t midimsg_getChan(midiMsg_t *msg);
//uint8_t midimsg_getKey(midiMsg_t *msg);
//uint8_t midimsg_getVelocity(midiMsg_t *msg);
//
//// Modifying midi messages
//void midimsg_setChan(midiMsg_t *msg, uint8_t chan);
//void midimsg_setKey(midiMsg_t *msg, uint8_t key);
//void midimsg_setVelocity(midiMsg_t *msg, uint8_t velocity);
//
//// Constructing midi messages
//void midimsg_newNoteOn(midiMsg_t *msg, uint8_t chan, uint8_t key, uint8_t velocity);
//void midimsg_newNoteOff(midiMsg_t *msg, uint8_t chan, uint8_t key, uint8_t velocity);
//void midimsg_newKeyAfterTouch(midiMsg_t *msg, uint8_t chan, uint8_t key, uint8_t pressure);
//void midimsg_newChanAfterTouch(midiMsg_t *msg, uint8_t chan, uint8_t pressure);
//void midimsg_newPitchWheel(midiMsg_t *msg, uint8_t chan, int16_t pitch_wheel);
//void midimsg_newProgramChange(midiMsg_t *msg, uint8_t chan, uint8_t part);
//void midimsg_newContinuousCtrl(midiMsg_t *msg, uint8_t chan, uint8_t cc, uint8_t value);
//

//// Constructing midi messages byte by byte
void midimsg_newSetStatus(midiMsg_t * msg, uint8_t status);
void midimsg_addByte(midiMsg_t * msg, uint8_t value);


#endif /* MIDIMESSAGE_H_ */
