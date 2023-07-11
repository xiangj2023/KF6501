/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiEdit.cpp
* 
*   ���ģ�飺Һ������
*
* 
*	���� �������ֻ���������֣� ͨ������С��λ������󳤶ȣ� �Ƿ���ʾ���������������ֵĸ�ʽ
*		 �ı������ݰ���2������ m_szCaption ��ʾ�ı���  m_fCurValue �ڲ�ֵ
*	���� 
*   UpdateData(BOOL bIsSave) �����ı������ݣ�ֻ�Ǹ������ݣ� û��ˢ����ʾ��
*		 bIsSave=True�� �������룬 ��m_szCaption�����ݱ��浽m_fCurValue
*		 =False, ��m_fCurValue����С����λ�ã���󳤶ȣ��Ƿ������ŵĸ�ʽ���µ��ַ���m_szCaption
*	SetValue��GetValue �ڲ�ֵ��д����
*	SetDecimalNum����С����λ���� 
*	SetIsShowNegative�����Ƿ���ʾ�����ţ����ڿ����޸�Ϊ�����ģ���ʹ��ǰֵ������ҲҪ��ʾ�����Ա�������븺��
*	SetCursorPos(LONG pos) ���ù��λ���ڵڼ����ַ�λ��
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-5-14		��ʼ����
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
	m_nCursorType = CusorUnderLine; //�»��߹��

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
	m_nDisplayType  = Dec; //Ĭ��10������


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
	
	//���+-���޸�ֵ
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
		//�ж��Ƿ񳬹����ֵ
		if(m_fCurValue > m_fMAx)
		{
			SetValue(m_fMAx); 
			UpdateData(FALSE); // ���޸�������
		}
		if(m_fCurValue < m_fMin )
		{
			SetValue(m_fMin); 
			UpdateData(FALSE); // ���޸�������
		}
		
		OnPaint();
		UpdateWindow();
	}

	//������Զ�����Ӧ����
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
			UpdateData(FALSE); // ���޸�������
		}
		if(m_fCurValue < m_fMin )
		{
			SetValue(m_fMin); 
			UpdateData(FALSE); // ���޸�������
		}
		
		OnPaint();
		UpdateWindow();
	}



	//������Զ�����Ӧ����
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
		//�ж�������ַ����͵Ĳ����ƹ��
		m_nCursorPos++;
		m_nCursorPos = m_nCursorPos%m_nMaxLen;
		
		//��ֹ���ԵĶ���...����ѭ��
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

	//������Զ�����Ӧ����
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
		
		//��ֹ���ԵĶ���...����ѭ��
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

	//������Զ�����Ӧ����
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

	//�������ݸ�����Ϣ
	DWORD wParam;
	wParam = MAKELONG(m_nTabOrder, HMIEDIT_CHANGE);
	m_pOwner->SendMessage(HMIWM_COMMAND,wParam);

//@	//2009-3-6 11:19:17ȥ��CHmiEdit�Ļس���ת���ڣ� ���Ҫ��ת���Զ��庯����ʵ��
//@	//������ת
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
	//д����


	if(m_szCaption != NULL)
			Data_DrawText(m_nX+1, m_nY+1 ,(unsigned char*)m_szCaption, COLORINDEX, BKCOLORINDEX, m_nFont);

	if(m_bIsFocus)
	{
		DrawCursor(m_nX+m_nCursorPos*m_nFont/2,m_nY+m_nFont, m_nX+m_nCursorPos*m_nFont/2+m_nFont/2-1, 0 );

	}

	//���߿�
	Data_DrawHLine(m_nX, m_nY, m_nX+m_nXSize); //��
	Data_DrawHLine(m_nX, m_nY+m_nYSize,m_nX+m_nXSize); //��
	
	Data_DrawVLine(m_nX, m_nY, m_nY+m_nYSize); //��
	Data_DrawVLine(m_nX+m_nXSize, m_nY, m_nY+m_nYSize); //��

	Invalidate();
//	LCD_Paint(0,0,LCD_XSIZE, LCD_YSIZE);
}

//������ʾ�����ֵ�� ����ֵ�������÷�Χ
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
			//�ֿ�д�� ��Ȼ����̫��̫����
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
	if(m_nCursorType  == CusorUnderLine) //�»���
	{
		Data_DrawHLine(x0, y0, x1);
		Data_DrawHLine(x0, y0+1, x1);

	}
	else //����
	{
		Data_XorRect(x0, y0, x1-x0, y1-y0);
	}
}

void CHmiEdit::SetValueRange(float max, float min)
{
	m_fMAx = max;
	m_fMin = min;
}
