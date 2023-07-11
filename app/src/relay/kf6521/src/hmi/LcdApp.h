//{{Begin of Describtion of LcdApp.h
/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/
/*************************************************************************
* 
*   文件名称：
*			LCDapp.h
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
*		 液晶工具软件	2009-06-01		初始创建
* 
*************************************************************************/

#include "HmiApp.h"
#include "HmiWnd.h"
#include "HmiButton.h"
#include "HmiEdit.h"
#include "HmiList.h"
#include "HmiStatic.h"
#include "HmiDataList.h"
#include "HmiDataStatic.h"
#include "HmiCombo.h"

//}}End of Describtion of LcdApp.h

///////////////////////////////////////////////
class CLcdWndMainMenu : public CHmiWnd
{
public:
	CLcdWndMainMenu();
	BOOL Init();
	virtual ~CLcdWndMainMenu();
	void OnEnterBtnInput();
	void OnEnterBtnSoftSwitch();
	void OnEnterBtnSystem();
	void OnEnterBtntest();
	void OnEnterBtnAbout();
	CHmiButton m_BtnMeasure;
	CHmiButton m_BtnSetting;
	CHmiButton m_BtnInput;
	CHmiButton m_BtnSoftSwitch;
	CHmiButton m_BtnReport;
	CHmiButton m_BtnSystem;
	CHmiButton m_Btntest;
	CHmiButton m_BtnStatus;
	CHmiButton m_BtnAbout;
	CHmiObj* m_pObjList[10];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndMainMenu)
	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndMainMenu)
};

///////////////////////////////////////////////
class CLcdWndMeasure : public CHmiWnd
{
public:
	CLcdWndMeasure();
	BOOL Init();
	virtual ~CLcdWndMeasure();
	void OnEnterBtnHMeasure();
	void OnEnterBtnLMeasure();
	void OnEnterBtnPML();
	void OnEnterBtnAngle();
	void OnEnterBtnsyc();
	void OnEnterBtnKilowatt();
	void OnEnterBtnJDXX();
	CHmiButton m_BtnHMeasure;
	CHmiButton m_BtnLMeasure;
	CHmiButton m_BtnPML;
	CHmiButton m_BtnAngle;
	CHmiButton m_Btnsyc;
	CHmiButton m_BtnKilowatt;
	CHmiButton m_BtnJDXX;
	CHmiObj* m_pObjList[8];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndMeasure)
//}}end of user function and memeber(CLcdWndMeasure)
};

///////////////////////////////////////////////
class CLcdWndASModify : public CHmiWnd
{
public:
	CLcdWndASModify();
	BOOL Init();
	virtual ~CLcdWndASModify();
	CHmiEdit m_EditAsInput;
	CHmiDataStatic m_StaticASSymbol;
	CHmiDataStatic m_StaticAsUnit;
	CHmiDataStatic m_StaticDownLimit;
	CHmiDataStatic m_StaticUpLimit;
	CHmiDataStatic m_StaticAsName;
	CHmiCombo m_ComboAsCombo;
	CHmiObj* m_pObjList[8];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndASModify)
//	float m_fSysSetBuf[256]; //缓存用户输入的辅助定值
public:
	WORD m_wSysSetIndex; //当前显示的辅助定值序号
	BOOL m_bIsCombo; //输入是否是使用combo控件
public:
	void OnWndUpDown(LONG UpDown);
	void OnWndAddSub(LONG AddSub);
	
	BOOL OnWndInit();
	void OnWndEnter();
	static void SaveSysSetting();
//}}end of user function and memeber(CLcdWndASModify)
};

///////////////////////////////////////////////
class CLcdWndASQuery : public CHmiWnd
{
public:
	CLcdWndASQuery();
	BOOL Init();
	virtual ~CLcdWndASQuery();
	CHmiDataList m_ListASList;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndASQuery)
//}}end of user function and memeber(CLcdWndASQuery)
};

