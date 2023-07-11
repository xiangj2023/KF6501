/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			Logic.h
* 
*   软件模块：可编程逻辑方程
*
* 
*	描述	定义可编程逻辑方程外部接口函数
* 
* 
* 
*   函数	ResetAllRelayWord	复位所有继电器
*			ReadRelayWord	读继电器值 返回0或1
*			WriteRelayWord	写继电器 						
*			ClearKeepRelayWord	清空自保持继电器，序号为CLEAR_ALL_KEEP_RELAY_WORD是清空所有的
*			SetTimeRelayWord	设置时间继电器字的启动和返回时间
*			RunLogicParseModule	解释执行逻辑方程, 执行一遍
*			RecordRelayWord	记录继电器值 参数word[],byte[]. 其中 word[0]存放要记录值得继电器个数，后面依次是继电器序号					
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-12-29		初始创建
* 
*************************************************************************/

#ifndef __Logic_H
#define __Logic_H
#include "system.h"
#include "logicdef.h"
#include "relaydef.h" 

#define PLC_DEBUG_ST1 0x55AAAA55
#define PLC_DEBUG_ST2 0xEB9090EB

#ifdef __cplusplus
extern "C"{
#endif


// FuncType 表
//@TypeID	TypeName	TypeLabel	Describe
//@99	跳转继电器	JMP	跳转函数， 参数为标号或行号
#define			JMP		99	///跳转继电器、 100以后为逻辑方程系统函数
#define		SYSFUN_TYPE_BASE		100
//#define		LEDFLASH_FUN		109
#define		MAX_SYSFUN_TYPE		199 //最大系统函数类型， 200及以后为用户自定义函数类型
#define		USERFUN_TYPE_BASE	200

//ExpType 表
//@	5	脉冲继电器	Pulse	
#define			PULSE	5	//脉冲继电器 ，参数为脉冲常数
#define			GE		8	//比较继电器， 参数为保护测量值，立即数
#define			CONSTTIME 9

//RelayWordType 表
#define RW_TYPE_COUNTER	28

//pfnSysFun函数  参数1，参数2， 写入值
typedef void (*USER_FUN_RELAY_WORD_PTR)(WORD, WORD,BOOL);
extern LONG g_PLCStatus;
extern BYTE byRelayWordValue[];
extern DWORD g_dwPLCDebugStep1, g_dwPLCDebugStep2, g_dwPLCCurLine,g_dwPLCBreakPoint;

#ifdef __PL_WIN_SIMULATION__
#define PLC_ONESTEP (WM_USER+1)
#endif

//逻辑方程状态
#define PLC_SYS_OK  1
#define PLC_DEFAULT_OK 2
#define PLC_STATUS_ERR 0

//@ 逻辑方程参数文件头部定义 
typedef struct _TLogicPara_Head{
	BYTE byFileFlag[8];			// 文件标志 
	DWORD dwFileLen;			// 文件长度 
	DWORD dwRelayWordNum;		// 继电器字个数 
	DWORD dwExpressionNum;		// 表达式个数 
	DWORD dwCalcNodeNum;		// 执行节点个数 
}kTLogicPara_Head;

typedef struct _TCommonFileHead
{
	DWORD dwSize; //文件总长度
	DWORD dwVersion; //所用数据库版本号
//	WORD wFlag; //预留 2012-3-2 去掉wFlag，空间归到wVersion变成DWORD
	DWORD dwMagic; //预留
	TSysTime sysTime; //生成时间
	DWORD dwCheck; //校验码
}TCommonFileHead;


//@ 表达式描述结构 
typedef struct _TLogicPara_Expression{
	WORD wResultRelayWordIndex;	// 输出继电器字序号 
	WORD wResultRelayWordType;	// 输出继电器字执行类型 
	WORD wParm; //参数  
	WORD wParm2; // 参数2 2009-7-13 15:32:30 增加， 

	WORD wStartCalcNodeIndex;		//起始执行节点索引号
	WORD wCalcNodeNum;			// 执行节点个数
	
	WORD wParaEx; //扩展参数 2012-4-28 修改增加的
	WORD wParaEx2; //扩展参数2 2012-4-28 修改增加的
}kTLogicPara_Expression;

//@ 执行节点描述结构 
typedef struct _TLogicPara_CalcNode{
	WORD wRelayIndex;				// 继电器字索引号 
	WORD wOffset;					// 跳转偏移 
	WORD wCalcType;				// 运算方式 
	WORD wGetType;				// 提取方式 
}kTLogicPara_CalcNode;

// 时间继电器字额外数据描述结构 
typedef struct TTimeRelayWordInf{
    DWORD dwState;                     // 继电器字状态
    DWORD dwTime;                      // 纪录当前时间,用于判断启动或返回 
}TTimeRelayWordInf;

typedef struct TPulseRelayWordInf{
	BOOL bIsRun; //脉冲继电器是否运行
	DWORD dwTime; //运行定时器
	DWORD dwTimeParam; //波形定值
}TPulseRelayWordInf;

typedef struct TGERelayWordInf{
	LONG* npParm1;
	LONG* npParm2;
}TGERelayWordInf;

// 复位(清)所有的继电器字
void ResetAllRelayWord(void);

// 读继电器字 
//BOOL ReadRelayWord(WORD wRelayWordIndex);
#define ReadRelayWord(index)\
	(((byRelayWordValue[index] & 0x01) == 0x01) ? TRUE : FALSE)


// 写继电器字（按常规方式写） 
void WriteRelayWord(WORD wRelayWordIndex, BOOL bContext);


// 清自保持继电器字 
#define CLEAR_ALL_KEEP_RELAY_WORD	((WORD)-1)		// 清全部自保持继电器字 
#define LOGIC_READ_ALL_VALUE ((WORD)-1)  //读所有值
void ResetHoldRelayWord(WORD wRelayWordIndex, WORD wRs=0);

// 清时间继电器字
void ResetTimeRelayWord(WORD wRelayWordIndex, WORD wRsv=0);

//清跳闸继电器字
void ResetTripRelayWord(WORD wRelayWordIndex, WORD wRsv=0);

//清不掉电自保持继电器
void ResetNVRamHoldRelayword(WORD wRelayWordIndex, WORD wRsv=0);

//信号继电器字
void ResetSignalRelayWord(WORD wRelayWordIndex, WORD wRsv=0);

//传动
void CtrlTest(WORD wNo, BOOL  bIsStart);

//复归计数继电器
void ResetCount(WORD wRelayWordIndex, WORD wRsv=0);
void IncreaseCount(WORD wRelayWordIndex, WORD wOffset);
void DecreaseCount(WORD wRelayWordIndex, WORD wOffset);
//按继电器序号读计数值
WORD ReadCountByIndex(WORD wRelayWordIndex);
//按计数继电器号读计数值
WORD ReadCountByNo(WORD CountNo);

//功能：逻辑方程处理,外部运行逻辑方程时循环调用该函数
void LogicProcess(void);


// 初始化可编程逻辑模块, 只调用一次， 分配存储空间
BOOL InitLogicParseModule();

// 复位可编程逻辑模块,在逻辑方程参数下载后调用
BOOL ResetLogicParseModule(BOOL bIsNewPLCDownLoad = FALSE);

//读逻辑方程数据库版本
DWORD GetPLCDBVer();
//读逻辑方程通用文件校验码
WORD GetPLCCheckSum();


/****************************************************************************************
 * 
 * 功能描述: //记录继电器值
 * 
 * 参数说明: 
 *			- WORD* pwRelayIndexList
 *			  类型: 
 *			  说明: 
 *
 *			- WORD* pDst
 *			  类型: 
 *			  说明:  继电器序号列表 格式 （总继电器个数n， 继电器1序号， 继电器2序号,..继电器n序号
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
BOOL RecordRelayWord(WORD* pwRelayIndexList, LONG* pDst);

//刷新输入继电器（读遥信到继电器字）
void UpDateInPut();

//刷新出口继电器（写出口继电器到出口）
void UpDateOutPut();

//刷新软压板（读软压板到继电器字）
void UpDateSW();

//刷新配置（读配置到继电器字)
void UpDateCFG();

//更新led继电器字
void UpDateLed();

//更新调试开入继电器 (从规约缓冲区更新到继电器）2012-02-08
void UpDataDeBugInRW();

//更新调试开出继电器 (从继电器更新到规约缓冲区）2012-02-08
void UpDataDeBugOutRW();

//读所有虚遥信
STATUS ReadAllVirDI(BYTE *pBuf);

//遥控预置
void RemoteCtrlPre(WORD wRCNum, BOOL bContext);

//读反校继电器
BOOL RemoteCtrlVer(WORD wRCNum );

//写遥控执行继电器
void RemoteCtrlExe(WORD wRCNum, BOOL bContext);

//2012-02-13 当地控制执行
void LocalCtrlExe(WORD wLCNum, BOOL bContext);

//报告
void Report_Start(WORD wSeqID,WORD wReportIndex);

void Report_StartByInf(WORD wSeqID, WORD wInf);

void Report_Action(WORD wSeqID,WORD wReportIndex);

void Report_End(WORD wSeqID, WORD wRsv=0);

void Report_Alarm(WORD wReportIndex, WORD wRsv=0);


/************************************************************
	功能：读取闪烁led的值
	参数：
	     WORD wIndex[in] 要读的led序号， 为LOGIC_READ_ALL_VALUE时读取所有的led
	     BYTE* pByFrequency[out] 返回led频率
	     BYTE* pByValue[out] 返回led值
	返回：LONG  0:读取成功 1:读取失败
************************************************************/
LONG GetFlashLedValue(WORD wIndex, BYTE* pByFrequency, BYTE* pByValue);


/************************************************************
	功能：读取发送链接继电器的值
	参数：
	     WORD wStartIndex[in] 要读的发送链接继电器起始序号
	     WORD wCount[in] 要读的发送链接继电器个数
	     
	     BYTE* pByValue[out] 指向返回值的指针
	返回：LONG  0:读取成功 1:读取失败
************************************************************/
LONG GetTxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue);
/************************************************************
	功能：设置接收链接继电器的值
	参数：
	     WORD wStartIndex[in] 要写的接收链接继电器起始序号
	     WORD wCount[in] 要写的接收链接继电器个数
	     
	     BYTE* pByValue[in] 指向设置值的指针
	返回：LONG  0:读取成功 1:读取失败
************************************************************/

