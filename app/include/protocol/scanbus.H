#ifndef _SCANBUS_H_
#define _SCANBUS_H_

#include "system.h" 
#include "msgmap.h"
//宏定义区

/*------------------规约宏定义------------------------*/
#define		PRO_CANBUS			1		//从站CANbus规约的规约号

#define		MAX_FRAME_LEN		1200	//最大帧长
#define		MIN_FRAME_LEN		14		//最小帧长
#define		MAX_RETRY_NUM		5		//最大重发次数

#define		CAN_MAXFRAMELEN		255		//CANBUS最大帧长
#define		CAN_TIMER_TICK		100		//CANBUS基本定时器
#define		CAN_COMMIDLE		100		//CANBUS正常通信空闲时间
#define		CAN_RETRYIDLE		200		//CANBUS重发空闲时间
#define		CAN_RUNIDLE			2000	//CANBUS无数据时的空闲时间，发送通道空闲帧

#define		SER_MAXFRAMELEN		511		//串口最大帧长
#define		SER_TIMER_TICK		100		//串口基本定时器
#define		SER_COMMIDLE		500		//串口正常通信空闲时间
#define		SER_RETRYIDLE		1000	//串口重发空闲时间
#define		SER_RUNIDLE			2000	//串口无数据时的空闲时间，发送通道空闲帧

#define		ETH_MAXFRAMELEN		1200	//以太网最大帧长
#define		ETH_TIMER_TICK		100		//以太网基本定时器
#define		ETH_COMMIDLE		100		//以太网正常通信空闲时间
#define		ETH_RETRYIDLE		200		//以太网重发空闲时间
#define		ETH_RUNIDLE			2000	//以太网无数据时的空闲时间，发送通道空闲帧

#define     CAN_COMMERR			(20000/CAN_TIMER_TICK) 			//20秒钟无数据通信失败
#define		MAX_MEMBLOCK_SIZE	(200*1024)						//最大内存块长度
#define		MAX_FILE_WAITTIME	(30*60*1000/CAN_TIMER_TICK)		//最大文件传输超时时间


#define 	PERFRM_NUM_SET		40
#define 	PERFRM_NUM_SW		64
#define 	PERFRM_NUM_CF		40

#define 	PERFRM_NUM_BIATTR	40
#define 	PERFRM_NUM_BOATTR	40
#define 	PERFRM_NUM_AOATTR	40
#define 	PERFRM_NUM_MBO		6
#define 	PERFRM_NUM_DISTDIR	6

#if(SYSTEM_TYPE == _SYS_KF1300_)
#define		MAX_PROFILE_LEN		(5*1024*1024)	//最大规约传送文件长度
#else
#define		MAX_PROFILE_LEN		(3*1024*1024)	//最大规约传送文件长度
#endif

/*---------------------遥控信息标示-----------------------*/
#define		PBO_SELECT			1		//遥控选择
#define	 	PBO_EXECUTE			2		//遥控执行
#define	  	PBO_CANCEL 			3		//遥控撤销
#define		PBO_DIRECT			4		//遥控直接执行
#define		PBO_SUCESS  		1		//遥控成功
#define		PBO_FAIL			0		//遥控失败
			
#define     PBO_NULL			0		//不用
#define 	PBO_OPEN			1		//控分
#define 	PBO_CLOSE			2		//控合

#define		PBOINFOSIZE			9		//遥控保留信息字节数
//复位类别
#define		 RESET_WARM			0
#define      RESET_COLD			1
#define      RESET_DATA			2
#define      RECOVER_DATA			3
#define      RECOVER_COEF			4
#define      RECOVER_PARA			5

//数据掩码
/*
#define		AI_VALID			((DWORD)1<<0)	//允许遥测主动上传
#define		BI_VALID			((DWORD)1<<1)	//允许遥信主动上传
#define		CI_VALID			((DWORD)1<<2)	//允许电度主动上传
#define		BURSTAI_VALID		((DWORD)1<<8)	//允许变化遥测主动上传
#define		SOE_VALID			((DWORD)1<<9)	//允许SOE主动上传	
#define		BURSTBI_VALID		((DWORD)1<<10)	//允许变化遥信主动上传
#define		RELAYALARM_VALID	((DWORD)1<<11)	//允许保护告警事项主动上传
#define		RELAYSTART_VALID	((DWORD)1<<12)	//允许保护启动事项主动上传
#define		RELAYACTION_VALID	((DWORD)1<<13)	//允许保护动作事项主动上传
#define		BO_VALID			((DWORD)1<<21)	//允许遥控
#define		SAFETY_VALID		((DWORD)1<<7)	//允许平安报文上报
#define		FILE_VALID			((DWORD)1<<24)	//允许文件传输	
*/

