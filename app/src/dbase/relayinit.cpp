/********************************************************************************************************/
/*																										*/
/*        							��򿭷������ɷ����޹�˾											*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           													*/
/*																										*/
/*      relayinit.cpp                                     												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ������˱������ʼ������غ���																*/
/*																										*/
/* ����																									*/
/*																										*/
/*      ����																							*/
/*																										*/
/* ��������ݽṹ																						*/
/*      None																							*/
/*																										*/
/* ����																									*/
/*																										*/
/*      InitSWDispBase()			��ʼ����ѹ������													*/
/*      InitCFGDispBase()			��ʼ������������													*/
/*      InitSETDispBase()			��ʼ����ֵ����														*/
/*      InitSysSETDispBase()		��ʼ��������ֵ����													*/
/*      InitCFDispBase()			��ʼ��ϵ������														*/
/*      InitRMDispBase()			��ʼ����������ֵ����												*/
/*      InitREDispBase()			��ʼ��������������													*/
/*      InitSWDataBase()			��ʼ����ѹ������													*/
/*      InitCFGDataBase()			��ʼ������������													*/
/*      InitSETDataBase()			��ʼ����ֵ����														*/
/*      InitSysSETDataBase()		��ʼ��������ֵ����													*/
/*      InitCFDataBase()			��ʼ��ϵ������														*/
/*      InitRMDataBase()			��ʼ����������ֵ����												*/
/*      InitBIEWrap()				��ʼ��ң�ű�λ���ݿ�												*/
/*      InitSOEWrap()				��ʼ��SOE���ݿ�														*/
/*      InitRESWrap()				��ʼ�����������������ݿ�											*/
/*      InitREArap()				��ʼ�����������������ݿ�											*/
/*      InitRELrap()				��ʼ�������澯�������ݿ�											*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*      Dbfunc.h                  	���ݿ⺯������														*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0												*/
/********************************************************************************************************/
#include "daefunc.h"
#include "dbfunc.h"
#include "endian.h"
#include "Relaydef.h"
#include "initrelaydisp.h"
#include "initrelaydata.h"
#include "relay.h"

extern	TSysConfigTable	*G_pCfgTable;
extern 	WORD	g_wRelayDistNo;

CLink				G_RelayRepSaveLink;		//�������������������
CLink				G_RelayRepFreeLink;		//�������������������
CLink				G_DistDataSaveLink;		//����¼�����ݴ洢����
CLink				G_DistDataFreeLink;		//����¼�����ݿ�������
void				*G_pDistFileBlock;		//¼���ļ�����

void *GetDefaultRelayData(BYTE DataType)
{
	TFileHead         	*filehead;
    TRelayDataTable		*pRelayDataTable;
	TDBaseConfigTable	*pDbase = &G_pCfgTable->DBaseConfig;
	DWORD				*pSW,*pSWor;
	float				*pSET;
	float				*pSystemSET;
	long				*pCF;
	
	// 1.1 ��ȱʡ�ļ���ʼ��
	filehead = (TFileHead *)RelayDataInit;
	
	if( DAE_CheckSystemFile(RelayDataInit,filehead->dwSize) != DB_ERR_OK)
		return NULL;

	pRelayDataTable = (TRelayDataTable *)( filehead + 1 );

	pSW = (DWORD*)( pRelayDataTable + 1);
	pSWor = (DWORD*)( pSW + (pDbase->nSW+31)/32 );
	if(DataType == DB_DATA_SW)
	{
		if(DB_CheckSWData(pSW,pDbase->nSW))
		{
			LogError("GetDefaultRelayData",FILE_LINE,"From default SW data init......");
			return pSW;
		}
		else
			return NULL;
	}
	
	pSET = (float*)( pSWor + (pDbase->nSW+31)/32);		
	if(DataType == DB_DATA_SET)
	{
		if(DB_CheckSETData(pSET+pDbase->nSETAreaCur*(pDbase->nSET+1),pDbase->nSET))
		{
			LogError("GetDefaultRelayData",FILE_LINE,"From default SET data init......");
			return pSET;
		}
		else
			return NULL;
	}

	pSystemSET		= (float*)( pSET + (pDbase->nSET+1)*pDbase->nSETArea);
	if(DataType == DB_DATA_SYSSET)
	{
		if(DB_CheckSETData(pSystemSET,pDbase->nSystemSET))
		{
			LogError("GetDefaultRelayData",FILE_LINE,"From default SystemSET data init......");
			return pSystemSET;
		}
		else
			return NULL;
	}
	
	pCF = (long*)( pSystemSET+ pDbase->nSystemSET+1);	
	if(DataType == DB_DATA_CF)
	{
		if(DB_CheckCFData(pCF,pDbase->nCF))
		{
			LogError("GetDefaultRelayData",FILE_LINE,"From default CF data init......");
			return pCF;
		}
		else
			return NULL;
	}

	return NULL;
}

/*��ʼ����ѹ������*/
STATUS InitSWBase(DWORD *pData)
{
	WORD 					num,LongWordNum,i;
	DWORD 					*pSW;
	TDBaseConfigTable		*pDbase;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;
	num 	= pDbase->nSW;
	LongWordNum	= ((num+31)/32)*2;
	
	/*��������ռ�*/
	pSW = (DWORD *)OS_MemAlloc(sizeof(DWORD)*LongWordNum);
	ASSERT(pSW != NULL);
	if(pSW == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSWData = pSW;

	//�����Ч��
	if(pData != NULL)
	{
		if(!DB_CheckSWData(pData,num))
		{
			LogError("InitSWBase",FILE_LINE,"  EEPROM SW Error!!!");
			DAE_SetInitFlags(SS_EEPROM_SW_ERR);
			pData = (DWORD *)GetDefaultRelayData(DB_DATA_SW);
		//	pData = NULL;
		}
		else
		{
			DAE_ClearInitFlags(SS_EEPROM_SW_ERR);
		}
	}
	
	/*��ʼ����������*/
	if(pData == NULL)
	{
		memset(pSW,0x00,sizeof(DWORD)*LongWordNum/2);
		memset(pSW+LongWordNum/2,0xFF,sizeof(DWORD)*LongWordNum/2);
	}
	else
	{
		//�Ƚ�������ת��
		for(i=0;i<LongWordNum;i++)
			pSW[i] = ltobl(pData[i]);
	}
	
	//��ʼ������ָ��
	pDbase->pSWOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pSWOpt == NULL)
		return DB_ERR_MEM_ALLOC;
	
	return DB_ERR_OK;
}

/*��ʼ������������*/
/*
STATUS InitCFGBase(DWORD *pData)
{
	WORD 					num,LongWordNum,i;
	DWORD 					*pCFG;
	TDBaseConfigTable		*pDbase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nCFG;
	LongWordNum	= (num+31)/32;

	//��������ռ�
	pCFG = (DWORD *)pDbase->pSETDataFloat;
	ASSERT(pCFG != NULL);
	if(pCFG == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pCFGData = pCFG;

	//��ʼ����������
	if(pData == NULL)
	{
		//��ʼ��ȱʡ����
		memset(pCFG,0,sizeof(DWORD)*LongWordNum);
		DAE_SetInitFlags(SS_EEPROM_CFG_ERR);
	}
	else
	{
		//�Ƚ�������ת��
		for(i=0;i<LongWordNum;i++)
			pCFG[i] = ltobl(pData[i]);
	}

	//��ʼ������ָ��
	pDbase->pCFGOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pCFGOpt == NULL)
		return DB_ERR_MEM_ALLOC;
	
	return DB_ERR_OK;
}
*/

/*��ʼ����ֵ����*/
STATUS InitSETBase(float *pData)
{
	WORD 					num,i;
	float 					*pSETfloat;
	TDBaseConfigTable		*pDbase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nSET;

	/*��������ռ�*/
	pSETfloat = (float *)OS_MemAlloc(sizeof(float)*(num+1));
	ASSERT(pSETfloat != NULL);
	if(pSETfloat == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSETDataFloat = pSETfloat;
	
	//�ټ�鶨ֵ��Ч��
	//������ж�ֵ������ֻ��鵱ǰ��ֵ��?>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(pData != NULL)
	//for(i=0;i<pDbase->nSETArea;i++)
	{
		if(!DB_CheckSETData(pData+pDbase->nSETAreaCur*(num+1),num))
		{
			LogError("InitSETBase",FILE_LINE,"  EEPROM SET Error!!!");
			DAE_SetInitFlags(SS_EEPROM_SET_ERR);
			pData = (float *)GetDefaultRelayData(DB_DATA_SET);
	//		pData = NULL;
	//		break;
		}
		else
		{
			DAE_ClearInitFlags(SS_EEPROM_SET_ERR);
		}
	}
	
	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pData == NULL)
		{
			//�����ֳ�ʼ��Ϊ0
			if(i<(DB_GetCFGNum()+31)/32)
				*(DWORD *)&pSETfloat[i] = 0;//0xFFFFFFFF;
			else
			{
				pSETfloat[i] = 0.0;
/*				
				switch(i)
				{
				case 1:	pSETfloat[i] = 0.0; break;
				case 2:	pSETfloat[i] = 0.0; break;
				case 3: pSETfloat[i] = 0.0; break;
				case 4: pSETfloat[i] = 5.0; break;
				case 5: pSETfloat[i] = 0.0; break;
				}
*/
			}
		}
		else
		{
			//�Ƚ�������ת��
			pSETfloat[i] = CONVERTFLOAT(pData[i+pDbase->nSETAreaCur*(num+1)]);
		}
	}

	
	// ���¼���У��
	DAE_MakeCheckSum((BYTE *)pSETfloat,sizeof(float)*num);
	
	// ��ʼ������ָ��
	pDbase->pCFGOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pCFGOpt == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSETOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pSETOpt == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSETAreaOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pSETAreaOpt == NULL)
		return DB_ERR_MEM_ALLOC;

