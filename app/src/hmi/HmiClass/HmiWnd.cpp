/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   �ļ����ƣ�HmiWnd.cpp
*   ���ģ�飺<...>
*   �� �� �ţ�1.0
*   �������ڣ�2008-3-3
*   ��    �ߣ�kangqian
*   ��    �ܣ�<...>
*   �޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-8-11		��ʼ����
* 		kangqian	2010-9-14		�޸Ĺ���m_ppObjList�;��崰����ռ���
*									m_pObjectlist�Ĺ��������崰������m_pObjectlist
*									����Ϊ�ؼ���Ŀ+1�����飬 ���������һ����Ŀָ���
*									�����ڴ���û�пؼ���ʱ��Ҳ��������ؼ���taborder
*									��Ϊ��1��ʼ�ģ���������в��ܽ������뽹��Ŀؼ�����һ����-1�������Ǵ�0��ʼ
*************************************************************************/

// HmiWnd.cpp: implementation of the CHmiWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "HmiWnd.h"
#include "HmiApp.h"
#include "HmiMSG.h"
#ifndef __PL_WIN_SIMULATION__
#include "LcdCfg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const HMI_MSGMAP CHmiWnd::messageMap =
{
    //�������Ϣӳ�����
    NULL,

    //�������Ϣӳ�����
    &CHmiWnd::_messageEntries[0]
};

//ȡ������Ϣӳ������
const HMI_MSGMAP* CHmiWnd::GetMessageMap() const 
{
    return &CHmiWnd::messageMap;
}
//@#define ON_HMIWM_PAINT() \
//@{ WM_HMIPAINT,0, 0,0, Hmisig_vv, ((HMI_PMSG)(void (CHmiWnd::*)(void))&OnPaint},
//@#define ON_HMIWM_INIT() \


//����������Ϣӳ��
const HMI_MSGMAP_ENTRY CHmiWnd::_messageEntries[] =
{
	ON_HMIWND_PAINT()
	ON_HMIWND_MOVEFOCUS()
	ON_HMIWND_CHGPAGE()
//	ON_HMIWM_INIT()
	ON_HMIWND_SHOWWINDOW()
//	ON_HMIWND_TIMER()
    { 0, 0, 0, 0, HmiSig_end, 0 }     // nothing here
};
/*------------------------------------------------------------------------------------------*/
/* ����:������Ϣ���Ⱥ��������ݽ��յ���Ϣ���Զ�������Ӧ����Ϣ������                        */
/* ����:msg��������յ�����Ϣ                                                               */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
/*------------------------------------------------------------------------------------------*/
/****************************************************************************************
 * 
 * ��������: 
 * 
 * ����˵��: 
 *			- UINT message
 *			  ����: 
 *			  ˵��: ������յ�����Ϣ����Ϣ�ţ����ڵ���Ϣ��HMI_WND_INIT��HMI_WND_ENTER�����ؼ���ϢΪHMIWM_COMMAND��
 *
 *			- DWORD wParam
 *			  ����: 
 *			  ˵��: ����dword�Ͳ������ؼ���Ϣʹ�øò����� ��Ϣ�ţ���HMIBTN_ENTER��HMILIST_ENTER�ȺͿؼ�Idƴ��
 *
 *			- LONG lParam
 *			  ����: 
 *			  ˵��: ���õĺ���ʹ�õĲ���
 *
 *			- LONG *Result
 *			  ����: 
 *			  ˵��: 
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
BOOL CHmiWnd::OnWndMsg(UINT message, DWORD wParam, LONG lParam, LONG *Result)
{
    int  IsMap;
    const HMI_MSGMAP_ENTRY* lpEntry;
    BOOL bFound = FALSE;
	WORD nCode = HIWORD(wParam);
	WORD nID = LOWORD(wParam);

   //ȡ��ǰӦ�ó��������Ϣӳ���
    const HMI_MSGMAP* pMessageMap = GetMessageMap();
    
    //������Ӧ�ó����༰�����л������Ϣӳ���,����msg��Ϣ�ĵ��Ⱥ���
    for ( ; pMessageMap != NULL;pMessageMap = pMessageMap->pBaseMap)
    {
        //ASSERT(pMessageMap != pMessageMap->pBaseMap);
        
        //������Ϣӳ����Һ���,���Ҹ���Ϣ��ӳ����е����
        lpEntry = FindMessageEntry(pMessageMap->lpEntries,message,nCode,nID);
        
        //�ж��ڱ����Ƿ��ҵ�����Ϣ��ӳ��
        if(lpEntry != NULL)
        {
            //�ҵ�����Ϣ��ӳ��,���ò��ҳɹ���־,�˳�ѭ��
            bFound = TRUE;
            break;
        }
    }
	*Result = bFound;
    
    //�ж��Ƿ���ҵ�����Ϣ��ӳ��
    if(bFound)
    {
        /*�ҵ���Ӧ����Ϣӳ��*/
        IsMap = 1;
        union HmiMessageMapFunctions mmf;
        
        //����Ϣ������ָ��ת��Ϊ��Ϣӳ���ʽ
        mmf.pHmifn = lpEntry->pfn;
        
        int nSig = lpEntry->nSig;
        
        //������Ӧ����Ϣ�����ʽ,������Ϣ������
        switch (nSig)
        {
        default:
            //ASSERT(FALSE);
            break;
        case HmiSig_vv:
            (this->*mmf.pHmifn_vv)();
            break;          
        case HmiSig_vw:
            (this->*mmf.pHmifn_vw)(lParam);
            break;          
        case HmiSig_vwdw:
            (this->*mmf.pHmifn_vwdw)(lParam,wParam);
            break;            
        }		
		return TRUE;
    }
    else
    {
        //δ�ҵ�����Ϣ��Ӧ����Ϣӳ�����
        IsMap = 0;
		return FALSE;

        //����ϵͳȱʡ��Ϣ������
//@        OnDefault();
    }
}

