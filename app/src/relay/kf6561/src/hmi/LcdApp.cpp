/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/
/*************************************************************************
* 
*   文件名称：
*			LCDApp.cpp
* 
*   软件模块：液晶应用程序
*
* 
*	描述	实现液晶应用程序中各窗口构造函数，析构函数，init函数
*			该文件由工具软件自动生成，不要手动修改
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		 液晶工具软件	2015-03-27		初始创建
* 
*************************************************************************/

#include "lcdapp.h"
#include "bitmapdata.h"
#ifndef __PL_WIN_SIMULATION__
#include "lcdcfg.h"
#endif
//************窗口MainMenu: KF6561电容器保护测控装置*******************
CLcdWndMainMenu::~CLcdWndMainMenu()
{
}
CLcdWndMainMenu::CLcdWndMainMenu()
{
}
BOOL CLcdWndMainMenu::Init()
{
	SetClassName("CLcdWndMainMenu");
	SetCaption(tr("KF6561电容器保护测控装置"));
	SetEscWnd(NULL);
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(2);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnMeasure.SetBitmap(measure,2,3,85,29,12);
	m_BtnMeasure.SetPosition(15,35,110,70,0);
	m_BtnMeasure.SetTabOrder(1);
	m_pObjList[1] = &m_BtnMeasure;
	m_BtnMeasure.SetFont(16);
	m_BtnMeasure.SetOwner(this);
	m_BtnMeasure.SetCaption(tr(""));
	m_BtnMeasure.SetHaveEnterFun(FALSE);
	m_BtnMeasure.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndMeasure));
	m_BtnSetting.SetBitmap(setting,2,3,85,28,12);
	m_BtnSetting.SetPosition(125,35,220,70,0);
	m_BtnSetting.SetTabOrder(2);
	m_pObjList[2] = &m_BtnSetting;
	m_BtnSetting.SetFont(0);
	m_BtnSetting.SetOwner(this);
	m_BtnSetting.SetCaption(tr(""));
	m_BtnSetting.SetHaveEnterFun(FALSE);
	m_BtnSetting.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	m_BtnInput.SetBitmap(input,2,3,85,28,12);
	m_BtnInput.SetPosition(15,75,110,110,0);
	m_BtnInput.SetTabOrder(3);
	m_pObjList[3] = &m_BtnInput;
	m_BtnInput.SetFont(16);
	m_BtnInput.SetOwner(this);
	m_BtnInput.SetCaption(tr(""));
	m_BtnInput.SetHaveEnterFun(TRUE);
	m_BtnInput.SetEnterWnd(NULL);
	m_BtnSoftSwitch.SetBitmap(softswitch,2,3,85,28,12);
	m_BtnSoftSwitch.SetPosition(125,75,220,110,0);
	m_BtnSoftSwitch.SetTabOrder(4);
	m_pObjList[4] = &m_BtnSoftSwitch;
	m_BtnSoftSwitch.SetFont(0);
	m_BtnSoftSwitch.SetOwner(this);
	m_BtnSoftSwitch.SetCaption(tr(""));
	m_BtnSoftSwitch.SetHaveEnterFun(TRUE);
	m_BtnSoftSwitch.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSoftSwitch));
	m_BtnReport.SetBitmap(report,2,3,85,28,12);
	m_BtnReport.SetPosition(15,117,110,152,0);
	m_BtnReport.SetTabOrder(5);
	m_pObjList[5] = &m_BtnReport;
	m_BtnReport.SetFont(16);
	m_BtnReport.SetOwner(this);
	m_BtnReport.SetCaption(tr(""));
	m_BtnReport.SetHaveEnterFun(FALSE);
	m_BtnReport.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	m_BtnSystem.SetBitmap(set,5,2,85,28,12);
	m_BtnSystem.SetPosition(125,117,220,152,0);
	m_BtnSystem.SetTabOrder(6);
	m_pObjList[6] = &m_BtnSystem;
	m_BtnSystem.SetFont(16);
	m_BtnSystem.SetOwner(this);
	m_BtnSystem.SetCaption(tr(""));
	m_BtnSystem.SetHaveEnterFun(TRUE);
	m_BtnSystem.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	m_Btntest.SetBitmap(test,2,3,85,28,12);
	m_Btntest.SetPosition(15,31,110,66,1);
	m_Btntest.SetTabOrder(7);
	m_pObjList[7] = &m_Btntest;
	m_Btntest.SetFont(16);
	m_Btntest.SetOwner(this);
	m_Btntest.SetCaption(tr(""));
	m_Btntest.SetHaveEnterFun(TRUE);
	m_Btntest.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
	m_BtnStatus.SetBitmap(status,2,3,85,28,12);
	m_BtnStatus.SetPosition(125,31,220,66,1);
	m_BtnStatus.SetTabOrder(8);
	m_pObjList[8] = &m_BtnStatus;
	m_BtnStatus.SetFont(16);
	m_BtnStatus.SetOwner(this);
	m_BtnStatus.SetCaption(tr(""));
	m_BtnStatus.SetHaveEnterFun(FALSE);
	m_BtnStatus.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	m_BtnAbout.SetBitmap(about,2,3,85,28,12);
	m_BtnAbout.SetPosition(15,73,110,108,1);
	m_BtnAbout.SetTabOrder(9);
	m_pObjList[9] = &m_BtnAbout;
	m_BtnAbout.SetFont(16);
	m_BtnAbout.SetOwner(this);
	m_BtnAbout.SetCaption(tr(""));
	m_BtnAbout.SetHaveEnterFun(TRUE);
	m_BtnAbout.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(2);
	SetObjNum(9);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndMainMenu,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndMainMenu)
	ON_HMIBTN_ENTER(3, OnEnterBtnInput)
	ON_HMIBTN_ENTER(4, OnEnterBtnSoftSwitch)
	ON_HMIBTN_ENTER(6, OnEnterBtnSystem)
	ON_HMIBTN_ENTER(7, OnEnterBtntest)
	ON_HMIBTN_ENTER(9, OnEnterBtnAbout)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口Measure: 测量*******************
CLcdWndMeasure::~CLcdWndMeasure()
{
}
CLcdWndMeasure::CLcdWndMeasure()
{
}
BOOL CLcdWndMeasure::Init()
{
	SetClassName("CLcdWndMeasure");
	SetCaption(tr("测量"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnHMeasure.SetPosition(21,28,216,53,0);
	m_BtnHMeasure.SetTabOrder(1);
	m_pObjList[1] = &m_BtnHMeasure;
	m_BtnHMeasure.SetFont(0);
	m_BtnHMeasure.SetOwner(this);
	m_BtnHMeasure.SetCaption(tr("一次侧测量值"));
	m_BtnHMeasure.SetHaveEnterFun(TRUE);
	m_BtnHMeasure.SetEnterWnd(NULL);
	m_BtnLMeasure.SetPosition(21,61,216,86,0);
	m_BtnLMeasure.SetTabOrder(2);
	m_pObjList[2] = &m_BtnLMeasure;
	m_BtnLMeasure.SetFont(0);
	m_BtnLMeasure.SetOwner(this);
	m_BtnLMeasure.SetCaption(tr("二次侧测量值"));
	m_BtnLMeasure.SetHaveEnterFun(TRUE);
	m_BtnLMeasure.SetEnterWnd(NULL);
	m_BtnPML.SetPosition(21,94,216,119,0);
	m_BtnPML.SetTabOrder(3);
	m_pObjList[3] = &m_BtnPML;
	m_BtnPML.SetFont(0);
	m_BtnPML.SetOwner(this);
	m_BtnPML.SetCaption(tr("二次侧保护值"));
	m_BtnPML.SetHaveEnterFun(TRUE);
	m_BtnPML.SetEnterWnd(NULL);
	m_BtnAngle.SetPosition(21,128,216,153,0);
	m_BtnAngle.SetTabOrder(4);
	m_pObjList[4] = &m_BtnAngle;
	m_BtnAngle.SetFont(16);
	m_BtnAngle.SetOwner(this);
	m_BtnAngle.SetCaption(tr("电压电流相位"));
	m_BtnAngle.SetHaveEnterFun(TRUE);
	m_BtnAngle.SetEnterWnd(NULL);
	m_BtnKilowatt.SetPosition(22,29,217,54,1);
	m_BtnKilowatt.SetTabOrder(5);
	m_pObjList[5] = &m_BtnKilowatt;
	m_BtnKilowatt.SetFont(16);
	m_BtnKilowatt.SetOwner(this);
	m_BtnKilowatt.SetCaption(tr("实时电度值"));
	m_BtnKilowatt.SetHaveEnterFun(TRUE);
	m_BtnKilowatt.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(2);
	SetObjNum(5);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndMeasure,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndMeasure)
	ON_HMIBTN_ENTER(1, OnEnterBtnHMeasure)
	ON_HMIBTN_ENTER(2, OnEnterBtnLMeasure)
	ON_HMIBTN_ENTER(3, OnEnterBtnPML)
	ON_HMIBTN_ENTER(4, OnEnterBtnAngle)
	ON_HMIBTN_ENTER(5, OnEnterBtnKilowatt)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口ASModify: 公用定值修改*******************
CLcdWndASModify::~CLcdWndASModify()
{
}
CLcdWndASModify::CLcdWndASModify()
{
}
BOOL CLcdWndASModify::Init()
{
	SetClassName("CLcdWndASModify");
	SetCaption(tr("公用定值修改"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditAsInput.SetPosition(102,38,187,58,0);
	m_EditAsInput.SetTabOrder(1);
	m_pObjList[1] = &m_EditAsInput;
	m_EditAsInput.SetFont(16);
	m_EditAsInput.SetOwner(this);
	m_EditAsInput.SetCaption(tr(""));
	m_EditAsInput.SetMaxLen(8);
	m_EditAsInput.SetDecimalNum(2);
	m_EditAsInput.SetValueRange(500,-500);
	m_EditAsInput.SetIsShowNegative(FALSE);
	m_EditAsInput.SetUDFun(-1);
	m_EditAsInput.SetLRFun(0);
	m_EditAsInput.SetAddSubFun(3);
	m_EditAsInput.SetHaveUpDownFun(FALSE);
	m_EditAsInput.SetHaveLeftRightFun(FALSE);
	m_EditAsInput.SetHaveEnterFun(FALSE);
	m_EditAsInput.SetHaveAddSubFun(FALSE);
	m_StaticASSymbol.SetPosition(14,41,94,56,0);
	m_StaticASSymbol.SetTabOrder(-3);
	m_pObjList[3] = &m_StaticASSymbol;
	m_StaticASSymbol.SetFont(16);
	m_StaticASSymbol.SetOwner(this);
	m_StaticASSymbol.SetCaption(tr("AS1="));
	m_StaticASSymbol.SetDataSource(-1,0);
	m_StaticASSymbol.SetTimer(0);
	m_StaticAsUnit.SetPosition(195,38,230,53,0);
	m_StaticAsUnit.SetTabOrder(-4);
	m_pObjList[4] = &m_StaticAsUnit;
	m_StaticAsUnit.SetFont(16);
	m_StaticAsUnit.SetOwner(this);
	m_StaticAsUnit.SetCaption(tr("V"));
	m_StaticAsUnit.SetDataSource(-1,0);
	m_StaticAsUnit.SetTimer(0);
	m_StaticDownLimit.SetPosition(14,111,159,126,0);
	m_StaticDownLimit.SetTabOrder(-5);
	m_pObjList[5] = &m_StaticDownLimit;
	m_StaticDownLimit.SetFont(16);
	m_StaticDownLimit.SetOwner(this);
	m_StaticDownLimit.SetCaption(tr("下限: 000v"));
	m_StaticDownLimit.SetDataSource(-1,0);
	m_StaticDownLimit.SetTimer(0);
	m_StaticUpLimit.SetPosition(14,134,179,149,0);
	m_StaticUpLimit.SetTabOrder(-6);
	m_pObjList[6] = &m_StaticUpLimit;
	m_StaticUpLimit.SetFont(16);
	m_StaticUpLimit.SetOwner(this);
	m_StaticUpLimit.SetCaption(tr("上限："));
	m_StaticUpLimit.SetDataSource(-1,0);
	m_StaticUpLimit.SetTimer(0);
	m_StaticAsName.SetPosition(14,80,234,100,0);
	m_StaticAsName.SetTabOrder(-7);
	m_pObjList[7] = &m_StaticAsName;
	m_StaticAsName.SetFont(16);
	m_StaticAsName.SetOwner(this);
	m_StaticAsName.SetCaption(tr("辅助定值名称"));
	m_StaticAsName.SetDataSource(-1,0);
	m_StaticAsName.SetTimer(0);
	m_ComboAsCombo.SetPosition(128,63,228,88,0);
	m_ComboAsCombo.SetTabOrder(2);
	m_pObjList[2] = &m_ComboAsCombo;
	m_ComboAsCombo.SetFont(12);
	m_ComboAsCombo.SetOwner(this);
	m_ComboAsCombo.SetAddSubFun(3);
	m_ComboAsCombo.SetHaveEnterFun(FALSE);
	m_ComboAsCombo.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(7);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndASModify,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndASModify)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口ASQuery: 公用定值查询*******************
CLcdWndASQuery::~CLcdWndASQuery()
{
}
CLcdWndASQuery::CLcdWndASQuery()
{
}
BOOL CLcdWndASQuery::Init()
{
	SetClassName("CLcdWndASQuery");
	SetCaption(tr("公用定值查询"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_ListASList.SetPosition(4,26,234,151,0);
	m_ListASList.SetTabOrder(1);
	m_pObjList[1] = &m_ListASList;
	m_ListASList.SetFont(16);
	m_ListASList.SetOwner(this);
	m_ListASList.SetType(ASQuery);
	m_ListASList.SetOwner(this);
	m_ListASList.SetTimer(0);
	m_ListASList.SetUDFun(1);
	m_ListASList.SetLRFun(1);
	m_ListASList.SetHaveUpDownFun(FALSE);
	m_ListASList.SetHaveLeftRightFun(FALSE);
	m_ListASList.SetHaveEnterFun(FALSE);
	m_ListASList.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndASQuery,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndASQuery)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口Report: 报告*******************
CLcdWndReport::~CLcdWndReport()
{
}
CLcdWndReport::CLcdWndReport()
{
}
BOOL CLcdWndReport::Init()
{
	SetClassName("CLcdWndReport");
	SetCaption(tr("报告"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnActionReport.SetPosition(26,28,216,53,0);
	m_BtnActionReport.SetTabOrder(1);
	m_pObjList[1] = &m_BtnActionReport;
	m_BtnActionReport.SetFont(0);
	m_BtnActionReport.SetOwner(this);
	m_BtnActionReport.SetCaption(tr("动作报告"));
	m_BtnActionReport.SetHaveEnterFun(TRUE);
	m_BtnActionReport.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndActionReport));
	m_BtnAlarmReport.SetPosition(26,61,216,86,0);
	m_BtnAlarmReport.SetTabOrder(2);
	m_pObjList[2] = &m_BtnAlarmReport;
	m_BtnAlarmReport.SetFont(0);
	m_BtnAlarmReport.SetOwner(this);
	m_BtnAlarmReport.SetCaption(tr("告警报告"));
	m_BtnAlarmReport.SetHaveEnterFun(TRUE);
	m_BtnAlarmReport.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAlarmReport));
	m_BtnStartReport.SetPosition(26,94,216,119,0);
	m_BtnStartReport.SetTabOrder(3);
	m_pObjList[3] = &m_BtnStartReport;
	m_BtnStartReport.SetFont(0);
	m_BtnStartReport.SetOwner(this);
	m_BtnStartReport.SetCaption(tr("启动报告"));
	m_BtnStartReport.SetHaveEnterFun(TRUE);
	m_BtnStartReport.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndStartReport));
	m_BtnSoe.SetPosition(26,127,216,152,0);
	m_BtnSoe.SetTabOrder(4);
	m_pObjList[4] = &m_BtnSoe;
	m_BtnSoe.SetFont(0);
	m_BtnSoe.SetOwner(this);
	m_BtnSoe.SetCaption(tr("SOE事项"));
	m_BtnSoe.SetHaveEnterFun(TRUE);
	m_BtnSoe.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSoeReport));
	m_BtnOperation.SetPosition(26,33,216,58,1);
	m_BtnOperation.SetTabOrder(5);
	m_pObjList[5] = &m_BtnOperation;
	m_BtnOperation.SetFont(0);
	m_BtnOperation.SetOwner(this);
	m_BtnOperation.SetCaption(tr("操作事项"));
	m_BtnOperation.SetHaveEnterFun(TRUE);
	m_BtnOperation.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndOperationReport));
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(2);
	SetObjNum(5);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndReport,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndReport)
	ON_HMIBTN_ENTER(1, OnEnterBtnActionReport)
	ON_HMIBTN_ENTER(2, OnEnterBtnAlarmReport)
	ON_HMIBTN_ENTER(3, OnEnterBtnStartReport)
	ON_HMIBTN_ENTER(4, OnEnterBtnSoe)
	ON_HMIBTN_ENTER(5, OnEnterBtnOperation)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口ActionReport: 动作报告*******************
