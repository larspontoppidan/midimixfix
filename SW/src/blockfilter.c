/*
 * blockfilter.c
 *
 *  Created on: Dec 22, 2012
 *      Author: lars
 */


// Block filter

#include "common.h"
#include "blockfilter.h"
#include "midimessage.h"
#include "util.h"
#include "menu.h"
#include "pgmstrings.h"
#include <string.h>

// The block filter can do blocks of:

// Channels:            In1 Ch1 to In2 Ch16


typedef struct
{
    // The block filter is configured by these:
    int8_t source;
    int8_t chan;

//    int8_t type;
//
//    // To make the ISR implementation efficient, the filter is realized by
//    // these four variables
//    uint8_t status_mask;
//    uint8_t status_compare;
//    uint8_t byte1_mask;
//    uint8_t byte1_compare;
} blockFilter_t;

// Variables

static bool_t filterEnabled;

static char titleString[] PROGMEM = "Block filter ";

static uint8_t menuManageMode;

#define FILTER_COUNT_MAX 8

static uint8_t filterCount;
static blockFilter_t filters[FILTER_COUNT_MAX];



void blockf_Initialize(void)
{
    filterCount = 0;
    filterEnabled = FALSE;
}

void blockf_HookMidiMsg_ISR(mmsg_t *msg)
{
    /*
    if (filterEnabled != FALSE)
    {
        uint8_t i;
        uint8_t x;

        for (i = 0; i < filter_count; i++)
        {
            if ((msg->source & source) != 0)
            {
                if ((msg->midi_status & bfBlock[i].status_mask)
                        == bfBlock[i].status_compare)
                {
                    if ((msg->midi_data[0] & bfBlock[i].byte1_mask)
                            == bfBlock[i].byte1_compare)
                    {
                        msg->flags |= MMSG_FLAG_DISCARD;
                        break;
                    }
                }
            }
        }
    }
    */

    // Right now, lets just implement a simplified version, only source / channel blocks:

    if (filterEnabled != FALSE)
    {
        uint8_t i;
        uint8_t x;

        for (i = 0; i < filterCount; i++)
        {
            if ((msg->flags & filters[i].source) != 0)
            {
                x = msg->midi_status;

                if ((x & 0xF0) != 0xF0u)
                {
                    // Its a channel something message, check if we allow this channel
                    if ((x & 0x0F) == filters[i].chan)
                    {
                        // This message has to stop!
                        msg->flags |= MMSG_FLAG_DISCARD;
                        break;
                    }
                }
            }
        }
    }

}

void blockf_HookTick_ISR(void)
{

}

void blockf_HookMainLoop(void)
{

}

uint8_t blockf_MenuGetSubCount(void)
{
    return filterEnabled ? (filterCount + 1) : 0;
}

char *blockf_RenderBlock(char *dest, uint8_t i)
{
    dest = pstr_writeInX(dest, filters[i].source);
    (*dest++) = ' ';

    dest = util_strCpy_P(dest, pstr_Chan);
    dest = util_strWriteInt8LA(dest, filters[i].chan + 1);

    return dest;
}

void blockf_AddBlock(void)
{
    if (filterCount < FILTER_COUNT_MAX)
    {
        filters[filterCount].source = 1;
        filters[filterCount].chan = 0;
        filterCount++;
    }
}

void blockf_RemoveBlock(void)
{
    if (filterCount > 0)
    {
        filterCount--;
    }
}

void blockf_MenuGetText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        util_strCpy_P(dest, titleString);

        if (filterEnabled)
        {
            util_strCpy_P(dest + 14, pstr_OnParentheses);
        }
        else
        {
            util_strCpy_P(dest + 14, pstr_OffParentheses);
        }
    }
    else if (item == (filterCount + 1))
    {
        dest = util_strCpy_P(dest, pstr_ManageEllipsis);

        if (menuManageMode == 1)
        {
            util_strCpy_P(dest, pstr_Add);
        }
        else if (menuManageMode == 2)
        {
            util_strCpy_P(dest, pstr_Remove);
        }
    }
    else
    {
        blockf_RenderBlock(dest, item - 1);
    }
}

uint8_t blockf_MenuHandleEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
{
    uint8_t ret = MENU_EDIT_MODE_UNAVAIL;

    if (item == 0)
    {
        if (edit_mode == 0)
        {
            if (user_event == MENU_EVENT_SELECT)
            {
                // We are at root level and user wants to edit title menu

                // Ok, move cursor to ON / OFF
                ret = 16;
            }
        }
        else if (edit_mode == 1)
        {
            if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
            {
                // Toggle on / off status
                filterEnabled = util_boundedAddInt8(filterEnabled, 0, 1, knob_delta);

                // Keep cursor at position, and notify menu that this may alter our submenu
                ret = 16 | MENU_UPDATE_ALL;
            }
        }
    }
    else if (item == (filterCount + 1))
    {
        if (edit_mode == 0)
        {
            if (user_event == MENU_EVENT_SELECT)
            {
                // User starts to enter manage mode
                ret = 10;

                // Our mode is initially Add
                menuManageMode = 1;
            }
        }
        else if (edit_mode == 1)
        {
            ret = 10;

            if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
            {
                // User is turning knob in manage mode
                menuManageMode = util_boundedAddInt8(menuManageMode, 1, 2, knob_delta);
            }
            else if (user_event == MENU_EVENT_SELECT)
            {
                // User is selecting current manage mode
                if (menuManageMode == 1)
                {
                    blockf_AddBlock();
                    menuManageMode = 0;
                    ret = MENU_EDIT_MODE_UNAVAIL | MENU_UPDATE_ALL;
                }
                else if (menuManageMode == 2)
                {
                    blockf_RemoveBlock();
                    menuManageMode = 0;
                    ret = MENU_EDIT_MODE_UNAVAIL | MENU_UPDATE_ALL;
                }
            }
            else if (user_event == MENU_EVENT_BACK)
            {
                // Backing out of manage mode
                menuManageMode = 0;
            }
        }
    }
    else
    {
        // We are highlighting one block filter
        if (user_event == MENU_EVENT_SELECT)
        {
            if (edit_mode == 0)
            {
                // User enters first edit mode
                ret = 0;
            }
            else if (edit_mode == 1)
            {
                // User enters second edit mode
                ret = 4;
            }
        }
        else if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
        {
            if (edit_mode == 1)
            {
                ret = 0;
                filters[item-1].source =
                        util_boundedAddInt8(filters[item-1].source, 1, 3, knob_delta);
            }
            else if (edit_mode == 2)
            {
                ret = 4;
                filters[item-1].chan =
                    util_boundedAddInt8(filters[item-1].chan, 0, 15, knob_delta);
            }
        }
    }

    return ret;
}

// Configuration store and load implementation

uint8_t blockf_ConfigGetSize(void)
{
    return 2 + FILTER_COUNT_MAX * sizeof(blockFilter_t);
}

void blockf_ConfigSave(uint8_t *dest)
{
    *(dest++) = filterEnabled;
    *(dest++) = filterCount;

    memcpy(dest, &(filters[0]), FILTER_COUNT_MAX * sizeof(blockFilter_t));
}

void blockf_ConfigLoad(uint8_t *dest)
{
    filterEnabled = *(dest++);
    filterCount = *(dest++);

    memcpy(&(filters[0]), dest, FILTER_COUNT_MAX * sizeof(blockFilter_t));
}
