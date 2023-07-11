// HmiMsgBox.cpp: implementation of the CHmiMsgBox class.
//
//////////////////////////////////////////////////////////////////////

//@#include "wingui.h"
//显示遥测一次，二次， 保护一次二次值的对话框
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			
#include "HmiMsgBox.h"
#include "translator.h"
#include "Lcdcfg.h"
#include "LcdApp.h"
#include "Relay_defs.h"
#include "logic.h"
#include "db_err.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern char OperationStr[][32];
extern WORD g_wLCDContrast;
extern int SysTempture;		//装置温度值,放大10倍
extern DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
extern DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常


WORD g_wEth = 0; //修改以太网A还是B？

extern 	WORD g_kCalPass(DWORD dwInput);
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器


extern DWORD dwAITaskID;



TComInfo g_BaudInfo[]={

	{0,"NULL"},
	{1,"150"},
	{2,"300"},
	{3,"600"},
	{4,"1200"},
	{5,"2400"},
	{6,"4800"},
	{7,"9600"},
	{8,"19200"},
	{9,"38400"}
};
TComInfo g_DataInfo[]={	
	{0,"8"},
	{1,"7"},
	{2,"6"},
	{3,"5"}
};
TComInfo g_VerifyInfo[]={	
	{0,"NONE"},
	{1,"EVEN"},
	{2,"ODD"}
};
TComInfo g_StopInfo[]={
	{0,"1"},
	{1,"2"}
};
TComInfo g_ProtocolInfo[]={
	{0,"NULL"},
	{1,"NET"},
	{2,"MAINT"},
	{3,"SHELL"},
	{4,"IEC103"},
	{5,"IEC101"},
	{6,"IEC104"},
	{7,"GM104"},
	{8,"ModRtu"}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_HMIMESSAGE_MAP(CHmiMsgBox,CHmiWnd)
//{{HMI_MSG_MAP(CHmiMsgBox)
ON_HMIBTN_ENTER(2, OnEnterBtnOk)
ON_HMIBTN_ENTER(3, OnEnterBtnCancel)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

CHmiMsgBox::CHmiMsgBox()
{
	m_BtnCancel.SetPosition(34,112, 99,137);
	m_BtnOk.SetPosition(128,112,203,137);
}
BOOL CHmiMsgBox::InitDlg()
{
	SetClassName("CHmiMsgBox");

	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);

	m_Stmsg.SetPosition(13,30,83,65,0);
	m_Stmsg.SetTabOrder(-1);
	m_Stmsg.SetFont(16);
	m_Stmsg.SetOwner(this);
	m_Stmsg.SetTimer(-1);
	m_pObjList[1] = &m_Stmsg;

	m_pObjList[2] = &m_BtnOk;
	m_BtnOk.SetTabOrder(2);
	m_BtnOk.SetCaption(tr("确认"));
	m_BtnOk.SetOwner(this);
	m_BtnOk.SetHaveEnterFun(TRUE);
	m_BtnOk.SetEnterWnd(NULL);
	m_BtnOk.SetFocus(FALSE);
		
	m_pObjList[3] = &m_BtnCancel;
	m_BtnCancel.SetTabOrder(3);
	m_BtnCancel.SetCaption(tr("取消"));
	m_BtnCancel.SetOwner(this);
	m_BtnCancel.SetHaveEnterFun(TRUE);
	m_BtnCancel.SetEnterWnd(NULL);
	m_BtnCancel.SetFocus(FALSE);

	m_BtnOk.SetPosition(34,112, 109,137);
	m_BtnCancel.SetPosition(128,112,203,137);

	SetPageNum(1);
	SetObjNum(3);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);

	SetFocus(&m_BtnOk);
	SetTimer(0);

	return TRUE;
}

CHmiMsgBox::~CHmiMsgBox()
{

}
void CHmiMsgBox::DoModal(const char *szCaption1,  const char* szMsg1, CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, 
						 LONG nType, YES_NO_PTR pYES_Fn, YES_NO_PTR pNO_Fn)
{
	InitDlg();
	m_BtnOk.SetEnterWnd(pOkWnd);
	m_BtnCancel.SetEnterWnd(pCancelWnd);
	SetEscWnd(pCancelWnd);
	//SetEnterWnd(pOkWnd); 2010-8-26 这样会引起处理流程混乱，去掉
	m_Stmsg.SetCaption(szMsg1);
	SetCaption(szCaption1);

	m_pFnNo = pNO_Fn;
	m_pFnYes = pYES_Fn;

	//是否显示cancel按钮
	if (nType == HMI_MB_OKCANCEL)
	{
		m_BtnCancel.SetVisible(TRUE);
		
		m_BtnOk.SetPosition(34,112, 109,137);
		m_BtnCancel.SetPosition(128,112,203,137);
	}
	else //默认为显示一个ok按钮的
	{
		m_BtnOk.SetPosition(128,112,203,137);
		
		m_BtnCancel.SetVisible(FALSE);
	}
	//2010-8-26, 去掉没有按钮的对话框，没有按钮的引起流程混乱
//	else
//		SetObjNum(1);
	Show();
}



void CHmiMsgBox::OnEnterBtnOk()
{
	//对话框的确认,取消按钮如果有处理函数的话，跳转到什么目标窗口由函数来决定，
	//不自动跳转 , 属性设置放在此处而不是放在domodal函数中是防止对话框嵌套的时候
	//更改混乱
	if(m_pFnYes)
		m_BtnOk.DisableAutoEnterWnd();
	else
		m_BtnOk.EnableAutoEnterWnd();
	

	if (m_pFnYes != NULL)
	{
		YES_NO_PTR pFnYes;//因为函数中可能调用CHmiMsgBox.domodal函数，修改m_pFnYes，引起混乱
		pFnYes = m_pFnYes;//所以复制为零时变量
		pFnYes(); 
	}
}

void CHmiMsgBox::OnEnterBtnCancel()
{
	//对话框的确认,取消按钮如果有处理函数的话，跳转到什么目标窗口由函数来决定，
	//不自动跳转属性设置放在此处而不是放在domodal函数中是防止对话框嵌套的时候
	//更改混乱
	if(m_pFnNo)
		m_BtnCancel.DisableAutoEnterWnd();
	else
		m_BtnCancel.EnableAutoEnterWnd();
	
	if (m_pFnNo != NULL)
	{
		YES_NO_PTR pFnNo;//因为函数中可能调用CHmiMsgBox.domodal函数，修改m_pFnYes，引起混乱
		pFnNo = m_pFnNo;//所以复制为零时变量
		pFnNo(); 
	}

}

BEGIN_HMIMESSAGE_MAP(CHmiPasswordBox,CHmiWnd)
//{{HMI_MSG_MAP(CHmiPasswordBox)
//ON_HMIWND_ENTER()
//ON_HMIWND_INIT()
///}}HMI_MSG
END_HMIMESSAGE_MAP()
/****************************************************************************************
 * 功能描述: 密码对话框
 * 参数说明: 
 *			- char *szCaption1 标题
 *			- char* szMsg1 文本
 *			- CHmiWnd *pPassOkWnd 密码正确跳转窗口
 *			- CHmiWnd *pCancelWnd 取消按钮跳转窗口
 *			- LONG nPassType =0 密码类型 0： 一般密码 1：管理密码
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/


void CHmiPasswordBox::DoModal(const char *szCaption1,  const char* szMsg1, CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType)
{
	InitDlg();
	m_pOKWnd = pPassOkWnd;
	SetEscWnd(pCancelWnd); //取消跳转直接设置， 确认跳转在判断密码正确后才设置
	m_StaticMsg.SetCaption(szMsg1);
	SetCaption(szCaption1);
	m_nType = nPassType;
	Show();
}


CHmiPasswordBox::CHmiPasswordBox()
{
	m_pOKWnd = NULL;
	m_pEnterWnd = NULL;
	m_pEscWnd = NULL;
}
CHmiPasswordBox::~CHmiPasswordBox()
{
	
}
BOOL CHmiPasswordBox::InitDlg()
{
	
	SetClassName("CHmiPasswordBox");
	SetCaption(tr("密码输入"));
	SetEscWnd(NULL);
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_EditPass.SetPosition(52,64,177,89,0);
	m_EditPass.SetTabOrder(1);
	m_pObjList[1] = &m_EditPass;
	m_EditPass.SetFont(16);
	m_EditPass.SetOwner(this);
	m_EditPass.SetCaption("0000");
	m_EditPass.SetMaxLen(4);
	m_EditPass.SetDecimalNum(0);
	m_EditPass.SetValueRange(9999,0);
	m_EditPass.SetIsShowNegative(FALSE);
	m_EditPass.SetUDFun(-1);
	m_EditPass.SetLRFun(0);
	m_EditPass.SetAddSubFun(3);
	m_EditPass.SetHaveUpDownFun(FALSE);
	m_EditPass.SetHaveLeftRightFun(FALSE);
	m_EditPass.SetHaveEnterFun(FALSE);
	m_EditPass.SetHaveAddSubFun(FALSE);
	m_EditPass.SetCursorPos(0);
	m_StaticMsg.SetPosition(28,41,133,61,0);
	m_StaticMsg.SetTabOrder(-2);
	m_pObjList[2] = &m_StaticMsg;
	m_StaticMsg.SetFont(16);
	m_StaticMsg.SetOwner(this);
	m_StaticMsg.SetCaption(tr("请输入密码："));
	m_StaticMsg2.SetPosition(18,133,228,153,0);
	m_StaticMsg2.SetTabOrder(-3);
	m_pObjList[3] = &m_StaticMsg2;
	m_StaticMsg2.SetFont(16);
	m_StaticMsg2.SetOwner(this);
	m_StaticMsg2.SetCaption(tr("按Enter键确认 按Esc取消"));
	m_StaticMsg2.SetTimer(0);

	m_StaticDynamic.SetFont(16);
	m_StaticDynamic.SetOwner(this);
	m_StaticDynamic.SetCaption(tr("动态码"));
	m_StaticDynamic.SetTimer(-1);
	m_StaticDynamic.SetPosition(18,99,95,119,0);
	m_pObjList[4] = &m_StaticDynamic;
	char Dynamic[8];
	m_DynamicValue.SetFont(16);
	m_DynamicValue.SetOwner(this);
	value = dwRelayMSCnt;
	sprintf(Dynamic, "%X", value);	
	m_DynamicValue.SetCaption(Dynamic);
	m_DynamicValue.SetTimer(-1);
	m_DynamicValue.SetPosition(125,99,200,119,0);	
	m_pObjList[5] = &m_DynamicValue;

	m_StaticMsg.SetTimer(-1);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(5);
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;
	SetTimer(0);
	b_dpasswordOK = FALSE;
	b_upasswordOK = FALSE;
	return TRUE;
}

BOOL CHmiPasswordBox::OnWndInit()
{
	return TRUE;
}
void CHmiPasswordBox::OnWndEnter()
{
	//判读密码不正确则不跳转
	char strPass[16];
	char superPass[16]="6501";
	const char* strInput = m_EditPass.GetCaption();
	b_dpasswordOK = FALSE;
	b_upasswordOK = FALSE;

	//动态码验证密码方式
	WORD calu;
	char str[16];
	calu = g_kCalPass(value);	
	sprintf(str, "%04d", calu);
	if(strcmp(str, strInput) == 0)
	{
		b_dpasswordOK = TRUE;
	}
	//用户密码验证方式,管理员密码拥有全部权限	
	if(m_nType == 3) //出厂数据需要用超级密码
	{
		if((strcmp(superPass, strInput) == 0))
		{
			SetEnterWnd(m_pOKWnd);		
			CHmiWnd::OnWndEnter();
			return;
		}
		return ;
	}


	
	LONG rc;
	rc = DB_GetAdminPassWord(strPass);	
	if((strcmp(strPass, strInput) == 0)||(b_dpasswordOK == TRUE))
	{
		SetEnterWnd(m_pOKWnd);		
		CHmiWnd::OnWndEnter();
		return;
	}
	if(m_nType == 0) //一般密码
	{
		rc = DB_GetUserPassWord(strPass);
		if((strcmp(strPass, strInput) == 0)||(b_dpasswordOK == TRUE))
		{
			SetEnterWnd(m_pOKWnd);		
			CHmiWnd::OnWndEnter();
		}
	}
}


//************窗口CHmiPassChgBox: 密码设置--压板密码*******************
BEGIN_HMIMESSAGE_MAP(CHmiPassChgBox,CHmiWnd)
//{{HMI_MSG_MAP(CHmiPassChgBox)
//ON_HMIWND_ENTER()
///}}HMI_MSG
END_HMIMESSAGE_MAP()


CHmiPassChgBox::~CHmiPassChgBox()
{
}
CHmiPassChgBox::CHmiPassChgBox()
{
}
void CHmiPassChgBox::DoModal(const char *szCaption1, CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType)
{
	InitDlg();
	
	m_pOKWnd = pPassOkWnd;
	SetEscWnd(pCancelWnd);
	SetCaption(szCaption1);
	m_nType = nPassType;
	Show();
}

BOOL CHmiPassChgBox::InitDlg()
{
	
	SetClassName("CHmiPassChgBox");
	SetCaption(tr("密码设置"));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditOldPass.SetPosition(126,31,221,51,0);
	m_EditOldPass.SetTabOrder(1);
	m_pObjList[1] = &m_EditOldPass;
	m_EditOldPass.SetFont(16);
	m_EditOldPass.SetOwner(this);
	m_EditOldPass.SetCaption("0000");
	m_EditOldPass.SetMaxLen(4);
	m_EditOldPass.SetDecimalNum(0);
	m_EditOldPass.SetValueRange(9999,0);
	m_EditOldPass.SetIsShowNegative(FALSE);
	m_EditOldPass.SetUDFun(-1);
	m_EditOldPass.SetLRFun(0);
	m_EditOldPass.SetAddSubFun(3);
	m_EditOldPass.SetHaveUpDownFun(FALSE);
	m_EditOldPass.SetHaveLeftRightFun(FALSE);
	m_EditOldPass.SetHaveEnterFun(FALSE);
	m_EditOldPass.SetHaveAddSubFun(FALSE);
	m_EditNewPass.SetPosition(126,67,221,87,0);
	m_EditNewPass.SetTabOrder(2);
	m_pObjList[2] = &m_EditNewPass;
	m_EditNewPass.SetFont(16);
	m_EditNewPass.SetOwner(this);
	m_EditNewPass.SetCaption("0000");
	m_EditNewPass.SetMaxLen(4);
	m_EditNewPass.SetDecimalNum(0);
	m_EditNewPass.SetValueRange(9999,0);
	m_EditNewPass.SetIsShowNegative(FALSE);
	m_EditNewPass.SetUDFun(-1);
	m_EditNewPass.SetLRFun(0);
	m_EditNewPass.SetAddSubFun(3);
	m_EditNewPass.SetHaveUpDownFun(FALSE);
	m_EditNewPass.SetHaveLeftRightFun(FALSE);
	m_EditNewPass.SetHaveEnterFun(FALSE);
	m_EditNewPass.SetHaveAddSubFun(FALSE);
	m_EditNewPass.SetFocus(FALSE);
	m_EditOldPass.SetCursorPos(0);
	m_EditNewPass.SetCursorPos(0);
	m_Static4836.SetPosition(20,33,115,58,0);
	m_Static4836.SetTabOrder(-4);
	m_pObjList[4] = &m_Static4836;
	m_Static4836.SetFont(16);
	m_Static4836.SetOwner(this);
	m_Static4836.SetCaption(tr("请输入原密码"));
	m_Static4836.SetTimer(-1);
	m_Static4839.SetPosition(20,73,115,93,0);
	m_Static4839.SetTabOrder(-3);
	m_pObjList[3] = &m_Static4839;
	m_Static4839.SetFont(16);
	m_Static4839.SetOwner(this);
	m_Static4839.SetCaption(tr("请输入新密码"));
	m_Static4839.SetTimer(-1);

	m_StaticDynamic.SetFont(16);
	m_StaticDynamic.SetOwner(this);
	m_StaticDynamic.SetCaption(tr("动态码"));
	m_StaticDynamic.SetTimer(-1);
	m_StaticDynamic.SetPosition(20,108,115,128,0);
	m_pObjList[5] = &m_StaticDynamic;

	
	m_DynamicValue.SetFont(16);
	m_DynamicValue.SetOwner(this);

	value = dwRelayMSCnt;
	sprintf(Dynamic, "%X", value);
	
	m_DynamicValue.SetCaption(Dynamic);
	m_DynamicValue.SetTimer(-1);
	m_DynamicValue.SetPosition(126,108,221,128,0);
	
	m_pObjList[6] = &m_DynamicValue;

	m_StaticMsg.SetPosition(16,137,231,157,0);
	m_StaticMsg.SetTabOrder(-5);
	m_pObjList[7] = &m_StaticMsg;
	m_StaticMsg.SetFont(16);
	m_StaticMsg.SetOwner(this);
	m_StaticMsg.SetCaption(tr("按Enter保存， 按Esc取消"));
	m_StaticMsg.SetTimer(-1);
	
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(6);
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;
	SetTimer(0);
	return TRUE;
}

void CHmiPassChgBox::OnWndEnter()
{
	//判读密码不正确则不跳转
	char strPass[16];
	LONG rc;
	const char* strInput = m_EditOldPass.GetCaption();
	const char* strInputNew = m_EditNewPass.GetCaption();
	BOOL bIsOldPassOk = FALSE;
	WORD calu;

	char str[16];
	calu = g_kCalPass(value);	
	sprintf(str, "%04d", calu);

	if(strcmp(str, strInput) == 0)
	{
		bIsOldPassOk = TRUE;
	}

	//是否超级密码
/*	if(strcmp(g_strSuperPass, strInput) == 0)
	{
		bIsOldPassOk = TRUE;
	}*/
	else
	{

		if(m_nType == 0) //一般密码
			rc = DB_GetUserPassWord(strPass);
		else
			rc = DB_GetAdminPassWord(strPass);
		
			
		if(strcmp(strPass, strInput) == 0)
			bIsOldPassOk = TRUE;
	}
	if(bIsOldPassOk)
	{
		//保存新密码
		if(m_nType == 0)
			DB_SetUserPassWord(strInputNew);
		else
			DB_SetAdminPassWord(strInputNew);
		SetEnterWnd(m_pOKWnd);
		Close();
		CHmiWnd::OnWndEnter();
		
	}
}

BOOL CHmiPassChgBox::OnWndInit()
{
	m_EditNewPass.SetValue(0);
	m_EditOldPass.SetValue(0);
	
	m_EditNewPass.UpdateData(FALSE);
	m_EditOldPass.UpdateData(FALSE);
	return TRUE;
}

