/*
 * superdelay.c
 *
 *  Created on: Feb 23, 2013
 *      Author: lars
 */


//
//
// Superdelay
//
//
// Every delay voice works as follows:
//
//
//  Example
//
//  Beat     Player       DelayVoice1       DelayVoice2      Output
//  ----     ------       -----------       -----------      ------
//   x  .... NoteOn(1) ....................................  NoteOn
//
//           NoteOff(1)                                      NoteOff
//
//           NoteOn(2)                                       NoteOn
//   x  ................  NoteOn(1) .......................  NoteOn
//           NoteOff(2)                                                 (NoteOff supressed!)
//                        NoteOff(1)                         NoteOff
//
//                                          NoteOn(2)        NoteOn
//   x  ................  NoteOn ..........................  NoteOn
//                                          NoteOff(2)                  (NoteOff supressed!)
//                        NoteOff                            NoteOff
//
//                                          NoteOn           NoteOn
//   x  ...................................................
//                                          NoteOff          NoteOff
//
//
//
// Superdelay is implemented with the following separate functionality
//
// DelayVoice Management:
// - InitVoices                  Set all voices to reset state
// - FindQuietest Voice          Go through the delayvoices and return the one which is the least active
// - KillAllVoices               Ask all delayvoices to shut up
// - CheckNoteActiveState(key)   Go through delayvoices and check if any of them is holding key
//
// DelayVoice Events
// - HandleNoteOn(mmsg)          Sets up delay voice according to note message
// - HandleTick(tick)            Does what have to be done at this tick
// - HandleNoteOff(mmsg)         Registers note off event
// - AdjustVelocity(velocity)    Change intensity of delay state
// - KillDelayVoice              Stops the delay voice. May generate NoteOff
//
// MidiMessage sending
// - SendNoteOn                  Sends note on
// - SendNoteOff                 Sends note off
//

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
#include "errors.h"
#include <string.h>

/////////////////////////    Defines     /////////////////////////

#define NOTE_OFF_VELOCITY     0


#define USE_VELOCITY_OFF      0
#define USE_VELOCITY_CHAN_AFT 1
#define USE_VELOCITY_KEY_AFT  2
#define USE_VELOCITY_NOTE_OFF 3
#define USE_VELOCITY_CC_FLAG  128

#define OVERLAP_TIME_DIFF     5      // x10 ms

typedef struct
{
    uint8_t source;
    uint8_t chan;
    bool_t  sendNoteOff;   // TODO If true, send NoteOff before repeating a note
    bool_t  hardOff;       // TODO If true, stop delaying instantly at note off
    bool_t  quantizeOn;    // TODO If true, start notes fixed on delay speed
    uint8_t tapKey;        // TODO Tapping this key will set speed
    uint8_t speed;         // Delay speed in tick count
    uint8_t feedback;      // 128 is 100%, 64 is 50%, 255 is 200%
    uint8_t stopVelocity;  // Velocity to stop repeating at
    uint8_t stopRepeat;    // 0 means unlimited. Repeat count limit
    uint8_t useVelocity;   // Special ways to change velocity of ongoing delay
} delaySetup_t;

// TODO swing delay?!

#define STATUS_FREE           0x00 // Delay inactive
#define STATUS_GOT_NOTE_ON    0x01 // We have received note on
#define STATUS_GOT_NOTE_OFF   0x02 // We have received note off
#define STATUS_IS_NOTE_ON     0x04 // Currently playing a note
#define STATUS_ARM_NOTE_ON    0x08 // Next NoteOn event is armed
#define STATUS_ARM_NOTE_OFF   0x10 // Next NoteOff event is armed
#define STATUS_DELAY_DONE     0x20 // Next NoteOff event stops this delay voice

typedef struct
{
    uint8_t  status;
    uint8_t  key;
    uint16_t keyOnDuration;   // If STATUS_GOT_NOTE_OFF key duration in ticks, otherwise time of note on
    uint16_t nextNoteOnTime;  // When to send next NoteOn
    uint16_t nextNoteOffTime; // When to send next NoteOff
    uint8_t  velocity;
    uint8_t  repeats;
} delayVoice_t;

#define DELAY_VOICES_MAX 20


/////////////////////////   Variables    /////////////////////////

static delayVoice_t DelayVoices[DELAY_VOICES_MAX];
static delaySetup_t DelaySetup;

bool_t filterEnabled;

/////////////////////////   Prototypes   /////////////////////////


// ---- DelayVoice Management ----

// Set all voices to reset state
static void InitVoices(void);


static bool_t CheckIfMasked(uint8_t voice);

