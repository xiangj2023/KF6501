//*******************************************************************************************//
//  模块名称: sDF103.H
//  模块功能: 实现IEC103从站规约的头文件处理
//  编制日期: 2010.6.5
//  说明：
//*******************************************************************************************//
#ifndef _SIEC103_H
#define _SIEC103_H

//********************  头文件引用区  ********************
#include "protocol.h"
#include "msgmap.h"
#include "relaydef.h"
#include "iecdefs.h"

#ifdef INSTALL_S103 

//********************  宏定义区  ************************
//报文缓冲区大小定义
#define MAX_TXDFRAME_SIZE  255  //最大发送帧长
#define MAX_RXDFRAME_SIZE  255  //最大接收帧长
#define MIN_RXDFRAME_SIZE  5    //最小接收帧长

#define MAX_COMMERR_TIME	10 		//10秒钟无数据通信失败

//广播地址的定义
#define MASTER_BROADCAST_ADDRESS  0xFF      //主站对从站的广播


//帧同步字符的定义
#define SYN_START_10   0x10     //固定帧长启动字符
#define SYN_START_68   0x68     //可变帧长启动字符
#define SYN_STOP       0x16     //帧结束字符


//==== 控制域的定义 ====

//启动报文位定义
#define PRM_MASTER     1        //启动站
#define PRM_SLAVE      0        //从动站

//帧计数有效位的宏定义
#define FCV_VALID      1        //帧计数位有效
#define FCV_INVALID    0        //帧计数位无效

//要求访问位定义
#define ACD_HAVECLASS1 1        //有1级用户数据
#define ACD_NOCLASS1   0        //无1级用户数据

//数据流控制位定义
#define DFC_OK         0        //正常
#define DFC_OVERFLOW   1        //缓冲区满

//功能码的定义
//FC_S 表示监视方向(从站)
#define FC_S_ACK          0     //确认
#define FC_S_BUSY         1     //链路忙
#define FC_S_DATA         8     //以数据响应
#define FC_S_NODATA       9     //无召唤数据
#define FC_S_LINKOK       11    //链路正常
#define FC_S_LINKERROR    14    //链路服务未工作
#define FC_S_LINKUNFIN    15    //链路服务未完成

//FC_M 表示控制方向(主站)
#define FC_M_RESETCU      0     //复位通信单元
#define FC_M_DATA         3     //传送数据,要求确认
#define FC_M_BROADCAST    4     //传送数据,无应答
#define FC_M_RESETFCB     7     //复位帧计数位
#define FC_M_CALLLINK     9     //召唤链路状态
#define FC_M_CALLCLASS1   10    //召唤1级数据
#define FC_M_CALLCLASS2   11    //召唤2级数据


//==== 应用服务数据单元定义 ====

//类型标识的定义,1-31为兼容范围,32-255为专用范围
//TYP_S 表示监视方向(从站)
#define TYP_S_TM       1        //带时标的报文
#define TYP_S_TMR      2        //带相对时标的报文
#define TYP_S_ME1      3        //被测值Ⅰ
#define TYP_S_TME      4        //带相对时标的被测值
#define TYP_S_IDENT    5        //标识
#define TYP_S_SST      6        //时间同步
#define TYP_S_CALLSTOP 8        //总召唤终止
#define TYP_S_ME2      9        //被测值Ⅱ
#define TYP_S_ME3	   50		//被测值III

#define TYP_S_BISP 	   40		//单点遥信
#define TYP_S_BIDP 	   41		//双点遥信
#define TYP_S_BIGP 	   44		//成组遥信

#define TYP_S_GDATA    10       //通用分类数据
#define TYP_S_GIDENT   11       //通用分类标识

#define TYP_S_DD	   36		//脉冲计数量帧
#define TYP_S_YKRTN    64		//遥控返回帧
#define TYP_S_ACK88    88       //召唤脉冲计数量确认


//TYP_M 表示控制方向(主站),
#define TYP_M_SST      6        //时间同步
#define TYP_M_CALL     7        //总召唤
#define TYP_M_GDATA    10       //通用分类数据
#define TYP_M_COMMAND  20       //一般命令
#define TYP_M_GCOMMAND 21       //通用命令

#define TYP_M_CALLDD   88		//召唤脉冲计数量
#define TYP_M_YKCOMMAND 64      //遥控处理

