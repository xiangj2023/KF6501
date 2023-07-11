/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			mapbit.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           镜像位任务处理                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      镜像位处理，为实现快速交换，以太网、CAN网、串口的独立收发管理。                          
*                                                                                  
* 函数                                                                             
*                                                                                  
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      LZ               11/06/21    初始创建                         
*                                                                                  
********************************************************************************************/
#include "socklib.h"
#include "inetlib.h"
#include "iflib.h"
#include "parafunc.h"
#include "logic.h"
#include "mapbit.h"
#include "protocol.h"

extern BOOL RelayTaskInitOK;

BEGIN_MESSAGE_MAP(CMapBit,CApp)
	ON_SM_TIMEOUT(CMapBit)
	ON_SM_NETCARDPARACHG(CMapBit)
END_MESSAGE_MAP()

// 获取一帧
BOOL SearchOneMapFrame(PBYTE Buf,WORD wLen)
{
	TMBFrame		*pRxdHead=(TMBFrame *)Buf;
		
	//长度检查
	if(wLen < sizeof(TMBFrame))
		return	FM_LESS;
	
	//帧头检查
	if(pRxdHead->START!=0x68)
		return	FM_ERR;

	//数据检查
	if((pRxdHead->DATA1[0]^pRxdHead->DATA2[0]) != 0xFF)
		return	FM_ERR;
	if((pRxdHead->DATA1[1]^pRxdHead->DATA2[1]) != 0xFF)
		return	FM_ERR;
	
	//校验检查 			
	if(pRxdHead->CRC != LPC(0x55, Buf, sizeof(TMBFrame)-1))
		return	FM_ERR;
	
	return (FM_OK|sizeof(TMBFrame));
}

BOOL SearchMapFrame(TBufMsg* pMsgBuf,TBufMsg  *pFrameInfo)
{
	DWORD dwRtn;
	DWORD dwLen;//已处理过的字节数
	short nMsgLen;
	
	NeatenIoBuf(pMsgBuf);
 
	while(1)
	{
		nMsgLen = pMsgBuf->wWritePtr - pMsgBuf->wReadPtr;
		
		if(nMsgLen<=0)
		{
			return FALSE;
		}
		
		dwRtn = SearchOneMapFrame(&pMsgBuf->pBuf[pMsgBuf->wReadPtr], nMsgLen);
		dwLen = dwRtn & ~FM_SHIELD; //Len=已处理过的字节数

		switch(dwRtn& FM_SHIELD)
		{
		case FM_OK:
			pFrameInfo->pBuf = &pMsgBuf->pBuf[pMsgBuf->wReadPtr];  //记录有效报文的起始地址
			pFrameInfo->wWritePtr = (WORD)dwLen; //记录有效报文的长度

			pMsgBuf->wReadPtr += (WORD)dwLen; //指针移到下一报文处
			
			if(pMsgBuf->wReadPtr >= pMsgBuf->wWritePtr)
				pMsgBuf->wReadPtr = pMsgBuf->wWritePtr;
			
			return TRUE;
		
		case FM_ERR:
			if(!dwLen)
				pMsgBuf->wReadPtr++;
			else 
				pMsgBuf->wReadPtr += (WORD)dwLen; //指针移到下一报文处
			break;
		case FM_LESS:
			if(((pMsgBuf->wWritePtr-pMsgBuf->wReadPtr)>=pMsgBuf->wBufLen)&&(dwLen==0))
			  pMsgBuf->wReadPtr +=1;

			pMsgBuf->wReadPtr += (WORD)dwLen; 

			return FALSE;
		}
	}
}

void MapBitTask(DWORD *pdwApp)
{
	CMapBit *pMapBit = new CMapBit(pdwApp);
	if(pMapBit->Init() != TRUE)
	{
		::OS_Exit(0); 
	}
	
	pMapBit->Run();
}	

