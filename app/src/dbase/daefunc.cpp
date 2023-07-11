/********************************************************************************************************/
/*																										*/
/*        						��򿭷������ɷ����޹�˾												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      Daefunc.cpp                                     1.0												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ��������л�����صĽӿں���																*/
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
/*	DB_GetRunRecordNum()			��ȡ���м�¼��������δ��ȡ���ݸ���									*/
/*	DB_IsRunRecordEmpty()			���м�¼�������Ƿ�Ϊ��												*/
/*	DB_GetRunRecordHead()			��ȡ���м�¼������ͷָ��											*/
/*	DB_GetRunRecordRead()			��ȡ���м�¼��������ָ��											*/
/*	DB_SetRunRecordRead()			�������м�¼��������ָ��											*/
/*	DB_IncRunRecordRead()			�������м�¼��������ָ��											*/
/*	ReportMsg()						����������Ϣ������������Ϣ�ʹ����¼								*/
/*	LogError()						�Ǽ��쳣��Ϣ�������쳣��Ϣ����,�ļ���,�к�,��ǰ����,ʱ��,�쳣��Ϣ��	*/
/*	DB_ReadLogInfo()				��ȡ���м�¼														*/
/*	DB_Misc_GetNum()				��ȡ�����¼���������δ��ȡ�¼����ݸ���								*/
/*	DB_Misc_IsEmpty()				�����¼��������Ƿ�Ϊ��												*/
/*	DB_Misc_GetHead()				��ȡ�����¼�������ͷָ��											*/
/*	DB_Misc_GetRead()				��ȡ�����¼���������ָ��											*/
/*	DB_Misc_SetRead()				���ò����¼���������ָ��											*/
/*	DB_Misc_IncRead()				���������¼���������ָ��											*/
/*	DB_Misc_Read()					�Ӳ����¼��������ж�һ�������¼�����								*/
/*	DB_Misc_Write()					������¼���������д��һ�������¼�����								*/
/*	DB_Register_Reset ()			ע��һ��ϵͳ��λ�¼�												*/
/*	DB_Register_FileDown ()			ע��һ���ļ���װ�¼�												*/
/*	DB_Register_Comm ()				ע��һ��ͨ���¼�													*/
/*	DB_Register_BO()				ע��һ��ң���¼�													*/
/*	DB_Register_AO()				ע��һ��ң���¼�													*/
/*	DB_Register_SETTING()			ע��һ���޸Ķ�ֵ�¼�												*/
/*	DB_Register_SETArea ()			ע��һ���޸ĵ�ǰ��ֵ�����¼�										*/
/*	DB_Register_ParaChange()		ע��һ���޸Ĳ����¼�												*/
/*	DB_Register_PowerOnOff()		ע��һ���޸Ĳ����¼�												*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*		Daefunc.h						ϵͳ��������													*/
/*		Db_err.h						���ݿⷵ��ֵ													*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0												*/
/********************************************************************************************************/
#include "Daefunc.h"	
#include "Dbfunc.h"		
#include "parafunc.h"		
#include "Db_err.h"
#include "portdefs.h"
#include "Aa1024.h"
#include "Relaydef.h"

#include "serial.h"
#include "misi.h" 
#include "csocket.h"
#include "netentry.h"
#include "canlink.h"

extern TTransWrap		g_GLBTransWrap;
extern TBasicConfig		G_tBasicConfig;
extern	TSysConfigTable	*G_pCfgTable;
extern	DWORD dwRelayTaskID;
extern HANDLE g_hRelayTaskID;


STATUS DAE_GetFlashFileLength(char *strFileName,DWORD *dwSize);
STATUS DAE_GetEEPROMFileLength(DWORD dwAddr,DWORD *dwFileLength);

STATUS DAE_ReadEEPROMFile(DWORD dwAddr,void *pFile,DWORD dwFileLen);
STATUS DAE_WriteEEPROMFile(DWORD dwAddr,void *pFile,DWORD dwFileLen);

STATUS DAE_ReadFlashFile(char *strFileName,void *pFile,DWORD dwFileLen);
STATUS DAE_WriteFlashFile(char *strFileName,void *pFile,DWORD dwFileLen);


WORD DB_GetAppNum()
{
	return G_pCfgTable->DBaseConfig.nTaskAPP;
}

static	HANDLE G_HAPPCACHE = 0;
static	TAppInfo *G_PAPPCACHE = NULL;

TAppInfo *DB_GetAppInfo()
{
    TAppInfo 	*pAppInfo;
    DWORD 		app;

    pAppInfo = G_pCfgTable->TaskApp;
    app = RtosGetCurrentTaskId();

	//����Ѱ��ʱ�Ŀ��ٲ���
	if(app == G_HAPPCACHE)
		return G_PAPPCACHE;

	//���������
	G_HAPPCACHE = app;
	G_PAPPCACHE = NULL;

    for(int i=0; i<DAE_MAX_APP_NUM; i++ )
    {
		if( pAppInfo[i].hTaskID == app )
		{
			//��������ָ��
			G_PAPPCACHE = &pAppInfo[i];
			break;
		}
    }
	
	return G_PAPPCACHE;
}

STATUS  DB_SetEventFlag(TAppInfo *pApp,DWORD dwFlag)
{
 	if(pApp == NULL)
 	{
	    pApp = DB_GetAppInfo();
		if(pApp == NULL)
			return DB_ERR_NOSUPPORT;
 	}

	pApp->bEvent |= dwFlag;
	
	return DB_ERR_OK;
}

STATUS  DB_ClearEventFlag(TAppInfo	*pApp,DWORD	dwFlag)
{
 	if(pApp == NULL)
 	{
	    pApp = DB_GetAppInfo();
		if(pApp == NULL)
			return DB_ERR_NOSUPPORT;
 	}

	pApp->bEvent &= ~dwFlag;
	
	return DB_ERR_OK;
}

BOOL  DB_GetEventFlag(TAppInfo *pApp,DWORD dwFlag)
{
 	if(pApp == NULL)
 	{
	    pApp = DB_GetAppInfo();
		if(pApp == NULL)
			return FALSE;
 	}

	return (pApp->bEvent & dwFlag);
}

/************************************************************
	���ܣ�ȡ�豸���ͺ�
	��������
	���أ�װ�����͡�
************************************************************/
WORD DB_GetDevType()
{
	return G_pCfgTable->DBaseConfig.wType;
}

/************************************************************
	���ܣ�ȡ�����CRC��
	��������
	���أ�װ�����͡�
************************************************************/
WORD DB_GetCodeCrc()
{
	return G_pCfgTable->DBaseConfig.wCodeCrc;
}

/************************************************************
	���ܣ�ȡ�豸��ID
	��������
	���أ�װ�����͡�
************************************************************/
DWORD DB_GetDevID()
{
	return G_pCfgTable->DBaseConfig.dwDevID;
}


/************************************************************
	���ܣ�ȡ�豸��ID
	��������
	���أ�װ�����͡�
************************************************************/
DWORD DB_GetDevAddress()
{
	return G_pCfgTable->DBaseConfig.wAddress;
}

/************************************************************
	���ܣ�ȡ��λ����ַ
	��������
	���أ���λ����ַ��
************************************************************/
DWORD DB_GetMainAddress()
{
	return G_pCfgTable->DBaseConfig.wMainAddr;
}

/*
void DAE_SetRunningFlags(DWORD FlagNo)
{	
	DWORD DwordNo; 	
	DWORD BitSign; 	

	DWORD *pFlag=(DWORD *)&G_pCfgTable->SystemStatus.Status;//
	
    DwordNo 	= FlagNo/32;
	BitSign  	= (0x01<<(FlagNo%32));

	pFlag[DwordNo] |= BitSign;
}

void DAE_ClearRunningFlags(DWORD FlagNo)
{	
	DWORD DwordNo; 	
	DWORD BitSign; 	

	DWORD *pFlag=(DWORD *)&G_pCfgTable->SystemStatus.Status;//
	
    DwordNo 	= FlagNo/32;
	BitSign  	= (0x01<<(FlagNo%32));

	pFlag[DwordNo] &= ~BitSign;
}

BOOL DAE_GetRunningFlags(DWORD FlagNo)
{	
	DWORD DwordNo; 	
	DWORD BitSign; 	

	DWORD *pFlag=(DWORD *)&G_pCfgTable->SystemStatus.Status;//
	
    DwordNo 	= FlagNo/32;
	BitSign  	= (0x01<<(FlagNo%32));

	return pFlag[DwordNo]&BitSign;
}

void DAE_SetInitFlags(DWORD FlagNo)
{	
	DWORD DwordNo; 	
	DWORD BitSign; 	

	DWORD *pFlag=(DWORD *)&G_pCfgTable->SystemStatus.InitStatus;//
	
    DwordNo 	= FlagNo/32;
	BitSign  	= (0x01<<(FlagNo%32));

	pFlag[DwordNo] |= BitSign;
}

void DAE_ClearInitFlags(DWORD FlagNo)
{	
	DWORD DwordNo; 	
	DWORD BitSign; 	

	DWORD *pFlag=(DWORD *)&G_pCfgTable->SystemStatus.InitStatus;//
	
    DwordNo 	= FlagNo/32;
	BitSign  	= (0x01<<(FlagNo%32));

	pFlag[DwordNo] &= ~BitSign;
	
}

BOOL DAE_GetInitFlags(DWORD FlagNo)
{	
	DWORD DwordNo; 	
	DWORD BitSign; 	

	DWORD *pFlag=(DWORD *)&G_pCfgTable->SystemStatus.InitStatus;//
	
    DwordNo 	= FlagNo/32;
	BitSign  	= (0x01<<(FlagNo%32));

	return pFlag[DwordNo]&BitSign;
}
*/

