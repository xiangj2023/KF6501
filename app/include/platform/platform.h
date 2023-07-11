
#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "memdefs.h"
#include "absosdef.h"
#include "absos.h"
#include "timedef.h"

#if(OS_PLATFORM == OS_VXWORKS)
	#include "osvxwork.h"
#endif

//������ʹ�þ������ϵͳ�Ĳ�ͬ����ÿ����Ϣ���ƿ�ĵ�Ԫ����
#if(OS_PLATFORM == OS_UCOSII)
	#define MAX_UNIT_NUM			4
	#define TASK_PRI_BASE			10	/*�������ȼ���ʼֵ*/
	#define MAX_APP_TASK_NUM		20	/*ϵͳӦ������������*/
	#define MIN_TASK_STACK_LEN		64	/*�����ջ��С����*/
	#define MAX_NORMAL_TIMER_NUM    16   	//ϵͳ�����ͨ��ʱ�����ӳ�ʼ����ʼ��ʱ����������
	#define MAX_SPECIAL_TIMER_NUM	16		//ϵͳ������ⶨʱ������ָ��ʱ�俪ʼ��ʱ����������	
	#define DEFAULT_TASK_STACK    	128 	//Ĭ�������ջ��С
	#define NORMAL_TIMER_TASK_PRIO  4    	//��ͨ��ʱ���������ȼ�,��uCOS-II�Ľ��飬ǰ�ĸ����ȼ���ñ���
#else
	#define MAX_UNIT_NUM 			16
	#define TASK_PRI_BASE			0	/*�������ȼ���ʼֵ*/
	#define MAX_APP_TASK_NUM		128	/*ϵͳӦ������������*/
	#define MIN_TASK_STACK_LEN		256	/*�����ջ��С����*/
	#define MAX_NORMAL_TIMER_NUM	64	//ϵͳ�����ͨ��ʱ�����ӳ�ʼ����ʼ��ʱ����������
	#define MAX_SPECIAL_TIMER_NUM	64	//ϵͳ������ⶨʱ������ָ��ʱ�俪ʼ��ʱ����������	
	#define DEFAULT_TASK_STACK    	4096 	//Ĭ�������ջ��С
	#define NORMAL_TIMER_TASK_PRIO	40	//��ͨ��ʱ���������ȼ�,���ڹ���������ͨ��ʱ��
#endif


//����,ͬ���ź�����ʶ,�ɸ�����Ҫ����
enum {STDOUT_LOCK = 0,MEM_LOCK,SYSMSGNODE_LOCK, DAI_LOCK, SPI_LOCK, IIC_LOCK, FLASH_LOCK, HASH_LOCK};
enum {RELAY_TASK = 0,RESV0, RESV1};

/*---------------------------------------------------------------------*/
/* �ڵ�ʽģʽ�·�����ڴ��ṹΪ:                                     */
/*  TMemTop + malloc memory +gap[MAX_GAPLEN]                           */
/* ��ϵͳ����ʱ��IDLE�����Զ���������ڴ������鿴�ڴ����˵ı�־�Ƿ�  */    
/* ��Ӧ�ó����д                                                      */
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
/* �����ź�����                                                          */
/*---------------------------------------------------------------------*/
typedef struct mutextable{
    LPSTR        pName;         /*�ź�������*/
    TMutexHandle hMutex;        /*RTOS�õ����ź������*/    
//    WORD        dwStatus;      /*���ź���״̬*/
}TMutexTable;

typedef struct CSemtable{
    LPSTR        pName;         /*�ź�������*/
    TCSemHandle hCSem;        /*RTOS�õ����ź������*/    
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

//�������
#define OS_TaskRegister 		RtosTaskRegister
#define OS_GetTaskTab			RtosGetTaskTab

//��ʱ������
#define OS_Sleep				RtosSleep

//�жϹ���
#define OS_IntLock				RtosIntLock
#define OS_IntUnLock			RtosIntUnLock
#define OS_SetVector			RtosSetVector
#define OS_IntEnable			RtosIntEnable
#define OS_IntDisable			RtosIntDisable

//�ź�������
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
* ����һ������                                                          
*************************************************************************************/
DWORD OS_CreateTask(STRING *name,TTASK_ENTRY func,DWORD pri,DWORD stack,DWORD arg2,DWORD arg3,DWORD arg4);

/*************************************************************************************
* ��������ʼ���ڴ�ڵ���ƿ�                                                           
*************************************************************************************/
void InitMemBlock(dMemBlock **p,  WORD UnitNumOfPerBlock,WORD UnitLen,DWORD SemId);

/*************************************************************************************
* ������Ϣ                                                           
*************************************************************************************/
DWORD OS_PostMessage(DWORD tid, DWORD msg, DWORD harg, DWORD warg,\
														DWORD parg, DWORD larg);
/*************************************************************************************
* ������Ϣ                                                           
*************************************************************************************/
DWORD OS_GetMessage(DWORD tid,TMessage* msg, DWORD mode = 0);

/*************************************************************************************
* ��������ʱ��                                                           
*************************************************************************************/
DWORD OS_SetTimer(DWORD tid,DWORD sign,DWORD interval, TSysTime *pBeginTime);

/*************************************************************************************
* ɾ������ʱ��                                                           
*************************************************************************************/
DWORD OS_DelTimer(DWORD tid,DWORD sign);

/*************************************************************************************
* ������뱣��״̬����ֹ����������ռ                                                           
*************************************************************************************/
DWORD OS_Protect(DWORD tid);

/*************************************************************************************
* �����˳�����״̬��������������                                                           
*************************************************************************************/
DWORD OS_UnProtect(DWORD tid);

/*************************************************************************************
* �������                                                           
*************************************************************************************/
DWORD OS_Suspend(DWORD tid);

/*************************************************************************************
* ����ָ�                                                           
*************************************************************************************/
DWORD OS_Resume(DWORD tid);

/*************************************************************************************
* ����ɾ��                                                           
*************************************************************************************/
DWORD OS_Kill(DWORD tid);

/*************************************************************************************
* �ٽ���Դ����                                                           
*************************************************************************************/
DWORD OS_LockM(DWORD id);

/*************************************************************************************
* �ٽ���Դ�����˳�                                                           
*************************************************************************************/
DWORD OS_UnLockM(DWORD id);

/*------------------------------------------------------------------------------------------*/
/* ����: ȫ�ּ����ź�����ȡ����                       									*/
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemGet(DWORD id, DWORD mode);

/*------------------------------------------------------------------------------------------*/
/* ����: ȫ�ּ����ź����ͷŲ���                       									*/
/*------------------------------------------------------------------------------------------*/
DWORD OS_GlobCSemSet(DWORD id);

/*************************************************************************************
* �����˳�                                                           
*************************************************************************************/
void OS_Exit(BYTE code);

/*************************************************************************************
* ��ǰ����ע��,�����ǰ�������м���                                                           
*************************************************************************************/
DWORD OS_TaskRegister(void);

/*************************************************************************************
* ���ݲ�ͬ����ϵͳ���ͣ���������ں�������תΪͳһָ����ʽ                                                          
*************************************************************************************/
//DWORD *GetTaskArg(TASKARG arg);

/*************************************************************************************
* ��ָ�����ڴ���ƿ�ȡ���нڵ�                                                           
*************************************************************************************/
void *OS_GetNode(dMemBlock *pMemBlock);


/*************************************************************************************
* ϵͳ��λ                                                           
*************************************************************************************/
void SystemReset(DWORD type);

#endif
