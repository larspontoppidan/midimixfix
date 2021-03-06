/*
 * menu.h
 *
 *  Created on: Dec 12, 2012
 *      Author: lars
 */

#ifndef MENU_H_
#define MENU_H_


#include "common.h"


void menu_initialize(void);

void menu_handleMainLoop(void);

// User interface:

void menu_handleUserTurns(int8_t delta, bool_t pushed);

void menu_handleUserSelects(void);
void menu_handleUserBacks(void);

void menu_notifyRefresh_SAFE(void);


///////// Defines

// Event types for MenuEvent function:

#define MENU_EVENT_BACK      0u
#define MENU_EVENT_SELECT    1u
#define MENU_EVENT_TURN      2u
#define MENU_EVENT_TURN_PUSH 3u

// Return values from MenuEvent function:

#define MENU_EDIT_MODE_UNAVAIL 0x1F
#define MENU_SET_CURSOR_MASK   0x1F
#define MENU_UPDATE_ALL        0x20
#define MENU_RESERVE_DISPLAY   0x40




#endif /* MENU_H_ */
