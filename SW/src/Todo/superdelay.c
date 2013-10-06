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
//
//
//  In this example, the player plays two note ons on the same key:
//
//  Delay
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
#include "midigenerics.h"
#include "hal.h"
#include "midiio.h"
#include "util.h"
#include "menu.h"
#include "pgmstrings.h"
#include "errors.h"
#include <string.h>
#include <avr/pgmspace.h>

/////////////////////////    Defines     /////////////////////////

#define NOTE_OFF_VELOCITY     0


#define USE_VELOCITY_OFF      0
#define USE_VELOCITY_CHAN_AFT 1
#define USE_VELOCITY_KEY_AFT  2
#define USE_VELOCITY_NOTE_OFF 3
#define USE_VELOCITY_CC_FLAG  128

#define OVERLAP_TIME_DIFF     6      // x10 ms

#define MINIMUM_SPEED         8      // x10 ms

#define INVALID_NUMBER        0xFF

typedef struct
{
    uint8_t Source;
    uint8_t Chan;
    uint8_t TapKey;        // TODO Tapping this key will set speed
    uint8_t Speed;         // Delay speed in tick count
    int8_t  SwingAmount;  // Added to speed of 1 3 5... beats, subtracted from 2 4 6... beats
    uint8_t Feedback;      // 128 is 100%, 64 is 50%, 255 is 200%
    uint8_t StopVelocity;  // Velocity to stop repeating at
    uint8_t StopRepeat;    // 0 means unlimited. Repeat count limit
    uint8_t EnableCc;      // Midi controller to enable / disable new delay voices
} delaySetup_t;


#define STATUS_FREE           0x00 // Delay inactive
#define STATUS_GOT_NOTE_ON    0x01 // We have received note on
#define STATUS_GOT_NOTE_OFF   0x02 // We have received note off
#define STATUS_IS_NOTE_ON     0x04 // Currently playing a note
#define STATUS_ARM_NOTE_ON    0x08 // Next NoteOn event is armed
#define STATUS_ARM_NOTE_OFF   0x10 // Next NoteOff event is armed
#define STATUS_DELAY_DONE     0x20 // Next NoteOff event stops this delay voice
#define STATUS_PROLONGED      0x40 // This delay cycle uses the speed + swing_amount (instead of - )
#define STATUS_MUTED          0x80 // This delay cycle uses the speed + swing_amount (instead of - )


typedef struct
{
    uint8_t  Status;
    uint8_t  Key;
    uint16_t KeyOnDuration;   // If STATUS_GOT_NOTE_OFF key duration in ticks, otherwise time of note on
    uint16_t NextNoteOnTime;  // When to send next NoteOn
    uint16_t NextNoteOffTime; // When to send next NoteOff
    uint8_t  Velocity;
    uint8_t  Repeats;
} delayVoice_t;

#define DELAY_VOICES_MAX 20


// Menu


#define MENU_TITLE     0  // " Midi delay     (ON)"
#define MENU_IN_CHAN   1  // "  Use In:1 Chan:1   "
#define MENU_SPEED     2  // "  Delay Time:XXXX ms"
#define MENU_SWING     3  // "  Swing     :XXX  ms"
#define MENU_FEEDBACK  4  // "  Feedback  :110    "
#define MENU_STOP_VEL  5  // "  Stop Velocity:XX  "
#define MENU_STOP_REP  6  // "  Stop Repeats :XX  "
#define MENU_ENABLE_CC 7  // "  EnableCC:OFF      "
#define MENU_TAP_SPEED 8  // "  Tap Speed Key:C-4 "

#define MENU_SUB_ITEMS 8

typedef struct
{
    char Text[19];
    uint8_t Cursor;
    void *Data;
} menuItem_t;

#define MENU_ITEMS 10
#define MENU_SPECIAL_CHAN 9


/////////////////////////   Variables    /////////////////////////

static delayVoice_t DelayVoices[DELAY_VOICES_MAX];
static delaySetup_t DelaySetup;

static bool_t FilterEnabled;

static uint16_t TapSpeedTick;
static bool_t TapSpeedValid;

static bool_t DynamicEnable;


