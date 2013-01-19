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
    uint8_t type;
    int16_t gain;
    int8_t offset;
} cmath_curve_t;


void cmath_CurveReset(cmath_curve_t *curve);

// Functions for displaying the curve spec
char *cmath_WriteLow(char *dest, cmath_curve_t *curve);
char *cmath_WriteType(char *dest, cmath_curve_t *curve);
char *cmath_WriteHigh(char *dest, cmath_curve_t *curve);

// Applying curve on a value
uint8_t cmath_CurveApply(uint8_t x, cmath_curve_t *curve);





#endif /* CURVEMATH_H_ */
