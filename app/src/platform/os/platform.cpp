/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			platform.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           通用操作系统平台接口                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      实现对操作系统的封装，为上层应用提供统一的操作接口                          
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	OS_InitPlat		临界资源加锁操作
*      	OS_CreateTask	临界资源解锁操作                                                               
*      	OS_PostMessage  发送消息                                                                          
*      	OS_GetMessage   接收消息                                                                         
*      	OS_SetTimer     创建定时器                                                                       
*      	OS_DelTimer     删除定时器                                                                       
*      	OS_Protect      任务保护，禁止抢占                                                                      
*      	OS_UnProtect    解除任务保护                                                                        
*      	OS_Suspend      任务挂起                                                                      
*      	OS_Resume       任务回复                                                                     
*      	OS_Kill         任务删除                                                                   
*		OS_LockM		临界资源保护
*		OS_UnLockM		退出临界资源保护
*		OS_Exit			退出当前任务
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/3/21    初始创建                         
*                                                                                  
********************************************************************************************/

#include "platform.h"

#if(OS_PLATFORM == OS_NULL)
	#error "No define OS type!"
#endif

#if(OS_PLATFORM == OS_VXWORKS)

//系统退出代码
#if	(__DEBUG__ == YES)		
const STATIC STRING *g_szExitMsg[]={
    "normal exit!",            /*exit=0*/
    "memory overflow!",         /*exit=1*/
    "messgage list error",      /*exit=2*/
    "timer list error",         /*exit=3*/
    "RTOS disorder",            /*exit=4*/
    "recieve quit message",     /*exit=5*/
    "assert failure",           /*exit=6*/   
};
#endif

//系统空闲消息节点内存控制块指针
dMemBlock *g_pMsgMemBlock = NULL;

//定义当前正确创建的任务个数
OSINT8 CurTaskCount;

//任务描述表
TTask_Map TaskTable[MAX_APP_TASK_NUM+1];


#if(__DEBUG__ == YES)
const STATIC TMemTop *g_pMemLink=NULL;
#endif 


