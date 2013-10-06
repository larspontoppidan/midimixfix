/*
 * filterhooks.h
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */

#ifndef FILTERHOOKS_H_
#define FILTERHOOKS_H_

#include "Filters/midiin.h"
#include "Filters/midiout.h"


// Filters can hook up direct function calls in the program flow.

// It is done by modifying the following defines:

// NOTE: Core functionality of MidiMixFix should not use these hooks.

#define FILTER_HOOKS_INITIALIZE()  \
    do {                           \
      midiin_initialize();         \
      midiout_initialize();        \
    } while (0)
//    midilog_initialize();
//    blockflt_initialize();
//    curveflt_initialize();
//    pedals_initialize();
//    genmsg_initialize();
//    superdly_initialize();
//    presets_initialize();
//    psplit_initialize();


// For hooking up to the tick interrupt (100 Hz)
#define FILTER_HOOKS_TICK_ISR()   \
    do {                          \
    } while (0)
//    pedals_handleTick_ISR();
//    midilog_handleTick_ISR();
//    blockflt_handleTick_ISR();
//    superdly_handleTick_ISR();
//    psplit_handleTick_ISR();


// For hooking up to the main loop idle loop
#define FILTER_HOOKS_MAIN_LOOP()  \
    do {                          \
    } while (0)
//    pedals_handleMainLoop();
//    midilog_handleMainLoop();
//    blockflt_handleMainLoop();
//    curveflt_handleMainLoop();
//    superdly_handleMainLoop();



#endif /* FILTERHOOKS_H_ */
