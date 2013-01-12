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
#include <string.h>

//////////// DEFINES

#define NOT_ACTIVE 0xFF

#define MENU_INDENT_CHAR  ' '
#define MENU_CURSOR_CHAR  LCD_RIGHTARROW


//////////// VARIABLES


// Menu state:

// A component may take exclusive LCD access, then componentExclusive is TRUE
bool_t componentExclusive;

// Current row highlighted on display, 0 to 3:
uint8_t menuRow;

// The component providing stuff for currently selected menu item:
uint8_t menuComponent;

// The subitem of the component's menu currently active:
uint8_t menuSubItem;  // 0 means title item, 1 means subitem 0, ...

// Edit mode, if any:
uint8_t menuEditMode;

uint8_t lineBuffer[LCD_COLUMNS + 5];

//////////// PROTOTYPES

static void RenderRow(uint8_t row, uint8_t component, uint8_t subitem);
static void BlankRow(uint8_t row);
static bool_t GetPreviousMenuItem(uint8_t *comp, uint8_t *subitem);
static bool_t GetNextMenuItem(uint8_t *comp, uint8_t *subitem);
static void Render(bool_t render_all);
static void SetCursor(uint8_t row, uint8_t column);

//////////// IMPLEMENTATION PRIVATE FUNCTIONS

static void RenderRow(uint8_t row, uint8_t component, uint8_t subitem)
{
    uint8_t i;

    // Clear line buffer
    memset(lineBuffer, 0u, LCD_COLUMNS);

    // Ask component to write into linebuffer at some offset
    if (subitem == 0u)
    {
        menue_GetMenuText(component, (char*) &(lineBuffer[1u]), subitem);
    }
    else
    {
        lineBuffer[1u] = MENU_INDENT_CHAR;
        menue_GetMenuText(component, (char*) &(lineBuffer[2u]), subitem);
    }

    lcd_SetCursor(row, 1u);

    for (i = 1u; i < LCD_COLUMNS; i++)
    {
        if (lineBuffer[i] != 0)
        {
            lcd_Write(lineBuffer[i]);
        }
        else
        {
            lcd_Write(' ');
        }
    }
}

static void BlankRow(uint8_t row)
{
    lcd_SetCursor(row, 1u);
    lcd_WriteRepeat(' ', LCD_COLUMNS - 1);
}

static bool_t GetPreviousMenuItem(uint8_t *comp, uint8_t *subitem)
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
        *subitem = menue_GetSubMenuCount(*comp);
        success = TRUE;
    }
    else
    {
        success = FALSE;
    }

    return success;
}

