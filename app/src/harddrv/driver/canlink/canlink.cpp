/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			canlink.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           CAN网链路层管理，以及对应于端口管理的操作接口                                                                      
*			                                                                
* 描述                                                                             
*                                                                                  
*       对链路层进行管理，链路层数据包括:模块层缓冲和端口层缓冲，其中模块层缓冲为每个通信                      
*       从模块提供一组收发缓冲区(包括链路层命令缓冲和应用层数据缓冲)；端口层缓冲则向上对应                                                                                  
*       具体的CAN端口，每个CAN端口对应一组，每组包括发送和接收缓冲，其中接收缓冲为环形缓冲区                                                                                 
*       发送缓冲为普通主备缓冲。                                                                                 
*                                                                                        
* 函数                                                                             
*                                                                                  
*      	CCanTask::Init		CAN链路任务初始化
*      	CCanTask::OnTimeOut	CAN链路定时消息处理                                                              
*      	CCanTask::ErrProc  	CAN链路错误处理                                                                         
*		CCanTask::RxdProc	模块层缓冲接收数据处理
*		CCanTask::TxdProc	模块层缓冲发送数据处理
*		CCanTask::PortBufProc		端口缓冲区处理
*		CCanTask::isPortTxdBufIdle	检查端口层发送缓冲区是否空闲
*		CCanTask::OnCanHardWare		CAN硬件数据收发消息响应处理
*		CCanTask::ApiRxdFramProc	接收应用报文处理
*		CCanTask::CanLinkInit		CAN链路层管理初始化
*		ChangeToCanID		根据从模块点对点地址和标志,转换为标准发送ID
*		isCanPortTxdIdle	检查指定CAN网端口是否发送空闲
*		CanOpen				CAN网端口管理接口，端口打开
*		CanRead				CAN网端口管理接口，从指定CAN网端口的接收端口缓冲区读取一定长度数据
*		CanWrite			CAN网端口管理接口，向指定CAN网端口的发送端口缓冲区写入一定长度数据
*		
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/9/7    初始创建                         
*                                                                                  
********************************************************************************************/
#include "canlink.h"
#include "candrv.h"
#include "daefunc.h"

BEGIN_MESSAGE_MAP(CCanTask,CApp)
	ON_SM_TIMEOUT(CCanTask)
	ON_SM_CANTXDRXDOK(CCanTask)
END_MESSAGE_MAP()


//地址映射表，数组下标为CAN通信地址，内容为模块设备偏移，用于直接定位具体的pCanModuleData
DWORD dwCanAddMapTab[MAX_MODULE_NUM];

//CAN链路层任务ID
DWORD dwCanLinkTaskID;

BOOL bCanLinkInit = FALSE;

//CAN端口数据缓冲
TCanPortData *pCanPortData[MAX_CAN_PORT];

//CAN网端口属性
TCanPort CanPortAttr[MAX_CAN_PORT];

DWORD canwritecnt = 0;

//CAN网链路层任务入口
void CanLinkTask(DWORD *pdwApp)
{
	CCanTask *pCanTask = new CCanTask(pdwApp);
	pCanTask->Init();
	
	bCanLinkInit = TRUE;
	
	pCanTask->Run();
}	

CCanTask::CCanTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

