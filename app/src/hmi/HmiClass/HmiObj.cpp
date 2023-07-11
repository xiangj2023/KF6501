/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiObj.cpp
* 
*   ���ģ�飺
*
* 
*	����
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
*		kangqian	2008-6-5		��ʼ����
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
			sprintf(format, " %s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;
	case lAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;		
	case plAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;			
	case phAI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;		
	case CI:		
		if (value>=0)
		{
			sprintf(format, " %s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str, format, fval);
		}
		else
		{
			value = abs(value);
			sprintf(format, "%s%df", "%06.", nDec); //�統nDecΪ3ʱ�� format Ϊ"%.3f", 
			float fval = ((float)value)/nDecValue;
			sprintf(str1, format, fval);		
			sprintf(str, "-%s", str1);
		}
		break;
		
	}



//@	if (value>=0)
//@	{
//@		sprintf(format, "%s%dd", "%d.%0", nDec); //�統nDecΪ3ʱ�� format Ϊ"%d.%03d", 
//@
//@		sprintf(str, format, value/nDecValue,  value%nDecValue);
//@	}
//@	else
//@	{
//@		sprintf(format, "%s%dd", "-%d.%0", nDec); //�統nDecΪ3ʱ�� format Ϊ"%d.%03d", 
//@
//@		value = abs(value);
//@		sprintf(str, format, value/nDecValue,  value%nDecValue);
//@	}

}

LONG CHmiObj::s_nLangIndex = 0; //Ĭ������0
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
	m_nCursorType = CusorUnderLine; //�»��߹��
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
	//����Ƿ����������Ƶ�������Ļȥ���Ƿ����ļ��㷽����Һ����ˢ�·�ʽ�й�
	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
	//��շǷ���
	return TRUE;
}

/****************************************************************************************
 * 
 * ��������:  ����λͼ
 * 
 * ����˵��: 
 *			- char* bitmap
 *			  ����: 
 *			  ˵��: λͼ����
 *
 *			- LONG l
 *			  ����: 
 *			  ˵��: ��
 *
 *			- LONG t
 *			  ����: 
 *			  ˵��: ��
 *
 *			- LONG Width
 *			  ����: 
 *			  ˵��: ��
 *
 *			- LONG Height
 *			  ����: 
 *			  ˵��: ��
 *
 *			- LONG bytePerLine
 *			  ����: 
 *			  ˵��: ÿ����ռ���ֽ�������λͼ�Ĵ洢��ʽ�й�
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
void CHmiObj::SetBitmap(unsigned char* bitmap, LONG l, LONG t, LONG Width, LONG Height, LONG bytePerLine)
{
	m_bmData = bitmap; //λͼ����
	m_bmLeft = l; 
	m_bmTop= t;
	m_bmHeight = Height;
	m_bmWidth= Width;
	m_bmBPL = bytePerLine  ; //λͼ���ϣ� �ߣ��� ÿ���ֽ���
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