WORD DAE_CheckSum(BYTE *pBuf,DWORD nLen)
{
	WORD	wTmp = 0x55AA;
	DWORD	i;
	
	if(pBuf == NULL)
		return 0;

	for(i=0; i < nLen; i++)
	{
		wTmp += pBuf[i];
	}
		
	return ~wTmp;
}

BOOL DAE_IsCheckSumOK(BYTE *pBuf,DWORD nLen)
{
	WORD	wCheck;

	if(pBuf == NULL)
		return FALSE;

	wCheck = MAKEWORD(pBuf[nLen],pBuf[nLen+1]);

	if( DAE_CheckSum(pBuf,nLen) != wCheck )
		return FALSE;
	else
		return TRUE;
}

BOOL DAE_MakeCheckSum(BYTE *pBuf,DWORD nLen)
{
	WORD	wCheck;

	if(pBuf == NULL)
		return FALSE;

	wCheck = DAE_CheckSum(pBuf,nLen);

	pBuf[nLen] = LOBYTE(wCheck);
	pBuf[nLen+1] = HIBYTE(wCheck);

	return TRUE;
}

BOOL DAE_NvRAMAddrCheck(void *Addr)
{
	if(Addr==NULL)
	{
		return FALSE;
	}
	
	if((DWORD)Addr<DBASE_NVRAM_BASE)
	{
		return FALSE;
	}
	
	if((DWORD)Addr>=DBASE_NVRAM_END)
	{
		return FALSE;
	}
	
	return TRUE; 
}

void 	DAE_InitNvMem( void )
{
	TMemLink 	*NvRamLink;
	TMCB		*pAlloced;				//�ѷ����ڴ�ָ��	

	NvRamLink = &G_pCfgTable->NvRamLink;

	//��ʼ���ڴ���		
	NvRamLink->pFree = (LPBYTE)(DBASE_NVRAM_BASE+ALIGN(sizeof(TSysConfigTable),ALIGNMENT));
	NvRamLink->dwSize = DBASE_NVRAM_LENGTH-ALIGN(sizeof(TSysConfigTable),ALIGNMENT);
	NvRamLink->dwSign = MMLNK_SIGN_VALID;	

	//����һ���սڵ�
	pAlloced = NvRamLink->pAlloced = (TMCB*)NvRamLink->pFree;
	pAlloced->dwSign = MCB_SIGN_ALLOC;
	pAlloced->dwSize = 0;

	//��������ָ��
	NvRamLink->dwSize -= sizeof(TMCB);
	NvRamLink->pFree += sizeof(TMCB);
	
}

void*	DAE_MemAlloc(DWORD size)
{
    PBYTE 		buf;
    TMCB 		*pmcb;
    
	if(size==0)
		return NULL;
	
    //����Ӧ�����������ڴ泤��ΪALIGNMENT�ı���
    size = ALIGN(size,ALIGNMENT);
	
    //ȡ����NVRAM�Ĺ���ڵ�
	TMemLink	*pNvRamLink = &G_pCfgTable->NvRamLink;				//��̬�ڴ����

    ASSERT(pNvRamLink != NULL);
    ASSERT(size != 0);

    //���������ڴ泤��Ϊ��
	if((size == 0)||(pNvRamLink == NULL))
		return NULL;
		
    //���ݿ�����ڴ����δ��ʼ��
    if(pNvRamLink->dwSign != MMLNK_SIGN_VALID)
        return NULL;
    	
    //�жϿ���RAM�����Ƿ�����������ĳ��Ȳ���ϵͳ����NVRAM�Ƿ�����
    if( pNvRamLink->dwSize >= (size+sizeof(TMCB)) ) /*&& !(status & STATUS_NVRAM_ERROR))*/
    {
        buf = pNvRamLink->pFree;
        pmcb = (TMCB *)(buf-sizeof(TMCB));

	    pmcb->dwSign = MCB_SIGN_ALLOC;
        pmcb->dwSize = size;
        pNvRamLink->dwSize -= (size+sizeof(TMCB));
        pNvRamLink->pFree += (size+sizeof(TMCB));

	    //��¼��������һ����¼
	    pmcb = (TMCB *PFAR)(buf+size);
	    pmcb->dwSign = MCB_SIGN_FREE;
	    pmcb->dwSize = 0;
        
    }
    else
    {
		buf = (BYTE*)OS_MemAlloc(size);
    }

	if(buf==NULL)
		return NULL;
	
    while(size--) *(buf+size) = 0;
	    return buf;
}

BOOL DAE_CheckNvMalloc()
{
	TMCB			*pMCB;

	// 1���������ָ��Ϸ���
	if(!DAE_NvRAMAddrCheck(G_pCfgTable->DBaseConfig.pBurstBIWrap))
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"BurstBIWrap error!!!");
		return FALSE;
	}
	
	if(!DAE_NvRAMAddrCheck(G_pCfgTable->DBaseConfig.pEventBIWrap))
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"EventBIWrap error!!!");
		return FALSE;
	}
	
	if(!DAE_NvRAMAddrCheck(G_pCfgTable->DBaseConfig.pRSTTWrap))
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Relay start report wrap error!!!");
		return FALSE;
	}
	
	if(!DAE_NvRAMAddrCheck(G_pCfgTable->DBaseConfig.pRACTWrap))
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Relay action report wrap error!!!");
		return FALSE;
	}
	
	if(!DAE_NvRAMAddrCheck(G_pCfgTable->DBaseConfig.pRALMWrap))
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Relay alarm report wrap error!!!");
		return FALSE;
	}
	
	if(!DAE_NvRAMAddrCheck(G_pCfgTable->NvRamLink.pAlloced))
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Naram alloced ptr error!!!");
		return FALSE;
	}
	
	// 2��������ݹ�ģ�仯
	if(G_pCfgTable->DBaseConfig.pBurstBIWrap->nMax != MAX_BIE_NUM)
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"BurstBIWrap size change!!!");
		return FALSE;
	}
	
	if(G_pCfgTable->DBaseConfig.pEventBIWrap->nMax != MAX_SOE_NUM)
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"EventBIWrap size change!!!");
		return FALSE;
	}
	
	if(G_pCfgTable->DBaseConfig.pRSTTWrap->nMax != MAX_RSTT_NUM)
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Relay start report wrap size change!!!");
		return FALSE;
	}
	
	if(G_pCfgTable->DBaseConfig.pRACTWrap->nMax != MAX_RACT_NUM)
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Relay action report wrap size change!!!");
		return FALSE;
	}
	
	if(G_pCfgTable->DBaseConfig.pRALMWrap->nMax != MAX_RALM_NUM)
	{
		LogError("DAE_CheckNvMalloc",FILE_LINE,"Relay alarm report wrap size change!!!");
		return FALSE;
	}
	
	pMCB = G_pCfgTable->NvRamLink.pAlloced;
	
	// 3������ڴ���
	while(pMCB->dwSize != 0)
	{		
		if((pMCB->dwSign != MCB_SIGN_ALLOC) && (pMCB->dwSign != MCB_SIGN_FREE))
			return FALSE;

		//��󳤶ȼ�� 128Kz
		if(pMCB->dwSize>0x20000)
			return FALSE;

		//��󳤶ȼ�� 128K
		if((pMCB->dwSize + sizeof(TMCB))>0x20000)
			return FALSE;
		
		if(((DWORD)pMCB + pMCB->dwSize + sizeof(TMCB))>(DBASE_NVRAM_BASE+DBASE_NVRAM_LENGTH))
			return FALSE;
		
		pMCB = (TMCB *)((PBYTE)pMCB + sizeof(TMCB) + pMCB->dwSize);
	}
	return TRUE;
}

BOOL DAE_CheckHard()
{
	// 1��NvRAM���
	// 2��FLASH���
	return TRUE;
}

BOOL DAE_CheckSysStatus()
{
	TStatus				*pSystemStatus;	

	pSystemStatus = &G_pCfgTable->SystemStatus;

	//�������״̬��Ч��־
	if(pSystemStatus->dwFlag != SYS_RUNNING_OK)
		return FALSE;

	/*��Ᵽ���澯�¼�ָ��Ϸ���*/
//	if(pSystemStatus->nRelayAlarmT > MAX_RALM_LENGTH)
//		return FALSE;

	/*��Ᵽ�������¼�ָ��Ϸ���*/
//	if(pSystemStatus->nRelayEventT > MAX_RACT_LENGTH)
//		return FALSE;

	return TRUE;
}

BOOL DAE_CheckMISCWrap()
{
	TMiscWrap *pMisc;

	pMisc = &G_pCfgTable->MiscWrap;

	if(pMisc->dwMagic != WRAP_INIT_FLAG)
		return FALSE;

	return TRUE;
}

BOOL DAE_CheckRecordWrap()
{
	TRecordWrap *pRecord;

	pRecord = &G_pCfgTable->RecordWrap;

	if(pRecord->dwMagic != WRAP_INIT_FLAG)
		return FALSE;

	return TRUE;
}

