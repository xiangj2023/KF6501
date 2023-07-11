//=======================================================================================
// 名称: Siec104.h
// 功能: IEC104从站规约类的定义
// 编写：lz 2011-12-23
// 改写：
//=======================================================================================
#ifndef _SIEC104_H
#define _SIEC104_H

#include "protocol.h"
#include "msgmap.h"
#include "relaydef.h"
#include "iecdefs.h"

#ifdef INSTALL_SGM104 


//定义104中引用的101的版本
#define VER1997			1997
#define VER2002			2002

#define VER				VER2002

#define MAX_TXDFRAME_SIZE  255  //最大发送帧长
#define MAX_RXDFRAME_SIZE  255  //最大接收帧长
#define MIN_RXDFRAME_SIZE  5    //最小接收帧长
#define MAX_COMMERR_TIME	30 		//10秒钟无数据通信失败

const	BYTE	NBYTES_LINKADDR		= 2;	//链路地址字节数 1 or 2
const	BYTE	NBYTES_COMMADDR		= 2;	//公共地址字节数 1 or 2
const	BYTE	NBYTES_REASON		= 2;	//传送原因字节数 1 or 2
const	BYTE	NBYTES_ELMENTADDR	= 3;	//信息元素地址字节数 2 or 3

const	BYTE	MAXFRMLEN	 = 255;			//最大帧长 包括同步字、长度	
const	BYTE	MAXGRPUP	 = 4;			//最大组号

#if(SYSTEM_TYPE == _SYS_KF1300_)

#define GRP_AINUM   40     //每组最多发送遥测个数	 96->48   
#define GRP_BINUM   127    //每组最多发送遥信字节
#define GRP_CINUM   24     //每组最多发送电度个数	 32 -> 24
#define GRP_SOENUM  20     //24     //每组最多发送SOE个数
#define GRP_AIENUM  16     //1     // 每组最多发送AIE个数

#else

#define GRP_AINUM   80     //每组最多发送遥测个数	 96->48   
#define GRP_BINUM   127    //每组最多发送遥信字节
#define GRP_CINUM   24     //每组最多发送电度个数	 32 -> 24
#define GRP_SOENUM  20     //24     //每组最多发送SOE个数
#define GRP_AIENUM  16     //1     // 每组最多发送AIE个数

#endif

#define CHGAI_ABS	1		 //ai变化范围取绝对值
#define CHGAI_PER	2		 //ai变化范围取百分比

//系統特定參數
#define SF_INIT_OVER		 (USER_SFLAG+0)	//子站初始化完成
#define SF_BUFFLOWOVER	 	 (USER_SFLAG+1)	//发送缓冲区已占满
#define SF_CallData	         (USER_SFLAG+2)	//目前处于召唤数据状态
#define SF_STOPSEND	         (USER_SFLAG+3)	//缓冲区满停止发送

#define SF_BIGRP1		 (USER_SFLAG+8)   //总召唤遥信组1   //必需为8的位数
#define SF_BIGRP2		 (USER_SFLAG+9)   //总召唤遥信组2
#define SF_BIGRP3		 (USER_SFLAG+10)   //总召唤遥信组3
#define SF_BIGRP4		 (USER_SFLAG+11)   //总召唤遥信组4
#define SF_BIGRP5		 (USER_SFLAG+12)   //总召唤遥信组5
#define SF_BIGRP6		 (USER_SFLAG+13)   //总召唤遥信组6
#define SF_BIGRP7		 (USER_SFLAG+14)   //总召唤遥信组7
#define SF_BIGRP8		 (USER_SFLAG+15)   //总召唤遥信组8
#define SF_BIGRP9		 (USER_SFLAG+16)   //总召唤遥信组9  
#define SF_BIGRP10		 (USER_SFLAG+17)   //总召唤遥信组10
#define SF_BIGRP11		 (USER_SFLAG+18)   //总召唤遥信组11
#define SF_BIGRP12		 (USER_SFLAG+19)   //总召唤遥信组12
#define SF_BIGRP13		 (USER_SFLAG+20)   //总召唤遥信组13
#define SF_BIGRP14		 (USER_SFLAG+21)   //总召唤遥信组14
#define SF_BIGRP15		 (USER_SFLAG+22)   //总召唤遥信组15
#define SF_BIGRP16		 (USER_SFLAG+23)   //总召唤遥信组15
#define SF_AIGRP1		 (USER_SFLAG+24)   //总召唤遥测组1
#define SF_AIGRP2		 (USER_SFLAG+25)   //总召唤遥测组2
#define SF_AIGRP3		 (USER_SFLAG+26)   //总召唤遥测组3
#define SF_AIGRP4		 (USER_SFLAG+27)   //总召唤遥测组4
#define SF_AIGRP5		 (USER_SFLAG+28)   //总召唤遥测组5
#define SF_AIGRP6		 (USER_SFLAG+29)   //总召唤遥测组6
#define SF_AIGRP7		 (USER_SFLAG+30)   //总召唤遥测组7
#define SF_AIGRP8		 (USER_SFLAG+31)   //总召唤遥测组8
#define SF_AIGRP9		 (USER_SFLAG+32)   //总召唤遥测组9
#define SF_AIGRP10		 (USER_SFLAG+33)   //总召唤遥测组10
#define SF_AIGRP11		 (USER_SFLAG+34)   //总召唤遥测组11
#define SF_AIGRP12		 (USER_SFLAG+35)   //总召唤遥测组12
#define SF_AIGRP13		 (USER_SFLAG+36)   //总召唤遥测组13
#define SF_AIGRP14		 (USER_SFLAG+37)   //总召唤遥测组14
#define SF_AIGRP15		 (USER_SFLAG+38)   //总召唤遥测组15
#define SF_AIGRP16		 (USER_SFLAG+39)   //总召唤遥测组16
#define SF_ALLSTOP		 (USER_SFLAG+40)   //总召唤结束

