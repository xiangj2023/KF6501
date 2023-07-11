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
*           		�����㷨����     FOR  KF6571�������ϲ��װ��                                                          
*			                                                                
* ����                                                                             
*      ���ģʽ�϶࣬ͳһ����ʵ�ֱȽϸ��ӣ�
*      ��˸���ģʽ����Ӧ�жϸ����õ��������Կռ任�ɶ��ԣ�����ά��                                                                            
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
*      �޸���           �޸�����          �޸�����                                      
*                                                                                  
*      yanzh            11/12/27          ��ʼ����  
*	   yanzh			2014/3-2014/4     �޸Ĳ��ģʽ
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
#include "fltask.h"
#include "parafunc.h"

extern DWORD dwRelayMSCnt;		//�����ú��������
extern DWORD g_hDbaseTaskID;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD dwRelayDataPtr;	//AD��ǰдָ��
extern DWORD dwADDataWritePtr;
extern BYTE g_RptSerialNum;

extern BOOL RelayCheckErr;			//�����Լ����
extern BOOL RelayCheckInitOK;
extern DWORD dwADResumeCnt;

extern BOOL g_bCallManualFLDataCompleted;
extern BOOL g_bCallHistoryFLDataCompleted;
extern BOOL g_bCallLowValueFLDataCompleted;
extern BYTE *g_MaintSW;


DWORD dwRelayTaskID;
BOOL RelayTaskInitOK = FALSE;
DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
TStructCheck StructCheck[CHECK_NUM];					//�ṹ��У������
TRunTimeRM	*pSysRM;		//��������ֵ����ָ��
TRunTimeBI  *pSysBI;		//ң��ʵʱ����ָ��

subChannelCheck 		HeartbeatFLData[MAX_SUBSTATION_NUM];   //ͨ�Ŵ��������õĻ�����
subChannelCheck 		ManualFLData[MAX_SUBSTATION_NUM];
subChannelCheck 		RelayFLData[FLCOMPLICATENUM][MAX_SUBSTATION_NUM];
subChannelCheck 		ForeFLData[MAX_SUBSTATION_NUM];
subChannelCheck 		LowValFLData[MAX_SUBSTATION_NUM];

subChannelCheck		HeartbeatData;//����Ϊ����ʱ�������ٻ�����������
subChannelCheck		UploadData;	  //����Ϊ����ʱ�������ٻ�������(���������Ĳ�࣬ʵ���������ʹ��)
subChannelCheck		*g_pHistoryData = (subChannelCheck*)FL_NAVRAM_BASE; //Ӧ����NvRam�У��ٲ���ʷ����ʹ��
subChannelCheck 		*g_pLowValData = g_pHistoryData+1;
SLockSSData				*g_pSS_LockHistory = (SLockSSData*)(FL_NAVRAM_BASE+sizeof(subChannelCheck)*2); //Ӧ����NvRam�У���¼���һ�εı��������������ݣ������ٲ���ʷ //416���ֽ�
SLockSSData 			*g_pSS_LockLowVoltage = g_pSS_LockHistory+1;//Ӧ����NvRam�У���¼��ѹ�������ݣ������ٲ��ѹ����

FLComplicate			FLComplicateInfo[FLCOMPLICATENUM];

extern DWORD IrigQuality;

BEGIN_MESSAGE_MAP(CRelayTask,CApp)
	ON_SM_SAMPLEEND(CRelayTask)
	ON_SM_RELAYDATA(CRelayTask)
	ON_SM_TIMEOUT(CRelayTask)
END_MESSAGE_MAP()

void RelayTask(DWORD *pdwApp)
{
	TMessage msg;
	DWORD status = ERR_MESSAGE_OK;
	OS_TaskDesp *pTask;

	OS_Sleep(1500);
	CRelayTask *pRelayTask = new CRelayTask(pdwApp);
	pRelayTask->Init();

	pTask = OS_GetTaskTab(dwRelayTaskID);

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
				case SM_STEPOVERECHO:
					pRelayTask->OnStepOver(msg.hParam);
					break;
				case SM_GETFLDATA:
					pRelayTask->OnGetFLData( msg.hParam, msg.wParam, msg.pParam);
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
/****************************************************************************************
��  ��: Init
��  ��: ���������ʼ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/

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
	InitDCRecInfo();
	
	dwChanDataMask = ( (0x01<<CHANNEL_6571_U1)|(0x01<<CHANNEL_6571_U2)|(0x01<<CHANNEL_6571_U3)|(0x01<<CHANNEL_6571_U4) \
					|(0x01<<CHANNEL_6571_IT1)|(0x01<<CHANNEL_6571_IF1)|(0x01<<CHANNEL_6571_IT2)|(0x01<<CHANNEL_6571_IF2) \
					|(0x01<<CHANNEL_6571_IT3)|(0x01<<CHANNEL_6571_IF3)|(0x01<<CHANNEL_6571_IAT1)|(0x01<<CHANNEL_6571_IAT2));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2));

	dwComposeDataMask = ((0x01<<CHANNEL_6571_IP1)|(0x01<<CHANNEL_6571_IP2)|(0x01<<CHANNEL_6571_IP3)|(0x01<<CHANNEL_6571_CB));

	//---------------------------------------------------------------------
	//��ʼ����ȡ����������ʱ������
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

	//---------------------------------------------------------------------
	//�ϳ�ͨ�������ڻ�д���ݣ�ÿ�λ�д�ĵ���Ϊ������������
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*4);		
	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*4));

	//---------------------------------------------------------------------
	//�ϳ�ͨ�������ڱ��α�������ϳɲ�������жϣ���Ҫ�Ǳ�������
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);		
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*3));

	//---------------------------------------------------------------------
	//�ϳ�ͨ�������ڱ������ATʱ���ϳ����ϵ�����������Ĳ�����
	dwATSingleBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);		
	ASSERT(dwATSingleBuf != NULL);
	if(dwATSingleBuf == NULL)
	{
		LogError("CRelayTask,dwATSingleBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwATSingleBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*3));

	//---------------------------------------------------------------------
	//ͻ��������ȡ���ݻ�����
	dwPickUpBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*3); 	
	ASSERT(dwPickUpBuf != NULL);
	if(dwPickUpBuf == NULL)
	{
		LogError("CRelayTask,dwPickUpBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickUpBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*3));

	//---------------------------------------------------------------------
	//����1�����������ǰ����ȡ��������
	dwIncre1CurBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);	
	ASSERT(dwIncre1CurBuf != NULL);
	if(dwIncre1CurBuf == NULL)
	{
		LogError("CRelayTask,dwIncre1CurBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwIncre1CurBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM));
	//---------------------------------------------------------------------
	//����2�����������ǰ����ȡ��������
	dwIncre2CurBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);	
	ASSERT(dwIncre2CurBuf != NULL);
	if(dwIncre2CurBuf == NULL)
	{
		LogError("CRelayTask,dwIncre2CurBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwIncre2CurBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM));

	//---------------------------------------------------------------------
	//����Ϊ����ʱ����Ӧ�ٻ�ȡ�����㻺����
	dwTraceDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*CHANNNEL_NUM);	
	ASSERT(dwTraceDataBuf != NULL);
	if(dwTraceDataBuf == NULL)
	{
		LogError("CRelayTask,dwTraceDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwTraceDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*CHANNNEL_NUM));

	//---------------------------------------------------------------------
	//�ϳɵ�ѹ�����㻺����
	dwComposeUBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*4);	
	ASSERT(dwTraceDataBuf != NULL);
	if(dwTraceDataBuf == NULL)
	{
		LogError("CRelayTask,dwTraceDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwTraceDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*4));

	//---------------------------------------------------------------------
	//�����ϳɹ��緽ʽ�£�ͨ�����ݻ�����,������  yanzh 2014��1��17�� 14:25:08
	dwCCModeDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*4);	
	ASSERT(dwCCModeDataBuf != NULL);
	if(dwCCModeDataBuf == NULL)
	{
		LogError("CRelayTask,dwCCModeDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwCCModeDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*4));

	//---------------------------------------------------------------------
	//�����ϳɹ��緽ʽ�£������ٻ�ʱͨ�����ݻ�����,������  yanzh 2014��1��17�� 14:25:08
	dwSubCCModeDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);  
	ASSERT(dwSubCCModeDataBuf != NULL);
	if(dwSubCCModeDataBuf == NULL)
	{
	   LogError("CRelayTask,dwSubCCModeDataBuf",FILE_LINE,"malloc error.");
	   return;
	}
	memset(dwSubCCModeDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));
	//---------------------------------------------------------------------
	
	//��ʼ���߼����̲���
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	
	//��ʼ�������Լ�Ϊ����״̬
	for(LONG i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	//���Ƚ���һ�鿪��ʧЧ�Լ���
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();
	m_bLastRecStatusFlag = FALSE;

	RelCFGCLR = CFG_STATUS_CLR;			//�����˳�
	RelCFGSET = CFG_STATUS_SET;			//����Ͷ��
	wFaultSINum =0;
	RptSerialNumOfAct = 0;
	dwFLPickupTime = 0;
	m_bFaultOnFlag = FALSE;
	m_dwFaultSINum = 0;
	HaveMakeStartRptFlag = FALSE;
	m_bFaultCheckOutFlag = FALSE;
	manualFLPickFlag = FALSE; 	 //ʵ����������־
	manualFLTimeoutFlag = FALSE; //ʵ���೬ʱ��־
	lastManualRWStatus = FALSE;
	SubGPSTimeoutFlag = FALSE;
	haveMakeGPSERRRpt = FALSE;	
	m_bStartHandOverFlag = FALSE;		//����Խ���л������־
	m_bRecoverHandOverFlag = FALSE; 	//�ָ�Խ���л������־
	m_lOtherRelayTrip = FALSE;	
	exchangeElecFlag = 0;
	bStopRefreshRMFlag = FALSE;
	pRMHeadAddr = NULL;
	lowVolCounter = 0;	
	lowVolPerTime = RELAY_SAMP_INTERVAL*625;
	lowVolTimeSet = *RS_pn27D*1000;
	m_bLogicChanged = FALSE;
	g_MaintSW = SW_pbyMAINT;
	pTSubData = NULL;
	putoutLedFlag = TRUE;	
	
	//��ض�ֵ��ʼ��
	InitFLIntervalSet();
	//��ʼ�����������INF��
	InitRptSN();
	//��װ�õĹ��ϵ���Ĭ��Ϊ��ʾһ��ֵ������Һ���ϲ����޸�
	DB_SetFDShowMode(FD_SHOWMODE_FIRST);
	//��ʼ��ͨ�����ϼ̵����ֱ�	
	ChanErrRW[0] = RW_CH1ERR;
	ChanErrRW[1] = RW_CH2ERR;
	ChanErrRW[2] = RW_CH3ERR;
	ChanErrRW[3] = RW_CH4ERR;
	ChanErrRW[4] = RW_CH5ERR;	
	//�����Ͼ��뼰������ʼ��Ϊ0
	WriteRMData(RM_pnDist,RM_DIST,0);
	WriteRMData(RM_pnKMS,RM_KMS,0);
	SetTimer(1,5);
	//ȡ����ֵ���ţ��ж��Ƿ���Խ��ģʽ
	if(DB_GetCurrentSETAreaNo() == FLTASK_SETAREA_STEPOVER_A)
	{
		WriteRelayWord(RW_HANDOVERMODE,RELAY_ON);
	}

	
	RelayTaskInitOK = TRUE;

}
/****************************************************************************************
��  ��: InitRealy
��  ��: ��ʼ��������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void  CRelayTask::InitRealy(void)
{
	if(*CFG_pbyISSS == CFG_STATUS_SET)
	{
		InitRel50();
		InitRel21();
		InitRel50D();
		InitSubInfo();
		InitFaultLocate();
	}
	
}
/****************************************************************************************
��  ��: InitFLIntervalSet
��  ��: ��ʼ�������������ض�ֵ
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitFLIntervalSet() 
{	
	DistanceSet[0] = *RS_pnD1;
	DistanceSet[1] = *RS_pnD2;
	DistanceSet[2] = *RS_pnD3;
	DistanceSet[3] = *RS_pnD4;
	DistanceSet[4] = *RS_pnD5;
	
	DistCoef[0] = *RS_pnKss;
	DistCoef[1] = *RS_pnK1;
	DistCoef[2] = *RS_pnK2;
	DistCoef[3] = *RS_pnK3;
	DistCoef[4] = *RS_pnK4;
	DistCoef[5] = *RS_pnK5;
	
	QValue[0] = *RS_pnQss;
	QValue[1] = *RS_pnQ1;
	QValue[2] = *RS_pnQ2;
	QValue[3] = *RS_pnQ3;
	QValue[4] = *RS_pnQ4;
	QValue[5] = *RS_pnQ5;
	
}

/****************************************************************************************
��  ��: OnTimeOut
��  ��: ʱ�䶨ʱ����Ӧ����
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::OnTimeOut(DWORD id)
{
	
	if(dwADResumeCnt ==0)
		return;
	else
	{
		LogicProcess();
	}
}
/****************************************************************************************
��  ��: OnSampleEnd
��  ��: �ж���Ӧ��������������������
��  ��: 
����ֵ: ��
*****************************************************************************************/
void CRelayTask::OnSampleEnd(DWORD arg)
{		
	
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}
	BOCheck();
	dwDataPtr = dwRelayDataPtr;	
	ReadAbsTime(&FLPickTime);
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	WriteComposeBuf();
	CaluUI();          	
	WriteRM();		
 	FaultLocate();	
	FLPickUpCheck();
	LogicProcess();	
 	HandOverProcess();
 //	LockDataForLowVol(); //��ʱ���øù���
	WriteDCChannel();
	if(*CFG_pbyISSS == CFG_STATUS_CLR) //������Ϊ��������⵽��ද���̵�������λ�󼴿ɽ��临λ
	{
		if(ReadRelayWord(RW_AFLACT))
			WriteRelayWord(RW_AFLACT,RELAY_OFF);
		if(ReadRelayWord(RW_MFLACT))
			WriteRelayWord(RW_MFLACT,RELAY_OFF);
	}
	else
	{
		if(putoutLedFlag)
		{			
			if(ReadRelayWord(RW_AFLACT))
				WriteRelayWord(RW_AFLACT,RELAY_OFF);
			if(ReadRelayWord(RW_MFLACT))
				WriteRelayWord(RW_MFLACT,RELAY_OFF);

			putoutLedFlag = FALSE;
		}			
	}
}
/****************************************************************************************
��  ��: BOCheck
��  ��: �����Լ�
		�����Լ�ÿ�α��������Լ�һ·
		ʧЧ�Լ�ÿ200�α��������Լ�һ·
		�ɱ���߼��������غ󣬿����Լ�ֹͣ1��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::BOCheck()
{	
	//���Ƚ��п����Լ�,ÿ�α��������һ·����
	if(!m_bLogicChanged)
		BoBreakCheck();
	
	//ʧЧ�Լ�,200�α����������һ·�������	
	static WORD wInvalidCheckCnt;
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
}
/****************************************************************************************
��  ��: WriteComposeBuf
��  ��: �ϳɲ�����
		���ݸ��ֹ��緽ʽ�ϳɼ�������Ĳ�����
		��������Ҫ���л�д������������
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::WriteComposeBuf()
{
	for(WORD i=0;i<READ_POINT_NUM;i++)
	{
		if(*AS_pnPM == AT2MODE)//��ר���緽ʽ
		{
			dwComposeUBuf[i] = dwSampBuf[i]-dwSampBuf[i+READ_POINT_NUM]; //UP1
			dwComposeUBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM*2]-dwSampBuf[i+READ_POINT_NUM*3]; //UP2
			dwComposeUBuf[i+READ_POINT_NUM*2] = dwSampBuf[i]-dwSampBuf[i+READ_POINT_NUM*2]; //U13
			dwComposeUBuf[i+READ_POINT_NUM*3] = dwSampBuf[i+READ_POINT_NUM]-dwSampBuf[i+READ_POINT_NUM*3]; //U24
		}
		//�����ϳɷ�ʽ�������������е�������кϳɡ�Ҫ��I1������,I2������,I3�Ӻ�������---ֻ����������ϳ�
		if((*CFG_pbyCCMODE == CFG_STATUS_SET)&&(*CFG_pbyISSS == CFG_STATUS_CLR))
		{
			//�ϳ�����TF�ߵ���
			dwCCModeDataBuf[i] = (dwSampBuf[i+READ_POINT_NUM*4]*dwCTRateCoef1+dwSampBuf[i+READ_POINT_NUM*8]*1000)/1000;
			dwCCModeDataBuf[i+READ_POINT_NUM] = (dwSampBuf[i+READ_POINT_NUM*5]*dwCTRateCoef1+dwSampBuf[i+READ_POINT_NUM*9]*1000)/1000;
			//�ϳ�����TF�ߵ���
			dwCCModeDataBuf[i+READ_POINT_NUM*2] = (dwSampBuf[i+READ_POINT_NUM*6]*dwCTRateCoef2+dwSampBuf[i+READ_POINT_NUM*8]*1000)/1000;
			dwCCModeDataBuf[i+READ_POINT_NUM*3] = (dwSampBuf[i+READ_POINT_NUM*7]*dwCTRateCoef2+dwSampBuf[i+READ_POINT_NUM*9]*1000)/1000;
		}
		//�ϳɱ���������ԭʼ��������
		dwComposeBuf[i] = dwSampBuf[i+READ_POINT_NUM*4]-dwSampBuf[i+READ_POINT_NUM*5]; //IP1
		dwComposeBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM*6]-dwSampBuf[i+READ_POINT_NUM*7]; //IP2
		dwComposeBuf[i+READ_POINT_NUM*2] = dwSampBuf[i+READ_POINT_NUM*8]-dwSampBuf[i+READ_POINT_NUM*9]; //IP3
		
	}
	if((*CFG_pbyISSS == CFG_STATUS_SET)&&(*CFG_pbyNONEAT == CFG_STATUS_SET)) //�������AT
	{
		//�������AT������£����ϵ����������е�TF�ߵ������кϳ�
		for(WORD i=0;i<READ_POINT_NUM;i++)
		{
			dwATSingleBuf[i] = dwSampBuf[i+READ_POINT_NUM*4]+dwSampBuf[i+READ_POINT_NUM*5]; //
			dwATSingleBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM*6]+dwSampBuf[i+READ_POINT_NUM*7]; //
			dwATSingleBuf[i+READ_POINT_NUM*2] = dwSampBuf[i+READ_POINT_NUM*8]+dwSampBuf[i+READ_POINT_NUM*9];
		}
	}
	//���㵱ǰ�Ŀ������任ң��ֵ
	BYTE tCB = 0;
	for(int i=0;i<8;i++)
	{
		if(ReadRelayWord(RW_VIN1+i))
			tCB = tCB|(1<<i);
	}
	m_CB = tCB;
	
	//ͨ�����ݻ�д,��������������
	for(WORD i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL] = dwComposeBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL*2] = dwComposeBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL*3] = m_CB;
	}
	
	
	WriteComposeData(dwRecBuf, dwComposeDataMask);
}

/****************************************************************************************
��  ��: CaluUI
��  ��: �����ͨ���ĵ�����ѹģֵ
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::CaluUI(void)
{	
	// 1.U1
	CaluBaseFourier_20(dwSampBuf,&m_U1);
	CaluModValue(&m_U1);
	CaluAngle(&m_U1,AngleTab);
	m_U1mod = m_U1.Mod*(*AS_pnUPTRatio);
	if(m_U1.Mod >= *AS_pn59U1)
	{
		WriteRelayWord(RW_59U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(m_U1.Mod >= *AS_pn27U1)
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}
	
	// 1.U2
	CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM,&m_U2);
	CaluModValue(&m_U2);
	CaluAngle(&m_U2,AngleTab);
	m_U2mod = m_U2.Mod*(*AS_pnUPTRatio);
	if(m_U2.Mod >= *AS_pn59U2)
	{
		WriteRelayWord(RW_59U2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(m_U2.Mod >= *AS_pn27U2)
	{
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U2, RELAY_ON);
	}

	// 1.U3
	CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*2,&m_U3);
	CaluModValue(&m_U3);
	CaluAngle(&m_U3,AngleTab);
	m_U3mod = m_U3.Mod*(*AS_pnUPTRatio);
	if(m_U3.Mod >= *AS_pn59U3)
	{
		WriteRelayWord(RW_59U3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3, RELAY_OFF);
	}
	if(m_U3.Mod >= *AS_pn27U3)
	{
		WriteRelayWord(RW_27U3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3, RELAY_ON);
	}

	// 1.U4
	CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*3,&m_U4);
	CaluModValue(&m_U4);
	CaluAngle(&m_U4,AngleTab);
	m_U4mod = m_U4.Mod*(*AS_pnUPTRatio);
	if(m_U4.Mod >= *AS_pn59U4)
	{
		WriteRelayWord(RW_59U4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U4, RELAY_OFF);
	}
	if(m_U4.Mod >= *AS_pn27U4)
	{
		WriteRelayWord(RW_27U4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U4, RELAY_ON);
	}
	// 3.IT1
	if((*CFG_pbyCCMODE == CFG_STATUS_SET)&&(*CFG_pbyISSS == CFG_STATUS_CLR)) //�����ϳɷ�ʽ�������������е�������кϳɡ�Ҫ��I1������,I2������,I3�Ӻ�������
	{
		CaluBaseFourier_20(dwCCModeDataBuf,&m_IT1);
		CaluModValue(&m_IT1);
		CaluAngle(&m_IT1,AngleTab);
		m_IT1mod = m_IT1.Mod*(*AS_pnI3CTRatio);
		
		CaluBaseFourier_20(dwCCModeDataBuf+READ_POINT_NUM,&m_IF1);
		CaluModValue(&m_IF1);
		CaluAngle(&m_IF1,AngleTab);
		m_IF1mod = m_IF1.Mod*(*AS_pnI3CTRatio);

		
		CaluBaseFourier_20(dwCCModeDataBuf+READ_POINT_NUM*2,&m_IT2);
		CaluModValue(&m_IT2);
		CaluAngle(&m_IT2,AngleTab);
		m_IT2mod = m_IT2.Mod*(*AS_pnI3CTRatio);

		CaluBaseFourier_20(dwCCModeDataBuf+READ_POINT_NUM*3,&m_IF2);
		CaluModValue(&m_IF2);
		CaluAngle(&m_IF2,AngleTab);
		m_IF2mod = m_IF2.Mod*(*AS_pnI3CTRatio);

		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*8,&m_IT3);
		CaluModValue(&m_IT3);
		CaluAngle(&m_IT3,AngleTab);
		m_IT3mod = m_IT3.Mod*(*AS_pnI3CTRatio);
		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*9,&m_IF3);
		CaluModValue(&m_IF3);
		CaluAngle(&m_IF3,AngleTab);
		m_IF3mod = m_IF3.Mod*(*AS_pnI3CTRatio);
	}
	else
	{
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*4,&m_IT1);
		CaluModValue(&m_IT1);
		CaluAngle(&m_IT1,AngleTab);
		m_IT1mod = m_IT1.Mod*(*AS_pnI1CTRatio);
		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*5,&m_IF1);
		CaluModValue(&m_IF1);
		CaluAngle(&m_IF1,AngleTab);
		m_IF1mod = m_IF1.Mod*(*AS_pnI1CTRatio);
	
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*6,&m_IT2);
		CaluModValue(&m_IT2);
		CaluAngle(&m_IT2,AngleTab);
		m_IT2mod = m_IT2.Mod*(*AS_pnI2CTRatio);
		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*7,&m_IF2);
		CaluModValue(&m_IF2);
		CaluAngle(&m_IF2,AngleTab);
		m_IF2mod = m_IF2.Mod*(*AS_pnI2CTRatio);
		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*8,&m_IT3);
		CaluModValue(&m_IT3);
		CaluAngle(&m_IT3,AngleTab);
		m_IT3mod = m_IT3.Mod*(*AS_pnI3CTRatio);
		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*9,&m_IF3);
		CaluModValue(&m_IF3);
		CaluAngle(&m_IF3,AngleTab);
		m_IF3mod = m_IF3.Mod*(*AS_pnI3CTRatio);

	}
	//�������AT����£�AT����Ϊ�ϳɵ��������������ߵĺϳɵ�����ʸ����
	//IAT=IT1+IF1+IT2+IF2+IT3+IF3,��Ϊʸ��
	if((*CFG_pbyISSS == CFG_STATUS_SET)&&(*CFG_pbyNONEAT == CFG_STATUS_SET)) 
	{
		CaluBaseFourier_20(dwATSingleBuf,&m_I1_AT);
		CaluModValue(&m_I1_AT);
		CaluBaseFourier_20(dwATSingleBuf+READ_POINT_NUM,&m_I2_AT);
		CaluModValue(&m_I2_AT);
		CaluBaseFourier_20(dwATSingleBuf+READ_POINT_NUM*2,&m_I3_AT);
		CaluModValue(&m_I3_AT);

		m_I_AT.Real = (m_I1_AT.Real*(*AS_pnI1CTRatio)+m_I2_AT.Real*(*AS_pnI2CTRatio)+m_I3_AT.Real*(*AS_pnI3CTRatio))/(*AS_pnI1CTRatio);
		m_I_AT.Imag = (m_I1_AT.Imag*(*AS_pnI1CTRatio)+m_I2_AT.Imag*(*AS_pnI2CTRatio)+m_I3_AT.Imag*(*AS_pnI3CTRatio))/(*AS_pnI1CTRatio);
		CaluModValue(&m_I_AT);
		m_IAT_SS = m_I_AT.Mod*(*AS_pnI1CTRatio);
	}
	//�������������AT����£�����AT��������ʵ�ʲ������ã���AT���ϵ���Ϊ���ߵ�ʸ����
	else
	{		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*10,&m_IAT1);
		CaluModValue(&m_IAT1);
		m_IAT1mod = m_IAT1.Mod*(*AS_pnIATCTRatio);
		
		CaluBaseFourier_20(dwSampBuf+READ_POINT_NUM*11,&m_IAT2);
		CaluModValue(&m_IAT2);
		m_IAT2mod = m_IAT2.Mod*(*AS_pnIATCTRatio);

		TRelElecValPar tempAT;
		tempAT.Real = (m_IAT1.Real+m_IAT2.Real);
		tempAT.Imag = (m_IAT1.Imag+m_IAT2.Imag);
		CaluModValue(&tempAT);
		m_IAT_SS = tempAT.Mod*(*AS_pnIATCTRatio);
		//m_IAT_SS = m_IAT1mod+m_IAT2mod;
		
	}
	if(*CFG_pbyISSS == CFG_STATUS_SET)//��Ϊ����������㱣����������������Ϣ
	{
		if(*AS_pnPM == AT2MODE)  //ȫ������ʽ�£������õĵ�ѹΪ�ϳɵ�ѹ
		{
			CaluBaseFourier_20(dwComposeUBuf,&m_UP1);
			CaluModValue(&m_UP1);
			m_UP1mod = m_UP1.Mod*(*AS_pnUPTRatio);

			CaluBaseFourier_20(dwComposeUBuf+READ_POINT_NUM,&m_UP2);
			CaluModValue(&m_UP2);
			m_UP2mod = m_UP2.Mod*(*AS_pnUPTRatio);

			CaluBaseFourier_20(dwComposeUBuf+READ_POINT_NUM*2,&m_U13);
			CaluModValue(&m_U13);
			m_U13mod = m_U13.Mod*(*AS_pnUPTRatio);

			CaluBaseFourier_20(dwComposeUBuf+READ_POINT_NUM*3,&m_U24);
			CaluModValue(&m_U24);
			m_U24mod = m_U24.Mod*(*AS_pnUPTRatio);
		}

		CaluBaseFourier_20(dwComposeBuf,&m_IP1);
		CaluModValue(&m_IP1);
		m_IP1mod = m_IP1.Mod*(*AS_pnI1CTRatio);	
		if(m_IP1.Mod >= *AS_pn50I1)
		{
			WriteRelayWord(RW_50I1, RELAY_ON);
		}
		else
		{
			WriteRelayWord(RW_50I1, RELAY_OFF);
		}
		if(m_IP1.Mod >= *AS_pn37I1)
		{
			WriteRelayWord(RW_37I1, RELAY_OFF);
		}
		else
		{
			WriteRelayWord(RW_37I1, RELAY_ON);
		}
		CaluSecondFourier_20(dwComposeBuf,&m_IP1_h2);
		CaluModValue(&m_IP1_h2);
		CaluThirdFourier_20(dwComposeBuf,&m_IP1_h3);
		CaluModValue(&m_IP1_h3);
		CaluFifthFourier_20(dwComposeBuf,&m_IP1_h5);
		CaluModValue(&m_IP1_h5);
		
		CaluBaseFourier_20(dwComposeBuf+READ_POINT_NUM,&m_IP2);
		CaluModValue(&m_IP2);
		m_IP2mod = m_IP2.Mod*(*AS_pnI2CTRatio);	
		if(m_IP2.Mod >= *AS_pn50I2)
		{
			WriteRelayWord(RW_50I2, RELAY_ON);
		}
		else
		{
			WriteRelayWord(RW_50I2, RELAY_OFF);
		}
		if(m_IP2.Mod >= *AS_pn37I2)
		{
			WriteRelayWord(RW_37I2, RELAY_OFF);
		}
		else
		{
			WriteRelayWord(RW_37I2, RELAY_ON);
		}
		CaluSecondFourier_20(dwComposeBuf+READ_POINT_NUM,&m_IP2_h2);
		CaluModValue(&m_IP2_h2);
		CaluThirdFourier_20(dwComposeBuf+READ_POINT_NUM,&m_IP2_h3);
		CaluModValue(&m_IP2_h3);
		CaluFifthFourier_20(dwComposeBuf+READ_POINT_NUM,&m_IP2_h5);
		CaluModValue(&m_IP2_h5);
		
		CaluBaseFourier_20(dwComposeBuf+READ_POINT_NUM*2,&m_IP3);
		CaluModValue(&m_IP3);
		m_IP3mod = m_IP3.Mod*(*AS_pnI3CTRatio);	
		if(m_IP3.Mod >= *AS_pn50I3)
		{
			WriteRelayWord(RW_50I3, RELAY_ON);
		}
		else
		{
			WriteRelayWord(RW_50I3, RELAY_OFF);
		}
		if(m_IP3.Mod >= *AS_pn37I3)
		{
			WriteRelayWord(RW_37I3, RELAY_OFF);
		}
		else
		{
			WriteRelayWord(RW_37I3, RELAY_ON);
		}
		CaluSecondFourier_20(dwComposeBuf+READ_POINT_NUM*2,&m_IP3_h2);
		CaluModValue(&m_IP3_h2);
		CaluThirdFourier_20(dwComposeBuf+READ_POINT_NUM*2,&m_IP3_h3);
		CaluModValue(&m_IP3_h3);
		CaluFifthFourier_20(dwComposeBuf+READ_POINT_NUM*2,&m_IP3_h5);
		CaluModValue(&m_IP3_h5);
		
		//���ݱ�����λ��״̬����������˳��
		AdjustCurrent();

		//��ͨAT���緽ʽ�£�U1�������TF֮��ĵ�ѹ��������ѹ���迹����ĵ�ѹ��ΪU1
		if(*AS_pnPM == AT1MODE)
		{
			m_UP = m_U1;
			m_ULock = m_U1;
		}
		//��ר���緽ʽ�£�U1/U2�������T/F�ߵĶԵص�ѹ����˱��������õĵ�ѹ��TF֮��ĺϳɵ�ѹ
		//�Ҹ÷�ʽ��û��T�ߺ�F�ߵ������迹
		else
		{
			m_UP = m_UP1;
			LONG temp = MIN3(m_U1.Mod,m_U2.Mod,m_UP1.Mod);
			if(temp == m_U1.Mod)
				m_ULock = m_U1;
			else if(temp == m_U2.Mod)
				m_ULock = m_U2;
			else if(temp == m_UP1.Mod)
				m_ULock = m_UP1;

			if((exchangeElecFlag == I1I2I3)||(exchangeElecFlag == I1I3I2))
			{				
				CaluRX(&m_U1,&m_IT1,&Impd_T,TRUE1,CF_pnZ);	//����T���迹				
				CaluRX(&m_U2,&m_IF1,&Impd_F,TRUE1,CF_pnZ);	//����F���迹
			}
			
			if((exchangeElecFlag == I3I1I2)||(exchangeElecFlag == I3I2I1))
			{				
				CaluRX(&m_U1,&m_IT3,&Impd_T,TRUE1,CF_pnZ);	//����T���迹				
				CaluRX(&m_U2,&m_IF3,&Impd_F,TRUE1,CF_pnZ);	//����F���迹
			}			
			if((exchangeElecFlag == I2I1I3)||(exchangeElecFlag == I2I3I1))
			{				
				CaluRX(&m_U1,&m_IT2,&Impd_T,TRUE1,CF_pnZ);	//����T���迹				
				CaluRX(&m_U2,&m_IF2,&Impd_F,TRUE1,CF_pnZ);	//����F���迹
			}
		}		
		CaluRX(&m_UP,&m_IP,&m_Impd,RW37IP,CF_pnZ);	//�����迹
		
		if((!ReadRelayWord(RW37IP))&&(m_IP_h2.Mod*(*CF_pnC2)>(*RS_pnKh1)*m_IP.Mod))
			WriteRelayWord(RW_LKH2, RELAY_ON);	//�ö���г�������̵�����	
		else
			WriteRelayWord(RW_LKH2, RELAY_OFF);
		
		if(!ReadRelayWord(RW37IP))
		{
			//����г�����ơ��ٶ�г�����ơ�����г��������Ͷ�������¼���K35
			if((*CFG_pby21_H_S== CFG_STATUS_SET)||(*CFG_pby50_H_S== CFG_STATUS_SET))
			{
				if((m_IP.Mod*100<(m_IP_h3.Mod+m_IP_h5.Mod))||(m_IP.Mod<2)) //���EEPROM���ˣ�RW_37IP���������ϣ���ʱҪ��֤IP����0
					K35 = 10000;
				else				
					K35 = *CF_pnC2 + (*RS_pnKhr1)*(m_IP_h3.Mod+m_IP_h5.Mod)/m_IP.Mod;
			}
		}
		else
			K35 = *CF_pnC2;

	}
}
void CRelayTask::AdjustCurrent()
{	
	//�����жϳ���������ߵ������������б��������ж�
	//���б����ö�·��ȡ��
	if(ReadRelayWord(RW_BGK152A)&&!ReadRelayWord(RW_BGK252A))
	{			
		//��������������ظ�ֵ
		m_I50D1_0 = m_IP3;
		m_I50D1_2 = m_IP3_h2;
		m_I50D1_3 = m_IP3_h3;
		m_I50D1_5 = m_IP3_h5;
		dwInCre1DataMask = 0x01<<CHANNEL_6571_IP3;
		//��������������ظ�ֵ
		m_I50D2_0 = m_IP2;
		m_I50D2_2 = m_IP2_h2;
		m_I50D2_3 = m_IP2_h3;
		m_I50D2_5 = m_IP2_h5;			
		dwInCre2DataMask = 0x01<<CHANNEL_6571_IP2;
		//���е����������е���
		if(m_IP2.Mod>=m_IP3.Mod)
		{				
			exchangeElecFlag = I2I3I1;
			m_IP = m_IP2;
			m_IP_h2 = m_IP2_h2;
			m_IP_h3 = m_IP2_h3;
			m_IP_h5 = m_IP2_h5; 					
			RW37IP = RW_37I2;				
			Rel21.R37IRelayWord = RW_37I2;
		}
		else
		{
			exchangeElecFlag = I3I2I1;
			m_IP = m_IP3;
			m_IP_h2 = m_IP3_h2;
			m_IP_h3 = m_IP3_h3;
			m_IP_h5 = m_IP3_h5; 					
			RW37IP = RW_37I3;				
			Rel21.R37IRelayWord = RW_37I3;
		}
	}
	//���б����ö�·��ȡ��
	else if(ReadRelayWord(RW_BGK152A)&&!ReadRelayWord(RW_BGK252A))
	{
		//��������������ظ�ֵ
		m_I50D1_0 = m_IP1;
		m_I50D1_2 = m_IP1_h2;
		m_I50D1_3 = m_IP1_h3;
		m_I50D1_5 = m_IP1_h5;		
		dwInCre1DataMask = 0x01<<CHANNEL_6571_IP1;
		//��������������ظ�ֵ
		m_I50D2_0 = m_IP3;
		m_I50D2_2 = m_IP3_h2;
		m_I50D2_3 = m_IP3_h3;
		m_I50D2_5 = m_IP3_h5;			
		dwInCre2DataMask = 0x01<<CHANNEL_6571_IP3;
		if(m_IP1.Mod>=m_IP3.Mod)
		{
			exchangeElecFlag = I1I3I2;
			m_IP = m_IP1;
			m_IP_h2 = m_IP1_h2;
			m_IP_h3 = m_IP1_h3;
			m_IP_h5 = m_IP1_h5; 					
			RW37IP = RW_37I1;				
			Rel21.R37IRelayWord = RW_37I1;
		}
		else
		{
			exchangeElecFlag = I3I1I2;
			m_IP = m_IP3;
			m_IP_h2 = m_IP3_h2;
			m_IP_h3 = m_IP3_h3;
			m_IP_h5 = m_IP3_h5; 					
			RW37IP = RW_37I3;				
			Rel21.R37IRelayWord = RW_37I3;
		}
	}
	//���ö�·��δͶ�룬��������������
	else
	{			
		//��������������ظ�ֵ
		m_I50D1_0 = m_IP1;
		m_I50D1_2 = m_IP1_h2;
		m_I50D1_3 = m_IP1_h3;
		m_I50D1_5 = m_IP1_h5;		
		dwInCre1DataMask = 0x01<<CHANNEL_6571_IP1;
		//��������������ظ�ֵ
		m_I50D2_0 = m_IP2;
		m_I50D2_2 = m_IP2_h2;
		m_I50D2_3 = m_IP2_h3;
		m_I50D2_5 = m_IP2_h5;			
		dwInCre2DataMask = 0x01<<CHANNEL_6571_IP2;
		if(m_IP1.Mod>=m_IP2.Mod)
		{
			exchangeElecFlag = I1I2I3;
			m_IP = m_IP1;
			m_IP_h2 = m_IP1_h2;
			m_IP_h3 = m_IP1_h3;
			m_IP_h5 = m_IP1_h5; 					
			RW37IP = RW_37I1;				
			Rel21.R37IRelayWord = RW_37I1;
		}
		else
		{
			exchangeElecFlag = I2I1I3;
			m_IP = m_IP2;
			m_IP_h2 = m_IP2_h2;
			m_IP_h3 = m_IP2_h3;
			m_IP_h5 = m_IP2_h5; 					
			RW37IP = RW_37I2;				
			Rel21.R37IRelayWord = RW_37I2;
		}
	}
		
	m_IP_h2.Mod =m_IP_h2.Mod*CurHarmCal_6571_I[0]/1000;
	m_IP_h3.Mod =m_IP_h3.Mod*CurHarmCal_6571_I[1]/1000;
	m_IP_h5.Mod =m_IP_h5.Mod*CurHarmCal_6571_I[2]/1000;
	*RM_pnIP_H2 = m_IP_h2.Mod;
	DB_LogWriteRM(RM_IP_H2, *RM_pnIP_H2); 
	*RM_pnIP_H3 = m_IP_h3.Mod;
	DB_LogWriteRM(RM_IP_H3, *RM_pnIP_H3); 
	*RM_pnIP_H5 = m_IP_h5.Mod;
	DB_LogWriteRM(RM_IP_H5, *RM_pnIP_H5); 
}

//�����ݿ���д�뱣������ֵ
void CRelayTask::WriteRM(void)
{ 
	TSysTime systime;
	LONG angle;
	//���������������������ֵ��ˢ��
	if(bStopRefreshRMFlag)  
		return;

	//д�뱾���ı�������ֵ������ʾһ��ֵ
	*RM_pnU1 = m_U1.Mod;
	*RM_pnU2 = m_U2.Mod;
	*RM_pnU3 = m_U3.Mod;
	*RM_pnU4 = m_U4.Mod;
	*RM_pnIt1 = m_IT1.Mod;
	*RM_pnIf1 = m_IF1.Mod;
	*RM_pnIt2 = m_IT2.Mod;
	*RM_pnIf2 = m_IF2.Mod;
	*RM_pnIt3 = m_IT3.Mod;
	*RM_pnIf3 = m_IF3.Mod;
	*RM_pnIat1 = m_IAT1.Mod;
	*RM_pnIat2 = m_IAT2.Mod;
	*RM_pnSSIAT = m_IAT_SS;
	*RM_pnUp = m_UP.Mod;
	*RM_pnI1 = m_IP1.Mod;
	*RM_pnI2 = m_IP2.Mod;
	*RM_pnI3 = m_IP3.Mod;
	*RM_pnIP_H2 = m_IP_h2.Mod;
	*RM_pnIP_H3 = m_IP_h3.Mod;
	*RM_pnIP_H5 = m_IP_h5.Mod;
	
	angle = m_U1.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnU1A = angle;
	else
		*RM_pnU1A = angle+36000;

	angle = m_U2.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnU2A = angle;
	else
		*RM_pnU2A = angle+36000;
	
	angle = m_U3.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnU3A = angle;
	else
		*RM_pnU3A = angle+36000;

	angle = m_U4.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnU4A = angle;
	else
		*RM_pnU4A = angle+36000;

	*RM_pnIT1A = 0;

	angle = m_IF1.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnIF1A = angle;
	else
		*RM_pnIF1A = angle+36000;

	angle = m_IT2.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnIT2A = angle;
	else
		*RM_pnIT2A = angle+36000;

	angle = m_IF2.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnIF2A = angle;
	else
		*RM_pnIF2A = angle+36000;

	angle = m_IT3.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnIT3A = angle;
	else
		*RM_pnIT3A = angle+36000;

	angle = m_IF3.Angle - m_IT1.Angle;
	if(angle>0)
		*RM_pnIF3A = angle;
	else
		*RM_pnIF3A = angle+36000;
	
	*RM_pnR_T = Impd_T.Real;
	*RM_pnX_T = Impd_T.Imag;	
	*RM_pnR_F = Impd_F.Real;
	*RM_pnX_F = Impd_F.Imag;
	*RM_pnR_TF = m_Impd.Real;
	*RM_pnX_TF = m_Impd.Imag;	
	*RM_pnNS = IrigQuality;
	
	DB_LogWriteRM(RM_U1, *RM_pnU1);	
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
	DB_LogWriteRM(RM_U3, *RM_pnU3);	
	DB_LogWriteRM(RM_U4, *RM_pnU4);	
	DB_LogWriteRM(RM_IT1, *RM_pnIt1);	
	DB_LogWriteRM(RM_IF1, *RM_pnIf1);	
	DB_LogWriteRM(RM_IT2, *RM_pnIt2);
	DB_LogWriteRM(RM_IF2, *RM_pnIf2);
	DB_LogWriteRM(RM_IT3, *RM_pnIt3);
	DB_LogWriteRM(RM_IF3, *RM_pnIf3);
	DB_LogWriteRM(RM_IAT1, *RM_pnIat1);
	DB_LogWriteRM(RM_IAT2, *RM_pnIat2);
	DB_LogWriteRM(RM_SSIAT, *RM_pnSSIAT);
	DB_LogWriteRM(RM_UP, *RM_pnUp);	
	DB_LogWriteRM(RM_I1, *RM_pnI1);
	DB_LogWriteRM(RM_I2, *RM_pnI2);
	DB_LogWriteRM(RM_I3, *RM_pnI3);	
	DB_LogWriteRM(RM_IP_H2, *RM_pnIP_H2);	
	DB_LogWriteRM(RM_IP_H3, *RM_pnIP_H3);
	DB_LogWriteRM(RM_IP_H5, *RM_pnIP_H5);	
	DB_LogWriteRM(RM_U1A, *RM_pnU1A);
	DB_LogWriteRM(RM_U2A, *RM_pnU2A);
	DB_LogWriteRM(RM_U3A, *RM_pnU3A);
	DB_LogWriteRM(RM_U4A, *RM_pnU4A);
	DB_LogWriteRM(RM_IT1A, *RM_pnIT1A);
	DB_LogWriteRM(RM_IF1A, *RM_pnIF1A);
	DB_LogWriteRM(RM_IT2A, *RM_pnIT2A);
	DB_LogWriteRM(RM_IF2A, *RM_pnIF2A);
	DB_LogWriteRM(RM_IT3A, *RM_pnIT3A);
	DB_LogWriteRM(RM_IF3A, *RM_pnIF3A);
	DB_LogWriteRM(RM_NS, *RM_pnNS);	
	DB_LogWriteRM(RM_R_T, *RM_pnR_T);
	DB_LogWriteRM(RM_X_T, *RM_pnX_T);
	DB_LogWriteRM(RM_R_F, *RM_pnR_F);
	DB_LogWriteRM(RM_X_F, *RM_pnX_F);
	DB_LogWriteRM(RM_R_TF, *RM_pnR_TF);
	DB_LogWriteRM(RM_X_TF, *RM_pnX_TF);

	//������Ϊ�����������ˢ����������
	if(*CFG_pbyISSS == CFG_STATUS_SET) 
	{
		LONG RMNo = RM_BOINCH2;
		pRMHeadAddr = RM_pnBOINCH2;
		
		for(LONG i=0;i<*RS_pnN;i++)
		{			
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].U1);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].U2);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].U3);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].U4);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].It1);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].If1);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].It2);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].If2);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].It3);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].If3);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].Iat1);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].Iat2);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].Iat);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].AIt1);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].AIf1);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].AIt2);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].AIf2);
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].AIt3);			
			WriteRMData(++pRMHeadAddr,++RMNo,HeartbeatFLData[i].AIf3);
			WriteRMData(++pRMHeadAddr,++RMNo,LOBYTE(HIWORD(HeartbeatFLData[i].GPSStatus)));			
			AbsTimeTo(&HeartbeatFLData[i].SubClock, &systime);			
			WriteRMData(++pRMHeadAddr,++RMNo, systime.Year);
			WriteRMData(++pRMHeadAddr,++RMNo, systime.Month);
			WriteRMData(++pRMHeadAddr,++RMNo, systime.Day);			
			WriteRMData(++pRMHeadAddr,++RMNo, systime.Hour);
			WriteRMData(++pRMHeadAddr,++RMNo, systime.Minute);
			WriteRMData(++pRMHeadAddr,++RMNo, LOWORD(HeartbeatFLData[i].GPSStatus));
 		}
	}	
}

void CRelayTask::InitStructCheck()
{
	WORD acc = 0x55aa;	
	StructCheck[0].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[0].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;

	for(LONG i=0;i<StructCheck[0].bigsize;i++)
	{
		acc += StructCheck[0].pstruct[i];
	}
	
	StructCheck[0].acc = acc;
	RelayCheckInitOK = TRUE;
}
/****************************************************************************************
��  ��: OnRelayChange
��  ��: ���������޸ĺ����Ӧ���������³�ʼ���������Լ�����Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::OnRelayChange(DWORD dwDataType,DWORD dwIndex)
{
	ReportMsg("OnRelayChange .... ");
	::InitAllRelayMem();
	InitRealy();
	InitStructCheck();	
	UpDateCFG();
	UpDateSW();
	InitRptSN();
	InitFLIntervalSet();
	//�����ϳɷ�ʽ�µ�CT���ϴ������������
	dwCTRateCoef1 = *AS_pnI3CTRatio*1000/(*AS_pnI1CTRatio);
	dwCTRateCoef2 = *AS_pnI3CTRatio*1000/(*AS_pnI2CTRatio);
	ReportMsg("isss: Addr=0x%x %d",(DWORD)CFG_pbyISSS,*CFG_pbyISSS);
}
/****************************************************************************************
��  ��: OnLogicChanged
��  ��: �ɱ���߼��������غ����Ӧ����
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::OnLogicChanged(void)
{
	m_bLogicChanged = TRUE;
	m_dwWaitLogicCounter = dwRelayMSCnt;
	m_bIsLogicOk &= ResetLogicParseModule(TRUE);
}
//���ݲ��ͨ�����񷢹�������Ϣ����Խ���Ƿ�ɹ��ı���
void CRelayTask::OnStepOver(WORD rc)
{
	WORD serialnum;
	if(rc == 0)
	{		
		DB_RelayStart(++g_RptSerialNum, RELAY_INF_NOUSED, NULL, FALSE);
		serialnum = g_RptSerialNum;
		MakeActRpt(R_STEPSUCC,serialnum);
		DB_RelayEnd(serialnum);	
	}
	if(rc == 1)
	{
		DB_RelayStart(++g_RptSerialNum, RELAY_INF_NOUSED, NULL, FALSE);
		serialnum = g_RptSerialNum;
		MakeActRpt(R_STEPFAIL,serialnum);
		DB_RelayEnd(serialnum);	
	}
}
void CRelayTask::InitRptSN()
{
	if(*AS_pnPM == AT1MODE)
	{
		rpt_F_TR = R_TRBREAK_F_UD;
		rpt_F_FR = R_FRBREAK_F_UD;
		rpt_B_TR = R_TRBREAK_B_UD;
		rpt_B_FR = R_FRBREAK_B_UD;
		rpt_TF = R_TFBREAK_UD;
		rpt_TimeOut = R_TIMEOUT_UD;
		rpt_ComTest = R_COMTEST_UD;
		rpt_GpsErr = R_GPSFAULT_UD;
		rpt_FLFail = R_FLFAIL_UD;
		rpt_F_TF = R_TFBREAK_F_UD;
		rpt_B_TF = R_TFBREAK_B_UD;
	}
	else
	{
		rpt_F_TR = R_TRBREAK_F;
		rpt_F_FR = R_FRBREAK_F;
		rpt_B_TR = R_TRBREAK_B;
		rpt_B_FR = R_FRBREAK_B;
		rpt_TF = R_TFBREAK;
		rpt_TimeOut = R_TIMEOUT;
		rpt_ComTest = R_COMTEST;
		rpt_GpsErr = R_GPSFAULT;
		rpt_FLFail = R_FLFAIL;
		
		rpt_F_TF = R_TFBREAK_F;
		rpt_B_TF = R_TFBREAK_B;
	}
}
/************************************************************************************************
����: ����Խ����ָ�Խ������
����: ��
����: ��
��ע:
************************************************************************************************/

