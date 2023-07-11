/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Dbasedef.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义了系统数据库中所有的数据结构															*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*	TDBaseConfigTable						数据库描述表												*/
/*	TRunTimeAI								遥测实时数据定义											*/
/*	TRunTimeBI								遥信实时数据定义											*/
/*	TRunTimeCI								电度实时数据定义											*/
/*	TRunTimeBO								遥控实时数据定义											*/
/*	TRunTimeAO								遥调实时数据定义											*/
/*	TBurstBIWrap							遥信变位数据库												*/
/*	TBurstBI								遥信变位数据定义											*/
/*	TEventBIWrap							SOE数据库													*/
/*	TEventBI								SOE数据定义													*/
/*	TRunTimeRM								保护测量值数据定义											*/
/*	TRelayRecord							保护报文头定义												*/
/*	TRelayEvent								保护分报文定义												*/
/*	TRelayItem								保护动作条目定义											*/
/*	TDistData								保护录波文件描述定义										*/
/*	TRelayEventPtrList						保护事项目录表												*/
/*	TBasicEventPtrList						基本事项目录表												*/
/* 函数																									*/
/*																										*/
/*      None																							*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*      Paradef.h                        参数结构定义													*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
/********************************************************************************************************/
#ifndef __DBASEDEF_H
#define __DBASEDEF_H

#include "Paradef.h"	/*参数结构定义*/
#include "clink.h" 

/*定义数据掩码*/
/*定义数据掩码*/
#define MASK_ALL				0xffffffff
#define MASK_NULL				0x00000000

#define	MASK_AI					((DWORD)(0x01<<0))
#define MASK_BI					((DWORD)(0x01<<1))
#define MASK_CI					((DWORD)(0x01<<2))
#define MASK_BCD				((DWORD)(0x01<<3))
#define MASK_SPI				((DWORD)(0x01<<4))
#define MASK_DPI				((DWORD)(0x01<<5))
#define MASK_READ_SET			((DWORD)(0x01<<6))
#define MASK_READ_PARA			((DWORD)(0x01<<7))

#define MASK_AIE				((DWORD)(0x01<<8))
#define MASK_BIE				((DWORD)(0x01<<9))
#define MASK_SOE				((DWORD)(0x01<<10))
#define MASK_RELAYALARMEV		((DWORD)(0x01<<11))
#define MASK_RELAYSTARTEV		((DWORD)(0x01<<12))
#define MASK_RELAYACTIONEV		((DWORD)(0x01<<13))
#define MASK_DISTDATA			((DWORD)(0x01<<15))

#define MASK_AIFRZ				((DWORD)(0x01<<16))
#define MASK_CIFRZ				((DWORD)(0x01<<17))
#define MASK_BO					((DWORD)(0x01<<18))
#define MASK_AO					((DWORD)(0x01<<19))
#define MASK_SYNC				((DWORD)(0x01<<20))
#define MASK_WRITE_SET			((DWORD)(0x01<<21))
#define MASK_WRITE_PARA			((DWORD)(0x01<<22))

#define MASK_DEVSTATUS			((DWORD)(0x01<<24))
#define MASK_TIME				((DWORD)(0x01<<31))

/*定义基本文件类型*/
#define DB_FILE_PARACFG			0x01
#define DB_FILE_RELAYDISP		0x02
#define DB_FILE_RELAYDATA		0x03
#define DB_FILE_RELAYEV			0x04
#define DB_FILE_BASICEV			0x05

/*定义基本参数类型*/
#define DB_PARA_DAI				0x01
#define DB_PARA_DI				0x02
#define DB_PARA_AI				0x03
#define DB_PARA_BI				0x04
#define DB_PARA_CI				0x05
#define DB_PARA_BO				0x06
#define DB_PARA_AO				0x07
#define DB_PARA_SER				0x08
#define DB_PARA_CAN				0x09
#define DB_PARA_ETH				0x10
#define DB_PARA_SYS				0x11

#define DB_PARA_DIST			0x21
#define DB_PARA_CFG				0x22
#define DB_PARA_SET				0x23
#define DB_PARA_SYSTEMSET		0x24
#define DB_PARA_CF				0x25
#define DB_PARA_RM				0x26
#define DB_PARA_CHANNELSRC		0x27
#define DB_PARA_CHANNEL			0x28
#define DB_PARA_ZERODRIFT		0x29
#define DB_PARA_CHANNELFULLVALUE	0x2A
#define DB_PARA_MBO				0x2B
#define DB_PARA_FL				0x2C