BOOL CheckSystemState()
{
	WORD	bStaus = TRUE;
	
	// 1�����Ӳ��
	if(!DAE_CheckHard())
	{
		DAE_SetRunningFlags(SS_SYS_HARD_ERR);		
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_SYS_HARD_ERR);		
	}
	
	// 2���������״̬
	if(!DAE_CheckSysStatus())
	{
		DAE_SetRunningFlags(SS_SYS_STATUS_ERR);		
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_SYS_STATUS_ERR);		
	}
		
	// 3�����NvRAM�ڴ���
	if(!DAE_CheckNvMalloc())		
	{
		DAE_SetRunningFlags(SS_NVRAM_LINK_ERR);
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_NVRAM_LINK_ERR);		
	}

	// 4����������¼��
	if(!DAE_CheckMISCWrap())		
	{
		DAE_SetRunningFlags(SS_NVRAM_MISC_ERR);
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_NVRAM_MISC_ERR);		
	}

	// 5��������м�¼��
	if(!DAE_CheckRecordWrap())		
	{
		DAE_SetRunningFlags(SS_NVRAM_RECORD_ERR);
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_NVRAM_RECORD_ERR);		
	}

	// 6�������ѹ��
	if(!DB_CheckSWData(DB_GetSWBase(),DB_GetSWNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_SW_ERR))
		{
			//����ʧ��ֱ�Ӹ澯�����ٸ���
			if(DB_UpdateSWBase() != DB_ERR_OK)
			{
				LogError("CheckSystemState", FILE_LINE, "SW Self Check Error!");
				DAE_SetRunningFlags(SS_EEPROM_SW_ERR);
				bStaus = FALSE;
			}
		}
		DAE_SetRunningFlags(SS_RAM_SW_ERR);		
	}
	else
	{
		DAE_ClearRunningFlags(SS_RAM_SW_ERR);		
	}
	
	// 7����鶨ֵ
	if(!DB_CheckSETData(DB_GetSETFloatBase(),DB_GetSETNum())||\
		!DB_CheckSETFixData(DB_GetSETFixBase(),DB_GetSETNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_SET_ERR))
		{
			//����ʧ��ֱ�Ӹ澯�����ٸ���
			if(DB_UpdateSETFloatBase() != DB_ERR_OK)
			{
				LogError("CheckSystemState", FILE_LINE, "SET Self Check Error!");
				DAE_SetRunningFlags(SS_EEPROM_SET_ERR);
				bStaus = FALSE;
			}
		}
		DAE_SetRunningFlags(SS_RAM_SET_ERR);		
	}
	else
	{
		DAE_ClearRunningFlags(SS_RAM_SET_ERR);		
	}

	// 8�����ϵͳ��ֵ
	if(!DB_CheckSETData(DB_GetSystemSETBase(),DB_GetSystemSETNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_SYSSET_ERR))
		{
			//����ʧ��ֱ�Ӹ澯�����ٸ���
			if(DB_UpdateSystemSETBase() != DB_ERR_OK)
			{
				LogError("CheckSystemState", FILE_LINE, "SyetemSET Self Check Error!");
				DAE_SetRunningFlags(SS_EEPROM_SYSSET_ERR);
				bStaus = FALSE;
			}
		}
		DAE_SetRunningFlags(SS_RAM_SYSSET_ERR);		
	}
	else
	{
		DAE_ClearRunningFlags(SS_RAM_SYSSET_ERR);		
	}
	
	// 9�����ϵ��
	if(!DB_CheckCFData(DB_GetCFBase(),DB_GetCFNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_CF_ERR))
		{
			//����ʧ��ֱ�Ӹ澯�����ٸ���
			if(DB_UpdateCFBase() != DB_ERR_OK)
			{
				LogError("CheckSystemState", FILE_LINE, "CF Self Check Error!");
				bStaus = FALSE;
				DAE_SetRunningFlags(SS_EEPROM_CF_ERR);
			}
		}
		DAE_SetRunningFlags(SS_RAM_CF_ERR);		
	}
	else
	{
		DAE_ClearRunningFlags(SS_RAM_CF_ERR);		
	}


	//�����жϳ����¼�
	if(DAE_GetRunningFlags(SS_EEPROM_SW_ERR)||DAE_GetRunningFlags(SS_EEPROM_SET_ERR)\
		||DAE_GetRunningFlags(SS_EEPROM_SYSSET_ERR)||DAE_GetRunningFlags(SS_EEPROM_CF_ERR)\
		||DAE_GetRunningFlags(SS_EEPROM_SETAREA_ERR))
	{
		DAE_SetRunningFlags(SS_EEPROM_DATA_ERR);
	}
	else
	{
		DAE_ClearRunningFlags(SS_EEPROM_DATA_ERR);
	}

	//�����жϳ����¼�
	if(DAE_GetRunningFlags(SS_RAM_SW_ERR)||DAE_GetRunningFlags(SS_RAM_SET_ERR)\
		||DAE_GetRunningFlags(SS_RAM_SYSSET_ERR)||DAE_GetRunningFlags(SS_RAM_CF_ERR))
	{
		DAE_SetRunningFlags(SS_RAM_DATA_ERR);
	}
	else
	{
		DAE_ClearRunningFlags(SS_RAM_DATA_ERR);
	}
	
	return bStaus;
}

STATUS EEPROM_Read(DWORD Addr,DWORD nSize,void *pBuf)
{
	if(EepromRead((BYTE *)pBuf, nSize, Addr) == ERR_SUCCESS)
		return DB_ERR_OK;
	else
		return DB_ERR_EEPROM;
}

STATUS EEPROM_Write(DWORD Addr,DWORD nSize,void *pBuf)
{
	if(EEPROMWrite((BYTE *)pBuf, nSize, Addr) == ERR_SUCCESS)
		return DB_ERR_OK;
	else
	{
		DAE_SetRunningFlags(SS_EEPROM_WRITE_ERR);
		return DB_ERR_EEPROM;
	}
}

STATUS DAE_GetFlashFileLength(char *strFileName,DWORD *dwSize)
{
	if(FS_GetFileLength(strFileName, dwSize) == TRUE)
		return DB_ERR_OK;
	else
		return DB_ERR_FILE_READ;
}

STATUS DAE_ReadFlashFile(char *strFileName,void *pFile,DWORD dwFileLen)
{
    DWORD 				filelength;
	DWORD				rc;

	rc = FS_ReadFile(strFileName, 0,dwFileLen,(BYTE *)pFile, &filelength,0);

	if((rc == FILE_OK)||(rc == FILE_END))
		return DB_ERR_OK;
	else
		return DB_ERR_FILE_WRITE;
}
//2012-5-9 ��̬ˢ���߼������� from z
STATUS DAE_WriteFlashFile(char *strFileName,void *pFile,DWORD dwFileLen)
{
	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;

	//����Ӧ�ó����ļ���д��ǰ����������ݵ������Լ��
	if( (!strcmp( strFileName, "vxworks" )) || (!strcmp( strFileName, "vxWorks")) )
	{
		if(DB_CodeFileIntegrityCheck((BYTE*)pFile, dwFileLen) != ERR_SUCCESS)
			return DB_ERR_FILE_WRITE;
	}
	
	if(FS_WriteFile( strFileName, 0,dwFileLen,(BYTE *)pFile,FILE_HEAD|FILE_END,0) != FILE_ERROR)
	{
		//���޸����߼��ļ������֪ͨ�����������³�ʼ���߼�
		if( !strcmp( strFileName, "logic.plc" ) )
		{
			//OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, 0, 0, 0);
			OS_PostMessage(g_hRelayTaskID, SM_LOGICCHANGED, 0, 0, 0, 0);
			ReportMsg("dae download ok");
		}
		//Ӧ�ó���������ɺ��Զ�������
		if( (!strcmp( strFileName, "vxworks" )) || (!strcmp( strFileName, "vxWorks")) )
		{
			DAE_ResetDevice(SYS_RESET_COLD);
		}
		return DB_ERR_OK;
	}
	else
		return DB_ERR_FILE_WRITE;
}
/*
STATUS DAE_WriteFlashFile(char *strFileName,void *pFile,DWORD dwFileLen)
{
	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;

	if(FS_WriteFile( strFileName, 0,dwFileLen,(BYTE *)pFile,FILE_HEAD|FILE_END,0) != FILE_ERROR)
		return DB_ERR_OK;
	else
		return DB_ERR_FILE_WRITE;
}*/

STATUS DAE_GetEEPROMFileLength(DWORD dwAddr,DWORD *dwFileLength)
{
    TFileHead         	filehead;
    DWORD 				rc;

	//�ȴ�EEPROM�ж����ļ�ͷ
	rc = EEPROM_Read(dwAddr, sizeof(TFileHead), &filehead);
	if(rc != DB_ERR_OK)
	{
        return DB_ERR_FILE_READ;
	}

	//����ļ�ͷУ�鳤��
	if(!DAE_IsCheckSumOK((BYTE *)&filehead, sizeof(TFileHead)-sizeof(WORD)))
	{
        return DB_ERR_CHECK;
	}

	*dwFileLength = filehead.dwSize;
		
	return DB_ERR_OK;
}

