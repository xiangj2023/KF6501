/*******************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
*******************************************************************************************/ 
                          
/*******************************************************************************************
*                                                                                  
* �ļ�����          
*			relay.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           		KF6560�����������㷨����                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*                                
*                                                                                  
* ����                                                                             
*                                                                                  
*      	
*		
*		
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      yanzh             08/12/19    ��ʼ����                         
*                                                                                  
*******************************************************************************************/
#include "relay.h"
#include "m5234timer.h"
#include "max11046.h"
#include "led.h"
#include "RelayMemInit.h"
#include "RelayDef.h"
#include "dbasedef.h"
#include "dbfunc.h"
#include "logic.h"
#include "logicdef.h"
#include "daedef.h"
#include "relayfun.h"
#include "daefunc.h"

extern DWORD dwADResumeCnt;
extern DWORD dwRelayMSCnt;		//�����ú��������
extern g_hDbaseTaskID;
extern dwRelayDataPtr;	//AD��ǰдָ��
extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern BYTE 	g_RptSerialNum;			//�������к� 
extern BOOL		RelayCheckErr;			//����Ԫ���Լ�����־
extern BOOL 	RelayCheckInitOK;		//����Ԫ���Լ��ʼ����־
extern BYTE 	*g_MaintSW;

BOOL	g_FaultOnFlag;				//�������ѭ�������־ 
BOOL 	RelayTaskInitOK;		//������ʼ������
DWORD 	dwRelayTaskID;
DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
TStructCheck StructCheck[CHECK_NUM];
TRunTimeRM	*pSysRM;		//��������ֵ����ָ��
TRunTimeBI  *pSysBI;		//ң��ʵʱ����ָ��

BEGIN_MESSAGE_MAP(CRelayTask,CApp)
	ON_SM_SAMPLEEND(CRelayTask)
	ON_SM_RELAYDATA(CRelayTask)
	ON_SM_TIMEOUT(CRelayTask)
END_MESSAGE_MAP()

void RelayTask(DWORD *pdwApp)
{
	TMessage msg;
	DWORD status;
	OS_TaskDesp *pTask;

	OS_Sleep(1500);
	CRelayTask *pRelayTask = new CRelayTask(pdwApp);
	pRelayTask->Init();

	pTask = OS_GetTaskTab(dwRelayTaskID);

    //ѭ�����ý�����Ϣ����,ֱ���յ������˳���Ϣ
    while(status != ERR_MESSAGE_QUIT)
    {
		status = OS_GetMessage(dwRelayTaskID, &msg);
		if(status == ERR_MESSAGE_OK)
		{
			//����������м���
			if(pTask != NULL)
				pTask->Counter = 0;

			switch(LOWORD(msg.lParam))
			{
				case SM_TIMEOUT:
					pRelayTask->OnTimeOut(0);					
					break;
				case SM_RELAYDATA:	//���������޸�
					pRelayTask->OnRelayChange(0, 0);					
					break;
				case SM_RELAY_INT:					
					pRelayTask->OnSampleEnd(0);					
					break;
				case SM_LOGICCHANGED:					
					pRelayTask->OnLogicChanged();					
					break;
				default:
					break;
			}
		}
	}
}	

