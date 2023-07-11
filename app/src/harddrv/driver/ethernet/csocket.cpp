#include "netentry.h"
#include "csocket.h"
#include "selectlib.h"

#if (INSTALL_NET == YES)
 
extern DWORD m_dwNetTaskID;

CSocket::CSocket()
{
	m_wRxdWritePtr = 0;
	m_wRxdReadPtr = 0;
	m_wTxdWritePtr = 0;
	m_wTxdReadPtr = 0;
	m_dwNoRunCount = 0;
	m_dwTestLinkCount = 0;
	m_dwWriteBlockNum = 0;
	m_bFirstSamePort = TRUE;
	m_dwTagetID = 0;

	memset(&m_netInfo, 0, sizeof(m_netInfo));
	memset(&m_CommCtrl, 0, sizeof(m_CommCtrl));

	m_bConnected = FALSE;
	m_bOpend = TRUE;
	m_bBockMode = 0;
	m_hSocket = INVALID_SOCKET;
	m_hASocket = INVALID_SOCKET;

	m_TxdBuf = NULL;
	m_RxdBuf = NULL;
	m_wMaxTxdLen = TXDBUFLEN;
	m_wMaxRxdLen = RXDBUFLEN;

	m_TxdBuf = new BYTE[m_wMaxTxdLen];
	m_RxdBuf = new BYTE[m_wMaxRxdLen];

	m_bWriteContinue = FALSE;
	m_bNotify = FALSE;
	m_dwNoReadCount = 0;
	m_dwError = 0;
	
	bCloseLink = FALSE;
}

CSocket::~CSocket()
{
}

void CSocket::Create()
{
}

void CSocket::MainTask()
{
	if(!m_bOpend)
		return;
	
	if(!CanSendData())
		return;

	if(bCloseLink)
	{
		bCloseLink = FALSE;
		CloseLink();
		return;
	}
	
	if(IsTcpServerMode())
		TcpServerLink();
	else
	{
		if(!m_bConnected)
		{
			m_dwError = 0;
			if(m_dwNoRunCount == 0)
				CreateLink();
		
			m_dwNoRunCount ++;

			if(m_dwNoRunCount >= NORUNTIME)
				m_dwNoRunCount = 0;
		}
	}

	if(!m_bConnected)
		return;

	m_dwNoRunCount = 0;
	
	WriteTask();
	ReadTask();

	if(!CanSendData())
		return;

	NotifyTask();//////监测读、写空闲时间 
}

void CSocket::ReadTask()
{
	if(m_bTcpMode == NET_TCP_MODE)
		TcpNetRead();
	if(m_bTcpMode == NET_UDP_MODE)
		UdpNetRead();
}

void CSocket::WriteTask()
{
	NetWrite();
}

void CSocket::TestLinkTask()
{
	AddTestLinkTime();

	if(!IsTestLinkTime())
		return;
	if(TestNetLink())
		return;

	CloseLink();
}

void CSocket::NotifyTask()
{
	BOOL bTxdBreak;
	BOOL bCommBreak;
	BOOL bWriteNotify;
	BOOL bNotify;
	DWORD dwTime;
	
	if(m_bWriteContinue && GetCanWriteNum())
	{
		m_bWriteContinue = FALSE;
		NotifyToUse(SM_WRITECONT, 0, 0);
	}

	SetReadMask();/////读通道空闲时间++
	SetWriteMask();////写通道空闲时间++

	bTxdBreak = (m_CommCtrl.dwNotifyTxdBreak > 0);	
	dwTime = notifyCount.dwNotifyTxdBreak * NET_TASK_TIMER;
	bTxdBreak &= (dwTime >= m_CommCtrl.dwNotifyTxdBreak);

	bCommBreak = (m_CommCtrl.dwNotifyCommBreak > 0);
	dwTime = notifyCount.dwNotifyCommBreak * NET_TASK_TIMER/2;
	bCommBreak &= (dwTime >= m_CommCtrl.dwNotifyCommBreak);

	bWriteNotify = (m_CommCtrl.dwNotifyWrite > 0);
	bWriteNotify &= (GetCanSendNum() <= m_CommCtrl.dwNotifyWrite);
	
	bNotify = (bTxdBreak | bCommBreak | bWriteNotify);
	
	if(!bNotify)
		return;

	if(m_bNotify)
	{
		if(bCommBreak)
		{
			NotifyToUse(SM_COMMIDLE, 0, 0);
			ClearReadMask();
			ClearWriteMask();
		}
		else if(bTxdBreak)
		{
			NotifyToUse(SM_COMMTXDIDLE, 0, 0);
			ClearWriteMask();
		}
		else if(bWriteNotify)
		{
			NotifyToUse(SM_WRITECOMM, 0, 0);
		}
	}
	else
	{
//		m_dwNoReadCount++;

		ClearReadMask();
		ClearWriteMask();

//		if(m_dwNoReadCount >= NOREADCOUNT)
		{
			m_dwNoReadCount = 0;
			m_bNotify = TRUE;
		}
	}
}