STATUS DAE_ReadEEPROMFile(DWORD dwAddr,void *pFile,DWORD dwFileLen)
{
    TFileHead         	*filehead;
    DWORD 				rc,size;

	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;

	//ֱ�Ӷ�������
	return EEPROM_Read(dwAddr, dwFileLen, pFile);
/*	
	//�ȴ�EEPROM�ж����ļ�ͷ
	rc = EEPROM_Read(dwAddr, sizeof(TFileHead), pFile);
	if(rc != DB_ERR_OK)
	{
        return DB_ERR_FILE_READ;
	}

	//����ļ�ͷУ�鳤��
	filehead = (TFileHead *)pFile;
	if(!DAE_IsCheckSumOK((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD)))
	{
        return DB_ERR_CHECK;
	}

	//����ļ���У��
	size = filehead->dwSize;
		
	rc = EEPROM_Read(dwAddr, size, pFile);
	if(rc != DB_ERR_OK)
	{
        return DB_ERR_FILE_READ;
	}
	
	if(!DAE_IsCheckSumOK((BYTE *)pFile, size-sizeof(WORD)))
	{
		return DB_ERR_CHECK;
	}
	return DB_ERR_OK;
*/
}

STATUS DAE_WriteEEPROMFile(DWORD dwAddr,void *pFile,DWORD dwFileLen)
{	
	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;
	
	return EEPROM_Write(dwAddr, dwFileLen, pFile);
}

void _FormatFileName(char *strFileName)
{
	for(int i=0;i<strlen(strFileName);i++)
		if((strFileName[i]>='A')&&(strFileName[i]<='Z'))
			strFileName[i] = strFileName[i]-'A'+'a';
}

