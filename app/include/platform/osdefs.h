#ifndef _OSDEFS_H
#define _OSDEFS_H

#include "sysdefs.h"


//����������ź�����־
#define BSEM_FULL	1		//��Դδռ��
#define BSEM_EMPTY	0

#define MUTEX_NOCREATE	0

#define MUTEX_ERROR         (1<<0)
#define TASK_PROTECTED      (1<<1)

//�����쳣�˳�������붨��
#define EC_MEMORY_INVLID    0
#define EC_MSG_ERROR        1
#define EC_TIMER_ERROR      2
#define EC_RTOS_DISORDER    3
#define EC_RXD_QUIT         4
#define EC_ASSERT           5

/*---------------------------------------------------------------------*/
/* ���ݲ��õ�RTOS����,����RTOS��ͨ�ü��ƽ̨�Ľӿ�                     */
/*---------------------------------------------------------------------*/
#if (OS_PLATFORM == OS_UCOSII)
	#include "ucosii.h"
	
	//�������
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
	
	//��Ϣ����
	#define RtosPostMessage			UcosiiPostMessage
	#define RtosGetMessage			UcosiiGetMessage
	
	//��ʱ������
	#define RtosSetTimer			UcosiiSetTimer
	#define RtosDelTimer			UcosiiDelTimer
	#define RtosSleep				UcosiiTaskDly
	
	//�жϹ���
	#define RtosIntEnable			UcosiiIntEnable
	#define RtosIntDisable			UcosiiIntDisable

	#define OS_IntMask              UcosiiIntMask
	#define OS_SetVector			UcosiiSetVector
	
	//�ź�������
	//���ݳ�ʼ�������������ͬ�������⼰����ʹ��
	#define RtosSemBCreate			UcosiiSemCreate
	#define RtosSemGet				UcosiiSemGet
	#define RtosSemRelease			UcosiiSemRelease
	
	//�ڴ����
	#define RtosMalloc             Ucosii_MemAlloc


#elif(OS_PLATFORM == OS_VXWORKS)
	//�������
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

	//��Ϣ����
	#define RtosPostMessage 		VXPostMessage
	#define RtosGetMessage			VXGetMessage

	//��ʱ������
	#define RtosSetTimer			VXSetTimer
	#define RtosDelTimer			VXDelTimer
	#define RtosSleep				VXTaskDly

	//�жϹ���
	#define RtosIntLock				VXIntLock
	#define RtosIntUnLock			VXIntUnLock
	#define RtosSetVector			VXSetVector
	#define RtosIntEnable			VXIntEnable
	#define RtosIntDisable			VXIntDisable

	//�ź�������
	//���ݳ�ʼ�������������ͬ�������⼰����ʹ��
	#define RtosSemBCreate			VXBSemCreate
	#define RtosSemGet				VXSemGet
	#define RtosSemRelease			VXSemRelease
	#define RtosSemCCreate			VXCSemCreate
	#define RtosMalloc 				VXMemAlloc//malloc
	
#endif	//endif OS_PLATFORM


#endif
