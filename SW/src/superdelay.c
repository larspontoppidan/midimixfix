/*
 * superdelay.c
 *
 *  Created on: Feb 23, 2013
 *      Author: lars
 */


//
//
// Midi Delay
//
// How to implement:
//
//

// MODE: Flood
// Delay:  *                   *                   *
// Input:  NoteOn                       NoteOff
// Output: NoteOn              NoteOn              NoteOn                       NoteOff
//
//
// MODE: SavePoly
// Delay:  *                   *                   *
// Input:  NoteOn                       NoteOff
// Output: NoteOn      NoteOff,NoteOn      NoteOff,NoteOn                       NoteOff
//
//
// MODE: HardOff
// Delay:  *                   *                   *
// Input:  NoteOn                       NoteOff
// Output: NoteOn      NoteOff,NoteOn   NoteOff
//
// MODE: FixedBeat
// Delay:         *                   *                   *
// Input:  NoteOn                       NoteOff
// Output:        NoteOn      NoteOff,NoteOn   NoteOff
//
//
// The delay continues
// VelNew = VelOld *

// Settings:
// SuperDelay       (ON)
// Delay on In1 Chan15
// Mode : SavePoly
// TapSpeed : OFF  / C-4
// Time     : 50 ms  to 650000 ms
// Feedback : 0 - 255
// Stop     : Vel.100, Vel.100, Rep.10     / 1 2 3, - 99
// Use.vel. : OFF / Chan.aft. / Key.Aft / NoteOff / Modulation


/////////////////////////    INCLUDES    /////////////////////////



#include "common.h"
#include "superdelay.h"
#include "midimessage.h"
#include "hal.h"
#include "midiio.h"
#include "util.h"
#include "menu.h"
#include "pgmstrings.h"
#include <string.h>

/////////////////////////    Defines     /////////////////////////

#define NOTE_OFF_VELOCITY     0


#define USE_VELOCITY_OFF      0
#define USE_VELOCITY_CHAN_AFT 1
#define USE_VELOCITY_KEY_AFT  2
#define USE_VELOCITY_NOTE_OFF 3
#define USE_VELOCITY_CC_FLAG  128


typedef struct
{
    uint8_t source;
    uint8_t chan;
    bool_t  sendNoteOff;   // If true, send NoteOff before repeating a note
    bool_t  hardOff;       // TODO If true, stop delaying instantly at note off
    bool_t  quantizeOn;    // TODO If true, start notes fixed on delay speed
    uint8_t tapKey;        // TODO Tapping this key will set speed
    uint8_t speed;         // Delay speed in tick count
    uint8_t feedback;      // 128 is 100%, 64 is 50%, 255 is 200%
    uint8_t stopVelocity;  // Velocity to stop repeating at
    uint8_t stopRepeat;    // 0 means unlimited. Repeat count limit
    uint8_t useVelocity;   // Special ways to change velocity of ongoing delay
} delaySetup_t;


#define STATUS_FREE           0x00 // Delay inactive
#define STATUS_GOT_NOTE_ON    0x01 // We have had note on
#define STATUS_GOT_NOTE_OFF   0x02 // We have had note off
#define STATUS_DELAY_DONE     0x04 // Delay have finished repeating
#define STATUS_SEND_NOTE_ON   0x08 // Next note on event is armed
#define STATUS_SEND_NOTE_OFF  0x10 // Next note off event is armed


typedef struct
{
    uint8_t  status;
    uint8_t  key;
    uint16_t keyOnDuration;   // If STATE_NOTE_ON: Tick count at NoteOn. Otherwise, key duration in ticks
    uint16_t nextNoteOnTime;  // When to send next NoteOn
    uint16_t nextNoteOffTime; // When to send next NoteOff
    uint8_t  velocity;
    uint8_t  repeats;
} delayState_t;

#define DELAY_KEYS_MAX 12

#define RETURN_INVALID 0xFF

/////////////////////////   Variables    /////////////////////////

static delayState_t delays[DELAY_KEYS_MAX];
static delaySetup_t delaySetup;

bool_t filterEnabled;

/////////////////////////   Prototypes   /////////////////////////

static uint8_t findDelay(uint8_t key);
static uint8_t newDelay(void);

static void processNoteOn(mmsg_t *msg);
static void processKeyAfterTouch(mmsg_t *msg);
static void processNoteOff(mmsg_t *msg);

static void setupNextDelayCycle(uint8_t index);

static void doNoteOnEvent(uint8_t index);
static void doNoteOffEvent(uint8_t index);

///////////////////////// Implementation /////////////////////////


// Delay engine
// ------------


// Seek for key in active delay records.
// Returns RETURN_INVALID if not found
static uint8_t findDelay(uint8_t key)
{
    uint8_t i;

    for (i = 0; i < DELAY_KEYS_MAX; i++)
    {
        if ((delays[i].status != STATUS_FREE) &&
                (delays[i].key == key))
        {
            return i;
        }
    }

    return RETURN_INVALID;
}

