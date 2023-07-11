/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			RelayMemInit.cpp
* 
*   ���ģ�飺�����ڲ�������ʼ��
*
* 
*	����	�����ݿ��ȡ��ֵ�����ã�ѹ�����Ϣ�������ڲ�������
* 
* 
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-12-20		��ʼ����
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
	//��֮ǰ�Ƿ��б�Ҫ�ж�RS_COUNT �����ݿ��ж�ֵ�����Ƿ�һ��
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
	//��������ֵ�ӿ�
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

//����������������ӣ� �����͹��ϵ���ʹ��ʾ��
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