void CRelayTask::HandOverProcess(void)
{	
	if(*CFG_pbyISSS == CFG_STATUS_SET)
	{
		//Խ���л�
		if((m_bStartHandOverFlag == FALSE)&&!ReadRelayWord(RW_HANDOVERMODE))
		{
			if(ReadRelayWord(RW_HANDOVER)&&!ReadRelayWord(RW_RECOVER))
			{
				OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STEPOVERCMD,0,0,0,0);
				WriteRelayWord(RW_HANDOVERMODE,RELAY_ON);
			}
		}
		m_bStartHandOverFlag = ReadRelayWord(RW_HANDOVER);
		//Խ���ָ�
		if((m_bRecoverHandOverFlag == FALSE)&&ReadRelayWord(RW_HANDOVERMODE))
		{
			if(ReadRelayWord(RW_RECOVER)&&!ReadRelayWord(RW_HANDOVER))
			{
				OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STEPOVERCMD,1,0,0,0);
				WriteRelayWord(RW_HANDOVERMODE,RELAY_OFF);
			}
		}
		m_bRecoverHandOverFlag = ReadRelayWord(RW_RECOVER);
	}
}

/************************************************************************************************
����: ������ͨ����д��¼������
����: ��
����: ��
��ע:
************************************************************************************************/

