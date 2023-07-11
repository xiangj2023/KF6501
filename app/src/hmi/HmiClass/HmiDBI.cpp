/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiDBI.cpp
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
*		kangqian	2011-3-30		��ʼ����
* 
*************************************************************************/
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			
#include "Db_err.h"

#include "HmiDBI.h"
#include "bitmapdata.h"
CHmiDoubleBI::CHmiDoubleBI()
{
	SetCaption("");
	
	m_wValue = DBI_VALUE_OFF;
}

CHmiDoubleBI::~CHmiDoubleBI()
{

}

/*
BOOL CHmiDoubleBI::Init(WORD x,WORD y, WORD OnId, WORD OffId, 
						unsigned char* bmpOff, unsigned char* bmpOn, unsigned char* bmpErr,
						WORD bmpWidth, WORD bmpHeight, WORD bmpBPL)
{
	SetPosition(x, y, x+bmpWidth, y+ bmpHeight);
	m_nOnId =  OnId;
	m_nOffId = OffId;

	SetBitmap(bmpOn, x, y, bmpWidth, bmpHeight, bmpBPL);
	m_OnbmData = bmpOn;
	m_OffbmData = bmpOff;
	m_ErrbmData = bmpErr;

	SetCaption("");
	return true;
}
*/
void CHmiDoubleBI::SetDBIBitmap(unsigned char* Onbmp,unsigned char* Offbmp, 
			   LONG bmpWidth, LONG bmpHeight, LONG bmpBPL,
			   LONG nOnId,LONG nType ,LONG nOffId , 
			   unsigned char* Errbmp )
{
	m_nOnId = nOnId;
	SetBitmap(Onbmp, 0, 0, bmpWidth, bmpHeight, bmpBPL);

	m_OnbmData = Onbmp;
	m_OffbmData = Offbmp;
	m_nType = nType;
	if(nType == BI_TYPE_DPI)
	{
		m_nOffId = nOffId;
		m_ErrbmData = Errbmp;
	}
	else
	{
		m_nOffId = -1;
		m_ErrbmData = NULL;
	}
}


void CHmiDoubleBI::OnPaint()
{
	
	m_nXSize = m_bmWidth;
	m_nYSize = m_bmHeight;
	if(m_wValue == DBI_VALUE_ON)
	{
		SetBitmap(m_OnbmData,m_bmLeft,m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL);
	}
	else if(m_wValue == DBI_VALUE_OFF)
	{
		SetBitmap(m_OffbmData,m_bmLeft,m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL);

	}
	else // if(m_wValue == DBI_VALUE_ERR)
	{
		SetBitmap(m_ErrbmData,m_bmLeft,m_bmTop, m_bmWidth, m_bmHeight, m_bmBPL);
	}
	

	CHmiStatic::OnPaint();
}

BOOL CHmiDoubleBI::GetDataFromDB()
{
	return true;

}
void CHmiDoubleBI::UpdateData(BOOL bIsSave)
{
	BYTE valOn, valOff;
	BYTE byOn, byOff;
	LONG rc1,rc2;
	WORD num = DB_GetBINum();
	if ((m_nOnId <num) && (m_nOnId != -1))
	{
		rc1 = DB_ReadBI(m_nOnId, &valOn);
	}

	if((DB_ERR_OK == rc1) && (valOn == DB_BI_CLOSE ))
		byOn = 2;
	else
		byOn = 0;

	//����
	if(BI_TYPE_SPI == m_nType)
	{ 
		if(byOn == 0)
			m_wValue = DBI_VALUE_OFF;
		else
			m_wValue = DBI_VALUE_ON;
		return;
	}
	
	//˫�� 
	if ((m_nOnId <num) && (m_nOffId != -1))
	{
		rc2 = DB_ReadBI(m_nOffId, &valOff);
	}

	//��������Ϊ�Ƿ�λ
	if((DB_ERR_OK == rc1) && (valOff == DB_BI_CLOSE ))
		byOff = 1;
	else
		byOff = 0;

	m_wValue = byOff|byOn;
}