#define SF_CIGRP1		 (USER_SFLAG+48)   //总召唤电度组1
#define SF_CIGRP2		 (USER_SFLAG+49)   //总召唤电度组2
#define SF_CIGRP3		 (USER_SFLAG+50)   //总召唤电度组3
#define SF_CIGRP4		 (USER_SFLAG+51)   //总召唤电度组4
#define SF_ALLCISTOP     (USER_SFLAG+52)   //总召唤电度结束

#define SF_STSend			 (USER_SFLAG+56)//发送定值定值
#define SF_STSelectEcho		 (USER_SFLAG+57)//定值
#define SF_SendChangedAI	 (USER_SFLAG+58)//发送召唤数据
#define SF_HAVEREPORT		 (USER_SFLAG+59)//有报告
#define SF_HAVEBIE           (USER_SFLAG+60)//有遥信变位
#define SF_HAVESOE           (USER_SFLAG+61)//有SOE
#define SF_BOEND         	 (USER_SFLAG+62)//遥控过程结束
#define SF_HAVEAIE           (USER_SFLAG+63)//有AIE

#define SF_SENDDIR			  (USER_SFLAG+64)	//主动上传目录
#define SF_CALLDIR			  (USER_SFLAG+66)	//召唤目录

#define SF_CALLRDSJ			  (USER_SFLAG+67)	//向保护模块召唤扰动数据
#define SF_FILEREADY		  (USER_SFLAG+68)	//文件准备好
#define SF_SENDSEC			  (USER_SFLAG+69)	//正在传送节
#define SF_LASTSEG			  (USER_SFLAG+70)	//最后的段
#define SF_LASTSEC			  (USER_SFLAG+71)	//最后的节

//TxdIdle消息时应发送的帧类别TITF(m_Txd Idle Task Flag)
#define TITF_Null		0	//无		
#define TITF_CallAll    2	//总召唤
#define TITF_CallAllCI  3	//总召唤电度
#define TITF_AutoSend   4	//主动上传模式

//宏定义区文件引用区
#define MAX_TXDFM_SIZE  255  //最大发送帧长
#define MAX_RXDFM_SIZE  255  //最大接收帧长
#define MIN_RXDFM_SIZE  6    //最小接收帧长
#define MIN_FM_SIZE 4
#define MAX_104SUBDEVNO	5	//104规约挂接模块个数

#define T1 20  	//T1时间间隔，information of Test APCI's
#define T2 5  	//T2时间间隔，Supervisory acknowledgements(t2<t1)
#define T3 20  	//T3时间间隔，idle state(t3>t1);

#define STARTHEAD	0x68
#define ACT			0x1
#define CON			0x2
#define GETBIT1		0x1
#define GETBIT2		0x3 
#define IEC_K_104	0x7 

#define  STARTDT_ACT 0x7
#define  STARTDT_CON 0x0B
#define  STOPDT_ACT  0x13
#define  STOPDT_CON  0x23
#define  TESTFR_ACT  0x43
#define  TESTFR_CON  0x83
#define  MAXSEQUENCER   0x8000

