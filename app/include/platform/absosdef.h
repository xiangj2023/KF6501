#ifndef _ABSOSDEF_H
#define _ABSOSDEF_H

#include "msgdefs.h"
#include "sysdefs.h"

#define TASK_HANDLE     (1<<16)

#define MAX_TASK_NAME_LEN	8    	//任务名称的最多字符数
#define MAX_PERTASKFREEMESSAGE	16	//定义每个任务基本空闲消息队列个数	
#define MAX_TASK_REGISTER	4	//定义每个任务保留的寄存器个数	

#define TASK_HANDLE     (1<<16)

#define SELF_TASK_ID		0x0	//自身任务id标志
#define	APP_TASKID_NULL		0x55AA33CC	//无效任务ID

#define MAX_TASK_ARG		4	//任务最大传递参数

//定义任务寄存器存放的数据内容
#define REG_MSG_SEM			0	//任务消息队列信号量
#define REG_TASK_STACK		1	//任务堆栈指针
#define REG_TASK_ID			2	//操作系统任务ID

#define MAX_TIMER       8   //每个任务允许的定时器个数

//定时器描述结构
typedef struct timerdesp{
    TTimerHandle    hTimer;     /*定时器句柄*/
    DWORD           dwSign;     /*定时器标识*/
    DWORD           dwEvent;    /*该定时器对应的事项，使用pSOS定时器时使用，其它RTOS可做它用*/
    TTimerFunc      fProc;      /*该定时器对象的函数指针*/
    void            *pPara;     /*定时函数可以带一个用户描述参数*/
}TTimerDesp;

/*定时器链节点*/
typedef struct timerwrap{
	struct timerwrap *pNext;          /*指向下一个定时器*/
	struct timerdesp TimerDesp;       /*定时器描述*/
}TTimerWrap;

//消息节点定义
typedef struct messagewrap{
	struct messagewrap *pNext;
	struct message     Msg;
}TMessageWrap;

//定义任务描述表
typedef struct os_taskdesp{

	//任务名称
	STRING TaskName[MAX_TASK_NAME_LEN];
	
	//任务优先级
	OSINT8	Priority;

    DWORD   dwTaskID;		//任务应用层ID

	//任务堆栈
	OSINT16	StackSize;

	//任务任务运行时间(单位为秒)
	DWORD dwRunTime;

	//任务分配的内存总长度
	DWORD dwAllocMemLen;

	//函数指针
	TTASK_ENTRY pTaskEntry;

    //适应于各种操作系统的消息句柄，给RTOS使用
    TQueueHandle hQueue;

	//函数参数
	DWORD	Args[MAX_TASK_ARG];


	//指向下一个任务描述表的指针
	struct os_taskdesp	 *pNextTask;

    	//任务消息链表首指针
    TMessageWrap *pFront;

    	//任务消息链表尾指针
    TMessageWrap *pLast;

    	//空闲节点链表
    TMessageWrap *pFreeList;
	
	//任务计数
	OSINT16	Counter;

	//任务定时器个数
	WORD wTimerNum;

    //任务有效标志
    OSINT8	IsDeleted;   
    
    //任务工作寄存器,分别保存任务消息队列信号量、任务堆栈指针(对于UCOS有效，删除任务时需要单独
    //单独删除)、操作系统任务ID
    DWORD   Register[MAX_TASK_REGISTER];

	//定时器ID
	DWORD dwTimeTab[MAX_TIMER];

    //任务空闲队列表，每个任务保留一个空闲队列
	#if(MAX_PERTASKFREEMESSAGE > 0)
    	struct messagewrap amsgFree[MAX_PERTASKFREEMESSAGE];
	#endif

}OS_TaskDesp;

	
//定义普通定时器节点
typedef struct os_timerdesp{

	//所属任务的描述表指针
	struct os_taskdesp *pTask;

	//定时器标识号,任务内部用，不同任务间可重复
	OSINT8	Sign;

	//当前计数值
	OSINT16	CurTime;

	//初始计数值
	OSINT16	InitTime;

	DWORD dwEvtBit;

	//指向下一个定时器节点的指针
	struct os_timerdesp *pNextTimer;

}OS_TimerDesp;

//定义特殊定时器(从指定时间点开始等时间)节点
struct SpecialTimer
{
	//所属任务的描述表指针
	struct os_taskdesp *pTask;
	
	//定时器标识号,任务内部用，不同任务间可重复
	OSINT8	Sign;

	//定时器开始时间
	DWORD BeginTime;
	
	OSINT16	Interval;		//注意此时间间隔单位为分钟
	
	struct SpecialTimer *pNextTimer;
};
		
#endif
