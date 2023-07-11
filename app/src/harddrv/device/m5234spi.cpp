/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			m5234spi.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           m5234��SPI�������룬�Լ�25AA1024���йز��Դ���                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      �ṩIIC���йس�ʼ������д���ƵȲ����ӿ�                          
*                                                                                  
* ����                                                                             
*                                                                                  
*      	M5234SpiInit		SPI��ʼ��
*		M5234SpiOperate		SPI��д�����ӿ�
*		M5234SpiInt			SPI�жϴ�����
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/11/26    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "m5234spi.h"

TM5234Spi M5234SpiBuf;		//spi��������

/*********************************************************************************************
 *
 * ��������     SPI��ʼ�������ö�Ӧ���ţ�����ʱ��Ƶ�ʣ������ж�,ƬѡΪI/O��ʽ����
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234SpiInit(void)
{
	M5234SpiReset();
/*	
	memset((void *)&M5234SpiBuf, 0, sizeof(TM5234Spi));
	MCF_GPIO_PAR_QSPI = 0x1f;	//����CS0��CS1��CLK��DIN��DOUT���ţ�����CS0/CS1ΪIO��ʽ
	
	MCF_GPIO_PDDR_QSPI |= 0x18;	//����ƬѡΪ���ģʽ
	
	SetSpiCs0Disact();			//����25AA1024ƬѡΪ��Ч״̬
	SetSpiCs1Disact();			//����AD7782ƬѡΪ��Ч״̬
	
	// ���ù���ģʽ���������ݼ�϶���衢CLK��Ч״̬Ϊ�͡��������������ݣ�ʱ��18.75MHz��8λ����
	MCF_QSPI_QMR = SPI_RATE_DIV|MCF_QSPI_QMR_MSTR|MCF_QSPI_QMR_DOHIE;

	//���÷�����ʱ��������������ʱ
	MCF_QSPI_QDLYR = 0x0;
*/
	OS_SetVector(MCF_INTC0_LVL_SPI, M5234SpiInt, 0);	//�жϼ���
	OS_IntEnable(MCF_INTC0_LVL_SPI);

	//�����ж����ȼ�
	MCF_INTC0_ICRn(18) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(4);

	//�����жϱ�־λ,��������ֱ�־��Ϣ
//	MCF_QSPI_QIR = 0xDD0D;
	
}

