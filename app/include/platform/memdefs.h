#ifndef _MEMDEFS_H
#define _MEMDEFS_H

//#include "clinkdefs.h"
//#include "syssel.h"
#include "sysdefs.h"

#define MM_PRIVATEMEMORY   	0x12000   /*64k  */
#define MM_SIGN         	0x12345678   /*系统使用的内存有效标志*/
#define MM_FROMREGION       0xFFFF

//定义双向链表
typedef struct{
  	struct node *Next;      //后一个节点指针
  	struct node *Prev;      //前一个节点指针
}TNode;

//定义双向链表管理结构
typedef struct tlink{

    //双向链表首节点
    struct node *m_pLink;

    //当前链表节点
    struct node *m_pCurrent;

    //该链表最大可以联结的节点数,=0表示联结的节点个数不受限制
    DWORD m_nMaxNode;

    //链表节点计数
    DWORD m_nNode;
}TLink;

/*-----------------------------------------------------------------------------------------*/
/* 动态内存链表管理节点                                                                    */
/*-----------------------------------------------------------------------------------------*/
typedef struct memctrblock{
	TLink			Link;					
	DWORD			nBlocks;				/* 该区间的块个数*/
	DWORD			nSize;					/* 每个块的大小*/
	DWORD			nCount;					/* 已经分配的块数*/
}CMemCtrBlock;

/*-----------------------------------------------------------------------------------------*/
/*动态分配内存头,用于表示该块区域的来源,大小等属性                                         */
/*-----------------------------------------------------------------------------------------*/
typedef struct cmemheader
{
  	TNode  Node;								/*内存块链表节点t*/
	#if (__DEBUG_MEMORY_T == YES)
  	BYTE   szTaskName[4];						/*在调试模式下记录请求内存的任务名称*/
	#endif
  	DWORD  dwFlag;							/*内存标记0x5743514d*/
  	DWORD  dwAttri;							/*低位字记录该块内存的所属节点号,=0XFFFF表示该内存从系统区分配*/										/*高位字为本内存区的长度*/
}TMemHeader;


//可变内存区定义
typedef struct tvmem
{
    struct tvmem *pNext;       //指向下一个分配的内存区
}TVMem;

//定义动态内存控制块，可用于系统空闲消息节点，定时器节点，Hash表节点的管理
typedef struct dmemblock
{
	 struct tvmem  *pMem;           //可变内存管理指针
     struct tvmem  *pFreeList;      //空闲节点链表
     DWORD  UsedUnitNum;            //已分配的单元个数
	 WORD  	UnitNumOfPerBlock;       //每块的单元个数
	 WORD  	UnitLen;                 //每个单元的长度
	 OSINT8	SemId;                   //平台级的信号量标识号
}dMemBlock;

#endif
