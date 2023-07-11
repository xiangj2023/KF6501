/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   �ļ����ƣ�HmiApp.cpp
*   ���ģ�飺<...>
*   �� �� �ţ�1.0
*   �������ڣ�2008-3-3
*   ��    �ߣ�kangqian
*   ��    �ܣ�<...>
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

CHmiMsgBox g_MsgBox; //��Ϣ��ʾ����
CHmiMsgBox g_MsgBox2; //��Ϣ��ʾ����

CHmiPasswordBox g_PssBox; //���봰��
CHmiPassChgBox g_PssChgBox; //�����趨����
CHmiLcdContrastBox g_ContrastBox; //�Աȶ��޸Ĵ���

CHmiInputDlg g_BIDlg; //������ʾ����
CHmiAIDlg g_AIDlg;  //AI��ʾ���ڣ�������ʾһ�ζ���ң�⣬ һ�ζ��α�������ֵ
CHmiLangDlg g_LangDlg; //��ʾ������ѡ��Ի���
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
CHmiWnd* CHmiApp::m_pInitWnd = NULL; //��ʼ����
CHmiWnd* CHmiApp::m_pRunWnd = NULL; //������Ϣ����
CHmiWnd* CHmiApp::m_pMainWnd = NULL; //���˵�����

CHmiWnd** CHmiApp::m_ppWndList = NULL;
LONG CHmiApp::m_nWinNum = 0;

float CHmiApp::m_fCurSetting[RS_COUNT+1];
float CHmiApp::m_fNewSetting[RS_COUNT+1];

BYTE CHmiApp::m_byCurCfg[CFG_COUNT+1];
BYTE CHmiApp::m_byNewCfg[CFG_COUNT+1];

BYTE CHmiApp::m_byCurSoftSwitch[SW_COUNT+1];
BYTE CHmiApp::m_byNewSoftSwitch[SW_COUNT+1];

LONG* CHmiApp::m_npCFList=NULL;  //�����û������ϵ��
BOOL CHmiApp::m_bCFListModified = FALSE;
WORD* CHmiApp::m_wpDIFilterTime=NULL; //�����û������ң��ȥ��ʱ��
BOOL CHmiApp::m_bDIFilterTimeModified = FALSE;

WORD* CHmiApp::m_wpDIClass=NULL; //DI���ԣ�
BOOL CHmiApp::m_bDIClassModified=FALSE;

WORD* CHmiApp::m_wpBOTime=NULL;	//�����û������ң�ر���ʱ��
BOOL CHmiApp::m_bBOTimeModified=FALSE;

BOOL CHmiApp::m_bCIBaseModified=FALSE;
DWORD* CHmiApp::m_dwpCIBase=NULL;

DWORD* CHmiApp::m_dwpCIConst=NULL; //���峣��
BOOL CHmiApp::m_bCIConstModified=FALSE;



WORD* CHmiApp::m_wpBIControl=NULL; // BI�Ƿ�ȡ�� �� ��wControl���D2λ
BOOL CHmiApp::m_bBIControlModified=FALSE;

WORD* CHmiApp::m_wpPortPara = NULL; //�˿ڹ�Լ����
BOOL CHmiApp::m_bPortParaModified = FALSE;

WORD* CHmiApp::m_wpPortProtocl = NULL; //�˿ڹ�Լ����

WORD* CHmiApp::m_wpVisibleSetList=NULL;

float* CHmiApp::m_fpSysSetList=NULL; //���渨����ֵ
BOOL CHmiApp::m_bSysSetListModified = FALSE;

CTranslator CHmiApp::m_Trans;

//Һ���Աȶ�
WORD g_wLCDContrast;
//extern DWORD dwRelayMSCnt;		//�����ú��������
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
	
	//��ϼ������µ�ʱ��
	m_dwCombKeyTimer = 0;
}

CHmiApp::~CHmiApp()
{

}

/*************************************************************************
* �Ķ���ʷ��¼��

Revision 1.0, 2008-3-3, kangqian
describe: ��ʼ����.
*************************************************************************/
#ifdef __PL_WIN_SIMULATION__
//��windows��ģ������ʱ���øú���
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

				//�Ƿ��Ƕ�ʱ����Ϣ
				if(nMsgCode == OS_TIMER)
				{	
					OnTimeOut(nParam1);
					continue;
				}

				if(m_bIsLightOff)
				{
					continue;
				}
				//�Ƿ��ǹ��ϵ����ȵȲ����ɾ��崰�ڣ�����Ӧ�����������Ϣ
				
				//������Ϣ
				m_nIdelTime = 0;
				m_pActiveWnd->WndProc(nMsgCode, nParam1, nParam2);
			}
		}
	}

}
#endif // #ifdef __PL_WIN_SIMULATION__