/*********************************************************************************************
 *
 * ��������     SPI��λ
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234SpiReset()
{
	memset((void *)&M5234SpiBuf, 0, sizeof(TM5234Spi));
	MCF_GPIO_PAR_QSPI = 0x1f;	//����CS0��CS1��CLK��DIN��DOUT���ţ�����CS0/CS1ΪIO��ʽ
	
	MCF_GPIO_PDDR_QSPI |= 0x18;	//����ƬѡΪ���ģʽ
	
	SetSpiCs0Disact();			//����25AA1024ƬѡΪ��Ч״̬
	SetSpiCs1Disact();			//����AD7782ƬѡΪ��Ч״̬
	
	// ���ù���ģʽ���������ݼ�϶���衢CLK��Ч״̬Ϊ�͡��������������ݣ�ʱ��18.75MHz��8λ����
//	MCF_QSPI_QMR = SPI_RATE_DIV|MCF_QSPI_QMR_MSTR|MCF_QSPI_QMR_DOHIE;

	MCF_QSPI_QMR = SPI_RATE_25AA1024_DIV|MCF_QSPI_QMR_MSTR|MCF_QSPI_QMR_DOHIE\
												|MCF_QSPI_QMR_CPHA|MCF_QSPI_QMR_CPOL;

	//���÷�����ʱ��������������ʱ
	MCF_QSPI_QDLYR = 0x0;
	
	//�����жϱ�־λ,��������ֱ�־��Ϣ
	MCF_QSPI_QIR = 0xDD0D;
	
}

/*********************************************************************************************
 *
 * ��������     SPI��д�����ӿڣ���ɶ�д����
 *
 * ����˵��      - bChip	: ���� 	���豸��ַ
 *				 - bCmdBuf	: ����	��������ָ��
 *				 - dwCmdLen	: ����  ���������
 *				 - bDataBuf	: ˫��	д����ȡ������ָ��
 *				 - dwDataLen: ����	д����ȡ�����ݳ���
 *				 - bCmd		: ����	��������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234SpiOperate(BYTE bChip,BYTE *bCmdBuf, DWORD dwCmdLen, BYTE *bDataBuf, DWORD dwDataLen, BYTE bCmd)
{
	DWORD address;
	int i, delay;
	
	//Ƭѡ��ַ�Ϸ��Լ��
	ASSERT(bChip<SPI_CS_NUM);
	if(bChip>=SPI_CS_NUM)
		return ERR_FAILURE;

	//�ٽ���Դ����,���Ƕ��SPI��������
//    OS_LockM(SPI_LOCK);

	taskLock();
	
	M5234SpiBuf.bCmdMode = bCmd;	//���ñ��β����Ĳ���ģʽ
	M5234SpiBuf.dwCmdLen = dwCmdLen;
	M5234SpiBuf.dwDataLen = dwDataLen;
	M5234SpiBuf.bCmdBuf = bCmdBuf;
	M5234SpiBuf.bDataBuf = bDataBuf;
	M5234SpiBuf.bCsAdd = bChip;
	
	M5234SpiBuf.bEndFlag = 0;

	//��������Ĵ�����Ϣ
	address = bChip;
	address <<= 8;
//	address |= 0x8000;//0x3000;	Ƭѡ������Ч����������ʱ

	//��ָ���ƶ�������RAM��ʼ
	MCF_QSPI_QAR = SPI_CMD_RAM_BASE;
	//����д����������
	for(i=0; i<SPI_FIFO_LEN; i++)
		MCF_QSPI_QDR = address;

	//д�뷢������
	MCF_QSPI_QAR = SPI_TXD_RAM_BASE;	//��ָ���ƶ�������RAM��ʼ
	for(i=0; i<SPI_FIFO_LEN; i++)		//���һ��д��16����������
	{
		if(M5234SpiBuf.dwCmdLen)			//����д������
		{
			MCF_QSPI_QDR = *M5234SpiBuf.bCmdBuf++;
			M5234SpiBuf.dwCmdLen--;
		}
		else
		{
			if( (bCmd==SPI_WRITE) && (M5234SpiBuf.dwDataLen) )	//д�����������ݷ������������
			{
				MCF_QSPI_QDR = *M5234SpiBuf.bDataBuf++;
				M5234SpiBuf.dwDataLen--;
			}
			else
				break;
		}
	}

	//����Ƭѡ��Ч
	SpiCsActive(M5234SpiBuf.bCsAdd);	
	
	//���ڶ�����������������Ϣ����һ�η�����ɡ�
	if(bCmd==SPI_READ)
		M5234SpiBuf.isFirst = TRUE;

	
	//����д�뷢�ͻ������ĸ������ö���βָ�룬������CS����Ч
	MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV|((i-1)<<8);

	//SPIʹ��
	MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;

	//ѭ���ȴ�������ݲ����Ƿ�ɹ�
	delay = (dwCmdLen+dwDataLen)*100;
	int number = 10000;
	for(i=0; i<delay; i++)
	{
		SysDelay(number);
		if(M5234SpiBuf.bEndFlag == 0xff)
		{
//			OS_UnLockM(SPI_LOCK);
			return ERR_SUCCESS;
		}

		//��ʱ�ָ�ϵͳ����
		if(delay == 5)
			taskUnlock();
	}
	
	//����Ƭѡ��Ч
	SpiCsDisact(M5234SpiBuf.bCsAdd);	
	
//    OS_UnLockM(SPI_LOCK);
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * ��������     SPI�жϴ�����
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234SpiInt(void * arg)
{
	WORD status;
	int i, len, intmask;

	intmask = intLock();
	//��ֹSPI����
	MCF_QSPI_QDLYR &= ~MCF_QSPI_QDLYR_SPE;

	status = MCF_QSPI_QIR;	//���жϼĴ���
	MCF_QSPI_QIR |= 0x000d;	//����жϼĴ�������Ч��־

	if( (status&0x000d) == MCF_QSPI_QIR_SPIF)	//ֻ��SPIF��λ��˵������
	{
		//��������Ƿ��շ����
		if(M5234SpiBuf.dwDataLen == 0)
		{
			if(M5234SpiBuf.bCmdMode==SPI_WRITE)	//�����д��������˵��д������������
			{
				M5234SpiBuf.bEndFlag = 0xff;	//��������������־
				intUnlock(intmask);
				taskUnlock();
				SpiCsDisact(M5234SpiBuf.bCsAdd);	
				return;
			}
		}
		//д�����������ݷ������������
		if(M5234SpiBuf.bCmdMode==SPI_WRITE)	
		{
			MCF_QSPI_QAR = SPI_TXD_RAM_BASE;	//��ָ���ƶ�������RAM��ʼ
			for(i=0; i<SPI_FIFO_LEN; i++)
			{
				if(M5234SpiBuf.dwDataLen) 
				{
					MCF_QSPI_QDR = *M5234SpiBuf.bDataBuf++;
					M5234SpiBuf.dwDataLen--;
				}
				else
					break;
			}
			
			//����д�뷢�ͻ������ĸ������ö���βָ�룬������CS����Ч
			MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV|((i-1)<<8);
		}
		else
		{
			if(M5234SpiBuf.isFirst == FALSE)	//��ȡ���յ�������
			{
				len = MCF_QSPI_QWR & 0x0f00;
				len >>= 8;
				MCF_QSPI_QAR = SPI_RXD_RAM_BASE;	//��ָ���ƶ�������RAM��ʼ
				for(i=0; i<=len; i++)
				{
					if(M5234SpiBuf.dwDataLen)
					{
						*M5234SpiBuf.bDataBuf++ = MCF_QSPI_QDR;
						M5234SpiBuf.dwDataLen--;
					}
					else
						break;
				}
			}
			else	//��һ��ֻ������������Ϣ������Ҫ���մ���(���Ƕ���AA7782,ʱ����Чʱ�ͷ�������)��
			{
				if(M5234SpiBuf.bCsAdd == AD7782_CS_ADDR)
				{
					//�˴�ȡ���������AD7782����
					len = MCF_QSPI_QWR & 0x0f00;
					len >>= 8;
					MCF_QSPI_QAR = SPI_RXD_RAM_BASE;	//��ָ���ƶ�������RAM��ʼ
					for(i=0; i<=len; i++)
					{
						if(M5234SpiBuf.dwDataLen)
						{
							*M5234SpiBuf.bDataBuf++ = MCF_QSPI_QDR;
							M5234SpiBuf.dwDataLen--;
						}
						else
							break;
					}
				}
				M5234SpiBuf.isFirst = FALSE;	
			}

			//����Ƿ����ݽ������
			if(M5234SpiBuf.dwDataLen == 0)	//���ݽ������
			{
				M5234SpiBuf.bEndFlag = 0xff;	//��������������־
				intUnlock(intmask);
				taskUnlock();
				SpiCsDisact(M5234SpiBuf.bCsAdd);	
				return;
			}
			else
			{
				//������һ�η��͵Ŀ����ݳ���
				len = (M5234SpiBuf.dwDataLen>SPI_FIFO_LEN) ? SPI_FIFO_LEN : M5234SpiBuf.dwDataLen;
				
				//����д�뷢�ͻ������ĸ������ö���βָ�룬������CS����Ч
				MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV|((len-1)<<8);

				//����FIFO��д����Ч����,�����������������������Ϣ
				MCF_QSPI_QAR = SPI_TXD_RAM_BASE;	//��ָ���ƶ�������RAM��ʼ
				for(i=0; i<len; i++)
					MCF_QSPI_QDR = 0xaa;
			}
		}
	}
	else	//�쳣��ֹ
	{
		M5234SpiBuf.bEndFlag = 0x55;	//�����쳣������־
		intUnlock(intmask);
		SpiCsDisact(M5234SpiBuf.bCsAdd);	
		return;
	}
	
	intUnlock(intmask);
	//SPIʹ��
	MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
}

#undef TESTEEPROM 
#ifdef TESTEEPROM

/**********************************************************************************
	EEPROM���Դ��뿪ʼ
***********************************************************************************/
#define	EEPROMBUFLEN	300

