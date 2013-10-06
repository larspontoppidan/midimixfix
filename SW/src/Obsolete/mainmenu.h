/*
 * mainmenu.h
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "common.h"


// The main menu contributes with two entities, the title line and general setup options


// The title menu entity

uint8_t mainmenu_titleGetSubCount(void);
void    mainmenu_titleGetText(char *dest, uint8_t item);
uint8_t mainmenu_titleHandleEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta);

// The general setup options

uint8_t mainmenu_setupGetSubCount(void);
void    mainmenu_setupGetText(char *dest, uint8_t item);
uint8_t mainmenu_setupHandleEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta);


uint8_t mainmenu_configGetSize(void);
void    mainmenu_configSave(uint8_t *dest);
void    mainmenu_configLoad(uint8_t *dest);


#endif /* MAINMENU_H_ */
