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
} bfBlock_t;

// Variables

bool_t bfEnabled;
char   bfTitle[]  PROGMEM = "Block filter ";

char   bfManage[] PROGMEM = "Manage... ";
char   bfAdd[]    PROGMEM = "Add";
char   bfRemove[] PROGMEM = "Remove";
char   bfIn[] PROGMEM = "In";
char   bfChan[] PROGMEM = " Chan";

char bfSources[3] = "12*";

uint8_t bfManageMode;

#define BF_BLOCK_MAX 8

uint8_t bfBlocks;
bfBlock_t bfBlock[8];

void blockf_Initialize(void)
{
    bfBlocks = 0;
    bfEnabled = FALSE;
}

void blockf_MessageIsrHook(mmsg_t *msg)
{
    /*
    if (bfEnabled != FALSE)
    {
        uint8_t i;
        uint8_t x;

        for (i = 0; i < bfBlocks; i++)
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

    if (bfEnabled != FALSE)
    {
        uint8_t i;
        uint8_t x;

        for (i = 0; i < bfBlocks; i++)
        {
            if ((msg->flags & bfBlock[i].source) != 0)
            {
                x = msg->midi_status;

                if ((x & 0xF0) != 0xF0u)
                {
                    // Its a channel something message, check if we allow this channel
                    if ((x & 0x0F) == bfBlock[i].chan)
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

void blockf_TickIsrHook(void)
{

}

void blockf_MainLoopHook(void)
{

}

uint8_t blockf_GetSubMenuCount(void)
{
    return bfEnabled ? (bfBlocks + 1) : 0;
}

char *blockf_RenderBlock(char *dest, uint8_t i)
{
    dest = util_StrCpy_P(dest, bfIn);
    (*dest++) = bfSources[(bfBlock[i].source)-1];
    dest = util_StrCpy_P(dest, bfChan);
    dest = util_StrWriteInt8LA(dest, bfBlock[i].chan + 1);

    return dest;
}

void blockf_AddBlock(void)
{
    if (bfBlocks < BF_BLOCK_MAX)
    {
        bfBlock[bfBlocks].source = 3;
        bfBlock[bfBlocks].chan = 0;
        bfBlocks++;
    }
}

void blockf_RemoveBlock(void)
{
    if (bfBlocks > 0)
    {
        bfBlocks--;
    }
}

void blockf_GetMenuText(char *dest, uint8_t item)
{
    if (item == 0)
    {
        dest = util_StrCpy_P(dest, bfTitle);

        if (bfEnabled)
        {
            util_StrCpy_P(dest, pstr_On);
        }
        else
        {
            util_StrCpy_P(dest, pstr_Off);
        }
    }
    else if (item == (bfBlocks + 1))
    {
        dest = util_StrCpy_P(dest, bfManage);

        if (bfManageMode == 1)
        {
            util_StrCpy_P(dest, bfAdd);
        }
        else if (bfManageMode == 2)
        {
            util_StrCpy_P(dest, bfRemove);
        }
    }
    else
    {
        blockf_RenderBlock(dest, item - 1);
    }
}

uint8_t blockf_MenuEvent(uint8_t item, uint8_t edit_mode, uint8_t user_event, int8_t knob_delta)
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
                ret = 13;
            }
        }
        else if (edit_mode == 1)
        {
            if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
            {
                // Toggle on / off status
                bfEnabled = util_BoundedAddInt8(bfEnabled, 0, 1, knob_delta);

                // Keep cursor at position, and notify menu that this may alter our submenu
                ret = 13 | MENU_UPDATE_ALL;
            }
        }
    }
    else if (item == (bfBlocks + 1))
    {
        if (edit_mode == 0)
        {
            if (user_event == MENU_EVENT_SELECT)
            {
                // User starts to enter manage mode
                ret = 10;

                // Our mode is initially Add
                bfManageMode = 1;
            }
        }
        else if (edit_mode == 1)
        {
            ret = 10;

            if ((user_event == MENU_EVENT_TURN) || (user_event == MENU_EVENT_TURN_PUSH))
            {
                // User is turning knob in manage mode
                bfManageMode = util_BoundedAddInt8(bfManageMode, 1, 2, knob_delta);
            }
            else if (user_event == MENU_EVENT_SELECT)
            {
                // User is selecting current manage mode
                if (bfManageMode == 1)
                {
                    blockf_AddBlock();
                    bfManageMode = 0;
                    ret = 0 | MENU_UPDATE_ALL;
                }
                else if (bfManageMode == 2)
                {
                    blockf_RemoveBlock();
                    bfManageMode = 0;
                    ret = 0 | MENU_UPDATE_ALL;
                }
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
                ret = 2;
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
                ret = 2;
                bfBlock[item-1].source =
                        util_BoundedAddInt8(bfBlock[item-1].source, 1, 3, knob_delta);
            }
            else if (edit_mode == 2)
            {
                ret = 4;
                bfBlock[item-1].chan =
                    util_BoundedAddInt8(bfBlock[item-1].chan, 0, 15, knob_delta);
            }
        }
    }

    return ret;
}