/*********************************************************************************************
 *
 * 功能描述     CAN链路任务初始化
 *
 * 参数说明      无
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::Init(void)
{
	
	dwCanLinkTaskID = GetTaskId();

	address = (BYTE)DB_GetDevAddress();
		
	CanLinkInit();
	
	SetTimer(1, CAN_LINK_TIME);
}

/*********************************************************************************************
 *
 * 功能描述     CAN链路定时消息处理，定时检查所有CAN端口，在端口打开的情况下，顺序完成如下
 *				操作:检查发送是否一直处于非空闲状态，若长时间处于发送非空闲态，进行异常处理；
 *				检查接收是否一直处于非空闲状态，若长时间处于接收非空闲态，进行异常处理；
 *				模块层数据接收处理(循环检查从模块的接收数据)；模块层数据发送处理(循环检查从
 *				模块的发送数据)；端口层数据缓冲区处理；对时处理。
 *
 * 参数说明      - id		: 输入 	备用
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::OnTimeOut(DWORD id)
{
	dwTimeCnt++;
	
	for(int i=0; i<MAX_CAN_PORT; i++)
	{
		//只有在CAN网端口打开的情况下才能进行数据处理
		if(CanPortAttr[i].dwFlag & CAN_PORT_OPEN)
		{
			//通道负荷监视
			PortLoadMonitor(i);

			//接收数据处理
			RxdProc(i);

			//定时检查端口层缓冲区，并进行相应处理。
			PortBufProc(i);

			//发送数据处理
			TxdProc(i);

			//主机对时
//			if( ( !(dwTimeCnt%CAN_BROADCAST_TIME) ) && (CanPortAttr[i].bEnableBroadTime == TRUE) )
//				BroadCastTime();
				
		}
	}
	
}

/*********************************************************************************************
 *
 * 功能描述     CAN通道负荷监视
 *
 * 参数说明      - port		: 输入 	通道号
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::PortLoadMonitor(BYTE port)
{
	
	//检查发送是否空闲
	if(CanPortAttr[port].dwFlag & CAN_PORT_TXD_IDLE)
		CanPortAttr[port].dwTxdBusyCnt = 0;
	else
	{
		CanPortAttr[port].dwTxdBusyCnt++;
		if(CanPortAttr[port].dwTxdBusyCnt>=CAN_PORT_BUSY_CNT)
		{
			CanPortAttr[port].dwTxdBusyCnt = 0;
			ErrProc(CAN_PORT_TXD_BUSY_ERR);
		}
	}
	
	//检查接收是否空闲
	if(CanPortAttr[port].dwFlag & CAN_PORT_RXD_IDLE)
		CanPortAttr[port].dwRxdBusyCnt = 0;
	else
	{
		CanPortAttr[port].dwRxdBusyCnt++;
		if(CanPortAttr[port].dwRxdBusyCnt>=CAN_PORT_BUSY_CNT)
		{
			CanPortAttr[port].dwRxdBusyCnt = 0;
			ErrProc(CAN_PORT_RXD_BUSY_ERR);
		}
	}
	
}

/*********************************************************************************************
 *
 * 功能描述     CAN链路错误处理
 *
 * 参数说明      - type		: 输入 	错误类型
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::ErrProc(BYTE type)
{
	
}

/*********************************************************************************************
 *
 * 功能描述     模块层缓冲接收数据处理:顺序检查所有从模块接收缓冲区，首先检查链路层、应用层数据
 *				是否过载，然后检查应用层接收报文是否准备好，若准备好则将该数据送给端口层环形接收
 *				缓冲区。
 *
 * 参数说明      - port		: 输入 	端口
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::RxdProc(BYTE port)
{
	TCanPortData *pCanChannel;
	TCanLinkFram *pLinkRxd;
	TCanCellBuf *pApiRxd;
	BYTE address;

	//处理接收缓冲区
//	for(int i=0; i<bModuleNum; i++)
	{
		pCanChannel = pCanPortData[port];

		//首先检查并处理链路层数据
		pLinkRxd = &pCanChannel->CanLinkCmdBuf.RxdBuf;

		//链路层接收数据完成，等待处理
		if(pLinkRxd->dwStatus == CAN_DATA_FINISH)
		{
			LinkRxdFramProc(port);
		}
		//链路层接收忙，异常
		else if(pLinkRxd->dwStatus == CAN_BUF_BUSY)
		{
			memset(pLinkRxd, 0, sizeof(TCanLinkFram));
			pLinkRxd->dwStatus = CAN_BUF_EMPTY;
			pCanChannel->CanStatic.dwLinkRxdErrCnt++;
		}

		//处理应用层数据缓冲区
		pApiRxd = &pCanChannel->CanDevBuf.RxdBuf;
		address = (BYTE)GET_CAN_MODULE_ADD(pApiRxd->dwCanID);
		
		//应用层报文接收完成，但是不一定正确
		if(pApiRxd->dwStatus == CAN_DATA_FINISH)
		{
			//检查接收缓冲区数据标志
			//有分包序号出错/帧总长度出错，丢弃本报文，并进行否定回答
			if((pApiRxd->dwFlag & SUB_FRAM_INDEX_ERR) || (pApiRxd->dwFlag & FRAM_LEN_ERR))
			{
				pCanChannel->CanStatic.dwApiRxdErrCnt++;
				TxdNAck(port);
				
				memset(pApiRxd, 0, sizeof(TCanCellBuf));
			}
			else	//接收数据正确，根据报文类型,写入应用层缓冲区，
			{
				if(pApiRxd->dwFlag & RXD_BUF_OVERRUN)
					pCanChannel->CanStatic.dwApiRxdOverRun++;
				
				TxdAck(port);
				ApiRxdFramProc(port);
			}
		}
		//应用层接收缓冲区忙，需要等待,如果长时间忙的话，说明链路有问题，需要重新复位链路
		else if(pApiRxd->dwStatus == CAN_BUF_BUSY)
		{
			pApiRxd->dwRxdBusyCnt++;

			//长时间持续接收忙，清除接收缓冲区，否定确认
			if(pApiRxd->dwRxdBusyCnt>CAN_RXD_BUSY_TIME)
			{
				pApiRxd->dwRxdBusyCnt = 0;
				pCanChannel->CanStatic.dwApiRxdErrCnt++;
				TxdNAck(port);
				
				memset(pApiRxd, 0, sizeof(TCanCellBuf));
			}
		}
	}
}


/*********************************************************************************************
 *
 * 功能描述    正常链路层报文接收处理
 *
 * 参数说明      - port	: 	输入 	CAN网端口号
 *
 * 返回代码
 *               成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CCanTask::LinkRxdFramProc(BYTE port)
{
	DWORD type, address;
	TCanStatic *pStatic;
	TCanCellBuf *pCellTxdBuf;
	
	TCanLinkFram *pLinkRxd = &pCanPortData[port]->CanLinkCmdBuf.RxdBuf;

	//接收统计处理
	pStatic = &pCanPortData[port]->CanStatic;
	pStatic->dwLinkRxdOkCnt++;

	if(pLinkRxd->dwFlag & RXD_BUF_OVERRUN)
		pStatic->dwLinkRxdOverRun++;

	//取链路层报文类型
	type = GET_CAN_FRAME_TYPE(pLinkRxd->dwCanID);
	address = GET_CAN_MODULE_ADD(pLinkRxd->dwCanID);
	
	//清除链路层接收缓存，等待新的处理
	memset(pLinkRxd, 0, sizeof(TCanLinkFram));
	
	switch(type)
	{
		//链路层肯定确认，说明发送数据正确被接收，清除应用层报文发送缓存，准备新发送
		case LINK_ACK_TYPE:
			if(pCanPortData[port]->CanDevBuf.TxdBuf.dwStatus == CAN_DATA_FINISH)
				memset(&pCanPortData[port]->CanDevBuf.TxdBuf,0, sizeof(TCanCellBuf));

			pStatic->dwApiTxdOkCnt++;
			break;
			
		//链路层否定确认，说明发送数据失败，对刚才的应用层报文进行重发
		case LINK_NACK_TYPE:
			//如果应用层数据发送缓冲区非空，重发该数据
			pCellTxdBuf = &pCanPortData[port]->CanDevBuf.TxdBuf;
			if(pCellTxdBuf->dwStatus != CAN_BUF_EMPTY)
			{
				pCellTxdBuf->dwStatus = CAN_DATA_READY;
				CanHardWrite(port, GET_CAN_MODULE_ADD(pCellTxdBuf->dwCanID), CAN_API_FRAME, FALSE);
			}
			pStatic->dwApiTxdErrCnt++;
			break;
		//复位链路，清除当前链路层发送缓存，进行肯定确认	
		case LINK_RESET_TYPE:
			ReportMsg("receive can%d link reset command.", port+1);
			CanHardReset(port);
			TxdAck(port);
			break;
		case LINK_BROAD_TIME:
			break;
		default:
			break;
	}

	return ERR_SUCCESS;
}

//链路层肯定确认报文发送
DWORD CCanTask::TxdAck(BYTE port)
{
	DWORD id = 0;
	TCanLinkFram *pLinkTxd;

	pLinkTxd = &pCanPortData[port]->CanLinkCmdBuf.TxdBuf;
	
	//报文类型
	id = LINK_ACK_TYPE<<LINK_FRAM_TYPE_OFFSET;
	
	//模块地址
//#ifdef CAN_MASTER
	id |= ((DWORD)address)<<SLAVE_MODULE_ADDR_OFFSET;
//#endif
	//主机地址
	id |= 0x1<<M_MANAGER_ADDR_OFFSET;
	//备机地址
	id |= 0x1<<B_MANAGER_ADDR_OFFSET;

	id |= 0x1<<SUB_FRAM_END_OFFSET;
	
	pLinkTxd->dwStatus = CAN_DATA_READY;
	pLinkTxd->dwFlag = 0;

	//由于报文类型在ID中可以完全识别，对于链路层报文，除了对时外，数据域中的内容无意义，定义长度为1，内容0xaa;
	pLinkTxd->length = 1;
	pLinkTxd->buffer[0] = 0x55;

	pLinkTxd->dwCanID = id;

	//发送报文
	return CanHardWrite(port, address, CAN_LINK_FRAME, TRUE);
}

//链路层否定确认报文发送
DWORD CCanTask::TxdNAck(BYTE port)
{
	DWORD id = 0;
	TCanLinkFram *pLinkTxd;

	pLinkTxd = &pCanPortData[port]->CanLinkCmdBuf.TxdBuf;
	
	//报文类型
	id = LINK_NACK_TYPE<<LINK_FRAM_TYPE_OFFSET;
	//模块地址
//#ifdef CAN_MASTER
	id |= ((DWORD)address)<<SLAVE_MODULE_ADDR_OFFSET;
//#endif
	//主机地址
	id |= 0x1<<M_MANAGER_ADDR_OFFSET;
	//备机地址
	id |= 0x1<<B_MANAGER_ADDR_OFFSET;

	id |= 0x1<<SUB_FRAM_END_OFFSET;
	
	pLinkTxd->dwStatus = CAN_DATA_READY;
	pLinkTxd->dwFlag = 0;

	//由于报文类型在ID中可以完全识别，对于链路层报文，除了对时外，数据域中的内容无意义，定义长度为1，内容0xaa;
	pLinkTxd->length = 1;
	pLinkTxd->buffer[0] = 0xaa;

	pLinkTxd->dwCanID = id;

	//发送报文
	return CanHardWrite(port, address, CAN_LINK_FRAME, TRUE);
}

/*********************************************************************************************
 *
 * 功能描述     模块层缓冲发送数据处理，对于每个从模块顺序进行如下处理:检查端口缓冲区应用层报
 *				文是否发送应答超时，如果超时进行3次重发，重发不成功则丢弃该应用报文；检查端口
 *				缓冲区是否有链路层命令需要发送，如果有则进行发送；检查端口缓冲区是否有应用层命
 *				令需要发送，如果有则进行发送
 *
 * 参数说明      - port		: 输入 	端口
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::TxdProc(BYTE port)
{
	TCanPortData *pCanTxd;
	BYTE address;
	TCanCellBuf *pApiFramTxd;
	
	//for(int i=0; i<bModuleNum; i++)
	{
		pCanTxd = pCanPortData[port];

		address = pCanTxd->address;
		
		pApiFramTxd = &pCanTxd->CanDevBuf.TxdBuf;
		
		//链路层命令不考虑重发，只考虑应用层数据的重发。对于超时无应答，重发次数最大3次，
		//超过3次，丢弃该发送报文。而对于否定应答会一直做重发处理
		//检查是否有应用层数据需要发送
		if(pApiFramTxd->dwStatus == CAN_DATA_READY)
		{
			//注意每次只能发送一个地址的数据，因为端口只有一个
			CanHardWrite(port, address, CAN_API_FRAME, TRUE);
			return;
		}
		//处于发送完等待接收状态或发送忙
		else if((pApiFramTxd->dwStatus == CAN_DATA_FINISH) || (pApiFramTxd->dwStatus == CAN_BUF_BUSY))
		{
			
			//超时重发时间计数
			pApiFramTxd->wResendDelay++;

			//超时重发处理
			if(pApiFramTxd->wResendDelay >= CAN_RESEND_TIME)
			{
				pApiFramTxd->wResendDelay = 0;

				//超过重发次数，则丢弃本发送报文
				if(pApiFramTxd->wResendCnt >= CAN_RESEND_LEVEL)
				{
					//清除应用报文发生缓冲区
					memset((void*)pApiFramTxd, 0, sizeof(TCanCellBuf));
					
					//CanHardReset(port);
				}
				else	//重发处理
				{
					
					pApiFramTxd->dwStatus = CAN_DATA_READY;
					CanHardWrite(port, address, CAN_API_FRAME, FALSE);
					pApiFramTxd->wResendCnt++;
					return;
				}
			}
			
		}
		else	//不处于发送完的等待接收状态，则清除重发等待计数值
		{
			pApiFramTxd->wResendDelay = 0;
		}

	}
}

/*********************************************************************************************
 *
 * 功能描述     端口缓冲区处理，检查端口发送缓冲区是否空闲，空闲则通知应用层任务；否则将端口
 *				发送缓冲区数据复制到对应的模块发送缓冲区中。
 *
 * 参数说明      - port		: 输入 	端口
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
extern TSysConfigTable	*G_pCfgTable;
void CCanTask::PortBufProc(BYTE port)
{
	//端口发送缓冲区全部空闲，通知应用层发送空闲
	if(isPortTxdBufIdle(port) == TRUE)
	{
		pCanPortData[port]->dwComIdleCnt++;
		if(pCanPortData[port]->dwComIdleCnt > pCanPortData[port]->dwComIdleLevel)
		{
//			if(port == 1)
//				printf("report com idle =%d, idlelevel=%d.\n", G_pCfgTable->SystemStatus.stTime.Lo%10000,\
//					pCanPortData[port]->dwComIdleLevel);
			OS_PostMessage(CanPortAttr[port].dwTargetId, SM_COMMIDLE, port, 0, 0, 0);
			pCanPortData[port]->dwComIdleCnt = 0;
		}
	}
	else
	{
		pCanPortData[port]->dwComIdleCnt = 0;
		
		//端口发送缓冲区非空，将数据复制到对应地址的链路层应用报文缓冲区
		PortBuf2FramBuf(port);
	}
}

/*********************************************************************************************
 *
 * 功能描述     检查端口层发送缓冲区是否空闲
 *
 * 参数说明      - port		: 输入 	端口
 *
 * 返回代码
 *               是或否 
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL CCanTask::isPortTxdBufIdle(BYTE port)
{
	TCanApiFram *pApiNBuf, *pApiUBuf;

	pApiNBuf = &pCanPortData[port]->CanApiNFram;
	pApiUBuf = &pCanPortData[port]->CanApiUFram;

	//普通报文和紧急报文发送缓冲区都空闲
	if( (pApiNBuf->TxdRBufNo == pApiNBuf->TxdWBufNo) && (pApiUBuf->TxdRBufNo == pApiUBuf->TxdWBufNo) )
		return TRUE;
	else 
		return FALSE;
}

/*********************************************************************************************
 *
 * 功能描述     端口发送缓冲区数据复制到对应地址的链路层应用报文缓冲区,但是必须保证链路层应用报文缓冲区
 *				应用报文缓冲区为空闲，否则不能覆盖
 *
 * 参数说明      - port		: 输入 	端口
 *
 * 返回代码
 *               是或否 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::PortBuf2FramBuf(BYTE port)
{
	TCanFramBuf *pTxdBuf;
	TCanCellBuf *pLinkBuf;
	TCanApiFram *pApiBuf = NULL;

	//首先检查紧并处理急报文发送缓冲区,然后处理普通报文
	if(pCanPortData[port]->CanApiUFram.TxdRBufNo != pCanPortData[port]->CanApiUFram.TxdWBufNo)
		pApiBuf = &pCanPortData[port]->CanApiUFram;
	else if(pCanPortData[port]->CanApiNFram.TxdRBufNo != pCanPortData[port]->CanApiNFram.TxdWBufNo)
		pApiBuf = &pCanPortData[port]->CanApiNFram;
	
	if(pApiBuf == NULL)
		return;

	//有数据需要发送，将数据送到应用链路层数据发送缓冲区
	pTxdBuf = &pApiBuf->TxdBuf[pApiBuf->TxdRBufNo];
	pLinkBuf = &pCanPortData[port]->CanDevBuf.TxdBuf;

	//检查链路层应用报文发送缓冲区是否空，非空的话不能覆盖该缓冲区
	if(pLinkBuf->dwStatus != CAN_BUF_EMPTY)
	{
//		ReportMsg("link data txd buffer busy, send frame failed, status = %x.", pLinkBuf->dwStatus);
		return;
	}

	//数据复制到对应缓冲区
	if(pApiBuf->TxdRBufNo != pApiBuf->TxdWBufNo)
	{
		memset(pLinkBuf, 0, sizeof(TCanCellBuf));
		memcpy(pLinkBuf->Buffer, pTxdBuf->Buffer, pTxdBuf->length);
		pLinkBuf->dwCanID = pTxdBuf->dwCanID;
		pLinkBuf->length = pTxdBuf->length;
		pLinkBuf->dwStatus = CAN_DATA_READY;

		//将原始缓冲区读指针更新
		pApiBuf->TxdRBufNo++;
		pApiBuf->TxdRBufNo %= CAN_API_BUF_NUM;
	}
}

/*********************************************************************************************
 *
 * 功能描述     CAN硬件数据收发消息响应处理，消息类型包括:完整接收到应用层数据、收到链路层肯定
 *				回答、收到链路层否定回答、收到对时命令等。
 *
 * 参数说明      - arg		: 输入 	消息参数
 *
 * 返回代码
 *               是或否 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::OnCanHardWare(DWORD arg)
{
	BYTE type, port;
	TCanStatic *pStatic;
	TCanCellBuf *pApiRxd;

	TCanPortData *pCanChannel;
	TCanCellBuf *pCellTxdBuf;

	//解析消息的信息体
	type = (BYTE)arg;
	port = (BYTE)(arg>>8);
	ASSERT(port<MAX_CAN_PORT);
	if(port>=MAX_CAN_PORT)
	{
		LogError("OnCanTxdRxdOK",FILE_LINE,"the port number invalide.");
		return;
	}
//	address = (BYTE)(arg>>16);

	pCanChannel = pCanPortData[port];
	pStatic = &pCanPortData[port]->CanStatic;

	switch(type)
	{
		case API_N_FRAM_TYPE:	//应用层报文接收完成，还需要检查是否完整
		case API_U_FRAM_TYPE:
			pApiRxd = &pCanChannel->CanDevBuf.RxdBuf;
//			address = (BYTE)GET_CAN_MODULE_ADD(pApiRxd->dwCanID);
			
			//应用层报文接收完成，但是不一定正确
			if(pApiRxd->dwStatus == CAN_DATA_FINISH)
			{
				//检查接收缓冲区数据标志
				//有分包序号出错/帧总长度出错，丢弃本报文，并进行否定回答
				if((pApiRxd->dwFlag & SUB_FRAM_INDEX_ERR) || (pApiRxd->dwFlag & FRAM_LEN_ERR))
				{
					pCanChannel->CanStatic.dwApiRxdErrCnt++;
					
					memset(pApiRxd, 0, sizeof(TCanCellBuf));
					TxdNAck(port);
				}
				else	//接收数据正确，根据报文类型,写入应用层缓冲区，
				{
					if(pApiRxd->dwFlag & RXD_BUF_OVERRUN)
						pCanChannel->CanStatic.dwApiRxdOverRun++;
					pCanChannel->CanStatic.dwApiRxdOkCnt++;
					
					TxdAck(port);
					
					ApiRxdFramProc(port);
				}
			}
			break;
		case LINK_ACK_TYPE:	//接收到链路层肯定回答
			
			//清除链路层接收缓存，等待新的处理
			memset(&pCanPortData[port]->CanLinkCmdBuf.RxdBuf, 0, sizeof(TCanLinkFram));

			//说明刚才发送的应用层数据被对方正确接收，可以进行新应用层数据的发送
			if(pCanPortData[port]->CanDevBuf.TxdBuf.dwStatus == CAN_DATA_FINISH)
				memset(&pCanPortData[port]->CanDevBuf.TxdBuf,0, sizeof(TCanCellBuf));
			
			pStatic->dwApiTxdOkCnt++;
			
//			//发送消息给应用层任务,发送空闲可以发送新应用层数据
//			OS_PostMessage(CanPortAttr[port].dwTargetId, SM_COMMIDLE, port, address, 0, 0);
			break;
		case LINK_NACK_TYPE:	//收到链路层否定回答，需要重发应用层数据
			//清除链路层接收缓存，等待新的处理
			memset(&pCanPortData[port]->CanLinkCmdBuf.RxdBuf, 0, sizeof(TCanLinkFram));

			//如果应用层数据发送缓冲区非空，重发该数据
			pCellTxdBuf = &pCanPortData[port]->CanDevBuf.TxdBuf;
			if(pCellTxdBuf->dwStatus != CAN_BUF_EMPTY)
			{
				pCellTxdBuf->dwStatus = CAN_DATA_READY;
				CanHardWrite(port, GET_CAN_MODULE_ADD(pCellTxdBuf->dwCanID), CAN_API_FRAME, FALSE);
			}
			pStatic->dwApiTxdErrCnt++;
			break;
		case LINK_BROAD_TIME:		//接收到广播对时命令
			//设置系统时钟
			
			//清除链路层接收缓存，等待新的处理
			memset(&pCanPortData[port]->CanLinkCmdBuf.RxdBuf, 0, sizeof(TCanLinkFram));

			break;
		case LINK_RESET_TYPE://复位链路
			//清除链路层接收缓存，等待新的处理
			CanHardReset(port);
			TxdAck(port);
			break;
		default:
			break;
	}
}

/*********************************************************************************************
 *
 * 功能描述     接收应用报文处理，将完整的模块层应用接收报文复制到端口层环形接收缓冲区，并
 *				通知应用层任务处理。
 *
 * 参数说明      - port		: 输入 	CAN网端口序号
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CCanTask::ApiRxdFramProc(BYTE port)
{
	TCanCellBuf *pRxdFrm;
	TCanApiFram *pApiRxd;
	DWORD type;
	BYTE *pFrame;

	pRxdFrm = &pCanPortData[port]->CanDevBuf.RxdBuf;

	type = (pRxdFrm->dwCanID>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK;

	//紧急报文和普通报文分别保存
	if(type == API_U_FRAM_TYPE)
		pApiRxd = &pCanPortData[port]->CanApiUFram;
	else
		pApiRxd = &pCanPortData[port]->CanApiNFram;


	//保存数据到应用层缓冲区,更新写缓冲区序号
	OS_Protect(0);
	pFrame = pRxdFrm->Buffer;
	memcpy(&pApiRxd->RxdBuf[pApiRxd->RxdWBufNo].Buffer, pRxdFrm->Buffer, pRxdFrm->length);
	pApiRxd->RxdBuf[pApiRxd->RxdWBufNo].length = pRxdFrm->length;
	pApiRxd->RxdBuf[pApiRxd->RxdWBufNo].dwCanID = pRxdFrm->dwCanID;
	pApiRxd->RxdWBufNo++;
	pApiRxd->RxdWBufNo %= CAN_API_BUF_NUM;

	//同时将接收到的应用层报文送到监视缓冲区
	TCanMonitorBuf *pMonitor = &pCanPortData[port]->MonitorBuf;
	BYTE *pBuf = pMonitor->RxdBuffer;
	for(int i=0; i<pRxdFrm->length; i++)
	{
		pBuf[pMonitor->wRxdWritePtr++] = pFrame[i];
		pMonitor->wRxdWritePtr %= CAN_MONITOR_BUF_LEN;
	}
	
	OS_UnProtect(0);

	//清除链路层应用报文缓存
	memset(pRxdFrm, 0, sizeof(TCanCellBuf));
	
	//发送消息给应用层任务
	OS_PostMessage(CanPortAttr[port].dwTargetId, SM_READCOMM, 0, 0, 0, 0);
	
}

/*********************************************************************************************
 *
 * 功能描述     CAN链路层管理初始化，根据模块数目，为模块层和端口层分配对应缓冲区。
 *
 * 参数说明      无
 *
 * 返回代码
 *               成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CCanTask::CanLinkInit()
{
	BYTE i;
//	BYTE *pTxdBuf, *pRxdBuf;
//	TCanPortData *pModule;
	
	memset((void*)CanPortAttr, 0, sizeof(TCanPort)*MAX_CAN_PORT);

	//初始化每个CAN模块
	for(i=0; i<MAX_CAN_PORT; i++)
	{
		//初始化CAN端口属性
		
		pCanPortData[i] = (TCanPortData*)OS_MemAlloc(sizeof(TCanPortData));
		ASSERT(pCanPortData[i] != NULL);
		if(pCanPortData[i] == NULL)
		{
			LogError("CanLinkInit",FILE_LINE,"malloc for pCanModuleData invalide.");
			return ERR_FAILURE;
		}

		memset((void*)pCanPortData[i], 0, sizeof(TCanPortData));

	}

	//设置默认COMIDLE 限值为500毫秒
	pCanPortData[i]->dwComIdleLevel = 500/CAN_LINK_TIME;
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     根据从模块点对点地址和标志(主机地址、备机地址、从模块组播地址)，转换为标准
 *				发送ID,即基于SJA1000标准的ID.
 *
 * 参数说明      - address	: 输入 	从模块点对点地址
 *				 - flag		: 输入	地址标识
 *
 * 返回代码
 *               标准ID
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD ChangeToCanID(BYTE address, BYTE flag)
{
	DWORD id = 0;

	id = address;

	id <<= SLAVE_MODULE_ADDR_OFFSET;	//转换从模块地址
	if(flag & MAIN_MANGER_ADDR)			//主机地址
		id |= (0x01<<M_MANAGER_ADDR_OFFSET);
	if(flag & BACK_MANAGER_ADDR)		//备机地址
		id |= (0x01<<B_MANAGER_ADDR_OFFSET);
	if(flag & SLAVE_MODULE_GRP_ADDR)	//从模块组播地址
		id |= (0x03<<SLAVE_MOD_GRP_ADD_OFFSET);
	return id;
}

/*********************************************************************************************
 *
 * 功能描述    检查指定CAN网端口是否发送空闲
 *
 * 参数说明      - port		: 输入 	CAN网端口序号
 *
 * 返回代码
 *               发送是否空闲
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL isCanPortTxdIdle(BYTE port)
{
	//检查CAN发送是否空闲，如果不空闲则不能发送数据
	if( CanPortAttr[port].dwFlag & CAN_PORT_TXD_IDLE )
		return TRUE;
	else
		return FALSE;
}


/*********************************************************************************************
 *
 * 功能描述    CAN网端口管理接口，端口打开
 *
 * 参数说明      - bPortSort	: 输入 	CAN网端口号
 *				 - dwFlag		: 输入	标识
 *
 * 返回代码
 *               成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanOpen(BYTE bPortSort, DWORD dwFlag)
{
	DWORD status;
	ASSERT(bPortSort>0);
	if(bPortSort == 0)
	{
		LogError("CanOpen",FILE_LINE,"the port number invalide.");
		return ERR_FAILURE;
	}

	//检查是否允许对时(即发送对时报文)
	if(dwFlag & ENABLE_BROAD_TIME)
		CanPortAttr[bPortSort-1].bEnableBroadTime = TRUE;
	else
		CanPortAttr[bPortSort-1].bEnableBroadTime = FALSE;
	
	status = CanHardOpen(bPortSort-1);
	if(status == ERR_SUCCESS)
	{
		CanPortAttr[bPortSort-1].dwFlag = CAN_PORT_OPEN|CAN_PORT_TXD_IDLE|CAN_PORT_RXD_IDLE;
	}

	//获取端口管理任务ID
	CanPortAttr[bPortSort-1].dwTargetId = RtosGetCurrentTaskId();
	
	return status;
}

/*********************************************************************************************
 *
 * 功能描述    CAN网端口管理接口，从指定CAN网端口的接收端口缓冲区中读取一定长度的数据。
 *
 * 参数说明      - bPortSort	: 输入 	CAN网端口号
 *				 - buf			: 输出	读取数据缓冲区
 *				 - dwLen		: 输入	准备读取的数据长度
 *				 - dwFlag		: 输入	标识
 *
 * 返回代码
 *               实际读取的数据长度
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanRead(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag)
{
	WORD len;
	TCanPortData *pModule;
	TCanApiFram *pRxdFram;
	
	ASSERT(bPortSort>0);
	if((bPortSort == 0) ||(bPortSort>MAX_CAN_PORT))
	{
		LogError("CanWrite",FILE_LINE,"the port number invalide.");
		return 0;
	}

	//检查数据长度是否有效
	if( (buf == NULL) || (dwLen == 0))
		return 0;

//	index = dwCanAddMapTab[dwFlag&0xff];
	pModule = pCanPortData[bPortSort-1];

	//首先处理接收到紧急报文
	pRxdFram = &pModule->CanApiUFram;
	if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
	{
		len = pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].length;
		OS_Protect(0);
		memcpy(buf, pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].Buffer, len);
		pRxdFram->RxdRBufNo++;
		pRxdFram->RxdRBufNo %= CAN_API_BUF_NUM;
		OS_UnProtect(0);

		//检查是否还有后续报文需要处理,通知应用层任务
		if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
			OS_PostMessage(CanPortAttr[bPortSort-1].dwTargetId, SM_READCOMM, 0, 0, 0, 0);

		return len;
	}

	//处理普通接收报文
	pRxdFram = &pModule->CanApiNFram;
	if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
	{
		len = pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].length;
		OS_Protect(0);
		memcpy(buf, pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].Buffer, len);
		pRxdFram->RxdRBufNo++;
		pRxdFram->RxdRBufNo %= CAN_API_BUF_NUM;
		OS_UnProtect(0);

		//检查是否还有后续报文需要处理,通知应用层任务
		if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
			OS_PostMessage(CanPortAttr[bPortSort-1].dwTargetId, SM_READCOMM, 0, 0, 0, 0);

		return len;
	}

	return 0;
}

/*********************************************************************************************
 *
 * 功能描述    CAN网端口管理接口，向指定CAN网端口的发送端口缓冲区写入一定长度的数据。
 *
 * 参数说明      - bPortSort	: 输入 	CAN网端口号
 *				 - buf			: 输出	原始写入数据缓冲区
 *				 - dwLen		: 输入	准备写入的数据长度
 *				 - dwFlag		: 输入	标识,最低字节表示从模块地址，最高位标示是否为紧急报文
 *
 * 返回代码
 *               实际写入的数据长度
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanWrite(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag)
{
	WORD len, num;
	TCanApiFram *pTxdFram;
	DWORD type;
//	static DWORD lotime, errcnt;
//	DWORD time;

	ASSERT(bPortSort>0);
	if((bPortSort == 0) ||(bPortSort>MAX_CAN_PORT))
	{
		LogError("CanWrite",FILE_LINE,"the port number invalide.");
		return 0;
	}

	//检查数据长度是否有效
	if( (buf == NULL) || (dwLen == 0) || (dwLen>CAN_FRAME_BUF_LEN))
		return 0;
/*
	if(bPortSort == 2)
	{
		time = G_pCfgTable->SystemStatus.stTime.Lo;
		printf("SJA100 CanWrite time=%d.\n", G_pCfgTable->SystemStatus.stTime.Lo%10000);

		if((time-lotime)<80)
			errcnt++;
		lotime = time;
	}
*/	
	//分别处理普通报文和紧急报文
	if(dwFlag &0x80000000)
	{
		pTxdFram = &pCanPortData[bPortSort-1]->CanApiUFram;
		type = API_U_FRAM_TYPE<<LINK_FRAM_TYPE_OFFSET;
	}
	else
	{
		pTxdFram = &pCanPortData[bPortSort-1]->CanApiNFram;
		type = API_N_FRAM_TYPE<<LINK_FRAM_TYPE_OFFSET;
	}

	//缓冲区数据传递
	num = pTxdFram->TxdWBufNo;
	OS_Protect(0);
	memcpy(pTxdFram->TxdBuf[num].Buffer, buf, dwLen);
	pTxdFram->TxdBuf[num].length = dwLen;
	
