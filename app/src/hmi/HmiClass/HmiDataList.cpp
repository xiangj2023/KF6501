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
*		-Init() 初始化函数， 根据列表类型设置各个键的响应方法，设置可编辑列
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

#include "HmiDataList.h"
#include "HmiApp.h"
#include "lcdapp.h"
#include "relaydef.h"
#include "Translator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern TComInfo g_ProtocolInfo[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//操作报告编码对应字符串
char OperationStr[][32] =
{
	QT_TRANSLATE_NOOP("CHmiDataList","null"), //没有
	QT_TRANSLATE_NOOP("CHmiDataList","系统复位"),//@	#define MISC_CLASS_RESET		1
	QT_TRANSLATE_NOOP("CHmiDataList","下装文件"),//@#define MISC_CLASS_DOWNFILE		2
	QT_TRANSLATE_NOOP("CHmiDataList","通讯事件"),//@#define MISC_CLASS_COMM			3
	QT_TRANSLATE_NOOP("CHmiDataList","遥控事件"),//@#define MISC_CLASS_BO			4
	QT_TRANSLATE_NOOP("CHmiDataList","遥调事件"),//@#define MISC_CLASS_AO			5
	QT_TRANSLATE_NOOP("CHmiDataList","同期事件"),//@#define MISC_CLASS_SYNC			6
	QT_TRANSLATE_NOOP("CHmiDataList","修改定值"),//@#define MISC_CLASS_SET			7
	QT_TRANSLATE_NOOP("CHmiDataList","修改定值区号"),//@#define MISC_CLASS_SETAREA		8
	QT_TRANSLATE_NOOP("CHmiDataList","修改软压板"),//@#define MISC_CLASS_SW			9
	QT_TRANSLATE_NOOP("CHmiDataList","修改配置"),//@#define MISC_CLASS_CFG			10
	QT_TRANSLATE_NOOP("CHmiDataList","修改系数"),//@#define MISC_CLASS_CF			11
	QT_TRANSLATE_NOOP("CHmiDataList","修改参数"),//@#define MISC_CLASS_PARA			12
	QT_TRANSLATE_NOOP("CHmiDataList","电源事件"),//@#define MISC_CLASS_POWER		13
	QT_TRANSLATE_NOOP("CHmiDataList","修改电度底数"),//@#define MISC_CLASS_CLEARCI		14
	QT_TRANSLATE_NOOP("CHmiDataList","遥测置数"),//@#define MISC_CLASS_MODIAI		15
	QT_TRANSLATE_NOOP("CHmiDataList","遥信置位"),//@#define MISC_CLASS_MODIBI		16
	QT_TRANSLATE_NOOP("CHmiDataList","电度置数"),//@#define MISC_CLASS_MODICI		17

};
IMPLEMENT_HMI_TR(CHmiDataList)
CHmiDataList::CHmiDataList()
{
	m_nCursorPos = 0;
	m_nAreaNo = 1;
	m_nDataPageNum  = 0;
	m_nCurItemIndex = 0;
}
/****************************************************************************************
 * 
 * 功能描述: 构造函数
 * 
 * 参数说明: 
 *			- LONG dataType
 *			  类型: 
 *			  说明: 数据列表类型，取值见EListType的定义
 *
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
	m_nDataPageNum = -1;
}

CHmiDataList::~CHmiDataList()
{

}

//初始化函数, 在父窗口的OnShowWindow中调用
BOOL CHmiDataList::Init()
{
	m_nPageSize = m_nYSize/m_nItemHeight;
	m_nCurColumnIndex = -1;
	m_nCurItemIndex = 0;
	m_nDataPageNum = 0; 

	//根据不同类型设置按键的响应属性,可编辑列， 鼠标的宽度,可显示的定值条数
	switch(m_nDataType)
	{
	case DIFilterTime:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetEditableColum(1,WIDTH_DI_FILTERTIME); //WORD 5位数
		break;
	case BOTime:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetEditableColum(1,WIDTH_DI_FILTERTIME); //WORD 5位数
		break;
	case CF:
		SetEditableColum(1, WIDTH_CF); //LONG 5位数
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;

	case CIConst:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		
		SetEditableColum(1, WIDTH_CI_CONST);
		break;


	case CIBase:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		
		SetEditableColum(1, WIDTH_CI_BASE); //
		break;


	case DINor:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		SetCurorWidth(16*2);  //取反， 正常 2个汉字
		SetEditableColum(1,1); 
		
		break;
	case SoePara:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		
		SetCurorWidth(16*3);  //产生， 不产生 3个汉字
		SetEditableColum(1,1); 
		break;

	case ListCI:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		break;
	case ListBI:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

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
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		//更新可以显示的定值列表
		HMIUpDataVisibleSetList(m_nAreaNo, &m_nAllItemNum, CHmiApp::m_wpVisibleSetList,TRUE);
		break;
	case ASQuery:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;
	case ConfigChange:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		SetCurorWidth(24);  //投, 退 1个汉字
		SetEditableColum(1,1); 

		break;
	case SoftSwitch:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetCurorWidth(24);  //投, 退 1个汉字
		SetEditableColum(1,1); 

		break;
	case SysStatus:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;

	case DIPara:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetCurorWidth(16*2); //2个汉字
		SetEditableColum(1, 1);
		break;
	case PortPara:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetCurorWidth(16*3); //2个汉字
		SetEditableColum(1, 6);
		break;
//@	case ActData:
//@		break;
//@	case AlarmData:
//@		break;
//@	case StartData:
//@		break;
	default:
		;
	}
	//从数据库读数据
	return GetDataFromDB();

}

BOOL CHmiDataList::OnUp()
{
	if (m_nAllItemNum == 0)
	{
		return TRUE;
	}
	LONG nOldPageNum = m_nDataPageNum;
	LONG nItemIndex;
	if(m_nUpDownFun == MoveFocus)
	{
		nItemIndex = (m_nPageSize*m_nDataPageNum + m_nCurItemIndex + m_nAllItemNum -1)%m_nAllItemNum;
		m_nDataPageNum = nItemIndex/m_nPageSize;
		m_nCurItemIndex = nItemIndex%m_nPageSize;
		
	}
	else if(m_nUpDownFun == ChgPage)
	{		
		m_nDataPageNum--;
		m_nCurItemIndex = 0;

		LONG nMaxPageCount = m_nAllItemNum/m_nPageSize;
		if (m_nAllItemNum%m_nPageSize) //还有不足一页的内容
		{
			nMaxPageCount += 1;
		}

		if (m_nDataPageNum<0)
		{
			m_nDataPageNum = nMaxPageCount-1; //最大页；
		}
	}
	
	DWORD wParam;
	if (m_bHaveSelChgFun)
	{
		wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}
	
	//如果已经翻到下一页了， 则重新从数据库读取一页数据
	if(m_nDataPageNum != nOldPageNum)
	{
		if(!GetDataFromDB())
		{
			m_pOwner->Close();
			return FALSE;
		}
		m_nCurItemIndex = 0;
	}
	
	OnPaint();
	UpdateWindow();
	return TRUE;

}
BOOL CHmiDataList::OnDown()
{
	if (m_nAllItemNum == 0)
	{
		return TRUE;
	}
	LONG nOldPageNum = m_nDataPageNum;
	LONG nItemIndex;
	if(m_nUpDownFun == MoveFocus)
	{
		nItemIndex = (m_nPageSize*m_nDataPageNum + m_nCurItemIndex +1)%m_nAllItemNum;
		m_nDataPageNum = nItemIndex/m_nPageSize;
		m_nCurItemIndex = nItemIndex%m_nPageSize;
		
	}
	else if(m_nUpDownFun == ChgPage)
	{		
		m_nCurItemIndex = 0;
		LONG nMaxPageCount = m_nAllItemNum/m_nPageSize;
		if (m_nAllItemNum%m_nPageSize) //还有不足一页的内容
		{
			nMaxPageCount += 1;
		}
		//最后一页了
		if (m_nDataPageNum >= (nMaxPageCount-1))
		{
			m_nDataPageNum = 0;
		}
		else
			m_nDataPageNum++;
	}
	
	DWORD wParam;
	if (m_bHaveSelChgFun)
	{
		wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}

	//如果已经翻到下一页了， 则重新从数据库读取一页数据
	if(m_nDataPageNum != nOldPageNum)
	{
		if(!GetDataFromDB())
		{
			m_pOwner->Close();
			return FALSE;
		}
		m_nCurItemIndex = 0;
	}

	OnPaint();
	UpdateWindow();
	return TRUE;
}

//本次显示的项目的起始号 
LONG CHmiDataList::GetItemBase()
{
	return m_nPageSize*m_nDataPageNum;
}

BOOL CHmiDataList::OnLeft()
{

	//如果是编辑态则移动光标
	if (m_nCurColumnIndex !=-1)
	{
		m_nCursorPos = m_nCursorPos +m_nMaxLenEditable -1;
		if (m_nMaxLenEditable!=0)
		{
			m_nCursorPos %= m_nMaxLenEditable;
		}
		OnPaint();
		UpdateWindow();
		return TRUE;
		
	}
	//不是编辑态移动焦点或翻页
	LONG nOldPageNum = m_nDataPageNum;
	if (m_nAllItemNum == 0)
	{
		return TRUE;
	}
	LONG nItemIndex;
	if(m_nLeftRightFun == MoveFocus)
	{
		nItemIndex = (m_nPageSize*m_nDataPageNum + m_nCurItemIndex + m_nAllItemNum - 1)%m_nAllItemNum;
		m_nDataPageNum = nItemIndex/m_nPageSize;
		m_nCurItemIndex = nItemIndex%m_nPageSize;
		
	}
	else if(m_nLeftRightFun == ChgPage)
	{		
//		nItemIndex = (m_nPageSize*m_nDataPageNum + m_nCurItemIndex - m_nPageSize + m_nAllItemNum)%m_nAllItemNum;
		m_nDataPageNum--;
		m_nCurItemIndex = 0;

		LONG nMaxPageCount = m_nAllItemNum/m_nPageSize;
		if (m_nAllItemNum%m_nPageSize) //还有不足一页的内容
		{
			nMaxPageCount += 1;
		}

		if (m_nDataPageNum<0)
		{
			m_nDataPageNum = nMaxPageCount-1; //最大页；
		}

//@		m_nDataPageNum = nItemIndex/m_nPageSize;
//@		m_nCurItemIndex = nItemIndex%m_nPageSize;
	}
	
	DWORD wParam;
	if (m_bHaveSelChgFun)
	{
		wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}
	
	//如果已经翻到下一页了， 则重新从数据库读取一页数据
	if(m_nDataPageNum != nOldPageNum)
	{
		if(!GetDataFromDB())
		{
			m_pOwner->Close();
			return FALSE;
		}
		m_nCurItemIndex = 0; //翻到下一页则显示下一页的第0条记录
	}
	
	OnPaint();
	UpdateWindow();
	return TRUE;
}
BOOL CHmiDataList::OnRight()
{
	//如果是编辑态则移动光标
	if (m_nCurColumnIndex !=-1)
	{
		m_nCursorPos++;
		if (m_nMaxLenEditable!=0)
		{
			m_nCursorPos %= m_nMaxLenEditable;
		}
		OnPaint();
		UpdateWindow();
		return TRUE;
		
	}

	LONG nOldPageNum = m_nDataPageNum;
	if (m_nAllItemNum == 0)
	{
		return TRUE;
	}
	LONG nItemIndex;
	if(m_nLeftRightFun == MoveFocus)
	{
		nItemIndex = (m_nPageSize*m_nDataPageNum + m_nCurItemIndex +1)%m_nAllItemNum;
		m_nDataPageNum = nItemIndex/m_nPageSize;
		m_nCurItemIndex = nItemIndex%m_nPageSize;
		
	}
	else if(m_nLeftRightFun == ChgPage)
	{		
//		nItemIndex = (m_nPageSize*m_nDataPageNum + m_nCurItemIndex +m_nPageSize)%m_nAllItemNum;
		m_nCurItemIndex = 0;
		LONG nMaxPageCount = m_nAllItemNum/m_nPageSize;
		if (m_nAllItemNum%m_nPageSize) //还有不足一页的内容
		{
			nMaxPageCount += 1;
		}
		//最后一页了
		if (m_nDataPageNum >= (nMaxPageCount-1))
		{
			m_nDataPageNum = 0;
		}
		else
			m_nDataPageNum++;

//@		m_nDataPageNum = nItemIndex/m_nPageSize;
//@		m_nCurItemIndex = nItemIndex%m_nPageSize;
	}
	
	DWORD wParam;
	if (m_bHaveSelChgFun)
	{
		wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}
	
	//如果已经翻到下一页了， 则重新从数据库读取一页数据
	if(m_nDataPageNum != nOldPageNum)
	{
		if(!GetDataFromDB())
		{
			m_pOwner->Close();
			return FALSE;
		}
		m_nCurItemIndex = 0; //翻到下一页则显示下一页的第0条记录
	}
	
	OnPaint();
	UpdateWindow();
	return TRUE;
}
BOOL CHmiDataList::OnAdd()
{
	//无输入焦点， 不处理加减键
	if(m_nEditableColumn == -1)
		return FALSE;
	if (m_nCurColumnIndex == -1)
	{
		m_nCurColumnIndex = m_nEditableColumn;
		OnPaint();
		UpdateWindow();
		return TRUE;
	}

	switch(m_nDataType)
	{
	case DIPara:
		ChgDIPara_wClassBuf(1);
		break;
	case DIFilterTime:
		ChgDIFilterTimeBuf(1);
		break;
	case DINor:
		ChgBINOBuf(1);
		break;
	case SoePara:
		ChgBISOEParaBuf(1);
		break;
	case ConfigChange:
		ChgCfgValue();
		break;
	case SoftSwitch:
		ChgSWValue();
		break;
	case CF:
		ChgCFBufValue(1);
		break;
	case CIBase:
		ChgCIBaseBufValue(1);
		break;
	case CIConst:
		ChgCIConstBufValue(1);
		break;
	case BOTime:
		ChgBOTime(1);
		break;
	
	case PortPara:
		ChgPortPara(1);
		break;
	default:
		return FALSE;
	}

	OnPaint();  //绘制内存
	UpdateWindow();  //内存到显示器
	return TRUE;
}
BOOL CHmiDataList::OnSub()
{
	
	//无输入焦点， 不处理加减键
	if(m_nEditableColumn == -1)
		return FALSE;
	
	if (m_nCurColumnIndex == -1)
	{
		m_nCurColumnIndex = m_nEditableColumn;
		OnPaint();
		UpdateWindow();
		return TRUE;
	}
	switch(m_nDataType)
	{
		case DIPara:
			ChgDIPara_wClassBuf(1);
			break;
			
		case DIFilterTime:
			ChgDIFilterTimeBuf(-1);
			break;
		case DINor:
			ChgBINOBuf(-1);
			break;
		case SoePara:
			ChgBISOEParaBuf(1);
			break;

		case ConfigChange:
			ChgCfgValue();
			break;
		case SoftSwitch:
			ChgSWValue();
			break;
		case CF:
			ChgCFBufValue(-1);
			break;
		case CIBase:
			ChgCIBaseBufValue(-1);
			break;
		case CIConst:
			ChgCIConstBufValue(-1);
			break;
		case BOTime:
			ChgBOTime(-1);
			break;
			
		case PortPara:
			ChgPortPara(-1);
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

void CHmiDataList::UpdateData(BOOL bIsSave)
{
	if(!bIsSave)
	GetDataFromDB();
}

/****************************************************************************************
 * 功能描述: 从数据库按照列表类型读取数据
 * 参数说明: 
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
BOOL CHmiDataList::GetDataFromDB()
{
	m_nItemCount = 0;
	m_nAllArea = DB_GetSETAreaNum();
	m_nRunAreaNo = DB_GetCurrentSETAreaNo();

	m_nItemHeight = 18;//
	m_nFont = 16; //

	char str[64];
	BOOL Result =  FALSE;

	switch(m_nDataType)
	{
	case CF:
		Result = ShowCFValueBuf();
		break;

	case DIFilterTime:
		Result = ShowDIPara_wFilterTimeBuf();
		break;

	case CIBase:
		Result = ShowCIBaseBuf();
		

		break;

	case CIConst:
		Result = ShowCIConstBuf();
		break;


	case ListCI:
		Result = ShowCIValue();
		break;
	case ListBI:
		Result = ShowBIValue();
		break;
	case Action:
		m_nFont = 12; //
		//动作报告
		Result = ShowActionReportDB();
		break;

	case Alarm:
		m_nFont = 12; //
		Result = ShowAlarmReportDB();
		break;

	case Start:
		//启动报告
		m_nFont = 12; //
		Result = ShowStartReportDB();
		break;

	case Operation:
		Result = ShowOperationReportDB();
		break;

	case Soe:
		Result = ShowSOEReportDB();
		break;
	case ConfigQuery:
		Result = ShowConfigDB(); //查询时直接从数据库读取
		break;
	case SettingQuery:
		sprintf(str, "%s:%d", tr("查看定值区"),m_nAreaNo);
		m_pOwner->SetCaption(str);


		Result = ShowSettingQueryDB(m_nAreaNo);
		break;
	case ASQuery:
		Result = ShowSysSettingQueryDB();
		break;
			
	case ConfigChange:
		m_nCursorWidth = 24;
		Result = ShowConfigBuf(); //更改时需要缓冲区
		break;
	case SoftSwitch:
		m_nCursorWidth = 24;
		Result = ShowSoftSwitchBuf();
		break;
	case SysStatus:
		break;

	case DINor:
		Result = ShowBINorBuf();
		break;

	case SoePara:
		Result = ShowSOEParaBuf();
		break;

	case DIPara:
		Result = ShowDIPara_wClassBuf();
		break;
	case BOTime:
		Result = ShowBOLastTimeBuf();
		break;
	case PortPara:
		Result = ShowPortPara();
		break;	


//@	case ActData:
//@		break;
//@	case AlarmData:
//@		break;
//@	case StartData:
//@		break;
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
	return Result;

}



//显示电度实时值
BOOL CHmiDataList::ShowCIValue()
{
	TCIPara tCIPara;
	LONG  i,rc;
	char str[64];
	DWORD val;

	m_nAllItemNum = DB_GetCINum();

	int nItemCount = 0;
	
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{

		rc = DB_GetCIPara(i, &tCIPara);
		rc = DB_ReadCI(i, &val);
		sprintf(str, "%s			%d", tCIPara.strSymbol, val);
		AddRow(str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;

}

BOOL CHmiDataList::ShowOperationReportDB()
{
	TMiscEvent tEvent;
	WORD num;
	char str[64];
	char str1[64];
	char str2[8];
	char strTime[64];
	char rptname[64];
	int i;
	LONG rc;
	WORD  wHead,wTail, wReportIndex;
	TSysTime out;
	LONG nItem;

//	rc	= DB_Misc_GetNum(&num);

	rc	= DB_Misc_GetHead(&wHead);
	rc	|= DB_Misc_GetTail(&wTail);
	num = (wTail+MAX_MISC_NUM-wHead)%MAX_MISC_NUM;

	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("操作记录"), tr("读操作记录错误"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}
	
	//IsEmpty = DB_Misc_IsEmpty();
	m_nAllItemNum = num;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无记录"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		return TRUE;
	}
/*
	rc = DB_Misc_GetTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("操作记录","CHmiDataList"), tr("读操作记录错误","CHmiDataList"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}
*/	
	int nItemCount =0;
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		wReportIndex = (wTail - i - 1 +MAX_MISC_NUM)%MAX_MISC_NUM;
		
		rc = DB_Misc_Read(wReportIndex, &tEvent);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("操作记录"), tr("读操作记录错误"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}
		AbsTimeTo(&(tEvent.absTime) ,&out);

		sprintf(strTime, "%02d/%02d %02d:%02d:%02d", 
			 out.Month, out.Day, out.Hour, out.Minute,out.Second);


		switch(tEvent.nClass)
		{
			case MISC_CLASS_RESET: 				
				strcpy(rptname,tr("装置复位"));
				break;
			case MISC_CLASS_SETAREA:
				strcpy(rptname,tr("切换定值区"));
				break;
			case MISC_CLASS_POWER:				
				if(tEvent.dwValue == 0)
					strcpy(rptname,tr("装置上电"));
				else
					strcpy(rptname,tr("装置掉电"));
				break;

			case MISC_CLASS_PARA:
				if(tEvent.dwValue == DB_PARA_DAI)
					strcpy(rptname,tr("修改DAI参数"));
				else if(tEvent.dwValue == DB_PARA_DI)
					strcpy(rptname,tr("修改DI参数"));
				else if(tEvent.dwValue == DB_PARA_AI)
					strcpy(rptname,tr("修改AI参数"));				
				else if(tEvent.dwValue == DB_PARA_BI)
					strcpy(rptname,tr("修改BI参数"));
				else if(tEvent.dwValue == DB_PARA_CI)
					strcpy(rptname,tr("修改CI参数"));
				else if(tEvent.dwValue == DB_PARA_BO)
					strcpy(rptname,tr("修改BO参数"));
				else if(tEvent.dwValue == DB_PARA_AO)
					strcpy(rptname,tr("修改AO参数"));
				else if(tEvent.dwValue == DB_PARA_SER)
					strcpy(rptname,tr("修改COM参数"));				
				else if(tEvent.dwValue == DB_PARA_CAN)
					strcpy(rptname,tr("修改CAN参数"));
				else if(tEvent.dwValue == DB_PARA_ETH)
					strcpy(rptname,tr("修改ETH参数"));
				else if(tEvent.dwValue == DB_PARA_SYS)
					strcpy(rptname,tr("修改SYS参数"));
				break;
			case MISC_CLASS_COMM:

				if((tEvent.dwAttr&0xF00) == 0x100)
					sprintf(str2,"%s%d","S",tEvent.dwAttr&0xF);
				if((tEvent.dwAttr&0xF00) == 0x200)
					sprintf(str2,"%s%d","C",tEvent.dwAttr&0xF);				
				if((tEvent.dwAttr&0xF00) == 0x300)
					sprintf(str2,"%s%d","A",tEvent.dwAttr&0xF);				
				if((tEvent.dwAttr&0xF00) == 0x400)
					sprintf(str2,"%s%d","B",tEvent.dwAttr&0xF);
				
				if(tEvent.dwValue == 0)
					sprintf(str1,"%s%s",tr("通讯恢复"),str2);
				else					
					sprintf(str1,"%s%s",tr("通讯中断"),str2);				
				strcpy(rptname,str1);
				break;
			default:
				strcpy(rptname,tr(OperationStr[tEvent.nClass]));
				break;
				
		}
		//序号显示从1开始
		sprintf(str, "%02d %s %s",  i+1, strTime,rptname);
		nItem = AddRow(str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}

