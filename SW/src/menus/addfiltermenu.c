
//
// Filename    : addfiltermenu.c
// Code module : Add filter menu implementation
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

// The add filter menu presents a list of all available filters for adding to
// filtersteps

// -------------------------------  INCLUDES  -----------------------------------

#include "../common.h"
#include "../errors.h"
#include "../util.h"
#include "../hal.h"
#include "../filters.h"
#include "../filtersteps.h"
#include "../ui.h"
#include <avr/pgmspace.h>
#include <string.h>

// ------------------------------  PROTOTYPES  ----------------------------------


static uint8_t initGetCursor(void);    // Initialize adding filters
static uint8_t getItemCount(void);     // Get item count when adding (all filters)
static void    drawItem(uint8_t item); // Write item from list of all filters
static void    handleUiEvent(uint8_t uiEvent);

static void    handleMoveEvent(uint8_t uiEvent);


// --------------------------  TYPES AND CONSTANTS  -----------------------------

static const char AddTitle[] PROGMEM = "--- ADD Filter: ----";

// Add filter menu interface declaration
const menuInterface_t PROGMEM addfiltermenu_AddMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        drawItem,         // fptrVoidUint8_t  drawItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t addCursorItem = 1;

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


// move cursor

static void handleMoveEvent(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (addCursorItem < FILTER_TYPE_COUNT)
        {
            addCursorItem++;
        }
        break;
    case UI_EVENT_MOVE_DOWN:
        if (addCursorItem > 1)
        {
            addCursorItem--;
        }
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        addCursorItem = FILTER_TYPE_COUNT;
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        addCursorItem = 1;
        break;
    }
}

// ADD menu

static uint8_t initGetCursor(void)
{
    return addCursorItem;
}

static uint8_t getItemCount(void)
{
    return FILTER_TYPE_COUNT+1;
}

static void drawItem(uint8_t item)
{
    if (item == 0)
    {
        // Title
        ui_menuDrawItemP(0, (uint8_t*)AddTitle);
    }
    else
    {
        // Menu item
        uint8_t buffer[21];
        memset(buffer, 0, 21);

        filter_getTitle(item - 1, (char*)(buffer));

        ui_menuDrawItem(item, buffer);
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_SELECT)
    {
        // Add filter
        fsteps_addFilter_MAIN(addCursorItem - 1, NULL);

        // Now the filter is in the last position, reorder so midiout is still
        // the last filter:
        fsteps_swapFilter_MAIN(fsteps_getCount_SAFE() - 1, fsteps_getCount_SAFE() - 2);

        // Back out of menu
        ui_menuBackOut();
    }
    else if (uiEvent == UI_EVENT_BACK)
    {
        ui_menuBackOut();
    }
    else
    {
        // Move cursor
        handleMoveEvent(uiEvent);

        // Let ui know we moved cursor
        ui_menuMoveCursor(addCursorItem, 0);
    }

}

