/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiApp.h
* 
*   软件模块：界面控件
*
* 
*	描述	界面应用基类， 液晶应用从该类继承
* 
* 
* 
*   函数	Init() 初始化函数，显示初始窗口， 子类的初始化函数需要调用该函数
*			OnTimerOut(DWORD id) 定时器消息响应函数					
*			OnKeyMsg()	键盘消息响应函数
*			RunApp()	在window上使用的模拟运行函数， 目标板上不使用该函数	
*			HMIMsgBox 	消息框
*			HMIPasswordBox 密码输入框
*			HMIChgPassBox 密码修改框		
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-4-21		初始创建
* 
*************************************************************************/

#ifndef __HmiApp_H
#define __HmiApp_H

#ifndef __PL_WIN_SIMULATION__
#include "system.h"
#include "msgmap.h"
#endif

#include "Translator.h"
#include "LcdDef.h"
#include "LcdCfg.h"
#include "relaydef.h"
// HmiApp.h: interface for the CHmiApp class.
//
//////////////////////////////////////////////////////////////////////
#include "HmiWnd.h"
#include "hmiObj.h"
#include "HmiButton.h"
#include "HmiEdit.h"
#include "HmiStatic.h"
#include "HmiList.h"
#include "HmiMsgBox.h"
#include "hmiDataList.h"
#include "Translator.h"
#include "HmiGraphWnd.h"

#define HMI_LANOFF_IDLETIME 600  //熄灭背光的空闲时间间隔
#define HMI_RUNWND_IDLETIME 60  //显示运行信息的空闲时间间隔
#define HMI_CONTRAST_CHECKTIME 60 //液晶辉度检测存盘间隔时间

#ifdef __PL_WIN_SIMULATION__
#define APPBASE 
#else
#define APPBASE  :public CApp
#endif


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
void HMIPasswordBox(const char *szCaption, const char* szMsg,  CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);

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
void HMIMsgBox(const char *szCaption,const char* szMsg
			   , CHmiWnd *pOkWnd=NULL, CHmiWnd *pCancelWnd =NULL, LONG nType = HMI_MB_OK
			   , YES_NO_PTR pYES_Fn=NULL, YES_NO_PTR pNO_Fn=NULL);


/****************************************************************************************
 * 功能描述: 显示密码更改对话框
 * 参数说明: 
 *			- char *szCaption1 [in] 对话框标题
 *			- CHmiWnd *pPassOkWnd
 *			- CHmiWnd *pCancelWnd
 *			- LONG nPassType =0  0 :普通密码 1:管理员密码
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
void HMIChgPassBox( const char *szCaption,CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);

//液晶对比度窗口
void HMIContrastBox(CHmiWnd* pOkWnd, CHmiWnd* pCancelWnd);



/****************************************************************************************
 * 功能描述: 更新可显示定值列表
 * 参数说明: 
 *			- long nAreaNo [in] 要显示的定值区号
 *			- LONG* nCount [out] 可显示的定值个数
 *			- WORD* SetList [out] 可显示定值列表 
 *			- BOOL bMode [in] 更新定值列表的原因:TRUE,定值查询；FALSE，定值修改
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
	BOOL HMIUpDataVisibleSetList(long nAreaNo, LONG* nCount, WORD* SetList,BOOL bMode);

/****************************************************************************************
 * 功能描述: 从数据读系数到Hmi缓存区
 * 参数说明: 
 *			
 * 返回代码: true 读成功， false 读失败
 * 其它: 
 *****************************************************************************************/
BOOL HMIReadCFToBuf();
BOOL HMISaveCFToDB();
BOOL HMIReadBOTimeToBuf();
BOOL HMISaveBOTimeToDB();
BOOL HMIReadDIFilterTimeToBuf();
BOOL HMISaveDIFilterTimeToDB();
BOOL HMIReadCIBaseToBuf();
BOOL HMISaveCIBaseToDB();
BOOL HMIReadCIPluseToBuf();

BOOL HMIReadBIControlToBuf();
BOOL HMISaveBIControlToDB();
BOOL HMIReadSetToBuf(WORD nAreaNO);
BOOL HMISaveSetToDB(WORD nAreaNO);
BOOL HMIReadCFGToBuf(WORD nAreaNO);
BOOL HMISaveCFGToDB(WORD nAreaNO);
BOOL HMIReadSysSetToBuf();
BOOL HMISaveSysSetToDB();
BOOL HMISaveCIPluseToDB();
BOOL HMIReadCIPulseToBuf();
BOOL HMIReadSWToBuf();
BOOL HMISaveSWToDB();
BOOL HMIReadPortParaToBuf();
BOOL HMISavePortParaToDB();

