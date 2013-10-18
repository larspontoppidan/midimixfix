
//
// Filename    : ui.c
// Code module : User interface (view and controller)
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

// ------------------------------  DESCRIPTION  ---------------------------------
//
// The ui module is first to process user interface events (knob turns, buttons)
//
//
// This module provides the overall menu roll up / down select, back functionality
// This functionality is used for all display screens, except for exclusive screens,
// such as midilog.
//
// Basically, a menu is something that can:
//   Draw a line on the screen (0 is top line, 3 is bottom)
//   Has a cursor concept
//   Handle user events
//
//


// The module implements a number of straightforward rolling menus, and can
// hand over the control of the user interface to another module, such as
// the filtermenu module.
//


// -------------------------------  INCLUDES  -----------------------------------

#include "ui.h"
#include "hal.h"
#include "common.h"
#include "util.h"
#include "lcd.h"
#include "errors.h"
#include "menuinterface.h"
#include "menus/mainmenu.h"

#include <avr/pgmspace.h>
#include <string.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------



// ----------------------------  LOCAL VARIABLES  -------------------------------


// State:

static const menuInterface_t *Menu;

// Two levels of menu parent
static const menuInterface_t *MenuParent;
static const menuInterface_t *MenuParentParent;

static int8_t MenuOffset;
static int8_t MenuCursorItem;


// ------------------------------  PROTOTYPES  ----------------------------------

static void drawAll(void);
static void drawCursor(uint8_t row, uint8_t col);

// Wrappers for accessing progmem stuff in menuInterface:
static inline bool_t  menuHasStaticTitle(const menuInterface_t *Menu);
static inline uint8_t menuInitGetCursor(const menuInterface_t *Menu);
static inline uint8_t menuGetItemCount(const menuInterface_t *Menu);
static inline void    menuDrawItem(const menuInterface_t *Menu, uint8_t item);
static inline void    menuHandleUiEvent(const menuInterface_t *Menu, uint8_t uiEvent);

// --------------------------  PRIVATE FUNCTIONS  -------------------------------

static inline bool_t menuHasStaticTitle(const menuInterface_t *Menu)
{
    return (bool_t)(pgm_read_byte(&(Menu->hasStaticTitle)));
}

static inline uint8_t menuInitGetCursor(const menuInterface_t *Menu)
{
    fptrUint8Void_t initGetCursor = (fptrUint8Void_t)pgm_read_word(&(Menu->initGetCursor));
    return (initGetCursor)();
}

static inline uint8_t menuGetItemCount(const menuInterface_t *Menu)
{
    fptrUint8Void_t getItemCount = (fptrUint8Void_t)pgm_read_word(&(Menu->getItemCount));
    return (getItemCount)();
}

static inline void menuDrawItem(const menuInterface_t *Menu, uint8_t item)
{
    fptrVoidUint8_t drawItem = (fptrVoidUint8_t)pgm_read_word(&(Menu->drawItem));
    (drawItem)(item);
}

static inline void menuHandleUiEvent(const menuInterface_t *Menu, uint8_t uiEvent)
{
    fptrVoidUint8_t handleUiEvent = (fptrVoidUint8_t)pgm_read_word(&(Menu->handleUiEvent));
    (handleUiEvent)(uiEvent);
}


static void drawAll(void)
{
    uint8_t i;

    // The LCD's four rows:
    //   row0: Menu title (always), menu_item = 0
    //   row1: First visible menu item, menu_item = 1, if MenuOffset = 0
    //   row2: Next  visible menu item, menu_item = 2, if MenuOffset = 0
    //   row3: Third visible menu item, menu_item = 3, if MenuOffset = 0

    // Now, if we have some offset:
    //   row1: menu_item = 2, if MenuOffset = 1

    // Cursor position refers to menu_item being pointed at

    if (menuHasStaticTitle(Menu))
    {
        // Title row is fixed
        menuDrawItem(Menu, 0);

        i = 1;
    }
    else
    {
        i = 0;
    }

    for (; i < LCD_ROWS; i++)
    {
        menuDrawItem(Menu, i + MenuOffset);
    }
}

