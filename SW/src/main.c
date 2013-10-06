
//
// Filename    : main.c
// Code module : Entrypoint and top level functionality
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
// ------------------------------  DESCRIPTION  ---------------------------------
//
// This source is compiled for Atmega644p with 64KB flash, 4KB ram, 2KB EEPROM


// -------------------------------  INCLUDES  -----------------------------------

#include "common.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "hardwaretest/hardwaretest.h"
#include "hal.h"
#include "lcd.h"
#include "quaddecode.h"
#include "ui.h"
#include "errors.h"
#include "filterhooks.h"
#include "filtersteps.h"
#include "midiprocessing.h"


// --------------------------  TYPES AND CONSTANTS  -----------------------------


// ----------------------------  LOCAL VARIABLES  -------------------------------


static bool_t UiSelectPushed = FALSE;
static bool_t UiBackPushed = FALSE;


// ------------------------------  PROTOTYPES  ----------------------------------


static void handleUi(void);


// ---------------------------  PRIVATE FUNCTIONS  ------------------------------


static void handleUi(void)
{
    int8_t i;

    i = quaddecode_getDelta_MAIN();

    while (i > 0)
    {
        ui_handleUserEvent(UI_EVENT_MOVE_UP);
        i--;
    }

    while (i < 0)
    {
        ui_handleUserEvent(UI_EVENT_MOVE_DOWN);
        i++;
    }

    i = quaddecode_getPushedDelta_MAIN();

    while (i > 0)
    {
        ui_handleUserEvent(UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK);
        i--;
    }

    while (i < 0)
    {
        ui_handleUserEvent(UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK);
        i++;
    }

    if (hal_buttonStatesGet() & HAL_BUTTON_SEL)
    {
        if (!UiSelectPushed)
        {
            ui_handleUserEvent(UI_EVENT_SELECT);
            UiSelectPushed = TRUE;
        }
    }
    else
    {
        UiSelectPushed = FALSE;
    }

    if (hal_buttonStatesGet() & HAL_BUTTON_BACK)
    {
        if (!UiBackPushed)
        {
            ui_handleUserEvent(UI_EVENT_BACK);
            UiBackPushed = TRUE;
        }
    }
    else
    {
        UiBackPushed = FALSE;
    }

    // TODO could use some better debounce filtering than just delaying here...
    _delay_ms(10);
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

int main(void)
{
    // Hardware test mode?
    hwtest_enterIfTestMode();

    // Initialize basic modules
    err_initialize();
    hal_initialize();
    lcd_initialize();
    quaddecode_initialize();
    midiproc_initialize();
    fsteps_initialize();

    // Initialize components
    FILTER_HOOKS_INITIALIZE();

    // Interrupts can start firing now
    hal_interruptsEnable();

    // Let menu start up
    ui_initialize();

    // Turn on display
    hal_lcdBacklightSet(TRUE);

    while(TRUE)
    {
        // Component hooks for main loop
        FILTER_HOOKS_MAIN_LOOP();

        // Check if user did something on the controls
        handleUi();
    }

    return 0;
}
