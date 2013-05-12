/*
 * blockfilter.c
 *
 *  Created on: Dec 22, 2012
 *      Author: lars
 */


/////////////////////////    INCLUDES    /////////////////////////


#include "common.h"
#include "blockfilter.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "util.h"
#include "menu.h"
#include "pgmstrings.h"
#include <string.h>

// The rules have the following structure:
//
// Input:   In1 In2 In*
// Map:     Chan1 - Chan16, P.Wheel, Chan.AT, Ctl:0 to Ctl:64
// To:      Chan1 - Chan16, P.Wheel, Chan.AT, Ctl:0 to Ctl:64, or NULL
//
// Map and Block (OFF)
// "In1 Chan2"
//   To block"
// "In:1 Ctl:4"
//   To Ctl:6"
// "In:1 Chan:1"
//   To Chan:2
//


// The user interface settings boils down to the following rule structure used for the filter:
//
// Rule structure:
//
// Selection of message:
//   Input filter:     1, 2, *
//   Channel filter:   1 - 16, *
//   Msg type filter:  Chan-A touch, Pich bend, Key atouch, Controller ..., *
//   Msg data filter:  Value, *
//
// What to do:
//   Action:           Discard, Change chan, Change CC, Chan.A.touch-To-Controller, Controller-To-Chan.A.touch
//   Action-data:      <depending on action>


/////////////////////////    Defines     /////////////////////////


// ---- The rule engine ----

// Note input is always filtered. Otherwise TYPE_GENERATED messages might be processed by
// the engine, which we don't want

#define MODE_OFF             0     // This rule is off

#define MODE_FILTER_CHAN     0x01  // Channel must equal filter_data1
#define MODE_FILTER_TYPE     0x02  // Midi message type must equal filter_data1
#define MODE_FILTER_DATA     0x04  // Midi message data1 must equal filter_data2

#define MODE_ACTION_MASK     0xF0
#define MODE_ACTION_DISCARD  0x10
#define MODE_ACTION_SET_CHAN 0x20  // Modify channel
#define MODE_ACTION_SET_CC   0x30  // Modify continuous controller type
#define MODE_ACTION_1_TO_CC  0x40  // Byte 1 of midi data to CC number action_data
//#define MODE_ACTION_1_TO_CAT 0x50  // Byte 1 of midi data to Channel-A.Touch
//#define MODE_ACTION_2_TO_CC  0x60  // Byte 2 of midi data to CC number action_data
#define MODE_ACTION_2_TO_CAT 0x70  // Byte 2 of midi data to Channel-A.Touch

typedef struct
{
    uint8_t Mode;

    uint8_t FilterInput;
    uint8_t FilterData1;
    uint8_t FilterData2;

    uint8_t ActionData;
} rule_t;

#define RULE_MAX 10



// ---- The user interface ----

// The user selects input 1, 2, *  then filter target is selected, which can be:

// Targets:
//   0  Discard (only valid as a input target)
#define TARGET_DISCARD      0

//   1  Chan 1
//   ...
//   16 Chan 16

#define TARGET_KEY_A_TOUCH  17
#define TARGET_CHAN_A_TOUCH 18
#define TARGET_PROG_CHANGE  19
#define TARGET_PITCH_WHEEL  20

static uint8_t targetStatusMap[4] PROGMEM =
{
        MIDI_STATUS_KEY_ATOUCH,
        MIDI_STATUS_CHAN_ATOUCH,
        MIDI_STATUS_PROG_CHANGE,
        MIDI_STATUS_PITCH_WHEEL
};

#define TARGET_FIRST_CC     21
//   21 CC:1
//   22 CC:2
//   ...

#define TARGET_LAST_CC      (TARGET_FIRST_CC + 63)

#define TARGET_MAX  TARGET_LAST_CC

typedef struct
{
    // The block filter is configured by these:
    uint8_t Input;
    uint8_t TargetIn;
    uint8_t TargetOut;

} ruleConfig_t;


static char TitleString[] PROGMEM = "Block and map";



/////////////////////////   Variables    /////////////////////////


static uint8_t RuleCount;

static ruleConfig_t RuleConfigs[RULE_MAX];
static rule_t       Rules[RULE_MAX];


