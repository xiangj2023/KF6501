/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiList.cpp
* 
*   软件模块：实现界面控件列表框功能
*
* 
*	描述：	按照界面控件的使用习惯命名为列表框，但实际使用中插入删除操作频率远小与遍历操作，
*			古内部数据存储结构采用数组，而不是链表	
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-5-13		初始创建
* 
*************************************************************************/



#include "HmiMSG.h"

#include "HmiList.h"
#include "HmiWnd.h"
#include "HmiApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiList::CHmiList()
{
	m_nItemHeight = 18;
	m_nCurItemIndex = 0;
	m_nCurPage = 0;
	m_nYSize = 90;
	m_nItemCount = 0;
	int i;
	for(i=0; i<MAX_LISTITEM; i++)
		m_nItemTxt[i][0] = '\0';
	m_nPageSize = 5;

	for(i=0; i<MAX_LISTCOLMN; i++)
		m_nColPos[i] = 0;
	m_nObjType = HmiList;
	m_pEnterWnd = NULL;
	m_nStatus = 0;
	m_nColumnCount = 1;
	m_nCurColumnIndex = -1;
	m_nCursorWidth = 8;
}

CHmiList::~CHmiList()
{

}

void CHmiList::OnPaint()
{
	int j;
	LONG  count;
	count = GetColumnCount();
	m_nPageSize = m_nYSize/m_nItemHeight;
	LONG pos;

	//清除原来的
	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);

//@	Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize);
//@	Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize);
//@	Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize);
//@	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize);

	//显示内容
	for(int i=0; i< m_nPageSize; i++)
	{
		if(((m_nCurPage*m_nPageSize+i)>=m_nItemCount))
			break;
		for(j=0; j<count; j++)
			Data_DrawText(m_nX+GetColPos(j)*m_nFont/2, m_nY+i*m_nItemHeight, (unsigned char*)GetItemText(m_nCurPage*m_nPageSize+i,j), COLORINDEX, BKCOLORINDEX, m_nFont);
	}
	//焦点反白显示
	if(m_nCurColumnIndex == -1) //滚动条
	{
		if(m_nCurItemIndex >= 0)
			DrawCursor(m_nX, m_nY+m_nCurItemIndex*m_nItemHeight, m_nXSize, m_nItemHeight,1);
	}
	else //某列输入焦点
	{
		pos = GetColPos(m_nCurColumnIndex);
		DrawCursor(m_nX+(pos+m_nCursorPos)*m_nFont/2, m_nY+m_nCurItemIndex*m_nItemHeight, m_nCursorWidth, m_nItemHeight-1,1);		
	}
	Invalidate();
//	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
}

void CHmiList::SetItemTxt(int nItemIndex, char *txt)
{

	if(nItemIndex<0||nItemIndex>MAX_LISTITEM)
		return;
	strcpy((m_nItemTxt[nItemIndex]), txt);
//	memset(&(m_nItemTxt[MAX_LENGTH-1]),0,1);
}


BOOL CHmiList::OnRight()
{
	if(m_nLeftRightFun == MoveFocus)
	{
		OnVScroll(1);
	}
	else if(m_nLeftRightFun == ChgPage)
	{
		OnChgPage(m_nPageSize);
	}
	else if(m_nLeftRightFun == Function)
	{
//@		SendMessage(HMILBN_UPDOWN, m_nTabOrder, 1);
		return TRUE;
	}
	if(m_nLeftRightFun == NoneFun)
		return false;

	//发送LBN_SELCHANGE消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

	return TRUE;

}
BOOL CHmiList::OnLeft()
{
	if(m_nLeftRightFun == MoveFocus)
	{
		OnVScroll(-1);
//@		return TRUE;
	}
	else if(m_nLeftRightFun == ChgPage)
	{
		OnChgPage(0-m_nPageSize);
//@		return TRUE;
	}
	else if(m_nLeftRightFun == Function)
	{
//@		SendMessage(HMILBN_UPDOWN, m_nTabOrder, -1);
		return TRUE;
	}
	if(m_nLeftRightFun == NoneFun)
		return false;


//@	int ItemIndex;
//@	ItemIndex = m_nCurPage*m_nPageSize +m_nCurItemIndex+m_nItemCount-m_nPageSize;
//@	ItemIndex = ItemIndex%m_nItemCount;
//@
//@	m_nCurItemIndex = ItemIndex%m_nPageSize;
//@	m_nCurPage = ItemIndex/m_nPageSize;
//@	OnPaint();

	//发送LBN_SELCHANGE消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

	return TRUE;

}

