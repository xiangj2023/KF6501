/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Daeinit.cpp                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义了系统数据库初始化的相关函数															*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      李政																							*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* 函数																									*/
/*																										*/
/*      CeateDAE()						创建数据库运行环境												*/
/*      CreateDbase()					初始化数据库													*/
/*      CreateTask()					创建系统任务													*/
/*		DbaseBackUp()					数据库备份														*/
/*      CheckSystemState()				检测系统状态													*/
/*		CheckHard()						检测硬件														*/
/*		CheckCfgTable()					检测配置表														*/
/*		CheckNvMalloc()					检测NvRAM内存链													*/
/*      InitCfgTable()					初始化数据库配置表												*/
/*      InitBasicParaBase()				初始化常规参数库												*/
/*      InitRelayDispBase()				初始化保护描述库												*/
/*      InitRelayDataBase()				初始化保护数据库												*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*      Daefunc.h                       系统库函数定义													*/
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
#include "protocol.h"
#include "relay.h"

TTransWrap		g_GLBTransWrap;
TBasicConfig	G_tBasicConfig;
TSysConfigTable	*G_pCfgTable = (TSysConfigTable *)DBASE_NVRAM_BASE;
HANDLE			g_hBOTaskID = APP_TASKID_NULL;
HANDLE			g_hLCDTaskID = APP_TASKID_NULL;
HANDLE			g_hRelayTaskID = APP_TASKID_NULL;
HANDLE			g_hDbaseTaskID = APP_TASKID_NULL;
HANDLE			g_hWdogTaskID = APP_TASKID_NULL;


DWORD	g_RunningFlag[DAE_MAX_FLAGS/32];

STATUS 	DAE_InitFileMap(WORD wType,void *pFile);
void 	DAE_InitDbaseFile();

BOOL 	DAE_CheckHard();
BOOL 	DAE_CheckSysStatus();
BOOL 	DAE_CheckNvMalloc();
BOOL 	DAE_CheckMISCWrap();
BOOL 	DAE_CheckRecordWrap();

void	DAE_InitSysStatus();
void 	DAE_InitConfig();

STATUS 	DAE_CreateDbase();
STATUS 	DAE_CreateTask();
STATUS	DAE_CreateMISCWrap();
STATUS	DAE_CreateRecordWrap();

STATUS 	InitBasicParaBase();
STATUS 	InitRelayDataBase();
STATUS 	InitRelayDispBase();
STATUS 	InitSETFixDataBase();
STATUS 	InitEventDataBase();
STATUS 	InitOnlineDataBase();

void 	MisiDaemon(DWORD *arg);
void    NetMainTask(DWORD *arg);
void 	CanLinkTask(DWORD *pdwApp);
void 	DBaseTask(DWORD *pdwApp);
void 	WDogTask(DWORD *pdwApp);
void 	LcdTask(DWORD *pdwApp);
void 	RelayTask(DWORD *pdwApp);
void 	DITask(DWORD *pdwApp);
void 	BOTask(DWORD *pdwApp);
void 	AITask(DWORD *pdwApp);

void 	BackUpTask(DWORD *pdwApp);
void 	MapBitTask(DWORD *pdwApp);
void 	FLTask(DWORD *pdwApp);

extern BOOL			g_bPowerOff = FALSE;
extern TMiscEvent	g_tPowerOffMisc;
extern TMiscEvent	g_tPowerOnMisc;
extern TMiscEvent	g_tResetMisc;

