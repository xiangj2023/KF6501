/********************************************************************************************************/
/*																										*/
/*        						��򿭷������ɷ����޹�˾												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      Dbfunc.h                                     1.0												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ��������ݿ�����صĽӿں���																*/
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
/*	DB_GetAINum()					��ȡң�����														*/
/*	DB_ReadAI()						��ң�����ȡһ��ң������											*/
/*	DB_WriteAI()					��ң�����дһ��ң������											*/
/*	DB_GetDispOfAI()				��ȡһ��ң�����������												*/
/*	DB_GetBINum()					��ȡң�Ÿ���														*/
/*	DB_ReadBI()						��ң�ſ���ȡһ��ң������											*/
/*	DB_WriteBI()					��ң�ſ���дһ��ң������											*/
/*	DB_GetCINum()					��ȡ��ȸ���														*/
/*	DB_ReadCI()						�ӵ�ȿ���ȡһ���������											*/
/*	DB_WriteCI()					���ȿ���дһ���������											*/
/* 	DB_GetBONum()					��ȡң�ظ���														*/
/*	DB_BOSelect()					ң��Ԥ�ò���														*/
/*	DB_BOSelectEcho()				ң��Ԥ����Ӧ														*/
/*	DB_BOExec()						ң��ִ�в���														*/
/*	DB_BOExecEcho()					ң��ִ����Ӧ														*/
/*	DB_BODirect()					ң��ֱ��ִ�в���													*/
/*	DB_BODirectEcho()				ң��ֱ��ִ����Ӧ													*/
/*	DB_AOSelcet()					ң��Ԥ�ò���														*/
/*	DB_AOSelectEcho()				ң��Ԥ����Ӧ														*/
/*	DB_AOExec()						ң��ִ�в���														*/
/*	DB_AOExecEcho()					ң��ִ����Ӧ														*/
/*	DB_AODirect()					ң��ֱ��ִ�в���													*/
/*	DB_AODirectEcho()				ң��ֱ��ִ����Ӧ													*/
/*	DB_GetBIENum()					��ȡδ��ȡ��λң�����ݸ���											*/
/*	DB_BIEIsEmpty()					��λң�Ż������Ƿ�Ϊ��												*/
/*	DB_ReadBIE()					�����ݿ��ж�ȡһ����λң������										*/
/*	DB_WriteBIE()					�����ݿ���д��һ����λң������										*/
/*	DB_GetBIEHead()					ȡ��λң�Ż�����ͷָ��												*/
/*	DB_GetBIERead()					ȡ��λң�Ż�������ָ��												*/
/*	DB_IncBIERead()					������λң�Ż�������ָ��											*/
/*	DB_GetSOENum()					��ȡδ��ȡSOE���ݸ���												*/
/*	DB_SOEIsEmpty()					SOE�������Ƿ�Ϊ��													*/
/*	DB_ReadSOE()					�����ݿ��ж�ȡһ��SOE����											*/
/*	DB_WriteSOE()					�����ݿ���д��һ��SOE����											*/
/*	DB_GetSOEHead()					ȡSOE������ͷָ��													*/
/*	DB_GetSOERead()					ȡSOE��������ָ��													*/
/*	DB_IncSOERead()					����SOE��������ָ��													*/
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

extern TBasicConfig	G_tBasicConfig;
extern TSysConfigTable	*G_pCfgTable;
extern HANDLE			g_hBOTaskID;
extern HANDLE			g_hLCDTaskID;
extern HANDLE			g_hRelayTaskID;

static CLink	G_BORegisterList;		//ң��ע���

/************************************************************
	���ܣ�ע��һ·ң�أ�������վ���Լ�ӹ�ң��
	������nBO,		--  ң�����
	���أ���ȷ����DB_ERR_OK�����󷵻ش���ֵ��
************************************************************/
STATUS DB_RegisterBOMsg(WORD nBO)
{
	CNode	*pNode;
	TBORegisterInfo	*pBO;

	// 1.2.1 ����ռ�
	pBO = (TBORegisterInfo *)OS_MemAlloc(sizeof(TBORegisterInfo));
	ASSERT(pBO != NULL);
	if(pBO == NULL)
	{
		LogError("RegisterBO",FILE_LINE,"Malloc BO register node error!!!");
		return DB_ERR_MEM_ALLOC;
	}

	if(nBO >= DB_GetBONum())
	{
		LogError("RegisterBO",FILE_LINE,"Bo no is overflow!!!");
		return DB_ERR_MEM_ALLOC;
	}
		
	// 1.2.2 ��ʼ�����ݽڵ�
    pBO->hTaskID = RtosGetCurrentTaskId();
	pBO->nBO = nBO;
	
	// 1.2.3 ����
	pNode = (CNode *)OS_MemAlloc(sizeof(CNode));
	pNode->pData = pBO;
	G_BORegisterList.AddTail(pNode);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����ң���Ƿ�ע��
	������nBO,		--  ң�����
	���أ���ȷ����ң��ע����Ϣ�����󷵻ؿ�ָ��
************************************************************/
TBORegisterInfo *DB_SearchRegisterBO(WORD nBO)
{
	TBORegisterInfo		*pBO;
    CNode 				*pTempNode;
	
	pTempNode = G_BORegisterList.GetHead();
	//�ж������Ƿ�Ϊ����,����Ϊ��������
	if(pTempNode == NULL)
		return NULL;

	do
    {
		pBO = (TBORegisterInfo	*)pTempNode->pData;
        if ( pBO->nBO == nBO)
        {
            return pBO;
        }

		//�����¸��ڵ�
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != G_BORegisterList.GetHead()));

	return NULL;
}

float _ConvertFloat(float fIn)
{
	BYTE	*pTemp,byTemp;
	
	pTemp = (BYTE *)&fIn;

	byTemp = pTemp[0];
	pTemp[0] = pTemp[3];
	pTemp[3] = byTemp;

	byTemp = pTemp[1];
	pTemp[1] = pTemp[2];
	pTemp[2] = byTemp;

	return *(float *)pTemp;
}

/*-----------------------------------------------------------------------------

            ���º������ڲ�����������ȡ�豸�и��������ݿ��ָ��

-----------------------------------------------------------------------------*/


TRunTimeAI	*DB_GetAIBase()
{
    return G_pCfgTable->DBaseConfig.pAI;
}
TRunTimeBI	*DB_GetBIBase()
{
    return G_pCfgTable->DBaseConfig.pBI;
}
TRunTimeCI	*DB_GetCIBase()
{
    return G_pCfgTable->DBaseConfig.pCI;
}
TRunTimeBO	*DB_GetBOBase()
{
    return G_pCfgTable->DBaseConfig.pBO;
}

/************************************************************
	���ܣ���ȡң�����
	��������
	���أ�ң�����
************************************************************/
WORD	DB_GetAINum()
{
	return G_pCfgTable->DBaseConfig.nAI;
}

