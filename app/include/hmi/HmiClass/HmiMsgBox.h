/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiMsgBox.h
* 
*   软件模块：对话框
*
* 
*	描述 CHmiMsgBox：通用消息对话框
*		 CHmiPasswordBox: 密码对话框
*		 CHmiAIDlg： 测量值显示对话框，可显示测量值， 保护测量值， 电度值
*		 CHmiInputDlg： 开入显示对话框， 
*		 CHmiLangDlg: 语言选择对话框
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2009-12-10		初始创建
* 		kangqian	2010-08-22		增加CHmiLangDlg
*************************************************************************/

#ifndef __HmiMsgBox_H
#define __HmiMsgBox_H


#include "HmiWnd.h"
#include "HmiStatic.h"
#include "HmiButton.h"
#include "HmiEdit.h"
#include "HmiDataStatic.h"
#include "HmiDataList.h"
#include "HmiCombo.h"

#define  HMI_MAX_BINUM  128
#define  HMI_MAX_AINUM	128
#define  HMI_MAX_RMNUM	128

#define  HMI_BI_PERLINE 6 //每行可以显示6个开入
#define  HMI_BI_PERPAGE 4 //每页显示4行

#define  HMI_AI_PERPAGE 10 //每页可以显示8个遥测

#define  HMI_AI_MAXDISP 256 //可显示的Ai的最大数目

#define HMI_MB_OK 1
#define HMI_MB_OKCANCEL 2

#define HMI_LCDCONTRAST_LOW 0x06
#define HMI_LCDCONTRAST_HIGH 0x19a
#define HMI_LCDCONTRAST_DEF 0x100
#define HMI_LCDCONTRAST_STEP 2

typedef struct TCOMINFO
{
	BYTE lValue;
	char strName[256];
}TComInfo;

//////////////////////////CHmiLangDlg//////////////////
class CHmiLangDlg : public CHmiWnd
{
public:
	CHmiLangDlg();
	BOOL InitDlg();
	virtual ~CHmiLangDlg();
	void OnEnterBtnLang0();
	void OnEnterBtnLang1();
	void OnEnterBtnLang2();
	void OnEnterBtnLang3();
	void OnEnterBtnLang4();
	void OnEnterBtnLang5();
	void OnEnterBtnLang6();
	void OnEnterBtnLang7();
	
	CHmiButton m_BtnLang0;
	CHmiButton m_BtnLang1;
	CHmiButton m_BtnLang2;
	CHmiButton m_BtnLang3;
	CHmiButton m_BtnLang4;
	CHmiButton m_BtnLang5;
	CHmiButton m_BtnLang6;
	CHmiButton m_BtnLang7;
	void DoModal(CHmiWnd* pCancelWnd);

	CHmiObj* m_pObjList[9];
	DECLARE_HMIMESSAGE_MAP() 
};




///////////////////////////////////////////////
class CHmiLcdContrastBox : public CHmiWnd
{
public:
	CHmiLcdContrastBox();
	BOOL InitDlg();
	virtual ~CHmiLcdContrastBox();

	
	void DoModal(CHmiWnd* pOkWnd, CHmiWnd* pCancelWnd);
//	void OnEnterBtnOk();
//	void OnEnterBtnCancel();
//	CHmiButton m_BtnOk;
//	CHmiButton m_BtnCancel;
	void OnWndEnter();


	CHmiDataStatic m_StaticMsg;
	CHmiDataStatic m_Static5334;
	CHmiDataStatic m_Static5335;
	CHmiObj* m_pObjList[4];
	BOOL OnAdd();
	BOOL OnSub();
	void OnPaint();
	int m_nX0, m_nY0, m_nX1, m_nY1;
	
	int m_nStep;
	int m_nContrastValue;
	
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndLcdContrastBox)
//手动添加的成员变量放在括号之间
//}}end of user function and memeber(CLcdWndLcdContrastBox)
};

class CHmiMsgBox : public CHmiWnd  
{
	CHmiStatic m_Stmsg; 
	CHmiButton m_BtnOk;
	CHmiButton m_BtnCancel;
	CHmiObj* m_pObjList[4]; //控件指针列表，要比控件多一个 m_pObjList[0]是空的
							//原因是如果窗口没有任何控件m_pObjList的定义仍然是有效的

public:
	CHmiMsgBox();
	void DoModal(const char *szCaption1,	const char* szMsg1, CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, 
							 LONG nType, YES_NO_PTR pYES_Fn=NULL, YES_NO_PTR pNO_Fn=NULL);
	YES_NO_PTR m_pFnYes;	//YES_NO响应函数 
	YES_NO_PTR m_pFnNo; 
	void OnEnterBtnOk();
	void OnEnterBtnCancel();
	BOOL InitDlg();