LONG CHmiWnd::SendMessage(UINT message, DWORD wParam,LONG lParam)
{
	LONG lRet;
	OnWndMsg(message, wParam, lParam,&lRet);
	return lRet;
}
/*------------------------------------------------------------------------------------------*/
/* ����:���������Ϣӳ�����,������Ϣ��Ӧ����Ϣӳ�����                                     */
/* ����:lpEntry �������Ϣӳ���ָ��,mMsg ��Ҫ���ҵ���Ϣ,nCode ��Ϣ����                     */
/* ���:��                                                                                  */
/* ����:��ӳ����в��ҵ�����Ϣ���ظ���Ϣ��Ӧ����Ϣӳ��,���򷵻�NULL                         */
/*------------------------------------------------------------------------------------------*/
const HMI_MSGMAP_ENTRY* FindMessageEntry(const HMI_MSGMAP_ENTRY* lpEntry,
                                         UINT nMsg, UINT nCode, UINT nID)
{    
    // ������Ϣӳ�������msg��Ϣ��ӳ������
    while (lpEntry->nSig != HmiSig_end)
    {
        //��ǰӳ����Ƿ���ϼ���������
        if (lpEntry->nMessage == nMsg && lpEntry->nCode == nCode &&
			nID >= lpEntry->nID && nID <= lpEntry->nLastID)
        {
            //��������,���ص�ǰ��Ϣ�����
            return lpEntry;
        }

        //�Ƚ���һ��ӳ�����
        lpEntry++;
    }

    //ӳ�����,δ�ҵ���Ӧ����Ϣ���,����NULL
    return NULL;    // not found
}
BOOL CHmiWnd::TransKeyMesssage(WORD wKey)
{
	BOOL val;
	switch(wKey)
	{
	case KEY_UP:
		return OnUp();
		break;
	case KEY_DOWN:
		return OnDown();
		break;
	case KEY_LEFT:
		return OnLeft();
		break;
	case KEY_RIGHT:
		return OnRight();
		break;
	case KEY_ENTER:
		return OnEnter();
		break;
	case KEY_ADD:
		val =  OnAdd();
		if(!val)
		{
			//���ڻҶ�
		}
		
		break;
	case KEY_MINUS:
		val =  OnSub();
		if(!val)
		{
			//���ڻҶ�
		}

		break;
	case KEY_ESCAPE:
		return OnEsc();
		break;
	default:
		break;
	}
	return TRUE;
}

