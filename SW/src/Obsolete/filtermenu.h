/*
 * filtermenu.h
 *
 *  Created on: Sep 12, 2013
 *      Author: lars
 */

#ifndef FILTERMENU_H_
#define FILTERMENU_H_

void filtermenu_initialize(void);

// Handle user event. If this handler can not take care of event, it is returned
// from the function, otherwise 0 is returned.
bool_t filtermenu_event(uint8_t user_event);

void filtermenu_setup(void);

#endif /* FILTERMENU_H_ */
