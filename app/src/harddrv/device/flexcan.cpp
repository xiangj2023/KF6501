/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			flexcan.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           flexcan驱动                                                                      
*			                                                                
* 描述                                                                             
*                                                                                  
*      mcf5234的CAN网驱动，5234只支持1路CAN.  由于有16个MB可以用作发送或接收，根据掩码的需要                        
*      将MB0用作发送，MB1和MB14用作接收。                                                                                  
*                                                                                        
* 函数                                                                             
*                                                                                  
*      	FlexCanInit			flexcan硬件初始化
*      	FlexCanMBInit		MB功能初始化                                                              
*      	FlexCanInt0		   	MB0中断处理函数                                                                         
*      	FlexCanInt1	    	MB1中断处理函数                                                                        
*      	FlexCanInt14    	MB14中断处理函数                                                                       
*      	FlexCanTxd		 	中断服务程序中发送处理过程                                                                       
*      	FlexCanRxd   		中断服务程序中接收处理过程                                                                      
*      	FlexCanErrInt   	flexcan错误中断处理过程                                                                        
*      	SJA1000ToFlexCanID  将SJA1000格式的ID转化为flexcan格式的ID                                                                      
*      	FlexCanToSJA1000ID  将flexcan格式的ID转化为SJA1000格式的ID                                                                    
*      	FlexCanWrite       	flexcan数据发送                                                                     
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/9/2    初始创建                         
*                                                                                  
********************************************************************************************/
#if(MCF5235_FLEXCAN == YES)

#include "flexcan.h"
#include "canlink.h"
#include "daefunc.h"

WORD wFlexCanPort;

static DWORD dwFlexMsg;

//统计信息记录
static TFlexCanStatic FlexCanStatic;

extern DWORD dwCanLinkTaskID;

DWORD dwFlexSelfAddr = 10;//0x0a
DWORD dwFlexPeerAddr = 20;//0x14

BYTE bFlexRxdBuf[9];

extern TCanPortData *pCanPortData[MAX_CAN_PORT];
extern TCanPort CanPortAttr[MAX_CAN_PORT];

DWORD dwSelfFramCnt = 0;
	
/*********************************************************************************************
 *
 * 功能描述     flexcan硬件初始化，CPU提供16个MB，根据需要MB0用作发送，MB1用作点对点发送，
 *				MB14用做组播发送。对于FlexCAN，每个MB占用一个中断资源，此外错误中断也占用一个
 *				单独的中断资源，因此需要加载4个中断处理程序
 *
 * 参数说明      - port		: 输入 	can网内部端口序号
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD FlexCanOpen(WORD port)
{
	memset((void*)&FlexCanStatic, 0, sizeof(TFlexCanStatic));

	//初始化CAN0引脚功能分配
	MCF_GPIO_PAR_UART &= 0xf0ff;
	MCF_GPIO_PAR_UART |= 0x0a00;
	
	FlexCanStatic.dwCanHardResetCnt++;

	wFlexCanPort = port&0xff;

	ASSERT(wFlexCanPort < MAX_CAN_PORT);
	if( wFlexCanPort >= MAX_CAN_PORT )
	{
        LogError("FlexCanMBInit",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	
	FlexCanEnb();
	FlexCanSReset();	//CAN模块软件复位，以复位所有相关寄存器及状态
	FlexCanMDis();

	//必须等到软件复位标志清除后才能说明复位过程结束
	while((MCF_CAN_CANMCR0&MCF_CAN_CANMCR_SOFTRST) != 0);

	//设置通信速率为50k
	MCF_CAN_CANCTRL0 |= MCF_CAN_CANCTRL_CLKSRC;	//采用内部总线时钟
	FlexCanEnb();
	MCF_CAN_CANCTRL0 |= ((CAN_PRESDIV<<24)|(CAN_PSEG1<<19)|(CAN_PSEG2<<16)|\
						(CAN_RJW<<22)|CAN_PROPSEG);
	
	//定义发送优先级策略
	MCF_CAN_CANCTRL0 |= (CAN_LMD_FIRST<<4);

	//初始化MB
	for(int i=0; i<=MAX_MB_NO; i++)
		FlexCanMBInit(i, CAN_MB_DISALBE);
	FlexCanMBInit(CAN_MB0, CAN_MB_TXD);		//MB0用作发送
	FlexCanMBInit(CAN_MB1, CAN_MB_RXD);		//MB1用作接收
	FlexCanMBInit(CAN_MB14, CAN_MB_RXD);	//MB14用作接收

/*
	//设置接收MB(具体为MB1)的识别ID，即本模块地址
	MCF_CAN0_MBUFX_ID(CAN_MB0) = 0;
	MCF_CAN0_MBUFX_ID(CAN_MB0) |= (0xff<<M_MANAGER_ADDR_OFFSET);
*/	
	//设置ID掩码寄存器
	//MASK0寄存器对应MB0~MB13,用作点对点地址掩码
