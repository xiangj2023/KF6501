#include "absos.h"
#include "platform.h"

//系统空闲消息节点内存控制块指针
extern dMemBlock *g_pMsgMemBlock;

//系统空闲消息链，当任务内的空闲消息链为空时，需要从系统空闲消息链中去空闲消息槽
STATIC TVMem *g_pMsgMem;
STATIC TMessageWrap *g_pMsgFreeList;
STATIC DWORD g_nEmptyMsg;

/*------------------------------------------------------------------------------------------*/
/* 功能: 可变内存分配，把分配的没一块内存用链表链接起来                                     */
/* 输入: pphead 可变内存区的链表首指针,unitnum 要分配的单元个数，unitlen要分配的每个单元的  */
/*       长度，分配的实际总长度为:unitnum*unitlen+sizeof(TVMem)                             */
/* 输出: pphead 改变后的可变长度内存区首指针                                                */
/* 返回: 应用程序可以使用的内存区首指针（移区管理表头）                                     */
/*------------------------------------------------------------------------------------------*/
void *OS_GetVMem(TVMem **pphead,WORD unitnum,WORD unitlen)
{
   	TVMem *pMem;
   	ASSERT(unitnum>0 && unitlen>0);

   	/*调用分配内存函数分配所需要的内存，分配的长度为：管理可变内存的指针+untinum*unitlen*/
   	pMem = (TVMem*)OS_MemAlloc( sizeof(TVMem)+unitnum*unitlen) ;
		
   	/*诊断分配的内存指针是否合法*/
   	ASSERT(pMem != NULL);
	if(pMem==NULL)
		return NULL;
	
   	/*把pMem插入到可变内存链表的表头*/
   	/*分配内存块的尾指针指向可变内存区首指针*/
   	INSERT_HEAD(*pphead,pMem);

   	/*调整内存指针，返回*/
   	return (void*)(pMem+1);
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 释放可变内存块                                                                     */
/* 输入: pHead 内存块首指针                                                                 */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_FreeVMem(TVMem *pHead)
{
    TVMem *pMem;
    while(pHead != NULL)
    {
        pMem = pHead->pNext;
        OS_MemFree(pHead);
        pHead = pMem;
    }
}


/*------------------------------------------------------------------------------------------*/
/* 功能: 在调式模式下,从任务消息队列头中取出一个消息,便于单步跟踪                           */
/* 输入: task 请求的任务描述表                                                              */
/* 输出: 消息指针的地址,=NULL 任务消息队列中没有消息                                        */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_GetTaskMsg(OS_TaskDesp * task,TMessageWrap **msg) 
{
	DWORD imask;

//	imask = OS_IntLock();  //当系数整定时不知道为何会产生异常中断
	OS_Protect(0);
	
    if(task->pFront == NULL) 
        *msg = NULL; 
    else 
	{
        *msg = task->pFront;
        task->pFront = task->pFront->pNext;
        if(task->pFront == NULL) 
            	task->pLast = NULL; 
    }

	//恢复系统调度
//    OS_IntUnLock(imask);
	OS_UnProtect(0);
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 把收到的消息链入任务消息队列                                                       */
/* 输入: task 任务描述表,msg收到的消息                                                      */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_SetTaskMsg(OS_TaskDesp *task,TMessageWrap* msg) 
{             
	DWORD imask;

	//imask = OS_IntLock();  
	OS_Protect(0);

    msg->pNext = NULL;
    if(task->pFront == NULL) 
    {
        ASSERT(task->pLast == NULL) ;
        task->pFront = task->pLast = msg; 
    } 
    else 
	{ 
        task->pLast->pNext = msg; 
        task->pLast = msg; 
        msg->pNext = NULL; 
    } 

		//恢复系统调度
		OS_UnProtect(0);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 申请空消息槽用于存储收到的消息                                                     */
/* 输入: task 任务描述表                                                                    */
/* 输出: msg 返回空消息槽指针,=NULL申请空消息失败                                           */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_GetEmptyMsg(OS_TaskDesp *task,TMessageWrap **msg) 
{
	DWORD imask;

    if(task->pFreeList != NULL) 
    { 
//		imask = OS_IntLock(); 
		OS_Protect(0);
		
        *(msg) = task->pFreeList; 
        task->pFreeList = task->pFreeList->pNext;   
        (*msg)->pNext = NULL;
		
		//恢复系统调度
//		OS_IntUnLock(imask);
		OS_UnProtect(0);
    } 
    else
    {
//        *(msg) =(TMessageWrap *)OS_GetNode(g_pMsgMemBlock); 
		*(msg) = OS_GetSysEmptyMsg();
 //   	ReportMsg("\r\n Task %d Get MSG Node Msg=%x ! !!!!!!!!\r\n",task->Priority,*(msg));
    }
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 释放消息槽                                                                         */
/* 输入: task任务描述表,msg消息槽指针                                                       */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_FreeMsg(OS_TaskDesp *task,TMessageWrap* msg) 
{
    DWORD imask;

//	imask = OS_IntLock();  
	OS_Protect(0);

    msg->pNext = task->pFreeList; 
    task->pFreeList = msg;

	//恢复系统调度
//    OS_IntUnLock(imask);
	OS_UnProtect(0);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 初始化管理空闲消息的数据对象                                                       */
/* 输入: 无                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void InitMsgWrap()
{
    //初始化空闲消息块内存链首指针
    g_pMsgMem = NULL;
    
    //初始化空闲消息节点链表 
    g_pMsgFreeList = NULL;
    
    //已分配的消息节点计数
    g_nEmptyMsg = 0;

#if(OS_PLATFORM ==  OS_WIN32)
	InitializeCriticalSection(&g_csMsgCommunicationSync);
#endif
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 取空闲消息节点，当任务预分配的空闲消息节点已分配完毕，可以调用系统的空闲消息节点   */
/* 输入: 无                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 空闲消息节点指针                                                                   */
/*------------------------------------------------------------------------------------------*/
#define MAX_MSG_WRAP (512/sizeof(TMessageWrap))
TMessageWrap *OS_GetSysEmptyMsg()
{
    TMessageWrap *pWrap;
    DWORD mask;
    
    //判断当前空闲Hash节点链是否为空
    if(g_pMsgFreeList == NULL)
    {
        //空闲节点链为空，分配一块MAX_MSG_WRAP空闲节点的内存，并链接成表
        //调用内存分配函数，分配内存多分配四个字节，用于把所有为空闲节点链分配的内存都链接起来
        pWrap = (TMessageWrap*)OS_GetVMem(&g_pMsgMem,MAX_MSG_WRAP,sizeof(TMessageWrap));
        
        //禁止任务调度
//        mask = OS_IntLock();
		OS_Protect(0);

        //把分配的MAX_MSG_WRAP个空闲TMessageWrap节点链接到一起
        for(int i=0;i<MAX_MSG_WRAP;i++,pWrap++)
            INSERT_HEAD(g_pMsgFreeList,pWrap);

        //恢复任务调度模式
//        OS_IntUnLock(mask);
		OS_UnProtect(0);
    }
    
    //hash空闲节点表不能为空
    ASSERT(g_pMsgFreeList != NULL);
    
   //禁止任务调度
//    mask = OS_IntLock();
   OS_Protect(0);

    //把hash空闲节点链表的首节点出链
    REMOVE_HEAD(g_pMsgFreeList,pWrap);
    
    //分配消息节点计数器加1
    g_nEmptyMsg++;
    
    //恢复任务调度模式
//    OS_IntUnLock(mask);
	OS_UnProtect(0);

    //返回分配的空闲节点指针
    return pWrap;
}