///////////////////////////////////////////////
class CLcdWndReport : public CHmiWnd
{
public:
	CLcdWndReport();
	BOOL Init();
	virtual ~CLcdWndReport();
	void OnEnterBtnActionReport();
	void OnEnterBtnAlarmReport();
	void OnEnterBtnStartReport();
	void OnEnterBtnSoe();
	void OnEnterBtnOperation();
	CHmiButton m_BtnActionReport;
	CHmiButton m_BtnAlarmReport;
	CHmiButton m_BtnStartReport;
	CHmiButton m_BtnSoe;
	CHmiButton m_BtnOperation;
	CHmiObj* m_pObjList[6];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndReport)
//}}end of user function and memeber(CLcdWndReport)
};

///////////////////////////////////////////////
class CLcdWndActionReport : public CHmiWnd
{
public:
	CLcdWndActionReport();
	BOOL Init();
	virtual ~CLcdWndActionReport();
	void OnEnterListActionReport();
	CHmiDataList m_ListActionReport;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndActionReport)
	CHmiActionDetailDlg m_ActionDetailDlg; //动作报告详情
//}}end of user function and memeber(CLcdWndActionReport)
};

///////////////////////////////////////////////
class CLcdWndAlarmReport : public CHmiWnd
{
public:
	CLcdWndAlarmReport();
	BOOL Init();
	virtual ~CLcdWndAlarmReport();
	void OnEnterListAlarmReport();
	CHmiDataList m_ListAlarmReport;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndAlarmReport)
	CHmiAlarmDetailDlg m_AlarmDetailDlg; //告警报告详情窗口
//}}end of user function and memeber(CLcdWndAlarmReport)
};

///////////////////////////////////////////////
class CLcdWndStartReport : public CHmiWnd
{
public:
	CLcdWndStartReport();
	BOOL Init();
	virtual ~CLcdWndStartReport();
	void OnEnterListStartReport();
	CHmiDataList m_ListStartReport;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndStartReport)
	CHmiStartDetailDlg m_StartDetailDlg; //启动报告详情
//}}end of user function and memeber(CLcdWndStartReport)
};

///////////////////////////////////////////////
class CLcdWndOperationReport : public CHmiWnd
{
public:
	CLcdWndOperationReport();
	BOOL Init();
	virtual ~CLcdWndOperationReport();
	CHmiDataList m_ListOperationReport;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndOperationReport)
	COperationDetailDlg m_OperationDetailDlg;

//}}end of user function and memeber(CLcdWndOperationReport)
};

///////////////////////////////////////////////
class CLcdWndSettingQueryAreaNumInput : public CHmiWnd
{
public:
	CLcdWndSettingQueryAreaNumInput();
	BOOL Init();
	virtual ~CLcdWndSettingQueryAreaNumInput();
	CHmiEdit m_EditAreaNum;
	CHmiDataStatic m_Static4883;
	CHmiDataStatic m_Static4915;
	CHmiDataStatic m_StaticNowAreaNum;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingQueryAreaNumInput)
	LONG m_nAreaNO;
	
	BOOL OnWndInit();
	void OnWndEnter();
//}}end of user function and memeber(CLcdWndSettingQueryAreaNumInput)
};

///////////////////////////////////////////////
class CLcdWndSoeReport : public CHmiWnd
{
public:
	CLcdWndSoeReport();
	BOOL Init();
	virtual ~CLcdWndSoeReport();
	CHmiDataList m_ListSoeReport;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSoeReport)
	CSOEDetailDlg m_SoeDetaildlg;
//}}end of user function and memeber(CLcdWndSoeReport)
};

///////////////////////////////////////////////
class CLcdWndSetting : public CHmiWnd
{
public:
	CLcdWndSetting();
	BOOL Init();
	virtual ~CLcdWndSetting();
	void OnEnterBtnSettingQuery();
	void OnEnterBtnsettingModify();
	void OnEnterBtnSettingCopy();
	void OnEnterBtnsettingExChg();
	void OnEnterBtnASModify();
	CHmiButton m_BtnSettingQuery;
	CHmiButton m_BtnsettingModify;
	CHmiButton m_BtnSettingCopy;
	CHmiButton m_BtnsettingExChg;
	CHmiButton m_BtnASQuery;
	CHmiButton m_BtnASModify;
	CHmiObj* m_pObjList[7];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSetting)
