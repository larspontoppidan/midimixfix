/*
 * quaddecode.c
 *
 *  Created on: Dec 9, 2012
 *      Author: lars
 */


#include "common.h"
#include "quaddecode.h"
#include "hal.h"

// This module decodes quadrature waveforms using an algorithm suited
// for the situation where two separate interrupt sources indicate
// separately when there are changes on two quadrature signals,
// named A and B:
//
// Mechanically stable positions: v
//           v           v           v
//          ___       _____       _____
// A wavef:    |_____|     |_____|
//          ______       _____       __
// B wavef:       |_____|     |_____|
//
// Real life experience with cheap rotary encoders show that they
// often have problems making a proper grounding of the signals, while
// the pull up resistors always get the job done. So, in reality the
// situation may look like this:
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
    bool_t  overflow;
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

static inline void resetTimer_ISR(stableTimer_t *p)
{
    p->value = HAL_FAST_TIMER_GET();
    p->overflow = FALSE;
}

// Returns true if stable timer has run out, otherwise false
static inline bool_t checkTimer_ISR(stableTimer_t *p)
{
    if ((p->overflow) ||
        ((uint8_t)(HAL_FAST_TIMER_GET() - p->value) > STABLE_TIMEOUT))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// Since the stable timer overflows each 13.1 ms, this function must be called
// at an interval at least 13.1 ms - STABLE_TIMEOUT, and not faster than STABLE_TIMEOUT
static inline void overflowSecureTimer_ISR(stableTimer_t *p)
{
    if (p->overflow == FALSE)
    {
        if ((uint8_t)(HAL_FAST_TIMER_GET() - p->value) > STABLE_TIMEOUT)
        {
            p->overflow = TRUE;
        }
    }
}


// --- Public functions


void quaddecode_initialize(void)
{
    StateNeitherHi = FALSE;
    StableTimerA.overflow = TRUE;
    StableTimerB.overflow = TRUE;
    KnobDelta = 0;
    KnobPushedDelta = 0;
}


void quaddecode_handleAChange_ISR(bool_t b_value, bool_t pushed)
{
    resetTimer_ISR(&StableTimerA);

    if ((b_value != 0) && (checkTimer_ISR(&StableTimerB)))
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
    resetTimer_ISR(&StableTimerB);

    if ((a_value != 0) && (checkTimer_ISR(&StableTimerA)))
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
    overflowSecureTimer_ISR(&StableTimerA);
    overflowSecureTimer_ISR(&StableTimerB);
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
