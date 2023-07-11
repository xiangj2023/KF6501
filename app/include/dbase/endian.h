#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#include "sysdefs.h"

//#define _BIG_ENDIAN_

#ifdef _BIG_ENDIAN_

    #define ltobl(addr)     ( (((ULONG)(addr) & 0x000000FF)<<24) | \
                              (((ULONG)(addr) & 0x0000FF00)<<8)  | \
                              (((ULONG)(addr) & 0x00FF0000)>>8)  | \
                              (((ULONG)(addr) & 0xFF000000)>>24) )
 
    #define btoll(addr)     ltobl(addr)
 
    #define ltobs(addr)     ( (((USHORT)(addr) & 0x000000FF)<<8)  | \
                              (((USHORT)(addr) & 0x0000FF00)>>8)  )
 
    #define btols(addr)     ltobs(addr)
#else

    #define ltobl(a)       ((ULONG)(a))
    #define btoll(a)       ((ULONG)(a))
    #define ltobs(a)       ((USHORT)(a))
    #define btols(a)       ((USHORT)(a))
#endif

#endif