//传送原因,1-63为兼容范围,64-255为专用范围
//COT_S 表示监视方向(从站)
#define COT_S_BURST    1        //突发
#define COT_S_CYCLE    2        //循环
#define COT_S_RESETFCB 3        //复位帧计数位
#define COT_S_RESETCU  4        //复位通信单元
#define COT_S_START	   5		// 启动或重新启动
#define COT_S_TEST     7        //测试模式
#define COT_S_SST      8        //时间同步

#define COT_S_CALL     9        //总召唤
#define COT_S_CALLSTOP 10       //总召唤终止
#define COT_S_LOCAL    11       //当地操作
#define COT_S_REMOTE   12       //远方操作
#define COT_S_ACK      20       //命令的肯定认可
#define COT_S_NAK      21       //命令的否定认可
#define COT_S_GWACK    40       //通用分类写命令的肯定认可
#define COT_S_GWNAK    41       //通用分类写命令的否定认可
#define COT_S_GRACK    42       //通用分类读命令的有效数据响应
#define COT_S_GRNAK    43       //通用分类读命令的无效数据响应
#define COT_S_GWANS    44       //通用分类写确认

//COT_M 表示控制方向(主站)
#define COT_M_SST      8        //时间同步
#define COT_M_CALL     9        //总召唤的启动
#define COT_M_COMMAND  20       //一般命令
#define COT_M_GWRITE   40       //通用分类写命令
#define COT_M_GREAD    42       //通用分类读命令

//功能类型
#define FUN_BU         1        //间隔单元
#define FUN_TZ         128      //距离保护
#define FUN_I          160      //过流保护
#define FUN_IT         176      //变压器差动保护
#define FUN_IL         192      //线路差动保护
#define FUN_GEN        254      //通用分类
#define FUN_GLB        255      //全局
#define FUN_DC         252      //直流保护

//信息序号
//INF_S 表示监视方向(从站)
//0-15,系统功能
#define INF_S_GLB              0     //属于全局功能类型

//16-31,状态
#define INF_S_BI           	16    //遥信
#define INF_S_SOE          	17    //SOE

//144-159,被测值
#define INF_S_MEA_I            144   //被测值I
#define INF_S_MEA_IV           145   //被测值I,V
#define INF_S_MEA_IVPQ         146   //被测值I,V,P,Q
#define INF_S_MEA_IVN          147   //被测值In,Ven
#define INF_S_MEA_2            148   //被测值2
#define INF_S_MEA_3            148   //被测值2

//INF_M 表示控制方向(主站)
//0-15,系统功能
#define INF_M_GLB              0     //属于全局功能类型

//一般命令标识 DCO
#define DCO_M_TESTMOD		0   //测试模式
#define DCO_M_RUNMOD		1   //运行模式
#define DCO_M_CALLYX		10	//召唤开入量
#define DCO_M_HISEVENT		20  //取历时动作纪录
#define DCO_M_TESTAD		30	//测试AD通道
#define DCO_M_SYSINF		40	//取模块系统信息
#define DCO_M_DEVREVERT		50	//装置复归

#define CMD_ST_SELECT   			0x0001
#define CMD_ST_EXECUTE  			0x0002
#define CMD_ST_CANCEL   			0x0004
#define CMD_STS_READ    			0x0008
#define CMD_STS_SELECT  			0x0010
#define CMD_STS_EXECUTE 			0x0020
#define CMD_STS_CANCEL  			0x0040
#define CMD_ST_READ     			0x0080
#define CMD_CFG_READ     			0x0100
#define CMD_SBO_READ      			0x0200
#define CMD_SBO_SELECT    			0x0400
#define CMD_SBO_EXECUTE  			0x0800
#define CMD_SBO_CANCEL  			0x1000
#define CMD_SYSST_SELECT    		0x2000

//==== 兼容级别的定义 ====
#define COL_NOGEN              2     //未采用通用分类服务
#define COL_GEN                3     //采用通用分类服务

#define SF_RETRY			(USER_SFLAG+0)   //重发标志
#define SF_CALLALL          (USER_SFLAG+1)   //总召唤
#define SF_CALLALL_AI       (USER_SFLAG+2)   //总召唤
#define SF_CALLALL_BI       (USER_SFLAG+3)   //总召唤
#define SF_CALLALL_STOP     (USER_SFLAG+4)	 //总召唤结束
#define SF_HAVESOE          (USER_SFLAG+5)   //有SOE
#define SF_HAVEREPORT       (USER_SFLAG+6)   //有报告

