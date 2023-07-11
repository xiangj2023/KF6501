/********************************************************************************************************/
/*																										*/
/*        						��򿭷������ɷ����޹�˾												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           													*/
/*																										*/
/*      relayfunc.cpp                                     												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ����屣��������صĽӿں���																*/
/*																										*/
/* ����																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* ��������ݽṹ																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* ����																									*/
/*																										*/
/*	DB_GetRMNum()					�����ݿ���ȡ��������ֵ����											*/
/*	DB_ReadRM()						��ң�����ȡһ����������ֵ����										*/
/*	DB_WriteRM()					��ң�����дһ����������ֵ����										*/
/*	DB_GetAITofRM()					��ȡһ����������ֵ��ң����������									*/
/*	DB_GetCFofRM()					��ȡһ����������ֵ��ϵ��											*/
/*	DB_GetRTofRM()					��ȡһ����������ֵ�ı��											*/
/*	DB_RelayStart()					������������ע��													*/
/*	DB_RelayWriteFD()				д�뱣�������Ĺ��ϵ���												*/
/*	DB_RelayAction()				д�뱣�������ֱ���													*/
/*	DB_RelayEnd()					������������														*/
/*	DB_RelayAlarm()					���������澯����													*/
/*	DB_ReadRelayAlarmReport()		���豸���ж�ȡ�����߾�����											*/
/*	DB_WriteRelayAlarmReport()		���豸����д�뱣���澯����											*/
/*	DB_GetRelayAlarmReportNum()		��ȡ�����澯���Ŀ���δ��ȡ���ĵ���Ŀ								*/
/*	DB_GetRelayAlarmReportRead()	��ȡ�����澯���Ŀ�Ķ�ָ��											*/
/*	DB_GetRelayAlarmReportHead()	��ȡ�����澯��Ϣ��������ͷָ��										*/
/*	DB_IncRelayAlarmReportRead()	���������澯��Ϣ��������ͷָ��										*/
/*	DB_ReadRelayStartReport()		���豸���ж�ȡ������������											*/
/*	DB_WriteRelayStartReport()		���豸����д�뱣����������											*/
/*	DB_GetRelayStartReportNum()		��ȡ�����������Ŀ���δ��ȡ���ĵ���Ŀ								*/
/*	DB_GetRelayStartReportRead()	��ȡ�����������Ŀ�Ķ�ָ��											*/
/*	DB_GetRelayStartReportHead()	��ȡ����������Ϣ��������ͷָ��										*/
/*	DB_IncRelayStartReportRead()	��������������Ϣ��������ͷָ��										*/
/*	DB_ReadRelayActionReport()		���豸���ж�ȡ������������											*/
/*	DB_WriteRelayActionReport()		���豸����д�뱣����������											*/
/*	DB_GetRelayActionReportNum()	��ȡ�����������Ŀ���δ��ȡ���ĵ���Ŀ								*/
/*	DB_GetRelayActionReportRead()	��ȡ�����������Ŀ�Ķ�ָ��											*/
/*	DB_GetRelayActionReportHead()	��ȡ����������Ϣ��������ͷָ��										*/
/*	DB_IncRelayActionReportRead()	��������������Ϣ��������ͷָ��										*/
/*	DB_GetSWNum()					�����ݿ���ȡ��ѹ�����												*/
/*	DB_GetSWStatus()				�����ݿ���ȡһ����ѹ��״̬											*/
/*	DB_SetSWStatus()				�����ݿ���д��һ����ѹ��״̬										*/
/*	DB_SetSWStatusConfirm()			�����ݿ���д��һ����ѹ��״̬ȷ��									*/
/*	DB_GetCFGNum()					�����ݿ���ȡһ��������״̬											*/
/*	DB_GetCFGStatus()				�����ݿ���ȡһ��������״̬											*/
/*	DB_SetCFGStatus()				�����ݿ���д��һ��������״̬										*/
/*	DB_SetCFGStatusConfirm()		�����ݿ���д��һ��������״̬ȷ��									*/
/*	DB_GetSETNum()					�����ݿ���ȡ��ֵ����												*/
/*	DB_GetSETFloatValue()			�����ݿ��ж�ȡ��ֵ�ĸ�����											*/
/*	DB_GetSETFixValue()				�����ݿ��ж�ȡ��ֵ��������											*/
/*	DB_SetSETValue()				�����ݿ���д�붨ֵ													*/
/*	DB_SetSETValueConfirm()			�����ݿ���д�붨ֵȷ��												*/
/*	DB_GetCurrentSETAreaNo()		��ȡ��ǰ��ֵ����													*/
/*	DB_SetCurrentSETAreaNo()		���õ�ǰ��ֵ����													*/
/*	DB_GetSystemSETNum()			�����ݿ���ȡ������ֵ����											*/
/*	DB_GetSystemSETFloatValue()			�����ݿ��ж�ȡ������ֵ����������									*/
/*	DB_GetSystemSETFixValue()			�����ݿ��ж�ȡ������ֵ����������									*/
/*	DB_SetSystemSETValue()			�����ݿ���д�븨����ֵȷ��											*/
/*	DB_SetSystemSETValueConfirm()	�����ݿ���д�븨����ֵȷ��											*/
/*	DB_GetRTNum()					�����ݿ���ȡ��ȸ���												*/
/*	DB_GetRTValue()					�����ݿ��ж�ȡ��ȣ���������										*/
/*	DB_SetRTValue()					�����ݿ���д����													*/
/*	DB_SetRTValueConfirm()			�����ݿ���д����ȷ��												*/
/*	DB_GetCFNum()					�����ݿ���ȡϵ������												*/
/*	DB_GetCFValue()					�����ݿ��ж�ȡϵ��ֵ												*/
/*	DB_SetCFValue()					�����ݿ���д��ϵ��ֵ												*/
/*	DB_SetCFValueConfirm()			�����ݿ���д��ϵ��ֵȷ��											*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*		Dbfunc.h						���ݿ⺯������													*/
/*		Db_err.h						���ݿⷵ��ֵ													*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0												*/
/********************************************************************************************************/
#include "daefunc.h"		
#include "dbfunc.h"			
#include "parafunc.h"	
#include "Relaydef.h"
#include "Sample.h"
#include "initrelaydata.h"

#define	SYSTEM_SET_DEFINE1	0

extern	TSysConfigTable		*G_pCfgTable;
extern	CLink				G_RelayRepSaveLink;
extern	CLink				G_RelayRepFreeLink;
extern	CLink				G_DistDataSaveLink;		//����¼�����ݴ洢����
extern	CLink				G_DistDataFreeLink;		//����¼�����ݿ�������
extern 	HANDLE				g_hBOTaskID;
extern 	HANDLE				g_hLCDTaskID;
extern 	HANDLE				g_hRelayTaskID;
extern 	HANDLE				g_hDbaseTaskID;

extern	void				*G_pDistFileBlock;		//¼���ļ�����

STATUS ReadHisSampData(DWORD dwPtr, DWORD dwMask, SHORT *pBuf, DWORD dwPointNum);
BOOL isSampDataLenFill(DWORD dwStartPtr, DWORD wPointNum);
DWORD BackSamplePtr(DWORD dwStartPtr, DWORD wPointNum);
STATUS DAE_ReadFlashFile(char *strFileName,void *pFile,DWORD dwFileLen);

WORD	g_wRelayDistNo;

DWORD	*DB_GetSWBase()
{
	return G_pCfgTable->DBaseConfig.pSWData;
}

DWORD	*DB_GetCFGBase()
{
	return G_pCfgTable->DBaseConfig.pCFGData;
}

float	*DB_GetSETFloatBase()
{
	return G_pCfgTable->DBaseConfig.pSETDataFloat;
}

long	*DB_GetSETFixBase()
{
	return G_pCfgTable->DBaseConfig.pSETDataFix;
}

float	*DB_GetSystemSETBase()
{
	return G_pCfgTable->DBaseConfig.pSystemSETData;
}

long	*DB_GetCFBase()
{
	return G_pCfgTable->DBaseConfig.pCFData;
}

