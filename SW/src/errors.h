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
    ERR_MODULE_MAIN,
    ERR_MODULE_HAL,
    ERR_MODULE_LCD,
    ERR_MODULE_BLOCKF,
    ERR_MODULE_CURVEF,
    ERR_MODULE_CMATH,
    ERR_MODULE_ERR,
    ERR_MODULE_GENMSG,
    ERR_MODULE_MMENU,
    ERR_MODULE_MENU,
    ERR_MODULE_MENUE,
    ERR_MODULE_MIDIIO,
    ERR_MODULE_MLOG,
    ERR_MODULE_MMSG,
    ERR_MODULE_MPARSER,
    ERR_MODULE_PSTR,
    ERR_MODULE_PRESETS,
    ERR_MODULE_QD,
    ERR_MODULE_UTIL
};

void err_Initialize(void);
void err_Raise(uint8_t module, uint16_t line_number);

void err_DebugPrint(uint8_t x);
void err_DebugPrintBlock(const void *src, uint8_t size);

void err_Reset(void);
char * err_Print(char *dest, uint8_t number);
uint8_t err_GetCount(void);

void err_PrintDebug(char *dst);


#endif /* ERRORS_H_ */