void CHmiApp::Init()
{
	//����ռ�

	//��ȡ�ֿ⵽�ڴ�
#ifndef __PL_WIN_SIMULATION__
	//������ļ�����Ļ��Ƕ��ļ����ݵ�ram��
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

	//װ�������ļ�
//	m_Trans.Load("trLan.bHS");

	//�������Բ���
	g_LanFileMan.Init();


	//ϵ�����뻺����
	int nCf = DB_GetCFNum();
	m_npCFList = new LONG[nCf+1];
	memset((void*)m_npCFList, 0, (nCf+1)*sizeof(LONG));

	//ֱ�Ӳ��õ�ȥ��ʱ��
	int nDI = DB_GetDINum();
	m_wpDIFilterTime = new WORD[nDI+1];
	memset((void*)m_wpDIFilterTime, 0, (nDI+1)*sizeof(WORD));

	int nBO = DB_GetBONum();
	m_wpBOTime = new WORD[nBO+1];
	memset((void*)m_wpBOTime, 0, (nBO+1)*sizeof(WORD));
	
	//���峣��
	WORD nCI = DB_GetPulseCINum();
	m_dwpCIConst = new DWORD[nCI+1];
	memset((void*)m_dwpCIConst, 0, (nCI+1)*sizeof(DWORD));

	//��ȵ���
	int nAllCINum = DB_GetCINum();
	m_dwpCIBase = new DWORD[nAllCINum+1];
	memset((void*)m_dwpCIBase, 0, (nAllCINum+1)*sizeof(DWORD));

	int nBI = DB_GetBINum();
	m_wpBIControl = new WORD[nBI+1];
	memset((void*)m_wpBIControl, 0, (nBI+1)*sizeof(WORD));


	//��ֵ
	int nSet = DB_GetSETNum();
	m_wpVisibleSetList = new WORD[nSet+1]; //������ʾ�Ķ�ֵ����б�
	memset((void*)m_wpVisibleSetList, 0, (nSet+1)*sizeof(WORD));

	//�˿ڲ���
	m_wpPortPara = new WORD[32];	
	memset((void*)m_wpPortPara, 0, (32)*sizeof(WORD));
	
	//�˿ں�
	m_wpPortProtocl = new WORD[10];	
	memset((void*)m_wpPortProtocl, 0, (10)*sizeof(WORD));

	//������ֵ
	int nSysSet = DB_GetSystemSETNum();
	m_fpSysSetList = new float[nSysSet+1];
	memset((void*)m_fpSysSetList, 0, (nSysSet+1)*sizeof(float));

	//��ȡeeprom �б����Һ���Աȶ�
	WORD wVal[2];
	rc = EEPROM_Read(EEPROM_SYSPARA_LCDCTLPAR, 4, wVal);
	if( ((wVal[0]&wVal[1]) == 0) && ((wVal[0]|wVal[1]) == 0xffff) )
		g_wLCDContrast =  wVal[0];
	else
		g_wLCDContrast = HMI_LCDCONTRAST_DEF;
	//ReportMsg("CHmiApp read lcd para vale = 0x%x.",g_wLCDContrast);
	
	//������ͼ�ļ�
	m_WndGraph.Load("Graph.grh");
	m_pInitWnd = &m_WndGraph;

	//Һ�������ö�ʱ��	
	SetTimer(1,1000);
}
void CHmiApp::OnTimeOut(DWORD id)
{
	if(m_pActiveWnd)
	{
		m_pActiveWnd->OnTimer(id);
	}

	m_nIdelTime++;

	//�����ö�ʱ��
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

	//��ʱ���Һ��Ϩ��
	if(!m_bIsLightOff)
	{
		m_dwKeyTime++;
		if(m_dwKeyTime == HMI_LANOFF_IDLETIME)	//����1�����޼��̰��£��رձ���
		{
			TurnOffLight();
		}
	}

//	//Һ���Զȼ�Ᵽ��
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
	//��ֹ��ż������������Һ������������
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

	//��������
	m_dwKeyTime = 0;
	TurnOnLight();
	
	//���̴���
	m_pActiveWnd->TransKeyMesssage((WORD)key);

	//������ϼ�����
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
	//�б����ʱ�̿����� 
	m_dwKeyTime = 0;
	TurnOnLight();
	switch(MsgCode)
	{
		//����
	case 0:
//		if(m_StartDetailDlg.ReadReport(wParam,0))
//			m_StartDetailDlg.DoModal(m_pMainWnd);
		break;
		//����
	case 1:
		
		if(m_ActionDetailDlg.ReadReport(wParam, 0, 0))
			m_ActionDetailDlg.DoModal(m_pMainWnd);
		break;
		//�澯
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
			sprintf(strCap, "%s", tr("ϵ���Զ�����"));
			sprintf(strMsg, "%s", tr("ϵ�������ɹ�"));		
		}
		else
		{
			sprintf(strCap, "%s", tr("ϵ���Զ�����"));
			sprintf(strMsg, "%s", tr("ϵ������ʧ��"));		
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
			sprintf(strCap, "%s", tr("ϵ���Զ�����"));
			sprintf(strMsg, "%s", tr("ϵ������ʧ��"));
			HMIMsgBox(strCap,  strMsg, &(CLcdApp::m_WndfactorySet), &(CLcdApp::m_WndfactorySet));		
		}
	}
}

