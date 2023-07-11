/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiDataList.cpp
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
*		kangqian	2008-4-21		初始创建
* 
*************************************************************************/

// HmiDataList.cpp: implementation of the CHmiDataList class.
//
//////////////////////////////////////////////////////////////////////

//@#include "..\hmi\wingui.h"
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			
#include "Db_err.h"

#include "HmiDataList.h"
#include "HmiApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char OperationStr[][32] =
{
	"null", //没有
	"系统复位",//@	#define MISC_CLASS_RESET		1
	"下装文件",//@#define MISC_CLASS_DOWNFILE		2
	"通讯事件",//@#define MISC_CLASS_COMM			3
	"遥控事件",//@#define MISC_CLASS_BO			4
	"遥调事件",//@#define MISC_CLASS_AO			5
	"同期事件",//@#define MISC_CLASS_SYNC			6
	"修改定值",//@#define MISC_CLASS_SET			7
	"改定值区号",//@#define MISC_CLASS_SETAREA		8
	"修改软压板",//@#define MISC_CLASS_SW			9
	"修改配置",//@#define MISC_CLASS_CFG			10
	"修改系数",//@#define MISC_CLASS_CF			11
	"修改参数",//@#define MISC_CLASS_PARA			12
	"电源事件",//@#define MISC_CLASS_POWER		13
	"设电度底数",//@#define MISC_CLASS_CLEARCI		14
	"遥测置数",//@#define MISC_CLASS_MODIAI		15
	"遥信置位",//@#define MISC_CLASS_MODIBI		16
	"电度置数",//@#define MISC_CLASS_MODICI		17

};
CHmiDataList::CHmiDataList()
{
	m_nCursorPos = 0;
	m_nAreaNo = 1;
}
/****************************************************************************************
 * 
 * 功能描述: 
 * 
 * 参数说明: 
 *			- LONG dataType
 *			  类型: 
 *			  说明: 
 *
 *			- LONG dd
 *			  类型: 
 *			  说明: 
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
CHmiDataList::CHmiDataList(LONG dataType)
{
	m_nDataType = dataType;
	m_nRecordPerLine = 1;
}

CHmiDataList::~CHmiDataList()
{

}

BOOL CHmiDataList::Init()
{
//@		NoneFun = -1, //-1无
//@	MoveFocus =0, //0移动焦点
//@	ChgPage =1, //1翻页
//@	Function =2, //函数
//@	ChgValue = 3, //改值

	//根据不同类型设置按键的响应属性
	switch(m_nDataType)
	{
	case ListCI:

		break;
	case ListBI:
		break;
	case Action:
		//动作报告
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		break;


	case Alarm:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		break;
	case Start:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		break;
	case Operation:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		break;
	case Soe:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;
	case ConfigQuery:
		m_bHaveEnterFun = FALSE;
		m_bHaveAddSubFun = FALSE;
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;
	case SettingQuery:
//		m_nAreaNo = DB_GetCurrentSETAreaNo();
		break;
	case ConfigChange:
		break;
	case SoftSwitch:

		break;
	case SysStatus:
		break;

	case DIPara:

		break;
	case DIFilterTime:

		break;
	case DINor:

		break;
	case CIBase:


		break;
	case CIConst:
		break;
	case SoePara:
		break;
//	case ActData:
//		break;
//	case AlarmData:
//		break;
//	case StartData:
//		break;
	default:
		;
	}
	return GetDataFromDB();

}

BOOL CHmiDataList::OnUp()
{
//@	char str[64];
//@	TSET_Disp tSetDisp;
//@	LONG nSel;
//@	CHmiList::OnUp();
//@	switch(m_nDataType)
//@	{
//@	case SettingQuery:
//@		nSel = GetCurSel();
//@		DB_GetSETDisp(nSel, &tSetDisp);
//@		sprintf(str, "定值查看：%d区 %s", m_nAreaNo, tSetDisp.strName);
//@		m_pOwner->SetCaption(str);
//@		m_pOwner->DrawCaption();
//@		break;
//@	default:
//@		break;
//@	}
	return CHmiList::OnUp();

}
BOOL CHmiDataList::OnDown()
{
	return CHmiList::OnDown();

}
BOOL CHmiDataList::OnLeft()
{
	switch(m_nDataType)
	{
	case DIFilterTime:
		if(m_nCurColumnIndex == -1)
			SelectItem(GetCurSel(), 1);
		else
		{
			m_nCursorPos +=4;
			m_nCursorPos++;
			m_nCursorPos %= 4;
			OnPaint(); 
			UpdateWindow();
			
		}
	
		break;
	case SoftSwitch:
		if(m_nCurColumnIndex == -1)
			SelectItem(GetCurSel(), 1);
		break;
	default:
		return  CHmiList::OnLeft();

	}
	return TRUE;
}
BOOL CHmiDataList::OnRight()
{
	switch(m_nDataType)
	{
	case DIFilterTime:
		if(m_nCurColumnIndex == -1)
			SelectItem(GetCurSel(), 1);
		else
		{
			m_nCursorPos +=4;
			m_nCursorPos++;
			m_nCursorPos %= 4;
			OnPaint();
			UpdateWindow();
		}
		break;
	default:
		return CHmiList::OnRight();
	}

	return TRUE;
}
BOOL CHmiDataList::OnAdd()
{
	switch(m_nDataType)
	{
	case DIFilterTime:
		ChgFilterTime();
		break;
	case DIPara:
		ChgBIParaValue();
		break;
	case DINor:
		break;
	case ConfigChange:
		ChgCfgValue();
		break;
	case SoftSwitch:
		ChgSWValue();
		break;
	default:
		return FALSE;
	}
//@	char strOn[3], strOff[3];
//@	sprintf(strOn, "%s", "投");
//@	sprintf(strOff, "%s", "退");
//@	LONG sel = GetCurSel();
//@	LONG pos = GetColPos(1);
//@	//判断是否是汉字
//@	if((unsigned char)(m_nItemTxt[sel][pos+m_nCursorPos])>0xA0)
//@	{
//@		if( (m_nItemTxt[sel][pos+m_nCursorPos] == strOn[0]) &&
//@			(m_nItemTxt[sel][pos+m_nCursorPos+1] == strOn[1]) )
//@		{
//@			m_nItemTxt[sel][pos+m_nCursorPos] = strOff[0];
//@			m_nItemTxt[sel][pos+m_nCursorPos+1] = strOff[1];
//@
//@		}
//@		else
//@		{
//@			m_nItemTxt[sel][pos+m_nCursorPos] = strOn[0];
//@			m_nItemTxt[sel][pos+m_nCursorPos+1] = strOn[1];
//@
//@		}
//@		
//@
//@	}
//@	else if(m_nItemTxt[sel][pos+m_nCursorPos] == '+' )
//@	{
//@		m_nItemTxt[sel][pos+m_nCursorPos] = '-';
//@
//@	}
//@	else if(m_nItemTxt[sel][pos+m_nCursorPos] == '-')
//@	{
//@		m_nItemTxt[sel][pos+m_nCursorPos] = '+';
//@	}
//@
//@	else if(m_nItemTxt[sel][pos+m_nCursorPos]== '9')
//@			m_nItemTxt[sel][pos+m_nCursorPos] = '0';
//@	else 
//@		m_nItemTxt[sel][pos+m_nCursorPos]++;

		

//@	switch(m_nDisplayType)
//@	{
//@	case Dec:
//@		else
//@			m_szCaption[m_nCursorPos]++;
//@	case Hex:
//@		if(m_szCaption[m_nCursorPos]== 'f' || m_szCaption[m_nCursorPos]== 'F')
//@			m_szCaption[m_nCursorPos] = 0;
//@		else
//@			m_szCaption[m_nCursorPos]++;
//@	}
	OnPaint();  //绘制内存
	UpdateWindow();  //内存到显示器
	return TRUE;
}
BOOL CHmiDataList::OnSub()
{
	switch(m_nDataType)
	{
	case DIFilterTime:
		break;
	case DIPara:
		break;
	case DINor:
		break;
	case ConfigChange:
		ChgCfgValue();
		break;
	case SoftSwitch:
		ChgSWValue();
		break;
	default:
		return FALSE;
	}
	OnPaint();  //绘制内存
	UpdateWindow();  //内存到显示器
	return TRUE;
}

//@BOOL CHmiDataList::OnEnter()
//@{
//@
//@	WORD wActionIndex;
//@	LONG nItem;
//@	nItem = GetCurSel();
//@	if(nItem <0)
//@		return false;
//@
//@	switch(m_nDataType)
//@	{
//@
//@	case DIFilterTime:
//@		SaveFilterTime();
//@		break;
//@	case Operation:
//@		OperationDetail();
//@		break;
//@	case Soe:
//@		SoeDetail();
//@		break;
//@	case Action:
//@		wActionIndex = 	GetItemData(nItem);
//@//		return false;
//@	ActionDetail(HIBYTE(wActionIndex),LOBYTE(wActionIndex));
//@		break;
//@
//@	case Alarm:
//@		wActionIndex = 	GetItemData(nItem);
//@		AlarmDetail(nItem);
//@		break;
//@	case Start:
//@		wActionIndex = 	GetItemData(nItem);
//@		StartDetail(nItem);
//@		break;
//@
//@	default:
//@		return FALSE;
//@	}
//@	return TRUE;
//@}


BOOL CHmiDataList::GetDataFromDB()
{
	ResetContent();
	m_nAllArea = DB_GetSETAreaNum();
	m_nRunAreaNo = DB_GetCurrentSETAreaNo();
//@	m_nAreaNo = m_pOwner->GetParam1();

	m_nItemHeight = 18;//
	m_nFont = 16; //



	m_nCurColumnIndex = -1;
	char str[64];


	switch(m_nDataType)
	{
	case ListCI:
		ListCIFun();
		break;
	case ListBI:
		ListBIFun();
		break;
	case Action:
		m_nFont = 12; //
		//动作报告
		ActionFun();
		break;


	case Alarm:
		m_nFont = 12; //
		AlarmFun();
		break;

	case Start:
		//启动报告
		m_nFont = 12; //
		StartFun();
		break;

	case Operation:
		OperationFun();
		break;

	case Soe:
		SoeFun();
		break;
	case ConfigQuery:
		ConfigQueryFun();
		break;
	case SettingQuery:
//		m_nAreaNo = DB_GetCurrentSETAreaNo();
		sprintf(str, "查看定值：%d区", m_nAreaNo);
		m_pOwner->SetCaption(str);
		SettingQueryFun(m_nAreaNo);
		break;
	case ASQuery:
		ASQueryFun();
		break;
			
	case ConfigChange:
		ConfigChangeFun();
		break;
	case SoftSwitch:
		m_nCursorWidth = 16;
		SoftSwitchFun();

		break;
	case SysStatus:
		break;

	case DIPara:
		BIParaFun();

		break;
	case DIFilterTime:
		BIFilterTimeFun();

		break;
	case DINor:
		BINorFun();

		break;
	case CIBase:


		break;
	case CIConst:
		CIConstFun();
		break;
	case SoePara:
		break;
//	case ActData:
//		break;
//	case AlarmData:
//		break;
//	case StartData:
//		break;
	case CF:
		CFFun();
		break;
	default:
		;
	}
//@	ConfigQuery, //10配置查询
//@	SettingQuery, //11定值查询
//@	ConfigChange, //12
//@	SoftSwitch, //13软压板
//@	fourteen, // 14
//@	SysStatus, //15
//@	DIPara, //16开入属性，电度或遥信
//@	DIFilterTime, //17去抖时间
//@	DINor, //18是否取反
//@	CIBase, //19电度底数
//@	CIConst, //脉冲常数
//@	SoePara, //21soe属性，是否记录
//@
//@	ActData, //22动作报告故障电量
//@	AlarmData, //23告警报告电量
//@	StartData, //24启动报告电量
	return FALSE;

}

//告警报告详情
CAlarmDetailDlg::~CAlarmDetailDlg()
{
}
CAlarmDetailDlg::CAlarmDetailDlg()
{
}

/****************************************************************************************
 * 
 * 功能描述: 设置实际要显示的故障电量个数，  计算对话框的页数
 * 
 * 参数说明: 
 *			- LONG Num
 *			  类型: 
 *			  说明: 实际显示的故障电量的个数
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
void CAlarmDetailDlg::SetFDNum(LONG Num)
{
	if(Num > MAX_FDNUM)
		Num = MAX_FDNUM;
	if(Num <= 0)
		return;

	LONG nPageNum = 1;

	//使控件有效
	for(int i=0; i<Num; i++)
		m_pObjList[6+i] = &m_Static[5+i];
	if(Num >4)
	{
		nPageNum = (Num-4)/10+1;
		if((Num-4)%10 != 0)
			nPageNum += 1;
	}

	SetPageNum(nPageNum);
	SetObjNum(5+Num);
}
//显示窗口并设置返回窗口
void CAlarmDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	Show(0,0);
}

//设置要显示的报告
BOOL CAlarmDetailDlg::ReadReport(LONG ItemIndex)
{
	char str[64];

	TAbsTime abstime;
	TSysTime systime;
	TRelayAlarmEv tAlarmReport;
	TRelayEventHead *pEvent;
	TRE_Disp tReDisp;
	int i;
	LONG rc;
	WORD Num;
	WORD inf;

	Init();
	
	//标题， 序号显示从1开始
	sprintf(str, "%s:%d", "告警报告", ItemIndex+1);
	SetCaption(str);

	rc	= DB_GetRelayAlarmReportNum(&Num);
//@	rc	= DB_GetRelayAlarmReportHead(&wHead);
//@	rc	= DB_GetRelayAlarmReportRead(&wRead);

	DB_ReadRelayAlarmReport(&tAlarmReport, ItemIndex);
	pEvent = &tAlarmReport.tEvent.tAction;

	inf = pEvent->nINF;
	abstime.Hi = pEvent->AbsTimeHi;
	abstime.Lo = pEvent->AbsTimeLo;
	AbsTimeTo(&abstime, &systime);
		
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		systime.Year, systime.Month, systime.Day, 
		systime.Hour, systime.Minute, systime.Second, systime.MSecond);

	DB_GetREDispByInf(inf, &tReDisp);
//@	if(tRelayActionEv.tEvent.tAction.bStatus)
//@		sprintf(str, "%02d %s %s%s", i, time, tReDisp.strName, "发出");
//@	else
//@		sprintf(str, "%02d %s %s%s", i, time, tReDisp.strName, "复归");
	
//	LONG FDNum = pEvent->FD_Num;
		
	m_Static[3].SetCaption(str); //  动作时间

	m_Static[2].SetCaption(tReDisp.strName); //  名称

	SetFDNum(pEvent->FD_Num);
		

	LONG 		RMIndex;
	DWORD 		dwValue;
	float		fValue;
	TRM_Disp	tRMDisp;
	BYTE		*pData;
	
	char sztmp[32];
	char strFormat[32];
	for(i= 0; i<pEvent->FD_Num; i++)
	{
		pData = (BYTE *)(pEvent+1)+i*(sizeof(WORD)+sizeof(float))+2;
		dwValue = MAKEDWORD(pData[0],pData[1],pData[2],pData[3]);
		fValue = *((float *)&dwValue);
			
		RMIndex = tReDisp.wRMList[i];
		DB_GetRMDisp(RMIndex, &tRMDisp); //得到该类型遥测的描述参数

		sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nSecDec, "f%s");

		sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strSecUnit);

//@	char sztmp[32];
//@	for(i= 0; i<pEvent->FD_Num; i++)
//@	{
//@		RMIndex = tReDisp.wRMList[i];
//@
//@		rc = DB_ReadRM(RMIndex, &lValue);
//@		rc = DB_GetRMDisp(RMIndex, &tRMPara);
//@
//@		wType = tRMPara.nType; //得到保护测量值的遥测类型号
//@		DB_GetAITofRM(wType, &taAITDisp); //得到该类型遥测的描述参数
//@
//@		sprintf(sztmp,"%s = %d %s", taAITDisp.strSymbol, lValue, taAITDisp.strFirUnit);
		
		//故障电量显示1次还是2次值
		m_Static[5+i].SetCaption(sztmp);
	}
	return TRUE;
}

BOOL CAlarmDetailDlg::Init()
{
	m_nCurPageIndex = 0;
	m_nCurTaborder = 0;

	SetCaption("动作报告： No.1");
	SetCaption("", 1);
	SetEscWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	SetHaveEnterFun(-1);
	SetHaveAddSubFun(0);
	SetHaveInitFun(2);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(13,30,83,45,0);
	m_Static[0].SetTabOrder(-5);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetTimer(-1);
	m_Static[0].SetCaption("动作时间");
	m_pObjList[1] = &m_Static[0];

	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(0,0,0,0);//;13,52,83,67,0);
	m_Static[1].SetTabOrder(-4);
	m_Static[1].SetFont(16);
	m_Static[1].SetOwner(this);
	m_Static[1].SetTimer(-1);
	m_Static[1].SetCaption(""); //相对时间");
	
	m_pObjList[2] = &m_Static[1];
	m_Static[2].SetDataSource(-1,0);
	m_Static[2].SetPosition(13,75,233,90,0);
	m_Static[2].SetTabOrder(-3);
	m_Static[2].SetFont(16);
	m_Static[2].SetOwner(this);
	m_Static[2].SetTimer(-1);
	m_Static[2].SetCaption("过流I段动作 ");
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(13,50,224,72,0); //109,52,224,72,0
	m_Static[3].SetTabOrder(-2);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetTimer(-1);
	m_Static[3].SetCaption("2008.01.02 13:13:23.123");
	m_pObjList[4] = &m_Static[3];

	m_Static[4].SetDataSource(-1,0);
	m_Static[4].SetPosition(0,0,0,0); //109,52,224,72,0);
	m_Static[4].SetTabOrder(-1);
	m_Static[4].SetFont(16);
	m_Static[4].SetOwner(this);
	m_Static[4].SetTimer(-1);
	m_Static[4].SetCaption(""); //0.01.600");
	m_pObjList[5] = &m_Static[4];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(5);
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;
	SetTimerSpan(0);
	

	//最大量初始化 用来显示故障电量的static
	int i;
	for(i=5; i<5+MAX_FDNUM; i++)
	{
		m_Static[i].SetDataSource(-1,0);
		m_Static[i].SetTabOrder(0-i-1);
		m_Static[i].SetFont(16);
		m_Static[i].SetOwner(this);
		m_Static[i].SetTimer(-1);
		m_pObjList[i+1] = NULL;

	}

	//第0页的4个控件位置
	m_Static[5].SetPosition(10,		30+25*3,	10+105,	30+25*3+17,	0);
	m_Static[6].SetPosition(105+20,	30+25*3,	105+20,	30+25*3+17,	0);
	m_Static[7].SetPosition(10,		30+25*4,	10+105,	30+25*4+17,	0);
	m_Static[8].SetPosition(105+20,	30+25*4,	20+105,	30+25*4+17,	0);
	
	int j=0;
	for(i=0; i<(MAX_FDNUM-4)/2; i++)
	{
		m_Static[9+2*i].SetPosition(10,		  30+25*j,	10+105,	30+25*j+17,	i/5+1);
		m_Static[9+2*i+1].SetPosition(105+20, 30+25*j,	105+20,	30+25*j+17,	i/5+1);
		j++;
		j = j%5;
	}

	return TRUE;
}
BEGIN_HMIMESSAGE_MAP(CAlarmDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CAlarmDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()



CCheckDlg::~CCheckDlg()
{
}
CCheckDlg::CCheckDlg()
{
}

BOOL CCheckDlg::OnEnter()
{
	if(CheckSystemState())
		m_Static[1].SetCaption("系统检测成功");
	else
		m_Static[1].SetCaption("系统检测失败，有错误");

	m_Static[1].OnPaint();
#ifndef __PL_WIN_SIMULATION__
	OS_Sleep(100);
#else
	Sleep(1000);
#endif
	return TRUE;
}

BOOL CCheckDlg::Init()
{
	SetCaption("自检");
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	SetHaveEnterFun(-1);
	SetHaveAddSubFun(0);
	SetHaveInitFun(-1);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(26,43,201,68);
	m_Static[0].SetTabOrder(-1);
	m_Static[0].SetOwner(this);
	m_Static[0].SetCaption("正在上电自检中....");
	m_pObjList[1] = &m_Static[0];
	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(31,93,126,113);
	m_Static[1].SetTabOrder(-2);
	m_Static[1].SetOwner(this);
	m_Static[1].SetCaption("*******状态 ");
	m_pObjList[2] = &m_Static[1];
	m_Static[2].SetDataSource(-1,0);
	m_Static[2].SetPosition(179,93,209,108);
	m_Static[2].SetTabOrder(-3);
	m_Static[2].SetOwner(this);
	m_Static[2].SetCaption("正常");
	m_pObjList[3] = &m_Static[2];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetObjNum(3);
	m_pActiveObj = m_pObjList[1];
	return TRUE;
}

//soedlg
CSOEDetailDlg::~CSOEDetailDlg()
{
}
CSOEDetailDlg::CSOEDetailDlg()
{
}

void CSOEDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	Show(0,0);
}

BOOL CSOEDetailDlg::ReadReport(LONG nSoeId)
{
	char str[64];
	TEventBI tEvent;
	TSysTime tSysTime;

	Init();
	
	sprintf(str, "%s:%02d", "SOE", nSoeId);
	
	SetCaption(str);
	DB_ReadSOE(&tEvent, nSoeId);

	AbsTimeTo(&(tEvent.stTime), &tSysTime);
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%03d", 
		tSysTime.Year, tSysTime.Month, tSysTime.Day, 
		tSysTime.Hour, tSysTime.Minute, tSysTime.Second, tSysTime.MSecond);

	m_Static[0].SetCaption(str);

	if(tEvent.bStatus)
	{
		sprintf(str, "%s:%d\t%s", "开入",tEvent.wNo, "合-->分");
		
	}
	else
	{
		sprintf(str, "%s:%d\t%s", "开入",tEvent.wNo, "分-->合");
	}
	
	m_Static[1].SetCaption(str);
	return TRUE;


}

BOOL CSOEDetailDlg::Init()
{
	SetCaption("SOE报告：No.5");
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	SetHaveEnterFun(-1);
	SetHaveAddSubFun(0);
	SetHaveInitFun(-1);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(20,46,220,69);
	m_Static[0].SetTabOrder(-1);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetCaption("2008.02.03.02 22:20:23.234");
	m_pObjList[1] = &m_Static[0];
	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(22,71,152,96);
	m_Static[1].SetTabOrder(-2);
	m_Static[1].SetFont(16);
	m_Static[1].SetOwner(this);
	m_Static[1].SetCaption("开入1  0-->1");
	m_pObjList[2] = &m_Static[1];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetObjNum(2);
	m_pActiveObj = m_pObjList[1];
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CSOEDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CSOEDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

//operation dlg
COperationDetailDlg::~COperationDetailDlg()
{
}
COperationDetailDlg::COperationDetailDlg()
{
}

void COperationDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	Show(0,0);
}

//设置要显示的操作记录号
BOOL COperationDetailDlg::ReadReport(LONG nItem)
{
	char str[32];
	TMiscEvent tEvent;
	TSysTime tSysTime;

		
	DB_Misc_Read(nItem, &tEvent);
	AbsTimeTo(&(tEvent.absTime) ,&tSysTime);


	Init();
	
	//标题
	sprintf(str, "%s:%d", "操作事项", nItem);
	SetCaption(str);
	
	//时间
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		tSysTime.Year, tSysTime.Month, tSysTime.Day, 
		tSysTime.Hour, tSysTime.Minute, tSysTime.Second, tSysTime.MSecond);
	m_Static[0].SetCaption(str);
	
	//内容
	m_Static[1].SetCaption(OperationStr[tEvent.nClass]);

	//value
	sprintf(str, "%d", tEvent.dwValue);
	m_Static[2].SetCaption(str);

	//attr
	sprintf(str, "%d", tEvent.dwAttr);
	m_Static[3].SetCaption(str);

	return TRUE;
}

BOOL COperationDetailDlg::Init()
{
	SetCaption("操作事项： No. 6");
	SetEscWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	SetHaveEnterFun(-1);
	SetHaveAddSubFun(0);
	SetHaveInitFun(-1);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(12,41,217,61);
	m_Static[0].SetTabOrder(-1);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetCaption("2008.02.03 11:23:24.234");
	m_pObjList[1] = &m_Static[0];
	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(12,70,142,95);
	m_Static[1].SetTabOrder(-2);
	m_Static[1].SetFont(16);
	m_Static[1].SetOwner(this);
	m_Static[1].SetCaption("定值投入");
	m_pObjList[2] = &m_Static[1];
	m_Static[2].SetDataSource(-1,0);
	m_Static[2].SetPosition(14,93,79,108);
	m_Static[2].SetTabOrder(0);
	m_Static[2].SetFont(16);
	m_Static[2].SetOwner(this);
	m_Static[2].SetCaption("第几路");
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(12,117,87,137);
	m_Static[3].SetTabOrder(0);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetCaption("值变化");
	m_pObjList[4] = &m_Static[3];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetObjNum(4);
	m_pActiveObj = m_pObjList[1];
	return TRUE;
}
BEGIN_HMIMESSAGE_MAP(COperationDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(COperationDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()





//动作报告详情
CActionDetailDlg::~CActionDetailDlg()
{
}
CActionDetailDlg::CActionDetailDlg()
{
}


//显示窗口并设置返回窗口
void CActionDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	Show(0,0);
}

void CActionDetailDlg::OnWndLeftRight(LONG LeftRight)
{
	if(LeftRight >0)
		m_nSubItem++;
	else
		m_nSubItem--;

	CHmiWnd* pWnd = GetEscWnd();
	ReadReport(m_nItem, m_nSubItem);

	SetEscWnd(pWnd);
	OnPaint();

}

//设置要显示的报告,根据报告读取编号读取报告内容
BOOL CActionDetailDlg::ReadReport(LONG ItemIndex, LONG subItemIndex)
{
	char str[64];
	TAbsTime abstime;
	TSysTime systime;
	TRelayActionEv tRelayActionEv;
	TRelayEventHead *pEvent;
	TRE_Disp tReDisp;
	int i;
	LONG rc;
	WORD Num; //总报告个数
	WORD ItemNum; //报告的分报告个数
	WORD wItemLen;
	WORD FDNum; //故障电量个数
	WORD inf;
	WORD offset =0;

	Init();
	
	rc	= DB_GetRelayActionReportNum(&Num);

	//查询目标大于总报告数,显示第一个
	if(Num <= ItemIndex)
	{
		ItemIndex = 0;
	}

	m_nItem = ItemIndex; //当前显示的报告号

	//找到总报告
	DB_ReadRelayActionReport(&tRelayActionEv, ItemIndex);
	pEvent = &tRelayActionEv.tEvent.tAction; 
	
	
	ItemNum = tRelayActionEv.tEvent.Num; //得到分报告个数


	//分报告号为负数则显示最后一个
	if(subItemIndex <0)
		subItemIndex = ItemNum-1;
	
	//分报告大于最大值则显示第一个
	if(subItemIndex>= ItemNum)
	{
		subItemIndex = 0;
	}

	m_nSubItem = subItemIndex;

	//标题， 序号显示从1开始
	sprintf(str, "%s:%d--%d/%d", "保护动作", ItemIndex+1, subItemIndex+1, ItemNum);
	SetCaption(str);

	//定位分报告
	offset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
	for(int j=0; j<subItemIndex; j++)
	{
		FDNum = pEvent->FD_Num;
		wItemLen = sizeof(TRelayEventHead) + FDNum*(sizeof(WORD)+sizeof(float));
		
		offset += wItemLen;
		
		//计算下一条分报告的位置
		pEvent = (TRelayEventHead *)(tRelayActionEv.aEvent+offset);
	}



	inf = pEvent->nINF;
	abstime.Hi = pEvent->AbsTimeHi;
	abstime.Lo = pEvent->AbsTimeLo;
	AbsTimeTo(&abstime, &systime);
		
		
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		systime.Year, systime.Month, systime.Day, 
		systime.Hour, systime.Minute, systime.Second, systime.MSecond);

	DB_GetREDispByInf(inf, &tReDisp);
//@	if(tRelayActionEv.tEvent.tAction.bStatus)
//@		sprintf(str, "%02d %s %s%s", i, time, tReDisp.strName, "发出");
//@	else
//@		sprintf(str, "%02d %s %s%s", i, time, tReDisp.strName, "复归");
	
//	LONG FDNum = pEvent->FD_Num;
		
	m_Static[3].SetCaption(str); //  动作时间

	m_Static[2].SetCaption(tReDisp.strName); //  名称

	SetFDNum(pEvent->FD_Num);
	
	LONG 		RMIndex;
	DWORD 		dwValue;
	float		fValue;
	TRM_Disp 	tRMDisp;
	BYTE		*pData;
	
	char sztmp[32];
	char strFormat[32];
	for(i= 0; i<pEvent->FD_Num; i++)
	{
		pData = (BYTE *)(pEvent+1)+i*(sizeof(WORD)+sizeof(float))+2;
		dwValue = MAKEDWORD(pData[0],pData[1],pData[2],pData[3]);
		fValue = *((float *)&dwValue);
			
		RMIndex = tReDisp.wRMList[i];
		DB_GetRMDisp(RMIndex, &tRMDisp); //得到该类型遥测的描述参数

		sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nSecDec, "f%s");
		//%

		sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strSecUnit);

//		sprintf(sztmp,"%s=%5.3f%s", taAITDisp.strSymbol, fValue, taAITDisp.strFirUnit);
		
		//故障电量显示1次还是2次值
		m_Static[5+i].SetCaption(sztmp);
		
	}
	return TRUE;
}

/****************************************************************************************
 * 
 * 功能描述: 设置实际要显示的故障电量个数，  计算对话框的页数
 * 
 * 参数说明: 
 *			- LONG Num
 *			  类型: 
 *			  说明: 实际显示的故障电量的个数
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
void CActionDetailDlg::SetFDNum(LONG Num)
{
	if(Num > MAX_FDNUM)
		Num = MAX_FDNUM;
	if(Num <= 0)
		return;

	LONG nPageNum = 1;

	//使控件有效
	for(int i=0; i<Num; i++)
		m_pObjList[6+i] = &m_Static[5+i];
	if(Num >4)
	{
		nPageNum = (Num-4)/10+1;
		if((Num-4)%10 != 0)
			nPageNum += 1;
	}

	SetPageNum(nPageNum);
	SetObjNum(5+Num);
}

BOOL CActionDetailDlg::Init()
{
	m_nCurPageIndex = 0;
	m_nCurTaborder = 0;

	SetCaption("动作报告： No.1");
	SetCaption("", 1);
	SetEscWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(Function);
	SetLROffset(1);
	SetHaveEnterFun(-1);
	SetHaveAddSubFun(0);
	SetHaveInitFun(2);
	SetHaveLeftRightFun(TRUE);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(13,30,83,45,0);
	m_Static[0].SetTabOrder(-5);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetTimer(-1);
	m_Static[0].SetCaption("动作时间");
	m_pObjList[1] = &m_Static[0];
	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(0,0,0,0);//;13,52,83,67,0);
	m_Static[1].SetTabOrder(-4);
	m_Static[1].SetFont(16);
	m_Static[1].SetOwner(this);
	m_Static[1].SetTimer(-1);
	m_Static[1].SetCaption(""); //相对时间");
	
	m_pObjList[2] = &m_Static[1];
	m_Static[2].SetDataSource(-1,0);
	m_Static[2].SetPosition(13,75,233,90,0);
	m_Static[2].SetTabOrder(-3);
	m_Static[2].SetFont(16);
	m_Static[2].SetOwner(this);
	m_Static[2].SetTimer(-1);
	m_Static[2].SetCaption("过流I段动作 ");
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(13,50,224,72,0); //109,52,224,72,0
	m_Static[3].SetTabOrder(-2);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetTimer(-1);
	m_Static[3].SetCaption("2008.01.02 13:13:23.123");
	m_pObjList[4] = &m_Static[3];

	m_Static[4].SetDataSource(-1,0);
	m_Static[4].SetPosition(0,0,0,0); //109,52,224,72,0);
	m_Static[4].SetTabOrder(-1);
	m_Static[4].SetFont(16);
	m_Static[4].SetOwner(this);
	m_Static[4].SetTimer(-1);
	m_Static[4].SetCaption(""); //0.01.600");
	m_pObjList[5] = &m_Static[4];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(5);
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;
	SetTimerSpan(0);
	

	//最大量初始化 用来显示故障电量的static
	int i;
	for(i=5; i<5+MAX_FDNUM; i++)
	{
		m_Static[i].SetDataSource(-1,0);
		m_Static[i].SetTabOrder(0-i-1);
		m_Static[i].SetFont(16);
		m_Static[i].SetOwner(this);
		m_Static[i].SetTimer(-1);
		m_pObjList[i+1] = NULL;

	}

	//第0页的4个控件位置
	m_Static[5].SetPosition(10,		30+25*3,	10+105,	30+25*3+17,	0);
	m_Static[6].SetPosition(105+20,	30+25*3,	105+20,	30+25*3+17,	0);
	m_Static[7].SetPosition(10,		30+25*4,	10+105,	30+25*4+17,	0);
	m_Static[8].SetPosition(105+20,	30+25*4,	20+105,	30+25*4+17,	0);
	
	int j=0;
	for(i=0; i<(MAX_FDNUM-4)/2; i++)
	{
		m_Static[9+2*i].SetPosition(10,		  30+25*j,	10+105,	30+25*j+17,	i/5+1);
		m_Static[9+2*i+1].SetPosition(105+20, 30+25*j,	105+20,	30+25*j+17,	i/5+1);
		j++;
		j = j%5;
	}

	return TRUE;
}
BEGIN_HMIMESSAGE_MAP(CActionDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CActionDetailDlg)
	ON_HMIWND_LEFTRIGHT()
///}}HMI_MSG
END_HMIMESSAGE_MAP()

void CHmiDataList::ListCIFun()
{
	TCIPara tCIPara;
	LONG num, i;
	char str[64];
	DWORD val;

	num = DB_GetCINum();
	for(i =0; i<num; i++)
	{
		DB_GetCIPara(i, &tCIPara);
		DB_ReadCI(i, &val);
		sprintf(str, "%s			%d", tCIPara.strSymbol, val);
		AddString(str);
	}

}

void CHmiDataList::OperationFun()
{
	TMiscEvent tEvent;
	WORD num;
	char str[64];
	char strTime[64];
	int i;
	LONG rc;
	WORD wHead,IsEmpty;
	TSysTime out;

	rc	= DB_Misc_GetNum(&num);
	rc	= DB_Misc_GetHead(&wHead);
	IsEmpty = DB_Misc_IsEmpty();
	for(i=0; i<num; i++)
	{
		
		DB_Misc_Read(i, &tEvent);
		AbsTimeTo(&(tEvent.absTime) ,&out);

		sprintf(strTime, "%02d/%02d %02d:%02d", 
			 out.Month, out.Day, out.Hour, out.Minute);

		//序号显示从1开始
		sprintf(str, "%02d %s %s",  i+1, strTime,OperationStr[tEvent.nClass]);

//@		switch(tEvent.nClass)
//@		{
//@		case MISC_CLASS_RESET:
//@			sprintf(str, "%02d %s %s",  i, strTime,"系统复位" );
//@			break;
//@		case MISC_CLASS_DOWNFILE:
//@			sprintf(str, "%02d %s %s",  i,   strTime,"文件下装" );
//@			break;
//@		case MISC_CLASS_COMM:
//@			sprintf(str, "%02d %s % s",  i,  strTime,"通信事件");
//@			break;
//@		case MISC_CLASS_BO:
//@			sprintf(str, "%02d %s % s",  i, strTime, "遥控事件");
//@			break;
//@		case MISC_CLASS_AO:
//@			sprintf(str, "%02d %s % s",  i,  strTime, "遥调事件");
//@			break;
//@		case MISC_CLASS_SET:
//@			sprintf(str, "%02d %s % s",  i, strTime, "修改定值");
//@			break;
//@		case MISC_CLASS_SYNC:
//@			
//@			break;
//@		case MISC_CLASS_SETAREA:
//@			sprintf(str, "%02d %s % s",  i,  strTime, "改定值区号");
//@			break;
//@		case MISC_CLASS_SW:
//@			sprintf(str, "%02d %s % s",  i, strTime, "修改软压板");
//@			break;
//@		case MISC_CLASS_CFG:
//@			sprintf(str, "%02d %s % s",  i, strTime, "修改配置");
//@			break;
//@		case MISC_CLASS_CF:
//@			sprintf(str, "%02d %s % s",  i,  strTime, "系数修改");
//@			
//@			break;
//@		case MISC_CLASS_PARA:
//@			sprintf(str, "%02d %s % s",  i, strTime, "修改参数");
//@			break;
//@		case MISC_CLASS_POWER:
//@			sprintf(str, "%02d %s % s",  i,  strTime, "系统上电");
//@			break;
//@		case MISC_CLASS_CLEARCI:
//@			sprintf(str, "%02d %s % s",  i, strTime, "设电度底数");
//@			break;
//@		case MISC_CLASS_MODIAI:
//@			break;
//@		case MISC_CLASS_MODIBI:
//@			break;
//@		case MISC_CLASS_MODICI:
//@			break;
//@		}
		AddString(str);
	}
}

void CHmiDataList::SoeFun()
{
	TEventBI tSOE;
	WORD num;
	char str[64];
	int i;
	LONG rc;
	WORD wHead, wRead, IsEmpty;
	TSysTime out;
	char strStatus[8];

	rc	= DB_GetSOENum(&num);
	rc	= DB_GetSOEHead(&wHead);
	rc	= DB_GetSOERead(&wRead);
	IsEmpty = DB_SOEIsEmpty();
	if(m_nCurBaseIndex<0)
		m_nCurBaseIndex = num-1;
	WORD wIndex;
	int j=0;
	for(i=m_nPageSize;i>0;i--)
	{
		wIndex = m_nCurBaseIndex+i;
		
		DB_ReadSOE(&tSOE,wIndex);
		//			DB_IncSOERead();
		//需要决定时间和相对时间转换函数
		AbsTimeTo(&(tSOE.stTime) ,&out);
		if(tSOE.bStatus)
			sprintf(strStatus, "%s", "0-->1");
		else
			sprintf(strStatus, "%s", "1-->0");

		sprintf(str, "%02d)%02d/%02d/%02d %02d:%02d:%02d:%03d IN%02d %s", wIndex, 
			out.Year-2000, out.Month, out.Day, out.Hour, out.Minute, out.Second, out.MSecond,
			tSOE.wNo, strStatus);
		AddString(str);
		//可能是soe编号，
		SetItemData(j++,wIndex);
	}
	SetFont(12);
	


}

void CHmiDataList::ConfigQueryFun()
{
	WORD num;
	int i;
	LONG rc;
	BYTE CFGStatus;
	TCFG_Disp tCfgDisp;
	char str[64];

	//配置查看
	num  = DB_GetCFGNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-2));

	

	for(i=0;i<num;i++)
	{
		rc = DB_GetCFGStatus(m_nAreaNo,i, CFGStatus);
		rc = DB_GetCFGDisp(i, &tCfgDisp);
		if(CFGStatus == CFG_STATUS_SET)
		{
			sprintf(str, "%s", "投");

		}
		else
		{
			sprintf(str, "%s", "退");
		}
		AddString(tCfgDisp.strName);
		SetItemData(i, CFGStatus);
		SetItemText(i, 1, str);
	}
//@	sprintf(str, "配置查看：%d区", m_nAreaNo);
//@	m_pOwner->SetCaption(str);
//@	m_pOwner->DrawCaption();


}

void CHmiDataList::SoftSwitchFun()
{
	WORD num;
	char str[64];
	int i;
	LONG rc;
	BYTE Status;
	TSW_Disp tswDisp;

	num = DB_GetSWNum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-2));
	
	for(i=0;i<num;i++)
	{
		rc = DB_GetSWStatus(i, Status);
		rc = DB_GetSWDisp(i, &tswDisp);
		if(Status ==SW_STATUS_SET)
		{
			sprintf(str, "%s",  "投");
		}
		else
		{
			sprintf(str, "%s",  "退");
		}
		AddString(tswDisp.strName);
		SetItemText(i,1, str);
		
	}

}

void CHmiDataList::BIParaFun()
{
	TDIPara tDIPara;
	WORD num;
	char str[64];
	int i;
	LONG rc;

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-5));

	num = DB_GetBINum() + DB_GetCINum();
	for(i = 0; i<num; i++)
	{
		rc = DB_GetDIPara(i, &tDIPara);
		sprintf(str, "%02d", i);
		AddString(str);

		if(tDIPara.wClass == DI_TYPE_PLUS)
			sprintf(str, "%s", "电度");
		else
			sprintf(str, "%s",  "遥信");
		SetItemText(i, 1, str);
	}

}

void CHmiDataList::BIFilterTimeFun()
{
	TDIPara tDIPara;
	WORD num;
	char str[64];
	int i;
	LONG rc;

	num = DB_GetBINum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-4));
	
	//对应到遥信，取得遥信名称

	for(i = 0; i<num; i++)
	{
		rc = DB_GetDIPara(i, &tDIPara);
		sprintf(str, "YX%d",  i);
		AddString(str);

		sprintf(str, "%d",  tDIPara.wFilterTime);
		SetItemData(i, tDIPara.wFilterTime);
		SetItemText(i, 1, str);
	}
	
}

void CHmiDataList::BINorFun()
{
	TBIPara tBIPara;
	WORD num;
	char str[64];
	int i;
	LONG rc;

	num = DB_GetBINum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-4));


	num = DB_GetBINum();
	for(i = 0; i<num; i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		
		
		if(tBIPara.wControl&BI_ATTR_CONVERT)
			sprintf(str, "%s"  , "取反");
		else
			sprintf(str, "%s" , "不反");
		AddString(tBIPara.strSymbol);
		SetItemData(i, tBIPara.wControl);
		SetItemText(i, 1, str);
	}
	
}

void CHmiDataList::CIConstFun()
{
	TCIPara tCIPara;
	WORD num;
	char str[64];
	int i;

	num = DB_GetCINum();
	for(i =0; i<num; i++)
	{
		DB_GetCIPara(i, &tCIPara);
		sprintf(str, "%s\t%d", tCIPara.strSymbol, tCIPara.dwConst);
		AddString(str);
	}


}
void CHmiDataList::ASQueryFun()
{
	LONG Num,rc, nItem;
	float	  fSETValue[256];
	WORD i;
	char strFormat[32];
	char str[32];

	TSysSET_Disp tSysSETDisp;
	
	Num = DB_GetSystemSETNum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-10));

	DB_GetSystemSETFloatValue(SET_VALUE_ALL, fSETValue);
	for(i=0; i<Num; i++)
	{
		DB_GetSysSETDisp(i, &tSysSETDisp);
		nItem= AddString(tSysSETDisp.strCHName);
		
		sprintf(strFormat, "%s%d%s", "%.", tSysSETDisp.nDec, "f%s"); //"%.2f%s"
		sprintf(str, strFormat,  fSETValue[i], tSysSETDisp.strUnit);
		
		SetItemText(nItem, 1, str);
	}
	
}
void CHmiDataList::CFFun()
{
	WORD i;
	LONG Num,rc;
	LONG lCfValueList[256];
	Num = DB_GetCFNum();
	TCF_Disp tCFDisp;
	LONG nItem;
	char str[32];
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-10));

	
	DB_GetCFValue(CF_VALUE_ALL, lCfValueList);
	for(i=0; i<Num; i++)
	{
		DB_GetCFDisp(i, &tCFDisp);
		nItem= AddString(tCFDisp.strSymbol);
	
		sprintf(str, "%05d",  lCfValueList[i]);
		SetItemText(nItem, 1, str);
	}
	
}


void CHmiDataList::SettingQueryFun(LONG nAreaNo)
{
	LONG Num,rc;
	int i,j;
	float	  fSETValue[256];
	TSET_Disp	tSETDisp;
	char str[64];
	char strFormat[32];
	BYTE *byCFGIndexList;
	BOOL	bCFGSet,bRelation;
	
	DB_GetSETFloatValue(nAreaNo, SET_VALUE_ALL, fSETValue);	
	Num = DB_GetSETNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-10));

	//只显示配置投入了的定值
	BYTE CFGStatus;

	WORD cfgNum = DB_GetCFGNum();
	LONG cfgWordNum = cfgNum/32;
	if(cfgNum%32)
	{
		cfgWordNum += 1;
	}
	WORD nCfgNum; //定值关联的配置个数

//定值的前几个是配置
	for(i=cfgWordNum; i<Num; i++)
	{
		DB_GetSETDisp(i, &tSETDisp);
		byCFGIndexList = &(tSETDisp.CFGList[0]);
		nCfgNum = tSETDisp.nCFG;

		bRelation = FALSE;
		bCFGSet = FALSE;
		

		//检查定值是否需要显示(无关联 或者 投入)
		if(nCfgNum >0)
		{
			for(j=0;j<nCfgNum;j++)
			{
				if(byCFGIndexList[j] == SETCFG_INVALID)
					continue;
			
				bRelation = TRUE;
			
				rc = DB_GetCFGStatus(m_nAreaNo,byCFGIndexList[j], CFGStatus);
				if((rc == DB_ERR_OK)&&(CFGStatus == CFG_STATUS_SET)) //配置投入则显示定值
					{
						bCFGSet = TRUE;
						break;
					}
			}
		}

		// 关联 而且 未投入
		if(bRelation && (!bCFGSet))
		{
			//判断关联配置是否投入
			continue;
		}
		j = AddString(tSETDisp.strCHName);
//		SetItemData(i, CFGStatus);

		sprintf(strFormat, "%s%d%s", "%.", tSETDisp.nDec, "f%s"); //"%.2f%s"
		sprintf(str, strFormat,  fSETValue[i], tSETDisp.strUnit);

		SetItemText(j, 1, str);

	}
}

void CHmiDataList::ListBIFun()
{
	LONG Num, i;
	BYTE val;
	TBIPara tBIPara;
	Num = DB_GetBINum();
	char str[64];
	for(i=0; i<Num; i++)
	{
		DB_GetBIPara(i,&tBIPara);
		DB_ReadBI(i, &val);
		if(val == DB_BI_OPEN)
		{
			sprintf(str,"%d%s \t\t\t%s", i, tBIPara.strSymbol , "□");
			
		}
		else
		{
			sprintf(str,"%d%s \t\t\t%s", i, tBIPara.strSymbol, "■");
		}
		AddString(str);
	}
}


void CHmiDataList::SaveFilterTime()
{
	TDIPara tDIPara;
	char str[64];
	LONG data;
	LONG rc;
	for(int i=0; i< m_nItemCount; i++)
	{
		strcpy(str,  GetItemText(i,1));
		data = atol(str);
		if(data != GetItemData(i))
		{
			DB_GetDIPara(i,&tDIPara);
			tDIPara.wFilterTime = data;
			rc = DB_WriteDIPara(i, &tDIPara);
			rc = DB_WriteDIParaConfirm(i);

		}

	}

}

void CHmiDataList::OperationDetail()
{
	LONG nItem;
	nItem = GetCurSel();
	m_OperationDetailDlg.ReadReport(nItem);

	m_OperationDetailDlg.DoModal(m_pOwner);
}

void CHmiDataList::SoeDetail()
{
//@	LONG nSoeId, nItem;
//@	nItem = GetCurSel();
//@	nSoeId = GetItemData(nItem);
//@
//@	m_SoeDetaildlg.ReadReport(nSoeId);
//@	m_SoeDetaildlg.DoModal(m_pOwner);
	

	
}
BOOL CHmiDataList::StartDetail(LONG ItemIndex)
{

	m_StartDetailDlg.ReadReport(ItemIndex);
	m_StartDetailDlg.DoModal(m_pOwner);
	return TRUE;
}


//@BOOL CHmiDataList::AlarmDetail(LONG ItemIndex)
//@{
//@
//@	m_AlarmDetailDlg.ReadReport(ItemIndex);
//@	m_AlarmDetailDlg.DoModal(m_pOwner);
//@	return TRUE;
//@}
/****************************************************************************************
 * 
 * 功能描述: 显示动作报告详情
 * 
 * 参数说明: 
 *			- LONG ItemIndex
 *			  类型: 
 *			  说明: 动作报告编号
 *
 *			- LONG subItemIndex
 *			  类型: 
 *			  说明: 在该报告中的分报告编号
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
//@BOOL CHmiDataList::ActionDetail(LONG ItemIndex, LONG subItemIndex)
//@{
//@	WORD offset =0;
//@
//@	m_ActionDetailDlg.ReadReport(ItemIndex, subItemIndex);
//@	
//@	m_ActionDetailDlg.DoModal(m_pOwner);
//@	return true;
//@}

void CHmiDataList::StartFun()
{
	char str[64];
	char time[32];
	TAbsTime abstime;
	TSysTime systime;
	TRelayStartEv   tStartReport;
	TRelayEventHead *pEvent;
	TRE_Disp tReDisp;
	int i;
	LONG rc, inf, index;
	WORD Num;

	rc	= DB_GetRelayStartReportNum(&Num);

	for(i=0; i<Num; i++)
	{
		DB_ReadRelayStartReport(&tStartReport, i);
		pEvent = &tStartReport.tEvent.tAction;

		inf = pEvent->nINF;
		abstime.Hi = pEvent->AbsTimeHi;
		abstime.Lo = pEvent->AbsTimeLo;
		AbsTimeTo(&abstime, &systime);
		
		
		sprintf(time, "%02d/%02d %02d:%02d",   systime.Month, systime.Day,
			systime.Hour, systime.Minute);

		DB_GetREDispByInf(inf, &tReDisp);

	
		//序号显示从1开始
		sprintf(str, "%02d %s %s%s", i+1, time, tReDisp.strName, tr("启动"));	

		index = AddString(str);
		SetItemData(index, i);
	}
}

//08-12-24:17:15 改為列表只显示总报告， 不显示分报告
void CHmiDataList::ActionFun()
{
	char str[64];
	char time[32];
	TAbsTime abstime;
	TSysTime systime;
	TRelayActionEv tRelayActionEv;
	TRelayEventHead *pEvent;
	TRE_Disp tReDisp;
	int i;
	LONG rc;
	WORD Num; //总报告个数
	WORD ItemNum; //报告的分报告个数
	WORD wItemLen;
	WORD FDNum; //故障电量个数
	WORD inf;
	WORD offset =0;
	LONG index;
	WORD wActionIndex; //在列表中数据中保存动作索引号， 高字节代表报告序号， 低字节代表分报告序号
	rc	= DB_GetRelayActionReportNum(&Num);
//@	rc	= DB_GetRelayActionReportHead(&wHead);
//@	rc	= DB_GetRelayActionReportRead(&wRead);

	for(i=0;i<Num;i++)
	{
		DB_ReadRelayActionReport(&tRelayActionEv, i);
		pEvent = &tRelayActionEv.tEvent.tAction; //指向某个分报告

//@		offset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
//@
//@		ItemNum = tRelayActionEv.tEvent.Num; //得到分报告个数
//@
//@		for(int j=0; j<ItemNum; j++)
//@		{
			FDNum = pEvent->FD_Num;
//@			wItemLen = sizeof(TRelayEventHead) + FDNum*(sizeof(WORD)+sizeof(float));

			inf = pEvent->nINF;
			abstime.Hi = pEvent->AbsTimeHi;
			abstime.Lo = pEvent->AbsTimeLo;
			AbsTimeTo(&abstime, &systime);

			
			sprintf(time, "%02d/%02d %02d:%02d",   systime.Month, systime.Day,
				systime.Hour, systime.Minute);
			DB_GetREDispByInf(inf, &tReDisp);

			//序号显示从1开始，以符合用户习惯
			if(tRelayActionEv.tEvent.tAction.bStatus)
				sprintf(str, "%02d %s %s%s", i+1, time, tReDisp.strName, tr("发出"));
			else
				sprintf(str, "%02d %s %s%s", i+1, time, tReDisp.strName, tr("复归"));
			
			index = AddString(str);

			offset += wItemLen;

			wActionIndex = MAKEWORD(0,i);

			SetItemData(index, wActionIndex);

//@			//计算下一条分报告的位置
//@			pEvent = (TRelayEventHead *)(tRelayActionEv.aEvent+offset);
//@		}
	}
}

void CHmiDataList::AlarmFun()
{
	char str[64];
	char time[32];
	TAbsTime abstime;
	TSysTime systime;
	TRelayAlarmEv tAlarmReport;
	TRE_Disp tReDisp;
	int i;
	LONG rc;
	WORD Num;
	WORD inf;
	rc	= DB_GetRelayAlarmReportNum(&Num);
	LONG nItem;
//@	rc	= DB_GetRelayAlarmReportHead(&wHead);
//@	rc	= DB_GetRelayAlarmReportRead(&wRead);

	//告警报告无分报告
	for(i=0;i<Num;i++)
	{
		DB_ReadRelayAlarmReport(&tAlarmReport, i);
		inf = tAlarmReport.tEvent.tAction.nINF;
		abstime.Hi = tAlarmReport.tEvent.tAction.AbsTimeHi;
		abstime.Lo = tAlarmReport.tEvent.tAction.AbsTimeLo;
		AbsTimeTo(&abstime, &systime);

		sprintf(time, "%02d/%02d %02d:%02d",   systime.Month, systime.Day,
			systime.Hour, systime.Minute);
		DB_GetREDispByInf(inf, &tReDisp);

		//显示从1开始
		if(tAlarmReport.tEvent.tAction.bStatus)
			sprintf(str, "%02d %s %s%s", i+1, time, tReDisp.strName, tr("发出"));
		else
			sprintf(str, "%02d %s %s%s", i+1, time, tReDisp.strName, tr("复归"));


		nItem = AddString(str);
		SetItemData(nItem,i);
	}


}

void CHmiDataList::ChgQueryAera(BOOL isAdd)
{
	if(isAdd)
	{
		m_nAreaNo++;
		m_nAreaNo %= m_nAllArea;
	}
	else
	{
		m_nAreaNo += m_nAllArea;
		m_nAreaNo--;
		m_nAreaNo %= m_nAllArea;
	}

}

void CHmiDataList::ConfigChangeFun()
{
	WORD num;
	int i;
	LONG rc;
	BYTE CFGStatus;
	TCFG_Disp tCfgDisp;
	char str[64];


	//配置查看
	num  = DB_GetCFGNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-2));
	

	for(i=0;i<num;i++)
	{
		rc = DB_GetCFGStatus(m_nAreaNo,i, CFGStatus);
		rc = DB_GetCFGDisp(i, &tCfgDisp);
		if(CFGStatus == CFG_STATUS_SET)
		{
			sprintf(str, "%s", "投");

		}
		else
		{
			sprintf(str, "%s", "退");
		}
		AddString(tCfgDisp.strName);
		SetItemData(i, CFGStatus);
		SetItemText(i, 1, str);
	}
//@	sprintf(str, "配置修改：%d区", m_nAreaNo);
//@	m_pOwner->SetCaption(str);
//@	m_pOwner->DrawCaption();
}

BOOL CHmiDataList::ChgBoolValue()
{
	char strOn[3], strOff[3];
	sprintf(strOn, "%s", "投");
	sprintf(strOff, "%s", "退");
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	if( (m_nItemTxt[sel][pos+m_nCursorPos] == strOn[0]) &&
		(m_nItemTxt[sel][pos+m_nCursorPos+1] == strOn[1]) )
	{
		m_nItemTxt[sel][pos+m_nCursorPos] = strOff[0];
		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOff[1];
		
	}
	else
	{
		m_nItemTxt[sel][pos+m_nCursorPos] = strOn[0];
		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOn[1];
		
	}
	return TRUE;

}

BOOL CHmiDataList::ChgBIParaValue()
{
	char strYX[5], strDD[5];
	sprintf(strYX, "%s", "遥信");
	sprintf(strDD, "%s", "电度");
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	if( (m_nItemTxt[sel][pos+m_nCursorPos] == strYX[0]) &&
		(m_nItemTxt[sel][pos+m_nCursorPos+1] == strYX[1]) )
	{
		strcpy(&(m_nItemTxt[sel][pos+m_nCursorPos]),  strDD);
//@		m_nItemTxt[sel][pos+m_nCursorPos] = strOff[0];
//@		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOff[1];
		
	}
	else
	{
		strcpy(&(m_nItemTxt[sel][pos+m_nCursorPos]),  strYX);

//@		m_nItemTxt[sel][pos+m_nCursorPos] = strOn[0];
//@		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOn[1];
		
	}
	return TRUE;

}

BOOL CHmiDataList::ChgFilterTime()
{
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	if(m_nItemTxt[sel][pos+m_nCursorPos]== '9')
			m_nItemTxt[sel][pos+m_nCursorPos] = '0';
	else 
		m_nItemTxt[sel][pos+m_nCursorPos]++;

	return TRUE;

}
BOOL CHmiDataList::ChgSWValue() //更改软压板值
{
	char strOn[3], strOff[3];
	sprintf(strOn, "%s", "投");
	sprintf(strOff, "%s", "退");
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	if( (m_nItemTxt[sel][pos+m_nCursorPos] == strOn[0]) &&
		(m_nItemTxt[sel][pos+m_nCursorPos+1] == strOn[1]) )
	{
		m_nItemTxt[sel][pos+m_nCursorPos] = strOff[0];
		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOff[1];
		CHmiApp::m_byNewSoftSwitch[sel] = SW_STATUS_CUT;
		
	}
	else
	{
		m_nItemTxt[sel][pos+m_nCursorPos] = strOn[0];
		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOn[1];
		CHmiApp::m_byNewSoftSwitch[sel] = SW_STATUS_SET;
		
	}
	return TRUE;
}

BOOL CHmiDataList::ChgCFValue(LONG offset)
{
	
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	m_nItemTxt[sel][pos+m_nCursorPos] = offset + m_nItemTxt[sel][pos+m_nCursorPos] +10;
 	m_nItemTxt[sel][pos+m_nCursorPos] = m_nItemTxt[sel][pos+m_nCursorPos]%10;
	return TRUE;
}

//把对配置的更改缓冲起来
BOOL CHmiDataList::ChgCfgValue()
{
	char strOn[3], strOff[3];
	sprintf(strOn, "%s", "投");
	sprintf(strOff, "%s", "退");
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	if( (m_nItemTxt[sel][pos+m_nCursorPos] == strOn[0]) &&
		(m_nItemTxt[sel][pos+m_nCursorPos+1] == strOn[1]) )
	{
		m_nItemTxt[sel][pos+m_nCursorPos] = strOff[0];
		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOff[1];
		CHmiApp::m_byNewCfg[sel] = CFG_STATUS_CLR;
		
	}
	else
	{
		m_nItemTxt[sel][pos+m_nCursorPos] = strOn[0];
		m_nItemTxt[sel][pos+m_nCursorPos+1] = strOn[1];
		CHmiApp::m_byNewCfg[sel] = CFG_STATUS_SET;
		
	}
	return TRUE;
}

//--------------启动报告
//启动报告详情
CStartDetailDlg::~CStartDetailDlg()
{
}
CStartDetailDlg::CStartDetailDlg()
{
}

/****************************************************************************************
 * 
 * 功能描述: 设置实际要显示的故障电量个数，  计算对话框的页数
 * 
 * 参数说明: 
 *			- LONG Num
 *			  类型: 
 *			  说明: 实际显示的故障电量的个数
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
void CStartDetailDlg::SetFDNum(LONG Num)
{
	if(Num > MAX_FDNUM)
		Num = MAX_FDNUM;
	if(Num <= 0)
		return;

	LONG nPageNum = 1;

	//使控件有效
	for(int i=0; i<Num; i++)
		m_pObjList[6+i] = &m_Static[5+i];
	if(Num >4)
	{
		nPageNum = (Num-4)/10+1;
		if((Num-4)%10 != 0)
			nPageNum += 1;
	}

	SetPageNum(nPageNum);
	SetObjNum(5+Num);
}
//显示窗口并设置返回窗口
void CStartDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	Show(0,0);
}

//设置要显示的报告
BOOL CStartDetailDlg::ReadReport(LONG ItemIndex)
{
	char str[64];

	TAbsTime abstime;
	TSysTime systime;
	TRelayStartEv tStartReport;
	TRelayEventHead *pEvent;
	TRE_Disp tReDisp;
	int i;
	LONG rc;
	WORD Num;
	WORD inf;

	Init();
	
	//标题，显示序号从1开始
	sprintf(str, "%s:%d", "启动报告", ItemIndex+1);
	SetCaption(str);

	rc	= DB_GetRelayStartReportNum(&Num);
//@	rc	= DB_GetRelayAlarmReportHead(&wHead);
//@	rc	= DB_GetRelayAlarmReportRead(&wRead);

	DB_ReadRelayStartReport(&tStartReport, ItemIndex);
	pEvent = &tStartReport.tEvent.tAction;

	inf = pEvent->nINF;
	abstime.Hi = pEvent->AbsTimeHi;
	abstime.Lo = pEvent->AbsTimeLo;
	AbsTimeTo(&abstime, &systime);
		
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		systime.Year, systime.Month, systime.Day, 
		systime.Hour, systime.Minute, systime.Second, systime.MSecond);

	DB_GetREDispByInf(inf, &tReDisp);
//@	if(tRelayActionEv.tEvent.tAction.bStatus)
//@		sprintf(str, "%02d %s %s%s", i, time, tReDisp.strName, "发出");
//@	else
//@		sprintf(str, "%02d %s %s%s", i, time, tReDisp.strName, "复归");
	
//	LONG FDNum = pEvent->FD_Num;
		
	m_Static[3].SetCaption(str); //  动作时间

	m_Static[2].SetCaption(tReDisp.strName); //  名称

	SetFDNum(pEvent->FD_Num);
		

	LONG 		RMIndex;
	DWORD 		dwValue;
	float		fValue;
	TRM_Disp 	tRMDisp;
	BYTE		*pData;
	
	char sztmp[32];
	char strFormat[32];
	for(i= 0; i<pEvent->FD_Num; i++)
	{
		pData = (BYTE *)(pEvent+1)+i*(sizeof(WORD)+sizeof(float))+2;
		dwValue = MAKEDWORD(pData[0],pData[1],pData[2],pData[3]);
		fValue = *((float *)&dwValue);
			
		RMIndex = tReDisp.wRMList[i];
		DB_GetRMDisp(RMIndex, &tRMDisp); //得到该类型遥测的描述参数

		sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nSecDec, "f%s");

		sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strSecUnit);		
		//故障电量显示1次还是2次值
		m_Static[5+i].SetCaption(sztmp);
	}
	return TRUE;
}

BOOL CStartDetailDlg::Init()
{
	m_nCurPageIndex = 0;
	m_nCurTaborder = 0;

	SetCaption("启动报告： No.1");
	SetCaption("", 1);
	SetEscWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	SetHaveEnterFun(-1);
	SetHaveAddSubFun(0);
	SetHaveInitFun(2);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(13,30,83,45,0);
	m_Static[0].SetTabOrder(-5);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetTimer(-1);
	m_Static[0].SetCaption("动作时间");
	m_pObjList[1] = &m_Static[0];

	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(0,0,0,0);//;13,52,83,67,0);
	m_Static[1].SetTabOrder(-4);
	m_Static[1].SetFont(16);
	m_Static[1].SetOwner(this);
	m_Static[1].SetTimer(-1);
	m_Static[1].SetCaption(""); //相对时间");
	
	m_pObjList[2] = &m_Static[1];
	m_Static[2].SetDataSource(-1,0);
	m_Static[2].SetPosition(13,75,233,90,0);
	m_Static[2].SetTabOrder(-3);
	m_Static[2].SetFont(16);
	m_Static[2].SetOwner(this);
	m_Static[2].SetTimer(-1);
	m_Static[2].SetCaption("过流I段动作 ");
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(13,50,224,72,0); //109,52,224,72,0
	m_Static[3].SetTabOrder(-2);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetTimer(-1);
	m_Static[3].SetCaption("2008.01.02 13:13:23.123");
	m_pObjList[4] = &m_Static[3];

	m_Static[4].SetDataSource(-1,0);
	m_Static[4].SetPosition(0,0,0,0); //109,52,224,72,0);
	m_Static[4].SetTabOrder(-1);
	m_Static[4].SetFont(16);
	m_Static[4].SetOwner(this);
	m_Static[4].SetTimer(-1);
	m_Static[4].SetCaption(""); //0.01.600");
	m_pObjList[5] = &m_Static[4];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(5);
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;
	SetTimerSpan(0);
	

	//最大量初始化 用来显示故障电量的static
	int i;
	for(i=5; i<5+MAX_FDNUM; i++)
	{
		m_Static[i].SetDataSource(-1,0);
		m_Static[i].SetTabOrder(0-i-1);
		m_Static[i].SetFont(16);
		m_Static[i].SetOwner(this);
		m_Static[i].SetTimer(-1);
		m_pObjList[i+1] = NULL;

	}

	//第0页的4个控件位置
	m_Static[5].SetPosition(10,		30+25*3,	10+105,	30+25*3+17,	0);
	m_Static[6].SetPosition(105+20,	30+25*3,	105+20,	30+25*3+17,	0);
	m_Static[7].SetPosition(10,		30+25*4,	10+105,	30+25*4+17,	0);
	m_Static[8].SetPosition(105+20,	30+25*4,	20+105,	30+25*4+17,	0);
	
	int j=0;
	for(i=0; i<(MAX_FDNUM-4)/2; i++)
	{
		m_Static[9+2*i].SetPosition(10,		  30+25*j,	10+105,	30+25*j+17,	i/10+1);
		m_Static[9+2*i+1].SetPosition(105+20, 30+25*j,	105+20,	30+25*j+17,	i/10+1);
		j++;
		j = j%5;
	}

	return TRUE;
}
BEGIN_HMIMESSAGE_MAP(CStartDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CAlarmDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()
