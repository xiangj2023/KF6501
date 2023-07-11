/********************************************************************************************************/
/*																										*/
/*        						天津凯发电气股份有限公司												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Dbase.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义数据库中相关的接口函数																*/
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
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*		Dbfunc.h						数据库函数声明													*/
/*		Db_err.h						数据库返回值													*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
/********************************************************************************************************/

#include "Parafunc.h"	
#include "dbfunc.h"	
#include "daefunc.h"	
#include "dbase.h"
#include "relaydef.h"
#include "relay_defs.h"
#include "relay.h"
#include "led.h"
//#define _TEST_DISTURB_

extern 	TSysConfigTable		*G_pCfgTable;
extern	CLink				G_RelayRepSaveLink;
extern	CLink				G_RelayRepFreeLink;
extern  TStructCheck 		StructCheck[];					//结构体校验数组

void StructACCCheck(TStructCheck *structcheck,WORD StructNum);
	
#ifdef _TEST_DISTURB_
BYTE			*pDistTempBlock;
#endif

#define INCALID_ALARM_INF 0xFFFF

TSCAlarmItem	g_aSelfCheckAlarmList[]=
{
	{SS_PROGRAM_ROM_EERR,		R_ROMERR,		R_ROMERR_R},		//程序ROM故障
	{SS_EEPROM_DATA_ERR, 		R_EEPROMERR,	R_EEPROMERR_R},		//EEPROM定值故障
	{SS_AD_ERR,					R_ADERR,		R_ADERR_R},			//AD故障
	{SS_EEPROM_SW_ERR,			R_SLERR,		R_SLERR_R},			//软压板故障
	{SS_RAM_DATA_ERR,			R_RAMERR,		R_RAMERR_R},		//RAM定值故障
//	{SS_NVRAM_LINK_ERR,			R_NVRAMERR,		R_NVRAMERR_R},		//NVRAM内容出错
	{SS_12V_ERR,				R_POWERERR,		R_POWERERR_R},		//5V电源故障
	{SS_EEPROM_WRITE_ERR,		R_EEPROM_W_ERR, INCALID_ALARM_INF}, //EEPROM写入故障	
	{SS_FILE_DIFFRELAYDATA_ERR,	R_RELDATAERR,	R_RELDATAERR_R},	//保护数据异常；
};

void DB_BackUpEvent()
{	
	TAbsTime				tAbsTime;
	TFileMap 				*pFileMap;
	TDBaseConfigTable		*pDbase;

	pDbase = &G_pCfgTable->DBaseConfig;
	
	// 1。备份电度
	// 2。备份事项
	//		2。1 备份SOE
	//		2。2 备份操作记录
	// 		2。3 备份保护启动报告
	pFileMap = pDbase->pBasicEventFileMap;
	if(pFileMap != NULL)
	{
		//镜像文件需要更新
		if(pFileMap->wFlag & FILEMAP_FLAGS_MODIFY)
		{
			ReadAbsTime(&tAbsTime);
			AbsTimeSub(&tAbsTime, &pFileMap->absTime);
			// 5秒钟以后存盘		
			if((tAbsTime.Hi > 0)||(tAbsTime.Lo > 5000))
			{
				pFileMap->wFlag &= ~FILEMAP_FLAGS_MODIFY;
				DB_SaveBasicEventFile();
				DAE_SetRunningFlags(SS_FILE_BASICEVENT_CHG);
			}
		}
	}
	
	//		2。4 备份保护动作
	pFileMap = pDbase->pRelayEventFileMap;
	if(pFileMap != NULL)
	{
		//镜像文件需要更新
		if(pFileMap->wFlag & FILEMAP_FLAGS_MODIFY)
		{
			ReadAbsTime(&tAbsTime);
			AbsTimeSub(&tAbsTime, &pFileMap->absTime);
			// 3秒钟以后存盘
			if((tAbsTime.Hi > 0)||(tAbsTime.Lo > 3000))
			{
//				pFileMap->wFlag &= ~FILEMAP_FLAGS_MODIFY;
				DB_SaveRelayEventFile();
				DAE_SetRunningFlags(SS_FILE_RELAYEVENT_CHG);
			}
		}
	}
}

