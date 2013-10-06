
//
// Filename    : filtermenu.c
// Code module : Implements menu system for filters
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



#include "common.h"
#include "menu.h"
#include "lcd.h"
#include "menuentities.h"
#include "presets.h"
#include "errors.h"
#include <string.h>


// --------------------------  TYPES AND CONSTANTS  -----------------------------


#define NOT_ACTIVE 0xFF

#define MENU_INDENT_CHAR  ' '
#define MENU_CURSOR_CHAR  LCD_CHAR_RIGHTARROW



// ----------------------------  LOCAL VARIABLES  -------------------------------


// Menu state:

// Current row highlighted on display, 0 to 3:
static uint8_t CurrentRow;

// The component providing stuff for currently selected menu item:
static uint8_t CurrentComponent;

// The subitem of the component's menu currently active:
static uint8_t CurrentSubItem;  // 0 means title item, 1 means subitem 0, ...

// Edit mode, if any:
static uint8_t CurrentEditMode;

static uint8_t LineBuffer[LCD_COLUMNS + 5];

// How much needs to be redrawn
static bool_t NeedRedraw;

#define NEED_REDRAW_NONE 0
#define NEED_REDRAW_ROW  1
#define NEED_REDRAW_ALL  2

// ------------------------------  PROTOTYPES  ----------------------------------

static void renderRow(uint8_t row, uint8_t component, uint8_t subitem);
static void writeBlankRow(uint8_t row);
static bool_t getPreviousMenuItem(uint8_t *comp, uint8_t *subitem);
static bool_t getNextMenuItem(uint8_t *comp, uint8_t *subitem);
static void render(bool_t render_all);
static void setCursor(uint8_t row, uint8_t column);


// ---------------------------  PRIVATE FUNCTIONS  ------------------------------

static void renderRow(uint8_t row, uint8_t component, uint8_t subitem)
{
    uint8_t i;

    // Clear line buffer
    memset(LineBuffer, 0u, LCD_COLUMNS);

    // Ask component to write into linebuffer at some offset
    if (subitem == 0u)
    {
        menuents_getText(component, (char*) &(LineBuffer[1u]), subitem);
    }
    else
    {
        LineBuffer[1u] = MENU_INDENT_CHAR;
        menuents_getText(component, (char*) &(LineBuffer[2u]), subitem);
    }

    lcd_setCursor(row, 1u);

    for (i = 1u; i < LCD_COLUMNS; i++)
    {
        if (LineBuffer[i] != 0)
        {
            lcd_write(LineBuffer[i]);
        }
        else
        {
            lcd_write(' ');
        }
    }
}

static void writeBlankRow(uint8_t row)
{
    lcd_setCursor(row, 1u);
    lcd_writeRepeat(' ', LCD_COLUMNS - 1);
}

static bool_t getPreviousMenuItem(uint8_t *comp, uint8_t *subitem)
{
    bool_t success;

    if (*subitem > 0)
    {
        (*subitem)--;
        success = TRUE;
    }
    else if (*comp >= 1)
    {
        (*comp)--;
        *subitem = menuents_getSubCount(*comp);
        success = TRUE;
    }
    else
    {
        success = FALSE;
    }

    return success;
}

static bool_t getNextMenuItem(uint8_t *comp, uint8_t *subitem)
{
    bool_t success;
    uint8_t subitemcount;

    subitemcount = menuents_getSubCount(*comp);

    if (*subitem < subitemcount)
    {
        (*subitem)++;
        success = TRUE;
    }
    else if ((uint8_t)((*comp) + 1u) < MENUE_ENTITY_COUNT)
    {
        (*comp)++;
        (*subitem) = 0;
        success = TRUE;
    }
    else
    {
        success = FALSE;
    }

    return success;
}

// Render draws current selected line or all.
// Modifies cursor!
static void render(bool_t render_all)
{
    uint8_t subitemcount;

    // Start with current row. Is the menu item even existing?

    subitemcount = menuents_getSubCount(CurrentComponent);

    if (subitemcount < CurrentSubItem)
    {
        // No! Back into what's available in selected component
        CurrentSubItem = subitemcount;

        // Menu has changed, we must ensure all is rendered
        render_all = TRUE;
    }

    // Render current row
    renderRow(CurrentRow, CurrentComponent, CurrentSubItem);

    if (render_all)
    {
        uint8_t row;
        uint8_t comp;
        uint8_t subitem;

        // Rows below current row?
        row = CurrentRow;
        comp = CurrentComponent;
        subitem = CurrentSubItem;

        while ((row + 1) < LCD_ROWS)
        {
            // Lets render next row
            row++;

            // Go to next menu item
            if (getNextMenuItem(&comp, &subitem))
            {
                // It exists
                renderRow(row, comp, subitem);
            }
            else
            {
                // We exceeded bottom of menu
                writeBlankRow(row);
            }
        }

        // Rows above current row?
        row = CurrentRow;
        comp = CurrentComponent;
        subitem = CurrentSubItem;

        while (row >= 1)
        {
            // Lets render above row
            row--;

            // Go to previous menu item
            if (subitem > 0)
            {
                subitem--;
                renderRow(row, comp, subitem);
            }
            else if (comp >= 1)
            {
                comp--;
                subitemcount = menuents_getSubCount(comp);
                subitem = subitemcount;
                renderRow(row, comp, subitem);
            }
            else
            {
                // We exceeded top of menu
                writeBlankRow(row);
            }
        }
    }
}

