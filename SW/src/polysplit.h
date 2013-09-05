/*
 * polysplit.h
 *
 *  Created on: Jul 14, 2013
 *      Author: lars
 */

#ifndef POLYSPLIT_H_
#define POLYSPLIT_H_


void    psplit_initialize(void);
void    psplit_handleMidiMsg_ISR(midiMsg_t *msg);
void    psplit_handleTick_ISR(void);
uint8_t psplit_menuGetSubCount(void);
void    psplit_menuGetText(char *dest, uint8_t item);
uint8_t psplit_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta);
uint8_t psplit_configGetSize(void);
void    psplit_configSave(uint8_t *dest);
void    psplit_configLoad(uint8_t *dest);


#endif /* POLYSPLIT_H_ */
