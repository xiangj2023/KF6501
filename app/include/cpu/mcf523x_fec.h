/*
 * File:    mcf523x_fec.h
 * Purpose: Register and bit definitions for the MCF523X
 *
 * Notes:   
 *  
 */

#ifndef __MCF523X_FEC_H__
#define __MCF523X_FEC_H__

/*********************************************************************
*
* Fast Ethernet Controller (FEC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_FEC_EIR                   (*(DWORD*)(void*)(&__IPSBAR[0x001004]))
#define MCF_FEC_EIMR                  (*(DWORD*)(void*)(&__IPSBAR[0x001008]))
#define MCF_FEC_RDAR                  (*(DWORD*)(void*)(&__IPSBAR[0x001010]))
#define MCF_FEC_TDAR                  (*(DWORD*)(void*)(&__IPSBAR[0x001014]))
#define MCF_FEC_ECR                   (*(DWORD*)(void*)(&__IPSBAR[0x001024]))
#define MCF_FEC_MMFR                  (*(DWORD*)(void*)(&__IPSBAR[0x001040]))
#define MCF_FEC_MSCR                  (*(DWORD*)(void*)(&__IPSBAR[0x001044]))
#define MCF_FEC_MIBC                  (*(DWORD*)(void*)(&__IPSBAR[0x001064]))
#define MCF_FEC_RCR                   (*(DWORD*)(void*)(&__IPSBAR[0x001084]))
#define MCF_FEC_TCR                   (*(DWORD*)(void*)(&__IPSBAR[0x0010C4]))
#define MCF_FEC_PALR                  (*(DWORD*)(void*)(&__IPSBAR[0x0010E4]))
#define MCF_FEC_PAUR                  (*(DWORD*)(void*)(&__IPSBAR[0x0010E8]))
#define MCF_FEC_OPD                   (*(DWORD*)(void*)(&__IPSBAR[0x0010EC]))
#define MCF_FEC_IAUR                  (*(DWORD*)(void*)(&__IPSBAR[0x001118]))
#define MCF_FEC_IALR                  (*(DWORD*)(void*)(&__IPSBAR[0x00111C]))
#define MCF_FEC_GAUR                  (*(DWORD*)(void*)(&__IPSBAR[0x001120]))
#define MCF_FEC_GALR                  (*(DWORD*)(void*)(&__IPSBAR[0x001124]))
#define MCF_FEC_TFWR                  (*(DWORD*)(void*)(&__IPSBAR[0x001144]))
#define MCF_FEC_FRBR                  (*(DWORD*)(void*)(&__IPSBAR[0x00114C]))
#define MCF_FEC_FRSR                  (*(DWORD*)(void*)(&__IPSBAR[0x001150]))
#define MCF_FEC_ERDSR                 (*(DWORD*)(void*)(&__IPSBAR[0x001180]))
#define MCF_FEC_ETDSR                 (*(DWORD*)(void*)(&__IPSBAR[0x001184]))
#define MCF_FEC_EMRBR                 (*(DWORD*)(void*)(&__IPSBAR[0x001188]))
#define MCF_FEC_RMON_T_DROP           (*(DWORD*)(void*)(&__IPSBAR[0x001200]))
#define MCF_FEC_RMON_T_PACKETS        (*(DWORD*)(void*)(&__IPSBAR[0x001204]))
#define MCF_FEC_RMON_T_BC_PKT         (*(DWORD*)(void*)(&__IPSBAR[0x001208]))
#define MCF_FEC_RMON_T_MC_PKT         (*(DWORD*)(void*)(&__IPSBAR[0x00120C]))
#define MCF_FEC_RMON_T_CRC_ALIGN      (*(DWORD*)(void*)(&__IPSBAR[0x001210]))
#define MCF_FEC_RMON_T_UNDERSIZE      (*(DWORD*)(void*)(&__IPSBAR[0x001214]))
#define MCF_FEC_RMON_T_OVERSIZE       (*(DWORD*)(void*)(&__IPSBAR[0x001218]))
#define MCF_FEC_RMON_T_FRAG           (*(DWORD*)(void*)(&__IPSBAR[0x00121C]))
#define MCF_FEC_RMON_T_JAB            (*(DWORD*)(void*)(&__IPSBAR[0x001220]))
#define MCF_FEC_RMON_T_COL            (*(DWORD*)(void*)(&__IPSBAR[0x001224]))
#define MCF_FEC_RMON_T_P64            (*(DWORD*)(void*)(&__IPSBAR[0x001228]))
#define MCF_FEC_RMON_T_P65TO127       (*(DWORD*)(void*)(&__IPSBAR[0x00122C]))
#define MCF_FEC_RMON_T_P128TO255      (*(DWORD*)(void*)(&__IPSBAR[0x001230]))
#define MCF_FEC_RMON_T_P256TO511      (*(DWORD*)(void*)(&__IPSBAR[0x001234]))
#define MCF_FEC_RMON_T_P512TO1023     (*(DWORD*)(void*)(&__IPSBAR[0x001238]))
#define MCF_FEC_RMON_T_P1024TO2047    (*(DWORD*)(void*)(&__IPSBAR[0x00123C]))
#define MCF_FEC_RMON_T_P_GTE2048      (*(DWORD*)(void*)(&__IPSBAR[0x001240]))
#define MCF_FEC_RMON_T_OCTETS         (*(DWORD*)(void*)(&__IPSBAR[0x001244]))
#define MCF_FEC_IEEE_T_DROP           (*(DWORD*)(void*)(&__IPSBAR[0x001248]))
#define MCF_FEC_IEEE_T_FRAME_OK       (*(DWORD*)(void*)(&__IPSBAR[0x00124C]))
#define MCF_FEC_IEEE_T_1COL           (*(DWORD*)(void*)(&__IPSBAR[0x001250]))
#define MCF_FEC_IEEE_T_MCOL           (*(DWORD*)(void*)(&__IPSBAR[0x001254]))
#define MCF_FEC_IEEE_T_DEF            (*(DWORD*)(void*)(&__IPSBAR[0x001258]))
#define MCF_FEC_IEEE_T_LCOL           (*(DWORD*)(void*)(&__IPSBAR[0x00125C]))
#define MCF_FEC_IEEE_T_EXCOL          (*(DWORD*)(void*)(&__IPSBAR[0x001260]))
#define MCF_FEC_IEEE_T_MACERR         (*(DWORD*)(void*)(&__IPSBAR[0x001264]))
#define MCF_FEC_IEEE_T_CSERR          (*(DWORD*)(void*)(&__IPSBAR[0x001268]))
#define MCF_FEC_IEEE_T_SQE            (*(DWORD*)(void*)(&__IPSBAR[0x00126C]))
#define MCF_FEC_IEEE_T_FDXFC          (*(DWORD*)(void*)(&__IPSBAR[0x001270]))
#define MCF_FEC_IEEE_T_OCTETS_OK      (*(DWORD*)(void*)(&__IPSBAR[0x001274]))
#define MCF_FEC_RMON_R_PACKETS        (*(DWORD*)(void*)(&__IPSBAR[0x001284]))
#define MCF_FEC_RMON_R_BC_PKT         (*(DWORD*)(void*)(&__IPSBAR[0x001288]))
#define MCF_FEC_RMON_R_MC_PKT         (*(DWORD*)(void*)(&__IPSBAR[0x00128C]))
#define MCF_FEC_RMON_R_CRC_ALIGN      (*(DWORD*)(void*)(&__IPSBAR[0x001290]))
#define MCF_FEC_RMON_R_UNDERSIZE      (*(DWORD*)(void*)(&__IPSBAR[0x001294]))
#define MCF_FEC_RMON_R_OVERSIZE       (*(DWORD*)(void*)(&__IPSBAR[0x001298]))
#define MCF_FEC_RMON_R_FRAG           (*(DWORD*)(void*)(&__IPSBAR[0x00129C]))
#define MCF_FEC_RMON_R_JAB            (*(DWORD*)(void*)(&__IPSBAR[0x0012A0]))
#define MCF_FEC_RMON_R_RESVD_0        (*(DWORD*)(void*)(&__IPSBAR[0x0012A4]))
#define MCF_FEC_RMON_R_P64            (*(DWORD*)(void*)(&__IPSBAR[0x0012A8]))
#define MCF_FEC_RMON_R_P65TO127       (*(DWORD*)(void*)(&__IPSBAR[0x0012AC]))
#define MCF_FEC_RMON_R_P128TO255      (*(DWORD*)(void*)(&__IPSBAR[0x0012B0]))
#define MCF_FEC_RMON_R_P256TO511      (*(DWORD*)(void*)(&__IPSBAR[0x0012B4]))
#define MCF_FEC_RMON_R_512TO1023      (*(DWORD*)(void*)(&__IPSBAR[0x0012B8]))
#define MCF_FEC_RMON_R_P_GTE2048      (*(DWORD*)(void*)(&__IPSBAR[0x0012C0]))
#define MCF_FEC_RMON_R_1024TO2047     (*(DWORD*)(void*)(&__IPSBAR[0x0012BC]))
#define MCF_FEC_RMON_R_OCTETS         (*(DWORD*)(void*)(&__IPSBAR[0x0012C4]))
#define MCF_FEC_IEEE_R_DROP           (*(DWORD*)(void*)(&__IPSBAR[0x0012C8]))
#define MCF_FEC_IEEE_R_FRAME_OK       (*(DWORD*)(void*)(&__IPSBAR[0x0012CC]))
#define MCF_FEC_IEEE_R_CRC            (*(DWORD*)(void*)(&__IPSBAR[0x0012D0]))
#define MCF_FEC_IEEE_R_ALIGN          (*(DWORD*)(void*)(&__IPSBAR[0x0012D4]))
#define MCF_FEC_IEEE_R_MACERR         (*(DWORD*)(void*)(&__IPSBAR[0x0012D8]))
#define MCF_FEC_IEEE_R_FDXFC          (*(DWORD*)(void*)(&__IPSBAR[0x0012DC]))
#define MCF_FEC_IEEE_R_OCTETS_OK      (*(DWORD*)(void*)(&__IPSBAR[0x0012E0]))

/* Bit definitions and macros for MCF_FEC_EIR */
#define MCF_FEC_EIR_UN                  (0x00080000)
#define MCF_FEC_EIR_RL                  (0x00100000)
#define MCF_FEC_EIR_LC                  (0x00200000)
#define MCF_FEC_EIR_EBERR               (0x00400000)
#define MCF_FEC_EIR_MII                 (0x00800000)
#define MCF_FEC_EIR_RXB                 (0x01000000)
#define MCF_FEC_EIR_RXF                 (0x02000000)
#define MCF_FEC_EIR_TXB                 (0x04000000)
#define MCF_FEC_EIR_TXF                 (0x08000000)
#define MCF_FEC_EIR_GRA                 (0x10000000)
#define MCF_FEC_EIR_BABT                (0x20000000)
#define MCF_FEC_EIR_BABR                (0x40000000)
#define MCF_FEC_EIR_HBERR               (0x80000000)
#define MCF_FEC_EIR_CLEAR_ALL           (0xFFFFFFFF)

