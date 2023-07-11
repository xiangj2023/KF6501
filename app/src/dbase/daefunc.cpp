/********************************************************************************************************/
/*																										*/
/*        						天津凯发电气股份有限公司												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Daefunc.cpp                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义运行环境相关的接口函数																*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* 函数																									*/
/*																										*/
/*	DB_GetRunRecordNum()			读取运行记录缓冲区中未读取数据个数									*/
/*	DB_IsRunRecordEmpty()			运行记录缓冲区是否为空												*/
/*	DB_GetRunRecordHead()			读取运行记录缓冲区头指针											*/
/*	DB_GetRunRecordRead()			读取运行记录缓冲区读指针											*/
/*	DB_SetRunRecordRead()			设置运行记录缓冲区读指针											*/
/*	DB_IncRunRecordRead()			递增运行记录缓冲区读指针											*/
/*	ReportMsg()						发布运行信息，包括调试信息和错误记录								*/
/*	LogError()						登记异常信息，包括异常信息计数,文件名,行号,当前任务,时间,异常信息等	*/
/*	DB_ReadLogInfo()				读取运行记录														*/
/*	DB_Misc_GetNum()				读取操作事件缓冲区中未读取事件数据个数								*/
/*	DB_Misc_IsEmpty()				操作事件缓冲区是否为空												*/
/*	DB_Misc_GetHead()				读取操作事件缓冲区头指针											*/
/*	DB_Misc_GetRead()				读取操作事件缓冲区读指针											*/
/*	DB_Misc_SetRead()				设置操作事件缓冲区读指针											*/
/*	DB_Misc_IncRead()				递增操作事件缓冲区读指针											*/
/*	DB_Misc_Read()					从操作事件缓冲区中读一条操作事件数据								*/
/*	DB_Misc_Write()					向操作事件缓冲区中写入一条操作事件数据								*/
/*	DB_Register_Reset ()			注册一个系统复位事件												*/
/*	DB_Register_FileDown ()			注册一个文件下装事件												*/
/*	DB_Register_Comm ()				注册一个通信事件													*/
/*	DB_Register_BO()				注册一个遥控事件													*/
/*	DB_Register_AO()				注册一个遥调事件													*/
/*	DB_Register_SETTING()			注册一个修改定值事件												*/
/*	DB_Register_SETArea ()			注册一个修改当前定值区号事件										*/
/*	DB_Register_ParaChange()		注册一个修改参数事件												*/
/*	DB_Register_PowerOnOff()		注册一个修改参数事件												*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*		Daefunc.h						系统函数声明													*/
/*		Db_err.h						数据库返回值													*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
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

	//连续寻找时的快速查找
	if(app == G_HAPPCACHE)
		return G_PAPPCACHE;

	//跟踪任务号
	G_HAPPCACHE = app;
	G_PAPPCACHE = NULL;

    for(int i=0; i<DAE_MAX_APP_NUM; i++ )
    {
		if( pAppInfo[i].hTaskID == app )
		{
			//跟踪任务指针
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
	功能：取设备的型号
	参数：无
	返回：装置类型。
************************************************************/
WORD DB_GetDevType()
{
	return G_pCfgTable->DBaseConfig.wType;
}

/************************************************************
	功能：取程序的CRC码
	参数：无
	返回：装置类型。
************************************************************/
WORD DB_GetCodeCrc()
{
	return G_pCfgTable->DBaseConfig.wCodeCrc;
}

/************************************************************
	功能：取设备的ID
	参数：无
	返回：装置类型。
************************************************************/
DWORD DB_GetDevID()
{
	return G_pCfgTable->DBaseConfig.dwDevID;
}


/************************************************************
	功能：取设备的ID
	参数：无
	返回：装置类型。
************************************************************/
DWORD DB_GetDevAddress()
{
	return G_pCfgTable->DBaseConfig.wAddress;
}

