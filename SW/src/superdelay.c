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

#define OVERLAP_TIME_DIFF     6      // x10 ms

#define MINIMUM_SPEED         8      // x10 ms

#define INVALID_NUMBER        0xFF

typedef struct
{
    uint8_t source;
    uint8_t chan;
    uint8_t tapKey;        // TODO Tapping this key will set speed
    uint8_t speed;         // Delay speed in tick count
    int8_t  swing_amount;  // Added to speed of 1 3 5... beats, subtracted from 2 4 6... beats
    uint8_t feedback;      // 128 is 100%, 64 is 50%, 255 is 200%
    uint8_t stopVelocity;  // Velocity to stop repeating at
    uint8_t stopRepeat;    // 0 means unlimited. Repeat count limit
    uint8_t enableCc;      // Midi controller to enable / disable new delay voices
} delaySetup_t;


#define STATUS_FREE           0x00 // Delay inactive
#define STATUS_GOT_NOTE_ON    0x01 // We have received note on
#define STATUS_GOT_NOTE_OFF   0x02 // We have received note off
#define STATUS_IS_NOTE_ON     0x04 // Currently playing a note
#define STATUS_ARM_NOTE_ON    0x08 // Next NoteOn event is armed
#define STATUS_ARM_NOTE_OFF   0x10 // Next NoteOff event is armed
#define STATUS_DELAY_DONE     0x20 // Next NoteOff event stops this delay voice
#define STATUS_PROLONGED      0x40 // This delay cycle uses the speed + swing_amount (instead of - )


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
    char text[19];
    uint8_t cursor;
    void *data;
} menuItem_t;

#define MENU_ITEMS 10
#define MENU_SPECIAL_CHAN 9


/////////////////////////   Variables    /////////////////////////

static delayVoice_t DelayVoices[DELAY_VOICES_MAX];
static delaySetup_t DelaySetup;

static bool_t filterEnabled;

static uint16_t TapSpeedTick;
static bool_t TapSpeedValid;

static bool_t dynamicEnable;


menuItem_t MenuItems[MENU_ITEMS] PROGMEM =
{
    {"Midi delay    %O",  17, &(filterEnabled) },
    {"Input: %i",          7, &(DelaySetup.source) },
    {"Del.Time: %U0 ms",  13, &(DelaySetup.speed) },
    {"Swing   :%I",       13, &(DelaySetup.swing_amount) },
    {"Feedback:  %U",     13, &(DelaySetup.feedback) },
    {"Stop Vel:  %U",     13, &(DelaySetup.stopVelocity) },
    {"Stop Rep:  %U",     13, &(DelaySetup.stopRepeat) },
    {"EnableCC:%c",        9, &(DelaySetup.enableCc) },
    {"TapSpeed: %n",      10, &(DelaySetup.tapKey) },
    {" Chan: %i",         15, &(DelaySetup.chan) },
};


/////////////////////////   Prototypes   /////////////////////////


// ---- DelayVoice Management ----

// Set all voices to reset state
static void InitVoices(void);

static void SetFilterEnabled(bool_t enable);  // TODO mark as main thread!

static bool_t CheckIfMasked(uint8_t voice);

// Go through the delay voices and return the one which is the least active
static uint8_t FindQuietestVoice(void);

// Go through delay voices and check if any of them is holding key
static uint8_t CheckNoteActiveState(uint8_t key);

// ---- DelayVoice Events ----

static void SetupNextDelayCycle(uint8_t voice); // Private

// Sets up delay voice according to note message
static void HandleNoteOn(uint8_t voice, mmsg_t *msg);

// Does what have to be done at this tick
static void HandleTick(uint8_t voice, uint16_t tick);

// Registers note off event
static void HandleNoteOff(uint8_t voice, mmsg_t *msg);

// May generate NoteOff
static void KillVoice(uint8_t voice, uint8_t dont_touch_this_key);

// ---- TapSpeed functionality