BOOL CHmiDataList::ShowSOEReportDB()
{
	TEventBI tSOE;
	WORD num;
	char str[64];
	int i;
	LONG rc;
	WORD  wHead,wTail, wReportIndex;
	TSysTime out;
	char strStatus[32];

//	rc	= DB_GetSOENum(&num);
	rc	= DB_GetSOEHead(&wHead);
	rc  |= DB_GetSOETail(&wTail);
	num = (wTail+MAX_SOE_NUM-wHead)%MAX_SOE_NUM;

	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("SOE记录"), tr("读SOE记录错误"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}
		
	m_nAllItemNum = num;
	
	int nItemCount =0, nItem = 0;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无记录"));
		
		nItem = AddRow(str);
		SetItemData(nItem,HMI_LIST_NORECORD);
		return TRUE;
	}


	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		wReportIndex = (wTail -i -1 + MAX_SOE_NUM)%MAX_SOE_NUM;
		rc = DB_ReadSOE(&tSOE,wReportIndex);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("SOE记录"), tr("读SOE记录错误"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}
		
		//			DB_IncSOERead();
		//需要决定时间和相对时间转换函数
		AbsTimeTo(&(tSOE.stTime) ,&out);
		if(tSOE.bStatus)
			sprintf(strStatus, "%s", "0-->1");
		else
			sprintf(strStatus, "%s", "1-->0");

		sprintf(str, "%02d)%02d/%02d/%02d %02d:%02d:%02d:%03d BI%02d %s", i, 
			out.Year-2000, out.Month, out.Day, out.Hour, out.Minute, out.Second, out.MSecond,
			tSOE.wNo+1, strStatus);
		nItem = AddRow(str);
		//可能是soe编号，
		SetItemData(nItem,wReportIndex);
		if((++nItemCount) == m_nPageSize)
			break;

	}
	SetFont(12);
	return TRUE;
}

