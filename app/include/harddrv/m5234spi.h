#ifndef _M5234SPI_H_
#define _M5234SPI_H_

#include "resm5234.h"
#include "system.h"

//����SPI��ģ���Ӧ��ַ����Ҫ����Ӳ��Ƭѡ��ַȷ��
#define AA1024_CS_ADDR			0x0e
#define AD7782_CS_ADDR			0x0f

#define SPI_RATE_25AA1024_DIV			3		//SPIʱ�ӷ�Ƶ��
#define SPI_RATE_AD7782_DIV				10		//���ʱ��Ƶ��5MHz

#define SPI_READ	0x55		//��spi����
#define SPI_WRITE	0xaa		//дspi����

#define SPI_FIFO_LEN	16		
#define SPI_CS_NUM		16		//SPI���Ƭѡ����

//����SPI�ĸ���RAM�Ĵ�����ַ
#define SPI_TXD_RAM_BASE	0x00
#define SPI_RXD_RAM_BASE	0x10
#define SPI_CMD_RAM_BASE	0x20

struct TM5234Spi
{
	BYTE bCmdMode;		//�����������ģʽ������д��
	BYTE bEndFlag;		//�����������������־
	BYTE bCsAdd;		//Ƭѡ��ַ
//	BYTE SpiFlag;		//
	DWORD dwCmdLen;		//������Ϣ����
	DWORD dwDataLen;		//׼����ȡ��д��������Ϣ����
	
	BYTE *bCmdBuf;		//������Ϣ������
	BYTE *bDataBuf;		//������Ϣ������
	BOOL isFirst;		//�Ƿ�Ϊ��һ�η��ͣ����ڶ�������Ч����Ϊ���ڶ���������һ��ֻ��������,
						//�ٶ�������Ϣ����һ�η������
};

//Ƭѡ����Ч, cs0ΪAA1024Ƭѡ
#define SetSpiCs0Active()	MCF_GPIO_PODR_QSPI &= (~0x08)
#define SetSpiCs0Disact()	MCF_GPIO_PODR_QSPI |= 0x08

//cs1Ϊ����ADƬѡ
#define SetSpiCs1Active()	MCF_GPIO_PODR_QSPI &= (~0x10)
#define SetSpiCs1Disact()	MCF_GPIO_PODR_QSPI |= 0x10

//SPI����ģʽ���ã����ݴ�ģ���Ҫ���������,25AA1024֧��00��11ģʽ��ѡ��00ģʽ
#define SetAA1024Mode()		MCF_QSPI_QMR = SPI_RATE_25AA1024_DIV|MCF_QSPI_QMR_MSTR\
											|MCF_QSPI_QMR_DOHIE|MCF_QSPI_QMR_CPHA|MCF_QSPI_QMR_CPOL
//AD7782֧��11ģʽ��ѡ��11ģʽ
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
