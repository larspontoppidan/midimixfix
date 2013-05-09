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

static bool_t UiSelectPushed = FALSE;
static bool_t UiBackPushed = FALSE;


/// PROTOTYPES

static void handleUi(void);


/// IMPLEMENTATION

static void handleUi(void)
{
    int8_t i;

    i = QuadDecode_getDelta_MAIN();

    if (i != 0u)
    {
        Menu_handleUserTurns(i, FALSE);
    }

    i = QuadDecode_getPushedDelta_MAIN();

    if (i != 0u)
    {
        Menu_handleUserTurns(i, TRUE);
    }

    if (Hal_buttonStatesGet() & HAL_BUTTON_SEL)
    {
        if (!UiSelectPushed)
        {
            Menu_handleUserSelects();
            UiSelectPushed = TRUE;
        }
    }
    else
    {
        UiSelectPushed = FALSE;
    }

    if (Hal_buttonStatesGet() & HAL_BUTTON_BACK)
    {
        if (!UiBackPushed)
        {
            Menu_handleUserBacks();
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

int main(void)
{
    // Initialize basic modules
    Err_initialize();
    Hal_initialize();
    Lcd_initialize();
    QuadDecode_initialize();
    MidiIo_initialize();
    MidiParser_initialize();

    // Initialize components
    COMP_HOOKS_INITIALIZE();

    // Interrupts can start firing now
    Hal_interruptsEnable();

    // Let menu start up
    Menu_initialize();

    // Turn on display
    Hal_lcdBacklightSet(TRUE);

    while(TRUE)
    {
        // Component hooks for main loop
        COMP_HOOKS_MAIN_LOOP();

        // We are handling menu aspects here. Call the menu hook
        Menu_handleMainLoop();

        // Check if user did something on the controls
        handleUi();
    }

    return 0;
}
