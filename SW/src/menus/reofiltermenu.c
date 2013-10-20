
//
// Filename    : reofiltermenu.c
// Code module : Remove/Reorder filter menus implementation
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
#include "../midiprocessing.h"
#include "../ui.h"
#include "../util.h"
#include "../menuinterface.h"
#include "../pgmstrings.h"
#include <avr/pgmspace.h>
#include <string.h>

//
// Implements three menus dealing with filter adding, removing and
// reordering.
//
// The add filter menu uses a list of all available filters

// ------------------------------  PROTOTYPES  ----------------------------------


// The remove / reorder menus interface functions:
static uint8_t reInitGetCursor(void);  // Initialize removing filters
static uint8_t oInitGetCursor(void);   // Initialize re ordering filters
static uint8_t getItemCount(void);     // Get item count when removing or reordering (all loaded filters)
static void    writeItem(uint8_t item, void *dest);
static void    handleUiEvent(uint8_t uiEvent);

// Private helper functions:
static void    handleMoveEvent(uint8_t uiEvent);

// --------------------------  TYPES AND CONSTANTS  -----------------------------


// Organize filter menus:

static const char ReMenuTitle[] PROGMEM = "--- REMOVE Filter: -";
static const char OMenuTitle[] PROGMEM = "--- REORDER Filters:";

// Remove filter menu
const menuInterface_t PROGMEM reofiltermenu_RemoveMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        reInitGetCursor,  // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

// Reorder filter menu
const menuInterface_t PROGMEM reofiltermenu_ReorderMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        oInitGetCursor,   // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};


// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t selectedItem = 1;

static bool_t movingFilterMode = FALSE;
static bool_t removeNotReorderMenu = FALSE;  // True if remove mode

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


static uint8_t reInitGetCursor(void)
{
    movingFilterMode = FALSE;
    removeNotReorderMenu = TRUE;
    return selectedItem;
}


static uint8_t oInitGetCursor(void)
{
    movingFilterMode = FALSE;
    removeNotReorderMenu = FALSE;
    return selectedItem;
}

static void writeItem(uint8_t item, void *dest)
{
    if (item == 0)
    {
        // Title
        util_copyString_P(dest, removeNotReorderMenu ? ReMenuTitle : OMenuTitle);
    }
    else if ((item-1) < midiproc_getFilterCount_SAFE())
    {
        filters_instance_t filter = midiproc_getFilterInstance_SAFE(item - 1);

        if ((movingFilterMode) && (item == selectedItem))
        {
            // We are currently moving this filter, indent one space
            util_copyString_P((char*)(dest) + 1, filters_getFilterTitle(filter.FilterType));
        }
        else
        {
            util_copyString_P(dest, filters_getFilterTitle(filter.FilterType));
        }
    }
    else
    {
        // Empty line
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_SELECT)
    {
        if (removeNotReorderMenu)
        {
            // Remove filter menu and select is pushed,

            // this filter needs to go, unless it's the final midi out filter
            if ((selectedItem - 1) < (midiproc_getFilterCount_SAFE() - 1))
            {
                midiproc_stop_MAIN();
                midiproc_removeFilter_MAIN(selectedItem - 1);
                midiproc_start_MAIN();

                // Back out of menu
                ui_menuBackOut();
            }
        }
        else
        {
            // Reorder filter menu and select is pushed,
            // Go into moving mode, unless its the last filter
            if ((movingFilterMode == FALSE) &&
                    ((selectedItem - 1) != (midiproc_getFilterCount_SAFE() - 1)))
            {
                movingFilterMode = TRUE;

                // We need to redraw this line
                ui_requestUpdate(selectedItem);

                // With correct cursor indent
                ui_menuMoveCursor(selectedItem, 2);
            }
            else
            {
                movingFilterMode = FALSE;

                // We need to redraw this line
                ui_requestUpdate(selectedItem);

                // With correct cursor indent
                ui_menuMoveCursor(selectedItem, 0);
            }
        }
    }
    else if (uiEvent == UI_EVENT_BACK)
    {
        ui_menuBackOut();
    }
    else
    {
        if (movingFilterMode)
        {
            // Reorder menu mode, currently moving a filter:

            // Record currently selected
            uint8_t oldItem = selectedItem;

            // Update according to user move
            handleMoveEvent(uiEvent);

            // Did we get to final midi out filter?
            if ((selectedItem - 1) == (midiproc_getFilterCount_SAFE() - 1))
            {
                // Then we must abort this
                selectedItem = oldItem;
            }
            else
            {
                // Ok, if cursor changed swap around and draw both
                if (selectedItem != oldItem)
                {
                    midiproc_stop_MAIN();
                    midiproc_swapFilters_MAIN(oldItem - 1, selectedItem - 1);
                    midiproc_start_MAIN();

                    // Draw both
                    ui_requestUpdate(oldItem);
                    ui_requestUpdate(selectedItem);

                    // Set cursor with indent
                    ui_menuMoveCursor(selectedItem, 2);
                }
            }

        }
        else
        {
            // Reorder or remove filter menu, not currently moving a filter,
            // just move cursor

            handleMoveEvent(uiEvent);
            ui_menuMoveCursor(selectedItem, 0);
        }
    }

}

static uint8_t getItemCount(void)
{
    return midiproc_getFilterCount_SAFE() + 1;
}

static void handleMoveEvent(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (selectedItem < midiproc_getFilterCount_SAFE())
        {
            selectedItem++;
        }
        break;
    case UI_EVENT_MOVE_DOWN:
        if (selectedItem > 1)
        {
            selectedItem--;
        }
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        selectedItem = midiproc_getFilterCount_SAFE();
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        selectedItem = 1;
        break;
    }
}