#define DEFINE_MUTEX(name) \
{#name,MUTEX_NOCREATE}

//系统信号量表
STATIC struct mutextable  g_mutexTable[]=
{
    DEFINE_MUTEX(STDOUT_LOCK),        
    DEFINE_MUTEX(MEM_LOCK),     
    DEFINE_MUTEX(SYSMSGNODE_LOCK),  
    DEFINE_MUTEX(DAI_LOCK),  
    DEFINE_MUTEX(SPI_LOCK),  
    DEFINE_MUTEX(IIC_LOCK),  
	DEFINE_MUTEX(FLASH_LOCK),  
    {NULL,NULL}                   
};

struct CSemtable  gCSemTable[]=
{
    DEFINE_MUTEX(RELAY_TASK),        
    DEFINE_MUTEX(RESV0),     
    DEFINE_MUTEX(RESV1),  
    {NULL,NULL}                   
};

/*------------------------------------------------------------------------------------------*/
/* 功能: 初始化通用监控平台                                                                 */
/* 输入:无                                                                                  */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_InitPlat(void)
{
	BYTE i;
	//初始化任务计数器
	CurTaskCount = 0;

	for(i=0;i<ArraySize(g_mutexTable);i++)
	{
		//创建平台使用的信号量
		if(OS_SemBCreate((BYTE *)g_mutexTable[i].pName,&g_mutexTable[i].hMutex, BSEM_FULL)!=ERR_SUCCESS)
		{
        		g_mutexTable[i].hMutex = MUTEX_NOCREATE;
//        		TRACE("Create platform sem error");
        	}	
	}

/*
	//创建系统计数信号量
	for(i=0;i<ArraySize(gCSemTable);i++)
	{
		//创建平台使用的信号量
		if(OS_SemCCreate((BYTE *)gCSemTable[i].pName,&gCSemTable[i].hCSem, 0)!=ERR_SUCCESS)
		{
			gCSemTable[i].hCSem = MUTEX_NOCREATE;
	//		TRACE("Create platform sem error");
		}	
	}
	
	//创建系统消息节点内存控制块
	InitMemBlock(&g_pMsgMemBlock,4, sizeof(TMessageWrap), SYSMSGNODE_LOCK);

	//检查消息节点内存控制块的有效性
	ASSERT(g_pMsgMemBlock != NULL);
	
	//创建系统Hash节点内存控制块
//	g_pHashMemBlock=InitMemBlock(  MAX_HASH_NODE, sizeof(THashNode), HASH_LOCK);
*/	
}	

/*------------------------------------------------------------------------------------------*/
/* 功能: 创建一个任务                                                                       */
/* 输入: name 任务名称，func 任务运行函数指针，pri 任务优先级，stack 任务堆栈大小,arg2任务第*/
/*       二个启动参数，arg3为弟三个启动参数，args4 为第四个启动参数。其中，任务的第一启动参 */
/*		 数为任务ID号，由系统提供															*/
/* 输出: 无                                                                                 */
/* 返回: =0任务创建失败，否则返回任务ID号                                                   */
/*------------------------------------------------------------------------------------------*/
DWORD OS_CreateTask(STRING *name,TTASK_ENTRY func,DWORD pri,DWORD stack,DWORD arg2,DWORD arg3,DWORD arg4)
{
	OSINT16 rtn;
	BYTE *addr = NULL;
	OS_TaskDesp *pTask;

	//对任务优先级、任务堆栈大小等进行合法性检查
	ASSERT((func!=NULL)&&(pri>=TASK_PRI_BASE)&&(stack>=MIN_TASK_STACK_LEN));
	
	addr=(BYTE *)OS_MemAlloc(sizeof(OS_TaskDesp));
	if(addr==(BYTE *)NULL)
	{
		TRACE("Task malloc error!");
		return ERR_FAILURE;//申请内存失败
	}	

	//检查任务优先级
	if(pri<TASK_PRI_BASE)
	{
		TRACE("Task prioity invalide!");
		return ERR_FAILURE;
	}

	if(stack<MIN_TASK_STACK_LEN)
	{
		TRACE("Task prioity invalide!");
		return ERR_FAILURE;
	}

	//填写任务描述表相关信息
	pTask=(OS_TaskDesp *)addr;

	//填写任务名字
	for(rtn=0;rtn<MAX_TASK_NAME_LEN;rtn++)
		pTask->TaskName[rtn]=name[rtn];
	
	pTask->Priority=(OSINT8)pri;//填写任务优先级	
	pTask->pTaskEntry=func;//填写函数指针
	pTask->StackSize = (OSINT16)stack;
	pTask->Args[1] = arg2;
	pTask->Args[2] = arg3;
	pTask->Args[3] = arg4;
	
	pTask->pFreeList = NULL;
	pTask->pFront = pTask->pLast = NULL;
	
	//如果每个任务都定义了空闲消息队列，把空闲消息都链接起来
	#if(MAX_PERTASKFREEMESSAGE > 0)
    	for(rtn=0;rtn<MAX_PERTASKFREEMESSAGE;rtn++)
    	{
        	pTask->amsgFree[rtn].pNext = pTask->pFreeList;
        	pTask->pFreeList = &pTask->amsgFree[rtn]; 
    	}
	#endif
	
	//创建任务
	rtn = RtosCreateTask(pTask);
	if(rtn != ERR_SUCCESS)
	{
		OS_MemFree((void*)addr);	//注意先释放资源
		TRACE("Create Task error.\n");
		return ERR_FAILURE;
	}
	
	//返回成功
	return pTask->dwTaskID;
////	return (OSINT16)pTask->Priority;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 创建并初始化内存节点控制块                                                      */
/* 输入: UnitNmbOfPerBlock每块的单元个数,UnitLen每个单元的长度,SemId平台级的信号量标识号                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 内存节点控制块指针                                                                                 */
/*------------------------------------------------------------------------------------------*/
void InitMemBlock(dMemBlock **p,  WORD UnitNumOfPerBlock,WORD UnitLen,DWORD SemId)
{
	//申请控制块内存
	*p=(dMemBlock*)OS_MemAlloc( sizeof(dMemBlock)) ;

	/*诊断分配的内存指针是否合法*/
    ASSERT(*p != NULL);
	
    //初始化空闲内存链首指针
    (*p)->pMem = NULL;
    
    //初始化空闲链表 
    (*p)->pFreeList = NULL;
    
    //已分配的节点(单元)计数
    (*p)->UsedUnitNum = 0;

	//初始化每块的单元数
	(*p)->UnitNumOfPerBlock = UnitNumOfPerBlock;

	//初始化每个单元的长度,至少为一个指针大小
	if(UnitLen<4)
		(*p)->UnitLen=4;
	else
	    (*p)->UnitLen=UnitLen;
    
	//使用的信号量的标识号
	(*p)->SemId=(OSINT8)SemId;

}

/*------------------------------------------------------------------------------------------*/
/* 功能: 向任务发送消息                                                                     */
/* 输入: tid 目的任务号,msg 消息号，larg,harg,warg,parg分别为消息参数                       */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS消息发送成功，其它发送失败                                              */
/*------------------------------------------------------------------------------------------*/
DWORD OS_PostMessage(DWORD tid, DWORD msg, DWORD harg, DWORD warg,\
														DWORD parg, DWORD larg)
{
	OS_TaskDesp *pTask;
	
	//查找任务描述表
    pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		ReportMsg("Not found the task to post message.\n");
		return ERR_NO_FIND_TASK;
	}

	return (DWORD)RtosPostMessage(pTask,(OSINT16)msg,harg,warg,parg,(OSINT16)larg);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 从任务消息队列中取出一条消息,消息队列空时任务进入阻塞状态                          */
/* 输入: pTask 请求的任务号,=0取自身任务消息;                                              */
/* 输出: msg取得的消息指针                                                                  */
/* 返回: ERR_SUCCESS 取消息正常,  其它失败                                                  */                                                                             
/*------------------------------------------------------------------------------------------*/
DWORD OS_GetMessage(DWORD tid,TMessage* msg, DWORD mode)
{
	OS_TaskDesp *pTask;
	
	//判断自身任务还是其它任务
    pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		ReportMsg("Not found the task to get message.\n");
		return ERR_NO_FIND_TASK;
	}
	
	return (DWORD)RtosGetMessage(pTask, msg, (OSINT8)mode);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 设置任务定时器                                                                     */
/* 输入: tid任务号(SELF_TASK_ID表示自身任务),sign 定时器标识符,interval 任务定时间隔,单位为毫秒     */                      
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS创建成功,其它创建失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
//注意对于特殊定时器，interval的时间单位为分钟
DWORD OS_SetTimer(DWORD tid,DWORD sign,DWORD interval, TSysTime *pBeginTime)
{
	OS_TaskDesp *pTask;

	//判断定时间隔的有效性
	if(0==MS2TICK(interval))
		return ERR_INVALID_TIMER;
	
	//取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);

	if(pTask==NULL)
	   return ERR_NO_FIND_TASK;

	return (DWORD)RtosSetTimer(pTask, (OSINT8)sign, (OSINT16)interval, pBeginTime);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 删除任务定时器                                                                     */
/* 输入: tid任务号,sign 定时器标识符                                      */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS删除成功,其它删除失败的返回代码                                         */
/*------------------------------------------------------------------------------------------*/
DWORD OS_DelTimer(DWORD tid,DWORD sign)
{
	OS_TaskDesp *pTask;
	
	//取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);

	if(pTask==NULL)
	   return ERR_NO_FIND_TASK;	
	
	return (DWORD)RtosDelTimer(pTask, (OSINT8)sign);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务进入保护状态，禁止其它任务抢占                                                 */
/* 输入: 任务ID号,id=0时表示正在运行的任务                                                  */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS函数调用成功，其它失败                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Protect(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;
/*
	//根据任务ID获取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}
*/	
	return (DWORD)RtosProtect(pTask);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务退出保护状态，允许正常调度                                                 */
/* 输入: 任务ID号,id=0时表示正在运行的任务                                                  */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS函数调用成功，其它失败                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_UnProtect(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;
/*
	//根据任务ID获取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}
*/	
	return (DWORD)RtosUnProtect(pTask);
}	

/*------------------------------------------------------------------------------------------*/
/* 功能: 指定任务进入挂起状态                                                               */
/* 输入: tid 请求的任务ID号                                                                 */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS函数调用成功，其它调用失败                                              */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Suspend(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;

	//根据任务ID获取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}
    return (DWORD)RtosSuspend(pTask);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 指定任务从挂起状态到正常运行模式                                                       */
/* 输入: tid 请求的任务ID号                                                                 */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS函数调用成功，其它调用失败                                              */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Resume(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;

	//根据任务ID获取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}

   	return (DWORD)RtosResume(pTask);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 删除指定任务                                                                       */
/* 输入: tid 请求删除的任务ID号 ，=0时 删除请求任务                                         */
/* 输出: 无                                                                                 */
/* 返回: ERR_SUCCESS删除成功，其它删除失败                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Kill(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;
	OSINT16 rc;

	//根据任务ID获取任务描述表
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}

	rc=RtosTaskKill(pTask);
    
	//释放任务描述表
	if(rc==ERR_SUCCESS)
		OS_MemFree(pTask);

	return (DWORD)rc;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务进入临界资源区,请求信号量.该函数主要提供给平台级函数使用                       */
/* 输入: id 临界区编号                                                                      */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
DWORD OS_LockM(DWORD id)
{
	OSINT16 rc;
	//引用的信号量是否越界
    if(id >= ArraySize(g_mutexTable))
    	return ERR_INVALID_SEM;

    //判断该信号量是否已经成功创建
    if(g_mutexTable[id].hMutex == MUTEX_NOCREATE)
   	{
//       	TRACE("Lock mutex sem not be created.\n");
		return ERR_SEM_NO_CREATE;
   	}
    	
   	//调用获取函数
	rc = OS_SemGet(g_mutexTable[id].hMutex, SEM_SUPEND);
//	if(rc != ERR_SUCCESS)
//		ReportMsg("Get mutex sem error.\n");
	return (DWORD)rc;
}	

/*------------------------------------------------------------------------------------------*/
/* 功能: 任务退出临界资源区,释放信号量.该函数主要提供给平台级函数使用                       */
/* 输入: id 临界区编号                                                                      */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
DWORD OS_UnLockM(DWORD id)
{
	OSINT16 rc;
	//引用的信号量是否越界
    if(id >= ArraySize(g_mutexTable))
       	return ERR_INVALID_SEM;
    
	//判断该信号量是否已经成功创建
    if(g_mutexTable[id].hMutex == MUTEX_NOCREATE)
   	{
//       	ReportMsg("Lock mutex sem not be created.\n");
		return ERR_SEM_NO_CREATE;
    } 
    
    //释放信号量
    rc = OS_SemRelease(g_mutexTable[id].hMutex);
//    if(rc != ERR_SUCCESS)
//    	ReportMsg("Release mutex sem error.\n");
    return (DWORD)rc;	
}	

/*------------------------------------------------------------------------------------------*/
/* 功能: 全局计数信号量获取操作                       									*/
/* 输入: id 计数信号量序号                                                                      */
/* 输出: 无                                                                                 */
/* 返回:                                                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemGet(DWORD id, DWORD mode)
{
	return (DWORD)OS_SemGet(gCSemTable[id].hCSem, mode);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 全局计数信号量释放操作                       									*/
/* 输入: id 计数信号量序号                                                                      */
/* 输出: 无                                                                                 */
/* 返回:                                                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemSet(DWORD id)
{
	return (DWORD)OS_SemRelease(gCSemTable[id].hCSem);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 退出当前运行的任务                                                                 */
/* 输入: code退出代码                                                                       */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_Exit(BYTE code)
{
   	//取当前任务的描述信息
   	OS_TaskDesp *pInfo = OS_GetTaskTab(SELF_TASK_ID);
   	STRING *pname="unkown task";
	if(pInfo == NULL)
		return;
	pname = (STRING*)pInfo->TaskName;
        
   	//错误代码为系统已经定义输出相应的错误信息，否则输出错误代码
#if	(__DEBUG__ == YES)		
   	if(code < ArraySize(g_szExitMsg))
       	ReportMsg("Task %s has exit for %s",pname,g_szExitMsg[code]);
   	else 
        ReportMsg("Task %s has exit with code = 0x%x",pname,code);
#endif
   	//任务睡眠500毫秒，以保证异常信息能正常输出
   	OS_Sleep(500);
   	//置任务退出标志
   	pInfo->IsDeleted = 1;
		
   	//调用RTOS函数退出当前任务
   	RtosSuspend(pInfo);
}

/*********************************************************************************************
 *
 * 功能描述     当前任务注册,清除当前任务运行计数
 *
 * 参数说明      
 * 			     
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD OS_TaskRegister(void)
{
    OS_TaskDesp* pTask = OS_GetTaskTab(SELF_TASK_ID);
    
    if( pTask==NULL )
    {
        LogError("OS_TaskRegister",FILE_LINE,"get task id error.");
        return ERR_NO_FIND_TASK;
    }
    
    pTask->Counter=0;
    return ERR_SUCCESS;        
}

/*********************************************************************************************
 *
 * 功能描述     根据不同操作系统类型，将任务入口函数参数转为统一指针形式
 *
 * 参数说明      - arg	: 输入 	任务入口函数参数
 *
 * 返回代码
 *                任务入口参数指针
 *
 * 其它说明：          
 *
**********************************************************************************************/
/*
DWORD *GetTaskArg(TASKARG arg)
{
	#if(OS_PLATFORM == OS_UCOSII)
		return (DWORD)arg;
	#else
		return (DWORD*)arg;
	#endif
}
*/
/*------------------------------------------------------------------------------------------*/
/* 功能: 从指定的内存控制块取空闲节点                                                      */
/* 输入: pMemBlock内存控制块指针                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 获取的节点指针                                                                   */
/*------------------------------------------------------------------------------------------*/
void *OS_GetNode(dMemBlock *pMemBlock)
{
	WORD i;
    TVMem *pNode;
	BYTE *pTemp;

	//用平台级信号量保护
	OS_LockM(pMemBlock->SemId);
    
    //判断当前空闲节点链是否为空
    if(pMemBlock->pFreeList == NULL)
    {
       	//空闲节点链为空，分配一块空闲节点的内存，并链接成表
       	//调用内存分配函数，分配内存多分配四个字节，用于把所有为空闲节点链分配的内存都链接起来
       	pNode = (TVMem*)OS_GetVMem(&(pMemBlock->pMem),pMemBlock->UnitNumOfPerBlock,pMemBlock->UnitLen);
    	if(pNode==NULL)
			return NULL;
			
       	//把分配的空闲节点链接到一起
       	for(i=0; i<pMemBlock->UnitNumOfPerBlock; i++)
		{
        	//插入到链表头
            INSERT_HEAD(pMemBlock->pFreeList,pNode);

			//调整pNode
			pTemp=(BYTE *)pNode;
			pTemp+=pMemBlock->UnitLen;
			pNode=(TVMem*)pTemp;
		}
		
    	ReportMsg("\r\n MASSAGE BLOCK LINK MALLOC, task id=%x ! !!!!!!!!\r\n", RtosGetCurrentTaskId());
    }
    
    //空闲节点表不能为空
    ASSERT(pMemBlock->pFreeList != NULL);

    //把空闲节点链表的首节点出链
    REMOVE_HEAD(pMemBlock->pFreeList,pNode);
    
    //已分配节点计数器加1
    pMemBlock->UsedUnitNum++;

	//释放平台级信号量
	OS_UnLockM(pMemBlock->SemId);
    
    //返回分配的空闲节点指针
    return (void *)pNode;
}



#if(__DEBUG_MEMORY_T == YES)
#define MEM_DATA(mem) ((BYTE *)((TMemTop *)mem + 1))
#define MEM_HEAD(mem) (((TMemTop *)mem)-1)
BOOL CheckMem(BYTE* pbuf,BYTE data,DWORD len)
{
    BOOL IsOK = TRUE;
    for(int i=0;i<len;i++,pbuf++)
    {
        if(*pbuf != data)
        {
            ReportMsg("Memory check error ");
            IsOK = FALSE;
        }
    }
    return IsOK;
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 在内存调试模式下的分配内存(系统在后台可以检查内存的合法性)                         */
/* 输入: size 请求分配的长度,file 当前申请分配内存的文件名称,line 调用该函数的行号          */
/* 输出: 无                                                                                 */
/* 返回: =NULL分配失败,其它实际分配的内存指针                                               */
/*------------------------------------------------------------------------------------------*/
void* OS_MemAllocDebug(int size,char *file,DWORD line)
{
    TMemTop *pMemTop;
	OS_TaskDesp *pTask = NULL;

    if(size == 0)
        return (void *)NULL;

    //把需要分配的字节长度长字对齐
    size = ALIGN(size,ALIGNMENT);
    
    //调用内存分配函数，分配相应长度的内存，其中内存头和内存间隙用于检查是否有内存溢出
    //sizeof(TMemTop)/*内存调式头*/+size/*请求的长度*/+MAX_GAPLEN/*内存间隙*/
    pMemTop = (TMemTop*)OS_MemAlloc(size+sizeof(TMemTop)+sizeof(BYTE)*MAX_GAPLEN);
    
    //填充内存头
    pMemTop->szFileName = file;  //文件名称指针
    pMemTop->nLine = line;      //请求内存分配函数所在的行号
    pMemTop->nSize = size;      //请求分配的长度
    pTask = OS_GetTaskTab(SELF_TASK_ID)->dwTaskID;   //取请求分配的任务ID号
    pMemTop->hTask = pTask;
	if(pTask != NULL)
		pTask->dwAllocMemLen += size;

    //在分配内存的首端填充特殊标识字符
    OS_MemSet(pMemTop->aGap,MEM_GAP_FILL,MAX_GAPLEN);
    //在分配内存的末端填充特殊标识字符
    OS_MemSet(MEM_DATA(pMemTop)+size,MEM_GAP_FILL,MAX_GAPLEN);

//    //进入临界区保护
//    OS_LockM(MEM_LOCK);
    //把分配的内存块插入到已分配的内存链中
    INSERT_HEAD(g_pMemLink,pMemTop);
//    OS_UnLockM(MEM_LOCK);

    //返回地址（分配的地址+内存头）
    return (void *)MEM_DATA(pMemTop);
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 内存调式模式下的释放内存函数,该函数同时校验内存的合法性                            */
/* 输入: buf释放的内存区指针                                                                */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_MemFreeDebug(void *pbuf)
{
	OS_TaskDesp *pTask = NULL;

    TMemTop *pMemTop = MEM_HEAD(pbuf);
    if(pbuf == NULL)
        return;
    //检查内存区头是否被破坏
    if(!CheckMem(pMemTop->aGap,MAX_GAPLEN,MEM_GAP_FILL))
    {
        ReportMsg("Memory DAMAGED! ");
    }

    //检查内存区未是否被破坏
    if(!CheckMem((BYTE*)pbuf+pMemTop->nSize,MAX_GAPLEN,MEM_GAP_FILL))
    {
        ReportMsg("Memory DAMAGED! ");
    }

	/*调整任务表中分配内存总长度记录*/
    pTask = OS_GetTaskTab(SELF_TASK_ID)->dwTaskID;   //取请求分配的任务ID号
	if(pTask != NULL)
		pTask->dwAllocMemLen -= pMemTop->nSize;
	
    //进入临界区保护
//    OS_LockM(MEM_LOCK);
    //把分配的内存块插入到已分配的内存链中
    REMOVE_HEAD(g_pMemLink,pMemTop);
//    OS_UnLockM(MEM_LOCK);

    //调用内存释放函数释放指定内存区
    OS_MemFree(pMemTop);
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 判断内存区的是否被破坏                                                             */
/* 输入: pbuf 内存区指针,该内存区必须是调用  MemAlloc分配返回的地址                         */
/* 输出: 无                                                                                 */
/* 返回: TRUE 地址未破坏,FALSE地址被破坏                                                    */
/*------------------------------------------------------------------------------------------*/
BOOL IsAddressValid(char *pbuf)
{
    TMemTop *pMemTop;
    pMemTop = (TMemTop *)pbuf;
    //检查内存区头是否被破坏
    if(!CheckMem(pMemTop->aGap,MAX_GAPLEN,MEM_GAP_FILL))
    {
        return FALSE;
    }

    //检查内存区未是否被破坏
    if(!CheckMem((BYTE*)pbuf+pMemTop->nSize,MAX_GAPLEN,MEM_GAP_FILL))
    {
        return FALSE;
    }
	return TRUE;
}
#undef MEM_DATA
#undef MEM_HEAD
#endif


/*------------------------------------------------------------------------------------------*/
/* 功能: 系统复位                                                             */
/* 输入: 复位类型                         */
/* 输出: 无                                                                                 */
/* 返回: 无                                                    */
/*------------------------------------------------------------------------------------------*/
extern HANDLE	g_hWdogTaskID;
void SystemReset(DWORD type)
{
	//通过控制看门狗刷新实现复位，调用reboot无法实现
	OS_PostMessage(g_hWdogTaskID, SM_SYS_RESET, 0, 0, 0, 0);
//	OS_Sleep(100);
//	reboot(BOOT_QUICK_AUTOBOOT);
}


#endif			