	virtual ~CHmiMsgBox();
	DECLARE_HMIMESSAGE_MAP() 
	//防止外部调用Show函数,确保只能通过DoModal来显示
private: 
	 void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
};
///////////////////////////////////////////////
//密码对话框， 为了减少焦点移动操作， 不增加确认和取消按钮， 直接使用enter 键和esc键
class CHmiPasswordBox : public CHmiWnd
{
	
	CHmiStatic m_StaticMsg;
	CHmiStatic m_StaticMsg2;
	
	CHmiStatic m_StaticDynamic;
	CHmiStatic m_DynamicValue;
	CHmiObj* m_pObjList[6];
	int m_nType;// 密码类型
	void OnWndEnter();
	BOOL OnWndInit();
	CHmiEdit m_EditPass;
	BOOL InitDlg();
	CHmiWnd *m_pOKWnd; // 确认且密码正确跳转窗口
	
	DWORD value;	
	char Dynamic[8];
	BOOL b_dpasswordOK;
	BOOL b_upasswordOK;
public:
	CHmiPasswordBox();
	virtual ~CHmiPasswordBox();
	void DoModal(const char *szCaption1, const char* szMsg1,  CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);
	DECLARE_HMIMESSAGE_MAP() 
	//防止外部调用Show函数,确保只能通过DoModal来显示
private: 
	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
		
};

//密码设置对话框
///////////////////////////////////////////////
class CHmiPassChgBox : public CHmiWnd
{
	BOOL InitDlg();
	void OnWndEnter();
	BOOL OnWndInit();
	CHmiEdit m_EditOldPass;
	CHmiEdit m_EditNewPass;
	CHmiStatic m_Static4836;
	CHmiStatic m_Static4839;
	CHmiStatic m_StaticDynamic;
	CHmiStatic m_DynamicValue;
	CHmiStatic m_StaticMsg;
	
	char Dynamic[8];
	DWORD value;
	CHmiObj* m_pObjList[8];
	CHmiWnd *m_pOKWnd; // 确认且密码正确跳转窗口
	
	int m_nType;
public:
	CHmiPassChgBox();
	virtual ~CHmiPassChgBox();
	void DoModal(const char *szCaption1,   CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);
	DECLARE_HMIMESSAGE_MAP() 
		//防止外部调用Show函数,确保只能通过DoModal来显示
private: 
	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};

};



///////////////////////////////////////////////
////开入显示窗口
class CHmiInputDlg : public CHmiWnd
{
public:
	CHmiInputDlg();
	//重载绘制函数
	void OnPaint();

	void DoModal(CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd);

	BOOL InitDlg();
	virtual ~CHmiInputDlg();
	CHmiDataStatic m_StBI[HMI_BI_PERPAGE*HMI_BI_PERLINE+1];
	CHmiObj* m_pObjList[HMI_BI_PERPAGE*HMI_BI_PERLINE+1];
	DECLARE_HMIMESSAGE_MAP() 
	//防止外部调用Show函数,确保只能通过DoModal来显示
private: 
	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
};

/////////////////////////////////////////////////////////
//////测量值显示对话框
class CHmiAIDlg : public CHmiWnd
{
public:

	void OnPaint(); //重载绘制函数

	CHmiAIDlg();
	BOOL InitDlg(LONG nDataType, WORD wAiNum =0, WORD* wAiNoList=NULL,char* cCaption=NULL);
	void DoModal(CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, LONG ndataType, WORD wAiNum =0, WORD* wAiNoList=NULL,char* cCaption=NULL);
	virtual ~CHmiAIDlg();
private:
	WORD m_nAllItemNum; //总共要显示的值的个数
	WORD m_wAiIndexList[HMI_AI_MAXDISP]; //显示序号列表
	CHmiDataStatic m_StAi[HMI_AI_PERPAGE];
	CHmiObj* m_pObjList[HMI_AI_PERPAGE+1];
	BOOL m_bIsAINoListValid;  //显示序号列表是否可用
	DECLARE_HMIMESSAGE_MAP() 
	//{{begin of user function and memeber(CHmiAIDlg)
	LONG m_nDataType; //要显示的数据类型， 可以显示遥测一次， 遥测二次， 保护一次， 保护二次
	LONG m_nDisplayType; //显示类型
	//防止外部调用Show函数,确保只能通过DoModal来显示
private: 
	 void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};

	
};
/**********************/

