
//
// Filename    : filtermenu.c
// Code module : Individual filter configuration menu implementation
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
#include "../menuinterface.h"
#include "../midimessage.h"
#include <avr/pgmspace.h>
#include <string.h>

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t initGetCursor(void);
static uint8_t getItemCount(void);
static void    writeItem(uint8_t item, void *buffer);
static void    handleUiEvent(uint8_t uiEvent);


static void countItems(void);
static void setCurrentFilter(uint8_t item);
static void handleMoveEvent(uint8_t uiEvent);
static uint8_t routeToAscii(uint8_t route);
static void setCurrentCursor(void);

// --------------------------  TYPES AND CONSTANTS  -----------------------------


const menuInterface_t PROGMEM filtermenu_Menu =
{
        FALSE,            // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

// ----------------------------  LOCAL VARIABLES  -------------------------------

uint8_t selectedItem = 0;
uint8_t itemCount;
uint8_t mode;

#define MODE_NO_EDIT      0 // Not editing anything
#define MODE_EDIT_FILTER  1 // Editing config of a filter
#define MODE_EDIT_IN      2 // Editing input of filter
#define MODE_EDIT_OUT     3 // Editing output of filter

uint8_t currentFilterMenu;  // 0 is title menu, 1 is first menu item
filters_instance_t currentFilter;
uint8_t currentFilterStep;

// --------------------------  PRIVATE FUNCTIONS  -------------------------------

static void countItems(void)
{
    uint8_t i;

    // One row for each filter
    itemCount = midiproc_getFilterSteps_SAFE();

    for (i = 0; i < midiproc_getFilterSteps_SAFE(); i++)
    {
        // Plus it's submenus
        itemCount += filters_getMenuItems(midiproc_getFilterInstance_SAFE(i).FilterType);
    }
}

// Based selectedItem, find current filter and filtermenu
static void setCurrentFilter(uint8_t item)
{
    uint8_t i;
    uint8_t n;
    uint8_t count;

    // One row for each filter
    n = item;

    for (i = 0; i < midiproc_getFilterSteps_SAFE(); i++)
    {
        currentFilter = midiproc_getFilterInstance_SAFE(i);
        currentFilterStep = i;
        currentFilterMenu = n;

        // Are we at top menu for this filter?
        if (n == 0)
        {
            // Yes
            break;
        }

        // Move on
        n--;

        // Are we at a submenu for this filter?
        count = filters_getMenuItems(currentFilter.FilterType);

        if (n < count)
        {
            // Yes
            break;
        }
        else
        {
            // No, move on by number of submenu items
            n -= count;
        }
    }
}



static void handleMoveEvent(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        if (selectedItem < (itemCount - 1))
        {
            selectedItem++;
        }
        break;

    case UI_EVENT_MOVE_DOWN:
        if (selectedItem > 0)
        {
            selectedItem--;
        }
        break;

    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        selectedItem = itemCount - 1;
        break;

    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        selectedItem = 0;
        break;
    }
}

static uint8_t routeToAscii(uint8_t route)
{
    uint8_t ret;

    if (route == MIDIMSG_ROUTE_ALL)
    {
        ret = 'A';
    }
    else if (route == MIDIMSG_ROUTE_INACTIVE)
    {
        ret = 'X';
    }
    else if (route <= 9)
    {

        ret = route + '0';
    }
    else
    {
        ret = '?';
    }

    return ret;
}