#define SF_REPORT_DD		(USER_SFLAG+7)   //召唤电度
#define SF_RESETCU			(USER_SFLAG+8)
#define SF_RESETFCB			(USER_SFLAG+9)
#define SF_START			(USER_SFLAG+10)
#define SF_SST_ACK			(USER_SFLAG+11)

#define SF_HaveBOReturn   	(USER_SFLAG+12)    //有遥控返校

#define SF_GEN_DATA					(USER_SFLAG+32)		//通用数据标志区
#define SF_GEN_READSTDATA			(USER_SFLAG+32)		//读定值
#define SF_GEN_READSTAREA			(USER_SFLAG+33)		//读定值区
#define SF_GEN_READSBO				(USER_SFLAG+34)		//读软压板
#define SF_GEN_READSYSSTDATA		(USER_SFLAG+35)		//读系统定值
#define SF_GEN_WRITEWITHCONFIRM		(USER_SFLAG+36)		//带确认的写
#define SF_GEN_WRITEWITHEXECUTE		(USER_SFLAG+37)		//带执行的写
#define SF_GEN_WRITEWITHCANCEL		(USER_SFLAG+38)		//写条目终止

//#define SF_GEN_WRITEALLSTDATA		(USER_SFLAG+35)		//写所有定值
//#define SF_GEN_WRITESTDATA			(USER_SFLAG+36)		//写单个定值
//#define SF_GEN_WRITESTAREA			(USER_SFLAG+37)		//写定值区号
//#define SF_GEN_WRITESBO				(USER_SFLAG+38)		//写软压板
//#define SF_GEN_WRITESYSSET			(USER_SFLAG+39)		//写软压板

//==== 应用服务数据单元定义 ====

//类型标识的定义,1-31为兼容范围,32-255为专用范围
//TYP_S 表示监视方向(从站)
//TYP_M 表示控制方向(主站)
#define TYP_S_CALLSPI   40					//总召唤时传输的单点信息状态帧				 
#define TYP_S_BURSTSPI  41					//带时标的单点信息状态变化帧
#define TYP_S_CALLDPI	42					//总召唤时传输的双点信息状态帧
#define TYP_S_BURSTDPI	43					//带时标的双点信息状态变化帧
#define TYP_S_CALLYX	44					//总召唤时传输的单点状态和状态变化信息帧
#define TYP_S_REPORT	61					//带相对时标的故障报告报文

#define PBO_NEW         0x40    //新遥控命令
#define PBO_BUSY        0x41    //遥控正在处理（命令已下发，等待返校）
#define PBO_OK          0x42    //遥控返校正确或执行正确
#define PBO_ERR         0x43    //遥控错误（不支持的遥控命令）

/*需要在保护头文件里面定义*/
#define KF6500_TYPE_AI	TYP_S_ME3
#define KF6500_TYPE_BI	TYP_S_ME2

#define M_TYPE_Ia		0//M_I
#define M_TYPE_Ib		0//M_IT
#define M_TYPE_Ic		0//M_IF
#define M_TYPE_Ua		0//M_U1
#define M_TYPE_Ub		0//M_U2
#define M_TYPE_Uc		0//M_U3
#define M_TYPE_P		0//M_P
#define M_TYPE_Q		0//M_Q
#define M_TYPE_PF		0//RM_FZA

#define DEV_FUN_TYPE	FUN_BU

#define MAX_IND61_FDNUM	38 		//61帧中最大故障电量个数

#pragma	pack(1)

//控制域结构定义(控制方向)
typedef struct
{
	BYTE RES:1;            		//保留
	BYTE PRM:1;                 //启动站标识位
	BYTE FCB:1;                 //帧计数位
	BYTE FCV:1;                 //帧计数有效位
	BYTE FC:4;                  //功能码
}T_M_Control;

//控制域结构定义(监视方向)
typedef struct
{
	BYTE RES:1;            		//保留
	BYTE PRM:1;                 //启动站标识位
	BYTE ACD:1;                 //要求访问位
	BYTE DFC:1;                 //数据流控制
	BYTE FC:4;                  //功能码
}T_S_Control;

