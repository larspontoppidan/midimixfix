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

uint8_t ErrorCount;
error_t ErrorBuffer[ERROR_BUFFER_SIZE];

uint8_t DebugCursor;

void err_Initialize(void)
{
    ErrorCount = 0;
    DebugCursor = 0;
}


void err_Raise(uint8_t module, uint16_t line_number)
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

void err_DebugPrint(uint8_t x)
{

    // TODO this better
    char buffer[3];

    lcd_CursorSet(0, DebugCursor);

    util_strWriteHex(buffer, x);

    lcd_Write(buffer[0]);
    lcd_Write(buffer[1]);
    lcd_Write('<');

    DebugCursor += 2;

    if (DebugCursor >= 18)
    {
        DebugCursor = 0;
    }
}

void err_DebugPrintBlock(const void *src, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        err_DebugPrint(((uint8_t*)src)[i]);
    }
}

void err_Reset(void)
{
    ErrorCount = 0;
}

char * err_Print(char *dest, uint8_t number)
{
    if (number < ErrorCount)
    {
        dest = util_strWriteInt8LA(dest, ErrorBuffer[number].Module);
        *(dest++) = '-';
        dest = util_strWriteInt16LA(dest, ErrorBuffer[number].LineNumber);
    }

    return dest;
}

uint8_t err_GetCount(void)
{
    return ErrorCount;
}