//#ifdef CAN_MASTER
	pTxdFram->TxdBuf[num].dwCanID = (DB_GetDevAddress()&0x00ff)<<SLAVE_MODULE_ADDR_OFFSET;
//	printf("Can%d Write, module address=%d.\n ", bPortSort-1, DB_GetDevAddress());
/*
#else
	pTxdFram->TxdBuf[num].dwCanID = 0;
#endif
*/
	//设置发送主备机地址
	pTxdFram->TxdBuf[num].dwCanID |= M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET;
	pTxdFram->TxdBuf[num].dwCanID |= B_MANAGER_ADDR_MASK<<B_MANAGER_ADDR_OFFSET;

	//设置报文类型标示
	pTxdFram->TxdBuf[num].dwCanID |= type;
	
	//移动发送数据写指针
	pTxdFram->TxdWBufNo++;
	pTxdFram->TxdWBufNo %= CAN_API_BUF_NUM;

	//同时将数据复制到监视缓冲区
	BYTE *pBuf = pCanPortData[bPortSort-1]->MonitorBuf.TxdBuffer;
	TCanMonitorBuf *pMonitor = &pCanPortData[bPortSort-1]->MonitorBuf;
	for(int i=0; i<dwLen; i++)
	{
		pBuf[pMonitor->wTxdWritePtr++] = buf[i];
		pMonitor->wTxdWritePtr %= CAN_MONITOR_BUF_LEN;
	}
	OS_UnProtect(0);

	return dwLen;
}