//}}end of user function and memeber(CLcdWndSetting)
};

///////////////////////////////////////////////
class CLcdWndconfigQury : public CHmiWnd
{
public:
	CLcdWndconfigQury();
	BOOL Init();
	virtual ~CLcdWndconfigQury();
	CHmiDataList m_ListConfigQuery;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndconfigQury)
public:
	WORD m_wAreaNo;
	
	BOOL OnWndInit();
//}}end of user function and memeber(CLcdWndconfigQury)
};

///////////////////////////////////////////////
class CLcdWndSettingQuery : public CHmiWnd
{
public:
	CLcdWndSettingQuery();
	BOOL Init();
	virtual ~CLcdWndSettingQuery();
	CHmiDataList m_ListSettingQuery;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingQuery)
public:
	WORD m_wAreaNo;
	WORD m_wDipsSettingList[512];
	
	BOOL OnWndInit();
//}}end of user function and memeber(CLcdWndSettingQuery)
};

///////////////////////////////////////////////
class CLcdWndconfigModify : public CHmiWnd
{
public:
	CLcdWndconfigModify();
	BOOL Init();
	virtual ~CLcdWndconfigModify();
	void OnEnterListConfigModify();
	CHmiDataList m_ListConfigModify;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndconfigModify)
public:
	static	WORD m_wAreaNo; //要修改的定值区号
	CHmiMsgBox m_msgBox; //无可显示的定值时提示
	static	void SaveConfig(); //保存配置
	
	BOOL OnWndInit();
//	void OnWndEnter();
//}}end of user function and memeber(CLcdWndconfigModify)
};

///////////////////////////////////////////////
class CLcdWndSettingModify : public CHmiWnd
{
public:
	CLcdWndSettingModify();
	BOOL Init();
	virtual ~CLcdWndSettingModify();
	CHmiEdit m_EditSettingData;
	CHmiDataStatic m_StaticSymbol;
	CHmiDataStatic m_StaticName;
	CHmiDataStatic m_StaticUpLimit;
	CHmiDataStatic m_StaticDownLimit;
	CHmiDataStatic m_StaticUnit;
	CHmiCombo m_ComboSettingComb;
	CHmiObj* m_pObjList[8];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingModify)
public:
	WORD m_wAreaNo;
	WORD m_wDispSettingCount; //可显示的定值个数
//	WORD m_wDipsSettingList[256]; //可显示的定值列表
	BOOL m_bIsCombo; //是否使用combo最为输入
	
	WORD m_wDispSetIndex; //定值显示序号，不是数据库中定值序号，而是可显示列表中的序号
	void OnWndUpDown(LONG UpDown);
	void OnWndAddSub(LONG AddSub);
	
	BOOL OnWndInit();
	void OnWndEnter();
//}}end of user function and memeber(CLcdWndSettingModify)
};

///////////////////////////////////////////////
class CLcdWndSettingSave : public CHmiWnd
{
public:
	CLcdWndSettingSave();
	BOOL Init();
	virtual ~CLcdWndSettingSave();
	void OnEnterBtnok();
	CHmiButton m_Btnok;
	CHmiButton m_Btncancel;
	CHmiDataStatic m_Static4697;
	CHmiDataStatic m_Static4698;
	CHmiDataStatic m_Static4699;
	CHmiObj* m_pObjList[6];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingSave)
public:
	WORD m_wAreaNo;
//}}end of user function and memeber(CLcdWndSettingSave)
};

