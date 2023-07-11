/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：HmiApp.cpp
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-3-3
*   作    者：kangqian
*   功    能：<...>
* 
*************************************************************************/

// HmiApp.cpp: implementation of the CHmiApp class.
//
//////////////////////////////////////////////////////////////////////
#include "parafunc.h"		
#include "DaeFunc.h"
#include "dbfunc.h"			

#include "HmiApp.h"
#include "LcdApp.h"
#include "relay_defs.h"
//@#include "..\AutoCode\Lcdpara.h"
extern unsigned char *HZK16;//[];


#ifndef 	__PL_WIN_SIMULATION__

BEGIN_MESSAGE_MAP(CHmiApp,CApp)
	ON_SM_TIMEOUT(CHmiApp)
	ON_SM_KEYDOWN(CHmiApp)
	ON_SM_RELAYEVENT(CHmiApp)
	ON_SM_AUTOADJUSTECHO(CHmiApp)
//	ON_SM_AUTOADJUSTECHO(CHmiApp)
END_MESSAGE_MAP()

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiMsgBox g_MsgBox; //消息提示窗口
CHmiMsgBox g_MsgBox2; //消息提示窗口

CHmiPasswordBox g_PssBox; //密码窗口
CHmiPassChgBox g_PssChgBox; //密码设定窗口
CHmiLcdContrastBox g_ContrastBox; //对比度修改窗口

CHmiInputDlg g_BIDlg; //开入显示窗口
CHmiAIDlg g_AIDlg;  //AI显示窗口，可以显示一次二次遥测， 一次二次保护测量值
CHmiLangDlg g_LangDlg; //显示多语言选择对话框
CHmiDlgSerialSet g_SerialSet;
CHmiDlgSysStatus g_SysStatus;
CHmiDlgWComStatus g_ComStatusDlg;
CHmiDlgAddressSetting g_ModifyIP;
CLcdWndDefaultPara g_DefaultPara;
CLcdWndversion g_Version;
CLcdWndAutoJustStep2 g_WndAutoJustStep2;
CLcdWndAutoJustStep1 g_WndAutoJustStep1;
CLcdWndPortPara 	g_WndPortPara;
CLcdWndETHportModity g_WndPortProtocol;






CHmiWnd* CHmiApp::m_pActiveWnd = NULL;
CHmiWnd* CHmiApp::m_pInitWnd = NULL; //初始化创
CHmiWnd* CHmiApp::m_pRunWnd = NULL; //运行信息窗口
CHmiWnd* CHmiApp::m_pMainWnd = NULL; //主菜单窗口

CHmiWnd** CHmiApp::m_ppWndList = NULL;
LONG CHmiApp::m_nWinNum = 0;

float CHmiApp::m_fCurSetting[RS_COUNT+1];
float CHmiApp::m_fNewSetting[RS_COUNT+1];

BYTE CHmiApp::m_byCurCfg[CFG_COUNT+1];
BYTE CHmiApp::m_byNewCfg[CFG_COUNT+1];

BYTE CHmiApp::m_byCurSoftSwitch[SW_COUNT+1];
BYTE CHmiApp::m_byNewSoftSwitch[SW_COUNT+1];

LONG* CHmiApp::m_npCFList=NULL;  //缓存用户输入的系数
BOOL CHmiApp::m_bCFListModified = FALSE;
WORD* CHmiApp::m_wpDIFilterTime=NULL; //缓存用户输入的遥信去抖时间
BOOL CHmiApp::m_bDIFilterTimeModified = FALSE;

WORD* CHmiApp::m_wpDIClass=NULL; //DI属性，
BOOL CHmiApp::m_bDIClassModified=FALSE;

WORD* CHmiApp::m_wpBOTime=NULL;	//缓存用户输入的遥控保持时间
BOOL CHmiApp::m_bBOTimeModified=FALSE;

BOOL CHmiApp::m_bCIBaseModified=FALSE;
DWORD* CHmiApp::m_dwpCIBase=NULL;

DWORD* CHmiApp::m_dwpCIConst=NULL; //脉冲常数
BOOL CHmiApp::m_bCIConstModified=FALSE;



WORD* CHmiApp::m_wpBIControl=NULL; // BI是否取反 ， 是wControl项的D2位
BOOL CHmiApp::m_bBIControlModified=FALSE;

WORD* CHmiApp::m_wpPortPara = NULL; //端口规约参数
BOOL CHmiApp::m_bPortParaModified = FALSE;

WORD* CHmiApp::m_wpPortProtocl = NULL; //端口规约参数

WORD* CHmiApp::m_wpVisibleSetList=NULL;

float* CHmiApp::m_fpSysSetList=NULL; //保存辅助定值
BOOL CHmiApp::m_bSysSetListModified = FALSE;

CTranslator CHmiApp::m_Trans;

//液晶对比度
WORD g_wLCDContrast;
//extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
BOOL g_bDisplayEEpromButton;

