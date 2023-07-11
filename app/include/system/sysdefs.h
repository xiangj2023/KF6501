/************************************************************************************************/
/*CopyRight:																					*/
/*						��򿭷������ɷ����޹�˾												*/
/*Description:																					*/
/*						ϵͳ�����������ͼ�����ת������															*/
/*Modification history:																			*/
/*						������	zqz 	08/03/21												*/
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

//����9S12���ܴ����ڲ����ⲿRAM�ռ�����
#undef _SYS_USE_OUTMEM

#ifdef	_SYS_USE_OUTMEM 		
	#define PFAR	far
#else
	#define PFAR	
#endif

//����CPU���Ͷ�����߼����ȼ�
#if(SYS_CPU_TYPE == CPU_ARM7)
	#define MAX_ILEV	0		//����ARM��ֻ������ȫ��IRQ
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
	//����ϵͳƽ̨ʹ�õ��������Ͷ���
	#define OSINT8				BYTE
	#define OSINT16 			WORD
	#define OSINT32 			DWORD
#else
	//����ϵͳƽ̨ʹ�õ��������Ͷ���
	#define OSINT8				DWORD
	#define OSINT16 			DWORD
	#define OSINT32 			DWORD
#endif


//������ں������Ͷ���
typedef void (*TTASK_ENTRY)(void*);

//��ʱ��ִ�еĺ���ָ��
typedef void (*TTimerFunc )(DWORD,void*);

//Ϊ��Ӧ����ʵʱ����ϵͳ�����񡢶��С��ź�������ʱ���ȶ�����������壬��ϵͳ
//��������Ӧ�Ķ�������������Ϊ��ָ�뷽ʽ
typedef void* TTaskHandle;
typedef void* TQueueHandle;
typedef void* TMutexHandle;
typedef void* TTimerHandle;
typedef void* TCSemHandle;

//��������hash��ȵĵ������ݽṹ
typedef void*               ITERATE;
//����������־
#define ITERATE_END     ((ITERATE)NULL)
//������ʼ��־
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

//�жϺ���������Ͷ���
typedef void (*TISR_ENTRY)(void*);	//��(...)ʱ���������⣬��֪��ɶԭ��

/*----------------------------------------------------------------------------------------*/
/* �ѵ�ַ��ĳ�ַ�ʽ����                                                                   */
/*----------------------------------------------------------------------------------------*/
#define ALIGN(addr,align)  (((DWORD)(addr)+(align)-1)&(~(align-1)))
#define ALIGNMENT                4

//��ʱ����
#define __DEBUG_MESSAGE 	NO




#endif	//define _SYSDEFS_H_
