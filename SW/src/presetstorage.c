//
// Filename    : curvefilter.c
// Code module : Filter for transforming continuous controller message values
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

// EEPROM LAYOUT:
//
// First, the header:
//
//    uint8_t Sig[4];         // Should be "MMFX" where X indicates version of EEPROM layout ('2' currently)
//    uint8_t PresetSlots;    // Number of preset slots
//    uint8_t PresetSlotSize; // Size of preset slots
//
// After the header comes the first preset slot occupying PRESET_SLOT_SIZE
// After that, comes the next preset slot, etc.
//
// Each preset slot has the following layout:
//
//    uint8_t InstanceCount   // If InstanceCount == 0, it means preset is empty
//
//    for each instance
//    {
//      uint32_t InstanceUniqueId
//      uint8_t  RouteIn
//      uint8_t  RouteOut
//      uint8_t  InstanceDataLength
//      uint8_t  InstanceData[InstanceDataLength]
//    }
//
//    uint8_t PresetSlotChecksum
//
// END
//
// The PresetSlot Checksum is an arithmetic sum of all bytes in preset slot
// added with EEPROM_CHKSUM_RESET


// HINT:
// To read out eeprom for investigation, using AVRDUDE:
//     avrdude -pm644p -cavrisp2 -Pusb -U eeprom:r:data.eep:r
// To investigate the data use
//     hexdump -C -v data.eep



// -------------------------------  INCLUDES  -----------------------------------

#include "common.h"
#include "util.h"
#include "midiprocessing.h"
#include "filters.h"
#include "presetstorage.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "errors.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------


#define EEPROM_ADDR_HEADER      0
#define EEPROM_HEADER_SIZE      6

#define EEPROM_ADDR_FIRST_SLOT  (EEPROM_ADDR_HEADER + EEPROM_HEADER_SIZE)


#define EEPROM_CHKSUM_RESET     0xAB

#define SIG_1   'M'
#define SIG_2   'M'
#define SIG_3   'F'
#define SIG_4   2

#define PRESET_SLOT_SIZE   200   // 2048 bytes of EEPROM / 10 slots

// From header file:
// #define PRESET_SLOTS


static const uint8_t EepromHeader[EEPROM_HEADER_SIZE] PROGMEM =
{
        SIG_1,
        SIG_2,
        SIG_3,
        SIG_4,
        PRESETS_SLOTS,
        PRESET_SLOT_SIZE
};


// Saving and loading configuration requires a temporary buffer
#define DATA_BUFFER_SIZE  30



// -------------------------------  VARIABLES  ----------------------------------

// Buffer for saving / loading config
static uint8_t DataBuffer[DATA_BUFFER_SIZE];
static uint8_t DataBufferSize;

// Current state of reading / writing
static uint16_t Address;
static uint8_t ChkSum;

// ------------------------------  PROTOTYPES  ----------------------------------


static void writeDataBuffer(void);
static void writeByte(uint8_t x);
static void writeLong(uint32_t x);

static void     readDataBuffer(void);
static uint8_t  readByte(void);
static uint32_t readLong(void);

static void   writeEepromHeader(void);
static void writeEmptySlot(uint8_t slot);


// --------------------------  PRIVATE FUNCTIONS  ------------------------------


// Write DataBufferSize bytes from DataBuffer to EEPROM, updating ChkSum
static void writeDataBuffer(void)
{
    uint8_t i;
    uint8_t x;

    if (DataBufferSize <= DATA_BUFFER_SIZE)
    {
        for (i = 0; i < DataBufferSize; i++)
        {
            x = DataBuffer[i];
            eeprom_write_byte((uint8_t*)(Address++), x);
            ChkSum ^= x;
        }
    }
    else
    {
        err_raise_MAIN(ERR_MODULE_PRESETS, __LINE__);
    }
}

// Read DataBufferSize bytes into DataBuffer from EEPROM, updating ChkSum
static void readDataBuffer(void)
{
    uint8_t i;
    uint8_t x;

    if (DataBufferSize <= DATA_BUFFER_SIZE)
    {
        for (i = 0; i < DataBufferSize; i++)
        {
            x = eeprom_read_byte((const uint8_t*)(Address++));
            DataBuffer[i] = x;
            ChkSum ^= x;
        }
    }
    else
    {
        err_raise_MAIN(ERR_MODULE_PRESETS, __LINE__);
    }
}

// Read one byte from EEPROM, updating ChkSum
static uint8_t readByte(void)
{
    uint8_t x;

    x = eeprom_read_byte((const uint8_t*)(Address++));
    ChkSum ^= x;

    return x;
}

// Write one byte to EEPROM, updating ChkSum
static void writeByte(uint8_t x)
{
    eeprom_write_byte((uint8_t*)(Address++), x);
    ChkSum ^= x;
}


// Read four bytes from EEPROM, updating ChkSum, return as long
static uint32_t readLong(void)
{
    uint8_t a, b, c, d;

    // Get the four bytes
    a = eeprom_read_byte((const uint8_t*)(Address++));
    ChkSum ^= a;
    b = eeprom_read_byte((const uint8_t*)(Address++));
    ChkSum ^= b;
    c = eeprom_read_byte((const uint8_t*)(Address++));
    ChkSum ^= c;
    d = eeprom_read_byte((const uint8_t*)(Address++));
    ChkSum ^= d;

    // Return as a long
    return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d;
}

