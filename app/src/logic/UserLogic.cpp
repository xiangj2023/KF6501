/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			UserLogic.cpp
* 
*   软件模块：逻辑方程
*
* 
*	描述 自定义写函数
* 
* 
* 
*   函数 _EmtpyUserFun_ 空写函数， 范例
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-12-30		初始创建
* 
*************************************************************************/

//#include "stdafx.h" 
#include "Logic.h"
#include "logicdef.h"
#include "relaydef.h" 
extern BYTE byLedFlashBuf[];

/****************************************************************************************
 * 
 * 功能描述: 空写函数， 举例写函数定义规则及参数意义
 * 
 * 参数说明: 
 *			- WORD wParm, WORD wParm2
 *			  类型: 
 *			  说明: 参数  如 JMP[BG1] 中的BG1
 *
 *			- BOOL bContext
 *			  类型: 
 *			  说明: 继电器值 TRUE，FALSE
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
void _EmtpyUserFun_(WORD wParm, WORD wParm2, BOOL bContext) 
{
}

//请在此定义自定义写函数，定义方式参考EmtpyWrite
//2011-6-16
//201 闪灯函数
void LedFlash(WORD wRelayIndex,WORD LedFrequency, BOOL bContext)
{
	if(wRelayIndex <LED_RELAY_WORD_START) return;
	if(wRelayIndex >LED_RELAY_WORD_START + LED_RELAY_WORD_NUM) return;
	WORD wLedIndex = wRelayIndex - LED_RELAY_WORD_START;
	byLedFlashBuf[wLedIndex] <<= 1; // 在高一位纪录上次状态,用于沿判断 

	BYTE byFrequency = 0xF0 &(LedFrequency<<4);
	//高4为放频率， 第四位放值， 其中bit0放 最新的，bit1放上一次的
	byLedFlashBuf[wLedIndex] = byFrequency | ( byLedFlashBuf[wLedIndex]&0xF);

	if(bContext)
	{
		byLedFlashBuf[wLedIndex] |= 0x01;
	}
	else
	{
		byLedFlashBuf[wLedIndex] &= (~0x01);
		
	}
}


// 自定义写继电器字函数列表 ， 自定义函数请按数据库对应顺序添入函数列表
USER_FUN_RELAY_WORD_PTR s_pfnUserFunList[] =
{
	//在下面添加自定义写函数

	_EmtpyUserFun_,	//示例 200 空函数
	
	LedFlash, //201


};


