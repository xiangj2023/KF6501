/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   �ļ����ƣ�HmiButton.cpp
*   ���ģ�飺<...>
*   �� �� �ţ�1.0
*   �������ڣ�2008-3-3
*   ��    �ߣ�kangqian
*   ��    �ܣ�<��ť�ؼ�ʵ��>
* 
*************************************************************************/

// HmiButton.cpp: implementation of the CHmiButton class.
//
//////////////////////////////////////////////////////////////////////
#include "HmiWnd.h"
#include "HmiButton.h"
#include "HmiApp.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiButton::CHmiButton()
{
//	m_nCursorType = CusorUnderLine; //�»��߹��
	m_nCursorType =  CusorHighLight; //���ڹ��

	m_szCaption[0] = '\0';
	m_szCaption[0] = '\0';

	m_pEnterWnd = NULL;
	m_bHaveEnterFun = FALSE;
	m_nObjType = HmiButton;
	m_bIsAutoEnterWnd = TRUE;
}

/*************************************************************************
* 
*   ��������     
* 
*   ����˵����// ����: LONG x
// ����: LONG y
// ����: LONG xsize
// ����: LONG ysize): CHmiObj(x
// ����: y
// ����: xsize
// ����: ysize
// ����: 
*/
CHmiButton::CHmiButton(LONG x, LONG y, LONG xsize, LONG ysize)
	: CHmiObj(x,y,xsize, ysize)
{
	m_szCaption[0] = '\0';

	m_pEnterWnd = NULL;
}

CHmiButton::~CHmiButton()
{

}


BOOL CHmiButton::OnEnter()
{
	BOOL bResult;
	if(m_bHaveEnterFun||(m_pEnterWnd != NULL))
		bResult = TRUE;
	else 
		bResult = FALSE;
		
	//Ϊ���������Ч�ʶ�����m_bHaveEnterFun�� ����ÿ�ζ�����Ϣ�� ���������б�
	if(m_bHaveEnterFun)
	{
		//1.֪ͨ�����ڷ���click��Ϣ
		DWORD wParam;
		wParam = MAKELONG(m_nTabOrder, HMIBTN_ENTER);
		
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}


	//2.��ת���´���	
	if((m_pEnterWnd != NULL)&& m_bIsAutoEnterWnd)
	{
//		CHmiApp::m_pActiveWnd = m_pEnterWnd;
		m_pEnterWnd->Show();
	}
	return bResult;

}
BOOL CHmiButton::OnLeft()
{

	return false;
}

BOOL CHmiButton::OnRight()
{

	return false;
}
BOOL CHmiButton::OnUp()
{

	return false;
}
BOOL CHmiButton::OnDown()
{
	return false;
}

void CHmiButton::OnPaint()
{
//@	if(!m_bIsFocus)
//@	{
		//����ԭ��
		Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);
		//���߿�
		Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize); //��
		Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize); //��
		Data_DrawHLine(m_nX, m_nY+m_nYSize+1,m_nX+m_nXSize+1); //��

		Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize); //��
		Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize); //��
		Data_DrawVLine(m_nX+m_nXSize+1, m_nY, m_nY+m_nYSize); //��

		//��λͼ
		if(m_bmData != NULL)
			Data_DrawBitmap(m_nX+m_bmLeft, m_nY+m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL, m_bmData, BKCOLORINDEX,COLORINDEX);


		//д����
		int nX, nY;
		if(m_szCaption != NULL)
		{
			if (m_nXSize <strlen(m_szCaption)*8)
			{
				nX = m_nX;
			}
			else
			{
				nX = m_nX+ (m_nXSize - strlen(m_szCaption)*8)/2; //�������λ��
			}
			if (m_nYSize<16)
			{	
				nY = m_nY;
			}
			else
				nY = m_nY+ (m_nYSize - 16)/2;

			Data_DrawText(nX, nY, (unsigned char*)m_szCaption, COLORINDEX, BKCOLORINDEX, 16);
		}

		if(m_bIsFocus)
		{
			if (m_nCursorType == CusorUnderLine) //�߹��
			{
				Data_DrawHDotLine(m_nX+2, m_nY+2, m_nX+m_nXSize-4); //��
				Data_DrawHDotLine(m_nX+2, m_nY+m_nYSize-2,m_nX+m_nXSize-4+1); //��
				
				Data_DrawVDotLine(m_nX+2, m_nY+2, m_nY+m_nYSize-4); //��
				Data_DrawVDotLine(m_nX+m_nXSize-2, m_nY+2, m_nY+m_nYSize-4); //��
			}
			else // ���ڹ��
			{
				Data_XorRect(m_nX+2, m_nY+2, m_nXSize-4, m_nYSize-4);

			}

		}

//@	}

//@	else
//@	{
//@		Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize);
//@	}
//@	if(b_IsFresh)
		Invalidate();
//		LCD_Paint(m_nX, m_nY, m_nXSize, m_nYSize);

}