// 以太网管理类
BOOL CMBEth::Init(CMapBit *pMapClass,BYTE no,TMBO_Disp *pMBO)
{
	m_pMapBit = pMapClass;

	m_ObjNo = no;
	m_ip = pMBO->dwSideIP;
	m_SelfAddr = pMBO->bySelfAddr;
	m_SideAddr = pMBO->bySideAddr;
	
	m_SMBStart = pMBO->bySMBStart;
	m_SMBNum = pMBO->bySMBNum;
	m_RMBStart = pMBO->byRMBStart;
	m_RMBNum = pMBO->byRMBNum;

	// 每个镜像位对像独立管理
	m_RecvInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
	m_RecvInfo.wBufLen 	= 1024;
	m_RecvInfo.wReadPtr	= 0;
	m_RecvInfo.wWritePtr= 0;

	m_dwCommBreakCnt = 0;

	return TRUE;
}

static WORD wSendCnt=0,wRecvCnt=0;

BOOL CMBEth::SendData()
{
	TMBFrame	tFrame;
	BYTE		RWStatus[32];
//	TAbsTime 	time;
//	DWORD		lo;
	
//	ReadAbsTime(&time);

	tFrame.START = 0x68;
	tFrame.DesADDR = m_SideAddr;
	tFrame.SrcADDR = m_SelfAddr;

/*
	lo = (time.Lo % 100);
	tFrame.DATA1 = ((lo/10)<<4)+(lo%10);
	
	lo = ((time.Lo/100) % 100);
	tFrame.DATA2 = ((lo/10)<<4)+(lo%10);

	lo = ((time.Lo/10000) % 100);
	tFrame.DATA3 = ((lo/10)<<4)+(lo%10);

	lo = ((time.Lo/1000000) % 100);
	tFrame.DATA4 = ((lo/10)<<4)+(lo%10);
*/
 	// 取发送继电器字
	GetTxLinkValue(m_SMBStart, m_SMBNum, RWStatus);

	wSendCnt++;
	
	if((wSendCnt%100) == 0)
//		ReportMsg("CMBEth:SendData(%d - %d): %d %d %d %d %d %d %d %d",m_SMBStart,m_SMBStart+m_SMBNum,\
//					RWStatus[0],RWStatus[1],RWStatus[2],RWStatus[3],RWStatus[4],RWStatus[5],RWStatus[6],RWStatus[7]);
	
	// 填充继电器字状态
	memset(tFrame.DATA1,0x00,2);
	memset(tFrame.DATA2,0xFF,2);
	for(int i=0;i<m_SMBNum;i++)
	{
		if(RWStatus[i] == 0)
		{
			tFrame.DATA1[i/8] &= ~(0x01<<(i%8));
			tFrame.DATA2[i/8] |= (0x01<<(i%8));			
		}
		else
		{
			tFrame.DATA1[i/8] |= (0x01<<(i%8));			
			tFrame.DATA2[i/8] &= ~(0x01<<(i%8));
		}
	}
	
	tFrame.CRC = LPC(0x55,(BYTE *)&tFrame,sizeof(tFrame)-1);
			
	m_pMapBit->SendUDPData(m_ip,(BYTE *)&tFrame,sizeof(tFrame));

	return TRUE;
}

BOOL CMBEth::RecvData()
{
	TMBFrame	*pFrame;
	BYTE		RWStatus[32];
	BYTE		*pBuf;
	DWORD 		dwRtn;
	WORD 		wLen;
	
	NeatenIoBuf(&m_RecvInfo);//整理m_Rxd内存 （清理已处理数据）

	pBuf = &m_RecvInfo.pBuf[m_RecvInfo.wWritePtr];
	wLen = m_RecvInfo.wBufLen - m_RecvInfo.wWritePtr;

	dwRtn = m_pMapBit->RecvUDPData(m_ip,pBuf, wLen);

	if(dwRtn)
	{		
		m_RecvInfo.wWritePtr += (WORD)dwRtn;

		do
		{
			// 帧长不够长时返回
			if(!SearchMapFrame(&m_RecvInfo,&m_FrameInfo))
				break;
			
			if(m_dwCommBreakCnt >= MAPBIT_COMMBREAK_MAX)
			{
				ReportMsg("IP Address %d.%d.%d.%d comm OK!!!",HHBYTE(m_ip),HLBYTE(m_ip),LHBYTE(m_ip),LLBYTE(m_ip));
			}

			m_dwCommBreakCnt = 0;
			
			//处理帧
			pFrame = (TMBFrame	*)m_FrameInfo.pBuf;

			//接收继电器字
			for(int i=0;i<m_RMBNum;i++)
			{
				if(pFrame->DATA1[i/8] & (0x01<<(i%8)))
					RWStatus[i] = 1;
				else
					RWStatus[i] = 0;
			}

			//写入继电器字
			wRecvCnt++;
			
			if((wRecvCnt%100) == 0)
				ReportMsg("CMBEth:RecvData(%d - %d): %d %d %d %d %d %d %d %d",m_RMBStart,m_RMBStart+m_RMBNum,\
							RWStatus[0],RWStatus[1],RWStatus[2],RWStatus[3],RWStatus[4],RWStatus[5],RWStatus[6],RWStatus[7]);

			DAE_ClearRunningFlags(SS_MAPBIT_COMM_ERR+m_ObjNo);
			
			SetRxLinkValue(m_RMBStart,m_RMBNum,RWStatus);
		}while(1);
	}
	else
	{
		m_dwCommBreakCnt++;

		if(m_dwCommBreakCnt == MAPBIT_COMMBREAK_MAX)
		{
			DAE_SetRunningFlags(SS_MAPBIT_COMM_ERR+m_ObjNo);
			ReportMsg("IP Address %d.%d.%d.%d commbreak!!!",HHBYTE(m_ip),HLBYTE(m_ip),LHBYTE(m_ip),LLBYTE(m_ip));
		}
	}
	return TRUE;
}