CLcdWndActionReport::~CLcdWndActionReport()
{
}
CLcdWndActionReport::CLcdWndActionReport()
{
}
BOOL CLcdWndActionReport::Init()
{
	SetClassName("CLcdWndActionReport");
	SetCaption(tr("动作报告"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListActionReport.SetPosition(8,27,233,157,0);
	m_ListActionReport.SetTabOrder(1);
	m_pObjList[1] = &m_ListActionReport;
	m_ListActionReport.SetFont(12);
	m_ListActionReport.SetOwner(this);
	m_ListActionReport.SetType(Action);
	m_ListActionReport.SetOwner(this);
	m_ListActionReport.SetTimer(0);
	m_ListActionReport.SetUDFun(0);
	m_ListActionReport.SetLRFun(0);
	m_ListActionReport.SetHaveUpDownFun(FALSE);
	m_ListActionReport.SetHaveLeftRightFun(FALSE);
	m_ListActionReport.SetHaveEnterFun(TRUE);
	m_ListActionReport.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndActionReport,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndActionReport)
	ON_HMILIST_ENTER(1, OnEnterListActionReport)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口AlarmReport: 告警报告*******************
CLcdWndAlarmReport::~CLcdWndAlarmReport()
{
}
CLcdWndAlarmReport::CLcdWndAlarmReport()
{
}
BOOL CLcdWndAlarmReport::Init()
{
	SetClassName("CLcdWndAlarmReport");
	SetCaption(tr("告警报告"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListAlarmReport.SetPosition(4,25,234,150,0);
	m_ListAlarmReport.SetTabOrder(1);
	m_pObjList[1] = &m_ListAlarmReport;
	m_ListAlarmReport.SetFont(16);
	m_ListAlarmReport.SetOwner(this);
	m_ListAlarmReport.SetType(Alarm);
	m_ListAlarmReport.SetOwner(this);
	m_ListAlarmReport.SetTimer(0);
	m_ListAlarmReport.SetUDFun(0);
	m_ListAlarmReport.SetLRFun(0);
	m_ListAlarmReport.SetHaveUpDownFun(FALSE);
	m_ListAlarmReport.SetHaveLeftRightFun(FALSE);
	m_ListAlarmReport.SetHaveEnterFun(TRUE);
	m_ListAlarmReport.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndAlarmReport,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndAlarmReport)
	ON_HMILIST_ENTER(1, OnEnterListAlarmReport)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口StartReport: 启动报告*******************
CLcdWndStartReport::~CLcdWndStartReport()
{
}
CLcdWndStartReport::CLcdWndStartReport()
{
}
BOOL CLcdWndStartReport::Init()
{
	SetClassName("CLcdWndStartReport");
	SetCaption(tr("启动报告"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListStartReport.SetPosition(8,28,228,158,0);
	m_ListStartReport.SetTabOrder(1);
	m_pObjList[1] = &m_ListStartReport;
	m_ListStartReport.SetFont(16);
	m_ListStartReport.SetOwner(this);
	m_ListStartReport.SetType(Start);
	m_ListStartReport.SetOwner(this);
	m_ListStartReport.SetTimer(0);
	m_ListStartReport.SetUDFun(0);
	m_ListStartReport.SetLRFun(0);
	m_ListStartReport.SetHaveUpDownFun(FALSE);
	m_ListStartReport.SetHaveLeftRightFun(FALSE);
	m_ListStartReport.SetHaveEnterFun(TRUE);
	m_ListStartReport.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndStartReport,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndStartReport)
	ON_HMILIST_ENTER(1, OnEnterListStartReport)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口OperationReport: 操作事项*******************
CLcdWndOperationReport::~CLcdWndOperationReport()
{
}
CLcdWndOperationReport::CLcdWndOperationReport()
{
}
BOOL CLcdWndOperationReport::Init()
{
	SetClassName("CLcdWndOperationReport");
	SetCaption(tr("操作事项"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	SetEnterWnd(NULL);
	SetUDFun(-1);
	SetUDOffset(1);
	SetLRFun(-1);
	SetLROffset(1);
	m_ListOperationReport.SetPosition(3,22,235,155,0);
	m_ListOperationReport.SetTabOrder(1);
	m_pObjList[1] = &m_ListOperationReport;
	m_ListOperationReport.SetFont(16);
	m_ListOperationReport.SetOwner(this);
	m_ListOperationReport.SetType(Operation);
	m_ListOperationReport.SetOwner(this);
	m_ListOperationReport.SetTimer(0);
	m_ListOperationReport.SetUDFun(1);
	m_ListOperationReport.SetLRFun(-1);
	m_ListOperationReport.SetHaveUpDownFun(FALSE);
	m_ListOperationReport.SetHaveLeftRightFun(FALSE);
	m_ListOperationReport.SetHaveEnterFun(FALSE);
	m_ListOperationReport.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndOperationReport,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndOperationReport)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingQueryAreaNumInput: 定值查询*******************
CLcdWndSettingQueryAreaNumInput::~CLcdWndSettingQueryAreaNumInput()
{
}
CLcdWndSettingQueryAreaNumInput::CLcdWndSettingQueryAreaNumInput()
{
}
BOOL CLcdWndSettingQueryAreaNumInput::Init()
{
	SetClassName("CLcdWndSettingQueryAreaNumInput");
	SetCaption(tr("定值查询"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndconfigQury));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditAreaNum.SetPosition(180,79,210,99,0);
	m_EditAreaNum.SetTabOrder(1);
	m_pObjList[1] = &m_EditAreaNum;
	m_EditAreaNum.SetFont(16);
	m_EditAreaNum.SetOwner(this);
	m_EditAreaNum.SetCaption(tr("00"));
	m_EditAreaNum.SetMaxLen(2);
	m_EditAreaNum.SetDecimalNum(0);
	m_EditAreaNum.SetValueRange(15,0);
	m_EditAreaNum.SetIsShowNegative(FALSE);
	m_EditAreaNum.SetUDFun(0);
	m_EditAreaNum.SetLRFun(3);
	m_EditAreaNum.SetAddSubFun(3);
	m_EditAreaNum.SetHaveUpDownFun(FALSE);
	m_EditAreaNum.SetHaveLeftRightFun(FALSE);
	m_EditAreaNum.SetHaveEnterFun(FALSE);
	m_EditAreaNum.SetHaveAddSubFun(FALSE);
	m_Static4883.SetPosition(14,80,169,100,0);
	m_Static4883.SetTabOrder(-4);
	m_pObjList[4] = &m_Static4883;
	m_Static4883.SetFont(16);
	m_Static4883.SetOwner(this);
	m_Static4883.SetCaption(tr("查询区号："));
	m_Static4883.SetDataSource(-1,0);
	m_Static4883.SetTimer(0);
	m_Static4915.SetPosition(14,44,164,64,0);
	m_Static4915.SetTabOrder(-3);
	m_pObjList[3] = &m_Static4915;
	m_Static4915.SetFont(16);
	m_Static4915.SetOwner(this);
	m_Static4915.SetCaption(tr("当前运行区号："));
	m_Static4915.SetDataSource(-1,0);
	m_Static4915.SetTimer(0);
	m_StaticNowAreaNum.SetPosition(180,44,210,59,0);
	m_StaticNowAreaNum.SetTabOrder(-2);
	m_pObjList[2] = &m_StaticNowAreaNum;
	m_StaticNowAreaNum.SetFont(16);
	m_StaticNowAreaNum.SetOwner(this);
	m_StaticNowAreaNum.SetCaption(tr("0"));
	m_StaticNowAreaNum.SetDataSource(-1,0);
	m_StaticNowAreaNum.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingQueryAreaNumInput,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingQueryAreaNumInput)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SoeReport: SOE报告*******************
CLcdWndSoeReport::~CLcdWndSoeReport()
{
}
CLcdWndSoeReport::CLcdWndSoeReport()
{
}
BOOL CLcdWndSoeReport::Init()
{
	SetClassName("CLcdWndSoeReport");
	SetCaption(tr("SOE报告"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndReport));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListSoeReport.SetPosition(5,26,235,156,0);
	m_ListSoeReport.SetTabOrder(1);
	m_pObjList[1] = &m_ListSoeReport;
	m_ListSoeReport.SetFont(12);
	m_ListSoeReport.SetOwner(this);
	m_ListSoeReport.SetType(Soe);
	m_ListSoeReport.SetOwner(this);
	m_ListSoeReport.SetTimer(0);
	m_ListSoeReport.SetUDFun(0);
	m_ListSoeReport.SetLRFun(0);
	m_ListSoeReport.SetHaveUpDownFun(FALSE);
	m_ListSoeReport.SetHaveLeftRightFun(FALSE);
	m_ListSoeReport.SetHaveEnterFun(FALSE);
	m_ListSoeReport.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSoeReport,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSoeReport)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口Setting: 定值*******************
CLcdWndSetting::~CLcdWndSetting()
{
}
CLcdWndSetting::CLcdWndSetting()
{
}
BOOL CLcdWndSetting::Init()
{
	SetClassName("CLcdWndSetting");
	SetCaption(tr("定值"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnSettingQuery.SetPosition(26,28,211,53,0);
	m_BtnSettingQuery.SetTabOrder(1);
	m_pObjList[1] = &m_BtnSettingQuery;
	m_BtnSettingQuery.SetFont(0);
	m_BtnSettingQuery.SetOwner(this);
	m_BtnSettingQuery.SetCaption(tr("定值查询"));
	m_BtnSettingQuery.SetHaveEnterFun(TRUE);
	m_BtnSettingQuery.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSettingQueryAreaNumInput));
	m_BtnsettingModify.SetPosition(26,61,211,86,0);
	m_BtnsettingModify.SetTabOrder(2);
	m_pObjList[2] = &m_BtnsettingModify;
	m_BtnsettingModify.SetFont(0);
	m_BtnsettingModify.SetOwner(this);
	m_BtnsettingModify.SetCaption(tr("定值修改"));
	m_BtnsettingModify.SetHaveEnterFun(TRUE);
	m_BtnsettingModify.SetEnterWnd(NULL);
	m_BtnSettingCopy.SetPosition(26,94,211,119,0);
	m_BtnSettingCopy.SetTabOrder(3);
	m_pObjList[3] = &m_BtnSettingCopy;
	m_BtnSettingCopy.SetFont(0);
	m_BtnSettingCopy.SetOwner(this);
	m_BtnSettingCopy.SetCaption(tr("定值复制"));
	m_BtnSettingCopy.SetHaveEnterFun(TRUE);
	m_BtnSettingCopy.SetEnterWnd(NULL);
	m_BtnsettingExChg.SetPosition(26,127,211,152,0);
	m_BtnsettingExChg.SetTabOrder(4);
	m_pObjList[4] = &m_BtnsettingExChg;
	m_BtnsettingExChg.SetFont(0);
	m_BtnsettingExChg.SetOwner(this);
	m_BtnsettingExChg.SetCaption(tr("定值切换"));
	m_BtnsettingExChg.SetHaveEnterFun(TRUE);
	m_BtnsettingExChg.SetEnterWnd(NULL);
	m_BtnASQuery.SetPosition(26,27,211,52,1);
	m_BtnASQuery.SetTabOrder(5);
	m_pObjList[5] = &m_BtnASQuery;
	m_BtnASQuery.SetFont(16);
	m_BtnASQuery.SetOwner(this);
	m_BtnASQuery.SetCaption(tr("公用定值查询"));
	m_BtnASQuery.SetHaveEnterFun(FALSE);
	m_BtnASQuery.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndASQuery));
	m_BtnASModify.SetPosition(26,60,211,85,1);
	m_BtnASModify.SetTabOrder(6);
	m_pObjList[6] = &m_BtnASModify;
	m_BtnASModify.SetFont(16);
	m_BtnASModify.SetOwner(this);
	m_BtnASModify.SetCaption(tr("公用定值修改"));
	m_BtnASModify.SetHaveEnterFun(TRUE);
	m_BtnASModify.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(2);
	SetObjNum(6);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSetting,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSetting)
	ON_HMIBTN_ENTER(1, OnEnterBtnSettingQuery)
	ON_HMIBTN_ENTER(2, OnEnterBtnsettingModify)
	ON_HMIBTN_ENTER(3, OnEnterBtnSettingCopy)
	ON_HMIBTN_ENTER(4, OnEnterBtnsettingExChg)
	ON_HMIBTN_ENTER(6, OnEnterBtnASModify)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口configQury: 配置查看：1区*******************
CLcdWndconfigQury::~CLcdWndconfigQury()
{
}
CLcdWndconfigQury::CLcdWndconfigQury()
{
}
BOOL CLcdWndconfigQury::Init()
{
	SetClassName("CLcdWndconfigQury");
	SetCaption(tr("配置查看：1区"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSettingQuery));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListConfigQuery.SetPosition(6,26,236,156,0);
	m_ListConfigQuery.SetTabOrder(1);
	m_pObjList[1] = &m_ListConfigQuery;
	m_ListConfigQuery.SetFont(16);
	m_ListConfigQuery.SetOwner(this);
	m_ListConfigQuery.SetType(ConfigQuery);
	m_ListConfigQuery.SetOwner(this);
	m_ListConfigQuery.SetTimer(0);
	m_ListConfigQuery.SetUDFun(1);
	m_ListConfigQuery.SetLRFun(1);
	m_ListConfigQuery.SetHaveUpDownFun(FALSE);
	m_ListConfigQuery.SetHaveLeftRightFun(FALSE);
	m_ListConfigQuery.SetHaveEnterFun(FALSE);
	m_ListConfigQuery.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndconfigQury,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndconfigQury)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingQuery: 定值查询：1区  突变量定值*******************
CLcdWndSettingQuery::~CLcdWndSettingQuery()
{
}
CLcdWndSettingQuery::CLcdWndSettingQuery()
{
}
BOOL CLcdWndSettingQuery::Init()
{
	SetClassName("CLcdWndSettingQuery");
	SetCaption(tr("定值查询：1区  突变量定值"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListSettingQuery.SetPosition(1,24,240,159,0);
	m_ListSettingQuery.SetTabOrder(1);
	m_pObjList[1] = &m_ListSettingQuery;
	m_ListSettingQuery.SetFont(16);
	m_ListSettingQuery.SetOwner(this);
	m_ListSettingQuery.SetType(SettingQuery);
	m_ListSettingQuery.SetOwner(this);
	m_ListSettingQuery.SetTimer(0);
	m_ListSettingQuery.SetUDFun(0);
	m_ListSettingQuery.SetLRFun(0);
	m_ListSettingQuery.SetHaveUpDownFun(FALSE);
	m_ListSettingQuery.SetHaveLeftRightFun(FALSE);
	m_ListSettingQuery.SetHaveEnterFun(FALSE);
	m_ListSettingQuery.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingQuery,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingQuery)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口configModify: 配置修改：1区*******************
CLcdWndconfigModify::~CLcdWndconfigModify()
{
}
CLcdWndconfigModify::CLcdWndconfigModify()
{
}
BOOL CLcdWndconfigModify::Init()
{
	SetClassName("CLcdWndconfigModify");
	SetCaption(tr("配置修改：1区"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSettingModify));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListConfigModify.SetPosition(8,31,228,141,0);
	m_ListConfigModify.SetTabOrder(1);
	m_pObjList[1] = &m_ListConfigModify;
	m_ListConfigModify.SetFont(16);
	m_ListConfigModify.SetOwner(this);
	m_ListConfigModify.SetType(ConfigChange);
	m_ListConfigModify.SetOwner(this);
	m_ListConfigModify.SetTimer(0);
	m_ListConfigModify.SetUDFun(0);
	m_ListConfigModify.SetLRFun(0);
	m_ListConfigModify.SetHaveUpDownFun(FALSE);
	m_ListConfigModify.SetHaveLeftRightFun(FALSE);
	m_ListConfigModify.SetHaveEnterFun(TRUE);
	m_ListConfigModify.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndconfigModify,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndconfigModify)
	ON_HMILIST_ENTER(1, OnEnterListConfigModify)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingModify: 定值修改：1区*******************
CLcdWndSettingModify::~CLcdWndSettingModify()
{
}
CLcdWndSettingModify::CLcdWndSettingModify()
{
}
BOOL CLcdWndSettingModify::Init()
{
	SetClassName("CLcdWndSettingModify");
	SetCaption(tr("定值修改：1区"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSettingSave));
	SetUDFun(2);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditSettingData.SetPosition(34,62,184,87,0);
	m_EditSettingData.SetTabOrder(2);
	m_pObjList[2] = &m_EditSettingData;
	m_EditSettingData.SetFont(16);
	m_EditSettingData.SetOwner(this);
	m_EditSettingData.SetCaption(tr("0.5"));
	m_EditSettingData.SetMaxLen(8);
	m_EditSettingData.SetDecimalNum(2);
	m_EditSettingData.SetValueRange(500,-500);
	m_EditSettingData.SetIsShowNegative(FALSE);
	m_EditSettingData.SetUDFun(-1);
	m_EditSettingData.SetLRFun(0);
	m_EditSettingData.SetAddSubFun(3);
	m_EditSettingData.SetHaveUpDownFun(FALSE);
	m_EditSettingData.SetHaveLeftRightFun(FALSE);
	m_EditSettingData.SetHaveEnterFun(FALSE);
	m_EditSettingData.SetHaveAddSubFun(FALSE);
	m_StaticSymbol.SetPosition(16,36,61,56,0);
	m_StaticSymbol.SetTabOrder(-5);
	m_pObjList[5] = &m_StaticSymbol;
	m_StaticSymbol.SetFont(16);
	m_StaticSymbol.SetOwner(this);
	m_StaticSymbol.SetCaption(tr("IQD ="));
	m_StaticSymbol.SetDataSource(-1,0);
	m_StaticSymbol.SetTimer(-1);
	m_StaticName.SetPosition(16,99,221,124,0);
	m_StaticName.SetTabOrder(-4);
	m_pObjList[4] = &m_StaticName;
	m_StaticName.SetFont(16);
	m_StaticName.SetOwner(this);
	m_StaticName.SetCaption(tr("突变量启动定值"));
	m_StaticName.SetDataSource(-1,0);
	m_StaticName.SetTimer(-1);
	m_StaticUpLimit.SetPosition(131,129,231,149,0);
	m_StaticUpLimit.SetTabOrder(-6);
	m_pObjList[6] = &m_StaticUpLimit;
	m_StaticUpLimit.SetFont(16);
	m_StaticUpLimit.SetOwner(this);
	m_StaticUpLimit.SetCaption(tr("下限 0.0"));
	m_StaticUpLimit.SetDataSource(-1,0);
	m_StaticUpLimit.SetTimer(-1);
	m_StaticDownLimit.SetPosition(16,129,116,149,0);
	m_StaticDownLimit.SetTabOrder(-3);
	m_pObjList[3] = &m_StaticDownLimit;
	m_StaticDownLimit.SetFont(16);
	m_StaticDownLimit.SetOwner(this);
	m_StaticDownLimit.SetCaption(tr("上限10.0"));
	m_StaticDownLimit.SetDataSource(-1,0);
	m_StaticDownLimit.SetTimer(0);
	m_StaticUnit.SetPosition(194,67,219,87,0);
	m_StaticUnit.SetTabOrder(-7);
	m_pObjList[7] = &m_StaticUnit;
	m_StaticUnit.SetFont(16);
	m_StaticUnit.SetOwner(this);
	m_StaticUnit.SetCaption(tr("A"));
	m_StaticUnit.SetDataSource(-1,0);
	m_StaticUnit.SetTimer(0);
	m_ComboSettingComb.SetPosition(133,31,218,56,0);
	m_ComboSettingComb.SetTabOrder(1);
	m_pObjList[1] = &m_ComboSettingComb;
	m_ComboSettingComb.SetFont(12);
	m_ComboSettingComb.SetOwner(this);
	m_ComboSettingComb.SetAddSubFun(3);
	m_ComboSettingComb.SetHaveEnterFun(FALSE);
	m_ComboSettingComb.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(7);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingModify,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingModify)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingSave: 定值固化*******************
CLcdWndSettingSave::~CLcdWndSettingSave()
{
}
CLcdWndSettingSave::CLcdWndSettingSave()
{
}
BOOL CLcdWndSettingSave::Init()
{
	SetClassName("CLcdWndSettingSave");
	SetCaption(tr("定值固化"));
	SetEscWnd(NULL);
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_Btnok.SetPosition(35,118,105,143,0);
	m_Btnok.SetTabOrder(1);
	m_pObjList[1] = &m_Btnok;
	m_Btnok.SetFont(0);
	m_Btnok.SetOwner(this);
	m_Btnok.SetCaption(tr("固化"));
	m_Btnok.SetHaveEnterFun(TRUE);
	m_Btnok.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSettingSwitchConfirm));
	m_Btncancel.SetPosition(133,118,203,143,0);
	m_Btncancel.SetTabOrder(2);
	m_pObjList[2] = &m_Btncancel;
	m_Btncancel.SetFont(0);
	m_Btncancel.SetOwner(this);
	m_Btncancel.SetCaption(tr("放弃"));
	m_Btncancel.SetHaveEnterFun(FALSE);
	m_Btncancel.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	m_Static4697.SetPosition(12,32,212,52,0);
	m_Static4697.SetTabOrder(-3);
	m_pObjList[3] = &m_Static4697;
	m_Static4697.SetFont(16);
	m_Static4697.SetOwner(this);
	m_Static4697.SetCaption(tr("请确认配置和定值"));
	m_Static4697.SetDataSource(-1,0);
	m_Static4697.SetTimer(-1);
	m_Static4698.SetPosition(12,56,212,76,0);
	m_Static4698.SetTabOrder(-5);
	m_pObjList[5] = &m_Static4698;
	m_Static4698.SetFont(16);
	m_Static4698.SetOwner(this);
	m_Static4698.SetCaption(tr("已经正确修改"));
	m_Static4698.SetDataSource(-1,0);
	m_Static4698.SetTimer(-1);
	m_Static4699.SetPosition(12,87,212,107,0);
	m_Static4699.SetTabOrder(-4);
	m_pObjList[4] = &m_Static4699;
	m_Static4699.SetFont(16);
	m_Static4699.SetOwner(this);
	m_Static4699.SetCaption(tr("是否固化？"));
	m_Static4699.SetDataSource(-1,0);
	m_Static4699.SetTimer(-1);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(5);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingSave,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingSave)
	ON_HMIBTN_ENTER(1, OnEnterBtnok)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingSwitch: 定值切换*******************
CLcdWndSettingSwitch::~CLcdWndSettingSwitch()
{
}
CLcdWndSettingSwitch::CLcdWndSettingSwitch()
{
}
BOOL CLcdWndSettingSwitch::Init()
{
	SetClassName("CLcdWndSettingSwitch");
	SetCaption(tr("定值切换"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSettingSwitchConfirm));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditAreaNum.SetPosition(163,89,213,109,0);
	m_EditAreaNum.SetTabOrder(1);
	m_pObjList[1] = &m_EditAreaNum;
	m_EditAreaNum.SetFont(16);
	m_EditAreaNum.SetOwner(this);
	m_EditAreaNum.SetCaption(tr("00"));
	m_EditAreaNum.SetMaxLen(2);
	m_EditAreaNum.SetDecimalNum(0);
	m_EditAreaNum.SetValueRange(15,0);
	m_EditAreaNum.SetIsShowNegative(FALSE);
	m_EditAreaNum.SetUDFun(0);
	m_EditAreaNum.SetLRFun(0);
	m_EditAreaNum.SetAddSubFun(3);
	m_EditAreaNum.SetHaveUpDownFun(FALSE);
	m_EditAreaNum.SetHaveLeftRightFun(FALSE);
	m_EditAreaNum.SetHaveEnterFun(FALSE);
	m_EditAreaNum.SetHaveAddSubFun(FALSE);
	m_Static4702.SetPosition(12,54,152,74,0);
	m_Static4702.SetTabOrder(-3);
	m_pObjList[3] = &m_Static4702;
	m_Static4702.SetFont(16);
	m_Static4702.SetOwner(this);
	m_Static4702.SetCaption(tr("当前定值区号："));
	m_Static4702.SetDataSource(-1,0);
	m_Static4702.SetTimer(-1);
	m_Static4706.SetPosition(12,90,152,110,0);
	m_Static4706.SetTabOrder(-2);
	m_pObjList[2] = &m_Static4706;
	m_Static4706.SetFont(16);
	m_Static4706.SetOwner(this);
	m_Static4706.SetCaption(tr("投入定值区号："));
	m_Static4706.SetDataSource(-1,0);
	m_Static4706.SetTimer(-1);
	m_StaticNowAreaNum.SetPosition(163,55,193,75,0);
	m_StaticNowAreaNum.SetTabOrder(-4);
	m_pObjList[4] = &m_StaticNowAreaNum;
	m_StaticNowAreaNum.SetFont(16);
	m_StaticNowAreaNum.SetOwner(this);
	m_StaticNowAreaNum.SetCaption(tr("00"));
	m_StaticNowAreaNum.SetDataSource(-1,-1);
	m_StaticNowAreaNum.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingSwitch,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingSwitch)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingCopy: 定值复制*******************
CLcdWndSettingCopy::~CLcdWndSettingCopy()
{
}
CLcdWndSettingCopy::CLcdWndSettingCopy()
{
}
BOOL CLcdWndSettingCopy::Init()
{
	SetClassName("CLcdWndSettingCopy");
	SetCaption(tr("定值复制"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_EditSoureAreaNum.SetPosition(165,38,215,58,0);
	m_EditSoureAreaNum.SetTabOrder(1);
	m_pObjList[1] = &m_EditSoureAreaNum;
	m_EditSoureAreaNum.SetFont(16);
	m_EditSoureAreaNum.SetOwner(this);
	m_EditSoureAreaNum.SetCaption(tr("00"));
	m_EditSoureAreaNum.SetMaxLen(2);
	m_EditSoureAreaNum.SetDecimalNum(0);
	m_EditSoureAreaNum.SetValueRange(15,0);
	m_EditSoureAreaNum.SetIsShowNegative(FALSE);
	m_EditSoureAreaNum.SetUDFun(-1);
	m_EditSoureAreaNum.SetLRFun(0);
	m_EditSoureAreaNum.SetAddSubFun(3);
	m_EditSoureAreaNum.SetHaveUpDownFun(FALSE);
	m_EditSoureAreaNum.SetHaveLeftRightFun(FALSE);
	m_EditSoureAreaNum.SetHaveEnterFun(FALSE);
	m_EditSoureAreaNum.SetHaveAddSubFun(FALSE);
	m_EditDstStartAreaNum.SetPosition(67,89,117,109,0);
	m_EditDstStartAreaNum.SetTabOrder(2);
	m_pObjList[2] = &m_EditDstStartAreaNum;
	m_EditDstStartAreaNum.SetFont(16);
	m_EditDstStartAreaNum.SetOwner(this);
	m_EditDstStartAreaNum.SetCaption(tr("00"));
	m_EditDstStartAreaNum.SetMaxLen(2);
	m_EditDstStartAreaNum.SetDecimalNum(0);
	m_EditDstStartAreaNum.SetValueRange(15,0);
	m_EditDstStartAreaNum.SetIsShowNegative(FALSE);
	m_EditDstStartAreaNum.SetUDFun(-1);
	m_EditDstStartAreaNum.SetLRFun(0);
	m_EditDstStartAreaNum.SetAddSubFun(3);
	m_EditDstStartAreaNum.SetHaveUpDownFun(FALSE);
	m_EditDstStartAreaNum.SetHaveLeftRightFun(FALSE);
	m_EditDstStartAreaNum.SetHaveEnterFun(FALSE);
	m_EditDstStartAreaNum.SetHaveAddSubFun(FALSE);
	m_EditDstEndAreaNum.SetPosition(90,124,140,144,0);
	m_EditDstEndAreaNum.SetTabOrder(3);
	m_pObjList[3] = &m_EditDstEndAreaNum;
	m_EditDstEndAreaNum.SetFont(16);
	m_EditDstEndAreaNum.SetOwner(this);
	m_EditDstEndAreaNum.SetCaption(tr("00"));
	m_EditDstEndAreaNum.SetMaxLen(2);
	m_EditDstEndAreaNum.SetDecimalNum(0);
	m_EditDstEndAreaNum.SetValueRange(15,0);
	m_EditDstEndAreaNum.SetIsShowNegative(FALSE);
	m_EditDstEndAreaNum.SetUDFun(-1);
	m_EditDstEndAreaNum.SetLRFun(0);
	m_EditDstEndAreaNum.SetAddSubFun(3);
	m_EditDstEndAreaNum.SetHaveUpDownFun(FALSE);
	m_EditDstEndAreaNum.SetHaveLeftRightFun(FALSE);
	m_EditDstEndAreaNum.SetHaveEnterFun(FALSE);
	m_EditDstEndAreaNum.SetHaveAddSubFun(FALSE);
	m_Static4710.SetPosition(14,38,154,58,0);
	m_Static4710.SetTabOrder(-7);
	m_pObjList[7] = &m_Static4710;
	m_Static4710.SetFont(0);
	m_Static4710.SetOwner(this);
	m_Static4710.SetCaption(tr("请输入源定值区号："));
	m_Static4710.SetDataSource(-1,0);
	m_Static4710.SetTimer(-1);
	m_Static4711.SetPosition(14,65,154,85,0);
	m_Static4711.SetTabOrder(-8);
	m_pObjList[8] = &m_Static4711;
	m_Static4711.SetFont(0);
	m_Static4711.SetOwner(this);
	m_Static4711.SetCaption(tr("请输入目标定值区号："));
	m_Static4711.SetDataSource(-1,0);
	m_Static4711.SetTimer(-1);
	m_Static4885.SetPosition(17,89,57,104,0);
	m_Static4885.SetTabOrder(-6);
	m_pObjList[6] = &m_Static4885;
	m_Static4885.SetFont(16);
	m_Static4885.SetOwner(this);
	m_Static4885.SetCaption(tr("从"));
	m_Static4885.SetDataSource(-1,1);
	m_Static4885.SetTimer(1);
	m_Static4886.SetPosition(134,89,214,104,0);
	m_Static4886.SetTabOrder(-5);
	m_pObjList[5] = &m_Static4886;
	m_Static4886.SetFont(16);
	m_Static4886.SetOwner(this);
	m_Static4886.SetCaption(tr("区到"));
	m_Static4886.SetDataSource(-1,1);
	m_Static4886.SetTimer(0);
	m_Static5350.SetPosition(153,124,213,144,0);
	m_Static5350.SetTabOrder(-4);
	m_pObjList[4] = &m_Static5350;
	m_Static5350.SetFont(16);
	m_Static5350.SetOwner(this);
	m_Static5350.SetCaption(tr("区"));
	m_Static5350.SetDataSource(-1,0);
	m_Static5350.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(8);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingCopy,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingCopy)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SoftSwitch: 软压板*******************
CLcdWndSoftSwitch::~CLcdWndSoftSwitch()
{
}
CLcdWndSoftSwitch::CLcdWndSoftSwitch()
{
}
BOOL CLcdWndSoftSwitch::Init()
{
	SetClassName("CLcdWndSoftSwitch");
	SetCaption(tr("软压板"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListSoftSwitch.SetPosition(10,30,230,145,0);
	m_ListSoftSwitch.SetTabOrder(1);
	m_pObjList[1] = &m_ListSoftSwitch;
	m_ListSoftSwitch.SetFont(16);
	m_ListSoftSwitch.SetOwner(this);
	m_ListSoftSwitch.SetType(SoftSwitch);
	m_ListSoftSwitch.SetOwner(this);
	m_ListSoftSwitch.SetTimer(0);
	m_ListSoftSwitch.SetUDFun(0);
	m_ListSoftSwitch.SetLRFun(0);
	m_ListSoftSwitch.SetHaveUpDownFun(FALSE);
	m_ListSoftSwitch.SetHaveLeftRightFun(FALSE);
	m_ListSoftSwitch.SetHaveEnterFun(TRUE);
	m_ListSoftSwitch.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSoftSwitch,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSoftSwitch)
	ON_HMILIST_ENTER(1, OnEnterListSoftSwitch)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SysSetting: 设置*******************
CLcdWndSysSetting::~CLcdWndSysSetting()
{
}
CLcdWndSysSetting::CLcdWndSysSetting()
{
}
BOOL CLcdWndSysSetting::Init()
{
	SetClassName("CLcdWndSysSetting");
	SetCaption(tr("设置"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnAdress.SetPosition(17,26,217,51,0);
	m_BtnAdress.SetTabOrder(1);
	m_pObjList[1] = &m_BtnAdress;
	m_BtnAdress.SetFont(0);
	m_BtnAdress.SetOwner(this);
	m_BtnAdress.SetCaption(tr("通信设置"));
	m_BtnAdress.SetHaveEnterFun(FALSE);
	m_BtnAdress.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_BtnInputSetting.SetPosition(17,57,217,82,0);
	m_BtnInputSetting.SetTabOrder(2);
	m_pObjList[2] = &m_BtnInputSetting;
	m_BtnInputSetting.SetFont(0);
	m_BtnInputSetting.SetOwner(this);
	m_BtnInputSetting.SetCaption(tr("开入设置"));
	m_BtnInputSetting.SetHaveEnterFun(FALSE);
	m_BtnInputSetting.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndInputSetting));
	m_BtnBOTime.SetPosition(17,89,217,114,0);
	m_BtnBOTime.SetTabOrder(3);
	m_pObjList[3] = &m_BtnBOTime;
	m_BtnBOTime.SetFont(16);
	m_BtnBOTime.SetOwner(this);
	m_BtnBOTime.SetCaption(tr("遥控时间"));
	m_BtnBOTime.SetHaveEnterFun(TRUE);
	m_BtnBOTime.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndBOTime));
	m_BtnCIBase.SetPosition(17,121,217,146,0);
	m_BtnCIBase.SetTabOrder(4);
	m_pObjList[4] = &m_BtnCIBase;
	m_BtnCIBase.SetFont(16);
	m_BtnCIBase.SetOwner(this);
	m_BtnCIBase.SetCaption(tr("电度参数"));
	m_BtnCIBase.SetHaveEnterFun(FALSE);
	m_BtnCIBase.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndCIPara));
	m_BtnDeviceTime.SetPosition(17,30,217,55,1);
	m_BtnDeviceTime.SetTabOrder(5);
	m_pObjList[5] = &m_BtnDeviceTime;
	m_BtnDeviceTime.SetFont(0);
	m_BtnDeviceTime.SetOwner(this);
	m_BtnDeviceTime.SetCaption(tr("装置时间"));
	m_BtnDeviceTime.SetHaveEnterFun(FALSE);
	m_BtnDeviceTime.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_Wndtime));
	m_BtnLanguage.SetPosition(17,62,217,87,1);
	m_BtnLanguage.SetTabOrder(6);
	m_pObjList[6] = &m_BtnLanguage;
	m_BtnLanguage.SetFont(16);
	m_BtnLanguage.SetOwner(this);
	m_BtnLanguage.SetCaption(tr("系统语言"));
	m_BtnLanguage.SetHaveEnterFun(TRUE);
	m_BtnLanguage.SetEnterWnd(NULL);
	m_BtnPassSetting.SetPosition(17,94,217,119,1);
	m_BtnPassSetting.SetTabOrder(7);
	m_pObjList[7] = &m_BtnPassSetting;
	m_BtnPassSetting.SetFont(0);
	m_BtnPassSetting.SetOwner(this);
	m_BtnPassSetting.SetCaption(tr("密码设置"));
	m_BtnPassSetting.SetHaveEnterFun(FALSE);
	m_BtnPassSetting.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndPassChange));
	m_BtnSysParam.SetPosition(17,126,217,151,1);
	m_BtnSysParam.SetTabOrder(8);
	m_pObjList[8] = &m_BtnSysParam;
	m_BtnSysParam.SetFont(0);
	m_BtnSysParam.SetOwner(this);
	m_BtnSysParam.SetCaption(tr("其他参数"));
	m_BtnSysParam.SetHaveEnterFun(FALSE);
	m_BtnSysParam.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_Wndsyspara));
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(2);
	SetObjNum(8);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSysSetting,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSysSetting)
	ON_HMIBTN_ENTER(3, OnEnterBtnBOTime)
	ON_HMIBTN_ENTER(6, OnEnterBtnLanguage)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口BOTime: 遥控保持时间*******************
