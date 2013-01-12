/*
 * curvemath.h
 *
 *  Created on: Jan 12, 2013
 *      Author: lars
 */

#ifndef CURVEMATH_H_
#define CURVEMATH_H_

#include "common.h"


#define CURVEM_TYPES 7

typedef struct
{
    // The curve definition:
    uint8_t curve_type;
    int16_t gain;
    int8_t offset;
} curvemath_t;


void curvem_ResetCurve(curvemath_t *curve);

// Functions for displaying the curve spec
char *curvem_WriteLow(char *dest, curvemath_t *curve);
char *curvem_WriteType(char *dest, curvemath_t *curve);
char *curvem_WriteHigh(char *dest, curvemath_t *curve);

// Applying curve on a value
uint8_t curvem_ApplyCurve(uint8_t x, curvemath_t *curve);





#endif /* CURVEMATH_H_ */