static menuItem_t const MenuItems[MENU_ITEMS] PROGMEM =
{
    {"Midi delay    %O",  17, &(FilterEnabled) },
    {"Input: %i",          7, &(DelaySetup.Source) },
    {"Del.Time: %U0 ms",  13, &(DelaySetup.Speed) },
    {"Swing   :%I",       13, &(DelaySetup.SwingAmount) },
    {"Feedback:  %U",     13, &(DelaySetup.Feedback) },
    {"Stop Vel:  %U",     13, &(DelaySetup.StopVelocity) },
    {"Stop Rep:  %U",     13, &(DelaySetup.StopRepeat) },
    {"EnableCC:%c",        9, &(DelaySetup.EnableCc) },
    {"TapSpeed: %n",      10, &(DelaySetup.TapKey) },
    {" Chan: %i",         15, &(DelaySetup.Chan) },
};


/////////////////////////   Prototypes   /////////////////////////


// ---- DelayVoice Management ----

// Set all voices to reset state
static void initVoices(void);

static void setFilterEnabled(bool_t enable);  // TODO mark as main thread!

static bool_t checkIfMasked(uint8_t voice);

// Go through the delay voices and return the one which is the least active
static uint8_t findQuietestVoice(void);

// Go through delay voices and check if any of them is holding key
static uint8_t checkNoteActiveState(uint8_t key);

// ---- DelayVoice Events ----

static void setupNextDelayCycle(uint8_t voice); // Private

// Sets up delay voice according to note message
static void handleNoteOn(uint8_t voice, midiMsg_t *msg);

// Does what have to be done at this tick
static void handleTick(uint8_t voice, uint16_t tick);

// Registers note off event
static void handleNoteOff(uint8_t voice, midiMsg_t *msg);

// May generate NoteOff
static void killVoice(uint8_t voice, uint8_t dont_touch_this_key);

// ---- TapSpeed functionality

static void handleTapSpeedEvent(uint16_t tick);
static void handleTapSpeedTick(uint16_t tick);

// ---- MidiMessage sending ----

static void sendNoteOn(uint8_t key, uint8_t velocity);
static void sendNoteOff(uint8_t key);

// ---- MENU implementation ----

static char *writeMenuLine(char *dest, uint8_t item);

///////////////////////// Implementation /////////////////////////



// ---- DelayVoice Management ----

// Set all voices to reset state
static void initVoices(void)
{
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        DelayVoices[i].Status = STATUS_FREE;
    }
}

// Main thread function
static void setFilterEnabled(bool_t enable)
{
    if (enable)
    {
        initVoices();
        TapSpeedValid = FALSE;
        DynamicEnable = (DelaySetup.EnableCc == INVALID_NUMBER);
    }
    else
    {
        if (FilterEnabled)
        {
            uint8_t i;

            // Filter is enabled right now. We must make sure active voices send a note off
            hal_interruptsDisable();

            for (i = 0; i < DELAY_VOICES_MAX; i++)
            {
                if (DelayVoices[i].Status != STATUS_FREE)
                {
                    DelayVoices[i].Status |= STATUS_DELAY_DONE;
                }
            }

            hal_interruptsEnable();
        }
    }


    FilterEnabled = enable;
}

// Voice is about to sound. Check if some other delay voice actually masks the contribution
// of this voice
static bool_t checkIfMasked(uint8_t voice)
{
    uint8_t i;
    uint8_t masked = FALSE;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (i != voice)
        {
            if (DelayVoices[i].Status != STATUS_FREE)
            {
                if ((DelayVoices[i].Key == DelayVoices[voice].Key) &&
                      (DelayVoices[i].Velocity > DelayVoices[voice].Velocity))
                {
                    // Okay, we have a voice with same key and higher velocity. What about timing?
                    uint16_t time_diff;

                    time_diff = (int16_t)(DelayVoices[i].NextNoteOnTime - DelayVoices[voice].NextNoteOnTime);

                    // The result of this subtraction can be:

                    // If [i] is later than [voice]:     0 to (delay speed / 2)
                    // If [voice] is later than [i]:     (delayspeed/2 to delayspeed)

                    if (DelayVoices[i].Status & STATUS_PROLONGED)
                    {
                        if ((time_diff < OVERLAP_TIME_DIFF) ||
                             (((uint16_t)(DelaySetup.Speed + DelaySetup.SwingAmount) - time_diff) < OVERLAP_TIME_DIFF) )
                        {
                            masked = TRUE;
                        }
                    }
                    else
                    {
                        if ((time_diff < OVERLAP_TIME_DIFF) ||
                             (((uint16_t)(DelaySetup.Speed - DelaySetup.SwingAmount) - time_diff) < OVERLAP_TIME_DIFF) )
                        {
                            masked = TRUE;
                        }
                    }
                }
            }
        }

        if (masked)
        {
            break;
        }
    }

    return masked;
}

