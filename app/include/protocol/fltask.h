#ifndef _FLTASK_H_
#define _FLTASK_H_

#include "resm5234.h"
#include "system.h"
#include "msgmap.h"
#include "protocol.h"

#define FLTASK_TICK						20						//定义定时器节拍20mS
#define FLTASK_TIMEOUT					(3000/FLTASK_TICK)		//定义通信超时时间
#define FLTASK_HEARTBEET				(6000/FLTASK_TICK)		//定义心跳周期
#define FLTASK_COMMBREAK_MAX			(20000/FLTASK_TICK)		//定义通信中断时间

#define FLTASK_MINFRMLEN				6						//最小帧长
#define FLTASK_MAXFRMLEN				1024					//最大帧长

#define FLTASK_SERPORTID				0x102	//串行缺省串口号
#define FLTASK_SETAREA_NORMAL				0		//正常定值区号
#define FLTASK_SETAREA_STEPOVER_A			1		//本测越区定值区号
#define FLTASK_SETAREA_STEPOVER_B			2		//对侧越区定值区号

#define FLTASK_MODE_HEARTBEAT 	0	//心跳
#define FLTASK_MODE_AUTOFL	  	1	//自动测距
#define FLTASK_MODE_MANUALFL  	2	//手动测距
#define FLTASK_MODE_HISTORYFL 	3	//历史召测
#define FLTASK_MODE_LOWVOLFL  	4	//低压召测
#define FLTASK_MODE_SOSWITCH	5	//越区命令
#define FLTASK_MODE_SORETURN	6	//越区返回命令

#define FL_SUBST_SELFAT 	1   //本侧AT所
#define FL_SUBST_SELFSP 	2	//本侧分区所
#define FL_SUBST_SIDESP 	3	//对侧分区所
#define FL_SUBST_SIDEAT 	4	//对侧AT所
#define FL_SUBST_SIDEMAIN 	5	//对侧变电所


#define FLTASK_FUNC_HEARTREQ  		0x00	//心跳请求
#define FLTASK_FUNC_HEARTRES  		0x80	//心跳响应

#define FLTASK_FUNC_RELAYREQ  		0x01	//保护启动测距请求
#define FLTASK_FUNC_RELAYRES  		0x81	//保护启动测距响应

#define FLTASK_FUNC_MANUALREQ  		0x02	//手动测距请求
#define FLTASK_FUNC_MANUALRES  		0x82	//手动测距响应

#define FLTASK_FUNC_HISTREQ  		0x03	//召测历史请求
#define FLTASK_FUNC_HISTRES  		0x83	//召测历史响应

#define FLTASK_FUNC_LOWVALREQ  		0x04	//召测低压锁定请求
#define FLTASK_FUNC_LOWVALRES  		0x84	//召测低压锁定响应

#define FLTASK_FUNC_STEPOVERCMDREQ  		0x05	//越区启动指令
#define FLTASK_FUNC_STEPOVERCMDRES  		0x85	//越区启动响应

#define FLTASK_FUNC_STEPOVERRETREQ  		0x06	//越区返回指令
#define FLTASK_FUNC_STEPOVERRETRES  		0x86	//越区返回响应

#define FLTASK_SUBSET_POLLING  		1	//轮询模式
#define FLTASK_SUBSET_COMMAND  		2	//命令模式

#define FLTASK_LINK_SERIAL  		1	//串口连接模式
#define FLTASK_LINK_TRANS  			2	//路由连接模式
#define FLTASK_LINK_ETHERNET  		0	//以太网直连接

#define FLTASK_COMMAND_SWITCH		0x00	//越区命令
#define FLTASK_COMMAND_RETURN		0x01	//越区返回命令


class CFLTask;

typedef struct
{
	BYTE	START;			//起始
	BYTE	DevNo;			//设备编号
	BYTE	Func;			//功能码
	BYTE	DataLenLO;		//数据域长度低字节	
	BYTE	DataLenHI;		//数据域长度高字节	
	BYTE	DATA;			//数据域	
}TFLFrameHead;

// 子站信息
class CFLSubDev
{
	private:
		CFLTask		*m_pFLTask;
		BYTE		m_byParaNo;			//参数编号
		BYTE		m_byNetAddr;		//子网通信地址
		BYTE		m_byFLNo;			//测距编号
		BYTE		m_byLinkMode;		//连接方式
		BOOL		m_bRoute;			//是否路由
		BOOL		m_bValid;			//是否跨区有效
		BOOL		m_bInclude;			//是否包含，每次过程启动时划分子集
		BOOL		m_bSideDev;			//是对侧设备
		TFL_Disp	m_FLDisp;			//测距对象描述		

