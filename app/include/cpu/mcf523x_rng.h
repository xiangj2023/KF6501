/*
 * File:    mcf523x_rng.h
 * Purpose: Register and bit definitions for the MCF523X
 *
 * Notes:   
 *  
 */

#ifndef __MCF523X_RNG_H__
#define __MCF523X_RNG_H__

/*********************************************************************
*
* Random Number Generator (RNG)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_RNG_RNGCR     (*(DWORD*)(void*)(&__IPSBAR[0x1A0000]))
#define MCF_RNG_RNGSR     (*(DWORD*)(void*)(&__IPSBAR[0x1A0004]))
#define MCF_RNG_RNGER     (*(DWORD*)(void*)(&__IPSBAR[0x1A0008]))
#define MCF_RNG_RNGOUT    (*(DWORD*)(void*)(&__IPSBAR[0x1A000C]))

/* Bit definitions and macros for MCF_RNG_RNGCR */
#define MCF_RNG_RNGCR_GO        (0x00000001)
#define MCF_RNG_RNGCR_HA        (0x00000002)
#define MCF_RNG_RNGCR_IM        (0x00000004)
#define MCF_RNG_RNGCR_CI        (0x00000008)

/* Bit definitions and macros for MCF_RNG_RNGSR */
#define MCF_RNG_RNGSR_SV        (0x00000001)
#define MCF_RNG_RNGSR_LRS       (0x00000002)
#define MCF_RNG_RNGSR_FUF       (0x00000004)
#define MCF_RNG_RNGSR_EI        (0x00000008)
#define MCF_RNG_RNGSR_OFL(x)    (((x)&0x000000FF)<<8)
#define MCF_RNG_RNGSR_OFS(x)    (((x)&0x000000FF)<<16)

/********************************************************************/

#endif /* __MCF523X_RNG_H__ */