//应用服务数据单元结构
typedef struct
{
	BYTE byTYP;                 //类型标识
	BYTE byVSQ;                 //可变结构限定词
	BYTE byCOT;                 //传送原因
	BYTE byADDRESS;             //应用服务单元地址
	BYTE byFUN;                 //功能类型
	BYTE byINF;                 //信息序号
	BYTE byData;                //数据区
	BYTE byGenData;				//通用数据区
}TAsdu;

//固定长度数据帧头结构(控制方向)
typedef struct
{
	BYTE byStart;               //启动字符
	T_M_Control Control;        //控制域
	BYTE byAddress;             //地址域
	BYTE byChecksum;            //帧校验和
	BYTE byStop;                //结束字符
}T_M_FrameFix;

//可变长度数据帧头结构(控制方向)
typedef struct
{
	BYTE byStart1;              //启动字符
	BYTE byLength1;             //长度
	BYTE byLength2;             //长度(重复)
	BYTE byStart2;              //启动字符(重复)
	T_M_Control Control;          //控制域
	BYTE byAddress;             //地址域
	TAsdu Asdu;                 //应用服务数据单元
}T_M_FrameVar;

//帧头结构(控制方向)
typedef union
{
	T_M_FrameFix FrameFix_10;
	T_M_FrameVar FrameVar_68;
}T_M_FrameHead;

//固定长度数据监视方向帧头结构
typedef struct
{
	BYTE byStart;               //启动字符
	T_S_Control Control;        //控制域
	BYTE byAddress;             //地址域
	BYTE byChecksum;            //帧校验和
	BYTE byStop;                //结束字符
}T_S_FrameFix;

//可变长度数据监视方向帧头结构
typedef struct
{
	BYTE byStart1;              //启动字符
	BYTE byLength1;             //长度
	BYTE byLength2;             //长度(重复)
	BYTE byStart2;              //启动字符(重复)
	T_S_Control Control;        //控制域
	BYTE byAddress;             //地址域
	TAsdu Asdu;                 //应用服务数据单元
}T_S_FrameVar;

//监视方向帧头结构
typedef union
{
	T_S_FrameFix FrameFix_10;
	T_S_FrameVar FrameVar_68;
}T_S_FrameHead;

#pragma pack()

//********************  类定义区  ************************
class CSiec103 : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	TCtlComm ctlComm;

	BOOL Init(void);//初始化
    CSiec103(DWORD *pdwApp):CProtocol(pdwApp){};
	