// Go through the delay voices and return the one which is the least active
static uint8_t findQuietestVoice(void)
{
    uint8_t best;
    uint8_t best_vel;
    uint8_t i;

    // Initial value of best if above maximum velocity
    best = 0;
    best_vel = 255;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].Status == STATUS_FREE)
        {
            // A completely free voice, use it, we are done
            best = i;
            break;
        }
        else
        {
            // Check if this voice velocity is better than current best
            if (DelayVoices[i].Velocity < best_vel)
            {
                best_vel = DelayVoices[i].Velocity;
                best = i;
            }
        }
    }

    return best;
}

// Find the voice which has registered note on, but not note off, for key
static uint8_t FindActiveVoice(uint8_t key)
{
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].Key == key)
        {
            if ((DelayVoices[i].Status & (STATUS_GOT_NOTE_ON | STATUS_GOT_NOTE_OFF))
                    == STATUS_GOT_NOTE_ON)
            // This is it, the voice has got note on, but not note off for this key
            return i;
        }
    }

    // Not found!

    return 0;
}


// Go through delay voices and check if any of them is holding key
static uint8_t checkNoteActiveState(uint8_t key)
{
    uint8_t key_active = 0;
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].Key == key)
        {
            if (DelayVoices[i].Status & STATUS_IS_NOTE_ON)
            {
                // A delay voice is generating a note
                key_active = DelayVoices[i].Velocity;
            }
            else if ((DelayVoices[i].Status & (STATUS_GOT_NOTE_ON | STATUS_GOT_NOTE_OFF))
                    == STATUS_GOT_NOTE_ON)
            {
                // We have got note on, but not note off. It means user is holding key.
                key_active = DelayVoices[i].Velocity;
            }
        }

        if (key_active != 0)
        {
            break;
        }
    }

    return key_active;
}

// ---- DelayVoice Events ----


// Updates state of delay and sets up next NoteOff and NoteOn events
static void setupNextDelayCycle(uint8_t index)
{
    uint16_t w;

    // Step forward in delay state variables:

    // Reduce velocity according to feedback
    w = DelayVoices[index].Velocity;
    w = (w * (uint16_t)(DelaySetup.Feedback)) >> 7u;
    if (w > 127)
    {
        w = 127;
    }

    DelayVoices[index].Velocity = w;

    // Increment repeat counter
    DelayVoices[index].Repeats++;

    // Store time of the current event we just had
    uint16_t this_event_time = DelayVoices[index].NextNoteOnTime;

    // Toggle prolonged state
    DelayVoices[index].Status ^= STATUS_PROLONGED;

    // Have we reached end of delaying?
    if ((DelayVoices[index].Velocity <= DelaySetup.StopVelocity) ||
            ((DelaySetup.StopRepeat != 0) && (DelayVoices[index].Repeats >= DelaySetup.StopRepeat)))
    {
        // Yes. Set DONE status
        DelayVoices[index].Status |= STATUS_DELAY_DONE;

        // Disarm generating Note On
        DelayVoices[index].Status &= ~STATUS_ARM_NOTE_ON;
    }
    else
    {
        // Prepare next Note On event
        uint8_t speed;

        if (DelayVoices[index].Status & STATUS_PROLONGED)
        {
            speed = DelaySetup.Speed + DelaySetup.SwingAmount;
        }
        else
        {
            speed = DelaySetup.Speed - DelaySetup.SwingAmount;
        }

        DelayVoices[index].NextNoteOnTime = this_event_time + (uint16_t)(speed);

        DelayVoices[index].Status |= STATUS_ARM_NOTE_ON;
    }

    if ((DelayVoices[index].Status & STATUS_MUTED) == 0)
    {
        // Have user released key?
        if (DelayVoices[index].Status & STATUS_GOT_NOTE_OFF)
        {
            // Yes. Then we know key duration, configure note off:
            DelayVoices[index].NextNoteOffTime = this_event_time + DelayVoices[index].KeyOnDuration;
            DelayVoices[index].Status |= STATUS_ARM_NOTE_OFF;
        }
        else
        {
            // User is holding the key, don't generate note off
            DelayVoices[index].Status &= ~STATUS_ARM_NOTE_OFF;
        }
    }
}