//显示数据库中的配置
BOOL CHmiDataList::ShowConfigDB()
{
	int nItemCount = 0, nItem = 0;
	LONG rc;
	BYTE CFGStatus;
	TCFG_Disp tCfgDisp;
	char str[64];
	WORD i;

	//配置查看
	m_nAllItemNum = DB_GetCFGNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-3));
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	//一次只读一屏可以显示的
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		rc = DB_GetCFGStatus(m_nAreaNo,i, CFGStatus);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("配置查看"), tr("读配置错误"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		rc = DB_GetCFGDisp(i, &tCfgDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("配置查看"), tr("读配置错误"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		if(CFGStatus == CFG_STATUS_SET)
		{
			sprintf(str, "%s",  tr("投"));

		}
		else
		{
			sprintf(str, "%s",  tr("退"));
		}
		nItem = AddRow(TR_GetCFGName(&tCfgDisp,i));
		SetItemData(nItem, CFGStatus);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;

}

BOOL CHmiDataList::ShowSoftSwitchBuf()
{
	char str[64];
	int i;
	LONG rc;
	BYTE Status;
	TSW_Disp tswDisp;

	 
	m_nAllItemNum = DB_GetSWNum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-3));

	int nItemCount =0, nItem = 0;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}


	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		Status = CHmiApp::m_byNewSoftSwitch[i];
		rc = DB_GetSWDisp(i, &tswDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("软压板"), tr("读软压板错误"),
				CHmiApp::m_pMainWnd,CHmiApp::m_pMainWnd,HMI_MB_OK);
			return FALSE;
		}
		
		if(Status ==SW_STATUS_SET)
		{
			sprintf(str, "%s",  tr("投"));
		}
		else
		{
			sprintf(str, "%s",  tr("退"));
		}
		nItem =  AddRow(TR_GetSWName(&tswDisp,i));
		SetItemText(nItem,1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;

}

