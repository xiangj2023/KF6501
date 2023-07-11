/********************************************************************************************************/
/*																										*/
/*        							��򿭷������ɷ����޹�˾											*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           													*/
/*																										*/
/*      Dbaseinit.cpp                                     												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ����������ݿ��ʼ������غ���																*/
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
/*      InitAITBase()				��ʼ��ң����������													*/
/*      InitDAIBase()				��ʼ��ģ����ֱ�Ӳ��������											*/
/*      InitDIBase()				��ʼ��������ֱ�Ӳ��������											*/
/*      InitAIBase()				��ʼ��ң������														*/
/*      InitBIBase()				��ʼ��ң������														*/
/*      InitCIBase()				��ʼ���������														*/
/*      InitBOBase()				��ʼ��ң������														*/
/*      InitAOBase()				��ʼ��ң������														*/
/*      InitSerialPara()			��ʼ�����ڲ���														*/
/*      InitCanBusPara()			��ʼ��CAN�ڲ���														*/
/*      InitEthernetPara()			��ʼ����̫���ڲ���													*/
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
#include "db_err.h"
#include "initparaconf.h"
#include "scdefs.h"

extern TBasicConfig	G_tBasicConfig;
extern TSysConfigTable	*G_pCfgTable;

STATUS InitRelayStartWrap();
STATUS InitRelayAlarmWrap();
STATUS InitRelayActionWrap();
STATUS InitRelayActionCache();
STATUS InitRelayDistLink();

//��ʼ��ģ����ֱ�Ӳ��������
STATUS InitDAIBase(TDAIPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TDAIPara			*pDAIBase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nDAI;

	pDAIBase = (TDAIPara *)OS_MemAlloc(sizeof(TDAIPara)*num);

	ASSERT(pDAIBase != NULL);
	if(pDAIBase == NULL)
		return DB_ERR_MEM_ALLOC;
	
	pDbase->pDAIPara = pDAIBase;
	
	for(i=0;i<num;i++)
	{
		//��ʼ��ȱʡ����
		if(pPara == NULL)
		{
			pDAIBase->wLineNo = 0; 
			pDAIBase->wClass  = DAI_ATTR_3J;
			pDAIBase++;
		}
		//��ʼ��ʵ�ʲ���
		else
		{
			pDAIBase->wLineNo = ltobs(pPara->wLineNo); 
			pDAIBase->wClass  = ltobs(pPara->wClass);

			pDAIBase++;
			pPara++;
		}
	}
	return DB_ERR_OK;
}

//��ʼ��������ֱ�Ӳ��������
STATUS InitDIBase(TDIPara	*pPara)
{
	WORD 				num,i, j;
	WORD				wFilterTime[MAX_DI_NUM+1];
	TDBaseConfigTable	*pDbase;
	TDIPara				*pDIBase;
	BOOL				bEEPROMValid=FALSE;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nDI;

	pDIBase = (TDIPara *)OS_MemAlloc(sizeof(TDIPara)*num);

	ASSERT(pDIBase != NULL);
	if(pDIBase == NULL)
		return DB_ERR_MEM_ALLOC;
	
	pDbase->pDIPara	= pDIBase;

	// ��ȡң��ȥ��ʱ��
	if(num<=MAX_DI_NUM)
	if(EEPROM_Read(EEPROM_DI_FILTERTIME,sizeof(WORD)*(MAX_DI_NUM+1),wFilterTime) == DB_ERR_OK)
	if(DAE_IsCheckSumOK((BYTE *)wFilterTime,sizeof(WORD)*num))
		bEEPROMValid = TRUE;
	else
		LogError("InitDIBase",FILE_LINE,"EEPROM DI filter time check error!");
	
	j = 0;
	for(i=0;i<num;i++)
	{
		//��ʼ��ȱʡ����
		if(pPara == NULL)
		{
			//��ʼ��DI����
			if(i<34)
				pDIBase->wClass 		= DI_TYPE_SP; 
			else if(i<47)
			{
				pDIBase->wClass 		= DI_TYPE_INNER; 
				pDIBase->bInDINo 		= j;		//��ʼ���ڲ�ң�ŵ����
				pDIBase->bInDIBoardNo	= 0;
				j++;
			}
			else
				pDIBase->wClass 		= DI_TYPE_VIR; 
			
			pDIBase->wFilterTime	= 50; 
		}
		//��ʼ��ʵ�ʲ���
		else
		{
			pDIBase->wClass 		= ltobs(pPara->wClass); 
			pDIBase->bInDINo         = pPara->bInDINo;
			pDIBase->bInDIBoardNo = pPara->bInDIBoardNo;

			// ��EEPROMΪ׼
			if(i<MAX_DI_NUM)
			{
				if(bEEPROMValid)
					pDIBase->wFilterTime  	= wFilterTime[i];
				else
				{
					pDIBase->wFilterTime  	= ltobs(pPara->wFilterTime);
					wFilterTime[i] = pDIBase->wFilterTime;
				}
			}
			
			pPara++;
		}

		pDIBase++;
	}

	// ��дEEPROM
	if(!bEEPROMValid)
	{
		if(num <= MAX_DI_NUM)
			DAE_MakeCheckSum((BYTE *)wFilterTime,sizeof(WORD)*num);
		EEPROM_Write(EEPROM_DI_FILTERTIME,sizeof(WORD)*(MAX_DI_NUM+1),wFilterTime);
	}
	
	return DB_ERR_OK;
}