//	InitCFGBase((DWORD *)pData);
	
	return DB_ERR_OK;
}

/*��ʼ��������ֵ����*/
STATUS InitSystemSETBase(float *pData)
{
	WORD 					num,i;
	float 					*pSETfloat;
	TDBaseConfigTable		*pDbase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nSystemSET;

	/*��������ռ�*/
	pSETfloat = (float *)OS_MemAlloc(sizeof(float)*(num+1));
	ASSERT(pSETfloat != NULL);
	if(pSETfloat == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSystemSETData = pSETfloat;
	
	//�ټ�鶨ֵ��Ч��
	if(pData != NULL)
	{
		if(!DB_CheckSETData(pData,num))
		{
	    	LogError("InitSystemSETBase",FILE_LINE,"  EEPROM System SET Error!!!");
			DAE_SetInitFlags(SS_EEPROM_SYSSET_ERR);
			pData = (float *)GetDefaultRelayData(DB_DATA_SYSSET);
		//	pData = NULL;
		}
		else
		{
			DAE_ClearInitFlags(SS_EEPROM_SYSSET_ERR);
		}
	}	
	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pData == NULL)
		{
			pSETfloat[i] = 1.0;
		}
		else
		{
			//�Ƚ�������ת��
			pSETfloat[i] = CONVERTFLOAT(pData[i]);
		}
	}
	
	// ����У��
	DAE_MakeCheckSum((BYTE *)pSETfloat,sizeof(float)*num);
		
	return DB_ERR_OK;
}

/*��ʼ��ϵ������*/
STATUS InitCFBase(long *pData)
{
	WORD 					num,i;
	long 					*pCF;
	TDBaseConfigTable		*pDbase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nCF;

	/*��������ռ�*/
	pCF = (long *)OS_MemAlloc(sizeof(long)*(num+1));
	ASSERT(pCF != NULL);
	if(pCF == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pCFData = pCF;
	
	//�������ָ��ǿգ�ѡ����ʵ�ϵ����
	if(pData != NULL)
	{
		//ѡ��1Aϵ����
		if(DB_Is1ASystem())
			pData = pData+num+1;
	}

	//�ټ��ϵ����Ч��
	if(pData)
	{
		if(!DB_CheckCFData(pData,num))
		{
        	LogError("InitCFBase",FILE_LINE,"  EEPROM CF data Error!!!");
			DAE_SetInitFlags(SS_EEPROM_CF_ERR);
			pData = (long *)GetDefaultRelayData(DB_DATA_CF);
		}
		else
		{
			DAE_ClearInitFlags(SS_EEPROM_CF_ERR);
		}
	}
	
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
			/*
			switch(i)
			{
			case 0:	pCF[0] = 306; break;				
			case 1: pCF[1] = 306; break;
			case 2: pCF[2] = 306; break;
			case 3: pCF[3] = 115; break;
			case 4: pCF[4] = 115; break;
			case 5: pCF[5] = 115; break;
			case 6: pCF[6] = 6448; break;
			case 7: pCF[7] = 6448; break;
			case 8: pCF[8] = 6448; break;
			case 9: pCF[9] = 161; break;
			case 10: pCF[10] = 161; break;
			case 11: pCF[11] = 161; break;
			case 12: pCF[12] = 1299; break;
			case 13: pCF[13] = 1299; break;
			case 14: pCF[14] = 0; break;
			case 15: pCF[15] = 752; break;

			case 16: pCF[16] = 752; break;
			case 17: pCF[17] = 752; break;
			case 18: pCF[18] = 1000; break;

			case 19: pCF[19] = 100; break;
			case 20: pCF[20] = 10; break;
			case 21: pCF[21] = 1; break;

			}
			*/
		}
		else
		{
			//�Ƚ�������ת��
			pCF[i] = ltobl(pData[i]);
		}
	}
	
	// ����У��
	DAE_MakeCheckSum((BYTE *)pCF,sizeof(float)*num);
	
	//��ʼ������ָ��
	pDbase->pCFOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pCFOpt == NULL)
		return DB_ERR_MEM_ALLOC;
	
	return DB_ERR_OK;
}

/*��ʼ������������*/
STATUS InitCFGDisp(TCFG_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TCFG_Disp 				*pCFGDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nCFG;

	/*��������ռ�*/
	pCFGDisp = (TCFG_Disp *)OS_MemAlloc(sizeof(TCFG_Disp)*num);
	ASSERT(pCFGDisp != NULL);
	if(pCFGDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pCFGDisp = pCFGDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pCFGDisp->strCHName,"�����ٶ�");
				break;
			case 1:
				strcpy(pCFGDisp->strCHName,"����1��");
				break;
			}				
		}
		else
		{
			memcpy(pCFGDisp->strCHName,pPara->strCHName,20);
			pPara++;
		}

		pCFGDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ����ֵ����*/
STATUS InitSETDisp(TSET_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TSET_Disp 				*pSETDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nSET;

	/*��������ռ�*/
	pSETDisp = (TSET_Disp *)OS_MemAlloc(sizeof(TSET_Disp)*num);
	ASSERT(pSETDisp != NULL);
	if(pSETDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSETDisp = pSETDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:
				strcpy(pSETDisp->strCHName,"������1");
				strcpy(pSETDisp->strSymbol,"CFG1");
				pSETDisp->fMaxValue = 0.0;
				pSETDisp->fMinValue = 0.0;
				strcpy(pSETDisp->strUnit,"");
				pSETDisp->nCFG = 0;
				pSETDisp->nDec = 0;
				pSETDisp->nCFIndex = 1;
				break;
			case 1:	
				strcpy(pSETDisp->strCHName,"ͻ��������");
				strcpy(pSETDisp->strSymbol,"PIC");
				pSETDisp->fMaxValue = 10.000;
				pSETDisp->fMinValue = 0.0;
				strcpy(pSETDisp->strUnit,"A");
				pSETDisp->nCFG = 0;
				pSETDisp->nDec = 2;
				pSETDisp->nCFIndex = 1;
				break;
			case 2:
				strcpy(pSETDisp->strCHName,"�����ٶϵ���");
				strcpy(pSETDisp->strSymbol,"R50I0C");
				pSETDisp->fMaxValue = 10.000;
				pSETDisp->fMinValue = 0.0;
				strcpy(pSETDisp->strUnit,"A");
				pSETDisp->nCFG = 0;
				pSETDisp->nDec = 2;
				pSETDisp->nCFIndex = 1;
				break;
			case 3:
				strcpy(pSETDisp->strCHName,"�����ٶ�ʱ��");
				strcpy(pSETDisp->strSymbol,"R50I0T");
				pSETDisp->fMaxValue = 100.000;
				pSETDisp->fMinValue = 0.0;
				strcpy(pSETDisp->strUnit,"S");
				pSETDisp->nCFG = 0;
				pSETDisp->nDec = 2;
				pSETDisp->nCFIndex = 0;
				break;
			case 4:
				strcpy(pSETDisp->strCHName,"����1�ε���");
				strcpy(pSETDisp->strSymbol,"R50I1C");
				pSETDisp->fMaxValue = 60.000;
				pSETDisp->fMinValue = 0.0;
				strcpy(pSETDisp->strUnit,"A");
				pSETDisp->nCFG = 1;
				pSETDisp->nDec = 2;
				pSETDisp->nCFIndex = 1;
				break;
			case 5:
				strcpy(pSETDisp->strCHName,"����1��ʱ��");
				strcpy(pSETDisp->strSymbol,"R50I1T");
				pSETDisp->fMaxValue = 100.000;
				pSETDisp->fMinValue = 0.0;
				strcpy(pSETDisp->strUnit,"A");
				pSETDisp->nCFG = 1;
				pSETDisp->nDec = 2;
				pSETDisp->nCFIndex = 0;
				break;
			}	

			pSETDisp->nItemNum = 0;
			pSETDisp->nItemStart = 0;			
		}
		else
		{
			memcpy(pSETDisp->strCHName,pPara->strCHName,20);
			memcpy(pSETDisp->strSymbol,pPara->strSymbol,16);
			pSETDisp->fMaxValue = CONVERTFLOAT(pPara->fMaxValue);
			pSETDisp->fMinValue = CONVERTFLOAT(pPara->fMinValue);
			memcpy(pSETDisp->strUnit,pPara->strUnit,8);
			pSETDisp->nCFG = RLTOBS(pPara->nCFG);
			pSETDisp->nDec = RLTOBS(pPara->nDec);
			pSETDisp->nCFIndex = RLTOBS(pPara->nCFIndex);
			memcpy(pSETDisp->CFGList,pPara->CFGList, SETCFG_MAXNUM );
			pSETDisp->nItemNum = RLTOBS(pPara->nItemNum);
			pSETDisp->nItemStart = RLTOBS(pPara->nItemStart);	
			
			pPara++;
		}

		pSETDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ��������ֵ����*/
