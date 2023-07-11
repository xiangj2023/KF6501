/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			Logic.c
* 
*   软件模块：可编程逻辑方程
*
* 
*	描述	实现可编程逻辑方程模块
*
* 	关于继电器沿操作的几点说明:
*		沿正确有效有两个关键因素
*		1)继电器写函数在写新值之前把旧值保存到高位， 本条只有脉冲继电器不符合，其他继电器都保存高位了
*		2)逻辑方程的每个刷新周期内继电器值都刷新。
*		  逻辑运行自动刷新=左边的继电器;自动刷新开入和链接继电器
* 		  所以除开入和链接外，其他不在=左边出现的继电器的沿操作都是不支持的，除非每周期额外调用写函数刷新(不管值是否变化)
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	28/11/2008		初始创建
* 		kagnqian 	21/04/2010		1.常数时间继电器及软压板操作函数的实现
* 									2.修改系统函数接口RELAY_SYSFUN_PTR的定义，
* 									 从1个参数改为2个参数， 报告函数及软压板操作函数用2个参数
*		kangqian	12/10/2010	1.增加格式化信号继电器eeprom的函数FormatSignalEEprom
*								2.信号继电器保存到eeprom修改为保存正反值2份
*								3.读取逻辑方程文件先读文件头， 得到文件大小再读逻辑到内存
*		kangqian	06/16/2011	1.闪灯函数，接收链接继电器，发送链接继电器
*		kangqian	09/15/2011	1.增加nvram中间继电器和自保持继电器
*		kangqian	12/13/2011	1.增加调试继电器相关内容，主要是DebugRunLogic函数
*		kangqian 	12/20/2011  1.修改ResetLogicParseModule内容，在下载了新逻辑.plc文件时调用
*								   该函数可以使新逻辑立即投入使用，不需要重启
*		kangqian 	02/08/2012	1.增加关于调试开入DBIn及调试开出DBOut的处理部分
*		kangqian	02/13/2012	1.增加一种当地控制继电器LocalCtrlExe
*		kangqian 	03/05/2012	1.把TCommonFileHead中的wVersion改为DWORD,原来的wFlag去掉，相应修改GetPLCDBVer()
*		kangqian	03/27/2012	1.增加计数继电器的相关函数，递增，递减，复归，修改比较继电器使比较参数也可以是计数继电器
*	参数文件结构变化的重大修改	2.修改kTLogicPara_Expression数据结构，增加扩展参数paraex, paraex1，相应修改默认逻辑等
*								3.比较继电器修改，比较内容可以是常数，保护测量值，定值，辅助定值，计数器
*		kangqian	05/22/2012	1.增加OUTPUT_NVRAM标记， 选择是否把开出继电器保存到nvram中
*		kangqian	05/25/2012  1.增加关于plc参数版本是否兼容的判断，增加关于数据库版本的判断 _CheckFileFlage
*		kangqian	07/30/2012	1._OUTPUT_NVRAM_定义反了，SYS_CPU_TYPE==CPU_COLDFIRE中应该为false
								2.InitNVRamRelyWord中(g_pByNVRam[2*i]^g_pByNVRam[2*i+1]) == 0是错误的，相反的字节异或后结果为255
								3.信号继要写eeprom,在数据库任务中保存的，保存的是0x00,0x01, 或者0x01,0x00,
									和其他nvram中保存的继电器保存值和取反值不同	，判断需要判bBuf[2*i]^bBuf[2*i+1]==0x01							
		kangqian	07/312012	1.ResetLogicParseModule(BOOL bIsNewPLCDownLoad = FALSE),增加参数bIsNewPLCDownLoad，以区分是系统初始化
								时装置逻辑，还是运行中下载新逻辑，对于eeprom及nvram中保存的继电器，下载新逻辑要把里面的值都清掉
		kangqian	12/31/2014  1.InitNVRamRelyWord 函数内byNKeepBuf偏移计算不对，从g_pByNVRam + NM_RW_NUM应该是 _pByNVRam + NM_RW_NUM*2
								  为了方便理清，增加g_pByNKeepRam指向nKeep继电器的nvram缓冲区
*************************************************************************/
//#ifdef __PL_WIN_SIMULATION__
//#include "stdafx.h" 
//#endif

#include "plconfig.h"

#include "Logic.h"
#include "logicdef.h"
//#include "sysdef.h"
#include "relaydef.h" 
#include "sample.h"
#include "Aa1024.h"
#ifdef __PL_WIN_SIMULATION__
#include "platform.h"
#define	SW_STATUS_SET			1
#define	SW_STATUS_CUT			2
#define	CFG_STATUS_SET			1
#define	CFG_STATUS_CUT			2
#define	DB_BI_OPEN				0
#define	DB_BI_CLOSE				1
#define DB_EXECUTE				1
#define DB_CANCEL 				2
#else
#include "Dbasedef.h"
#include "Dbfunc.h"
#include "DaeFunc.h"
#include "Db_err.h"
#include "BO.h"
#include "relayfun.h"
//1300中开出继电器需要保存到nvram，6500不保存
//
#if(SYS_CPU_TYPE==CPU_COLDFIRE)   //1//6500包括led.h文件
#define _OUTPUT_NVRAM_  FALSE
#include "led.h"
extern  HANDLE g_hDbaseTaskID;
#else 
#define _OUTPUT_NVRAM_  TRUE //1300使用保存在nram中的output
extern  OSHANDLE g_hDbaseTaskID;
#endif //end of #if(SYS_CPU_TYPE==CPU_COLDFIRE)   //1//1300不包括led.h文件

#endif //end of #ifdef __PL_WIN_SIMULATION__
#include "DefaultLogic.h"
//装置复位标志 
#define SYS_RESET_FLAG		0x55aa33cc

static BYTE* g_pByNVRam = (BYTE*)LOGIC_NVRAM_BASE;
static BYTE* g_pByNKeepRam = (BYTE*)(LOGIC_NVRAM_BASE+ NM_RW_NUM*2); //2014/12/31
static BYTE* g_pByOutputNVRam = (BYTE*)(LOGIC_NVRAM_BASE+ (NM_RW_NUM+NKEEP_RW_NUM)*2);

extern TTimeRelayWordInf TimeRelayWordInf[];  // 时间继电器字的额外描述数据结构 

extern TTimeRelayWordInf ConstTimeRelayWordInf[]; //常数时间继电器用
extern 	WORD TIMERelayWordConst[][2]; //常数时间继电器参数(在方程中写常数)

extern	DWORD TripRelayWordInf[]; //跳闸继电器的额外结构
extern TPulseRelayWordInf PulseRelayWordInf[]; //脉冲继电器结构
extern TGERelayWordInf GERelayWordInf[]; //比较继电器结构
extern LONG GEParmList[]; //比较继电器参数

//@extern LONG InputRelayWordBI[]; //开入继电器与BI的关联关系


extern BYTE byVInValue[]; //Vin存放
extern BYTE byOutPreValue[]; //out上次的状态
extern BYTE byDebugRelayBuf[]; //调试继电器缓冲区

//2011-6-16 10:53:08 闪灯缓冲区，接收链路继电器， 发送链路继电器
extern BYTE byLedFlashBuf[];
extern BYTE byTxLinkBuf[];
extern BYTE byRxLinkBuf[];

//2012-02-13 调试开入调试开出继电器缓冲区
extern BYTE byDebugInRWBuf[];
extern BYTE byDebugOutRwBuf[];

//2012-5-3 计数器
extern LONG nCountRelayBuf[];

//@extern TTimeSetting	TimeSetting[]; //时间继电器定值
extern  BYTE byRelayWordValue[]; //继电器值
extern BYTE*  g_byPLCSource;      //逻辑方程参数在e2的地址

extern volatile DWORD dwRelayMSCnt;		//保护用毫秒计数器

extern TSysConfigTable	*G_pCfgTable;

extern TRunTimeBI  *pSysBI;		//遥信实时数据指针	在relay.cpp中定义	

extern USER_FUN_RELAY_WORD_PTR s_pfnUserFunList[];
//extern kTLogicPara_Expression Logic_Expression[];
//extern kTLogicPara_Head Logic_Head;
//extern kTLogicPara_CalcNode Logic_Node[];
//extern TCommonFileHead CommonFile_Head;

#define  DITYPE_PULSE 5 //脉冲电度
#define  DITYPE_VIRIN 7 //虚拟遥信
#define  DITYPE_INSIDEIN 6 //内部遥信
#define  DITYPE_SBI 1 //单点遥信

//扩展参数类型,生成到参数文件里的
enum ParaExType
{
	Type_NONE = 128,
	Type_Const,
	Type_RM,
	Type_RS ,
	Type_AS,
	Type_Counter,
	Type_Report ,
	Type_Float,
	Type_SEQ ,
};

//plc文件标记， 3001表示工具软件的版本号为3.0.0.1， 其中3是主版本号，如果参数结构发生变化
//plc文件不能兼容，主版本号会变化
const BYTE g_byPLCFlag[8] = "PLC3001";


//调试用{
#ifdef __PL_WIN_SIMULATION__
MSG msg;
extern BOOL g_bIsEndThread;
#endif
//}


#define _WriteRelayWord(index,bContext)\
{\
	byRelayWordValue[index] <<= 4;\
	(bContext) ? (byRelayWordValue[index] |= 0x01) : (byRelayWordValue[index] &= 0xF0);\
}

/****************************************************/
/*		可编程逻辑方程模块内部数据结构				*/
/****************************************************/

typedef BOOL (*READ_RELAY_WORD_PTR)(WORD);		//ReadRelayWord函数 
typedef void (*WRITE_RELAY_WORD_PTR)(WORD, BOOL);//WriteRelayWord函数 , 继电器序号， 写入值
//2009.10.15 函数参数改为2个, 2011-6-16函数参数改为3个，第三个参数是=号右边的结果
//以前所有函数都是在等号右边等于true的时候才执行，现在把等号右边的值
//最为一个参数传给函数，比如ledflash =1和=0函数运行的情况不同
typedef void (*RELAY_SYSFUN_PTR)(WORD, WORD);  //逻辑方程系统函数

//逻辑参数状态
LONG g_PLCStatus = PLC_STATUS_ERR; //syslogic.plc 状

//调试逻辑方程用 
DWORD g_dwPLCDebugStep1=0, g_dwPLCDebugStep2=0; //调试状态标志
DWORD g_dwPLCCurLine=0, g_dwPLCBreakPoint=0; //g_dwPLCCurLine运行的行， g_dwPLCBreakPoint调试断点
DWORD g_dwPLCDebugLoop=0; //循环数，当g_dwPLCCurLine==g_dwPLCBreakPoint时有用
DWORD g_dwPLCDebugOneStep=0; //单步运行

LONG g_nPLCLineSys = 0;


BYTE* g_LogicParaBuf = NULL;		// 逻辑方程参数缓冲区 
static struct _TCommonFileHead* s_pCommonFileHead = NULL;
static struct _TLogicPara_Head* s_pLogicParaHead = NULL;		// 逻辑方程参数文件头部 
static struct _TLogicPara_Expression* s_pExpression = NULL ;		// 表达式描述 
static struct _TLogicPara_CalcNode* s_pCalcNode = NULL;			// 执行节点描述 
static WORD* s_pwChecksum = NULL;
//static BOOL bIsRepStarted = FALSE;  //是否已经启动报告了


// 继电器字执行状态,用于时间继电器字
#define RELAY_STATE_A	0	//分状态
#define RELAY_STATE_B	1	//分->合计时状态
#define RELAY_STATE_C	2	//合状态
#define RELAY_STATE_D	3	//合->分计时状态

//虚遥信状态首次刷新成功标志
BOOL isVirDIValide = FALSE;

void DebugRunLogic(DWORD* CurLine);


//------------可编程逻辑方程模块内部接口函数----------------------------