BOOL DB_CheckSWData(DWORD *pSW,WORD num)
{
	WORD 					i,j;
	DWORD					Mask;

	if(pSW == NULL)
		return FALSE;
	
	//��λ��ⷴ��У��
	j = (num+31)/32;
	
	for(i=0;i<num;i++)
	{
		Mask = 0x01<<(i%32);
		
		if((pSW[i/32]&Mask) == (pSW[j+(i/32)]&Mask))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL DB_CheckSETData(float *pSET,WORD	num)
{
	if(pSET == NULL)
		return FALSE;
	
	if(DAE_IsCheckSumOK((BYTE *)pSET, num*sizeof(float)))
		return TRUE;
	else
		return FALSE;
}

BOOL DB_CheckSETFixData(long *pSET,WORD	num)
{
	if(pSET == NULL)
		return FALSE;
	
	if(DAE_IsCheckSumOK((BYTE *)pSET, num*sizeof(long)))
		return TRUE;
	else
		return FALSE;
}

BOOL DB_CheckSETArea(WORD AreaCur,WORD AreaB)
{
	if((AreaCur^AreaB)!=0xFFFF)
		return FALSE;
	else
		return TRUE;
}

BOOL DB_CheckCFData(long *pCF,WORD num)
{
	if(pCF == NULL)
		return FALSE;
	
	if(DAE_IsCheckSumOK((BYTE *)pCF, num*sizeof(long)))
		return TRUE;
	else
		return FALSE;
}

/*���㶨ֵ�����㶨ֵ*/
STATUS ConvertSETFloat2Fix(WORD no,float fValue,long *plValue)
{
	TSET_Disp				*pSETDisp;
	WORD					CFGNum,nCFIndex;
	long					lcf,lRatio;
	
	ASSERT(plValue != NULL)
	if(plValue == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSETNum())
	if(no >= DB_GetSETNum())
		return DB_ERR_NO;
	
		//����������
	CFGNum = (DB_GetCFGNum()+31)/32;
	if(no<CFGNum)
	{
		*plValue = *(DWORD *)&fValue;
		return DB_ERR_OK;
	}	
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSETDisp;
	nCFIndex = pSETDisp[no].nCFIndex;

	ASSERT(nCFIndex < DB_GetCFNum())
	if(nCFIndex >= DB_GetCFNum())
	{
		LogError("ConvertSETFloat2Fix",FILE_LINE,"InValid CF No %d!!!",nCFIndex);
		return DB_ERR_NO;
	}
	
	lcf	 		= G_pCfgTable->DBaseConfig.pCFData[nCFIndex];
	lRatio	 	= G_pCfgTable->DBaseConfig.pCFDisp[nCFIndex].wCof;

	if(lcf == 0) 
		lcf = 1;
	if(lRatio == 0) 
		lRatio = 1;
	
	//	��ϵ��/����	
	*plValue = (long)(fValue*lcf)/lRatio;

	return DB_ERR_OK;
}

/*���㶨ֵ�����㶨ֵ*/
STATUS ConvertSETFix2Float(WORD no,long lValue,float *pfValue)
{
	TSET_Disp				*pSETDisp;
	WORD					nCFIndex;
	long					lcf,lRatio;
	
	ASSERT(pfValue != NULL)
	if(pfValue == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSETNum())
	if(no >= DB_GetSETNum())
		return DB_ERR_NO;
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSETDisp;
	nCFIndex = pSETDisp[no].nCFIndex;

	ASSERT(nCFIndex < DB_GetCFNum())
	if(nCFIndex >= DB_GetCFNum())
	{
		LogError("ConvertSETFix2Float",FILE_LINE,"InValid CF No %d!!!",nCFIndex);
		return DB_ERR_NO;
	}
	
	lcf	 		= G_pCfgTable->DBaseConfig.pCFData[nCFIndex];
	lRatio	 	= G_pCfgTable->DBaseConfig.pCFDisp[nCFIndex].wCof;

	if(lcf == 0) 
		lcf = 1;
	if(lRatio == 0) 
		lRatio = 1;
	
	//	��ϵ��/����	
	*pfValue = (float)(lValue*lRatio)/lcf;

	return DB_ERR_OK;
}

STATUS ConvertSystemSETFloat2Fix(WORD no,float fValue,long *plValue)
{
	TSysSET_Disp			*pSETDisp;
	WORD					nCFIndex;
	long					lcf,lRatio;
	
	ASSERT(plValue != NULL)
	if(plValue == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSystemSETNum())
	if(no >= DB_GetSystemSETNum())
		return DB_ERR_NO;
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSystemSETDisp;
	nCFIndex = pSETDisp[no].nCFIndex;

	ASSERT(nCFIndex < DB_GetCFNum())
	if(nCFIndex >= DB_GetCFNum())
	{
		LogError("ConvertSystemSETFloat2Fix",FILE_LINE,"InValid CF No!!!");
		return DB_ERR_NO;
	}
	
	lcf	 		= G_pCfgTable->DBaseConfig.pCFData[nCFIndex];
	lRatio	 	= G_pCfgTable->DBaseConfig.pCFDisp[nCFIndex].wCof;

	if(lcf == 0) 
		lcf = 1;
	if(lRatio == 0) 
		lRatio = 1;
	
	//	��ϵ��/����	
	*plValue = (long)(fValue*lcf)/lRatio;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ת����������ֵΪ���θ�����
	������wNo		-- ��������ֵ���
		lValue		-- ��������ֵ������
		*pfValue	--ת����ĸ�����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS ConvertRMFix2SecondFloat(WORD	wNo,long lValue,float *pfValue)
{
	TCF_Disp			tCFDisp;
	STATUS				rc;
	long				lCFValue = 1;
	long				lCFRatio = 1;
	
	// 1�����ָ��ĺϷ���
	ASSERT(pfValue != NULL);
	if(pfValue == NULL)
		return DB_ERR_PTRNULL;

	//ת����ֵΪ�����ʽ
	rc = DB_GetCFofRM(wNo, &lCFValue);
	if((rc != DB_ERR_OK)||(lCFValue == 0))
	{
		lCFValue = 1;
		LogError("ConvertRMFix2SecondFloat",FILE_LINE,"InValid CF !!!");
	}

	rc = DB_GetCFDispofRM(wNo, &tCFDisp);
	if((rc != DB_ERR_OK)||(tCFDisp.wCof == 0))
	{
		lCFRatio = 1;
		LogError("ConvertRMFix2SecondFloat",FILE_LINE,"InValid COF !!!");
	}
	else
		lCFRatio = tCFDisp.wCof;

	
	*pfValue = (float)(lValue*lCFRatio)/lCFValue;

	return rc;
}

/************************************************************
	���ܣ�ת����������ֵΪһ�θ�����
	������wNo		-- ��������ֵ���
		lValue		-- ��������ֵ������
		*pfValue	--ת����ĸ�����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS ConvertRMFix2FirstFloat(WORD	wNo,long lValue,float *pfValue)
{
	TCF_Disp			tCFDisp;
	STATUS				rc;
	long				lCFValue = 1;
	long				lCFRatio = 1;
	float				fRT = 1;
	long				FirRate=1;
	
	TRM_Disp		tRMDisp;
	// 1�����ָ��ĺϷ���
	ASSERT(pfValue != NULL);
	if(pfValue == NULL)
		return DB_ERR_PTRNULL;

	//ת����ֵΪ�����ʽ
	rc = DB_GetCFofRM(wNo, &lCFValue);
	if((rc != DB_ERR_OK)||(lCFValue == 0))
	{
		lCFValue = 1;
		LogError("ConvertRMFix2FirstFloat",FILE_LINE,"InValid CF !!!");
	}

	rc = DB_GetCFDispofRM(wNo, &tCFDisp);
	if((rc != DB_ERR_OK)||(tCFDisp.wCof == 0))
	{
		lCFRatio = 1;
		LogError("ConvertRMFix2FirstFloat",FILE_LINE,"InValid COF !!!");
	}
	else
		lCFRatio = tCFDisp.wCof;

	//һ������Ҫ�˱��
	rc = DB_GetRTofRM(wNo, &fRT);
	if(rc != DB_ERR_OK)
	{
		fRT = 1;
		LogError("ConvertRMFix2FirstFloat",FILE_LINE,"InValid RT !!!");
	}

	
	rc = DB_GetRMDisp(wNo, &tRMDisp); //��������ֵ��ʾһ��ʱҪ�����ƣ�yanzh 2012��7��14��15:33:13
	if(rc != DB_ERR_OK)
		return rc;
	
	switch(tRMDisp.nFirRate)   
	{
		case 0:  break; 
		case 1: FirRate = 10; break;	
		case 2: FirRate = 100; break;	
		case 3: FirRate = 1000; break;	
		default:  break;	
	}
	*pfValue = (float)(lValue*lCFRatio*fRT)/(lCFValue*FirRate);

	return rc;
}

//�������ϵ���ת����һ������
STATUS	ConvertRelayEvent2FirstData(TRelayRecordHead *pEvent)
{
	TRelayEventHead tRelayEvent;
	TRE_Disp		tREDisp;
	BYTE			*aEvent;
	BYTE			*pDataBuf;
	DWORD			dwValue;
	float			fValue;
	float			fRT = 1;

	DWORD			dwOffset;
	int 			m,n;
	STATUS			rc;
	TRM_Disp		tRMDisp;

	
	ASSERT(pEvent != NULL);
	if(pEvent == NULL)
	{
		LogError("DB_ConvertRelayEvent2FirstData",FILE_LINE,"pEvent Is NULL!!!");
		return DB_ERR_BUFNULL;
	}

	aEvent = (BYTE *)&pEvent->tAction;
	dwOffset = 0;

	// �������ֱ���
	for(n=0;n<pEvent->Num;n++)
	{
		// 1��ȡһ���ֱ���ͷ
		memcpy(&tRelayEvent,aEvent+dwOffset,sizeof(TRelayEventHead)); 	
	
		// 2��ȡ����ת�����ϵ���
		dwOffset += sizeof(tRelayEvent);
		pDataBuf = (BYTE *)&aEvent[dwOffset];
		
		// 2.1 ȡ��������������
		rc = DB_GetREDispByInf(tRelayEvent.nINF, &tREDisp);		
		if(rc != DB_ERR_OK)
		{
			LogError("DB_ConvertRelayEvent2FirstData",FILE_LINE,"InValid INF !!!");
			return rc;
		}
		
		// 2.2 ���ת�����ϵ���ֵ
		for(m=0;m<tRelayEvent.FD_Num;m++,pDataBuf+=6)
		{
			rc = DB_GetRMDisp(tREDisp.wRMList[m], &tRMDisp);
			if(rc != DB_ERR_OK)
				return rc;

			if(tRMDisp.wAttr == RM_ATTR_HEX)
				continue;
			
			// 2.2.1 ����ȡ������
			dwValue = MAKEDWORD(pDataBuf[2],pDataBuf[3],pDataBuf[4],pDataBuf[5]);
			fValue = *(float *)&dwValue;

			// 2.2.2 �ٳ��ϱ��
			rc = DB_GetRTofRM(tREDisp.wRMList[m], &fRT);
			if(rc != DB_ERR_OK)
			{
				fRT = 1;
				LogError("DB_ConvertRelayEvent2FirstData",FILE_LINE,"InValid RT !!!");
			}	
			fValue *= fRT;


			switch(tRMDisp.nFirRate)
			{
				case 0:  break;	
				case 1: fValue /= 10; break;	
				case 2: fValue /= 100; break;	
				case 3: fValue /= 1000; break;	
				default:  break;	
			}
			switch(tRMDisp.nFirDec)
			{
				case 0: fValue = (float)((int)fValue);break;	
				case 1: fValue = ((float)((int)(fValue*10)))/10; break;	
				case 2: fValue = ((float)((int)(fValue*100)))/100; break;	
				case 3: fValue = ((float)((int)(fValue*1000)))/1000; break;	
				default:  break;	
			}

			// 2.2.2 ��ת�ػ�����
			dwValue = *(DWORD *)&fValue;
			pDataBuf[2] = LLBYTE(dwValue);
			pDataBuf[3] = LHBYTE(dwValue);
			pDataBuf[4] = HLBYTE(dwValue);
			pDataBuf[5] = HHBYTE(dwValue);
		}
	
		// 3����������һ���ֱ���
		dwOffset += tRelayEvent.FD_Num*6;			
	}

	return DB_ERR_OK;
}


/************************************************************
	���ܣ������ݿ���ȡ��������ֵ������
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetRMNum()
{
	return G_pCfgTable->DBaseConfig.nRM;
}

/************************************************************
	���ܣ������ݿ���ȡ������������������
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetRENum()
{
	return G_pCfgTable->DBaseConfig.nRE;
}

/************************************************************
	���ܣ������ݿ���ȡ����������
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetTCNum()
{
	return G_pCfgTable->DBaseConfig.nTC;
}

/************************************************************
	���ܣ���ȡ��������ֵԭʼ���ݡ�
	������no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadRM( WORD no, long *retval )
{
	TRunTimeRM		*pRM;
	WORD			num;
	float			fValue;
	STATUS			rc;
			   
	if( retval == NULL )
		return DB_ERR_BUFNULL;

	pRM = G_pCfgTable->DBaseConfig.pRM;
	num = G_pCfgTable->DBaseConfig.nRM;

	// 1. ������
	if(no >= num)
	{
		LogError("DB_ReadRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. ���ָ��
	if( pRM == NULL )
	{
		LogError("DB_ReadRM",FILE_LINE,"The RM base is null!");
		return DB_ERR_BASENULL;
	}

	*retval = pRM[no].lValue;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�����ȡ���βౣ������ֵ���ݡ�
	������no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadSecondRM( WORD no, long *retval )
{
	TRM_Disp		tRMDisp;
	long			lValue;
	float			fValue;
	STATUS			rc;
			   
	rc = DB_ReadRM(no,&lValue);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_ReadSecondRM",FILE_LINE,"Read RM raw data error!");
		return rc;
	}

	// 3. ��������ֵ���л���
	rc = ConvertRMFix2SecondFloat(no, lValue, &fValue);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_ReadSecondRM",FILE_LINE,"The RM Convert Error!");
		return rc;
	}

	// 4. ���ն���ֵ��С��λ�����涨����
	if(DB_GetRMDisp(no,&tRMDisp) != DB_ERR_OK)											
	{
		LogError("DB_ReadSecondRM",FILE_LINE,"The RM Typr is Error!");
		fValue =  (long)(fValue*1000);
	}

	switch(tRMDisp.nSecDec)
	{
	case 0: *retval = (long)fValue; break;	
	case 1: *retval = (long)(fValue*10); break; 
	case 2: *retval = (long)(fValue*100); break;	
	case 3: *retval = (long)(fValue*1000); break;	
	case 4: *retval = (long)(fValue*10000); break;	
	default: *retval = (long)(fValue*100000); break;	
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�����ȡһ�βౣ������ֵ���ݡ�
	������no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadFirstRM( WORD no, long *retval )
{
	TRM_Disp		tRMDisp;
	TRunTimeRM		*pRM;
    WORD 			num;
	float			fValue;
    STATUS			rc;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pRM = G_pCfgTable->DBaseConfig.pRM;
	num = G_pCfgTable->DBaseConfig.nRM;
	
	// 1. ������
	if(no >= num)
	{
		LogError("DB_ReadFirstRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. ���ָ��
    if( pRM == NULL )
    {
        LogError("DB_ReadFirstRM",FILE_LINE,"The RM base is null!");
        return DB_ERR_BASENULL;
    }
	
	// 3. ��������ֵ���л���
	rc = ConvertRMFix2FirstFloat(no, pRM[no].lValue, &fValue);
	if( rc != DB_ERR_OK)
	{
        LogError("DB_ReadFirstRM",FILE_LINE,"The RM Convert Error!");
		return rc;
	}
	
	// 4. ����һ��ֵ��С��λ�����涨����
	if(DB_GetRMDisp(no,&tRMDisp) != DB_ERR_OK)											
	{
        LogError("DB_ReadFirstRM",FILE_LINE,"The RM disp is Error!");
		fValue =  (long)(fValue*1000);
	}

	switch(tRMDisp.nFirDec)
	{
	case 0: *retval = (long)fValue; break;	
	case 1: *retval = (long)(fValue*10); break;	
	case 2: *retval = (long)(fValue*100); break;	
	case 3: *retval = (long)(fValue*1000); break;	
	case 4: *retval = (long)(fValue*10000); break;	
	default: *retval = (long)(fValue*100000); break;	
	}
	
	return DB_ERR_OK;
}


/************************************************************
	���ܣ���ң�����дһ����������ֵ���ݡ�
	������no,		--  ���
		  value,	--  ����ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_WriteRM(WORD no, long value )
{
	TRunTimeRM		*pRM;
    WORD 			num;
               	
	pRM = G_pCfgTable->DBaseConfig.pRM;
	num = G_pCfgTable->DBaseConfig.nRM;
	
	// ������
	if(no >= num)
	{
		LogError("DB_WriteRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pRM == NULL )
    {
        LogError("DB_WriteRM",FILE_LINE,"The RM base is null!");
        return DB_ERR_BASENULL;
    }
	
	pRM[no].lValue = value;
		
	return DB_ERR_OK;
}

CNode *DB_RelayCacheSearch (WORD wSeq)
{
	TRelayEventCache	*pRelayCache;
    CNode 				*pTempNode;
	CLink 				*pLink;

	
	pLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	if(pLink == NULL)
		return NULL;
	
	pTempNode = pLink->GetHead();
	//�ж������Ƿ�Ϊ����,����Ϊ��������
	if(pTempNode == NULL)
		return NULL;

	do
    {
		pRelayCache = (TRelayEventCache	*)pTempNode->pData;
        if ( pRelayCache->nSeq == wSeq)
        {
            return pTempNode;
        }

		//�����¸��ڵ�
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != pLink->GetHead()));

	return NULL;
}

CNode *DB_GetEndedRelayCache ()
{
	TRelayEventCache	*pRelayCache;
    CNode 				*pTempNode;
	CNode 				*pOlderNode= NULL;
	CLink 				*pLink;
	WORD				OlderTimeHi=0xFFFF;					//����ʱ�䡣����ʱ��
	DWORD				OlderTimeLo=0xFFFFFFFF;				//����ʱ�䡣����ʱ��
	
	
	pLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	if(pLink == NULL)
		return NULL;
	
	pTempNode = pLink->GetHead();
	//�ж������Ƿ�Ϊ����,����Ϊ��������
	if(pTempNode == NULL)
		return NULL;
	do
    {
		pRelayCache = (TRelayEventCache	*)pTempNode->pData;

		// 1.�Ƿ��д����̽ڵ�
        if ( pRelayCache->bStatus&RELAY_CACHE_END )
        {
			// 2.ȡ���ϵ��¼�
			if((pRelayCache->pAction->stHead.AbsTimeHi < OlderTimeHi)||\
				((pRelayCache->pAction->stHead.AbsTimeHi == OlderTimeHi)&&(pRelayCache->pAction->stHead.AbsTimeLo < OlderTimeLo)))
			{
				pOlderNode = pTempNode;
				OlderTimeHi = pRelayCache->pAction->stHead.AbsTimeHi;
				OlderTimeLo = pRelayCache->pAction->stHead.AbsTimeLo;
			}					
        }

		//�����¸��ڵ�
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != pLink->GetHead()));

	return pOlderNode;
}

//��CACHE�еı�������������붯���������
STATUS	DB_RelayCache2Event(TRelayActionFix *pAction,TRelayEventHead *pEvent)
{
	TRE_Disp 			tRelayInfDisp;
	TRM_Disp 			tRMDisp;
	DWORD				dwFDData;
	STATUS				rc;
	
	BYTE				*pFDData;
	int					i;
	
	ASSERT(pAction != NULL);
	if(pAction == NULL)
	{
		LogError("DB_RelayCache2Event",FILE_LINE,"Cache Is NULL!!!");
		return DB_ERR_BUFNULL;
	}

	ASSERT(pEvent != NULL);
	if(pEvent == NULL)
	{
		LogError("DB_RelayCache2Event",FILE_LINE,"pEvent Is NULL!!!");
		return DB_ERR_BUFNULL;
	}
		
	memcpy(pEvent,&pAction->stHead,sizeof(TRelayEventHead));

	// 1 ��ȡ������������
	rc = DB_GetREDispByInf(pAction->stHead.nINF,&tRelayInfDisp);
	ASSERT(rc == DB_ERR_OK);
	if(rc != DB_ERR_OK)
	{
		return DB_ERR_RELAY_INF;
	}
	
	pFDData = (BYTE *)(pEvent+1);
	for(i=0;i<pEvent->FD_Num;i++)
	{	
		if(DB_GetRMDisp(tRelayInfDisp.wRMList[i], &tRMDisp) == DB_ERR_OK)
		{
			pFDData[0] = LOBYTE(tRMDisp.nIndex);
			pFDData[1] = HIBYTE(tRMDisp.nIndex);
		}
		else
		{
			LogError("DB_RelayCache2Event",FILE_LINE,"DB_GetRMDisp(%d) is error!!!",tRelayInfDisp.wRMList[i]);
			pFDData[0] = LOBYTE(tRelayInfDisp.wRMList[i]);
			pFDData[1] = HIBYTE(tRelayInfDisp.wRMList[i]);
		}
		
		// 2.���ձ�������ֵ��ת����ֵΪ�����ʽ
		if(tRMDisp.wAttr == RM_ATTR_HEX)
			dwFDData = pAction->FD_Value[i];
		else 
		{
//			if(DB_IsFDShowFirst())
//				rc = ConvertRMFix2FirstFloat(tRelayInfDisp.wRMList[i],pAction->FD_Value[i],(float *)&dwFDData);
//			else
				rc = ConvertRMFix2SecondFloat(tRelayInfDisp.wRMList[i],pAction->FD_Value[i],(float *)&dwFDData);
		}
		
		if(rc != DB_ERR_OK)
		{
			LogError("DB_RelayCache2Event",FILE_LINE,"FD Fix to Float Convert Error!!!");
		}
				
		pFDData[2] = LLBYTE(dwFDData);
		pFDData[3] = LHBYTE(dwFDData);
		pFDData[4] = HLBYTE(dwFDData);
		pFDData[5] = HHBYTE(dwFDData);
	
		pFDData += 6;
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�������������ע��
	������wSeq		-- ȫ����ʱ��ţ��������ֲ���������������Ϣ���������ڽ��ᴩ��һ���������������������
		wInf		--���������������
		*pTime		--����ʱ��
		bMakeStart	--�Ƿ��¼������������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_RelayStart (WORD wSeq, WORD wInf, TAbsTime *pTime,BOOL bMakeStart)
{
	TRelayEventCache	*pRelayCache;
	TRelayEventHead		*pEventHead;
	CLink				*pFreeLink;
	CLink				*pSaveLink;
	CNode				*pNode;
	TAbsTime			tTime;

//	ReportMsg("start wSeq=%d,inf=%d",wSeq,wInf);
	// 1�����CACHE����ĺϷ���
	pSaveLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	pFreeLink = &G_RelayRepFreeLink;//G_pCfgTable->DBaseConfig.pRelayRepFreeLink;

	ASSERT((pSaveLink != NULL)&&(pFreeLink != NULL));
	if( (pSaveLink == NULL)||(pFreeLink == NULL))
	{
		LogError("DB_RelayStart",FILE_LINE,"The Cache Link is NULL");
		return DB_ERR_BASENULL;
	}
	
	// 2�������ŵĺϷ���
	if( DB_RelayCacheSearch(wSeq) != NULL)
	{
		//��Ų��Ϸ���Ϊ���ⱨ�Ļ��ң�ֱ�ӷ���
		LogError("DB_RelayStart",FILE_LINE,"SEQ Error!!!");
		return DB_ERR_RELAY_SEQ;
	}
	
	OS_Protect(0);
	
	// 3������һ������CANCHE�ڵ�
	pNode = pFreeLink->RemoveHead();
	if(pNode == NULL)
	{
		OS_UnProtect(0);
		LogError("DB_RelayStart",FILE_LINE,"The Report Cache run out!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
	pRelayCache = (TRelayEventCache	*)pNode->pData;
	// ����ʧ��ֱ�ӷ���
	ASSERT(pRelayCache != NULL);
	
	// 4��ע��CACHE�ڵ������Ϣ
	pRelayCache->bStatus = 0;
	
	if(bMakeStart&&(wInf != RELAY_INF_NOUSED))
		pRelayCache->bStatus |= RELAY_CACHE_MAKESTART;
	else
		pRelayCache->bStatus &= ~RELAY_CACHE_MAKESTART;
	
	pRelayCache->bStatus 	&= ~RELAY_CACHE_STATUSMASK;
	pRelayCache->bStatus 	|= RELAY_CACHE_START;
	pRelayCache->nSeq 		= wSeq;
	pRelayCache->nCnt 		= 1;
	pRelayCache->dwTimeOut 	= RELAY_CACHE_HOLDTIME;

	pEventHead = &pRelayCache->pAction[0].stHead;
	pEventHead->nINF	= wInf;
	pEventHead->bStatus = RELAYEVENT_STATUS_ACT;
	pEventHead->FD_Num  = 0;

	if(pTime == NULL)
	{
		ReadAbsTime(&tTime);
		pEventHead->AbsTimeHi = tTime.Hi;
		pEventHead->AbsTimeLo = tTime.Lo;	
	}
	else
	{
		pEventHead->AbsTimeHi = pTime->Hi;
		pEventHead->AbsTimeLo = pTime->Lo;	
	}
	// 5���������������
	pSaveLink->AddTail(pNode);
		
	OS_UnProtect(0);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�д�뱣�������Ĺ��ϵ���
	������Seq	-- ȫ����ʱ��ţ�ͬ�ϡ�
	  pFDList	-- ���ϵ����б�
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_RelayWriteFD (WORD Seq, long *pFDList)
{
	TRE_Disp 			tRelayInfDisp;
	TRelayEventCache	*pCache;
	TRelayActionFix		*pEvent;
    CNode 				*pNode;
	STATUS				rc;
	
	// 1��Ѱ�Ҷ�Ӧ��ŵ�CACHE����
	pNode = DB_RelayCacheSearch(Seq);
	ASSERT(pNode != NULL);
	if(pNode == NULL)
	{
		LogError("DB_RelayWriteFD",FILE_LINE,"InValid SEQ !!!");
		return DB_ERR_RELAY_SEQ;
	}

	OS_Protect(0);
	
	// 2����ȡINF��Ϣ
	pCache = (TRelayEventCache *)pNode->pData;

	if(pCache->bStatus&RELAY_CACHE_END)
	{
		OS_UnProtect(0);
		LogError("DB_RelayWriteFD",FILE_LINE,"SEQ%d is end!!!",Seq);
		return DB_ERR_RELAY_SEQ;
	}
	
	pEvent = &pCache->pAction[0];
	if(pEvent->stHead.nINF == RELAY_INF_NOUSED)
	{
		OS_UnProtect(0);
		return DB_ERR_OK;
	}

	rc = DB_GetREDispByInf(pEvent->stHead.nINF,&tRelayInfDisp);
	ASSERT(rc == DB_ERR_OK);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_RelayWriteFD",FILE_LINE,"InValid INF!!!");
		OS_UnProtect(0);
		return DB_ERR_RELAY_INF;
	}
	
	// 3��д����ϵ���
	pEvent->stHead.FD_Num = tRelayInfDisp.nFD;
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		//pEvent->FD_Index[i] = tRelayInfDisp.wFDList[i];
		pEvent->FD_Value[i] = pFDList[i];
	}

	pCache->dwTimeOut 	= RELAY_CACHE_HOLDTIME;
	
	OS_UnProtect(0);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�д�뱣�������ֱ���
	������Seq	-- ȫ����ʱ��ţ�ͬ�ϡ�
		pAction	-- �����ֱ�����Ϣָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_RelayAction (WORD Seq, TRelayEventItem *pAction)
{
	TRE_Disp 			tRelayInfDisp;
	TRelayEventCache	*pCache;
	TRelayActionFix		*pEvent;
    CNode 				*pNode;
	STATUS				rc;

	// 1���������ָ��ĺϷ���
	ASSERT(pAction != NULL);
	if(pAction == NULL)
		return DB_ERR_PTRNULL;
	
	// 2��Ѱ�Ҷ�Ӧ��ŵ�CACHE����
	pNode = DB_RelayCacheSearch(Seq);
	ASSERT(pNode != NULL);
	if(pNode == NULL)
	{
		LogError("DB_RelayAction",FILE_LINE,"InValid SEQ !!!");
		return DB_ERR_RELAY_SEQ;
	}
	
	pCache = (TRelayEventCache *)pNode->pData;
	ASSERT(pCache != NULL);
	if(pCache == NULL)
	{
		LogError("DB_RelayAction",FILE_LINE,"Cache Is Empty!!!");
		return DB_ERR_RELAY_NOCACHE;
	}

	if(pCache->bStatus&RELAY_CACHE_END)
	{
		LogError("DB_RelayAction",FILE_LINE,"SEQ%d is end!!!",Seq);
		return DB_ERR_RELAY_SEQ;
	}
	
	if(pCache->nCnt >= MAX_SEGACT_NUM)
	{
		LogError("DB_RelayAction",FILE_LINE,"Too Many Part Report!!!");
		return DB_ERR_RELAY_OVERFLOW;
	}
	
	// 3����ȡINF��Ϣ
	rc = DB_GetREDispByInf(pAction->nINF,&tRelayInfDisp);
	ASSERT(rc == DB_ERR_OK)
	if(rc != DB_ERR_OK)
	{
		LogError("DB_RelayAction",FILE_LINE,"InValid INF(%d)!!!",pAction->nINF);
		return DB_ERR_RELAY_INF;
	}
	
	OS_Protect(0);
	
	// 3��д�뱨��
	pEvent = &pCache->pAction[pCache->nCnt];
	
	pEvent->stHead.nINF 	= pAction->nINF;
	pEvent->stHead.bStatus 	= pAction->bStatus&RELAYEVENT_STATUS_MASK;
	pEvent->stHead.FD_Num 	= tRelayInfDisp.nFD;
	pEvent->stHead.AbsTimeHi= pAction->stTime.Hi;
	pEvent->stHead.AbsTimeLo= pAction->stTime.Lo;
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		//pEvent->FD_Index[i] = tRelayInfDisp.wFDList[i];
		pEvent->FD_Value[i] = pAction->FD_Value[i];
	}
	
	// 2������CACHE��Ϣ
	pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
	pCache->bStatus |= RELAY_CACHE_ACTED;
	pCache->dwTimeOut= RELAY_CACHE_HOLDTIME;
	pCache->nCnt++;
	
//	ReportMsg("act wSeq=%d,inf=%d",Seq,pAction->nINF);
	OS_UnProtect(0);

	return DB_ERR_OK;
}
	
/************************************************************
	���ܣ�������������
	������Seq	-- ȫ����ʱ��ţ�ͬ�ϡ�
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_RelayEnd (WORD Seq,BOOL bValid)
{
	TRelayEventCache	*pCache;
    CNode 				*pCacheNode;
	
	// 1��Ѱ�Ҷ�Ӧ��ŵ�CACHE����
	pCacheNode = DB_RelayCacheSearch(Seq);
	ASSERT(pCacheNode != NULL);
	if(pCacheNode == NULL)
	{
		LogError("DB_RelayEnd",FILE_LINE,"InValid SEQ !!!");
		return DB_ERR_RELAY_SEQ;
	}
	
	pCache = (TRelayEventCache *)pCacheNode->pData;
	ASSERT(pCache != NULL);
	if(pCache == NULL)
	{
		LogError("DB_RelayEnd",FILE_LINE,"Cache Is Empty!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
	
	OS_Protect(0);

	// 2������CACHE��Ϣ
	if(!bValid)
	{
		pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
		pCache->bStatus  = RELAY_CACHE_READY;
		pCache->dwTimeOut= 0;

		// �ͷŴ洢����
		G_RelayRepSaveLink.RemoveNode(pCacheNode);
		G_RelayRepFreeLink.AddTail(pCacheNode);
		
		LogError("DB_RelayEnd",FILE_LINE,"Cache is abandoned!");

		OS_UnProtect(0);
		
		return DB_ERR_OK;
	}

	pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
	pCache->bStatus |= RELAY_CACHE_END;
	pCache->dwTimeOut= RELAY_CACHE_HOLDTIME;

	OS_UnProtect(0);

//	ReportMsg("end wSeq=%d",Seq);

	OS_PostMessage(g_hDbaseTaskID, SM_SAVEREPORT, 1, Seq, 0, 0);
	
	return DB_ERR_OK;
}
/************************************************************
	���ܣ�д�뱣�������ֱ���
	������Seq	-- ȫ����ʱ��ţ�ͬ�ϡ�
		pAction	-- �����ֱ�����Ϣָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_RelayAlarm (TRelayEventItem *pAlarm)
{
	TRE_Disp 			tRelayInfDisp;
	TRM_Disp 			tRMDisp;
	TRelayAlarmEv		tAlarmEv;
	TRelayEventHead		*pEvent;
	TRelayRecordHead	*pRecord;
	BYTE				*pFDData;
	STATUS				rc;
	WORD				wLength;
	WORD				wFDIndex;
	DWORD				dwFDData;

	// 1���������ָ��ĺϷ���
	if( pAlarm == NULL )
		 return DB_ERR_BUFNULL;

	// 2����ȡINF��Ϣ
	rc = DB_GetREDispByInf(pAlarm->nINF,&tRelayInfDisp);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_RelayAlarm",FILE_LINE,"InValid INF %d!!!",pAlarm->nINF);
		return DB_ERR_RELAY_INF;
	}

	// 3����鱨�泤�Ȳ���Խ��
	wLength = sizeof(TRelayRecordHead)+tRelayInfDisp.nFD*(sizeof(WORD)+sizeof(float));
	ASSERT(wLength<=MAX_RALM_LENGTH);
	if(wLength > MAX_RALM_LENGTH)
	{
		LogError("DB_RelayAlarm",FILE_LINE,"ALARM REPORT too long!!! FD=%d",tRelayInfDisp.nFD);
		return DB_ERR_RELAY_OVERFLOW;
	}
	
	// 4����䱨��ͷ
	pRecord = &tAlarmEv.tEvent;
	pRecord->Num = 1;
	pRecord->wFaultNo = 0xFFFF;
	
	pEvent = &pRecord->tAction;
	pEvent->bStatus = pAlarm->bStatus;
	pEvent->nINF 	= pAlarm->nINF;
	pEvent->FD_Num 	= tRelayInfDisp.nFD;
	pEvent->AbsTimeHi	= pAlarm->stTime.Hi;
	pEvent->AbsTimeLo	= pAlarm->stTime.Lo;
		
	// 5��д����ϵ���
	pFDData = tAlarmEv.aEvent+sizeof(TRelayRecordHead);
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		if(DB_GetRMDisp(tRelayInfDisp.wRMList[i], &tRMDisp) == DB_ERR_OK)
			wFDIndex = tRMDisp.nIndex;
		else
		{
			LogError("DB_RelayAlarm",FILE_LINE,"DB_GetRMDisp(%d) is error!!!",tRelayInfDisp.wRMList[i]);
			wFDIndex = tRelayInfDisp.wRMList[i];
		}
		
		// 2.���ձ�������ֵ��ת����ֵΪ�����ʽ
		if(tRMDisp.wAttr == RM_ATTR_HEX)
			dwFDData = pAlarm->FD_Value[i];
		else 
		{
//			if(DB_IsFDShowFirst())
//				rc = ConvertRMFix2FirstFloat(tRelayInfDisp.wRMList[i],pAlarm->FD_Value[i],(float *)&dwFDData);
//			else
				rc = ConvertRMFix2SecondFloat(tRelayInfDisp.wRMList[i],pAlarm->FD_Value[i],(float *)&dwFDData);
		}

		if(rc != DB_ERR_OK)
		{
			LogError("DB_RelayAlarm",FILE_LINE,"FD Error!!!");
			return DB_ERR_RELAY_FD;
		}

		pFDData[0] = LOBYTE(wFDIndex);
		pFDData[1] = HIBYTE(wFDIndex);
		pFDData[2] = LLBYTE(dwFDData);
		pFDData[3] = LHBYTE(dwFDData);
		pFDData[4] = HLBYTE(dwFDData);
		pFDData[5] = HHBYTE(dwFDData);

		pFDData += 6;
	}

	// 6��д�뱨��
	return DB_WriteRelayAlarmReport(&tAlarmEv);
}

/************************************************************
	���ܣ��������������
	������	wInf		--���������������
		 	*pTime		--����ʱ�䣬��ָ��ʱȡϵͳ��ǰʱ��
			pFDList		--���ϵ����б�
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_MakeRelayEventReport(WORD wInf, TAbsTime *pTime,long *pFDList)
{
	TRE_Disp 			tRelayInfDisp;
	TRelayStartEv		tStartEv;
	TAbsTime			tEventTime;
	TRM_Disp			tRMDisp;
	TRelayEventHead		*pEvent;
	TRelayRecordHead	*pRecord;
	BYTE				*pFDData;
	STATUS				rc;
	WORD				wLength;
	WORD				wFDIndex;
	DWORD				dwFDData;

	// 1����ȡINF��Ϣ
	rc = DB_GetREDispByInf(wInf,&tRelayInfDisp);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_MakeRelayEventReport",FILE_LINE,"InValid INF %d!!!",wInf);
		return DB_ERR_RELAY_INF;
	}

	// 3����鱨�泤�Ȳ���Խ��
	wLength = sizeof(TRelayRecordHead)+tRelayInfDisp.nFD*(sizeof(WORD)+sizeof(float));
	ASSERT(wLength<=MAX_RSTT_LENGTH);
	if(wLength > MAX_RSTT_LENGTH)
	{
		LogError("DB_MakeRelayEventReport",FILE_LINE,"Relay event too long!!!");
		return DB_ERR_RELAY_OVERFLOW;
	}
	
	// 4����䱨��ͷ
	pRecord = &tStartEv.tEvent;
	pRecord->Num = 1;
	pRecord->wFaultNo = 0x33CC;
	
	pEvent = &pRecord->tAction;
	pEvent->bStatus = 1;
	pEvent->nINF 	= wInf;
	pEvent->FD_Num 	= tRelayInfDisp.nFD;

	if(pTime != NULL)
		tEventTime = *pTime;
	else
		ReadAbsTime(&tEventTime);
		
	pEvent->AbsTimeHi	= tEventTime.Hi;
	pEvent->AbsTimeLo	= tEventTime.Lo;
		
	// 5��д����ϵ���
	pFDData = tStartEv.aEvent+sizeof(TRelayRecordHead);
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		if(DB_GetRMDisp(tRelayInfDisp.wRMList[i], &tRMDisp) == DB_ERR_OK)
			wFDIndex = tRMDisp.nIndex;
		else
		{
			LogError("DB_MakeRelayEventReport",FILE_LINE,"DB_GetRMDisp(%d) is error!!!",tRelayInfDisp.wRMList[i]);
			wFDIndex = tRelayInfDisp.wRMList[i];
		}

		rc = ConvertRMFix2SecondFloat(tRelayInfDisp.wRMList[i],pFDList[i],(float *)&dwFDData);
		if(rc != DB_ERR_OK)
		{
			LogError("DB_MakeRelayEventReport",FILE_LINE,"FD Error!!!");
			return DB_ERR_RELAY_FD;
		}

		pFDData[0] = LOBYTE(wFDIndex);
		pFDData[1] = HIBYTE(wFDIndex);
		pFDData[2] = LLBYTE(dwFDData);
		pFDData[3] = LHBYTE(dwFDData);
		pFDData[4] = HLBYTE(dwFDData);
		pFDData[5] = HHBYTE(dwFDData);

		pFDData += 6;
	}

	// 6��д�뱨��
	return DB_WriteRelayStartReport(&tStartEv);
}

/************************************************************
	���ܣ�����������ʱ����
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_SaveRelayActionOnTime()
{
	TRelayEventHead		*pEvent;
	TRelayStartEv		tStartEv;
	TRelayActionEv		tActionEv;
	TRelayRecordHead	*pRecord;
    CNode 				*pCacheNode;
	TRelayEventCache	*pCache;
	WORD				wItemLen,wOffset,wStartInf;
	int					i;
	
	// 1��Ѱ�Ҵ����̵�CACHE����
	do{
		
		pCacheNode = DB_GetEndedRelayCache();
		if(pCacheNode == NULL)
			break;
		
		pCache = (TRelayEventCache *)pCacheNode->pData;
		ASSERT(pCache != NULL);
		if(pCache == NULL)
		{
			LogError("DB_SaveRelayActionOnTime",FILE_LINE,"Cache Is Empty!!!");
			break;
		}
		
		// 3��д�뱨��
		// 3��1 д����������
		
		if(pCache->bStatus & RELAY_CACHE_MAKESTART)
		{
			if((pCache->nCnt <= 1)&&(pCache->pAction->stHead.FD_Num == 0))
			{
				//���ֻ��һ���������棬����û�й��ϵ��������ٴ���
			}
			else
			{
				pRecord = &tStartEv.tEvent;
				pRecord->Num = 1;
				pRecord->wFaultNo = g_wRelayDistNo;
				
				//��֯��������
				pEvent = &pRecord->tAction;

				//���泤�Ȳ���Խ��
				wItemLen = sizeof(TRelayRecordHead)+pCache->pAction[0].stHead.FD_Num*(sizeof(WORD)+sizeof(float));
				ASSERT(wItemLen<=MAX_RSTT_LENGTH);
				if(wItemLen<=MAX_RSTT_LENGTH)
					DB_RelayCache2Event(pCache->pAction, pEvent);
				else
				{
					LogError("DB_SaveRelayActionOnTime",FILE_LINE,"START REPORT too long!!!");
				}
				
				if(DB_WriteRelayStartReport(&tStartEv,NULL) != DB_ERR_OK)
				{
					LogError("DB_SaveRelayActionOnTime",FILE_LINE,"Save START REPORT error!!!");
				}
			}
		}
		
		// 3��2 д�붯������
		if(pCache->nCnt > 1)
		{
			pRecord = &tActionEv.tEvent;
			pEvent	= &pRecord->tAction;
			
			pRecord->Num = 0;
			wOffset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
			
			for(i=0;i<pCache->nCnt;i++)
			{
				wStartInf = pCache->pAction[i].stHead.nINF;
				if(wStartInf == RELAY_INF_NOUSED)
					continue;
				
				// 3��2��1 ����ֱ��泤��
				wItemLen = sizeof(TRelayEventHead)+pCache->pAction[i].stHead.FD_Num*(sizeof(WORD)+sizeof(float));

				// 3��2��2 �ۼƷֱ���֮ǰ���г����ж�
				if((wOffset + wItemLen) > (MAX_RACT_LENGTH-2))
				{
					// 1���ȴ�һ������
					pRecord->wFaultNo = g_wRelayDistNo;
					DB_WriteRelayActionReport(&tActionEv,NULL);
					g_wRelayDistNo = (g_wRelayDistNo+1)%10000;

					// 3���������һ����������
					memset(&tActionEv,0,sizeof(TRelayActionEv));
					pEvent	= &pRecord->tAction;
					
//					if(pCache->pAction[0].stHead.nINF != RELAY_INF_NOUSED)
//					{
//						DB_RelayCache2Event(&pCache->pAction[0], pEvent);
//						wOffset = sizeof(TRelayRecordHead)+pCache->pAction[0].stHead.FD_Num*(sizeof(WORD)+sizeof(float));

//						// 4���������ָ��
//						pEvent = (TRelayEventHead *)(tActionEv.aEvent+wOffset);
//						pRecord->Num = 1;
//					}
//					else
					{
						pRecord->Num = 0;
						wOffset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
					}
				}
				
				// ������Ӻ����ֱ���
				DB_RelayCache2Event(&pCache->pAction[i], pEvent);
				wOffset += wItemLen;
				
				pRecord->Num++;
				pEvent = (TRelayEventHead *)(tActionEv.aEvent+wOffset);
			}

			pRecord->wFaultNo = g_wRelayDistNo;
			DB_WriteRelayActionReport(&tActionEv, NULL);
			g_wRelayDistNo = (g_wRelayDistNo+1)%10000;
		}
		
		OS_Protect(0);

		// 4���ͷŴ洢����
		G_RelayRepSaveLink.RemoveNode(pCacheNode);
		G_RelayRepFreeLink.AddTail(pCacheNode);
		
		pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
		pCache->bStatus = RELAY_CACHE_READY;
		pCache->dwTimeOut= 0;

//		LogError("DB_SaveRelayActionOnTime",FILE_LINE,"<------INF:%d Num:%d  FreeNodeNum=%x SaveNodeNum=%x------>",tActionEv.tEvent.wFaultNo,pCache->nCnt,\
	//		(DWORD)G_RelayRepFreeLink.GetNodeNum(),(DWORD)G_RelayRepSaveLink.GetNodeNum());

		OS_UnProtect(0);
	
	}while(pCacheNode != NULL);
	
	return DB_ERR_OK;
}
/************************************************************
	���ܣ����豸���ж�ȡ�����澯����
	������ *pReport 	--  �����ı����澯��Ϣ
			rp    		--  �ⲿ��ָ�루ά���ã���һ��������BLANK
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_ReadRelayAlarmReport(TRelayAlarmEv *pReport, WORD rp)
{
    TRelayAlarmWrap 	*pWrap;
	TAppInfo			*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( pReport==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nRALMRead%MAX_RALM_NUM;
    }
    else
    {
		rp = rp%MAX_RALM_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*pReport = pWrap->pEvent[rp];

	// ��ⱨ��У�飬Modify by lz. 2013.10.08
	if(!DAE_IsCheckSumOK(pReport->aEvent,MAX_RALM_LENGTH-2))
		return DB_ERR_CHECK;
		
	// ��ʾһ�ι��ϵ�������£���Ҫת��
	if(DB_IsFDShowFirst())
		ConvertRelayEvent2FirstData(&pReport->tEvent);

	// 5�������ȡ���ֱ����������־
    DB_ClearEventFlag(pAppInfo,APP_EVENT_RALM);
                                     
	return DB_ERR_OK;
}

/************************************************************
	���ܣ����豸����д�뱣���澯����
	������ *pReport 	--  ��д��ı����澯��Ϣ                      
			pTime 		--  �����澯������ʱ�䡣��Ϊ��ʱ��ʹ��ϵͳʱ�䡣
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_WriteRelayAlarmReport(TRelayAlarmEv *pReport,TAbsTime *pTime)
{
    TRelayAlarmWrap 	*pWrap;
    TAppInfo 			*pAppInfo;
    WORD 				i, wWritePtr;
                     
	// 1���������ָ��ĺϷ���
	if( pReport == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// ���㱨��У�飬Modify by lz. 2013.10.08
	DAE_MakeCheckSum(pReport->aEvent,MAX_RALM_LENGTH-2);
	
	// 3��������
	wWritePtr = pWrap->nTailIndex;

	OS_Protect(0);
	
	pWrap->pEvent[wWritePtr] = *pReport;
    pWrap->nTailIndex = (wWritePtr+1)%MAX_RALM_NUM;
	
	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RALM_NUM;

	OS_UnProtect(0);
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	
	for(i=0;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
	{	
		if(!(pAppInfo->bMask & APP_EVENT_RALM))
		{
			pAppInfo->nRALMRead = pWrap->nTailIndex;
			continue;
		}

		if( pAppInfo->nRALMRead == pWrap->nTailIndex )
			 pAppInfo->nRALMRead = (pWrap->nTailIndex+1)%MAX_RALM_NUM;

		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_RALM))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_RALM);

			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_RELAYEVENT,2,wWritePtr,0,0);
		}
	}

	DB_RegisterFileChange(DB_FILE_RELAYEV);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����澯���Ŀ���δ��ȡ���ĵ���Ŀ
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayAlarmReportNum(WORD *retval)
{
	TRelayAlarmWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3����ȡ��ָ��    
    rc = DB_GetRelayAlarmReportRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_RALM_NUM-rp)%MAX_RALM_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ���ȡ�����澯���Ŀ�Ķ�ָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayAlarmReportRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // ������������ָ��
    *retval = pAppInfo->nRALMRead%MAX_RALM_NUM;

	return DB_ERR_OK;
}
/************************************************************
	���ܣ���ȡ�����澯��Ϣ��������ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayAlarmReportHead(WORD *retval)
{
	TRelayAlarmWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RALM_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����澯��Ϣ��������βָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayAlarmReportTail(WORD *retval)
{
	TRelayAlarmWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RALM_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ����������澯��Ϣ��������ͷָ��
	��������
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_IncRelayAlarmReportRead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nRALMRead = (pAppInfo->nRALMRead+1)%MAX_RALM_NUM;

	//DB_RegisterFileChange(DB_FILE_RELAYEV);
		
	return DB_ERR_OK;
}

/************************************************************
	���ܣ����豸���ж�ȡ������������
	������ *pReport 	--  �����ı���������Ϣ
			rp    		--  �ⲿ��ָ�루ά���ã���һ��������BLANK
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_ReadRelayStartReport(TRelayStartEv *pReport, WORD rp)
{
    TRelayStartWrap 	*pWrap;
	TAppInfo			*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( pReport==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nRSTTRead%MAX_RSTT_NUM;
    }
    else
    {
		rp = rp%MAX_RSTT_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*pReport = pWrap->pEvent[rp];

	
	// ��ʾһ�ι��ϵ�������£���Ҫת��
	if(DB_IsFDShowFirst())
		ConvertRelayEvent2FirstData(&pReport->tEvent);

	// 5�������ȡ���ֱ����������־
    DB_ClearEventFlag(pAppInfo,APP_EVENT_RSTT);
                                     
	return DB_ERR_OK;
}

/************************************************************
	���ܣ����豸����д�뱣����������
	������ *pReport 	--  �����ı���������Ϣ                     
			pTime 		--  ��������������ʱ�䣬�������ڲ�������ң�ű仯�¼��á���Ϊ��ʱ��ʹ��ϵͳʱ�䡣
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_WriteRelayStartReport(TRelayStartEv *pReport, TAbsTime *pTime)
{
    TRelayStartWrap 	*pWrap;
    TAppInfo 			*pAppInfo;
    WORD 				i, wWritePtr;
                     
	// 1���������ָ��ĺϷ���
	if( pReport == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3��������
	wWritePtr = pWrap->nTailIndex;

	OS_Protect(0);
	
    pWrap->pEvent[wWritePtr] = *pReport;
    pWrap->nTailIndex = (wWritePtr+1)%MAX_RSTT_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RSTT_NUM;

	OS_UnProtect(0);
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_RSTT))
		{
			pAppInfo->nRSTTRead = pWrap->nTailIndex;
			continue;
		}
		
	    if( pAppInfo->nRSTTRead == pWrap->nTailIndex )
	         pAppInfo->nRSTTRead = (pWrap->nTailIndex+1)%MAX_RSTT_NUM;

		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_RSTT))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_RSTT);
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_RELAYEVENT, 0,wWritePtr,0,0);
		}
	}

//	DB_RegisterFileChange(DB_FILE_BASICEV);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����������Ŀ���δ��ȡ���ĵ���Ŀ
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayStartReportNum(WORD *retval)
{
	TRelayStartWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3����ȡ��ָ��    
    rc = DB_GetRelayStartReportRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_RSTT_NUM-rp)%MAX_RSTT_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ���ȡ�����������Ŀ�Ķ�ָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayStartReportRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // ������������ָ��
    *retval = pAppInfo->nRSTTRead%MAX_RSTT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ����������Ϣ��������ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayStartReportHead(WORD *retval)
{
	TRelayStartWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RSTT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ����������Ϣ��������ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayStartReportTail(WORD *retval)
{
	TRelayStartWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RSTT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���������������Ϣ��������ͷָ��
	��������
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_IncRelayStartReportRead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nRSTTRead = (pAppInfo->nRSTTRead+1)%MAX_RSTT_NUM;

	// 3���洢ָ��
	//DB_RegisterFileChange(DB_FILE_BASICEV);
	
	return DB_ERR_OK;
}


/************************************************************
	���ܣ����豸���ж�ȡ�����澯����
	������	*pReport 	--  �����ı���������Ϣ
			rp    		--  �ⲿ��ָ�루ά���ã���һ��������BLANK
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_ReadRelayActionReport(TRelayActionEv *pReport, WORD rp)
{
    TRelayActionWrap 	*pWrap;
	TAppInfo			*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( pReport==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nRACTRead%MAX_RACT_NUM;
    }
    else
    {
		rp = rp%MAX_RACT_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*pReport = pWrap->pEvent[rp];

	// ��ⱨ��У�飬Modify by lz. 2013.10.08
	if(!DAE_IsCheckSumOK(pReport->aEvent,MAX_RACT_LENGTH-2))
		return DB_ERR_CHECK;
	
	// ��ʾһ�ι��ϵ�������£���Ҫת��
	if(DB_IsFDShowFirst())
		ConvertRelayEvent2FirstData(&pReport->tEvent);

	// 5�������ȡ���ֱ����������־
    DB_ClearEventFlag(pAppInfo,APP_EVENT_RACT);
									 
	return DB_ERR_OK;
}

/************************************************************
	���ܣ����豸����д�뱣����������
	������ *pReport 	--  ��д��ı���������Ϣ                      
			pTime 		--  ��������������ʱ�䣬�������ڲ�������ң�ű仯�¼��á���Ϊ��ʱ��ʹ��ϵͳʱ�䡣
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_WriteRelayActionReport(TRelayActionEv *pReport, TAbsTime *pTime)
{
    TRelayActionWrap 	*pWrap;
    TAppInfo 			*pAppInfo;
    WORD 				i, wWritePtr;
                     
	// 1���������ָ��ĺϷ���
	if( pReport == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// ���㱨��У�飬Modify by lz. 2013.10.08
	DAE_MakeCheckSum(pReport->aEvent,MAX_RACT_LENGTH-2);

	// 3��������
	wWritePtr = pWrap->nTailIndex;
	
	OS_Protect(0);
	
    pWrap->pEvent[wWritePtr] = *pReport;
    pWrap->nTailIndex = (wWritePtr+1)%MAX_RACT_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RACT_NUM;

	OS_UnProtect(0);
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_RACT))
		{
			pAppInfo->nRACTRead = pWrap->nTailIndex;
			continue;
		}
		
	    if( pAppInfo->nRACTRead == pWrap->nTailIndex )
	         pAppInfo->nRACTRead = (pWrap->nTailIndex+1)%MAX_RACT_NUM;

//		if(!DB_GetEventFlag(&pAppInfo[i], APP_EVENT_RACT))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_RACT);
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_RELAYEVENT,1,wWritePtr,0,0);
		}
	}

	DB_RegisterFileChange(DB_FILE_RELAYEV);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����������Ŀ���δ��ȡ���ĵ���Ŀ
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayActionReportNum(WORD *retval)
{
	TRelayActionWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3����ȡ��ָ��    
    rc = DB_GetRelayActionReportRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_RACT_NUM-rp)%MAX_RACT_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ���ȡ�����������Ŀ�Ķ�ָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayActionReportRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // ������������ָ��
    *retval = pAppInfo->nRACTRead%MAX_RACT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ����������Ϣ��������ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayActionReportHead(WORD *retval)
{
	TRelayActionWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RACT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ����������Ϣ��������βָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetRelayActionReportTail(WORD *retval)
{
	TRelayActionWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RACT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���������������Ϣ��������ͷָ��
	��������
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_IncRelayActionReportRead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nRACTRead = (pAppInfo->nRACTRead+1)%MAX_RACT_NUM;

	// 3���洢ָ��
	//DB_RegisterFileChange(DB_FILE_RELAYEV);
		
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���ȡ��ѹ�������
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetSWNum()
{
	return G_pCfgTable->DBaseConfig.nSW;
}
		
/************************************************************
	���ܣ������ݿ���ȡһ����ѹ��״̬
	������no,    		--  ��ѹ�����
		  Status	    --  ��ѹ��״̬	
						Ͷ��SW_STATUS_SET���У�SW_STATUS_CUT
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetSWStatus(WORD no, BYTE &Status)
{
	DWORD 			*pSW;
    WORD 			num;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
               
	pSW =  G_pCfgTable->DBaseConfig.pSWData;
	num  = G_pCfgTable->DBaseConfig.nSW;
	
	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_GetSWStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSW == NULL )
    {
        LogError("DB_GetSWStatus",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	if(pSW[DwordNo] & BitSign)
		Status = SW_STATUS_SET;
	else
		Status = SW_STATUS_CUT;
		
	return DB_ERR_OK;
}
		
/************************************************************
	���ܣ������ݿ���д��һ����ѹ��״̬
	������no,    		--  ��ѹ�����
		  Status	    --  ��ѹ��״̬	
						Ͷ��CFG_STATUS_SET���У�CFG_STATUS_CLR
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetSWStatus( WORD no, BYTE Status )
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
               
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;
	
	// 1��������
	if(no >= num)
	{
		LogError("DB_SetSWStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSWOnline == NULL )
    {
        LogError("DB_SetSWStatus",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 2��д�������޸���
	for(int i=0;i<2*((num+31)/32);i++)
		pSWOnline[i] = G_pCfgTable->DBaseConfig.pSWData[i];
	
	if(Status == SW_STATUS_SET)
	{
	 	pSWOnline[DwordNo] |= BitSign;
	}
	else
	{
	 	pSWOnline[DwordNo] &= ~BitSign;
	}
	
	// 3��ע���޸���ѹ�����
	pSWOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSWOpt->dwOption1 	= no;
	pSWOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSWOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д��һ����ѹ��״̬ȷ��
	������no,    		--  ��ѹ�����
			Attr		--  ��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetSWStatusConfirm( WORD no ,WORD Attr)
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSW;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
    STATUS			rc;
	
	pSW 		= G_pCfgTable->DBaseConfig.pSWData;
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;

	// 1���������Ϸ���
	if(pSWOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSWOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pSWOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;
	
	// 2��������
	if(no >= num)
	{
		LogError("DB_SetSWStatusConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pSW == NULL)||(pSWOnline == NULL))
    {
        LogError("DB_SetSWStatusConfirm",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

	if(Attr == DB_CANCEL)
	{
		pSWOpt->wTimeCnt	= 0;
		pSWOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	
    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 3�����������޸����Ľ��д����ѹ������
	if(pSWOnline[DwordNo] & BitSign)
	{
	 	pSW[DwordNo] |= BitSign;
	 	pSW[DwordNo+(num+31)/32] &= ~BitSign;
	}
	else
	{
	 	pSW[DwordNo] &= ~BitSign;
	 	pSW[DwordNo+(num+31)/32] |= BitSign;
	}

	// 4�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_SW,no);
	DAE_ClearRunningFlags(SS_EEPROM_SW_ERR); 	
	
	// 5��ע���޸���ѹ�����
	pSWOpt->wTimeCnt 	= 0;
	pSWOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, no, 0, 0);

#if(MODUEL_TYPE == MT_KF6571)
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL), SM_RELAYDATA, MISC_CLASS_SW, no, 0, 0);
#endif
	
	if(pSWOnline[DwordNo] & BitSign)
		DB_Register_SW(no,SW_STATUS_SET);
	else
		DB_Register_SW(no,SW_STATUS_CUT);
		
	return rc;
}

/************************************************************
	���ܣ������ݿ���д��ȫ����ѹ��״̬
	������no,    		--  ��ѹ�����
		  Status	    --  ��ѹ��״̬��	
						Ͷ��CFG_STATUS_SET���У�CFG_STATUS_CLR
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetAllSWStatus(PBYTE Status)
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
               
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;
	
    if( pSWOnline == NULL )
    {
        LogError("DB_SetSWStatus",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

	// ���״̬д��
	for(int i=0;i<num;i++)
	{
		DwordNo 	= i/32;
		BitSign 	= (0x01<<(i%32));
		
		if(Status[i/8]&(0x01<<(i%8)))
		{
			pSWOnline[DwordNo] |= BitSign;
		}
		else
		{
			pSWOnline[DwordNo] &= ~BitSign;
		}
	}
	
	// 3��ע���޸���ѹ�����
	pSWOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSWOpt->dwOption1 	= SET_VALUE_ALL;
	pSWOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSWOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д��ȫ����ѹ��״̬ȷ��
	������Attr		--  ��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetAllSWStatusConfirm(WORD Attr)
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSW;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
    STATUS			rc;
	
	pSW 		= G_pCfgTable->DBaseConfig.pSWData;
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;

	// 1���������Ϸ���
	if(pSWOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSWOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pSWOpt->dwOption1 != SET_VALUE_ALL)
		return DB_ERR_OPT_PARA;
	
	// 2��������
    if((pSW == NULL)||(pSWOnline == NULL))
    {
        LogError("DB_SetSWStatusConfirm",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

	if(Attr == DB_CANCEL)
	{
		pSWOpt->wTimeCnt	= 0;
		pSWOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// ����������
	memcpy(pSW,pSWOnline,sizeof(DWORD)*((num+31)/32));
	for(int i=0;i<((num+31)/32);i++)
		pSW[i+((num+31)/32)] = ~pSW[i];
		
	// 4�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_SW,SET_VALUE_ALL);
	DAE_ClearRunningFlags(SS_EEPROM_SW_ERR); 	
	
	// 5��ע���޸���ѹ�����
	pSWOpt->wTimeCnt 	= 0;
	pSWOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, SET_VALUE_ALL, 0, 0);
	
	DB_Register_SW(SET_VALUE_ALL,0);
		
	return rc;
}

/************************************************************
	���ܣ������ݿ���ȡ�����ָ�����
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetCFGNum()
{
	return G_pCfgTable->DBaseConfig.nCFG;
}

/************************************************************
	���ܣ������ݿ���ȡ��������Ϣ
	������AreaNo,    	--  ����������
		  dwCFGWord	    --  �������б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetCFGWord(WORD AreaNo,DWORD *dwCFGWord)
{
    WORD 			num,i;
	STATUS			rc;
	
	num  = G_pCfgTable->DBaseConfig.nCFG;

	for(i=0;i<(num+31)/32;i++)	
	{
		rc = DB_GetSETFloatValue(AreaNo,i,(float *)&dwCFGWord[i]);
		if(rc != DB_ERR_OK)
		{
			LogError("DB_GetCFGStatus",FILE_LINE,"Read CFG Word Error!");
			return rc;
		}
	}
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д��������
	������AreaNo,    	--  ����������
		  dwCFGWord	    --  �������б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCFGWord(WORD AreaNo, DWORD *dwCFGWord)
{
	TRunTimeOPT				*pSETOpt;
	TDBaseConfigTable		*pDbase;
	DWORD 					*pSET;
	DWORD 					*pSETMap;
	WORD					num;
	int 					i;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	num  	= (pDbase->nCFG+31)/32;
	pSET 	= (DWORD *)pDbase->tModData.pSETData;
	pSETOpt	= pDbase->pSETOpt;
	
	// 1���������ָ��
	ASSERT(dwCFGWord != NULL);
	if(dwCFGWord == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pSET != NULL);
	ASSERT(pSETOpt != NULL);
	if((pSET == NULL)||(pSETOpt == NULL))
		return DB_ERR_BASENULL;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 2����鶨ֵ����
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetCFGWord",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 3��д�������޸���	
	if(pDbase->tMapData.pSETData == NULL)
		return DB_ERR_BASENULL;
	
	pSETMap	= (DWORD *)&pDbase->tMapData.pSETData[AreaNo*(num+1)];

	// 3��1 �ȸ��������޸�����
	for(i=0;i<num;i++)
	 	pSET[i] = ltobl(pSETMap[i]);

	// 3��2 ��д��Ҫ�޸ĵ�����
	for(i=0;i<num;i++)
		pSET[i] = dwCFGWord[i];

	// 4��ע���޸������ֲ���
	pSETOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSETOpt->dwOption1 	= AreaNo;
	pSETOpt->dwOption2 	= SET_VALUE_ALL;
	pSETOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSETOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д��һ��������������ȷ��
	������AreaNo,    	--  ����������
			Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCFGWordConfirm(WORD AreaNo,WORD Attr )
{
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	TRunTimeOPT				*pSETOpt;
	DWORD 					*pSET;
    WORD 					CFGNum;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= (DWORD *)pDbase->tModData.pSETData;
	pSETOpt	= pDbase->pSETOpt;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1���������Ϸ���
	if(pSETOpt->dwAppID != RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETOpt->wStatus != OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if((pSETOpt->dwOption1 != AreaNo)||(pSETOpt->dwOption2 != SET_VALUE_ALL))
		return DB_ERR_OPT_PARA;

	// 2���������ָ��
	ASSERT(pSET != NULL);
	if(pSET == NULL)
		return DB_ERR_BASENULL;

	// 3����鶨ֵ����
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetCFGWordConfirm",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETOpt->wTimeCnt	= 0;
		pSETOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 5������ǵ�ǰ��ֵ������д��RAM
	CFGNum = (pDbase->nCFG+31)/32;
	
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		for(int i=0;i<CFGNum;i++)
		{
			pDbase->pSETDataFloat[i] = *(float*)(pSET+i);
			pDbase->pSETDataFix[i] = pSET[i];
		}
	}

	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFloat, sizeof(float)*pDbase->nSET);
	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFix, sizeof(long)*pDbase->nSET);

	// 4�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_SET,SET_VALUE_ALL);

	// 5��ע���޸���ѹ�����
	pSETOpt->wTimeCnt 	= 0;
	pSETOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(SET_VALUE_ALL,AreaNo), 0, 0);

	DB_Register_SET(AreaNo,SET_VALUE_ALL);
	
	return rc;
}

/************************************************************
	���ܣ������ݿ���ȡһ��������λ״̬
	������no,    		--  ������λ���
		  Status	    --  ������λ״̬	
							Ͷ��CFG_STATUS_SET���У�CFG_STATUS_CLR
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetCFGStatus(WORD AreaNo,BYTE no, BYTE &Status)
{
	DWORD 			dwCFGWord;
	DWORD 			BitSign; 	
	WORD 			DwordNo; 	
    WORD 			num;
	STATUS			rc;
	
	num  = G_pCfgTable->DBaseConfig.nCFG;

	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_GetCFGStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	rc = DB_GetSETFloatValue(AreaNo,DwordNo,(float *)&dwCFGWord);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_GetCFGStatus",FILE_LINE,"Read CFG Word Error!",no);
		return rc;
	}
	
	if(dwCFGWord & BitSign)
		Status = CFG_STATUS_SET;
	else
		Status = CFG_STATUS_CLR;
		
	return DB_ERR_OK;
}
/************************************************************
	���ܣ������ݿ���д��һ��������λ״̬
	������no,    		--  ������λ���
		  Status	    --  ������λ״̬	
							Ͷ��CFG_STATUS_SET���У�CFG_STATUS_CLR
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCFGStatus(WORD AreaNo, WORD no, BYTE Status)
{
	TRunTimeOPT		*pCFGOpt;		
	DWORD 			dwCFGWord;
	DWORD 			BitSign; 	
	WORD 			DwordNo; 	
    WORD 			num;
    STATUS			rc;

	pCFGOpt		= G_pCfgTable->DBaseConfig.pCFGOpt;
	num			= G_pCfgTable->DBaseConfig.nCFG;
	
	// 1��������
	if(no >= num)
	{
		LogError("DB_SetCFGStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	rc = DB_GetSETFloatValue(AreaNo,DwordNo,(float *)&dwCFGWord);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_SetCFGStatus",FILE_LINE,"Read CFG Word Error!",no);
		return rc;
	}
	
	if(Status == CFG_STATUS_SET)
	 	dwCFGWord |= BitSign;
	else
	 	dwCFGWord &= ~BitSign;

	// 3��ע���޸������ֲ���
	pCFGOpt->dwAppID 	= RtosGetCurrentTaskId();
	pCFGOpt->dwOption1 	= no;
	pCFGOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pCFGOpt->wStatus 	= OPT_STATUS_SET;

	return DB_SetSETValue(AreaNo, DwordNo, (float *)&dwCFGWord);

/*	
	TRunTimeOPT		*pCFGOpt;
	DWORD 			*pCFGOnline;
	
	pCFGOnline 	= G_pCfgTable->DBaseConfig.tModData.pCFGData;
	pCFGOpt		= G_pCfgTable->DBaseConfig.pSWOpt;
	
    if( pCFGOnline == NULL )
    {
        LogError("DB_SetCFGStatus",FILE_LINE,"The CFG base is null!");
        return DB_ERR_BASENULL;
    }
    
	// 2��д�������޸���
	for(int i=0;i<(num+31)/32;i++)
		pCFGOnline[i] = G_pCfgTable->DBaseConfig.pCFGData;
	
	if(Status == CFG_STATUS_SET)
	 	pCFGOnline[DwordNo] |= BitSign;
	else
	 	pCFGOnline[DwordNo] &= ~BitSign;

	// 3��ע���޸������ֲ���
	pCFGOpt->dwAppID 	= RtosGetCurrentTaskId();
	pCFGOpt->dwOption1 	= no;
	pCFGOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pCFGOpt->wStatus 	= OPT_STATUS_SET;
*/
}

/************************************************************
	���ܣ������ݿ���д��һ��������λ״̬ȷ��
	������no,    		--  ������λ���
			Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCFGStatusConfirm(WORD AreaNo,WORD no,WORD Attr)
{
	TRunTimeOPT		*pCFGOpt;
	DWORD 			dwCFGWord;
	DWORD 			BitSign; 	
	WORD 			DwordNo; 	
    WORD 			num;
    STATUS			rc;
	
	pCFGOpt	 	= G_pCfgTable->DBaseConfig.pCFGOpt;
	num  		= G_pCfgTable->DBaseConfig.nCFG;
	// 1���������Ϸ���
	if(pCFGOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pCFGOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pCFGOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;

	// 2��������
	if(no >= num)
	{
		LogError("DB_SetCFGStatusConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pCFGOpt->wTimeCnt	= 0;
		pCFGOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 3��ע���޸������ֲ���
	pCFGOpt->wTimeCnt 	= 0;
	pCFGOpt->wStatus 	= OPT_STATUS_READY;

	rc = DB_SetSETValueConfirm(AreaNo, DwordNo,Attr);
	if(rc != DB_ERR_OK)
		return rc;
	
	dwCFGWord = *(DWORD *)&G_pCfgTable->DBaseConfig.tModData.pSETData[DwordNo];

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_CFG, no, 0, 0);

	if(dwCFGWord & BitSign)
		DB_Register_CFG(no,CFG_STATUS_SET);
	else
		DB_Register_CFG(no,CFG_STATUS_CLR);

	return rc;
/*	
	TRunTimeOPT		*pCFGOpt;
	DWORD 			*pCFG;
	DWORD 			*pCFGOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
               
	pCFG		= G_pCfgTable->DBaseConfig.pCFGData;
	pCFGOnline 	= G_pCfgTable->DBaseConfig.tModData.pCFGData;
	pCFGOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nCFG;
	
	// 1���������Ϸ���
	if(pCFGOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pCFGOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pCFGOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;

	// 2��������
	if(no >= num)
	{
		LogError("DB_SetCFGStatusConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pCFG == NULL)||(pCFGOnline == NULL))
    {
        LogError("DB_SetCFGStatusConfirm",FILE_LINE,"The CFG base is null!");
        return DB_ERR_BASENULL;
    }

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 3�����������޸����Ľ��д����ѹ������
	if(pCFGOnline[DwordNo] & BitSign)
	{
	 	pCFG[DwordNo] |= BitSign;
	}
	else
	{
	 	pCFG[DwordNo] &= ~BitSign;
	}

	// 4�����汣�������ļ�
	DB_ModifyDataFile(DB_DATA_CFG,no);

	// 5��ע���޸���ѹ�����
	pCFGOpt->wTimeCnt 	= 0;
	pCFGOpt->wStatus 	= OPT_STATUS_READY;

	if(pCFGOnline[DwordNo] & BitSign)
		DB_Register_CFG(no,CFG_STATUS_SET);
	else
		DB_Register_CFG(no,CFG_STATUS_CLR);
	
	return DB_ERR_OK;
*/	
}

/************************************************************
	���ܣ������ݿ���ȡ��ֵ������
	��������
	���أ���ȷ���ظ��������󷵻�0xFFFF��
************************************************************/
WORD	DB_GetSETNum()
{
	return G_pCfgTable->DBaseConfig.nSET;
}

/************************************************************
	���ܣ������ݿ��ж�ȡ��ֵ�ĸ�������
	������	AreaNo		--  ��ֵ���š�	SET_AREA_CURRENT����ǰ��ֵ����
			no,   		--  ��ֵ��š�	SET_VALUE_ALL����ȫ����ֵ

			fValue	    --  ���㶨ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetSETFloatValue( WORD AreaNo,WORD no, float *fValueList)
{
	float 			*pSET;
    WORD 			num;
               
	pSET = G_pCfgTable->DBaseConfig.pSETDataFloat;
	num  = G_pCfgTable->DBaseConfig.nSET;

	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1���������ָ��
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2����鶨ֵ����
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_GetSETFloatValue",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 3����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSETFloatValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4������ǵ�ǰ��ֵ����ֱ�Ӵ�RAM��ȡ,������ļ��ж�ȡ
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		if(no == SET_VALUE_ALL)
			memcpy(fValueList,pSET,sizeof(float)*num);
		else
			*fValueList = pSET[no];		
	}
	else
	{
		// ��ǰ��ֵ�����쳣
		if(G_pCfgTable->DBaseConfig.tMapData.pSETData != NULL)
		{
			pSET = &G_pCfgTable->DBaseConfig.tMapData.pSETData[AreaNo*(num+1)];
		}

		if(no == SET_VALUE_ALL)
		{
			for(int i=0;i<num;i++)
				fValueList[i] = CONVERTFLOAT(pSET[i]);
		}
		else
			*fValueList = CONVERTFLOAT(pSET[no]);		
	}
	
	return DB_ERR_OK;
}
	
/************************************************************
	���ܣ������ݿ��ж�ȡ��ֵ����������
	������	AreaNo		--  ��ֵ���š�ͬ�ϡ�
			no,   		--  ��ֵ��š�ͬ�ϡ�
		  fValue	    --  ���㶨ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetSETFixValue( WORD AreaNo,WORD no, long *lValueList)
{
	long 			*pSETFix;
	float 			*pSETFloat;
    WORD 			num;
               
	pSETFix = G_pCfgTable->DBaseConfig.pSETDataFix;
	num  = G_pCfgTable->DBaseConfig.nSET;

	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1���������ָ��
	ASSERT(lValueList != NULL);
	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2����鶨ֵ����
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_GetSETFixValue",FILE_LINE,"The Area No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	// 3����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSETFixValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4������ǵ�ǰ��ֵ����ֱ�Ӵ�RAM��ȡ,������ļ��ж�ȡ
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		if(no == SET_VALUE_ALL)
			memcpy(lValueList,pSETFix,sizeof(float)*num);
		else
			*lValueList = pSETFix[no];		
	}
	else
	{
		if(G_pCfgTable->DBaseConfig.tMapData.pSETData != NULL)
			pSETFloat = &G_pCfgTable->DBaseConfig.tMapData.pSETData[AreaNo*(num+1)];

		if(no == SET_VALUE_ALL)
		{
			for(int i=0;i<num;i++)
			{
				ConvertSETFloat2Fix(i, CONVERTFLOAT(pSETFloat[i]), &lValueList[i]);
			}
		}
		else
		{
			ConvertSETFloat2Fix(no, CONVERTFLOAT(pSETFloat[no]), lValueList);
		}
	}
	
	return DB_ERR_OK;
}
	
/************************************************************
	���ܣ������ݿ���д�붨ֵ
	������	AreaNo		--	��ֵ���š�ͬ�ϡ�
			no, 		--	��ֵ��š�ͬ�ϡ�
			fValue		--	���㶨ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetSETValue(WORD AreaNo,WORD no, float *fValueList )
{
	TRunTimeOPT 			*pSETOpt;
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	float					*pSET;
	float					*pSETMap;
	TSET_Disp				tSetDisp;
	WORD					num,CFGNum;
	
	pDbase	= &G_pCfgTable->DBaseConfig;	 
	num 	= pDbase->nSET;
	CFGNum	= (pDbase->nCFG+31)/32;
	pSET	= pDbase->tModData.pSETData;
	pSETOpt = pDbase->pSETOpt;
	
	// 1���������ָ��
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pSET != NULL);
	ASSERT(pSETOpt != NULL);
	if((pSET == NULL)||(pSETOpt == NULL))
		return DB_ERR_BASENULL;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 2����鶨ֵ����
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetSETValue",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 3����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSETValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4��д�������޸���	
	if(pDbase->tMapData.pSETData == NULL)
		return DB_ERR_BASENULL;
	
	pSETMap = &pDbase->tMapData.pSETData[AreaNo*(num+1)];

	// 4��1 �ȸ��������޸�����
	for(int i=0;i<num;i++)
		pSET[i] = CONVERTFLOAT(pSETMap[i]);

	// 4��2 ��д��Ҫ�޸ĵ�����
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
		{
			if(i >= CFGNum)
			if(DB_GetSETDisp(i, &tSetDisp) == DB_ERR_OK)
			{
				if((fValueList[i] - tSetDisp.fMaxValue) > 0.000001)
				{
					LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
						i,fValueList[i],tSetDisp.fMaxValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
				
				if((fValueList[i]-tSetDisp.fMinValue) < -0.000001 )
				{
					LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
						i,fValueList[i],tSetDisp.fMinValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
			}
			pSET[i] = fValueList[i];
		}
	}
	else
	{
		if(no >= CFGNum)
		if(DB_GetSETDisp(no, &tSetDisp) == DB_ERR_OK)
		{
			if((*fValueList - tSetDisp.fMaxValue) > 0.000001)
			{
				LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
					no,*fValueList,tSetDisp.fMaxValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
			
			if((*fValueList - tSetDisp.fMinValue) < -0.000001 )
			{
				LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
					no,*fValueList,tSetDisp.fMinValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
		}
		pSET[no]= *fValueList;		
	}
	
	// 5��ע���޸������ֲ���
	pSETOpt->dwAppID	= RtosGetCurrentTaskId();
	pSETOpt->dwOption1	= AreaNo;
	pSETOpt->dwOption2	= no;
	pSETOpt->wTimeCnt	= OPT_TIMEOUT_NUM;
	pSETOpt->wStatus	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д�붨ֵȷ��
	������	AreaNo   --  ��ֵ���š�ͬ�ϡ�
			no,   	 --  ��ֵ��š�ͬ�ϡ�
		Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetSETValueConfirm(WORD AreaNo,WORD no,WORD Attr)
{
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	TRunTimeOPT				*pSETOpt;
	float 					*pSET;
    WORD 					num;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= pDbase->tModData.pSETData;
	pSETOpt	= pDbase->pSETOpt;
	num  	= pDbase->nSET;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1���������Ϸ���
	if(pSETOpt->dwAppID != RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETOpt->wStatus != OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if((pSETOpt->dwOption1 != AreaNo)||(pSETOpt->dwOption2 != no))
		return DB_ERR_OPT_PARA;

	// 2���������ָ��
	ASSERT(pSET != NULL);
	if(pSET == NULL)
		return DB_ERR_BASENULL;

	// 3����鶨ֵ����
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetSETValueConfirm",FILE_LINE,"The Area No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	// 4����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSETValueConfirm",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETOpt->wTimeCnt	= 0;
		pSETOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 5������ǵ�ǰ��ֵ������д��RAM
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		if(no == SET_VALUE_ALL)
		{
			for(int i=0;i<num;i++)
			{
				pDbase->pSETDataFloat[i] = pSET[i];
				//����������
				ConvertSETFloat2Fix(i,pSET[i],pDbase->pSETDataFix+i);
			}
		}
		else
		{
			pDbase->pSETDataFloat[no] = pSET[no];
			//����������
			ConvertSETFloat2Fix(no,pSET[no],pDbase->pSETDataFix+no);
		}
	}

	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFloat, sizeof(float)*num);
	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFix, sizeof(long)*num);

	// 4�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_SET,no);
	DAE_ClearRunningFlags(SS_EEPROM_SET_ERR); 	

	// 5��ע���޸���ѹ�����
	pSETOpt->wTimeCnt 	= 0;
	pSETOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(no,AreaNo), 0, 0);

	// �޸Ķ�ֵʱ֪ͨ���ͨ������
	
#if(MODUEL_TYPE == MT_KF6571)
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL), SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(no,AreaNo), 0, 0);
#endif

	DB_Register_SET(AreaNo,no);
	
	return rc;
}

/************************************************************
	���ܣ���ȡ��ǰ��ֵ���š�
	������	��	
	���أ���ȷ���ض�ֵ���ţ����󷵻�0xFF��
************************************************************/
WORD 	DB_GetSETAreaNum()
{
	return G_pCfgTable->DBaseConfig.nSETArea;
}

/************************************************************
	���ܣ���ȡ��ǰ��ֵ���š�
	������	��	
	���أ���ȷ���ض�ֵ���ţ����󷵻�0xFF��
************************************************************/
WORD 	DB_GetCurrentSETAreaNo()
{
	return G_pCfgTable->DBaseConfig.nSETAreaCur;
}

/************************************************************
	���ܣ����õ�ǰ��ֵ���š�
	������	AreaNo   --  ��ֵ���š�
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCurrentSETAreaNo ( WORD AreaNo)
{
	TRunTimeOPT				*pSETAreaOpt;
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�

	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSETAreaOpt	= pDbase->pSETAreaOpt;
	
	// 1����鶨ֵ���ŵĺϷ���
	if( AreaNo >= pDbase->nSETArea)
	{
		LogError("DB_SetCurrentSETAreaNo",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 2���޸Ķ�ֵ����
	pDbase->tModData.pRelayCfg->nSETAreaCur = AreaNo;
	pDbase->tModData.pRelayCfg->nSETAreaCurB = ~AreaNo;
	
	// 3��ע�������޸Ĺ���
	pSETAreaOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSETAreaOpt->dwOption1 	= AreaNo;
	pSETAreaOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSETAreaOpt->wStatus 	= OPT_STATUS_SET;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ����õ�ǰ��ֵ����ȷ��
	������	AreaNo   --  ��ֵ���š�
			Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCurrentSETAreaNoConfirm ( WORD AreaNo,WORD Attr)
{
	TRunTimeOPT				*pSETAreaOpt;
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	TRelayDataTable			*pRelayCfg;				//�������ñ�
	TMapDataList			*pMap;
	float					*pSETfloat;
	long					*pSETFix;
	WORD					Num;
	STATUS					rc;
	
	pDbase 		= &G_pCfgTable->DBaseConfig;
	pMap 		= &pDbase->tMapData;
	pRelayCfg	= pDbase->tModData.pRelayCfg;
	pSETAreaOpt	= pDbase->pSETAreaOpt;
	pSETfloat	= pDbase->pSETDataFloat;
	pSETFix		= pDbase->pSETDataFix;

	ASSERT(pRelayCfg);
	ASSERT(pSETAreaOpt);
	ASSERT(pSETfloat);
	ASSERT(pSETFix);
	
	// 1���������Ϸ���
	if(pSETAreaOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETAreaOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pSETAreaOpt->dwOption1 != AreaNo)
		return DB_ERR_OPT_PARA;

	// 2��������źϷ���
	if( AreaNo >= pDbase->nSETArea)
	{
		LogError("DB_SetCurrentSETAreaNoConfirm",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETAreaOpt->wTimeCnt	= 0;
		pSETAreaOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	

	// 3����д��RAM
	pDbase->nSETAreaCur		= pRelayCfg->nSETAreaCur;		
	pDbase->nSETAreaCurB 	= pRelayCfg->nSETAreaCurB;		

	// 4�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_SETAREACUR,0);
	DAE_ClearRunningFlags(SS_EEPROM_SETAREA_ERR); 	

	// 5���ٸ��µ�ǰ��ֵ
	Num = DB_GetSETNum();
	for(int i=0;i<Num;i++)
	{
		pSETfloat[i] = CONVERTFLOAT(pMap->pSETData[pDbase->nSETAreaCur*(Num+1)+i]);
		ConvertSETFloat2Fix(i,pSETfloat[i],&pSETFix[i]);
	}

	// ���¼���У��
	DAE_MakeCheckSum((BYTE *)pSETfloat,sizeof(float)*Num);
	DAE_MakeCheckSum((BYTE *)pSETFix,sizeof(long)*Num);
	
	// 6��ע���޸���ѹ�����
	pSETAreaOpt->wTimeCnt 	= 0;
	pSETAreaOpt->wStatus 	= OPT_STATUS_READY;
	
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SETAREA, AreaNo, 0, 0);

	// �ǲ��ͨ�������л���ֵ��ʱ֪ͨ���ͨ������
	if(MODUEL_TYPE == MT_KF6571)
	{
		if(DAE_GetCurrentTaskPortId() != SYS_PORTID_FL)
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL), SM_RELAYDATA, MISC_CLASS_SETAREA, AreaNo, 0, 0);
	}

	DB_Register_SETArea(AreaNo);

	return rc;	
}

/************************************************************
	���ܣ������ݿ���ȡ������ֵ������
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetSystemSETNum()
{
	return G_pCfgTable->DBaseConfig.nSystemSET;
}

/************************************************************
	���ܣ������ݿ��ж�ȡ������ֵ������������
	������	no,   		--  ��ֵ���	SET_VALUE_ALL����ȫ����ֵ
		    fValue		--  ���㶨ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetSystemSETFloatValue(WORD no, float *fValueList)
{
	float 			*pSET;
    WORD 			num;
               
	pSET =  G_pCfgTable->DBaseConfig.pSystemSETData;
	num  = G_pCfgTable->DBaseConfig.nSystemSET;
	
	// 1���������ָ��
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSystemSETFloatValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 3��ֱ�Ӵ�RAM��ȡ
	if(no == SET_VALUE_ALL)
		memcpy(fValueList,pSET,sizeof(float)*num);
	else
		*fValueList = pSET[no];		
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ��ж�ȡ������ֵ������������
	������	no,   		--  ��ֵ���	SET_VALUE_ALL����ȫ����ֵ
		    lValue		--  ���㶨ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetSystemSETFixValue(WORD no, long *lValueList)
{
	float 			*pSETFloat;
    WORD 			num;
               
	pSETFloat =  G_pCfgTable->DBaseConfig.pSystemSETData;
	num  = G_pCfgTable->DBaseConfig.nSystemSET;
	
	// 1���������ָ��
	ASSERT(lValueList != NULL);
	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSystemSETFixValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 3��ֱ�Ӵ�RAM��ȡ
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
		{
			ConvertSystemSETFloat2Fix(i, pSETFloat[i], &lValueList[i]);
		}
	}
	else
	{
		ConvertSystemSETFloat2Fix(no, pSETFloat[no], lValueList);
	}
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д�븨����ֵ��
	������no,   	 	--  ��ֵ��š�ͬ�ϡ�
		  fValue	   	--  ���㶨ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetSystemSETValue (WORD no, float *fValueList )
{
	TRunTimeOPT				*pSETOpt;
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	float 					*pSET;
	float 					*pSETMap;
	TSysSET_Disp			tSetDisp;
    WORD 					num;

	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= pDbase->tModData.pSystemSETData;
	pSETMap	= pDbase->tMapData.pSystemSETData;
	pSETOpt	= pDbase->pCFOpt;
	num  	= pDbase->nSystemSET;
	
	// 1���������ָ��
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pSET != NULL);
	ASSERT(pSETMap != NULL);
	ASSERT(pSETOpt != NULL);
	if((pSET == NULL)||(pSETMap == NULL)||(pSETOpt == NULL))
		return DB_ERR_BASENULL;

	// 2����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSystemSETValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4��д�������޸���

	// 4��1 �ȸ��������޸�����
	for(int i=0;i<num;i++)
	{
	 	pSET[i] = CONVERTFLOAT(pSETMap[i]);
	}
	
	// 4��2 ��д��Ҫ�޸ĵ�����
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
		{
			// Խ�޼��
			if(DB_GetSysSETDisp(i, &tSetDisp) == DB_ERR_OK)
			{
				if((fValueList[i] - tSetDisp.fMaxValue) > 0.000001)
				{
					LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
						i,fValueList[i],tSetDisp.fMaxValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
				
				if((fValueList[i]-tSetDisp.fMinValue) < -0.000001 )
				{
					LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
						i,fValueList[i],tSetDisp.fMinValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
			}
			pSET[i] = fValueList[i];
		}
	}
	else
	{
		// Խ�޼��
		if(DB_GetSysSETDisp(no, &tSetDisp) == DB_ERR_OK)
		{
			if((*fValueList - tSetDisp.fMaxValue) > 0.000001)
			{
				LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
					no,*fValueList,tSetDisp.fMaxValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
			
			if((*fValueList - tSetDisp.fMinValue) < -0.000001 )
			{
				LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
					no,*fValueList,tSetDisp.fMinValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
		}
		pSET[no]= *fValueList;		
	}
	
	// 6��ע���޸������ֲ���
	pSETOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSETOpt->dwOption1 	= SET_AREANO_INVALID;
	pSETOpt->dwOption2 	= no;
	pSETOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSETOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д�븨����ֵȷ�ϡ�
	������	AreaNo   --  ��ֵ���š�ͬ�ϡ�
			no,   	 --  ��ֵ��š�ͬ�ϡ�
			Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetSystemSETValueConfirm(WORD no,WORD Attr)
{
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	TRunTimeOPT				*pSETOpt;
	float 					*pSET;
    WORD 					num;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= pDbase->tModData.pSystemSETData;
	pSETOpt	= pDbase->pCFOpt;
	num  	= pDbase->nSystemSET;
	
	// 1���������Ϸ���
	if(pSETOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if((pSETOpt->dwOption1 != SET_AREANO_INVALID)||(pSETOpt->dwOption2 != no))
		return DB_ERR_OPT_PARA;

	// 2���������ָ��
	ASSERT(pSET != NULL);
	if(pSET == NULL)
		return DB_ERR_BASENULL;
	
	// 3����鶨ֵ���
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSystemSETValueConfirm",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETOpt->wTimeCnt	= 0;
		pSETOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	

	// 5����д��RAM
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
			pDbase->pSystemSETData[i] = pSET[i];
	}
	else
		pDbase->pSystemSETData[no] = pSET[no];	
	
	DAE_MakeCheckSum((BYTE *)pDbase->pSystemSETData, sizeof(float)*num);

	// 4�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_SYSSET,no);
	DAE_ClearRunningFlags(SS_EEPROM_SYSSET_ERR); 	

	// 5��ע���޸���ѹ�����
	pSETOpt->wTimeCnt 	= 0;
	pSETOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, no, 0, 0);

	DB_Register_SET(SET_AREANO_INVALID,no);

	//�����0��������ֵ���������³�ʼ��ϵ����
	if((no == SET_VALUE_ALL)||(no == 0))
		DB_UpdateCFBase();
	
	return rc;
}

/************************************************************
���ܣ�ͨ����һ��������ֵ����������жϵ�ǰϵͳ�Ƿ�����1A�������
	  �ú������ڵ�ǰϵ����дʱ���ж�Ӧ�ö�д�Ǹ�����
������	��	
���أ�������ֵ1A����TRUE�����򷵻ش���		
************************************************************/
BOOL	DB_Is1ASystem()
{
/*	float 	fRating = 5.0;
	long	lRating;
	
	DB_GetSystemSETFloatValue(SYSTEM_SET_DEFINE1, &fRating);

	lRating = fRating;

	if(lRating == 1)
		return TRUE;
	else
		return FALSE;
*/
	
	long	lRating;
	float	fRating = 5.0;

	
	DB_GetSystemSETFloatValue(SYSTEM_SET_DEFINE1, &fRating);
	lRating = (long)fRating;

	if(lRating == RATE_1A)
		return TRUE;
	else
		return FALSE;
}

/************************************************************
���ܣ���ǰϵͳ���ϵ����Ƿ���ʾһ��ֵ 
��������	
���أ�TRUE/FALSE		
************************************************************/
BOOL	DB_IsFDShowFirst()
{
	return (G_pCfgTable->DBaseConfig.dwControl&DB_RELAY_FDSECOND)?FALSE:TRUE;
}

/************************************************************
���ܣ���ǰϵͳ�Ƿ�Ҫ������ֵ��
��������	
���أ�TRUE/FALSE		
************************************************************/
BOOL	DB_IsCaculateIntegral()
{
	return (G_pCfgTable->DBaseConfig.dwControl&DB_RELAY_CACLINT)?TRUE:FALSE;
}
/************************************************************
���ܣ���ȡ��ǰ��ʱ��Դ����
��������	
���أ�TRUE��DK3511/FALSE,KF6511		
************************************************************/
BOOL	DB_WhichIRGB()
{
	return (G_pCfgTable->DBaseConfig.dwControl&DB_IRGB_DK3511)?TRUE:FALSE;
}

/************************************************************
	���ܣ������ݿ���ȡ��ȸ�����
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetRTNum()
{
	return G_pCfgTable->DBaseConfig.nRT;
}

/************************************************************
	���ܣ������ݿ��ж�ȡ��ȣ�����������
	������	no,   	 	--  ������
		    fValueList	--  �������б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetRTValue(WORD no, float *fValueList)
{
//	TSysSET_Disp		*pSETDisp;
//	TCF_Disp			*pCFDisp;
//	long				Ratio;
	int					SETNo;
	STATUS 				rc;

	// 1��ȡ���㶨ֵ
	SETNo = no+DB_GetSystemSETNum()-DB_GetRTNum();
	
	rc = DB_GetSystemSETFloatValue(SETNo, fValueList);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_GetRTValue",FILE_LINE,"Get RT Value Error!");
		return DB_ERR_RELAY_SET;
	}

/*
	// 2����������	
	pSETDisp = G_pCfgTable->DBaseConfig.pSystemSETDisp;
	Ratio	 = G_pCfgTable->DBaseConfig.pCFDisp[pSETDisp[SETNo].nCFIndex].wCof;
	if(Ratio == 0)
		Ratio = 1;

	*fValueList = (*fValueList)/Ratio;
*/
	
	return DB_ERR_OK;
}
		
