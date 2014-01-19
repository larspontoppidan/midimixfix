
//
// Filename    : midiprocessing.c
// Code module : Handling of the midi processing filter chain
// Project     : Midimixfix
// URL         : http://larsee.dk
//
// --------------------------------  LICENSE  -----------------------------------
//
// Copyright (c) 2013, Lars Ole Pontoppidan (Larsp)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// *  Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// *  Neither the name of the original author (Lars Ole Pontoppidan) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//
// ------------------------------  DESCRIPTION  ---------------------------------
//
// This module takes care of the processing chain for midi messages


// -------------------------------  INCLUDES  -----------------------------------

#include "midiprocessing.h"

#include "common.h"
#include "midimessage.h"
#include "filters.h"
#include "errors.h"
#include "hal.h"
#include "Filters/midiin.h"

#include <util/delay.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------

#define MSG_BUFFER_SIZE 15u

#define FILTERSTEPS_MAX 20

#define SYNC_WAIT_MS  1

// ----------------------------  LOCAL VARIABLES  -------------------------------


// The processing chain consists of filter instances and their routing setup:
static filters_instance_t FilterInstances[FILTERSTEPS_MAX];
static midiproc_route_t   FilterRoutes[FILTERSTEPS_MAX];
static uint8_t            FilterCount;

// The MIDI msg ring buffer holds midi messages and their step in filter processing chain
static uint8_t   MsgBufferFilterStep[MSG_BUFFER_SIZE];
static midiMsg_t MsgBuffer[MSG_BUFFER_SIZE];

static uint8_t   MsgBufferHead;  // TODO can we live without volatile here?
static uint8_t   MsgBufferTail;

static bool_t volatile ProcessingNow; // Only ISR access
static bool_t volatile ProcessingEnabled;

// ------------------------------  PROTOTYPES  ----------------------------------

static void processMessages_ISR(void);
static void correctFilterRoute(uint8_t filter_type, midiproc_route_t *route);

// ---------------------------  PRIVATE FUNCTIONS  -------------------------------

static void processMessages_ISR(void)
{
    ProcessingNow = TRUE;

    while (MsgBufferHead != MsgBufferTail)
    {
        // Process buffered message at index MsgBufferTail

        // Fetch the filter that made this message, identified by it's step in the filter chain:
        uint8_t step = MsgBufferFilterStep[MsgBufferTail];

        // The filter's route out setting sets the msg's initial route:
        uint8_t route = FilterRoutes[step].Out;

        // The first filter to attempt processing is the _next_ filter in chain
        step++;

        // Keep processing until route is inactive or we have done all filters in chain:
        while ((route != MIDIMSG_ROUTE_INACTIVE) && (step < FilterCount))
        {
            // Does this filter specify this route as input?
            if ((FilterRoutes[step].In == MIDIMSG_ROUTE_ALL) ||
                (FilterRoutes[step].In == route))
            {
                // It does, have filter process the message
                uint8_t result = filters_processMidiMsg(FilterInstances[step], &(MsgBuffer[MsgBufferTail]));

                // Deal with result of processing
                switch (result)
                {
                case FILTER_PROCESS_DID:
                    // Filter did select the message for processing,
                    // msg must go on the filter's "out" route, if it isn't set to All:
                    if (FilterRoutes[step].Out != MIDIMSG_ROUTE_ALL)
                    {
                        route = FilterRoutes[step].Out;
                    }
                    break;

                case FILTER_PROCESS_DISCARD:
                    // Filter chose to discard message, set route to inactive
                    route = MIDIMSG_ROUTE_INACTIVE;
                    break;

                case FILTER_PROCESS_NEUTRAL:
                    // Filter chose not to process msg. No changes.
                    break;
                }
            }

            // Increment to next filter step
            step++;
        }

        // Move on to next msg, increment MsgBufferTail:
        MsgBufferTail++;
        if (MsgBufferTail >= MSG_BUFFER_SIZE)
        {
            MsgBufferTail = 0u;
        }
    }

    ProcessingNow = FALSE;
}

// Correct the route according to what's allowed for the filter's operating mode
static void correctFilterRoute(uint8_t filter_type, midiproc_route_t *route)
{
    // TODO: check lower/upper bounds on route?

    switch (filters_getFilterMode(filter_type))
    {
    case FILTER_MODE_IN:
        // Pure input filter, make sure Out is inactive
        route->Out = MIDIMSG_ROUTE_INACTIVE;
        break;

    case FILTER_MODE_OUT:
        // Pure output filter, make sure In is inactive
        route->In = MIDIMSG_ROUTE_INACTIVE;

        // and make sure Out is not a wildcard route
        if (route->Out == MIDIMSG_ROUTE_ALL)
        {
            route->Out = MIDIMSG_ROUTE_INACTIVE;
        }
        break;

    case FILTER_MODE_IN_OUT:
        // Filter with both in and out possibilities, don't apply any restrictions
        break;

    case FILTER_MODE_PROCESSOR:
        // Processing filters must operating on the same input and output route.
        // Use the input for governing route:
        route->Out = route->In;
        break;
    }
}

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

