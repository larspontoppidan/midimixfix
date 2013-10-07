
//
// Filename    : midilog.c
// Code module : Logging and display of midi messages on 20x4 LCD
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
// ------------------------------  DESCRIPTION  ---------------------------------
//
// The midi log can either show the input picked up from In1/In2 with/without realtime messages,
// depending on how the IO setup is configured.
//
// It can also show the messages sent on Out after all processing / generating / filtering
// has been done.
//
//
// The log is a ring buffer with increasing number meaning newer message.
// Log[LogIndexIsr] is reserved for writing in the ISR and should not be displayed.
//
//
// The log display is arranged with newest message at bottom:
//
//
// LCD row 0: WindowBuffer[0]: LogIndex[LogIndexMain - RowOffset - 3]
// LCD row 1: WindowBuffer[1]: LogIndex[LogIndexMain - RowOffset - 2]
// LCD row 2: WindowBuffer[2]: LogIndex[LogIndexMain - RowOffset - 1]
// LCD row 3: WindowBuffer[3]: LogIndex[LogIndexMain - RowOffset]
//
// Note the LogIndex lookups must be made respecting the ring buffer configuration,
// thus, if (LogIndexMain - RowOffset - X) is negative, it must added LOG_SIZE.
//


// -------------------------------  INCLUDES  -----------------------------------

#include "common.h"
#include "midilog.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "errors.h"
#include "lcd.h"
#include "util.h"
#include <string.h>
#include "ui.h"

// --------------------------  TYPES AND CONSTANTS  -----------------------------

#define LOG_SIZE 32

#define MIDILOG_SOURCE_OFF 0


#define BUFFER_MAX 32



// ----------------------------  LOCAL VARIABLES  -------------------------------

midiMsg_t Log[LOG_SIZE];

uint8_t volatile LogIndexIsr;

uint8_t LogSource;   // If this is 0, no source is selected and log is offline
bool_t  LogFiltered;
bool_t  LogPaused;

// Window:

uint8_t LogIndexMain; // The latest log index seen from main thread's perspective
bool_t NeedRefresh;
int8_t RowOffset;
int8_t ColOffset;

// Window buffer

uint8_t WindowBuffer[LCD_ROWS][BUFFER_MAX];


// ------------------------------  PROTOTYPES  ----------------------------------

static void addMsgToLog(midiMsg_t *msg);
static void writeWindowBuffer(void);
static void resetLog(void);
static void resetPan(void);
static bool_t checkForNewMsg(void);
static void writeDisplay(void);

// ---------------------------  PRIVATE FUNCTIONS  ------------------------------

static void addMsgToLog(midiMsg_t *msg)
{
    if (LogIndexIsr < LOG_SIZE)
    {
        memcpy(&(Log[LogIndexIsr]), msg, sizeof(midiMsg_t));
    }
    else
    {
        err_raise(ERR_MODULE_MLOG, __LINE__);
    }

    LogIndexIsr++;

    if (LogIndexIsr >= LOG_SIZE)
    {
        LogIndexIsr = 0;
    }
}

static void writeWindowBuffer(void)
{
    uint8_t row;
    uint8_t index;

    char *ptr;

    for (row = 0; row < LCD_ROWS; row++)
    {
        ptr = (char*)(WindowBuffer[row]);

        // Calculate location in log ring buffer
        index = LogIndexMain - (LCD_ROWS - 1) + row - RowOffset;

        // Loop around if we got below 0
        if (((int8_t)index) < 0)
        {
            index += LOG_SIZE;
        }

        // Write raw decode
        ptr = midimsg_writeRaw(ptr, &(Log[index]));

        // Pad with spaces until we are at col 8, or at least one
        do
        {
            (*ptr++) = ' ';
        } while (ptr < (char*)&(WindowBuffer[row][8]));

        // Write parsed message
        ptr = midimsg_writeParsed(ptr, &(Log[index]));

        // Write spaces for the rest of this row
        while (ptr < (char*)&(WindowBuffer[row][BUFFER_MAX]))
        {
            (*ptr++) = ' ';
        }
    }
}

static void resetLog(void)
{
    memset(&Log, 0, sizeof(Log));

    LogIndexIsr = 1u;
    LogIndexMain = 0u;
}


static void resetPan(void)
{
    RowOffset = 0;
    ColOffset = 8;
}


static bool_t checkForNewMsg(void)
{
    bool_t ret;
    uint8_t i;

    // Calculate index of record just before LogIndexIsr
    i = LogIndexIsr;

    if (i == 0)
    {
        i = LOG_SIZE-1;
    }
    else
    {
        i--;
    }

    if (i == LogIndexMain)
    {
        // No new messages
        ret = FALSE;
    }
    else
    {
        // LogIndexIsr has moved since last time, update main index
        LogIndexMain = i;
        ret = TRUE;
    }

    return ret;
}