CHmiWnd::CHmiWnd()//DWORD *pAppId)
{ 
	m_pEscWnd = NULL;
	m_pEnterWnd = NULL;
	m_nUpDownOffset = 2;
	m_nLeftRightOffset = 1;
	m_nCurTaborder = 1;
	m_nCurPageIndex = 0;
	m_szCaption[0] = '\0';
	m_bIsClosed = TRUE;
	m_nPageNum = 1;

	m_nX =0;
	m_nY =0;
	m_nXSize = LCD_XSIZE ;
	m_nYSize = LCD_YSIZE;
	m_nObjType = HmiWnd;
	m_bIsShowTitle = TRUE;
}

CHmiWnd::~CHmiWnd()
{

}


/*************************************************************************
* �Ķ���ʷ��¼��

Revision 1.0, 2008-3-3, kangqian
describe: ��ʼ����.
*************************************************************************/

void CHmiWnd::OnTimer(LONG TimerID)
{
		//2010-9-3 ȥ�������Զ��嶨ʱ����Ӧ������ ���Ҫ��ʱˢ�£���ֱ������
		//CHmiWnd��OnTimer ��������
		//�����Զ��嶨ʱ����Ӧ����
	/*	if(m_nFreshTimer>0)
		{
			m_nTimerValue++;
			if (m_nTimerValue >= m_nFreshTimer)
			{
				m_nTimerValue = 0;
				SendMessage(HMI_WND_TIMER, 0, TimerID);
				bNeededRepaint = TRUE;
	
			}
		}
	*/
	//��ʱ�������˵Ļ��ŵ���
	
	if(m_nFreshTimer>0)
	{
		m_nTimerValue++;
		if (m_nTimerValue >= m_nFreshTimer)
		{
			m_nTimerValue = 0;
			OnWndTimer( TimerID);
		}
	}
}
//the wnd is about to be showen
BOOL CHmiWnd::OnShowWindow()
{
	//�Ƿ���Ҫ���³�ʼ����������ڹر��������µ���Ĭ�ϳ�ʼ������
	BOOL bIsNeedInt = m_bIsClosed;
	
	if(bIsNeedInt)
	{
		Init();
	}

	//���Զ����ʼ���������Զ����ʼ������ÿ����ʾ��Ҫ����
	 if(!OnWndInit())
	 	return FALSE;
	

	//ֻ��dataList, datastatic�г�ʼ������
	//������ڹر��˲���Ҫ��ʼ��
	if(!bIsNeedInt)
		return TRUE;

	for(int i=0; i<m_nObjNum; i++)
	{
		if((m_ppObjList[i]->m_nPageIndex != m_nCurPageIndex)||(!m_ppObjList[i]->GetVisible()))
			continue;
		if(!m_ppObjList[i]->Init())
			return FALSE;
	}
	
	m_bIsClosed = FALSE;
	return TRUE;
}

void CHmiWnd::OnPaint()
{
	//2010-9-13 �����Ƴ�����ʱ�� �ų�Ϊ�˻������
	CHmiApp::m_pActiveWnd = this;
	Data_Clear();

	
	//��λͼ
	if(m_bmData != NULL)
		Data_DrawBitmap(m_nX+m_bmLeft, m_nY+m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL, m_bmData, BKCOLORINDEX,COLORINDEX);

	//����,���Ը��ݴ��ڷ������Ƿ�Ҫ������
	if(m_bIsShowTitle) //Ŀǰ�б���λͼ�Ĵ��ڲ�����������
		DrawCaption();

	//��ҳ���ؼ�
	for(int i=0; i<m_nObjNum; i++)
	{
		if((m_ppObjList[i]->m_nPageIndex != m_nCurPageIndex) || (m_ppObjList[i]->GetVisible() != TRUE))
			continue;
		m_ppObjList[i]->OnPaint();
	}

	// 
	char up[3], down[3];
	sprintf(up, "%c", 0x7f); //��
	sprintf(down, "%c", 0x80 );//��

	//����ж�������ʾ��ҳ��ʶ
	//�·�
	if(m_nPageNum > m_nCurPageIndex+1)
	{
		Data_DrawText(m_nX+m_nXSize-16, m_nY+m_nYSize-9, (unsigned char*)down, COLORINDEX, BKCOLORINDEX, 16);
	}
	
	//�Ϸ�
	if(m_nCurPageIndex >0)
	{
		Data_DrawText(m_nX+m_nXSize-16, m_nY+20+1, (unsigned char*)up, COLORINDEX, BKCOLORINDEX, 16);
	}

	//��ǷǷ�
	Invalidate();


}


