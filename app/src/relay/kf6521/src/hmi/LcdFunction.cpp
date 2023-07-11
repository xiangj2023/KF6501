
//{{Begin of Describtion of LcdFunction.cpp
/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/
/*************************************************************************
* 
*   文件名称：
*			LCDFunction.cpp
* 
*   软件模块：液晶应用程序头文件
*
* 
*	描述	液晶各界面类定义
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
*		 液晶工具软件	2009-02-26		初始创建
* 
*************************************************************************/
//add include .h files below
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			
#include "Db_err.h"
#include "lcdapp.h"
#include "RelayDef.h"
#include "HmiApp.h"
#include "relay.h"
#include "logicdef.h"
#include "hmiobj.h"
#include "relay_defs.h"
#include "relayfun.h"
#include "logic.h"
#include "bitmapdata.h"


extern CHmiInputDlg g_BIDlg;
extern CHmiAIDlg g_AIDlg;
extern DWORD dwAITaskID;
extern DWORD dwTaskDIId;
extern BYTE g_RptSerialNum;
extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;
extern HANDLE g_hWdogTaskID;

//extern char g_strDefaultPara[13];
extern BOOL g_bDisplayEEpromButton;
extern CHmiLangDlg g_LangDlg;
extern CHmiDlgSerialSet g_SerialSet;
extern CHmiDlgSysStatus g_SysStatus;
extern CHmiDlgWComStatus g_ComStatusDlg;
extern CHmiDlgAddressSetting g_ModifyIP;
extern CLcdWndDefaultPara g_DefaultPara;
extern CLcdWndversion g_Version;

extern WORD g_wEth;
extern CLcdWndAutoJustStep1 g_WndAutoJustStep1;

extern CLcdWndPortPara 	g_WndPortPara;
extern CLcdWndETHportModity g_WndPortProtocol;


//}}End of Describtion of LcdFunction.cpp
/*
void CLcdWndSoeReport::OnEnterListSoeReport()
{
	int nItem,nDispID;
	nItem = m_ListSoeReport.GetCurSel();
	nDispID = nItem + m_ListSoeReport.GetItemBase()+1; //起始

	//得到报告的数据库标识号
	WORD wIndex = (WORD)m_ListSoeReport.GetItemData(nItem);
	if (wIndex == HMI_LIST_NORECORD)
	{
		return;
	}
	if(m_SoeDetaildlg.ReadReport(wIndex,nDispID))
		m_SoeDetaildlg.DoModal(this);
	else
		HMIMsgBox(tr("SOE报告"),tr("读SOE报告失败!"),this,this);

}
*/
void CLcdWndMeasure::OnEnterBtnHMeasure()
{		
	char DlgCaption[64];
	sprintf(DlgCaption, tr("一次侧测量值\n"));	

	WORD whAIIndex[] = {M_UA,M_UB,M_UC,M_UAB,M_UBC,M_UCA,M_U1,M_U2,M_U3,M_IA,M_IB,M_IC,M_I0,M_I1}; 
	g_AIDlg.DoModal(this, this, hAI,14,whAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnLMeasure()
{	
	char DlgCaption[64];
	sprintf(DlgCaption, tr("二次侧测量值\n"));	
	WORD wlAIIndex[] = {M_UA,M_UB,M_UC,M_UAB,M_UBC,M_UCA,M_U1,M_U2,M_U3,M_IA,M_IB,M_IC,M_I0,M_I1,M_P,M_Q,M_COS,M_F1,M_F2};	
	g_AIDlg.DoModal(this, this, lAI,19,wlAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnPML()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("二次侧保护值\n"));	
	WORD wplAIIndex[] = {RM_UA,RM_UB,RM_UC,RM_UAB,RM_UBC,RM_UCA,RM_U0,RM_UL1,RM_UL2,RM_UL3, \
		RM_IA,RM_IB,RM_IC,RM_I0,RM_I1,RM_U2,RM_U05,RM_I05};	
	g_AIDlg.DoModal(this, this, plAI,18,wplAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnAngle()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("电压电流相位\n"));	
	WORD wplAIIndex[] = {RM_AUA,RM_AUB,RM_AUC,RM_AUAB,RM_AUBC,RM_AUCA,RM_AUL1,RM_AUL2,RM_AUL3, \
		RM_AIPA,RM_AIPB,RM_AIPC,RM_AIP1,RM_AIMA,RM_AIMB,RM_AIMC,RM_AU01,RM_AI01,RM_AU05,RM_AI05};	
	g_AIDlg.DoModal(this, this, plAI,20,wplAIIndex,DlgCaption);
}

void CLcdWndMeasure::OnEnterBtnsyc()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("检同期\n"));	
	WORD wplAIIndex[] = {RM_F1,RM_F2,RM_DIFFU,RM_DIFFA,RM_DIFFDQ};	
	g_AIDlg.DoModal(this, this, plAI,5,wplAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnKilowatt()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("实时电度值\n"));	
	g_AIDlg.DoModal(this, this, CI,0,NULL,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnJDXX()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("接地选线\n"));	
	WORD wplAIIndex[] = {RM_U0,RM_I0,RM_U05,RM_I05,RM_AI01,RM_AI05};	
	g_AIDlg.DoModal(this, this, phAI,6,wplAIIndex,DlgCaption);
}

void CLcdWndReport::OnEnterBtnActionReport()
{
	CLcdApp::m_WndActionReport.Close();

}
void CLcdWndReport::OnEnterBtnOperation()
{
	CLcdApp::m_WndOperationReport.Close();

}

void CLcdWndActionReport::OnEnterListActionReport()
{
	int nItem, nDispID;
	nItem = m_ListActionReport.GetCurSel();
	nDispID = nItem + m_ListActionReport.GetItemBase()+1; //起始
	WORD wActionIndex = (WORD)m_ListActionReport.GetItemData(nItem);
	if (wActionIndex == HMI_LIST_NORECORD)
	{
		return;
	}

	//读取报告正确才显示
	if(m_ActionDetailDlg.ReadReport(HIBYTE(wActionIndex), LOBYTE(wActionIndex),nDispID))
		m_ActionDetailDlg.DoModal(this);
		
	else
		HMIMsgBox(tr("动作报告"), tr("读动作报告失败!"), this, this, HMI_MB_OK);

}

void CLcdWndAlarmReport::OnEnterListAlarmReport()
{
	
	int nItem, nDispID;
	nItem = m_ListAlarmReport.GetCurSel();
	WORD wActionIndex = (WORD)m_ListAlarmReport.GetItemData(nItem);
	nDispID = nItem + m_ListAlarmReport.GetItemBase()+1; //起始
	
	if (wActionIndex == HMI_LIST_NORECORD)
	{
		return;
	}

	//读取正确才实现 
	if(m_AlarmDetailDlg.ReadReport(wActionIndex,nDispID))
		m_AlarmDetailDlg.DoModal(this);
	else
		HMIMsgBox(tr("告警报告"), tr("读告警报告失败!"), this, this, HMI_MB_OK);

}

//定值查询 区号输入窗口
void CLcdWndSettingQueryAreaNumInput::OnWndEnter()
{
	LONG AreaNo;
	m_EditAreaNum.UpdateData();
	AreaNo = (LONG)m_EditAreaNum.GetValue();
	//设置定值查询和配置查询窗口区号都是输入的区号
	CLcdApp::m_WndSettingQuery.m_wAreaNo = AreaNo;
	CLcdApp::m_WndconfigQury.m_wAreaNo = AreaNo;
	
	CHmiWnd::OnWndEnter();
}
BOOL CLcdWndSettingQueryAreaNumInput::OnWndInit()
{
	//显示当前的定值区号
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);
	//输入框中也显示当前定值区号
	m_EditAreaNum.SetValue(nAreaNo);
	m_EditAreaNum.UpdateData(FALSE);
	return TRUE;

}
void CLcdWndSetting::OnEnterBtnSettingQuery()
{
	CLcdApp::m_WndconfigQury.Close();
	CLcdApp::m_WndSettingQuery.Close();

}

//定值修改
void CLcdWndSetting::OnEnterBtnsettingModify()
{
	//关闭配置修改窗口， 确保重新刷新
	CLcdApp::m_WndconfigModify.Close();
	//密码窗口
	HMIPasswordBox(tr("输入密码"),  tr("输入密码"),&(CLcdApp::m_WndSettingChgAreaNumInput), this,0);
}

//定值切换
void CLcdWndSetting::OnEnterBtnsettingExChg()
{
	//密码窗口
	HMIPasswordBox(tr("输入密码"),  tr("输入密码"),&(CLcdApp::m_WndSettingSwitch), this,0);

}