/*��ʼ��ң�����*/
STATUS InitAIBase(TAIPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TAIPara				*pAIParaBase;
	TRunTimeAI			*pAIDataBase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nAI;

	/*�ȷ�������ռ�*/
	pAIParaBase = (TAIPara *)OS_MemAlloc(sizeof(TAIPara)*num);
	ASSERT(pAIParaBase != NULL);
	if(pAIParaBase == NULL)
		return DB_ERR_MEM_ALLOC;

	/*�ٷ������ݿռ�*/
	pAIDataBase = (TRunTimeAI *)OS_MemAlloc(sizeof(TRunTimeAI)*num);
	ASSERT(pAIDataBase != NULL);
	if(pAIDataBase == NULL)
		return DB_ERR_MEM_ALLOC;
	
	pDbase->pAIAttr= pAIParaBase;
	pDbase->pAI	= pAIDataBase;

	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pAIParaBase->strSymbol,"U1");
				strcpy(pAIParaBase->strFirUnit1,"kV");
				strcpy(pAIParaBase->strSecUnit,"V");
				pAIParaBase->nFirDec1 = 5;
				pAIParaBase->nSecDec = 3;
				break;
			case 1:
				strcpy(pAIParaBase->strSymbol,"Up");
				strcpy(pAIParaBase->strFirUnit1,"kV");
				strcpy(pAIParaBase->strSecUnit,"V");
				pAIParaBase->nFirDec1 = 0;
				pAIParaBase->nSecDec = 1;
				break;
			case 2:
				strcpy(pAIParaBase->strSymbol,"Up");
				strcpy(pAIParaBase->strFirUnit1,"kV");
				strcpy(pAIParaBase->strSecUnit,"V");
				pAIParaBase->nFirDec1 = 0;
				pAIParaBase->nSecDec = 1;
				break;
			case 3:
				strcpy(pAIParaBase->strSymbol,"Ip");
				strcpy(pAIParaBase->strFirUnit1,"A");
				strcpy(pAIParaBase->strSecUnit,"A");
				pAIParaBase->nFirDec1 = 0;
				pAIParaBase->nSecDec = 2;
				break;
			}	

			pAIParaBase->wNameIndex 	= i; 
			pAIParaBase->wRatioIndex	= 0;
			pAIParaBase->nCFIndex		= 0;
			pAIParaBase->wControl		= 0;
			pAIParaBase->a				= 1;
			pAIParaBase->b				= 1;
			pAIParaBase->c				= 1;
			pAIParaBase->wDeadLine		= 1;			
			pAIParaBase->lHiUpLimit 	= 0x7FFFFFFF;
			pAIParaBase->lUpLimit		= 0x7FFFFFFF;
			pAIParaBase->lDownLimit 	= 0;
			pAIParaBase->lLowDownLimit	= 0;
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			memcpy(pAIParaBase->strSymbol,pPara->strSymbol,8);
			memcpy(pAIParaBase->strFirUnit1,pPara->strFirUnit1,6);
			memcpy(pAIParaBase->strFirUnit2,pPara->strFirUnit2,6);
			memcpy(pAIParaBase->strFirUnit3,pPara->strFirUnit3,6);
			memcpy(pAIParaBase->strSecUnit,pPara->strSecUnit,6);
			pAIParaBase->nFirDec1 = pPara->nFirDec1;
			pAIParaBase->nFirDec2 = pPara->nFirDec2;
			pAIParaBase->nFirDec3 = pPara->nFirDec3;
			pAIParaBase->nSecDec = pPara->nSecDec;
			pAIParaBase->nDataRate2		= pPara->nDataRate2;
			pAIParaBase->nDataRate3		= pPara->nDataRate3;
			pAIParaBase->wNameIndex		= ltobs(pPara->wNameIndex); 
			pAIParaBase->wRatioIndex	= ltobs(pPara->wRatioIndex);
			pAIParaBase->wControl		= ltobs(pPara->wControl);
			pAIParaBase->a				= ltobs(pPara->a);
			pAIParaBase->b				= ltobs(pPara->b);
			pAIParaBase->c				= ltobs(pPara->c);
			pAIParaBase->wDeadLine		= ltobs(pPara->wDeadLine);			
			pAIParaBase->lHiUpLimit		= ltobl(pPara->lHiUpLimit);
			pAIParaBase->lUpLimit		= ltobl(pPara->lUpLimit);
			pAIParaBase->lDownLimit		= ltobl(pPara->lDownLimit);
			pAIParaBase->lLowDownLimit	= ltobl(pPara->lLowDownLimit);

			pAIParaBase->lDispLimit12	= ltobl(pPara->lDispLimit12);
			pAIParaBase->lDispLimit23	= ltobl(pPara->lDispLimit23);

			pAIParaBase->nCFIndex		= ltobs(pPara->nCFIndex);
			
			pPara++;

		}
		pAIParaBase++;

		/*��ʼ������*/
		pAIDataBase->wStatus = AI_STATUS_RESET|AI_STATUS_ONLINE;
		pAIDataBase++;
	}

	return DB_ERR_OK;
}

/*��ʼ��ң�Ų���*/
STATUS InitBIBase(TBIPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TBIPara				*pBIParaBase;
	TRunTimeBI			*pBIDataBase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nBI;

	/*�ȷ�������ռ�*/
	pBIParaBase = (TBIPara *)OS_MemAlloc(sizeof(TBIPara)*num);
	ASSERT(pBIParaBase != NULL);
	if(pBIParaBase == NULL)
		return DB_ERR_MEM_ALLOC;

	/*�ٷ������ݿռ�*/
	pBIDataBase = (TRunTimeBI *)OS_MemAlloc(sizeof(TRunTimeBI)*num);
	ASSERT(pBIDataBase != NULL);
	if(pBIDataBase == NULL)
		return DB_ERR_MEM_ALLOC;
	
	pDbase->pBIAttr= pBIParaBase;
	pDbase->pBI	= pBIDataBase;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:
				strcpy(pBIParaBase->strSymbol,"�źŸ���");
				pBIParaBase->wNameIndex = 0;
				pBIParaBase->wControl	= 8;
				break;
			case 1:
				strcpy(pBIParaBase->strSymbol,"����ѹ��");
				pBIParaBase->wNameIndex = 0;
				pBIParaBase->wControl	= 2;
				break;
			}		
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			memcpy(pBIParaBase->strSymbol,pPara->strSymbol,8);
			pBIParaBase->wNameIndex = ltobs(pPara->wNameIndex);
			pBIParaBase->wControl 	= ltobs(pPara->wControl);
			pBIParaBase->wMaptoDINo = ltobs(pPara->wMaptoDINo);//������Ŵ�0��ʼ
			pPara++;
		}
		pBIParaBase++;

		/*��ʼ�����ݲ���*/
		pBIDataBase->bStatus = BI_STATUS_RESET|BI_STATUS_ONLINE;
		pBIDataBase++;
	}

	return DB_ERR_OK;
}

/*��ʼ����Ȳ���*/
STATUS InitCIBase(TCIPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TCIPara				*pCIParaBase;
	TRunTimeCI			*pCIDataBase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nCI;

	/*�ȷ�������ռ�*/
	pCIParaBase = (TCIPara *)OS_MemAlloc(sizeof(TCIPara)*num);
	ASSERT(pCIParaBase != NULL);
	if(pCIParaBase == NULL)
		return DB_ERR_MEM_ALLOC;

	/*�ٷ������ݿռ�*/
	/*���ݿ��ڴ����쳣ʱ���¿��ٿռ�*/
	/*
	if(DAE_GetInitFlags(SS_NVRAM_INIT)||DAE_GetInitFlags(SS_NVRAM_LINK_ERR))
	{
		DAE_SetInitFlags(SS_NVRAM_CI_ERR);
		
		pCIDataBase = (TRunTimeCI *)DAE_MemAlloc(sizeof(TRunTimeCI)*num);
		ASSERT(pCIDataBase != NULL);
		if(pCIDataBase == NULL)
			return DB_ERR_NVMEM_ALLOC;
		
		pDbase->pCI = pCIDataBase;
	}
	else
		pCIDataBase = pDbase->pCI;
	*/
	pCIDataBase = (TRunTimeCI	*)DBASE_NVRAM_CI;
	pDbase->pCIAttr = pCIParaBase;
/*
	pCIDataBase = (TRunTimeCI *)OS_MemAlloc(sizeof(TRunTimeCI)*num);
	ASSERT(pCIDataBase != NULL);
	if(pCIDataBase == NULL)
		return DB_ERR_MEM_ALLOC;
*/	
	pDbase->pCIAttr = pCIParaBase;
	pDbase->pCI = pCIDataBase;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			switch(i)
			{
			case 0:	
				strcpy(pCIParaBase->strSymbol,"DD1");
				pCIParaBase->wNameIndex = 0;
				pCIParaBase->wControl 	= 4;
				pCIParaBase->dwConst	= 400;
				pCIParaBase->a			= 1;
				pCIParaBase->b			= 1;
				break;
			case 1:
				strcpy(pCIParaBase->strSymbol,"DD2");
				pCIParaBase->wNameIndex = 0;
				pCIParaBase->wControl 	= 12;
				pCIParaBase->dwConst	= 1000;
				pCIParaBase->a			= 2;
				pCIParaBase->b			= 2;
				break;
			}	
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			memcpy(pCIParaBase->strSymbol,pPara->strSymbol,8);
			pCIParaBase->wNameIndex = ltobs(pPara->wNameIndex);
			pCIParaBase->wControl 	= ltobs(pPara->wControl);
			pCIParaBase->dwConst	= ltobs(pPara->dwConst);
			pCIParaBase->a			= ltobs(pPara->a);
			pCIParaBase->b			= ltobs(pPara->b);
			pCIParaBase->wMaptoDINo = ltobs(pPara->wMaptoDINo);//������Ŵ�0��ʼ
			
			pPara++;
		}
		/*��ʼ�����ݲ���*/
		pCIDataBase->bStatus = CI_STATUS_ONLINE;
		if(pCIParaBase->wControl&CI_ATTR_PULSE)
		{
			// �������������Ҫ�����������ֵ
			pCIDataBase->dwValue %= (100000*pCIParaBase->dwConst);
		}
		else
		{
			// ���������������Ҫ����0.01ȡģ
			pCIDataBase->dwValue %= 10000000;
		}

		pCIParaBase++;
		pCIDataBase++;
	}

	return DB_ERR_OK;
}

