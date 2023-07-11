/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：HmiStatic.cpp
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-4-3
*   作    者：kangqian
*   功    能：<implementation of the CHmiStatic class.>
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
	//擦除原有
	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);
		
	//画位图
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
* 改动历史纪录：

Revision 1.0, 2008-4-3, kangqian
describe: 初始创建.
*************************************************************************/
