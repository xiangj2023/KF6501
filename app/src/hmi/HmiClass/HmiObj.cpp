/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiObj.cpp
* 
*   软件模块：
*
* 
*	描述
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
*		kangqian	2008-6-5		初始创建
* 
*************************************************************************/


#include "HmiObj.h"
#include "HmiDataStatic.h"
#include "translator.h"
#include "hmiapp.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_HMI_TR(CHmiObj)

void GetFloatString(LONG value, LONG nDec, char* str,LONG dwDataType)
{
	char str1[256];
	LONG nDecValue = 1;
	for(int i=0; i<nDec; i++)
	{
		nDecValue *= 10;
	}


	char format[16];
	switch(dwDataType)
	{
	case hAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;
	case lAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;		
	case plAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;			
	case phAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;		
	case CI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //如当nDec为3时， format 为"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;
		
	}



//@	if (value>=0)
//@	{
//@		sprintf(format, "%s%dd", "%d.%0", nDec); //如当nDec为3时， format 为"%d.%03d", 
//@
//@		sprintf(str, format, value/nDecValue,  value%nDecValue);
//@	}
//@	else
//@	{
//@		sprintf(format, "%s%dd", "-%d.%0", nDec); //如当nDec为3时， format 为"%d.%03d", 
//@
//@		value = abs(value);
//@		sprintf(str, format, value/nDecValue,  value%nDecValue);
//@	}

}

LONG CHmiObj::s_nLangIndex = 0; //默认语言0
CHmiObj::CHmiObj()
	: m_nX(0), m_nY(0), m_nXSize(0), m_nYSize(0)
{
	m_bIsFocus = false;
	m_nTabOrder = -1;
	m_nPageIndex = 0;
	m_nFreshTimer = 0;
	m_bmData = NULL;
	m_nTimerValue = 0;
	m_bIsVisible = TRUE;
	m_nCursorType = CusorUnderLine; //下划线光标
	m_bIsAutoEnterWnd = TRUE;

}
CHmiObj::CHmiObj(LONG x, LONG y, LONG xsize, LONG ysize)
	: m_nX(x), m_nY(y), m_nXSize(xsize), m_nYSize(ysize)
{
	m_bIsFocus = false;
	m_nTabOrder = -1;
	m_nPageIndex = 0;
	m_nFreshTimer =0;
	m_bmData = NULL;
	m_bIsVisible = TRUE;
	m_bIsAutoEnterWnd = TRUE;
}
CHmiObj::~CHmiObj()
{

}



BOOL CHmiObj::Init()
{
	return TRUE;
}
void CHmiObj::UpdateData(BOOL bIsSave)
{

}
void CHmiObj::OnPaint()
{

}

BOOL CHmiObj::OnAdd()
{
	return false;

}

BOOL CHmiObj::OnSub()
{
	return false;

}

BOOL CHmiObj::OnLeft()
{
	return false;

}

BOOL CHmiObj::OnRight()
{
	return false;

}

BOOL CHmiObj::OnEsc()
{
	return false;

}

BOOL CHmiObj::OnEnter()
{
	return false;

}
BOOL CHmiObj::OnUp()
{
	return false;

}
BOOL CHmiObj::OnDown()
{
	return false;

}

void CHmiObj::ReDraw()
{
	OnPaint();
}

void CHmiObj::Invalidate(BOOL bErase)
{

}

BOOL CHmiObj::InvalidateRect(LONG l,LONG t,LONG r,LONG b, BOOL bErase)
{
	return TRUE;

}

BOOL CHmiObj::UpdateWindow()
{
	//计算非法区，并绘制到物理屏幕去。非法区的计算方法和液晶的刷新方式有关
	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
	//清空非法区
	return TRUE;
}

/****************************************************************************************
 * 
 * 功能描述:  设置位图
 * 
 * 参数说明: 
 *			- char* bitmap
 *			  类型: 
 *			  说明: 位图数组
 *
 *			- LONG l
 *			  类型: 
 *			  说明: 左
 *
 *			- LONG t
 *			  类型: 
 *			  说明: 上
 *
 *			- LONG Width
 *			  类型: 
 *			  说明: 宽
 *
 *			- LONG Height
 *			  类型: 
 *			  说明: 高
 *
 *			- LONG bytePerLine
 *			  类型: 
 *			  说明: 每行所占的字节数，和位图的存储方式有关
 *
 * 返回代码: 
 * 
 * 其它: 
 * 
 *****************************************************************************************/
void CHmiObj::SetBitmap(unsigned char* bitmap, LONG l, LONG t, LONG Width, LONG Height, LONG bytePerLine)
{
	m_bmData = bitmap; //位图数据
	m_bmLeft = l; 
	m_bmTop= t;
	m_bmHeight = Height;
	m_bmWidth= Width;
	m_bmBPL = bytePerLine  ; //位图左，上， 高，宽， 每行字节数
}

void CHmiObj::SetCaption(const char *szCaption)
{
	memset(m_szCaption,0,LCD_STRLEN);
	strcpy(m_szCaption, szCaption);
	m_szCaption[LCD_STRLEN-1] = '\0';
}
const char* CHmiObj::GetCaption()
{
	return m_szCaption;
}


void CHmiObj::EnableAutoEnterWnd()
{
	m_bIsAutoEnterWnd = TRUE;
}
void CHmiObj::DisableAutoEnterWnd()
{
	m_bIsAutoEnterWnd = FALSE;
}

