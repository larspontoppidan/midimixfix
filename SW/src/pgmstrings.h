/*
 * pgmstrings.h
 *
 *  Created on: Dec 25, 2012
 *      Author: lars
 */

#ifndef PGMSTRINGS_H_
#define PGMSTRINGS_H_

#include <avr/pgmspace.h>

extern char PROGMEM PStr_On[];
extern char PROGMEM PStr_Off[];

extern char PROGMEM PStr_PlusParentheses[];
extern char PROGMEM PStr_MinusParentheses[];
extern char PROGMEM PStr_OnParentheses[];
extern char PROGMEM PStr_OffParentheses[];
extern char PROGMEM PStr_Discard[];
extern char PROGMEM PStr_Through[];
extern char PROGMEM PStr_Use[];

extern char PROGMEM PStr_Chan[];

char *PStr_writeInX(char *dest, uint8_t i);

#endif /* PGMSTRINGS_H_ */
