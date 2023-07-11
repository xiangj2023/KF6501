#ifndef _CANDRV_H_
#define _CANDRV_H_

#include "system.h"

//CAN驱动接口函数定义
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
* CAN初始化														  
*************************************************************************************/
DWORD CanHardInit(WORD port);

/*************************************************************************************
* CAN硬件打开														  
*************************************************************************************/
DWORD CanHardOpen(WORD port);

/*************************************************************************************
* CAN硬件数据发送														  
*************************************************************************************/
DWORD CanHardWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite);

DWORD CanHardReset(WORD port);

#endif
