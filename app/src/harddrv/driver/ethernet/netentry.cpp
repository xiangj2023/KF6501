#include "sys/ioctl.h"
#include "netentry.h"
#include "daedef.h"
#include "parafunc.h"

#if (INSTALL_NET == YES)

#include "csocket.h"
#include "dbfunc.h"

DWORD g_hNetTaskID = APP_TASKID_NULL;
CSocket* m_pSocket;
DWORD m_dwLinkNum;
DWORD m_dwWatchDogTime;
BOOL m_bLinkRun;
DWORD m_dwTimerID;

#define satosin(sa) ((SOCKET_ADDR *)(sa))
#define MAX_BUF 1024

int GetSocketID(WORD wPortID);

void NetMainTask(DWORD *arg)
{
	TMessage 	msg;
	DWORD 		status = ERR_MESSAGE_OK;
	OS_TaskDesp *pTask;

	if(!InitEthernet())
		return;
	
	g_hNetTaskID = arg[0];
	
	OS_SetTimer(g_hNetTaskID,1,NET_TASK_TIMER, NULL);
		
	pTask = OS_GetTaskTab(g_hNetTaskID);

	//循环调用进行消息调度,直到收到任务退出消息
	while(status != ERR_MESSAGE_QUIT)
	{
		//调用取消息函数,从任务消息队列头,取需要处理的消息
		status = OS_GetMessage(g_hNetTaskID,&msg);
			
		//判断接收的消息是否为有效消息
		if(status == ERR_MESSAGE_OK)
		{
			//清除任务运行计数
			if(pTask != NULL)
				pTask->Counter = 0;

			switch(LOWORD(msg.lParam))
			{
				case	SM_TIMEOUT:
					NetOnTimer();
					break;
				case	SM_NETCARDPARACHG:
					OnNetCardParaChange();
					break;
				default:
						break;
			}		 
		}
	}	 
}

//////////网络初始化函数
BOOL InitEthernet()
{
	m_pSocket = NULL;
	m_bLinkRun = FALSE;
	
	m_dwLinkNum = DB_GetEthLinkNum();
	if(m_dwLinkNum == 0)
		return FALSE;
	
	m_pSocket = new CSocket[m_dwLinkNum];
	if(m_pSocket == NULL)
		return FALSE;
	
	InitSocket();

	InstallNetCard();
	
	return TRUE;
}

void InitSocket()
{
	TEthPortPara	tNetPara;
	int 			i=0;
	WORD			wNum;

	wNum = DB_GetEthLinkNum();
	
	for(i=0;i<wNum;i++)
	{
		if(DB_GetETHPara(i, &tNetPara) != 0)
			continue;
					
		m_pSocket[i].m_dwPort = tNetPara.wPortID;
		m_pSocket[i].m_netInfo = tNetPara;
		
		m_pSocket[i].InitCommMode();
	}
	
	SearchSamePort();
}

void InstallNetCard()
{
	WORD  wPort;
	DWORD dwIP,dwGateWay;
	DWORD dwMask;
	BYTE  HPort;
	int  i;
	char GateWayIpAddr[20];

#if(INSTALL_LAN1 == YES)
	DB_GetEthAPara(dwIP, dwMask, dwGateWay);
	SetNetIpAddr(0, dwIP, dwMask);

	if(dwGateWay != 0)
	{
		memset(GateWayIpAddr, 0, sizeof(GateWayIpAddr));
		sprintf(GateWayIpAddr,"%d.%d.%d.%d",HHBYTE(dwGateWay),HLBYTE(dwGateWay),LHBYTE(dwGateWay),LLBYTE(dwGateWay));
		
		if(mRouteAdd("0.0.0.0", GateWayIpAddr, 0, 0, 0) == ERROR)
		{
			LogError("Add_a_route", FILE_LINE, "add route error errno=0x%x.",errno);
		}
		else
		{
			LogError("Add_a_route", FILE_LINE, "add route GW=%s.",GateWayIpAddr);
		}
	}
#endif

#if (INSTALL_LAN2 == YES)
	DB_GetEthBPara(dwIP, dwMask, dwGateWay);
	SetNetIpAddr(1, dwIP, dwMask);
	
	if(dwGateWay != 0)
	{
		memset(GateWayIpAddr, 0, sizeof(GateWayIpAddr));
		sprintf(GateWayIpAddr,"%d.%d.%d.%d",HHBYTE(dwGateWay),HLBYTE(dwGateWay),LHBYTE(dwGateWay),LLBYTE(dwGateWay));
		
		if(mRouteAdd("0.0.0.0", GateWayIpAddr, 0, 0, 0) == ERROR)
		{
			LogError("Add_a_route", FILE_LINE, "add route error errno=0x%x.",errno);
		}
		else
		{
			LogError("Add_a_route", FILE_LINE, "add route GW=%s.",GateWayIpAddr);
		}
	}

#endif

}

