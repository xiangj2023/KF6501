/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			m5234spi.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           m5234的SPI驱动代码，以及25AA1024的有关测试代码                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      提供IIC的有关初始化、读写控制等操作接口                          
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	M5234SpiInit		SPI初始化
*		M5234SpiOperate		SPI读写操作接口
*		M5234SpiInt			SPI中断处理函数
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/11/26    初始创建                         
*                                                                                  
********************************************************************************************/
#include "m5234spi.h"

TM5234Spi M5234SpiBuf;		//spi操作缓冲

/*********************************************************************************************
 *
 * 功能描述     SPI初始化，设置对应引脚，设置时钟频率，加载中断,片选为I/O方式控制
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234SpiInit(void)
{
	M5234SpiReset();
/*	
	memset((void *)&M5234SpiBuf, 0, sizeof(TM5234Spi));
	MCF_GPIO_PAR_QSPI = 0x1f;	//设置CS0、CS1、CLK、DIN、DOUT引脚，其中CS0/CS1为IO方式
	
	MCF_GPIO_PDDR_QSPI |= 0x18;	//设置片选为输出模式
	
	SetSpiCs0Disact();			//设置25AA1024片选为无效状态
	SetSpiCs1Disact();			//设置AD7782片选为无效状态
	
	// 设置工作模式，发送数据间隙高阻、CLK无效状态为低、上升沿锁存数据，时钟18.75MHz，8位数据
	MCF_QSPI_QMR = SPI_RATE_DIV|MCF_QSPI_QMR_MSTR|MCF_QSPI_QMR_DOHIE;

	//设置发送延时参数，不考虑延时
	MCF_QSPI_QDLYR = 0x0;
*/
	OS_SetVector(MCF_INTC0_LVL_SPI, M5234SpiInt, 0);	//中断加载
	OS_IntEnable(MCF_INTC0_LVL_SPI);

	//设置中断优先级
	MCF_INTC0_ICRn(18) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(4);

	//设置中断标志位,并清除部分标志信息
//	MCF_QSPI_QIR = 0xDD0D;
	
}