//ϵ��
/*
void CHmiApp::OnAutoAdjustEcho1(DWORD status, DWORD par )
{
	//statusΪУ׼���������ΪSUCCESS��FAILURE
	char strMsg[64], strCap[64];

	if(status == SUCCESS)
	{  
		CLcdApp::m_WndAutoJustSetp2.Show();

	}
	else
	{
		sprintf(strCap, "%s", tr("ϵ���Զ�����"));
		sprintf(strMsg, "%s", tr("ϵ������ʧ��"));
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

///////////////////////ȫ�ֺ���///////////////////////////
/****************************************************************************************
 * ��������: ��ʾ������ĶԻ���
 * ����˵��: 
 *			- char *szCaption1 [in] �Ի������
 *			- CHmiWnd *pPassOkWnd [in]
 *			- CHmiWnd *pCancelWnd [in]
 *			- LONG nPassType =0 [in] 0 :��ͨ���� 1:����Ա����
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
void HMIChgPassBox(const char *szCaption1, CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType)
{
	g_PssChgBox.DoModal(szCaption1, pPassOkWnd,  pCancelWnd,  nPassType);
}

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
void HMIPasswordBox(const char *szCaption1, const char* szMsg1 
					, CHmiWnd *pPassOkWnd, CHmiWnd *pCancelWnd, LONG nPassType )
{
	g_PssBox.DoModal(szCaption1,  szMsg1,  pPassOkWnd, pCancelWnd, nPassType);


}

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

void HMIMsgBox(const char *szCaption,const char* szMsg, CHmiWnd *pOkWnd, CHmiWnd *pCancelWnd, LONG nType , YES_NO_PTR pYES_Fn, YES_NO_PTR pNO_Fn)
{
	g_MsgBox.DoModal(szCaption,  szMsg,  pOkWnd, pCancelWnd,  nType, pYES_Fn,  pNO_Fn);
}

//��ʾҺ���ԱȶȶԻ���
void HMIContrastBox(CHmiWnd* pOkWnd, CHmiWnd* pCancelWnd)
{
	g_ContrastBox.DoModal(pOkWnd, pCancelWnd);
}


/****************************************************************************************
 * ��������: ���¿���ʾ��ֵ�б�
 * ����˵��: 
 *			- long nAreaNo [in] Ҫ��ʾ�Ķ�ֵ����
 *			- LONG* nCount [out] ����ʾ�Ķ�ֵ����
 *			- WORD* SetList [out] ����ʾ��ֵ����б�
 *			- BOOL bMode [in] ���¶�ֵ�б��ԭ��:TRUE,��ֵ��ѯ��FALSE����ֵ�޸�
 * ���ش���: 
 * ����: ���Ӳ���bMode,װ�����������ѯ��ֵʱm_byNewCfg����Ϊ�յ����޷���ʾ��Ͷ��Ķ�ֵ
 *****************************************************************************************/
