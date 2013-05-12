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
#include "pedals.h"
#include "superdelay.h"
#include "presets.h"

// Components can hook up functions to be called at different points in the program flow.
// It is done by modifying the following defines:

#define COMP_HOOKS_INITIALIZE()  \
    do {                         \
    midilog_initialize();        \
    blockflt_initialize();       \
    curveflt_initialize();       \
    pedals_initialize();         \
    genmsg_initialize();         \
    superdly_initialize();       \
    presets_initialize();        \
    } while (0)

// For processing normal incoming messages
#define COMP_HOOKS_MIDI_MSG_IN_ISR(x) \
    do {                              \
    midilog_handleMidiMsgIn_ISR(x);   \
    blockflt_handleMidiMsg_ISR(x);    \
    curveflt_handleMidiMsg_ISR(x);    \
    superdly_handleMidiMsg_ISR(x);    \
    } while (0)

// For processing incoming realtime messages
#define COMP_HOOKS_RT_MIDI_MSG_ISR(x) \
    do {                              \
    midilog_handleMidiMsgIn_ISR(x);   \
    } while (0)

// For logging outgoing messages (no processing should be done at this point)
#define COMP_HOOKS_MIDI_MSG_OUT_ISR(x) \
    do {                               \
    midilog_handleMidiMsgOut_ISR(x);   \
    } while (0)

// For hooking up to the tick interrupt
#define COMP_HOOKS_TICK_ISR()   \
    do {                        \
    pedals_handleTick_ISR();    \
    midilog_handleTick_ISR();   \
    blockflt_handleTick_ISR();  \
    superdly_handleTick_ISR();  \
    } while (0)

// For hooking up to the main loop idle loop
#define COMP_HOOKS_MAIN_LOOP()  \
    do {                        \
    pedals_handleMainLoop();    \
    midilog_handleMainLoop();   \
    blockflt_handleMainLoop();  \
    curveflt_handleMainLoop();  \
    superdly_handleMainLoop();  \
    } while (0)




#endif /* COMPONENTHOOKS_H_ */