//显示缓冲区中的系数值， 在调用该函数之前需要把系数读入缓冲区中
BOOL CHmiDataList::ShowCFValueBuf()
{
	WORD i;
	LONG* lCfValueList;;
	m_nAllItemNum = DB_GetCFNum();
	TCF_Disp tCFDisp;
	LONG nItem;
	char str[32];
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-10));
	LONG rc;

	lCfValueList = CHmiApp::m_npCFList;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}
	
	int nItemCount = 0;
	//一次只读一屏可以显示的
	char strFormat[8];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_CF, "d");
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		rc = DB_GetCFDisp(i, &tCFDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("系数查看"), tr("读系数错误"),
				CHmiApp::m_pMainWnd,CHmiApp::m_pMainWnd,HMI_MB_OK);
			return FALSE;
		}
		nItem= AddRow(tCFDisp.strSymbol);
		
		sprintf(str, strFormat,  lCfValueList[i]);
		//格式类似与		sprintf(str, "%04d",  lCfValueList[i]);

		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}

//显示缓冲区的DI，需要在调用该函数之前的适当时机从数据库先读入缓冲区中
BOOL CHmiDataList::ShowDIPara_wClassBuf()
{

	char str[64];
	int i;

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-5));

	m_nAllItemNum = DB_GetDINum();
	
	int nItemCount = 0, nItem=0;
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		
		sprintf(str, "%s%02d", tr("开入"),i);
		nItem = AddRow(str);

		if(CHmiApp::m_wpDIClass[i] == DI_TYPE_PLUS)
			sprintf(str, "%s",  tr("电度"));
		else if(CHmiApp::m_wpDIClass[i] == DI_TYPE_SP)
			sprintf(str, "%s",   tr("遥信"));
		else if(CHmiApp::m_wpDIClass[i] == DI_TYPE_INNER)
			sprintf(str, "%s",   tr("内部遥信"));
		
		else if(CHmiApp::m_wpDIClass[i] == DI_TYPE_VIR)
			sprintf(str, "%s",   tr("虚遥信"));
		else
			sprintf(str, "%s",   tr("其他"));
			
		SetItemText(nItem, 1, str);

		if((++nItemCount) == m_nPageSize)
			break;

	}
	return TRUE;
}
//显示缓冲区中直接采样点去抖时间
BOOL CHmiDataList::ShowDIPara_wFilterTimeBuf()
{
	char str[64];
	int i;

	m_nAllItemNum = DB_GetDINum();
	SetColumnCount(3);
	LONG width = this->GetXSize();// width得到的是像素宽度
	SetColPos(1, width/8-WIDTH_DI_FILTERTIME-3); //SetColPos 设置的是字符位置
	SetColPos(2, width/8-2);

	char strFormat[16];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_DI_FILTERTIME, "d");
	
	
	int nItemCount = 0, nItem = 0;

	//显示缓冲区中的数据
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		sprintf(str, "%s%d","DI", i+1);
		nItem = AddRow(str);

		sprintf(str, strFormat, CHmiApp::m_wpDIFilterTime[i]);
		SetItemData(nItem, CHmiApp::m_wpDIFilterTime[i]);
		SetItemText(nItem, 1, str);
		SetItemText(nItem, 2, "ms");
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}
//BI是否产生带时标的SOE wControl的D1， 显示缓冲区中的值
BOOL CHmiDataList::ShowSOEParaBuf()
{
	TBIPara tBIPara;
	char str[64];
	int i, nItem;
	LONG rc;
	
	m_nAllItemNum = DB_GetBINum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-8));
	
	int nItemCount = 0;
	//一次只读一屏可以显示的
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI属性"), tr("读BI参数错误"),
				CHmiApp::m_pMainWnd,CHmiApp::m_pMainWnd,HMI_MB_OK);
			return FALSE;
		}
		
		
		if (CHmiApp::m_wpBIControl[i]&BI_ATTR_WTPEVENT)
			sprintf(str, "%s"  ,  tr("产生"));
		else
			sprintf(str, "%s" ,  tr("不产生"));
		nItem = AddRow(TR_GetBIName(&tBIPara,i));
		SetItemData(nItem, CHmiApp::m_wpBIControl[i]);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
		
	}
	return TRUE;
}

