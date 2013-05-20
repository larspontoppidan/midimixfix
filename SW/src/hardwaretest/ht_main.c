/*
 * hwtestmain.c
 *
 *  Created on: May 19, 2013
 *      Author: lars
 */


#include "../common.h"
#include "ht_main.h"
#include "ht_hal.h"
#include "../util.h"
#include <util/delay.h>
#include <string.h>

static void flashNumber(uint16_t i)
{
    char str[6];
    char *p;

    // Start with some substantial delay
    _delay_ms(2000);

    // Convert to base 10 and zero terminate
    p = util_writeInt16LA(str, i);
    *p = 0;

    // Go through digits until zero
    p = str;
    while (*p != 0)
    {
        hthal_flashLed( *p-'0' );
        p++;
    }

}

static void testGndShort(bool_t dont_test[])
{
    uint8_t i;

    for (i = 1; i <= 44; i++)
    {
        if (dont_test[i] == FALSE)
        {
            if (hthal_testIoGnd(i))
            {
                flashNumber(100 + i);

                dont_test[i] = TRUE;
            }
        }
    }
}

static void testVccShort(bool_t dont_test[])
{
    uint8_t i;

    for (i = 1; i <= 44; i++)
    {
        if (dont_test[i] == FALSE)
        {
            if (hthal_testIoVcc(i))
            {
                flashNumber(200 + i);

                dont_test[i] = TRUE;
            }
        }
    }
}

static void testOtherShort(bool_t dont_test[])
{
    uint8_t i, j;
    bool_t got_known_short = FALSE;

    for (i = 1; i <= 44; i++)
    {
        for (j = 1; j <= 44; j++)
        {
            if ((dont_test[i] == FALSE) && (dont_test[j] == FALSE))
            {
                if (hthal_testIoShort(i, j))
                {
                    if (((i == 12) && (j == 31)) ||
                        ((i == 31) && (j == 12)) )
                    {
                        // Known short
                        got_known_short = TRUE;
                    }
                    else
                    {
                        flashNumber(300 + i);
                    }
                }
            }
        }
    }

    if (got_known_short == FALSE)
    {
        flashNumber(400);
    }
}

void htmain_enterIfTestmode(void)
{
    // Look for a short between 1 and 3
    if (hthal_testIoShort(1, 3))
    {
        bool_t dont_test[45];

        // OK, this is testmode

        hthal_initialize();

        memset(dont_test, FALSE, 45);

        dont_test[40] = TRUE; // USB
        dont_test[41] = TRUE; // USB
        dont_test[42] = TRUE; // USB
        dont_test[44] = TRUE; // LED

        // OK, we are in test mode, Say hello with three short blinks
        hthal_flashLed(0);
        hthal_flashLed(0);
        hthal_flashLed(0);

        testGndShort(dont_test);
        testVccShort(dont_test);
        testOtherShort(dont_test);



        // Stay here
        while(TRUE)
        {
            ;
        }
    }

}

