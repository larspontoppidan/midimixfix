/*
 * quaddecode.c
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */


#include "common.h"
#include "quaddecode.h"
#include "hal.h"

// This module decodes quadrature waveforms using an algorithm which is
// tailor made for the situation where two separate interrupt sources
// indicate separately when there are changes on two quadrature signals,
// named A and B.
//
// The algorithm uses four states, which is somewhat unusual, as they are
// defined on signal change events combined with the constant state of the
// non changing signal. State 0 is when A changes (we don't care what value
// it changes from or to) and B is high. State 1 is when the B changes
// and A is low, etc.
//
// Defining the states on edges allows the algorithm to not care what way
// the changing signal is changing, eliminating certain race conditions,
// that may arise on noisy signals. Another benefit is an inherent Schmitt-
// trigger characteristic of the state changes. Finally, the algorithm is
// incredibly simple and efficient to implement.
//
// A disadvantage of the algorithm is that only 1/4 of the available
// quadrature resolution is usable. However, this fits perfectly for counting
// the clicks on a typical rotary encoder.
//
//
// A wavef: ^\___/^^^\___/^^^
// B wavef: ^^^\___/^^^\___/^^^
//
// Mechanical
// Clicks:  |       |       |
//
// A change: x   x   x   x
// B change:   x   x   x   x
//
// State:    0 1 2 3 0 1 2 3 0
//
// Trigger new value on transition from 1 to 2, or 2 to 1
//


static uint8_t CurrentState;

static int8_t volatile KnobDelta;
static int8_t volatile KnobPushedDelta;


static int8_t UpdateState(uint8_t new_state)
{
    int8_t delta;

    if ((new_state == 2) && (CurrentState == 1))
    {
        delta = -1;
    }
    else if ((new_state == 1) && (CurrentState == 2))
    {
        delta = 1;
    }
    else
    {
        delta = 0;
    }

    CurrentState = new_state;

    return delta;
}

void QuadDecode_initialize(void)
{
    CurrentState = 0;
    KnobDelta = 0;
    KnobPushedDelta = 0;
}


void QuadDecode_handleAChange_ISR(bool_t b_value, bool_t pushed)
{
    uint8_t new_state;

    new_state = b_value ? 0u : 2u;

    if (pushed)
    {
        KnobPushedDelta += UpdateState(new_state);
    }
    else
    {
        KnobDelta += UpdateState(new_state);
    }
}

void QuadDecode_handleBChange_ISR(bool_t a_value, bool_t pushed)
{
    uint8_t new_state;

    new_state = a_value ? 3u : 1u;

    if (pushed)
    {
        KnobPushedDelta += UpdateState(new_state);
    }
    else
    {
        KnobDelta += UpdateState(new_state);
    }
}


int8_t QuadDecode_getPushedDelta_MAIN(void)
{
    int8_t delta = 0;

    // Only go in an disable interrupts if necessary
    if (KnobPushedDelta != 0)
    {
        // Read out the delta value and reset it in one
        // atomic operation:

        Hal_interruptsDisable();

        delta = KnobPushedDelta;
        KnobPushedDelta = 0u;

        Hal_interruptsEnable();
    }

    return delta;
}

int8_t QuadDecode_getDelta_MAIN(void)
{
    int8_t delta = 0;

    // Only go in an disable interrupts if necessary
    if (KnobDelta != 0)
    {
        // Read out the delta value and reset it in one
        // atomic operation:

        Hal_interruptsDisable();

        delta = KnobDelta;
        KnobDelta = 0u;

        Hal_interruptsEnable();
    }

    return delta;
}