BOOL CHmiDataList::ShowBINorBuf()
{
	TBIPara tBIPara;
	char str[64];
	int i, nItem;
	LONG rc;


	m_nAllItemNum = DB_GetBINum();
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-4));
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	int nItemCount = 0;
	//一次只读一屏可以显示的
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI属性"), tr("读BI参数错误!"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		
		if (CHmiApp::m_wpBIControl[i]&BI_ATTR_CONVERT)
			sprintf(str, "%s"  ,  tr("取反"));
		else
			sprintf(str, "%s" ,  tr("正常"));
		
		nItem = AddRow(TR_GetBIName(&tBIPara,i));
		SetItemData(nItem, CHmiApp::m_wpBIControl[i]);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}
//显示缓冲区中的电度底数
BOOL CHmiDataList::ShowCIBaseBuf()
{
	TCIPara tCIPara;
	char str[64];
	int i;
	SetColumnCount(2);
	LONG width = this->GetXSize();// width得到的是像素宽度
	LONG rc;
	
	char strFormat[8];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_CI_BASE, "d");

	m_nAllItemNum = DB_GetCINum();
	SetColumnCount(2);
	SetColPos(1, (width/8-WIDTH_CI_BASE));

	int nItemCount = 0, nItem =0;
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}
	
	//一次只读一屏可以显示的
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetCIPara(i, &tCIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("CI属性"), tr("读CI参数错误"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		nItem = AddRow(tCIPara.strSymbol);
		
		sprintf(str, strFormat, CHmiApp::m_dwpCIBase[i]);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
		
	}
	return TRUE;

}

