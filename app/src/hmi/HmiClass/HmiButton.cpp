/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：HmiButton.cpp
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-3-3
*   作    者：kangqian
*   功    能：<按钮控件实现>
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
//	m_nCursorType = CusorUnderLine; //下划线光标
	m_nCursorType =  CusorHighLight; //反黑光标

	m_szCaption[0] = '\0';
	m_szCaption[0] = '\0';

	m_pEnterWnd = NULL;
	m_bHaveEnterFun = FALSE;
	m_nObjType = HmiButton;
	m_bIsAutoEnterWnd = TRUE;
}

/*************************************************************************
* 
*   功能描述     
* 
*   参数说明：// 参数: LONG x
// 参数: LONG y
// 参数: LONG xsize
// 参数: LONG ysize): CHmiObj(x
// 参数: y
// 参数: xsize
// 参数: ysize
// 其它: 
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
		
	//为了提高运行效率而增加m_bHaveEnterFun， 否则每次都发消息， 搜索函数列表
	if(m_bHaveEnterFun)
	{
		//1.通知父窗口发生click消息
		DWORD wParam;
		wParam = MAKELONG(m_nTabOrder, HMIBTN_ENTER);
		
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam);
	}


	//2.跳转到新窗口	
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
		//擦除原有
		Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);
		//画边框
		Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize); //上
		Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize); //下
		Data_DrawHLine(m_nX, m_nY+m_nYSize+1,m_nX+m_nXSize+1); //下

		Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize); //左
		Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize); //右
		Data_DrawVLine(m_nX+m_nXSize+1, m_nY, m_nY+m_nYSize); //右

		//画位图
		if(m_bmData != NULL)
			Data_DrawBitmap(m_nX+m_bmLeft, m_nY+m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL, m_bmData, BKCOLORINDEX,COLORINDEX);


		//写标题
		int nX, nY;
		if(m_szCaption != NULL)
		{
			if (m_nXSize <strlen(m_szCaption)*8)
			{
				nX = m_nX;
			}
			else
			{
				nX = m_nX+ (m_nXSize - strlen(m_szCaption)*8)/2; //计算居中位置
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
			if (m_nCursorType == CusorUnderLine) //线光标
			{
				Data_DrawHDotLine(m_nX+2, m_nY+2, m_nX+m_nXSize-4); //上
				Data_DrawHDotLine(m_nX+2, m_nY+m_nYSize-2,m_nX+m_nXSize-4+1); //下
				
				Data_DrawVDotLine(m_nX+2, m_nY+2, m_nY+m_nYSize-4); //左
				Data_DrawVDotLine(m_nX+m_nXSize-2, m_nY+2, m_nY+m_nYSize-4); //右
			}
			else // 反黑光标
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

