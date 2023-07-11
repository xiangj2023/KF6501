/**************************************************************************
 * FILE NAME: $RCSfile: etpu_pwm.h,v $       COPYRIGHT (c) FREESCALE 2004 *
 * DESCRIPTION:                                     All Rights Reserved   *
 * This file contains the prototypes and defines for the eTPU Pulse Width *
 * Modulation (PWM) API.                                                  *
 *========================================================================*
 * ORIGINAL AUTHOR: Jeff Loeliger (r12110)                                *
 * $Log: etpu_pwm.h,v $
 * Revision 2.0  2004/12/22 13:46:47  r12110
 * -First release into CVS.
 * -Added support for utils 2.0 functions.
 *
 **************************************************************************/
 #ifndef _ETPU_PWM_H_
 #define _ETPU_PWM_H_

 #ifdef __cplusplus
	 extern "C" {
#endif

#include "etpu_pwm_auto.h"     /* Auto generated header file for eTPU PWM */

/**************************************************************************/
/*                       Function Prototypes                              */
/**************************************************************************/

/* New eTPU functions */
/* PWM channel initialization */
int32 fs_etpu_pwm_init( uint8 channel, uint8 priority, uint32 freq, uint16 duty, \
          uint8 polarity, uint8 timebase, uint32 timebase_freq);

/* PWM duty cycle update */
void fs_etpu_pwm_duty( uint8 channel, uint16 duty);
void fs_etpu_pwm_duty_immed( uint8 channel, uint16 duty);

/* PWM duty cycle and freq update */
int32 fs_etpu_pwm_update( uint8 channel, uint32 freq, uint16 duty, \
       uint32 timebase_freq);

/* PWM information */
uint32 fs_etpu_pwm_get_freq( uint8 channel, uint32 timebase_freq);

/* TPU3 PWM API compatible functions to be added */
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





