/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiEdit.cpp
* 
*   软件模块：液晶界面
*
* 
*	描述 输入框中只能输入数字， 通过设置小数位数，最大长度， 是否显示正负号来设置数字的格式
*		 文本框内容包括2个变量 m_szCaption 显示的标题  m_fCurValue 内部值
*	函数 
*   UpdateData(BOOL bIsSave) 更新文本框内容（只是更新内容， 没有刷新显示）
*		 bIsSave=True， 保存输入， 把m_szCaption的内容保存到m_fCurValue
*		 =False, 把m_fCurValue按照小数点位置，最大长度，是否正负号的格式更新到字符串m_szCaption
*	SetValue，GetValue 内部值读写操作
*	SetDecimalNum设置小数点位数； 
*	SetIsShowNegative设置是否显示正负号，对于可以修改为负数的，即使当前值是正数也要显示正号以便可以输入负数
*	SetCursorPos(LONG pos) 设置光标位置在第几个字符位置
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-5-14		初始创建
* 
*************************************************************************/


// HmiEdit.cpp: implementation of the CHmiEdit class.
//
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "HmiWnd.h"
#include "HmiApp.h"
#include "HmiEdit.h"
#include "HmiMSG.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiEdit::CHmiEdit()
{
	m_nCursorType = CusorUnderLine; //下划线光标

	m_nCursorPos = 0;
	m_nFont = 16;
	m_nMaxLen = 1;
	m_nDecimalNum = 0;
	m_bIsShowNegative = FALSE;

	memset(m_szCaption, '0', LCD_STRLEN);
	m_szCaption[LCD_STRLEN-1] = '\0';

	m_fCurValue = 0;
	m_pEnterWnd = NULL;
	m_nObjType = HmiEdit;
	m_nDisplayType  = Dec; //默认10进制数


}

CHmiEdit::~CHmiEdit()
{

}

void CHmiEdit::SetValue(float val, LONG nDecimalNum, LONG nLength)
{
	m_fCurValue = val;
	m_nDecimalNum = nDecimalNum;
	m_nMaxLen = nLength;
	UpdateData(FALSE);

}

BOOL CHmiEdit::OnAdd()
{
	float fold = m_fCurValue;
	
	//如果+-键修改值
	if (m_nAddSubFun !=-1)
	{
		switch(m_nDisplayType)
		{
		case Dec:
			if(m_szCaption[m_nCursorPos] == '+' )
			{
				m_szCaption[m_nCursorPos] = '-';
				
			}
			else if(m_szCaption[m_nCursorPos] == '-')
			{
				m_szCaption[m_nCursorPos] = '+';
			}
			
			else if(m_szCaption[m_nCursorPos]== '9')
				m_szCaption[m_nCursorPos] = '0';
			else
				m_szCaption[m_nCursorPos]++;
			break;
		case Hex:
			if(m_szCaption[m_nCursorPos]== 'f' || m_szCaption[m_nCursorPos]== 'F')
				m_szCaption[m_nCursorPos] = 0;
			else
				m_szCaption[m_nCursorPos]++;
			break;
		}
		
		UpdateData();
		//判断是否超过最大值
		if(m_fCurValue > m_fMAx)
		{
			SetValue(m_fMAx); 
			UpdateData(FALSE); // 不修改内容了
		}
		if(m_fCurValue < m_fMin )
		{
			SetValue(m_fMin); 
			UpdateData(FALSE); // 不修改内容了
		}
		
		OnPaint();
		UpdateWindow();
	}

	//如果有自定义响应函数
	 if (m_bHaveAddSubFun)
	 {
		 DWORD wParam;
		 wParam = MAKELONG(m_nTabOrder, HMIEDIT_ADDSUB);
		 m_pOwner->SendMessage(HMIWM_COMMAND,wParam, 1);
	 }
	return TRUE;
}
BOOL CHmiEdit::OnSub()
{

	float fold = m_fCurValue;
	if (m_nAddSubFun !=-1)
	{
		switch(m_nDisplayType)
		{
			
		case Dec:
			if(m_szCaption[m_nCursorPos] == '+' )
			{
				m_szCaption[m_nCursorPos] = '-';
			}
			else if(m_szCaption[m_nCursorPos] == '-')
			{
				m_szCaption[m_nCursorPos] = '+';
			}
			
			else if(m_szCaption[m_nCursorPos]=='0')
				m_szCaption[m_nCursorPos] = '9';
			else
				m_szCaption[m_nCursorPos]--;
			break;
		case Hex:
			if(m_szCaption[m_nCursorPos]== '0')
				m_szCaption[m_nCursorPos] = 'F';
			else
				m_szCaption[m_nCursorPos]--;
			break;
		}
		
		UpdateData();
		
		if(m_fCurValue > m_fMAx)
		{
			SetValue(m_fMAx); 
			UpdateData(FALSE); // 不修改内容了
		}
		if(m_fCurValue < m_fMin )
		{
			SetValue(m_fMin); 
			UpdateData(FALSE); // 不修改内容了
		}
		
		OnPaint();
		UpdateWindow();
	}



	//如果有自定义响应函数
	if (m_bHaveAddSubFun)
	{
		DWORD wParam;
		wParam = MAKELONG(m_nTabOrder, HMIEDIT_ADDSUB);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam, -1);
	}

	return TRUE;
}
BOOL CHmiEdit::OnRight()
{
	if (m_nLeftRightFun != -1)
	{
		//判断如果是字符串型的不能移光标
		m_nCursorPos++;
		m_nCursorPos = m_nCursorPos%m_nMaxLen;
		
		//防止所以的都是...而死循环
		int nLoop = 0;
		while(m_szCaption[m_nCursorPos] == '.')
		{
			m_nCursorPos++;
			m_nCursorPos = m_nCursorPos%m_nMaxLen;
			if(nLoop++ > m_nMaxLen-1)
				break;
		}
		OnPaint();
		UpdateWindow();
	}

	//如果有自定义响应函数
	if (m_bHaveLeftRightFun)
	{
		DWORD wParam;
		wParam = MAKELONG(m_nTabOrder, HMIEDIT_LEFTRIGHT);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam, 1);
	}
	return TRUE;

}
BOOL CHmiEdit::OnLeft()
{
	if (m_nLeftRightFun != -1)
	{
		m_nCursorPos = m_nCursorPos + m_nMaxLen;
		m_nCursorPos--;
		m_nCursorPos = m_nCursorPos%m_nMaxLen;
		
		//防止所以的都是...而死循环
		int nLoop = 0;
		while(m_szCaption[m_nCursorPos] == '.')
		{
			m_nCursorPos = m_nCursorPos + m_nMaxLen;
			m_nCursorPos--;
			m_nCursorPos = m_nCursorPos%m_nMaxLen;
			
			if(nLoop++ > m_nMaxLen-1)
				break;
			
		}
		
		OnPaint();
		UpdateWindow();

	}

	//如果有自定义响应函数
	if (m_bHaveLeftRightFun)
	{
		DWORD wParam;
		wParam = MAKELONG(m_nTabOrder, HMIEDIT_LEFTRIGHT);
		m_pOwner->SendMessage(HMIWM_COMMAND,wParam, -1);
	}
	return TRUE;
}
BOOL CHmiEdit::OnEnter()
{
	m_fCurValue = atol((char*)(&m_szCaption));

	//发送数据更改消息
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMIEDIT_CHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

//@	//2009-3-6 11:19:17去掉CHmiEdit的回车跳转窗口， 如果要跳转在自定义函数中实现
//@	//窗口跳转
//@	if(m_pEnterWnd != NULL)
//@	{
//@		CHmiApp::m_pActiveWnd = m_pEnterWnd;
//@		m_pEnterWnd->Show(m_fCurValue, 0);
//@		return true;
//@	}
	return false;

}

