/*
 * pgmstrings.c
 *
 *  Created on: Dec 25, 2012
 *      Author: lars
 */


#include "common.h"
#include "pgmstrings.h"


char const pstr_Empty[] PROGMEM = "";

char const pstr_On[]  PROGMEM = "On";
char const pstr_Off[] PROGMEM = "Off";

char const pstr_PlusParentheses[]  PROGMEM = "(+)";
char const pstr_MinusParentheses[] PROGMEM = "(-)";
char const pstr_OnParentheses[]  PROGMEM = " (ON)";
char const pstr_OffParentheses[] PROGMEM = "(OFF)";

char const pstr_Discard[] PROGMEM = "Discard";
char const pstr_Through[] PROGMEM = "Through";
char const pstr_Use[]     PROGMEM = "Use";

char const pstr_Chan[]   PROGMEM = "Chan";


static char const StrInSources[4] = "X12*";


// Write "InX", "In1, In2, In*
char *pstr_writeInX(char *dest, uint8_t i)
{
    (*dest++) = 'I';
    (*dest++) = 'n';
    (*dest++) = StrInSources[i];

    return dest;
}
