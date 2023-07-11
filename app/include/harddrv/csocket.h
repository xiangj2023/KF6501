#include "portdefs.h"
#include <socklib.h>
#include <inetlib.h>
#include <iflib.h>
#include <selectlib.h>
#include <routeLib.h>
#include <ipproto.h>

#if(INSTALL_NET == YES)

#ifndef _SOCKET_CLASS
#define _SOCKET_CLASS


#define TXDBUFLEN			2048
#define RXDBUFLEN			2048

#define MAXBLOCKNUM			100
#define NOREADCOUNT			20
#define NORUNTIME			2000/NET_TASK_TIMER

#define MAXWRITEBLOCKNUM	200
#define MAXREADBLOCKNUM		10000
#define TESTLINKTIME		3000
#define MAXERRORTIME		80

#define NET_UDP_MODE		0
#define NET_TCP_MODE		1

#define NET_SERVER_MASK 	1
#define NET_SERVER_MODE		0
#define NET_CLIENT_MODE		1

#define NET_ROUTE_MASK		0x04
#define NET_ROUTE_MODE		1
#define NET_NOROUTE_MODE	0

#define NOTIFYREANUM		10
#define NOTIFYWRITENUM		10

#define ioctlsocket         ioctl

typedef sockaddr 				TSOCKET;
typedef struct sockaddr_in 		SOCKET_ADDR;
typedef int						SOCKET;

struct  TNetNotifyCount
{
	DWORD		dwNotifyWrite;
	DWORD		dwNotifyRead;
	DWORD		dwNotifyRxdBreak;
	DWORD		dwNotifyTxdBreak;
	DWORD		dwNotifyCommBreak;
};

class CSocket
{
public:
	CSocket();
	~CSocket();

	BOOL NeedListening();
	BOOL m_bFirstSamePort;
	SOCKET m_hSocket;
	SOCKET m_hASocket;
	DWORD m_dwError;
	BOOL  bCloseLink;
	
	TEthPortPara m_netInfo;

	BYTE m_bTcpMode;
	BYTE m_bServerMode;
	DWORD m_dwTagetID;
	DWORD m_dwTaskID;
	DWORD m_dwTimeID;

	DWORD m_dwTestLinkCount;
	DWORD m_dwNoReadCount;
	BYTE m_bBockMode;
	TCtlComm m_CommCtrl;
	DWORD m_dwPort;
	DWORD m_dwReadBlockNum;
	DWORD m_dwWriteBlockNum;

	void MainTask();
	void TestLinkTask();
	int WriteData(BYTE* pBuf, WORD wLen, DWORD dwFlag);
	int ReadData(BYTE* pBuf, WORD wLen, DWORD dwFlag);
	void SetNetInfo(TEthPortPara *pInfo);
	DWORD EthernetControl(char* pBuf);
	void InitCommMode();
	DWORD OpenRxdTxdBuf(WORD wRxdLen, WORD wTxdLen);
	BOOL CanSendData();
	BOOL AcceptLink(SOCKET hSocket);//////for tcp server mode;
	BOOL IsServerMode();
	void ResetBuf();
	void CloseLink();

	WORD GetTxdWritePtr();
	WORD GetRxdWritePtr();
	WORD ReadFromTxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen);
	WORD ReadFromRxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen);

	WORD ReadNewTxdData(BYTE* pBuf, WORD wOffset,WORD wLen);
	WORD ReadNewRxdData(BYTE* pBuf, WORD wOffset,WORD wLen);
	
private:
	DWORD m_dwNoRunCount;
	BOOL m_bWriteContinue;
	WORD m_wAppID;
	BOOL m_bNotify;
	WORD m_bNoReadCount;
	BOOL m_bConnected;
	BOOL m_bOpend;
	WORD m_wRxdWritePtr;
	WORD m_wRxdReadPtr;
	WORD m_wTxdWritePtr;
	WORD m_wTxdReadPtr;
	BYTE* m_TxdBuf;
	BYTE* m_RxdBuf;
	WORD m_wMaxTxdLen;
	WORD m_wMaxRxdLen;
	
	TNetNotifyCount notifyCount;
	BOOL CreateLink();

	void ReadTask();
	void WriteTask();
	void NotifyTask();

	void Create();
	void NetError();
	DWORD GetLastError();

	void ListeningTask();
	void OnListeningLink();
	SOCKET GetServerSocket();
	SOCKET CreateListeningSocket();
	SOCKET OpenTcpServerLink();
	SOCKET OpenTcpClientLink();
	SOCKET OpenUdpLink();
	void TcpServerLink();
	int CloseSocket(SOCKET hSocket);
	SOCKET CreateSocket();
	DWORD GetBroadAddr();
	DWORD GetTargetAddr();
	void NetWriteError();

	void WriteRxdBuf(BYTE* bBuf, WORD wNum);
	void WriteTxdBuf(BYTE* pBuf, WORD wNum);
	WORD GetLeftRxdNum();
	WORD GetCanWriteNum();
	WORD GetCanSendNum();
	DWORD NotifyToUse(WORD wMsg, WORD wLen, DWORD dwFlag);
	void TcpNetRead();
	void UdpNetRead();
	void NetWrite();
	WORD MakeSendBuf(BYTE* pBuf);
	void AddTxdReadPtr(WORD wNum);
	void ClearWriteMask();
	void ClearReadMask();
	void SetWriteMask();
	void SetReadMask();

	void AddTestLinkTime();
	void ResetTestLinkTime();
	BOOL IsTestLinkTime();

	BOOL IsTcpServerMode();
	BOOL IsTcpClientMode();
	BOOL IsUdpMode();
	BOOL TestNetLink();
	BOOL NoNeedListening();
};
									
#endif
#endif

