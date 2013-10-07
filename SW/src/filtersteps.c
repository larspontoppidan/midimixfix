
//
// Filename    : filtersteps.c
// Code module : Enumeration of loaded filters in the processing chain
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
// The order and routing of the filter steps currently loaded is stored by this
// module
//

// TODO in general we should start/ stop midi processing when modifying filter chain

// -------------------------------  INCLUDES  -----------------------------------

#include "common.h"
#include "filters.h"
#include <string.h>

// --------------------------  TYPES AND CONSTANTS  -----------------------------

#define FILTERSTEPS_MAX 20

// ----------------------------  LOCAL VARIABLES  -------------------------------

filter_t FilterStep[FILTERSTEPS_MAX];
uint8_t FilterStepCount;

// ------------------------------  PROTOTYPES  ----------------------------------

// ---------------------------  PRIVATE FUNCTIONS  ------------------------------

// ---------------------------  PUBLIC FUNCTIONS  -------------------------------



void fsteps_initialize(void)
{
    FilterStepCount = 0;
}

uint8_t fsteps_getCount_SAFE(void)
{
    return FilterStepCount;
}

filter_t* fsteps_getFilter_SAFE(uint8_t step)
{
    filter_t *ret = NULL;

    if (step < FilterStepCount)
    {
        ret = &(FilterStep[step]);
    }

    return ret;
}

bool_t fsteps_addFilter_MAIN(uint8_t filter_type, uint8_t *config)
{
    bool_t success = FALSE;

    if (FilterStepCount < FILTERSTEPS_MAX)
    {
        filter_t *newfilter = &(FilterStep[FilterStepCount]);

        // Prepare new filter step. Clean data struct
        memset(newfilter, 0, sizeof(filter_t));

        // Set Instantiate
        newfilter->StepNumber = FilterStepCount;
        newfilter->RouteIn = MIDIMSG_ROUTE_ALL;
        newfilter->RouteOut = 1;
        newfilter->FilterType = filter_type;

        success = filter_new(filter_type, config, newfilter);

        if (success)
        {
            FilterStepCount++;
        }
    }

    return success;
}

void fsteps_removeFilter_MAIN(uint8_t step)
{
    uint8_t i;

    // Send the destroy message to filter
    filter_request(&(FilterStep[step]), FILTER_REQ_DESTROY);

    if (step < FilterStepCount)
    {
        FilterStepCount--;

        for (i = step; i < FilterStepCount; i++)
        {
            memcpy(&(FilterStep[i]), &(FilterStep[i+1]), sizeof(filter_t));

            // Update stepnumber variable
            FilterStep[i].StepNumber = i;

            // Inform filter that self struct was updated
            filter_request(&(FilterStep[i]), FILTER_REQ_UPDATE_SELF);
        }
    }

}

void fsteps_swapFilter_MAIN(uint8_t step1, uint8_t step2)
{
    filter_t tmp;


    memcpy(&tmp, &(FilterStep[step1]), sizeof(filter_t));

    memcpy(&(FilterStep[step1]), &(FilterStep[step2]), sizeof(filter_t));

    memcpy(&(FilterStep[step2]), &tmp, sizeof(filter_t));


    // Update stepnumber variables
    FilterStep[step1].StepNumber = step1;
    FilterStep[step2].StepNumber = step2;

    // Inform filter that self struct was updated
    filter_request(&(FilterStep[step1]), FILTER_REQ_UPDATE_SELF);
    filter_request(&(FilterStep[step2]), FILTER_REQ_UPDATE_SELF);

}
