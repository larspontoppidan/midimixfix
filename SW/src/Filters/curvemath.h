/*
 * curvemath.h
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */

#ifndef CURVEMATH_H_
#define CURVEMATH_H_

#include "../common.h"


#define CURVEMATH_TYPES 7

typedef struct
{
    // The curve definition:
    uint8_t Type;
    int16_t Gain;
    int8_t Offset;
} curveMath_t;


void curvemath_reset(curveMath_t *curve);

// Functions for displaying the curve spec
char *curvemath_writeLow(char *dest, curveMath_t *curve);
char *curvemath_writeType(char *dest, curveMath_t *curve);
char *curvemath_writeHigh(char *dest, curveMath_t *curve);

// Applying curve on a value
uint8_t curvemath_apply(uint8_t x, curveMath_t *curve);



#endif /* CURVEMATH_H_ */
