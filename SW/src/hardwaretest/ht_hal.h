/*
 * hwtesthal.h
 *
 *  Created on: May 19, 2013
 *      Author: lars
 */

#ifndef HT_HAL_H_
#define HT_HAL_H_


#include "../common.h"

void   hthal_initialize(void);

bool_t hthal_testIoGnd(uint8_t pin_no);
bool_t hthal_testIoVcc(uint8_t pin_no);
bool_t hthal_testIoShort(uint8_t pin_no1, uint8_t pin_no2);

void   hthal_flashLed(uint8_t flashes);


#endif /* HWTESTHAL_H_ */