static void writeDisplay(void)
{
    uint8_t row;
    uint8_t col;
    int8_t i;

    for (row = 0; row < LCD_ROWS; row++)
    {
        lcd_setCursor(row, 0);

        // Blit data from window buffer to lcd
        for (col = 0; col < LCD_COLUMNS; col++)
        {
            // Inside buffer?
            i = col + ColOffset;
            if ((i >= 0) && (i < BUFFER_MAX))
            {
                lcd_write(WindowBuffer[row][i]);
            }
            else
            {
                lcd_write(' ');
            }
        }
    }

    lcd_setCursor(0, LCD_COLUMNS - 2);

    // Write source

    if (LogSource == MIDILOG_SOURCE_IN1)
    {
        lcd_write('1');
    }
    else if (LogSource == MIDILOG_SOURCE_IN2)
    {
        lcd_write('2');
    }
    else if (LogSource == MIDILOG_SOURCE_OUT)
    {
        lcd_write('O');
    }

    // Write P if we are paused
    lcd_write(LogPaused ? 'P' : ' ');

    // Set cursor on top right position
    lcd_setCursor(0, LCD_COLUMNS-1);
}


// ---------------------------  PUBLIC FUNCTIONS  -------------------------------

void midilog_initialize(void)
{
    resetPan();

    LogSource = MIDILOG_SOURCE_OFF;
    LogPaused = FALSE;
    LogFiltered = FALSE;
    LogIndexMain = 0u;
    LogIndexIsr = 1u;
    NeedRefresh = TRUE;
}

void midilog_handleMidiIn1_ISR(midiMsg_t *msg)
{
    if ((LogSource == MIDILOG_SOURCE_IN1) && !LogPaused)
    {
        addMsgToLog(msg);
    }
}


void midilog_handleMidiIn1RtSysx_ISR(midiMsg_t *msg)
{
    if ((LogSource == MIDILOG_SOURCE_IN1) && !LogPaused && !LogFiltered)
    {
        addMsgToLog(msg);
    }
}


void midilog_handleMidiIn2_ISR(midiMsg_t *msg)
{
    if ((LogSource == MIDILOG_SOURCE_IN2) && !LogPaused)
    {
        addMsgToLog(msg);
    }
}

void midilog_handleMidiIn2RtSysx_ISR(midiMsg_t *msg)
{
    if ((LogSource == MIDILOG_SOURCE_IN2) && !LogPaused && !LogFiltered)
    {
        addMsgToLog(msg);
    }
}

void midilog_handleMidiOut_ISR(midiMsg_t *msg)
{
    if ((LogSource == MIDILOG_SOURCE_OUT) && !LogPaused)
    {
        addMsgToLog(msg);
    }
}


void midilog_handleMidiOutRtSysx_ISR(midiMsg_t *msg)
{
    if ((LogSource == MIDILOG_SOURCE_OUT) && !LogPaused && !LogFiltered)
    {
        addMsgToLog(msg);
    }
}


void midilog_handleMainLoop(void)
{
    if (LogSource != MIDILOG_SOURCE_OFF)
    {
        if (LogPaused == FALSE)
        {
            // We are live
            if (checkForNewMsg())
            {
                NeedRefresh = TRUE;

                // Move to last row when there are new messages
                RowOffset = 0u;
            }
        }

        if (NeedRefresh)
        {
            // Better update log then
            NeedRefresh = FALSE;
            writeWindowBuffer();
            writeDisplay();
        }
    }
}

void midilog_configure(uint8_t source, bool_t filtered)
{
    // Enable log with given source
    LogSource = source;
    LogPaused = FALSE;
    LogFiltered = filtered;

    resetLog();
    NeedRefresh = TRUE;
}

void midilog_stop(void)
{
    LogSource = MIDILOG_SOURCE_OFF;
}

void midilog_handleUi(uint8_t uiEvent)
{
    switch (uiEvent)
    {
    case UI_EVENT_MOVE_UP:
        RowOffset = util_boundedAddInt8(RowOffset, 0, LOG_SIZE - LCD_ROWS, -1);
        break;
    case UI_EVENT_MOVE_DOWN:
        RowOffset = util_boundedAddInt8(RowOffset, 0, LOG_SIZE - LCD_ROWS, 1);
        break;
    case UI_EVENT_MOVE_UP | UI_MOVE_FAST_MASK:
        ColOffset = util_boundedAddInt8(ColOffset, -10, 10, 1);
        break;
    case UI_EVENT_MOVE_DOWN | UI_MOVE_FAST_MASK:
        ColOffset = util_boundedAddInt8(ColOffset, -10, 10, -1);
        break;
    case UI_EVENT_SELECT:
        LogPaused = !LogPaused;
        break;
    }

    NeedRefresh = TRUE;
}
