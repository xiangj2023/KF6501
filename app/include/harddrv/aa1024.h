#ifndef _AA1024_H_
#define _AA1024_H_

#if(EEPROM_MODULE == AA1024)

#include "resm5234.h"
#include "system.h"

#define AA1024_PAGE_LEN		256		//AA1024页长度定义

#define AA1024_LEN			0x20000	//AA1024总长度

#define AA1024_STSREG_DELAY	3		//读状态寄存器延时

//定义操作命令
#define AA1024_READ			0x03
#define AA1024_WRITE		0x02
#define AA1024_WREN			0x06
#define AA1024_WRDI			0x04
#define AA1024_RDSR			0x05
#define AA1024_WRSR			0x01
#define AA1024_DPD			0xb9	//进入低功耗模式
#define AA1024_RDPD			0xab	//退出低功耗模式
#define AA1024_CE			0xc7	//片擦除

//BP0/BP1控制命令定义
#define AA1024_BLOCK_PT		0x8c	//块保护有效
#define AA1024_BLOCK_UNPT	0x80	//块保护无效	

//定义状态寄存器状态位
#define AA1024_STS_WIP		0x01	//正在写操作
#define AA1024_STS_WEL		0x02	//写使能
#define AA1024_STS_BP0		0x04	//块保护
#define AA1024_STS_BP1		0x08	//块保护
#define AA1024_STS_WPEN		0x80	//写保护使能

//WP引脚控制操作定义
#define AA1024WP()		outportb(EEPROM_PRO, 0)		//写保护
#define AA1024WUnP()	outportb(EEPROM_PRO, 1)	//写允许

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
