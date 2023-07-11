#if(OS_PLATFORM == OS_VXWORKS)

#include "osvxwork.h"
#include "absos.h"
#include "m5234spi.h"

void *SysMallocPtr = (void *)(SYS_DRAM_BASE+SYS_DRAM_LEN);	//�ڴ�������ռ�ָ��

//����Hash��
extern THashTable* g_hashTaskMap;

extern OSINT8 CurTaskCount;

//����������
extern TTask_Map TaskTable[];

/*ϵͳ��ʱ������ͷָ��*/
OS_TimerDesp *pNormalTimerList;
/*������ϵͳ��ʱ���ĸ���*/
BYTE NormalTimerNum;

//��ͨ��ʱ�������ź�������
SEM_ID NorTickSem = NULL; 

/*��ͨ��ʱ����������*/
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
	//��ʼ��ϵͳӲ��ʱ��
	M5234SpiInit(); 
}


/*------------------------------------------------------------------------------------------*/
/* ����: ����ϵͳ��ʼ��                                                                         */
/* ����: ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void VXWorksInit(void)
{
	OSINT8 taskid; 

	//��ʼ��ϵͳ��ʱ������ͷ
	pNormalTimerList=NULL;

	//��ʼ��ϵͳ��ʱ���ĸ���
	NormalTimerNum=0;

	//������ͨ��ʱ�������ź���
	NorTickSem = semCCreate(SEM_Q_PRIORITY, 0);
	ASSERT(NorTickSem != NULL);

	//��UCOSII��ͬ��ϵͳ���ж�ʱ���ڴ������ƿ�Ŀռ䲻��������ÿ����ʱ���ڵ�ռ�ֱ�Ӵ�
	//ϵͳ�ڴ����
	//������ͨ��ʱ����������
	taskid = taskSpawn("NTimeTask", NORMAL_TIMER_TASK_PRIO, 0, DEFAULT_TASK_STACK, \
					(FUNCPTR)VXNormalTimerTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ASSERT(taskid != ERROR);

/*
	//����ϵͳ���ж�ʱ���ڴ�������ƿ�
	pFreeNormalTimer=(OS_MEM *)0;
	addr=Ucosii_MemAlloc(MAX_NORMAL_TIMER_NUM*sizeof(OS_TimerDesp));
	if(addr)
	   pFreeNormalTimer=OSMemCreate (addr, MAX_NORMAL_TIMER_NUM, sizeof(OS_TimerDesp), &err);

	//��������ջ�ռ�
	addr0 = (BYTE *)Ucosii_MemAlloc(DEFAULT_TASK_STACK);
	if(addr!= NULL)
	{
		//����ջ����������ȷ��ջָ��
		#if OS_STK_GROWTH==0
			pStack=(OS_STK *)addr0;
		#else
			pStack=(OS_STK *)(addr0+StackSize-4);
		#endif

		//������ͨ��ʱ������
		OSTaskCreate(UcosiiNormalTimerTask, 0, pStack, NORMAL_TIMER_TASK_PRIO);
	}
*/
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��ͨ��ʱ������,����tick�жϺ�����ͬ���ź���������������ͨ��ʱ����                  */
/*		 �����Ӧ�����Ͷ�ʱ��Ϣ															*/
/* ����: ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void VXNormalTimerTask(void)
{
	OS_TimerDesp *pTimerNode;
//	STATIC int TimerCnt;
	int rtn;
	
	while(1)
	{
		//�����ҷ�ʽ�ȴ�ʱ�ӵ�વĵ��������ü����ź����ķ�ʽ��OSTimeTickHook()ͬ��
		semTake(NorTickSem, WAIT_FOREVER);
		
//		if(rtn==ERR_SUCCESS)
		{
//			TimerCnt++;
//			if((TimerCnt%200)==0)
//				ReportMsg("Timer Task Run OK, TimerCnt=%d....", TimerCnt);
			//�����ʱ���������ȼ�������Ӧ�ó������ȼ�����,��ô�˴��ɲ��ñ�����

			//��ֹ�����л�
			taskLock();

			//��鶨ʱ������
			pTimerNode=pNormalTimerList;
			while(pTimerNode!=NULL)
			{
				if(pTimerNode->CurTime>0)	//��ʱʱ��δ��
				{
					pTimerNode->CurTime--;//����ֵ�ݼ�����
					if(pTimerNode->CurTime == 0)
					{
						//������ǰ����ֵ
						pTimerNode->CurTime=pTimerNode->InitTime;
						
						//����Ӧ�������Ͷ�ʱ��Ϣ
						VXPostMessage(pTimerNode->pTask,SM_TIMEOUT,pTimerNode->dwEvtBit,0,0,0);
					}
						
				}
				//����ָ��
				pTimerNode=pTimerNode->pNextTimer;
			}
			//�ָ������л�
			taskUnlock();
		}//if
	}//while(1)
}


/*------------------------------------------------------------------------------------------*/
/* ����: ����һ������                                                                       */
/* ����: pTask--������ƿ�																	*/
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�����ɹ�,��������ʧ��                                                   */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXCreateTask(OS_TaskDesp *pTask)
{
	BYTE *addr = NULL;
	OSINT8 taskid; 
//	SEM_ID pSem = NULL;
	DWORD mask;
	MSG_Q_ID msgid;
	
	//��鲢��������ջ��С				
	if(pTask->StackSize == 0)
		pTask->StackSize = MIN_TASK_STACK_LEN;				
	
	//��������ջ�ռ�
	addr = (BYTE *)VXMemAlloc(pTask->StackSize);
	ASSERT(addr != NULL);
	
	//ջָ���������������Ĵ����ﱣ��
	pTask->Register[REG_TASK_STACK]=(DWORD)addr;
/*
	//������������Ϣ���м����ź���,��֤���ں˴�������֮ǰ����
	pSem = semCCreate(SEM_Q_PRIORITY, 0);
	ASSERT(pSem != NULL);
	pTask->Register[REG_MSG_SEM]=(DWORD)pSem;
*/
	CurTaskCount++;	
	ASSERT(CurTaskCount<=MAX_APP_TASK_NUM);

	//��ʼ����ʱ��������Ϊ��Ч
	for(int i=0; i<MAX_TIMER; i++)
		pTask->dwTimeTab[i] = 0xffffffff;

	//����Ӧ�ò�ID
	pTask->dwTaskID = (CurTaskCount+TASK_HANDLE);

	//����Ӧ�ò�ID
	pTask->Args[0] = pTask->dwTaskID;//(DWORD)pTask;

	//��������(ע��������ݵ���)
	taskid=taskSpawn(pTask->TaskName, pTask->Priority, 0, pTask->StackSize, (FUNCPTR)pTask->pTaskEntry,\
					(DWORD)(&pTask->Args[0]), 0, 0, 0, 0,0,0,0,0,0);
					//pTask->Args[0],pTask->Args[1],pTask->Args[2],pTask->Args[3],0,0,0,0,0,0);
					
	if(ERROR == (int)taskid)
	{
		if(CurTaskCount > 0)
			CurTaskCount--;
		
		TRACE("Create Task error.\n");
		//��������ʧ��
		return ERR_FAILURE; 
	}

	pTask->Register[REG_TASK_ID] = taskid;	//��Ų���ϵͳ�����ԭʼ����ID
	
	pTask->IsDeleted = 0;
	pTask->Counter = 0;
	pTask->wTimerNum = 0;

	//������Ϣ����
	msgid = msgQCreate(16, sizeof(TMessage), MSG_Q_PRIORITY|MSG_Q_EVENTSEND_ERR_NOTIFY);
	if(msgid == NULL)
	{
		ReportMsg("Create task msg error.");
		return ERR_FAILURE;
	}
	pTask->hQueue = (void*)msgid;

	
    //������HASH������ڼ䣬��ֹ����֮�����ռ
//	mask = OS_IntLock();

	//�����������ֵ��������������ָ�����
	TaskTable[CurTaskCount].pTask = pTask;

	//������������ָ��ŵ����������������У��������ڲ�������������
	taskVarAdd(taskid, (int *)pTask);
	
	//�ָ�������ռģʽ
//	OS_IntUnLock(mask);
	
	//���سɹ�
	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ����Ӧ�ò�����IDȡ���������������		                                            */
/* ����: ����Ӧ�ò�ID                      			                                        */
/* ���: ��						                                                            */
/* ����: ����������ָ����ָ��						                                        */
/*------------------------------------------------------------------------------------------*/
OS_TaskDesp *VXGetTaskTab(OSINT8 id)
{
	int vxID, i;

	if(id == APP_TASKID_NULL)
		return (OS_TaskDesp *)NULL;
	
	if(id == SELF_TASK_ID)	//ȡ��ǰ�����������
	{
		vxID = taskIdSelf();
		for(i=1; i<=(MAX_APP_TASK_NUM+1); i++)
		{
			if(TaskTable[i].pTask->Register[REG_TASK_ID] == vxID)	//������ϵͳID�Ƿ�һ��
				return TaskTable[i].pTask;
		}
	}
	else
	{
		//�������ID�ĺϷ���
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
		if(TaskTable[i].pTask->Register[REG_TASK_ID] == vxID)	//������ϵͳID�Ƿ�һ��
			return TaskTable[i].pTask->dwTaskID;
	}

	return 0;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ���񱣻���ͨ����ֹ�������ʵ��                                                     */
/* ����: ����������ָ��                    			                                        */
/* ���: ��						                                                            */
/* ����: �ɹ���ʧ��									                                        */
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
/* ����: ��ֹ���񱣻���ͨ�������������ʵ��                                                 */
/* ����: ����������ָ��                    			                                        */
/* ���: ��						                                                            */
/* ����: �ɹ���ʧ��									                                        */
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
/* ����: ����������						                                                */
/* ����: ����������ָ��                    			                                        */
/* ���: ��						                                                            */
/* ����: �ɹ���ʧ��									                                        */
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
/* ����: ����ָ�����						                                                */
/* ����: ����������ָ��                    			                                        */
/* ���: ��						                                                            */
/* ����: �ɹ���ʧ��									                                        */
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
/* ����: ����ָ�����						                                                */
/* ����: ����������ָ��                    			                                        */
/* ���: ��						                                                            */
/* ����: �ɹ���ʧ��									                                        */
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
/* ����: ������ʱ,��ֹ���жϴ�������е���	                                                */
/* ����: ��ʱ������	                    			                                        */
/* ���: ��						                                                            */
/* ����: �ɹ���ʧ��									                                        */
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
/* ����: �����������ź���,�ṩ��Ӧ�ó���ʹ��                                                      */
/* ����: �ź�����ʼֵ                                                                       */
/* ���: MutexId�������ź�����ʶ                                                            */
/* ����: ERR_SUCCESS�����ɹ�,��������ʧ�ܵķ��ش���                                         */
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
/* ����: ���������ź���,�ṩ��Ӧ�ó���ʹ��                                                      */
/* ����: �ź�����ʼֵ                                                                       */
/* ���: �����ź����������ź�����ʶ                                                            */
/* ����: ERR_SUCCESS�����ɹ�,��������ʧ�ܵķ��ش���                                         */
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
/* ����: ��ȡ�ź���,�ṩ��Ӧ�ó���ʹ��                                                      */
/* ����: MutexId�ź�����ʶ��,TimeOut�ȴ�ʱ��(SEM_SUPEND���ҵȴ���SEM_NOWAIT���ȴ��������ȴ��ĺ�������    */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS��ȡ�ɹ�,������ȡʧ�ܵķ��ش���                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXSemGet(TMutexHandle MutexId, OSINT16 TimeOut)
{
	int ticks;
	STATUS rtn;
    
	//���ȴ���ʽ
	if(TimeOut==SEM_NOWAIT)
	{
		rtn = semTake((SEM_ID)MutexId, NO_WAIT);
	    	if(rtn == OK )
	    		return ERR_SUCCESS;
    		else
		    	return ERR_GET_SEM_ERR;
	}

	//�ȴ���ʽ
	//ת��������
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
/* ����: �ͷ��ź���,�ṩ��Ӧ�ó���ʹ��                                                      */
/* ����: MutexId�ź�����ʶ��                                                                */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�ͷųɹ�,�����ͷ�ʧ�ܵķ��ش���                                         */
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
/* ����: ��ȡϵͳ��̬RAM�ռ�			                                                    */
/* ����: ����ռ䳤��		                                                                */
/* ���:                                                                                  	*/
/* ����: ��ȡ�Ķ�̬RAM�ռ��ַָ��                                         					*/
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
/* ����: ����intLockLevelSet���趨�ļ���һ�µ��жϣ�����ϵͳ���Ƚ����ε����ȼ��趨Ϊ���	*/
/* ����: ��                                 						                        */
/* ���:  							                                                        */
/* ����: ϵͳ��ǰ���ε��ж����ȼ�                                                           */
/*------------------------------------------------------------------------------------------*/
DWORD VXIntLock(void)
{
	return intLock();
}	

/*------------------------------------------------------------------------------------------*/
/* ����: �������رյ��ж����ȼ�																*/
/* ����: IntLev�жϼ���(��߻�����)                                                         */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void VXIntUnLock(DWORD intLev)
{
	intUnlock(intLev);
}	

/*********************************************************************************************
 *
 * ��������     �жϼ���
 *
 * ����˵��      - ivect	: ���� 	�ж�����
 *				 - isr		: ����	�жϷ���ӿں���
 *				 - arg		: ����	���ݸ��жϷ������Ĳ���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
OSINT16 VXSetVector(DWORD ivect,TISR_ENTRY isr,DWORD arg)
{
	int rc;
	//�����Ϸ��Լ��
	ASSERT(ivect >= MIN_I_VECT_NO);
	ASSERT(ivect <= MAX_I_VECT_NO);

	if( (ivect < MIN_I_VECT_NO) || (ivect > MAX_I_VECT_NO) )
	{
        LogError("VXSetVector",FILE_LINE,"the vector number %d error.", ivect);
		return ERR_FAILURE;
	}

	//�жϼ���
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
 * ��������     �ж�����
 *
 * ����˵��      - ivect	: ���� 	�ж�����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
OSINT16 VXIntEnable(DWORD ivect)
{
	int rc;
	//�����Ϸ��Լ��
	ASSERT(ivect >= MIN_I_VECT_NO);
	ASSERT(ivect <= MAX_I_VECT_NO);

	if( (ivect < MIN_I_VECT_NO) || (ivect > MAX_I_VECT_NO) )
	{
        LogError("VXIntEnable",FILE_LINE,"the vector number %d error.", ivect);
		return ERR_FAILURE;
	}

	//�жϼ���
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
 * ��������     �жϽ�ֹ
 *
 * ����˵��      - ivect	: ���� 	�ж�����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
OSINT16 VXIntDisable(DWORD ivect)
{
	int rc;
	//�����Ϸ��Լ��
	ASSERT(ivect >= MIN_I_VECT_NO);
	ASSERT(ivect <= MAX_I_VECT_NO);

	if( (ivect < MIN_I_VECT_NO) || (ivect > MAX_I_VECT_NO) )
	{
        LogError("VXIntDisable",FILE_LINE,"the vector number %d error.", ivect);
		return ERR_FAILURE;
	}

	//�жϼ���
	rc = intDisable(ivect);
	if(rc == ERROR)
	{
        LogError("VXIntDisable",FILE_LINE,"disable the vector number %d error.", ivect);
		return ERR_FAILURE;
	}
	return ERR_SUCCESS;
	
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��������ʱ��                                                                     */
/* ����: pTask,sign ��ʱ����ʶ��,interval ����ʱ���,��λΪ����     */                      
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS�����ɹ�,��������ʧ�ܵķ��ش���                                         */
/*------------------------------------------------------------------------------------------*/
//ע��������ⶨʱ����interval��ʱ�䵥λΪ����
OSINT16 VXSetTimer(OS_TaskDesp *pTask,OSINT8 sign, OSINT16 interval, TSysTime *pBeginTime)
{
	OS_TimerDesp *pTimerNode = NULL;
	int i;

	//�Ϸ��Լ��
	ASSERT(pTask != NULL);
	ASSERT(interval != NULL);
	
	//������ͨ��ʱ��
	if(pBeginTime == NULL)
	{
		if(NormalTimerNum>=MAX_NORMAL_TIMER_NUM)
		{
			TRACE("set normal timer overflow");
			return ERR_INVALID_TIMER;
			
		}
		
		//��鱾����ʱ�������Ƿ���Ч
		if(pTask->wTimerNum>=MAX_TIMER)
		{
			ReportMsg("Create too many time for one task.");
			return ERR_INVALID_TIMER;
			
		}

		//���ö�ʱ���Ƿ���ڣ������ظ�����
		pTimerNode=pNormalTimerList;
		while(pTimerNode!=NULL)
		{
			if((pTimerNode->pTask == pTask) && (pTimerNode->Sign == sign))
				return ERR_INVALID_TIMER;
			pTimerNode = pTimerNode->pNextTimer;
		}

		//��ȡ���ж�ʱ���ڵ�
		pTimerNode=(OS_TimerDesp *)VXMemAlloc(sizeof(os_timerdesp));
		ASSERT(pTimerNode != NULL);

		//��д��ʱ���ڵ�������Ϣ
		pTimerNode->pTask=pTask;                     //�����������ָ��
		pTimerNode->Sign=sign;                       //��ʱ����ʶ��
		pTimerNode->CurTime= MS2TICK(interval);      //��ǰ����ֵ
		pTimerNode->InitTime=MS2TICK(interval);      //��ʼ����ֵ

		//�������м�¼��ʱ��ID
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
		
		//��ֹ�����л�
		taskLock();

		//����ýڵ㵽ϵͳ��ʱ������
		pTimerNode->pNextTimer=pNormalTimerList;
		pNormalTimerList=pTimerNode;
		NormalTimerNum++;

		//�ָ������л�
		taskUnlock();
//		ReportMsg("set timer OK!");
	}

	return ERR_SUCCESS;

}

/*------------------------------------------------------------------------------------------*/
/* ����: ɾ������ʱ��                                                                     */
/* ����: tid�����(0xff��ʾ��������),sign ��ʱ����ʶ��                                      */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESSɾ���ɹ�,����ɾ��ʧ�ܵķ��ش���                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXDelTimer(OS_TaskDesp *pTask,OSINT8 sign)
{
	OS_TimerDesp *pTimerNode;
	OS_TimerDesp **ppPrev;
	int i;
	
	//��ֹ�����л�
	taskLock();

	ppPrev=&pNormalTimerList;

	//������ͨ��ʱ������
	for(pTimerNode = *ppPrev; pTimerNode != NULL; pTimerNode = pTimerNode->pNextTimer)
	{
		//�ж��Ƿ�����Ҫɾ���Ķ�ʱ��
		if((pTimerNode->pTask==pTask)&&(pTimerNode->Sign==sign))
		{
			//�Ѹö�ʱ���ڵ��ϵͳ��ʱ������ɾ��
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

			//�ָ������л�
			taskUnlock();
	
	        //�ͷŶ�ʱ���ڵ㵽ϵͳ���ж�ʱ����
	        free((void*)pTimerNode);
			return ERR_SUCCESS;
	  	}
	    //����Ҫ���ҵĶ�ʱ����������һ����ʱ���ڵ�
	    ppPrev = &pTimerNode->pNextTimer;
		if(*ppPrev == NULL)
			break;
	}
    //�ָ������л�
	taskUnlock();

	//û���ҵ�ָ���Ķ�ʱ��
	ReportMsg("Not find to be deleted timer!");
	return ERR_NO_FIND_TIMER;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ɾ��ָ���������еĶ�ʱ��������ɾ��ʱʹ��                                             */
/* ����: pTask                                                                              */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESSɾ���ɹ�,����ɾ��ʧ�ܵķ��ش���                                         */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXDelAllTaskTimer(OS_TaskDesp *pTask)
{
	OS_TimerDesp *pTimerNode;
	OS_TimerDesp **ppPrev;
	
	//��ֹ�����л�
	taskLock();

	ppPrev=&pNormalTimerList;

	//������ͨ��ʱ������
	for(pTimerNode = *ppPrev;pTimerNode != NULL;pTimerNode = pTimerNode->pNextTimer)
	{
		//�ж��Ƿ�����Ҫɾ���Ķ�ʱ��
	    if(pTimerNode->pTask==pTask)
	 	{
	    	//�Ѹö�ʱ���ڵ��ϵͳ��ʱ������ɾ��
	        *ppPrev = pTimerNode->pNextTimer;
			NormalTimerNum--;
	
	        //�ͷŶ�ʱ���ڵ㵽ϵͳ���ж�ʱ����
	        free((void*)pTimerNode);
		}
	    //����Ҫ���ҵĶ�ʱ����������һ����ʱ���ڵ�
	    ppPrev = &pTimerNode->pNextTimer;
	}

   	//�ָ������л�
	taskUnlock();

	return ERR_SUCCESS;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ����������Ϣ                                                                     */
/* ����: pTask Ŀ������������ָ��,msg ��Ϣ�ţ�larg,harg,warg,parg�ֱ�Ϊ��Ϣ����             */
/* ���: ��                                                                                 */
/* ����: ERR_SUCCESS��Ϣ���ͳɹ�����������ʧ��                                              */
/*------------------------------------------------------------------------------------------*/
OSINT16 VXPostMessage(OS_TaskDesp *pTask, OSINT16 msg, OSINT32 harg, OSINT32 warg,\
													OSINT32 parg, OSINT16 larg)
{
    TMessage message;
    STATUS rc;
	int errorno;
          
    if( pTask==NULL )
        return ERR_POST_SEM;

	//��ʱ��Ϣ����,�����¼���Ϣ����
	if((msg == SM_TIMEOUT) || (msg == SM_RELAY_INT) ||(msg == SM_RELAYTIMER))
	{
		//���Ͷ�ʱ�¼���
		rc = eventSend(pTask->Register[REG_TASK_ID],harg);
		if(rc == ERROR)
		{
			ReportMsg("postmessage eventSend timeout error, errorno=%x, task name=%s.", errno, pTask->TaskName);
			return ERR_POST_MSG;
		}
		return ERR_SUCCESS;
	}
	
	//��ͨ��Ϣ����
	message.lParam = MAKELONG(msg,larg);
	message.hParam = harg;
	message.wParam = warg;
	message.pParam = parg;
	
	//���÷ǵȴ���ͨ���ȼ���ʽ������Ϣ,ͬʱ����EV_DBNOTIFYΪ���¼�
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

	//ȡ���нڵ�
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

    //����Ϣ�ڵ������Ϣ����
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
/* ����: ��������Ϣ������ȡ��һ����Ϣ,��Ϣ���п�ʱ�����������״̬                          */
/* ����: pTask msg mode(��Ч)		                                                    */
/* ���: msgȡ�õ���Ϣָ��                                                                  */
/* ����: ERR_MESSAGE_OK ȡ��Ϣ����, û��������¼���ERR_MESSAGE_QUIT �����˳�		    */
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

	//��������Ϣ������ȡ����Ҫ�������Ϣ
	OS_GetTaskMsg(pTask,&wmsg);
	
    //�ж���Ϣ�Ƿ���Ч
    if(wmsg == NULL)
    {
        //������Ϣ�������Ѿ�û����Ҫ�������Ϣ,���ú�������������
        //��������ev_receive��������;û���账�������,���������õ�����
		rc = eventReceive(EV_MASKALL, EVENTS_WAIT_ANY, timeout, (UINT32*)&dwEvent);
		if(rc == ERROR)
		{
			ReportMsg("Getmessage eventReceive error, errorno=%x, task name=%s", errno, pTask->TaskName);
			return ERR_MESSAGE_NULL;
		}
        
        //�жϽ��յ��µ������Ƿ�Ϊ��,һ���������ִ�е�����,dwEventӦ�ò�������
        //��Ϊû������ev_receive��������������;ֻ������,����Ż�ִ�е�����.����
        //ϵͳ���Թ�����,���ֹ�dwEvent=0������.
        if(dwEvent == 0)
        {
            //���յ�������Ϊ��,ע�������Ϣ
            ReportMsg("GetMessage,event=0.....");

            //������ϢΪ�յĴ������
            return ERR_MESSAGE_NULL;
        }

        //�жϽ��յ��������Ƿ�Ϊ��ʱ����
        if(dwEvent & EV_TIMERMASK)
        {
            //�Ƕ�ʱ����,���ö�ʱ�������
            VXEventTimer(pTask,dwEvent);
        }
        
        //�жϽ��յ������Ƿ�Ϊ��Ϣ���ձ�־
        if(dwEvent & EV_DBNOTIFY)
        {
            //���յ�����Ϊ��Ϣ���ձ�־,���ý�����Ϣ����
            VXEvDBNotify(pTask );
        }

		//�¼���Ϣ����
		if(dwEvent & EV_EVENTMAPMASK)
		{
			VXEventMsg(pTask, dwEvent);
		}

        //��������յ���Ϣ������,ȡ������ͷ��һ����Ϣ
        OS_GetTaskMsg(pTask,&wmsg);
    }
	
    //�ж�ȡ������Ϣ�Ƿ���Ч,���������ȡ������Ϣ��Ӧ��Ϊ��,���ڵ��Թ����з�������Ϣ�յ�����
    if(wmsg != NULL)
    {
        //��Ϣ����,�Ѹ���Ϣ�����������������msg
        (*msg)=wmsg->Msg;
        
        //����Ϣ���õĽڵ����ӵĿ�����Ϣ��
        OS_FreeMsg(pTask,wmsg);
    }
    else
    {
//		ReportMsg("Get Null message, task name=%s.", pTask->TaskName);
        //��ϢΪ��,������Ϣ��״̬
        status = ERR_MESSAGE_NULL;
        
    }

    //�жϵ�ǰ�յ�����Ϣ�Ƿ�Ϊ���������˳���Ϣ
    if(LOWORD(msg->lParam) == SM_QUIT)
    {
        //�յ������˳���Ϣ,���ú�������״̬Ϊ������Ϣ�˳�״̬
        status = ERR_MESSAGE_QUIT;

    }

    //����ʵ�ʽ��
    return status;
		
	
/*	
	int rtn, type;
	TMessageWrap  *pMsg;
	OSINT16 status = ERR_MESSAGE_OK;
	SEM_ID pSemId;
	
	//��ȡ��Ϣ��������ʽ���У��������������ع���
	pSemId = (SEM_ID)pTask->Register[0];

	if(mode == 0)
		type = WAIT_FOREVER;
	else
		type = NO_WAIT;
	//����Ϣ������ȡ��Ϣ,���ҵȴ���ʽ
	rtn = semTake(pSemId, type);
	if(rtn == OK)//ȡ��Ϣ�ɹ�
	{
		
		//��ȡ��Ϣ�ڵ�
		OS_GetTaskMsg(pTask, &pMsg);

		if(pMsg == NULL)
		{
			ReportMsg("Task %d VX get null message!\n", pTask->Priority);
			return ERR_MESSAGE_NULL;
		}
		//ȡ��Ϣ��Ϣ
		(*msg) = pMsg->Msg;

		//�ͷ���Ϣ�ڵ�
		OS_FreeMsg(pTask, pMsg);

    	//�жϵ�ǰ�յ�����Ϣ�Ƿ�Ϊ���������˳���Ϣ
    	if(LOBYTE(msg->lParam) == SM_QUIT)
    	{
        	//�յ������˳���Ϣ,���ú�������״̬Ϊ������Ϣ�˳�״̬
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

    //��������ʱ����,����������event����һ�µĶ�ʱ��
    for(int i=0;i<MAX_TIMER;i++)
    {
        //�жϵ�ǰ��ʱ����Ӧ����������λ,�Ƿ��������յ�����������λ��
        if(event & bitm)
        {
			//��������Ѿ�����Ķ�ʱ����־
			tmask &= ~bitm;

			if(pTask->dwTimeTab[i] != 0xffffffff)
			{
	            //���û�ж�Ӧ�Ķ�ʱ�������Ѹö�ʱ������뵽�������Ϣ������
	            //�ӿ��б��з��������Ϣ�ڵ�
	            OS_GetEmptyMsg(pTask,&msg);
	            
	            //�жϷ����Ƿ�ɹ�
	            if(msg != NULL)
	            {
	                //��Ϣ����ɹ�,�����Ϣ����
	                msg->Msg.lParam = MAKELONG(SM_TIMEOUT,0);
	                msg->Msg.hParam = pTask->dwTimeTab[i];
	                msg->Msg.wParam = 0;
	                msg->Msg.pParam = 0;

	                //�Ѹ���Ϣ�ڵ����뵽������Ϣ������
	                OS_SetTaskMsg(pTask,msg);
	            }
	            else
	            {
	                //��Ϣ�ڵ����ʧ��,�ǼǴ�����Ϣ
	                TRACE("EventTimer,df_GetEmptymsg error");
	                //   Exit();
	            }//end of else ---no message room is system error
			}
        }//end of if----get event map to timer id

		//�ж��Ƿ���ĩ����Ķ�ʱ��
		if(event & tmask)
			//�ж���һ����������
			bitm <<= 1;
		//���ж�ʱ�¼���������ϣ��˳�ѭ��
		else break;
    }//end of for--polling all register for timer id
}

void VXEvDBNotify(OS_TaskDesp *pTask)
{
    TMessageWrap *wmsg;
    DWORD rc;
	TMessage msg;


    //������ϵͳ��Ϣ����,ֱ�����п�Ϊֹ
    while(1)
    { 
        //����pSOSϵͳ����,����������ж�pSOS��Ϣ
        rc = msgQReceive((MSG_Q_ID)pTask->hQueue,(char*)&msg, sizeof(TMessage), NO_WAIT);
        
        //�ж�������Ϣ�����Ƿ��ѿ�
        if(rc == ERROR)
        {
            //������Ϣ����Ϊ��,�˳�ѭ��
            return;
        }
        
		//�ж�ȡ��Ϣ���������Ƿ�ɹ�
		if(rc != sizeof(TMessage))
		{
			ReportMsg("q_receive error ,rc=%d",rc);
			return;
		}

		//�ӿ���������ȡ������Ϣ�ڵ�,���ڴ洢�յ���OS��Ϣ
        OS_GetEmptyMsg(pTask,&wmsg);

        //�ж�ȡ������Ϣ�ڵ��Ƿ�ɹ�
        if(wmsg==NULL)
        {
            //ȡ������Ϣ�ڵ�ʧ��,���������Ѿ�û�п�����Ϣ�ڵ�,ϵͳ�쳣����,���³�ʼ��������
            //��ֹϵͳһ�е���,������߼��ж�

            //�ǼǴ�����Ϣ
            TRACE("EvDBNotify,No Free message");

            //ϵͳ������
            //SystemReset(WARM_RESET);
            return;
        }

        wmsg->Msg = msg;

        //�������δ��,�Ѷ�����pSOS��Ϣ���뵽������Ϣ���б�
        OS_SetTaskMsg(pTask,wmsg);
    }
}

void VXEventMsg(OS_TaskDesp *pTask, DWORD dwEvent)
{
    TMessageWrap *msg;

	//���û�ж�Ӧ�Ķ�ʱ�������Ѹö�ʱ������뵽�������Ϣ������
	//�ӿ��б��з��������Ϣ�ڵ�
	OS_GetEmptyMsg(pTask,&msg);
		
	//�жϷ����Ƿ�ɹ�
	if(msg != NULL)
	{
		//��Ϣ����ɹ�,�����Ϣ����
		if(dwEvent&EV_RELAYBIT)
			msg->Msg.lParam = MAKELONG(SM_RELAY_INT,0);
		else if(dwEvent&EV_RELAYTIMEBIT)
			msg->Msg.lParam = MAKELONG(SM_RELAYTIMER,0);
		msg->Msg.hParam = 0;
		msg->Msg.wParam = 0;
		msg->Msg.pParam = 0;

		//�Ѹ���Ϣ�ڵ����뵽������Ϣ������
		OS_SetTaskMsg(pTask,msg);
	}
	else
	{
		//��Ϣ�ڵ����ʧ��,�ǼǴ�����Ϣ
		TRACE("EventTimer,df_GetEmptymsg error");
	}
	
}

#endif
