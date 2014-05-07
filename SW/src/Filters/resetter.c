//
// Filename    : resetter.c
// Code module : Transmission of an array of MIDI controllers to reset equipment
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2014, Lars Ole Pontoppidan (Larsp)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// *  Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// *  Neither the name of the original author (Lars Ole Pontoppidan) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//
// ------------------------------  DESCRIPTION  ---------------------------------



//From http://www.midi.org/techspecs/rp15.php:
//--------------
//Upon receipt of Reset All Controllers message (Controller #121) the following actions are taken for the specified MIDI channel:
//
//    Set Expression (#11) to 127
//    Set Modulation (#1) to 0
//    Set Pedals (#64, #65, #66, #67) to 0
//    Set Registered and Non-registered parameter number LSB and MSB (#98-#101) to null value (127)
//    Set pitch bender to center (64/0)
//    Reset channel pressure to 0
//    Reset polyphonic pressure for all notes to 0.
//    Do NOT reset Bank Select (#0/#32)
//    Do NOT reset Volume (#7)
//    Do NOT reset Pan (#10)
//    Do NOT reset Program Change
//    Do NOT reset Effect Controllers (#91-#95)
//    Do NOT reset Sound Controllers (#70-#79)
//    Do NOT reset other channel mode messages (#120-#127)
//    Do NOT reset registered or non-registered parameters.
//
//

// |--------------------|
// | Resetter        1>1|
// |  When: On input    |
// |  Slot: 10 NotesOff | (cc 123)
// |  Send: 0           |
// |--------------------|
//
// Slots:
//   NotesOff
//   ResetAll


// -------------------------------  INCLUDES  -----------------------------------

#include "../midiprocessing.h"
#include "../common.h"
#include "../midimessage.h"
#include "../midigenerics.h"
#include "../errors.h"
#include "../util.h"
#include "../ui.h"
#include "../menus/filtermenu.h"
#include "resetter.h"
#include "../pgmstrings.h"
#include <avr/pgmspace.h>
#include "filteruids.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------


#define SEND_MIDI_CHAN     0


#define MODE_NO_AUTO       0
#define MODE_ON_NOW        1
#define MODE_ON_LOAD       2
#define MODE_CC_TRIGGER    3


#define SLOT_NOTES_OFF_MASK   1
#define SLOT_RESET_CTRL_MASK  2

#define SLOT_NOTES_OFF  0
#define SLOT_RESET_CTRL 1
#define SLOT_PGM_CHG    4

#define FIRST_FREE_SLOT 2
#define FREE_SLOTS      24

#define TOTAL_SLOTS (FREE_SLOTS + FIRST_FREE_SLOT)


static uint8_t const FreeSlotCcs[FREE_SLOTS] PROGMEM =
{
    0   , //    {"Bank.Sel"},  // 0
    32  , //    {"Bank.Sel LSB"},  // 32
    0xFF, //    special place holder for Program change
    7   , //    {"Volume"},    // 7
    8   , //    {"Balance"},   // 8
    10  , //    {"Pan"},       // 10
    12  , //    {"Eff.Ctl.1"}, // 12
    13  , //    {"Eff.Ctl.2"}, // 13
    70  , //    {"Variation"}, // 70
    71  , //    {"Intensity"}, // 71
    72  , //    {"Rel.Time"},  // 72
    73  , //    {"Att.Time"},  // 73
    74  , //    {"Brightns."}, // 74
    75  , //    {"DecayTime"}, // 75
    76  , //    {"VibRate"},   // 76
    77  , //    {"VibDepth"},  // 77
    78  , //    {"VibDelay"},  // 78
    79  , //    {"Undef.79"},  // 79
    84  , //    {"PortaCtl"},  // 84
    91  , //    {"Reverb"},    // 91
    92  , //    {"Tremolo"},   // 92
    93  , //    {"Chorus"},    // 93
    94  , //    {"DetuDepth"}, // 94
    95  , //    {"PhasDepth"}, // 95
};

