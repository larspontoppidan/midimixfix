/*
 * filteruids.h
 *
 *  Created on: Nov 3, 2013
 *      Author: lars
 */

#ifndef FILTERUIDS_H_
#define FILTERUIDS_H_

// A filter UID is a 32-bit number for uniquely identify filters that are load/save compatible
//
// The UID consists of its ID (byte 1, 2, 3) and a version number (byte 4)
//
// The ID's are partitioned into ranges allocated for plugins authors:
//
// Range 0      to 10000 : Lars Ole Pontoppidan
// Range 100001 to       : ?

#define FILTER_ID_MIDIIN_IN1           2
#define FILTER_ID_MIDIIN_IN2           3
#define FILTER_ID_MIDIIN_IN1REALTIME   4
#define FILTER_ID_MIDIIN_IN2REALTIME   5
#define FILTER_ID_MIDIIN_IN1SYSEX      6
#define FILTER_ID_MIDIIN_IN2SYSEX      7
#define FILTER_ID_PEDALS_IN1           10
#define FILTER_ID_PEDALS_IN2           11
//#define FILTER_ID_ROUTECHAN_FILTER     20
#define FILTER_ID_SETCHAN_FILTER       21
#define FILTER_ID_MODIFYCC_FILTER      22
#define FILTER_ID_TRANSFILTER_FILTER   23
#define FILTER_ID_CURVE_FILTER         24
#define FILTER_ID_GENMSG_FILTER        25
#define FILTER_ID_POLYSPLIT_FILTER     26
#define FILTER_ID_ROUTEMSG_FILTER      27
#define FILTER_ID_DUPMSG_FILTER        28
#define FILTER_ID_ATSMOOTHER_FILTER    29
#define FILTER_ID_RESETTER_FILTER      30
#define FILTER_ID_CCNOTEON_FILTER      31
#define FILTER_ID_MIDIOUT_FILTER       1

#define FILTER_UID(id, version)  (((uint32_t)id) << 8 | (version))

#endif /* FILTERUIDS_H_ */