//定义系统参数类型
#define DB_SYSPARA_ETHA			0x01
#define DB_SYSPARA_ETHB			0x02
#define DB_SYSPARA_ADDR			0x03
#define DB_SYSPARA_PASSWORD		0x04
#define DB_SYSPARA_SERPARA		0x05
#define DB_SYSPARA_CANPARA		0x06
#define DB_SYSPARA_ETHPARA		0x07
#define DB_SYSPARA_CIOPT		0x08
#define DB_SYSPARA_LANGUAGE		0x09
#define DB_SYSPARA_FDSHOW		0x0A
#define DB_SYSPARA_CTRLWORD		0x0B
#define DB_SYSPARA_IRGBTYPE		0x0C

/*定义基本存盘数据类型*/
#define DB_DATA_SET				0x01
#define DB_DATA_SETAREACUR		0x02
#define DB_DATA_SYSSET			0x03
#define DB_DATA_CF				0x04
#define DB_DATA_SW				0x05

/*定义基本存盘事件类型*/
#define DB_EVENT_RACT			0x00
#define DB_EVENT_RALM			0x01
#define DB_EVENT_RSTT			0x02
#define DB_EVENT_BIE			0x03
#define DB_EVENT_SOE			0x04
#define DB_EVENT_MISC			0x05
#define DB_EVENT_LOG			0x06
#define DB_EVENT_RELAYPTR		0x07
#define DB_EVENT_BASICPTR		0x08

//定义缺省数据个数
#define	DB_DEFAULT_DAINUM		10		
#define	DB_DEFAULT_DINUM		(34+13+10)		
#define	DB_DEFAULT_AINUM		4		
#define	DB_DEFAULT_BINUM		(34+13+10)		
#define	DB_DEFAULT_CINUM		2		
#define	DB_DEFAULT_AONUM		10		
#define	DB_DEFAULT_BONUM		20		

//实时遥测定义

#define AI_STATUS_ONLINE		((WORD)(0x01<<0))
#define AI_STATUS_RESET			((WORD)(0x01<<1))
#define AI_STATUS_COMBRK		((WORD)(0x01<<2))
#define AI_STATUS_MODIFY		((WORD)(0x01<<3))

typedef struct
{
	long				lValue;					//遥测值		
	WORD				wStatus;				//状态	D0=1在线		0	离线
												//		D1=1重启动		0	正常                
												//		D2=1通讯中断    0 	正常             
												//		D3=1强行修改    0 	正常
}TRunTimeAI;

//实时遥信定义

#define BI_STATUS_ONLINE		((BYTE)(0x01<<0))
#define BI_STATUS_RESET			((BYTE)(0x01<<1))
#define BI_STATUS_COMBRK		((BYTE)(0x01<<2))
#define BI_STATUS_MODIFY		((BYTE)(0x01<<3))

typedef struct
{
	BYTE				bStatus;				//遥信状态,最高位表示遥信的当前状态
												//D0=1在线 0 离线
												//D1=1重启动0 正常               
												//D2=1通讯中断 0 正常            
												//D3=1强行修改 0 正常           
												//D4~6 保留                       
												//D7当前状态
}TRunTimeBI;

//实时电度定义
#define CI_STATUS_ONLINE		((WORD)(0x01<<0))
#define CI_STATUS_RESET			((WORD)(0x01<<1))
#define CI_STATUS_COMBRK		((WORD)(0x01<<2))
#define CI_STATUS_MODIFY		((WORD)(0x01<<3))
#define CI_STATUS_OVERFLOW		((WORD)(0x01<<5))

typedef struct
{
	WORD				bStatus;				//状态	D0=1在线 0 离线
												//		D3=1强行修改 0 正常            
												//		D5=1 溢出 0 正常               
												//		D6~15 保留
	DWORD				dwValue;				//电度值	
}TRunTimeCI;

#define	TRunTimeBCD				void
#define TRunTimeSPI				void

//实时遥控定义

#define BO_STATUS_ONLINE		((WORD)(0x01<<0))
#define BO_STATUS_RESET			((WORD)(0x01<<1))
#define BO_STATUS_REMOT			((WORD)(0x01<<3))
#define BO_STATUS_LOCAL			((WORD)(0x01<<4))

#define BO_STATUS_INDSHIFT		5
#define BO_STATUS_INDMASK		((WORD)(0x07<<5))
#define GET_BO_STATUS(s)		(((s)&BO_STATUS_INDMASK)>>BO_STATUS_INDSHIFT)