#ifdef __PL_WIN_SIMULATION__
#define BASE_PARA
#else
#define BASE_PARA :CApp(pAppId)
#endif

IMPLEMENT_HMI_TR(CHmiApp)

CHmiApp::CHmiApp(DWORD *pAppId) BASE_PARA
{
	m_bIsLightOff = FALSE;
	m_nIdelTime = 0;
	m_dwKeyTime = 0;
//	m_wContrastCheckTime = 0;
	
	//组合键被按下的时间
	m_dwCombKeyTimer = 0;
}

CHmiApp::~CHmiApp()
{

}

/*************************************************************************
* 改动历史纪录：

Revision 1.0, 2008-3-3, kangqian
describe: 初始创建.
*************************************************************************/
#ifdef __PL_WIN_SIMULATION__
//在windows中模拟运行时调用该函数
void  CHmiApp::RunApp()
{
	BOOL res;
	LONG nMsgCode, nParam1, nParam2;
	BOOL bExit;
	while(1)
	{
//		m_pActiveWnd->Show(0,0);
		bExit = false;
		while(!bExit)
		{
			res = WaitForMsg(nMsgCode, nParam1, nParam2);
			if(res)
			{

				//是否是定时器消息
				if(nMsgCode == OS_TIMER)
				{	
					OnTimeOut(nParam1);
					continue;
				}

				if(m_bIsLightOff)
				{
					continue;
				}
				//是否是故障弹屏等等不是由具体窗口，而是应用来处理的消息
				
				//窗口消息
				m_nIdelTime = 0;
				m_pActiveWnd->WndProc(nMsgCode, nParam1, nParam2);
			}
		}
	}

}
#endif // #ifdef __PL_WIN_SIMULATION__

void CHmiApp::Init()
{
	//分配空间

	//读取字库到内存
#ifndef __PL_WIN_SIMULATION__
	//如果用文件管理的话是读文件内容到ram中
	DWORD dwFileLen;
	char strFileName[256];

	sprintf(strFileName, "%s", "hzk16.hzk");
	
	STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc == DB_ERR_OK) && (dwFileLen > 0))
	{
		if(dwFileLen>0x40000)
		{
			LogError("DAE_ReadFile",FILE_LINE,"The hzk16.hzk file length error!");
			return;
		}
		
		HZK16 = (unsigned char *)OS_MemAlloc(dwFileLen);
		ASSERT(HZK16 != NULL);
		if(HZK16 == NULL)
		{
			ReportMsg(" The hzk16 buffer error.");
			return;
		}

		rc = DAE_ReadFile(strFileName, HZK16,dwFileLen);
		if(rc != DB_ERR_OK)
		{
			LogError("DAE_ReadFile",FILE_LINE,"The hzk16.hzk Date File fileName=%%s Read Err!",strFileName);
			return;
		}
	}
#endif

	//装载语言文件
