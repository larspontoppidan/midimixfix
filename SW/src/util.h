/*
 * util.h
 *
 *  Created on: Dec 21, 2012
 *      Author: lars
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <avr/pgmspace.h>

char *util_writeInt16(char *dest, int16_t value);
char *util_writeUint8(char *dest, uint8_t value);
char *util_writeInt8(char *dest, int8_t x);
char *util_writeInt8LA(char *dest, int8_t value);
char *util_writeInt16LA(char *dest, int16_t value);
char *util_writeHex(char *dest, uint8_t value);
char *util_copyString_P(char *dest, PGM_P src);

int8_t util_boundedAddInt8(int8_t value, int8_t min, int8_t max, int8_t add);

uint8_t util_boundedAddUint8(uint8_t value, uint8_t min, uint8_t max, int8_t add);

void util_writeNumberParentheses(char *dest, uint8_t value);

char *util_writeFormat_P(char *dest, PGM_P src, uint8_t data);


#endif /* UTIL_H_ */
