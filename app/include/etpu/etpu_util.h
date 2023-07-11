/**************************************************************************
 * FILE NAME: $RCSfile: etpu_util.h,v $      COPYRIGHT (c) FREESCALE 2004 *
 * DESCRIPTION:                                     All Rights Reserved   *
 * This file contains useful macros and prototypes for using the eTPU.    *
 *========================================================================*
 * ORIGINAL AUTHOR: Jeff Loeliger (r12110)                                *
 * $Log: etpu_util.h,v $
 * Revision 1.1  2004/12/15 10:21:44  r54529
 * *** empty log message ***
 *
 * Revision 2.0  2004/11/30 16:11:34  r12110
 * -Updated functions to remove requirement for etpu_config.h file.
 *
 * Revision 1.1  2004/11/12 10:39:10  r12110
 * Initial version checked into CVS.
 *........................................................................*
 * 0.01  J. Loeliger  13/Jun/03    Initial version of file.               *
 * 0.02  M. Brejl     12/Nov/03    etpu routines elaborated               *
 * 0.03  J. Loeliger  17/Nov/03    Merged several versions of file        *
 *                                  together.                             *
 * 0.04  J. Loeliger  19/Mar/04    Change function prefix to fs_ for      *
 *                                  Freescale semiconductor.              *
 * 0.05  J. Loeliger  26/May/04    Added 24 bit data types.               *
 *       M. Brejl                  Added FS_ to macros                    *
 *                                 Changed filename to fs_etpu_util       *
 *                                 Added pin high/low macros.             *
 * 0.06 J. Loeliger  20/Jul/04    Change include to just typedefs.h       *
 * 0.07 J. Loeliger  16/Aug/04    Removed typedef from etpu_config_t      *
 * 0.1  J. Loeliger  01/Sep/04    Added fs_etpu_malloc2 function.         *
 **************************************************************************/
#ifndef _ETPU_UTIL_H_
#define _ETPU_UTIL_H_

#include "typedefs.h"     /* standard types */
#include "etpu_struct.h"  /* eTPU module structure definition */

extern uint32 *fs_free_param; /* pointer to the first free parameter */

extern struct eTPU_struct *eTPU;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*************************************************************************
* Type Definitions
*************************************************************************/
typedef uint32 ufract24;
typedef int32 fract24;

typedef uint32 uint24;
typedef int32 int24;

/*************************************************************/
/* NOTE: All channel numbers for both eTPUs are absolute.    */
/* eTPU_A has channels 0-31 and eTPU_B has channels 64-95.   */
/*************************************************************/

/* Configuration structure structure */
struct etpu_config_t{
    uint32 mcr;
    uint32 misc;
    uint32 ecr_a;
    uint32 tbcr_a;
    uint32 stacr_a;
    uint32 ecr_b;
    uint32 tbcr_b;
    uint32 stacr_b;
};

/* function prototypes */
uint32 fs_etpu_init(struct etpu_config_t p_etpu_config, uint32 *code,
                  uint32 code_size, uint32 *globals, uint32 globals_size);

uint32 *fs_etpu_malloc(uint16 num_bytes);
uint32 *fs_etpu_malloc2(uint8 channel, uint16 num_bytes);

void fs_timer_start(void);

uint32 *fs_etpu_data_ram(uint8 channel);

void    fs_etpu_set_hsr(uint8 channel, uint8 hsr);
uint8 fs_etpu_get_hsr(uint8 channel);

void fs_etpu_enable(uint8 channel, uint8 priority);
void fs_etpu_disable(uint8 channel);

void fs_etpu_interrupt_enable(uint8 channel);
void fs_etpu_interrupt_disable(uint8 channel);
void fs_etpu_dma_enable(uint8 channel);
void fs_etpu_dma_disable(uint8 channel);

void fs_etpu_set_chan_local_32(uint8 channel, uint32 offset, uint32 value);
void fs_etpu_set_chan_local_24(uint8 channel, uint32 offset, uint24 value);
void fs_etpu_set_chan_local_16(uint8 channel, uint32 offset, uint16 value);
void fs_etpu_set_chan_local_8(uint8 channel, uint32 offset, uint8 value);

uint32 fs_etpu_get_chan_local_32(uint8 channel, uint32 offset);
 int24 fs_etpu_get_chan_local_24s(uint8 channel, uint32 offset);
uint24 fs_etpu_get_chan_local_24(uint8 channel, uint32 offset);
uint16 fs_etpu_get_chan_local_16(uint8 channel, uint32 offset);
uint8  fs_etpu_get_chan_local_8(uint8 channel, uint32 offset);

void fs_etpu_set_global_32(uint32 offset, uint32 value);
void fs_etpu_set_global_24(uint32 offset, uint24 value);
void fs_etpu_set_global_16(uint32 offset, uint16 value);
void fs_etpu_set_global_8(uint32 offset, uint8 value);

uint32 fs_etpu_get_global32(uint32 offset);
 int24 fs_etpu_get_global_24s(uint32 offset);
uint24 fs_etpu_get_global_24(uint32 offset);
uint16 fs_etpu_get_global_16(uint32 offset);
uint8  fs_etpu_get_global_8(uint32 offset);

uint32 *fs_memcpy32(uint32 *dest, uint32 *source, uint32 size);
void      fs_memset32(uint32 *start, uint32 value, int32 size);

/*************************************************************/
/*                   Definition of terms                     */
/*************************************************************/
/*********************************************************/
/* Channel Priority values used by several function APIs */
/*********************************************************/
#define FS_ETPU_PRIORITY_HIGH         0x3
#define FS_ETPU_PRIORITY_MIDDLE       0x2
#define FS_ETPU_PRIORITY_LOW          0x1
#define FS_ETPU_PRIORITY_DISABLE      0x0