//-------------CHmiInputDlg------------------
BEGIN_HMIMESSAGE_MAP(CHmiInputDlg,CHmiWnd)
//{{HMI_MSG_MAP(CHmiInputDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


void CHmiInputDlg::OnPaint()
{
	int i= 0;
	for (i=0; i<4; i++)
	{
		m_StBI[i*6+0].SetPosition(7, 34+i*34, 22, 49+i*34, m_nCurPageIndex);
		m_StBI[i*6+1].SetPosition(45, 34+i*34, 60, 49+i*34, m_nCurPageIndex);
		m_StBI[i*6+2].SetPosition(83, 34+i*34, 98, 49+i*34, m_nCurPageIndex);
		m_StBI[i*6+3].SetPosition(121, 34+i*34, 136, 49+i*34, m_nCurPageIndex);
		m_StBI[i*6+4].SetPosition(159, 34+i*34, 175, 49+i*34, m_nCurPageIndex);
		m_StBI[i*6+5].SetPosition(197, 34+i*34, 212, 49+i*34, m_nCurPageIndex);
	}

	for (i=0; i<24; i++)
	{	
		m_StBI[i].SetVisible(TRUE);
		m_StBI[i].SetDataSource(BI, i+m_nCurPageIndex*24);
		m_StBI[i].Init();
	}

	WORD num = DB_GetBINum();
	//看剩下的是否不够一页
	int nNext = num - m_nCurPageIndex*24;
	
	if (nNext <24)
	{
		SetObjNum(nNext);
	}
	else
	{
		SetObjNum(24);
	}
	CHmiWnd::OnPaint();
}

void CHmiInputDlg::DoModal(CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd)
{
	InitDlg();
	SetEscWnd(pCancelWnd);
	SetEnterWnd(pOkWnd);

	Show();
}

BOOL CHmiInputDlg::InitDlg()
{
	WORD i;
	
	SetClassName("CHmiInputDlg");
	SetCaption(tr("开入"));
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(2);
	SetLROffset(1);

	for (i=0; i<4; i++)
	{
		m_StBI[i*HMI_BI_PERLINE+0].SetPosition(7, 34+i*34, 22, 49+i*34, 0);
		m_StBI[i*HMI_BI_PERLINE+1].SetPosition(45, 34+i*34, 60, 49+i*34, 0);
		m_StBI[i*HMI_BI_PERLINE+2].SetPosition(83, 34+i*34, 98, 49+i*34, 0);
		m_StBI[i*HMI_BI_PERLINE+3].SetPosition(121, 34+i*34, 136, 49+i*34, 0);
		m_StBI[i*HMI_BI_PERLINE+4].SetPosition(159, 34+i*34, 175, 49+i*34, 0);
		m_StBI[i*HMI_BI_PERLINE+5].SetPosition(197, 34+i*34, 212, 49+i*34, 0);
	}

	for (i=0; i<24; i++)
	{
		m_StBI[i].SetOwner(this);
		m_StBI[i].SetFont(16);
		m_StBI[i].SetTabOrder(0-i-1);
		m_StBI[i].SetDataSource(BI, i);
		m_StBI[i].SetTimer(1);
		m_pObjList[i+1] = &(m_StBI[i]);
	}

	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);

	WORD num  = DB_GetBINum();
	LONG nPage = num/24;
	if (num%24!= 0)
	{
		nPage += 1;
	}
	
	SetPageNum(nPage);

	if (num <24)
	{
		SetObjNum(num);
	}
	else
	{
		SetObjNum(24);
	}

	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;
	
	//启动窗口定制器，只有窗口定时器启动了， 窗口内控件才能实时刷新
	SetTimer(1);
	return TRUE;
}

CHmiInputDlg::CHmiInputDlg()
{

}

CHmiInputDlg::~CHmiInputDlg()
{
	
}


/////////////////----------CHmiAIDlg-----------------
BEGIN_HMIMESSAGE_MAP(CHmiAIDlg,CHmiWnd)
//{{HMI_MSG_MAP(CHmiAIDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

//遥测显示
/****************************************************************************************
 * 功能描述: 显示遥测保护测量值等
 * 参数说明: 
 * 返回代码: 
 * 其它: 一共只有HMI_AI_PERPAGE个static, 根据当前的翻页位置来设置要显示的测量值序号SetDataSource
 *****************************************************************************************/
void CHmiAIDlg::OnPaint()
{
	WORD i;
	if(m_nAllItemNum<=5)
	{
		for(i=0;i<m_nAllItemNum;i++)
			m_StAi[i].SetPosition(68,29+i*26,179,49+i*26, m_nCurPageIndex);
		
	}
	else
	{
		for (i=0; i<5; i++)
		{
			m_StAi[2*i].SetPosition(0, 29+i*26, 111, 49+i*26, m_nCurPageIndex);
			m_StAi[2*i+1].SetPosition(116, 29+i*26, 228, 49+i*26, m_nCurPageIndex);
		}
	}
	//看剩下的是否不够一页
	int nLeft = m_nAllItemNum - m_nCurPageIndex*HMI_AI_PERPAGE;
	int nDispCount=0; 
	if (nLeft <HMI_AI_PERPAGE)
	{
		nDispCount = nLeft;
	}
	else
	{
		nDispCount = HMI_AI_PERPAGE;
	}
	SetObjNum(nDispCount);

	WORD wAiIndex;
	for (i=0; i<nDispCount; i++)
	{
		if (m_bIsAINoListValid)
			wAiIndex = m_wAiIndexList[i+m_nCurPageIndex*HMI_AI_PERPAGE];
		else
			wAiIndex = i;
		m_StAi[i].SetVisible(TRUE);
		m_StAi[i].SetDataSource(m_nDataType, wAiIndex);
		m_StAi[i].Init();

	}
		CHmiWnd::OnPaint();
}



/****************************************************************************************
 * 功能描述: 初始化对话框
 * 参数说明: 
 *			- LONG nDataType 显示的类型 hAI 一次遥测 lAI 二次遥测 CI 电度
 *			phAI保护一次值， lhAI 保护二次值
 *			WORD wAiNum 要显示的项目的数目
 *			WORD* wAiNoList 要显示的项目的序号列表
 *			wAiNum=0 或者 wAiNoList=NULL的时候，显示全部项目
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
BOOL CHmiAIDlg::InitDlg(LONG nDataType, WORD wAiNum, WORD* wAiNoList,char* cCaption)
{
	
	SetClassName("CHmiAIDlg");
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);

	//启动窗口定制器，只有窗口定时器启动了， 窗口内控件才能实时刷新
	SetTimer(1);
	
	for (int i=0; i<HMI_AI_PERPAGE; i++)
	{
		m_StAi[i].SetTabOrder(0-1-i);
		m_StAi[i].SetOwner(this);
		m_StAi[i].SetFont(16);
		m_StAi[i].SetTimer(1);
		m_pObjList[i+1] = &(m_StAi[i]);
	}
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;

	m_nDataType = nDataType;

	//不能超过最大可显示个数
	if (wAiNum>HMI_AI_MAXDISP)
	{
		wAiNum = HMI_AI_MAXDISP;
	}
	if(cCaption!='\0')
		SetCaption(cCaption);
	
	switch (m_nDataType)
	{
	case hAI:
//		SetCaption(tr("测量一次值"));
		m_nAllItemNum = DB_GetAINum();
		break;
	case lAI:
		m_nAllItemNum = DB_GetAINum();
//		SetCaption(tr("测量二次值"));
		break;
	case phAI:
		m_nAllItemNum = DB_GetRMNum();
//		SetCaption(tr("保护一次值"));
		break;
	case plAI:
		m_nAllItemNum =DB_GetRMNum();
//		SetCaption(tr("保护二次值"));
		break;
	case CI:
		m_nAllItemNum = DB_GetCINum();
//		SetCaption(tr("电度"));
		break;
	} //end case 
	
	if ((wAiNum <=0 )||(wAiNoList == NULL))
	{
		m_bIsAINoListValid = false;
	}
	else
	{
		m_nAllItemNum = wAiNum;
		memcpy(m_wAiIndexList, wAiNoList, wAiNum*sizeof(WORD));
		m_bIsAINoListValid = TRUE;
	}
	LONG nPage = m_nAllItemNum/HMI_AI_PERPAGE;
	if (m_nAllItemNum%HMI_AI_PERPAGE!= 0)
	{
		nPage += 1;
	}
	
	SetPageNum(nPage);
	m_nCurPageIndex = 0;
	return TRUE;
}


void CHmiAIDlg::DoModal(CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, LONG nDataType, WORD wAiNum, WORD* wAiNoList,char* cCaption)
{
	InitDlg(nDataType, wAiNum, wAiNoList,cCaption);
	SetEscWnd(pCancelWnd);
	SetEnterWnd(pOkWnd);
	Show();
}

CHmiAIDlg::CHmiAIDlg()
{
	m_bIsAINoListValid = false;

}

CHmiAIDlg::~CHmiAIDlg()
{
	
}
//--------------启动报告
//启动报告详情
CHmiStartDetailDlg::~CHmiStartDetailDlg()
{
}
CHmiStartDetailDlg::CHmiStartDetailDlg()
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
void CHmiStartDetailDlg::SetFDNum(LONG Num)
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
/****************************************************************************************
 * 功能描述: 显示启动报告详情窗口
 * 参数说明: 
 *			- CHmiWnd* pWndReturn  返回窗口，按esc键返回到pWndReturn窗口 
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
void CHmiStartDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	CHmiWnd::Show();
}

/****************************************************************************************
 * 功能描述: 从数据库读取启动报告
 * 参数说明: 
 *			- LONG ItemIndex 启动报告序号
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
BOOL CHmiStartDetailDlg::ReadReport(LONG ItemIndex, LONG DispIndex)
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
	m_bIsClosed = FALSE; //防止再运行一遍init
	
	//标题，序号是显示序号， 不是内部序号
	sprintf(str, "%s:%d", tr("启动报告"), DispIndex);
	SetCaption(str);

	rc	= DB_GetRelayStartReportNum(&Num);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}

	rc = DB_ReadRelayStartReport(&tStartReport, ItemIndex);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	
	pEvent = &tStartReport.tEvent.tAction;

	inf = pEvent->nINF;
	abstime.Hi = pEvent->AbsTimeHi;
	abstime.Lo = pEvent->AbsTimeLo;
	AbsTimeTo(&abstime, &systime);
		
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		systime.Year, systime.Month, systime.Day, 
		systime.Hour, systime.Minute, systime.Second, systime.MSecond);

	rc = DB_GetREDispByInf(inf, &tReDisp);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
		
	m_Static[3].SetCaption(str); //  动作时间

	m_Static[2].SetCaption(TR_GetREName(&tReDisp,inf)); //  名称

	SetFDNum(pEvent->FD_Num);
		

	LONG 		RMIndex;
	DWORD 		dwValue;
	float		fValue;
	TRM_Disp 	tRMDisp;
	BYTE		*pData;
	
	char sztmp[32];
	char strFormat[32];

	//故障电量显示一次值还是二次值
	BOOL bIsShowFirst = TRUE;
	bIsShowFirst = DB_IsFDShowFirst();
		
	for(i= 0; i<pEvent->FD_Num; i++)
	{
		pData = (BYTE *)(pEvent+1)+i*(sizeof(WORD)+sizeof(float))+2;
		dwValue = MAKEDWORD(pData[0],pData[1],pData[2],pData[3]);
		fValue = *((float *)&dwValue);
		
		RMIndex = tReDisp.wRMList[i];
		rc = DB_GetRMDisp(RMIndex, &tRMDisp); //得到该类型遥测的描述参数
		if(DB_ERR_OK != rc)
		{
			return FALSE;
		}
		
		if (bIsShowFirst)
		{
			sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nFirDec, "f%s");
			sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strFirUnit);
			
		}
		else
		{
			sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nSecDec, "f%s");
			sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strSecUnit);
		}
		
		
		m_Static[5+i].SetCaption(sztmp);
	
	}
	return TRUE;
}

/****************************************************************************************
 * 功能描述: 初始化启动报告详情窗口
 * 参数说明: 
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
BOOL CHmiStartDetailDlg::Init()
{
	SetClassName("CHmiStartDetailDlg");
	m_nCurPageIndex = 0;
	m_nCurTaborder = 1;

	SetCaption(tr("启动报告： No.1"));

	SetEscWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(13,30,83,45,0);
	m_Static[0].SetTabOrder(-5);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetTimer(-1);
	m_Static[0].SetCaption(tr("动作时间"));
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
	m_Static[2].SetCaption(tr("xx动作 "));
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(13,50,224,72,0); //109,52,224,72,0
	m_Static[3].SetTabOrder(-2);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetTimer(-1);
	m_Static[3].SetCaption(tr("2008.01.02 13:13:23.123"));
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
	SetTimer(0);
	

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
BEGIN_HMIMESSAGE_MAP(CHmiStartDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CHmiStartDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//------------动作报告详情---------------
CHmiActionDetailDlg::~CHmiActionDetailDlg()
{
}
CHmiActionDetailDlg::CHmiActionDetailDlg()
{
}


//显示窗口并设置返回窗口
void CHmiActionDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	CHmiWnd::Show();
}

void CHmiActionDetailDlg::OnWndLeftRight(LONG LeftRight)
{
	if(LeftRight >0)
		m_nSubItem++;
	else
		m_nSubItem--;
	
	CHmiWnd* pWnd = GetEscWnd();
	ReadReport(m_nItem, m_nSubItem, m_nDispIndex);

	SetEscWnd(pWnd);
	OnPaint();
	UpdateWindow();

}

/****************************************************************************************
 * 功能描述: 读动作报告
 * 参数说明: 
 *		-LONG ItemIndex 动作报告数据库标识序号， 读数据用
 *		-LONG subItemIndex	分报告号
 *		-LONG DispIndex	显示序号，显示用
 * 返回代码: true 读成功， false 读失败
 * 其它: 
 *****************************************************************************************/
BOOL CHmiActionDetailDlg::ReadReport(LONG ItemIndex, LONG subItemIndex, LONG DispIndex)
{
	char str[64];
	TAbsTime abstime;
	TSysTime systime;
	TRelayActionEv tRelayActionEv;
	TRelayEventHead *pEvent;
	TRE_Disp tReDisp;
	int i;
	LONG rc;

	WORD ItemNum; //报告的分报告个数
	WORD wItemLen;
	WORD FDNum; //故障电量个数
	WORD inf;
	WORD offset =0;

	Init();
	
	m_bIsClosed = FALSE; //防止再运行一遍init

	//查询目标大于总报告数,显示第一个
	if(MAX_RACT_NUM <= ItemIndex)
	{
		ItemIndex = 0;
	}

	m_nItem = ItemIndex; //当前显示的报告号
	m_nDispIndex = DispIndex;

	//找到总报告
	rc = DB_ReadRelayActionReport(&tRelayActionEv, ItemIndex);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
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

	//标题， 序号为显示序号
	sprintf(str, "%s:%d--%d/%d", tr("保护动作"), DispIndex, subItemIndex+1, ItemNum);
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

	rc = DB_GetREDispByInf(inf, &tReDisp);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
		
	m_Static[3].SetCaption(str); //  动作时间

	m_Static[2].SetCaption(TR_GetREName(&tReDisp,inf)); //  名称

	SetFDNum(pEvent->FD_Num);
	
	LONG 		RMIndex;
	DWORD 		dwValue;
	float		fValue;
	TRM_Disp 	tRMDisp;
	BYTE		*pData;
	
	char sztmp[32];
	char strFormat[32];

	//故障电量显示一次值还是二次值
	BOOL bIsShowFirst = TRUE;
	bIsShowFirst = DB_IsFDShowFirst();

	for(i= 0; i<pEvent->FD_Num; i++)
	{
		RMIndex = tReDisp.wRMList[i];
		rc = DB_GetRMDisp(RMIndex, &tRMDisp); //得到该类型遥测的描述参数

		pData = (BYTE *)(pEvent+1)+i*(sizeof(WORD)+sizeof(float))+2;
		dwValue = MAKEDWORD(pData[0],pData[1],pData[2],pData[3]);

		if(tRMDisp.wAttr == RM_ATTR_HEX)
		{
			sprintf(sztmp,"%s=%X", tRMDisp.strSymbol, dwValue);
		}
		else
		{		
			fValue = *((float *)&dwValue);
				
			if(DB_ERR_OK != rc)
			{
				return FALSE;
			}
			

			if (bIsShowFirst)
			{
				sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nFirDec, "f%s");
				sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strFirUnit);

			}
			else
			{
				sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nSecDec, "f%s");
				sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strSecUnit);
			}
		}
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
void CHmiActionDetailDlg::SetFDNum(LONG Num)
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

BOOL CHmiActionDetailDlg::Init()
{
	m_nCurPageIndex = 0;
	m_nCurTaborder = 1;

	SetClassName("CHmiActionDetailDlg");

	SetCaption(tr("动作报告： No.x"));
	SetEscWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(Function);
	SetLROffset(1);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(13,30,83,45,0);
	m_Static[0].SetTabOrder(-5);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetTimer(-1);
	m_Static[0].SetCaption(tr("动作时间"));
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
	m_Static[2].SetCaption(tr("xx动作 "));
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(13,50,224,72,0); //109,52,224,72,0
	m_Static[3].SetTabOrder(-2);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetTimer(-1);
	m_Static[3].SetCaption(tr("2008.01.02 13:13:23.123"));
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
	SetTimer(0);
	

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
BEGIN_HMIMESSAGE_MAP(CHmiActionDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CHmiActionDetailDlg)
	ON_HMIWND_LEFTRIGHT()
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//操作记录对话框
COperationDetailDlg::~COperationDetailDlg()
{
}
COperationDetailDlg::COperationDetailDlg()
{
}

void COperationDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	CHmiWnd::Show();
}

/****************************************************************************************
 * 功能描述: 读告警报告
 * 参数说明: 
 *		-LONG nItem 报告数据库标识序号， 读数据用
 *		-LONG DispIndex	显示序号，显示用
 * 返回代码: true 读成功， false 读失败
 * 其它: 
 *****************************************************************************************/
