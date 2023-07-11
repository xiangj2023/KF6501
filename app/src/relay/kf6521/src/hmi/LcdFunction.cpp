
//{{Begin of Describtion of LcdFunction.cpp
/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/
/*************************************************************************
* 
*   �ļ����ƣ�
*			LCDFunction.cpp
* 
*   ���ģ�飺Һ��Ӧ�ó���ͷ�ļ�
*
* 
*	����	Һ���������ඨ��
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
*		 Һ���������	2009-02-26		��ʼ����
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
	nDispID = nItem + m_ListSoeReport.GetItemBase()+1; //��ʼ

	//�õ���������ݿ��ʶ��
	WORD wIndex = (WORD)m_ListSoeReport.GetItemData(nItem);
	if (wIndex == HMI_LIST_NORECORD)
	{
		return;
	}
	if(m_SoeDetaildlg.ReadReport(wIndex,nDispID))
		m_SoeDetaildlg.DoModal(this);
	else
		HMIMsgBox(tr("SOE����"),tr("��SOE����ʧ��!"),this,this);

}
*/
void CLcdWndMeasure::OnEnterBtnHMeasure()
{		
	char DlgCaption[64];
	sprintf(DlgCaption, tr("һ�β����ֵ\n"));	

	WORD whAIIndex[] = {M_UA,M_UB,M_UC,M_UAB,M_UBC,M_UCA,M_U1,M_U2,M_U3,M_IA,M_IB,M_IC,M_I0,M_I1}; 
	g_AIDlg.DoModal(this, this, hAI,14,whAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnLMeasure()
{	
	char DlgCaption[64];
	sprintf(DlgCaption, tr("���β����ֵ\n"));	
	WORD wlAIIndex[] = {M_UA,M_UB,M_UC,M_UAB,M_UBC,M_UCA,M_U1,M_U2,M_U3,M_IA,M_IB,M_IC,M_I0,M_I1,M_P,M_Q,M_COS,M_F1,M_F2};	
	g_AIDlg.DoModal(this, this, lAI,19,wlAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnPML()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("���βౣ��ֵ\n"));	
	WORD wplAIIndex[] = {RM_UA,RM_UB,RM_UC,RM_UAB,RM_UBC,RM_UCA,RM_U0,RM_UL1,RM_UL2,RM_UL3, \
		RM_IA,RM_IB,RM_IC,RM_I0,RM_I1,RM_U2,RM_U05,RM_I05};	
	g_AIDlg.DoModal(this, this, plAI,18,wplAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnAngle()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("��ѹ������λ\n"));	
	WORD wplAIIndex[] = {RM_AUA,RM_AUB,RM_AUC,RM_AUAB,RM_AUBC,RM_AUCA,RM_AUL1,RM_AUL2,RM_AUL3, \
		RM_AIPA,RM_AIPB,RM_AIPC,RM_AIP1,RM_AIMA,RM_AIMB,RM_AIMC,RM_AU01,RM_AI01,RM_AU05,RM_AI05};	
	g_AIDlg.DoModal(this, this, plAI,20,wplAIIndex,DlgCaption);
}

void CLcdWndMeasure::OnEnterBtnsyc()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("��ͬ��\n"));	
	WORD wplAIIndex[] = {RM_F1,RM_F2,RM_DIFFU,RM_DIFFA,RM_DIFFDQ};	
	g_AIDlg.DoModal(this, this, plAI,5,wplAIIndex,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnKilowatt()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("ʵʱ���ֵ\n"));	
	g_AIDlg.DoModal(this, this, CI,0,NULL,DlgCaption);
}
void CLcdWndMeasure::OnEnterBtnJDXX()
{
	char DlgCaption[64];
	sprintf(DlgCaption, tr("�ӵ�ѡ��\n"));	
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
	nDispID = nItem + m_ListActionReport.GetItemBase()+1; //��ʼ
	WORD wActionIndex = (WORD)m_ListActionReport.GetItemData(nItem);
	if (wActionIndex == HMI_LIST_NORECORD)
	{
		return;
	}

	//��ȡ������ȷ����ʾ
	if(m_ActionDetailDlg.ReadReport(HIBYTE(wActionIndex), LOBYTE(wActionIndex),nDispID))
		m_ActionDetailDlg.DoModal(this);
		
	else
		HMIMsgBox(tr("��������"), tr("����������ʧ��!"), this, this, HMI_MB_OK);

}

void CLcdWndAlarmReport::OnEnterListAlarmReport()
{
	
	int nItem, nDispID;
	nItem = m_ListAlarmReport.GetCurSel();
	WORD wActionIndex = (WORD)m_ListAlarmReport.GetItemData(nItem);
	nDispID = nItem + m_ListAlarmReport.GetItemBase()+1; //��ʼ
	
	if (wActionIndex == HMI_LIST_NORECORD)
	{
		return;
	}

	//��ȡ��ȷ��ʵ�� 
	if(m_AlarmDetailDlg.ReadReport(wActionIndex,nDispID))
		m_AlarmDetailDlg.DoModal(this);
	else
		HMIMsgBox(tr("�澯����"), tr("���澯����ʧ��!"), this, this, HMI_MB_OK);

}