BOOL CHmiWnd::Init()
{
	return TRUE;
}

char* CHmiWnd::GetClassName()
{
	return m_pszClassName;
} 
void CHmiWnd::SetClassName(char* name)
{
	strcpy(m_pszClassName, name);
}

/****************************************************************************************
* ��������: �����ַ�����Ŀǰװ�ص�����
* ����˵��: 
*			���ڴ����࣬ �����ַ����������ľ��Ǵ����Լ�	
*			
*			- const char* pszSrc Դ�ַ����� ���ݸ��ַ�������Ϊ�������Ե��ַ���
*			- const char* pszComment ��ע��Ĭ��ֵΪNULL��ͨ������Ҫ��
* ���ش���: ����Ŀ�����Ե��ַ���
* ����: 
*****************************************************************************************/
const char* CHmiWnd::tr(const char* pszSrc, const char* pszComment)
{
	if (pszSrc[0] == '\0')
	{
		return pszSrc;
	}
	const char* pDest;
	int nRes;

	pDest = g_Trans.GetTrString(GetClassName(), pszSrc, pszComment,nRes);
		

	if (pDest[0] == '\0')
	{
		return pszSrc;
	}
	else
		return pDest;

//	return pszSrc;
}


void CHmiWnd::SetFocus( CHmiObj* pObj)
{
	if (pObj == NULL)
		return;
	SetFocus(pObj->GetTabOrder());

}

//���ý����ΪnObjIndex�Ŀؼ�Ϊ���㣬
//����ؼ�nObjIndex���ܽ��ܽ��㣬Ѱ����һ���ɽ��ܽ���Ŀؼ�
//�������Ҳ����򱾴��ڵ�һ���ؼ�Ϊ����ؼ�
//ע�⣬ ������Ǵ�1��ʼ��nObjIndex
void CHmiWnd::SetFocus(LONG nTaborder)
{
	if(nTaborder <1)
		nTaborder = 1;
	int nObjIndex = nTaborder -1;
	
	if(m_pActiveObj)
		m_pActiveObj->m_bIsFocus = FALSE;
	if(nObjIndex >= m_nObjNum)
		nObjIndex = 0;
	
	if(m_ppObjList[nObjIndex] != NULL)
	{
		//�жϸÿ����Ƿ���Խ��ܽ���
		//������ѭ��
		int count = m_nObjNum+1;
		m_pActiveObj = m_ppObjList[nObjIndex];
		while(((m_pActiveObj->m_nTabOrder <=0)) 
			|| !m_pActiveObj->GetVisible())
		{
			nObjIndex++;
			nObjIndex %=(m_nObjNum); //��Ŵ�1��ʼ�ģ�
			m_pActiveObj = m_ppObjList[nObjIndex];
			if(count <= 0)
				break;
			count--;
		}
	}
	else
	{
		m_pActiveObj = m_ppObjList[0];
	}
	m_pActiveObj->m_bIsFocus =TRUE;


	m_nCurTaborder = nObjIndex+1;
	if(m_nCurPageIndex != m_pActiveObj->m_nPageIndex)
	{
		m_nCurPageIndex = m_pActiveObj->m_nPageIndex;
	//���ƹ�����Ӧ���������
	//	Data_Clear();
	//	OnPaint();
	//	UpdateWindow();
	}
}



BOOL CHmiWnd::OnAdd()
{
	if(m_pActiveObj != NULL)
	{
		m_pActiveObj->OnAdd();
	}

	OnWndAddSub(1);
	return TRUE;
}

BOOL CHmiWnd::OnSub()
{
	if(m_pActiveObj != NULL)
			m_pActiveObj->OnSub();

	OnWndAddSub(-1);
	return TRUE;
}

