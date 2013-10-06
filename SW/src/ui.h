/*
 * ui.h
 *
 *  Created on: Sep 12, 2013
 *      Author: lars
 */

#ifndef UI_H_
#define UI_H_

#include "common.h"

// Called from main:

void ui_initialize(void);

void ui_handleUserEvent(uint8_t user_event);


// All menus must implement this call back interface:

typedef struct
{
    bool_t hasStaticTitle;          // If true, the menu has a static line 0
    fptrUint8Void_t initGetCursor; // When a menu is entered, this is called initially
    fptrUint8Void_t getItemCount;   // Returns number of menu items, including line 0
    fptrVoidUint8_t drawItem;       // Draw one line of menu. Line 0 may be title or first entry
    fptrVoidUint8_t handleUiEvent;  // Handle the ui event
} menu_t;


// UI events
#define UI_EVENT_NONE       0
#define UI_EVENT_SELECT     1
#define UI_EVENT_BACK       2
#define UI_EVENT_MOVE_UP    3
#define UI_EVENT_MOVE_DOWN  4

#define UI_MOVE_FAST_MASK   8 // This may be combined with MOVE_UP / DOWN



// When entering a menu, ui_menuEnter must be called with the new menu manifest.
// This will make ui render the menu.
void ui_menuEnter(menu_t *menu);

// Leave current menu
void ui_menuBackOut(void);

// Instruct UI to draw cursor on some item with some indent
void ui_menuMoveCursor(uint8_t cursorItem, uint8_t cursorIndent);


// When a menu is drawing one item, it must prepare the data and call this function
// (instead of accessing LCD module directly)
void ui_menuDrawItem(uint8_t item, uint8_t const *data);

// Draw menu item with PGM space data
void ui_menuDrawItemP(uint8_t item, uint8_t const *data);


#endif /* UI_H_ */