typedef struct
{
    uint8_t Mode;
    uint8_t FixedSlots;
    uint8_t FreeSlots[FREE_SLOTS];
} config_t;

#define DONT_SEND 0xFF
#define DO_SEND   0xFE

#define INSTANCE_MAX 2

typedef struct
{
    bool_t  InUse;
    uint8_t FilterStep;
    bool_t  CcTriggered;
    uint8_t TransmissionStatus;
    uint8_t EditNow;
} instance_t;

#define TRANSMISSION_IDLE       0xFF
#define TRANSMISSION_START      0

static char const MenuTitle[]    PROGMEM = "Resetter";

static char const MenuSetting1[] PROGMEM = "Send: ";
static char const MenuSetting2[] PROGMEM = "Slot: %i ";
static char const MenuSetting3[] PROGMEM = "Val.: ";


// ------------------------------  PROTOTYPES  ----------------------------------

// Filter interface

static uint8_t resetter_Create(uint8_t filter_step);
static void    resetter_Destroy(uint8_t instance);
static void    resetter_SetFilterStep(uint8_t instance, uint8_t filter_step);
static void    resetter_LoadConfig(uint8_t instance, void* data);
static void    resetter_SaveConfig(uint8_t instance, void* data);
static uint8_t resetter_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg);
static void    resetter_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest);
static void    resetter_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event);

// Private functions

static void    resetConfig(config_t *cfg);
static bool_t  getMsgValue(uint8_t instance, uint8_t slot);
static void    makeMsg(uint8_t slot, uint8_t value, midiMsg_t *msg);

static void sendSlotMsg_MAIN(uint8_t instance, uint8_t slot);


// -------------------------------  VARIABLES  ----------------------------------


const filterInterface_t resetter_Filter PROGMEM =
{
        resetter_Create,
        resetter_Destroy,
        resetter_SetFilterStep,
        resetter_LoadConfig,
        resetter_SaveConfig,
        resetter_ProcessMidiMsg,
        resetter_WriteMenuText,
        resetter_HandleUiEvent,

        sizeof(config_t),   // Number of bytes in configuration
        3,                  // Number of menu items (0 means only title, 1 is one item)
        6,                  // Cursor indentation in menu
        FILTER_MODE_IN_OUT, // Filter operation mode
        MenuTitle,          // Static filter title (this may be different from in-menu title)

        FILTER_UID(FILTER_ID_RESETTER_FILTER, 1)
};

static instance_t  Instance[INSTANCE_MAX];
static config_t    Config[INSTANCE_MAX];


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void resetter_handleTick_ISR(void)
{
    uint8_t i;

    for (i = 0; i < INSTANCE_MAX; i++)
    {
        if (Instance[i].InUse)
        {
            uint8_t send = DONT_SEND;

            while ((send == DONT_SEND) && (Instance[i].TransmissionStatus < TOTAL_SLOTS))
            {
                send = getMsgValue(i, Instance[i].TransmissionStatus);

                if (send != DONT_SEND)
                {
                    midiMsg_t msg;
                    makeMsg(Instance[i].TransmissionStatus, send, &msg);
                    midiproc_addMessage_ISR(&msg, Instance[i].FilterStep);
                }

                Instance[i].TransmissionStatus++;
            }
        }
    }
}


// ------------------------  FILTER INTERFACE FUNCTIONS  ------------------------

static uint8_t resetter_Create(uint8_t filter_step)
{
    uint8_t ret = FILTER_CREATE_FAILED;
    uint8_t i;

    for (i = 0; i < INSTANCE_MAX; i++)
    {
        if (Instance[i].InUse == FALSE)
        {
            // Reset config
            resetConfig(&(Config[i]));

            // Setup instance
            Instance[i].FilterStep = filter_step;
            Instance[i].InUse = TRUE;
            Instance[i].CcTriggered = FALSE;
            Instance[i].TransmissionStatus = TRANSMISSION_IDLE;
            Instance[i].EditNow = 0;

            // Return instance number
            ret = i;
            break;
        }
    }

    return ret;
}