/************************************************************
	���ܣ���ң�����ȡһ��ң������
	������no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadAI( WORD no, long	*retval)
{
	TRunTimeAI		*pAI;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pAI = G_pCfgTable->DBaseConfig.pAI;
	num = G_pCfgTable->DBaseConfig.nAI;
	
	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_ReadAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pAI==NULL )
    {
        LogError("DB_ReadAI",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }
	
	*retval = pAI[no].lValue;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�����ȡң�⸡�����ݡ�
	������no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadFloatAI( WORD no, float *retval )
{
	TRunTimeAI		*pAI;
	TCF_Disp		tCFDisp;
	long			lCFValue = 1;
	long			lCFRatio = 1;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pAI = G_pCfgTable->DBaseConfig.pAI;
	num = G_pCfgTable->DBaseConfig.nAI;
	
	// 1. ������
	if(no >= num)
	{
		LogError("DB_ReadFloatAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. ���ָ��
    if( pAI == NULL )
    {
        LogError("DB_ReadFloatAI",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }
	
	// 3. ��������ֵ���л���
	
	//ת����ֵΪ�����ʽ
	if((DB_GetCFofAI(no, &lCFValue) != DB_ERR_OK)||(lCFValue == 0))
	{
		lCFValue = 1;
		LogError("DB_ReadFloatAI",FILE_LINE,"InValid CF !!!");
	}
	
	if((DB_GetCFDispofAI(no, &tCFDisp) != DB_ERR_OK)||(tCFDisp.wCof == 0))
	{
		lCFRatio = 1;
		LogError("DB_ReadFloatAI",FILE_LINE,"InValid COF !!!");
	}
	else
		lCFRatio = tCFDisp.wCof;
	

	*retval = (float)(pAI[no].lValue*lCFRatio)/lCFValue;	
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�����ȡ���β�ң�����ݡ�
	������no,		--  ���
		  retval	--  ���ص�ֵ
			sUnit	  --  ��λ
			nDec	  --  С����λ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadSecondAI( WORD no, long *retval,char *sUnit, BYTE *nDec)
{
	TAIPara			tAIPara;
	float			fValue;
    STATUS			rc;
	
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	// 1. ��ȡ���β���ֵ������
	rc = DB_ReadFloatAI(no,&fValue);
	if( rc != DB_ERR_OK)
        return rc;
	
	// 2. ���ն���ֵ��С��λ�����涨����
	if(DB_GetAIPara(no,&tAIPara) != DB_ERR_OK)											
	{
        LogError("DB_ReadSecondAI",FILE_LINE,"The AI Type is Error!");
		return (long)(fValue*1000);
	}

	if(nDec != NULL)
		*nDec = tAIPara.nSecDec;

	if(sUnit != NULL)
	{
		memcpy(sUnit,tAIPara.strSecUnit,6);
		sUnit[5] = '\0';
	}
	
	switch(tAIPara.nSecDec)
	{
	case 0: *retval = (long)fValue; break;	
	case 1: *retval = (long)(fValue*10); break;	
	case 2: *retval = (long)(fValue*100); break;	
	case 3: *retval = (long)(fValue*1000); break;	
	case 4: *retval = (long)(fValue*10000); break;	
	default: *retval = (long)(fValue*100000); break;	
	}

	// ������ʱֱ�ӷ��ص�ǰֵ
	if( !(G_pCfgTable->DBaseConfig.pAI[no].wStatus & AI_STATUS_ONLINE) )
		*retval = G_pCfgTable->DBaseConfig.pAI[no].lValue;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�����ȡһ�β�ң�����ݡ�
	������no,		--  ���
		  retval	--  ���ص�ֵ
		  sUnit		--  ��λ
		  nDec		--	С����λ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadFirstAI( WORD no, long *retval, char *sUnit, BYTE *nDec)
{
	TAIPara			tAIPara;
	float			fAIValue,fAbsValue;
	float			fRTValue;
	long			lRate;
    STATUS			rc;
	
    if( retval == NULL )
        return DB_ERR_BUFNULL;
		
	// 1. ��ȡ���β���ֵ������
	rc = DB_ReadFloatAI(no,&fAIValue);
	if( rc != DB_ERR_OK)
        return rc;
	
	// 2����ȡ��������ֵ����
	if(DB_GetAIPara(no,&tAIPara) == DB_ERR_OK)											
	{
		// 3�����ϱ��
		rc = DB_GetRTValue(tAIPara.wRatioIndex, &fRTValue);
		if(rc == DB_ERR_OK)
			fAIValue *= fRTValue;
		else
	        LogError("DB_ReadFirstAI",FILE_LINE,"Read AI RT Error!");

		// 4.�ж���Χ
		if(fAIValue < 0)
			fAbsValue = fAIValue*(-1);
		else
			fAbsValue = fAIValue;
		
		if(fAbsValue < tAIPara.lDispLimit12)
		{
			if(nDec != NULL)
				*nDec = tAIPara.nFirDec1;
			lRate = 0;
			if(sUnit != NULL)
				memcpy(sUnit,tAIPara.strFirUnit1,6);
		}
		else if(fAbsValue < tAIPara.lDispLimit23)
		{
			if(nDec != NULL)
				*nDec = tAIPara.nFirDec2;
			lRate = tAIPara.nDataRate2; 
			if(sUnit != NULL)
				memcpy(sUnit,tAIPara.strFirUnit2,6);
		}
		else
		{
			if(nDec != NULL)
				*nDec = tAIPara.nFirDec3;
			lRate = tAIPara.nDataRate3; 
			if(sUnit != NULL)
				memcpy(sUnit,tAIPara.strFirUnit3,6);
		}	
	}
	else
	{
		//ȱʡ�Ŵ�1000����ʾ
        LogError("DB_ReadFirstAI",FILE_LINE,"Read AI para Error!");
		if(sUnit != NULL)
			sUnit[0] = '\0';
		
		if(nDec != NULL)
			*nDec = 3;
		lRate = 0;
	}

	if(sUnit != NULL)
		sUnit[5] = '\0';
	
	// 5. ����һ��ֵ��С��λ�����涨����
	switch(lRate)
	{
	case 1: fAbsValue /= 10; break;	
	case 2: fAbsValue /= 100; break;	
	case 3: fAbsValue /= 1000; break;	
	case 6: fAbsValue /= 1000000; break;	
	default:  break;	
	}

	switch(*nDec)
	{
	case 0: fAbsValue = fAbsValue; break;	
	case 1: fAbsValue = (fAbsValue*10+0.5); break;	
	case 2: fAbsValue = (fAbsValue*100+0.5); break;	
	case 3: fAbsValue = (fAbsValue*1000+0.5); break;	
	case 4: fAbsValue = (fAbsValue*10000+0.5); break;	
	default: fAbsValue = (fAbsValue*100000+0.5); break;	
	}

	//���ݷ��ŷ���
	if(fAIValue < 0)
		*retval = (long)fAbsValue*(-1);
	else
		*retval = (long)fAbsValue;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�����дһ��ң������
	������no,		--  ң������
		  value,	--  ң��ֵ
	     mode		--  д��ģʽ 	DB_WRITE_COMPEL��ǿ������ DB_WRITE_UNCOMPEL��ȡ��ǿ��������		
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_WriteAI(WORD no, long value, DWORD mode )
{
	TRunTimeAI		*pAI;
    WORD 			num;
               
	pAI = G_pCfgTable->DBaseConfig.pAI;
	num = G_pCfgTable->DBaseConfig.nAI;
	
	// ����ȡ����
	if(no >= num)
	{
		return DB_ERR_NO;
	}

    if( pAI==NULL )
    {
        LogError("DB_WriteAI",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		// ����������ģʽ��,����Զ��������־,������߱�־,��������
		pAI[no].wStatus &= ~AI_STATUS_ONLINE;
		pAI[no].wStatus |= AI_STATUS_MODIFY;
		pAI[no].lValue = value;

		DB_Register_SetData(MISC_CLASS_MODIAI, no, value);
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// ������ȡ������ģʽ��,�������߱�־,���Զ��������־
		pAI[no].wStatus |= AI_STATUS_ONLINE;
		pAI[no].wStatus &= ~AI_STATUS_MODIFY;
		pAI[no].lValue = value;
	}
	else if( pAI[no].wStatus & AI_STATUS_ONLINE )
	{
		pAI[no].lValue = value;
	}

	pAI[no].wStatus &= ~AI_STATUS_RESET;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡң�Ÿ���
	��������
	���أ�ң�Ÿ���
************************************************************/
WORD	DB_GetBINum()
{
	return G_pCfgTable->DBaseConfig.nBI;
}