#define BO_STATUS_READY			0
#define BO_STATUS_SELECT		1
#define BO_STATUS_SELSUCCESS	2
#define BO_STATUS_EXE			3
#define BO_STATUS_DIREXE		4

#define BO_RUNTIME_TIMEOUT		20

typedef struct
{
	WORD				wStatus;				//状态，D0=1 在线：控制该输出点的设备工作正常，对该点的二进制输出命令可以正确工作；=0 离线：设备工作不正常，对该点的二进制输出命令不能成功
												//D1=1 重启动；=0：正常
												//D3=1 远方控制。数字输出点已受控于源方设备，其当前状态一示于STATE位；=0正常
												//D4=1 当地控制。数字输出点已受控于本设备，其当前状态一示于STATE位
												//以下内容用D5~D7表示：	
												//	=0 遥控就绪
												//	=1 接受遥控预置，等待预置反较
												//	=2 预置成功，等待执行
												//	=3 接受遥控执行，等待执行反较
	WORD				wTimeOutCnt;			//超时计数											
	DWORD				dwAppID;				//源任务号,发出遥控的任务号
	TAbsTime			tLastExecTime;			//操作时间,上一次遥控操作时间
}TRunTimeBO;

#define	TRunTimeTQ				void

#define OPT_STATUS_READY			0
#define OPT_STATUS_SET				1
#define OPT_TIMEOUT_NUM				60

typedef struct
{
	WORD				wStatus;				//	=0 操作空闲
												//	=1 接收设置
												//	=2 设置确认
	WORD				wTimeCnt;				//超时计时，单位S											
	DWORD				dwOption1;				//选项1
	DWORD				dwOption2;				//选项2
	DWORD				dwAppID;				//源任务号,发出遥控的任务号
}TRunTimeOPT;

//实时遥调定义
#define AO_STATUS_ONLINE		((DWORD)(0x01<<0))
#define AO_STATUS_RESET			((DWORD)(0x01<<1))
#define AO_STATUS_REMOT			((DWORD)(0x01<<3))
#define AO_STATUS_LOCAL			((DWORD)(0x01<<4))

#define AO_STATUS_INDSHIFT		5
#define AO_STATUS_INDMASK		((DWORD)(0x07<<5))
#define GET_AO_STATUS(s)		(((s)&BO_STATUS_INDMASK)>>BO_STATUS_INDSHIFT)

#define AO_STATUS_READY			0
#define AO_STATUS_SELECT		1
#define AO_STATUS_SELSUCCESS	2
#define AO_STATUS_EXE			3

typedef struct
{
	WORD				wStatus;				//状态	D0=1 在线：控制该输出点的设备工作正常，对该点的遥调命令可以正确工作；=0 离线：设备工作不正常，对该点的遥调命令不能成功
												//		D2=1 重启动；=0：正常
												//		D3=1 远方强制数据。数字输出点已受控于源方设备，其当前状态一示于STATE位；=0正常
												//		D4=1 当地强制数据。数字输出点已受控于本设备，其当前状态一示于STATE位

												//以下内容用D5~D7表示：
												//	=0 遥调就绪
												//	=1 接受遥调预置，等待预置反较
												//	=2 预置成功，等待执行
												//	=3 接受遥调执行，等待执行反较
	DWORD				dwAppID;				//源任务号发出遥调的任务号
	TAbsTime			tLastExecTime;			//操作时间上一次遥调操作时间
}TRunTimeAO;

//保护测量值运行库
#define RM_STATUS_ONLINE		((DWORD)(0x01<<0))
#define RM_STATUS_MODIFY		((DWORD)(0x01<<3))

typedef struct
{
	long					lValue;						//测量值			
	WORD					wStatus;					//状态。D0=1离线        0 在线
}TRunTimeRM;											//		D3=1强行修改    0 	正常


#define MAX_BIE_NUM					1024	

//遥信状态定义
#define	DB_BI_OPEN					0
#define	DB_BI_CLOSE					1

#define BI_OPEN						DB_BI_OPEN
#define BI_CLOSE					DB_BI_CLOSE

//遥信变位事件定义
typedef struct
{
	WORD				wNo;					//遥信序号	
	BYTE				bStatus;				//变位后状态	0x00：分 0xFF：合
}TBurstBI;