#define AA1024_CS_ADDR	0x0e

#define AA1024_PAGE_LEN	20

#define AA1024_READ		0x03
#define AA1024_WRITE	0x02
#define AA1024_WREN		0x06
#define AA1024_WRDI		0x04
#define AA1024_RDSR		0x05
#define AA1024_WRSR		0x01

BYTE EepromCmdBuf[EEPROMBUFLEN];
BYTE EepromDataBuf[EEPROMBUFLEN], EepromReadBuf[256];
WORD EepromCmdLen, EepromDataLen;
DWORD dwAddress = 0xa000;
WORD wEepromCnt = 0;

extern void AA1024WriteFlash();
extern void AA1024ReadFlash();
extern void AA1024ReadStatus();
extern void AA1024EnablWrite();

void AA1024WriteFlash()
{
	int i;

	wEepromCnt++;
	
	for(i=0; i<EEPROMBUFLEN; i++)
		EepromDataBuf[i] = i+wEepromCnt;

	outportb(EEPROM_PRO, 1);	//WP�Ÿߣ�д������ֹ
	
	EepromCmdBuf[0] = AA1024_WRITE;

	EepromCmdBuf[1] = (BYTE)(dwAddress>>16);
	EepromCmdBuf[2] = (BYTE)(dwAddress>>8);
	EepromCmdBuf[3] = (BYTE)(dwAddress);
	EepromCmdLen = 4;

	EepromDataLen = AA1024_PAGE_LEN;

	if(M5234SpiOperate(AA1024_CS_ADDR,EepromCmdBuf, EepromCmdLen, EepromDataBuf, \
		EepromDataLen, SPI_WRITE) == ERR_SUCCESS)
		ReportMsg("AA1024WriteFlash OK.");
	else
		ReportMsg("AA1024WriteFlash Error.");
	
	outportb(EEPROM_PRO, 0);	//WP�ŵͣ�д��������
	
}