///////////////////////////////////////////////
class CLcdWndSettingSwitch : public CHmiWnd
{
public:
	CLcdWndSettingSwitch();
	BOOL Init();
	virtual ~CLcdWndSettingSwitch();
	CHmiEdit m_EditAreaNum;
	CHmiDataStatic m_Static4702;
	CHmiDataStatic m_Static4706;
	CHmiDataStatic m_StaticNowAreaNum;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingSwitch)
	BOOL OnWndInit();
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndSettingSwitch)
};

///////////////////////////////////////////////
class CLcdWndSettingCopy : public CHmiWnd
{
public:
	CLcdWndSettingCopy();
	BOOL Init();
	virtual ~CLcdWndSettingCopy();
	CHmiEdit m_EditSoureAreaNum;
	CHmiEdit m_EditDstStartAreaNum;
	CHmiEdit m_EditDstEndAreaNum;
	CHmiDataStatic m_Static4710;
	CHmiDataStatic m_Static4711;
	CHmiDataStatic m_Static4885;
	CHmiDataStatic m_Static4886;
	CHmiDataStatic m_Static5350;
	CHmiObj* m_pObjList[9];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingCopy)
	static void CopyAreaSet();
public:
	static	WORD m_wSourArea, m_wStartArea, m_wEndArea;
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndSettingCopy)
};

///////////////////////////////////////////////
class CLcdWndSoftSwitch : public CHmiWnd
{
public:
	CLcdWndSoftSwitch();
	BOOL Init();
	virtual ~CLcdWndSoftSwitch();
	void OnEnterListSoftSwitch();
	CHmiDataList m_ListSoftSwitch;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSoftSwitch)
	static void SaveSoftSwitch();
//	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndSoftSwitch)
};

///////////////////////////////////////////////
class CLcdWndSysSetting : public CHmiWnd
{
public:
	CLcdWndSysSetting();
	BOOL Init();
	virtual ~CLcdWndSysSetting();
	void OnEnterBtnBOTime();
	void OnEnterBtnLanguage();
	CHmiButton m_BtnAdress;
	CHmiButton m_BtnInputSetting;
	CHmiButton m_BtnBOTime;
	CHmiButton m_BtnCIPara;
	CHmiButton m_BtnDeviceTime;
	CHmiButton m_BtnLanguage;
	CHmiButton m_BtnPassSetting;
	CHmiButton m_BtnSysParam;
	CHmiObj* m_pObjList[9];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSysSetting)
//}}end of user function and memeber(CLcdWndSysSetting)
};

///////////////////////////////////////////////
class CLcdWndCIPara : public CHmiWnd
{
public:
	CLcdWndCIPara();
	BOOL Init();
	virtual ~CLcdWndCIPara();
	void OnEnterBtnDIProperty();
	void OnEnterBtnIntegralKW();
	void OnEnterBtnCIbase();
	void OnEnterBtnPulseConst();
	CHmiButton m_BtnDIProperty;
	CHmiButton m_BtnIntegralKW;
	CHmiButton m_BtnCIbase;
	CHmiButton m_BtnPulseConst;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndCIPara)
//手动添加的成员变量放在括号之间
	static void ModefyDIProperty();
	static void ModifyIntKWCaluMode();

//}}end of user function and memeber(CLcdWndCIPara)
};

///////////////////////////////////////////////
class CLcdWndBOTime : public CHmiWnd
{
public:
	CLcdWndBOTime();
	BOOL Init();
	virtual ~CLcdWndBOTime();
	CHmiDataList m_ListBOTimeList;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndBOTime)
//手动添加的成员变量放在括号之间
	static void SaveBOLastTime();
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndBOTime)
};

///////////////////////////////////////////////
class CLcdWndtime : public CHmiWnd
{
public:
	CLcdWndtime();
	BOOL Init();
	virtual ~CLcdWndtime();
	CHmiEdit m_EditYear;
	CHmiEdit m_EditMonth;
	CHmiEdit m_EditDay;
	CHmiEdit m_EditHour;
	CHmiEdit m_EditMinute;
	CHmiEdit m_EditSecond;
	CHmiDataStatic m_Static4737;
	CHmiDataStatic m_Static4738;
	CHmiDataStatic m_Static4739;
	CHmiDataStatic m_Static4740;
	CHmiDataStatic m_Static4741;
	CHmiDataStatic m_Static4742;
	CHmiObj* m_pObjList[13];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndtime)
	BOOL OnWndInit();
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndtime)
};