/************************************************************
	���ܣ���ң�ſ���дһ��ң������
	������ 
	      no,		--  ң�ŵ����
		  value,	--  ң��ֵ		BI_OPEN BI_CLOSE
	     mode		--  д��ģʽ 	DB_COMPEL��ǿ������	DB_UNCOMPEL��ȡ��ǿ������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
/*
STATUS DB_WriteBI(WORD no, BYTE  value, DWORD mode)
{
	TRunTimeBI		*pBI;
    WORD 			num;
               
	pBI = G_pCfgTable->DBaseConfig.pBI;
	num = G_pCfgTable->DBaseConfig.nBI;
	
	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_WriteBI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI==NULL )
    {
        LogError("DB_WriteBI",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		TBurstBI tBIE;
		TEventBI tSOE;
		
		// ����������ģʽ��,����Զ��������־,������߱�־,��������
		pBI[no].bStatus &= ~BI_STATUS_ONLINE;
		pBI[no].bStatus |= BI_STATUS_MODIFY;

		// �����״̬
		if(pBI[no].bStatus&0x80)
			pBI[no].bStatus |= (0x01<<6);
		else
			pBI[no].bStatus &= ~(0x01<<6);

		// ������״̬
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= (0x01<<7);
		else
			pBI[no].bStatus &= ~(0x01<<7);

		tBIE.wNo = no;
		tBIE.bStatus = value;
		DB_WriteBIE(&tBIE);
		
		tSOE.wNo = no;
		tSOE.bStatus = value;
		ReadAbsTime(&tSOE.stTime);
		DB_WriteSOE(&tSOE);
		
		DB_Register_SetData(MISC_CLASS_MODIBI, no, value);
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// ������ȡ������ģʽ��,�������߱�־,���Զ��������־
		pBI[no].bStatus |= AI_STATUS_ONLINE;
		pBI[no].bStatus &= ~AI_STATUS_MODIFY;
		// �ָ���״̬
		if(pBI[no].bStatus&0x40)
			pBI[no].bStatus |= (0x01<<7);
		else
			pBI[no].bStatus &= ~(0x01<<7);
	}
	else if( pBI[no].bStatus & AI_STATUS_ONLINE )
	{
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;
	}

	pBI[no].bStatus &= ~BI_STATUS_RESET;
		
	return DB_ERR_OK;
}
*/
STATUS DB_WriteBI(WORD no, BYTE  value, DWORD mode)
{
	TRunTimeBI		*pBI;
    WORD 			num;
               
	pBI = G_pCfgTable->DBaseConfig.pBI;
	num = G_pCfgTable->DBaseConfig.nBI;
	
	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_WriteBI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI==NULL )
    {
        LogError("DB_WriteBI",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		TBurstBI tBIE;
		TEventBI tSOE;
		
		// ����������ģʽ��,����Զ��������־,������߱�־,��������
		pBI[no].bStatus &= ~BI_STATUS_ONLINE;
		pBI[no].bStatus |= BI_STATUS_MODIFY;
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;

		tBIE.wNo = no;
		tBIE.bStatus = value;
		DB_WriteBIE(&tBIE);
		
		tSOE.wNo = no;
		tSOE.bStatus = value;
		ReadAbsTime(&tSOE.stTime);
		DB_WriteSOE(&tSOE);
		
		DB_Register_SetData(MISC_CLASS_MODIBI, no, value);
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// ������ȡ������ģʽ��,�������߱�־,���Զ��������־
		pBI[no].bStatus |= AI_STATUS_ONLINE;
		pBI[no].bStatus &= ~AI_STATUS_MODIFY;
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;
	}
	else if( pBI[no].bStatus & AI_STATUS_ONLINE )
	{
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;
	}

	pBI[no].bStatus &= ~BI_STATUS_RESET;
		
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�ſ���ȡһ����Ʒ��������ң��
	������ 
	      no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadBIWithQuality(WORD no, BYTE *retval)
{
	TRunTimeBI		*pBI;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pBI = G_pCfgTable->DBaseConfig.pBI;
	num = G_pCfgTable->DBaseConfig.nBI;
	
	// ����ȡ����
	if(no >= num)
	{
		//LogError("DB_ReadBI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI==NULL )
    {
        LogError("DB_ReadBI",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	*retval = pBI[no].bStatus;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ң�ſ���ȡһ��ң��״̬
	������ 
	      no,		--  ���
		  retval	--  ���ص�ֵ BI_OPEN BI_CLOSE
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadBI(WORD no, BYTE *retval)
{
	DB_ReadBIWithQuality(no,retval);

	if((*retval)&0x80)
		*retval = DB_BI_CLOSE;
	else
		*retval = DB_BI_OPEN;

	return DB_ERR_OK;
}

/*------------------------------------------------------------------------------------------
	��  �ܣ����豸��ȡһ���ֽ�ң�ţ�С�ڵ���8��ң�ţ�����
	������ 
	 	dev,     	--  �豸��,û�ú�������������
	  	start,    	--  ��ʼ���
	 	retval   	--  ���ص�ֵ,
         mode      	--  ң���ֽ���ң�ŵ�����˳��
                         DB_BI_BYTE18:ң�Ŵ�D0�ŵ�D7
                         DB_BI_BYTE81:ң�Ŵ�D7�ŵ�D0
	���أ���ȷ����0�����󷵻ش���ֵ��
-------------------------------------------------------------------------------------------*/
STATUS DB_ReadBIByte(WORD start, BYTE *retval, WORD mode)
{
	STATUS		status;
    BYTE	 	temp;

    if( retval == NULL )
        return DB_ERR_BUFNULL;

	*retval = 0;
	
    for(int i=0; i<8; i++ )
    {
        if( mode == DB_BI_BYTE81)
            status = DB_ReadBI(start+i, &temp);
        else
            status = DB_ReadBI(start+7-i, &temp);

        if( status != DB_ERR_OK )
            return status;

		if(temp == DB_BI_CLOSE)
			*retval |= 0x01<<i;
		else
			*retval &= ~(0x01<<i);
    }
	
    return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ��ȸ���
	��������
	���أ���ȸ���
************************************************************/
WORD	DB_GetCINum()
{
	return G_pCfgTable->DBaseConfig.nCI;
}

/************************************************************
	���ܣ����ȿ����ۼ�һ���������
	������ 
	      no,		--  ������
		  value,	--  ֵ��������λС���ĵ������ֵ����:123.45��Ӧ������12345
	     mode		--  д��ģʽ 	DB_COMPEL��ǿ������	DB_UNCOMPEL��ȡ��ǿ������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_WriteCI(WORD no, DWORD value, DWORD mode )
{
	TRunTimeCI		*pCI;
	TCIPara			tPara;
	STATUS			rc;
    WORD 			num;
               
	pCI = G_pCfgTable->DBaseConfig.pCI;
	num = G_pCfgTable->DBaseConfig.nCI;

	// �����峣��
	rc = DB_GetCIPara(no,&tPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_WriteCI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCI==NULL )
    {
        LogError("DB_WriteCI",FILE_LINE,"The CI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		// ����������ģʽ��,����Զ��������־,������߱�־,��������
		pCI[no].bStatus &= ~CI_STATUS_ONLINE;
		pCI[no].bStatus |= CI_STATUS_MODIFY;
		
		if(tPara.wControl&CI_ATTR_PULSE)
			value = (DWORD)((QWORD)value*tPara.dwConst/100); 		//��Ⱦ��ȵ���Ϊ0.001��   ͬʱ�����������  yanxs 20140405
		
		pCI[no].dwValue = value;
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// ������ȡ������ģʽ��,�������߱�־,���Զ��������־
		pCI[no].bStatus |= CI_STATUS_ONLINE;
		pCI[no].bStatus &= ~CI_STATUS_MODIFY;
		
		if(tPara.wControl&CI_ATTR_PULSE)
			value = (DWORD)((QWORD)value*tPara.dwConst/100); 		//��Ⱦ��ȵ���Ϊ0.001��   ͬʱ�����������  yanxs 20140405
		
		pCI[no].dwValue = value;
	}
	else if( pCI[no].bStatus & AI_STATUS_ONLINE )
	{
		pCI[no].dwValue += value;
	}

	// ���99999.99
	if(tPara.wControl&CI_ATTR_PULSE)
	{
		// �������������Ҫ�����������ֵ
		pCI[no].dwValue %= (100000*tPara.dwConst);
	}
	else
	{
		// ���������������Ҫ����0.01ȡģ
		pCI[no].dwValue %= 10000000;
	}
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����һ����ȵ���
	������ 
	      no,		--  ������
		  value,	--  ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetCI(WORD no, DWORD value)
{
	DB_Register_SetData(MISC_CLASS_MODICI, no, value);
	return DB_WriteCI(no, value,DB_WRITE_UNCOMPEL);
}

/************************************************************
	���ܣ��ӵ�ȿ���ȡһ����ȵ���
	������ 
	      no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadCIRaw(WORD no, DWORD *retval )
{
	TRunTimeCI		*pCI;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pCI = G_pCfgTable->DBaseConfig.pCI;
	num = G_pCfgTable->DBaseConfig.nCI;
	
	// ����ȡ����
	if(no >= num)
	{
		LogError("DB_ReadCI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCI==NULL )
    {
        LogError("DB_ReadCI",FILE_LINE,"The CI base is null!");
        return DB_ERR_BASENULL;
    }
	
	*retval = pCI[no].dwValue;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ��ӵ�ȿ���ȡһ���������
	������ 
	      no,		--  ���
		  retval	--  ���ص�ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_ReadCI(WORD no, DWORD *retval )
{
	TCIPara	tPara;
	DWORD	dwValue;
	STATUS	rc;
	
	// �����峣��
	rc = DB_GetCIPara(no,&tPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	rc = DB_ReadCIRaw(no,&dwValue);
	if(rc != DB_ERR_OK)
		return rc;

	// �̶���λС��
	if(tPara.wControl&CI_ATTR_PULSE)
	{
		//  ���峣���ۼӵ�������������Ҫ������
		dwValue = (long long)dwValue*100/tPara.dwConst;
	}
	
	*retval = dwValue;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡң�ظ���
	��������
	���أ�ң�ظ���
************************************************************/
WORD	DB_GetBONum()
{
	return G_pCfgTable->DBaseConfig.nBO;
}

/************************************************************
	���ܣ���ȡңң������
	��������
	���أ�ң�ظ���
************************************************************/
WORD	DB_GetAONum()
{
	return G_pCfgTable->DBaseConfig.nAO;
}

/************************************************************
	���ܣ���ȡֱ�����ֲ��������
	��������
	���أ����ֲ��������
************************************************************/
WORD	DB_GetDINum()
{
	return G_pCfgTable->DBaseConfig.nDI;
}

/************************************************************
	���ܣ�ң��Ԥ�ò���
	������ switchno		-- ң�ص��
	       attr,     	-- ң������ BO_OPEN��BO_CLOSE�� BO_NULL
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	 DB_BOSelect(WORD  switchno, WORD  attr)
{
    TRunTimeBO	*pBase;
    TBOPara		*pAttr;
    WORD 		num, index;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;

/*
    index = DB_BLANK;

	// 1������ң�ؿ��غź����Բ��Ҷ�Ӧ��ң��
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/
    index = switchno;

	// 2����¼ң������
    pBase[index].dwAppID = RtosGetCurrentTaskId();

	// 3������ң�س�ʱ��ʱ
	pBase[index].wTimeOutCnt = BO_RUNTIME_TIMEOUT;
	
	// 4����¼���β���ң��ʱ��
	ReadAbsTime(&pBase[index].tLastExecTime);

	// 5����¼ң��״̬
	pBase[index].wStatus = BO_STATUS_SELECT;

	TBORegisterInfo *pRegBO = DB_SearchRegisterBO(switchno);

	// 6������ң����Ϣ��ע����ң�صĴ���>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(pRegBO != NULL)
	{
		LogError("DB_BOSelect",FILE_LINE,"Send BO %d message!!!",index);
		OS_PostMessage(pRegBO->hTaskID, SM_BOSELECT, index, attr, 0,0);
	}
	else	
	    OS_PostMessage(g_hBOTaskID, SM_BOSELECT, index, attr, 0,0);
	
    return DB_ERR_OK;
}

/************************************************************
	���ܣ�ң��Ԥ����Ӧ
	������index, 	-- ң�ص��
	          attr,     -- ң������ BO_OPEN��BO_CLOSE��BO_NULL
              result    -- ң�ط��Ͻ�� 0 -- ��ȷ������ -- ����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
DWORD DB_BOSelectEcho(WORD index,WORD attr,WORD result )
{
    TRunTimeBO		*pBase;
    TBOPara			*pAttr;
    WORD 			num;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;

/*
    index = DB_BLANK;

	// 1������ң�ؿ��غź����Բ��Ҷ�Ӧ��ң��
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/

	if(index >= num)
        return DB_ERR_NOSUPPORT;
		
	// 2������ң�س�ʱ��ʱ
	pBase[index].wTimeOutCnt = 0;
	
	// 3����¼ң��״̬
	pBase[index].wStatus = BO_STATUS_SELSUCCESS;

	// 4������ң�ط�У��Ϣ
    OS_PostMessage( pBase[index].dwAppID, SM_BOSELECTECHO, 0, MAKELONG( pAttr[index].wSwitchNo, attr ), result, 0 );

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ң��ִ�в���
	������ switchno,	-- ң�ص��
	          attr,		-- ң������ BO_OPEN�� BO_CLOSE��BO_NULL
              mode		-- ң�ط�ʽ 1 -- ң��ִ�У�2 -- ң�س���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
DWORD DB_BOExec(WORD switchno,WORD attr,WORD mode )
{
    TRunTimeBO	*pBase;
    TBOPara		*pAttr;
    WORD 		num, index;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;
/*
    index = DB_BLANK;

	// 1������ң�ؿ��غź����Բ��Ҷ�Ӧ��ң��
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/

	index = switchno;

	// 2����¼ң������
    pBase[index].dwAppID = RtosGetCurrentTaskId();

	// 3������ң�س�ʱ��ʱ
	pBase[index].wTimeOutCnt = BO_RUNTIME_TIMEOUT;
	
	// 4����¼���β���ң��ʱ��
	ReadAbsTime(&pBase[index].tLastExecTime);

	// 5����¼ң��״̬
	pBase[index].wStatus = BO_STATUS_EXE;

	// 6������ң����Ϣ
	TBORegisterInfo *pRegBO = DB_SearchRegisterBO(switchno);

	if(pRegBO != NULL)
	{
		LogError("DB_BOExec",FILE_LINE,"Send BO %d message!!!");
		OS_PostMessage(pRegBO->hTaskID, SM_BOEXEC, index, attr, mode,  0 );
	}
	else
		OS_PostMessage(g_hBOTaskID, SM_BOEXEC, index, attr, mode,  0 );

	// 7��ע��ң�ز����¼�
    DB_Register_BO(switchno, attr);
	
    return DB_ERR_OK;
}

/************************************************************
	���ܣ�ң��ִ����Ӧ
	������index,  	--   ʵ�ʿ�ң�غ�
			attr,		--   ң������ BO_OPEN��BO_CLOSE��BO_NULL
     		mode		-- 	ң�ط�ʽ1 -- ң��ִ�У�2 -- ң�س���
			result		--   ����ֵ 0 -- ��ȷ������ -- ����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
DWORD DB_BOExecEcho(WORD index,WORD attr,WORD mode,WORD result )
{
    TRunTimeBO		*pBase;
    TBOPara			*pAttr;
    WORD 			num;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;
/*
    index = DB_BLANK;

	// 1������ң�ؿ��غź����Բ��Ҷ�Ӧ��ң��
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/
	if(index >= num)
        return DB_ERR_NOSUPPORT;

	// 2������ң�س�ʱ��ʱ
	pBase[index].wTimeOutCnt = 0;
	
	// 3����¼ң��״̬
	pBase[index].wStatus = BO_STATUS_READY;

	// 4������ң�ط�У��Ϣ
    OS_PostMessage( pBase[index].dwAppID, SM_BOEXECECHO, 0, MAKELONG( pAttr[index].wSwitchNo, attr ), result, 0 );

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ң��ֱ��ִ�в���
	������ switchno,	-- ң�ص��
	       attr,		-- ң������ BO_OPEN��BO_CLOSE��BO_NULL
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
DWORD DB_BODirect(WORD switchno, WORD attr )
{
    TRunTimeBO	*pBase;
    TBOPara		*pAttr;
    WORD 		num, index;

    //Get BO Attribute Base
    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;

/*
    index = DB_BLANK;

	// 1������ң�ؿ��غź����Բ��Ҷ�Ӧ��ң��
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/

	index = switchno;

	// 2����¼ң������
    pBase[index].dwAppID = RtosGetCurrentTaskId();

	// 3������ң�س�ʱ��ʱ
	pBase[index].wTimeOutCnt = BO_RUNTIME_TIMEOUT;
	
	// 4����¼���β���ң��ʱ��
	ReadAbsTime(&pBase[index].tLastExecTime);

	// 5����¼ң��״̬
	pBase[index].wStatus = BO_STATUS_DIREXE;

	// 6������ң����Ϣ
	TBORegisterInfo *pRegBO = DB_SearchRegisterBO(switchno);

	if(pRegBO != NULL)
	{
		LogError("DB_BODirect",FILE_LINE,"Send BO %d message!!!");
		OS_PostMessage(pRegBO->hTaskID, SM_BODIRECT, index, attr , 0, 0 );
	}
	else
    	OS_PostMessage(g_hBOTaskID, SM_BODIRECT, index, attr , 0, 0 );

	// 7��ע��ң�ز����¼�
    DB_Register_BO(switchno, attr);
	
    return DB_ERR_OK;
}

/************************************************************
	���ܣ�ң��ֱ��ִ����Ӧ
	������ index,		-- ң�ص��
			attr,		--   ң������ BO_OPEN��BO_CLOSE��BO_NULL
     		mode		-- 	ң�ط�ʽ1 -- ң��ִ�У�2 -- ң�س���
			result		--   ����ֵ 0 -- ��ȷ������ -- ����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
DWORD DB_BODirectEcho(WORD index,WORD attr,WORD result )
{
    TRunTimeBO		*pBase;
    TBOPara			*pAttr;
    WORD 			num;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;
/*
    index = DB_BLANK;

	// 1������ң�ؿ��غź����Բ��Ҷ�Ӧ��ң��
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/
	if(index >= num)
        return DB_ERR_NOSUPPORT;

	// 2������ң�س�ʱ��ʱ
	pBase[index].wTimeOutCnt = 0;
	
	// 3����¼ң��״̬
	pBase[index].wStatus = BO_STATUS_READY;

	// 4������ң�ط�У��Ϣ
    OS_PostMessage( pBase[index].dwAppID, SM_BODIRECTECHO, 0, MAKELONG( pAttr[index].wSwitchNo, attr ), result, 0 );

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�������������
	������ no			-- ����·�ţ���ң��·�Ų�ͬ
	       par,     	-- ��������ʱ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS	 DB_BOTest(WORD  no, WORD  par)
{
	//Ŀǰ���ݿ������û�д���·���Ĳ�������Ҫ���Ӵ����Խ��б�Ҫ�ļ��
	
	// 6������ң����Ϣ��ע����ң�صĴ���>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    OS_PostMessage(g_hBOTaskID, SM_BOTEST, no, par, 0,0);
	
    return DB_ERR_OK;
}


/************************************************************
	���ܣ���ȡң������
	��������
	���أ�ң������
************************************************************/
//WORD	DB_GetAONum();

/************************************************************
	���ܣ�ң���������
	������ no		-- ң�����
	       value,	-- ң��ֵ 
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
//DWORD DB_AOSelect(WORD no,long value);

/************************************************************
	���ܣ�ң��Ԥ�÷�У
	������ no,     	--   ʵ�ʿ�ң����
		value   	--   ң��ֵ 
		result 		--   ����ֵ 0 -- ��ȷ�� 
								1 -- �����ң��·�Ŵ���
                                2 -- �����ң��ֵ���Ϸ�
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
//DWORD DB_AOSelectEcho(WORD no, long  value, WORD result );

/************************************************************
	���ܣ�ң��ִ�в���
	������ no,    	-- ң�����
	       value, 	-- ң��ֵ
            mode   	-- ִ�з�ʽ 1 -- ң��ִ�У�2 -- ң������
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
//DWORD DB_AOExec(WORD no, short value, WORD mode );

/************************************************************
	���ܣ�ң��ִ�з�У
	������ no,    		--   ʵ�ʿ�ң����
			value,		--   ң��ֵ   
           mode,  		--   ִ�з�ʽ	1 -- ң��ִ�У� 2 -- ң������
			result		--   ����ֵ		0 -- ��ȷ��		1 -- ����
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
//DWORD DB_AOExecEcho(WORD no, short value, WORD mode, WORD result );

/************************************************************
	���ܣ�ң��ֱ��ִ�в���
	������ no,     	-- ң�����
	       value,  	-- ң��ֵ 
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
//DWORD DB_AODirect(WORD no,short value);

/************************************************************
	���ܣ�ң��ֱ��ִ�з���
	������ no,    	--   ʵ�ʿ�ң����
			value  	--   ң��ֵ 
			result 	--   ����ֵ 0 -- ң��ִ�гɹ��� 1 -- ң��ִ��ʧ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
//DWORD DB_AODirectEcho(WORD no, short value, WORD result );

/************************************************************
	���ܣ���ȡδ��ȡ��λң�����ݸ���
	������*retval	��������ָ��
	���أ�δ��ȡ��λң�����ݸ���
************************************************************/
STATUS DB_GetBIENum(WORD *retval)
{
	TBurstBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3����ȡ��ָ��    
    rc = DB_GetBIERead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_BIE_NUM-rp)%MAX_BIE_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ���λң�Ż������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_BIEIsEmpty()
{
	TBurstBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return TRUE;

    // 2����ȡ��ָ��    
    rc = DB_GetBIERead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	���ܣ������ݿ��ж�ȡһ����λң������
	������	rp   	--  ��ȡ�����õ�ָ��,��BLANKʱ���ڲ���ָ��
			*event	--	���ص�����ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_ReadBIE(TBurstBI *event, DWORD rp )
{
    TBurstBIWrap 	*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nBIERead%MAX_BIE_NUM;
    }
    else
    {
		rp = rp%MAX_BIE_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*event = pWrap->pEvent[rp];
	
	// 5�������ȡ���ֱ����������־
   	DB_ClearEventFlag(pAppInfo,APP_EVENT_BIE);
                                     
	return DB_ERR_OK;
}

/************************************************************
	���ܣ������ݿ���д��һ����λң������
	������event, --  д������� 
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_WriteBIE(TBurstBI *pEvent )
{
    TBurstBIWrap 	*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1���������ָ��ĺϷ���
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3��������
	OS_Protect(0);
	
    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_BIE_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_BIE_NUM;

	OS_UnProtect(0);
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++, pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_BIE))
		{
			pAppInfo->nBIERead = pWrap->nTailIndex;
			continue;
		}
		
	    if( pAppInfo->nBIERead == pWrap->nTailIndex )
	         pAppInfo->nBIERead = (pWrap->nTailIndex+1)%MAX_BIE_NUM;

		//�����ݿ�����ͳһ������ֹѩ��ʱ���ܼ�ʱ����
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_BIE))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_BIE);
			
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_BIBURST, 0,0,0,0);
		}
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡ��λң�Ż�����ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetBIEHead(WORD *retval)
{
	TBurstBIWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_BIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡ��λң�Ż�����βָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetBIETail(WORD *retval)
{
	TBurstBIWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_BIE_NUM;

	return DB_ERR_OK;
}
		
/************************************************************
	���ܣ�ȡ��λң�Ż�������ָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetBIERead(WORD *retval)
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
    *retval = pAppInfo->nBIERead%MAX_BIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�������λң�Ż�������ָ��
	��������
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_IncBIERead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nBIERead = (pAppInfo->nBIERead+1)%MAX_BIE_NUM;
		
	return DB_ERR_OK;
}


/************************************************************
	���ܣ���ȡδ��ȡSOE���ݸ���
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetSOENum(WORD *retval)
{
	TEventBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3����ȡ��ָ��    
    rc = DB_GetSOERead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_SOE_NUM-rp)%MAX_SOE_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ�SOE�������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_SOEIsEmpty()
{
	TEventBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return TRUE;

    // 2����ȡ��ָ��    
    rc = DB_GetSOERead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	���ܣ������ݿ��ж�ȡһ��SOE����
	������rp		--  ��ȡ�����õ�ָ��,��BLANKʱ���ڲ���ָ��
	�����event		--  ���ص����� 
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_ReadSOE(TEventBI *event, DWORD rp )
{
    TEventBIWrap 	*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nSOERead%MAX_SOE_NUM;
    }
    else
    {
		rp = rp%MAX_SOE_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*event = pWrap->pEvent[rp];
	
	// 5�������ȡ������������־
    DB_ClearEventFlag(pAppInfo,APP_EVENT_SOE);
                                     
	return DB_ERR_OK;
}

/************************************************************
	���ܣ����豸��д��һ��SOE����
	������pEvent,	--  д������� 
 	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_WriteSOE( TEventBI *pEvent )
{
    TEventBIWrap 	*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1���������ָ��ĺϷ���
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3��������
	OS_Protect(0);
	
    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_SOE_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_SOE_NUM;

	OS_UnProtect(0);
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++, pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_SOE))
		{
			pAppInfo->nSOERead = pWrap->nTailIndex;
			continue;
		}
	    if( pAppInfo->nSOERead == pWrap->nTailIndex )
	         pAppInfo->nSOERead = (pWrap->nTailIndex+1)%MAX_SOE_NUM;
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_SOE))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_SOE);
			//�����ݿ�����ͳһ����Ϣ����ֹѩ��ʱ���ܼ�ʱ����
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_BIEVENT, 0,0,0,0);
		}
	}
	
	DB_RegisterFileChange(DB_FILE_BASICEV);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡSOE������ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetSOEHead(WORD *retval)
{
	TEventBIWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_SOE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡSOE������βָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetSOETail(WORD *retval)
{
	TEventBIWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_SOE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡSOE��������ָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetSOERead(WORD *retval)
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
    *retval = pAppInfo->nSOERead%MAX_SOE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����SOE��������ָ��
	��������
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_IncSOERead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nSOERead = (pAppInfo->nSOERead+1)%MAX_SOE_NUM;

	//DB_RegisterFileChange(DB_FILE_BASICEV);
		
	return DB_ERR_OK;
}

STATUS DB_RegisterFileChange(WORD wFileType)
{
	TFileMap				*pFileMap;

	// �����ļ�
	switch(wFileType)
	{
	case DB_FILE_PARACFG:
		pFileMap = G_pCfgTable->DBaseConfig.pBasicParaFileMap;
		break;
	case DB_FILE_RELAYDATA:
		pFileMap = G_pCfgTable->DBaseConfig.pRelayDataFileMap;
		break;
	case DB_FILE_RELAYEV:		
		pFileMap = G_pCfgTable->DBaseConfig.pRelayEventFileMap;
		break;
	case DB_FILE_BASICEV:
		pFileMap = G_pCfgTable->DBaseConfig.pBasicEventFileMap;
		break;
	default:
		return DB_ERR_FILE_TYPE;
	}
	
	if(pFileMap == NULL)
		return DB_ERR_MAP_NULL;

	// ����ʱ��
	ReadAbsTime(&pFileMap->absTime);

	// �ñ�־
	pFileMap->wFlag |= FILEMAP_FLAGS_MODIFY;

	return DB_ERR_OK;
}

STATUS DB_SaveBasicEventFile()
{	
    TFileHead         		*filehead;
	TFileMap				*pFileMap;
	TEventBI				*pEventBI;	
	TEventBIWrap			*pEventBIWrap;
//	TRelayStartEv			*pRSTT;
//	TRelayStartWrap 		*pRSTTWrap;
	TMiscEvent				*pMISC;	
	TMiscWrap				*pMiscWrap;
	TBasicEventPtrList		*pPtrList;
	
	pEventBIWrap 	= G_pCfgTable->DBaseConfig.pEventBIWrap;
//	pRSTTWrap		= G_pCfgTable->DBaseConfig.pRSTTWrap;
	pMiscWrap 		= &G_pCfgTable->MiscWrap;
	pFileMap 		= G_pCfgTable->DBaseConfig.pBasicEventFileMap;
	
	if(pFileMap == NULL)
		return DB_ERR_MAP_NULL;
	
	filehead = (TFileHead *)pFileMap->pRamData;
	pPtrList = (TBasicEventPtrList	*)(filehead+1);

	// 1�������ļ�����
	pEventBI = (TEventBI	 *)((BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TBasicEventPtrList));
	
	memcpy(pEventBI,pEventBIWrap->pEvent,sizeof(TEventBI)*MAX_SOE_NUM);
				
	pPtrList->nMaxSOE = pEventBIWrap->nMax;
	pPtrList->nSOET = pEventBIWrap->nTailIndex;
	pPtrList->nSOEH = pEventBIWrap->nHeadIndex;

	pMISC = (TMiscEvent *)((BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
		+sizeof(TEventBI)*MAX_SOE_NUM);
	
	memcpy(pMISC,pMiscWrap->pEvent,sizeof(TMiscEvent)*MAX_MISC_NUM);
	
	pPtrList->nMaxOptRecord = pMiscWrap->nMax;
	pPtrList->nOptRecordT = pMiscWrap->nTailIndex;
	pPtrList->nOptRecordH = pMiscWrap->nHeadIndex;

/*	
	pRSTT = (TRelayStartEv	*)((BYTE *)pFileMap->pRamData\
			+sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
			+sizeof(TEventBI)*MAX_SOE_NUM\
			+sizeof(TMiscEvent)*MAX_MISC_NUM);
	memcpy(pRSTT,pRSTTWrap->pEvent,sizeof(TRelayStartEv)*MAX_RSTT_NUM);
	
	pPtrList->nMaxRelaySart= pRSTTWrap->nMax;
	pPtrList->nRelayStartT = pRSTTWrap->nTailIndex;
	pPtrList->nRelayStartH = pRSTTWrap->nHeadIndex;
*/
	// 2�����¼����ļ�У��
	filehead->dwSize = sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
				+sizeof(TEventBI)*MAX_SOE_NUM\
				+sizeof(TMiscEvent)*MAX_MISC_NUM\
				+sizeof(WORD);
	filehead->dwSize = ltobl(filehead->dwSize);
	
	ReadSystemTime(&filehead->stTime);
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-2);
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-2);

	// 3��д���ļ�
	return DAE_WriteFile(BASICEVENTFILE, filehead,filehead->dwSize);
}

/************************************************************
	���ܣ�ȡ�û���¼����
	������*strPassWord	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetUserPassWord(char *strPassWord)
{
	memcpy(strPassWord,G_pCfgTable->DBaseConfig.chPassWord1,8);
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡ����Ա��¼����
	������*strPassWord	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetAdminPassWord(char *strPassWord)
{
	memcpy(strPassWord,G_pCfgTable->DBaseConfig.chPassWord2,8);
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡ�û���¼����
	������	strUserPassWord	�����û�����ָ��
			strAdminPassWord	���ع���Ա����ָ��;
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetAccessPassWord(char *strUserPassWord,char *strAdminPassWord)
{
	if(strUserPassWord)
		memcpy(strUserPassWord,G_pCfgTable->DBaseConfig.chPassWord1,8);
	
	if(strAdminPassWord)
		memcpy(strAdminPassWord,G_pCfgTable->DBaseConfig.chPassWord2,8);
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡ���Ա��
	������NONE
	���أ����Ա��
************************************************************/
WORD DB_GetLanguageNo()
{
	return G_pCfgTable->DBaseConfig.nLanguageNo;
}

/************************************************************
	���ܣ�ȡװ�õ�ַ
	������NONE
	���أ�װ�õ�ַ
************************************************************/
WORD DB_GetAddress()
{
	return G_pCfgTable->DBaseConfig.wAddress;
}

/************************************************************
	���ܣ���ȡ����A��IP����������
	������
			dwIP 		IP��ַ
			dwMask		��������
			dwGateWay	����
	���أ���ȡ���
************************************************************/
STATUS DB_GetEthAPara(DWORD &dwIP,DWORD &dwMask,DWORD &dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			dwGateWayIP = pEthernet->dwGateWay1;
			return DB_ERR_OK;
		}
	}

	return DB_ERR_FAILURE;
}

/************************************************************
	���ܣ���ȡ����B��IP����������
	������
			dwIP 		IP��ַ
			dwMask		��������
			dwGateWay	����
	���أ���ȡ���
************************************************************/
STATUS DB_GetEthBPara(DWORD &dwIP,DWORD &dwMask,DWORD &dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			dwGateWayIP = pEthernet->dwGateWay1;
			return DB_ERR_OK;
		}
	}

	return DB_ERR_FAILURE;
}

