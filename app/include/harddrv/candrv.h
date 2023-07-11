#ifndef _CANDRV_H_
#define _CANDRV_H_

#include "system.h"

//CAN�����ӿں�������
typedef DWORD (*FCanInit)(WORD);
typedef DWORD (*FCanOpen)(WORD);
typedef DWORD (*FCanRead)();
typedef DWORD (*FCanWrite)(WORD, BYTE, BYTE, BOOL);
typedef DWORD (*FCanReset)(WORD);

struct TCanDriver
{
	FCanInit pInit;
	FCanOpen pOpen;
	FCanRead pRead;
	FCanWrite pWrite;
	FCanReset pReset;
};

/*************************************************************************************
* CAN��ʼ��														  
*************************************************************************************/
DWORD CanHardInit(WORD port);

/*************************************************************************************
* CANӲ����														  
*************************************************************************************/
DWORD CanHardOpen(WORD port);

/*************************************************************************************
* CANӲ�����ݷ���														  
*************************************************************************************/
DWORD CanHardWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite);

DWORD CanHardReset(WORD port);

#endif