static void setCurrentCursor(void)
{
    uint8_t col = 0;

    switch (mode)
    {
    case MODE_NO_EDIT:
        col = 0;
        break;
    case MODE_EDIT_IN:
        col = 17;
        break;
    case MODE_EDIT_OUT:
        col = 19;
        break;
    case MODE_EDIT_FILTER:
        // Get indentation from filter
        col = filters_getMenuIndent(currentFilter.FilterType) + 2;
        break;
    }

    ui_menuMoveCursor(selectedItem, col);
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

static uint8_t initGetCursor(void)
{
    mode = MODE_NO_EDIT;
    countItems();

    if (selectedItem >= itemCount)
    {
        selectedItem = itemCount - 1;
    }

    return selectedItem;
}

static uint8_t getItemCount(void)
{
    return itemCount;
}

static void writeItem(uint8_t item, void *dest)
{
    uint8_t *buffer = dest;

    if (item < itemCount)
    {
        // Find item
        setCurrentFilter(item);

        if (currentFilterMenu == 0)
        {
            // Write menu text
            filters_writeMenuText(currentFilter, 0, buffer);

            // We are at title menu, write filter input / output routes:
            midiproc_route_t route = midiproc_getFilterRoute_SAFE(currentFilterStep);

            // The style depends on filter operating mode
            uint8_t mode = filters_getFilterMode(currentFilter.FilterType);

            if (mode != FILTER_MODE_OUT)
            {
                buffer[16] = routeToAscii(route.In);
            }

            buffer[17] = '>';

            if (mode != FILTER_MODE_IN)
            {
                buffer[18] = routeToAscii(route.Out);
            }
        }
        else
        {
            // Write menu text
            filters_writeMenuText(currentFilter, currentFilterMenu, &(buffer[1]));
        }
    }

}

static void handleUiEvent(uint8_t uiEvent)
{
    // Make sure we are at selected item
    setCurrentFilter(selectedItem);

    if (uiEvent == UI_EVENT_BACK)
    {
        if (mode == MODE_NO_EDIT)
        {
            // We are at root level, back out
            ui_menuBackOut();
        }
        else
        {
            // We are editing something, just exit edit mode
            mode = MODE_NO_EDIT;
            setCurrentCursor();
        }
    }
    else if (uiEvent == UI_EVENT_SELECT)
    {
        // Selecting stuff.
        if (currentFilterMenu == 0)
        {
            uint8_t filter_mode = filters_getFilterMode(currentFilter.FilterType);

            // In this case we toggle between root menu, in and out.
            // Set cursor accordingly
            switch (mode)
            {
            case MODE_NO_EDIT:
                if (filter_mode == FILTER_MODE_OUT)
                {
                    mode = MODE_EDIT_OUT;
                }
                else
                {
                    mode = MODE_EDIT_IN;
                }
                break;
            case MODE_EDIT_IN:
                if ((filter_mode == FILTER_MODE_IN) ||
                    (filter_mode == FILTER_MODE_PROCESSOR))
                {
                    mode = MODE_NO_EDIT;
                }
                else
                {
                    mode = MODE_EDIT_OUT;
                }
                break;
            case MODE_EDIT_OUT:
                mode = MODE_NO_EDIT;
                break;
            }
        }
        else
        {
            // In this case we toggle between root menu and filter edit
            if (mode == MODE_NO_EDIT)
            {
                mode = MODE_EDIT_FILTER;

                // Let filter handle the select event
                filters_handleUiEvent(currentFilter, currentFilterMenu, uiEvent);
            }
            else
            {
                mode = MODE_NO_EDIT;
            }
        }

        setCurrentCursor();
    }
    else
    {
        // Incrementing / decrementing stuff
        if (mode == MODE_NO_EDIT)
        {
            // Moving cursor
            handleMoveEvent(uiEvent);
        }
        else if ((mode == MODE_EDIT_IN) ||
                 (mode == MODE_EDIT_OUT))
        {
            // We are editing IN/OUT
            midiproc_route_t route = midiproc_getFilterRoute_SAFE(currentFilterStep);

            if (mode == MODE_EDIT_IN)
            {
                route.In = util_boundedAddInt8(route.In, -1, 9, ui_eventToDelta(uiEvent, 10));
            }
            else
            {
                route.Out = util_boundedAddInt8(route.Out, -1, 9, ui_eventToDelta(uiEvent, 10));
            }

            // Update route. First we must stop processing...
            midiproc_stop_MAIN();
            midiproc_setRoute_MAIN(currentFilterStep, route);
            midiproc_start_MAIN();

        }
        else if (mode == MODE_EDIT_FILTER)
        {
            // Let filter handle this edit operation
            filters_handleUiEvent(currentFilter, currentFilterMenu, uiEvent);
        }
        else
        {
            // TODO ERROR!
        }

        // Update this in any case and set cursor
        ui_requestUpdate(selectedItem);
        setCurrentCursor();
    }
}

void filtermenu_RequestUpdate(filters_instance_t filter, uint8_t menu_item)
{
    // We won't even start this business if we are not the current menu
    if (ui_getCurrentMenu() == &filtermenu_Menu)
    {
        // Find filter menu item
        uint8_t i;
        uint8_t item = 0;
        bool_t found = FALSE;

        for (i = 0; (i < midiproc_getFilterSteps_SAFE()) && (found == FALSE); i++)
        {
            filters_instance_t fi = midiproc_getFilterInstance_SAFE(i);
            if ((fi.FilterType == filter.FilterType) &&
                (fi.Instance == filter.Instance))
            {
                // This is the filter. Add desired menu item
                item += menu_item;
                found = TRUE;
            }
            else
            {
                // Move on to next filter, add it's menu item count
                item += 1 + filters_getMenuItems(fi.FilterType);
            }
        }

        if (found)
        {
            ui_requestUpdate(item);
        }
        else
        {
            // This shouldn't happen
            // TODO err_raise_MAIN()
        }
    }
}