void CRelayTask::WriteDCChannel()
{
	RecordRelayWord(DCMacroArray,DCCaluResult);	
	for(LONG i =0;i<RELAY_SAMP_INTERVAL;i++)
	{
		DCChannelVal[i] = DCCaluResult[0];
		DCChannelVal[i+RELAY_SAMP_INTERVAL] = DCCaluResult[1];
	}
	WriteComposeData(DCChannelVal,dwDCDataMask, TRUE);	
}
/************************************************************************************************
����: ��ʼ������ͨ��¼�������Ϣ
����: ��
����: ��
��ע:
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
void CRelayTask::InitRel50D()
{	
	//������������Ԫ��************************************************************************************
	Rel50D1.pRelCurSet				= RS_pn50DC;                   			 //��������������ֵ 
	Rel50D1.pRelTimeSet				= RS_pnFLT;//&dwRelTime;					//��������ʱ�䶨ֵ 
	Rel50D1.pRelLockCurSet			= RS_pn50D_37C;					//��������С����������ֵ 
	Rel50D1.pRelKh1					= RS_pnKh1;						//����г������ϵ�� 
	Rel50D1.pRelKhr					= RS_pnKhr2;						//��������г����Ȩϵ�� 
	Rel50D1.pH2Coef					= CF_pnC2;
	Rel50D1.pRelCFG					= CFG_pby50DI_PI;						//������������ 
	Rel50D1.pRelSW					= SW_pbyFL;						//����������ѹ�� 
	Rel50D1.pRelElec1				= &m_I50D1_0;						//��ǰ��������ָ�� 
	Rel50D1.pRelElec2				= &m_I50D1_2;						//��ǰ����г������ָ�� 
	Rel50D1.pRelElec3				= &m_I50D1_3;						//��ǰ����г������ָ�� 
	Rel50D1.pRelElec5				= &m_I50D1_5;						//��ǰ���г������ָ�� 
	Rel50D1.pFaultSINum				= &m_dwFaultSINum;					//���ϴ���������������ָ�� 
	Rel50D1.ACTRelayWord			= RW_TR50DI_1;					//�������������̵����� 
	Rel50D1.StartRelayWord			= RW_PI50DI_1;					//����������μ̵����� 
	Rel50D1.LockRelayWord			= RW_LKPI50DI_1;					//��α����̵����� 
	Rel50D1.RSTRelayWord			= RW_LKRST50DI_1;					//���ر����̵����� 
	Rel50D1.LoopCtrlRelayWord		= RW_LPC50DI_1;				//����ѭ�����Ƽ̵����� 	
	Rel50D1.OtherRelTrip			= &m_lOtherRelayTrip;
	Rel50D1.R79ActRelayWord			= FALSE1;
	Rel50D1.H2LockRelayWord			= RW_LKH2;
	Rel50D1.RptSerialNum			= &RptSerialNumOfAct;
	Rel50D1.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel50D1.wRelRptNum				= RPT_NULL;
	Rel50D1.wPickUpReportNum		= RPT_NULL;						//��α������к�
	Rel50D1.wResetReportNum			= RPT_NULL;						//���ر������к�
	Rel50D1.dwPreI1Mod				= 0;						//����ǰ����ģֵ 
	Rel50D1.dwPreI3Mod				= 0;						//����ǰ����г��ģֵ 
	Rel50D1.dwPreI5Mod				= 0;						//����ǰ���г��ģֵ 
	Rel50D1.PreCurHaveCalcFlag		= FALSE;					//����ǰ�����Ѿ������־ 
	Rel50D1.wRelFCounter			= 0;
	Rel50D1.wRelRCounter			= 0;
	Rel50D1.wExcepWordNum			= 14;
	
	//������������Ԫ��************************************************************************************
	Rel50D2.pRelCurSet				= RS_pn50DC;                   			 //��������������ֵ 
	Rel50D2.pRelTimeSet				= RS_pnFLT;//&dwRelTime;					//��������ʱ�䶨ֵ 
	Rel50D2.pRelLockCurSet			= RS_pn50D_37C;					//��������С����������ֵ 
	Rel50D2.pRelKh1					= RS_pnKh1;						//����г������ϵ�� 
	Rel50D2.pRelKhr					= RS_pnKhr2;						//��������г����Ȩϵ�� 

	Rel50D2.pH2Coef					= CF_pnC2;
	Rel50D2.pRelCFG					= CFG_pby50DI_PI;						//������������ 
	Rel50D2.pRelSW					= SW_pbyFL;						//����������ѹ�� 


	Rel50D2.pRelElec1				= &m_I50D2_0;						//��ǰ��������ָ�� 
	Rel50D2.pRelElec2				= &m_I50D2_2;						//��ǰ����г������ָ�� 
	Rel50D2.pRelElec3				= &m_I50D2_3;						//��ǰ����г������ָ�� 
	Rel50D2.pRelElec5				= &m_I50D2_5;						//��ǰ���г������ָ�� 
	Rel50D2.pFaultSINum				= &m_dwFaultSINum;					//���ϴ���������������ָ�� 
	Rel50D2.ACTRelayWord			= RW_TR50DI_2;					//�������������̵����� 
	Rel50D2.StartRelayWord			= RW_PI50DI_2;					//����������μ̵����� 
	Rel50D2.LockRelayWord			= RW_LKPI50DI_2;					//��α����̵����� 
	Rel50D2.RSTRelayWord			= RW_LKRST50DI_2;					//���ر����̵����� 
	Rel50D2.LoopCtrlRelayWord		= RW_LPC50DI_2;				//����ѭ�����Ƽ̵����� 	
	Rel50D2.OtherRelTrip			= &m_lOtherRelayTrip;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Rel50D2.R79ActRelayWord			= FALSE1;
	Rel50D2.H2LockRelayWord			= RW_LKH2;
	Rel50D2.RptSerialNum			= &RptSerialNumOfAct;
	Rel50D2.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel50D2.wRelRptNum				= RPT_NULL;
	Rel50D2.wPickUpReportNum		= RPT_NULL;						//��α������к�
	Rel50D2.wResetReportNum			= RPT_NULL;						//���ر������к�
	Rel50D2.dwPreI1Mod				= 0;						//����ǰ����ģֵ 
	Rel50D2.dwPreI3Mod				= 0;						//����ǰ����г��ģֵ 
	Rel50D2.dwPreI5Mod				= 0;						//����ǰ���г��ģֵ 
	Rel50D2.PreCurHaveCalcFlag		= FALSE;					//����ǰ�����Ѿ������־ 
	Rel50D2.wRelFCounter			= 0;
	Rel50D2.wRelRCounter			= 0;
	Rel50D2.wExcepWordNum			= 14;
}
void CRelayTask::InitRel21()
{	
	Rel21.pRel21RSet					= RS_pn21R;					//���뱣�����趨ֵ 
	Rel21.pRel21XSet					= RS_pn21X;					//���뱣���翹��ֵ 
	Rel21.pRel21TSet    				= RS_pnFLT;//&dwRelTime;					//���뱣��ʱ�䶨ֵ 
	Rel21.pRel21Khr1Set					= RS_pnKhr1;					//���뱣��г������ϵ�� 
	Rel21.pRel21LineAngleSet			= RS_pn78LA;					//��·�迹�� 
	Rel21.pRel21AccTSet					= NULL;							//�������ٶ���ʱ�� 
	Rel21.pRel21PTBreakCurSet			= NULL;				//PT���߱���������ֵ 
	Rel21.pH2Coef						= CF_pnC2;						//����г������ϵ����г����Ȩϵ���ķŴ���
	Rel21.pRel21ZOffset					= RS_pnZOFFSET;					//ƽ���ı����迹ƫ��ֵ 
	Rel21.pRel21AOffset					= RS_pnAOFFSET;				//�迹�ı��νǶ�ƫ��
	
	Rel21.pRel21CFG						= CFG_pby21_PI;						//���뱣������ 
	Rel21.pRel21SW						= SW_pbyFL;						//���뱣����ѹ�� 
	Rel21.pRel21DirCFG					= &RelCFGSET;					//���뱣������ 
	Rel21.pRel21QuadTypeCFG				= CFG_pbyPARALLG;					//ƽ���ı��� 
	Rel21.pRel21HLockCFG				= CFG_pby21_H_L;					//���뱣��г������ 
	Rel21.pRel21HSuprCFG				= CFG_pby21_H_S;					//���뱣��г������ 
	Rel21.pRel21PTBreakCFG				= &RelCFGCLR;				//PT���߼�� 
	Rel21.pRel21ACCCFG					= &RelCFGCLR;							//����ٹ��ܿ��� 
	Rel21.pRelFLCFG						= &RelCFGCLR;				//���ϲ��Ͷ�� 
	Rel21.pSysTimer						= &dwRelayMSCnt;					//ϵͳ��ʱ��ָ�� 
	Rel21.pTangTabAddr					= TangAngleTab;						//���б��׵�ַ 

	Rel21.pRel21Cur						= &m_IP;							//����ָ�� 
	Rel21.pRel21Vol						= &m_UP;							//��ѹָ�� 
	Rel21.pRel21Impd					= &m_Impd;						//��ǰ���迹 
	Rel21.pRel21H3						= &m_IP_h3;							//����г��ָ�� 
	Rel21.pRel21H5						= &m_IP_h5;							//���г��ָ�� 

	Rel21.StartRelayWord				= RW_PI21;						//���뱣����μ̵����� 
	Rel21.ACTRelayWord					= RW_TR21;						//���뱣�������̵����� 
	Rel21.ACCRelayWord					= FALSE1;						//���������̵����� 
	Rel21.ACCActRelayWord				= FALSE1;					//����ٶ����̵����� 
	Rel21.PTBreakRelayWord				= FALSE1;					//PT���߼̵����� 
	Rel21.H2LockRelayWord				= RW_LKH2;					//����г�������̵����� 
	Rel21.LockRelayWord					= RW_LKPI21;						//��α����̵����� 
	Rel21.LoopCtrlRelayWord				= RW_LPC21;					//����ѭ�����Ƽ̵����� 
	Rel21.R37IRelayWord					= 0;						//�����̵�����

	Rel21.wRel21RptNum					= RPT_NULL;				     	//���뱣��������� 
	Rel21.wRel21AccRptNum				= RPT_NULL;					//���뱣������ٱ������ 
	Rel21.wRel21PTLockRptNum			= RPT_NULL;					//PT���߱������뱣��������� 
	
	Rel21.dwK35							= &K35;
	Rel21.RptSerialNum					= &RptSerialNumOfAct;
	//**********************��η��ر��沿��*************	yanxs  13-03-08
	Rel21.wPickUpReportNum			= RPT_NULL;						//��α������к�
	Rel21.wResetReportNum			= RPT_NULL;						//���ر������к�
	//**********************************************************************	
//	Rel21.dwRel21Timer					= 0;						//��ʱ�� 	
	Rel21.byRel21FCounter				= 0;						//���뱣�����ϼ����� 
	Rel21.byRel21RCounter				= 0;						//���뱣�����ؼ����� 
	Rel21.wExcepWordNum					= 8;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
void CRelayTask::InitRel50()
{	
	Rel50.pElecSet				= RS_pn50C;          						//��������������ֵ 
	Rel50.pTimeSet				= RS_pnFLT;				 				//��������ʱ�䶨ֵ 
	Rel50.pHarmKhr1Set			= RS_pnKhr1;							//��������г����Ȩϵ�� 
	Rel50.pVolLockSet			= RS_pn50_27V;				 	    	//����������ѹ������ֵ 
	Rel50.pAccActTimeSet		= RS_pnFLT;//&dwRelTime;						//������������ٶ���ʱ�䶨ֵ 
	Rel50.pH2Coef				= CF_pnC2;							//����г������ϵ����г����Ȩϵ���ķŴ���

	Rel50.pRelCFG				= CFG_pby50_PI;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	Rel50.pRelSW				= SW_pbyFL;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	Rel50.pRelHLockCFG			= CFG_pby50_H_L;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50.pRelHSuprCFG			= CFG_pby50_H_S; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50.pRelFDCFG				= &RelCFGSET;					//
	Rel50.pRelVolLockCFG		= CFG_pby50_27V;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	Rel50.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	Rel50.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	Rel50.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	Rel50.pRelElec 				= &m_IP;						//��ǰ����ָ�� 		
	Rel50.pRelHarm3				= &m_IP_h3;						//��ǰ����г��ָ�� 	
	Rel50.pRelHarm5				= &m_IP_h5;						//��ǰ���г��ָ�� 	
	Rel50.pRelVolt				= &m_ULock;						//��ǰ���������ĵ�ѹ��ָ�� 

	Rel50.StartRelayWord		= RW_PI50I;							//����������μ̵����� 
	Rel50.ActRelayWord			= RW_TR50I;							//�������������̵����� 
	Rel50.AccRelayWord			= FALSE1;							//���������̵�����,��һ��ʱ��̵����֣���ʱ�����Զ����� 
	Rel50.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	Rel50.PForwardRelayWord		= FALSE1;						//����������̵����� 
	Rel50.PRewardRelayWord  	= FALSE1;
	Rel50.H2LockRelayWord		= RW_LKH2;						//����г�������̵����� 
	Rel50.LockRelayWord			= RW_LKPI50I;							//��α����̵����� 
	Rel50.RSTRelayWord			= RW_LKRST50I;							//���ر����̵����� 
	Rel50.LoopCtrlRelayWord		= RW_LPC50I;						//����ѭ�����Ƽ̵����� 
	
	Rel50.wReportNum			= RPT_NULL;								//��������������� 
	Rel50.wAccReportNum			= RPT_NULL;							//���ٶ���������� 
	
	Rel50.dwK35					= &K35;
	Rel50.RptSerialNum			= &RptSerialNumOfAct;
	Rel50.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-08
	Rel50.wPickUpReportNum			= RPT_NULL;						//��α������к�
	Rel50.wResetReportNum			= RPT_NULL;						//���ر������к�
	//**********************************************************************	


//	Rel50.dwRelTimer			= 0;			 				//��ʱ�� 
	Rel50.byRelFCounter			= 0;						//���ϼ����� 
	Rel50.byRelRCounter			= 0;						//���ؼ����� 	
	Rel50.wExcepWordNum			= 8;
	Rel50.wInfo 				= 0;
}
void CRelayTask::InitSubInfo()
{
	HeartbeatFLData[0].subNo = 0;
	HeartbeatFLData[0].subTimer = dwRelayMSCnt;
	HeartbeatFLData[0].subRxdFlag = FALSE;
	HeartbeatFLData[0].subFaultFlag = FALSE;
	HeartbeatFLData[0].subFaultInf = R_CHANNEL1ERR;
	HeartbeatFLData[0].subDataInf = R_SUB1DATA;	
	HeartbeatFLData[0].subGPSInf = R_SUB1GPSERR;	
	HeartbeatFLData[0].subDataLossInf = R_SUB1LOSS;
	
	HeartbeatFLData[1].subNo = 1;
	HeartbeatFLData[1].subTimer = dwRelayMSCnt;
	HeartbeatFLData[1].subRxdFlag = FALSE;
	HeartbeatFLData[1].subFaultFlag = FALSE;
	HeartbeatFLData[1].subFaultInf = R_CHANNEL2ERR;
	HeartbeatFLData[1].subDataInf = R_SUB2DATA;
	HeartbeatFLData[1].subGPSInf = R_SUB2GPSERR;	
	HeartbeatFLData[1].subDataLossInf = R_SUB2LOSS;
	
	HeartbeatFLData[2].subNo = 2;
	HeartbeatFLData[2].subTimer = dwRelayMSCnt;
	HeartbeatFLData[2].subRxdFlag = FALSE;
	HeartbeatFLData[2].subFaultFlag = FALSE;
	HeartbeatFLData[2].subFaultInf = R_CHANNEL3ERR;
	HeartbeatFLData[2].subDataInf = R_SUB3DATA;
	HeartbeatFLData[2].subGPSInf = R_SUB3GPSERR;	
	HeartbeatFLData[2].subDataLossInf = R_SUB3LOSS;

	HeartbeatFLData[3].subNo = 3;
	HeartbeatFLData[3].subTimer = dwRelayMSCnt;
	HeartbeatFLData[3].subRxdFlag = FALSE;
	HeartbeatFLData[3].subFaultFlag = FALSE;
	HeartbeatFLData[3].subFaultInf = R_CHANNEL4ERR;
	HeartbeatFLData[3].subDataInf = R_SUB4DATA;
	HeartbeatFLData[3].subGPSInf = R_SUB4GPSERR;
	HeartbeatFLData[3].subDataLossInf = R_SUB4LOSS;
	
	HeartbeatFLData[4].subNo = 4;
	HeartbeatFLData[4].subTimer = dwRelayMSCnt;
	HeartbeatFLData[4].subRxdFlag = FALSE;
	HeartbeatFLData[4].subFaultFlag = FALSE;
	HeartbeatFLData[4].subFaultInf = R_CHANNEL5ERR;
	HeartbeatFLData[4].subDataInf = R_SUB5DATA;
	HeartbeatFLData[4].subGPSInf = R_SUB5GPSERR;
	HeartbeatFLData[4].subDataLossInf = R_SUB5LOSS;

	ManualFLData[0].subNo = 0;
	ManualFLData[0].subTimer = 0;
	ManualFLData[0].subRxdFlag = FALSE;
	ManualFLData[0].subFaultFlag = FALSE;
	ManualFLData[0].subFaultInf = R_CHANNEL1ERR;
	ManualFLData[0].subDataInf = R_SUB1DATA;
	ManualFLData[0].subGPSInf = R_SUB1GPSERR;	
	ManualFLData[0].subDataLossInf = R_SUB1LOSS;
	
	ManualFLData[1].subNo = 1;
	ManualFLData[1].subTimer = 0;
	ManualFLData[1].subRxdFlag = FALSE;
	ManualFLData[1].subFaultFlag = FALSE;
	ManualFLData[1].subFaultInf = R_CHANNEL2ERR;
	ManualFLData[1].subDataInf = R_SUB2DATA;
	ManualFLData[1].subGPSInf = R_SUB2GPSERR;	
	ManualFLData[1].subDataLossInf = R_SUB2LOSS;
	
	ManualFLData[2].subNo = 2;
	ManualFLData[2].subTimer = 0;
	ManualFLData[2].subRxdFlag = FALSE;
	ManualFLData[2].subFaultFlag = FALSE;
	ManualFLData[2].subFaultInf = R_CHANNEL3ERR;
	ManualFLData[2].subDataInf = R_SUB3DATA;
	ManualFLData[2].subGPSInf = R_SUB3GPSERR;	
	ManualFLData[2].subDataLossInf = R_SUB3LOSS;

	ManualFLData[3].subNo = 3;
	ManualFLData[3].subTimer = 0;
	ManualFLData[3].subRxdFlag = FALSE;
	ManualFLData[3].subFaultFlag = FALSE;
	ManualFLData[3].subFaultInf = R_CHANNEL4ERR;
	ManualFLData[3].subDataInf = R_SUB4DATA;
	ManualFLData[3].subGPSInf = R_SUB4GPSERR;	
	ManualFLData[3].subDataLossInf = R_SUB4LOSS;

	
	ManualFLData[4].subNo = 4;
	ManualFLData[4].subTimer = 0;
	ManualFLData[4].subRxdFlag = FALSE;
	ManualFLData[4].subFaultFlag = FALSE;
	ManualFLData[4].subFaultInf = R_CHANNEL5ERR;
	ManualFLData[4].subDataInf = R_SUB5DATA;
	ManualFLData[4].subGPSInf = R_SUB5GPSERR;	
	ManualFLData[4].subDataLossInf = R_SUB5LOSS;

	for(WORD i=0;i<FLCOMPLICATENUM;i++)
	{
		RelayFLData[i][0].subNo = 0;
		RelayFLData[i][0].subTimer = 0;
		RelayFLData[i][0].subRxdFlag = FALSE;
		RelayFLData[i][0].subFaultFlag = FALSE;
		RelayFLData[i][0].subFaultInf = R_CHANNEL1ERR;
		RelayFLData[i][0].subDataInf = R_SUB1DATA;	
		RelayFLData[i][0].subGPSInf = R_SUB1GPSERR;
		RelayFLData[i][0].subDataLossInf = R_SUB1LOSS;
	
		RelayFLData[i][1].subNo = 1;
		RelayFLData[i][1].subTimer = 0;
		RelayFLData[i][1].subRxdFlag = FALSE;
		RelayFLData[i][1].subFaultFlag = FALSE;
		RelayFLData[i][1].subFaultInf = R_CHANNEL2ERR;
		RelayFLData[i][1].subDataInf = R_SUB2DATA;	
		RelayFLData[i][1].subGPSInf = R_SUB2GPSERR;
		RelayFLData[i][1].subDataLossInf = R_SUB2LOSS;

		RelayFLData[i][2].subNo = 2;
		RelayFLData[i][2].subTimer = 0;
		RelayFLData[i][2].subRxdFlag = FALSE;
		RelayFLData[i][2].subFaultFlag = FALSE;
		RelayFLData[i][2].subFaultInf = R_CHANNEL3ERR;
		RelayFLData[i][2].subDataInf = R_SUB3DATA;	
		RelayFLData[i][2].subGPSInf = R_SUB3GPSERR;
		RelayFLData[i][2].subDataLossInf = R_SUB3LOSS;

		RelayFLData[i][3].subNo = 3;
		RelayFLData[i][3].subTimer = 0;
		RelayFLData[i][3].subRxdFlag = FALSE;
		RelayFLData[i][3].subFaultFlag = FALSE;
		RelayFLData[i][3].subFaultInf = R_CHANNEL4ERR;
		RelayFLData[i][3].subDataInf = R_SUB4DATA;
		RelayFLData[i][3].subGPSInf = R_SUB4GPSERR;
		RelayFLData[i][3].subDataLossInf = R_SUB4LOSS;

		
		RelayFLData[i][4].subNo = 4;
		RelayFLData[i][4].subTimer = 0;
		RelayFLData[i][4].subRxdFlag = FALSE;
		RelayFLData[i][4].subFaultFlag = FALSE;
		RelayFLData[i][4].subFaultInf = R_CHANNEL5ERR;
		RelayFLData[i][4].subDataInf = R_SUB5DATA;
		RelayFLData[i][4].subGPSInf = R_SUB5GPSERR;
		RelayFLData[i][4].subDataLossInf = R_SUB5LOSS;
	}
	
	ForeFLData[0].subNo = 0;
	ForeFLData[0].subTimer = 0;
	ForeFLData[0].subRxdFlag = FALSE;
	ForeFLData[0].subFaultFlag = FALSE;
	ForeFLData[0].subFaultInf = R_CHANNEL1ERR;
	ForeFLData[0].subDataInf = R_SUB1DATA;	
	ForeFLData[0].subGPSInf = R_SUB1GPSERR;	
	ForeFLData[0].subDataLossInf = R_SUB1LOSS;
	
	ForeFLData[1].subNo = 1;
	ForeFLData[1].subTimer = 0;
	ForeFLData[1].subRxdFlag = FALSE;
	ForeFLData[1].subFaultFlag = FALSE;
	ForeFLData[1].subFaultInf = R_CHANNEL2ERR;
	ForeFLData[1].subDataInf = R_SUB2DATA;	
	ForeFLData[1].subGPSInf = R_SUB2GPSERR;	
	ForeFLData[1].subDataLossInf = R_SUB2LOSS;
	
	ForeFLData[2].subNo = 2;
	ForeFLData[2].subTimer = 0;
	ForeFLData[2].subRxdFlag = FALSE;
	ForeFLData[2].subFaultFlag = FALSE;
	ForeFLData[2].subFaultInf = R_CHANNEL3ERR;
	ForeFLData[2].subDataInf = R_SUB3DATA;	
	ForeFLData[2].subGPSInf = R_SUB3GPSERR;
	ForeFLData[2].subDataLossInf = R_SUB3LOSS;
	
	ForeFLData[3].subNo = 3;
	ForeFLData[3].subTimer = 0;
	ForeFLData[3].subRxdFlag = FALSE;
	ForeFLData[3].subFaultFlag = FALSE;
	ForeFLData[3].subFaultInf = R_CHANNEL4ERR;
	ForeFLData[3].subDataInf = R_SUB4DATA;
	ForeFLData[3].subGPSInf = R_SUB4GPSERR;
	ForeFLData[3].subDataLossInf = R_SUB4LOSS;

	ForeFLData[4].subNo = 4;
	ForeFLData[4].subTimer = 0;
	ForeFLData[4].subRxdFlag = FALSE;
	ForeFLData[4].subFaultFlag = FALSE;
	ForeFLData[4].subFaultInf = R_CHANNEL5ERR;
	ForeFLData[4].subDataInf = R_SUB5DATA;
	ForeFLData[4].subGPSInf = R_SUB5GPSERR;
	ForeFLData[4].subDataLossInf = R_SUB5LOSS;

	LowValFLData[0].subNo = 0;
	LowValFLData[0].subTimer = 0;
	LowValFLData[0].subRxdFlag = FALSE;
	LowValFLData[0].subFaultFlag = FALSE;
	LowValFLData[0].subFaultInf = R_CHANNEL1ERR;
	LowValFLData[0].subDataInf = R_SUB1DATA;	
	LowValFLData[0].subGPSInf = R_SUB1GPSERR;
	LowValFLData[0].subDataLossInf = R_SUB1LOSS;
	
	LowValFLData[1].subNo = 1;
	LowValFLData[1].subTimer = 0;
	LowValFLData[1].subRxdFlag = FALSE;
	LowValFLData[1].subFaultFlag = FALSE;
	LowValFLData[1].subFaultInf = R_CHANNEL2ERR;
	LowValFLData[1].subDataInf = R_SUB2DATA;	
	LowValFLData[1].subGPSInf = R_SUB2GPSERR;
	LowValFLData[1].subDataLossInf = R_SUB2LOSS;
	
	LowValFLData[2].subNo = 2;
	LowValFLData[2].subTimer = 0;
	LowValFLData[2].subRxdFlag = FALSE;
	LowValFLData[2].subFaultFlag = FALSE;
	LowValFLData[2].subFaultInf = R_CHANNEL3ERR;
	LowValFLData[2].subDataInf = R_SUB3DATA;	
	LowValFLData[2].subGPSInf = R_SUB3GPSERR;
	LowValFLData[2].subDataLossInf = R_SUB3LOSS;

	LowValFLData[3].subNo = 3;
	LowValFLData[3].subTimer = 0;
	LowValFLData[3].subRxdFlag = FALSE;
	LowValFLData[3].subFaultFlag = FALSE;
	LowValFLData[3].subFaultInf = R_CHANNEL4ERR;
	LowValFLData[3].subDataInf = R_SUB4DATA;
	LowValFLData[3].subGPSInf = R_SUB4GPSERR;
	LowValFLData[3].subDataLossInf = R_SUB4LOSS;
	
	LowValFLData[4].subNo = 4;
	LowValFLData[4].subTimer = 0;
	LowValFLData[4].subRxdFlag = FALSE;
	LowValFLData[4].subFaultFlag = FALSE;
	LowValFLData[4].subFaultInf = R_CHANNEL5ERR;
	LowValFLData[4].subDataInf = R_SUB5DATA;
	LowValFLData[4].subGPSInf = R_SUB5GPSERR;
	LowValFLData[4].subDataLossInf = R_SUB5LOSS;
}
void CRelayTask::InitFaultLocate()
{
	T_FaultLocate.pRel21FLCFG				= &RelCFGSET;						//���ϲ�������� 
	T_FaultLocate.pRel21FLNSet				= RS_pnT21FLN;                             	       //���ֶ�����ֵ 
	T_FaultLocate.pRel21FL1LSet				= RS_pnT21FLN1L; 						//��1�ξ��붨ֵ 
	T_FaultLocate.pRel21FL1XSet				= RS_pnT21FLN1X;						//��1�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL2LSet				= RS_pnT21FLN2L; 						//��2�ξ��붨ֵ 
	T_FaultLocate.pRel21FL2XSet				= RS_pnT21FLN2X;  					//��2�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL3LSet				= RS_pnT21FLN3L ;  						//��3�ξ��붨ֵ 
	T_FaultLocate.pRel21FL3XSet				= RS_pnT21FLN3X; 						//��3�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL4LSet				= RS_pnT21FLN4L ;  						//��4�ξ��붨ֵ 
	T_FaultLocate.pRel21FL4XSet				= RS_pnT21FLN4X; 						//��4�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL5LSet				= RS_pnT21FLN5L;  						//��5�ξ��붨ֵ 
	T_FaultLocate.pRel21FL5XSet				= RS_pnT21FLN5X; 						//��5�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL6LSet				= RS_pnT21FLN6L; 						//��6�ξ��붨ֵ 
	T_FaultLocate.pRel21FL6XSet				= RS_pnT21FLN6X; 						//��6�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL7LSet				= NULL;						//��7�ξ��붨ֵ 
	T_FaultLocate.pRel21FL7XSet				= NULL; 						//��7�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL8LSet				= NULL; 						//��8�ξ��붨ֵ 
	T_FaultLocate.pRel21FL8XSet				= NULL;						//��8�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL9LSet				= NULL; 						//��9�ξ��붨ֵ 
	T_FaultLocate.pRel21FL9XSet				= NULL;						//��9�ε�λ�翹��ֵ 
	T_FaultLocate.pRel21FL10LSet			= NULL; 					//��10�ξ��붨ֵ 
	T_FaultLocate.pRel21FL10XSet			= NULL;					//��10�ξ��붨ֵ 
    T_FaultLocate.pFaultValueL				= RM_pnDist;						//���ϵ���L��ָ�� 	  
	T_FaultLocate.pnLCoef					= CF_pnC2;
	T_FaultLocate.FaultValueNum				= RM_DIST;
	T_FaultLocate.pRel21FLImpd				= &m_LImpd_T;						//��ǰ�迹��ָ�� 
	T_FaultLocate.wExcepWordNum				= 0;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ0 	
	CaluFaultLocateSetTable(&T_FaultLocate);  								//F�߹��ϲ�ඨָ�����

	F_FaultLocate.pRel21FLCFG				= &RelCFGSET; 					//���ϲ�������� 
	F_FaultLocate.pRel21FLNSet				= RS_pnF21FLN;									   //���ֶ�����ֵ 
	F_FaultLocate.pRel21FL1LSet 			= RS_pnF21FLN1L;						//��1�ξ��붨ֵ 
	F_FaultLocate.pRel21FL1XSet 			= RS_pnF21FLN1X;						//��1�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL2LSet 			= RS_pnF21FLN2L;						//��2�ξ��붨ֵ 
	F_FaultLocate.pRel21FL2XSet 			= RS_pnF21FLN2X;					//��2�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL3LSet 			= RS_pnF21FLN3L ;						//��3�ξ��붨ֵ 
	F_FaultLocate.pRel21FL3XSet 			= RS_pnF21FLN3X;						//��3�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL4LSet 			= RS_pnF21FLN4L ;						//��4�ξ��붨ֵ 
	F_FaultLocate.pRel21FL4XSet 			= RS_pnF21FLN4X;						//��4�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL5LSet 			= RS_pnF21FLN5L;						//��5�ξ��붨ֵ 
	F_FaultLocate.pRel21FL5XSet 			= RS_pnF21FLN5X;						//��5�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL6LSet 			= RS_pnF21FLN6L;						//��6�ξ��붨ֵ 
	F_FaultLocate.pRel21FL6XSet 			= RS_pnF21FLN6X;						//��6�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL7LSet 			= NULL; 					//��7�ξ��붨ֵ 
	F_FaultLocate.pRel21FL7XSet 			= NULL; 						//��7�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL8LSet 			= NULL; 						//��8�ξ��붨ֵ 
	F_FaultLocate.pRel21FL8XSet 			= NULL; 					//��8�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL9LSet 			= NULL; 						//��9�ξ��붨ֵ 
	F_FaultLocate.pRel21FL9XSet 			= NULL; 					//��9�ε�λ�翹��ֵ 
	F_FaultLocate.pRel21FL10LSet			= NULL; 					//��10�ξ��붨ֵ 
	F_FaultLocate.pRel21FL10XSet			= NULL; 				//��10�ξ��붨ֵ 
	F_FaultLocate.pFaultValueL				= RM_pnDist;						//���ϵ���L��ָ��	  
	F_FaultLocate.pnLCoef					= CF_pnC2;
	F_FaultLocate.FaultValueNum 			= RM_DIST;
	F_FaultLocate.pRel21FLImpd				= &m_LImpd_F;						//��ǰ�迹��ָ�� 
	F_FaultLocate.wExcepWordNum 			= 0;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ0	
	CaluFaultLocateSetTable(&F_FaultLocate);								//F�߹��ϲ�ඨָ�����
	
	TF_FaultLocate.pRel21FLCFG				= &RelCFGSET; 					//���ϲ�������� 
	TF_FaultLocate.pRel21FLNSet				= RS_pnTF21FLN;									   //���ֶ�����ֵ 
	TF_FaultLocate.pRel21FL1LSet 			= RS_pnTF21FLN1L;						//��1�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL1XSet 			= RS_pnTF21FLN1X;						//��1�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL2LSet 			= RS_pnTF21FLN2L;						//��2�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL2XSet 			= RS_pnTF21FLN2X;					//��2�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL3LSet 			= RS_pnTF21FLN3L ;						//��3�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL3XSet 			= RS_pnTF21FLN3X;						//��3�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL4LSet 			= RS_pnTF21FLN4L ;						//��4�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL4XSet 			= RS_pnTF21FLN4X;						//��4�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL5LSet 			= RS_pnTF21FLN5L;						//��5�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL5XSet 			= RS_pnTF21FLN5X;						//��5�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL6LSet 			= RS_pnTF21FLN6L;						//��6�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL6XSet 			= RS_pnTF21FLN6X;						//��6�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL7LSet 			= NULL; 					//��7�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL7XSet 			= NULL; 						//��7�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL8LSet 			= NULL; 						//��8�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL8XSet 			= NULL; 					//��8�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL9LSet 			= NULL; 						//��9�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL9XSet 			= NULL; 					//��9�ε�λ�翹��ֵ 
	TF_FaultLocate.pRel21FL10LSet			= NULL; 					//��10�ξ��붨ֵ 
	TF_FaultLocate.pRel21FL10XSet			= NULL; 				//��10�ξ��붨ֵ 
	TF_FaultLocate.pFaultValueL				= RM_pnDist;						//���ϵ���L��ָ��	  
	TF_FaultLocate.pnLCoef					= CF_pnC2;
	TF_FaultLocate.FaultValueNum 			= RM_DIST;
	TF_FaultLocate.pRel21FLImpd				= &m_LImpd_TF;						//��ǰ�迹��ָ�� 
	TF_FaultLocate.wExcepWordNum 			= 0;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ0	
	CaluFaultLocateSetTable(&TF_FaultLocate);								//F�߹��ϲ�ඨָ�����
}
void CRelayTask::CaluFaultLocateSetTable(TRelFaultLocatePar* Relay )
{	
	if ((*Relay->pRel21FLCFG) == CFG_STATUS_CLR)
	{
		return;				//δͶ�룬�˳� 
	}
	//��һ�ζ�ֵ���� 
	Relay->SetTab[0]  = 0;
	Relay->SetTab[1]  = *(Relay->pRel21FL1XSet);
	Relay->SetTab[2]  = (*(Relay->pRel21FL1LSet)) * (*(Relay->pRel21FL1XSet));
	if (*(Relay->pRel21FLNSet) < 2)
		return;
	//�ڶ��ζ�ֵ���� 
	Relay->SetTab[3]  = Relay->SetTab[0] + (*(Relay->pRel21FL1LSet));
	Relay->SetTab[4]  = *(Relay->pRel21FL2XSet);
	Relay->SetTab[5]  = Relay->SetTab[2] + (*(Relay->pRel21FL2XSet)*(*(Relay->pRel21FL2LSet)));
	if (*(Relay->pRel21FLNSet) < 3)
		return;
	//�����ζ�ֵ���� 
	Relay->SetTab[6]  = Relay->SetTab[3] + (*(Relay->pRel21FL2LSet));
	Relay->SetTab[7]  = *(Relay->pRel21FL3XSet);
	Relay->SetTab[8]  = Relay->SetTab[5] + (*(Relay->pRel21FL3XSet)*(*(Relay->pRel21FL3LSet)));
	if (*(Relay->pRel21FLNSet) < 4)
		return;
	//���Ķζ�ֵ���� 
	Relay->SetTab[9]  = Relay->SetTab[6] + (*(Relay->pRel21FL3LSet));
	Relay->SetTab[10] = *(Relay->pRel21FL4XSet);
	Relay->SetTab[11] = Relay->SetTab[8] + (*(Relay->pRel21FL4XSet)*(*(Relay->pRel21FL4LSet)));
	if (*(Relay->pRel21FLNSet) < 5)
		return;
	//����ζ�ֵ���� 
	Relay->SetTab[12] = Relay->SetTab[9] + (*(Relay->pRel21FL4LSet));
	Relay->SetTab[13] = *(Relay->pRel21FL5XSet);
	Relay->SetTab[14] = Relay->SetTab[11] + (*(Relay->pRel21FL5XSet)*(*(Relay->pRel21FL5LSet)));
	if (*(Relay->pRel21FLNSet) < 6)
		return;
	//�����ζ�ֵ���� 
	Relay->SetTab[15] = Relay->SetTab[12] + (*(Relay->pRel21FL5LSet));
	Relay->SetTab[16] = *(Relay->pRel21FL6XSet);
	Relay->SetTab[17] = Relay->SetTab[14] + (*(Relay->pRel21FL6XSet)*(*(Relay->pRel21FL6LSet)));
	if (*(Relay->pRel21FLNSet) < 7)
		return;
	//���߶ζ�ֵ���� 
	Relay->SetTab[18] = Relay->SetTab[15] + (*(Relay->pRel21FL6LSet));
	Relay->SetTab[19] = *(Relay->pRel21FL7XSet);
	Relay->SetTab[20] = Relay->SetTab[17] + (*(Relay->pRel21FL7XSet)*(*(Relay->pRel21FL7LSet)));
	if (*(Relay->pRel21FLNSet) < 8)
		return;
	//�ڰ˶ζ�ֵ���� 
	Relay->SetTab[21] = Relay->SetTab[18] + (*(Relay->pRel21FL7LSet));
	Relay->SetTab[22] = *(Relay->pRel21FL8XSet);
	Relay->SetTab[23] = Relay->SetTab[20] + (*(Relay->pRel21FL8XSet)*(*(Relay->pRel21FL8LSet)));
	if (*(Relay->pRel21FLNSet) < 9)
		return;
	//�ھŶζ�ֵ���� 
	Relay->SetTab[24] = Relay->SetTab[21] + (*(Relay->pRel21FL8LSet));
	Relay->SetTab[25] = *(Relay->pRel21FL9XSet);
	Relay->SetTab[26] = Relay->SetTab[23] + (*(Relay->pRel21FL9XSet)*(*(Relay->pRel21FL9LSet)));
	if (*(Relay->pRel21FLNSet) < 10)
		return;
	//��ʮ�ζ�ֵ���� 
	Relay->SetTab[27] = Relay->SetTab[24] + (*(Relay->pRel21FL9LSet));
	Relay->SetTab[28] = *(Relay->pRel21FL10XSet);
	Relay->SetTab[29] = Relay->SetTab[26] + (*(Relay->pRel21FL10XSet)*(*(Relay->pRel21FL10LSet)));
	if (*(Relay->pRel21FLNSet) > 10)
		return;
}
/***********************************************************************************

���Ե翹�����,�ٲ��������ݺ��ж��Ƿ�����翹�����
1.���ҳ��������е�������
2.�ж���T�ߵ�������F�ߵ�����(ͬһ����)
3.������AT���ϵ���Ϊ0(�о�ΪС��30A)
4.���������С������50������С����С��30A��������Ϊ���������һ����·��������
5.�����жϽ��е翹�����
***********************************************************************************/
BOOL CRelayTask::XLineWithSub(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	if(*AS_pnPM == AT1MODE) //��ͨAT���緽ʽ�£������е翹�������ж�
		return FALSE;
 
	LONG I1,I2,I11,I21;
	LONG It1,If1,It2,If2;
 	DWORD inf;

//	if((exchangeElecFlag == I3I2I1)||(exchangeElecFlag == I2I3I1))
//	{
//		It1 = lkSSData[no].subData.IT3;
//		If1 = lkSSData[no].subData.IF3;
//	}
//	else
//	{
//		It1 = lkSSData[no].subData.IT1;
//		If1 = lkSSData[no].subData.IF1;
//	}
//	if((exchangeElecFlag == I3I1I2)||(exchangeElecFlag == I1I3I2))
//	{
// 		It2 = lkSSData[no].subData.IT3;
//		If2 = lkSSData[no].subData.IF3;
//	}
//	else
//	{
// 		It2 = lkSSData[no].subData.IT2;
//		If2 = lkSSData[no].subData.IF2;
//	}

	It1 = tsubData[0].IT1;
	If1 = tsubData[0].IF1;
	It2 = tsubData[0].IT2;
	If2 = tsubData[0].IF2;

 	//�����ϴ����������ң�AT����Ϊ��  �����ϴ�����Ϊһ��ֵ 
  	for(WORD i=0;i<*RS_pnN;i++)
	{
 		if(tsubData[i].IAT>30) 
 			return FALSE;
 	}
	I1 = MAX2(It1,If1);
	I11 = MIN2(It1,If1);
	I2 = MAX2(It2,If2);
	I21 = MIN2(It2,If2);
	//������������
	if(I1>=I2)
	{
		if((I1>50*I11)&&(I11<30))
		{	
			if(I1 == It1)
			{				
				ReportMsg("X %d fault locate = %d",no,dwRelayMSCnt);
				m_LImpd_T = lkSSData[no].lockImpd_T;
				CalcFLX(&T_FaultLocate);				
				faultINF = R_TR_B_X;
			}
			else
			{				
				ReportMsg("X fault locate = %d",dwRelayMSCnt);
				m_LImpd_F = lkSSData[no].lockImpd_F;
				CalcFLX(&F_FaultLocate);				
				faultINF = R_FR_B_X;
			}				
 			return TRUE;
		}
		return FALSE;
	}
	//������������
	else 
	{
		if((I2>50*I21)&&(I21<30*(*CF_pnIT1)))
		{
			if(I2 == It2)
			{				
				ReportMsg("X fault locate = %d",dwRelayMSCnt);
				m_LImpd_T = lkSSData[no].lockImpd_T;
				CalcFLX(&T_FaultLocate);					
				faultINF = R_TR_F_X;
			}
			else
			{				
				ReportMsg("X fault locate = %d",dwRelayMSCnt);
				m_LImpd_F = lkSSData[no].lockImpd_F;
				CalcFLX(&F_FaultLocate);					
				faultINF = R_FR_F_X;
			}
 			return TRUE;
		}		
		return FALSE;
	}
}
void CRelayTask::CalcFLX(TRelFaultLocatePar *Relay)
{
	WORD num = 0;
	DWORD React = ToAbs(Relay->pRel21FLImpd->Imag)*(*Relay->pnLCoef);

	if ((*(Relay->pRel21FLNSet)>MAXFLNUM) || (*(Relay->pRel21FLNSet)<=0))
	{
		*Relay->pFaultValueL= 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	for (;num<(*(Relay->pRel21FLNSet));num++)	  //�жϹ��ϵ������ĸ����� 
	{
		if (React<=Relay->SetTab[num*3+2])
			break;		
	}
	if ((num == (*(Relay->pRel21FLNSet))||(Relay->SetTab[num*3+1] == 0)))  //�翹�������ֵ�����ߵ�λ�迹Ϊ0 
	{
		*(Relay->pFaultValueL) = 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	if (num == 0)  //���ϵ��ڵ�һ����	
	{
		*(Relay->pFaultValueL) = React/Relay->SetTab[1];
		DB_LogWriteRM(Relay->FaultValueNum, *(Relay->pFaultValueL) );
		return;
	}
	
	*(Relay->pFaultValueL) = Relay->SetTab[num*3] + (React - Relay->SetTab[num*3-1])/Relay->SetTab[num*3+1];
	
	DB_LogWriteRM(Relay->FaultValueNum, *(Relay->pFaultValueL) );
}

/******************************************************************************************************
*	ͨ��ͨ�����
*	30��֮����ͨѶ��Ӧ��Ϊͨ������
*******************************************************************************************************/
void CRelayTask::ChannelCheck()
{
	//���ѹ�弰����
	if((*SW_pbyFL == SW_STATUS_CUT)||(*CFG_pbyISSS == CFG_STATUS_CLR)||(*CFG_pbyCHECKCOM == CFG_STATUS_CLR ))
	{
		for(LONG n=0;n<MAX_SUBSTATION_NUM;n++)
			WriteRelayWord(ChanErrRW[n],RELAY_OFF);	
		return; //��ͨ��״̬�̵�����
	}
	if((*RS_pnN <= 0)||(*RS_pnN>5)) //����������ֵС�ڵ���0���ߴ���3
		return;
	for(LONG i=0;i<*RS_pnN;i++)
	{
		if(HeartbeatFLData[i].subRxdFlag == TRUE)
		{
			HeartbeatFLData[i].subTimer = dwRelayMSCnt;
			HeartbeatFLData[i].subRxdFlag = FALSE;
			HeartbeatFLData[i].subFaultFlag = FALSE;
			WriteRelayWord(ChanErrRW[i],RELAY_OFF);			
		}
		else
		{
			if(HeartbeatFLData[i].subFaultFlag == FALSE) //������ͨ���Ѿ�����
			{
				if(dwRelayMSCnt - HeartbeatFLData[i].subTimer>60000) //60������Чͨ�ţ������ϱ��棬
				{
					TAbsTime time;
					ReadAbsTime(&time);
					//������ͨ�����ϱ��棬�ñ����޹��ϵ���
					DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&time,FALSE);
					channelCheckRptSerNo = g_RptSerialNum;
					MakeActRpt(HeartbeatFLData[i].subFaultInf,channelCheckRptSerNo);
					DB_RelayEnd(channelCheckRptSerNo); 
					WriteRelayWord(ChanErrRW[i],RELAY_ON);
					HeartbeatFLData[i].subFaultFlag = TRUE;					
				}
			}
		}
	}
}
/****************************************************************************************
��  ��: PickUpCheck
��  ��: �����Ĺ���������ͻ���������ж�
��  ��: ��
����ֵ: TRUE,����;FALSE,δ����
*****************************************************************************************/

BOOL CRelayTask::RelayPickUpCheck()
{	
	if((*Rel50.pRelCFG == CFG_STATUS_SET)&&(*Rel50.pRelSW == SW_STATUS_SET))
	{
		if(Rel50.pRelElec->Mod > *Rel50.pElecSet)
		{
			WriteRelayWord(RW_RELPICKUP,RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_FLPICKUP].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			ReportMsg("relay pickup = %d",dwRelayMSCnt);
			return TRUE;
		}
	}
	//ͻ������������
	BYTE  pChannel[]={CHANNEL_6571_IP1,CHANNEL_6571_IP2,CHANNEL_6571_IP3};
	if((*Rel50.pRelCFG == CFG_STATUS_SET)&&(*Rel50.pRelSW == SW_STATUS_SET) || \
	   (*Rel50D1.pRelCFG == CFG_STATUS_SET)&&(*Rel50D1.pRelSW == SW_STATUS_SET) || \
	   (*Rel21.pRel21CFG == CFG_STATUS_SET)&&(*Rel21.pRel21SW == SW_STATUS_SET))
	{
		ReadPreSingleData(dwPickUpBuf,pChannel,3,RELAY_SAMP_INTERVAL);

		//�㷨1�����ݺ�RELAY_SAMP_INTERVALȷ����ͨ���㷨
		//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
		//ע��:RELAY_SAMP_INTERVAL��ֵ����С��3
		for(LONG j=0;j<3;j++)
		{
			m_wPickCheck = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1 = dwPickUpBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickUpBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickUpBuf[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));
				
				//���ø����ż�ֵ�жϣ���I>0.25*��I'+ C,���Ц�I'Ϊ�ϸ�һ���ڽ��ĵ��ͻ����ֵ��CΪͻ����������ֵ
				if(lPickValue*4>*RS_pnPIC*4+m_lPickUpValueArray[j])
				{
					m_wPickCheck++;
					if(m_wPickCheck ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime.Lo-(i*1000+500)/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_RELPICKUP, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_FLPICKUP].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
						RptSerialNumOfStart = g_RptSerialNum;
						
						ReportMsg("relay pickup = %d",dwRelayMSCnt);
						m_wPickCheck =0;
						for(LONG m=0;m<3;m++)
						{
							m_wPickCheckNext[m] =0;
							m_lPickUpValueArray[m]=0;
						}
						
						return TRUE;	
					}
					if(i<2)
					{
						m_wPickCheckNext[j]++;
						if(m_wPickCheckNext[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime);
							PickTime = PickUpTime.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime.Lo-(i*1000+500)/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_RELPICKUP, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_FLPICKUP].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
							RptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
							RptSerialNumOfStart = g_RptSerialNum;
							
							ReportMsg("relay pickup = %d",dwRelayMSCnt);
							m_wPickCheck =0;
							for(LONG m=0;m<3;m++)
							{
								m_wPickCheckNext[m] =0;
								m_lPickUpValueArray[m]=0;
							}
							
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						m_wPickCheckNext[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						m_wPickCheckNext[j]++;
				}
				else
				{
					m_wPickCheck =0;
					m_wPickCheckNext[j]=0;
				}
				m_lPickUpValueArray[j] = lPickValue;
			}
		}
	}
	WriteRelayWord(RW_RELPICKUP, RELAY_OFF);
	return FALSE;
}
/****************************************************************************************
��  ��: FaultLocate
��  ��: ���������
��  ��: ��
����ֵ: 
*****************************************************************************************/
void CRelayTask::FaultLocate()
{	
	//��鱾����GPS�Ƿ�����
	checkSelfGps();
	// �����Ǳ����
	if(*CFG_pbyISSS == CFG_STATUS_SET) 
	{	
		//������ͨ�����
		ChannelCheck();
		//��鱣������Ƿ�����
		FaultLocateAuto();
		//����ֶ�����Ƿ�����
		FaultLocateManual();		
	}
	else
		//����Ϊ���������������ݻ�������ˢ������
		RefreshHeartData();
}
/****************************************************************************************
��  ��: RefreshHeartData
��  ��: ���������ݻ�������д��ʵʱ���ݣ�ͨѶ����ֱ�ӴӸû�������ȡ���ϴ�
��  ��: ��
����ֵ: 
*****************************************************************************************/
void CRelayTask::RefreshHeartData()
{
	HeartbeatData.subNo = *RS_pnNID;
	HeartbeatData.GPSStatus = GetGPSStatus(ERR_SUCCESS,m_CB);	//GPSʱ�������������������
	HeartbeatData.U1 = m_U1mod/(*CF_pnU1);
	HeartbeatData.U2 = m_U2mod/(*CF_pnU2);
	HeartbeatData.U3 = m_U3mod/(*CF_pnU3);
	HeartbeatData.U4 = m_U4mod/(*CF_pnU4);
	HeartbeatData.It1 = m_IT1mod/(*CF_pnIT1);
	HeartbeatData.If1 = m_IF1mod/(*CF_pnIF1);
	HeartbeatData.It2 = m_IT2mod/(*CF_pnIT2);
	HeartbeatData.If2 = m_IF2mod/(*CF_pnIF2);
	HeartbeatData.It3 = m_IT3mod/(*CF_pnIT3);
	HeartbeatData.If3 = m_IF3mod/(*CF_pnIF3);
	HeartbeatData.Iat1 = m_IAT1mod/(*CF_pnIAT1);
	HeartbeatData.Iat2 = m_IAT2mod/(*CF_pnIAT2);
	HeartbeatData.Iat = m_IAT_SS/(*CF_pnIAT2);
	HeartbeatData.AIt1 = m_IT1.Angle;
	HeartbeatData.AIf1 = m_IF1.Angle;
	HeartbeatData.AIt2 = m_IT2.Angle;
	HeartbeatData.AIf2 = m_IF2.Angle;
	HeartbeatData.AIt3 = m_IT3.Angle;
	HeartbeatData.AIf3 = m_IF3.Angle;
	HeartbeatData.SubClock = FLPickTime;
}

