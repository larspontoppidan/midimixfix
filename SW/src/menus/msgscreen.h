/*
 * msgscreen.h
 *
 *  Created on: Oct 20, 2013
 *      Author: lars
 */

#ifndef MSGSCREEN_H_
#define MSGSCREEN_H_


#include "../common.h"
#include "../menuinterface.h"
#include <avr/pgmspace.h>

void msgscreen_Show_FormatP(const char *msg, uint8_t data, uint8_t write_offset);


#endif /* MSGSCREEN_H_ */