/*缺省上报间隔*/
#define     CAN_AIREPTM			3
#define     CAN_BIREPTM			5
#define     CAN_CIREPTM			30
#define     CAN_SAFETYREPTM		5
#define     CAN_AIDEADZONE		1
#define     CAN_BIEREPTM		100
#define     CAN_BIBREPTM		100
#define     CAN_RELAYREPTM		100
#define     CAN_BURSTAIREPTM	500
#define     CAN_LEDREPTM		60

/*-----------------功能码定义区-----------------------*/
#define		FUN_CON				00	//确认
#define		FUN_IDLE			01	//空闲
#define		FUN_READVALUE		02	//读指定数据对象的值
#define		FUN_READSPECIALVALUE	03	//读特定数据对象的值
#define		FUN_READATTRIB		12	//读指定数据对象的属性
#define		FUN_WRITEVALUE		13	//写指定数据对象
#define		FUN_WRITEATTRIB		14	//写指定数据对象的属性
#define		FUN_SUBSTITUTE		17	//替代指定数据对象的值
#define		FUN_UNSUBSTITUTE	18	//取消替代指定数据对象的值

#define		FUN_SELECT			20	//选择指定的数据对象
#define		FUN_SELECTWITHVALUE	21	//带值的对象选择
#define		FUN_OPERATE			22	//操作指定的数据对象
#define		FUN_CANCEL			23	//取消指定数据对象的选择操作
#define		FUN_DIROPERATE		24	//直接操作指定的数据对象
#define		FUN_READFILE		31	//读文件操作
#define		FUN_WRITEFILE		32	//写文件操作
#define		FUN_DELETEFILE		33	//删除文件操作
#define		FUN_REPORTVALUE		40	//数据对象值报告
#define		FUN_QUERYLOGBYREAD	41	//基于读指针的事项查询
#define		FUN_QUERYLOGBYWRITE	42	//基于写指针的事项查询
#define		FUN_QUERYLOGBYOFF	43	//基于偏移量的事项查询
#define		FUN_COMMONCONTROL	50	//通用控制命令

#define		FUN_LOGICDEBUG		60	//逻辑方程调试用

/*-----------------数据对象集定义区-----------------------*/
#define		DOC_NONE			0	//无效信息
#define		DOC_AI				01	//遥测
#define		DOC_AIWITHQUALITY	02	//带品质描述的遥测
#define		DOC_AIBURST			03	//变化遥测
#define		DOC_AIEVENT			04	//遥测事项顺序纪录

#define		DOC_BI				8	//单点遥信
#define		DOC_DBI				9	//双点遥信
#define		DOC_BIGROUP			10	//单点遥信
#define		DOC_BIWITHQUALITY	11	//带品质描述的单电遥信
#define		DOC_DBI				12	//双点遥信
#define		DOC_DBIWITHQUALITY	13	//带品质描述的双点遥信
#define		DOC_BIBURST			14	//单点变化遥信
#define		DOC_DBIBURST		15	//双点变化遥信
#define		DOC_BIEVENT			16	//单点的事项顺序纪录
#define		DOC_DBIEVENTT		17	//双点事项顺序纪录
#define		DOC_RW				18	//继电器字状态
#define		DOC_CI				20	//电度计数值
#define		DOC_CIWITHQUALITY	21	//带品质描述的电度计数值
#define		DOC_SRM				22	//二次侧保护测量值
#define		DOC_SRMITHQUALITY	23	//带品质描述的二次侧保护测量值
#define		DOC_FRM				24	//一次侧保护测量值
#define		DOC_FRMITHQUALITY	25	//带品质描述的一次侧保护测量值
#define		DOC_DCSIDATA		26	//直流保护统计数据

#define		DOC_BO				30	//单点遥控数据
#define		DOC_DBO				31	//双点遥控数据
#define		DOC_AO				32	//遥调
#define		DOC_SYNC			34	//同期
#define		DOC_RELAYCTRL		35	//传动
#define		DOC_SPECIALCMD		36	//模拟报告\启动录波\定值复制
#define		DOC_LEDSTATUS		37	//LED灯状态
#define		DOC_RELAYCTRLWITHBLOCK		38	//带闭锁保护传动


#define		DOC_SETTING			40	//定值
#define		DOC_SETTINGAREA		41	//当前定值区号
#define		DOC_SW				42	//软压板
#define		DOC_SYSTEMSETTING	43	//辅助定值
#define		DOC_CF				44	//系数
#define		DOC_RELAYALARMREPORT		46	//保护告警报告
#define		DOC_RELAYSTARTREPORT		47	//保护启动报告
#define		DOC_RELAYACTIONREPORT		48	//保护动作报告

