/************************************************************************************************/
/*CopyRight:																					*/
/*						天津凯发电器股份有限公司												*/
/*Description:																					*/
/*						系统常用数据类型及类型转换定义															*/
/*Modification history:																			*/
/*						创建：	zqz 	08/03/21												*/
/*																								*/
/************************************************************************************************/

#ifndef _SYSDEFS_H_
#define _SYSDEFS_H_

#include "syscfg.h"

#define NEAR                near
#define FAR                 far

#undef  TRUE
#define TRUE                1

#undef  FALSE
#define FALSE               0

#undef  YES
#define YES					1

#undef	NO					
#define NO					0
#undef  ON
#define ON                  1

#undef  OFF
#define OFF                 0

#undef  SUCCESS
#define SUCCESS             1

#undef  FAILURE
#define FAILURE             0

//define uniform data type for system
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned int        UINT;
typedef UINT *              PUINT;
typedef UINT *              LPUINT;
typedef long	            LONG;
typedef short               SHORT;
typedef char *              LPSTR;
typedef const char *        LPCSTR;
typedef BYTE *              PBYTE;
typedef BYTE *              LPBYTE;
typedef int *               PINT;
typedef int *               LPINT;
typedef WORD *              PWORD;
typedef WORD *              LPWORD;
typedef long *              PLONG;
typedef long *              LPLONG;
typedef DWORD *             PDWORD;
typedef DWORD *             LPDWORD;
typedef void *              LPVOID;
typedef void *              PVOID;
typedef unsigned int        RESULT;
typedef unsigned long       HANDLE;


#define ArraySize(a)        (sizeof(a)/sizeof(a[0]))

//define basic type exchange operate
#define PTRTOINT(x)         (*((UINT far*)x))
#define PTRTODWORD(x)       (*((DWORD far*)x))
#define TOBYTE(x)           (*((LPBYTE)&x))
#define TOUINT(x)           (*((LPUINT)&x))
#define TODWORD(x)          (*((LPDWORD)&x))
#define TOWORD(x)           (*((LPWORD)&x))
#define ArraySize(a)        (sizeof(a)/sizeof(a[0]))

#define TEST_BITS(a,b)		(a&b)
#define SET_BITS(a,b)		(a |= (0x01<<(b)))
#define CLEAR_BITS(a,b)		(a &= ~(b))
#define GET_MASK(a)			(0x01<<a)

//对于9S12可能存在内部和外部RAM空间的情况
#undef _SYS_USE_OUTMEM

#ifdef	_SYS_USE_OUTMEM 		
	#define PFAR	far
#else
	#define PFAR	
#endif

//根据CPU类型定义最高级优先级
#if(SYS_CPU_TYPE == CPU_ARM7)
	#define MAX_ILEV	0		//对于ARM，只能屏蔽全部IRQ
#elif(SYS_CPU_TYPE == CPU_COLDFIRE)
	#define MAX_ILEV	7		
#elif(SYS_CPU_TYPE == CPU_X86)
	#define MAX_ILEV	1
#elif(SYS_CPU_TYPE == CPU_HC12)
	#define MAX_ILEV	7
#endif


#if(OS_PLATFORM != OS_WIN32)

	//define storage type operate that's independence CPU type
	#define LOBYTE(w)           ((BYTE)(w))
	#define HIBYTE(w)           ((BYTE)((WORD)(w) >> 8))
	#define LOWORD(l)           ((WORD)(l))
	#define HIWORD(l)           ((WORD)((DWORD)(l) >> 16))
	#define LLBYTE(w)			((BYTE)(w))
	#define LHBYTE(w)			((BYTE)((WORD)(w) >> 8))
	#define HLBYTE(w)			((BYTE)((DWORD)(w) >> 16))
	#define HHBYTE(w)			((BYTE)((DWORD)(w) >>24))
				
	#define LOLOBYTE(w)         ((BYTE)(w))
	#define LOHIBYTE(w)         ((BYTE)((WORD)(w) >> 8))
	#define HILOBYTE(w)         ((BYTE)((DWORD)(w) >> 16))
	#define HIHIBYTE(w)         ((BYTE)((DWORD)(w) >>24))
				
	#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
	#define MAKEDWORD(ll,lh,hl,hh) (MAKELONG(MAKEWORD(ll,lh),MAKEWORD(hl,hh)))
	#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))
	#define MAKEADDR(ptr)       ((LONG)((LPBYTE)ptr))
	
	#ifndef NULL
	#define NULL		0
	#endif
	
	typedef signed char 		 SBYTE;
	typedef signed short		 SWORD;
	typedef signed long 		 SDWORD;
	typedef BYTE *PFAR			 F_PBYTE;
	typedef WORD *PFAR			 F_PWORD;
	typedef long *PFAR			 F_PLONG;
	typedef char				 STRING;
	typedef unsigned long long	 QWORD;			
	typedef long long	 		 LLONG; 			
#endif 

#if(OS_PLATFORM != OS_VXWORKS)
	typedef int		STATUS;
	typedef int		BOOL;
#endif

#if(OS_PLATFORM == OS_UCOSII)
	//操作系统平台使用的数据类型定义
	#define OSINT8				BYTE
	#define OSINT16 			WORD
	#define OSINT32 			DWORD
#else
	//操作系统平台使用的数据类型定义
	#define OSINT8				DWORD
	#define OSINT16 			DWORD
	#define OSINT32 			DWORD
#endif


//任务入口函数类型定义
typedef void (*TTASK_ENTRY)(void*);

//定时器执行的函数指针
typedef void (*TTimerFunc )(DWORD,void*);

//为适应各种实时操作系统对任务、队列、信号量、定时器等对象的描述定义，本系统
//对所有相应的对象描述都定义为空指针方式
typedef void* TTaskHandle;
typedef void* TQueueHandle;
typedef void* TMutexHandle;
typedef void* TTimerHandle;
typedef void* TCSemHandle;

//定义链表、hash表等的叠代数据结构
typedef void*               ITERATE;
//叠代结束标志
#define ITERATE_END     ((ITERATE)NULL)
//叠代开始标志
#define ITERATE_BEGIN   ((ITERATE)-1)

// declare hash function
typedef DWORD (*HashFunc)(DWORD);

#define inportb(addr)  	*( (unsigned char *)(addr) )
#define outportb(addr,data)   *( (unsigned char *)(addr) )=(data)
#define inport(addr)  	*( (unsigned short *)(addr) )
#define outport(addr,data)   *( (unsigned short *)(addr) )=(data)
#define inportl(addr)	*( (unsigned long *)(addr) )
#define outportl(addr,data)	 *( (unsigned long *)(addr) ) = (data)

#define SysDelay(num)	\
{\
	while(num)	num--;\
}

//中断函数入口类型定义
typedef void (*TISR_ENTRY)(void*);	//用(...)时编译有问题，不知道啥原因

/*----------------------------------------------------------------------------------------*/
/* 把地址按某种方式对齐                                                                   */
/*----------------------------------------------------------------------------------------*/
#define ALIGN(addr,align)  (((DWORD)(addr)+(align)-1)&(~(align-1)))
#define ALIGNMENT                4

//临时定义
#define __DEBUG_MESSAGE 	NO




#endif	//define _SYSDEFS_H_
