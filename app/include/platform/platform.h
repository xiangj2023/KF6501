
#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "memdefs.h"
#include "absosdef.h"
#include "absos.h"
#include "timedef.h"

#if(OS_PLATFORM == OS_VXWORKS)
	#include "osvxwork.h"
#endif

//根据所使用具体操作系统的不同定义每个消息控制块的单元个数
#if(OS_PLATFORM == OS_UCOSII)
	#define MAX_UNIT_NUM			4
	#define TASK_PRI_BASE			10	/*任务优先级起始值*/
	#define MAX_APP_TASK_NUM		20	/*系统应用任务最大个数*/
	#define MIN_TASK_STACK_LEN		64	/*任务堆栈最小长度*/
	#define MAX_NORMAL_TIMER_NUM    16   	//系统最多普通定时器（从初始化开始等时间间隔）个数
	#define MAX_SPECIAL_TIMER_NUM	16		//系统最多特殊定时器（从指定时间开始等时间间隔）个数	
	#define DEFAULT_TASK_STACK    	128 	//默认任务堆栈大小
	#define NORMAL_TIMER_TASK_PRIO  4    	//普通定时器任务优先级,按uCOS-II的建议，前四个优先级最好保留
#else
	#define MAX_UNIT_NUM 			16
	#define TASK_PRI_BASE			0	/*任务优先级起始值*/
	#define MAX_APP_TASK_NUM		128	/*系统应用任务最大个数*/
	#define MIN_TASK_STACK_LEN		256	/*任务堆栈最小长度*/
	#define MAX_NORMAL_TIMER_NUM	64	//系统最多普通定时器（从初始化开始等时间间隔）个数
	#define MAX_SPECIAL_TIMER_NUM	64	//系统最多特殊定时器（从指定时间开始等时间间隔）个数	
	#define DEFAULT_TASK_STACK    	4096 	//默认任务堆栈大小
	#define NORMAL_TIMER_TASK_PRIO	40	//普通定时器任务优先级,用于管理所有普通定时器
#endif


//互斥,同步信号量标识,可根据需要增加
enum {STDOUT_LOCK = 0,MEM_LOCK,SYSMSGNODE_LOCK, DAI_LOCK, SPI_LOCK, IIC_LOCK, FLASH_LOCK, HASH_LOCK};
enum {RELAY_TASK = 0,RESV0, RESV1};

/*---------------------------------------------------------------------*/
/* 在调式模式下分配的内存块结构为:                                     */
/*  TMemTop + malloc memory +gap[MAX_GAPLEN]                           */
/* 当系统空闲时，IDLE任务自动监测分配的内存链，查看内存两端的标志是否  */    
/* 被应用程序改写                                                      */
/*---------------------------------------------------------------------*/
#define MAX_GAPLEN   4
#define MEM_GAP_FILL  0xA5
struct TMemTop
{
    TMemTop *pNext;
    LPCSTR  szFileName;
    int     nLine;
    DWORD   nSize;
    DWORD   hTask;
    BYTE    aGap[MAX_GAPLEN];
};

/*---------------------------------------------------------------------*/
/* 公用信号量表                                                          */
/*---------------------------------------------------------------------*/
typedef struct mutextable{
    LPSTR        pName;         /*信号量名称*/
    TMutexHandle hMutex;        /*RTOS用到的信号量句柄*/    
//    WORD        dwStatus;      /*该信号量状态*/
}TMutexTable;

typedef struct CSemtable{
    LPSTR        pName;         /*信号量名称*/
    TCSemHandle hCSem;        /*RTOS用到的信号量句柄*/    
}TCSemtable;

typedef struct	task_map
{
	OS_TaskDesp *pTask;
}TTask_Map;	

void OS_InitPlat(void);

#if(__DEBUG_MEMORY_T== YES)
    void* OS_MemAllocDebug(int size,char *file,DWORD line);
    void OS_MemFreeDebug(void *pbuf);
#endif

//任务管理
#define OS_TaskRegister 		RtosTaskRegister
#define OS_GetTaskTab			RtosGetTaskTab

//定时器管理
#define OS_Sleep				RtosSleep

//中断管理
#define OS_IntLock				RtosIntLock
#define OS_IntUnLock			RtosIntUnLock
#define OS_SetVector			RtosSetVector
#define OS_IntEnable			RtosIntEnable
#define OS_IntDisable			RtosIntDisable