void DB_ProcessBOException()
{
	WORD				i=0;
	TBOPara				tBOAttr;
	TRunTimeBO			*pBOBase;
	
	pBOBase = G_pCfgTable->DBaseConfig.pBO;
	ASSERT(pBOBase);
	
	for(i=0;i<DB_GetBONum();i++)
	{
		if(pBOBase[i].wTimeOutCnt != 0)
		{
			pBOBase[i].wTimeOutCnt--;
			if(pBOBase[i].wTimeOutCnt == 0)
			{
				DB_GetBOPara(i, &tBOAttr);
				
				//发送返校信息
				switch(pBOBase[i].wStatus)
				{
				case BO_STATUS_SELECT:
					OS_PostMessage(pBOBase[i].dwAppID, SM_BOSELECTECHO,0,\
						MAKELONG(tBOAttr.wSwitchNo, BO_GET_OPT(tBOAttr.dwControl)),BO_RET_TIMEOUT,0);
					break;
					
				case BO_STATUS_EXE:
					OS_PostMessage(pBOBase[i].dwAppID, SM_BOEXECECHO, 0, \
						MAKELONG(tBOAttr.wSwitchNo, BO_GET_OPT(tBOAttr.dwControl)), BO_RET_TIMEOUT, 0 );
					break;
					
				case BO_STATUS_DIREXE:
					OS_PostMessage(pBOBase[i].dwAppID, SM_BODIRECTECHO, 0, \
						MAKELONG(tBOAttr.wSwitchNo, BO_GET_OPT(tBOAttr.dwControl)), BO_RET_TIMEOUT, 0 );
					break;
					
				default:
					break;
				}
				pBOBase[i].wStatus = BO_STATUS_READY;
			}
		}
	}
}

void DB_RecoverRelayAction()
{
	CLink 				*pActionLink;
    CNode 				*pTempNode;
	TRelayEventCache	*pActionCache;

	pActionLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	if(pActionLink == NULL)
		return;
	
	pTempNode = pActionLink->GetHead();
	//判断链表是否为空链,链表为空链返回
	do
    {
		if(pTempNode == NULL)
			break;
		
		pActionCache = (TRelayEventCache	*)pTempNode->pData;
		ASSERT(pActionCache);
		
		if(pActionCache->bStatus != RELAY_CACHE_READY)
		{
			if(pActionCache->dwTimeOut != 0)
			{
				pActionCache->dwTimeOut--;
				if(pActionCache->dwTimeOut == 0)
				{
					//更新状态，释放节点
					pActionCache->bStatus = RELAY_CACHE_READY;
					//释放节点
					pTempNode = pTempNode->Next;
					
					pActionLink->RemoveNode(pTempNode);
					G_RelayRepFreeLink.AddTail(pTempNode);

					continue;
				}
			}
		}
		//遍历下个节点
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != pActionLink->GetHead()));
}

void DB_RecoverOperateState()
{
	TRunTimeOPT				*pRuntimeOpt;		
	TDBaseConfigTable		*pDBase;				//数据库配置表
	WORD					i=0;

	pDBase = &G_pCfgTable->DBaseConfig;
	
	for(i=0;i<5;i++)
	{
		switch(i)
		{
		// 4。1 参数修改超时
		case 0:
			pRuntimeOpt = pDBase->pParaOpt;
			break;	
		// 4。2 软压板修改超时
		case 1:
			pRuntimeOpt = pDBase->pSWOpt;
			break;	
		
		// 4。3 定值修改超时
		case 2:
			pRuntimeOpt = pDBase->pSETOpt;
			break;	

		// 4。4 定值区号修改超时
		case 3:
			pRuntimeOpt = pDBase->pSETAreaOpt;
			break;	

		// 4。5 系数修改超时
		case 4:
			pRuntimeOpt = pDBase->pCFOpt;
			break;	
		default:
			break;;
		}
		
		if(pRuntimeOpt->wTimeCnt != 0)
		{
			pRuntimeOpt->wTimeCnt--;
			if(pRuntimeOpt->wTimeCnt == 0)
			{
				pRuntimeOpt->dwAppID	= APP_TASKID_NULL;
				pRuntimeOpt->wTimeCnt	= 0;
				pRuntimeOpt->wStatus	= OPT_STATUS_READY; 
			}
		}
	}
}