// Seek for empty delay record
// Returns RETURN_INVALID if out of space!
static uint8_t newDelay(void)
{
    uint8_t i;

    for (i = 0; i < DELAY_KEYS_MAX; i++)
    {
        if (delays[i].status == STATUS_FREE)
        {
            return i;
        }
    }

    return RETURN_INVALID;
}

// Process a key on message.
static void processNoteOn(mmsg_t *msg)
{
    uint8_t index;
    uint8_t key;

    key = msg->midi_data[0];

    // Look for key in active delays:
    index = findDelay(key);

    if (index == RETURN_INVALID)
    {
        // Not already in active delays, create new record
        index = newDelay();
    }

    if (index != RETURN_INVALID)
    {
        delays[index].key = key;
        delays[index].keyOnDuration = msg->receive_tick;
        delays[index].nextNoteOnTime = msg->receive_tick;
        delays[index].status = STATUS_GOT_NOTE_ON;
        delays[index].repeats = 0;
        delays[index].velocity = msg->midi_data[1];

        setupNextDelayCycle(index);
    }
    else
    {
        // Out of space in delay records! Don't process this key on.
    }
}


// Process a key off message.
static void processNoteOff(mmsg_t *msg)
{
    uint8_t index;
    uint8_t key;

    key = msg->midi_data[0];

    // Look for key in active delays:
    index = findDelay(key);

    if (index == RETURN_INVALID)
    {
        // Not already in active delays, strange, just leave this message alone
    }
    else
    {
        if ((delays[index].status & STATUS_GOT_NOTE_OFF) == 0)
        {
            // The delay hasn't got NoteOff before now.
            delays[index].status |= STATUS_GOT_NOTE_OFF;

            // We can now calculate key duration
            delays[index].keyOnDuration = (msg->receive_tick) - (delays[index].keyOnDuration);

            // Is keyduration is shorter than our delay speed?
            if (delays[index].keyOnDuration < (uint16_t)(delaySetup.speed))
            {
                // Let the NoteOff message be sent
            }
            else
            {
                // It's longer. Is this a long awaited final note off after delay has finished?
                if (delays[index].status & STATUS_DELAY_DONE)
                {
                    // Then configure a note off event delayed by delay speed
                    delays[index].nextNoteOffTime = msg->receive_tick + (uint16_t)(delaySetup.speed);
                    delays[index].status |= STATUS_SEND_NOTE_OFF;
                }

                // This delay message should be discarded
                msg->flags |= MMSG_FLAG_DISCARD;
            }
        }
        else
        {
            // This is an unexpected note off while our delay is active? Kill it
            msg->flags |= MMSG_FLAG_DISCARD;
        }

    }
}

// Process a key aftertouch message.
static void processKeyAfterTouch(mmsg_t *msg)
{
    // TODO
}


// Updates state of delay and sets up next NoteOff and NoteOn event.
//
// This function must be called after very first NoteOn has been sent,
// or after a NoteOn event has been performed.
//
static void setupNextDelayCycle(uint8_t index)
{
    uint16_t w;

    // At this point, we have just sent a NoteOn message.

    // Step forward in delay state variables:

    // Reduce velocity according to feedback
    w = delays[index].velocity;
    w = (w * (uint16_t)(delaySetup.feedback)) >> 7u;
    if (w > 127)
    {
        w = 127;
    }
    delays[index].velocity = w;

    // Increment repeat counter
    delays[index].repeats++;

    // Store time of the current event we just had
    uint16_t this_event_time = delays[index].nextNoteOnTime;

    // Clear the send note on/off statuses
    delays[index].status &= ~(STATUS_SEND_NOTE_ON | STATUS_SEND_NOTE_OFF);

    // Have we reached end of delaying?
    if ((delays[index].velocity <= delaySetup.stopVelocity) ||
            ((delaySetup.stopRepeat != 0) && (delays[index].repeats >= delaySetup.stopRepeat)))
    {
        // Yes
        delays[index].status |= STATUS_DELAY_DONE;

        // Have user actually released key?
        if (delays[index].status & STATUS_GOT_NOTE_OFF)
        {
            // Yes. Configure last note off to be after key duration
            delays[index].nextNoteOffTime = this_event_time + delays[index].keyOnDuration;
            delays[index].status |= STATUS_SEND_NOTE_OFF;
        }
        else
        {
            // User is holding the key longer than it took the delay to finish repeating.
            // We don't know if the voice may actually still sound, so we must wait.
        }
    }
    else
    {
        // Delay has not ended. Prepare next NoteOff event:

        // Do we know key duration? and is it shorter than our delay speed?
        if ((delays[index].status & STATUS_GOT_NOTE_OFF) &&
            (delays[index].keyOnDuration < (uint16_t)delaySetup.speed))
        {
            // It's shorter, in this case we must configure a note off after key duration
            delays[index].nextNoteOffTime = this_event_time + delays[index].keyOnDuration;
            delays[index].status |= STATUS_SEND_NOTE_OFF;
        }
        else
        {
            // Key duration is longer than delay speed, or user have not lifted key yet

            // Depending on configuration we may send a NoteOff just before the next NoteOn:
            if (delaySetup.sendNoteOff)
            {
                delays[index].nextNoteOffTime = this_event_time + (uint16_t)(delaySetup.speed) - 1;
                delays[index].status |= STATUS_SEND_NOTE_OFF;
            }
        }

        // Prepare next NoteOn event. Simple:
        delays[index].nextNoteOnTime = this_event_time + (uint16_t)(delaySetup.speed);
        delays[index].status |= STATUS_SEND_NOTE_ON;
    }

}