/************************************************************
	功能：取上位机地址
	参数：无
	返回：上位机地址。
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
	TMCB		*pAlloced;				//已分配内存指针	

	NvRamLink = &G_pCfgTable->NvRamLink;

	//初始化内存链		
	NvRamLink->pFree = (LPBYTE)(DBASE_NVRAM_BASE+ALIGN(sizeof(TSysConfigTable),ALIGNMENT));
	NvRamLink->dwSize = DBASE_NVRAM_LENGTH-ALIGN(sizeof(TSysConfigTable),ALIGNMENT);
	NvRamLink->dwSign = MMLNK_SIGN_VALID;	

	//分配一个空节点
	pAlloced = NvRamLink->pAlloced = (TMCB*)NvRamLink->pFree;
	pAlloced->dwSign = MCB_SIGN_ALLOC;
	pAlloced->dwSize = 0;

	//修正空闲指针
	NvRamLink->dwSize -= sizeof(TMCB);
	NvRamLink->pFree += sizeof(TMCB);
	
}

void*	DAE_MemAlloc(DWORD size)
{
    PBYTE 		buf;
    TMCB 		*pmcb;
    
	if(size==0)
		return NULL;
	
    //调节应该请求分配的内存长度为ALIGNMENT的倍数
    size = ALIGN(size,ALIGNMENT);
	
    //取空闲NVRAM的管理节点
	TMemLink	*pNvRamLink = &G_pCfgTable->NvRamLink;				//静态内存分配

    ASSERT(pNvRamLink != NULL);
    ASSERT(size != 0);

    //请求分配的内存长度为零
	if((size == 0)||(pNvRamLink == NULL))
		return NULL;
		
    //数据库空闲内存表尚未初始化
    if(pNvRamLink->dwSign != MMLNK_SIGN_VALID)
        return NULL;
    	
    //判断空闲RAM长度是否大于请求分配的长度并且系统测试NVRAM是否正常
    if( pNvRamLink->dwSize >= (size+sizeof(TMCB)) ) /*&& !(status & STATUS_NVRAM_ERROR))*/
    {
        buf = pNvRamLink->pFree;
        pmcb = (TMCB *)(buf-sizeof(TMCB));

	    pmcb->dwSign = MCB_SIGN_ALLOC;
        pmcb->dwSize = size;
        pNvRamLink->dwSize -= (size+sizeof(TMCB));
        pNvRamLink->pFree += (size+sizeof(TMCB));

	    //记录分配的最后一条记录
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

	// 1。检查数据指针合法性
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
	
	// 2。检查数据规模变化
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
	
	// 3。检查内存链
	while(pMCB->dwSize != 0)
	{		
		if((pMCB->dwSign != MCB_SIGN_ALLOC) && (pMCB->dwSign != MCB_SIGN_FREE))
			return FALSE;

		//最大长度检查 128Kz
		if(pMCB->dwSize>0x20000)
			return FALSE;

		//最大长度检查 128K
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
	// 1。NvRAM检测
	// 2。FLASH检测
	return TRUE;
}

BOOL DAE_CheckSysStatus()
{
	TStatus				*pSystemStatus;	

	pSystemStatus = &G_pCfgTable->SystemStatus;

	//检查运行状态有效标志
	if(pSystemStatus->dwFlag != SYS_RUNNING_OK)
		return FALSE;

	/*检测保护告警事件指针合法性*/
//	if(pSystemStatus->nRelayAlarmT > MAX_RALM_LENGTH)
//		return FALSE;

	/*检测保护动作事件指针合法性*/
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
	
	// 1。检测硬件
	if(!DAE_CheckHard())
	{
		DAE_SetRunningFlags(SS_SYS_HARD_ERR);		
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_SYS_HARD_ERR);		
	}
	
	// 2。检查配置状态
	if(!DAE_CheckSysStatus())
	{
		DAE_SetRunningFlags(SS_SYS_STATUS_ERR);		
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_SYS_STATUS_ERR);		
	}
		
	// 3。检查NvRAM内存链
	if(!DAE_CheckNvMalloc())		
	{
		DAE_SetRunningFlags(SS_NVRAM_LINK_ERR);
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_NVRAM_LINK_ERR);		
	}

	// 4。检查操作记录库
	if(!DAE_CheckMISCWrap())		
	{
		DAE_SetRunningFlags(SS_NVRAM_MISC_ERR);
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_NVRAM_MISC_ERR);		
	}

	// 5。检查运行记录库
	if(!DAE_CheckRecordWrap())		
	{
		DAE_SetRunningFlags(SS_NVRAM_RECORD_ERR);
		bStaus = FALSE;
	}
	else
	{
		DAE_ClearRunningFlags(SS_NVRAM_RECORD_ERR);		
	}

	// 6。检查软压板
	if(!DB_CheckSWData(DB_GetSWBase(),DB_GetSWNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_SW_ERR))
		{
			//更新失败直接告警，不再更新
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
	
	// 7。检查定值
	if(!DB_CheckSETData(DB_GetSETFloatBase(),DB_GetSETNum())||\
		!DB_CheckSETFixData(DB_GetSETFixBase(),DB_GetSETNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_SET_ERR))
		{
			//更新失败直接告警，不再更新
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

	// 8。检查系统定值
	if(!DB_CheckSETData(DB_GetSystemSETBase(),DB_GetSystemSETNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_SYSSET_ERR))
		{
			//更新失败直接告警，不再更新
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
	
	// 9。检查系数
	if(!DB_CheckCFData(DB_GetCFBase(),DB_GetCFNum()))
	{
		if(!DAE_GetRunningFlags(SS_EEPROM_CF_ERR))
		{
			//更新失败直接告警，不再更新
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


	//集中判断出错事件
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

	//集中判断出错事件
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
//2012-5-9 动态刷新逻辑方程用 from z
STATUS DAE_WriteFlashFile(char *strFileName,void *pFile,DWORD dwFileLen)
{
	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;

	//对于应用程序文件，写入前必须进行内容的完整性检查
	if( (!strcmp( strFileName, "vxworks" )) || (!strcmp( strFileName, "vxWorks")) )
	{
		if(DB_CodeFileIntegrityCheck((BYTE*)pFile, dwFileLen) != ERR_SUCCESS)
			return DB_ERR_FILE_WRITE;
	}
	
	if(FS_WriteFile( strFileName, 0,dwFileLen,(BYTE *)pFile,FILE_HEAD|FILE_END,0) != FILE_ERROR)
	{
		//当修改完逻辑文件后必须通知保护任务重新初始化逻辑
		if( !strcmp( strFileName, "logic.plc" ) )
		{
			//OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, 0, 0, 0);
			OS_PostMessage(g_hRelayTaskID, SM_LOGICCHANGED, 0, 0, 0, 0);
			ReportMsg("dae download ok");
		}
		//应用程序下载完成后自动热启动
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

	//先从EEPROM中读出文件头
	rc = EEPROM_Read(dwAddr, sizeof(TFileHead), &filehead);
	if(rc != DB_ERR_OK)
	{
        return DB_ERR_FILE_READ;
	}

	//检查文件头校验长度
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

	//直接读出内容
	return EEPROM_Read(dwAddr, dwFileLen, pFile);
/*	
	//先从EEPROM中读出文件头
	rc = EEPROM_Read(dwAddr, sizeof(TFileHead), pFile);
	if(rc != DB_ERR_OK)
	{
        return DB_ERR_FILE_READ;
	}

	//检查文件头校验长度
	filehead = (TFileHead *)pFile;
	if(!DAE_IsCheckSumOK((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD)))
	{
        return DB_ERR_CHECK;
	}

	//检查文件体校验
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
		
	//检查文件头校验长度
	if(!DAE_IsCheckSumOK((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD)))
	{
		return DB_ERR_CHECK;
	}
	
	//检查文件体校验		
	if(!DAE_IsCheckSumOK((BYTE *)pFile, dwLen-sizeof(WORD)))
	{
		return DB_ERR_CHECK;
	}
	
	return DB_ERR_OK;
}

STATUS DAE_ReadFile(char *strFileName,BYTE *pFile,DWORD dwFileLen)
{
	_FormatFileName(strFileName);
	
	//  根据存储类别读取
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
	功能：读取运行记录缓冲区中未读取数据个数。
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_GetRunRecordNum(WORD *retval)
{
	TRecordWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = &G_pCfgTable->RecordWrap;

    // 3。获取读指针    
    rc = DB_GetRunRecordRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_RUNREC_NUM-rp)%MAX_RUNREC_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：运行记录缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_IsRunRecordEmpty()
{
	TRecordWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。获取事项库指针             
    pWrap = &G_pCfgTable->RecordWrap;

    // 2。获取读指针    
    rc = DB_GetRunRecordRead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	功能：从数据库中读取一条运行记录信息
	参数：	rp   	--  读取数据用的指针,当BLANK时用内部读指针
			*event	--	返回的事项指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_ReadRunRecord(TRecordBlock *event, DWORD rp )
{
    TRecordWrap		*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = &G_pCfgTable->RecordWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
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
                       
	// 4。读取事项信息
	*event = pWrap->pEvent[rp];
                                     
	return DB_ERR_OK;
}
		
/************************************************************
	功能：向运行记录库中写入一条运行记录数据。
	参数：*str,		--  运行记录字符串
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteRunRecord(char *str)
{
    TRecordWrap		*pWrap;
	TAppInfo		*pAppInfo;
    WORD 			i, size;
                     
	// 1。检查事项指针的合法性
	if( str == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	if(G_pCfgTable == NULL)
	{
		printf("%s",str);
		return DB_ERR_BASENULL;
	}
	
	pWrap = &G_pCfgTable->RecordWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	//检查对应内存表是否初始化
	if(pWrap->dwMagic != WRAP_INIT_FLAG )
		return DB_ERR_WRAP_INIT;
	
	// 3。存数据
    size = strlen(str);
	if(size >= ES_MAX_LINE)
		size = ES_MAX_LINE;	
    pWrap->pEvent[pWrap->nTailIndex].nCharSize = size;
    memcpy(pWrap->pEvent[pWrap->nTailIndex].szRecordMsg,str,size);
	
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_RUNREC_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RUNREC_NUM;
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++)
	{
	    if( pAppInfo[i].nLogRead == pWrap->nTailIndex )
	         pAppInfo[i].nLogRead = (pWrap->nTailIndex+1)%MAX_RUNREC_NUM;
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：读取运行记录缓冲区头指针。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordHead(WORD *retval)
{
	TRecordWrap		*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = &G_pCfgTable->RecordWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：读取运行记录缓冲区读指针。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    *retval = pAppInfo->nLogRead%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：读取运行记录缓冲区尾指针。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordTail(WORD *retval)
{
	TRecordWrap		*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = &G_pCfgTable->RecordWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：设置运行记录缓冲区读指针。
	参数：	rp   	-- 修正的读指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetRunRecordRead(WORD rp)
{
	TAppInfo	*pAppInfo;

    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    pAppInfo->nLogRead = rp%MAX_RUNREC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增运行记录缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_IncRunRecordRead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nLogRead = (pAppInfo->nLogRead+1)%MAX_RUNREC_NUM;
		
	return DB_ERR_OK;
}

/************************************************************
	功能：读取操作事件缓冲区中未读取事件数据个数。
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_Misc_GetNum(WORD *retval)
{
	TMiscWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = &G_pCfgTable->MiscWrap;

    // 3。获取读指针    
    rc = DB_Misc_GetRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_MISC_NUM-rp)%MAX_MISC_NUM;
    
    return DB_ERR_OK;   
}

/************************************************************
	功能：操作事件缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_Misc_IsEmpty()
{
	TMiscWrap		*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。获取事项库指针             
    pWrap = &G_pCfgTable->MiscWrap;

    // 2。获取读指针    
    rc = DB_Misc_GetRead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	功能：读取操作事件缓冲区头指针。
	参数：无
	返回：事件缓冲区头指针。
************************************************************/
STATUS DB_Misc_GetHead(WORD *retval)
{
	TMiscWrap		*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = &G_pCfgTable->MiscWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_MISC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：读取操作事件缓冲区尾指针。
	参数：无
	返回：事件缓冲区尾指针。
************************************************************/
STATUS DB_Misc_GetTail(WORD *retval)
{
	TMiscWrap		*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = &G_pCfgTable->MiscWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_MISC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：读取操作事件缓冲区读指针。
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_Misc_GetRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    *retval = pAppInfo->nMISCRead%MAX_MISC_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：设置操作事件缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_SetRead(WORD rp)
{
	TAppInfo	*pAppInfo;

    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    pAppInfo->nMISCRead = rp%MAX_MISC_NUM;

	return DB_ERR_OK;
}
/************************************************************
	功能：递增操作事件缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_IncRead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nMISCRead = (pAppInfo->nMISCRead+1)%MAX_MISC_NUM;

//	DB_RegisterFileChange(DB_FILE_BASICEV);
		
	return DB_ERR_OK;}

/************************************************************
	功能：从操作事件缓冲区中读一条操作事件数据。
	参数：rp	    --  序号
		data,		--  操作事件记录
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_Read(DWORD rp, TMiscEvent *pEvent)
{
    TMiscWrap 		*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( pEvent == NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = &G_pCfgTable->MiscWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
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
                       
	// 4。读取事项信息
	*pEvent= pWrap->pEvent[rp];
	                                     
	return DB_ERR_OK;
}
		
/************************************************************
	功能：向操作事件缓冲区中写入一条操作事件数据。
	参数：data,		--  操作事件记录
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_Write(TMiscEvent *pEvent)
{
    TMiscWrap		*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1。检查事项指针的合法性
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = &G_pCfgTable->MiscWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3。存数据
    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_MISC_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_MISC_NUM;
	
	// 5。修正各任务读指针,并注册事项信息
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
	功能：注册一个系统复位事件。
	参数：status	-- 复位方式   0 ：热复位 1：冷复位
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个文件下装事件。
	参数：strFileName 	-- 文件名
		   Flag			-- 文件操作结果
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个通信事件。
	参数：	status		-- 0：通信恢复 1：通信失败
			dwPortID	通信终端的端口
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个遥控事件。
	参数： 	no,  		-- 遥控号
			attr		-- 操作属性 0：空 1：分 2：合
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个遥调事件。
	参数： 	no,  		-- 遥调号
			lValue		-- 输出值
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个修改定值事件。
	参数： 	AreaNo,		-- 定值区号
			SETNo	-- 定值序号
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个修改当前定值区号事件。
	参数： 	AreaNo,		-- 定值区号
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个修改参数事件。
	参数： 	ParaType,   -- 参数类别
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个修改参数事件。
	参数： 	Status,		-- 0：上电 1：停电
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个软压板投切事件。
	参数：no,    		--  软压板序号
		  Status	    --  软压板状态	
						投：SW_STATUS_SET，切：SW_STATUS_CLR
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个配置字投切事件。
	参数：no,    		--  配置字序号
		  Status	    --  软压板状态	
							投：CFG_STATUS_SET，切：CFG_STATUS_CLR
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个系数修改事件。
	参数：no,    		--  系数序号
	返回：正确返回0，错误返回错误值。
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
	功能：注册一个置数操作。
	参数：no,    		--  植入序号
	参数：dwValue,    	--  植入数值
	返回：正确返回0，错误返回错误值。
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

//取端口名称
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

//读取应用程序文件版本信息
//为了消除不同时间编译内容不同的问题，只进行部分内容的校验，校验长度记录在用户信息的Res[0]中，以前的版本中没有该内容
STATUS	DB_GetCodeFileVerInf(TCodeFileInf *pFileInf, DWORD *pCrcLen)
{
	DWORD len;
	BYTE data[4];
	DWORD UserInfAdd;

	//首先从文件开始的第12字节位置读取文件版本信息在文件中的地址
	if(FS_ReadFile("vxWorks", 12, 4, data, &len, 0) == FILE_ERROR)
	{
		ReportMsg("Read the vxworks file head error");
		return ERR_FAILURE;
	}

	//计算地址，大端小端顺序调整
	UserInfAdd = MAKEDWORD(data[0], data[1], data[2], data[3]);

	//应用程序大小有效检查，大于512K
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

	//调整顺序
	pFileInf->wCheckSum = MAKEWORD(((BYTE)(pFileInf->wCheckSum>>8)), ((BYTE)pFileInf->wCheckSum));

	pFileInf->CompileTime.Year = MAKEWORD((BYTE)(pFileInf->CompileTime.Year>>8), (BYTE)pFileInf->CompileTime.Year);
	pFileInf->EditTime.Year = MAKEWORD((BYTE)(pFileInf->EditTime.Year>>8), (BYTE)pFileInf->EditTime.Year);

	pFileInf->Res[0] = MAKEDWORD((BYTE)(pFileInf->Res[0]>>24), (BYTE)(pFileInf->Res[0]>>16),\
										(BYTE)(pFileInf->Res[0]>>8), (BYTE)(pFileInf->Res[0]));

	//如果添加的用户信息中有CRC校验长度，则选用该长度进行校验，否则采用原始文件长度进行校验
	//注意校验时需要去掉文件开始的16个字节
	if(pFileInf->Res[0] == 0)
		*pCrcLen = UserInfAdd-16;
	else
		*pCrcLen = pFileInf->Res[0]-16;
			
	//根据实际应用情况，对CRC校验长度进行合法性检查
	if((*pCrcLen>MOST_CODE_FILE_LEN)||(*pCrcLen<LEAST_CODE_FILE_LEN))
		return ERR_FAILURE;

	ReportMsg("Read the vxworks file version information OK!!!!");
	return ERR_SUCCESS;
}

//程序代码内容校验，16位累加和校验
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
	offset = 16;		//从文件开始的16字节进行校验
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
			//累加和校验
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

//为保证系统可靠性，需要对应用程序的内容进行合法性检查，确保写入的应用程序完整有效
STATUS DB_CodeFileIntegrityCheck(void *pFile, DWORD length)
{
	DWORD *pFileHead;
	BYTE *pBase;
	DWORD dwContentLen, dwCrcLen;
	TCodeFileInf *pUserInfor, tempUserInfor;
	WORD machine;
	
	//长度检查
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
	
	//检查文件头ELF标示
	pFileHead = (DWORD*)pFile;
	if(*pFileHead != 0x7f454c46)
	{
		ReportMsg("The elf file head invalid.");
		return ERR_FAILURE;
	}

	pBase = (BYTE*)pFile;

	//检查ELF中CPU类型，以此区分代码文件与实际硬件是否一致
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
	
	//根据文件长度以及ELF文件所增加的用户信息的位置，检查代码有效长度是否正确
	dwContentLen = MAKEDWORD(pBase[12], pBase[13], pBase[14], pBase[15]);
	if((dwContentLen >= length) ||(dwContentLen<LEAST_CODE_FILE_LEN))
	{
		ReportMsg("the User information address invalide");
		return ERR_FAILURE;
	}

	//调整用户信息的字节顺序，Res[0]中保存了crc校验的数据长度
//	pUserInfor = (TCodeFileInf*)&pBase[dwContentLen];
	pUserInfor = &tempUserInfor;
	memcpy((void*)pUserInfor, (void*)&pBase[dwContentLen], sizeof(TCodeFileInf));
	pUserInfor->wCheckSum = MAKEWORD((BYTE)(pUserInfor->wCheckSum>>8), (BYTE)pUserInfor->wCheckSum);
	pUserInfor->Res[0] = MAKEDWORD((BYTE)(pUserInfor->Res[0]>>24), (BYTE)(pUserInfor->Res[0]>>16),\
									(BYTE)(pUserInfor->Res[0]>>8), (BYTE)(pUserInfor->Res[0]));
	
	//注意交验时去掉16字节文件头,兼容性考虑，首先检查校验长度信息是否有效，否则采用最初的校验长度即用户信息前的内容
	if(pUserInfor->Res[0]==0)
		dwCrcLen = dwContentLen-16;
	else
		dwCrcLen = pUserInfor->Res[0]-16;

	//根据实际应用情况，对CRC校验长度进行合法性检查
	if((dwCrcLen>MOST_CODE_FILE_LEN)||(dwCrcLen<LEAST_CODE_FILE_LEN))
	{
		ReportMsg("the code file crc check length error");
		return ERR_FAILURE;
	}

	//校验文件内容
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
			// 越界检查
			if(dwOffset >= DBASE_NVRAM_SIZE)
				return 0;

			// 长度修正
			if(dwOffset+wLen >= DBASE_NVRAM_SIZE)
				wLen = DBASE_NVRAM_SIZE-dwOffset;

			// 拷贝数据
			memcpy(pBuf,(BYTE *)(DBASE_NVRAM_BASE+dwOffset),wLen);
			wRtn = wLen;
			break;
			
		case DAE_VARTYPE_EEPROM:
			// 越界检查
			if(dwOffset >= AA1024_LEN)
				return 0;

			// 长度修正
			if(dwOffset+wLen >= AA1024_LEN)
				wLen = AA1024_LEN-dwOffset;

			// 拷贝数据
			EEPROM_Read(dwOffset, wLen, pBuf);
			wRtn = wLen;
			break;
			
		case DAE_VARTYPE_SERTXDBUF:
			// 越界检查
			if(dwOffset >= MISITXDBUFLEN)
				return 0;

			// 长度修正
			if(wLen > MISITXDBUFLEN)
				wLen = MISITXDBUFLEN-dwOffset;

			// 拷贝数据
			pSerial = GetSortSerial(wNo-1);
			if(pSerial == NULL)
			{
				wRtn = 0;
				break;
			}

			wRtn = pSerial->ReadFromTxdBuf(pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_SERRXDBUF:
			// 越界检查
			if(dwOffset >= MISIRXDBUFLEN)
				return 0;

			// 长度修正
			if(wLen > MISIRXDBUFLEN)
				return 0;

			// 拷贝数据
			pSerial = GetSortSerial(wNo-1);
			if(pSerial == NULL)
			{
				wRtn = 0;
				break;
			}
			wRtn = pSerial->ReadFromRxdBuf(pBuf, dwOffset, wLen);
			break;

		case DAE_VARTYPE_CANTXDBUF:
			// 越界检查
			if(dwOffset >= CAN_MONITOR_BUF_LEN)
				return 0;

			// 长度修正
			if(wLen > CAN_MONITOR_BUF_LEN)
				wLen = CAN_MONITOR_BUF_LEN-dwOffset;

			// 拷贝数据
			wRtn = ReadFromCanTxdBuf(wNo,pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_CANRXDBUF:
			// 越界检查
			if(dwOffset >= CAN_MONITOR_BUF_LEN)
				return 0;

			// 长度修正
			if(wLen > CAN_MONITOR_BUF_LEN)
				wLen = CAN_MONITOR_BUF_LEN-dwOffset;

			wRtn = ReadFromCanRxdBuf(wNo,pBuf, dwOffset, wLen);
			break;
			
		case DAE_VARTYPE_ETHATXDBUF:
		case DAE_VARTYPE_ETHBTXDBUF:
			// 越界检查
			if(dwOffset >= TXDBUFLEN)
				return 0;

			// 长度修正
			if(wLen > TXDBUFLEN)
				wLen = TXDBUFLEN-dwOffset;

			// 拷贝数据
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
			// 越界检查
			if(dwOffset >= RXDBUFLEN)
				return 0;

			// 长度修正
			if(wLen > RXDBUFLEN)
				wLen = RXDBUFLEN-dwOffset;

			// 拷贝数据
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
	// 冷启动
	if(type != 0)
		G_pCfgTable->SystemStatus.dwFlag = 0;

	LogError("system reset ",FILE_LINE,"by task 0x%x.", DAE_GetCurrentTaskPortId());

	SystemReset(type);
}

// 注册端口通信状态
STATUS DAE_SetCommStatus(WORD wPort,BYTE Status)
{
 	TAppInfo	*pTask;
	STATUS		rc;
	int 		i;
	
	// 1 根据端口号查找相应的任务
	pTask =  G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++,pTask++)
	{
		if((pTask->dwPortID == wPort)&&(pTask->hTaskID != APP_TASKID_NULL))
			break;
	}

	if(i == DAE_MAX_APP_NUM)
		return DB_ERR_ID;

	// 2 根据通信状态做相应处理
	if(Status == COMM_STATUS_OK)
	{
		// 2.1 注册端口通信状态
		DB_ClearEventFlag(pTask,APP_EVENT_COMM);

		// 2.2 注册双网通信状态

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
		
		//按照端口分别记录通信状态
		switch(MAJOR_PORT(wPort))
		{
			case SC_PORT_ETHERNETA:	DAE_ClearRunningFlags(SS_ETHA_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_ETHERNETB:	DAE_ClearRunningFlags(SS_ETHB_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_SERIAL:	DAE_ClearRunningFlags(SS_SER_COMM_ERR+MINOR_PORT(wPort)-2); break; 	
			case SC_PORT_CANBUS:	DAE_ClearRunningFlags(SS_CAN_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
		}

		// 2.3 注册通信事件
		DB_Register_Comm(0,wPort);
	}
	else
	{
		// 2.1 注册端口通信状态
		DB_SetEventFlag(pTask,APP_EVENT_COMM);
		
		// 2.2 注册双网通信状态

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

		//按照端口分别记录通信状态
		switch(MAJOR_PORT(wPort))
		{
			case SC_PORT_ETHERNETA:	DAE_SetRunningFlags(SS_ETHA_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_ETHERNETB:	DAE_SetRunningFlags(SS_ETHB_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
			case SC_PORT_SERIAL:	DAE_SetRunningFlags(SS_SER_COMM_ERR+MINOR_PORT(wPort)-2); break; 	
			case SC_PORT_CANBUS:	DAE_SetRunningFlags(SS_CAN_COMM_ERR+MINOR_PORT(wPort)-1); break; 	
		}

		// 2.3 注册通信事件
		DB_Register_Comm(1,wPort);
	}
	
	
	return DB_ERR_OK;
}

// 获取端口通信状态
STATUS DAE_GetCommStatus(WORD wPort,BYTE &Status)  //lz 20140314
{
 	TAppInfo	tTask;
	STATUS		rc;
	BOOL 		bBreak = TRUE;
	
	// 根据端口号查找相应的任务
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
	
	// 根据端口号查找相应的任务
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
	// 1.填充标志
	G_tBasicConfig.dwInitFlag = BAISIC_INIT_FLAG;

	// 2.计算校验
	DAE_MakeCheckSum((BYTE *)&G_tBasicConfig,sizeof(TBasicConfig)-2);

	// 3.写入EEPROM
	return EEPROM_Write(EEPROM_BASICCONFIG_START,sizeof(TBasicConfig),&G_tBasicConfig);
}

/************************************************************
	功能：获取镜像位对象通信状态
	参数：pStatus		--   通断状态位串指针
	返回：正确时返回DB_ERR_OK，错误返回错误码
************************************************************/
STATUS DAE_GetMBOCommStatus(BYTE *pStatus)
{
	if(pStatus == NULL)
		return DB_ERR_PTRNULL;

	// 清空
	memset(pStatus,0,(MAX_MAPBITOBJ_NUM+7)/8);

	// 逐位填写
	for(int i=0;i<MAX_MAPBITOBJ_NUM;i++)
	{
		if(DAE_GetRunningFlags(SS_MAPBIT_COMM_ERR+i))
			pStatus[i/8] |= 0x01<<(i%8);
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：读取透明数据缓冲区尾指针。
	参数：无
	返回：事件缓冲区尾指针。
************************************************************/
WORD DAE_GetTransDataTail()
{
	return g_GLBTransWrap.nTailIndex%DAE_MAX_TRANSNUM;
}

/************************************************************
	功能：从数据库中读取一条透明数据
	参数：	rp   	--  读取数据用的指针
			*event	--	返回的事项指针
	返回：正确返回0，错误返回错误代码
************************************************************/
WORD DAE_ReadTransData(BYTE *pBuf,WORD rp)
{
	TFixTransData	*pEvent;
	WORD			wLen;
	WORD			wPortID = DAE_GetCurrentTaskPortId();
	
    // 1。检查事项指针的合法性
    if( pBuf==NULL )
        return 0;
                     
	rp = rp%MAX_RUNREC_NUM;
                       
	// 2。读取事项信息
	pEvent = &g_GLBTransWrap.pEvent[rp];

	// 3 检查本端口数据
	if((pEvent->tHead.wDesPort != wPortID)&&(pEvent->tHead.wDesPort != DAE_TRANSPORT_BROADCAST))
		return 0;

	// 4、拷贝数据
	wLen = pEvent->tHead.wDataLen+sizeof(TTransDataHead)-1;
	if(wLen > DAE_MAX_TRANSLEN)
	{
		LogError("DB_ReadTransData",FILE_LINE,"Trans data length is overflow! (%d)",wLen);
		wLen = DAE_MAX_TRANSLEN;
	}
	memcpy(pBuf,pEvent,wLen);

	// 5、返回结果
	return (pEvent->tHead.wDataLen+sizeof(TTransDataHead)-1);
}
		
/************************************************************
	功能：向透明数据库中写入一条透明数据。
	参数：*str,		--  运行记录字符串
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DAE_WriteTransData(WORD wClass,WORD wDesPort,WORD wDataLen,BYTE *pBuf)
{
	TFixTransData	*pEvent;
	
	// 1。检查事项指针的合法性
	if( pBuf == NULL )
		 return DB_ERR_BUFNULL;


	// 2、写入事项
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

	// 3、消息通知
	if(wDesPort == DAE_TRANSPORT_BROADCAST)
	{
		// 广播消息
		for(int i=0;i<DAE_MAX_APP_NUM;i++)
		{
			if(!(G_pCfgTable->TaskApp[i].bMask & APP_EVENT_OIE))
				continue;

			// 剔除自身
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
	功能：获取对应以太网口的MAC信息
	参数：devno		--   网口编号
	参数：pMac		--   需要获取的MAC信息
	返回：正确时返回DB_ERR_OK，错误返回错误码
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

