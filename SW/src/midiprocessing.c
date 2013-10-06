/*
 * midiprocessing.c
 *
 *  Created on: Sep 11, 2013
 *      Author: lars
 */


#include "midiprocessing.h"

#include "common.h"
#include "midimessage.h"
#include "filters.h"
#include "filtersteps.h"
#include "errors.h"
#include "hal.h"

#include <util/delay.h>
#include <string.h>


#define MIDIPROC_BUFFER_SIZE 4u


static uint8_t   BufferFilterStep[MIDIPROC_BUFFER_SIZE];
static midiMsg_t BufferMsg[MIDIPROC_BUFFER_SIZE];

static uint8_t BufferHead;  // TODO can we live without volatile here?
static uint8_t BufferTail;

static bool_t volatile ProcessingNow; // Only ISR access

static bool_t volatile ProcessingEnabled;


static void processMessages_ISR(void);

static void processMessages_ISR(void)
{
    ProcessingNow = TRUE;

    while (BufferHead != BufferTail)
    {
        while (BufferMsg[BufferTail].Route != MIDIMSG_ROUTE_INACTIVE)
        {
            filter_t *filter;

            filter = fsteps_getFilter_SAFE(BufferFilterStep[BufferTail]);

            if (filter != NULL)
            {
                // Does this filter specify this route as input?
                if ((filter->RouteIn == MIDIMSG_ROUTE_ALL) ||
                    (filter->RouteIn == BufferMsg[BufferTail].Route))
                {
                    filter_processMidiMsg(filter, &(BufferMsg[BufferTail]));
                }

                // Increment to next filter step
                BufferFilterStep[BufferTail]++;
            }
            else
            {
                // The filter step is invalid. This means msg ran through the
                // processing chain without getting dismissed.

                // Discarding the message
                BufferMsg[BufferTail].Route = MIDIMSG_ROUTE_INACTIVE;
            }
        }

        // We are done processing this msg which means it is discarded,
        // increment BufferTail

        BufferTail++;
        if (BufferTail >= MIDIPROC_BUFFER_SIZE)
        {
            BufferTail = 0u;
        }
    }

    ProcessingNow = FALSE;
}

void midiproc_initialize(void)
{
    ProcessingEnabled = FALSE;
    BufferHead = 0u;
    BufferTail = 0u;
    ProcessingNow = FALSE;
}

void midiproc_handleTick_ISR(void)
{
    processMessages_ISR();
}

void midiproc_addMessage_ISR(midiMsg_t *msg, uint8_t filter_step)
{
    uint8_t msg_index;

    // Lets use next free number in buffer
    msg_index = BufferHead;

    // Increment head variable
    BufferHead++;

    if (BufferHead >= MIDIPROC_BUFFER_SIZE)
    {
        BufferHead = 0u;
    }

    if (BufferHead == BufferTail)
    {
        // Buffer overflow. We can't handle this situation nicely.
        // Discard message and assert error
        err_raise_ISR(ERR_MODULE_MIDIPROC, __LINE__);

        BufferHead = msg_index;
    }
    else
    {
        // Setup new message
        BufferFilterStep[msg_index] = filter_step;
        memcpy(&(BufferMsg[msg_index]), msg, sizeof(midiMsg_t));

        // Is this call a consequence of some other messageprocessing?
        // In that case, we don't need to do the process message call:
        if (ProcessingNow == FALSE)
        {
            processMessages_ISR();
        }
    }
}

void midiproc_addMessage_MAIN(midiMsg_t *msg, uint8_t filter_step)
{
    bool_t success;

    if (ProcessingEnabled)
    {
        do
        {
            uint8_t msg_index;

            // Pull number for next free number in buffer in a safe region

            hal_interruptsDisable();
            msg_index = BufferHead;

            // Increment head variable
            BufferHead++;

            if (BufferHead >= MIDIPROC_BUFFER_SIZE)
            {
                BufferHead = 0u;
            }

            if (BufferHead == BufferTail)
            {
                // Oops, there wasn't room, pull back change of BufferHead
                BufferHead = msg_index;

                hal_interruptsEnable();

                // Take a little break before trying this again
                _delay_ms(10);

                success = FALSE;
            }
            else
            {
                // We got a slot in the buffer, setup new message
                BufferFilterStep[msg_index] = filter_step;
                memcpy(&(BufferMsg[msg_index]), msg, sizeof(midiMsg_t));

                // We are done. Message will be processed at next tick
                hal_interruptsEnable();

                success = TRUE;
            }

        } while (success == FALSE);
    }

}


void midiproc_start_MAIN(void)
{
    // TODO implement this in a nice and failsafe manner
    ProcessingEnabled = TRUE;
}

void midiproc_stop_MAIN(void)
{
    ProcessingEnabled = FALSE;
}