static void resetter_Destroy(uint8_t instance)
{
    Instance[instance].InUse = FALSE;
}

static void resetter_SetFilterStep(uint8_t instance, uint8_t filter_step)
{
    if (Instance[instance].InUse)
    {
        Instance[instance].FilterStep = filter_step;
    }
}

static void resetter_LoadConfig(uint8_t instance, void* data)
{
    Config[instance] = *((config_t*)data);
}

static void resetter_SaveConfig(uint8_t instance, void* data)
{
    *((config_t*)data) = Config[instance];
}

static uint8_t resetter_ProcessMidiMsg(uint8_t instance, midiMsg_t *msg)
{
    if (Config[instance].Mode == MODE_CC_TRIGGER)
    {
        if ((msg->Data[MIDIMSG_STATUS] & MIDI_STATUS_MASK) == MIDI_STATUS_CTRL_CHANGE)
        {
            // We treat the CC value with a simple schmitt trigger logic:
            if (Instance[instance].CcTriggered)
            {
                if (msg->Data[MIDIMSG_DATA2] < 64)
                {
                    // Transition from above 64 to below 64
                    Instance[instance].CcTriggered = FALSE;
                }
            }
            else
            {
                if (msg->Data[MIDIMSG_DATA2] > 64)
                {
                    // Transition from below 64 to above 64, this is our signal!
                    Instance[instance].TransmissionStatus = TRANSMISSION_START;
                    Instance[instance].CcTriggered = TRUE;
                }
            }
        }
    }

    return FILTER_PROCESS_NEUTRAL;
}


// Menu integration
static void resetter_WriteMenuText(uint8_t instance, uint8_t menu_item, void *dest)
{
    uint8_t x;

    switch (menu_item)
    {
    case 0:
        util_copyString_P(dest, MenuTitle);
        break;

    case 1:
        dest = util_copyString_P(dest, MenuSetting1);
        switch (Config[instance].Mode)
        {
        case MODE_NO_AUTO:
            util_copyString_P(dest, PSTR("Never"));
            break;
        case MODE_ON_NOW:
            util_copyString_P(dest, PSTR("Now"));
            break;
        case MODE_ON_LOAD:
            util_copyString_P(dest, PSTR("On load"));
            break;
        case MODE_CC_TRIGGER:
            util_copyString_P(dest, PSTR("CC Trigger"));
            break;
        }
        break;

    case 2:
        x = Instance[instance].EditNow;
        dest = util_writeFormat_P(dest, MenuSetting2, x + 1);

        if (x == SLOT_NOTES_OFF)
        {
            util_copyString_P(dest, PSTR("NotesOff"));
        }
        else if (x == SLOT_RESET_CTRL)
        {
            util_copyString_P(dest, PSTR("ResetCtrl"));
        }
        else if (x == SLOT_PGM_CHG)
        {
            util_copyString_P(dest, PSTR("ProgChg"));
        }
        else
        {
            midi_writeControllerName(dest, pgm_read_byte(&(FreeSlotCcs[x - FIRST_FREE_SLOT])) );
        }
        break;

    case 3:
        dest = util_copyString_P(dest, MenuSetting3);

        x = getMsgValue(instance, Instance[instance].EditNow);

        if (x == DONT_SEND)
        {
            util_copyString_P(dest, PSTR("Don't send"));
        }
        else if (x == DO_SEND)
        {
            util_copyString_P(dest, PSTR("Send"));
        }
        else
        {
            if (Instance[instance].EditNow == SLOT_PGM_CHG)
            {
                // Always write prog change values 1 based
                x++;
            }

            util_writeUint8(dest, x);
        }
        break;
    }
}