// Sets up delay voice according to note message
static void handleNoteOn(uint8_t index, midiMsg_t *msg)
{
    DelayVoices[index].Key = msg->Data[0];
    DelayVoices[index].KeyOnDuration = msg->ReceivedTick;
    DelayVoices[index].NextNoteOnTime = msg->ReceivedTick;
    DelayVoices[index].Repeats = 0;
    DelayVoices[index].Velocity = msg->Data[1];

    if (DynamicEnable)
    {
        DelayVoices[index].Status = STATUS_GOT_NOTE_ON;
    }
    else
    {
        // We are not enabled. Delay voice is status muted
        DelayVoices[index].Status = STATUS_GOT_NOTE_ON | STATUS_MUTED;
    }

    setupNextDelayCycle(index);
}

// User generated a note off
static void handleNoteOff(uint8_t index, midiMsg_t *msg)
{
    if ((DelayVoices[index].Status & STATUS_GOT_NOTE_OFF) == 0)
    {
        // The delay hasn't got NoteOff before now.
        DelayVoices[index].Status |= STATUS_GOT_NOTE_OFF;

        // We can now calculate key duration
        DelayVoices[index].KeyOnDuration = (msg->ReceivedTick) - (DelayVoices[index].KeyOnDuration);

        // If delay voice has already stopped sounding, we need to take care of the note off event here
        if (DelayVoices[index].Status & STATUS_DELAY_DONE)
        {
            DelayVoices[index].NextNoteOffTime = (msg->ReceivedTick) + (uint16_t)(DelaySetup.Speed);
            DelayVoices[index].Status |= STATUS_ARM_NOTE_OFF;
        }
    }

    // Do we let this note off survive?
    if (checkNoteActiveState(msg->Data[0]) == TRUE)
    {
        // Something is keeping this note active, don't send note off
        msg->Flags |= MIDIMSG_FLAG_DISCARD;
    }
}


// Kill off a voice. If key of voice == dont_touch_this_key, no noteoff will be sent
static void killVoice(uint8_t voice, uint8_t dont_touch_this_key)
{
    // Force the delay voice to status free.

    // Check if the note active state changed by doing that, in that case
    // send a note off

    if (DelayVoices[voice].Status != STATUS_FREE)
    {
        uint8_t key = DelayVoices[voice].Key;

        DelayVoices[voice].Status = STATUS_FREE;

        if (key != dont_touch_this_key)
        {
            if (checkNoteActiveState(key) == FALSE)
            {
                sendNoteOff(key);
            }
        }

    }
}