//定义 K W的值
#define IEC104_K			12
#define IEC104_W			8

#define T1_TIMERID			(IEC104_K+0)		//T1定时器
#define T2_TIMERID			(IEC104_K+1)		//T2定时器
#define T3_TIMERID	 		(IEC104_K+2)		//T3定时器			
#define BO_TIMERID			(IEC104_K+3)		//遥控预置超时定时器
#define MAX_TIMERNUM		(IEC104_K+4) 		//定时器总个数

//信息体地址划分
#define AI_ADDR_START  0x4001	//遥测量起始点号
#define AI_ADDR_STOP   0x5000	//遥测量终止点号
#define BI_ADDR_START  0x0001	//遥信量起始点号
#define BI_ADDR_STOP   0x1000	//遥信量终止点号
#define CI_ADDR_START  0x6401	//电度量起始点号
#define CI_ADDR_STOP   0x6600	//电度量终止点号
#define BO_ADDR_START  0x6001	//遥控量起始点号
#define BO_ADDR_STOP   0x6200	//遥控量终止点号
#define AO_ADDR_START  0x6201	//遥调量起始点号
#define AO_ADDR_STOP   0x6400	//遥调量终止点号

#define STSELECT	0				//预制
#define STEXEC		1				//执行	
#define STCANCEL	2				//撤销
#define STSBOSELECT	1<<3			//软压板预制
#define STSBOEXEC	1<<4			//软压板执行
#define STSBOCANCEL	1<<5			//软压板撤销
#define STSBONONE	1<<6			//不操作软压板

#define MAX_IND8D_FDNUM		37
#define MAX_IND8C_FDNUM		32

typedef struct 
{
    unsigned char Start; //启动字符
    unsigned char Length; //长度
    unsigned char Control1; //控制域1
    unsigned char Control2; //控制域2
    unsigned char Control3; //控制域3
    unsigned char Control4; //控制域4
    unsigned char Style;   //类型标识
    unsigned char Definitive; //结构限定词
    unsigned char Reason_L; //传送原因
    unsigned char Reason_H; //传送原因
    unsigned char Address_L;   //公共地址（子站站址）
    unsigned char Address_H;   //公共地址（子站站址）
    unsigned char Data;  //数据开始
}TS104RxdFm,TS104TxdFm; //I104S规约的可变帧长结构    

typedef struct
{
   int    nTxdSeqNum; //记录发送包序号
   BYTE   DataLen;//数据长度
   BYTE   DataBuf[MAXFRMLEN];  //数据缓冲区
}TDataBuf; //保存从站104的发送报文

typedef struct 
{
	DWORD TxdIdleTxdFm; //TxdIdle消息时应发送的帧类别 见TITF_。。。
	WORD CallDataAddr; //召唤数据的地址：
	
}TS104Info; //S101规约的每一模块的信息     

//==== 结构定义区 ========

struct TProTimer{
	BOOL bTimer;
	WORD wTick;
	WORD wValue;
};

#define	S104_FILESATUS_IDLE		0
#define	S104_FILESATUS_INITDIR	0x01
#define	S104_FILESATUS_AUTODIR	0x02
#define	S104_FILESATUS_INITFILE	0x04
#define	S104_FILESATUS_SELFILE	0x08
#define	S104_FILESATUS_SELSEC	0x10

typedef struct
{
	BYTE		bStatus;	// 状态机信息
	BYTE		nCurDirNo;	// 当前处理目录编号	
	DWORD		dwInfoAddr;	// 信息体地址
	BYTE		nFileNum;	// 文件总个数
	BYTE		nSentCnt;	// 已发送文件个数
	char		strFileNameList[DIST_CAPACITY_MAX][DIST_INDEX_NAMELEN];//文件名字列表
	TSysTime	tFileTimeList[DIST_CAPACITY_MAX];//录波文件时间表
}TDistDirInfo;

typedef struct
{
	WORD        addrLen;		//公共地址长度
	WORD        rsnLen;			//传送原因长度
	WORD        elemaddrLen;	//信息元素长度

	WORD        yxbase;			//遥信基址
	WORD		ycbase;			//遥测基址
    WORD        ykbase;			//遥控基址
	WORD        ddbase;			//电度基址

	WORD		yxfrmType;		//遥信帧类型 1-单点,3-双点
	WORD		soefrmType;		//SOE帧类型 2-短时标 30-单点,31-双点
	WORD		ycfrmType;		//遥测帧类型 9-归一化,11-标度化,13-浮点  21-不带品质描述的归一化值
	WORD		ycbfrmType;		//变化遥测帧类型 9-归一化,11-标度化,13-浮点  21-不带品质描述的归一化值
	WORD		ykfrmType;		//遥控帧类型 45-单命令  46-双命令
	WORD		ddfrmType;		//电度帧类型 15
	WORD		rptfrmType;		//报告帧类型

	WORD		intsetcode;		//整型定值编码
}TS104Para;

