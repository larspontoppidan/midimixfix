/*
 * errors.h
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#include "common.h"


enum
{
    ERR_MODULE_MAIN = 0,// 0
    ERR_MODULE_HAL,     // 1
    ERR_MODULE_LCD,     // 2
    ERR_MODULE_BLOCKF,  // 3
    ERR_MODULE_CURVEF,  // 4
    ERR_MODULE_CMATH,   // 5
    ERR_MODULE_ERR,     // 6
    ERR_MODULE_GENMSG,  // 7
    ERR_MODULE_MMENU,   // 8
    ERR_MODULE_MENU,    // 9
    ERR_MODULE_MENUE,   // 10
    ERR_MODULE_MIDIIO,  // 11
    ERR_MODULE_MLOG,    // 12
    ERR_MODULE_MMSG,    // 13
    ERR_MODULE_MPARSER, // 14
    ERR_MODULE_PSTR,    // 15
    ERR_MODULE_PRESETS, // 16
    ERR_MODULE_QD,      // 17
    ERR_MODULE_UTIL,    // 18
    ERR_MODULE_SDELAY   // 19
};

void err_Initialize(void);
void err_Raise(uint8_t module, uint16_t line_number);

void err_DebugPrint(uint8_t x);
void err_DebugPrintBlock(const void *src, uint8_t size);

void err_Reset(void);
char * err_Print(char *dest, uint8_t number);
uint8_t err_GetCount(void);


#endif /* ERRORS_H_ */