/************************************************************
	���ܣ��޸�װ�õ�ַ
	������wAddr װ�õ�ַ
	���أ��޸Ľ��
************************************************************/
STATUS DB_ModifyAddress(WORD wAddr)
{	
	G_pCfgTable->DBaseConfig.wAddress = wAddr;
	G_pCfgTable->DBaseConfig.tMapData.pConfig->wAddress = ltobs(wAddr);

	// ����EEPORM����
	G_tBasicConfig.wNetAddress = wAddr;
	DAE_UpdateBasicConfig();

	//���汣�������ļ�
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_ADDR);
}

/************************************************************
	���ܣ���ȡ����A��IP����������
	������
			dwIP 	IP��ַ
			wMask	��������
	���أ���ȡ���
************************************************************/
/*
STATUS DB_GetNetAIPMask(DWORD &dwIP,DWORD &dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			return DB_ERR_OK;
		}
	}

	dwIP = 0xC0A864c8;
	dwMask = 0xFFFFFF00;
	return DB_ERR_FAILURE;
}
*/
/************************************************************
	���ܣ���ȡ����B��IP����������
	������
			dwIP 	IP��ַ
			wMask	��������
	���أ���ȡ���
************************************************************/
/*
STATUS DB_GetNetBIPMask(DWORD &dwIP,DWORD &dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			return DB_ERR_OK;
		}
	}

	dwIP = 0xAC140A14;
	dwMask = 0xFFFFFF00;
	return DB_ERR_FAILURE;
}
*/

