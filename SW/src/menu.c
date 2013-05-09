/*
 * menu.c
 *
 *  Created on: Dec 12, 2012
 *      Author: lars
 */


#include "common.h"
#include "menu.h"
#include "lcd.h"
#include "menuentities.h"
#include "presets.h"
#include "errors.h"
#include <string.h>

//////////// DEFINES

#define NOT_ACTIVE 0xFF

#define MENU_INDENT_CHAR  ' '
#define MENU_CURSOR_CHAR  LCD_CHAR_RIGHTARROW


//////////// VARIABLES


// Menu state:

// A component may take exclusive LCD access, then componentExclusive is TRUE
static bool_t ComponentExclusive;

// Current row highlighted on display, 0 to 3:
static uint8_t CurrentRow;

// The component providing stuff for currently selected menu item:
static uint8_t CurrentComponent;

// The subitem of the component's menu currently active:
static uint8_t CurrentSubItem;  // 0 means title item, 1 means subitem 0, ...

// Edit mode, if any:
static uint8_t CurrentEditMode;

static uint8_t LineBuffer[LCD_COLUMNS + 5];

static volatile bool_t RefreshRequested;

//////////// PROTOTYPES

static void renderRow(uint8_t row, uint8_t component, uint8_t subitem);
static void writeBlankRow(uint8_t row);
static bool_t getPreviousMenuItem(uint8_t *comp, uint8_t *subitem);
static bool_t getNextMenuItem(uint8_t *comp, uint8_t *subitem);
static void render(bool_t render_all);
static void setCursor(uint8_t row, uint8_t column);

//////////// IMPLEMENTATION PRIVATE FUNCTIONS

static void renderRow(uint8_t row, uint8_t component, uint8_t subitem)
{
    uint8_t i;

    // Clear line buffer
    memset(LineBuffer, 0u, LCD_COLUMNS);

    // Ask component to write into linebuffer at some offset
    if (subitem == 0u)
    {
        MenuEnts_getText(component, (char*) &(LineBuffer[1u]), subitem);
    }
    else
    {
        LineBuffer[1u] = MENU_INDENT_CHAR;
        MenuEnts_getText(component, (char*) &(LineBuffer[2u]), subitem);
    }

    Lcd_setCursor(row, 1u);

    for (i = 1u; i < LCD_COLUMNS; i++)
    {
        if (LineBuffer[i] != 0)
        {
            Lcd_write(LineBuffer[i]);
        }
        else
        {
            Lcd_write(' ');
        }
    }
}

static void writeBlankRow(uint8_t row)
{
    Lcd_setCursor(row, 1u);
    Lcd_writeRepeat(' ', LCD_COLUMNS - 1);
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
        *subitem = MenuEnts_getSubCount(*comp);
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

    subitemcount = MenuEnts_getSubCount(*comp);

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

    subitemcount = MenuEnts_getSubCount(CurrentComponent);

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
                subitemcount = MenuEnts_getSubCount(comp);
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
        Lcd_setCursor(i, 0u);
        if (i == row)
        {
            Lcd_write(MENU_CURSOR_CHAR);
        }
        else
        {
            Lcd_write(' ');
        }
    }

    // Set cursor at requested spot
    Lcd_setCursor(row, column);
}

//////////// IMPLEMENTATION PUBLIC FUNCTIONS


void Menu_initialize(void)
{
    ComponentExclusive = FALSE;
    CurrentRow = 1;
    CurrentComponent = 1;
    CurrentSubItem = 0;
    CurrentEditMode = 0;
    RefreshRequested = FALSE;

    // Render all
    render(TRUE);

    // Set cursor
    setCursor(CurrentRow, 0u);
}


void Menu_notifyRefresh_SAFE(void)
{
    RefreshRequested = TRUE;
}

void Menu_handleMainLoop(void)
{
    if (RefreshRequested == TRUE)
    {
        RefreshRequested = FALSE;

        render(TRUE);
        setCursor(CurrentRow, 0u);
    }
}

void Menu_handleUserTurns(int8_t delta, bool_t pushed)
{
    uint8_t event;

    event = pushed ? MENU_EVENT_TURN_PUSH : MENU_EVENT_TURN;

    if (ComponentExclusive)
    {
        // Let component handle event
        MenuEnts_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode, event, delta);
    }
    else
    {
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
            ret = MenuEnts_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode, event, delta);

            // Notify presets module that component may have changed its configuration
            Presets_notifyConfigMayChange(CurrentComponent);

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

void Menu_handleUserSelects(void)
{
    if (ComponentExclusive)
    {
        // Let component handle event
        MenuEnts_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode,
                        MENU_EVENT_SELECT, 0u);
    }
    else
    {
        uint8_t ret;

        // Send the select command to component
        ret = MenuEnts_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode,
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


void Menu_handleUserBacks(void)
{
    // Let component know back was pushed
    MenuEnts_handleEvent(CurrentComponent, CurrentSubItem, CurrentEditMode,
                    MENU_EVENT_BACK, 0u);

    // Back button means we exit the exclusive mode
    ComponentExclusive = FALSE;

    CurrentEditMode = 0;

    // Render all
    render(TRUE);

    // Set cursor
    setCursor(CurrentRow, 0u);
}