/* Bit definitions and macros for MCF_FEC_EIMR */
#define MCF_FEC_EIMR_UN                 (0x00080000)
#define MCF_FEC_EIMR_RL                 (0x00100000)
#define MCF_FEC_EIMR_LC                 (0x00200000)
#define MCF_FEC_EIMR_EBERR              (0x00400000)
#define MCF_FEC_EIMR_MII                (0x00800000)
#define MCF_FEC_EIMR_RXB                (0x01000000)
#define MCF_FEC_EIMR_RXF                (0x02000000)
#define MCF_FEC_EIMR_TXB                (0x04000000)
#define MCF_FEC_EIMR_TXF                (0x08000000)
#define MCF_FEC_EIMR_GRA                (0x10000000)
#define MCF_FEC_EIMR_BABT               (0x20000000)
#define MCF_FEC_EIMR_BABR               (0x40000000)
#define MCF_FEC_EIMR_HBERR              (0x80000000)
#define MCF_FEC_EIMR_MASK_ALL           (0x00000000)
#define MCF_FEC_EIMR_UNMASK_ALL         (0xFFFFFFFF)

/* Bit definitions and macros for MCF_FEC_RDAR */
#define MCF_FEC_RDAR_R_DES_ACTIVE       (0x01000000)

/* Bit definitions and macros for MCF_FEC_TDAR */
#define MCF_FEC_TDAR_X_DES_ACTIVE       (0x01000000)

