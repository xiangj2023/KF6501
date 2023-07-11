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

#ifdef LIST_SHARE_SPACE
TListItem CHmiList::m_tItem[MAX_LISTITEM]; 
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiList::CHmiList()
{
	m_nCursorType = CusorHighLight; //���ڹ��
	m_nItemHeight = 18;
	m_nCurItemIndex = 0;
	m_nCurPage = 0;
	m_nYSize = 90;
	m_nItemCount = 0;
	m_bHaveSelChgFun = FALSE;
	int i,j;
	m_nEditableColumn = -1;
	for(i=0; i<MAX_LISTITEM; i++)
		for (j=0; j<MAX_LISTCOLMN; j++)
			strcpy(m_tItem[i].szColumnTxt[j], "\0");

//		m_nItemTxt[i][0] = '\0';
	m_nPageSize = 5;

	for(i=0; i<MAX_LISTCOLMN; i++)
		m_nColPos[i] = 0;
	m_nObjType = HmiList;
	m_pEnterWnd = NULL;
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
		//��ʾһ��
		for(j=0; j<m_nColumnCount; j++)
			Data_DrawText(m_nX+GetColPos(j)*m_nFont/2, m_nY+i*m_nItemHeight, (unsigned char*)GetItemText(m_nCurPage*m_nPageSize+i,j), COLORINDEX, BKCOLORINDEX, m_nFont);
	}
	//���㷴����ʾ
	if(m_nCurColumnIndex == -1) //������
	{
		if(m_nCurItemIndex >= 0)
			DrawCursor(m_nX, m_nY+m_nCurItemIndex*m_nItemHeight, m_nXSize, m_nItemHeight);
	}
	else //ĳ�����뽹��
	{
		pos = GetColPos(m_nCurColumnIndex);
		DrawCursor(m_nX+(pos+m_nCursorPos)*m_nFont/2, m_nY+m_nCurItemIndex*m_nItemHeight, m_nCursorWidth, m_nItemHeight-1);		
	}

//@	//���߿�
//@	Data_DrawHLine(m_nX-1, m_nY-1, m_nX+m_nXSize); //��
//@//	Data_DrawHLine(m_nX-1, m_nY+m_nYSize,m_nX+m_nXSize); //��
//@	Data_DrawHLine(m_nX-1, m_nY+m_nYSize+1,m_nX+m_nXSize+1); //��
//@	
//@	Data_DrawVLine(m_nX-1, m_nY, m_nY+m_nYSize); //��
//@//	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize); //��
//@	Data_DrawVLine(m_nX+m_nXSize+1, m_nY, m_nY+m_nYSize); //��

	Invalidate();
//	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
}

void CHmiList::SetItemTxt(int nItemIndex, char *txt)
{

	if(nItemIndex<0||nItemIndex>MAX_LISTITEM)
		return;
	strcpy(m_tItem[nItemIndex].szColumnTxt[0], txt);
//	strcpy((m_nItemTxt[nItemIndex]), txt);
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
BOOL CHmiList::OnEsc()
{
	if (m_nCurColumnIndex != -1)
	{
		m_nCurColumnIndex = -1;
		OnPaint();
		UpdateWindow();
		return TRUE;
	}
	return FALSE;
}

BOOL CHmiList::OnEnter()
{
	//2010-9-9 ��ͨ���س���������༭̬�� ֻͨ���Ӽ�������༭̬
	/*
	//�пɱ༭�����ҵ�ǰ���Ǳ༭̬
	if ((m_nEditableColumn != -1)&&(m_nCurColumnIndex == -1))
	{
		m_nCurColumnIndex = m_nEditableColumn;
		OnPaint();
		UpdateWindow();
		return TRUE;
	}*/

	//����LBN_DBLCLK��Ϣ
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMILIST_ENTER);
	//�д�����
	if(m_pOwner->SendMessage(HMIWM_COMMAND,wParam))
		return TRUE;


//@	//2009-3-6 11:19:58ȥ����܇ֱ�����D����,
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

void CHmiList::DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1)
{
	if(m_nCursorType  == CusorUnderLine) //�»���
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //����
	{
		Data_XorRect(x0, y0, x1, y1);
	}
}
 