//遥信变位事件库
typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TBurstBI				pEvent[MAX_BIE_NUM];		//事项缓存			
}TBurstBIWrap;

#define MAX_SOE_NUM			1024	

//SOE
typedef struct
{
	WORD					wNo;						//遥信序号	
	BYTE					bStatus;					//变位后状态,0x00：分 0xFF：合
	TAbsTime				stTime;						//发生时间,绝对时间
}TEventBI;

//SOE事件库
typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TEventBI				pEvent[MAX_SOE_NUM];		//事项缓存			
}TEventBIWrap;


#define MAX_AIE_NUM            4096 /*每个设备配置的遥测事件库环型缓冲区长度*/

/*----------------------------------------------------------------------------------------*/
/* 遥测事件定义														              */
/*----------------------------------------------------------------------------------------*/
typedef struct{
	WORD		nNo;					/*遥测事件对应的点号*/
	BYTE		bStatus;				/*遥测的状态*/
	long		lValue;					/*遥测值*/
	TAbsTime	stTime;					/*遥测事件的时标*/
}TEventAI;

/*----------------------------------------------------------------------------------------*/
/* 遥测事件环型缓冲区定义													              */
/*----------------------------------------------------------------------------------------*/
typedef struct{
	DWORD		dwMagic;				//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD		hSem;					//信号量ID,共享资源保护
	WORD		nMax;					//缓冲大小		
	WORD		nHeadIndex;				//头指针			
	WORD		nTailIndex;				//尾指针			
	TEventAI	pEvent[MAX_AIE_NUM];	/*事项缓冲区*/
}TEventAIWrap;

//录波数据
#define		MAX_DISTBLOCK_NUM				4			//最大故障录波数据块个数
#define		MAX_DISTBLOCK_LEN				(256*1024)	//最大故障录波数据块长度

#define		DIST_STATUS_IDLE				0x0000		//空闲
#define		DIST_STATUS_BUSY				0x55AA		//正在操作BUSY 59535542
#define		DIST_STATUS_SAVE				0x33CC		//准备存盘SAVE 45564153

typedef struct
{
	WORD		wStatus;			//  CACHE状态
	WORD		wPerSize;			//  数据单元长度
	DWORD		dwPrevSampleNum;	//	触发前采样点数
	DWORD		dwRearSampleNum;	//	触发后采样点数
	DWORD		dwMask;				//  通道掩码
	DWORD		dwSamplePtr;		//  采样缓冲指针
	TAbsTime	tTime;				//  触发时刻
}TDistHead;

//保护事项
#define		MAX_RECACHE_NUM					16			//最大并发保护动作CACHE空间个数
#define		MAX_SEGACT_NUM					32			//每个CACHE空间最大支持的分报文个数
#define 	MAX_RSTT_NUM					256			//保护启动报告最大保存个数
#define 	MAX_RALM_NUM					128			//保护告警报告最大保存个数
#define 	MAX_RACT_NUM					128			//保护动作报告最大保存个数
#define		MAX_RACT_LENGTH					512			//保护动作报告最大长度
#define		MAX_RSTT_LENGTH					304			//保护启动报告最大长度
#define		MAX_RALM_LENGTH					224	//64			//保护告警报告最大长度

//保护报文头
#define		RELAYEVENT_TYPE_ACTION		0
#define		RELAYEVENT_TYPE_ALARM		1
#define		RELAYEVENT_TYPE_START		2	

#define		RELAYEVENT_STATUS_MASK		0x0F	
#define		RELAYEVENT_STATUS_HAVEDIST	0x80	
#define		RELAYEVENT_STATUS_ACT		1	
#define		RELAYEVENT_STATUS_RLS		0	
		

//保护报文头
typedef struct
{
	WORD					nINF;						//INF						
	WORD					bStatus;					//状态。1：动作产生；0：动作复归
	WORD					FD_Num;						//故障电量个数		
	WORD					AbsTimeHi;					//发生时间。绝对时标
	DWORD					AbsTimeLo;					//发生时间。绝对时标
}TRelayEventHead;

typedef struct
{
//	DWORD					dwDevType;					//设备类型	
//	DWORD					dwRepType;					//报告类型。0:动作1:告警2:启动
	WORD					Num;						//分报文个数	
	WORD					wFaultNo;					//故障编号
	TRelayEventHead			tAction;					//分报文数据。可变长。
}TRelayRecordHead;

