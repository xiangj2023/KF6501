#ifndef _CPROTOCOL_H_
#define _CPROTOCOL_H_

#include "system.h"
#include "appcore.h"
#include "platform.h"
#include "portdefs.h"
#include "daedef.h"
#include "daefunc.h"
#include "dbfunc.h"

#define INSTALL_S103
#define INSTALL_S104
//#define INSTALL_SGM104
//#define INSTALL_MMODRTU

#define DF_Exit				OS_EXIT


//标志定义
#define MAX_TASKFLAGNO    	256    			//每个模块最多的标志位数

#define FLAG_OFF   			0
#define FLAG_ON    			1
#define FLAG_SWAP  			2

//=============  标志位定义  ===============
#define 	SYSTEM_SFLAG     	0      		//系统保留标志位区
#define 	USER_SFLAG       	32     		//用户标志位区 

#define 	SF_CommErr         	(SYSTEM_SFLAG+0)    //通讯中断，用于通讯优化
#define 	SF_RxdNeed        	(SYSTEM_SFLAG+1)    //需要接收帧,用于通讯中断判断
#define 	SF_Sleep	  	  	(SYSTEM_SFLAG+2)    //模块处于主动休眠期（无下行数据）
#define 	SF_WakeUp	  	  	(SYSTEM_SFLAG+3)    //发送重试，用于主站规约的通讯优化

//＝＝＝＝＝＝　系统专用事件标记＝＝＝＝＝＝＝＝

//=============  通讯检测位定义  ===============
#define FM_SHIELD   		0xFFFF0000      //屏蔽字
#define FM_OK       		0x00010000      //检测到一个完整的帧
#define FM_ERR      		0x00020000      //检测到一个校验错误的帧
#define FM_LESS     		0x00030000      //检测到一个不完整的帧（还未收齐）

//所需要的通讯消息
#define MCC_COMMMSG 		(0x1F)  		//需要CommCtrl控制的消息
#define MCC_ReadComm		(1<<0)  		//需要SM_READCOMM消息
#define MCC_WriteComm		(1<<1)  		//需要SM_WRITECOMM消息
#define MCC_CommIdle		(1<<2)  		//需要SM_COMMIDLE消息
#define MCC_CommTxdIdle		(1<<3)  		//需要SM_TXDIDLE消息
#define MCC_Baud			(1<<4)  		//改变波特率
#define MCC_CLOSELINK		(1<<5) 
#define DRVBUFSIZE  512      				//底层驱动发送／接收缓冲区大小

#define TM_1S     			 1       //1		//秒定时

typedef struct 
 {
    DWORD Flag[ MAX_TASKFLAGNO/32 ];
 } FLAGS ; //规约io


struct TBufMsg
{
	WORD wReadPtr;
	WORD wWritePtr;
	WORD wBufLen;
	BYTE* pBuf;
};

struct TMsgCtrl
{
	DWORD dwCode;     		//消息映射
	DWORD dwChgFlag;  		//改变标志
	
	WORD  wRxdPreNum;       //接收满wRxdPreNum个字符后发消息
	WORD  wTxdPreNum;       //发送少于wTxdPreNum个字符后发消息
	DWORD dwCommIdleTime;   //通道空闲CommIdleTime毫秒后发消息  
	DWORD dwTxdIdleTime;  	//通道空闲CommIdleTime毫秒后发消息
	DWORD dwBaud;  			//波特率　　若为０则使用不改变波特率
};

struct CommBuf
{
	WORD ReadPtr;
	WORD WritePtr;
	BYTE* Buf;
};

/*------------------------------------------------------------------------------*/
/*        注册通讯发送/接收事件 DevCommRegiste函数中Flag的定义                  */
/*------------------------------------------------------------------------------*/
#define DCR_Txd   1    //发送一报文
#define DCR_Rxd   2    //接收一正确报文

void NeatenIoBuf(TBufMsg* pMsgBuf);

//*******************************  规约类定义区  *******************************//
class CProtocol: public CApp
{
	DECLARE_MESSAGE_MAP()

public:
   	CProtocol(DWORD *pdwApp);
	BOOL Init();
	void Exit(void *p)
	{
		ReportMsg((char *)p);
		::OS_Exit(0); 
	};
	
private:
	BOOL m_bHaveRxdFm; //是否还有未处理完的和数据
	
	TBufMsg  m_FrameInfo;
	TMsgCtrl m_MsgCtrl;
	FLAGS 	 m_Flags;
	DWORD	 m_TxdFlag; 
	WORD	 m_CommErrCnt;
	WORD 	 m_dwRetryCount;  //重发次数统计

	WORD	m_TxdFrmCnt;
	WORD	m_RxdFrmCnt;
	
	void 	CommCtrl();
	DWORD 	ReadFromComm();

//	void 	NeatenIoBuf(TBufMsg* pMsgBuf);

	BOOL 	SearchFrame();//查找校验正确的报文