static void setCursor(uint8_t row, uint8_t column)
{
    // Place correct row cursor
    uint8_t i;

    for (i = 0u; i < LCD_ROWS; i++)
    {
        lcd_setCursor(i, 0u);
        if (i == row)
        {
            lcd_write(MENU_CURSOR_CHAR);
        }
        else
        {
            lcd_write(' ');
        }
    }

    // Set cursor at requested spot
    lcd_setCursor(row, column);
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void filtermenu_userSelects(void);
void filtermenu_userBacks(void);


void filtermenu_initialize(void)
{
    CurrentRow = 1;
    CurrentComponent = 1;
    CurrentSubItem = 0;
    CurrentEditMode = 0;

    NeedRedraw = NEED_REDRAW_ALL;
}

void filtermenu_draw(uint8_t mask_line)
{
    // TODO maskline

    if (NeedRedraw == NEED_REDRAW_ALL)
    {
        render(TRUE);
        setCursor(CurrentRow, 0u);
    }
    else if (NeedRedraw == NEED_REDRAW_ROW)
    {
        render(FALSE);
        setCursor(CurrentRow, 0u);
    }
}

void filtermenu_userTurns(int8_t delta, bool_t pushed)
{
    uint8_t event;

    event = pushed ? MENU_EVENT_TURN_PUSH : MENU_EVENT_TURN;

    if (CurrentEditMode == 0)
    {
        // Moving cursor at root level
        while (delta > 0)
        {
            if (getNextMenuItem(&CurrentComponent, &CurrentSubItem))
            {
                // Successfully moved one down
                if ((CurrentRow + 2) < LCD_ROWS)
                {
                    CurrentRow++;
                }
            }

            delta--;
        }

        while (delta < 0)
        {
            if (getPreviousMenuItem(&CurrentComponent, &CurrentSubItem))
            {
                // Successfully moved one up
                if (CurrentRow > 1)
                {
                    CurrentRow--;
                }
            }

            // Allow selecting row 0 on top menu item
            if (CurrentComponent == 0)
            {
                CurrentRow = 0;
            }

            delta++;
        }

        // Render all TODO maybe we just need to move cursor
        render(TRUE);
        setCursor(CurrentRow, 0u);
    }
    else
    {
        uint8_t ret;

        // Editing stuff, let component handle event
        ret = menuents_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode, event, delta);

        // Notify presets module that component may have changed its configuration
        presets_notifyConfigMayChange(CurrentComponent);

        // Update this line or all
        render(ret & MENU_UPDATE_ALL);

        // Set cursor
        if (CurrentSubItem == 0)
        {
            setCursor(CurrentRow, (ret & MENU_SET_CURSOR_MASK) + 1);
        }
        else
        {
            setCursor(CurrentRow, (ret & MENU_SET_CURSOR_MASK) + 2);
        }


    }
}

void menu_handleUserSelects(void)
{
    if (ComponentExclusive)
    {
        // Let component handle event
        menuents_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode,
                        MENU_EVENT_SELECT, 0u);
    }
    else
    {
        uint8_t ret;

        // Send the select command to component
        ret = menuents_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode,
                              MENU_EVENT_SELECT, 0u);

        // We advance edit mode if this is accepted by component
        CurrentEditMode++;

        if (ret == MENU_RESERVE_DISPLAY)
        {
            // Component wants to take over the display
            ComponentExclusive = TRUE;
        }
        else if ((ret & MENU_SET_CURSOR_MASK) == MENU_EDIT_MODE_UNAVAIL)
        {
            // OK, back out to root mode
            CurrentEditMode = 0;

            // Update this line or all
            render(ret & MENU_UPDATE_ALL);
            setCursor(CurrentRow, 0u);
        }
        else
        {
            // Update this line or all
            render(ret & MENU_UPDATE_ALL);

            // Set cursor
            if (CurrentSubItem == 0)
            {
                setCursor(CurrentRow, (ret & MENU_SET_CURSOR_MASK) + 1);
            }
            else
            {
                setCursor(CurrentRow, (ret & MENU_SET_CURSOR_MASK) + 2);
            }
        }
    }
}


void menu_handleUserBacks(void)
{
    // Let component know back was pushed
    menuents_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode,
                    MENU_EVENT_BACK, 0u);

    CurrentEditMode = 0;

    // Render all
    render(TRUE);

    // Set cursor
    setCursor(CurrentRow, 0u);
}