BOOL CHmiWnd::OnLeft()
{
	if(m_pActiveObj != NULL)
	{
		if(m_pActiveObj->OnLeft())
			return true;
	}

	if(m_nLeftRightFun == MoveFocus)
	{
		SendMessage(HMI_WND_MOVEFOCUS, 0,  0-m_nLeftRightOffset);
	}
	else if(m_nLeftRightFun == ChgPage)
	{
		SendMessage(HMI_WND_CHGPAGE, 0, -1);
	}

	OnWndLeftRight(-1);
	return true;

}

BOOL CHmiWnd::OnRight()
{
	if(m_pActiveObj != NULL)
	{
		if(m_pActiveObj->OnRight())
		return true;
	}
	if(m_nLeftRightFun == MoveFocus)
	{
		SendMessage(HMI_WND_MOVEFOCUS, 0,  m_nLeftRightOffset);
	}
	else if(m_nLeftRightFun == ChgPage)
	{
		SendMessage(HMI_WND_CHGPAGE, 0, 1);
	}

	OnWndLeftRight(1);

	return true;

}

BOOL CHmiWnd::OnEsc()
{
	if(m_pActiveObj != NULL)
	{

		if(m_pActiveObj->OnEsc())
			return TRUE;
	}

	OnWndEsc();	
	return TRUE;
}

BOOL CHmiWnd::OnEnter()
{
	if(m_pActiveObj != NULL)
	{
		if(m_pActiveObj->OnEnter())
		return TRUE;
	}

	OnWndEnter();

	
	return TRUE;
}
BOOL CHmiWnd::OnUp()
{
	if(m_pActiveObj != NULL)
	{
		if(m_pActiveObj->OnUp())
			return TRUE;
	}

	if(m_nUpDownFun == MoveFocus)
	{
		SendMessage(HMI_WND_MOVEFOCUS, 0,  0-m_nUpDownOffset);
	}
	else if(m_nUpDownFun == ChgPage)
	{
		SendMessage(HMI_WND_CHGPAGE, 0, -1);
	}
	
	OnWndUpDown(-1);

	return TRUE;
}

BOOL CHmiWnd::OnDown()
{

	if(m_pActiveObj != NULL)
	{
		if(m_pActiveObj->OnDown())
			return true;
	}

	if(m_nUpDownFun == MoveFocus)
	{
		SendMessage(HMI_WND_MOVEFOCUS, 0, m_nUpDownOffset);
	}
	else if(m_nUpDownFun == ChgPage)
	{
		SendMessage(HMI_WND_CHGPAGE, 0, 1);
	}

	OnWndUpDown(1);

	return true;
}
void CHmiWnd::Show(DWORD param1, DWORD param2)
{
//	CHmiWnd* pOldActinveWnd ;
//	if(CHmiApp::m_pActiveWnd)
//		pOldActinveWnd = CHmiApp::m_pActiveWnd;
//	else
//		pOldActinveWnd = CHmiApp::m_pMainWnd;
		
//	CHmiApp::m_pActiveWnd = this;
//	if(m_pActiveObj == NULL)
//		m_pActiveObj = m_ppObjList[0];
//	Data_Clear();

//	SendMessage(HMI_WND_SHOWWINDOW);

//��ʼ���ɹ��˲���ʾ���ڣ� �����ʼ��ʧ���ˣ� ����ʾ������
	if(OnShowWindow())
	{
		SendMessage(HMI_WND_PAINT);
		
		//���ڴ���ʾ��������
		UpdateWindow();
		
	}
	else
	{
//		CHmiApp::m_pActiveWnd = pOldActinveWnd;
	}
}
/*void CHmiWnd::Show()
{
	CHmiApp::m_pActiveWnd = this;
	if(m_pActiveObj == NULL)
		m_pActiveObj = m_ppObjList[0];
	Data_Clear();

	//�û������wndinit������ÿ����ʾǰ�����õ�
	SendMessage(HMI_WND_SHOWWINDOW);

	SendMessage(HMI_WND_PAINT);
	m_bIsClosed = FALSE;
	
}*/