#define		DOC_OPERATEEVENT	51	//操作事件
#define		DOC_LOGERROR		52	//异常信息
#define		DOC_VERSION			53	//系统版本信息
#define		DOC_NETCFG			54	//网络配置
#define		DOC_SAFETY			55	//平安报文
#define		DOC_DEVSTATUS		56	//设备状态
#define		DOC_WATCHPORT		57	//端口报文监视
#define		DOC_DEVDISP			58	//设备描述信息

#define		DOC_SYSTIME			60	//相对时钟
#define		DOC_ABSTIME			61	//绝对时钟
#define		DOC_FILEDATA		62	//文件数据
#define		DOC_FILEDISP		63	//文件描述
#define		DOC_VARIAL			64	//变量（端口缓冲数据）
#define     DOC_COE				65  //整定系数
#define		DOC_SYSRESET		66	//系统复位
#define		DOC_TRANSDATA		67	//透明数据
#define		DOC_DISTDIR			68	//录波目录

#define		DOC_CHANNELSRC		70	//通道源
#define		DOC_CHANNEL			71	//通道
#define		DOC_ZERODRIFT		72	//通道零漂
#define		DOC_ZEROOFFSET		73	//通道零点偏移
#define		DOC_FULLVALUE		74	//通道满值
#define		DOC_CFG				75	//配置字

#define		DOC_RELAYALARMITEM		76	//保护告警报告条目
#define		DOC_RELAYSTARTITEM		77	//保护启动报告条目
#define		DOC_RELAYACTIONITEM		78	//保护动作报告条目

#define		DOC_SYSCONFIG		80	//系统配置参数
#define		DOC_MAPBITOBJ		81	//镜像位对象
#define		DOC_BREAKERTRIPCNT	82	//断路器跳闸次数
#define		DOC_FLPARA			83	//测距参数

#define		DOC_CLASS1			84	// 一级数据
#define		DOC_CLASS2			85	// 二级数据

/*-----------------逻辑方程调试数据对象定义---------------*/
#define		DEBUG_RELAY_STEP1   90  //调试阶段1， 写值的
#define		DEBUG_RELAY_STEP2	91	//调试阶段2,  写值的
#define		DEBUG_RELAY_CONTEXT	92	//继电器调试逻辑方程上下文， 第一个数据是行号
//#define		DEBUG_RELAY_LINE	93	//调试继电器用， 当前行号及断点（已经运行了的行）
#define		DEBUG_RELAY_BP 93 //调试断点
#define		DEBUG_RELAY_STOP 94 //调试结束
#define		DEBUG_RELAY_WATCH 95 //读继电器值
#define		DEBUG_RELAY_DBOUT 96 //调试开出 
#define		DEBUG_RELAY_DBIN 97 //调试开入

/*-----------------------其它项目定义---------------------------------*/
#define		SELF_CLASS		4	//本站地址级别

#define		CLASS_1_ADDR_MASK	0x4000	//一级地址掩码
#define		CLASS_2_ADDR_MASK	0x1800	//二级地址掩码
#define		CLASS_3_ADDR_MASK	0x0380	//三级地址掩码
#define		CLASS_4_ADDR_MASK	0x003F	//四级地址掩码

#define		CLASS_1_ADDR_OFFSET		14	//一级地址偏移
#define		CLASS_2_ADDR_OFFSET		11	//二级地址偏移
#define		CLASS_3_ADDR_OFFSET		7	//三级地址偏移
#define		CLASS_4_ADDR_OFFSET		0	//四级地址偏移

#define		CLASS_1_ADDR_VALID	0x8000	//一级地址有效
#define		CLASS_2_ADDR_VALID	0x2000	//二级地址有效
#define		CLASS_3_ADDR_VALID	0x0400	//三级地址有效
#define		CLASS_4_ADDR_VALID	0x0040	//四级地址有效


#if (SELF_CLASS==1)
#define 	NET_ADDR_MASK		CLASS_1_ADDR_MASK	//本站地址掩码
#define 	NET_ADDR_VALID		CLASS_1_ADDR_VALID	//有效地址
#endif

#if (SELF_CLASS==2)
#define 	NET_ADDR_MASK		CLASS_2_ADDR_MASK
#define 	NET_ADDR_VALID		CLASS_2_ADDR_VALID	//有效地址
#endif

#if (SELF_CLASS==3)
#define 	NET_ADDR_MASK		CLASS_3_ADDR_MASK
#define 	NET_ADDR_VALID		CLASS_3_ADDR_VALID	//有效地址
#endif

#if (SELF_CLASS==4)
#define 	NET_ADDR_MASK		CLASS_4_ADDR_MASK
#define 	NET_ADDR_VALID		CLASS_4_ADDR_VALID	//有效地址
#endif

