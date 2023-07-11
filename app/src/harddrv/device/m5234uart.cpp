/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			m5234uart.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           m5234的UART驱动代码                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      提供UART0/UART1的有关初始化、打开、发送控制、接收控制、IO控制等操作接口                          
*      M523X的UART的FIFO较小，接收4字节(含1字节的移位寄存器)，发送2字节(含1字节的移位寄存器)                                                                                  
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	M5234UartInit		UART初始化，关闭收发功能
*      	M5234UartOpen		打开串口，加载中断，设置波特率、校验、方向控制信号等                                                              
*      	M5234UartSetBaud	设置UART波特率                                                                         
*      	M5234UartSetComPar 	设置UART基本通信参数，如校验、数据位、起始位、停止位等                                                                        
*      	M5234UartModemInit 	根据串口通道类型初始化MODEM控制信号，对于UART只支持RTS和CTS信号                                                                       
*      	M5234UartTxd		UART发送控制，当允许发送时，发送一个字节数据以触发发送中断                                                                       
*      	M5234UartRxd	    UART接收控制                                                                      
*      	M5234UartInt	    UART中断处理函数                                                                      
*      	M5234UartRxdInt	    UART中断接收过程处理                                                                      
*      	M5234UartTxdInt	    UART中断发送过程处理                                                                      
*      	M5234UartWriteAChar	通过UART发送一个字节数据，以触发发送中断                                                                        
*      	M5234UartIOCtl	   	UART的IO控制接口函数                                                                        
*      	M5234UartModemSet	MODEM信号输出控制，对于colefire的UART，只能控制RTS信号                                                                     
*      	M5234UartModemGet  	读取MODEM输入信号状态，对于colefire的UART，只能读取CTS信号                                                                        
*      	M5234UartBaudSet	UART波特率设置                                                                        
*      	M5234UartBaudGet	UART波特率值读取，实际上读取的是串口类的波特率序号所对应的波特率值                                                                        
*      	M5234UartParSet	   	UART通信参数设置，包括数据位、校验、停止位等内容                                                                        
*      	M5235UartParGet	   	根据UART相关寄存器的值，获得当前基本通信参数                                                                        
*      	M5234UartReset	   	UART复位，复位发送、接收、错误状态，并重新打开发送、接收，仅允许接收中断                                                                        
*      	M5234UartIsTxdEnd	检查UART的连续一帧发送数据是否真正发送完成                                                                        
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/7/10    初始创建                         
*                                                                                  
********************************************************************************************/

#include "m5234uart.h"
#include "misi.h"

#define UART_HARD_BUF_LEN	512

BYTE bUartHardBuf[UART_HARD_BUF_LEN];
WORD wUartBufPtr = 0;

#if (MCF5234_UART == YES)

/*********************************************************************************************
 *
 * 功能描述     UART初始化，关闭收发功能
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234UartInit()
{

	//由于UART0需要作为调试用，因此初始化UART1时不能关闭该通道，所以暂时不做任何处理
/*	
	WORD IntLev
	IntLev = OS_IntLock();
	
	//关闭UART0的发送和接收
	*MCF_UART_UCR0 = MCF_UART_UCR_TX_DISABLED|MCF_UART_UCR_RX_DISABLED;

	//关闭UART1的发送和接收
	*MCF_UART_UCR1 = MCF_UART_UCR_TX_DISABLED|MCF_UART_UCR_RX_DISABLED;

	OS_IntUnLock(IntLev);
*/	
	//串口使用UART0和UART2，UART1用作CAN0的收发
	MCF_GPIO_PAR_UART = 0x3A0C;	//syslib.c中初始化为0x40FF
}

