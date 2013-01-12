/*
 * pgmstrings.c
 *
 *  Created on: Dec 25, 2012
 *      Author: lars
 */


#include "common.h"
#include "pgmstrings.h"


char pstr_ON[] PROGMEM = "ON";
char pstr_OFF[] PROGMEM = "OFF";

char pstr_ShowParentheses[] PROGMEM = "(show)";
char pstr_HideParentheses[] PROGMEM = "(hide)";
char pstr_On[] PROGMEM = "On";
char pstr_Off[] PROGMEM = "Off";

char pstr_Discard[] PROGMEM = "Discard";
char pstr_Through[] PROGMEM = "Through";
char pstr_Use[] PROGMEM = "Use";

char pstr_ManageEllipsis[] PROGMEM = "Manage... ";
char pstr_Add[]    PROGMEM = "Add";
char pstr_Remove[] PROGMEM = "Remove";
char pstr_Chan[] PROGMEM = "Chan";


char InSources[4] = "X12*";


// Write "InX", "In1, In2, In*
char *pstr_WriteInX(char *dest, uint8_t i)
{
    (*dest++) = 'I';
    (*dest++) = 'n';
    (*dest++) = InSources[i];

    return dest;
}
