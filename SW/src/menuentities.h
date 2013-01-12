/*
 * menuentities.h
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */

#ifndef MENUENTITIES_H_
#define MENUENTITIES_H_


#define MENUE_ENTITY_COUNT  5u


uint8_t menue_GetSubMenuCount(uint8_t entity);

void    menue_GetMenuText(uint8_t entity, char *dest, uint8_t item);

uint8_t menue_MenuEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                        uint8_t user_event, int8_t knob_delta);


// The possible return values of menue_MenuEvent are defined in menu.h




#endif /* MENUENTITIES_H_ */