/////////////////////////////////////
//////soe详情显示痘翱?
class CSOEDetailDlg : public CHmiWnd
{
public:
	CSOEDetailDlg();
	virtual ~CSOEDetailDlg();
	BOOL ReadReport(LONG nItem,LONG DispIndex); //读soe数据
	void DoModal(CHmiWnd* pWndReturn);
	DECLARE_HMIMESSAGE_MAP() 
protected:
	BOOL Init();
	CHmiDataStatic m_Static[2];
	CHmiObj* m_pObjList[3];
	//防止外部调用Show函数,只能通过DoModal来显示
	private: 
		void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};

};

class COperationDetailDlg : public CHmiWnd
{
public:
	COperationDetailDlg();
	BOOL Init();
	virtual ~COperationDetailDlg();
	void DoModal(CHmiWnd* pWndReturn);
	BOOL ReadReport(LONG ItemIndex,LONG DispIndex);
	DECLARE_HMIMESSAGE_MAP() 

protected:
	CHmiDataStatic m_Static[4];
	CHmiObj* m_pObjList[5];
	//防止外部调用Show函数,只能通过DoModal来显示
	private: 
		void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};


};

class CHmiAlarmDetailDlg : public CHmiWnd
{
public:
	CHmiAlarmDetailDlg();
	BOOL Init();
	virtual ~CHmiAlarmDetailDlg();

	BOOL ReadReport(LONG ItemIndex,LONG DispIndex);//设置要显示的报告
	void DoModal(CHmiWnd* pWndReturn);
	DECLARE_HMIMESSAGE_MAP() 

protected:

	void SetFDNum(LONG Num);
	CHmiDataStatic m_Static[5+MAX_FDNUM];	//一个保护动作的最大故障电量数
	CHmiObj* m_pObjList[6+MAX_FDNUM];
	//防止外部调用Show函数,只能通过DoModal来显示
	private: 
		void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};

};

class CHmiStartDetailDlg : public CHmiWnd
{
public:
	CHmiStartDetailDlg();
	BOOL Init();
	virtual ~CHmiStartDetailDlg();

	BOOL ReadReport(LONG ItemIndex,LONG DispIndex);//设置要显示的报告
	void DoModal(CHmiWnd* pWndReturn);
	DECLARE_HMIMESSAGE_MAP() 

protected:

	void SetFDNum(LONG Num);
	CHmiDataStatic m_Static[5+MAX_FDNUM];	//一个启动报告的最大故障电量数
	CHmiObj* m_pObjList[6+MAX_FDNUM];
	//防止外部调用Show函数,只能通过DoModal来显示
private: 
	void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};
};

//动作报告对话框
class CHmiActionDetailDlg : public CHmiWnd
{
public:
	CHmiActionDetailDlg();

	void OnWndLeftRight(LONG LeftRight);

	BOOL ReadReport(LONG ItemIndex, LONG subItemIndex,LONG DispIndex);//设置要显示的报告
	BOOL Init();
	void DoModal(CHmiWnd* pWndReturn); //显示窗口并设置返回窗口

	virtual ~CHmiActionDetailDlg();

	DECLARE_HMIMESSAGE_MAP() 
protected:
	void SetFDNum(LONG Num); //设置实际显示的故障电量
	CHmiDataStatic m_Static[5+MAX_FDNUM];	//一个保护动作的最大故障电量数
	CHmiObj* m_pObjList[6+MAX_FDNUM];

	LONG m_nItem; //当前显示的报告号(数据库标识号
	LONG m_nSubItem; //当前显示的分报告号
	LONG m_nDispIndex; //显示序号
	
	//防止外部调用Show函数,只能通过DoModal来显示
	private: 
		void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};
	
};


class CHmiDlgSerialSet : public CHmiWnd
{
public:
	CHmiDlgSerialSet();
	BOOL Init();
	virtual ~CHmiDlgSerialSet();
	void OnEnterBtnSave();
	