/************************************************************
	���ܣ������ݿ���д���ȡ�
	������no,   	 	--  �����š�ͬ�ϡ�
		  fValue	   	--  �������б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetRTValue (WORD no, float *fValueList )
{
	if(no >= DB_GetRTNum())
	{
		LogError("DB_SetRTValue",FILE_LINE,"The RT No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	if(DB_GetSystemSETNum() < DB_GetRTNum())
	{
		LogError("DB_SetRTValue",FILE_LINE,"The RTNum=%d is overflow!",DB_GetRTNum());
		return DB_ERR_NO;
	}

	return DB_SetSystemSETValue(no+DB_GetSystemSETNum()-DB_GetRTNum(), fValueList);
}

/************************************************************
	���ܣ������ݿ���д����ȷ�ϡ�
	������	AreaNo   --  ������š�ͬ�ϡ�
			no,   	 --  �����š�ͬ�ϡ�
			Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetRTValueConfirm(WORD no,WORD Attr)
{
	if(no >= DB_GetRTNum())
	{
		LogError("DB_SetRTValueConfirm",FILE_LINE,"The RT No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	if(DB_GetSystemSETNum() < DB_GetRTNum())
	{
		LogError("DB_SetRTValueConfirm",FILE_LINE,"The RTNum=%d is overflow!",DB_GetRTNum());
		return DB_ERR_NO;
	}
		
	return DB_SetSystemSETValueConfirm(no+DB_GetSystemSETNum()-DB_GetRTNum(),Attr);
}

/************************************************************
	���ܣ������ݿ���ȡϵ��������
	��������
	���أ���ȷ���ظ��������󷵻�0xFF��
************************************************************/
WORD	DB_GetCFNum()
{
	return G_pCfgTable->DBaseConfig.nCF;
}

