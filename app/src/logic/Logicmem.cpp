/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			logicmem.cpp
* 
*   软件模块：
*
* 
*	描述
* 
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2009-1-6		初始创建
* 
*************************************************************************/
//window
//#ifdef __PL_WIN_SIMULATION__
//#include "stdafx.h" 
//#endif

#include "logicDef.h"
#include "Logic.h"
#include "PLConfig.h"
//@#include "PlatFormDef.h"

//在windows中模拟运行逻辑方程时使用
#ifdef __PL_WIN_SIMULATION__
    BYTE _PLDataBuf[10*1024];             /* 在window模拟平台中模拟e2,放逻辑方程的.plc文件的 */
#endif

//逻辑参数状态

TTimeRelayWordInf TimeRelayWordInf[TIME_RELAY_WORD_NUM+1];  // 时间继电器字

//2010-4-21 增加常数时间继电器的相关内容
WORD TIMERelayWordConst[CONST_TIME_RW_NUM+1][2]; //时间继电器常数参数(如果在方程中写常数)
TTimeRelayWordInf ConstTimeRelayWordInf[CONST_TIME_RW_NUM+1]; //常数时间继电器

DWORD TripRelayWordInf[TRIP_RELAY_WORD_NUM+1];  //跳闸继电器计时

TPulseRelayWordInf PulseRelayWordInf[PULSE_RELAY_WORD_NUM+1]; //脉冲继电器

TGERelayWordInf GERelayWordInf[GE_RELAY_WORD_NUM+1]; //比较继电器
LONG GEParmList[GE_RELAY_WORD_NUM+1]; //保存比较继电器参数(计算过的)

BYTE byRelayWordValue[RELAY_WORD_NUM+1];

BYTE byVInValue[VIN_RELAY_WORD_NUM+1]; //虚遥信的值
BYTE byOutPreValue[OUTPUT_RELAY_WORD_NUM+1]; // 记录出口继电器的上一次状态

//@WORD wInputRelayMap[INPUT_RELAY_WORD_NUM+1]; //开入继电器与开入映射关系
//@WORD wVirInputRelayMap[VIN_RELAY_WORD_NUM+1]; //虚遥信与开入映射关系

//2012-5-23 修改开入属性后遥信个数和电度个数不变化了，所以对于关系不会变了
//@LONG InputRelayWordBI[INPUT_RELAY_WORD_NUM+1]; //开入继电器与BI的关联关系


BYTE byDebugRelayBuf[VIRTURAL_RELAY_WORD_NUM+1]; //调试继电器缓冲区
//2011-6-16 10:37:49 为1300Led增加闪烁数据区， 为发送链路继电器和接收链路继电器增加缓冲
BYTE byLedFlashBuf[LED_RELAY_WORD_NUM+1]; 
BYTE byTxLinkBuf[TXLINK_RW_NUM+1]; ////+1是为了防止个数为0变量不存在，需要条件编译，导致函数也需要条件编译很麻烦
BYTE byRxLinkBuf[RXLINK_RW_NUM+1];

//2012-2-8 调试继电器缓冲区
BYTE byDebugInRWBuf[DEBUG_IN_NUM+1];
BYTE byDebugOutRwBuf[DEBUG_OUT_NUM+1];

//2012-4-28 计数继电器
LONG nCountRelayBuf[CNT_RELAY_WORD_NUM+1];

#ifdef __PL_WIN_SIMULATION__
BYTE* g_byPLCSource = (BYTE*)(PL_ADDR_BASE);

BOOL g_bIsEndThread;
#endif

//下面代码由工具软件生成