void midiproc_initialize(void)
{
    ProcessingEnabled = FALSE;
    MsgBufferHead = 0u;
    MsgBufferTail = 0u;
    ProcessingNow = FALSE;

    FilterCount = 0;
}

void midiproc_loadDefaultFilters(void)
{
    midiproc_removeAllFilters_MAIN();

    midiproc_addFilter_MAIN(FILTER_TYPE_MIDIIN_IN1);
    midiproc_addFilter_MAIN(FILTER_TYPE_MIDIIN_IN2);
    midiproc_addFilter_MAIN(FILTER_TYPE_MIDIOUT_FILTER);
}


void midiproc_handleTick_ISR(void)
{
    processMessages_ISR();
}

void midiproc_addMessage_ISR(midiMsg_t *msg, uint8_t filter_step)
{
    uint8_t msg_index;

    // Lets use next free number in buffer
    msg_index = MsgBufferHead;

    // Increment head variable
    MsgBufferHead++;

    if (MsgBufferHead >= MSG_BUFFER_SIZE)
    {
        MsgBufferHead = 0u;
    }

    if (MsgBufferHead == MsgBufferTail)
    {
        // Buffer overflow. We can't handle this situation nicely.
        // Discard message and assert error
        err_raise_ISR(ERR_MODULE_MIDIPROC, __LINE__);

        MsgBufferHead = msg_index;
    }
    else
    {
        // Setup new message
        MsgBufferFilterStep[msg_index] = filter_step;
        MsgBuffer[msg_index] = *msg;

        // If ProcessingNow flag is true, this function call is a consequence
        // of some message processing initiated by processMessages. In that case
        // the processMessages function loop will move on to the message by
        // itself and we shouldn't call it.

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
            msg_index = MsgBufferHead;

            // Increment head variable
            MsgBufferHead++;

            if (MsgBufferHead >= MSG_BUFFER_SIZE)
            {
                MsgBufferHead = 0u;
            }

            if (MsgBufferHead == MsgBufferTail)
            {
                // Oops, there wasn't room, pull back change of MsgBufferHead
                MsgBufferHead = msg_index;

                hal_interruptsEnable();

                // Take a little break before trying this again
                _delay_ms(SYNC_WAIT_MS);

                success = FALSE;
            }
            else
            {
                // We got a slot in the buffer, setup new message
                MsgBufferFilterStep[msg_index] = filter_step;
                MsgBuffer[msg_index] = *msg;

                // We are done. Message will be processed at next tick
                hal_interruptsEnable();

                success = TRUE;
            }

        } while (success == FALSE);
    }
}


void midiproc_start_MAIN(void)
{
    // Go right ahead and start processing again

    ProcessingEnabled = TRUE;
    hal_interruptsEnable();    // TODO push pop this instead
}

void midiproc_stop_MAIN(void)
{
    // Now messages may be trickling through the processing system and we need
    // to turn off interrupts and shut down processing such that changes to the processing
    // chain can be made safely.

    // Wait for midiinputs, midioutputs and processing chain to be empty before commencing.

    while (ProcessingEnabled)
    {
        if (midiin_getIsReceivingStatus() == FALSE)
        {
            // Thats a start. Now we have at least a couple of bytes time before
            // more action happens on the midi inputs

            // Go into safe region and check MsgBuffer and midi transmission status
            hal_interruptsDisable();

            if ((MsgBufferHead == MsgBufferTail) && hal_getMidiTxBufferEmpty_ISR())
            {
                // Buffer empty and nothing in TX buffer!
                // We can stop processing now. Stay in safe region
                ProcessingEnabled = FALSE;
            }
            else
            {
                // Something wasn't finished, we have to try again in a little while
                hal_interruptsEnable();

                // Take a little break before trying this again
                _delay_ms(SYNC_WAIT_MS);
            }
        }
    }
}


// Filter management

uint8_t midiproc_getFilterSteps_SAFE(void)
{
    return FilterCount;
}

filters_instance_t midiproc_getFilterInstance_SAFE(uint8_t step)
{
    filters_instance_t ret;

    ret.FilterType = FILTER_CREATE_FAILED;
    ret.Instance = 0;

    if (step < FilterCount)
    {
        ret = FilterInstances[step];
    }

    return ret;
}