//��ֵ��ѯ �������봰��
void CLcdWndSettingQueryAreaNumInput::OnWndEnter()
{
	LONG AreaNo;
	m_EditAreaNum.UpdateData();
	AreaNo = (LONG)m_EditAreaNum.GetValue();
	//���ö�ֵ��ѯ�����ò�ѯ�������Ŷ������������
	CLcdApp::m_WndSettingQuery.m_wAreaNo = AreaNo;
	CLcdApp::m_WndconfigQury.m_wAreaNo = AreaNo;
	
	CHmiWnd::OnWndEnter();
}
BOOL CLcdWndSettingQueryAreaNumInput::OnWndInit()
{
	//��ʾ��ǰ�Ķ�ֵ����
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);
	//�������Ҳ��ʾ��ǰ��ֵ����
	m_EditAreaNum.SetValue(nAreaNo);
	m_EditAreaNum.UpdateData(FALSE);
	return TRUE;

}
void CLcdWndSetting::OnEnterBtnSettingQuery()
{
	CLcdApp::m_WndconfigQury.Close();
	CLcdApp::m_WndSettingQuery.Close();

}

//��ֵ�޸�
void CLcdWndSetting::OnEnterBtnsettingModify()
{
	//�ر������޸Ĵ��ڣ� ȷ������ˢ��
	CLcdApp::m_WndconfigModify.Close();
	//���봰��
	HMIPasswordBox(tr("��������"),  tr("��������"),&(CLcdApp::m_WndSettingChgAreaNumInput), this,0);
}

//��ֵ�л�
void CLcdWndSetting::OnEnterBtnsettingExChg()
{
	//���봰��
	HMIPasswordBox(tr("��������"),  tr("��������"),&(CLcdApp::m_WndSettingSwitch), this,0);

}

//��ֵ����
void CLcdWndSetting::OnEnterBtnSettingCopy()
{
	//���봰��
	HMIPasswordBox(tr("��������"),  tr("��������"),&(CLcdApp::m_WndSettingCopy), this,0);

}

//���ò�ѯ
BOOL CLcdWndconfigQury::OnWndInit()
{
	m_ListConfigQuery.SetAreaNo(m_wAreaNo);
	char szCaption1[64];
	
	sprintf(szCaption1, tr("���ò鿴��%d��"), m_wAreaNo);
	SetCaption(szCaption1);
	return TRUE;
}