/*********************************************************************************************
 *
 * 功能描述     打开串口，加载中断，设置波特率、校验、方向控制信号等
 *
 * 参数说明      - wPort	: 输入 	串口号
 *				 - portInfo	: 输入	端口参数
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234UartOpen(WORD wPort, TSerialMap *portInfo)
{
	CSerial* pSerial;
	BYTE channel;
	DWORD IntLev;
//	static BYTE ModeReg[2];

	//检查串口类的合法性
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartOpen",FILE_LINE,"port invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	pSerial->m_PortInfo = *portInfo;

	channel = portInfo->nChannel;
	if( (channel != M5234_UART_CHAN0) && (channel != M5234_UART_CHAN1) )
	{
		LogError("M5234UartOpen",FILE_LINE,"uart channel invalidate");
		return ERR_SERIAL_CHANNEL;
	}
	
	//中断加载,注意测试一下中断函数传递的参数是否有问题
	if(channel == M5234_UART_CHAN0)	//UART0
	{
		OS_SetVector(MCF_INTC0_LVL_UART0, M5234UartInt, (DWORD)pSerial);
		//syslib.c中对UART0~2的MCF_INTC0_ICRn进行了初始化
//		OS_IntEnable(MCF_INTC0_LVL_UART0);
//	  	MCF_INTC0_ICRn(13) = MCF_INTC0_ICRn_IL(4) | MCF_INTC0_ICRn_IP(2);  
//	    MCF_GPIO_PAR_UART |= 0x40FF;
	}
	else	//UART1
	{
		//注意，实际上使用的是UART2的引脚资源
		OS_SetVector(MCF_INTC0_LVL_UART2, M5234UartInt, (DWORD)pSerial);
		OS_IntEnable(MCF_INTC0_LVL_UART2);
		MCF_INTC0_ICRn(15) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(0);  
	}

	IntLev = OS_IntLock();
	//清除中断屏蔽寄存器
	MCF_UART_UIMR(channel) = 0x00;

	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//复位接收
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_RX;

	//复位发送
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_TX;

	//复位错误状态标志位
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_ERROR;

	//清除2个模式寄存器，注意下面同样的2句指令是对2个模式寄存器的清除操作。
	MCF_UART_UMR(channel) = 0x00;
	MCF_UART_UMR(channel) = 0x00;

	//设置时钟选择寄存器
	MCF_UART_UCSR(channel) = MCF_UART_UCSR_RCS_SYS_CLK|MCF_UART_UCSR_TCS_SYS_CLK;

	OS_IntUnLock(IntLev);
	
	//设置波特率
	M5234UartSetBaud(channel, pSerial->m_serialPara.byBaud);

	//设置串口数据位、校验位等
	M5234UartSetComPar(channel, pSerial->m_serialPara.bFlag);

	//设置MODEM方向控制信号
	M5234UartModemInit(channel, pSerial->m_serialPara.bFlag);

	IntLev = OS_IntLock();
	//允许发送和接收
	MCF_UART_UCR(channel) = MCF_UART_UCR_TX_ENABLED;
	MCF_UART_UCR(channel) = MCF_UART_UCR_RX_ENABLED;

	//打开接收中断，发送中断只有在发送数据之前打开，且发送完成后关闭发送中断以避免无效的中断
	MCF_UART_UIMR(channel) = MCF_UART_UIMR_RXRDY_FU;	//接收准备好中断
	pSerial->m_bIntMode = MCF_UART_UIMR_RXRDY_FU;
	
	OS_IntUnLock(IntLev);

	//检查寄存器是否设置正确
//	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
//	ModeReg[0] = MCF_UART_UMR(channel);
//	ModeReg[1] = MCF_UART_UMR(channel);
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     设置UART波特率，注意波特率的计算方法
 *
 * 参数说明      - channel	: 输入 	UART的内部序号
 *				 - BaudNum	: 输入	波特率序号
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234UartSetBaud(BYTE channel, BYTE BaudNum)
{
	DWORD val, baud;
	DWORD IntLev;

	if( (channel != M5234_UART_CHAN0) && (channel != M5234_UART_CHAN1) )
	{
		LogError("M5234UartSetBaud",FILE_LINE,"uart channel invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	//根据波特率序号取对应波特率值
	NumToBaud(BaudNum, &baud);

	//波特率计算
	val = (IN_SYS_CLOCK/baud + 16)/32;

	//设置波特率寄存器
	IntLev = OS_IntLock();
	MCF_UART_UBG1(channel) = HIBYTE(val);
	MCF_UART_UBG2(channel) = LOBYTE(val);
	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     设置UART基本通信参数，如校验、数据位、起始位、停止位等
 *
 * 参数说明      - channel	: 输入 	UART的内部序号
 *				 - par		: 输入	基本通信参数
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234UartSetComPar(BYTE channel, BYTE par)
{
	BYTE ModeReg1, ModeReg2;
	DWORD IntLev;
	
	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
		
	//首先读取模式寄存器原始值，注意操作完UMR1x后指针会自动指向UMR2x
	ModeReg1 = MCF_UART_UMR(channel);
	ModeReg2 = MCF_UART_UMR(channel);

	//设置数据位
	ModeReg1 &= 0xfc;
	switch(par&DATA_BIT_MASK)
	{
		case DATA_BIT_5:
			//ModeReg1 &= 0xfc;	//标志位值为0
			break;
		case DATA_BIT_6:
			ModeReg1 |= MCF_UART_UMR_BC_6;
			break;
		case DATA_BIT_7:
			ModeReg1 |= MCF_UART_UMR_BC_7;
			break;
		case DATA_BIT_8:
			ModeReg1 |= MCF_UART_UMR_BC_8;
			break;
	}

	//设置校验
	ModeReg1 &= ~(0x07<<2);
	switch(par&PARITY_BIT_MASK)
	{
		case NO_PARITY:
			ModeReg1 |= MCF_UART_UMR_PM_NONE;
			break;
		case EVEN_PARITY:	//标志位值为0
			break;
		case ODD_PARITY:
			ModeReg1 |= MCF_UART_UMR_PM_ODD;
			break;
	}

	//设置停止位
	ModeReg2 &= ~0x0f;
	switch(par&STOP_BIT_MASK)
	{
		case STOP_BIT_1:
			ModeReg2 |= MCF_UART_UMR_SB_STOP_BITS_1;
			break;
		case STOP_BIT_2:
			ModeReg2 |= MCF_UART_UMR_SB_STOP_BITS_2;
			break;
	}

	IntLev = OS_IntLock();
	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//设置UART模式寄存器
	MCF_UART_UMR(channel) = ModeReg1;
	MCF_UART_UMR(channel) = ModeReg2;

	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     根据串口通道类型初始化MODEM控制信号，对于UART只支持RTS和CTS信号
 *				对于485方式，采用自动RTS控制方式
 *
 * 参数说明      - channel	: 输入 	UART的内部序号
 *				 - par		: 输入	基本通信参数
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234UartModemInit(BYTE channel, BYTE medium)
{
	BYTE ModeReg1;
	DWORD IntLev;
	
	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
		
	//首先读取模式寄存器原始值，注意操作完UMR1x后指针会自动指向UMR2x
	ModeReg1 = MCF_UART_UMR(channel);

	ModeReg1 &= 0x7f;	//首先清除RTS控制位

	//初始化RTS控制信号,如果是自动485方式则需要初始化RTS信号，透明方式不需要控制RTS,
	//其它方式软件控制RTS信号
	if( medium & SERIAL_ATTR_RS485)
		ModeReg1 |= MCF_UART_UMR_RXRTS;	
	
	//当需要软件控制RTS信号时，设置RTS的初始状态为高即无效状态
	//注意RTS的控制方式要通过UOP0n(高)、UOP1n(低)分别完成
	if( medium & SERIAL_ATTR_RTSCTR )
		MCF_UART_UOP0(channel) = MCF_UART_UOP0_RTS;	

	IntLev = OS_IntLock();
	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
	
	//设置RTS控制信号
	MCF_UART_UMR(channel) = ModeReg1;
	
	OS_IntUnLock(IntLev);
	
	return ERR_SUCCESS;
	
}


/*********************************************************************************************
 *
 * 功能描述     UART发送控制，当允许发送时，发送一个字节数据以触发发送中断
 *
 * 参数说明      - wPort	: 输入 	端口号
 *				 - bFlag	: 输入	发送控制标志
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234UartTxd(WORD wPort, BYTE bFlag)
{
	CSerial* pSerial;
	BYTE channel, data;
	DWORD IntLev;

	//检查串口类的合法性
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartTxd",FILE_LINE,"port invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	channel = pSerial->m_PortInfo.nChannel;
	
	IntLev = OS_IntLock();

	if(bFlag)
	{
		pSerial->m_bIntMode |= MCF_UART_UIMR_TXRDY;	//发送准备好中断
	}
	else
	{
		pSerial->m_bIntMode &= ~MCF_UART_UIMR_TXRDY;
//		MCF_UART_UCR(channel) = MCF_UART_UCR_TX_DISABLED;
	}


	//发送一个字节数据数据触发发送中断
	if(bFlag)
	{
		if(pSerial->ReadFromBuf(&data, 1))
			MCF_UART_UTB(channel) = data;	//发送数据写入发送BUFFER
//		MCF_UART_UCR(channel) = MCF_UART_UCR_TX_ENABLED;
	}

	MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     UART接收控制
 *
 * 参数说明      - wPort	: 输入 	端口号
 *				 - bFlag	: 输入	接收控制标志
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234UartRxd(WORD wPort, BYTE bFlag)
{
	CSerial* pSerial;
	BYTE channel;
	DWORD IntLev;

	//检查串口类的合法性
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartRxd",FILE_LINE,"port invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	channel = pSerial->m_PortInfo.nChannel;
	

	if(bFlag)
		pSerial->m_bIntMode |= MCF_UART_UIMR_RXRDY_FU;	//接收准备好中断
	else
		pSerial->m_bIntMode &= ~MCF_UART_UIMR_RXRDY_FU;

	IntLev = OS_IntLock();
	MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
	
}

/*********************************************************************************************
 *
 * 功能描述     UART中断处理函数
 *
 * 参数说明      - arg		: 输入 	参数，实际为串口类指针地址
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234UartInt(void * arg)
{
	BYTE status, channel;
	DWORD IntLev;
	static BYTE Regisr, Regsr;
	CSerial* pSerial = (CSerial*)arg;

	channel = pSerial->m_PortInfo.nChannel;

//	Regisr = MCF_UART_UISR(channel);
//	Regsr = MCF_UART_USR(channel);
		
	IntLev = OS_IntLock();
	
	//检查中断状态寄存器，并进行有关的中断处理
	while( status = ( MCF_UART_UISR(channel) & pSerial->m_bIntMode ) )
	{
		if(status & MCF_UART_UIMR_RXRDY_FU)
			M5234UartRxdInt(pSerial);
		if(status & MCF_UART_UIMR_TXRDY)
			M5234UartTxdInt(pSerial);
	}
	OS_IntUnLock(IntLev);
}

/*********************************************************************************************
 *
 * 功能描述     UART中断接收过程处理
 *
 * 参数说明      - pSerial	: 输入 	串口类指针
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234UartRxdInt(CSerial* pSerial)
{
	BYTE status, channel, data, flag;
	int i = 0;

	channel = pSerial->m_PortInfo.nChannel;

	//根据状态寄存器的状态处理接收
	while( (status = MCF_UART_USR(channel)) & MCF_UART_USR_RXRDY )
	{
		i++;
		flag = 0x0;
		data = MCF_UART_URB(channel);

		if(status & MCF_UART_USR_PE)	//校验出错
		{
			flag = 0xaa;
			pSerial->m_dwPECnt++;
		}
		if(status & MCF_UART_USR_FE)	//没有检测到停止位
		{
			flag = 0xaa;
			pSerial->m_dwFECnt++;
		}
		if(status & MCF_UART_USR_RB)	//接收中止，此时接收FIFO中有个无效的零数据进入
		{
			flag = 0xaa;
		}
		if(status & MCF_UART_USR_OE)	//过载错误
		{
			pSerial->m_dwOECnt++;
			flag = 0xaa;
		}

		//正确的接收数据写入串口接收缓冲区
		if(flag == 0)	
		{
			pSerial->WriteToBuf(&data, 1);
			bUartHardBuf[wUartBufPtr] = data;
			wUartBufPtr++;
			wUartBufPtr %= UART_HARD_BUF_LEN;
		}
		else
		{
			//复位错误状态标志位
			MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_ERROR;
		}

		//连续接收数据个数不可能超过接收FIFO长度
		if(i > UART_RXD_FIFO_LEN)	
			break;
	}
	
}

/*********************************************************************************************
 *
 * 功能描述     UART中断发送过程处理
 *
 * 参数说明      - pSerial	: 输入 	串口类指针
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234UartTxdInt(CSerial* pSerial)
{
	BYTE data, channel, status;
	int i=0;

	channel = pSerial->m_PortInfo.nChannel;

	status = MCF_UART_USR(channel);
	//根据状态寄存器的相关标志进行发送处理
	while(MCF_UART_USR(channel) & MCF_UART_USR_TXRDY)
	{
		i++;

		//串口发送缓冲区空，则关闭发送中断
		if(!pSerial->GetCanSendNum())
		{
			pSerial->m_bIntMode &= ~MCF_UART_UIMR_TXRDY; //禁止产生发送空中断
			
			MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
			
			pSerial->SerialTxdCtr(FALSE);
			break;
		}
		
		//发送数据
		if(pSerial->ReadFromBuf(&data, 1))
			MCF_UART_UTB(channel) = data;


		//连续发送数据个数不可能超过发送FIFO长度
		if(i>UART_TXD_FIFO_LEN)			
			break;
	}
}

/*********************************************************************************************
 *
 * 功能描述     通过UART发送一个字节数据，以触发发送中断
 *
 * 参数说明      - wPort	: 输入 	串口号
 *				 - bData	: 输入	准备发送的字节数据
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartWriteAChar(WORD wPort, BYTE bData)
{
	CSerial* pSerial;
	BYTE channel;
	DWORD IntLev;

	//检查串口类的合法性
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartWriteAChar",FILE_LINE,"port invalidate");
		return FALSE;
	}

	channel = pSerial->m_PortInfo.nChannel;
	
	IntLev = OS_IntLock();

	pSerial->m_bIntMode |= MCF_UART_UIMR_TXRDY;	//发送准备好中断

	MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
	
	MCF_UART_UTB(channel) = bData;	//发送数据写入发送BUFFER

	OS_IntUnLock(IntLev);

	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     UART的IO控制接口函数，需要注意各个参数的含义和顺序
 *
 * 参数说明      - pdwPar1	: |---MODEM_SET:		输入	MODEM输出控制信号标志位
 *							  |---MODEM_GET:		输出	MODEM输入状态信号		
 *							  |---BAUD_SET:			输入	准备写入的波特率值		
 *							  |---BAUD_GET:			输出	读取的波特率值	
 *							  |---COM_PAR_SET:		输入	准备设置的串口通信参数标志		
 *							  |---COM_PAR_GET:		输出	读出的通信参数标志		
 *							  |---COM_CHAN_RESET	无效		
 *							  |---COM_TXD_END		无效		
 *
 *				 - pdwPar2	: |---MODEM_SET:		输入	MODEM输出控制信号高低标志
 *							  |---MODEM_GET:		无效		
 *							  |---BAUD_SET:			无效		
 *							  |---BAUD_GET:			无效
 *							  |---COM_PAR_SET:		无效		
 *							  |---COM_PAR_GET:		无效		
 *							  |---COM_CHAN_RESET	无效		
 *							  |---COM_TXD_END		无效		
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartIOCtl(WORD wPort, BYTE bCmd, DWORD *pdwPar1, DWORD *pdwPar2)
{
	CSerial* pSerial;
	BYTE channel;

	//检查串口类的合法性
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartIOCtl",FILE_LINE,"port invalidate");
		return FALSE;
	}
	//检查串口通道序号的合法性
	channel = pSerial->m_PortInfo.nChannel;
	if( (channel != M5234_UART_CHAN0) && (channel != M5234_UART_CHAN1) )
	{
		LogError("M5234UartIOCtl",FILE_LINE,"channel invalidate");
		return FALSE;
	}
		
	switch(bCmd)
	{
		case MODEM_SET:	//MODEM输出信号控制
			return M5234UartModemSet(channel, (BYTE)(*pdwPar1), (BYTE)(*pdwPar2));
		case MODEM_GET:	//MODEM输入信号读取
			return M5234UartModemGet(channel, (BYTE*)pdwPar1);
		case BAUD_SET:	//波特率设置
			return M5234UartBaudSet(channel, *pdwPar1);
		case BAUD_GET:	//波特率值读取
			return M5234UartBaudGet(pSerial, pdwPar1);
		case COM_PAR_SET://串口通信参数设置
			return M5234UartParSet(channel, (BYTE)(*pdwPar1));
		case COM_PAR_GET://串口通信参数读取
			return M5235UartParGet(channel, (BYTE*)pdwPar1);
		case COM_CHAN_RESET://串口复位
			return M5234UartReset(channel);
		case COM_TXD_END:	//检查发送是否真正结束
			return M5234UartIsTxdEnd(channel);
		default:
			return FALSE;
	}
}

/*********************************************************************************************
 *
 * 功能描述     MODEM信号输出控制，对于colefire的UART，只能控制RTS信号
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *				 - out		: 输入	MODEM输出控制信号各标志位
 *				 - flag		: 输入	MODEM输出控制高低标志
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartModemSet(BYTE channel, BYTE out, BYTE flag)
{
	if(flag)	//置MODEM输出信号有效
	{
		//由于只有RTS信号可以控制，因此不需要判断out中的标志位
		MCF_UART_UOP1(channel) = MCF_UART_UOP1_RTS;	//UOP1置位控制RTS信号有效
	}
	else		//置MODEM输出信号无效
	{
		MCF_UART_UOP0(channel) = MCF_UART_UOP0_RTS;	//UOP0置位控制RTS信号无效
	}
	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     读取MODEM输入信号状态，对于colefire的UART，只能读取CTS信号
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *				 - flag		: 输出	MODEM各输入信号高低状态
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartModemGet(BYTE channel, BYTE *pFlag)
{
	BYTE status;

	*pFlag = 0;

	//读UART的输入端口寄存器中的CTS状态位
	status = MCF_UART_UIP(channel);
	if(status & MCF_UART_UIP_CTS)
		*pFlag |= MODEM_CTS;

	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     UART波特率设置
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *				 - baud		: 输入	准备设置的波特率值
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartBaudSet(BYTE channel, DWORD baud)
{
	DWORD val;
	DWORD IntLev;
	
	val = (IN_SYS_CLOCK/baud + 16)/32;

	//设置波特率寄存器
	IntLev = OS_IntLock();
	MCF_UART_UBG1(channel) = HIBYTE(val);
	MCF_UART_UBG1(channel) = LOBYTE(val);
	OS_IntUnLock(IntLev);

	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     UART波特率值读取，实际上读取的是串口类的波特率序号所对应的波特率值
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *				 - pBaud	: 输出	读取的波特率值
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartBaudGet(CSerial* pSerial, DWORD *pBaud)
{
	NumToBaud(pSerial->m_serialPara.byBaud, pBaud);
	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     UART通信参数设置，包括数据位、校验、停止位等内容
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *				 - par		: 输入	准备设置的通信参数
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartParSet(BYTE channel, BYTE par)
{
	if( M5234UartSetComPar(channel, par) == ERR_SUCCESS )
		return TRUE;
	else
		return FALSE;
}

/*********************************************************************************************
 *
 * 功能描述     根据UART相关寄存器的值，获得当前基本通信参数
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *				 - pPar		: 输出	读取的当前UART通信参数
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5235UartParGet(BYTE channel, BYTE *pPar)
{
	BYTE umr1, umr2, par;

	par = 0;
	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//读取UART的模式寄存器
	umr1 = MCF_UART_UMR(channel);
	umr2 = MCF_UART_UMR(channel);

	//检查数据位个数
	switch(umr1&MCF_UART_UMR_BC_8)
	{
		case MCF_UART_UMR_BC_5:
			par |= DATA_BIT_5;
			break;
		case MCF_UART_UMR_BC_6:
			par |= DATA_BIT_6;
			break;
		case MCF_UART_UMR_BC_7:
			par |= DATA_BIT_7;
			break;
		case MCF_UART_UMR_BC_8:
			par |= DATA_BIT_8;
			break;
	}
	//检查校验位情况
	switch( umr1&(0x03<<3) )
	{
		case 0:
			if(umr1&MCF_UART_UMR_PM_ODD)
				par |= ODD_PARITY;
			else
				par |= EVEN_PARITY;
			break;
		case MCF_UART_UMR_PM_NONE:
			par |= NO_PARITY;
			break;
	}
	//检查停止位
	switch(umr2&0x0f)
	{
		case MCF_UART_UMR_SB_STOP_BITS_1:
			par |= STOP_BIT_1;
			break;
		case MCF_UART_UMR_SB_STOP_BITS_2:
			par |= STOP_BIT_2;
			break;
	}

	*pPar = par;
	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     UART复位，复位发送、接收、错误状态，并重新打开发送、接收，仅允许接收中断
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartReset(BYTE channel)
{
	DWORD IntLev;

	IntLev = OS_IntLock();
	//清除中断屏蔽寄存器
	MCF_UART_UIMR(channel) = 0x00;

	//关闭发送和接收
	MCF_UART_UCR(channel) = MCF_UART_UCR_TX_DISABLED;
	MCF_UART_UCR(channel) = MCF_UART_UCR_RX_DISABLED;
	
	//复位模式寄存器指针，使模式寄存器指针指向UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//复位接收
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_RX;

	//复位发送
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_TX;

	//复位错误状态标志位
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_ERROR;
	
	//允许发送和接收
	MCF_UART_UCR(channel) = MCF_UART_UCR_TX_ENABLED;
	MCF_UART_UCR(channel) = MCF_UART_UCR_RX_ENABLED;

	//打开接收中断，发送中断只有在发送数据之前打开，且发送完成后关闭发送中断以避免无效的中断
	MCF_UART_UIMR(channel) |= MCF_UART_UIMR_RXRDY_FU;	//接收准备好中断

	OS_IntUnLock(IntLev);
	
	return TRUE;
}


/*********************************************************************************************
 *
 * 功能描述     检查UART的连续一帧发送数据是否真正发送完成，因为写入发送FIFO并不等发送
 *				真正完成，对于485方式这时如果进行方向控制的话可能导致发送数据不完整
 *
 * 参数说明      - channel	: 输入 	UART通道序号
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL M5234UartIsTxdEnd(BYTE channel)
{
	BYTE status;
	//检查发送空标志位，该位置位说明发送移位寄存器和发送保持寄存器都没有数据
	status = MCF_UART_USR(channel);
	if(status & MCF_UART_USR_TXEMP)
		return TRUE;
	else
		return FALSE;
}

#endif