/************************************************************
	���ܣ��޸�����A��IP����������
	������
			dwIP 	IP��ַ
			wMask	��������
	���أ��޸Ľ��
************************************************************/
/*
STATUS DB_ModifyNetAIPMask(DWORD dwIP,DWORD dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			// 1. �޸����ݿ�����A��IP
			pEthernet->dwSelfIP = dwIP;
			pEthernet->dwSelfMask= dwMask;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. ͬ���޸��ļ�
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);

			// 3��ͬ���޸�EEPROM����
			G_tBasicConfig.dwNetAIP = dwIP;
			G_tBasicConfig.dwNetAMask = dwMask;
		}
	}
	DAE_UpdateBasicConfig();
	
	// 3��֪ͨ�����������³�ʼ������
	
	// 4����������ļ�
	return DB_ModifyParaFile(DB_PARA_ETH,1);
}
*/
/************************************************************
	���ܣ��޸�����B��IP����������
	������
			dwIP 	IP��ַ
			wMask	��������
	���أ��޸Ľ��
************************************************************/
/*
STATUS DB_ModifyNetBIPMask(DWORD dwIP,DWORD dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			// 1. �޸����ݿ�����A��IP
			pEthernet->dwSelfIP = dwIP;
			pEthernet->dwSelfMask= dwMask;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. ͬ���޸��ļ�
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);

			// 3��ͬ���޸�EEPROM����
			G_tBasicConfig.dwNetBIP = dwIP;
			G_tBasicConfig.dwNetBMask = dwMask;
		}
	}
	
	DAE_UpdateBasicConfig();
	// 3��֪ͨ�����������³�ʼ������
	
	// 4����������ļ�
	return DB_ModifyParaFile(DB_PARA_ETH,2);
}
*/
/************************************************************
	���ܣ��޸�����A��IP����������
	������
			dwIP 		IP��ַ
			dwMask		��������
			dwGateWay	����
	���أ��޸Ľ��
************************************************************/
STATUS DB_ModifyEthAPara(DWORD dwIP,DWORD dwMask,DWORD dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			// 1. �޸����ݿ�����A��IP
			pEthernet->dwSelfIP   = dwIP;
			pEthernet->dwSelfMask = dwMask;
			pEthernet->dwGateWay1 = dwGateWayIP;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. ͬ���޸��ļ�
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);
			pEthernet->dwGateWay1= ltobl(dwGateWayIP);
			
			// 3��ͬ���޸�EEPROM����
			G_tBasicConfig.dwNetAIP = dwIP;
			G_tBasicConfig.dwNetAMask = dwMask;
			G_tBasicConfig.dwNetAGWIP = dwGateWayIP;				
		}
	}
	
	// 4 ����EEPORM����
	DAE_UpdateBasicConfig();
	
	// 5��֪ͨ�����������³�ʼ������
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_MAPBIT), SM_NETCARDPARACHG, 0, 0, 0, 0);
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_ETHMAIN), SM_NETCARDPARACHG, 0, 0, 0, 0);
	
	// 6����������ļ�
	return DB_ModifyParaFile(DB_PARA_ETH,1);
}