#define		BIT_FIR				0x80	//首包数据位
#define		BIT_FIN				0x40	//末包数据位
#define		BIT_CON				0x20	//需确认位
#define		TRAN_SEQ_MASK		0x3F	//传输层帧序列号掩码
#define		APP_SEQ_MASK		0x1F	//应用层帧序列号掩码
#define		APP_REQSEQ_MAX		15		//最大请求帧序号
#define		APP_RESSEQ_MIN		16		//最小响应帧序号
#define		APP_RESSEQ_MAX		31		//最大响应帧序号

#define		SYS_RESET			0x01	//系统复位
#define		HAVE_CLASS1			0x02	//有一级数据
#define		HAVE_CLASS2			0x04	//有二级数据
#define		NEED_CONFIG			0x08	//需网络配置
#define		STATUS_TIM_ERROR	0x10	//请求时钟
#define		STATUS_CTL_ERROR	0x20	//操作异常
#define		STATUS_CTL_BLOCK	0x40	//操作闭锁

#define		INDEX_SIZE_MASK		0x70	//指标规模掩码

#define		INDEX_SIZE_0		0		//无对象索引
#define		INDEX_8BIT			1		//对象前置1个字节索引
#define		INDEX_16BIT			2		//对象前置2个字节索引
#define		INDEX_32BIT			3		//对象前置4个字节索引
#define		SCALE_8BIT			4		//对象前置1个字节对象规模
#define		SCALE_16BIT			5		//对象前置2个字节对象规模
#define		SCALE_32BIT			6		//对象前置4个字节对象规模

#define		QUALIFIER_CODE_MASK	0x0F	//限定词码掩码

#define		QUALI_START_STOP_8	0		//对象段有8位的START和8位字节的STOP
#define		QUALI_START_STOP_16	1		//对象段有16位的START和16位的STOP
#define		QUALI_START_STOP_32	2		//对象段有32位的START和32位的STOP
#define		QUALI_NUM_8			3		//对象段有8位的对象个数
#define		QUALI_NUM_16		4		//对象段有16位的对象个数
#define		QUALI_NUM_32		5		//对象段有32位的对象个数
#define		QUALI_ADDR_8		6		//对象段有8位的对象绝对地址和个数
#define		QUALI_ADDR_16		7		//对象段有16位的对象绝对地址和个数
#define		QUALI_ADDR_32		8		//对象段有32位的对象绝对地址和个数
#define		QUALI_NORANGE		9		//无变程段。所指的是给定型式的全部数据对象

/*---------------------任务标志定义区----------------------------*/
//控制  0-15
#define 	SF_Time		  		(USER_SFLAG+0)    //需要对时
#define 	SF_HaveBO         	(USER_SFLAG+1)    //有遥控
#define 	SF_HaveBOReturn   	(USER_SFLAG+2)    //有遥控返校
#define 	SF_HaveTQ         	(USER_SFLAG+3)    //有同期
#define 	SF_HaveST         	(USER_SFLAG+4)    //有定值
#define 	SF_NewHaveTQ      	(USER_SFLAG+5)    //有new同期
#define 	SF_PROTECTRESET   	(USER_SFLAG+6)    //有保护复归
#define		SF_TIMING			(USER_SFLAG+7)	  //有对时请求

//状态 16-31
#define 	SF_NeedConfig       (USER_SFLAG+16)   //需要下发配置
#define 	SF_BURSTAI          (USER_SFLAG+17)   //变化遥测时间间隔到
#define 	SF_BURSTBI	    	(USER_SFLAG+18)   //未发送变位遥信时间间隔到
#define 	SF_AIREPORT         (USER_SFLAG+19)   //全遥测上报时间间隔到
#define 	SF_BIREPORT         (USER_SFLAG+20)   //全遥信上报时间间隔到
#define 	SF_CIREPORT         (USER_SFLAG+21)   //全遥测上报时间间隔到
#define 	SF_SOE		    	(USER_SFLAG+22)   //未发送SOE时间间隔到
#define 	SF_RELAYALARM		(USER_SFLAG+23)   //未发送保护告警事项时间间隔到
#define 	SF_RELAYSTART		(USER_SFLAG+24)   //未发送保护启动事项时间间隔到
#define 	SF_RELAYACTION		(USER_SFLAG+25)   //未发送保护动作事项时间间隔到
#define		SF_WAITCONFIRM		(USER_SFLAG+26)   //从站等待确认标志
#define		SF_NEEDCONFIRM		(USER_SFLAG+27)   //主站请求等待确认标志
#define 	SF_SAFEREPORT		(USER_SFLAG+28)	  //平安报文
#define		SF_MAKECOEECHO		(USER_SFLAG+29)	  //修改系数返回
#define 	SF_LEDREPORT        (USER_SFLAG+30)   //LED状态上报时间间隔到
#define 	SF_LEDBURST         (USER_SFLAG+31)   //LED变化状态