static void doNoteOnEvent(uint8_t index)
{
    uint8_t msg_index;

    // Send the note on message
    if (delays[index].status & STATUS_SEND_NOTE_ON)
    {
        msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_GENERATED | MMSG_FLAG_MSG_OK,
                MIDI_STATUS_NOTE_ON | delaySetup.chan);
        midiio_MsgAddData_ISR(msg_index, delays[index].key);
        midiio_MsgAddData_ISR(msg_index, delays[index].velocity);
        midiio_MsgFinish_ISR(msg_index, 0);

        // After note on, we must prepare the next cycle
        setupNextDelayCycle(index);
    }
}

static void doNoteOffEvent(uint8_t index)
{
    uint8_t msg_index;

    // Send note off message
    if (delays[index].status & STATUS_SEND_NOTE_OFF)
    {
        msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_GENERATED | MMSG_FLAG_MSG_OK,
                MIDI_STATUS_NOTE_OFF | delaySetup.chan);
        midiio_MsgAddData_ISR(msg_index, delays[index].key);
        midiio_MsgAddData_ISR(msg_index, NOTE_OFF_VELOCITY);
        midiio_MsgFinish_ISR(msg_index, 0);

        // Check if this was the final note off
        if (delays[index].status & STATUS_DELAY_DONE)
        {
            // We are done, release this delay
            delays[index].status = STATUS_FREE;
        }
    }
}


// Public functions

void sdelay_Initialize(void)
{
    // Set setup defaults
    filterEnabled = FALSE;
    memset(&delaySetup, 0, sizeof(delaySetup_t));

    // Set delay state defaults
    memset(delays, 0, sizeof(delayState_t) * DELAY_KEYS_MAX);

    // Set some defaults for testing:
    delaySetup.feedback = 90;
    delaySetup.stopRepeat = 0;
    delaySetup.stopVelocity = 3;
    delaySetup.speed = 50; // One second
    delaySetup.chan = 0;
    delaySetup.source = MMSG_SOURCE_INPUT1;
    delaySetup.sendNoteOff = FALSE;// TRUE;

    filterEnabled = TRUE;
}

void sdelay_HookMidiMsg_ISR(mmsg_t *msg)
{
    if ((msg->flags & delaySetup.source) != 0)
    {
        // TODO also check if message is OK

        uint8_t x = msg->midi_status;

        // Do we listen to this channel
        if ((x & MIDI_CHANNEL_MASK) == delaySetup.chan)
        {
            // Deal with this message if status is something we are interested in
            switch (x & MIDI_STATUS_MASK)
            {
            case MIDI_STATUS_NOTE_ON:
                processNoteOn(msg);
                break;
            case MIDI_STATUS_NOTE_OFF:
                processNoteOff(msg);
                break;
            case MIDI_STATUS_KEY_ATOUCH:
                processKeyAfterTouch(msg);
                break;
            }
        }
    }
}


void sdelay_HookTick_ISR(void)
{
    // Go through the active delays and check if there are delay events to do

    uint8_t i;
    uint16_t tickNow = hal_TickCountGet_ISR();

    for (i = 0; i < DELAY_KEYS_MAX; i++)
    {
        if (delays[i].status != STATUS_FREE)
        {
            if (delays[i].nextNoteOffTime == tickNow)
            {
                doNoteOffEvent(i);
            }

            if (delays[i].nextNoteOnTime == tickNow)
            {
                doNoteOnEvent(i);
            }
        }
    }
}


void sdelay_HookMainLoop(void)
{
    // TODO Maybe do maintenance to make sure we don't have stuck delays hanging around
}



uint8_t sdelay_MenuGetSubCount(void)
{
    return 0;
}

void    sdelay_MenuGetText(char *dest, uint8_t item)
{
    dest[0] = 'S';
    dest[1] = 'D';
    dest[2] = 0;
}

uint8_t sdelay_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    return 0;
}




uint8_t sdelay_ConfigGetSize(void)
{
    return 1 + sizeof(delaySetup_t);
}

void sdelay_ConfigSave(uint8_t *dest)
{
    *(dest++) = filterEnabled;

    memcpy(dest, &(delaySetup), sizeof(delaySetup_t));
}

void sdelay_ConfigLoad(uint8_t *dest)
{
    filterEnabled = *(dest++);

    memcpy(&(delaySetup), dest, sizeof(delaySetup_t));
}