/************************************************************
	���ܣ��޸�����B��IP����������
	������
			dwIP 		IP��ַ
			dwMask		��������
			dwGateWay	����
	���أ��޸Ľ��
************************************************************/
STATUS DB_ModifyEthBPara(DWORD dwIP,DWORD dwMask,DWORD dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			// 1. �޸����ݿ�����A��IP
			pEthernet->dwSelfIP = dwIP;
			pEthernet->dwSelfMask= dwMask;
			pEthernet->dwGateWay1 = dwGateWayIP;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. ͬ���޸��ļ�
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);
			pEthernet->dwGateWay1= ltobl(dwGateWayIP);

			// 3��ͬ���޸�EEPROM����
			G_tBasicConfig.dwNetBIP = dwIP;
			G_tBasicConfig.dwNetBMask = dwMask;
			G_tBasicConfig.dwNetBGWIP = dwGateWayIP;	
		}
	}
	
	// 4 ����EEPORM����
	DAE_UpdateBasicConfig();
	
	// 5��֪ͨ�����������³�ʼ������
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_MAPBIT), SM_NETCARDPARACHG, 0, 0, 0, 0);
	
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_ETHMAIN), SM_NETCARDPARACHG, 0, 0, 0, 0);
	
	// 6����������ļ�
	return DB_ModifyParaFile(DB_PARA_ETH,2);
}


/************************************************************
���ܣ����õ�ǰϵͳ���ϵ�����ʾģʽ 
������mode ��ʾģʽ	FD_SHOWMODE_FIRST/FD_SHOWMODE_SECOND
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS	DB_SetFDShowMode(BYTE mode)
{
	if(mode==FD_SHOWMODE_FIRST)
	{
		G_pCfgTable->DBaseConfig.dwControl &=  ~DB_RELAY_FDSECOND;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl &=  ~DB_RELAY_FDSECOND;
	}
	else
	{
		G_pCfgTable->DBaseConfig.dwControl |=  DB_RELAY_FDSECOND;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl |=  DB_RELAY_FDSECOND;
	}
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_FDSHOW);
}
/************************************************************
���ܣ����õ�ǰϵͳGPSʱ��Դ��ʽ
������flag��ΪTRUE,���ʾDK3511,����ΪKF6511
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS	DB_SetIRGBType(BOOL flag)
{
	if(flag)
	{
		G_pCfgTable->DBaseConfig.dwControl |=  DB_IRGB_DK3511;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl |=  DB_IRGB_DK3511;
	}
	else
	{
		G_pCfgTable->DBaseConfig.dwControl &=  ~DB_IRGB_DK3511;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl &=  ~DB_IRGB_DK3511;
	}
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_IRGBTYPE);
}

/************************************************************
���ܣ����õ�ǰϵͳ���Ա��
������no ���Ա��	
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS DB_SetLanguageNo(BYTE no)
{
	G_pCfgTable->DBaseConfig.nLanguageNo = no;
	G_pCfgTable->DBaseConfig.tMapData.pConfig->nLanguageNo = no;
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_LANGUAGE);
}

/************************************************************
���ܣ����õ�ǰϵͳ�û�����
������strPassWord ������	
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS DB_SetUserPassWord(const char *strPassWord)
{
	memcpy(G_pCfgTable->DBaseConfig.chPassWord1,strPassWord,8);
	memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord1,strPassWord,8);
	// ����EEPORM����
	memcpy(G_tBasicConfig.chPassWord1,strPassWord,8);
	DAE_UpdateBasicConfig();

	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_PASSWORD);
}

/************************************************************
���ܣ����õ�ǰϵͳ����Ա����
������strPassWord ������	
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS DB_SetAdminPassWord(const char *strPassWord)
{
	memcpy(G_pCfgTable->DBaseConfig.chPassWord2,strPassWord,8);
	memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord2,strPassWord,8);
	// ����EEPORM����
	memcpy(G_tBasicConfig.chPassWord2,strPassWord,8);
	DAE_UpdateBasicConfig();
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_PASSWORD);
}

/************************************************************
���ܣ����õ�ǰϵͳ����
������	strUserPassWord �û��������룻
		strAdminPassWord����Ա����	
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS DB_SetAccessPassWord(char *strUserPassWord,char *strAdminPassWord)
{
	if(strUserPassWord != NULL)
	{
		memcpy(G_pCfgTable->DBaseConfig.chPassWord1,strUserPassWord,8);
		memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord1,strUserPassWord,8);
		memcpy(G_tBasicConfig.chPassWord1,strUserPassWord,8);
	}

	if(strAdminPassWord != NULL)
	{
		memcpy(G_pCfgTable->DBaseConfig.chPassWord2,strAdminPassWord,8);
		memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord2,strAdminPassWord,8);
		memcpy(G_tBasicConfig.chPassWord2,strAdminPassWord,8);
	}

	// ����EEPORM����
	DAE_UpdateBasicConfig();

	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_PASSWORD);
}

/************************************************************
���ܣ����õ�ǰϵͳ�Ƿ������ֵ��
������mode: TRUE ����/FALSE ������
���أ�������ȷ���� DB_ERR_FILE_OK; ���򷵻ش������		
************************************************************/
STATUS	DB_SetCICaculate(BOOL mode)
{
	if(mode)
	{
		G_pCfgTable->DBaseConfig.dwControl |=  DB_RELAY_CACLINT;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl |=  DB_RELAY_CACLINT;
	}
	else
	{
		G_pCfgTable->DBaseConfig.dwControl &=  ~DB_RELAY_CACLINT;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl &=  ~DB_RELAY_CACLINT;
	}
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_CIOPT);
}

// �Ƿ����˿�
BOOL IsReservPort(WORD wPortID)
{	
	TEthPortPara		tEthernet;

	switch(MAJOR_PORT(wPortID))
	{
		case SC_PORT_SERIAL:
			// ��������һ���˿�
			if(MINOR_PORT(wPortID) < 2)
				return TRUE;
			break;
			
		case SC_PORT_ETHERNETA:
//		case SC_PORT_ETHERNETB:
			if(MINOR_PORT(wPortID) <= 2)
				return TRUE;
/*			
			for(int i=0;i<DB_GetEthLinkNum();i++)
			{
				DB_GetETHPara(i,&tEthernet);

				// �ҵ��˿�
				if(tEthernet.wPortID != wPortID)
					continue;

				// ������
				if(tEthernet.dwMode & ETHLINK_MODE_CLIENT)
					continue;

				// TCP
				if(tEthernet.dwMode & ETHLINK_MODE_UDP)
					continue;

				// �˿ں�701��702����ά����Shell��Լ
				if(MAJOR_PORT(wPortID) == SC_PORT_ETHERNETA)
				if((tEthernet.dwSelfPort == 701)||(tEthernet.dwSelfPort == 702))
					return TRUE;
			}
*/			
			break;
		default:
			break;
	}

	return FALSE;
}

