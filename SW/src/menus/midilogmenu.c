
//
// Filename    : midilogmenu.c
// Code module : Midilog menu implementation
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
#include "../midilog.h"
#include "../util.h"
#include "../hal.h"
#include "../filters.h"
#include "../ui.h"
#include <avr/pgmspace.h>


// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t initGetCursor(void);
static uint8_t getItemCount(void);
static void    drawItem(uint8_t item);
static void    handleUiEvent(uint8_t uiEvent);

static void handleSelectEvent(void);
static void handleMoveEvent(uint8_t uiEvent);

// --------------------------  TYPES AND CONSTANTS  -----------------------------

// Menu self declaration struct
const menuInterface_t PROGMEM midilogmenu_Menu =
{
        TRUE,             // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        drawItem,         // fptrVoidUint8_t  drawItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};


// Menu items enumeration
enum
{
    ITEM_TITLE = 0,
    ITEM_INPUT1,
    ITEM_INPUT1_ALL,
    ITEM_INPUT2,
    ITEM_INPUT2_ALL,
    ITEM_OUTPUT,
    ITEM_OUTPUT_ALL,
    ITEM_COUNT
};

// Menu item texts:
#define MENUITEM_MAX 19

typedef struct
{
    uint8_t Text[MENUITEM_MAX];
} menuItem_t;

// Main menu:

static const menuItem_t MlogItem[ITEM_COUNT] PROGMEM =
{
    {"--- SHOW MIDI LOG:"},
    {"Input 1 no rt/sysx"},
    {"Input 1 all"},
    {"Input 2 no rt/sysx"},
    {"Input 2 all"},
    {"Output no rt/sysx"},
    {"Output all"},
};


// ----------------------------  LOCAL VARIABLES  -------------------------------

// Current cursor position
static uint8_t cursorItem = 1;


static bool_t logActive = FALSE;

// ---------------------------  PRIVATE FUNCTIONS  -------------------------------

static void handleSelectEvent(void)
{
    switch(cursorItem)
    {
    case ITEM_INPUT1:
        midilog_configure(MIDILOG_SOURCE_IN1, TRUE);
        logActive = TRUE;
        break;
    case ITEM_INPUT1_ALL:
        midilog_configure(MIDILOG_SOURCE_IN1, FALSE);
        logActive = TRUE;
        break;
    case ITEM_INPUT2:
        midilog_configure(MIDILOG_SOURCE_IN2, TRUE);
        logActive = TRUE;
        break;
    case ITEM_INPUT2_ALL:
        midilog_configure(MIDILOG_SOURCE_IN2, FALSE);
        logActive = TRUE;
        break;
    case ITEM_OUTPUT:
        midilog_configure(MIDILOG_SOURCE_OUT, TRUE);
        logActive = TRUE;
        break;
    case ITEM_OUTPUT_ALL:
        midilog_configure(MIDILOG_SOURCE_OUT, FALSE);
        logActive = TRUE;
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
    logActive = FALSE;

    return cursorItem;
}

static uint8_t getItemCount(void)
{
    // This menu always have the same number of items
    return ITEM_COUNT;
}

static void drawItem(uint8_t item)
{
    if (logActive == FALSE)
    {
        // We are instructed to draw an item.
        // Since everything is progmem strings for this menu, this is simple:
        ui_menuDrawItemP(item, MlogItem[item].Text);
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (logActive)
    {
        if (uiEvent == UI_EVENT_BACK)
        {
            midilog_stop();
            logActive = FALSE;
            ui_menuBackOut();
        }
        else
        {
            midilog_handleUi(uiEvent);
        }
    }
    else
    {
        if (uiEvent == UI_EVENT_BACK)
        {
            ui_menuBackOut();
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
}