void CRelayTask::FLPickUpCheck()
{
	WORD unPickNum = 0; //δ�����ĸ���
	WORD no=0;
	if(*CFG_pbyISSS == CFG_STATUS_CLR)
		return;
	for(;no<FLCOMPLICATENUM;no++)
	{
		//�в������
		if(FLComplicateInfo[no].pickupFlag)
		{
			//������������
			if(FLComplicateInfo[no].dataOKFlag)
			{
				ReportMsg("ComplicateInfo %d,call data ok!",no);				
				FaultLocateCalu(&RelaySubData[no][0],RelayFLData[no],no,FLComplicateInfo[no].rptSerialNo);
				DB_RelayEnd(FLComplicateInfo[no].rptSerialNo);	
				FLComplicateInfo[no].pickupFlag = FALSE;
				FLComplicateInfo[no].dataOKFlag = FALSE;
				FLComplicateInfo[no].rptSerialNo = MAX_REPORT_INDEX;
//				break;
			}
			else
			{
				if(dwRelayMSCnt - FLComplicateInfo[no].pickupTime >50000) 
				{							
					//�ٻ��������ݳ�ʱ����ʧ�ܱ��棬��ע�����β�����
					ReportMsg("call data timeout = %d",dwRelayMSCnt);
					OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,MAKEWORD(FLTASK_MODE_AUTOFL,no),0,0,0);	//������Ϣֹͣ�ٻ���������				
					WriteRMData(RM_pnDist,RM_DIST,0);
					WriteRMData(RM_pnKMS,RM_KMS,0);					
					MakeActRpt(rpt_TimeOut,FLComplicateInfo[no].rptSerialNo);
					FLSubTimeout(RelayFLData[no],FLComplicateInfo[no].rptSerialNo);				
					ResetFoultLocate();					
					DB_RelayEnd(FLComplicateInfo[no].rptSerialNo); 
					FLComplicateInfo[no].pickupFlag = FALSE;
					FLComplicateInfo[no].dataOKFlag = FALSE;
					FLComplicateInfo[no].rptSerialNo = MAX_REPORT_INDEX;
//					return;
				}
			}
		}
		else
			unPickNum++;		
	}

	for(int i=0;i<FLCOMPLICATENUM;i++)
	{
		if(FLComplicateInfo[i].pickupFlag == TRUE)
			return;
	}
	
	putoutLedFlag = TRUE;		   
	bStopRefreshRMFlag = FALSE;
//	if(no==4)
//	{
//		if(unPickNum==4)
//		{
//			//�����Ĳ���ж�����ɣ����ද���̵�����
//			putoutLedFlag = TRUE;          
//			bStopRefreshRMFlag = FALSE;
//		}
//		return;
//	}
	
//	FaultLocateCalu(&RelaySubData[no][0],RelayFLData[no],no,FLComplicateInfo[no].rptSerialNo);
//	DB_RelayEnd(FLComplicateInfo[no].rptSerialNo); 	
//	FLComplicateInfo[no].pickupFlag = FALSE;
//	FLComplicateInfo[no].dataOKFlag = FALSE;
//	FLComplicateInfo[no].rptSerialNo = MAX_REPORT_INDEX;
	
}

/****************************************************************************************
��  ��: FaultLocateAuto
��  ��: ������������ж�
��  ��: ��
����ֵ: 
*****************************************************************************************/

void CRelayTask::FaultLocateAuto()   //��������
{	
	BYTE no = 0;
	if(*SW_pbyFL == SW_STATUS_CUT)  //ѹ��δͶ��
	{
		ResetFoultLocate();
		return;
	}
	//�������������δͶ�����˳�
	if((*CFG_pby21_PI == CFG_STATUS_CLR)&&(*CFG_pby50_PI == CFG_STATUS_CLR)&&(*CFG_pby50DI_PI == CFG_STATUS_CLR))
	{		
		ResetAllRelay();					
		ResetFoultLocate();
		return;
	}
	if(m_bFaultOnFlag == FALSE) //��δ�б�������
	{
		if(!RelayPickUpCheck())//����δ����
		{
			ResetFoultLocate();
			return;
		}
		else //��������
		{
			ResetAllRelay();
			m_bFaultOnFlag=TRUE;
			m_dwFaultSINum=0;       //������������ʼ�����ϴ���������������
			return;
		}
	}
	m_dwFaultSINum +=RELAY_SAMP_INTERVAL;
	if(m_dwFaultSINum < POINT_PER_PERIOD)	  //������������ȴ�һ�ܲ�	
	{
		
		CalcInCreCurrent(&Rel50D1,dwIncre1CurBuf,dwInCre1DataMask);		
		CalcInCreCurrent(&Rel50D2,dwIncre2CurBuf,dwInCre2DataMask);
		ResetFoultLocate();
		return;
	}
	
	if(!HaveMakeStartRptFlag)	//������ϵ���������������д����ϵ���
	{
		MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
		HaveMakeStartRptFlag = TRUE;
	}
	GeneralRelay21(&Rel21);
	GeneralOverValueRelay(&Rel50);
	IncreCurrentRelay(&Rel50D1);		
	IncreCurrentRelay(&Rel50D2);	
	if(ReadRelayWord(Rel21.LoopCtrlRelayWord)||ReadRelayWord(Rel50.LoopCtrlRelayWord)||ReadRelayWord(Rel50D1.LoopCtrlRelayWord)\
		||ReadRelayWord(Rel50D2.LoopCtrlRelayWord))
	{			
		dwFaultLKTime = dwRelayMSCnt;
		m_bFaultCheckOutFlag = TRUE;
		if(ReadRelayWord(Rel21.ACTRelayWord)||ReadRelayWord(Rel50.ActRelayWord)||ReadRelayWord(Rel50D1.ACTRelayWord) \
			||ReadRelayWord(Rel50D2.ACTRelayWord))
		{
			if(!m_bHaveTripFlag)
			{
				//��ס����һ����������
				if(ReadRelayWord(Rel21.ACTRelayWord))
					relayType = Relay21;
				else if(ReadRelayWord(Rel50.ActRelayWord))
					relayType = Relay50;				
				else if(ReadRelayWord(Rel50D1.ACTRelayWord))
					relayType = Relay50D1;
				else
					relayType = Relay50D2;
				
				m_bHaveTripFlag = TRUE;		
				
//				ReportMsg("relay trip = %d",dwRelayMSCnt);
				//���ҿ�λ
				for(;no<FLCOMPLICATENUM;no++)  
				{
					if(!FLComplicateInfo[no].pickupFlag)						{
						
//						ReportMsg("ComplicateInfo no = %d",no);
						break;
					}
				}
				//�޿�λ���Ѿ���FLCOMPLICATENUM�β����ڴ���
				if(no == FLCOMPLICATENUM) 
				{
//					ReportMsg("ComplicateInfo no reach MAX!!!!!");
					return;
				}

				//�����������Ҫ�ٻ���������
				// 1���������Ӧ����Ͷ�룬���ǿ�ר����ģʽ.��Ե��ǿ�רģʽ����������(����Ӧ)����������(�̶�ģʽ)�Լ�Խ��(�̶�ģʽ)
				// 2. ��ר����ģʽ�Ҳ������Ӧ����δͶ�룬�̶�ʹ��ȫ����ģʽ���в��
				// 3. ���ϵ�����Ͷ�룬������ͨAT����ģʽ(scott��ѹ��)
				if(((*AS_pnPM == AT2MODE))|| \
					((*CFG_pbyAT_C_RATE ==CFG_STATUS_SET)&&(*AS_pnPM == AT1MODE)))
				{
					putoutLedFlag = FALSE;
					WriteRelayWord(RW_AFLACT,RELAY_ON);		//�ò�������̵�����
					FLComplicateInfo[no].pickupFlag = TRUE;
					FLComplicateInfo[no].pickupTime = dwRelayMSCnt;
					FLComplicateInfo[no].rptSerialNo = RptSerialNumOfAct;
					FLComplicateInfo[no].dataOKFlag = FALSE;
					MakeActRpt(R_FLACT,RptSerialNumOfAct); //����ද������ 	
					
//					ReportMsg("send message call data = %d",dwRelayMSCnt);
					LockingSSData(no,FLTASK_MODE_AUTOFL,TRUE);//������ǰ����������
					bStopRefreshRMFlag = TRUE;
					OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_CALLSUBDATA,FLPickTime.Lo,FLPickTime.Hi,MAKEWORD(FLTASK_MODE_AUTOFL,no),0);						
					OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DBASE), SM_STARTDIST, dwDataPtr,*AS_pnRECD,FLPickTime.Lo,FLPickTime.Hi);
					return;
				}					
				//���¼��־���ָ��ͨAT��ʽ�£�����Ҫ�ٻ��������ݵ����
				else
				{
 					//�����е�����Ͷ��
					if((*CFG_pbyUD_C_RATE == CFG_STATUS_SET)&&(*AS_pnPM == AT1MODE))
					{						
						OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DBASE), SM_STARTDIST, dwDataPtr,*AS_pnRECD,FLPickTime.Lo,FLPickTime.Hi);
						WriteRelayWord(RW_AFLACT,RELAY_ON);		//�ò�������̵�����
						UpDownLineCurrent(RptSerialNumOfAct);						
					}			
					//����������Ͷ��
					if((*CFG_pbyNL_C_RATE == CFG_STATUS_SET)&&(*AS_pnPM == AT1MODE))
					{					
						OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DBASE), SM_STARTDIST, dwDataPtr,*AS_pnRECD,FLPickTime.Lo,FLPickTime.Hi);
						WriteRelayWord(RW_AFLACT,RELAY_ON);		//�ò�������̵�����
						B_LineCurrent(RptSerialNumOfAct);
					}
				}
			}			
			else
				return;	
		}
		else
			return;
	}
	//�ޱ���ѭ����������λ���ȴ�0.5����˳����ϴ���
	else
	{		
		if(!m_bFaultCheckOutFlag)
		{
			m_bFaultCheckOutFlag = TRUE;
			dwFaultLKTime = dwRelayMSCnt;
		}
		else
		{
			if(dwRelayMSCnt - dwFaultLKTime>500)
			{
				if(!m_bHaveTripFlag)					
					DB_RelayEnd(RptSerialNumOfAct);
//				ReportMsg("relay reset = %d",dwRelayMSCnt);
				m_bHaveTripFlag = FALSE;
				ResetFoultLocate();
				ResetAllRelay();					
				HaveMakeStartRptFlag = FALSE;
				m_bFaultOnFlag = FALSE;
				m_dwFaultSINum = 0; 
				m_bFaultCheckOutFlag = FALSE;
				WriteRelayWord(RW_RELPICKUP, RELAY_OFF);
				return;
			}
		}
	}
}
/****************************************************************************************
��  ��: FaultLocateManual
��  ��: ʵ����������ж�
		���ֶ��ٻ���ʷ����(��һ�εı��������������)����ѹ���������Լ������ɵ�ʵʱ����
		���б����������ʵ���౻�������
		�ò�෽ʽ�����ǲ������������ٲⲻ��ͬʱ����
		60��֮������Ӧ����Ϊ�ٲ�ʧ��
��  ��: ��
����ֵ: 
*****************************************************************************************/

void CRelayTask::FaultLocateManual() //�ֶ�ʵ������
{
	if(*SW_pbyFL == SW_STATUS_CUT) //ѹ���˳�
	{
		manualFLPickFlag = FALSE; //ʵ����������־
		manualFLTimeoutFlag = FALSE; //ʵ���೬ʱ��־
		return;
	}
	if(ReadRelayWord(RW_AFLACT)) //�б���������ڽ���
		return;
	if(manualFLPickFlag == FALSE) //ʵ������δ����
	{
		if(ReadRelayWord(RW_MSTART)&&(!lastManualRWStatus)) //����������������
		{
			manualFLPickFlag = TRUE;
			manualFLPickTime = dwRelayMSCnt;
			manualFLTimeoutFlag = FALSE;	
			WriteRelayWord(RW_MFLACT,RELAY_ON);
			bStopRefreshRMFlag = TRUE;			
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_FLPICKUP].wInf,&FLPickTime,FALSE);
			manualFLSerialNo = g_RptSerialNum;
			MakeActRpt(R_CALLACT,manualFLSerialNo);	
			if(ReadRelayWord(RW_GETHISTORY)) // ȡ��ʷ����
			{
				ReportMsg("FL pickup for get history");
				manualDataType = FLTASK_MODE_HISTORYFL;
				g_bCallHistoryFLDataCompleted = FALSE;
				LockingSSData(FLCOMPLICATENUM,FLTASK_MODE_HISTORYFL,TRUE);
			}
			else if(ReadRelayWord(RW_GETLOWVOL)) // ȡ��ѹ��������
			{
				
				ReportMsg("FL pickup for get lowvotage");
				manualDataType = FLTASK_MODE_LOWVOLFL;
				g_bCallLowValueFLDataCompleted = FALSE;
				LockingSSData(FLCOMPLICATENUM,FLTASK_MODE_LOWVOLFL,TRUE);
			}
			else		//ȡ�����ɵ�����ʵʱ����
			{
				ReportMsg("FL pickup for get load");
				manualDataType = FLTASK_MODE_MANUALFL;				
				g_bCallManualFLDataCompleted = FALSE;
				LockingSSData(FLCOMPLICATENUM,FLTASK_MODE_MANUALFL,TRUE);
			}
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_CALLSUBDATA,FLPickTime.Lo,FLPickTime.Hi,MAKEWORD(manualDataType,FLCOMPLICATENUM),0);				
		}	
		
		lastManualRWStatus = ReadRelayWord(RW_MSTART);
	}
	else
	{	
		if((g_bCallManualFLDataCompleted == TRUE)&&(manualDataType == FLTASK_MODE_MANUALFL)) //�������ݽ�������
		{
			FaultLocateCalu(ManualSubData,ManualFLData,FLCOMPLICATENUM,manualFLSerialNo);
			WriteRelayWord(RW_MFLACT,RELAY_OFF);
			manualFLPickFlag = FALSE;
			manualFLTimeoutFlag = FALSE;
			bStopRefreshRMFlag = FALSE;
			DB_RelayEnd(manualFLSerialNo); 	
			g_bCallManualFLDataCompleted = FALSE;
		}
		else if((g_bCallLowValueFLDataCompleted == TRUE)&&(manualDataType == FLTASK_MODE_LOWVOLFL))
		{
			FaultLocateCalu(LowVolSubData,LowValFLData,FLCOMPLICATENUM,manualFLSerialNo);
			WriteRelayWord(RW_MFLACT,RELAY_OFF);
			manualFLPickFlag = FALSE;
			manualFLTimeoutFlag = FALSE;
			bStopRefreshRMFlag = FALSE;
			DB_RelayEnd(manualFLSerialNo); 	
			g_bCallLowValueFLDataCompleted = FALSE;
		}
		else if((g_bCallHistoryFLDataCompleted == TRUE)&&(manualDataType == FLTASK_MODE_HISTORYFL))
		{
			FaultLocateCalu(HistorySubData,ForeFLData,FLCOMPLICATENUM,manualFLSerialNo);
			WriteRelayWord(RW_MFLACT,RELAY_OFF);
			manualFLPickFlag = FALSE;
			manualFLTimeoutFlag = FALSE;
			bStopRefreshRMFlag = FALSE;
			DB_RelayEnd(manualFLSerialNo); 	
			g_bCallHistoryFLDataCompleted = FALSE;
		}
		else
		{
			if(dwRelayMSCnt - manualFLPickTime>60000)
			{				
				OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,MAKEWORD(manualDataType,FLCOMPLICATENUM),0,0,0);
				
				*RM_pnDist = 0;	//�˶���������ֵ����ʵʱ���㣬ʧ��ʱ����0
				*RM_pnKMS = 0;
				MakeActRpt(rpt_TimeOut,manualFLSerialNo); 
				FLSubTimeout(ManualFLData,manualFLSerialNo);
				manualFLTimeoutFlag = TRUE;				
				manualFLPickFlag = FALSE;	
				bStopRefreshRMFlag = FALSE;
				g_bCallManualFLDataCompleted = FALSE;				
				DB_RelayEnd(manualFLSerialNo); 				
				WriteRelayWord(RW_MFLACT,RELAY_OFF);
				
			}
		}		
	}	
	
}

