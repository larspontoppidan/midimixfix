
//
// Filename    : msgscreen.c
// Code module : Display simple message on screen
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
#include "../midilog.h"
#include "../util.h"
#include "../hal.h"
#include "../filters.h"
#include "../ui.h"
#include "../util.h"
#include "../menuinterface.h"
#include <avr/pgmspace.h>
#include <string.h>


// ------------------------------  PROTOTYPES  ----------------------------------

static uint8_t initGetCursor(void);
static uint8_t getItemCount(void);
static void    writeItem(uint8_t item, void *dest);
static void    handleUiEvent(uint8_t uiEvent);

// --------------------------  TYPES AND CONSTANTS  -----------------------------

// Menu self declaration struct
static const menuInterface_t PROGMEM msgscreen_Menu =
{
        TRUE,             // bool_t hasStaticTitle;
        initGetCursor,    // fptrUint8Void_t  enterGetCursor;
        getItemCount,     // fptrUint8Void_t  getItemCount;
        writeItem,        // fptrVoidUint8Voidp_t writeItem;
        handleUiEvent     // fptrVoidUint8_t  handleUiEvent;
};


static const char Title[] PROGMEM = "----  Message:  ----";


// ----------------------------  LOCAL VARIABLES  -------------------------------

const char *Message;
uint8_t Data;
uint8_t WriteOffset;


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------


void msgscreen_Show_FormatP(const char *msg, uint8_t data, uint8_t write_offset)
{
    Message = msg;
    Data = data;
    WriteOffset = write_offset;
    ui_menuEnter(&msgscreen_Menu);
}


// ---------------------------  MENU CALLBACKS ------------------------------


static uint8_t initGetCursor(void)
{
    // We are about to enter this menu. Just return cursor position
    return 2;
}

static uint8_t getItemCount(void)
{
    // This "menu" always have the same number of items
    return 3;
}

static void writeItem(uint8_t item, void *dest)
{
    if (item == 0)
    {
        util_copyString_P(dest, Title);
    }
    else if (item == 2)
    {
        util_writeFormat_P((char*)dest + WriteOffset, Message, Data);
    }
}

static void handleUiEvent(uint8_t uiEvent)
{
    // Leave this screen if user backs or selects
    if ((uiEvent == UI_EVENT_BACK) ||
        (uiEvent == UI_EVENT_SELECT))
    {
        ui_menuBackOut();
    }
}