//定值拷贝
void CLcdWndSetting::OnEnterBtnSettingCopy()
{
	//密码窗口
	HMIPasswordBox(tr("输入密码"),  tr("输入密码"),&(CLcdApp::m_WndSettingCopy), this,0);

}

//配置查询
BOOL CLcdWndconfigQury::OnWndInit()
{
	m_ListConfigQuery.SetAreaNo(m_wAreaNo);
	char szCaption1[64];
	
	sprintf(szCaption1, tr("配置查看：%d区"), m_wAreaNo);
	SetCaption(szCaption1);
	return TRUE;
}

//定值查询
BOOL CLcdWndSettingQuery::OnWndInit()
{
	m_ListSettingQuery.SetAreaNo(m_wAreaNo);
	char szCaption1[64];
	sprintf(szCaption1, tr("定值查看：%d区"), m_wAreaNo);
	SetCaption(szCaption1);
	return TRUE;
}
WORD CLcdWndconfigModify::m_wAreaNo = 0;
//自定义函数， 没有可显示的定值时保存配置
void CLcdWndconfigModify::SaveConfig()
{
	//比较定值和配置是否有不相同， 如果有则提示要固化
	WORD r1 = memcmp(CHmiApp::m_byCurCfg,CHmiApp::m_byNewCfg, CFG_COUNT);
	if(r1 != 0)
	{
		CLcdApp::m_WndSettingSave.m_wAreaNo = m_wAreaNo;; //定值区号传过去

		CLcdApp::m_WndconfigModify.SetEnterWnd(&CLcdApp::m_WndSettingSave);
	}
	else
		CLcdApp::m_WndconfigModify.SetEnterWnd(&CLcdApp::m_WndSetting);
	
	CLcdApp::m_WndconfigModify.CHmiWnd::OnWndEnter();
}
void CLcdWndSettingModify::OnWndUpDown(LONG UpDown)
{
	if (m_wDispSettingCount == 0)
		return;

	m_wDispSetIndex = m_wDispSetIndex + m_wDispSettingCount + UpDown;
	m_wDispSetIndex = m_wDispSetIndex%m_wDispSettingCount;
	OnWndInit();
	OnPaint();
	UpdateWindow();
}

//保存定值修改
void CLcdWndSettingModify::OnWndEnter()
{
	//比较定值和配置是否有不相同， 如果有则提示要固化
	WORD r1 = memcmp(CHmiApp::m_byCurCfg,CHmiApp::m_byNewCfg, CFG_COUNT);
	WORD r2 =  memcmp(CHmiApp::m_fCurSetting, CHmiApp::m_fNewSetting, sizeof(float)*RS_COUNT);

	if(r1 != 0  || r2 != 0)
	{
		CLcdApp::m_WndSettingSave.m_wAreaNo = m_wAreaNo;; //定值区号传过去

		SetEnterWnd(&CLcdApp::m_WndSettingSave);
	}
	else
		SetEnterWnd(&CLcdApp::m_WndSetting);
	
	CHmiWnd::OnWndEnter();

}

void CLcdWndSettingModify::OnWndAddSub(LONG AddSub)
{
	//需要对用户输入的定值进行缓存
	//判读是编辑框输入还是combo框输入

	float fValue;
	WORD wSetNo = CHmiApp::m_wpVisibleSetList[m_wDispSetIndex];

	if(m_bIsCombo)
	{
		LONG nItem = m_ComboSettingComb.GetCurSel();
		LONG lval = m_ComboSettingComb.GetItemData(nItem);
		fValue = lval;
	}
	else
	{
		fValue = m_EditSettingData.GetValue();
	}

	CHmiApp::m_fNewSetting[wSetNo] = fValue;
}

