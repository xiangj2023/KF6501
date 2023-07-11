/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   �ļ����ƣ�HmiStatic.cpp
*   ���ģ�飺<...>
*   �� �� �ţ�1.0
*   �������ڣ�2008-4-3
*   ��    �ߣ�kangqian
*   ��    �ܣ�<implementation of the CHmiStatic class.>
* 
*************************************************************************/


#include "HmiWnd.h"
#include "HmiApp.h"
#include "HmiStatic.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CHmiStatic::OnPaint()
{
	//����ԭ��
	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);
		
	//��λͼ
		if(m_bmData != NULL)
			Data_DrawBitmap(m_nX+m_bmLeft, m_nY+m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL, m_bmData, BKCOLORINDEX,COLORINDEX);

	LONG nLen;
	if(m_szCaption != NULL)
	{
		nLen = strlen(m_szCaption);

		Data_DrawText(m_nX, m_nY ,(unsigned char*)m_szCaption, COLORINDEX, BKCOLORINDEX, m_nFont);
	}
	Invalidate();
//	LCD_Paint(m_nX, m_nY, m_nXSize, m_nYSize);

}

CHmiStatic::CHmiStatic()
{
	m_nFont= 12;
	m_nObjType = HmiStatic;

}

CHmiStatic::~CHmiStatic()
{

}

/*************************************************************************
* �Ķ���ʷ��¼��

Revision 1.0, 2008-4-3, kangqian
describe: ��ʼ����.
*************************************************************************/