/************************************************************
	���ܣ���ȡ���ڹ�Լ����
	������
			no 			�������
			pConfig		���ڹ�Լ����
	���أ���ȡ���
************************************************************/
STATUS DB_GetSerialConfig(WORD no,TSerialConfig *pConfig)
{
	TSerialPara 	tPara;
	STATUS			rc;

	rc = DB_GetSerialPara(no, &tPara);
	
	if(rc == DB_ERR_OK)
	{
		pConfig->byNo = no+1;
		pConfig->byBaud = tPara.byBaud;
		pConfig->byMode = tPara.bFlag;
		pConfig->byDriver = tPara.byDriver;
	}

	return rc;
}

/************************************************************
	���ܣ����ô��ڹ�Լ����
	������
			no 			�������
			pConfig		���ڹ�Լ����
	���أ���ȡ���
************************************************************/
STATUS DB_SetSerialConfig(WORD no,TSerialConfig *pConfig)
{
	TSerialPara 	tPara;
	STATUS			rc;
	
	// ת��Ϊ��0���	
	for(int i=0;i<DB_GetSerialNum();i++,pConfig++)
	{
		// ����д��ʱ
		if(no != DB_PARA_ALL)
			i = no-1;
		
		rc = DB_GetSerialPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

		if(!IsReservPort(tPara.wPortID))
		{
			tPara.byBaud = pConfig->byBaud;
			tPara.bFlag = pConfig->byMode;
			tPara.byDriver = pConfig->byDriver;
		}
		
		// 2����д����
		rc = DB_WriteSerialPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;
	
		// ����д��ʱ
		if(no != DB_PARA_ALL)
			break;
	}

	if(no != DB_PARA_ALL)
		return DB_WriteSerialParaConfirm(no-1);
	else
		return DB_WriteSerialParaConfirm(DB_PARA_ALL);
}

/************************************************************
	���ܣ���ȡCAN����Լ����
	������
			no 			CANBUS���
			pConfig		CANBUS��Լ����
	���أ���ȡ���
************************************************************/
STATUS DB_GetCanbusConfig(WORD no,TCanbusConfig *pConfig)
{
	TCanBusPara 	tPara;
	STATUS			rc;

	rc = DB_GetCANPara(no, &tPara);
	
	if(rc == DB_ERR_OK)
	{
		pConfig->byNo = no+1;
		pConfig->wBaud = tPara.wBaud;
		pConfig->byDriver = tPara.byDriver;
	}

	return rc;
}

/************************************************************
	���ܣ�����CAN����Լ����
	������
			no 			CANBUS���
			pConfig		CANBUS��Լ����
	���أ���ȡ���
************************************************************/
STATUS DB_SetCanbusConfig(WORD no,TCanbusConfig *pConfig)
{
	TCanBusPara 	tPara;
	STATUS			rc;
	
	// ת��Ϊ��0���
	for(int i=0;i<DB_GetCANNum();i++,pConfig++)
	{
		// ����д��ʱ
		if(no != DB_PARA_ALL)
			i = no-1;
		
		// 1���ȶ�����
		rc = DB_GetCANPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;
		
		if(!IsReservPort(tPara.wPortID))
		{
			tPara.wBaud = pConfig->wBaud;
			tPara.byDriver = pConfig->byDriver;
		}
		
		// 2����д����
		rc = DB_WriteCANPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

		// ����д��ʱ
		if(no != DB_PARA_ALL)
			break;
	}
		
	// 3��ȷ��֮��
	if(no != DB_PARA_ALL)
		rc = DB_WriteCANParaConfirm(no-1);
	else
		rc = DB_WriteCANParaConfirm(DB_PARA_ALL);
	return rc;
}

/************************************************************
	���ܣ���ȡ��̫�����Ӳ���
	������
			no 			��̫���������
			pConfig		��̫�����ӹ�Լ����
	���أ���ȡ���
************************************************************/
STATUS DB_GetEthLinkConfig(WORD no,TEthLinkConfig *pConfig)
{
	TEthPortPara	tPara;
	STATUS			rc;

	rc = DB_GetETHPara(no, &tPara);
	
	if(rc == DB_ERR_OK)
	{
		pConfig->byNetCardNo = MAJOR_PORT(tPara.wPortID)-SC_PORT_ETHERNETA+1;
		pConfig->byLinkNo = MINOR_PORT(tPara.wPortID);
		pConfig->byMode = tPara.dwMode;
		pConfig->byDriver = tPara.wDriver;
		pConfig->dwSideIP = tPara.dwSideIP;
		pConfig->wPort = tPara.dwSelfPort;
	}

	return rc;
}

/************************************************************
	���ܣ�������̫�����Ӳ���
	������
			no 			��̫���������
			pConfig		��̫�����ӹ�Լ����
	���أ���ȡ���
************************************************************/
STATUS DB_SetEthLinkConfig(WORD no,TEthLinkConfig *pConfig)
{
	TEthPortPara	tPara;
	STATUS			rc;
	
	// ת��Ϊ��0���
	for(int i=0;i<DB_GetEthLinkNum();i++,pConfig++)
	{
		// ����д��ʱ
		if(no != DB_PARA_ALL)
			i = no-1;
		
		// 1���ȶ�����
		rc = DB_GetETHPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

		//����֪���˿ڣ��޳�Һ����Shell��Լ
		if(!IsReservPort(tPara.wPortID))
		{
			tPara.dwMode = pConfig->byMode;
			tPara.wDriver = pConfig->byDriver;
			tPara.dwSideIP = pConfig->dwSideIP;
			tPara.dwSelfPort = pConfig->wPort;
			tPara.dwSidePort = pConfig->wPort;
		}
		
		// 2����д����
		rc = DB_WriteETHPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

	}
	
	// 3��ȷ��֮��
	if(no != DB_PARA_ALL)
		rc = DB_WriteETHParaConfirm(no-1);
	else
		rc = DB_WriteETHParaConfirm(DB_PARA_ALL);
	return rc;
}

/************************************************************
	���ܣ���ȡ���ݿ�汾��
	��������
	���أ��汾��
************************************************************/
STATUS DB_GetDataBaseVersion(BYTE *sVersion)
{	
	BYTE				*pFile;
    TFileHead         	*filehead;
	
	if(sVersion == NULL)
		return DB_ERR_PTRNULL;

	// ������0
	memset(sVersion,0,8);

	// paraconf.dat
	if(G_pCfgTable->DBaseConfig.pBasicParaFileMap != NULL)
	{
		pFile = (BYTE *)G_pCfgTable->DBaseConfig.pBasicParaFileMap->pRamData;
		filehead = (TFileHead  *)pFile;

		// �汾��
		sVersion[0] = LOBYTE(filehead->wFlag);
		sVersion[1] = HIBYTE(filehead->wFlag);
		sVersion[2] = LOBYTE(filehead->wVersion);
		sVersion[3] = HIBYTE(filehead->wVersion);

		// У����
		sVersion[4] = pFile[filehead->dwSize-2];
		sVersion[5] = pFile[filehead->dwSize-1];		
	}

	// relaydisp .dat
	if(G_pCfgTable->DBaseConfig.pRelayDispFileMap != NULL)
	{
		pFile = (BYTE *)G_pCfgTable->DBaseConfig.pRelayDispFileMap->pRamData;
		filehead = (TFileHead  *)pFile;

		// У����
		sVersion[6] = pFile[filehead->dwSize-2];
		sVersion[7] = pFile[filehead->dwSize-1];		
	}

	return DB_ERR_OK;
}

STATUS MakeRelayEventFile(void *pFileBuf);

/************************************************************
	���ܣ����װ���¼���
	������wMask �¼��������� 
	���أ���ȷ����DB_ERR_OK;���󷵻ش�����
************************************************************/
STATUS DB_ClearEvent(WORD wMask)
{
	TRelayActionWrap *pACTWrap;
	TRelayAlarmWrap *pALMWrap;
	TRelayStartWrap *pSTTWrap;
	TEventBIWrap 	*pSOEWrap;
	TBurstBIWrap 	*pBIEWrap;
	TMiscWrap		*pOPTWrap;
	TRecordWrap		*pLOGWrap;
	void	*pFileBuf;

	// 1 ���NvRAM

	// 1.1 ��������¼�
	if(wMask & (0x01<<DB_EVENT_RACT))
	{
		pACTWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
		pACTWrap->dwMagic = WRAP_INIT_FLAG;
		pACTWrap->nMax = MAX_RACT_NUM;
		pACTWrap->nHeadIndex = 0;
		pACTWrap->nTailIndex = 0;
		
		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nRACTRead = 0;
	}
	// 1.2 ����澯�¼�
	if(wMask & (0x01<<DB_EVENT_RALM))
	{
		pALMWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
		pALMWrap->dwMagic = WRAP_INIT_FLAG;
		pALMWrap->nMax = MAX_RALM_NUM;
		pALMWrap->nHeadIndex = 0;
		pALMWrap->nTailIndex = 0;
		
		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nRALMRead = 0;
	}
	
	// 1.3 ��������¼�
	if(wMask & (0x01<<DB_EVENT_RSTT))
	{
		pSTTWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
		pSTTWrap->dwMagic = WRAP_INIT_FLAG;
		pSTTWrap->nMax = MAX_RSTT_NUM;
		pSTTWrap->nHeadIndex = 0;
		pSTTWrap->nTailIndex = 0;

		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nRSTTRead = 0;
	}
	
	// 1.4 ���SOE�¼�
	if(wMask & (0x01<<DB_EVENT_SOE))
	{
		pSOEWrap = 	G_pCfgTable->DBaseConfig.pEventBIWrap;
		pSOEWrap->dwMagic = WRAP_INIT_FLAG;
		pSOEWrap->nMax = MAX_SOE_NUM;
		pSOEWrap->nHeadIndex = 0;
		pSOEWrap->nTailIndex = 0;

		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nSOERead = 0;
	}
	
	// 1.5 ���BIE�¼�
	if(wMask & (0x01<<DB_EVENT_BIE))
	{
		pBIEWrap = 	G_pCfgTable->DBaseConfig.pBurstBIWrap;
		pBIEWrap->dwMagic = WRAP_INIT_FLAG;
		pBIEWrap->nMax = MAX_BIE_NUM;
		pBIEWrap->nHeadIndex = 0;
		pBIEWrap->nTailIndex = 0;

		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nBIERead = 0;
	}
	
	// 1.6 ���������¼
	if(wMask & (0x01<<DB_EVENT_MISC))
	{
		pOPTWrap = 	&G_pCfgTable->MiscWrap;
		pOPTWrap->dwMagic = WRAP_INIT_FLAG;
		pOPTWrap->nMax = MAX_MISC_NUM;
		pOPTWrap->nHeadIndex = 0;
		pOPTWrap->nTailIndex = 0;

		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nMISCRead = 0;
	}

	// 1.7 ���LOG��Ϣ
	if(wMask & (0x01<<DB_EVENT_LOG))
	{
		pLOGWrap = 	&G_pCfgTable->RecordWrap;
		pLOGWrap->dwMagic = WRAP_INIT_FLAG;
		pLOGWrap->nMax = MAX_RUNREC_NUM;
		pLOGWrap->nHeadIndex = 0;
		pLOGWrap->nTailIndex = 0;

		// ������������ָ��
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nLogRead = 0;
	}
	
	// 2 ���EEPROM
	if(wMask & ((0x01<<DB_EVENT_RALM)|(0x01<<DB_EVENT_RACT)))
	{
		// 2.1  ָ����
		if(G_pCfgTable->DBaseConfig.pRelayEventFileMap ==  NULL)
			return DB_ERR_PTRNULL;

		pFileBuf = G_pCfgTable->DBaseConfig.pRelayEventFileMap->pRamData;
		if(pFileBuf ==  NULL)
			return DB_ERR_PTRNULL;
		
		// 2.2 ���������ļ�
		MakeRelayEventFile(pFileBuf);	
	}
	
	// 3 ���FLASH
	if(wMask & ((0x01<<DB_EVENT_SOE)|(0x01<<DB_EVENT_MISC)))
		FS_DelFile(BASICEVENTFILE,0);
	
	return DB_ERR_OK;
}