	void DoModal(CHmiWnd* pCancelWnd);
	CHmiButton m_BtnSave;
	CHmiButton m_BtnESC;
	CHmiDataStatic m_StaticBaud;
	CHmiDataStatic m_StaticData;
	CHmiDataStatic m_StaticVerify;
	CHmiDataStatic m_StaticStopByte;
	CHmiCombo m_ComboCBaud;
	CHmiCombo m_ComboCData;
	CHmiCombo m_ComboCVerify;
	CHmiCombo m_ComboCStopByte;
	CHmiObj* m_pObjList[11];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndWSerialSet)
//手动添加的成员变量放在括号之间
	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndWSerialSet)
};


///////////////////////////////////////////////
class CHmiDlgSysStatus : public CHmiWnd
{
public:
	CHmiDlgSysStatus();
	BOOL Init();
	virtual ~CHmiDlgSysStatus();
	void DoModal(CHmiWnd* pCancelWnd);
	CHmiDataStatic m_Staticstatic1;
	CHmiDataStatic m_Staticstatic2;
	CHmiDataStatic m_Staticstatic3;
	CHmiDataStatic m_Statictempreture;
	CHmiDataStatic m_Staticstatic4;
	CHmiDataStatic m_Staticbtime;
	CHmiDataStatic m_Staticbreakfail;
	CHmiDataStatic m_Staticbreakdown;
	CHmiObj* m_pObjList[9];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndSysStatus)
	void OnTimer(LONG TimerID);
	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndSysStatus)
};


///////////////////////////////////////////////
class CHmiDlgWComStatus : public CHmiWnd
{
public:
	CHmiDlgWComStatus();
//	BOOL Init();
	void DoModal(CHmiWnd* pCancelWnd);
	virtual ~CHmiDlgWComStatus();
	
	CHmiDataStatic m_Static[16];
	CHmiObj* m_pObjList[19];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndWComStatus)
//手动添加的成员变量放在括号之间
	void OnTimer(LONG TimerID);
	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndWComStatus)
};

///////////////////////////////////////////////
class CHmiDlgAddressSetting : public CHmiWnd
{
public:
	CHmiDlgAddressSetting();
	BOOL Init();
	void DoModal(CHmiWnd* pCancelWnd);
	virtual ~CHmiDlgAddressSetting();
	void OnEnterBtnenter();
	CHmiButton m_Btnenter;
	CHmiButton m_Btnesc;
	CHmiEdit m_EditIp1;
	CHmiEdit m_EditIp2;
	CHmiEdit m_EditIp3;
	CHmiEdit m_EditIp4;
	CHmiEdit m_EditIp5;
	CHmiEdit m_EditIp9;
	CHmiEdit m_EditIp10;
	CHmiEdit m_EditIp6;
	CHmiEdit m_EditIp7;
	CHmiEdit m_EditIp8;
	CHmiEdit m_EditIp11;
	CHmiEdit m_EditIp12;
	CHmiDataStatic m_Staticgate;
	CHmiDataStatic m_Staticadd;
	CHmiDataStatic m_Staticmask;
	CHmiObj* m_pObjList[18];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CHmiDlgAddressSetting)
	BOOL OnWndInit();

//}}end of user function and memeber(CHmiDlgAddressSetting)
};

class CLcdWndDefaultPara : public CHmiWnd
{
public:
	void DoModal(CHmiWnd* pCancelWnd);
	CLcdWndDefaultPara();
	
	BOOL Init();
	virtual ~CLcdWndDefaultPara();
	CHmiEdit m_EditDefaultParaPassword;
	CHmiDataStatic m_Staticstatic1;
	CHmiDataStatic m_Staticstatic2;
	CHmiDataStatic m_Staticdynamic;
	CHmiDataStatic m_Staticpassword;
	CHmiDataStatic m_Staticstatic3;
	CHmiDataStatic m_Staticstatic4;
	CHmiDataStatic m_Staticstatic5;
	CHmiObj* m_pObjList[9];
	DWORD value;
	char dynamic[16];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndDefaultPara)
//手动添加的成员变量放在括号之间
	BOOL OnWndInit();
	void OnWndEnter();
//}}end of user function and memeber(CLcdWndDefaultPara)
};
///////////////////////////////////////////////
class CLcdWndversion : public CHmiWnd
{
public:
	CLcdWndversion();
	BOOL Init();
	virtual ~CLcdWndversion();
	
