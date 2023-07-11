/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：HmiWnd.cpp
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-3-3
*   作    者：kangqian
*   功    能：<...>
*   修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-8-11		初始创建
* 		kangqian	2010-9-14		修改关于m_ppObjList和具体窗口类空间中
*									m_pObjectlist的关联，具体窗口类中m_pObjectlist
*									定义为控件数目+1的数组， 并保留最后一个条目指向空
*									这样在窗口没有控件的时候也不会出错，控件的taborder
*									号为从1开始的，这样如果有不能接受输入焦点的控件，第一个是-1，而不是从0开始
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
    //基类的消息映射入口
    NULL,

    //自身的消息映射入口
    &CHmiWnd::_messageEntries[0]
};

//取自身消息映射表入口
const HMI_MSGMAP* CHmiWnd::GetMessageMap() const 
{
    return &CHmiWnd::messageMap;
}
//@#define ON_HMIWM_PAINT() \
//@{ WM_HMIPAINT,0, 0,0, Hmisig_vv, ((HMI_PMSG)(void (CHmiWnd::*)(void))&OnPaint},
//@#define ON_HMIWM_INIT() \


//定义基类的消息映射
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
/* 功能:任务消息调度函数，根据接收的消息，自动调度相应的消息处理函数                        */
/* 输入:msg：任务接收到的消息                                                               */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/
/****************************************************************************************
 * 
 * 功能描述: 
 * 
 * 参数说明: 
 *			- UINT message
 *			  类型: 
 *			  说明: 任务接收到的消息的消息号（窗口的消息有HMI_WND_INIT，HMI_WND_ENTER。。控件消息为HMIWM_COMMAND）
 *
 *			- DWORD wParam
 *			  类型: 
 *			  说明: 参数dword型参数，控件消息使用该参数， 消息号（如HMIBTN_ENTER，HMILIST_ENTER等和控件Id拼成
 *
 *			- LONG lParam
 *			  类型: 
 *			  说明: 调用的函数使用的参数
 *
 *			- LONG *Result
 *			  类型: 
 *			  说明: 
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
BOOL CHmiWnd::OnWndMsg(UINT message, DWORD wParam, LONG lParam, LONG *Result)
{
    int  IsMap;
    const HMI_MSGMAP_ENTRY* lpEntry;
    BOOL bFound = FALSE;
	WORD nCode = HIWORD(wParam);
	WORD nID = LOWORD(wParam);

   //取当前应用程序类的消息映射表
    const HMI_MSGMAP* pMessageMap = GetMessageMap();
    
    //遍历本应用程序类及其所有基类的消息映射表,查找msg消息的调度函数
    for ( ; pMessageMap != NULL;pMessageMap = pMessageMap->pBaseMap)
    {
        //ASSERT(pMessageMap != pMessageMap->pBaseMap);
        
        //调用消息映射查找函数,查找该消息在映射表中的入口
        lpEntry = FindMessageEntry(pMessageMap->lpEntries,message,nCode,nID);
        
        //判断在本类是否找到该消息的映射
        if(lpEntry != NULL)
        {
            //找到该消息的映射,设置查找成功标志,退出循环
            bFound = TRUE;
            break;
        }
    }
	*Result = bFound;
    
    //判断是否查找到该消息的映射
    if(bFound)
    {
        /*找到相应的消息映射*/
        IsMap = 1;
        union HmiMessageMapFunctions mmf;
        
        //把消息处理函数指针转换为消息映射格式
        mmf.pHmifn = lpEntry->pfn;
        
        int nSig = lpEntry->nSig;
        
        //依据相应的消息处理格式,调用消息处理函数
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
        //未找到该消息对应的消息映射入口
        IsMap = 0;
		return FALSE;

        //调用系统缺省消息处理函数
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
/* 功能:从任务的消息映射表中,查找消息对应的消息映射入口                                     */
/* 输入:lpEntry 任务的消息映射表指针,mMsg 需要查找的消息,nCode 消息参数                     */
/* 输出:无                                                                                  */
/* 返回:在映射表中查找到该消息返回该消息对应的消息映射,否则返回NULL                         */
/*------------------------------------------------------------------------------------------*/
const HMI_MSGMAP_ENTRY* FindMessageEntry(const HMI_MSGMAP_ENTRY* lpEntry,
                                         UINT nMsg, UINT nCode, UINT nID)
{    
    // 遍历消息映射表，查找msg消息的映射表入口
    while (lpEntry->nSig != HmiSig_end)
    {
        //当前映射表是否符合检索的条件
        if (lpEntry->nMessage == nMsg && lpEntry->nCode == nCode &&
			nID >= lpEntry->nID && nID <= lpEntry->nLastID)
        {
            //符合条件,返回当前消息表入口
            return lpEntry;
        }

        //比较下一个映射表项
        lpEntry++;
    }

    //映射表内,未找到对应的消息入口,返回NULL
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
			//调节灰度
		}
		
		break;
	case KEY_MINUS:
		val =  OnSub();
		if(!val)
		{
			//调节灰度
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
* 改动历史纪录：

Revision 1.0, 2008-3-3, kangqian
describe: 初始创建.
*************************************************************************/

void CHmiWnd::OnTimer(LONG TimerID)
{
		//2010-9-3 去掉窗口自定义定时器响应函数， 如果要定时刷新，可直接重载
		//CHmiWnd的OnTimer 函数即可
		//窗口自定义定时器响应函数
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
	//定时器启动了的话才调用
	
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
	//是否需要重新初始化，如果窗口关闭了则重新调用默认初始化函数
	BOOL bIsNeedInt = m_bIsClosed;
	
	if(bIsNeedInt)
	{
		Init();
	}

	//有自定义初始化函数，自定义初始化函数每次显示都要调用
	 if(!OnWndInit())
	 	return FALSE;
	

	//只有dataList, datastatic有初始化函数
	//如果窗口关闭了才需要初始化
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
	//2010-9-13 当绘制出来的时候， 才成为了活动窗口了
	CHmiApp::m_pActiveWnd = this;
	Data_Clear();

	
	//画位图
	if(m_bmData != NULL)
		Data_DrawBitmap(m_nX+m_bmLeft, m_nY+m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL, m_bmData, BKCOLORINDEX,COLORINDEX);

	//标题,可以根据窗口风格决定是否要标题栏
	if(m_bIsShowTitle) //目前有背景位图的窗口不划标题栏了
		DrawCaption();

	//本页各控件
	for(int i=0; i<m_nObjNum; i++)
	{
		if((m_ppObjList[i]->m_nPageIndex != m_nCurPageIndex) || (m_ppObjList[i]->GetVisible() != TRUE))
			continue;
		m_ppObjList[i]->OnPaint();
	}

	// 
	char up[3], down[3];
	sprintf(up, "%c", 0x7f); //上
	sprintf(down, "%c", 0x80 );//下

	//如果有多屏，显示翻页标识
	//下翻
	if(m_nPageNum > m_nCurPageIndex+1)
	{
		Data_DrawText(m_nX+m_nXSize-16, m_nY+m_nYSize-9, (unsigned char*)down, COLORINDEX, BKCOLORINDEX, 16);
	}
	
	//上翻
	if(m_nCurPageIndex >0)
	{
		Data_DrawText(m_nX+m_nXSize-16, m_nY+20+1, (unsigned char*)up, COLORINDEX, BKCOLORINDEX, 16);
	}

	//标记非法
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
* 功能描述: 翻译字符串到目前装载的语言
* 参数说明: 
*			对于窗口类， 翻译字符串的上下文就是窗口自己	
*			
*			- const char* pszSrc 源字符串， 根据该字符串翻译为其他语言的字符串
*			- const char* pszComment 备注，默认值为NULL，通常不需要填
* 返回代码: 返回目标语言的字符串
* 其它: 
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

//设置焦点号为nObjIndex的控件为焦点，
//如果控件nObjIndex不能接受焦点，寻找下一个可接受焦点的控件
//遍历都找不到则本窗口第一个控件为焦点控件
//注意， 焦点号是从1开始的nObjIndex
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
		//判断该控制是否可以接受焦点
		//避免死循环
		int count = m_nObjNum+1;
		m_pActiveObj = m_ppObjList[nObjIndex];
		while(((m_pActiveObj->m_nTabOrder <=0)) 
			|| !m_pActiveObj->GetVisible())
		{
			nObjIndex++;
			nObjIndex %=(m_nObjNum); //序号从1开始的，
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
	//绘制工作不应该在这完成
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

//初始化成功了才显示窗口， 如果初始化失败了， 不显示窗口了
	if(OnShowWindow())
	{
		SendMessage(HMI_WND_PAINT);
		
		//从内存显示到物理屏
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

	//用户定义的wndinit函数是每次显示前都调用的
	SendMessage(HMI_WND_SHOWWINDOW);

	SendMessage(HMI_WND_PAINT);
	m_bIsClosed = FALSE;
	
}*/

//关闭窗口， 关闭的的窗口再 下次Show的时候还会调用init函数，没关闭的不调用
//比如有列表框的窗口，如果不关闭显示的还是原来的内容， 关闭的话列表框会重读
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
	//移动焦点，如果焦点控件不在本屏内，屏幕翻动
	//是否可接受焦点也要判断
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

	//还要把焦点移到本页的第一个控件上

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

//绘制标题到内存
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
		//关闭本窗口
		Close(); 
		m_pEscWnd->Show();
	}
}
void CHmiWnd::OnWndTimer(LONG nTimerID)
{
	BOOL bNeededRepaint = FALSE;
	//定时刷新的控件定时刷新， 目前hmidatastatic自动定时刷新的
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

	//需要屏幕刷新
	if (bNeededRepaint)
	{
		UpdateWindow();
	}	
}