STATUS InitSystemSETDisp(TSysSET_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TSysSET_Disp 			*pSETDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nSystemSET;

	/*��������ռ�*/
	pSETDisp = (TSysSET_Disp *)OS_MemAlloc(sizeof(TSysSET_Disp)*num);
	ASSERT(pSETDisp != NULL);
	if(pSETDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSystemSETDisp = pSETDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pSETDisp->strCHName,"RCTRat");
				strcpy(pSETDisp->strSymbol,"RCTRat");
				pSETDisp->fMaxValue = 10000;
				pSETDisp->fMinValue = 0;
				strcpy(pSETDisp->strUnit,"");
				pSETDisp->nDec = 3;
				pSETDisp->nCFIndex = 3;
				break;
			case 1:
				strcpy(pSETDisp->strCHName,"MCTRat");
				strcpy(pSETDisp->strSymbol,"MCTRat");
				pSETDisp->fMaxValue = 10000;
				pSETDisp->fMinValue = 0;
				strcpy(pSETDisp->strUnit,"");
				pSETDisp->nDec = 3;
				pSETDisp->nCFIndex = 3;
				break;
			case 2:	
				strcpy(pSETDisp->strCHName,"RTRat");
				strcpy(pSETDisp->strSymbol,"RTRat");
				pSETDisp->fMaxValue = 10000;
				pSETDisp->fMinValue = 0;
				strcpy(pSETDisp->strUnit,"");
				pSETDisp->nDec = 3;
				pSETDisp->nCFIndex = 3;
				break;
			}
			
			pSETDisp->nItemNum = 0;
			pSETDisp->nItemStart = 0;			
		}
		else
		{
			memcpy(pSETDisp->strCHName,pPara->strCHName,20);
			memcpy(pSETDisp->strSymbol,pPara->strSymbol,16);
			pSETDisp->fMaxValue = CONVERTFLOAT(pPara->fMaxValue);
			pSETDisp->fMinValue = CONVERTFLOAT(pPara->fMinValue);
			memcpy(pSETDisp->strUnit,pPara->strUnit,8);
			pSETDisp->nDec =  pPara->nDec;
			pSETDisp->nCFIndex =  pPara->nCFIndex;
			pSETDisp->nItemNum = RLTOBS(pPara->nItemNum);
			pSETDisp->nItemStart = RLTOBS(pPara->nItemStart);	

			pPara++;
		}

		pSETDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ������������*/
STATUS InitSWDisp(TSW_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TSW_Disp 				*pSWDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nSW;

	/*��������ռ�*/
	pSWDisp = (TSW_Disp *)OS_MemAlloc(sizeof(TSW_Disp)*num);
	ASSERT(pSWDisp != NULL);
	if(pSWDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSWDisp = pSWDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pSWDisp->strCHName,"�����ٶ�");
				break;
			case 1:
				strcpy(pSWDisp->strCHName,"����1��");
				break;
			}
		}
		else
		{
			memcpy(pSWDisp->strCHName,pPara->strCHName,20);
			pPara++;
		}

		pSWDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ������������*/
STATUS InitCFDisp(TCF_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TCF_Disp 				*pCFDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nCF;

	/*��������ռ�*/
	pCFDisp = (TCF_Disp *)OS_MemAlloc(sizeof(TCF_Disp)*num);
	ASSERT(pCFDisp != NULL);
	if(pCFDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pCFDisp = pCFDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pCFDisp->strSymbol,"TCoef");
				pCFDisp->wCof = 1;
				break;
			case 1:	
				strcpy(pCFDisp->strSymbol,"IpCoef");
				pCFDisp->wCof = 100;
				break;
			case 2:
				strcpy(pCFDisp->strSymbol,"UpCoef");
				pCFDisp->wCof = 100;
				break;

			case 3:
				strcpy(pCFDisp->strSymbol,"HCoef");
				pCFDisp->wCof = 1;
				break;
			}				
		}
		else
		{
			memcpy(pCFDisp->strSymbol,pPara->strSymbol,8);
			pCFDisp->wCof = ltobs(pPara->wCof);
			pPara++;
		}

		pCFDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ����������ֵ����*/
STATUS InitRMDisp(TRM_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TRM_Disp 				*pRMDisp;
	TRunTimeRM 				*pRMDataBase;	
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nRM;

	/*��������ռ�*/
	pRMDisp = (TRM_Disp *)OS_MemAlloc(sizeof(TRM_Disp)*num);
	ASSERT(pRMDisp != NULL);
	if(pRMDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	/*�ٷ������ݿռ�*/
	pRMDataBase = (TRunTimeRM *)OS_MemAlloc(sizeof(TRunTimeRM)*num);
	ASSERT(pRMDataBase != NULL);
	if(pRMDataBase == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pRMDisp = pRMDisp;
	pDbase->pRM = pRMDataBase;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pRMDisp->strSymbol,"Itp");
				strcpy(pRMDisp->strFirUnit,"A");
				strcpy(pRMDisp->strSecUnit,"A");
				pRMDisp->nFirDec = 0;
				pRMDisp->nSecDec = 2;
				pRMDisp->nIndex		= 0;
				pRMDisp->nRTIndex	= 0;
				pRMDisp->wAttr		= 0;
				pRMDisp->nCFIndex	= 1;
				break;

			case 1:	
				strcpy(pRMDisp->strSymbol,"Ifp");
				strcpy(pRMDisp->strFirUnit,"A");
				strcpy(pRMDisp->strSecUnit,"A");
				pRMDisp->nFirDec = 0;
				pRMDisp->nSecDec = 2;
				pRMDisp->nIndex		= 0;
				pRMDisp->nRTIndex	= 0;
				pRMDisp->wAttr		= 0;
				pRMDisp->nCFIndex	= 1;
				break;

			case 2:	
				strcpy(pRMDisp->strSymbol,"U1");
				strcpy(pRMDisp->strFirUnit,"KV");
				strcpy(pRMDisp->strSecUnit,"V");
				pRMDisp->nFirDec = 5;
				pRMDisp->nSecDec = 3;
				pRMDisp->nIndex		= 0;
				pRMDisp->nRTIndex	= 1;
				pRMDisp->wAttr		= 0;
				pRMDisp->nCFIndex	= 2;
				break;

			case 3:	
				strcpy(pRMDisp->strSymbol,"U2");
				strcpy(pRMDisp->strFirUnit,"KV");
				strcpy(pRMDisp->strSecUnit,"V");
				pRMDisp->nFirDec = 5;
				pRMDisp->nSecDec = 3;
				pRMDisp->nIndex		= 0;
				pRMDisp->nRTIndex	= 2;
				pRMDisp->wAttr		= 0;
				pRMDisp->nCFIndex	= 2;
				break;
			}	
		}
		else
		{
			memcpy(pRMDisp->strSymbol,pPara->strSymbol,8);
			memcpy(pRMDisp->strFirUnit,pPara->strFirUnit,6);
			memcpy(pRMDisp->strSecUnit,pPara->strSecUnit,6);
			pRMDisp->nFirDec = pPara->nFirDec;
			pRMDisp->nSecDec = pPara->nSecDec;
			pRMDisp->nIndex		= ltobs(pPara->nIndex);
			pRMDisp->nRTIndex	= ltobs(pPara->nRTIndex);
			pRMDisp->wAttr		= ltobs(pPara->wAttr);
			pRMDisp->nCFIndex	= ltobs(pPara->nCFIndex);
			pRMDisp->nFirRate	= ltobs(pPara->nFirRate);
			pPara++;
		}

		pRMDisp++;

		/*��ʼ������*/
		pRMDataBase->wStatus = AI_STATUS_RESET|AI_STATUS_ONLINE;
		pRMDataBase++;
	}

	return DB_ERR_OK;
}

