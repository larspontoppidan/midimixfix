/*
 * menuinterface.h
 *
 *  Created on: Oct 13, 2013
 *      Author: lars
 */

#ifndef MENUINTERFACE_H_
#define MENUINTERFACE_H_

#include "common.h"

// All menus must implement this call back interface, as a constant PROGMEM struct

typedef struct
{
    bool_t hasStaticTitle;          // If true, the menu has a static line 0
    fptrUint8Void_t initGetCursor; // When a menu is entered, this is called initially
    fptrUint8Void_t getItemCount;   // Returns number of menu items, including line 0
    fptrVoidUint8_t drawItem;       // Draw one line of menu. Line 0 may be title or first entry
    fptrVoidUint8_t handleUiEvent;  // Handle the ui event
} menuInterface_t;




#endif /* MENUINTERFACE_H_ */
