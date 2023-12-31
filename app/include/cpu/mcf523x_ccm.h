/*
 * File:    mcf523x_ccm.h
 * Purpose: Register and bit definitions for the MCF523X
 *
 * Notes:   
 *  
 */

#ifndef __MCF523X_CCM_H__
#define __MCF523X_CCM_H__

/*********************************************************************
*
* Chip Configuration Module (CCM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_CCM_CCR     (*(WORD*)(void*)(&__IPSBAR[0x110004]))
#define MCF_CCM_LPCR    (*(BYTE *)(void*)(&__IPSBAR[0x110007]))
#define MCF_CCM_CIR     (*(WORD*)(void*)(&__IPSBAR[0x11000A]))
#define MCF_CCM_RCON    (*(WORD*)(void*)(&__IPSBAR[0x110008]))

/* Bit definitions and macros for MCF_CCM_CCR */
#define MCF_CCM_CCR_BMT(x)        (((x)&0x0007)<<0)
#define MCF_CCM_CCR_BME           (0x0008)
#define MCF_CCM_CCR_SZEN          (0x0040)
#define MCF_CCM_CCR_MODE(x)       (((x)&0x0007)<<8)
//#define MCF_CCM_CCR_MODE(x)       ((WORD)(((x)&0x0007)<<8))
//#define MCF_CCM_CCR_BMT(x)        ((WORD)(((x)&0x0007)))

/* Bit definitions and macros for MCF_CCM_LPCR */
#define MCF_CCM_LPCR_STPMD(x)     (((x)&0x03)<<3)
#define MCF_CCM_LPCR_LPMD(x)      (((x)&0x03)<<6)
#define MCF_CCM_LPCR_LPMD_STOP    (0xC0)
#define MCF_CCM_LPCR_LPMD_WAIT    (0x80)
#define MCF_CCM_LPCR_LPMD_DOZE    (0x40)
#define MCF_CCM_LPCR_LPMD_RUN     (0x00)
//#define MCF_CCM_LPCR_STPMD(x)     ((BYTE)(((x)&0x0003)<<3))

/* Bit definitions and macros for MCF_CCM_CIR */
#define MCF_CCM_CIR_PRN(x)        (((x)&0x003F)<<0)
#define MCF_CCM_CIR_PIN(x)        (((x)&0x03FF)<<6)

/* Bit definitions and macros for MCF_CCM_RCON */
#define MCF_CCM_RCON_MODE         (0x0001)
#define MCF_CCM_RCON_BOOTPS(x)    (((x)&0x0003)<<3)
#define MCF_CCM_RCON_RLOAD        (0x0020)
#define MCF_CCM_RCON_RCSC(x)      (((x)&0x0003)<<8)

/********************************************************************/

#endif /* __MCF523X_CCM_H__ */
