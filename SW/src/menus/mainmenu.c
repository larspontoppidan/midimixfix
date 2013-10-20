
//
// Filename    : mainmenu.c
// Code module : Main midimixfix menu implementation implementation
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
#include "../hal.h"
#include "../filters.h"
#include "../ui.h"
#include "../menuinterface.h"
#include "../lcd.h"
#include "../util.h"
#include <avr/pgmspace.h>

#include "presetsmenu.h"
#include "addfiltermenu.h"
#include "reofiltermenu.h"
#include "filtermenu.h"
#include "midilogmenu.h"

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t initGetCursor(void);
static uint8_t getItemCount(void);
static void    writeItem(uint8_t item, void *dest);
static void    handleUiEvent(uint8_t uiEvent);

static void handleSelectEvent(void);
static void handleMoveEvent(uint8_t uiEvent);

// --------------------------  TYPES AND CONSTANTS  -----------------------------

// Menu self declaration struct
const menuInterface_t PROGMEM mainmenu_Menu =
{
        TRUE,             // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

// Menu items enumeration
enum
{
    ITEM_TITLE = 0,
    ITEM_FILTERS,
    ITEM_LOAD_PRESET,
    ITEM_SAVE_PRESET,
    ITEM_ADD_FILT,
    ITEM_REMOVE_FILT,
    ITEM_REORG_FILT,
    ITEM_MIDILOG,
    ITEM_BOOTLOADER,
    ITEM_ABOUT,
    ITEM_COUNT
};

// Menu item texts:
#define MENUITEM_MAX 21

typedef struct
{
    char Text[MENUITEM_MAX];
} menuItem_t;

// Main menu:

static const menuItem_t MainItem[ITEM_COUNT] PROGMEM =
{
    {"**** MIDIMIXFIX ****"},
    {" Setup filters"},
    {" Load preset"},
    {" Save preset"},
    {" Add filter"},
    {" Remove filter"},
    {" Reorder filters"},
    {" Show midi log"},
    {" Bootloader"},
    {" About"}
};


// ----------------------------  LOCAL VARIABLES  -------------------------------

// Current cursor position
static uint8_t cursorItem = 1;

// ---------------------------  PRIVATE FUNCTIONS  -------------------------------

static void handleSelectEvent(void)
{
    switch(cursorItem)
    {
    case ITEM_FILTERS:
        ui_menuEnter(&filtermenu_Menu);
        break;
    case ITEM_LOAD_PRESET:
        ui_menuEnter(&presetsmenu_LoadMenu);
        break;
    case ITEM_SAVE_PRESET:
        ui_menuEnter(&presetsmenu_SaveMenu);
        break;
    case ITEM_ADD_FILT:
        ui_menuEnter(&addfiltermenu_AddMenu);
        break;
    case ITEM_REMOVE_FILT:
        ui_menuEnter(&reofiltermenu_RemoveMenu);
        break;
    case ITEM_REORG_FILT:
        ui_menuEnter(&reofiltermenu_ReorderMenu);
        break;
    case ITEM_MIDILOG:
        ui_menuEnter(&midilogmenu_Menu);
        break;
    case ITEM_BOOTLOADER:
        lcd_clear();
        lcd_writeString_P(PSTR("Bootloader activated"));
        hal_lcdBacklightSet(FALSE);
        hal_jumpBootloader();
        break;
    case ITEM_ABOUT:
        break;
    }
}

static void handleMoveEvent(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (cursorItem < (ITEM_COUNT - 1))
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
        cursorItem = ITEM_COUNT - 1;
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        cursorItem = 1;
        break;
    }
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


// ---------------------------  MENU CALLBACKS ------------------------------


static uint8_t initGetCursor(void)
{
    // We are about to enter this menu. Just return cursor position
    return cursorItem;
}

static uint8_t getItemCount(void)
{
    // This menu always have the same number of items
    return ITEM_COUNT;
}

static void writeItem(uint8_t item, void *dest)
{
    // We are instructed to draw an item.
    // Since everything is progmem strings for this menu, this is simple:
    util_copyString_P(dest, MainItem[item].Text);
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_BACK)
    {
        // User tries to back out of main menu. Not going to happen...
    }
    else if (uiEvent == UI_EVENT_SELECT)
    {
        // Selecting stuff in main menu:
        handleSelectEvent();
    }
    else
    {
        // Moving cursor
        handleMoveEvent(uiEvent);

        // Let ui know we moved cursor
        ui_menuMoveCursor(cursorItem, 0);
    }
}