// Do what have to be done at this tick
static void handleTick(uint8_t voice, uint16_t tick)
{
    uint8_t status = DelayVoices[voice].Status;

    if (status != STATUS_FREE)
    {
        if (status & STATUS_ARM_NOTE_OFF)
        {
            if (DelayVoices[voice].NextNoteOffTime == tick)
            {
                if (checkNoteActiveState(DelayVoices[voice].Key))
                {
                    // We must generate a note off. Remove note on status
                    DelayVoices[voice].Status = status & ~STATUS_IS_NOTE_ON;

                    // Send note off if nothing else holding this key active
                    if (checkNoteActiveState(DelayVoices[voice].Key) == FALSE)
                    {
                        sendNoteOff(DelayVoices[voice].Key);
                    }
                }

                // Was this the final note off?
                if (status & STATUS_DELAY_DONE)
                {
                    // Then set delay voice free
                    DelayVoices[voice].Status = STATUS_FREE;
                }
            }
        }

        if (status & STATUS_ARM_NOTE_ON)
        {
            if (DelayVoices[voice].NextNoteOnTime == tick)
            {
                if ((status & STATUS_MUTED) == 0)
                {
                    if (checkIfMasked(voice) == FALSE)
                    {
                        uint8_t velocity;

                        // We must generate a note on
                        velocity = checkNoteActiveState(DelayVoices[voice].Key);

                        if (velocity != 0u)
                        {
                            sendNoteOff(DelayVoices[voice].Key);
                        }

                        // Use the highest velocity of our delayvoice and the note
                        // we may just have silenced:
                        if (DelayVoices[voice].Velocity > velocity)
                        {
                            velocity = DelayVoices[voice].Velocity;
                        }

                        DelayVoices[voice].Status = status | STATUS_IS_NOTE_ON;

                        sendNoteOn(DelayVoices[voice].Key, velocity);
                    }
                }

                setupNextDelayCycle(voice);
            }
        }
    }
}

// ---- Tap Speed handling ----

static void handleTapSpeedEvent(uint16_t tick)
{
    if (TapSpeedValid)
    {
        uint16_t speedProposal;
        uint8_t swing;

        speedProposal = tick - TapSpeedTick;
        swing = DelaySetup.SwingAmount;

        if ((int8_t)swing < 0)
        {
            // Maybe we have use weird negative swing...
            swing = -swing;
        }

        if (swing != 0)
        {
            // We have swing going, user probably wants to tap double tempo
            speedProposal >>= 1;
        }

        if ((speedProposal < (uint16_t)(254 - swing)) &&
            (speedProposal > (uint16_t)(MINIMUM_SPEED + swing)))
        {
            // Allright, we got a new delay speed:
            DelaySetup.Speed = (uint8_t)speedProposal;

            menu_notifyRefresh_SAFE();
        }

    }

    // In any case, we got a new valid speed tapping input
    TapSpeedTick = tick;
    TapSpeedValid = TRUE;
}

static void handleTapSpeedTick(uint16_t tick)
{
    // Make sure we invalidate a speed tap, if we are above limit

    if (TapSpeedValid)
    {
        uint16_t speedProposal;
        speedProposal = tick - TapSpeedTick;

        if (speedProposal > 255)
        {
            TapSpeedValid = FALSE;
        }
    }
}

// ---- MidiMessage sending ----

static void sendNoteOn(uint8_t key, uint8_t velocity)
{
    uint8_t msg_index = midiio_msgNew_ISR(MIDIMSG_SOURCE_GENERATED | MIDIMSG_FLAG_MSG_OK,
            MIDI_STATUS_NOTE_ON | DelaySetup.Chan);
    midiio_msgAddData_ISR(msg_index, key);
    midiio_msgAddData_ISR(msg_index, velocity);
    midiio_msgFinish_ISR(msg_index, 0);
}

static void sendNoteOff(uint8_t key)
{
    uint8_t msg_index = midiio_msgNew_ISR(MIDIMSG_SOURCE_GENERATED | MIDIMSG_FLAG_MSG_OK,
            MIDI_STATUS_NOTE_OFF | DelaySetup.Chan);
    midiio_msgAddData_ISR(msg_index, key);
    midiio_msgAddData_ISR(msg_index, NOTE_OFF_VELOCITY);
    midiio_msgFinish_ISR(msg_index, 0);
}



// ---- Public functions ----

void superdly_initialize(void)
{
    // Set setup defaults
    FilterEnabled = FALSE;
    DynamicEnable = TRUE;
    TapSpeedValid = FALSE;

    DelaySetup.Feedback = 110;
    DelaySetup.StopRepeat = 0;
    DelaySetup.StopVelocity = 4;
    DelaySetup.Speed = 50;
    DelaySetup.SwingAmount = 0;
    DelaySetup.TapKey = 0x1C;
    DelaySetup.Chan = 0;
    DelaySetup.Source = MIDIMSG_SOURCE_INPUT1;
    DelaySetup.EnableCc = INVALID_NUMBER;

    // Set delay state defaults
    initVoices();
}