// CAN网管理类
BOOL CMBCan::Init(BYTE no,TMBO_Disp	*pMBO)
{
	m_PortID = DB_GetMBOPortID(pMBO);
		
	m_ObjNo = no;
	//  打开CAN网
	m_SelfAddr = pMBO->bySelfAddr;
	m_SideAddr = pMBO->bySideAddr;
	m_SMBStart = pMBO->bySMBStart;
	m_SMBNum = pMBO->bySMBNum;
	m_RMBStart = pMBO->byRMBStart;
	m_RMBNum = pMBO->byRMBNum;
	return TRUE;
}

BOOL CMBCan::SendData()
{
	return TRUE;
}

BOOL CMBCan::RecvData(BYTE *pBuf)
{
	return TRUE;
}

// 串口管理类
BOOL CMBSer::Init(BYTE no,TMBO_Disp	*pMBO)
{
	m_PortID = DB_GetMBOPortID(pMBO);
	m_ObjNo = no;
	
	m_SelfAddr = pMBO->bySelfAddr;
	m_SideAddr = pMBO->bySideAddr;
	m_SMBStart = pMBO->bySMBStart;
	m_SMBNum = pMBO->bySMBNum;
	m_RMBStart = pMBO->byRMBStart;
	m_RMBNum = pMBO->byRMBNum;

	return OpenComm(m_PortID, 512, 512,0);
}

BOOL CMBSer::SendData()
{
	//发送帧
	TMBFrame	tFrame;
	BYTE		RWStatus[32];
	
	tFrame.START = 0x68;
	tFrame.DesADDR = m_SideAddr;
	tFrame.SrcADDR = m_SelfAddr;

 	// 取发送继电器字
	GetTxLinkValue(m_SMBStart, m_SMBNum, RWStatus);

	// 填充继电器字状态
	memset(tFrame.DATA1,0x00,2);
	memset(tFrame.DATA2,0xFF,2);
	for(int i=0;i<m_SMBNum;i++)
	{
		if(RWStatus[i] == 0)
		{
			tFrame.DATA1[i/8] &= ~(0x01<<(i%8));
			tFrame.DATA2[i/8] |= (0x01<<(i%8));			
		}
		else
		{
			tFrame.DATA1[i/8] |= (0x01<<(i%8));			
			tFrame.DATA2[i/8] &= ~(0x01<<(i%8));
		}
	}

	tFrame.CRC = LPC(0x55,(BYTE *)&tFrame,sizeof(tFrame)-1);
	
	::WriteComm(m_PortID,(BYTE *)&tFrame,sizeof(tFrame),0);

	return TRUE;
}

BOOL CMBSer::RecvData(BYTE *pBuf)
{
	TMBFrame	*pFrame;
	BYTE		RWStatus[32];
	
	//处理帧
	pFrame = (TMBFrame	*)pBuf;
	
	//接收继电器字
	for(int i=0;i<m_RMBNum;i++)
	{
		if(pFrame->DATA1[i/8] & (0x01<<(i%8)))
			RWStatus[i] = 1;
		else
			RWStatus[i] = 0;
	}
	
	//写入继电器字
	SetRxLinkValue(m_RMBStart,m_RMBNum,RWStatus);
	
	return TRUE;
}