protected:
	WORD  m_LocalYXNum;						//本站遥信数
	WORD  m_LocalYCNum;						//本站遥测数
	WORD  m_LocalDDNum;						//本站电度数
	WORD  m_LocalSTNum;						//本站定值个数
	BYTE *m_OldYXTable;						//保存上次总召唤发送的遥信

	BYTE  	Time[7];
	BYTE	m_nLeftActionReportNum;
	WORD	m_NoRxdCounter;					//无接收数据计数
	
	TBO	  				m_BO;
	TST					m_ST;
	WORD				m_SBONo;			//软压板号
	TRelayReport		m_tCurrentReport;
	TRelayEventHead		*m_pNextRelayEvent;
	
	BYTE				*m_pLast68Frame;	//保存上一帧控制方向报文
	CommBuf			m_Txd;
	
	T_S_FrameHead * pTxdFrameHead;     //发送帧头指针
	T_M_FrameHead * pRxdFrameHead;     //接收帧头指针

	TIECInfo		m_DevInfo;       //处理时需要存储的信息结构

	DWORD	SearchOneFrame(PBYTE pBuf,WORD wLength);
	BYTE	CheckSum(BYTE * pBuf,BYTE byLength);		  //计算校验和
	BOOL	ReceiveControlFlag(BYTE byStart);            //处理接收控制字的标志位
	
	//接收帧处理函数
	void OnFrame(BYTE* pBuf);   //接收解帧

	BOOL Rxd10_ResetCU(void);              //接收复位通信单元帧
	BOOL Rxd10_ResetFCB(void);			   //接收复位帧计数位帧
	BOOL Rxd10_Call_Link(void);			   //召唤链路状态
	BOOL Rxd10_Call_Class1(void);		   //召唤一级数据
	BOOL Rxd10_Call_Class2(void);		   //召唤二级数据
	BOOL Rxd68_SST_6(void);				   //对时
	BOOL Rxd68_CallAll_7(void);			   //总召唤
	BOOL Rxd68_Command_20(void);		   //一般命令()
	BOOL Rxd68_YKCommand_64(void);		   //遥控命令
	BOOL Rxd68_CallDD_88(void);			   //召唤电度

	BOOL Rxd68_GComm_21(void);
	BOOL Rxd68_STAreaRead_21();
	BOOL Rxd68_STDataRead_21();
	BOOL Rxd68_SBORead_21();
	BOOL Rxd68_SYSSTDataRead_21();
	
	BOOL Rxd68_GData_10(void);
	BOOL Rxd68_STDataWriteAll_10();
	BOOL Rxd68_STDataWriteOne_10();
	BOOL Rxd68_STAreaWrite_10();
	BOOL Rxd68_SBOWrite_10();
	BOOL Rxd68_SysSTDataWrite_10();

	//发送帧处理函数
	//======  兼容范围  ============
	BOOL Txd10_BaseFrame(BYTE byFC);
	BOOL Txd10_ACK(void) { return Txd10_BaseFrame(FC_S_ACK); };
	BOOL Txd10_LinkOk_11(void) { return Txd10_BaseFrame(FC_S_LINKOK); };
	BOOL Txd10_NoData_9(void) { return Txd10_BaseFrame(FC_S_NODATA); };
	BOOL TxdClass1Data(void);
	BOOL TxdClass2Data(BYTE Reason);
	BOOL Txd68_AI_9(BYTE Reason);
	BOOL Txd68_AI_50(BYTE Reason);
	BOOL Txd68_RM_GEN(BYTE Reason);
	BOOL Txd68_Tm_1(void);
	BOOL Txd68_CallStop_8(void);
	BOOL Txd_Indication_5(BYTE Reason);
	BOOL Txd_SST_6(void);

	BOOL Txd68_STDataReadRtn_10(BYTE Group,BYTE Entry);
	BOOL Txd68_STAreaReadRtn_10();
	BOOL Txd68_SBOReadRtn_10();
	BOOL Txd68_GenWriteWithConfirm();
	BOOL Txd68_GenWriteWithExucute();

	//=======  专用范围  ===========
	BOOL Txd68_DD_36(void);
	BOOL Txd68_CallAllYX_1(TEventBI *evSoe,BYTE Reason);
	BOOL Txd68_BI_40(BYTE Reason);
	BOOL Txd68_BI_44(BYTE Reason);
	BOOL Txd68_SOE_41(void);
	BOOL Txd68_Report_61();
	BOOL Txd68_BO_Echo_64(void);

	//========  响应函数 ============
	void OnTimeOut(DWORD id);
	void OnSOE();
	void OnRelayEvent(DWORD MsgCode, DWORD wParam);
	void OnBOSelectEcho(WORD switchno,WORD attrib,WORD result);
	void OnBOExecEcho(WORD switchno,WORD attrib,WORD result);	

	//其他处理函数
	BOOL HaveNewReport(void);
	BOOL HaveNewSOE(void);
	BOOL GetACDFlag(void);
	BOOL Txd68_Head(BYTE byFC);
	BOOL Txd68_Tail(void);
	BOOL SetAsduHead(BYTE byTYP,BYTE byVSQ,BYTE byCOT,BYTE byADDRESS,BYTE byFUN,BYTE byINF);
	void Make103P_BI(DWORD BIGroupNo);
	void Make103P_AI(DWORD AINo);
	void Make103P_Report(WORD wFaultNo,TRelayEventHead *pEvent);//TAbsTime *pStartTime);
	BOOL P103_YK_Pro(BYTE byYKNo,BYTE byActionTyp);
	BOOL SearchNewReport(TRelayReport &Report,BYTE &Type);
	
		//通过遥控信息表方式设置遥控,默认为需要返校,用于较复杂(需保存较多原始信息)的遥控
	BOOL SetBO(TBO* pBO,BYTE NeedReturnFlag=1);
	//通过遥控路号\命令\属性方式设置遥控,默认为需要返校,用于不需保存原始信息的遥控
	BOOL SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag=1);
};

#endif

#endif