STATUS DAE_GetFileLength(char *strFileName,DWORD *dwSize)
{
	_FormatFileName(strFileName);
	
	if(strcmp(strFileName,RELAYDATAFILE)==0)
	{
		//return DAE_GetEEPROMFileLength(EEPROM_RELAY_DATA,dwSize);
		*dwSize  = sizeof(TFileHead)+sizeof(TRelayDataTable);
		*dwSize += (SW_COUNT+31)/32*2*sizeof(DWORD);
		*dwSize += (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		*dwSize += (AS_COUNT+1)*sizeof(float);
		*dwSize += (CF_COUNT+1)*2*sizeof(long)+sizeof(WORD);
							
		return DB_ERR_OK;
	}
	else if(strcmp(strFileName,RELAYEVENTFILE)==0)
	{
		//return DAE_GetEEPROMFileLength(EEPROM_RELAY_EVENT,dwSize);		
		*dwSize = sizeof(TFileHead)+sizeof(TRelayEventPtrList);
		*dwSize += sizeof(TRelayActionEv)*MAX_RACT_NUM;
		*dwSize += sizeof(TRelayAlarmEv)*MAX_RALM_NUM+sizeof(WORD);

		return DB_ERR_OK;
	}
	else	
	{
		return DAE_GetFlashFileLength(strFileName,dwSize);
	}
}

STATUS DAE_CheckSystemFile(BYTE *pFile,DWORD dwLen)
{
	TFileHead			*filehead;
	
	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;
	
	filehead = (TFileHead *)pFile;
		
	//����ļ�ͷУ�鳤��
	if(!DAE_IsCheckSumOK((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD)))
	{
		return DB_ERR_CHECK;
	}
	
	//����ļ���У��		
	if(!DAE_IsCheckSumOK((BYTE *)pFile, dwLen-sizeof(WORD)))
	{
		return DB_ERR_CHECK;
	}
	
	return DB_ERR_OK;
}

STATUS DAE_ReadFile(char *strFileName,BYTE *pFile,DWORD dwFileLen)
{
	_FormatFileName(strFileName);
	
	//  ���ݴ洢����ȡ
	if(strcmp(strFileName,RELAYDATAFILE)==0)
	{
	//	return DAE_ReadEEPROMFile(EEPROM_RELAY_DATA,pFile,dwFileLen);
		if(EEPROM_Read(EEPROM_RELAY_DATA, dwFileLen, pFile) != DB_ERR_OK)
		{
			return DB_ERR_FILE_READ;
		}
	}
	else if(strcmp(strFileName,RELAYEVENTFILE)==0)
	{
	//	return DAE_ReadEEPROMFile(EEPROM_RELAY_EVENT,pFile,dwFileLen);
		if(EEPROM_Read(EEPROM_RELAY_EVENT, dwFileLen, pFile) != DB_ERR_OK)
		{
			return DB_ERR_FILE_READ;
		}
	}
	else	
	
	{
		return DAE_ReadFlashFile(strFileName,pFile,dwFileLen);
	}
}

STATUS DAE_WriteFile(char *strFileName,void *pFile,DWORD dwFileLen)
{
	STATUS rc = DB_ERR_OK;
	
	_FormatFileName(strFileName);
	
	if(strcmp(strFileName,RELAYDATAFILE)==0)
	{
//		rc = DAE_WriteEEPROMFile(EEPROM_RELAY_DATA,pFile,dwFileLen);
		EEPROM_Write(EEPROM_RELAY_DATA,dwFileLen,pFile);
	}
	else if(strcmp(strFileName,RELAYEVENTFILE)==0)
	{
//		rc = DAE_WriteEEPROMFile(EEPROM_RELAY_EVENT,pFile,dwFileLen);
		EEPROM_Write(EEPROM_RELAY_EVENT,dwFileLen,pFile);
	}
	else
	{
		rc = DAE_WriteFlashFile(strFileName,pFile,dwFileLen);
	}

	if(rc != DB_ERR_OK)
		LogError("DAE_WriteFile",FILE_LINE,"\n Error on write File %s!<=====",strFileName);

	return rc;
}

/************************************************************
	���ܣ���ȡ���м�¼��������δ��ȡ���ݸ�����
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_GetRunRecordNum(WORD *retval)
{
	TRecordWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = &G_pCfgTable->RecordWrap;

    // 3����ȡ��ָ��    
    rc = DB_GetRunRecordRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_RUNREC_NUM-rp)%MAX_RUNREC_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	���ܣ����м�¼�������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_IsRunRecordEmpty()
{
	TRecordWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����ȡ�����ָ��             
    pWrap = &G_pCfgTable->RecordWrap;

    // 2����ȡ��ָ��    
    rc = DB_GetRunRecordRead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	���ܣ������ݿ��ж�ȡһ�����м�¼��Ϣ
	������	rp   	--  ��ȡ�����õ�ָ��,��BLANKʱ���ڲ���ָ��
			*event	--	���ص�����ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_ReadRunRecord(TRecordBlock *event, DWORD rp )
{
    TRecordWrap		*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = &G_pCfgTable->RecordWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		pAppInfo = DB_GetAppInfo();
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nLogRead%MAX_RUNREC_NUM;
    }
    else
    {
		rp = rp%MAX_RUNREC_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*event = pWrap->pEvent[rp];
                                     
	return DB_ERR_OK;
}
		
/************************************************************
	���ܣ������м�¼����д��һ�����м�¼���ݡ�
	������*str,		--  ���м�¼�ַ���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_WriteRunRecord(char *str)
{
    TRecordWrap		*pWrap;
	TAppInfo		*pAppInfo;
    WORD 			i, size;
                     
	// 1���������ָ��ĺϷ���
	if( str == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	if(G_pCfgTable == NULL)
	{
		printf("%s",str);
		return DB_ERR_BASENULL;
	}
	
	pWrap = &G_pCfgTable->RecordWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	//����Ӧ�ڴ���Ƿ��ʼ��
	if(pWrap->dwMagic != WRAP_INIT_FLAG )
		return DB_ERR_WRAP_INIT;
	
	// 3��������
    size = strlen(str);
	if(size >= ES_MAX_LINE)
		size = ES_MAX_LINE;	
    pWrap->pEvent[pWrap->nTailIndex].nCharSize = size;
    memcpy(pWrap->pEvent[pWrap->nTailIndex].szRecordMsg,str,size);
	
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_RUNREC_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RUNREC_NUM;
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++)
	{
	    if( pAppInfo[i].nLogRead == pWrap->nTailIndex )
	         pAppInfo[i].nLogRead = (pWrap->nTailIndex+1)%MAX_RUNREC_NUM;
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ���м�¼������ͷָ�롣
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordHead(WORD *retval)
{
	TRecordWrap		*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = &G_pCfgTable->RecordWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ���м�¼��������ָ�롣
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordRead(WORD *retval)
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
    *retval = pAppInfo->nLogRead%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ���м�¼������βָ�롣
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordTail(WORD *retval)
{
	TRecordWrap		*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = &G_pCfgTable->RecordWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ��������м�¼��������ָ�롣
	������	rp   	-- �����Ķ�ָ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetRunRecordRead(WORD rp)
{
	TAppInfo	*pAppInfo;

    // ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // ������������ָ��
    pAppInfo->nLogRead = rp%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ��������м�¼��������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_IncRunRecordRead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nLogRead = (pAppInfo->nLogRead+1)%MAX_RUNREC_NUM;
		
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����¼���������δ��ȡ�¼����ݸ�����
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_Misc_GetNum(WORD *retval)
{
	TMiscWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = &G_pCfgTable->MiscWrap;

    // 3����ȡ��ָ��    
    rc = DB_Misc_GetRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4�������������    
    *retval = (pWrap->nTailIndex+MAX_MISC_NUM-rp)%MAX_MISC_NUM;
    
    return DB_ERR_OK;   
}

/************************************************************
	���ܣ������¼��������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_Misc_IsEmpty()
{
	TMiscWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1����ȡ�����ָ��             
    pWrap = &G_pCfgTable->MiscWrap;

    // 2����ȡ��ָ��    
    rc = DB_Misc_GetRead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	���ܣ���ȡ�����¼�������ͷָ�롣
	��������
	���أ��¼�������ͷָ�롣
************************************************************/
STATUS DB_Misc_GetHead(WORD *retval)
{
	TMiscWrap		*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = &G_pCfgTable->MiscWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_MISC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����¼�������βָ�롣
	��������
	���أ��¼�������βָ�롣
************************************************************/
STATUS DB_Misc_GetTail(WORD *retval)
{
	TMiscWrap		*pWrap;

    // 1����鷵������ָ��Ϸ���            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2����ȡ�����ָ��             
    pWrap = &G_pCfgTable->MiscWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_MISC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ�����¼���������ָ�롣
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_Misc_GetRead(WORD *retval)
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
    *retval = pAppInfo->nMISCRead%MAX_MISC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	���ܣ����ò����¼���������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_SetRead(WORD rp)
{
	TAppInfo	*pAppInfo;

    // ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // ������������ָ��
    pAppInfo->nMISCRead = rp%MAX_MISC_NUM;

	return DB_ERR_OK;
}
/************************************************************
	���ܣ����������¼���������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_IncRead()
{
	TAppInfo		*pAppInfo;

    // 1��ȡ��ǰ����
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2������ָ��
    pAppInfo->nMISCRead = (pAppInfo->nMISCRead+1)%MAX_MISC_NUM;

//	DB_RegisterFileChange(DB_FILE_BASICEV);
		
	return DB_ERR_OK;}

/************************************************************
	���ܣ��Ӳ����¼��������ж�һ�������¼����ݡ�
	������rp	    --  ���
		data,		--  �����¼���¼
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_Read(DWORD rp, TMiscEvent *pEvent)
{
    TMiscWrap 		*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1���������ָ��ĺϷ���
    if( pEvent == NULL )
        return DB_ERR_BUFNULL;
                     
    // 2����ȡ�����ָ��
    pWrap = &G_pCfgTable->MiscWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	
	// 3������Чָ�룬ֱ�Ӷ�ȡ��ǰָ������ݣ������ȡָ��ָ������
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nMISCRead%MAX_MISC_NUM;
    }
    else
    {
		rp = rp%MAX_MISC_NUM;
    }
                       
	// 4����ȡ������Ϣ
	*pEvent= pWrap->pEvent[rp];
	                                     
	return DB_ERR_OK;
}
		
/************************************************************
	���ܣ�������¼���������д��һ�������¼����ݡ�
	������data,		--  �����¼���¼
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_Write(TMiscEvent *pEvent)
{
    TMiscWrap		*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1���������ָ��ĺϷ���
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2����ȡ�����ָ��
	pWrap = &G_pCfgTable->MiscWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3��������
    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_MISC_NUM;

	// 4�����������ͷָ��
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_MISC_NUM;
	
	// 5�������������ָ��,��ע��������Ϣ
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++)
	{
	    if( pAppInfo[i].nMISCRead == pWrap->nTailIndex )
	         pAppInfo[i].nMISCRead = (pWrap->nTailIndex+1)%MAX_MISC_NUM;
	}

	DB_RegisterFileChange(DB_FILE_BASICEV);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ��ϵͳ��λ�¼���
	������status	-- ��λ��ʽ   0 ���ȸ�λ 1���临λ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_Reset (DWORD status)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_RESET;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator = DAE_GetCurrentTaskPortId();
	tMisc.dwValue = status;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ���ļ���װ�¼���
	������strFileName 	-- �ļ���
		   Flag			-- �ļ��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_FileDown (char *strFileName,DWORD Flag)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_DOWNFILE;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator = DAE_GetCurrentTaskPortId();
	if(strcmp(strFileName,PARACFGFILE)==0)
	{
		tMisc.dwValue = 1;
	}
	else if(strcmp(strFileName,RELAYDATAFILE)==0)
	{
		tMisc.dwValue = 2;
	}
	else if(strcmp(strFileName,RELAYDISPFILE)==0)
	{
		tMisc.dwValue = 3;
	}
	else if(strcmp(strFileName,"vxworks")==0)
	{
		tMisc.dwValue = 4;
	}
	else
		tMisc.dwValue = 0xFF;
	
	tMisc.dwAttr = Flag;

	DB_Misc_Write(&tMisc);
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ��ͨ���¼���
	������	status		-- 0��ͨ�Żָ� 1��ͨ��ʧ��
			dwPortID	ͨ���ն˵Ķ˿�
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_Comm (DWORD status,DWORD dwPortID)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_COMM;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= status;
	tMisc.dwAttr	= dwPortID;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ��ң���¼���
	������ 	no,  		-- ң�غ�
			attr		-- �������� 0���� 1���� 2����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_BO(WORD no, WORD attr)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_BO;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= no;
	tMisc.dwAttr	= attr;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ��ң���¼���
	������ 	no,  		-- ң����
			lValue		-- ���ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_AO(WORD no, long  lValue)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_AO;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= no;
	tMisc.dwAttr	= lValue;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ���޸Ķ�ֵ�¼���
	������ 	AreaNo,		-- ��ֵ����
			SETNo	-- ��ֵ���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_SET(WORD AreaNo,WORD SETNo)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_SET;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= MAKELONG(SETNo,AreaNo);

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ���޸ĵ�ǰ��ֵ�����¼���
	������ 	AreaNo,		-- ��ֵ����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_SETArea (WORD AreaNo)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_SETAREA;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= AreaNo;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ���޸Ĳ����¼���
	������ 	ParaType,   -- �������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_ParaChange (DWORD ParaType,WORD no)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_PARA;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= ParaType;
	tMisc.dwAttr	= no;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ���޸Ĳ����¼���
	������ 	Status,		-- 0���ϵ� 1��ͣ��
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_PowerOnOff (DWORD Status)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_POWER;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= Status;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ����ѹ��Ͷ���¼���
	������no,    		--  ��ѹ�����
		  Status	    --  ��ѹ��״̬	
						Ͷ��SW_STATUS_SET���У�SW_STATUS_CLR
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_SW (WORD no,BYTE Status)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_SW;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= no;
	tMisc.dwAttr	= Status;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ��������Ͷ���¼���
	������no,    		--  ���������
		  Status	    --  ��ѹ��״̬	
							Ͷ��CFG_STATUS_SET���У�CFG_STATUS_CLR
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_CFG (WORD no,BYTE Status)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_CFG;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= no;
	tMisc.dwAttr	= Status;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

/************************************************************
	���ܣ�ע��һ��ϵ���޸��¼���
	������no,    		--  ϵ�����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_CF (WORD no)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= MISC_CLASS_CF;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= no;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}


/************************************************************
	���ܣ�ע��һ������������
	������no,    		--  ֲ�����
	������dwValue,    	--  ֲ����ֵ
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Register_SetData (WORD Type,WORD no,DWORD dwValue)
{
	TMiscEvent	tMisc;

	ReadAbsTime(&tMisc.absTime);
	tMisc.nClass 	= Type;
	tMisc.dwDevName = DB_GetDevID();
	tMisc.dwOperator= DAE_GetCurrentTaskPortId();
	tMisc.dwValue 	= no;
	tMisc.dwAttr	= dwValue;

	DB_Misc_Write(&tMisc);

	return DB_ERR_OK;
}

STATUS DAE_Portid2AppInfo(WORD wPortID,TAppInfo *pDesApp)
{
	TAppInfo 				*pAppInfo;
	int 					i;
		
	pAppInfo =  G_pCfgTable->TaskApp;

	for(i=0;i<DAE_MAX_APP_NUM;i++)
	{
		if((pAppInfo[i].dwPortID == wPortID)&&(pAppInfo[i].hTaskID != APP_TASKID_NULL))
		{
			*pDesApp = pAppInfo[i];
			return DB_ERR_OK;
		}
	}

	return DB_ERR_ID;
}

HANDLE DAE_GetTaskID(WORD wPortID)
{
	TAppInfo tApp;
	
	if(DAE_Portid2AppInfo(wPortID,&tApp) == DB_ERR_OK)
		return tApp.hTaskID;
	else
		return APP_TASKID_NULL;
}

DWORD	DAE_GetNetADataMask()
{
	return G_pCfgTable->SystemStatus.dwNetAMask;
}

DWORD	DAE_GetNetBDataMask()
{
	return G_pCfgTable->SystemStatus.dwNetBMask;
}

void	DAE_SetNetADataMask(DWORD dwMask)
{
	G_pCfgTable->SystemStatus.dwNetAMask = dwMask;
}

void	DAE_SetNetBDataMask(DWORD dwMask)
{
	G_pCfgTable->SystemStatus.dwNetBMask = dwMask;
}

WORD	DAE_GetCurrentTaskPortId()
{
	OS_TaskDesp 			*pTask;

	pTask = RtosGetTask(SELF_TASK_ID);
	if(pTask == NULL)
		return 0xFFFF;
	else
		return pTask->Args[1];
}

//ȡ�˿�����
STATUS DB_GetPortName(WORD wPort,char *strName)
{
	ASSERT(strName != NULL);
	if(strName == NULL)
	{
		return DB_ERR_PTRNULL;
	}

	strName[0] = '\0';
	
	if(MAJOR_PORT(wPort) == SC_PORT_SYSTEM)
	{
		switch(wPort)
		{
		case SYS_PORTID_AI:		strcat(strName,"SYS_AI"); break;
		case SYS_PORTID_DI:		strcat(strName,"SYS_DI"); break;
		case SYS_PORTID_BO:		strcat(strName,"SYS_BO"); break;
		case SYS_PORTID_LCD:	strcat(strName,"SYS_LCD"); break;
		case SYS_PORTID_DBASE:	strcat(strName,"SYS_DBASE"); break;
		case SYS_PORTID_SERADMIN:	strcat(strName,"SYS_SER"); break;
		case SYS_PORTID_CANADMIN:	strcat(strName,"SYS_CAN"); break;
		case SYS_PORTID_ETHMAIN:	strcat(strName,"SYS_ETH"); break;
		case SYS_PORTID_WATCHDOG:	strcat(strName,"SYS_WD"); break;
		case SYS_PORTID_RELAY:	strcat(strName,"SYS_RELAY"); break;
		case SYS_PORTID_BACKUP:	strcat(strName,"SYS_BACKUP"); break;
		case SYS_PORTID_MAPBIT:	strcat(strName,"SYS_MAPBIT"); break;
		default:	strcat(strName,"SYS_UNKNOW"); break;
		}
	}
	else
	{
		switch(MAJOR_PORT(wPort))
		{
		case SC_PORT_SERIAL:	sprintf(strName,"SER%d",MINOR_PORT(wPort)); break;
		case SC_PORT_CANBUS:	sprintf(strName,"CAN%d",MINOR_PORT(wPort)); break;
		case SC_PORT_ETHERNETA:	sprintf(strName,"ETHA%d",MINOR_PORT(wPort)); break;
		case SC_PORT_ETHERNETB:	sprintf(strName,"ETHB%d",MINOR_PORT(wPort)); break;
		default:	strcat(strName,"COM_UNKNOW"); break;
		}
	}

	return DB_ERR_OK;
}

//��ȡӦ�ó����ļ��汾��Ϣ
//Ϊ��������ͬʱ��������ݲ�ͬ�����⣬ֻ���в������ݵ�У�飬У�鳤�ȼ�¼���û���Ϣ��Res[0]�У���ǰ�İ汾��û�и�����
STATUS	DB_GetCodeFileVerInf(TCodeFileInf *pFileInf, DWORD *pCrcLen)
{
	DWORD len;
	BYTE data[4];
	DWORD UserInfAdd;

	//���ȴ��ļ���ʼ�ĵ�12�ֽ�λ�ö�ȡ�ļ��汾��Ϣ���ļ��еĵ�ַ
	if(FS_ReadFile("vxWorks", 12, 4, data, &len, 0) == FILE_ERROR)
	{
		ReportMsg("Read the vxworks file head error");
		return ERR_FAILURE;
	}

	//�����ַ�����С��˳�����
	UserInfAdd = MAKEDWORD(data[0], data[1], data[2], data[3]);

	//Ӧ�ó����С��Ч��飬����512K
	if((UserInfAdd < LEAST_CODE_FILE_LEN) || (UserInfAdd > MOST_CODE_FILE_LEN))
	{
		ReportMsg("The vxworks file head information invalid.");
		return ERR_FAILURE;
	}

	if(FS_ReadFile("vxWorks", UserInfAdd, sizeof(TCodeFileInf), (BYTE*)pFileInf, &len, 0) == FILE_ERROR)
	{
		ReportMsg("Read the vxworks file version information error");
		return ERR_FAILURE;
	}

	//����˳��
	pFileInf->wCheckSum = MAKEWORD(((BYTE)(pFileInf->wCheckSum>>8)), ((BYTE)pFileInf->wCheckSum));

	pFileInf->CompileTime.Year = MAKEWORD((BYTE)(pFileInf->CompileTime.Year>>8), (BYTE)pFileInf->CompileTime.Year);
	pFileInf->EditTime.Year = MAKEWORD((BYTE)(pFileInf->EditTime.Year>>8), (BYTE)pFileInf->EditTime.Year);

	pFileInf->Res[0] = MAKEDWORD((BYTE)(pFileInf->Res[0]>>24), (BYTE)(pFileInf->Res[0]>>16),\
										(BYTE)(pFileInf->Res[0]>>8), (BYTE)(pFileInf->Res[0]));

	//�����ӵ��û���Ϣ����CRCУ�鳤�ȣ���ѡ�øó��Ƚ���У�飬�������ԭʼ�ļ����Ƚ���У��
	//ע��У��ʱ��Ҫȥ���ļ���ʼ��16���ֽ�
	if(pFileInf->Res[0] == 0)
		*pCrcLen = UserInfAdd-16;
	else
		*pCrcLen = pFileInf->Res[0]-16;
			
	//����ʵ��Ӧ���������CRCУ�鳤�Ƚ��кϷ��Լ��
	if((*pCrcLen>MOST_CODE_FILE_LEN)||(*pCrcLen<LEAST_CODE_FILE_LEN))
		return ERR_FAILURE;

	ReportMsg("Read the vxworks file version information OK!!!!");
	return ERR_SUCCESS;
}

//�����������У�飬16λ�ۼӺ�У��
STATUS DB_CodeFileCheckSum(TCodeFileInf *pFileInf, DWORD dwCheckLen)
{
	BYTE *pData = NULL;
	DWORD len, offset, reallen;
	WORD wCheckSum = 0x55aa;
	STATUS rtn; 

	if(pFileInf == NULL)
		return ERR_FAILURE;
	
	pData = (BYTE*)malloc(CODE_FILE_BUF_LEN);
	if(pData == NULL)
		return ERR_FAILURE;

	len = dwCheckLen;
	offset = 16;		//���ļ���ʼ��16�ֽڽ���У��
	while(len)
	{
		if(len>CODE_FILE_BUF_LEN)
			rtn = FS_ReadFile("vxWorks", offset, CODE_FILE_BUF_LEN, pData, &reallen, 0);
		else
			rtn = FS_ReadFile("vxWorks", offset, len, pData, &reallen, 0);
			
		if(rtn == FILE_ERROR)
		{
			ReportMsg("Read the vxworks file error.");
			free(pData);
			return ERR_FAILURE;
		}
		else
		{
			//�ۼӺ�У��
			for(int i=0; i<reallen; i++)
				wCheckSum += pData[i];
			len -= reallen;	
			offset += reallen;
		}
	}

	free(pData);
	wCheckSum = ~wCheckSum;

	if(wCheckSum == pFileInf->wCheckSum)
	{
		ReportMsg("The code file check sum OK...");
		return ERR_SUCCESS;
	}
	else
	{
		ReportMsg("The code file check sum error...");
		return ERR_SUCCESS;
	}
}

//Ϊ��֤ϵͳ�ɿ��ԣ���Ҫ��Ӧ�ó�������ݽ��кϷ��Լ�飬ȷ��д���Ӧ�ó���������Ч
STATUS DB_CodeFileIntegrityCheck(void *pFile, DWORD length)
{
	DWORD *pFileHead;
	BYTE *pBase;
	DWORD dwContentLen, dwCrcLen;
	TCodeFileInf *pUserInfor, tempUserInfor;
	WORD machine;
	
	//���ȼ��
	if((length<LEAST_CODE_FILE_LEN) ||(length>MOST_CODE_FILE_LEN))
	{
		ReportMsg("the code file length invalide.");
		return ERR_FAILURE;
	}

	if(pFile == NULL)
	{
		ReportMsg("the code file pointer invalide.");
		return ERR_FAILURE;
	}
	
	//����ļ�ͷELF��ʾ
	pFileHead = (DWORD*)pFile;
	if(*pFileHead != 0x7f454c46)
	{
		ReportMsg("The elf file head invalid.");
		return ERR_FAILURE;
	}

	pBase = (BYTE*)pFile;

	//���ELF��CPU���ͣ��Դ����ִ����ļ���ʵ��Ӳ���Ƿ�һ��
	machine = MAKEWORD(pBase[0x13], pBase[0x12]);
#if(SYSTEM_TYPE == _SYS_KF1300_)
	if(machine != EM_PPC)
#elif(SYSTEM_TYPE == _SYS_KF6500_)
	if(machine != EM_68K)
#endif
	{
		ReportMsg("the elf file type invalide..");
		return ERR_FAILURE;
	}
	
	//�����ļ������Լ�ELF�ļ������ӵ��û���Ϣ��λ�ã���������Ч�����Ƿ���ȷ
	dwContentLen = MAKEDWORD(pBase[12], pBase[13], pBase[14], pBase[15]);
	if((dwContentLen >= length) ||(dwContentLen<LEAST_CODE_FILE_LEN))
	{
		ReportMsg("the User information address invalide");
		return ERR_FAILURE;
	}

	//�����û���Ϣ���ֽ�˳��Res[0]�б�����crcУ������ݳ���
//	pUserInfor = (TCodeFileInf*)&pBase[dwContentLen];
	pUserInfor = &tempUserInfor;
	memcpy((void*)pUserInfor, (void*)&pBase[dwContentLen], sizeof(TCodeFileInf));
	pUserInfor->wCheckSum = MAKEWORD((BYTE)(pUserInfor->wCheckSum>>8), (BYTE)pUserInfor->wCheckSum);
	pUserInfor->Res[0] = MAKEDWORD((BYTE)(pUserInfor->Res[0]>>24), (BYTE)(pUserInfor->Res[0]>>16),\
									(BYTE)(pUserInfor->Res[0]>>8), (BYTE)(pUserInfor->Res[0]));
	
	//ע�⽻��ʱȥ��16�ֽ��ļ�ͷ,�����Կ��ǣ����ȼ��У�鳤����Ϣ�Ƿ���Ч��������������У�鳤�ȼ��û���Ϣǰ������
	if(pUserInfor->Res[0]==0)
		dwCrcLen = dwContentLen-16;
	else
		dwCrcLen = pUserInfor->Res[0]-16;

	//����ʵ��Ӧ���������CRCУ�鳤�Ƚ��кϷ��Լ��
	if((dwCrcLen>MOST_CODE_FILE_LEN)||(dwCrcLen<LEAST_CODE_FILE_LEN))
	{
		ReportMsg("the code file crc check length error");
		return ERR_FAILURE;
	}

	//У���ļ�����
	if(DAE_CheckSum((BYTE*)&pBase[16], dwCrcLen) != pUserInfor->wCheckSum)
	{
		ReportMsg("the code file crc check error.");
		return ERR_FAILURE;
	}

	return ERR_SUCCESS;
}

DWORD DAE_GetMemVarSize(DWORD dwID)
{
	WORD	wType = HIWORD(dwID);
	
	switch(wType)
	{
		case DAE_VARTYPE_NVRAM: 	return DBASE_NVRAM_SIZE;
		case DAE_VARTYPE_EEPROM:	return AA1024_LEN;
		case DAE_VARTYPE_SERTXDBUF: return MISITXDBUFLEN;
		case DAE_VARTYPE_SERRXDBUF: return MISIRXDBUFLEN;
		case DAE_VARTYPE_CANTXDBUF: return 512;
		case DAE_VARTYPE_CANRXDBUF: return 512;
		case DAE_VARTYPE_ETHATXDBUF: 
		case DAE_VARTYPE_ETHBTXDBUF: return TXDBUFLEN;
		case DAE_VARTYPE_ETHARXDBUF: 
		case DAE_VARTYPE_ETHBRXDBUF: return RXDBUFLEN;
		default: break;
	}

	return 0;
}

WORD DAE_ReadMemVar(DWORD dwID,DWORD dwOffset,DWORD wLen,BYTE *pBuf)
{
	CSerial *pSerial;
	CSocket	*pSocket;
	WORD	wType = HIWORD(dwID);
	WORD	wNo = LOWORD(dwID);
	WORD	wPortID;
	WORD	wRtn = 0;
	
	switch(wType)
	{
		case DAE_VARTYPE_NVRAM:
			// Խ����
			if(dwOffset >= DBASE_NVRAM_SIZE)
				return 0;

			// ��������
			if(dwOffset+wLen >= DBASE_NVRAM_SIZE)
				wLen = DBASE_NVRAM_SIZE-dwOffset;

			// ��������
			memcpy(pBuf,(BYTE *)(DBASE_NVRAM_BASE+dwOffset),wLen);
			wRtn = wLen;
			break;
			
		case DAE_VARTYPE_EEPROM:
			// Խ����
			if(dwOffset >= AA1024_LEN)
				return 0;

			// ��������
			if(dwOffset+wLen >= AA1024_LEN)
				wLen = AA1024_LEN-dwOffset;

			// ��������
			EEPROM_Read(dwOffset, wLen, pBuf);
			wRtn = wLen;
			break;
			
		case DAE_VARTYPE_SERTXDBUF:
			// Խ����
			if(dwOffset >= MISITXDBUFLEN)
				return 0;

			// ��������
			if(wLen > MISITXDBUFLEN)
				wLen = MISITXDBUFLEN-dwOffset;

			// ��������
			pSerial = GetSortSerial(wNo-1);
			if(pSerial == NULL)
			{
				wRtn = 0;
				break;
			}

			wRtn = pSerial->ReadFromTxdBuf(pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_SERRXDBUF:
			// Խ����
			if(dwOffset >= MISIRXDBUFLEN)
				return 0;

			// ��������
			if(wLen > MISIRXDBUFLEN)
				return 0;

			// ��������
			pSerial = GetSortSerial(wNo-1);
			if(pSerial == NULL)
			{
				wRtn = 0;
				break;
			}
			wRtn = pSerial->ReadFromRxdBuf(pBuf, dwOffset, wLen);
			break;

		case DAE_VARTYPE_CANTXDBUF:
			// Խ����
			if(dwOffset >= CAN_MONITOR_BUF_LEN)
				return 0;

			// ��������
			if(wLen > CAN_MONITOR_BUF_LEN)
				wLen = CAN_MONITOR_BUF_LEN-dwOffset;

			// ��������
			wRtn = ReadFromCanTxdBuf(wNo,pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_CANRXDBUF:
			// Խ����
			if(dwOffset >= CAN_MONITOR_BUF_LEN)
				return 0;

			// ��������
			if(wLen > CAN_MONITOR_BUF_LEN)
				wLen = CAN_MONITOR_BUF_LEN-dwOffset;

			wRtn = ReadFromCanRxdBuf(wNo,pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_ETHATXDBUF:
		case DAE_VARTYPE_ETHBTXDBUF:
			// Խ����
			if(dwOffset >= TXDBUFLEN)
				return 0;

			// ��������
			if(wLen > TXDBUFLEN)
				wLen = TXDBUFLEN-dwOffset;

			// ��������
			if(wType == DAE_VARTYPE_ETHATXDBUF)
				wPortID = MAKEWORD(wNo,SC_PORT_ETHERNETA);
			else
				wPortID = MAKEWORD(wNo,SC_PORT_ETHERNETB);
				
			pSocket = GetSortSocket(GetSocketID(wPortID));
			if(pSocket == NULL)
			{
				wRtn = 0;
				break;
			}
			
			wRtn = pSocket->ReadFromTxdBuf(pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_ETHARXDBUF:
		case DAE_VARTYPE_ETHBRXDBUF:
			// Խ����
			if(dwOffset >= RXDBUFLEN)
				return 0;

			// ��������
			if(wLen > RXDBUFLEN)
				wLen = RXDBUFLEN-dwOffset;

			// ��������
			if(wType == DAE_VARTYPE_ETHARXDBUF)
				wPortID = MAKEWORD(wNo,SC_PORT_ETHERNETA);
			else
				wPortID = MAKEWORD(wNo,SC_PORT_ETHERNETB);
			
			pSocket = GetSortSocket(GetSocketID(wPortID));
			if(pSocket == NULL)
			{
				wRtn = 0;
				break;
			}
			
			wRtn = pSocket->ReadFromRxdBuf(pBuf, dwOffset, wLen);
			break;

		default:
			return 0;
	}

	return wRtn;
}

STATUS DAE_WriteMemVar(DWORD dwID,DWORD dwOffset,DWORD wLen,BYTE *pBuf)
{
	return DB_ERR_OK;
}

void DAE_ResetDevice(DWORD type)
{
	// ������
	if(type != 0)
		G_pCfgTable->SystemStatus.dwFlag = 0;

	LogError("system reset ",FILE_LINE,"by task 0x%x.", DAE_GetCurrentTaskPortId());

	SystemReset(type);
}

// ע��˿�ͨ��״̬
STATUS DAE_SetCommStatus(WORD wPort,BYTE Status)
{
 	TAppInfo	*pTask;
	STATUS		rc;
	int 		i;
	
	// 1 ���ݶ˿ںŲ�����Ӧ������
	pTask =  G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++,pTask++)
	{
		if((pTask->dwPortID == wPort)&&(pTask->hTaskID != APP_TASKID_NULL))
			break;
	}

	if(i == DAE_MAX_APP_NUM)
		return DB_ERR_ID;

	// 2 ����ͨ��״̬����Ӧ����
	if(Status == COMM_STATUS_OK)
	{
		// 2.1 ע��˿�ͨ��״̬
		DB_ClearEventFlag(pTask,APP_EVENT_COMM);

		// 2.2 ע��˫��ͨ��״̬

		if(wPort == ((SC_PORT_CANBUS<<8)+1))
		{
			DAE_ClearRunningFlags(SS_NETACOMM); 	
		}
		else if(wPort == ((SC_PORT_CANBUS<<8)+2))
		{
			DAE_ClearRunningFlags(SS_NETBCOMM); 	
		}
		else if(wPort == ((SC_PORT_ETHERNETA<<8)+1))
		{
			DAE_ClearRunningFlags(SS_NETACOMM); 	
		}
		else if(wPort == ((SC_PORT_ETHERNETB<<8)+1))
		{
			DAE_ClearRunningFlags(SS_NETBCOMM); 	
		}
		
		//���ն˿ڷֱ��¼ͨ��״̬
		switch(MAJOR_PORT(wPort))
		{
			case SC_PORT_ETHERNETA:	DAE_ClearRunningFlags(SS_ETHA_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_ETHERNETB:	DAE_ClearRunningFlags(SS_ETHB_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_SERIAL:	DAE_ClearRunningFlags(SS_SER_COMM_ERR+MINOR_PORT(wPort)-2); break; 	
			case SC_PORT_CANBUS:	DAE_ClearRunningFlags(SS_CAN_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
		}

		// 2.3 ע��ͨ���¼�
		DB_Register_Comm(0,wPort);
	}
	else
	{
		// 2.1 ע��˿�ͨ��״̬
		DB_SetEventFlag(pTask,APP_EVENT_COMM);
		
		// 2.2 ע��˫��ͨ��״̬

		if(wPort == ((SC_PORT_CANBUS<<8)+1))
		{
			DAE_SetRunningFlags(SS_NETACOMM); 	
		}
		else if(wPort == ((SC_PORT_CANBUS<<8)+2))
		{
			DAE_SetRunningFlags(SS_NETBCOMM); 	
		}
		else if(wPort == ((SC_PORT_ETHERNETA<<8)+1))
		{
			DAE_SetRunningFlags(SS_NETACOMM); 	
		}
		else if(wPort == ((SC_PORT_ETHERNETB<<8)+1))
		{
			DAE_SetRunningFlags(SS_NETBCOMM); 	
		}

		//���ն˿ڷֱ��¼ͨ��״̬
		switch(MAJOR_PORT(wPort))
		{
			case SC_PORT_ETHERNETA:	DAE_SetRunningFlags(SS_ETHA_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_ETHERNETB:	DAE_SetRunningFlags(SS_ETHB_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_SERIAL:	DAE_SetRunningFlags(SS_SER_COMM_ERR+MINOR_PORT(wPort)-2); break; 	
			case SC_PORT_CANBUS:	DAE_SetRunningFlags(SS_CAN_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
		}

		// 2.3 ע��ͨ���¼�
		DB_Register_Comm(1,wPort);
	}
	
	
	return DB_ERR_OK;
}

// ��ȡ�˿�ͨ��״̬
STATUS DAE_GetCommStatus(WORD wPort,BYTE &Status)  //lz 20140314
{
 	TAppInfo	tTask;
	STATUS		rc;
	BOOL 		bBreak = TRUE;
	
	// ���ݶ˿ںŲ�����Ӧ������
	rc = DAE_Portid2AppInfo(wPort,&tTask);
	if(rc != DB_ERR_OK)
		return rc;
	
	switch(MAJOR_PORT(wPort))
	{
		case SC_PORT_ETHERNETA: bBreak = DAE_GetRunningFlags(SS_ETHA_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
		case SC_PORT_ETHERNETB: bBreak = DAE_GetRunningFlags(SS_ETHB_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
		case SC_PORT_SERIAL:	bBreak = DAE_GetRunningFlags(SS_SER_COMM_ERR+MINOR_PORT(wPort)-2); break;	
		case SC_PORT_CANBUS:	bBreak = DAE_GetRunningFlags(SS_CAN_COMM_ERR+MINOR_PORT(wPort)-1); break;	
		default: return DB_ERR_FAILURE;
	}

	if(bBreak)
		Status = COMM_STATUS_BREAK;
	else
		Status = COMM_STATUS_OK;
	
	return DB_ERR_OK;
	
/*	
 	TAppInfo	tTask;
	STATUS		rc;
	
	// ���ݶ˿ںŲ�����Ӧ������
	rc = DAE_Portid2AppInfo(wPort,&tTask);

	if(rc != DB_ERR_OK)
		return rc;

	if(tTask.bEvent&APP_EVENT_COMM)
		Status = COMM_STATUS_BREAK;
	else
		Status = COMM_STATUS_OK;

	return DB_ERR_OK;
*/	
}

STATUS DAE_UpdateBasicConfig()
{
	// 1.����־
	G_tBasicConfig.dwInitFlag = BAISIC_INIT_FLAG;

	// 2.����У��
	DAE_MakeCheckSum((BYTE *)&G_tBasicConfig,sizeof(TBasicConfig)-2);

	// 3.д��EEPROM
	return EEPROM_Write(EEPROM_BASICCONFIG_START,sizeof(TBasicConfig),&G_tBasicConfig);
}

/************************************************************
	���ܣ���ȡ����λ����ͨ��״̬
	������pStatus		--   ͨ��״̬λ��ָ��
	���أ���ȷʱ����DB_ERR_OK�����󷵻ش�����
************************************************************/
STATUS DAE_GetMBOCommStatus(BYTE *pStatus)
{
	if(pStatus == NULL)
		return DB_ERR_PTRNULL;

	// ���
	memset(pStatus,0,(MAX_MAPBITOBJ_NUM+7)/8);

	// ��λ��д
	for(int i=0;i<MAX_MAPBITOBJ_NUM;i++)
	{
		if(DAE_GetRunningFlags(SS_MAPBIT_COMM_ERR+i))
			pStatus[i/8] |= 0x01<<(i%8);
	}

	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ͸�����ݻ�����βָ�롣
	��������
	���أ��¼�������βָ�롣
************************************************************/
WORD DAE_GetTransDataTail()
{
	return g_GLBTransWrap.nTailIndex%DAE_MAX_TRANSNUM;
}

/************************************************************
	���ܣ������ݿ��ж�ȡһ��͸������
	������	rp   	--  ��ȡ�����õ�ָ��
			*event	--	���ص�����ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
WORD DAE_ReadTransData(BYTE *pBuf,WORD rp)
{
	TFixTransData	*pEvent;
	WORD			wLen;
	WORD			wPortID = DAE_GetCurrentTaskPortId();
	
    // 1���������ָ��ĺϷ���
    if( pBuf==NULL )
        return 0;
                     
	rp = rp%MAX_RUNREC_NUM;
                       
	// 2����ȡ������Ϣ
	pEvent = &g_GLBTransWrap.pEvent[rp];

	// 3 ��鱾�˿�����
	if((pEvent->tHead.wDesPort != wPortID)&&(pEvent->tHead.wDesPort != DAE_TRANSPORT_BROADCAST))
		return 0;

	// 4����������
	wLen = pEvent->tHead.wDataLen+sizeof(TTransDataHead)-1;
	if(wLen > DAE_MAX_TRANSLEN)
	{
		LogError("DB_ReadTransData",FILE_LINE,"Trans data length is overflow! (%d)",wLen);
		wLen = DAE_MAX_TRANSLEN;
	}
	memcpy(pBuf,pEvent,wLen);

	// 5�����ؽ��
	return (pEvent->tHead.wDataLen+sizeof(TTransDataHead)-1);
}
		
/************************************************************
	���ܣ���͸�����ݿ���д��һ��͸�����ݡ�
	������*str,		--  ���м�¼�ַ���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DAE_WriteTransData(WORD wClass,WORD wDesPort,WORD wDataLen,BYTE *pBuf)
{
	TFixTransData	*pEvent;
	
	// 1���������ָ��ĺϷ���
	if( pBuf == NULL )
		 return DB_ERR_BUFNULL;


	// 2��д������
	if((wDataLen+sizeof(TTransDataHead)-1) > DAE_MAX_TRANSLEN)
	{
		LogError("DB_WriteTransData",FILE_LINE,"Trans data length is overflow! (%d)",wDataLen);
		wDataLen = DAE_MAX_TRANSLEN-(sizeof(TTransDataHead)-1);
	}

	pEvent = &g_GLBTransWrap.pEvent[g_GLBTransWrap.nTailIndex];
	pEvent->tHead.wClass = wClass;
	pEvent->tHead.wSrcPort = DAE_GetCurrentTaskPortId();
	pEvent->tHead.wDesPort = wDesPort;
	pEvent->tHead.wDataLen = wDataLen;
	memcpy(&pEvent->tHead.Data,pBuf,wDataLen);
	
	g_GLBTransWrap.nTailIndex = (g_GLBTransWrap.nTailIndex+1)%DAE_MAX_TRANSNUM;

	// 3����Ϣ֪ͨ
	if(wDesPort == DAE_TRANSPORT_BROADCAST)
	{
		// �㲥��Ϣ
		for(int i=0;i<DAE_MAX_APP_NUM;i++)
		{
			if(!(G_pCfgTable->TaskApp[i].bMask & APP_EVENT_OIE))
				continue;

			// �޳�����
			if(G_pCfgTable->TaskApp[i].dwPortID == DAE_GetCurrentTaskPortId())
				continue;
				
			if(G_pCfgTable->TaskApp[i].hTaskID != APP_TASKID_NULL)
				OS_PostMessage(G_pCfgTable->TaskApp[i].hTaskID, (WORD)SM_TRANSOIE, pEvent->tHead.wSrcPort,0,0,0);
		}
	}
	else
	{
		OS_PostMessage(DAE_GetTaskID(pEvent->tHead.wDesPort),SM_TRANSOIE,pEvent->tHead.wSrcPort, 0, 0, 0);
	}
	
	return DB_ERR_OK;
}

/************************************************************
	���ܣ���ȡ��Ӧ��̫���ڵ�MAC��Ϣ
	������devno		--   ���ڱ��
	������pMac		--   ��Ҫ��ȡ��MAC��Ϣ
	���أ���ȷʱ����DB_ERR_OK�����󷵻ش�����
************************************************************/
#if (SYSTEM_TYPE == _SYS_KF1300_)
	#define MAC_BASE_ADD	(LPC_CS0+0xe0)
#elif(SYSTEM_TYPE == _SYS_KF6500_)
	#define MAC_BASE_ADD	(CODE_FLASH_BASE+0x3f100)
#endif
STATUS DB_GetNetMac(BYTE devno, BYTE *pMac)
{
	BYTE *ptr;
	ASSERT(devno<2);
	if(devno>=2)
		return DB_ERR_FAILURE;

	ptr = (BYTE*)(MAC_BASE_ADD+devno*6);

	for(int i=0; i<6; i++)
		pMac[i] = ptr[i];
	
	return DB_ERR_OK;
}

