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

void Err_initialize(void)
{
    ErrorCount = 0;
    DebugCursor = 0;
}


void Err_raise(uint8_t module, uint16_t line_number)
{
    //hal_InterruptsDisable();  TODO critical section

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

void Err_debugPrint(uint8_t x)
{

    // TODO this better
    char buffer[3];

    Lcd_setCursor(0, DebugCursor);

    Util_writeHex(buffer, x);

    Lcd_write(buffer[0]);
    Lcd_write(buffer[1]);
    Lcd_write('<');

    DebugCursor += 2;

    if (DebugCursor >= 18)
    {
        DebugCursor = 0;
    }
}

void Err_debugPrintBlock(const void *src, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        Err_debugPrint(((uint8_t*)src)[i]);
    }
}

void Err_reset(void)
{
    ErrorCount = 0;
}

char * Err_print(char *dest, uint8_t number)
{
    if (number < ErrorCount)
    {
        dest = Util_writeInt8LA(dest, ErrorBuffer[number].Module);
        *(dest++) = '-';
        dest = Util_writeInt16LA(dest, ErrorBuffer[number].LineNumber);
    }

    return dest;
}

uint8_t Err_getCount(void)
{
    return ErrorCount;
}