class CHmiApp  APPBASE
{

#ifndef __PL_WIN_SIMULATION__
	DECLARE_MESSAGE_MAP()
#endif

	DECLARE_HMI_TR()

public:
	
	//缓存定值和配置修改
	static float m_fCurSetting[RS_COUNT+1]; //防止RS_COUNT为0时错误
	static float m_fNewSetting[RS_COUNT+1];
	static BYTE m_byCurCfg[CFG_COUNT+1];
	static BYTE m_byNewCfg[CFG_COUNT+1];
	
	static BYTE m_byCurSoftSwitch[SW_COUNT+1];
	static BYTE m_byNewSoftSwitch[SW_COUNT+1];

	//下面的是根据数据库参数动态分配空间的
	static LONG* m_npCFList;  //缓存用户输入的系数
	static BOOL m_bCFListModified; //系数缓冲区修改过
	
	static WORD* m_wpDIFilterTime; //缓存用户输入的遥信去抖时间
	static BOOL  m_bDIFilterTimeModified; //去抖时间缓冲区修改标识
	
	static WORD* m_wpDIClass; //缓冲用户输入的DI属性
	static BOOL  m_bDIClassModified; // DI属性修改标志
	
	static WORD* m_wpBOTime; //缓存用户输入的遥控保持时间
	static BOOL m_bBOTimeModified; //遥控保持时间修改标志
	
	
	static DWORD* m_dwpCIConst; //脉冲常数
	static BOOL  m_bCIConstModified;
	
	static DWORD* m_dwpCIBase; //电度底数
	static BOOL m_bCIBaseModified; //电度底数缓冲区修改标识

	
	static WORD* m_wpBIControl; // BI是否取反 ， 是wControl项的D2位
	static BOOL m_bBIControlModified;

	static WORD* m_wpPortPara;   //端口规约参数
	static BOOL m_bPortParaModified;

	
	static WORD* m_wpPortProtocl;   //端口规约参数

	static WORD* m_wpVisibleSetList; 	//保存可显示的定值的列表
	
	static float* m_fpSysSetList; //保存辅助定值
	static BOOL m_bSysSetListModified;

	virtual void Init();
	void OnTimeOut(DWORD id);
	void OnKeyDown(DWORD key, DWORD par );

	void OnKeyMsg(long MsgCode, long wParam, long lParam);
	void OnRelayEvent(DWORD MsgCode, DWORD wParam);
//	void OnAutoAdjustEcho1(DWORD status, DWORD par ); //系数自动校正返回消息响应函数
	void OnAutoAdjustEcho(DWORD status, DWORD par );
	static CHmiWnd* m_pActiveWnd; //活动窗口
	static CHmiWnd* m_pInitWnd; //初始化创
	static CHmiWnd* m_pRunWnd; //运行信息窗口
	static CHmiWnd* m_pMainWnd; //主菜单窗口
	CHmiApp(DWORD *pdwApp);
	virtual ~CHmiApp();
	static CHmiWnd** m_ppWndList;

	COperationDetailDlg  m_OperationDetailDlg;
	CHmiActionDetailDlg m_ActionDetailDlg; //动作报告详情
	CHmiAlarmDetailDlg m_AlarmDetailDlg; //告警报告详情窗口
	CHmiStartDetailDlg m_StartDetailDlg; //启动报告详情窗口
	

	CHmiInputDlg m_BIDlg; //开入显示窗口	
	//2011-4-1 15:36:03
	CHmiGraphWnd m_WndGraph; //主接线图
	static CTranslator m_Trans;


	//在windows 上模拟运行时使用
#ifdef __PL_WIN_SIMULATION__
	void  RunApp();
#endif

	void TurnOnLight();
	void TurnOffLight();


	//关闭所有窗口
	static void CloseAllWindows();
	
	static LONG m_nWinNum; //窗口个数

	inline void SetWinNum(LONG num)
	{
		m_nWinNum = num;
	}
	LONG GetWinNum()
	{
		return m_nWinNum;
	}

protected:
		
	volatile  BOOL m_bIsLightOff; // 是否是关了背光了 
	volatile LONG m_nIdelTime; //空闲时间
	DWORD m_dwKeyTime;		//按键空闲记时
//	WORD m_wContrastCheckTime; //液晶辉度存盘时间 2011-8-25 不需要了，定时存盘不正确
	DWORD m_dwCombKeyTimer; //组合键被按下的时间

};

/**********************/

#endif /* __HmiApp_H */