/****************************************************************************************
��  ��: FLSubTimeout
��  ��: �ٻ��������ݳ�ʱ����
		������ʱ�жϣ����������������ݱ��棬��ʱ������������
��  ��: ��
����ֵ: 
*****************************************************************************************/

void CRelayTask::FLSubTimeout(subChannelCheck FLData[],BYTE rptSerialNo)
{
	for(LONG i=0;i<*RS_pnN;i++)
	{
		if(FLData[i].subRxdFlag == FALSE) //��ʱ������������
		{			
			FLData[i].U1 = 0;
			FLData[i].U2 = 0;	
			FLData[i].U3 = 0;
			FLData[i].U4 = 0;
			FLData[i].It1 = 0;
			FLData[i].If1 = 0;
			FLData[i].It2 = 0;
			FLData[i].If2 = 0;
			FLData[i].It3 = 0;
			FLData[i].If3 = 0;
			FLData[i].Iat1 = 0;
			FLData[i].Iat2 = 0;
			FLData[i].Iat = 0;
			FLData[i].AIt1 = 0;
			FLData[i].AIf1 = 0;
			FLData[i].AIt2 = 0;
			FLData[i].AIf2 = 0;
			FLData[i].AIt3 = 0;
			FLData[i].AIf3 = 0;	
			MakeSingleSubFLRpt(i,FLData[i].subDataLossInf,&FLData[i],rptSerialNo);
		}
		else
			MakeSingleSubFLRpt(i,FLData[i].subDataInf,&FLData[i],rptSerialNo);
			
	}
}
/****************************************************************************************
��  ��: FaultLocateCalu
��  ��: ������
��  ��: subData		-- ���ڼ���Ļ��������������5����ͤ�Ĺ�������
		FLData		-- ԭʼ���ݻ��������������4����ͤ�Ĺ�������(û������)
		no			-- ���ϲ�����
		rptSerialNo -- �������

����ֵ: 
*****************************************************************************************/

void CRelayTask::FaultLocateCalu(TSubData *subData,subChannelCheck FLData[],WORD no,BYTE rptSerialNo)
{
	BOOL subGetDataErr = FALSE;	
	SubGPSTimeoutFlag = FALSE;		
	//�˶���������ֵ����ʵʱ���㣬ʧ��ʱ����0
	*RM_pnDist = 0;
	*RM_pnKMS = 0;
	// 1.ˢ���������ݵ����㻺����
	RefreshSubData(subData,FLData);
	// 2.�ж��Ƿ���GPSʱ��������������������У��򲻽��в����㡣
	for(LONG i=0;i<*RS_pnN;i++)
	{
		if((FLData[i].GPSStatus&0x200)==0)
		{
			subGetDataErr = TRUE;
			ReportMsg("sub %d get data failed!",i);
		}
		//ʱ����������8����ʱ����10ms
		if((LOBYTE(LOWORD(FLData[i].GPSStatus))>=8)||((FLData[i].GPSStatus&0x100)==0))
		{
			SubGPSTimeoutFlag = TRUE;	
			ReportMsg("sub %d GPS error!",i);
		}
	}
	if((SubGPSTimeoutFlag == TRUE)||(subGetDataErr == TRUE))
	{	
		MakeSSFLRpt(no,rptSerialNo,rpt_GpsErr);
		//����������Ӧ����
		for(LONG i=0;i<*RS_pnN;i++)
		{
			//������ʱ���������꣬���ݲ�����
			if((LOBYTE(LOWORD(FLData[i].GPSStatus))>=8)||((FLData[i].GPSStatus&0x200)==0)||\
				((FLData[i].GPSStatus&0x100)==0)) 
				MakeSingleSubFLRpt(i,FLData[i].subGPSInf,&FLData[i],rptSerialNo);
			else				
				MakeSingleSubFLRpt(i,FLData[i].subDataInf,&FLData[i],rptSerialNo);
		}
		return;
	}

	// 3.���в�����
	if((*CFG_pbyAT_C_RATE ==CFG_STATUS_SET)&&(*AS_pnPM == AT1MODE))
	{
		//SCOTT��ѹ������ͨAT���緽ʽ�½�ʹ�����ϵ����Ƚ��й��ϲ��
		CalcFLMode_2(subData,no,rptSerialNo);
	}
	if((*CFG_pbyFL_AUTO == CFG_STATUS_CLR)&&(*AS_pnPM == AT2MODE))
	{
		//��ר���緽ʽ����ģʽ����������Ӧ�жϣ��̶�ʹ��ȫ����ģʽ���
		if(*RS_pnN == 2)
		{
			if(!XLineWithSub(subData,no,rptSerialNo))
			CalcFLMode_0(subData,no,rptSerialNo);
		}
		else if(*RS_pnN == 3)
			CalcFLWith3Sub(subData,no,rptSerialNo);
		else
		{			
			*RM_pnDist = 0;
			faultINF = R_UNKOWNMODE;
		}
			
	}
	if((*CFG_pbyFL_AUTO ==CFG_STATUS_SET)&&(*AS_pnPM == AT2MODE))
	{
		//��ר���緽ʽ�£�����ģʽ����Ӧ�жϣ���������Ӧ�Ĳ�෽�����м���
		if(ReadRelayWord(RW_HANDOVERMODE))
		{
			CalcFLWithHandOver(subData,no,rptSerialNo);
		}
		else
		{
			if(*RS_pnN == 2)
			{
				CheckPowerSupplyMode(subData);
				switch(powerSupplyMode)
				{
					case FLMODE_0:
						CalcFLMode_0(subData,no,rptSerialNo);
						break;					
					case FLMODE_1:
						CalcFLMode_1(subData,no,rptSerialNo);
						break;
					case FLMODE_2:
						CalcFLMode_2(subData,no,rptSerialNo);
						break;
					case FLMODE_3:
						CalcFLMode_3(subData,no,rptSerialNo);
						break;
					case FLMODE_4:
						CalcFLMode_4(subData,no,rptSerialNo);
						break;
					case FLMODE_5:
						CalcFLMode_5(subData,no,rptSerialNo);
						break;
					case FLMODE_6:
						CalcFLMode_6(subData,no,rptSerialNo);
						break;				
					case FLMODE_7:
						CalcFLMode_7(subData,no,rptSerialNo);
						break;
					case FLMODE_8:
						CalcFLMode_8(subData,no,rptSerialNo);
						break;				
					case FLMODE_9:
						CalcFLMode_9(subData,no,rptSerialNo);
						break;				
					case FLMODE_10:
						CalcFLMode_10(subData,no,rptSerialNo);
						break;				
					case FLMODE_11:
						CalcFLMode_11(subData,no,rptSerialNo);
						break;					
					case FLMODE_12:
						CalcFLMode_12(subData,no,rptSerialNo);
						break;
					default:
						*RM_pnDist = 0;
						faultINF = R_UNKOWNMODE;
						break;
				}
			}
			else if(*RS_pnN == 3)
			{
				CalcFLWith3Sub(subData,no,rptSerialNo);
			}
			else 
			{
				*RM_pnDist = 0;
				faultINF = R_UNKOWNMODE;
			}
		}
	}
	CaluKM();	
	// 4.����Ӧ����������������
	MakeSSFLRpt(no,rptSerialNo,faultINF);
	MakeSubFLRpt(FLData,no,rptSerialNo,0);

}
WORD CRelayTask::CheckPowerSupplyMode(TSubData *tsubData)
{
	BYTE CB_SS,CB_AT,CB_SSP;
	CB_SS = tsubData[0].CBStatus;
	CB_AT = tsubData[1].CBStatus;
	CB_SSP = tsubData[2].CBStatus;

	ReportMsg("ss_sb= %d,at_cb= %d,ssp_cb= %d",CB_SS,CB_AT,CB_SSP);
	powerSupplyMode = FLMODE_NULL;

	//ȫ������ʽ
	if((CB_SS == 0x03)&&((CB_AT == 0x1E)||(CB_AT == 0x1D))&&((CB_SSP == 0x1E)||(CB_SSP == 0x1D)))
		powerSupplyMode = FLMODE_0;
	
	//����ȫ������ʽ
	if(((CB_SS == 0x05)||(CB_SS == 0x06))&&((CB_AT == 0x1E)||(CB_AT == 0x1D))&&((CB_SSP == 0x1E)||(CB_SSP == 0x1D)))
		powerSupplyMode = FLMODE_1;

	//˫����AT��ʽ
	if(((CB_SS == 0x03))&&((CB_AT == 0x0F))&&((CB_SSP == 0x0F)))
		powerSupplyMode = FLMODE_2;
	
	//ȫ������ʽ��AT��ATȫ����
	if(((CB_SS == 0x03))&&((CB_AT == 0x00))&&((CB_SSP == 0x1E)||(CB_SSP == 0x1D)))
		powerSupplyMode = FLMODE_3;

	//ȫ������ʽ�·�����ATȫ����
	if(((CB_SS == 0x03))&&((CB_AT == 0x1E)||(CB_AT == 0x1D))&&(CB_SSP == 0x00))
		powerSupplyMode = FLMODE_4;

	//˫����ֱ����ʽ
	if(((CB_SS == 0x03))&&((CB_AT == 0x00))&&((CB_SSP == 0x00)))
		powerSupplyMode = FLMODE_5;
	
	//�����µ�˫����ֱ����ʽ
	if(((CB_SS == 0x05)||(CB_SS == 0x06))&&((CB_AT == 0x00))&&((CB_SSP == 0x00)))	
		powerSupplyMode = FLMODE_6;

	//ĩ�˲�����˫����ֱ����ʽ
	if(((CB_SS == 0x03))&&((CB_AT == 0x00))&&((CB_SSP == 0x0C)))	
		powerSupplyMode = FLMODE_7;
	
	//����ֱ��������AT��ʽ
	if(((CB_SS == 0x03))&&((CB_AT == 0x0A))&&((CB_SSP == 0x0A)))	
		powerSupplyMode = FLMODE_8;
	
	//����ֱ��������AT��ʽ
	if(((CB_SS == 0x03))&&((CB_AT == 0x05))&&((CB_SSP == 0x05)))	
		powerSupplyMode = FLMODE_9;

	ReportMsg("PS mode  = %d",powerSupplyMode);

	return powerSupplyMode;
}

void CRelayTask::MakeSSFLRpt(WORD no,BYTE rptSerialNo,WORD SSInf) //����ද������
{		
	WriteRMData(RM_pnU1,RM_U1,lkSSData[no].subData.U1);
	WriteRMData(RM_pnU2,RM_U2,lkSSData[no].subData.U2);
	WriteRMData(RM_pnU3,RM_U3,lkSSData[no].subData.U3);
	WriteRMData(RM_pnU4,RM_U4,lkSSData[no].subData.U4);
	WriteRMData(RM_pnIt1,RM_IT1,lkSSData[no].subData.IT1);
	WriteRMData(RM_pnIf1,RM_IF1,lkSSData[no].subData.IF1);
	WriteRMData(RM_pnIt2,RM_IT2,lkSSData[no].subData.IT2);
	WriteRMData(RM_pnIf2,RM_IF2,lkSSData[no].subData.IF2);
	WriteRMData(RM_pnIt3,RM_IT3,lkSSData[no].subData.IT3);
	WriteRMData(RM_pnIat1,RM_IAT1,lkSSData[no].subData.IAT1);
	WriteRMData(RM_pnIat2,RM_IAT2,lkSSData[no].subData.IAT2);
	WriteRMData(RM_pnSSIAT,RM_SSIAT,lkSSData[no].subData.IAT);
	WriteRMData(RM_pnU1A,RM_U1A,lkSSData[no].subData.AU1);
	WriteRMData(RM_pnU2A,RM_U2A,lkSSData[no].subData.AU2);
	WriteRMData(RM_pnU3A,RM_U3A,lkSSData[no].subData.AU3);
	WriteRMData(RM_pnU4A,RM_U4A,lkSSData[no].subData.AU4);
	WriteRMData(RM_pnIT1A,RM_IT1A,lkSSData[no].subData.AIT1);
	WriteRMData(RM_pnIF1A,RM_IF1A,lkSSData[no].subData.AIF1);
	WriteRMData(RM_pnIT2A,RM_IT2A,lkSSData[no].subData.AIT2);
	WriteRMData(RM_pnIF2A,RM_IF2A,lkSSData[no].subData.AIF2);
	WriteRMData(RM_pnIT3A,RM_IT3A,lkSSData[no].subData.AIT3);
	WriteRMData(RM_pnIF3A,RM_IF3A,lkSSData[no].subData.AIF3);	
	WriteRMData(RM_pnR_T,RM_R_T,lkSSData[no].lockImpd_T.Real);
	WriteRMData(RM_pnX_T,RM_X_T,lkSSData[no].lockImpd_T.Imag);	
	WriteRMData(RM_pnR_F,RM_R_T,lkSSData[no].lockImpd_F.Real);
	WriteRMData(RM_pnX_F,RM_X_T,lkSSData[no].lockImpd_F.Imag);
	WriteRMData(RM_pnR_TF,RM_R_T,lkSSData[no].lockImpd.Real);
	WriteRMData(RM_pnX_TF,RM_X_T,lkSSData[no].lockImpd.Imag);
	WriteRMData(RM_pnDist,RM_DIST,*RM_pnDist);
	WriteRMData(RM_pnKMS,RM_KMS,*RM_pnKMS);

	//�迹�ı�������ֵ��������㣬�����ֵ翹�������ϲ��
	MakeActRpt(SSInf,rptSerialNo);
}
void CRelayTask::MakeSubFLRpt(subChannelCheck FLData[],WORD no,BYTE rptSerialNo,WORD subInfType)
{
	DWORD inf;
	for(LONG j=0;j<*RS_pnN;j++)
	{		
		if(subInfType == 0)			
			inf = FLData[j].subDataInf;
		if(subInfType == 1)
			inf = FLData[j].subGPSInf;
		if(subInfType == 2)
			inf = FLData[j].subFaultInf;
		
		MakeSingleSubFLRpt(j,inf,&FLData[j],rptSerialNo);
	}	
}
void CRelayTask::MakeSingleSubFLRpt(WORD subno,DWORD inf,subChannelCheck *data,BYTE rptSerialNo)
{
	LONG* pSubBaseAddr = NULL;
	LONG  SubBaseNo = 0;
	if(subno == 0)
	{
		pSubBaseAddr = RM_pnSub1_U1;
		SubBaseNo = RM_SUB1_U1;
	}
	else if(subno ==1)
	{
		pSubBaseAddr = RM_pnSub2_U1;	
		SubBaseNo = RM_SUB2_U1;
	}
	else if(subno ==2)
	{
		pSubBaseAddr = RM_pnSub3_U1;	
		SubBaseNo = RM_SUB3_U1;
	}
	else if(subno ==3)
	{
		pSubBaseAddr = RM_pnSub4_U1;	
		SubBaseNo = RM_SUB4_U1;
	}
	else if(subno == 4)
	{
		pSubBaseAddr = RM_pnSub5_U1;	
		SubBaseNo = RM_SUB5_U1;
	}
	else
		return;

	WriteRMData(pSubBaseAddr,SubBaseNo,data->U1);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->U2);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->U3);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->U4);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->It1);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->If1);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->It2);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->If2);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->It3);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->If3);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->Iat1);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->Iat2);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->Iat);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->AIt1);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->AIf1);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->AIt2);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->AIf2);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->AIt3);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,data->AIf3);
	WriteRMData(++pSubBaseAddr,++SubBaseNo,LOBYTE(HIWORD(data->GPSStatus)));
//	ReportMsg("status = %x",*pSubBaseAddr);
	MakeActRpt(inf,rptSerialNo);
	
}

void CRelayTask::BoostingCurrent(TSubData *subData) 	//���ϵ�����
{
	TSubData * pSubData = (TSubData *)subData;
	
	if(*CFG_pbyAT_C_RATE == CFG_STATUS_CLR) //���ϵ�����δͶ��
		return;
	
	DWORD totalfeedout;		
	DWORD temp1,temp2,temp3,temp4,temp5,temp6;
	//totalfeedout = pSubData[0].IT1+pSubData[0].IF1+pSubData[0].IT2+pSubData[0].IF2;
	totalfeedout = pSubData[0].IP1+pSubData[0].IP2;
	if(ReadRelayWord(RW_HANDOVERMODE))
	{
		if(faultRange == RANGE1)//��һ������ϲ��������е����ȷ�ʽ
		{
//			temp1 = pSubData[0].IT1+pSubData[0].IF1;
//			temp2 = pSubData[0].IT2+pSubData[0].IF2;
			temp1 = pSubData[0].IP1;
			temp2 = pSubData[0].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
		}
		else if(faultRange == RANGE2) //�ڶ�����������ϵ����ȷ�ʽ
		{			
			LONG	rangeNo;		
			LONG   Q1,Q2,C1,C2;
			rangeNo = faultRange&0x00F; 

			Q1 = QValue[rangeNo];
			C1 = DistCoef[rangeNo];
			if(m_SSPSelect == FL_SUBST_SELFSP)
			{
				Q2 = QValue[rangeNo+1];
				C2 = DistCoef[rangeNo+1];
			}
			else if(m_SSPSelect == FL_SUBST_SIDESP)
			{
				Q2 = QValue[rangeNo+2];
				C2 = DistCoef[rangeNo+2];
			}
			
			temp1 = pSubData[rangeNo+1].IAT*C2;
			temp2 = pSubData[rangeNo].IAT*C1;
			temp3 = temp1+temp2; 
			temp4 = 10000-Q1-Q2;
			if(temp4!=0&&temp3!=0)
			{
				temp5 = (((LLONG)temp1*10000)/temp3)-Q1;
				temp6 = ((LLONG)temp5*DistanceSet[rangeNo])/temp4;		
			}
			*RM_pnDist = *RS_pnD1+temp6;
		}
		else if(faultRange == RANGE3)
		{
			LONG	rangeNo;			
			rangeNo = faultRange&0x00F; 			
			LONG   Q1,Q2,C1,C2;
			Q2 = QValue[rangeNo+2];
			C2 = DistCoef[rangeNo+2];
			
			if(m_SSPSelect == FL_SUBST_SELFSP)
			{
				Q1 = QValue[rangeNo];
				C1 = DistCoef[rangeNo];
			}
			else if(m_SSPSelect == FL_SUBST_SIDESP)
			{
				Q1 = QValue[rangeNo+1];
				C1 = DistCoef[rangeNo+1];
			}
			temp1 = pSubData[rangeNo+1].IAT*C2;
			temp2 = pSubData[rangeNo].IAT*C1;
			temp3 = temp1+temp2; 
			temp4 = 10000-Q1-Q2;
			if(temp4!=0&&temp3!=0)
			{
				temp5 = (((LLONG)temp1*10000)/temp3)-Q1;
				temp6 = ((LLONG)temp5*DistanceSet[rangeNo])/temp4;		//Խ��ʱ��Ȼ��5������������Ϊ4������
			}
			*RM_pnDist = *RS_pnD1+*RS_pnD2+temp6;
		}
		else if(faultRange == RANGE4) //����������������е�����
		{
//			temp1 = pSubData[4].IT1+pSubData[4].IF1;
//			temp2 = pSubData[4].IT2+pSubData[4].IF2;
			temp1 = pSubData[4].IP1;
			temp2 = pSubData[4].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = *RS_pnD1+*RS_pnD2+*RS_pnD3+((LLONG)*RS_pnD4*2*temp3)/totalfeedout;
		}		
		DB_LogWriteRM(RM_DIST,*RM_pnDist);
		return;
	}
	
	if(*RS_pnN == 2) //ֻ����������,���÷ֶε������е����ȼ���
	{
		if(faultRange == RANGE1)
		{
//			temp1 = pSubData[0].IT1+pSubData[0].IF1;
//			temp2 = pSubData[0].IT2+pSubData[0].IF2;
			temp1 = pSubData[0].IP1;
			temp2 = pSubData[0].IP2;

			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
		}
		if(faultRange == RANGE2)
		{
//			temp1 = pSubData[2].IT1+pSubData[2].IF1;
//			temp2 = pSubData[2].IT2+pSubData[2].IF2;
			
			temp1 = pSubData[2].IP1;
			temp2 = pSubData[2].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = *RS_pnD1+((LLONG)*RS_pnD2*2*temp3)/totalfeedout;
		}
		DB_LogWriteRM(RM_DIST,*RM_pnDist);
		return;
	}
	if(*RS_pnN == 3)
	{
		if(faultRange == RANGE1)//��һ������ϲ��������е����ȷ�ʽ
		{
//			temp1 = pSubData[0].IT1+pSubData[0].IF1;
//			temp2 = pSubData[0].IT2+pSubData[0].IF2;
			
			temp1 = pSubData[0].IP1;
			temp2 = pSubData[0].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
		}
		else if(faultRange == RANGE2) //�ڶ�����������ϵ����ȷ�ʽ
		{			
			LONG	rangeNo;			
			rangeNo = faultRange&0x00F; 
			temp1 = pSubData[rangeNo+1].IAT*DistCoef[rangeNo+1];
			temp2 = pSubData[rangeNo].IAT*DistCoef[rangeNo];
			temp3 = temp1+temp2; 
			temp4 = 10000-QValue[rangeNo]-QValue[rangeNo+1];
			if(temp4!=0&&temp3!=0)
			{
				temp5 = (((LLONG)temp1*10000)/temp3)-QValue[rangeNo];
				temp6 = ((LLONG)temp5*DistanceSet[rangeNo])/temp4;		
			}
			*RM_pnDist = *RS_pnD1+temp6;
		}
		else if(faultRange == RANGE3) //����������������е�����
		{
//			temp1 = pSubData[3].IT1+pSubData[3].IF1;
//			temp2 = pSubData[3].IT2+pSubData[3].IF2;
			
			temp1 = pSubData[3].IP1;
			temp2 = pSubData[3].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = *RS_pnD1+*RS_pnD2+((LLONG)*RS_pnD3*2*temp3)/totalfeedout;
		}		
		DB_LogWriteRM(RM_DIST,*RM_pnDist);
	}
	if((*RS_pnN == 4)||(*RS_pnN == 5))
	{
		if(faultRange == RANGE1)//��һ������ϲ��������е����ȷ�ʽ
		{
//			temp1 = pSubData[0].IT1+pSubData[0].IF1;
//			temp2 = pSubData[0].IT2+pSubData[0].IF2;
			
			temp1 = pSubData[0].IP1;
			temp2 = pSubData[0].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
		}
		else if(faultRange == RANGE2) //�ڶ�����������ϵ����ȷ�ʽ
		{			
			LONG	rangeNo;			
			rangeNo = faultRange&0x00F; 
			temp1 = pSubData[rangeNo+1].IAT*DistCoef[rangeNo+1];
			temp2 = pSubData[rangeNo].IAT*DistCoef[rangeNo];
			temp3 = temp1+temp2; 
			temp4 = 10000-QValue[rangeNo]-QValue[rangeNo+1];
			if(temp4!=0&&temp3!=0)
			{
				temp5 = (((LLONG)temp1*10000)/temp3)-QValue[rangeNo];
				temp6 = ((LLONG)temp5*DistanceSet[rangeNo])/temp4;		
			}
			*RM_pnDist = *RS_pnD1+temp6;
		}
		else if(faultRange == RANGE3)
		{
			LONG	rangeNo;			
			rangeNo = faultRange&0x00F; 
			temp1 = pSubData[rangeNo+1].IAT*DistCoef[rangeNo+1];
			temp2 = pSubData[rangeNo].IAT*DistCoef[rangeNo];
			temp3 = temp1+temp2; 
			temp4 = 10000-QValue[rangeNo]-QValue[rangeNo+1];
			if(temp4!=0&&temp3!=0)
			{
				temp5 = (((LLONG)temp1*10000)/temp3)-QValue[rangeNo];
				temp6 = ((LLONG)temp5*DistanceSet[rangeNo])/temp4;		
			}
			*RM_pnDist = *RS_pnD1+*RS_pnD2+temp6;
		}
		else if(faultRange == RANGE4) //����������������е�����
		{
//			temp1 = pSubData[4].IT1+pSubData[4].IF1;
//			temp2 = pSubData[4].IT2+pSubData[4].IF2;			
			temp1 = pSubData[4].IP1;
			temp2 = pSubData[4].IP2;
			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = *RS_pnD1+*RS_pnD2+*RS_pnD3+((LLONG)*RS_pnD4*2*temp3)/totalfeedout;
		}		
		DB_LogWriteRM(RM_DIST,*RM_pnDist);
	}

}
void CRelayTask::UpDownLineCurrent(BYTE rptSerialNo) // �����е�����
{	
	DWORD inf;
	if(*CFG_pbyUD_C_RATE == CFG_STATUS_CLR) //�����е�����δͶ��
		return;

	DWORD tIp1,tIp2,tIp3;
	DWORD tIT1,tIF1,tIT2,tIF2;
	DWORD totalfeedout,maxIp,minI;
	//��������ж�
	if((exchangeElecFlag == I3I2I1)||(exchangeElecFlag == I2I3I1))
	{
		tIT1 = m_IT3mod;
		tIF1 = m_IF3mod;
		tIp1 = m_IP3mod;
	}
	else
	{
		tIT1 = m_IT1mod;
		tIF1 = m_IF1mod;
		tIp1 = m_IP1mod;
	}
	if((exchangeElecFlag == I3I1I2)||(exchangeElecFlag == I1I3I2))
	{
		tIT2 = m_IT3mod;
		tIF2 = m_IF3mod;
		tIp2 = m_IP3mod;
	}
	else
	{
		tIT2 = m_IT2mod;
		tIF2 = m_IF2mod;
		tIp2 = m_IP2mod;
	}
	totalfeedout = tIp1+tIp2;
	maxIp = MAX4(tIT1,tIF1,tIT2,tIF2);
	if(maxIp == tIT1)
	{
		faultDirection = DOWNLINE;			
		faultType = T_R;
		inf = rpt_B_TR;
	}
	else if(maxIp == tIF1)
	{
		faultDirection = DOWNLINE;			
		faultType = F_R;
		inf = rpt_B_FR;
	}
	else if(maxIp == tIT2)
	{
		faultDirection = UPLINE;			
		faultType = T_R;
		inf = rpt_F_TR;
	}
	else
	{
		faultDirection = UPLINE;			
		faultType = F_R;
		inf = rpt_F_FR;
	}
	minI = MIN2(tIp1,tIp2);
	*RM_pnDist = ((LLONG)*RS_pnLa*2*minI)/totalfeedout;
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
	CaluKM();
	MakeActRpt(inf,rptSerialNo);	
	DB_RelayEnd(rptSerialNo);
}
void CRelayTask::B_LineCurrent(BYTE rptSerialNo)		//����������
{
	DWORD inf;
	if(*CFG_pbyNL_C_RATE == CFG_STATUS_CLR) //����������δͶ��
		return;
	LLONG temp;
	temp = (LLONG)(m_IT1mod+m_IF1mod-m_IAT_SS)*(*RS_pnLa);
	*RM_pnDist = temp/(m_IT1mod+m_IF1mod);		
	if(m_IT1mod>m_IF1mod)
	{
		faultType = T_R;
		inf = rpt_F_TR;
	}
	else
	{
		faultType = F_R;
		inf = rpt_F_FR;
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);	
	CaluKM();
	MakeActRpt(inf,rptSerialNo);	
	DB_RelayEnd(rptSerialNo);

}

