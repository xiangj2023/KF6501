/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiDataList.cpp
* 
*   ���ģ�飺
*
* 
*	����
* 
* 
* 
*   ����
*		-Init() ��ʼ�������� �����б��������ø���������Ӧ���������ÿɱ༭��
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-4-21		��ʼ����
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
//������������Ӧ�ַ���
char OperationStr[][32] =
{
	QT_TRANSLATE_NOOP("CHmiDataList","null"), //û��
	QT_TRANSLATE_NOOP("CHmiDataList","ϵͳ��λ"),//@	#define MISC_CLASS_RESET		1
	QT_TRANSLATE_NOOP("CHmiDataList","��װ�ļ�"),//@#define MISC_CLASS_DOWNFILE		2
	QT_TRANSLATE_NOOP("CHmiDataList","ͨѶ�¼�"),//@#define MISC_CLASS_COMM			3
	QT_TRANSLATE_NOOP("CHmiDataList","ң���¼�"),//@#define MISC_CLASS_BO			4
	QT_TRANSLATE_NOOP("CHmiDataList","ң���¼�"),//@#define MISC_CLASS_AO			5
	QT_TRANSLATE_NOOP("CHmiDataList","ͬ���¼�"),//@#define MISC_CLASS_SYNC			6
	QT_TRANSLATE_NOOP("CHmiDataList","�޸Ķ�ֵ"),//@#define MISC_CLASS_SET			7
	QT_TRANSLATE_NOOP("CHmiDataList","�޸Ķ�ֵ����"),//@#define MISC_CLASS_SETAREA		8
	QT_TRANSLATE_NOOP("CHmiDataList","�޸���ѹ��"),//@#define MISC_CLASS_SW			9
	QT_TRANSLATE_NOOP("CHmiDataList","�޸�����"),//@#define MISC_CLASS_CFG			10
	QT_TRANSLATE_NOOP("CHmiDataList","�޸�ϵ��"),//@#define MISC_CLASS_CF			11
	QT_TRANSLATE_NOOP("CHmiDataList","�޸Ĳ���"),//@#define MISC_CLASS_PARA			12
	QT_TRANSLATE_NOOP("CHmiDataList","��Դ�¼�"),//@#define MISC_CLASS_POWER		13
	QT_TRANSLATE_NOOP("CHmiDataList","�޸ĵ�ȵ���"),//@#define MISC_CLASS_CLEARCI		14
	QT_TRANSLATE_NOOP("CHmiDataList","ң������"),//@#define MISC_CLASS_MODIAI		15
	QT_TRANSLATE_NOOP("CHmiDataList","ң����λ"),//@#define MISC_CLASS_MODIBI		16
	QT_TRANSLATE_NOOP("CHmiDataList","�������"),//@#define MISC_CLASS_MODICI		17

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
 * ��������: ���캯��
 * 
 * ����˵��: 
 *			- LONG dataType
 *			  ����: 
 *			  ˵��: �����б����ͣ�ȡֵ��EListType�Ķ���
 *
 *
 * ���ش���: 
 * 
 * ����: 
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

//��ʼ������, �ڸ����ڵ�OnShowWindow�е���
BOOL CHmiDataList::Init()
{
	m_nPageSize = m_nYSize/m_nItemHeight;
	m_nCurColumnIndex = -1;
	m_nCurItemIndex = 0;
	m_nDataPageNum = 0; 

	//���ݲ�ͬ�������ð�������Ӧ����,�ɱ༭�У� ���Ŀ��,����ʾ�Ķ�ֵ����
	switch(m_nDataType)
	{
	case DIFilterTime:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetEditableColum(1,WIDTH_DI_FILTERTIME); //WORD 5λ��
		break;
	case BOTime:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetEditableColum(1,WIDTH_DI_FILTERTIME); //WORD 5λ��
		break;
	case CF:
		SetEditableColum(1, WIDTH_CF); //LONG 5λ��
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

		SetCurorWidth(16*2);  //ȡ���� ���� 2������
		SetEditableColum(1,1); 
		
		break;
	case SoePara:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		
		SetCurorWidth(16*3);  //������ ������ 3������
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
		//��������
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

		//���¿�����ʾ�Ķ�ֵ�б�
		HMIUpDataVisibleSetList(m_nAreaNo, &m_nAllItemNum, CHmiApp::m_wpVisibleSetList,TRUE);
		break;
	case ASQuery:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;
	case ConfigChange:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;

		SetCurorWidth(24);  //Ͷ, �� 1������
		SetEditableColum(1,1); 

		break;
	case SoftSwitch:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetCurorWidth(24);  //Ͷ, �� 1������
		SetEditableColum(1,1); 

		break;
	case SysStatus:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		break;

	case DIPara:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetCurorWidth(16*2); //2������
		SetEditableColum(1, 1);
		break;
	case PortPara:
		m_nUpDownFun = MoveFocus;
		m_nLeftRightFun = ChgPage;
		SetCurorWidth(16*3); //2������
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
	//�����ݿ������
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
		if (m_nAllItemNum%m_nPageSize) //���в���һҳ������
		{
			nMaxPageCount += 1;
		}

		if (m_nDataPageNum<0)
		{
			m_nDataPageNum = nMaxPageCount-1; //���ҳ��
		}
	}
	
	DWORD wParam;
	if (m_bHaveSelChgFun)
	{
		wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}
	
	//����Ѿ�������һҳ�ˣ� �����´����ݿ��ȡһҳ����
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
		if (m_nAllItemNum%m_nPageSize) //���в���һҳ������
		{
			nMaxPageCount += 1;
		}
		//���һҳ��
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

	//����Ѿ�������һҳ�ˣ� �����´����ݿ��ȡһҳ����
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

//������ʾ����Ŀ����ʼ�� 
LONG CHmiDataList::GetItemBase()
{
	return m_nPageSize*m_nDataPageNum;
}

BOOL CHmiDataList::OnLeft()
{

	//����Ǳ༭̬���ƶ����
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
	//���Ǳ༭̬�ƶ������ҳ
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
		if (m_nAllItemNum%m_nPageSize) //���в���һҳ������
		{
			nMaxPageCount += 1;
		}

		if (m_nDataPageNum<0)
		{
			m_nDataPageNum = nMaxPageCount-1; //���ҳ��
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
	
	//����Ѿ�������һҳ�ˣ� �����´����ݿ��ȡһҳ����
	if(m_nDataPageNum != nOldPageNum)
	{
		if(!GetDataFromDB())
		{
			m_pOwner->Close();
			return FALSE;
		}
		m_nCurItemIndex = 0; //������һҳ����ʾ��һҳ�ĵ�0����¼
	}
	
	OnPaint();
	UpdateWindow();
	return TRUE;
}
BOOL CHmiDataList::OnRight()
{
	//����Ǳ༭̬���ƶ����
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
		if (m_nAllItemNum%m_nPageSize) //���в���һҳ������
		{
			nMaxPageCount += 1;
		}
		//���һҳ��
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
	
	//����Ѿ�������һҳ�ˣ� �����´����ݿ��ȡһҳ����
	if(m_nDataPageNum != nOldPageNum)
	{
		if(!GetDataFromDB())
		{
			m_pOwner->Close();
			return FALSE;
		}
		m_nCurItemIndex = 0; //������һҳ����ʾ��һҳ�ĵ�0����¼
	}
	
	OnPaint();
	UpdateWindow();
	return TRUE;
}
BOOL CHmiDataList::OnAdd()
{
	//�����뽹�㣬 ������Ӽ���
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

	OnPaint();  //�����ڴ�
	UpdateWindow();  //�ڴ浽��ʾ��
	return TRUE;
}
BOOL CHmiDataList::OnSub()
{
	
	//�����뽹�㣬 ������Ӽ���
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
	OnPaint();  //�����ڴ�
	UpdateWindow();  //�ڴ浽��ʾ��
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
 * ��������: �����ݿⰴ���б����Ͷ�ȡ����
 * ����˵��: 
 * ���ش���: 
 * ����: 
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
		//��������
		Result = ShowActionReportDB();
		break;

	case Alarm:
		m_nFont = 12; //
		Result = ShowAlarmReportDB();
		break;

	case Start:
		//��������
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
		Result = ShowConfigDB(); //��ѯʱֱ�Ӵ����ݿ��ȡ
		break;
	case SettingQuery:
		sprintf(str, "%s:%d", tr("�鿴��ֵ��"),m_nAreaNo);
		m_pOwner->SetCaption(str);


		Result = ShowSettingQueryDB(m_nAreaNo);
		break;
	case ASQuery:
		Result = ShowSysSettingQueryDB();
		break;
			
	case ConfigChange:
		m_nCursorWidth = 24;
		Result = ShowConfigBuf(); //����ʱ��Ҫ������
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
//@	ConfigQuery, //10���ò�ѯ
//@	SettingQuery, //11��ֵ��ѯ
//@	ConfigChange, //12
//@	SoftSwitch, //13��ѹ��
//@	fourteen, // 14
//@	SysStatus, //15
//@	DIPara, //16�������ԣ���Ȼ�ң��
//@	DIFilterTime, //17ȥ��ʱ��
//@	DINor, //18�Ƿ�ȡ��
//@	CIBase, //19��ȵ���
//@	CIConst, //���峣��
//@	SoePara, //21soe���ԣ��Ƿ��¼
//@
//@	ActData, //22����������ϵ���
//@	AlarmData, //23�澯�������
//@	StartData, //24�����������
	return Result;

}



//��ʾ���ʵʱֵ
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
		HMIMsgBox( tr("������¼"), tr("��������¼����"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}
	
	//IsEmpty = DB_Misc_IsEmpty();
	m_nAllItemNum = num;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("�޼�¼"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		return TRUE;
	}
/*
	rc = DB_Misc_GetTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("������¼","CHmiDataList"), tr("��������¼����","CHmiDataList"),
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
			HMIMsgBox( tr("������¼"), tr("��������¼����"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}
		AbsTimeTo(&(tEvent.absTime) ,&out);

		sprintf(strTime, "%02d/%02d %02d:%02d:%02d", 
			 out.Month, out.Day, out.Hour, out.Minute,out.Second);


		switch(tEvent.nClass)
		{
			case MISC_CLASS_RESET: 				
				strcpy(rptname,tr("װ�ø�λ"));
				break;
			case MISC_CLASS_SETAREA:
				strcpy(rptname,tr("�л���ֵ��"));
				break;
			case MISC_CLASS_POWER:				
				if(tEvent.dwValue == 0)
					strcpy(rptname,tr("װ���ϵ�"));
				else
					strcpy(rptname,tr("װ�õ���"));
				break;

			case MISC_CLASS_PARA:
				if(tEvent.dwValue == DB_PARA_DAI)
					strcpy(rptname,tr("�޸�DAI����"));
				else if(tEvent.dwValue == DB_PARA_DI)
					strcpy(rptname,tr("�޸�DI����"));
				else if(tEvent.dwValue == DB_PARA_AI)
					strcpy(rptname,tr("�޸�AI����"));				
				else if(tEvent.dwValue == DB_PARA_BI)
					strcpy(rptname,tr("�޸�BI����"));
				else if(tEvent.dwValue == DB_PARA_CI)
					strcpy(rptname,tr("�޸�CI����"));
				else if(tEvent.dwValue == DB_PARA_BO)
					strcpy(rptname,tr("�޸�BO����"));
				else if(tEvent.dwValue == DB_PARA_AO)
					strcpy(rptname,tr("�޸�AO����"));
				else if(tEvent.dwValue == DB_PARA_SER)
					strcpy(rptname,tr("�޸�COM����"));				
				else if(tEvent.dwValue == DB_PARA_CAN)
					strcpy(rptname,tr("�޸�CAN����"));
				else if(tEvent.dwValue == DB_PARA_ETH)
					strcpy(rptname,tr("�޸�ETH����"));
				else if(tEvent.dwValue == DB_PARA_SYS)
					strcpy(rptname,tr("�޸�SYS����"));
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
					sprintf(str1,"%s%s",tr("ͨѶ�ָ�"),str2);
				else					
					sprintf(str1,"%s%s",tr("ͨѶ�ж�"),str2);				
				strcpy(rptname,str1);
				break;
			default:
				strcpy(rptname,tr(OperationStr[tEvent.nClass]));
				break;
				
		}
		//�����ʾ��1��ʼ
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
		HMIMsgBox( tr("SOE��¼"), tr("��SOE��¼����"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}
		
	m_nAllItemNum = num;
	
	int nItemCount =0, nItem = 0;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("�޼�¼"));
		
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
			HMIMsgBox( tr("SOE��¼"), tr("��SOE��¼����"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}
		
		//			DB_IncSOERead();
		//��Ҫ����ʱ������ʱ��ת������
		AbsTimeTo(&(tSOE.stTime) ,&out);
		if(tSOE.bStatus)
			sprintf(strStatus, "%s", "0-->1");
		else
			sprintf(strStatus, "%s", "1-->0");

		sprintf(str, "%02d)%02d/%02d/%02d %02d:%02d:%02d:%03d BI%02d %s", i, 
			out.Year-2000, out.Month, out.Day, out.Hour, out.Minute, out.Second, out.MSecond,
			tSOE.wNo+1, strStatus);
		nItem = AddRow(str);
		//������soe��ţ�
		SetItemData(nItem,wReportIndex);
		if((++nItemCount) == m_nPageSize)
			break;

	}
	SetFont(12);
	return TRUE;
}

//��ʾ���ݿ��е�����
BOOL CHmiDataList::ShowConfigDB()
{
	int nItemCount = 0, nItem = 0;
	LONG rc;
	BYTE CFGStatus;
	TCFG_Disp tCfgDisp;
	char str[64];
	WORD i;

	//���ò鿴
	m_nAllItemNum = DB_GetCFGNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-3));
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	//һ��ֻ��һ��������ʾ��
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		rc = DB_GetCFGStatus(m_nAreaNo,i, CFGStatus);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("���ò鿴"), tr("�����ô���"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		rc = DB_GetCFGDisp(i, &tCfgDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("���ò鿴"), tr("�����ô���"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		if(CFGStatus == CFG_STATUS_SET)
		{
			sprintf(str, "%s",  tr("Ͷ"));

		}
		else
		{
			sprintf(str, "%s",  tr("��"));
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
		sprintf(str, "%s",  tr("����Ŀ"));

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
			HMIMsgBox( tr("��ѹ��"), tr("����ѹ�����"),
				CHmiApp::m_pMainWnd,CHmiApp::m_pMainWnd,HMI_MB_OK);
			return FALSE;
		}
		
		if(Status ==SW_STATUS_SET)
		{
			sprintf(str, "%s",  tr("Ͷ"));
		}
		else
		{
			sprintf(str, "%s",  tr("��"));
		}
		nItem =  AddRow(TR_GetSWName(&tswDisp,i));
		SetItemText(nItem,1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;

}

//��ʾ�������е�ϵ��ֵ�� �ڵ��øú���֮ǰ��Ҫ��ϵ�����뻺������
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
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}
	
	int nItemCount = 0;
	//һ��ֻ��һ��������ʾ��
	char strFormat[8];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_CF, "d");
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		rc = DB_GetCFDisp(i, &tCFDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("ϵ���鿴"), tr("��ϵ������"),
				CHmiApp::m_pMainWnd,CHmiApp::m_pMainWnd,HMI_MB_OK);
			return FALSE;
		}
		nItem= AddRow(tCFDisp.strSymbol);
		
		sprintf(str, strFormat,  lCfValueList[i]);
		//��ʽ������		sprintf(str, "%04d",  lCfValueList[i]);

		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}

//��ʾ��������DI����Ҫ�ڵ��øú���֮ǰ���ʵ�ʱ�������ݿ��ȶ��뻺������
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
		
		sprintf(str, "%s%02d", tr("����"),i);
		nItem = AddRow(str);

		if(CHmiApp::m_wpDIClass[i] == DI_TYPE_PLUS)
			sprintf(str, "%s",  tr("���"));
		else if(CHmiApp::m_wpDIClass[i] == DI_TYPE_SP)
			sprintf(str, "%s",   tr("ң��"));
		else if(CHmiApp::m_wpDIClass[i] == DI_TYPE_INNER)
			sprintf(str, "%s",   tr("�ڲ�ң��"));
		
		else if(CHmiApp::m_wpDIClass[i] == DI_TYPE_VIR)
			sprintf(str, "%s",   tr("��ң��"));
		else
			sprintf(str, "%s",   tr("����"));
			
		SetItemText(nItem, 1, str);

		if((++nItemCount) == m_nPageSize)
			break;

	}
	return TRUE;
}
//��ʾ��������ֱ�Ӳ�����ȥ��ʱ��
BOOL CHmiDataList::ShowDIPara_wFilterTimeBuf()
{
	char str[64];
	int i;

	m_nAllItemNum = DB_GetDINum();
	SetColumnCount(3);
	LONG width = this->GetXSize();// width�õ��������ؿ��
	SetColPos(1, width/8-WIDTH_DI_FILTERTIME-3); //SetColPos ���õ����ַ�λ��
	SetColPos(2, width/8-2);

	char strFormat[16];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_DI_FILTERTIME, "d");
	
	
	int nItemCount = 0, nItem = 0;

	//��ʾ�������е�����
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
//BI�Ƿ������ʱ���SOE wControl��D1�� ��ʾ�������е�ֵ
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
	//һ��ֻ��һ��������ʾ��
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI����"), tr("��BI��������"),
				CHmiApp::m_pMainWnd,CHmiApp::m_pMainWnd,HMI_MB_OK);
			return FALSE;
		}
		
		
		if (CHmiApp::m_wpBIControl[i]&BI_ATTR_WTPEVENT)
			sprintf(str, "%s"  ,  tr("����"));
		else
			sprintf(str, "%s" ,  tr("������"));
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
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	int nItemCount = 0;
	//һ��ֻ��һ��������ʾ��
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI����"), tr("��BI��������!"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		
		if (CHmiApp::m_wpBIControl[i]&BI_ATTR_CONVERT)
			sprintf(str, "%s"  ,  tr("ȡ��"));
		else
			sprintf(str, "%s" ,  tr("����"));
		
		nItem = AddRow(TR_GetBIName(&tBIPara,i));
		SetItemData(nItem, CHmiApp::m_wpBIControl[i]);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}
//��ʾ�������еĵ�ȵ���
BOOL CHmiDataList::ShowCIBaseBuf()
{
	TCIPara tCIPara;
	char str[64];
	int i;
	SetColumnCount(2);
	LONG width = this->GetXSize();// width�õ��������ؿ��
	LONG rc;
	
	char strFormat[8];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_CI_BASE, "d");

	m_nAllItemNum = DB_GetCINum();
	SetColumnCount(2);
	SetColPos(1, (width/8-WIDTH_CI_BASE));

	int nItemCount = 0, nItem =0;
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}
	
	//һ��ֻ��һ��������ʾ��
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetCIPara(i, &tCIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("CI����"), tr("��CI��������"),
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

//��ʾ�������е����峣��
BOOL CHmiDataList::ShowCIConstBuf()
{
	TCIPara tCIPara;
	char str[64];
	int i;
	SetColumnCount(2);
	LONG rc;
	
	char strFormat[16];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_CI_CONST, "d");
	
	//��ȵ�ǰ��·�ǻ��ֵ�ȣ����ֵ��û�����峣��
	m_nAllItemNum = DB_GetCINum()-INTERGRAL_KWH_COUNT;

	
	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-WIDTH_CI_CONST));

	int nItemCount = 0, nItem = 0;
	
	if (m_nAllItemNum  <= 0)
	{
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	//һ��ֻ��һ��������ʾ��
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetCIPara(i+INTERGRAL_KWH_COUNT, &tCIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("CI����"), tr("��CI��������"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		nItem = AddRow(tCIPara.strSymbol);

		sprintf(str, strFormat, CHmiApp::m_dwpCIConst[i]);//ע����ֵ�������峣��
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
	char strFormat[32], str[64]; // ��ʾ��ʽ

	//������ֵ��ʾ�б����
	char strSetDataName[MAX_DISP_COUNT][HMI_DBSTRING_SIZE];
	WORD wSetNameCount;
	int nValueIndex;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("�޹��ö�ֵ"));
		
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
			HMIMsgBox( tr("���ö�ֵ"), tr("�����ö�ֵ����"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		nItem = AddRow(TR_GetSysSETName(&tSysSET_Disp,i));
		rc = DB_GetSystemSETFloatValue(i, &fVal);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("���ö�ֵ"), tr("�����ö�ֵ����"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}

		//2010-9-9
		//�и�����ֵȡֵ�����ģ���ȡ
		BOOL bDataNameOk = FALSE;
		wSetNameCount = 0;
		if(tSysSET_Disp.nItemNum >0)
		{
			//kang 2012-10-31 
//			rc = DB_GetSystemSETDataName(i, &strSetDataName[0][0],wSetNameCount);
			bDataNameOk = TR_GetSysSETDataName(&tSysSET_Disp, i,&strSetDataName[0][0],wSetNameCount);

			
			if(!bDataNameOk)
			{
				HMIMsgBox( tr("���ö�ֵ�鿴"), tr("�����ö�ֵȡֵ��������"),
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
		sprintf(str, "%s",  tr("�޿���ʾ�Ķ�ֵ"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
		SetItemText(nItem, 1, "");

		return TRUE;

	}
	
	//�����ж�ֵ��ֵ
	rc = DB_GetSETFloatValue(nAreaNo, SET_VALUE_ALL, fSETValue);	
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("��ֵ�"), tr("����ֵ����"),
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
		//ֻ��ʾ����Ͷ���˵Ķ�ֵ
		nSetIndex = CHmiApp::m_wpVisibleSetList[i];
		rc = DB_GetSETDisp(nSetIndex, &tSETDisp);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("��ֵ�鿴"), tr("����ֵ����"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		//2010-9-9
		//�ж�ֵȡֵ�����ģ���ȡ
		wSetNameCount = 0;
		if(tSETDisp.nItemNum >0)
		{
			//2012-10-31 kang
			BOOL bSetDataOk = TR_GetSETDataName(&tSETDisp,nSetIndex, &strSetDataName[0][0],wSetNameCount);
			
			if(!bSetDataOk)
			{
				HMIMsgBox( tr("��ֵ�鿴"), tr("����ֵȡֵ��������"),
					&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
				return FALSE;
			}
		}

		j = AddRow(TR_GetSETName(&tSETDisp,nSetIndex));


		nSetValue = fSETValue[nSetIndex]; //
		//2010-9-9����Ҫ����ʾ�ķ�ʽ�����жϣ� ������б�򣬻���Ҫ��ʾ��������������ֵ
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
//��ʾң��ʵʱֵ
BOOL CHmiDataList::ShowBIValue()
{
	LONG  i,rc,nItem ;
	BYTE val;
	TBIPara tBIPara;
	m_nAllItemNum = DB_GetBINum();
	char str[64];	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
		SetItemText(nItem, 1, "");
		return TRUE;
	}


	int nItemCount =0;
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		rc = DB_GetBIPara(i,&tBIPara);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI�鿴"), tr("��BI��������"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		rc = DB_ReadBI(i, &val);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("BI�鿴"), tr("��BIֵ����"),
				&CLcdApp::m_WndInputSetting,&CLcdApp::m_WndInputSetting,HMI_MB_OK);
			return FALSE;
		}
		
		if(val == DB_BI_OPEN)
		{
			sprintf(str,"%d%s \t\t\t%s", i, tBIPara.strSymbol , "��");
			
		}
		else
		{
			sprintf(str,"%d%s \t\t\t%s", i, tBIPara.strSymbol, "��");
		}
		nItem = AddRow(str);
		if((++nItemCount) == m_nPageSize) //ֵ��ȡ��ǰҳ������
			break;

	}
	return TRUE;
}

//�������г��ڱ���ʱ��
BOOL CHmiDataList::ShowBOLastTimeBuf()
{
	char str[64];
	int i;

	m_nAllItemNum = DB_GetBONum();
	
	SetColumnCount(3);
	LONG width = this->GetXSize();// width�õ��������ؿ��
	SetColPos(1, width/8-WIDTH_DI_FILTERTIME-3); //SetColPos ���õ����ַ�λ��
	SetColPos(2, width/8-2);

	char strFormat[16];
	sprintf(strFormat, "%s%d%s", "%0", WIDTH_DI_FILTERTIME, "d");
	
	
	int nItemCount = 0, nItem = 0;
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	//��ʾ�������е�����
	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		sprintf(str, "%s%d",  tr("ң��"), i+1);
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

	// ��ͷβָ��
	rc = DB_GetRelayStartReportHead(&wHead);
	rc |= DB_GetRelayStartReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("��������"), tr("��ȡ��������ʧ��!"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}

	// ���㱨���ܸ���
	num = (wTail-wHead+MAX_RSTT_NUM )%MAX_RSTT_NUM;
	
	m_nAllItemNum = num;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("�޼�¼"));
		
		index = AddRow(str);
		SetItemData(index, HMI_LIST_NORECORD);
		
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
		SetItemText(index, 1, "");
		return TRUE;
	}
	int nItemCount =0;
	rc = DB_GetRelayStartReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox( tr("��������"), tr("��ȡ��������ʧ��!"),
			&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
		return FALSE;
	}

	for(i=m_nDataPageNum*m_nPageSize; i<m_nAllItemNum; i++)
	{
		wReportIndex = (wTail -i -1 + MAX_RSTT_NUM)%MAX_RSTT_NUM;

		rc = DB_ReadRelayStartReport(&tStartReport, wReportIndex);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox( tr("��������"), tr("��ȡ��������ʧ��!"),
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
			HMIMsgBox( tr("��������"), tr("��ȡ��������ʧ��!"),
				&CLcdApp::m_WndReport,&CLcdApp::m_WndReport,HMI_MB_OK);
			return FALSE;
		}

	
		//�����ʾ��1��ʼ
		sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr(""));	

		index = AddRow(str);
		SetItemData(index, wReportIndex);
		if((++nItemCount) == m_nPageSize)
			break;

	}
	return TRUE;
}


//08-12-24:17:15 �Ğ��б�ֻ��ʾ�ܱ��棬 ����ʾ�ֱ��棬 ����Ӧ�ô����µĿ�ʼ��ʾ��
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
	WORD Num; //�ܱ������
	WORD inf;
//@	WORD offset =0;
	WORD FDNum;
	LONG index;
	WORD wActionIndex; //���б��������б��涯�������ţ� ���ֽڴ�������ţ� ���ֽڴ���ֱ������

	WORD wHead,wTail;

	// ��ͷβָ��
	rc = DB_GetRelayActionReportHead(&wHead);
	rc |= DB_GetRelayActionReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("��������"), tr("����������ʧ��!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
		return FALSE;
	}

	// ���㱨���ܸ���
	Num = (wTail-wHead+MAX_RACT_NUM )%MAX_RACT_NUM;
	
	m_nAllItemNum = Num;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("�޼�¼"));

		index = AddRow(str);
		SetItemData(index, HMI_LIST_NORECORD);
		
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
		SetItemText(index, 1, "");
		return TRUE;
	}

	
	int nItemCount = 0;
	WORD wReportItem;

	//һ��ֻ��һ��������ʾ��, ���µ���ʾ��������

	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		wReportItem = (wTail - 1 - i+MAX_RACT_NUM )%MAX_RACT_NUM;
		rc = DB_ReadRelayActionReport(&tRelayActionEv, wReportItem);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox(tr("��������"), tr("����������ʧ��!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
			return FALSE;
		}
		pEvent = &tRelayActionEv.tEvent.tAction; //ָ��ĳ���ֱ���

//@		offset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
//@
//@		ItemNum = tRelayActionEv.tEvent.Num; //�õ��ֱ������
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
				HMIMsgBox(tr("��������"), tr("����������ʧ��!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
				return FALSE;
			}

			//�����ʾ��1��ʼ���Է����û�ϰ��
			
			sprintf(str, "%02d %s %s", i+1, time,  TR_GetREName(&tReDisp,inf));
//			if(tRelayActionEv.tEvent.tAction.bStatus)
//				sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("����"));
//			else
//				sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("����"));
			
			index = AddRow(str);

//@			offset += wItemLen;

			wActionIndex = MAKEWORD(0,wReportItem);

			SetItemData(index, wActionIndex);

			if((++nItemCount) == m_nPageSize)
				break;

//@			//������һ���ֱ����λ��
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

	// ��ͷβָ��
	rc = DB_GetRelayAlarmReportHead(&wHead);
	rc |= DB_GetRelayAlarmReportTail(&wTail);
	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("�澯����"), tr("���澯����ʧ��!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
		return FALSE;
	}

	// ���㱨���ܸ���
	Num = (wTail-wHead+MAX_RALM_NUM )%MAX_RALM_NUM;
	m_nAllItemNum = Num;
	LONG nItem;
	WORD wReportIndex;

	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("�޼�¼"));
		
		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
		SetItemText(nItem, 1, "");
		return TRUE;
	}

	if(DB_ERR_OK != rc)
	{
		HMIMsgBox(tr("�澯����"), tr("���澯����ʧ��!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
		return FALSE;
	}

	//�澯�����޷ֱ���
	
	int nItemCount = 0;
	//һ��ֻ��һ��������ʾ��
	for(i=m_nDataPageNum*m_nPageSize;i<m_nAllItemNum;i++)
	{
		wReportIndex = (wTail -1 -i + MAX_RALM_NUM)%MAX_RALM_NUM;

		rc = DB_ReadRelayAlarmReport(&tAlarmReport, wReportIndex);
		if(DB_ERR_OK != rc)
		{
			HMIMsgBox(tr("�澯����"), tr("���澯����ʧ��!"), &CLcdApp::m_WndReport,&CLcdApp::m_WndReport);
			return FALSE;
		}
		inf = tAlarmReport.tEvent.tAction.nINF;
		abstime.Hi = tAlarmReport.tEvent.tAction.AbsTimeHi;
		abstime.Lo = tAlarmReport.tEvent.tAction.AbsTimeLo;
		AbsTimeTo(&abstime, &systime);

		sprintf(time, "%02d/%02d %02d:%02d",   systime.Month, systime.Day,
			systime.Hour, systime.Minute);
		DB_GetREDispByInf(inf, &tReDisp);

		//��ʾ��1��ʼ
		sprintf(str, "%02d %s %s", i+1, time,  TR_GetREName(&tReDisp,inf));
/*		if(tAlarmReport.tEvent.tAction.bStatus)
			sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("����"));
		else
			sprintf(str, "%02d %s %s%s", i+1, time,  TR_GetREName(&tReDisp,inf),  tr("����"));

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
	LONG width = this->GetXSize();// width�õ��������ؿ��
	SetColPos(1, width/8-WIDTH_DI_FILTERTIME-3); //SetColPos ���õ����ַ�λ��
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
	
	//���Ľ�����ʾ
	OnChgValue(nOffset, txt1, 9);
	
	//�������뻺������ֵ
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

//��ʾ������������
BOOL CHmiDataList::ShowConfigBuf() //ConfigChangeFun()
{
	LONG rc;
	BYTE CFGStatus;
	TCFG_Disp tCfgDisp;
	char str[64];

	//���ò鿴
	m_nAllItemNum  = DB_GetCFGNum();

	SetColumnCount(2);
	LONG width = this->GetXSize();
	SetColPos(1, (width/8-3));
	int nItemCount = 0, nItem = 0;
	
	if (m_nAllItemNum == 0)
	{
		sprintf(str, "%s",  tr("����Ŀ"));

		nItem = AddRow(str);
		SetItemData(nItem, HMI_LIST_NORECORD);
		
		//2011-8-25�޸� ����ڶ��л���ʾ�ϴα��������
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
			HMIMsgBox( tr("������"), tr("�������ִ���"),
				&CLcdApp::m_WndSetting,&CLcdApp::m_WndSetting,HMI_MB_OK);
			return FALSE;
		}
		
		if(CFGStatus == CFG_STATUS_SET)
		{
			sprintf(str, "%s",  tr("Ͷ"));
		}
		else
		{
			sprintf(str, "%s",  tr("��"));

		}

		nItem = AddRow(TR_GetCFGName(&tCfgDisp,i));
		SetItemData(nItem, CFGStatus);
		SetItemText(nItem, 1, str);
		if((++nItemCount) == m_nPageSize)
			break;
	}
	return TRUE;
}

//�޸�BI��wControl-D1�Ƿ����soe
BOOL CHmiDataList::ChgBISOEParaBuf(int nOffset)
{
	//��ʾ�ַ���
	char strDisp[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","����"),
		QT_TRANSLATE_NOOP("CHmiDataList","������"),
	};
	
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	if (m_nCurColumnIndex != -1)
	{
		//���Ľ�����ʾ
		OnChgValue(nOffset, strDisp, 2);
		
		//�������뻺������ֵ
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


//�޸�ң��ȡ��������
BOOL CHmiDataList::ChgBINOBuf(int nOffset)
{
	if (m_nCurColumnIndex == -1)
	{
		return true;
	}
	char txt1[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","ȡ��"),
		QT_TRANSLATE_NOOP("CHmiDataList","����"),
	};
	char* pszVal;
	LONG sel = GetCurSel();
	LONG nItem;
	
	//���Ľ�����ʾ
	OnChgValue(nOffset, txt1, 2);
	
	//�������뻺������ֵ
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
		QT_TRANSLATE_NOOP("CHmiDataList","ң��"),
		QT_TRANSLATE_NOOP("CHmiDataList","���"),
	};


	LONG sel = GetCurSel();
	//���ǿ��޸ĵ���������
	LONG nItem;
	nItem = m_nDataPageNum*m_nPageSize + sel;
	if( (CHmiApp::m_wpDIClass[nItem] != DI_TYPE_SP) &&
		(CHmiApp::m_wpDIClass[nItem] != DI_TYPE_PLUS) )
		return  FALSE;
		
	
	
	if (m_nCurColumnIndex != -1)
	{
		//���Ľ�����ʾ
		OnChgValue(nOffset,strDisp,2);
		
		//�������뻺������ֵ
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
		//���Ľ�����ʾ
		OnChgValue(nOffset);

		//�������뻺������ֵ
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_wpDIFilterTime[nItem] = atoi(pszVal);
		CHmiApp::m_bDIFilterTimeModified = TRUE;

	}
	return TRUE;
}
BOOL CHmiDataList::ChgSWValue() //������ѹ��ֵ
{

	char txt1[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","Ͷ"),
		QT_TRANSLATE_NOOP("CHmiDataList","��"),
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
		//���Ľ�����ʾ
		OnChgValue(nOffset);
		
		//�������뻺������ֵ
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		//�б���ֻ�ı������ȵ����峣�������ֵ�������峣��
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
		//���Ľ�����ʾ
		OnChgValue(nOffset);
		
		//�������뻺������ֵ
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
		//���Ľ�����ʾ
		OnChgValue(nOffset);

		//�������뻺������ֵ
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_npCFList[nItem] = atoi(pszVal);
		CHmiApp::m_bCFListModified = TRUE;
	}
	return TRUE;
}


//�Ѷ����õĸ��Ļ�������
BOOL CHmiDataList::ChgCfgValue()
{
	char txt1[][32] = 
	{
		QT_TRANSLATE_NOOP("CHmiDataList","Ͷ"),
		QT_TRANSLATE_NOOP("CHmiDataList","��"),
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
//�޸�ң�ر���ʱ��
BOOL CHmiDataList::ChgBOTime(LONG nOffset)
{
	LONG sel = GetCurSel();
	LONG pos = GetColPos(1);
	LONG nItem;
	char* pszVal;
	if (m_nCurColumnIndex != -1)
	{
		//���Ľ�����ʾ
		OnChgValue(nOffset);

		//�������뻺������ֵ
		pszVal = GetItemText(sel, 1);
		nItem = m_nDataPageNum*m_nPageSize + sel;
		CHmiApp::m_wpBOTime[nItem] = atoi(pszVal);
		CHmiApp::m_bBOTimeModified = TRUE;
	}
	return TRUE;
}


