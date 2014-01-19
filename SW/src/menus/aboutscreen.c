
//
// Filename    : aboutscreen.c
// Code module : Display MIDIMIXFIX credits, version info and pending errors!
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
#include "../util.h"
#include "../menuinterface.h"
#include <avr/pgmspace.h>
#include <string.h>


// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t initGetCursor(void);
static uint8_t getItemCount(void);
static void    writeItem(uint8_t item, void *dest);
static void    handleUiEvent(uint8_t uiEvent);

// --------------------------  TYPES AND CONSTANTS  -----------------------------

// Menu self declaration struct
const menuInterface_t PROGMEM aboutscreen_Menu =
{
        TRUE,             // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};


// Menu item texts:
#define MENUITEM_MAX 21

typedef struct
{
    char Text[MENUITEM_MAX];
} menuItem_t;


#define ITEM_COUNT 9

static const menuItem_t MainItem[ITEM_COUNT] PROGMEM =
{
    {"----   ABOUT:   ----"},
    {"  MIDIMIXFIX v1.0"},       // TODO configuration management
    {"       By:"},
    {"Lars O. Pontoppidan"},
    {" http://larsee.dk"},
    {""},
    {"  Copyright 2013"},
    {""},
    {"Errors:"}                  // TODO this type of string storage could use some compression
};


// ----------------------------  LOCAL VARIABLES  -------------------------------

// Current cursor position
static uint8_t cursorItem = 1;


// ------------------------  MENU INTERFACE FUNCTIONS ---------------------------


static uint8_t initGetCursor(void)
{
    return cursorItem;
}

static uint8_t getItemCount(void)
{
    uint8_t ret;

    if (err_getCount() == 0)
    {
        ret = ITEM_COUNT - 2;
    }
    else
    {
        ret = ITEM_COUNT + err_getCount();
    }

    return ret;
}

static void writeItem(uint8_t item, void *dest)
{
    if (item < ITEM_COUNT)
    {
        util_copyString_P(dest, MainItem[item].Text);
    }
    else
    {
        err_print((char*)dest + 1, item - ITEM_COUNT);
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_BACK)
    {
        ui_menuBackOut();
    }
    else
    {
        // Moving cursor
        cursorItem = util_boundedAddUint8(cursorItem, 1, getItemCount() - 1, ui_eventToDelta(uiEvent, 25));
        ui_menuMoveCursor(cursorItem, 0);
    }
}