/* Bit definitions and macros for MCF_FEC_ECR */
#define MCF_FEC_ECR_RESET               (0x00000001)
#define MCF_FEC_ECR_ETHER_EN            (0x00000002)

/* Bit definitions and macros for MCF_FEC_MMFR */
#define MCF_FEC_MMFR_DATA(x)            (((x)&0x0000FFFF)<<0)
#define MCF_FEC_MMFR_TA(x)              (((x)&0x00000003)<<16)
#define MCF_FEC_MMFR_RA(x)              (((x)&0x0000001F)<<18)
#define MCF_FEC_MMFR_PA(x)              (((x)&0x0000001F)<<23)
#define MCF_FEC_MMFR_OP(x)              (((x)&0x00000003)<<28)
#define MCF_FEC_MMFR_ST(x)              (((x)&0x00000003)<<30)
#define MCF_FEC_MMFR_ST_01              (0x40000000)
#define MCF_FEC_MMFR_OP_READ            (0x20000000)
#define MCF_FEC_MMFR_OP_WRITE           (0x10000000)
#define MCF_FEC_MMFR_TA_10              (0x00020000)

/* Bit definitions and macros for MCF_FEC_MSCR */
#define MCF_FEC_MSCR_MII_SPEED(x)       (((x)&0x0000003F)<<1)
#define MCF_FEC_MSCR_DIS_PREAMBLE       (0x00000080)

