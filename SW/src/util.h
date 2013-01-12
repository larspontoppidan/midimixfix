/*
 * util.h
 *
 *  Created on: Dec 21, 2012
 *      Author: lars
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <avr/pgmspace.h>

char *util_StrWriteInt16(char *dest, int16_t value);
char *util_StrWriteUint8(char *dest, uint8_t value);
char *util_StrWriteInt8LA(char *dest, int8_t value);
char *util_StrWriteInt16LA(char *dest, int16_t value);
char *util_StrWriteHex(char *dest, uint8_t value);
char *util_StrCpy_P(char *dest, PGM_P src);

int8_t util_BoundedAddInt8(int8_t value, int8_t min, int8_t max, int8_t add);

#endif /* UTIL_H_ */