#define		SF_SENDRES_SET			(USER_SFLAG+32)	  //定值返回
#define		SF_SENDRES_SYSTEMSET	(USER_SFLAG+33)	  //辅助定值返回
#define		SF_SENDRES_SW			(USER_SFLAG+34)	  //软压板返回
#define		SF_SENDRES_CF			(USER_SFLAG+35)	  //系数返回

#define		SF_SENDRES_BIATTR		(USER_SFLAG+36)	  //遥信属性返回
#define		SF_SENDRES_BOATTR		(USER_SFLAG+37)	  //遥控属性返回
#define		SF_SENDRES_AOATTR		(USER_SFLAG+38)	  //遥调属性返回
#define		SF_SENDRES_MBOPARA		(USER_SFLAG+39)	  //镜像位对象参数返回

#define 	SF_AIE		    		(USER_SFLAG+40)   //未发送AIE时间间隔到
#define		SF_SENDRES_DISTDIR		(USER_SFLAG+41)	  //录波目录返回


#define		PORT_ID_CANA		((SC_PORT_CANBUS<<8)+1)
#define		PORT_ID_CANB		((SC_PORT_CANBUS<<8)+2)
#define		PORT_ID_ETHA		((SC_PORT_ETHERNETA<<8)+1)
#define		PORT_ID_ETHB		((SC_PORT_ETHERNETB<<8)+1)

/*-----------------------结构定义区------------------------------*/
//发送帧格式定义
typedef struct
{
	BYTE 	START;			//起始字节
	BYTE	LengthLo;		//帧长低字节
	BYTE	LengthHi;		//帧长高字节
	BYTE    DestAddrLO;		//目的地址
	BYTE    DestAddrHI;		//目的地址
	BYTE    SourceAddrLO;	//源地址
	BYTE    SourceAddrHI;	//源地址
	BYTE	TControl;		//传输层控制字节
	BYTE	APPControl;		//应用层控制字节
	BYTE	FunCode;		//功能码
	BYTE	Status;			//子站状态
	BYTE	ObjType;		//对象类型
	BYTE	ObjQulifier;	//对象限定词
	BYTE	Range;			//变程
}TTxdHead;

typedef struct
{
	BYTE 	START;			//起始字节
	BYTE	LengthLo;		//帧长低字节
	BYTE	LengthHi;		//帧长高字节
	BYTE    DestAddrLO;		//目的地址
	BYTE    DestAddrHI;		//目的地址
	BYTE    SourceAddrLO;	//目的地址
	BYTE    SourceAddrHI;	//目的地址
	BYTE	TControl;		//传输层控制字节
	BYTE	APPControl;		//应用层控制字节
	BYTE	FunCode;		//功能码
	BYTE	ObjType;		//对象类型
	BYTE	ObjQulifier;	//对象限定词
	BYTE	Range;			//变程
}TRxdHead;

//遥控结构
typedef struct
{
	WORD No;	 			//遥控路号
	WORD Command;			//动作 PBO_SELECT PBO_EXECUTE  PBO_CANCEL PBO_DIRECT
	WORD Attrib;			//状态（合分）PBO_NULL PBO_OPEN, PBO_CLOSE,
	WORD NeedRetFlag; 		//是否需要遥控返校信息标志
	BYTE Info[PBOINFOSIZE]; //保留，遥于保留特殊信息
	WORD RetResult;			//遥控返校状态 PBO_SUCESS  PBO_FAIL
}TCanBo;

//定值结构
typedef struct
{
	float *pSET;  			//定值数据区
	WORD AreaNo;			//定值命令
	WORD SETNo;				//定值序号
	WORD Off;   			//处理偏移量
}TCanST;

//网络配置信息
typedef struct
{
  	DWORD   	dwStatus;		//通道数据掩码
	WORD		wAIRepTM;		//遥测上报间隔
	WORD		wBIRepTM;		//遥信上报间隔
	WORD		wCIRepTM;		//电度上报间隔
	WORD		AIDeadArea;		//遥测死区范围(万分之)
	WORD		AIBurstRepTM;   //变化遥测上报
	WORD		BIBurstRepTM; 	//变化遥信上报
	WORD		SoeRepTM;		//SOE上报间隔
	WORD		TimingTM;       //对时间隔
	WORD		SaftyRepTM;		//平安报文
	WORD		wLEDStatusTM;	//LED状态
}TCanNetCfg;


#define	MAX_FILE_SECTIONSIZE		512	//文件段长
#define	FILE_STATUS_BEGINTRANS		1	

