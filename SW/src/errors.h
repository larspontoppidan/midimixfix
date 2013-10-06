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
    ERR_MODULE_UI,      // 9
    ERR_MODULE_MENUE,   // 10
    ERR_MODULE_MIDIIO,  // 11
    ERR_MODULE_MLOG,    // 12
    ERR_MODULE_MMSG,    // 13
    ERR_MODULE_MPARSER, // 14
    ERR_MODULE_PSTR,    // 15
    ERR_MODULE_PRESETS, // 16
    ERR_MODULE_QD,      // 17
    ERR_MODULE_UTIL,    // 18
    ERR_MODULE_SDELAY,  // 19
    ERR_MODULE_MIDIPROC,
    ERR_MODULE_FILTERS,
    ERR_MODULE_MIDIINPUT,
};

void err_initialize(void);
void err_raise(uint8_t module, uint16_t line_number); // TODO Obsolete! Use _INT or _MAIN
void err_raise_ISR(uint8_t module, uint16_t line_number);
void err_raise_MAIN(uint8_t module, uint16_t line_number);

void err_debugPrint(uint8_t x);
void err_debugPrintBlock(const void *src, uint8_t size);
void err_debugPrintInt16(int16_t x);

void err_reset(void);
char * err_print(char *dest, uint8_t number);
uint8_t err_getCount(void);


#endif /* ERRORS_H_ */