/*********************************************************************************************
 *
 * 功能描述    CAN网端口控制接口，主要用来控制COMIDLE时间间隔
 *
 * 参数说明      - bPortSort	: 输入 	CAN网端口号
 *				 - comm			: 输出	控制参数
 *
 * 返回代码
 *               
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanCtrl(BYTE portSort,TCtlComm *comm)
{
	BYTE port;
	
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("CanCtrl",FILE_LINE,"the port number invalide.");
		return ERR_FAILURE;
	}

	port = portSort - 1;
	//读取控制参数
	if(comm->dwFuncCode == FC_GET)
	{
		*comm = pCanPortData[port]->CommCtl;
		return ERR_SUCCESS;
	}

	if(comm->dwFuncCode == FC_SET)
	{
		pCanPortData[port]->CommCtl = *comm;
		
		if(comm->dwCtlMask & CM_NOTIFYCOMMBREAK)
		{
//			if(port == 1)
//				ReportMsg("set CAN%d comidle level=%dms", port, comm->dwNotifyCommBreak);
			
			//重新设置COMIDLE限值，并清除当前计数值
			//pCanPortData[port]->dwComIdleCnt = 0;
			pCanPortData[port]->dwComIdleLevel = comm->dwNotifyCommBreak/CAN_LINK_TIME;
		}
		return ERR_SUCCESS;
	}
	
	return ERR_FAILURE;
}

WORD GetCanTxdWritePtr(WORD portSort)
{
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("GetCanTxdWritePtr",FILE_LINE,"the port number invalide.");
		return 0;
	}
	
	return pCanPortData[portSort-1]->MonitorBuf.wTxdWritePtr;
}

WORD GetCanRxdWritePtr(WORD portSort)
{
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("GetCanRxdWritePtr",FILE_LINE,"the port number invalide.");
		return 0;
	}
	
	return pCanPortData[portSort-1]->MonitorBuf.wRxdWritePtr;
}

/*********************************************************************************************
 *
 * 功能描述      从CAN通道发送缓冲区指定位置读取一定长度的数据，供维护查看
 *
 * 参数说明   	- portSort:	输入	本类端口中的端口序号   
 *				- pBuf	: 	输出	需要读取的数据缓存	
 *				- wLen :	输入	准备读取的数据长度
 *				- wOffset:	输入	相对于起始地址的偏移
 *
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD ReadFromCanTxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;
	BYTE *pCanTxd;
	
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadFromCanTxdBuf",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanTxd = pCanPortData[portSort-1]->MonitorBuf.TxdBuffer;
	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = pCanTxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
		
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      从CAN通道接收缓冲区指定位置读取一定长度的数据，供维护查看
 *
 * 参数说明   	- portSort:	输入	本类端口中的端口序号   
 *				- pBuf	: 	输出	需要读取的数据缓存	
 *				- wLen :	输入	准备读取的数据长度
 *				- wOffset:	输入	相对于起始地址的偏移
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD ReadFromCanRxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;
	BYTE *pCanRxd;

	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadFromCanRxdBuf",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanRxd = pCanPortData[portSort-1]->MonitorBuf.RxdBuffer;
	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = pCanRxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
	
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      获取发送写指针之前的新数据，供维护查看
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD ReadNewCanTxdData(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i, wTxdWritePtr;
	BYTE *pCanTxd;
		
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadFromCanRxdBuf",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanTxd = pCanPortData[portSort-1]->MonitorBuf.TxdBuffer;
	wTxdWritePtr =  pCanPortData[portSort-1]->MonitorBuf.wTxdWritePtr;
		
	if(wTxdWritePtr >= wOffset)
		wLeft = wTxdWritePtr - wOffset;
	else
		wLeft = CAN_MONITOR_BUF_LEN - (wOffset-wTxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
		
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = pCanTxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
	
	return wLeft;
}

/*********************************************************************************************
 *
 * 功能描述      获取接收写指针之前的新数据，供维护查看
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD ReadNewCanRxdData(WORD portSort,BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i, wRxdWritePtr;
	BYTE *pCanRxd;

	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadNewCanRxdData",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanRxd = pCanPortData[portSort-1]->MonitorBuf.RxdBuffer;
	wRxdWritePtr =  pCanPortData[portSort-1]->MonitorBuf.wRxdWritePtr;

	if(wRxdWritePtr >= wOffset)
		wLeft = wRxdWritePtr - wOffset;
	else
		wLeft = CAN_MONITOR_BUF_LEN - (wOffset-wRxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
	
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = pCanRxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
	
	return wLeft;
}


#define CAN_DEBUG
#ifdef CAN_DEBUG

extern DWORD dwSelfFramCnt;

BEGIN_MESSAGE_MAP(CCanTest,CApp)
	ON_SM_TIMEOUT(CCanTest)
	ON_SM_READCOMM(CCanTest)
	ON_SM_COMMIDLE(CCanTest)
END_MESSAGE_MAP()

void CanTestTask(DWORD *pdwApp);

void CanTestTask(DWORD *pdwApp)
{

	CCanTest *pCanTest = new CCanTest(pdwApp);
	pCanTest->Init();
	
	pCanTest->Run();

}

CCanTest::CCanTest(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CCanTest::Init(void)
{
	int i;
#ifdef 	CAN_MASTER
	ReportMsg("Master can task is running...");
#else
	ReportMsg("Slaver can task is running...");
#endif
	flag = 0;
	timecounter = 0;
	writecnt = readcnt = 0;

	port = SJA1000_PORT;
	
	for(i=0; i<sizeof(bTxdBuf); i++)
		bTxdBuf[i] = i;

	for(i=0; i<sizeof(bAnswerFram); i++)
		bAnswerFram[i] = 0x80+i;

	address = DB_GetDevAddress();
	
	SetTimer(1, 100);
}

void CCanTest::OnTimeOut(DWORD id)
{
	if(bCanLinkInit == TRUE)
	{
		if(flag == 0)
		{
			CanOpen(port, 0);
			flag = 0xff;
		}
#ifdef CAN_MASTER
		if(flag == 0xff)
		{
			CanWrite(port, bTxdBuf, 32, address);
			writecnt++;
		}
#endif			
	}

	timecounter++;
	if((timecounter%20) == 0)
	{
#ifdef CAN_MASTER
		ReportMsg("send fram counter=%d, txdokcnt=%d, txderrcnt=%d", writecnt,\
		pCanPortData[port-1]->CanStatic.dwApiTxdOkCnt,\
		pCanPortData[port-1]->CanStatic.dwApiTxdErrCnt);
#else
		ReportMsg("receive counter=%d, apirxdokcnt=%d, selffrm=%d, apirxderror=%d, ", readcnt, \
		pCanPortData[port-1]->CanStatic.dwApiRxdOkCnt,\
		dwSelfFramCnt, \
		pCanPortData[port-1]->CanStatic.dwApiRxdErrCnt);
#endif
	}
}

void CCanTest::OnReadComm(DWORD dwPort, DWORD dwFlag)
{
	DWORD len;

	readcnt++;
	
	len = CanRead(port, bRxdBuf, CAN_FRAME_BUF_LEN, 0);
	if(len)
		ReportMsg("receive frame len=%d", len);
	
#ifndef CAN_MASTER
	CanWrite(port, bAnswerFram, sizeof(bAnswerFram), address);
#endif	

}

void CCanTest::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	static DWORD idlecnt;

	idlecnt++;
}

#endif