BOOL HMIUpDataVisibleSetList(long nAreaNo, LONG* nCount, WORD* SetList,BOOL bMode)
{

	//ֻ��ʾ����Ͷ���˵Ķ�ֵ
	//�������б��ɹ��� ��ʾ
	//û�й������õ� ��ʾ
	// ���������õģ����������б���������Ƿ���Ͷ�룬��Ͷ�����ʾ
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


	//��ÿ����ֵ�� �õ����������б�, ����������Ϊ0�Ķ�ֵ��ż�����ʾ�б�
	//������ȡ�������õ�ֵ��ֻҪ��һ��Ͷ�룬��ö�ֵ��ż�����ʾ�б�
	int i, m;
	for(i=cfgWordNum; i<nSet; i++)
	{
		rc = DB_GetSETDisp(i, &tSETDisp);
		if(DB_ERR_OK != rc)
		{
			//�����ݿ�����޷��õ������б���ʾ�ö�ֵ
			SetList[k++] = i;
			bNoERR = FALSE;
			continue;
		}
			

		//�õ���ֵ������������Ϣ
		for(LONG m=0;m<SETCFG_MAXNUM;m++)
			nCFGIndex[m] = tSETDisp.CFGList[m];
		nCfgCount = tSETDisp.nCFG;

		//���������õĶ�ֱֵ����ʾ
		if(nCfgCount <=0)
		{
			SetList[k++] = i;
			continue;
		}
		

		//��鶨ֵ�Ƿ���Ҫ��ʾ(�޹��� ���� Ͷ��)
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

			if(CFGStatus == CFG_STATUS_SET) //����Ͷ������ʾ��ֵ
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

//��ϵ�����ݵ����������ɹ�����true,ʧ�ܷ���false
BOOL HMIReadCFToBuf()
{
	//�����ݿ��ȡϵ���Ļ�����
	LONG rc = DB_GetCFValue(CF_VALUE_ALL, CHmiApp::m_npCFList);
	if(DB_ERR_OK != rc)
		return FALSE;
	CHmiApp::m_bCFListModified = FALSE;
	return TRUE;
}

//���滺������ϵ�����ݵ����ݿ�
//�ɹ�����true����ȡʧ�ܷ���false
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
//�����ݿ��ȡң�ر���ʱ�䵽������
//�ɹ�����true��ʧ�ܷ���false
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
//����ң�ر���ʱ�������е����ݵ����ݿ�
//����ɹ�����true������ʧ�ܷ���false
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
//�����ݿ��ȡ����ȥ��ʱ�䵽������
//��ȡ�ɹ�����true����ȡʧ�ܷ���false
BOOL HMIReadDIFilterTimeToBuf()
{
	LONG rc;
	//��ʼ��DiFilterTime�Ļ�����
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

//���滺�����Ŀ���ȥ��ʱ�����ݵ����ݿ�
//�ɹ�����true����ȡʧ�ܷ���false
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

//�����ݿ��ȡ�������Ե�������
//��ȡ�ɹ�����true����ȡʧ�ܷ���false
BOOL HMIReadDIClassToBuf()
{
	
	LONG rc;
	//��ʼ��DiFilterTime�Ļ�����
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
//���滺�����Ŀ����������ݵ����ݿ�
//�ɹ�����true����ȡʧ�ܷ���false
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

//�����ݿ��ȡ��ȵ�����������
//��ȡ�ɹ�����true����ȡʧ�ܷ���false
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

//���滺�����ĵ�ȵ������ݵ����ݿ�
//�ɹ�����true����ȡʧ�ܷ���false
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
//ע�⣬���ֵ�������峣��
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
//�����ݿ��ȡ���峣����������
//��ȡ�ɹ�����true����ȡʧ�ܷ���false
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

//�����ݿ��ȡң�����Ե�������
//��ȡ�ɹ�����true����ȡʧ�ܷ���false
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
//���滺������ң�Ų������ݵ����ݿ�
//�ɹ�����true����ȡʧ�ܷ���false
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

//�����ݿ��ȡ������ֵ��������
//��ȡ�ɹ�����true����ȡʧ�ܷ���false
BOOL HMIReadSysSetToBuf()
{
	//��ȡ������ֵ��������
	
	CHmiApp::m_bSysSetListModified = FALSE;
	LONG rc = DB_GetSystemSETFloatValue(SET_VALUE_ALL, CLcdApp::m_fpSysSetList);
	if (rc != DB_ERR_OK)
	{
		return FALSE;
	}
	return TRUE;

}
//���滺���������ݵ����ݿ�
//�ɹ�����true����ȡʧ�ܷ���false
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
	//��ȡ���ڵ���ѹ�建������
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
	//����ֵ�����õ�������
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