/****************************************************************************************
 * 
 * 功能描述: 写继电器字(用于常规继电器字)，保留上次变位信息，用于沿操作 
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值， TRUE or FAlSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_Normal(WORD wRelayWordIndex, BOOL bContext)
{
	_WriteRelayWord(wRelayWordIndex, bContext);
	
//@	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 
//@	if (bContext)
//@	{
//@		byRelayWordValue[wRelayWordIndex] |= 0x01;
//@	}
//@	else
//@	{
//@		byRelayWordValue[wRelayWordIndex] &= 0xF0;
//@	}
}



/****************************************************************************************
 * 
 * 功能描述: 写继电器字(用于时间继电器字) 
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号（所以继电器字一起的序号， 不是时间继电器内部序号）
 *
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值 ， TRUE 或 FALSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Time(WORD wRelayWordIndex,  BOOL bContext)
{
	WORD wIndex = wRelayWordIndex - TIME_RELAY_WORD_START;
	WORD dwState = TimeRelayWordInf[wIndex].dwState;

	//2011-7-5 每次赋值都要左移的， 修正原来延时间隔中不左移导致沿一直存在的错误
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 
	
	if (bContext)
	{
		//无启动延时的直接合了
		if(*(TimeSetting[wIndex].pStartTime) == 0)  
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			return;
		}

		//当前分位  记录时间，准换为分->合计时
		if (dwState == RELAY_STATE_A) 
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_B;
			TimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;//保护用毫秒计数器;
			byRelayWordValue[wRelayWordIndex] &= 0xF0;
			return;
		}

		//当前分->合计时  时间到了转换为合位，  时间不到继续分->合计时
		else if(dwState == RELAY_STATE_B) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - TimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= *(TimeSetting[wIndex].pStartTime) )
			{
				/* 启动时间到 */
				TimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			else
			{
				byRelayWordValue[wRelayWordIndex] &= 0xF0; //还是分
			}
			return;
		}

		//合，合-》分计时  转换为合位
		else if (dwState == RELAY_STATE_C || dwState == RELAY_STATE_D) 
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;

			return;
		}
	}
	else
	{
		//无复归延时的直接分了
		if( *(TimeSetting[wIndex].pReturnTime) == 0)
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 

			return;

		}

		 //分，分-》合计时  转换为 分位 
		if (dwState == RELAY_STATE_B || dwState == RELAY_STATE_A)
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 

			return;
		}

		 //合  转换为合->分计时
		else if (dwState == RELAY_STATE_C)
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_D;
			TimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			return;
		}

		//合->分计时	时间到转换为分位， 时间不到继续计时
		else if(dwState == RELAY_STATE_D) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - TimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= *(TimeSetting[wIndex].pReturnTime) )
			{
				/* 启动时间到 */
				TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
				byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			}
			else
			{
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			return;


		}
	}
}


/****************************************************************************************
 * 
 * 功能描述: 写常数时间继电器字
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号（所以继电器字一起的序号， 不是常数时间继电器内部序号）
 *
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值 ， TRUE 或 FALSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_ConstTime(WORD wRelayWordIndex,  BOOL bContext)
{
	WORD wIndex = wRelayWordIndex - CONST_TIME_RW_START;
	WORD dwState = ConstTimeRelayWordInf[wIndex].dwState;

	//2011-7-5 每次赋值都要左移的， 修正原来延时间隔中不左移导致沿一直存在的错误
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 

	if (bContext)
	{
		//无启动延时的直接合了
		if(TIMERelayWordConst[wIndex][0] == 0)  
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			return;

		}

		//当前分位  转换为分->合计时
		if (dwState == RELAY_STATE_A) 
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_B;
			ConstTimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;//保护用毫秒计数器;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			return;
		}

		//当前分->合计时
		else if(dwState == RELAY_STATE_B) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - ConstTimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= TIMERelayWordConst[wIndex][0] )
			{
				/* 启动时间到 */
				ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			else
			{
				
				byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			}
			return;
		}

		//合，合-》分计时
		else if (dwState == RELAY_STATE_C || dwState == RELAY_STATE_D) 
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;

			return;
		}
	}
	else
	{
		//无复归延时的直接分了
		if(TIMERelayWordConst[wIndex][1] == 0)
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			return;

		}

		 //分，分-》合计时
		if (dwState == RELAY_STATE_B || dwState == RELAY_STATE_A)
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			return;
		}

		 //合 转换为合->分计时
		else if (dwState == RELAY_STATE_C)
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_D;
			//2012-2-13 开始计时
			ConstTimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;
			
			byRelayWordValue[wRelayWordIndex] |= 0x01;

			return;
		}

		//合->分计时		
		else if(dwState == RELAY_STATE_D) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - ConstTimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= TIMERelayWordConst[wIndex][1] )
			{
				/* 返回时间到 */
				ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
				byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			}
			else
			{
				
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			return;


		}
	}
}

/****************************************************************************************
 * 
 * 功能描述: 写自保持继电器字
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值 TRURE 或 FALSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_Keep(WORD wRelayWordIndex,  BOOL bContext)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];

	//每次赋值都要左移，不管状态怎么变化
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 

	if ((byValue & 0x01) == 0x01)  //原来为1则不管bContext 还是1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;  /* 保持1状态不变 */
		return;	
	}
	else
	{	
		if(bContext)
			byRelayWordValue[wRelayWordIndex] |= 0x01;

	}

}

/****************************************************************************************
 * 
 * 功能描述: 写跳闸继电器
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 
 *
 * 返回代码: 
 * 
 * 其它: 写跳闸继电器， 也是一种自保持继电器
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Trip(WORD wRelayWordIndex,  BOOL bContext)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	WORD wIndex = wRelayWordIndex - TRIP_RELAY_WORD_START;


	//每次赋值都要左移
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 

	if ((byValue & 0x01) == 0x01)  //原来为1则还是1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;  /* 保持1状态不变 */
		return;	
	}
	else
	{	//原来是0现在写1
		if(bContext)
		{
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			//记录从0-》1的时刻
			TripRelayWordInf[wIndex] = dwRelayMSCnt; 
		}

	}
}
/****************************************************************************************
 * 
 * 功能描述: 写脉冲继电器继电器值
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Pulse(WORD wRelayWordIndex,  BOOL bContext)
{
	if (wRelayWordIndex < PULSE_RELAY_WORD_START) return;
	WORD wIndex = wRelayWordIndex - PULSE_RELAY_WORD_START;
	if (wIndex>= PULSE_RELAY_WORD_NUM) return;
	
	//启动
	if (bContext && !PulseRelayWordInf[wIndex].bIsRun)
	{
		PulseRelayWordInf[wIndex].bIsRun = TRUE;
		PulseRelayWordInf[wIndex].dwTime = dwRelayMSCnt;
	}

	//停止
	if (!bContext && PulseRelayWordInf[wIndex].bIsRun)
	{
		PulseRelayWordInf[wIndex].bIsRun = FALSE;
		PulseRelayWordInf[wIndex].dwTime = 0;
		byRelayWordValue[wRelayWordIndex] = 0;  //停止以后值也有变成0而不是保持不变
	}

	//正常运行
	if (bContext && PulseRelayWordInf[wIndex].bIsRun)
	{
		if ((dwRelayMSCnt - PulseRelayWordInf[wIndex].dwTime) > PulseRelayWordInf[wIndex].dwTimeParam )
		{
			//取反， 重新记时
			byRelayWordValue[wRelayWordIndex] = !byRelayWordValue[wRelayWordIndex];
			PulseRelayWordInf[wIndex].dwTime =  dwRelayMSCnt;
		}
	}
}



/****************************************************************************************
 * 
 * 功能描述: 写比较继电器继电器值
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_GE(WORD wRelayWordIndex,  BOOL bContext)
{
	if (wRelayWordIndex < GE_RELAY_WORD_START) return;
	WORD wIndex = wRelayWordIndex - GE_RELAY_WORD_START;
	if (wIndex>= GE_RELAY_WORD_NUM) return;

	if(!bContext)
		return;
	if( *(GERelayWordInf[wIndex].npParm1) >= *(GERelayWordInf[wIndex].npParm2))
		{
			_WriteRelayWord(wRelayWordIndex, TRUE);
		}
	else
		{
			_WriteRelayWord(wRelayWordIndex, FALSE);
		}
}

/****************************************************************************************
 * 
 * 功能描述: 写信号继电器继电器值, 信号继电器也是自保持的，当前值发生变化时要保存到e2
 * e2中只要保存当前值就行， 不保存沿。继电器字内存中保存沿
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Signal(WORD wRelayWordIndex,  BOOL bContext)
{
	//是一种自保持继电器
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
//	BYTE byNotValue = ~byValue;
	WORD wIndex = wRelayWordIndex - SIGNAL_RELAY_WORD_START;
	if (wIndex>= SIGNAL_RELAY_WORD_NUM) return;

	//值变化要写到e2, e2中不管上次值， 只关心本次值的
/*	if((byValue & 0x01) != bContext)
	{
		//写道e2中, 保存正反两份
		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex, 1, (void*)&(byValue));
		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex+1, 1, (void*)&(byNotValue));
	}
*/	
	//每次赋值都要左移
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 
	
	if ((byValue & 0x01) == 0x01)  //原来为1则还是1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;	/* 保持1状态不变 */
		return; 
	}
	else
	{	
		if(bContext) //原来为0且现在要写1
		{
//			BYTE byVal = 1;
//			BYTE byNotVal = ~byVal;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			//不能在逻辑方程任务中写EEPROM，通知数据库修改
			OS_PostMessage(g_hDbaseTaskID, SM_WRITESINGNALRW, EEPROM_RELAY_SINGAL+2*wIndex,0x10,0,0);
			
//			EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex, 1, &(byVal));
//			EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex+1, 1, &(byNotVal));
			
		}
		
	}
}


/****************************************************************************************
 * 
 * 功能描述: 写虚遥信继电器继电器值
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_VIn(WORD wRelayWordIndex,  BOOL bContext)
{
//	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	WORD wIndex = wRelayWordIndex - VIN_RELAY_WORD_START;

	//写入数组
	_WriteRelayWord(wRelayWordIndex, bContext);

	//还要写入内存中
	byVInValue[wIndex] =  bContext;
}


/****************************************************************************************
 * 
 * 功能描述: 读常规继电器继电器值
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Normal(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x01) == 0x01) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * 功能描述: 读取反继电器值
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Not(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x01) == 0) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * 功能描述: 读上升沿继电器
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Up(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x01) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * 功能描述: 读下降沿继电器
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Down(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x10) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * 功能描述: 读上升沿并取反继电器
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_NotUp(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x01) ? FALSE : TRUE;
}

/****************************************************************************************
 * 
 * 功能描述: 读下降沿并取反继电器
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号
 *
 * 返回代码: 继电器值 TRURE 或 FALSE
 * 
 * 其它: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_NotDown(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x10) ? FALSE : TRUE;
}
/****************************************************************************************
 * 
 * 功能描述: 写电保存开出继电器字，保留上次变位信息，用于沿操作，数值变化时要保存到nvram
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值， TRUE or FAlSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_NMOut(WORD wRelayWordIndex, BOOL bContext)
{
	//值和上次的不同，需要保存到nvram中
	BYTE byOld = byRelayWordValue[wRelayWordIndex];
	_WriteRelayWord(wRelayWordIndex, bContext);
	
	if(byOld != byRelayWordValue[wRelayWordIndex])
	{
		//有变化则写到nvram中
		WORD wOutIndex = wRelayWordIndex - OUTPUT_RELAY_WORD_START;
		if(wOutIndex >= OUTPUT_RELAY_WORD_NUM)
			return;
	
		g_pByOutputNVRam[2*wOutIndex]= byRelayWordValue[wRelayWordIndex];
		g_pByOutputNVRam[2*wOutIndex+1] = ~byRelayWordValue[wRelayWordIndex];	
	}
}

/****************************************************************************************
 * 
 * 功能描述: 写电保持继电器字(用于电保持中间继电器字)，保留上次变位信息，用于沿操作，数值变化时要保存到nvram
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值， TRUE or FAlSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_NMNormal(WORD wRelayWordIndex, BOOL bContext)
{
	//值和上次的不同，需要保存到nvram中
	BYTE byOld = byRelayWordValue[wRelayWordIndex];
	_WriteRelayWord(wRelayWordIndex, bContext);
	
	if(byOld != byRelayWordValue[wRelayWordIndex])
	{
		//有变化则写到nvram中
		WORD wNHIndex = wRelayWordIndex - NM_RW_START;
		if(wNHIndex >= NM_RW_NUM)
			return;
	
		g_pByNVRam[2*wNHIndex]= byRelayWordValue[wRelayWordIndex];
		g_pByNVRam[2*wNHIndex+1] = ~byRelayWordValue[wRelayWordIndex];	
	}
}
/****************************************************************************************
 * 
 * 功能描述: 写电保持继自保持电器字，数值变化时要保存到nvram，需要复归函数才能变0
 * 
 * 参数说明: 
 *			- WORD wRelayWordIndex
 *			  类型: 
 *			  说明: 继电器字序号
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 写入值， TRUE or FAlSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_NHKeep(WORD wRelayWordIndex,  BOOL bContext)
{
	ASSERT(wRelayWordIndex > NKEEP_RW_START);
	ASSERT((wRelayWordIndex -  NKEEP_RW_NUM) > NKEEP_RW_START);
	
	BYTE byValue = byRelayWordValue[wRelayWordIndex];

	//每次赋值都要左移，不管状态怎么变化
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 

	if ((byValue & 0x01) == 0x01)  //原来为1则不管bContext 还是1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;  /* 保持1状态不变 */
		return;	
	}
	else
	{	
		if(bContext)
			byRelayWordValue[wRelayWordIndex] |= 0x01;

	}
	if(byValue != byRelayWordValue[wRelayWordIndex] )
	{
		
		//有变化则写到nvram中
		WORD wNHIndex = wRelayWordIndex - NKEEP_RW_START;
		if(wNHIndex >= NKEEP_RW_NUM)
			return;
		g_pByNVRam[2*wNHIndex+NM_RW_NUM]= byRelayWordValue[wRelayWordIndex];
		g_pByNVRam[2*wNHIndex+1+NM_RW_NUM] = ~byRelayWordValue[wRelayWordIndex];	
	}
}