/************************************************************
	���ܣ������ݿ��ж�ȡϵ��ֵ��
	������	no,   		--  ϵ�����	CF_VALUE_ALL����ȫ��ϵ��ֵ
		    lValueList	--  ϵ��ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_GetCFValue(WORD no,long *lValueList)
{
	long 			*pCF;
    WORD 			num;
               
	pCF =  G_pCfgTable->DBaseConfig.pCFData;
	num  = G_pCfgTable->DBaseConfig.nCF;
	
	// 1���������ָ��
	ASSERT(pCF != NULL);
	ASSERT(lValueList != NULL);

	if(pCF == NULL)
		return DB_ERR_BASENULL;

	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2����鶨ֵ���
	if((no != CF_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetCFValue",FILE_LINE,"The CF No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 3��ֱ�Ӵ�RAM��ȡ
	if(no == CF_VALUE_ALL)
		memcpy(lValueList,pCF,sizeof(long)*num);
	else
		*lValueList = pCF[no];		
	
	return DB_ERR_OK;
}
		
/************************************************************
	���ܣ������ݿ���д��ϵ��ֵ��
	������	no,   		--  ϵ����š�ͬ�ϡ�
		    lValueList	--  ϵ��ֵ�б�	
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCFValue (WORD no, long *lValueList )
{
	TRunTimeOPT				*pCFOpt;
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	long 					*pCF;
	long 					*pCFMap;
    WORD 					num;

	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pCF 	= pDbase->tModData.pCFData;
	pCFMap	= pDbase->tMapData.pCFData;
	pCFOpt	= pDbase->pCFOpt;
	num  	= pDbase->nCF;
	
	// 1���������ָ��
	ASSERT(lValueList != NULL);
	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pCF != NULL);
	ASSERT(pCFMap != NULL);
	ASSERT(pCFOpt != NULL);
	if((pCF == NULL)||(pCFMap == NULL)||(pCFOpt == NULL))
		return DB_ERR_BASENULL;

	// 2����鶨ֵ���
	if((no != CF_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetCFValue",FILE_LINE,"The CF No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4��д�������޸���

	// 4��1 �ȸ��������޸�����
	for(int i=0;i<num;i++)
	 	pCF[i] = RLTOBL(pCFMap[i]);
	
	// 4��2 ��д��Ҫ�޸ĵ�����
	if(no == CF_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
			pCF[i] = lValueList[i];
	}
	else
		pCF[no]= *lValueList;		

	// 6��ע���޸������ֲ���
	pCFOpt->dwAppID 	= RtosGetCurrentTaskId();
	pCFOpt->dwOption1 	= no;
	pCFOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pCFOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д��ϵ��ֵȷ�ϡ�
	������	no,   		--  ϵ����š�ͬ�ϡ�
			Attr		--	��������,=DB_EXECUTE ִ��;=DB_CANCEL ȡ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCFValueConfirm(WORD no,WORD Attr)
{
	TDBaseConfigTable		*pDbase;				//���ݿ����ñ�
	TRunTimeOPT				*pCFOpt;
	long 					*pCF;
    WORD 					num;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pCF 	= pDbase->tModData.pCFData;
	pCFOpt	= pDbase->pCFOpt;
	num  	= pDbase->nCF;
	
	// 1���������Ϸ���
	if(pCFOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pCFOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pCFOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;

	// 2���������ָ��
	ASSERT(pCF != NULL);
	if(pCF == NULL)
		return DB_ERR_BASENULL;
	
	// 3����鶨ֵ���
	if((no != CF_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetCFValueConfirm",FILE_LINE,"The CF No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pCFOpt->wTimeCnt	= 0;
		pCFOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 4����д��RAM
	if(no == CF_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
			pDbase->pCFData[i] = pCF[i];
	}
	else
		pDbase->pCFData[no] = pCF[no];	

	DAE_MakeCheckSum((BYTE *)pDbase->pCFData, sizeof(float)*num);
	
	// 5�����汣�������ļ�
	rc = DB_ModifyDataFile(DB_DATA_CF,no);

	DAE_ClearRunningFlags(SS_EEPROM_CF_ERR); 	

	// 6��ע���޸���ѹ�����
	pCFOpt->wTimeCnt 	= 0;
	pCFOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_CF, no, 0, 0);

	DB_Register_CF(no);
	
	// 7���ٸ��¶��㶨ֵ
	DB_UpdateSETFixDataBase();
	
	return rc;
}

//BYTE TempTestBuffer[sizeof(float)*(RS_COUNT+1)*SETTING_AREA_COUNT];

STATUS DB_ModifyDataFile(WORD wType,WORD no)
{
    TFileHead         	*filehead;
	TMapDataList		*pMap;
	TMapDataList		*pMod;
	TRunTimeOPT			*pSETOpt;
//	long				*pCFData;					//ϵ������ָ��		
	WORD				num,i;
	DWORD				dwEEPROMAddr;
	DWORD				dwEEPROMLen;
	BYTE				*pEEPROMBuf;
	
//	DWORD				dwTemp;
	
	pMap = &G_pCfgTable->DBaseConfig.tMapData;
	pMod = &G_pCfgTable->DBaseConfig.tModData;

	ASSERT(pMap != NULL);
	ASSERT(pMod != NULL);
	if((pMap == NULL)||(pMod == NULL))
	{
		return DB_ERR_OPT_SAVE;
	}

	dwEEPROMAddr  = sizeof(TFileHead)+sizeof(TRelayDataTable);
	
	// 1�������ļ�����
	switch(wType)
	{
	case DB_DATA_SW:
		num = (DB_GetSWNum()+31)/32;
		for(i=0;i<num;i++)
		{
			pMap->pSWData[i] = RLTOBL(pMod->pSWData[i]);
			pMap->pSWData[num+i] = ~pMap->pSWData[i];
		}

		dwEEPROMLen = (SW_COUNT+31)/32*2*sizeof(DWORD);
		pEEPROMBuf = (BYTE *)pMap->pSWData;
		break;	
		
/*
	case DB_DATA_CFG:
		num = DB_GetCFGNum();
		for(i=0;i<(num+31)/32;i++)
		{
			dwTemp = *(DWORD *)(pMod->pSETData+i);
			*(DWORD *)(pMap->pSETData+i) = RLTOBL(dwTemp);
		}
		break;	
*/

	case DB_DATA_SET:
		pSETOpt	= G_pCfgTable->DBaseConfig.pSETOpt;
		ASSERT(pSETOpt->dwOption1<DB_GetSETAreaNum());
		if(pSETOpt->dwOption1 >= DB_GetSETAreaNum())
		{
			LogError("DB_ModifyDataFile", FILE_LINE, "Set SET Area No Err!");
			break;
		}
		
		num 	= DB_GetSETNum();
		for(i=0;i<num;i++)
		{
			pMap->pSETData[pSETOpt->dwOption1*(num+1)+i] = CONVERTFLOAT(pMod->pSETData[i]);
		}
		DAE_MakeCheckSum((BYTE *)&pMap->pSETData[pSETOpt->dwOption1*(num+1)],sizeof(float)*num);