CLcdWndBOTime::~CLcdWndBOTime()
{
}
CLcdWndBOTime::CLcdWndBOTime()
{
}
BOOL CLcdWndBOTime::Init()
{
	SetClassName("CLcdWndBOTime");
	SetCaption(tr("遥控保持时间"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(-1);
	m_ListBOTimeList.SetPosition(8,26,228,151,0);
	m_ListBOTimeList.SetTabOrder(1);
	m_pObjList[1] = &m_ListBOTimeList;
	m_ListBOTimeList.SetFont(16);
	m_ListBOTimeList.SetOwner(this);
	m_ListBOTimeList.SetType(BOTime);
	m_ListBOTimeList.SetOwner(this);
	m_ListBOTimeList.SetTimer(0);
	m_ListBOTimeList.SetUDFun(0);
	m_ListBOTimeList.SetLRFun(1);
	m_ListBOTimeList.SetHaveUpDownFun(FALSE);
	m_ListBOTimeList.SetHaveLeftRightFun(FALSE);
	m_ListBOTimeList.SetHaveEnterFun(FALSE);
	m_ListBOTimeList.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndBOTime,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndBOTime)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口CIPara: 电度参数*******************
CLcdWndCIPara::~CLcdWndCIPara()
{
}
CLcdWndCIPara::CLcdWndCIPara()
{
}
BOOL CLcdWndCIPara::Init()
{
	SetClassName("CLcdWndCIPara");
	SetCaption(tr("电度参数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnDIProperty.SetPosition(23,30,223,55,0);
	m_BtnDIProperty.SetTabOrder(1);
	m_pObjList[1] = &m_BtnDIProperty;
	m_BtnDIProperty.SetFont(16);
	m_BtnDIProperty.SetOwner(this);
	m_BtnDIProperty.SetCaption(tr("开入属性"));
	m_BtnDIProperty.SetHaveEnterFun(TRUE);
	m_BtnDIProperty.SetEnterWnd(NULL);
	m_BtnIntegralKW.SetPosition(23,63,223,88,0);
	m_BtnIntegralKW.SetTabOrder(2);
	m_pObjList[2] = &m_BtnIntegralKW;
	m_BtnIntegralKW.SetFont(16);
	m_BtnIntegralKW.SetOwner(this);
	m_BtnIntegralKW.SetCaption(tr("积分电度计算"));
	m_BtnIntegralKW.SetHaveEnterFun(TRUE);
	m_BtnIntegralKW.SetEnterWnd(NULL);
	m_BtnCIBase.SetPosition(23,96,223,121,0);
	m_BtnCIBase.SetTabOrder(3);
	m_pObjList[3] = &m_BtnCIBase;
	m_BtnCIBase.SetFont(16);
	m_BtnCIBase.SetOwner(this);
	m_BtnCIBase.SetCaption(tr("电度底数"));
	m_BtnCIBase.SetHaveEnterFun(TRUE);
	m_BtnCIBase.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndCiBase));
	m_BtnPulseConst.SetPosition(23,129,223,154,0);
	m_BtnPulseConst.SetTabOrder(4);
	m_pObjList[4] = &m_BtnPulseConst;
	m_BtnPulseConst.SetFont(16);
	m_BtnPulseConst.SetOwner(this);
	m_BtnPulseConst.SetCaption(tr("脉冲常数"));
	m_BtnPulseConst.SetHaveEnterFun(TRUE);
	m_BtnPulseConst.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndPulseConst));
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndCIPara,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndCIPara)
	ON_HMIBTN_ENTER(1, OnEnterBtnDIProperty)
	ON_HMIBTN_ENTER(2, OnEnterBtnIntegralKW)
	ON_HMIBTN_ENTER(3, OnEnterBtnCIBase)
	ON_HMIBTN_ENTER(4, OnEnterBtnPulseConst)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口time: 装置时间*******************