//	m_Trans.Load("trLan.bHS");

	//读多语言部分
	g_LanFileMan.Init();


	//系数输入缓冲区
	int nCf = DB_GetCFNum();
	m_npCFList = new LONG[nCf+1];
	memset((void*)m_npCFList, 0, (nCf+1)*sizeof(LONG));

	//直接采用点去抖时间
	int nDI = DB_GetDINum();
	m_wpDIFilterTime = new WORD[nDI+1];
	memset((void*)m_wpDIFilterTime, 0, (nDI+1)*sizeof(WORD));

	int nBO = DB_GetBONum();
	m_wpBOTime = new WORD[nBO+1];
	memset((void*)m_wpBOTime, 0, (nBO+1)*sizeof(WORD));
	
	//脉冲常数
	WORD nCI = DB_GetPulseCINum();
	m_dwpCIConst = new DWORD[nCI+1];
	memset((void*)m_dwpCIConst, 0, (nCI+1)*sizeof(DWORD));

	//电度底数
	int nAllCINum = DB_GetCINum();
	m_dwpCIBase = new DWORD[nAllCINum+1];
	memset((void*)m_dwpCIBase, 0, (nAllCINum+1)*sizeof(DWORD));

	int nBI = DB_GetBINum();
	m_wpBIControl = new WORD[nBI+1];
	memset((void*)m_wpBIControl, 0, (nBI+1)*sizeof(WORD));


	//定值
	int nSet = DB_GetSETNum();
	m_wpVisibleSetList = new WORD[nSet+1]; //可以显示的定值序号列表
	memset((void*)m_wpVisibleSetList, 0, (nSet+1)*sizeof(WORD));

	//端口参数
	m_wpPortPara = new WORD[32];	
	memset((void*)m_wpPortPara, 0, (32)*sizeof(WORD));
	
	//端口号
	m_wpPortProtocl = new WORD[10];	
	memset((void*)m_wpPortProtocl, 0, (10)*sizeof(WORD));

	//辅助定值
	int nSysSet = DB_GetSystemSETNum();
	m_fpSysSetList = new float[nSysSet+1];
	memset((void*)m_fpSysSetList, 0, (nSysSet+1)*sizeof(float));

	//读取eeprom 中保存的液晶对比度
	WORD wVal[2];
	rc = EEPROM_Read(EEPROM_SYSPARA_LCDCTLPAR, 4, wVal);
	if( ((wVal[0]&wVal[1]) == 0) && ((wVal[0]|wVal[1]) == 0xffff) )
		g_wLCDContrast =  wVal[0];
	else
		g_wLCDContrast = HMI_LCDCONTRAST_DEF;
	//ReportMsg("CHmiApp read lcd para vale = 0x%x.",g_wLCDContrast);
	
	//读接线图文件
	m_WndGraph.Load("Graph.grh");
	m_pInitWnd = &m_WndGraph;

	//液晶任务用定时器	
	SetTimer(1,1000);
}
void CHmiApp::OnTimeOut(DWORD id)
{
	if(m_pActiveWnd)
	{
		m_pActiveWnd->OnTimer(id);
	}

	m_nIdelTime++;

	//窗口用定时器
	if(m_nIdelTime == HMI_RUNWND_IDLETIME)
	{
		m_nIdelTime = 0;
		if((m_pRunWnd != NULL)&&(m_pRunWnd != m_pActiveWnd))
		{
			if(m_pActiveWnd)
				m_pActiveWnd->Close();
			m_pRunWnd->Show();
		}
	}

	//定时检查液晶熄屏
	if(!m_bIsLightOff)
	{
		m_dwKeyTime++;
		if(m_dwKeyTime == HMI_LANOFF_IDLETIME)	//超过1分钟无键盘按下，关闭背光
		{
			TurnOffLight();
		}
	}

//	//液晶辉度检测保存
//	m_wContrastCheckTime++;
//	if(m_wContrastCheckTime == HMI_CONTRAST_CHECKTIME)
//	{
//		m_wContrastCheckTime = 0;
		
//		WORD wVal[2],wNotVal;
//		LONG rc = EEPROM_Read(EEPROM_SYSPARA_LCDCTLPAR, 4, wVal);
//		if(wVal[0] != g_wLCDContrast)
//		{
//			wNotVal = ~g_wLCDContrast;
//			EEPROM_Write(EEPROM_SYSPARA_LCDCTLPAR, 2, &g_wLCDContrast);
//			EEPROM_Write(EEPROM_SYSPARA_LCDCTLPAR+2, 2, &wNotVal);
			//ReportMsg("g_wLCDContrast = 0x%x.", g_wLCDContrast);
//		}
//	}
	m_dwCombKeyTimer++;
	if(m_dwCombKeyTimer > 10)
	{
		g_bDisplayEEpromButton = FALSE;
	}
}

void CHmiApp::TurnOnLight()
{
	//防止电磁兼容试验过程中液晶被打死机。
	if(m_bIsLightOff == TRUE)
		LcdCtlerInit();
	
	m_bIsLightOff = FALSE;
	LcdBackLightOn();
	
}
void CHmiApp::TurnOffLight()
{
	m_bIsLightOff = TRUE;
	LcdBackLightOff();
}
void CHmiApp::OnKeyMsg(long nMsgCode, long nParam1, long nParam2)
{
	m_nIdelTime = 0;
	if(m_pActiveWnd)
		m_pActiveWnd->WndProc(nMsgCode, nParam1, nParam2);
}

void CHmiApp::OnKeyDown(DWORD key, DWORD par )
{
	m_nIdelTime = 0;

	//点亮背光
	m_dwKeyTime = 0;
	TurnOnLight();
	
	//键盘处理
	m_pActiveWnd->TransKeyMesssage((WORD)key);

	//增加组合键处理
	if(key == KEY_COMB)
	{
		m_dwCombKeyTimer = 0;
		g_bDisplayEEpromButton = TRUE;
	}
	
	if(key == KEY_LCDC)
	{
			HMIContrastBox((CHmiWnd*)&(CLcdApp::m_WndMainMenu),(CHmiWnd*)&(CLcdApp::m_WndMainMenu));
	}
}

void CHmiApp::OnRelayEvent(DWORD MsgCode, DWORD wParam)
{
	//有报告的时刻开背光 
	m_dwKeyTime = 0;
	TurnOnLight();
	switch(MsgCode)
	{
		//启动
	case 0:
//		if(m_StartDetailDlg.ReadReport(wParam,0))
//			m_StartDetailDlg.DoModal(m_pMainWnd);
		break;
		//动作
	case 1:
		
		if(m_ActionDetailDlg.ReadReport(wParam, 0, 0))
			m_ActionDetailDlg.DoModal(m_pMainWnd);
		break;
		//告警
	case 2:
		if(m_AlarmDetailDlg.ReadReport(wParam,0))
			m_AlarmDetailDlg.DoModal(m_pMainWnd);

		break;
	}
}