//		LogError("DB_ModifyDataFile",FILE_LINE,"The Set Data Modified! Zero=%d, Num=%d Check=0x%x:%x ",\
//			G_pCfgTable->DBaseConfig.nSETAreaCur,G_pCfgTable->DBaseConfig.nSET,\
//			*((    (BYTE *)&pMap->pSETData[pSETOpt->dwOption1*(num+1)]  )+num*sizeof(float)),\
//			*((    (BYTE *)&pMap->pSETData[pSETOpt->dwOption1*(num+1)]  )+num*sizeof(float)+1));

		dwEEPROMAddr += (SW_COUNT+31)/32*2*sizeof(DWORD);
		dwEEPROMLen   = (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		pEEPROMBuf = (BYTE *)pMap->pSETData;

		break;	
		
	case DB_DATA_SETAREACUR:
		pMap->pRelayCfg->nSETAreaCur = RLTOBS(pMod->pRelayCfg->nSETAreaCur);
		pMap->pRelayCfg->nSETAreaCurB = RLTOBS(pMod->pRelayCfg->nSETAreaCurB);

		dwEEPROMAddr  = sizeof(TFileHead);
		dwEEPROMLen   = sizeof(TRelayDataTable);
		pEEPROMBuf = (BYTE *)pMap->pRelayCfg;
		break;	
		
	case DB_DATA_SYSSET:
		num 	= DB_GetSystemSETNum();
		for(i=0;i<num;i++)
		{
			pMap->pSystemSETData[i] = CONVERTFLOAT(pMod->pSystemSETData[i]);
		}
		DAE_MakeCheckSum((BYTE *)pMap->pSystemSETData,sizeof(float)*num);

		dwEEPROMAddr += (SW_COUNT+31)/32*2*sizeof(DWORD);
		dwEEPROMAddr += (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		dwEEPROMLen  = (AS_COUNT+1)*sizeof(float);
		pEEPROMBuf = (BYTE *)pMap->pSystemSETData;

		break;	

	case DB_DATA_CF:
/*
		num 	= DB_GetCFNum();
		//���ݶֵѡȡ��ͬ�Ķ�ֵ��
		if(DB_Is1ASystem())
		{
			pCFData = pMap->pCFData+num+1;
		}
		else
		{
			pCFData = pMap->pCFData;
		}
		
		for(i=0;i<num;i++)
		{
			pCFData[i] = RLTOBL(pMod->pCFData[i]);
		}
		
		DAE_MakeCheckSum((BYTE *)pCFData,sizeof(long)*num);
		DAE_MakeCheckSum((BYTE *)(pCFData+num+1),sizeof(long)*num);

		dwEEPROMAddr += (SW_COUNT+31)/32*2*sizeof(DWORD);
		dwEEPROMAddr += (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		dwEEPROMAddr += (AS_COUNT+1)*sizeof(float);
		dwEEPROMLen  = (CF_COUNT+1)*2*sizeof(long)+2;
		pEEPROMBuf = (BYTE *)pMap->pCFData;
*/

		// ��Ϊֱ��д��ָ��������
		DAE_MakeCheckSum((BYTE *)pMod->pCFData,sizeof(long)*CF_COUNT);

		if(DB_Is1ASystem())
			dwEEPROMAddr = EEPROM_RELAY_CFBASE+(CF_COUNT+1)*sizeof(long);
		else
			dwEEPROMAddr = EEPROM_RELAY_CFBASE;
			
		dwEEPROMLen  = (CF_COUNT+1)*sizeof(long);

		EEPROM_Write(dwEEPROMAddr,dwEEPROMLen,pMod->pCFData);

		// ֱ�ӷ���
		return DB_ERR_OK;	

	default:
		return DB_ERR_OPT_SAVE;
	}

	// 2�����¼����ļ�У��
	if(G_pCfgTable->DBaseConfig.pRelayDataFileMap == NULL)
		return DB_ERR_FILE_WRITE;
		
	filehead = (TFileHead *)G_pCfgTable->DBaseConfig.pRelayDataFileMap->pRamData;

	//�����޸�ʱ��
	ReadSystemTime(&filehead->stTime);	
	filehead->stTime.Year = ltobs(filehead->stTime.Year);
	filehead->stTime.MSecond= ltobs(filehead->stTime.MSecond);

	//�����޸ı�־
	filehead->dwMagic = ltobl(FILE_MAGIC_MODI);

	//���¼���У��
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-sizeof(WORD));	

	// 3��д���ļ�����
