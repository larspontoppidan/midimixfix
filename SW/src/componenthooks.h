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
#include "curvefilter.h"
#include "superdelay.h"
#include "presets.h"

// Components can hook up functions to be called at different points in the program flow.
// It is done by modifying the following defines:

#define COMP_INITIALIZE_HOOKS()   \
    do {                        \
    mlog_Initialize();          \
    BlockFlt_initialize();        \
    CurveFlt_initialize();        \
    genmsg_initialize();        \
    sdelay_Initialize();        \
    presets_initialize();       \
    } while (0)

// For processing normal incoming messages
#define COMP_MESSAGE_ISR_HOOKS(x) \
    do {                        \
    mlog_handleMidiMsgIn_ISR(x);   \
    BlockFlt_hookMidiMsg_ISR(x);   \
    CurveFlt_hookMidiMsg_ISR(x);   \
    sdelay_HookMidiMsg_ISR(x);   \
    } while (0)

// For processing incoming realtime messages
#define COMP_RT_MESSAGE_ISR_HOOKS(x) \
    do {                             \
    mlog_handleMidiMsgIn_ISR(x);        \
    } while (0)

// For logging outgoing messages (no processing should be done at this point)
#define COMP_OUT_MESSAGE_ISR_HOOKS(x) \
    do {                              \
    mlog_handleMidiMsgOut_ISR(x);        \
    } while (0)

// For hooking up to the tick interrupt
#define COMP_TICK_ISR_HOOKS()   \
    do {                        \
    mlog_handleTick_ISR();      \
    BlockFlt_hookTick_ISR();      \
    sdelay_HookTick_ISR();      \
    } while (0)

// For hooking up to the main loop idle loop
#define COMP_MAIN_LOOP_HOOKS()    \
    do {                        \
    mlog_handleMainLoop();        \
    BlockFlt_hookMainLoop();      \
    CurveFlt_hookMainLoop();      \
    sdelay_HookMainLoop();      \
    } while (0)




#endif /* COMPONENTHOOKS_H_ */