// Write one byte to EEPROM, updating ChkSum
static void writeLong(uint32_t x)
{
    uint8_t b;

    b = (uint8_t) (x >> 24);
    ChkSum ^= b;
    eeprom_write_byte((uint8_t*)(Address++), b);

    b = (uint8_t) (x >> 16);
    ChkSum ^= b;
    eeprom_write_byte((uint8_t*)(Address++), b);

    b = (uint8_t) (x >> 8);
    ChkSum ^= b;
    eeprom_write_byte((uint8_t*)(Address++), b);

    b = (uint8_t) (x);
    ChkSum ^= b;
    eeprom_write_byte((uint8_t*)(Address++), b);
}

static void writeEepromHeader(void)
{
    uint8_t i;

    Address = EEPROM_ADDR_HEADER;

    for (i = 0; i < EEPROM_HEADER_SIZE; i++)
    {
        writeByte(pgm_read_byte(&(EepromHeader[i])));
    }
}

static void writeEmptySlot(uint8_t slot)
{
    // Calc address of the slot we are about to load/check
    Address = (uint16_t)EEPROM_ADDR_FIRST_SLOT +
              (uint16_t)PRESET_SLOT_SIZE * (uint16_t)slot;

    ChkSum = EEPROM_CHKSUM_RESET;

    // Write filter steps = 0 and checksum
    writeByte(0);
    writeByte(ChkSum);
}



// ---------------------------  PUBLIC FUNCTIONS  ------------------------------


bool_t presets_validateEeprom(void)
{
    uint8_t i;
    bool_t success = TRUE;

    Address = EEPROM_ADDR_HEADER;

    for (i = 0; i < EEPROM_HEADER_SIZE; i++)
    {
        if (pgm_read_byte(&(EepromHeader[i])) != readByte())
        {
            success = FALSE;
        }
    }

    return success;
}

void presets_resetEeprom(void)
{
    uint8_t i;

    // This eeprom is unitialized or using an incorrect format

    // Initialize
    writeEepromHeader();

    for (i = 0; i < PRESETS_SLOTS; i++)
    {
        writeEmptySlot(i);
    }
}

uint8_t presets_load(uint8_t slot, bool_t test)
{
    uint8_t ret = PRESET_OK;
    uint8_t fsteps;
    uint8_t i;

    if (test == FALSE)
    {
        // Remove all current filters
        midiproc_removeAllFilters_MAIN();
    }

    // Calc address of the slot we are about to load/check
    Address = (uint16_t)EEPROM_ADDR_FIRST_SLOT +
              (uint16_t)PRESET_SLOT_SIZE * (uint16_t)slot;

    ChkSum = EEPROM_CHKSUM_RESET;

    // Get number of filter steps in that slot
    fsteps = readByte();

    for (i = 0; i < fsteps; i++)
    {
        uint8_t filter_type;
        midiproc_route_t route;

        // Load filter data
        filter_type = filters_findFilterType(readLong());
        route.In = readByte();
        route.Out = readByte();
        DataBufferSize = readByte();
        readDataBuffer();

        if (filter_type != FILTERS_FILTER_NOT_FOUND)
        {
            if (test == FALSE)
            {
                if (!midiproc_loadFilter_MAIN(filter_type, route, DataBuffer))
                {
                    ret = PRESET_LOAD_ERROR;
                }
            }
        }
        else
        {
            ret = PRESET_INVALID_UID;
        }
    }

    if (ret == PRESET_OK)
    {
        // Now read checksum.
        (void)readByte();

        // Since it's an XOR checksum, we must be back to zero now
        if (ChkSum != 0)
        {
            ret = PRESET_CHKSUM_ERROR;
        }
        else
        {
            // Final check is to see if this was infact empty...
            if (fsteps == 0)
            {
                // Empty slot is not OK for loading, as it would result in problems
                ret = PRESET_EMPTY;
            }
        }
    }

    return ret;
}

uint8_t presets_save(uint8_t slot)
{
    uint8_t i;
    uint8_t fsteps;

    fsteps = midiproc_getFilterSteps_SAFE();

    // Start by checking if the save is too big
    uint16_t size = 2;
    for (i = 0; i < fsteps; i++)
    {
        filters_instance_t instance = midiproc_getFilterInstance_SAFE(i);
        size += 7 + filters_getConfigSize(instance.FilterType);
    }

    if (size >= PRESET_SLOT_SIZE)
    {
        return PRESET_SAVE_TOO_BIG;
    }

    // Calc address of the slot we are about to load/check
    Address = (uint16_t)EEPROM_ADDR_FIRST_SLOT +
              (uint16_t)PRESET_SLOT_SIZE * (uint16_t)slot;

    ChkSum = EEPROM_CHKSUM_RESET;

    // Write number of filter steps
    writeByte(fsteps);

    for (i = 0; i < fsteps; i++)
    {
        filters_instance_t instance = midiproc_getFilterInstance_SAFE(i);
        midiproc_route_t route = midiproc_getFilterRoute_SAFE(i);

        // Write unique id
        writeLong(filters_getUniqueId(instance.FilterType));

        // Write route
        writeByte(route.In);
        writeByte(route.Out);

        // Fetch filter config
        DataBufferSize = filters_getConfigSize(instance.FilterType);
        if (DataBufferSize <= DATA_BUFFER_SIZE)
        {
            filters_saveConfig(instance, DataBuffer);
        }
        else
        {
            // Not enough room in buffer!
            err_raise_MAIN(ERR_MODULE_PRESETS, __LINE__);
            DataBufferSize = 0;
        }

        // Write size of data and buffer contents
        writeByte(DataBufferSize);
        writeDataBuffer();
    }

    // Finally write checksum
    writeByte(ChkSum);

    return PRESET_OK;
}



