#ifndef _MEMDEFS_H
#define _MEMDEFS_H

//#include "clinkdefs.h"
//#include "syssel.h"
#include "sysdefs.h"

#define MM_PRIVATEMEMORY   	0x12000   /*64k  */
#define MM_SIGN         	0x12345678   /*ϵͳʹ�õ��ڴ���Ч��־*/
#define MM_FROMREGION       0xFFFF

//����˫������
typedef struct{
  	struct node *Next;      //��һ���ڵ�ָ��
  	struct node *Prev;      //ǰһ���ڵ�ָ��
}TNode;

//����˫���������ṹ
typedef struct tlink{

    //˫�������׽ڵ�
    struct node *m_pLink;

    //��ǰ����ڵ�
    struct node *m_pCurrent;

    //����������������Ľڵ���,=0��ʾ����Ľڵ������������
    DWORD m_nMaxNode;

    //����ڵ����
    DWORD m_nNode;
}TLink;

/*-----------------------------------------------------------------------------------------*/
/* ��̬�ڴ��������ڵ�                                                                    */
/*-----------------------------------------------------------------------------------------*/
typedef struct memctrblock{
	TLink			Link;					
	DWORD			nBlocks;				/* ������Ŀ����*/
	DWORD			nSize;					/* ÿ����Ĵ�С*/
	DWORD			nCount;					/* �Ѿ�����Ŀ���*/
}CMemCtrBlock;

/*-----------------------------------------------------------------------------------------*/
/*��̬�����ڴ�ͷ,���ڱ�ʾ�ÿ��������Դ,��С������                                         */
/*-----------------------------------------------------------------------------------------*/
typedef struct cmemheader
{
  	TNode  Node;								/*�ڴ������ڵ�t*/
	#if (__DEBUG_MEMORY_T == YES)
  	BYTE   szTaskName[4];						/*�ڵ���ģʽ�¼�¼�����ڴ����������*/
	#endif
  	DWORD  dwFlag;							/*�ڴ���0x5743514d*/
  	DWORD  dwAttri;							/*��λ�ּ�¼�ÿ��ڴ�������ڵ��,=0XFFFF��ʾ���ڴ��ϵͳ������*/										/*��λ��Ϊ���ڴ����ĳ���*/
}TMemHeader;


//�ɱ��ڴ�������
typedef struct tvmem
{
    struct tvmem *pNext;       //ָ����һ��������ڴ���
}TVMem;

//���嶯̬�ڴ���ƿ飬������ϵͳ������Ϣ�ڵ㣬��ʱ���ڵ㣬Hash��ڵ�Ĺ���
typedef struct dmemblock
{
	 struct tvmem  *pMem;           //�ɱ��ڴ����ָ��
     struct tvmem  *pFreeList;      //���нڵ�����
     DWORD  UsedUnitNum;            //�ѷ���ĵ�Ԫ����
	 WORD  	UnitNumOfPerBlock;       //ÿ��ĵ�Ԫ����
	 WORD  	UnitLen;                 //ÿ����Ԫ�ĳ���
	 OSINT8	SemId;                   //ƽ̨�����ź�����ʶ��
}dMemBlock;

#endif