BOOL CLcdWndSettingModify::OnWndInit()
{
	if(m_wDispSetIndex >= m_wDispSettingCount)
		m_wDispSetIndex = 0;


	TSET_Disp	tSETDisp;
	char str[64];
	char strFormat[32];
	char strSetDataName[MAX_DISP_COUNT][HMI_DBSTRING_SIZE];
	WORD wSetNameCount; //定值取值描述个数
	LONG rc;

	WORD wSetNo = CHmiApp::m_wpVisibleSetList[m_wDispSetIndex];

	rc = DB_GetSETDisp(wSetNo, &tSETDisp);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("定值修改"),tr("读定值信息失败!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
		return FALSE;
	}

	BOOL bIsSetDataNameOk = TR_GetSETDataName(&tSETDisp,wSetNo, &strSetDataName[0][0], wSetNameCount);
	if(!bIsSetDataNameOk)
	{
		HMIMsgBox(tr("定值修改"),tr("读定值信息失败!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
		return FALSE;
	}
	
	//修改标题
	sprintf(str, tr("修改定值: %02d区"), m_wAreaNo);
	SetCaption(str);
	DrawCaption();

	//定值简称
	sprintf(str, "%s =", tSETDisp.strSymbol);
	m_StaticSymbol.SetCaption(str);

	
	//定值名称
	m_StaticName.SetCaption(TR_GetSETName(&tSETDisp,wSetNo));
	sprintf(str, tr("下限%.2f"), tSETDisp.fMinValue);
	m_StaticUpLimit.SetCaption(str);
	sprintf(str, tr("上限%.2f"), tSETDisp.fMaxValue);
	m_StaticDownLimit.SetCaption(str);

	//定值单位
	m_StaticUnit.SetCaption(tSETDisp.strUnit);

	LONG symoble = 0;
	//判断定值类型， 是应该edit输入还是combo选择
	if (wSetNameCount>0) //combo
	{
		m_bIsCombo = TRUE;
		m_ComboSettingComb.SetVisible(TRUE);
		m_EditSettingData.SetVisible(FALSE);
		SetFocus(m_ComboSettingComb.GetTabOrder());

		m_ComboSettingComb.m_nX = m_EditSettingData.m_nX;
		m_ComboSettingComb.m_nY = m_EditSettingData.m_nY;
		m_ComboSettingComb.m_nYSize = m_EditSettingData.m_nYSize;
		m_ComboSettingComb.m_nXSize = m_EditSettingData.m_nXSize;

		int nItemCount = wSetNameCount;
		m_ComboSettingComb.ResetContent();
		int nItem;

		for (int i=0; i<nItemCount; i++)
		{
			nItem = m_ComboSettingComb.AddString(strSetDataName[i]);
			m_ComboSettingComb.SetItemData(nItem, i);

			if(i == CLcdApp::m_fNewSetting[wSetNo])
				m_ComboSettingComb.SelectItem(i);
		}
	}
	else
	{
		m_bIsCombo = FALSE;
		m_ComboSettingComb.SetVisible(FALSE);
		m_EditSettingData.SetVisible(TRUE);
		SetFocus(m_EditSettingData.GetTabOrder());
	//定值是否有负值， 会出现负数的话要显示正负号
	if(tSETDisp.fMinValue <0)
	{
		symoble= 1;
		m_EditSettingData.SetIsShowNegative(TRUE);
	}
	else
		m_EditSettingData.SetIsShowNegative(FALSE);


	float tmpf = tSETDisp.fMaxValue;
	if(abs(tSETDisp.fMinValue) > tmpf)
		tmpf = abs(tSETDisp.fMinValue);

	sprintf(strFormat, "%s%d%s", "%.", tSETDisp.nDec, "f");

	//用于统计长度以确定edit的最大位数
	sprintf(str, strFormat, tmpf);
	LONG len = strlen(str) + symoble;

    //设置输入限制
	m_EditSettingData.SetMaxLen(len);
	m_EditSettingData.SetValueRange(tSETDisp.fMaxValue, tSETDisp.fMinValue);
	m_EditSettingData.SetDecimalNum(tSETDisp.nDec);

	m_EditSettingData.SetValue(CLcdApp::m_fNewSetting[wSetNo]);
	m_EditSettingData.SetCursorPos(0);
	m_EditSettingData.UpdateData(FALSE);
	}

	return TRUE;

}

void CLcdWndSettingSave::OnEnterBtnok()
{
	WORD wCfgNum = DB_GetCFGNum();
	WORD nWord = wCfgNum/32;
	int i;
	DWORD BitSign, DwordNo;
	
	//修改的配置更新到定值缓冲区
	DWORD* pdwCFGWord;

	for (i=0; i<wCfgNum; i++)
	{    
		DwordNo 	= i/32;
		BitSign = (0x01<<(i%32));

		pdwCFGWord = (DWORD*)(&CHmiApp::m_fNewSetting[DwordNo]);

		if (CHmiApp::m_byNewCfg[i] == CFG_STATUS_SET)
			*pdwCFGWord = (*pdwCFGWord)|BitSign;
		else
			*pdwCFGWord = (*pdwCFGWord)&(~BitSign);
	}

	//定值保存
	if(!HMISaveSetToDB(m_wAreaNo))
	{
		HMIMsgBox(tr("定值保存"),tr("保存定值失败!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
		DB_GetSETFloatValue(m_wAreaNo, SET_VALUE_ALL, CHmiApp::m_fCurSetting);
		m_Btnok.DisableAutoEnterWnd();
		return;
	}
//	DB_SetSETValue(m_wAreaNo,SET_VALUE_ALL, CHmiApp::m_fNewSetting);
//	DB_SetSETValueConfirm(m_wAreaNo,SET_VALUE_ALL, DB_EXECUTE);
	 DB_GetSETFloatValue(m_wAreaNo, SET_VALUE_ALL, CHmiApp::m_fCurSetting);


	 WORD wCurAreaNO = DB_GetCurrentSETAreaNo();

	 //修改的是否是当前定值区
	 if(wCurAreaNO != m_wAreaNo)
	 {
		 m_Btnok.SetEnterWnd(&CLcdApp::m_WndSettingSwitchConfirm);
		 //定值固话后确认是否要切换定值
		 CLcdApp::m_WndSettingSwitchConfirm.m_wNewAreaNo = m_wAreaNo;
	 }
	 else
	 {
		 m_Btnok.SetEnterWnd(&CLcdApp::m_WndSetting);
	 }
	 m_Btnok.EnableAutoEnterWnd();

}

void CLcdWndSettingSwitch::OnWndEnter()
{
	WORD wAreaNo = m_EditAreaNum.GetValue();

	CLcdApp::m_WndSettingSwitchConfirm.m_wNewAreaNo = wAreaNo;
	
	CHmiWnd::OnWndEnter();
}

BOOL CLcdWndSettingSwitch::OnWndInit()
{
	//在文本框中显示当前定值区号
	WORD wAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", wAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);

	return TRUE;
}

WORD CLcdWndSettingCopy::m_wSourArea=0;
WORD CLcdWndSettingCopy::m_wStartArea=0;
WORD CLcdWndSettingCopy::m_wEndArea=0;

void CLcdWndSettingCopy::OnWndEnter()
{

	m_wSourArea = m_EditSoureAreaNum.GetValue();
	m_wStartArea = m_EditDstStartAreaNum.GetValue();
	m_wEndArea = m_EditDstEndAreaNum.GetValue();
	char Msg1[64];
	sprintf(Msg1, tr("确定要整区复制定值区%d到\n定值区%d-%d ?"), m_wSourArea, m_wStartArea, m_wEndArea);
	YES_NO_PTR pfn = (YES_NO_PTR)CopyAreaSet;
	HMIMsgBox(tr("定值复制"),  Msg1,  this, this, HMI_MB_OKCANCEL, (YES_NO_PTR)pfn);
}
//整区拷贝定值
void CLcdWndSettingCopy::CopyAreaSet()
{
	//读定值
	float fvalue[256];
	STATUS rc = 0;
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndSettingCopy;
	rc=DB_GetSETFloatValue(m_wSourArea,  SET_VALUE_ALL, fvalue);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(pWnd->tr("定值复制"),pWnd->tr("定值区读取失败!"),pWnd, pWnd);
		return;
	}
	if(m_wStartArea>m_wEndArea)
	{
		HMIMsgBox(pWnd->tr("定值复制"),pWnd->tr("定值区号设置错误!"),pWnd, pWnd);
		return;
	}
	WORD i;
	for(i = m_wStartArea; i<= m_wEndArea; i++ )
	{
		rc= DB_SetSETValue(i, SET_VALUE_ALL, fvalue);
		if(DB_ERR_OK != rc)
			break;
		rc = DB_SetSETValueConfirm(i, SET_VALUE_ALL, DB_EXECUTE);
		if(DB_ERR_OK != rc)
			break;
	}
	if(i == m_wEndArea+1)
		CLcdApp::m_WndSetting.Show();
	else
	{		
		char Msg1[64];
		sprintf(Msg1, pWnd->tr("定值区%d-%d复制失败!"),i,m_wEndArea);
		HMIMsgBox(pWnd->tr("定值复制"),Msg1,pWnd, pWnd);
	}
}

void CLcdWndSoftSwitch::SaveSoftSwitch()
{
	//静态函数的this是无效的，也不能调用非静态成员函数
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndSoftSwitch;
	if(HMISaveSWToDB())
		CLcdApp::m_WndSoftSwitch.Show();
	else
		HMIMsgBox(pWnd->tr("软压板"),pWnd->tr("软压板投退失败!"),pWnd, pWnd);
}
void CLcdWndSoftSwitch::OnEnterListSoftSwitch()
{
	LONG r = memcmp(CHmiApp::m_byCurSoftSwitch, CHmiApp::m_byNewSoftSwitch, SW_COUNT);
	//如果有变化才保存
	if(r != 0)
	{		
		YES_NO_PTR pfn = (YES_NO_PTR)SaveSoftSwitch;
		HMIMsgBox(tr("软压板"),tr("确定要投退软压板?"),this, this, HMI_MB_OKCANCEL,pfn);		
	}
	else
	{
		//关闭压板窗口先
		Close();
		//显示主菜单窗口
		CLcdApp::m_WndMainMenu.Show();
	}

}
/*
BOOL CLcdWndSoftSwitch::OnWndInit()
{
	//读取现在的软压板缓存起来
	WORD num = DB_GetSWNum();
	if(num > sizeof(CHmiApp::m_byCurSoftSwitch))
	{
		num = sizeof(CHmiApp::m_byCurSoftSwitch);
		LogError("CLcdWndSoftSwitch::OnWndInit", FILE_LINE, "softswitch num overflow");

	}
	WORD i; 
	for(i=0; i<num; i++)
	{
		DB_GetSWStatus(i, CHmiApp::m_byCurSoftSwitch[i]);
		DB_GetSWStatus(i, CHmiApp::m_byNewSoftSwitch[i]);
	}
	return TRUE;
}
*/
void CLcdWndtime::OnWndEnter()
{
	//设置时间
	TSysTime newTime;
	newTime.Year = m_EditYear.GetValue();
	newTime.Month = m_EditMonth.GetValue();
	newTime.Day = m_EditDay.GetValue();
	newTime.Hour = m_EditHour.GetValue();
	newTime.Minute = m_EditMinute.GetValue();
	newTime.Second = m_EditSecond.GetValue();
	newTime.MSecond = 0;
	long ret = WriteSystemTime(&newTime);
	CHmiWnd::OnWndEnter();
}

BOOL CLcdWndtime::OnWndInit()
{
	TSysTime  tSysTime;
	TAbsTime tAbsTime;

	ReadAbsTime(&tAbsTime);
	AbsTimeTo(&tAbsTime, &tSysTime);
	m_EditYear.SetValue(tSysTime.Year,0,4);
	m_EditYear.UpdateData(false);

	m_EditMonth.SetValue(tSysTime.Month,0, 2);
	m_EditMonth.UpdateData(false);

	m_EditDay.SetValue(tSysTime.Day,0,2);
	m_EditDay.UpdateData(false);


	m_EditHour.SetValue(tSysTime.Hour,0,2);
	m_EditHour.UpdateData(false);

	m_EditMinute.SetValue(tSysTime.Minute,0,2);
	m_EditMinute.UpdateData(false);

	m_EditSecond.SetValue(tSysTime.Second,0,2);
	m_EditSecond.UpdateData(false);

	return TRUE;
}


//保存DI去抖时间
void CLcdWndBIFilter::SaveDIFilterTime()
{
	CHmiWnd* pWnd = &CLcdApp::m_WndBIFilter;
	if(!HMISaveDIFilterTimeToDB())
	{
		HMIMsgBox(pWnd->tr("开入去抖"), pWnd->tr("开入去抖时间保存失败!"), pWnd, pWnd, HMI_MB_OKCANCEL);
	}
	else
	{
		pWnd->Show();
	}
	
	//保存函数出现部分保存正确，部分保存错误也返回错误
	//所以不管保存函数返回成功否都发改消息，
	OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}
void CLcdWndBIFilter::OnWndEnter()
{
	if(!CHmiApp::m_bDIFilterTimeModified)
		return;

	YES_NO_PTR pfn = (YES_NO_PTR)SaveDIFilterTime;
	HMIMsgBox(tr("修改开入去抖时间"),  tr("确定要保存修改的内容?"),  this, this, HMI_MB_OKCANCEL, pfn);

	
}
 //保存 遥信是否产生soe属性 ， 对应数据库中wControl-D1
void CLcdWndSoeProperty::SaveBISoePara()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndSoeProperty;
	if(!HMISaveBIControlToDB())
	{
		//静态函数中this不可用，所以用CLcdApp::m_WndSoeProperty
		HMIMsgBox(pWnd->tr("soe属性"), pWnd->tr("保存soe属性失败!"),pWnd, pWnd);
	}
	else
	{
		pWnd->Show();
	}
	//保存函数出现部分保存正确，部分保存错误也返回错误
	//所以不管保存函数返回成功否都发改消息，
	OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}

void CLcdWndSoeProperty::OnWndEnter()
{
	if(!CHmiApp::m_bBIControlModified)
		return;
	char strCaption[64], strMsg[64];
	sprintf(strCaption, "%s", tr("修改是否产生Soe"));
	sprintf(strMsg, "%s", tr("确定要保存修改的内容?"));
	YES_NO_PTR pfn = (YES_NO_PTR)SaveBISoePara;
	
	HMIMsgBox(strCaption,  strMsg,  this, this, HMI_MB_OKCANCEL, pfn);
}

void CLcdWndOutputTest::OnWndEnter()
{
	m_EditOutputNum.UpdateData(TRUE);
	m_EditOutputTime.UpdateData(TRUE);
	WORD wNum = m_EditOutputNum.GetValue();
	WORD wTime = m_EditOutputTime.GetValue();
	if((wNum>CTRLTEST_RELAY_WORD_NUM)||(wNum<=0))		
	{
		LogError("BOTEST",FILE_LINE,"the bo num is err");
		return;
	}
		
	//传动出口
	DB_BOTest(wNum-1,wTime);

	CHmiWnd::OnWndEnter();


}
void CLcdWndReportTest::OnWndEnter()
{
	TAbsTime RptTime;
	WORD	 RptNo;
	ReadAbsTime(&RptTime);
	DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&RptTime,FALSE); //向数据库中写入启动报告
	RptNo = g_RptSerialNum;
	MakeActRpt(R_COMTEST,RptNo);	
	DB_RelayEnd(RptNo); 			//向数据库中写入结束报告

	
	CHmiWnd::OnWndEnter();
}

BOOL CLcdWndCANAddress::OnWndInit()
{
	BYTE DevID;
	DevID = DB_GetAddress();
	m_EditCANA.SetValue(DevID);
	m_EditCANA.UpdateData(FALSE);	
	return TRUE;
}
void CLcdWndCANAddress::OnEnterBtnenter()
{
	BYTE DevID;
	STATUS rc;
	DevID = m_EditCANA.GetValue();
	rc = DB_ModifyAddress(DevID);
	if(DB_ERR_OK != rc)
	{
		m_Btnenter.DisableAutoEnterWnd();
		HMIMsgBox(tr("CAN网地址"),tr("修改CAN网地址失败!"), this);
	}
	m_Btnenter.EnableAutoEnterWnd();
}


void CLcdWndPassChange::OnEnterBtnChgSettingPass()
{
	HMIChgPassBox(tr("管理员密码修改"), this,this, 1);
}

void CLcdWndPassChange::OnEnterBtnChgCommonPass()
{
	
	HMIChgPassBox(tr("普通密码修改"), this,this, 0);
}


void CLcdWndSettingChgAreaNumInput::OnWndEnter()
{
	LONG nAreaNo = m_EditInputAreaNum.GetValue();

	CLcdApp::m_WndSettingModify.m_wAreaNo = nAreaNo;
	CLcdApp::m_WndconfigModify.m_wAreaNo = nAreaNo;
	SetEnterWnd(&CLcdApp::m_WndconfigModify);
	int i;

	//读配置到缓存区

	if(!HMIReadCFGToBuf(nAreaNo))
	{
		SetEnterWnd(NULL);
		HMIMsgBox(tr("定值修改"),tr("读配置失败!"),this,this);
		return;
	}
	
	//读取当前定值到缓冲区
	if(!HMIReadSetToBuf(nAreaNo))
	{
		SetEnterWnd(NULL);
		HMIMsgBox(tr("定值修改"),tr("读定值失败!"),this,this);
		return;
	}
	
	CHmiWnd::OnWndEnter();
}

BOOL CLcdWndSettingChgAreaNumInput::OnWndInit()
{
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);
	return TRUE;

}
BOOL CLcdWndSettingSwitchConfirm::OnWndInit()
{
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNo.SetCaption(szAreaNo);

	sprintf(szAreaNo, "%d", m_wNewAreaNo);
	m_StaticNewAreaNo.SetCaption(szAreaNo);
	return TRUE;

}
void CLcdWndSettingSwitchConfirm::OnEnterBtnOk()
{
	LONG rc;
	rc = DB_SetCurrentSETAreaNo(m_wNewAreaNo);
	if(DB_ERR_OK ==rc)
		rc = DB_SetCurrentSETAreaNoConfirm(m_wNewAreaNo, DB_EXECUTE);
	
	//使用不成功
	if(DB_ERR_OK != rc)
	{
		m_BtnOk.DisableAutoEnterWnd();
		HMIMsgBox(tr("定值切换"), tr("定值切换失败!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
	}
	else
	{
		m_BtnOk.EnableAutoEnterWnd();
	}
	

}
void CLcdWndStartReport::OnEnterListStartReport()
{
	int nItem,nDispID;
	nItem = m_ListStartReport.GetCurSel();
	
	nDispID = nItem + m_ListStartReport.GetItemBase()+1; //起始
	//得到报告的数据库标识号
	WORD wIndex = (WORD)m_ListStartReport.GetItemData(nItem);
	if (wIndex == HMI_LIST_NORECORD)
	{
		return;
	}
	
	if(m_StartDetailDlg.ReadReport(wIndex,nDispID))
		m_StartDetailDlg.DoModal(this);
	else
		HMIMsgBox(tr("启动报告"),tr("读启动报告失败!"),this,this);
}
/*
void CLcdWndOperationReport::OnEnterListOperationReport()
{
	
	int nItem,nDispID;
	nItem = m_ListOperationReport.GetCurSel();

	nDispID = nItem + m_ListOperationReport.GetItemBase()+1; //起始
	//得到报告的数据库标识号
	WORD wIndex = (WORD)m_ListOperationReport.GetItemData(nItem);
	if (wIndex == HMI_LIST_NORECORD)
	{
		return;
	}

	if(m_OperationDetailDlg.ReadReport(wIndex,nDispID))
		m_OperationDetailDlg.DoModal(this);
	else
		HMIMsgBox(tr("操作事项"),tr("读操作事项失败!"),this,this);
}
*/
void CLcdWndconfigModify::OnEnterListConfigModify()
{

	LONG nDispSetCount = 0;
	WORD wNum = DB_GetSETNum();
	WORD wCfgCount = DB_GetCFGNum();
	TSET_Disp tSetDisp;
	WORD wCfgIndex;
	WORD i;
	LONG rc;

	//更新可显示的定值列表 保存到全局数组 CHmiApp::m_wpVisibleSetList
	BOOL res = HMIUpDataVisibleSetList(m_wAreaNo, &nDispSetCount, CHmiApp::m_wpVisibleSetList,FALSE);

	//是否有可显示的定值
	if (nDispSetCount ==0)
	{
		HMIMsgBox(tr("注意"),	tr("没有投入的定值,是否要继续"), 
			&(CLcdApp::m_WndSettingSave), this, HMI_MB_OK, SaveConfig);
			
	}
	else
	{
		SetEnterWnd(&CLcdApp::m_WndSettingModify);
		CLcdApp::m_WndSettingModify.m_wDispSettingCount = nDispSetCount;
		CLcdApp::m_WndSettingModify.m_wDispSetIndex =0;		
		CLcdApp::m_WndSettingModify.Show();
	}


}
//保存遥信取反信息
void CLcdWndBiNot::ChangeBInegation()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndBiNot;
	
	//保存函数出现部分保存正确，部分保存错误也返回错误
	//所以不管保存函数返回成功否都发改消息，
	if(!HMISaveBIControlToDB())
		HMIMsgBox(pWnd->tr("BI属性修改"), pWnd->tr("保存BI属性失败!"), pWnd, pWnd, HMI_MB_OKCANCEL);
	else
		pWnd->Show();
	OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}

//遥信取反保存
void CLcdWndBiNot::OnEnterListBiNot()
{
	if(CHmiApp::m_bBIControlModified)
	{
		YES_NO_PTR pfn = (YES_NO_PTR)ChangeBInegation;
		HMIMsgBox(tr("遥信取反"),  tr("确定要保存遥信取反信息?"),  this, this, HMI_MB_OKCANCEL, pfn);
	}

}
BOOL CLcdWndconfigModify::OnWndInit()
{
	//配置列表
	m_ListConfigModify.SetAreaNo(m_wAreaNo);

	char szCaption[32];
	//修改标题
	sprintf(szCaption, tr("修改配置: %02d区"), m_wAreaNo);
	SetCaption(szCaption);

	DrawCaption();
	return TRUE;
}

void CLcdWndOutputTest::OnAddSubEditOutputNum(LONG AddSub)
{
	m_EditOutputNum.UpdateData();
	int nNum = m_EditOutputNum.GetValue();
	char str[32];
	
	TTC_Disp TC_Disp;
	LONG rc = DB_GetTCDisp(nNum-1,&TC_Disp);
	if(DB_ERR_OK == rc)
	{
		sprintf(str, TR_GetTCName(&TC_Disp,nNum-1));
	}
	else
	{
		sprintf(str, "TestOut%d", nNum-1);
	}
	m_StaticOutputName.SetCaption(str);
	m_StaticOutputName.ReDraw();
	UpdateWindow();
}

//辅助定制保存,字定义静态函数
void CLcdWndASModify::SaveSysSetting()
{
	CHmiWnd* pWnd = &CLcdApp::m_WndASModify;
	
	if(!HMISaveSysSetToDB())
	{
		HMIMsgBox(pWnd->tr("公用定值"), pWnd->tr("公用定值保存失败!"), pWnd,pWnd);
	}
	else
	{
		pWnd->Show();
	}
}

void CLcdWndASModify::OnWndEnter()
{
	if(!CHmiApp::m_bSysSetListModified)
	{
		CLcdApp::m_WndSetting.Show();
		return;
	}
	YES_NO_PTR pfn = (YES_NO_PTR)SaveSysSetting;
	HMIMsgBox(tr("修改公用定值"),  tr("确定要修改公用定值?"),  this, this, HMI_MB_OKCANCEL, pfn);

}

BOOL CLcdWndASModify::OnWndInit()
{

	TSysSET_Disp	tSysSETDisp;
	char str[64];
	char strFormat[32];
	char strSetDataName[MAX_DISP_COUNT][HMI_DBSTRING_SIZE];
	WORD wSetNameCount; //定值取值描述个数

	LONG rc = DB_GetSysSETDisp(m_wSysSetIndex,  &tSysSETDisp);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("公用定值"), tr("读公用定值错误!"), &CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting);
		return FALSE;
	}
	BOOL bSysSetDataOk = TR_GetSysSETDataName(&tSysSETDisp,m_wSysSetIndex, &strSetDataName[0][0],wSetNameCount);
	if(!bSysSetDataOk)
	{
		HMIMsgBox(tr("公用定值"), tr("读公用定值错误!"), &CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting);
		return FALSE;
	}
	
	//修改标题
	sprintf(str, tr("修改公用定值"));
	SetCaption(str);

	DrawCaption();

	//定值英文名
	sprintf(str, "%s =", tSysSETDisp.strSymbol);
	m_StaticASSymbol.SetCaption(str);

	
	//定值名称
	m_StaticAsName.SetCaption(TR_GetSysSETName(&tSysSETDisp,m_wSysSetIndex));

	sprintf(str, tr("下限%.2f"), tSysSETDisp.fMinValue);
	m_StaticDownLimit.SetCaption(str);
	sprintf(str, "Low %0.2f", tSysSETDisp.fMinValue);

	sprintf(str, tr("上限%.2f"), tSysSETDisp.fMaxValue);
	m_StaticUpLimit.SetCaption(str);
	sprintf(str, "Up %.2f", tSysSETDisp.fMaxValue);

	//定值单位
	m_StaticAsUnit.SetCaption(tSysSETDisp.strUnit);

	//正负号
	LONG symoble = 0; 
	//判断定值类型， 是应该edit输入还是combo选择
	if (wSetNameCount > 0) //combo
	{
		m_bIsCombo = TRUE;
		m_ComboAsCombo.SetVisible(TRUE);
		m_EditAsInput.SetVisible(FALSE);
		SetFocus(m_ComboAsCombo.GetTabOrder());

		m_ComboAsCombo.m_nX = m_EditAsInput.m_nX;
		m_ComboAsCombo.m_nY = m_EditAsInput.m_nY;
		m_ComboAsCombo.m_nYSize = m_EditAsInput.m_nYSize;
		m_ComboAsCombo.m_nXSize = m_EditAsInput.m_nXSize;

		int nItemCount = wSetNameCount;
		m_ComboAsCombo.ResetContent();
		int nItem;

		for (int i=0; i<nItemCount; i++)
		{
			nItem = m_ComboAsCombo.AddString(strSetDataName[i]);
			m_ComboAsCombo.SetItemData(nItem, i);
			if(i == CLcdApp::m_fpSysSetList[m_wSysSetIndex])
				m_ComboAsCombo.SelectItem(i);
		}

	}
	else
	{
		m_bIsCombo = FALSE;
		m_ComboAsCombo.SetVisible(FALSE);
		m_EditAsInput.SetVisible(TRUE);
		SetFocus(m_EditAsInput.GetTabOrder());
		//定值是否有负值， 会出现负数的话要显示正负号
		if(tSysSETDisp.fMinValue <0)
		{
			symoble= 1;
			m_EditAsInput.SetIsShowNegative(TRUE);
		}
		else
			m_EditAsInput.SetIsShowNegative(FALSE);
		
		
		float tmpf = tSysSETDisp.fMaxValue;
		if(abs(tSysSETDisp.fMinValue) > tmpf)
			tmpf = abs(tSysSETDisp.fMinValue);
		
		sprintf(strFormat, "%s%d%s", "%.", tSysSETDisp.nDec, "f");
		
		//用于统计长度以确定edit的最大位数
		sprintf(str, strFormat, tmpf);
		LONG len = strlen(str) + symoble;
		
		//设置输入限制
		m_EditAsInput.SetMaxLen(len);
		m_EditAsInput.SetValueRange(tSysSETDisp.fMaxValue, tSysSETDisp.fMinValue);
		m_EditAsInput.SetDecimalNum(tSysSETDisp.nDec);
		
		m_EditAsInput.SetValue(CLcdApp::m_fpSysSetList[m_wSysSetIndex]);
		m_EditAsInput.SetCursorPos(0);
		m_EditAsInput.UpdateData(FALSE);
	}
	return TRUE;
}