#define	FILE_START_READ				1
#define	FILE_STOP_READ				2
#define	FILE_START_WRITE			3
#define	FILE_STOP_WRITE				4
//文件结构
typedef struct
{
	BYTE 		*pData;  			//文件数据区
	char    	strFileName[32];	//文件名
	DWORD		dwFileLength;		//文件总长度
	WORD		bStatus;			//=0:就绪；1:已启动传输
	WORD 		nLastSection;		//上一桢文件段号
	WORD		nTotalSection;		//文件总段数
	WORD 		nPerSize;   		//每段文件长度
	WORD		wFileCrc;			//文件校验
	WORD		nTimer;				//文件传输定时器
}TCanFile;

//响应帧纪录
typedef struct
{
	BYTE	ResFunCode;		//响应功能码
	BYTE	ResSeq;			//响应帧序号
	BYTE	ObjType;		//对象类型
	BYTE	ObjNum;			//对象个数
}TResInfo;


//CAN数据定时器标识
enum CANDATA_TIMER
{
	AIBURST_ID=0,		//搜索变化遥测
	AI_ID,				//发送全遥测
	BI_ID,				//发送全遥信
	CI_ID,				//发送全电度
	BIBURST_ID,			//未发送成功的变化遥信发送间隔
	SOE_ID,				//未发送成功的SOE发送间隔
	RELAYALARM_ID,		//未发送成功的保护告警事项发送间隔
	RELAYSTART_ID,		//未发送成功的保护启动事项发送间隔
	RELAYACTION_ID,		//未发送成功的保护动作事项发送间隔
	BORET_ID,			//遥控返校
	SAFETY_ID,			//平安报文发送间隔
	SNINFO_ID,
	LED_ID,				//LED状态
	TIMING_ID, 			//对时
	TIMER_NUM			//定时器个数
};

// BI属性描述
typedef struct
{
	WORD	wControl;
	WORD	wFilterTime;
}TBIProPara;

// BO属性描述
typedef struct
{
	WORD	wLastTime;
}TBOProPara;

// AO属性描述
typedef struct
{
	WORD			wControl;			//控制字D0~D2：	
										//		=0:±5V;
										//		=1:±10V; 
										//		=2:20mA;
	WORD			wCHIndex;			//名称	索引号
	WORD			wRange;				//量程	
}TAOProPara;

#define DIST_FILENAME_LEN	16

typedef struct
{
	BYTE		nFileNum;	// 文件总个数
	BYTE		nSentCnt;	// 已发送文件个数
	char		strFileNameList[DIST_CAPACITY_MAX][DIST_FILENAME_LEN];//文件名字列表
	TSysTime	tFileTimeList[DIST_CAPACITY_MAX];//录波文件时间表
}TDistDirItem;

void	sCanBusTask(void);

class CSCanbusPro : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	BOOL Init(void);//初始化
	CSCanbusPro(DWORD *pdwApp):  CProtocol(pdwApp){};
			