CLcdWndtime::~CLcdWndtime()
{
}
CLcdWndtime::CLcdWndtime()
{
}
BOOL CLcdWndtime::Init()
{
	SetClassName("CLcdWndtime");
	SetCaption(tr("装置时间"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(3);
	m_EditYear.SetPosition(14,55,69,75,0);
	m_EditYear.SetTabOrder(1);
	m_pObjList[1] = &m_EditYear;
	m_EditYear.SetFont(16);
	m_EditYear.SetOwner(this);
	m_EditYear.SetCaption(tr("2008"));
	m_EditYear.SetMaxLen(4);
	m_EditYear.SetDecimalNum(0);
	m_EditYear.SetValueRange(3000,0);
	m_EditYear.SetIsShowNegative(FALSE);
	m_EditYear.SetUDFun(-1);
	m_EditYear.SetLRFun(0);
	m_EditYear.SetAddSubFun(0);
	m_EditYear.SetHaveUpDownFun(FALSE);
	m_EditYear.SetHaveLeftRightFun(FALSE);
	m_EditYear.SetHaveEnterFun(FALSE);
	m_EditYear.SetHaveAddSubFun(FALSE);
	m_EditMonth.SetPosition(87,55,142,75,0);
	m_EditMonth.SetTabOrder(2);
	m_pObjList[2] = &m_EditMonth;
	m_EditMonth.SetFont(16);
	m_EditMonth.SetOwner(this);
	m_EditMonth.SetCaption(tr("12"));
	m_EditMonth.SetMaxLen(2);
	m_EditMonth.SetDecimalNum(0);
	m_EditMonth.SetValueRange(12,1);
	m_EditMonth.SetIsShowNegative(FALSE);
	m_EditMonth.SetUDFun(-1);
	m_EditMonth.SetLRFun(0);
	m_EditMonth.SetAddSubFun(0);
	m_EditMonth.SetHaveUpDownFun(FALSE);
	m_EditMonth.SetHaveLeftRightFun(FALSE);
	m_EditMonth.SetHaveEnterFun(FALSE);
	m_EditMonth.SetHaveAddSubFun(FALSE);
	m_EditDay.SetPosition(161,55,216,75,0);
	m_EditDay.SetTabOrder(3);
	m_pObjList[3] = &m_EditDay;
	m_EditDay.SetFont(16);
	m_EditDay.SetOwner(this);
	m_EditDay.SetCaption(tr("01"));
	m_EditDay.SetMaxLen(2);
	m_EditDay.SetDecimalNum(0);
	m_EditDay.SetValueRange(31,1);
	m_EditDay.SetIsShowNegative(FALSE);
	m_EditDay.SetUDFun(-1);
	m_EditDay.SetLRFun(0);
	m_EditDay.SetAddSubFun(0);
	m_EditDay.SetHaveUpDownFun(FALSE);
	m_EditDay.SetHaveLeftRightFun(FALSE);
	m_EditDay.SetHaveEnterFun(FALSE);
	m_EditDay.SetHaveAddSubFun(FALSE);
	m_EditHour.SetPosition(14,114,69,134,0);
	m_EditHour.SetTabOrder(4);
	m_pObjList[4] = &m_EditHour;
	m_EditHour.SetFont(16);
	m_EditHour.SetOwner(this);
	m_EditHour.SetCaption(tr("13"));
	m_EditHour.SetMaxLen(2);
	m_EditHour.SetDecimalNum(0);
	m_EditHour.SetValueRange(23,0);
	m_EditHour.SetIsShowNegative(FALSE);
	m_EditHour.SetUDFun(-1);
	m_EditHour.SetLRFun(0);
	m_EditHour.SetAddSubFun(0);
	m_EditHour.SetHaveUpDownFun(FALSE);
	m_EditHour.SetHaveLeftRightFun(FALSE);
	m_EditHour.SetHaveEnterFun(FALSE);
	m_EditHour.SetHaveAddSubFun(FALSE);
	m_EditMinute.SetPosition(87,114,142,134,0);
	m_EditMinute.SetTabOrder(5);
	m_pObjList[5] = &m_EditMinute;
	m_EditMinute.SetFont(16);
	m_EditMinute.SetOwner(this);
	m_EditMinute.SetCaption(tr("15"));
	m_EditMinute.SetMaxLen(2);
	m_EditMinute.SetDecimalNum(0);
	m_EditMinute.SetValueRange(59,0);
	m_EditMinute.SetIsShowNegative(FALSE);
	m_EditMinute.SetUDFun(-1);
	m_EditMinute.SetLRFun(0);
	m_EditMinute.SetAddSubFun(0);
	m_EditMinute.SetHaveUpDownFun(FALSE);
	m_EditMinute.SetHaveLeftRightFun(FALSE);
	m_EditMinute.SetHaveEnterFun(FALSE);
	m_EditMinute.SetHaveAddSubFun(FALSE);
	m_EditSecond.SetPosition(161,114,216,134,0);
	m_EditSecond.SetTabOrder(6);
	m_pObjList[6] = &m_EditSecond;
	m_EditSecond.SetFont(16);
	m_EditSecond.SetOwner(this);
	m_EditSecond.SetCaption(tr("17"));
	m_EditSecond.SetMaxLen(2);
	m_EditSecond.SetDecimalNum(0);
	m_EditSecond.SetValueRange(59,0);
	m_EditSecond.SetIsShowNegative(FALSE);
	m_EditSecond.SetUDFun(-1);
	m_EditSecond.SetLRFun(0);
	m_EditSecond.SetAddSubFun(0);
	m_EditSecond.SetHaveUpDownFun(FALSE);
	m_EditSecond.SetHaveLeftRightFun(FALSE);
	m_EditSecond.SetHaveEnterFun(FALSE);
	m_EditSecond.SetHaveAddSubFun(FALSE);
	m_Static4737.SetPosition(14,32,69,52,0);
	m_Static4737.SetTabOrder(-12);
	m_pObjList[12] = &m_Static4737;
	m_Static4737.SetFont(16);
	m_Static4737.SetOwner(this);
	m_Static4737.SetCaption(tr("年"));
	m_Static4737.SetDataSource(-1,0);
	m_Static4737.SetTimer(-1);
	m_Static4738.SetPosition(87,32,142,52,0);
	m_Static4738.SetTabOrder(-11);
	m_pObjList[11] = &m_Static4738;
	m_Static4738.SetFont(16);
	m_Static4738.SetOwner(this);
	m_Static4738.SetCaption(tr("月"));
	m_Static4738.SetDataSource(-1,0);
	m_Static4738.SetTimer(-1);
	m_Static4739.SetPosition(14,89,69,109,0);
	m_Static4739.SetTabOrder(-10);
	m_pObjList[10] = &m_Static4739;
	m_Static4739.SetFont(16);
	m_Static4739.SetOwner(this);
	m_Static4739.SetCaption(tr("时"));
	m_Static4739.SetDataSource(-1,0);
	m_Static4739.SetTimer(-1);
	m_Static4740.SetPosition(87,89,142,109,0);
	m_Static4740.SetTabOrder(-9);
	m_pObjList[9] = &m_Static4740;
	m_Static4740.SetFont(16);
	m_Static4740.SetOwner(this);
	m_Static4740.SetCaption(tr("分"));
	m_Static4740.SetDataSource(-1,0);
	m_Static4740.SetTimer(-1);
	m_Static4741.SetPosition(161,89,216,109,0);
	m_Static4741.SetTabOrder(-8);
	m_pObjList[8] = &m_Static4741;
	m_Static4741.SetFont(16);
	m_Static4741.SetOwner(this);
	m_Static4741.SetCaption(tr("秒"));
	m_Static4741.SetDataSource(-1,0);
	m_Static4741.SetTimer(-1);
	m_Static4742.SetPosition(161,32,216,52,0);
	m_Static4742.SetTabOrder(-7);
	m_pObjList[7] = &m_Static4742;
	m_Static4742.SetFont(0);
	m_Static4742.SetOwner(this);
	m_Static4742.SetCaption(tr("日"));
	m_Static4742.SetDataSource(-1,0);
	m_Static4742.SetTimer(-1);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(12);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndtime,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndtime)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口InputSetting: 开入设置*******************
CLcdWndInputSetting::~CLcdWndInputSetting()
{
}
CLcdWndInputSetting::CLcdWndInputSetting()
{
}
BOOL CLcdWndInputSetting::Init()
{
	SetClassName("CLcdWndInputSetting");
	SetCaption(tr("开入设置"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnSoeProperty.SetPosition(18,31,213,56,0);
	m_BtnSoeProperty.SetTabOrder(1);
	m_pObjList[1] = &m_BtnSoeProperty;
	m_BtnSoeProperty.SetFont(0);
	m_BtnSoeProperty.SetOwner(this);
	m_BtnSoeProperty.SetCaption(tr("SOE属性"));
	m_BtnSoeProperty.SetHaveEnterFun(TRUE);
	m_BtnSoeProperty.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSoeProperty));
	m_BtnBIFilter.SetPosition(18,63,213,88,0);
	m_BtnBIFilter.SetTabOrder(2);
	m_pObjList[2] = &m_BtnBIFilter;
	m_BtnBIFilter.SetFont(0);
	m_BtnBIFilter.SetOwner(this);
	m_BtnBIFilter.SetCaption(tr("遥信去抖"));
	m_BtnBIFilter.SetHaveEnterFun(TRUE);
	m_BtnBIFilter.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndBIFilter));
	m_BtnBiNo.SetPosition(18,95,213,120,0);
	m_BtnBiNo.SetTabOrder(3);
	m_pObjList[3] = &m_BtnBiNo;
	m_BtnBiNo.SetFont(0);
	m_BtnBiNo.SetOwner(this);
	m_BtnBiNo.SetCaption(tr("遥信取反"));
	m_BtnBiNo.SetHaveEnterFun(TRUE);
	m_BtnBiNo.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndBiNot));
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(3);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndInputSetting,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndInputSetting)
	ON_HMIBTN_ENTER(1, OnEnterBtnSoeProperty)
	ON_HMIBTN_ENTER(2, OnEnterBtnBIFilter)
	ON_HMIBTN_ENTER(3, OnEnterBtnBiNo)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口BIFilter: 遥信去抖（单位：ms)*******************