int CSocket::ReadData(BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	WORD wLeft;
	
	if(!CanSendData())
		return 0;

	m_bNotify = TRUE;

	ClearReadMask();
	m_dwNoReadCount = 0;

	wLeft = GetLeftRxdNum();

	wLen = (wLeft>wLen) ? wLen : wLeft;

	if(wLen == 0 || pBuf == NULL)
		return 0;

	for(int i = 0; i< wLen; i++)
	{
		pBuf[i] = m_RxdBuf[m_wRxdReadPtr];
		m_wRxdReadPtr +=1;
		m_wRxdReadPtr %= m_wMaxRxdLen;
	}

	return wLen;
}

int CSocket::WriteData(BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	WORD wLeft;

	if(!CanSendData())
		return 0;
	
	m_bWriteContinue = FALSE;
	m_dwNoReadCount = 0;
	m_bNotify = TRUE;
	ClearWriteMask();
	
	wLeft = GetCanWriteNum();

	if(wLen > wLeft)
	{
		wLen = wLeft;
		m_bWriteContinue = TRUE;
	}

	if(wLen == 0 || pBuf == NULL)
		return 0;

	for(int i = 0; i< wLen; i++)
	{
		m_TxdBuf[m_wTxdWritePtr] = pBuf[i];
		m_wTxdWritePtr +=1;
		m_wTxdWritePtr %= m_wMaxTxdLen;
	}

	return wLen;
}

void CSocket::WriteRxdBuf(BYTE* pBuf, WORD wNum)
{
	for(int i = 0; i < wNum; i++)
	{
		m_RxdBuf[m_wRxdWritePtr] = pBuf[i];
		m_wRxdWritePtr ++;
		m_wRxdWritePtr %= m_wMaxRxdLen;
	}
}

void CSocket::WriteTxdBuf(BYTE* pBuf, WORD wNum)
{
	for(int i = 0; i < wNum; i++)
	{
		m_TxdBuf[m_wTxdWritePtr] = pBuf[i];
		m_wTxdWritePtr ++;
		m_wTxdWritePtr %= m_wMaxTxdLen;
	}
}

WORD CSocket::GetLeftRxdNum()
{
	WORD wLen;
	WORD wRxdWritePtr;
	WORD wRxdReadPtr;

	wRxdWritePtr = m_wRxdWritePtr;/////资源保护,防止在进行下列操作时，指针变化
	wRxdReadPtr = m_wRxdReadPtr;///////

	if(wRxdWritePtr >= wRxdReadPtr)
		wLen = wRxdWritePtr - wRxdReadPtr;
	else
		wLen = m_wMaxRxdLen - (wRxdReadPtr-wRxdWritePtr);

	return wLen;
}

WORD CSocket::GetCanWriteNum()
{
	WORD wLen;
	WORD wTxdWritePtr;
	WORD wTxdReadPtr;

	wTxdWritePtr = m_wTxdWritePtr;
	wTxdReadPtr = m_wTxdReadPtr;

	if(wTxdWritePtr > wTxdReadPtr)
		wLen = m_wMaxTxdLen - (wTxdWritePtr - wTxdReadPtr);
	else if(wTxdWritePtr < wTxdReadPtr)
		wLen =  wTxdReadPtr - wTxdWritePtr;
	else
		wLen = m_wMaxTxdLen;

	return wLen-1;
}

WORD CSocket::GetCanSendNum()
{
	WORD wLen;
	WORD wTxdWritePtr;
	WORD wTxdReadPtr;
	
	wTxdWritePtr = m_wTxdWritePtr;
	wTxdReadPtr = m_wTxdReadPtr;

	if(wTxdWritePtr >= wTxdReadPtr)
		wLen =  wTxdWritePtr - wTxdReadPtr;
	else
		wLen =  m_wMaxTxdLen - (wTxdReadPtr - wTxdWritePtr);

	return wLen;
}

DWORD CSocket::NotifyToUse(WORD wMsg, WORD wLen, DWORD dwFlag)
{
	if(m_dwTagetID == 0)
		return 0;

	m_dwNoReadCount = 0;
	m_bNotify = FALSE;
	
	wLen = 0;
	dwFlag = 0;

	OS_PostMessage(m_dwTagetID, wMsg, wLen, m_dwPort, dwFlag,0);

	return 1;
}