//
void CHmiApp::OnAutoAdjustEcho(DWORD STATUS, DWORD par)
{
	if(par == FULLVALUE)
	{
		char strCap[64], strMsg[64];
		
		if(STATUS == SUCCESS)
		{  
			sprintf(strCap, "%s", tr("系数自动整定"));
			sprintf(strMsg, "%s", tr("系数整定成功"));		
		}
		else
		{
			sprintf(strCap, "%s", tr("系数自动整定"));
			sprintf(strMsg, "%s", tr("系数整定失败"));		
		}		
		HMIMsgBox(strCap,  strMsg, &(CLcdApp::m_WndfactorySet), &(CLcdApp::m_WndfactorySet));
	}
	if(par == ZERODRIFT)
	{
		char strMsg[64], strMsg2[64], strCap[64], strCap2[64];
		
		if(STATUS == SUCCESS)
		{  
			g_WndAutoJustStep2.Show();		
		}
		else
		{
			sprintf(strCap, "%s", tr("系数自动整定"));
			sprintf(strMsg, "%s", tr("系数整定失败"));
			HMIMsgBox(strCap,  strMsg, &(CLcdApp::m_WndfactorySet), &(CLcdApp::m_WndfactorySet));		
		}
	}
}

//系数
/*
void CHmiApp::OnAutoAdjustEcho1(DWORD status, DWORD par )
{
	//status为校准结果，可能为SUCCESS、FAILURE
	char strMsg[64], strCap[64];

	if(status == SUCCESS)
	{  
		CLcdApp::m_WndAutoJustSetp2.Show();

	}
	else
	{
		sprintf(strCap, "%s", tr("系数自动整定"));
		sprintf(strMsg, "%s", tr("系数整定失败"));
		HMIMsgBox(strCap,  strMsg, &(CLcdApp::m_WndTest), &(CLcdApp::m_WndTest));

	}

	
}
*/
void  CHmiApp::CloseAllWindows()
{
	for(int i=0; i<m_nWinNum; i++)
	{
		
		m_ppWndList[i]->Close();
	}
}

///////////////////////全局函数///////////////////////////
/****************************************************************************************
 * 功能描述: 显示密码更改对话框
 * 参数说明: 
 *			- char *szCaption1 [in] 对话框标题
 *			- CHmiWnd *pPassOkWnd [in]
 *			- CHmiWnd *pCancelWnd [in]
 *			- LONG nPassType =0 [in] 0 :普通密码 1:管理员密码
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
void HMIChgPassBox(const char *szCaption1, CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType)
{
	g_PssChgBox.DoModal(szCaption1, pPassOkWnd,  pCancelWnd,  nPassType);
}

/****************************************************************************************
 * 功能描述: 显示密码对话框
 * 参数说明: 
 *			- char *szCaption [in] 标题
 *			
 *			- char* szMsg [in] 显示消息
 *			
 *			- CHmiWnd *pPassOkWnd [in] 密码正确跳转窗口
 *			- CHmiWnd *pCancelWnd [in] 取消跳转窗口
 *			- LONG nPassType =0 [in] 密码类型 0：普通密码 1:管理员密码
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
void HMIPasswordBox(const char *szCaption1, const char* szMsg1 
					, CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType )
{
	g_PssBox.DoModal(szCaption1,  szMsg1,  pPassOkWnd, pCancelWnd, nPassType);


}

/****************************************************************************************
 * 功能描述: 显示消息对话框
 * 参数说明: 
 *			- char *szCaption1 [in] 对话框标题
 *			- char* szMsg1 [in] 显示消息, 一行显示不下时，可以使用"\n"换行显示
 *			- CHmiWnd *pOkWnd [in] ok按钮调转窗口
 *			- CHmiWnd *pCancelWnd [in] cancel按钮跳转窗口
 *			- LONG nType = HMI_MB_OK [in] 
 *				   HMI_MB_OK 显示ok按钮  HMI_MB_OKCANCEL ok和cancel按钮都显示
 *			- YES_NO_PTR pYES_Fn=NULL [in] ok按钮响应函数
 *			- YES_NO_PTR pNO_Fn=NULL [in] cancel按钮响应函数
 * 返回代码: 
 * 其它: 注意， 消息对话框的按钮响应函数有特殊处理，如果没有按钮响应函数则跳转窗口有效，
 *		如果有按钮响应函数，则跳转窗口无效(这样可以保证跳转到哪个窗口完全由处理函数控制 )	
 *****************************************************************************************/

void HMIMsgBox(const char *szCaption,const char* szMsg, CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, LONG nType , YES_NO_PTR pYES_Fn, YES_NO_PTR pNO_Fn)
{
	g_MsgBox.DoModal(szCaption,  szMsg,  pOkWnd, pCancelWnd,  nType, pYES_Fn,  pNO_Fn);
}

//显示液晶对比度对话框
void HMIContrastBox(CHmiWnd* pOkWnd, CHmiWnd* pCancelWnd)
{
	g_ContrastBox.DoModal(pOkWnd, pCancelWnd);
}