//定义规约任务表
TAppItem  ProTaskTab[]=
{
	DEFINE_APPLICATION("NotU",NULL,0,TASK_STACK_LEN),				/* 0  空规约*/
	DEFINE_APPLICATION("NetPro",SCanbus_Task ,NETPRO_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("weihu",SCanbus_Task ,SHELL_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("Shell",WHShell_Task,SHELL_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("IEC103",S103_Task,NETPRO_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("IEC101",NULL,REMOTE_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("IEC104",S104_Task,REMOTE_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("GM104",SGm104_Task,REMOTE_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("ModRtu",MModRtu_Task,REMOTE_PRIORITY,TASK_STACK_LEN)
};

//得到规约入口项个数
#define MAX_PROTOCOL_NUM	(sizeof(ProTaskTab)/sizeof(TAppItem))

#define		SYSTEM_TASK_NULL			0
#define		SYSTEM_TASK_SERADMIN		1
#define		SYSTEM_TASK_CANADMIN		2
#define		SYSTEM_TASK_ETHMAIN			3
#define		SYSTEM_TASK_DBASE			4
#define		SYSTEM_TASK_AI				5
#define		SYSTEM_TASK_DI				6
#define		SYSTEM_TASK_BO				7
#define		SYSTEM_TASK_WATCHDOG		8
#define		SYSTEM_TASK_RELAY			9
#define		SYSTEM_TASK_BACKUP			10
#define		SYSTEM_TASK_MAPBIT			11
#define		SYSTEM_TASK_LCD				12
#define		SYSTEM_TASK_FL				13

//定义系统任务表
TAppItem SysTaskTab[]=
{		
	DEFINE_APPLICATION("NotU",NULL,0,TASK_STACK_LEN),				/* 0  */
	DEFINE_APPLICATION("MISI",MisiDaemon,MISI_PRIORITY,10000),
	DEFINE_APPLICATION("CAN",CanLinkTask,CAN_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("ETH",NetMainTask,ETHERNET_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("Dbase",DBaseTask,DB_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("AI",AITask,AI_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("DI",DITask,DI_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("BO",BOTask,BO_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("WDog",WDogTask,WDOG_PRIORITY,10000),
	DEFINE_APPLICATION("Relay",RelayTask,RELAY_PRIORITY,40000),
	DEFINE_APPLICATION("BACK",BackUpTask,BACKUP_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("MAP",NULL,MAPBIT_PRIORITY,TASK_STACK_LEN),
	DEFINE_APPLICATION("Lcd",LcdTask,LCD_PRIORITY,50000),
#if(MODUEL_TYPE == MT_KF6571)
	DEFINE_APPLICATION("FL",FLTask,LCD_PRIORITY,50000),
#endif	
};

void CreateDAE()
{		
    // 1. 初始化调试信息库
    InitDebugInfo();

	// 2。初始化运行环境。
	DAE_InitConfig();

	// 3。初始化文件
	DAE_InitDbaseFile();
	
	// 4。根据检测结果初始化数据库
	DAE_CreateDbase();

	// 5.注册停上电&复位事件
	if(g_bPowerOff) 	
	{		
		DB_Misc_Write(&g_tPowerOffMisc);
		DB_Misc_Write(&g_tPowerOnMisc);
	}

	if(DAE_GetInitFlags(SS_NVRAM_INIT)||DAE_GetInitFlags(SS_NVRAM_LINK_ERR))
		DB_Register_Reset(SYS_RESET_COLD);
	else
		DB_Register_Reset(SYS_RESET_WARM);

	// 5.以初始化标志填充运行标志

	//集中判断EEPROM出错事件
	if(DAE_GetInitFlags(SS_EEPROM_SW_ERR)||DAE_GetInitFlags(SS_EEPROM_SET_ERR)\
		||DAE_GetInitFlags(SS_EEPROM_SYSSET_ERR)||DAE_GetInitFlags(SS_EEPROM_SETAREA_ERR)\
		||DAE_GetInitFlags(SS_EEPROM_CF_ERR))
	{
		DAE_SetInitFlags(SS_EEPROM_DATA_ERR);
	}
	else
	{
		DAE_ClearInitFlags(SS_EEPROM_DATA_ERR);
	}

	memcpy(&g_RunningFlag,&G_pCfgTable->SystemStatus.InitStatus,sizeof(DAEFLAGS));

	// 初始化透传数据缓冲区
	memset(&g_GLBTransWrap,0,sizeof(TTransWrap));

	// 初始化规约参数
	InitProtocolPara();

	// 6。根据检测结果创建任务
	DAE_CreateTask();
}

/*初始化文件镜像*/
STATUS DAE_InitFileMap(WORD wType,void *pFile)
{
    TFileHead         		*filehead;
	TFileMap 				*pFileMap;
	TDBaseConfigTable		*pDbase;
	
	pDbase = &G_pCfgTable->DBaseConfig;

	ASSERT(pFile != NULL);
	if(pFile == NULL)
		return DB_ERR_BUFNULL;
	
	//分配RAM镜像节点
	pFileMap = (TFileMap *)OS_MemAlloc(sizeof(TFileMap));
	ASSERT(pFileMap != NULL);
	if(pFileMap == NULL)
		return DB_ERR_MEM_ALLOC;
	
	//分类初始化镜像区
	switch(wType)
	{
	case DB_FILE_PARACFG:
		pDbase->pBasicParaFileMap = pFileMap;
		strcpy(pFileMap->sFileName,PARACFGFILE);
		break;
	case DB_FILE_RELAYDATA:
		pDbase->pRelayDataFileMap = pFileMap;
		strcpy(pFileMap->sFileName,RELAYDATAFILE);
		break;
	case DB_FILE_RELAYEV:		
		pDbase->pRelayEventFileMap = pFileMap;
		strcpy(pFileMap->sFileName,RELAYEVENTFILE);
		break;
	case DB_FILE_BASICEV:
		pDbase->pBasicEventFileMap = pFileMap;
		strcpy(pFileMap->sFileName,BASICEVENTFILE);
		break;
	case DB_FILE_RELAYDISP:
		pDbase->pRelayDispFileMap = pFileMap;
		strcpy(pFileMap->sFileName,RELAYDISPFILE);
		break;
	default:
		return DB_ERR_FILE_TYPE;
	}	
	

	filehead = (TFileHead *)pFile;
	pFileMap->wFileType 	= 0;
	pFileMap->wFlag			= FILEMAP_FLAGS_INIT;
	pFileMap->pRamData 		= pFile;
	pFileMap->dwFlashOff 	= NULL;
	pFileMap->nSize 		= filehead->dwSize;
	pFileMap->absTime		= G_pCfgTable->SystemStatus.stTime;
	pFileMap->wCheck		= DAE_CheckSum((BYTE *)pFileMap,(BYTE *)&pFileMap->wCheck-(BYTE *)pFileMap);

	return DB_ERR_OK;
}

void DAE_InitDbaseFile()
{
    TFileHead         		*filehead;
	BYTE					*pFileBuf = NULL;
    DWORD 					filelength;
	STATUS 					rc;
	
	// 初始化参数文件指针
	G_pCfgTable->DBaseConfig.pBasicParaFileMap = NULL;
	G_pCfgTable->DBaseConfig.pRelayDispFileMap = NULL;
	G_pCfgTable->DBaseConfig.pRelayDataFileMap = NULL;
	G_pCfgTable->DBaseConfig.pBasicEventFileMap = NULL;
	G_pCfgTable->DBaseConfig.pRelayEventFileMap = NULL;

	
	// 1.=============读取基本参数文件=============
	
	do{				
		pFileBuf = NULL;
		rc = DB_ERR_FAILURE;
		
		// 1.1 读取文件长度
		rc = DAE_GetFileLength(PARACFGFILE,&filelength);
		
		if( rc != DB_ERR_OK)
		{
	        LogError("DAE_InitDbaseFile",FILE_LINE,"Read file paraconf.dar length error!");
			break;
		}
		
		if(filelength > MAX_FILE_LEN)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"File paraconf.dar length(%d) Error!",filelength);
			break;
		}

		// 1.2 初始化分配文件缓冲区
		pFileBuf = (BYTE *)OS_MemAlloc(filelength);
		if( pFileBuf == NULL )
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Malloc paraconf.dar file buffer error!!!");
			break;
		}
			
		// 1.2 读取文件内容
		rc = DAE_ReadFile(PARACFGFILE,pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Read paraconf.dar file Error!");
			break;
		}

		rc = DAE_CheckSystemFile(pFileBuf,filelength);
	    if( rc != DB_ERR_OK)
	    {
	        LogError("DAE_InitDbaseFile",FILE_LINE,"paraconf.dar file check error!");
			break;
	    }
		
		// 1.3 初始化文件镜像区
		DAE_InitFileMap(DB_FILE_PARACFG,pFileBuf);
		
		rc = DB_ERR_OK;
	}while(0);

	//  异常处理
	if( rc != DB_ERR_OK)
		DAE_SetInitFlags(SS_FILE_BASICPARA_ERR);
	else
		DAE_ClearInitFlags(SS_FILE_BASICPARA_ERR);
	
	// 2。=============初始化保护数据文件=============
	do{
		pFileBuf = NULL;
		rc = DB_ERR_FAILURE;
		
		// 2.1 计算文件长度
		DAE_GetFileLength(RELAYDATAFILE,&filelength);
	
		// 2.2 初始化分配文件缓冲区
		pFileBuf = (BYTE *)OS_MemAlloc(filelength);
		if( pFileBuf == NULL )
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Malloc relayconf.dar file buffer error!!!");
			break;
		}
			
		// 2.3 读取文件内容
		rc = DAE_ReadFile(RELAYDATAFILE,pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Read relayconf.dar file Error!");
			break;
		}
		
		rc = DAE_CheckSystemFile(pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"relayconf.dar file check error!");
			break;
		}		
		rc = DB_ERR_OK;
	}while(0);

	// 无论什么结果都需要保留指针，为数据库创建做准备
	DAE_InitFileMap(DB_FILE_RELAYDATA,pFileBuf);

	//  异常处理
	if( rc != DB_ERR_OK)
		DAE_SetInitFlags(SS_FILE_RELAYDATA_ERR);
	else
		DAE_ClearInitFlags(SS_FILE_RELAYDATA_ERR);
		
	// 3。=============初始化保护描述文件=============
	do{		
		pFileBuf = NULL;
		rc = DB_ERR_FAILURE;
		
		// 3.1 读取文件长度
		rc = DAE_GetFileLength(RELAYDISPFILE,&filelength);
		
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Read relaydisp.dar file length error!");
			break;
		}
		
		if(filelength > MAX_FILE_LEN)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"relaydisp.dar file length Error!");
			break;
		}
		
		// 3.2 初始化分配文件缓冲区
		pFileBuf = (BYTE *)OS_MemAlloc(filelength);
		if( pFileBuf == NULL )
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Malloc relaydisp.dar file buffer error!!!");
			break;
		}
			
		// 3.3 读取文件内容
		rc = DAE_ReadFile(RELAYDISPFILE,pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Read relaydisp.dar file Error!");
			break;
		}
		
		rc = DAE_CheckSystemFile(pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"relaydisp.dar file check error!");
			break;
		}
		
		// 3.3 初始化文件镜像区
		DAE_InitFileMap(DB_FILE_RELAYDISP,pFileBuf);	
		rc = DB_ERR_OK;
	}while(0);
	
	if( rc != DB_ERR_OK)
		DAE_SetInitFlags(SS_FILE_RELAYDISP_ERR);
	else
		DAE_ClearInitFlags(SS_FILE_RELAYDISP_ERR);
		
	// 4。=============初始化基本事项文件=============
	do{		
		pFileBuf = NULL;
		rc = DB_ERR_FAILURE;
		
		// 4.1 计算文件长度
		filelength = sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
					+sizeof(TMiscEvent)*MAX_MISC_NUM\
					+sizeof(TEventBI)*MAX_SOE_NUM\
					+sizeof(WORD);

		// 4.2 初始化分配文件缓冲区
		pFileBuf = (BYTE *)OS_MemAlloc(filelength);
		if( pFileBuf == NULL )
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Malloc basicevent.dat file buffer error!!!");
			break;
		}
			
		// 4.3 读取文件内容
		rc = DAE_ReadFile(BASICEVENTFILE,pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Read basicevent.dat file Error!");
			break;
		}
		
		rc = DAE_CheckSystemFile(pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"basicevent.dat file check error!");
			break;
		}		
			
		rc = DB_ERR_OK;
	}while(0);	
	
	// 无论什么结果都需要保留指针，为数据库创建做准备
	// 4.3 初始化文件镜像区
	DAE_InitFileMap(DB_FILE_BASICEV,pFileBuf);
	
	if( rc != DB_ERR_OK)
		DAE_SetInitFlags(SS_FILE_BASICEVENT_ERR);
	else
		DAE_ClearInitFlags(SS_FILE_BASICEVENT_ERR);
		
	// 4。=============初始化保护报告文件=============
	do{		
		pFileBuf = NULL;
		rc = DB_ERR_FAILURE;
		
		// 2.1 计算文件长度
		filelength = sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
					+sizeof(TRelayActionEv)*MAX_RACT_NUM\
					+sizeof(TRelayAlarmEv)*MAX_RALM_NUM+sizeof(WORD);

		// 2.2 初始化分配文件缓冲区
		pFileBuf = (BYTE *)OS_MemAlloc(filelength);
		if( pFileBuf == NULL )
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Malloc relayevent.dat file buffer error!!!");
			break;
		}
			
		// 2.3 读取文件内容
		rc = DAE_ReadFile(RELAYEVENTFILE,pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"Read relayevent.dat file Error!");
			break;
		}
		
		rc = DAE_CheckSystemFile(pFileBuf,filelength);
		if( rc != DB_ERR_OK)
		{
			LogError("DAE_InitDbaseFile",FILE_LINE,"relayevent.dat file check error!");
			break;
		}		
		rc = DB_ERR_OK;
	}while(0);	
	
	// 无论什么结果都需要保留指针，为数据库创建做准备
	DAE_InitFileMap(DB_FILE_RELAYEV,pFileBuf);
	
	if( rc != DB_ERR_OK)
		DAE_SetInitFlags(SS_FILE_RELAYEVENT_ERR);
	else
		DAE_ClearInitFlags(SS_FILE_RELAYEVENT_ERR);
}