///////////////////////////////////////////////
class CLcdWndInputSetting : public CHmiWnd
{
public:
	CLcdWndInputSetting();
	BOOL Init();
	virtual ~CLcdWndInputSetting();
	void OnEnterBtnSoeProperty();
	void OnEnterBtnBIFilter();
	void OnEnterBtnBiNo();
	CHmiButton m_BtnSoeProperty;
	CHmiButton m_BtnBIFilter;
	CHmiButton m_BtnBiNo;
	CHmiObj* m_pObjList[4];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndInputSetting)
//}}end of user function and memeber(CLcdWndInputSetting)
};

///////////////////////////////////////////////
class CLcdWndBIFilter : public CHmiWnd
{
public:
	CLcdWndBIFilter();
	BOOL Init();
	virtual ~CLcdWndBIFilter();
	CHmiDataList m_ListListBiFilter;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndBIFilter)
	static void SaveDIFilterTime(); //保存DI去抖时间
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndBIFilter)
};

///////////////////////////////////////////////
class CLcdWndBiNot : public CHmiWnd
{
public:
	CLcdWndBiNot();
	BOOL Init();
	virtual ~CLcdWndBiNot();
	void OnEnterListBiNot();
	CHmiDataList m_ListBiNot;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndBiNot)
	static void ChangeBInegation(); //保存遥信取反信息

//}}end of user function and memeber(CLcdWndBiNot)
};

///////////////////////////////////////////////
class CLcdWndSoeProperty : public CHmiWnd
{
public:
	CLcdWndSoeProperty();
	BOOL Init();
	virtual ~CLcdWndSoeProperty();
	CHmiDataList m_ListSoePropetry;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSoeProperty)
	static void SaveBISoePara(); //保存 遥信是否产生soe属性 ， 对应数据库中wControl-D1
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndSoeProperty)
};

///////////////////////////////////////////////
class CLcdWndCiBase : public CHmiWnd
{
public:
	CLcdWndCiBase();
	BOOL Init();
	virtual ~CLcdWndCiBase();
	CHmiEdit m_EditIntKW1;
	CHmiEdit m_EditIntKW2;
	CHmiEdit m_EditPulseKW1;
	CHmiEdit m_EditPulseKW2;
	CHmiDataStatic m_StaticStaticIntKW1;
	CHmiDataStatic m_StaticStaticIntKW2;
	CHmiDataStatic m_StaticStaticPulseKW1;
	CHmiDataStatic m_StaticStaticPulseKW2;
	CHmiObj* m_pObjList[9];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndCiBase)
//手动添加的成员变量放在括号之间
	void OnWndEnter();	
	BOOL OnWndInit();
	DWORD CIbase[4];
	static void SaveCiBase();

//}}end of user function and memeber(CLcdWndCiBase)
};

///////////////////////////////////////////////
class CLcdWndPulseConst : public CHmiWnd
{
public:
	CLcdWndPulseConst();
	BOOL Init();
	virtual ~CLcdWndPulseConst();
	void OnEnterListPulseConst();
	CHmiDataList m_ListPulseConst;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndPulseConst)
//手动添加的成员变量放在括号之间
	static void SavePulseConst();
//}}end of user function and memeber(CLcdWndPulseConst)
};

///////////////////////////////////////////////
class CLcdWndCF : public CHmiWnd
{
public:
	CLcdWndCF();
	BOOL Init();
	virtual ~CLcdWndCF();
	CHmiDataList m_ListCFList;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndCF)
	static void ChgCF();	
	void OnWndEnter();
//}}end of user function and memeber(CLcdWndCF)
};