/*��ʼ�������¼�����*/
STATUS InitREDisp(TRE_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TRE_Disp 				*pREDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nRE;

	/*��������ռ�*/
	pREDisp = (TRE_Disp *)OS_MemAlloc(sizeof(TRE_Disp)*num);
	ASSERT(pREDisp != NULL);
	if(pREDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pREDisp = pREDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:
				strcpy(pREDisp->strCHName,"��������");
				pREDisp->wInf 		= 1;
				pREDisp->nFD		= 4;
				pREDisp->wRMList[0]	= 0;
				pREDisp->wRMList[1]	= 1;
				pREDisp->wRMList[2]	= 2;
				pREDisp->wRMList[3]	= 3;
				break;

			case 1:	
				strcpy(pREDisp->strCHName,"�����ٶ϶���");
				pREDisp->wInf 		= 2;
				pREDisp->nFD		= 4;
				pREDisp->wRMList[0]	= 0;
				pREDisp->wRMList[1]	= 1;
				pREDisp->wRMList[2]	= 2;
				pREDisp->wRMList[3]	= 3;
//				pREDisp->nFD		= 32;
//				for(int j=0;j<32;j++)
//					pREDisp->wRMList[j]	= j%4;
				break;
			case 2:
				strcpy(pREDisp->strCHName,"����1�ζ���");
				pREDisp->wInf 		= 3;
				pREDisp->nFD		= 4;
				pREDisp->wRMList[0]	= 0;
				pREDisp->wRMList[1]	= 1;
				pREDisp->wRMList[2]	= 2;
				pREDisp->wRMList[3]	= 3;
//				pREDisp->nFD		= 32;
//				for(int j=0;j<32;j++)
//					pREDisp->wRMList[j]	= j%4;
				break;
			}
		}
		else
		{
			memcpy(pREDisp->strCHName,pPara->strCHName,20);
			pREDisp->wInf 		= ltobs(pPara->wInf);
			pREDisp->nFD		= ltobs(pPara->nFD);
			for(int j=0;j<MAX_FD_NUM;j++)
			{
				pREDisp->wRMList[j]	= ltobs(pPara->wRMList[j]);
			}
			pPara++;
		}

		pREDisp++;
	}

	return DB_ERR_OK;
}


/*��ʼ��¼����������*/
STATUS InitDistDisp(TDist_Disp *pPara)
{
	TDBaseConfigTable		*pDbase;
	TDist_Disp 				*pDistDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;

	/*��������ռ�*/
	pDistDisp = (TDist_Disp *)OS_MemAlloc(sizeof(TDist_Disp));
	ASSERT(pDistDisp != NULL);
	if(pDistDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pDistPara = pDistDisp;

	/*��ʼ��ȱʡ����*/
	if(pPara == NULL)
	{
		strcpy(pDistDisp->strDevName,"DEFAULT");
		pDistDisp->wSampleRate 		= 1600;
		pDistDisp->wSysFrequency	= 5000;
		pDistDisp->wRecordCapacity	= 8;
		pDistDisp->dwTotalSampleNum	= 4800;
		pDistDisp->dwPrevSampleNum	= 1600;
		pDistDisp->wAnalogChannelNum= 1;
		pDistDisp->wDigitalChannelNum= 1;
	}
	else
	{
		memcpy(pDistDisp->strDevName,pPara->strDevName,64);
		pDistDisp->wSampleRate 		= ltobs(pPara->wSampleRate);
		pDistDisp->wSysFrequency	= ltobs(pPara->wSysFrequency);
		pDistDisp->wRecordCapacity	= ltobs(pPara->wRecordCapacity);
		pDistDisp->dwTotalSampleNum	= ltobl(pPara->dwTotalSampleNum);
		pDistDisp->dwPrevSampleNum	= ltobl(pPara->dwPrevSampleNum);
		pDistDisp->wAnalogChannelNum= ltobs(pPara->wAnalogChannelNum);
		pDistDisp->wDigitalChannelNum= ltobs(pPara->wDigitalChannelNum);
	}
	
	return DB_ERR_OK;
}

STATUS InitACDisp(TAC_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TAC_Disp 				*pACDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->pDistPara->wAnalogChannelNum;

	/*��������ռ�*/
	pACDisp = (TAC_Disp *)OS_MemAlloc(sizeof(TAC_Disp)*num);
	ASSERT(pACDisp != NULL);
	if(pACDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pACPara= pACDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			strcpy(pACDisp->strName,"I");
			strcpy(pACDisp->strUnit,"A");
			pACDisp->wIndex 			= 0;
			pACDisp->wCoefIndex			= 0;
			pACDisp->wCompressMultiple	= 1;
		}
		else
		{
			memcpy(pACDisp->strName,pPara->strName,16);
			memcpy(pACDisp->strUnit,pPara->strUnit,8);
			pACDisp->wIndex 			= ltobs(pPara->wIndex);
			pACDisp->wCoefIndex			= ltobs(pPara->wCoefIndex);
			pACDisp->wCompressMultiple	= ltobs(pPara->wCompressMultiple);
			
			pACDisp->wRTIndex			= ltobs(pPara->wRTIndex);
			pACDisp->fCFRatio			= ltobs(pPara->fCFRatio);
			pPara++;
		}

		pACDisp++;
	}

	return DB_ERR_OK;
}

STATUS InitDCDisp(TDC_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TDC_Disp 				*pDCDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->pDistPara->wDigitalChannelNum;

	/*��������ռ�*/
	pDCDisp = (TDC_Disp *)OS_MemAlloc(sizeof(TDC_Disp)*num);
	ASSERT(pDCDisp != NULL);
	if(pDCDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pDCPara= pDCDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			strcpy(pDCDisp->strName,"RW");
			pDCDisp->wRelayMacro 		= 0;
		}
		else
		{
			memcpy(pDCDisp->strName,pPara->strName,8);
			pDCDisp->wRelayMacro 		= ltobs(pPara->wRelayMacro);
			pPara++;
		}

		pDCDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ����������*/
STATUS InitTCDisp(TTC_Disp *pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TTC_Disp 				*pTCDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nTC;

	/*��������ռ�*/
	pTCDisp = (TTC_Disp *)OS_MemAlloc(sizeof(TTC_Disp)*num);
	ASSERT(pTCDisp != NULL);
	if(pTCDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pTCDisp = pTCDisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			strcpy(pTCDisp->strCHName,"TTTTTTTTTT");
		}
		else
		{
			memcpy(pTCDisp->strCHName,pPara->strCHName,20);
			pPara++;
		}

		pTCDisp++;
	}

	return DB_ERR_OK;
}

/*��ʼ������λ�������*/
STATUS InitMPODisp(TMBO_Disp *pPara,WORD wParaNum)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TMBO_Disp 				*pMBODisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nMBO;

	/*��������ռ�*/
	pMBODisp = (TMBO_Disp *)OS_MemAlloc(sizeof(TMBO_Disp)*num);
	ASSERT(pMBODisp != NULL);
	if(pMBODisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pMBODisp = pMBODisp;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if((pPara == NULL)||(i >= wParaNum))
		{
			switch(i)
			{
				case 0:
					strcpy(pMBODisp->strStationName,"STA1");
					pMBODisp->wControl = 0;
					pMBODisp->bySelfAddr= 1;
					pMBODisp->bySideAddr= 1;
					pMBODisp->dwSideIP = 0xC0A864B1;
					pMBODisp->dwBaud = 9600;
					pMBODisp->bySMBStart = 0;
					pMBODisp->bySMBNum = 32;
					pMBODisp->byRMBStart = 0;
					pMBODisp->byRMBNum = 32;
					break;
				case 1:
					strcpy(pMBODisp->strStationName,"STA2");
					pMBODisp->wControl = 0;
					pMBODisp->bySelfAddr= 0;
					pMBODisp->bySideAddr= 2;
					pMBODisp->dwSideIP = 0xC0A864AA;
					break;
					
				case 2:
					strcpy(pMBODisp->strStationName,"STA3");
					pMBODisp->wControl = 4;
					pMBODisp->bySelfAddr= 0;
					pMBODisp->bySideAddr= 3;
					pMBODisp->dwBaud = 9600;
					break;

				default:
					break;
			}
		}
		else
		{
			memcpy(pMBODisp->strStationName,pPara->strStationName,20);
			pMBODisp->wControl = ltobs(pPara->wControl);
			pMBODisp->bySelfAddr= (pPara->bySelfAddr);
			pMBODisp->bySideAddr= (pPara->bySideAddr);
			pMBODisp->dwSideIP =  ltobl(pPara->dwSideIP);
			pMBODisp->dwBaud = ltobl(pPara->dwBaud);
			pMBODisp->bySMBStart = pPara->bySMBStart;
			pMBODisp->bySMBNum = pPara->bySMBNum;
			pMBODisp->byRMBStart = pPara->byRMBStart;
			pMBODisp->byRMBNum = pPara->byRMBNum;
			
			pPara++;
		}

		pMBODisp++;
	}

	return DB_ERR_OK;
}


