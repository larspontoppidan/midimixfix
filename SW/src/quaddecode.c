/*
 * quaddecode.c
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */


#include "common.h"
#include "quaddecode.h"
#include "hal.h"

// This module decodes quadrature waveforms using an algorithm suited for the
// situation where two separate interrupt sources indicate separately when
// there are changes on quadrature signals, named A and B:
//
// Mechanically stable positions: v
//           v           v           v
//          ___       _____       _____
// A wavef:    |_____|     |_____|
//          ______       _____       __
// B wavef:       |_____|     |_____|
//
// Real life experience with cheap rotary encoders show that they often have
// problems making a good connection internally, which is used for grounding
// the waveforms, while the pull up resistors always get the job done.
// So, in reality the situation may look like this:
//          ___       _____       _____
// A wavef:    |||||||     |||||||
//          ______       _____       __
// B wavef:       |||||||     |||||||
//
// To reliably decode the above waveforms, with a proper noise rejection,
// the algorithm uses three states:
//
//   A:  A is stable high
//   B:  B is stable high
//   N:  Neither A nor B is stable high
//
// When A change interrupt fires, it will reset the A stable timer. Since
// we don't really know to what A is changing, we don't care about the value
// of A, but we check the status for B. If B is high and it's stable timer
// ran out, well, then we are in B stable high state. Otherwise we are in the
// no stable high state.
//
// Swap A and B in above and it describes what happens in the other interrupt.
//
// Now, we end up with the following development of states:
// (u indicates un-stable, in that stable timer has not yet run out)
//
//           v            v            v
//          ___       ______       _____
// A wavef:    |||||||u     |||||||u
//          ______       ______       __
// B wavef:       |||||||u     |||||||u
//
// State:      B  N    A    B  N    A
//
// The state transitions are translated as follows:
//
//  B -> N -> A must increment index by one
//  A -> N -> B must decrement index by one
//
// All other transitions are ignored.
//
// OTHER DEPENDENCIES:
// Hal module must provide a HAL_FAST_TIMER_GET() 8-bit free running timer
// incrementing each 51.2 us
//

#define STATE_A_HI  0
#define STATE_B_HI  1

static uint8_t State;
static bool_t  StateNeitherHi;

static int8_t volatile KnobDelta;
static int8_t volatile KnobPushedDelta;

// Stable timer
#define STABLE_TIMEOUT 5u // The stable timer will run out after between:
                          // STABLE_TIMEOUT and (STABLE_TIMEOUT + 1) x 51.2 us

typedef struct
{
    uint8_t value;
    bool_t  timeout;
} stableTimer_t;

static stableTimer_t StableTimerA;
static stableTimer_t StableTimerB;

// --- Private helper functions


static inline int8_t updateState(uint8_t new_state)
{
    int8_t delta = 0;

    if (StateNeitherHi && (new_state != State))
    {
        if (new_state == STATE_A_HI)
        {
            delta = -1;
        }
        else
        {
            delta = 1;
        }
    }

    State = new_state;
    StateNeitherHi = FALSE;

    return delta;
}

static inline void updateStateNeitherHi(void)
{
    StateNeitherHi = TRUE;
}

static inline void resetStableTimer_ISR(stableTimer_t *p)
{
    p->value = HAL_FAST_TIMER_GET();
    p->timeout = FALSE;
}


// Update the stable timer timeout flag.
// Since the 8-bit timer value overflows each 13.1 ms, this function must be called
// at an interval of at most 13.1 ms - STABLE_TIMEOUT, otherwise timeout detections
// may be missed.
static inline void checkStableTimer_ISR(stableTimer_t *p)
{
    // The stable timer may have exceeded timeout and had it catched
    // already, in that case timeout will be true. In other cases,
    // the timer value must be checked.

    if (p->timeout == FALSE)
    {
        if ((uint8_t)(HAL_FAST_TIMER_GET() - p->value) > STABLE_TIMEOUT)
        {
            p->timeout = TRUE;
        }
    }
}


// --- Public functions


void quaddecode_initialize(void)
{
    StateNeitherHi = FALSE;
    StableTimerA.timeout = TRUE;
    StableTimerB.timeout = TRUE;
    KnobDelta = 0;
    KnobPushedDelta = 0;
}


void quaddecode_handleAChange_ISR(bool_t b_value, bool_t pushed)
{
    resetStableTimer_ISR(&StableTimerA);
    checkStableTimer_ISR(&StableTimerB);

    if ((b_value != FALSE) && (StableTimerB.timeout != FALSE))
    {
        int8_t delta = updateState(STATE_B_HI);

        if (pushed)
        {
            KnobPushedDelta += delta;
        }
        else
        {
            KnobDelta += delta;
        }
    }
    else
    {
        updateStateNeitherHi();
    }
}

void quaddecode_handleBChange_ISR(bool_t a_value, bool_t pushed)
{
    resetStableTimer_ISR(&StableTimerB);
    checkStableTimer_ISR(&StableTimerA);

    if ((a_value != FALSE) && (StableTimerA.timeout != FALSE))
    {
        int8_t delta = updateState(STATE_A_HI);

        if (pushed)
        {
            KnobPushedDelta += delta;
        }
        else
        {
            KnobDelta += delta;
        }
    }
    else
    {
        updateStateNeitherHi();
    }
}

void quaddecode_handleTickIsr_ISR(void)
{
    checkStableTimer_ISR(&StableTimerA);
    checkStableTimer_ISR(&StableTimerB);
}

int8_t quaddecode_getPushedDelta_MAIN(void)
{
    int8_t delta = 0;

    // Only go in an disable interrupts if necessary
    if (KnobPushedDelta != 0)
    {
        // Read out the delta value and reset it in one
        // atomic operation:

        hal_interruptsDisable();

        delta = KnobPushedDelta;
        KnobPushedDelta = 0u;

        hal_interruptsEnable();
    }

    return delta;
}

int8_t quaddecode_getDelta_MAIN(void)
{
    int8_t delta = 0;

    // Only go in an disable interrupts if necessary
    if (KnobDelta != 0)
    {
        // Read out the delta value and reset it in one
        // atomic operation:

        hal_interruptsDisable();

        delta = KnobDelta;
        KnobDelta = 0u;

        hal_interruptsEnable();
    }

    return delta;
}
