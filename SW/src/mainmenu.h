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

uint8_t mainmenu_TitleGetSubMenuCount(void);
void    mainmenu_TitleGetMenuText(char *dest, uint8_t item);
uint8_t mainmenu_TitleMenuEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta);

// The general setup options

uint8_t mainmenu_SetupGetSubMenuCount(void);
void    mainmenu_SetupGetMenuText(char *dest, uint8_t item);
uint8_t mainmenu_SetupMenuEvent(uint8_t item, uint8_t edit_mode,
                                uint8_t user_event, int8_t knob_delta);


uint8_t mainmenu_ConfigGetSize(void);
void    mainmenu_ConfigSave(uint8_t *dest);
void    mainmenu_ConfigLoad(uint8_t *dest);


#endif /* MAINMENU_H_ */
