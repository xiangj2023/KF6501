#ifndef _AA1024_H_
#define _AA1024_H_

#if(EEPROM_MODULE == AA1024)

#include "resm5234.h"
#include "system.h"

#define AA1024_PAGE_LEN		256		//AA1024ҳ���ȶ���

#define AA1024_LEN			0x20000	//AA1024�ܳ���

#define AA1024_STSREG_DELAY	3		//��״̬�Ĵ�����ʱ

//�����������
#define AA1024_READ			0x03
#define AA1024_WRITE		0x02
#define AA1024_WREN			0x06
#define AA1024_WRDI			0x04
#define AA1024_RDSR			0x05
#define AA1024_WRSR			0x01
#define AA1024_DPD			0xb9	//����͹���ģʽ
#define AA1024_RDPD			0xab	//�˳��͹���ģʽ
#define AA1024_CE			0xc7	//Ƭ����

//BP0/BP1���������
#define AA1024_BLOCK_PT		0x8c	//�鱣����Ч
#define AA1024_BLOCK_UNPT	0x80	//�鱣����Ч	

//����״̬�Ĵ���״̬λ
#define AA1024_STS_WIP		0x01	//����д����
#define AA1024_STS_WEL		0x02	//дʹ��
#define AA1024_STS_BP0		0x04	//�鱣��
#define AA1024_STS_BP1		0x08	//�鱣��
#define AA1024_STS_WPEN		0x80	//д����ʹ��

//WP���ſ��Ʋ�������
#define AA1024WP()		outportb(EEPROM_PRO, 0)		//д����
#define AA1024WUnP()	outportb(EEPROM_PRO, 1)	//д����

DWORD AA1024WEnable(BOOL isEnable);

DWORD AA1024ReadStatus(BYTE *status);
DWORD AA1024WriteStatus(BYTE status);
DWORD AA1024WriteBP(BYTE data);
DWORD AA1024DPDInOut(BOOL isIn);
DWORD AA1024PageRead(BYTE *pBuf, DWORD len, DWORD address);
DWORD AA1024PageWrite(BYTE *pBuf, DWORD len, DWORD address);
DWORD AA1024Read(BYTE *pBuf, DWORD len, DWORD address);
DWORD AA1024Write(BYTE *pBuf, DWORD len, DWORD address);
DWORD AA1024ChipErase();
DWORD EepromRead(BYTE *pBuf, DWORD len, DWORD address);
DWORD EEPROMWrite(BYTE *pBuf, DWORD len, DWORD address);
DWORD EEPROMChipErase();
BOOL DataCmp(BYTE *pData1, BYTE *pData2, DWORD len);

#endif

#endif