// 响应网卡参数改变
void   OnNetCardParaChange()
{
	// 等待2秒钟，其他任务关闭 
	OS_Sleep(2000);

	// 关闭所有SOCKET
	for(int i=0;i<DB_GetEthLinkNum();i++)
		m_pSocket[i].CloseLink();

	// 重新初始化网卡
	InitSocket();	
	InstallNetCard();
}

BOOL GetNetCardIpAttr(DWORD i, DWORD& dwIP,  DWORD& dwMask)
{
	TEthPortPara netInfo;
	
	if(m_pSocket == NULL || i >= m_dwLinkNum)
		return FALSE;
		
	netInfo = m_pSocket[i].m_netInfo;
	
	dwIP = netInfo.dwSelfIP;

	if(dwIP < INVALID_IP_ADDRESS)
	{
		dwIP = 0xc0A86464;////192.168.100.100
		dwMask = 0xFFFF0000;
	
		return TRUE;
	}

	dwMask = netInfo.dwSelfMask;

	return TRUE;
}

void NetOnTimer()
{
	for(int i=0; i<m_dwLinkNum; i++)
		m_pSocket[i].MainTask();
}

DWORD EthernetOpen(WORD wPortID, WORD wRxdLen, WORD wTxdLen, DWORD dwFlag)
{
	int nNum;
	BOOL bRtn;
	
	nNum = GetSocketID(wPortID);

	if(nNum < 0 || nNum >= m_dwLinkNum)
		return 1;
	
	m_pSocket[nNum].m_dwTagetID = RtosGetCurrentTaskId();

	bRtn = m_pSocket[nNum].OpenRxdTxdBuf(wRxdLen, wTxdLen);

	return bRtn;
}

int EthernetRead(WORD wPortID, BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	int nNum;
	int nRtn;

	nNum = GetSocketID(wPortID);

	if(nNum < 0 || nNum >= m_dwLinkNum)
		return 0;

	OS_Protect(0);

	nRtn =  m_pSocket[nNum].ReadData(pBuf, wLen, dwFlag);

	OS_UnProtect(0);

	return nRtn;
}

int EthernetWrite(WORD wPortID, BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	int nNum;
	int nRtn;

 	nNum = GetSocketID(wPortID);

	if(nNum < 0 || nNum >= m_dwLinkNum)
		return 0;
	
	OS_Protect(0);

	nRtn =  m_pSocket[nNum].WriteData(pBuf, wLen, dwFlag);

	OS_UnProtect(0);

	return nRtn;
}

DWORD EthernetClose(WORD wPortID)
{
	int nNum;

	nNum = GetSocketID(wPortID);

	if(nNum < 0 || nNum >= m_dwLinkNum)
		return ERR_SERIAL_CHANNEL;

	OS_Protect(0);

	m_pSocket[nNum].CloseLink();

	OS_UnProtect(0);
	
	return ERR_SUCCESS;
}

int GetSocketID(WORD wPortID)
{
	for(int i=0; i<m_dwLinkNum; i++)
	{
		if(wPortID == m_pSocket[i].m_dwPort)
			return i;
	}

	return -1;
}

DWORD EthernetControl(WORD wPortID, char* pBuf)
{
	int nNum;
	DWORD dwRtn;

	TCtlComm *pCommCtrl = (TCtlComm*)pBuf;
	if(pCommCtrl == NULL)	
		return 1;

	nNum = GetSocketID(wPortID);

	if(nNum < 0 || nNum > m_dwLinkNum)
		return 1;
	
	dwRtn =  m_pSocket[nNum].EthernetControl(pBuf);
	
	return dwRtn;
}

DWORD AfxGetLinkNum()
{
	return m_dwLinkNum;
}

SOCKET AfxGetSamePortSocket(TEthPortPara *pInfo)
{
	BOOL 			bSamePort;
	TEthPortPara 	netInfo;

	if(pInfo == NULL)
		return INVALID_SOCKET;
	if(m_pSocket == NULL)
		return INVALID_SOCKET;
	
	bSamePort = FALSE;

	for(int i=0; i<m_dwLinkNum; i++)
	{
		netInfo = m_pSocket[i].m_netInfo;
		bSamePort = (netInfo.dwSelfIP == pInfo->dwSelfIP);
		bSamePort &= (netInfo.dwSelfPort == pInfo->dwSelfPort);
		
		if(bSamePort && m_pSocket[i].m_bFirstSamePort)
			return m_pSocket[i].m_hSocket;
	}
	return INVALID_SOCKET;
}

void SearchSamePort()
{
	BOOL 			bSamePort;
	TEthPortPara	netInfo;
	TEthPortPara	netInfo1;

	if(m_pSocket == NULL)
		return;
	
	for(int i=0; i<m_dwLinkNum; i++)
	{
		if(!m_pSocket[i].m_bFirstSamePort)
			continue;

		netInfo = m_pSocket[i].m_netInfo;

		for(int j=i+1; j<m_dwLinkNum; j++)
		{
			netInfo1 = m_pSocket[j].m_netInfo;
			bSamePort = (netInfo1.dwSelfIP == netInfo.dwSelfIP);
			bSamePort &= (netInfo1.dwSelfPort == netInfo.dwSelfPort);

			if(bSamePort)
				m_pSocket[j].m_bFirstSamePort = FALSE;
		}
	}
}