/////////////////////////   Prototypes   /////////////////////////

static bool_t processRuleConfig(uint8_t index);

static char *writeTargetName(char *dest, uint8_t target);


///////////////////////// Implementation /////////////////////////

// Process the input and output configured by user. A corresponding rule is set up
// if possible and TRUE is returned. Otherwise FALSE is returned.
static bool_t processRuleConfig(uint8_t index)
{
    uint8_t mode = 0;
    uint8_t t_in = RuleConfigs[index].TargetIn;
    uint8_t t_out = RuleConfigs[index].TargetOut;
    bool_t rule_accepted = FALSE;

    Rules[index].Mode = MODE_OFF;
    Rules[index].FilterInput = RuleConfigs[index].Input;

    if (t_in == TARGET_DISCARD)
    {
        // We can't have "discard" as input
    }
    else if (t_in > TARGET_LAST_CC)
    {
        // Out of range
    }
    else if (t_in < 17)
    {
        // The input is a channel

        mode |= MODE_FILTER_CHAN;
        Rules[index].FilterData1 = t_in - 1;

        // Output can be discard or another channel

        if (t_out == TARGET_DISCARD)
        {
            mode |= MODE_ACTION_DISCARD;
            rule_accepted = TRUE;
        }
        else if (t_out < 17)
        {
            mode |= MODE_ACTION_SET_CHAN;
            Rules[index].ActionData = t_out - 1;
            rule_accepted = TRUE;
        }
    }
    else if ((t_in >= TARGET_FIRST_CC) &&
             (t_in <= TARGET_LAST_CC))
    {
        // Input is a continuous controller

        mode |= MODE_FILTER_TYPE | MODE_FILTER_DATA;
        Rules[index].FilterData1 = MIDI_STATUS_CTRL_CHANGE;
        Rules[index].FilterData2 = t_in - TARGET_FIRST_CC;

        // Output can be discard, another controller or chan.a.touch

        if (t_out == TARGET_DISCARD)
        {
            mode |= MODE_ACTION_DISCARD;
            rule_accepted = TRUE;
        }
        else if  ((t_out >= TARGET_FIRST_CC) &&
                  (t_out <= TARGET_LAST_CC))
        {
            mode |= MODE_ACTION_SET_CC;
            Rules[index].ActionData = t_out - TARGET_FIRST_CC;
            rule_accepted = TRUE;
        }
        else if (t_out == TARGET_CHAN_A_TOUCH)
        {
            // Convert from continuous controller to chan aftertouch,
            // this means byte 2 of midi message must be passed on:
            mode |= MODE_ACTION_2_TO_CAT;
            rule_accepted = TRUE;
        }
    }
    else if (t_in == TARGET_CHAN_A_TOUCH)
    {
        // Input is channel after touch

        mode |= MODE_FILTER_TYPE;
        Rules[index].FilterData1 = MIDI_STATUS_CHAN_ATOUCH;

        // Output can be discard or another controller

        if (t_out == TARGET_DISCARD)
        {
            mode |= MODE_ACTION_DISCARD;
            rule_accepted = TRUE;
        }
        else if  ((t_out >= TARGET_FIRST_CC) &&
                  (t_out <= TARGET_LAST_CC))
        {
            // Convert from chan aftertouch to continuous controller,
            // this means byte 1 of midi message must be passed on:
            mode |= MODE_ACTION_1_TO_CC;
            Rules[index].ActionData = t_out - TARGET_FIRST_CC;
            rule_accepted = TRUE;
        }
    }
    else
    {
        mode |= MODE_FILTER_TYPE;
        Rules[index].FilterData1 = pgm_read_byte( & (targetStatusMap[t_in - 17]));

        // For these target types, we only allow discard as output

        if (t_out == TARGET_DISCARD)
        {
            mode |= MODE_ACTION_DISCARD;
            rule_accepted = TRUE;
        }
    }

    if (rule_accepted)
    {
        Rules[index].Mode = mode;
    }
    else
    {
        Rules[index].Mode = MODE_OFF;
    }

    return rule_accepted;
}


