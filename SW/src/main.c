/*
 * main.c
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */

// This source is compiled for Atmega644p with 64KB flash, 4KB ram, 2KB EEPROM

#include "common.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include "hal.h"
#include "lcd.h"
#include "quaddecode.h"
#include "midiio.h"
#include "midiparser.h"
#include "menu.h"
#include "componenthooks.h"
#include "errors.h"
#include "generatemsg.h"

/// VARIABLES

bool_t uiSelectPushed = FALSE;
bool_t uiBackPushed = FALSE;


/// PROTOTYPES

static void HandleUi(void);


/// IMPLEMENTATION

static void HandleUi(void)
{
    int8_t i;

    i = qd_getDelta();

    if (i != 0u)
    {
        Menu_handleUserTurns(i, FALSE);
    }

    i = qd_getPushedDelta();

    if (i != 0u)
    {
        Menu_handleUserTurns(i, TRUE);
    }

    if (Hal_buttonStatesGet() & HAL_BUTTON_SEL)
    {
        if (!uiSelectPushed)
        {
            Menu_handleUserSelects();
            uiSelectPushed = TRUE;
        }
    }
    else
    {
        uiSelectPushed = FALSE;
    }

    if (Hal_buttonStatesGet() & HAL_BUTTON_BACK)
    {
        if (!uiBackPushed)
        {
            Menu_handleUserBacks();
            uiBackPushed = TRUE;
        }
    }
    else
    {
        uiBackPushed = FALSE;
    }

    // TODO could use some better debounce filtering than just delaying here...
    _delay_ms(10);
}

int main(void)
{
    // Initialize basic modules
    err_Initialize();
    Hal_initialize();
    Lcd_initialize();
    qd_initialize();
    MidiIo_initialize();
    mparser_initialize();

    // Initialize components
    COMP_INITIALIZE_HOOKS();

    // Interrupts can start firing now
    Hal_interruptsEnable();

    // Let menu start up
    Menu_initialize();

    // Turn on display
    Hal_lcdBacklightSet(TRUE);

    while(TRUE)
    {
        // Component hooks for main loop
        COMP_MAIN_LOOP_HOOKS();

        // We are handling menu aspectes here. Call the menu hook
        Menu_handleMainLoopHook();

        // Check if user did something on the controls
        HandleUi();
    }

    return 0;
}