/****************************************************************************************
 * 功能描述: 更新可显示定值列表
 * 参数说明: 
 *			- long nAreaNo [in] 要显示的定值区号
 *			- LONG* nCount [out] 可显示的定值个数
 *			- WORD* SetList [out] 可显示定值序号列表
 *			- BOOL bMode [in] 更新定值列表的原因:TRUE,定值查询；FALSE，定值修改
 * 返回代码: 
 * 其它: 增加参数bMode,装置在启动后查询定值时m_byNewCfg内容为空导致无法显示已投入的定值
 *****************************************************************************************/
BOOL HMIUpDataVisibleSetList(long nAreaNo, LONG* nCount, WORD* SetList,BOOL bMode)
{

	//只显示配置投入了的定值
	//读关联列表不成功的 显示
	//没有关联配置的 显示
	// 关联了配置的，遍历关联列表，检查配置是否有投入，有投入的显示
	BYTE CFGStatus;
	BYTE nCFGIndex[SETCFG_MAXNUM];
	LONG rc;

	WORD cfgNum = DB_GetCFGNum();
	WORD nSet = DB_GetSETNum();
	LONG cfgWordNum = cfgNum/32;
	if(cfgNum%32)
	{
		cfgWordNum += 1;
	}

	int j,k=0;
	BOOL  bNoERR = TRUE;
	TSET_Disp tSETDisp;
	LONG nCfgCount;


	//对每个定值， 得到关联配置列表, 关联配置数为0的定值序号加入显示列表
	//遍历读取关联配置的值，只要有一个投入，则该定值序号加入显示列表
	int i, m;
	for(i=cfgWordNum; i<nSet; i++)
	{
		rc = DB_GetSETDisp(i, &tSETDisp);
		if(DB_ERR_OK != rc)
		{
			//读数据库错误，无法得到关联列表，显示该定值
			SetList[k++] = i;
			bNoERR = FALSE;
			continue;
		}
			

		//得到定值关联的配置信息
		for(LONG m=0;m<SETCFG_MAXNUM;m++)
			nCFGIndex[m] = tSETDisp.CFGList[m];
		nCfgCount = tSETDisp.nCFG;

		//不关联配置的定值直接显示
		if(nCfgCount <=0)
		{
			SetList[k++] = i;
			continue;
		}
		

		//检查定值是否需要显示(无关联 或者 投入)
		for(j=0;j<nCfgCount;j++)
		{
			if(nCFGIndex[j] == SETCFG_INVALID)
				continue;
			if(bMode == TRUE)
			{
				rc = DB_GetCFGStatus(nAreaNo,nCFGIndex[j], CFGStatus);
			}
			else
			{
				CFGStatus = CHmiApp::m_byNewCfg[nCFGIndex[j]];
			}

			if(CFGStatus == CFG_STATUS_SET) //配置投入则显示定值
			{
				SetList[k++] = i;
				break;
			}
			if(DB_ERR_OK !=rc)
				bNoERR = FALSE;
		}
		
	}

	*nCount = k;
	return bNoERR;
		
}

//读系数数据到缓冲区，成功返回true,失败返回false
BOOL HMIReadCFToBuf()
{
	//从数据库读取系数的缓冲区
	LONG rc = DB_GetCFValue(CF_VALUE_ALL, CHmiApp::m_npCFList);
	if(DB_ERR_OK != rc)
		return FALSE;
	CHmiApp::m_bCFListModified = FALSE;
	return TRUE;
}

//保存缓冲区的系数数据到数据库
//成功返回true，读取失败返回false
BOOL HMISaveCFToDB()
{
	LONG rc = DB_SetCFValue(CF_VALUE_ALL, CHmiApp::m_npCFList);
	if(DB_ERR_OK != rc)
		return FALSE;
	rc = DB_SetCFValueConfirm(CF_VALUE_ALL, DB_EXECUTE);
	if(DB_ERR_OK != rc)
		return FALSE;

	CHmiApp::m_bCFListModified = FALSE;
	return TRUE;
}
//从数据库读取遥控保持时间到缓冲区
//成功返回true，失败返回false
BOOL HMIReadBOTimeToBuf()
{
	TBOPara BOPara;
	LONG rc;
	WORD num = DB_GetBONum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetBOPara(i, &BOPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		
		CLcdApp::m_wpBOTime[i] = BOPara.wLastTime;
	}
	CHmiApp::m_bBOTimeModified = FALSE;
	return TRUE;
	
}
//保存遥控保持时缓冲区中的数据到数据库
//保存成功返回true，保存失败返回false
BOOL HMISaveBOTimeToDB()
{
	TBOPara tBOPara;
	LONG rc;
	
	WORD num = DB_GetBONum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetBOPara(i, &tBOPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		
		if (tBOPara.wLastTime!= CLcdApp::m_wpBOTime[i])
		{
			tBOPara.wLastTime = CLcdApp::m_wpBOTime[i];
			rc =  DB_WriteBOPara(i, &tBOPara);
			rc = DB_WriteBOParaConfirm(i);
			
			if(DB_ERR_OK != rc)
				return FALSE;
		}
	}
	CHmiApp::m_bBOTimeModified = FALSE;
	return TRUE;
}
//从数据库读取开入去抖时间到缓冲区
//读取成功返回true，读取失败返回false
BOOL HMIReadDIFilterTimeToBuf()
{
	LONG rc;
	//初始化DiFilterTime的缓冲区
	TDIPara DIPara;
	WORD num = DB_GetBINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetDIPara(i, &DIPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		CLcdApp::m_wpDIFilterTime[i] = DIPara.wFilterTime;
	}
	CHmiApp::m_bDIFilterTimeModified = FALSE;
	return TRUE;
	
}