DWORD CSocket::OpenRxdTxdBuf(WORD wRxdLen, WORD wTxdLen)
{
	DWORD dwVarName;

	dwVarName = m_dwPort&0xFFFF0000;
	dwVarName += ((m_dwPort&0xFF)<<8);

#ifdef GETBUFLENFROMCTL
	if(wRxdLen == 0 || wRxdLen > RXDBUFLEN)
		return 1;
	if(wTxdLen == 0 || wTxdLen > TXDBUFLEN)
		return 1;

	if(m_RxdBuf)
		delete m_RxdBuf;
	if(m_TxdBuf)
		delete m_TxdBuf;

	m_wMaxRxdLen = wRxdLen;
	m_wMaxTxdLen = wTxdLen;

	m_RxdBuf = new BYTE[m_wMaxRxdLen];
	m_TxdBuf = new BYTE[m_wMaxTxdLen];

	if(m_RxdBuf == NULL || m_TxdBuf == NULL)
		return 1;
#endif
	
	m_bOpend = TRUE;

	return ERR_SUCCESS;
}

void CSocket::SetNetInfo(TEthPortPara *pInfo)
{
	if(pInfo == NULL)
		return;
	m_netInfo = *pInfo;
}

DWORD CSocket::EthernetControl(char* pBuf)
{
	TCtlComm *TempCtlComm;
	
	TempCtlComm = (TCtlComm *)pBuf;
	if(TempCtlComm == NULL)
		return 1;
	
	if(TempCtlComm->dwFuncCode == FC_GET)
	{
		*TempCtlComm = m_CommCtrl;
		return ERR_SUCCESS;
	}
	if(TempCtlComm->dwFuncCode != FC_SET)
		return 1;

	m_CommCtrl = *(TCtlComm*)pBuf;
	if(m_CommCtrl.dwNotifyWrite > m_wMaxTxdLen)
		m_CommCtrl.dwNotifyWrite = m_wMaxTxdLen;

	if(m_CommCtrl.dwNotifyRead > m_wMaxRxdLen)
		m_CommCtrl.dwNotifyRead = m_wMaxRxdLen/2;

/*
	if(m_CommCtrl.dwCtlMask &CM_NOTIFYWRITE)
	{
		if(m_CommCtrl.dwNotifyWrite > m_wMaxTxdLen)
			m_CommCtrl.dwNotifyWrite = m_wMaxTxdLen;
	}
	else
		m_CommCtrl.dwNotifyWrite = 0;

	if(m_CommCtrl.dwCtlMask & CM_NOTIFYRXDBREAK)
		m_CommCtrl.dwNotifyRxdBreak /= NET_TASK_TIMER;
	else
		m_CommCtrl.dwNotifyRxdBreak = 0;

	if(m_CommCtrl.dwCtlMask & CM_NOTIFYTXDBREAK)
		m_CommCtrl.dwNotifyTxdBreak /= NET_TASK_TIMER;
	else
		m_CommCtrl.dwNotifyTxdBreak = 0;

	if(m_CommCtrl.dwCtlMask & CM_NOTIFYCOMMBREAK)
		m_CommCtrl.dwNotifyCommBreak /= NET_TASK_TIMER;
	else
		m_CommCtrl.dwNotifyCommBreak = 0;
*/
	if(m_CommCtrl.dwCtlMask &CM_CLOSELINK)
	{
		bCloseLink = TRUE;
		m_CommCtrl.dwCtlMask &= ~CM_CLOSELINK;
	}


	return ERR_SUCCESS;
}

void CSocket::InitCommMode()
{
	DWORD dwMode;

	dwMode = m_netInfo.dwMode;
	
	if(dwMode & ETHLINK_MODE_UDP)
		m_bTcpMode = NET_UDP_MODE;
	else
		m_bTcpMode = NET_TCP_MODE;
	
	if(dwMode & ETHLINK_MODE_CLIENT)
		m_bServerMode = NET_CLIENT_MODE;
	else
		m_bServerMode = NET_SERVER_MODE;
}

DWORD CSocket::GetLastError()
{
	DWORD dwErr;

	dwErr = errno;

	return dwErr;
}