//	return DAE_WriteFile(RELAYDATAFILE, filehead,filehead->dwSize);

	// 3.1 �޸�����д��EEPROM�ļ�
	EEPROM_Write(EEPROM_RELAY_DATA+dwEEPROMAddr,dwEEPROMLen,pEEPROMBuf);

//	LogError("DB_ModifyDataFile",FILE_LINE,"write data! Start=0x%x, Len=%d", \
//		EEPROM_RELAY_DATA+dwEEPROMAddr,dwEEPROMLen);

	
	// 3.1 �ļ�ͷд��EEPROM�ļ�
	EEPROM_Write(EEPROM_RELAY_DATA,sizeof(TFileHead),(BYTE *)filehead);

//	LogError("DB_ModifyDataFile",FILE_LINE,"write head! Start=0x%x, Len=%d", \
//		EEPROM_RELAY_DATA,sizeof(TFileHead));

	// 3.2 �ļ�У��д��EEPROM
	pEEPROMBuf = (BYTE *)filehead;
	pEEPROMBuf += filehead->dwSize-sizeof(WORD);	
	EEPROM_Write(EEPROM_RELAY_DATA+filehead->dwSize-2,2,pEEPROMBuf);

//	LogError("DB_ModifyDataFile",FILE_LINE,"write check! Start=0x%x, Len=%d", \
//		EEPROM_RELAY_DATA+filehead->dwSize-2,2);

	//�ٶ�������һ��
//	EEPROM_Read(EEPROM_RELAY_DATA+dwEEPROMAddr,dwEEPROMLen,TempTestBuffer);
//	for(i=0;i<dwEEPROMLen;i++)
//	{
//		if(TempTestBuffer[i]!=pEEPROMBuf[i])
//		{
//			LogError("DB_ModifyDataFile",FILE_LINE,"The EEPROM write error! Pos=%d", i);
//			break;
//		}
//	}

	return DB_ERR_OK;
}

STATUS DB_SaveRelayEventFile()
{	
    TFileHead         		*filehead;
	TFileMap				*pFileMap;
	TRelayAlarmWrap			*pRALMWrap;
	TRelayActionWrap		*pRACTWrap;
	TRelayEventPtrList		*pPtrList;
	TRelayAlarmEv			*pRALMEvent;	
	TRelayActionEv			*pRACTEvent;	
	TAppInfo				tTempApp;
	WORD					wPtr;
	
	pRALMWrap 	= G_pCfgTable->DBaseConfig.pRALMWrap;
	pRACTWrap 	= G_pCfgTable->DBaseConfig.pRACTWrap;
	pFileMap 	= G_pCfgTable->DBaseConfig.pRelayEventFileMap;

	if((pRALMWrap == NULL)||(pRACTWrap == NULL))
	{
		LogError("DB_SaveRelayEventFile",FILE_LINE,"The wrap database is NULL!!!");
		return DB_ERR_BASENULL;
	}
	
	if(pFileMap == NULL)
		return DB_ERR_MAP_NULL;
	
	filehead = (TFileHead *)pFileMap->pRamData;
	pPtrList = (TRelayEventPtrList	*)(filehead+1);

	// 1�������ļ�����
	
/*
	// 1.1 �����澯����
	pRALMEvent = (TRelayAlarmEv  *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
					+sizeof(TRelayActionEv)*MAX_RACT_NUM);

	//���ڿձ�ֱ��ȫ��д��;����ֻд�²�����
	if(pPtrList->nRelayAlarmH == pPtrList->nRelayAlarmT)
		wPtr = pRALMWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayAlarmT%MAX_RALM_NUM;

	for(;wPtr !=pRALMWrap->nTailIndex;)
	{
		pRALMEvent[wPtr] = pRALMWrap->pEvent[wPtr];
					
		//�����ļ������淶��Ӧ�ø����ļ�������ģ���ļ�����Ч�ʾͱȽϵ���
		//����ֱ�Ӳ���EEPROM��				
		EEPROM_Write(EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead), \
						sizeof(TRelayAlarmEv), &pRALMEvent[wPtr]);

		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM alarm event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead),sizeof(TRelayAlarmEv));
		wPtr = (wPtr+1)%MAX_RALM_NUM;
	}
							
	// 1.2 ������������
	pRACTEvent = (TRelayActionEv	 *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList));
	
	//���ڿձ�ֱ��ȫ��д��;����ֻд�²�����
	if(pPtrList->nRelayActionH == pPtrList->nRelayActionT)
		wPtr = pRACTWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayActionT%MAX_RACT_NUM;

	for(;wPtr !=pRACTWrap->nTailIndex;)
	{
		pRACTEvent[wPtr] = pRACTWrap->pEvent[wPtr];
		//�����ļ������淶��Ӧ�ø����ļ�������ģ���ļ�����Ч�ʾͱȽϵ���
		//����ֱ�Ӳ���EEPROM��
		EEPROM_Write(EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead, \
						sizeof(TRelayActionEv), &pRACTEvent[wPtr]);
					
		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM action event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead,sizeof(TRelayActionEv));
		wPtr = (wPtr+1)%MAX_RACT_NUM;
	}
				
	// 1.3 ��������ָ��
	pPtrList->nMaxRalayAction = MAX_RACT_NUM;
	pPtrList->nRelayActionT = pRACTWrap->nTailIndex;
	pPtrList->nRelayActionH = pRACTWrap->nHeadIndex;
	
	pPtrList->nMaxRalayAlarm = MAX_RALM_NUM;
	pPtrList->nRelayAlarmT = pRALMWrap->nTailIndex;
	pPtrList->nRelayAlarmH = pRALMWrap->nHeadIndex;

	//����Һ���¼���ָ��
	if(DAE_Portid2AppInfo(SYS_PORTID_LCD,&tTempApp) == DB_ERR_OK)
	{
		pPtrList->nRelayActionLEDR = tTempApp.nRACTRead;
		pPtrList->nRelayAlarmLEDR = tTempApp.nRALMRead;
	}
	//��������1�¼���ָ��
	if((DAE_Portid2AppInfo((SC_PORT_CANBUS<<8)+1,&tTempApp) == DB_ERR_OK)\
		||(DAE_Portid2AppInfo((SC_PORT_ETHERNETA<<8)+1,&tTempApp) == DB_ERR_OK))
	{
		pPtrList->nRelayActionNet1R = tTempApp.nRACTRead;
		pPtrList->nRelayAlarmNet1R = tTempApp.nRALMRead;
	}
	//��������2�¼���ָ��
	if((DAE_Portid2AppInfo((SC_PORT_CANBUS<<8)+2,&tTempApp) == DB_ERR_OK)\
		||(DAE_Portid2AppInfo((SC_PORT_ETHERNETB<<8)+1,&tTempApp) == DB_ERR_OK))
	{
		pPtrList->nRelayActionNet2R = tTempApp.nRACTRead;
		pPtrList->nRelayAlarmNet2R = tTempApp.nRALMRead;
	}
*/
	// 1.1 �����澯����
	pRALMEvent = (TRelayAlarmEv  *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
					+sizeof(TRelayActionEv)*MAX_RACT_NUM);

	//���ڿձ�ֱ��ȫ��д��;����ֻд�²�����
	if(pPtrList->nRelayAlarmH == pPtrList->nRelayAlarmT)
		wPtr = pRALMWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayAlarmT%MAX_RALM_NUM;

	if(wPtr !=pRALMWrap->nTailIndex)
	{
		pRALMEvent[wPtr] = pRALMWrap->pEvent[wPtr];
					
		//�����ļ������淶��Ӧ�ø����ļ�������ģ���ļ�����Ч�ʾͱȽϵ���
		//����ֱ�Ӳ���EEPROM��				
		EEPROM_Write(EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead), \
						sizeof(TRelayAlarmEv), &pRALMEvent[wPtr]);

//		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM alarm event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead),sizeof(TRelayAlarmEv));
		wPtr = (wPtr+1)%MAX_RALM_NUM;

		pPtrList->nRelayAlarmT = wPtr;
		pPtrList->nRelayAlarmH = pRALMWrap->nHeadIndex;		
		pPtrList->nMaxRalayAlarm = MAX_RALM_NUM;
	}
							
	// 1.2 ������������
	pRACTEvent = (TRelayActionEv	 *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList));
	
	//���ڿձ�ֱ��ȫ��д��;����ֻд�²�����
	if(pPtrList->nRelayActionH == pPtrList->nRelayActionT)
		wPtr = pRACTWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayActionT%MAX_RACT_NUM;

	if(wPtr !=pRACTWrap->nTailIndex)
	{
		pRACTEvent[wPtr] = pRACTWrap->pEvent[wPtr];
		//�����ļ������淶��Ӧ�ø����ļ�������ģ���ļ�����Ч�ʾͱȽϵ���
		//����ֱ�Ӳ���EEPROM��
		EEPROM_Write(EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead, \
						sizeof(TRelayActionEv), &pRACTEvent[wPtr]);
					
//		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM action event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead,sizeof(TRelayActionEv));
		wPtr = (wPtr+1)%MAX_RACT_NUM;

		pPtrList->nRelayActionT = wPtr;
		pPtrList->nRelayActionH = pRACTWrap->nHeadIndex;
		pPtrList->nMaxRalayAction = MAX_RACT_NUM;
	}
				
	ReadSystemTime(&filehead->stTime);

	//BigEnden�洢
	filehead->wFlag = 0x8000;
	
	// 2�����¼����ļ�У��
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-sizeof(WORD));	
	
	// 3��д���ļ�����
	//�����ļ������淶��Ӧ�ø����ļ�������ģ���ļ�����Ч�ʾͱȽϵ���
	//����ֱ�Ӳ���EEPROM��

	// 3.1 д�ļ�ͷ��ָ���
	EEPROM_Write(EEPROM_RELAY_EVENT, sizeof(TFileHead)+sizeof(TRelayEventPtrList), filehead);
//	LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM relay event file Head! Addr=0x%08x Len=%d",EEPROM_RELAY_EVENT,sizeof(TFileHead)+sizeof(TRelayEventPtrList));

	// 3.2 дУ��
	EEPROM_Write(EEPROM_RELAY_EVENT+filehead->dwSize-2, 2, (BYTE *)filehead+filehead->dwSize-2);
//	LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM relay event file check! Addr=0x%08x",EEPROM_RELAY_EVENT+filehead->dwSize-2);

	if(pPtrList->nRelayActionT ==pRACTWrap->nTailIndex)
	if(pPtrList->nRelayAlarmT ==pRALMWrap->nTailIndex)
		pFileMap->wFlag &= ~FILEMAP_FLAGS_MODIFY;
	
	return DB_ERR_OK;	
}

/*=================¼��������=======================*/

/************************************************************
* ����˵��      - dwStartPtr	: ���� 	ԭʼ�������ݻ�������ʼλ��
*				- wPointNum : ����	����Ƚϵ�ͨ������
*************************************************************/
/*
BOOL isSampDataLenFill(DWORD dwStartPtr, DWORD wPointNum)
{
	return TRUE;
}
*/
/************************************************************
* ����˵��		- dwPtr    : ����  ��ʷ������ʼλ��
*				- ChanMask : ����  ͨ������
*				- pBuf	   : ���  ��Ҫ��������ݻ�����
*				- wPointNum    : ����  ��Ҫ��ȡ�Ĳ�������
*************************************************************/
/*
STATUS ReadHisSampData(DWORD dwPtr, DWORD dwMask, SHORT *pBuf, DWORD dwPointNum)
{
	WORD	wCnt=0;
	BYTE	*pDistData = (BYTE *)pBuf;
	
	for(int i=0;i<32;i++)
		if(dwMask&(0x01<<i))
			wCnt++;
		
	if(dwPtr == 0)
	{
		//�ܲ�������
		for(int m=0;m<dwPointNum;m++)
		{
			//��ģ��ͨ����ֵ
			for(int n=0;n<wCnt;n++)
			{
				pDistData[m*wCnt*2+n*2] = 0x11*(n+1)+(m%10);
				pDistData[m*wCnt*2+n*2+1] = 0;//0x11*(n+1)+(m%10);
			}
		}	
	}
	else
	{
		//�ܲ�������
		for(int m=0;m<dwPointNum;m++)
		{
			//��ģ��ͨ����ֵ
			for(int n=0;n<wCnt;n++)
			{
				pDistData[m*wCnt*2+n*2] = 0x11*(n+1);
				pDistData[m*wCnt*2+n*2+1] = 0x11*(n+1);
			}
		}	
	}	

	return DB_ERR_OK;
}
*/
/*=================¼��������=======================*/

static void FormatDecData(char *str,DWORD data,BYTE MaxLen)
{
	BYTE 	BitData;
	DWORD	HiData;
	
	HiData = data;
	str[MaxLen] = '\0';
	
	for(int i=0;i<MaxLen;i++)
	{
		BitData = HiData%10;
		str[MaxLen-1-i] = BitData+'0';
		HiData /= 10;		
	}
}

