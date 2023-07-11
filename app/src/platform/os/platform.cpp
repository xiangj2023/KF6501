/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			platform.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ͨ�ò���ϵͳƽ̨�ӿ�                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ʵ�ֶԲ���ϵͳ�ķ�װ��Ϊ�ϲ�Ӧ���ṩͳһ�Ĳ����ӿ�                          
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	OS_InitPlat		�ٽ���Դ��������
*      	OS_CreateTask	�ٽ���Դ��������                                                               
*      	OS_PostMessage  ������Ϣ                                                                          
*      	OS_GetMessage   ������Ϣ                                                                         
*      	OS_SetTimer     ������ʱ��                                                                       
*      	OS_DelTimer     ɾ����ʱ��                                                                       
*      	OS_Protect      ���񱣻�����ֹ��ռ                                                                      
*      	OS_UnProtect    ������񱣻�                                                                        
*      	OS_Suspend      �������                                                                      
*      	OS_Resume       ����ظ�                                                                     
*      	OS_Kill         ����ɾ��                                                                   
*		OS_LockM		�ٽ���Դ����
*		OS_UnLockM		�˳��ٽ���Դ����
*		OS_Exit			�˳���ǰ����
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/3/21    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "platform.h"

#if(OS_PLATFORM == OS_NULL)
	#error "No define OS type!"
#endif

#if(OS_PLATFORM == OS_VXWORKS)

//ϵͳ�˳�����
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

//ϵͳ������Ϣ�ڵ��ڴ���ƿ�ָ��
dMemBlock *g_pMsgMemBlock = NULL;

//���嵱ǰ��ȷ�������������
OSINT8 CurTaskCount;

//����������
TTask_Map TaskTable[MAX_APP_TASK_NUM+1];


#if(__DEBUG__ == YES)
const STATIC TMemTop *g_pMemLink=NULL;
#endif 


