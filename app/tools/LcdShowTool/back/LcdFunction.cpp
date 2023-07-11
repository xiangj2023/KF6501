/*************************************************************************
*
*   �ļ����ƣ�LcdApp.cpp
*   ��    �ߣ��������
*   ��    �ܣ�cpp�ļ�
*
*************************************************************************/
//add include .h files below
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			
#include "Db_err.h"
#include "lcdapp.h"

//@//@//@//@//@//@//@//End of File head

//Begin of void CLcdWndMeasure::OnEnterBtnHMeasure()
void CLcdWndMeasure::OnEnterBtnHMeasure()
{

}
//End of void CLcdWndMeasure::OnEnterBtnHMeasure()
//Begin of void CLcdWndMeasure::OnEnterBtnLMeasure()
void CLcdWndMeasure::OnEnterBtnLMeasure()
{

}
//End of void CLcdWndMeasure::OnEnterBtnLMeasure()
//Begin of void CLcdWndMeasure::OnEnterBtnPMH()
void CLcdWndMeasure::OnEnterBtnPMH()
{

}
//End of void CLcdWndMeasure::OnEnterBtnPMH()
//Begin of void CLcdWndMeasure::OnEnterBtnPML()
void CLcdWndMeasure::OnEnterBtnPML()
{

}
//End of void CLcdWndMeasure::OnEnterBtnPML()
//Begin of void CLcdWndMeasure::OnEnterBtnRealDD()
void CLcdWndMeasure::OnEnterBtnRealDD()
{

}
//End of void CLcdWndMeasure::OnEnterBtnRealDD()
//Begin of void CLcdWndDD::OnEnterListrealDD()
void CLcdWndDD::OnEnterListrealDD()
{

}
//End of void CLcdWndDD::OnEnterListrealDD()
//Begin of void CLcdWndDD::OnAddSubListrealDD(LONG AddSub)
void CLcdWndDD::OnAddSubListrealDD(LONG AddSub)
{

}
//End of void CLcdWndDD::OnAddSubListrealDD(LONG AddSub)
//Begin of void CLcdWndInput::OnWndLeftRight(LONG LeftRight)
void CLcdWndInput::OnWndLeftRight(LONG LeftRight)
{

}
//End of void CLcdWndInput::OnWndLeftRight(LONG LeftRight)
//Begin of void CLcdWndActionReport::OnWndInit()
void CLcdWndActionReport::OnWndInit()
{

}
//End of void CLcdWndActionReport::OnWndInit()
//Begin of void CLcdWndActionReport::OnEnterList4640()
void CLcdWndActionReport::OnEnterList4640()
{

}
//End of void CLcdWndActionReport::OnEnterList4640()
//Begin of void CLcdWndActionReport::OnAddSubList4640(LONG AddSub)
void CLcdWndActionReport::OnAddSubList4640(LONG AddSub)
{

}
//End of void CLcdWndActionReport::OnAddSubList4640(LONG AddSub)
//Begin of void CLcdWndActionReportDetail::OnWndInit()
void CLcdWndActionReportDetail::OnWndInit()
{

}
//End of void CLcdWndActionReportDetail::OnWndInit()
//Begin of void CLcdWndAlarmReport::OnEnterList4662()
void CLcdWndAlarmReport::OnEnterList4662()
{

}
//End of void CLcdWndAlarmReport::OnEnterList4662()
//Begin of void CLcdWndSetting::OnEnterBtnSettingCopy()
void CLcdWndSetting::OnEnterBtnSettingCopy()
{

}
//End of void CLcdWndSetting::OnEnterBtnSettingCopy()
//Begin of void CLcdWndconfigQury::OnWndEnter()
void CLcdWndconfigQury::OnWndEnter()
{
	m_pEnterWnd->SetParam1(GetParam1());

}
//End of void CLcdWndconfigQury::OnWndEnter()
//Begin of void CLcdWndconfigModify::OnAddSubList4692(LONG AddSub)
void CLcdWndconfigModify::OnAddSubList4692(LONG AddSub)
{

}
//End of void CLcdWndconfigModify::OnAddSubList4692(LONG AddSub)
//Begin of void CLcdWndSettingModify::OnWndUpDown(LONG UpDown)
void CLcdWndSettingModify::OnWndUpDown(LONG UpDown)
{

	LONG Num = DB_GetSETNum();

	if(UpDown>0)
	{
		m_nInputParam2++;
		m_nInputParam2 %= Num;
	}
	else
	{
		m_nInputParam2 += Num;
		m_nInputParam2--;
		m_nInputParam2 %= Num;
	}
	

	OnWndInit();
	OnPaint();

}
//End of void CLcdWndSettingModify::OnWndUpDown(LONG UpDown)
//Begin of void CLcdWndSettingModify::OnWndEnter()
void CLcdWndSettingModify::OnWndEnter()
{
	//�Ƚ϶�ֵ�������Ƿ��в���ͬ�� ���������ʾҪ�̻�
	WORD r1 = memcmp(CHmiApp::m_byCurCfg,CHmiApp::m_byNewCfg, 256);
	WORD r2 =  memcmp(CHmiApp::m_fCurSetting, CHmiApp::m_fNewSetting, sizeof(float)*256);

	if(r1 != 0  || r2 != 0)
	{
		CLcdApp::m_WndSettingSave.SetParam1(GetParam1()); //��ֵ���Ŵ���ȥ

		SetEnterWnd(&CLcdApp::m_WndSettingSave);
	}
	else
		SetEnterWnd(&CLcdApp::m_WndSetting);

}
//End of void CLcdWndSettingModify::OnWndEnter()
//Begin of void CLcdWndSettingModify::OnWndAddSub(LONG AddSub)
void CLcdWndSettingModify::OnWndAddSub(LONG AddSub)
{
	//��Ҫ���û�����Ķ�ֵ���л��棬Ŀǰ���������, û�б����û�����
	float fValue = m_EditSettingData.GetValue();
	CLcdApp::m_fNewSetting[m_nInputParam2] = fValue;


}
//End of void CLcdWndSettingModify::OnWndAddSub(LONG AddSub)
//Begin of void CLcdWndSettingModify::OnWndInit()
void CLcdWndSettingModify::OnWndInit()
{
	//���λ��


	if(m_nInputParam1>16 || m_nInputParam1<0)
		m_nInputParam1 = 0;
	if(m_nInputParam2<0 || m_nInputParam2 >DB_GetSETNum())
		m_nInputParam2 = 0;


	float	  fSETValue[20];
	TSET_Disp	tSETDisp;
	char str[64];
	char strFormat[32];

	DB_GetSETDisp(m_nInputParam2, &tSETDisp);
	
	int nSetNum = DB_GetSETNum();
	//�������˳��Ķ�ֵ����ʾ
	int nLoop = nSetNum;

	while((tSETDisp.nCFGIndex  != 0xFF)&&(m_nInputParam2 < nSetNum))
	{
		DB_GetSETDisp(m_nInputParam2, &tSETDisp);

		//�ж��Ƿ񳬱�
		if(tSETDisp.nCFGIndex > DB_GetCFGNum())
		{
			LogError("CLcdWndSettingModify::OnWndInit",FILE_LINE,
				"The tSETDisp.nCFGIndex=%d is overflow!",DB_GetCFGNum());
			break;

		}

		if(CHmiApp::m_byNewCfg[tSETDisp.nCFGIndex] == CFG_STATUS_SET)
			break;
		m_nInputParam2++;
		nLoop --;
	}

	//�ӵ�һ�������һ����û������Ͷ���
	if(nLoop == 0)
	{
		LogError("CLcdWndSettingModify::OnWndInit",FILE_LINE,
			"no Cfg be Setted");
	//��ʾû��һ������Ͷ��
	}

	//�޸ı���
	sprintf(str, "�޸Ķ�ֵ: %02d��", m_nInputParam1);
	SetCaption(str);
	DrawCaption();


//	DB_GetSETFloatValue(m_nInputParam1, SET_VALUE_ALL, fSETValue);	
	

	//��ֵӢ����
	sprintf(str, "%s =", tSETDisp.strSymbol);
	m_StaticSymbol.SetCaption(str);
	
	//��ֵ����
	m_StaticName.SetCaption(tSETDisp.strName);

	sprintf(str, "����%.2f", tSETDisp.fMinValue);
	m_StaticUpLimit.SetCaption(str);

	sprintf(str, "����%.2f", tSETDisp.fMaxValue);
	m_StaticDownLimit.SetCaption(str);
	
	//��ֵ��λ
	m_StaticUnit.SetCaption(tSETDisp.strUnit);

	LONG symoble = 0;
	
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

	m_EditSettingData.SetValue(CLcdApp::m_fNewSetting[GetParam2()]);
	m_EditSettingData.SetCursorPos(0);
	m_EditSettingData.UpdateData(FALSE);

}
//End of void CLcdWndSettingModify::OnWndInit()
//Begin of void CLcdWndSettingSwitch::OnWndEnter()
void CLcdWndSettingSwitch::OnWndEnter()
{
	LONG nAreaNo = m_EditAreaNum.GetValue();
	LONG rc;
	m_pEnterWnd->SetParam1(nAreaNo);

	rc = DB_SetCurrentSETAreaNo(nAreaNo);

}
//End of void CLcdWndSettingSwitch::OnWndEnter()
//Begin of void CLcdWndSettingSwitch::OnWndInit()
void CLcdWndSettingSwitch::OnWndInit()
{
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);

}
//End of void CLcdWndSettingSwitch::OnWndInit()
//Begin of void CLcdWndInputParam::OnAddSubList4750(LONG AddSub)
void CLcdWndInputParam::OnAddSubList4750(LONG AddSub)
{

}
//End of void CLcdWndInputParam::OnAddSubList4750(LONG AddSub)
//Begin of void CLcdWndSettingSaveConfirm::OnEnterBtnOk()
void CLcdWndSettingSaveConfirm::OnEnterBtnOk()
{

}
//End of void CLcdWndSettingSaveConfirm::OnEnterBtnOk()
//Begin of void CLcdWndSetting::OnEnterBtnSettingQuery()
void CLcdWndSetting::OnEnterBtnSettingQuery()
{
	m_BtnSettingQuery.GetEnterWnd()->SetParam1(GetParam1());

}
//End of void CLcdWndSetting::OnEnterBtnSettingQuery()
//Begin of void CLcdWndSetting::OnEnterBtnsettingModify()
void CLcdWndSetting::OnEnterBtnsettingModify()
{
	m_BtnsettingModify.GetEnterWnd()->SetParam1(GetParam1());
}
//End of void CLcdWndSetting::OnEnterBtnsettingModify()
//Begin of void CLcdWndSetting::OnEnterBtnsettingExChg()
void CLcdWndSetting::OnEnterBtnsettingExChg()
{

}
//End of void CLcdWndSetting::OnEnterBtnsettingExChg()
//Begin of void CLcdWndReport::OnEnterBtnActionReport()
void CLcdWndReport::OnEnterBtnActionReport()
{

}
//End of void CLcdWndReport::OnEnterBtnActionReport()
//Begin of void CLcdWndReport::OnEnterBtnOperation()
void CLcdWndReport::OnEnterBtnOperation()
{

}
//End of void CLcdWndReport::OnEnterBtnOperation()
//Begin of void CLcdWndSettingQueryAreaNumInput::OnWndEnter()
void CLcdWndSettingQueryAreaNumInput::OnWndEnter()
{
	LONG AreaNo;
	m_EditAreaNum.UpdateData();
	AreaNo = m_EditAreaNum.GetValue();

	m_pEnterWnd->SetParam1(AreaNo);

}
//End of void CLcdWndSettingQueryAreaNumInput::OnWndEnter()
//Begin of void CLcdWndSettingQueryAreaNumInput::OnWndInit()
void CLcdWndSettingQueryAreaNumInput::OnWndInit()
{
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);

	m_EditAreaNum.SetValue(nAreaNo);
	UpdateData(FALSE);

}
//End of void CLcdWndSettingQueryAreaNumInput::OnWndInit()
//Begin of void CLcdWndSettingCopy::OnWndEnter()
void CLcdWndSettingCopy::OnWndEnter()
{
	WORD wSourArea, wStartArea, wEndArea;

	wSourArea = m_EditSoureAreaNum.GetValue();
	wStartArea = m_EditDstStartAreaNum.GetValue();
	wEndArea = m_EditDstEndAreaNum.GetValue();

	GetEnterWnd()->SetParam1(wSourArea);
	GetEnterWnd()->SetParam2(wStartArea);
	GetEnterWnd()->SetParam3(wEndArea);

}
//End of void CLcdWndSettingCopy::OnWndEnter()
//Begin of void CLcdWndSettingCopy::OnWndInit()
void CLcdWndSettingCopy::OnWndInit()
{

}
//End of void CLcdWndSettingCopy::OnWndInit()
//Begin of void CLcdWndSoftSwitch::OnWndEnter()
void CLcdWndSoftSwitch::OnWndEnter()
{
	LONG r = memcmp(CHmiApp::m_byCurSoftSwitch, CHmiApp::m_byNewSoftSwitch, 256);
	if(r != 0)
	{
		SetEnterWnd(&CLcdApp::m_WndsoftswitchConfirm);
		
	}
	else
	{
		SetEnterWnd(&CLcdApp::m_WndMainMenu);
	}


}
//End of void CLcdWndSoftSwitch::OnWndEnter()
//Begin of void CLcdWndtime::OnWndEnter()
void CLcdWndtime::OnWndEnter()
{

}
//End of void CLcdWndtime::OnWndEnter()
//Begin of void CLcdWndtime::OnWndInit()
void CLcdWndtime::OnWndInit()
{

}
//End of void CLcdWndtime::OnWndInit()
//Begin of void CLcdWndInputParam::OnWndEnter()
void CLcdWndInputParam::OnWndEnter()
{

}
//End of void CLcdWndInputParam::OnWndEnter()
//Begin of void CLcdWndwnd26::OnWndEnter()
void CLcdWndwnd26::OnWndEnter()
{

}
//End of void CLcdWndwnd26::OnWndEnter()
//Begin of void CLcdWndwnd31::OnWndEnter()
void CLcdWndwnd31::OnWndEnter()
{

}
//End of void CLcdWndwnd31::OnWndEnter()
//Begin of void CLcdWndSwitchParam::OnWndEnter()
void CLcdWndSwitchParam::OnWndEnter()
{

}
//End of void CLcdWndSwitchParam::OnWndEnter()
//Begin of void CLcdWndSwitchParam::OnWndInit()
void CLcdWndSwitchParam::OnWndInit()
{

}
//End of void CLcdWndSwitchParam::OnWndInit()
//Begin of void CLcdWndSoeProperty::OnWndEnter()
void CLcdWndSoeProperty::OnWndEnter()
{

}
//End of void CLcdWndSoeProperty::OnWndEnter()
//Begin of void CLcdWndCiBase::OnWndEnter()
void CLcdWndCiBase::OnWndEnter()
{

}
//End of void CLcdWndCiBase::OnWndEnter()
//Begin of void CLcdWndPluseConst::OnWndEnter()
void CLcdWndPluseConst::OnWndEnter()
{

}
//End of void CLcdWndPluseConst::OnWndEnter()
//Begin of void CLcdWndIfTest::OnEnterBtnOk()
void CLcdWndIfTest::OnEnterBtnOk()
{

}
//End of void CLcdWndIfTest::OnEnterBtnOk()
//Begin of void CLcdWndIfTest::OnEnterBtnCancel()
void CLcdWndIfTest::OnEnterBtnCancel()
{

}
//End of void CLcdWndIfTest::OnEnterBtnCancel()
//Begin of void CLcdWndTest::OnEnterBtnOutputTest()
void CLcdWndTest::OnEnterBtnOutputTest()
{

}
//End of void CLcdWndTest::OnEnterBtnOutputTest()
//Begin of void CLcdWndTest::OnEnterBtnLenTest()
void CLcdWndTest::OnEnterBtnLenTest()
{

}
//End of void CLcdWndTest::OnEnterBtnLenTest()
//Begin of void CLcdWndTest::OnEnterBtnReportTest()
void CLcdWndTest::OnEnterBtnReportTest()
{

}
//End of void CLcdWndTest::OnEnterBtnReportTest()
//Begin of void CLcdWndTest::OnEnterBtnChannelFactor()
void CLcdWndTest::OnEnterBtnChannelFactor()
{

}
//End of void CLcdWndTest::OnEnterBtnChannelFactor()
//Begin of void CLcdWndTest::OnEnterBtnAllSelfTest()
void CLcdWndTest::OnEnterBtnAllSelfTest()
{

}
//End of void CLcdWndTest::OnEnterBtnAllSelfTest()
//Begin of void CLcdWndOutputTest::OnWndEnter()
void CLcdWndOutputTest::OnWndEnter()
{

}
//End of void CLcdWndOutputTest::OnWndEnter()
//Begin of void CLcdWndLanTest::OnWndEnter()
void CLcdWndLanTest::OnWndEnter()
{

}
//End of void CLcdWndLanTest::OnWndEnter()
//Begin of void CLcdWndLanTest::OnWndInit()
void CLcdWndLanTest::OnWndInit()
{

}
//End of void CLcdWndLanTest::OnWndInit()
//Begin of void CLcdWndReportTest::OnWndEnter()
void CLcdWndReportTest::OnWndEnter()
{

}
//End of void CLcdWndReportTest::OnWndEnter()
//Begin of void CLcdWndAddressSetting::OnWndEnter()
void CLcdWndAddressSetting::OnWndEnter()
{

}
//End of void CLcdWndAddressSetting::OnWndEnter()
//Begin of void CLcdWndAddressSetting::OnWndInit()
void CLcdWndAddressSetting::OnWndInit()
{

}
//End of void CLcdWndAddressSetting::OnWndInit()
//Begin of void CLcdWndIfSettingCopy::OnEnterBtnOk()
void CLcdWndIfSettingCopy::OnEnterBtnOk()
{
	//����ֵ
	float fvalue[256];
	DB_GetSETFloatValue(GetParam1(), SET_VALUE_ALL, fvalue);
	WORD AreaSource, AreaStart, AreaEnd;
	AreaStart = GetParam2();
	AreaEnd = GetParam3();
	AreaSource = GetParam1();

	WORD i, j;
	for(i = AreaStart; i<= AreaEnd; i++ )
	{
		DB_SetSETValue(i, SET_VALUE_ALL, fvalue);
		DB_SetSETValueConfirm(i, SET_VALUE_ALL, DB_EXECUTE);
	}

	WORD cfgNum = DB_GetCFGNum();
	LONG cfgWordNum = cfgNum/32;

	if(cfgNum%32)
	{
		cfgWordNum += 1;
	}

	DWORD dwCfgWordValue;
	for(i =0; i<cfgWordNum; i++)
	{
		DB_GetCFGWord(AreaSource, i, &dwCfgWordValue);
		
		for(j= AreaStart; j<= AreaEnd;  j++)
		{
			DB_SetCFGWord(j, i, dwCfgWordValue);
			DB_SetCFGStatusConfirm(j, i, DB_EXECUTE);
		}
	}

}
//End of void CLcdWndIfSettingCopy::OnEnterBtnOk()
//Begin of void CLcdWndIfSettingCopy::OnEnterBtnCancel()
void CLcdWndIfSettingCopy::OnEnterBtnCancel()
{

}
//End of void CLcdWndIfSettingCopy::OnEnterBtnCancel()
//Begin of void CLcdWndIfPassChg::OnEnterBtnOk()
void CLcdWndIfPassChg::OnEnterBtnOk()
{

}
//End of void CLcdWndIfPassChg::OnEnterBtnOk()
//Begin of void CLcdWndIfPassChg::OnEnterBtnCancel()
void CLcdWndIfPassChg::OnEnterBtnCancel()
{

}
//End of void CLcdWndIfPassChg::OnEnterBtnCancel()
//Begin of void CLcdWndPassChgInput::OnWndEnter()
void CLcdWndPassChgInput::OnWndEnter()
{

}
//End of void CLcdWndPassChgInput::OnWndEnter()
//Begin of void CLcdWndPassInput::OnWndEnter()
void CLcdWndPassInput::OnWndEnter()
{
	//�ж����벻��ȷ����ת
	LONG Pass = m_EditPass.GetValue();
	if(Pass == 0)
		SetEnterWnd(&CLcdApp::m_WndSettingChgAreaNumInput);
	else
		SetEnterWnd(NULL);

}
//End of void CLcdWndPassInput::OnWndEnter()
//Begin of void CLcdWndSettingChgAreaNumInput::OnWndInit()
void CLcdWndSettingChgAreaNumInput::OnWndInit()
{
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNum.SetCaption(szAreaNo);

}
//End of void CLcdWndSettingChgAreaNumInput::OnWndInit()
//Begin of void CLcdWndsyspara::OnEnterBtnLang1()
void CLcdWndsyspara::OnEnterBtnLang1()
{
	s_nLangIndex = 0;

}
//End of void CLcdWndsyspara::OnEnterBtnLang1()
//Begin of void CLcdWndsyspara::OnEnterBtnLang2()
void CLcdWndsyspara::OnEnterBtnLang2()
{
	s_nLangIndex = 1;

}
//End of void CLcdWndsyspara::OnEnterBtnLang2()
//Begin of void CLcdWndAllSelfTest::OnWndEnter()
void CLcdWndAllSelfTest::OnWndEnter()
{

}
//End of void CLcdWndAllSelfTest::OnWndEnter()
//Begin of void CLcdWndSettingQuery::OnWndInit()
void CLcdWndSettingQuery::OnWndInit()
{

}
//End of void CLcdWndSettingQuery::OnWndInit()
//Begin of void CLcdWndSettingSwitchConfirm::OnWndEnter()
void CLcdWndSettingSwitchConfirm::OnWndEnter()
{

}
//End of void CLcdWndSettingSwitchConfirm::OnWndEnter()
//Begin of void CLcdWndSettingSwitchConfirm::OnWndInit()
void CLcdWndSettingSwitchConfirm::OnWndInit()
{
	LONG nAreaNo = DB_GetCurrentSETAreaNo();
	char szAreaNo[8];
	sprintf(szAreaNo, "%d", nAreaNo);
	m_StaticNowAreaNo.SetCaption(szAreaNo);

	sprintf(szAreaNo, "%d", GetParam1());
	m_StaticNewAreaNo.SetCaption(szAreaNo);

}
//End of void CLcdWndSettingSwitchConfirm::OnWndInit()
//Begin of void CLcdWndSettingSwitchConfirm::OnEnterBtnOk()
void CLcdWndSettingSwitchConfirm::OnEnterBtnOk()
{
	LONG rc;
	rc = DB_SetCurrentSETAreaNoConfirm(GetParam1(), DB_EXECUTE);
	//ʹ�ò��ɹ��Ƿ�Ӧ������ʾ��

}
//End of void CLcdWndSettingSwitchConfirm::OnEnterBtnOk()
//Begin of void CLcdWndSettingSwitchConfirm::OnEnterBtnCancel()
void CLcdWndSettingSwitchConfirm::OnEnterBtnCancel()
{

}
//End of void CLcdWndSettingSwitchConfirm::OnEnterBtnCancel()
//Begin of void CLcdWndPassInput::OnWndInit()
void CLcdWndPassInput::OnWndInit()
{

}
//End of void CLcdWndPassInput::OnWndInit()
//Begin of void CLcdWndconfigModify::OnWndEnter()
void CLcdWndconfigModify::OnWndEnter()
{
	m_pEnterWnd->SetParam1(GetParam1());
	m_pEnterWnd->SetParam2(0);

}
//End of void CLcdWndconfigModify::OnWndEnter()
//Begin of void CLcdWndSettingChgAreaNumInput::OnWndEnter()
void CLcdWndSettingChgAreaNumInput::OnWndEnter()
{
	LONG nAreaNo = m_EditInputAreaNum.GetValue();
	m_pEnterWnd->SetParam1(nAreaNo);

	int i;

	//��ȡ��ǰ��ֵ������ʼ��������
	//������
	WORD wCfgNum = DB_GetCFGNum();

	for(i=0; i<wCfgNum; i++)
	{
		DB_GetCFGStatus(nAreaNo,i, CLcdApp::m_byCurCfg[i]);
		DB_GetCFGStatus(nAreaNo,i, CLcdApp::m_byNewCfg[i]);
	}

	//����ֵ���õ�������
	DB_GetSETFloatValue(nAreaNo, SET_VALUE_ALL, CLcdApp::m_fCurSetting);
	DB_GetSETFloatValue(nAreaNo, SET_VALUE_ALL, CLcdApp::m_fNewSetting);



}
//End of void CLcdWndSettingChgAreaNumInput::OnWndEnter()
//Begin of void CLcdWndSettingSave::OnEnterBtnok()
void CLcdWndSettingSave::OnEnterBtnok()
{
	WORD wAreaNO = GetParam1();
	//��ֵ
	DB_SetSETValue(wAreaNO,SET_VALUE_ALL, CHmiApp::m_fNewSetting);
	DB_SetSETValueConfirm(wAreaNO,SET_VALUE_ALL, DB_EXECUTE);

	DB_GetSETFloatValue(wAreaNO, SET_VALUE_ALL, CHmiApp::m_fCurSetting);



	//����
	WORD wCfgNum = DB_GetCFGNum();
	 for(int i=0; i< wCfgNum; i++)
	 {
		 if(CHmiApp::m_byCurCfg[i] != CHmiApp::m_byNewCfg[i])
		 {
			 DB_SetCFGStatus(wAreaNO, i, CHmiApp::m_byNewCfg[i]);
			 DB_SetCFGStatusConfirm(wAreaNO, i, DB_EXECUTE);
			 CHmiApp::m_byCurCfg[i] = CHmiApp::m_byNewCfg[i];

		 }
	 }

	 WORD wCurAreaNO = DB_GetSETAreaNum();

	 if(wCurAreaNO != wAreaNO)
	 {
		 m_Btnok.SetEnterWnd(&CLcdApp::m_WndSettingSwitchConfirm);
		 //��ֵ�̻���ȷ���Ƿ�Ҫ�л���ֵ
		 m_Btnok.GetEnterWnd()->SetParam1(wAreaNO);
		 //Ͷ���޸ĵĶ�ֵ���� ��ȷ�Ϻ����Ч
		 DB_SetCurrentSETAreaNo(wAreaNO);
	 }
	 else
	 {
		 m_Btnok.SetEnterWnd(&CLcdApp::m_WndSetting);
	 }

}
//End of void CLcdWndSettingSave::OnEnterBtnok()
//Begin of void CLcdWndPassChange::OnEnterBtnChgSettingPass()
void CLcdWndPassChange::OnEnterBtnChgSettingPass()
{
	m_BtnChgSettingPass.GetEnterWnd()->SetCaption("�����޸�--��ֵ����");

}
//End of void CLcdWndPassChange::OnEnterBtnChgSettingPass()
//Begin of void CLcdWndPassChange::OnEnterBtnChgSwitchPass()
void CLcdWndPassChange::OnEnterBtnChgSwitchPass()
{
	m_BtnChgSwitchPass.GetEnterWnd()->SetCaption("�����޸�--ѹ������");

}
//End of void CLcdWndPassChange::OnEnterBtnChgSwitchPass()
//Begin of void CLcdWndPassChange::OnEnterBtnChgCommonPass()
void CLcdWndPassChange::OnEnterBtnChgCommonPass()
{
	m_BtnChgCommonPass.GetEnterWnd()->SetCaption("�����޸�--һ������");

}
//End of void CLcdWndPassChange::OnEnterBtnChgCommonPass()
//Begin of void CLcdWndPassChgInput::OnWndInit()
void CLcdWndPassChgInput::OnWndInit()
{
	m_EditNewPass.SetValue(0);
	m_EditOldPass.SetValue(0);

	m_EditNewPass.UpdateData(FALSE);
	m_EditOldPass.UpdateData(FALSE);
}
//End of void CLcdWndPassChgInput::OnWndInit()
//Begin of void CLcdWndsoftswitchConfirm::OnEnterBtnOk()
void CLcdWndsoftswitchConfirm::OnEnterBtnOk()
{
	LONG num = DB_GetSWNum();
	WORD i;

	for(i=0; i<num; i++)
	{
		if(CHmiApp::m_byCurSoftSwitch[i] != CHmiApp::m_byNewSoftSwitch[i])
		{
			DB_SetSWStatus(i, CHmiApp::m_byNewSoftSwitch[i]);
			DB_SetSWStatusConfirm(i, DB_EXECUTE);
			CHmiApp::m_byCurSoftSwitch[i] = CHmiApp::m_byNewSoftSwitch[i];
		}
	}
	
	SetEnterWnd(&CLcdApp::m_WndSoftSwitch);

}
//End of void CLcdWndsoftswitchConfirm::OnEnterBtnOk()
//Begin of void CLcdWndSoftSwitch::OnWndInit()
void CLcdWndSoftSwitch::OnWndInit()
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

}
//End of void CLcdWndSoftSwitch::OnWndInit()
