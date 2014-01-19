///*
// * ratelimit.c
// *
// *  Created on: Jan 19, 2014
// *      Author: lars
// */
//

#include "ratelimit.h"
//
//static void filterReset(uint8_t chan, uint8_t value);
//static void filterSetTarget(uint8_t chan, uint8_t value);
//static void filterUpdate(uint8_t chan);
//
//
//static void filterReset(uint8_t chan, uint8_t value)
//{
//    ChannelState[chan].FilterTarget = value;
//    ChannelState[chan].FilterState = value;
//}
//
//static void filterSetTarget(uint8_t chan, uint8_t value)
//{
//    ChannelState[chan].FilterTarget = value;
//}
//
//static void filterUpdate(uint8_t chan)
//{
//    if (ChannelState[chan].FilterState != ChannelState[chan].FilterTarget)
//    {
//        // Work in a wider integer space to avoid wrap-around problems
//        // in the following calculations
//
//        int16_t s = ChannelState[chan].FilterState;
//        int16_t t = ChannelState[chan].FilterTarget;
//
//        if (t > s)
//        {
//            // We have to increase value
//            s += (int16_t)(Config.AtRateLimit);
//
//            if (s > t)
//            {
//                // We got too far, lets arrive at target then:
//                s = t;
//            }
//        }
//        else
//        {
//            // We have to decrease value
//            s -= (int16_t)(Config.AtRateLimit);
//
//            if (s < t)
//            {
//                // We got too far, lets arrive at target then:
//                s = t;
//            }
//        }
//
//        // Update state
//        ChannelState[chan].FilterState = (uint8_t)s;
//    }
//}
//
//
//void psplit_handleTick_ISR(void)
//{
//    if (TickDivider == 0)
//    {
//        // Check if the rate limit filter is on
//
//        if (Config.AtRateLimit != 0)
//        {
//            uint8_t i;
//
//            // It is, update all the filters and send if there is stuff to do
//            for (i = 0; i < Config.OutChannels; i++)
//            {
//                filterUpdate(i);
//
//                if (ChannelState[i].FilterState != ChannelState[i].LatestCc)
//                {
//                    sendAtMsg(i, ChannelState[i].FilterState);
//                }
//            }
//        }
//
//        TickDivider = TICK_DIVISION - 1;
//    }
//    else
//    {
//        TickDivider--;
//    }
//
//}
//