/****************************************************************************************
 * 
 * ��������: �����б��е�ǰѡ�����ֵ
 * 
 * ����˵��: 
 *			- LONG offset 
 *			  ����: 
 *			  ˵��: ֵ������
 *
 *			- char strList[][32]
 *			  ����: 
 *			  ˵��: ��ʾ���飬 �ò���Ϊnullʱֱ����ʾ��ֵ�� ��nullʱ��ʾ�ַ���
 *
 *			- LONG num
 *			  ����: 
 *			  ˵��: strList[][32]����Ч���ݸ���
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
void CHmiList::OnChgValue(LONG offset, char strList[][32], LONG num)
{
	LONG row = GetCurSel();
	LONG col = GetCurCol();
//	LONG pos = GetColPos(1);
	char str[64];
	strcpy(str, GetItemText(row,col));
	int val;
	int i;

	val = str[m_nCursorPos];
	if ((strList != NULL) && (num >0))
	{
		for (i=0; i<num; i++)
		{
			if (strcmp(CHmiDataList::tr(strList[i]), str) == 0)
				break;
		}
		i = i + offset + num;
		i = i%num;

		strcpy(str,CHmiDataList::tr(strList[i]));
	}
	else if ((val <='9') && (val >='0')) 
	{
		val = val + offset -'0' +10; // -'0'�ַ�������� +10��ֹ�����ɸ���
		val = val%10;
		str[m_nCursorPos] = val +'0'; //���ֱ���ַ�
	}
	else if (val == '-')
	{
		val = '+';
		str[m_nCursorPos] = val;
	}
	else if (val == '+')
	{
		val = '-';
		str[m_nCursorPos] = val;
	}

	SetItemText(row,col, str);
}
LONG CHmiList::SetEditableColum(LONG nCol, LONG nMaxLen, BOOL bIsShowNagetive)
{
	m_nEditableColumn = nCol;
	m_nMaxLenEditable = nMaxLen;
	m_bIsShowNegative = bIsShowNagetive;
	return nCol;
}

LONG CHmiList::AddRow(const char* pszItem)
{
	LONG index = m_nItemCount;
//	strcpy(m_nItemTxt[m_nItemCount], pszItem);
	strcpy(m_tItem[m_nItemCount].szColumnTxt[0], pszItem);
	m_nItemCount++;
	m_nItemCount %= MAX_LISTITEM;
	return index;
}
LONG CHmiList::GetCount( ) const
{
	return m_nItemCount;
}
LONG CHmiList::GetCurSel( ) const
{
	return m_nCurItemIndex + m_nCurPage*m_nPageSize;
}
LONG CHmiList::GetCurCol()
{
	return m_nCurColumnIndex;
}
LONG CHmiList::SetCurSel(LONG nSelect )
{
	m_nCurItemIndex = nSelect%m_nPageSize;
	m_nCurPage = nSelect/m_nPageSize;

	return 0;
}
void  CHmiList::SetCurCol(LONG nColumn)
{
	m_nCurColumnIndex  = nColumn;
}
void CHmiList::ResetContent( )
{
	m_nItemCount = 0;
	m_nCurPage = 0;
	m_nCurItemIndex =0;
	m_nCurColumnIndex = -1;
	m_nCursorPos = 0; 
	m_nColumnCount = 1;
}
LONG CHmiList::SetItemData(LONG nItem, LONG nData)
{
	m_tItem[nItem].nData = nData;

//@	if(nItem<0||nItem>MAX_LISTITEM)
//@		return -1;
//@	m_nItemData[nItem] = nData;
	return 0;
}
LONG CHmiList::GetItemData(LONG nItem)
{
	nItem %= MAX_LISTITEM;
	return m_tItem[nItem].nData;
//	return m_nItemData[nItem];
}


void CHmiList::SetItemText(LONG nItem, LONG nSubItem, char *txt)
{
	strcpy(m_tItem[nItem].szColumnTxt[nSubItem], txt);

//@	LONG pos = GetColPos(nSubItem);
//@
//@	strcpy(&(m_nItemTxt[nItem][pos]), txt);
}

char* CHmiList::GetItemText(LONG nItem, LONG nSubItem)
{
	nItem %= MAX_LISTITEM;
	nSubItem %= MAX_LISTCOLMN;
	return m_tItem[nItem].szColumnTxt[nSubItem];
//@	LONG pos = GetColPos(nSubItem);
//@	return &(m_nItemTxt[nItem][pos]);

}
void CHmiList::SetColPos(LONG col, LONG pos)
{
	if(col>MAX_LISTCOLMN)
		return;
	m_nColPos[col] = pos;
}
LONG CHmiList::GetColPos(LONG col)
{
	col %= MAX_LISTCOLMN;
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