// Go through the delay voices and return the one which is the least active
static uint8_t FindQuietestVoice(void);

// Ask all delay voices to shut up
static void KillAllVoices(void);

// Go through delay voices and check if any of them is holding key
static bool_t CheckNoteActiveState(uint8_t key);

// ---- DelayVoice Events ----

static void SetupNextDelayCycle(uint8_t voice); // Private

// Sets up delay voice according to note message
static void HandleNoteOn(uint8_t voice, mmsg_t *msg);

// Does what have to be done at this tick
static void HandleTick(uint8_t voice, uint16_t tick);

// Registers note off event
static void HandleNoteOff(uint8_t voice, mmsg_t *msg);

// Change intensity of delay state
static void AdjustVelocity(uint8_t voice, uint8_t velocity);

// May generate NoteOff
static void KillVoice(uint8_t voice, uint8_t dont_touch_this_key);

// ---- MidiMessage sending ----

static void SendNoteOn(uint8_t key, uint8_t velocity);
static void SendNoteOff(uint8_t key);

///////////////////////// Implementation /////////////////////////



// ---- DelayVoice Management ----

// Set all voices to reset state
static void InitVoices(void)
{
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        DelayVoices[i].status = STATUS_FREE;
    }
}

// Voice is about to sound. Check if some other delay voice actually masks the contribution
// of this voice
static bool_t CheckIfMasked(uint8_t voice)
{
    uint8_t i;
    uint8_t masked = FALSE;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (i != voice)
        {
            if (DelayVoices[i].status != STATUS_FREE)
            {
                if ((DelayVoices[i].key == DelayVoices[voice].key) &&
                      (DelayVoices[i].velocity > DelayVoices[voice].velocity))
                {
                    // Okay, we have a voice with same key and higher velocity. What about timing?
                    uint16_t time_diff;

                    time_diff = (int16_t)(DelayVoices[i].nextNoteOnTime - DelayVoices[voice].nextNoteOnTime);

                    // The result of this subtraction can be:

                    // If [i] is later than [voice]:     0 to (delay speed / 2)
                    // If [voice] is later than [i]:     (delayspeed/2 to delayspeed)

                    if ((time_diff < OVERLAP_TIME_DIFF) ||
                         ((DelaySetup.speed - time_diff) < OVERLAP_TIME_DIFF) )
                    {
                        masked = TRUE;
                        break;
                    }

                }
            }
        }
    }

    return masked;
}

// Go through the delay voices and return the one which is the least active
static uint8_t FindQuietestVoice(void)
{
    uint8_t best;
    uint8_t best_vel;
    uint8_t i;

    // Initial value of best if above maximum velocity
    best = 0;
    best_vel = 255;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].status == STATUS_FREE)
        {
            // A completely free voice, use it, we are done
            best = i;
            break;
        }
        else
        {
            // Check if this voice velocity is better than current best
            if (DelayVoices[i].velocity < best_vel)
            {
                best_vel = DelayVoices[i].velocity;
                best = i;
            }
        }
    }

    return best;
}

// Go through the delay voices and return the one which is the least active
static uint8_t FindActiveVoice(uint8_t key)
{
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].key == key)
        {
            if ((DelayVoices[i].status & (STATUS_GOT_NOTE_ON | STATUS_GOT_NOTE_OFF))
                    == STATUS_GOT_NOTE_ON)
            // This is it, the voice has got note on, but not note off for this key
            return i;
        }
    }

    // Not found!
    err_Raise(ERR_MODULE_SDELAY, __LINE__);

    return 0;
}


// Ask all delay voices to shut up
static void KillAllVoices(void)
{
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        KillVoice(i, 255);
    }
}

// Go through delay voices and check if any of them is holding key
static bool_t CheckNoteActiveState(uint8_t key)
{
    bool_t key_active = FALSE;
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].key == key)
        {
            if (DelayVoices[i].status & STATUS_IS_NOTE_ON)
            {
                // A delay voice is generating a note
                key_active = TRUE;
            }
            else if ((DelayVoices[i].status & (STATUS_GOT_NOTE_ON | STATUS_GOT_NOTE_OFF))
                    == STATUS_GOT_NOTE_ON)
            {
                // We have got note on, but not note off. It means user is holding key.
                key_active = TRUE;
            }
        }
    }

    return key_active;
}

// ---- DelayVoice Events ----