protected:
	TRxdHead	*m_RxdHead;			//接收帧头
	TCanBo		m_Bo;				//遥控信息纪录
	TCanST		m_SET;				//定值信息纪录
	TCanFile	m_File;				//文件传送记录
	TCanNetCfg	m_NetCfg;			//网络配置信息
	TResInfo	m_ResInfo;			//响应信息，包括非请求响应
	TDistDirItem	m_DistDir;		//录波目录	

	WORD		m_nMaxFrameLen;		//最大帧长
	WORD		m_nBurstIdle;		//触发时间
	WORD		m_nRetryIdle;		//重发空闲
	
	WORD		m_wSAddr;			//模块地址				
	WORD		m_wDAddr;			//目的地址				
	WORD 		dwGBAddress;	    //广播报文的地址
	WORD 		m_wTimer[TIMER_NUM];//定时器
	DWORD  		m_dwTaskRunCount;  	//任务正常运行时间统计(秒)
	DWORD		m_NoRxdCounter;		//无接收数据计数
	BYTE		m_LastReqSeq;		//上次请求的SEQ
	BYTE		m_CurrentReqSeq;	//当前请求帧的SEQ
	BYTE		m_CurrentResSeq;	//当前响应帧的SEQ
	WORD		LastAIPtr;			//上次已发送全遥测指针
	WORD		LastBIPtr;			//上次已发送全遥信指针
	WORD		LastCIPtr;			//上次已发送全电度指针
	WORD		LastBurstAIPtr;		//上次已发送变化遥测指针
	WORD		m_WrapHeadPtr;		//当前事件头指针，维护软件用
	WORD		m_OptEventPtr;
	WORD		m_LogInfoPtr;
	WORD		m_wFileOffset;		//下装文件偏移量

	WORD		m_RetryAddr;		//重发地址
	WORD		m_RetryLen;			//重发长度
	WORD		m_RetryCount;		//重发计数
	
	WORD		m_wWatchPortID; 	//监视端口ID
	WORD		m_nWatchPortDog;	//端口监视看门狗

	BYTE		m_CommandSeq;		//待确认序号
	WORD        m_CommandAddr;		//戴确认地址
	WORD		m_FileTransTimer;	//文件传输定时器
	PBYTE		m_RetryBuf;			//重发缓冲区
	SWORD		*pOldAI;			//存放旧遥测的数据区指针
	void		*m_pMemBlock;		//预留内存块
	BYTE		*m_pLEDStatus;
	BYTE		*m_pLEDFreq;

	WORD		m_TransDataRP;		//透明数据读指针			
	WORD		m_TransReqPortID;	//请求方透传端口ID
	
	WORD	Get_MaxFrameLen();
	WORD 	Get_CommIdle();
	WORD 	Get_RetryIdle();
	
	BOOL 	IsNetTask(void);
	void 	TxdMonitor();
	
	DWORD 	SearchOneFrame(PBYTE Buf,WORD wLen);
	BYTE 	GetStationStatus();
	BYTE 	HaveClass1Data();
	WORD  	GetLayerAddr(WORD wAddr);
	void 	ClearFileTrans();

	void 	OnCommIdle(DWORD dwLen,DWORD PortID);
	void 	OnFrame(PBYTE Buf);
	void 	OnTimeOut(DWORD id);
	
	void 	OnSOE();
	void 	OnAIE();
	void 	OnRelayEvent(DWORD MsgCode, DWORD wParam);
	void 	OnBIBurst();
	void    OnRelayAlarm();
	void    OnRelayStart();
	void    OnRelayAction();
	void 	OnBOSelectEcho(WORD switchno,WORD attrib,WORD result);
	void 	OnBOExecEcho(WORD switchno,WORD Mode,WORD result);
	void 	OnBODirectEcho(WORD switchno,WORD Mode,WORD result);	
	void 	OnCOEMake(DWORD hPara,DWORD wPara,DWORD pPara,WORD lPara);
	void 	OnAutoAdjustEcho(DWORD dwResult,DWORD dwClass);
	void 	OnTransOIE(DWORD dwPort);

	void	AlterConfig(DWORD dwFlag);
	BOOL 	AIChange(long diff,long OldAI,WORD wAIDeadArea);

	void 	YKPocess();
	void 	IncResReq();
	void 	BackRetryFrame();
	BOOL 	ProcessRetryStatus();
	
	void 	TxdHead(BYTE AppCtrl,BYTE FunCode);
	void 	TxdTail(DWORD dwTxdFlag);
	
	void 	TxdConfirm();
	BOOL  	TxdAI();
	BOOL  	TxdBI();
	BOOL  	TxdCI();
	BOOL 	TxdBurstAI();
	BOOL 	TxdBurstBI();
	BOOL 	TxdSoe();
	BOOL 	TxdAIE();
	BOOL 	TxdRelayAlarmReport();
	BOOL 	TxdRelayStartReport();
	BOOL 	TxdRelayActionReport();
	BOOL 	TxdBOReturn();
	void	TxdIdleRep();
	BOOL 	TxdSafetyRep();


	BOOL 	RxdReadSETReq();
	BOOL 	RxdReadSystemSETReq();
	BOOL 	RxdReadSWReq();
	BOOL 	RxdReadCFReq();
	
	BOOL  	TxdAIRes();
	BOOL  	TxdAIWithQualityRes();
	BOOL  	TxdBIRes();
	BOOL  	TxdBIWithQualityRes();
	BOOL  	TxdCIRes();
	BOOL  	TxdCIWithQualityRes();
	BOOL	TxdSetCIValueRes();
	BOOL    TxdWriteSETRes();
	BOOL 	TxdWriteSETAreaRes();
	BOOL    TxdWriteSystemSETRes();
	BOOL    TxdWriteSWRes();
	BOOL 	TxdWriteCFRes();
	BOOL    TxdReadSETRes();
	BOOL 	TxdReadSETAreaRes();
	BOOL    TxdReadSystemSETRes();
	BOOL    TxdReadSWRes();
	BOOL 	TxdReadCFRes();
	
	BOOL  	TxdCoeRes();
	BOOL  	TxdAbsTimeRes();
	BOOL  	TxdLogInfoRes();


	WORD 	FillBurstBIEvent(BYTE *pBuf,TBurstBI *pEvnet);
	WORD  	FillSOEEvent(BYTE *pBuf,TEventBI *pEvnet);
	WORD 	FillOperateEvent(BYTE *pBuf,TMiscEvent *pEvnet);
	WORD 	FillRelayEvent(BYTE *pBuf,BYTE *pEvent,BOOL bItem);
	WORD 	FillInvalidEvent(BYTE *pBuf);
	BOOL 	TxdWrapAttrRes();

	BOOL  	TxdBIBByReadRes();
	BOOL  	TxdBIEByReadRes();
	BOOL  	TxdOptEventByReadRes();
	BOOL 	TxdRelayAlarmReportByReadRes(BOOL bList);
	BOOL 	TxdRelayStartReportByReadRes(BOOL bList);
	BOOL 	TxdRelayActionReportByReadRes(BOOL bList);

	BOOL  	TxdBIBByWriteRes();
	BOOL  	TxdBIEByWriteRes();
	BOOL  	TxdOptEventByWriteRes();
	BOOL 	TxdRelayAlarmReportByWriteRes(BOOL bList);
	BOOL 	TxdRelayStartReportByWriteRes(BOOL bList);
	BOOL 	TxdRelayActionReportByWriteRes(BOOL bList);

	BOOL  	TxdVarRes();
	BOOL	TxdWriteVarRes();

	BOOL 	TxdFileLoad();
	BOOL 	TxdFileDown();
	
	BOOL	RxdConfirm();
	void 	RxdNetCfg();
	BOOL  	RxdAbsTime();
	void  	RxdCoe();
	void    RxdFileDown();
