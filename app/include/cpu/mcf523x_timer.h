/*
 * File:    mcf523x_timer.h
 * Purpose: Register and bit definitions for the MCF523X
 *
 * Notes:   
 *  
 */

#ifndef __MCF523X_TIMER_H__
#define __MCF523X_TIMER_H__

/*********************************************************************
*
* DMA Timers (TIMER)
*
*********************************************************************/

/* Register read/write macros */
/*
#define MCF_TIMER_DTMR0       (*(WORD*)(void*)(&__IPSBAR[0x000400]))
#define MCF_TIMER_DTXMR0      (*(BYTE *)(void*)(&__IPSBAR[0x000402]))
#define MCF_TIMER_DTER0       (*(BYTE *)(void*)(&__IPSBAR[0x000403]))
#define MCF_TIMER_DTRR0       (*(DWORD*)(void*)(&__IPSBAR[0x000404]))
#define MCF_TIMER_DTCR0       (*(DWORD*)(void*)(&__IPSBAR[0x000408]))
#define MCF_TIMER_DTCN0       (*(DWORD*)(void*)(&__IPSBAR[0x00040C]))
#define MCF_TIMER_DTMR1       (*(WORD*)(void*)(&__IPSBAR[0x000440]))
#define MCF_TIMER_DTXMR1      (*(BYTE *)(void*)(&__IPSBAR[0x000442]))
#define MCF_TIMER_DTER1       (*(BYTE *)(void*)(&__IPSBAR[0x000443]))
#define MCF_TIMER_DTRR1       (*(DWORD*)(void*)(&__IPSBAR[0x000444]))
#define MCF_TIMER_DTCR1       (*(DWORD*)(void*)(&__IPSBAR[0x000448]))
#define MCF_TIMER_DTCN1       (*(DWORD*)(void*)(&__IPSBAR[0x00044C]))
#define MCF_TIMER_DTMR2       (*(WORD*)(void*)(&__IPSBAR[0x000480]))
#define MCF_TIMER_DTXMR2      (*(BYTE *)(void*)(&__IPSBAR[0x000482]))
#define MCF_TIMER_DTER2       (*(BYTE *)(void*)(&__IPSBAR[0x000483]))
#define MCF_TIMER_DTRR2       (*(DWORD*)(void*)(&__IPSBAR[0x000484]))
#define MCF_TIMER_DTCR2       (*(DWORD*)(void*)(&__IPSBAR[0x000488]))
#define MCF_TIMER_DTCN2       (*(DWORD*)(void*)(&__IPSBAR[0x00048C]))
#define MCF_TIMER_DTMR3       (*(WORD*)(void*)(&__IPSBAR[0x0004C0]))
#define MCF_TIMER_DTXMR3      (*(BYTE *)(void*)(&__IPSBAR[0x0004C2]))
#define MCF_TIMER_DTER3       (*(BYTE *)(void*)(&__IPSBAR[0x0004C3]))
#define MCF_TIMER_DTRR3       (*(DWORD*)(void*)(&__IPSBAR[0x0004C4]))
#define MCF_TIMER_DTCR3       (*(DWORD*)(void*)(&__IPSBAR[0x0004C8]))
#define MCF_TIMER_DTCN3       (*(DWORD*)(void*)(&__IPSBAR[0x0004CC]))
#define MCF_TIMER_DTMR(x)     (*(WORD*)(void*)(&__IPSBAR[0x000400+((x)*0x040)]))
#define MCF_TIMER_DTXMR(x)    (*(BYTE *)(void*)(&__IPSBAR[0x000402+((x)*0x040)]))
#define MCF_TIMER_DTER(x)     (*(BYTE *)(void*)(&__IPSBAR[0x000403+((x)*0x040)]))
#define MCF_TIMER_DTRR(x)     (*(DWORD*)(void*)(&__IPSBAR[0x000404+((x)*0x040)]))
#define MCF_TIMER_DTCR(x)     (*(DWORD*)(void*)(&__IPSBAR[0x000408+((x)*0x040)]))
#define MCF_TIMER_DTCN(x)     (*(DWORD*)(void*)(&__IPSBAR[0x00040C+((x)*0x040)]))
*/
#define MCF_TIMER_DTMR(x)     (*(WORD*)(__IPSBAR+0x000400+((x)*0x040)))
#define MCF_TIMER_DTXMR(x)    (*(BYTE *)(__IPSBAR+0x000402+((x)*0x040)))
#define MCF_TIMER_DTER(x)     (*(BYTE *)(__IPSBAR+0x000403+((x)*0x040)))
#define MCF_TIMER_DTRR(x)     (*(DWORD*)(__IPSBAR+0x000404+((x)*0x040)))
#define MCF_TIMER_DTCR(x)     (*(DWORD*)(__IPSBAR+0x000408+((x)*0x040)))
#define MCF_TIMER_DTCN(x)     (*(DWORD*)(__IPSBAR+0x00040C+((x)*0x040)))

/* Bit definitions and macros for MCF_TIMER_DTMR */
#define MCF_TIMER_DTMR_RST          (0x0001)
#define MCF_TIMER_DTMR_CLK(x)       (((x)&0x0003)<<1)
#define MCF_TIMER_DTMR_FRR          (0x0008)
#define MCF_TIMER_DTMR_ORRI         (0x0010)
#define MCF_TIMER_DTMR_OM           (0x0020)
#define MCF_TIMER_DTMR_CE(x)        (((x)&0x0003)<<6)
#define MCF_TIMER_DTMR_PS(x)        (((x)&0x00FF)<<8)
#define MCF_TIMER_DTMR_CE_ANY       (0x00C0)
#define MCF_TIMER_DTMR_CE_FALL      (0x0080)
#define MCF_TIMER_DTMR_CE_RISE      (0x0040)
#define MCF_TIMER_DTMR_CE_NONE      (0x0000)
#define MCF_TIMER_DTMR_CLK_DTIN     (0x0006)
#define MCF_TIMER_DTMR_CLK_DIV16    (0x0004)
#define MCF_TIMER_DTMR_CLK_DIV1     (0x0002)
#define MCF_TIMER_DTMR_CLK_STOP     (0x0000)

/* Bit definitions and macros for MCF_TIMER_DTXMR */
#define MCF_TIMER_DTXMR_MODE16      (0x01)
#define MCF_TIMER_DTXMR_DMAEN       (0x80)

/* Bit definitions and macros for MCF_TIMER_DTER */
#define MCF_TIMER_DTER_CAP          (0x01)
#define MCF_TIMER_DTER_REF          (0x02)

/********************************************************************/

#endif /* __MCF523X_TIMER_H__ */