CMapBit::CMapBit(DWORD *pdwApp):CApp(pdwApp)
{
	m_dwTickCnt = 0;
	m_EthNum = 0;
	m_CanNum = 0;
	m_SerNum = 0;
}	

BOOL CMapBit::Init(void)
{
	WORD		num;
	TMBO_Disp	tMBO;

	while(!RelayTaskInitOK)
	{
		Sleep(100);
	}

	num = DB_GetMapBitObjNum();

	if(num == 0)
		return FALSE;
	
	// 逐个初始化镜像位端口
	for(int i=0;i<num;i++)
	{
		if(DB_GetMapBitObjDisp(i, &tMBO) != DB_ERR_OK)
			continue;

		if(!(tMBO.wControl&0x8000))
			continue;
			
		switch(tMBO.wControl&0x0F)
		{
			// 初始化以太网
			case MAPBIT_PORTTYPE_ETHA:
			case MAPBIT_PORTTYPE_ETHB:
				m_EthInfo[m_EthNum++].Init(this,i,&tMBO);
				break;

			// 初始化CAN网
			case MAPBIT_PORTTYPE_CANA:	
			case MAPBIT_PORTTYPE_CANB:
				m_CanInfo[m_CanNum++].Init(i,&tMBO);
				break;
					
			// 初始化串口
			case MAPBIT_PORTTYPE_SERIAL1:
			case MAPBIT_PORTTYPE_SERIAL2:
			case MAPBIT_PORTTYPE_SERIAL3:	
				m_SerInfo[m_SerNum++].Init(i,&tMBO);
				break;

			default:
				DAE_SetRunningFlags(SS_MAPBIT_PARA_ERR);
				return FALSE;
		}
	}

	// 如果有以太网镜像位对象
	if(m_EthNum)
	{
		m_hSocket = -1;
		m_dwError = 0;
		m_dwWriteBlockNum = 0;	
		CreateSocket();
	}

	// 如果有CAN网镜像位对象
	if(m_CanNum)
	{
		m_CanRxdInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
		m_CanRxdInfo.wBufLen = 1024;
		m_CanRxdInfo.wReadPtr = 0;
		m_CanRxdInfo.wWritePtr= 0;
	}

	// 如果有串口镜像位对象
	if(m_SerNum)
	{
		m_SerRxdInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
		m_SerRxdInfo.wBufLen = 1024;
		m_SerRxdInfo.wReadPtr = 0;
		m_SerRxdInfo.wWritePtr= 0;
	}

	// 20毫秒节拍
	SetTimer(1,MAPBIT_TIME);
	
	return TRUE;
}