	void 	OnReadComm(DWORD  dwPort, DWORD dwFlag);
	void 	OnWriteCont(DWORD dwPort, DWORD dwFlag);
	
protected:
	TBufMsg			m_RxdInfo;
	TBufMsg 		m_TxdInfo;
	DWORD  			m_dwAppID;   		//任务ID号
	DWORD 			m_wPortID;			//端口ID号
	BYTE  			m_GBAddressFlag; 	//是否是广播地址
	WORD			m_TxdWritePtr;		//发送写指针
	BYTE* 			GetTxdBuf();
	TAppInfo   		m_App;

	BOOL 	CommOpen();
	DWORD 	GetPortID(void){return m_wPortID;};//获取端口ID号
	void 	SetBufLen(WORD wRxdLen, WORD wTxdLen);
	void 	GetBufLen(WORD& wRxdLen, WORD& wTxdLen);
	void 	StartMsg(DWORD MsgID);
	void 	StartMsg(DWORD MsgID, DWORD MsgValue);
	void 	StopMsg(DWORD MsgID);
	WORD 	WriteComm(WORD wLen, WORD Address = 0, DWORD dwFlag = 0);
	BOOL 	ShowMsg(void* head,BYTE *pbuf=NULL,WORD len=0,void *tail=NULL);//显示信息
	
	void 	ResetRetryCount(){ m_dwRetryCount=0;};   //重发次数清0
	void 	IncRetryCount(){ m_dwRetryCount++;};     //重发次数加1
	void 	DisableRetry(){ m_dwRetryCount=1000;};   //禁止重发
	WORD 	GetRetryCount(){return m_dwRetryCount;}; //获取重发次数
    void 	TxdRetry(void); //重发报文
	void 	DisplayRxdBuf();
	void 	DisplayTxdBuf();
	
	virtual DWORD SearchOneFrame(BYTE* pBuf, WORD nLen);
	virtual void OnFrame(BYTE* pBuf){};

	virtual DWORD GetGBAddress(void);
	
	void 	ClearRxdBuf(void)//清空接收缓冲区
	{	
	     	while(ReadFromComm())
		    m_RxdInfo.wWritePtr = m_RxdInfo.wReadPtr = 0;
	};  

	void	SetFlag(DWORD FlagNo,DWORD Status=FLAG_ON);
	DWORD	GetFlag(DWORD FlagNo);
	void	SetFlagByte(DWORD ByteNo,BYTE Status);
	BYTE	GetFlagByte(DWORD ByteNo);
	void	SetFlagWord (DWORD WordNo,WORD Status);
	WORD	GetFlagWord(DWORD WordNo);
	void 	SetFlagDWord (DWORD DWordNo,DWORD Status);
	DWORD	GetFlagDWord (DWORD DWordNo);
	BOOL  	CheckClearFlag(DWORD FlagNo);
	BOOL  	RegisterCommStatus(BYTE Status);
};

/*标准LPC校验算法*/
BYTE LPC(BYTE BeginCode,F_PBYTE Buf,WORD num);

BYTE CheckSum(BYTE BeginCode,F_PBYTE Buf,DWORD num);

BOOL  IsTimeNew(TAbsTime time1,TAbsTime time2);

#define INIPARA_MAX_LENGTH		5120		//文件长度为5k
#define INIPARA_MAX_LINELENGTH  100			//每行的最大长度
#define INIPARA_MAX_SEGLENGTH   30			//段名的最大长度
#define INIPARA_MAX_PARALENGTH  30			//变量名的最大长度
#define INIPARA_MAX_VALUELENGTH 30			//变量值的最大长度

#define INIPARA_MAX_FILENAMELENGTH 256		//变量值的最大长度

#define INIPARA_MAX_ITEMNUM 100			//变量条目最大个数

typedef struct
{
	char Section[INIPARA_MAX_SEGLENGTH];
	char ParaName[INIPARA_MAX_PARALENGTH];
	char ParaValue[INIPARA_MAX_VALUELENGTH];
}TIniParaData;

int 	InitProtocolPara();
int 	chtohex(char c);
DWORD 	atoix(char* ParaValue);

void 	SCanbus_Task(DWORD *pdwApp);
void 	WHShell_Task(DWORD *pdwApp);

#ifdef INSTALL_S103
void 	S103_Task(DWORD *pdwApp);
#else
#define S103_Task NULL
#endif

#ifdef INSTALL_S104
void 	S104_Task(DWORD *pdwApp);
#else
#define S104_Task NULL
#endif

#ifdef INSTALL_SGM104
void 	SGm104_Task(DWORD *pdwApp);
#else
#define SGm104_Task NULL
#endif

#ifdef INSTALL_MMODRTU
void 	MModRtu_Task(DWORD *pdwApp);
#else
#define MModRtu_Task NULL
#endif

extern TIniParaData* g_IniParaData;
extern DWORD        g_IniParaNum;


#endif

