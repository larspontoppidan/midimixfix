
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
#include <avr/pgmspace.h>


// ------------------------------  PROTOTYPES  ----------------------------------


static uint8_t saveInitGetCursor(void);
static uint8_t loadInitGetCursor(void);
static uint8_t getItemCount(void);
static void    writeItem(uint8_t item, void *dest);
static void    handleUiEvent(uint8_t uiEvent);

static void handleLoadEvent(void);
static void handleSaveEvent(void);
static void handleMoveEvent(uint8_t uiEvent);


// --------------------------  TYPES AND CONSTANTS  -----------------------------

#define PRESET_COUNT 10

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

static const char PresetItem[] PROGMEM = "Preset %i";


// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t cursorItem = 1;
static bool_t  saveNotLoad;


// ---------------------------  PRIVATE FUNCTIONS  -------------------------------


static void handleLoadEvent(void)
{

}

static void handleSaveEvent(void)
{

}

static void handleMoveEvent(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (cursorItem < (PRESET_COUNT - 1))
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
        cursorItem = PRESET_COUNT - 1;
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
    return cursorItem;
}

static uint8_t loadInitGetCursor(void)
{
    saveNotLoad = FALSE;
    return cursorItem;
}

static uint8_t getItemCount(void)
{
    return PRESET_COUNT;
}

static void writeItem(uint8_t item, void *dest)
{
    if (item == 0)
    {
        util_copyString_P(dest, saveNotLoad ? PresetSaveTitle : PresetLoadTitle);
    }
    else
    {
        util_writeFormat_P(dest, PresetItem, item);
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
            handleSaveEvent();
        }
        else
        {
            handleLoadEvent();
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

