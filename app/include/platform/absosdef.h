#ifndef _ABSOSDEF_H
#define _ABSOSDEF_H

#include "msgdefs.h"
#include "sysdefs.h"

#define TASK_HANDLE     (1<<16)

#define MAX_TASK_NAME_LEN	8    	//�������Ƶ�����ַ���
#define MAX_PERTASKFREEMESSAGE	16	//����ÿ���������������Ϣ���и���	
#define MAX_TASK_REGISTER	4	//����ÿ���������ļĴ�������	

#define TASK_HANDLE     (1<<16)

#define SELF_TASK_ID		0x0	//��������id��־
#define	APP_TASKID_NULL		0x55AA33CC	//��Ч����ID

#define MAX_TASK_ARG		4	//������󴫵ݲ���

//��������Ĵ�����ŵ���������
#define REG_MSG_SEM			0	//������Ϣ�����ź���
#define REG_TASK_STACK		1	//�����ջָ��
#define REG_TASK_ID			2	//����ϵͳ����ID

#define MAX_TIMER       8   //ÿ����������Ķ�ʱ������

//��ʱ�������ṹ
typedef struct timerdesp{
    TTimerHandle    hTimer;     /*��ʱ�����*/
    DWORD           dwSign;     /*��ʱ����ʶ*/
    DWORD           dwEvent;    /*�ö�ʱ����Ӧ�����ʹ��pSOS��ʱ��ʱʹ�ã�����RTOS��������*/
    TTimerFunc      fProc;      /*�ö�ʱ������ĺ���ָ��*/
    void            *pPara;     /*��ʱ�������Դ�һ���û���������*/
}TTimerDesp;

/*��ʱ�����ڵ�*/
typedef struct timerwrap{
	struct timerwrap *pNext;          /*ָ����һ����ʱ��*/
	struct timerdesp TimerDesp;       /*��ʱ������*/
}TTimerWrap;

//��Ϣ�ڵ㶨��
typedef struct messagewrap{
	struct messagewrap *pNext;
	struct message     Msg;
}TMessageWrap;

//��������������
typedef struct os_taskdesp{

	//��������
	STRING TaskName[MAX_TASK_NAME_LEN];
	
	//�������ȼ�
	OSINT8	Priority;

    DWORD   dwTaskID;		//����Ӧ�ò�ID

	//�����ջ
	OSINT16	StackSize;

	//������������ʱ��(��λΪ��)
	DWORD dwRunTime;

	//���������ڴ��ܳ���
	DWORD dwAllocMemLen;

	//����ָ��
	TTASK_ENTRY pTaskEntry;

    //��Ӧ�ڸ��ֲ���ϵͳ����Ϣ�������RTOSʹ��
    TQueueHandle hQueue;

	//��������
	DWORD	Args[MAX_TASK_ARG];


	//ָ����һ�������������ָ��
	struct os_taskdesp	 *pNextTask;

    	//������Ϣ������ָ��
    TMessageWrap *pFront;

    	//������Ϣ����βָ��
    TMessageWrap *pLast;

    	//���нڵ�����
    TMessageWrap *pFreeList;
	
	//�������
	OSINT16	Counter;

	//����ʱ������
	WORD wTimerNum;

    //������Ч��־
    OSINT8	IsDeleted;   
    
    //�������Ĵ���,�ֱ𱣴�������Ϣ�����ź����������ջָ��(����UCOS��Ч��ɾ������ʱ��Ҫ����
    //����ɾ��)������ϵͳ����ID
    DWORD   Register[MAX_TASK_REGISTER];

	//��ʱ��ID
	DWORD dwTimeTab[MAX_TIMER];

    //������ж��б�ÿ��������һ�����ж���
	#if(MAX_PERTASKFREEMESSAGE > 0)
    	struct messagewrap amsgFree[MAX_PERTASKFREEMESSAGE];
	#endif

}OS_TaskDesp;

	
//������ͨ��ʱ���ڵ�
typedef struct os_timerdesp{

	//���������������ָ��
	struct os_taskdesp *pTask;

	//��ʱ����ʶ��,�����ڲ��ã���ͬ�������ظ�
	OSINT8	Sign;

	//��ǰ����ֵ
	OSINT16	CurTime;

	//��ʼ����ֵ
	OSINT16	InitTime;

	DWORD dwEvtBit;

	//ָ����һ����ʱ���ڵ��ָ��
	struct os_timerdesp *pNextTimer;

}OS_TimerDesp;

//�������ⶨʱ��(��ָ��ʱ��㿪ʼ��ʱ��)�ڵ�
struct SpecialTimer
{
	//���������������ָ��
	struct os_taskdesp *pTask;
	
	//��ʱ����ʶ��,�����ڲ��ã���ͬ�������ظ�
	OSINT8	Sign;

	//��ʱ����ʼʱ��
	DWORD BeginTime;
	
	OSINT16	Interval;		//ע���ʱ������λΪ����
	
	struct SpecialTimer *pNextTimer;
};
		
#endif
