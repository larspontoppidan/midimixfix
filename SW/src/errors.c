/*
 * errors.c
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#include "errors.h"
#include "common.h"
#include "hal.h"
#include "lcd.h"
#include "util.h"

typedef struct
{
    uint8_t Module;
    uint8_t LineNumber;
} error_t;


#define ERROR_BUFFER_SIZE 10

static uint8_t ErrorCount;
static error_t ErrorBuffer[ERROR_BUFFER_SIZE];

static uint8_t DebugCursor;

void err_initialize(void)
{
    ErrorCount = 0;
    DebugCursor = 0;
}


void err_raise(uint8_t module, uint16_t line_number)
{
    //hal_InterruptsDisable();  TODO obsolete this function

    if (ErrorCount < ERROR_BUFFER_SIZE)
    {
        ErrorCount++;
        //hal_InterruptsEnable();

        ErrorBuffer[ErrorCount-1].Module = module;
        ErrorBuffer[ErrorCount-1].LineNumber = line_number;
    }
    else
    {
        //hal_InterruptsEnable();
    }
}


void err_raise_ISR(uint8_t module, uint16_t line_number)
{
    if (ErrorCount < ERROR_BUFFER_SIZE)
    {
        ErrorCount++;

        ErrorBuffer[ErrorCount-1].Module = module;
        ErrorBuffer[ErrorCount-1].LineNumber = line_number;
    }
}


void err_raise_MAIN(uint8_t module, uint16_t line_number)
{
    hal_interruptsDisable();

    if (ErrorCount < ERROR_BUFFER_SIZE)
    {
        ErrorCount++;
        hal_interruptsEnable();

        ErrorBuffer[ErrorCount-1].Module = module;
        ErrorBuffer[ErrorCount-1].LineNumber = line_number;
    }
    else
    {
        hal_interruptsEnable();
    }
}

void err_debugPrint(uint8_t x)
{

    // TODO this better
    char buffer[3];

    lcd_setCursor(0, DebugCursor);

    util_writeHex(buffer, x);

    lcd_write(buffer[0]);
    lcd_write(buffer[1]);
    lcd_write('<');

    DebugCursor += 2;

    if (DebugCursor >= 18)
    {
        DebugCursor = 0;
    }
}

void err_debugPrintInt16(int16_t x)
{
    // TODO this better
    char buffer[7];
    char *p;

    lcd_setCursor(0, DebugCursor);

    // Write Int16 to buffer and zero terminate
    p = util_writeInt16(buffer, x);
    *p = 0;

    // Blit to LCD
    lcd_writeString(buffer);

    // Move debug cursor
    DebugCursor += 6;

    if (DebugCursor >= 18)
    {
        DebugCursor = 0;
    }
}

void err_debugPrintBlock(const void *src, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        err_debugPrint(((uint8_t*)src)[i]);
    }
}

void err_reset(void)
{
    ErrorCount = 0;
}

char * err_print(char *dest, uint8_t number)
{
    if (number < ErrorCount)
    {
        dest = util_writeInt8LA(dest, ErrorBuffer[number].Module);
        *(dest++) = '-';
        dest = util_writeInt16LA(dest, ErrorBuffer[number].LineNumber);
    }

    return dest;
}

uint8_t err_getCount(void)
{
    return ErrorCount;
}