/*��ʼ��ң�ز���*/
STATUS InitBOBase(TBOPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TBOPara				*pBOParaBase;
	TRunTimeBO			*pBODataBase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nBO;

	/*�ȷ�������ռ�*/
	pBOParaBase = (TBOPara *)OS_MemAlloc(sizeof(TBOPara)*num);
	ASSERT(pBOParaBase != NULL);
	if(pBOParaBase == NULL)
		return DB_ERR_MEM_ALLOC;

	/*�ٷ������ݿռ�*/
	pBODataBase = (TRunTimeBO *)OS_MemAlloc(sizeof(TRunTimeBO)*num);
	ASSERT(pBODataBase != NULL);
	if(pBODataBase == NULL)
		return DB_ERR_MEM_ALLOC;
	
	pDbase->pBOAttr= pBOParaBase;
	pDbase->pBO	= pBODataBase;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			strcpy(pBOParaBase->strSymbol,"YK");
			pBOParaBase->wSwitchNo = i;
			pBOParaBase->dwControl = (BO_NULL<<BO_ATTR_OPTSHIFT);
			pBOParaBase->wLastTime = 5000;
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			memcpy(pBOParaBase->strSymbol,pPara->strSymbol,8);
			pBOParaBase->dwControl 	= ltobl(pPara->dwControl);
			pBOParaBase->wNameIndex	= ltobs(pPara->wNameIndex);
			pBOParaBase->wSwitchNo	= ltobs(pPara->wSwitchNo);	//������Ŵ�0��ʼ
			pBOParaBase->wRelayNo	= ltobs(pPara->wRelayNo);			
			pBOParaBase->wLastTime	= ltobs(pPara->wLastTime);
			
			pPara++;
		}
		pBOParaBase++;

		/*��ʼ�����ݲ���*/
		pBODataBase->wStatus 		= BO_STATUS_ONLINE|BO_STATUS_RESET;
		pBODataBase++;
	}

	return DB_ERR_OK;
}

/*��ʼ��ң������*/
STATUS InitAOBase(TAOPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TAOPara				*pAOParaBase;
	TRunTimeAO			*pAODataBase;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nAO;

	/*�ȷ�������ռ�*/
	pAOParaBase = (TAOPara *)OS_MemAlloc(sizeof(TAOPara)*num);
	ASSERT(pAOParaBase != NULL);
	if(pAOParaBase == NULL)
		return DB_ERR_MEM_ALLOC;

	/*�ٷ������ݿռ�*/
	pAODataBase = (TRunTimeAO *)OS_MemAlloc(sizeof(TRunTimeAO)*num);
	ASSERT(pAODataBase != NULL);
	if(pAODataBase == NULL)
		return DB_ERR_MEM_ALLOC;
	
	pDbase->pAOAttr= pAOParaBase;
	pDbase->pAO	= pAODataBase;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			strcpy(pAOParaBase->strSymbol,"YT");
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			memcpy(pAOParaBase->strSymbol,pPara->strSymbol,8);
			pAOParaBase->wControl 	= ltobs(pPara->wControl);
			pAOParaBase->wNameIndex	= ltobs(pPara->wNameIndex);
			pAOParaBase->nID		= ltobs(pPara->nID);
			pAOParaBase->a			= ltobs(pPara->a);			
			pAOParaBase->b			= ltobs(pPara->b);			
			pAOParaBase->c			= ltobs(pPara->c);			
			
			pPara++;
		}
		pAOParaBase++;

		/*��ʼ�����ݲ���*/
		pAODataBase->wStatus = AO_STATUS_ONLINE|AO_STATUS_RESET;
		pAODataBase++;
	}

	return DB_ERR_OK;
}

/*��ʼ�����ڲ���*/
STATUS InitSerialPara(TSerialPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TSerialPara			*pSerialPara;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nSerNum;
	
	/*��������ռ�*/
	pSerialPara = (TSerialPara *)OS_MemAlloc(sizeof(TSerialPara)*num);
	ASSERT(pSerialPara != NULL);
	if(pSerialPara == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pSerial	= pSerialPara;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			pSerialPara->wPortID 	= (SC_PORT_SERIAL<<8)+i+2;
			pSerialPara->bFlag 		= 0;
			pSerialPara->byBaud 	= 7;	//9600
			pSerialPara->byDriver 	= 1;	
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			pSerialPara->wPortID 	= ltobs(pPara->wPortID);
			pSerialPara->bFlag 		= pPara->bFlag;
			pSerialPara->byBaud 	= pPara->byBaud;
			pSerialPara->byDriver 	= pPara->byDriver;	
			pPara++;
		}

		pSerialPara++;
	}

	return DB_ERR_OK;
}

/*��ʼ��CANBUS����*/
STATUS InitCanBusPara(TCanBusPara	*pPara)
{
	WORD 				num,i;
	TDBaseConfigTable	*pDbase;
	TCanBusPara			*pCanBusPara;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nCanNum;

	/*��������ռ�*/
	pCanBusPara = (TCanBusPara *)OS_MemAlloc(sizeof(TCanBusPara)*num);
	ASSERT(pCanBusPara != NULL);
	if(pCanBusPara == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pCAN	= pCanBusPara;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			pCanBusPara->wPortID 	= (SC_PORT_CANBUS<<8)+i+1;
			pCanBusPara->wBaud		= 512;
			pCanBusPara->dwMask 	= MASK_ALL;
			pCanBusPara->byDriver 	= 0;	
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			pCanBusPara->wPortID 	= ltobs(pPara->wPortID);
			pCanBusPara->wBaud 		= ltobs(pPara->wBaud);
			pCanBusPara->dwMask 	= ltobl(pPara->dwMask);
			pCanBusPara->byDriver 	= pPara->byDriver;	
			
			pPara++;
		}
		pCanBusPara++;
	}

	return DB_ERR_OK;
}