///////////////////////////////////////////////
class CLcdWndTest : public CHmiWnd
{
public:
	CLcdWndTest();
	BOOL Init();
	virtual ~CLcdWndTest();
	void OnEnterBtnFactoryData();
	CHmiButton m_BtnOutputTest;
	CHmiButton m_BtnLenTest;
	CHmiButton m_BtnReportTest;
	CHmiButton m_BtnManualREC;
	CHmiButton m_BtnFactoryData;
	CHmiObj* m_pObjList[6];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndTest)
//	static	void AutoJustCF();
//	static  void ReportTest();
//}}end of user function and memeber(CLcdWndTest)
};

///////////////////////////////////////////////
class CLcdWndfactorySet : public CHmiWnd
{
public:
	CLcdWndfactorySet();
	BOOL Init();
	virtual ~CLcdWndfactorySet();
	void OnEnterBtnManualCoef();
	void OnEnterBtnAutoCoef();
	CHmiButton m_BtnManualCoef;
	CHmiButton m_BtnAutoCoef;
	CHmiObj* m_pObjList[3];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndfactorySet)
//手动添加的成员变量放在括号之间
//}}end of user function and memeber(CLcdWndfactorySet)
};

///////////////////////////////////////////////
class CLcdWndManualREC : public CHmiWnd
{
public:
	CLcdWndManualREC();
	BOOL Init();
	virtual ~CLcdWndManualREC();
	CHmiDataStatic m_Staticstatic1;
	CHmiDataStatic m_Staticstatic2;
	CHmiDataStatic m_Staticstatic3;
	CHmiObj* m_pObjList[4];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndManualREC)
//手动添加的成员变量放在括号之间
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndManualREC)
};

///////////////////////////////////////////////
class CLcdWndOutputTest : public CHmiWnd
{
public:
	CLcdWndOutputTest();
	BOOL Init();
	virtual ~CLcdWndOutputTest();
	void OnEnterEditOutputNum();
	void OnAddSubEditOutputNum(LONG AddSub);
	CHmiEdit m_EditOutputNum;
	CHmiEdit m_EditOutputTime;
	CHmiDataStatic m_StaticOutputName;
	CHmiDataStatic m_Static4771;
	CHmiDataStatic m_Static4773;
	CHmiDataStatic m_Static4775;
	CHmiDataStatic m_Static4776;
	CHmiObj* m_pObjList[8];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndOutputTest)
	BOOL OnWndInit();
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndOutputTest)
};

///////////////////////////////////////////////
class CLcdWndLanTest : public CHmiWnd
{
public:
	CLcdWndLanTest();
	BOOL Init();
	virtual ~CLcdWndLanTest();
	void OnEnterBtnStart();
	CHmiButton m_BtnStart;
	CHmiButton m_BtnCancel;
	CHmiDataStatic m_Static4779;
	CHmiDataStatic m_Static4780;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndLanTest)
//}}end of user function and memeber(CLcdWndLanTest)
};

///////////////////////////////////////////////
class CLcdWndReportTest : public CHmiWnd
{
public:
	CLcdWndReportTest();
	BOOL Init();
	virtual ~CLcdWndReportTest();
	CHmiDataStatic m_Static4784;
	CHmiDataStatic m_Static4785;
	CHmiObj* m_pObjList[3];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndReportTest)
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndReportTest)
};

///////////////////////////////////////////////
class CLcdWndAddress : public CHmiWnd
{
public:
	CLcdWndAddress();
	BOOL Init();
	virtual ~CLcdWndAddress();
	void OnEnterBtnSerialSet();
	CHmiButton m_BtnCAN;
	CHmiButton m_BtnETH;
	CHmiButton m_BtnPortStatus;
	CHmiButton m_BtnSerialSet;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndAddress)
//手动添加的成员变量放在括号之间
//}}end of user function and memeber(CLcdWndAddress)
};