// 读继电器字函数列表 
static READ_RELAY_WORD_PTR s_pfnReadList[] =
{
	_ReadRelayWord_Normal,
	_ReadRelayWord_Not,
	_ReadRelayWord_Up,
	_ReadRelayWord_Down,
	_ReadRelayWord_NotUp,
	_ReadRelayWord_NotDown,
};

// 写继电器字函数列表 , 顺序与ExpType保持一致
//@TypeID	TypeName	TypeLabel	Dec
//@0	常规继电器	Normal	
//@1	自保持继电器	Keep	
//@2	时间继电器	Time	
//@3	跳闸继电器	TR	
//@4	调试继电器	Virtual	
//@5	脉冲继电器	Pulse	
//@6	信号继电器	Single	
//@7	虚拟开入继电器	Vin	
//@8	比较继电器	GE	
//@9	常数时间继电器	CT

static WRITE_RELAY_WORD_PTR s_pfnWriteList[] =
{
	_WriteRelayWord_Normal,
	_WriteRelayWord_Keep,
	_WriteRelayWord_Time,
	_WriteRelayWord_Trip,
	NULL,
	_WriteRelayWord_Pulse, //脉冲逻辑方程写函数
	_WriteRelayWord_Signal, //信号继电器写函数
	_WriteRelayWord_VIn, //虚拟开入继电器写函数
	_WriteRelayWord_GE, //比较继电器写函数
	_WriteRelayWord_ConstTime, //常数时间继电器写函数
	_WriteRelayWord_NMNormal, //电保持中间继电器
	_WriteRelayWord_NHKeep,//电保持自保持继电器
	_WriteRelayWord_NMOut,//电保存开出

};
//2012-5-25 检测plc的文件标志以及 版本号是否和数据库版本号一致
BOOL _CheckFileFlage(_TCommonFileHead* pFileHead, _TLogicPara_Head* pHead)
{
	BYTE byVersion[8];
	
	BYTE* pFlag = pHead->byFileFlag;
	if((pFlag[0]!=g_byPLCFlag[0])||(pFlag[1]!=g_byPLCFlag[1])||(pFlag[2]!=g_byPLCFlag[2]))
	{
		LogError("_CheckFileFlage",FILE_LINE,"文件不是plc格式的!");
		return FALSE;
	}
	if(pFlag[3]!=g_byPLCFlag[3])
	{
		LogError("_CheckFileFlage",FILE_LINE,"plc文件结构与当前程序不兼容!");
		return FALSE;
	}
	
	DWORD wdVersion;
	
	STATUS rc = DB_GetDataBaseVersion(byVersion);
	if(rc == DB_ERR_OK)
	{
		wdVersion = MAKEDWORD(byVersion[0], byVersion[1], byVersion[2], byVersion[3]);
		if(pFileHead->dwVersion != wdVersion)
		{
			
			LogError("_CheckFileFlage",FILE_LINE,"plc文件版本号:%X和数据库版本号:%X不相同",
				pFileHead->dwVersion, wdVersion);
			//return FALSE;
		}
	}
	return TRUE;
	
}