void CLcdWndASModify::OnWndUpDown(LONG UpDown)
{
	WORD num = DB_GetSystemSETNum();
	if(num <= 0)
		return;
	m_wSysSetIndex = m_wSysSetIndex + num + UpDown;
	m_wSysSetIndex = m_wSysSetIndex%num;
	OnWndInit();
	OnPaint();
	UpdateWindow();
}
void CLcdWndCF::ChgCF()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndCF;
	if(!HMISaveCFToDB())
		HMIMsgBox(pWnd->tr(""), pWnd->tr(""), pWnd, pWnd, HMI_MB_OKCANCEL);
	else
		pWnd->Show();
}

//保存系数
void CLcdWndCF::OnWndEnter()
{
	if(!CHmiApp::m_bCFListModified)
		return;
	YES_NO_PTR pfn = (YES_NO_PTR)ChgCF;
	HMIMsgBox(tr("修改系数"),  tr("确定要修改系数?"),  this, this, HMI_MB_OKCANCEL, pfn);
}
void CLcdWndOutputTest::OnEnterEditOutputNum()
{

}

void CLcdWndSetting::OnEnterBtnASModify()
{
	//读取辅助定值到缓冲区
	LONG rc = DB_GetSystemSETFloatValue(SET_VALUE_ALL, CLcdApp::m_fpSysSetList);	
	CLcdApp::m_WndASModify.m_wSysSetIndex = 0;
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("定值"),tr("读公用定值失败!"), this,this);
	}
	else
	{
		//密码窗口
		HMIPasswordBox(tr("输入密码"), tr("输入密码"),
			&(CLcdApp::m_WndASModify), this,0);
	}

}

