/**************************************************************************
 * FILE NAME: $RCSfile: etpu_ic.h,v $        COPYRIGHT (c) FREESCALE 2004 *
 * DESCRIPTION:                                     All Rights Reserved   *
 * This file contains the prototypes and defines for the eTPU Input       *
 * Capture (IC) API.                                                      *
 *========================================================================*
 * ORIGINAL AUTHOR: Jeff Loeliger (r12110)                                *
 * $Log: etpu_ic.h,v $
 * Revision 1.1  2004/12/15 14:56:09  r12110
 * -Initial version checked into CVS.
 *
 *........................................................................*
 * 0.1   J. Loeliger  01/Jun/04  Initial version.                         *
 **************************************************************************/
#ifndef _ETPU_IC_H_
#define _ETPU_IC_H_

#include "etpu_ic_auto.h"	/* Auto generated header file for eTPU IC */

#ifdef __cplusplus
	extern "C" {
#endif

/**************************************************************************/
/*                            Definitions                                 */
/**************************************************************************/

/**************************************************************************/
/*                       Function Prototypes                              */
/**************************************************************************/

/* IC channel initialization */
int32 fs_etpu_ic_init(uint8 channel, uint8 priority, uint8 mode,
         uint8 timebase, uint8 edge, uint32 max_count);

int32 fs_etpu_ic_init_ram(uint8 channel, uint8 priority, uint8 mode,
         uint8 edge, uint32 max_count, uint32 *address);

int32 fs_etpu_ic_init_link(uint8 channel, uint8 priority, uint8 mode,
         uint8 timebase, uint8 edge, uint32 max_count, uint32 link1,
         uint32 link2);

int32 fs_etpu_ic_init_link_ram(uint8 channel, uint8 priority, uint8 mode,
         uint8 edge, uint32 max_count, uint32 link1, uint32 link2,
         uint32 *address);


/* Data read functions */
int32 fs_etpu_ic_read_trans_count(uint8 channel);
int32 fs_etpu_ic_read_final_time(uint8 channel);
int32 fs_etpu_ic_read_last_time(uint8 channel);

/* TPU3 IC API compatible functions to be added */

#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************
 *
 * Copyright:
 *	Freescale Semiconductor, INC. All Rights Reserved.
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of Freescale
 *  Semiconductor, Inc. This software is provided on an "AS IS"
 *  basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, Freescale
 *  Semiconductor DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED,
 *  INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A
 *  PARTICULAR PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH
 *  REGARD TO THE SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF)
 *  AND ANY ACCOMPANYING WRITTEN MATERIALS.
 *
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL Freescale Semiconductor BE LIABLE FOR ANY DAMAGES WHATSOEVER
 *  (INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 *  BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER
 *  PECUNIARY LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.
 *
 *  Freescale Semiconductor assumes no responsibility for the
 *  maintenance and support of this software
 *
 ********************************************************************/