/************************************************************
	���ܣ���ȡ¼�������ļ���
	����: pDistBlock	-- ¼������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
BOOL	DB_GetDistFileName(TAbsTime *pTime,char *strFileName)
{
	TSysTime	tSysTime;
	
	// 1.2 ת����ϵͳʱ��
	AbsTimeTo(pTime, &tSysTime);
	tSysTime.Year %= 100;
	
	strFileName[0] = '0'+tSysTime.Year/10;
	strFileName[1] = '0'+tSysTime.Year%10;
	strFileName[2] = '0'+tSysTime.Month/10;
	strFileName[3] = '0'+tSysTime.Month%10;
	strFileName[4] = '0'+tSysTime.Day/10;
	strFileName[5] = '0'+tSysTime.Day%10;
	strFileName[6] = '0'+tSysTime.Hour/10;
	strFileName[7] = '0'+tSysTime.Hour%10;
	strFileName[8] = '0'+tSysTime.Minute/10;
	strFileName[9] = '0'+tSysTime.Minute%10;
	strFileName[10] = '0'+tSysTime.Second/10;
	strFileName[11] = '0'+tSysTime.Second%10;
	strFileName[12] = '0'+tSysTime.MSecond/100;
	strFileName[13] = '0'+(tSysTime.MSecond%100)/10;
	strFileName[14] = '0'+tSysTime.MSecond%10;
	strFileName[15] = '\0';
	
	return TRUE;
}

/************************************************************
	���ܣ���ȡ¼�����ݳ���
	����: None
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
DWORD DB_GetDistDataLength()
{
	TDist_Disp		*pDistDisp;
	DWORD			dwBLength;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	if(pDistDisp == NULL)
		dwBLength= MAX_DISTBLOCK_LEN;
	else
	{
		//ÿ����Ŀ����
		dwBLength = 4+4+pDistDisp->wAnalogChannelNum*2+(pDistDisp->wDigitalChannelNum+15)/16*2;
		//������Ŀ����
		dwBLength *= pDistDisp->dwTotalSampleNum;
		//����һ��ͷ
		dwBLength += sizeof(TDistHead);
	}

	//���ٷ���2K
	if(dwBLength < 2048)
		dwBLength = 2048;

	//������128K
	//if(dwBLength > MAX_DISTBLOCK_LEN)
	//	dwBLength = MAX_DISTBLOCK_LEN;

	return dwBLength;
}

/************************************************************
	���ܣ�д��¼�����ݿ�
	����: pDistBlock	-- ¼������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_WriteDistDataBlock (void *pDistBlock)
{
    CNode 				*pNode;
	
	// 1���������ָ��ĺϷ���
	ASSERT(pDistBlock != NULL);
	if(pDistBlock == NULL)
		return DB_ERR_PTRNULL;
	
	// 2��д��¼��������Ϣ	
	OS_Protect(0);
	
	// 3.1 ����һ���������ݿ�ڵ�
	pNode = G_DistDataFreeLink.RemoveHead();
	if(pNode == NULL)
	{
		OS_UnProtect(0);
		LogError("DB_RelayWriteDistData",FILE_LINE,"The Dist Block run out!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
	
	// 3.2 �������ݿ�
	memcpy(pNode->pData,pDistBlock,DB_GetDistDataLength());	
	
	// 3.3 ����CACHE¼��������
	G_DistDataSaveLink.AddTail(pNode);
	
	OS_UnProtect(0);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����һ��¼����д�봥��ǰ¼������
	����: 	pTime		-- ����ʱ��ָ��
			dwSamplePtr	-- ����ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_StartDistRecord(DWORD	dwSamplePtr,TAbsTime *pTime,DWORD dwPrevSampleNum)
{
    CNode 				*pNode;
	TDistHead			*pHead;
	TDist_Disp			*pDistDisp;
	TAC_Disp			*pACDisp;
	TDC_Disp			*pDCDisp;
	WORD				i;
	
	if(G_DistDataFreeLink.IsEmpty())
		return DB_ERR_OK;
	
	// �ж����ݿ�ָ��Ϸ���
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pACDisp = G_pCfgTable->DBaseConfig.pACPara;
	pDCDisp = G_pCfgTable->DBaseConfig.pDCPara;
	if((pDistDisp == NULL)||(pACDisp == NULL)||(pDCDisp == NULL))
		return DB_ERR_BASENULL;
	
	// 1. ��ȡ���нڵ�
	pNode = G_DistDataFreeLink.RemoveHead();
	if(pNode == NULL)
	{
		LogError("DB_StartDistRecord",FILE_LINE,"The Dist Block run out!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
			
	// 2. ����¼������ͷ
	pHead = (TDistHead *)pNode->pData;

	pHead->wStatus 	= DIST_STATUS_BUSY;
	pHead->wPerSize = pDistDisp->wAnalogChannelNum*2+(pDistDisp->wDigitalChannelNum+15)/16*2;
	pHead->dwSamplePtr 	= dwSamplePtr;
	pHead->tTime 		= *pTime;

	// 2.1 ��������������ݴ���
	if(dwPrevSampleNum <= pDistDisp->dwTotalSampleNum)
	{
		pHead->dwPrevSampleNum = dwPrevSampleNum;
	}
	else if(pDistDisp->dwPrevSampleNum <= pDistDisp->dwTotalSampleNum)
	{
		pHead->dwPrevSampleNum = pDistDisp->dwPrevSampleNum;
	}
	else
	{
		pHead->dwPrevSampleNum = pDistDisp->dwTotalSampleNum;
	}
	
	pHead->dwRearSampleNum = pDistDisp->dwTotalSampleNum-pHead->dwPrevSampleNum;
	
	// 2.2 ����ͨ������
	pHead->dwMask = 0;
	for(i=0;i<pDistDisp->wAnalogChannelNum;i++)
		pHead->dwMask |= 0x01<<pACDisp[i].wIndex;
	for(i=0;i<(pDistDisp->wDigitalChannelNum+15)/16;i++)
		pHead->dwMask |= 0x01<<(AD_CHAN_NUM+A_COM_CHAN_NUM+i);

	dwSamplePtr = BackSamplePtr(dwSamplePtr,pHead->dwPrevSampleNum);
	
	// 3��д�봥��ǰ¼��������Ϣ
	ReadHisSampData(dwSamplePtr,pHead->dwMask,(SHORT *)(pHead+1),pHead->dwPrevSampleNum);
	
	//ReportMsg("DB_StartDistRecord:  Start dist(%x->%x) PrevSampleNum =%d",(DWORD)pNode,(DWORD)pHead,pHead->dwPrevSampleNum);

	// 4. �ڵ�д����洢����	
	G_DistDataSaveLink.AddTail(pNode);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����д��¼�����ݿ�
	����: pDistBlock	-- ¼������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_TraceDistRecord ()
{
	TDistHead			*pHead;
    CNode 				*pTempNode;
	BYTE				*pDistData;

	if(G_DistDataSaveLink.IsEmpty())
		return DB_ERR_OK;

	// 1. ˳�����������ڵ�
	G_DistDataSaveLink.ToHead();
	
	pTempNode = G_DistDataSaveLink.Next();

	//�ж������Ƿ�Ϊ����,����Ϊ��������
	while(pTempNode != NULL)
    {
		// 2. �Դ�׷�ٵĽڵ����׷��
		pHead = (TDistHead *)pTempNode->pData;
		if(pHead->wStatus == DIST_STATUS_BUSY)
		{
			// 3. ¼��������д��ڵ��ڴ�
			if(isSampDataLenFill(pHead->dwSamplePtr,pHead->dwRearSampleNum))
			{
				// 3.1 д������
				pDistData = (BYTE *)(pHead+1);
				pDistData += pHead->wPerSize*pHead->dwPrevSampleNum;
				ReadHisSampData(pHead->dwSamplePtr,pHead->dwMask,(SHORT *)pDistData,pHead->dwRearSampleNum);

				// 3.2 ����¼���洢״̬
				pHead->wStatus = DIST_STATUS_SAVE;

				//ReportMsg("DB_TraceDistRecord:	Stop dist(%x->%x) RearSampleNum =%d",(DWORD)pTempNode,(DWORD)pHead,pHead->dwRearSampleNum);
			}
		}
		
		pTempNode = G_DistDataSaveLink.Next();
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����¼�������ļ�
	����: pDistBlock	-- ¼������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SaveDistConfFile(char	 *strFileName,TAbsTime *pDistTime,DWORD dwPrevSampleNum)
{
	char				strConfigFileName[32];
	char 				strFHour[3];
	char 				strFMinute[3];
	char 				strFSecond[3];	
	char 				strFMSecond[7];
	
	TAC_Disp			*pACDisp;
	TDC_Disp			*pDCDisp;
	TDist_Disp			*pDistDisp;
	char				*pFileData;
	TCF_Disp			tCFDisp;
	TSysTime			tSysTime;
	TAbsTime			t1,t2;
	long				lCFValue = 1;
	long				lCFRatio = 1;
	float				a,fRTValue;
	WORD				wDCNum,wACNum;
	STATUS				rc;
	int					i;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pACDisp = G_pCfgTable->DBaseConfig.pACPara;
	pDCDisp = G_pCfgTable->DBaseConfig.pDCPara;

	if((pDistDisp == NULL)||(pACDisp == NULL)||(pDCDisp == NULL))
		return DB_ERR_BASENULL;
			
	// 1.��д�������ļ�
		
	// 1.1 ���������ļ� 
	pFileData = (char *)G_pDistFileBlock;

	// station_name,recdev_id,rev_year<CR/LF>
	pFileData[0] = '\0';
	sprintf(pFileData,"KEYVIA,%s,2000\r\n",pDistDisp->strDevName);
	pFileData += strlen(pFileData);
	
	// TT,##A,##D<CR/LF>
	wDCNum	= pDistDisp->wDigitalChannelNum;
	wACNum	= pDistDisp->wAnalogChannelNum;

	pFileData[0] = '\0';
	sprintf(pFileData,"%d,%dA,%dD\r\n",wACNum+wDCNum,wACNum,wDCNum);
	pFileData += strlen(pFileData);
	
	// An,ch_id,ph,ccbm,uu,a,b,skew,min,max,primary,secondary,PS<CR/LF>
	for(i=0;i<wACNum;i++)
	{
		// ȡϵ��
		rc = DB_GetCFValue(pACDisp[i].wCoefIndex, &lCFValue);
		if((rc != DB_ERR_OK)||(lCFValue == 0))
		{
			lCFValue = 1;
			LogError("DB_SaveDistConfFile",FILE_LINE,"InValid AC's CF !!!");
		}
		
		rc = DB_GetCFDisp(pACDisp[i].wCoefIndex, &tCFDisp);
		if((rc != DB_ERR_OK)||(tCFDisp.wCof == 0))
		{
			lCFRatio = 1;
			LogError("DB_SaveDistConfFile",FILE_LINE,"InValid AC's CF Disp !!!");
		}
		else
			lCFRatio = tCFDisp.wCof;
	
		a = ((float)lCFRatio)/((float)lCFValue);

		//ȡ���
		rc = DB_GetRTValue(pACDisp[i].wRTIndex,&fRTValue);
		if((rc != DB_ERR_OK)||(fRTValue == 0))
		{
			fRTValue = 1;
			LogError("DB_SaveDistConfFile",FILE_LINE,"InValid RT Value !!!");
		}
		
		//�������ű���
		if(pACDisp[i].wCompressMultiple != 0)
			a *= pACDisp[i].wCompressMultiple;

		//����ϵ������
		a *= pACDisp[i].fCFRatio;
		
		pFileData[0] = '\0';
		sprintf(pFileData,"%d,%s,,,%s,%f,0,0,-32768,32767,%d,1,S\r\n",\
			i+1,pACDisp[i].strName,pACDisp[i].strUnit,a,\
			/*(float)1000000/(pDistDisp->wSampleRate*pDistDisp->wAnalogChannelNum),\*/
			(long)fRTValue);
		pFileData += strlen(pFileData);
	}
	
	// Dn,ch_id,ph,ccbm,y<CR/LF>
	for(i=0;i<wDCNum;i++)
	{
		pFileData[0] = '\0';
		sprintf(pFileData,"%d,%s,,,0\r\n",i+1,pDCDisp[i].strName);
		pFileData += strlen(pFileData);
	}
	
	// If<CR/LF>
	pFileData[0] = '\0';
	if(pDistDisp->wSysFrequency%100)
		sprintf(pFileData,"%d.%d\r\n",pDistDisp->wSysFrequency/100,pDistDisp->wSysFrequency%100);
	else
		sprintf(pFileData,"%d\r\n",pDistDisp->wSysFrequency/100);
	pFileData += strlen(pFileData);
	
	// nrates
	pFileData[0] = '\0';
	sprintf(pFileData,"1\r\n");
	pFileData += strlen(pFileData);
	
	// samp,endsamp
	pFileData[0] = '\0';
	sprintf(pFileData,"%d,%d\r\n",pDistDisp->wSampleRate,pDistDisp->dwTotalSampleNum);
	pFileData += strlen(pFileData);
	
	// dd/mm/yyyy,hh:mm:ss.ssssss<CR/LF>

	// �����һ�����ݵ�ʱ��
	t1 = *pDistTime;
	t2.Hi = 0;
	t2.Lo = 1000*dwPrevSampleNum/pDistDisp->wSampleRate;
	AbsTimeSub(&t1,&t2);

	AbsTimeTo(&t1,&tSysTime);

	//����ʽ��ʱ��
	FormatDecData(strFHour, tSysTime.Hour,2);
	FormatDecData(strFMinute, tSysTime.Minute,2);
	FormatDecData(strFSecond, tSysTime.Second,2);
	FormatDecData(strFMSecond, tSysTime.MSecond*1000,6);
	
	pFileData[0] = '\0';
	sprintf(pFileData,"%d/%d/%d,%s:%s:%s.%s\r\n",tSysTime.Day,tSysTime.Month,tSysTime.Year,\
			strFHour,strFMinute,strFSecond,strFMSecond);
	pFileData += strlen(pFileData);

	// dd/mm/yyyy,hh:mm:ss.ssssss<CR/LF>

	//���㴥����ʱ��
	AbsTimeTo(pDistTime,&tSysTime);
	
	//����ʽ��ʱ��
	FormatDecData(strFHour, tSysTime.Hour,2);
	FormatDecData(strFMinute, tSysTime.Minute,2);
	FormatDecData(strFSecond, tSysTime.Second,2);
	FormatDecData(strFMSecond, tSysTime.MSecond*1000,6);
	
	pFileData[0] = '\0';
	sprintf(pFileData,"%d/%d/%d,%s:%s:%s.%s\r\n",tSysTime.Day,tSysTime.Month,tSysTime.Year,\
		strFHour,strFMinute,strFSecond,strFMSecond);
	pFileData += strlen(pFileData);
	
	// ft<CR/LF>
	pFileData[0] = '\0';
	sprintf(pFileData,"BINARY\r\n");
	pFileData += strlen(pFileData);
	
	// Timemult<CR/LF>
	pFileData[0] = '\0';
	sprintf(pFileData,"1\r\n");
	pFileData += strlen(pFileData);

	// 1.2 ����д���ļ�
	strcpy(strConfigFileName,strFileName);
	strcat(strConfigFileName,".CFG");

	return DAE_WriteFile(strConfigFileName,(BYTE *)G_pDistFileBlock,pFileData-(char *)G_pDistFileBlock);
}