	public:
		CFLSubDev();
		
		DWORD 		m_dwNetIP;			//IP地址
		WORD		m_nTimeoutCnt;		//通信超时计数
		WORD		m_nNoAckCnt;		//通信无应答计数
		BOOL		m_bProcFlag;		//轮询标志
		BOOL		m_bHeart;			//有心跳
		BOOL		m_bHaveAck;			//有应答
		BOOL		m_bRelayFL;			//保护测距
		BOOL		m_bManualFL;		//手动测距
		BOOL		m_bForeFL;			//召测历史
		BOOL		m_bLowValFL;		//召测低压锁定
		BOOL		m_bStepOverCmd;		//越区指令执行
		BOOL		m_bStepOverRet;		//越区指令返回

		BYTE		m_byFLDataIndex;	//测距数据区编号
		BYTE		m_CallType;			//召测类型
		
		TBufMsg  	m_TxdInfo;			//发送缓冲
		TFLFrameHead *m_pRxdFrame;		//接收帧;

		BOOL 	Init(BYTE no,TFL_Disp *pFLDisp,CFLTask *pFLTask);
		void 	ClearAllPollCmd();
		
		BYTE	GetFLNo(){return m_byFLNo;};
		BYTE	GetDevType(){return (m_FLDisp.wControl&0x0F);};
		BYTE	GetLinkMode(){return m_byLinkMode;};
		BOOL	IsValid(){return m_bValid;};
		BOOL	IsPolling(){return m_bInclude;};
		BOOL	IsSideDev(){return m_bSideDev;};
		
		void 	Include(){m_bInclude = TRUE;};
		void 	Disinclude(){m_bInclude = FALSE;};
/*		
		BOOL 	Enable()
		{	
			if(m_bValid)
				return TRUE;
			
			m_bValid = TRUE;
			m_FLDisp.wControl |= 0x8000;
			return DB_SetFLObjDisp(m_byParaNo, &m_FLDisp);
		};
		
		BOOL 	Disenable()
		{
			if(!m_bValid)
				return TRUE;
			
			m_bValid = FALSE;
			m_FLDisp.wControl &= 0x7FFF;	
			return DB_SetFLObjDisp(m_byParaNo, &m_FLDisp);
		};
*/		
		void 	SwitchNetComm();
		BOOL 	SendData();
		BOOL 	TxdProcess();
		BOOL 	OnFrame();
		BOOL 	OnDataIsOK();

		BOOL 	FillFLSendData(subChannelCheck	*pFLData);
		BOOL 	SaveFLrecvData(subChannelCheck	*pFLData);
		BOOL 	TxdFrame(BYTE FuncCode,WORD wDataLen);

		// 发送越区切换命令
		BOOL 	TxdStepOverCmd();
		// 发送越区返回命令
		BOOL 	TxdStepOverRet();
		// 发送保护测距命令
		BOOL 	TxdRelayFLCmd();
		// 发送手动测距命令
		BOOL 	TxdManualFLCmd();
		// 召测历史测距数据
		BOOL 	TxdHistoryFLCmd();
		// 召测低压锁定数据请求
		BOOL 	TxdLowValFLCmd();
		// 召测低压锁定数据请求
		BOOL 	TxdHeartFLCmd();

		// 接收发送越区切换命令请求
		BOOL 	RxdStepOverCmdReq();
		// 接收发送越区返回命令请求
		BOOL 	RxdStepOverRetReq();
		// 接收发送保护测距命令请求
		BOOL 	RxdRelayFLCmdReq();
		// 接收发送手动测距命令请求
		BOOL 	RxdManualFLCmdReq();
		// 接收召测历史测距数据请求
		BOOL 	RxdHistoryFLCmdReq();
		// 接收召测低压锁定数据请求
		BOOL 	RxdLowValFLCmdReq();		
		// 接收召测心跳数据请求
		BOOL 	RxdHeartBeatCmdReq();

		// 接收发送越区切换命令响应
		BOOL 	RxdStepOverCmdRes();
		// 接收发送越区返回命令响应
		BOOL 	RxdStepOverRetRes();
		// 接收发送保护测距命令响应
		BOOL 	RxdRelayFLCmdRes();
		// 接收发送手动测距命令响应
		BOOL 	RxdManualFLCmdRes();
		// 接收召测历史测距数据响应
		BOOL 	RxdHistoryFLCmdRes();
		// 接收召测低压锁定数据响应
		BOOL 	RxdLowValFLCmdRes();
		// 接收心跳数据响应
		BOOL 	RxdHeartBeatCmdRes();
};

