#if(OS_PLATFORM == OS_VXWORKS)

#include "osvxwork.h"
#include "absos.h"
#include "m5234spi.h"

void *SysMallocPtr = (void *)(SYS_DRAM_BASE+SYS_DRAM_LEN);	//内存分配空域空间指针

//任务Hash表
extern THashTable* g_hashTaskMap;

extern OSINT8 CurTaskCount;

//任务描述表
extern TTask_Map TaskTable[];

/*系统定时器链表头指针*/
OS_TimerDesp *pNormalTimerList;
/*创建的系统定时器的个数*/
BYTE NormalTimerNum;

//普通定时器计数信号量定义
SEM_ID NorTickSem = NULL; 

/*普通定时器管理任务*/
void VXNormalTimerTask(void);

void VXMain(void);

void VXMain(void)
{
	OS_InitPlat();
	
	VXWorksInit();

/*	
	DWORD status;
	status = VX_MkDir( DATA_DIR);
	if(status == FILE_OK)
		ReportMsg("Receate data dir OK !");
	VX_DeleteDir(TEMP_DIR);
	status = VX_MkDir(TEMP_DIR);
	if(status == FILE_OK)
		ReportMsg("Recreate temp dir OK !");
*/
	//初始化系统硬件时钟
	M5234SpiInit(); 
}