void CLcdWndASModify::OnWndAddSub(LONG AddSub)
{
	float fValue;
	if(m_bIsCombo)
	{
		LONG nItem = m_ComboAsCombo.GetCurSel();
		LONG lval = m_ComboAsCombo.GetItemData(nItem);
		fValue = lval;
	}
	else
	{
		fValue = m_EditAsInput.GetValue();
	}
	
	CHmiApp::m_fpSysSetList[m_wSysSetIndex] = fValue;
	float r = CHmiApp::m_fpSysSetList[m_wSysSetIndex];
	
	CHmiApp::m_bSysSetListModified = TRUE;

}


//读取BIPara到缓冲区
void CLcdWndInputSetting::OnEnterBtnSoeProperty()
{
	
	//读取BIPara到缓冲区
	if(!HMIReadBIControlToBuf())
	{ 
		//读取失败，不自动跳转到的m_BtnSoeProperty按钮的EnterWnd
		m_BtnSoeProperty.DisableAutoEnterWnd();
		//失败提示
		HMIMsgBox(tr("SOE属性"), tr("读取SOE属性失败!"), this, this);
	}
	else
	{
		m_BtnSoeProperty.EnableAutoEnterWnd();
	}

}

void CLcdWndInputSetting::OnEnterBtnBIFilter()
{
	//初始化DiFilterTime的缓冲区
	if(!HMIReadDIFilterTimeToBuf())
	{ 
		//读取失败，不自动跳转到的按钮的EnterWnd
		m_BtnBIFilter.DisableAutoEnterWnd();
		//失败提示
		HMIMsgBox(tr("开入去抖时间"), tr("读取开入去抖时间失败!"), this, this);
	}
	else
	{
		m_BtnBIFilter.EnableAutoEnterWnd();
	}
}
void CLcdWndInputSetting::OnEnterBtnBiNo()
{
	//初始化BI是否取反缓冲区
	if(!HMIReadBIControlToBuf())
	{ 
		//读取失败，不自动跳转到的按钮的EnterWnd
		m_BtnBiNo.DisableAutoEnterWnd();
		//失败提示
		HMIMsgBox(tr("开入属性"), tr("读取开入属性失败!"), this, this);
	}
	else
	{
		m_BtnBiNo.EnableAutoEnterWnd();
	}

}
void CLcdWndMainMenu::OnEnterBtnInput()
{
	g_BIDlg.DoModal(this, this);
}

