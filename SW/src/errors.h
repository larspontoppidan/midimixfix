/*
 * errors.h
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#include "common.h"

// Error codes:
#define ERR_MIDIIN1_OVERFLOW  1
#define ERR_MIDIIN2_OVERFLOW  2

#define ERR_INVALID_MENU_ENTITY  1   // componentfactory.c
#define ERR_INTERNAL1   100   // lcd.c, lcd_SetCursor: Invalid column / row
#define ERR_INTERNAL2   101   // quaddecode.c, lcd_SetCursor: Invalid column / row
#define ERR_INTERNAL3   102   // midimessage.c, mmsg_UpdateParser
#define ERR_INTERNAL4   103   // midimessage.c, mmsg_GetMsgByte
#define ERR_INTERNAL5   105   // hal.c
#define ERR_INTERNAL6   106   // midilogger.c

void err_Register(uint8_t err_code);
void err_UnRegister(uint8_t err_code);


#endif /* ERRORS_H_ */