static void resetter_HandleUiEvent(uint8_t instance, uint8_t menu_item, uint8_t ui_event)
{
    int8_t delta;
    uint8_t x;
    filters_instance_t fi;
    fi.FilterType = FILTER_TYPE_RESETTER_FILTER;
    fi.Instance = instance;

    delta = ui_eventToDelta(ui_event, 10);

    switch (menu_item)
    {
    case 1:
        Config[instance].Mode = util_boundedAddUint8(Config[instance].Mode, 0, 3, delta);

        if (Config[instance].Mode == MODE_ON_NOW)
        {
            // Start transmission now! TODO this is not very intuitive..
            Instance[instance].TransmissionStatus = TRANSMISSION_START;
        }
        break;

    case 2:
        Instance[instance].EditNow = util_boundedAddUint8(Instance[instance].EditNow, 0, TOTAL_SLOTS - 1, delta);
        filtermenu_RequestUpdate(fi, 3);
        break;

    case 3:
        x = Instance[instance].EditNow;
        if (x == SLOT_NOTES_OFF)
        {
            Config[instance].FixedSlots = ui_eventToSetClearBit(ui_event,
                    Config[instance].FixedSlots, SLOT_NOTES_OFF_MASK);
        }
        else if (x == SLOT_RESET_CTRL)
        {
            Config[instance].FixedSlots = ui_eventToSetClearBit(ui_event,
                    Config[instance].FixedSlots, SLOT_RESET_CTRL_MASK);
        }
        else
        {
            Config[instance].FreeSlots[x - FIRST_FREE_SLOT] = util_boundedAddInt8(
                    Config[instance].FreeSlots[x - FIRST_FREE_SLOT], (int8_t)DONT_SEND, 127, delta);
        }

        sendSlotMsg_MAIN(instance, x);
        break;
    }
}


// ---------------------------  PRIVATE FUNCTIONS  -----------------------------


static void resetConfig(config_t *cfg)
{
    // Set defaults
    cfg->Mode = MODE_NO_AUTO;
    cfg->FixedSlots = 0;

    for (uint8_t i = 0; i < FREE_SLOTS; i++)
    {
        cfg->FreeSlots[i] = DONT_SEND;
    }
}


static void sendSlotMsg_MAIN(uint8_t instance, uint8_t slot)
{
    uint8_t value = getMsgValue(instance, slot);

    if (value != DONT_SEND)
    {
        midiMsg_t msg;
        makeMsg(slot, value, &msg);
        midiproc_addMessage_MAIN(&msg, Instance[instance].FilterStep);
    }
}

static void makeMsg(uint8_t slot, uint8_t value, midiMsg_t *msg)
{
    if (slot == SLOT_NOTES_OFF)
    {
        midimsg_newContinuousCtrl(msg, SEND_MIDI_CHAN, MIDI_CTRL_NOTES_OFF, 0);
    }
    else if (slot == SLOT_RESET_CTRL)
    {
        midimsg_newContinuousCtrl(msg, SEND_MIDI_CHAN, MIDI_CTRL_RESET_CTRL, 0);
    }
    else if (slot == SLOT_PGM_CHG)
    {
        midimsg_newProgramChange(msg, SEND_MIDI_CHAN, value);
    }
    else if (slot < TOTAL_SLOTS)
    {
        midimsg_newContinuousCtrl(msg, SEND_MIDI_CHAN,  pgm_read_byte(&(FreeSlotCcs[slot - FIRST_FREE_SLOT])), value);
    }
}

static bool_t getMsgValue(uint8_t instance, uint8_t slot)
{
    bool_t ret = DONT_SEND;

    if (slot == SLOT_NOTES_OFF)
    {
        if (Config[instance].FixedSlots & SLOT_NOTES_OFF_MASK)
        {
            ret = DO_SEND;
        }
    }
    else if (slot == SLOT_RESET_CTRL)
    {
        if (Config[instance].FixedSlots & SLOT_RESET_CTRL_MASK)
        {
            ret = DO_SEND;
        }
    }
    else if (slot < TOTAL_SLOTS)
    {
        ret = Config[instance].FreeSlots[slot - FIRST_FREE_SLOT];
    }

    return ret;
}