void CRelayTask::ResetFoultLocate()
{
	
}
void CRelayTask::ResetAllRelay()
{
	ResetR50DI(&Rel50D1);
	ResetR50DI(&Rel50D2);
	ResetRelay21(&Rel21);
	ResetOverValueRelay(&Rel50);
}
//�ú������������������ݲ���������ˢ�µ����㻺������
//�����ݻ����ڽ������ʱ����౨��Ĺ��ϵ���
//�������ֶ��ٲ�����������ʵʱ����
//��ʷ������������һ�εı��������������
//��ѹ�����������ѹʱ������¼������
void CRelayTask::LockingSSData(WORD no,WORD FLType,BOOL refreshFlag)
{
	//����Ǳ����������������ֶ��ٲ�ʵʱ���ݣ�����ʵʱ����	
	//pTSubData��һָ�룬���ݲ�������ָ��ͬ�����ݽṹ��
	//���Խ���������������ˢ�µ���ͬ�����ݽṹ��
	if((FLType == FLTASK_MODE_AUTOFL)||(FLType == FLTASK_MODE_MANUALFL))
	{
		lkSSData[no].subData.U1 = m_U1.Mod;
		lkSSData[no].subData.U2 = m_U2.Mod;
		lkSSData[no].subData.U3 = m_U3.Mod;
		lkSSData[no].subData.U4 = m_U4.Mod;
		lkSSData[no].subData.IT1 = m_IT1.Mod;
		lkSSData[no].subData.IF1 = m_IF1.Mod;
		lkSSData[no].subData.IT2 = m_IT2.Mod;
		lkSSData[no].subData.IF2 = m_IF2.Mod;
		lkSSData[no].subData.IT3 = m_IT3.Mod;
		lkSSData[no].subData.IF3 = m_IF3.Mod;
		lkSSData[no].subData.IAT1 = m_IAT1.Mod;
		lkSSData[no].subData.IAT2 = m_IAT2.Mod;
		lkSSData[no].subData.IAT = m_IAT_SS; //ע�⣬�����Ѿ���һ��ֵ�ˣ��������ĵ������Ƕ���ֵ
		lkSSData[no].subData.AU1 = m_U1.Angle - m_IT1.Angle;
		lkSSData[no].subData.AU2 = m_U2.Angle - m_IT1.Angle;
		lkSSData[no].subData.AU3 = m_U3.Angle - m_IT1.Angle;
		lkSSData[no].subData.AU4 = m_U4.Angle - m_IT1.Angle;
		lkSSData[no].subData.AIT1 = 0;
		lkSSData[no].subData.AIF1 = m_IF1.Angle - m_IT1.Angle;
		lkSSData[no].subData.AIT2 = m_IT2.Angle - m_IT1.Angle;
		lkSSData[no].subData.AIF2 = m_IF2.Angle - m_IT1.Angle;
		lkSSData[no].subData.AIT3 = m_IT3.Angle - m_IT1.Angle;
		lkSSData[no].subData.AIF3 = m_IF3.Angle - m_IT1.Angle;
		lkSSData[no].subData.CBStatus = m_CB;
		lkSSData[no].subData.IP1 = m_IP1.Mod;
		lkSSData[no].subData.IP2 = m_IP2.Mod;
		lkSSData[no].subData.IP3 = m_IP3.Mod;
		//����ʸ�������ڵ翹�����
		lkSSData[no].lockU1 = m_U1;	
		lkSSData[no].lockU2 = m_U2;
		lkSSData[no].lockU3 = m_U3;
		lkSSData[no].lockU4 = m_U4;
		lkSSData[no].lockIT1 = m_IT1;
		lkSSData[no].lockIF1 = m_IF1;
		lkSSData[no].lockIT2 = m_IT2;
		lkSSData[no].lockIF2 = m_IF2;
		lkSSData[no].lockIT3 = m_IT3;
		lkSSData[no].lockIF3 = m_IF3;
		lkSSData[no].lockIAT1 = m_IAT1;
		lkSSData[no].lockIAT2 = m_IAT2;		
		lkSSData[no].lockUp1 = m_UP1;
		lkSSData[no].lockUp2 = m_UP2;
		lkSSData[no].lockIp1 = m_IP1;
		lkSSData[no].lockIp2 = m_IP2;
		lkSSData[no].lockIp3 = m_IP3;
		lkSSData[no].lockImpd = m_Impd;
		lkSSData[no].lockImpd_T = Impd_T;
		lkSSData[no].lockImpd_F = Impd_F;
		lkSSData[no].exchangeFlag = exchangeElecFlag;
		lkSSData[no].relaytype = relayType;

		if(FLType == FLTASK_MODE_MANUALFL)
		{
			pTSubData = &ManualSubData[0];
		}
		else
		{
			pTSubData = &RelaySubData[no][0];
			(*g_pSS_LockHistory) = lkSSData[no];  //Ҫ��¼�����µ��������ݣ������ٲ���ʷ
		}		
	}
	else if(FLType == FLTASK_MODE_HISTORYFL)
	{
		lkSSData[no] = (*g_pSS_LockHistory);
		pTSubData = &HistorySubData[0];
	}
	
	else if(FLType == FLTASK_MODE_LOWVOLFL)
	{
		lkSSData[no] = (*g_pSS_LockLowVoltage);
		pTSubData = &LowVolSubData[0];
	}
	else
		return;

 	//��һ��ֵ����ˢ�����㻺����
	pTSubData->U1 = lkSSData[no].subData.U1*(*AS_pnUPTRatio);
	pTSubData->U2 = lkSSData[no].subData.U2*(*AS_pnUPTRatio);
	pTSubData->U3 = lkSSData[no].subData.U3*(*AS_pnUPTRatio);
	pTSubData->U4 = lkSSData[no].subData.U4*(*AS_pnUPTRatio);
	pTSubData->IT1 = lkSSData[no].subData.IT1*(*AS_pnI1CTRatio)/(*CF_pnIT1); //ת����Ϊ����ϵ����һ��ֵ���Ŵ�100��
	pTSubData->IF1 = lkSSData[no].subData.IF1*(*AS_pnI1CTRatio)/(*CF_pnIF1);
	pTSubData->IT2 = lkSSData[no].subData.IT2*(*AS_pnI2CTRatio)/(*CF_pnIT2);
	pTSubData->IF2 = lkSSData[no].subData.IF2*(*AS_pnI2CTRatio)/(*CF_pnIF2);
	pTSubData->IT3 = lkSSData[no].subData.IT3*(*AS_pnI3CTRatio)/(*CF_pnIT3);
	pTSubData->IF3 = lkSSData[no].subData.IF3*(*AS_pnI3CTRatio)/(*CF_pnIF3);
	pTSubData->IAT1 = lkSSData[no].subData.IAT1*(*AS_pnIATCTRatio)/(*CF_pnIAT1);
	pTSubData->IAT2 = lkSSData[no].subData.IAT2*(*AS_pnIATCTRatio)/(*CF_pnIAT2);
	pTSubData->IAT = lkSSData[no].subData.IAT/(*CF_pnIAT1);
	pTSubData->CBStatus = lkSSData[no].subData.CBStatus;
	pTSubData->IP1 = lkSSData[no].subData.IP1*(*AS_pnI1CTRatio)/(*CF_pnIT1);
	pTSubData->IP2 = lkSSData[no].subData.IP2*(*AS_pnI2CTRatio)/(*CF_pnIT2);
	pTSubData->IP3 = lkSSData[no].subData.IP3*(*AS_pnI3CTRatio)/(*CF_pnIT3);

	//�жϱ�����������ߵ�Ͷ��������ߵ�������ʹ֮�ڼ�����ֻ��Ҫʹ��IT1��IF1��IT2��IF2
	//�ڲ���������У�IT3,IF3���ٲ������м���
	if((lkSSData[no].exchangeFlag == I3I2I1)||(lkSSData[no].exchangeFlag  == I2I3I1))
	{
		LONG tempT,tempF,tempP;
		tempT = pTSubData->IT1;
		tempF = pTSubData->IF1;
		tempP = pTSubData->IP1;
		pTSubData->IT1 = pTSubData->IT3;
		pTSubData->IF1 = pTSubData->IF3;
		pTSubData->IP1 = pTSubData->IP3;
		pTSubData->IT3 = tempT;
		pTSubData->IF3 = tempF;
		pTSubData->IP3 = tempP;
		
	}
	else if((lkSSData[no].exchangeFlag  == I1I3I2)||(lkSSData[no].exchangeFlag  == I3I1I2))
	{
		
		LONG tempT,tempF,tempP;
		tempT = pTSubData->IT2;
		tempF = pTSubData->IF2;	
		tempP = pTSubData->IP2;
		pTSubData->IT2 = pTSubData->IT3;
		pTSubData->IF2 = pTSubData->IF3;
		pTSubData->IP2 = pTSubData->IP3;
		pTSubData->IT3 = tempT;
		pTSubData->IF3 = tempF;
		pTSubData->IP3 = tempP;
		
	}	
}
void CRelayTask::RefreshSubData(TSubData *subData,subChannelCheck fldata[])	//������������
{
	if(*RS_pnN<1||*RS_pnN>6)
	{
		return; //����������ֵ������
	}
	
	if(!ReadRelayWord(RW_HANDOVERMODE)) //��Խ��ģʽ��������ģʽ
	{
		for(LONG i=0;i<*RS_pnN;i++)
		{		
			subData[i+1].U1 = fldata[i].U1;	
			subData[i+1].U2 = fldata[i].U2;
			subData[i+1].U3 = fldata[i].U3;
			subData[i+1].U4 = fldata[i].U4;
			subData[i+1].IT1 = fldata[i].It1;
			subData[i+1].IF1 = fldata[i].If1;	
			subData[i+1].IT2 = fldata[i].It2;
			subData[i+1].IF2 = fldata[i].If2;
			subData[i+1].IT3 = fldata[i].It3;
			subData[i+1].IF3 = fldata[i].If3;
			subData[i+1].IAT1 = fldata[i].Iat1;
			subData[i+1].IAT2 = fldata[i].Iat2;
			subData[i+1].IAT = fldata[i].Iat;
			subData[i+1].IP1 = fldata[i].Ip1;
			subData[i+1].IP2 = fldata[i].Ip2;
			subData[i+1].IP3 = fldata[i].Ip3;
			subData[i+1].CBStatus = LOBYTE(HIWORD(fldata[i].GPSStatus));// (fldata[i].GPSStatus>>16)&0xFF;
			
//			ReportMsg("sub %d,CB =%d, GPSSTATUS=%X",i,subData[i+1].CBStatus,fldata[i].GPSStatus);
		}
	}
	else
	{
		//Խ��ģʽ�£�������������ֵ����кϳ�
		TFL_Disp *tFL;
		for(LONG i=0;i<*RS_pnN;i++)
		{
			DB_GetFLObjDisp(i,tFL);
			if(tFL->wControl&0xF == FL_SUBST_SELFAT)//����AT
			{
				subData[1].U1 = fldata[i].U1;	
				subData[1].U2 = fldata[i].U2;
				subData[1].U3 = fldata[i].U3;
				subData[1].U4 = fldata[i].U4;
				subData[1].IT1 = fldata[i].It1;
				subData[1].IF1 = fldata[i].If1;	
				subData[1].IT2 = fldata[i].It2;
				subData[1].IF2 = fldata[i].If2;
				subData[1].IT3 = fldata[i].It3;
				subData[1].IF3 = fldata[i].If3;
				subData[1].IAT1 = fldata[i].Iat1;
				subData[1].IAT2 = fldata[i].Iat2;
				subData[1].IAT = fldata[i].Iat1+fldata[i].Iat2;
				
				subData[1].IP1 = fldata[i].Ip1;
				subData[1].IP2 = fldata[i].Ip2;
				subData[1].IP3 = fldata[i].Ip3;
			}
			if(tFL->wControl&0xF == FL_SUBST_SELFSP) //����SP
			{
				subData[2].U1 = fldata[i].U1;	
				subData[2].U2 = fldata[i].U2;
				subData[2].U3 = fldata[i].U3;
				subData[2].U4 = fldata[i].U4;
				subData[2].IT1 = fldata[i].It1;
				subData[2].IF1 = fldata[i].If1;	
				subData[2].IT2 = fldata[i].It2;
				subData[2].IF2 = fldata[i].If2;
				subData[2].IT3 = fldata[i].It3;
				subData[2].IF3 = fldata[i].If3;
				subData[2].IAT1 = fldata[i].Iat1;
				subData[2].IAT2 = fldata[i].Iat2;
				subData[2].IAT = fldata[i].Iat1+fldata[i].Iat2;
				
				subData[2].IP1 = fldata[i].Ip1;
				subData[2].IP2 = fldata[i].Ip2;
				subData[2].IP3 = fldata[i].Ip3;
				if(subData[2].IAT<30)
					m_SSPSelect = FL_SUBST_SIDESP;
			}			
			if(tFL->wControl&0xF == FL_SUBST_SIDESP) //�Բ�SP
			{
				//��ѹȡ��1�����ǼӺ�?????
				subData[2].IT1 += fldata[i].It1;
				subData[2].IF1 += fldata[i].If1;	
				subData[2].IT2 += fldata[i].It2;
				subData[2].IF2 += fldata[i].If2;
				subData[2].IT3 += fldata[i].It3;
				subData[2].IF3 += fldata[i].If3;
				subData[2].IAT1 += fldata[i].Iat1;
				subData[2].IAT2 += fldata[i].Iat2;
				subData[2].IAT += fldata[i].Iat1+fldata[i].Iat2;
				subData[2].IP1 = fldata[i].Ip1;
				subData[2].IP2 = fldata[i].Ip2;
				subData[2].IP3 = fldata[i].Ip3;

				if(fldata[i].Iat1+fldata[i].Iat2 <30)
					m_SSPSelect = FL_SUBST_SIDESP;
				
			}
			if(tFL->wControl&0xF == FL_SUBST_SIDEAT) //�Բ�AT
			{
				subData[3].U1 = fldata[i].U1;	
				subData[3].U2 = fldata[i].U2;
				subData[3].U3 = fldata[i].U3;
				subData[3].U4 = fldata[i].U4;
				subData[3].IT1 = fldata[i].It1;
				subData[3].IF1 = fldata[i].If1;	
				subData[3].IT2 = fldata[i].It2;
				subData[3].IF2 = fldata[i].If2;
				subData[3].IT3 = fldata[i].It3;
				subData[3].IF3 = fldata[i].If3;
				subData[3].IAT1 = fldata[i].Iat1;
				subData[3].IAT2 = fldata[i].Iat2;
				subData[3].IAT = fldata[i].Iat1+fldata[i].Iat2;
				
				subData[3].IP1 = fldata[i].Ip1;
				subData[3].IP2 = fldata[i].Ip2;
				subData[3].IP3 = fldata[i].Ip3;
			}	
			
			if(tFL->wControl&0xF == FL_SUBST_SIDEMAIN) //�Բ�AT
			{
				subData[4].U1 = fldata[i].U1;	
				subData[4].U2 = fldata[i].U2;
				subData[4].U3 = fldata[i].U3;
				subData[4].U4 = fldata[i].U4;
				subData[4].IT1 = fldata[i].It1;
				subData[4].IF1 = fldata[i].If1;	
				subData[4].IT2 = fldata[i].It2;
				subData[4].IF2 = fldata[i].If2;
				subData[4].IT3 = fldata[i].It3;
				subData[4].IF3 = fldata[i].If3;
				subData[4].IAT1 = fldata[i].Iat1;
				subData[4].IAT2 = fldata[i].Iat2;
				subData[4].IAT = fldata[i].Iat1+fldata[i].Iat2;
				
				subData[4].IP1 = fldata[i].Ip1;
				subData[4].IP2 = fldata[i].Ip2;
				subData[4].IP3 = fldata[i].Ip3;
			}			
		}
		
	}
	
}
//����T�� F�� TF���迹
void CRelayTask::CaluImpd(WORD no)
{
	//�жϵ�������״̬�������ĵ�������Ӧ�ĵ�ѹ����T��F�迹
	if((lkSSData[no].exchangeFlag == I1I2I3)||(lkSSData[no].exchangeFlag == I1I3I2))
	{
		CaluRX(&lkSSData[no].lockU1,&lkSSData[no].lockIT1,&Impd_T,TRUE1,CF_pnZ);	//����T���迹
		CaluRX(&lkSSData[no].lockU2,&lkSSData[no].lockIF1,&Impd_F,TRUE1,CF_pnZ);
	}
	else if((lkSSData[no].exchangeFlag == I2I1I3)||(lkSSData[no].exchangeFlag == I2I3I1))
	{
		CaluRX(&lkSSData[no].lockU3,&lkSSData[no].lockIT2,&Impd_T,TRUE1,CF_pnZ);	//����T���迹
		CaluRX(&lkSSData[no].lockU4,&lkSSData[no].lockIF2,&Impd_F,TRUE1,CF_pnZ);
	}
	else if(lkSSData[no].exchangeFlag == I3I2I1)
	{
		CaluRX(&lkSSData[no].lockU1,&lkSSData[no].lockIT3,&Impd_T,TRUE1,CF_pnZ);	//����T���迹
		CaluRX(&lkSSData[no].lockU2,&lkSSData[no].lockIF3,&Impd_F,TRUE1,CF_pnZ);
	}
	else if(lkSSData[no].exchangeFlag == I3I1I2)
	{
		CaluRX(&lkSSData[no].lockU3,&lkSSData[no].lockIT3,&Impd_T,TRUE1,CF_pnZ);	//����T���迹
		CaluRX(&lkSSData[no].lockU4,&lkSSData[no].lockIF3,&Impd_F,TRUE1,CF_pnZ);
	}

	WriteRMData(RM_pnR_T,RM_R_T,Impd_T.Real);
	WriteRMData(RM_pnX_T,RM_X_T,Impd_T.Imag);
	WriteRMData(RM_pnR_F,RM_R_F,Impd_F.Real);
	WriteRMData(RM_pnX_F,RM_X_F,Impd_F.Imag);
	WriteRMData(RM_pnR_TF,RM_R_TF,lkSSData[no].lockImpd.Real);
	WriteRMData(RM_pnX_TF,RM_X_TF,lkSSData[no].lockImpd.Imag);
}
//���㹫���
void CRelayTask::CaluKM()
{
	if(*RS_pnSD == 0)
		*RM_pnKMS = (*RS_pnS0)+(*RM_pnDist)-(*RS_pnLc);
	else
		*RM_pnKMS = (*RS_pnS0)-(*RM_pnDist)+(*RS_pnLc);
	DB_LogWriteRM(RM_KMS, *RM_pnKMS);
}
LONG CRelayTask::MAX3(LONG x,LONG y,LONG z)
{
	 return (x>=y?(x>=z?x:z):(y>=z?y:z));
}
LONG CRelayTask::MIN3(LONG x,LONG y,LONG z)
{
	 return (x>=y?(y>=z?z:y):(x>=z?z:x));
}
LONG CRelayTask::MAX2(LONG x,LONG y)
{
	 return x>=y? x:y;
}
//�������ѹ����Сֵ
LONG CRelayTask::MIN2(LONG x,LONG y)
{
	 return x>=y? y:x;
}
LONG CRelayTask::MAX4(LONG x,LONG y,LONG z,LONG w)
{
	LONG te;
	te = MAX3(x,y,z);
	te = MAX2(te,w);
	return te;
}
LONG CRelayTask::MAX5(LONG x,LONG y,LONG z,LONG w,long v)
{
	LONG t1,t2;
	t1=MAX3(x,y,z);
	t2=MAX2(w,v);
	return MAX2(t1,t2);
}
void CRelayTask::checkSelfGps()
{
	if(*CFG_pbyCHECKGPS == CFG_STATUS_CLR)
		return;
	if(DAE_GetRunningFlags(SS_TIME_B)||((IrigQuality&0xFF)>0x08))
	{
		if(!haveMakeGPSERRRpt)
		{
			WriteRelayWord(RW_GPSERR,RELAY_ON);
			MakeAlarmRpt(R_GPSERR);
			haveMakeGPSERRRpt = TRUE;
		}
	}
	else
	{
		WriteRelayWord(RW_GPSERR,RELAY_OFF);
		haveMakeGPSERRRpt = FALSE;
	}
}

//״̬�ָ���λ��˵��
//��һ�ֽڱ�ʾGPSʱ������
//�ڶ��ֽ����λ��ʾGPS�Ƿ���Ч
//�ڶ��ֽڵڶ�λ��ʾ����������Ƿ���Ч
//�ڶ��ֽ�����λ����
//�����ֽڼ�¼������ǰ8·���⿪���״̬
//�����ֽڱ���
DWORD CRelayTask::GetGPSStatus(STATUS flag,BYTE CB)
{
	DWORD status = 0;
	if(flag == ERR_SUCCESS)    //ȡ���ɹ�
		status |= 0x200;
	
	if(DAE_GetRunningFlags(SS_TIME_B)) //��B��ʽ�ź�
	{
		status &= 0xfffffeff;
		status |= 0xFF;   //��B��ʽ�źţ���ʱ�������������ֵ
	}
	else//B��ʽ�ź�����		
	{
		status |= 0x100;
		status |= (IrigQuality&0xFF); 
	}
	
		
	DWORD temp = ((DWORD)CB<<16);
	status |= temp;
	
 return status;
}


/************************************************************************************************
����: ����ʱ����ٻ�����ȡ����������
����: ��
����: ��
��ע:
************************************************************************************************/

