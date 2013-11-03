/*
 * midiprocessing.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef MIDIPROCESSING_H_
#define MIDIPROCESSING_H_


#include "common.h"
#include "filters.h"
#include "midimessage.h"


// This module takes care of the processing chain for midi messages

void midiproc_initialize(void);
void midiproc_handleTick_ISR(void);

void midiproc_addMessage_ISR(midiMsg_t *msg, uint8_t filter_step);
void midiproc_addMessage_MAIN(midiMsg_t *msg, uint8_t filter_step);

void midiproc_start_MAIN(void);
void midiproc_stop_MAIN(void);


// Filter steps


typedef struct
{
    uint8_t In;
    uint8_t Out;
} midiproc_route_t;





// Filter steps management
uint8_t midiproc_getFilterSteps_SAFE(void);
bool_t midiproc_addFilter_MAIN(uint8_t filter_type);
bool_t midiproc_loadFilter_MAIN(uint8_t filter_type, midiproc_route_t route, uint8_t *filter_config);

void   midiproc_removeFilter_MAIN(uint8_t step);
void   midiproc_swapFilters_MAIN(uint8_t step1, uint8_t step2);
void   midiproc_removeAllFilters_MAIN(void);

// Accessing one filter

filters_instance_t midiproc_getFilterInstance_SAFE(uint8_t step);

midiproc_route_t midiproc_getFilterRoute_SAFE(uint8_t step);
void  midiproc_setRoute_MAIN(uint8_t step, midiproc_route_t route);




#endif /* MIDIPROCESSING_H_ */