typedef struct
{
	BYTE		m_SeqCnt;					//并发计数
	BYTE		m_SeqList[FLCOMPLICATENUM];	//并发序号表
	TAbsTime	m_Time[FLCOMPLICATENUM];	//并发测距时间
}TRelayFLCtrlInfo;

class CFLTask:public CApp
{
    DECLARE_MESSAGE_MAP()

	private:
		BYTE	m_FLSubNum;			//测距子所个数
		BYTE	m_FLMainNo;			//测距主所全局编号
		BYTE	m_byLinkMode;		//连接模式
		BOOL	m_bMainStation;		//是否主所
		BOOL	m_bCommValid;		//是否需要通信
		BOOL	m_bStepOver;		//是当前处于越区
		
		int 	m_hSocket;
		DWORD	m_dwError;
		DWORD	m_dwWriteBlockNum;

		DWORD 	m_nTickCnt; 		//Tick计数

		BYTE	m_DevNo;	//当前轮询模块
		CFLSubDev m_FLSubDev[MAX_SUBSTATION_NUM];//测距子所

		void		*m_pMemBlock;		//预留内存块						
		TBufMsg  	m_FrameInfo;		//帧缓冲
		TBufMsg  	m_RecvInfo;			//接收缓冲
			
		BOOL 	CreateSocket();
		BOOL 	CloseSocket();
		
		void 	OnTimeOut(DWORD id);
		void 	OnCallSubData(DWORD dwTimeLO,WORD wTimeHi,WORD Mode);
		void 	OnStopCallData(DWORD Mode);
		void 	OnStepOverCmd(DWORD Mode);
		void 	OnStopStepOver(DWORD Mode);
		void 	OnDataIsOk(DWORD DataPtr,WORD Result);
		void 	OnRelayChange(DWORD dwDataType,DWORD dwIndex);

		BOOL 	SearchOneFLFrame(PBYTE Buf,WORD wLen);
		BOOL 	SearchFLFrame(TBufMsg* pMsgBuf,TBufMsg  *pFrameInfo);

	public:
		CFLTask(DWORD *pdwApp);
		
		BOOL 	Init(void);
		BOOL 	InitFLPara(void);
		BOOL 	InitSubST(void);
		BOOL 	InitMainST(void);
		
		BOOL		m_bCommand;			//越区命令过程
		DWORD		m_nCommandCnt;		//命令倒计时
		BYTE		m_CmdType;			//越区命令类型
		TAbsTime	m_tFLTime;			//测距时间
		WORD 		m_SerPortID;		//串口ID
		WORD		m_TransPortID;		//透传端口ID
		DWORD		m_dwMSPAddr;		//主所IP地址
		WORD		m_TransDataRP;		//透明数据读指针	
		
		BOOL		m_bRelayFL;			//启动保护测距过程
		BOOL		m_bManualFL;		//启动手动测距过程
		BOOL		m_bForeFL;			//启动召测历史过程
		BOOL		m_bLowValFL;		//启动召测低压锁定过程

		TRelayFLCtrlInfo	m_tRelayFLInfo;	//保护测距信息

		WORD 	SendUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
		WORD 	RecvUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
		WORD 	RecvTransData(BYTE * pBuf,WORD wLen);

		BOOL 	IsCallDataEnd(BYTE byFLType); //召测结束
		BOOL 	IsCommandEnd();			//越区切换结束
				
		CFLSubDev *GetDev();

		BYTE	GetLinkMode(){return m_byLinkMode;};
		BYTE	*GetRecvFrame(){return m_FrameInfo.pBuf;};
		void	ClearRecvFrame(){m_FrameInfo.wWritePtr = 0;};
		BOOL	IsMainStation(){return m_bMainStation;};	//是否主所	
		BOOL	IsStepOver(){return m_bStepOver;};	//是否越区	
		
		BOOL 	SwitchToNextSerDev(void);
		BOOL 	SwitchToAddress(BYTE Addr);
		void 	SwitchPollingSubSet(BYTE Mode);

		WORD 	RecvData();		//接收数据

		void 	TxdMonitor();	//发送管理
		void 	RxdMonitor();	//接收管理

		
		BOOL PushRelayFL(BYTE Seq,TAbsTime *pTime);		
		BOOL PopRelayFL();
		BOOL DelRelayFL(BYTE Seq);
		BOOL IsCurrentRelayFL(BYTE Seq);
		BOOL HaveRelayFLCmd();
		BYTE GetRelayFLSeq();
		BOOL GetRelayFLTime(TAbsTime *pTime);			
};

void	FLTASKTask(DWORD *pdwApp);

#endif