void superdly_handleMidiMsg_ISR(midiMsg_t *msg)
{
    if (FilterEnabled)
    {
        if ((msg->Flags & (DelaySetup.Source | MIDIMSG_SOURCE_GENERATED)) != 0)
        {
            // TODO also check if message is OK

            uint8_t x = msg->MidiStatus;

            // Do we listen to this channel
            if ((x & MIDI_CHANNEL_MASK) == DelaySetup.Chan)
            {
                uint8_t voice;
                bool_t done_with_msg = FALSE;

                // Maybe this is tap speed?
                if (msg->Data[0] == DelaySetup.TapKey)
                {
                    switch (x & MIDI_STATUS_MASK)
                    {
                    case MIDI_STATUS_NOTE_ON:
                        // TODO GENERAL BUG ! this will not work with NOTE_OFF's using NOTE_ON at velocity 0
                        // This should be fixed on a general level, affects all filters

                        handleTapSpeedEvent(msg->ReceivedTick);

                    case MIDI_STATUS_NOTE_OFF:   // NOTE: Intentional fall through

                    case MIDI_STATUS_KEY_ATOUCH: // NOTE: Intentional fall through

                        // Discard all NoteOn/Off/Aftertouch messages regarding this key
                        msg->Flags |= MIDIMSG_FLAG_DISCARD;
                        done_with_msg = TRUE;
                        break;
                    }
                }

                if (done_with_msg == FALSE)
                {
                    // Deal with this message if status is something we are interested in
                    switch (x & MIDI_STATUS_MASK)
                    {
                    case MIDI_STATUS_NOTE_ON:
                        // Find a delayvoice for this note on
                        voice = findQuietestVoice();
                        // Make sure to generate note-off for the voice if required
                        killVoice(voice, msg->Data[0]);
                        // Setup voice according to this note on
                        handleNoteOn(voice, msg);
                        break;

                    case MIDI_STATUS_NOTE_OFF:
                        // Find the delayvoice that needs to get this note off
                        voice = FindActiveVoice(msg->Data[0]);
                        handleNoteOff(voice, msg);
                        break;

                    case MIDI_STATUS_CTRL_CHANGE:

                        // Are we listening to this controller?
                        if (msg->Data[0] == DelaySetup.EnableCc)
                        {
                            DynamicEnable = (msg->Data[1] > 0x40);
                            msg->Flags |= MIDIMSG_FLAG_DISCARD;
                        }
                        break;

                        //case MIDI_STATUS_KEY_ATOUCH:
                        //    // Find the delayvoice that needs to get this note off
                        //    voice = FindActiveVoice(msg->midi_data[0]);
                        //    AdjustVelocity(voice, msg->midi_data[1]);
                        //    break;
                    }
                }
            }
        }
    }

    // Profiling: Typical load about 33 us. Worst case about 87 us.
}


void superdly_handleTick_ISR(void)
{
    // Go through the active delays and check if there are delay events to do

    uint8_t i;
    uint16_t tick_now = hal_tickCountGet_ISR();

    handleTapSpeedTick(tick_now);

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        handleTick(i, tick_now);
    }

    // Profiling. Typical worst case 0.26 ms, may hit 0.3 ms
}


void superdly_handleMainLoop(void)
{
    // Maybe do maintenance to make sure we don't have stuck delays hanging around
}


uint8_t superdly_menuGetSubCount(void)
{
    return FilterEnabled ? MENU_SUB_ITEMS : 0;
}

static char *writeMenuLine(char *dest, uint8_t item)
{
    uint8_t *p;
    uint8_t data;

    // Write the menu line of text

    // First we must retrieve the data pointer from PROGMEM table:
    p = (uint8_t*) pgm_read_word( &(MenuItems[item].Data) );
    data = *p;

    // Special processing necessary?
    if (item == MENU_SPECIAL_CHAN)
    {
        data++;
    }

    // Write the formatted menu text
    return util_writeFormat_P(dest, MenuItems[item].Text, data);
}