//extern const struct TReportInf ReportInf;

//定时自检
void DB_MakeSelfCheckAlarm()
{
	TRelayEventItem 	tAlarm;
	WORD				nIndex,i,num;
	DWORD               newStatus,oldStatus;
	
	num = ArraySize(g_aSelfCheckAlarmList);

	// 1.逐位比较待检测的状态位
	for(i=0;i<num;i++)
	{
		nIndex = g_aSelfCheckAlarmList[i].nIndex;
		newStatus = g_RunningFlag[nIndex/32]&(0x01<<(nIndex%32));
		oldStatus = G_pCfgTable->SystemStatus.Status.Flag[nIndex/32]&(0x01<<(nIndex%32));
		
		if(newStatus != oldStatus)
		{
			// 2. 生成告警报告
			ReadAbsTime(&tAlarm.stTime);
			memset(tAlarm.FD_Value,0,MAX_FD_NUM*sizeof(long));
			
			if(DAE_GetRunningFlags(nIndex))
			// 2.1 产生
			{
				if(g_aSelfCheckAlarmList[i].nAlarmINF == INCALID_ALARM_INF)
					continue;
                else
				    tAlarm.nINF = ReportInf[g_aSelfCheckAlarmList[i].nAlarmINF].wInf;
				
				tAlarm.bStatus = 1;
				
				LEDOnOff(ALARM_LED,TRUE);
			}
			else
			// 2.2 恢复
			{	
				if(g_aSelfCheckAlarmList[i].nRecoverINF == INCALID_ALARM_INF)
                    continue;
				else
				{   
                    if(g_aSelfCheckAlarmList[i].nRecoverINF == g_aSelfCheckAlarmList[i].nAlarmINF)
						tAlarm.bStatus = 0;
					else
						tAlarm.bStatus = 1;
					
					tAlarm.nINF = ReportInf[g_aSelfCheckAlarmList[i].nRecoverINF].wInf;
				}
				LEDOnOff(ALARM_LED,FALSE);
			}
			
			DB_RelayAlarm(&tAlarm);
		}
	}

	// 3. 更新数据库状态
	memcpy(G_pCfgTable->SystemStatus.Status.Flag,g_RunningFlag,sizeof(DAEFLAGS));
}


BEGIN_MESSAGE_MAP(CDBaseTask,CApp)
ON_SM_TIMEOUT(CDBaseTask)
ON_SM_STARTDIST(CDBaseTask)
ON_SM_SAVREPORT(CDBaseTask)
ON_SM_WRITESINGNALRW(CDBaseTask)

END_MESSAGE_MAP()

void DBaseTask(DWORD *pdwApp)
{
	CDBaseTask dbase(pdwApp);
	
	if( dbase.Init()==FALSE )
	{
		OS_Exit(0); 
	}
	
	dbase.Run();		
}

BOOL CDBaseTask::Init()
{
	DWORD			dwLen;
	
	//为文件操作分配临时缓冲
	SetTimer(1,1000);
	m_dwTimerCnt = 0;
	
	return TRUE;
}