/*��ʼ����̫������*/
STATUS InitEthernetPara(TEthPortPara	*pPara)
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	TEthPortPara			*pEthernetPara;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	num = pDbase->nEthNum;

	/*��������ռ�*/
	pEthernetPara = (TEthPortPara *)OS_MemAlloc(sizeof(TEthPortPara)*num);
	ASSERT(pEthernetPara != NULL);
	if(pEthernetPara == NULL)
		return DB_ERR_MEM_ALLOC;

	pDbase->pEthernet = pEthernetPara;

	/*��ʼ����������*/
	for(i=0;i<num;i++)
	{
		/*��ʼ��ȱʡ����*/
		if(pPara == NULL)
		{
			pEthernetPara->dwMask 		= MASK_ALL;
			switch(i)	
			{
				case 0:
					// A��
					pEthernetPara->wPortID		= (SC_PORT_ETHERNETA<<8)+1;
					pEthernetPara->wDriver 		= 4;	
					pEthernetPara->dwSideIP		= (192<<24)+(168<<16)+(100<<8)+177;	
					pEthernetPara->dwSideMask	= 0xFFFFFF00;	
					pEthernetPara->dwSidePort	= 0;	
					pEthernetPara->dwSelfIP		= (192<<24)+(168<<16)+(100<<8)+200;	
					pEthernetPara->dwSelfIP 	= G_tBasicConfig.dwNetAIP;	
					pEthernetPara->dwSelfMask	= G_tBasicConfig.dwNetAMask;
					pEthernetPara->dwMode		= 0;
					break;
				case 1:
					// A��
					pEthernetPara->wPortID		= (SC_PORT_ETHERNETA<<8)+2;
					pEthernetPara->wDriver 		= 3;	
					pEthernetPara->dwSideIP		= (192<<24)+(168<<16)+(100<<8)+177;	
					pEthernetPara->dwSideMask	= 0xFFFFFF00;	
					pEthernetPara->dwSidePort	= 0;	
					pEthernetPara->dwSelfIP		= (192<<24)+(168<<16)+(100<<8)+200;	
					pEthernetPara->dwSelfIP 	= G_tBasicConfig.dwNetAIP;	
					pEthernetPara->dwSelfMask	= G_tBasicConfig.dwNetAMask;
					pEthernetPara->dwMode		= 0;
					break;
					
				case 2:
					// B��
					pEthernetPara->wPortID		= (SC_PORT_ETHERNETB<<8)+1;
					pEthernetPara->wDriver 		= 4;	
					pEthernetPara->dwSideIP		= (192<<24)+(168<<16)+(116<<8)+177;	
					pEthernetPara->dwSideMask	= 0xFFFFFF00;	
					pEthernetPara->dwSidePort	= 0;	
					pEthernetPara->dwSelfIP		= (192<<24)+(168<<16)+(116<<8)+200;	
					pEthernetPara->dwSelfIP 	= G_tBasicConfig.dwNetBIP;	
					pEthernetPara->dwSelfMask	= G_tBasicConfig.dwNetBMask;
					pEthernetPara->dwMode		= 0;
					break;
					
				default:
					// B��
					pEthernetPara->wPortID		= (SC_PORT_ETHERNETB<<8)+i-1;
					pEthernetPara->wDriver		= 3;	
					pEthernetPara->dwSideIP 	= (192<<24)+(168<<16)+(116<<8)+177; 
					pEthernetPara->dwSideMask	= 0xFFFFFF00;	
					pEthernetPara->dwSidePort	= 0;	
					pEthernetPara->dwSelfIP 	= (192<<24)+(168<<16)+(116<<8)+200; 
					pEthernetPara->dwSelfIP 	= G_tBasicConfig.dwNetBIP;	
					pEthernetPara->dwSelfMask	= G_tBasicConfig.dwNetBMask;
					pEthernetPara->dwMode		= 0;
					break;
			}
			pEthernetPara->dwGateWay1	= (192<<24)+(168<<16)+(100<<8)+1;
			pEthernetPara->dwGateWay2	= (192<<24)+(168<<16)+(100<<8)+1;
			pEthernetPara->dwDHCPIP		= (192<<24)+(168<<16)+(100<<8)+1;
			strcpy(pEthernetPara->chUser1,"USER1");
			strcpy(pEthernetPara->chUser2,"USER2");
			strcpy(pEthernetPara->chUser3,"USER3");
			strcpy(pEthernetPara->chUser4,"USER4");
			
			strcpy(pEthernetPara->chPassword1,"KEYVIA");
			strcpy(pEthernetPara->chPassword2,"KEYVIA");
			strcpy(pEthernetPara->chPassword3,"KEYVIA");
			strcpy(pEthernetPara->chPassword4,"KEYVIA");
		}
		/*��ʼ��ʵ�ʲ���*/
		else
		{
			pEthernetPara->wPortID 		= ltobs(pPara->wPortID);
			pEthernetPara->wDriver 		= ltobs(pPara->wDriver);	
			pEthernetPara->dwMask 		= ltobl(pPara->dwMask);
			pEthernetPara->dwSideIP		= ltobl(pPara->dwSideIP);	
			pEthernetPara->dwSideMask	= ltobl(pPara->dwSideMask);	
			pEthernetPara->dwSidePort	= ltobl(pPara->dwSidePort);	
			pEthernetPara->dwSelfPort	= ltobl(pPara->dwSelfPort);	
			pEthernetPara->dwMode		= ltobl(pPara->dwMode);	
			pEthernetPara->dwGateWay2	= ltobl(pPara->dwGateWay2);
			pEthernetPara->dwDHCPIP		= ltobl(pPara->dwDHCPIP);
			
			memcpy(pEthernetPara->chUser1,pPara->chUser1,ETHLINK_MAXLEN_USERNAME);
			memcpy(pEthernetPara->chUser2,pPara->chUser2,ETHLINK_MAXLEN_USERNAME);
			memcpy(pEthernetPara->chUser3,pPara->chUser3,ETHLINK_MAXLEN_USERNAME);
			memcpy(pEthernetPara->chUser4,pPara->chUser4,ETHLINK_MAXLEN_USERNAME);
			
			memcpy(pEthernetPara->chPassword1,pPara->chPassword1,ETHLINK_MAXLEN_PASSWORD);
			memcpy(pEthernetPara->chPassword2,pPara->chPassword2,ETHLINK_MAXLEN_PASSWORD);
			memcpy(pEthernetPara->chPassword3,pPara->chPassword3,ETHLINK_MAXLEN_PASSWORD);
			memcpy(pEthernetPara->chPassword4,pPara->chPassword4,ETHLINK_MAXLEN_PASSWORD);
			
			// EEPROM��������ʱ��EEPROMΪ׼
			if(G_tBasicConfig.dwInitFlag == BAISIC_INIT_FLAG)
			{
				if(MAJOR_PORT(pEthernetPara->wPortID) == SC_PORT_ETHERNETA)
				{
					pEthernetPara->dwSelfIP		= G_tBasicConfig.dwNetAIP;	
					pEthernetPara->dwSelfMask	= G_tBasicConfig.dwNetAMask;	
					pEthernetPara->dwGateWay1	= G_tBasicConfig.dwNetAGWIP;
				}
				else
				{
					pEthernetPara->dwSelfIP		= G_tBasicConfig.dwNetBIP;	
					pEthernetPara->dwSelfMask	= G_tBasicConfig.dwNetBMask;	
					pEthernetPara->dwGateWay1	= G_tBasicConfig.dwNetBGWIP;
				}
			}
			else
			{
				// EEPROM����������ʱ�Բ���ˢ��֮
				if(MAJOR_PORT(pEthernetPara->wPortID) == SC_PORT_ETHERNETA)
				{
					G_tBasicConfig.dwNetAIP = pEthernetPara->dwSelfIP = ltobl(pPara->dwSelfIP);	
					G_tBasicConfig.dwNetAMask = pEthernetPara->dwSelfMask = ltobl(pPara->dwSelfMask);	
					G_tBasicConfig.dwNetAGWIP = pEthernetPara->dwGateWay1 = ltobl(pPara->dwGateWay1);
				}
				else
				{
					G_tBasicConfig.dwNetBIP = pEthernetPara->dwSelfIP = ltobl(pPara->dwSelfIP);	
					G_tBasicConfig.dwNetBMask = pEthernetPara->dwSelfMask = ltobl(pPara->dwSelfMask);	
					G_tBasicConfig.dwNetBGWIP = pEthernetPara->dwGateWay1 = ltobl(pPara->dwGateWay1);
				}
			}
						
			pPara++;
		}
		pEthernetPara++;
	}

	return DB_ERR_OK;
}