BOOL CHmiList::OnUp()
{
	if(m_nUpDownFun == MoveFocus)
	{
		OnVScroll(-1);
	}
	else if(m_nUpDownFun == ChgPage)
	{
		OnVScroll(0-m_nPageSize);
	}
	else if(m_nUpDownFun == NoneFun)
		return false;

//@	int ItemIndex;
//@	ItemIndex = m_nCurPage*m_nPageSize +m_nCurItemIndex+m_nItemCount-1;
//@	ItemIndex = ItemIndex%m_nItemCount;
//@
//@	m_nCurItemIndex = ItemIndex%m_nPageSize;
//@	m_nCurPage = ItemIndex/m_nPageSize;
//@	OnPaint();
//@
	//发送LBN_SELCHANGE消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

	return TRUE;

}
BOOL CHmiList::OnDown()
{
	if(m_nUpDownFun == MoveFocus)
	{
		OnVScroll(1);
	}
	else if(m_nUpDownFun == ChgPage)
	{
		OnVScroll(m_nPageSize);
	}
	else 
	if(m_nUpDownFun == NoneFun)
		return false;

//@	int ItemIndex;
//@	ItemIndex = m_nCurPage*m_nPageSize +m_nCurItemIndex+1;
//@	ItemIndex = ItemIndex%m_nItemCount;
//@
//@	m_nCurItemIndex = ItemIndex%m_nPageSize;
//@	m_nCurPage = ItemIndex/m_nPageSize;
//@	
//@	OnPaint();
	//发送LBN_SELCHANGE消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

	return TRUE;

}
void CHmiList::OnChgPage(LONG offset)
{
	if(m_nItemCount == 0)
		return;
	if(m_nItemCount == 0)
		return;
	int ItemIndex;
	if(offset>0)
		m_nCurItemIndex = m_nPageSize; //焦点到下一页的第0行
	else
		m_nCurItemIndex = 0 - m_nPageSize;

	ItemIndex = m_nCurPage*m_nPageSize +m_nCurItemIndex;

	//翻页都是翻到页的开始一行
	if(ItemIndex >= m_nItemCount)
		ItemIndex = 0;

	
	if(ItemIndex < 0)
		ItemIndex = 0;

	m_nCurItemIndex = ItemIndex%m_nPageSize;
	m_nCurPage = ItemIndex/m_nPageSize;
	
	OnPaint();
	UpdateWindow();




}

void CHmiList::OnVScroll(LONG offset)
{
	if(m_nItemCount == 0)
		return;
	int ItemIndex;
	ItemIndex = m_nCurPage*m_nPageSize +m_nCurItemIndex+offset;
	ItemIndex = ItemIndex%m_nItemCount;
	
	if(ItemIndex < 0)
		ItemIndex = 0;

	m_nCurItemIndex = ItemIndex%m_nPageSize;
	m_nCurPage = ItemIndex/m_nPageSize;
	
	OnPaint();
	UpdateWindow();
}
BOOL CHmiList::OnEnter()
{
	//发送LBN_DBLCLK消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_ENTER);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);


//@	//2009-3-6 11:19:58去掉回車直接跳轉窗口
//@	//2.跳转到新窗口	
//@	if(m_pEnterWnd != NULL)
//@	{
//@		CHmiApp::m_pActiveWnd = m_pEnterWnd;
//@		m_pEnterWnd->Show(0,0);
//@		return TRUE;
//@	}
	return FALSE;
}

void CHmiList::SetSize(LONG x, LONG y, LONG xSize, LONG ySize)
{
	CHmiObj::SetSize(x, y, xSize, ySize);
	m_nPageSize = ySize/m_nItemHeight;
}

void CHmiList::FillData()
{

}
void CHmiList::DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1,LONG cursorMode)
{
	if(cursorMode == 0) //下话线
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //反黑
	{
		Data_XorRect(x0, y0, x1, y1);
	}
}

LONG CHmiList::AddString(const char* pszItem)
{
	LONG index = m_nItemCount;
	strcpy(m_nItemTxt[m_nItemCount], pszItem);
	m_nItemCount++;
	m_nItemCount %= MAX_LISTITEM;
	return index;
}
LONG CHmiList::GetCount( ) const
{
	return m_nItemCount;
}
LONG CHmiList::GetTopIndex( ) const
{
	return -1;
}
LONG CHmiList::SetTopIndex( int nIndex )
{
	return nIndex;
}
//@	int GetSel( int nIndex ) const; //目前只支持单选，不支持多选
LONG CHmiList::GetTextLen(LONG nIndex ) const
{
	return -1;
}
LONG CHmiList::GetCurSel( ) const
{
	return m_nCurItemIndex + m_nCurPage*m_nPageSize;
}
LONG CHmiList::SetCurSel(LONG nSelect )
{
	m_nCurItemIndex = nSelect%m_nPageSize;
	m_nCurPage = nSelect/m_nPageSize;

	return 0;
}
LONG CHmiList::InsertString(LONG nIndex, char* lpszItem )
{
	return -1;
}
LONG CHmiList::DeleteString( UINT nIndex )
{
	return -1;
}
LONG CHmiList::FindString(LONG nStartAfter, char* lpszItem ) const
{
	return -1;
}
LONG CHmiList::SelectString(LONG nStartAfter, char* lpszItem )
{
	return -1;
}
LONG CHmiList::InitStorage (LONG nItems, LONG nBytes )
{
	return -1;
}
void CHmiList::ResetContent( )
{
	m_nCurItemIndex = -1;
	m_nItemCount = 0;
	m_nCurPage = 0;
}
LONG CHmiList::SetItemData(LONG nItem, LONG nData)
{
	if(nItem<0||nItem>MAX_LISTITEM)
		return -1;
	m_nItemData[nItem] = nData;
	return 0;
}
LONG CHmiList::GetItemData(LONG nItem)
{
	return m_nItemData[nItem];
}


void CHmiList::SetItemText(LONG nItem, LONG nSubItem, char *txt)
{
	LONG pos = GetColPos(nSubItem);

	strcpy(&(m_nItemTxt[nItem][pos]), txt);
}

char* CHmiList::GetItemText(LONG nItem, LONG nSubItem)
{
	LONG pos = GetColPos(nSubItem);
	return &(m_nItemTxt[nItem][pos]);

}
void CHmiList::SetColPos(LONG col, LONG pos)
{
	if(col>MAX_LISTCOLMN)
		return;
	m_nColPos[col] = pos;
}
LONG CHmiList::GetColPos(LONG col)
{
	return  m_nColPos[col];
}
void CHmiList::SelectItem(LONG nItem, LONG nSubItem)
{
	SetCurSel(nItem);
	m_nCurColumnIndex = nSubItem;
	m_nCursorPos = 0;
	OnPaint();
	UpdateWindow();
}
