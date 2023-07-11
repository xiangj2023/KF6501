/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiApp.h
* 
*   ���ģ�飺����ؼ�
*
* 
*	����	����Ӧ�û��࣬ Һ��Ӧ�ôӸ���̳�
* 
* 
* 
*   ����	Init() ��ʼ����������ʾ��ʼ���ڣ� ����ĳ�ʼ��������Ҫ���øú���
*			OnTimerOut(DWORD id) ��ʱ����Ϣ��Ӧ����					
*			OnKeyMsg()	������Ϣ��Ӧ����
*			RunApp()	��window��ʹ�õ�ģ�����к����� Ŀ����ϲ�ʹ�øú���	
*			HMIMsgBox 	��Ϣ��
*			HMIPasswordBox ���������
*			HMIChgPassBox �����޸Ŀ�		
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-4-21		��ʼ����
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

#define HMI_LANOFF_IDLETIME 600  //Ϩ�𱳹�Ŀ���ʱ����
#define HMI_RUNWND_IDLETIME 60  //��ʾ������Ϣ�Ŀ���ʱ����
#define HMI_CONTRAST_CHECKTIME 60 //Һ���Զȼ����̼��ʱ��

#ifdef __PL_WIN_SIMULATION__
#define APPBASE 
#else
#define APPBASE  :public CApp
#endif


/****************************************************************************************
 * ��������: ��ʾ����Ի���
 * ����˵��: 
 *			- char *szCaption [in] ����
 *			
 *			- char* szMsg [in] ��ʾ��Ϣ
 *			
 *			- CHmiWnd *pPassOkWnd [in] ������ȷ��ת����
 *			- CHmiWnd *pCancelWnd [in] ȡ����ת����
 *			- LONG nPassType =0 [in] �������� 0����ͨ���� 1:����Ա����
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
void HMIPasswordBox(const char *szCaption, const char* szMsg,  CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);

/****************************************************************************************
 * ��������: ��ʾ��Ϣ�Ի���
 * ����˵��: 
 *			- char *szCaption1 [in] �Ի������
 *			- char* szMsg1 [in] ��ʾ��Ϣ, һ����ʾ����ʱ������ʹ��"\n"������ʾ
 *			- CHmiWnd *pOkWnd [in] ok��ť��ת����
 *			- CHmiWnd *pCancelWnd [in] cancel��ť��ת����
 *			- LONG nType = HMI_MB_OK [in] 
 *				   HMI_MB_OK ��ʾok��ť  HMI_MB_OKCANCEL ok��cancel��ť����ʾ
 *			- YES_NO_PTR pYES_Fn=NULL [in] ok��ť��Ӧ����
 *			- YES_NO_PTR pNO_Fn=NULL [in] cancel��ť��Ӧ����
 * ���ش���: 
 * ����: ע�⣬ ��Ϣ�Ի���İ�ť��Ӧ���������⴦�����û�а�ť��Ӧ��������ת������Ч��
 *		����а�ť��Ӧ����������ת������Ч(�������Ա�֤��ת���ĸ�������ȫ�ɴ��������� )	
 *****************************************************************************************/
void HMIMsgBox(const char *szCaption,const char* szMsg
			   , CHmiWnd *pOkWnd=NULL, CHmiWnd *pCancelWnd =NULL, LONG nType = HMI_MB_OK
			   , YES_NO_PTR pYES_Fn=NULL, YES_NO_PTR pNO_Fn=NULL);