#ifdef CAN_MASTER
	MCF_CAN_RXGMASK0 = (M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET);
#else
	//注意，此处需要增加对主机地址位的识别，以判断是否为自己发送的数据，否则自己发送的数据会接收回来，
	MCF_CAN_RXGMASK0 = (SLAVE_MODULE_ADDR_MASK<<SLAVE_MODULE_ADDR_OFFSET)|(M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET);
#endif

	//MASK14寄存器对应MB14,设置组播掩码
	MCF_CAN_RX14MASK0 = (SLAVE_MOD_GRP_ADD_MASK<<SLAVE_MOD_GRP_ADD_OFFSET);

	//中断加载,注意对于CAN通信，每个MB占用一个中断资源，因此需要分别加载中断处理
	OS_SetVector(MCF_INTC1_LVL_CAN0_BUF0, FlexCanInt0, 0);
	OS_SetVector(MCF_INTC1_LVL_CAN0_BUF1, FlexCanInt1, 0);
	OS_SetVector(MCF_INTC1_LVL_CAN0_BUF14, FlexCanInt14, 0);
	
	//注意错误中断单独占用一个中断资源
	OS_SetVector(MCF_INTC1_LVL_CAN0_ERR, FlexCanErrInt, 0);

	MCF_INTC1_ICRn(8) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(7);
	MCF_INTC1_ICRn(9) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(6);
	MCF_INTC1_ICRn(22) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(5);
	MCF_INTC1_ICRn(24) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(4);

	OS_IntEnable(MCF_INTC1_LVL_CAN0_BUF0);
	OS_IntEnable(MCF_INTC1_LVL_CAN0_BUF1);
	OS_IntEnable(MCF_INTC1_LVL_CAN0_BUF14);
	OS_IntEnable(MCF_INTC1_LVL_CAN0_ERR);
	
	//允许产生错误中断
	MCF_CAN_CANCTRL0 |= MCF_CAN_CANCTRL_ERRMSK;
	
	//设置CAN中断屏蔽寄存器，允许MB0/MB1/MB14产生正常发送/接收中断
	MCF_CAN_IMASK0 = MB_INT_MASK;

	//清除中断标志
	MCF_CAN_IFLAG0 = 0xffff;

	//设置CAN进入正常功能，开始工作
	MCF_CAN_CANMCR0 &= ~(MCF_CAN_CANMCR_MDIS|MCF_CAN_CANMCR_FRZ|MCF_CAN_CANMCR_HALT|\
						MCF_CAN_CANMCR_SUPV);

	OS_Sleep(10);

	//检查CAN是否准备好
	if((MCF_CAN_CANMCR0&MCF_CAN_CANMCR_NOTRDY) != 0)
	{
        LogError("FlexCanMBInit",FILE_LINE,"the CAN not ready, initailize fail.");
		return ERR_FAILURE;
	}

	//初始化接收MB1为点对点接收地址,即本模块通信地址
	MCF_CAN0_MBUFX_CTRL(CAN_MB1) = 0;
#ifdef CAN_MASTER
	MCF_CAN0_MBUFX_ID(CAN_MB1) = (M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET);