/* Bit definitions and macros for MCF_FEC_MIBC */
#define MCF_FEC_MIBC_MIB_IDLE           (0x40000000)
#define MCF_FEC_MIBC_MIB_DISABLE        (0x80000000)

/* Bit definitions and macros for MCF_FEC_RCR */
#define MCF_FEC_RCR_LOOP                (0x00000001)
#define MCF_FEC_RCR_DRT                 (0x00000002)
#define MCF_FEC_RCR_MII_MODE            (0x00000004)
#define MCF_FEC_RCR_PROM                (0x00000008)
#define MCF_FEC_RCR_BC_REJ              (0x00000010)
#define MCF_FEC_RCR_FCE                 (0x00000020)
#define MCF_FEC_RCR_MAX_FL(x)           (((x)&0x000007FF)<<16)

/* Bit definitions and macros for MCF_FEC_TCR */
#define MCF_FEC_TCR_GTS                 (0x00000001)
#define MCF_FEC_TCR_HBC                 (0x00000002)
#define MCF_FEC_TCR_FDEN                (0x00000004)
#define MCF_FEC_TCR_TFC_PAUSE           (0x00000008)
#define MCF_FEC_TCR_RFC_PAUSE           (0x00000010)

/* Bit definitions and macros for MCF_FEC_PAUR */
#define MCF_FEC_PAUR_TYPE(x)            (((x)&0x0000FFFF)<<0)
#define MCF_FEC_PAUR_PADDR2(x)          (((x)&0x0000FFFF)<<16)

/* Bit definitions and macros for MCF_FEC_OPD */
#define MCF_FEC_OPD_PAUSE_DUR(x)        (((x)&0x0000FFFF)<<0)
#define MCF_FEC_OPD_OPCODE(x)           (((x)&0x0000FFFF)<<16)

/* Bit definitions and macros for MCF_FEC_TFWR */
#define MCF_FEC_TFWR_X_WMRK(x)          (((x)&0x00000003)<<0)

/* Bit definitions and macros for MCF_FEC_FRBR */
#define MCF_FEC_FRBR_R_BOUND(x)         (((x)&0x000000FF)<<2)

/* Bit definitions and macros for MCF_FEC_FRSR */
#define MCF_FEC_FRSR_R_FSTART(x)        (((x)&0x000000FF)<<2)

/* Bit definitions and macros for MCF_FEC_ERDSR */
#define MCF_FEC_ERDSR_R_DES_START(x)    (((x)&0x3FFFFFFF)<<2)

/* Bit definitions and macros for MCF_FEC_ETDSR */
#define MCF_FEC_ETDSR_X_DES_START(x)    (((x)&0x3FFFFFFF)<<2)

/* Bit definitions and macros for MCF_FEC_EMRBR */
#define MCF_FEC_EMRBR_R_BUF_SIZE(x)     (((x)&0x0000007F)<<4)

/********************************************************************/

#endif /* __MCF523X_FEC_H__ */