WORD CSocket::MakeSendBuf(BYTE* pBuf)
{
	WORD wNum;
	WORD wReadPtr;
	
	if(pBuf == NULL)
		return 0;

	wNum = GetCanSendNum();

	if(m_bTcpMode == NET_UDP_MODE)
		wNum = (wNum>1023?1023:wNum);
	else
		wNum = (wNum>2047?2047:wNum);
	
	if(wNum == 0)
		return 0;
	wReadPtr = m_wTxdReadPtr;

	for(int i=0; i<wNum; i++)
	{
		pBuf[i] = m_TxdBuf[wReadPtr];
		wReadPtr ++;
		wReadPtr %= m_wMaxTxdLen;
	}

	return wNum;
}

void CSocket::AddTxdReadPtr(WORD wNum)
{
	m_wTxdReadPtr += wNum;
	m_wTxdReadPtr %= m_wMaxTxdLen;
}

void CSocket::ResetBuf()
{
	m_wRxdWritePtr = 0;
	m_wRxdReadPtr = 0;
	m_wTxdWritePtr = 0;
	m_wTxdReadPtr = 0;
}

void CSocket::CloseLink()
{
	m_bConnected = FALSE;
	m_dwError  = 0;
	if(m_hSocket != INVALID_SOCKET)
	{
		CloseSocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

	if(m_hASocket != INVALID_SOCKET)
	{
		CloseSocket(m_hASocket);
		m_hASocket = INVALID_SOCKET;
		OS_PostMessage(m_dwTagetID, SM_NOTIFYETHCLOSED, m_dwPort,0,0,0);
	}
	
	ClearWriteMask();
	ClearReadMask();
	ResetTestLinkTime();
	ResetBuf();

	m_dwReadBlockNum = 0;
	m_dwWriteBlockNum = 0;
	m_bNoReadCount = 0;
	m_dwNoReadCount = 0;
	m_bNotify = FALSE;
}

void CSocket::TcpServerLink()
{
	if(NeedListening())
		ListeningTask();
}
	
void CSocket::ListeningTask()
{
	if(m_hSocket == INVALID_SOCKET)
	{
		m_hSocket = CreateListeningSocket();
		if(m_hSocket == INVALID_SOCKET)
			return;
	}

	OnListeningLink();
}

void CSocket::OnListeningLink()
{
	SOCKET hSocket;
	CSocket* pSocket;
	struct timeval tVal;
	fd_set rd;
	long lgRtn;
	SOCKET_ADDR acceptAddr;
	 
	if(m_hSocket == INVALID_SOCKET)
		return;
	
//	lgTime = 100*1000;//AfxGetLinkNum()*1000;
	tVal.tv_sec = 0;//lgTime/1000000; ///s
	tVal.tv_usec = 0;//lgTime%1000000; //us

	FD_ZERO(&rd);
	FD_SET(m_hSocket, &rd);
	
//Modify for Ethernet Maintain 2005.04.05 	
//	if(tVal.tv_sec > 0||tVal.tv_usec > 0)
//    {
	    lgRtn = 0;
	    lgRtn = select(FD_SETSIZE, &rd, NULL, NULL, &tVal);
	
	    if(lgRtn <= 0)
		    return;
//    }
	int bLen = sizeof(SOCKET_ADDR);
		
	acceptAddr.sin_family = AF_INET;
	acceptAddr.sin_addr.s_addr = htonl(m_netInfo.dwSideIP);
	acceptAddr.sin_port = htons(m_netInfo.dwSelfPort);
	
	hSocket = accept(m_hSocket, (TSOCKET*)&acceptAddr, (int*)&bLen);

	if(hSocket == INVALID_SOCKET)
		return;

	pSocket = AfxGetSocket(&m_netInfo, htonl(acceptAddr.sin_addr.s_addr));
	if(pSocket == NULL)
	{
		CloseSocket(hSocket);
		return;
	}

	if(!pSocket->AcceptLink(hSocket))
		CloseSocket(hSocket);

//	ReportMsg("SOCKET: IP%x Accept Link OK!!!",m_netInfo.dwSideIP);

}

BOOL CSocket::AcceptLink(SOCKET hSocket)
{
	if(!IsTcpServerMode())
		return FALSE;

	if(m_hASocket != INVALID_SOCKET)
	{
		CloseSocket(m_hASocket);
		m_hASocket = INVALID_SOCKET;
	}

	m_hASocket = hSocket;
	m_bConnected = TRUE;

	OS_PostMessage(m_dwTagetID, SM_NOTIFYETHLINKED,  m_dwPort,0,0,0);

	return TRUE;
}

BOOL CSocket::CreateLink()
{	
	if(m_bConnected)
		return TRUE;

	if(IsTcpClientMode())
	{
		if(m_hASocket == INVALID_SOCKET)
		{
			m_hASocket = OpenTcpClientLink();
			if(m_hASocket == INVALID_SOCKET)
				return FALSE;
			m_bConnected = TRUE;
			return TRUE;;
		}
	}
	
	if(IsUdpMode())
	{
		if(m_hASocket == INVALID_SOCKET)
		{
			m_hASocket = OpenUdpLink();
			if(m_hASocket == INVALID_SOCKET)
				return FALSE;
			m_hSocket = m_hASocket;
			m_bConnected = TRUE;
			return TRUE;;
		}
	}

	return FALSE;
}

SOCKET CSocket::GetServerSocket()
{
	SOCKET hSocket;

	hSocket = AfxGetSamePortSocket(&m_netInfo);

	return hSocket;
}

SOCKET CSocket::CreateListeningSocket()
{
	SOCKET hSocket;

	if(m_bFirstSamePort)
		hSocket =  OpenTcpServerLink();
	else
		hSocket = GetServerSocket();

	return hSocket;
}

SOCKET CSocket::OpenTcpServerLink()
{
	SOCKET_ADDR selfAddr;
	long 		lgListen ;
	SOCKET 		hSocket;

	hSocket = INVALID_SOCKET;

	if(m_bTcpMode != NET_TCP_MODE || m_bServerMode != NET_SERVER_MODE)
		return INVALID_SOCKET;

	CloseLink();
	
	hSocket = CreateSocket();

	if(hSocket == INVALID_SOCKET)
		return INVALID_SOCKET;
	//need bind	//bind local address
	memset((char *)&selfAddr, 0, sizeof(SOCKET_ADDR));

	selfAddr.sin_family = AF_INET;
	selfAddr.sin_addr.s_addr = htonl(m_netInfo.dwSelfIP);
	selfAddr.sin_port = htons(m_netInfo.dwSelfPort);

	if(SOCKET_ERROR == bind(hSocket, (TSOCKET*)&selfAddr,sizeof(SOCKET_ADDR)))
	{
		CloseSocket(hSocket);
		hSocket = INVALID_SOCKET;

		return INVALID_SOCKET;
	}
	lgListen = listen(hSocket, 4);

	if(lgListen == SOCKET_ERROR)
	{
		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

	//设置路由，添加网关
	if(m_netInfo.dwMode & NET_ROUTE_MASK)
	{
		::Add_a_route(hSocket, m_netInfo.dwGateWay1, 0,0);
	}

	return hSocket;
}

SOCKET CSocket::OpenTcpClientLink()
{
	SOCKET_ADDR sideAddr;
	long lgRtn;
	fd_set wd;
	struct timeval tVal;
	SOCKET hSocket;
	long lgTime;

	hSocket = INVALID_SOCKET;

	if(!IsTcpClientMode())
		return INVALID_SOCKET;

	CloseLink();
	
	hSocket = CreateSocket();

	if(hSocket == INVALID_SOCKET)
		return hSocket;
	//not need bind 
	sideAddr.sin_family = AF_INET;
	sideAddr.sin_port = htons(m_netInfo.dwSidePort);
	sideAddr.sin_addr.s_addr = htonl(m_netInfo.dwSideIP);

	lgRtn = connect(hSocket, (TSOCKET*)&sideAddr,sizeof(SOCKET_ADDR));
	
	if(lgRtn == SOCKET_ERROR)
	{
		if(GetLastError() == EISCONN)////已经创建
			return hSocket;
		if(GetLastError() == EINPROGRESS)
		{
			lgTime = 100*AfxGetLinkNum()*1000;
			tVal.tv_sec = lgTime/1000000;    /*s*/
			tVal.tv_usec = lgTime%1000000; /*us*//*return not block*/

			FD_ZERO(&wd);
			FD_SET(hSocket, &wd);

			lgRtn = select(FD_SETSIZE, NULL, &wd, NULL, &tVal);

			if(lgRtn > 0 )
				return hSocket;//connect ok
		
//			ReportMsg("SOCKET: IP%x Close at connect!!!",m_netInfo.dwSideIP);
			
			CloseSocket(hSocket);
			return INVALID_SOCKET;
		}

		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

//	ReportMsg("SOCKET: IP%x connect OK!!!",m_netInfo.dwSideIP);
	return hSocket;
}

SOCKET CSocket::OpenUdpLink()
{
	SOCKET_ADDR selfAddr;
	SOCKET hSocket;
	int nRtn;
	BOOL bReuseAddr = TRUE;

	hSocket = INVALID_SOCKET;

	if(!IsUdpMode())
		return INVALID_SOCKET;
	
	CloseLink();
	
	if(!m_bFirstSamePort)
	{
		hSocket = GetServerSocket();
		return hSocket;
	} 

	hSocket = CreateSocket();

	if(hSocket == INVALID_SOCKET)
		return INVALID_SOCKET;

	//bind local address
	memset((char *)&selfAddr, 0, sizeof(SOCKET_ADDR));
	selfAddr.sin_family = AF_INET;
	selfAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(m_netInfo.dwSelfIP);
	selfAddr.sin_port = htons(m_netInfo.dwSelfPort);
 
	if(SOCKET_ERROR == bind(hSocket, (TSOCKET*)&selfAddr, sizeof(SOCKET_ADDR)))
	{
//		DWORD dwError;
//		dwError = GetLastError();
		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

	nRtn = (int)setsockopt(hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bReuseAddr,sizeof(int));
	if(nRtn == -1)
	{
		nRtn = GetLastError();
		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

	//设置路由，添加网关
	if(m_netInfo.dwMode & NET_ROUTE_MASK)
	{
		::Add_a_route(hSocket, m_netInfo.dwGateWay1, 0,0);
	}

	return hSocket;
}

int CSocket::CloseSocket(SOCKET hSocket)
{
	if(hSocket != INVALID_SOCKET)
	{
		OS_PostMessage(m_dwTagetID,(WORD)SM_COMMEXCEPT,0,m_dwPort,0,0);
		return close(hSocket);//psos，close(a):关闭指定的Socket
	}
	return 0;	
}

SOCKET CSocket::CreateSocket()
{
	int nSocketType;
	int nBufLen;
	BOOL bReuseAddr;
	SOCKET hSocket;
	int nRtn;
	int nRtn1 = 0;
	struct linger Linger;

	nSocketType = (m_bTcpMode == NET_UDP_MODE ? SOCK_DGRAM : SOCK_STREAM);
	hSocket = socket(AF_INET, nSocketType, 0);
	
	if(hSocket == INVALID_SOCKET)
	{
		nRtn = GetLastError();
		return INVALID_SOCKET;
	}

	/*set buf size*/
	nBufLen = 2*1024;
	setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nBufLen, sizeof(int));
	setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufLen, sizeof(int));
	bReuseAddr = TRUE; 

	nRtn = 0;
	nRtn = (int)setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuseAddr, sizeof(int));//ss=0:success;-1:fail
//	nRtn1 = (int)setsockopt(hSocket, SOL_SOCKET, SO_REUSEPORT, (char*)bReuseAddr, sizeof(int));//ss=0:success;-1:fail

	if(nRtn == -1 || nRtn1 == -1)
	{
		nRtn = GetLastError();

		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

	int nBio = 1;//m_bBockMode;
	if(SOCKET_ERROR == ioctlsocket(hSocket,FIONBIO ,(int)&nBio) )
	{
		//ioctlsocket==ioctl,psos对socket设置操作方式，FIONBIO：设置阻塞方式，nbio=0:block,1:noBlock
		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

	Linger.l_onoff = 1;
	Linger.l_linger = 0;
	if(setsockopt(hSocket, SOL_SOCKET, SO_LINGER, \
		(char *)&Linger, sizeof(Linger)) < 0)
	{
		CloseSocket(hSocket);
		return INVALID_SOCKET;
	}

	return hSocket;
}

void CSocket::TcpNetRead()
{
	long nReadNum,left;
	BYTE bBuf[RXDBUFLEN];

	if(m_bTcpMode != NET_TCP_MODE)
		return;
	if(m_hASocket == INVALID_SOCKET)
		return;
	
	left = m_wMaxRxdLen-GetLeftRxdNum()-1;
	if(!left)
		return;
		
	nReadNum = recv(m_hASocket, (char*)bBuf, left, 0);
	
	if(nReadNum < 0)
		NetError();
	if(nReadNum > 0)
	{
		WriteRxdBuf(bBuf, nReadNum);
		NotifyToUse(SM_READCOMM, 0, 0);
		ResetTestLinkTime();
		m_dwError = 0;
	}
	if(nReadNum == 0)////连接以断开
		NetError();
}

void CSocket::UdpNetRead()
{
	long nReadNum;
	BYTE bBuf[RXDBUFLEN];
	WORD wMaxRead;
	int nAddrLen;
	SOCKET_ADDR toAddr;

	if(m_bTcpMode != NET_UDP_MODE)
		return;
	if(m_hASocket == INVALID_SOCKET)
		return;
	
	wMaxRead = (m_wMaxRxdLen <= 1023 ? m_wMaxRxdLen-1 : 1023);
	
	memset((void *)&toAddr,0 ,sizeof(SOCKET_ADDR));
	nAddrLen = sizeof(SOCKET_ADDR);
	
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(m_netInfo.dwSidePort);
	toAddr.sin_addr.s_addr = htonl(m_netInfo.dwSideIP);

	nReadNum = recvfrom(m_hASocket, (char*)bBuf, wMaxRead, 0, (TSOCKET*)&toAddr, &nAddrLen);

	if(nReadNum < 0)
		NetError();
	if(nReadNum > 0)
	{
		ResetTestLinkTime();
		m_dwError = 0;
		DWORD dwRxdIP = ntohl(toAddr.sin_addr.s_addr);
//		DWORD dwRxdPort =  ntohs(toAddr.sin_port);
				
		if(dwRxdIP != m_netInfo.dwSelfIP)// && dwRxdPort == m_netInfo.dwSidePort)
		{
			WriteRxdBuf(bBuf, nReadNum);
			NotifyToUse(SM_READCOMM, 0, 0);
		}
	}
	if(nReadNum == 0)////连接以断开
		NetError();
}

void CSocket::NetWrite()
{
	WORD wNum;
	BYTE bBuf[TXDBUFLEN];
	long lgRtn;
	int nAddrLen;
	SOCKET_ADDR toAddr;
	DWORD dwBoradAddr;

	dwBoradAddr = GetTargetAddr();

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(m_netInfo.dwSidePort);
	toAddr.sin_addr.s_addr = htonl(dwBoradAddr);//INADDR_BROADCAST;//htonl(m_netInfo.dwSideIP);

	nAddrLen = sizeof(SOCKET_ADDR);

	if(m_hASocket == INVALID_SOCKET)
		return;
	
	wNum = MakeSendBuf(bBuf);

	if(wNum == 0)
		return;
 		
	if(m_bTcpMode == NET_UDP_MODE)	
		lgRtn = sendto(m_hASocket, (char*)bBuf, wNum, 0, (TSOCKET*)&toAddr, nAddrLen);

	else
		lgRtn = send(m_hASocket, (char*)bBuf, wNum, 0);

	if(lgRtn > 0)
	{
		AddTxdReadPtr(lgRtn);
		ResetTestLinkTime();
		m_dwWriteBlockNum = 0;
		m_dwError = 0;
	}

	if(lgRtn <= 0)////=0连接断开
		NetWriteError();
}

BOOL CSocket::TestNetLink()
{
	WORD wNum;
	char lzBuf[] = "Has Linked!";
	long lgRtn;
	int nAddrLen;
	SOCKET_ADDR toAddr;

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(m_netInfo.dwSidePort);
	toAddr.sin_addr.s_addr = htonl(GetTargetAddr());

	nAddrLen = sizeof(SOCKET_ADDR);

	if(m_hASocket == INVALID_SOCKET)
		return FALSE;
	
	wNum = strlen(lzBuf);

	if(m_bTcpMode == NET_UDP_MODE)	
		lgRtn = sendto(m_hASocket, lzBuf, wNum, 0, (TSOCKET*)&toAddr, nAddrLen);
	else
		lgRtn = send(m_hASocket, lzBuf, wNum, 0);

	if(lgRtn > 0)
		return TRUE;
	
	return FALSE;
}

void CSocket::NetWriteError()
{
	DWORD dwErr;

	dwErr = GetLastError();
	if(dwErr == EWOULDBLOCK)
	{
		m_dwWriteBlockNum ++;
		if(m_dwWriteBlockNum > MAXBLOCKNUM)
		{
			m_dwWriteBlockNum = 0;

//			ReportMsg("SOCKET: IP%x Close at WriteBlock Out!!!",m_netInfo.dwSideIP);

			CloseLink();
		}
		return;
	}

	m_dwError ++;
//	ReportMsg("SOCKET: IP%x error at Write!!! ErrNo=%x",m_netInfo.dwSideIP,dwErr);

		
	if(m_dwError > MAXERRORTIME)
	{
		m_dwError = 0;
		CloseLink();
	}
}

void CSocket::NetError()
{
	DWORD dwErr;

	dwErr = GetLastError();
	if(dwErr == EWOULDBLOCK)
		return;

	m_dwError ++;
//	ReportMsg("SOCKET: IP%x error at Read!!!  ErrNo=%x",m_netInfo.dwSideIP,dwErr);

		
	if(m_dwError > MAXERRORTIME)
	{
		m_dwError = 0;
//		ReportMsg("SOCKET: IP%x Close at RW_Error Out!!!",m_netInfo.dwSideIP);
		CloseLink();
	}
}

void CSocket::SetWriteMask()
{
	notifyCount.dwNotifyTxdBreak ++;
	notifyCount.dwNotifyCommBreak ++;

}

void CSocket::ClearWriteMask()
{
	notifyCount.dwNotifyTxdBreak = 0;
	notifyCount.dwNotifyCommBreak = 0;
}

void CSocket::SetReadMask()
{
	notifyCount.dwNotifyRxdBreak ++;
	notifyCount.dwNotifyCommBreak ++;
}

void CSocket::ClearReadMask()
{
	notifyCount.dwNotifyRxdBreak = 0;
	notifyCount.dwNotifyCommBreak = 0;
}

BOOL CSocket::IsServerMode()
{
	if(m_bServerMode == NET_SERVER_MODE)
		return TRUE;
	return FALSE;
}

BOOL CSocket::IsTcpServerMode()
{
	if(m_bTcpMode == NET_TCP_MODE && m_bServerMode == NET_SERVER_MODE)
		return TRUE;
	return FALSE;
}

BOOL CSocket::IsTcpClientMode()
{
	if(m_bTcpMode == NET_TCP_MODE && m_bServerMode == NET_CLIENT_MODE)
		return TRUE;
	return FALSE;

}

BOOL CSocket::IsUdpMode()
{
	if(m_bTcpMode == NET_UDP_MODE)
		return TRUE;
	return FALSE;
}

void CSocket::AddTestLinkTime()
{
	m_dwTestLinkCount ++;
}

void CSocket::ResetTestLinkTime()
{
	m_dwTestLinkCount = 0;
}

BOOL CSocket::IsTestLinkTime()
{
	if(m_dwTestLinkCount >= TESTLINKTIME)
	{
		ResetTestLinkTime();
		return TRUE;
	}
	return FALSE;
}

BOOL CSocket::CanSendData()
{
////0:备机不发送数据
////1:备机发送数据
/*
	BOOL bSend;
	bSend = (m_netInfo.dwMode>>31)&0x01;

	if(GetIdentify() == WM_BACKUP && !bSend)
		return FALSE;
*/
	return TRUE;
}

DWORD CSocket::GetBroadAddr()
{
	DWORD dwBroadAddr;

	dwBroadAddr = m_netInfo.dwSelfIP | ~m_netInfo.dwSelfMask;

	return dwBroadAddr;
}

DWORD CSocket::GetTargetAddr()
{
	if(IsServerMode() && IsUdpMode())
		return GetBroadAddr();

	return m_netInfo.dwSideIP;
}

BOOL CSocket::NeedListening()
{
	if(m_bFirstSamePort && IsTcpServerMode())
		return TRUE;
	return FALSE;
}

BOOL CSocket::NoNeedListening()
{
	BOOL bNoNeedListen;
	
	bNoNeedListen = IsUdpMode();
	bNoNeedListen |= IsTcpClientMode();

	return bNoNeedListen;
}

WORD CSocket::GetTxdWritePtr()
{
	return m_wTxdWritePtr;
}

WORD CSocket::GetRxdWritePtr()
{
	return m_wRxdWritePtr;
}

/*********************************************************************************************
 *
 * 功能描述      从发送缓冲区指定位置读取一定长度的数据，供维护查看
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
WORD CSocket::ReadFromTxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	for(int i = 0; i< wLen; i++)
	{
		pBuf[i] = m_TxdBuf[(wOffset+i)%m_wMaxTxdLen];
	}
		
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      从接收缓冲区指定位置读取一定长度的数据，供维护查看
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
WORD CSocket::ReadFromRxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	for(int i = 0; i< wLen; i++)
	{
		pBuf[i] = m_RxdBuf[(wOffset+i)%m_wMaxRxdLen];
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
WORD CSocket::ReadNewTxdData(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i;

	if(m_wTxdWritePtr >= wOffset)
		wLeft = m_wTxdWritePtr - wOffset;
	else
		wLeft = m_wMaxTxdLen - (wOffset-m_wTxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
	
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_TxdBuf[(wOffset+i)%m_wMaxTxdLen];
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
WORD CSocket::ReadNewRxdData(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i;

	if(m_wRxdWritePtr >= wOffset)
		wLeft = m_wRxdWritePtr - wOffset;
	else
		wLeft = m_wMaxRxdLen - (wOffset-m_wRxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
	
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_RxdBuf[(wOffset+i)%m_wMaxRxdLen];
	}
	
	return wLeft;
}

#endif

