
//
// Filename    : orgfiltermenu.c
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
#include "../filtersteps.h"
#include "../ui.h"
#include <avr/pgmspace.h>
#include <string.h>

//
// Implements three menus dealing with filter adding, removing and
// reordering.
//
// The add filter menu uses a list of all available filters

// ------------------------------  PROTOTYPES  ----------------------------------


// These functions are common for remove / reorganize menus:
static uint8_t remInitGetCursor(void);    // Initialize removing filters
static uint8_t orgInitGetCursor(void);    // Initialize removing filters

static uint8_t getItemCount(void);     // Get item count when removing or reordering (all loaded filters)

// The drawing and ui events are different
static void    drawItem(uint8_t item); // Write item from list of all loaded filters
static void    handleUiEvent(uint8_t uiEvent);

static void    handleMoveEvent(uint8_t uiEvent);

// --------------------------  TYPES AND CONSTANTS  -----------------------------


// Organize filter menus:

static const char RemTitle[] PROGMEM = "--- REMOVE Filter: -";
static const char OrgTitle[] PROGMEM = "--- REORDER Filters:";

// Remove filter menu
menu_t RemMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        remInitGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        drawItem,      // fptrVoidUint8_t  drawItem;
        handleUiEvent  // fptrVoidUint8_t  handleUiEvent;
};

// Reorder filter menu
menu_t OrgMenu =
{
        TRUE,             // bool_t hasStaticTitle;
        orgInitGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        drawItem,      // fptrVoidUint8_t  drawItem;
        handleUiEvent  // fptrVoidUint8_t  handleUiEvent;
};


// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t cursorItem = 1;

static bool_t movingMode = FALSE;
static bool_t remNotOrg = FALSE;  // True if remove mode

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


menu_t * orgfiltermenu_getRemoveMenu(void)
{
    return &RemMenu;
}

menu_t * orgfiltermenu_getReorderMenu(void)
{
    return &OrgMenu;
}

static uint8_t remInitGetCursor(void)
{
    movingMode = FALSE;
    remNotOrg = TRUE;
    return cursorItem;
}


static uint8_t orgInitGetCursor(void)
{
    movingMode = FALSE;
    remNotOrg = FALSE;
    return cursorItem;
}

static void drawItem(uint8_t item)
{
    if (item == 0)
    {
        // Title
        ui_menuDrawItemP(item, (uint8_t *)(remNotOrg ? RemTitle : OrgTitle));
    }
    else
    {
        uint8_t buffer[21];
        memset(buffer, 0, 21);
        filter_t *filter = fsteps_getFilter_SAFE(item - 1);

        if (filter != NULL)
        {
            if ((movingMode) && (item == cursorItem))
            {
                // We are currently moving this filter, indent one space
                buffer[0] = ' ';
                filter_getTitle(filter->FilterType, (char*)(buffer + 1));
            }
            else
            {
                filter_getTitle(filter->FilterType, (char*)(buffer));
            }
        }

        ui_menuDrawItem(item, buffer);
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_SELECT)
    {
        if (remNotOrg)
        {
            // Remove the filter step
            fsteps_removeFilter_MAIN(cursorItem - 1);

            // Back out of menu
            ui_menuBackOut();
        }
        else
        {
            // Toggle moving mode
            movingMode = !movingMode;

            // We need to redraw this line
            drawItem(cursorItem);

            // With cursor indent
            ui_menuMoveCursor(cursorItem, 3);

        }
    }
    else if (uiEvent == UI_EVENT_BACK)
    {
        ui_menuBackOut();
    }
    else
    {
        if (movingMode)
        {
            // Record currently selected
            uint8_t oldItem = cursorItem;

            //
            handleMoveEvent(uiEvent);

            // Any change, then swap around and draw both
            if (cursorItem != oldItem)
            {
                fsteps_swapFilter_MAIN(oldItem - 1, cursorItem - 1);

                // Draw both
                drawItem(oldItem);
                drawItem(cursorItem);

                // Set cursor with indent
                ui_menuMoveCursor(cursorItem, 3);
            }

        }
        else
        {
            // Move cursor
            handleMoveEvent(uiEvent);

            ui_menuMoveCursor(cursorItem, 0);
        }
    }

}

static uint8_t getItemCount(void)
{
    return fsteps_getCount_SAFE() + 1;
}

static void handleMoveEvent(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (cursorItem < (fsteps_getCount_SAFE()))
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
        cursorItem = fsteps_getCount_SAFE();
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        cursorItem = 1;
        break;
    }
}