CSocket* AfxGetSocket(TEthPortPara *pInfo, DWORD dwSideIP)
{
	BOOL 			bSamePort;
	TEthPortPara 	netInfo;

	if(m_pSocket == NULL || pInfo == NULL)
		return NULL;
	
	for(int i=0; i<m_dwLinkNum; i++)
	{
		netInfo = m_pSocket[i].m_netInfo;

		bSamePort = (netInfo.dwSelfIP == pInfo->dwSelfIP);
		bSamePort &= (netInfo.dwSelfPort == pInfo->dwSelfPort);
//		bSamePort &= (netInfo.dwSideIP == dwSideIP);

		bSamePort &= m_pSocket[i].IsServerMode();

		if(bSamePort)
			return &m_pSocket[i];
	}

	return NULL;
}

void SetNetIpAddr(int nNINo, DWORD dwIP, DWORD dwMask)
{
	char strName[16];
	char strIpAddr[16];

	if(nNINo == 0)
	{
		strcpy(strName,"motfec0");
	}
	else
	{
		strcpy(strName,"ene1");
		/*
		if(ipAttach(1,"ene") == OK)
			printf("ipAttach OK.\n");
		else
			printf("ipAttach ERROR.\n");
			*/
	}
	
	sprintf(strIpAddr,"%d.%d.%d.%d",HHBYTE(dwIP),HLBYTE(dwIP),LHBYTE(dwIP),LLBYTE(dwIP));
	
	if(ifMaskSet(strName,dwMask) != 0)
		LogError("SetNetIpAddr", FILE_LINE,"Set NetCard A Mask Error!"); 
	if(ifAddrSet(strName,strIpAddr) != 0)
		LogError("SetNetIpAddr", FILE_LINE,"Set NetCard A Address Error!"); 
}

//加入一个路由表项
//例 Add_a_route(s,				0xac140a01, 0,		   0xxxxxxxxx);
//	 Add_a_route(s,				0xac140a01, 0xac010a00,0xffffff00);
//	 Add_a_route(s,				0xac140a01, 0xac010aaa,0xxxxxxxxx);
void Add_a_route(SOCKET hSocket, DWORD gw, DWORD dst,DWORD dstmsk)
{
/*	
		int  cmd  = SIOCADDRT;
		WORD Flag = 0;
		if(!gw) return;

		if(!dst)							Flag = 0;			//default Gw dst = 0;
		else if(!(dst & (~dstmsk) ))		Flag = RTF_MASK;	//Subnet  GW dst = 0xac010a00;	 dstmsk = 0xffffff00;
		else								Flag = RTF_HOST;	//Host    Gw dst = 0xac010aaa;

		struct rtentry rt;  
        memset((char *) &rt.rt_dst, 0,sizeof(rt.rt_dst));  
        rt.rt_dst.sa_family = AF_INET; 
        ((SOCKET_ADDR *) &rt.rt_dst)->sin_addr.s_addr = htonl(dst);//dest;//htonl(0xac010aaa);//0xaa0a01ac;// 

        memset((char *) &rt.rt_gateway, 0,sizeof(rt.rt_gateway));  
        rt.rt_gateway.sa_family = AF_INET; 
        ((SOCKET_ADDR *) &rt.rt_gateway)->sin_addr.s_addr = htonl(gw);//htonl(0xac140a01);//0x010a14ac;// 
		
		if(Flag & RTF_MASK)
			rt.rt_netmask = htonl(dstmsk);

		rt.rt_flags |= (RTF_GATEWAY);
		rt.rt_flags |= Flag;

        if (ioctl(hSocket, cmd, (char *) &rt) < 0) 
		{ 
//           	LogError("Add_a_route", FILE_LINE,"ioctl(SIOCADDRT)error:0x%x hSocket = %x gw=0x%x, dst=0x%x dstmsk=0x%x", errno,hSocket,gw,dst,dstmsk); 
            return; 
		}
//		LogError("Add_a_route", FILE_LINE,"ioctl(SIOCADDRT):ok hSocket = %x gw=0x%x, dst=0x%x dstmsk=0x%x", hSocket,gw,dst,dstmsk); 
*/
}

/*********************************************************************************************
 *
 * 功能描述     根据以太网序号获取对应以太网连接的类指针
 *
 * 参数说明      - wNum	: 输入 	以太网序号
 *
 * 返回代码
 *                以太网连接类指针
 *
 * 其它说明：          
 *
**********************************************************************************************/
CSocket* GetSortSocket(WORD wNum)
{
	if(wNum >= m_dwLinkNum)
		return NULL;
	return &m_pSocket[wNum];
}

#endif

