#ifndef _SHELL_H_
#define _SHELL_H_

#include "system.h" 
#include "msgmap.h"

void WHShell_Task(DWORD *pdwApp);

#define		MAX_FRAME_LEN		255		//最大帧长
#define		MIN_FRAME_LEN		3		//最小帧长
#define		CAN_COMMIDLE		40		//CANBUS正常通信空闲时间
#define		SHELL_BASIC_TIMER	50
#define		SHELL_SAMPLE_TIMER	(100/SHELL_BASIC_TIMER)
#define 	MAX_COMMERR_TIME	(10000/SHELL_BASIC_TIMER) 		//10秒钟无数据通信失败

#define		CMD_TYPE_NULL		0
#define		CMD_TYPE_RECORD		1
#define		CMD_TYPE_TXDRXD		2
#define		CMD_TYPE_SHOWYC		3
#define		CMD_TYPE_SHOWYX		4
#define		CMD_TYPE_SHOWBIE	5
#define		CMD_TYPE_SHOWSOE	6
#define		CMD_TYPE_SHOWREP	7
#define		CMD_TYPE_SHOWRM		8
#define		CMD_TYPE_SHOWDD		9
#define 	CMD_TYPE_YK			10
#define		CMD_TYPE_MISC		11
#define		CMD_TYPE_SHOWSAMPLE		12
#define		CMD_TYPE_SHOWDISTDIR	13
#define		CMD_TYPE_SHOWRUNSTA		14
#define		CMD_TYPE_SHOWINITSTA	15
#define 	CMD_TYPE_SETTIME		16
#define		CMD_TYPE_GETTIME		17
#define 	CMD_TYPE_SHOWSYSVALUE	18
#define 	CMD_TYPE_UPDATEDATA		19
#define 	CMD_TYPE_HELP			20
#define 	CMD_TYPE_VARMEM			21
#define 	CMD_TYPE_VAREEPROM		22
#define 	CMD_TYPE_VARTXD			23
#define 	CMD_TYPE_VARRXD			24
#define 	CMD_TYPE_SHOWPORT		25
#define 	CMD_TYPE_UPDATECF		26
#define 	CMD_TYPE_SHOWSTAT		27
#define 	CMD_TYPE_REBOOT			28
#define 	CMD_TYPE_KEY			29
#define		CMD_TYPE_LCDINF			30
#define		CMD_TYPE_CLEAREVENT		31
#define		CMD_TYPE_SHOWRMRAW		32
#define		CMD_TYPE_RESETPARA		33
#define		CMD_TYPE_SHOWMAPBIT		34
#define		CMD_TYPE_MAKEREP		35
#define		CMD_TYPE_MAKEALM		36
#define		CMD_TYPE_SHOWFL			37
#define		CMD_TYPE_ACK			255

class CWHShell : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	BOOL Init(void);//初始化
	CWHShell(DWORD *pdwApp):  CProtocol(pdwApp){};
	
protected:
	WORD	m_wCmdType;
	WORD	m_wCmdTypeLast;
	WORD	m_wCmdPara[4];
	WORD	m_CommandPtr;
	WORD	m_ShowPortID;	//监视端口号
	DWORD	m_dwOffset;	
	DWORD	m_dwCount;	
	char	m_CommandBuffer[MAX_FRAME_LEN];
	BYTE	m_DataBuffer[512];	
	char	m_LastCommand[64];
	DWORD	m_dwTimerCnt;
	BOOL	m_bCommandExute;
	BOOL	m_bShowAll;
	BOOL	m_bEcho;
	BYTE	m_RebootType;
	
	WORD	m_nRecordRead;
	WORD	m_wShowCnt,m_wShowMax;
	WORD	m_nSTTRead,m_nALMRead,m_nACTRead;
	DWORD	m_NoRxdCounter;
	
	void 	TxdMonitor();
	DWORD 	SearchOneFrame(PBYTE Buf,WORD wLen);
	void 	ClearCommand(char *pCmdEnd);
	void 	ProcessAckCommand();
	BOOL 	SearchCommand();
	void 	ExuteCommand();
	void 	OnCommIdle(DWORD dwLen,DWORD PortID);
	void 	OnFrame(PBYTE Buf);
	void 	OnTimeOut(DWORD id);
	void 	OnEthLinked(DWORD dwPara);
	void 	OnEthClosed(DWORD dwPara);
};

#endif	