/*********************************************************************************************
 *
 * 功能描述     SPI复位
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234SpiReset()
{
	memset((void *)&M5234SpiBuf, 0, sizeof(TM5234Spi));
	MCF_GPIO_PAR_QSPI = 0x1f;	//设置CS0、CS1、CLK、DIN、DOUT引脚，其中CS0/CS1为IO方式
	
	MCF_GPIO_PDDR_QSPI |= 0x18;	//设置片选为输出模式
	
	SetSpiCs0Disact();			//设置25AA1024片选为无效状态
	SetSpiCs1Disact();			//设置AD7782片选为无效状态
	
	// 设置工作模式，发送数据间隙高阻、CLK无效状态为低、上升沿锁存数据，时钟18.75MHz，8位数据
//	MCF_QSPI_QMR = SPI_RATE_DIV|MCF_QSPI_QMR_MSTR|MCF_QSPI_QMR_DOHIE;

	MCF_QSPI_QMR = SPI_RATE_25AA1024_DIV|MCF_QSPI_QMR_MSTR|MCF_QSPI_QMR_DOHIE\
												|MCF_QSPI_QMR_CPHA|MCF_QSPI_QMR_CPOL;

	//设置发送延时参数，不考虑延时
	MCF_QSPI_QDLYR = 0x0;
	
	//设置中断标志位,并清除部分标志信息
	MCF_QSPI_QIR = 0xDD0D;
	
}

/*********************************************************************************************
 *
 * 功能描述     SPI读写操作接口，完成读写操作
 *
 * 参数说明      - bChip	: 输入 	从设备地址
 *				 - bCmdBuf	: 输入	发送命令指针
 *				 - dwCmdLen	: 输入  发送命令长度
 *				 - bDataBuf	: 双向	写入或读取的数据指针
 *				 - dwDataLen: 输入	写入或读取的数据长度
 *				 - bCmd		: 输入	操作类型
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234SpiOperate(BYTE bChip,BYTE *bCmdBuf, DWORD dwCmdLen, BYTE *bDataBuf, DWORD dwDataLen, BYTE bCmd)
{
	DWORD address;
	int i, delay;
	
	//片选地址合法性检查
	ASSERT(bChip<SPI_CS_NUM);
	if(bChip>=SPI_CS_NUM)
		return ERR_FAILURE;

	//临界资源保护,考虑多个SPI外设的情况
//    OS_LockM(SPI_LOCK);

	taskLock();
	
	M5234SpiBuf.bCmdMode = bCmd;	//设置本次操作的操作模式
	M5234SpiBuf.dwCmdLen = dwCmdLen;
	M5234SpiBuf.dwDataLen = dwDataLen;
	M5234SpiBuf.bCmdBuf = bCmdBuf;
	M5234SpiBuf.bDataBuf = bDataBuf;
	M5234SpiBuf.bCsAdd = bChip;
	
	M5234SpiBuf.bEndFlag = 0;

	//调整命令寄存器信息
	address = bChip;
	address <<= 8;
//	address |= 0x8000;//0x3000;	片选持续有效，不插入延时

	//将指针移动到命令RAM开始
	MCF_QSPI_QAR = SPI_CMD_RAM_BASE;
	//连续写入所有命令
	for(i=0; i<SPI_FIFO_LEN; i++)
		MCF_QSPI_QDR = address;

	//写入发送数据
	MCF_QSPI_QAR = SPI_TXD_RAM_BASE;	//将指针移动到发送RAM开始
	for(i=0; i<SPI_FIFO_LEN; i++)		//最多一次写入16个发送数据
	{
		if(M5234SpiBuf.dwCmdLen)			//首先写入命令
		{
			MCF_QSPI_QDR = *M5234SpiBuf.bCmdBuf++;
			M5234SpiBuf.dwCmdLen--;
		}
		else
		{
			if( (bCmd==SPI_WRITE) && (M5234SpiBuf.dwDataLen) )	//写操作且有数据发送则继续发送
			{
				MCF_QSPI_QDR = *M5234SpiBuf.bDataBuf++;
				M5234SpiBuf.dwDataLen--;
			}
			else
				break;
		}
	}

	//设置片选有效
	SpiCsActive(M5234SpiBuf.bCsAdd);	
	
	//对于读操作，假设命令信息可以一次发送完成。
	if(bCmd==SPI_READ)
		M5234SpiBuf.isFirst = TRUE;

	
	//根据写入发送缓冲区的个数设置队列尾指针，并设置CS低有效
	MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV|((i-1)<<8);

	//SPI使能
	MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;

	//循环等待检查数据操作是否成功
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

		//及时恢复系统调度
		if(delay == 5)
			taskUnlock();
	}
	
	//设置片选无效
	SpiCsDisact(M5234SpiBuf.bCsAdd);	
	
//    OS_UnLockM(SPI_LOCK);
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * 功能描述     SPI中断处理函数
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234SpiInt(void * arg)
{
	WORD status;
	int i, len, intmask;

	intmask = intLock();
	//禁止SPI功能
	MCF_QSPI_QDLYR &= ~MCF_QSPI_QDLYR_SPE;

	status = MCF_QSPI_QIR;	//读中断寄存器
	MCF_QSPI_QIR |= 0x000d;	//清除中断寄存器的有效标志

	if( (status&0x000d) == MCF_QSPI_QIR_SPIF)	//只有SPIF置位，说明正常
	{
		//检查数据是否收发完成
		if(M5234SpiBuf.dwDataLen == 0)
		{
			if(M5234SpiBuf.bCmdMode==SPI_WRITE)	//如果是写操作，则说明写操作正常结束
			{
				M5234SpiBuf.bEndFlag = 0xff;	//设置正常结束标志
				intUnlock(intmask);
				taskUnlock();
				SpiCsDisact(M5234SpiBuf.bCsAdd);	
				return;
			}
		}
		//写操作且有数据发送则继续发送
		if(M5234SpiBuf.bCmdMode==SPI_WRITE)	
		{
			MCF_QSPI_QAR = SPI_TXD_RAM_BASE;	//将指针移动到发送RAM开始
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
			
			//根据写入发送缓冲区的个数设置队列尾指针，并设置CS低有效
			MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV|((i-1)<<8);
		}
		else
		{
			if(M5234SpiBuf.isFirst == FALSE)	//读取接收到的数据
			{
				len = MCF_QSPI_QWR & 0x0f00;
				len >>= 8;
				MCF_QSPI_QAR = SPI_RXD_RAM_BASE;	//将指针移动到接收RAM开始
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
			else	//第一次只发送了命令信息，不需要接收处理(但是对于AA7782,时钟有效时就发送数据)。
			{
				if(M5234SpiBuf.bCsAdd == AD7782_CS_ADDR)
				{
					//此处取数据是针对AD7782处理，
					len = MCF_QSPI_QWR & 0x0f00;
					len >>= 8;
					MCF_QSPI_QAR = SPI_RXD_RAM_BASE;	//将指针移动到接收RAM开始
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

			//检查是否数据接收完成
			if(M5234SpiBuf.dwDataLen == 0)	//数据接收完成
			{
				M5234SpiBuf.bEndFlag = 0xff;	//设置正常结束标志
				intUnlock(intmask);
				taskUnlock();
				SpiCsDisact(M5234SpiBuf.bCsAdd);	
				return;
			}
			else
			{
				//计算下一次发送的空数据长度
				len = (M5234SpiBuf.dwDataLen>SPI_FIFO_LEN) ? SPI_FIFO_LEN : M5234SpiBuf.dwDataLen;
				
				//根据写入发送缓冲区的个数设置队列尾指针，并设置CS低有效
				MCF_QSPI_QWR = MCF_QSPI_QWR_CSIV|((len-1)<<8);

				//向发送FIFO中写入无效数据,否则会继续发送最初的命令信息
				MCF_QSPI_QAR = SPI_TXD_RAM_BASE;	//将指针移动到发送RAM开始
				for(i=0; i<len; i++)
					MCF_QSPI_QDR = 0xaa;
			}
		}
	}
	else	//异常中止
	{
		M5234SpiBuf.bEndFlag = 0x55;	//设置异常结束标志
		intUnlock(intmask);
		SpiCsDisact(M5234SpiBuf.bCsAdd);	
		return;
	}
	
	intUnlock(intmask);
	//SPI使能
	MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
}

#undef TESTEEPROM 
#ifdef TESTEEPROM

/**********************************************************************************
	EEPROM测试代码开始
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

	outportb(EEPROM_PRO, 1);	//WP脚高，写保护禁止
	
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
	
	outportb(EEPROM_PRO, 0);	//WP脚低，写保护允许
	
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
	测试代码结束
***********************************************************************************/


