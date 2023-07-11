#ifndef _MAPBIT_H_
#define _MAPBIT_H_

#include "resm5234.h"
#include "system.h"
#include "msgmap.h"
#include "protocol.h"

#define MAPBIT_TIME						10						//定义看门狗定时器时间间隔
#define MAPBIT_COMMBREAK_MAX			(15000/MAPBIT_TIME)		//定义通信中断时间

class CMapBit;

typedef struct
{
	BYTE	START;			//起始
	BYTE	DesADDR;		//目的地址
	BYTE	SrcADDR;		//源地址
	BYTE	DATA1[2];		//数据位	
	BYTE	DATA2[2];		//数据位取反	
	BYTE	CRC;			//校验
}TMBFrame;


// 以太网管理
class CMBEth
{
	private:
		DWORD		m_ip;				//对端IP地址
		BYTE		m_ObjNo;			//对象序号
		BYTE		m_SelfAddr; 		//本机站址
		BYTE		m_SideAddr; 		//对端站址
		BYTE		m_SMBStart; 		//发送镜像位起始位置
		BYTE		m_SMBNum;			//发送镜像位个数
		BYTE		m_RMBStart; 		//接收镜像位起始位置
		BYTE		m_RMBNum;			//接收镜像位个数

		TBufMsg  m_FrameInfo;
		TBufMsg  m_RecvInfo;
		
	public:
		DWORD		m_dwCommBreakCnt;	//通信中断计数器

		CMapBit		*m_pMapBit;
		
		BOOL Init(CMapBit *pMapClass,BYTE no,TMBO_Disp	*pMBO);
		BOOL SendData();
		BOOL RecvData();
};

// CAN网管理
class CMBCan
{
	private:
		WORD 		m_PortID;
		BYTE		m_ObjNo;			//对象序号
		BYTE		m_SelfAddr; 		//本机站址
		BYTE		m_SideAddr; 		//对端站址
		BYTE		m_SMBStart; 		//发送镜像位起始位置
		BYTE		m_SMBNum;			//发送镜像位个数
		BYTE		m_RMBStart; 		//接收镜像位起始位置
		BYTE		m_RMBNum;			//接收镜像位个数


	public:	
		
		BOOL Init(BYTE no,TMBO_Disp	*pMBO);
		BOOL SendData();
		BOOL RecvData(BYTE *pBuf);
		
		BYTE GetSelfAddress(){return m_SelfAddr;};	
		BYTE GetSideAddress(){return m_SideAddr;};	
		WORD GetPortID(){return m_PortID;};	
};

// 串口管理
class CMBSer
{
	private:
		WORD 		m_PortID;			//端口ID
		BOOL		m_bOpen;			//端口打开标志
		BYTE		m_ObjNo;			//对象序号
		BYTE		m_SelfAddr; 		//本机站址
		BYTE		m_SideAddr; 		//对端站址
		BYTE		m_SMBStart; 		//发送镜像位起始位置
		BYTE		m_SMBNum;			//发送镜像位个数
		BYTE		m_RMBStart; 		//接收镜像位起始位置
		BYTE		m_RMBNum;			//接收镜像位个数
	public:

		BOOL Init(BYTE no,TMBO_Disp	*pMBO);
		BOOL SendData();
		BOOL RecvData(BYTE *pBuf);

		BYTE GetSelfAddress(){return m_SelfAddr;};	
		BYTE GetSideAddress(){return m_SideAddr;};	
		WORD GetPortID(){return m_PortID;};	
};

class CMapBit:public CApp
{
    DECLARE_MESSAGE_MAP()

	private:
		int 	m_hSocket;
		DWORD	m_dwError;
		DWORD	m_dwWriteBlockNum;

		DWORD m_dwTickCnt; //Tick计数
		
		TBufMsg  m_SerFrameInfo;
		TBufMsg  m_CanFrameInfo;

		TBufMsg  m_SerRxdInfo;
		TBufMsg  m_CanRxdInfo;
			
		BYTE		m_EthNum;
		BYTE		m_SerNum;
		BYTE		m_CanNum;

		CMBEth  	m_EthInfo[MAX_MAPBITOBJ_NUM];
		CMBCan 		m_CanInfo[MAX_MAPBITOBJ_NUM];
		CMBSer 		m_SerInfo[MAX_MAPBITOBJ_NUM];
			
		BOOL 	CreateSocket();
		BOOL 	CloseSocket();
		
		void 	OnTimeOut(DWORD id);
		void 	OnRelayRTMB(DWORD num);
		void 	OnReadComm(DWORD dwPort, DWORD dwFlag);
		void 	OnNetCardParaChange();

		void 	SerRecvProcess(WORD wPortID);
		void 	CanRecvProcess(WORD wPortID);
		
	public:
		WORD 	SendUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
		WORD 	RecvUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
	
		CMapBit(DWORD *pdwApp);
		BOOL Init(void);
};		

void MapBitTask(DWORD *pdwApp);


#endif