//保存缓冲区的开入去抖时间数据到数据库
//成功返回true，读取失败返回false
BOOL HMISaveDIFilterTimeToDB()
{
	TDIPara tDIPara;
	LONG rc;
	
	WORD num = DB_GetBINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetDIPara(i, &tDIPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		if (tDIPara.wFilterTime != CLcdApp::m_wpDIFilterTime[i])
		{
			tDIPara.wFilterTime = CLcdApp::m_wpDIFilterTime[i];
			rc = DB_WriteDIPara(i, &tDIPara);
			rc = DB_WriteDIParaConfirm(i);
			if(DB_ERR_OK != rc)
				return FALSE;
		}
	}
	CHmiApp::m_bDIFilterTimeModified = FALSE;
	return TRUE;
}

//从数据库读取开入属性到缓冲区
//读取成功返回true，读取失败返回false
BOOL HMIReadDIClassToBuf()
{
	
	LONG rc;
	//初始化DiFilterTime的缓冲区
	TDIPara DIPara;
	WORD num = DB_GetBINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetDIPara(i, &DIPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		CLcdApp::m_wpDIClass[i] = DIPara.wClass;
	}
	CHmiApp::m_bDIClassModified = FALSE;
	return TRUE;
		
}
//保存缓冲区的开入属性数据到数据库
//成功返回true，读取失败返回false
BOOL HMISaveDIClassToBuf()
{
	TDIPara tDIPara;
	LONG rc;
	
	WORD num = DB_GetBINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetDIPara(i, &tDIPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		if (tDIPara.wClass != CLcdApp::m_wpDIClass[i])
		{
			tDIPara.wClass = CLcdApp::m_wpDIClass[i];
			rc = DB_WriteDIPara(i, &tDIPara);
			rc = DB_WriteDIParaConfirm(i);
			if(DB_ERR_OK != rc)
				return FALSE;
		}
	}
	CHmiApp::m_bDIClassModified = FALSE;
	return TRUE;
}

//从数据库读取电度底数到缓冲区
//读取成功返回true，读取失败返回false
BOOL HMIReadCIBaseToBuf()
{
	
	WORD num = DB_GetCINum();
	LONG rc;
	for(WORD i=0; i<num;i++)
	{
//		rc = DB_ReadCIRaw(i, &(CHmiApp::m_dwpCIBase[i]));
		rc = DB_ReadCI(i, &(CHmiApp::m_dwpCIBase[i]));
		if (rc != DB_ERR_OK)
		{
			return FALSE;
		}
	}
	CHmiApp::m_bCIBaseModified = FALSE;
	return TRUE;
}

//保存缓冲区的电度底数数据到数据库
//成功返回true，读取失败返回false
BOOL HMISaveCIBaseToDB()
{
	WORD num = DB_GetCINum();
	LONG rc;
	for(WORD i=0; i<num;i++)
	{
		rc = DB_SetCI(i, CHmiApp::m_dwpCIBase[i]);
		
		if (rc != DB_ERR_OK)
		{
			return FALSE;
		}
	}
	CHmiApp::m_bCIBaseModified = FALSE;
	return TRUE;
	
}
//注意，积分电度无脉冲常数
BOOL HMISaveCIPluseToDB()
{
	TCIPara tCIPara;
	LONG rc;
	WORD num = DB_GetPulseCINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetCIPara(i+INTERGRAL_KWH_COUNT, &tCIPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		if(tCIPara.dwConst != CHmiApp::m_dwpCIConst[i])
		{
			tCIPara.dwConst = CHmiApp::m_dwpCIConst[i];
			rc = DB_WriteCIPara(i+INTERGRAL_KWH_COUNT, &tCIPara);
			if(DB_ERR_OK != rc)
				return FALSE;
			
			rc = DB_WriteCIParaConfirm(i+INTERGRAL_KWH_COUNT);
			if(DB_ERR_OK != rc)
				return FALSE;
		}
	}
	CHmiApp::m_bCIConstModified = FALSE;
	return TRUE;
}
//从数据库读取脉冲常数到缓冲区
//读取成功返回true，读取失败返回false
BOOL HMIReadCIPluseToBuf()
{
	TCIPara tCIPara;
	LONG rc;
	WORD num = DB_GetPulseCINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetCIPara(i+INTERGRAL_KWH_COUNT, &tCIPara);
		if(DB_ERR_OK != rc)
			return FALSE;
		CHmiApp::m_dwpCIConst[i] = tCIPara.dwConst;
	}
	CHmiApp::m_bCIConstModified = FALSE;
	return TRUE;
}

