
//
// Filename    : filterorganize.c
// Code module : Add/Remove/Reorder filter menu implementation
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
#include "../ui.h"
#include <avr/pgmspace.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------


// ----------------------------  LOCAL VARIABLES  -------------------------------


// ------------------------------  PROTOTYPES  ----------------------------------


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------























// Preset menu:

static const char PresetLoadTitle[] PROGMEM = "--- LOAD Preset: ---";
static const char PresetSaveTitle[] PROGMEM = "--- SAVE Preset: ---";

static const char PresetItem[] PROGMEM = "Preset %i";

#define PRESET_MENU_SIZE 10


// Organize filter menus:

static const char AddFilterTitle[] PROGMEM =     "--- ADD Filter: ----";
static const char RemoveFilterTitle[] PROGMEM =  "--- REMOVE Filter: -";
static const char ReorderFilterTitle[] PROGMEM = "--- REORDER Filters:";

// ----------------------------  LOCAL VARIABLES  -------------------------------

static uint8_t UiMode;

static int8_t MenuOffset;
static int8_t MenuSelected;


static uint8_t LineBuffer[LCD_COLUMNS + 2];


// ------------------------------  PROTOTYPES  ----------------------------------

// Generic menu public functions:
static void menuSetup(void);
static void menuMoveCursor(uint8_t user_event);

// Generic menu private functions:
static void menuDrawAll(void);
static void menuDrawRow(uint8_t row, uint8_t menu_item);
static void menuDrawCursor(uint8_t row);
static uint8_t menuGetLength(void);

// Main menu functions:
static void mainMenuEvent(uint8_t user_event);
static void mainMenuDraw(uint8_t menu_item, uint8_t *dest);

// Preset menu functions:
static uint8_t presetMenuEvent(uint8_t user_event);
static void presetMenuDraw(uint8_t menu_item, uint8_t *dest);

// Add filter menu
static uint8_t addFilterMenuEvent(uint8_t user_event);
static void addFilterMenuDraw(uint8_t menu_item, uint8_t *dest);

// Organize filter menu
static uint8_t orgFilterMenuEvent(uint8_t user_event);
static void orgFilterMenuDraw(uint8_t menu_item, uint8_t *dest);

static void jumpBootloader(void);

// ---------------------------  PRIVATE FUNCTIONS  ------------------------------

// Generic menu functions
// ----------------------

// Setup the current menu
static void menuSetup(void)
{
    MenuOffset = 0;   // 0 means menu is at top
    MenuSelected = 1; // 1 means first item is selected

    menuDrawAll();
}

// Deal with up/down user event, moving cursor
static void menuMoveCursor(uint8_t user_event)
{
    // Update menu selected variable
    switch (user_event)
    {
    case UI_EVENT_MOVE_UP:
        if (MenuSelected < menuGetLength())
        {
            MenuSelected++;
        }
        break;
    case UI_EVENT_MOVE_DOWN:
        if (MenuSelected > 1)
        {
            MenuSelected--;
        }
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        MenuSelected = menuGetLength();
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        MenuSelected = 1;
        break;
    }

    // Is the selection over whats currently visible?
    if ((MenuSelected - MenuOffset) < 1)
    {
        // Move up
        MenuOffset =  MenuSelected - 1;

        // Then we have to draw all
        menuDrawAll();
    }
    else if ((MenuSelected - MenuOffset) > 3)
    {
        // Move down
        MenuOffset = MenuSelected - 3;


        // Title row is fixed
        menuDrawRow(0, 0);

        // The three menu rows depend on offset / selection mathematics
        for (i = 1; i < LCD_ROWS; i++)
        {
            menuDrawRow(i, i + MenuOffset);
        }

        // Place cursor
        menuDrawCursor(MenuSelected - MenuOffset);        // Then we have to draw all
        menuDrawAll();
    }
    else
    {
        // Selection is visible, just move the cursor
        menuDrawCursor(MenuSelected - MenuOffset);
    }
}

// Render complete screen of menu, including positioning cursor
static void menuDrawAll(void)
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


    // Title row is fixed
    menuDrawRow(0, 0);

    // The three menu rows depend on offset / selection mathematics
    for (i = 1; i < LCD_ROWS; i++)
    {
        menuDrawRow(i, i + MenuOffset);
    }

    // Place cursor
    menuDrawCursor(MenuSelected - MenuOffset);
}

