/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiDataStatic.cpp
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
*		kangqian	2008-5-21		��ʼ����
* 
*************************************************************************/

// HmiDataStatic.cpp: implementation of the CHmiDataStatic class.
//
//////////////////////////////////////////////////////////////////////
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			

#include "HmiDataStatic.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHmiDataStatic::CHmiDataStatic()
{

}

CHmiDataStatic::~CHmiDataStatic()
{

}
void CHmiDataStatic::UpdateData(BOOL bIsSave)
{
	if(bIsSave)
		return;
	GetDataFromDB();

}
BOOL CHmiDataStatic::Init()
{
//	return TRUE;
	return GetDataFromDB();
}

BOOL CHmiDataStatic::GetDataFromDB()
{
	TAIPara 	tAIPara;
	TRM_Disp	tRMPara;
	TCIPara tCIPara;
	TBIPara tBIPara;

	TSysTime  tSysTime;
	TAbsTime tAbsTime;
	LONG lValue, rc;

	BYTE val;
	DWORD dwVal;
	if(m_nDataType == -1)
		return TRUE;
	char str[256];
	char strTmp[256];
	char strFormat[32];
	
	char strUnit[32];
	char strSymbol[32];
	WORD num;

	switch(m_nDataType)
	{
	case hAI:
		//ң��һ��ֵ
		num = DB_GetAINum();
		if (m_nDataOffset >=num)
		{
			this->SetVisible(FALSE);
			break;
		}
		rc = DB_GetAIPara(m_nDataOffset, &tAIPara);
		if(DB_ERR_OK != rc)
			sprintf(strSymbol,"hAI%d", m_nDataOffset);
		else
			sprintf(strSymbol,"%s", tAIPara.strSymbol);
			
		rc = DB_ReadFirstAI(m_nDataOffset, &lValue,strUnit,&val);
		if(DB_ERR_OK != rc)
		{
			val = 0;
			strcpy(strUnit , "E");
			lValue = 0;
		}

		GetFloatString(lValue,val, str,hAI);

		sprintf(strTmp,"%-4s=%s%s", strSymbol, str, strUnit);

		SetCaption(strTmp);
		Invalidate();
		break;

	case lAI:
		//ң��2��ֵ
		num = DB_GetAINum();
		if (m_nDataOffset >=num)
		{
			this->SetVisible(FALSE);
			//LogError("CHmiDataStatic::GetDataFromDB",FILE_LINE,"m_nDataOffset>AINum!");
			break;
		}

		rc = DB_ReadSecondAI(m_nDataOffset, &lValue,strUnit,&val);	
		if(DB_ERR_OK != rc)
			{
				val = 0;
				strcpy(strUnit , "E");
				lValue = 0;
			}
		GetFloatString(lValue, val , str,lAI);

		rc = DB_GetAIPara(m_nDataOffset, &tAIPara);
		if(DB_ERR_OK != rc)
			sprintf(strSymbol,"lAI%d", m_nDataOffset);
		else
			sprintf(strSymbol,"%s", tAIPara.strSymbol);
		
		sprintf(strTmp,"%-4s=%s%s", strSymbol, str, strUnit);
		SetCaption(strTmp);
		Invalidate();
		break;

	case phAI:
		//����һ��ֵ
		num = DB_GetRMNum();
		if (m_nDataOffset >=num)
		{
			this->SetVisible(FALSE);
			//LogError("CHmiDataStatic::GetDataFromDB",FILE_LINE,"m_nDataOffset>RMNum!");
			break;
		}
		rc = DB_ReadFirstRM(m_nDataOffset, &lValue);
		
		if(DB_ERR_OK != rc)
		{
			lValue = 0;
		}
		rc = DB_GetRMDisp(m_nDataOffset, &tRMPara);
		if(DB_ERR_OK != rc)
		{
			tRMPara.nFirDec = 0;
			strcpy(tRMPara.strSymbol, "ERR");
			strcpy(tRMPara.strFirUnit, "E");
		}
		
		if(tRMPara.wAttr == RM_ATTR_HEX)
		{			
			sprintf(strTmp,"%4s= %04X%s", tRMPara.strSymbol, lValue, tRMPara.strFirUnit);
		}
		else
		{
			GetFloatString(lValue, tRMPara.nFirDec , str,phAI);
			sprintf(strTmp,"%4s=%s%s", tRMPara.strSymbol, str, tRMPara.strFirUnit);
		}
		SetCaption(strTmp);
		Invalidate();
		break;

	case plAI:
		//��������ֵ
		num = DB_GetRMNum();
		if (m_nDataOffset >=num)
		{
			this->SetVisible(FALSE);
			//LogError("CHmiDataStatic::GetDataFromDB",FILE_LINE,"m_nDataOffset>RMNum!");
			break;
		}

		rc = DB_ReadSecondRM(m_nDataOffset, &lValue);
		
		if(DB_ERR_OK != rc)
			{
				lValue = 0;
			}
		rc = DB_GetRMDisp(m_nDataOffset, &tRMPara);
		
		if(DB_ERR_OK != rc)
			{
				tRMPara.nSecDec = 0;
				strcpy(tRMPara.strSymbol, "ERR");
				strcpy(tRMPara.strSecUnit , "E");
			}
		GetFloatString(lValue, tRMPara.nSecDec  , str,plAI);
		sprintf(strTmp,"%4s=%s%s", tRMPara.strSymbol, str, tRMPara.strSecUnit);
		SetCaption(strTmp);
		Invalidate();

		break;
	case BI:
		//���룬��ʾ���ݵ�ʱ��Ҫ�ÿ��ķ����ʵ�ķ����ʾ0��1
		num = DB_GetBINum();
		if (m_nDataOffset >=num)
		{
			this->SetVisible(FALSE);
			//LogError("CHmiDataStatic::GetDataFromDB",FILE_LINE,"m_nDataOffset>BINum!");
			break;
		}

		rc = DB_ReadBI(m_nDataOffset, &val);
		//2010-08-18  ��ʾ�����Ϊ������Ŷ�����BI���
		rc = DB_GetBIPara(m_nDataOffset, &tBIPara);
		if(rc != DB_ERR_OK)
			break;
			

		if(val == DB_BI_OPEN)
		{
			sprintf(str,"%02d%s", tBIPara.wMaptoDINo+1, "��");

		}
		else
		{
			sprintf(str,"%02d%s", tBIPara.wMaptoDINo+1, "��");
		}
		SetCaption(str);
		Invalidate();


		break;
	case CI:
		//ʵʱ���
		num = DB_GetCINum();
		if (m_nDataOffset >=num)
		{
			this->SetVisible(FALSE);
			break;
		}
		
		rc = DB_ReadCI(m_nDataOffset, &dwVal);
		if(DB_ERR_OK != rc)
			dwVal = 0;
		rc = DB_GetCIPara(m_nDataOffset, &tCIPara);
		if(DB_ERR_OK != rc)
			strcpy(tCIPara.strSymbol, "ERR");
		char format1[16];
		char str1[32];
		sprintf(format1, "%s%df", "%08.", 2);
		sprintf(str1,format1,(float)dwVal/100);
		sprintf(strTmp,"%-3s=%s", tCIPara.strSymbol, str1);
		SetCaption(strTmp);
		Invalidate();
		break;

	case Time:
		//ʱ��
		ReadAbsTime(&tAbsTime);
		AbsTimeTo(&tAbsTime, &tSysTime);
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d",
			tSysTime.Year, tSysTime.Month, tSysTime.Day,
			tSysTime.Hour, tSysTime.Minute, tSysTime.Second);

		sprintf(strTmp, "%04d-%02d-%02d   %02d:%02d:%02d",
			tSysTime.Year, tSysTime.Month, tSysTime.Day,
			tSysTime.Hour, tSysTime.Minute, tSysTime.Second);
		SetCaption(str);
		Invalidate();
		
	default:
		;
	}
	return TRUE;
}