//从数据库读取遥信属性到缓冲区
//读取成功返回true，读取失败返回false
BOOL HMIReadBIControlToBuf()
{
	LONG rc;
	TBIPara tBIPara;
	WORD num = DB_GetBINum();
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		if (rc != DB_ERR_OK)
		{
			return FALSE;
		}
		CHmiApp::m_wpBIControl[i] = tBIPara.wControl;
	}
	CHmiApp::m_bBIControlModified = FALSE;
	return TRUE;
	
}
//保存缓冲区的遥信参数数据到数据库
//成功返回true，读取失败返回false
BOOL HMISaveBIControlToDB()
{
	TBIPara tBIPara;
	WORD num = DB_GetBINum();
	LONG rc;
	for (WORD i=0; i<num; i++)
	{
		rc = DB_GetBIPara(i, &tBIPara);
		
		if (rc != DB_ERR_OK)
		{
			return FALSE;
		}
		if (tBIPara.wControl != CHmiApp::m_wpBIControl[i])
		{
			tBIPara.wControl = CHmiApp::m_wpBIControl[i];
			rc =DB_WriteBIPara(i, &tBIPara);
			rc = DB_WriteBIParaConfirm(i);
			if (rc != DB_ERR_OK)
			{
				return FALSE;
			}
			
		}
	}
	CHmiApp::m_bBIControlModified = FALSE;
	return TRUE;
	
}

//从数据库读取辅助定值到缓冲区
//读取成功返回true，读取失败返回false
BOOL HMIReadSysSetToBuf()
{
	//读取辅助定值到缓冲区
	
	CHmiApp::m_bSysSetListModified = FALSE;
	LONG rc = DB_GetSystemSETFloatValue(SET_VALUE_ALL, CLcdApp::m_fpSysSetList);
	if (rc != DB_ERR_OK)
	{
		return FALSE;
	}
	return TRUE;

}
//保存缓冲区的数据到数据库
//成功返回true，读取失败返回false
BOOL HMISaveSysSetToDB()
{
	LONG rc;
	rc = DB_SetSystemSETValue(SET_VALUE_ALL, CLcdApp::m_fpSysSetList);
	
	if (rc != DB_ERR_OK)
	{
		return FALSE;
	}
	rc = DB_SetSystemSETValueConfirm(SET_VALUE_ALL, DB_EXECUTE);
	
	if (rc != DB_ERR_OK)
	{
		return FALSE;
	}
	CHmiApp::m_bSysSetListModified = FALSE;
	return TRUE;
}

