/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiCombo.cpp
* 
*   软件模块：实现界面控件Combo
*
* 
*	描述：	按照界面控件的使用习惯命名为Combo，但实际使用中插入删除操作频率远小与遍历操作，
*			
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

#include "HmiCombo.h"
#include "HmiWnd.h"
#include "HmiApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiCombo::CHmiCombo()
{
	m_nCursorType = CusorHighLight; //下划线光标

	m_nCurItemIndex = 0;
	m_nYSize = 90;
	m_nItemCount = 0;
	int i;
	for(i=0; i<MAX_LISTITEM; i++)
		m_nItemTxt[i][0] = '\0';

	m_nObjType = HmiList;
	m_pEnterWnd = NULL;
	m_nCursorWidth = 8;
	m_nFont = 16;
}

CHmiCombo::~CHmiCombo()
{

}

void CHmiCombo::OnPaint()
{

	//清除原来的
	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);

//@	Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize);
//@	Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize);
//@	Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize);
//@	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize);

	//显示内容
	Data_DrawText(m_nX, m_nY, (unsigned char*)GetItemText(m_nCurItemIndex), COLORINDEX, BKCOLORINDEX, m_nFont);
	//焦点反白显示
//	(m_nX+m_nCursorPos*m_nFont/2,m_nY+m_nFont, m_nX+m_nCursorPos*m_nFont/2+m_nFont/2-1, 0 );

	if(m_bIsFocus)
		DrawCursor(m_nX, m_nY, m_nXSize+m_nX, m_nYSize+m_nY,1);
	
	Invalidate();
//	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
}

void CHmiCombo::SetItemTxt(int nItemIndex, char *txt)
{

	if(nItemIndex<0||nItemIndex>MAX_LISTITEM)
		return;
	txt[MAX_LENGTH-1] = '/0';
	strcpy((m_nItemTxt[nItemIndex]), txt);
}

BOOL CHmiCombo::OnUp()
{
	return false;
		OnVScroll(-1);

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
BOOL CHmiCombo::OnDown()
{
	return false;
	{
		OnVScroll(1);
	}

	//发送LBN_SELCHANGE消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_SELCHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

	return TRUE;
}
BOOL CHmiCombo::OnAdd()
{
	OnVScroll(1);
	return TRUE;
}
BOOL CHmiCombo::OnSub()
{
	OnVScroll(-1);
	return TRUE;
}


void CHmiCombo::OnVScroll(LONG offset)
{
	if(m_nItemCount == 0)
		return;
	m_nCurItemIndex += offset;
	if(m_nCurItemIndex < 0)
		m_nCurItemIndex += m_nItemCount;

	m_nCurItemIndex = m_nCurItemIndex %m_nItemCount;
	
	OnPaint();
	UpdateWindow();
}
BOOL CHmiCombo::OnEnter()
{
	//发送LBN_DBLCLK消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_ENTER);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

	return FALSE;
}

void CHmiCombo::SetSize(LONG x, LONG y, LONG xSize, LONG ySize)
{
	CHmiObj::SetSize(x, y, xSize, ySize);
}

void CHmiCombo::DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1,LONG cursorMode)
{
	if(cursorMode == 0) //下话线
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //反黑
	{
		Data_XorRect(x0, y0, x1-x0, y1-y0);
	}
}

LONG CHmiCombo::AddString(const char* pszItem)
{
	LONG index = m_nItemCount;
	strcpy(m_nItemTxt[m_nItemCount], pszItem);
	m_nItemCount++;
	m_nItemCount %= MAX_LISTITEM;
	return index;
}
LONG CHmiCombo::GetCount( ) const
{
	return m_nItemCount;
}
LONG CHmiCombo::GetCurSel( ) const
{
	return m_nCurItemIndex ;
}
LONG CHmiCombo::SetCurSel(LONG nSelect )
{
	m_nCurItemIndex = nSelect%m_nItemCount;
	return 0;
}

void CHmiCombo::ResetContent( )
{
	m_nCurItemIndex = 0;
	m_nItemCount = 0;
}
LONG CHmiCombo::SetItemData(LONG nItem, LONG nData)
{
	if(nItem<0||nItem>MAX_LISTITEM)
		return -1;
	m_nItemData[nItem] = nData;
	return 0;
}
LONG CHmiCombo::GetItemData(LONG nItem)
{
	return m_nItemData[nItem];
}


char* CHmiCombo::GetItemText(LONG nItem)
{
	return m_nItemTxt[nItem];

}
void CHmiCombo::SelectItem(LONG nItem)
{
	SetCurSel(nItem);
//	OnPaint();
//	UpdateWindow();
}