void CLcdWndMainMenu::OnEnterBtntest()
{
	HMIPasswordBox(tr("请输入密码"), tr("请输入密码:"), &(CLcdApp::m_WndTest), this,1);
	m_Btntest.SetEnterWnd(NULL);
}


void CLcdWndAutoJustStep1::OnWndEnter()
{
	//发送死区校正消息
	OS_PostMessage(dwAITaskID,SM_AUTOADJUST, RtosGetCurrentTaskId(),0,0, 0);
	
	//显示提示窗口
	HMIMsgBox(tr("校正系数"), tr("正在校正死区系数,\n请等待..."));

}


BOOL CLcdWndMainMenu::OnWndInit()
{
	//不是语言0的话就设置按钮的标题， 并且不显示位图
	//2011-5-4 
	SetEscWnd(CHmiApp::m_pRunWnd); 
	if(DB_GetLanguageNo()!= 0)
	{
		m_BtnMeasure.SetBitmap(NULL,0,0,0,0,0);
		m_BtnSetting.SetBitmap(NULL,0,0,0,0,0);
		m_BtnInput.SetBitmap(NULL,0,0,0,0,0);
		m_BtnSoftSwitch.SetBitmap(NULL,0,0,0,0,0);
		m_BtnReport.SetBitmap(NULL,0,0,0,0,0);
		m_BtnSystem.SetBitmap(NULL,0,0,0,0,0);
		m_Btntest.SetBitmap(NULL,0,0,0,0,0);
		m_BtnAbout.SetBitmap(NULL,0,0,0,0,0);


		SetUDFun(MoveFocus);
		SetUDOffset(1);
		SetLRFun(MoveFocus);
		SetLROffset(1);
		m_BtnMeasure.SetPosition(27,34,207,59,0);
		m_BtnMeasure.SetTabOrder(1);
		m_BtnMeasure.SetFont(16);
		m_BtnMeasure.SetOwner(this);
		m_BtnMeasure.SetCaption(tr("测量查看"));
		m_BtnMeasure.SetHaveEnterFun(FALSE);
		m_BtnMeasure.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndMeasure));
		m_BtnInput.SetPosition(27,94,207,119,0);
		m_BtnInput.SetTabOrder(3);
		m_BtnInput.SetFont(16);
		m_BtnInput.SetOwner(this);
		m_BtnInput.SetCaption(tr("开入查看"));
		m_BtnInput.SetHaveEnterFun(TRUE);
		m_BtnInput.SetEnterWnd(NULL);
		m_BtnSetting.SetPosition(27,64,207,89,0);
		m_BtnSetting.SetTabOrder(2);
		m_BtnSetting.SetFont(0);
		m_BtnSetting.SetOwner(this);
		m_BtnSetting.SetCaption(tr("保护定值"));
		m_BtnSetting.SetHaveEnterFun(FALSE);
		m_BtnSetting.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
		m_BtnSoftSwitch.SetPosition(27,124,207,149,0);
		m_BtnSoftSwitch.SetTabOrder(4);
		m_BtnSoftSwitch.SetFont(0);
		m_BtnSoftSwitch.SetOwner(this);
		m_BtnSoftSwitch.SetCaption(tr("保护压板"));
		m_BtnSoftSwitch.SetHaveEnterFun(TRUE);
		m_BtnSoftSwitch.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSoftSwitch));
		m_BtnReport.SetPosition(27,34,207,59,1);
		m_BtnReport.SetTabOrder(5);
		m_BtnReport.SetFont(16);
		m_BtnReport.SetOwner(this);
		m_BtnReport.SetCaption(tr("报告查看"));
		m_BtnReport.SetHaveEnterFun(FALSE);
		m_BtnReport.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
		m_Btntest.SetPosition(27,94,207,119,1);
		m_Btntest.SetTabOrder(7);
		m_Btntest.SetFont(16);
		m_Btntest.SetOwner(this);
		m_Btntest.SetCaption(tr("功能测试"));
		m_Btntest.SetHaveEnterFun(TRUE);
		m_Btntest.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
		m_BtnSystem.SetPosition(27,64,207,89,1);
		m_BtnSystem.SetTabOrder(6);
		m_BtnSystem.SetFont(16);
		m_BtnSystem.SetOwner(this);
		m_BtnSystem.SetCaption(tr("系统设置"));
		m_BtnSystem.SetHaveEnterFun(TRUE);
		m_BtnSystem.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
		m_BtnStatus.SetPosition(27,124,207,149,1);
		m_BtnStatus.SetTabOrder(9);
		m_BtnStatus.SetFont(16);
		m_BtnStatus.SetOwner(this);
		m_BtnStatus.SetCaption(tr("系统状态"));
		m_BtnStatus.SetHaveEnterFun(FALSE);
		m_BtnStatus.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
		m_BtnAbout.SetPosition(27,34,207,59,2);
		m_BtnAbout.SetTabOrder(9);
		m_BtnAbout.SetFont(16);
		m_BtnAbout.SetOwner(this);
		m_BtnAbout.SetCaption(tr("关    于"));
		m_BtnAbout.SetHaveEnterFun(TRUE);
		m_BtnAbout.SetEnterWnd(NULL);
	}
	else
	{
		m_BtnMeasure.SetCaption(tr(""));
		m_BtnMeasure.SetBitmap(measure,2,3,85,29,12);
		m_BtnSetting.SetBitmap(setting,2,3,85,28,12);
		m_BtnSetting.SetCaption(tr(""));
		m_BtnInput.SetBitmap(input,2,3,85,28,12);
		m_BtnInput.SetCaption(tr(""));
		m_BtnSoftSwitch.SetBitmap(softswitch,2,3,85,28,12);
		m_BtnSoftSwitch.SetCaption(tr(""));
		m_BtnReport.SetBitmap(report,2,3,85,28,12);
		m_BtnReport.SetCaption(tr(""));
		m_BtnSystem.SetBitmap(set,5,2,85,28,12);
		m_BtnSystem.SetCaption(tr(""));
		m_Btntest.SetCaption(tr(""));		
		m_Btntest.SetBitmap(test,2,3,85,28,12);
		m_BtnAbout.SetCaption(tr(""));		
		m_BtnAbout.SetBitmap(about,2,3,85,28,12);		
		m_BtnStatus.SetCaption(tr(""));		
		m_BtnStatus.SetBitmap(status,2,3,85,28,12);
	}
	return TRUE;
	

}

void CLcdWndSysSetting::OnEnterBtnBOTime()
{
	if(!HMIReadBOTimeToBuf())
	{
		//读取失败，提示错误
		HMIMsgBox(tr("设置"),tr("读取遥控保持时间错误!"),this,this);
		//不自动调整的按钮的Enterwnd
		m_BtnBOTime.DisableAutoEnterWnd();
	}
	else
	{
		m_BtnBOTime.EnableAutoEnterWnd();
	}
}
void CLcdWndBOTime::OnWndEnter()
{
	if(!CHmiApp::m_bBOTimeModified)
		return;
	char strCaption[64], strMsg[64];

	sprintf(strCaption, "%s", tr("修改遥控保持时间"));
	sprintf(strMsg, "%s", tr("确定要保存修改的内容?"));


	YES_NO_PTR pfn = (YES_NO_PTR)SaveBOLastTime;
	HMIMsgBox(strCaption,  strMsg,  this, this, HMI_MB_OKCANCEL, pfn);
}
void CLcdWndBOTime::SaveBOLastTime()
{
	if(!HMISaveBOTimeToDB())
	{
		HMIMsgBox(CHmiObj::tr("遥控持续时间","CLcdWndBOTime"),  CHmiObj::tr("保存遥控持续时间失败!","CLcdWndBOTime"),
			&(CLcdApp::m_WndBOTime) ,&(CLcdApp::m_WndBOTime), HMI_MB_OK);
	}
	else
	{
		CLcdApp::m_WndBOTime.Show();
	}
}

