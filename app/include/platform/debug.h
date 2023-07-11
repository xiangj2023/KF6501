#ifndef _DEBUG_H
#define _DEBUG_H

#include "sysdefs.h"
#include "system.h"

//STATUS DB_WriteRunRecord(char *str);

/*�ض��壬���ڵ���, ע�����Abort������Ҫ���¿������*/
#if(__DEBUG__ == YES)
	#define STATIC
	#define Abort(code)		OS_Exit(code)

	#define ASSERT(f)	/*\
		do{\
			if(!(f)) \
			  AssertFaild(#f,FILE_LINE); \
		}while(0); 
*/		
#else
	#define STATIC	static
	#define Abort(code) 	OS_Exit(code)
	#define ASSERT(f)     
#endif

/*���������Ϣ�������*/
#if(_MESSAGE_OUTPUT == __TO_CONSOLE__)
	#define DebugOutput(str) printf("%s",str)
#elif(_MESSAGE_OUTPUT == __TO_NVRAM__)
	#define DebugOutput(str) DB_WriteRunRecord(str)
#elif(_MESSAGE_OUTPUT == __TO_ALL__)
	#define DebugOutput(str) {DB_WriteRunRecord(str); printf("%s",str);}
#elif(_MESSAGE_OUTPUT == __TO_NULL__)
	#define DebugOutput(str) 
#endif

/*�ض���ReportMsg���*/
#if(__REP_MSG__ == YES)
	#define ReportMsg	 ReportNormal
#else
	#define ReportMsg
#endif

/*�ض���LogError���*/
#if(__LOG_ERR__ == YES)
	#define LogError	 ReportError
#else
	#define LogError
#endif

#if(__INT_LOG__ == YES)
	#if(OS_PLATFORM == OS_VXWORKS)
		#define RapidLog(iof)		logMsg(iof,0,0,0,0,0,0)
	#else
		#define RapidLog(iof)		ReportNormal(iof)
	#endif
#else
	#define RapidLog(iof)
#endif

//ȡ��ǰ���ļ������к�
#define FILE_LINE (FileLine(__FILE__,__LINE__))
char *FileLine(char file[],int line);

void ReportNormal(char *iof,...);

void ReportError(char *func,char *file,char *iof,...);

#if (__DEBUG__ == YES)
	void AssertFaild(LPSTR str,LPSTR fileline);
#endif

#define TRACE	

#endif


