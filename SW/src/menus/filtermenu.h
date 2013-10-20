/*
 * filtermenu.h
 *
 *  Created on: Oct 6, 2013
 *      Author: lars
 */

#ifndef FILTERMENU_H_
#define FILTERMENU_H_

#include "../common.h"
#include "../menuinterface.h"
#include "../filterinterface.h"
#include <avr/pgmspace.h>

extern const menuInterface_t PROGMEM filtermenu_Menu;

void filtermenu_RequestUpdate(filters_instance_t filter, uint8_t menu_item);


#endif /* FILTERMENU_H_ */