/*------------------------------------------------------------------------------------------*/
/* 功能: 操作系统初始化                                                                         */
/* 输入: 无                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void VXWorksInit(void)
{
	OSINT8 taskid; 

	//初始化系统定时器链表头
	pNormalTimerList=NULL;

	//初始化系统定时器的个数
	NormalTimerNum=0;

	//创建普通定时器计数信号量
	NorTickSem = semCCreate(SEM_Q_PRIORITY, 0);
	ASSERT(NorTickSem != NULL);

	//与UCOSII不同，系统空闲定时器内存分配控制块的空间不单独管理，每个定时器节点空间直接从
	//系统内存分配
	//创建普通定时器管理任务
	taskid = taskSpawn("NTimeTask", NORMAL_TIMER_TASK_PRIO, 0, DEFAULT_TASK_STACK, \
					(FUNCPTR)VXNormalTimerTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ASSERT(taskid != ERROR);

/*
	//创建系统空闲定时器内存分区控制块
	pFreeNormalTimer=(OS_MEM *)0;
	addr=Ucosii_MemAlloc(MAX_NORMAL_TIMER_NUM*sizeof(OS_TimerDesp));
	if(addr)
	   pFreeNormalTimer=OSMemCreate (addr, MAX_NORMAL_TIMER_NUM, sizeof(OS_TimerDesp), &err);

	//申请任务栈空间
	addr0 = (BYTE *)Ucosii_MemAlloc(DEFAULT_TASK_STACK);
	if(addr!= NULL)
	{
		//根据栈的增长方向确定栈指针
		#if OS_STK_GROWTH==0
			pStack=(OS_STK *)addr0;
		#else
			pStack=(OS_STK *)(addr0+StackSize-4);
		#endif

		//创建普通定时器任务
		OSTaskCreate(UcosiiNormalTimerTask, 0, pStack, NORMAL_TIMER_TASK_PRIO);
	}
*/
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 普通定时器任务,接收tick中断函数的同步信号量，管理所有普通定时器，                  */
/*		 并向对应任务发送定时消息															*/
/* 输入: 无                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void VXNormalTimerTask(void)
{
	OS_TimerDesp *pTimerNode;
//	STATIC int TimerCnt;
	int rtn;
	
	while(1)
	{
		//以悬挂方式等待时钟滴嗒的到来，采用计数信号量的方式和OSTimeTickHook()同步
		semTake(NorTickSem, WAIT_FOREVER);
		
//		if(rtn==ERR_SUCCESS)
		{
//			TimerCnt++;
//			if((TimerCnt%200)==0)
//				ReportMsg("Timer Task Run OK, TimerCnt=%d....", TimerCnt);
			//如果定时器任务优先级比其它应用程序优先级都高,那么此处可不用保护。

			//禁止任务切换
			taskLock();

			//检查定时器链表
			pTimerNode=pNormalTimerList;
			while(pTimerNode!=NULL)
			{
				if(pTimerNode->CurTime>0)	//定时时间未到
				{
					pTimerNode->CurTime--;//计数值递减操作
					if(pTimerNode->CurTime == 0)
					{
						//调整当前计数值
						pTimerNode->CurTime=pTimerNode->InitTime;
						
						//向相应的任务发送定时消息
						VXPostMessage(pTimerNode->pTask,SM_TIMEOUT,pTimerNode->dwEvtBit,0,0,0);
					}
						
				}
				//调整指针
				pTimerNode=pTimerNode->pNextTimer;
			}
			//恢复任务切换
			taskUnlock();
		}//if
	}//while(1)
}


/*------------------------------------------------------------------------------------------*/
/* 功能: 创建一个任务                                                                       */
/* 输入: pTask--任务控制块																	*/
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS创建成功,其它创建失败                                                   */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXCreateTask(OS_TaskDesp *pTask)
{
	BYTE *addr = NULL;
	OSINT8 taskid; 
//	SEM_ID pSem = NULL;
	DWORD mask;
	MSG_Q_ID msgid;
	
	//检查并调整任务栈大小				
	if(pTask->StackSize == 0)
		pTask->StackSize = MIN_TASK_STACK_LEN;				
	
	//申请任务栈空间
	addr = (BYTE *)VXMemAlloc(pTask->StackSize);
	ASSERT(addr != NULL);
	
	//栈指针放在任务描述表寄存器里保存
	pTask->Register[REG_TASK_STACK]=(DWORD)addr;
/*
	//创建各任务消息队列计数信号量,保证在内核创建任务之前进行
	pSem = semCCreate(SEM_Q_PRIORITY, 0);
	ASSERT(pSem != NULL);
	pTask->Register[REG_MSG_SEM]=(DWORD)pSem;
*/
	CurTaskCount++;	
	ASSERT(CurTaskCount<=MAX_APP_TASK_NUM);

	//初始化定时器表内容为无效
	for(int i=0; i<MAX_TIMER; i++)
		pTask->dwTimeTab[i] = 0xffffffff;

	//保存应用层ID
	pTask->dwTaskID = (CurTaskCount+TASK_HANDLE);

	//保存应用层ID
	pTask->Args[0] = pTask->dwTaskID;//(DWORD)pTask;

	//创建任务(注意参数传递调整)
	taskid=taskSpawn(pTask->TaskName, pTask->Priority, 0, pTask->StackSize, (FUNCPTR)pTask->pTaskEntry,\
					(DWORD)(&pTask->Args[0]), 0, 0, 0, 0,0,0,0,0,0);
					//pTask->Args[0],pTask->Args[1],pTask->Args[2],pTask->Args[3],0,0,0,0,0,0);
					
	if(ERROR == (int)taskid)
	{
		if(CurTaskCount > 0)
			CurTaskCount--;
		
		TRACE("Create Task error.\n");
		//创建任务失败
		return ERR_FAILURE; 
	}

	pTask->Register[REG_TASK_ID] = taskid;	//存放操作系统分配的原始任务ID
	
	pTask->IsDeleted = 0;
	pTask->Counter = 0;
	pTask->wTimerNum = 0;

	//创建消息队列
	msgid = msgQCreate(16, sizeof(TMessage), MSG_Q_PRIORITY|MSG_Q_EVENTSEND_ERR_NOTIFY);
	if(msgid == NULL)
	{
		ReportMsg("Create task msg error.");
		return ERR_FAILURE;
	}
	pTask->hQueue = (void*)msgid;

	
    //在任务HASH表操作期间，禁止任务之间的抢占
//	mask = OS_IntLock();

	//根据任务计数值，将任务描述表指针存入
	TaskTable[CurTaskCount].pTask = pTask;

	//将任务描述表指针放到该任务的任务变量中，这样便于查找任务描述表
	taskVarAdd(taskid, (int *)pTask);
	
	//恢复任务抢占模式
//	OS_IntUnLock(mask);
	
	//返回成功
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 根据应用层任务ID取任务的任务描述表		                                            */
/* 输入: 任务应用层ID                      			                                        */
/* 输出: 无						                                                            */
/* 返回: 任务描述表指针或空指针						                                        */
/*------------------------------------------------------------------------------------------*/
OS_TaskDesp *VXGetTaskTab(OSINT8 id)
{
	int vxID, i;

	if(id == APP_TASKID_NULL)
		return (OS_TaskDesp *)NULL;
	
	if(id == SELF_TASK_ID)	//取当前任务的描述表
	{
		vxID = taskIdSelf();
		for(i=1; i<=(MAX_APP_TASK_NUM+1); i++)
		{
			if(TaskTable[i].pTask->Register[REG_TASK_ID] == vxID)	//检查操作系统ID是否一致
				return TaskTable[i].pTask;
		}
	}
	else
	{
		//检查任务ID的合法性
		ASSERT((id-TASK_HANDLE) <= MAX_APP_TASK_NUM);
		return TaskTable[id-TASK_HANDLE].pTask;
	}

	return (OS_TaskDesp *)NULL;
}

HANDLE VXGetCurrentTaskId()
{
	int vxID, i;
	
	vxID = taskIdSelf();
	for(i=1; i<=(MAX_APP_TASK_NUM+1); i++)
	{
		if(TaskTable[i].pTask->Register[REG_TASK_ID] == vxID)	//检查操作系统ID是否一致
			return TaskTable[i].pTask->dwTaskID;
	}

	return 0;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务保护，通过禁止任务调度实现                                                     */
/* 输入: 任务描述表指针                    			                                        */
/* 输出: 无						                                                            */
/* 返回: 成功或失败									                                        */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXProtect(OS_TaskDesp *pTask)
{
	int rtn;

	rtn = taskLock();
	if(rtn != OK)
	{
		ReportMsg("Task lock error errno=%d.\n",rtn);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 禁止任务保护，通过允许任务调度实现                                                 */
/* 输入: 任务描述表指针                    			                                        */
/* 输出: 无						                                                            */
/* 返回: 成功或失败									                                        */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXUnProtect(OS_TaskDesp *pTask)
{
	int rtn;

	rtn = taskUnlock();
	if(rtn != OK)
	{
		ReportMsg("Task unlock error errno=%d.\n",rtn);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务挂起操作						                                                */
/* 输入: 任务描述表指针                    			                                        */
/* 输出: 无						                                                            */
/* 返回: 成功或失败									                                        */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXTaskSuspend(OS_TaskDesp *pTask)
{
	int rtn;

	ASSERT(pTask != NULL);
	
	rtn = taskSuspend(pTask->Register[REG_TASK_ID]);
	if(rtn != OK)
	{
		ReportMsg("Task suspend error for errno=%d.\n",rtn);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务恢复操作						                                                */
/* 输入: 任务描述表指针                    			                                        */
/* 输出: 无						                                                            */
/* 返回: 成功或失败									                                        */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXTaskResume(OS_TaskDesp *pTask)
{
	int rtn;

	ASSERT(pTask != NULL);
	
	rtn = taskResume(pTask->Register[REG_TASK_ID]);
	if(rtn != OK)
	{
		ReportMsg("Task resume error for errno=%d.\n",rtn);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务恢复操作						                                                */
/* 输入: 任务描述表指针                    			                                        */
/* 输出: 无						                                                            */
/* 返回: 成功或失败									                                        */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXTaskKill(OS_TaskDesp *pTask)
{
	int rtn;

	ASSERT(pTask != NULL);
	
	rtn = taskDelete(pTask->Register[REG_TASK_ID]);
	if(rtn != OK)
	{
		ReportMsg("Task delete error for errno=%d.\n",rtn);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务延时,禁止在中断处理程序中调用	                                                */
/* 输入: 延时毫秒数	                    			                                        */
/* 输出: 无						                                                            */
/* 返回: 成功或失败									                                        */
/*------------------------------------------------------------------------------------------*/
void VXTaskDly(OSINT16 msecond)
{
	int ticks;

	if(msecond == 0)
		return;
	ticks = MS2TICK(msecond);
	
	taskDelay(ticks);
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 创建二进制信号量,提供给应用程序使用                                                      */
/* 输入: 信号量初始值                                                                       */
/* 输出: MutexId创建的信号量标识                                                            */
/* 返回: ERR_SUCCESS创建成功,其它创建失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXBSemCreate(BYTE *name,TMutexHandle *pmux,OSINT16 flag)
{
	SEM_ID  pBSem = NULL;
	if(flag == BSEM_FULL)
		pBSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	else if(flag == BSEM_EMPTY)
		pBSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	if(pBSem != NULL)
	{
		*pmux=(TMutexHandle*)pBSem;
		return ERR_SUCCESS;
	}
	else
	{
		TRACE("Create sem failure!");
		return ERR_SEM_CREATE_ERR;
	}	
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 创建计数信号量,提供给应用程序使用                                                      */
/* 输入: 信号量初始值                                                                       */
/* 输出: 计数信号量创建的信号量标识                                                            */
/* 返回: ERR_SUCCESS创建成功,其它创建失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXCSemCreate(BYTE *name,TCSemHandle *pCSem,OSINT16 flag)
{
	SEM_ID  psem = NULL;
	psem = semCCreate(SEM_Q_PRIORITY, 0);
	if(psem != NULL)
	{
		*pCSem=(TMutexHandle*)psem;
		return ERR_SUCCESS;
	}
	else
	{
		TRACE("Create sem failure!");
		return ERR_SEM_CREATE_ERR;
	}	
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 获取信号量,提供给应用程序使用                                                      */
/* 输入: MutexId信号量标识号,TimeOut等待时间(SEM_SUPEND悬挂等待，SEM_NOWAIT不等待，其它等待的毫秒数）    */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS获取成功,其它获取失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXSemGet(TMutexHandle MutexId, OSINT16 TimeOut)
{
	int ticks;
	STATUS rtn;
    
	//不等待方式
	if(TimeOut==SEM_NOWAIT)
	{
		rtn = semTake((SEM_ID)MutexId, NO_WAIT);
	    	if(rtn == OK )
	    		return ERR_SUCCESS;
    		else
		    	return ERR_GET_SEM_ERR;
	}

	//等待方式
	//转换毫秒数
	if(TimeOut != SEM_SUPEND)
    		ticks = MS2TICK(TimeOut);
	else
		ticks = WAIT_FOREVER;
	rtn = semTake((SEM_ID)MutexId,ticks);
	
	if(rtn == OK)
		return ERR_SUCCESS;
	else
	{
//		TRACE("Get Sem failure.\n");
		return ERR_GET_SEM_ERR;
	}
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 释放信号量,提供给应用程序使用                                                      */
/* 输入: MutexId信号量标识号                                                                */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS释放成功,其它释放失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXSemRelease(TMutexHandle MutexId)
{
	OSINT16 rtn;
	
	rtn = (OSINT16)semGive((SEM_ID)MutexId);
	if(rtn == OK)
		return ERR_SUCCESS;
	else
	{
//		TRACE("Release sem failure.\n");
		return ERR_RELEASE_SEM_ERR;
	}
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 获取系统动态RAM空间			                                                    */
/* 输入: 申请空间长度		                                                                */
/* 输出:                                                                                  	*/
/* 返回: 获取的动态RAM空间地址指针                                         					*/
/*------------------------------------------------------------------------------------------*/
void *VXMemAlloc(DWORD Length)
{
	if(Length == 0)
		return NULL;
	
	if((((DWORD)SysMallocPtr)-Length)<SYS_DRAM_BASE)
	{
		ReportMsg("malloc failure!\n");
		return NULL;
	}
////	OS_LockM(MEM_LOCK);

	SysMallocPtr=malloc(Length);
	if(SysMallocPtr == NULL)
	{
        LogError("VXMemAlloc",FILE_LINE,"get memory failed.");
		return NULL;
	}
	memset(SysMallocPtr,0,Length);
	
////	OS_UnLockM(MEM_LOCK);

	return SysMallocPtr;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 屏蔽intLockLevelSet所设定的级别一下的中断，操作系统首先将屏蔽的优先级设定为最高	*/
/* 输入: 无                                 						                        */
/* 输出:  							                                                        */
/* 返回: 系统当前屏蔽的中断优先级                                                           */
/*------------------------------------------------------------------------------------------*/
DWORD VXIntLock(void)
{
	return intLock();
}	

/*------------------------------------------------------------------------------------------*/
/* 功能: 消除被关闭的中断优先级																*/
/* 输入: IntLev中断级别(最高或其它)                                                         */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void VXIntUnLock(DWORD intLev)
{
	intUnlock(intLev);
}	

/*********************************************************************************************
 *
 * 功能描述     中断加载
 *
 * 参数说明      - ivect	: 输入 	中断向量
 *				 - isr		: 输入	中断服务接口函数
 *				 - arg		: 输入	传递给中断服务函数的参数
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
OSINT16 VXSetVector(DWORD ivect,TISR_ENTRY isr,DWORD arg)
{
	int rc;
	//参数合法性检查
	ASSERT(ivect >= MIN_I_VECT_NO);
	ASSERT(ivect <= MAX_I_VECT_NO);

	if( (ivect < MIN_I_VECT_NO) || (ivect > MAX_I_VECT_NO) )
	{
        LogError("VXSetVector",FILE_LINE,"the vector number %d error.", ivect);
		return ERR_FAILURE;
	}

	//中断加载
	rc = intConnect((VOIDFUNCPTR*)INUM_TO_IVEC(ivect), (VOIDFUNCPTR)isr, (int)arg);
	if(rc == ERROR)
	{
        LogError("VXSetVector",FILE_LINE,"connect the vector number %d error.", ivect);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     中断允许
 *
 * 参数说明      - ivect	: 输入 	中断向量
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
OSINT16 VXIntEnable(DWORD ivect)
{
	int rc;
	//参数合法性检查
	ASSERT(ivect >= MIN_I_VECT_NO);
	ASSERT(ivect <= MAX_I_VECT_NO);

	if( (ivect < MIN_I_VECT_NO) || (ivect > MAX_I_VECT_NO) )
	{
        LogError("VXIntEnable",FILE_LINE,"the vector number %d error.", ivect);
		return ERR_FAILURE;
	}

	//中断加载
	rc = intEnable(ivect);
	if(rc == ERROR)
	{
        LogError("VXIntEnable",FILE_LINE,"enable the vector number %d error.", ivect);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
	
}

/*********************************************************************************************
 *
 * 功能描述     中断禁止
 *
 * 参数说明      - ivect	: 输入 	中断向量
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
OSINT16 VXIntDisable(DWORD ivect)
{
	int rc;
	//参数合法性检查
	ASSERT(ivect >= MIN_I_VECT_NO);
	ASSERT(ivect <= MAX_I_VECT_NO);

	if( (ivect < MIN_I_VECT_NO) || (ivect > MAX_I_VECT_NO) )
	{
        LogError("VXIntDisable",FILE_LINE,"the vector number %d error.", ivect);
		return ERR_FAILURE;
	}

	//中断加载
	rc = intDisable(ivect);
	if(rc == ERROR)
	{
        LogError("VXIntDisable",FILE_LINE,"disable the vector number %d error.", ivect);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
	
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 设置任务定时器                                                                     */
/* 输入: pTask,sign 定时器标识符,interval 任务定时间隔,单位为毫秒     */                      
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS创建成功,其它创建失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
//注意对于特殊定时器，interval的时间单位为分钟
OSINT16 VXSetTimer(OS_TaskDesp *pTask,OSINT8 sign, OSINT16 interval, TSysTime *pBeginTime)
{
	OS_TimerDesp *pTimerNode = NULL;
	int i;

	//合法性检查
	ASSERT(pTask != NULL);
	ASSERT(interval != NULL);
	
	//创建普通定时器
	if(pBeginTime == NULL)
	{
		if(NormalTimerNum>=MAX_NORMAL_TIMER_NUM)
		{
			TRACE("set normal timer overflow");
			return ERR_INVALID_TIMER;
			
		}
		
		//检查本任务定时器个数是否有效
		if(pTask->wTimerNum>=MAX_TIMER)
		{
			ReportMsg("Create too many time for one task.");
			return ERR_INVALID_TIMER;
			
		}

		//检查该定时器是否存在，避免重复创建
		pTimerNode=pNormalTimerList;
		while(pTimerNode!=NULL)
		{
			if((pTimerNode->pTask == pTask) && (pTimerNode->Sign == sign))
				return ERR_INVALID_TIMER;
			pTimerNode = pTimerNode->pNextTimer;
		}

		//获取空闲定时器节点
		pTimerNode=(OS_TimerDesp *)VXMemAlloc(sizeof(os_timerdesp));
		ASSERT(pTimerNode != NULL);

		//填写定时器节点的相关信息
		pTimerNode->pTask=pTask;                     //任务的描述表指针
		pTimerNode->Sign=sign;                       //定时器标识号
		pTimerNode->CurTime= MS2TICK(interval);      //当前计数值
		pTimerNode->InitTime=MS2TICK(interval);      //初始计数值

		//在任务中记录定时器ID
		for(i=0; i<MAX_TIMER; i++)
		{
			if(pTask->dwTimeTab[i] == 0xffffffff)
			{
				pTask->dwTimeTab[i] = sign;
				pTimerNode->dwEvtBit = (EV_TIMERSTART<<i);
				pTask->wTimerNum++;
				break;
			}
		}
		if(i == MAX_TIMER)
		{
			ReportMsg("task %s settimer error.", pTask->TaskName);
			return ERR_INVALID_TIMER;
		}
		
		//禁止任务切换
		taskLock();

		//插入该节点到系统定时器链表
		pTimerNode->pNextTimer=pNormalTimerList;
		pNormalTimerList=pTimerNode;
		NormalTimerNum++;

		//恢复任务切换
		taskUnlock();
//		ReportMsg("set timer OK!");
	}

	return ERR_SUCCESS;

}

/*------------------------------------------------------------------------------------------*/
/* 功能: 删除任务定时器                                                                     */
/* 输入: tid任务号(0xff表示自身任务),sign 定时器标识符                                      */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS删除成功,其它删除失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXDelTimer(OS_TaskDesp *pTask,OSINT8 sign)
{
	OS_TimerDesp *pTimerNode;
	OS_TimerDesp **ppPrev;
	int i;
	
	//禁止任务切换
	taskLock();

	ppPrev=&pNormalTimerList;

	//查找普通定时器链表
	for(pTimerNode = *ppPrev; pTimerNode != NULL; pTimerNode = pTimerNode->pNextTimer)
	{
		//判断是否是所要删除的定时器
		if((pTimerNode->pTask==pTask)&&(pTimerNode->Sign==sign))
		{
			//把该定时器节点从系统定时器表中删除
	    	*ppPrev = pTimerNode->pNextTimer;
			NormalTimerNum--;
	
			for(i=0; i<MAX_TIMER; i++)
			{
				if(pTask->dwTimeTab[i] == sign)
				{
					pTask->dwTimeTab[i] = 0xffffffff;
					if(pTask->wTimerNum)
						pTask->wTimerNum--;
				}
			}

			//恢复任务切换
			taskUnlock();
	
	        //释放定时器节点到系统空闲定时器表
	        free((void*)pTimerNode);
			return ERR_SUCCESS;
	  	}
	    //不是要查找的定时器，搜索下一个定时器节点
	    ppPrev = &pTimerNode->pNextTimer;
		if(*ppPrev == NULL)
			break;
	}
    //恢复任务切换
	taskUnlock();

	//没有找到指定的定时器
	ReportMsg("Not find to be deleted timer!");
	return ERR_NO_FIND_TIMER;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 删除指定任务所有的定时器，任务被删除时使用                                             */
/* 输入: pTask                                                                              */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS删除成功,其它删除失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXDelAllTaskTimer(OS_TaskDesp *pTask)
{
	OS_TimerDesp *pTimerNode;
	OS_TimerDesp **ppPrev;
	
	//禁止任务切换
	taskLock();

	ppPrev=&pNormalTimerList;

	//查找普通定时器链表
	for(pTimerNode = *ppPrev;pTimerNode != NULL;pTimerNode = pTimerNode->pNextTimer)
	{
		//判断是否是所要删除的定时器
	    if(pTimerNode->pTask==pTask)
	 	{
	    	//把该定时器节点从系统定时器表中删除
	        *ppPrev = pTimerNode->pNextTimer;
			NormalTimerNum--;
	
	        //释放定时器节点到系统空闲定时器表
	        free((void*)pTimerNode);
		}
	    //不是要查找的定时器，搜索下一个定时器节点
	    ppPrev = &pTimerNode->pNextTimer;
	}

   	//恢复任务切换
	taskUnlock();

	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 向任务发送消息                                                                     */
/* 输入: pTask 目的任务描述表指针,msg 消息号，larg,harg,warg,parg分别为消息参数             */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS消息发送成功，其它发送失败                                              */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXPostMessage(OS_TaskDesp *pTask, OSINT16 msg, OSINT32 harg, OSINT32 warg,\
													OSINT32 parg, OSINT16 larg)
{
    TMessage message;
    STATUS rc;
	int errorno;
          
    if( pTask==NULL )
        return ERR_POST_SEM;

	//定时消息处理,其它事件消息处理
	if((msg == SM_TIMEOUT) || (msg == SM_RELAY_INT) ||(msg == SM_RELAYTIMER))
	{
		//发送定时事件，
		rc = eventSend(pTask->Register[REG_TASK_ID],harg);
		if(rc == ERROR)
		{
			ReportMsg("postmessage eventSend timeout error, errorno=%x, task name=%s.", errno, pTask->TaskName);
			return ERR_POST_MSG;
		}
		return ERR_SUCCESS;
	}
	
	//普通消息处理
	message.lParam = MAKELONG(msg,larg);
	message.hParam = harg;
	message.wParam = warg;
	message.pParam = parg;
	
	//采用非等待普通优先级方式发送消息,同时产生EV_DBNOTIFY为的事件
	rc = msgQSend((MSG_Q_ID)pTask->hQueue,(char*)&message, sizeof(message), NO_WAIT, MSG_PRI_NORMAL);
	if(rc == ERROR)
	{
		errorno = errno;
		ReportMsg("postmessage msgQSend error, errorno=%x, task name=%s", errorno, pTask->TaskName);
		return ERR_POST_MSG;
	}
	
	//
	rc = eventSend(pTask->Register[REG_TASK_ID],EV_DBNOTIFY);
	if(rc == ERROR)
	{
		ReportMsg("postmessage eventSend error, errorno=%x, task name=%s.", errno, pTask->TaskName);
		return ERR_POST_MSG;
	}
	
	return ERR_SUCCESS;
/*	
	TMessageWrap  *pMsg = NULL;
	int rtn;
	SEM_ID pSemId;

	//取空闲节点
	OS_GetEmptyMsg(pTask, &pMsg);

	if(pMsg == NULL)
	{
		TRACE("Get message free node error!");
		return ERR_GET_FREE_NODE;
	}

	pMsg->Msg.lParam = MAKELONG(WORD(msg),WORD(larg));
    pMsg->Msg.hParam = harg;
    pMsg->Msg.wParam = warg;
    pMsg->Msg.pParam = parg;

    //将消息节点放入消息队列
    OS_SetTaskMsg(pTask, pMsg);

	pSemId = (SEM_ID)pTask->Register[0];
	rtn = semGive(pSemId);
    if(rtn == ERROR)
    {
    	ReportMsg("Post the message sem error!");
    	return ERR_POST_SEM; 
    }	
//    else
//		ReportMsg("Post message OK, sem=0x%x....", (DWORD)pSemId);
	return ERR_SUCCESS;
*/	
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 从任务消息队列中取出一条消息,消息队列空时任务进入阻塞状态                          */
/* 输入: pTask msg mode(无效)		                                                    */
/* 输出: msg取得的消息指针                                                                  */
/* 返回: ERR_MESSAGE_OK 取消息正常, 没有请求的事件，ERR_MESSAGE_QUIT 任务退出		    */
/*                                                                                          */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXGetMessage(OS_TaskDesp *pTask,TMessage* msg, BYTE mode)
{
	int rtn, timeout;
    TMessageWrap *wmsg;
	STATUS rc;
	DWORD dwEvent;
    DWORD  status = ERR_MESSAGE_OK;
	
	
	if(mode == 0)
		timeout = WAIT_FOREVER;
	else
		timeout = NO_WAIT;

	//从任务消息链表中取出需要处理的消息
	OS_GetTaskMsg(pTask,&wmsg);
	
    //判断消息是否有效
    if(wmsg == NULL)
    {
        //任务消息链表中已经没有需要处理的消息,调用函数接收新事项
        //有新事项ev_receive函数返回;没有需处理的事项,则阻塞调用的任务
		rc = eventReceive(EV_MASKALL, EVENTS_WAIT_ANY, timeout, (UINT32*)&dwEvent);
		if(rc == ERROR)
		{
			ReportMsg("Getmessage eventReceive error, errorno=%x, task name=%s", errno, pTask->TaskName);
			return ERR_MESSAGE_NULL;
		}
        
        //判断接收到新的事项是否为空,一般情况任务执行到这里,dwEvent应该不等于零
        //因为没有事项ev_receive会阻塞调用任务;只有事项,任务才会执行到这里.但在
        //系统调试过程中,出现过dwEvent=0的现象.
        if(dwEvent == 0)
        {
            //接收到的事项为空,注册错误信息
            ReportMsg("GetMessage,event=0.....");

            //返回消息为空的错误代码
            return ERR_MESSAGE_NULL;
        }

        //判断接收到的事项是否为定时事项
        if(dwEvent & EV_TIMERMASK)
        {
            //是定时事项,调用定时事项处理函数
            VXEventTimer(pTask,dwEvent);
        }
        
        //判断接收的事项是否为消息接收标志
        if(dwEvent & EV_DBNOTIFY)
        {
            //接收的事项为消息接收标志,调用接收消息函数
            VXEvDBNotify(pTask );
        }

		//事件消息处理
		if(dwEvent & EV_EVENTMAPMASK)
		{
			VXEventMsg(pTask, dwEvent);
		}

        //从任务接收的消息链表中,取出队列头的一条消息
        OS_GetTaskMsg(pTask,&wmsg);
    }
	
    //判断取出的消息是否有效,正常情况下取出的消息不应该为空,但在调试过程中发现有消息空的现象
    if(wmsg != NULL)
    {
        //消息正常,把该消息拷贝到函数输出参数msg
        (*msg)=wmsg->Msg;
        
        //把消息不用的节点链接的空闲消息链
        OS_FreeMsg(pTask,wmsg);
    }
    else
    {
//		ReportMsg("Get Null message, task name=%s.", pTask->TaskName);
        //消息为空,返回消息空状态
        status = ERR_MESSAGE_NULL;
        
    }

    //判断当前收到的消息是否为请求任务退出消息
    if(LOWORD(msg->lParam) == SM_QUIT)
    {
        //收到任务退出消息,设置函数返回状态为请求消息退出状态
        status = ERR_MESSAGE_QUIT;

    }

    //返回实际结果
    return status;
		
	
/*	
	int rtn, type;
	TMessageWrap  *pMsg;
	OSINT16 status = ERR_MESSAGE_OK;
	SEM_ID pSemId;
	
	//获取消息以阻塞方式进行，不具有立即返回功能
	pSemId = (SEM_ID)pTask->Register[0];

	if(mode == 0)
		type = WAIT_FOREVER;
	else
		type = NO_WAIT;
	//从消息队列里取消息,悬挂等待方式
	rtn = semTake(pSemId, type);
	if(rtn == OK)//取消息成功
	{
		
		//获取消息节点
		OS_GetTaskMsg(pTask, &pMsg);

		if(pMsg == NULL)
		{
			ReportMsg("Task %d VX get null message!\n", pTask->Priority);
			return ERR_MESSAGE_NULL;
		}
		//取消息信息
		(*msg) = pMsg->Msg;

		//释放消息节点
		OS_FreeMsg(pTask, pMsg);

    	//判断当前收到的消息是否为请求任务退出消息
    	if(LOBYTE(msg->lParam) == SM_QUIT)
    	{
        	//收到任务退出消息,设置函数返回状态为请求消息退出状态
        	status = ERR_MESSAGE_QUIT;
    	}
    	
//		ReportMsg("Take message sem OK!");
		return status;
	}//if(err==OS_NO_ERR)
	else
	{
//		ReportMsg("Take message sem error!");
		return ERR_GET_SEM;
	}
*/		
}

void VXEventTimer(OS_TaskDesp *pTask,DWORD event)
{
    TMessageWrap *msg;
    DWORD bitm=EV_TIMERSTART;
	DWORD tmask = EV_TIMERMASK & event;

    //遍历任务定时器表,查找掩码与event掩码一致的定时器
    for(int i=0;i<MAX_TIMER;i++)
    {
        //判断当前定时器对应的事项掩码位,是否在任务收到的事项掩码位内
        if(event & bitm)
        {
			//清除处理已经处理的定时器标志
			tmask &= ~bitm;

			if(pTask->dwTimeTab[i] != 0xffffffff)
			{
	            //如果没有对应的定时函数，把该定时事项插入到任务的消息队列中
	            //从空闲表中分配空闲消息节点
	            OS_GetEmptyMsg(pTask,&msg);
	            
	            //判断分配是否成功
	            if(msg != NULL)
	            {
	                //消息分配成功,填充消息内容
	                msg->Msg.lParam = MAKELONG(SM_TIMEOUT,0);
	                msg->Msg.hParam = pTask->dwTimeTab[i];
	                msg->Msg.wParam = 0;
	                msg->Msg.pParam = 0;

	                //把该消息节点链入到任务消息链表中
	                OS_SetTaskMsg(pTask,msg);
	            }
	            else
	            {
	                //消息节点分配失败,登记错误信息
	                TRACE("EventTimer,df_GetEmptymsg error");
	                //   Exit();
	            }//end of else ---no message room is system error
			}
        }//end of if----get event map to timer id

		//判断是否还有末处理的定时器
		if(event & tmask)
			//判断下一个事项掩码
			bitm <<= 1;
		//所有定时事件都处理完毕，退出循环
		else break;
    }//end of for--polling all register for timer id
}

void VXEvDBNotify(OS_TaskDesp *pTask)
{
    TMessageWrap *wmsg;
    DWORD rc;
	TMessage msg;


    //读操作系统消息队列,直到队列空为止
    while(1)
    { 
        //调用pSOS系统函数,从任务队列中读pSOS消息
        rc = msgQReceive((MSG_Q_ID)pTask->hQueue,(char*)&msg, sizeof(TMessage), NO_WAIT);
        
        //判断任务消息队列是否已空
        if(rc == ERROR)
        {
            //任务消息队列为空,退出循环
            return;
        }
        
		//判断取消息函数操作是否成功
		if(rc != sizeof(TMessage))
		{
			ReportMsg("q_receive error ,rc=%d",rc);
			return;
		}

		//从空闲链表中取空闲消息节点,用于存储收到的OS消息
        OS_GetEmptyMsg(pTask,&wmsg);

        //判断取空闲消息节点是否成功
        if(wmsg==NULL)
        {
            //取空闲消息节点失败,空闲链中已经没有空闲消息节点,系统异常错误,重新初始化空闲链
            //禁止系统一切调度,屏蔽最高级中断

            //登记错误信息
            TRACE("EvDBNotify,No Free message");

            //系统热启动
            //SystemReset(WARM_RESET);
            return;
        }

        wmsg->Msg = msg;

        //任务队列未空,把读到的pSOS消息链入到任务消息队列表
        OS_SetTaskMsg(pTask,wmsg);
    }
}

void VXEventMsg(OS_TaskDesp *pTask, DWORD dwEvent)
{
    TMessageWrap *msg;

	//如果没有对应的定时函数，把该定时事项插入到任务的消息队列中
	//从空闲表中分配空闲消息节点
	OS_GetEmptyMsg(pTask,&msg);
		
	//判断分配是否成功
	if(msg != NULL)
	{
		//消息分配成功,填充消息内容
		if(dwEvent&EV_RELAYBIT)
			msg->Msg.lParam = MAKELONG(SM_RELAY_INT,0);
		else if(dwEvent&EV_RELAYTIMEBIT)
			msg->Msg.lParam = MAKELONG(SM_RELAYTIMER,0);
		msg->Msg.hParam = 0;
		msg->Msg.wParam = 0;
		msg->Msg.pParam = 0;

		//把该消息节点链入到任务消息链表中
		OS_SetTaskMsg(pTask,msg);
	}
	else
	{
		//消息节点分配失败,登记错误信息
		TRACE("EventTimer,df_GetEmptymsg error");
	}
	
}

#endif