#else
	MCF_CAN0_MBUFX_ID(CAN_MB1) = (DB_GetDevAddress()<<SLAVE_MODULE_ADDR_OFFSET);
#endif
	MCF_CAN0_MBUFX_CTRL(CAN_MB1) = (MB_RX_EMPTY|MB_CTL_IDE_MASK);
	
	//初始化接收MB14为从模块组地址
	MCF_CAN0_MBUFX_CTRL(CAN_MB14) = 0;
	MCF_CAN0_MBUFX_ID(CAN_MB14) = (0x03<<SLAVE_MOD_GRP_ADD_OFFSET);
	MCF_CAN0_MBUFX_CTRL(CAN_MB14) |= (MB_RX_EMPTY|MB_CTL_IDE_MASK);
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     flexcan的MB功能初始化，根据需要可以初始化为发送、接收、不用功能，初始状态
 *				都为非激活状态
 *
 * 参数说明      - no		: 输入 	MB序号
 *       		 - type		: 输入	需要设置的功能类型
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanMBInit(BYTE no, BYTE type)
{
	ASSERT(no <= MAX_MB_NO);
	if(no>MAX_MB_NO)
	{
        LogError("FlexCanMBInit",FILE_LINE,"the MB no=%d ,and is invalide.",no);
		return;
	}

	//发送用MB
	if(type == CAN_MB_TXD)
	{
		ClearMDCode(no);
		MCF_CAN0_MBUFX_CTRL(no) |= MB_TX_INACTIVE;
	}
	else if(type == CAN_MB_RXD)	//接收用MB
	{
		ClearMDCode(no);
		MCF_CAN0_MBUFX_CTRL(no) |= MB_RX_INACTIVE;
	}
	else	//禁止使用
	{
		ClearMDCode(no);
	}
	
}