//�رմ��ڣ� �رյĵĴ����� �´�Show��ʱ�򻹻����init������û�رյĲ�����
//�������б��Ĵ��ڣ�������ر���ʾ�Ļ���ԭ�������ݣ� �رյĻ��б����ض�
void CHmiWnd::Close()
{
	m_bIsClosed = TRUE;
	
}
LONG CHmiWnd::WndProc(long MsgCode, long wParam, long lParam)
{
	LONG nMsgCode = MsgCode;
	WORD nKey = LOWORD(wParam);
	if(nMsgCode == HMI_KEYDOWN)
	{
		TransKeyMesssage(nKey);
	}
	if(nMsgCode == OS_TIMER)
	{
		SendMessage(HMI_WND_TIMER,wParam);
	}


	return 1;
}

void CHmiWnd::OnMoveFocus(LONG offset)
{
	//�ƶ����㣬�������ؼ����ڱ����ڣ���Ļ����
	//�Ƿ�ɽ��ܽ���ҲҪ�ж�
	LONG nNewFocus;
	nNewFocus = m_nCurTaborder + offset;
	if(nNewFocus <0)
		nNewFocus = 1;
	if(m_pActiveObj)
		m_pActiveObj->SetFocus(false);

	SetFocus(nNewFocus);
	OnPaint();
	UpdateWindow();
}

void CHmiWnd::OnChgPage(LONG offset)
{
	m_nCurPageIndex = m_nCurPageIndex + offset;

	//��Ҫ�ѽ����Ƶ���ҳ�ĵ�һ���ؼ���

	if(m_nCurPageIndex  < 0)
		m_nCurPageIndex = 0;

	m_nCurPageIndex %= m_nPageNum;

	OnPaint();
	UpdateWindow();
}

void CHmiWnd::SetActiveObj(CHmiObj *pObj)
{
	if(m_pActiveObj != NULL)
		m_pActiveObj->m_bIsFocus = FALSE;

	m_pActiveObj = pObj;
	m_pActiveObj->m_bIsFocus = TRUE;
	m_nCurTaborder = abs(m_pActiveObj->GetTabOrder());
}

//���Ʊ��⵽�ڴ�
void CHmiWnd::DrawCaption()
{
	Data_FillSolidRect(m_nX+1,m_nY+1, m_nX+m_nXSize-2, m_nY+20, COLORINDEX);
	if(m_szCaption !=NULL)
		Data_DrawText(m_nX+3, m_nY+3, (unsigned char*)m_szCaption, BKCOLORINDEX, COLORINDEX,16);
	InvalidateRect(m_nX, m_nY, m_nX+m_nXSize-2, m_nY+20);
}

void CHmiWnd::OnWndLeftRight(LONG leftRight)
{
}
void CHmiWnd::OnWndUpDown(LONG UpDown)
{
}
void CHmiWnd::OnWndAddSub(LONG AddSub)
{
}
BOOL CHmiWnd::OnWndInit()
{
	return TRUE;
}
void CHmiWnd::OnWndEnter()
{
	if( (m_pEnterWnd != NULL) )
	{
	
		m_pEnterWnd->Show();
	}
}
void CHmiWnd::OnWndEsc()
{
	if(m_pEscWnd)
	{
		//�رձ�����
		Close(); 
		m_pEscWnd->Show();
	}
}
void CHmiWnd::OnWndTimer(LONG nTimerID)
{
	BOOL bNeededRepaint = FALSE;
	//��ʱˢ�µĿؼ���ʱˢ�£� Ŀǰhmidatastatic�Զ���ʱˢ�µ�
	for(int i=0; i<m_nObjNum; i++)
	{
		if(!m_ppObjList[i]->GetVisible())
			continue;
		if(m_ppObjList[i]->m_nFreshTimer >0 )
		{
			m_ppObjList[i]->m_nTimerValue++;
			if(m_ppObjList[i]->m_nTimerValue == m_ppObjList[i]->m_nFreshTimer)
			{
				m_ppObjList[i]->m_nTimerValue = 0;
				bNeededRepaint = TRUE;
				m_ppObjList[i]->UpdateData(FALSE);
				if(m_ppObjList[i]->m_nPageIndex != m_nCurPageIndex)
					continue;
				m_ppObjList[i]->OnPaint();
			}
		}
	}

	//��Ҫ��Ļˢ��
	if (bNeededRepaint)
	{
		UpdateWindow();
	}	
}
