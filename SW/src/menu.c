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
static bool_t componentExclusive;

// Current row highlighted on display, 0 to 3:
static uint8_t currentRow;

// The component providing stuff for currently selected menu item:
static uint8_t currentComponent;

// The subitem of the component's menu currently active:
static uint8_t currentSubItem;  // 0 means title item, 1 means subitem 0, ...

// Edit mode, if any:
static uint8_t currentEditMode;

static uint8_t lineBuffer[LCD_COLUMNS + 5];

static volatile bool_t refreshRequested;

//////////// PROTOTYPES

static void RenderRow(uint8_t row, uint8_t component, uint8_t subitem);
static void WriteBlankRow(uint8_t row);
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
        menue_GetText(component, (char*) &(lineBuffer[1u]), subitem);
    }
    else
    {
        lineBuffer[1u] = MENU_INDENT_CHAR;
        menue_GetText(component, (char*) &(lineBuffer[2u]), subitem);
    }

    lcd_CursorSet(row, 1u);

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

static void WriteBlankRow(uint8_t row)
{
    lcd_CursorSet(row, 1u);
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
        *subitem = menue_GetSubCount(*comp);
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

    subitemcount = menue_GetSubCount(*comp);

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
static void Render(bool_t render_all)
{
    uint8_t subitemcount;

    // Start with current row. Is the menu item even existing?

    subitemcount = menue_GetSubCount(currentComponent);

    if (subitemcount < currentSubItem)
    {
        // No! Back into what's available in selected component
        currentSubItem = subitemcount;

        // Menu has changed, we must ensure all is rendered
        render_all = TRUE;
    }

    // Render current row
    RenderRow(currentRow, currentComponent, currentSubItem);

    if (render_all)
    {
        uint8_t row;
        uint8_t comp;
        uint8_t subitem;

        // Rows below current row?
        row = currentRow;
        comp = currentComponent;
        subitem = currentSubItem;

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
                WriteBlankRow(row);
            }
        }

        // Rows above current row?
        row = currentRow;
        comp = currentComponent;
        subitem = currentSubItem;

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
                subitemcount = menue_GetSubCount(comp);
                subitem = subitemcount;
                RenderRow(row, comp, subitem);
            }
            else
            {
                // We exceeded top of menu
                WriteBlankRow(row);
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
        lcd_CursorSet(i, 0u);
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
    lcd_CursorSet(row, column);
}

//////////// IMPLEMENTATION PUBLIC FUNCTIONS


void menu_Initialize(void)
{
    componentExclusive = FALSE;
    currentRow = 1;
    currentComponent = 1;
    currentSubItem = 0;
    currentEditMode = 0;
    refreshRequested = FALSE;

    // Render all
    Render(TRUE);

    // Set cursor
    SetCursor(currentRow, 0u);
}


void menu_NotifyRefresh_SAFE(void)
{
    refreshRequested = TRUE;
}

void menu_HookMainLoop(void)
{
    if (refreshRequested == TRUE)
    {
        refreshRequested = FALSE;

        Render(TRUE);
        SetCursor(currentRow, 0u);
    }
}

void menu_UserTurns(int8_t delta, bool_t pushed)
{
    uint8_t event;

    event = pushed ? MENU_EVENT_TURN_PUSH : MENU_EVENT_TURN;

    if (componentExclusive)
    {
        // Let component handle event
        menue_HandleEvent(currentComponent, currentSubItem, currentEditMode, event, delta);
    }
    else
    {
        if (currentEditMode == 0)
        {
            // Moving cursor at root level
            while (delta > 0)
            {
                if (GetNextMenuItem(&currentComponent, &currentSubItem))
                {
                    // Successfully moved one down
                    if ((currentRow + 2) < LCD_ROWS)
                    {
                        currentRow++;
                    }
                }

                delta--;
            }

            while (delta < 0)
            {
                if (GetPreviousMenuItem(&currentComponent, &currentSubItem))
                {
                    // Successfully moved one up
                    if (currentRow > 1)
                    {
                        currentRow--;
                    }
                }

                // Allow selecting row 0 on top menu item
                if (currentComponent == 0)
                {
                    currentRow = 0;
                }

                delta++;
            }

            // Render all TODO maybe we just need to move cursor
            Render(TRUE);
            SetCursor(currentRow, 0u);
        }
        else
        {
            uint8_t ret;

            // Editing stuff, let component handle event
            ret = menue_HandleEvent(currentComponent, currentSubItem, currentEditMode, event, delta);

            // Notify presets module that component may have changed its configuration
            presets_configMayChangeNotify(currentComponent);

            // Update this line or all
            Render(ret & MENU_UPDATE_ALL);

            // Set cursor
            if (currentSubItem == 0)
            {
                SetCursor(currentRow, (ret & MENU_SET_CURSOR_MASK) + 1);
            }
            else
            {
                SetCursor(currentRow, (ret & MENU_SET_CURSOR_MASK) + 2);
            }


        }
    }
}

void menu_UserSelects(void)
{
    if (componentExclusive)
    {
        // Let component handle event
        menue_HandleEvent(currentComponent, currentSubItem, currentEditMode,
                        MENU_EVENT_SELECT, 0u);
    }
    else
    {
        uint8_t ret;

        // Send the select command to component
        ret = menue_HandleEvent(currentComponent, currentSubItem, currentEditMode,
                              MENU_EVENT_SELECT, 0u);

        // We advance edit mode if this is accepted by component
        currentEditMode++;

        if (ret == MENU_RESERVE_DISPLAY)
        {
            // Component wants to take over the display
            componentExclusive = TRUE;
        }
        else if ((ret & MENU_SET_CURSOR_MASK) == MENU_EDIT_MODE_UNAVAIL)
        {
            // OK, back out to root mode
            currentEditMode = 0;

            // Update this line or all
            Render(ret & MENU_UPDATE_ALL);
            SetCursor(currentRow, 0u);
        }
        else
        {
            // Update this line or all
            Render(ret & MENU_UPDATE_ALL);

            // Set cursor
            if (currentSubItem == 0)
            {
                SetCursor(currentRow, (ret & MENU_SET_CURSOR_MASK) + 1);
            }
            else
            {
                SetCursor(currentRow, (ret & MENU_SET_CURSOR_MASK) + 2);
            }
        }
    }
}


void menu_UserBacks(void)
{
    // Let component know back was pushed
    menue_HandleEvent(currentComponent, currentSubItem, currentEditMode,
                    MENU_EVENT_BACK, 0u);

    // Back button means we exit the exclusive mode
    componentExclusive = FALSE;

    currentEditMode = 0;

    // Render all
    Render(TRUE);

    // Set cursor
    SetCursor(currentRow, 0u);
}