static char *writeTargetName(char *dest, uint8_t target)
{
    if (target == TARGET_DISCARD)
    {
        dest = util_copyString_P(dest, PSTR("discard"));
    }
    else if (target < 17)
    {
        // A channel
        dest = util_writeFormat_P(dest, PSTR("Chan %i"), target);
    }
    else if (target < TARGET_FIRST_CC)
    {
        // A midi message type
        dest = midi_writeStatusName(dest, pgm_read_byte( & (targetStatusMap[target - 17])) );
    }
    else if (target <= TARGET_LAST_CC)
    {
        // A controller
        dest = util_copyString_P(dest, PSTR("CC: "));
        dest = midi_writeControllerName(dest, target - TARGET_FIRST_CC);
    }

    return dest;
}


void blockflt_initialize(void)
{
    uint8_t i;

    RuleCount = 0;

    // Set default setup in all slots
    for (i = 0; i < RULE_MAX; i++)
    {
        RuleConfigs[i].Input = 1;
        RuleConfigs[i].TargetIn = 1;
        RuleConfigs[i].TargetOut = 0;
        processRuleConfig(i);
    }

}

void blockflt_handleMidiMsg_ISR(midiMsg_t *msg)
{
    uint8_t midistatus = msg->MidiStatus;

    // TODO check if message is valid

    if ((midistatus & 0xF0) != 0xF0u)
    {
        // Its a channel something message, we only deal with this kind of message

        uint8_t i;
        bool_t discard = FALSE;

        for (i = 0; i < RuleCount; i++)
        {
            // Always check source

            if ((msg->Flags & Rules[i].FilterInput) != 0)
            {
                uint8_t mode = Rules[i].Mode;
                bool_t selected;

                if (mode & MODE_FILTER_CHAN)
                {
                    // We want a certain channel
                    selected = (midistatus & MIDI_CHANNEL_MASK) == Rules[i].FilterData1;
                }
                else
                {
                    selected = TRUE;

                    if (mode & MODE_FILTER_TYPE)
                    {
                        // We want a certain status type
                        selected = selected && ((midistatus & MIDI_STATUS_MASK) == Rules[i].FilterData1);
                    }

                    if (mode & MODE_FILTER_DATA)
                    {
                        // We want first data byte to match
                        selected = selected && (msg->Data[0] == Rules[i].FilterData2);
                    }
                }

                if (selected)
                {
                    // Selection filter suggests we deal with this message,
                    // do what we have to do:

                    switch (mode & MODE_ACTION_MASK)
                    {
                    case MODE_ACTION_DISCARD:
                        discard = TRUE;
                        break;

                    case MODE_ACTION_SET_CHAN:
                        //
                        midistatus = (midistatus & ~MIDI_CHANNEL_MASK) | Rules[i].ActionData;
                        msg->MidiStatus = midistatus;
                        break;

                    case MODE_ACTION_SET_CC:
                        // Change continuous controller type
                        msg->Data[0] = Rules[i].ActionData;
                        break;

                    case MODE_ACTION_1_TO_CC:
                        // Convert message to continuous controller
                        midistatus = (midistatus & ~MIDI_STATUS_MASK) | MIDI_STATUS_CTRL_CHANGE;
                        msg->MidiStatus = midistatus;
                        msg->Data[1] = msg->Data[0];
                        msg->Data[0] = Rules[i].ActionData;
                        msg->DataLen = 2;
                        break;

                    case MODE_ACTION_2_TO_CAT:
                        // Convert message to channel after touch
                        midistatus = (midistatus & ~MIDI_STATUS_MASK) | MIDI_STATUS_CHAN_ATOUCH;
                        msg->MidiStatus = midistatus;
                        msg->Data[0] = msg->Data[1];
                        msg->DataLen = 1;
                        break;
                    }
                }
            }

            if (discard)
            {
                msg->Flags |= MIDIMSG_FLAG_DISCARD;
                break;
            }

        } // END: for (i = 0; i < ruleCount; i++)
    }

    // Profiling: Mapping CC to CC may take 4 us.
    //            Channel block: worst case 2.6 us.
    //            Channel map:   worst case 3.5 us
}

void blockflt_handleTick_ISR(void)
{
    // TODO remove
}

void blockflt_handleMainLoop(void)
{
    // TODO remove
}

uint8_t blockflt_menuGetSubCount(void)
{
    return RuleCount * 2;
}

