
#include "debug.h"

STATUS DB_WriteRunRecord(char *str);

//调试输出控制台长度
#if(OS_PLATFORM == OS_UCOSII)
	#define MAX_STDOUT_STRING	64
#else
	#define MAX_STDOUT_STRING	1024
	
	//任务Hash表
	THashTable *g_hashTaskMap;
#endif

//输出控制台缓冲区
STATIC STRING g_pStdOut[MAX_STDOUT_STRING];
//输出信息总条目计数
STATIC DWORD g_nOutPut=0;

void ReportNormal(char *iof,...)
{
    DWORD len;
    va_list args;
    char *pbuf = (char*)g_pStdOut;

    //使用标准控制台加锁
    OS_LockM(STDOUT_LOCK);

    //向异常信息缓冲区写入:异常信息总计数、当前任务名称、当前文件名称和行号、当前时间
    len = sprintf(pbuf,"\r\nE%02d:",g_nOutPut);

    //写入可变参数
    va_start(args,iof);
    len += vsprintf(pbuf+len,iof, args);
    va_end(args);

    //向控制台输出的字符串长度应小于控制台的最大缓冲区
    //ASSERT(strlen(pbuf)<MAX_STDOUT_STRING);

    //根据debug.h中_DMESSAGE_OUTPUT的定义，重定向输出信息
    DebugOutput(g_pStdOut);    //remed for test

    //输出信息的个数加1
    g_nOutPut++;

    //释放控制台资源
    OS_UnLockM(STDOUT_LOCK);
}

/*--------------------------------------------------------------------------------------*/
/* 功能:系统异常信息登记                                                                */
/* 输入:func 当前函数名;file 用FILE_LINE宏当前程序名称和行号;iof 可变长度参数,使用同    */
/*      printf参数;输出的异常信息包括:异常信息计数,文件名,行号,当前任务,时间,异常信息.. */
/* 输出:无                                                                              */
/* 返回:无                                                                              */
/*--------------------------------------------------------------------------------------*/
void ReportError(char *func,char *file,char *iof,...)
{  
    DWORD len=0;
    va_list args;
   	OS_TaskDesp *pInfo = OS_GetTaskTab(SELF_TASK_ID);
    char *pbuf = (char*)g_pStdOut;

    //使用标准控制台加锁
    OS_LockM(STDOUT_LOCK);

    //向异常信息缓冲区写入:异常信息总计数、当前任务名称、当前文件名称和行号、当前时间
    len = sprintf(pbuf,"\r\nE%02d:[%s]",g_nOutPut,
        (pInfo==NULL)?"null":pInfo->TaskName);

    //如果文件名称不为零，输出文件名称
    if(file != NULL)
        len+=sprintf(pbuf+len,"%s",file);

    //函数名称不为零，输出文件名称
    if(func != NULL)
        len += sprintf(pbuf+len," %s()",func);
  
    //写入可变参数
    va_start(args,iof);
    len += vsprintf(pbuf+len,iof, args);
    va_end(args);

    //根据debug.h中_DMESSAGE_OUTPUT的定义，重定向输出信息
    DebugOutput(g_pStdOut);

    //输出信息的个数加1
    g_nOutPut++;

    //释放控制台资源
    OS_UnLockM(STDOUT_LOCK);
}


/*------------------------------------------------------------------------------------------*/
/* 功能:把当前指令所在的程序名称和行号转化为字符串格式                                      */
/* 输入:file用__FILE__宏,提供当前程序名;line __LINE__宏,提供当前行号                        */
/* 输出:无                                                                                  */
/* 返回:包含程序名和行号的字符串                                                            */
/*------------------------------------------------------------------------------------------*/
char *FileLine(char file[],int line)
{
    static char fileline[52];
    WORD len=strlen(file);
    
    //除去各级目录，取当前文件名
    while((file[len] != '\\')&&(file[len]!='/')&&(len != 0))
        len--;
    if(len != 0)
        len++;
    sprintf(fileline,"%s-%d",&file[len],line);
    return fileline;
}

#if (__DEBUG__ == YES)
void AssertFaild(LPSTR str,LPSTR fileline)
{
    ReportMsg("Assert: \"%s\" failed,in %s\n", str,fileline);
    Abort(EC_ASSERT);
}
#endif


