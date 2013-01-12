/*
 * componenthooks.h
 *
 *  Created on: Dec 23, 2012
 *      Author: lars
 */

#ifndef COMPONENTHOOKS_H_
#define COMPONENTHOOKS_H_


#include "common.h"

#include "midilogger.h"
#include "blockfilter.h"


// Components can hook up functions to be called at different points in the program flow.
// It is done by modifying the following defines:

#define COMP_INITIALIZE_HOOKS()   \
    do {                        \
    mlog_Initialize();          \
    blockf_Initialize();        \
    } while (0)

// For processing normal incoming messages
#define COMP_MESSAGE_ISR_HOOKS(x) \
    do {                        \
    mlog_InMessageIsrHook(x);     \
    blockf_MessageIsrHook(x);   \
    } while (0)

// For processing incoming realtime messages
#define COMP_RT_MESSAGE_ISR_HOOKS(x) \
    do {                             \
    mlog_InMessageIsrHook(x);          \
    } while (0)

// For logging outgoing messages (no processing should be done at this point)
#define COMP_OUT_MESSAGE_ISR_HOOKS(x) \
    do {                              \
    mlog_OutMessageIsrHook(x);        \
    } while (0)

// For hooking up to the tick interrupt
#define COMP_TICK_ISR_HOOKS()     \
    do {                        \
    mlog_TickIsrHook();         \
    blockf_TickIsrHook();       \
    } while (0)

// For hooking up to the main loop idle loop
#define COMP_MAIN_LOOP_HOOKS()    \
    do {                        \
    mlog_MainLoopHook();        \
    blockf_MainLoopHook();      \
    } while (0)




#endif /* COMPONENTHOOKS_H_ */