BOOL HMISaveSWToDB()
{
	LONG num = DB_GetSWNum();
	WORD i;
	LONG rc;

	for(i=0; i<num; i++)
	{
		if(CHmiApp::m_byCurSoftSwitch[i] != CHmiApp::m_byNewSoftSwitch[i])
		{
			rc = DB_SetSWStatus(i, CHmiApp::m_byNewSoftSwitch[i]);
			
			if (rc != DB_ERR_OK)
			{
				return FALSE;
			}
			rc = DB_SetSWStatusConfirm(i, DB_EXECUTE);
			
			if (rc != DB_ERR_OK)
			{
				return FALSE;
			}
			CHmiApp::m_byCurSoftSwitch[i] = CHmiApp::m_byNewSoftSwitch[i];
		}
	}
	return TRUE;
}
BOOL HMIReadSWToBuf()
{
	//读取现在的软压板缓存起来
	WORD num = DB_GetSWNum();
	LONG rc;
	if(num > sizeof(CHmiApp::m_byCurSoftSwitch))
	{
		num = sizeof(CHmiApp::m_byCurSoftSwitch);
		LogError("HMIReadSWToBuf", FILE_LINE, "softswitch num overflow");
	}
	WORD i; 
	for(i=0; i<num; i++)
	{
		rc = DB_GetSWStatus(i, CHmiApp::m_byCurSoftSwitch[i]);
		
		if (rc != DB_ERR_OK)
		{
			return FALSE;
		}
		rc = DB_GetSWStatus(i, CHmiApp::m_byNewSoftSwitch[i]);
		
		if (rc != DB_ERR_OK)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL HMIReadSetToBuf(WORD nAreaNo)
{
	
	LONG rc;
	//读定值和配置到缓冲区
	rc = DB_GetSETFloatValue(nAreaNo, SET_VALUE_ALL, CHmiApp::m_fCurSetting);
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	memcpy(CHmiApp::m_fNewSetting, CHmiApp::m_fCurSetting, RS_COUNT*sizeof(float));
	return TRUE;
	
}
BOOL HMISaveSetToDB(WORD nAreaNo)
{
	LONG rc;
	
	rc = DB_SetSETValue(nAreaNo,SET_VALUE_ALL, CHmiApp::m_fNewSetting);
	if(DB_ERR_OK == rc)
	{
		rc = DB_SetSETValueConfirm(nAreaNo,SET_VALUE_ALL, DB_EXECUTE);
	}
	if(DB_ERR_OK != rc)
		return FALSE;
	else 
		return TRUE;
	
}
BOOL HMIReadCFGToBuf(WORD nAreaNo)
{
	
	WORD wCfgNum = DB_GetCFGNum();
	LONG rc;
	int i;

	
	for(i=0; i<wCfgNum; i++)
	{
		rc = DB_GetCFGStatus(nAreaNo,i, CHmiApp::m_byCurCfg[i]);
		
		if(DB_ERR_OK != rc)
		{
			return FALSE;
		}
	}
	
	memcpy(CHmiApp::m_byNewCfg, CHmiApp::m_byCurCfg, CFG_COUNT);
	return TRUE;
}
BOOL HMISaveCFGToBuf(WORD nAreaNo)
{
	return TRUE;
}
BOOL HMIReadPortParaToBuf()
{	
	WORD num,sernum,cannum,ethnum;
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();
	num  = sernum+cannum+ethnum;
	WORD sn=0;

	if(sernum)
	{
		TSerialPara tSerial;
		for(LONG j=0;j<sernum;j++)
		{
			if(DB_ERR_OK != DB_GetSerialPara(j,&tSerial))
				return FALSE;
			if(tSerial.byDriver>8||tSerial.byDriver<0)
				CHmiApp::m_wpPortPara[sn] = 0;
			else
			{
//				dd = tSerial.byDriver;
				CHmiApp::m_wpPortPara[sn] = tSerial.byDriver;
//				tt = CHmiApp::m_wpPortPara[sn];
			}
			sn++;
		}
	}
	if(cannum)
	{		
		TCanBusPara tCan;
		for(LONG m=0;m<cannum;m++)
		{
			if(DB_ERR_OK != DB_GetCANPara(m,&tCan))
				return FALSE;
			if(tCan.byDriver>8||tCan.byDriver<0)
				CHmiApp::m_wpPortPara[sn] = 0;
			else
				CHmiApp::m_wpPortPara[sn] = tCan.byDriver;			
			sn++;
		}
	}
	if(ethnum)
	{
		TEthPortPara tEth;
		for(LONG n=0;n<ethnum;n++)
		{
			if(DB_ERR_OK != DB_GetETHPara(n,&tEth))
				return FALSE;
			if(tEth.wDriver>8||tEth.wDriver<0)
				CHmiApp::m_wpPortPara[sn] = 0;
			else
			{
				if((tEth.wPortID&0xF00)==0x300)
					CHmiApp::m_wpPortPara[sn] = tEth.wDriver;			
				if((tEth.wPortID&0xF00)==0x400)
					CHmiApp::m_wpPortPara[sn] = tEth.wDriver;			
			}
			sn++;
		}
	}
	return TRUE;
}
BOOL HMISavePortParaToDB()
{
	WORD num,sernum,cannum,ethnum;
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();
	num  = sernum+cannum+ethnum;
	WORD sn=0;
	if(sernum)
	{
		TSerialPara tSerial;
		for(LONG j=0;j<sernum;j++)
		{
			if(DB_ERR_OK != DB_GetSerialPara(j,&tSerial))
				return FALSE;

			if(tSerial.byDriver != CHmiApp::m_wpPortPara[sn])
			{
				tSerial.byDriver = CHmiApp::m_wpPortPara[sn];
				DB_WriteSerialPara(j,&tSerial);
				DB_WriteSerialParaConfirm(j);
			}
			sn++;
		}
	}
	if(cannum)
	{
		TCanBusPara tCan;
		for(LONG m=0;m<cannum;m++)
		{
			if(DB_ERR_OK != DB_GetCANPara(m,&tCan))
				return FALSE;
			if(tCan.byDriver != CHmiApp::m_wpPortPara[sn])
			{
				tCan.byDriver = CHmiApp::m_wpPortPara[sn];
				DB_WriteCANPara(m,&tCan);
				DB_WriteCANParaConfirm(m);
			}
			sn++;
		}
	}
	if(ethnum)
	{
		TEthPortPara tEth;
		for(LONG n=0;n<ethnum;n++)
		{
			if(DB_ERR_OK != DB_GetETHPara(n,&tEth))
				return FALSE;

			if(tEth.wDriver != CHmiApp::m_wpPortPara[sn])
			{
				tEth.wDriver = CHmiApp::m_wpPortPara[sn];
				DB_WriteETHPara(n,&tEth);
				DB_WriteETHParaConfirm(n);
			}	
			sn++;
		}
	}
	return TRUE;
}