/*��ʼ������λ�������*/
STATUS InitFLDisp(TFL_Disp *pPara,WORD wParaNum)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TFL_Disp 				*pFLDisp;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nFL;

	/*��������ռ�*/
	pFLDisp = (TFL_Disp *)OS_MemAlloc(sizeof(TFL_Disp)*num);
	ASSERT(pFLDisp != NULL);
	if(pFLDisp == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pFLDisp = pFLDisp;

	/*��ʼ����������*/
	if(pPara == NULL)
	{
		pFLDisp[0].wControl = 0xA000;
		pFLDisp[0].byNetAddr = 1;
		pFLDisp[0].byFLNo = 4;
		pFLDisp[0].dwMNetIP =  0xC0A864B1;
		pFLDisp[0].dwSNetIP =  0xAC140AB1;
		pFLDisp[0].dwRsv =  0;
		
		pFLDisp[1].wControl = 0x2001;
		pFLDisp[1].byNetAddr = 1;
		pFLDisp[1].byFLNo = 3;
		pFLDisp[1].dwMNetIP =  0xC0A864B1;
		pFLDisp[1].dwSNetIP =  0xAC140AB1;
		pFLDisp[1].dwRsv =  0;

		pFLDisp[2].wControl = 0xA002;
		pFLDisp[2].byNetAddr = 1;
		pFLDisp[2].byFLNo = 2;
		pFLDisp[2].dwMNetIP =  0xC0A864B1;
		pFLDisp[2].dwSNetIP =  0xAC140AB1;
		pFLDisp[2].dwRsv =	0;

		pFLDisp[3].wControl = 0xA003;
		pFLDisp[3].byNetAddr = 1;
		pFLDisp[3].byFLNo = 1;
		pFLDisp[3].dwMNetIP =  0xC0A864B1;
		pFLDisp[3].dwSNetIP =  0xAC140AB1;
		pFLDisp[3].dwRsv =	0;

		pFLDisp[4].wControl = 0xA004;
		pFLDisp[4].byNetAddr = 1;
		pFLDisp[4].byFLNo = 0;
		pFLDisp[4].dwMNetIP =  0xC0A864B1;
		pFLDisp[4].dwSNetIP =  0xAC140AB1;
		pFLDisp[4].dwRsv =	0;

	}
	else
	{
		for(i=0;i<num;i++,pFLDisp++)
		{
			if(i >= wParaNum)
			{
				pFLDisp->wControl = 0x0000;
			}
			else
			{
				pFLDisp->wControl = ltobs(pPara->wControl);
				pFLDisp->byNetAddr = pPara->byNetAddr;
				pFLDisp->byFLNo = pPara->byFLNo;
				pFLDisp->dwMNetIP =  ltobl(pPara->dwMNetIP);
				pFLDisp->dwSNetIP =  ltobl(pPara->dwSNetIP);
				pFLDisp->dwRsv =  ltobl(pPara->dwRsv);
				
				pPara++;
			}
		}
	}

	return DB_ERR_OK;
}

