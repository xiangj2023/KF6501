/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			RelayMemInit.cpp
* 
*   软件模块：保护内部变量初始化
*
* 
*	描述	从数据库读取定值，配置，压板等信息到保护内部缓冲区
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
*		kangqian	2008-12-20		初始创建
* 
*************************************************************************/


#include "sysdefs.h"
#include "relayMemInit.h"

#include "Dbfunc.h"

BOOL InitAllRelayMem(void)
{
	*G_pwCurAreaNo = DB_GetCurrentSETAreaNo();
//	ReportMsg("cur set area no =%d",*G_pwCurAreaNo);
	int i;
	if(!InitMemSetting()) return false;
	if(!InitMemCFG()) return false;
	if(!InitMemSoftSwitch()) return false;
	if(!InitMemCoeff()) return false;
	if(!InitMemAssistSetting()) return false;
	return TRUE;

}

BOOL ResetMemRM()
{
	LONG* pRMbase = (LONG*)RM_BUF_ADDR;
	for(int i=0; i<RM_COUNT; i++)
	{
		*pRMbase++ = 0;
	}
	return TRUE;
}

/*BOOL ResetMemBI()
{
	BYTE* pBIbase = (BYTE*)BI_BUF_ADDR;
	for(int i=0; i<BI_COUNT; i++)
	{
		*pBIbase++ = 0;
	}
	return TRUE;
}
*/
BOOL InitMemSetting()
{
	//读之前是否有必要判断RS_COUNT 与数据库中定值个数是否一样
	LONG num = DB_GetSETNum();
	if(RS_COUNT != num)
		return false;

	LONG rc;

	LONG* pRSbase = (LONG*)RS_BUF_ADDR;
	for(int i=0; i<RS_COUNT; i++)
	{
		rc =DB_GetSETFixValue(*G_pwCurAreaNo, i, (LONG*)(pRSbase)++);
		if(rc != 0)
			return false;
	}
	return TRUE;

}
BOOL InitMemCFG()
{
	BYTE* pCfgBase = (BYTE*)CFG_BUF_ADDR;
	LONG rc;
	
	for(int i=0; i<CFG_COUNT; i++)
	{
		rc = DB_GetCFGStatus(*G_pwCurAreaNo, i, *(pCfgBase++));
		if(rc != 0)
			return FALSE;
	}
	return TRUE;

}
BOOL InitMemSoftSwitch()
{
	BYTE* pSWBase = (BYTE*)SW_BUF_ADDR;
	LONG rc;
	for(int i=0; i<SW_COUNT; i++)
	{
		rc = DB_GetSWStatus(i, *(pSWBase++));
		if(rc != 0)
			return FALSE;
	}
	return TRUE;

}
BOOL InitMemCoeff()
{
	LONG* pCfBase = (LONG*)CF_BUF_ADDR;
	LONG rc;
	for(int i=0; i<CF_COUNT; i++)
	{
		rc = DB_GetCFValue(i, (LONG*)(pCfBase)++);
		if(rc !=0)
			return FALSE;
	}
	return TRUE;

}
BOOL InitMemAssistSetting()
{
	LONG* pASBase = (LONG*)AS_BUF_ADDR;
	LONG rc;
	LONG temp;
	//读辅助定值接口
	for(int i=0; i<AS_COUNT; i++)
	{
//	by zengqingzhao
		rc = DB_GetSystemSETFixValue(i, (LONG*)(pASBase)++);
//		rc = DB_GetSystemSETFixValue(i, &temp);
		if(rc != 0)
			return FALSE;
	}
	return TRUE;

}

//以下是作报告的例子， 报告宏和故障电量使用示例
//@	
//@//@	RM_
//@//@
//@//@	ReportInf[R_rep1].
//@	tAction.nINF = 	ReportInf[R_rep1].wInf;
//@	tAction.bStatus = 0;
//@	tAction.stTime.Hi = 0x2002;
//@	tAction.stTime.Lo = 0x44444444;
//@	
//@
//@	for(j=0;j<ReportInf[R_rep1].dwFialureNum ;j++)
//@		tAction.FD_Value[j] = *(ReportInf[R_rep1].pFailureData[j]);
//@	DB_RelayAction(102, &tAction);
