
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
#include "../midiprocessing.h"
#include "../ui.h"
#include "../menuinterface.h"
#include "../util.h"
#include "msgscreen.h"
#include <avr/pgmspace.h>
#include <string.h>

// ------------------------------  PROTOTYPES  ----------------------------------


static uint8_t initGetCursor(void);    // Initialize adding filters
static uint8_t getItemCount(void);     // Get item count when adding (all filters)
static void    writeItem(uint8_t item, void *dest);
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
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
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
        if (addCursorItem < (FILTERS_TYPE_COUNT-1))
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
        addCursorItem = (FILTERS_TYPE_COUNT-1);
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
    return (FILTERS_TYPE_COUNT-1) + 1;
}

static void writeItem(uint8_t item, void *dest)
{
    if (item == 0)
    {
        // Title
        util_copyString_P(dest, AddTitle);
    }
    else
    {
        // Menu item
        util_copyString_P(dest, filters_getFilterTitle(item - 1));
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_SELECT)
    {
        // Add filter
        midiproc_stop_MAIN();

        if (midiproc_addFilter_MAIN(addCursorItem - 1))
        {
            // Now the filter is in the last position, reorder so midiout is still
            // the last filter:
            midiproc_swapFilters_MAIN(midiproc_getFilterSteps_SAFE() - 1, midiproc_getFilterSteps_SAFE() - 2);
            midiproc_start_MAIN();

            msgscreen_Show_P(PSTR("FILTER ADDED"), 3);
        }
        else
        {
            midiproc_start_MAIN();

            msgscreen_Show_P(PSTR("ADD FAILED!"), 4);
        }

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

