#ifndef _ABSOS_H
#define _ABSOS_H

#include "platform.h"
#include "msgdefs.h"
#include "sysdefs.h"
#include "absosdef.h"
#include "memdefs.h"

#define INSERT_HEAD(head,node) \
do{ \
    (node)->pNext = head; \
    head = node; \
}while(0); 

#define REMOVE_HEAD(head,node) \
do{ \
    node = head; \
    head = (head)->pNext; \
    (node)->pNext = NULL; \
}while(0); 

/*------------------------------------------------------------------------------------------*/
/* 功能: 可变内存分配，把分配的没一块内存用链表链接起来                                     */
/* 输入: pphead 可变内存区的链表首指针,unitnum 要分配的单元个数，unitlen要分配的每个单元的  */
/*       长度，分配的实际总长度为:unitnum*unitlen+sizeof(TVMem)                             */
/* 输出: pphead 改变后的可变长度内存区首指针                                                */
/* 返回: 应用程序可以使用的内存区首指针（移区管理表头）                                     */
/*------------------------------------------------------------------------------------------*/
void *OS_GetVMem(TVMem **pphead,WORD unitnum,WORD unitlen);
/*------------------------------------------------------------------------------------------*/
/* 功能: 释放可变内存块                                                                     */
/* 输入: pHead 内存块首指针                                                                 */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_FreeVMem(TVMem *pHead);


/*------------------------------------------------------------------------------------------*/
/* 功能: 在调式模式下,从任务消息队列头中取出一个消息,便于单步跟踪                           */
/* 输入: task 请求的任务描述表                                                              */
/* 输出: 消息指针的地址,=NULL 任务消息队列中没有消息                                        */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_GetTaskMsg(OS_TaskDesp * task,TMessageWrap **msg); 

/*------------------------------------------------------------------------------------------*/
/* 功能: 把收到的消息链入任务消息队列                                                       */
/* 输入: task 任务描述表,msg收到的消息                                                      */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_SetTaskMsg(OS_TaskDesp *task,TMessageWrap* msg) ;

/*------------------------------------------------------------------------------------------*/
/* 功能: 申请空消息槽用于存储收到的消息                                                     */
/* 输入: task 任务描述表                                                                    */
/* 输出: msg 返回空消息槽指针,=NULL申请空消息失败                                           */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void OS_GetEmptyMsg(OS_TaskDesp *task,TMessageWrap **msg); 


/*------------------------------------------------------------------------------------------*/
/* 功能: 释放消息槽                                                                         */
/* 输入: task任务描述表,msg消息槽指针                                                       */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
void OS_FreeMsg(OS_TaskDesp *task,TMessageWrap* msg) ;

void InitMsgWrap();
TMessageWrap *OS_GetSysEmptyMsg();

#endif 

