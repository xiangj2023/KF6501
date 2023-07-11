/**************************************************************************
 * FILE NAME: $RCSfile: etpu_ppa.h,v $       COPYRIGHT (c) FREESCALE 2004 *
 * DESCRIPTION:                                     All Rights Reserved   *
 * This file contains the prototypes and defines for ETPU Pulse or Period *
 * Accumulator function (PPA) API                                         *
 *========================================================================*
 * ORIGINAL AUTHOR: Geoff Emerson (r47354)                                *
 * $Log: etpu_ppa.h,v $
 * Revision 1.1  2004/12/09 14:38:31  r47354
 * Updates as per QOM API rel_2_1
 *
 *........................................................................*
 * 0.1   G. Emerson  8/Sep/04  Initial version.                           *
 **************************************************************************/
#ifndef _ETPU_PPA_H_
#define _ETPU_PPA_H_

#ifdef __cplusplus
	 extern "C" {
#endif

/**************************************************************************/
#include "etpu_ppa_auto.h"		/*Auto generated header file for eTPU PPA */

/**************************************************************************/
/*                       Function Prototypes                              */
/**************************************************************************/

/* PPA channel initialization */
int32 fs_etpu_ppa_init( uint8 channel, uint8 priority, 
                          uint8 timebase,uint8 mode, uint32 max_count, 
                          uint32 sample_time );
                       
                       
uint8 fs_etpu_ppa_get_accumulation( uint8 channel, uint32 *result );

uint8 fs_etpu_ppa_get_immediate( uint8 channel, uint32 *result, 
                                   uint32 *current_count );

#ifdef __cplusplus
}
#endif

#endif                      
/* TPU3 PPA API compatible functions to be added */
