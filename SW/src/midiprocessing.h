/*
 * midiprocessing.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef MIDIPROCESSING_H_
#define MIDIPROCESSING_H_


#include "common.h"
#include "midimessage.h"


// This module takes care of the processing chain for midi messages

void midiproc_initialize(void);
void midiproc_handleTick_ISR(void);

void midiproc_addMessage_ISR(midiMsg_t *msg, uint8_t filter_step);
void midiproc_addMessage_MAIN(midiMsg_t *msg, uint8_t filter_step);

void midiproc_start_MAIN(void);
void midiproc_stop_MAIN(void);


#endif /* MIDIPROCESSING_H_ */
