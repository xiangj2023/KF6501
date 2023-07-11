#include "absos.h"
#include "platform.h"

//ϵͳ������Ϣ�ڵ��ڴ���ƿ�ָ��
extern dMemBlock *g_pMsgMemBlock;

//ϵͳ������Ϣ�����������ڵĿ�����Ϣ��Ϊ��ʱ����Ҫ��ϵͳ������Ϣ����ȥ������Ϣ��
STATIC TVMem *g_pMsgMem;
STATIC TMessageWrap *g_pMsgFreeList;
STATIC DWORD g_nEmptyMsg;

/*------------------------------------------------------------------------------------------*/
/* ����: �ɱ��ڴ���䣬�ѷ����ûһ���ڴ���������������                                     */
/* ����: pphead �ɱ��ڴ�����������ָ��,unitnum Ҫ����ĵ�Ԫ������unitlenҪ�����ÿ����Ԫ��  */
/*       ���ȣ������ʵ���ܳ���Ϊ:unitnum*unitlen+sizeof(TVMem)                             */
/* ���: pphead �ı��Ŀɱ䳤���ڴ�����ָ��                                                */
/* ����: Ӧ�ó������ʹ�õ��ڴ�����ָ�루���������ͷ��                                     */
/*------------------------------------------------------------------------------------------*/
void *OS_GetVMem(TVMem **pphead,WORD unitnum,WORD unitlen)
{
   	TVMem *pMem;
   	ASSERT(unitnum>0 && unitlen>0);

   	/*���÷����ڴ溯����������Ҫ���ڴ棬����ĳ���Ϊ������ɱ��ڴ��ָ��+untinum*unitlen*/
   	pMem = (TVMem*)OS_MemAlloc( sizeof(TVMem)+unitnum*unitlen) ;
		
   	/*��Ϸ�����ڴ�ָ���Ƿ�Ϸ�*/
   	ASSERT(pMem != NULL);
	if(pMem==NULL)
		return NULL;
	
   	/*��pMem���뵽�ɱ��ڴ�����ı�ͷ*/
   	/*�����ڴ���βָ��ָ��ɱ��ڴ�����ָ��*/
   	INSERT_HEAD(*pphead,pMem);

   	/*�����ڴ�ָ�룬����*/
   	return (void*)(pMem+1);
}
/*------------------------------------------------------------------------------------------*/
/* ����: �ͷſɱ��ڴ��                                                                     */
/* ����: pHead �ڴ����ָ��                                                                 */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
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
/* ����: �ڵ�ʽģʽ��,��������Ϣ����ͷ��ȡ��һ����Ϣ,���ڵ�������                           */
/* ����: task ���������������                                                              */
/* ���: ��Ϣָ��ĵ�ַ,=NULL ������Ϣ������û����Ϣ                                        */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_GetTaskMsg(OS_TaskDesp * task,TMessageWrap **msg) 
{
	DWORD imask;

//	imask = OS_IntLock();  //��ϵ������ʱ��֪��Ϊ�λ�����쳣�ж�
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

	//�ָ�ϵͳ����
//    OS_IntUnLock(imask);
	OS_UnProtect(0);
}
/*------------------------------------------------------------------------------------------*/
/* ����: ���յ�����Ϣ����������Ϣ����                                                       */
/* ����: task ����������,msg�յ�����Ϣ                                                      */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
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

		//�ָ�ϵͳ����
		OS_UnProtect(0);
}

/*------------------------------------------------------------------------------------------*/
/* ����: �������Ϣ�����ڴ洢�յ�����Ϣ                                                     */
/* ����: task ����������                                                                    */
/* ���: msg ���ؿ���Ϣ��ָ��,=NULL�������Ϣʧ��                                           */
/* ����: ��                                                                                 */
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
		
		//�ָ�ϵͳ����
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
/* ����: �ͷ���Ϣ��                                                                         */
/* ����: task����������,msg��Ϣ��ָ��                                                       */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_FreeMsg(OS_TaskDesp *task,TMessageWrap* msg) 
{
    DWORD imask;

//	imask = OS_IntLock();  
	OS_Protect(0);

    msg->pNext = task->pFreeList; 
    task->pFreeList = msg;

	//�ָ�ϵͳ����
//    OS_IntUnLock(imask);
	OS_UnProtect(0);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��ʼ�����������Ϣ�����ݶ���                                                       */
/* ����: ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void InitMsgWrap()
{
    //��ʼ��������Ϣ���ڴ�����ָ��
    g_pMsgMem = NULL;
    
    //��ʼ��������Ϣ�ڵ����� 
    g_pMsgFreeList = NULL;
    
    //�ѷ������Ϣ�ڵ����
    g_nEmptyMsg = 0;

#if(OS_PLATFORM ==  OS_WIN32)
	InitializeCriticalSection(&g_csMsgCommunicationSync);
#endif
}

/*------------------------------------------------------------------------------------------*/
/* ����: ȡ������Ϣ�ڵ㣬������Ԥ����Ŀ�����Ϣ�ڵ��ѷ�����ϣ����Ե���ϵͳ�Ŀ�����Ϣ�ڵ�   */
/* ����: ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: ������Ϣ�ڵ�ָ��                                                                   */
/*------------------------------------------------------------------------------------------*/
#define MAX_MSG_WRAP (512/sizeof(TMessageWrap))
TMessageWrap *OS_GetSysEmptyMsg()
{
    TMessageWrap *pWrap;
    DWORD mask;
    
    //�жϵ�ǰ����Hash�ڵ����Ƿ�Ϊ��
    if(g_pMsgFreeList == NULL)
    {
        //���нڵ���Ϊ�գ�����һ��MAX_MSG_WRAP���нڵ���ڴ棬�����ӳɱ�
        //�����ڴ���亯���������ڴ������ĸ��ֽڣ����ڰ�����Ϊ���нڵ���������ڴ涼��������
        pWrap = (TMessageWrap*)OS_GetVMem(&g_pMsgMem,MAX_MSG_WRAP,sizeof(TMessageWrap));
        
        //��ֹ�������
//        mask = OS_IntLock();
		OS_Protect(0);

        //�ѷ����MAX_MSG_WRAP������TMessageWrap�ڵ����ӵ�һ��
        for(int i=0;i<MAX_MSG_WRAP;i++,pWrap++)
            INSERT_HEAD(g_pMsgFreeList,pWrap);

        //�ָ��������ģʽ
//        OS_IntUnLock(mask);
		OS_UnProtect(0);
    }
    
    //hash���нڵ����Ϊ��
    ASSERT(g_pMsgFreeList != NULL);
    
   //��ֹ�������
//    mask = OS_IntLock();
   OS_Protect(0);

    //��hash���нڵ�������׽ڵ����
    REMOVE_HEAD(g_pMsgFreeList,pWrap);
    
    //������Ϣ�ڵ��������1
    g_nEmptyMsg++;
    
    //�ָ��������ģʽ
//    OS_IntUnLock(mask);
	OS_UnProtect(0);

    //���ط���Ŀ��нڵ�ָ��
    return pWrap;
}