// 最大节数
#define MAXFILENUM			2
#define PERSECDATALEN		(10*1024)
#define PERSEGDATALEN		200

typedef struct
{
	BYTE*		pFileBuf;	// 文件数据缓冲区
	
	DWORD		dwFileLen;	// 被召唤的文件长度
	DWORD		dwSecLen;	// 被召唤的节长度
	WORD		nSubDir;	// 子目录名		
	WORD		nFileName;	// 文件名字		
	BYTE		nFileNo;	// 文件编号
	BYTE		bFileChs;	// 文件校验和
	
	BYTE*		pSecStart;	// 节数据缓冲
	BYTE		nSecNum;	// 节数
	BYTE		nSecName;	// 节名字
	BYTE		bSecChs;	// 节校验

	WORD		nSegNum;	// 段数
	WORD		nSegNo;		// 段号

	BYTE*		pFileStart[MAXFILENUM];	//数据起始位值
	DWORD		dwDataLen[MAXFILENUM];	//数据长度
	BYTE		bCheck[MAXFILENUM];	//数据校验和
	BYTE		bReady[MAXFILENUM];	//文件准备好
}TDistFileInfo;


class CGm104 : public CProtocol
{
	DECLARE_MESSAGE_MAP()
//标准规约变量
protected:
	DWORD		m_dwTaskRunCount;	//任务正常运行时间统计(秒)
	WORD		m_NoRxdCounter;		//无接收数据计数
	WORD  		m_LocalSTNum;		//本站定值个数
	WORD		m_wFaultNo;
	
	TProTimer 	m_timer[MAX_TIMERNUM];

    WORD 		m_dwTxdCount;  		//当前发送帧记数
    WORD 		m_dwRxdCount;  		//当前接收帧记数 
    BYTE		BufWritePtr,BufAckPtr;
	
    BOOL 		bIsSTARTDT; 		//允许数据发送标志 
    BOOL 		Is101BO;			//判断101或者104遥控格式
	BYTE		m_nLeftActionReportNum;
	WORD		m_wChangeAIReadPtr;

	WORD  m_nSOENoAckCnt;		//SOE发送个数
	WORD  m_nSOENewTxSeq;		//SOE发送序号
	
	WORD  m_nBIENoAckCnt;		//BIE发送个数
	WORD  m_nBIENewTxSeq;		//BIE发送序号

	WORD  m_nALMNoAckCnt;		//告警报告发送个数
	WORD  m_nALMNewTxSeq;		//告警报告发送序号

	WORD  m_nACTNoAckCnt;		//动作报告发送个数
	WORD  m_nACTNewTxSeq;		//动作报告发送序号

	WORD  m_nAIENoAckCnt;		//AIE发送个数
	WORD  m_nAIENewTxSeq;		//AIE发送序号
	
    TS104RxdFm *pRxdFm;  			//接收帧头指针
    TS104TxdFm *pTxdFm;  			//发送帧头指针
    CommBuf 	m_Txd;
    
    TS104Info  	m_S104Info;				//S104规约信息 
    TDataBuf   	m_TxDataBuf[IEC104_K];	//保存104规约K帧数据包
    
	TBO	  		m_BO;
	TST			m_ST;
	BYTE				m_CurrentReportType;	//当前发送报告类型
	TRelayReport		m_tCurrentReport;
	TRelayEventHead		*m_pNextRelayEvent;

	long		*m_pOldAI;

	TDistDirInfo	m_DistDir;
	TDistFileInfo	m_DistFile;
	TS104Para		m_S104para;
	
public:
    BOOL Init(void);//初始化
	BOOL InitPara(void);
    CGm104(DWORD *pdwApp):CProtocol(pdwApp)
	{ 
	    m_dwTxdCount = 0;  //发送帧记数
	    m_dwRxdCount = 0;  //接收帧记数
	    bIsSTARTDT = false;
	    Is101BO = FALSE;
    };
	
protected:
//控制域报文的处理
    BOOL	RxdAPCIInfo();        //接收处理APCI信息
    BOOL	Txd68_ShortTime();    //发送短时标时间
    BOOL	Txd68_LongTime(TSysTime *pTime);     //发送长时标时间 取3610时间