static void HandleTapSpeedEvent(uint16_t tick);
static void HandleTapSpeedTick(uint16_t tick);

// ---- MidiMessage sending ----

static void SendNoteOn(uint8_t key, uint8_t velocity);
static void SendNoteOff(uint8_t key);

// ---- MENU implementation ----

static char *WriteMenuLine(char *dest, uint8_t item);

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

// Main thread function
static void SetFilterEnabled(bool_t enable)
{
    if (enable)
    {
        InitVoices();
        TapSpeedValid = FALSE;
        dynamicEnable = (DelaySetup.enableCc == -1);
    }
    else
    {
        if (filterEnabled)
        {
            uint8_t i;

            // Filter is enabled right now. We must make sure active voices send a note off
            hal_InterruptsDisable();

            for (i = 0; i < DELAY_VOICES_MAX; i++)
            {
                if (DelayVoices[i].status != STATUS_FREE)
                {
                    DelayVoices[i].status |= STATUS_DELAY_DONE;
                }
            }

            hal_InterruptsEnable();
        }
    }


    filterEnabled = enable;
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

                    if (DelayVoices[i].status & STATUS_PROLONGED)
                    {
                        if ((time_diff < OVERLAP_TIME_DIFF) ||
                             (((uint16_t)(DelaySetup.speed + DelaySetup.swing_amount) - time_diff) < OVERLAP_TIME_DIFF) )
                        {
                            masked = TRUE;
                        }
                    }
                    else
                    {
                        if ((time_diff < OVERLAP_TIME_DIFF) ||
                             (((uint16_t)(DelaySetup.speed - DelaySetup.swing_amount) - time_diff) < OVERLAP_TIME_DIFF) )
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

// Find the voice which has registered note on, but not note off, for key
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


// Go through delay voices and check if any of them is holding key
static uint8_t CheckNoteActiveState(uint8_t key)
{
    uint8_t key_active = 0;
    uint8_t i;

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        if (DelayVoices[i].key == key)
        {
            if (DelayVoices[i].status & STATUS_IS_NOTE_ON)
            {
                // A delay voice is generating a note
                key_active = DelayVoices[i].velocity;
            }
            else if ((DelayVoices[i].status & (STATUS_GOT_NOTE_ON | STATUS_GOT_NOTE_OFF))
                    == STATUS_GOT_NOTE_ON)
            {
                // We have got note on, but not note off. It means user is holding key.
                key_active = DelayVoices[i].velocity;
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

    // Toggle prolonged state
    DelayVoices[index].status ^= STATUS_PROLONGED;

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
        uint8_t speed;

        if (DelayVoices[index].status & STATUS_PROLONGED)
        {
            speed = DelaySetup.speed + DelaySetup.swing_amount;
        }
        else
        {
            speed = DelaySetup.speed - DelaySetup.swing_amount;
        }

        DelayVoices[index].nextNoteOnTime = this_event_time + (uint16_t)(speed);

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
    DelayVoices[index].key = msg->midi_data[0];
    DelayVoices[index].keyOnDuration = msg->receive_tick;
    DelayVoices[index].nextNoteOnTime = msg->receive_tick;
    DelayVoices[index].repeats = 0;
    DelayVoices[index].velocity = msg->midi_data[1];
    DelayVoices[index].status = STATUS_GOT_NOTE_ON;

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

        if (key != dont_touch_this_key)
        {
            if (CheckNoteActiveState(key) == FALSE)
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
                    uint8_t velocity;

                    // We must generate a note on
                    velocity = CheckNoteActiveState(DelayVoices[voice].key);

                    if (velocity != 0u)
                    {
                        SendNoteOff(DelayVoices[voice].key);
                    }

                    // Use the highest velocity of our delayvoice and the note
                    // we may just have silenced:
                    if (DelayVoices[voice].velocity > velocity)
                    {
                        velocity = DelayVoices[voice].velocity;
                    }

                    DelayVoices[voice].status = status | STATUS_IS_NOTE_ON;

                    SendNoteOn(DelayVoices[voice].key, velocity);
                }

                SetupNextDelayCycle(voice);
            }
        }
    }
}

// ---- Tap Speed handling ----

static void HandleTapSpeedEvent(uint16_t tick)
{
    if (TapSpeedValid)
    {
        uint16_t speedProposal;
        uint8_t swing;

        speedProposal = tick - TapSpeedTick;
        swing = DelaySetup.swing_amount;

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
            DelaySetup.speed = (uint8_t)speedProposal;

            menu_NotifyRefresh_SAFE();
        }

    }

    // In any case, we got a new valid speed tapping input
    TapSpeedTick = tick;
    TapSpeedValid = TRUE;
}