// Updates state of delay and sets up next NoteOff and NoteOn events
static void SetupNextDelayCycle(uint8_t index)
{
    uint16_t w;

    // Step forward in delay state variables:

    // Reduce velocity according to feedback
    w = DelayVoices[index].velocity;
    w = (w * (uint16_t)(DelaySetup.feedback)) >> 7u;
    if (w > 127)
    {
        w = 127;
    }
    DelayVoices[index].velocity = w;

    // Increment repeat counter
    DelayVoices[index].repeats++;

    // Store time of the current event we just had
    uint16_t this_event_time = DelayVoices[index].nextNoteOnTime;

    // Have we reached end of delaying?
    if ((DelayVoices[index].velocity <= DelaySetup.stopVelocity) ||
            ((DelaySetup.stopRepeat != 0) && (DelayVoices[index].repeats >= DelaySetup.stopRepeat)))
    {
        // Yes. Set DONE status
        DelayVoices[index].status |= STATUS_DELAY_DONE;

        // Disarm generating Note On
        DelayVoices[index].status &= ~STATUS_ARM_NOTE_ON;
    }
    else
    {
        // Prepare next Note On event
        DelayVoices[index].nextNoteOnTime = this_event_time + (uint16_t)(DelaySetup.speed);
        DelayVoices[index].status |= STATUS_ARM_NOTE_ON;
    }

    // Have user released key?
    if (DelayVoices[index].status & STATUS_GOT_NOTE_OFF)
    {
        // Yes. Then we know key duration, configure note off:
        DelayVoices[index].nextNoteOffTime = this_event_time + DelayVoices[index].keyOnDuration;
        DelayVoices[index].status |= STATUS_ARM_NOTE_OFF;
    }
    else
    {
        // User is holding the key, don't generate note off
        DelayVoices[index].status &= ~STATUS_ARM_NOTE_OFF;
    }
}


// Sets up delay voice according to note message
static void HandleNoteOn(uint8_t index, mmsg_t *msg)
{
    // At note on we don't generate the note on message. It will must be
    // let through in the hook msg interrupt function

    // Set up the delay voice according to the note on message
    DelayVoices[index].key = msg->midi_data[0];
    DelayVoices[index].keyOnDuration = msg->receive_tick;
    DelayVoices[index].nextNoteOnTime = msg->receive_tick;
    DelayVoices[index].status = STATUS_GOT_NOTE_ON;
    DelayVoices[index].repeats = 0;
    DelayVoices[index].velocity = msg->midi_data[1];

    // Prepare next delay cycle
    SetupNextDelayCycle(index);
}

// User generated a note off
static void HandleNoteOff(uint8_t index, mmsg_t *msg)
{
    if ((DelayVoices[index].status & STATUS_GOT_NOTE_OFF) == 0)
    {
        // The delay hasn't got NoteOff before now.
        DelayVoices[index].status |= STATUS_GOT_NOTE_OFF;

        // We can now calculate key duration
        DelayVoices[index].keyOnDuration = (msg->receive_tick) - (DelayVoices[index].keyOnDuration);

        // If delay voice has already stopped sounding, we need to take care of the note off event here
        if (DelayVoices[index].status & STATUS_DELAY_DONE)
        {
            DelayVoices[index].nextNoteOffTime = (msg->receive_tick) + (uint16_t)(DelaySetup.speed);
            DelayVoices[index].status |= STATUS_ARM_NOTE_OFF;
        }
    }

    // Do we let this note off survive?
    if (CheckNoteActiveState(msg->midi_data[0]) == TRUE)
    {
        // Something is keeping this note active, don't send note off
        msg->flags |= MMSG_FLAG_DISCARD;
    }
}

// Change intensity of delay state
static void AdjustVelocity(uint8_t voice, uint8_t vel)
{
    uint8_t new_vel = vel;

    if (new_vel < 20)
    {
        new_vel = 20;
    }

    new_vel += DelayVoices[voice].velocity;
    new_vel >>= 1;

    new_vel += DelayVoices[voice].velocity;
    new_vel >>= 1;

    DelayVoices[voice].velocity = new_vel;
}

// Kill off a voice. If key of voice == dont_touch_this_key, no noteoff will be sent
static void KillVoice(uint8_t voice, uint8_t dont_touch_this_key)
{
    // Force the delay voice to status free.

    // Check if the note active state changed by doing that, in that case
    // send a note off

    if (DelayVoices[voice].status != STATUS_FREE)
    {
        uint8_t key = DelayVoices[voice].key;

        DelayVoices[voice].status = STATUS_FREE;

        if (CheckNoteActiveState(key) == FALSE)
        {
            if (key != dont_touch_this_key)
            {
                SendNoteOff(key);
            }
        }

    }
}