//检查缓冲区中逻辑方程是否合法 dwFileLen为参数长度
BOOL _CheckPLCParam(DWORD dwFileLen)
{
	//计算文件头校验码 必须先校验文件头， 文件头不校验的话s_pCommonFileHead->dwSize是不可信的
	DWORD i;
	DWORD dwParaOffset = (DWORD)g_LogicParaBuf;
	
	//读通用文件头
	s_pCommonFileHead = (_TCommonFileHead*)dwParaOffset;

	//判断文件大小
	if(dwFileLen != s_pCommonFileHead->dwSize)
	{
		LogError("CheckPLCParam",FILE_LINE,"paraLen  error %d %d", dwFileLen, s_pCommonFileHead->dwSize);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	//计算文件头校验码是否正确
	WORD wCheck = DAE_CheckSum(g_LogicParaBuf, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);

	if(dwCheck != s_pCommonFileHead->dwCheck)
	{
		LogError("_CheckPLCParam",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,s_pCommonFileHead->dwCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}

	//计算整个校验码(文件头外的部分)是否正确
	wCheck = DAE_CheckSum(g_LogicParaBuf+sizeof(_TCommonFileHead), 
				s_pCommonFileHead->dwSize - 2-sizeof(_TCommonFileHead));
	WORD* pFileCheck = (WORD*)(g_LogicParaBuf+s_pCommonFileHead->dwSize -2);
	if(wCheck != *pFileCheck)
	{
		LogError("_CheckPLCParam",FILE_LINE,"File checksum error 0x%X  0x%X",
			wCheck,*pFileCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	dwParaOffset += sizeof(_TCommonFileHead);
	
	// 读取逻辑方程参数文件头部 
	s_pLogicParaHead = (_TLogicPara_Head*)dwParaOffset;
	if (s_pLogicParaHead->dwRelayWordNum != RELAY_WORD_NUM) 
	{
		LogError("ResetLogicPar",FILE_LINE,"relayWordNum err 0x%X 0x%X ",
			s_pLogicParaHead->dwRelayWordNum, RELAY_WORD_NUM);
		return FALSE;
	}
	if (s_pLogicParaHead->dwRelayWordNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwRelayWordNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwExpressionNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwExpressionNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwCalcNodeNum == 0)
	{
		LogError("ResetLogicPar",FILE_LINE,"dwCalcNodeNum = 0");
		return FALSE;
	}
	
	//2012-5-25 文件结构判断
	if(!_CheckFileFlage(s_pCommonFileHead,s_pLogicParaHead))
		return FALSE;

	
	dwParaOffset += sizeof(_TLogicPara_Head);


	// 读取表达式描述结构 
	s_pExpression = (_TLogicPara_Expression*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwExpressionNum; i++)
	{
		if (s_pExpression[i].wCalcNodeNum == 0) 
		{
				
				LogError("ResetLogicPar",FILE_LINE,"s_pExpression[i].wCalcNodeNum == 0");
				return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_Expression) * s_pLogicParaHead->dwExpressionNum;

	// 读取执行节点描述结构 
	s_pCalcNode = (_TLogicPara_CalcNode*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwCalcNodeNum; i++)
	{
		if (s_pCalcNode[i].wRelayIndex >= RELAY_WORD_NUM)
		{
			
			LogError("ResetLogicPar",FILE_LINE,"s_pCalcNode.wRelayIndex >= RELAY_WORD_NUM");
			return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_CalcNode) * s_pLogicParaHead->dwCalcNodeNum;

	
	//读取校验码
	//2012-6-8 从按照dwParaOffset找到校验改为用s_pCommonFileHead->dwSize找到的pFileCheck
	//因为plc部分还包括和逻辑运行无关的括号信息(反向时有用，在这儿没用)
	s_pwChecksum = (WORD*)pFileCheck;	

	return TRUE;
}
/****************************************************/
/*		以下是可编程逻辑方程模块外部接口函数				*/
/****************************************************/
// 初始化可编程逻辑模块 ,只调用一次， 分配逻辑方程存储空间用
BOOL InitLogicParseModule(void)
{
	//考虑CPU板软件自动识别功能，需要通过代码实现对指向NVRAM空间指针的初始化，而不能通过
	//编译初始化方式完成，否则导致指针初始化错误
	g_pByNVRam = (BYTE*)LOGIC_NVRAM_BASE;
	g_pByNKeepRam = (BYTE*)(LOGIC_NVRAM_BASE+ NM_RW_NUM*2);
	g_pByOutputNVRam = (BYTE*)(LOGIC_NVRAM_BASE+ (NM_RW_NUM+NKEEP_RW_NUM)*2);

	// 为系统逻辑方程模块分配内存 
	g_LogicParaBuf = (BYTE*)OS_MemAlloc(__PL_LOGIC_SIZE__);
	

	if((g_LogicParaBuf == NULL))
	{
		LogError("InitLogicParseModule",FILE_LINE,"Mem allocate fail");
		return FALSE;
	}
	
	return TRUE;
}


/************************************************************
	功能：//参数为常数时候的初始化
	参数：
	     WORD wIndex[in] 比较继电器序号
	     WORD wPara[in] 常数值 
	     WORD wParaOther[in] 作为比较的另一个参数
	     WORD wPara[in] 作为比较的另一个参数的扩展参数ex
	返回：LONG  0:成功 1:失败
************************************************************/

BOOL InitConstGePara(WORD wIndex,WORD wPara, WORD wParaOther, WORD wExOther)
{
	
	LONG lCF, LVal, rc, lSetVal, lSysSetVal;
	WORD wRT, wCFIndex, wSetAreaNo,wDec;
	TRM_Disp RM_Disp;
	TCF_Disp CF_Disp;
	TSET_Disp tSET_Disp;
	float fVal;

	//另一个参数的类型
	////2保护测量值 3定值 4 辅助定值 5计数器
	switch(wExOther)
	{
		case Type_RM:
			rc = DB_GetRMDisp(wParaOther, &RM_Disp);
			if(rc == DB_ERR_OK)
			{
				wCFIndex = RM_Disp.nCFIndex;
				DB_GetCFDisp(wCFIndex, &CF_Disp);
				lCF = DB_GetCFValue(wCFIndex, &lCF);
				wRT = CF_Disp.wCof;
			}
			else
			{
				g_PLCStatus = PLC_STATUS_ERR;
				LogError("Logic",FILE_LINE,"InitGERelayWord DB_GetRMDisp");
				return FALSE;
			}

			//
			
			if(lCF == 0) 
				lCF = 1;
			if(wRT == 0) 
				wRT = 1;
			
			//	乘系数/倍率  且在逻辑方程工具软件中放大了10倍，
			LVal = (long)(wPara*lCF)/wRT/10;
			GEParmList[wIndex] = LVal;
			break;
		case Type_RS:
			//把常数转换为整形值
			fVal = (float)wPara/10;
			rc = ConvertSETFloat2Fix(wParaOther, fVal, &LVal);
			
			if(rc != DB_ERR_OK)
			{
				g_PLCStatus = PLC_STATUS_ERR;
				LogError("Logic",FILE_LINE,"InitGERelayWord DB_GetRMDisp");
				return FALSE;
			}
			
			GEParmList[wIndex] = LVal;
			break;
		case Type_AS:
			
			fVal = (float)wPara/10;
			rc = ConvertSystemSETFloat2Fix(wParaOther, fVal, &LVal);
			
			if(rc != DB_ERR_OK)
			{
				g_PLCStatus = PLC_STATUS_ERR;
				LogError("Logic",FILE_LINE,"InitGERelayWord DB_GetRMDisp");
				return FALSE;
			}
			GEParmList[wIndex] = LVal;
			break;
		case RW_TYPE_COUNTER:
			
			GEParmList[wIndex] = wPara/10;
			break;
			
		}
	return TRUE;

}

//初始化比较继电器参数 1 常数 2保护测量值 3定值 4 辅助定值 5计数器
//初始化表达pExpression 的GE继电器 的参数之一wWhichPara  1:wPara; 2:wPara2
BOOL InitOneGEPara(_TLogicPara_Expression* pExpression, WORD wWhichPara)
{
	
	LONG** pnPara;
	TRM_Disp RM_Disp;
	TCF_Disp CF_Disp;
	LONG lCF, LVal, rc, lSetVal, lSysSetVal;
	WORD wRT, wCFIndex, wSetAreaNo;
	WORD wIndex, wPara, wParaEx, wParaOther, wExOther,wCounterIndex;

	
	wIndex = pExpression->wResultRelayWordIndex - GE_RELAY_WORD_START;

	//是参数1还是参数2
	if(wWhichPara == 1)
	{
		
		pnPara = &(GERelayWordInf[wIndex].npParm1);
		
		wPara = pExpression->wParm;
		wParaEx = pExpression->wParaEx;
		wParaOther= pExpression->wParm2;
		wExOther= pExpression->wParaEx2;
	}
	else
	{
		pnPara = &(GERelayWordInf[wIndex].npParm2);
		
		wPara = pExpression->wParm2;
		wParaEx = pExpression->wParaEx2;
		wParaOther= pExpression->wParm;
		wExOther= pExpression->wParaEx;
	}
	
	switch(wParaEx)
	{
		//常数最复杂， 需要判断比较的对象来乘除合适的倍率以便于比较
		case Type_Float:
			{
			if(!InitConstGePara(wIndex,wPara,wParaOther,wExOther))
				return FALSE;
			*pnPara = &(GEParmList[wIndex]);
			}
			break;
		case Type_RM:
			*pnPara= (LONG*)(RM_BUF_ADDR + 4*wPara);
			break;
		case Type_RS:
			*pnPara = (LONG*)(RS_BUF_ADDR + 4*wPara);
			break;
		case Type_AS:
			*pnPara = (LONG*)(AS_BUF_ADDR + 4*wPara);
			break;
		case RW_TYPE_COUNTER:
			wCounterIndex = wPara - CNT_RELAY_WORD_START;
			if(wCounterIndex >= CNT_RELAY_WORD_NUM)
				return FALSE;
			*pnPara = &(nCountRelayBuf[wCounterIndex]);
			break;
	}
	return TRUE;
}

BOOL InitGERelayWord()
{
	int nLineNum;
	nLineNum = s_pLogicParaHead->dwExpressionNum;
	_TLogicPara_Expression* pExpression = s_pExpression;		// 表达式描述 
	
	for(int i=0; i<nLineNum; i++)
	{
		if(pExpression[i].wResultRelayWordType != GE)
			continue;

		if(!InitOneGEPara(&(pExpression[i]), 1))
			return FALSE;

		
		if(!InitOneGEPara(&(pExpression[i]), 2))
			return FALSE;
		
	}
	return TRUE;
}





//初始化保存到nvram内的继电器(NM和NKeep) 以及1300的开出继电器
//如果
BOOL InitNVRamRelyWord(BOOL bIsNewPLCDownLoad = FALSE)
{
	int i;
	//如果是新下载的逻辑，清除保存的值
	if(bIsNewPLCDownLoad)
	{
		
		LogError("Logic", FILE_LINE, "new logic Reset NVRamRelayword");
		for(i = 0; i<NM_RW_NUM; i++)
		{
			g_pByNVRam[2*i] = 0;
			g_pByNVRam[2*i+1] = 0xFF;
		}
		
		for(i = 0; i<NKEEP_RW_NUM; i++)
		{
			g_pByNKeepRam[2*i] = 0;
			g_pByNKeepRam[2*i+1] = 0xFF;
		}

		if(!_OUTPUT_NVRAM_)
			return TRUE;
		for(i = 0; i<OUTPUT_RELAY_WORD_NUM; i++)
		{
			g_pByOutputNVRam[2*i] = 0;
			g_pByOutputNVRam[2*i+1] = 0xFF;
		}
		
		
		return TRUE;
	}

	//是否复位重启决定是否从nvram读取值
	DWORD *pFlag = (DWORD*)SYS_START_FLAG_ADD;	//装置复位内部RAM标志

	//如果不是装置复位，不需要拷贝
	if(*pFlag != SYS_RESET_FLAG)	
		return TRUE;

	for(i = 0; i<NM_RW_NUM; i++)
	{
		//校验正确
		if(g_pByNVRam[2*i]^g_pByNVRam[2*i+1] == 255 )
			byRelayWordValue[NM_RW_START+i] = g_pByNVRam[2*i]&0x01;
		else
		{
			byRelayWordValue[NM_RW_START+i] = 0;
			BYTE byNotVal = ~byRelayWordValue[NM_RW_START+i];
			g_pByNVRam[2*i] = byRelayWordValue[NM_RW_START+i];
			g_pByNVRam[2*i+1] = byNotVal;
			
			LogError("Logic", FILE_LINE, "NM Relayword value %d in NVRam reset", i);
		}
	}


	for(i = 0; i<NKEEP_RW_NUM; i++)
	{
		//校验正确
		if(g_pByNKeepRam[2*i]^g_pByNKeepRam[2*i+1] == 255)
	    	byRelayWordValue[NKEEP_RW_START+i] = g_pByNKeepRam[2*i]&0x01;
		else
		{
			byRelayWordValue[NKEEP_RW_START+i] = 0;
			BYTE byNotVal = ~byRelayWordValue[NKEEP_RW_START+i];
			g_pByNKeepRam[2*i] = byRelayWordValue[NKEEP_RW_START+i];
			g_pByNKeepRam[2*i+1] = byNotVal;
			
			LogError("Logic", FILE_LINE, "NKEEP Relayword value %d in NVRam reset", i);
		}
	}

	//2012-5-23 开出是否保存在nvram中的
	if(!_OUTPUT_NVRAM_)
		return TRUE;

	for(i = 0; i<OUTPUT_RELAY_WORD_NUM; i++)
	{
		//校验正确
		if(g_pByOutputNVRam[2*i]^g_pByOutputNVRam[2*i+1] == 255)
			byRelayWordValue[OUTPUT_RELAY_WORD_START+i] = g_pByOutputNVRam[2*i]&0x01;
		else
		{
			byRelayWordValue[OUTPUT_RELAY_WORD_START+i] = 0;
			BYTE byNotVal = ~byRelayWordValue[OUTPUT_RELAY_WORD_START+i];
			g_pByOutputNVRam[2*i] = byRelayWordValue[OUTPUT_RELAY_WORD_START+i];
			g_pByOutputNVRam[2*i+1] = byNotVal;
			
			LogError("Logic", FILE_LINE, "NMout Relayword value %d in NMoutVRam reset", i);
		}
	}

	return TRUE;

}
//信号继要写eeprom,在数据库任务中保存的，保存的是0x00,0x01, 或者0x01,0x00,
//和其他nvram中保存的继电器保存值和取反值不同
BOOL InitSignalRelayWord(BOOL bIsNewPLCDownLoad = FALSE)
{
	BYTE bBuf[SIGNAL_RELAY_WORD_NUM*2+1];
	//如果是新下载了逻辑后初始化信号继电器，需要清除eeprom保存的值
	if(bIsNewPLCDownLoad)
	{
		for(int j=0;j<SIGNAL_RELAY_WORD_NUM; j++)
		{
			bBuf[2*j] = 0x00;
			bBuf[2*j+1]=0x01;
		}
		EEPROM_Write(EEPROM_RELAY_SINGAL, SIGNAL_RELAY_WORD_NUM*2, bBuf);
		
		LogError("Logic", FILE_LINE, "new logic reset SignalRelayWord");
		return TRUE;
	}

	//不是新下载逻辑，需要从eeprom读取值到继电器
	EEPROM_Read(EEPROM_RELAY_SINGAL, SIGNAL_RELAY_WORD_NUM*2, bBuf);
	
	for(int i = 0; i<SIGNAL_RELAY_WORD_NUM; i++)
	{
		//校验正确
		if(bBuf[2*i]^bBuf[2*i+1] == 0x01)
			byRelayWordValue[SIGNAL_RELAY_WORD_START+i] = bBuf[2*i]&0x01;
		else
		{
			byRelayWordValue[SIGNAL_RELAY_WORD_START+i] = 0x00;
			BYTE byNotValLastBit = 0x01;
			
			EEPROM_Write(EEPROM_RELAY_SINGAL+2*i, 1, &byRelayWordValue[SIGNAL_RELAY_WORD_START+i]);
			
			EEPROM_Write(EEPROM_RELAY_SINGAL+2*i+1, 1, &byNotValLastBit);
			LogError("Logic", FILE_LINE, "signal value %d in eeprom reset", i);
		}
	}
	return TRUE;
	
}

//初始化开入继电器参数 修改开入属性后电度和遥信个数不编号了，所以对应关系不变了
/*
BOOL InitInputRelayPara()
{
	LONG i, j,rc ;
	j=0;
	TDIPara DIPara;
	for(i=0; i<INPUT_RELAY_WORD_NUM; i++)
	{
		rc = DB_GetDIPara(i, &DIPara);
		if(rc != DB_ERR_OK)
		{
			LogError("InitInputRelayPara",FILE_LINE,"DB_GetDIPara error");
			g_PLCStatus = PLC_STATUS_ERR;
			return FALSE;
		}
		if((DIPara.wClass == DITYPE_VIRIN )|| (DIPara.wClass == DITYPE_INSIDEIN) 
			|| (DIPara.wClass == DITYPE_SBI))
		{
			InputRelayWordBI[i] = j++;
		}
		else
		{
			InputRelayWordBI[i] = -1;
			j++;
		}
	}
	return TRUE;
}*/

//初始化固定时间继电器
BOOL InitConstTimeRelayWord()
{
	int nLineNum;
	nLineNum = s_pLogicParaHead->dwExpressionNum;
	_TLogicPara_Expression* pExpression = s_pExpression;		// 表达式描述 
	
	WORD wParm1, wParm2, wIndex;



	memset((void*)TIMERelayWordConst, 0, sizeof(WORD)*(CONST_TIME_RW_NUM+1)*2);
	
	
	for(int i=0; i<nLineNum; i++)
	{


		if(pExpression[i].wResultRelayWordType != CONSTTIME)
			continue;
		wParm1 = pExpression[i].wParm;
		wParm2 = pExpression[i].wParm2;
		wIndex = pExpression[i].wResultRelayWordIndex - CONST_TIME_RW_START;

		if(wParm1 != 0) 
		{
			TIMERelayWordConst[wIndex][0] = wParm1;
//			TimeSetting[wIndex].pStartTime = &(TIMERelayWordConst[wIndex][0]);
		}

		if(wParm2 !=0)
		{
			TIMERelayWordConst[wIndex][1] = wParm2;
//			TimeSetting[wIndex].pReturnTime = &(TIMERelayWordConst[wIndex][1]);
		}

		
	}
	return TRUE;

	
}
//从文件装载方程到分配的逻辑方程参数缓冲区
DWORD _LoadFileLogicPar(char* strFileName)
{
	DWORD dwFileLen, dwLogicLen;

	STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc != DB_ERR_OK) || (dwFileLen <=sizeof(TFileHead)) )
	{
		LogError("ResetLogicParseModule",FILE_LINE,"DAE_ReadFile err");
		return 0;
	}
	
	//先读取文件头， 安装文件描述的文件大小读逻辑部分
	TFileHead tFileHead;
	rc = DAE_ReadFile(strFileName, (BYTE*)(&tFileHead),sizeof(TFileHead));
	if((rc != DB_ERR_OK))
	{
		LogError("ResetLogicParseModule",FILE_LINE,"DAE_ReadFile err");
		return 0;
	}
	
	//校验文件头， 读取文件长度
	WORD wCheck = DAE_CheckSum((BYTE*)&tFileHead, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);
			
	if(dwCheck != tFileHead.dwCheck)
	{
		LogError("_CheckPLCParam",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,tFileHead.dwCheck);
		return 0;
	}
	dwLogicLen = tFileHead.dwSize;

	
	if((dwLogicLen>__PL_LOGIC_SIZE__)||(dwLogicLen<0))
	{
		LogError("ResetLogicParseModule",FILE_LINE,"The logic size is > __PL_LOGIC_SIZE__!");
		return 0;
	}
	else
	{
		rc = DAE_ReadFile(strFileName, g_LogicParaBuf,dwLogicLen);
		if(DB_ERR_OK == rc)
		{
			return dwLogicLen;
		}
		else
		{
			return 0;
		}
	}
}

//从数组装载方程到分配的逻辑方程参数缓冲区， 返回参数长度(方便相同的自检)
DWORD _LoadDefaultLogicPar()
{

	MakeAlarmRpt(R_LOGICERR);
	DWORD dwParaOffset = (DWORD)g_LogicParaBuf;
	
	s_pCommonFileHead = (_TCommonFileHead*)dwParaOffset;
	memcpy(s_pCommonFileHead, (void*)&CommonFile_Head, sizeof(CommonFile_Head));

	s_pLogicParaHead = (kTLogicPara_Head*)(s_pCommonFileHead+1);
	memcpy(s_pLogicParaHead, (void*)&Logic_Head, sizeof(Logic_Head));

	s_pExpression = (kTLogicPara_Expression*)(s_pLogicParaHead+1);
	memcpy(s_pExpression, Logic_Expression, sizeof(Logic_Expression));

	s_pCalcNode =(kTLogicPara_CalcNode*)((BYTE*)s_pExpression + sizeof(Logic_Expression));
	memcpy(s_pCalcNode, Logic_Node, sizeof(Logic_Node));

	//校验码
	s_pwChecksum = (WORD*)((BYTE*)s_pCalcNode + sizeof(Logic_Node));
	*s_pwChecksum =  wChksumCpp;
	return (DWORD(s_pwChecksum) - dwParaOffset +2); //参数总长度
}


//2012-5-25 函数不再使用了
/*

//冲逻辑方程参数缓冲区重置逻辑参数
BOOL _ResetLogicPar(DWORD dwFileLen)
{
	DWORD i;
	DWORD dwParaOffset = (DWORD)g_LogicParaBuf;
	
	//读通用文件头
	s_pCommonFileHead = (_TCommonFileHead*)dwParaOffset;

	//判断文件大小
	if(dwFileLen != s_pCommonFileHead->dwSize)
	{
		LogError("ResetLogicPar",FILE_LINE,"FileLen  error %d %d", dwFileLen, s_pCommonFileHead->dwSize);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	dwParaOffset += sizeof(_TCommonFileHead);
	//计算文件头校验码是否正确
	WORD wCheck = DAE_CheckSum(g_LogicParaBuf, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);

	if(dwCheck != s_pCommonFileHead->dwCheck)
	{
		LogError("ResetLogicPar",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,s_pCommonFileHead->dwCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}

	//计算整个校验码是否正确
	wCheck = DAE_CheckSum(g_LogicParaBuf, s_pCommonFileHead->dwSize - 2);
	WORD* pFileCheck = (WORD*)(g_LogicParaBuf+s_pCommonFileHead->dwSize -2);
	if(wCheck != *pFileCheck)
	{
		LogError("ResetLogicPar",FILE_LINE,"File checksum error 0x%X  0x%X",
			wCheck,*pFileCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	// 读取逻辑方程参数文件头部 
	s_pLogicParaHead = (_TLogicPara_Head*)dwParaOffset;

	//2012-5-25 文件结构判断
	if(!_CheckFileFlage(s_pCommonFileHead,s_pLogicParaHead))
		return FALSE;

	
	if (s_pLogicParaHead->dwRelayWordNum != RELAY_WORD_NUM) 
	{
		LogError("ResetLogicPar",FILE_LINE,"relayWordNum err 0x%X 0x%X ",
			s_pLogicParaHead->dwRelayWordNum, RELAY_WORD_NUM);
		return FALSE;
	}
	if (s_pLogicParaHead->dwRelayWordNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwRelayWordNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwExpressionNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwExpressionNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwCalcNodeNum == 0)
	{
		LogError("ResetLogicPar",FILE_LINE,"dwCalcNodeNum = 0");
		return FALSE;
	}
	dwParaOffset += sizeof(_TLogicPara_Head);


	// 读取表达式描述结构 
	s_pExpression = (_TLogicPara_Expression*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwExpressionNum; i++)
	{
		if (s_pExpression[i].wCalcNodeNum == 0) 
		{
				
				LogError("ResetLogicPar",FILE_LINE,"s_pExpression[i].wCalcNodeNum == 0");
				return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_Expression) * s_pLogicParaHead->dwExpressionNum;

	// 读取执行节点描述结构 
	s_pCalcNode = (_TLogicPara_CalcNode*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwCalcNodeNum; i++)
	{
		if (s_pCalcNode[i].wRelayIndex >= RELAY_WORD_NUM)
		{
			
			LogError("ResetLogicPar",FILE_LINE,"s_pCalcNode.wRelayIndex >= RELAY_WORD_NUM");
			return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_CalcNode) * s_pLogicParaHead->dwCalcNodeNum;

	
	//读取校验码
	s_pwChecksum = (WORD*)dwParaOffset;	
	
	//各个结构都没错误才能走到这里
	g_PLCStatus = PLC_SYS_OK;
	return TRUE;
}*/

// 复位可编程逻辑模块,在逻辑方程参数下载后调用
//继电器字复位中与方程参数无关的直接复位， 与方程参数有关的读方程正确后复位
BOOL ResetLogicParseModule(BOOL bIsNewPLCDownLoad)
{
	//2011-12-20 复位完成前必须是PLC_STATUS_ERR， 防止LogicProcess在逻辑初始化
	//未完成前就运行逻辑方程 
	g_PLCStatus = PLC_STATUS_ERR;
	
	g_dwPLCDebugStep1 = 0;
	g_dwPLCDebugStep2 = 0;
	
	s_pCommonFileHead = NULL;
	
	s_pLogicParaHead = NULL;

	s_pExpression = NULL;

	s_pCalcNode = NULL;

	// 复位所有继电器字 
	ResetAllRelayWord();

	// 复位时间继电器字描述结构 
	memset((void*)TimeRelayWordInf, 0, sizeof(struct TTimeRelayWordInf) * TIME_RELAY_WORD_NUM); 
	memset((void*)ConstTimeRelayWordInf, 0, sizeof(struct TTimeRelayWordInf) * CONST_TIME_RW_NUM);
	memset((void*)TripRelayWordInf, 0, sizeof(DWORD)* TRIP_RELAY_WORD_NUM);
	memset((void*)PulseRelayWordInf, 0, sizeof(struct TPulseRelayWordInf) * PULSE_RELAY_WORD_NUM);
	memset((void*)byVInValue, 0, VIN_RELAY_WORD_NUM);
	
	memset((void*)byOutPreValue, 0, OUTPUT_RELAY_WORD_NUM);
	memset((void*)nCountRelayBuf, 0, CNT_RELAY_WORD_NUM*2);
	
	//初始化开入继电器参数 2012-5-23 遥信和开入对应关系不随电度配置编号了，不需要该函数了
//@	if(!InitInputRelayPara())
//@		return FALSE;
	

//@	//从eprom中读取逻辑方程到ram中

	//测试， 从缓冲区读取
	#ifdef __PL_WIN_SIMULATION__
	memcpy(g_LogicParaBuf, g_byPLCSource ,__PL_LOGIC_SIZE__);
	#else
	//如果用文件管理的话是读文件内容到ram中
	DWORD dwLogicSize;
	
	char strFileName[256];
	sprintf(strFileName, "%s", "Logic.PLC");
	
	//逻辑方程文件
	
	//2011-12-20 nPLCStatus暂存g_PLCStatus状态
	LONG nPLCStatus;
	
	dwLogicSize = _LoadFileLogicPar(strFileName);
	if(dwLogicSize>0)
	{
		if(!_CheckPLCParam(dwLogicSize))
		{
			nPLCStatus = PLC_STATUS_ERR;
		}
		else
			nPLCStatus = PLC_SYS_OK;
	}
	else
	{
		LogError("ResetLogicParseModule",FILE_LINE,"The Logic Date File fileName=%s Read Err!",strFileName);
		nPLCStatus = PLC_STATUS_ERR;
	}

	//逻辑方程文件错误，使用默认系统逻辑
	if(PLC_STATUS_ERR ==  nPLCStatus)
	{
		dwLogicSize = _LoadDefaultLogicPar();

		if(_CheckPLCParam(dwLogicSize))
		{
			nPLCStatus = PLC_DEFAULT_OK;
			LogError("ResetLogicParseModule",FILE_LINE,"Default logic Ok");
		}
		else
		{
			g_PLCStatus = PLC_STATUS_ERR;
			LogError("ResetLogicParseModule",FILE_LINE,"Default logic ERR");
			return FALSE;
		}
		
	}
	#endif

	//从e2读取信号继电器到内存中
	InitSignalRelayWord(bIsNewPLCDownLoad);

	//从nvram读nvram继电器 2011-9-15
	InitNVRamRelyWord(bIsNewPLCDownLoad);

	//下面两个和方程有关， 必须方程正确了才能初始化
	//初始化比较继电器参数
	if(!InitGERelayWord())
	{
		return FALSE;
	}

	//初始化常数时间继电器
	InitConstTimeRelayWord();

	UpDateInPut();
	UpDateCFG();  //kang 修改为在保护任务中接收到配置等修改消息才更新配置和压板
	UpDateSW(); 	//   在OnRelayChange中调用

//kang 2012-5-29 强制刷新一次开出，使开出与继电器状态相符合
	for(int i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		byOutPreValue[i] = ~byRelayWordValue[i+OUTPUT_RELAY_WORD_START];
	UpDateOutPut();
		
	
	//2011-12-20 初始化完成了
	g_PLCStatus = nPLCStatus; 
	return TRUE;
}

// 复位(清)所有的继电器字 
void ResetAllRelayWord(void)
{
	// 清继电器字变量 
	memset((void*)byRelayWordValue, 0, RELAY_WORD_NUM);

	// 注意:需要将TRUE继电器字类型需要设置为1
	byRelayWordValue[1] = 1;
}

// 读继电器字
/*BOOL ReadRelayWord(WORD wRelayWordIndex)
{
	if (wRelayWordIndex >= RELAY_WORD_NUM) return FALSE;
	if ((byRelayWordValue[wRelayWordIndex] & 0x01) == 0x01)
		return TRUE;
	return FALSE;
}*/


// 写继电器字对外接口 
void WriteRelayWord(WORD wRelayWordIndex, BOOL bContext)
{
	if((wRelayWordIndex >= RELAY_WORD_NUM)|| (wRelayWordIndex < 0))
		return;
	if(wRelayWordIndex<2)
		return;
	OS_Protect(0);
	WORD wExcType = RelayWordTypeList[wRelayWordIndex-2];
	s_pfnWriteList[wExcType](wRelayWordIndex, bContext);
	OS_UnProtect(0);
}

//清信号继电器
void ResetSignalRelayWord(WORD wRelayWordIndex, WORD wRsv)
{
	if (wRelayWordIndex < SIGNAL_RELAY_WORD_START) 
	{
		LogError("ResetSignalRelayWord",FILE_LINE,"wRelayWordIndex < SIGNAL_RELAY_WORD_START");
		return;
	}
	
	if (wRelayWordIndex >= SIGNAL_RELAY_WORD_START + SIGNAL_RELAY_WORD_NUM)
	{
		LogError("ResetSignalRelayWord",FILE_LINE,"wRelayWordIndex 越上限");
		return;
	}

	WORD wIndex = wRelayWordIndex - SIGNAL_RELAY_WORD_START;
	if( (byRelayWordValue[wRelayWordIndex] & 0x01) == 0x1)
	{
//		BYTE byVal = 0;
//		BYTE byNotVal = ~byVal;
		//保持到e2
//		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex, 1, &byVal);
//		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex+1, 1, &byNotVal);
		
		//不能在逻辑方程任务中写EEPROM，通知数据库修改
		OS_PostMessage(g_hDbaseTaskID, SM_WRITESINGNALRW, EEPROM_RELAY_SINGAL+2*wIndex,0x01,0,0);
	}
	
	// 在高位纪录上次状态,用于沿判断 
	byRelayWordValue[wRelayWordIndex] <<= 4;	
	// 清继电器字 
	byRelayWordValue[wRelayWordIndex] &= 0xF0;


}

//清跳闸继电器
void ResetTripRelayWord(WORD wRelayWordIndex , WORD wRsv)
{
	if (wRelayWordIndex < TRIP_RELAY_WORD_START) return;
	if (wRelayWordIndex >= TRIP_RELAY_WORD_START + TRIP_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - TRIP_RELAY_WORD_START;

	DWORD dwTimeGap = dwRelayMSCnt - TripRelayWordInf[wIndex];
	
	if ( dwTimeGap >= *(TripSetting[wIndex]) ) //保持时间到
	{
		// 清继电器字 
		byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 
		
		byRelayWordValue[wRelayWordIndex] &= 0xF0;

	}
	return;
}

//清时间继电器字(时间继电器和常数时间继电器都用这个函数)
void ResetTimeRelayWord(WORD wRelayWordIndex , WORD wRsv)
{
	
	WORD wIndex;
	if((wRelayWordIndex >=TIME_RELAY_WORD_START)&&(wRelayWordIndex < TIME_RELAY_WORD_START + TIME_RELAY_WORD_NUM))
	{
		wIndex = wRelayWordIndex - TIME_RELAY_WORD_START;
		TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A; //分状态
		TimeRelayWordInf[wIndex].dwTime = 0;//保护用毫秒计数器;
	}
	else if((wRelayWordIndex >=CONST_TIME_RW_START)
			&&(wRelayWordIndex < CONST_TIME_RW_START + CONST_TIME_RW_NUM))
	{
		wIndex = wRelayWordIndex - CONST_TIME_RW_START;
		ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A; //分状态
		ConstTimeRelayWordInf[wIndex].dwTime = 0;//保护用毫秒计数器;
	}
	else 
		return;
	
	//清继电器字，时间继电器复归不保存上次状态了
	byRelayWordValue[wRelayWordIndex] =0;
	

}
void ResetNVRamHoldRelayword(WORD wRelayWordIndex, WORD wRsv)
{
	// 清单个Nvram自保持继电器字 
	// 序号边界判断 
	if (wRelayWordIndex < NKEEP_RW_START) return;
	if (wRelayWordIndex >= NKEEP_RW_START + NKEEP_RW_NUM) return;

	// 清继电器字 ，Hold复归还保存上一次状态
	BYTE byOldValue;
	byOldValue = byRelayWordValue[wRelayWordIndex];
	byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 
		
	byRelayWordValue[wRelayWordIndex] &= 0xF0;

	if(byOldValue != byRelayWordValue[wRelayWordIndex] )
	{
		
		//有变化则写到nvram中
		WORD wNHIndex = wRelayWordIndex - NKEEP_RW_START;
		if(wNHIndex >= NKEEP_RW_NUM)
			return;
		g_pByNKeepRam[2*wNHIndex]= byRelayWordValue[wRelayWordIndex];
		g_pByNKeepRam[2*wNHIndex+1] = ~byRelayWordValue[wRelayWordIndex];	
	}
}


// 清自保持继电器字 
void ResetHoldRelayWord(WORD wRelayWordIndex, WORD wRsv)
{
	if (wRelayWordIndex == CLEAR_ALL_KEEP_RELAY_WORD)
	{
		// 清所有的自保持继电器字 
		int i = 0;
		for (i = KEEP_RELAY_WORD_START; i < KEEP_RELAY_WORD_START + KEEP_RELAY_WORD_NUM; i++)
		{
			byRelayWordValue[i] <<= 4;	// 在高位纪录上次状态,用于沿判断 

			byRelayWordValue[i] &= 0xF0;
		}
	}
	else
	{
		// 清单个自保持继电器字 
		// 序号边界判断 
		if (wRelayWordIndex < KEEP_RELAY_WORD_START) return;
		if (wRelayWordIndex >= KEEP_RELAY_WORD_START + KEEP_RELAY_WORD_NUM) return;

		// 清继电器字 ，Hold复归还保存上一次状态
		byRelayWordValue[wRelayWordIndex] <<= 4;	// 在高位纪录上次状态,用于沿判断 
		
		byRelayWordValue[wRelayWordIndex] &= 0xF0;

	}
}

//105 保护动作报告
void Report_Action(WORD wSeqID, WORD wReportIndex)
{
	TRelayEventItem RelayEventItem;
	
	ReadAbsTime(&RelayEventItem.stTime);	
	RelayEventItem.bStatus  = 1;	//动作
	RelayEventItem.nINF	 = ReportInf[wReportIndex].wInf;//INF号
	for(int j=0;j<ReportInf[wReportIndex].dwFialureNum ;j++)
		RelayEventItem.FD_Value[j] = *(ReportInf[wReportIndex].pFailureData[j]);
		
	DB_RelayAction(wSeqID,&RelayEventItem);	//向数据库中写入动作报告

}


//104 保护启动报告
void Report_Start(WORD wSeqID, WORD wReportIndex)
{
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(wSeqID,ReportInf[wReportIndex].wInf,&AbsTime,TRUE);
	
}

//2011-12-13 增加参数为inf号的报告函数
void Report_StartByInf(WORD wSeqID, WORD wInf)
{
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(wSeqID,wInf,&AbsTime,TRUE);
}
//106 结束报告
void Report_End(WORD wSeqID, WORD wRsv)
{
	DB_RelayEnd(wSeqID);
//	bIsRepStarted = FALSE;
}
//107 告警报告
void Report_Alarm(WORD wReportIndex, WORD wRsv)
{
	TRelayEventItem RelayEventItem;
	ReadAbsTime(&RelayEventItem.stTime);	
	RelayEventItem.bStatus  = 1;	//动作
	RelayEventItem.nINF	 = ReportInf[wReportIndex].wInf;//INF号
	for(int j=0;j<ReportInf[wReportIndex].dwFialureNum ;j++)
		RelayEventItem.FD_Value[j] = *(ReportInf[wReportIndex].pFailureData[j]);
	
	DB_RelayAlarm(&RelayEventItem);
}
//2011-12-13 增加参数为inf号的报告函数
void Report_AlarmByInf(WORD wInf, WORD wRsv)
{
	TRelayEventItem RelayEventItem;
	ReadAbsTime(&RelayEventItem.stTime);	
	RelayEventItem.bStatus  = 1;	//动作
	RelayEventItem.nINF	 = wInf;//INF号
	int nCount = DB_GetRENum(); //(sizeof(ReportInf))/sizeof(ReportInf[0]);
	WORD wReportIndex = WORD(-1);

	//通过inf号找到报告顺序号
	for(int i=0; i<nCount; i++)
	{
		if(ReportInf[i].wInf == wInf)
		{
			wReportIndex = i;
			break;
		}
			
	}
	if(wReportIndex != WORD(-1))
	{
		for(int j=0;j<ReportInf[wReportIndex].dwFialureNum ;j++)
			RelayEventItem.FD_Value[j] = *(ReportInf[wReportIndex].pFailureData[j]);
	}
	
	DB_RelayAlarm(&RelayEventItem);
}


//108 软压板投退函数
// wSWRelayIndex 压板继电器编号
// WORD value 0，1 退或者投
void SW_OnOff(WORD wSWRelayIndex, WORD value)
{
	if(wSWRelayIndex <SW_RELAY_WORD_START) return;
	if(wSWRelayIndex >SW_RELAY_WORD_START + SW_RELAY_WORD_NUM) return;

	//值相同， 不需要修改
	if(_ReadRelayWord_Normal(wSWRelayIndex) == value)
		return;
	
	WORD wSWIndex = SWIndexMap[wSWRelayIndex-SW_RELAY_WORD_START];
	(value) ? (DB_SetSWStatus(wSWIndex, SW_STATUS_SET)):(DB_SetSWStatus(wSWIndex, SW_STATUS_CUT));
	DB_SetSWStatusConfirm(wSWIndex, DB_EXECUTE);
}


//逻辑方程系统函数
//@TypeID	TypeName	TypeLabel	Describe
//@99	跳转继电器	JMP	跳转函数， 参数为标号或行号
//@100	复归自保持继电器	RH	复归自保持继电器函数， 参数为自保持继电器
//@101	复归时间继电器	RT	复归时间继电器函数， 参数为时间继电器
//@102	复归跳闸继电器	RTR	参数为跳闸继电器
//@103	复归信号继电器	RS	信号继电器复归函数， 参数为信号继电器序号
//@104 启动报告	RSTART	启动报告， 没有参数
//@105 动作报告	RACTION 动作报告， 参数为报告序号
//@106 报告结束	REND	报告结束，	没有参数
//@107 告警报告	RALARM	告警报告， 参数为报告序号
//@108	压板投退	    OFSL	    压板投退函数， 参数为压板继电器宏，及投或退ON/OFF

static RELAY_SYSFUN_PTR s_pfnSysFunList[] =
{
	ResetHoldRelayWord, //100 复归自保持
	ResetTimeRelayWord, //101 复归时间
	ResetTripRelayWord, //102 复归跳闸继电器
	ResetSignalRelayWord, //103 复归信号继电器
	Report_Start, //104 保护启动报告
	Report_Action, //105 保护动作报告
	Report_End, //106 结束报告
	Report_Alarm, //107 告警报告
	SW_OnOff, //108 压板投退函数
	ResetNVRamHoldRelayword, //109 复归不掉电自保持
	IncreaseCount, //110 递增
	DecreaseCount, //111 递减
	ResetCount, //112 复归计数继电器
};



/****************************************************************************************
 * 
 * 功能描述:  解释执行逻辑方程(一遍)
 * 
 * 参数说明: 
 *			- DWORD* CurLine
 *			  类型: 
 *			  说明: 当前执行到第几行了
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
	void RunLogicParseModule(DWORD* CurLine)
	{
		DWORD i = 0, j = 0;
		BOOL bResult = FALSE;		/* 执行结果 */
		DWORD dwRelayIndex = 0; 	/* 继电器字索引号 */
		DWORD dwCalcEndIndex = 0;
		DWORD dwResultRelayWordType = 0;
		WORD wResultRelayWordIndex = 0;
		struct _TLogicPara_CalcNode* p = NULL;	/* 当前执行节点 */
		DWORD wTmp,wTmp2;
		BYTE mask[] = {0x01, 0x01, 0x11, 0x11, 0x11, 0x11};
		BYTE flag[] = {0x01, 0x00, 0x01, 0x10, 0x01, 0x10};
		DWORD dwIndex;
		
		struct _TLogicPara_Head* pLogicParaHead = s_pLogicParaHead; 	// 逻辑方程参数文件头部 
		struct _TLogicPara_Expression* pExpression = s_pExpression; 	// 表达式描述 
		struct _TLogicPara_CalcNode* pCalcNode = s_pCalcNode;			// 执行节点描述 
	
		if((pLogicParaHead == NULL)||(pExpression == NULL)||(pCalcNode ==NULL))
			return;
		
		//如果是调试逻辑方程
		if((g_dwPLCDebugStep1 == PLC_DEBUG_ST1)&&(g_dwPLCDebugStep2 == PLC_DEBUG_ST2))
		{
			DebugRunLogic(CurLine);
			return;
		}
	
	
		/* 循环所有的表达式 */
		for (i = 0; i < pLogicParaHead->dwExpressionNum; i++)
		{
	
	//@ 	//以下为了在windows下调试用
#ifdef __PL_WIN_SIMULATION__
			BOOL ret;
			ret = GetMessage(&msg, NULL, WM_USER, WM_USER+100);
			if(g_bIsEndThread)
			{
				AfxEndThread(0);
			}
#endif
	//@ 	//调试用的
	
			dwCalcEndIndex = pExpression[i].wStartCalcNodeIndex + pExpression[i].wCalcNodeNum;
	
			/* 循环执行当前表达式的所有执行节点 */
			for (j = pExpression[i].wStartCalcNodeIndex; j < dwCalcEndIndex; j++)
			{
				/* 获取当前继电器字内容 */
				p = &(pCalcNode[j]);
				dwRelayIndex = p->wRelayIndex;
	//			bResult = s_pfnReadList[p->dwGetType](dwRelayIndex);
	
				dwIndex = p->wGetType;
				if(dwIndex<4)
				{
					bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
											? TRUE : FALSE;
				}
				else
				{
					bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
											? FALSE : TRUE;
				}
				
				/*
				 * 在(1 + ...)和(0 * ...)的情况时跳跃
				 * 技巧1:合理分配执行类型的内容,使判断简单.
				 * 技巧2:正常流程为:
				 * if (1 + ...) {bResult = 1; j += 跳转偏移;} “+”dwCalcType 10
				 * if (0 * ...) {bResult = 0; j += 跳转偏移;}  ”*" ”dwCalcType 11
				 * if (#) {bResult = 继电器字内容; break;}
				 * 该处合并了各种流程,效率提高,但增加了阅读难度.
				 */
				if ((bResult + p->wCalcType) == 11)
				{
					//if (p->wOffset == (DWORD)-1) break; 2011-12-9应该是word -1，即0xffff
					if (p->wOffset == (WORD)-1) break;
					j += p->wOffset - 1;	/* 注意: 在for循环开始处j++ */
				}
				
	
			}
	
			dwResultRelayWordType = pExpression[i].wResultRelayWordType;
			wResultRelayWordIndex = pExpression[i].wResultRelayWordIndex;
	
			if (dwResultRelayWordType == PULSE)
			{
				PulseRelayWordInf[wResultRelayWordIndex-PULSE_RELAY_WORD_START].dwTimeParam= pExpression[i].wParm;
			}
	
			if(dwResultRelayWordType < JMP)  //继电器写类型
			{
				// 将执行结果输出到继电器字中 
				dwRelayIndex = pExpression[i].wResultRelayWordIndex;
				s_pfnWriteList[pExpression[i].wResultRelayWordType](dwRelayIndex, bResult);
			}
	
			else if(dwResultRelayWordType == JMP)// 跳转继电器
			{
				  if(bResult)
				  {
					  wTmp = pExpression[i].wParm;
					  i+= wTmp -1; //跳几行, -1应为循环会自动+1
				  }
			}
	
			else if(dwResultRelayWordType < USERFUN_TYPE_BASE ) //逻辑方程系统函数
			{
				if(bResult)
				{
					  wTmp = pExpression[i].wParm;
					 wTmp2 = pExpression[i].wParm2;
					 s_pfnSysFunList[dwResultRelayWordType-SYSFUN_TYPE_BASE](wTmp, wTmp2);
				}
			}
	
			else if(dwResultRelayWordType >= USERFUN_TYPE_BASE) //逻辑方程自定义函数类型
			{
				wTmp = pExpression[i].wParm;
				wTmp2 = pExpression[i].wParm2;
				s_pfnUserFunList[dwResultRelayWordType-USERFUN_TYPE_BASE](wTmp,wTmp2, bResult);
	
			}
	
	
		}
		//2011-12-13更新
		//运行态时实时行号没有意义，为了加快逻辑速度，在转完一圈(而不是向调试时一样每一句)更新一次行号
		if(pLogicParaHead->dwExpressionNum !=0)
			*CurLine = (i+1)%(pLogicParaHead->dwExpressionNum);

	}



/****************************************************************************************
 * 
 * 功能描述: 录波继电器值
 * 
 * 参数说明: 
 *			- WORD* pwRelayIndexList
 *			  类型: 
 *			  说明: 继电器序号列表 格式 （总继电器个数n， 继电器1序号， 继电器2序号,..继电器n序号
 *
 *			- BYTE* pDst
 *			  类型: 
 *			  说明: 录波数据数组
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
BOOL RecordRelayWord(WORD* pwRelayIndexList, LONG* pDst)
{
	WORD wCount;
	WORD byBitNo = 0;
	int i;

	wCount = pwRelayIndexList[0];

	memset(pDst,0,(wCount+15)/16*sizeof(DWORD));
	
	for(i=1; i<=wCount; i++)
	{
		byBitNo = i-1;
			
		if(ReadRelayWord(pwRelayIndexList[i]))
			pDst[byBitNo/16] |= 0x01<<(byBitNo%16);
		else
			pDst[byBitNo/16] &= ~(0x01<<(byBitNo%16));
	}
	return TRUE;

}

//写传动
void CtrlTest(WORD wNo, BOOL  bIsStart)
{
	if (wNo >= CTRLTEST_RELAY_WORD_NUM) return;
	
	WORD wIndex = wNo + CTRLTEST_RELAY_WORD_START;

	_WriteRelayWord(wIndex, bIsStart);

}

//写遥控预置继电器
void RemoteCtrlPre(WORD wRCNum, BOOL bContext)
{
//	if (wRCNum < RCP_RELAY_WORD_START) return;
	if (wRCNum >= RCP_RELAY_WORD_NUM) return;
	
	WORD wIndex = wRCNum + RCP_RELAY_WORD_START;
	
	_WriteRelayWord(wIndex, bContext);

}

//读反校继电器
BOOL RemoteCtrlVer(WORD wRCNum )
{
	if (wRCNum >=  RCV_RELAY_WORD_NUM) return FALSE;

	WORD wIndex = wRCNum + RCV_RELAY_WORD_START;
	return ReadRelayWord(wIndex);

}

 /****************************************************************************************
  * 
  * 功能描述: //写遥控执行继电器
  * 
  * 参数说明: 
  * 		 - WORD wRCNum
  * 		   类型: 
  * 		   说明: 遥控序号
  *
  *
  * 		 - BOOL bContext
  * 		   类型: 
  * 		   说明: 写入值 TRURE 或 FALSE
  *
  * 返回代码: 
  * 
  * 其它: 
  * 
  *****************************************************************************************/

 void RemoteCtrlExe(WORD wRCNum, BOOL bContext)
{
	if (wRCNum >= RCE_RELAY_WORD_NUM) return;
	
	WORD wIndex = wRCNum + RCE_RELAY_WORD_START;
	
	_WriteRelayWord(wIndex, bContext);

}

//2012-02-13 写当地控制执行继电器 wLCNum是遥控序号
 void LocalCtrlExe(WORD wLCNum, BOOL bContext)
{
	if (wLCNum >= LCE_RELAY_WORD_NUM) return;
	
	WORD wIndex = wLCNum + LCE_RELAY_WORD_START;
	
	_WriteRelayWord(wIndex, bContext);
}


//刷新LED继电器（写Led继电器到LED灯)
void UpDateLed()
{
	
	BOOL value;	
	static BOOL bOldValue[LED_RELAY_WORD_NUM];
	for (int i=0; i<LED_RELAY_WORD_NUM; i++)
	{
		value = ReadRelayWord(i+LED_RELAY_WORD_START);
#if(SYS_CPU_TYPE == CPU_COLDFIRE)
		LEDOnOff(i, value);
#endif
	/*	if(bOldValue[i]!=value)
		{
			LEDOnOff(i, value);
			bOldValue[i] = value;
		}
		*/
	}
	
}


//刷新出口继电器（写出口继电器到出口）
void UpDateOutPut()
{
	WORD i, wBIIndex;;

	BOOL bVal;
	for (i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
	{	
		bVal = ReadRelayWord(i+OUTPUT_RELAY_WORD_START);
//		byPreValue = byOutPreValue[i];
		
		wBIIndex = OutPutMap[i]; //得到第i个出口继电器对应的实际BO号
		
		//策略： 已经prvval==1 && bVal == 0   收出口
		//		 preval==0 && bVal == 1  跳出口	
		if(byOutPreValue[i] != bVal)
		{			
			//ReportMsg("OUT=%d, status=%d, laststatus=%d",i+1,bVal,byOutPreValue[i]);
			RealBOControl(wBIIndex, bVal);
			byOutPreValue[i] = bVal;
		}
	}

}

void UpDateInPut()
{
	WORD i;

	for (i=0; i<INPUT_RELAY_WORD_NUM; i++)
	{	
		//配置成遥信的要读取最新值， 对于配置成电度的不更新(初始化时为0)
//		if(InputRelayWordBI[i] != -1)
//			_WriteRelayWord(i+INPUT_RELAY_WORD_START, DB_LogReadBI(InputRelayWordBI[i]));
//		_WriteRelayWord(i+INPUT_RELAY_WORD_START, DB_LogReadBI(i));
		_WriteRelayWord(i+INPUT_RELAY_WORD_START, ((pSysBI[i].bStatus&0x80) ? TRUE : FALSE) );

		
	}
}

//刷新软压板（读软压板到继电器字）
void UpDateSW()
{
	WORD i;
	BYTE byStatus;

	for (i=0; i<SW_RELAY_WORD_NUM; i++)
	{
		byStatus = *(SWMap[i]);
		if (SW_STATUS_SET == byStatus)
		{
			_WriteRelayWord(i+SW_RELAY_WORD_START, TRUE);
		}
		else
		{
			_WriteRelayWord(i+SW_RELAY_WORD_START, FALSE);
		}


	}

}

//刷新配置（读配置到继电器字)
void UpDateCFG()
{
	WORD i;
	BYTE byStatus;
	
	for (i=0; i<CFG_RELAY_WORD_NUM; i++)
	{
		byStatus = *(CFGMap[i]);
			if (CFG_STATUS_SET == byStatus)
			{
				_WriteRelayWord(i+CFG_RELAY_WORD_START, TRUE);
			}
			else
			{
				_WriteRelayWord(i+CFG_RELAY_WORD_START, FALSE);
			}
	}

}

//更新调试继电器(从buf读到继电器字中澹)
void UpDataDebugRelay()
{

	int i;
	BYTE byValue;

	for (i=0; i<VIRTURAL_RELAY_WORD_NUM; i++)
	{	
		byValue = byDebugRelayBuf[i];
		if (byValue)
		{
			_WriteRelayWord(i+VIRTUAL_RELAY_WORD_START, TRUE);
		}
		else
			_WriteRelayWord(i+VIRTUAL_RELAY_WORD_START, FALSE);
		
		
	}
	
}
//接收链接继电器从接受缓冲区更新到继电器字
void UpDataRXLinkRelay()
{
	
		WORD i;

		for (i=0; i<RXLINK_RW_NUM; i++)
		{	
			_WriteRelayWord(i+RXLINK_RW_START, byRxLinkBuf[i]);
		}
	
}
//从继电器字更新到发生链接缓冲区, 只要0，1值， 不要脉冲
void UpDataTXLinkRelay()
{
	WORD i;
	BYTE byValue;
	for (i=0; i<TXLINK_RW_NUM; i++)
	{	  
		byValue = ReadRelayWord(i+TXLINK_RW_START);
		byTxLinkBuf[i] = byValue;
	}
}

//更新对于逻辑方程来说是输入的继电器值(开入， 配置， 压板,调试继电器)
inline void UpDateInputRelayWord()
{
//	UpDateCFG();  kang 修改为在保护任务中接收到配置等修改消息才更新配置和压板
//	UpDateSW();        在OnRelayChange中调用

	UpDateInPut();
//	UpDataDebugRelay();
	
	UpDataRXLinkRelay();
}

//更新对于逻辑方程来说是输出的继电器值(开出， led,发送链接继电器)
inline void UpDateOutputRelayWord()
{
#if (SYS_CPU_TYPE==CPU_COLDFIRE)    //1300 不需要led功能
	UpDateLed();
#endif
	UpDateOutPut();
	UpDataTXLinkRelay();
}

//kangqian    05/04/2011  ReadAllVirDI 返回的虚拟开入改为按位保存，bit0表示vin0

STATUS ReadAllVirDI(BYTE *pBuf)
{	
	BYTE byBitNo;
	int i;

//	if(isVirDIValide == FALSE)
//		return  FAILURE;
	
	for(i=0; i<VIN_RELAY_WORD_NUM; i++)
	{
		byBitNo = i;
			
		if(byVInValue[i])
			pBuf[byBitNo/8] |= 0x01<<(byBitNo%8);
		else
			pBuf[byBitNo/8] &= ~(0x01<<(byBitNo%8));
	}

	return SUCCESS;
	
}

DWORD GetPLCDBVer()
{
	if(s_pCommonFileHead)
		return s_pCommonFileHead->dwVersion;
	else
		return 0;
}
//读逻辑方程校验码
WORD GetPLCCheckSum()
{
	if(s_pwChecksum)
		return *s_pwChecksum;
	else
		return 0;
}
//逻辑自检
WORD PLCSelfCheck()
{
	if(g_PLCStatus == PLC_STATUS_ERR)
		return 0xFFFF;
	//文件头
	//计算文件头校验码 必须先校验文件头， 文件头不校验的话s_pCommonFileHead->dwSize是不可信的
	WORD wCheck = DAE_CheckSum(g_LogicParaBuf, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);

	if(dwCheck != s_pCommonFileHead->dwCheck)
	{
		LogError("PLCSelfCheck",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,s_pCommonFileHead->dwCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return 0xFFFF;
	}

	//文件
	wCheck = DAE_CheckSum(g_LogicParaBuf+sizeof(TCommonFileHead),
				s_pCommonFileHead->dwSize - 2-sizeof(TCommonFileHead));
	if(wCheck != *s_pwChecksum)
	{
		g_PLCStatus = PLC_STATUS_ERR;

		//如果校验错误，则重启逻辑方程
		LogError("PLCSelfCheck",FILE_LINE,"File checksum error 0x%X, 0x%X", wCheck,*s_pwChecksum);
		ResetLogicParseModule();
	}
	return 0;
}
/************************************************************
	功能：逻辑方程处理,外部运行逻辑方程时循环调用该函数
	参数：无
	返回：无
************************************************************/
void LogicProcess(void)
{	
	static DWORD dwCnt=0;

	if(g_PLCStatus == PLC_STATUS_ERR)
		return;
	
	//逻辑方程处理
	if((byRelayWordValue[1] != 0x1)||(byRelayWordValue[0]!=0x0))
	{
		byRelayWordValue[1] = 0x1;
		LogError("CRelayTask",FILE_LINE,"RelayWord True1 changed");
	}
	
	UpDateInputRelayWord();
	RunLogicParseModule(&g_dwPLCCurLine);//使g_dwPLCCurLine随着方程运行更新
	UpDateOutputRelayWord();	

	if(isVirDIValide == FALSE)
	{
		dwCnt++;
		if(dwCnt>100)
		{
			isVirDIValide = TRUE;
		}
	}
}

/************************************************************
	功能：初始化信号继电器保存在eeprom中的值为0， 在更新逻辑方程时需要调用
	参数：无
	返回：无
************************************************************/
void FormatSignalEEprom()
{
	if(SIGNAL_RELAY_WORD_NUM <= 0)
		return;
	BYTE byVal = 0;
	BYTE byNotVal = ~byVal;
		
	BYTE bBuf[SIGNAL_RELAY_WORD_NUM*2+1];
	int i=0;
	for(int i=0; i<SIGNAL_RELAY_WORD_NUM; i++)
	{
		bBuf[2*i] = byVal;
		bBuf[2*i+1] =  byNotVal;
	}
	EEPROM_Write(EEPROM_RELAY_SINGAL, SIGNAL_RELAY_WORD_NUM*2, bBuf);
	
	LogError("Logic", FILE_LINE, "All signal value  in eeprom reset", i);
}

/************************************************************
	功能：读取闪烁led的值
	参数：
	     WORD wIndex[in] 要读的led序号， 为LOGIC_READ_ALL_VALUE时读取所有的led
	     BYTE* pByFrequency[out] 返回参数头指针
	     BYTE* pByValue[out] 返回值头指针
	返回：LONG  0:读取成功 1:读取失败
************************************************************/

LONG GetFlashLedValue(WORD wIndex, BYTE* pByFrequency, BYTE* pByValue)
{
	if(LOGIC_READ_ALL_VALUE == wIndex)
	{
		for(WORD i=0; i<LED_RELAY_WORD_NUM; i++)
		{
			pByFrequency[i] = (byLedFlashBuf[i]>>4)&0x0F;
			pByValue[i] = byLedFlashBuf[i]&0x01; //最低位为值
		}
		return 0;
		
	}
	else if(wIndex<LED_RELAY_WORD_NUM)
	{
		
		*pByFrequency = (byLedFlashBuf[wIndex]>>4)&0x0F;
		*pByValue = byLedFlashBuf[wIndex]&0x01; //最低位为值
		return 0;
	}
	return 1;
	
}



/************************************************************
	功能：读取发送链接继电器的值
	参数：
	     WORD wStartIndex[in] 要读的发送链接继电器起始序号
	     WORD wCount[in] 要读的发送链接继电器个数
	     
	     BYTE* pByValue[out] 指向返回值的指针
	返回：LONG  0:读取成功 1:读取失败
************************************************************/
LONG GetTxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue)
{
	
	if((wStartIndex+wCount)>TXLINK_RW_NUM) return 1;
	
	memcpy(pByValue, &(byTxLinkBuf[wStartIndex]),wCount);
	return 0;
}

/************************************************************
	功能：设置接收链接继电器的值
	参数：
	     WORD wStartIndex[in] 要写的接收链接继电器起始序号
	     WORD wCount[in] 要写的接收链接继电器个数
	     
	     BYTE* pByValue[in] 指向设置值的指针
	返回：LONG  0:读取成功 1:读取失败
************************************************************/

LONG SetRxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue)
{
	if((wStartIndex+wCount)>RXLINK_RW_NUM) return 1;
	
	memcpy(&(byRxLinkBuf[wStartIndex]),pByValue, wCount);
	return 0;
}

void WriteDebugRW(WORD wIndex,BYTE byVal)
{
	WORD wDebugIndex;

	if(wIndex < VIRTUAL_RELAY_WORD_START)
		return;

	wDebugIndex = wIndex - VIRTUAL_RELAY_WORD_START;

	if(wDebugIndex >= VIRTURAL_RELAY_WORD_NUM)
		return;
	
	byDebugRelayBuf[wDebugIndex] = byVal;
}
/************************************************************
	功能：读继电器的完全值，不只是0，1状态，高位还保存了上次的状态
	参数：
	     WORD wIndex[in] 继电器序号
	     
	返回：BYTE  继电器完全值，其中bit0是当前值，bit4是上次的值
************************************************************/
BYTE ReadRWFullValue(WORD wIndex)
{
	if(wIndex >= RELAY_WORD_NUM)
		return BYTE(-1);
	return byRelayWordValue[wIndex];
}

const WORD GetPLCDebugBreakPoint()
{
	return g_dwPLCBreakPoint;
}
const WORD GetPLCCurLine()
{
	return g_dwPLCCurLine;
}

void SetPLCDebugBreakPoint(DWORD dwVal)
{
	g_dwPLCBreakPoint = dwVal;
}

void SetPLCDebugStep1(DWORD dwVal)
{
	g_dwPLCDebugStep1 = dwVal;
}
void SetPLCDebugStep2(DWORD dwVal)
{
	g_dwPLCDebugStep2 = dwVal;
}
void StopPLCDebug()
{
	g_dwPLCDebugStep1 = 0;
	g_dwPLCDebugStep2 = 0;
	g_dwPLCBreakPoint = 0;
}

void SetPLCDebugLoop(DWORD dwVal)
{
	g_dwPLCDebugLoop = dwVal;
}
void SetPLCDebugOneStep(DWORD dwVal)
{
	g_dwPLCDebugOneStep = 1;
}
/****************************************************************************************
 * 
 * 功能描述:  调试执行逻辑方程(一遍) 
 * 
 * 参数说明: 
 *			- long* CurLine
 *			  类型: 
 *			  说明: 当前执行到第几行了
 *
 * 返回代码: 
 * 
 * 其它: 其实主要执行部分和RunLogicParseModule完全相同，只有起始行号和结束行号istart和iend
 与正常运行不同，但是为了避免调试对正常运行逻辑造成 实际运行效率及代码可读性的降低，
 复制代码形成独立的RunDebugLogic函数 而不是在RunLogicParseModule增加额外代码
 1.正常运行时是从0行运行到最后一行，不管中间怎么调转(jmp只支持向下跳转，
 			所以不会来回运行，只能是带跳跃的从0运行到最后一行
 		 2.调试单步执行 判断g_dwPLCBreakPoint为-1 是单步运行一步设置iend是istart+1,不管是否有跳转，都是只执行一行，执行完修正
 		 g_dwPLCBreakPoint 为g_dwPLCCurLine
 		 3.运行到断点 iEnd为断点，1)如果g_dwPLCBreakPoint小于iStart, 或者要转一圈的，设end为总行号，本次运行到
 		   末行，下次循环来从末行运行到断点行。
 		   2)其他则设iend为断点
 * 
 *****************************************************************************************/
void DebugRunLogic(DWORD* CurLine)
{
	DWORD i = 0, j = 0;
	BOOL bResult = FALSE;		/* 执行结果 */
	DWORD dwRelayIndex = 0;		/* 继电器字索引号 */
	DWORD dwCalcEndIndex = 0;
	DWORD dwResultRelayWordType = 0;
	WORD wResultRelayWordIndex = 0;
	struct _TLogicPara_CalcNode* p = NULL;	/* 当前执行节点 */
	DWORD wTmp,wTmp2;
	BYTE mask[] = {0x01, 0x01, 0x11, 0x11, 0x11, 0x11};
	BYTE flag[] = {0x01, 0x00, 0x01, 0x10, 0x01, 0x10};
	DWORD dwIndex;
	
	struct _TLogicPara_Head* pLogicParaHead = s_pLogicParaHead;		// 逻辑方程参数文件头部 
	struct _TLogicPara_Expression* pExpression = s_pExpression;		// 表达式描述 
	struct _TLogicPara_CalcNode* pCalcNode = s_pCalcNode;			// 执行节点描述 

	if((pLogicParaHead == NULL)||(pExpression == NULL)||(pCalcNode ==NULL))
		return;

	//逻辑方程调试用
	DWORD iStart=0, iEnd=0;

	iStart = g_dwPLCCurLine;//+1;
		//2
	//单步运行，向下运行一步(可能是跳转到其他行，不定是下一行
	if(g_dwPLCDebugOneStep >0)
	{
		iEnd = iStart+1;
	}
	else
	{
		//本次循环先运行到末行，下次从末行运行到断点
		if((g_dwPLCBreakPoint<iStart) //断点在当前行前面，也就是需要先转一圈回来才到断点
			|| ((g_dwPLCBreakPoint == iStart)&&(g_dwPLCDebugLoop>0)) ) //转一圈
		{
			iEnd = pLogicParaHead->dwExpressionNum;
		}
		else
		{
			iEnd = g_dwPLCBreakPoint;
		}
		
		if(g_dwPLCDebugLoop>0)
			g_dwPLCDebugLoop--;
		else
			g_dwPLCDebugLoop = 0;
	}
	/* 循环所有的表达式 */
	for (i = iStart; i <iEnd ; i++)
	{

//@		//以下为了在windows下调试用
#ifdef __PL_WIN_SIMULATION__
		BOOL ret;
		ret = GetMessage(&msg, NULL, WM_USER, WM_USER+100);
		if(g_bIsEndThread)
		{
			AfxEndThread(0);
		}
#endif
//@		//调试用的

		dwCalcEndIndex = pExpression[i].wStartCalcNodeIndex + pExpression[i].wCalcNodeNum;

		/* 循环执行当前表达式的所有执行节点 */
		for (j = pExpression[i].wStartCalcNodeIndex; j < dwCalcEndIndex; j++)
		{
			/* 获取当前继电器字内容 */
			p = &(pCalcNode[j]);
			dwRelayIndex = p->wRelayIndex;
//			bResult = s_pfnReadList[p->dwGetType](dwRelayIndex);

			dwIndex = p->wGetType;
			if(dwIndex<4)
			{
				bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
										? TRUE : FALSE;
			}
			else
			{
				bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
										? FALSE : TRUE;
			}
			
			/*
			 * 在(1 + ...)和(0 * ...)的情况时跳跃
			 * 技巧1:合理分配执行类型的内容,使判断简单.
			 * 技巧2:正常流程为:
			 * if (1 + ...) {bResult = 1; j += 跳转偏移;} “+”dwCalcType 10
			 * if (0 * ...) {bResult = 0; j += 跳转偏移;}  ”*" ”dwCalcType 11
			 * if (#) {bResult = 继电器字内容; break;}
			 * 该处合并了各种流程,效率提高,但增加了阅读难度.
			 */
			if ((bResult + p->wCalcType) == 11)
			{
				//if (p->wOffset == (DWORD)-1) break; 2011-12-9应该是word -1，即0xffff
				if (p->wOffset == (WORD)-1) break;
				j += p->wOffset - 1;	/* 注意: 在for循环开始处j++ */
			}
			

		}

		dwResultRelayWordType = pExpression[i].wResultRelayWordType;
		wResultRelayWordIndex = pExpression[i].wResultRelayWordIndex;

		if (dwResultRelayWordType == PULSE)
		{
			PulseRelayWordInf[wResultRelayWordIndex-PULSE_RELAY_WORD_START].dwTimeParam= pExpression[i].wParm;
		}

		if(dwResultRelayWordType < JMP)  //继电器写类型
		{
			// 将执行结果输出到继电器字中 
			dwRelayIndex = pExpression[i].wResultRelayWordIndex;
			s_pfnWriteList[pExpression[i].wResultRelayWordType](dwRelayIndex, bResult);
		}

		else if(dwResultRelayWordType == JMP)//	跳转继电器
		{
			  if(bResult)
			  {
				  wTmp = pExpression[i].wParm;
				  i+= wTmp -1; //跳几行, -1应为循环会自动+1
			  }
		}

		else if(dwResultRelayWordType < USERFUN_TYPE_BASE ) //逻辑方程系统函数
		{
			if(bResult)
			{
				  wTmp = pExpression[i].wParm;
				 wTmp2 = pExpression[i].wParm2;
				 s_pfnSysFunList[dwResultRelayWordType-SYSFUN_TYPE_BASE](wTmp, wTmp2);
			}
		}

		else if(dwResultRelayWordType >= USERFUN_TYPE_BASE) //逻辑方程自定义函数类型
		{
			wTmp = pExpression[i].wParm;
			wTmp2 = pExpression[i].wParm2;
			s_pfnUserFunList[dwResultRelayWordType-USERFUN_TYPE_BASE](wTmp,wTmp2, bResult);

		}
		if(pLogicParaHead->dwExpressionNum !=0)
			*CurLine = (i+1)%(pLogicParaHead->dwExpressionNum);
		//CurLine表示接下来要执行的行

	}

	if(g_dwPLCDebugOneStep>0)
	{
		g_dwPLCDebugOneStep--;
		g_dwPLCBreakPoint = *CurLine;
	}	
}


void WriteDebugIn(WORD wDebugInIndex,BYTE byVal)
{
	if(wDebugInIndex >= DEBUG_IN_NUM)
		return;

	byDebugInRWBuf[wDebugInIndex] = byVal;
}

BYTE ReadDebugOut(WORD wDebugOutIndex)
{
	
	if(wDebugOutIndex >= DEBUG_OUT_NUM)
		return FALSE1;
	return byDebugOutRwBuf[wDebugOutIndex];
}
//更新调试开入继电器 (从规约缓冲区更新到继电器）2012-02-08
void UpDataDebugInRW()
{
	WORD i;
	
	BOOL bVal;
	for (i=0; i<DEBUG_IN_NUM; i++)
	{
		_WriteRelayWord(i+DEBUG_IN_START,((byDebugInRWBuf[i]&0x01) ? TRUE : FALSE) );
	}
	
}

//更新调试开出继电器 (从继电器更新到规约缓冲区）2012-02-08
void UpDataDebugOutRW()
{
	WORD i, wBIIndex;;
	
	BOOL byVal;
	for (i=0; i<DEBUG_OUT_NUM; i++)
	{	
		byVal = ReadRelayWord(i+DEBUG_OUT_START);
		byDebugOutRwBuf[i] = byVal&0x01;
	}
}
//计数继电器
void ResetCount(WORD wRelayWordIndex, WORD wRsv)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	nCountRelayBuf[wIndex] = 0;
	return;
}
void IncreaseCount(WORD wRelayWordIndex, WORD wOffset)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	nCountRelayBuf[wIndex] += wOffset;
}
void DecreaseCount(WORD wRelayWordIndex, WORD wOffset)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	
	if(nCountRelayBuf[wIndex] <= wOffset)
	{
			nCountRelayBuf[wIndex] = 0;
			return;
	}
	
	nCountRelayBuf[wIndex] -= wOffset;
}
//按继电器总序号读计数值
WORD ReadCountByIndex(WORD wRelayWordIndex)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return 0;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return 0;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	return nCountRelayBuf[wIndex];
}

//按计数继电器号读计数值
WORD ReadCountByNo(WORD CountNo)
{
	if (CountNo >= CNT_RELAY_WORD_NUM) return 0;
	return nCountRelayBuf[CountNo];
}