/*********************************************************************************************
 *
 * 功能描述     flexcan的MB0中断处理函数，由于MB0用作发送，一旦产生本中断说明发生了一次
 *				正确的数据发送。
 *
 * 参数说明      - arg		: 备用
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanInt0(void * arg)
{
	if(MCF_CAN_IFLAG0&MCF_CAN_IFLAG_BUF0I)	//检查MB中断标志
	{
		//清除对应中断标志
		ClearFlexCanIntFlg(CAN_MB0);
		
		FlexCanStatic.dwCanIntTxdOKCtn++;
		FlexCanTxd(CAN_MB0);
	}
	else	//不明原因中断
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
}

/*********************************************************************************************
 *
 * 功能描述     flexcan的MB1中断处理函数，由于MB1用作点对点数据接收，一旦产生本中断说明
 *				发生了一次正确的数据接收。
 *
 * 参数说明      - arg		: 备用
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanInt1(void * arg)
{
	if(MCF_CAN_IFLAG0&MCF_CAN_IFLAG_BUF1I)	//检查对应MB中断标志是否存在
	{
		//清除对应中断标志
		ClearFlexCanIntFlg(CAN_MB1);
		
		FlexCanStatic.dwCanIntRxdOKCtn++;
		FlexCanRxd(CAN_MB1);
	}
	else
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
}

/*********************************************************************************************
 *
 * 功能描述     flexcan的MB14中断处理函数，由于MB14用作组播数据接收，一旦产生本中断说明
 *				发生了一次正确的数据接收。
 *
 * 参数说明      - arg		: 备用
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanInt14(void * arg)
{
	if(MCF_CAN_IFLAG0&MCF_CAN_IFLAG_BUF14I)	//检查对应MB中断标志是否存在
	{
		//清除对应中断标志
		ClearFlexCanIntFlg(CAN_MB14);
		
		FlexCanStatic.dwCanIntRxdOKCtn++;
		FlexCanRxd(CAN_MB14);
	}
	else
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
}

/*********************************************************************************************
 *
 * 功能描述     中断服务程序中发送处理过程，完成后续分包的发送处理；一个完整的数据包发送结束后
 *				及时通知链路层管理任务。对于链路层命令(数据长度不超过8个)，一旦进入本过程，
 *				说明链路命令已经发送完成。
 *
 * 参数说明      - no		: 输入 	MB序号
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanTxd(BYTE no)
{
	WORD wMBCtl;
	
	MCF_CAN0_MBUFX_CTRL(no) = MB_TX_INACTIVE;

	//处理链路层发送过程，由于链路层命令短，当产生链路层发送中断时，说明链路命令已经发送完成
	
	TCanLinkFram *pLinkTxdBuf;
	pLinkTxdBuf = &pCanPortData[wFlexCanPort]->CanLinkCmdBuf.TxdBuf;
	
	if(pLinkTxdBuf->dwStatus == CAN_BUF_BUSY)
	{
		//清除链路发送缓冲区及所有标志，可以进行新链路层数据发送
		memset((void*)pLinkTxdBuf, 0 , sizeof(TCanLinkFram));
				
		//设置CAN网发送空闲标志标志
		CanPortAttr[wFlexCanPort].dwFlag |= CAN_PORT_TXD_IDLE;
		return;
	}

	//应用层报文数据发送处理
	TCanCellBuf *pApiTxdBuf;
	WORD len, sendnum;

	pApiTxdBuf = &pCanPortData[wFlexCanPort]->CanDevBuf.TxdBuf;
	if( pApiTxdBuf->dwStatus== CAN_BUF_BUSY )	//应用层发送缓冲区忙，说明还有数据没有发送结束，需要继续发送
	{
		//数据发送完成，结束发送状态
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
		{
			pApiTxdBuf->ptr = 0;
			
			//设置CAN网发送空闲标志标志
			CanPortAttr[wFlexCanPort].dwFlag |= CAN_PORT_TXD_IDLE;
			
			pApiTxdBuf->dwStatus = CAN_DATA_FINISH;	//数据发送结束
			return;
		}
		
		//计算发送数据长度
		sendnum = pApiTxdBuf->length - pApiTxdBuf->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;

		//将需要发送的数据送到对应MB的数据区中
		for(int i=0; i<len; i++)
			MCF_CAN0_MBUFX_BYTEX(no, i) = pApiTxdBuf->Buffer[pApiTxdBuf->ptr++];
		
		//根据要求，累加应用层分包序号，并写入对应寄存器
		pApiTxdBuf->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);
	
		//写入数据长度
		wMBCtl = (len<<MB_CLT_LEN_OFFSET);
		
		wMBCtl |= MB_CTL_IDE_MASK;	//扩展格式
		wMBCtl |= MB_TX_SEND_DATA;		//发送命令
		wMBCtl |= MB_CTL_SRR_MASK; //设置数据帧标志

		//检查本次发送是否为最后一个分包
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
		{
			pApiTxdBuf->dwCanID |= (0x01<<SUB_FRAM_END_OFFSET);
		}
		MCF_CAN0_MBUFX_ID(no) = pApiTxdBuf->dwCanID;
		
		MCF_CAN0_MBUFX_CTRL(no) =  wMBCtl;	//触发发送

		return;
	}

	//其它情况，设置CAN网发送空闲标志标志
	CanPortAttr[wFlexCanPort].dwFlag |= CAN_PORT_TXD_IDLE;
}

/*********************************************************************************************
 *
 * 功能描述     中断服务程序中接收处理过程。对于应用层分包，序号检查每个分包序号是否正确，当
 *				收到最后一个分包后，通知链路任务进行处理。对于链路层命令，一旦进入本函数，说明
 *				已经收到一个完整的链路报文。
 *
 * 参数说明      - no		: 输入 	MB序号
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanRxd(BYTE no)
{
	WORD len,  type, index;
	DWORD id;
	int i;
	char data;
	static int reg;
	
	//首先关闭接收MB功能,防止新数据进入
	ClearMDCode(no);

	reg = MCF_CAN_CANCTRL0;
	
	id = MCF_CAN0_MBUFX_ID(no);		//取接收ID

	if(id&(M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET))
		dwSelfFramCnt++;
	
	len = (MCF_CAN0_MBUFX_CTRL(no)&0x000f)>>MB_CLT_LEN_OFFSET;	//取数据长度
	len %= (CAN_LINK_BUF_LEN+1);
	type = (id>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK;			//检查报文类型
	
	//根据报文进行相应处理
	if(type&LINK_FRAM_FLAG)	//链路层层数据
	{
		TCanLinkFram *pLinkRxd;

		pLinkRxd = &pCanPortData[wFlexCanPort]->CanLinkCmdBuf.RxdBuf;
		
		//检查接收缓冲区是否空闲
		if(pLinkRxd->dwStatus != CAN_BUF_EMPTY)
		{
			//接收过载
			pLinkRxd->dwFlag |= RXD_BUF_OVERRUN;
			FlexCanStatic.dwCanRxdOverRunCnt++;
//			pCanPortData[wFlexCanPort]->CanStatic.dwLinkRxdOverRun++;
		}
		
		//保存链路数据
		for(i=0; i<len; i++)
			pLinkRxd->buffer[i] = MCF_CAN0_MBUFX_BYTEX(no, i);

		pLinkRxd->dwStatus = CAN_DATA_FINISH;	//接收数据完成，链路层可以处理

		//通知链路层任务,收到链路层报文
		OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wFlexCanPort, 0, 0), 0, 0, 0);
	}
	else	//应用层报文处理
	{
		TCanCellBuf *pApiRxd;

		pApiRxd = &pCanPortData[wFlexCanPort]->CanDevBuf.RxdBuf;
		
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
//					pCanPortData[wFlexCanPort]->CanStatic.dwApiRxdOverRun++;
				}
						
				//不管原来是不是有数据，直接覆盖原来的数据
				pApiRxd->length = 0;
				pApiRxd->ptr = 0;
			}	
			
			//保存数据
			pApiRxd->dwStatus = CAN_BUF_BUSY;
			for(i=0; i<len; i++)
				pApiRxd->Buffer[pApiRxd->ptr++] = MCF_CAN0_MBUFX_BYTEX(no, i);
			pApiRxd->length += len;
			
			pApiRxd->length %= CAN_FRAME_BUF_LEN;
			pApiRxd->ptr %= CAN_FRAME_BUF_LEN;
			
		}
		else
		{
			for(i=0; i<len; i++)
				data = MCF_CAN0_MBUFX_BYTEX(no, i);
			pApiRxd->dwFlag |= SUB_FRAM_INDEX_ERR;
//			pCanPortData[wFlexCanPort]->CanStatic.dwApiRxdErrCnt++;
		}
		
		//检查是不是最后一个分包
		if(id & (SUB_FRAM_END_MASK<<SUB_FRAM_END_OFFSET))
		{
			pApiRxd->dwStatus = CAN_DATA_FINISH;	//接收数据完成，链路层可以处理
			
			//最后一个分包接收正确，通知链路层任务进行肯定确认
			OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wFlexCanPort, 0, 0), 0, 0, 0);
		}
	}
	
	MCF_CAN0_MBUFX_CTRL(no) = (MB_RX_EMPTY|MB_CTL_IDE_MASK);
}

/*********************************************************************************************
 *
 * 功能描述     flexcan错误中断处理过程
 *
 * 参数说明      - arg		: 备用
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FlexCanErrInt(void * arg)
{
	WORD status;
	
	//读取错误状态寄存器，必须首先保存，因为读操作会清除其中的部分位信息
	status = MCF_CAN_ERRSTAT0;
	
	if(status & MCF_CAN_ERRSTAT_ERRINT)	//错误中断
	{
		FlexCanStatic.dwCanIntErrCnt++;
		
		//由于错误标志中对于发送接收类型没有明确区分，不单独统计接收、发送错误中断
		
	}
	else	//不明原因
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
	
	MCF_CAN_ERRSTAT0 = 0xffff;	//清除错误中断标志
}

/*********************************************************************************************
 *
 * 功能描述     flexcan数据发送，包括应用层数据包和链路层命令。本操作会触发一次发送中断
 *
 * 参数说明      - pBuf			: 输入 	发送数据结构指针
 *       		 - type			: 输入	数据类型
 *       		 - isNewWrite	: 输入	是否为重发
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD FlexCanWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite)
{
	DWORD id;
	WORD len, wMBCtl;
	WORD sendnum;

	MCF_CAN0_MBUFX_CTRL(CAN_MB0) = MB_TX_INACTIVE;
	
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
			MCF_CAN0_MBUFX_BYTEX(CAN_MB0, i) = pLinkTxd->buffer[i];
		
		len = pLinkTxd->length;

		//写入ID
/*		
#ifdef CAN_MASTER		
		pLinkTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
	pLinkTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		MCF_CAN0_MBUFX_ID(CAN_MB0) = pLinkTxd->dwCanID;

		pLinkTxd->dwStatus = CAN_BUF_BUSY;	//数据发送结束
		
		//写入数据长度
		wMBCtl = len<<MB_CLT_LEN_OFFSET;
		
		wMBCtl |= MB_CTL_IDE_MASK;	//扩展格式
		wMBCtl |= MB_TX_SEND_DATA; 	//发送命令
		wMBCtl |= MB_CTL_SRR_MASK; //设置数据帧标志

		MCF_CAN0_MBUFX_CTRL(CAN_MB0) =  wMBCtl; //触发发送
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

		//计算发送数据长度
		sendnum = pApiTxd->length - pApiTxd->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;
		
		//将需要发送的数据送到对应MB的数据区中
		for(int i=0; i<len; i++)
		{
			MCF_CAN0_MBUFX_BYTEX(CAN_MB0, i) = pApiTxd->Buffer[pApiTxd->ptr++];
		}
/*		
#ifdef CAN_MASTER		
		pApiTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
		pApiTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		if(pApiTxd->length<=CAN_LINK_BUF_LEN)
			pApiTxd->dwCanID |= (0x1<<SUB_FRAM_END_OFFSET);
		
		MCF_CAN0_MBUFX_ID(CAN_MB0) = pApiTxd->dwCanID;
		
		//写入数据长度
		wMBCtl = len<<MB_CLT_LEN_OFFSET;
		
		wMBCtl |= MB_CTL_IDE_MASK;	//扩展格式
		wMBCtl |= MB_TX_SEND_DATA; 	//发送命令
		wMBCtl |= MB_CTL_SRR_MASK; //设置数据帧标志

		pApiTxd->dwStatus = CAN_BUF_BUSY;
		
		MCF_CAN0_MBUFX_CTRL(CAN_MB0) =  wMBCtl; //触发发送
		
	}
	return ERR_SUCCESS;
}

DWORD FlexCanReset(WORD port)
{
	DWORD reg;
//	ReportMsg("FlexCanReset........");
	
	//复位CAN模块
	FlexCanSReset();
	FlexCanStatic.dwCanHardResetCnt++;

	OS_Sleep(10);
	FlexCanEnb();
	MCF_CAN_IMASK0 = MB_INT_MASK;
	MCF_CAN_IFLAG0 = 0xffff;
	
	//设置CAN进入正常功能，开始工作
	MCF_CAN_CANMCR0 &= ~(MCF_CAN_CANMCR_MDIS|MCF_CAN_CANMCR_FRZ|MCF_CAN_CANMCR_HALT|\
						MCF_CAN_CANMCR_SUPV);

	memset(&pCanPortData[wFlexCanPort]->CanDevBuf, 0, sizeof(TCanDevBuf));
	memset(&pCanPortData[wFlexCanPort]->CanLinkCmdBuf, 0, sizeof(TCanLinkCmdBuf));
	
	CanPortAttr[wFlexCanPort].dwFlag = (CAN_PORT_OPEN|CAN_PORT_TXD_IDLE|CAN_PORT_RXD_IDLE);
	
	return ERR_SUCCESS;
}


#endif