//��ֵ��ѯ
BOOL CLcdWndSettingQuery::OnWndInit()
{
	m_ListSettingQuery.SetAreaNo(m_wAreaNo);
	char szCaption1[64];
	sprintf(szCaption1, tr("��ֵ�鿴��%d��"), m_wAreaNo);
	SetCaption(szCaption1);
	return TRUE;
}
WORD CLcdWndconfigModify::m_wAreaNo = 0;
//�Զ��庯���� û�п���ʾ�Ķ�ֵʱ��������
void CLcdWndconfigModify::SaveConfig()
{
	//�Ƚ϶�ֵ�������Ƿ��в���ͬ�� ���������ʾҪ�̻�
	WORD r1 = memcmp(CHmiApp::m_byCurCfg,CHmiApp::m_byNewCfg, CFG_COUNT);
	if(r1 != 0)
	{
		CLcdApp::m_WndSettingSave.m_wAreaNo = m_wAreaNo;; //��ֵ���Ŵ���ȥ

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

//���涨ֵ�޸�
void CLcdWndSettingModify::OnWndEnter()
{
	//�Ƚ϶�ֵ�������Ƿ��в���ͬ�� ���������ʾҪ�̻�
	WORD r1 = memcmp(CHmiApp::m_byCurCfg,CHmiApp::m_byNewCfg, CFG_COUNT);
	WORD r2 =  memcmp(CHmiApp::m_fCurSetting, CHmiApp::m_fNewSetting, sizeof(float)*RS_COUNT);

	if(r1 != 0  || r2 != 0)
	{
		CLcdApp::m_WndSettingSave.m_wAreaNo = m_wAreaNo;; //��ֵ���Ŵ���ȥ

		SetEnterWnd(&CLcdApp::m_WndSettingSave);
	}
	else
		SetEnterWnd(&CLcdApp::m_WndSetting);
	
	CHmiWnd::OnWndEnter();

}

void CLcdWndSettingModify::OnWndAddSub(LONG AddSub)
{
	//��Ҫ���û�����Ķ�ֵ���л���
	//�ж��Ǳ༭�����뻹��combo������

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
	WORD wSetNameCount; //��ֵȡֵ��������
	LONG rc;

	WORD wSetNo = CHmiApp::m_wpVisibleSetList[m_wDispSetIndex];

	rc = DB_GetSETDisp(wSetNo, &tSETDisp);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("��ֵ�޸�"),tr("����ֵ��Ϣʧ��!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
		return FALSE;
	}

	BOOL bIsSetDataNameOk = TR_GetSETDataName(&tSETDisp,wSetNo, &strSetDataName[0][0], wSetNameCount);
	if(!bIsSetDataNameOk)
	{
		HMIMsgBox(tr("��ֵ�޸�"),tr("����ֵ��Ϣʧ��!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
		return FALSE;
	}
	
	//�޸ı���
	sprintf(str, tr("�޸Ķ�ֵ: %02d��"), m_wAreaNo);
	SetCaption(str);
	DrawCaption();

	//��ֵ���
	sprintf(str, "%s =", tSETDisp.strSymbol);
	m_StaticSymbol.SetCaption(str);

	
	//��ֵ����
	m_StaticName.SetCaption(TR_GetSETName(&tSETDisp,wSetNo));
	sprintf(str, tr("����%.2f"), tSETDisp.fMinValue);
	m_StaticUpLimit.SetCaption(str);
	sprintf(str, tr("����%.2f"), tSETDisp.fMaxValue);
	m_StaticDownLimit.SetCaption(str);

	//��ֵ��λ
	m_StaticUnit.SetCaption(tSETDisp.strUnit);

	LONG symoble = 0;
	//�ж϶�ֵ���ͣ� ��Ӧ��edit���뻹��comboѡ��
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
	//��ֵ�Ƿ��и�ֵ�� ����ָ����Ļ�Ҫ��ʾ������
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

	//����ͳ�Ƴ�����ȷ��edit�����λ��
	sprintf(str, strFormat, tmpf);
	LONG len = strlen(str) + symoble;

    //������������
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
	
	//�޸ĵ����ø��µ���ֵ������
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

	//��ֵ����
	if(!HMISaveSetToDB(m_wAreaNo))
	{
		HMIMsgBox(tr("��ֵ����"),tr("���涨ֵʧ��!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
		DB_GetSETFloatValue(m_wAreaNo, SET_VALUE_ALL, CHmiApp::m_fCurSetting);
		m_Btnok.DisableAutoEnterWnd();
		return;
	}
//	DB_SetSETValue(m_wAreaNo,SET_VALUE_ALL, CHmiApp::m_fNewSetting);
//	DB_SetSETValueConfirm(m_wAreaNo,SET_VALUE_ALL, DB_EXECUTE);
	 DB_GetSETFloatValue(m_wAreaNo, SET_VALUE_ALL, CHmiApp::m_fCurSetting);


	 WORD wCurAreaNO = DB_GetCurrentSETAreaNo();

	 //�޸ĵ��Ƿ��ǵ�ǰ��ֵ��
	 if(wCurAreaNO != m_wAreaNo)
	 {
		 m_Btnok.SetEnterWnd(&CLcdApp::m_WndSettingSwitchConfirm);
		 //��ֵ�̻���ȷ���Ƿ�Ҫ�л���ֵ
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
	//���ı�������ʾ��ǰ��ֵ����
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
	sprintf(Msg1, tr("ȷ��Ҫ�������ƶ�ֵ��%d��\n��ֵ��%d-%d ?"), m_wSourArea, m_wStartArea, m_wEndArea);
	YES_NO_PTR pfn = (YES_NO_PTR)CopyAreaSet;
	HMIMsgBox(tr("��ֵ����"),  Msg1,  this, this, HMI_MB_OKCANCEL, (YES_NO_PTR)pfn);
}
//����������ֵ
void CLcdWndSettingCopy::CopyAreaSet()
{
	//����ֵ
	float fvalue[256];
	STATUS rc = 0;
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndSettingCopy;
	rc=DB_GetSETFloatValue(m_wSourArea,  SET_VALUE_ALL, fvalue);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(pWnd->tr("��ֵ����"),pWnd->tr("��ֵ����ȡʧ��!"),pWnd, pWnd);
		return;
	}
	if(m_wStartArea>m_wEndArea)
	{
		HMIMsgBox(pWnd->tr("��ֵ����"),pWnd->tr("��ֵ�������ô���!"),pWnd, pWnd);
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
		sprintf(Msg1, pWnd->tr("��ֵ��%d-%d����ʧ��!"),i,m_wEndArea);
		HMIMsgBox(pWnd->tr("��ֵ����"),Msg1,pWnd, pWnd);
	}
}

void CLcdWndSoftSwitch::SaveSoftSwitch()
{
	//��̬������this����Ч�ģ�Ҳ���ܵ��÷Ǿ�̬��Ա����
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndSoftSwitch;
	if(HMISaveSWToDB())
		CLcdApp::m_WndSoftSwitch.Show();
	else
		HMIMsgBox(pWnd->tr("��ѹ��"),pWnd->tr("��ѹ��Ͷ��ʧ��!"),pWnd, pWnd);
}
void CLcdWndSoftSwitch::OnEnterListSoftSwitch()
{
	LONG r = memcmp(CHmiApp::m_byCurSoftSwitch, CHmiApp::m_byNewSoftSwitch, SW_COUNT);
	//����б仯�ű���
	if(r != 0)
	{		
		YES_NO_PTR pfn = (YES_NO_PTR)SaveSoftSwitch;
		HMIMsgBox(tr("��ѹ��"),tr("ȷ��ҪͶ����ѹ��?"),this, this, HMI_MB_OKCANCEL,pfn);		
	}
	else
	{
		//�ر�ѹ�崰����
		Close();
		//��ʾ���˵�����
		CLcdApp::m_WndMainMenu.Show();
	}

}
/*
BOOL CLcdWndSoftSwitch::OnWndInit()
{
	//��ȡ���ڵ���ѹ�建������
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
	//����ʱ��
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


//����DIȥ��ʱ��
void CLcdWndBIFilter::SaveDIFilterTime()
{
	CHmiWnd* pWnd = &CLcdApp::m_WndBIFilter;
	if(!HMISaveDIFilterTimeToDB())
	{
		HMIMsgBox(pWnd->tr("����ȥ��"), pWnd->tr("����ȥ��ʱ�䱣��ʧ��!"), pWnd, pWnd, HMI_MB_OKCANCEL);
	}
	else
	{
		pWnd->Show();
	}
	
	//���溯�����ֲ��ֱ�����ȷ�����ֱ������Ҳ���ش���
	//���Բ��ܱ��溯�����سɹ��񶼷�����Ϣ��
	OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}
void CLcdWndBIFilter::OnWndEnter()
{
	if(!CHmiApp::m_bDIFilterTimeModified)
		return;

	YES_NO_PTR pfn = (YES_NO_PTR)SaveDIFilterTime;
	HMIMsgBox(tr("�޸Ŀ���ȥ��ʱ��"),  tr("ȷ��Ҫ�����޸ĵ�����?"),  this, this, HMI_MB_OKCANCEL, pfn);

	
}
 //���� ң���Ƿ����soe���� �� ��Ӧ���ݿ���wControl-D1
void CLcdWndSoeProperty::SaveBISoePara()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndSoeProperty;
	if(!HMISaveBIControlToDB())
	{
		//��̬������this�����ã�������CLcdApp::m_WndSoeProperty
		HMIMsgBox(pWnd->tr("soe����"), pWnd->tr("����soe����ʧ��!"),pWnd, pWnd);
	}
	else
	{
		pWnd->Show();
	}
	//���溯�����ֲ��ֱ�����ȷ�����ֱ������Ҳ���ش���
	//���Բ��ܱ��溯�����سɹ��񶼷�����Ϣ��
	OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}

void CLcdWndSoeProperty::OnWndEnter()
{
	if(!CHmiApp::m_bBIControlModified)
		return;
	char strCaption[64], strMsg[64];
	sprintf(strCaption, "%s", tr("�޸��Ƿ����Soe"));
	sprintf(strMsg, "%s", tr("ȷ��Ҫ�����޸ĵ�����?"));
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
		
	//��������
	DB_BOTest(wNum-1,wTime);

	CHmiWnd::OnWndEnter();


}
void CLcdWndReportTest::OnWndEnter()
{
	TAbsTime RptTime;
	WORD	 RptNo;
	ReadAbsTime(&RptTime);
	DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&RptTime,FALSE); //�����ݿ���д����������
	RptNo = g_RptSerialNum;
	MakeActRpt(R_COMTEST,RptNo);	
	DB_RelayEnd(RptNo); 			//�����ݿ���д���������

	
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
		HMIMsgBox(tr("CAN����ַ"),tr("�޸�CAN����ַʧ��!"), this);
	}
	m_Btnenter.EnableAutoEnterWnd();
}


void CLcdWndPassChange::OnEnterBtnChgSettingPass()
{
	HMIChgPassBox(tr("����Ա�����޸�"), this,this, 1);
}

void CLcdWndPassChange::OnEnterBtnChgCommonPass()
{
	
	HMIChgPassBox(tr("��ͨ�����޸�"), this,this, 0);
}


void CLcdWndSettingChgAreaNumInput::OnWndEnter()
{
	LONG nAreaNo = m_EditInputAreaNum.GetValue();

	CLcdApp::m_WndSettingModify.m_wAreaNo = nAreaNo;
	CLcdApp::m_WndconfigModify.m_wAreaNo = nAreaNo;
	SetEnterWnd(&CLcdApp::m_WndconfigModify);
	int i;

	//�����õ�������

	if(!HMIReadCFGToBuf(nAreaNo))
	{
		SetEnterWnd(NULL);
		HMIMsgBox(tr("��ֵ�޸�"),tr("������ʧ��!"),this,this);
		return;
	}
	
	//��ȡ��ǰ��ֵ��������
	if(!HMIReadSetToBuf(nAreaNo))
	{
		SetEnterWnd(NULL);
		HMIMsgBox(tr("��ֵ�޸�"),tr("����ֵʧ��!"),this,this);
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
	
	//ʹ�ò��ɹ�
	if(DB_ERR_OK != rc)
	{
		m_BtnOk.DisableAutoEnterWnd();
		HMIMsgBox(tr("��ֵ�л�"), tr("��ֵ�л�ʧ��!"), &CLcdApp::m_WndSetting, &CLcdApp::m_WndSetting);
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
	
	nDispID = nItem + m_ListStartReport.GetItemBase()+1; //��ʼ
	//�õ���������ݿ��ʶ��
	WORD wIndex = (WORD)m_ListStartReport.GetItemData(nItem);
	if (wIndex == HMI_LIST_NORECORD)
	{
		return;
	}
	
	if(m_StartDetailDlg.ReadReport(wIndex,nDispID))
		m_StartDetailDlg.DoModal(this);
	else
		HMIMsgBox(tr("��������"),tr("����������ʧ��!"),this,this);
}
/*
void CLcdWndOperationReport::OnEnterListOperationReport()
{
	
	int nItem,nDispID;
	nItem = m_ListOperationReport.GetCurSel();

	nDispID = nItem + m_ListOperationReport.GetItemBase()+1; //��ʼ
	//�õ���������ݿ��ʶ��
	WORD wIndex = (WORD)m_ListOperationReport.GetItemData(nItem);
	if (wIndex == HMI_LIST_NORECORD)
	{
		return;
	}

	if(m_OperationDetailDlg.ReadReport(wIndex,nDispID))
		m_OperationDetailDlg.DoModal(this);
	else
		HMIMsgBox(tr("��������"),tr("����������ʧ��!"),this,this);
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

	//���¿���ʾ�Ķ�ֵ�б� ���浽ȫ������ CHmiApp::m_wpVisibleSetList
	BOOL res = HMIUpDataVisibleSetList(m_wAreaNo, &nDispSetCount, CHmiApp::m_wpVisibleSetList,FALSE);

	//�Ƿ��п���ʾ�Ķ�ֵ
	if (nDispSetCount ==0)
	{
		HMIMsgBox(tr("ע��"),	tr("û��Ͷ��Ķ�ֵ,�Ƿ�Ҫ����"), 
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
//����ң��ȡ����Ϣ
void CLcdWndBiNot::ChangeBInegation()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndBiNot;
	
	//���溯�����ֲ��ֱ�����ȷ�����ֱ������Ҳ���ش���
	//���Բ��ܱ��溯�����سɹ��񶼷�����Ϣ��
	if(!HMISaveBIControlToDB())
		HMIMsgBox(pWnd->tr("BI�����޸�"), pWnd->tr("����BI����ʧ��!"), pWnd, pWnd, HMI_MB_OKCANCEL);
	else
		pWnd->Show();
	OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}

//ң��ȡ������
void CLcdWndBiNot::OnEnterListBiNot()
{
	if(CHmiApp::m_bBIControlModified)
	{
		YES_NO_PTR pfn = (YES_NO_PTR)ChangeBInegation;
		HMIMsgBox(tr("ң��ȡ��"),  tr("ȷ��Ҫ����ң��ȡ����Ϣ?"),  this, this, HMI_MB_OKCANCEL, pfn);
	}

}
BOOL CLcdWndconfigModify::OnWndInit()
{
	//�����б�
	m_ListConfigModify.SetAreaNo(m_wAreaNo);

	char szCaption[32];
	//�޸ı���
	sprintf(szCaption, tr("�޸�����: %02d��"), m_wAreaNo);
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

//�������Ʊ���,�ֶ��徲̬����
void CLcdWndASModify::SaveSysSetting()
{
	CHmiWnd* pWnd = &CLcdApp::m_WndASModify;
	
	if(!HMISaveSysSetToDB())
	{
		HMIMsgBox(pWnd->tr("���ö�ֵ"), pWnd->tr("���ö�ֵ����ʧ��!"), pWnd,pWnd);
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
	HMIMsgBox(tr("�޸Ĺ��ö�ֵ"),  tr("ȷ��Ҫ�޸Ĺ��ö�ֵ?"),  this, this, HMI_MB_OKCANCEL, pfn);

}

BOOL CLcdWndASModify::OnWndInit()
{

	TSysSET_Disp	tSysSETDisp;
	char str[64];
	char strFormat[32];
	char strSetDataName[MAX_DISP_COUNT][HMI_DBSTRING_SIZE];
	WORD wSetNameCount; //��ֵȡֵ��������

	LONG rc = DB_GetSysSETDisp(m_wSysSetIndex,  &tSysSETDisp);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("���ö�ֵ"), tr("�����ö�ֵ����!"), &CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting);
		return FALSE;
	}
	BOOL bSysSetDataOk = TR_GetSysSETDataName(&tSysSETDisp,m_wSysSetIndex, &strSetDataName[0][0],wSetNameCount);
	if(!bSysSetDataOk)
	{
		HMIMsgBox(tr("���ö�ֵ"), tr("�����ö�ֵ����!"), &CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting);
		return FALSE;
	}
	
	//�޸ı���
	sprintf(str, tr("�޸Ĺ��ö�ֵ"));
	SetCaption(str);

	DrawCaption();

	//��ֵӢ����
	sprintf(str, "%s =", tSysSETDisp.strSymbol);
	m_StaticASSymbol.SetCaption(str);

	
	//��ֵ����
	m_StaticAsName.SetCaption(TR_GetSysSETName(&tSysSETDisp,m_wSysSetIndex));

	sprintf(str, tr("����%.2f"), tSysSETDisp.fMinValue);
	m_StaticDownLimit.SetCaption(str);
	sprintf(str, "Low %0.2f", tSysSETDisp.fMinValue);

	sprintf(str, tr("����%.2f"), tSysSETDisp.fMaxValue);
	m_StaticUpLimit.SetCaption(str);
	sprintf(str, "Up %.2f", tSysSETDisp.fMaxValue);

	//��ֵ��λ
	m_StaticAsUnit.SetCaption(tSysSETDisp.strUnit);

	//������
	LONG symoble = 0; 
	//�ж϶�ֵ���ͣ� ��Ӧ��edit���뻹��comboѡ��
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
		//��ֵ�Ƿ��и�ֵ�� ����ָ����Ļ�Ҫ��ʾ������
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
		
		//����ͳ�Ƴ�����ȷ��edit�����λ��
		sprintf(str, strFormat, tmpf);
		LONG len = strlen(str) + symoble;
		
		//������������
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

//����ϵ��
void CLcdWndCF::OnWndEnter()
{
	if(!CHmiApp::m_bCFListModified)
		return;
	YES_NO_PTR pfn = (YES_NO_PTR)ChgCF;
	HMIMsgBox(tr("�޸�ϵ��"),  tr("ȷ��Ҫ�޸�ϵ��?"),  this, this, HMI_MB_OKCANCEL, pfn);
}
void CLcdWndOutputTest::OnEnterEditOutputNum()
{

}

void CLcdWndSetting::OnEnterBtnASModify()
{
	//��ȡ������ֵ��������
	LONG rc = DB_GetSystemSETFloatValue(SET_VALUE_ALL, CLcdApp::m_fpSysSetList);	
	CLcdApp::m_WndASModify.m_wSysSetIndex = 0;
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("��ֵ"),tr("�����ö�ֵʧ��!"), this,this);
	}
	else
	{
		//���봰��
		HMIPasswordBox(tr("��������"), tr("��������"),
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


//��ȡBIPara��������
void CLcdWndInputSetting::OnEnterBtnSoeProperty()
{
	
	//��ȡBIPara��������
	if(!HMIReadBIControlToBuf())
	{ 
		//��ȡʧ�ܣ����Զ���ת����m_BtnSoeProperty��ť��EnterWnd
		m_BtnSoeProperty.DisableAutoEnterWnd();
		//ʧ����ʾ
		HMIMsgBox(tr("SOE����"), tr("��ȡSOE����ʧ��!"), this, this);
	}
	else
	{
		m_BtnSoeProperty.EnableAutoEnterWnd();
	}

}

void CLcdWndInputSetting::OnEnterBtnBIFilter()
{
	//��ʼ��DiFilterTime�Ļ�����
	if(!HMIReadDIFilterTimeToBuf())
	{ 
		//��ȡʧ�ܣ����Զ���ת���İ�ť��EnterWnd
		m_BtnBIFilter.DisableAutoEnterWnd();
		//ʧ����ʾ
		HMIMsgBox(tr("����ȥ��ʱ��"), tr("��ȡ����ȥ��ʱ��ʧ��!"), this, this);
	}
	else
	{
		m_BtnBIFilter.EnableAutoEnterWnd();
	}
}
void CLcdWndInputSetting::OnEnterBtnBiNo()
{
	//��ʼ��BI�Ƿ�ȡ��������
	if(!HMIReadBIControlToBuf())
	{ 
		//��ȡʧ�ܣ����Զ���ת���İ�ť��EnterWnd
		m_BtnBiNo.DisableAutoEnterWnd();
		//ʧ����ʾ
		HMIMsgBox(tr("��������"), tr("��ȡ��������ʧ��!"), this, this);
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
	HMIPasswordBox(tr("����������"), tr("����������:"), &(CLcdApp::m_WndTest), this,1);
	m_Btntest.SetEnterWnd(NULL);
}


void CLcdWndAutoJustStep1::OnWndEnter()
{
	//��������У����Ϣ
	OS_PostMessage(dwAITaskID,SM_AUTOADJUST, RtosGetCurrentTaskId(),0,0, 0);
	
	//��ʾ��ʾ����
	HMIMsgBox(tr("У��ϵ��"), tr("����У������ϵ��,\n��ȴ�..."));

}


BOOL CLcdWndMainMenu::OnWndInit()
{
	//��������0�Ļ������ð�ť�ı��⣬ ���Ҳ���ʾλͼ
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
		m_BtnMeasure.SetCaption(tr("�����鿴"));
		m_BtnMeasure.SetHaveEnterFun(FALSE);
		m_BtnMeasure.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndMeasure));
		m_BtnInput.SetPosition(27,94,207,119,0);
		m_BtnInput.SetTabOrder(3);
		m_BtnInput.SetFont(16);
		m_BtnInput.SetOwner(this);
		m_BtnInput.SetCaption(tr("����鿴"));
		m_BtnInput.SetHaveEnterFun(TRUE);
		m_BtnInput.SetEnterWnd(NULL);
		m_BtnSetting.SetPosition(27,64,207,89,0);
		m_BtnSetting.SetTabOrder(2);
		m_BtnSetting.SetFont(0);
		m_BtnSetting.SetOwner(this);
		m_BtnSetting.SetCaption(tr("������ֵ"));
		m_BtnSetting.SetHaveEnterFun(FALSE);
		m_BtnSetting.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
		m_BtnSoftSwitch.SetPosition(27,124,207,149,0);
		m_BtnSoftSwitch.SetTabOrder(4);
		m_BtnSoftSwitch.SetFont(0);
		m_BtnSoftSwitch.SetOwner(this);
		m_BtnSoftSwitch.SetCaption(tr("����ѹ��"));
		m_BtnSoftSwitch.SetHaveEnterFun(TRUE);
		m_BtnSoftSwitch.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSoftSwitch));
		m_BtnReport.SetPosition(27,34,207,59,1);
		m_BtnReport.SetTabOrder(5);
		m_BtnReport.SetFont(16);
		m_BtnReport.SetOwner(this);
		m_BtnReport.SetCaption(tr("����鿴"));
		m_BtnReport.SetHaveEnterFun(FALSE);
		m_BtnReport.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
		m_Btntest.SetPosition(27,94,207,119,1);
		m_Btntest.SetTabOrder(7);
		m_Btntest.SetFont(16);
		m_Btntest.SetOwner(this);
		m_Btntest.SetCaption(tr("���ܲ���"));
		m_Btntest.SetHaveEnterFun(TRUE);
		m_Btntest.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
		m_BtnSystem.SetPosition(27,64,207,89,1);
		m_BtnSystem.SetTabOrder(6);
		m_BtnSystem.SetFont(16);
		m_BtnSystem.SetOwner(this);
		m_BtnSystem.SetCaption(tr("ϵͳ����"));
		m_BtnSystem.SetHaveEnterFun(TRUE);
		m_BtnSystem.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
		m_BtnStatus.SetPosition(27,124,207,149,1);
		m_BtnStatus.SetTabOrder(9);
		m_BtnStatus.SetFont(16);
		m_BtnStatus.SetOwner(this);
		m_BtnStatus.SetCaption(tr("ϵͳ״̬"));
		m_BtnStatus.SetHaveEnterFun(FALSE);
		m_BtnStatus.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
		m_BtnAbout.SetPosition(27,34,207,59,2);
		m_BtnAbout.SetTabOrder(9);
		m_BtnAbout.SetFont(16);
		m_BtnAbout.SetOwner(this);
		m_BtnAbout.SetCaption(tr("��    ��"));
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
		//��ȡʧ�ܣ���ʾ����
		HMIMsgBox(tr("����"),tr("��ȡң�ر���ʱ�����!"),this,this);
		//���Զ������İ�ť��Enterwnd
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

	sprintf(strCaption, "%s", tr("�޸�ң�ر���ʱ��"));
	sprintf(strMsg, "%s", tr("ȷ��Ҫ�����޸ĵ�����?"));


	YES_NO_PTR pfn = (YES_NO_PTR)SaveBOLastTime;
	HMIMsgBox(strCaption,  strMsg,  this, this, HMI_MB_OKCANCEL, pfn);
}
void CLcdWndBOTime::SaveBOLastTime()
{
	if(!HMISaveBOTimeToDB())
	{
		HMIMsgBox(CHmiObj::tr("ң�س���ʱ��","CLcdWndBOTime"),  CHmiObj::tr("����ң�س���ʱ��ʧ��!","CLcdWndBOTime"),
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
	//�ر���ѹ�崰�ڣ�ȷ��ˢ��
	CLcdApp::m_WndSoftSwitch.Close();
	//��ȡ��ѹ�����ݵ���������������ɹ�����ʾ��ȡʧ��
	if(!HMIReadSWToBuf())
		HMIMsgBox(tr("��ѹ��"), tr("����ѹ������ʧ��!"), this, this);
	else
	{
		CLcdApp::m_WndSoftSwitch.Show();
	}
}

void CLcdWndMainMenu::OnEnterBtnSystem()
{
	m_BtnSystem.SetEnterWnd(NULL);
	HMIPasswordBox(tr("����������"), tr("����������:"), &(CLcdApp::m_WndSysSetting), this,1);
}

void CLcdWndReport::OnEnterBtnAlarmReport()
{
	CLcdApp::m_WndAlarmReport.Close();
}

void CLcdWndReport::OnEnterBtnSoe()
{
	//��ֹ��ǰû�йر�
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
		sprintf(Msg, tr("��ǰ���ϵ�����ʾһ��ֵ\n\nҪ��Ϊ����ֵ��?"));
		HMIMsgBox(tr("���ϵ�����ʾ��ʽ"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("��ǰ���ϵ�����ʾ����ֵ\n\nҪ��Ϊһ��ֵ��?"));
		HMIMsgBox(tr("���ϵ�����ʾ��ʽ"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
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
//		HMIMsgBox(tr("���ֵ��"), tr("���û��ֵ�ȴ���!"), this, this);
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
		HMIMsgBox(tr("���峣��"),  tr("ȷ��Ҫ�޸����峣��?\n�޸ĺ��븴λװ��"),  this, this, HMI_MB_OKCANCEL, pfn);
	}
}
void CLcdWndPulseConst::SavePulseConst()
{
	CHmiWnd* pWnd = (CHmiWnd*)&CLcdApp::m_WndPulseConst;
	
	//���溯�����ֲ��ֱ�����ȷ�����ֱ������Ҳ���ش���
	//���Բ��ܱ��溯�����سɹ��񶼷�����Ϣ��
	if(!HMISaveCIPluseToDB())
		HMIMsgBox(pWnd->tr("���峣��"), pWnd->tr("�������峣��ʧ��!"), pWnd, pWnd, HMI_MB_OKCANCEL);
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
	sprintf(Msg1, tr("ȷ��Ҫ�޸ĵ�ȵ���?"));
	YES_NO_PTR pfn = (YES_NO_PTR)SaveCiBase;
	HMIMsgBox(tr("��ȵ���"),  Msg1,  this, this, HMI_MB_OKCANCEL, (YES_NO_PTR)pfn);
	
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
			HMIMsgBox(pWnd->tr("��ȵ���"), pWnd->tr("�����ȵ���ʧ��!"), pWnd, pWnd);
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
	//���봰��
	HMIPasswordBox(tr("��������"),  tr("��������"),&(g_DefaultPara), this,1);
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
	g_wEth = 0x0a; //A��
	g_ModifyIP.DoModal(this);
}

void CLcdWndEthadd::OnEnterBtnethb()
{
	g_wEth = 0x0b; //B��
	g_ModifyIP.DoModal(this);
}

void CLcdWndMainMenu::OnEnterBtnAbout()
{
	g_Version.DoModal(this);

}
void CLcdWndTest::OnEnterBtnFactoryData()
{
	m_BtnFactoryData.SetEnterWnd(NULL);
	HMIPasswordBox(tr("����������"), tr("����������:"), &(CLcdApp::m_WndfactorySet), this,3);

}

void CLcdWndfactorySet::OnEnterBtnManualCoef()
{
	LONG rc=DB_GetCFValue(CF_VALUE_ALL, CHmiApp::m_npCFList);
	if(rc!=DB_ERR_OK)
	{
		HMIMsgBox(tr("ϵ��"),tr("��ϵ��ʧ��!"), this);
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
		sprintf(Msg, tr("ǰ��·���������Ϊ���\n\nҪ��Ϊң����?\n���ĺ��븴λװ��"));
		HMIMsgBox(tr("��������"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("ǰ��·���������Ϊң��\n\nҪ��Ϊ�����?\n���ĺ��븴λװ��"));
		HMIMsgBox(tr("��������"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
}

void CLcdWndCIPara::OnEnterBtnIntegralKW()
{
	char Msg[64];
	BOOL IsCaluIntKW = DB_IsCaculateIntegral();
	YES_NO_PTR pfn = (YES_NO_PTR)ModifyIntKWCaluMode;
	if(IsCaluIntKW == TRUE)
	{
		sprintf(Msg, tr("��ǰװ�ü�����ֵ��\n\nҪȡ��������ֵ����?"));
		HMIMsgBox(tr("���ֵ��"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("��ǰװ�ò�������ֵ��\n\nҪ������ֵ����?"));
		HMIMsgBox(tr("���ֵ��"),Msg,&(CLcdApp::m_WndCIPara), this, HMI_MB_OKCANCEL,pfn);
	}
}

void CLcdWndCIPara::OnEnterBtnCIbase()
{
	BOOL IsCaluIntKW = DB_IsCaculateIntegral();;
	WORD wPulseKWNum = DB_GetPulseCINum();

	if((wPulseKWNum==0)&&(IsCaluIntKW == FALSE))
	{
		HMIMsgBox(tr("��ȵ���"),tr("��ǰ��װ��û��������������\n\n�Ҳ�������ֵ��!"),this, this, HMI_MB_OK);
		m_BtnCIbase.DisableAutoEnterWnd();
		return;
	}
	//��ʼ�����峣��������
	if(!HMIReadCIBaseToBuf())
	{ 
		//��ȡʧ�ܣ����Զ���ת���İ�ť��EnterWnd
		m_BtnCIbase.DisableAutoEnterWnd();
		//ʧ����ʾ
		HMIMsgBox(tr("��ȵ���"), tr("��ȡ��ȵ���ʧ��!"), this, this);
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
		HMIMsgBox(tr("���峣��"),tr("��ǰ��װ��û������������!"),this, this, HMI_MB_OK);
		m_BtnPulseConst.DisableAutoEnterWnd();
		return;
	}
	
	//��ʼ�����峣��������
	if(!HMIReadCIPluseToBuf())
	{ 
		//��ȡʧ�ܣ����Զ���ת���İ�ť��EnterWnd
		m_BtnPulseConst.DisableAutoEnterWnd();
		//ʧ����ʾ
		HMIMsgBox(tr("���峣��"), tr("��ȡ���峣��ʧ��!"), this, this);
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
			HMIMsgBox(pWnd->tr("��������"),pWnd->tr("�޸Ŀ������Դ���!"),pWnd, pWnd, HMI_MB_OK);
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
			HMIMsgBox(pWnd->tr("��������"),pWnd->tr("�޸Ŀ������Դ���!"),pWnd, pWnd, HMI_MB_OK);
			return;
		}
		rc = DB_WriteDIParaConfirm(i);		
		if(DB_ERR_OK !=rc)
		{		
			HMIMsgBox(pWnd->tr("��������"),pWnd->tr("�޸Ŀ������Դ���!"),pWnd, pWnd, HMI_MB_OK);
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
		//��ȡʧ�ܣ����Զ���ת���İ�ť��EnterWnd
		//m_BtnPortStatus.DisableAutoEnterWnd();
		//ʧ����ʾ
		HMIMsgBox(tr("�˿ڲ���"), tr("��ȡ�˿ڲ���ʧ��!"), this, this);
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
		sprintf(Msg, tr("��ǰʱ��Դ��ʽΪDK3511\n\nҪ����ΪKF6511��ʽ��?"));
		HMIMsgBox(tr("ʱ��Դ��ʽ"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
	}
	else
	{
		sprintf(Msg, tr("��ǰʱ��Դ��ʽΪKF6511\n\nҪ����ΪDK3511��ʽ��?"));
		HMIMsgBox(tr("ʱ��Դ��ʽ"),Msg,&(CLcdApp::m_Wndsyspara), this, HMI_MB_OKCANCEL,pfn);
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