STATUS InitRelayStartWrap()
{
//	TFileMap				*pFileMap;
//  TFileHead         		*filehead;
	TRelayStartWrap			*pEventWrap;
//	TBasicEventPtrList		*pBasicEventPtr;
	
	pEventWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;

	// 1����Ч��־��顢NvRAM�ڴ�������
	if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
	{
		// 2���ȷ���NVRAM�ռ�
		pEventWrap = (TRelayStartWrap *)DAE_MemAlloc(sizeof(TRelayStartWrap));
		ASSERT(pEventWrap != NULL);
		if(pEventWrap == NULL)
			return DB_ERR_NVMEM_ALLOC;
	}
	else
	{
		if((pEventWrap->dwMagic == WRAP_INIT_FLAG)&&(pEventWrap->nMax == MAX_RSTT_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_RSTTWRAP_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_RSTTWRAP_ERR);
	}
	
	// 3����ʼ��
	pEventWrap->dwMagic = WRAP_INIT_FLAG;
	pEventWrap->nMax = MAX_RSTT_NUM;
	pEventWrap->nHeadIndex = 0;
	pEventWrap->nTailIndex = 0;
	
	
	// 4���ٴ��ļ��лָ��¼���¼
/*	
	pFileMap = G_pCfgTable->DBaseConfig.pBasicEventFileMap;
	ASSERT(pFileMap != NULL);
	filehead = (TFileHead *)pFileMap->pRamData;
	pBasicEventPtr = (TBasicEventPtrList	*)(filehead+1);
		
	pEventWrap->nHeadIndex = pBasicEventPtr->nRelayStartH%MAX_RSTT_NUM;
	pEventWrap->nTailIndex = pBasicEventPtr->nRelayStartT%MAX_RSTT_NUM;
	memcpy(pEventWrap->pEvent,(BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
		+sizeof(TEventBI)*MAX_SOE_NUM\
		+sizeof(TMiscEvent)*MAX_MISC_NUM,\
		sizeof(TRelayStartEv)*MAX_RSTT_NUM);
*/		
	G_pCfgTable->DBaseConfig.pRSTTWrap = pEventWrap;

//	LogError("InitRelayStartWrap",FILE_LINE,"Recover data from file......");
		
	return DB_ERR_OK;
}

STATUS InitRelayActionWrap()
{
	TFileMap					*pFileMap;
    TFileHead         			*filehead;
	TRelayActionWrap			*pEventWrap;
	TRelayEventPtrList			*pRelayEventPtr;
	
	pEventWrap = G_pCfgTable->DBaseConfig.pRACTWrap;

	// 1����Ч��־��顢NvRAM�ڴ�������
	if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
	{
		// 2���ȷ���NVRAM�ռ�
		pEventWrap = (TRelayActionWrap *)DAE_MemAlloc(sizeof(TRelayActionWrap));
		ASSERT(pEventWrap != NULL);
		if(pEventWrap == NULL)
			return DB_ERR_NVMEM_ALLOC;
	}
	else
	{
		if((pEventWrap->dwMagic == WRAP_INIT_FLAG)&&(pEventWrap->nMax == MAX_RACT_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_RACTWRAP_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_RACTWRAP_ERR);
	}
/*
	// 1���ȷ���RAM�ռ�
	pEventWrap = (TRelayActionWrap *)OS_MemAlloc(sizeof(TRelayActionWrap));
	ASSERT(pEventWrap != NULL);
	if(pEventWrap == NULL)
		return DB_ERR_MEM_ALLOC;
*/

	pEventWrap->dwMagic = WRAP_INIT_FLAG;
	pEventWrap->nMax = MAX_RACT_NUM;
	pEventWrap->nHeadIndex = 0;
	pEventWrap->nTailIndex = 0;
		
	// 3���ٴ��ļ��лָ��¼���¼
	pFileMap = G_pCfgTable->DBaseConfig.pRelayEventFileMap;
	ASSERT(pFileMap != NULL);
	filehead = (TFileHead *)pFileMap->pRamData;
	pRelayEventPtr = (TRelayEventPtrList	*)(filehead+1);
		
	pEventWrap->nHeadIndex = pRelayEventPtr->nRelayActionH%MAX_RACT_NUM;
	pEventWrap->nTailIndex = pRelayEventPtr->nRelayActionT%MAX_RACT_NUM;
	memcpy(pEventWrap->pEvent,(BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
		,sizeof(TRelayActionEv)*MAX_RACT_NUM);
		
	G_pCfgTable->DBaseConfig.pRACTWrap = pEventWrap;

	// 4����¼�Ŷ����ݼ�����
	g_wRelayDistNo = pRelayEventPtr->nDistCnt;

	LogError("InitRelayActionWrap",FILE_LINE,"Recover data from EEPROM......");
			
	return DB_ERR_OK;
}

STATUS InitRelayAlarmWrap()
{
	TFileMap					*pFileMap;
    TFileHead         			*filehead;
	TRelayAlarmWrap				*pEventWrap;
	TRelayEventPtrList			*pRelayEventPtr;

	pEventWrap = G_pCfgTable->DBaseConfig.pRALMWrap;

	// 1����Ч��־��顢NvRAM�ڴ�������
	if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
	{
		// 2���ȷ���NVRAM�ռ�
		pEventWrap = (TRelayAlarmWrap *)DAE_MemAlloc(sizeof(TRelayAlarmWrap));
		ASSERT(pEventWrap != NULL);
		if(pEventWrap == NULL)
			return DB_ERR_NVMEM_ALLOC;
	}	
	else
	{
		if((pEventWrap->dwMagic == WRAP_INIT_FLAG)&&(pEventWrap->nMax == MAX_RALM_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_RALMWRAP_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_RALMWRAP_ERR);
	}

/*	
	// 1���ȷ���RAM�ռ�
	pEventWrap = (TRelayAlarmWrap *)OS_MemAlloc(sizeof(TRelayAlarmWrap));
	ASSERT(pEventWrap != NULL);
	if(pEventWrap == NULL)
		return DB_ERR_MEM_ALLOC;
*/		
	pEventWrap->dwMagic = WRAP_INIT_FLAG;
	pEventWrap->nMax = MAX_RALM_NUM;
	pEventWrap->nHeadIndex = 0;
	pEventWrap->nTailIndex = 0;
		
	// 3���ٴ��ļ��лָ��¼���¼
	pFileMap = G_pCfgTable->DBaseConfig.pRelayEventFileMap;
	ASSERT(pFileMap != NULL);
	filehead = (TFileHead *)pFileMap->pRamData;
	pRelayEventPtr = (TRelayEventPtrList	*)(filehead+1);
		
	pEventWrap->nHeadIndex = pRelayEventPtr->nRelayAlarmH%MAX_RALM_NUM;
	pEventWrap->nTailIndex = pRelayEventPtr->nRelayAlarmT%MAX_RALM_NUM;
	memcpy(pEventWrap->pEvent,(BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
		+sizeof(TRelayActionEv)*MAX_RACT_NUM,\
		sizeof(TRelayAlarmEv)*MAX_RALM_NUM);
		
	G_pCfgTable->DBaseConfig.pRALMWrap = pEventWrap;

	LogError("InitRelayAlarmWrap",FILE_LINE,"Recover data from EEPROM......");
		
	return DB_ERR_OK;
}

STATUS InitRelayActionCache()
{
	CLink				*pFreeLink;
//	CLink				*pSaveLink;
	CNode				*pNode;
	TRelayEventCache	*pDataCache;
	
	// 1.�ȷ�����������RAM�ռ�
/*	
	pFreeLink = new CLink(0);//(CLink *)OS_MemAlloc(sizeof(CLink));
	ASSERT(pFreeLink != NULL);
	if(pFreeLink == NULL)
		return DB_ERR_MEM_ALLOC;
		
	// 1.1 ��ʼ����������
	//pFreeLink.CLink(0);
*/
	pFreeLink = &G_RelayRepFreeLink;

	// 1.2 �������ݽڵ�
	for(int i=0;i<MAX_RECACHE_NUM;i++)
	{
		// 1.2.1 ����CACHE�ռ�
		pDataCache = (TRelayEventCache *)OS_MemAlloc(sizeof(TRelayEventCache));
		ASSERT(pDataCache != NULL);
		if(pDataCache == NULL)
		{
			return DB_ERR_MEM_ALLOC;
		}
		
		// 1.2.2 ��ʼ�����ݽڵ�
		pDataCache->bStatus = RELAY_CACHE_READY;
		
		// 1.2.3 ������нڵ�
		pNode = (CNode *)OS_MemAlloc(sizeof(CNode));
		pNode->pData = pDataCache;
		pFreeLink->AddTail(pNode);
	}

	// 2.�ٷ�����������RAM�ռ�
/*	
	pSaveLink = (CLink *)OS_MemAlloc(sizeof(CLink));
	ASSERT(pSaveLink != NULL);
	if(pSaveLink == NULL)
		return DB_ERR_MEM_ALLOC;

	// 2.1 ��ʼ����������
//	pSaveLink.CLink(0);
*/

//	G_pCfgTable->DBaseConfig.pRelayRepFreeLink = pFreeLink;
//	G_pCfgTable->DBaseConfig.pRelayRepSaveLink = pSaveLink;
	
	return DB_ERR_OK;
}

STATUS InitRelayDistLink()
{
	CLink			*pFreeLink;
	CNode			*pNode;
	void			*pDistBlock;
	DWORD			dwBlockLength;
	
	// 1. ��ʼ����������
	pFreeLink = &G_DistDataFreeLink;

	// 2������¼�����ݳ���
	dwBlockLength = DB_GetDistDataLength();
	
	// 3������¼���ļ����ݻ���
	G_pDistFileBlock = OS_MemAlloc(dwBlockLength);
	
	// 4�� �������ݽڵ�
	for(int i=0;i<MAX_DISTBLOCK_NUM;i++)
	{
		pDistBlock = OS_MemAlloc(dwBlockLength);
		ASSERT(pDistBlock != NULL);
		if(pDistBlock == NULL)
		{
			return DB_ERR_MEM_ALLOC;
		}
		
		pNode = (CNode *)OS_MemAlloc(sizeof(CNode));
		ASSERT(pNode != NULL);
		if(pNode == NULL)
		{
			OS_MemFree(pDistBlock);
			return DB_ERR_MEM_ALLOC;
		}
		
		pNode->pData = pDistBlock;

		//ReportMsg("InitRelayDistLink:  Make node(%x->%x) len =%d",(DWORD)pNode,(DWORD)pDistBlock,dwBlockLength);
		pFreeLink->AddTail(pNode);
	}
	
	return DB_ERR_OK;
}

STATUS InitDefaultRelayDispBase()
{
	TDBaseConfigTable	*pDbase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	
	//��ʼ��ȱʡ���ݿ����ñ�
	/* �Ƿ�Ӧ�ø���EEPROM��ȱʡ��ģ�����ͳ�ʼ��Ĭ��ֵ?	*/
	/* ����Ҫȷ��   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/ 
	pDbase->nRM       	= RM_COUNT;
	pDbase->nRE       	= MAX_REPORT_INDEX+1;
	pDbase->nTC       	= RTC_COUNT;
	pDbase->nMBO       	= 0;
	pDbase->nFL       	= MAX_SUBSTATION_NUM;

	//��ʼ���������
	InitSWDisp(NULL);
	InitCFGDisp(NULL);
	InitSETDisp(NULL);
	InitSystemSETDisp(NULL);
	InitCFDisp(NULL);
	InitRMDisp(NULL);
	InitREDisp(NULL);
	InitDistDisp(NULL);
	InitACDisp(NULL);
	InitDCDisp(NULL);
	InitTCDisp(NULL);
	InitMPODisp(NULL,0);
	InitFLDisp(NULL,0);	
	return DB_ERR_OK;
}

/*
STATUS InitDefaultRelayDataBase()
{
	TDBaseConfigTable	*pDbase;
//	TMapDataList		*pMapData;
//	WORD				Length;
//	int					i;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	
	pDbase->nCFG      	= 2;
	pDbase->nSET      	= 6;
	pDbase->nSETArea	= 16;
	pDbase->nSETAreaCur	= 0x0000;
	pDbase->nSETAreaCurB= 0xFFFF;
	pDbase->nSystemSET 	= 3;
	pDbase->nSW       	= 2;
	pDbase->nRT       	= 3;
	pDbase->nCF       	= 4;
	pDbase->nRM       	= 4;
    pDbase->nRW   		= 0;

//	DAE_SetInitFlags(SS_RAM_SW_ERR);
//	DAE_SetInitFlags(SS_RAM_CFG_ERR);
//	DAE_SetInitFlags(SS_RAM_SET_ERR);
//	DAE_SetInitFlags(SS_RAM_SYSSET_ERR);
//	DAE_SetInitFlags(SS_RAM_CF_ERR);

	InitSWBase(NULL);
	InitSETBase(NULL);
	InitSystemSETBase(NULL);
	InitCFBase(NULL);

	return DB_ERR_OK;
}
*/

STATUS MakeDefaultRelayDataFile(BYTE *pFile)
{
	TFileHead			*pFileHead;
	TRelayDataTable		*pConfigTable;
	DWORD 				dwLength;
	
	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;

	DAE_GetFileLength(RELAYDATAFILE, &dwLength);
	
	// 1. ��ʼ���ļ�ͷ
	pFileHead = (TFileHead *)pFile;

	pFileHead->dwSize   = ltobl(dwLength);
	pFileHead->wVersion = ltobs(0x0100);
	pFileHead->wFlag    = ltobs(0);
	pFileHead->dwMagic  = ltobl(FILE_MAGIC_MODI);
	ReadSystemTime(&pFileHead->stTime);
	pFileHead->stTime.Year = ltobs(pFileHead->stTime.Year);
	pFileHead->stTime.MSecond= ltobs(pFileHead->stTime.MSecond);
	
	DAE_MakeCheckSum((BYTE *)pFileHead,sizeof(TFileHead)-sizeof(WORD));
	
	// 2. ��ʼ����������
	pConfigTable = (TRelayDataTable	*)(pFile+sizeof(TFileHead));
	pConfigTable->nSW = ltobs(SW_COUNT);
	pConfigTable->nCFG = ltobs(CFG_COUNT);	
	pConfigTable->nSET = ltobs(RS_COUNT);
	pConfigTable->nSETArea = ltobs(SETTING_AREA_COUNT);
	pConfigTable->nSETAreaCur = ltobs(0x0000);
	pConfigTable->nSETAreaCurB = ltobs(0xFFFF);
	pConfigTable->nSystemSET = ltobs(AS_COUNT);
	pConfigTable->nRT = ltobs(AS_RT_NUM);
	pConfigTable->nCF = ltobs(CF_COUNT);
	
	// 3. ��д�ļ�У��
	DAE_MakeCheckSum(pFile,dwLength-sizeof(WORD));

	// 4. �ļ��ֿ�д��EEPROM�����ݲ��ֲ�����
	// ����У�������ļ�����ʼ��ʱ�������������ڶ�ֵ�������߻ָ���������Ϊ��Ԥ����²������� by Lz 2013.10.24
	//EEPROM_Write(EEPROM_RELAY_DATA,sizeof(TFileHead)+sizeof(TRelayDataTable),pFile);
	//EEPROM_Write(EEPROM_RELAY_DATA+dwLength-2,2,&pFile[dwLength-2]);
	
	return DB_ERR_OK;
}

STATUS InitRelayDispBase()
{
    TFileHead         	*filehead;
    TRelayDispTable		*pRelayDispTable;
	TDBaseConfigTable	*pDbase;
	TCFG_Disp			*pCFGDisp;
	TSET_Disp			*pSETDisp;
	TSysSET_Disp		*pSystemSetDisp;
	TSW_Disp			*pSWDisp;
	TCF_Disp			*pCFDisp;
	TRM_Disp			*pRMDisp;
	TRE_Disp			*pREDisp;
	TDist_Disp			*pDistDisp;
	TAC_Disp			*pACDisp;
	TDC_Disp			*pDCDisp;
	TTC_Disp			*pTCDisp;
	TMBO_Disp			*pMBODisp;
	TFL_Disp			*pFLDisp;
    DWORD 				rc;
	
	pDbase = &G_pCfgTable->DBaseConfig;

	pDbase->pSWDisp= NULL;
	pDbase->pCFGDisp= NULL;
	pDbase->pSETDisp= NULL;
	pDbase->pSystemSETDisp= NULL;
	pDbase->pCFDisp= NULL;
	pDbase->pRMDisp= NULL;
	pDbase->pREDisp= NULL;
	pDbase->pDistPara= NULL;
	pDbase->pACPara= NULL;
	pDbase->pDCPara= NULL;
	pDbase->pDataName= NULL;
	
	
	// 1 ����ļ���ʼ��ʧ�ܣ�����ȱʡ������ʼ��
	if(DAE_GetInitFlags(SS_FILE_RELAYDISP_ERR))
	{
		// 2 ��ȡ�ļ�����
		filehead = (TFileHead *)RelayDispInit;

		// ���ȱʡ�ļ��Ϸ���
	    if( DAE_CheckSystemFile(RelayDispInit,filehead->dwSize) != DB_ERR_OK)
	    {
	        LogError("InitRelayDispBase",FILE_LINE,"default relaydisp.dar file check error!");
			InitDefaultRelayDispBase();
			return DB_ERR_FILE_CHECK;
	    }
		else
		{
			LogError("InitRelayDispBase",FILE_LINE,"From default relaydisp.dar file init......");
		}
	}
	else
	{
		// 2 ��ȡ�ļ�����
		filehead = (TFileHead *)pDbase->pRelayDispFileMap->pRamData;
	}

	pRelayDispTable = (TRelayDispTable *)( filehead + 1 );

	// ������õ�һ����
	if((pDbase->nSW != ltobs(pRelayDispTable->nSW))\
		||(pDbase->nCFG != ltobs(pRelayDispTable->nCFG))\
		||(pDbase->nSET != ltobs(pRelayDispTable->nSET))\
		||(pDbase->nSETArea != ltobs(pRelayDispTable->nSETArea))\
		||(pDbase->nSystemSET != ltobs(pRelayDispTable->nSystemSET))\
		||(pDbase->nRT != ltobs(pRelayDispTable->nRT))\
		||(pDbase->nCF != ltobs(pRelayDispTable->nCF)))
	{
		//����������ȱʡ��
		DAE_SetInitFlags(SS_FILE_RELAYDISP_ERR);

		if(pDbase->nSW != ltobs(pRelayDispTable->nSW))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar SW num is error! %d != %d",pDbase->nSW,pRelayDispTable->nSW);
		}

		if(pDbase->nCFG != ltobs(pRelayDispTable->nCFG))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar CFG num is error! %d != %d",pDbase->nCFG,pRelayDispTable->nCFG);
		}
		if(pDbase->nSET != ltobs(pRelayDispTable->nSET))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar SET num is error! %d != %d",pDbase->nSET,pRelayDispTable->nSET);
		}
		if(pDbase->nSETArea != ltobs(pRelayDispTable->nSETArea))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar SETArea num is error! %d != %d",pDbase->nSETArea,pRelayDispTable->nSETArea);
		}
		if(pDbase->nSystemSET != ltobs(pRelayDispTable->nSystemSET))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar SystemSET num is error! %d != %d",pDbase->nSystemSET,pRelayDispTable->nSystemSET);
		}
		if(pDbase->nRT != ltobs(pRelayDispTable->nRT))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar RT num is error! %d != %d",pDbase->nRT,pRelayDispTable->nRT);
		}
		if(pDbase->nCF != ltobs(pRelayDispTable->nCF))
		{
			LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar CF num is error! %d != %d",pDbase->nCF,pRelayDispTable->nCF);
		}
			
		InitDefaultRelayDispBase();

        LogError("InitRelayDispBase",FILE_LINE,"Relaydisp.dar data num is error!");

		return DB_ERR_FILE_CHECK;
	}	

	DAE_ClearInitFlags(SS_FILE_RELAYDISP_ERR);
	
	// ��ʼ��������
	pDbase->nRM       	= ltobs(pRelayDispTable->nRM);
	pDbase->nRE       	= ltobs(pRelayDispTable->nRE);
	pDbase->nDataName	= ltobs(pRelayDispTable->nDataName);
	pDbase->nTC 		= ltobs(pRelayDispTable->nTC);
	pDbase->nMBO       	= ltobs(pRelayDispTable->nMBO);
	pDbase->nFL       	= ltobs(pRelayDispTable->nFL);
	
	// 5 �����ļ����ó�ʼ������
	pSWDisp			= (TSW_Disp*)( pRelayDispTable + 1 );
    pCFGDisp		= (TCFG_Disp*)( pSWDisp + ltobs(pRelayDispTable->nSW));
    pSETDisp		= (TSET_Disp*)( pCFGDisp + ltobs(pRelayDispTable->nCFG));
    pSystemSetDisp	= (TSysSET_Disp*)( pSETDisp + ltobs(pRelayDispTable->nSET));
    pCFDisp			= (TCF_Disp*)( pSystemSetDisp+ ltobs(pRelayDispTable->nSystemSET));
    pRMDisp			= (TRM_Disp*)( pCFDisp+ ltobs(pRelayDispTable->nCF));
    pREDisp 		= (TRE_Disp*)( pRMDisp+ ltobs(pRelayDispTable->nRM));
    pDistDisp 		= (TDist_Disp*)( pREDisp+ ltobs(pRelayDispTable->nRE));
	
	InitSWDisp(pSWDisp);
	InitCFGDisp(pCFGDisp);
	InitSETDisp(pSETDisp);
	InitSystemSETDisp(pSystemSetDisp);
	InitCFDisp(pCFDisp);
	InitRMDisp(pRMDisp);
	InitREDisp(pREDisp);
	InitDistDisp(pDistDisp);	
	//���ڱ��������ļ���û�й���¼�������ã������ʱ����Ϊȱʡ����
	//InitDistDisp(NULL);

	//��ʼ���Ŷ�����

	//ģ��ͨ�������Ϸ��Լ��
	if((pDbase->pDistPara->wAnalogChannelNum > DIST_ANALOGCHN_MAX)||\
		(pDbase->pDistPara->wDigitalChannelNum > DIST_DIGITALCHN_MAX)||\
		(pDbase->pDistPara->dwTotalSampleNum > DIST_SAMPLENUM_MAX))
	{
        LogError("InitRelayDispBase",FILE_LINE,"Relay Disturb Para Error!!!");
		
		pDbase->pDistPara->wSampleRate 		= 1600;
		pDbase->pDistPara->wSysFrequency	= 5000;
		pDbase->pDistPara->wRecordCapacity	= 8;
		pDbase->pDistPara->dwTotalSampleNum	= 4800;
		pDbase->pDistPara->dwPrevSampleNum	= 1600;
		pDbase->pDistPara->wAnalogChannelNum= 1;
		pDbase->pDistPara->wDigitalChannelNum= 1;

		
		pDbase->pDistPara->wAnalogChannelNum = DIST_ANALOGCHN_MAX;
		InitACDisp(NULL);
		
		pDbase->pDistPara->wDigitalChannelNum = DIST_DIGITALCHN_MAX;
		InitDCDisp(NULL);

		pDbase->pDataName= NULL;
	}
	else
	{
		pACDisp 		= (TAC_Disp*)( pDistDisp + 1);
		
		InitACDisp(pACDisp);
		
		pDCDisp 		= (TDC_Disp*)( pACDisp + ltobs(pDistDisp->wAnalogChannelNum));
	
		InitDCDisp(pDCDisp);		

		pDbase->pDataName= (char*)( pDCDisp + pDbase->pDistPara->wDigitalChannelNum);
	}

	// ��ʼ����������	
	pTCDisp = (TTC_Disp*)( pDbase->pDataName+(pDbase->nDataName*SETNAME_MAXLENGTH) );
    pMBODisp = (TMBO_Disp*)( pTCDisp+ ltobs(pRelayDispTable->nTC));
	pFLDisp = (TFL_Disp*)( pMBODisp+ ltobs(pRelayDispTable->nMBO));

	InitTCDisp(pTCDisp);
	InitMPODisp(pMBODisp,pDbase->nMBO);
	InitFLDisp(pFLDisp,pDbase->nFL);

	// ����
//	pDbase->nFL       	= MAX_SUBSTATION_NUM;
//	InitFLDisp(NULL,0);
	
	// ��¼����λ��������ļ�ָ��
	pDbase->tMapData.pMBODisp = pMBODisp;
	pDbase->tMapData.pFLDisp = pFLDisp;
	
	return DB_ERR_OK;
}