void CHmiEdit::OnPaint()
{

	Data_FillSolidRect(m_nX, m_nY, m_nX+m_nXSize, m_nY+m_nYSize, BKCOLORINDEX);
	//写标题


	if(m_szCaption != NULL)
			Data_DrawText(m_nX+1, m_nY+1 ,(unsigned char*)m_szCaption, COLORINDEX, BKCOLORINDEX, m_nFont);

	if(m_bIsFocus)
	{
		DrawCursor(m_nX+m_nCursorPos*m_nFont/2,m_nY+m_nFont, m_nX+m_nCursorPos*m_nFont/2+m_nFont/2-1, 0 );

	}

	//画边框
	Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize); //上
	Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize); //下
	
	Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize); //左
	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize); //右

	Invalidate();
//	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
}

//设置显示的最大值， 输入值不超过该范围
void CHmiEdit::SetUpperLimit( float fMax )
{
	m_fMAx =fMax;
}
float CHmiEdit::GetUpperLimit( ) const
{
	return m_fMAx;
}

void CHmiEdit::SetLowerLimit( float fMin )
{
	m_fMin = fMin;
}
float CHmiEdit::GetLowerLimit( ) const
{
	return m_fMin;
}
void CHmiEdit::UpdateData(BOOL bIsSave)
{
	char format[16];
	if(bIsSave)
	{
		switch(m_nDisplayType)
		{
		case Dec:
			m_fCurValue = 	atof(m_szCaption);
			break;
		case Hex:
			m_fCurValue = (float)strtol(m_szCaption, NULL, 16);
		}
		
	}
	else
	{
		switch(m_nDisplayType)
		{
		case Dec:
			//分开写， 不然参数太多太乱了
			if(m_bIsShowNegative)
			{
				if(m_nDecimalNum == 0)
					sprintf((char*)(&format), "%s%dd", "%+#0", m_nMaxLen);
				else
					sprintf((char*)(&format), "%s%d.%df", "%+#0", m_nMaxLen, m_nDecimalNum); 
			}
			else
			{
				if(m_nDecimalNum == 0)
					sprintf((char*)(&format),  "%s%dd", "%#0", m_nMaxLen);
				else
					sprintf((char*)(&format), "%s%d.%df", "%#0", m_nMaxLen, m_nDecimalNum);
			}

			break;
		case Hex:
			sprintf((char*)(&format), "%s", "%X");

			break;
		case Bin:
			sprintf((char*)(&format), "%s", "%b");
			break;
		case Boolean:
			break;
		default:
			;
		}

		if((m_nDecimalNum !=0) && (m_nDisplayType == Dec))
		{
			sprintf((char*)(&m_szCaption), format, m_fCurValue);
		}
		else
		{
			LONG lvalue = (LONG)m_fCurValue;
			sprintf((char*)(&m_szCaption), format, lvalue);

		}

	}

}

void CHmiEdit::DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1)
{
	if(m_nCursorType  == CusorUnderLine) //下话线
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //反黑
	{
		Data_XorRect(x0, y0, x1-x0, y1-y0);
	}
}

void CHmiEdit::SetValueRange(float max, float min)
{
	m_fMAx = max;
	m_fMin = min;
}