static void HandleTapSpeedTick(uint16_t tick)
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
    dynamicEnable = TRUE;
    TapSpeedValid = FALSE;

    DelaySetup.feedback = 110;
    DelaySetup.stopRepeat = 0;
    DelaySetup.stopVelocity = 4;
    DelaySetup.speed = 50;
    DelaySetup.swing_amount = 0;
    DelaySetup.tapKey = 0x1C;
    DelaySetup.chan = 0;
    DelaySetup.source = MMSG_SOURCE_INPUT1;
    DelaySetup.enableCc = -1;

    // Set delay state defaults
    InitVoices();
}

void sdelay_HookMidiMsg_ISR(mmsg_t *msg)
{
    if (filterEnabled)
    {
        if ((msg->flags & DelaySetup.source) != 0)
        {
            // TODO also check if message is OK

            uint8_t x = msg->midi_status;

            // Do we listen to this channel
            if ((x & MIDI_CHANNEL_MASK) == DelaySetup.chan)
            {
                uint8_t voice;
                bool_t done_with_msg = FALSE;

                // Maybe this is tap speed?
                if (msg->midi_data[0] == DelaySetup.tapKey)
                {
                    switch (x & MIDI_STATUS_MASK)
                    {
                    case MIDI_STATUS_NOTE_ON:
                        // TODO GENERAL BUG ! this will not work with NOTE_OFF's using NOTE_ON at velocity 0
                        // This should be fixed on a general level, affects all filters

                        HandleTapSpeedEvent(msg->receive_tick);

                    case MIDI_STATUS_NOTE_OFF:   // NOTE: Intentional fall through

                    case MIDI_STATUS_KEY_ATOUCH: // NOTE: Intentional fall through

                        // Discard all NoteOn/Off/Aftertouch messages regarding this key
                        msg->flags |= MMSG_FLAG_DISCARD;
                        done_with_msg = TRUE;
                    }
                }

                if (done_with_msg == FALSE)
                {
                    // Deal with this message if status is something we are interested in
                    switch (x & MIDI_STATUS_MASK)
                    {
                    case MIDI_STATUS_NOTE_ON:
                        if (dynamicEnable)
                        {
                            // Find a delayvoice for this note on
                            voice = FindQuietestVoice();
                            // Make sure to generate note-off for the voice if required
                            KillVoice(voice, msg->midi_data[0]);
                            // Setup voice according to this note on
                            HandleNoteOn(voice, msg);
                        }
                        break;

                    case MIDI_STATUS_NOTE_OFF:
                        // Find the delayvoice that needs to get this note off
                        voice = FindActiveVoice(msg->midi_data[0]);
                        HandleNoteOff(voice, msg);
                        break;

                    case MIDI_STATUS_CTRL_CHANGE:

                        // Are we listening to this controller?
                        if (msg->midi_data[0] == DelaySetup.enableCc)
                        {
                            dynamicEnable = (msg->midi_data[1] > 0x40);
                            msg->flags |= MMSG_FLAG_DISCARD;
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
}


void sdelay_HookTick_ISR(void)
{
    // Go through the active delays and check if there are delay events to do

    uint8_t i;
    uint16_t tickNow = hal_TickCountGet_ISR();

    HandleTapSpeedTick(tickNow);

    for (i = 0; i < DELAY_VOICES_MAX; i++)
    {
        HandleTick(i, tickNow);
    }
}


void sdelay_HookMainLoop(void)
{
    // Maybe do maintenance to make sure we don't have stuck delays hanging around
}


uint8_t sdelay_MenuGetSubCount(void)
{
    return filterEnabled ? MENU_SUB_ITEMS : 0;
}

static char *WriteMenuLine(char *dest, uint8_t item)
{
    uint8_t *p;
    uint8_t data;

    // Write the menu line of text

    // First we must retrieve the data pointer from PROGMEM table:
    p = (uint8_t*) pgm_read_word( &(MenuItems[item].data) );
    data = *p;

    // Special processing necessary?
    if (item == MENU_SPECIAL_CHAN)
    {
        data++;
    }

    // Write the formatted menu text
    return util_strWriteFormat_P(dest, MenuItems[item].text, data);
}

void sdelay_MenuGetText(char *dest, uint8_t item)
{

    dest = WriteMenuLine(dest, item);

    // Some items need special handling

    switch(item)
    {
    case MENU_IN_CHAN:
        // We are not done yet, write channel also:
        dest = WriteMenuLine(dest, MENU_SPECIAL_CHAN);
        break;

    case MENU_SWING:
        // Only write 0 if swing is 0
        if (DelaySetup.swing_amount == 0)
        {
            dest = util_strCpy_P(dest - 2, PSTR("OFF"));
        }
        else
        {
            dest = util_strCpy_P(dest, PSTR("0 ms"));
        }
        break;
    }

}

uint8_t sdelay_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
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
        ret = pgm_read_byte( &(MenuItems[menu_item].cursor) );
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
            SetFilterEnabled(knob_delta > 0);
            ret |= MENU_UPDATE_ALL;
            break;
        case MENU_IN_CHAN:
            DelaySetup.source = util_boundedAddInt8(DelaySetup.source, 1, 2, knob_delta);
            break;
        case MENU_SPEED:
            DelaySetup.speed = util_boundedAddUint8(DelaySetup.speed, MINIMUM_SPEED, 255, knob_delta);
            break;
        case MENU_SWING:
            DelaySetup.swing_amount = util_boundedAddInt8(DelaySetup.swing_amount, -100, 100, knob_delta);
            break;
        case MENU_FEEDBACK:
            DelaySetup.feedback = util_boundedAddUint8(DelaySetup.feedback, 1, 127, knob_delta);
            break;
        case MENU_STOP_VEL:
            DelaySetup.stopVelocity = util_boundedAddUint8(DelaySetup.stopVelocity, 1, 127, knob_delta);
            break;
        case MENU_STOP_REP:
            DelaySetup.stopRepeat = util_boundedAddUint8(DelaySetup.stopRepeat, 0, 100, knob_delta);
            break;
        case MENU_ENABLE_CC:
            DelaySetup.enableCc = util_boundedAddInt8(DelaySetup.enableCc, -1, 64, knob_delta);
            // If we are changing how to use enable / disable, better make sure we are enabled actually
            dynamicEnable = (DelaySetup.enableCc == -1);
            break;
        case MENU_TAP_SPEED:
            DelaySetup.tapKey = util_boundedAddUint8(DelaySetup.tapKey, 0, 127, knob_delta);
            break;
        case MENU_SPECIAL_CHAN:
            DelaySetup.chan = util_boundedAddInt8(DelaySetup.chan, 0, 15, knob_delta);
            break;
        }
    }

    return ret;
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
    bool_t enable;

    enable = *(dest++);

    hal_InterruptsDisable();
    memcpy(&(DelaySetup), dest, sizeof(delaySetup_t));
    hal_InterruptsEnable();

    SetFilterEnabled(enable);
}