bool_t midiproc_addFilter_MAIN(uint8_t filter_type)
{
    uint8_t new_instance = FILTER_CREATE_FAILED;

    if (ProcessingEnabled == FALSE)
    {
        uint8_t new_filter_step = FilterCount;

        if (new_filter_step < FILTERSTEPS_MAX)
        {
            new_instance = filters_create(filter_type, new_filter_step);

            if (new_instance != FILTER_CREATE_FAILED)
            {
                FilterInstances[new_filter_step].FilterType = filter_type;
                FilterInstances[new_filter_step].Instance = new_instance;

                // Set default In/Out routes according to filter mode
                FilterRoutes[new_filter_step].In = 1;
                FilterRoutes[new_filter_step].Out = 1;

                // Apply the filter processing mode rules to the route settings:
                correctFilterRoute(filter_type, &(FilterRoutes[new_filter_step]) );
                FilterCount++;
            }
        }
        else
        {
            // Ran out of steps, return false
        }
    }
    else
    {
        // This is only allowed when processing is stopped!
        err_raise_MAIN(ERR_MODULE_MIDIPROC, __LINE__);
    }

    return (new_instance != FILTER_CREATE_FAILED);
}


bool_t midiproc_loadFilter_MAIN(uint8_t filter_type, midiproc_route_t route, uint8_t *filter_config)
{
    uint8_t new_instance = FILTER_CREATE_FAILED;

    if (ProcessingEnabled == FALSE)
    {
        uint8_t new_filter_step = FilterCount;

        if (new_filter_step < FILTERSTEPS_MAX)
        {
            new_instance = filters_create(filter_type, new_filter_step);

            if (new_instance != FILTER_CREATE_FAILED)
            {
                FilterInstances[new_filter_step].FilterType = filter_type;
                FilterInstances[new_filter_step].Instance = new_instance;

                // Set default In/Out routes according to filter mode
                FilterRoutes[new_filter_step] = route;

                // Apply the filter processing mode rules to the route settings:
                correctFilterRoute(filter_type, &(FilterRoutes[new_filter_step]) );

                // Load the filter config
                filters_loadConfig(FilterInstances[new_filter_step], filter_config);

                FilterCount++;
            }
        }
        else
        {
            // Ran out of steps, return false
        }
    }
    else
    {
        // This is only allowed when processing is stopped!
        err_raise_MAIN(ERR_MODULE_MIDIPROC, __LINE__);
    }

    return (new_instance != FILTER_CREATE_FAILED);
}


void midiproc_removeFilter_MAIN(uint8_t step)
{
    if (ProcessingEnabled == FALSE)
    {
        uint8_t i;

        // Send the destroy message to filter
        filters_destroy(FilterInstances[step]);

        FilterCount--;

        for (i = step; i < FilterCount; i++)
        {
            FilterInstances[i] = FilterInstances[i + 1];
            FilterRoutes[i] = FilterRoutes[i + 1];

            // Inform filter that step number has changed
            filters_setFilterStep(FilterInstances[i], i);
        }

    }
    else
    {
        // This is only allowed when processing is stopped!
        err_raise_MAIN(ERR_MODULE_MIDIPROC, __LINE__);
    }
}

void midiproc_removeAllFilters_MAIN(void)
{
    if (ProcessingEnabled == FALSE)
    {
        uint8_t i;

        // Send the destroy message to all filters

        for (i = 0; i < FilterCount; i++)
        {
            filters_destroy(FilterInstances[i]);
        }

        // Zero the filtercount
        FilterCount = 0;
    }
    else
    {
        // This is only allowed when processing is stopped!
        err_raise_MAIN(ERR_MODULE_MIDIPROC, __LINE__);
    }
}

void midiproc_swapFilters_MAIN(uint8_t step1, uint8_t step2)
{
    if (ProcessingEnabled == FALSE)
    {
        filters_instance_t tmpInstance;
        midiproc_route_t tmpRoute;

        tmpInstance = FilterInstances[step1];
        FilterInstances[step1] = FilterInstances[step2];
        FilterInstances[step2] = tmpInstance;

        tmpRoute = FilterRoutes[step1];
        FilterRoutes[step1] = FilterRoutes[step2];
        FilterRoutes[step2] = tmpRoute;

        // Inform filters that step numbers have changed
        filters_setFilterStep(FilterInstances[step1], step1);
        filters_setFilterStep(FilterInstances[step2], step2);
    }
    else
    {
        // This is only allowed when processing is stopped!
        err_raise_MAIN(ERR_MODULE_MIDIPROC, __LINE__);
    }
}


midiproc_route_t midiproc_getFilterRoute_SAFE(uint8_t step)
{
    return FilterRoutes[step];
}

void midiproc_setRoute_MAIN(uint8_t step, midiproc_route_t route)
{
    if (ProcessingEnabled == FALSE)
    {
        if (step < FilterCount)
        {
            correctFilterRoute(FilterInstances[step].FilterType, &route);
            FilterRoutes[step] = route;
        }
    }
    else
    {
        // This is only allowed when processing is stopped!
        err_raise_MAIN(ERR_MODULE_MIDIPROC, __LINE__);
    }
}

