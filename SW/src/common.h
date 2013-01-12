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




// BUILD CONFIGURATION

#define BUILD_TX_RUNNING_STATUS  FALSE  // If TRUE, the transmitter will use running status

#define BUILD_VERSION_MAJOR  0
#define BUILD_VERSION_MINOR  1


#endif /* COMMON_H_ */