void CRelayTask::OnGetFLData(WORD arg1,DWORD arg2,DWORD dataType)
{
	STATUS rc;	
	BYTE	Type = LOBYTE(dataType);
	BYTE	Seq = HIBYTE(dataType);
	BYTE 	reportSerialNum;
	TAbsTime time;
	//�����������������	
	ReadAbsTime(&PickUpTime);
	DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&PickUpTime,FALSE); 
	reportSerialNum = g_RptSerialNum;
	MakeActRpt(R_FLPICKUP,reportSerialNum);
	DB_RelayEnd(reportSerialNum);
	//������ද���̵�������λ
	if(Type == FLTASK_MODE_AUTOFL)
		WriteRelayWord(RW_AFLACT,RELAY_ON);
	if(Type == FLTASK_MODE_MANUALFL)
		WriteRelayWord(RW_MFLACT,RELAY_ON);



	TAbsTime CurTime;		
	QWORD	 time1,time2;
	DWORD	 time3; 	
	DWORD	 dwSamplePtr;
	BOOL     validFlag = FALSE;
	
	dwSamplePtr = dwADDataWritePtr; 	
	ReadAbsTime(&CurTime);
	time1 = (((QWORD)CurTime.Hi)<<32) + CurTime.Lo; //����ϵͳ��ǰʱ��
	time2 = (((QWORD)arg1)<<32) + arg2; //��ഥ��ʱ��

	//�ϴ��������㣬ȡ�����ɹ�ʱ�ϴ�0
	subU1.Mod = 0;
	subU2.Mod = 0;
	subU3.Mod = 0;
	subU4.Mod = 0;
	subIT1.Mod = 0;
	subIF1.Mod = 0;
	subIT2.Mod = 0;
	subIF2.Mod = 0;
	subIT3.Mod = 0;
	subIF3.Mod = 0;
	subIAT1.Mod = 0;
	subIAT2.Mod = 0;
	subIT1.Angle = 0;
	subIF1.Angle = 0;
	subIT2.Angle = 0;
	subIF2.Angle = 0;
	subIT3.Angle = 0;
	subIF3.Angle = 0;
	subIp1.Mod = 0;
	subIp2.Mod = 0;
	subIp3.Mod = 0;
	
	LONG tCB = 0;

	//�ж�ʱ��ĺ����ԣ���ȡ����ֵ����ָ��ʱ�̵�����
	if(((Type == FLTASK_MODE_AUTOFL)||(Type == FLTASK_MODE_MANUALFL))&&(time2 <= time1))
	{
		time.Hi = arg1;
		time.Lo = arg2;
		rc = ReadParticularTimeData(dwTraceDataBuf,time);
		if(rc != ERR_SUCCESS)
			validFlag = FALSE;
		else
		{
			CaluBaseFourier_20(dwTraceDataBuf,&subU1);
			CaluModValue(&subU1);	
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM,&subU2);
			CaluModValue(&subU2);
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*2,&subU3);
			CaluModValue(&subU3);
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*3,&subU4);
			CaluModValue(&subU4);

			//�����������ϳɷ�ʽ�£����е����ϳ�
			if(*CFG_pbyCCMODE == CFG_STATUS_SET) 
			{
				for(int i=0;i<READ_POINT_NUM;i++)
				{
					//�ϳ�����TF�ߵ���
					dwSubCCModeDataBuf[i] = (dwTraceDataBuf[i+READ_POINT_NUM*4]*dwCTRateCoef1+dwTraceDataBuf[i+READ_POINT_NUM*8]*1000)/1000;
					dwSubCCModeDataBuf[i+READ_POINT_NUM] = (dwTraceDataBuf[i+READ_POINT_NUM*5]*dwCTRateCoef1+dwTraceDataBuf[i+READ_POINT_NUM*9]*1000)/1000;
					//�ϳ�����TF�ߵ���
					dwSubCCModeDataBuf[i+READ_POINT_NUM*2] = (dwTraceDataBuf[i+READ_POINT_NUM*6]*dwCTRateCoef2+dwTraceDataBuf[i+READ_POINT_NUM*8]*1000)/1000;
					dwSubCCModeDataBuf[i+READ_POINT_NUM*3] = (dwTraceDataBuf[i+READ_POINT_NUM*7]*dwCTRateCoef2+dwTraceDataBuf[i+READ_POINT_NUM*9]*1000)/1000;

					dwSubCCModeDataBuf[i+READ_POINT_NUM*4] = dwSubCCModeDataBuf[i]-dwSubCCModeDataBuf[i+READ_POINT_NUM]; //�ϳ�����IP
					dwSubCCModeDataBuf[i+READ_POINT_NUM*5] = dwSubCCModeDataBuf[i+READ_POINT_NUM*2]-dwSubCCModeDataBuf[i+READ_POINT_NUM*3]; //�ϳ�����IP
				}
				
				CaluBaseFourier_20(dwSubCCModeDataBuf,&subIT1);
				CaluModValue(&subIT1);
				CaluAngle(&subIT1,AngleTab);	
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM,&subIF1);
				CaluModValue(&subIF1);
				CaluAngle(&subIF1,AngleTab);
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM*2,&subIT2);
				CaluModValue(&subIT2);
				CaluAngle(&subIT2,AngleTab);
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM*3,&subIF2);
				CaluModValue(&subIF2);
				CaluAngle(&subIF2,AngleTab);
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM*4,&subIp1);
				CaluModValue(&subIp1);
				CaluAngle(&subIp1,AngleTab);				
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM*5,&subIp2);
				CaluModValue(&subIp2);
				CaluAngle(&subIp2,AngleTab);
			}
			else
			{
				for(int i=0;i<READ_POINT_NUM;i++)
				{
					//�ϳ�����ip����
					dwSubCCModeDataBuf[i] = dwTraceDataBuf[i+READ_POINT_NUM*4] - dwTraceDataBuf[i+READ_POINT_NUM*5];
					//�ϳ�����IP����
					dwSubCCModeDataBuf[i+READ_POINT_NUM] = dwTraceDataBuf[i+READ_POINT_NUM*6] - dwTraceDataBuf[i+READ_POINT_NUM*7];
					//�ϳ�3�����ߵ�IP����
					dwSubCCModeDataBuf[i+READ_POINT_NUM*2] = dwTraceDataBuf[i+READ_POINT_NUM*8] - dwTraceDataBuf[i+READ_POINT_NUM*9];
				}
				CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*4,&subIT1);
				CaluModValue(&subIT1);
				CaluAngle(&subIT1,AngleTab);	
				CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*5,&subIF1);
				CaluModValue(&subIF1);
				CaluAngle(&subIF1,AngleTab);
				CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*6,&subIT2);
				CaluModValue(&subIT2);
				CaluAngle(&subIT2,AngleTab);
				CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*7,&subIF2);
				CaluModValue(&subIF2);
				CaluAngle(&subIF2,AngleTab);
				
				CaluBaseFourier_20(dwSubCCModeDataBuf,&subIp1);
				CaluModValue(&subIp1);
				CaluAngle(&subIp1,AngleTab);				
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM,&subIp2);
				CaluModValue(&subIp2);
				CaluAngle(&subIp2,AngleTab);				
				CaluBaseFourier_20(dwSubCCModeDataBuf+READ_POINT_NUM*2,&subIp3);
				CaluModValue(&subIp3);
				CaluAngle(&subIp3,AngleTab);
			}
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*8,&subIT3);
			CaluModValue(&subIT3);
			CaluAngle(&subIT3,AngleTab);
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*9,&subIF3);
			CaluModValue(&subIF3);
			CaluAngle(&subIF3,AngleTab);
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*10,&subIAT1);
			CaluModValue(&subIAT1);
			CaluBaseFourier_20(dwTraceDataBuf+READ_POINT_NUM*11,&subIAT2);
			CaluModValue(&subIAT2);
			
			subIAT.Real = subIAT1.Real+subIAT2.Real;
			subIAT.Imag = subIAT1.Imag+subIAT2.Imag;
			CaluModValue(&subIAT);
			//���ر任��ֱ��ȡ��ָ��ʱ��ֵ����
			tCB =(BYTE) dwTraceDataBuf[READ_POINT_NUM*16-1];
			ReportMsg("CB= %d",tCB);
			//������Ч��־
			validFlag = TRUE;
		}
	}
	//�������������뵽��Լ�ϴ���������
	UploadData.U1 = subU1.Mod*(*AS_pnUPTRatio)/(*CF_pnU1);  //����Ϊ����������ֵΪһ�ε�ѹ(kv)�Ŵ�100��
	UploadData.U2 = subU2.Mod*(*AS_pnUPTRatio)/(*CF_pnU2);
	UploadData.U3 = subU3.Mod*(*AS_pnUPTRatio)/(*CF_pnU3);
	UploadData.U4 = subU4.Mod*(*AS_pnUPTRatio)/(*CF_pnU4);
	UploadData.It1 = subIT1.Mod*(*AS_pnI1CTRatio)/(*CF_pnIT1);
	UploadData.If1 = subIF1.Mod*(*AS_pnI1CTRatio)/(*CF_pnIF1);
	UploadData.It2 = subIT2.Mod*(*AS_pnI2CTRatio)/(*CF_pnIT2);
	UploadData.If2 = subIF2.Mod*(*AS_pnI2CTRatio)/(*CF_pnIF2);
	UploadData.It3 = subIT3.Mod*(*AS_pnI3CTRatio)/(*CF_pnIT3);
	UploadData.If3 = subIF3.Mod*(*AS_pnI3CTRatio)/(*CF_pnIF3);
	UploadData.Iat1 = subIAT1.Mod*(*AS_pnIATCTRatio)/(*CF_pnIAT1);
	UploadData.Iat2 = subIAT2.Mod*(*AS_pnIATCTRatio)/(*CF_pnIAT2);
	UploadData.Iat = subIAT.Mod*(*AS_pnIATCTRatio)/(*CF_pnIAT2);
	UploadData.AIt1 = 0;
	UploadData.AIf1 = subIF1.Angle - subIT1.Angle;
	UploadData.AIt2 = subIT2.Angle - subIT1.Angle;
	UploadData.AIf2 = subIF2.Angle - subIT1.Angle;
	UploadData.AIt3 = subIT3.Angle - subIT1.Angle;
	UploadData.AIf3 = subIF3.Angle - subIT1.Angle;
	UploadData.Ip1  = subIp1.Mod*(*AS_pnI1CTRatio)/(*CF_pnIT1);  //2014-10-23 16:19:20���������������������ϴ���yanzh
	UploadData.Ip2  = subIp2.Mod*(*AS_pnI2CTRatio)/(*CF_pnIT2);
	UploadData.Ip3  = subIp3.Mod*(*AS_pnI3CTRatio)/(*CF_pnIT3);
	
	UploadData.ComplicateNo = Seq;
	UploadData.GPSStatus = GetGPSStatus(rc,tCB);
	ReportMsg("sub GPS status = %x",UploadData.GPSStatus);
	//����ʷ���ݻ������б���һ�ݱ��ݣ��Ա��ٻ���ʷ����ʱʹ��
	if(dataType == FLTASK_MODE_AUTOFL)
		(*g_pHistoryData) = UploadData ;

	//��������¼���������Լ����������OK����Ϣ
	if(validFlag == TRUE)
	{
		//��������¼��
		if((Type == FLTASK_MODE_AUTOFL)&&(time1-time2<=3598000))
		{
			time3 = time1-time2;			
			dwSamplePtr = (SAMPLE_DATA_LEN+dwSamplePtr-time3*CHANNNEL_NUM)%SAMPLE_DATA_LEN; 			
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DBASE), SM_STARTDIST, dwSamplePtr,*AS_pnRECD,time.Lo,time.Hi);
		}
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_DATAISOK,(DWORD)&UploadData,TRUE,0,0);		
	}
	else
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_DATAISOK,(DWORD)&UploadData,FALSE,0,0);
	
}
void CRelayTask::LockDataForLowVol()//����ѹ����һ��ֵʱ��������
{
	if(*CFG_pbyCALLDATA == CFG_STATUS_CLR) //��ѹ��������Ͷ��
		return;
		
	if(m_U1.Mod >= *RS_pn27U)
	{
		lastVolHaveMark = FALSE;
		lowVolCounter = 0;
		return;
	}
	else
	{
		if(lastVolHaveMark == TRUE)
			return;
		
		lowVolCounter += lowVolPerTime;
		if(lowVolCounter >= lowVolTimeSet)
		{
			if(*CFG_pbyISSS == CFG_STATUS_SET)
			{
				g_pSS_LockLowVoltage->subData.U1 = m_U1.Mod;
				g_pSS_LockLowVoltage->subData.U2 = m_U2.Mod;
				g_pSS_LockLowVoltage->subData.U3 = m_U3.Mod;
				g_pSS_LockLowVoltage->subData.U4 = m_U4.Mod;
				g_pSS_LockLowVoltage->subData.IT1 = m_IT1.Mod;
				g_pSS_LockLowVoltage->subData.IF1 = m_IF1.Mod;
				g_pSS_LockLowVoltage->subData.IT2 = m_IT2.Mod;
				g_pSS_LockLowVoltage->subData.IF2 = m_IF2.Mod;
				g_pSS_LockLowVoltage->subData.IT3 = m_IT3.Mod;
				g_pSS_LockLowVoltage->subData.IF3 = m_IF3.Mod;
				g_pSS_LockLowVoltage->subData.IAT1 = m_IAT1.Mod;
				g_pSS_LockLowVoltage->subData.IAT2 = m_IAT2.Mod;
				g_pSS_LockLowVoltage->subData.IAT = m_IAT_SS;
				g_pSS_LockLowVoltage->subData.AU1 = m_U1.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AU2 = m_U2.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AU3 = m_U3.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AU4 = m_U4.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AIT1 = 0;
				g_pSS_LockLowVoltage->subData.AIF1 = m_IF1.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AIT2 = m_IT2.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AIF2 = m_IF2.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AIT3 = m_IT3.Angle - m_IT1.Angle;
				g_pSS_LockLowVoltage->subData.AIF3 = m_IF3.Angle - m_IT1.Angle;
				//����ʸ�������ڵ翹�����
				g_pSS_LockLowVoltage->lockU1 = m_U1; 
				g_pSS_LockLowVoltage->lockU2 = m_U2;
				g_pSS_LockLowVoltage->lockU3 = m_U3;
				g_pSS_LockLowVoltage->lockU4 = m_U4;
				g_pSS_LockLowVoltage->lockIT1 = m_IT1;
				g_pSS_LockLowVoltage->lockIF1 = m_IF1;
				g_pSS_LockLowVoltage->lockIT2 = m_IT2;
				g_pSS_LockLowVoltage->lockIF2 = m_IF2;
				g_pSS_LockLowVoltage->lockIT3 = m_IT3;
				g_pSS_LockLowVoltage->lockIF3 = m_IF3;
				g_pSS_LockLowVoltage->lockIAT1 = m_IAT1;
				g_pSS_LockLowVoltage->lockIAT2 = m_IAT2; 	
				g_pSS_LockLowVoltage->lockUp1 = m_UP1;
				g_pSS_LockLowVoltage->lockUp2 = m_UP2;
				g_pSS_LockLowVoltage->lockIp1 = m_IP1;
				g_pSS_LockLowVoltage->lockIp2 = m_IP2;
				g_pSS_LockLowVoltage->lockIp3 = m_IP3;
				g_pSS_LockLowVoltage->lockImpd = m_Impd;
				g_pSS_LockLowVoltage->exchangeFlag = exchangeElecFlag;
			}
			else
			{
				g_pLowValData->U1 = m_U1mod/(*CF_pnU1);
				g_pLowValData->U2 = m_U2mod/(*CF_pnU1);
				g_pLowValData->U3 = m_U3mod/(*CF_pnU1);
				g_pLowValData->U4 = m_U4mod/(*CF_pnU1);
				g_pLowValData->It1 = m_IT1mod/(*CF_pnIT1);
				g_pLowValData->If1 = m_IF1mod/(*CF_pnIF1);
				g_pLowValData->It2 = m_IT2mod/(*CF_pnIT2);
				g_pLowValData->If2 = m_IF2mod/(*CF_pnIF2);
				g_pLowValData->It3 = m_IT3mod/(*CF_pnIT3);
				g_pLowValData->If3 = m_IF3mod/(*CF_pnIF3);
				g_pLowValData->Iat1 = m_IAT1mod/(*CF_pnIAT1);
				g_pLowValData->Iat2 = m_IAT2mod/(*CF_pnIAT2);
				g_pLowValData->Iat = m_IAT_SS/(*CF_pnIAT1);
				g_pLowValData->AIt1 = m_IT1.Angle;
				g_pLowValData->AIf1 = m_IF1.Angle;
				g_pLowValData->AIt2 = m_IT2.Angle;
				g_pLowValData->AIf2 = m_IF2.Angle;
				g_pLowValData->AIt3 = m_IT3.Angle;
				g_pLowValData->AIf3 = m_IF3.Angle;
				g_pLowValData->GPSStatus = GetGPSStatus(ERR_SUCCESS,m_CB);
			}
			lastVolHaveMark = TRUE;
		}
	}
		
}
/***********************************************************************************
//���������Ĳ��
//��ģʽ�£�����������Ӧ���жϣ��Թ̶�ģʽ(ȫ����)���в�����
***********************************************************************************/
void CRelayTask::CalcFLWith3Sub(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	DWORD temp1,temp2,temp3,temp4,temp5,temp6=0;
	LONG maxIp,maxIat;
	LONG totalfeedout;	
	LONG hl_t[4];
	LONG hl_f[4];
	//�����������
	for(int i=0;i<4;i++)
	{
		hl_t[i] = ToAbs(tsubData[i].IT1-tsubData[i].IT2);
		hl_f[i] = ToAbs(tsubData[i].IF1-tsubData[i].IF2);
	}
	//�����ж��Ƿ�ΪTF����
	maxIp = MAX4(tsubData[0].IT1,tsubData[0].IT2,tsubData[0].IF1,tsubData[0].IF2);
	maxIat = MAX4(tsubData[0].IAT,tsubData[1].IAT,tsubData[2].IAT,tsubData[3].IAT);
	totalfeedout = tsubData[0].IT1+tsubData[0].IF1+tsubData[0].IT2+tsubData[0].IF2;

	if(maxIp*100>=*RS_pnKF*maxIat) 
	{
		LONG dist1,dist2;
		//��TF��������£�ʹ��T�ߺ�F�ߵĺ������������жϹ�������
		temp1 = MAX4(hl_t[0]+hl_f[0],hl_t[1]+hl_f[1],hl_t[2]+hl_f[2],hl_t[3]+hl_f[3]);  
		if(temp1 == (hl_t[0]+hl_f[0]))
		{
			//���������������󣬹���λ�ڵ�һ����
			dist1 = 0;
			dist2 = *RS_pnD1;
			temp2 = tsubData[0].IT1+tsubData[0].IF1;
			temp3 = tsubData[0].IT2+tsubData[0].IF2;
		}
		else if(temp1 == (hl_t[1]+hl_f[1]))
		{
			//�ڶ�����������������󣬹���λ�ڵ�һ�����ڶ�����
			if((hl_t[0]+hl_f[0])>=(hl_t[1]+hl_f[1]))
			{
				//����λ�ڵ�һ����
				dist1 = 0;
				dist2 = *RS_pnD1;
				temp2 = tsubData[0].IT1+tsubData[0].IF1;
				temp3 = tsubData[0].IT2+tsubData[0].IF2;
			}
			else
			{
				//����λ�ڵڶ�����
				dist1 = *RS_pnD1;
				dist2 = *RS_pnD2;				
				temp2 = tsubData[1].IT1+tsubData[1].IF1;
				temp3 = tsubData[1].IT2+tsubData[1].IF2;
				totalfeedout = tsubData[1].IT1+tsubData[1].IF1+tsubData[1].IT2+tsubData[1].IF2;
			}
		}
		else if(temp1 == (hl_t[2]+hl_f[2]))
		{
			//��������������������󣬹���λ�ڵڶ�������������
			if((hl_t[1]+hl_f[1])>=(hl_t[3]+hl_f[3]))
			{
				//����λ�ڵڶ�����
				dist1 = *RS_pnD1;
				dist2 = *RS_pnD2;				
				temp2 = tsubData[1].IT1+tsubData[1].IF1;
				temp3 = tsubData[1].IT2+tsubData[1].IF2;				
				totalfeedout = tsubData[1].IT1+tsubData[1].IF1+tsubData[1].IT2+tsubData[1].IF2;
			}
			else
			{
				//����λ�ڵ�������
				dist1 = *RS_pnD1+(*RS_pnD2);
				dist2 = *RS_pnD3;				
				temp2 = tsubData[3].IT1+tsubData[3].IF1;
				temp3 = tsubData[3].IT2+tsubData[3].IF2;
			}
		}
		else
		{
			//��������������������󣬹���λ�ڵ�������
			dist1 = *RS_pnD1+(*RS_pnD2);
			dist2 = *RS_pnD3;				
			temp2 = tsubData[3].IT1+tsubData[3].IF1;
			temp3 = tsubData[3].IT2+tsubData[3].IF2;
		}
		
		temp4 = MIN2(temp2,temp3);
		*RM_pnDist = dist1+ ((LLONG)temp4*2*dist2)/totalfeedout;			
	}


	//ʹ�ú������������жϹ�������
	temp1 = MAX4(hl_t[0],hl_t[1],hl_t[2],hl_t[3]);
	temp2 = MAX4(hl_f[0],hl_f[1],hl_f[2],hl_f[3]);
	if(temp1 >= temp2)
	{
		if((temp1 == hl_t[0])||((temp1 == hl_t[1])&&(hl_t[0]>hl_t[2])))
			faultRange = RANGE1;
		else if(((temp1 == hl_t[1])&&(hl_t[2]>hl_t[1]))||((temp1 == hl_t[2])&&(hl_t[1]>hl_t[3])))
			faultRange = RANGE2;
		else 
			faultRange = RANGE3;		
	}
	else
	{
		if((temp1 == hl_f[0])||((temp1 == hl_f[1])&&(hl_f[0]>hl_f[2])))
			faultRange = RANGE1;
		else if(((temp1 == hl_f[1])&&(hl_f[2]>hl_f[1]))||((temp1 == hl_f[2])&&(hl_f[1]>hl_f[3])))
			faultRange = RANGE2;
		else 
			faultRange = RANGE3;		
	}
	//���ݹ����������˵�8�������жϹ������ͼ�����
	if(faultRange == RANGE1)
	{
		//����λ�ڵ�һ���䣬ʹ�÷ֶ������е����Ƚ��в��
		temp1 = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[1].IT1,tsubData[1].IF1);
		temp2 = MAX4(tsubData[0].IT2,tsubData[0].IF2,tsubData[1].IT2,tsubData[1].IF2);
		if(temp1>=temp2)
		{			
			faultDirection = DOWNLINE;
			if((temp1 == tsubData[0].IT1)||(temp1 == tsubData[1].IT1))
			{				
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp1 == tsubData[0].IT2)||(temp1 == tsubData[1].IT2))
			{				
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}
		
		//ʹ�÷ֶε������е����Ƚ��в��
		temp1 = tsubData[0].IT1+tsubData[0].IF1;
		temp2 = tsubData[0].IT2+tsubData[0].IF2;
		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
	}
	else if(faultRange == RANGE2)
	{
		//����λ�ڵڶ����䣬ʹ�����ϵ����Ƚ��в��
		temp1 = MAX4(tsubData[1].IT1,tsubData[1].IF1,tsubData[2].IT1,tsubData[2].IF1);
		temp2 = MAX4(tsubData[1].IT2,tsubData[1].IF2,tsubData[2].IT2,tsubData[2].IF2);
		if(temp1>=temp2)
		{			
			faultDirection = DOWNLINE;
			if((temp1 == tsubData[1].IT1)||(temp1 == tsubData[2].IT1))
			{				
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp1 == tsubData[1].IT2)||(temp1 == tsubData[2].IT2))
			{				
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}
		
		temp1 = tsubData[2].IAT*DistCoef[2];
		temp2 = tsubData[1].IAT*DistCoef[1];
		temp3 = temp1+temp2; 
		temp4 = 10000-QValue[1]-QValue[2];
		if(temp4!=0&&temp3!=0)
		{
			temp5 = (((LLONG)temp1*10000)/temp3)-QValue[1];
			temp6 = ((LLONG)temp5*(*RS_pnD2))/temp4;		
		}
		*RM_pnDist = *RS_pnD1+temp6;		
	}
	else
	{
		//����λ�ڵ������䣬ʹ�÷ֶ������е������в��
		//����λ�ڵ�һ���䣬ʹ�÷ֶ������е����Ƚ��в��
		temp1 = MAX4(tsubData[2].IT1,tsubData[2].IF1,tsubData[3].IT1,tsubData[3].IF1);
		temp2 = MAX4(tsubData[2].IT2,tsubData[2].IF2,tsubData[3].IT2,tsubData[3].IF2);
		if(temp1>=temp2)
		{			
			faultDirection = DOWNLINE;
			if((temp1 == tsubData[2].IT1)||(temp1 == tsubData[3].IT1))
			{				
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp1 == tsubData[2].IT2)||(temp1 == tsubData[3].IT2))
			{				
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}
		
		//ʹ�÷ֶε������е����Ƚ��в��
		temp1 = tsubData[3].IT1+tsubData[3].IF1;
		temp2 = tsubData[3].IT2+tsubData[3].IF2;
		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = *RS_pnD1+*RS_pnD2+((LLONG)*RS_pnD3*2*temp3)/totalfeedout;
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);	
}
/***********************************************************************************
//Խ��ģʽ�µĲ��
ʵ��Ϊ5�������Ĳ�࣬��������۸�������2�����������
Խ��ʱ���������е���̨���װ�ú�Ϊһ��ʹ��
�Բ�����ʵ���˳����У������ϴ�Ϊ0
��˸�ģʽʵ��Ϊ3�������Ĳ�࣬���һ������Ϊֱ����ʽ
***********************************************************************************/
void CRelayTask::CalcFLWithHandOver(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	DWORD temp1,temp2,temp3,temp4,temp5,temp6=0;
	LONG maxIp,maxIat;
	LONG totalfeedout;	
	LONG hl_t[4];
	LONG hl_f[4];
	//�����������
	for(int i=0;i<4;i++)
	{
		hl_t[i] = ToAbs(tsubData[i].IT1-tsubData[i].IT2);
		hl_f[i] = ToAbs(tsubData[i].IF1-tsubData[i].IF2);
	}
	//�����ж��Ƿ�ΪTF����
	maxIp = MAX4(tsubData[0].IT1,tsubData[0].IT2,tsubData[0].IF1,tsubData[0].IF2);
	maxIat = MAX4(tsubData[0].IAT,tsubData[1].IAT,tsubData[2].IAT,tsubData[3].IAT);
	totalfeedout = tsubData[0].IT1+tsubData[0].IF1+tsubData[0].IT2+tsubData[0].IF2;

	if(maxIp*100>=*RS_pnKF*maxIat) 
	{
		LONG dist1,dist2;
		//��TF��������£�ʹ��T�ߺ�F�ߵĺ������������жϹ�������
		temp1 = MAX4(hl_t[0]+hl_f[0],hl_t[1]+hl_f[1],hl_t[2]+hl_f[2],hl_t[3]+hl_f[3]);  
		//����һ��ֱ�����䣬��˲���ֱ���Ժ�������֮�͵ľ���ֵ���жϹ������䣬
		//���������������������ܴ󣬼�������������T��֮���F��֮�������ֵ�ϴ������
		if((temp1 == (hl_t[0]+hl_f[0]))&&((!IsAEqualB(tsubData[0].IT1,tsubData[0].IT2))|| \
			(!IsAEqualB(tsubData[0].IF1,tsubData[0].IF2))))
		{
			//���������������󣬹���λ�ڵ�һ����
			dist1 = 0;
			dist2 = *RS_pnD1;
			temp2 = tsubData[0].IT1+tsubData[0].IF1;
			temp3 = tsubData[0].IT2+tsubData[0].IF2;
		}
		else if((temp1 == (hl_t[1]+hl_f[1]))&&((!IsAEqualB(tsubData[1].IT1,tsubData[1].IT2))|| \
			(!IsAEqualB(tsubData[1].IF1,tsubData[1].IF2))))
		{
			//�ڶ�����������������󣬹���λ�ڵ�һ�����ڶ�����
			if((hl_t[0]+hl_f[0])>=(hl_t[1]+hl_f[1]))
			{
				//����λ�ڵ�һ����
				dist1 = 0;
				dist2 = *RS_pnD1;
				temp2 = tsubData[0].IT1+tsubData[0].IF1;
				temp3 = tsubData[0].IT2+tsubData[0].IF2;
			}
			else
			{
				//����λ�ڵڶ�����
				dist1 = *RS_pnD1;
				dist2 = *RS_pnD2;				
				temp2 = tsubData[1].IT1+tsubData[1].IF1;
				temp3 = tsubData[1].IT2+tsubData[1].IF2;
				totalfeedout = tsubData[1].IT1+tsubData[1].IF1+tsubData[1].IT2+tsubData[1].IF2;
			}
		}
		else if((temp1 == (hl_t[2]+hl_f[2]))&&((!IsAEqualB(tsubData[2].IT1,tsubData[2].IT2))|| \
			(!IsAEqualB(tsubData[2].IF1,tsubData[2].IF2))))
		{
			//��������������������󣬹���λ�ڵڶ�������������
			if((hl_t[1]+hl_f[1])>=(hl_t[3]+hl_f[3]))
			{
				//����λ�ڵڶ�����
				dist1 = *RS_pnD1;
				dist2 = *RS_pnD2;				
				temp2 = tsubData[1].IT1+tsubData[1].IF1;
				temp3 = tsubData[1].IT2+tsubData[1].IF2;				
				totalfeedout = tsubData[1].IT1+tsubData[1].IF1+tsubData[1].IT2+tsubData[1].IF2;
			}
			else
			{
				//����λ�ڵ�������
				dist1 = *RS_pnD1+(*RS_pnD2);
				dist2 = *RS_pnD3;				
				temp2 = tsubData[3].IT1+tsubData[3].IF1;
				temp3 = tsubData[3].IT2+tsubData[3].IF2;
			}
		}
		else if((temp1 == (hl_t[3]+hl_f[3]))&&((!IsAEqualB(tsubData[3].IT1,tsubData[3].IT2))|| \
			(!IsAEqualB(tsubData[3].IF1,tsubData[3].IF2))))
		{
			//��������������������󣬹���λ�ڵ�������
			dist1 = *RS_pnD1+(*RS_pnD2);
			dist2 = *RS_pnD3;				
			temp2 = tsubData[3].IT1+tsubData[3].IF1;
			temp3 = tsubData[3].IT2+tsubData[3].IF2;
		}
		else
		{
			//����������Ȼ�ܱȽϳ���С��������ֵ����Խ�С������Ϊ���Ϸ�����ֱ������			
			dist1 = *RS_pnD1+(*RS_pnD2)+(*RS_pnD3);
			dist2 = *RS_pnD4;				
			temp2 = 1;
			temp3 = 1;
			totalfeedout = 2;
		}
		temp4 = MIN2(temp2,temp3);
		*RM_pnDist = dist1+ ((LLONG)temp4*2*dist2)/totalfeedout;			
	}


	//ʹ�ú������������жϹ�������
	//�ж�ԭ���ǣ��ҳ��������������Ҹú���������ֵ�ϴ�
	temp1 = MAX4(hl_t[0],hl_t[1],hl_t[2],hl_t[3]);
	temp2 = MAX4(hl_f[0],hl_f[1],hl_f[2],hl_f[3]);
	if(temp1 >= temp2)
	{
		if(((temp1 == hl_t[0])&&(!IsAEqualB(tsubData[0].IT1,tsubData[0].IT2)))|| \
			((temp1 == hl_t[1])&&((!IsAEqualB(tsubData[1].IT1,tsubData[1].IT2)))&&(hl_t[0]>hl_t[2])))
			faultRange = RANGE1;
		else if(((temp1 == hl_t[1])&&((!IsAEqualB(tsubData[1].IT1,tsubData[1].IT2)))&&(hl_t[2]>hl_t[1]))|| \
			((temp1 == hl_t[2])&&((!IsAEqualB(tsubData[2].IT1,tsubData[2].IT2)))&&(hl_t[1]>hl_t[3])))
			faultRange = RANGE2;
		else if((temp1 == hl_t[2])&&((!IsAEqualB(tsubData[3].IT1,tsubData[3].IT2))))
			faultRange = RANGE3;
		else
			faultRange = RANGE4;
	}
	else
	{		
		if(((temp1 == hl_f[0])&&(!IsAEqualB(tsubData[0].IF1,tsubData[0].IF2)))|| \
			((temp1 == hl_f[1])&&((!IsAEqualB(tsubData[1].IF1,tsubData[1].IF2)))&&(hl_f[0]>hl_f[2])))
			faultRange = RANGE1;
		else if(((temp1 == hl_f[1])&&((!IsAEqualB(tsubData[1].IF1,tsubData[1].IF2)))&&(hl_f[2]>hl_f[1]))|| \
			((temp1 == hl_f[2])&&((!IsAEqualB(tsubData[2].IF1,tsubData[2].IF2)))&&(hl_f[1]>hl_f[3])))
			faultRange = RANGE2;
		else if((temp1 == hl_f[2])&&((!IsAEqualB(tsubData[3].IF1,tsubData[3].IF2))))
			faultRange = RANGE3;
		else
			faultRange = RANGE4;

	}
	//���ݹ����������˵�8�������жϹ������ͼ�����
	if(faultRange == RANGE1)
	{
		//����λ�ڵ�һ���䣬ʹ�÷ֶ������е����Ƚ��в��
		temp1 = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[1].IT1,tsubData[1].IF1);
		temp2 = MAX4(tsubData[0].IT2,tsubData[0].IF2,tsubData[1].IT2,tsubData[1].IF2);
		if(temp1>=temp2)
		{			
			faultDirection = DOWNLINE;
			if((temp1 == tsubData[0].IT1)||(temp1 == tsubData[1].IT1))
			{				
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp1 == tsubData[0].IT2)||(temp1 == tsubData[1].IT2))
			{				
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}
		
		//ʹ�÷ֶε������е����Ƚ��в��
		temp1 = tsubData[0].IT1+tsubData[0].IF1;
		temp2 = tsubData[0].IT2+tsubData[0].IF2;
		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
	}
	else if(faultRange == RANGE2)
	{
		//����λ�ڵڶ����䣬ʹ�����ϵ����Ƚ��в��
		temp1 = MAX4(tsubData[1].IT1,tsubData[1].IF1,tsubData[2].IT1,tsubData[2].IF1);
		temp2 = MAX4(tsubData[1].IT2,tsubData[1].IF2,tsubData[2].IT2,tsubData[2].IF2);
		if(temp1>=temp2)
		{			
			faultDirection = DOWNLINE;
			if((temp1 == tsubData[1].IT1)||(temp1 == tsubData[2].IT1))
			{				
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp1 == tsubData[1].IT2)||(temp1 == tsubData[2].IT2))
			{				
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}
		
		temp1 = tsubData[2].IAT*DistCoef[2];
		temp2 = tsubData[1].IAT*DistCoef[1];
		temp3 = temp1+temp2; 
		temp4 = 10000-QValue[1]-QValue[2];
		if(temp4!=0&&temp3!=0)
		{
			temp5 = (((LLONG)temp1*10000)/temp3)-QValue[1];
			temp6 = ((LLONG)temp5*(*RS_pnD2))/temp4;		
		}
		*RM_pnDist = *RS_pnD1+temp6;		
	}
	else if(faultRange == RANGE3)
	{
		//����λ�ڵ������䣬��Ϊ��������Ϊֱ������
		//����ʹ�÷ֶ������е������в��
		temp1 = MAX4(tsubData[2].IT1,tsubData[2].IF1,tsubData[3].IT1,tsubData[3].IF1);
		temp2 = MAX4(tsubData[2].IT2,tsubData[2].IF2,tsubData[3].IT2,tsubData[3].IF2);
		if(temp1>=temp2)
		{			
			faultDirection = DOWNLINE;
			if((temp1 == tsubData[2].IT1)||(temp1 == tsubData[3].IT1))
			{				
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp1 == tsubData[2].IT2)||(temp1 == tsubData[3].IT2))
			{				
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}
		
		//ʹ�÷ֶε������е����Ƚ��в��
		temp1 = tsubData[3].IT1+tsubData[3].IF1;
		temp2 = tsubData[3].IT2+tsubData[3].IF2;
		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = *RS_pnD1+*RS_pnD2+((LLONG)*RS_pnD3*2*temp3)/totalfeedout;
	}
	else
	{
		//����λ�ڵ������䣬������Ϊֱ����ʽ����
		//��ʱ�޲�෽�������Ͼ��븳���ֵ
		*RM_pnDist = *RS_pnD1+*RS_pnD2+*RS_pnD3+*RS_pnD3;		
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);	
	
}

/***********************************************************************************
//�������У����������֮��û�й�ϵ������Ҫ�ж������л������з����Ĺ���
//�����������������뱣����������Ϊ����������������·��������
//������������ʱ��ֱ���ж�������·������������������
***********************************************************************************/
int CRelayTask::WhichShortcircuit(TSubData *tsubData,WORD no)
{	
	if((lkSSData[no].relaytype == Relay50)||(lkSSData[no].relaytype == Relay21))
	{
		//�������������������������з�������
		if((tsubData[0].IT1+tsubData[0].IF1) >= (tsubData[0].IT2+tsubData[0].IF2))
			return DOWNLINE;
		else
			//���з�������
			return UPLINE;
	}
	else if(lkSSData[no].relaytype == Relay50D1)
		return DOWNLINE;
	else
		return UPLINE;
}