#define FS_ETPU_PIN_HIGH 1
#define FS_ETPU_PIN_LOW  0

/* FS_ETPU timebases */
#define FS_ETPU_TCR1   0
#define FS_ETPU_TCR2   1
#define FS_ETPU_TCR1_A 0
#define FS_ETPU_TCR2_A 1
#define FS_ETPU_TCR1_B 0
#define FS_ETPU_TCR2_B 1

/* MCR - Module Configuration Register */
#define FS_ETPU_MISC_ENABLE            0x00000200 /* Code RAM MISC Enable */
#define FS_ETPU_MISC_DISABLE           0x00000000

#define FS_ETPU_VIS_ON                 0x00000040 /* Code RAM Visibility */
#define FS_ETPU_VIS_OFF                0x00000000

#define FS_ETPU_GLOBAL_TIMEBASE_ENABLE  0x00000001 /* Global Time Base Enable */
#define FS_ETPU_GLOBAL_TIMEBASE_DISABLE 0x00000000

/* ECR - Engine Configuration Register */
#define FS_ETPU_FILTER_CLOCK_DIV2      0x00000000 /* Filter Prescaler Clock */
#define FS_ETPU_FILTER_CLOCK_DIV4      0x00010000 /* Control                */
#define FS_ETPU_FILTER_CLOCK_DIV8      0x00020000
#define FS_ETPU_FILTER_CLOCK_DIV16     0x00030000
#define FS_ETPU_FILTER_CLOCK_DIV32     0x00040000
#define FS_ETPU_FILTER_CLOCK_DIV64     0x00050000
#define FS_ETPU_FILTER_CLOCK_DIV128    0x00060000
#define FS_ETPU_FILTER_CLOCK_DIV256    0x00070000

#define FS_ETPU_CHAN_FILTER_2SAMPLE    0x00000000 /* Channel filter mode */
#define FS_ETPU_CHAN_FILTER_3SAMPLE    0x00008000
#define FS_ETPU_CHAN_FILTER_CONT       0x0000C000

#define FS_ETPU_ENGINE_ENABLE          0x00000000  /* Low Power Stop Bit */
#define FS_ETPU_ENGINE_DISABLE         0x40000000

/* TBCR - Time Base Configuration Register */
#define FS_ETPU_TCRCLK_MODE_2SAMPLE     0x00000000 /* TCRCLK Signal Filter Control*/
#define FS_ETPU_TCRCLK_MODE_INTEGRATION 0x10000000
#define FS_ETPU_TCRCLK_INPUT_DIV2CLOCK  0x00000000
#define FS_ETPU_TCRCLK_INPUT_CHANCLOCK  0x08000000

#define FS_ETPU_TCR1CTL_TCRCLK         0x00000000  /* TCR1 Clock/Gate Control */
#define FS_ETPU_TCR1CTL_DIV2           0x00008000

#define FS_ETPU_ANGLE_MODE_ENABLE      0x02000000   /* Angle Mode */
#define FS_ETPU_ANGLE_MODE_DISABLE     0x00000000

#define FS_ETPU_TCR2CTL_GATEDDIV8      0x00000000   /* TCR2 Clock/Gate Control */
#define FS_ETPU_TCR2CTL_RISE           0x20000000
#define FS_ETPU_TCR2CTL_FALL           0x40000000
#define FS_ETPU_TCR2CTL_RISEFALL       0x60000000
#define FS_ETPU_TCR2CTL_DIV8           0x80000000

/* STACR - Shared Time And Angle Coount Register */
#define FS_ETPU_TCR1_STAC_ENABLE       0x80000000   /* TCR1 Resource Enable */
#define FS_ETPU_TCR1_STAC_DISABLE      0x00000000

#define FS_ETPU_TCR1_STAC_CLIENT       0x00000000   /* TCR1 Resource Control */
#define FS_ETPU_TCR1_STAC_SERVER       0x40000000

#define FS_ETPU_TCR2_STAC_ENABLE       0x00008000   /* TCR2 Resource Enable */
#define FS_ETPU_TCR2_STAC_DISABLE      0x00000000

#define FS_ETPU_TCR2_STAC_CLIENT       0x00000000   /* TCR2 Resource Control */
#define FS_ETPU_TCR2_STAC_SERVER       0x00004000

/* CxCR - Channel x Configuration Registr */
#define FS_ETPU_INTERRUPT_ENABLE      0x80000000  /* Channel Interruput Enable */
#define FS_ETPU_INTERRUPT_DISABLE     0x00000000

#define FS_ETPU_DMA_ENABLE            0x40000000  /* Data Transfer Request Enable*/
#define FS_ETPU_DMA_DISABLE           0x00000000

#define FS_ETPU_ENTRY_TABLE_STANDARD  0x00000000  /* Entry Table Condition Select*/
#define FS_ETPU_ENTRY_TABLE_ALTERNATE 0x01000000

#define FS_ETPU_OUTPUT_DISABLE_OFF    0x00000000  /* Output Disable */
#define FS_ETPU_OUTPUT_DISABLE_LOW    0x0000C000
#define FS_ETPU_OUTPUT_DISABLE_HIGH   0x00008000

/* eTPU error return codes */
#define FS_ETPU_ERROR_MALLOC   1
#define FS_ETPU_ERROR_FREQ     2
#define FS_ETPU_ERROR_VALUE    3
#define FS_ETPU_ERROR_CODESIZE 4

/* channel macro */
#define FS_ETPU_ENGINE_CHANNEL(x,y) (((x)-1)*128 + y) /* channel number by
                                                      (engine,channel) */

#endif /* _ETPU_UTIL_H_ */

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