STATUS InitRelayDataBase()
{
	TFileHead         	*filehead;
    TRelayDataTable		*pRelayDataTable;
	TDBaseConfigTable	*pDbase;
	TMapDataList		*pMapData;
//	DWORD				*pCFG;
	DWORD				*pSW,*pSWor;
	float				*pSET;
	float				*pSystemSET;
	long				*pCF;
	BYTE				*pParaBuf = NULL;
	
	pDbase = &G_pCfgTable->DBaseConfig;

	pDbase->pSWData= NULL;
	pDbase->pCFGData= NULL;
	pDbase->pSETDataFix= NULL;
	pDbase->pSETDataFloat= NULL;
	pDbase->pSystemSETData= NULL;
	pDbase->pCFData= NULL;
	
	pDbase->pSWDisp= NULL;
	pDbase->pCFGDisp= NULL;
	pDbase->pSETDisp= NULL;
	pDbase->pSystemSETDisp= NULL;
	pDbase->pCFDisp= NULL;
	pDbase->pRMDisp= NULL;
	pDbase->pREDisp= NULL;
	pDbase->pDistPara= NULL;
	pDbase->pACPara= NULL;
	pDbase->pDCPara= NULL;
	
	pDbase->pSWOpt = NULL;
	pDbase->pCFGOpt = NULL;
	pDbase->pSETOpt= NULL;
	pDbase->pSETAreaOpt= NULL;
	pDbase->pCFOpt= NULL;

	// �������ش���!!!!
	if(G_pCfgTable->DBaseConfig.pRelayDataFileMap ==  NULL)
		return DB_ERR_MEM_ALLOC;
	
	pParaBuf = (BYTE *)G_pCfgTable->DBaseConfig.pRelayDataFileMap->pRamData;
	
	// 1 ����ļ���ʼ��ʧ�ܣ�����ȱʡ������ʼ��
	if(DAE_GetInitFlags(SS_FILE_RELAYDATA_ERR))
	{
		// 1.2 ��EEPROMȱʡ���ݳ�ʼ��
		MakeDefaultRelayDataFile(pParaBuf); 
		filehead = (TFileHead *)pParaBuf;
		LogError("InitRelayDataBase",FILE_LINE,"Relay data check error!");
/*		
		// 1.1 ��ȱʡ�ļ���ʼ��
		filehead = (TFileHead *)RelayDataInit;

	    if( DAE_CheckSystemFile(RelayDataInit,filehead->dwSize) != DB_ERR_OK)
		{
			// 1.2 ��EEPROMȱʡ���ݳ�ʼ��
			MakeDefaultRelayDataFile(pParaBuf); 
			filehead = (TFileHead *)pParaBuf;
			
			LogError("InitRelayDataBase",FILE_LINE,"default relay data check error!");
		}
		else
		{
			LogError("InitRelayDataBase",FILE_LINE,"From default relay data init......");
		}
*/		
	}
	else
	{
		// 1.3 ��ȡ�ļ�����
		filehead = (TFileHead *)pParaBuf;
	}
	
	pRelayDataTable = (TRelayDataTable *)( filehead + 1 );

	// ������õ�һ����
	if((pRelayDataTable->nSW != ltobs(SW_COUNT))\
		||(pRelayDataTable->nCFG != ltobs(CFG_COUNT))\
		||(pRelayDataTable->nSET != ltobs(RS_COUNT))\
		||(pRelayDataTable->nSETArea != ltobs(SETTING_AREA_COUNT))\
		||(pRelayDataTable->nSystemSET != ltobs(AS_COUNT))\
		||(pRelayDataTable->nRT != ltobs(AS_RT_NUM))\
		||(pRelayDataTable->nCF != ltobs(CF_COUNT)))
	{
		if(pRelayDataTable->nSW != ltobs(SW_COUNT))
		{
			LogError("InitRelayDataBase",FILE_LINE,"Relayconf.dar SW num is error! %d != %d",pRelayDataTable->nSW,SW_COUNT);
		}

		if(pRelayDataTable->nCFG != ltobs(CFG_COUNT))
		{
			LogError("InitRelayDataBase",FILE_LINE,"Relayconf.dar CFG num is error! %d != %d",pRelayDataTable->nCFG,CFG_COUNT);
		}
		if(pRelayDataTable->nSET != ltobs(RS_COUNT))
		{
			LogError("InitRelayDataBase",FILE_LINE,"Relayconf.dar SET num is error! %d != %d",pRelayDataTable->nSET,RS_COUNT);
		}
		if(pRelayDataTable->nSETArea != ltobs(SETTING_AREA_COUNT))
		{
			LogError("InitRelayDataBase",FILE_LINE,"Relayconf.dar SETArea num is error! %d != %d",pRelayDataTable->nSETArea,SETTING_AREA_COUNT);
		}
		if(pRelayDataTable->nSystemSET != ltobs(AS_COUNT))
		{
			LogError("InitRelayDataBase",FILE_LINE,"Relayconf.dar SystemSET num is error! %d != %d",pRelayDataTable->nSystemSET,AS_COUNT);
		}
		if(pRelayDataTable->nCF != ltobs(CF_COUNT))
		{
			LogError("InitRelayDataBase",FILE_LINE,"Relayconf.dar CF num is error! %d != %d",pRelayDataTable->nCF,CF_COUNT);
		}
		
		DAE_SetInitFlags(SS_FILE_DIFFRELAYDATA_ERR);
	}
	else
		DAE_ClearInitFlags(SS_FILE_DIFFRELAYDATA_ERR);
		
	pDbase->nSW      	= RLTOBS(pRelayDataTable->nSW);
	pDbase->nCFG      	= RLTOBS(pRelayDataTable->nCFG);
	pDbase->nSET       	= RLTOBS(pRelayDataTable->nSET);
	pDbase->nSETArea   	= RLTOBS(pRelayDataTable->nSETArea);
	pDbase->nSETAreaCur	= RLTOBS(pRelayDataTable->nSETAreaCur);
	pDbase->nSETAreaCurB= RLTOBS(pRelayDataTable->nSETAreaCurB);
	pDbase->nSystemSET	= RLTOBS(pRelayDataTable->nSystemSET);//AS_COUNT;
	pDbase->nRT       	= RLTOBS(pRelayDataTable->nRT);//AS_RT_NUM;
	pDbase->nCF       	= RLTOBS(pRelayDataTable->nCF);//CF_COUNT;
		
	// 5 �����ļ����ó�ʼ����������
	// 5 �����ļ����ó�ʼ����������
	pSW 			= (DWORD*)( pRelayDataTable + 1);
	pSWor			= (DWORD*)( pSW + (pDbase->nSW+31)/32 );
	pSET			= (float*)( pSWor + (pDbase->nSW+31)/32);
	pSystemSET		= (float*)( pSET + (pDbase->nSET+1)*pDbase->nSETArea);
//	  pCF				= (long*)( pSystemSET+ pDbase->nSystemSET+1);	

	// ϵ���������
	pCF 			= (long*)OS_MemAlloc( (pDbase->nCF+1)*sizeof(long)*2);
	EEPROM_Read(EEPROM_RELAY_CFBASE, (pDbase->nCF+1)*sizeof(long)*2, pCF);

	//�ȼ�鶨��ֵ����
	if(!DB_CheckSETArea(pDbase->nSETAreaCur,pDbase->nSETAreaCurB))
		DAE_SetInitFlags(SS_EEPROM_SETAREA_ERR);
	else
		DAE_ClearInitFlags(SS_EEPROM_SETAREA_ERR);

	InitSWBase(pSW);
	InitSETBase(pSET);
	InitSystemSETBase(pSystemSET);
	InitCFBase(pCF);
		
	//ע���������
	pMapData = &pDbase->tMapData;
	pMapData->pRelayCfg		= pRelayDataTable;
	pMapData->pSWData		= pSW;
	pMapData->pSETData		= pSET;
	pMapData->pSystemSETData= pSystemSET;
	pMapData->pCFData		= pCF;

	return DB_ERR_OK;
}

