/*
 * menuentities.h
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */

#ifndef MENUENTITIES_H_
#define MENUENTITIES_H_


#define MENUE_ENTITY_COUNT  8u


uint8_t MenuEnts_getSubCount(uint8_t entity);

void    MenuEnts_getText(uint8_t entity, char *dest, uint8_t item);

uint8_t MenuEnts_handleEvent(uint8_t entity, uint8_t item, uint8_t edit_mode,
                        uint8_t user_event, int8_t knob_delta);

uint8_t MenuEnts_configGetSize(uint8_t entity);
void    MenuEnts_configSave(uint8_t entity, uint8_t *dest);
void    MenuEnts_configLoad(uint8_t entity, uint8_t *dest);


// The possible return values of menue_HandleEvent are defined in menu.h




#endif /* MENUENTITIES_H_ */
