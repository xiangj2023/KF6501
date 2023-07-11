#ifndef _M5234IIC_H_
#define _M5234IIC_H_

#include "resm5234.h"
#include "system.h"

#define IIC_PRESCALER		0x0d	//75000/160 = 468.75kHz
#define IIC_DELAY			(1000000/468750*20)		//�����ֽڲ�����ʱ

#define IIC_READ			1
#define IIC_WRITE			0

struct TM5234iic{
	BYTE bSlaveDevAddr; 	//�ӻ���ַ
	BYTE bDevCmdAddr;		//���豸�������ַ�������ɾ���Ĵ��豸������
	BYTE *bIicBuf;			//���ݻ�������д����ʱΪд�����ݵĻ�������������ʱΪ�������ݴ�Ż�����
	BYTE bIicDataLen;		//׼��д����ȡ�����ݵĳ���
	BYTE bCmdAddrEnb;		//�����ַʹ�ܿ��ƣ�������ʱ������Ϊ1��д����ʱ������Ϊ2 (�ᱻ����)
	
	BYTE bIicEnd;			//����������־
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
