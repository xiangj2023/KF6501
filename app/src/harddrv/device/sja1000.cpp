/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/

/********************************************************************************************
*                                                                                  
* 文件名称          
*			sja1000.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           sja1000驱动                                                                      
*			                                                                
* 描述                                                                             
*                                                                                  
*       sja1000驱动接口                       
*                                                                                        
*                                                                                        
* 函数                                                                             
*                                                                                  
*      	Sja1000Open			SJA1000硬件初始化
*      	Sja1000Int		   	SJA1000中断处理函数                                                                         
*      	Sja1000RxdInt	    接收中断处理函数                                                                        
*      	Sja1000TxdInt    	发送中断处理函数                                                                       
*      	Sja1000Reset	 	中断服务程序中发送处理过程                                                                       
*      	Sja1000Write   		发送数据，触发发送中断                                                                      
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/9/2    初始创建                         
*                                                                                  
********************************************************************************************/

#if(MCF5234_SJA1000 == YES)

#include "sja1000.h"
#include "canlink.h"
#include "daefunc.h"

WORD wSja1000Port;
//DWORD wMyAdd = 10;
//DWORD wPeerAdd = 20;

//static DWORD dwSjaTxdIntCnt, dwSjaRxdIntCnt, dwSjaErrorIntCnt;

//BYTE bSja1000RxdBuf[9];

static TSja100Static Sja100Static;

extern TCanPortData *pCanPortData[MAX_CAN_PORT];
extern TCanPort CanPortAttr[MAX_CAN_PORT];
extern DWORD dwCanLinkTaskID;