/************************************************************
���ܣ��������������ݴ�FLASH�ļ�����EEPROM
������NONE
���أ�NONE
************************************************************/
void DB_ResetBasicConfig()
{
	TEthPortPara			*pEthFile;
	TEthPortPara 			*pEthBase;
	WORD					wFilterTime[MAX_DI_NUM+1];
	
	// 1.���ļ��и���A B������
	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthFile = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
		pEthBase = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		// 1. �޸����ݿ�����A��IP
		pEthBase->dwSelfIP   = ltobl(pEthFile->dwSelfIP);
		pEthBase->dwSelfMask = ltobl(pEthFile->dwSelfMask);
		pEthBase->dwGateWay1 = ltobl(pEthFile->dwGateWay1);
						
		if(HIBYTE(pEthBase->wPortID) == SC_PORT_ETHERNETA)
		{
			// 3��ͬ���޸�EEPROM����
			G_tBasicConfig.dwNetAIP = ltobl(pEthFile->dwSelfIP);
			G_tBasicConfig.dwNetAMask = ltobl(pEthFile->dwSelfMask);
			G_tBasicConfig.dwNetAGWIP = ltobl(pEthFile->dwGateWay1);	
		}
		
		if(HIBYTE(pEthBase->wPortID) == SC_PORT_ETHERNETB)
		{
			// 3��ͬ���޸�EEPROM����
			G_tBasicConfig.dwNetBIP = ltobl(pEthFile->dwSelfIP);
			G_tBasicConfig.dwNetBMask = ltobl(pEthFile->dwSelfMask);
			G_tBasicConfig.dwNetBGWIP = ltobl(pEthFile->dwGateWay1);	
		}
	}

	// 2.���ļ��и��µ�ַ����
	G_pCfgTable->DBaseConfig.wAddress = ltobs(G_pCfgTable->DBaseConfig.tMapData.pConfig->wAddress);
	G_tBasicConfig.wNetAddress = G_pCfgTable->DBaseConfig.wAddress;
	memcpy(G_tBasicConfig.chPassWord1,G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord1,8);
	memcpy(G_tBasicConfig.chPassWord2,G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord2,8);

	DAE_UpdateBasicConfig();
	
	// 3.���ļ��и���ң��ȥ������
	for(int i=0;i<DB_GetDINum();i++)
	{
		G_pCfgTable->DBaseConfig.pDIPara[i].wFilterTime = ltobs(G_pCfgTable->DBaseConfig.tMapData.pDIAttr[i].wFilterTime);

		if(i<MAX_DI_NUM)
			wFilterTime[i] = G_pCfgTable->DBaseConfig.pDIPara[i].wFilterTime;
	}
	
	if(DB_GetDINum()<MAX_DI_NUM)
		DAE_MakeCheckSum((BYTE *)wFilterTime,sizeof(WORD)*DB_GetDINum());
	EEPROM_Write(EEPROM_DI_FILTERTIME,sizeof(WORD)*(MAX_DI_NUM+1),wFilterTime);

	LogError("DB_ResetBasicConfig",FILE_LINE,"System basic config is recover OK!!");
}

/************************************************************
	���ܣ���ȡδ��ȡAIE���ݸ���
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetAIENum(WORD *retval)
{
	TEventAIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3����ȡ��ָ��    
    rc = DB_GetAIERead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_AIE_NUM-rp)%MAX_AIE_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ�AIE�������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_AIEIsEmpty()
{
	TEventAIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return TRUE;

    // 2����ȡ��ָ��    
    rc = DB_GetAIERead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	���ܣ������ݿ��ж�ȡһ��AIE����
	������rp		--  ��ȡ�����õ�ָ��,��BLANKʱ���ڲ���ָ��
	�����event		--  ���ص����� 
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_ReadAIE(TEventAI *event, DWORD rp )
{
    TEventAIWrap 	*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nAIERead%MAX_AIE_NUM;
    }
    else
    {
		rp = rp%MAX_AIE_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*event = pWrap->pEvent[rp];
	
	// 5�������ȡ������������־
    DB_ClearEventFlag(pAppInfo,APP_EVENT_AIE);
                                     
	return DB_ERR_OK;
}

DWORD nTestAIEWriteCnt = 0;

/************************************************************
	���ܣ����豸��д��һ��AIE����
	������pEvent,	--  д������� 
 	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_WriteAIE( TEventAI *pEvent )
{
    TEventAIWrap 	*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1���������ָ��ĺϷ���
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;
	//���¼��е�ң���ԭʼֵֵת��Ϊʵ��ң��ֵ�����Ŵ���Ӧ�ı���
	TAIPara			tAIPara;	
	TCF_Disp		tCFDisp;
	long			lCFValue = 1;
	long			lCFRatio = 1;
	float 			value;
	if(DB_GetAIPara(pEvent->nNo,&tAIPara) == DB_ERR_OK)											
	{
		if((DB_GetCFofAI(pEvent->nNo, &lCFValue) != DB_ERR_OK)||(lCFValue == 0))
		{
			lCFValue = 1;
			LogError("DB_ReadFloatAI",FILE_LINE,"InValid CF !!!");
		}		
		if((DB_GetCFDispofAI(pEvent->nNo, &tCFDisp) != DB_ERR_OK)||(tCFDisp.wCof == 0))
		{
			lCFRatio = 1;
			LogError("DB_ReadFloatAI",FILE_LINE,"InValid COF !!!");
		}
		else
			lCFRatio = tCFDisp.wCof;
		
		value = (float)(pEvent->lValue*lCFRatio)/lCFValue;
		switch(tAIPara.nSecDec)
		{
		case 0: pEvent->lValue = (long)value; break;	
		case 1: pEvent->lValue = (long)(value*10); break; 
		case 2: pEvent->lValue = (long)(value*100); break;	
		case 3: pEvent->lValue = (long)(value*1000); break;	
		case 4: pEvent->lValue = (long)(value*10000); break;	
		default: pEvent->lValue = (long)(value*100000); break;	
		}
	}
	// 3��������
	OS_Protect(0);

    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_AIE_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_AIE_NUM;

	OS_UnProtect(0);
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++, pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_AIE))
		{
			pAppInfo->nAIERead = pWrap->nTailIndex;
			continue;
		}
	    if( pAppInfo->nAIERead == pWrap->nTailIndex )
	         pAppInfo->nAIERead = (pWrap->nTailIndex+1)%MAX_AIE_NUM;
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_AIE))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_AIE);
			//�����ݿ�����ͳһ����Ϣ����ֹѩ��ʱ���ܼ�ʱ����
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_AIEVENT, 0,0,0,0);
		}
	}

	nTestAIEWriteCnt++;
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡAIE������ͷָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetAIEHead(WORD *retval)
{
	TEventAIWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_AIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡAIE������βָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetAIETail(WORD *retval)
{
	TEventAIWrap	*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_AIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡAIE��������ָ��
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش���ֵ
************************************************************/
STATUS DB_GetAIERead(WORD *retval)
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
    *retval = pAppInfo->nAIERead%MAX_AIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�����AIE��������ָ��
	��������
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_IncAIERead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nAIERead = (pAppInfo->nAIERead+1)%MAX_AIE_NUM;

	//DB_RegisterFileChange(DB_FILE_BASICEV);
		
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ȡϵͳ������
	������NONE
	���أ�������
************************************************************/
DWORD DB_GetSystemControlWord()
{
	return G_pCfgTable->DBaseConfig.dwControl;
}

/************************************************************
	���ܣ��޸�ϵͳ������
	������dwCtrlWord ������
	���أ��޸Ľ��
************************************************************/
STATUS DB_SetSystemControlWord(DWORD dwCtrlWord)
{	
	G_pCfgTable->DBaseConfig.dwControl = dwCtrlWord;
	G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl = ltobl(dwCtrlWord);

	//���汣�������ļ�
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_ADDR);
}