//显示缓冲区中的脉冲常数
BOOL CHmiDataList::ShowCIConstBuf()
{
	TCIPara tCIPara;
	char str[64];
	int i;
	SetColumnCount(2);
	LONG rc;
	
	char strFormat[16];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_CI_CONST, "d");
	
	//电度的前两路是积分电度，积分电度没有脉冲常数
	m_nAllItemNum = DB_GetCINum()-INTERGRAL_KWH_COUNT;

	
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-WIDTH_CI_CONST));

	int nItemCount = 0, nItem = 0;
	
	if (m_nAllItemNum  <= 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	//一次只读一屏可以显示的
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetCIPara(i+INTERGRAL_KWH_COUNT, &tCIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("CI属性"), tr("读CI参数错误"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		nItem = AddRow(tCIPara.strSymbol);

		sprintf(str, strFormat, CHmiApp::m_dwpCIConst[i]);//注意积分电度无脉冲常数
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;

	}
	return TRUE;
}

BOOL CHmiDataList::ShowSysSettingQueryDB()
{
	LONG rc, nItem = 0, nItemCount = 0;
	m_nAllItemNum = DB_GetSystemSETNum();
	WORD i;
	char strFormat[32], str[64]; // 显示格式

	//辅助定值显示列表相关
	char strSetDataName[MAX_DISP_COUNT][HMI_DBSTRING_SIZE];
	WORD wSetNameCount;
	int nValueIndex;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无公用定值"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;

	}

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-7));

	TSysSET_Disp tSysSET_Disp;
	float fVal;
	for( i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetSysSETDisp(i, &tSysSET_Disp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("公用定值"), tr("读公用定值错误"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		nItem = AddRow(TR_GetSysSETName(&tSysSET_Disp,i));
		rc = DB_GetSystemSETFloatValue(i, &fVal);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("公用定值"), tr("读公用定值错误"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}

		//2010-9-9
		//有辅助定值取值描述的，读取
		BOOL bDataNameOk = FALSE;
		wSetNameCount = 0;
		if(tSysSET_Disp.nItemNum >0)
		{
			//kang 2012-10-31 
//			rc = DB_GetSystemSETDataName(i, &strSetDataName[0][0],wSetNameCount);
			bDataNameOk = TR_GetSysSETDataName(&tSysSET_Disp, i,&strSetDataName[0][0],wSetNameCount);

			
			if(!bDataNameOk)
			{
				HMIMsgBox( tr("公用定值查看"), tr("读公用定值取值描述错误"),
					&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
				return FALSE;
			}
		}
		nValueIndex = fVal;
		
		if((nValueIndex <wSetNameCount )&&(nValueIndex >=0)&&(wSetNameCount>0))
		{
			sprintf(str,strSetDataName[nValueIndex]);
				
		}
		else
		{
			sprintf(strFormat, "%s%d%s", "%.", tSysSET_Disp.nDec, "f%s"); //"%.2f%s"
			sprintf(str, strFormat,  fVal, tSysSET_Disp.strUnit);
		}
		
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}

BOOL CHmiDataList::ShowSettingQueryDB(LONG nAreaNo)
{
	LONG rc;
	int i,j, nItem = 0;
	float	  fSETValue[256];
	TSET_Disp	tSETDisp;
	char str[64];
	char strFormat[32];
	char strSetDataName[MAX_DISP_COUNT][HMI_DBSTRING_SIZE];
	WORD wSetNameCount;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无可显示的定值"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(nItem, 1, "");

		return TRUE;

	}
	
	//读所有定值的值
	rc = DB_GetSETFloatValue(nAreaNo, SET_VALUE_ALL, fSETValue);	
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("定值差看"), tr("读定值错误"),
			&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
		return FALSE;
	}

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-7));


	WORD cfgNum = DB_GetCFGNum();
	LONG cfgWordNum = cfgNum/32;
	if(cfgNum%32)
	{
		cfgWordNum += 1;
	}
	WORD nSetIndex;
	int nSetValue;

	for( i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		//只显示配置投入了的定值
		nSetIndex = CHmiApp::m_wpVisibleSetList[i];
		rc = DB_GetSETDisp(nSetIndex, &tSETDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("定值查看"), tr("读定值错误"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		//2010-9-9
		//有定值取值描述的，读取
		wSetNameCount = 0;
		if(tSETDisp.nItemNum >0)
		{
			//2012-10-31 kang
			BOOL bSetDataOk = TR_GetSETDataName(&tSETDisp,nSetIndex, &strSetDataName[0][0],wSetNameCount);
			
			if(!bSetDataOk)
			{
				HMIMsgBox( tr("定值查看"), tr("读定值取值描述错误"),
					&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
				return FALSE;
			}
		}

		j = AddRow(TR_GetSETName(&tSETDisp,nSetIndex));


		nSetValue = fSETValue[nSetIndex]; //
		//2010-9-9还需要对显示的方式进行判断， 如果是列表框，还需要显示其描述，而不是值
		if((nSetValue <wSetNameCount )&&(nSetValue >=0)&&(wSetNameCount>0))
		{
			sprintf(str,strSetDataName[nSetValue]);
				
		}
		else
		{
			sprintf(strFormat, "%s%d%s", "%.", tSETDisp.nDec, "f%s"); //"%.2f%s"
			sprintf(str, strFormat,  fSETValue[nSetIndex], tSETDisp.strUnit);
		}

		SetItemText(j, 1, str);
		if((++nItem) == m_nPageSize)
			break;

	}
	return TRUE;
}
//显示遥信实时值
BOOL CHmiDataList::ShowBIValue()
{
	LONG  i,rc,nItem ;
	BYTE val;
	TBIPara tBIPara;
	m_nAllItemNum = DB_GetBINum();
	char str[64];	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(nItem, 1, "");
		return TRUE;
	}


	int nItemCount =0;
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetBIPara(i,&tBIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI查看"), tr("读BI参数错误"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		rc = DB_ReadBI(i, &val);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI查看"), tr("读BI值错误"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		if(val == DB_BI_OPEN)
		{
			sprintf(str,"%d%s \t\t\t%s", i, tBIPara.strSymbol , "□");
			
		}
		else
		{
			sprintf(str,"%d%s \t\t\t%s", i, tBIPara.strSymbol, "■");
		}
		nItem = AddRow(str);
		if((++nItemCount) == m_nPageSize) //值读取当前页的数据
			break;

	}
	return TRUE;
}

//缓冲区中出口保持时间
BOOL CHmiDataList::ShowBOLastTimeBuf()
{
	char str[64];
	int i;

	m_nAllItemNum = DB_GetBONum();
	
	SetColumnCount(3);
	LONG width = this->GetXSize();// width得到的是像素宽度
	SetColPos(1, width/8-WIDTH_DI_FILTERTIME-3); //SetColPos 设置的是字符位置
	SetColPos(2, width/8-2);

	char strFormat[16];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_DI_FILTERTIME, "d");
	
	
	int nItemCount = 0, nItem = 0;
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	//显示缓冲区中的数据
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		sprintf(str, "%s%d",  tr("遥控"), i+1);
		nItem = AddRow(str);

		sprintf(str, strFormat, CHmiApp::m_wpBOTime[i]);
		SetItemData(nItem, CHmiApp::m_wpBOTime[i]);
		SetItemText(nItem, 1, str);
		SetItemText(nItem, 2, "ms");
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}
BOOL CHmiDataList::ShowStartReportDB()
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
	WORD num, wReportIndex;

	WORD wHead,wTail;

	// 读头尾指针
	rc = DB_GetRelayStartReportHead(&wHead);
	rc |= DB_GetRelayStartReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("启动报告"), tr("读取启动报告失败!"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}

	// 计算报告总个数
	num = (wTail-wHead+MAX_RSTT_NUM )%MAX_RSTT_NUM;
	
	m_nAllItemNum = num;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无记录"));
		
		index = AddRow(str);
		SetItemData(index, HMI_LIST_NORECORD);
		
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(index, 1, "");
		return TRUE;
	}
	int nItemCount =0;
	rc = DB_GetRelayStartReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("启动报告"), tr("读取启动报告失败!"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}

	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		wReportIndex = (wTail -i -1 + MAX_RSTT_NUM)%MAX_RSTT_NUM;

		rc = DB_ReadRelayStartReport(&tStartReport, wReportIndex);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("启动报告"), tr("读取启动报告失败!"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}
		
		pEvent = &tStartReport.tEvent.tAction;

		inf = pEvent->nINF;
		abstime.Hi = pEvent->AbsTimeHi;
		abstime.Lo = pEvent->AbsTimeLo;
		AbsTimeTo(&abstime, &systime);
		
		
		sprintf(time, "%02d/%02d %02d:%02d",   systime.Month, systime.Day,
			systime.Hour, systime.Minute);

		rc = DB_GetREDispByInf(inf, &tReDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("启动报告"), tr("读取启动报告失败!"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}

	
		//序号显示从1开始
		sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr(""));	

		index = AddRow(str);
		SetItemData(index, wReportIndex);
		if((++nItemCount) == m_nPageSize)
			break;

	}
	return TRUE;
}


//08-12-24:17:15 改為列表只显示总报告， 不显示分报告， 报告应该从最新的开始显示。
BOOL CHmiDataList::ShowActionReportDB()
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
	WORD inf;