/****************************************************************************************
 * ��������: ��ʾ������ĶԻ���
 * ����˵��: 
 *			- char *szCaption1 [in] �Ի������
 *			- CHmiWnd *pPassOkWnd
 *			- CHmiWnd *pCancelWnd
 *			- LONG nPassType =0  0 :��ͨ���� 1:����Ա����
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
void HMIChgPassBox( const char *szCaption,CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType =0);

//Һ���Աȶȴ���
void HMIContrastBox(CHmiWnd* pOkWnd, CHmiWnd* pCancelWnd);



/****************************************************************************************
 * ��������: ���¿���ʾ��ֵ�б�
 * ����˵��: 
 *			- long nAreaNo [in] Ҫ��ʾ�Ķ�ֵ����
 *			- LONG* nCount [out] ����ʾ�Ķ�ֵ����
 *			- WORD* SetList [out] ����ʾ��ֵ�б� 
 *			- BOOL bMode [in] ���¶�ֵ�б��ԭ��:TRUE,��ֵ��ѯ��FALSE����ֵ�޸�
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
	BOOL HMIUpDataVisibleSetList(long nAreaNo, LONG* nCount, WORD* SetList,BOOL bMode);

/****************************************************************************************
 * ��������: �����ݶ�ϵ����Hmi������
 * ����˵��: 
 *			
 * ���ش���: true ���ɹ��� false ��ʧ��
 * ����: 
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
	
	//���涨ֵ�������޸�
	static float m_fCurSetting[RS_COUNT+1]; //��ֹRS_COUNTΪ0ʱ����
	static float m_fNewSetting[RS_COUNT+1];
	static BYTE m_byCurCfg[CFG_COUNT+1];
	static BYTE m_byNewCfg[CFG_COUNT+1];
	
	static BYTE m_byCurSoftSwitch[SW_COUNT+1];
	static BYTE m_byNewSoftSwitch[SW_COUNT+1];

	//������Ǹ������ݿ������̬����ռ��
	static LONG* m_npCFList;  //�����û������ϵ��
	static BOOL m_bCFListModified; //ϵ���������޸Ĺ�
	
	static WORD* m_wpDIFilterTime; //�����û������ң��ȥ��ʱ��
	static BOOL  m_bDIFilterTimeModified; //ȥ��ʱ�仺�����޸ı�ʶ
	
	static WORD* m_wpDIClass; //�����û������DI����
	static BOOL  m_bDIClassModified; // DI�����޸ı�־
	
	static WORD* m_wpBOTime; //�����û������ң�ر���ʱ��
	static BOOL m_bBOTimeModified; //ң�ر���ʱ���޸ı�־
	
	
	static DWORD* m_dwpCIConst; //���峣��
	static BOOL  m_bCIConstModified;
	
	static DWORD* m_dwpCIBase; //��ȵ���
	static BOOL m_bCIBaseModified; //��ȵ����������޸ı�ʶ

	
	static WORD* m_wpBIControl; // BI�Ƿ�ȡ�� �� ��wControl���D2λ
	static BOOL m_bBIControlModified;

	static WORD* m_wpPortPara;   //�˿ڹ�Լ����
	static BOOL m_bPortParaModified;

	
	static WORD* m_wpPortProtocl;   //�˿ڹ�Լ����

	static WORD* m_wpVisibleSetList; 	//�������ʾ�Ķ�ֵ���б�
	
	static float* m_fpSysSetList; //���渨����ֵ
	static BOOL m_bSysSetListModified;

	virtual void Init();
	void OnTimeOut(DWORD id);
	void OnKeyDown(DWORD key, DWORD par );

	void OnKeyMsg(long MsgCode, long wParam, long lParam);
	void OnRelayEvent(DWORD MsgCode, DWORD wParam);
//	void OnAutoAdjustEcho1(DWORD status, DWORD par ); //ϵ���Զ�У��������Ϣ��Ӧ����
	void OnAutoAdjustEcho(DWORD status, DWORD par );
	static CHmiWnd* m_pActiveWnd; //�����
	static CHmiWnd* m_pInitWnd; //��ʼ����
	static CHmiWnd* m_pRunWnd; //������Ϣ����
	static CHmiWnd* m_pMainWnd; //���˵�����
	CHmiApp(DWORD *pdwApp);
	virtual ~CHmiApp();
	static CHmiWnd** m_ppWndList;

	COperationDetailDlg  m_OperationDetailDlg;
	CHmiActionDetailDlg m_ActionDetailDlg; //������������
	CHmiAlarmDetailDlg m_AlarmDetailDlg; //�澯�������鴰��
	CHmiStartDetailDlg m_StartDetailDlg; //�����������鴰��
	

	CHmiInputDlg m_BIDlg; //������ʾ����	
	//2011-4-1 15:36:03
	CHmiGraphWnd m_WndGraph; //������ͼ
	static CTranslator m_Trans;


	//��windows ��ģ������ʱʹ��
#ifdef __PL_WIN_SIMULATION__
	void  RunApp();
#endif

	void TurnOnLight();
	void TurnOffLight();


	//�ر����д���
	static void CloseAllWindows();
	
	static LONG m_nWinNum; //���ڸ���

	inline void SetWinNum(LONG num)
	{
		m_nWinNum = num;
	}
	LONG GetWinNum()
	{
		return m_nWinNum;
	}

protected:
		
	volatile  BOOL m_bIsLightOff; // �Ƿ��ǹ��˱����� 
	volatile LONG m_nIdelTime; //����ʱ��
	DWORD m_dwKeyTime;		//�������м�ʱ
//	WORD m_wContrastCheckTime; //Һ���Զȴ���ʱ�� 2011-8-25 ����Ҫ�ˣ���ʱ���̲���ȷ
	DWORD m_dwCombKeyTimer; //��ϼ������µ�ʱ��

};

/**********************/

#endif /* __HmiApp_H */
