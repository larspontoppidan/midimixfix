/*
 * polysplit.c
 *
 *  Created on: Jul 14, 2013
 *      Author: lars
 */

//
// Splitting polyphonic properties into multiple channels
//
// This feature performs the following conversion:
//
// ChannelSplit
//   Source
//   Channel
//   Use ch.: 2 thru 7
//   Map KeyAT: Expr.
//   Filter: No
//   Filter: Yes
//   Filter: Yes+KeyOn
//


//
//
//








/////////////////////////    INCLUDES    /////////////////////////

#include "common.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "hal.h"
#include "midiio.h"
#include "util.h"
#include "menu.h"
#include "pgmstrings.h"
#include "errors.h"
#include <string.h>

/////////////////////////    Defines     /////////////////////////

// Configuration

typedef struct
{
    bool_t FilterEnabled;
    uint8_t Source;
    uint8_t SourceChannel;
    uint8_t OutChannels;     // E.g., if 5, the channels 1,2,3,4,5 are used
    uint8_t FirstChannel;
    uint8_t PolyAtCc;        // What poly aftertouch controls
    uint8_t AtRateLimit;     // 0 means off. 1 or more is active
} config_t;


// State

typedef struct
{
    uint8_t Channel;
    uint8_t NoteOffTime;      // 255 is key on, 254 is key off and counting downwards
    uint8_t LatestKey;        // The key played.
    uint8_t LatestCc;         // The latest sent value of our CC
    uint8_t FilterState;      // What value
    uint8_t FilterTarget;
} channel_state_t;

// When note state is 255, the key has just been pressed
// At key off, the state is initially 254 and then counts down to zero
#define NOTE_STATE_ON   255


#define CHANNEL_MAX 9

#define NOT_FOUND 0xFF


#define TICK_DIVISION 10  // 100 Hz ISR divided by this will be the rate of the rate limit filter transmissions

/////////////////////////   Variables    /////////////////////////

static config_t Config;
static channel_state_t ChannelState[CHANNEL_MAX];
static uint8_t TickDivider = 0;

/////////////////////////   Prototypes   /////////////////////////

// Channel management:

static void initChannels(void);
static uint8_t findActiveChannel(uint8_t key);
static uint8_t findFreeChannel(uint8_t key);

static void filterReset(uint8_t chan, uint8_t value);
static void filterSetTarget(uint8_t chan, uint8_t value);
static void filterUpdate(uint8_t chan);


static void changeMsgChannel(midiMsg_t *msg, uint8_t chan);
static void discardMsg(midiMsg_t *msg);
static void sendAtMsg(uint8_t chan, uint8_t value);


///////////////////////// Implementation /////////////////////////

static void initChannels(void)
{
    uint8_t i;

    for (i = 0; i < Config.OutChannels; i++)
    {
        memset(&(ChannelState[i]), 0, sizeof(channel_state_t));

        ChannelState[i].Channel = i + Config.FirstChannel;
    }
}

