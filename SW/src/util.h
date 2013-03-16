/*
 * util.h
 *
 *  Created on: Dec 21, 2012
 *      Author: lars
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <avr/pgmspace.h>

char *util_strWriteInt16(char *dest, int16_t value);
char *util_strWriteUint8(char *dest, uint8_t value);
char *util_strWriteInt8(char *dest, int8_t x);
char *util_strWriteInt8LA(char *dest, int8_t value);
char *util_strWriteInt16LA(char *dest, int16_t value);
char *util_strWriteHex(char *dest, uint8_t value);
char *util_strCpy_P(char *dest, PGM_P src);

int8_t util_boundedAddInt8(int8_t value, int8_t min, int8_t max, int8_t add);

uint8_t util_boundedAddUint8(uint8_t value, uint8_t min, uint8_t max, int8_t add);

void util_strWriteNumberParentheses(char *dest, uint8_t value);

char *util_strWriteFormat_P(char *dest, PGM_P src, uint8_t data);


#endif /* UTIL_H_ */