// Do what have to be done at this tick
static void HandleTick(uint8_t voice, uint16_t tick)
{
    uint8_t status = DelayVoices[voice].status;

    if (status != STATUS_FREE)
    {
        if (status & STATUS_ARM_NOTE_OFF)
        {
            if (DelayVoices[voice].nextNoteOffTime == tick)
            {
                if (CheckNoteActiveState(DelayVoices[voice].key))
                {
                    // We must generate a note off. Remove note on status
                    DelayVoices[voice].status = status & ~STATUS_IS_NOTE_ON;

                    // Send note off if nothing else holding this key active
                    if (CheckNoteActiveState(DelayVoices[voice].key) == FALSE)
                    {
                        SendNoteOff(DelayVoices[voice].key);
                    }
                }

                // Was this the final note off?
                if (status & STATUS_DELAY_DONE)
                {
                    // Then set delay voice free
                    DelayVoices[voice].status = STATUS_FREE;
                }
            }
        }

        if (status & STATUS_ARM_NOTE_ON)
        {
            if (DelayVoices[voice].nextNoteOnTime == tick)
            {
                if (CheckIfMasked(voice) == FALSE)
                {
                    // We must generate a note on

                    if (CheckNoteActiveState(DelayVoices[voice].key) == TRUE)
                    {
                        SendNoteOff(DelayVoices[voice].key);
                    }

                    DelayVoices[voice].status = status | STATUS_IS_NOTE_ON;

                    SendNoteOn(DelayVoices[voice].key, DelayVoices[voice].velocity);
                }

                // Advance to next delay cycle in any case
                SetupNextDelayCycle(voice);
            }
        }
    }
}


// ---- MidiMessage sending ----

static void SendNoteOn(uint8_t key, uint8_t velocity)
{
    uint8_t msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_GENERATED | MMSG_FLAG_MSG_OK,
            MIDI_STATUS_NOTE_ON | DelaySetup.chan);
    midiio_MsgAddData_ISR(msg_index, key);
    midiio_MsgAddData_ISR(msg_index, velocity);
    midiio_MsgFinish_ISR(msg_index, 0);
}

static void SendNoteOff(uint8_t key)
{
    uint8_t msg_index = midiio_MsgNew_ISR(MMSG_SOURCE_GENERATED | MMSG_FLAG_MSG_OK,
            MIDI_STATUS_NOTE_OFF | DelaySetup.chan);
    midiio_MsgAddData_ISR(msg_index, key);
    midiio_MsgAddData_ISR(msg_index, NOTE_OFF_VELOCITY);
    midiio_MsgFinish_ISR(msg_index, 0);
}





// ---- Public functions ----

void sdelay_Initialize(void)
{
    // Set setup defaults
    filterEnabled = FALSE;
    memset(&DelaySetup, 0, sizeof(delaySetup_t));

    // Set delay state defaults
    InitVoices();

    // Set some defaults for testing:
    DelaySetup.feedback = 90;
    DelaySetup.stopRepeat = 0;
    DelaySetup.stopVelocity = 4;
    DelaySetup.speed = 50; // One second
    DelaySetup.chan = 0;
    DelaySetup.source = MMSG_SOURCE_INPUT1;
    DelaySetup.sendNoteOff = FALSE;// TRUE;

    filterEnabled = TRUE;
}

void sdelay_HookMidiMsg_ISR(mmsg_t *msg)
{
    if ((msg->flags & DelaySetup.source) != 0)
    {
        // TODO also check if message is OK

        uint8_t x = msg->midi_status;

        // Do we listen to this channel
        if ((x & MIDI_CHANNEL_MASK) == DelaySetup.chan)
        {
            uint8_t voice;

            // Deal with this message if status is something we are interested in
            switch (x & MIDI_STATUS_MASK)
            {
            case MIDI_STATUS_NOTE_ON:
                // Find a delayvoice for this note on
                voice = FindQuietestVoice();
                KillVoice(voice, msg->midi_data[0]);
                HandleNoteOn(voice, msg);
                break;

            case MIDI_STATUS_NOTE_OFF:
                // Find the delayvoice that needs to get this note off
                voice = FindActiveVoice(msg->midi_data[0]);
                HandleNoteOff(voice, msg);
                break;

            case MIDI_STATUS_KEY_ATOUCH:
                // Find the delayvoice that needs to get this note off
                //voice = FindActiveVoice(msg->midi_data[0]);
                //AdjustVelocity(voice, msg->midi_data[1]);
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

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        HandleTick(i, tickNow);
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

    memcpy(dest, &(DelaySetup), sizeof(delaySetup_t));
}

void sdelay_ConfigLoad(uint8_t *dest)
{
    filterEnabled = *(dest++);

    memcpy(&(DelaySetup), dest, sizeof(delaySetup_t));
}