//@	WORD offset =0;
	WORD FDNum;
	LONG index;
	WORD wActionIndex; //在列表中数据中保存动作索引号， 高字节代表报告序号， 低字节代表分报告序号

	WORD wHead,wTail;

	// 读头尾指针
	rc = DB_GetRelayActionReportHead(&wHead);
	rc |= DB_GetRelayActionReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("动作报告"), tr("读动作报告失败!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
		return FALSE;
	}

	// 计算报告总个数
	Num = (wTail-wHead+MAX_RACT_NUM )%MAX_RACT_NUM;
	
	m_nAllItemNum = Num;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无记录"));

		index = AddRow(str);
		SetItemData(index, HMI_LIST_NORECORD);
		
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(index, 1, "");
		return TRUE;
	}

	
	int nItemCount = 0;
	WORD wReportItem;

	//一次只读一屏可以显示的, 最新的显示在最上面

	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		wReportItem = (wTail - 1 - i+MAX_RACT_NUM )%MAX_RACT_NUM;
		rc = DB_ReadRelayActionReport(&tRelayActionEv, wReportItem);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox(tr("动作报告"), tr("读动作报告失败!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
			return FALSE;
		}
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
			rc = DB_GetREDispByInf(inf, &tReDisp);
			if(DB_ERR_OK != rc)
			{
				HMIMsgBox(tr("动作报告"), tr("读动作报告失败!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
				return FALSE;
			}

			//序号显示从1开始，以符合用户习惯
			
			sprintf(str, "%02d %s %s", i+1, time,  TR_GetREName(&tReDisp,inf));
//			if(tRelayActionEv.tEvent.tAction.bStatus)
//				sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("发出"));
//			else
//				sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("复归"));
			
			index = AddRow(str);

//@			offset += wItemLen;

			wActionIndex = MAKEWORD(0,wReportItem);

			SetItemData(index, wActionIndex);

			if((++nItemCount) == m_nPageSize)
				break;

//@			//计算下一条分报告的位置
//@			pEvent = (TRelayEventHead *)(tRelayActionEv.aEvent+offset);
//@		}
	}
	return TRUE;
}

BOOL CHmiDataList::ShowAlarmReportDB()
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

	WORD wHead,wTail;

	// 读头尾指针
	rc = DB_GetRelayAlarmReportHead(&wHead);
	rc |= DB_GetRelayAlarmReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("告警报告"), tr("读告警报告失败!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
		return FALSE;
	}

	// 计算报告总个数
	Num = (wTail-wHead+MAX_RALM_NUM )%MAX_RALM_NUM;
	m_nAllItemNum = Num;
	LONG nItem;
	WORD wReportIndex;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无记录"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("告警报告"), tr("读告警报告失败!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
		return FALSE;
	}

	//告警报告无分报告
	
	int nItemCount = 0;
	//一次只读一屏可以显示的
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		wReportIndex = (wTail -1 -i + MAX_RALM_NUM)%MAX_RALM_NUM;

		rc = DB_ReadRelayAlarmReport(&tAlarmReport, wReportIndex);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox(tr("告警报告"), tr("读告警报告失败!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
			return FALSE;
		}
		inf = tAlarmReport.tEvent.tAction.nINF;
		abstime.Hi = tAlarmReport.tEvent.tAction.AbsTimeHi;
		abstime.Lo = tAlarmReport.tEvent.tAction.AbsTimeLo;
		AbsTimeTo(&abstime, &systime);

		sprintf(time, "%02d/%02d %02d:%02d",   systime.Month, systime.Day,
			systime.Hour, systime.Minute);
		DB_GetREDispByInf(inf, &tReDisp);

		//显示从1开始
		sprintf(str, "%02d %s %s", i+1, time,  TR_GetREName(&tReDisp,inf));
/*		if(tAlarmReport.tEvent.tAction.bStatus)
			sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("发出"));
		else
			sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("复归"));

*/
		nItem = AddRow(str);
		SetItemData(nItem,wReportIndex);

		if((++nItemCount) == m_nPageSize)
			break;

	}
	return TRUE;
}
BOOL CHmiDataList::ShowPortPara()
{
	char str[64],str1[16];
	int i;
	WORD num,sernum,cannum,ethnum;
	WORD sn = 0;
		
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();	
	m_nAllItemNum = sernum+cannum+ethnum;

	SetColumnCount(2);
	LONG width = this->GetXSize();// width得到的是像素宽度
	SetColPos(1, width/8-WIDTH_DI_FILTERTIME-3); //SetColPos 设置的是字符位置
	SetColPos(2, width/8-2);

	int nItemCount = 0, nItem = 0;

	int number = m_nDataPageNum*m_nPageSize;
	if((sernum))
	{		
		TSerialPara tSerial;
		for(LONG j=0;j<sernum;j++)
		{
			if(j<number)
			{				
				sn++;
				continue;
			}
			DB_GetSerialPara(j,&tSerial);			
			sprintf(str, "%s%d","SER",tSerial.wPortID&0xFF);			
			nItem = AddRow(str);		
			SetItemData(nItem, CHmiApp::m_wpPortPara[sn]);			
			SetItemText(nItem,1,g_ProtocolInfo[CHmiApp::m_wpPortPara[sn]].strName);
			sn++;
		}
	}
	if(cannum)
	{		
		TCanBusPara tCan;
		for(LONG j=0;j<cannum;j++)
		{			
			if((j+sernum) <number)
			{				
				sn++;
				continue;
			}
			DB_GetCANPara(j,&tCan);			
			sprintf(str, "%s%d","CAN",tCan.wPortID&0xFF);			
			nItem = AddRow(str);			
			SetItemData(nItem, CHmiApp::m_wpPortPara[sn]);
			SetItemText(nItem,1,g_ProtocolInfo[CHmiApp::m_wpPortPara[sn]].strName);
			sn++;
		}
	}
	if(ethnum)
	{		
		TEthPortPara tEth;
		for(LONG j=0;j<ethnum;j++)
		{			
			if((j+sernum+cannum) <number)
			{				
				sn++;
				continue;
			}
			DB_GetETHPara(j,&tEth);	
			if((tEth.wPortID&0xF00)==0x300)				
				sprintf(str, "%s%s%d%s%d","ETHA,","link",tEth.wPortID&0xFF,",",tEth.dwSelfPort);
			if((tEth.wPortID&0xF00)==0x400)
				sprintf(str, "%s%s%d%s%d","ETHB,","link",tEth.wPortID&0xFF,",",tEth.dwSelfPort);
			nItem = AddRow(str);			
			SetItemData(nItem, CHmiApp::m_wpPortPara[sn]);
			SetItemText(nItem,1,g_ProtocolInfo[CHmiApp::m_wpPortPara[sn]].strName);
			sn++;
		}
	}	
	return TRUE;
}