///////////////////////////////////////////////
class CLcdWndEthadd : public CHmiWnd
{
public:
	CLcdWndEthadd();
	BOOL Init();
	virtual ~CLcdWndEthadd();
	void OnEnterBtnetha();
	void OnEnterBtnethb();
	CHmiButton m_Btnetha;
	CHmiButton m_Btnethb;
	CHmiObj* m_pObjList[3];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndEthadd)
//手动添加的成员变量放在括号之间
//}}end of user function and memeber(CLcdWndEthadd)
};

///////////////////////////////////////////////
class CLcdWndCANAddress : public CHmiWnd
{
public:
	CLcdWndCANAddress();
	BOOL Init();
	virtual ~CLcdWndCANAddress();
	void OnEnterBtnenter();
	CHmiButton m_Btnenter;
	CHmiButton m_BtnESC;
	CHmiEdit m_EditCANA;
	CHmiDataStatic m_Static5417;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndCANAddress)
//手动添加的成员变量放在括号之间
	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndCANAddress)
};

///////////////////////////////////////////////
class CLcdWndPassChange : public CHmiWnd
{
public:
	CLcdWndPassChange();
	BOOL Init();
	virtual ~CLcdWndPassChange();
	void OnEnterBtnChgCommonPass();
	void OnEnterBtnChgSettingPass();
	CHmiButton m_BtnChgCommonPass;
	CHmiButton m_BtnChgSettingPass;
	CHmiObj* m_pObjList[3];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndPassChange)
//}}end of user function and memeber(CLcdWndPassChange)
};

///////////////////////////////////////////////
class CLcdWndSettingChgAreaNumInput : public CHmiWnd
{
public:
	CLcdWndSettingChgAreaNumInput();
	BOOL Init();
	virtual ~CLcdWndSettingChgAreaNumInput();
	CHmiEdit m_EditInputAreaNum;
	CHmiDataStatic m_Static4899;
	CHmiDataStatic m_Static4917;
	CHmiDataStatic m_StaticNowAreaNum;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingChgAreaNumInput)
	BOOL OnWndInit();
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndSettingChgAreaNumInput)
};

///////////////////////////////////////////////
class CLcdWndsyspara : public CHmiWnd
{
public:
	CLcdWndsyspara();
	BOOL Init();
	virtual ~CLcdWndsyspara();
	void OnEnterBtnContrast();
	void OnEnterBtnFVDH();
	void OnEnterBtnIRGBType();
	void OnEnterBtnDefaultPara();
	CHmiButton m_BtnContrast;
	CHmiButton m_BtnFVDH;
	CHmiButton m_BtnIRGBType;
	CHmiButton m_BtnDefaultPara;
	CHmiObj* m_pObjList[5];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndsyspara)
	BOOL OnWndInit();	
	static void ModifyFVDispMode();
	static void ModifyIntKWCaluMode();	
	static void ModifyIRGBType()
//}}end of user function and memeber(CLcdWndsyspara)
};

///////////////////////////////////////////////
class CLcdWndPortStatus : public CHmiWnd
{
public:
	CLcdWndPortStatus();
	BOOL Init();
	virtual ~CLcdWndPortStatus();
	void OnEnterBtnPort();
	void OnEnterBtnProtocol();
	CHmiButton m_BtnPort;
	CHmiButton m_BtnProtocol;
	CHmiObj* m_pObjList[3];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndPortStatus)
//手动添加的成员变量放在括号之间
//}}end of user function and memeber(CLcdWndPortStatus)
};

///////////////////////////////////////////////
class CLcdWndSettingSwitchConfirm : public CHmiWnd
{
public:
	CLcdWndSettingSwitchConfirm();
	BOOL Init();
	virtual ~CLcdWndSettingSwitchConfirm();
	void OnEnterBtnOk();
	CHmiButton m_BtnOk;
	CHmiButton m_BtnCancel;
	CHmiDataStatic m_Static4920;
	CHmiDataStatic m_StaticNowAreaNo;
	CHmiDataStatic m_StaticNewAreaNo;
	CHmiDataStatic m_Static44444;
	CHmiObj* m_pObjList[7];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSettingSwitchConfirm)