void CLcdWndManualREC::OnWndEnter()
{
	TAbsTime abstime;
	ReadAbsTime(&abstime);
	OS_PostMessage(g_hDbaseTaskID, SM_STARTDIST, dwRelayDataPtr,*AS_pnRECD*16/10,abstime.Lo,abstime.Hi);
	CHmiWnd::OnWndEnter();

}

void CLcdWndLanTest::OnEnterBtnStart()
{
	OS_PostMessage(g_hWdogTaskID, SM_LEDCHECK, 0,0,0,0);
	
}

BOOL CLcdWndOutputTest::OnWndInit()
{
	m_EditOutputNum.UpdateData();
	int nNum = m_EditOutputNum.GetValue();
	char str[32];
	TTC_Disp TC_Disp;
	DB_GetTCDisp(nNum-1,&TC_Disp);
	
	sprintf(str, TR_GetTCName(&TC_Disp,nNum-1));
	m_StaticOutputName.SetCaption(str);
	m_StaticOutputName.ReDraw();
	return TRUE;
}
void CLcdWndMainMenu::OnEnterBtnSoftSwitch()
{
	//关闭软压板窗口，确保刷新
	CLcdApp::m_WndSoftSwitch.Close();
	//读取软压板数据到缓存区，如果不成功则提示读取失败
	if(!HMIReadSWToBuf())
		HMIMsgBox(tr("软压板"), tr("读软压板数据失败!"), this, this);
	else
	{
		CLcdApp::m_WndSoftSwitch.Show();
	}
}

void CLcdWndMainMenu::OnEnterBtnSystem()
{
	m_BtnSystem.SetEnterWnd(NULL);
	HMIPasswordBox(tr("请输入密码"), tr("请输入密码:"), &(CLcdApp::m_WndSysSetting), this,1);
}

void CLcdWndReport::OnEnterBtnAlarmReport()
{
	CLcdApp::m_WndAlarmReport.Close();
}

void CLcdWndReport::OnEnterBtnSoe()
{
	//防止以前没有关闭
	CLcdApp::m_WndSoeReport.Close();

}

void CLcdWndReport::OnEnterBtnStartReport()
{
	CLcdApp::m_WndStartReport.Close();
}

//HMIContrastBox(this, this);