BOOL COperationDetailDlg::ReadReport(LONG nItem, LONG DispIndex)
{
	LONG rc;
	char str[32];
	TMiscEvent tEvent;
	TSysTime tSysTime;

		
	rc = DB_Misc_Read(nItem, &tEvent);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	
	AbsTimeTo(&(tEvent.absTime) ,&tSysTime);


	Init();
	
	m_bIsClosed = FALSE; //防止再运行一遍init
	//标题
	sprintf(str, "%s:%d", tr("操作事项"), DispIndex);
	SetCaption(str);
	
	//时间
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		tSysTime.Year, tSysTime.Month, tSysTime.Day, 
		tSysTime.Hour, tSysTime.Minute, tSysTime.Second, tSysTime.MSecond);
	m_Static[0].SetCaption(str);
	
	//内容, OperationStr 数组的上下文是CHmiDataList
	m_Static[1].SetCaption(CHmiDataList::tr(OperationStr[tEvent.nClass]));

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
	m_nCurPageIndex = 0;
	m_nCurTaborder = 1;
	
	SetClassName("COperationDetailDlg");
	SetCaption(tr("操作事项： No.x"));
	SetEscWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
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
	m_Static[1].SetCaption(tr("xx操作"));
	m_pObjList[2] = &m_Static[1];
	m_Static[2].SetDataSource(-1,0);
	m_Static[2].SetPosition(14,93,79,108);
	m_Static[2].SetTabOrder(0);
	m_Static[2].SetFont(16);
	m_Static[2].SetOwner(this);
	m_Static[2].SetCaption(tr("第几路"));
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(12,117,87,137);
	m_Static[3].SetTabOrder(0);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetCaption(tr("值变化"));
	m_pObjList[4] = &m_Static[3];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetObjNum(4);
	SetPageNum(1);
	m_pActiveObj = m_pObjList[1];
	return TRUE;
}
BEGIN_HMIMESSAGE_MAP(COperationDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(COperationDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//Soe报告详情对话框
CSOEDetailDlg::~CSOEDetailDlg()
{
}
CSOEDetailDlg::CSOEDetailDlg()
{
}

void CSOEDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	CHmiWnd::Show();
}
/****************************************************************************************
 * 功能描述: 读告警报告
 * 参数说明: 
 *		-LONG ItemIndex 报告数据库标识序号， 读数据用
 *		-LONG DispIndex	显示序号，显示用
 * 返回代码: true 读成功， false 读失败
 * 其它: 
 *****************************************************************************************/
BOOL CSOEDetailDlg::ReadReport(LONG ItemIndex, LONG DispIndex)
{
	LONG rc;
	char str[64];
	TEventBI tEvent;
	TSysTime tSysTime;

	Init();
	m_bIsClosed = FALSE; //防止再运行一遍init

	//标题
	sprintf(str, "%s:%02d", "SOE", DispIndex);
	
	SetCaption(str);
	rc = DB_ReadSOE(&tEvent, ItemIndex);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	

	AbsTimeTo(&(tEvent.stTime), &tSysTime);
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%03d", 
		tSysTime.Year, tSysTime.Month, tSysTime.Day, 
		tSysTime.Hour, tSysTime.Minute, tSysTime.Second, tSysTime.MSecond);

	m_Static[0].SetCaption(str);

	if(tEvent.bStatus)
	{
		sprintf(str, "%s:%d\t%s", tr("开入"),tEvent.wNo, tr("合-->分"));
		
	}
	else
	{
		sprintf(str, "%s:%d\t%s", tr("开入"),tEvent.wNo, tr("分-->合"));
	}
	
	m_Static[1].SetCaption(str);
	return TRUE;


}

BOOL CSOEDetailDlg::Init()
{
	
	m_nCurPageIndex = 0;
	m_nCurTaborder = 1;
	SetClassName("CSOEDetailDlg");
	SetCaption(tr("SOE报告：No.x"));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(20,46,220,69);
	m_Static[0].SetTabOrder(-1);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetCaption(tr("2008.02.03.02 22:20:23.234"));
	m_pObjList[1] = &m_Static[0];
	m_Static[1].SetDataSource(-1,0);
	m_Static[1].SetPosition(22,71,152,96);
	m_Static[1].SetTabOrder(-2);
	m_Static[1].SetFont(16);
	m_Static[1].SetOwner(this);
	m_Static[1].SetCaption(tr("开入1  0-->1"));
	m_pObjList[2] = &m_Static[1];
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetObjNum(2);
	//SetPageNum(1);
	m_pActiveObj = m_pObjList[1];
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CSOEDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CSOEDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

//告警报告详情---------------------------------
CHmiAlarmDetailDlg::~CHmiAlarmDetailDlg()
{
}
CHmiAlarmDetailDlg::CHmiAlarmDetailDlg()
{
}

/****************************************************************************************
 * 
 * 功能描述: 设置实际要显示的故障电量个数，  根据个数计算对话框的页数
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
void CHmiAlarmDetailDlg::SetFDNum(LONG Num)
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

void CHmiAlarmDetailDlg::DoModal(CHmiWnd* pWndReturn)
{
	SetEscWnd(pWndReturn);
	CHmiWnd::Show();
}

/****************************************************************************************
 * 功能描述: 读告警报告
 * 参数说明: 
 *		-LONG ItemIndex 报告数据库标识序号， 读数据用
 *		-LONG DispIndex	显示序号，显示用
 * 返回代码: true 读成功， false 读失败
 * 其它: 
 *****************************************************************************************/
BOOL CHmiAlarmDetailDlg::ReadReport(LONG ItemIndex, LONG DispIndex)
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
	
	m_bIsClosed = FALSE; //防止再运行一遍init
	//标题， 序号显示的是显示序号，不是内部编号
	sprintf(str, "%s:%d", tr("告警报告"), DispIndex);
	SetCaption(str);


	rc	= DB_GetRelayAlarmReportNum(&Num);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}

	rc = DB_ReadRelayAlarmReport(&tAlarmReport, ItemIndex);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	
	pEvent = &tAlarmReport.tEvent.tAction;

	inf = pEvent->nINF;
	abstime.Hi = pEvent->AbsTimeHi;
	abstime.Lo = pEvent->AbsTimeLo;
	AbsTimeTo(&abstime, &systime);
		
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d:%d", 
		systime.Year, systime.Month, systime.Day, 
		systime.Hour, systime.Minute, systime.Second, systime.MSecond);

	rc = DB_GetREDispByInf(inf, &tReDisp);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
		
	m_Static[3].SetCaption(str); //  动作时间

	m_Static[2].SetCaption(TR_GetREName(&tReDisp,inf)); //  名称

	SetFDNum(pEvent->FD_Num);
		

	LONG 		RMIndex;
	DWORD 		dwValue;
	float		fValue;
	TRM_Disp	tRMDisp;
	BYTE		*pData;
	
	char sztmp[32];
	char strFormat[32];

	//故障电量显示一次值还是二次值
	BOOL bIsShowFirst;
	//是否显示一次值
	bIsShowFirst = DB_IsFDShowFirst();
	
	for(i= 0; i<pEvent->FD_Num; i++)
	{
		RMIndex = tReDisp.wRMList[i];
		rc = DB_GetRMDisp(RMIndex, &tRMDisp); //得到该类型遥测的描述参数
		pData = (BYTE *)(pEvent+1)+i*(sizeof(WORD)+sizeof(float))+2;
		dwValue = MAKEDWORD(pData[0],pData[1],pData[2],pData[3]);	
		if(tRMDisp.wAttr == RM_ATTR_HEX)
		{
			//sprintf(strFormat, "%s%X", "%s=");
			sprintf(sztmp,"%s=%X", tRMDisp.strSymbol, dwValue);
		}
		else
		{
			fValue = *((float *)&dwValue);
				
			if(DB_ERR_OK != rc)
			{
				return FALSE;
			}

			if (bIsShowFirst)
			{			
				sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nFirDec, "f%s");
				sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strFirUnit);
				
			}
			else
			{
				sprintf(strFormat, "%s%d%s", "%s=%.", tRMDisp.nSecDec, "f%s");
				sprintf(sztmp,strFormat, tRMDisp.strSymbol, fValue, tRMDisp.strSecUnit);
			}
		}		
		
		m_Static[5+i].SetCaption(sztmp);
	}
	return TRUE;
}

BOOL CHmiAlarmDetailDlg::Init()
{
	m_nCurPageIndex = 0;
	m_nCurTaborder = 1;
	SetClassName("CHmiAlarmDetailDlg");

	SetCaption(tr("动作报告： No.x"));
	SetEscWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);

	m_Static[0].SetDataSource(-1,0);
	m_Static[0].SetPosition(13,30,83,45,0);
	m_Static[0].SetTabOrder(-5);
	m_Static[0].SetFont(16);
	m_Static[0].SetOwner(this);
	m_Static[0].SetTimer(-1);
	m_Static[0].SetCaption(tr("动作时间"));
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
	m_Static[2].SetCaption(tr("xx动作 "));
	m_pObjList[3] = &m_Static[2];
	m_Static[3].SetDataSource(-1,0);
	m_Static[3].SetPosition(13,50,224,72,0); //109,52,224,72,0
	m_Static[3].SetTabOrder(-2);
	m_Static[3].SetFont(16);
	m_Static[3].SetOwner(this);
	m_Static[3].SetTimer(-1);
	m_Static[3].SetCaption(tr("2008.01.02 13:13:23.123"));
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
	SetTimer(0);
	

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
BEGIN_HMIMESSAGE_MAP(CHmiAlarmDetailDlg,CHmiWnd)
//{{HMI_MSG_MAP(CHmiAlarmDetailDlg)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