    BOOL	Handle_I_Format();    //处理I格式
    BOOL	Handle_U_Format();    //处理U格式
    BOOL	Handle_S_Format();    //处理S格式     
    
	BOOL	Txd_InitOver(void);		//子站初始化完成
	BOOL	ResetVar(void);			//初始化變量

	// 超时重发管理
	BOOL 	MySetTimer(BYTE bTimerID, WORD wSecond);
	BOOL 	MyKillTimer(BYTE bTimerID);
	void 	MyCheckTimer();
	void 	MyOnTimer(BYTE bTimerID);
	BOOL 	SaveDataBuf(); 
	void 	RetryFrame(BYTE bItem);
	BOOL 	KillPrevFrame(WORD dwTxdNum);

	//消息相应函数
	void 	OnCommIdle(DWORD dwLen,DWORD PortID);
    void	OnTimeOut(DWORD dwTimerID);
	void 	OnBIBurst();
	void  	OnSOE();
	void 	OnRelayEvent(DWORD MsgCode, DWORD wParam);
	void 	OnAIE();

	//遥控预置返校消息映射
	void 	OnBOSelectEcho(WORD switchno,WORD attrib,WORD result);	
	void 	OnBOExecEcho(WORD switchno,WORD attrib,WORD result);
	
	void 	OnEthLinked(DWORD dwPara);
	void 	OnEthClosed(DWORD dwPara);
	void 	OnDisturb(DWORD dwFileNo);
	
	//通过遥控信息表方式设置遥控,默认为需要返校,用于较复杂(需保存较多原始信息)的遥控
	BOOL 	SetBO(TBO* pBO,BYTE NeedReturnFlag=1);
	//通过遥控路号\命令\属性方式设置遥控,默认为需要返校,用于不需保存原始信息的遥控
	BOOL 	SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag=1);
	BOOL 	SetBO(WORD Command){return SetBO(m_BO.No,Command,m_BO.Attrib);};
	BOOL 	HaveNewReport(void);
	BOOL 	SearchNewReport(TRelayReport &Report,BYTE &Type);
	BOOL 	SearchChangedAI(WORD *pAINo,long *pAIValue, DWORD ChangedValue, WORD wControlWord);//查找变化遥测
	void 	FreshOldAI();

    DWORD	SearchOneFrame(PBYTE Buf,WORD Len);
    void	OnFrame(BYTE* pBuf);
    BOOL	Rxd68(void); //包含ASDU数据单元的帧

