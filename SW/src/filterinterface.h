/*
 * filterinterface.h
 *
 *  Created on: Oct 13, 2013
 *      Author: lars
 */

#ifndef FILTERINTERFACE_H_
#define FILTERINTERFACE_H_

#include "common.h"
#include "midimessage.h"

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


// --

// "Filter" is the internal name for all processing, input and output steps that the
// user can setup and place in the chain of processing done in midimixfix.
//
// A filter is implemented in an object oriented fashion. A filter "class" may have
// in 0, 1 or more instances. A filters class is typically implemented in a codemodule
// and must provide a const filterInterface_t with data and function pointers, located
// in PROGMEM.
//
// When a filter is instantiated, it is allocated an instance number starting from 0.
// The functions in a filter class always take the instance number as first parameter.
//


// The filter unique ID is composed of the following parts:
#define FILTER_ID_TYPE     0x00000001ul
#define FILTER_ID_VERSION  0x00000100ul
#define FILTER_ID_AUTHOR   0x01000000ul


#define FILTER_MODE_IN         1  // The filter inputs from one route
#define FILTER_MODE_OUT        2  // The filter outputs to one route
#define FILTER_MODE_IN_OUT     3  // The filter inputs and outputs on two possibly different routes
#define FILTER_MODE_PROCESSOR  4  // The filter inputs and outputs on same route


#define FILTER_PROCESS_DISCARD 0  // Message must be discarded
#define FILTER_PROCESS_NEUTRAL 1  // Message was not touched
#define FILTER_PROCESS_DID     2  // Message must passed on with the filter output route

#define FILTER_CREATE_FAILED      0xFF

#define FILTER_STEP_NA        0xFF

// The function pointers types are defined in common.h and here:
typedef uint8_t (*fptrProcessMidiMsg_t)(uint8_t, midiMsg_t *);
typedef void (*fptrWriteMenuText_t)(uint8_t, uint8_t, void *);
typedef void (*fptrHandleUserEvent_t)(uint8_t, uint8_t, uint8_t);

// All filters must implement this call back interface, as a constant PROGMEM struct

typedef struct
{
    // new: Create new filter instance.
    // Returns:    instance_number or FILTER_CREATE_FAILED
    // Parameters: uint8_t filter_step
    fptrUint8Uint8_t     create;

    // destroy: Destroy filter instance.
    // Parameters: uint8_t instance_number
    fptrVoidUint8_t      destroy;

    // setFilterStep: All filter instances must keep track of the step they have in processing chain,
    //                if they want to post their own new midi messages in the processing chain
    //                This function is called whenever a filter gets created or a new position in the chain
    // Parameters: uint8_t instance_number,
    //             uint8_t filter_step
    fptrVoidUint8Uint8_t setFilterStep;

    // loadConfig: Load filter config. NOTE filter_step should not be saved as part of config
    // Parameters: uint8_t instance_number, void* to config data
    fptrVoidUint8Voidp_t loadConfig;

    // saveConfig: Save filter config. NOTE filter_step should not be saved as part of config
    // Parameters: uint8_t instance_number, void* to config data
    fptrVoidUint8Voidp_t saveConfig;

    // processMidiMsg: Process MIDI message
    // Parameters: uint8_t instance_number, midiMsg_t * for midi message
    // Returns: FILTER_PROCESS_DISCARD, FILTER_PROCESS_NEUTRAL or FILTER_PROCESS_DID
    fptrProcessMidiMsg_t processMidiMsg;

    // writeMenuText: Write text of a menu item.
    // Parameters: uint8_t instance_number,
    //             uint8_t menu_item        // Which menu item to write, 0 is menu title, 1 is first item
    //             void *dest to write in,  // Target char buffer to write in
    fptrWriteMenuText_t writeMenuText;

    // handleUiEvent: Handle user interface event
    // Parameters: uint8_t instance_number,
    //             uint8_t menu_item        // Which menu item was accessed, 1 is first item
    //             uint8_t user_event       // USER_EVENT_MOVE_UP / DOWN may have FAST_MASK set
    fptrHandleUserEvent_t handleUiEvent;

    // Public const variables defining filter:

    const uint8_t  ConfigSize;     // Number of bytes in configuration
    const uint8_t  MenuItems;      // Number of menu items (0 means only title, 1 is one item)
    const uint8_t  MenuIndent;     // Cursor indentation in menu
    const uint8_t  FilterMode;     // Filter operation mode
    const char     *FilterTitle;   // Static filter title (this may be different from in-menu title)
    const uint32_t UniqueId;       // Unique identification of the filter class

} filterInterface_t;





#endif /* FILTERINTERFACE_H_ */