/*********************************************************************************************
 *
 * 功能描述     SJA1000硬件初始化
 *
 * 参数说明      - port		: 输入 	can网内部端口序号
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD Sja1000Open(WORD port)
{
	WORD address = 0;
	
	wSja1000Port = port;
	
	//复位
	Sja1000HDReset();

	SetClkDiv();
	
	outportb(SJA_OUTCTRL, 0x1a);//设置OCR参数

	//设置通信速率为50k
	SetBusTime0();
	SetBusTime1();

//地址位置匹配调整
#ifdef CAN_MASTER
	outportb(SJA_ACSMASK, 0x7f);	//D29
	outportb(SJA_ACSMASK+1, 0xff);	
	
	outportb(SJA_ACSMASK+2, 0x7f);	//D21~24
	outportb(SJA_ACSMASK+3, 0xff);	//D17~20
	
	address = 0x8000;
	
#else
	//双滤波设置,只能匹配D13~D28位,第一组设置接收点对点地址，第二组设置接收组播地址
	outportb(SJA_ACSMASK, 0xf0);	//D21~24
	outportb(SJA_ACSMASK+1, 0x0f);	//D17~20

	outportb(SJA_ACSMASK+2, 0xcf);	//D25~D26
	outportb(SJA_ACSMASK+3, 0xff);
	
	address = DB_GetDevAddress();
	address <<= 4;
	address |= 0x3000;
#endif

	outportb(SJA_ACSCODE+0, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+1, (BYTE)address);
	
	outportb(SJA_ACSCODE+2, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+3, (BYTE)address);

	//SJA1000中断为IRQ3
	OS_SetVector(MCF_INTC0_LVL_LVL3, Sja1000Int, 0);
	OS_IntEnable(MCF_INTC0_LVL_LVL3);
	
	//设置电平触发中断
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA3(MCF_EPORT_EPPAR_EPPAx_LEVEL);
	//数据方向为输入
	MCF_EPORT_EPDDR &= (~MCF_EPORT_EPDDR_EPDD3);
	//中断允许
	MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE3;

	//设置中断允许位
	outportb(SJA_INTENABLE, IR_IER_RI|IR_IER_TI|IR_IER_EI|IR_IER_DOI|IR_IER_BEI);

	//进入正常模式
	Sja1000Enable();

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     sja1000中断处理，包括接收、发送和异常处理
 *
 * 参数说明      
 *				 无
 *       		 
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void Sja1000Int(void * arg)
{
	static DWORD dwSjaErrIntCnt;
	BYTE status;

	//清除对应的外部中断标志
	MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF3;
	
	status = inportb(SJA_INTR);	//读中断状态,对后寄存器状态自动清除

	//接收中断
	if(status & IR_IER_RI)	
	{
		Sja1000RxdInt();
		Sja100Static.dwRxdIntCnt++;
		dwSjaErrIntCnt = 0;
	}

	//发送中断
	if(status & IR_IER_TI)
	{
		Sja1000TxdInt();
		Sja100Static.dwTxdIntCnt++;
		dwSjaErrIntCnt = 0;
	}

	//错误中断
	if(status & IR_ERR_MASK)
	{
		Sja100Static.dwErrorIntCnt++;
		
		dwSjaErrIntCnt++;
		if(dwSjaErrIntCnt>100)
		{
			dwSjaErrIntCnt = 0;
			Sja1000Reset(wSja1000Port);
		}
	}
}

/*********************************************************************************************
 *
 * 功能描述     sja1000接收中断处理，对于应用层报文，需要检查分包序号是否正确，对于链路层报文
 *				直接通知守护任务处理
 *
 * 参数说明      
 *				 无
 *       		 
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void Sja1000RxdInt(void)
{
	WORD len,  type, index;
	DWORD id;
	int i;

	//获取ID
	id = MAKEDWORD(inportb(SJA_RXDID+4), inportb(SJA_RXDID+3), 
			inportb(SJA_RXDID+2), inportb(SJA_RXDID+1));
	id >>= 3;
	
	len = inportb(SJA_RXDID)&0x000f;	//取数据长度
	len %= (CAN_LINK_BUF_LEN+1);
	type = (id>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK; 		//检查报文类型
		
	//根据报文进行相应处理
	if(type&LINK_FRAM_FLAG) //链路层层数据
	{
		TCanLinkFram *pLinkRxd;
		pLinkRxd = &pCanPortData[wSja1000Port]->CanLinkCmdBuf.RxdBuf;
			
		//检查接收缓冲区是否空闲
		if(pLinkRxd->dwStatus != CAN_BUF_EMPTY)
		{
			//接收过载
			pLinkRxd->dwFlag |= RXD_BUF_OVERRUN;
		}
			
		//保存链路数据
		for(i=0; i<len; i++)
			pLinkRxd->buffer[i] = inportb(SJA_RXDDATA+i);
	
		pLinkRxd->dwStatus = CAN_DATA_FINISH;	//接收数据完成，链路层可以处理
	
		//通知链路层任务,收到链路层报文
		OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wSja1000Port, 0, 0), 0, 0, 0);
	}
	else	//应用层报文处理
	{
		TCanCellBuf *pApiRxd;

		pApiRxd = &pCanPortData[wSja1000Port]->CanDevBuf.RxdBuf;
			
		pApiRxd->dwCanID = id;
	
		//检查分包序号是否正确
		index = (id>>SUB_FRAM_NO_OFFSET)&SUB_FRAM_NO_MASK;
		if((index -(pApiRxd->ptr+CAN_LINK_BUF_LEN-1)/CAN_LINK_BUF_LEN) == 1)
		{
			//不管序号是否正确，接收到的数据首先放到接收缓冲区，最后由链路任务统一检查处理。
			//第一个分包需要单独处理
			if(index == 1)	
			{
				if(pApiRxd->dwStatus != CAN_BUF_EMPTY)//检查接收缓冲区是否空
				{
					pApiRxd->dwFlag |= RXD_BUF_OVERRUN;
				}
							
				//不管原来是不是有数据，直接覆盖原来的数据
				pApiRxd->length = 0;
				pApiRxd->ptr = 0;
			}	
			
			//保存数据
			pApiRxd->dwStatus = CAN_BUF_BUSY;
			for(i=0; i<len; i++)
				pApiRxd->Buffer[pApiRxd->ptr++] = inportb(SJA_RXDDATA+i);
			pApiRxd->length += len;
				
			pApiRxd->length %= CAN_FRAME_BUF_LEN;
			pApiRxd->ptr %= CAN_FRAME_BUF_LEN;
			
		}
		else
		{
			pApiRxd->dwFlag |= SUB_FRAM_INDEX_ERR;
		}
			
		//检查是不是最后一个分包
		if(id & (SUB_FRAM_END_MASK<<SUB_FRAM_END_OFFSET))
		{
			pApiRxd->dwStatus = CAN_DATA_FINISH;	//接收数据完成，链路层可以处理
				
			//最后一个分包接收正确，通知链路层任务进行肯定确认
			OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wSja1000Port, 0, 0), 0, 0, 0);
		}
	}

	//释放接收BUF
	ReleaseRxdBuf();
}

/*********************************************************************************************
 *
 * 功能描述     sja1000发送中断处理，对于应用层报文，需要检查分包序号是否正确，对于链路层报文
 *				直接通知守护任务处理
 *
 * 参数说明      
 *				 无
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void Sja1000TxdInt(void)
{
	DWORD id;
	
	//处理链路层发送过程，由于链路层命令短，当产生链路层发送中断时，说明链路命令已经发送完成
	TCanLinkFram *pLinkTxdBuf;
	pLinkTxdBuf = &pCanPortData[wSja1000Port]->CanLinkCmdBuf.TxdBuf;
	
	if(pLinkTxdBuf->dwStatus == CAN_BUF_BUSY)
	{
		//清除链路发送缓冲区及所有标志，可以进行新链路层数据发送
		memset((void*)pLinkTxdBuf, 0 , sizeof(TCanLinkFram));
				
		//设置CAN网发送空闲标志标志
		CanPortAttr[wSja1000Port].dwFlag |= CAN_PORT_TXD_IDLE;
		return;
	}

	//应用层报文数据发送处理
	TCanCellBuf *pApiTxdBuf;
	WORD len, sendnum;

	pApiTxdBuf = &pCanPortData[wSja1000Port]->CanDevBuf.TxdBuf;
	if( pApiTxdBuf->dwStatus== CAN_BUF_BUSY )	//应用层发送缓冲区忙，说明还有数据没有发送结束，需要继续发送
	{
		//数据发送完成，结束发送状态
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
		{
			pApiTxdBuf->ptr = 0;
			
			//设置CAN网发送空闲标志标志
			CanPortAttr[wSja1000Port].dwFlag |= CAN_PORT_TXD_IDLE;
			
			pApiTxdBuf->dwStatus = CAN_DATA_FINISH;	//数据发送结束
			return;
		}
		
		//计算发送数据长度
		sendnum = pApiTxdBuf->length - pApiTxdBuf->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;

		//将需要发送的数据送到对应MB的数据区中
		for(int i=0; i<len; i++)
			outportb(SJA_TXDDATA+i, pApiTxdBuf->Buffer[pApiTxdBuf->ptr++]);
		
		//根据要求，累加应用层分包序号，并写入对应寄存器
		pApiTxdBuf->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);

		//检查本次发送是否为最后一个分包
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
			pApiTxdBuf->dwCanID |= (0x01<<SUB_FRAM_END_OFFSET);
	
		//写入数据长度及FF标示
		outportb(SJA_TXDID, len|EFF_FF);

		//写入ID
		id = pApiTxdBuf->dwCanID;
		id <<= 3;
		outportb(SJA_TXDID+1, (BYTE)(id>>24));
		outportb(SJA_TXDID+2, (BYTE)(id>>16));
		outportb(SJA_TXDID+3, (BYTE)(id>>8));
		outportb(SJA_TXDID+4, (BYTE)id);

		//启动发送命令
		SendRequest();
		return;
	}

	//其它情况，设置CAN网发送空闲标志标志
	CanPortAttr[wSja1000Port].dwFlag |= CAN_PORT_TXD_IDLE;
	
}

DWORD Sja1000Reset(WORD port)
{
	WORD address;

//	ReportMsg("SJA1000 hardware reset....");
	Sja1000HDReset();

	SetClkDiv();
	
	outportb(SJA_OUTCTRL, 0x1a);//设置OCR参数

	//设置通信速率为50k
	SetBusTime0();
	SetBusTime1();

	//地址位置匹配调整
#ifdef CAN_MASTER
	outportb(SJA_ACSMASK, 0x7f);	//D29
	outportb(SJA_ACSMASK+1, 0xff);	
		
	outportb(SJA_ACSMASK+2, 0x7f);	//D21~24
	outportb(SJA_ACSMASK+3, 0xff);	//D17~20
		
	address = 0x8000;
		
#else
	//双滤波设置,只能匹配D13~D28位,第一组设置接收点对点地址，第二组设置接收组播地址
	outportb(SJA_ACSMASK, 0xf0);	//D21~24
	outportb(SJA_ACSMASK+1, 0x0f);	//D17~20
	
	outportb(SJA_ACSMASK+2, 0xcf);	//D25~D26
	outportb(SJA_ACSMASK+3, 0xff);
		
	address = DB_GetDevAddress();
	address <<= 4;
	address |= 0x3000;
#endif
	
	outportb(SJA_ACSCODE+0, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+1, (BYTE)address);
		
	outportb(SJA_ACSCODE+2, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+3, (BYTE)address);

	//设置中断允许位
	outportb(SJA_INTENABLE, IR_IER_RI|IR_IER_TI|IR_IER_EI|IR_IER_DOI|IR_IER_BEI);

	//进入正常模式
	Sja1000Enable();

	memset(&pCanPortData[wSja1000Port]->CanDevBuf, 0, sizeof(TCanDevBuf));
	memset(&pCanPortData[wSja1000Port]->CanLinkCmdBuf, 0, sizeof(TCanLinkCmdBuf));
	
	CanPortAttr[wSja1000Port].dwFlag = (CAN_PORT_OPEN|CAN_PORT_TXD_IDLE|CAN_PORT_RXD_IDLE);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     sja1000数据发送，包括应用层数据包和链路层命令。本操作会触发一次发送中断
 *
 * 参数说明      - port			: 输入 	发送数据结构指针
 *				 - address		: 输入  发送数据地址
 *       		 - type			: 输入	数据类型
 *       		 - isNewWrite	: 输入	是否为重发
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD Sja1000Write(WORD port, BYTE address, BYTE type, BOOL isNewWrite)
{
	DWORD id;
	WORD len;
	WORD sendnum;

	if(type == CAN_LINK_FRAME)	//链路层数据
	{
		TCanLinkFram *pLinkTxd;
		
		pLinkTxd = &pCanPortData[port]->CanLinkCmdBuf.TxdBuf;
		if(pLinkTxd->dwStatus != CAN_DATA_READY)
		{
			LogError("FlexCanWrite",FILE_LINE,"the txd link buffer's status error.");
			return ERR_FAILURE;
		}

		if(pLinkTxd->length>CAN_LINK_BUF_LEN)
		{
			LogError("FlexCanWrite",FILE_LINE,"the txd link buffer length error.");
			return ERR_FAILURE;
		}
			
		//设置硬件通道发送忙标志
		CanPortAttr[port].dwFlag &= (~CAN_PORT_TXD_IDLE);
		
		//分包序号为1
		pLinkTxd->dwCanID &= ~(SUB_FRAM_NO_MASK<<SUB_FRAM_NO_OFFSET);
		pLinkTxd->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);

		//写入发送FIFO
		for(int i=0; i<pLinkTxd->length; i++)
			outportb(SJA_TXDDATA+i, pLinkTxd->buffer[i]);
		
		len = pLinkTxd->length;
/*
#ifdef CAN_MASTER		
		pLinkTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
		pLinkTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		id = pLinkTxd->dwCanID;

		pLinkTxd->dwStatus = CAN_BUF_BUSY;	//数据发送结束
	}
	else if(type == CAN_API_FRAME)	//应用层数据
	{
		TCanCellBuf *pApiTxd;
		
		pApiTxd = &pCanPortData[port]->CanDevBuf.TxdBuf;
		
		//只有处于准备状态的发送数据才能开始发送，否则说明本数据进入发送状态
		if(pApiTxd->dwStatus != CAN_DATA_READY)
		{
			LogError("FlexCanWrite",FILE_LINE,"the txd api buffer's status error.");
			return ERR_FAILURE;
		}
		
		//设置CAN网发送忙标志
		CanPortAttr[port].dwFlag &= (~CAN_PORT_TXD_IDLE);

		if(isNewWrite == FALSE)
		{
			pCanPortData[port]->CanStatic.dwApiResendCnt++;
			pApiTxd->ptr = 0;
		}

		//清除最后一个分包标志
		pApiTxd->dwCanID &= ~(0x01<<SUB_FRAM_END_OFFSET);

		//第一次发送，第一个分包序号为1
		pApiTxd->dwCanID &= ~(SUB_FRAM_NO_MASK<<SUB_FRAM_NO_OFFSET);
		pApiTxd->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);

		//注意有些应用层报文可能一个分包就可以发送完成，因此需要判断是否需要设置分包结束标志
		if(pApiTxd->length<=CAN_LINK_BUF_LEN)
			pApiTxd->dwCanID |= (0x1<<SUB_FRAM_END_OFFSET);

		//计算发送数据长度
		sendnum = pApiTxd->length - pApiTxd->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;
		
		//将需要发送的数据送到对应MB的数据区中
		for(int i=0; i<len; i++)
		{
			outportb(SJA_TXDDATA+i, pApiTxd->Buffer[pApiTxd->ptr++]);
		}
/*		
#ifdef CAN_MASTER		
		pApiTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
		pApiTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		id = pApiTxd->dwCanID;

		pApiTxd->dwStatus = CAN_BUF_BUSY;
	}

	
	//写入数据长度及FF标示
	outportb(SJA_TXDID, len|EFF_FF);
	
	//写入ID
	id <<= 3;
	outportb(SJA_TXDID+1, (BYTE)(id>>24));
	outportb(SJA_TXDID+2, (BYTE)(id>>16));
	outportb(SJA_TXDID+3, (BYTE)(id>>8));
	outportb(SJA_TXDID+4, (BYTE)id);
	
	//启动发送命令
	SendRequest();
	
	return ERR_SUCCESS;
}

/*
void TestSja1000Txd()
{
	BYTE TxdBuf[20], len;
	int i;
	
	len = bSja1000RxdBuf[0];
	
	TxdBuf[0] = (EFF_FF|len);
	
	TxdBuf[1] = wMyAdd;
	TxdBuf[2] = 0;
	TxdBuf[3] = 0;
	TxdBuf[4] = 0;

	for(i=0; i<len; i++)
		TxdBuf[5+i] = bSja1000RxdBuf[i+1];

	for(i=0; i<(len+5); i++)
		outportb(SJA_TXBUFFER+i, TxdBuf[i]);

	SendRequest();
	
}

void TestSja1000Hard(DWORD *pdwApp)
{
	OS_TaskDesp *pTask = NULL;

	pTask = OS_GetTaskTab(SELF_TASK_ID);

	Sja1000Open(0);
	
	memset(bSja1000RxdBuf, 0, sizeof(bSja1000RxdBuf));
	//outportb(LED1_CS_BASE,0xff);
	while(1)
	{
		//OS_Sleep(1000);
		//printf("Test SJA1000 hardware...\n");
		//outportb(LED1_CS_BASE,0);
		//outportb(LED2_CS_BASE,0);
		//outportb(LED3_CS_BASE,0);
		//printf("Sja1000: Txd=%d Rxd=%d Error = %d.\n",dwSjaTxdIntCnt, dwSjaRxdIntCnt, dwSjaErrorIntCnt);
		if(bSja1000RxdBuf[0])
		{
			TestSja1000Txd();
			bSja1000RxdBuf[0] = 0;
		}

		OS_Sleep(20);

		pTask->Counter = 0;
	}
}
*/
#endif
