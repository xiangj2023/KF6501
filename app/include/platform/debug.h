#ifndef _DEBUG_H
#define _DEBUG_H

#include "sysdefs.h"
#include "system.h"

//STATUS DB_WriteRunRecord(char *str);

/*重定义，便于调试, 注意对于Abort函数需要重新考虑设计*/
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

/*定义调试信息输出方向*/
#if(_MESSAGE_OUTPUT == __TO_CONSOLE__)
	#define DebugOutput(str) printf("%s",str)
#elif(_MESSAGE_OUTPUT == __TO_NVRAM__)
	#define DebugOutput(str) DB_WriteRunRecord(str)
#elif(_MESSAGE_OUTPUT == __TO_ALL__)
	#define DebugOutput(str) {DB_WriteRunRecord(str); printf("%s",str);}
#elif(_MESSAGE_OUTPUT == __TO_NULL__)
	#define DebugOutput(str) 
#endif

/*重定义ReportMsg输出*/
#if(__REP_MSG__ == YES)
	#define ReportMsg	 ReportNormal
#else
	#define ReportMsg
#endif

/*重定义LogError输出*/
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

//取当前的文件名和行号
#define FILE_LINE (FileLine(__FILE__,__LINE__))
char *FileLine(char file[],int line);

void ReportNormal(char *iof,...);

void ReportError(char *func,char *file,char *iof,...);

#if (__DEBUG__ == YES)
	void AssertFaild(LPSTR str,LPSTR fileline);
#endif

#define TRACE	

#endif