// Find channel that currently has this key pressed
static uint8_t findActiveChannel(uint8_t key)
{
    uint8_t i;
    uint8_t ret = NOT_FOUND;

    for (i = 0; i < Config.OutChannels; i++)
    {
        if ((ChannelState[i].NoteOffTime == NOTE_STATE_ON) &&
                (ChannelState[i].LatestKey == key))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

// Find a free channel that either played the key last time
// or secondarily, the channel not used for the longest time
static uint8_t findFreeChannel(uint8_t key)
{
    uint8_t i;
    uint8_t ret = NOT_FOUND;
    uint8_t best_free = NOTE_STATE_ON;

    for (i = 0; i < Config.OutChannels; i++)
    {
        if ((ChannelState[i].NoteOffTime != NOTE_STATE_ON) &&
                (ChannelState[i].LatestKey == key))
        {
            // We got a hit right here
            ret = i;
            break;
        }

        if (ChannelState[i].NoteOffTime < best_free)
        {
            best_free = ChannelState[i].NoteOffTime;
            ret = i;
        }
    }

    return ret;
}

static void filterReset(uint8_t chan, uint8_t value)
{
    ChannelState[chan].FilterTarget = value;
    ChannelState[chan].FilterState = value;
}

static void filterSetTarget(uint8_t chan, uint8_t value)
{
    ChannelState[chan].FilterTarget = value;
}

static void filterUpdate(uint8_t chan)
{
    if (ChannelState[chan].FilterState != ChannelState[chan].FilterTarget)
    {
        // Work in a wider integer space to avoid wrap-around problems
        // in the following calculations

        int16_t s = ChannelState[chan].FilterState;
        int16_t t = ChannelState[chan].FilterTarget;

        if (t > s)
        {
            // We have to increase value
            s += (int16_t)(Config.AtRateLimit);

            if (s > t)
            {
                // We got too far, lets arrive at target then:
                s = t;
            }
        }
        else
        {
            // We have to decrease value
            s -= (int16_t)(Config.AtRateLimit);

            if (s < t)
            {
                // We got too far, lets arrive at target then:
                s = t;
            }
        }

        // Update state
        ChannelState[chan].FilterState = (uint8_t)s;
    }

}

static void changeMsgChannel(midiMsg_t *msg, uint8_t chan)
{
    uint8_t ms = msg->MidiStatus;

    // AND away current channel info:
    ms &= MIDI_STATUS_MASK;

    // Add desired channel info
    ms |= (chan & MIDI_CHANNEL_MASK);

    msg->MidiStatus = ms;
}

static void discardMsg(midiMsg_t *msg)
{
    msg->Flags |= MIDIMSG_FLAG_DISCARD;
}

static void sendAtMsg(uint8_t chan, uint8_t value)
{
    uint8_t i;

    // Generate message with correct channel and Midi Control Change
    i = midiio_msgNew_ISR(MIDIMSG_SOURCE_GENERATED | MIDIMSG_FLAG_MSG_OK,
            MIDI_STATUS_CTRL_CHANGE | ChannelState[chan].Channel);

    // Add controller type
    midiio_msgAddData_ISR(i, Config.PolyAtCc);

    // Add value
    midiio_msgAddData_ISR(i, value);

    // Thats it
    midiio_msgFinish_ISR(i, 0);

    // Register we have sent this value
    ChannelState[chan].LatestCc = value;
}


void psplit_initialize(void)
{
    // Set defaults
    Config.FirstChannel = 1;
    Config.OutChannels = 7;
    Config.FilterEnabled = TRUE;
    Config.PolyAtCc = MIDI_CTRL_EXPRESSION;
    Config.Source = MIDIMSG_SOURCE_INPUT1 | MIDIMSG_SOURCE_INPUT2;
    Config.SourceChannel = 0;
    Config.AtRateLimit = 40;

    // Init state of channels
    initChannels();
}

void handleNoteOne(midiMsg_t *msg)
{
    // Find a channel for this
    uint8_t c = findFreeChannel(msg->Data[0]);

    // Do we need to adjust anything based on key on?
    if (Config.AtRateLimit != 0)
    {
        // Use velocity as reset value for the filter
        filterReset(c, msg->Data[1]);

        // Already send a controller change
        sendAtMsg(c, msg->Data[1]);
    }

    changeMsgChannel(msg, ChannelState[c].Channel);

    ChannelState[c].LatestKey = msg->Data[0];
    ChannelState[c].NoteOffTime = NOTE_STATE_ON;
}

void handleNoteOff(midiMsg_t *msg)
{
    // Find correct channel for this
    uint8_t c = findActiveChannel(msg->Data[0]);

    if (c != NOT_FOUND)
    {
        changeMsgChannel(msg, ChannelState[c].Channel);

        ChannelState[c].NoteOffTime = NOTE_STATE_ON - 1;

        // Decrement all off channel's NoteOffvalue
        uint8_t i;

        for (i = 0; i < Config.OutChannels; i++)
        {
            if ((ChannelState[i].NoteOffTime != NOTE_STATE_ON) &&
                (ChannelState[i].NoteOffTime != 0))
            {
                ChannelState[i].NoteOffTime--;
            }
        }

    }
    else
    {
        discardMsg(msg);
    }
}

void handleKeyAT(midiMsg_t *msg)
{
    // Find correct channel for this
    uint8_t c = findActiveChannel(msg->Data[0]);

    if (c != NOT_FOUND)
    {
        if (Config.FilterEnabled)
        {
            // This updates our filter
            filterSetTarget(c, msg->Data[1]);
        }
        else
        {
            // No filter in use. Directly send desired At message
            sendAtMsg(c, msg->Data[1]);
        }
    }

    // Discard this msg in any case
    discardMsg(msg);
}

void broadcastMsg(midiMsg_t *msg)
{
    uint8_t c;
    uint8_t status;

    status = msg->MidiStatus & MIDI_STATUS_MASK;

    // Broadcast messages to all channels
    for (c = 0; c < Config.OutChannels; c++)
    {
        uint8_t i;
        uint8_t j;

        // Generate message with new channel, but otherwise same midi status
        i = midiio_msgNew_ISR(MIDIMSG_SOURCE_GENERATED | MIDIMSG_FLAG_MSG_OK,
                status | ChannelState[c].Channel);

        // Add the payload
        for (j = 0; j < msg->DataLen; j++)
        {
            midiio_msgAddData_ISR(i, msg->Data[j]);
        }

        midiio_msgFinish_ISR(i, 0);
    }

}

void psplit_handleMidiMsg_ISR(midiMsg_t *msg)
{
    if (Config.FilterEnabled)
    {
        if ((msg->Flags & Config.Source) != 0)
        {
            // TODO also check if message is OK

            uint8_t x = msg->MidiStatus;

            // Do we listen to this channel
            if ((x & MIDI_CHANNEL_MASK) == Config.SourceChannel)
            {
                // Deal with this message if status is something we are interested in
                switch (x & MIDI_STATUS_MASK)
                {
                case MIDI_STATUS_NOTE_ON:
                    handleNoteOne(msg);
                    break;

                case MIDI_STATUS_NOTE_OFF:
                    handleNoteOff(msg);
                    break;

                case MIDI_STATUS_KEY_ATOUCH:
                    handleKeyAT(msg);
                    break;

                case MIDI_STATUS_CTRL_CHANGE:
                case MIDI_STATUS_PROG_CHANGE:
                case MIDI_STATUS_PITCH_WHEEL:
                    broadcastMsg(msg);
                    discardMsg(msg);
                    break;

                //case MIDI_STATUS_CHAN_ATOUCH:
                default:
                    break;
                }

            }
        }
    }

}

void psplit_handleTick_ISR(void)
{
    if (TickDivider == 0)
    {
        // Check if the rate limit filter is on

        if (Config.AtRateLimit != 0)
        {
            uint8_t i;

            // It is, update all the filters and send if there is stuff to do
            for (i = 0; i < Config.OutChannels; i++)
            {
                filterUpdate(i);

                if (ChannelState[i].FilterState != ChannelState[i].LatestCc)
                {
                    sendAtMsg(i, ChannelState[i].FilterState);
                }
            }
        }

        TickDivider = TICK_DIVISION - 1;
    }
    else
    {
        TickDivider--;
    }

}

uint8_t psplit_menuGetSubCount(void)
{
    return 0;
}

void psplit_menuGetText(char *dest, uint8_t item)
{
    (void)dest;
    (void)item;
}

uint8_t psplit_menuHandleEvent(uint8_t item, uint8_t edit_mode,
        uint8_t user_event, int8_t knob_delta)
{
    (void)item;
    (void)edit_mode;
    (void)user_event;
    (void)knob_delta;

    return 0;
}

uint8_t psplit_configGetSize(void)
{
    return 0;
}

void psplit_configSave(uint8_t *dest)
{
    (void)dest;
}

void psplit_configLoad(uint8_t *dest)
{
    (void)dest;
}