/************************************************************
	���ܣ�����¼�������ļ�
	����: pDistBlock	-- ¼������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SaveDistDataFile(char	 *strFileName,BYTE *pDistData)
{
	char				strDataFileName[32];
	TDist_Disp			*pDistDisp;
	TAC_Disp			*pACDisp;
	BYTE				*pFileData;
	WORD				wACNum,wDCNum;
	DWORD				dwOffset,dwSampleTime;
	short				sSampleValue;
	STATUS				rc;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pACDisp = G_pCfgTable->DBaseConfig.pACPara;
	if((pDistDisp == NULL)||(pACDisp == NULL))
		return DB_ERR_BASENULL;

	// 1.�ȴ��������ļ�
	
	// 2.���������ļ� 
	pFileData = (BYTE *)G_pDistFileBlock;
	pDistData += sizeof(TDistHead);
	
	dwOffset = 0;
	dwSampleTime = 0;

	wDCNum	= (pDistDisp->wDigitalChannelNum+15)/16;
	wACNum	= pDistDisp->wAnalogChannelNum;
			
	//�ӵ�һ�β�����ʼ�����仺����
 	for(long i=0; i<pDistDisp->dwTotalSampleNum; i++)
	{
		// 2.1 ���
		pFileData[dwOffset++] = LLBYTE(i+1);
		pFileData[dwOffset++] = LHBYTE(i+1);
		pFileData[dwOffset++] = HLBYTE(i+1);
		pFileData[dwOffset++] = HHBYTE(i+1);

		// 2.2 ����ʱ��
		pFileData[dwOffset++] = LLBYTE(dwSampleTime);
		pFileData[dwOffset++] = LHBYTE(dwSampleTime);
		pFileData[dwOffset++] = HLBYTE(dwSampleTime);
		pFileData[dwOffset++] = HHBYTE(dwSampleTime);
		//�п�������ۼ����!!!
		dwSampleTime += 1000000/pDistDisp->wSampleRate;

		// 2.3 �������ģ��������
		for(int m=0;m<wACNum;m++)
		{
			sSampleValue = MAKEWORD(pDistData[m*2+1],pDistData[m*2]);
			//����ѹ������
			if(pACDisp[m].wCompressMultiple != 0)
				sSampleValue /= pACDisp[m].wCompressMultiple;
			
			pFileData[dwOffset++] = LOBYTE(sSampleValue);
			pFileData[dwOffset++] = HIBYTE(sSampleValue);
		}
		

		// 2.4 �����������������
		pDistData += wACNum*2;

		for(int n=0;n<wDCNum;n++)
		{
			pFileData[dwOffset++] = pDistData[n*2+1];
			pFileData[dwOffset++] = pDistData[n*2];
		}
		
		pDistData += wDCNum*2;



//		memcpy(pFileData+dwOffset,pDistData+wACNum*2,wDCNum*2);
//		dwOffset += wDCNum*2;

//		pDistData += wACNum*2+wDCNum*2;
	}

	// 3. ����д���ļ�
	strcpy(strDataFileName,strFileName);
	strcat(strDataFileName,".DAT");

//	TAbsTime	t1,t2;
//	ReadAbsTime(&t1);
	rc = DAE_WriteFile(strDataFileName,(BYTE *)G_pDistFileBlock,dwOffset);
//	ReadAbsTime(&t2);

//	AbsTimeSub(&t2, &t1);
//	ReportMsg("Write Dist_Data File Last %d MS",t2.Lo);
	
	return rc;
}

/************************************************************
	���ܣ���������ɾ��¼���ļ�
	����: none
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
void DB_DeleteIndexDistFile(char *strFileName)
{
	char	strIndexFileName[DIST_INDEX_NAMELEN];
	char	strOldFileName[DIST_INDEX_NAMELEN];
	char	*pTempFileName;

	strIndexFileName[0] = 0;

	//����Խ��
	strFileName[DIST_INDEX_NAMELEN-1] = 0;
	
	strcat(strIndexFileName,strFileName);
	
	if(pTempFileName=strstr(strIndexFileName,"INDEX_"))
	{
		//ɾ�������ļ�
		strcpy(strOldFileName,pTempFileName+6);
		strcat(strOldFileName,".CFG");
		FS_DelFile(strOldFileName,0);

		//ɾ�������ļ�
		strcpy(strOldFileName,pTempFileName+6);
		strcat(strOldFileName,".DAT");
		FS_DelFile(strOldFileName,0);
	}
}

/************************************************************
	���ܣ�����¼�������ļ�
	����: none
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateDistIndex(char	 *strFileName)
{
	BYTE			*pFileData;
	TDistIndex		*pDistList;
	TDist_Disp		*pDistDisp;
	TSysTime		tSysTime;
	DWORD			dwFileLength,dwRtnLength;
	char			strIndexFileName[DIST_INDEX_NAMELEN];
	STATUS			rc;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	if(pDistDisp == NULL)
		return DB_ERR_BASENULL;

	pFileData = (BYTE *)G_pDistFileBlock;
	pDistList = (TDistIndex *)pFileData;
	
	// 1����¼�������ļ�
	dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*DIST_CAPACITY_MAX;
	memset(pFileData,0x00,dwFileLength);
	
	rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwFileLength != dwRtnLength)||(pDistList->wMagic != DIST_INDEX_INIT))
//	rc = EEPROM_Read(EEPROM_DISTINDEX_START, dwFileLength, pFileData);
//   if( (rc == FILE_ERROR)||(pDistList->wMagic != DIST_INDEX_INIT))
    {
		// �����ļ�����
		LogError("DB_UpdateDistIndex",FILE_LINE,"\n Error on open DistList.ind!");
		pDistList->wMagic = DIST_INDEX_INIT;
		pDistList->nMax 		= pDistDisp->wRecordCapacity;
		pDistList->nHeadIndex	= 0;
		pDistList->nTailIndex	= 0;
    }
			
	// 3������¼���ļ���

	pFileData += sizeof(TDistIndex);
	
	// 3.1 ���¼�������仯��ֱ�Ӹ�λָ��
	if((pDistDisp->wRecordCapacity) != pDistList->nMax)
	{
		//�Ѷ�����ļ�ɾ��
		for(int i=pDistDisp->wRecordCapacity;i<DIST_CAPACITY_MAX;i++)
			DB_DeleteIndexDistFile((char *)pFileData+i*DIST_INDEX_NAMELEN);
		
		pDistList->nMax = pDistDisp->wRecordCapacity;
		pDistList->nHeadIndex = pDistList->nTailIndex = 0;
	}
	
	pFileData += pDistList->nTailIndex*DIST_INDEX_NAMELEN;

	// 3.2 ɾ����ǰ�ռ���ļ�
	DB_DeleteIndexDistFile((char *)pFileData);
	
	// 3.3 ����µ�����
	memset(strIndexFileName,0,DIST_INDEX_NAMELEN);
	strcpy(strIndexFileName,"INDEX_");
	strcpy(strIndexFileName+6,strFileName);
	memcpy(pFileData,strIndexFileName,DIST_INDEX_NAMELEN);

	// 3.4 ����ָ��
	pDistList->nTailIndex++;
	pDistList->nTailIndex %= pDistList->nMax;
	if(pDistList->nHeadIndex == pDistList->nTailIndex)
	{
		pDistList->nHeadIndex++;
		pDistList->nHeadIndex %= pDistList->nMax;
	}	

	// 3.5 ���������ļ�ʱ��,BCD��
	ReadSystemTime(&tSysTime);
	tSysTime.Year 		%= 100;
	pDistList->Year 	= ((tSysTime.Year/10)<<4)+(tSysTime.Year%10);
	pDistList->Month	= ((tSysTime.Month/10)<<4)+(tSysTime.Month%10);
	pDistList->Day		= ((tSysTime.Day/10)<<4)+(tSysTime.Day%10);
	pDistList->Hour		= ((tSysTime.Hour/10)<<4)+(tSysTime.Hour%10);
	pDistList->Minute	= ((tSysTime.Minute/10)<<4)+(tSysTime.Minute%10);
	pDistList->Second	= ((tSysTime.Second/10)<<4)+(tSysTime.Second%10);
	pDistList->MSecondHi= tSysTime.MSecond/100;
	pDistList->MSecondLo= tSysTime.MSecond%100;
	pDistList->MSecondHi	= ((pDistList->MSecondHi/10)<<4)+(pDistList->MSecondHi%10);
	pDistList->MSecondLo	= ((pDistList->MSecondLo/10)<<4)+(pDistList->MSecondLo%10);
		
	// 4��д���ļ�
//	rc = EEPROM_Write(EEPROM_DISTINDEX_START, dwFileLength, G_pDistFileBlock);
//	if(rc != DB_ERR_OK)
//		LogError("DB_UpdateDistIndex",FILE_LINE,"\n Error on write DistList.ind!");
	
	return DAE_WriteFile("DistList.ind",(BYTE *)G_pDistFileBlock,dwFileLength);
}

/************************************************************
	���ܣ��洢¼�������ļ�
	����: none
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	DB_SaveDistRecord()
{
	char				strFileName[32];
    CNode 				*pTempNode;
	BYTE				*pDistData;
	TDistHead			*pHead;
	static STATUS				rc;

	G_DistDataSaveLink.ToHead();
	
	pTempNode = G_DistDataSaveLink.Next();

	//�ж������Ƿ�Ϊ����,����Ϊ��������
	while(pTempNode != NULL)
    {
		pDistData = (BYTE *)pTempNode->pData;
		pHead = (TDistHead	*)pDistData;

		if(pHead->wStatus == DIST_STATUS_SAVE)
		{
			// 1������¼��ʱ�������ļ���
			rc = DB_GetDistFileName(&pHead->tTime,strFileName);

			// 2�������ļ�����
			rc = DB_UpdateDistIndex(strFileName);
			
			// 3�����������ļ�
			rc = DB_SaveDistConfFile(strFileName,&pHead->tTime,pHead->dwPrevSampleNum);
			
			// 4�����������ļ�
			rc = DB_SaveDistDataFile(strFileName,pDistData);
			
			// 5��������нڵ�����
			pHead->wStatus = DIST_STATUS_IDLE;
			// 5.1 ɾ�������̽ڵ�
			G_DistDataSaveLink.RemoveNode(pTempNode);
			// 5.2 ������нڵ�����
			G_DistDataFreeLink.AddTail(pTempNode);

			// �����й�Լ������¼�������Ϣ
			for(int i=SYS_PORT_MAX;i<DAE_MAX_APP_NUM;i++)
			{
				if(G_pCfgTable->TaskApp[i].hTaskID != APP_TASKID_NULL)
					OS_PostMessage(G_pCfgTable->TaskApp[i].hTaskID, (WORD)SM_DISTEVENT, 0,0,0,0);
			}

			break;
		}
		
		pTempNode = G_DistDataSaveLink.Next();
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ����±��������ļ�
	������NONE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateRelayDataMap()
{
	BYTE					*pParaBuf;
	TRelayDataTable 		*pRelayDataTable;
	TFileMap				*pFileMap;		//���������ļ�������ָ��	
	TDBaseConfigTable		*pDbase;
	STATUS					rc;
	DWORD					filelength;

	pDbase = &G_pCfgTable->DBaseConfig;
	pFileMap = pDbase->pRelayDataFileMap;

	// 1 ���ļ�����
	rc = DAE_GetFileLength(RELAYDATAFILE,&filelength);
	if( rc != DB_ERR_OK)
		return DB_ERR_FILE_READ;

	// 2 ����ļ�����
	if(filelength > MAX_FILE_LEN)
		return DB_ERR_FILE_CHECK;
	if(pFileMap->nSize != ltobl(filelength))
		return DB_ERR_FILE_CHECK;
	
	// 3 ȡ�ļ�������
	pParaBuf = (BYTE *)OS_MemAlloc(filelength);
	if( pParaBuf == NULL )
		return DB_ERR_PTRNULL;

	// 4 ��ȡ�ļ�
	rc = DAE_ReadFile(RELAYDATAFILE,pParaBuf,filelength);
	
	// 5 ����ļ�����У��
	if( (rc != DB_ERR_OK)||(DAE_CheckSystemFile(pParaBuf,filelength) != DB_ERR_OK))
	{
		OS_MemFree(pParaBuf);
		return DB_ERR_FILE_CHECK;
	}

	// ����ļ��ṹ
	pRelayDataTable = (TRelayDataTable	*)(pParaBuf+sizeof(TFileHead));
	if((pRelayDataTable->nSW != ltobs(pDbase->nSW))||\
		(pRelayDataTable->nSET != ltobs(pDbase->nSET))||\
		(pRelayDataTable->nSystemSET!= ltobs(pDbase->nSystemSET))||\
		(pRelayDataTable->nCF != ltobs(pDbase->nCF)))
	{
		OS_MemFree(pParaBuf);
		return DB_ERR_FILE_CHECK;
	}	
	
	// 6 ����RAM������
	
	// 6.1 �����ļ������
	pFileMap->wFileType 	= 0;
	pFileMap->wFlag			|= FILEMAP_FLAGS_MODEND;
	pFileMap->dwFlashOff 	= NULL;

	memcpy(pFileMap->pRamData,pParaBuf,filelength);	

	pFileMap->nSize 		= ltobl(filelength);
	pFileMap->absTime.Lo	= ltobl(G_pCfgTable->SystemStatus.stTime.Lo);
	pFileMap->absTime.Hi	= ltobs(G_pCfgTable->SystemStatus.stTime.Hi);
	pFileMap->wCheck		= DAE_CheckSum((BYTE *)pFileMap,(BYTE *)&pFileMap->wCheck-(BYTE *)pFileMap);
	pFileMap->wCheck		= ltobs(pFileMap->wCheck);

	OS_MemFree(pParaBuf);
	return DB_ERR_OK;
}

/************************************************************
	���ܣ����¸��㶨ֵ
	������NONE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateSETFloatBase()
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	float 					*pDbaseSET;
	float 					*pMapSET;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	pDbaseSET = pDbase->pSETDataFloat;
	pMapSET = pDbase->tMapData.pSETData;
	num = pDbase->nSET;
	
	if((pDbaseSET == NULL)||(pMapSET == NULL))
		return DB_ERR_BASENULL;
	
	// 1.��RAM�ļ������лָ�
	if(!DB_CheckSETData(pMapSET,num))
	{
		// 2.��EEPROM�ļ��лָ�
		if(DB_UpdateRelayDataMap() != DB_ERR_OK)
			return DB_ERR_FILE_CHECK;
	}
	
	// 2 ���¼��
	if(!DB_CheckSETData(pMapSET,num))
	{
		DAE_SetRunningFlags(SS_EEPROM_SET_ERR);
		return DB_ERR_CHECK;
	}
	
	// 3 ˢ������

	// 3.1 �ֽ�ת��
	for(i=0;i<num;i++)
		pDbaseSET[i] = CONVERTFLOAT(pMapSET[i+pDbase->nSETAreaCur*(num+1)]);
	
	// 3.2 ���¼���У��
	DAE_MakeCheckSum((BYTE *)pMapSET,sizeof(float)*num);

	// 4.������Ϣ����¼����
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(SET_VALUE_ALL,SET_AREA_CURRENT), 0, 0);

	DB_Register_SET(SET_AREA_CURRENT,SET_VALUE_ALL);

	// 5.�ٸ��¶��㶨ֵ
	DB_UpdateSETFixDataBase();

	return DB_ERR_OK;
}

/************************************************************
	���ܣ����¸�����ֵ
	������NONE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateSystemSETBase()
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	float 					*pDbaseSET;
	float 					*pMapSET;
	
	pDbase 		= &G_pCfgTable->DBaseConfig;
	pDbaseSET 	= pDbase->pSystemSETData;
	pMapSET 	= pDbase->tMapData.pSystemSETData;
	num 		= pDbase->nSystemSET;
	
	if((pDbaseSET == NULL)||(pMapSET == NULL))
		return DB_ERR_BASENULL;
	
	// 1.��RAM�ļ������лָ�
	if(!DB_CheckSETData(pMapSET,num))
	{
		// 2.��EEPROM�ļ��лָ�
		if(DB_UpdateRelayDataMap() != DB_ERR_OK)
			return DB_ERR_FILE_CHECK;
	}
	
	// 2 ���¼��
	if(!DB_CheckSETData(pMapSET,num))
	{
		DAE_SetRunningFlags(SS_EEPROM_SYSSET_ERR);
		return DB_ERR_CHECK;
	}
	
	// 3 ˢ������

	// 3.1 �ֽ�ת��
	for(i=0;i<num;i++)
		pDbaseSET[i] = CONVERTFLOAT(pMapSET[i]);
	
	// 3.2 ���¼���У��
	DAE_MakeCheckSum((BYTE *)pMapSET,sizeof(float)*num);

	// 4.������Ϣ����¼����
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, SET_VALUE_ALL, 0, 0);

	DB_Register_SET(SET_AREANO_INVALID,SET_VALUE_ALL);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ����¶��㶨ֵ
	������NONE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateSETFixDataBase()
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	float 					*pSETfloat;
	long 					*pSETFix;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	pSETfloat = pDbase->pSETDataFloat;
	pSETFix = pDbase->pSETDataFix;
	num = pDbase->nSET;

	if((pSETfloat == NULL)||(pSETFix == NULL))
		return DB_ERR_BASENULL;
	
	//����С��λ����ʼ�����㶨ֵ
	for(i=0;i<num;i++)
	{
		//����������
		ConvertSETFloat2Fix(i,pSETfloat[i],&pSETFix[i]);
	}
	
	// ���¼���У��
	DAE_MakeCheckSum((BYTE *)pSETFix,sizeof(float)*num);
	
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(SET_VALUE_ALL,SET_AREA_CURRENT), 0, 0);

	DB_Register_SET(DB_GetCurrentSETAreaNo(),SET_VALUE_ALL);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����ϵ�����ݡ�
	����: NONE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateCFBase()
{
	WORD 					num,i;
	long					*pData;
	long 					*pCF;
	STATUS					rc;
	
	num = G_pCfgTable->DBaseConfig.nCF;
	pCF = G_pCfgTable->DBaseConfig.pCFData;
	pData = G_pCfgTable->DBaseConfig.tModData.pCFData;
	
	//�������ָ��ǿգ�ѡ����ʵ�ϵ����
	if(pData != NULL)
	{
		if(DB_Is1ASystem())
			rc = EEPROM_Read(EEPROM_RELAY_CFBASE+(CF_COUNT+1)*sizeof(long),(CF_COUNT+1)*sizeof(long),pData);
		else
			rc = EEPROM_Read(EEPROM_RELAY_CFBASE,(CF_COUNT+1)*sizeof(long),pData);

		if(rc != DB_ERR_OK)
		{
			LogError("UpdateCFBase",FILE_LINE,"Read eerpom data Error!!!");
			pData = NULL;
		}
	}

	// ���¼��
	if(!DB_CheckCFData(pData,num))
	{
		LogError("UpdateCFBase",FILE_LINE,"CF data Error!!!");
		DAE_SetRunningFlags(SS_EEPROM_CF_ERR);
		pData = NULL;
	}
	else
		DAE_ClearRunningFlags(SS_EEPROM_CF_ERR);
		
	//�ټ��ϵ����Ч��

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pData == NULL)
		{
			if(DB_Is1ASystem())
				pCF[i] = CFDefaultVal2[i]; 
			else
				pCF[i] = CFDefaultVal1[i]; 
		}
		else
		{
			//�Ƚ�������ת��
			pCF[i] = pData[i];
		}
	}
	
	// ����У��
	DAE_MakeCheckSum((BYTE *)pCF,sizeof(float)*num);

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_CF, CF_VALUE_ALL, 0, 0);

	DB_Register_CF(CF_VALUE_ALL);

	// �ٸ��¶��㶨ֵ
	DB_UpdateSETFixDataBase();
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�������ѹ��
	����: NONE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_UpdateSWBase()
{
	WORD 					num,i;
	DWORD					*pData;
	DWORD 					*pSW;
	
	num = G_pCfgTable->DBaseConfig.nSW;
	pSW = G_pCfgTable->DBaseConfig.pSWData;
	pData = G_pCfgTable->DBaseConfig.tMapData.pSWData;

	// 1.��RAM�ļ������лָ�
	if(!DB_CheckSWData(pData,num))
	{
		// 2.��EEPROM�ļ��лָ�
		if(DB_UpdateRelayDataMap() != DB_ERR_OK)
			return DB_ERR_FILE_CHECK;
	}
	
	//�ټ��ϵ����Ч��
	if(!DB_CheckSWData(pData,num))
	{
		LogError("DB_UpdateSWBase",FILE_LINE,"SW data Error!!!");
		DAE_SetRunningFlags(SS_EEPROM_SW_ERR);
		return DB_ERR_CHECK;
	}
	

	/*��ʼ����������*/
	for(i=0;i<((num+31)/32)*2;i++)
		pSW[i] = ltobl(pData[i]);
	
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, 0xFFFF, 0, 0);
	
	DB_Register_SW(0xFFFF,0);
	
	return DB_ERR_OK;
}

STATUS DB_UpdateRelayData()
{
	//�����������ļ����ݴ�FLASH�ļ�����EEPROM
	TFileHead			*filehead;
	BYTE				*pParaBuf = NULL;
	DWORD				dwFileLength,dwRtnLength;
	STATUS				rc;
	
	// 1 ��ȡ�ļ�
#ifdef _RELAY_DATA_FILE_
	rc = DAE_GetFlashFileLength(RELAYDATAFILE,&dwFileLength);
	if( rc == DB_ERR_OK)
	{
		if(dwFileLength > MAX_FILE_LEN)
			dwFileLength = MAX_FILE_LEN;
		
		//������ʱ�õ��ļ�������
		pParaBuf = (BYTE *)OS_MemAlloc(dwFileLength);
		
		if( pParaBuf != NULL )
		{
			memset(pParaBuf,0,dwFileLength);
			rc = DAE_ReadFlashFile(RELAYDATAFILE,pParaBuf,dwFileLength);
		}
		else
		{
			LogError("UpdateRelayData",FILE_LINE,"Allocate memory for read parameter file error!");
			rc = DB_ERR_MEM_ALLOC;
		}
	}
	
	// 2 д��EEPROM
	if(rc != DB_ERR_OK)
	{
		LogError("UpdateRelayData",FILE_LINE,"Read Relay Data File ERR!!!");		
		OS_MemFree(pParaBuf);
		return rc;
	}

	rc = DAE_CheckSystemFile(pParaBuf,dwFileLength);
	
	if(rc == DB_ERR_OK)	
	{
		rc = EEPROM_Write(EEPROM_RELAY_DATA,dwFileLength,pParaBuf);
		
		if(rc == DB_ERR_OK)
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File OK!!!");
		else
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data write EEPROM error!!!");
	}
	else
	{
		LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File check ERROR!!!");
	}
	
	OS_MemFree(pParaBuf);

#else
	dwFileLength = 0;
	pParaBuf = (BYTE *)RelayDataInit;
	
	DAE_GetFileLength(RELAYDATAFILE, &dwFileLength);
	if(dwFileLength != MAKEDWORD(pParaBuf[3],pParaBuf[2],pParaBuf[1],pParaBuf[0]))
	{
		LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File length ERROR!!!");
		rc = DB_ERR_FILE_CHECK;
	}
	
	rc = DAE_CheckSystemFile(pParaBuf,dwFileLength);
	
	if(rc == DB_ERR_OK)	
	{
		rc = EEPROM_Write(EEPROM_RELAY_DATA,dwFileLength,pParaBuf);
		
		if(rc == DB_ERR_OK)
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File OK!!!");
		else
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data write EEPROM error!!!");
	}
	else
	{
		LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File check ERROR!!!");
	}
#endif

	return rc;
}

STATUS DB_UpdateCFData()
{
	//�����������ļ����ݴ�FLASH�ļ�����EEPROM
	TFileHead			*filehead;
    TRelayDataTable		*pRelayDataTable;
	BYTE				*pParaBuf = NULL;
	DWORD				dwFileLength,dwRtnLength;
	DWORD				dwCFPos,dwCFLen;
	STATUS				rc = DB_ERR_FAILURE;
	
	// 1 ��ȡ�ļ�����
	do
	{
#ifdef _RELAY_DATA_FILE_
		rc = DAE_GetFileLength(RELAYDATAFILE,&dwFileLength);
		if( rc != DB_ERR_OK)
		{
			ReportMsg("Read Relay Data File fail!!!");
			break;
		}
		
		if(dwFileLength > EEPROM_RELAY_DATASIZE)
		{
			ReportMsg("Relay Data File length overflow!!!");
			break;
		}
			
		// 2.������ʱ�õ��ļ�������
		pParaBuf = (BYTE *)OS_MemAlloc(dwFileLength);
			
		if( pParaBuf == NULL )
		{
			ReportMsg("Allocate memory for read parameter file fail!");
			break;
		}
		
		// 3 ��ȡ�ļ�����
		rc = DAE_ReadFlashFile(RELAYDATAFILE,pParaBuf,dwFileLength);
		if(rc != DB_ERR_OK)
		{
			ReportMsg("Read Relay Data File fail!!!");
			break;
		}

#else
		dwFileLength = 0;
		pParaBuf = (BYTE *)RelayDataInit;
				
		DAE_GetFileLength(RELAYDATAFILE, &dwFileLength);
		if(dwFileLength != MAKEDWORD(pParaBuf[3],pParaBuf[2],pParaBuf[1],pParaBuf[0]))
		{
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File length ERROR!!!");
			rc = DB_ERR_FILE_CHECK;
		}
#endif
		
		// 3 ����ļ��Ϸ���
		rc = DAE_CheckSystemFile(pParaBuf,dwFileLength);
		if(rc != DB_ERR_OK) 
		{
			ReportMsg("Check Relay Data File fail!!!");
			break;
		}
		
		// 4 д��EEPROM
		filehead = (TFileHead *)pParaBuf;
		pRelayDataTable = (TRelayDataTable *)( filehead + 1 );
		
		dwCFPos  = sizeof(TFileHead)+sizeof(TRelayDataTable);
		dwCFPos += (pRelayDataTable->nSW+31)/32*2*sizeof(DWORD);
		dwCFPos += (pRelayDataTable->nSET+1)*pRelayDataTable->nSETArea*sizeof(float);
		dwCFPos += (pRelayDataTable->nSystemSET+1)*sizeof(float);
		dwCFLen = (pRelayDataTable->nCF+1)*sizeof(long)*2;
		
		//	4.1 ϵ�����ȼ��
		if(dwCFLen >= EEPROM_RELAY_CFSIZE)
		{
			ReportMsg("Relay file data fail!!!");
			rc = DB_ERR_FAILURE;
			break;
		}
		
		// 4.2 ϵ��У����
		if(!DAE_IsCheckSumOK(pParaBuf+dwCFPos, pRelayDataTable->nCF*sizeof(long)))
		{
			ReportMsg("Relay file data fail!!!");
			rc = DB_ERR_FAILURE;
			break;
		}
		
		if(!DAE_IsCheckSumOK(pParaBuf+dwCFPos+(pRelayDataTable->nCF+1)*sizeof(long), pRelayDataTable->nCF*sizeof(long)))
		{
			ReportMsg("Relay file data fail!!!");
			rc = DB_ERR_FAILURE;
			break;
		}
		
		rc = EEPROM_Write(EEPROM_RELAY_CFBASE,dwCFLen,pParaBuf+dwCFPos);
		
		if(rc == DB_ERR_OK)
			ReportMsg("Update CF data success!!!");
		else
			ReportMsg("Update CF data write eeprom error!!!");
	}while(0);
	
	//	�ͷ���ʱ�ռ�
#ifdef _RELAY_DATA_FILE_
	if( pParaBuf != NULL )
		OS_MemFree(pParaBuf);
#endif

	return rc;
}