//	BOOL 	RxdDDSetValue();

	BOOL 	TxdReadChannelAttrRes();
	BOOL 	TxdWriteChannelAttrRes();

	BOOL 	TxdReadChannelSrcRes();
	BOOL 	TxdWriteChannelSrcRes();

	BOOL 	TxdReadChannelValueRes();
	BOOL 	TxdWriteChannelValueRes();

	BOOL 	RxdReadBIAttrReq();
	BOOL 	TxdReadBIAttrRes();
	BOOL 	TxdWriteBIAttrRes();
	
	BOOL 	RxdReadBOAttrReq();
	BOOL 	TxdReadBOAttrRes();
	BOOL 	TxdWriteBOAttrRes();

	BOOL 	RxdReadAOAttrReq();
	BOOL 	TxdReadAOAttrRes();
	BOOL 	TxdWriteAOAttrRes();

	BOOL 	TxdReadSysParaRes();
	BOOL 	TxdWriteSysParaRes();
	BOOL 	TxdReadVersionRes();
	BOOL 	TxdReadDevStatusRes();

	BOOL 	TxdReadCFGRes();
	BOOL 	TxdWriteCFGRes();

	BOOL 	TxdRelayControlRes(BOOL bBlock);
	BOOL 	TxdSpecialOperateRes();

	BOOL 	TxdReadAOCoeRes();
	BOOL 	TxdWriteAOCoeRes();
	BOOL 	TxdSelectAOCoeRes();

	
	BOOL  	TxdLEDStatus(BOOL bBurst);
	BOOL  	TxdLEDStatusRes();
	BOOL 	TxdWatchPortRes();
	BOOL 	TxdVarDataRes();

	BOOL 	TxdSRMRes();
	BOOL 	TxdReadDevDispRes();

	BOOL	TxdReadRWStatusRes();
	BOOL	RxdWriteRWStatus();	

	BOOL 	RxdReadMBOReq();
	BOOL 	TxdReadMBORes();
	BOOL 	TxdWriteMBORes();

	BOOL 	TxdReadBreakerTripCntRes();
	BOOL 	RxdWriteBreakerTripCnt();

//-------------------------debugPLC----------------
	BOOL Rxd_DebugStep1Req(); 
	BOOL Txd_DebugStep1Res(); 
	BOOL Rxd_DebugStep2Req();
	BOOL Txd_DebugStep2Res();
	BOOL Rxd_DebugSetBreakPointReq(); //发送设置断点请求
	BOOL Txd_DebugSetBreakPointRes(); //接收到设置断点应答
	BOOL Rxd_DebugGetContextReq(); //读取上下文（包括行号和上下文继电器）
	BOOL Txd_DebugGetContextRes(); //接收到上下返回
	BOOL Rxd_DebugStopReq();//调试结束
	BOOL Txd_DebugStopRes();	
	BOOL Rxd_DebugWatchReq();//plc读取继电器
	BOOL Txd_DebugWatchRes();

	//读取调试输出继电器
	BOOL Rxd_GetDBOutReq();
	BOOL Txd_GetDBOutRes();
	//设置调试输入继电器
	BOOL Rxd_SetDBINReq();
	BOOL Txd_SetDBINRes(BYTE byCout, WORD wStart);
	
	BOOL RxdTransData();//接收透明数据
	BOOL TxdTransData();//发送透明数据

	BOOL TxdReadFLParaRes();
	BOOL TxdWriteFLParaRes();
	
	BOOL TxdReadDCSIDataRes();
	BOOL Rxd_SetAIValue(BYTE FunCode);
	BOOL Rxd_SetBIValue(BYTE FunCode);
	
	void ClearAllEvent();
	BOOL RxdReadDistDirReq();
	BOOL TxdReadDistDirRes();

	BOOL TxdReadClass1();
	BOOL TxdReadClass2();
};

#endif	