	void DoModal(CHmiWnd* pCancelWnd);
	CHmiDataStatic m_StaticScrc;
	CHmiDataStatic m_Staticcrc;
	CHmiDataStatic m_StaticSsoft;
	CHmiDataStatic m_Staticsoft;
	CHmiDataStatic m_StaticSsyslogic;
	CHmiDataStatic m_Staticsyslogic;
	CHmiDataStatic m_StaticSdatabase;
	CHmiDataStatic m_Staticdatabase;
	CHmiDataStatic m_StaticScreatetime;
	CHmiDataStatic m_Staticcreatetime;
	CHmiDataStatic m_StaticSedittime;
	CHmiDataStatic m_Staticedittime;
	CHmiDataStatic m_Staticmac;	
	CHmiDataStatic m_Staticmaca;
	CHmiDataStatic m_Staticmacb;
	CHmiObj* m_pObjList[16];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndversion)
	BOOL OnWndInit();

//}}end of user function and memeber(CLcdWndversion)
};


///////////////////////////////////////////////
class CLcdWndAutoJustStep2 : public CHmiWnd
{
public:
	CLcdWndAutoJustStep2();
	BOOL Init();
	virtual ~CLcdWndAutoJustStep2();
	CHmiEdit m_EditBaseV;
	CHmiEdit m_EditBaseI;
	CHmiDataStatic m_Static5067;
	CHmiDataStatic m_Static5070;
	CHmiDataStatic m_Static5072;
	CHmiDataStatic m_Static5073;
	CHmiDataStatic m_Static5074;
	CHmiObj* m_pObjList[8];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndAutoJustStep2)
	BOOL OnWndInit();
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndAutoJustStep2)
};

///////////////////////////////////////////////
class CLcdWndAutoJustStep1 : public CHmiWnd
{
public:
	CLcdWndAutoJustStep1();
	BOOL Init();
	virtual ~CLcdWndAutoJustStep1();
	CHmiDataStatic m_Static5066;
	CHmiDataStatic m_Static5063;
	CHmiDataStatic m_Static5064;
	CHmiDataStatic m_Static5065;
	CHmiDataStatic m_Static5068;
	CHmiObj* m_pObjList[6];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndAutoJustStep1)
//手动添加的成员变量放在括号之间
    void OnWndEnter();
//}}end of user function and memeber(CLcdWndAutoJustStep1)
};
///////////////////////////////////////////////
class CLcdWndPortPara : public CHmiWnd
{
public:
	CLcdWndPortPara();
	BOOL Init();
	
	void DoModal(CHmiWnd* pCancelWnd);
	virtual ~CLcdWndPortPara();
	CHmiDataList m_ListPortParaList;
	CHmiObj* m_pObjList[2];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndPortPara)
//手动添加的成员变量放在括号之间
	static void SavePortPara(); //保存DI去抖时间
	void OnWndEnter();

//}}end of user function and memeber(CLcdWndPortPara)
};
///////////////////////////////////////////////
class CLcdWndETHportModity : public CHmiWnd
{
public:
	CLcdWndETHportModity();
	BOOL Init();
	void DoModal(CHmiWnd* pCancelWnd);
	virtual ~CLcdWndETHportModity();
	CHmiEdit m_EditETHA_Link1;
	CHmiEdit m_EditETHA_Link2;
	CHmiEdit m_EditETHA_Link3;
	CHmiEdit m_EditETHA_Link4;
	CHmiEdit m_EditETHA_Link5;
	CHmiEdit m_EditETHB_Link1;
	CHmiEdit m_EditETHB_Link2;
	CHmiEdit m_EditETHB_Link3;
	CHmiEdit m_EditETHB_Link4;
	CHmiEdit m_EditETHB_Link5;
	CHmiDataStatic m_Static5619;
	CHmiDataStatic m_Static5620;
	CHmiDataStatic m_Static5621;
	CHmiDataStatic m_Static5622;
	CHmiDataStatic m_Static5623;
	CHmiDataStatic m_Static5624;
	CHmiDataStatic m_Static5625;
	CHmiDataStatic m_Static5626;
	CHmiDataStatic m_Static5627;
	CHmiDataStatic m_Static5628;
	CHmiObj* m_pObjList[21];
	DECLARE_HMIMESSAGE_MAP() 
//{{begin of user function and memeber(CLcdWndETHportModity)
//手动添加的成员变量放在括号之间
	BOOL OnWndInit();	
	void OnWndEnter();
	WORD ethportnum[10];
	BOOL ethPortnumChaFlag[10];
	static void SaveETHPortNum();
//}}end of user function and memeber(CLcdWndETHportModity)
};


#endif /* __HmiMsgBox_H */