void blockflt_menuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        // Write first menu line
        util_copyString_P(dest, TitleString);
        util_writeNumberParentheses(dest + 14, RuleCount);
    }
    else
    {
        uint8_t c = (item - 1) >> 1;

        if ((item & 1) == 1)
        {
            // First line of a block filter, write: "InX ..."
            dest = pstr_writeInX(dest, RuleConfigs[c].Input);
            (*dest++) = ' ';

            // Write input target
            dest = writeTargetName(dest, RuleConfigs[c].TargetIn);
        }
        else
        {
            // Second line of a block filter, write " to "
            dest = util_copyString_P(dest, PSTR(" to "));

            // Write output target
            dest = writeTargetName(dest, RuleConfigs[c].TargetOut);

            // Notify user if this config is invalid
            if (Rules[c].Mode == MODE_OFF)
            {
                dest = util_copyString_P(dest, PSTR(" ERR!"));
            }
        }

    }
}

uint8_t blockflt_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    // Generally, we want the pushed turns to be x10
    if (user_event == MENU_EVENT_TURN_PUSH)
    {
        user_event = MENU_EVENT_TURN;
        knob_delta *= 10;
    }

    if (item == 0)
    {
        if (edit_mode == 0)
        {
            if (user_event == MENU_EVENT_SELECT)
            {
                // We are at root level and user wants to edit title menu

                // Ok, move cursor to ON / OFF
                ret = 17;
            }
        }
        else if (edit_mode == 1)
        {
            if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
            {
                // Modify filter count
                RuleCount = util_boundedAddInt8(RuleCount, 0, RULE_MAX, knob_delta);

                // Keep cursor at position, and notify menu that this may alter our submenu
                ret = 17 | MENU_UPDATE_ALL;
            }
        }
    }
    else
    {
        uint8_t c = (item - 1) >> 1;

        if ((item & 1) == 1)
        {
            // We are highlighting block filter input
            if (user_event == MENU_EVENT_SELECT)
            {
                if (edit_mode == 0)
                {
                    // User enters first edit mode
                    ret = 0;
                }
                else if (edit_mode == 1)
                {
                    // User enters second edit mode
                    ret = 4;
                }
            }
            else if (user_event == MENU_EVENT_TURN)
            {
                if (edit_mode == 1)
                {
                    ret = 0;
                    RuleConfigs[c].Input =
                            util_boundedAddInt8(RuleConfigs[c].Input, 1, 3, knob_delta);
                }
                else if (edit_mode == 2)
                {
                    ret = 4 | MENU_UPDATE_ALL;

                    RuleConfigs[c].TargetIn =
                            util_boundedAddInt8(RuleConfigs[c].TargetIn, 1, TARGET_MAX, knob_delta);

                }

                processRuleConfig(c);
            }
        }
        else
        {
            // We are highlighting block filter output line
            if (user_event == MENU_EVENT_SELECT)
            {
                if (edit_mode == 0)
                {
                    // User enters first edit mode
                    ret = 5;
                }
            }
            else if (user_event == MENU_EVENT_TURN)
            {
                if (edit_mode == 1)
                {
                    ret = 5;

                    RuleConfigs[c].TargetOut =
                            util_boundedAddInt8(RuleConfigs[c].TargetOut, 0, TARGET_MAX, knob_delta);

                    processRuleConfig(c);
                }
            }
        }
    }

    return ret;
}

// Configuration store and load implementation

uint8_t blockflt_configGetSize(void)
{
    return 1 + RULE_MAX * sizeof(ruleConfig_t);
}

void blockflt_configSave(uint8_t *dest)
{
    *(dest++) = RuleCount;

    memcpy(dest, &(RuleConfigs[0]), RULE_MAX * sizeof(ruleConfig_t));
}

void blockflt_configLoad(uint8_t *dest)
{
    uint8_t new_count;
    uint8_t i;

    RuleCount = 0;

    new_count = *(dest++);

    memcpy(&(RuleConfigs[0]), dest, RULE_MAX * sizeof(ruleConfig_t));

    for (i = 0; i < new_count; i++)
    {
        processRuleConfig(i);
    }

    RuleCount = new_count;

}