static void drawCursor(uint8_t row, uint8_t col)
{
    uint8_t i;

    if (menuHasStaticTitle(Menu))
    {
        i = 1;
    }
    else
    {
        i = 0;
    }

    for (; i < LCD_ROWS; i++)
    {
        lcd_setCursor(i, 0);
        lcd_write((i == row) ? LCD_CHAR_RIGHTARROW : ' ');
    }

    // Set the visible cursor correctly
    lcd_setCursor(row, col);
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------



// This will reset variables and display main menu
void ui_initialize(void)
{
    // Reset variables
    MenuOffset = 0;
    MenuCursorItem = 0;

    MenuParent = NULL;
    MenuParentParent = NULL;

    // We want main menu hooked up initially
    ui_menuEnter(&mainmenu_Menu);
}


void ui_handleUserEvent(uint8_t user_event)
{
    // Pass on user event to currently active menu

    if (Menu != NULL)
    {
        menuHandleUiEvent(Menu, user_event);
    }
    else
    {
        // TODO check this other places also
        err_raise(ERR_MODULE_UI, __LINE__);
    }
}


// When entering a menu, ui_menuEnter must be called with the new menu manifest.
// This will make ui render the menu.
void ui_menuEnter(const menuInterface_t *menu)
{
    // Store pointer to menu, keeping two parents

    if (MenuParentParent != NULL)
    {
        // We are nesting too deep!
        err_raise(ERR_MODULE_UI, __LINE__);
    }

    MenuParentParent = MenuParent;
    MenuParent = Menu;
    Menu = menu;

    // Set an off-limits menu offset, and get new menu's default cursor
    MenuOffset = -1;

    // MoveCursor function will fix everything and redraw current view now
    ui_menuMoveCursor(menuInitGetCursor(Menu), 0);
}

// Leave current menu. Let parent menu get in focus
void ui_menuBackOut(void)
{
    // Store pointer to menu, keeping two parents
    Menu = MenuParent;
    MenuParent = MenuParentParent;
    MenuParentParent = NULL;

    // Set am off-limits menu offset, and get new menu's default cursor
    MenuOffset = -1;

    // MoveCursor function will fix everything anMenuCursorItemd redraw current view now
    ui_menuMoveCursor(menuInitGetCursor(Menu), 0);
}

// Make sure cursorItem is visible, and place cursor correctly
void ui_menuMoveCursor(uint8_t cursorItem, uint8_t cursorIndent)
{
    uint8_t itemCount;
    bool_t needRedraw = FALSE;

    // Get number of items
    itemCount = menuGetItemCount(Menu);

    // Bounds check cursor position
    if (cursorItem >= itemCount)
    {
        cursorItem = itemCount - 1;
    }
    else if (((int8_t)cursorItem) < 0)
    {
        cursorItem = 0;
    }

    // Load desired cursor position
    MenuCursorItem = cursorItem;

    // Is MenuOffset in range?
    if (MenuOffset < 0)
    {
        MenuOffset = 0;
        needRedraw = TRUE;
    }

    // Is cursorItem visible?

    if (menuHasStaticTitle(Menu))
    {
        if (cursorItem < (MenuOffset+1))
        {
            // We must move offset
            needRedraw = TRUE;
            MenuOffset = cursorItem - 1;
        }
        else if (cursorItem > (MenuOffset + 3))
        {
            needRedraw = TRUE;
            MenuOffset = cursorItem - 3;
        }
    }
    else
    {
        if (cursorItem < MenuOffset)
        {
            // We must move offset
            needRedraw = TRUE;
            MenuOffset = cursorItem;
        }
        else if (cursorItem > (MenuOffset + 3))
        {
            needRedraw = TRUE;
            MenuOffset = cursorItem - 3;
        }
    }

    if (needRedraw)
    {
        drawAll();
    }

    drawCursor(MenuCursorItem - MenuOffset, cursorIndent);
}


void ui_menuDrawItem(uint8_t item, uint8_t const *data)
{
    uint8_t row;
    uint8_t col;

    // A what LCD row is this?
    if (menuHasStaticTitle(Menu) && (item == 0))
    {
        row = 0;
        col = 0;
    }
    else
    {
        row = item - MenuOffset;
        col = 1;
    }

    // Is this visible?
    if (row < LCD_ROWS)
    {
        uint8_t i;

        // Blit to LCD
        lcd_setCursor(row, col);

        for (i = 0u; i <  (LCD_COLUMNS-col); i++)
        {
            if (data[i] != 0)
            {
                lcd_write(data[i]);
            }
            else
            {
                break;
            }
        }

        // Make sure we blank out rest of line
        for (; i < (LCD_COLUMNS-col); i++)
        {
            lcd_write(' ');
        }
    }

}

void ui_menuDrawItemP(uint8_t item, const char *data)
{
    uint8_t row, col;

    // A what LCD row is this?
    if (menuHasStaticTitle(Menu) && (item == 0))
    {
        row = 0;
        col = 0;
    }
    else
    {
        row = item - MenuOffset;
        col = 1;
    }

    // Is this visible?
    if (row < LCD_ROWS)
    {
        uint8_t i;

        // Blit to LCD
        lcd_setCursor(row, col);

        for (i = 0u; i <  (LCD_COLUMNS-col); i++)
        {
            uint8_t x = pgm_read_byte( &(data[i]) );

            if (x != 0)
            {
                lcd_write(x);
            }
            else
            {
                break;
            }
        }

        // Make sure we blank out rest of line
        for (; i <  (LCD_COLUMNS-col); i++)
        {
            lcd_write(' ');
        }
    }

}


int8_t ui_eventToDelta(uint8_t ui_event, int8_t fast_speed)
{
    int8_t delta = 0;

    switch (ui_event)
    {
    case UI_EVENT_MOVE_UP:
        delta = 1;
        break;
    case UI_EVENT_MOVE_DOWN:
        delta = -1;
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        delta = fast_speed;
        break;

    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        delta = -fast_speed;
        break;
    }

    return delta;
}