/*��ʼ�����㶨ֵ*/
STATUS InitSETFixDataBase()
{
	WORD 					num,i,CFGNum;
	TDBaseConfigTable		*pDbase;
	float 					*pSETfloat;
	long 					*pSETFix;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	pSETfloat = pDbase->pSETDataFloat;
	num = pDbase->nSET;

	if(pSETfloat == NULL)
		return DB_ERR_BUFNULL;

	if(pDbase->pSETDataFix != NULL)
		pSETFix = pDbase->pSETDataFix;
	else
	{
		pSETFix = (long *)OS_MemAlloc(sizeof(long)*(num+1));
		ASSERT(pSETFix != NULL);
		if(pSETFix == NULL)
			return DB_ERR_MEM_ALLOC;
	}
	
	//����С��λ����ʼ�����㶨ֵ
	CFGNum = (pDbase->nCFG+31)/32;
	for(i=0;i<num;i++)
	{
		//����������
		if(i<CFGNum)
			pSETFix[i] = *(DWORD *)(pSETfloat+i);
		else
			ConvertSETFloat2Fix(i,pSETfloat[i],&pSETFix[i]);
	}
	
	// ���¼���У��
	DAE_MakeCheckSum((BYTE *)pSETFix,sizeof(float)*num);
	
	pDbase->pSETDataFix = pSETFix;

	return DB_ERR_OK;
}