//定长保护分报文
typedef struct
{
	TRelayEventHead			stHead;
//	WORD					FD_Index[MAX_FD_NUM];		//故障电量索引,按照最大长度预留
	long					FD_Value[MAX_FD_NUM];		//故障电量数值,按照最大长度预留
}TRelayActionFix;


//并发保护报文CACHE空间结构(中间变量)

#define		RELAY_CACHE_HOLDTIME				300
#define		RELAY_CACHE_STATUSMASK				0x000F
#define		RELAY_CACHE_MAKESTART				((WORD)(0x01<<4))
#define		RELAY_CACHE_READY					0
#define		RELAY_CACHE_START					((WORD)(0x01<<0))
#define		RELAY_CACHE_ACTED					((WORD)(0x01<<1))
#define		RELAY_CACHE_END						((WORD)(0x01<<2))

typedef struct
{
	WORD					bStatus;					// D0~D3状态	0：就绪
														//		1：已记录过动作启动
														//		2：已记录过动作报告
														//		3: 动作结束
														// D5:需要记录启动报文
														//	其他无效
	WORD					nSeq;						//全局序号
	WORD					nCnt;						//分报文计数
	DWORD					dwTimeOut;					//最后发生时间绝对时标，记录最后一个分报文的时标，防止长时间不操作引起CACHE阻塞
	TRelayActionFix			pAction[MAX_SEGACT_NUM];	//定长报文数据	
}TRelayEventCache;


//保护程序填写分报文结构(中间变量)
typedef struct
{
	WORD					nINF;						//INF					
	WORD					bStatus;					//状态。1：动作产生；0：动作复归
	TAbsTime				stTime;						//发生时间。绝对时标
	long					FD_Value[MAX_FD_NUM];		//故障电量数值。定长，最大故障电量个数
}TRelayEventItem;

//保护启动事件
typedef union
{
	BYTE				aEvent[MAX_RSTT_LENGTH];
	TRelayRecordHead	tEvent;
}TRelayStartEv;

//保护告警事件
typedef union
{
	BYTE				aEvent[MAX_RALM_LENGTH];
	TRelayRecordHead	tEvent;
}TRelayAlarmEv;

typedef struct
{
	WORD				nIndex;							//状态位索引
	WORD				nAlarmINF;						//告警信息序号INF
	WORD				nRecoverINF;					//恢复信息序号INF
}TSCAlarmItem;

//保护动作事件
typedef union
{
	BYTE				aEvent[MAX_RACT_LENGTH];
	TRelayRecordHead	tEvent;
}TRelayActionEv;

typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TRelayStartEv			pEvent[MAX_RSTT_NUM];		//事项缓存			
}TRelayStartWrap;

typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TRelayAlarmEv			pEvent[MAX_RALM_NUM];		//事项缓存			
}TRelayAlarmWrap;

typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TRelayActionEv			pEvent[MAX_RACT_NUM];		//事项缓存			
}TRelayActionWrap;

//录波文件目录表
#define	DIST_INDEX_INIT			0x55AA
#define	DIST_INDEX_NAMELEN		32

typedef struct
{
	WORD					wMagic;						//初始化标志,有效标志WRAP_INIT_FLAG
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针	
    BYTE        Year;                   /*年*/
    BYTE        Month;                  /*月*/
    BYTE        Day;                    /*日*/
    BYTE        Hour;                   /*时*/
    BYTE        Minute;                 /*分*/
    BYTE        Second;                 /*秒*/
	BYTE		MSecondHi;				/*毫秒*/
	BYTE		MSecondLo;				/*毫秒*/
}TDistIndex;

//保护事件指针目录表
typedef struct
{
	WORD					nMaxRalayAction;			//保护动作事件存储容量	
	WORD					nRelayActionT;				//保护动作事件尾指针		
	WORD					nRelayActionH;				//保护动作事件头指针		
	WORD					nRelayActionLEDR;			//保护动作事件LED读指针	
	WORD					nRelayActionNet1R;			//保护动作事件NET1读指针	
	WORD					nRelayActionNet2R;			//保护动作事件NET2读指针	
	WORD					nMaxRalayAlarm;				//保护告警事件存储容量	
	WORD					nRelayAlarmT;				//保护告警事件尾指针		
	WORD					nRelayAlarmH;				//保护告警事件头指针		
	WORD					nRelayAlarmLEDR;			//保护告警事件LED读指针	
	WORD					nRelayAlarmNet1R;			//保护告警事件NET1读指针	
	WORD					nRelayAlarmNet2R;			//保护告警事件NET2读指针	
	WORD					nDistCnt;					//扰动数据计数器			
	WORD					Rsv[7];						//备份					
}TRelayEventPtrList;