//消息接口
//OS_PostMessage(g_hDbaseTaskID, (WORD)SM_STARTDIST, dwStartPtr,dwTimeHi,dwTimeLo,0);
//启动录波消息响应函数
void CDBaseTask::OnStartDist(DWORD dwStart,DWORD dwPrevSampleNum,DWORD dwTimeLo,WORD wTimeHi)
{
	TAbsTime stTime;

	stTime.Hi = wTimeHi;
	stTime.Lo = dwTimeLo;

	//启动录波
	DB_StartDistRecord(dwStart,&stTime,dwPrevSampleNum);
}

void CDBaseTask::OnSaveReport(DWORD MsgCode, DWORD wParam)
{
	// 存储保护动作和启动报告
	DB_SaveRelayActionOnTime();
}

void CDBaseTask::OnTimeOut(DWORD id)
{      
	TSysTime systime;
	
	m_dwTimerCnt++;

	//追踪录波任务
	DB_TraceDistRecord();

	// 每10秒检测一次
	if((m_dwTimerCnt%10) == 0)
		CheckSystemState();
	
	// 每60秒检测保护自检
	if((m_dwTimerCnt%60) == 0)
		StructACCCheck(StructCheck,CHECK_NUM);
	
	//在B格式信号有效的前提下,每5分钟定时写入时钟硬件时间,否则不更新
	if((m_dwTimerCnt%300) == 0)
	{
		if(DAE_GetRunningFlags(SS_TIME_B) == 0)
		{
			ReadSystemTime(&systime);
			WriteHardTime(&systime);
		}
	}

	/*周期测试如下内容*/

	// 1。处理遥控超时
	DB_ProcessBOException();
		
	/* 4。操作超时处理*/
	DB_RecoverOperateState();

	/* 5。生成自检告警报告*/
	DB_MakeSelfCheckAlarm();

	/* 6。存储保护动作和启动报告*/
	DB_SaveRelayActionOnTime();
}
void CDBaseTask::OnWriteSingnalRW(DWORD dwEEPRomAddr,DWORD val)
{
	BYTE bBuf[2];
	if(val==0x10)
	{
		bBuf[0]=1;
		bBuf[1]=0;
	}
	if(val==0x01)
	{
		bBuf[0]=0;
		bBuf[1]=1;
	}	
	EEPROM_Write(dwEEPRomAddr,2, bBuf);
//	ReportMsg("Write signal. Addr=%d val=%d",dwEEPRomAddr,val);
}

/*
void DB_CheckWriteDBVent()
{
    TAppInfo 		*pAppInfo;
    WORD 			i,num;

    pAppInfo = G_pCfgTable->TaskApp;
    if( pAppInfo == NULL )
	   return ;

	//检查数据库任务，是否有雪崩类型的数据库写入事件，及时通知对应任务
	num = G_pCfgTable->DBaseConfig.nTaskAPP; 
	for(i=0;i<num;i++, pAppInfo++)
	{
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_SOE))
		{
			OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_BIEVENT, 0,0,0,0);
		}
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_BIE))
		{
			OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_BIBURST, 0,0,0,0);
		}
	}
	
}
*/

BEGIN_MESSAGE_MAP(CBackUpTask,CApp)
ON_SM_TIMEOUT(CBackUpTask)
END_MESSAGE_MAP()

// 专门用于文件备份和EEPROM备份
void BackUpTask(DWORD *pdwApp)
{
	CBackUpTask backup(pdwApp);
	
	if( backup.Init()==FALSE )
	{
		OS_Exit(0); 
	}
	
	backup.Run();		
}

BOOL CBackUpTask::Init()
{
	DWORD			dwLen;
	
	SetTimer(1,1000);
	m_dwTimerCnt = 0;
	
	return TRUE;
}

void CBackUpTask::OnTimeOut(DWORD id)
{       
	m_dwTimerCnt++;

	// 1 每10秒存盘一次录波数据
//	if((m_dwTimerCnt%10) == 0)
		DB_SaveDistRecord();
		
	// 2。定时文件存盘
	DB_BackUpEvent();

	// 3。保护动作报告超时	
	//DB_RecoverRelayAction();
}


