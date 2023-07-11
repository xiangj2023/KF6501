/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiCombo.cpp
* 
*   ���ģ�飺ʵ�ֽ���ؼ�Combo
*
* 
*	������	���ս���ؼ���ʹ��ϰ������ΪCombo����ʵ��ʹ���в���ɾ������Ƶ��ԶС�����������
*			
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

#include "HmiCombo.h"
#include "HmiWnd.h"
#include "HmiApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiCombo::CHmiCombo()
{
	m_nCursorType = CusorHighLight; //�»��߹��

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

	//���ԭ����
	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);

//@	Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize);
//@	Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize);
//@	Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize);
//@	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize);

	//��ʾ����
	Data_DrawText(m_nX, m_nY, (unsigned char*)GetItemText(m_nCurItemIndex), COLORINDEX, BKCOLORINDEX, m_nFont);
	//���㷴����ʾ
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
	//����LBN_SELCHANGE��Ϣ
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

	//����LBN_SELCHANGE��Ϣ
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
	//����LBN_DBLCLK��Ϣ
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
	if(cursorMode == 0) //�»���
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //����
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
