/********************************************************************************************************/
/*																										*/
/*        						��򿭷������ɷ����޹�˾												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      Dbase.h                                     1.0												*/
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
extern  TStructCheck 		StructCheck[];					//�ṹ��У������

void StructACCCheck(TStructCheck *structcheck,WORD StructNum);
	
#ifdef _TEST_DISTURB_
BYTE			*pDistTempBlock;
#endif

#define INCALID_ALARM_INF 0xFFFF

TSCAlarmItem	g_aSelfCheckAlarmList[]=
{
	{SS_PROGRAM_ROM_EERR,		R_ROMERR,		R_ROMERR_R},		//����ROM����
	{SS_EEPROM_DATA_ERR, 		R_EEPROMERR,	R_EEPROMERR_R},		//EEPROM��ֵ����
	{SS_AD_ERR,					R_ADERR,		R_ADERR_R},			//AD����
	{SS_EEPROM_SW_ERR,			R_SLERR,		R_SLERR_R},			//��ѹ�����
	{SS_RAM_DATA_ERR,			R_RAMERR,		R_RAMERR_R},		//RAM��ֵ����
//	{SS_NVRAM_LINK_ERR,			R_NVRAMERR,		R_NVRAMERR_R},		//NVRAM���ݳ���
	{SS_12V_ERR,				R_POWERERR,		R_POWERERR_R},		//5V��Դ����
	{SS_EEPROM_WRITE_ERR,		R_EEPROM_W_ERR, INCALID_ALARM_INF}, //EEPROMд�����	
	{SS_FILE_DIFFRELAYDATA_ERR,	R_RELDATAERR,	R_RELDATAERR_R},	//���������쳣��
};

void DB_BackUpEvent()
{	
	TAbsTime				tAbsTime;
	TFileMap 				*pFileMap;
	TDBaseConfigTable		*pDbase;

	pDbase = &G_pCfgTable->DBaseConfig;
	
	// 1�����ݵ��
	// 2����������
	//		2��1 ����SOE
	//		2��2 ���ݲ�����¼
	// 		2��3 ���ݱ�����������
	pFileMap = pDbase->pBasicEventFileMap;
	if(pFileMap != NULL)
	{
		//�����ļ���Ҫ����
		if(pFileMap->wFlag & FILEMAP_FLAGS_MODIFY)
		{
			ReadAbsTime(&tAbsTime);
			AbsTimeSub(&tAbsTime, &pFileMap->absTime);
			// 5�����Ժ����		
			if((tAbsTime.Hi > 0)||(tAbsTime.Lo > 5000))
			{
				pFileMap->wFlag &= ~FILEMAP_FLAGS_MODIFY;
				DB_SaveBasicEventFile();
				DAE_SetRunningFlags(SS_FILE_BASICEVENT_CHG);
			}
		}
	}
	
	//		2��4 ���ݱ�������
	pFileMap = pDbase->pRelayEventFileMap;
	if(pFileMap != NULL)
	{
		//�����ļ���Ҫ����
		if(pFileMap->wFlag & FILEMAP_FLAGS_MODIFY)
		{
			ReadAbsTime(&tAbsTime);
			AbsTimeSub(&tAbsTime, &pFileMap->absTime);
			// 3�����Ժ����
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
				
				//���ͷ�У��Ϣ
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
	//�ж������Ƿ�Ϊ����,����Ϊ��������
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
					//����״̬���ͷŽڵ�
					pActionCache->bStatus = RELAY_CACHE_READY;
					//�ͷŽڵ�
					pTempNode = pTempNode->Next;
					
					pActionLink->RemoveNode(pTempNode);
					G_RelayRepFreeLink.AddTail(pTempNode);

					continue;
				}
			}
		}
		//�����¸��ڵ�
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != pActionLink->GetHead()));
}