void superdly_menuGetText(char *dest, uint8_t item)
{

    dest = writeMenuLine(dest, item);

    // Some items need special handling

    switch(item)
    {
    case MENU_IN_CHAN:
        // We are not done yet, write channel also:
        dest = writeMenuLine(dest, MENU_SPECIAL_CHAN);
        break;

    case MENU_SWING:
        // Only write 0 if swing is 0
        if (DelaySetup.SwingAmount == 0)
        {
            dest = util_copyString_P(dest - 2, PSTR("OFF"));
        }
        else
        {
            dest = util_copyString_P(dest, PSTR("0 ms"));
        }
        break;
    }

}

uint8_t superdly_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret;
    uint8_t menu_item;

    // Are we changing the edit_mode level?
    if (user_event == MENU_EVENT_SELECT)
    {
        edit_mode++;
    }
    else if (user_event == MENU_EVENT_BACK)
    {
        edit_mode--;
    }

    // Is current position valid, and what menu_item (in our system) does it represent?
    menu_item = INVALID_NUMBER;
    if (edit_mode == 1)
    {
        // OK
        menu_item = item;
    }
    else if (edit_mode == 2)
    {
        // Only possible at IN_CHAN combined menu
        if (item == MENU_IN_CHAN)
        {
            menu_item = MENU_SPECIAL_CHAN;
        }
    }


    // Set return value. If we can accept the edit mode, send cursor position:

    if (menu_item != INVALID_NUMBER)
    {
        ret = pgm_read_byte( &(MenuItems[menu_item].Cursor) );
    }
    else
    {
        ret = MENU_EDIT_MODE_UNAVAIL;
    }

    if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
    {
        // If we are turning pushed, multiply advance by 10
        if (user_event == MENU_EVENT_TURN_PUSH)
        {
            knob_delta *= 10;
        }

        // Adjust what must be adjusted
        switch (menu_item)
        {
        case MENU_TITLE:
            setFilterEnabled(knob_delta > 0);
            ret |= MENU_UPDATE_ALL;
            break;
        case MENU_IN_CHAN:
            DelaySetup.Source = util_boundedAddInt8(DelaySetup.Source, 1, 2, knob_delta);
            break;
        case MENU_SPEED:
            DelaySetup.Speed = util_boundedAddUint8(DelaySetup.Speed, MINIMUM_SPEED, 255, knob_delta);
            break;
        case MENU_SWING:
            DelaySetup.SwingAmount = util_boundedAddInt8(DelaySetup.SwingAmount, -100, 100, knob_delta);
            break;
        case MENU_FEEDBACK:
            DelaySetup.Feedback = util_boundedAddUint8(DelaySetup.Feedback, 1, 127, knob_delta);
            break;
        case MENU_STOP_VEL:
            DelaySetup.StopVelocity = util_boundedAddUint8(DelaySetup.StopVelocity, 1, 127, knob_delta);
            break;
        case MENU_STOP_REP:
            DelaySetup.StopRepeat = util_boundedAddUint8(DelaySetup.StopRepeat, 0, 100, knob_delta);
            break;
        case MENU_ENABLE_CC:
            DelaySetup.EnableCc = util_boundedAddInt8(DelaySetup.EnableCc, -1, 64, knob_delta);
            // If we are changing how to use enable / disable, better make sure we are enabled actually
            DynamicEnable = (DelaySetup.EnableCc == INVALID_NUMBER);
            break;
        case MENU_TAP_SPEED:
            DelaySetup.TapKey = util_boundedAddUint8(DelaySetup.TapKey, 0, 127, knob_delta);
            break;
        case MENU_SPECIAL_CHAN:
            DelaySetup.Chan = util_boundedAddInt8(DelaySetup.Chan, 0, 15, knob_delta);
            break;
        }
    }

    return ret;
}


uint8_t superdly_configGetSize(void)
{
    return 1 + sizeof(delaySetup_t);
}

void superdly_configSave(uint8_t *dest)
{
    *(dest++) = FilterEnabled;

    memcpy(dest, &(DelaySetup), sizeof(delaySetup_t));
}

void superdly_configLoad(uint8_t *dest)
{
    bool_t enable;

    enable = *(dest++);

    hal_interruptsDisable();
    memcpy(&(DelaySetup), dest, sizeof(delaySetup_t));
    hal_interruptsEnable();

    setFilterEnabled(enable);
}