//常规事件指针目录表
typedef struct
{
	WORD					nMaxSOE;					//SOE存储容量			
	WORD					nSOET;						//SOE尾指针			
	WORD					nSOEH;						//SOE头指针			
	WORD					nMaxOptRecord;				//操作记录存储容量	
	WORD					nOptRecordT;				//操作记录尾指针		
	WORD					nOptRecordH;				//操作记录头指针		
	WORD					nMaxRelaySart;				//保护启动报告存储容量
	WORD					nRelayStartT;				//保护启动报告尾指针	
	WORD					nRelayStartH;				//保护启动报告头指针	
	WORD					Rsv[7];						//备份				
}TBasicEventPtrList;


//文件镜像
#define	FILE_MAXLEN_NAME		16
#define	FILE_TYPE_PARACFG		0x01
#define	FILE_TYPE_PARADISP		0x02
#define	FILE_TYPE_RELAYDISP		0x03
#define	FILE_TYPE_RELAYDATA		0x04

#define FILEMAP_FLAGS_INIT			((WORD)(0x01<<0))
#define FILEMAP_FLAGS_MODIFY		((WORD)(0x01<<1))
#define FILEMAP_FLAGS_MODEND		((WORD)(0x01<<2))

typedef struct
{
	char					sFileName[FILE_MAXLEN_NAME];				//文件名,如录波数据文件可以描述为DISTxxxx，其中xxxx代表录波数据故障点号
	WORD					wFileType;					//文件类型			
	WORD					wFlag;						//文件标志。D0：=1 文件初始化
														//			D1：=1 文件改变
														//			D3：=1 结束文件操作
														//			其他备用
	void					*pRamData;					//RAM数据指针。镜像区中的文件地址
	DWORD					dwFlashOff;					//Flash文件偏移。FLASH区中的文件偏移（文件系统用不上）
	DWORD					nSize;						//文件长度			
	TAbsTime				absTime;					//存盘时间。上次文件存盘时间
	WORD					wCheck;						//校验码。累加和校验
}TFileMap;

//在线修改参数指针
typedef struct
{
	TParaConfigTable		*pConfig;
	TDAIPara				*pDAIAttr;					//模拟采样点属性			
	TAIPara					*pAIAttr;					//遥测属性				
	TDIPara					*pDIAttr;					//数字采样点属性			
	TBIPara					*pBIAttr;					//遥信属性				
	TCIPara					*pCIAttr;					//电度属性				
	TBOPara					*pBOAttr;					//遥控属性				
	TAOPara					*pAOAttr;					//遥调属性				
	TSerialPara				*pSerial;					//串口参数					
	TCanBusPara				*pCANBus;					//CAN参数						
	TEthPortPara			*pEthernet;					//以太网参数	
	TMBO_Disp				*pMBODisp;					//镜像位对象描述表
	TRelayDataTable			*pRelayCfg;					//保护配置表
	TFL_Disp				*pFLDisp;					//测距对象描述表
	DWORD					*pSWData;					//软压板数据指针		
	DWORD					*pCFGData;					//配置字数据指针		
	float					*pSETData;					//保护定值数据指针	
	float					*pSystemSETData;			//辅助定值数据指针	
	long					*pCFData;					//系数数据指针		
}TMapDataList;

//数据库配置表
#define		DB_MAXLEN_PASSWORD	8		
#define		DB_CONTROL_BO		((DWORD)(0x01<<0))
#define		DB_CONTROL_AO		((DWORD)(0x01<<1))
#define		DB_CONTROL_TIME		((DWORD)(0x01<<2))
#define		DB_CONTROL_AICAL	((DWORD)(0x01<<3))
#define		DB_CONTROL_CICAL	((DWORD)(0x01<<4))
#define		DB_CONTROL_EVREP	((DWORD)(0x01<<5))
#define		DB_CONTROL_MISC		((DWORD)(0x01<<7))

#define		DB_RELAY_FDSECOND	((DWORD)(0x01<<8))		//故障电量按照二次数据存储
#define		DB_RELAY_CACLINT	((DWORD)(0x01<<9))		//是否计算积分电度
#define		DB_IRGB_DK3511		((DWORD)(0x01<<10))		//GPS时钟源为DK3511

#define		SET_AREANO_INVALID	0xAAAA

