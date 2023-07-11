/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiList.cpp
* 
*   ���ģ�飺ʵ�ֽ���ؼ��б����
*
* 
*	������	���ս���ؼ���ʹ��ϰ������Ϊ�б�򣬵�ʵ��ʹ���в���ɾ������Ƶ��ԶС�����������
*			���ڲ����ݴ洢�ṹ�������飬����������	
* 
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-5-13		��ʼ����
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

	//���ԭ����
	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);

//@	Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize);
//@	Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize);
//@	Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize);
//@	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize);

	//��ʾ����
	for(int i=0; i< m_nPageSize; i++)
	{
		if(((m_nCurPage*m_nPageSize+i)>=m_nItemCount))
			break;
		for(j=0; j<count; j++)
			Data_DrawText(m_nX+GetColPos(j)*m_nFont/2, m_nY+i*m_nItemHeight, (unsigned char*)GetItemText(m_nCurPage*m_nPageSize+i,j), COLORINDEX, BKCOLORINDEX, m_nFont);
	}
	//���㷴����ʾ
	if(m_nCurColumnIndex == -1) //������
	{
		if(m_nCurItemIndex >= 0)
			DrawCursor(m_nX, m_nY+m_nCurItemIndex*m_nItemHeight, m_nXSize, m_nItemHeight,1);
	}
	else //ĳ�����뽹��
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

	//����LBN_SELCHANGE��Ϣ
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

	//����LBN_SELCHANGE��Ϣ
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
	//����LBN_SELCHANGE��Ϣ
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
	//����LBN_SELCHANGE��Ϣ
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
		m_nCurItemIndex = m_nPageSize; //���㵽��һҳ�ĵ�0��
	else
		m_nCurItemIndex = 0 - m_nPageSize;

	ItemIndex = m_nCurPage*m_nPageSize +m_nCurItemIndex;

	//��ҳ���Ƿ���ҳ�Ŀ�ʼһ��
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
	//����LBN_DBLCLK��Ϣ
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_ENTER);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);


//@	//2009-3-6 11:19:58ȥ����܇ֱ�����D����
//@	//2.��ת���´���	
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
	if(cursorMode == 0) //�»���
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //����
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
//@	int GetSel( int nIndex ) const; //Ŀǰֻ֧�ֵ�ѡ����֧�ֶ�ѡ
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