public:
	WORD m_wNewAreaNo;	
	BOOL OnWndInit();
//}}end of user function and memeber(CLcdWndSettingSwitchConfirm)
};

///////////////////////////////////////////////
class CLcdWndWStatus : public CHmiWnd
{
public:
	CLcdWndWStatus();
	BOOL Init();
	virtual ~CLcdWndWStatus();
	void OnEnterBtnDevStatus();
	void OnEnterBtnComStatus();
	CHmiButton m_BtnDevStatus;
	CHmiButton m_BtnComStatus;
	CHmiObj* m_pObjList[3];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndWStatus)
//手动添加的成员变量放在括号之间
//}}end of user function and memeber(CLcdWndWStatus)
};

class CLcdApp : public CHmiApp
{
public:
	CLcdApp(DWORD *pdwApp);
	virtual ~CLcdApp();
	void Init();
	void CallCheck();
	static CLcdWndMainMenu 	m_WndMainMenu;
	static CLcdWndMeasure 	m_WndMeasure;
	static CLcdWndASModify 	m_WndASModify;
	static CLcdWndASQuery 	m_WndASQuery;
	static CLcdWndReport 	m_WndReport;
	static CLcdWndActionReport 	m_WndActionReport;
	static CLcdWndAlarmReport 	m_WndAlarmReport;
	static CLcdWndStartReport 	m_WndStartReport;
	static CLcdWndOperationReport 	m_WndOperationReport;
	static CLcdWndSettingQueryAreaNumInput 	m_WndSettingQueryAreaNumInput;
	static CLcdWndSoeReport 	m_WndSoeReport;
	static CLcdWndSetting 	m_WndSetting;
	static CLcdWndconfigQury 	m_WndconfigQury;
	static CLcdWndSettingQuery 	m_WndSettingQuery;
	static CLcdWndconfigModify 	m_WndconfigModify;
	static CLcdWndSettingModify 	m_WndSettingModify;
	static CLcdWndSettingSave 	m_WndSettingSave;
	static CLcdWndSettingSwitch 	m_WndSettingSwitch;
	static CLcdWndSettingCopy 	m_WndSettingCopy;
	static CLcdWndSoftSwitch 	m_WndSoftSwitch;
	static CLcdWndSysSetting 	m_WndSysSetting;
	static CLcdWndCIPara 	m_WndCIPara;
	static CLcdWndBOTime 	m_WndBOTime;
	static CLcdWndtime 	m_Wndtime;
	static CLcdWndInputSetting 	m_WndInputSetting;
	static CLcdWndBIFilter 	m_WndBIFilter;
	static CLcdWndBiNot 	m_WndBiNot;
	static CLcdWndSoeProperty 	m_WndSoeProperty;
	static CLcdWndCiBase 	m_WndCiBase;
	static CLcdWndPulseConst 	m_WndPulseConst;
	static CLcdWndCF 	m_WndCF;
	static CLcdWndTest 	m_WndTest;
	static CLcdWndfactorySet 	m_WndfactorySet;
	static CLcdWndManualREC 	m_WndManualREC;
	static CLcdWndOutputTest 	m_WndOutputTest;
	static CLcdWndLanTest 	m_WndLanTest;
	static CLcdWndReportTest 	m_WndReportTest;
	static CLcdWndAddress 	m_WndAddress;
	static CLcdWndEthadd 	m_WndEthadd;
	static CLcdWndCANAddress 	m_WndCANAddress;
	static CLcdWndPassChange 	m_WndPassChange;
	static CLcdWndSettingChgAreaNumInput 	m_WndSettingChgAreaNumInput;
	static CLcdWndsyspara 	m_Wndsyspara;
	static CLcdWndPortStatus 	m_WndPortStatus;
	static CLcdWndSettingSwitchConfirm 	m_WndSettingSwitchConfirm;
	static CLcdWndWStatus 	m_WndWStatus;

	CHmiWnd* m_pWndList[46];
};