void CLcdWndsyspara::OnEnterBtnFVDH()
{
	char Msg[64];
	BOOL DispMode;
	DispMode = DB_IsFDShowFirst();
	YES_NO_PTR pfn = (YES_NO_PTR)ModifyFVDispMode;
	if(DispMode == TRUE)
	{
		sprintf(Msg, tr("当前故障电量显示一次值\n\n要改为二次值吗?"));
		HMIMsgBox(tr("故障电量显示方式"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("当前故障电量显示二次值\n\n要改为一次值吗?"));
		HMIMsgBox(tr("故障电量显示方式"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
	}
}
void CLcdWndsyspara::ModifyFVDispMode()
{
	BOOL DispMode;
	DispMode = DB_IsFDShowFirst();
	if(DispMode == TRUE)
	{
		DB_SetFDShowMode(FD_SHOWMODE_SECOND);
	}
	else
	{
		DB_SetFDShowMode(FD_SHOWMODE_FIRST);
	}
	CLcdApp::m_Wndsyspara.Show();
}

void CLcdWndsyspara::ModifyIntKWCaluMode()
{
	STATUS rc;
	BOOL IsCaluIntKW = DB_IsCaculateIntegral();
	if(IsCaluIntKW == TRUE)
		rc = DB_SetCICaculate(FALSE);
	else
		rc = DB_SetCICaculate(TRUE);

//	if(DB_ERR_OK!=rc)
//	{
//		HMIMsgBox(tr("积分电度"), tr("设置积分电度错误!"), this, this);
//		return;
//	}	
	CLcdApp::m_Wndsyspara.Show();

	
}


void CLcdWndsyspara::OnEnterBtnContrast()
{
	HMIContrastBox(this,this);
}


void CLcdWndPulseConst::OnEnterListPulseConst()
{
	if(CHmiApp::m_bCIConstModified)
	{
		YES_NO_PTR pfn = (YES_NO_PTR)SavePulseConst;
		HMIMsgBox(tr("脉冲常数"),  tr("确定要修改脉冲常数?\n修改后请复位装置"),  this, this, HMI_MB_OKCANCEL, pfn);
	}
}
void CLcdWndPulseConst::SavePulseConst()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndPulseConst;
	
	//保存函数出现部分保存正确，部分保存错误也返回错误
	//所以不管保存函数返回成功否都发改消息，
	if(!HMISaveCIPluseToDB())
		HMIMsgBox(pWnd->tr("脉冲常数"), pWnd->tr("保存脉冲常数失败!"), pWnd, pWnd, HMI_MB_OKCANCEL);
	else
		pWnd->Show();
}

BOOL CLcdWndCiBase::OnWndInit()
{		
	char format[16];
	char str[32];

	sprintf(format, "%s%df", "%08.", 2);	
	sprintf(str,format,(float)CHmiApp::m_dwpCIBase[0]/100);
	m_EditIntKW1.SetCaption(str);
	sprintf(str,format,(float)CHmiApp::m_dwpCIBase[1]/100);
	m_EditIntKW2.SetCaption(str);
	sprintf(str,format,(float)CHmiApp::m_dwpCIBase[2]/100);
	m_EditPulseKW1.SetCaption(str);
	sprintf(str,format,(float)CHmiApp::m_dwpCIBase[3]/100);
	m_EditPulseKW2.SetCaption(str);







/*
	float value;
	value = CHmiApp::m_dwpCIBase[0]/100;
	m_EditIntKW1.SetValue(value);
	value = CHmiApp::m_dwpCIBase[1]/100;
	m_EditIntKW2.SetValue(value);	
	value = CHmiApp::m_dwpCIBase[2]/100;
	m_EditPulseKW1.SetValue(value);
	value = CHmiApp::m_dwpCIBase[3]/100;
	m_EditPulseKW2.SetValue(value);	

	m_EditIntKW1.UpdateData(FALSE);
	m_EditIntKW2.UpdateData(FALSE);
	m_EditPulseKW1.UpdateData(FALSE);
	m_EditPulseKW2.UpdateData(FALSE);
	*/	
}
void CLcdWndCiBase::OnWndEnter()
{	
	STATUS rc;
	m_EditIntKW1.UpdateData(TRUE);
	m_EditIntKW2.UpdateData(TRUE);	
	m_EditPulseKW1.UpdateData(TRUE);
	m_EditPulseKW2.UpdateData(TRUE);
	CIbase[0]=(DWORD)(m_EditIntKW1.GetValue()*100);
	CIbase[1]=(DWORD)(m_EditIntKW2.GetValue()*100);
	CIbase[2]=(DWORD)(m_EditPulseKW1.GetValue()*100);
	CIbase[3]=(DWORD)(m_EditPulseKW2.GetValue()*100);
	char Msg1[64];
	sprintf(Msg1, tr("确定要修改电度底数?"));
	YES_NO_PTR pfn = (YES_NO_PTR)SaveCiBase;
	HMIMsgBox(tr("电度底数"),  Msg1,  this, this, HMI_MB_OKCANCEL, (YES_NO_PTR)pfn);
	
}
void CLcdWndCiBase::SaveCiBase()
{
	STATUS rc;	
	CLcdWndCiBase *pWnd = &(CLcdApp::m_WndCiBase);

	for(LONG i=0;i<4;i++)
	{
		rc = DB_SetCI(i, pWnd->CIbase[i]);
		
		if (rc != DB_ERR_OK)
		{
			HMIMsgBox(pWnd->tr("电度底数"), pWnd->tr("保存电度底数失败!"), pWnd, pWnd);
			break;
		}	
	}
	CLcdApp::m_WndCIPara.Show();
}
BOOL CLcdWndsyspara::OnWndInit()
{	
	if(!g_bDisplayEEpromButton)
		m_BtnDefaultPara.SetVisible(FALSE);
	else
		m_BtnDefaultPara.SetVisible(TRUE);
}

void CLcdWndsyspara::OnEnterBtnDefaultPara()
{
	CLcdApp::m_Wndsyspara.Close();
	//密码窗口
	HMIPasswordBox(tr("输入密码"),  tr("输入密码"),&(g_DefaultPara), this,1);
}

void CLcdWndSysSetting::OnEnterBtnLanguage()
{
	g_LangDlg.DoModal(this);

}

void CLcdWndAddress::OnEnterBtnSerialSet()
{
	g_SerialSet.DoModal(this);

}

void CLcdWndWStatus::OnEnterBtnDevStatus()
{
	g_SysStatus.DoModal(this);

}

void CLcdWndWStatus::OnEnterBtnComStatus()
{
	g_ComStatusDlg.DoModal(this);
}

void CLcdWndEthadd::OnEnterBtnetha()
{
	g_wEth = 0x0a; //A网
	g_ModifyIP.DoModal(this);
}

void CLcdWndEthadd::OnEnterBtnethb()
{
	g_wEth = 0x0b; //B网
	g_ModifyIP.DoModal(this);
}

void CLcdWndMainMenu::OnEnterBtnAbout()
{
	g_Version.DoModal(this);

}
void CLcdWndTest::OnEnterBtnFactoryData()
{
	m_BtnFactoryData.SetEnterWnd(NULL);
	HMIPasswordBox(tr("请输入密码"), tr("请输入密码:"), &(CLcdApp::m_WndfactorySet), this,3);

}

void CLcdWndfactorySet::OnEnterBtnManualCoef()
{
	LONG rc=DB_GetCFValue(CF_VALUE_ALL, CHmiApp::m_npCFList);
	if(rc!=DB_ERR_OK)
	{
		HMIMsgBox(tr("系数"),tr("读系数失败!"), this);
	}
	else
		CLcdApp::m_WndCF.Show();
}

void CLcdWndfactorySet::OnEnterBtnAutoCoef()
{
	g_WndAutoJustStep1.Show();
}

void CLcdWndCIPara::OnEnterBtnDIProperty()
{
	char Msg[64];
	BOOL IsPulseKW;
	STATUS rc;
	TDIPara	pDI;
	
	rc = DB_GetDIPara(0,&pDI);

	if(((pDI.wClass)&DI_ATTR_TYPEMASK) ==DI_TYPE_SP)
		IsPulseKW = FALSE;
	else
		IsPulseKW = TRUE;
	YES_NO_PTR pfn = (YES_NO_PTR)ModefyDIProperty;
	if(IsPulseKW == TRUE)
	{
		sprintf(Msg, tr("前两路开入的属性为电度\n\n要改为遥信吗?\n更改后请复位装置"));
		HMIMsgBox(tr("开入属性"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("前两路开入的属性为遥信\n\n要改为电度吗?\n更改后请复位装置"));
		HMIMsgBox(tr("开入属性"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
}

void CLcdWndCIPara::OnEnterBtnIntegralKW()
{
	char Msg[64];
	BOOL IsCaluIntKW = DB_IsCaculateIntegral();
	YES_NO_PTR pfn = (YES_NO_PTR)ModifyIntKWCaluMode;
	if(IsCaluIntKW == TRUE)
	{
		sprintf(Msg, tr("当前装置计算积分电度\n\n要取消计算积分电度吗?"));
		HMIMsgBox(tr("积分电度"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("当前装置不计算积分电度\n\n要计算积分电度吗?"));
		HMIMsgBox(tr("积分电度"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
}

void CLcdWndCIPara::OnEnterBtnCIbase()
{
	BOOL IsCaluIntKW = DB_IsCaculateIntegral();;
	WORD wPulseKWNum = DB_GetPulseCINum();

	if((wPulseKWNum==0)&&(IsCaluIntKW == FALSE))
	{
		HMIMsgBox(tr("电度底数"),tr("当前本装置没有设置脉冲电度且\n\n且不计算积分电度!"),this, this, HMI_MB_OK);
		m_BtnCIbase.DisableAutoEnterWnd();
		return;
	}
	//初始化脉冲常数缓冲区
	if(!HMIReadCIBaseToBuf())
	{ 
		//读取失败，不自动跳转到的按钮的EnterWnd
		m_BtnCIbase.DisableAutoEnterWnd();
		//失败提示
		HMIMsgBox(tr("电度底数"), tr("读取电度底数失败!"), this, this);
	}
	else
	{
		m_BtnCIbase.EnableAutoEnterWnd();
	}
}

void CLcdWndCIPara::OnEnterBtnPulseConst()
{
	char Msg[64];
	BOOL IsPulseKW;
	STATUS rc;
	TDIPara	*pDI;
	
	if(DB_GetPulseCINum()==0)
	{
		HMIMsgBox(tr("脉冲常数"),tr("当前本装置没有设置脉冲电度!"),this, this, HMI_MB_OK);
		m_BtnPulseConst.DisableAutoEnterWnd();
		return;
	}
	
	//初始化脉冲常数缓冲区
	if(!HMIReadCIPluseToBuf())
	{ 
		//读取失败，不自动跳转到的按钮的EnterWnd
		m_BtnPulseConst.DisableAutoEnterWnd();
		//失败提示
		HMIMsgBox(tr("脉冲常数"), tr("读取脉冲常数失败!"), this, this);
	}
	else
	{
		m_BtnPulseConst.EnableAutoEnterWnd();
	}
}
void CLcdWndCIPara::ModefyDIProperty()
{	
	CHmiWnd *pWnd = &(CLcdApp::m_WndCIPara);
	STATUS rc;
	TDIPara	pDI;	
	for(LONG i=0;i<2;i++)
	{
		rc = DB_GetDIPara(i,&pDI);		
		if(DB_ERR_OK !=rc)
		{		
			HMIMsgBox(pWnd->tr("开入属性"),pWnd->tr("修改开入属性错误!"),pWnd, pWnd, HMI_MB_OK);
			break;
		}
		if(((pDI.wClass)&DI_ATTR_TYPEMASK) ==DI_TYPE_SP)
		{
			pDI.wClass &=~DI_ATTR_TYPEMASK;
			pDI.wClass |= DI_TYPE_PLUS;
		}
		else
		{			
			pDI.wClass &=~DI_ATTR_TYPEMASK;
			pDI.wClass |= DI_TYPE_SP;
		}
		
		rc = DB_WriteDIPara(i, &pDI);		
		if(DB_ERR_OK !=rc)
		{		
			HMIMsgBox(pWnd->tr("开入属性"),pWnd->tr("修改开入属性错误!"),pWnd, pWnd, HMI_MB_OK);
			return;
		}
		rc = DB_WriteDIParaConfirm(i);		
		if(DB_ERR_OK !=rc)
		{		
			HMIMsgBox(pWnd->tr("开入属性"),pWnd->tr("修改开入属性错误!"),pWnd, pWnd, HMI_MB_OK);
			return;
		}		
	}
	CLcdApp::m_WndCIPara.Show();
}
void CLcdWndCIPara::ModifyIntKWCaluMode()
{
	STATUS rc;
	BOOL IsCaluIntKW = DB_IsCaculateIntegral();
	if(IsCaluIntKW == TRUE)
		rc = DB_SetCICaculate(FALSE);
	else
		rc = DB_SetCICaculate(TRUE);

	CLcdApp::m_WndCIPara.Show();	
}
void CLcdWndPortStatus::OnEnterBtnProtocol()
{
	if(!HMIReadPortParaToBuf())
	{ 
		//读取失败，不自动跳转到的按钮的EnterWnd
		//m_BtnPortStatus.DisableAutoEnterWnd();
		//失败提示
		HMIMsgBox(tr("端口参数"), tr("读取端口参数失败!"), this, this);
	}
	else
	{
		g_WndPortPara.DoModal(this);
	}
}

void CLcdWndPortStatus::OnEnterBtnPort()
{
	g_WndPortProtocol.DoModal(this);
}
void CLcdWndsyspara::OnEnterBtnIRGBType()
{
	char Msg[64];
	BOOL flag = DB_WhichIRGB();
	YES_NO_PTR pfn = (YES_NO_PTR)ModifyIRGBType;
	if(flag)
	{
		sprintf(Msg, tr("当前时钟源格式为DK3511\n\n要设置为KF6511格式吗?"));
		HMIMsgBox(tr("时钟源格式"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("当前时钟源格式为KF6511\n\n要设置为DK3511格式吗?"));
		HMIMsgBox(tr("时钟源格式"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
	}

}
void CLcdWndsyspara::ModifyIRGBType()
{	
	BOOL flag = DB_WhichIRGB();
	if(flag)
		DB_SetIRGBType(FALSE);
	else
		DB_SetIRGBType(TRUE);
	CLcdApp::m_Wndsyspara.Show();
}