//信号量管理
#define OS_SemBCreate			RtosSemBCreate
#define OS_SemGet				RtosSemGet
#define OS_SemRelease			RtosSemRelease
#define OS_SemCCreate			RtosSemCCreate

#if(__DEBUG_MEMORY_T== YES)
	#define OS_MemAlloc	OS_MemAllocDebug(int size,char *file=pThisFile,__LINE__);
	#define OS_MemFree 	OS_MemFreeDebug(void *pbuf);
#else
	#define OS_MemAlloc 			RtosMalloc
	#define OS_MemFree				free
#endif
#define OS_MemSet				memset
#define OS_MemCmp				memcmp
#define OS_StrCpy				strcpy
#define OS_StrLen				strlen
#define OS_Printf					printf
#define OS_Sprintf				sprintf
#define OS_Malloc 				malloc


/*************************************************************************************
* 创建一个任务                                                          
*************************************************************************************/
DWORD OS_CreateTask(STRING *name,TTASK_ENTRY func,DWORD pri,DWORD stack,DWORD arg2,DWORD arg3,DWORD arg4);

/*************************************************************************************
* 创建并初始化内存节点控制块                                                           
*************************************************************************************/
void InitMemBlock(dMemBlock **p,  WORD UnitNumOfPerBlock,WORD UnitLen,DWORD SemId);

/*************************************************************************************
* 发送消息                                                           
*************************************************************************************/
DWORD OS_PostMessage(DWORD tid, DWORD msg, DWORD harg, DWORD warg,\
														DWORD parg, DWORD larg);
/*************************************************************************************
* 接收消息                                                           
*************************************************************************************/
DWORD OS_GetMessage(DWORD tid,TMessage* msg, DWORD mode = 0);

/*************************************************************************************
* 设置任务定时器                                                           
*************************************************************************************/
DWORD OS_SetTimer(DWORD tid,DWORD sign,DWORD interval, TSysTime *pBeginTime);

/*************************************************************************************
* 删除任务定时器                                                           
*************************************************************************************/
DWORD OS_DelTimer(DWORD tid,DWORD sign);

/*************************************************************************************
* 任务进入保护状态，禁止其它任务抢占                                                           
*************************************************************************************/
DWORD OS_Protect(DWORD tid);

/*************************************************************************************
* 任务退出保护状态，允许正常调度                                                           
*************************************************************************************/
DWORD OS_UnProtect(DWORD tid);

/*************************************************************************************
* 任务挂起                                                           
*************************************************************************************/
DWORD OS_Suspend(DWORD tid);

/*************************************************************************************
* 任务恢复                                                           
*************************************************************************************/
DWORD OS_Resume(DWORD tid);

/*************************************************************************************
* 任务删除                                                           
*************************************************************************************/
DWORD OS_Kill(DWORD tid);

/*************************************************************************************
* 临界资源保护                                                           
*************************************************************************************/
DWORD OS_LockM(DWORD id);

/*************************************************************************************
* 临界资源保护退出                                                           
*************************************************************************************/
DWORD OS_UnLockM(DWORD id);

/*------------------------------------------------------------------------------------------*/
/* 功能: 全局计数信号量获取操作                       									*/
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemGet(DWORD id, DWORD mode);

/*------------------------------------------------------------------------------------------*/
/* 功能: 全局计数信号量释放操作                       									*/
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemSet(DWORD id);

/*************************************************************************************
* 任务退出                                                           
*************************************************************************************/
void OS_Exit(BYTE code);

/*************************************************************************************
* 当前任务注册,清除当前任务运行计数                                                           
*************************************************************************************/
DWORD OS_TaskRegister(void);

/*************************************************************************************
* 根据不同操作系统类型，将任务入口函数参数转为统一指针形式                                                          
*************************************************************************************/
//DWORD *GetTaskArg(TASKARG arg);

/*************************************************************************************
* 从指定的内存控制块取空闲节点                                                           
*************************************************************************************/
void *OS_GetNode(dMemBlock *pMemBlock);


/*************************************************************************************
* 系统复位                                                           
*************************************************************************************/
void SystemReset(DWORD type);

#endif
