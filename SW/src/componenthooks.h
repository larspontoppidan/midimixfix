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

#define COMP_HOOKS_INITIALIZE()  \
    do {                         \
    MidiLog_initialize();        \
    BlockFlt_initialize();       \
    CurveFlt_initialize();       \
    GenMsg_initialize();         \
    SuperDly_initialize();       \
    Presets_initialize();        \
    } while (0)

// For processing normal incoming messages
#define COMP_HOOKS_MIDI_MSG_IN_ISR(x) \
    do {                              \
    MidiLog_handleMidiMsgIn_ISR(x);   \
    BlockFlt_handleMidiMsg_ISR(x);    \
    CurveFlt_handleMidiMsg_ISR(x);    \
    SuperDly_handleMidiMsg_ISR(x);    \
    } while (0)

// For processing incoming realtime messages
#define COMP_HOOKS_RT_MIDI_MSG_ISR(x) \
    do {                              \
    MidiLog_handleMidiMsgIn_ISR(x);   \
    } while (0)

// For logging outgoing messages (no processing should be done at this point)
#define COMP_HOOKS_MIDI_MSG_OUT_ISR(x) \
    do {                               \
    MidiLog_handleMidiMsgOut_ISR(x);   \
    } while (0)

// For hooking up to the tick interrupt
#define COMP_HOOKS_TICK_ISR()   \
    do {                        \
    MidiLog_handleTick_ISR();   \
    BlockFlt_handleTick_ISR();  \
    SuperDly_handleTick_ISR();  \
    } while (0)

// For hooking up to the main loop idle loop
#define COMP_HOOKS_MAIN_LOOP()  \
    do {                        \
    MidiLog_handleMainLoop();   \
    BlockFlt_handleMainLoop();  \
    CurveFlt_handleMainLoop();  \
    SuperDly_handleMainLoop();  \
    } while (0)




#endif /* COMPONENTHOOKS_H_ */