STATUS InitBurstBIWrap()
{
	TBurstBIWrap 		*pEventWrap;
	
	pEventWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;

	if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
	{
		//����NVRAM�ռ�
		pEventWrap = (TBurstBIWrap *)DAE_MemAlloc(sizeof(TBurstBIWrap));
		ASSERT(pEventWrap != NULL);
		if(pEventWrap == NULL)
			return DB_ERR_NVMEM_ALLOC;
	}	
	else
	{
		// ���WRAP״̬
		if((pEventWrap->dwMagic == WRAP_INIT_FLAG)&&(pEventWrap->nMax == MAX_BIE_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_BURSTBI_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_BURSTBI_ERR);
	}
	
	pEventWrap->dwMagic = WRAP_INIT_FLAG;
	pEventWrap->nMax = MAX_BIE_NUM;
	pEventWrap->nHeadIndex = 0;
	pEventWrap->nTailIndex = 0;
			
	G_pCfgTable->DBaseConfig.pBurstBIWrap = pEventWrap;

	LogError("InitBurstBIWrap",FILE_LINE,"Initial nvram malloc......");

	return DB_ERR_WRAP_INIT;
}

STATUS InitEventBIWrap()
{
	TFileMap				*pFileMap;
    TFileHead         		*filehead;
	TEventBIWrap			*pEventWrap;
	TBasicEventPtrList		*pBasicEventPtr;
	
	pEventWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;

	// 1����Ч��־��顢NvRAM�ڴ�������
	if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
	{
		// 2���ȷ���NVRAM�ռ�
		pEventWrap = (TEventBIWrap *)DAE_MemAlloc(sizeof(TEventBIWrap));
		ASSERT(pEventWrap != NULL);
		if(pEventWrap == NULL)
			return DB_ERR_NVMEM_ALLOC;		
	}
	else
	{
		if((pEventWrap->dwMagic == WRAP_INIT_FLAG)&&(pEventWrap->nMax == MAX_SOE_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_EVENTBI_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_EVENTBI_ERR);
	}

	// 3����ʼ��
	pEventWrap->dwMagic = WRAP_INIT_FLAG;
	pEventWrap->nMax = MAX_SOE_NUM;
	pEventWrap->nHeadIndex = 0;
	pEventWrap->nTailIndex = 0;
		
	// 4���ٴ�FLASH�лָ��¼���¼
	pFileMap = G_pCfgTable->DBaseConfig.pBasicEventFileMap;
	ASSERT(pFileMap != NULL);
	filehead = (TFileHead *)pFileMap->pRamData;
	pBasicEventPtr = (TBasicEventPtrList	*)(filehead+1);
		
	pEventWrap->nHeadIndex = pBasicEventPtr->nSOEH%MAX_SOE_NUM;
	pEventWrap->nTailIndex = pBasicEventPtr->nSOET%MAX_SOE_NUM;
	memcpy(pEventWrap->pEvent,(BYTE *)(pBasicEventPtr+1),sizeof(TEventBI)*MAX_SOE_NUM);
		
	G_pCfgTable->DBaseConfig.pEventBIWrap = pEventWrap;

	LogError("InitEventBIWrap",FILE_LINE,"Recover data from file......");
		
	return DB_ERR_OK;
}

STATUS InitEventAIWrap()
{
	TEventAIWrap			*pEventWrap;
	
	// 1���ȷ���NVRAM�ռ�
	pEventWrap = (TEventAIWrap *)OS_Malloc(sizeof(TEventAIWrap));
	ASSERT(pEventWrap != NULL);
	if(pEventWrap == NULL)
		return DB_ERR_NVMEM_ALLOC;

	// 2����ʼ��
	pEventWrap->dwMagic = WRAP_INIT_FLAG;
	pEventWrap->nMax = MAX_AIE_NUM;
	pEventWrap->nHeadIndex = 0;
	pEventWrap->nTailIndex = 0;
		
	G_pCfgTable->DBaseConfig.pEventAIWrap = pEventWrap;
	
	return DB_ERR_OK;
}

STATUS InitDefaultBasicBase()
{
	TDBaseConfigTable	*pDbase;
//	TMapDataList		*pMapData;
	
	pDbase = &G_pCfgTable->DBaseConfig;
//	pMapData = &pDbase->tMapData;

	//��ʼ��ȱʡ���ݿ����ñ�
	pDbase->wAddress	= G_tBasicConfig.wNetAddress;
	memcpy(pDbase->chPassWord1,G_tBasicConfig.chPassWord1,8);
	memcpy(pDbase->chPassWord2,G_tBasicConfig.chPassWord2,8);

	pDbase->wMainAddr 	= 0x00;
	pDbase->wType		= MD_DEFAULT;
	pDbase->wCodeCrc	= 0x55aa;
	pDbase->dwControl 	= 0xFFFF;

	pDbase->nDAI      	= DB_DEFAULT_DAINUM;
	pDbase->nDI      	= DB_DEFAULT_DINUM;	//����ʵң�š��ڲ�ң�ź���ң��
	pDbase->nAI       	= DB_DEFAULT_AINUM;
	pDbase->nBI       	= DB_DEFAULT_BINUM;
	pDbase->nCI       	= DB_DEFAULT_CINUM;
	pDbase->nBO       	= DB_DEFAULT_BONUM;
	pDbase->nAO       	= DB_DEFAULT_AONUM;

    pDbase->nSerNum 	= 1;
    pDbase->nCanNum     = 1;
    pDbase->nEthNum		= 4;
	
	//��ʼ���������
	InitDAIBase(NULL);	
	InitDIBase(NULL);	
	InitAIBase(NULL);	
	InitBIBase(NULL);	
	InitCIBase(NULL);	
	InitBOBase(NULL);	
	InitAOBase(NULL);	
	InitSerialPara(NULL);
	InitCanBusPara(NULL);
	InitEthernetPara(NULL);	

	//��ʼ������ָ��
	pDbase->pParaOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pParaOpt == NULL)
		return DB_ERR_MEM_ALLOC;

	/*
	//��ʼ������������ָ��
	pMapData->pAIAttr		= (TAIPara *)OS_Malloc(sizeof(TAIPara)*pDbase->nAI);
	pMapData->pBIAttr		= (TBIPara *)OS_Malloc(sizeof(TBIPara)*pDbase->nBI);
	pMapData->pCIAttr		= (TCIPara *)OS_Malloc(sizeof(TCIPara)*pDbase->nCI);
	pMapData->pDAIAttr		= (TDAIPara *)OS_Malloc(sizeof(TDAIPara)*pDbase->nDAI);
	pMapData->pDIAttr		= (TDIPara *)OS_Malloc(sizeof(TDIPara)*pDbase->nDI);
	pMapData->pBOAttr		= (TBOPara *)OS_Malloc(sizeof(TBOPara)*pDbase->nBO);
	pMapData->pSerial		= (TSerialPara *)OS_Malloc(sizeof(TSerialPara)*pDbase->nSerNum);
	pMapData->pCANBus		= (TCanBusPara *)OS_Malloc(sizeof(TCanBusPara)*pDbase->nCanNum);
	pMapData->pEthernet		= (TEthPortPara*)OS_Malloc(sizeof(TEthPortPara)*pDbase->nEthNum);
	*/
	return DB_ERR_OK;
}

STATUS InitBasicParaBase()
{
    TFileHead         	*filehead;
    TParaConfigTable	*prealbaseconf;
	TDBaseConfigTable	*pDbase;
	TMapDataList		*pMapData;
    TDAIPara   			*prealdai;
    TAIPara         	*prealai;
    TDIPara   			*prealdi;
    TBIPara         	*prealbi;
    TCIPara         	*prealci;
    TBOPara         	*prealbo;
    TAOPara         	*prealao;
	TSerialPara			*pSerial;
	TCanBusPara			*pCanBus;
	TEthPortPara		*pEthPort;
    DWORD 				rc;


	pDbase = &G_pCfgTable->DBaseConfig;
	
	//��ʼ�����ݿ�ָ��
	pDbase->pAI = NULL;
	pDbase->pRM = NULL;
	pDbase->pBI = NULL;
	pDbase->pBCD = NULL;
	pDbase->pSPI = NULL;
	pDbase->pAO = NULL;
	pDbase->pBO = NULL;
	pDbase->pTQ = NULL;
	pDbase->pParaOpt = NULL;
	pDbase->pDAIPara= NULL;
	pDbase->pDIPara= NULL;
	pDbase->pAIAttr= NULL;
	pDbase->pBIAttr= NULL;
	pDbase->pCIAttr= NULL;
	pDbase->pBOAttr= NULL;
	pDbase->pAOAttr= NULL;
	pDbase->pSerial= NULL;
	pDbase->pCAN= NULL;
	pDbase->pEthernet= NULL;

	memset(&pDbase->tMapData,0,sizeof(TMapDataList));
	memset(&pDbase->tModData,0,sizeof(TMapDataList));

	// 1 ����ļ���ʼ��ʧ�ܣ�����ȱʡ������ʼ��
	if(DAE_GetInitFlags(SS_FILE_BASICPARA_ERR))
	{
		// 2 ��ȡ�ļ�����
		filehead = (TFileHead *)ParaConfInit;
	    prealbaseconf = (TParaConfigTable *)( filehead + 1 );

		// ���ȱʡ�ļ��Ϸ���
	    if( DAE_CheckSystemFile(ParaConfInit,filehead->dwSize) != DB_ERR_OK)
	    {
	        LogError("InitBasicParaBase",FILE_LINE,"default paraconf.dar file check error!");
			InitDefaultBasicBase();
			return DB_ERR_FILE_CHECK;
	    }
		else
		{
			LogError("InitBasicParaBase",FILE_LINE,"From default paraconf.dar file init......");
		}
	}
	else
	{
		// 2 ��ȡ�ļ�����
		filehead = (TFileHead *)pDbase->pBasicParaFileMap->pRamData;
	    prealbaseconf = (TParaConfigTable *)( filehead + 1 );
	}

	// 3.��ʼ��������
	pDbase->dwDevID		= ltobl(prealbaseconf->dwDevID);
	memcpy(pDbase->chHostName, prealbaseconf->chHostName, HOST_NAME_LEN);
	pDbase->wMainAddr 	= ltobs(prealbaseconf->wMainAddr);
	pDbase->dwControl 	= ltobl(prealbaseconf->dwControl);
	pDbase->wType		= ltobs(prealbaseconf->wTYPE);
	pDbase->wCodeCrc	= ltobs(prealbaseconf->wCodeCrc);
	pDbase->nDAI      	= ltobs(prealbaseconf->nDAI);
	pDbase->nDI      	= ltobs(prealbaseconf->nDI);
	pDbase->nAI       	= ltobs(prealbaseconf->nAI);
	pDbase->nBI       	= ltobs(prealbaseconf->nBI);
	pDbase->nCI       	= ltobs(prealbaseconf->nCI);
	pDbase->nBO       	= ltobs(prealbaseconf->nBO);
	pDbase->nAO       	= ltobs(prealbaseconf->nAO);
    pDbase->nSerNum 	= ltobs(prealbaseconf->nSerNum);
    pDbase->nCanNum     = ltobs(prealbaseconf->nCanNum);
    pDbase->nEthNum		= ltobs(prealbaseconf->nEthNum);
	pDbase->nLanguageNo = ltobs(prealbaseconf->nLanguageNo);

	// ��EEPROM��ȡ
	if(G_tBasicConfig.dwInitFlag == BAISIC_INIT_FLAG)
	{
		pDbase->wAddress  	= G_tBasicConfig.wNetAddress;
		memcpy(pDbase->chPassWord1,G_tBasicConfig.chPassWord1,8);
		memcpy(pDbase->chPassWord2,G_tBasicConfig.chPassWord2,8);
	}
	else
	{
		pDbase->wAddress  	= ltobs(prealbaseconf->wAddress);
		memcpy(pDbase->chPassWord1,prealbaseconf->chPassWord1,8);
		memcpy(pDbase->chPassWord2,prealbaseconf->chPassWord2,8);

		G_tBasicConfig.wNetAddress = pDbase->wAddress;
		memcpy(G_tBasicConfig.chPassWord1,pDbase->chPassWord1,8);
		memcpy(G_tBasicConfig.chPassWord2,pDbase->chPassWord2,8);
	}
		
	/* ����ʣ�µĲ����ļ� */
    prealdai= (TDAIPara	*)( prealbaseconf + 1 );
    prealdi = (TDIPara*)( prealdai  + pDbase->nDAI);
    prealai = (TAIPara*)( prealdi + pDbase->nDI);
    prealbi = (TBIPara*)( prealai  + pDbase->nAI);
    prealci = (TCIPara*)( prealbi  + pDbase->nBI);
    prealbo = (TBOPara*)( prealci  + pDbase->nCI);
    prealao = (TAOPara*)( prealbo  + pDbase->nBO);
    pSerial = (TSerialPara*)(prealao+pDbase->nAO);
	pCanBus = (TCanBusPara*)(pSerial+pDbase->nSerNum);
	pEthPort= (TEthPortPara*)(pCanBus+pDbase->nCanNum);
	
	// ��̬���ݲ���
	InitDAIBase(prealdai);
	InitDIBase(prealdi);
	InitAIBase(prealai);
	InitBIBase(prealbi);
	InitCIBase(prealci);

    // ���������ݲ���
	InitBOBase(prealbo);
	InitAOBase(prealao);
                                
    // �˿ڲ���
	InitSerialPara(pSerial);
	InitCanBusPara(pCanBus);
	InitEthernetPara(pEthPort);

	//ע���������
	if(!DAE_GetInitFlags(SS_FILE_BASICPARA_ERR))
	{
		pMapData = &pDbase->tMapData;
		pMapData->pConfig 	= prealbaseconf;
		pMapData->pDAIAttr	= prealdai;
		pMapData->pAIAttr	= prealai;
		pMapData->pDIAttr 	= prealdi;
		pMapData->pBIAttr 	= prealbi;
		pMapData->pCIAttr 	= prealci;
		pMapData->pBOAttr 	= prealbo;
		pMapData->pAOAttr 	= prealao;
		pMapData->pSerial 	= pSerial;
		pMapData->pCANBus 	= pCanBus;
		pMapData->pEthernet	= pEthPort;
	}
	
	//��ʼ������ָ��
	pDbase->pParaOpt = (TRunTimeOPT *)OS_MemAlloc(sizeof(TRunTimeOPT));
	if(pDbase->pParaOpt == NULL)
		return DB_ERR_MEM_ALLOC;

	return DB_ERR_OK;
}

STATUS MakeBasicEventFile(void *pFileBuf)
{
	TFileMap				*pFileMap;
    TFileHead         		*filehead;
	TEventBIWrap			*pBIWrap;
	TMiscWrap 				*pMiscWrap;
	TBasicEventPtrList		*pBasicEventPtr;

	if(pFileBuf == NULL)
		return DB_ERR_PTRNULL;
	
	//��ʼ�������¼��ļ�
	filehead 	= (TFileHead *)pFileBuf;
	memset(filehead,0,sizeof(TFileHead));
	filehead->dwSize = sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
				+sizeof(TEventBI)*MAX_SOE_NUM\
				+sizeof(TMiscEvent)*MAX_MISC_NUM\
				+sizeof(WORD);
		
	filehead->wVersion = 0x0100;
	filehead->wFlag    = 0x8000;	//BigEnden�洢
	filehead->dwMagic  = FILE_MAGIC_INIT;
	ReadSystemTime(&filehead->stTime);
		
	//��ʼ��ָ���¼Ϊ0
	memset(filehead+1,0,sizeof(TBasicEventPtrList));

	if((!DAE_GetInitFlags(SS_NVRAM_LINK_ERR))\
		&&(!DAE_GetInitFlags(SS_NVRAM_INIT)))\
	{
		pBasicEventPtr = (TBasicEventPtrList	*)(filehead+1);
		
		// �ָ�SOE�¼�
		pBIWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
		if((pBIWrap->dwMagic == WRAP_INIT_FLAG)\
			&&(pBIWrap->nMax == MAX_SOE_NUM))
		{
			pBasicEventPtr->nSOEH = pBIWrap->nHeadIndex%MAX_SOE_NUM;
			pBasicEventPtr->nSOET = pBIWrap->nTailIndex%MAX_SOE_NUM;
			memcpy((BYTE *)(pBasicEventPtr+1),pBIWrap->pEvent,sizeof(TEventBI)*MAX_SOE_NUM);
		}
		
		// �ָ������¼�
		pMiscWrap = &G_pCfgTable->MiscWrap;
		if((pMiscWrap->dwMagic == WRAP_INIT_FLAG)\
		&&(pMiscWrap->nMax == MAX_MISC_NUM))
		{
			pBasicEventPtr->nOptRecordH = pMiscWrap->nHeadIndex%MAX_MISC_NUM;
			pBasicEventPtr->nOptRecordT = pMiscWrap->nTailIndex%MAX_MISC_NUM;
			memcpy((BYTE *)(pBasicEventPtr+1)+sizeof(TEventBI)*MAX_SOE_NUM,pMiscWrap->pEvent,sizeof(TMiscEvent)*MAX_MISC_NUM);
		}
	}

	DAE_MakeCheckSum((BYTE *)filehead,sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead,filehead->dwSize-sizeof(WORD));

	DAE_WriteFile(BASICEVENTFILE, pFileBuf,filehead->dwSize);

	return DB_ERR_OK;
}

STATUS MakeRelayEventFile(void *pFileBuf)
{
    TFileHead         		*filehead;
	TRelayAlarmWrap			*pAlmWrap;
	TRelayActionWrap		*pActWrap;
	TRelayEventPtrList		*pRelayEventPtr;
	
	if(pFileBuf == NULL)
		return DB_ERR_PTRNULL;
	
	//��ʼ�������¼��ļ�
	filehead	= (TFileHead *)pFileBuf;
	memset(filehead,0,sizeof(TFileHead));
	filehead->dwSize  = sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
					+sizeof(TRelayActionEv)*MAX_RACT_NUM\
					+sizeof(TRelayAlarmEv)*MAX_RALM_NUM+sizeof(WORD);
					
	filehead->wVersion = 0x0100;
	filehead->wFlag    = 0x8000;	//BigEnden�洢
	filehead->dwMagic  = FILE_MAGIC_INIT;
	ReadSystemTime(&filehead->stTime);
	
	//��ʼ��ָ���¼Ϊ0
	memset(filehead+1,0,sizeof(TRelayEventPtrList));
	pRelayEventPtr = (TRelayEventPtrList	*)(filehead+1);
	
	// 1����Ч��־��顢NvRAM�ڴ�������
	if((!DAE_GetInitFlags(SS_NVRAM_LINK_ERR))\
		&&(!DAE_GetInitFlags(SS_NVRAM_INIT)))\
	{
		pRelayEventPtr->nMaxRalayAction = MAX_RACT_NUM;
		pRelayEventPtr->nMaxRalayAlarm = MAX_RACT_NUM;
		
		// �ָ������¼�
		pActWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
		if((pActWrap->dwMagic == WRAP_INIT_FLAG)\
			&&(pActWrap->nMax == MAX_RACT_NUM))
		{
			pRelayEventPtr->nRelayActionH = pActWrap->nHeadIndex%MAX_RACT_NUM;
			pRelayEventPtr->nRelayActionT = pActWrap->nTailIndex%MAX_RACT_NUM;
			memcpy((BYTE *)(pRelayEventPtr+1),pActWrap->pEvent,sizeof(TRelayActionEv)*MAX_RACT_NUM);
		}
		
		// �ָ��澯�¼�
		pAlmWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
		if((pAlmWrap->dwMagic == WRAP_INIT_FLAG)\
		&&(pAlmWrap->nMax == MAX_RALM_NUM))
		{
			pRelayEventPtr->nRelayAlarmH = pAlmWrap->nHeadIndex%MAX_RALM_NUM;
			pRelayEventPtr->nRelayAlarmT = pAlmWrap->nTailIndex%MAX_RALM_NUM;
			memcpy((BYTE *)(pRelayEventPtr+1)+sizeof(TRelayActionEv)*MAX_RACT_NUM,pAlmWrap->pEvent,sizeof(TRelayAlarmEv)*MAX_RALM_NUM);	
		}
		LogError("MakeRelayEventFile",FILE_LINE,"Init eeprom relay event file from NvRAM!!!");
	}
	else
	{
		pRelayEventPtr = (TRelayEventPtrList	*)(filehead+1);
		pRelayEventPtr->nMaxRalayAction = MAX_RACT_NUM;
		pRelayEventPtr->nRelayActionH = 0;
		pRelayEventPtr->nRelayActionT = 0;

		pRelayEventPtr->nMaxRalayAlarm = MAX_RALM_NUM;
		pRelayEventPtr->nRelayAlarmH = 0;
		pRelayEventPtr->nRelayAlarmT = 0;

		LogError("MakeRelayEventFile",FILE_LINE,"Clean eeprom relay event file!!!");
	}
	
	DAE_MakeCheckSum((BYTE *)filehead,sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead,filehead->dwSize-sizeof(WORD));
		
	DAE_WriteFile(RELAYEVENTFILE, pFileBuf,filehead->dwSize);

//	LogError("MakeRelayEventFile",FILE_LINE,"Save event file! Addr=0x%08x Len=%d Check=%2x%2x",\
//		EEPROM_RELAY_EVENT,filehead->dwSize,*((BYTE*)pFileBuf+filehead->dwSize-2),*((BYTE*)pFileBuf+filehead->dwSize-1));
	
	return DB_ERR_OK;
}

STATUS InitEventDataBase()
{
    TFileHead         		*filehead;
	BYTE					*pFileBuf = NULL;
    DWORD 					filelength;
	static STATUS 			rc;

	// 1����ʼ�����������ļ�
	if(DAE_GetInitFlags(SS_FILE_RELAYEVENT_ERR))
	{
		// 1.1 ����������ʧ��ʱֱ�ӷ���
		if(G_pCfgTable->DBaseConfig.pRelayEventFileMap ==  NULL)
			return DB_ERR_MEM_ALLOC;

		// 1.2 ȡ�ļ�����ָ��
		pFileBuf = (BYTE *)G_pCfgTable->DBaseConfig.pRelayEventFileMap->pRamData;

		// 1.3 ���������ļ�
		MakeRelayEventFile(pFileBuf);
	}


	// 2����ʼ����ͨ�����ļ�
	if(DAE_GetInitFlags(SS_FILE_BASICEVENT_ERR))
	{
		// 2.1 ����������ʧ��ʱֱ�ӷ���
		if(G_pCfgTable->DBaseConfig.pBasicEventFileMap ==  NULL)
			return DB_ERR_MEM_ALLOC;

		// 2.2 ȡ�ļ�����ָ��
		pFileBuf = (BYTE *)G_pCfgTable->DBaseConfig.pBasicEventFileMap->pRamData;
		
		// 2.3 ���������ļ�
		MakeBasicEventFile(pFileBuf);
	}
	
	// 1����ʼ��ң�ű�λ�����
	rc = InitBurstBIWrap();

	// 2����ʼ��SOE�����
	rc = InitEventBIWrap();

	// 3����ʼ���������������
	rc = InitRelayStartWrap();

	// 4����ʼ�������澯�����
	rc = InitRelayAlarmWrap();

	// 5����ʼ���������������
	rc = InitRelayActionWrap();

	// 6����ʼ�������������
	rc = InitRelayActionCache();

	// 7����ʼ������¼����������
	rc = InitRelayDistLink();

	// 8����ʼ���仯ң���¼�������
	rc = InitEventAIWrap();
	
	return DB_ERR_OK;
}

STATUS InitOnlineDataBase()
{
	TMapDataList		*pModData;
	TDBaseConfigTable	*pDbase;

	pDbase = &G_pCfgTable->DBaseConfig;
	pModData = &pDbase->tModData;

	pModData->pConfig = (TParaConfigTable *)OS_MemAlloc(sizeof(TParaConfigTable));
	if(pModData->pConfig == NULL)
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for Para Config Table error!");
	}
	
	// 1��ֱ��ģ���������������
	pModData->pDAIAttr	= (TDAIPara *)OS_MemAlloc(pDbase->nDAI*sizeof(TDAIPara));
	if((pModData->pDAIAttr == NULL)&&(pDbase->nDAI!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for DAI modify base error!");
	}
	// 2��ֱ�������������������
	pModData->pDIAttr	= (TDIPara *)OS_MemAlloc(pDbase->nDI*sizeof(TDIPara));
	if((pModData->pDIAttr == NULL)&&(pDbase->nDI!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for DI modify base error!");
	}

	// 3��ң�������
	pModData->pAIAttr	= (TAIPara *)OS_MemAlloc(pDbase->nAI*sizeof(TAIPara));
	if((pModData->pAIAttr == NULL)&&(pDbase->nAI!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for AI modify base error!");
	}

	// 4��ң�Ų�����
	pModData->pBIAttr	= (TBIPara *)OS_MemAlloc(pDbase->nBI*sizeof(TBIPara));
	if((pModData->pBIAttr == NULL)&&(pDbase->nBI!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for BI modify base error!");
	}
	
	// 3����Ȳ�����
	pModData->pCIAttr	= (TCIPara *)OS_MemAlloc(pDbase->nCI*sizeof(TCIPara));
	if((pModData->pCIAttr == NULL)&&(pDbase->nCI!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for CI modify base error!");
	}

	// 4��ң�ز�����
	pModData->pBOAttr	= (TBOPara *)OS_MemAlloc(pDbase->nBO*sizeof(TBOPara));
	if((pModData->pBOAttr == NULL)&&(pDbase->nBO!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for BO modify base error!");
	}

	// 5��ң��������
	pModData->pAOAttr	= (TAOPara *)OS_MemAlloc(pDbase->nAO*sizeof(TAOPara));
	if((pModData->pAOAttr == NULL)&&(pDbase->nAO!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for AO modify base error!");
	}

	// 6�����ڲ�����
	pModData->pSerial	= (TSerialPara *)OS_MemAlloc(pDbase->nSerNum*sizeof(TSerialPara));
	if((pModData->pSerial == NULL)&&(pDbase->nSerNum!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for Serial modify para error!");
	}

	// 7��CAN��������
	pModData->pCANBus  = (TCanBusPara *)OS_MemAlloc(pDbase->nCanNum*sizeof(TCanBusPara));
	if((pModData->pCANBus == NULL)&&(pDbase->nCanNum!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for CanBus modify para error!");
	}

	// 8����̫��������
	pModData->pEthernet = (TEthPortPara *)OS_MemAlloc(pDbase->nEthNum*sizeof(TEthPortPara));
	if((pModData->pEthernet == NULL)&&(pDbase->nEthNum!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for Ehernet modify base error!");
	}

	// 9����ѹ��������
	pModData->pSWData = (DWORD *)OS_MemAlloc(pDbase->nSW*sizeof(DWORD));
	if((pModData->pSWData == NULL)&&(pDbase->nSW!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for SW base error!");
	}

	// 11����ֵ������
	pModData->pSETData = (float *)OS_MemAlloc(pDbase->nSET*sizeof(float));
	if((pModData->pSETData == NULL)&&(pDbase->nSET!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for SET base error!");
	}

	// 12��������ֵ������
	pModData->pSystemSETData = (float *)OS_MemAlloc(pDbase->nSystemSET*sizeof(float));
	if((pModData->pSystemSETData == NULL)&&(pDbase->nSystemSET!=0))
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for System SET base error!");
	}

	// 13��ϵ��������
	pModData->pCFData = (long *)OS_MemAlloc((pDbase->nCF+1)*sizeof(long));
	if(pModData->pCFData == NULL)
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate memory for CF base error!");
	}

	// 14�������������ñ�
	pModData->pRelayCfg = (TRelayDataTable *)OS_MemAlloc(sizeof(TRelayDataTable));
	if(pModData->pRelayCfg == NULL)
	{
        LogError("InitOnlineDataBase",FILE_LINE,"Allocate Relay Config Table error!");
	}

	return DB_ERR_OK;
}


