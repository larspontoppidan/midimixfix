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

#define MIDIMSG_SOURCE_INPUT1        0x01
#define MIDIMSG_SOURCE_INPUT2        0x02
#define MIDIMSG_SOURCE_GENERATED     0x04
#define MIDIMSG_FLAG_RAW             0x08   // If the raw flag is set, it means data without status!
#define MIDIMSG_FLAG_MSG_OK          0x10   // If set, the message had expected length
#define MIDIMSG_FLAG_RUNNING_STATUS  0x20   // If set, running status was used by transmitter
#define MIDIMSG_FLAG_MODIFIED        0x40   // If set, filters have modified this message
#define MIDIMSG_FLAG_DISCARD         0x80   // If set, filters have decided to discard the message

typedef struct
{
    uint16_t ReceivedTick;
    uint8_t Flags;
    uint8_t MidiStatus;
    uint8_t DataLen; // 0 to data_max
    uint8_t Data[MIDIMSG_DATA_MAX];
} midiMsg_t;


// Accessing the raw bytes of a message:
uint8_t midimsg_getLength(midiMsg_t *msg);
uint8_t midimsg_getByte(midiMsg_t *msg, uint8_t index);

// Converting message to text
char *midimsg_writeRaw(char *dest, midiMsg_t *msg);
char *midimsg_writeParsed(char *dest, midiMsg_t *msg);



#endif /* MIDIMESSAGE_H_ */
