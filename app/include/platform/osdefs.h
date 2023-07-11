#ifndef _OSDEFS_H
#define _OSDEFS_H

#include "sysdefs.h"


//定义二进制信号量标志
#define BSEM_FULL	1		//资源未占用
#define BSEM_EMPTY	0

#define MUTEX_NOCREATE	0

#define MUTEX_ERROR         (1<<0)
#define TASK_PROTECTED      (1<<1)

//任务异常退出错误代码定义
#define EC_MEMORY_INVLID    0
#define EC_MSG_ERROR        1
#define EC_TIMER_ERROR      2
#define EC_RTOS_DISORDER    3
#define EC_RXD_QUIT         4
#define EC_ASSERT           5

/*---------------------------------------------------------------------*/
/* 根据采用的RTOS类型,定义RTOS和通用监控平台的接口                     */
/*---------------------------------------------------------------------*/
#if (OS_PLATFORM == OS_UCOSII)
	#include "ucosii.h"
	
	//任务管理
	#define RtosCreateTask			UcosiiCreateTask
	#define RtosGetCurrentTask		UcosiiGetCurrentTask
	#define RtosGetCurrentTaskId	UcosiiGetCurrentTaskId
	#define RtosProtect				UcosiiProtect
	#define RtosUnProtect			UcosiiUnProtect
	#define RtosSuspend				UcosiiTaskSuspend
	#define RtosResume				UcosiiTaskResume
	#define RtosTaskKill			UcosiiTaskKill
	#define RtosTaskExit			UcosiiExit
	#define RtosLookUp				UcosiiLookUp
	#define RtosTaskRegister			UcosiiTaskRegister
	
	//消息管理
	#define RtosPostMessage			UcosiiPostMessage
	#define RtosGetMessage			UcosiiGetMessage
	
	//定时器管理
	#define RtosSetTimer			UcosiiSetTimer
	#define RtosDelTimer			UcosiiDelTimer
	#define RtosSleep				UcosiiTaskDly
	
	//中断管理
	#define RtosIntEnable			UcosiiIntEnable
	#define RtosIntDisable			UcosiiIntDisable

	#define OS_IntMask              UcosiiIntMask
	#define OS_SetVector			UcosiiSetVector
	
	//信号量管理
	//根据初始化情况，可用作同步、互斥及计数使用
	#define RtosSemBCreate			UcosiiSemCreate
	#define RtosSemGet				UcosiiSemGet
	#define RtosSemRelease			UcosiiSemRelease
	
	//内存管理
	#define RtosMalloc             Ucosii_MemAlloc


#elif(OS_PLATFORM == OS_VXWORKS)
	//任务管理
	#define RtosCreateTask			VXCreateTask
	#define RtosGetTask				VXGetTaskTab
	#define RtosGetCurrentTaskId	VXGetCurrentTaskId
	#define RtosProtect 			VXProtect
	#define RtosUnProtect			VXUnProtect
	#define RtosSuspend 			VXTaskSuspend
	#define RtosResume				VXTaskResume
	#define RtosTaskKill			VXTaskKill
//	#define RtosTaskExit			VXExit
	#define RtosLookUp				VXLookUp
	#define RtosTaskRegister 		VXTaskRegister
	#define RtosGetTaskTab			VXGetTaskTab

	//消息管理
	#define RtosPostMessage 		VXPostMessage
	#define RtosGetMessage			VXGetMessage

	//定时器管理
	#define RtosSetTimer			VXSetTimer
	#define RtosDelTimer			VXDelTimer
	#define RtosSleep				VXTaskDly

	//中断管理
	#define RtosIntLock				VXIntLock
	#define RtosIntUnLock			VXIntUnLock
	#define RtosSetVector			VXSetVector
	#define RtosIntEnable			VXIntEnable
	#define RtosIntDisable			VXIntDisable

	//信号量管理
	//根据初始化情况，可用作同步、互斥及计数使用
	#define RtosSemBCreate			VXBSemCreate
	#define RtosSemGet				VXSemGet
	#define RtosSemRelease			VXSemRelease
	#define RtosSemCCreate			VXCSemCreate
	#define RtosMalloc 				VXMemAlloc//malloc
	
#endif	//endif OS_PLATFORM


#endif
