/*
 * pgmstrings.c
 *
 *  Created on: Dec 25, 2012
 *      Author: lars
 */


#include "common.h"
#include "pgmstrings.h"


char PStr_On[]  PROGMEM = "On";
char PStr_Off[] PROGMEM = "Off";

char PStr_PlusParentheses[]  PROGMEM = "(+)";
char PStr_MinusParentheses[] PROGMEM = "(-)";
char PStr_OnParentheses[]  PROGMEM = " (ON)";
char PStr_OffParentheses[] PROGMEM = "(OFF)";

char PStr_Discard[] PROGMEM = "Discard";
char PStr_Through[] PROGMEM = "Through";
char PStr_Use[]     PROGMEM = "Use";

char PStr_Chan[]   PROGMEM = "Chan";


static char StrInSources[4] = "X12*";


// Write "InX", "In1, In2, In*
char *PStr_writeInX(char *dest, uint8_t i)
{
    (*dest++) = 'I';
    (*dest++) = 'n';
    (*dest++) = StrInSources[i];

    return dest;
}