typedef struct
{
	DWORD				dwDevID;				//设备标识	
	char				chHostName[HOST_NAME_LEN];//主机名		
	WORD				wAddress;				//本设备地址				
	WORD				wMainAddr;				//上位机地址				
	WORD				wType;					//模块类型	
	WORD				wCodeCrc;				//程序CRC码
	char				chPassWord1[8];			//密码1					
	char				chPassWord2[8];			//密码2					
	DWORD				dwControl;				//控制字。	D0=1遥控允许
												//			D1=1 遥调允许
												//			D2=1 对钟允许
												//			D3=1 遥测上传乘系数
												//			D4=1 电度上传乘系数
												//			D5=1 异常数据主动上报
												//			D6=1 电度定时上发主站
												//			D7=1 是否产生操作事件
												//			D8=1 故障电量按照一次值存储
												//			D9=1 是否计算积分电度
												//			D10=1 GPS时钟源为DK3511，否则为KF6511
	WORD				nDAI;					//直接模拟采样点个数		
	WORD				nDI;					//直接数字采样点个数		
	WORD				nAI;					//遥测个数				
	WORD				nBI;					//遥信个数				
	WORD				nCI;					//电度个数				
	WORD				nSPI;					//SPI个数					
	WORD				nBCD;					//BCD个数					
	WORD				nBO;					//遥控个数				
	WORD				nAO;					//遥调个数				
//	WORD				nAIType;				//遥测类型数				
	WORD				nSerNum;				//串口个数				
	WORD				nCanNum;				//CAN网端口个数			
	WORD				nEthNum;				//以太网端口个数			
	WORD				nSW;					//软压板个数		
	WORD				nCFG;					//配置字个数		
	WORD				nSET;					//定值个数		
	WORD				nSETArea;				//定值区个数		
	WORD				nSETAreaCur;			//当前定值区号		
	WORD				nSETAreaCurB;			//当前定值区号反码	
	WORD				nSystemSET;				//辅助定值个数	
	WORD				nRT;					//变比个数		
	WORD				nCF;					//系数个数		
	WORD				nRM;					//保护测量值个数	
	WORD				nRE;					//保护动作个数	
	WORD				nRW;					//继电器字个数	
	WORD				nTaskAPP;				//数据库任务计数			
	WORD				nDataName;				//数据名称总个数			
	WORD				nTC;					//传动个数
	WORD				nLanguageNo;			//语言编号
	WORD				nMBO;					//镜像位对象个数
	WORD				nFL;					//测距对象个数
	TRunTimeAI*			pAI;					//遥测实时数据指针		
	TRunTimeRM*			pRM;					//保护测量值数据指针		
	TRunTimeBI*			pBI;					//遥信实时数据指针		
	TRunTimeCI*			pCI;					//电度实时数据指针，NvRAM存储
	TRunTimeBCD*		pBCD;					//BCD实时数据指针			
	TRunTimeSPI*		pSPI;					//SPI实时数据指针			
	TRunTimeAO*			pAO;					//遥调实时数据指针		
	TRunTimeBO*			pBO;					//遥控实时数据指针		
	TRunTimeTQ*			pTQ;					//同期实时数据指针		
	TRunTimeOPT*		pParaOpt;				//参数操作记录		
	TRunTimeOPT*		pSWOpt;					//软压板操作记录		
	TRunTimeOPT*		pCFGOpt;				//配置字操作记录	
	TRunTimeOPT*		pSETOpt;				//保护定值操作记录		
	TRunTimeOPT*		pSETAreaOpt;			//保护定值区操作记录		
	TRunTimeOPT*		pCFOpt;					//系数操作记录		
	DWORD*				pSWData;				//软压板数据指针			
	DWORD*				pCFGData;				//配置字数据指针			
	long*				pSETDataFix;			//保护定值数据指针		
	float*				pSETDataFloat;			//保护定值数据指针		
	float*				pSystemSETData;			//辅助定值数据指针		
	long*				pCFData;				//系数数据指针			
	TBurstBIWrap*		pBurstBIWrap;			//遥信变位缓冲指针，NvRAM存储
	TEventBIWrap*		pEventBIWrap;			//遥信事项缓冲指针，NvRAM存储
	TRelayStartWrap*	pRSTTWrap;				//保护启动事件缓冲指针		
	TRelayAlarmWrap*	pRALMWrap;				//保护告警事件缓冲指针		
	TRelayActionWrap*	pRACTWrap;				//保护动作事件缓冲指针		
	TDAIPara*			pDAIPara;				//模拟量采样点参数			
	TDIPara*			pDIPara;				//数字采样点参数
	TAIPara*			pAIAttr;				//遥测属性				
	TBIPara*			pBIAttr;				//遥信属性				
	TCIPara*			pCIAttr;				//电度属性				
	TBOPara*			pBOAttr;				//遥控属性				
	TAOPara*			pAOAttr;				//遥调属性				
	TSW_Disp*			pSWDisp;				//软压板描述指针			
	TCFG_Disp*			pCFGDisp;				//配置字描述指针			
	TSET_Disp*			pSETDisp;				//保护定值描述指针		
	TSysSET_Disp*		pSystemSETDisp;			//辅助定值描述指针		
	TCF_Disp*			pCFDisp;				//系数描述指针			
	TRM_Disp*			pRMDisp;				//故障电量描述指针			
	TRE_Disp*			pREDisp;				//保护事件描述指针			
	TDist_Disp*			pDistPara;				//录波参数
	TAC_Disp*			pACPara;				//录波模拟通道参数
	TDC_Disp*			pDCPara;				//录波数字通道参数
	char*				pDataName;				//数据名称表
	TSerialPara*		pSerial;				//串口参数				
	TCanBusPara*		pCAN;					//CAN参数					
	TEthPortPara*		pEthernet;				//以太网参数
	TFileMap*			pBasicParaFileMap;		//常规参数文件镜像区指针	
	TFileMap*			pRelayDispFileMap;		//保护描述文件镜像区指针	
	TFileMap*			pRelayDataFileMap;		//保护数据文件镜像区指针	
	TFileMap*			pBasicEventFileMap;		//基本事项文件镜像区指针	
	TFileMap*			pRelayEventFileMap;		//保护事项文件镜像区指针
	TMapDataList		tMapData;				//镜像数据区	
	TMapDataList		tModData;				//在线修改数据区			
	TTC_Disp*			pTCDisp;				//传动参数区
	TMBO_Disp*			pMBODisp;				//镜像位对象参数区
	TFL_Disp*			pFLDisp;				//测距对象参数区
	TEventAIWrap*		pEventAIWrap;			//遥测事项缓冲指针，RAM存储
	void*				pRsv[7];				//备用。预留7个指针
}TDBaseConfigTable;