CRelayTask::CRelayTask(DWORD *pdwApp):CApp(pdwApp)
{
}	
void CRelayTask::Init(void) 
{
	dwRelayTaskID = GetTaskId();
	
	ADCInit();			//��ʼ��ADC�����ز����ж�
	RelayTimeInit();	//��ʼ�������ú��붨ʱ��
	#if(HARD_WARE_VER<=HW_VERSION_11)
		ADTimeInit();		//��ʼ������
	#endif
	
/*	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	
	//��ȡ���ݿⱣ������ֵ����ָ�룬�Լӿ�����ٶ�
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	
	InitAllRelayMem();
	InitRealy();

//	RelayCheck();

	g_RptSerialNum = 0;					//�������кţ����ݿ���֯��
	RptSerialNumOfAct =0;				//�����������кţ�������������
	RptSerialNumOfStart=0;				//�����������кţ��������ϵ�������������
	FaultSINum = 0;					//���������������
	PickCheck =0;
	PickCheckNext =0;	
	lSampData1 =0;
	lSampData2 =0;
	lSampDate3 =0;
	lPickValue =0;
	m_lPickUpValue =0;
	PickUpRW = RW_PICKUP;			//���������̵����֣�������ģֵ����ͻ��������
	FaultOnFlag = FALSE;				//�������ѭ�������־
	g_FaultOnFlag =FALSE;
	HaveMakeStartRptFlag = FALSE;		//�����������־
	m_bFaultLKFlag = FALSE;				//ͻ��������������������ʼʱ�̱�־
	FaultCheckOutFlag = FALSE;			//������ϱ�־
	m_bHaveMakeTripFailRpt = FALSE;
	RelCFGCLR = CFG_STATUS_CLR;		//���������˳�
	RelCFGSET = CFG_STATUS_SET;		//���������˳�
//	m_bNextRelayStart = FALSE;		//���ι��ϴ����п��ܳ��ֵڶ��ζ�����־


	
	dwChanDataMask = ((0x01<<CHANNEL_6560_U1)|(0x01<<CHANNEL_6560_U2)|(0x01<<CHANNEL_6560_Ud1)|(0x01<<CHANNEL_6560_Ud2)|\
						(0x01<<CHANNEL_6560_Ud3)|(0x01<<CHANNEL_6560_IP)|(0x01<<CHANNEL_6560_I1)|\
						(0x01<<CHANNEL_6560_I2)|(0x01<<CHANNEL_6560_I3));
	
	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	dwComposeMask = 0x01<<CHANNEL_6560_Up;
	
	
	//��ʼ����ȡ����������ʱ������
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*9);	
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	dwRECBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3));
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM));
	
	//�����ǳ�ʼ���߼����̲���
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();

	
	//��ʼ�������Լ�Ϊ����״̬
	int i;
	for(i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}

	//���Ƚ���һ�鿪��ʧЧ�Լ���
	for(i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();

	InitDCRecInfo();

	g_MaintSW = SW_pbyMAINT;
	m_bLogicChanged = FALSE;
	RelayTaskInitOK = TRUE;
	SetTimer(1,5);

}
//------------------------------------------------------------------------------------------ 
// ����: OnTimeOut	 
// ����: ��ʱ����Ϣ��Ӧ����
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::OnTimeOut(DWORD id)
{	
	if(dwADResumeCnt ==0)
		return;
	else
	{
		LogicProcess();
	}

}

void CRelayTask::OnSampleEnd(DWORD arg)
{	
	g_FaultOnFlag = FaultOnFlag;
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}
	
//	if(RelayCheckErr)
//	{
//		//ReportMsg("Relay Check Serious Error,and Relay Task Locked!");
//		LedAllOn();
//		return ;
//	}
	//��ʼ������ͨ����������
	static WORD wInvalidCheckCnt;	
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
	
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	WriteCompseData();
	CaluUI();          //���ݲ������ݣ��������It,If,U1,U2
	
	//���Ƚ��п����Լ�
	if(!m_bLogicChanged)
		BoBreakCheck();
	
	if(FaultOnFlag == FALSE)
	{
		CheckPTBrok_1U(&RelPT);					//PT���ߴ���
	}
	
	FalutProcess();								//����ѭ������
	GeneralUnderValRelay(&Rel27U);				//�͵�ѹ�����ӳ���
	Process52BF();
	LogicProcess();								//�߼����̴���
	
	if(m_bLastRecStatusFlag == FALSE)
	{
		if(ReadRelayWord(RW_REC))
		{
			TAbsTime abstime;
			ReadAbsTime(&abstime);
			OS_PostMessage(g_hDbaseTaskID, SM_STARTDIST, dwDataPtr,*AS_pnRECD*16/10,abstime.Lo,abstime.Hi);
		}
	}	
	m_bLastRecStatusFlag = ReadRelayWord(RW_REC);

	
	//ʧЧ�Լ�,200�α����������һ·�������
	if(!m_bLogicChanged)
	{
		if(wInvalidCheckCnt == 200)
		{
			BoInvalidCheck();
			wInvalidCheckCnt = 0;
		}
		else
			wInvalidCheckCnt++;
	}
	else
	{
		if(dwRelayMSCnt - m_dwWaitLogicCounter>1000)
		{
			m_bLogicChanged = FALSE;
		}
	}
	
	WriteDCChannel();							//д����¼��
//	MCF_GPIO_PODR_TIMER &= ~0x02;		//�͵�ƽ
}
//------------------------------------------------------------------------------------------ 
// ����: CaluUI	 
// ����: ���㱣������ֵ
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::CaluUI(void)
{	
	//����UPģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwComposeBuf,&UP);
	CaluModValue(&UP);
	*RM_pnUp = UP.Mod;
	DB_LogWriteRM(RM_UP, *RM_pnUp);
	if(UP.Mod > *AS_pn59UP)
	{
		WriteRelayWord(RW_59UP, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UP, RELAY_OFF);
	}
	if(UP.Mod <*AS_pn27UP)
	{
		WriteRelayWord(RW_27UP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UP, RELAY_OFF);
	}	

	//����U1ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf,&U1);
	CaluModValue(&U1);	
	*RM_pnU1 = U1.Mod;
	DB_LogWriteRM(RM_U1, *RM_pnU1);	
	if(U1.Mod > *AS_pn59U1)
	{
		WriteRelayWord(RW_59U1, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(U1.Mod <*AS_pn27U1)
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}	

	//����U2ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	*RM_pnU2 = U2.Mod;
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
	if(U2.Mod > *AS_pn59U2)
	{
		WriteRelayWord(RW_59U2, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(U2.Mod <*AS_pn27U2)
	{
		WriteRelayWord(RW_27U2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}	
	//����Ud1ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&Ud1);
	CaluModValue(&Ud1);
	*RM_pnUd1 = Ud1.Mod;
	DB_LogWriteRM(RM_UD1, *RM_pnUd1);	
	if(Ud1.Mod > *AS_pn59Ud1)
	{
		WriteRelayWord(RW_59UD1, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UD1, RELAY_OFF);
	}
	if(Ud1.Mod <*AS_pn27Ud1)
	{
		WriteRelayWord(RW_27UD1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UD1, RELAY_OFF);
	}	
	//����Ud2ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&Ud2);
	CaluModValue(&Ud2);
	*RM_pnUd2 = Ud2.Mod;
	DB_LogWriteRM(RM_UD2, *RM_pnUd2);	
	if(Ud2.Mod > *AS_pn59Ud2)
	{
		WriteRelayWord(RW_59UD2, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UD2, RELAY_OFF);
	}
	if(Ud2.Mod <*AS_pn27Ud2)
	{
		WriteRelayWord(RW_27UD2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UD2, RELAY_OFF);
	}	
	//����Ud2ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&Ud3);
	CaluModValue(&Ud3);
	*RM_pnUd3 = Ud3.Mod;
	DB_LogWriteRM(RM_UD3, *RM_pnUd3);	
	if(Ud3.Mod > *AS_pn59Ud3)
	{
		WriteRelayWord(RW_59UD3, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UD3, RELAY_OFF);
	}
	if(Ud3.Mod <*AS_pn27Ud3)
	{
		WriteRelayWord(RW_27UD3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UD3, RELAY_OFF);
	}	

	//����IPģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&IP);
	CaluModValue(&IP);
	*RM_pnIp = IP.Mod;
	DB_LogWriteRM(RM_IP, *RM_pnIp);
	if(IP.Mod>*AS_pn50IP)
	{
		WriteRelayWord(RW_50IP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IP, RELAY_OFF);
	}
	if(IP.Mod>*AS_pn37IP)
	{
		WriteRelayWord(RW_37IP, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IP, RELAY_ON);
	}
	
	//����I1ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&I1);
	CaluModValue(&I1);
	*RM_pnI1 = I1.Mod;
	DB_LogWriteRM(RM_I1, *RM_pnI1);	
	if(I1.Mod>*AS_pn50I1)
	{
		WriteRelayWord(RW_50I1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1, RELAY_OFF);
	}
	if(I1.Mod>*AS_pn37I1)
	{
		WriteRelayWord(RW_37I1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1, RELAY_ON);
	}
	//����I2ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&I2);
	CaluModValue(&I2);
	*RM_pnI2 = I2.Mod;
	DB_LogWriteRM(RM_I2, *RM_pnI2);	
	if(I2.Mod>*AS_pn50I2)
	{
		WriteRelayWord(RW_50I2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I2, RELAY_OFF);
	}
	if(I2.Mod>*AS_pn37I2)
	{
		WriteRelayWord(RW_37I2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I2, RELAY_ON);
	}
	//����I3ģֵ��������ѹ��ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&I3);
	CaluModValue(&I3);
	*RM_pnI3 = I3.Mod;
	DB_LogWriteRM(RM_I3, *RM_pnI3);
	if(I3.Mod>*AS_pn50I3)
	{
		WriteRelayWord(RW_50I3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I3, RELAY_OFF);
	}
	if(I3.Mod>*AS_pn37I3)
	{
		WriteRelayWord(RW_37I3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I3, RELAY_ON);
	}
	
	CaluSecondFourier(dwSampBuf+READ_POINT_NUM*5,&IP2);
	CaluModValue(&IP2);
	IP2.Mod = IP2.Mod*CurHarmCal[0]/1000;	

	CaluThirdFourier(dwSampBuf+READ_POINT_NUM*5,&IP3);
	CaluModValue(&IP3);
	IP3.Mod = IP3.Mod*CurHarmCal[1]/1000;
	*RM_pnIh3 = IP3.Mod;
	DB_LogWriteRM(RM_IH3, *RM_pnIh3);

	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*5,&IP5);
	CaluModValue(&IP5);
	IP5.Mod = IP5.Mod*CurHarmCal[2]/1000;
	*RM_pnIh5 = IP5.Mod;
	DB_LogWriteRM(RM_IH5, *RM_pnIh5);

	CaluSeventhFourier(dwSampBuf+READ_POINT_NUM*5,&IP7);
	CaluModValue(&IP7);
	IP7.Mod = IP7.Mod*CurHarmCal[3]/1000;
	*RM_pnIh7 = IP7.Mod;
	DB_LogWriteRM(RM_IH7, *RM_pnIh7);
	
	m_IPH = IP3.Mod*IP3.Mod+IP5.Mod*IP5.Mod*25/9+IP7.Mod*IP7.Mod*49/9;
	IPH.Mod= Evolution64(m_IPH);
}


/************************************************************************************************
*����: ��ʼ���뱣��ֱ����صĶ�ֵ
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitSet(void)
{
	if(*AS_pnPM == BTMODE)
	{
		WriteRelayWord(RW_BTMODE,RELAY_ON);
		WriteRelayWord(RW_ATMODE,RELAY_OFF);
	}
	
	else if(*AS_pnPM == ATMODE)
	{
		WriteRelayWord(RW_BTMODE,RELAY_OFF);
		WriteRelayWord(RW_ATMODE,RELAY_ON);
	}
}

void  CRelayTask::InitRealy(void)
{
	InitRel50I0();
	InitRel50I1();
	InitRel59U();
	InitRel50IH();
	InitRel60I1();
	InitRel60I2();
	InitRel50ST();
	InitRel60Ud1();
	InitRel60Ud2();
	InitRel60Ud3();
	InitRel27U();
	InitRelPT();
	InitStructCheck();
	
}
/************************************************************************************************
*����: ������ͨ����д��¼������
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::WriteDCChannel()
{
	RecordRelayWord(DCMacroArray,DCCaluResult);	
	for(LONG i =0;i<RELAY_SAMP_INTERVAL;i++)
	{
		DCChannelVal[i] = DCCaluResult[0];
		DCChannelVal[i+RELAY_SAMP_INTERVAL] = DCCaluResult[1];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*2] = DCCaluResult[2];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*3] = DCCaluResult[3];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*4] = DCCaluResult[4];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*5] = DCCaluResult[5];
	}
	WriteComposeData(DCChannelVal,dwDCDataMask, TRUE);	
}
void CRelayTask::WriteCompseData(void)
{
	if(*AS_pnPM == BTMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i];
		}
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			dwRECBuf[i]= dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		}
	}
	else if(*AS_pnPM == ATMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i]-dwSampBuf[i+READ_POINT_NUM];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			dwRECBuf[i]= dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i]-dwSampBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		}
	}
	WriteComposeData(dwRECBuf, dwComposeMask);
}

/************************************************************************************************
*����: ��ʼ������ͨ��¼�������Ϣ
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitDCRecInfo()
{	
	TDist_Disp			*pDistDisp;	
	TDC_Disp			*pDCDisp;
	WORD				wDCNum;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pDCDisp = G_pCfgTable->DBaseConfig.pDCPara;
	wDCNum = pDistDisp->wDigitalChannelNum;
	DCMacroArray[0] = wDCNum;
	for(WORD i=0;i<wDCNum;i++)
		DCMacroArray[i+1] = pDCDisp[i].wRelayMacro;
	
}

//------------------------------------------------------------------------------------------ 
// ����: InitRel50I0	 
// ����: ��ʼ���ٶϱ���Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I0(void)
{
	Rel50I0.pElecSet			= RS_pn50_0C;          						//��������������ֵ
	Rel50I0.pTimeSet			= RS_pn50_0D;				 			//��������ʱ�䶨ֵ
	Rel50I0.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel50I0.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel50I0.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I0.pH2Coef				= NULL;

	Rel50I0.pRelCFG				= CFG_pby50_0;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I0.pRelSW				= SW_pby50_0;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I0.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel50I0.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I0.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel50I0.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I0.pRelElec 			= &IP;									//��ǰ����ָ��		
	Rel50I0.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel50I0.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel50I0.pRelVolt			= &UP;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I0.pRel47UCFG		= &RelCFGCLR;;
	Rel50I0.R47ULKRelayWord   = FALSE1;

	Rel50I0.StartRelayWord		= RW_PI50_0;						//����������μ̵�����
	Rel50I0.ActRelayWord		= RW_TR50_0;							//�������������̵�����
	Rel50I0.AccRelayWord		= FALSE1;								//���������̵�����
	Rel50I0.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel50I0.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel50I0.PRewardRelayWord    = FALSE1;
	Rel50I0.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel50I0.LockRelayWord		= RW_LKPI50_0;						//��α����̵�����
	Rel50I0.RSTRelayWord		= RW_LKRST50_0;							//���ر����̵�����
	Rel50I0.LoopCtrlRelayWord	= RW_LPC50_0;						//����ѭ�����Ƽ̵�����
	Rel50I0.wReportNum			= R_R50I0;							//���������������
	Rel50I0.wAccReportNum		= 0xFFFF;								//���ٶ����������
	Rel50I0.RptSerialNum		= &RptSerialNumOfAct;			

	Rel50I0.dwK35				= NULL;
	Rel50I0.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel50I0.wPickUpReportNum	= R_R50I0_PI;						//��α������к�
	Rel50I0.wResetReportNum		= R_R50I0_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50I0.dwRelTimer			= 0;			 			//��ʱ��
	Rel50I0.byRelFCounter		= 0;						//���ϼ�����
	Rel50I0.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50I0.wExcepWordNum        =8;
	Rel50I0.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// ����: InitRel50I1	 
// ����: ��ʼ����������Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I1(void)
{		
	Rel50I1.pElecSet			= RS_pn50_1C;          					//��������������ֵ
	Rel50I1.pTimeSet			= RS_pn50_1D;				 			//��������ʱ�䶨ֵ
	Rel50I1.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel50I1.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel50I1.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I1.pH2Coef				= NULL;

	Rel50I1.pRelCFG				= CFG_pby50_1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I1.pRelSW				= SW_pby50_1;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I1.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I1.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I1.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I1.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I1.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel50I1.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I1.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel50I1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I1.pRelElec 			= &IP;									//��ǰ����ָ��		
	Rel50I1.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel50I1.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel50I1.pRelVolt			= &UP;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I1.pRel47UCFG		= &RelCFGCLR;;
	Rel50I1.R47ULKRelayWord   = FALSE1;

	Rel50I1.StartRelayWord		= RW_PI50_1;						//����������μ̵�����
	Rel50I1.ActRelayWord		= RW_TR50_1;							//�������������̵�����
	Rel50I1.AccRelayWord		= FALSE1;								//���������̵�����
	Rel50I1.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel50I1.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel50I1.PRewardRelayWord    = FALSE1;
	Rel50I1.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel50I1.LockRelayWord		= RW_LKPI50_1;						//��α����̵�����
	Rel50I1.RSTRelayWord		= RW_LKRST50_1;							//���ر����̵�����
	Rel50I1.LoopCtrlRelayWord	= RW_LPC50_1;						//����ѭ�����Ƽ̵�����
	Rel50I1.wReportNum			= R_R50I1;							//���������������
	Rel50I1.wAccReportNum		= 0xFFFF;								//���ٶ����������
	Rel50I1.RptSerialNum		= &RptSerialNumOfAct;			
	Rel50I1.dwK35				= NULL;
	
	Rel50I1.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel50I1.wPickUpReportNum	= R_R50I1_PI;						//��α������к�
	Rel50I1.wResetReportNum		= R_R50I1_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50I1.dwRelTimer			= 0;			 			//��ʱ��
	Rel50I1.byRelFCounter		= 0;						//���ϼ�����
	Rel50I1.byRelRCounter		= 0;						//���ؼ�����	
	Rel50I1.wExcepWordNum       = 8;
	Rel50I1.wInfo				= 0;

}
//------------------------------------------------------------------------------------------ 
// ����: InitRel59U	 
// ����: ��ʼ����ѹ����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel59U()
{	
	Rel59U.pElecSet				= RS_pn59V;          					//��������������ֵ
	Rel59U.pTimeSet				= RS_pn59D;				 			//��������ʱ�䶨ֵ
	Rel59U.pHarmKhr1Set			= NULL;									//��������г����Ȩϵ��
	Rel59U.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel59U.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel59U.pH2Coef				= NULL;

	Rel59U.pRelCFG				= CFG_pby59;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel59U.pRelSW				= SW_pby59;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel59U.pRelHLockCFG			= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U.pRelHSuprCFG			= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel59U.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel59U.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel59U.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel59U.pRelElec 			= &UP;									//��ǰ����ָ��		
	Rel59U.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel59U.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel59U.pRelVolt				= NULL;									//��ǰ���������ĵ�ѹ��ָ��


	Rel59U.pRel47UCFG		= &RelCFGCLR;;
	Rel59U.R47ULKRelayWord   = FALSE1;

	Rel59U.StartRelayWord		= RW_PI59;						//����������μ̵�����
	Rel59U.ActRelayWord			= RW_TR59;							//�������������̵�����
	Rel59U.AccRelayWord			= FALSE1;								//���������̵�����
	Rel59U.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel59U.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel59U.PRewardRelayWord     = FALSE1;
	Rel59U.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel59U.LockRelayWord		= RW_LKPI59;						//��α����̵�����
	Rel59U.RSTRelayWord			= RW_LKRST59;							//���ر����̵�����
	Rel59U.LoopCtrlRelayWord	= RW_LPC59;						//����ѭ�����Ƽ̵�����
	Rel59U.wReportNum			= R_R59U;								//���������������
	Rel59U.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel59U.dwK35				= NULL;
	
	Rel59U.bMakeActRpt			= TRUE;
	Rel59U.RptSerialNum			= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel59U.wPickUpReportNum		= R_R59U_PI;						//��α������к�
	Rel59U.wResetReportNum		= R_R59U_RES;						//���ر������к�
	//**********************************************************************	
//	Rel59U.dwRelTimer			= 0;			 			//��ʱ��
	Rel59U.byRelFCounter		= 0;						//���ϼ�����
	Rel59U.byRelRCounter		= 0;						//���ؼ�����		
	Rel59U.wExcepWordNum        = 8;
	Rel59U.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// ����: InitRel50IH	 
// ����: ��ʼ��г����������Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50IH(void)
{
	Rel50IH.pElecSet			= RS_pn50_HC;          					//��������������ֵ
	Rel50IH.pTimeSet			= RS_pn50_HD;				 			//��������ʱ�䶨ֵ
	Rel50IH.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel50IH.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel50IH.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50IH.pH2Coef				= NULL;

	Rel50IH.pRelCFG				= CFG_pby50_H;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IH.pRelSW				= SW_pby50_H;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IH.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IH.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IH.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IH.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IH.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel50IH.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50IH.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel50IH.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50IH.pRelElec 			= &IPH;									//��ǰ����ָ��		
	Rel50IH.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel50IH.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel50IH.pRelVolt			= &UP;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50IH.pRel47UCFG		= &RelCFGCLR;;
	Rel50IH.R47ULKRelayWord   = FALSE1;

	Rel50IH.StartRelayWord		= RW_PI50H;						//����������μ̵�����
	Rel50IH.ActRelayWord		= RW_TR50H;							//�������������̵�����
	Rel50IH.AccRelayWord		= FALSE1;								//���������̵�����
	Rel50IH.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel50IH.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel50IH.PRewardRelayWord    = FALSE1;
	Rel50IH.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel50IH.LockRelayWord		= RW_LKPI50H;						//��α����̵�����
	Rel50IH.RSTRelayWord		= RW_LKRST50H;							//���ر����̵�����
	Rel50IH.LoopCtrlRelayWord	= RW_LPC50H;						//����ѭ�����Ƽ̵�����
	Rel50IH.wReportNum			= R_R50IH;								//���������������
	Rel50IH.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50IH.dwK35				= NULL;
	Rel50IH.bMakeActRpt			= TRUE;
	Rel50IH.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel50IH.wPickUpReportNum	= R_R50IH_PI;						//��α������к�
	Rel50IH.wResetReportNum		= R_R50IH_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50IH.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IH.byRelFCounter		= 0;						//���ϼ�����
	Rel50IH.byRelRCounter		= 0;						//���ؼ�����		
	Rel50IH.wExcepWordNum		= 8;
	Rel50IH.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// ����: InitRel60I1	 
// ����: ��ʼ������1����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60I1(void)
{
	Rel60I1.pElecSet			= RS_pn60I1C;          					//��������������ֵ
	Rel60I1.pTimeSet			= RS_pn60I1D;				 			//��������ʱ�䶨ֵ
	Rel60I1.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel60I1.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel60I1.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel60I1.pH2Coef				= NULL;

	Rel60I1.pRelCFG				= CFG_pby60I1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel60I1.pRelSW				= SW_pby60I;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel60I1.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I1.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I1.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I1.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I1.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel60I1.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel60I1.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel60I1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel60I1.pRelElec 			= &I1;									//��ǰ����ָ��		
	Rel60I1.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel60I1.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel60I1.pRelVolt			= &UP;									//��ǰ���������ĵ�ѹ��ָ��

	Rel60I1.pRel47UCFG		= &RelCFGCLR;;
	Rel60I1.R47ULKRelayWord   = FALSE1;

	Rel60I1.StartRelayWord		= RW_PI60I1;						//����������μ̵�����
	Rel60I1.ActRelayWord		= RW_TR60I1;							//�������������̵�����
	Rel60I1.AccRelayWord		= FALSE1;								//���������̵�����
	Rel60I1.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel60I1.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel60I1.PRewardRelayWord    = FALSE1;
	Rel60I1.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel60I1.LockRelayWord		= RW_LKPI60I1;						//��α����̵�����
	Rel60I1.RSTRelayWord		= RW_LKRST60I1;							//���ر����̵�����
	Rel60I1.LoopCtrlRelayWord	= RW_LPC60I1;						//����ѭ�����Ƽ̵�����
	Rel60I1.wReportNum			= R_R60I1;								//���������������
	Rel60I1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel60I1.dwK35				= NULL;
	
	Rel60I1.bMakeActRpt			= TRUE;
	Rel60I1.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel60I1.wPickUpReportNum	= R_R60I1_PI;						//��α������к�
	Rel60I1.wResetReportNum		= R_R60I1_RES;						//���ر������к�
	//**********************************************************************	
//	Rel60I1.dwRelTimer			= 0;			 			//��ʱ��
	Rel60I1.byRelFCounter		= 0;						//���ϼ�����
	Rel60I1.byRelRCounter		= 0;						//���ؼ�����		
	Rel60I1.wExcepWordNum		= 8;
	Rel60I1.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// ����: InitRel60I2	 
// ����: ��ʼ������2����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60I2(void)
{
	Rel60I2.pElecSet			= RS_pn60I2C;          					//��������������ֵ
	Rel60I2.pTimeSet			= RS_pn60I2D;				 			//��������ʱ�䶨ֵ
	Rel60I2.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel60I2.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel60I2.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel60I2.pH2Coef				= NULL;

	Rel60I2.pRelCFG				= CFG_pby60I2;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel60I2.pRelSW				= SW_pby60I;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel60I2.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I2.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I2.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I2.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60I2.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel60I2.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel60I2.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel60I2.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel60I2.pRelElec 			= &I2;									//��ǰ����ָ��		
	Rel60I2.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel60I2.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel60I2.pRelVolt			= &UP;									//��ǰ���������ĵ�ѹ��ָ��

	Rel60I2.pRel47UCFG		= &RelCFGCLR;;
	Rel60I2.R47ULKRelayWord   = FALSE1;

	Rel60I2.StartRelayWord		= RW_PI60I2;						//����������μ̵�����
	Rel60I2.ActRelayWord		= RW_TR60I2;							//�������������̵�����
	Rel60I2.AccRelayWord		= FALSE1;								//���������̵�����
	Rel60I2.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel60I2.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel60I2.PRewardRelayWord    = FALSE1;
	Rel60I2.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel60I2.LockRelayWord		= RW_LKPI60I2;						//��α����̵�����
	Rel60I2.RSTRelayWord		= RW_LKRST60I2;							//���ر����̵�����
	Rel60I2.LoopCtrlRelayWord	= RW_LPC60I2;						//����ѭ�����Ƽ̵�����
	Rel60I2.wReportNum			= R_R60I2;								//���������������
	Rel60I2.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel60I2.dwK35				= NULL;
	
	Rel60I2.bMakeActRpt			= TRUE;
	Rel60I2.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel60I2.wPickUpReportNum	= R_R60I2_PI;						//��α������к�
	Rel60I2.wResetReportNum		= R_R60I2_RES;						//���ر������к�
	//**********************************************************************	
//	Rel60I2.dwRelTimer			= 0;			 			//��ʱ��
	Rel60I2.byRelFCounter		= 0;						//���ϼ�����
	Rel60I2.byRelRCounter		= 0;						//���ؼ�����		
	Rel60I2.wExcepWordNum		= 8;
	Rel60I2.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// ����: InitRel50ST	 
// ����: ��ʼ��֧·��������Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50ST(void)
{
	Rel50ST.pElecSet			= RS_pn50STC;          					//��������������ֵ
	Rel50ST.pTimeSet			= RS_pn50STD;				 			//��������ʱ�䶨ֵ
	Rel50ST.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel50ST.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel50ST.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50ST.pH2Coef				= NULL;

	Rel50ST.pRelCFG				= CFG_pby50ST;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50ST.pRelSW				= SW_pby50ST;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50ST.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50ST.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50ST.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50ST.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50ST.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel50ST.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50ST.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel50ST.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50ST.pRelElec 			= &I3;									//��ǰ����ָ��		
	Rel50ST.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel50ST.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel50ST.pRelVolt			= &UP;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50ST.pRel47UCFG		= &RelCFGCLR;;
	Rel50ST.R47ULKRelayWord   = FALSE1;

	Rel50ST.StartRelayWord		= RW_PI50ST;						//����������μ̵�����
	Rel50ST.ActRelayWord		= RW_TR50ST;							//�������������̵�����
	Rel50ST.AccRelayWord		= FALSE1;								//���������̵�����
	Rel50ST.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel50ST.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel50ST.PRewardRelayWord	= FALSE1;
	Rel50ST.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel50ST.LockRelayWord		= RW_LKPI50ST;						//��α����̵�����
	Rel50ST.RSTRelayWord		= RW_LKRST50ST;							//���ر����̵�����
	Rel50ST.LoopCtrlRelayWord	= RW_LPC50ST;						//����ѭ�����Ƽ̵�����
	Rel50ST.wReportNum			= R_R50ST;								//���������������
	Rel50ST.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50ST.dwK35				= NULL;
	Rel50ST.bMakeActRpt			= TRUE;
	Rel50ST.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel50ST.wPickUpReportNum	= R_R50ST_PI;						//��α������к�
	Rel50ST.wResetReportNum		= R_R50ST_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50ST.dwRelTimer			= 0;			 			//��ʱ��
	Rel50ST.byRelFCounter		= 0;						//���ϼ�����
	Rel50ST.byRelRCounter		= 0;						//���ؼ�����		
	Rel50ST.wExcepWordNum		= 8;
	Rel50ST.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// ����: InitRel60U1	 
// ����: ��ʼ����ѹ1����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60Ud1(void)
{
	Rel60Ud1.pElecSet			= RS_pn60U1V;          					//��������������ֵ
	Rel60Ud1.pTimeSet			= RS_pn60U1D;				 			//��������ʱ�䶨ֵ
	Rel60Ud1.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel60Ud1.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel60Ud1.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel60Ud1.pH2Coef				= NULL;

	Rel60Ud1.pRelCFG				= CFG_pby60U1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel60Ud1.pRelSW				= SW_pby60U;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel60Ud1.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud1.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud1.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud1.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud1.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel60Ud1.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel60Ud1.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel60Ud1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel60Ud1.pRelElec 			= &Ud1;									//��ǰ����ָ��		
	Rel60Ud1.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel60Ud1.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel60Ud1.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��
	Rel60Ud1.pRel47UCFG		= &RelCFGCLR;;
	Rel60Ud1.R47ULKRelayWord   = FALSE1;


	Rel60Ud1.StartRelayWord		= RW_PI60U1;						//����������μ̵�����
	Rel60Ud1.ActRelayWord		= RW_TR60U1;							//�������������̵�����
	Rel60Ud1.AccRelayWord		= FALSE1;								//���������̵�����
	Rel60Ud1.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel60Ud1.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel60Ud1.PRewardRelayWord	= FALSE1;
	Rel60Ud1.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel60Ud1.LockRelayWord		= RW_LKPI60U1;						//��α����̵�����
	Rel60Ud1.RSTRelayWord		= RW_LKRST60U1;							//���ر����̵�����
	Rel60Ud1.LoopCtrlRelayWord	= RW_LPC60U1;						//����ѭ�����Ƽ̵�����
	Rel60Ud1.wReportNum			= R_R60U1;								//���������������
	Rel60Ud1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel60Ud1.dwK35				= NULL;
	
	Rel60Ud1.bMakeActRpt			= TRUE;
	Rel60Ud1.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel60Ud1.wPickUpReportNum	= R_R60U1_PI;						//��α������к�
	Rel60Ud1.wResetReportNum	= R_R60U1_RES;						//���ر������к�
	//**********************************************************************	
//	Rel60Ud1.dwRelTimer			= 0;			 			//��ʱ��
	Rel60Ud1.byRelFCounter		= 0;						//���ϼ�����
	Rel60Ud1.byRelRCounter		= 0;						//���ؼ�����		
	Rel60Ud1.wExcepWordNum		= 8;
	Rel60Ud1.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// ����: InitRel60U2	 
// ����: ��ʼ����ѹ2����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60Ud2(void)
{
	Rel60Ud2.pElecSet			= RS_pn60U2V;          					//��������������ֵ
	Rel60Ud2.pTimeSet			= RS_pn60U2D;				 			//��������ʱ�䶨ֵ
	Rel60Ud2.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel60Ud2.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel60Ud2.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel60Ud2.pH2Coef				= NULL;

	Rel60Ud2.pRelCFG				= CFG_pby60U2;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel60Ud2.pRelSW				= SW_pby60U;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel60Ud2.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud2.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud2.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud2.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud2.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel60Ud2.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel60Ud2.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel60Ud2.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel60Ud2.pRelElec 			= &Ud2;									//��ǰ����ָ��		
	Rel60Ud2.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel60Ud2.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel60Ud2.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel60Ud2.StartRelayWord		= RW_PI60U2;						//����������μ̵�����
	Rel60Ud2.ActRelayWord		= RW_TR60U2;							//�������������̵�����
	Rel60Ud2.AccRelayWord		= FALSE1;								//���������̵�����
	Rel60Ud2.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel60Ud2.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel60Ud2.PRewardRelayWord	= FALSE1;
	Rel60Ud2.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel60Ud2.LockRelayWord		= RW_LKPI60U2;						//��α����̵�����
	Rel60Ud2.RSTRelayWord		= RW_LKRST60U2;							//���ر����̵�����
	Rel60Ud2.LoopCtrlRelayWord	= RW_LPC60U2;						//����ѭ�����Ƽ̵�����
	Rel60Ud2.wReportNum			= R_R60U2;								//���������������
	Rel60Ud2.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel60Ud2.dwK35				= NULL;

	
	Rel60Ud2.pRel47UCFG		= &RelCFGCLR;;
	Rel60Ud2.R47ULKRelayWord   = FALSE1;
	Rel60Ud2.bMakeActRpt			= TRUE;
	Rel60Ud2.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel60Ud2.wPickUpReportNum	= R_R60U2_PI;						//��α������к�
	Rel60Ud2.wResetReportNum	= R_R60U2_RES;						//���ر������к�
	//**********************************************************************	
//	Rel60Ud2.dwRelTimer			= 0;			 			//��ʱ��
	Rel60Ud2.byRelFCounter		= 0;						//���ϼ�����
	Rel60Ud2.byRelRCounter		= 0;						//���ؼ�����		
	Rel60Ud2.wExcepWordNum		= 8;
	Rel60Ud2.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// ����: InitRel60U3	 
// ����: ��ʼ����ѹ3����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel60Ud3(void)
{
	Rel60Ud3.pElecSet			= RS_pn60U3V;          					//��������������ֵ
	Rel60Ud3.pTimeSet			= RS_pn60U3D;				 			//��������ʱ�䶨ֵ
	Rel60Ud3.pHarmKhr1Set		= NULL;									//��������г����Ȩϵ��
	Rel60Ud3.pVolLockSet			= NULL;				 	    				//����������ѹ������ֵ
	Rel60Ud3.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel60Ud3.pH2Coef				= NULL;

	Rel60Ud3.pRelCFG				= CFG_pby60U3;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel60Ud3.pRelSW				= SW_pby60U;         						//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel60Ud3.pRelHLockCFG		= &RelCFGCLR;			    					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud3.pRelHSuprCFG		= &RelCFGCLR; 							//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud3.pRelPDCFG			= &RelCFGCLR;		    						//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud3.pRelVolLockCFG		= &RelCFGCLR;								//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel60Ud3.pRelFDCFG			= &RelCFGCLR;								//���ʷ����ܿ���λ--1:������2:������
	Rel60Ud3.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel60Ud3.pRelFLCFG			= &RelCFGCLR;								//���ϲ��Ͷ��
	Rel60Ud3.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel60Ud3.pRelElec 			= &Ud3;									//��ǰ����ָ��		
	Rel60Ud3.pRelHarm3			= &IP3;									//��ǰ����г��ָ��	
	Rel60Ud3.pRelHarm5			= &IP5;									//��ǰ���г��ָ��	
	Rel60Ud3.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��
	Rel60Ud3.pRel47UCFG		= &RelCFGCLR;;
	Rel60Ud3.R47ULKRelayWord   = FALSE1;

	Rel60Ud3.StartRelayWord		= RW_PI60U3;						//����������μ̵�����
	Rel60Ud3.ActRelayWord		= RW_TR60U3;							//�������������̵�����
	Rel60Ud3.AccRelayWord		= FALSE1;								//���������̵�����
	Rel60Ud3.AccActRelayWord		= FALSE1;								//����ٶ����̵�����
	Rel60Ud3.PForwardRelayWord	= FALSE1;								//���ʷ���̵�����
	Rel60Ud3.PRewardRelayWord	= FALSE1;
	Rel60Ud3.H2LockRelayWord		= FALSE1;								//����г�������̵�����
	Rel60Ud3.LockRelayWord		= RW_LKPI60U3;						//��α����̵�����
	Rel60Ud3.RSTRelayWord		= RW_LKRST60U3;							//���ر����̵�����
	Rel60Ud3.LoopCtrlRelayWord	= RW_LPC60U3;						//����ѭ�����Ƽ̵�����
	Rel60Ud3.wReportNum			= R_R60U3;								//���������������
	Rel60Ud3.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel60Ud3.dwK35				= NULL;
	
	Rel60Ud3.bMakeActRpt			= TRUE;
	Rel60Ud3.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************��η��ر��沿��*************	yanxs  13-03-21
	Rel60Ud3.wPickUpReportNum	= R_R60U3_PI;						//��α������к�
	Rel60Ud3.wResetReportNum	= R_R60U3_RES;						//���ر������к�
	//**********************************************************************	
//	Rel60Ud3.dwRelTimer			= 0;			 			//��ʱ��
	Rel60Ud3.byRelFCounter		= 0;						//���ϼ�����
	Rel60Ud3.byRelRCounter		= 0;						//���ؼ�����		
	Rel60Ud3.wExcepWordNum		= 8;
	Rel60Ud3.wInfo				= 0;

}
//------------------------------------------------------------------------------------------ 
// ����: InitRel27U	 
// ����: ��ʼ����ѹ����Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel27U()
{
	Rel27U.pELeSet					= RS_pn27V;						//������ֵ 
	Rel27U.pTimeSet					= RS_pn27D;						//ʱ�䶨ֵ 
	Rel27U.pLockCurSet				= RS_pn27_LK50;					//����������ֵ 

	Rel27U.pRelCFG					= CFG_pby27;					  	 //Ƿ���������� 
	Rel27U.pRelSW					= SW_pby27;					  	 //Ƿ��������ѹ�� 
	Rel27U.pRelAlarmCFG				= &RelCFGCLR;							//Ƿ������Ͷ��բ 
	Rel27U.pRelCurLockCFG			= &RelCFGSET;
	
	Rel27U.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ�� 

	Rel27U.pRelElec					= &UP;						//��ǰ�ĵ���ָ�� 
	Rel27U.pRelLockElec				= &IP;						//�������� 
	Rel27U.ActRelayWord				= RW_TR27;					//Ƿ�����������̵����� 
	Rel27U.StartRelayWord			= RW_PI27;					//Ƿ��������μ̵����� 
	Rel27U.AlarmRelayWord			= FALSE1;					//Ƿ�������澯�̵����� 
	Rel27U.LockRelayWord			= RW_LKPI27;				//��α����̵����� 
	Rel27U.RSTRelayWord				= RW_LKRST27;					//���Ϸ��ر����̵����� 
	Rel27U.LoopCtrlRelayWord		= RW_LPC27;				//����ѭ�����Ƽ̵����� 
	Rel27U.wR52BFRealyword			= RW_52BF;
	Rel27U.wActRptNum				= R_R27U;					//Ƿ����������������� 
	Rel27U.wAlarmRptNum				= 0xFFFF;					//Ƿ�������澯������� 
	Rel27U.wTRFailRptNum			= R_TRFAIL;					//��·����բʧ�ܱ������
	Rel27U.wPickUpRptNum			= R_ACTPICK;					//���������������
	Rel27U.RptSerialNum				= &g_RptSerialNum;				//�����������к�
	
	Rel27U.R27RptSerialNum			= 0;
//	Rel27U.dwRelTimer				= 0;						//Ƿ��������ʱ�� 	
	Rel27U.byRelFCounter			= 0;					//���ϼ����� 
	Rel27U.byRelRCounter			= 0;					//���ؼ����� 
	Rel27U.bHaveMake52BFRptFlag		= FALSE;
	Rel27U.wExcepWordNum			= 14;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
	Rel27U.wInfo				 	= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: InitRelPT	 
// ����: ��ʼ��PT���߱���Ԫ�� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRelPT()
{	
	RelPT.pRelPTBrokCFG 		= CFG_pby74PT;						//PT��������

	RelPT.pR74PTU1Set			= RS_pn74PTV;						//PT���߼���ѹ��ֵ
	RelPT.pR74PTU2Set			= NULL;
	RelPT.pR74PTISet			= RS_pn74PTC;						//pt���߼�������ֵ
	RelPT.pR74PTTSet			= RS_pn74PTD;						//pt���߼��ʱ�䶨ֵ
	
	RelPT.pRelCur				= &IP;							//�ж��õ���
	RelPT.pRelU1				= &U1;							//�ж��õ�һ·��ѹ	
	RelPT.pRelU2				= NULL;

	RelPT.StartRelayWord		= RW_PI74PT;						//PT1������μ̵�����
	RelPT.ActRelayWord			= RW_AR74PT;						//PT1���߶����̵�����
	RelPT.dwSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��
	RelPT.wRelRptNum			= R_PTBR;						//PT���߶��������1
	RelPT.wRelResRptNum			= R_PTBR_RES;					//PT���߷��ر����1
	 
//	RelPT.dwRelTimer			= 0;							//PT1���߶�ʱ��	
	RelPT.wExcepWordNum			= 5;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6
}
void CRelayTask::InitStructCheck(void)
{	
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&Rel50I0;
	StructCheck[0].bigsize = sizeof(Rel50I0)/2-Rel50I0.wExcepWordNum;
	StructCheck[1].pstruct = (WORD*)&Rel50I1;
	StructCheck[1].bigsize = sizeof(Rel50I1)/2-Rel50I1.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel59U;
	StructCheck[2].bigsize = sizeof(Rel59U)/2-Rel59U.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel50IH;
	StructCheck[3].bigsize = sizeof(Rel50IH)/2-Rel50IH.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel60I1;
	StructCheck[4].bigsize = sizeof(Rel60I1)/2-Rel60I1.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel60I2;
	StructCheck[5].bigsize = sizeof(Rel60I2)/2-Rel60I2.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel50ST;
	StructCheck[6].bigsize = sizeof(Rel50ST)/2-Rel50ST.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel60Ud1;
	StructCheck[7].bigsize = sizeof(Rel60Ud1)/2-Rel60Ud1.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&Rel60Ud2;
	StructCheck[8].bigsize = sizeof(Rel60Ud2)/2-Rel60Ud2.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&Rel60Ud3;
	StructCheck[9].bigsize = sizeof(Rel60Ud3)/2-Rel60Ud3.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&Rel27U;
	StructCheck[10].bigsize = sizeof(Rel27U)/2-Rel27U.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)&RelPT;
	StructCheck[11].bigsize = sizeof(RelPT)/2-RelPT.wExcepWordNum;
	StructCheck[12].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[12].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;	
	for(LONG j=0;j<CHECK_NUM;j++)
	{
		for(LONG i=0;i<StructCheck[j].bigsize;i++)
		{
			acc += StructCheck[j].pstruct[i];
		}
		StructCheck[j].acc = acc;
		acc = 0x55aa;
	}

	RelayCheckInitOK = TRUE;
}

//------------------------------------------------------------------------------------------ 
// ����: OnRelayChange	 
// ����: ��Ӧ��ֵ�޸���Ϣ���� 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::OnRelayChange(DWORD dwDataType,DWORD dwIndex)
{
	//���³�ʼ����������
	::InitAllRelayMem();
	InitRealy();	
	InitSet();
	InitDCRecInfo();
	InitStructCheck();
	
	UpDateCFG();
	UpDateSW();

}
void CRelayTask::OnLogicChanged(void)
{
	m_bLogicChanged = TRUE;
	m_dwWaitLogicCounter = dwRelayMSCnt;

	m_bIsLogicOk &= ResetLogicParseModule(TRUE);
}

//------------------------------------------------------------------------------------------ 
// ����: ģֵ�����б����							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	BYTE  pChannel[]={CHANNEL_6560_IP};
	if(ReadRelayWord(RW_52BF))  //��·����բʧ�ܣ������б��������жϴ��� 
	{
		WriteRelayWord(PickUpRW, RELAY_OFF);
		return FALSE;
	}
	if((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(Rel50I0.pRelElec->Mod > *Rel50I0.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*Rel50I1.pRelCFG == CFG_STATUS_SET)&& (*Rel50I1.pRelSW == SW_STATUS_SET)) //����Ͷ�� 
	{
		if(Rel50I1.pRelElec->Mod > *Rel50I1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50IH.pRelCFG == CFG_STATUS_SET) && (*Rel50IH.pRelSW == SW_STATUS_SET)) //г������Ͷ�� 
	{
		if(Rel50IH.pRelElec->Mod > *Rel50IH.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60I1.pRelCFG == CFG_STATUS_SET)&& (*Rel60I1.pRelSW == SW_STATUS_SET)) //����1��Ͷ��
	{
		if(Rel60I1.pRelElec->Mod > *Rel60I1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60I2.pRelCFG == CFG_STATUS_SET) && (*Rel60I2.pRelSW == SW_STATUS_SET)) //����2��Ͷ��
	{
		if(Rel60I2.pRelElec->Mod > *Rel60I2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50ST.pRelCFG == CFG_STATUS_SET) && (*Rel50ST.pRelSW == SW_STATUS_SET)) //֧·����Ͷ��
	{
		if(Rel50ST.pRelElec->Mod > *Rel50ST.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60Ud1.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud1.pRelSW == SW_STATUS_SET)) //��ѹ1Ͷ��
	{
		if(Rel60Ud1.pRelElec->Mod > *Rel60Ud1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60Ud2.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud2.pRelSW == SW_STATUS_SET)) //��ѹ2Ͷ��
	{
		if(Rel60Ud2.pRelElec->Mod > *Rel60Ud2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60Ud3.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud3.pRelSW == SW_STATUS_SET)) //��ѹ3Ͷ��
	{
		if(Rel60Ud3.pRelElec->Mod > *Rel60Ud3.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel59U.pRelCFG == CFG_STATUS_SET) && (*Rel59U.pRelSW == SW_STATUS_SET)) //��ѹ����Ͷ��
	{
		if(Rel59U.pRelElec->Mod > *Rel59U.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}

//ͻ���������жϲ���
	//������һ������Ͷ��
	if(((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET)) || \
		((*Rel50I1.pRelCFG == CFG_STATUS_SET) && (*Rel50I1.pRelSW == SW_STATUS_SET)) || \
		((*Rel50IH.pRelCFG == CFG_STATUS_SET) && (*Rel50IH.pRelSW == SW_STATUS_SET)) || \
		((*Rel60I1.pRelCFG == CFG_STATUS_SET) && (*Rel60I1.pRelSW == SW_STATUS_SET)) || \
		((*Rel60I2.pRelCFG == CFG_STATUS_SET) && (*Rel60I2.pRelSW == SW_STATUS_SET))  || \
		((*Rel60Ud1.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud1.pRelSW == SW_STATUS_SET)) || \
		((*Rel60Ud2.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud2.pRelSW == SW_STATUS_SET)) || \
		((*Rel60Ud3.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud3.pRelSW == SW_STATUS_SET)) || \
		((*Rel50ST.pRelCFG == CFG_STATUS_SET) && (*Rel50ST.pRelSW == SW_STATUS_SET)) ||	\
		((*Rel59U.pRelCFG == CFG_STATUS_SET) && (*Rel59U.pRelSW == SW_STATUS_SET)))
	
	{
		PickCheck = 0;
		ReadPreSingleData(dwPickDataBuf,pChannel,1,RELAY_SAMP_INTERVAL);
		//�㷨1�����ݺ�RELAY_SAMP_INTERVALȷ����ͨ���㷨
	//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i];
			lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i];
			lSampDate3 = dwPickDataBuf[i];
			lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));

			
	//		if(lPickValue>*RS_pnPIC)
				
			//���ø����ż�ֵ�жϣ���I>0.25*��I'+ C,���Ц�I'Ϊ�ϸ�һ���ڽ��ĵ��ͻ����ֵ��CΪͻ����������ֵ
			if(lPickValue*4>*RS_pnPIC*4+m_lPickUpValue)
			{
				PickCheck++;
				if(PickCheck ==3)
				{					
					QWORD PickTime; 					
					ReadAbsTime(&PickUpTime);
					PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
					PickUpTime.Lo= (DWORD)PickTime;
					PickUpTime.Hi= (WORD)(PickTime>>32);	

					WriteRelayWord(PickUpRW, RELAY_ON);
					ReadAbsTime(&PickUpTime);
					DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
					RptSerialNumOfAct = g_RptSerialNum;
					DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
					RptSerialNumOfStart = g_RptSerialNum;
					PickCheck =0;
					PickCheckNext =0;
					return TRUE;	
				}
				if(i<2)
				{
					PickCheckNext++;
					if(PickCheckNext ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	

						WriteRelayWord(PickUpRW, RELAY_ON);
						ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
						RptSerialNumOfStart = g_RptSerialNum;
						PickCheck =0;
						PickCheckNext =0;
						return TRUE;	
					}
				}
				if((i>=2)&&((RELAY_SAMP_INTERVAL-i) >= 3))
					PickCheckNext =0;
				
				if((RELAY_SAMP_INTERVAL-i) < 3)
					PickCheckNext++;
			}
			else
			{
				PickCheck =0;
				PickCheckNext=0;
			}
			m_lPickUpValue = lPickValue;
		}	
	}
	
	WriteRelayWord(PickUpRW, RELAY_OFF);
	return FALSE;
}
//------------------------------------------------------------------------------------------ 
// ����: ����ѭ�����Ƴ���							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
void CRelayTask::FalutProcess(void)
{ 	
	//LONG checktime=0;
	if(FaultOnFlag ==FALSE)   						//��δ������ϴ���
	{		
		RelayPickUpCHK();					//ִ���������
		if(ReadRelayWord(PickUpRW))
		{
			ResetAllRelay(TRUE);
			FaultSINum = RELAY_SAMP_INTERVAL;       //������������ʼ�����ϴ���������������
			FaultOnFlag = TRUE;
			WriteRelayWord(RelPT.StartRelayWord,RELAY_OFF);
//			return;
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(FaultSINum<POINT_PER_PERIOD)					//���ϴ���������������С��һ�ܲ���������
	{
		return;
	}
	else
	{
		if(HaveMakeStartRptFlag == FALSE)
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}
		
		GeneralOverValueRelay(&Rel50I0);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel50I1);				//���ù��������ӳ���
		GeneralOverValueRelay(&Rel60I1);				//���ò���1�����ӳ���
		GeneralOverValueRelay(&Rel60I2);				//���ò���2�����ӳ���
		GeneralOverValueRelay(&Rel50ST);				//����֧·���������ӳ���
		GeneralOverValueRelay(&Rel50IH);				//����г�����������ӳ���
	 	GeneralOverValueRelay(&Rel60Ud1);				//���ò�ѹ1�����ӳ���
		GeneralOverValueRelay(&Rel60Ud2);				//���ò�ѹ2�����ӳ���
		GeneralOverValueRelay(&Rel60Ud3);				//���ò�ѹ3�����ӳ���
		GeneralOverValueRelay(&Rel59U);				//���ù�ѹ�����ӳ���
	

		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveMakeTripFailRpt)
			{
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   //����·����բʧ�ܱ���
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}
		if(ReadRelayWord(Rel50I0.LoopCtrlRelayWord)||ReadRelayWord(Rel50I1.LoopCtrlRelayWord)||ReadRelayWord(Rel60I1.LoopCtrlRelayWord)  		//�б���ѭ�����Ƽ̵����ֱ���λ
			||ReadRelayWord(Rel60I2.LoopCtrlRelayWord)||ReadRelayWord(Rel50ST.LoopCtrlRelayWord)||ReadRelayWord(Rel60Ud1.LoopCtrlRelayWord) 
			||ReadRelayWord(Rel60Ud2.LoopCtrlRelayWord)||ReadRelayWord(Rel60Ud3.LoopCtrlRelayWord)||ReadRelayWord(Rel59U.LoopCtrlRelayWord)
			||ReadRelayWord(Rel50IH.LoopCtrlRelayWord))
		{
			
			if(ReadRelayWord(RW_52BF))			//��բʧ�ܣ������б�־������������
			{
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   //����·����բʧ�ܱ���
				WriteRelayWord(PickUpRW, RELAY_OFF);
				FaultCheckOutFlag = FALSE;
				FaultOnFlag = FALSE;
				HaveMakeStartRptFlag= FALSE;
				
				m_bHaveMakeTripFailRpt = FALSE;
				DB_RelayEnd(RptSerialNumOfAct); //�����ݿ���д���������
				return;
			}
			else
			{
				FaultLockTime = dwRelayMSCnt;		//���ù���������ʱ�� 
				FaultCheckOutFlag = TRUE;			//�б���ѭ�����Ƽ̵�������λ���ü�����ϱ�־
				return;
			}
		}
		else				//�ޱ���ѭ�����Ƽ̵�����λ: 1�����ϴ����Ѿ�������2��δ����������
		{
			if(FaultCheckOutFlag ==TRUE	)      // 1�����ϴ����Ѿ�����
			{			
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime>500 )      //���ϴ���������޹���ʱ�䳬��1�룬��λ���������̵����֣����־������������
				{	
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;
					
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct);	
					ResetAllRelay(TRUE);
					return;					
				}
				else
					return;
			}
			else							//δ����������
			{
				if(m_bFaultLKFlag == FALSE)     //���ϸ���������¼��ʼ����ʱ��
				{	
					FaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				
				if(dwRelayMSCnt -FaultLockTime>1000 )	//δ�����������޹���ʱ�䳬��0.5�룬��λ���������̵����֣����־������������
				{
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;
					
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct); 
					m_bFaultLKFlag = FALSE;
					ResetAllRelay(TRUE);
					return;					
				}
				else
					return;
			}			
		}		
	}
}
void CRelayTask::Process52BF()
{
	if(ReadRelayWord(RW_52BF))
	{
		m_b52BFFlag = TRUE;
	}
	if(m_b52BFFlag)
	{
		if(!ReadRelayWord(RW_52BF))
		{
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&Rel27U);
			m_b52BFFlag = FALSE;
		}
	}
}

//------------------------------------------------------------------------------------------
//����: ����Ԫ�����鸴�麯��							                              
// ����: ��                                                                                
// ���:                                                         
// ����: ��                                                                                
//------------------------------------------------------------------------------------------

void CRelayTask::ResetAllRelay(BOOL resetFlag)
{
/*	if(ReadRelayWord(RW_52BF))			//��բʧ�̵ܼ�������λ
	{
		TripFailFlag=TRUE;					//����բʧ�ܱ�־
		return;
	}
	else									//����բʧ�̵ܼ�������λ������բʧ�ܱ�־�����鸴�飬����ȡ����բʧ�̵ܼ����ֵ��½���
	{
		if(TripFailFlag)
		{
*/			
			ResetOverValueRelay(&Rel50I0,resetFlag);
			ResetOverValueRelay(&Rel50I1,resetFlag);
			ResetOverValueRelay(&Rel60I1,resetFlag);
			ResetOverValueRelay(&Rel60I2,resetFlag);
			ResetOverValueRelay(&Rel50ST,resetFlag);
			ResetOverValueRelay(&Rel60Ud1,resetFlag);
			ResetOverValueRelay(&Rel60Ud2,resetFlag);
			ResetOverValueRelay(&Rel60Ud3,resetFlag);
			ResetOverValueRelay(&Rel59U,resetFlag);
			ResetOverValueRelay(&Rel50IH,resetFlag);
//			TripFailFlag=FALSE;
//		}		
//	}	
}
