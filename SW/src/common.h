/*
 * common.h
 *
 *  Created on: Dec 8, 2012
 *      Author: lars
 */

#ifndef COMMON_H_
#define COMMON_H_

// TYPES

#include <stdint.h>



typedef uint8_t bool_t;

#define TRUE  1u
#define FALSE 0u

#define NULL ((void *)0)

#define UNUSED(x) (void)(x)


// BUILD CONFIGURATION

#define BUILD_VERSION_STRING   " MIDIMIXFIX v1.02  "

// Generic function pointers

typedef uint8_t (*fptrUint8Uint8_t)(uint8_t);
typedef uint8_t (*fptrUint8Void_t)(void);
typedef void (*fptrVoidUint8_t)(uint8_t);
typedef void (*fptrVoidVoid_t)(void);

typedef void (*fptrVoidVoidp_t)(void*);
typedef void (*fptrVoidUint8Uint8_t)(uint8_t, uint8_t);
typedef void (*fptrVoidUint8Voidp_t)(uint8_t, void*);







#endif /* COMMON_H_ */