void AA1024ReadFlash()
{
	memset((void *)EepromReadBuf, 0, sizeof(EepromReadBuf));
	
	EepromCmdBuf[0] = AA1024_READ;
	EepromCmdBuf[1] = (BYTE)(dwAddress>>16);
	EepromCmdBuf[2] = (BYTE)(dwAddress>>8);
	EepromCmdBuf[3] = (BYTE)(dwAddress);
	EepromCmdLen = 4;

	EepromDataLen = 256;

	if(M5234SpiOperate(AA1024_CS_ADDR,EepromCmdBuf, EepromCmdLen, EepromReadBuf, \
		EepromDataLen, SPI_READ) == ERR_SUCCESS)
		ReportMsg("AA1024ReadFlash OK.");
	else
		ReportMsg("AA1024ReadFlash Error.");
	
}

void AA1024ReadStatus()
{
	EepromCmdBuf[0] = AA1024_RDSR;
	
	EepromCmdLen = 1;
	
	memset((void *)EepromReadBuf, 0, sizeof(EepromReadBuf));
	
	EepromDataLen = 1;
	
	if(M5234SpiOperate(AA1024_CS_ADDR,EepromCmdBuf, EepromCmdLen, EepromReadBuf, \
		EepromDataLen, SPI_READ) == ERR_SUCCESS)
		ReportMsg("AA1024ReadStatus OK.");
	else
		ReportMsg("AA1024ReadStatus Error.");
	
}

void AA1024EnablWrite()
{
	EepromCmdBuf[0] = AA1024_WREN;
	
	EepromCmdLen = 1;
	
//	memset((void *)EepromDataBuf, 0, sizeof(EepromDataBuf));
	
	EepromDataLen = 0;
	
	if(M5234SpiOperate(AA1024_CS_ADDR,EepromCmdBuf, EepromCmdLen, EepromDataBuf, \
		EepromDataLen, SPI_WRITE) == ERR_SUCCESS)
		ReportMsg("AA1024EnablWrite OK.");
	else
		ReportMsg("AA1024EnablWrite Error.");
	
}

#endif
/**********************************************************************************
	���Դ������
***********************************************************************************/


/**********************************************************************************
	ADC���Դ��뿪ʼ
***********************************************************************************/
#ifdef _DBUG_ADC_
SHORT wADC1Data[100][8];
SHORT wADC2Data[100][8];
WORD wADCPtr, wADCNewPtr;
	

void ADCHardInit(void)
{
	wADCPtr = wADCNewPtr = 0;
	MCF_GPIO_PAR_TIMER = 0x0000;	//�������ж�ʱ������ΪI/O��ʽ

	outport(ADC1_CS_BASE, 0x0004);	//����ADC����ģʽ:�ⲿ�ο���CONVSTģʽ
	outport(ADC2_CS_BASE, 0x0004);

	//ADCΪIRQ6
	OS_SetVector(MCF_INTC0_LVL_LVL6, ADCSampInt, 0);
	OS_IntEnable(MCF_INTC0_LVL_LVL6);

	//�����½����ж�
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_EPPAx_FALLING);
	//�ж�����
	MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE6;

	//����DT2OUT DT2IN��ΪADC��CONVST�����źţ�����������
	//����DT2OUT DT2INΪ�����ʽ
