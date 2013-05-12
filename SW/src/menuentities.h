/*
 * menuentities.h
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */

#ifndef MENUENTITIES_H_
#define MENUENTITIES_H_


#define MENUE_ENTITY_COUNT  10u


uint8_t menuents_getSubCount(uint8_t entity);

void    menuents_getText(uint8_t entity, char *dest, uint8_t item);

uint8_t menuents_handleEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                        uint8_t user_event, int8_t knob_delta);

uint8_t menuents_configGetSize(uint8_t entity);
void    menuents_configSave(uint8_t entity, uint8_t *dest);
void    menuents_configLoad(uint8_t entity, uint8_t *dest);


// The possible return values of menue_HandleEvent are defined in menu.h




#endif /* MENUENTITIES_H_ */