void DAE_InitConfig()
{
//	memset(&G_pCfgTable->SystemStatus.InitStatus,0,sizeof(DAEFLAGS));
	// 恢复重启前状态
	memcpy(&G_pCfgTable->SystemStatus.InitStatus,&G_pCfgTable->SystemStatus.Status,sizeof(DAEFLAGS));
	
	// 1。检测硬件
	if(!DAE_CheckHard())
		DAE_SetInitFlags(SS_SYS_HARD_ERR);		
	else
		DAE_ClearInitFlags(SS_SYS_HARD_ERR);		
		
	// 2。检查配置状态
	DAE_ClearInitFlags(SS_NVRAM_INIT);
	
	if(!DAE_CheckSysStatus())
	{		
		DAE_InitSysStatus();
		DAE_SetInitFlags(SS_SYS_STATUS_ERR);	
		
		DAE_InitNvMem();
		DAE_SetInitFlags(SS_NVRAM_INIT);
		LogError("DAE_InitConfig",FILE_LINE,"System restart cold......");
		return;
	}
	else
	{
		DAE_ClearInitFlags(SS_SYS_STATUS_ERR);	
	}
	
	// 3。检查NvRAM内存链
	if(!DAE_CheckNvMalloc())
	{
		LogError("DAE_InitConfig",FILE_LINE,"Nvram check error!!!");
		DAE_SetInitFlags(SS_NVRAM_LINK_ERR);
		
		DAE_InitNvMem();
		DAE_SetInitFlags(SS_NVRAM_INIT);
	}
	else
	{
		DAE_ClearInitFlags(SS_NVRAM_LINK_ERR);
	}
}