//	MCF_GPIO_PDDR_TIMER |= (MCF_GPIO_PDDR_TIMER_PDDR_TIMER2|MCF_GPIO_PDDR_TIMER_PDDR_TIMER6);
	MCF_GPIO_PDDR_TIMER = 0xff;
	//���ó�ʼ״̬Ϊ��
	//MCF_GPIO_PODR_TIMER &= (~(MCF_GPIO_PODR_TIMER_PODR_TIMER2|MCF_GPIO_PODR_TIMER_PODR_TIMER6));

	
}

void ADCSampStart(void)
{
/*	
	MCF_GPIO_PODR_TIMER = 0x00;
	MCF_GPIO_PODR_TIMER = 0xff;
	OS_Sleep(500);
	ADCSampInt(0);
	MCF_GPIO_PODR_TIMER = 0x00;
*/
	MCF_GPIO_PODR_TIMER = 0x00;
	MCF_GPIO_PODR_TIMER = 0xff;
	ReportMsg("ADC Int counter=%d", wADCNewPtr);
	
	
/*
	ReportMsg("ADC Int counter=%d", wADCNewPtr);
	ReportMsg("ADC1: %6d, %6d, %6d, %6d, %6d, %6d, %6d, %6d ",wADC1Data[wADCPtr][0],wADC1Data[wADCPtr][1],\
			wADC1Data[wADCPtr][2],wADC1Data[wADCPtr][3],wADC1Data[wADCPtr][4],wADC1Data[wADCPtr][5],\
			wADC1Data[wADCPtr][6],wADC1Data[wADCPtr][7]);
	ReportMsg("ADC2: %6d, %6d, %6d, %6d, %6d, %6d, %6d, %6d ",wADC2Data[wADCPtr][0],wADC2Data[wADCPtr][1],\
			wADC2Data[wADCPtr][2],wADC2Data[wADCPtr][3],wADC2Data[wADCPtr][4],wADC2Data[wADCPtr][5],\
			wADC2Data[wADCPtr][6],wADC2Data[wADCPtr][7]);
*/			
}


void ADCSampInt(void * arg)
{
	BYTE status;
	WORD value;

	MCF_EPORT_EPFR |= 0x40;
	wADCPtr = wADCNewPtr;
/*
	for(int i=0 ;i<8; i++)
	{
		wADC1Data[wADCPtr][i] = inport(ADC1_CS_BASE);
		wADC2Data[wADCPtr][i] = inport(ADC2_CS_BASE);
	}
*/
/*
	ReportMsg("ADC1: %6d, %6d, %6d, %6d, %6d, %6d, %6d, %6d ",wADC1Data[wADCPtr][0],wADC1Data[wADCPtr][1],\
			wADC1Data[wADCPtr][2],wADC1Data[wADCPtr][3],wADC1Data[wADCPtr][4],wADC1Data[wADCPtr][5],\
			wADC1Data[wADCPtr][6],wADC1Data[wADCPtr][7]);
	ReportMsg("ADC2: %6d, %6d, %6d, %6d, %6d, %6d, %6d, %6d ",wADC2Data[wADCPtr][0],wADC2Data[wADCPtr][1],\
			wADC2Data[wADCPtr][2],wADC2Data[wADCPtr][3],wADC2Data[wADCPtr][4],wADC2Data[wADCPtr][5],\
			wADC2Data[wADCPtr][6],wADC2Data[wADCPtr][7]);
*/
	wADCNewPtr++;
	wADCNewPtr %= 100;
}
#endif

#undef _DBUG_AD7782_ 
#ifdef _DBUG_AD7782_

BYTE bAD7782ReadCnt = 0;
DWORD dwAD7782Data = 0;
void ReadAD7782()
{
	BYTE bCmdBuf[8], bCmdLen;
	BYTE bDataBuf[8], bDataLen;


	SetAD7782Mode();
	
	bCmdLen = 1;

	memset(bDataBuf, 0, sizeof(bDataBuf));

	bDataLen = 3;

	if(M5234SpiOperate(AD7782_CS_ADDR,bCmdBuf, bCmdLen, bDataBuf, bDataLen, SPI_READ) == ERR_SUCCESS)
	{
		dwAD7782Data += MAKEDWORD(bDataBuf[2], bDataBuf[1],bDataBuf[0],0 );
		bAD7782ReadCnt++;
		if(bAD7782ReadCnt == 10)
		{
			dwAD7782Data /= 10;
			
			ReportMsg("The AD7782 value is 0x%x %8d.", dwAD7782Data, dwAD7782Data);

			dwAD7782Data = 0;
			bAD7782ReadCnt = 0;
		}
	}
	
}


#endif
/**********************************************************************************
	���Դ������
***********************************************************************************/