LONG SetRxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue);

/************************************************************
	功能：设置调试继电器的值
	参数：
	     WORD wIndex[in] 要写的继电器的序号，
	     
	     BYTE byVal[in] 值
************************************************************/
void WriteDebugRW(WORD wIndex,BYTE byVal);

/************************************************************
	功能：读继电器全值
	参数：
	     WORD wIndex[in] 要读的继电器的序号，
	     
	返回:    BYTE  继电器值
************************************************************/
BYTE ReadRWFullValue(WORD wIndex);

/************************************************************
	功能：读逻辑方程当前行号
	参数：
	     
	     
	返回:    DWORD  当前正在执行的逻辑方程的行号
************************************************************/
const WORD GetPLCCurLine(); 
const WORD GetPLCDebugBreakPoint();
void SetPLCDebugBreakPoint(DWORD dwVal);
void SetPLCDebugStep1(DWORD dwVal);
void SetPLCDebugStep2(DWORD dwVal);
void StopPLCDebug();
void SetPLCDebugLoop(DWORD dwVal);
void SetPLCDebugOneStep(DWORD dwVal);

/************************************************************
	功能：写调试开入的值
	参数：
	     WORD wDebugInIndex[in] 要写的调试开入的序号
	     
	     BYTE byVal[in] 值
************************************************************/
void WriteDebugIn(WORD wDebugInIndex,BYTE byVal);

/************************************************************
	功能：读调试开出的值
	参数：
	     WORD wDebugOutIndex[in] 要读的调试开出的序号
	     
	返回值     BYTE 值
************************************************************/
BYTE ReadDebugOut(WORD wDebugOutIndex);

/*******************/

#ifdef __cplusplus
}
#endif

#endif /* __Logic_H */