#define DEFINE_MUTEX(name) \
{#name,MUTEX_NOCREATE}

//ϵͳ�ź�����
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
/* ����: ��ʼ��ͨ�ü��ƽ̨                                                                 */
/* ����:��                                                                                  */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_InitPlat(void)
{
	BYTE i;
	//��ʼ�����������
	CurTaskCount = 0;

	for(i=0;i<ArraySize(g_mutexTable);i++)
	{
		//����ƽ̨ʹ�õ��ź���
		if(OS_SemBCreate((BYTE *)g_mutexTable[i].pName,&g_mutexTable[i].hMutex, BSEM_FULL)!=ERR_SUCCESS)
		{
        		g_mutexTable[i].hMutex = MUTEX_NOCREATE;
//        		TRACE("Create platform sem error");
        	}	
	}

/*
	//����ϵͳ�����ź���
	for(i=0;i<ArraySize(gCSemTable);i++)
	{
		//����ƽ̨ʹ�õ��ź���
		if(OS_SemCCreate((BYTE *)gCSemTable[i].pName,&gCSemTable[i].hCSem, 0)!=ERR_SUCCESS)
		{
			gCSemTable[i].hCSem = MUTEX_NOCREATE;
	//		TRACE("Create platform sem error");
		}	
	}
	
	//����ϵͳ��Ϣ�ڵ��ڴ���ƿ�
	InitMemBlock(&g_pMsgMemBlock,4, sizeof(TMessageWrap), SYSMSGNODE_LOCK);

	//�����Ϣ�ڵ��ڴ���ƿ����Ч��
	ASSERT(g_pMsgMemBlock != NULL);
	
	//����ϵͳHash�ڵ��ڴ���ƿ�
//	g_pHashMemBlock=InitMemBlock(  MAX_HASH_NODE, sizeof(THashNode), HASH_LOCK);
*/	
}	

/*------------------------------------------------------------------------------------------*/
/* ����: ����һ������                                                                       */
/* ����: name �������ƣ�func �������к���ָ�룬pri �������ȼ���stack �����ջ��С,arg2�����*/
/*       ��������������arg3Ϊ����������������args4 Ϊ���ĸ��������������У�����ĵ�һ������ */
/*		 ��Ϊ����ID�ţ���ϵͳ�ṩ															*/
/* ���: ��                                                                                 */
/* ����: =0���񴴽�ʧ�ܣ����򷵻�����ID��                                                   */
/*------------------------------------------------------------------------------------------*/
DWORD OS_CreateTask(STRING *name,TTASK_ENTRY func,DWORD pri,DWORD stack,DWORD arg2,DWORD arg3,DWORD arg4)
{
	OSINT16 rtn;
	BYTE *addr = NULL;
	OS_TaskDesp *pTask;

	//���������ȼ��������ջ��С�Ƚ��кϷ��Լ��
	ASSERT((func!=NULL)&&(pri>=TASK_PRI_BASE)&&(stack>=MIN_TASK_STACK_LEN));
	
	addr=(BYTE *)OS_MemAlloc(sizeof(OS_TaskDesp));
	if(addr==(BYTE *)NULL)
	{
		TRACE("Task malloc error!");
		return ERR_FAILURE;//�����ڴ�ʧ��
	}	

	//����������ȼ�
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

	//��д���������������Ϣ
	pTask=(OS_TaskDesp *)addr;

	//��д��������
	for(rtn=0;rtn<MAX_TASK_NAME_LEN;rtn++)
		pTask->TaskName[rtn]=name[rtn];
	
	pTask->Priority=(OSINT8)pri;//��д�������ȼ�	
	pTask->pTaskEntry=func;//��д����ָ��
	pTask->StackSize = (OSINT16)stack;
	pTask->Args[1] = arg2;
	pTask->Args[2] = arg3;
	pTask->Args[3] = arg4;
	
	pTask->pFreeList = NULL;
	pTask->pFront = pTask->pLast = NULL;
	
	//���ÿ�����񶼶����˿�����Ϣ���У��ѿ�����Ϣ����������
	#if(MAX_PERTASKFREEMESSAGE > 0)
    	for(rtn=0;rtn<MAX_PERTASKFREEMESSAGE;rtn++)
    	{
        	pTask->amsgFree[rtn].pNext = pTask->pFreeList;
        	pTask->pFreeList = &pTask->amsgFree[rtn]; 
    	}
	#endif
	
	//��������
	rtn = RtosCreateTask(pTask);
	if(rtn != ERR_SUCCESS)
	{
		OS_MemFree((void*)addr);	//ע�����ͷ���Դ
		TRACE("Create Task error.\n");
		return ERR_FAILURE;
	}
	
	//���سɹ�
	return pTask->dwTaskID;
////	return (OSINT16)pTask->Priority;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��������ʼ���ڴ�ڵ���ƿ�                                                      */
/* ����: UnitNmbOfPerBlockÿ��ĵ�Ԫ����,UnitLenÿ����Ԫ�ĳ���,SemIdƽ̨�����ź�����ʶ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: �ڴ�ڵ���ƿ�ָ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void InitMemBlock(dMemBlock **p,  WORD UnitNumOfPerBlock,WORD UnitLen,DWORD SemId)
{
	//������ƿ��ڴ�
	*p=(dMemBlock*)OS_MemAlloc( sizeof(dMemBlock)) ;

	/*��Ϸ�����ڴ�ָ���Ƿ�Ϸ�*/
    ASSERT(*p != NULL);
	
    //��ʼ�������ڴ�����ָ��
    (*p)->pMem = NULL;
    
    //��ʼ���������� 
    (*p)->pFreeList = NULL;
    
    //�ѷ���Ľڵ�(��Ԫ)����
    (*p)->UsedUnitNum = 0;

	//��ʼ��ÿ��ĵ�Ԫ��
	(*p)->UnitNumOfPerBlock = UnitNumOfPerBlock;

	//��ʼ��ÿ����Ԫ�ĳ���,����Ϊһ��ָ���С
	if(UnitLen<4)
		(*p)->UnitLen=4;
	else
	    (*p)->UnitLen=UnitLen;
    
	//ʹ�õ��ź����ı�ʶ��
	(*p)->SemId=(OSINT8)SemId;

}

/*------------------------------------------------------------------------------------------*/
/* ����: ����������Ϣ                                                                     */
/* ����: tid Ŀ�������,msg ��Ϣ�ţ�larg,harg,warg,parg�ֱ�Ϊ��Ϣ����                       */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS��Ϣ���ͳɹ�����������ʧ��                                              */
/*------------------------------------------------------------------------------------------*/
DWORD OS_PostMessage(DWORD tid, DWORD msg, DWORD harg, DWORD warg,\
														DWORD parg, DWORD larg)
{
	OS_TaskDesp *pTask;
	
	//��������������
    pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		ReportMsg("Not found the task to post message.\n");
		return ERR_NO_FIND_TASK;
	}

	return (DWORD)RtosPostMessage(pTask,(OSINT16)msg,harg,warg,parg,(OSINT16)larg);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��������Ϣ������ȡ��һ����Ϣ,��Ϣ���п�ʱ�����������״̬                          */
/* ����: pTask ����������,=0ȡ����������Ϣ;                                              */
/* ���: msgȡ�õ���Ϣָ��                                                                  */
/* ����: ERR_SUCCESS ȡ��Ϣ����,  ����ʧ��                                                  */                                                                             
/*------------------------------------------------------------------------------------------*/
DWORD OS_GetMessage(DWORD tid,TMessage* msg, DWORD mode)
{
	OS_TaskDesp *pTask;
	
	//�ж�������������������
    pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		ReportMsg("Not found the task to get message.\n");
		return ERR_NO_FIND_TASK;
	}
	
	return (DWORD)RtosGetMessage(pTask, msg, (OSINT8)mode);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��������ʱ��                                                                     */
/* ����: tid�����(SELF_TASK_ID��ʾ��������),sign ��ʱ����ʶ��,interval ����ʱ���,��λΪ����     */                      
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�����ɹ�,��������ʧ�ܵķ��ش���                                         */
/*------------------------------------------------------------------------------------------*/
//ע��������ⶨʱ����interval��ʱ�䵥λΪ����
DWORD OS_SetTimer(DWORD tid,DWORD sign,DWORD interval, TSysTime *pBeginTime)
{
	OS_TaskDesp *pTask;

	//�ж϶�ʱ�������Ч��
	if(0==MS2TICK(interval))
		return ERR_INVALID_TIMER;
	
	//ȡ����������
	pTask = OS_GetTaskTab((OSINT8)tid);

	if(pTask==NULL)
	   return ERR_NO_FIND_TASK;

	return (DWORD)RtosSetTimer(pTask, (OSINT8)sign, (OSINT16)interval, pBeginTime);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ɾ������ʱ��                                                                     */
/* ����: tid�����,sign ��ʱ����ʶ��                                      */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESSɾ���ɹ�,����ɾ��ʧ�ܵķ��ش���                                         */
/*------------------------------------------------------------------------------------------*/
DWORD OS_DelTimer(DWORD tid,DWORD sign)
{
	OS_TaskDesp *pTask;
	
	//ȡ����������
	pTask = OS_GetTaskTab((OSINT8)tid);

	if(pTask==NULL)
	   return ERR_NO_FIND_TASK;	
	
	return (DWORD)RtosDelTimer(pTask, (OSINT8)sign);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ������뱣��״̬����ֹ����������ռ                                                 */
/* ����: ����ID��,id=0ʱ��ʾ�������е�����                                                  */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�������óɹ�������ʧ��                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Protect(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;
/*
	//��������ID��ȡ����������
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
/* ����: �����˳�����״̬��������������                                                 */
/* ����: ����ID��,id=0ʱ��ʾ�������е�����                                                  */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�������óɹ�������ʧ��                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_UnProtect(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;
/*
	//��������ID��ȡ����������
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
/* ����: ָ������������״̬                                                               */
/* ����: tid ���������ID��                                                                 */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�������óɹ�����������ʧ��                                              */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Suspend(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;

	//��������ID��ȡ����������
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}
    return (DWORD)RtosSuspend(pTask);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ָ������ӹ���״̬����������ģʽ                                                       */
/* ����: tid ���������ID��                                                                 */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�������óɹ�����������ʧ��                                              */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Resume(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;

	//��������ID��ȡ����������
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}

   	return (DWORD)RtosResume(pTask);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ɾ��ָ������                                                                       */
/* ����: tid ����ɾ��������ID�� ��=0ʱ ɾ����������                                         */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESSɾ���ɹ�������ɾ��ʧ��                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_Kill(DWORD tid)
{
	OS_TaskDesp *pTask = NULL;
	OSINT16 rc;

	//��������ID��ȡ����������
	pTask = OS_GetTaskTab((OSINT8)tid);
	if(pTask==NULL)
	{
		TRACE("Get task table error.\n");
		return ERR_NO_FIND_TASK;
	}

	rc=RtosTaskKill(pTask);
    
	//�ͷ�����������
	if(rc==ERR_SUCCESS)
		OS_MemFree(pTask);

	return (DWORD)rc;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��������ٽ���Դ��,�����ź���.�ú�����Ҫ�ṩ��ƽ̨������ʹ��                       */
/* ����: id �ٽ������                                                                      */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
DWORD OS_LockM(DWORD id)
{
	OSINT16 rc;
	//���õ��ź����Ƿ�Խ��
    if(id >= ArraySize(g_mutexTable))
    	return ERR_INVALID_SEM;

    //�жϸ��ź����Ƿ��Ѿ��ɹ�����
    if(g_mutexTable[id].hMutex == MUTEX_NOCREATE)
   	{
//       	TRACE("Lock mutex sem not be created.\n");
		return ERR_SEM_NO_CREATE;
   	}
    	
   	//���û�ȡ����
	rc = OS_SemGet(g_mutexTable[id].hMutex, SEM_SUPEND);
//	if(rc != ERR_SUCCESS)
//		ReportMsg("Get mutex sem error.\n");
	return (DWORD)rc;
}	

/*------------------------------------------------------------------------------------------*/
/* ����: �����˳��ٽ���Դ��,�ͷ��ź���.�ú�����Ҫ�ṩ��ƽ̨������ʹ��                       */
/* ����: id �ٽ������                                                                      */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
DWORD OS_UnLockM(DWORD id)
{
	OSINT16 rc;
	//���õ��ź����Ƿ�Խ��
    if(id >= ArraySize(g_mutexTable))
       	return ERR_INVALID_SEM;
    
	//�жϸ��ź����Ƿ��Ѿ��ɹ�����
    if(g_mutexTable[id].hMutex == MUTEX_NOCREATE)
   	{
//       	ReportMsg("Lock mutex sem not be created.\n");
		return ERR_SEM_NO_CREATE;
    } 
    
    //�ͷ��ź���
    rc = OS_SemRelease(g_mutexTable[id].hMutex);
//    if(rc != ERR_SUCCESS)
//    	ReportMsg("Release mutex sem error.\n");
    return (DWORD)rc;	
}	

/*------------------------------------------------------------------------------------------*/
/* ����: ȫ�ּ����ź�����ȡ����                       									*/
/* ����: id �����ź������                                                                      */
/* ���: ��                                                                                 */
/* ����:                                                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemGet(DWORD id, DWORD mode)
{
	return (DWORD)OS_SemGet(gCSemTable[id].hCSem, mode);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ȫ�ּ����ź����ͷŲ���                       									*/
/* ����: id �����ź������                                                                      */
/* ���: ��                                                                                 */
/* ����:                                                                                  */
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemSet(DWORD id)
{
	return (DWORD)OS_SemRelease(gCSemTable[id].hCSem);
}

/*------------------------------------------------------------------------------------------*/
/* ����: �˳���ǰ���е�����                                                                 */
/* ����: code�˳�����                                                                       */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_Exit(BYTE code)
{
   	//ȡ��ǰ�����������Ϣ
   	OS_TaskDesp *pInfo = OS_GetTaskTab(SELF_TASK_ID);
   	STRING *pname="unkown task";
	if(pInfo == NULL)
		return;
	pname = (STRING*)pInfo->TaskName;
        
   	//�������Ϊϵͳ�Ѿ����������Ӧ�Ĵ�����Ϣ����������������
#if	(__DEBUG__ == YES)		
   	if(code < ArraySize(g_szExitMsg))
       	ReportMsg("Task %s has exit for %s",pname,g_szExitMsg[code]);
   	else 
        ReportMsg("Task %s has exit with code = 0x%x",pname,code);
#endif
   	//����˯��500���룬�Ա�֤�쳣��Ϣ���������
   	OS_Sleep(500);
   	//�������˳���־
   	pInfo->IsDeleted = 1;
		
   	//����RTOS�����˳���ǰ����
   	RtosSuspend(pInfo);
}

/*********************************************************************************************
 *
 * ��������     ��ǰ����ע��,�����ǰ�������м���
 *
 * ����˵��      
 * 			     
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
 * ��������     ���ݲ�ͬ����ϵͳ���ͣ���������ں�������תΪͳһָ����ʽ
 *
 * ����˵��      - arg	: ���� 	������ں�������
 *
 * ���ش���
 *                ������ڲ���ָ��
 *
 * ����˵����          
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
/* ����: ��ָ�����ڴ���ƿ�ȡ���нڵ�                                                      */
/* ����: pMemBlock�ڴ���ƿ�ָ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: ��ȡ�Ľڵ�ָ��                                                                   */
/*------------------------------------------------------------------------------------------*/
void *OS_GetNode(dMemBlock *pMemBlock)
{
	WORD i;
    TVMem *pNode;
	BYTE *pTemp;

	//��ƽ̨���ź�������
	OS_LockM(pMemBlock->SemId);
    
    //�жϵ�ǰ���нڵ����Ƿ�Ϊ��
    if(pMemBlock->pFreeList == NULL)
    {
       	//���нڵ���Ϊ�գ�����һ����нڵ���ڴ棬�����ӳɱ�
       	//�����ڴ���亯���������ڴ������ĸ��ֽڣ����ڰ�����Ϊ���нڵ���������ڴ涼��������
       	pNode = (TVMem*)OS_GetVMem(&(pMemBlock->pMem),pMemBlock->UnitNumOfPerBlock,pMemBlock->UnitLen);
    	if(pNode==NULL)
			return NULL;
			
       	//�ѷ���Ŀ��нڵ����ӵ�һ��
       	for(i=0; i<pMemBlock->UnitNumOfPerBlock; i++)
		{
        	//���뵽����ͷ
            INSERT_HEAD(pMemBlock->pFreeList,pNode);

			//����pNode
			pTemp=(BYTE *)pNode;
			pTemp+=pMemBlock->UnitLen;
			pNode=(TVMem*)pTemp;
		}
		
    	ReportMsg("\r\n MASSAGE BLOCK LINK MALLOC, task id=%x ! !!!!!!!!\r\n", RtosGetCurrentTaskId());
    }
    
    //���нڵ����Ϊ��
    ASSERT(pMemBlock->pFreeList != NULL);

    //�ѿ��нڵ�������׽ڵ����
    REMOVE_HEAD(pMemBlock->pFreeList,pNode);
    
    //�ѷ���ڵ��������1
    pMemBlock->UsedUnitNum++;

	//�ͷ�ƽ̨���ź���
	OS_UnLockM(pMemBlock->SemId);
    
    //���ط���Ŀ��нڵ�ָ��
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
/* ����: ���ڴ����ģʽ�µķ����ڴ�(ϵͳ�ں�̨���Լ���ڴ�ĺϷ���)                         */
/* ����: size �������ĳ���,file ��ǰ��������ڴ���ļ�����,line ���øú������к�          */
/* ���: ��                                                                                 */
/* ����: =NULL����ʧ��,����ʵ�ʷ�����ڴ�ָ��                                               */
/*------------------------------------------------------------------------------------------*/
void* OS_MemAllocDebug(int size,char *file,DWORD line)
{
    TMemTop *pMemTop;
	OS_TaskDesp *pTask = NULL;

    if(size == 0)
        return (void *)NULL;

    //����Ҫ������ֽڳ��ȳ��ֶ���
    size = ALIGN(size,ALIGNMENT);
    
    //�����ڴ���亯����������Ӧ���ȵ��ڴ棬�����ڴ�ͷ���ڴ��϶���ڼ���Ƿ����ڴ����
    //sizeof(TMemTop)/*�ڴ��ʽͷ*/+size/*����ĳ���*/+MAX_GAPLEN/*�ڴ��϶*/
    pMemTop = (TMemTop*)OS_MemAlloc(size+sizeof(TMemTop)+sizeof(BYTE)*MAX_GAPLEN);
    
    //����ڴ�ͷ
    pMemTop->szFileName = file;  //�ļ�����ָ��
    pMemTop->nLine = line;      //�����ڴ���亯�����ڵ��к�
    pMemTop->nSize = size;      //�������ĳ���
    pTask = OS_GetTaskTab(SELF_TASK_ID)->dwTaskID;   //ȡ������������ID��
    pMemTop->hTask = pTask;
	if(pTask != NULL)
		pTask->dwAllocMemLen += size;

    //�ڷ����ڴ���׶���������ʶ�ַ�
    OS_MemSet(pMemTop->aGap,MEM_GAP_FILL,MAX_GAPLEN);
    //�ڷ����ڴ��ĩ����������ʶ�ַ�
    OS_MemSet(MEM_DATA(pMemTop)+size,MEM_GAP_FILL,MAX_GAPLEN);

//    //�����ٽ�������
//    OS_LockM(MEM_LOCK);
    //�ѷ�����ڴ����뵽�ѷ�����ڴ�����
    INSERT_HEAD(g_pMemLink,pMemTop);
//    OS_UnLockM(MEM_LOCK);

    //���ص�ַ������ĵ�ַ+�ڴ�ͷ��
    return (void *)MEM_DATA(pMemTop);
}
/*------------------------------------------------------------------------------------------*/
/* ����: �ڴ��ʽģʽ�µ��ͷ��ڴ溯��,�ú���ͬʱУ���ڴ�ĺϷ���                            */
/* ����: buf�ͷŵ��ڴ���ָ��                                                                */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_MemFreeDebug(void *pbuf)
{
	OS_TaskDesp *pTask = NULL;

    TMemTop *pMemTop = MEM_HEAD(pbuf);
    if(pbuf == NULL)
        return;
    //����ڴ���ͷ�Ƿ��ƻ�
    if(!CheckMem(pMemTop->aGap,MAX_GAPLEN,MEM_GAP_FILL))
    {
        ReportMsg("Memory DAMAGED! ");
    }

    //����ڴ���δ�Ƿ��ƻ�
    if(!CheckMem((BYTE*)pbuf+pMemTop->nSize,MAX_GAPLEN,MEM_GAP_FILL))
    {
        ReportMsg("Memory DAMAGED! ");
    }

	/*����������з����ڴ��ܳ��ȼ�¼*/
    pTask = OS_GetTaskTab(SELF_TASK_ID)->dwTaskID;   //ȡ������������ID��
	if(pTask != NULL)
		pTask->dwAllocMemLen -= pMemTop->nSize;
	
    //�����ٽ�������
//    OS_LockM(MEM_LOCK);
    //�ѷ�����ڴ����뵽�ѷ�����ڴ�����
    REMOVE_HEAD(g_pMemLink,pMemTop);
//    OS_UnLockM(MEM_LOCK);

    //�����ڴ��ͷź����ͷ�ָ���ڴ���
    OS_MemFree(pMemTop);
}
/*------------------------------------------------------------------------------------------*/
/* ����: �ж��ڴ������Ƿ��ƻ�                                                             */
/* ����: pbuf �ڴ���ָ��,���ڴ��������ǵ���  MemAlloc���䷵�صĵ�ַ                         */
/* ���: ��                                                                                 */
/* ����: TRUE ��ַδ�ƻ�,FALSE��ַ���ƻ�                                                    */
/*------------------------------------------------------------------------------------------*/
BOOL IsAddressValid(char *pbuf)
{
    TMemTop *pMemTop;
    pMemTop = (TMemTop *)pbuf;
    //����ڴ���ͷ�Ƿ��ƻ�
    if(!CheckMem(pMemTop->aGap,MAX_GAPLEN,MEM_GAP_FILL))
    {
        return FALSE;
    }

    //����ڴ���δ�Ƿ��ƻ�
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
/* ����: ϵͳ��λ                                                             */
/* ����: ��λ����                         */
/* ���: ��                                                                                 */
/* ����: ��                                                    */
/*------------------------------------------------------------------------------------------*/
extern HANDLE	g_hWdogTaskID;
void SystemReset(DWORD type)
{
	//ͨ�����ƿ��Ź�ˢ��ʵ�ָ�λ������reboot�޷�ʵ��
	OS_PostMessage(g_hWdogTaskID, SM_SYS_RESET, 0, 0, 0, 0);
//	OS_Sleep(100);
//	reboot(BOOT_QUICK_AUTOBOOT);
}


#endif			
