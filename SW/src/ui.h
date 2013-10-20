/*
 * ui.h
 *
 *  Created on: Sep 12, 2013
 *      Author: lars
 */

#ifndef UI_H_
#define UI_H_

#include "common.h"
#include "menuinterface.h"

// Called from main:

void ui_initialize(void);

void ui_handleMainLoopHandler(void);


void ui_handleUserEvent(uint8_t user_event);


// UI events
#define UI_EVENT_NONE       0
#define UI_EVENT_SELECT     1
#define UI_EVENT_BACK       2
#define UI_EVENT_MOVE_UP    3
#define UI_EVENT_MOVE_DOWN  4

#define UI_MOVE_FAST_MASK   8 // This may be combined with MOVE_UP / DOWN

// Submenus can request ui module to update some or part of the display:
void ui_requestUpdateAll(void);
void ui_requestUpdate(uint8_t menu_item);

// When entering a menu, ui_menuEnter must be called with the new menu manifest.
// This will make ui render the menu.
void ui_menuEnter(const menuInterface_t *menu);

// Leave current menu
void ui_menuBackOut(void);

// Instruct UI to draw cursor on some item with some indent
void ui_menuMoveCursor(uint8_t cursorItem, uint8_t cursorIndent);



int8_t ui_eventToDelta(uint8_t ui_event, int8_t fast_speed);


#endif /* UI_H_ */