#define VER_INF_LEN			16		//版本信息长度(软件)
#define HW_VER_INF_LEN		8		//硬件版本信息
#define USER_INF_LEN		126		//用户描述信息长度
#define CODE_FILE_BUF_LEN	20000	//文件读取缓存长度

//应用程序版本信息
typedef struct
{
	WORD wCheckSum;					//应用程序文件校验码，16位累加和,内容从
	TSysTime CompileTime;			//文件创建时间,为初始编译时时间
	TSysTime EditTime;				//文件处理时间，即新文件生成时间
	char SoftWareVer[VER_INF_LEN];	//软件版本信息，字符串，内容如:10.2.20
	char HardWareVer[HW_VER_INF_LEN];	//硬件版本信息，字符串，内容如:10.2.20
	char UserInf[USER_INF_LEN];		//本程序对应的用户描述信息，内容如:北京15号线地铁项目，vxwroks操作系统，586CPU，16路串口
	DWORD Res[8];					//备用,第一个为CRC校验长度(包括文件开始的16个字节)
}TCodeFileInf;

// 串口规约参数
typedef struct
{
	BYTE		byNo;		//串口编号
	BYTE		byMode;		//串口控制模式										
	BYTE		byDriver;	//串口规约编号
	BYTE		byBaud; 	//串口波特率
}TSerialConfig;

// CANBUS规约参数
typedef struct
{
	BYTE		byNo;		//CAN口编号
	BYTE		byDriver;	//规约编号
	WORD		wBaud; 		//通信速率
}TCanbusConfig;

// 以太网连接规约参数
typedef struct
{
	BYTE		byNetCardNo;//网卡编号
	BYTE		byLinkNo;	//连接编号
	BYTE		byDriver;	//规约编号，同上
	BYTE		byMode;		//控制模式,
	DWORD		dwSideIP;	//对端IP地址
	WORD		wPort;		//端口号
}TEthLinkConfig;

// 遥控注册表节点
typedef struct
{
	DWORD	hTaskID;
	WORD	nBO;
}TBORegisterInfo;

#endif