void DB_RecoverOperateState()
{
	TRunTimeOPT				*pRuntimeOpt;		
	TDBaseConfigTable		*pDBase;				//���ݿ����ñ�
	WORD					i=0;

	pDBase = &G_pCfgTable->DBaseConfig;
	
	for(i=0;i<5;i++)
	{
		switch(i)
		{
		// 4��1 �����޸ĳ�ʱ
		case 0:
			pRuntimeOpt = pDBase->pParaOpt;
			break;	
		// 4��2 ��ѹ���޸ĳ�ʱ
		case 1:
			pRuntimeOpt = pDBase->pSWOpt;
			break;	
		
		// 4��3 ��ֵ�޸ĳ�ʱ
		case 2:
			pRuntimeOpt = pDBase->pSETOpt;
			break;	

		// 4��4 ��ֵ�����޸ĳ�ʱ
		case 3:
			pRuntimeOpt = pDBase->pSETAreaOpt;
			break;	

		// 4��5 ϵ���޸ĳ�ʱ
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

//��ʱ�Լ�
void DB_MakeSelfCheckAlarm()
{
	TRelayEventItem 	tAlarm;
	WORD				nIndex,i,num;
	DWORD               newStatus,oldStatus;
	
	num = ArraySize(g_aSelfCheckAlarmList);

	// 1.��λ�Ƚϴ�����״̬λ
	for(i=0;i<num;i++)
	{
		nIndex = g_aSelfCheckAlarmList[i].nIndex;
		newStatus = g_RunningFlag[nIndex/32]&(0x01<<(nIndex%32));
		oldStatus = G_pCfgTable->SystemStatus.Status.Flag[nIndex/32]&(0x01<<(nIndex%32));
		
		if(newStatus != oldStatus)
		{
			// 2. ���ɸ澯����
			ReadAbsTime(&tAlarm.stTime);
			memset(tAlarm.FD_Value,0,MAX_FD_NUM*sizeof(long));
			
			if(DAE_GetRunningFlags(nIndex))
			// 2.1 ����
			{
				if(g_aSelfCheckAlarmList[i].nAlarmINF == INCALID_ALARM_INF)
					continue;
                else
				    tAlarm.nINF = ReportInf[g_aSelfCheckAlarmList[i].nAlarmINF].wInf;
				
				tAlarm.bStatus = 1;
				
				LEDOnOff(ALARM_LED,TRUE);
			}
			else
			// 2.2 �ָ�
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

	// 3. �������ݿ�״̬
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
	
	//Ϊ�ļ�����������ʱ����
	SetTimer(1,1000);
	m_dwTimerCnt = 0;
	
	return TRUE;
}

//��Ϣ�ӿ�
//OS_PostMessage(g_hDbaseTaskID, (WORD)SM_STARTDIST, dwStartPtr,dwTimeHi,dwTimeLo,0);
//����¼����Ϣ��Ӧ����
void CDBaseTask::OnStartDist(DWORD dwStart,DWORD dwPrevSampleNum,DWORD dwTimeLo,WORD wTimeHi)
{
	TAbsTime stTime;

	stTime.Hi = wTimeHi;
	stTime.Lo = dwTimeLo;

	//����¼��
	DB_StartDistRecord(dwStart,&stTime,dwPrevSampleNum);
}

void CDBaseTask::OnSaveReport(DWORD MsgCode, DWORD wParam)
{
	// �洢������������������
	DB_SaveRelayActionOnTime();
}

void CDBaseTask::OnTimeOut(DWORD id)
{      
	TSysTime systime;
	
	m_dwTimerCnt++;

	//׷��¼������
	DB_TraceDistRecord();

	// ÿ10����һ��
	if((m_dwTimerCnt%10) == 0)
		CheckSystemState();
	
	// ÿ60���Ᵽ���Լ�
	if((m_dwTimerCnt%60) == 0)
		StructACCCheck(StructCheck,CHECK_NUM);
	
	//��B��ʽ�ź���Ч��ǰ����,ÿ5���Ӷ�ʱд��ʱ��Ӳ��ʱ��,���򲻸���
	if((m_dwTimerCnt%300) == 0)
	{
		if(DAE_GetRunningFlags(SS_TIME_B) == 0)
		{
			ReadSystemTime(&systime);
			WriteHardTime(&systime);
		}
	}

	/*���ڲ�����������*/

	// 1������ң�س�ʱ
	DB_ProcessBOException();
		
	/* 4��������ʱ����*/
	DB_RecoverOperateState();

	/* 5�������Լ�澯����*/
	DB_MakeSelfCheckAlarm();

	/* 6���洢������������������*/
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

	//������ݿ������Ƿ���ѩ�����͵����ݿ�д���¼�����ʱ֪ͨ��Ӧ����
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

// ר�������ļ����ݺ�EEPROM����
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

	// 1 ÿ10�����һ��¼������
//	if((m_dwTimerCnt%10) == 0)
		DB_SaveDistRecord();
		
	// 2����ʱ�ļ�����
	DB_BackUpEvent();

	// 3�������������泬ʱ	
	//DB_RecoverRelayAction();
}


