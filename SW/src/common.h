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

//typedef unsigned char uint8_t;
//typedef signed char int8_t;

//typedef unsigned short uint16_t;
//typedef signed short int16_t;

//typedef unsigned long uint32_t;
//typedef signed long int32_t;


typedef uint8_t bool_t;

#define TRUE  1u
#define FALSE 0u

#define NULL ((void *)0)

#define UNUSED(x) (void)(x)


// BUILD CONFIGURATION

#define BUILD_TX_RUNNING_STATUS  FALSE  // If TRUE, the transmitter will use running status

#define BUILD_VERSION_MAJOR  0
#define BUILD_VERSION_MINOR  1


#endif /* COMMON_H_ */