/*初始化系统状态*/
void	DAE_InitSysStatus()
{
	TStatus				*pSystemStatus = &G_pCfgTable->SystemStatus;
	TSysTime			time;

	ReadSystemTime(&time);
	
	// 1. NvRAM结构清零
	memset(G_pCfgTable,0,sizeof(TSysConfigTable));

	// 2。初始化系统状态
	
	// 	2。1 初始化系统运行标志 
	pSystemStatus->dwFlag = SYS_RUNNING_OK;

	//  2。2 初始化时间信息
	WriteSystemTime(&time);
}

/*创建操作记录库*/
STATUS	DAE_CreateMISCWrap()
{
	TMiscWrap 			*pMisc;
	BYTE				*pTemp = NULL;
	TBasicEventPtrList	*pBasicEventPtr;
	DWORD 				rc,filelength;

	pMisc = &G_pCfgTable->MiscWrap;

	if((!DAE_GetInitFlags(SS_NVRAM_LINK_ERR))&&(!DAE_GetInitFlags(SS_NVRAM_INIT)))
	{
		if((pMisc->dwMagic == WRAP_INIT_FLAG)&&(pMisc->nMax == MAX_MISC_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_MISC_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_MISC_ERR);
	}

	/*清除空间*/
	memset(pMisc,0,sizeof(TMiscWrap));
	
	/*初始化标志*/
	pMisc->dwMagic = WRAP_INIT_FLAG;
	pMisc->nMax = MAX_MISC_NUM;
	pMisc->nHeadIndex = 0;
	pMisc->nTailIndex = 0;
	
	/*从FLASH中恢复事件记录*/

	if(DAE_GetInitFlags(SS_FILE_BASICEVENT_ERR))
		return DB_ERR_OK;

	pTemp = (BYTE *)G_pCfgTable->DBaseConfig.pBasicEventFileMap->pRamData;
		
	/*恢复数据*/
	pBasicEventPtr = (TBasicEventPtrList *)((BYTE *)pTemp+sizeof(TFileHead));
		
	pMisc->nHeadIndex = pBasicEventPtr->nOptRecordH%MAX_MISC_NUM;
	pMisc->nTailIndex = pBasicEventPtr->nOptRecordT%MAX_MISC_NUM;
	memcpy(pMisc->pEvent,(BYTE *)(pBasicEventPtr+1)+sizeof(TEventBI)*MAX_SOE_NUM,sizeof(TMiscEvent)*MAX_MISC_NUM);

	LogError("DAE_CreateMISCWrap",FILE_LINE,"Recover data from file......");
	
	return DB_ERR_FAILURE;
}

/*创建运行记录库*/
STATUS	DAE_CreateRecordWrap()
{
	TRecordWrap 		*pRecord;
	
	pRecord = &G_pCfgTable->RecordWrap;

	if((!DAE_GetInitFlags(SS_NVRAM_LINK_ERR))&&(!DAE_GetInitFlags(SS_NVRAM_INIT)))
	{
		if((pRecord->dwMagic == WRAP_INIT_FLAG)&&(pRecord->nMax == MAX_RUNREC_NUM))
		{
			DAE_ClearInitFlags(SS_NVRAM_RECORD_ERR);
			return DB_ERR_OK;
		}
		else
			DAE_SetInitFlags(SS_NVRAM_RECORD_ERR);
	}

	/*清除空间*/
	memset(pRecord,0,sizeof(TRecordWrap));
	
	/*初始化标志*/
	pRecord->dwMagic = WRAP_INIT_FLAG;
	pRecord->nMax = MAX_RUNREC_NUM;
	pRecord->nHeadIndex = 0;
	pRecord->nTailIndex = 0;

	LogError("DAE_CreateRecordWrap",FILE_LINE,"Initial nvram data......");
	
	return DB_ERR_FAILURE;
}

STATUS DAE_InitBasicConfig()
{
	// 从EEPROM中读取基本配置参数
	if(EEPROM_Read(EEPROM_BASICCONFIG_START,sizeof(TBasicConfig),&G_tBasicConfig) != DB_ERR_OK)
		return DB_ERR_CHECK;

	do{
		// 检查初始化标志
		if(G_tBasicConfig.dwInitFlag != BAISIC_INIT_FLAG)
			break;

		// 检查校验和标志位
		if(!DAE_IsCheckSumOK((BYTE *)&G_tBasicConfig,sizeof(TBasicConfig)-2))		
		{
			memset(&G_tBasicConfig,0,sizeof(TBasicConfig));
			break;
		}
		
		return DB_ERR_OK;
	}while(0);
	
	// 初始化为缺省值
	memset(&G_tBasicConfig,0,sizeof(TBasicConfig));
	
	G_tBasicConfig.dwNetAIP = (192<<24)+(168<<16)+((100)<<8)+200;
	G_tBasicConfig.dwNetAMask = 0xFFFFFF00;
	G_tBasicConfig.dwNetAGWIP = (192<<24)+(168<<16)+((100)<<8)+1;
	G_tBasicConfig.dwNetBIP = (172<<24)+(20<<16)+((10)<<8)+200;
	G_tBasicConfig.dwNetBMask = 0xFFFFFF00;
	G_tBasicConfig.dwNetBIP = (172<<24)+(20<<16)+((10)<<8)+1;
	G_tBasicConfig.dwNetBGWIP = (172<<24)+(20<<16)+((10)<<8)+1;
	G_tBasicConfig.wNetAddress = 1;
	strcat(G_tBasicConfig.chPassWord1,"6500");
	strcat(G_tBasicConfig.chPassWord2,"6500");

	ReportMsg("Read basic config para from EEPROM fail!");
	
	return DB_ERR_CHECK;
}

STATUS DAE_CreateDbase()
{
	static	STATUS	status;

	// 从EEPROM中读取基本配置参数
	status = DAE_InitBasicConfig();
	
	// 检查操作记录库
	status |= DAE_CreateMISCWrap();
	
	// 检查运行记录库
	status |= DAE_CreateRecordWrap();
	
	// 1。初始化常规参数库
	status |= InitBasicParaBase();
	
	// 2。初始化保护数据库
	status |= InitRelayDataBase();

	// 3。初始化保护描述库
	status |= InitRelayDispBase();

	// 4。初始化定点定值库
	status |= InitSETFixDataBase();
	
	// 5。初始化事件数据库
	status |= InitEventDataBase();

	// 6。初始化在线修改参数库
	status |= InitOnlineDataBase();

	// 基本参数出错时需要回填
	if(G_tBasicConfig.dwInitFlag != BAISIC_INIT_FLAG)
		DAE_UpdateBasicConfig();

	return status;
}

void DAE_UpdateTaskEventReadPtr(TAppInfo *pAppInfo)
{
	TDBaseConfigTable		*pDBaseConfig;
	WORD	nTotalNum,unReadNum;
	
	pDBaseConfig = &G_pCfgTable->DBaseConfig;

	// 1.修正变位遥信读指针
    unReadNum = (pDBaseConfig->pBurstBIWrap->nTailIndex+MAX_BIE_NUM-pAppInfo->nBIERead)%MAX_BIE_NUM;
    nTotalNum = (pDBaseConfig->pBurstBIWrap->nTailIndex+MAX_BIE_NUM-pDBaseConfig->pBurstBIWrap->nHeadIndex)%MAX_BIE_NUM;
	if(unReadNum > nTotalNum)
	{
		pAppInfo->nBIERead	= pDBaseConfig->pBurstBIWrap->nTailIndex;
		LogError("DAE_CreateTask",FILE_LINE,"Port 0x%x BIE read index error",pAppInfo->dwPortID);
	}
	
	// 2.修正SOE读指针
    unReadNum = (pDBaseConfig->pEventBIWrap->nTailIndex+MAX_SOE_NUM-pAppInfo->nSOERead)%MAX_SOE_NUM;
    nTotalNum = (pDBaseConfig->pEventBIWrap->nTailIndex+MAX_SOE_NUM-pDBaseConfig->pEventBIWrap->nHeadIndex)%MAX_SOE_NUM;
	if(unReadNum > nTotalNum)
	{
		pAppInfo->nSOERead	= pDBaseConfig->pEventBIWrap->nTailIndex;
		LogError("DAE_CreateTask",FILE_LINE,"Port 0x%x SOE read index error",pAppInfo->dwPortID);
	}
	
	// 3.修正保护启动读指针
    unReadNum = (pDBaseConfig->pRSTTWrap->nTailIndex+MAX_RSTT_NUM-pAppInfo->nRSTTRead)%MAX_RSTT_NUM;
    nTotalNum = (pDBaseConfig->pRSTTWrap->nTailIndex+MAX_RSTT_NUM-pDBaseConfig->pRSTTWrap->nHeadIndex)%MAX_RSTT_NUM;
	if(unReadNum > nTotalNum)
	{
		pAppInfo->nRSTTRead	= pDBaseConfig->pRSTTWrap->nTailIndex;
		LogError("DAE_CreateTask",FILE_LINE,"Port 0x%x RSTT read index error",pAppInfo->dwPortID);
	}
	
	// 4.修正保护动作读指针
    unReadNum = (pDBaseConfig->pRACTWrap->nTailIndex+MAX_RACT_NUM-pAppInfo->nRACTRead)%MAX_RACT_NUM;
    nTotalNum = (pDBaseConfig->pRACTWrap->nTailIndex+MAX_RACT_NUM-pDBaseConfig->pRACTWrap->nHeadIndex)%MAX_RACT_NUM;
	if(unReadNum > nTotalNum)
	{
		pAppInfo->nRACTRead	= pDBaseConfig->pRACTWrap->nTailIndex;
		LogError("DAE_CreateTask",FILE_LINE,"Port 0x%x RACT read index error",pAppInfo->dwPortID);
	}
	
	// 5.修正保护告警读指针
    unReadNum = (pDBaseConfig->pRALMWrap->nTailIndex+MAX_RALM_NUM-pAppInfo->nRALMRead)%MAX_RALM_NUM;
    nTotalNum = (pDBaseConfig->pRALMWrap->nTailIndex+MAX_RALM_NUM-pDBaseConfig->pRALMWrap->nHeadIndex)%MAX_RALM_NUM;
	if(unReadNum > nTotalNum)
	{
		pAppInfo->nRALMRead	= pDBaseConfig->pRALMWrap->nTailIndex;	
		LogError("DAE_CreateTask",FILE_LINE,"Port 0x%x RALM read index error",pAppInfo->dwPortID);
	}
}
	
TAppInfo *DAE_SearchTask(WORD wPortID)
{
    TAppInfo 				*pAppInfo;
	TDBaseConfigTable		*pDBaseConfig;
	int 					i,j;
	
	pDBaseConfig = &G_pCfgTable->DBaseConfig;
		
	pAppInfo =  G_pCfgTable->TaskApp;

	// 1。先查找该端口对应的任务信息
	for(i=SYS_PORT_MAX,j=-1;i<DAE_MAX_APP_NUM;i++)
	{
		if(pAppInfo[i].dwPortID == wPortID)
			return &pAppInfo[i];

		// 标记第一个空任务
		if((j==-1)&&(pAppInfo[i].dwPortID == 0)&&(pAppInfo[i].pAppItem == NULL))
			j = i;
	}

	// 2。没有找到则返回第一个空任务指针
	if(j > 0)
	{
		// 初始化事件指针
		pAppInfo[j].nBIERead	= pDBaseConfig->pBurstBIWrap->nTailIndex;
		pAppInfo[j].nSOERead	= pDBaseConfig->pEventBIWrap->nTailIndex;
		pAppInfo[j].nRSTTRead 	= pDBaseConfig->pRSTTWrap->nTailIndex;
		pAppInfo[j].nRACTRead 	= pDBaseConfig->pRACTWrap->nTailIndex;
		pAppInfo[j].nRALMRead 	= pDBaseConfig->pRALMWrap->nTailIndex;
		pAppInfo[j].nLogRead	= G_pCfgTable->RecordWrap.nTailIndex;
		pAppInfo[j].nMISCRead 	= G_pCfgTable->MiscWrap.nTailIndex;
		
		return &pAppInfo[j];
	}
	else
	{
		LogError("DAE_SearchTask",FILE_LINE,"Search Nothing!!!");
		return NULL;
	}
}

STATUS DAE_CreateTask()
{
	TDBaseConfigTable		*pDBaseConfig;				//数据库配置表
	TAppItem 				*pAppItem;
    TAppInfo 				*pAppInfo;
	TRelayEventPtrList		*pRelayEventPtr;
	TFileMap				*pFileMap;
	WORD					nTaskAPP;
	int 					i;
		
	pDBaseConfig = &G_pCfgTable->DBaseConfig;
	pAppInfo =  G_pCfgTable->TaskApp;

	// 如果内存链破坏，重新构建
	if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
		OS_MemSet(pAppInfo,0,sizeof(TAppInfo)*DAE_MAX_APP_NUM);
	
	for(i=0;i<DAE_MAX_APP_NUM;i++)
	{
		pAppInfo[i].hTaskID	= APP_TASKID_NULL;
		pAppInfo[i].pAppItem = NULL;
	}
	
	// 1 注册系统任务	Lcd、Dbase、Misi、WatchDag、AI、BI、BO等
	
	pAppInfo[0].dwPortID	= SYS_PORTID_SERADMIN;
	pAppInfo[0].bMask 		= 0;
	pAppInfo[0].pAppItem 	= &SysTaskTab[SYSTEM_TASK_SERADMIN];

	pAppInfo[1].dwPortID	= SYS_PORTID_CANADMIN;
	pAppInfo[1].pAppItem 	= &SysTaskTab[SYSTEM_TASK_CANADMIN];

	pAppInfo[2].dwPortID	= SYS_PORTID_ETHMAIN;
	pAppInfo[2].pAppItem 	= &SysTaskTab[SYSTEM_TASK_ETHMAIN];

	pAppInfo[3].dwPortID	= SYS_PORTID_DBASE;
	pAppInfo[3].pAppItem 	= &SysTaskTab[SYSTEM_TASK_DBASE];

	pAppInfo[4].dwPortID	= SYS_PORTID_AI;
	pAppInfo[4].pAppItem 	= &SysTaskTab[SYSTEM_TASK_AI];

	pAppInfo[5].dwPortID	= SYS_PORTID_DI;
	pAppInfo[5].pAppItem 	= &SysTaskTab[SYSTEM_TASK_DI];

	pAppInfo[6].dwPortID	= SYS_PORTID_BO;
	pAppInfo[6].pAppItem 	= &SysTaskTab[SYSTEM_TASK_BO];

	pAppInfo[7].dwPortID	= SYS_PORTID_WATCHDOG;
	pAppInfo[7].pAppItem 	= &SysTaskTab[SYSTEM_TASK_WATCHDOG];

	pAppInfo[8].dwPortID	= SYS_PORTID_RELAY;
	pAppInfo[8].pAppItem 	= &SysTaskTab[SYSTEM_TASK_RELAY];

	pAppInfo[9].dwPortID	= SYS_PORTID_BACKUP;
	pAppInfo[9].pAppItem 	= &SysTaskTab[SYSTEM_TASK_BACKUP];

	pAppInfo[10].dwPortID	= SYS_PORTID_MAPBIT;
	pAppInfo[10].pAppItem 	= &SysTaskTab[SYSTEM_TASK_MAPBIT];

	pAppInfo[11].dwPortID	= SYS_PORTID_LCD;
	pAppInfo[11].bMask 		= APP_EVENT_SOE|APP_EVENT_RALM|APP_EVENT_RSTT|APP_EVENT_RACT;
	pAppInfo[11].pAppItem 	= &SysTaskTab[SYSTEM_TASK_LCD];

	// 6571设备测距使能
#if(MODUEL_TYPE == MT_KF6571)
		pAppInfo[12].dwPortID	= SYS_PORTID_FL;	
		pAppInfo[12].bMask 		= APP_EVENT_OIE;	//需要透明数据
		pAppInfo[12].pAppItem 	= &SysTaskTab[SYSTEM_TASK_FL];
		nTaskAPP = 13;
#else
		nTaskAPP = 12;
#endif
	
	// 2。注册规约任务	
	
	// 2。1 注册串口任务
	for(i=0;i<pDBaseConfig->nSerNum;i++)
	{
		if(pDBaseConfig->pSerial[i].byDriver >= ArraySize(ProTaskTab))
		{
			LogError("DAE_CreateTask",FILE_LINE,"Serial driver no is overflow!!!");
			continue;
		}
		
		pAppInfo = DAE_SearchTask(pDBaseConfig->pSerial[i].wPortID);
		ASSERT(pAppInfo != NULL);
		if(pAppInfo != NULL)
		{
			pAppInfo->dwPortID	= pDBaseConfig->pSerial[i].wPortID;
			pAppInfo->pAppItem	= &ProTaskTab[pDBaseConfig->pSerial[i].byDriver];
			pAppInfo->bMask		= APP_EVENT_ALL;
		}
	}
	
	nTaskAPP += pDBaseConfig->nSerNum;
	
	// 2。2 注册Can网任务
	for(i=0;i<pDBaseConfig->nCanNum;i++)
	{
		if(pDBaseConfig->pCAN[i].byDriver >= ArraySize(ProTaskTab))
		{
			LogError("DAE_CreateTask",FILE_LINE,"Can driver no is overflow!!!");
			continue;
		}
		
		pAppInfo = DAE_SearchTask(pDBaseConfig->pCAN[i].wPortID);
		ASSERT(pAppInfo != NULL);
		if(pAppInfo != NULL)
		{
			pAppInfo->dwPortID	= pDBaseConfig->pCAN[i].wPortID;
			pAppInfo->pAppItem	= &ProTaskTab[pDBaseConfig->pCAN[i].byDriver];
			pAppInfo->bMask		= APP_EVENT_ALL;
		}
	}
	
	nTaskAPP += pDBaseConfig->nCanNum;

	// 2。3 注册以太网任务
	for(i=0;i<pDBaseConfig->nEthNum;i++)
	{
		if(pDBaseConfig->pEthernet[i].wDriver >= ArraySize(ProTaskTab))
		{
			LogError("DAE_CreateTask",FILE_LINE,"Ethernet driver no is overflow!!!");
			continue;
		}
		
		pAppInfo = DAE_SearchTask(pDBaseConfig->pEthernet[i].wPortID);
		ASSERT(pAppInfo != NULL);
		if(pAppInfo != NULL)
		{
			pAppInfo->dwPortID	= pDBaseConfig->pEthernet[i].wPortID;
			pAppInfo->pAppItem	= &ProTaskTab[pDBaseConfig->pEthernet[i].wDriver];
			pAppInfo->bMask		= APP_EVENT_ALL;
		}
	}
	
	nTaskAPP += pDBaseConfig->nEthNum;
	
	pDBaseConfig->nTaskAPP = nTaskAPP;

	// 4。创建任务
	pAppInfo =  G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++)
	{
		// 4。1 直接更新保护告警和动作事件指针,以EEPROM中存储的为准
		pFileMap = G_pCfgTable->DBaseConfig.pRelayEventFileMap;
/*
		if(pFileMap != NULL)
		{
			pRelayEventPtr = (TRelayEventPtrList *)((BYTE *)pFileMap->pRamData+sizeof(TFileHead));

			switch(pAppInfo[i].dwPortID)
			{
				case SYS_PORTID_LCD:
					pAppInfo[i].nRALMRead = pRelayEventPtr->nRelayAlarmLEDR;
					pAppInfo[i].nRACTRead = pRelayEventPtr->nRelayActionLEDR;
					break;
				case ((SC_PORT_CANBUS<<8)+1):
				case ((SC_PORT_ETHERNETA<<8)+1):
					pAppInfo[i].nRALMRead = pRelayEventPtr->nRelayAlarmNet1R;
					pAppInfo[i].nRACTRead = pRelayEventPtr->nRelayActionNet1R;
					break;
				case ((SC_PORT_CANBUS<<8)+2):
				case ((SC_PORT_ETHERNETB<<8)+1):
					pAppInfo[i].nRALMRead = pRelayEventPtr->nRelayAlarmNet2R;
					pAppInfo[i].nRACTRead = pRelayEventPtr->nRelayActionNet2R;
					break;
				default:	
					break;
			}
		}
*/		
		// 4。2 如果内存链破坏，则从数据库中更新SOE、遥信变位和保护启动事件指针
		if(DAE_GetInitFlags(SS_NVRAM_LINK_ERR)||DAE_GetInitFlags(SS_NVRAM_INIT))
		{
			pAppInfo[i].nBIERead	= pDBaseConfig->pBurstBIWrap->nTailIndex;
			pAppInfo[i].nSOERead	= pDBaseConfig->pEventBIWrap->nTailIndex;
			pAppInfo[i].nRSTTRead	= pDBaseConfig->pRSTTWrap->nTailIndex;
			pAppInfo[i].nLogRead	= G_pCfgTable->RecordWrap.nTailIndex;
			pAppInfo[i].nMISCRead	= G_pCfgTable->MiscWrap.nTailIndex;

			pAppInfo[i].nRALMRead = pDBaseConfig->pRALMWrap->nTailIndex;
			pAppInfo[i].nRACTRead = pDBaseConfig->pRACTWrap->nTailIndex;
		}

		// 不保存
		pAppInfo[i].nAIERead = 0;

		// 4。3 事件读指针异常时需要更新
		DAE_UpdateTaskEventReadPtr(&pAppInfo[i]);
		
		// 4。4　更新事项标志
		pAppInfo[i].bEvent = 0;

/*		
		//遥信变位事项
		if(pAppInfo[i].bMask&APP_EVENT_BIE)
		{
			if(pAppInfo[i].nBIERead	== pDBaseConfig->pBurstBIWrap->nTailIndex)
				pAppInfo[i].bEvent &= ~APP_EVENT_BIE;
			else
				pAppInfo[i].bEvent |= APP_EVENT_BIE;
		}
		
		//SOE事件
		if(pAppInfo[i].bMask&APP_EVENT_SOE)
		{
			if(pAppInfo[i].nSOERead	== pDBaseConfig->pEventBIWrap->nTailIndex)
				pAppInfo[i].bEvent &= ~APP_EVENT_SOE;
			else
				pAppInfo[i].bEvent |= APP_EVENT_SOE;
		}
		
		//保护启动事件
		if(pAppInfo[i].bMask&APP_EVENT_RSTT)
		{
			if(pAppInfo[i].nRSTTRead== pDBaseConfig->pRSTTWrap->nTailIndex)
				pAppInfo[i].bEvent &= ~APP_EVENT_RSTT;
			else
				pAppInfo[i].bEvent |= APP_EVENT_RSTT;
		}
		
		//保护告警事件
		if(pAppInfo[i].bMask&APP_EVENT_RALM)
		{
			if(pAppInfo[i].nRALMRead== pDBaseConfig->pRALMWrap->nTailIndex)
				pAppInfo[i].bEvent &= ~APP_EVENT_RALM;
			else
				pAppInfo[i].bEvent |= APP_EVENT_RALM;
		}
		
		//保护动作事件
		if(pAppInfo[i].bMask&APP_EVENT_RACT)
		{
			if(pAppInfo[i].nRACTRead== pDBaseConfig->pRACTWrap->nTailIndex)
				pAppInfo[i].bEvent &= ~APP_EVENT_RACT;
			else
				pAppInfo[i].bEvent |= APP_EVENT_RACT;
		}
*/
		
		// 4。4 创建任务
		pAppItem = pAppInfo[i].pAppItem;

		// 镜像端口不启动规约
		if(DB_IsMapBitPort(pAppInfo[i].dwPortID))
		{
			LogError("DAE_CreateTask",FILE_LINE,"Port:%x task exit for MAPBIT@@@@@@@@@@@@@@",pAppInfo[i].dwPortID);
			pAppItem = NULL;
		}
		
		if((pAppItem != NULL)&&(pAppItem->fpEntry != NULL))
			pAppInfo[i].hTaskID = OS_CreateTask(pAppItem->szName, pAppItem->fpEntry, pAppItem->wPriority, pAppItem->wSysStack, pAppInfo[i].dwPortID, 0, 0);

		// 4。5 记录关键任务的任务ID
		switch(pAppInfo[i].dwPortID)
		{
			case SYS_PORTID_BO:		g_hBOTaskID  = pAppInfo[i].hTaskID; break;
			case SYS_PORTID_LCD:	g_hLCDTaskID = pAppInfo[i].hTaskID; break;
			case SYS_PORTID_RELAY:	g_hRelayTaskID = pAppInfo[i].hTaskID; break;
			case SYS_PORTID_DBASE:	g_hDbaseTaskID = pAppInfo[i].hTaskID; break;
			case SYS_PORTID_WATCHDOG:	g_hWdogTaskID = pAppInfo[i].hTaskID; break;
			default:break;
		}
	}		
		
	// 5。启动任务

	return DB_ERR_OK;
}

