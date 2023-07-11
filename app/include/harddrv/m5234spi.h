#ifndef _M5234SPI_H_
#define _M5234SPI_H_

#include "resm5234.h"
#include "system.h"

//定义SPI从模块对应地址，需要根据硬件片选地址确定
#define AA1024_CS_ADDR			0x0e
#define AD7782_CS_ADDR			0x0f

#define SPI_RATE_25AA1024_DIV			3		//SPI时钟分频数
#define SPI_RATE_AD7782_DIV				10		//最大时钟频率5MHz

#define SPI_READ	0x55		//读spi外设
#define SPI_WRITE	0xaa		//写spi外设

#define SPI_FIFO_LEN	16		
#define SPI_CS_NUM		16		//SPI最大片选个数

//定义SPI的各种RAM寄存器基址
#define SPI_TXD_RAM_BASE	0x00
#define SPI_RXD_RAM_BASE	0x10
#define SPI_CMD_RAM_BASE	0x20

struct TM5234Spi
{
	BYTE bCmdMode;		//本次命令操作模式（读或写）
	BYTE bEndFlag;		//本次命令操作结束标志
	BYTE bCsAdd;		//片选地址
//	BYTE SpiFlag;		//
	DWORD dwCmdLen;		//命令信息长度
	DWORD dwDataLen;		//准备读取或写入数据信息长度
	
	BYTE *bCmdBuf;		//命令信息缓冲区
	BYTE *bDataBuf;		//数据信息缓冲区
	BOOL isFirst;		//是否为第一次发送，对于读操作有效，因为对于读操作，第一次只发送命令,
						//假定命令信息可以一次发送完成
};

//片选低有效, cs0为AA1024片选
#define SetSpiCs0Active()	MCF_GPIO_PODR_QSPI &= (~0x08)
#define SetSpiCs0Disact()	MCF_GPIO_PODR_QSPI |= 0x08

//cs1为串行AD片选
#define SetSpiCs1Active()	MCF_GPIO_PODR_QSPI &= (~0x10)
#define SetSpiCs1Disact()	MCF_GPIO_PODR_QSPI |= 0x10

//SPI工作模式设置，根据从模块的要求进行设置,25AA1024支持00和11模式，选择00模式
#define SetAA1024Mode()		MCF_QSPI_QMR = SPI_RATE_25AA1024_DIV|MCF_QSPI_QMR_MSTR\
											|MCF_QSPI_QMR_DOHIE|MCF_QSPI_QMR_CPHA|MCF_QSPI_QMR_CPOL
//AD7782支持11模式，选择11模式
#define SetAD7782Mode()		MCF_QSPI_QMR = SPI_RATE_AD7782_DIV|MCF_QSPI_QMR_MSTR\
											|MCF_QSPI_QMR_DOHIE|MCF_QSPI_QMR_CPHA|MCF_QSPI_QMR_CPOL

#define SpiCsActive(cs)	\
	do{\
		if(cs==AA1024_CS_ADDR)\
			SetSpiCs0Active();\
		else\
		{\
			SetSpiCs1Active();\
			OS_Sleep(150);\
		}\
	}while(0)

#define SpiCsDisact(cs)	\
	do{\
		if(cs==AA1024_CS_ADDR)\
			SetSpiCs0Disact();\
		else\
			SetSpiCs1Disact();\
	}while(0)


void M5234SpiInit(void);
void M5234SpiInt(void * arg);
//DWORD M5234SpiOperate(BYTE bChip,BYTE *bCmdBuf,BYTE bCmdLen,BYTE *bDataBuf=NULL,WORD wDataLen=0,BYTE bCmd=SPI_WRITE);
DWORD M5234SpiOperate(BYTE bChip,BYTE *bCmdBuf, DWORD dwCmdLen, BYTE *bDataBuf, DWORD dwDataLen, BYTE bCmd);
void M5234SpiReset();


/*	
#define StartADC1()	MCF_GPIO_PODR_TIMER |= MCF_GPIO_PODR_TIMER_PODR_TIMER2
#define StartADC2()	MCF_GPIO_PODR_TIMER |= MCF_GPIO_PODR_TIMER_PODR_TIMER6
#define StopADC1()	MCF_GPIO_PODR_TIMER &= (~MCF_GPIO_PODR_TIMER_PODR_TIMER2)
#define StopADC2()	MCF_GPIO_PODR_TIMER &= (~MCF_GPIO_PODR_TIMER_PODR_TIMER6)
	
void Max11046Init();
void ADCSampStart();
void ADInt(void * arg);
*/

#ifdef _DBUG_ADC_

#define ADC_BUF_LEN			20
oid ADCHardInit(void);
void ADCSampStart(void);
void ADCSampInt(void * arg);

#endif

#endif