BOOL CHmiDataList::ChgPortPara(LONG nOffset)
{
	if (m_nCurColumnIndex == -1)
	{
		return true;
	}
	char txt1[][32] = 
	{
		"NULL",
		"NET",
		"MAINT",
		"SHELL",
		"IEC103",
		"IEC101",
		"IEC104",
		"GM104",
		"ModRtu"
	};
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	
	//更改界面显示
	OnChgValue(nOffset, txt1, 9);
	
	//更改输入缓冲区的值
	pszVal = GetItemText(sel, 1);
	nItem = m_nDataPageNum*m_nPageSize + sel;
	if(strcmp(pszVal,txt1[0])==0)
		CHmiApp::m_wpPortPara[nItem] = 0;
	else if(strcmp(pszVal,txt1[1])==0)
		CHmiApp::m_wpPortPara[nItem] = 1;
	else if(strcmp(pszVal,txt1[2])==0)
		CHmiApp::m_wpPortPara[nItem] = 2;
	else if(strcmp(pszVal,txt1[3])==0)
		CHmiApp::m_wpPortPara[nItem] = 3;
	else if(strcmp(pszVal,txt1[4])==0)
		CHmiApp::m_wpPortPara[nItem] = 4;
	else if(strcmp(pszVal,txt1[5])==0)
		CHmiApp::m_wpPortPara[nItem] = 5;
	else if(strcmp(pszVal,txt1[6])==0)
		CHmiApp::m_wpPortPara[nItem] = 6;
	else if(strcmp(pszVal,txt1[7])==0)
		CHmiApp::m_wpPortPara[nItem] = 7;
	else if(strcmp(pszVal,txt1[8])==0)
		CHmiApp::m_wpPortPara[nItem] = 8;

	CHmiApp::m_bPortParaModified = TRUE;

	return TRUE;
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

//显示缓冲区中配置
BOOL CHmiDataList::ShowConfigBuf() //ConfigChangeFun()
{
	LONG rc;
	BYTE CFGStatus;
	TCFG_Disp tCfgDisp;
	char str[64];

	//配置查看
	m_nAllItemNum  = DB_GetCFGNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-3));
	int nItemCount = 0, nItem = 0;
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("无条目"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25修改 否则第二列回显示上次保存的数据
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	for(WORD i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
//@		rc = DB_GetCFGStatus(m_nAreaNo,i, CFGStatus);
		CFGStatus = CHmiApp::m_byNewCfg[i];
		rc = DB_GetCFGDisp(i, &tCfgDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("配置字"), tr("读配置字错误"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		if(CFGStatus == CFG_STATUS_SET)
		{
			sprintf(str, "%s",  tr("投"));
		}
		else
		{
			sprintf(str, "%s",  tr("退"));

		}

		nItem = AddRow(TR_GetCFGName(&tCfgDisp,i));
		SetItemData(nItem, CFGStatus);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}

//修改BI中wControl-D1是否产生soe
BOOL CHmiDataList::ChgBISOEParaBuf(int nOffset)
{
	//显示字符串
	char strDisp[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","产生"),
		QT_TRANSLATE_NOOP("CHmiDataList","不产生"),
	};
	
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset, strDisp, 2);
		
		//更改输入缓冲区的值
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		if (strcmp(pszVal,  tr(strDisp[0])) == 0)
		{
			CHmiApp::m_wpBIControl[nItem] |= BI_ATTR_WTPEVENT;
		}
		else
		{
			CHmiApp::m_wpBIControl[nItem] &= ~BI_ATTR_WTPEVENT;
		}
		CHmiApp::m_bBIControlModified = TRUE;
	}
	return TRUE;
}


//修改遥信取反缓冲区
BOOL CHmiDataList::ChgBINOBuf(int nOffset)
{
	if (m_nCurColumnIndex == -1)
	{
		return true;
	}
	char txt1[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","取反"),
		QT_TRANSLATE_NOOP("CHmiDataList","正常"),
	};
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	
	//更改界面显示
	OnChgValue(nOffset, txt1, 2);
	
	//更改输入缓冲区的值
	pszVal = GetItemText(sel, 1);
	nItem = m_nDataPageNum*m_nPageSize + sel;
	if (strcmp(pszVal,  tr(txt1[0], "CHmiDataList")) == 0)
	{
		CHmiApp::m_wpBIControl[nItem] |= BI_ATTR_CONVERT;
	}
	else
	{
		CHmiApp::m_wpBIControl[nItem] &= ~BI_ATTR_CONVERT;
	}
	CHmiApp::m_bBIControlModified = TRUE;

	return TRUE;
}

BOOL CHmiDataList::ChgDIPara_wClassBuf(LONG nOffset)
{
	char strDisp[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","遥信"),
		QT_TRANSLATE_NOOP("CHmiDataList","电度"),
	};


	LONG sel = GetCurSel();
	//不是可修改的两种类型
	LONG nItem;
	nItem = m_nDataPageNum*m_nPageSize + sel;
	if( (CHmiApp::m_wpDIClass[nItem] != DI_TYPE_SP) &&
		(CHmiApp::m_wpDIClass[nItem] != DI_TYPE_PLUS) )
		return  FALSE;
		
	
	
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset,strDisp,2);
		
		//更改输入缓冲区的值
		if (strcmp( tr(strDisp[0]), GetItemText(sel,1)) == 0)
			CHmiApp::m_wpDIClass[nItem]  = DI_TYPE_SP;
		else
			CHmiApp::m_wpDIClass[nItem]  = DI_TYPE_PLUS;
		
		CHmiApp::m_bDIClassModified = TRUE;
			
	}
	return TRUE;


}

BOOL CHmiDataList::ChgDIFilterTimeBuf(int nOffset)
{
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);
	LONG nItem;
	char* pszVal;
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset);

		//更改输入缓冲区的值
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_wpDIFilterTime[nItem] = atoi(pszVal);
		CHmiApp::m_bDIFilterTimeModified = TRUE;

	}
	return TRUE;
}
BOOL CHmiDataList::ChgSWValue() //更改软压板值
{

	char txt1[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","投"),
		QT_TRANSLATE_NOOP("CHmiDataList","退"),
	};

	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);

	OnChgValue(1, txt1, 2);

	LONG nItem;
	nItem = m_nDataPageNum*m_nPageSize + sel;

	if (strcmp( tr(txt1[0]), GetItemText(sel,1)) == 0)
	{
		CHmiApp::m_byNewSoftSwitch[nItem] = SW_STATUS_SET;
	}
	else
	{
		CHmiApp::m_byNewSoftSwitch[nItem] = SW_STATUS_CUT;		
	}

	return TRUE;
}
BOOL CHmiDataList::ChgCIConstBufValue(LONG nOffset)
{
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset);
		
		//更改输入缓冲区的值
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		//列表中只改变脉冲电度的脉冲常数，积分电度无脉冲常数
		CHmiApp::m_dwpCIConst[nItem] = atol(pszVal);
		CHmiApp::m_bCIConstModified = TRUE;
	}
	return TRUE;

}


BOOL CHmiDataList::ChgCIBaseBufValue(LONG nOffset)
{
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset);
		
		//更改输入缓冲区的值
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_dwpCIBase[nItem] = atol(pszVal);
		CHmiApp::m_bCIBaseModified = FALSE;
	}
	return TRUE;
}
BOOL CHmiDataList::ChgCFBufValue(LONG nOffset)
{
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset);

		//更改输入缓冲区的值
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_npCFList[nItem] = atoi(pszVal);
		CHmiApp::m_bCFListModified = TRUE;
	}
	return TRUE;
}


//把对配置的更改缓冲起来
BOOL CHmiDataList::ChgCfgValue()
{
	char txt1[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","投"),
		QT_TRANSLATE_NOOP("CHmiDataList","退"),
	};

	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);
	LONG nItem;

	OnChgValue(1, txt1, 2);

	nItem = m_nDataPageNum*m_nPageSize + sel;

	if (strcmp( tr(txt1[0]), GetItemText(sel,1)) == 0)
	{
		CHmiApp::m_byNewCfg[nItem] = CFG_STATUS_SET;
	}
	else
	{
		CHmiApp::m_byNewCfg[nItem] = CFG_STATUS_CLR;		
	}
	return TRUE;
}
//修改遥控保持时间
BOOL CHmiDataList::ChgBOTime(LONG nOffset)
{
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);
	LONG nItem;
	char* pszVal;
	if (m_nCurColumnIndex != -1)
	{
		//更改界面显示
		OnChgValue(nOffset);

		//更改输入缓冲区的值
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_wpBOTime[nItem] = atoi(pszVal);
		CHmiApp::m_bBOTimeModified = TRUE;
	}
	return TRUE;
}