BOOL CMapBit::CreateSocket()
{
	struct sockaddr_in selfAddr;
	struct linger Linger;
	int hSocket;
	int nVal;

	if(m_hSocket != -1)
		return TRUE;

	hSocket = -1;
			
	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(hSocket == -1)
	{
		LogError("CMapBit::CreateSocket",FILE_LINE,"create socket error.errno=%d",errno);
		return FALSE;
	}

	nVal = 2*1024;
	setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nVal, sizeof(int));
	setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nVal, sizeof(int));

	nVal = TRUE; 
	if((int)setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nVal, sizeof(int)) == -1)
	{
		LogError("CMapBit::CreateSocket",FILE_LINE,"setsockopt error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}

	nVal = 1;//m_bBockMode;
	if(-1 == ioctl(hSocket,FIONBIO ,(int)&nVal) )
	{
		//ioctlsocket==ioctl,psos对socket设置操作方式，FIONBIO：设置阻塞方式，nbio=0:block,1:noBlock
		close(hSocket);
		return FALSE;
	}

	Linger.l_onoff = 1;
	Linger.l_linger = 0;
	setsockopt(hSocket, SOL_SOCKET, SO_LINGER, \
		(char *)&Linger, sizeof(Linger));
	
//	if(setsockopt(hSocket, SOL_SOCKET, SO_LINGER, \
//		(char *)&Linger, sizeof(Linger)) < 0)
//	{
//		close(hSocket);
//		return FALSE;
//	}

	
	//bind local address
	memset((char *)&selfAddr, 0, sizeof(struct sockaddr_in));
	selfAddr.sin_family = AF_INET;
	selfAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(m_netInfo.dwSelfIP);
	selfAddr.sin_port = 6500;
	 
	if(-1 == bind(hSocket, (sockaddr*)&selfAddr, sizeof(sockaddr)))
	{
		LogError("CMapBit::CreateSocket",FILE_LINE,"bind error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}
	
	nVal = 1;;
	if(-1 == (int)setsockopt(hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&nVal,sizeof(int)))
	{
		LogError("CMapBit::CreateSocket",FILE_LINE,"bind error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}
		
	if(hSocket == -1)
		return FALSE;

	m_hSocket = hSocket;
	
	return TRUE;
}

BOOL CMapBit::CloseSocket()
{
	if(m_hSocket == -1)
		return FALSE;
	
	close(m_hSocket);
	m_hSocket = -1;
	return TRUE;
}

WORD CMapBit::SendUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen)
{
	long 	lgRtn;
	int 	nAddrLen;
	struct sockaddr_in toAddr;

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6500);
	toAddr.sin_addr.s_addr = htonl(ipAddr);

	nAddrLen = sizeof(struct sockaddr_in);

	if(m_hSocket == -1)
		return FALSE;
	
	if(wLen == 0)
		return FALSE;
 		
	lgRtn = sendto(m_hSocket, (char*)pBuf, wLen, 0, (sockaddr*)&toAddr, nAddrLen);

	if(lgRtn > 0)
	{
		m_dwError = 0;
	}

	if(lgRtn <= 0)////=0连接断开
	{
		//目的主机没开		
		if(errno == EHOSTDOWN)
			return FALSE;
			
		// 阻塞错误
		if(errno == EWOULDBLOCK)
		{
			m_dwWriteBlockNum ++;
			if(m_dwWriteBlockNum > 100)
			{
				m_dwWriteBlockNum = 0;
				CloseSocket();
			}
			return FALSE;
		}
		
		// 其他错误
		m_dwWriteBlockNum = 0;
		
		m_dwError ++;				
		if(m_dwError > 20)
		{
			m_dwError = 0;
			CloseSocket();
		}
		
		return FALSE;
	}

	return TRUE;
}

WORD CMapBit::RecvUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen)
{
	long nReadNum=0;
	int nAddrLen;
	struct sockaddr_in toAddr;

	if(wLen == 0)
		return 0;
	
	if(wLen > 1023)
		wLen = 1023;
	
	if(m_hSocket == -1)
		return 0;
				
	memset((void *)&toAddr,0 ,sizeof(struct sockaddr_in));
	nAddrLen = sizeof(struct sockaddr_in);
		
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6500);
	toAddr.sin_addr.s_addr = htonl(ipAddr);
	
	nReadNum = recvfrom(m_hSocket, (char*)pBuf, wLen, 0, (sockaddr*)&toAddr, &nAddrLen);

	// =0 时表示已断开
	if(nReadNum <= 0)
	{
		if(errno == EHOSTDOWN)
			return 0;
			
		if(errno == EWOULDBLOCK)
			return 0;
		
		m_dwError ++;
		if(m_dwError > 20)
		{
			m_dwError = 0;
			CloseSocket();
		}
	}
	
	if(nReadNum > 0)
	{
		m_dwError = 0;
//		DWORD dwRxdIP = ntohl(toAddr.sin_addr.s_addr);
//		ReportMsg("CMapBit::RecvUDPData,From %d.%d.%d.%d Len=%d",HHBYTE(dwRxdIP),HLBYTE(dwRxdIP),\
//			LHBYTE(dwRxdIP),LLBYTE(dwRxdIP),nReadNum);
		return nReadNum;
	}

	return 0;
}

void CMapBit::OnTimeOut(DWORD id)
{
	m_dwTickCnt++;

	// 1秒钟检查一次通信连接
	if((m_dwTickCnt%50)==0)
	{
		if((m_EthNum != 0)&&(m_hSocket == -1))
			CreateSocket();
	}
	
	// 1.处理以太网对象
	for(int i=0;i<m_EthNum;i++)
	{
		m_EthInfo[i].SendData();
		m_EthInfo[i].RecvData();
	}
	
	// 2.处理CAN网对象
	for(int i=0;i<m_CanNum;i++)
	{
		// 暂时只处理一个端口
		if(i == 0)
			CanRecvProcess(m_CanInfo->GetPortID());
		
		m_CanInfo[i].SendData();
	}
	
	// 2.处理串口对象
	for(int i=0;i<m_SerNum;i++)
	{
		// 暂时只处理一个端口
		if(i == 0)
			SerRecvProcess(m_SerInfo->GetPortID());

		m_SerInfo[i].SendData();
	}	
}

void CMapBit::OnRelayRTMB(DWORD num)
{
}

void CMapBit::OnReadComm(DWORD dwPort, DWORD dwFlag)
{
	for(int i=0;i<m_CanNum;i++)
		// 在CAN网对象查找
		if(dwPort == m_CanInfo[i].GetPortID())
		{
			CanRecvProcess(dwPort);
			return;
		}
		
		// 在串口查找
	for(int i=0;i<m_SerNum;i++)
		if(dwPort == m_SerInfo[i].GetPortID())
		{			
			SerRecvProcess(dwPort);
			return;
		}

	ReportMsg("MAPBIT: invaid recv port 0x%x %d@@@@@@@@@@@@@@@@",dwPort);
}

void CMapBit::OnNetCardParaChange()
{
	// 关闭SOCKET
	CloseSocket();

	// 等待5秒钟，网卡初始化
	Sleep(5000);
}

//串口接收响应
void CMapBit::SerRecvProcess(WORD wPortID)
{
	TMBFrame	*pFrame;
	BYTE		*pBuf;
	DWORD 		dwRtn;
	WORD 		wLen;
	
	NeatenIoBuf(&m_SerRxdInfo);//整理m_Rxd内存 （清理已处理数据）

	pBuf = &m_SerRxdInfo.pBuf[m_SerRxdInfo.wWritePtr];
	wLen = m_SerRxdInfo.wBufLen - m_SerRxdInfo.wWritePtr;

	dwRtn = ::ReadComm(wPortID, pBuf, wLen, 0);

	if(dwRtn)
	{		
		m_SerRxdInfo.wWritePtr += (WORD)dwRtn;

		do
		{
			// 帧长不够长时返回错误
			if(!SearchMapFrame(&m_SerRxdInfo,&m_SerFrameInfo))
				break;

			// 取出有效帧
			pFrame = (TMBFrame	*)m_SerFrameInfo.pBuf;
				
			for(int i=0;i<m_SerNum;i++)
			{
				// 地址检查
				if(pFrame->DesADDR != m_SerInfo[i].GetSelfAddress())
						continue;
				
				if(pFrame->SrcADDR != m_SerInfo[i].GetSideAddress())
						continue;
				
				m_SerInfo[i].RecvData(m_SerFrameInfo.pBuf);
			}
		}while(1);
	}
}

//CAN网接收响应
void CMapBit::CanRecvProcess(WORD wPortID)
{
	TMBFrame	*pFrame;
	BYTE* pBuf;
	DWORD dwRtn;
	WORD wLen;
	
	NeatenIoBuf(&m_CanRxdInfo);//整理m_Rxd内存 （清理已处理数据）

	pBuf = &m_CanRxdInfo.pBuf[m_CanRxdInfo.wWritePtr];
	wLen = m_CanRxdInfo.wBufLen - m_CanRxdInfo.wWritePtr;

	dwRtn = ::ReadComm(wPortID, pBuf, wLen, 0);

	if(dwRtn)
	{		
		m_CanRxdInfo.wWritePtr += (WORD)dwRtn;

		do
		{
			// 帧长不够长时返回
			if(!SearchMapFrame(&m_CanRxdInfo,&m_CanFrameInfo))
				break;

			// 取出有效帧
			pFrame = (TMBFrame	*)m_CanFrameInfo.pBuf;
				
			for(int i=0;i<m_CanNum;i++)
			{
				// 地址检查
				if(pFrame->DesADDR != m_CanInfo[i].GetSelfAddress())
						continue;
				
				if(pFrame->SrcADDR != m_CanInfo[i].GetSideAddress())
						continue;

				m_CanInfo[i].RecvData(m_CanFrameInfo.pBuf);
			}
		}while(1);
	}
}

