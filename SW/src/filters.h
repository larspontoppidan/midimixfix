/*
 * filters.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "common.h"
#include "midimessage.h"

// The filters module defines a common interface to all filters in the system: filterInstance_t
//
// Also acts as a filter factory and wraps function calls to filter instances.


// How to implement the filter menu. The string writing functions will be writing into
// a buffer that has been zeroed at first, thus it is optional to write a terminating zero.
//
// The indentation is the same for the entire filter and is used to place the cursor
//
// " Map and Block   A>A"
// "  From : Chan 2     "   indentation is 7 in this example, menu items is 2
// "  To   : Discard    "
//
// filter_request(uint8_t filter_type, char *dest)
// Must write a static title in dest, example "Map and Block", max 15 chars
//
// filter_getTitle(uint8_t filter_type, char *dest)
// Must write a static title in dest, example "Map and Block", max 15 chars
//
// filter_getMenuText(filterInstance_t* instance, char *dest, uint8_t item)
// Must write a line in the menu. Item 0 is first line, example : "From : Chan 2"
//
// filter_handleUiEvent(filterInstance_t* instance, uint8_t item, uint8_t user_event)
// When user increments or decrements an item in the menu with or without fast.
// The filter just changes it's values and when getMenuText is called the next
// time, the new value must be shown.
//

enum
{
    FILTER_MIDIIN1 = 0,
    FILTER_MIDIIN2,
    FILTER_MIDIIN1_REALT,
    FILTER_MIDIIN2_REALT,
    FILTER_MIDIIN1_SYSEX,
    FILTER_MIDIIN2_SYSEX,
    FILTER_MIDIOUT,
//    FILTER_PEDALIN1,
//    FILTER_PEDALIN2,
//    FILTER_GENERATEMSG,
//    FILTER_BLOCKFILTER,
//    FILTER_CURVEFILTER,
//    FILTER_SUPERDELAY,
//    FILTER_MIDILOG
    FILTER_TYPE_COUNT
};



// Instances of this struct means instances of filters
typedef struct
{
    // Basic declaration of filter type, step number in chain, and input output routes
    uint8_t FilterType;
    uint8_t StepNumber;
    uint8_t RouteIn;
    uint8_t RouteOut;

    // This particular filter may be one out of several instances of same filter type
    // This has to be handled internally by the filter module using this struct
    uint8_t Instance;

} filterInstance_t;


// Construct new filter.
//
// The new function will be handed a saved config or a null pointer.
//
// The function must return true if successfull. It may fail if there are no
// resources available or the filter can only be initiated once.
bool_t  filter_new(uint8_t filter_type, uint8_t *config, filterInstance_t* instance);

void    filter_saveConfig(filterInstance_t* instance, uint8_t *dest);

// Mechanism for both requests and queries to a filter:
uint8_t filter_request(filterInstance_t* instance, uint8_t request);

#define FILTER_REQ_DESTROY      1  // Release this filter instance
#define FILTER_REQ_UPDATE_SELF  2  // Notify that self struct has updated. Filter can revert illegal changes
#define FILTER_REQ_CONF_SIZE    3  // Get number of bytes in config
#define FILTER_REQ_MENU_ITEMS   4  // Get numner of menu items
#define FILTER_REQ_MENU_INDENT  5  // Get indentation used for values in menu


// Process midi msg
void filter_processMidiMsg(filterInstance_t* instance, midiMsg_t *msg);

// Name of filter
void filter_getTitle(uint8_t filter_type, char *dest);

// Menu integration
void filter_getMenuText(filterInstance_t* instance, char *dest, uint8_t item);

// In normal mode only UI_EVENT_MOVE_UP, DOWN and FAST are possible
void filter_handleUiEvent(filterInstance_t* instance, uint8_t item, uint8_t user_event);






#endif /* FILTERS_H_ */
