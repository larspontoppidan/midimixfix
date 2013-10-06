/*
 * filtersteps.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef FILTERSTEPS_H_
#define FILTERSTEPS_H_


#include "common.h"
#include "filters.h"

void fsteps_initialize(void);

uint8_t fsteps_getCount_SAFE(void);
filter_t* fsteps_getFilter_SAFE(uint8_t step);

bool_t fsteps_addFilter_MAIN(uint8_t filter_type, uint8_t *config);

void fsteps_removeFilter_MAIN(uint8_t step);

void fsteps_swapFilter_MAIN(uint8_t step1, uint8_t step2);



#endif /* FILTERSTEPS_H_ */
