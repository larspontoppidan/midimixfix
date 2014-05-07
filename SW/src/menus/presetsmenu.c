
//
// Filename    : presetsmenu.c
// Code module : Preset load / save menu implementation implementation
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2013, Lars Ole Pontoppidan (Larsp)
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


// -------------------------------  INCLUDES  -----------------------------------

#include "../common.h"
#include "../errors.h"
#include "../util.h"
#include "../ui.h"
#include "../menuinterface.h"
#include "../midiprocessing.h"
#include "../presetstorage.h"
#include "msgscreen.h"
#include <avr/pgmspace.h>


// ------------------------------  PROTOTYPES  ----------------------------------


static uint8_t saveInitGetCursor(void);
static uint8_t loadInitGetCursor(void);
static uint8_t getItemCount(void);
static void    writeItem(uint8_t item, void *dest);
static void    handleUiEvent(uint8_t uiEvent);

static void handleMoveEvent(uint8_t uiEvent);


// --------------------------  TYPES AND CONSTANTS  -----------------------------

const menuInterface_t PROGMEM presetsmenu_LoadMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        loadInitGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

const menuInterface_t PROGMEM presetsmenu_SaveMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        saveInitGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

static const char PresetLoadTitle[] PROGMEM = "--- LOAD Preset: ---";
static const char PresetSaveTitle[] PROGMEM = "--- SAVE Preset: ---";

static const char PresetItem[]     PROGMEM = "Preset %i";
static const char PresetDefaults[] PROGMEM = "Defaults";

// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t cursorItem = 1;
static bool_t  saveNotLoad;


// ---------------------------  PRIVATE FUNCTIONS  -------------------------------

static void handleMoveEvent(uint8_t uiEvent)
{
    uint8_t max = saveNotLoad ? (PRESETS_SLOTS) : (PRESETS_SLOTS + 1);

    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (cursorItem < max)
        {
            cursorItem++;
        }
        break;
    case UI_EVENT_MOVE_DOWN:
        if (cursorItem > 1)
        {
            cursorItem--;
        }
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        cursorItem = max;
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        cursorItem = 1;
        break;
    }
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


static uint8_t saveInitGetCursor(void)
{
    saveNotLoad = TRUE;

    if (cursorItem > PRESETS_SLOTS)
    {
        // When loading we can select one extra "slot", the defaults
        cursorItem = 1;
    }

    return cursorItem;
}

static uint8_t loadInitGetCursor(void)
{
    saveNotLoad = FALSE;
    return cursorItem;
}

static uint8_t getItemCount(void)
{
    // The menu title also counts, and if loading we also have "Defaults"

    return saveNotLoad ? (PRESETS_SLOTS + 1) : (PRESETS_SLOTS + 2);
}

static void writeItem(uint8_t item, void *dest)
{
    if (item == 0)
    {
        util_copyString_P(dest, saveNotLoad ? PresetSaveTitle : PresetLoadTitle);
    }
    else if (item <= PRESETS_SLOTS)
    {
        uint8_t r;

        if (item == 1)
        {
            dest = util_copyString_P(dest, PSTR("Start up"));
        }
        else
        {
            dest = util_writeFormat_P(dest, PresetItem, item);
        }

        r =  presets_load(item - 1, TRUE);
        switch (r)
        {
        case PRESET_OK:
            break;
        case PRESET_EMPTY:
            util_copyString_P(dest, PSTR(" (empty)"));
            break;
        case PRESET_CHKSUM_ERROR:
            util_writeFormat_P(dest, PSTR(" CORRUPT"), r);
            break;
        default:
            util_writeFormat_P(dest, PSTR(" ERR: %i"), r);
            break;
        }
    }
    else
    {
        util_copyString_P(dest, PresetDefaults);
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_BACK)
    {
        ui_menuBackOut();
    }
    else if (uiEvent == UI_EVENT_SELECT)
    {
        // Selecting stuff in main menu:
        if (saveNotLoad)
        {
            // Saving a preset
            uint8_t r = presets_save(cursorItem - 1);

            ui_menuBackOut();

            if (r == PRESET_OK)
            {
                msgscreen_Show_P(PSTR("PRESET SAVED"), 3);
            }
            else
            {
                // NOTE assumes only one possible error: PRESET_SAVE_TOO_BIG
                msgscreen_Show_P(PSTR("SAVE ERR: TOO BIG!"), 1);
            }
        }
        else if (cursorItem == (PRESETS_SLOTS + 1))
        {
            // Load defaults
            midiproc_stop_MAIN();
            midiproc_loadDefaultFilters();
            midiproc_start_MAIN();

            ui_menuBackOut();
            msgscreen_Show_P(PSTR("DEFAULTS LOADED"), 2);
        }
        else
        {
            // First we test
            uint8_t r = presets_load(cursorItem - 1, TRUE);

            if ((r != PRESET_EMPTY) && (r != PRESET_CHKSUM_ERROR))
            {
                // OK, lets load
                midiproc_stop_MAIN();
                r = presets_load(cursorItem - 1, FALSE);
                midiproc_start_MAIN();

                ui_menuBackOut();

                if (r == PRESET_OK)
                {
                    msgscreen_Show_P(PSTR("PRESET LOADED"), 3);
                }
                else
                {
                    msgscreen_Show_FormatP(PSTR("PRESET LOAD ERR:%i"), r, 1);
                }
            }
        }
    }
    else
    {
        // Moving cursor
        handleMoveEvent(uiEvent);

        // Let ui know we moved cursor
        ui_menuMoveCursor(cursorItem, 0);
    }

}

