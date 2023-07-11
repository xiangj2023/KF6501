/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			mapbit.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ����λ������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ����λ����Ϊʵ�ֿ��ٽ�������̫����CAN�������ڵĶ����շ�����                          
*                                                                                  
* ����                                                                             
*                                                                                  
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      LZ               11/06/21    ��ʼ����                         
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

// ��ȡһ֡
BOOL SearchOneMapFrame(PBYTE Buf,WORD wLen)
{
	TMBFrame		*pRxdHead=(TMBFrame *)Buf;
		
	//���ȼ��
	if(wLen < sizeof(TMBFrame))
		return	FM_LESS;
	
	//֡ͷ���
	if(pRxdHead->START!=0x68)
		return	FM_ERR;

	//���ݼ��
	if((pRxdHead->DATA1[0]^pRxdHead->DATA2[0]) != 0xFF)
		return	FM_ERR;
	if((pRxdHead->DATA1[1]^pRxdHead->DATA2[1]) != 0xFF)
		return	FM_ERR;
	
	//У���� 			
	if(pRxdHead->CRC != LPC(0x55, Buf, sizeof(TMBFrame)-1))
		return	FM_ERR;
	
	return (FM_OK|sizeof(TMBFrame));
}

BOOL SearchMapFrame(TBufMsg* pMsgBuf,TBufMsg  *pFrameInfo)
{
	DWORD dwRtn;
	DWORD dwLen;//�Ѵ�������ֽ���
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
		dwLen = dwRtn & ~FM_SHIELD; //Len=�Ѵ�������ֽ���

		switch(dwRtn& FM_SHIELD)
		{
		case FM_OK:
			pFrameInfo->pBuf = &pMsgBuf->pBuf[pMsgBuf->wReadPtr];  //��¼��Ч���ĵ���ʼ��ַ
			pFrameInfo->wWritePtr = (WORD)dwLen; //��¼��Ч���ĵĳ���

			pMsgBuf->wReadPtr += (WORD)dwLen; //ָ���Ƶ���һ���Ĵ�
			
			if(pMsgBuf->wReadPtr >= pMsgBuf->wWritePtr)
				pMsgBuf->wReadPtr = pMsgBuf->wWritePtr;
			
			return TRUE;
		
		case FM_ERR:
			if(!dwLen)
				pMsgBuf->wReadPtr++;
			else 
				pMsgBuf->wReadPtr += (WORD)dwLen; //ָ���Ƶ���һ���Ĵ�
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

// ��̫��������
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

	// ÿ������λ�����������
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
 	// ȡ���ͼ̵�����
	GetTxLinkValue(m_SMBStart, m_SMBNum, RWStatus);

	wSendCnt++;
	
	if((wSendCnt%100) == 0)
//		ReportMsg("CMBEth:SendData(%d - %d): %d %d %d %d %d %d %d %d",m_SMBStart,m_SMBStart+m_SMBNum,\
//					RWStatus[0],RWStatus[1],RWStatus[2],RWStatus[3],RWStatus[4],RWStatus[5],RWStatus[6],RWStatus[7]);
	
	// ���̵�����״̬
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
	
	NeatenIoBuf(&m_RecvInfo);//����m_Rxd�ڴ� �������Ѵ������ݣ�

	pBuf = &m_RecvInfo.pBuf[m_RecvInfo.wWritePtr];
	wLen = m_RecvInfo.wBufLen - m_RecvInfo.wWritePtr;

	dwRtn = m_pMapBit->RecvUDPData(m_ip,pBuf, wLen);

	if(dwRtn)
	{		
		m_RecvInfo.wWritePtr += (WORD)dwRtn;

		do
		{
			// ֡��������ʱ����
			if(!SearchMapFrame(&m_RecvInfo,&m_FrameInfo))
				break;
			
			if(m_dwCommBreakCnt >= MAPBIT_COMMBREAK_MAX)
			{
				ReportMsg("IP Address %d.%d.%d.%d comm OK!!!",HHBYTE(m_ip),HLBYTE(m_ip),LHBYTE(m_ip),LLBYTE(m_ip));
			}

			m_dwCommBreakCnt = 0;
			
			//����֡
			pFrame = (TMBFrame	*)m_FrameInfo.pBuf;

			//���ռ̵�����
			for(int i=0;i<m_RMBNum;i++)
			{
				if(pFrame->DATA1[i/8] & (0x01<<(i%8)))
					RWStatus[i] = 1;
				else
					RWStatus[i] = 0;
			}

			//д��̵�����
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

// CAN��������
BOOL CMBCan::Init(BYTE no,TMBO_Disp	*pMBO)
{
	m_PortID = DB_GetMBOPortID(pMBO);
		
	m_ObjNo = no;
	//  ��CAN��
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

// ���ڹ�����
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
	//����֡
	TMBFrame	tFrame;
	BYTE		RWStatus[32];
	
	tFrame.START = 0x68;
	tFrame.DesADDR = m_SideAddr;
	tFrame.SrcADDR = m_SelfAddr;

 	// ȡ���ͼ̵�����
	GetTxLinkValue(m_SMBStart, m_SMBNum, RWStatus);

	// ���̵�����״̬
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
	
	//����֡
	pFrame = (TMBFrame	*)pBuf;
	
	//���ռ̵�����
	for(int i=0;i<m_RMBNum;i++)
	{
		if(pFrame->DATA1[i/8] & (0x01<<(i%8)))
			RWStatus[i] = 1;
		else
			RWStatus[i] = 0;
	}
	
	//д��̵�����
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
	
	// �����ʼ������λ�˿�
	for(int i=0;i<num;i++)
	{
		if(DB_GetMapBitObjDisp(i, &tMBO) != DB_ERR_OK)
			continue;

		if(!(tMBO.wControl&0x8000))
			continue;
			
		switch(tMBO.wControl&0x0F)
		{
			// ��ʼ����̫��
			case MAPBIT_PORTTYPE_ETHA:
			case MAPBIT_PORTTYPE_ETHB:
				m_EthInfo[m_EthNum++].Init(this,i,&tMBO);
				break;

			// ��ʼ��CAN��
			case MAPBIT_PORTTYPE_CANA:	
			case MAPBIT_PORTTYPE_CANB:
				m_CanInfo[m_CanNum++].Init(i,&tMBO);
				break;
					
			// ��ʼ������
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

	// �������̫������λ����
	if(m_EthNum)
	{
		m_hSocket = -1;
		m_dwError = 0;
		m_dwWriteBlockNum = 0;	
		CreateSocket();
	}

	// �����CAN������λ����
	if(m_CanNum)
	{
		m_CanRxdInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
		m_CanRxdInfo.wBufLen = 1024;
		m_CanRxdInfo.wReadPtr = 0;
		m_CanRxdInfo.wWritePtr= 0;
	}

	// ����д��ھ���λ����
	if(m_SerNum)
	{
		m_SerRxdInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
		m_SerRxdInfo.wBufLen = 1024;
		m_SerRxdInfo.wReadPtr = 0;
		m_SerRxdInfo.wWritePtr= 0;
	}

	// 20�������
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
		//ioctlsocket==ioctl,psos��socket���ò�����ʽ��FIONBIO������������ʽ��nbio=0:block,1:noBlock
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

	if(lgRtn <= 0)////=0���ӶϿ�
	{
		//Ŀ������û��		
		if(errno == EHOSTDOWN)
			return FALSE;
			
		// ��������
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
		
		// ��������
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

	// =0 ʱ��ʾ�ѶϿ�
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

	// 1���Ӽ��һ��ͨ������
	if((m_dwTickCnt%50)==0)
	{
		if((m_EthNum != 0)&&(m_hSocket == -1))
			CreateSocket();
	}
	
	// 1.������̫������
	for(int i=0;i<m_EthNum;i++)
	{
		m_EthInfo[i].SendData();
		m_EthInfo[i].RecvData();
	}
	
	// 2.����CAN������
	for(int i=0;i<m_CanNum;i++)
	{
		// ��ʱֻ����һ���˿�
		if(i == 0)
			CanRecvProcess(m_CanInfo->GetPortID());
		
		m_CanInfo[i].SendData();
	}
	
	// 2.�����ڶ���
	for(int i=0;i<m_SerNum;i++)
	{
		// ��ʱֻ����һ���˿�
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
		// ��CAN���������
		if(dwPort == m_CanInfo[i].GetPortID())
		{
			CanRecvProcess(dwPort);
			return;
		}
		
		// �ڴ��ڲ���
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
	// �ر�SOCKET
	CloseSocket();

	// �ȴ�5���ӣ�������ʼ��
	Sleep(5000);
}

//���ڽ�����Ӧ
void CMapBit::SerRecvProcess(WORD wPortID)
{
	TMBFrame	*pFrame;
	BYTE		*pBuf;
	DWORD 		dwRtn;
	WORD 		wLen;
	
	NeatenIoBuf(&m_SerRxdInfo);//����m_Rxd�ڴ� �������Ѵ������ݣ�

	pBuf = &m_SerRxdInfo.pBuf[m_SerRxdInfo.wWritePtr];
	wLen = m_SerRxdInfo.wBufLen - m_SerRxdInfo.wWritePtr;

	dwRtn = ::ReadComm(wPortID, pBuf, wLen, 0);

	if(dwRtn)
	{		
		m_SerRxdInfo.wWritePtr += (WORD)dwRtn;

		do
		{
			// ֡��������ʱ���ش���
			if(!SearchMapFrame(&m_SerRxdInfo,&m_SerFrameInfo))
				break;

			// ȡ����Ч֡
			pFrame = (TMBFrame	*)m_SerFrameInfo.pBuf;
				
			for(int i=0;i<m_SerNum;i++)
			{
				// ��ַ���
				if(pFrame->DesADDR != m_SerInfo[i].GetSelfAddress())
						continue;
				
				if(pFrame->SrcADDR != m_SerInfo[i].GetSideAddress())
						continue;
				
				m_SerInfo[i].RecvData(m_SerFrameInfo.pBuf);
			}
		}while(1);
	}
}

//CAN��������Ӧ
void CMapBit::CanRecvProcess(WORD wPortID)
{
	TMBFrame	*pFrame;
	BYTE* pBuf;
	DWORD dwRtn;
	WORD wLen;
	
	NeatenIoBuf(&m_CanRxdInfo);//����m_Rxd�ڴ� �������Ѵ������ݣ�

	pBuf = &m_CanRxdInfo.pBuf[m_CanRxdInfo.wWritePtr];
	wLen = m_CanRxdInfo.wBufLen - m_CanRxdInfo.wWritePtr;

	dwRtn = ::ReadComm(wPortID, pBuf, wLen, 0);

	if(dwRtn)
	{		
		m_CanRxdInfo.wWritePtr += (WORD)dwRtn;

		do
		{
			// ֡��������ʱ����
			if(!SearchMapFrame(&m_CanRxdInfo,&m_CanFrameInfo))
				break;

			// ȡ����Ч֡
			pFrame = (TMBFrame	*)m_CanFrameInfo.pBuf;
				
			for(int i=0;i<m_CanNum;i++)
			{
				// ��ַ���
				if(pFrame->DesADDR != m_CanInfo[i].GetSelfAddress())
						continue;
				
				if(pFrame->SrcADDR != m_CanInfo[i].GetSideAddress())
						continue;

				m_CanInfo[i].RecvData(m_CanFrameInfo.pBuf);
			}
		}while(1);
	}
}