CLcdWndBIFilter::~CLcdWndBIFilter()
{
}
CLcdWndBIFilter::CLcdWndBIFilter()
{
}
BOOL CLcdWndBIFilter::Init()
{
	SetClassName("CLcdWndBIFilter");
	SetCaption(tr("遥信去抖（单位：ms)"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndInputSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListListBiFilter.SetPosition(15,26,220,151,0);
	m_ListListBiFilter.SetTabOrder(1);
	m_pObjList[1] = &m_ListListBiFilter;
	m_ListListBiFilter.SetFont(16);
	m_ListListBiFilter.SetOwner(this);
	m_ListListBiFilter.SetType(DIFilterTime);
	m_ListListBiFilter.SetOwner(this);
	m_ListListBiFilter.SetTimer(0);
	m_ListListBiFilter.SetUDFun(0);
	m_ListListBiFilter.SetLRFun(0);
	m_ListListBiFilter.SetHaveUpDownFun(FALSE);
	m_ListListBiFilter.SetHaveLeftRightFun(FALSE);
	m_ListListBiFilter.SetHaveEnterFun(FALSE);
	m_ListListBiFilter.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndBIFilter,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndBIFilter)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口BiNot: 遥信取反*******************
CLcdWndBiNot::~CLcdWndBiNot()
{
}
CLcdWndBiNot::CLcdWndBiNot()
{
}
BOOL CLcdWndBiNot::Init()
{
	SetClassName("CLcdWndBiNot");
	SetCaption(tr("遥信取反"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndInputSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListBiNot.SetPosition(11,33,231,148,0);
	m_ListBiNot.SetTabOrder(1);
	m_pObjList[1] = &m_ListBiNot;
	m_ListBiNot.SetFont(16);
	m_ListBiNot.SetOwner(this);
	m_ListBiNot.SetType(DINor);
	m_ListBiNot.SetOwner(this);
	m_ListBiNot.SetTimer(0);
	m_ListBiNot.SetUDFun(0);
	m_ListBiNot.SetLRFun(0);
	m_ListBiNot.SetHaveUpDownFun(FALSE);
	m_ListBiNot.SetHaveLeftRightFun(FALSE);
	m_ListBiNot.SetHaveEnterFun(TRUE);
	m_ListBiNot.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndBiNot,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndBiNot)
	ON_HMILIST_ENTER(1, OnEnterListBiNot)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SoeProperty: SOE属性*******************
CLcdWndSoeProperty::~CLcdWndSoeProperty()
{
}
CLcdWndSoeProperty::CLcdWndSoeProperty()
{
}
BOOL CLcdWndSoeProperty::Init()
{
	SetClassName("CLcdWndSoeProperty");
	SetCaption(tr("SOE属性"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndInputSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListSoePropetry.SetPosition(7,24,232,154,0);
	m_ListSoePropetry.SetTabOrder(1);
	m_pObjList[1] = &m_ListSoePropetry;
	m_ListSoePropetry.SetFont(12);
	m_ListSoePropetry.SetOwner(this);
	m_ListSoePropetry.SetType(SoePara);
	m_ListSoePropetry.SetOwner(this);
	m_ListSoePropetry.SetTimer(0);
	m_ListSoePropetry.SetUDFun(0);
	m_ListSoePropetry.SetLRFun(0);
	m_ListSoePropetry.SetHaveUpDownFun(FALSE);
	m_ListSoePropetry.SetHaveLeftRightFun(FALSE);
	m_ListSoePropetry.SetHaveEnterFun(FALSE);
	m_ListSoePropetry.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSoeProperty,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSoeProperty)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口CiBase: 电度底数*******************
CLcdWndCiBase::~CLcdWndCiBase()
{
}
CLcdWndCiBase::CLcdWndCiBase()
{
}
BOOL CLcdWndCiBase::Init()
{
	SetClassName("CLcdWndCiBase");
	SetCaption(tr("电度底数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndCIPara));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndCIPara));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditIntKW1.SetPosition(135,33,215,53,0);
	m_EditIntKW1.SetTabOrder(1);
	m_pObjList[1] = &m_EditIntKW1;
	m_EditIntKW1.SetFont(16);
	m_EditIntKW1.SetOwner(this);
	m_EditIntKW1.SetCaption(tr("12345.67"));
	m_EditIntKW1.SetMaxLen(8);
	m_EditIntKW1.SetDecimalNum(2);
	m_EditIntKW1.SetValueRange(99999.99,0);
	m_EditIntKW1.SetIsShowNegative(FALSE);
	m_EditIntKW1.SetUDFun(-1);
	m_EditIntKW1.SetLRFun(0);
	m_EditIntKW1.SetAddSubFun(3);
	m_EditIntKW1.SetHaveUpDownFun(FALSE);
	m_EditIntKW1.SetHaveLeftRightFun(FALSE);
	m_EditIntKW1.SetHaveEnterFun(FALSE);
	m_EditIntKW1.SetHaveAddSubFun(FALSE);
	m_EditIntKW2.SetPosition(135,60,215,80,0);
	m_EditIntKW2.SetTabOrder(2);
	m_pObjList[2] = &m_EditIntKW2;
	m_EditIntKW2.SetFont(16);
	m_EditIntKW2.SetOwner(this);
	m_EditIntKW2.SetCaption(tr("12345.67"));
	m_EditIntKW2.SetMaxLen(8);
	m_EditIntKW2.SetDecimalNum(2);
	m_EditIntKW2.SetValueRange(99999.99,0);
	m_EditIntKW2.SetIsShowNegative(FALSE);
	m_EditIntKW2.SetUDFun(-1);
	m_EditIntKW2.SetLRFun(0);
	m_EditIntKW2.SetAddSubFun(3);
	m_EditIntKW2.SetHaveUpDownFun(FALSE);
	m_EditIntKW2.SetHaveLeftRightFun(FALSE);
	m_EditIntKW2.SetHaveEnterFun(FALSE);
	m_EditIntKW2.SetHaveAddSubFun(FALSE);
	m_EditPulseKW1.SetPosition(135,88,215,108,0);
	m_EditPulseKW1.SetTabOrder(3);
	m_pObjList[3] = &m_EditPulseKW1;
	m_EditPulseKW1.SetFont(16);
	m_EditPulseKW1.SetOwner(this);
	m_EditPulseKW1.SetCaption(tr("12345.67"));
	m_EditPulseKW1.SetMaxLen(8);
	m_EditPulseKW1.SetDecimalNum(2);
	m_EditPulseKW1.SetValueRange(99999.99,0);
	m_EditPulseKW1.SetIsShowNegative(FALSE);
	m_EditPulseKW1.SetUDFun(-1);
	m_EditPulseKW1.SetLRFun(0);
	m_EditPulseKW1.SetAddSubFun(3);
	m_EditPulseKW1.SetHaveUpDownFun(FALSE);
	m_EditPulseKW1.SetHaveLeftRightFun(FALSE);
	m_EditPulseKW1.SetHaveEnterFun(FALSE);
	m_EditPulseKW1.SetHaveAddSubFun(FALSE);
	m_EditPulseKW2.SetPosition(135,116,215,136,0);
	m_EditPulseKW2.SetTabOrder(4);
	m_pObjList[4] = &m_EditPulseKW2;
	m_EditPulseKW2.SetFont(16);
	m_EditPulseKW2.SetOwner(this);
	m_EditPulseKW2.SetCaption(tr("12345.67"));
	m_EditPulseKW2.SetMaxLen(8);
	m_EditPulseKW2.SetDecimalNum(2);
	m_EditPulseKW2.SetValueRange(99999.99,0);
	m_EditPulseKW2.SetIsShowNegative(FALSE);
	m_EditPulseKW2.SetUDFun(-1);
	m_EditPulseKW2.SetLRFun(0);
	m_EditPulseKW2.SetAddSubFun(3);
	m_EditPulseKW2.SetHaveUpDownFun(FALSE);
	m_EditPulseKW2.SetHaveLeftRightFun(FALSE);
	m_EditPulseKW2.SetHaveEnterFun(FALSE);
	m_EditPulseKW2.SetHaveAddSubFun(FALSE);
	m_StaticStaticIntKW1.SetPosition(13,33,123,53,0);
	m_StaticStaticIntKW1.SetTabOrder(-5);
	m_pObjList[5] = &m_StaticStaticIntKW1;
	m_StaticStaticIntKW1.SetFont(16);
	m_StaticStaticIntKW1.SetOwner(this);
	m_StaticStaticIntKW1.SetCaption(tr("积分电度1="));
	m_StaticStaticIntKW1.SetDataSource(-1,0);
	m_StaticStaticIntKW1.SetTimer(0);
	m_StaticStaticIntKW2.SetPosition(13,60,123,80,0);
	m_StaticStaticIntKW2.SetTabOrder(-6);
	m_pObjList[6] = &m_StaticStaticIntKW2;
	m_StaticStaticIntKW2.SetFont(16);
	m_StaticStaticIntKW2.SetOwner(this);
	m_StaticStaticIntKW2.SetCaption(tr("积分电度2="));
	m_StaticStaticIntKW2.SetDataSource(-1,0);
	m_StaticStaticIntKW2.SetTimer(0);
	m_StaticStaticPulseKW1.SetPosition(13,88,123,108,0);
	m_StaticStaticPulseKW1.SetTabOrder(-7);
	m_pObjList[7] = &m_StaticStaticPulseKW1;
	m_StaticStaticPulseKW1.SetFont(16);
	m_StaticStaticPulseKW1.SetOwner(this);
	m_StaticStaticPulseKW1.SetCaption(tr("脉冲电度1="));
	m_StaticStaticPulseKW1.SetDataSource(-1,0);
	m_StaticStaticPulseKW1.SetTimer(0);
	m_StaticStaticPulseKW2.SetPosition(13,116,123,136,0);
	m_StaticStaticPulseKW2.SetTabOrder(-8);
	m_pObjList[8] = &m_StaticStaticPulseKW2;
	m_StaticStaticPulseKW2.SetFont(16);
	m_StaticStaticPulseKW2.SetOwner(this);
	m_StaticStaticPulseKW2.SetCaption(tr("脉冲电度2="));
	m_StaticStaticPulseKW2.SetDataSource(-1,0);
	m_StaticStaticPulseKW2.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(8);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndCiBase,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndCiBase)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口PulseConst: 脉冲常数*******************
CLcdWndPulseConst::~CLcdWndPulseConst()
{
}
CLcdWndPulseConst::CLcdWndPulseConst()
{
}
BOOL CLcdWndPulseConst::Init()
{
	SetClassName("CLcdWndPulseConst");
	SetCaption(tr("脉冲常数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndCIPara));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndCIPara));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_ListPulseConst.SetPosition(8,28,233,148,0);
	m_ListPulseConst.SetTabOrder(1);
	m_pObjList[1] = &m_ListPulseConst;
	m_ListPulseConst.SetFont(16);
	m_ListPulseConst.SetOwner(this);
	m_ListPulseConst.SetType(CIConst);
	m_ListPulseConst.SetOwner(this);
	m_ListPulseConst.SetTimer(0);
	m_ListPulseConst.SetUDFun(0);
	m_ListPulseConst.SetLRFun(1);
	m_ListPulseConst.SetHaveUpDownFun(FALSE);
	m_ListPulseConst.SetHaveLeftRightFun(FALSE);
	m_ListPulseConst.SetHaveEnterFun(TRUE);
	m_ListPulseConst.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndPulseConst,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndPulseConst)
	ON_HMILIST_ENTER(1, OnEnterListPulseConst)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口CF: 手动整定系数*******************
CLcdWndCF::~CLcdWndCF()
{
}
CLcdWndCF::CLcdWndCF()
{
}
BOOL CLcdWndCF::Init()
{
	SetClassName("CLcdWndCF");
	SetCaption(tr("手动整定系数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndfactorySet));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_ListCFList.SetPosition(10,31,225,156,0);
	m_ListCFList.SetTabOrder(1);
	m_pObjList[1] = &m_ListCFList;
	m_ListCFList.SetFont(16);
	m_ListCFList.SetOwner(this);
	m_ListCFList.SetType(CF);
	m_ListCFList.SetOwner(this);
	m_ListCFList.SetTimer(0);
	m_ListCFList.SetUDFun(0);
	m_ListCFList.SetLRFun(1);
	m_ListCFList.SetHaveUpDownFun(FALSE);
	m_ListCFList.SetHaveLeftRightFun(FALSE);
	m_ListCFList.SetHaveEnterFun(FALSE);
	m_ListCFList.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndCF,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndCF)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口Test: 测试*******************
CLcdWndTest::~CLcdWndTest()
{
}
CLcdWndTest::CLcdWndTest()
{
}
BOOL CLcdWndTest::Init()
{
	SetClassName("CLcdWndTest");
	SetCaption(tr("测试"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnOutputTest.SetPosition(17,26,217,51,0);
	m_BtnOutputTest.SetTabOrder(1);
	m_pObjList[1] = &m_BtnOutputTest;
	m_BtnOutputTest.SetFont(0);
	m_BtnOutputTest.SetOwner(this);
	m_BtnOutputTest.SetCaption(tr("开出传动"));
	m_BtnOutputTest.SetHaveEnterFun(FALSE);
	m_BtnOutputTest.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndOutputTest));
	m_BtnLenTest.SetPosition(17,58,217,83,0);
	m_BtnLenTest.SetTabOrder(2);
	m_pObjList[2] = &m_BtnLenTest;
	m_BtnLenTest.SetFont(0);
	m_BtnLenTest.SetOwner(this);
	m_BtnLenTest.SetCaption(tr("测试指示灯"));
	m_BtnLenTest.SetHaveEnterFun(FALSE);
	m_BtnLenTest.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndLanTest));
	m_BtnReportTest.SetPosition(17,89,217,114,0);
	m_BtnReportTest.SetTabOrder(3);
	m_pObjList[3] = &m_BtnReportTest;
	m_BtnReportTest.SetFont(0);
	m_BtnReportTest.SetOwner(this);
	m_BtnReportTest.SetCaption(tr("模拟报告"));
	m_BtnReportTest.SetHaveEnterFun(FALSE);
	m_BtnReportTest.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndReportTest));
	m_BtnManualREC.SetPosition(17,119,217,144,0);
	m_BtnManualREC.SetTabOrder(4);
	m_pObjList[4] = &m_BtnManualREC;
	m_BtnManualREC.SetFont(16);
	m_BtnManualREC.SetOwner(this);
	m_BtnManualREC.SetCaption(tr("手动录波测试"));
	m_BtnManualREC.SetHaveEnterFun(FALSE);
	m_BtnManualREC.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndManualREC));
	m_BtnfactorySet.SetPosition(17,31,217,56,1);
	m_BtnfactorySet.SetTabOrder(5);
	m_pObjList[5] = &m_BtnfactorySet;
	m_BtnfactorySet.SetFont(16);
	m_BtnfactorySet.SetOwner(this);
	m_BtnfactorySet.SetCaption(tr("出厂设置"));
	m_BtnfactorySet.SetHaveEnterFun(TRUE);
	m_BtnfactorySet.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(2);
	SetObjNum(5);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndTest,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndTest)
	ON_HMIBTN_ENTER(5, OnEnterBtnfactorySet)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口factorySet: 出厂设置*******************
CLcdWndfactorySet::~CLcdWndfactorySet()
{
}
CLcdWndfactorySet::CLcdWndfactorySet()
{
}
BOOL CLcdWndfactorySet::Init()
{
	SetClassName("CLcdWndfactorySet");
	SetCaption(tr("出厂设置"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnManualCoef.SetPosition(21,28,221,53,0);
	m_BtnManualCoef.SetTabOrder(1);
	m_pObjList[1] = &m_BtnManualCoef;
	m_BtnManualCoef.SetFont(16);
	m_BtnManualCoef.SetOwner(this);
	m_BtnManualCoef.SetCaption(tr("手动整定系数"));
	m_BtnManualCoef.SetHaveEnterFun(TRUE);
	m_BtnManualCoef.SetEnterWnd(NULL);
	m_BtnAutoCoef.SetPosition(21,61,221,86,0);
	m_BtnAutoCoef.SetTabOrder(2);
	m_pObjList[2] = &m_BtnAutoCoef;
	m_BtnAutoCoef.SetFont(16);
	m_BtnAutoCoef.SetOwner(this);
	m_BtnAutoCoef.SetCaption(tr("自动整定系数"));
	m_BtnAutoCoef.SetHaveEnterFun(TRUE);
	m_BtnAutoCoef.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndfactorySet,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndfactorySet)
	ON_HMIBTN_ENTER(1, OnEnterBtnManualCoef)
	ON_HMIBTN_ENTER(2, OnEnterBtnAutoCoef)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口ManualREC: 手动录波测试*******************
CLcdWndManualREC::~CLcdWndManualREC()
{
}
CLcdWndManualREC::CLcdWndManualREC()
{
}
BOOL CLcdWndManualREC::Init()
{
	SetClassName("CLcdWndManualREC");
	SetCaption(tr("手动录波测试"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_Staticstatic1.SetPosition(8,43,223,68,0);
	m_Staticstatic1.SetTabOrder(-1);
	m_pObjList[1] = &m_Staticstatic1;
	m_Staticstatic1.SetFont(16);
	m_Staticstatic1.SetOwner(this);
	m_Staticstatic1.SetCaption(tr("按下Enter键手动触发一次录波"));
	m_Staticstatic1.SetDataSource(-1,0);
	m_Staticstatic1.SetTimer(0);
	m_Staticstatic2.SetPosition(8,65,223,90,0);
	m_Staticstatic2.SetTabOrder(-2);
	m_pObjList[2] = &m_Staticstatic2;
	m_Staticstatic2.SetFont(16);
	m_Staticstatic2.SetOwner(this);
	m_Staticstatic2.SetCaption(tr("由于录波无返回信息,请不要反复"));
	m_Staticstatic2.SetDataSource(-1,0);
	m_Staticstatic2.SetTimer(0);
	m_Staticstatic3.SetPosition(8,88,223,113,0);
	m_Staticstatic3.SetTabOrder(-3);
	m_pObjList[3] = &m_Staticstatic3;
	m_Staticstatic3.SetFont(16);
	m_Staticstatic3.SetOwner(this);
	m_Staticstatic3.SetCaption(tr("按下Enter键"));
	m_Staticstatic3.SetDataSource(-1,0);
	m_Staticstatic3.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(3);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndManualREC,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndManualREC)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口OutputTest: 开出传动*******************
CLcdWndOutputTest::~CLcdWndOutputTest()
{
}
CLcdWndOutputTest::CLcdWndOutputTest()
{
}
BOOL CLcdWndOutputTest::Init()
{
	SetClassName("CLcdWndOutputTest");
	SetCaption(tr("开出传动"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(-1);
	SetLROffset(1);
	m_EditOutputNum.SetPosition(153,33,188,53,0);
	m_EditOutputNum.SetTabOrder(1);
	m_pObjList[1] = &m_EditOutputNum;
	m_EditOutputNum.SetFont(16);
	m_EditOutputNum.SetOwner(this);
	m_EditOutputNum.SetCaption(tr("01"));
	m_EditOutputNum.SetMaxLen(2);
	m_EditOutputNum.SetDecimalNum(0);
	m_EditOutputNum.SetValueRange(11,1);
	m_EditOutputNum.SetIsShowNegative(FALSE);
	m_EditOutputNum.SetUDFun(-1);
	m_EditOutputNum.SetLRFun(0);
	m_EditOutputNum.SetAddSubFun(3);
	m_EditOutputNum.SetHaveUpDownFun(FALSE);
	m_EditOutputNum.SetHaveLeftRightFun(FALSE);
	m_EditOutputNum.SetHaveEnterFun(TRUE);
	m_EditOutputNum.SetHaveAddSubFun(TRUE);
	m_EditOutputTime.SetPosition(153,60,188,80,0);
	m_EditOutputTime.SetTabOrder(2);
	m_pObjList[2] = &m_EditOutputTime;
	m_EditOutputTime.SetFont(16);
	m_EditOutputTime.SetOwner(this);
	m_EditOutputTime.SetCaption(tr("1000"));
	m_EditOutputTime.SetMaxLen(4);
	m_EditOutputTime.SetDecimalNum(0);
	m_EditOutputTime.SetValueRange(9999,0);
	m_EditOutputTime.SetIsShowNegative(FALSE);
	m_EditOutputTime.SetUDFun(-1);
	m_EditOutputTime.SetLRFun(0);
	m_EditOutputTime.SetAddSubFun(3);
	m_EditOutputTime.SetHaveUpDownFun(FALSE);
	m_EditOutputTime.SetHaveLeftRightFun(FALSE);
	m_EditOutputTime.SetHaveEnterFun(FALSE);
	m_EditOutputTime.SetHaveAddSubFun(FALSE);
	m_StaticOutputName.SetPosition(16,97,186,112,0);
	m_StaticOutputName.SetTabOrder(-3);
	m_pObjList[3] = &m_StaticOutputName;
	m_StaticOutputName.SetFont(16);
	m_StaticOutputName.SetOwner(this);
	m_StaticOutputName.SetCaption(tr("传动名称"));
	m_StaticOutputName.SetDataSource(-1,0);
	m_StaticOutputName.SetTimer(0);
	m_Static4771.SetPosition(16,34,141,54,0);
	m_Static4771.SetTabOrder(-4);
	m_pObjList[4] = &m_Static4771;
	m_Static4771.SetFont(0);
	m_Static4771.SetOwner(this);
	m_Static4771.SetCaption(tr("输入传动路号"));
	m_Static4771.SetDataSource(-1,0);
	m_Static4771.SetTimer(-1);
	m_Static4773.SetPosition(16,62,141,82,0);
	m_Static4773.SetTabOrder(-7);
	m_pObjList[7] = &m_Static4773;
	m_Static4773.SetFont(0);
	m_Static4773.SetOwner(this);
	m_Static4773.SetCaption(tr("输入传动时间"));
	m_Static4773.SetDataSource(-1,0);
	m_Static4773.SetTimer(-1);
	m_Static4775.SetPosition(193,60,218,80,0);
	m_Static4775.SetTabOrder(-5);
	m_pObjList[5] = &m_Static4775;
	m_Static4775.SetFont(16);
	m_Static4775.SetOwner(this);
	m_Static4775.SetCaption(tr("ms"));
	m_Static4775.SetDataSource(-1,0);
	m_Static4775.SetTimer(-1);
	m_Static4776.SetPosition(16,128,186,143,0);
	m_Static4776.SetTabOrder(-6);
	m_pObjList[6] = &m_Static4776;
	m_Static4776.SetFont(16);
	m_Static4776.SetOwner(this);
	m_Static4776.SetCaption(tr("按Enter键执行传动"));
	m_Static4776.SetDataSource(-1,0);
	m_Static4776.SetTimer(-1);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(7);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndOutputTest,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndOutputTest)
	ON_HMIEDIT_ENTER(1, OnEnterEditOutputNum)
	ON_HMIEDIT_ADDSUB(1, OnAddSubEditOutputNum)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口LanTest: 指示灯测试*******************
CLcdWndLanTest::~CLcdWndLanTest()
{
}
CLcdWndLanTest::CLcdWndLanTest()
{
}
BOOL CLcdWndLanTest::Init()
{
	SetClassName("CLcdWndLanTest");
	SetCaption(tr("指示灯测试"));
	SetEscWnd(NULL);
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnStart.SetPosition(32,114,92,134,0);
	m_BtnStart.SetTabOrder(1);
	m_pObjList[1] = &m_BtnStart;
	m_BtnStart.SetFont(16);
	m_BtnStart.SetOwner(this);
	m_BtnStart.SetCaption(tr("开始"));
	m_BtnStart.SetHaveEnterFun(TRUE);
	m_BtnStart.SetEnterWnd(NULL);
	m_BtnCancel.SetPosition(136,114,196,134,0);
	m_BtnCancel.SetTabOrder(2);
	m_pObjList[2] = &m_BtnCancel;
	m_BtnCancel.SetFont(16);
	m_BtnCancel.SetOwner(this);
	m_BtnCancel.SetCaption(tr("取消"));
	m_BtnCancel.SetHaveEnterFun(FALSE);
	m_BtnCancel.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
	m_Static4779.SetPosition(18,44,203,64,0);
	m_Static4779.SetTabOrder(-3);
	m_pObjList[3] = &m_Static4779;
	m_Static4779.SetFont(16);
	m_Static4779.SetOwner(this);
	m_Static4779.SetCaption(tr("按Enter键进入指示灯测试，"));
	m_Static4779.SetDataSource(-1,0);
	m_Static4779.SetTimer(-1);
	m_Static4780.SetPosition(18,66,203,86,0);
	m_Static4780.SetTabOrder(-4);
	m_pObjList[4] = &m_Static4780;
	m_Static4780.SetFont(16);
	m_Static4780.SetOwner(this);
	m_Static4780.SetCaption(tr("请查看指示灯...."));
	m_Static4780.SetDataSource(-1,0);
	m_Static4780.SetTimer(-1);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndLanTest,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndLanTest)
	ON_HMIBTN_ENTER(1, OnEnterBtnStart)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口ReportTest: 模拟报告*******************
CLcdWndReportTest::~CLcdWndReportTest()
{
}
CLcdWndReportTest::CLcdWndReportTest()
{
}
BOOL CLcdWndReportTest::Init()
{
	SetClassName("CLcdWndReportTest");
	SetCaption(tr("模拟报告"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndTest));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_Static4784.SetPosition(15,53,210,83,0);
	m_Static4784.SetTabOrder(-1);
	m_pObjList[1] = &m_Static4784;
	m_Static4784.SetFont(16);
	m_Static4784.SetOwner(this);
	m_Static4784.SetCaption(tr("按Enter键产生一条模拟报告"));
	m_Static4784.SetDataSource(-1,0);
	m_Static4784.SetTimer(-1);
	m_Static4785.SetPosition(15,74,210,104,0);
	m_Static4785.SetTabOrder(-2);
	m_pObjList[2] = &m_Static4785;
	m_Static4785.SetFont(16);
	m_Static4785.SetOwner(this);
	m_Static4785.SetCaption(tr("仅用于通讯测试！"));
	m_Static4785.SetDataSource(-1,0);
	m_Static4785.SetTimer(-1);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndReportTest,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndReportTest)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口Address: 通信设置*******************
CLcdWndAddress::~CLcdWndAddress()
{
}
CLcdWndAddress::CLcdWndAddress()
{
}
BOOL CLcdWndAddress::Init()
{
	SetClassName("CLcdWndAddress");
	SetCaption(tr("通信设置"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnCAN.SetPosition(24,28,214,53,0);
	m_BtnCAN.SetTabOrder(1);
	m_pObjList[1] = &m_BtnCAN;
	m_BtnCAN.SetFont(16);
	m_BtnCAN.SetOwner(this);
	m_BtnCAN.SetCaption(tr("设备地址"));
	m_BtnCAN.SetHaveEnterFun(FALSE);
	m_BtnCAN.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndCANAddress));
	m_BtnETH.SetPosition(24,61,214,86,0);
	m_BtnETH.SetTabOrder(2);
	m_pObjList[2] = &m_BtnETH;
	m_BtnETH.SetFont(16);
	m_BtnETH.SetOwner(this);
	m_BtnETH.SetCaption(tr("以太网地址"));
	m_BtnETH.SetHaveEnterFun(FALSE);
	m_BtnETH.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndEthadd));
	m_BtnPortStatus.SetPosition(24,94,214,119,0);
	m_BtnPortStatus.SetTabOrder(3);
	m_pObjList[3] = &m_BtnPortStatus;
	m_BtnPortStatus.SetFont(16);
	m_BtnPortStatus.SetOwner(this);
	m_BtnPortStatus.SetCaption(tr("端口参数"));
	m_BtnPortStatus.SetHaveEnterFun(FALSE);
	m_BtnPortStatus.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndPortStatus));
	m_BtnSerialSet.SetPosition(24,127,214,152,0);
	m_BtnSerialSet.SetTabOrder(4);
	m_pObjList[4] = &m_BtnSerialSet;
	m_BtnSerialSet.SetFont(16);
	m_BtnSerialSet.SetOwner(this);
	m_BtnSerialSet.SetCaption(tr("串口设置"));
	m_BtnSerialSet.SetHaveEnterFun(TRUE);
	m_BtnSerialSet.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndAddress,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndAddress)
	ON_HMIBTN_ENTER(4, OnEnterBtnSerialSet)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口Ethadd: 以太网地址*******************
CLcdWndEthadd::~CLcdWndEthadd()
{
}
CLcdWndEthadd::CLcdWndEthadd()
{
}
BOOL CLcdWndEthadd::Init()
{
	SetClassName("CLcdWndEthadd");
	SetCaption(tr("以太网地址"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_Btnetha.SetPosition(31,36,196,61,0);
	m_Btnetha.SetTabOrder(1);
	m_pObjList[1] = &m_Btnetha;
	m_Btnetha.SetFont(16);
	m_Btnetha.SetOwner(this);
	m_Btnetha.SetCaption(tr("以太网A"));
	m_Btnetha.SetHaveEnterFun(TRUE);
	m_Btnetha.SetEnterWnd(NULL);
	m_Btnethb.SetPosition(31,69,196,94,0);
	m_Btnethb.SetTabOrder(2);
	m_pObjList[2] = &m_Btnethb;
	m_Btnethb.SetFont(16);
	m_Btnethb.SetOwner(this);
	m_Btnethb.SetCaption(tr("以太网B"));
	m_Btnethb.SetHaveEnterFun(TRUE);
	m_Btnethb.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndEthadd,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndEthadd)
	ON_HMIBTN_ENTER(1, OnEnterBtnetha)
	ON_HMIBTN_ENTER(2, OnEnterBtnethb)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口CANAddress: 设备地址*******************
CLcdWndCANAddress::~CLcdWndCANAddress()
{
}
CLcdWndCANAddress::CLcdWndCANAddress()
{
}
BOOL CLcdWndCANAddress::Init()
{
	SetClassName("CLcdWndCANAddress");
	SetCaption(tr("设备地址"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_Btnenter.SetPosition(39,113,94,138,0);
	m_Btnenter.SetTabOrder(2);
	m_pObjList[2] = &m_Btnenter;
	m_Btnenter.SetFont(16);
	m_Btnenter.SetOwner(this);
	m_Btnenter.SetCaption(tr("确定"));
	m_Btnenter.SetHaveEnterFun(TRUE);
	m_Btnenter.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_BtnESC.SetPosition(128,113,183,138,0);
	m_BtnESC.SetTabOrder(3);
	m_pObjList[3] = &m_BtnESC;
	m_BtnESC.SetFont(16);
	m_BtnESC.SetOwner(this);
	m_BtnESC.SetCaption(tr("取消"));
	m_BtnESC.SetHaveEnterFun(FALSE);
	m_BtnESC.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_EditCANA.SetPosition(91,60,131,85,0);
	m_EditCANA.SetTabOrder(1);
	m_pObjList[1] = &m_EditCANA;
	m_EditCANA.SetFont(16);
	m_EditCANA.SetOwner(this);
	m_EditCANA.SetCaption(tr("10"));
	m_EditCANA.SetMaxLen(3);
	m_EditCANA.SetDecimalNum(0);
	m_EditCANA.SetValueRange(255,1);
	m_EditCANA.SetIsShowNegative(FALSE);
	m_EditCANA.SetUDFun(-1);
	m_EditCANA.SetLRFun(0);
	m_EditCANA.SetAddSubFun(3);
	m_EditCANA.SetHaveUpDownFun(FALSE);
	m_EditCANA.SetHaveLeftRightFun(FALSE);
	m_EditCANA.SetHaveEnterFun(FALSE);
	m_EditCANA.SetHaveAddSubFun(FALSE);
	m_Static5417.SetPosition(18,28,198,48,0);
	m_Static5417.SetTabOrder(-4);
	m_pObjList[4] = &m_Static5417;
	m_Static5417.SetFont(16);
	m_Static5417.SetOwner(this);
	m_Static5417.SetCaption(tr("设备地址:"));
	m_Static5417.SetDataSource(-1,0);
	m_Static5417.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndCANAddress,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndCANAddress)
	ON_HMIBTN_ENTER(2, OnEnterBtnenter)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口PassChange: 密码设置--菜单*******************
CLcdWndPassChange::~CLcdWndPassChange()
{
}
CLcdWndPassChange::CLcdWndPassChange()
{
}
BOOL CLcdWndPassChange::Init()
{
	SetClassName("CLcdWndPassChange");
	SetCaption(tr("密码设置--菜单"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnChgCommonPass.SetPosition(19,37,219,62,0);
	m_BtnChgCommonPass.SetTabOrder(1);
	m_pObjList[1] = &m_BtnChgCommonPass;
	m_BtnChgCommonPass.SetFont(16);
	m_BtnChgCommonPass.SetOwner(this);
	m_BtnChgCommonPass.SetCaption(tr("普通密码"));
	m_BtnChgCommonPass.SetHaveEnterFun(TRUE);
	m_BtnChgCommonPass.SetEnterWnd(NULL);
	m_BtnChgSettingPass.SetPosition(19,69,219,94,0);
	m_BtnChgSettingPass.SetTabOrder(2);
	m_pObjList[2] = &m_BtnChgSettingPass;
	m_BtnChgSettingPass.SetFont(16);
	m_BtnChgSettingPass.SetOwner(this);
	m_BtnChgSettingPass.SetCaption(tr("管理员密码"));
	m_BtnChgSettingPass.SetHaveEnterFun(TRUE);
	m_BtnChgSettingPass.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndPassChange,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndPassChange)
	ON_HMIBTN_ENTER(1, OnEnterBtnChgCommonPass)
	ON_HMIBTN_ENTER(2, OnEnterBtnChgSettingPass)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingChgAreaNumInput: 定值区号*******************
CLcdWndSettingChgAreaNumInput::~CLcdWndSettingChgAreaNumInput()
{
}
CLcdWndSettingChgAreaNumInput::CLcdWndSettingChgAreaNumInput()
{
}
BOOL CLcdWndSettingChgAreaNumInput::Init()
{
	SetClassName("CLcdWndSettingChgAreaNumInput");
	SetCaption(tr("定值区号"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndconfigModify));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditInputAreaNum.SetPosition(180,76,215,96,0);
	m_EditInputAreaNum.SetTabOrder(1);
	m_pObjList[1] = &m_EditInputAreaNum;
	m_EditInputAreaNum.SetFont(16);
	m_EditInputAreaNum.SetOwner(this);
	m_EditInputAreaNum.SetCaption(tr("00"));
	m_EditInputAreaNum.SetMaxLen(2);
	m_EditInputAreaNum.SetDecimalNum(0);
	m_EditInputAreaNum.SetValueRange(15,0);
	m_EditInputAreaNum.SetIsShowNegative(FALSE);
	m_EditInputAreaNum.SetUDFun(-1);
	m_EditInputAreaNum.SetLRFun(0);
	m_EditInputAreaNum.SetAddSubFun(3);
	m_EditInputAreaNum.SetHaveUpDownFun(FALSE);
	m_EditInputAreaNum.SetHaveLeftRightFun(FALSE);
	m_EditInputAreaNum.SetHaveEnterFun(FALSE);
	m_EditInputAreaNum.SetHaveAddSubFun(FALSE);
	m_Static4899.SetPosition(12,76,167,96,0);
	m_Static4899.SetTabOrder(-4);
	m_pObjList[4] = &m_Static4899;
	m_Static4899.SetFont(16);
	m_Static4899.SetOwner(this);
	m_Static4899.SetCaption(tr("要修改的定值区号"));
	m_Static4899.SetDataSource(-1,0);
	m_Static4899.SetTimer(0);
	m_Static4917.SetPosition(12,50,167,70,0);
	m_Static4917.SetTabOrder(-3);
	m_pObjList[3] = &m_Static4917;
	m_Static4917.SetFont(16);
	m_Static4917.SetOwner(this);
	m_Static4917.SetCaption(tr("当前运行定值区号"));
	m_Static4917.SetDataSource(-1,0);
	m_Static4917.SetTimer(0);
	m_StaticNowAreaNum.SetPosition(180,50,215,70,0);
	m_StaticNowAreaNum.SetTabOrder(-2);
	m_pObjList[2] = &m_StaticNowAreaNum;
	m_StaticNowAreaNum.SetFont(16);
	m_StaticNowAreaNum.SetOwner(this);
	m_StaticNowAreaNum.SetCaption(tr("0"));
	m_StaticNowAreaNum.SetDataSource(-1,0);
	m_StaticNowAreaNum.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingChgAreaNumInput,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingChgAreaNumInput)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口syspara: 其他参数*******************
CLcdWndsyspara::~CLcdWndsyspara()
{
}
CLcdWndsyspara::CLcdWndsyspara()
{
}
BOOL CLcdWndsyspara::Init()
{
	SetClassName("CLcdWndsyspara");
	SetCaption(tr("其他参数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSysSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnContrast.SetPosition(14,31,219,56,0);
	m_BtnContrast.SetTabOrder(1);
	m_pObjList[1] = &m_BtnContrast;
	m_BtnContrast.SetFont(16);
	m_BtnContrast.SetOwner(this);
	m_BtnContrast.SetCaption(tr("液晶对比度调整"));
	m_BtnContrast.SetHaveEnterFun(TRUE);
	m_BtnContrast.SetEnterWnd(NULL);
	m_BtnFVDH.SetPosition(14,62,219,87,0);
	m_BtnFVDH.SetTabOrder(2);
	m_pObjList[2] = &m_BtnFVDH;
	m_BtnFVDH.SetFont(16);
	m_BtnFVDH.SetOwner(this);
	m_BtnFVDH.SetCaption(tr("故障电量显示方式"));
	m_BtnFVDH.SetHaveEnterFun(TRUE);
	m_BtnFVDH.SetEnterWnd(NULL);
	m_BtnIRGBType.SetPosition(14,93,219,118,0);
	m_BtnIRGBType.SetTabOrder(3);
	m_pObjList[3] = &m_BtnIRGBType;
	m_BtnIRGBType.SetFont(16);
	m_BtnIRGBType.SetOwner(this);
	m_BtnIRGBType.SetCaption(tr("GPS格式设置"));
	m_BtnIRGBType.SetHaveEnterFun(TRUE);
	m_BtnIRGBType.SetEnterWnd(NULL);
	m_BtnDefaultPara.SetPosition(14,127,219,152,0);
	m_BtnDefaultPara.SetTabOrder(4);
	m_pObjList[4] = &m_BtnDefaultPara;
	m_BtnDefaultPara.SetFont(16);
	m_BtnDefaultPara.SetOwner(this);
	m_BtnDefaultPara.SetCaption(tr("恢复出厂设置"));
	m_BtnDefaultPara.SetHaveEnterFun(TRUE);
	m_BtnDefaultPara.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(4);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndsyspara,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndsyspara)
	ON_HMIBTN_ENTER(1, OnEnterBtnContrast)
	ON_HMIBTN_ENTER(2, OnEnterBtnFVDH)
	ON_HMIBTN_ENTER(3, OnEnterBtnIRGBType)
	ON_HMIBTN_ENTER(4, OnEnterBtnDefaultPara)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口SettingSwitchConfirm: 定值切换确认*******************
CLcdWndSettingSwitchConfirm::~CLcdWndSettingSwitchConfirm()
{
}
CLcdWndSettingSwitchConfirm::CLcdWndSettingSwitchConfirm()
{
}
BOOL CLcdWndSettingSwitchConfirm::Init()
{
	SetClassName("CLcdWndSettingSwitchConfirm");
	SetCaption(tr("定值切换确认"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnOk.SetPosition(34,118,99,143,0);
	m_BtnOk.SetTabOrder(1);
	m_pObjList[1] = &m_BtnOk;
	m_BtnOk.SetFont(16);
	m_BtnOk.SetOwner(this);
	m_BtnOk.SetCaption(tr("确认"));
	m_BtnOk.SetHaveEnterFun(TRUE);
	m_BtnOk.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	m_BtnCancel.SetPosition(131,118,196,143,0);
	m_BtnCancel.SetTabOrder(2);
	m_pObjList[2] = &m_BtnCancel;
	m_BtnCancel.SetFont(16);
	m_BtnCancel.SetOwner(this);
	m_BtnCancel.SetCaption(tr("取消"));
	m_BtnCancel.SetHaveEnterFun(FALSE);
	m_BtnCancel.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndSetting));
	m_Static4920.SetPosition(15,44,145,69,0);
	m_Static4920.SetTabOrder(-6);
	m_pObjList[6] = &m_Static4920;
	m_Static4920.SetFont(16);
	m_Static4920.SetOwner(this);
	m_Static4920.SetCaption(tr("当前定值区号"));
	m_Static4920.SetDataSource(-1,0);
	m_Static4920.SetTimer(0);
	m_StaticNowAreaNo.SetPosition(167,44,212,59,0);
	m_StaticNowAreaNo.SetTabOrder(-5);
	m_pObjList[5] = &m_StaticNowAreaNo;
	m_StaticNowAreaNo.SetFont(16);
	m_StaticNowAreaNo.SetOwner(this);
	m_StaticNowAreaNo.SetCaption(tr("0"));
	m_StaticNowAreaNo.SetDataSource(-1,0);
	m_StaticNowAreaNo.SetTimer(0);
	m_StaticNewAreaNo.SetPosition(167,75,212,90,0);
	m_StaticNewAreaNo.SetTabOrder(-4);
	m_pObjList[4] = &m_StaticNewAreaNo;
	m_StaticNewAreaNo.SetFont(16);
	m_StaticNewAreaNo.SetOwner(this);
	m_StaticNewAreaNo.SetCaption(tr("0"));
	m_StaticNewAreaNo.SetDataSource(-1,0);
	m_StaticNewAreaNo.SetTimer(0);
	m_Static44444.SetPosition(15,75,145,100,0);
	m_Static44444.SetTabOrder(-3);
	m_pObjList[3] = &m_Static44444;
	m_Static44444.SetFont(16);
	m_Static44444.SetOwner(this);
	m_Static44444.SetCaption(tr("投入定值区号"));
	m_Static44444.SetDataSource(-1,0);
	m_Static44444.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(6);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndSettingSwitchConfirm,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndSettingSwitchConfirm)
	ON_HMIBTN_ENTER(1, OnEnterBtnOk)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口PortStatus: 端口参数*******************
CLcdWndPortStatus::~CLcdWndPortStatus()
{
}
CLcdWndPortStatus::CLcdWndPortStatus()
{
}
BOOL CLcdWndPortStatus::Init()
{
	SetClassName("CLcdWndPortStatus");
	SetCaption(tr("端口参数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnPort.SetPosition(43,28,198,53,0);
	m_BtnPort.SetTabOrder(1);
	m_pObjList[1] = &m_BtnPort;
	m_BtnPort.SetFont(16);
	m_BtnPort.SetOwner(this);
	m_BtnPort.SetCaption(tr("通信端口"));
	m_BtnPort.SetHaveEnterFun(TRUE);
	m_BtnPort.SetEnterWnd(NULL);
	m_BtnProtocol.SetPosition(43,59,198,84,0);
	m_BtnProtocol.SetTabOrder(2);
	m_pObjList[2] = &m_BtnProtocol;
	m_BtnProtocol.SetFont(16);
	m_BtnProtocol.SetOwner(this);
	m_BtnProtocol.SetCaption(tr("通信规约"));
	m_BtnProtocol.SetHaveEnterFun(TRUE);
	m_BtnProtocol.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndPortStatus,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndPortStatus)
	ON_HMIBTN_ENTER(1, OnEnterBtnPort)
	ON_HMIBTN_ENTER(2, OnEnterBtnProtocol)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口WStatus: 状态*******************
CLcdWndWStatus::~CLcdWndWStatus()
{
}
CLcdWndWStatus::CLcdWndWStatus()
{
}
BOOL CLcdWndWStatus::Init()
{
	SetClassName("CLcdWndWStatus");
	SetCaption(tr("状态"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnDevStatus.SetPosition(17,31,217,56,0);
	m_BtnDevStatus.SetTabOrder(1);
	m_pObjList[1] = &m_BtnDevStatus;
	m_BtnDevStatus.SetFont(16);
	m_BtnDevStatus.SetOwner(this);
	m_BtnDevStatus.SetCaption(tr("装置状态"));
	m_BtnDevStatus.SetHaveEnterFun(TRUE);
	m_BtnDevStatus.SetEnterWnd(NULL);
	m_BtnComStatus.SetPosition(17,64,217,89,0);
	m_BtnComStatus.SetTabOrder(2);
	m_pObjList[2] = &m_BtnComStatus;
	m_BtnComStatus.SetFont(16);
	m_BtnComStatus.SetOwner(this);
	m_BtnComStatus.SetCaption(tr("通讯状态"));
	m_BtnComStatus.SetHaveEnterFun(TRUE);
	m_BtnComStatus.SetEnterWnd(NULL);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndWStatus,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndWStatus)
	ON_HMIBTN_ENTER(1, OnEnterBtnDevStatus)
	ON_HMIBTN_ENTER(2, OnEnterBtnComStatus)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

CLcdWndMainMenu 	CLcdApp::m_WndMainMenu;
CLcdWndMeasure 	CLcdApp::m_WndMeasure;
CLcdWndASModify 	CLcdApp::m_WndASModify;
CLcdWndASQuery 	CLcdApp::m_WndASQuery;
CLcdWndReport 	CLcdApp::m_WndReport;
CLcdWndActionReport 	CLcdApp::m_WndActionReport;
CLcdWndAlarmReport 	CLcdApp::m_WndAlarmReport;
CLcdWndStartReport 	CLcdApp::m_WndStartReport;
CLcdWndOperationReport 	CLcdApp::m_WndOperationReport;
CLcdWndSettingQueryAreaNumInput 	CLcdApp::m_WndSettingQueryAreaNumInput;
CLcdWndSoeReport 	CLcdApp::m_WndSoeReport;
CLcdWndSetting 	CLcdApp::m_WndSetting;
CLcdWndconfigQury 	CLcdApp::m_WndconfigQury;
CLcdWndSettingQuery 	CLcdApp::m_WndSettingQuery;
CLcdWndconfigModify 	CLcdApp::m_WndconfigModify;
CLcdWndSettingModify 	CLcdApp::m_WndSettingModify;
CLcdWndSettingSave 	CLcdApp::m_WndSettingSave;
CLcdWndSettingSwitch 	CLcdApp::m_WndSettingSwitch;
CLcdWndSettingCopy 	CLcdApp::m_WndSettingCopy;
CLcdWndSoftSwitch 	CLcdApp::m_WndSoftSwitch;
CLcdWndSysSetting 	CLcdApp::m_WndSysSetting;
CLcdWndBOTime 	CLcdApp::m_WndBOTime;
CLcdWndCIPara 	CLcdApp::m_WndCIPara;
CLcdWndtime 	CLcdApp::m_Wndtime;
CLcdWndInputSetting 	CLcdApp::m_WndInputSetting;
CLcdWndBIFilter 	CLcdApp::m_WndBIFilter;
CLcdWndBiNot 	CLcdApp::m_WndBiNot;
CLcdWndSoeProperty 	CLcdApp::m_WndSoeProperty;
CLcdWndCiBase 	CLcdApp::m_WndCiBase;
CLcdWndPulseConst 	CLcdApp::m_WndPulseConst;
CLcdWndCF 	CLcdApp::m_WndCF;
CLcdWndTest 	CLcdApp::m_WndTest;
CLcdWndfactorySet 	CLcdApp::m_WndfactorySet;
CLcdWndManualREC 	CLcdApp::m_WndManualREC;
CLcdWndOutputTest 	CLcdApp::m_WndOutputTest;
CLcdWndLanTest 	CLcdApp::m_WndLanTest;
CLcdWndReportTest 	CLcdApp::m_WndReportTest;
CLcdWndAddress 	CLcdApp::m_WndAddress;
CLcdWndEthadd 	CLcdApp::m_WndEthadd;
CLcdWndCANAddress 	CLcdApp::m_WndCANAddress;
CLcdWndPassChange 	CLcdApp::m_WndPassChange;
CLcdWndSettingChgAreaNumInput 	CLcdApp::m_WndSettingChgAreaNumInput;
CLcdWndsyspara 	CLcdApp::m_Wndsyspara;
CLcdWndSettingSwitchConfirm 	CLcdApp::m_WndSettingSwitchConfirm;
CLcdWndPortStatus 	CLcdApp::m_WndPortStatus;
CLcdWndWStatus 	CLcdApp::m_WndWStatus;
#ifndef __PL_WIN_SIMULATION__
extern void LcdTask(DWORD *pdwApp);
void LcdTask(DWORD *pdwApp)
{
	CLcdApp *pLcdApp = new CLcdApp(pdwApp);
	LcdCtlerInit();
	pLcdApp->Init();
	pLcdApp->Run();
}
#endif
CLcdApp::CLcdApp(DWORD *pdwApp):CHmiApp(pdwApp)
{
}
void CLcdApp::CallCheck()
{
}
void CLcdApp::Init()
{
	//调用父窗口初始化函数
	CHmiApp::Init();
	SetWinNum(46);
	m_pWndList[0] = &m_WndMainMenu;
	m_pWndList[1] = &m_WndMeasure;
	m_pWndList[2] = &m_WndASModify;
	m_pWndList[3] = &m_WndASQuery;
	m_pWndList[4] = &m_WndReport;
	m_pWndList[5] = &m_WndActionReport;
	m_pWndList[6] = &m_WndAlarmReport;
	m_pWndList[7] = &m_WndStartReport;
	m_pWndList[8] = &m_WndOperationReport;
	m_pWndList[9] = &m_WndSettingQueryAreaNumInput;
	m_pWndList[10] = &m_WndSoeReport;
	m_pWndList[11] = &m_WndSetting;
	m_pWndList[12] = &m_WndconfigQury;
	m_pWndList[13] = &m_WndSettingQuery;
	m_pWndList[14] = &m_WndconfigModify;
	m_pWndList[15] = &m_WndSettingModify;
	m_pWndList[16] = &m_WndSettingSave;
	m_pWndList[17] = &m_WndSettingSwitch;
	m_pWndList[18] = &m_WndSettingCopy;
	m_pWndList[19] = &m_WndSoftSwitch;
	m_pWndList[20] = &m_WndSysSetting;
	m_pWndList[21] = &m_WndBOTime;
	m_pWndList[22] = &m_WndCIPara;
	m_pWndList[23] = &m_Wndtime;
	m_pWndList[24] = &m_WndInputSetting;
	m_pWndList[25] = &m_WndBIFilter;
	m_pWndList[26] = &m_WndBiNot;
	m_pWndList[27] = &m_WndSoeProperty;
	m_pWndList[28] = &m_WndCiBase;
	m_pWndList[29] = &m_WndPulseConst;
	m_pWndList[30] = &m_WndCF;
	m_pWndList[31] = &m_WndTest;
	m_pWndList[32] = &m_WndfactorySet;
	m_pWndList[33] = &m_WndManualREC;
	m_pWndList[34] = &m_WndOutputTest;
	m_pWndList[35] = &m_WndLanTest;
	m_pWndList[36] = &m_WndReportTest;
	m_pWndList[37] = &m_WndAddress;
	m_pWndList[38] = &m_WndEthadd;
	m_pWndList[39] = &m_WndCANAddress;
	m_pWndList[40] = &m_WndPassChange;
	m_pWndList[41] = &m_WndSettingChgAreaNumInput;
	m_pWndList[42] = &m_Wndsyspara;
	m_pWndList[43] = &m_WndSettingSwitchConfirm;
	m_pWndList[44] = &m_WndPortStatus;
	m_pWndList[45] = &m_WndWStatus;
	m_pMainWnd = m_pWndList[0];
	CHmiApp::m_ppWndList =  &m_pWndList[0];
	for(int i=0; i<m_nWinNum; i++)
	{
		m_pWndList[i]->Init();
	}
	m_pRunWnd = m_pInitWnd;
	m_pInitWnd->SetEnterWnd(m_pMainWnd);
	m_pMainWnd->SetEscWnd(m_pInitWnd);
	m_pInitWnd->Show(0,0);
	//调用自检函数
	CallCheck();
}
CLcdApp::~CLcdApp()
{
}
