/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiMsgBox.h
* 
*   ���ģ�飺�Ի���
*
* 
*	���� CHmiMsgBox��ͨ����Ϣ�Ի���
*		 CHmiPasswordBox: ����Ի���
*		 CHmiAIDlg�� ����ֵ��ʾ�Ի��򣬿���ʾ����ֵ�� ��������ֵ�� ���ֵ
*		 CHmiInputDlg�� ������ʾ�Ի��� 
*		 CHmiLangDlg: ����ѡ��Ի���
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2009-12-10		��ʼ����
* 		kangqian	2010-08-22		����CHmiLangDlg
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

#define  HMI_BI_PERLINE 6 //ÿ�п�����ʾ6������
#define  HMI_BI_PERPAGE 4 //ÿҳ��ʾ4��

#define  HMI_AI_PERPAGE 10 //ÿҳ������ʾ8��ң��

#define  HMI_AI_MAXDISP 256 //����ʾ��Ai�������Ŀ

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
//�ֶ���ӵĳ�Ա������������֮��
//}}end of user function and memeber(CLcdWndLcdContrastBox)
};

class CHmiMsgBox : public CHmiWnd  
{
	CHmiStatic m_Stmsg; 
	CHmiButton m_BtnOk;
	CHmiButton m_BtnCancel;
	CHmiObj* m_pObjList[4]; //�ؼ�ָ���б�Ҫ�ȿؼ���һ�� m_pObjList[0]�ǿյ�
							//ԭ�����������û���κοؼ�m_pObjList�Ķ�����Ȼ����Ч��

public:
	CHmiMsgBox();
	void DoModal(const char *szCaption1,	const char* szMsg1, CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, 
							 LONG nType, YES_NO_PTR pYES_Fn=NULL, YES_NO_PTR pNO_Fn=NULL);
	YES_NO_PTR m_pFnYes;	//YES_NO��Ӧ���� 
	YES_NO_PTR m_pFnNo; 
	void OnEnterBtnOk();
	void OnEnterBtnCancel();
	BOOL InitDlg();

	virtual ~CHmiMsgBox();
	DECLARE_HMIMESSAGE_MAP() 
	//��ֹ�ⲿ����Show����,ȷ��ֻ��ͨ��DoModal����ʾ
private: 
	 void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
};
///////////////////////////////////////////////
//����Ի��� Ϊ�˼��ٽ����ƶ������� ������ȷ�Ϻ�ȡ����ť�� ֱ��ʹ��enter ����esc��
class CHmiPasswordBox : public CHmiWnd
{
	
	CHmiStatic m_StaticMsg;
	CHmiStatic m_StaticMsg2;
	
	CHmiStatic m_StaticDynamic;
	CHmiStatic m_DynamicValue;
	CHmiObj* m_pObjList[6];
	int m_nType;// ��������
	void OnWndEnter();
	BOOL OnWndInit();
	CHmiEdit m_EditPass;
	BOOL InitDlg();
	CHmiWnd *m_pOKWnd; // ȷ����������ȷ��ת����
	
	DWORD value;	
	char Dynamic[8];
	BOOL b_dpasswordOK;
	BOOL b_upasswordOK;
public:
	CHmiPasswordBox();
	virtual ~CHmiPasswordBox();
	void DoModal(const char *szCaption1, const char* szMsg1,  CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);
	DECLARE_HMIMESSAGE_MAP() 
	//��ֹ�ⲿ����Show����,ȷ��ֻ��ͨ��DoModal����ʾ
private: 
	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
		
};

//�������öԻ���
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
	CHmiWnd *m_pOKWnd; // ȷ����������ȷ��ת����
	
	int m_nType;
public:
	CHmiPassChgBox();
	virtual ~CHmiPassChgBox();
	void DoModal(const char *szCaption1,   CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);
	DECLARE_HMIMESSAGE_MAP() 
		//��ֹ�ⲿ����Show����,ȷ��ֻ��ͨ��DoModal����ʾ
private: 
	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};

};



///////////////////////////////////////////////
////������ʾ����
class CHmiInputDlg : public CHmiWnd
{
public:
	CHmiInputDlg();
	//���ػ��ƺ���
	void OnPaint();

	void DoModal(CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd);

	BOOL InitDlg();
	virtual ~CHmiInputDlg();
	CHmiDataStatic m_StBI[HMI_BI_PERPAGE*HMI_BI_PERLINE+1];
	CHmiObj* m_pObjList[HMI_BI_PERPAGE*HMI_BI_PERLINE+1];
	DECLARE_HMIMESSAGE_MAP() 
	//��ֹ�ⲿ����Show����,ȷ��ֻ��ͨ��DoModal����ʾ
private: 
	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
};

/////////////////////////////////////////////////////////
//////����ֵ��ʾ�Ի���
class CHmiAIDlg : public CHmiWnd
{
public:

	void OnPaint(); //���ػ��ƺ���

	CHmiAIDlg();
	BOOL InitDlg(LONG nDataType, WORD wAiNum =0, WORD* wAiNoList=NULL,char* cCaption=NULL);
	void DoModal(CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, LONG ndataType, WORD wAiNum =0, WORD* wAiNoList=NULL,char* cCaption=NULL);
	virtual ~CHmiAIDlg();
private:
	WORD m_nAllItemNum; //�ܹ�Ҫ��ʾ��ֵ�ĸ���
	WORD m_wAiIndexList[HMI_AI_MAXDISP]; //��ʾ����б�
	CHmiDataStatic m_StAi[HMI_AI_PERPAGE];
	CHmiObj* m_pObjList[HMI_AI_PERPAGE+1];
	BOOL m_bIsAINoListValid;  //��ʾ����б��Ƿ����
	DECLARE_HMIMESSAGE_MAP() 
	//{{begin of user function and memeber(CHmiAIDlg)
	LONG m_nDataType; //Ҫ��ʾ���������ͣ� ������ʾң��һ�Σ� ң����Σ� ����һ�Σ� ��������
	LONG m_nDisplayType; //��ʾ����
	//��ֹ�ⲿ����Show����,ȷ��ֻ��ͨ��DoModal����ʾ
private: 
	 void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};

	
};
/**********************/

/////////////////////////////////////
//////soe������ʾ����?
class CSOEDetailDlg : public CHmiWnd
{
public:
	CSOEDetailDlg();
	virtual ~CSOEDetailDlg();
	BOOL ReadReport(LONG nItem,LONG DispIndex); //��soe����
	void DoModal(CHmiWnd* pWndReturn);
	DECLARE_HMIMESSAGE_MAP() 
protected:
	BOOL Init();
	CHmiDataStatic m_Static[2];
	CHmiObj* m_pObjList[3];
	//��ֹ�ⲿ����Show����,ֻ��ͨ��DoModal����ʾ
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
	//��ֹ�ⲿ����Show����,ֻ��ͨ��DoModal����ʾ
	private: 
		void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};


};

class CHmiAlarmDetailDlg : public CHmiWnd
{
public:
	CHmiAlarmDetailDlg();
	BOOL Init();
	virtual ~CHmiAlarmDetailDlg();

	BOOL ReadReport(LONG ItemIndex,LONG DispIndex);//����Ҫ��ʾ�ı���
	void DoModal(CHmiWnd* pWndReturn);
	DECLARE_HMIMESSAGE_MAP() 

protected:

	void SetFDNum(LONG Num);
	CHmiDataStatic m_Static[5+MAX_FDNUM];	//һ�����������������ϵ�����
	CHmiObj* m_pObjList[6+MAX_FDNUM];
	//��ֹ�ⲿ����Show����,ֻ��ͨ��DoModal����ʾ
	private: 
		void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};

};

class CHmiStartDetailDlg : public CHmiWnd
{
public:
	CHmiStartDetailDlg();
	BOOL Init();
	virtual ~CHmiStartDetailDlg();

	BOOL ReadReport(LONG ItemIndex,LONG DispIndex);//����Ҫ��ʾ�ı���
	void DoModal(CHmiWnd* pWndReturn);
	DECLARE_HMIMESSAGE_MAP() 

protected:

	void SetFDNum(LONG Num);
	CHmiDataStatic m_Static[5+MAX_FDNUM];	//һ����������������ϵ�����
	CHmiObj* m_pObjList[6+MAX_FDNUM];
	//��ֹ�ⲿ����Show����,ֻ��ͨ��DoModal����ʾ
private: 
	void Show(DWORD param1, DWORD param2){CHmiWnd::Show(param1,param1);};
};

//��������Ի���
class CHmiActionDetailDlg : public CHmiWnd
{
public:
	CHmiActionDetailDlg();

	void OnWndLeftRight(LONG LeftRight);

	BOOL ReadReport(LONG ItemIndex, LONG subItemIndex,LONG DispIndex);//����Ҫ��ʾ�ı���
	BOOL Init();
	void DoModal(CHmiWnd* pWndReturn); //��ʾ���ڲ����÷��ش���

	virtual ~CHmiActionDetailDlg();

	DECLARE_HMIMESSAGE_MAP() 
protected:
	void SetFDNum(LONG Num); //����ʵ����ʾ�Ĺ��ϵ���
	CHmiDataStatic m_Static[5+MAX_FDNUM];	//һ�����������������ϵ�����
	CHmiObj* m_pObjList[6+MAX_FDNUM];

	LONG m_nItem; //��ǰ��ʾ�ı����(���ݿ��ʶ��
	LONG m_nSubItem; //��ǰ��ʾ�ķֱ����
	LONG m_nDispIndex; //��ʾ���
	
	//��ֹ�ⲿ����Show����,ֻ��ͨ��DoModal����ʾ
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
//�ֶ���ӵĳ�Ա������������֮��
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
//�ֶ���ӵĳ�Ա������������֮��
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
//�ֶ���ӵĳ�Ա������������֮��
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
//�ֶ���ӵĳ�Ա������������֮��
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
//�ֶ���ӵĳ�Ա������������֮��
	static void SavePortPara(); //����DIȥ��ʱ��
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
//�ֶ���ӵĳ�Ա������������֮��
	BOOL OnWndInit();	
	void OnWndEnter();
	WORD ethportnum[10];
	BOOL ethPortnumChaFlag[10];
	static void SaveETHPortNum();
//}}end of user function and memeber(CLcdWndETHportModity)
};


#endif /* __HmiMsgBox_H */
