#ifdef DEBUG
/**************************************************************************/
/* FILE NAME: typedefs.h       COPYRIGHT (c) Freescale Semiconductor 2004 */ 
/* VERSION:   0.5 - BETA VERSION                  All Rights Reserved     */
/*                                                                        */
/* DESCRIPTION:                                                           */
/* This file defines all of the data types for the Motorola header file.  */
/*========================================================================*/
/* AUTHOR: Jeff Loeliger                                                  */
/*                                                                        */
/* UPDATE HISTORY                                                         */
/* REV      AUTHOR      DATE       DESCRIPTION OF CHANGE                  */
/* ---   -----------  ---------    ---------------------                  */
/* 0.1   J. Loeliger  17/Feb/03    Initial version of file.               */
/* 0.2   J. Loeliger  06/Mar/03    Added DCC support.                     */
/* 0.3   J. Loeliger  07/May/03    Change to fully use ISO data types.    */
/* 0.4   J. Loeliger  17/Jun/03    Change name to motint.h and merge      */
/*                                  MPC5500 and MAC7100 files.            */
/* 0.5   J. Loeliger  04/Nov/03    Changed name to typedefs.h.            */
/**************************************************************************/
#endif

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

/*
#ifdef __MWERKS__    //Metrowerk CodeWarrior
    #include <stdint.h>

    // Standard typedefs used by header files, based on ISO C standard
    typedef volatile int8_t vint8_t;
    typedef volatile uint8_t vuint8_t;

    typedef volatile int16_t vint16_t;
    typedef volatile uint16_t vuint16_t;

    typedef volatile int32_t vint32_t;
    typedef volatile uint32_t vuint32_t;
    
    // MPC500 compatibility types
    typedef uint8_t  UINT8;
    typedef uint16_t UINT16;
    typedef int8_t  INT8;
    typedef int16_t INT16;
#endif __MWERKS__
*/


//#ifdef __DCC__   //Diab
    // This is needed because Diab does not currently have a stdint.h file

    typedef signed char int8;
    typedef unsigned char uint8;
    typedef volatile signed char vint8;
    typedef volatile unsigned char vuint8;

    typedef signed short int16;
    typedef unsigned short uint16;
    typedef volatile signed short vint16;
    typedef volatile unsigned short vuint16;

    typedef signed int int32;
    typedef unsigned int uint32;
    typedef volatile signed int vint32;
    typedef volatile unsigned int vuint32;
//#endif __DCC__


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
 ********************************************************************/