/***********************************************************************************
ȫ������ʽ�µĹ��ϲ�࣬�������������
***********************************************************************************/
void CRelayTask::CalcFLMode_0(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	//�����ж��Ƿ���TF����
	
	LONG	maxIp,maxIat;
	maxIp = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[0].IT2,tsubData[0].IF2);
	maxIat = MAX3(tsubData[0].IAT,tsubData[1].IAT,tsubData[2].IAT);
	
	DWORD	hl_t[3];
	DWORD	hl_f[3];
	DWORD	temp1,temp2,temp3;
	DWORD	totalfeedout;
	LONG	dist1,dist2;		

	//�����������
	for(WORD i=0;i<3;i++)
	{
		hl_t[i] = ToAbs(tsubData[i].IT1-tsubData[i].IT2);
		hl_f[i] = ToAbs(tsubData[i].IF1-tsubData[i].IF2);
	}
	//����������
	//totalfeedout = tsubData[0].IT1+tsubData[0].IF1+tsubData[0].IT2+tsubData[0].IF2;
	totalfeedout = tsubData[0].IP1+ tsubData[0].IP2; //2014-10-27 11:45:40 ����������Ϊ������ʸ����֮��

	//����TF���ϵ��оݣ������ж�ϵ����ֵ��ϵ��Ϊ10
	if(maxIp*100>=*RS_pnKF*maxIat) 
	{
		faultType = T_F;	 
		faultINF = TFFaultDir(tsubData);
		
		//���ݺ��������жϹ�������
		if((hl_t[0]+hl_f[0])>(hl_t[2]+hl_f[2]))
		{
			faultRange = RANGE1;
			dist1 = 0;
			dist2 = *RS_pnD1;
//			temp1 = tsubData[0].IT1+tsubData[0].IF1;  
//			temp2 = tsubData[0].IT2+tsubData[0].IF2;
			temp1 = tsubData[0].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
			temp2 = tsubData[0].IP2;
		}
		else
		{
			faultRange = RANGE2;
			dist1 = *RS_pnD1;
			dist2 = *RS_pnD2;
//			temp1 = tsubData[2].IT1+tsubData[2].IF1;
//			temp2 = tsubData[2].IT2+tsubData[2].IF2;
			temp1 = tsubData[2].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
			temp2 = tsubData[2].IP2;
		}			
		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = dist1+ ((LLONG)temp3*2*dist2)/totalfeedout;			
	}
	//��TF����
	else
	{		
		//�������ú����������жϹ�������
		temp1 = MAX3(hl_t[0],hl_t[1],hl_t[2]);
		temp2 = MAX3(hl_f[0],hl_f[1],hl_f[2]);
		temp3 = MAX2(temp1, temp2);
		if((temp3 == hl_t[0])||(temp3 == hl_f[0]))
			faultRange = RANGE1;
		else if((temp3 == hl_t[2])||(temp3 == hl_f[2]))			
			faultRange = RANGE2;
		else
		{
			if(temp3 == hl_t[1])
			{
				if(hl_t[0]>=hl_t[2])
					faultRange = RANGE1;
				else
					faultRange = RANGE2;
			}
			if(temp3 == hl_f[1])
			{
				if(hl_f[0]>=hl_f[2])
					faultRange = RANGE1;
				else
					faultRange = RANGE2;
			}
		}
		//�жϹ��Ϸ������Ͳ�ʹ�������е����Ƚ��в��
		if(faultRange == RANGE1)
		{			
			temp1 = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[1].IT1,tsubData[1].IF1);
			temp2 = MAX4(tsubData[0].IT2,tsubData[0].IF2,tsubData[1].IT2,tsubData[1].IF2);
			//����������������
			if(temp1>=temp2)
			{
				faultDirection = DOWNLINE;
				if((temp1==tsubData[0].IT1)||(temp1==tsubData[1].IT1))
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}
			}
			else
			{
				faultDirection = UPLINE;
				if((temp2==tsubData[0].IT2)||(temp2==tsubData[1].IT2))
				{
					faultType = T_R;
					faultINF = rpt_F_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_F_FR;
				}
			}		
			//ʹ�÷ֶε������е����Ƚ��в��
//			temp1 = tsubData[0].IT1+tsubData[0].IF1;
//			temp2 = tsubData[0].IT2+tsubData[0].IF2;
			temp1 = tsubData[0].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
			temp2 = tsubData[0].IP2;

			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = ((LLONG)*RS_pnD1*2*temp3)/totalfeedout;
		}
		else
		{			
			temp1 = MAX4(tsubData[1].IT1,tsubData[1].IF1,tsubData[2].IT1,tsubData[2].IF1);
			temp2 = MAX4(tsubData[1].IT2,tsubData[1].IF2,tsubData[2].IT2,tsubData[2].IF2);
			if(temp1>temp2)
			{				
				faultDirection = DOWNLINE;
				if((temp1==tsubData[1].IT1)||(temp1==tsubData[2].IT1))
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}
			}
			else
			{
				faultDirection = UPLINE;
				if((temp2==tsubData[1].IT2)||(temp2==tsubData[2].IT2))
				{
					faultType = T_R;
					faultINF = rpt_F_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_F_FR;
				}
			}
			
			//ʹ�÷ֶε������е����Ƚ��в��
//			temp1 = tsubData[2].IT1+tsubData[2].IF1;
//			temp2 = tsubData[2].IT2+tsubData[2].IF2;
			temp1 = tsubData[2].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
			temp2 = tsubData[2].IP2;

			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = *RS_pnD1+((LLONG)*RS_pnD2*2*temp3)/totalfeedout;
		}		
	}	
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}
/***********************************************************************************
����ȫ������ʽ�µĹ��ϲ�࣬�������������
***********************************************************************************/
void CRelayTask::CalcFLMode_1(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{	
	LONG	maxIp,maxIat,totalfeedout;
	maxIp = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[0].IT2,tsubData[0].IF2);
	maxIat = MAX3(tsubData[0].IAT,tsubData[1].IAT,tsubData[2].IAT);
	
	//����������
	//totalfeedout = tsubData[0].IT1+tsubData[0].IF1+tsubData[0].IT2+tsubData[0].IF2;	
	totalfeedout = tsubData[0].IP1+ tsubData[0].IP2; //2014-10-27 11:45:40 ����������Ϊ������ʸ����֮��
	//����TF���ϵ��оݣ������ж�ϵ����ֵ��ϵ��Ϊ10
	//��Ϊ�ǵ������У��˴�Ӧ�öԹ����б�ϵ�������ʵ��ķŴ���ȡ2��
	if(maxIp*100>=*RS_pnKF*2*maxIat) 
	{	
		//����TF�оݣ��迼���ú��ַ������в��
//		m_LImpd_TF = lkSSData[no].lockImpd;
//		CalcFLX(&TF_FaultLocate);	
		faultType = T_F;	
		faultINF = TFFaultDir(tsubData);

		*RM_pnDist = 0;

	}
	else
	{
		//��TF�����£�ֻ��ʹ�����ϵ����Ƚ��������ж�
		if(maxIat == tsubData[0].IAT)
			faultRange = RANGE1;
		else if(maxIat == tsubData[2].IAT)
			faultRange = RANGE2;
		else
		{
			if(tsubData[0].IAT>=tsubData[2].IAT)
				faultRange = RANGE1;
			else
				faultRange = RANGE2;
		}
		//�������λ�ڵ�һ���䣬ֻ��ʹ�����ϵ����Ƚ��в��
		//��λ�ڵڶ����䣬ʹ�÷ֶ������е����Ƚ��в��
		if(faultRange == RANGE1)
		{
			//����λ�ڵ�һ����ʹ��AT�����ĸ������жϹ��Ϸ�������
			//������������·��Ϊ������·
			LONG maxC = MAX4(tsubData[1].IT1,tsubData[1].IT2,tsubData[1].IF1,tsubData[1].IF2);
			if(maxC == tsubData[1].IT1)
			{
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else if(maxC == tsubData[1].IT2)
			{
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			
			else if(maxC == tsubData[1].IF1)
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
			//ʹ�����ϵ����Ƚ��в��
			DWORD temp1,temp2,temp3,temp4,temp5;
			temp1 = tsubData[0].IAT*DistCoef[0];
			temp2 = tsubData[1].IAT*DistCoef[1];
			temp3 = temp1+temp2; 
			temp4 = 10000-QValue[0]-QValue[1];
			if(temp4!=0&&temp3!=0)
			{
				temp5 = (((LLONG)temp2*10000)/temp3)-QValue[0];
				*RM_pnDist = ((LLONG)temp5*DistanceSet[0])/temp4;		
			}
			else
				*RM_pnDist = 0;
		}
		else
		{
			//����λ�ڵڶ����䣬������������·��Ϊ������·
			DWORD temp1,temp2,temp3;
			temp1 = MAX4(tsubData[1].IT1,tsubData[1].IT2,tsubData[2].IT1,tsubData[2].IT2);			
			temp2 = MAX4(tsubData[1].IF1,tsubData[1].IF2,tsubData[2].IF1,tsubData[2].IF2);
			temp3 = MAX2(temp1,temp2);
			if((temp3 == tsubData[1].IT1)||(temp3 == tsubData[2].IT1))
			{
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else if((temp3 == tsubData[1].IF1)||(temp3 == tsubData[2].IF1))
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
			else if((temp3 == tsubData[1].IT2)||(temp3 == tsubData[2].IT2))
			{
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
			//ʹ�������е����Ƚ��в��
//			temp1 = tsubData[2].IT1+tsubData[2].IF1;
//			temp2 = tsubData[2].IT2+tsubData[2].IF2;	
			temp1 = tsubData[2].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
			temp2 = tsubData[2].IP2;

			temp3 = MIN2(temp1,temp2);
			*RM_pnDist = *RS_pnD1+((LLONG)*RS_pnD2*2*temp3)/totalfeedout;
		}
	}
	
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}	
/***********************************************************************************
˫����AT����ģʽ
***********************************************************************************/
void CRelayTask::CalcFLMode_2(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	LONG maxIp,maxIat;
	LONG lineSN = WhichShortcircuit(tsubData,no);
	if(lineSN == DOWNLINE)
	{
		maxIp = MAX2(tsubData[0].IT1,tsubData[0].IF1);
		maxIat = MAX3(tsubData[0].IAT1, tsubData[1].IAT1, tsubData[2].IAT1);
		//�����ж��Ƿ���TF����
		if(maxIp*100>=*RS_pnKF*2*maxIat)
		{			
			//����TF�о���ʹ�õ翹�����в��
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp1,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F;	
			faultINF = TFFaultDir(tsubData);
		}
		else
		{
			//������������ֻ��ʹ�����ϵ����Ƚ��в��
			//���¾�ʹ��AT����TF�����жϹ�������
			//����λ�ڵ�һ����
			if(tsubData[0].IAT1>=tsubData[2].IAT1)
			{			
				faultRange = RANGE1;

				if(tsubData[0].IT1>tsubData[0].IF1)
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}
				DWORD temp1,temp2,temp3,temp4,temp5;
				temp1 = tsubData[0].IAT1*DistCoef[0];
				temp2 = tsubData[1].IAT1*DistCoef[1];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[0]-QValue[1];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[0];
					*RM_pnDist = ((LLONG)temp5*DistanceSet[0])/temp4;		
				}
				else
					*RM_pnDist = 0;
			}
			//����λ�ڵڶ�����
			else
			{				
				faultRange = RANGE2;
				if(tsubData[0].IT1>tsubData[0].IF1)
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}
				DWORD temp1,temp2,temp3,temp4,temp5;
				temp1 = tsubData[1].IAT1*DistCoef[1];
				temp2 = tsubData[2].IAT1*DistCoef[2];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[1]-QValue[2];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[1];
					*RM_pnDist = DistanceSet[0]+((LLONG)temp5*DistanceSet[1])/temp4;		
				}
				else
					*RM_pnDist = DistanceSet[0];
			}
		}
	}
	//���з�������
	else
	{
		maxIp = MAX2(tsubData[0].IT2,tsubData[0].IF2);
		maxIat = MAX3(tsubData[0].IAT2, tsubData[1].IAT2, tsubData[2].IAT2);
		//�����ж��Ƿ���TF����
		if(maxIp*100>=*RS_pnKF*2*maxIat)
		{			
			//����TF�о���ʹ�õ翹�����в��
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp2,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F;	
			faultINF = TFFaultDir(tsubData);
		}
		else
		{
			//������������ֻ��ʹ�����ϵ����Ƚ��в��
			//���¾�ʹ��AT����TF�����жϹ�������
			//����λ�ڵ�һ����
			if(tsubData[0].IAT2>=tsubData[2].IAT2)
			{			
				faultRange = RANGE1;
				if(tsubData[0].IT2>tsubData[0].IF2)
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}
				DWORD temp1,temp2,temp3,temp4,temp5;
				temp1 = tsubData[0].IAT2*DistCoef[0];
				temp2 = tsubData[1].IAT2*DistCoef[1];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[0]-QValue[1];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[0];
					*RM_pnDist = ((LLONG)temp5*DistanceSet[0])/temp4;		
				}
				else
					*RM_pnDist = 0;
			}
			//����λ�ڵڶ�����
			else
			{				
				faultRange = RANGE2;
				if(tsubData[0].IT2>tsubData[0].IF2)
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}
				DWORD temp1,temp2,temp3,temp4,temp5;
				temp1 = tsubData[1].IAT2*DistCoef[1];
				temp2 = tsubData[2].IAT2*DistCoef[2];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[1]-QValue[2];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[1];
					*RM_pnDist = DistanceSet[0]+((LLONG)temp5*DistanceSet[1])/temp4;		
				}
				else
					*RM_pnDist = DistanceSet[0];
			}
		}
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}	
/***********************************************************************************
//ȫ������ʽ��AT��ATȫ����
***********************************************************************************/
void CRelayTask::CalcFLMode_3(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	//�ڸ���ģʽ�£�������������ϲ�Ϊһ��
	//�ڹ����������˵İ˸��������ҳ���������������������·��Ϊ������·
	DWORD temp1,temp2,temp3;
	LONG totalfeedout;
	LONG maxIp,maxIat;
	//totalfeedout = tsubData[0].IT1+tsubData[0].IF1+tsubData[0].IT2+tsubData[0].IF2;	
	totalfeedout = tsubData[0].IP1+ tsubData[0].IP2; //2014-10-27 11:45:40 ����������Ϊ������ʸ����֮��
	maxIp = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[0].IT2,tsubData[0].IF2);
	maxIat = MAX3(tsubData[0].IAT,tsubData[1].IAT,tsubData[2].IAT);
	
	//����TF���ϵ��оݣ������ж�ϵ����ֵ��ϵ��Ϊ10
	if(maxIp*100>=*RS_pnKF*maxIat) 
	{
		faultType = T_F;	
		faultINF = TFFaultDir(tsubData);		
		temp1 = tsubData[0].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
		temp2 = tsubData[0].IP2;

		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = ((LLONG)(*RS_pnD1+*RS_pnD2)*2*temp3)/totalfeedout;			
	}
	else
	{		
		temp1 = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[2].IT1,tsubData[2].IF1);//����
		temp2 = MAX4(tsubData[0].IT2,tsubData[0].IF2,tsubData[2].IT2,tsubData[2].IF2);//����
		//����������������
		if(temp1>=temp2)
		{
			faultDirection = DOWNLINE;
			if((temp1==tsubData[0].IT1)||(temp1==tsubData[2].IT1))
			{
				faultType = T_R;
				faultINF = rpt_B_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_B_FR;
			}
		}
		else
		{
			faultDirection = UPLINE;
			if((temp2==tsubData[0].IT2)||(temp2==tsubData[2].IT2))
			{
				faultType = T_R;
				faultINF = rpt_F_TR;
			}
			else
			{
				faultType = F_R;
				faultINF = rpt_F_FR;
			}
		}		
		//ʹ�÷ֶε������е����Ƚ��в��
//		temp1 = tsubData[0].IT1+tsubData[0].IF1;
//		temp2 = tsubData[0].IT2+tsubData[0].IF2;
		temp1 = tsubData[0].IP1;	//2014-10-27 11:36:00������ʹ�ñ����ͣ���Ϊʸ������м���
		temp2 = tsubData[0].IP2;

		temp3 = MIN2(temp1,temp2);
		*RM_pnDist = ((LLONG)(*RS_pnD1+*RS_pnD2)*2*temp3)/totalfeedout;	
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}
/***********************************************************************************
ȫ������ʽ�·�����ATȫ����
***********************************************************************************/
void CRelayTask::CalcFLMode_4(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	
	*RM_pnDist = 0;
	faultINF = R_UNKOWNMODE;
}		
/***********************************************************************************
˫����ֱ����ʽ
***********************************************************************************/
void CRelayTask::CalcFLMode_5(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{	
	DWORD temp1,temp2;
	LONG lineSN = WhichShortcircuit(tsubData,no);
	if(lineSN == DOWNLINE)
	{
		temp1 = tsubData[0].IT1+tsubData[0].IF1;
		temp2 = ToAbs((tsubData[0].IT1-tsubData[0].IF1));
		//����֮�ʹ��ڶ���֮��20������Ϊ���߻�����ȣ���ΪTF����
		if(temp1>=temp2*20)
		{
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp1,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			
			CaluRX(&lkSSData[no].lockUp1,&lkSSData[no].lockIp1,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);				
			faultType = T_F;	
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//�������͵Ĺ���
			//ֱ�ӱȽ�������·�ĵ�����С����
			if(tsubData[0].IT1>=tsubData[0].IF1)
			{
				//T�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU1,&lkSSData[no].lockIT1,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_T = tempImpd;
				CalcFLX(&T_FaultLocate);				
				faultType = T_R;	
				faultINF = rpt_B_TR;
				lkSSData[no].lockImpd_T = tempImpd;
			}
			else
			{
				//F�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU2,&lkSSData[no].lockIF1,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_F = tempImpd;
				CalcFLX(&F_FaultLocate);				
				faultType = F_R;	
				faultINF = rpt_B_FR;
				lkSSData[no].lockImpd_F = tempImpd;
			}
		}
	}
	else
	{
		temp1 = tsubData[0].IT2+tsubData[0].IF2;
		temp2 = ToAbs((tsubData[0].IT2-tsubData[0].IF2));
		//����֮�ʹ��ڶ���֮��20������Ϊ���߻�����ȣ���ΪTF����
		if(temp1>=temp2*20)
		{
			TRelElecValPar tempImpd;
			CaluRX(&lkSSData[no].lockUp1,&lkSSData[no].lockIp2,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F;	
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//�������͵Ĺ���
			//ֱ�ӱȽ�������·�ĵ�����С����
			if(tsubData[0].IT2>=tsubData[0].IF2)
			{
				//T�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU1,&lkSSData[no].lockIT2,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_T = tempImpd;
				CalcFLX(&T_FaultLocate);					
				faultType = T_R;	
				faultINF = rpt_F_TR;
				lkSSData[no].lockImpd_T = tempImpd;
			}
			else
			{
				//F�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU2,&lkSSData[no].lockIF2,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_F = tempImpd;
				CalcFLX(&F_FaultLocate);					
				faultType = F_R;	
				faultINF = rpt_F_FR;
				lkSSData[no].lockImpd_F = tempImpd;
			}
		}
	}	
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}
/***********************************************************************************
������˫����ֱ����ʽ
***********************************************************************************/
void CRelayTask::CalcFLMode_6(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	
	*RM_pnDist = 0;
	faultINF = R_UNKOWNMODE;
}	
/***********************************************************************************
ĩ�˲�����˫����ֱ����ʽ
***********************************************************************************/
void CRelayTask::CalcFLMode_7(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	DWORD temp1,temp2,temp3;
	temp1 = MAX4(tsubData[0].IT1,tsubData[0].IF1,tsubData[0].IT2,tsubData[0].IF2);
	//�����������У������Ϸ���������
	if((temp1 == tsubData[0].IT1)||(temp1 == tsubData[0].IF1))
	{
		temp2 = tsubData[0].IT1+tsubData[0].IF1;
		temp3 = ToAbs((tsubData[0].IT1-tsubData[0].IF1));
		//��T�ߵ�����F�ߵ���������ȣ���Ϊ��TF����
		if(temp2>=temp3*20)
		{
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp1,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);				
			faultType = T_F; 
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//T�ߵ�������Ϊ��T�߹��ϣ�ʹ�������е����Ƚ��в��
			if(temp1 == tsubData[0].IT1)
			{				
				faultType = T_R; 
				faultINF = rpt_B_TR;
				*RM_pnDist = ((LLONG)(*RS_pnD1+*RS_pnD2)*2*tsubData[0].IT2)/(tsubData[0].IT1+tsubData[0].IT2);
			}
			//F�ߵ�������Ϊ��F�߹���
			else
			{				
				faultType = F_R; 
				faultINF = rpt_B_FR;
				*RM_pnDist = ((LLONG)(*RS_pnD1+*RS_pnD2)*2*tsubData[0].IF2)/(tsubData[0].IF1+tsubData[0].IF2);
			}
				
		}
	}
	else
	{
		//�����������У������Ϸ���������
		temp2 = tsubData[0].IT2+tsubData[0].IF2;
		temp3 = ToAbs((tsubData[0].IT2-tsubData[0].IF2));
		//��T�ߵ�����F�ߵ���������ȣ���Ϊ��TF����
		if(temp2>=temp3*20)
		{
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp2,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);				
			faultType = T_F; 
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//T�ߵ�������Ϊ��T�߹��ϣ�ʹ�������е����Ƚ��в��
			if(temp1 == tsubData[0].IT2)
			{				
				faultType = T_R; 
				faultINF = rpt_F_TR;
				*RM_pnDist = ((LLONG)(*RS_pnD1+*RS_pnD2)*2*tsubData[0].IT1)/(tsubData[0].IT1+tsubData[0].IT2);
			}
			//F�ߵ�������Ϊ��F�߹���
			else
			{				
				faultType = F_R ;
				faultINF = rpt_F_FR;
				*RM_pnDist = ((LLONG)(*RS_pnD1+*RS_pnD2)*2*tsubData[0].IF1)/(tsubData[0].IF1+tsubData[0].IF2);
			}				
		}
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}
/***********************************************************************************
����ֱ��������AT��ʽ
***********************************************************************************/
void CRelayTask::CalcFLMode_8(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{	
	LONG maxIp,maxIat;	
	DWORD temp1,temp2,temp3,temp4,temp5;
	LONG lineSN = WhichShortcircuit(tsubData,no);
	if(lineSN == DOWNLINE)
	{		
		//���Ϸ��������У�ֱ����ʽ��ֻ��ʹ�õ翹�����
		temp1 = tsubData[0].IT1+tsubData[0].IF1;
		temp2 = ToAbs((tsubData[0].IT1-tsubData[0].IF1));
		//����֮�ʹ��ڶ���֮��20������Ϊ���߻�����ȣ���ΪTF����
		if(temp1>=temp2*20)
		{
			TRelElecValPar tempImpd;
			CaluRX(&lkSSData[no].lockUp1,&lkSSData[no].lockIp1,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F; 
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//�������͵Ĺ���
			//ֱ�ӱȽ�������·�ĵ�����С����
			if(tsubData[0].IT1>=tsubData[0].IF1)
			{
				//T�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU1,&lkSSData[no].lockIT1,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_T = tempImpd;
				CalcFLX(&T_FaultLocate);					
				faultType = T_R; 
				faultINF = rpt_B_TR;
				lkSSData[no].lockImpd_T = tempImpd;
			}
			else
			{
				//F�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU2,&lkSSData[no].lockIF1,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_F = tempImpd;
				CalcFLX(&F_FaultLocate);					
				faultType = F_R; 
				faultINF = rpt_B_FR;
				lkSSData[no].lockImpd_F = tempImpd;
			}
		}
	}
	else
	{
		//���Ϸ��������У�����AT���磬ʹ�����ϵ����Ƚ��в��
		maxIp = MAX2(tsubData[0].IT2,tsubData[0].IF2);
		maxIat = MAX3(tsubData[0].IAT2, tsubData[1].IAT2, tsubData[2].IAT2);
		//�����ж��Ƿ���TF����
		if(maxIp*100>=*RS_pnKF*2*maxIat)
		{			
			//����TF�о���ʹ�õ翹�����в��
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp2,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F; 
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//������������ֻ��ʹ�����ϵ����Ƚ��в��
			//���¾�ʹ��AT����TF�����жϹ�������
			//����λ�ڵ�һ����
			if(tsubData[0].IAT2>=tsubData[2].IAT2)
			{			
				faultRange = RANGE1;
				if(tsubData[0].IT2>=tsubData[0].IF2)
				{
					faultType = T_R;
					faultINF = rpt_F_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_F_FR;
				}				
				temp1 = tsubData[0].IAT2*DistCoef[0];
				temp2 = tsubData[1].IAT2*DistCoef[1];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[0]-QValue[1];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[0];
					*RM_pnDist = ((LLONG)temp5*DistanceSet[0])/temp4;		
				}
				else
					*RM_pnDist = 0;
			}
			//����λ�ڵڶ�����
			else
			{				
				faultRange = RANGE2;
				if(tsubData[0].IT2>=tsubData[0].IF2)
				{
					faultType = T_R;
					faultINF = rpt_F_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_F_FR;
				}				
				temp1 = tsubData[1].IAT2*DistCoef[1];
				temp2 = tsubData[2].IAT2*DistCoef[2];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[1]-QValue[2];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[1];
					*RM_pnDist = DistanceSet[0]+((LLONG)temp5*DistanceSet[1])/temp4; 	
				}
				else
					*RM_pnDist = DistanceSet[0];
			}
		}
	}
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}	
/***********************************************************************************
����ֱ��������AT��ʽ
***********************************************************************************/
void CRelayTask::CalcFLMode_9(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{	
	LONG maxIp,maxIat;
	LONG lineSN = WhichShortcircuit(tsubData,no);
	DWORD temp1,temp2,temp3,temp4,temp5;
	if(lineSN == DOWNLINE)
	{
		maxIp = MAX2(tsubData[0].IT1,tsubData[0].IF1);
		maxIat = MAX3(tsubData[0].IAT1, tsubData[1].IAT1, tsubData[2].IAT1);
		//�����ж��Ƿ���TF����
		if(maxIp*100>=*RS_pnKF*2*maxIat)
		{			
			//����TF�о���ʹ�õ翹�����в��
			TRelElecValPar tempImpd;
			CaluRX(&m_UP1,&lkSSData[no].lockIp1,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F;	
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd= tempImpd;
		}
		else
		{
			//������������ֻ��ʹ�����ϵ����Ƚ��в��
			//���¾�ʹ��AT����TF�����жϹ�������
			//����λ�ڵ�һ����
			if(tsubData[0].IAT1>=tsubData[2].IAT1)
			{			
				faultRange = RANGE1;
				if(tsubData[0].IT1>=tsubData[0].IF1)
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}				
				temp1 = tsubData[0].IAT1*DistCoef[0];
				temp2 = tsubData[1].IAT1*DistCoef[1];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[0]-QValue[1];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[0];
					*RM_pnDist = ((LLONG)temp5*DistanceSet[0])/temp4;		
				}
				else
					*RM_pnDist = 0;
			}
			//����λ�ڵڶ�����
			else
			{				
				faultRange = RANGE2;
				if(tsubData[0].IT1>=tsubData[0].IF1)
				{
					faultType = T_R;
					faultINF = rpt_B_TR;
				}
				else
				{
					faultType = F_R;
					faultINF = rpt_B_FR;
				}				
				temp1 = tsubData[1].IAT1*DistCoef[1];
				temp2 = tsubData[2].IAT1*DistCoef[2];
				temp3 = temp1+temp2; 
				temp4 = 10000-QValue[1]-QValue[2];
				if(temp4!=0&&temp3!=0)
				{
					temp5 = (((LLONG)temp2*10000)/temp3)-QValue[1];
					*RM_pnDist = DistanceSet[0]+((LLONG)temp5*DistanceSet[1])/temp4;		
				}
				else
					*RM_pnDist = DistanceSet[0];
			}
		}
	}
	else
	{
		//���й��ϣ�ֻ��ʹ�õ翹�����в��	
		temp1 = tsubData[0].IT2+tsubData[0].IF2;
		temp2 = ToAbs((tsubData[0].IT2-tsubData[0].IF2));
		//����֮�ʹ��ڶ���֮��20������Ϊ���߻�����ȣ���ΪTF����
		if(temp1>=temp2*20)
		{
			TRelElecValPar tempImpd;
			CaluRX(&lkSSData[no].lockUp1,&lkSSData[no].lockIp2,&tempImpd,TRUE1,CF_pnZ);	//�����迹
			m_LImpd_TF = tempImpd;
			CalcFLX(&TF_FaultLocate);			
			faultType = T_F;	
			faultINF = TFFaultDir(tsubData);
			lkSSData[no].lockImpd = tempImpd;
		}
		else
		{
			//�������͵Ĺ���
			//ֱ�ӱȽ�������·�ĵ�����С����
			if(tsubData[0].IT2>=tsubData[0].IF2)
			{
				//T�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU1,&lkSSData[no].lockIT2,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_T = tempImpd;
				CalcFLX(&T_FaultLocate);					
				faultType = T_R; 
				faultINF = rpt_F_TR;
				lkSSData[no].lockImpd_T = tempImpd;
			}
			else
			{
				//F�߹���
				TRelElecValPar tempImpd;
				CaluRX(&lkSSData[no].lockU2,&lkSSData[no].lockIF2,&tempImpd,TRUE1,CF_pnZ); //�����迹
				m_LImpd_F = tempImpd;
				CalcFLX(&F_FaultLocate);					
				faultType = F_R ;
				faultINF = rpt_F_FR;
				lkSSData[no].lockImpd_F = tempImpd;
			}
		}
	}	
	DB_LogWriteRM(RM_DIST,*RM_pnDist);
}
void CRelayTask::CalcFLMode_10(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	
	*RM_pnDist = 0;
	faultINF = R_UNKOWNMODE;
} 	
void CRelayTask::CalcFLMode_11(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	
	*RM_pnDist = 0;
	faultINF = R_UNKOWNMODE;
}
void CRelayTask::CalcFLMode_12(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	
	*RM_pnDist = 0;
	faultINF = R_UNKOWNMODE;
} 	
void CRelayTask::CalcFLMode_13(TSubData *tsubData,WORD no,BYTE rptSerialNo)
{
	
	*RM_pnDist = 0;
	faultINF = R_UNKOWNMODE;
}
/**************************************************************************
�����ж����������Ƿ�������
***************************************************************************/
BOOL CRelayTask::IsAEqualB(LONG A, LONG B)
{	
	if(A<50&&B<50&&A>0&&B>0)
		return TRUE;	
	if(MAX2(A,B)*100<MIN2(A,B)*105)
		return TRUE;
	if((A+B)>=20*(A-B))
		return FALSE;

	return FALSE;
}
/**************************************************************************
TF����ʱ�����Ϸ�����ж�
***************************************************************************/

WORD CRelayTask::TFFaultDir(TSubData *tsubData)
{
	DWORD totalT1,totalF1,totalT2,totalF2;
	DWORD forwardVal,backwardVal;
	for(int i=0;i<*RS_pnN;i++)
	{
		totalT1 = tsubData[i].IT1;
		totalF1 = tsubData[i].IF1;
		totalT2 = tsubData[i].IT2;
		totalF2 = tsubData[i].IF2;
	}
	//����ֵ��
	forwardVal = ToAbs((totalT1-totalF1));
	//����ֵ��
	backwardVal = ToAbs((totalT2-totalF2));

	if(forwardVal>=backwardVal)
		return rpt_F_TF;
	else 
		return rpt_B_TF;

	
}