//************窗口LcdContrastBox: 液晶对比度*******************
CHmiLcdContrastBox::~CHmiLcdContrastBox()
{
}
CHmiLcdContrastBox::CHmiLcdContrastBox()
{
}
BOOL CHmiLcdContrastBox::InitDlg()
{
	SetClassName("CHmiLcdContrastBox");
	SetCaption(tr("液晶对比度"));
	SetEscWnd(NULL);
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
/*	m_BtnOk.SetPosition(44,126,104,151,0);
	m_BtnOk.SetTabOrder(1);
	m_pObjList[1] = &m_BtnOk;
	m_BtnOk.SetFont(16);
	m_BtnOk.SetOwner(this);
	m_BtnOk.SetCaption(tr("应用"));
	m_BtnOk.SetHaveEnterFun(TRUE);
	m_BtnOk.SetEnterWnd(NULL);
	m_BtnCancel.SetPosition(144,126,204,151,0);
	m_BtnCancel.SetTabOrder(2);
	m_pObjList[2] = &m_BtnCancel;
	m_BtnCancel.SetFont(16);
	m_BtnCancel.SetOwner(this);
	m_BtnCancel.SetCaption(tr("取消"));
	m_BtnCancel.SetHaveEnterFun(TRUE);
	m_BtnCancel.SetEnterWnd(NULL);*/
//	m_Btn5333.SetPosition(18,82,223,102,0);
//	m_Btn5333.SetTabOrder(-3);
//	m_pObjList[3] = &m_Btn5333;
	
	m_nX0 = 18;
	m_nY0 = 82;
	m_nX1 = 220,
	m_nY1 = 102;
	m_StaticMsg.SetPosition(15,35,160,55,0);
	m_StaticMsg.SetTabOrder(-1);
	m_pObjList[1] = &m_StaticMsg;
	m_StaticMsg.SetFont(16);
	m_StaticMsg.SetOwner(this);
	m_StaticMsg.SetCaption(tr("按加减键调节对比度"));
	m_StaticMsg.SetDataSource(-1,0);
	m_StaticMsg.SetTimer(0);
	m_Static5334.SetPosition(19,63,36,79,0);
	m_Static5334.SetTabOrder(-2);
	m_pObjList[2] = &m_Static5334;
	m_Static5334.SetFont(16);
	m_Static5334.SetOwner(this);
	m_Static5334.SetCaption(("-"));
	m_Static5334.SetDataSource(-1,0);
	m_Static5334.SetTimer(0);
	m_Static5335.SetPosition(215,62,230,82,0);
	m_Static5335.SetTabOrder(-3);
	m_pObjList[3] = &m_Static5335;
	m_Static5335.SetFont(16);
	m_Static5335.SetOwner(this);
	m_Static5335.SetCaption(("+"));
	m_Static5335.SetDataSource(-1,0);
	m_Static5335.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(3);
	//SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CHmiLcdContrastBox,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndLcdContrastBox)
//ON_HMIWND_ENTER()

//	ON_HMIBTN_ENTER(1, OnEnterBtnOk)
//	ON_HMIBTN_ENTER(2, OnEnterBtnCancel)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

BOOL CHmiLcdContrastBox::OnAdd()
{
	m_nContrastValue = m_nContrastValue + m_nStep;
	if(m_nContrastValue > m_nX1)
		m_nContrastValue = m_nX1;
	
	//边框
	Data_DrawRect(m_nX0, m_nY0, m_nX1, m_nY1);
	//填充部分
	Data_FillSolidRect(m_nX0, m_nY0, m_nContrastValue, m_nY1);

	
	for(int i=0; i<HMI_LCDCONTRAST_STEP; i++)
	{
		LcdBackLightUp();
		g_wLCDContrast++;
	}
	UpdateWindow();
	return TRUE;
}
BOOL CHmiLcdContrastBox::OnSub()
{
	
	m_nContrastValue -=  m_nStep;
	if(m_nContrastValue <m_nX0)
		m_nContrastValue = m_nX0;
	//清原来的内容
	Data_FillSolidRect(m_nX0, m_nY0,  m_nX1, m_nY1, BKCOLORINDEX);
	
	//边框
	Data_DrawRect(m_nX0, m_nY0, m_nX1, m_nY1);
	//填充部分
	Data_FillSolidRect(m_nX0, m_nY0, m_nContrastValue, m_nY1);

	for(int i=0; i<HMI_LCDCONTRAST_STEP; i++)
	{
		LcdBackLightDown();
		g_wLCDContrast--;
	}
	UpdateWindow();
	return TRUE;
}

void CHmiLcdContrastBox::OnPaint()
{
	CHmiWnd::OnPaint();
	//边框
	Data_DrawRect(m_nX0, m_nY0, m_nX1, m_nY1);
	//填充部分
	Data_FillSolidRect(m_nX0, m_nY0, m_nContrastValue, m_nY1);
}
void CHmiLcdContrastBox::DoModal(CHmiWnd* pOkWnd, CHmiWnd* pCancelWnd)
{
	InitDlg();
	SetEnterWnd(pOkWnd);
	SetEscWnd(pCancelWnd);
	//去数据库读初值先
	m_nContrastValue = (m_nX1-m_nX0)*g_wLCDContrast/(HMI_LCDCONTRAST_HIGH-HMI_LCDCONTRAST_LOW)+ m_nX0;
	m_nStep = (m_nX1-m_nX0)*HMI_LCDCONTRAST_STEP/(HMI_LCDCONTRAST_HIGH-HMI_LCDCONTRAST_LOW);
	Show();
}
void CHmiLcdContrastBox::OnWndEnter()
{
	WORD wVal[2],wNotVal;
//	LONG rc = EEPROM_Read(EEPROM_SYSPARA_LCDCTLPAR, 4, wVal);
//	if(wVal[0] != g_wLCDContrast)
	{
		wNotVal = ~g_wLCDContrast;
		EEPROM_Write(EEPROM_SYSPARA_LCDCTLPAR, 2, &g_wLCDContrast);
		EEPROM_Write(EEPROM_SYSPARA_LCDCTLPAR+2, 2, &wNotVal);
		//ReportMsg("g_wLCDContrast = 0x%x.", g_wLCDContrast);
	}
	CHmiWnd::OnWndEnter();
	
}


////////////////CHmiLangDlg/////////////////
//************窗口Language: 系统语言*******************
CHmiLangDlg::~CHmiLangDlg()
{
}

CHmiLangDlg::CHmiLangDlg()
{
	SetClassName("CHmiLangDlg");
	SetEscWnd(NULL);
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_pObjList[0] = NULL;
	m_pObjList[1] = &m_BtnLang0;	
	m_pObjList[2] = &m_BtnLang1;	
	m_pObjList[3] = &m_BtnLang2;	
	m_pObjList[4] = &m_BtnLang3;	
	m_pObjList[5] = &m_BtnLang4;	
	m_pObjList[6] = &m_BtnLang5;	
	m_pObjList[7] = &m_BtnLang6;	
	m_pObjList[8] = &m_BtnLang7;	
	m_ppObjList = &(m_pObjList[1]);
	int i;

	for(i=0; i<4; i++)
	{
		m_ppObjList[2*i]->SetPosition(12,31+i*31,117,56+i*31,0);
		
		m_ppObjList[2*i+1]->SetPosition(125,31+i*31,230,56+i*31,0);
	}

	for(i=0; i<8; i++)
	{
		m_ppObjList[i]->SetTabOrder(i+1);
		m_ppObjList[i]->SetOwner(this);
		m_ppObjList[i]->SetFont(16);
		
		((CHmiButton*)m_ppObjList[i])->SetEnterWnd(NULL);
		((CHmiButton*)m_ppObjList[i])->SetHaveEnterFun(TRUE);
	}
	
	SetPageNum(1);
	SetObjNum(8);
	SetTimer(0);
}
BOOL CHmiLangDlg::InitDlg()
{
	SetCaption(tr("系统语言"));
	int nLangCount = g_LanFileMan.GetLanCount();
	
	int nCurLang = g_LanFileMan.GetCurLang();
	for(int i=0; i<nLangCount; i++)
	{
		((CHmiButton*)m_ppObjList[i])->SetHaveEnterFun(TRUE);
		m_ppObjList[i]->SetCaption(g_LanFileMan.GetLanCaption(i));
		
	}
	SetFocus(nCurLang+1); //focus 从1开始的，而语言nCurLang是从0开始的
	SetObjNum(nLangCount);
	return TRUE;
}

void CHmiLangDlg::DoModal(CHmiWnd* pCancelWnd)
{
	InitDlg();
	SetEscWnd(pCancelWnd);
	Show();
}

void CHmiLangDlg::OnEnterBtnLang0()
{
	g_LanFileMan.LoadLan(0);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang1()
{
	g_LanFileMan.LoadLan(1);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang2()
{
	g_LanFileMan.LoadLan(2);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang3()
{
	
	g_LanFileMan.LoadLan(3);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang4()
{
	g_LanFileMan.LoadLan(4);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang5()
{
	g_LanFileMan.LoadLan(5);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang6()
{
	g_LanFileMan.LoadLan(6);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}
void CHmiLangDlg::OnEnterBtnLang7()
{
	g_LanFileMan.LoadLan(7);
	
	SetCaption(tr("系统语言"));
	DrawCaption();
	
	UpdateWindow();
}

BEGIN_HMIMESSAGE_MAP(CHmiLangDlg,CHmiWnd)
//{{HMI_MSG_MAP(CHmiLangDlg)
	ON_HMIBTN_ENTER(1, OnEnterBtnLang0)
	ON_HMIBTN_ENTER(2, OnEnterBtnLang1)
	ON_HMIBTN_ENTER(3, OnEnterBtnLang2)
	ON_HMIBTN_ENTER(4, OnEnterBtnLang3)
	ON_HMIBTN_ENTER(5, OnEnterBtnLang4)
	ON_HMIBTN_ENTER(6, OnEnterBtnLang5)
	ON_HMIBTN_ENTER(7, OnEnterBtnLang6)
	ON_HMIBTN_ENTER(8, OnEnterBtnLang7)
	
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口WSerialSet: 串口设置*******************
CHmiDlgSerialSet::~CHmiDlgSerialSet()
{
}
CHmiDlgSerialSet::CHmiDlgSerialSet()
{
}
BOOL CHmiDlgSerialSet::Init()
{
	SetClassName("CHmiDlgSerialSet");
	SetCaption(tr("串口设置"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_BtnSave.SetPosition(40,128,100,153,0);
	m_BtnSave.SetTabOrder(5);
	m_pObjList[5] = &m_BtnSave;
	m_BtnSave.SetFont(16);
	m_BtnSave.SetOwner(this);
	m_BtnSave.SetCaption(tr("保存"));
	m_BtnSave.SetHaveEnterFun(TRUE);
	m_BtnSave.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_BtnESC.SetPosition(126,128,186,153,0);
	m_BtnESC.SetTabOrder(6);
	m_pObjList[6] = &m_BtnESC;
	m_BtnESC.SetFont(16);
	m_BtnESC.SetOwner(this);
	m_BtnESC.SetCaption(tr("取消"));
	m_BtnESC.SetHaveEnterFun(FALSE);
	m_BtnESC.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_StaticBaud.SetPosition(30,29,95,49,0);
	m_StaticBaud.SetTabOrder(-10);
	m_pObjList[10] = &m_StaticBaud;
	m_StaticBaud.SetFont(16);
	m_StaticBaud.SetOwner(this);
	m_StaticBaud.SetCaption(tr("波特率："));
	m_StaticBaud.SetDataSource(-1,0);
	m_StaticBaud.SetTimer(0);
	m_StaticData.SetPosition(30,50,95,70,0);
	m_StaticData.SetTabOrder(-7);
	m_pObjList[7] = &m_StaticData;
	m_StaticData.SetFont(16);
	m_StaticData.SetOwner(this);
	m_StaticData.SetCaption(tr("数据位："));
	m_StaticData.SetDataSource(-1,0);
	m_StaticData.SetTimer(0);
	m_StaticVerify.SetPosition(30,71,95,91,0);
	m_StaticVerify.SetTabOrder(-8);
	m_pObjList[8] = &m_StaticVerify;
	m_StaticVerify.SetFont(16);
	m_StaticVerify.SetOwner(this);
	m_StaticVerify.SetCaption(tr("校验位："));
	m_StaticVerify.SetDataSource(-1,0);
	m_StaticVerify.SetTimer(0);
	m_StaticStopByte.SetPosition(30,92,95,112,0);
	m_StaticStopByte.SetTabOrder(-9);
	m_pObjList[9] = &m_StaticStopByte;
	m_StaticStopByte.SetFont(16);
	m_StaticStopByte.SetOwner(this);
	m_StaticStopByte.SetCaption(tr("停止位："));
	m_StaticStopByte.SetDataSource(-1,0);
	m_StaticStopByte.SetTimer(0);
	m_ComboCBaud.SetPosition(107,29,187,49,0);
	m_ComboCBaud.SetTabOrder(1);
	m_pObjList[1] = &m_ComboCBaud;
	m_ComboCBaud.SetFont(16);
	m_ComboCBaud.SetOwner(this);
	m_ComboCBaud.SetAddSubFun(3);
	m_ComboCBaud.SetHaveEnterFun(FALSE);
	m_ComboCBaud.SetHaveAddSubFun(FALSE);
	m_ComboCData.SetPosition(107,50,187,70,0);
	m_ComboCData.SetTabOrder(2);
	m_pObjList[2] = &m_ComboCData;
	m_ComboCData.SetFont(16);
	m_ComboCData.SetOwner(this);
	m_ComboCData.SetAddSubFun(3);
	m_ComboCData.SetHaveEnterFun(FALSE);
	m_ComboCData.SetHaveAddSubFun(FALSE);
	m_ComboCVerify.SetPosition(107,71,187,91,0);
	m_ComboCVerify.SetTabOrder(3);
	m_pObjList[3] = &m_ComboCVerify;
	m_ComboCVerify.SetFont(16);
	m_ComboCVerify.SetOwner(this);
	m_ComboCVerify.SetAddSubFun(3);
	m_ComboCVerify.SetHaveEnterFun(FALSE);
	m_ComboCVerify.SetHaveAddSubFun(FALSE);
	m_ComboCStopByte.SetPosition(107,92,187,112,0);
	m_ComboCStopByte.SetTabOrder(4);
	m_pObjList[4] = &m_ComboCStopByte;
	m_ComboCStopByte.SetFont(16);
	m_ComboCStopByte.SetOwner(this);
	m_ComboCStopByte.SetAddSubFun(3);
	m_ComboCStopByte.SetHaveEnterFun(FALSE);
	m_ComboCStopByte.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(10);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CHmiDlgSerialSet,CHmiWnd)
//{{HMI_MSG_MAP(CHmiDlgSerialSet)
	ON_HMIBTN_ENTER(5, OnEnterBtnSave)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

void CHmiDlgSerialSet::OnEnterBtnSave()
{
	TSerialPara tSerial;
	WORD num;

	WORD baudno;
	WORD datano;
	WORD verifyno;
	WORD stopno;

	baudno = m_ComboCBaud.GetCurSel();
	datano = m_ComboCData.GetCurSel();
	verifyno = m_ComboCVerify.GetCurSel();
	stopno = m_ComboCStopByte.GetCurSel();
	
	num  = DB_GetSerialNum();
	if(num == 0)
	{
		return ;
	}
	DB_GetSerialPara(0,&tSerial);
	tSerial.byBaud = g_BaudInfo[baudno].lValue;
	tSerial.bFlag &= 0xFFE0;
	tSerial.bFlag |= g_DataInfo[datano].lValue;
	tSerial.bFlag |= (g_VerifyInfo[verifyno].lValue<<2);
	tSerial.bFlag |= (g_StopInfo[stopno].lValue<<4);

	DB_WriteSerialPara(0,&tSerial);
	DB_WriteSerialParaConfirm(0);
	

}
BOOL CHmiDlgSerialSet::OnWndInit()
{
	WORD BaudNum = 10;
	WORD DataNum = 4;
	WORD VerifyNum = 3;
	WORD StopNum = 2;
	TSerialPara tSerial;
	WORD num;
	num  = DB_GetSerialNum();
	if(num == 0)
	{
		return FALSE;
	}
	DB_GetSerialPara(0,&tSerial);
	
	SetFocus(m_ComboCBaud.GetTabOrder());
	m_ComboCBaud.ResetContent();
	int nItem;
	for (int i=0; i<BaudNum; i++)
	{
		nItem = m_ComboCBaud.AddString(g_BaudInfo[i].strName);
		m_ComboCBaud.SetItemData(nItem, g_BaudInfo[i].lValue);
	}
	m_ComboCBaud.SetCurSel(tSerial.byBaud);
	
	m_ComboCData.ResetContent();
	for (int i=0; i<DataNum; i++)
	{
		nItem = m_ComboCData.AddString(g_DataInfo[i].strName);
		m_ComboCData.SetItemData(nItem, g_DataInfo[i].lValue);
	}
	m_ComboCData.SetCurSel(tSerial.bFlag&0x03);

	m_ComboCVerify.ResetContent();
	for (int i=0; i<VerifyNum; i++)
	{
		nItem = m_ComboCVerify.AddString(g_VerifyInfo[i].strName);
		m_ComboCVerify.SetItemData(nItem, g_VerifyInfo[i].lValue);
	}
	m_ComboCVerify.SetCurSel((tSerial.bFlag>>2)&0x03);

	m_ComboCStopByte.ResetContent();
	for (int i=0; i<StopNum; i++)
	{
		nItem = m_ComboCStopByte.AddString(g_StopInfo[i].strName);
		m_ComboCStopByte.SetItemData(nItem, g_StopInfo[i].lValue);
	}
	m_ComboCStopByte.SetCurSel((tSerial.bFlag>>4)&0x01);

	return TRUE;
	
}
void CHmiDlgSerialSet::DoModal(CHmiWnd* pCancelWnd)
{
//	InitDlg();
//	SetEscWnd(pCancelWnd);
	Show();
}

//************窗口SysStatus: 装置状态*******************
CHmiDlgSysStatus::~CHmiDlgSysStatus()
{
}
CHmiDlgSysStatus::CHmiDlgSysStatus()
{
}
BOOL CHmiDlgSysStatus::Init()
{
	SetClassName("CHmiDlgSysStatus");
	SetCaption(tr("装置状态"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_Staticstatic1.SetPosition(21,31,91,56,0);
	m_Staticstatic1.SetTabOrder(-3);
	m_pObjList[3] = &m_Staticstatic1;
	m_Staticstatic1.SetFont(16);
	m_Staticstatic1.SetOwner(this);
	m_Staticstatic1.SetCaption(tr("箱内温度"));
	m_Staticstatic1.SetDataSource(-1,0);
	m_Staticstatic1.SetTimer(0);
	m_Staticstatic2.SetPosition(21,55,91,80,0);
	m_Staticstatic2.SetTabOrder(-4);
	m_pObjList[4] = &m_Staticstatic2;
	m_Staticstatic2.SetFont(16);
	m_Staticstatic2.SetOwner(this);
	m_Staticstatic2.SetCaption(tr("开出失效"));
	m_Staticstatic2.SetDataSource(-1,0);
	m_Staticstatic2.SetTimer(0);
	m_Staticstatic3.SetPosition(21,79,91,104,0);
	m_Staticstatic3.SetTabOrder(-8);
	m_pObjList[8] = &m_Staticstatic3;
	m_Staticstatic3.SetFont(16);
	m_Staticstatic3.SetOwner(this);
	m_Staticstatic3.SetCaption(tr("开出击穿"));
	m_Staticstatic3.SetDataSource(-1,0);
	m_Staticstatic3.SetTimer(0);
	m_Statictempreture.SetPosition(120,31,215,56,0);
	m_Statictempreture.SetTabOrder(-5);
	m_pObjList[5] = &m_Statictempreture;
	m_Statictempreture.SetFont(16);
	m_Statictempreture.SetOwner(this);
	m_Statictempreture.SetCaption(tr("34"));
	m_Statictempreture.SetDataSource(-1,0);
	m_Statictempreture.SetTimer(0);
	m_Staticstatic4.SetPosition(21,103,91,128,0);
	m_Staticstatic4.SetTabOrder(-1);
	m_pObjList[1] = &m_Staticstatic4;
	m_Staticstatic4.SetFont(16);
	m_Staticstatic4.SetOwner(this);
	m_Staticstatic4.SetCaption(tr("B 码时钟"));
	m_Staticstatic4.SetDataSource(-1,0);
	m_Staticstatic4.SetTimer(0);
	m_Staticbtime.SetPosition(120,104,215,129,0);
	m_Staticbtime.SetTabOrder(-2);
	m_pObjList[2] = &m_Staticbtime;
	m_Staticbtime.SetFont(16);
	m_Staticbtime.SetOwner(this);
	m_Staticbtime.SetCaption(tr("  "));
	m_Staticbtime.SetDataSource(-1,0);
	m_Staticbtime.SetTimer(0);
	m_Staticbreakfail.SetPosition(120,55,215,80,0);
	m_Staticbreakfail.SetTabOrder(-6);
	m_pObjList[6] = &m_Staticbreakfail;
	m_Staticbreakfail.SetFont(16);
	m_Staticbreakfail.SetOwner(this);
	m_Staticbreakfail.SetCaption(tr("ffffffff"));
	m_Staticbreakfail.SetDataSource(-1,0);
	m_Staticbreakfail.SetTimer(0);
	m_Staticbreakdown.SetPosition(120,79,215,104,0);
	m_Staticbreakdown.SetTabOrder(-7);
	m_pObjList[7] = &m_Staticbreakdown;
	m_Staticbreakdown.SetFont(16);
	m_Staticbreakdown.SetOwner(this);
	m_Staticbreakdown.SetCaption(tr("ffffffff"));
	m_Staticbreakdown.SetDataSource(-1,0);
	m_Staticbreakdown.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(8);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CHmiDlgSysStatus,CHmiWnd)
//{{HMI_MSG_MAP(CHmiDlgSysStatus)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

BOOL CHmiDlgSysStatus::OnWndInit()
{
	
	DWORD word1,word2,word3;
	char format[16];
	char str[32];
	sprintf(format, "%s%df", "%03.", 1);	
	sprintf(str,format,(float)SysTempture/10);
	sprintf(str,"%s %s",str,"℃");
	m_Statictempreture.SetCaption(str);

	word1 = LOWORD(dwBoInvalidStatus[0]);
	word2 = MAKEWORD(LOBYTE(HIWORD(dwBoInvalidStatus[0])), LOBYTE(LOWORD(dwBoInvalidStatus[1])));
	word3 = MAKEWORD(HIBYTE(LOWORD(dwBoInvalidStatus[1])), LOBYTE(HIWORD(dwBoInvalidStatus[1])));
	sprintf(str,"%04X %04X %04X",word3,word2,word1);
	m_Staticbreakfail.SetCaption(str);
	
	word1 = LOWORD(dwBoBreakStatus[0]);
	word2 = MAKEWORD(LOBYTE(HIWORD(dwBoBreakStatus[0])), LOBYTE(LOWORD(dwBoBreakStatus[1])));
	word3 = MAKEWORD(HIBYTE(LOWORD(dwBoBreakStatus[1])), LOBYTE(HIWORD(dwBoBreakStatus[1])));
	sprintf(str,"%04X %04X %04X",word3,word2,word1);
	m_Staticbreakdown.SetCaption(str);


	if(DAE_GetRunningFlags(SS_TIME_B))
		m_Staticbtime.SetCaption(tr("无效"));
	else
		m_Staticbtime.SetCaption(tr("有效"));

	
	return TRUE;
}
void CHmiDlgSysStatus::OnTimer(LONG TimerID)
{
	OnWndInit();
	OnPaint();
	UpdateWindow();
}
void CHmiDlgSysStatus::DoModal(CHmiWnd* pCancelWnd)
{
	Show();
}
//************窗口WComStatus: 通讯状态*******************
CHmiDlgWComStatus::~CHmiDlgWComStatus()
{
}
CHmiDlgWComStatus::CHmiDlgWComStatus()
{
}
/*
BOOL CHmiDlgWComStatus::Init()
{
	SetClassName("CHmiDlgWComStatus");
	SetCaption(tr("通讯状态"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_Staticstatic2.SetPosition(24,63,94,78,0);
	m_Staticstatic2.SetTabOrder(-2);
	m_pObjList[2] = &m_Staticstatic2;
	m_Staticstatic2.SetFont(16);
	m_Staticstatic2.SetOwner(this);
	m_Staticstatic2.SetCaption(tr("ETHB: Err"));
	m_Staticstatic2.SetDataSource(-1,0);
	m_Staticstatic2.SetTimer(0);
	m_Staticstatic1.SetPosition(24,39,94,54,0);
	m_Staticstatic1.SetTabOrder(-1);
	m_pObjList[1] = &m_Staticstatic1;
	m_Staticstatic1.SetFont(16);
	m_Staticstatic1.SetOwner(this);
	m_Staticstatic1.SetCaption(tr("ETHA: OK"));
	m_Staticstatic1.SetDataSource(-1,0);
	m_Staticstatic1.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(2);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}*/
BOOL CHmiDlgWComStatus::OnWndInit()
{

	SetClassName("CHmiDlgWComStatus");
	SetCaption(tr("端口状态"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndWStatus));
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);

	WORD num,sernum,cannum,ethnum;
	char format[16];
	char str[32];
	char str1[32];
	char str2[32];
	char str3[32];
	char str4[32];
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();
	num  = sernum+cannum+ethnum;

	WORD page;

	if(num%7==0)
		page = num/7;
	else
		page = num/7+1;

	
	SetPageNum(page);

	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	for(LONG j=0;j<page;j++)
	{
		for(LONG i=0;i<7;i++)
		{
			m_Static[i+j*7].SetPosition(14,21+18*i,224,21+19*i+16,j); 	
			m_Static[i+j*7].SetFont(16);
			m_Static[i+j*7].SetOwner(this);
			m_Static[i+j*7].SetDataSource(-1,0);
			m_Static[i+j*7].SetTimer(0);
			m_pObjList[i+j*7+1]=&(m_Static[i+j*7]); 

		}
	}

	SetObjNum(num);

	WORD no = 0;
	BYTE commuStatus;
	if(sernum)
	{
		TSerialPara tSerial;
		for(LONG j=0;j<sernum;j++)
		{
			DB_GetSerialPara(j,&tSerial);
			if(DAE_GetCommStatus(tSerial.wPortID,commuStatus)!=DB_ERR_OK)
				sprintf(str3,"%s",tr("通讯异常"));
			else
			{
				if(commuStatus == COMM_STATUS_OK)
					sprintf(str3,"%s",tr("通讯正常"));
				else
					sprintf(str3,"%s",tr("通讯异常"));
			}
			if(tSerial.byDriver>8||tSerial.byDriver<0)	
				sprintf(str,"%s%d%s%s %s","SER",tSerial.wPortID&0xFF,": ","NULL",str3);
			else
				sprintf(str,"%s%d%s%s %s","SER",tSerial.wPortID&0xFF,": ",g_ProtocolInfo[tSerial.byDriver].strName,str3);
			m_Static[no].SetCaption(str);
			no++;
		}
	}
	if(cannum)
	{
		TCanBusPara tCan;
		for(LONG m=0;m<cannum;m++)
		{
			DB_GetCANPara(m,&tCan);
			
			if(DAE_GetCommStatus(tCan.wPortID,commuStatus)!=DB_ERR_OK)
				sprintf(str3,"%s",tr("通讯异常"));
			else
			{
				if(commuStatus == COMM_STATUS_OK)
					sprintf(str3,"%s",tr("通讯正常"));
				else
					sprintf(str3,"%s",tr("通讯异常"));
			}
			if(tCan.byDriver>8||tCan.byDriver<0)
				sprintf(str,"%s%d%s%s %s","CAN",tCan.wPortID&0xFF,": ","NULL",str3);
			else
				sprintf(str,"%s%d%s%s %s","CAN",tCan.wPortID&0xFF,": ",g_ProtocolInfo[tCan.byDriver].strName,str3);
			m_Static[no].SetCaption(str);
			no++;
		}
	}
	if(ethnum)
	{
		TEthPortPara tEth;
		for(LONG n=0;n<ethnum;n++)
		{
			DB_GetETHPara(n,&tEth);
			if((tEth.wPortID&0xF00)==0x300)
				sprintf(str4,"%s","ETHA: ");
			if((tEth.wPortID&0xF00)==0x400)
				sprintf(str4,"%s","ETHB: ");
			
			if(DAE_GetCommStatus(tEth.wPortID,commuStatus)!=DB_ERR_OK)				
				sprintf(str3,"%s",tr("通讯异常"));
			else
			{
				if(commuStatus == COMM_STATUS_OK)
					sprintf(str3,"%s",tr("通讯正常"));
				else
					sprintf(str3,"%s",tr("通讯异常"));
			}
			if(tEth.wDriver>8||tEth.wDriver<0)
				sprintf(str,"%s%d%s%s %s",str4,tEth.dwSelfPort,",","NULL",str3);
			else
				sprintf(str,"%s%d%s%s %s",str4,tEth.dwSelfPort,",",g_ProtocolInfo[tEth.wDriver].strName,str3);
			m_Static[no].SetCaption(str);
			no++;
		}
	}

	return TRUE;
	
}
void CHmiDlgWComStatus::OnTimer(LONG TimerID)
{
	OnWndInit();
	OnPaint();
	UpdateWindow();
}
void CHmiDlgWComStatus::DoModal(CHmiWnd* pCancelWnd)
{
	Show();
}

BEGIN_HMIMESSAGE_MAP(CHmiDlgWComStatus,CHmiWnd)
//{{HMI_MSG_MAP(CHmiDlgWComStatus)
///}}HMI_MSG
END_HMIMESSAGE_MAP()



//************窗口AddressSetting: 以太网A*******************
CHmiDlgAddressSetting::~CHmiDlgAddressSetting()
{
}
CHmiDlgAddressSetting::CHmiDlgAddressSetting()
{
}
BOOL CHmiDlgAddressSetting::Init()
{
	SetClassName("CHmiDlgAddressSetting");
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndEthadd));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_Btnenter.SetPosition(37,131,97,156,0);
	m_Btnenter.SetTabOrder(13);
	m_pObjList[13] = &m_Btnenter;
	m_Btnenter.SetFont(16);
	m_Btnenter.SetOwner(this);
	m_Btnenter.SetCaption(tr("确认"));
	m_Btnenter.SetHaveEnterFun(TRUE);
	m_Btnenter.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_Btnesc.SetPosition(125,131,185,156,0);
	m_Btnesc.SetTabOrder(14);
	m_pObjList[14] = &m_Btnesc;
	m_Btnesc.SetFont(16);
	m_Btnesc.SetOwner(this);
	m_Btnesc.SetCaption(tr("取消"));
	m_Btnesc.SetHaveEnterFun(FALSE);
	m_Btnesc.SetEnterWnd((CHmiWnd*)&(CLcdApp::m_WndAddress));
	m_EditIp1.SetPosition(61,34,96,54,0);
	m_EditIp1.SetTabOrder(1);
	m_pObjList[1] = &m_EditIp1;
	m_EditIp1.SetFont(16);
	m_EditIp1.SetOwner(this);
	m_EditIp1.SetCaption(tr("192"));
	m_EditIp1.SetMaxLen(3);
	m_EditIp1.SetDecimalNum(0);
	m_EditIp1.SetValueRange(255,0);
	m_EditIp1.SetIsShowNegative(FALSE);
	m_EditIp1.SetUDFun(-1);
	m_EditIp1.SetLRFun(0);
	m_EditIp1.SetAddSubFun(0);
	m_EditIp1.SetHaveUpDownFun(FALSE);
	m_EditIp1.SetHaveLeftRightFun(FALSE);
	m_EditIp1.SetHaveEnterFun(FALSE);
	m_EditIp1.SetHaveAddSubFun(FALSE);
	m_EditIp2.SetPosition(100,34,135,54,0);
	m_EditIp2.SetTabOrder(2);
	m_pObjList[2] = &m_EditIp2;
	m_EditIp2.SetFont(16);
	m_EditIp2.SetOwner(this);
	m_EditIp2.SetCaption(tr("168"));
	m_EditIp2.SetMaxLen(3);
	m_EditIp2.SetDecimalNum(0);
	m_EditIp2.SetValueRange(255,0);
	m_EditIp2.SetIsShowNegative(FALSE);
	m_EditIp2.SetUDFun(-1);
	m_EditIp2.SetLRFun(0);
	m_EditIp2.SetAddSubFun(0);
	m_EditIp2.SetHaveUpDownFun(FALSE);
	m_EditIp2.SetHaveLeftRightFun(FALSE);
	m_EditIp2.SetHaveEnterFun(FALSE);
	m_EditIp2.SetHaveAddSubFun(FALSE);
	m_EditIp3.SetPosition(141,34,176,54,0);
	m_EditIp3.SetTabOrder(3);
	m_pObjList[3] = &m_EditIp3;
	m_EditIp3.SetFont(16);
	m_EditIp3.SetOwner(this);
	m_EditIp3.SetCaption(tr("100"));
	m_EditIp3.SetMaxLen(3);
	m_EditIp3.SetDecimalNum(0);
	m_EditIp3.SetValueRange(255,0);
	m_EditIp3.SetIsShowNegative(FALSE);
	m_EditIp3.SetUDFun(-1);
	m_EditIp3.SetLRFun(0);
	m_EditIp3.SetAddSubFun(0);
	m_EditIp3.SetHaveUpDownFun(FALSE);
	m_EditIp3.SetHaveLeftRightFun(FALSE);
	m_EditIp3.SetHaveEnterFun(FALSE);
	m_EditIp3.SetHaveAddSubFun(FALSE);
	m_EditIp4.SetPosition(182,34,217,54,0);
	m_EditIp4.SetTabOrder(4);
	m_pObjList[4] = &m_EditIp4;
	m_EditIp4.SetFont(16);
	m_EditIp4.SetOwner(this);
	m_EditIp4.SetCaption(tr("111"));
	m_EditIp4.SetMaxLen(3);
	m_EditIp4.SetDecimalNum(0);
	m_EditIp4.SetValueRange(255,0);
	m_EditIp4.SetIsShowNegative(FALSE);
	m_EditIp4.SetUDFun(-1);
	m_EditIp4.SetLRFun(0);
	m_EditIp4.SetAddSubFun(0);
	m_EditIp4.SetHaveUpDownFun(FALSE);
	m_EditIp4.SetHaveLeftRightFun(FALSE);
	m_EditIp4.SetHaveEnterFun(FALSE);
	m_EditIp4.SetHaveAddSubFun(FALSE);
	m_EditIp5.SetPosition(61,65,96,85,0);
	m_EditIp5.SetTabOrder(5);
	m_pObjList[5] = &m_EditIp5;
	m_EditIp5.SetFont(16);
	m_EditIp5.SetOwner(this);
	m_EditIp5.SetCaption(tr("255"));
	m_EditIp5.SetMaxLen(3);
	m_EditIp5.SetDecimalNum(0);
	m_EditIp5.SetValueRange(255,0);
	m_EditIp5.SetIsShowNegative(FALSE);
	m_EditIp5.SetUDFun(-1);
	m_EditIp5.SetLRFun(0);
	m_EditIp5.SetAddSubFun(3);
	m_EditIp5.SetHaveUpDownFun(FALSE);
	m_EditIp5.SetHaveLeftRightFun(FALSE);
	m_EditIp5.SetHaveEnterFun(FALSE);
	m_EditIp5.SetHaveAddSubFun(FALSE);
	m_EditIp9.SetPosition(61,95,96,115,0);
	m_EditIp9.SetTabOrder(9);
	m_pObjList[9] = &m_EditIp9;
	m_EditIp9.SetFont(16);
	m_EditIp9.SetOwner(this);
	m_EditIp9.SetCaption(tr("000"));
	m_EditIp9.SetMaxLen(3);
	m_EditIp9.SetDecimalNum(0);
	m_EditIp9.SetValueRange(255,0);
	m_EditIp9.SetIsShowNegative(FALSE);
	m_EditIp9.SetUDFun(-1);
	m_EditIp9.SetLRFun(0);
	m_EditIp9.SetAddSubFun(3);
	m_EditIp9.SetHaveUpDownFun(FALSE);
	m_EditIp9.SetHaveLeftRightFun(FALSE);
	m_EditIp9.SetHaveEnterFun(FALSE);
	m_EditIp9.SetHaveAddSubFun(FALSE);
	m_EditIp10.SetPosition(100,95,135,115,0);
	m_EditIp10.SetTabOrder(10);
	m_pObjList[10] = &m_EditIp10;
	m_EditIp10.SetFont(16);
	m_EditIp10.SetOwner(this);
	m_EditIp10.SetCaption(tr("000"));
	m_EditIp10.SetMaxLen(3);
	m_EditIp10.SetDecimalNum(0);
	m_EditIp10.SetValueRange(255,0);
	m_EditIp10.SetIsShowNegative(FALSE);
	m_EditIp10.SetUDFun(-1);
	m_EditIp10.SetLRFun(0);
	m_EditIp10.SetAddSubFun(3);
	m_EditIp10.SetHaveUpDownFun(FALSE);
	m_EditIp10.SetHaveLeftRightFun(FALSE);
	m_EditIp10.SetHaveEnterFun(FALSE);
	m_EditIp10.SetHaveAddSubFun(FALSE);
	m_EditIp6.SetPosition(100,65,135,85,0);
	m_EditIp6.SetTabOrder(6);
	m_pObjList[6] = &m_EditIp6;
	m_EditIp6.SetFont(16);
	m_EditIp6.SetOwner(this);
	m_EditIp6.SetCaption(tr("255"));
	m_EditIp6.SetMaxLen(3);
	m_EditIp6.SetDecimalNum(0);
	m_EditIp6.SetValueRange(255,0);
	m_EditIp6.SetIsShowNegative(FALSE);
	m_EditIp6.SetUDFun(-1);
	m_EditIp6.SetLRFun(0);
	m_EditIp6.SetAddSubFun(3);
	m_EditIp6.SetHaveUpDownFun(FALSE);
	m_EditIp6.SetHaveLeftRightFun(FALSE);
	m_EditIp6.SetHaveEnterFun(FALSE);
	m_EditIp6.SetHaveAddSubFun(FALSE);
	m_EditIp7.SetPosition(141,65,176,85,0);
	m_EditIp7.SetTabOrder(7);
	m_pObjList[7] = &m_EditIp7;
	m_EditIp7.SetFont(16);
	m_EditIp7.SetOwner(this);
	m_EditIp7.SetCaption(tr("255"));
	m_EditIp7.SetMaxLen(3);
	m_EditIp7.SetDecimalNum(0);
	m_EditIp7.SetValueRange(255,0);
	m_EditIp7.SetIsShowNegative(FALSE);
	m_EditIp7.SetUDFun(-1);
	m_EditIp7.SetLRFun(0);
	m_EditIp7.SetAddSubFun(3);
	m_EditIp7.SetHaveUpDownFun(FALSE);
	m_EditIp7.SetHaveLeftRightFun(FALSE);
	m_EditIp7.SetHaveEnterFun(FALSE);
	m_EditIp7.SetHaveAddSubFun(FALSE);
	m_EditIp8.SetPosition(182,65,217,85,0);
	m_EditIp8.SetTabOrder(8);
	m_pObjList[8] = &m_EditIp8;
	m_EditIp8.SetFont(16);
	m_EditIp8.SetOwner(this);
	m_EditIp8.SetCaption(tr("000"));
	m_EditIp8.SetMaxLen(3);
	m_EditIp8.SetDecimalNum(0);
	m_EditIp8.SetValueRange(255,0);
	m_EditIp8.SetIsShowNegative(FALSE);
	m_EditIp8.SetUDFun(-1);
	m_EditIp8.SetLRFun(0);
	m_EditIp8.SetAddSubFun(3);
	m_EditIp8.SetHaveUpDownFun(FALSE);
	m_EditIp8.SetHaveLeftRightFun(FALSE);
	m_EditIp8.SetHaveEnterFun(FALSE);
	m_EditIp8.SetHaveAddSubFun(FALSE);
	m_EditIp11.SetPosition(141,95,176,115,0);
	m_EditIp11.SetTabOrder(11);
	m_pObjList[11] = &m_EditIp11;
	m_EditIp11.SetFont(16);
	m_EditIp11.SetOwner(this);
	m_EditIp11.SetCaption(tr("000"));
	m_EditIp11.SetMaxLen(3);
	m_EditIp11.SetDecimalNum(0);
	m_EditIp11.SetValueRange(255,0);
	m_EditIp11.SetIsShowNegative(FALSE);
	m_EditIp11.SetUDFun(-1);
	m_EditIp11.SetLRFun(0);
	m_EditIp11.SetAddSubFun(3);
	m_EditIp11.SetHaveUpDownFun(FALSE);
	m_EditIp11.SetHaveLeftRightFun(FALSE);
	m_EditIp11.SetHaveEnterFun(FALSE);
	m_EditIp11.SetHaveAddSubFun(FALSE);
	m_EditIp12.SetPosition(182,95,217,115,0);
	m_EditIp12.SetTabOrder(12);
	m_pObjList[12] = &m_EditIp12;
	m_EditIp12.SetFont(16);
	m_EditIp12.SetOwner(this);
	m_EditIp12.SetCaption(tr("000"));
	m_EditIp12.SetMaxLen(3);
	m_EditIp12.SetDecimalNum(0);
	m_EditIp12.SetValueRange(255,0);
	m_EditIp12.SetIsShowNegative(FALSE);
	m_EditIp12.SetUDFun(-1);
	m_EditIp12.SetLRFun(0);
	m_EditIp12.SetAddSubFun(3);
	m_EditIp12.SetHaveUpDownFun(FALSE);
	m_EditIp12.SetHaveLeftRightFun(FALSE);
	m_EditIp12.SetHaveEnterFun(FALSE);
	m_EditIp12.SetHaveAddSubFun(FALSE);
	m_Staticgate.SetPosition(10,95,50,110,0);
	m_Staticgate.SetTabOrder(-15);
	m_pObjList[15] = &m_Staticgate;
	m_Staticgate.SetFont(16);
	m_Staticgate.SetOwner(this);
	m_Staticgate.SetCaption(tr("网关:"));
	m_Staticgate.SetDataSource(-1,0);
	m_Staticgate.SetTimer(0);
	m_Staticadd.SetPosition(10,34,50,49,0);
	m_Staticadd.SetTabOrder(-17);
	m_pObjList[17] = &m_Staticadd;
	m_Staticadd.SetFont(16);
	m_Staticadd.SetOwner(this);
	m_Staticadd.SetCaption(tr("地址:"));
	m_Staticadd.SetDataSource(-1,0);
	m_Staticadd.SetTimer(-1);
	m_Staticmask.SetPosition(10,65,50,80,0);
	m_Staticmask.SetTabOrder(-16);
	m_pObjList[16] = &m_Staticmask;
	m_Staticmask.SetFont(16);
	m_Staticmask.SetOwner(this);
	m_Staticmask.SetCaption(tr("掩码："));
	m_Staticmask.SetDataSource(-1,0);
	m_Staticmask.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(17);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CHmiDlgAddressSetting,CHmiWnd)
//{{HMI_MSG_MAP(CHmiDlgAddressSetting)
	ON_HMIBTN_ENTER(13, OnEnterBtnenter)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


BOOL CHmiDlgAddressSetting::OnWndInit()
{
	DWORD dwIP;
	DWORD dwMASK;
	DWORD dwGate;

	if(g_wEth == 0x0a)
	{
		SetCaption(tr("以太网A"));
		DB_GetEthAPara(dwIP,dwMASK,dwGate);
	}
	else if(g_wEth == 0x0b)
	{
		SetCaption(tr("以太网B"));
		DB_GetEthBPara(dwIP,dwMASK,dwGate);
	}
	else
	{
		HMIMsgBox(tr("以太网地址"),tr("读取错误!"), this);
		return FALSE;
	}
	m_EditIp1.SetValue(HIHIBYTE(dwIP));
	m_EditIp2.SetValue(HILOBYTE(dwIP));
	m_EditIp3.SetValue(LOHIBYTE(dwIP));
	m_EditIp4.SetValue(LOLOBYTE(dwIP));
	m_EditIp5.SetValue(HIHIBYTE(dwMASK));
	m_EditIp6.SetValue(HILOBYTE(dwMASK));
	m_EditIp7.SetValue(LOHIBYTE(dwMASK));
	m_EditIp8.SetValue(LOLOBYTE(dwMASK));	
	m_EditIp9.SetValue(HIHIBYTE(dwGate));
	m_EditIp10.SetValue(HILOBYTE(dwGate));
	m_EditIp11.SetValue(LOHIBYTE(dwGate));
	m_EditIp12.SetValue(LOLOBYTE(dwGate));
	
	m_EditIp1.UpdateData(FALSE);
	m_EditIp2.UpdateData(FALSE);
	m_EditIp3.UpdateData(FALSE);
	m_EditIp4.UpdateData(FALSE);
	m_EditIp5.UpdateData(FALSE);
	m_EditIp6.UpdateData(FALSE);
	m_EditIp7.UpdateData(FALSE);
	m_EditIp8.UpdateData(FALSE);	
	m_EditIp9.UpdateData(FALSE);
	m_EditIp10.UpdateData(FALSE);
	m_EditIp11.UpdateData(FALSE);
	m_EditIp12.UpdateData(FALSE);
	
	return	TRUE;
}



void CHmiDlgAddressSetting::OnEnterBtnenter()
{
	STATUS rc;
	m_EditIp1.UpdateData(TRUE);
	m_EditIp2.UpdateData(TRUE);
	m_EditIp3.UpdateData(TRUE);
	m_EditIp4.UpdateData(TRUE);
	m_EditIp5.UpdateData(TRUE);
	m_EditIp6.UpdateData(TRUE);
	m_EditIp7.UpdateData(TRUE);
	m_EditIp8.UpdateData(TRUE);	
	m_EditIp9.UpdateData(TRUE);
	m_EditIp10.UpdateData(TRUE);
	m_EditIp11.UpdateData(TRUE);
	m_EditIp12.UpdateData(TRUE);

	BYTE IP1,IP2,IP3,IP4,IP5,IP6,IP7,IP8,IP9,IP10,IP11,IP12;
	DWORD IP,Mask,Gate;
	IP1  = m_EditIp1.GetValue();
	IP2  = m_EditIp2.GetValue();
	IP3  = m_EditIp3.GetValue();
	IP4  = m_EditIp4.GetValue();
	IP5  = m_EditIp5.GetValue();
	IP6  = m_EditIp6.GetValue();
	IP7  = m_EditIp7.GetValue();
	IP8  = m_EditIp8.GetValue();
	IP9  = m_EditIp9.GetValue();
	IP10 = m_EditIp10.GetValue();
	IP11 = m_EditIp11.GetValue();
	IP12 = m_EditIp12.GetValue();
	
	IP	= MAKEDWORD(IP4,IP3,IP2,IP1);
	Mask = MAKEDWORD(IP8,IP7,IP6,IP5);	
	Gate = MAKEDWORD(IP12,IP11,IP10,IP9);

	if(g_wEth == 0x0a)
	{
		rc = DB_ModifyEthAPara(IP,Mask,Gate);
	}
	else if(g_wEth == 0x0b)
	{
		rc = DB_ModifyEthBPara(IP,Mask,Gate);
	}
	else

		rc = DB_ERR_FAILURE;
	
	if(DB_ERR_OK != rc)
	{
		m_Btnenter.DisableAutoEnterWnd();
		HMIMsgBox(tr("以太网地址"),tr("修改以太网地址失败!"), this);
		return;
	}
}
void CHmiDlgAddressSetting::DoModal(CHmiWnd* pCancelWnd)
{
	Show();
}
//************窗口DefaultPara: 恢复出厂设置*******************
CLcdWndDefaultPara::~CLcdWndDefaultPara()
{
}
CLcdWndDefaultPara::CLcdWndDefaultPara()
{
}
BOOL CLcdWndDefaultPara::Init()
{
	SetClassName("CLcdWndDefaultPara");
	SetCaption(tr("恢复出厂设置"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditDefaultParaPassword.SetPosition(100,132,145,152,0);
	m_EditDefaultParaPassword.SetTabOrder(1);
	m_pObjList[1] = &m_EditDefaultParaPassword;
	m_EditDefaultParaPassword.SetFont(16);
	m_EditDefaultParaPassword.SetOwner(this);
	m_EditDefaultParaPassword.SetCaption(tr("0000"));
	m_EditDefaultParaPassword.SetMaxLen(4);
	m_EditDefaultParaPassword.SetDecimalNum(0);
	m_EditDefaultParaPassword.SetValueRange(9999,0);
	m_EditDefaultParaPassword.SetIsShowNegative(FALSE);
	m_EditDefaultParaPassword.SetUDFun(-1);
	m_EditDefaultParaPassword.SetLRFun(0);
	m_EditDefaultParaPassword.SetAddSubFun(3);
	m_EditDefaultParaPassword.SetHaveUpDownFun(FALSE);
	m_EditDefaultParaPassword.SetHaveLeftRightFun(FALSE);
	m_EditDefaultParaPassword.SetHaveEnterFun(FALSE);
	m_EditDefaultParaPassword.SetHaveAddSubFun(FALSE);
	m_Staticstatic1.SetPosition(7,23,227,38,0);
	m_Staticstatic1.SetTabOrder(-4);
	m_pObjList[4] = &m_Staticstatic1;
	m_Staticstatic1.SetFont(16);
	m_Staticstatic1.SetOwner(this);
	m_Staticstatic1.SetCaption(tr("注意:本次操作需谨慎进行！！！"));
	m_Staticstatic1.SetDataSource(-1,0);
	m_Staticstatic1.SetTimer(0);
	m_Staticstatic2.SetPosition(7,45,227,60,0);
	m_Staticstatic2.SetTabOrder(-5);
	m_pObjList[5] = &m_Staticstatic2;
	m_Staticstatic2.SetFont(16);
	m_Staticstatic2.SetOwner(this);
	m_Staticstatic2.SetCaption(tr("使用本功能将会使装置恢复至出"));
	m_Staticstatic2.SetDataSource(-1,0);
	m_Staticstatic2.SetTimer(0);
	m_Staticdynamic.SetPosition(126,106,227,126,0);
	m_Staticdynamic.SetTabOrder(-2);
	m_pObjList[2] = &m_Staticdynamic;
	m_Staticdynamic.SetFont(16);
	m_Staticdynamic.SetOwner(this);
	value = dwRelayMSCnt;	
	sprintf(dynamic, "%X", value);	
	m_Staticdynamic.SetCaption(dynamic);
	m_Staticdynamic.SetDataSource(-1,0);
	m_Staticdynamic.SetTimer(0);
	m_Staticpassword.SetPosition(17,132,72,152,0);
	m_Staticpassword.SetTabOrder(-3);
	m_pObjList[3] = &m_Staticpassword;
	m_Staticpassword.SetFont(16);
	m_Staticpassword.SetOwner(this);
	m_Staticpassword.SetCaption(tr("密码:"));
	m_Staticpassword.SetDataSource(-1,0);
	m_Staticpassword.SetTimer(0);
	m_Staticstatic3.SetPosition(7,67,227,82,0);
	m_Staticstatic3.SetTabOrder(-6);
	m_pObjList[6] = &m_Staticstatic3;
	m_Staticstatic3.SetFont(16);
	m_Staticstatic3.SetOwner(this);
	m_Staticstatic3.SetCaption(tr("厂状态,当前定值及系数会丢失！"));
	m_Staticstatic3.SetDataSource(-1,0);
	m_Staticstatic3.SetTimer(0);
	m_Staticstatic4.SetPosition(7,85,227,100,0);
	m_Staticstatic4.SetTabOrder(-7);
	m_pObjList[7] = &m_Staticstatic4;
	m_Staticstatic4.SetFont(16);
	m_Staticstatic4.SetOwner(this);
	m_Staticstatic4.SetCaption(tr("如要修改,输入密码后按下确认键"));
	m_Staticstatic4.SetDataSource(-1,0);
	m_Staticstatic4.SetTimer(0);
	m_Staticstatic5.SetPosition(17,106,72,126,0);
	m_Staticstatic5.SetTabOrder(-8);
	m_pObjList[8] = &m_Staticstatic5;
	m_Staticstatic5.SetFont(16);
	m_Staticstatic5.SetOwner(this);
	m_Staticstatic5.SetCaption(tr("动态码:"));
	m_Staticstatic5.SetDataSource(-1,0);
	m_Staticstatic5.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(8);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndDefaultPara,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndDefaultPara)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

BOOL CLcdWndDefaultPara::OnWndInit()
{
	m_EditDefaultParaPassword.SetCaption(tr("0000"));
	return TRUE;
}

void CLcdWndDefaultPara::OnWndEnter()
{
	STATUS rc;
	const char* strInput = m_EditDefaultParaPassword.GetCaption();

	WORD calu;
	char str[16];
	calu = g_kCalPass(value);	
	sprintf(str, "%04d", calu);

	if(strcmp(str, strInput) == 0)
	{		
		rc = DB_UpdateCFData();
		if(rc != DB_ERR_OK)
		{
			char msg[64];
			sprintf(msg, tr("恢复出厂设置失败!\n"));
			HMIMsgBox(tr("恢复出厂设置"),msg,&(CLcdApp::m_WndMainMenu), this, HMI_MB_OK,NULL);
//			return;
		}
		rc = DB_UpdateRelayData();
		if(rc != DB_ERR_OK)
		{
			char msg[64];
			sprintf(msg, tr("恢复出厂设置失败!!\n"));
			HMIMsgBox(tr("恢复出厂设置"),msg,&(CLcdApp::m_WndMainMenu), this, HMI_MB_OK,NULL);
			return;
		}
		CLcdApp::m_WndMainMenu.Show();
		return;
	}
	else
	{
		char msg[64];
		sprintf(msg, tr("密码验证失败!\n"));
		HMIMsgBox(tr("恢复出厂设置"),msg,&(CLcdApp::m_WndMainMenu), this, HMI_MB_OK,NULL);
	}
}
void CLcdWndDefaultPara::DoModal(CHmiWnd* pCancelWnd)
{
	Show();
}
//************窗口version: 关于*******************
CLcdWndversion::~CLcdWndversion()
{
}
CLcdWndversion::CLcdWndversion()
{
}
BOOL CLcdWndversion::Init()
{
/*	//******************产品登记用************************
	SetClassName("CLcdWndversion");
	SetCaption(tr("关于"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);

	m_StaticSsoft.SetPosition(9,29,109,44,0);     
	m_StaticSsoft.SetTabOrder(-1);
	m_pObjList[1] = &m_StaticSsoft;
	m_StaticSsoft.SetFont(16);
	m_StaticSsoft.SetOwner(this);
	m_StaticSsoft.SetCaption(tr("软件版本："));
	m_StaticSsoft.SetDataSource(-1,0);
	m_StaticSsoft.SetTimer(0);

	m_Staticsoft.SetPosition(120,29,228,44,0);
	m_Staticsoft.SetTabOrder(-2);
	m_pObjList[2] = &m_Staticsoft;
	m_Staticsoft.SetFont(16);
	m_Staticsoft.SetOwner(this);
	m_Staticsoft.SetCaption(tr("V1.0"));
	m_Staticsoft.SetDataSource(-1,0);
	m_Staticsoft.SetTimer(0);

	m_StaticSdatabase.SetPosition(9,52,109,67,0);
	m_StaticSdatabase.SetTabOrder(-3);
	m_pObjList[3] = &m_StaticSdatabase;
	m_StaticSdatabase.SetFont(16);
	m_StaticSdatabase.SetOwner(this);
	m_StaticSdatabase.SetCaption(tr("数据库版本:"));
	m_StaticSdatabase.SetDataSource(-1,0);
	m_StaticSdatabase.SetTimer(0);
	
	m_Staticdatabase.SetPosition(120,52,228,67,0);
	m_Staticdatabase.SetTabOrder(-4);
	m_pObjList[4] = &m_Staticdatabase;
	m_Staticdatabase.SetFont(16);
	m_Staticdatabase.SetOwner(this);
	m_Staticdatabase.SetCaption(tr("V1.0"));
	m_Staticdatabase.SetDataSource(-1,0);
	m_Staticdatabase.SetTimer(0);

	m_StaticSsyslogic.SetPosition(9,75,109,90,0);
	m_StaticSsyslogic.SetTabOrder(-5);
	m_pObjList[5] = &m_StaticSsyslogic;
	m_StaticSsyslogic.SetFont(16);
	m_StaticSsyslogic.SetOwner(this);
	m_StaticSsyslogic.SetCaption(tr("逻辑版本"));
	m_StaticSsyslogic.SetDataSource(-1,0);
	m_StaticSsyslogic.SetTimer(0);
	
	m_Staticsyslogic.SetPosition(120,75,228,90,0);
	m_Staticsyslogic.SetTabOrder(-6);
	m_pObjList[6] = &m_Staticsyslogic;
	m_Staticsyslogic.SetFont(16);
	m_Staticsyslogic.SetOwner(this);
	m_Staticsyslogic.SetCaption(tr("V1.0"));
	m_Staticsyslogic.SetDataSource(-1,0);
	m_Staticsyslogic.SetTimer(0);

	m_StaticScrc.SetPosition(9,98,109,113,0);
	m_StaticScrc.SetTabOrder(-7);
	m_pObjList[7] = &m_StaticScrc;
	m_StaticScrc.SetFont(16);
	m_StaticScrc.SetOwner(this);
	m_StaticScrc.SetCaption(tr("校验码:"));
	m_StaticScrc.SetDataSource(-1,0);
	m_StaticScrc.SetTimer(0);
	
	m_Staticcrc.SetPosition(120,98,228,113,0);
	m_Staticcrc.SetTabOrder(-8);
	m_pObjList[8] = &m_Staticcrc;
	m_Staticcrc.SetFont(16);
	m_Staticcrc.SetOwner(this);
	m_Staticcrc.SetCaption(tr("abcd"));
	m_Staticcrc.SetDataSource(-1,0);
	m_Staticcrc.SetTimer(0);

	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(8);
	SetFocus(5L);
	SetTimer(0);
	return TRUE;
*/	//******************以上产品登记用************************

	SetClassName("CLcdWndversion");           
	SetCaption(tr("关于"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	SetEnterWnd(NULL);
	SetUDFun(1);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);

	m_StaticSsoft.SetPosition(9,29,109,44,0);
	m_StaticSsoft.SetTabOrder(-1);
	m_pObjList[1] = &m_StaticSsoft;
	m_StaticSsoft.SetFont(16);
	m_StaticSsoft.SetOwner(this);
	m_StaticSsoft.SetCaption(tr("软件版本："));
	m_StaticSsoft.SetDataSource(-1,0);
	m_StaticSsoft.SetTimer(0);

	m_Staticsoft.SetPosition(28,47,190,62,0);
	m_Staticsoft.SetTabOrder(-2);
	m_pObjList[2] = &m_Staticsoft;
	m_Staticsoft.SetFont(16);
	m_Staticsoft.SetOwner(this);
	m_Staticsoft.SetCaption(tr("6520.1255.1255.1255-"));
	m_Staticsoft.SetDataSource(-1,0);
	m_Staticsoft.SetTimer(0);

	m_Staticcrc.SetPosition(190,47,228,62,0);
	m_Staticcrc.SetTabOrder(-3);
	m_pObjList[14] = &m_Staticcrc;
	m_Staticcrc.SetFont(16);
	m_Staticcrc.SetOwner(this);
	m_Staticcrc.SetCaption(tr("abcd"));
	m_Staticcrc.SetDataSource(-1,0);
	m_Staticcrc.SetTimer(0);
	
	m_StaticSdatabase.SetPosition(9,67,129,82,0);
	m_StaticSdatabase.SetTabOrder(-4);
	m_pObjList[3] = &m_StaticSdatabase;
	m_StaticSdatabase.SetFont(16);
	m_StaticSdatabase.SetOwner(this);
	m_StaticSdatabase.SetCaption(tr("数据库版本:"));
	m_StaticSdatabase.SetDataSource(-1,0);
	m_StaticSdatabase.SetTimer(0);

	m_Staticdatabase.SetPosition(28,85,228,100,0);
	m_Staticdatabase.SetTabOrder(-5);
	m_pObjList[4] = &m_Staticdatabase;
	m_Staticdatabase.SetFont(16);
	m_Staticdatabase.SetOwner(this);
	m_Staticdatabase.SetCaption(tr("6520-1255-ABCD-ABCD"));
	m_Staticdatabase.SetDataSource(-1,0);
	m_Staticdatabase.SetTimer(0);
	
	m_StaticSsyslogic.SetPosition(9,105,109,120,0);
	m_StaticSsyslogic.SetTabOrder(-6);
	m_pObjList[5] = &m_StaticSsyslogic;
	m_StaticSsyslogic.SetFont(16);
	m_StaticSsyslogic.SetOwner(this);
	m_StaticSsyslogic.SetCaption(tr("逻辑版本:"));
	m_StaticSsyslogic.SetDataSource(-1,0);
	m_StaticSsyslogic.SetTimer(0);
	
	m_Staticsyslogic.SetPosition(28,123,228,138,0);
	m_Staticsyslogic.SetTabOrder(-7);
	m_pObjList[6] = &m_Staticsyslogic;
	m_Staticsyslogic.SetFont(16);
	m_Staticsyslogic.SetOwner(this);
	m_Staticsyslogic.SetCaption(tr("6520-1255-ABCD"));
	m_Staticsyslogic.SetDataSource(-1,0);
	m_Staticsyslogic.SetTimer(0);
		
	m_StaticScreatetime.SetPosition(9,29,149,44,1);
	m_StaticScreatetime.SetTabOrder(-8);
	m_pObjList[7] = &m_StaticScreatetime;
	m_StaticScreatetime.SetFont(16);
	m_StaticScreatetime.SetOwner(this);
	m_StaticScreatetime.SetCaption(tr("程序创建时间:"));
	m_StaticScreatetime.SetDataSource(-1,0);
	m_StaticScreatetime.SetTimer(0);
	
	m_Staticcreatetime.SetPosition(28,47,228,62,1);
	m_Staticcreatetime.SetTabOrder(-9);
	m_pObjList[8] = &m_Staticcreatetime;
	m_Staticcreatetime.SetFont(16);
	m_Staticcreatetime.SetOwner(this);
	m_Staticcreatetime.SetCaption(tr("2010-07-26 10:07:25"));
	m_Staticcreatetime.SetDataSource(-1,0);
	m_Staticcreatetime.SetTimer(0);
	
	m_StaticSedittime.SetPosition(9,67,129,182,1);
	m_StaticSedittime.SetTabOrder(-10);
	m_pObjList[9] = &m_StaticSedittime;
	m_StaticSedittime.SetFont(16);
	m_StaticSedittime.SetOwner(this);
	m_StaticSedittime.SetCaption(tr("程序编辑时间"));
	m_StaticSedittime.SetDataSource(-1,0);
	m_StaticSedittime.SetTimer(0);
	
	m_Staticedittime.SetPosition(28,85,228,100,1);
	m_Staticedittime.SetTabOrder(-11);
	m_pObjList[10] = &m_Staticedittime;
	m_Staticedittime.SetFont(16);
	m_Staticedittime.SetOwner(this);
	m_Staticedittime.SetCaption(tr("2010-07-26 10:26:25"));
	m_Staticedittime.SetDataSource(-1,0);
	m_Staticedittime.SetTimer(0);

	m_Staticmac.SetPosition(9,105,129,120,1);
	m_Staticmac.SetTabOrder(-12);
	m_pObjList[11] = &m_Staticmac;
	m_Staticmac.SetFont(16);
	m_Staticmac.SetOwner(this);
	m_Staticmac.SetCaption(tr("装置MAC地址："));
	m_Staticmac.SetDataSource(-1,0);
	m_Staticmac.SetTimer(0);
	
	m_Staticmaca.SetPosition(28,123,228,138,1);
	m_Staticmaca.SetTabOrder(-13);
	m_pObjList[12] = &m_Staticmaca;
	m_Staticmaca.SetFont(16);
	m_Staticmaca.SetOwner(this);
	m_Staticmaca.SetCaption(tr("ETHA:AA-BB-CC-DD-EE-FF"));
	m_Staticmaca.SetDataSource(-1,0);
	m_Staticmaca.SetTimer(0);
	
	m_Staticmacb.SetPosition(28,29,228,44,2);
	m_Staticmacb.SetTabOrder(-14);
	m_pObjList[13] = &m_Staticmacb;
	m_Staticmacb.SetFont(16);
	m_Staticmacb.SetOwner(this);
	m_Staticmacb.SetCaption(tr("ETHB:AA-BB-CC-DD-EE-FF"));
	m_Staticmacb.SetDataSource(-1,0);
	m_Staticmacb.SetTimer(0);


	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(3);
	SetObjNum(14);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;

}

BEGIN_HMIMESSAGE_MAP(CLcdWndversion,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndversion)
///}}HMI_MSG
END_HMIMESSAGE_MAP()
BOOL CLcdWndversion::OnWndInit()
{
	//******************产品登记用************************
/*	TCodeFileInf FileInf;
	DWORD dwCheckLen;	
	char str[32];

	m_Staticsoft.SetCaption(tr("V1.0"));
	m_Staticdatabase.SetCaption(tr("V1.0"));
	m_Staticsyslogic.SetCaption(tr("V1.0"));

	if(DB_GetCodeFileVerInf(&FileInf, &dwCheckLen) == ERR_FAILURE)
		m_Staticcrc.SetCaption(tr("BCDA"));
	else
	{
		sprintf(str,"%x",FileInf.wCheckSum);
		m_Staticcrc.SetCaption(str);
	}
	return TRUE;
*/	//******************以上产品登记用************************
	TCodeFileInf FileInf;
	DWORD dwCheckLen;

	BYTE dataver[8];
	char format[16];
	char str[32];
	char str1[5];
	char str2[5];
	char str3[5];
	char str4[5];
	//逻辑方程版本号及校验码
	DWORD temp = GetPLCDBVer();
	WORD temp1,temp2;
	WORD part1,part2,part3,part4;
	temp1 = LOWORD(temp);
	temp2 = HIWORD(temp);
	
	if(PLC_STATUS_ERR == g_PLCStatus)
		sprintf(str,"%X-%X-%s",temp2,temp1,"ERRO");
	else if(PLC_SYS_OK == g_PLCStatus)
		sprintf(str,"%X-%X-%X",temp2,temp1,GetPLCCheckSum());
	else if(PLC_DEFAULT_OK == g_PLCStatus)
		sprintf(str,"%X-%X-%s",temp2,temp1,"DEFT");
	m_Staticsyslogic.SetCaption(str);


	if(DB_ERR_OK == DB_GetDataBaseVersion(dataver))
	{
		part1 = MAKEWORD(dataver[2], dataver[3]);		
		part2 = MAKEWORD(dataver[0], dataver[1]);
		part3 = MAKEWORD(dataver[4], dataver[5]);
		part4 = MAKEWORD(dataver[6], dataver[7]);		
		sprintf(str,"%X-%X-%X-%X",part1,part2,part3,part4);
		m_Staticdatabase.SetCaption(str);
	}
	else
	{
		sprintf(str,"%s","ERROR");
		m_Staticdatabase.SetCaption(str);
	}

	
	if(DB_GetCodeFileVerInf(&FileInf, &dwCheckLen) == ERR_FAILURE)
	{
		//ReportMsg("Error:Read CRC Error!!!");
		
		//校验码
		m_Staticcrc.SetCaption(tr("BCDA"));

		//软件版本号
		m_Staticsoft.SetCaption(tr("6500.2012.0000.0000-"));
		//软件创建时间
		m_Staticcreatetime.SetCaption(tr("2012-01-01 00:00:00"));
		//软件编辑时间
		m_Staticedittime.SetCaption(tr("2012-01-01 00:00:00"));
	}
	else
	{
		//校验码
		sprintf(str,"%x",FileInf.wCheckSum);
		m_Staticcrc.SetCaption(str);

		//软件版本号
		for(WORD i=0;i<4;i++)
		{
			str1[i]=FileInf.SoftWareVer[i];
			str2[i]=FileInf.SoftWareVer[i+4];
			str3[i]=FileInf.SoftWareVer[i+8];
			str4[i]=FileInf.SoftWareVer[i+12];
		}
		str1[4]='\0';
		str2[4]='\0';
		str3[4]='\0';
		str4[4]='\0';
		sprintf(str,"%s.%s.%s.%s-",str1,str2,str3,str4);
		m_Staticsoft.SetCaption(str);
		//软件创建时间
		
		sprintf(str,"%d.%d.%d %d:%d:%d",FileInf.CompileTime.Year,FileInf.CompileTime.Month,FileInf.CompileTime.Day,\
		FileInf.CompileTime.Hour,FileInf.CompileTime.Minute,FileInf.CompileTime.Second);
		m_Staticcreatetime.SetCaption(str);
		//软件编辑时间
		sprintf(str,"%d.%d.%d %d:%d:%d",FileInf.EditTime.Year,FileInf.EditTime.Month,FileInf.EditTime.Day, \
		FileInf.EditTime.Hour,FileInf.EditTime.Minute,FileInf.EditTime.Second);
		m_Staticedittime.SetCaption(str);
	}
	BYTE mac[6];
	BYTE s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12;
	DB_GetNetMac(0,mac);	
	s1=mac[0]>>4;
	s2=mac[0]&0x0F;	
	s3=mac[1]>>4;
	s4=mac[1]&0x0F;
	s5=mac[2]>>4;
	s6=mac[2]&0x0F;
	s7=mac[3]>>4;
	s8=mac[3]&0x0F;
	s9=mac[4]>>4;
	s10=mac[4]&0x0F;
	s11=mac[5]>>4;
	s12=mac[5]&0x0F;	
	sprintf(str,"ETHA:%X%X-%X%X-%X%X-%X%X-%X%X-%X%X",s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12);
	m_Staticmaca.SetCaption(str);
	
	DB_GetNetMac(1,mac);	
	s1=mac[0]>>4;
	s2=mac[0]&0x0F;	
	s3=mac[1]>>4;
	s4=mac[1]&0x0F;
	s5=mac[2]>>4;
	s6=mac[2]&0x0F;
	s7=mac[3]>>4;
	s8=mac[3]&0x0F;
	s9=mac[4]>>4;
	s10=mac[4]&0x0F;
	s11=mac[5]>>4;
	s12=mac[5]&0x0F;	
	sprintf(str,"ETHB:%X%X-%X%X-%X%X-%X%X-%X%X-%X%X",s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12);
	m_Staticmacb.SetCaption(str);
	return TRUE;

}
void CLcdWndversion::DoModal(CHmiWnd* pCancelWnd)
{
	Show();
}

//************窗口AutoJustSetp2: 自动系数整定step2*******************
CLcdWndAutoJustStep2::~CLcdWndAutoJustStep2()
{
}
CLcdWndAutoJustStep2::CLcdWndAutoJustStep2()
{
}
BOOL CLcdWndAutoJustStep2::Init()
{
	SetClassName("CLcdWndAutoJustStep2");
	SetCaption(tr("自动系数整定step2"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndfactorySet));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditBaseV.SetPosition(104,32,204,52,0);
	m_EditBaseV.SetTabOrder(1);
	m_pObjList[1] = &m_EditBaseV;
	m_EditBaseV.SetFont(16);
	m_EditBaseV.SetOwner(this);
	m_EditBaseV.SetCaption(tr("1"));
	m_EditBaseV.SetMaxLen(3);
	m_EditBaseV.SetDecimalNum(0);
	m_EditBaseV.SetValueRange(200,50);
	m_EditBaseV.SetIsShowNegative(FALSE);
	m_EditBaseV.SetUDFun(-1);
	m_EditBaseV.SetLRFun(0);
	m_EditBaseV.SetAddSubFun(3);
	m_EditBaseV.SetHaveUpDownFun(FALSE);
	m_EditBaseV.SetHaveLeftRightFun(FALSE);
	m_EditBaseV.SetHaveEnterFun(FALSE);
	m_EditBaseV.SetHaveAddSubFun(FALSE);
	m_EditBaseI.SetPosition(104,62,204,82,0);
	m_EditBaseI.SetTabOrder(2);
	m_pObjList[2] = &m_EditBaseI;
	m_EditBaseI.SetFont(16);
	m_EditBaseI.SetOwner(this);
	m_EditBaseI.SetCaption(tr("1"));
	m_EditBaseI.SetMaxLen(3);
	m_EditBaseI.SetDecimalNum(0);
	m_EditBaseI.SetValueRange(10,1);
	m_EditBaseI.SetIsShowNegative(FALSE);
	m_EditBaseI.SetUDFun(-1);
	m_EditBaseI.SetLRFun(0);
	m_EditBaseI.SetAddSubFun(3);
	m_EditBaseI.SetHaveUpDownFun(FALSE);
	m_EditBaseI.SetHaveLeftRightFun(FALSE);
	m_EditBaseI.SetHaveEnterFun(FALSE);
	m_EditBaseI.SetHaveAddSubFun(FALSE);
	m_Static5067.SetPosition(12,36,67,51,0);
	m_Static5067.SetTabOrder(-3);
	m_pObjList[3] = &m_Static5067;
	m_Static5067.SetFont(16);
	m_Static5067.SetOwner(this);
	m_Static5067.SetCaption(tr("电压(v)"));
	m_Static5067.SetDataSource(-1,0);
	m_Static5067.SetTimer(0);
	m_Static5070.SetPosition(12,67,67,87,0);
	m_Static5070.SetTabOrder(-4);
	m_pObjList[4] = &m_Static5070;
	m_Static5070.SetFont(16);
	m_Static5070.SetOwner(this);
	m_Static5070.SetCaption(tr("电流(A)"));
	m_Static5070.SetDataSource(-1,0);
	m_Static5070.SetTimer(0);
	m_Static5072.SetPosition(9,97,229,117,0);
	m_Static5072.SetTabOrder(-5);
	m_pObjList[5] = &m_Static5072;
	m_Static5072.SetFont(16);
	m_Static5072.SetOwner(this);
	m_Static5072.SetCaption(tr("输入电压电流值 加相应电压电流"));
	m_Static5072.SetDataSource(-1,0);
	m_Static5072.SetTimer(0);
	m_Static5073.SetPosition(8,120,218,140,0);
	m_Static5073.SetTabOrder(-6);
	m_pObjList[6] = &m_Static5073;
	m_Static5073.SetFont(16);
	m_Static5073.SetOwner(this);
	m_Static5073.SetCaption(tr("按Enter开始整定"));
	m_Static5073.SetDataSource(-1,0);
	m_Static5073.SetTimer(0);
	m_Static5074.SetPosition(9,140,224,159,0);
	m_Static5074.SetTabOrder(-7);
	m_pObjList[7] = &m_Static5074;
	m_Static5074.SetFont(16);
	m_Static5074.SetOwner(this);
	m_Static5074.SetCaption(tr("整定过程中不可取消"));
	m_Static5074.SetDataSource(-1,0);
	m_Static5074.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(7);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndAutoJustStep2,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndAutoJustStep2)
///}}HMI_MSG
END_HMIMESSAGE_MAP()


//************窗口AutoJustStep1: 自动系数整定step1*******************
CLcdWndAutoJustStep1::~CLcdWndAutoJustStep1()
{
}
CLcdWndAutoJustStep1::CLcdWndAutoJustStep1()
{
}
BOOL CLcdWndAutoJustStep1::Init()
{
	SetClassName("CLcdWndAutoJustStep1");
	SetCaption(tr("自动系数整定step1"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndfactorySet));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_Static5066.SetPosition(10,55,215,80,0);
	m_Static5066.SetTabOrder(-1);
	m_pObjList[1] = &m_Static5066;
	m_Static5066.SetFont(16);
	m_Static5066.SetOwner(this);
	m_Static5066.SetCaption(tr("需将供电方式设置为0,不加电"));
	m_Static5066.SetDataSource(-1,0);
	m_Static5066.SetTimer(0);
	m_Static5063.SetPosition(10,30,215,55,0);
	m_Static5063.SetTabOrder(-2);
	m_pObjList[2] = &m_Static5063;
	m_Static5063.SetFont(16);
	m_Static5063.SetOwner(this);
	m_Static5063.SetCaption(tr("死区值校正:"));
	m_Static5063.SetDataSource(-1,0);
	m_Static5063.SetTimer(0);
	m_Static5064.SetPosition(10,109,215,134,0);
	m_Static5064.SetTabOrder(-3);
	m_pObjList[3] = &m_Static5064;
	m_Static5064.SetFont(16);
	m_Static5064.SetOwner(this);
	m_Static5064.SetCaption(tr("按Enter开始校正"));
	m_Static5064.SetDataSource(-1,0);
	m_Static5064.SetTimer(0);
	m_Static5065.SetPosition(10,132,215,157,0);
	m_Static5065.SetTabOrder(-4);
	m_pObjList[4] = &m_Static5065;
	m_Static5065.SetFont(16);
	m_Static5065.SetOwner(this);
	m_Static5065.SetCaption(tr("按Esc返回上一级菜单"));
	m_Static5065.SetDataSource(-1,0);
	m_Static5065.SetTimer(0);
	m_Static5068.SetPosition(10,79,215,104,0);
	m_Static5068.SetTabOrder(-5);
	m_pObjList[5] = &m_Static5068;
	m_Static5068.SetFont(16);
	m_Static5068.SetOwner(this);
	m_Static5068.SetCaption(tr("流电压,校正过程中不能取消"));
	m_Static5068.SetDataSource(-1,0);
	m_Static5068.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(5);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}

BEGIN_HMIMESSAGE_MAP(CLcdWndAutoJustStep1,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndAutoJustStep1)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

BOOL CLcdWndAutoJustStep2::OnWndInit()
{
	//设置电压电流默认值
	m_EditBaseI.SetValue(1);
	m_EditBaseV.SetValue(100);
	m_EditBaseI.UpdateData(false);
	m_EditBaseV.UpdateData(false);

	return TRUE;
}
void CLcdWndAutoJustStep2::OnWndEnter()
{
	//读取输入的电压电流
	WORD wV = (WORD)m_EditBaseV.GetValue();
	WORD wI = (WORD)m_EditBaseI.GetValue();


	//发送校正消息
		OS_PostMessage(dwAITaskID,SM_AUTOADJUST,RtosGetCurrentTaskId(),wV,wI,0);
	
	//显示提示窗口
	HMIMsgBox(tr("校正系数"), tr("正在校正系数,\n请等待..."));

}
void CLcdWndAutoJustStep1::OnWndEnter()
{
	//发送死区校正消息
	OS_PostMessage(dwAITaskID,SM_AUTOADJUST, RtosGetCurrentTaskId(),0,0, 0);
	
	//显示提示窗口
	HMIMsgBox(tr("校正系数"), tr("正在校正死区系数,\n请等待..."));

}
//************窗口PortPara: 端口参数*******************
CLcdWndPortPara::~CLcdWndPortPara()
{
}
CLcdWndPortPara::CLcdWndPortPara()
{
}
BOOL CLcdWndPortPara::Init()
{
	SetClassName("CLcdWndPortPara");
	SetCaption(tr("端口参数"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndPortStatus));//m_WndAddress));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(1);
	SetLROffset(1);
	m_ListPortParaList.SetPosition(10,26,235,156,0);
	m_ListPortParaList.SetTabOrder(1);
	m_pObjList[1] = &m_ListPortParaList;
	m_ListPortParaList.SetFont(16);
	m_ListPortParaList.SetOwner(this);
	m_ListPortParaList.SetType(PortPara);
	m_ListPortParaList.SetOwner(this);
	m_ListPortParaList.SetTimer(0);
	m_ListPortParaList.SetUDFun(0);
	m_ListPortParaList.SetLRFun(0);
	m_ListPortParaList.SetHaveUpDownFun(FALSE);
	m_ListPortParaList.SetHaveLeftRightFun(FALSE);
	m_ListPortParaList.SetHaveEnterFun(FALSE);
	m_ListPortParaList.SetHaveAddSubFun(FALSE);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(1);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}
void CLcdWndPortPara::SavePortPara() 
{	
	CHmiWnd *pWnd = &CLcdApp::m_WndPortStatus;
	if(!HMISavePortParaToDB())
	{
		HMIMsgBox(pWnd->tr("端口参数"), pWnd->tr("端口参数保存失败!"), pWnd, pWnd, HMI_MB_OKCANCEL);
		
	}
	else
	{
		pWnd->Show();
	}
	
	//保存函数出现部分保存正确，部分保存错误也返回错误
	//所以不管保存函数返回成功否都发改消息，
	//OS_PostMessage(dwTaskDIId,SM_BIPARACHANGE,0,0,0,0);
}
void CLcdWndPortPara::OnWndEnter()
{
	if(!CHmiApp::m_bPortParaModified)
		return;

	YES_NO_PTR pfn = (YES_NO_PTR)SavePortPara;
	HMIMsgBox(tr("端口参数"),  tr("确定要保存修改的内容?\n修改后请复位装置!"),  this, this, HMI_MB_OKCANCEL, pfn);
}
void CLcdWndPortPara::DoModal(CHmiWnd* pCancelWnd)
{
	
	Show();

}

BEGIN_HMIMESSAGE_MAP(CLcdWndPortPara,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndPortPara)
///}}HMI_MSG
END_HMIMESSAGE_MAP()



//************窗口ETHportModity: 通信端口*******************
CLcdWndETHportModity::~CLcdWndETHportModity()
{
}
CLcdWndETHportModity::CLcdWndETHportModity()
{
}
BOOL CLcdWndETHportModity::Init()
{
	SetClassName("CLcdWndETHportModity");
	SetCaption(tr("通信端口"));
	SetEscWnd((CHmiWnd*)&(CLcdApp::m_WndPortStatus));
	SetEnterWnd(NULL);
	SetUDFun(0);
	SetUDOffset(1);
	SetLRFun(0);
	SetLROffset(1);
	m_EditETHA_Link1.SetPosition(71,30,111,50,0);
	m_EditETHA_Link1.SetTabOrder(1);
	m_pObjList[1] = &m_EditETHA_Link1;
	m_EditETHA_Link1.SetFont(16);
	m_EditETHA_Link1.SetOwner(this);
	m_EditETHA_Link1.SetCaption(tr("0000"));
	m_EditETHA_Link1.SetMaxLen(4);
	m_EditETHA_Link1.SetDecimalNum(0);
	m_EditETHA_Link1.SetValueRange(9999,1);
	m_EditETHA_Link1.SetIsShowNegative(FALSE);
	m_EditETHA_Link1.SetUDFun(-1);
	m_EditETHA_Link1.SetLRFun(0);
	m_EditETHA_Link1.SetAddSubFun(3);
	m_EditETHA_Link1.SetHaveUpDownFun(FALSE);
	m_EditETHA_Link1.SetHaveLeftRightFun(FALSE);
	m_EditETHA_Link1.SetHaveEnterFun(FALSE);
	m_EditETHA_Link1.SetHaveAddSubFun(FALSE);
	m_EditETHA_Link2.SetPosition(71,57,111,77,0);
	m_EditETHA_Link2.SetTabOrder(2);
	m_pObjList[2] = &m_EditETHA_Link2;
	m_EditETHA_Link2.SetFont(16);
	m_EditETHA_Link2.SetOwner(this);
	m_EditETHA_Link2.SetCaption(tr("0000"));
	m_EditETHA_Link2.SetMaxLen(4);
	m_EditETHA_Link2.SetDecimalNum(0);
	m_EditETHA_Link2.SetValueRange(9999,1);
	m_EditETHA_Link2.SetIsShowNegative(FALSE);
	m_EditETHA_Link2.SetUDFun(-1);
	m_EditETHA_Link2.SetLRFun(0);
	m_EditETHA_Link2.SetAddSubFun(3);
	m_EditETHA_Link2.SetHaveUpDownFun(FALSE);
	m_EditETHA_Link2.SetHaveLeftRightFun(FALSE);
	m_EditETHA_Link2.SetHaveEnterFun(FALSE);
	m_EditETHA_Link2.SetHaveAddSubFun(FALSE);
	m_EditETHA_Link3.SetPosition(71,84,111,104,0);
	m_EditETHA_Link3.SetTabOrder(3);
	m_pObjList[3] = &m_EditETHA_Link3;
	m_EditETHA_Link3.SetFont(16);
	m_EditETHA_Link3.SetOwner(this);
	m_EditETHA_Link3.SetCaption(tr("0000"));
	m_EditETHA_Link3.SetMaxLen(4);
	m_EditETHA_Link3.SetDecimalNum(0);
	m_EditETHA_Link3.SetValueRange(9999,1);
	m_EditETHA_Link3.SetIsShowNegative(FALSE);
	m_EditETHA_Link3.SetUDFun(-1);
	m_EditETHA_Link3.SetLRFun(0);
	m_EditETHA_Link3.SetAddSubFun(3);
	m_EditETHA_Link3.SetHaveUpDownFun(FALSE);
	m_EditETHA_Link3.SetHaveLeftRightFun(FALSE);
	m_EditETHA_Link3.SetHaveEnterFun(FALSE);
	m_EditETHA_Link3.SetHaveAddSubFun(FALSE);
	m_EditETHA_Link4.SetPosition(71,111,111,131,0);
	m_EditETHA_Link4.SetTabOrder(4);
	m_pObjList[4] = &m_EditETHA_Link4;
	m_EditETHA_Link4.SetFont(16);
	m_EditETHA_Link4.SetOwner(this);
	m_EditETHA_Link4.SetCaption(tr("0000"));
	m_EditETHA_Link4.SetMaxLen(4);
	m_EditETHA_Link4.SetDecimalNum(0);
	m_EditETHA_Link4.SetValueRange(9999,1);
	m_EditETHA_Link4.SetIsShowNegative(FALSE);
	m_EditETHA_Link4.SetUDFun(-1);
	m_EditETHA_Link4.SetLRFun(0);
	m_EditETHA_Link4.SetAddSubFun(3);
	m_EditETHA_Link4.SetHaveUpDownFun(FALSE);
	m_EditETHA_Link4.SetHaveLeftRightFun(FALSE);
	m_EditETHA_Link4.SetHaveEnterFun(FALSE);
	m_EditETHA_Link4.SetHaveAddSubFun(FALSE);
	m_EditETHA_Link5.SetPosition(71,138,111,158,0);
	m_EditETHA_Link5.SetTabOrder(5);
	m_pObjList[5] = &m_EditETHA_Link5;
	m_EditETHA_Link5.SetFont(16);
	m_EditETHA_Link5.SetOwner(this);
	m_EditETHA_Link5.SetCaption(tr("0000"));
	m_EditETHA_Link5.SetMaxLen(4);
	m_EditETHA_Link5.SetDecimalNum(0);
	m_EditETHA_Link5.SetValueRange(9999,1);
	m_EditETHA_Link5.SetIsShowNegative(FALSE);
	m_EditETHA_Link5.SetUDFun(-1);
	m_EditETHA_Link5.SetLRFun(0);
	m_EditETHA_Link5.SetAddSubFun(3);
	m_EditETHA_Link5.SetHaveUpDownFun(FALSE);
	m_EditETHA_Link5.SetHaveLeftRightFun(FALSE);
	m_EditETHA_Link5.SetHaveEnterFun(FALSE);
	m_EditETHA_Link5.SetHaveAddSubFun(FALSE);
	m_EditETHB_Link1.SetPosition(181,30,221,50,0);
	m_EditETHB_Link1.SetTabOrder(6);
	m_pObjList[6] = &m_EditETHB_Link1;
	m_EditETHB_Link1.SetFont(16);
	m_EditETHB_Link1.SetOwner(this);
	m_EditETHB_Link1.SetCaption(tr("0000"));
	m_EditETHB_Link1.SetMaxLen(4);
	m_EditETHB_Link1.SetDecimalNum(0);
	m_EditETHB_Link1.SetValueRange(9999,1);
	m_EditETHB_Link1.SetIsShowNegative(FALSE);
	m_EditETHB_Link1.SetUDFun(-1);
	m_EditETHB_Link1.SetLRFun(0);
	m_EditETHB_Link1.SetAddSubFun(3);
	m_EditETHB_Link1.SetHaveUpDownFun(FALSE);
	m_EditETHB_Link1.SetHaveLeftRightFun(FALSE);
	m_EditETHB_Link1.SetHaveEnterFun(FALSE);
	m_EditETHB_Link1.SetHaveAddSubFun(FALSE);
	m_EditETHB_Link2.SetPosition(181,57,221,77,0);
	m_EditETHB_Link2.SetTabOrder(7);
	m_pObjList[7] = &m_EditETHB_Link2;
	m_EditETHB_Link2.SetFont(16);
	m_EditETHB_Link2.SetOwner(this);
	m_EditETHB_Link2.SetCaption(tr("0000"));
	m_EditETHB_Link2.SetMaxLen(4);
	m_EditETHB_Link2.SetDecimalNum(0);
	m_EditETHB_Link2.SetValueRange(9999,1);
	m_EditETHB_Link2.SetIsShowNegative(FALSE);
	m_EditETHB_Link2.SetUDFun(-1);
	m_EditETHB_Link2.SetLRFun(0);
	m_EditETHB_Link2.SetAddSubFun(3);
	m_EditETHB_Link2.SetHaveUpDownFun(FALSE);
	m_EditETHB_Link2.SetHaveLeftRightFun(FALSE);
	m_EditETHB_Link2.SetHaveEnterFun(FALSE);
	m_EditETHB_Link2.SetHaveAddSubFun(FALSE);
	m_EditETHB_Link3.SetPosition(181,84,221,104,0);
	m_EditETHB_Link3.SetTabOrder(8);
	m_pObjList[8] = &m_EditETHB_Link3;
	m_EditETHB_Link3.SetFont(16);
	m_EditETHB_Link3.SetOwner(this);
	m_EditETHB_Link3.SetCaption(tr("0000"));
	m_EditETHB_Link3.SetMaxLen(4);
	m_EditETHB_Link3.SetDecimalNum(0);
	m_EditETHB_Link3.SetValueRange(9999,1);
	m_EditETHB_Link3.SetIsShowNegative(FALSE);
	m_EditETHB_Link3.SetUDFun(-1);
	m_EditETHB_Link3.SetLRFun(0);
	m_EditETHB_Link3.SetAddSubFun(3);
	m_EditETHB_Link3.SetHaveUpDownFun(FALSE);
	m_EditETHB_Link3.SetHaveLeftRightFun(FALSE);
	m_EditETHB_Link3.SetHaveEnterFun(FALSE);
	m_EditETHB_Link3.SetHaveAddSubFun(FALSE);
	m_EditETHB_Link4.SetPosition(181,111,221,131,0);
	m_EditETHB_Link4.SetTabOrder(9);
	m_pObjList[9] = &m_EditETHB_Link4;
	m_EditETHB_Link4.SetFont(16);
	m_EditETHB_Link4.SetOwner(this);
	m_EditETHB_Link4.SetCaption(tr("0000"));
	m_EditETHB_Link4.SetMaxLen(4);
	m_EditETHB_Link4.SetDecimalNum(0);
	m_EditETHB_Link4.SetValueRange(9999,1);
	m_EditETHB_Link4.SetIsShowNegative(FALSE);
	m_EditETHB_Link4.SetUDFun(-1);
	m_EditETHB_Link4.SetLRFun(0);
	m_EditETHB_Link4.SetAddSubFun(3);
	m_EditETHB_Link4.SetHaveUpDownFun(FALSE);
	m_EditETHB_Link4.SetHaveLeftRightFun(FALSE);
	m_EditETHB_Link4.SetHaveEnterFun(FALSE);
	m_EditETHB_Link4.SetHaveAddSubFun(FALSE);
	m_EditETHB_Link5.SetPosition(181,138,221,158,0);
	m_EditETHB_Link5.SetTabOrder(10);
	m_pObjList[10] = &m_EditETHB_Link5;
	m_EditETHB_Link5.SetFont(16);
	m_EditETHB_Link5.SetOwner(this);
	m_EditETHB_Link5.SetCaption(tr("0000"));
	m_EditETHB_Link5.SetMaxLen(4);
	m_EditETHB_Link5.SetDecimalNum(0);
	m_EditETHB_Link5.SetValueRange(9999,1);
	m_EditETHB_Link5.SetIsShowNegative(FALSE);
	m_EditETHB_Link5.SetUDFun(-1);
	m_EditETHB_Link5.SetLRFun(0);
	m_EditETHB_Link5.SetAddSubFun(3);
	m_EditETHB_Link5.SetHaveUpDownFun(FALSE);
	m_EditETHB_Link5.SetHaveLeftRightFun(FALSE);
	m_EditETHB_Link5.SetHaveEnterFun(FALSE);
	m_EditETHB_Link5.SetHaveAddSubFun(FALSE);
	m_Static5619.SetPosition(10,30,70,50,0);
	m_Static5619.SetTabOrder(-12);
	m_pObjList[12] = &m_Static5619;
	m_Static5619.SetFont(16);
	m_Static5619.SetOwner(this);
	m_Static5619.SetCaption(tr("ETHA_1:"));
	m_Static5619.SetDataSource(-1,0);
	m_Static5619.SetTimer(0);
	m_Static5620.SetPosition(10,57,70,77,0);
	m_Static5620.SetTabOrder(-13);
	m_pObjList[13] = &m_Static5620;
	m_Static5620.SetFont(16);
	m_Static5620.SetOwner(this);
	m_Static5620.SetCaption(tr("ETHA_2:"));
	m_Static5620.SetDataSource(-1,0);
	m_Static5620.SetTimer(0);
	m_Static5621.SetPosition(10,84,70,104,0);
	m_Static5621.SetTabOrder(-14);
	m_pObjList[14] = &m_Static5621;
	m_Static5621.SetFont(16);
	m_Static5621.SetOwner(this);
	m_Static5621.SetCaption(tr("ETHA_3:"));
	m_Static5621.SetDataSource(-1,0);
	m_Static5621.SetTimer(0);
	m_Static5622.SetPosition(10,111,70,131,0);
	m_Static5622.SetTabOrder(-15);
	m_pObjList[15] = &m_Static5622;
	m_Static5622.SetFont(16);
	m_Static5622.SetOwner(this);
	m_Static5622.SetCaption(tr("ETHA_4:"));
	m_Static5622.SetDataSource(-1,0);
	m_Static5622.SetTimer(0);
	m_Static5623.SetPosition(10,138,70,158,0);
	m_Static5623.SetTabOrder(-16);
	m_pObjList[16] = &m_Static5623;
	m_Static5623.SetFont(16);
	m_Static5623.SetOwner(this);
	m_Static5623.SetCaption(tr("ETHA_5:"));
	m_Static5623.SetDataSource(-1,0);
	m_Static5623.SetTimer(0);
	m_Static5624.SetPosition(120,30,180,50,0);
	m_Static5624.SetTabOrder(-17);
	m_pObjList[17] = &m_Static5624;
	m_Static5624.SetFont(16);
	m_Static5624.SetOwner(this);
	m_Static5624.SetCaption(tr("ETHB_1:"));
	m_Static5624.SetDataSource(-1,0);
	m_Static5624.SetTimer(0);
	m_Static5625.SetPosition(120,57,180,77,0);
	m_Static5625.SetTabOrder(-18);
	m_pObjList[18] = &m_Static5625;
	m_Static5625.SetFont(16);
	m_Static5625.SetOwner(this);
	m_Static5625.SetCaption(tr("ETHB_2:"));
	m_Static5625.SetDataSource(-1,0);
	m_Static5625.SetTimer(0);
	m_Static5626.SetPosition(120,84,180,104,0);
	m_Static5626.SetTabOrder(-11);
	m_pObjList[11] = &m_Static5626;
	m_Static5626.SetFont(16);
	m_Static5626.SetOwner(this);
	m_Static5626.SetCaption(tr("ETHB_3:"));
	m_Static5626.SetDataSource(-1,0);
	m_Static5626.SetTimer(0);
	m_Static5627.SetPosition(120,111,180,131,0);
	m_Static5627.SetTabOrder(-19);
	m_pObjList[19] = &m_Static5627;
	m_Static5627.SetFont(16);
	m_Static5627.SetOwner(this);
	m_Static5627.SetCaption(tr("ETHB_4:"));
	m_Static5627.SetDataSource(-1,0);
	m_Static5627.SetTimer(0);
	m_Static5628.SetPosition(120,138,180,158,0);
	m_Static5628.SetTabOrder(-20);
	m_pObjList[20] = &m_Static5628;
	m_Static5628.SetFont(16);
	m_Static5628.SetOwner(this);
	m_Static5628.SetCaption(tr("ETHB_5:"));
	m_Static5628.SetDataSource(-1,0);
	m_Static5628.SetTimer(0);
	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);
	SetPageNum(1);
	SetObjNum(20);
	SetFocus(1L);
	SetTimer(0);
	return TRUE;
}
BEGIN_HMIMESSAGE_MAP(CLcdWndETHportModity,CHmiWnd)
//{{HMI_MSG_MAP(CLcdWndETHportModity)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

BOOL CLcdWndETHportModity::OnWndInit()
{		
	TEthPortPara tEth;
	for(LONG n=0;n<10;n++)
	{
		if(DB_ERR_OK != DB_GetETHPara(n,&tEth))
			return FALSE;
		ethportnum[n] = tEth.dwSelfPort;
		CHmiApp::m_wpPortProtocl[n] = tEth.dwSelfPort;
	}
	m_EditETHA_Link1.SetValue(ethportnum[0]);
	m_EditETHA_Link2.SetValue(ethportnum[1]);
	m_EditETHA_Link3.SetValue(ethportnum[2]);
	m_EditETHA_Link4.SetValue(ethportnum[3]);
	m_EditETHA_Link5.SetValue(ethportnum[4]);
	m_EditETHB_Link1.SetValue(ethportnum[5]);
	m_EditETHB_Link2.SetValue(ethportnum[6]);
	m_EditETHB_Link3.SetValue(ethportnum[7]);
	m_EditETHB_Link4.SetValue(ethportnum[8]);
	m_EditETHB_Link5.SetValue(ethportnum[9]);
	m_EditETHA_Link1.UpdateData(FALSE);	
	m_EditETHA_Link2.UpdateData(FALSE);	
	m_EditETHA_Link3.UpdateData(FALSE);	
	m_EditETHA_Link4.UpdateData(FALSE);	
	m_EditETHA_Link5.UpdateData(FALSE);	
	m_EditETHB_Link1.UpdateData(FALSE);	
	m_EditETHB_Link2.UpdateData(FALSE);	
	m_EditETHB_Link3.UpdateData(FALSE);	
	m_EditETHB_Link4.UpdateData(FALSE);	
	m_EditETHB_Link5.UpdateData(FALSE);	

	return TRUE;
}
void CLcdWndETHportModity::OnWndEnter()
{	
	m_EditETHA_Link1.UpdateData(TRUE);	
	m_EditETHA_Link2.UpdateData(TRUE);	
	m_EditETHA_Link3.UpdateData(TRUE);	
	m_EditETHA_Link4.UpdateData(TRUE);	
	m_EditETHA_Link5.UpdateData(TRUE);	
	m_EditETHB_Link1.UpdateData(TRUE);	
	m_EditETHB_Link2.UpdateData(TRUE);	
	m_EditETHB_Link3.UpdateData(TRUE);	
	m_EditETHB_Link4.UpdateData(TRUE);	
	m_EditETHB_Link5.UpdateData(TRUE);	
	if(ethportnum[0]!=(WORD)m_EditETHA_Link1.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[0] = (WORD)m_EditETHA_Link1.GetValue();
	}
	if(ethportnum[1]!=(WORD)m_EditETHA_Link2.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[1] = (WORD)m_EditETHA_Link2.GetValue();
	}
	if(ethportnum[2]!=(WORD)m_EditETHA_Link3.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[2] = (WORD)m_EditETHA_Link3.GetValue();
	}
	if(ethportnum[3]!=(WORD)m_EditETHA_Link4.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[3] = (WORD)m_EditETHA_Link4.GetValue();
	}
	if(ethportnum[4]!=(WORD)m_EditETHA_Link5.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[4] = (WORD)m_EditETHA_Link5.GetValue();
	}
	if(ethportnum[5]!=(WORD)m_EditETHB_Link1.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[5] = (WORD)m_EditETHB_Link1.GetValue();
	}
	if(ethportnum[6]!=(WORD)m_EditETHB_Link2.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[6] = (WORD)m_EditETHB_Link2.GetValue();
	}
	if(ethportnum[7]!=(WORD)m_EditETHB_Link3.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[7] = (WORD)m_EditETHB_Link3.GetValue();
	}
	if(ethportnum[8]!=(WORD)m_EditETHB_Link4.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[8] = (WORD)m_EditETHB_Link4.GetValue();
	}
	if(ethportnum[9]!=(WORD)m_EditETHB_Link5.GetValue())
	{	
		CHmiApp::m_wpPortProtocl[9] = (WORD)m_EditETHB_Link5.GetValue();
	}
	char Msg1[64];
	sprintf(Msg1, tr("确定要修改通信端口参数?\n修改后请复位装置!"));
	YES_NO_PTR pfn = (YES_NO_PTR)SaveETHPortNum;		
	HMIMsgBox(tr("通信端口"),  Msg1,  this, this, HMI_MB_OKCANCEL, (YES_NO_PTR)pfn);
		
	
}
void CLcdWndETHportModity::SaveETHPortNum()
{
	STATUS rc;		
	TEthPortPara tEth;
	CHmiWnd *pWnd = &(CLcdApp::m_WndPortStatus);

	WORD port;
	for(LONG n=0;n<10;n++)
	{
		if(DB_ERR_OK != DB_GetETHPara(n,&tEth))
			break;
		port = tEth.dwSelfPort;

		if(port != CHmiApp::m_wpPortProtocl[n])
		{
			tEth.dwSelfPort = CHmiApp::m_wpPortProtocl[n];
			if ((DB_WriteETHPara(n,&tEth) != DB_ERR_OK)||(DB_WriteETHParaConfirm(n) !=DB_ERR_OK))
			{
				HMIMsgBox(pWnd->tr("通信端口"), pWnd->tr("保存通信端口失败!"), pWnd, pWnd);
				break;
			}
		}
	}

	pWnd->Show();
}
void CLcdWndETHportModity::DoModal(CHmiWnd* pCancelWnd)
{
	
	Show();

}

