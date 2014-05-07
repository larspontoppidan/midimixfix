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
#include "Filters/pedals.h"
#include "Filters/curvefilter.h"
#include "Filters/generatemsg.h"
#include "Filters/polysplit.h"
#include "Filters/atsmoother.h"
#include "Filters/resetter.h"

// Filters can hook up direct function calls in the program flow.

// It is done by modifying the following defines:

// NOTE: Core functionality of MidiMixFix should not use these hooks.

#define FILTER_HOOKS_INITIALIZE()  \
    do {                           \
      midiin_initialize();         \
      midiout_initialize();        \
      pedals_initialize();         \
      curveflt_initialize();       \
      genmsg_initialize();         \
    } while (0)

//    midilog_initialize();
//    blockflt_initialize();
//    pedals_initialize();
//    genmsg_initialize();
//    superdly_initialize();
//    presets_initialize();
//    psplit_initialize();


// For hooking up to the tick interrupt (100 Hz)
#define FILTER_HOOKS_TICK_ISR()   \
    do {                          \
        pedals_handleTick_ISR();  \
        atsmoother_handleTick_ISR(); \
        resetter_handleTick_ISR();  \
    } while (0)
//    blockflt_handleTick_ISR();
//    superdly_handleTick_ISR();


// For hooking up to the main loop idle loop
#define FILTER_HOOKS_MAIN_LOOP()  \
    do {                          \
        pedals_handleMainLoop();  \
        curveflt_handleMainLoop();\
    } while (0)
//    pedals_handleMainLoop();
//    midilog_handleMainLoop();
//    blockflt_handleMainLoop();
//    superdly_handleMainLoop();



#endif /* FILTERHOOKS_H_ */
