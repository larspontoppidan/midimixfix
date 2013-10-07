
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
#include "../filtersteps.h"
#include "../ui.h"
#include "../midimessage.h"
#include <avr/pgmspace.h>
#include <string.h>

// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t initGetCursor(void);
static uint8_t getItemCount(void);
static void    drawItem(uint8_t item);
static void    handleUiEvent(uint8_t uiEvent);


static void countItems(void);
static void setCurrentFilter(uint8_t item);
static void handleMoveEvent(uint8_t uiEvent);
static uint8_t routeToAscii(uint8_t route);
static int8_t eventToDelta(uint8_t uiEvent);
static void setCurrentCursor(void);

// --------------------------  TYPES AND CONSTANTS  -----------------------------


// Menu self declaration struct
menu_t FilterMenu =
{
        FALSE,            // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        drawItem,         // fptrVoidUint8_t  drawItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};

#define TITLE_MENU    0xFF

// ----------------------------  LOCAL VARIABLES  -------------------------------

uint8_t selectedItem = 0;
uint8_t itemCount;
uint8_t mode;

#define MODE_NO_EDIT      0 // Not editing anything
#define MODE_EDIT_FILTER  1 // Editing config of a filter
#define MODE_EDIT_IN      2 // Editing input of filter
#define MODE_EDIT_OUT     3 // Editing output of filter

uint8_t currentFilterMenu;  // TITLE_MENU, otherwise menu item
filter_t *currentFilter;

// --------------------------  PRIVATE FUNCTIONS  -------------------------------

static void countItems(void)
{
    uint8_t i;

    // One row for each filter
    itemCount = fsteps_getCount_SAFE();

    for (i = 0; i < fsteps_getCount_SAFE(); i++)
    {
        // Plus it's submenus
        itemCount += filter_request(fsteps_getFilter_SAFE(i), FILTER_REQ_MENU_ITEMS);
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

    for (i = 0; i < fsteps_getCount_SAFE(); i++)
    {
        currentFilter = fsteps_getFilter_SAFE(i);

        // Are we at top menu for this filter?
        if (n == 0)
        {
            // Yes
            currentFilterMenu = TITLE_MENU;
            break;
        }

        // Move on
        n--;

        // Are we at a submenu for this filter?
        count = filter_request(currentFilter, FILTER_REQ_MENU_ITEMS);

        if (n < count)
        {
            // Yes
            currentFilterMenu = n;
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
        ret = ' ';
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

static int8_t eventToDelta(uint8_t uiEvent)
{
    uint8_t delta = 0;

    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        delta = 1;
        break;
    case UI_EVENT_MOVE_DOWN:
        delta = -1;
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        delta = 10;
        break;

    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        delta = -10;
        break;
    }

    return delta;
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
        col = filter_request(currentFilter, FILTER_REQ_MENU_INDENT) + 2;
        break;
    }

    ui_menuMoveCursor(selectedItem, col);
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


menu_t * filtermenu_getMenu(void)
{
    return &FilterMenu;
}

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

static void drawItem(uint8_t item)
{
    uint8_t buffer[21];

    memset(buffer, ' ', 19);
    buffer[19] = 0;

    // Find item
    setCurrentFilter(item);

    if (currentFilterMenu == TITLE_MENU)
    {
        // Write filter title
        filter_getTitle(currentFilter->FilterType, (char*)(buffer));

        // Write filter input / output routes:
        buffer[16] = routeToAscii(currentFilter->RouteIn);
        buffer[17] = '>';
        buffer[18] = routeToAscii(currentFilter->RouteOut);
    }
    else
    {
        // Write filter submenu with indent
        filter_getMenuText(currentFilter, (char *)(buffer + 1), currentFilterMenu);
    }

    ui_menuDrawItem(item, buffer);
}

static void handleUiEvent(uint8_t uiEvent)
{
    if (uiEvent == UI_EVENT_BACK)
    {
        ui_menuBackOut();
    }
    else if (uiEvent == UI_EVENT_SELECT)
    {
        // Selecting stuff.

        // Make sure selectedItem is currentFilter
        setCurrentFilter(selectedItem);

        if (currentFilterMenu == TITLE_MENU)
        {
            // In this case we toggle between root menu, in and out.
            // Set cursor accordingly
            switch (mode)
            {
            case MODE_NO_EDIT:
                mode = MODE_EDIT_IN;
                break;
            case MODE_EDIT_IN:
                mode = MODE_EDIT_OUT;
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

        switch (mode)
        {
        case MODE_NO_EDIT:
            // Moving cursor
            handleMoveEvent(uiEvent);
            break;

        case MODE_EDIT_IN:
            currentFilter->RouteIn = util_boundedAddInt8(currentFilter->RouteIn, -1, 9, eventToDelta(uiEvent));

            // Notify filter of change
            filter_request(currentFilter, FILTER_REQ_UPDATE_SELF);
            break;

        case MODE_EDIT_OUT:
            currentFilter->RouteOut = util_boundedAddInt8(currentFilter->RouteOut, 0, 9, eventToDelta(uiEvent));

            // Notify filter of change
            filter_request(currentFilter, FILTER_REQ_UPDATE_SELF);
            break;

        case MODE_EDIT_FILTER:
            // Let filter handle this edit operation
            filter_handleUiEvent(currentFilter, currentFilterMenu, uiEvent);
            break;
        }

        // Update this in any case and set cursor
        drawItem(selectedItem);
        setCurrentCursor();
    }
}