// Write right arrow at correct cursor location and set correct LCD cursor
static void menuDrawCursor(uint8_t row)
{
    uint8_t i;

    // Write right arrow at correct cursor location

    for (i = 1; i < LCD_ROWS; i++)
    {
        lcd_setCursor(i, MENU_CURSOR_INDENT);
        lcd_write((i == row) ? LCD_CHAR_RIGHTARROW : ' ');
    }

    // Set the visible cursor correctly
    lcd_setCursor(row, MENU_CURSOR_INDENT);
}

// Fill linebuffer and blit to LCD with row contents of menu
static void menuDrawRow(uint8_t row, uint8_t menu_item)
{
    uint8_t i;

    // Clear line buffer
    memset(LineBuffer, 0u, LCD_COLUMNS);

    // Get contents
    switch (UiMode)
    {
    case MODE_MAIN_MENU:
        mainMenuDraw(menu_item, LineBuffer);
        break;
    case MODE_LOAD_PRESET:
    case MODE_SAVE_PRESET:
        presetMenuDraw(menu_item, LineBuffer);
        break;
    case MODE_ADD_FILT:
        addFilterMenuDraw(menu_item, LineBuffer);
        break;
    case MODE_REMOVE_FILT:
        orgFilterMenuDraw(menu_item, LineBuffer);
        break;
    }

    // Blit to LCD
    lcd_setCursor(row, 0u);

    for (i = 0u; i < LCD_COLUMNS; i++)
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

// What is length of current menu
static uint8_t menuGetLength(void)
{
    uint8_t ret = 0;

    switch (UiMode)
    {
    case MODE_MAIN_MENU:
        ret = MAIN_MENU_SIZE;
        break;
    case MODE_LOAD_PRESET:
    case MODE_SAVE_PRESET:
        ret = PRESET_MENU_SIZE;
        break;
    case MODE_ADD_FILT:
        ret = FILTER_TYPE_COUNT;
        break;
    case MODE_REMOVE_FILT:
        ret = fsteps_getCount_SAFE();
        break;
    }

    return ret;
}

// Main menu specific
// ------------------

// Handle select
static void mainMenuEvent(uint8_t user_event)
{
    if (user_event == UI_EVENT_SELECT)
    {
        // Use the fact that the modes map into menu items directly:
        UiMode = MenuSelected;

        switch (UiMode)
        {
        case MODE_FILTERS:
            // Let filter menu setup
            break;

        case MODE_BOOTLOADER:
            jumpBootloader();
            break;

        case MODE_LOAD_PRESET:
        case MODE_SAVE_PRESET:
        case MODE_ADD_FILT:
        case MODE_REMOVE_FILT:
        //case MODE_REORG_FILT:
            // For the other menus, we handle the setup
            menuSetup();
            break;

        default:
            break;
        }
    }
    else if (user_event == UI_EVENT_BACK)
    {
        // Can't back out of main menu
    }
    else
    {
        menuMoveCursor(user_event);
    }
}

static void mainMenuDraw(uint8_t menu_item, uint8_t *dest)
{
    if (menu_item == 0)
    {
        // Title
        util_copyString_P((char*)dest, MainTitle);
    }
    else
    {
        // Menu item
        util_copyString_P((char*)(dest + MENU_ITEM_INDENT), MainItem[menu_item - 1].Text);
    }
}


// Preset save/load menu specific
// ------------------------------

static uint8_t presetMenuEvent(uint8_t user_event)
{
    uint8_t ret = UI_EVENT_NONE;

    if (user_event == UI_EVENT_SELECT)
    {
        // Actually save or load the preset
        if (UiMode == MODE_LOAD_PRESET)
        {
            // Load preset: MenuSelected
            ret = UI_EVENT_BACK;
        }
        else if (UiMode == MODE_SAVE_PRESET)
        {
            // Save preset: MenuSelected
            ret = UI_EVENT_BACK;
        }
    }
    else if (user_event == UI_EVENT_BACK)
    {
        // Back out of this menu
        ret = UI_EVENT_BACK;
    }
    else
    {
        // Move cursor
        menuMoveCursor(user_event);
    }

    return ret;
}

static void presetMenuDraw(uint8_t menu_item, uint8_t *dest)
{
    if (menu_item == 0)
    {
        // Title

        if (UiMode == MODE_LOAD_PRESET)
        {
            util_copyString_P((char*)dest, PresetLoadTitle);
        }
        else
        {
            util_copyString_P((char*)dest, PresetSaveTitle);
        }
    }
    else
    {
        // Menu item
        util_writeFormat_P((char*)(dest + MENU_ITEM_INDENT), PresetItem, menu_item);
    }
}



// Add menu specific
// ------------------------------

static uint8_t addFilterMenuEvent(uint8_t user_event)
{
    uint8_t ret = UI_EVENT_NONE;

    if (user_event == UI_EVENT_SELECT)
    {
        // Load preset: MenuSelected
        fsteps_addFilter_MAIN(MenuSelected - 1, NULL);

        // Back out of menu
        ret = UI_EVENT_BACK;

    }
    else if (user_event == UI_EVENT_BACK)
    {
        ret = UI_EVENT_BACK;
    }
    else
    {
        // Move cursor
        menuMoveCursor(user_event);
    }

    return ret;
}

static void addFilterMenuDraw(uint8_t menu_item, uint8_t *dest)
{
    if (menu_item == 0)
    {
        // Title
        util_copyString_P((char*)dest, AddFilterTitle);
    }
    else
    {
        // Menu item
        filter_getTitle(menu_item - 1, (char*)(dest + MENU_ITEM_INDENT));
    }
}


// Remove/Reorder menu specific
// ------------------------------

static uint8_t orgFilterMenuEvent(uint8_t user_event)
{
    uint8_t ret = UI_EVENT_NONE;

    if (user_event == UI_EVENT_SELECT)
    {
        if (UiMode == MODE_REMOVE_FILT)
        {
            // Load preset: MenuSelected
            fsteps_removeFilter_MAIN(MenuSelected - 1);

            // Back out of menu
            ret = UI_EVENT_BACK;
        }

    }
    else if (user_event == UI_EVENT_BACK)
    {
        ret = UI_EVENT_BACK;
    }
    else
    {
        // Move cursor
        menuMoveCursor(user_event);
    }

    return ret;
}

static void orgFilterMenuDraw(uint8_t menu_item, uint8_t *dest)
{
    if (menu_item == 0)
    {
        // Title
        if (UiMode == MODE_REMOVE_FILT)
        {
            util_copyString_P((char*)dest, RemoveFilterTitle);
        }
    }
    else
    {
        // Menu item
        filter_t *filter = fsteps_getFilter_SAFE(menu_item - 1);

        if (filter != NULL)
        {
            filter_getTitle(filter->FilterType, (char*)(dest + MENU_ITEM_INDENT));
        }
    }
}

// ------------------------------------

static void jumpBootloader(void)
{
    lcd_clear();
    lcd_writeString_P(PSTR("Bootloader activated"));
    hal_lcdBacklightSet(FALSE);
    hal_jumpBootloader();
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

void ui_initialize(void)
{
    UiMode = MODE_MAIN_MENU;

    menuSetup();
}

void ui_handleUserEvent(uint8_t user_event)
{
    uint8_t unhandled_event = user_event;

    switch (UiMode)
    {
    case MODE_MAIN_MENU:
        mainMenuEvent(user_event);
        break;
    case MODE_FILTERS:
        //unhandled_event = filtermenu_event(user_event);
        break;
    case MODE_LOAD_PRESET:
    case MODE_SAVE_PRESET:
        unhandled_event = presetMenuEvent(user_event);
        break;
    case MODE_ADD_FILT:
        unhandled_event = addFilterMenuEvent(user_event);
        break;
    case MODE_REMOVE_FILT:
    case MODE_REORG_FILT:
        unhandled_event = orgFilterMenuEvent(user_event);
        break;
    case MODE_MIDILOG_IN:
        break;
    case MODE_MIDILOG_OUT:
        break;
    case MODE_BOOTLOADER:
        break;
    case MODE_ABOUT:
        break;
    }

    // The menus may send back events that can't be handled in their context,
    // such as going back to parent menu:
    if (unhandled_event == UI_EVENT_BACK)
    {
        UiMode = MODE_MAIN_MENU;
        menuSetup();
    }

}
