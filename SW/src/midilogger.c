/*
 * mididisplay.c
 *
 *  Created on: Dec 14, 2012
 *      Author: lars
 */

#include "common.h"

#include "common.h"
#include "midilogger.h"
#include "midimessage.h"
#include "midigenerics.h"
#include "midiio.h"
#include "errors.h"
#include "lcd.h"
#include "util.h"
#include <string.h>
#include "menu.h"

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


// The log buffer

#define LOG_SIZE 32
midiMsg_t Log[LOG_SIZE];
uint8_t volatile LogIndexIsr;

// Log controlling variables

#define LOG_MODE_OFF     0
#define LOG_MODE_ACTIVE  1
#define LOG_MODE_PAUSED1 2
#define LOG_MODE_PAUSED2 3

uint8_t LogMode;

bool_t LogEnabled;  // If true, logging is enabled
bool_t LogInNotOut; // If true, input is logged, false then output is logged


// Window:

uint8_t LogIndexMain; // The latest log index seen from main thread's perspective
bool_t NeedRefresh;
int8_t RowOffset;
int8_t ColOffset;

// Window buffer

#define BUFFER_MAX 32

uint8_t WindowBuffer[LCD_ROWS][BUFFER_MAX];


// Strings

static char PStrTitle[] PROGMEM =  "Show midi log (...)";

// The user can toggle the different modes with the select button
//   "*Log. In *"
//   "*Log. Out*"
//   "*Stop In *"
//   "*Stop Out*"

static char PStrOFF[] PROGMEM = "OFF";
static char PStrIn[]  PROGMEM = "In";
static char PStrOut[] PROGMEM = "Ou";

///////////

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
        ptr = MidiMsg_writeRaw(ptr, &(Log[index]));

        // Pad with spaces until we are at col 8, or at least one
        do
        {
            (*ptr++) = ' ';
        } while (ptr < (char*)&(WindowBuffer[row][8]));

        // Write parsed message
        ptr = MidiMsg_writeParsed(ptr, &(Log[index]));

        // Write spaces for the rest of this row
        while (ptr < (char*)&(WindowBuffer[row][BUFFER_MAX]))
        {
            (*ptr++) = ' ';
        }
    }
}

static void resetLog(void)
{
    LogEnabled = FALSE;

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
        Lcd_setCursor(row, 0);

        // Blit data from window buffer to lcd
        for (col = 0; col < LCD_COLUMNS; col++)
        {
            // Inside buffer?
            i = col + ColOffset;
            if ((i >= 0) && (i < BUFFER_MAX))
            {
                Lcd_write(WindowBuffer[row][i]);
            }
            else
            {
                Lcd_write(' ');
            }
        }
    }

    Lcd_setCursor(0,LCD_COLUMNS - 2);

    if (LogMode == LOG_MODE_ACTIVE)
    {
        // We are enabled, write arrow
        Lcd_write(LCD_CHAR_RIGHTARROW);
    }
    else
    {
        // First pause mode, write || symbol
        Lcd_write('"');
    }

    if (LogInNotOut)
    {
        Lcd_write('I');
    }
    else
    {
        Lcd_write('O');
    }

    // Set cursor depending on mode
    if (LogMode == LOG_MODE_PAUSED2)
    {
        Lcd_setCursor(0, LCD_COLUMNS-1);
    }
    else
    {
        Lcd_setCursor(LCD_ROWS-1, LCD_COLUMNS-1);
    }
}

static void handleUserEvent(uint8_t user_event, int8_t knob_delta)
{
    if (user_event == MENU_EVENT_BACK)
    {
        // Exit log mode
        LogEnabled = FALSE;
        LogMode = LOG_MODE_OFF;
    }
    else if (user_event == MENU_EVENT_TURN)
    {
        if ((LogMode == LOG_MODE_ACTIVE) || (LogMode == LOG_MODE_PAUSED1))
        {
            // Move row
            RowOffset = util_boundedAddInt8(RowOffset, 0, LOG_SIZE - LCD_ROWS, -knob_delta);
        }
        else if (LogMode == LOG_MODE_PAUSED2)
        {
            // Toggle input source
            if (knob_delta < 0)
            {
                LogInNotOut = TRUE;
            }
            else if (knob_delta > 0)
            {
                LogInNotOut = FALSE;
            }
        }


    }
    else if (user_event == MENU_EVENT_TURN_PUSH)
    {
        if ((LogMode == LOG_MODE_ACTIVE) || (LogMode == LOG_MODE_PAUSED1))
        {
            // Move col
            ColOffset = util_boundedAddInt8(ColOffset, -10, 10, knob_delta);
        }
    }
    else if (user_event == MENU_EVENT_SELECT)
    {
        // Toggle between LOG_MODE_ACTIVE, LOG_MODE_PAUSED1, LOG_MODE_PAUSED2
        // Also control if ISR should actively log stuff
        LogMode++;

        if (LogMode == (LOG_MODE_PAUSED2+1))
        {
            LogMode = LOG_MODE_ACTIVE;
            LogEnabled = TRUE;
        }
        else
        {
            LogEnabled = FALSE;
        }
    }

    // Always need refresh when doing this
    NeedRefresh = TRUE;
}


////// Function hooks


void MidiLog_initialize(void)
{
    resetPan();

    LogMode = LOG_MODE_OFF;
    LogEnabled = FALSE;
    LogIndexMain = 0u;
    LogIndexIsr = 1u;
    NeedRefresh = TRUE;
}


void MidiLog_handleMidiMsgIn_ISR(midiMsg_t *msg)
{
    // Grab incoming message if enabled

    if (LogEnabled && LogInNotOut)
    {

        if (LogIndexIsr < LOG_SIZE)
        {
            memcpy(&(Log[LogIndexIsr]), msg, sizeof(midiMsg_t));
        }
        else
        {
            err_Raise(ERR_MODULE_MLOG, __LINE__);
        }

        LogIndexIsr++;

        if (LogIndexIsr >= LOG_SIZE)
        {
            LogIndexIsr = 0;
        }
    }
}


void MidiLog_handleMidiMsgOut_ISR(midiMsg_t *msg)
{
    // Grab outgoing message if enabled

    if (LogEnabled && (LogInNotOut == FALSE))
    {
        if (LogIndexIsr < LOG_SIZE)
        {
            memcpy(&(Log[LogIndexIsr]), msg, sizeof(midiMsg_t));
        }
        else
        {
            err_Raise(ERR_MODULE_MLOG, __LINE__);
        }

        LogIndexIsr++;

        if (LogIndexIsr >= LOG_SIZE)
        {
            LogIndexIsr = 0;
        }
    }
}

void MidiLog_handleTick_ISR(void)
{

}

void MidiLog_handleMainLoop(void)
{
    if (LogMode != LOG_MODE_OFF)
    {
        if (LogMode == LOG_MODE_ACTIVE)
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

/// Menu implementation

uint8_t MidiLog_menuGetSubCount(void)
{
    return 0u;
}

void MidiLog_menuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        util_strCpy_P(dest, PStrTitle);
    }
}

uint8_t MidiLog_menuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    if (item == 0)
    {
        if ((edit_mode == 0) && (user_event == MENU_EVENT_SELECT))
        {
            // We want to take over the display when showing midi log
            ret = MENU_RESERVE_DISPLAY;

            // Enter midi logger mode
            resetLog();
            LogInNotOut = TRUE;
            LogEnabled = TRUE;
            NeedRefresh = TRUE;
            LogMode = LOG_MODE_ACTIVE;
        }
        else if (edit_mode == 1)
        {
            handleUserEvent(user_event, knob_delta);
        }
    }

    return ret;
}
