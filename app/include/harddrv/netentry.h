#include "system.h"
#include "paradef.h"
#include "csocket.h"

#if (INSTALL_NET == YES)
 
#ifndef _ETNERNET_ENTRY_H
#define _ETNERNET_ENTRY_H

class CSocket;

#define SOCKET_ERROR	(-1)
#define INVALID_SOCKET	(-1)

#define NET_TASK_TIMER		20
#define NET_CONNECT_TIMER	1000
#define NETWATCKMAXTIME		(4000/NET_TASK_TIMER)

#define NET_EV_TIMER 		0x04
#define WATCH_DOG_TIME 		TICK2MS(10000)

#define LANx_MTU  			1500
#define LANx_HWALEN 		6
#define LANx_FLAGS 			(0x8000+0x2000+0x1+0x800)  

#ifndef INVALID_IP_ADDRESS  
#define INVALID_IP_ADDRESS  0x01000000
#endif

#ifndef INVALID_IP_PORT     
#define INVALID_IP_PORT     100
#endif

#define NET_TO_HOST_TASK_NAME "PSVR"
#define NET_TO_HOST_TASK_SYS_STACK 0x8000
#define NET_TO_HOST_TASK_USR_STACK 0x1000
#define NET_TO_HOST_TASK_FLAG (0)
#define NET_TO_HOST_TASK_INIT_ATTR (T_NOPREEMPT | T_TSLICE |T_USER|T_ISR)
#define INVALID_SOCKET		(-1)

extern BOOL m_bLinkRun;

int     InitEthernet();
void    InstallNetCard();
void    OnNetCardParaChange();
BOOL    GetNetCardIpAttr(DWORD dwPort, DWORD& dwIP,  DWORD& dwMask);
void    InitSocket();
int 	GetSocketID(WORD wPortID);
void    NetOnTimer();
void    SetNetIpAddr(int nNINo, DWORD dwIP, DWORD dwMask);

DWORD    EthernetOpen(WORD wPortID, WORD wRxdLen, WORD wTxdLen, DWORD dwFlag);
DWORD    EthernetClose(WORD wPortID);
int      EthernetRead(WORD wPortID, BYTE* pBuf, WORD wLen, DWORD dwFlag);
int      EthernetWrite(WORD wPortID, BYTE* pBuf, WORD wLen, DWORD dwFlag);
DWORD    EthernetControl(WORD wPortID, char* pBuf);
DWORD    AfxGetLinkNum();
SOCKET   AfxGetSamePortSocket(TEthPortPara *pInfo);
CSocket* AfxGetSocket(TEthPortPara *pInfo, DWORD dwSideIP);
void     SearchSamePort();
void     EnterProtectSection(DWORD& dwOldMode);
void     ExitProtectSection(DWORD dwOldMode);
void     Add_a_route(SOCKET hSocket, DWORD gw, DWORD dst,DWORD dstmsk);
TEthPortPara	*GetEthernetInfo(DWORD dwPort);

CSocket* GetSortSocket(WORD wNum);

#endif
#endif



