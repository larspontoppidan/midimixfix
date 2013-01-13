/*
 * pgmstrings.h
 *
 *  Created on: Dec 25, 2012
 *      Author: lars
 */

#ifndef PGMSTRINGS_H_
#define PGMSTRINGS_H_

#include <avr/pgmspace.h>

extern char PROGMEM pstr_On[];
extern char PROGMEM pstr_Off[];

extern char PROGMEM pstr_PlusParentheses[];
extern char PROGMEM pstr_MinusParentheses[];
extern char PROGMEM pstr_OnParentheses[];
extern char PROGMEM pstr_OffParentheses[];
extern char PROGMEM pstr_Discard[];
extern char PROGMEM pstr_Through[];
extern char PROGMEM pstr_Use[];

extern char PROGMEM pstr_ManageEllipsis[];
extern char PROGMEM pstr_Add[];
extern char PROGMEM pstr_Remove[];
extern char PROGMEM pstr_Chan[];

char *pstr_WriteInX(char *dest, uint8_t i);

#endif /* PGMSTRINGS_H_ */