static bool_t GetNextMenuItem(uint8_t *comp, uint8_t *subitem)
{
    bool_t success;
    uint8_t subitemcount;

    subitemcount = menue_GetSubMenuCount(*comp);

    if (*subitem < subitemcount)
    {
        (*subitem)++;
        success = TRUE;
    }
    else if (((*comp) + 1) < MENUE_ENTITY_COUNT)
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
static void Render(bool_t render_all)
{
    uint8_t subitemcount;

    // Start with current row. Is the menu item even existing?

    subitemcount = menue_GetSubMenuCount(menuComponent);

    if (subitemcount < menuSubItem)
    {
        // No! Back into what's available in selected component
        menuSubItem = subitemcount;

        // Menu has changed, we must ensure all is rendered
        render_all = TRUE;
    }

    // Render current row
    RenderRow(menuRow, menuComponent, menuSubItem);

    if (render_all)
    {
        uint8_t row;
        uint8_t comp;
        uint8_t subitem;

        // Rows below current row?
        row = menuRow;
        comp = menuComponent;
        subitem = menuSubItem;

        while ((row + 1) < LCD_ROWS)
        {
            // Lets render next row
            row++;

            // Go to next menu item
            if (GetNextMenuItem(&comp, &subitem))
            {
                // It exists
                RenderRow(row, comp, subitem);
            }
            else
            {
                // We exceeded bottom of menu
                BlankRow(row);
            }
        }

        // Rows above current row?
        row = menuRow;
        comp = menuComponent;
        subitem = menuSubItem;

        while (row >= 1)
        {
            // Lets render above row
            row--;

            // Go to previous menu item
            if (subitem > 0)
            {
                subitem--;
                RenderRow(row, comp, subitem);
            }
            else if (comp >= 1)
            {
                comp--;
                subitemcount = menue_GetSubMenuCount(comp);
                subitem = subitemcount;
                RenderRow(row, comp, subitem);
            }
            else
            {
                // We exceeded top of menu
                BlankRow(row);
            }
        }
    }
}

static void SetCursor(uint8_t row, uint8_t column)
{
    // Place correct row cursor
    uint8_t i;

    for (i = 0u; i < LCD_ROWS; i++)
    {
        lcd_SetCursor(i, 0u);
        if (i == row)
        {
            lcd_Write(MENU_CURSOR_CHAR);
        }
        else
        {
            lcd_Write(' ');
        }
    }

    // Set cursor at requested spot
    lcd_SetCursor(row, column);
}

//////////// IMPLEMENTATION PUBLIC FUNCTIONS


void menu_Initialize(void)
{
    componentExclusive = FALSE;
    menuRow = 1;
    menuComponent = 1;
    menuSubItem = 0;
    menuEditMode = 0;

    // Render all
    Render(TRUE);

    // Set cursor
    SetCursor(menuRow, 0u);
}

void menu_UserTurns(int8_t delta, bool_t pushed)
{
    uint8_t event;

    event = pushed ? MENU_EVENT_TURN_PUSH : MENU_EVENT_TURN;

    if (componentExclusive)
    {
        // Let component handle event
        menue_MenuEvent(menuComponent, menuSubItem, menuEditMode, event, delta);
    }
    else
    {
        if (menuEditMode == 0)
        {
            // Moving cursor at root level
            while (delta > 0)
            {
                if (GetNextMenuItem(&menuComponent, &menuSubItem))
                {
                    // Successfully moved one down
                    if ((menuRow + 2) < LCD_ROWS)
                    {
                        menuRow++;
                    }
                }

                delta--;
            }

            while (delta < 0)
            {
                if (GetPreviousMenuItem(&menuComponent, &menuSubItem))
                {
                    // Successfully moved one up
                    if (menuRow > 1)
                    {
                        menuRow--;
                    }
                }

                // Allow selecting row 0 on top menu item
                if (menuComponent == 0)
                {
                    menuRow = 0;
                }

                delta++;
            }

            // Render all TODO maybe we just need to move cursor
            Render(TRUE);
            SetCursor(menuRow, 0u);
        }
        else
        {
            uint8_t ret;

            // Editing stuff, let component handle event
            ret = menue_MenuEvent(menuComponent, menuSubItem, menuEditMode, event, delta);

            // Update this line or all
            Render(ret & MENU_UPDATE_ALL);

            // Set cursor
            if (menuSubItem == 0)
            {
                SetCursor(menuRow, (ret & MENU_SET_CURSOR_MASK) + 1);
            }
            else
            {
                SetCursor(menuRow, (ret & MENU_SET_CURSOR_MASK) + 2);
            }
        }
    }
}

void menu_UserSelects(void)
{
    if (componentExclusive)
    {
        // Let component handle event
        menue_MenuEvent(menuComponent, menuSubItem, menuEditMode, MENU_EVENT_SELECT, 0u);
    }
    else
    {
        uint8_t ret;

        // Send the select command to component
        ret = menue_MenuEvent(menuComponent, menuSubItem, menuEditMode, MENU_EVENT_SELECT, 0u);

        // We advance edit mode if this is accepted by component
        menuEditMode++;

        if (ret == MENU_RESERVE_DISPLAY)
        {
            // Component wants to take over the display
            componentExclusive = TRUE;
        }
        else if ((ret & MENU_SET_CURSOR_MASK) == MENU_EDIT_MODE_UNAVAIL)
        {
            // OK, back out to root mode
            menuEditMode = 0;

            // Update this line or all
            Render(ret & MENU_UPDATE_ALL);
            SetCursor(menuRow, 0u);
        }
        else
        {
            // Update this line or all
            Render(ret & MENU_UPDATE_ALL);

            // Set cursor
            if (menuSubItem == 0)
            {
                SetCursor(menuRow, (ret & MENU_SET_CURSOR_MASK) + 1);
            }
            else
            {
                SetCursor(menuRow, (ret & MENU_SET_CURSOR_MASK) + 2);
            }
        }
    }
}


void menu_UserBacks(void)
{
    // Let component know back was pushed
    menue_MenuEvent(menuComponent, menuSubItem, menuEditMode, MENU_EVENT_BACK, 0u);

    // Back button means we exit the exclusive mode
    componentExclusive = FALSE;

    menuEditMode = 0;

    // Render all
    Render(TRUE);

    // Set cursor
    SetCursor(menuRow, 0u);
}