//发送报文处理函数
	BOOL	TxdMonitor(void);

    BOOL	Txd_Head(BYTE dwCode,WORD Reason);//发送报文头
    BOOL	Txd_Tail(BYTE Num);//发送报文尾

    BOOL	TxdCallAll(void);
    BOOL	TxdCallAllCI(void);
    BOOL	Txd_Class1(void); //发送1级数据
    BOOL	Rxd10_0B(void);//0x0B 召唤二级用户数据 ??
    BOOL	Txd_Class2(void); //发送2级数据

    BOOL	Rxd68_64(void);//总召唤/召唤某一组数据
    BOOL	Rxd68_64_14(void);//处理总召唤命令
    BOOL	Rxd68_64_14_6(void);//处理总召唤激活命令
    BOOL	Txd68_64_14_6(void);//处理总召唤激活命令
    BOOL	Txd_AIGroup(WORD GroupNo,BYTE Reason);//发送一组遥测
    BOOL	Txd_BIGroup(WORD GroupNo,BYTE Reason);//发送一组遥信
    BOOL	Txd_CIGroup(WORD GroupNo,BYTE Reason);//发送一组电度
    BOOL	Txd_AllStop(void);//发送总召唤结束帧
    BOOL	Rxd68_67(void);//对钟处理
    BOOL	Txd68_67(void);//对钟 
    BOOL	Rxd68_6E(void);//设置参数
    BOOL	Txd68_6E(void);//设置参数
    BOOL	Rxd68_68(void);//测试链路
    BOOL	Txd68_68(void);//测试链路
	BOOL	Txd_NoData(void){ return 0; }; //无所请求数据帧
    BOOL	SearchClass1(void); //检查有无1级数据
    BOOL	Txd_BurstBI(void); //发送变化遥信
    BOOL	Rxd68_65(void);//召唤电度
    BOOL	Rxd68_65_5(void); //召唤某组电度
    BOOL	Txd68_65_5(void); //召唤某组电度
    BOOL	Rxd68_65_6(void); ////召唤所有电度
    BOOL	Txd68_65_7(void); ////召唤所有电度
    BOOL	Txd_AllCIStop(void);//发送总召唤电度结束帧
    BOOL	Txd_Soe(void); //发送SOE数据
    BOOL	Txd_ChangeAI(void); //发送变化遥测数据
    BOOL	Rxd68_66(void);//读命令处理
    BOOL	Txd68_66_AI(WORD AINo);//发送由读命令形成的遥测数据
    BOOL	Txd68_66_BI(WORD BINo);//发送由读命令形成的遥信数据
    BOOL	Txd68_66_CI(WORD CINo);//发送由读命令形成的电度数据

	BOOL	Rxd68_2E2F3B3C(void);//遥控处理
    BOOL	Rxd68_2E2F3B3C_6(void);//遥控预置/执行命令
    BOOL	Rxd68_2E2F3B3C_8(void);//遥控撤销命令
    BOOL	Txd68_2E2F3B3C(BYTE Reason); //发送遥控预置/执行/撤销返校	
    
	//104规约带时标的测试命令
    BOOL	Rxd68_6B(void);//带时标测试链路
    BOOL	Txd68_6B(void);//带时标测试链路
    BOOL	Rxd68_69(void); //复位RTU
	BOOL	Txd68_69_Ack(void);

	DWORD	ToDevID(BYTE* Devid);
	//事故简报处理函数 
	BOOL	Txd_RelayEvent(void);	//发送保护事件报告

	//定值处理函数
	BOOL	Rxd_145(void);			//召唤定值区、召唤定值
	BOOL	Rxd_147(void);			//修改定值区预置 、修改定值区执行
	BOOL	Rxd68_145_STRead();     //取定值
	BOOL	Rxd68_145_STSelect();   //整区修改定值预置

	BOOL	Txd68_145_STReadEcho(void);		//取定值返回
	BOOL	Txd68_145_STSelectEcho(void);	//整区修改定值预置返回
	BOOL	Txd68_145_STExecEcho(void);		//修改定值执行/撤销返回
	
	BOOL	Txd68_145_STSReadEcho(void);	//取定值区号返回
	BOOL	Txd68_145_STSExecEcho(void);	//修改定值区号执行/撤销返回
	BOOL	Txd68_145_STSSelectEcho(void);	//修改定值区号预置返回

	void Txd_ElementAddr(DWORD dwAddr)
	{
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOLOBYTE(dwAddr); //信息体地址Lo  :装置ID

		if(NBYTES_ELMENTADDR == 2 || NBYTES_ELMENTADDR == 3)
			m_Txd.Buf[ m_Txd.WritePtr++ ] = LOHIBYTE(dwAddr); //信息体地址Hi	装置ID
		if(NBYTES_ELMENTADDR == 3)
			m_Txd.Buf[ m_Txd.WritePtr++ ] = HILOBYTE(dwAddr);	//信息体地址:

		return;
	}

	DWORD GetNewSoeNum(void);
	DWORD GetNewBieNum(void);
	DWORD GetNewAIENum(void);
	void  ClearSentEvent(DWORD dwAckCount);
	void  ClearAllEvent();

	//录波文件查询功能
	BYTE CheckSum(BYTE * pBuf,DWORD dwLength);
	BOOL DistFileName2Time(char *strFileName,TSysTime *pTime);
	BOOL InitDistDir();
	BOOL InitDistFile(BYTE nFileName);
	BOOL SelectFile(BYTE nFileName);
	BOOL SelectSec(BYTE nSecName);

	BOOL	Rxd_122(void);					//召唤
	BOOL	Rxd_124(void);					//确认文件
	
	BOOL	Txd_126_RDSJ_DIR_AllDir(); 		//召唤上传目录
	BOOL	Txd_126_RDSJ_DIR_SubDir(); 		//召唤单个目录
	BOOL	Txd_126_RDSJ_DIR_Auto(); 		//主动上传目录
	BOOL	Txd_120_RDSJ_F_FR(BOOL bReady);	//文件准备就绪	
	BOOL	Txd_121_RDSJ_F_SR(void);		//节准备就绪
	BOOL	Txd_125_RDSJ_F_SG(void);		//上传扰动数据 类型标识125：F_SG_NA_1 段
	BOOL	Txd_123_RDSJ_F_LS(void);		//最后的节,最后的段

};

#endif
#endif