/**********************************************************************************
	ADC测试代码开始
***********************************************************************************/
#ifdef _DBUG_ADC_
SHORT wADC1Data[100][8];
SHORT wADC2Data[100][8];
WORD wADCPtr, wADCNewPtr;
	

void ADCHardInit(void)
{
	wADCPtr = wADCNewPtr = 0;
	MCF_GPIO_PAR_TIMER = 0x0000;	//设置所有定时器引脚为I/O方式

	outport(ADC1_CS_BASE, 0x0004);	//设置ADC工作模式:外部参考、CONVST模式
	outport(ADC2_CS_BASE, 0x0004);

	//ADC为IRQ6
	OS_SetVector(MCF_INTC0_LVL_LVL6, ADCSampInt, 0);
	OS_IntEnable(MCF_INTC0_LVL_LVL6);

	//设置下降沿中断
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_EPPAx_FALLING);
	//中断允许
	MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE6;

	//设置DT2OUT DT2IN作为ADC的CONVST控制信号，上升沿启动
	//设置DT2OUT DT2IN为输出方式
//	MCF_GPIO_PDDR_TIMER |= (MCF_GPIO_PDDR_TIMER_PDDR_TIMER2|MCF_GPIO_PDDR_TIMER_PDDR_TIMER6);
	MCF_GPIO_PDDR_TIMER = 0xff;
	//设置初始状态为低
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
	测试代码结束
***********************************************************************************/

