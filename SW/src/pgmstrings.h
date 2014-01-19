/*
 * pgmstrings.h
 *
 *  Created on: Dec 25, 2012
 *      Author: lars
 */

#ifndef PGMSTRINGS_H_
#define PGMSTRINGS_H_

#include <avr/pgmspace.h>

extern char const PROGMEM pstr_Empty[];
extern char const PROGMEM pstr_On[];
extern char const PROGMEM pstr_Off[];

extern char const PROGMEM pstr_Yes[];
extern char const PROGMEM pstr_No[];

extern char const PROGMEM pstr_PlusParentheses[];
extern char const PROGMEM pstr_MinusParentheses[];
extern char const PROGMEM pstr_OnParentheses[];
extern char const PROGMEM pstr_OffParentheses[];
extern char const PROGMEM pstr_Discard[];
extern char const PROGMEM pstr_Through[];
extern char const PROGMEM pstr_Use[];

extern char const PROGMEM pstr_Chan[];

char *pstr_writeInX(char *dest, uint8_t i);

#endif /* PGMSTRINGS_H_ */
