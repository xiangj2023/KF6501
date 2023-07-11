#ifndef _M5234IIC_H_
#define _M5234IIC_H_

#include "resm5234.h"
#include "system.h"

#define IIC_PRESCALER		0x0d	//75000/160 = 468.75kHz
#define IIC_DELAY			(1000000/468750*20)		//单个字节操作延时

#define IIC_READ			1
#define IIC_WRITE			0

struct TM5234iic{
	BYTE bSlaveDevAddr; 	//从机地址
	BYTE bDevCmdAddr;		//从设备的命令地址（内容由具体的从设备决定）
	BYTE *bIicBuf;			//数据缓冲区，写操作时为写出数据的缓冲区，读操作时为读入数据存放缓冲区
	BYTE bIicDataLen;		//准备写入或读取的数据的长度
	BYTE bCmdAddrEnb;		//命令地址使能控制，读操作时请设置为1，写操作时请设置为2 (会被更改)
	
	BYTE bIicEnd;			//操作结束标志
	BOOL isRead;
//	BYTE bReg;
};

void M5234IICInit(void);
DWORD M5234IICRW(BYTE bDevAddr, BYTE bCmdAddr, BYTE *bBuf, BYTE bLen, BOOL isRead);
void M5234IICInt(void * arg);

#define M5234IICRead(bDevAddr, bCmdAddr, bBuf, bLen)	\
	M5234IICRW(bDevAddr,  bCmdAddr, bBuf, bLen, TRUE)

#define M5234IICWrite(bDevAddr, bCmdAddr, bBuf, bLen)	\
	M5234IICRW(bDevAddr, bCmdAddr, bBuf, bLen, FALSE)

#endif
