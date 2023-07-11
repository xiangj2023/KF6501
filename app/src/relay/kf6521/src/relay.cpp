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
*           �����㷨����                                                                       
*			                                                                
* ����      KF6521��·�������װ��                                                                       
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
*      yanzh             12/07/01    ��ʼ����                         
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
#include "bo.h"
#include "daefunc.h"

extern DWORD dwRelayMSCnt;		//�����ú��������
extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;			//AD��ǰдָ��
extern TSysConfigTable	*G_pCfgTable;
extern BYTE 	g_RptSerialNum;			//�������к� 
extern BOOL		RelayCheckErr;			//����Ԫ���Լ�����־
extern BOOL 	RelayCheckInitOK;		//����Ԫ���Լ��ʼ����־
extern BYTE		*g_MaintSW;

extern DWORD dwADDataWritePtr;	//�������������ݶ�дָ��

DWORD		dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD		dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
DWORD 		dwRelayTaskID;
BOOL 		RelayTaskInitOK = FALSE;
TStructCheck StructCheck[CHECK_NUM];
extern DWORD dwADResumeCnt;

TRunTimeRM	*pSysRM;		//��������ֵ����ָ��
TRunTimeBI  *pSysBI;		//ң��ʵʱ����ָ��
BOOL g_FaultOnFlag;
TLineData	JDXXElecData;


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

    //ѭ�����ý�����Ϣ����,ֱ���յ������˳���Ϣ
    while(status != ERR_MESSAGE_QUIT)
    {
		//����������м���
		if(pTask != NULL)
			pTask->Counter = 0;

		status = OS_GetMessage(dwRelayTaskID, &msg);
		if(status == ERR_MESSAGE_OK)
		{
			switch(LOWORD(msg.lParam))
			{
				case SM_TIMEOUT:
					pRelayTask->OnTimeOut(0);
					break;
				case SM_RELAYDATA:	//���������޸�
					pRelayTask->OnRelayChange(0, 0);
					break;
				case SM_RELAY_INT:
					pRelayTask->OnSampleEnd(0,0);
					break;				
				case SM_LOGICCHANGED:
					pRelayTask->OnLogicChanged();
					break;
				case SM_JDXXRESULT:
					pRelayTask->OnJDXXResult(0,msg.hParam);
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


/*	
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	//��ȡ���ݿⱣ������ֵ����ָ�룬�Լӿ�����ٶ�
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	//��ʼ�������ڲ�����
	InitAllRelayMem();	
	//��ʼ������Ԫ��
	InitRealy();
	//��ʼ������¼������	
	for(LONG i=0;i<97;i++)
	{
		DCMacroArray[i]=0;
	}
	InitDCRecInfo();	
	InitSet();
	
	RelCFGCLR = CFG_STATUS_CLR;			//�����˳�
	RelCFGSET = CFG_STATUS_SET;			//����Ͷ��
	
	g_RptSerialNum = 0;				//ϵͳ�ı������к�
	RptSerialNumOfAct =0;			//ϵͳ�ı��������������к�
	RptSerialNumOfStart=0;			//ϵͳ�Ĵ����ϵ����������������к�	
	FaultSINum = 0;					//�������������	
	
	TripFailFlag = FALSE;			//��բʧ�ܱ�־
	FaultOnFlag = FALSE;			//������ϴ����־
	g_FaultOnFlag = FALSE;			//������ϴ����־ ����CT���߼��
	HaveMakeStartRptFlag = FALSE;	//���������ϵ������������־
	m_bFaultLKFlag = FALSE;			//������ʼʱ��������־
	CurRecoverFlag =FALSE;   		//�غ�բ����������ָ���־
	FaultCheckOutFlag = FALSE;		//������ϱ�־
	m_bLastRecStatusFlag = FALSE;	//����¼�������������жϱ�־
	m_bPDCheckA = FALSE;				//���������书�ʷ������־
	m_bPDCheckB = FALSE;				//���������书�ʷ������־
	m_bPDCheckC = FALSE;				//���������书�ʷ������־
	m_b52BFFLAG = FALSE;
	m_bLogicChanged = FALSE;
	m_bR79PickUpRpt = FALSE;

	//---------------�ӵ�ѡ�߱�־��ʼ��---------------	
	JDXXOnFlag = FALSE;
	JDXXCheckFlag = FALSE;
	JDXXPickFlag = FALSE;
	JDXXForwardFlag = FALSE;
	HaveJDXXResultFlag = FALSE;
	m_bHaveMakeTripFailRpt = FALSE;
	
	PickCheck = 0;					//ͻ���������ж������������
	for(WORD i=0;i<5;i++)
		PickCheckNext[i] = 0;				//ͻ���������ж���������������������	
	lSampData1 = 0;
	lSampData2 = 0;
	lSampDate3 = 0;
	lPickValue = 0;
	m_lPickUpValue = 0;
	
	//����¼���̵�����ֵ�ϳ��������ʼ��
	for(LONG i=0;i<6;i++)
	{
		DCCaluResult[i]=0;
	}
	//�������ݶ�ȡͨ������
	dwChanDataMask = ( (0x01<<CHANNEL_6521_UA)|(0x01<<CHANNEL_6521_UB)|(0x01<<CHANNEL_6521_UC)|\
						(0x01<<CHANNEL_6521_U0)|(0x01<<CHANNEL_6521_U1)|(0x01<<CHANNEL_6521_U2)|\
						(0x01<<CHANNEL_6521_IA)|(0x01<<CHANNEL_6521_IB)|(0x01<<CHANNEL_6521_IC)|\
						(0x01<<CHANNEL_6521_I0)|(0x01<<CHANNEL_6521_I1)|(0x01<<CHANNEL_6521_U3));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	
	dwComposeMask = ((0x01<<CHANNEL_6521_UAB)|(0x01<<CHANNEL_6521_UBC)|(0x01<<CHANNEL_6521_UCA));
	
	m_dwFreqMask2 = CHANNEL_6521_U1;	
	//��ʼ����ȡ����������ʱ������
	//�������ݻ�����
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);	
	//�ϳ�ͨ�����ݻ�����
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	//ͻ����������ʷ���ݻ�����
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*5);
	//�����书�ʷ����ж����ݻ�����
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);

	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*12));


	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*2);

	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	
	ASSERT(dwPDDataBuf != NULL);
	if(dwPDDataBuf == NULL)
	{
		LogError("CRelayTask,dwPDDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPDDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM);
	
	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3));

//*************��Ƶ���**********************************************************

	m_wFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	m_FreqCaluBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*FREQUENCE_SAMP_NUM);

	ASSERT(m_wFreqBuf != NULL);
	if(m_wFreqBuf == NULL)
	{
		LogError("CRelayTask,m_wFreqBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_wFreqBuf, 0, sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	
	ASSERT(m_FreqCaluBuf != NULL);
	if(m_FreqCaluBuf == NULL)
	{
		LogError("CRelayTask,m_FreqCaluBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_FreqCaluBuf, 0, sizeof(DWORD)*FREQUENCE_SAMP_NUM);

//*******************************************************************************
	//�����ǳ�ʼ���߼����̲���
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	
	g_MaintSW = SW_pbyMAINT;

	//��ʼ�������Լ�Ϊ����״̬
	
	for(LONG i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();
	
	caluFlag = FALSE;
	RelayTaskInitOK = TRUE;
	SetTimer(1,5);
	

}
/************************************************************************************************
*����: ��ʼ���뱣��ֱ����صĶ�ֵ
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitSet(void)
{		
	Factor1.Real = -1000/2;
	Factor1.Imag = 1732/2;
	Factor1.Mod = 1000;
	
	Factor2.Real = -1000/2;
	Factor2.Imag = -1732/2;
	Factor2.Mod = 1000;

	m_U2Set = *CF_pnUp*8/100; //8V�����ѹ
	
}
/************************************************************************************************
*����: ��ʼ������Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	InitR50I0();
	InitR50I1();
	InitR50I2();
	InitR50I3();
	InitR50N1();
	InitR50N2();
	InitR50IOL();
	InitR50ISH();
	InitR27U();		
	InitRel79();	
	InitRel25();
	InitStructCheck();
	
}
void CRelayTask::OnTimeOut(DWORD id)
{	
	if(dwADResumeCnt ==0)
		return;
	else
	{
		LogicProcess();
	}
}

//��������������Ϣ����
void CRelayTask::OnSampleEnd(DWORD ptr, DWORD arg)
{	
	static WORD wInvalidCheckCnt;
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
	g_FaultOnFlag = FaultOnFlag;

	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
//		LEDOnOff(ALARM_LED, TRUE);
		RelayCheckErr = TRUE;
		return;
	}

	//���Ƚ��п����Լ�
	if(!m_bLogicChanged)
		BoBreakCheck();

	//��ȡ��������
	ReadSampleData(dwSampBuf,dwChanDataMask,0);	
	//�������It/If/U1/U2
	CaluUI(); 	
	//�ϳ�Up,Ipͨ������д
	WriteComposeBuf();
	//����Ip/Up�Լ�г������
	CaluUpIp();
	//���㸺���ѹ��������ѹ�����̵�����
	CaluU2();
	//����������ĽǶ�
	CaluUIAngle();

	//������ϴ���֮�󲻽���PT�����ж�
	if(!FaultOnFlag)
	{
		PTCheck();							
	}
	FalutProcess();								//����ѭ������
	GeneralUnderValRelay(&R27U);				//�͵�ѹ�����ӳ���	
	GeneralOverValueRelay(&R50IOL);
	AutoRCAct(&Rel79);							//�����غ�բ�ӳ���
	R59NAlarm();
	Process52BF();
	
	
	LogicProcess();								//�߼����̴���
	
	Rel25Check();		//��ͬ��
	JDXXCheck();
	
	//�ж��Ƿ����¼��
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
	
	//д����¼������
	WriteDCChannel();
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
	
}

//------------------------------------------------------------------------------------------ 
// ����: д�ϳɲ������ݻ�����	                                                           							          
// ����:  pBuf1,ԭʼ������������ݻ�������pBuf2,�ϳɲ��������ݻ�������dwMask,ͨ������                                        
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::WriteComposeBuf()
{	
	if(*AS_pnPM == PHASEU) //�ⲿ����������ѹ
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i]-dwSampBuf[i+READ_POINT_NUM];
			dwComposeBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM]-dwSampBuf[i+READ_POINT_NUM*2];
			dwComposeBuf[i+READ_POINT_NUM*2] = dwSampBuf[i+READ_POINT_NUM*2]-dwSampBuf[i];		
		}		
		CaluBaseFourier_S15(dwComposeBuf,&Uab);
		CaluModValue(&Uab);
		CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&Ubc);
		CaluModValue(&Ubc);
		CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*2,&Uca);
		CaluModValue(&Uca);
	}
	else //�ⲿ��������ߵ�ѹ
	{		
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i];
			dwComposeBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM];
			dwComposeBuf[i+READ_POINT_NUM*2] = dwSampBuf[i+READ_POINT_NUM*2];		
		}		
		Uab = Ua;
		Ubc = Ub;
		Uca = Uc;
		
	}
	*RM_pnUab= Uab.Mod;                                       
	DB_LogWriteRM(RM_UAB, *RM_pnUab);	
	*RM_pnUbc= Ubc.Mod;                                       
	DB_LogWriteRM(RM_UBC, *RM_pnUbc);	
	*RM_pnUca= Uca.Mod;                                       
	DB_LogWriteRM(RM_UCA, *RM_pnUca);	
	
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL] = dwComposeBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL*2] = dwComposeBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
	}
	WriteComposeData(dwRecBuf, dwComposeMask);

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
// ����: �������	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::CaluUI(void)
{		
	//����U1ģֵ��д������������U1��ѹ�̵����֡���ѹ�̵�����

	CaluBaseFourier_S15(dwSampBuf,&Ua);
	CaluModValue(&Ua);
	*RM_pnUa= Ua.Mod;                                       
	DB_LogWriteRM(RM_UA, *RM_pnUa);	
	if(Ua.Mod>=*AS_pn59UA)
	{
		WriteRelayWord(RW_59UA,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UA,RELAY_OFF);
	}
	if(Ua.Mod>=*AS_pn27UA)
	{
		WriteRelayWord(RW_27UA,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UA,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&Ub);
	CaluModValue(&Ub);
	*RM_pnUb= Ub.Mod;                                       
	DB_LogWriteRM(RM_UB, *RM_pnUb);	
	if(Ub.Mod>=*AS_pn59UB)
	{
		WriteRelayWord(RW_59UB,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UB,RELAY_OFF);
	}
	if(Ub.Mod>=*AS_pn27UB)
	{
		WriteRelayWord(RW_27UB,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UB,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&Uc);
	CaluModValue(&Uc);
	*RM_pnUc= Uc.Mod;                                       
	DB_LogWriteRM(RM_UC, *RM_pnUc);	
	if(Uc.Mod>=*AS_pn59UC)
	{
		WriteRelayWord(RW_59UC,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UC,RELAY_OFF);
	}
	if(Uc.Mod>=*AS_pn27UC)
	{
		WriteRelayWord(RW_27UC,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UC,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0);
	CaluModValue(&U0);
	*RM_pnU0= U0.Mod;                                       
	DB_LogWriteRM(RM_U0, *RM_pnU0);	
	if(U0.Mod>=*AS_pn59U0)
	{
		WriteRelayWord(RW_59U0,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U0,RELAY_OFF);
	}
	if(U0.Mod>=*AS_pn27U0)
	{
		WriteRelayWord(RW_27U0,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U0,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&U1);
	CaluModValue(&U1);
	*RM_pnUL1= U1.Mod;                                       
	DB_LogWriteRM(RM_UL1, *RM_pnUL1);	
	if(U1.Mod>=*AS_pn59U1)
	{
		WriteRelayWord(RW_59U1,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1,RELAY_OFF);
	}
	if(U1.Mod>=*AS_pn27U1)
	{
		WriteRelayWord(RW_27U1,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&U2);
	CaluModValue(&U2);
	*RM_pnUL2= U2.Mod;                                       
	DB_LogWriteRM(RM_UL2, *RM_pnUL2);	
	if(U2.Mod>=*AS_pn59U2)
	{
		WriteRelayWord(RW_59U2,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U2,RELAY_OFF);
	}
	if(U2.Mod>=*AS_pn27U2)
	{
		WriteRelayWord(RW_27U2,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U2,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&U3);
	CaluModValue(&U3);
	*RM_pnUL3= U3.Mod;                                       
	DB_LogWriteRM(RM_UL3, *RM_pnUL3);	
	if(U3.Mod>=*AS_pn59U3)
	{
		WriteRelayWord(RW_59U3,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3,RELAY_OFF);
	}
	if(U3.Mod>=*AS_pn27U3)
	{
		WriteRelayWord(RW_27U3,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&Ia);
	CaluModValue(&Ia);
	*RM_pnIa= Ia.Mod;                                       
	DB_LogWriteRM(RM_IA, *RM_pnIa);	
	if(Ia.Mod>=*AS_pn50IA)
	{
		WriteRelayWord(RW_50IA,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IA,RELAY_OFF);
	}
	if(Ia.Mod>=*AS_pn37IA)
	{
		WriteRelayWord(RW_37IA,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IA,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&Ib);
	CaluModValue(&Ib);
	*RM_pnIb= Ib.Mod;                                       
	DB_LogWriteRM(RM_IB, *RM_pnIb);	
	if(Ib.Mod>=*AS_pn50IB)
	{
		WriteRelayWord(RW_50IB,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IB,RELAY_OFF);
	}
	if(Ib.Mod>=*AS_pn37IB)
	{
		WriteRelayWord(RW_37IB,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IB,RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&Ic);
	CaluModValue(&Ic);
	*RM_pnIc= Ic.Mod;                                       
	DB_LogWriteRM(RM_IC, *RM_pnIc);	
	if(Ic.Mod>=*AS_pn50IC)
	{
		WriteRelayWord(RW_50IC,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IC,RELAY_OFF);
	}
	if(Ic.Mod>=*AS_pn37IC)
	{
		WriteRelayWord(RW_37IC,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IC,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&I0);
	CaluModValue(&I0);
	*RM_pnI0= I0.Mod;                                       
	DB_LogWriteRM(RM_I0, *RM_pnI0);	
	if(I0.Mod>=*AS_pn50I0)
	{
		WriteRelayWord(RW_50I0,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I0,RELAY_OFF);
	}
	if(I0.Mod>=*AS_pn37I0)
	{
		WriteRelayWord(RW_37I0,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I0,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&I1);
	CaluModValue(&I1);
	*RM_pnI1= I1.Mod;                                       
	DB_LogWriteRM(RM_I1, *RM_pnI1);	
	if(I1.Mod>=*AS_pn50I1)
	{
		WriteRelayWord(RW_50I1,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1,RELAY_OFF);
	}
	if(I1.Mod>=*AS_pn37I1)
	{
		WriteRelayWord(RW_37I1,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1,RELAY_ON);
	}
}
//------------------------------------------------------------------------------------------ 
// ����: ����ϳɵ���	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUpIp(void)
{
	Ip = ElecMax(Ia,Ib,Ic,m_Phase); //��������е����ֵ�����������
	ULK = ElecMin(Uab,Ubc,Uca);		//�ߵ�ѹ����Сֵ�����ڹ����ĵ�ѹ����
	Up = ElecMax(Uab,Ubc,Uca);		//�ߵ�ѹ�����ֵ�����ڵ�ѹ����

	//���ʷ���̵�����������������������е���	
	if(m_Phase == RELAY_PHASE_A)
	{
		R50I1.PForwardRelayWord = RW_APF;
		R50I1.PRewardRelayWord	= RW_APR;		
		R50I2.PForwardRelayWord = RW_APF;
		R50I2.PRewardRelayWord	= RW_APR;
		R50I3.PForwardRelayWord = RW_APF;
		R50I3.PRewardRelayWord	= RW_APR;
	}
	else if(m_Phase == RELAY_PHASE_B)		
	{
		R50I1.PForwardRelayWord = RW_BPF;
		R50I1.PRewardRelayWord	= RW_BPR;		
		R50I2.PForwardRelayWord = RW_BPF;
		R50I2.PRewardRelayWord	= RW_BPR;
		R50I3.PForwardRelayWord = RW_BPF;
		R50I3.PRewardRelayWord	= RW_BPR;
	}	
	else if(m_Phase == RELAY_PHASE_C)		
	{
		R50I1.PForwardRelayWord = RW_CPF;
		R50I1.PRewardRelayWord	= RW_CPR;		
		R50I2.PForwardRelayWord = RW_CPF;
		R50I2.PRewardRelayWord	= RW_CPR;
		R50I3.PForwardRelayWord = RW_CPF;
		R50I3.PRewardRelayWord	= RW_CPR;
	}
}
void CRelayTask::CaluU2()
{
	NU2.Real = (Uab.Real*1000 + Ubc.Real*Factor2.Real -Ubc.Imag*Factor2.Imag + Uca.Real*Factor1.Real -Uca.Imag*Factor1.Imag)/5196;
	NU2.Imag = (Uab.Imag*1000 + Ubc.Imag*Factor2.Real +Ubc.Real*Factor2.Imag + Uca.Imag*Factor1.Real +Uca.Real*Factor1.Imag)/5196;
	CaluModValue(&NU2);
	*RM_pnU2= NU2.Mod;                                       
	DB_LogWriteRM(RM_U2, *RM_pnU2);

	//�жϸ�ѹ�����Ƿ�����
	if((*R50I0.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P0_27)||(NU2.Mod>*RS_pn50P0_47)))
		WriteRelayWord(RW_LK50I0_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I0_47U,RELAY_ON);
	
	if((*R50I1.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P1_27)||(NU2.Mod>*RS_pn50P1_47)))
		WriteRelayWord(RW_LK50I1_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I1_47U,RELAY_ON);
	
	if((*R50I2.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P2_27)||(NU2.Mod>*RS_pn50P2_47)))
		WriteRelayWord(RW_LK50I2_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I2_47U,RELAY_ON);

	if((*R50I3.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P3_27)||(NU2.Mod>*RS_pn50P3_47)))
		WriteRelayWord(RW_LK50I3_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I3_47U,RELAY_ON);
	
}
void CRelayTask::CaluUIAngle()
{
	LONG angle;
	CaluAngle(&Ua,AngleTab);	
	*RM_pnAUa=0;
	DB_LogWriteRM(RM_AUA, *RM_pnAUa);
	
	CaluAngle(&Ub,AngleTab);	
	angle = Ub.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUb = angle;
	else
		*RM_pnAUb = angle+36000;
	DB_LogWriteRM(RM_AUB, *RM_pnAUb);

	CaluAngle(&Uc,AngleTab);
	angle = Uc.Angle - Ua.Angle;
	if(angle>0)		
		*RM_pnAUc = angle;
	else
		*RM_pnAUc = angle+36000;
	DB_LogWriteRM(RM_AUC, *RM_pnAUc);

	CaluAngle(&Uab,AngleTab);
	angle = Uab.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUab = angle;
	else
		*RM_pnAUab = angle+36000;
	DB_LogWriteRM(RM_AUAB, *RM_pnAUab);
	
	CaluAngle(&Ubc,AngleTab);
	angle = Ubc.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUbc = angle;
	else
		*RM_pnAUbc = angle+36000;
	DB_LogWriteRM(RM_AUBC, *RM_pnAUbc);
	
	CaluAngle(&Uca,AngleTab);
	angle = Uca.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUca = angle;
	else
		*RM_pnAUca = angle+36000;
	DB_LogWriteRM(RM_AUCA, *RM_pnAUca);

	CaluAngle(&U1,AngleTab);	
	angle = U1.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUl1 = angle;
	else
		*RM_pnAUl1 = angle+36000;
	DB_LogWriteRM(RM_AUL1, *RM_pnAUl1);
	
	CaluAngle(&U2,AngleTab);	
	angle = U2.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUl2 = angle;
	else
		*RM_pnAUl2 = angle+36000;
	DB_LogWriteRM(RM_AUL2, *RM_pnAUl2);
	
	CaluAngle(&U3,AngleTab);	
	angle = U3.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUl3 = angle;
	else
		*RM_pnAUl3 = angle+36000;
	DB_LogWriteRM(RM_AUL3, *RM_pnAUl3);

	CaluAngle(&Ia,AngleTab);	
	angle = Ia.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIpa = angle;
	else
		*RM_pnAIpa = angle+36000;
	DB_LogWriteRM(RM_AIPA, *RM_pnAIpa);

	CaluAngle(&Ib,AngleTab);	
	angle = Ib.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIpb = angle;
	else
		*RM_pnAIpb = angle+36000;
	DB_LogWriteRM(RM_AIPB, *RM_pnAIpb);

	CaluAngle(&Ic,AngleTab);	
	angle = Ic.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIpc = angle;
	else
		*RM_pnAIpc = angle+36000;
	DB_LogWriteRM(RM_AIPC, *RM_pnAIpc);

	CaluAngle(&U0,AngleTab);	
	*RM_pnAU01=0;
	DB_LogWriteRM(RM_AU01, *RM_pnAU01);

	CaluAngle(&I0,AngleTab);	
	if(I0.Angle>=U0.Angle)
		*RM_pnAI01=I0.Angle - U0.Angle;
	else		
		*RM_pnAI01=36000 + I0.Angle - U0.Angle;
	DB_LogWriteRM(RM_AI01, *RM_pnAI01);
	
	CaluAngle(&I1,AngleTab);	
	angle = I1.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIP1 = angle;
	else
		*RM_pnAIP1 = angle+36000;
	DB_LogWriteRM(RM_AIP1, *RM_pnAIP1);

	CaluU5I5();

}
void CRelayTask::CaluU5I5(void)
{
	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*3,&U5);
	CaluModValue(&U5);
	//���������в���,1111Ϊ����ϵ��
	U5.Mod = U5.Mod*1111/1000;
	U5.Imag = U5.Imag*1111/1000;
	U5.Real = U5.Real*1111/1000;
	*RM_pnU05= U5.Mod;										
	DB_LogWriteRM(RM_U05, *RM_pnU05); 	
	CaluAngle(&U5,AngleTab);	
	*RM_pnAU05=0;
	DB_LogWriteRM(RM_AU05, *RM_pnAU05);

	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*10,&I5);
	CaluModValue(&I5);	
	//���������в���,1100Ϊ����ϵ��
	I5.Mod = I5.Mod*1100/1000;
	I5.Imag = I5.Imag*1100/1000;
	I5.Real = I5.Real*1100/1000;
	*RM_pnI05= I5.Mod;										
	DB_LogWriteRM(RM_I05, *RM_pnI05); 	
	CaluAngle(&I5,AngleTab);	
	if(I5.Angle >= U5.Angle)
		*RM_pnAI05=I5.Angle - U5.Angle;
	else
		*RM_pnAI05=36000 + I5.Angle - U5.Angle;
	DB_LogWriteRM(RM_AI05, *RM_pnAI05);
}
void CRelayTask::CaluDF_DA_DV()
{
	LONG	Uh,Ul;
	if(*RS_pn25PH == SYN_PHASE_UA)
	{
		m_dwFreqMask1 = CHANNEL_6521_UA;
		Usyn = Ua;
	}
	else if(*RS_pn25PH == SYN_PHASE_UB)
	{
		m_dwFreqMask1 = CHANNEL_6521_UB;			
		Usyn = Ub;
	}
	else if(*RS_pn25PH == SYN_PHASE_UC)
	{
		m_dwFreqMask1 = CHANNEL_6521_UC;	
		Usyn = Uc;
	}
	else if(*RS_pn25PH == SYN_PHASE_UAB)
	{
		m_dwFreqMask1 = CHANNEL_6521_UAB;	
		Usyn = Uab;
	}
	else if(*RS_pn25PH == SYN_PHASE_UBC)
	{
		m_dwFreqMask1 = CHANNEL_6521_UBC;	
		Usyn = Ubc;
	}
	else if(*RS_pn25PH == SYN_PHASE_UCA)
	{
		m_dwFreqMask1 = CHANNEL_6521_UCA;
		Usyn = Uca;
	}
	else 
	{
		m_dwFreqMask1 = CHANNEL_6521_UA;
		Usyn = Ua;
	}
	if((Usyn.Mod>*RS_pn25VUL)||(Usyn.Mod<*RS_pn25VLL)||(U1.Mod>*RS_pn25VUL)||(U1.Mod<*RS_pn25VLL))
	{
		Rel25.bDVOutLimitFlag = TRUE;
	}
	else
	{
		Rel25.bDVOutLimitFlag = FALSE;
	}
	
	m_DU.Real = U1.Real - Usyn.Real;
	m_DU.Imag = U1.Imag - Usyn.Imag;
	CaluModValue(&m_DU);
	m_DV = m_DU.Mod;

	//���α����������һ��Ƶ�ʣ�����CPU����
	if(!caluFlag)
	{
		CalcImmeFrequence(dwADDataWritePtr,Usyn.Mod,m_dwFreqMask1,m_wFreqBuf,m_FreqCaluBuf,&m_dwFreq1);		
		caluFlag = TRUE;
	}
	else
	{		
		CalcImmeFrequence(dwADDataWritePtr,U1.Mod,m_dwFreqMask2,m_wFreqBuf,m_FreqCaluBuf,&m_dwFreq2);		
		caluFlag = FALSE;
	}

	
	//ReportMsg("F1=%d     F2=%d     cnt0=%d,cnt1=%d,cnt2=%d  ,dwRelayMSCnt = %d",m_dwFreq1,m_dwFreq2,cnt0,cnt1,cnt2,dwRelayMSCnt);
	DB_WriteAI(M_F1,m_dwFreq1);	
	DB_WriteAI(M_F2,m_dwFreq2);
	
	m_DF = m_dwFreq2 - m_dwFreq1;
	
	if(m_DF<=0)//����ǲ�ؼ��ǿ����ߵ�����˶�����
	{
		m_DA = U1.Angle - Usyn.Angle;
		if(m_DA<0)
			m_DA = 36000+m_DA;
	}
	else
	{
		m_DA = U1.Angle - Usyn.Angle;
		if(m_DA>=0)
			m_DA = 36000-m_DA;
		else
			m_DA = (-1)*m_DA;
	}
	m_DF = ToAbs(m_DF);

	if(m_DA>18000)
	{
		m_DQ = 36000-*Rel25.pDA;		
	}
	else
	{
		m_DQ = m_DA;
	}


	//д�뱣������ֵ
	*RM_pnF1= m_dwFreq1;                                       
	DB_LogWriteRM(RM_F1, *RM_pnF1);	
	*RM_pnF2= m_dwFreq2;                                       
	DB_LogWriteRM(RM_F2, *RM_pnF2);		
	*RM_pnDiffU= m_DV;                                       
	DB_LogWriteRM(RM_DIFFU, *RM_pnDiffU);	
	*RM_pnDiffA= m_DA;                                       
	DB_LogWriteRM(RM_DIFFA, *RM_pnDiffA);	
	*RM_pnDiffdq = m_DQ;
	DB_LogWriteRM(RM_DIFFDQ, *RM_pnDiffdq);
	
	//�ü���ѹ�̵�����
	if((Ua.Mod<*RS_pn25_27)&&(Ub.Mod<*RS_pn25_27)&&(Uc.Mod<*RS_pn25_27))
		WriteRelayWord(RW_25_27UM,RELAY_ON);
	else
		WriteRelayWord(RW_25_27UM,RELAY_OFF);

	if((U1.Mod<*RS_pn25_27)&&(U2.Mod<*RS_pn25_27)&&(U3.Mod<*RS_pn25_27))
		WriteRelayWord(RW_25_27UL,RELAY_ON);
	else
		WriteRelayWord(RW_25_27UL,RELAY_OFF);
}

void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&R50I1;
	StructCheck[0].bigsize = sizeof(R50I1)/2 - R50I1.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&R50I2;
	StructCheck[1].bigsize = sizeof(R50I2)/2 - R50I2.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&R50I3;
	StructCheck[2].bigsize = sizeof(R50I3)/2 - R50I3.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&R50N1;
	StructCheck[3].bigsize = sizeof(R50N1)/2 - R50N1.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&R50N2;
	StructCheck[4].bigsize = sizeof(R50N2)/2 - R50N2.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&R27U;
	StructCheck[5].bigsize = sizeof(R27U)/2 - R27U.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel79;
	StructCheck[6].bigsize = sizeof(Rel79)/2 - Rel79.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel25;
	StructCheck[7].bigsize = sizeof(Rel25)/2 - Rel25.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&R50I0;
	StructCheck[8].bigsize = sizeof(R50I0)/2 - R50I0.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&R50IOL;
	StructCheck[9].bigsize = sizeof(R50IOL)/2 - R50IOL.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&R50ISH;
	StructCheck[10].bigsize = sizeof(R50ISH)/2 - R50ISH.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[11].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
	for(LONG j=0;j<CHECK_NUM;j++)
	{
		for(LONG i=0;i<StructCheck[j].bigsize;i++)
		{
			acc += StructCheck[j].pstruct[i];
		}
		StructCheck[j].acc = acc;
		acc=0x55aa;
	}

	RelayCheckInitOK = TRUE;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ�������ٶϱ���Ԫ��                                                                    
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I0()
{
	R50I0.pElecSet				= RS_pn50P0C;          						//��������������ֵ 
	R50I0.pTimeSet				= RS_pn50P0D;				 				//��������ʱ�䶨ֵ 
	R50I0.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50I0.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50I0.pAccActTimeSet		= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R50I0.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50I0.pRelCFG				= CFG_pby50I0;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50I0.pRelSW				= SW_pby50I0;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50I0.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I0.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I0.pRelPDCFG				= CFG_pby50I0_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I0.pRelFDCFG				= CFG_pby50I0_PD;					//
	R50I0.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50I0.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R50I0.pRel47UCFG			= CFG_pby50I0_47;					//��ѹ��������
	R50I0.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	
	R50I0.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50I0.pRelElec 				= &Ip;						//��ǰ����ָ�� 		
	R50I0.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50I0.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50I0.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50I0.StartRelayWord		= RW_PI50I0;							//����������μ̵����� 
	R50I0.ActRelayWord			= RW_TR50I0;							//�������������̵����� 
	R50I0.AccRelayWord			= FALSE1;							//���������̵�����,��һ��ʱ��̵����֣���ʱ�����Զ����� 
	R50I0.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R50I0.PForwardRelayWord		= RW_APF;						//����������̵����� 
	R50I0.PRewardRelayWord  	= RW_APR;
	R50I0.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	R50I0.R47ULKRelayWord		= RW_LK50I0_47U;
	R50I0.LockRelayWord			= RW_LKPI50I0;							//��α����̵����� 
	R50I0.RSTRelayWord			= RW_LKRST50I0;							//���ر����̵����� 
	R50I0.LoopCtrlRelayWord		= RW_LPC50I0;						//����ѭ�����Ƽ̵����� 
	
	R50I0.wReportNum			= R_50I0;								//��������������� 
	R50I0.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R50I0.dwK35					= NULL;
	R50I0.RptSerialNum			= &RptSerialNumOfAct;
	R50I0.bMakeActRpt			= TRUE;
	R50I0.wPickUpReportNum		= R_50I0_PI;						//��α������к�
	R50I0.wResetReportNum		= R_50I0_RES;						//���ر������к�
	R50I0.wAccPickUpRptNum		= 0xFFFF;							//������α������к�
	R50I0.wAccResetRptNum		= 0xFFFF;							//���ٷ��ر������к�
	//**********************************************************************	
//	R50I0.dwRelTimer			= 0;			 				//��ʱ�� 
	R50I0.byRelFCounter			= 0;						//���ϼ����� 
	R50I0.byRelRCounter			= 0;						//���ؼ����� 	
	R50I0.wExcepWordNum			= 8;
	R50I0.wInfo 				= 0;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������I�α���Ԫ��	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I1()
{
	R50I1.pElecSet				= RS_pn50P1C;          						//��������������ֵ 
	R50I1.pTimeSet				= RS_pn50P1D;				 				//��������ʱ�䶨ֵ 
	R50I1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50I1.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50I1.pAccActTimeSet		= RS_pn94D;						//������������ٶ���ʱ�䶨ֵ 
	R50I1.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50I1.pRelCFG				= CFG_pby50I1;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50I1.pRelSW				= SW_pby50I1;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50I1.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I1.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I1.pRelPDCFG				= CFG_pby50I1_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I1.pRelFDCFG				= CFG_pby50I1_PD;					//
	R50I1.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50I1.pRelAccCFG			= CFG_pby50I1_94;							//���޺���ٹ���------1:�У�  2:�� 
	R50I1.pRel47UCFG			= CFG_pby50I1_47;					//��ѹ��������
	R50I1.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	
	R50I1.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50I1.pRelElec 				= &Ip;						//��ǰ����ָ�� 		
	R50I1.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50I1.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50I1.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50I1.StartRelayWord		= RW_PI50I1;							//����������μ̵����� 
	R50I1.ActRelayWord			= RW_TR50I1;							//�������������̵����� 
	R50I1.AccRelayWord			= RW_TSOTF;							//���������̵�����,��һ��ʱ��̵����֣���ʱ�����Զ����� 
	R50I1.AccActRelayWord		= RW_TR50I1_94;						//����ٶ����̵����� 
	R50I1.PForwardRelayWord		= RW_APF;						//����������̵����� 
	R50I1.PRewardRelayWord  	= RW_APR;
	R50I1.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	R50I1.R47ULKRelayWord		= RW_LK50I1_47U;
	R50I1.LockRelayWord			= RW_LKPI50I1;							//��α����̵����� 
	R50I1.RSTRelayWord			= RW_LKRST50I1;							//���ر����̵����� 
	R50I1.LoopCtrlRelayWord		= RW_LPC50I1;						//����ѭ�����Ƽ̵����� 
	
	R50I1.wReportNum			= R_50I1;								//��������������� 
	R50I1.wAccReportNum			= R_50I1_94;							//���ٶ���������� 
	
	R50I1.dwK35					= NULL;
	R50I1.RptSerialNum			= &RptSerialNumOfAct;
	R50I1.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50I1.wPickUpReportNum		= R_50I1_PI;						//��α������к�
	R50I1.wResetReportNum		= R_50I1_RES;						//���ر������к�
	R50I1.wAccPickUpRptNum		= R_50I1_94_PI;						//������α������к�
	R50I1.wAccResetRptNum		= R_50I1_94_RES;					//���ٷ��ر������к�
	//**********************************************************************	
//	R50I1.dwRelTimer			= 0;			 				//��ʱ�� 
	R50I1.byRelFCounter			= 0;						//���ϼ����� 
	R50I1.byRelRCounter			= 0;						//���ؼ����� 	
	R50I1.wExcepWordNum			= 8;
	R50I1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������II�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitR50I2()
{	
	R50I2.pElecSet				= RS_pn50P2C;          						//��������������ֵ 
	R50I2.pTimeSet				= RS_pn50P2D;				 				//��������ʱ�䶨ֵ 
	R50I2.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50I2.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50I2.pAccActTimeSet		= RS_pn94D;						//������������ٶ���ʱ�䶨ֵ 
	R50I2.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50I2.pRelCFG				= CFG_pby50I2;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50I2.pRelSW				= SW_pby50I2;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50I2.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I2.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I2.pRelPDCFG				= CFG_pby50I2_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I2.pRelFDCFG				= CFG_pby50I2_PD;
	R50I2.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50I2.pRelAccCFG			= CFG_pby50I2_94;							//���޺���ٹ���------1:�У�  2:�� 
	R50I2.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50I2.pRel47UCFG			= CFG_pby50I2_47;
	R50I2.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50I2.pRelElec 				= &Ip;						//��ǰ����ָ�� 		
	R50I2.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50I2.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50I2.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50I2.StartRelayWord		= RW_PI50I2;							//����������μ̵����� 
	R50I2.ActRelayWord			= RW_TR50I2;							//�������������̵����� 
	R50I2.AccRelayWord			= RW_TSOTF;							//���������̵����� 
	R50I2.AccActRelayWord		= RW_TR50I2_94;						//����ٶ����̵����� 
	R50I2.PForwardRelayWord		= RW_APF;						//����������̵����� 
	R50I2.PRewardRelayWord  	= RW_APR;					//���ʷ�����̵����� 
	R50I2.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R50I2.R47ULKRelayWord		= RW_LK50I2_47U;
	R50I2.LockRelayWord			= RW_LKPI50I2;							//��α����̵����� 
	R50I2.RSTRelayWord			= RW_LKRST50I2;							//���ر����̵����� 
	R50I2.LoopCtrlRelayWord		= RW_LPC50I2;						//����ѭ�����Ƽ̵����� 
	
	R50I2.wReportNum			= R_50I2;								//��������������� 
	R50I2.wAccReportNum			= R_50I2_94;							//���ٶ���������� 
	
	R50I2.dwK35					= NULL;
	R50I2.RptSerialNum			= &RptSerialNumOfAct;
	R50I2.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50I2.wPickUpReportNum		= R_50I2_PI;						//��α������к�
	R50I2.wResetReportNum		= R_50I2_RES;						//���ر������к�
	R50I2.wAccPickUpRptNum		= R_50I2_94_PI;						//������α������к�
	R50I2.wAccResetRptNum		= R_50I2_94_RES;					//���ٷ��ر������к�
	//**********************************************************************	

//	R50I2.dwRelTimer			= 0;			 				//��ʱ�� 
	R50I2.byRelFCounter			= 0;						//���ϼ����� 
	R50I2.byRelRCounter			= 0;						//���ؼ����� 
	R50I2.wExcepWordNum			= 8;
	R50I2.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������III�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I3()
{	
	R50I3.pElecSet				= RS_pn50P3C;          						//��������������ֵ 
	R50I3.pTimeSet				= RS_pn50P3D;				 				//��������ʱ�䶨ֵ 
	R50I3.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50I3.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50I3.pAccActTimeSet		= RS_pn94D;						//������������ٶ���ʱ�䶨ֵ 
	R50I3.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50I3.pRelCFG				= CFG_pby50I3;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50I3.pRelSW				= SW_pby50I3;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50I3.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I3.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I3.pRelPDCFG				= CFG_pby50I3_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I3.pRelFDCFG				= CFG_pby50I3_PD;
	R50I3.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50I3.pRelAccCFG			= CFG_pby50I3_94;							//���޺���ٹ���------1:�У�  2:�� 
	R50I3.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50I3.pRel47UCFG			= CFG_pby50I3_47;
	R50I3.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50I3.pRelElec 				= &Ip;						//��ǰ����ָ�� 		
	R50I3.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50I3.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50I3.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50I3.StartRelayWord		= RW_PI50I3;							//����������μ̵����� 
	R50I3.ActRelayWord			= RW_TR50I3;							//�������������̵����� 
	R50I3.AccRelayWord			= RW_TSOTF;							//���������̵����� 
	R50I3.AccActRelayWord		= RW_TR50I3_94;						//����ٶ����̵����� 
	R50I3.PForwardRelayWord		= RW_APF;						//����������̵����� 
	R50I3.PRewardRelayWord  	= RW_APR;					//���ʷ�����̵����� 
	R50I3.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R50I3.R47ULKRelayWord		= RW_LK50I3_47U;
	R50I3.LockRelayWord			= RW_LKPI50I3;							//��α����̵����� 
	R50I3.RSTRelayWord			= RW_LKRST50I3;							//���ر����̵����� 
	R50I3.LoopCtrlRelayWord		= RW_LPC50I3;						//����ѭ�����Ƽ̵����� 
	
	R50I3.wReportNum			= R_50I3;								//��������������� 
	R50I3.wAccReportNum			= R_50I3_94;							//���ٶ���������� 
	
	R50I3.dwK35					= NULL;
	R50I3.RptSerialNum			= &RptSerialNumOfAct;
	R50I3.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50I3.wPickUpReportNum		= R_50I3_PI;						//��α������к�
	R50I3.wResetReportNum		= R_50I3_RES;						//���ر������к�
	R50I3.wAccPickUpRptNum		= R_50I3_94_PI;						//������α������к�
	R50I3.wAccResetRptNum		= R_50I3_94_RES;					//���ٷ��ر������к�
	//**********************************************************************	

//	R50I3.dwRelTimer			= 0;			 				//��ʱ�� 
	R50I3.byRelFCounter			= 0;						//���ϼ����� 
	R50I3.byRelRCounter			= 0;						//���ؼ����� 
	R50I3.wExcepWordNum			= 8;
	R50I3.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������III�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50IOL()
{	
	R50IOL.pElecSet				= RS_pn50OLC;          						//��������������ֵ 
	R50IOL.pTimeSet				= RS_pn50OLD;				 				//��������ʱ�䶨ֵ 
	R50IOL.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50IOL.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50IOL.pAccActTimeSet		= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R50IOL.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50IOL.pRelCFG				= CFG_pby50IOL;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50IOL.pRelSW				= SW_pby50IOL;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50IOL.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50IOL.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50IOL.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50IOL.pRelFDCFG			= &RelCFGCLR;
	R50IOL.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50IOL.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R50IOL.pRelFLCFG			= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50IOL.pRel47UCFG			= &RelCFGCLR;
	R50IOL.pSysTimer			= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50IOL.pRelElec 				= &Ip;						//��ǰ����ָ�� 		
	R50IOL.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50IOL.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50IOL.pRelVolt					= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50IOL.StartRelayWord		= RW_PI50IOL;							//����������μ̵����� 
	R50IOL.ActRelayWord			= RW_TR50IOL;							//�������������̵����� 
	R50IOL.AccRelayWord			= FALSE1;							//���������̵����� 
	R50IOL.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R50IOL.PForwardRelayWord	= FALSE1;						//����������̵����� 
	R50IOL.PRewardRelayWord  	= FALSE1;					//���ʷ�����̵����� 
	R50IOL.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R50IOL.R47ULKRelayWord		= FALSE1;
	R50IOL.LockRelayWord		= RW_LKPI50IOL;							//��α����̵����� 
	R50IOL.RSTRelayWord			= RW_LKRST50IOL;							//���ر����̵����� 
	R50IOL.LoopCtrlRelayWord	= RW_LPC50IOL;						//����ѭ�����Ƽ̵����� 
	
	R50IOL.wReportNum			= R_50IOL;								//��������������� 
	R50IOL.wAccReportNum		= 0xFFFF;							//���ٶ���������� 
	
	R50IOL.dwK35				= NULL;
	R50IOL.RptSerialNum			= &RptSerialNumOfAct;
	R50IOL.bMakeActRpt			= FALSE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50IOL.wPickUpReportNum		= 0xFFFF;						//��α������к�
	R50IOL.wResetReportNum		= 0xFFFF;						//���ر������к�
	R50IOL.wAccPickUpRptNum		= 0xFFFF;						//������α������к�
	R50IOL.wAccResetRptNum		= 0xFFFF;						//���ٷ��ر������к�
	//**********************************************************************	

//	R50IOL.dwRelTimer			= 0;			 				//��ʱ�� 
	R50IOL.byRelFCounter			= 0;						//���ϼ����� 
	R50IOL.byRelRCounter			= 0;						//���ؼ����� 
	R50IOL.wExcepWordNum			= 8;
	R50IOL.wInfo 				= 0;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ�������������Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50N1()
{	
	R50N1.pElecSet				= RS_pn50N1C;          						//��������������ֵ 
	R50N1.pTimeSet				= RS_pn50N1D;				 				//��������ʱ�䶨ֵ 
	R50N1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50N1.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50N1.pAccActTimeSet		= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R50N1.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50N1.pRelCFG				= CFG_pby50N1;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50N1.pRelSW				= SW_pby50N1;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50N1.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50N1.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50N1.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50N1.pRelFDCFG				= &RelCFGCLR;
	R50N1.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50N1.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R50N1.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50N1.pRel47UCFG			= &RelCFGCLR;
	R50N1.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50N1.pRelElec 				= &I0;						//��ǰ����ָ�� 		
	R50N1.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50N1.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50N1.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50N1.StartRelayWord		= RW_PI50N1;							//����������μ̵����� 
	R50N1.ActRelayWord			= RW_TR50N1;							//�������������̵����� 
	R50N1.AccRelayWord			= FALSE1;							//���������̵����� 
	R50N1.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R50N1.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R50N1.PRewardRelayWord  	= FALSE1;					//���ʷ�����̵����� 
	R50N1.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R50N1.R47ULKRelayWord		= FALSE1;
	R50N1.LockRelayWord			= RW_LKPI50N1;							//��α����̵����� 
	R50N1.RSTRelayWord			= RW_LKRST50N1;							//���ر����̵����� 
	R50N1.LoopCtrlRelayWord		= RW_LPC50N1;						//����ѭ�����Ƽ̵����� 
	
	R50N1.wReportNum			= R_50N1;								//��������������� 
	R50N1.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R50N1.dwK35					= NULL;
	R50N1.RptSerialNum			= &RptSerialNumOfAct;
	R50N1.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50N1.wPickUpReportNum		= R_50N1_PI;						//��α������к�
	R50N1.wResetReportNum		= R_50N1_RES;						//���ر������к�
	R50N1.wAccPickUpRptNum		= 0xFFFF;							//������α������к�
	R50N1.wAccResetRptNum		= 0xFFFF;							//���ٷ��ر������к�
	//**********************************************************************	

//	R50N1.dwRelTimer			= 0;			 				//��ʱ�� 
	R50N1.byRelFCounter			= 0;						//���ϼ����� 
	R50N1.byRelRCounter			= 0;						//���ؼ����� 
	R50N1.wExcepWordNum			= 8;
	R50N1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ�������������Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50N2()
{	
	R50N2.pElecSet				= RS_pn50N2C;          						//��������������ֵ 
	R50N2.pTimeSet				= RS_pn50N2D;				 				//��������ʱ�䶨ֵ 
	R50N2.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50N2.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50N2.pAccActTimeSet		= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R50N2.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50N2.pRelCFG				= CFG_pby50N2;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50N2.pRelSW				= SW_pby50N2;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50N2.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50N2.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50N2.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50N2.pRelFDCFG				= &RelCFGCLR;
	R50N2.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50N2.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R50N2.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50N2.pRel47UCFG			= &RelCFGCLR;
	R50N2.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50N2.pRelElec 				= &I0;						//��ǰ����ָ�� 		
	R50N2.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50N2.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50N2.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50N2.StartRelayWord		= RW_PI50N2;							//����������μ̵����� 
	R50N2.ActRelayWord			= RW_TR50N2;							//�������������̵����� 
	R50N2.AccRelayWord			= FALSE1;							//���������̵����� 
	R50N2.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R50N2.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R50N2.PRewardRelayWord  	= FALSE1;					//���ʷ�����̵����� 
	R50N2.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R50N2.R47ULKRelayWord		= FALSE1;
	R50N2.LockRelayWord			= RW_LKPI50N2;							//��α����̵����� 
	R50N2.RSTRelayWord			= RW_LKRST50N2;							//���ر����̵����� 
	R50N2.LoopCtrlRelayWord		= RW_LPC50N2;						//����ѭ�����Ƽ̵����� 
	
	R50N2.wReportNum			= R_50N2;								//��������������� 
	R50N2.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R50N2.dwK35					= NULL;
	R50N2.RptSerialNum			= &RptSerialNumOfAct;
	R50N2.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50N2.wPickUpReportNum		= R_50N2_PI;						//��α������к�
	R50N2.wResetReportNum		= R_50N2_RES;						//���ر������к�
	R50N2.wAccPickUpRptNum		= 0xFFFF;							//������α������к�
	R50N2.wAccResetRptNum		= 0xFFFF;							//���ٷ��ر������к�
	//**********************************************************************	

//	R50N2.dwRelTimer			= 0;			 				//��ʱ�� 
	R50N2.byRelFCounter			= 0;						//���ϼ����� 
	R50N2.byRelRCounter			= 0;						//���ؼ����� 
	R50N2.wExcepWordNum			= 8;
	R50N2.wInfo 				= 0;
}

/********************************************************************************
*����: ��ʼ��ĸ�䱣��Ԫ��
*����: ��
*����: ��
*��ע:
**********************************************************************************/
void CRelayTask::InitR50ISH()
{
	R50ISH.pElecSet				= RS_pn50ISHC;								//��������������ֵ 
	R50ISH.pTimeSet				= RS_pn50ISHD;								//��������ʱ�䶨ֵ 
	R50ISH.pHarmKhr1Set			= NULL; 						//��������г����Ȩϵ�� 
	R50ISH.pVolLockSet			= NULL; 						//����������ѹ������ֵ 
	R50ISH.pAccActTimeSet		= RS_pn50ISHD; 					//������������ٶ���ʱ�䶨ֵ 
	R50ISH.pH2Coef				= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	R50ISH.pRelCFG				= CFG_pby50I_SH;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50ISH.pRelSW				= SW_pby50ISH;							//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50ISH.pRelHLockCFG			= &RelCFGCLR;							//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50ISH.pRelHSuprCFG			= &RelCFGCLR;						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50ISH.pRelPDCFG 			= &RelCFGCLR;							//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50ISH.pRelFDCFG 			= &RelCFGCLR;					//
	R50ISH.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50ISH.pRelAccCFG			= CFG_pby50I_SH;							//���޺���ٹ���------1:�У�  2:�� 
	R50ISH.pRel47UCFG			= &RelCFGCLR;					//��ѹ��������
	R50ISH.pRelFLCFG 			= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50ISH.pSysTimer 			= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ��	
	
	R50ISH.pRelElec				= &Ip;						//��ǰ����ָ��		
	R50ISH.pRelHarm3 			= NULL; 					//��ǰ����г��ָ��	
	R50ISH.pRelHarm5 			= NULL; 					//��ǰ���г��ָ��	
	R50ISH.pRelVolt				= NULL; 					//��ǰ���������ĵ�ѹ��ָ�� 

	R50ISH.StartRelayWord		= RW_PI50ISH;							//����������μ̵����� 
	R50ISH.ActRelayWord			= RW_TR50ISH;							//�������������̵����� 
	R50ISH.AccRelayWord			= RW_TSH;							//���������̵�����,��һ��ʱ��̵����֣���ʱ�����Զ����� 
	R50ISH.AccActRelayWord		= RW_TR50ISH;						//����ٶ����̵����� 
	R50ISH.PForwardRelayWord 	= FALSE1;						//����������̵����� 
	R50ISH.PRewardRelayWord		= FALSE1;
	R50ISH.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	R50ISH.R47ULKRelayWord		= FALSE1;
	R50ISH.LockRelayWord 		= RW_LKPI50ISH;							//��α����̵����� 
	R50ISH.RSTRelayWord			= RW_LKRST50ISH; 						//���ر����̵����� 
	R50ISH.LoopCtrlRelayWord 	= RW_LPC50ISH;						//����ѭ�����Ƽ̵����� 
	
	R50ISH.wReportNum			= R_50ISH;								//��������������� 
	R50ISH.wAccReportNum 		= R_50ISH;							//���ٶ���������� 
	
	R50ISH.dwK35 				= NULL;
	R50ISH.RptSerialNum			= &RptSerialNumOfAct;
	R50ISH.bMakeActRpt			= TRUE;
	R50ISH.wPickUpReportNum		= R_50ISH_PI;						//��α������к�
	R50ISH.wResetReportNum		= R_50ISH_RES;						//���ر������к�
	R50ISH.wAccPickUpRptNum		= R_50ISH_PI;							//������α������к�
	R50ISH.wAccResetRptNum		= R_50ISH_RES;							//���ٷ��ر������к�
	//**********************************************************************	
//	R50ISH.dwRelTimer			= 0;							//��ʱ�� 
	R50ISH.byRelFCounter 		= 0;						//���ϼ����� 
	R50ISH.byRelRCounter 		= 0;						//���ؼ�����	
	R50ISH.wExcepWordNum 		= 8;
	R50ISH.wInfo 				= 0;
}

/************************************************************************************************
*����: ��ʼ����ѹ����Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitR27U()
{
	R27U.pELeSet						= RS_pn27U;						//������ֵ 
	R27U.pTimeSet						= RS_pn27D;						//ʱ�䶨ֵ 
	R27U.pLockCurSet					= RS_pn27_LK37;					//����������ֵ 

	R27U.pRelCFG						= CFG_pby27U;					  	 //Ƿ���������� 
	R27U.pRelSW							= SW_pby27;					  	 //Ƿ��������ѹ�� 
	R27U.pRelAlarmCFG					= &RelCFGCLR;						//Ƿ������Ͷ��բ 
	R27U.pRelCurLockCFG					= &RelCFGSET;
	R27U.pSysTimer						= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ�� 

	R27U.pRelElec						= &Up;						//��ǰ�ĵ���ָ�� 
	R27U.pRelLockElec					= &Ip;					//�������� 
	R27U.ActRelayWord					= RW_TR27U;					//Ƿ�����������̵����� 
	R27U.StartRelayWord					= RW_PI27U;					//Ƿ��������μ̵����� 
	R27U.AlarmRelayWord					= FALSE1;					//Ƿ�������澯�̵����� 
	R27U.LockRelayWord					= RW_LKPI27;				//��α����̵����� 
	R27U.RSTRelayWord					= RW_LKRST27;					//���Ϸ��ر����̵����� 
	R27U.LoopCtrlRelayWord				= RW_LPC27U;				//����ѭ�����Ƽ̵����� 
	R27U.wR52BFRealyword				= RW_52BF;					//��·����բʧ�̵ܼ�����
	R27U.wActRptNum						= R_27TR;					//Ƿ����������������� 
	R27U.wAlarmRptNum					= 0xFFFF;					//Ƿ�������澯������� 
	R27U.wTRFailRptNum					= R_TRFAIL;
	R27U.wPickUpRptNum					= R_ACTPICK;
	R27U.RptSerialNum					= &g_RptSerialNum;
	
	R27U.R27RptSerialNum				= 0;
//	R27U.dwRelTimer						= 0;						//Ƿ��������ʱ�� 	
	R27U.byRelFCounter					= 0;					//���ϼ����� 
	R27U.byRelRCounter					= 0;					//���ؼ����� 
	R27U.bHaveMake52BFRptFlag			= FALSE;
	R27U.wExcepWordNum					= 14;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
/************************************************************************************************
*����: ��ʼ���غ�բ����Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRel79()
{
	Rel79.R79NumSet						= RS_pn79SH;						//Ͷ����غ�բ���� 
	Rel79.R79T1Set						= RS_pn79D1;							//��һ���غ�բ����ʱ�� 
	Rel79.R79T2Set						= RS_pn79D2;							//�ڶ����غ�բ����ʱ�� 
	Rel79.R79RSTSet						= RS_pn79CT;							//�غ�բ��ŵ�ʱ�� 
	Rel79.R79SOTFTSet					= RS_pnSOTFD;						//���ڹ��ϼ����ʱ����Ϊ�����Ͷ��ʱ�� 
	Rel79.R79PISTSet					= AS_pnTCB;						//�غ�բ����������ʱ����Ϊ��·��������ʱ 
	Rel79.R79CLSTSet					= RS_pn79CLSD;						//�غ�բ����������ʱ 

	
	Rel79.R79SCFG						= CFG_pby79UC;	//͵���غ�բ����
	Rel79.R79SW							= SW_pby79;
	Rel79.R79RW							= RW_CFG79;	//	�����÷����غ�բ�����̵�������

	Rel79.R79CYSRW						= RW_79CYS;					//�غ�բ����״̬�̵�����--�Ա��̵ּ����� 
	Rel79.R79RSSRW						= RW_79RSS;					//�غ�բ����״̬�̵�����--�Ա��̵ּ����� 
	Rel79.R79LOSRW						= RW_79LOS;					//�غ�բ����״̬�̵�����--�Ա��̵ּ����� 

	Rel79.R79SH0RW						= RW_79SH0;					//�غ�բ����״̬�̵�����һ--����̵����� 		
	Rel79.R79SH1RW						= RW_79SH1;					//�غ�բ����״̬�̵����ֶ�--����̵����� 

	Rel79.R79PIRW						= RW_79PI;					//�غ�բ�����߼��̵�����--�Ա��̵ּ����� 
	Rel79.R79PISRW						= RW_79PIS;					//�غ�բ���������߼��̵�����--�м�̵����� 
	Rel79.R79TORW						= RW_79TO;					//�غ�բ��ʱ���߼��̵�����--����̵����� 
	Rel79.R79CLSRW						= RW_79CLS;					//�غ�բ���������߼��̵�����--�м�̵����� 
	Rel79.R79CLRW						= RW_79CL;					//�غ�բ�����߼��̵�����--�Ա��̵ּ����� 
	Rel79.R79CLCRW						= RW_79CLC;					//�غ�բ����������߼��̵�����--�м�̵����� 
	Rel79.R79LOCKRW						= RW_79LK;					//�غ�բ�����߼��̵�����--�м�̵����� 
	Rel79.R79SOTFRW						= RW_TSOTF;					//�غ�բ���ڹ����߼��̵�����--����̵����� 
	Rel79.R79TRCHECK					= RW_79TRCK;				//�غ�բ���������жϼ̵�����

	Rel79.R7952A						= RW_52A;						//�غ�բ��·����λ�̵����� 
	Rel79.R7952B						= RW_52B;						//�غ�բ��·����λ�̵����� 
	Rel79.dwSysTimer					= &dwRelayMSCnt;;				//ϵͳ��ʱ��ָ�� 
	Rel79.wRpt79_1						= R_R79_1;					//һ���غ�բ��������� 
	Rel79.wRpt79_2						= R_R79_2;					//�����غ�բ��������� 
	Rel79.wRpt79Fail					= R_FA_79;				//�غ�բʧ�ܱ���
	Rel79.wRpt79LK_1					= R_R79LK_1;				//�غ�բ�������������
	Rel79.wRpt79LK_2					= R_R79LK_2;				//�غ�բ�������������
	Rel79.wRpt79_1_SUCC					= R_R79SUCC_1;			//һ���غ�բ�ɹ�����  by yanzh 2013��6��18�� 17:31:57
	Rel79.wRpt79_1_FAIL					= R_R79FAIL_1;			//һ���غ�բʧ�ܱ���
	Rel79.wRpt79_2_SUCC					= R_R79SUCC_2;			//�����غ�բ�ɹ�����
	Rel79.wRpt79_2_FAIL					= R_R79FAIL_2;			//�����غ�բʧ�ܱ���
	Rel79.wRpt79PI_1					= R_R79PI_1;				//�غ�բ��������	
	Rel79.wRpt79PI_2					= R_R79PI_2;				//�غ�բ��������
	Rel79.wRpt79NPI_1					= R_R79NPI_1;				//�غ�բδ��������
	Rel79.wRpt79NPI_2					= R_R79NPI_2;				//�غ�բδ��������	
	Rel79.wPickUpRptNum					= R_ACTPICK;

	Rel79.RptSerialNum					= &RptSerialNumOfAct;
	Rel79.SysRptSerialNum				= &g_RptSerialNum;
	Rel79.pFaultOnFlag					= &FaultOnFlag;	
	Rel79.pR79PIRptFlag 				= &m_bR79PickUpRpt;

	
	Rel79.R79RSFLAG						= FALSE;					//�غ�բ��ŵ���ʱ������־ 
	Rel79.R79PISFLAG					= FALSE;					//�غ�բ����������ʱ������־ 
	Rel79.R79TFLAG						= FALSE;					//�غ�բ������ʱ������־ 
	Rel79.R79CLSFLAG					= FALSE;					//�غ�բ����������ʱ������־ 
	Rel79.R79LKFlag						= FALSE;					//�غ�բ�����������־	
	Rel79.R79CLFlag						= FALSE;					//�غ�բ������־
//	Rel79.R79Timer						= 0;					//�غ�բ������ʱ��ʱ�� 
	Rel79.R79ShotCnt					= 0;					//�غ�բ�Ѿ��������� 	
	Rel79.wExcepWordNum					= 16;				//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ12 

}
void CRelayTask::InitRel25(void)
{
	Rel25.p25_27Set                     = RS_pn25_27;  //��բ����ѹ
	Rel25.p25DTSet                      = RS_pn25DT;   //ͬ�ڼ��ʱ��
	Rel25.p25VULSet                     = RS_pn25VUL;  //��ͬ�ڵ�ѹ����
	Rel25.p25VLLSet                     = RS_pn25VLL;  //��ͬ�ڵ�ѹ����
	Rel25.p25VDSet                      = RS_pn25VD;   //ͬ������ѹ��
	Rel25.p25FDSet                      = RS_pn25FD;   //ͬ������Ƶ��
	Rel25.p25PASet                      = RS_pn25PA;   //ͬ�ڹ���
	Rel25.p25PHSet                      = RS_pn25PH;   //ͬ�����
	Rel25.p52CLT						= RS_pn52CLT;
	Rel25.p25RCFG                       = CFG_pby25 ;  //�ֺϼ�ͬ������
	Rel25.p79_25RCFG					= CFG_pby79_25;
	Rel25.p25RSW                        = SW_pby25;    //�ֺϼ�ͬ����ѹ��

	Rel25.pUsyn							= &Usyn;
	Rel25.pU1							= &U1;
	Rel25.pDA							= &m_DA;		    //�ǲ�
	Rel25.pDV							= &m_DV; 	//ѹ��
	Rel25.pDF							= &m_DF;	//Ƶ��
	Rel25.pPA							= &m_FA;         //��ǰ��

    Rel25.w52A                          = RW_52A;       //��λ�̵�����
    Rel25.w25LK                         = RW_25LK;     //��ͬ�ڱ����̵�����
    Rel25.w79_25PI                      = RW_79_25PI;      //��ͬ�������̵�����
    Rel25.wMCL_25PI                     = RW_MCL_25PI;      //��ͬ�������̵�����
    Rel25.w25ActRW                      = RW_25CL;      //��ͬ�ڶ����̵�����
    Rel25.w25INT						= RW_25INT;

	Rel25.R25SucRptNum					= R_SU_25;	//��ͬ�ڳɹ��������
	Rel25.R25FalRptNum					= R_FA_25;	//��ͬ��ʧ�ܱ������
	Rel25.R25LKRptNum					= R_25LK;	//��ͬ�ڱ������������
	Rel25.R25_27RptNum					= R_WL_CL; //��ѹ�պϱ������
	Rel25.R25INTRptNum					= R_25BK;
	Rel25.RptSerialNum					= &RptSerialNumOfAct;					//ϵͳ�������к� 
	Rel25.pSysTimer						= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	
	Rel25.bManSynFlag                    = FALSE;        //�ֺϼ�ͬ�ڱ�־
	Rel25.bJudgeNetend                   = FALSE;        //���ж���ɱ�־
	Rel25.bSameNetFlag                   = FALSE;        //ͬ����־
	Rel25.bDiffNetFlag                   = FALSE;        //������־
	Rel25.bSigleNetFlag                  = FALSE;        //������־
	Rel25.bJudgeSameNetPIFlag            = FALSE;        //ͬ���ж���ʱ��α�־
	Rel25.bJudgeDiffNetPIFlag            = FALSE;        //�����ж���ʱ��α�־
	Rel25.bJudgeSigleNetPIFlag           = FALSE;        //�����ж���ʱ��α�־
	Rel25.bBusPTBreakFlag               = FALSE;         //ĸ��PT���߱�־
	Rel25.bHaveMakeLKRptFlag			= FALSE;     		//��ͬ�ڱ����������־
	Rel25.bHaveMakeINTRptFlag			= FALSE;       		//��ͬ�ڱ��жϱ����־
	Rel25.bDVOutLimitFlag				= FALSE;

//	Rel25.dwInitSingleNetChkTimer        = 0;            //�����ж�ʱ�䶨ʱ��
//	Rel25.dwInitSameleNetChkTimer        = 0;            //ͬ���ж�ʱ�䶨ʱ��
//	Rel25.dwInitDiffleNetChkTimer        = 0;            //�����ж�ʱ�䶨ʱ��
//	Rel25.dwSynCheckTime                 = 0;            //��ͬ��ʱ�䶨ʱ��
//	Rel25.dwSynClosePulseTimer           = 0;            //��ͬ�ں�բ����̵�����
	Rel25.wExcepWordNum                 = 36;
}

/************************************************************************************************
*����: ȡ��ģֵ��������
*����: ��
*����: ��
*��ע:
************************************************************************************************/

TRelElecValPar CRelayTask::ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3)
{
	if(ele1.Mod>=ele2.Mod)
	{
		if(ele1.Mod>=ele3.Mod)
		{
			
			return ele1;
		}
		else
			return ele3;		
	}
	else
	{
		if(ele2.Mod>=ele3.Mod)
			return ele2;
		else
			return ele3;
	}
}
/************************************************************************************************
*����: ȡ��ģֵ�������������������
*����: ��
*����: ��
*��ע:
************************************************************************************************/

TRelElecValPar CRelayTask::ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase)
{
	if(ele1.Mod>=ele2.Mod)
	{
		if(ele1.Mod>=ele3.Mod)
		{
			phase = RELAY_PHASE_A;
			return ele1;
		}
		else
		{
			phase = RELAY_PHASE_C;
			return ele3;
		}
	}
	else
	{
		if(ele2.Mod>=ele3.Mod)
		{
			phase = RELAY_PHASE_B;
			return ele2;
		}
		else
		{
			phase = RELAY_PHASE_C;
			return ele3;
		}
	}
}
/************************************************************************************************
*����: ȡ��ģֵ��С������
*����: ��
*����: ��
*��ע:
************************************************************************************************/
TRelElecValPar CRelayTask::ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3)
{
	if(ele1.Mod<=ele2.Mod)
	{
		if(ele1.Mod<=ele3.Mod)
			return ele1;
		else
			return ele3;
	}
	else
	{
		if(ele2.Mod<=ele3.Mod)
			return ele2;
		else
			return ele3;
	}
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
	BOOL haveRelayInput = FALSE;
	BYTE  pChannel[]={CHANNEL_6521_IA,CHANNEL_6521_IB,CHANNEL_6521_IC,CHANNEL_6521_I0,CHANNEL_6521_I1};
	if(ReadRelayWord(RW_52BF))  //��·����բʧ�ܣ������б��������жϴ��� 
	{
		WriteRelayWord(RW_PICKUP, RELAY_OFF);
		return FALSE;
	}
	
	if((*R50I0.pRelCFG == CFG_STATUS_SET) && (*R50I0.pRelSW == SW_STATUS_SET))   //�����ٶϱ���
	{
		haveRelayInput = TRUE;
		if(R50I0.pRelElec->Mod > *R50I0.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if(((*R50I1.pRelCFG == CFG_STATUS_SET)||(*R50I1.pRelAccCFG == CFG_STATUS_SET)) && (*R50I1.pRelSW == SW_STATUS_SET))   //����1�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R50I1.pRelElec->Mod > *R50I1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if(((*R50I2.pRelCFG == CFG_STATUS_SET)||(*R50I2.pRelAccCFG == CFG_STATUS_SET)) && (*R50I2.pRelSW == SW_STATUS_SET))   //����2�α���Ͷ�� 
	{		
		haveRelayInput = TRUE;
		if(R50I2.pRelElec->Mod > *R50I2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if(((*R50I3.pRelCFG == CFG_STATUS_SET)||(*R50I3.pRelAccCFG == CFG_STATUS_SET))&& (*R50I3.pRelSW == SW_STATUS_SET))   //����3�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R50I3.pRelElec->Mod > *R50I3.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R50N1.pRelCFG == CFG_STATUS_SET) && (*R50N1.pRelSW == SW_STATUS_SET))   //����1�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R50N1.pRelElec->Mod > *R50N1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R50N2.pRelCFG == CFG_STATUS_SET) && (*R50N2.pRelSW == SW_STATUS_SET))   //����1�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R50N2.pRelElec->Mod > *R50N2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R50ISH.pRelCFG == CFG_STATUS_SET) && (*R50ISH.pRelSW == SW_STATUS_SET))   //ĸ�߳�籣��Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R50ISH.pRelElec->Mod > *R50ISH.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}

	//ͻ������������
	//ͻ������������,�б���Ͷ��
	if(haveRelayInput)
	{
		 ReadPreSingleData(dwPickDataBuf,pChannel,5,RELAY_SAMP_INTERVAL);
		 for(LONG j=0;j<5;j++)
		 {	
			//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
			PickCheck = 0;	
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickDataBuf[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));
				
				if(lPickValue>*RS_pnPIC)
				{
					PickCheck++;
					if(PickCheck==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	

						WriteRelayWord(RW_PICKUP, RELAY_ON);
						ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
						RptSerialNumOfStart = g_RptSerialNum;
						PickCheck =0;
						PickCheckNext[j] =0;
						return TRUE;	
					}
					if(i<2)
					{
						PickCheckNext[j]++;
						if(PickCheckNext[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime);
							PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);	
							
							WriteRelayWord(RW_PICKUP, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
							RptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
							RptSerialNumOfStart = g_RptSerialNum;
							PickCheck =0;
							PickCheckNext[j] =0;
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						PickCheckNext[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						PickCheckNext[j]++;
				}
				else
				{
					PickCheck =0;
					PickCheckNext[j]=0;
				}
			 }
		 }
	}
	WriteRelayWord(RW_PICKUP, RELAY_OFF);
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
	if(!FaultOnFlag)   						//��δ������ϴ���
	{		
		RelayPickUpCHK();					//ִ���������
		if(ReadRelayWord(RW_PICKUP))
		{
			FaultSINum = 0;       //������������ʼ�����ϴ���������������
			FaultOnFlag=TRUE;
			ResetAllRelay(TRUE);
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(FaultSINum<READ_POINT_NUM)					//���ϴ���������������С��һ�ܲ���������
	{
		return;
	}	
	else
	{
		if(!HaveMakeStartRptFlag)   //������ϵ���������������д����ϵ���
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}

		if((*R50I0.pRelPDCFG == CFG_STATUS_SET)||(*R50I1.pRelPDCFG == CFG_STATUS_SET)||(*R50I2.pRelPDCFG == CFG_STATUS_SET)||(*R50I3.pRelPDCFG == CFG_STATUS_SET))
		{
			if(m_Phase == RELAY_PHASE_A)
			{
				if(!m_bPDCheckA)
				{
					if(!ReadRelayWord(RW_ARPT1))				//PT��������£����ʷ����������ʧЧ
						m_bPDCheckA = PowerDirCheck_90(&Ubc,&Ia,dwPDDataBuf,*RS_pn67ANG,*CF_pnUp,RW_APF,RW_APR,CHANNEL_6521_UBC,TRUE);
					else
					{						
						WriteRelayWord(RW_APF, RELAY_OFF);
						WriteRelayWord(RW_APR, RELAY_OFF);
					}
				}
			}
			else if(m_Phase == RELAY_PHASE_B)
			{
				if(!m_bPDCheckB)					
				{
					if(!ReadRelayWord(RW_ARPT1))				//PT��������£����ʷ����������ʧЧ
						m_bPDCheckB = PowerDirCheck_90(&Uca,&Ib,dwPDDataBuf,*RS_pn67ANG,*CF_pnUp,RW_BPF,RW_BPR,CHANNEL_6521_UCA,TRUE);
					else
					{						
						WriteRelayWord(RW_BPF, RELAY_OFF);
						WriteRelayWord(RW_BPR, RELAY_OFF);
					}
				}
			}
			else if(m_Phase == RELAY_PHASE_C)
			{
				if(!m_bPDCheckC)
				{
					if(!ReadRelayWord(RW_ARPT1))				//PT��������£����ʷ����������ʧЧ
						m_bPDCheckC = PowerDirCheck_90(&Uab,&Ic,dwPDDataBuf,*RS_pn67ANG,*CF_pnUp,RW_CPF,RW_CPR,CHANNEL_6521_UAB,TRUE);
					else
					{						
						WriteRelayWord(RW_CPF, RELAY_OFF);
						WriteRelayWord(RW_CPR, RELAY_OFF);
					}
				}
			}			
		}
		GeneralOverValueRelay(&R50I0);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&R50I1);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&R50I2);				//���ù���I�α����ӳ���
		GeneralOverValueRelay(&R50I3);				//���ù���III�α����ӳ���
		GeneralOverValueRelay(&R50N1);				//���ù���III�α����ӳ���
		GeneralOverValueRelay(&R50N2);
		GeneralOverValueRelay(&R50ISH);				//ĸ�߳�籣���ӳ���
		
		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveMakeTripFailRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//����·����բʧ�ܱ���
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}

		if(Rel79.R79TFLAG)						//�غ�բ�Ѿ�������ʱ״̬,�ȴ��غ�բ����
			return;
		//�غ�բ�����̵�������λ���ȴ������ָ����������ָ�������һ�η������ϣ��ü̵����ֱ���λ
		//������δ�ָ����߻ָ���δ��������������ʱ�䵽�󣬸ü̵�����Ҳ����λ
		//���ڸ�ʱ���ڵ����ָ�������FaultSINum������
		if(ReadRelayWord(Rel79.R79CLRW))  			
		{
			if(CurRecoverFlag)				 ///�غ�բ����������Ѿ��ָ�
				return;
			else
			{
				if((!ReadRelayWord(RW_37IA))||(!ReadRelayWord(RW_37IB))||(!ReadRelayWord(RW_37IC)))	//��������̵������Ƿ���λ
				{
					FaultSINum = 0;		//���³�ʼ�����ϴ���������������
					CurRecoverFlag = TRUE;    				//�õ����ָ���־
					return;
				}				
				return;
			}
		}
		// 1���б���ѭ�����Ƽ̵����ֱ���λ���ȴ��������������˳�
		if(ReadRelayWord(R50I0.LoopCtrlRelayWord)||ReadRelayWord(R50I1.LoopCtrlRelayWord)||ReadRelayWord(R50I2.LoopCtrlRelayWord)|| \
			ReadRelayWord(R50I3.LoopCtrlRelayWord)||ReadRelayWord(R50N1.LoopCtrlRelayWord)||ReadRelayWord(R50N2.LoopCtrlRelayWord)|| \
			ReadRelayWord(R50ISH.LoopCtrlRelayWord))
		{			
			// 1.1 �����Ѿ�����������բʧ��
			if(ReadRelayWord(RW_52BF))
			{
				WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
				FaultCheckOutFlag = FALSE;					//�������ϱ�־
				FaultOnFlag = FALSE;						//�����ѭ�������־
				HaveMakeStartRptFlag= FALSE;				//�����������ϵ������������־
				m_bIsHaveMakePTRpt = FALSE;					//������PT���߱������뱣���澯�����־				
				m_bFaultLKFlag = FALSE; 					//���������޶���ʱ����ʱ��������־	
				m_bPDCheckA = FALSE;							//�����������书�ʷ����жϱ�־
				m_bPDCheckB = FALSE;
				m_bPDCheckC = FALSE;
				CurRecoverFlag = FALSE;						//���غ�բ����������ָ���־
				m_bHaveMakeTripFailRpt = FALSE;
				DB_RelayEnd(RptSerialNumOfAct); 			//�����ݿ���д���������	
				return;
			}
			// 1.2 �ȴ�����������ˢ�¹���������ʱ�����ü�����ϱ�־
			else
			{
				FaultLockTime = dwRelayMSCnt;		
				FaultCheckOutFlag = TRUE;
				return;
			}
		}
		// 2���ޱ���ѭ�����Ƽ̵����ֱ���λ����ʱ�����Ѷ���������û�������κα�����������
		else				
		{
			// 2.1 ������ϱ�־����λ����ʾ����ѭ�����Ƽ̵�������������λ--��һ��������--�ȴ�RS_pnSOTFTʱ����˳�
			if(FaultCheckOutFlag)     
			{				
				if(ReadRelayWord(RW_T52BF)) //��·����բʧ���ж�������ʱ
				{
					FaultLockTime = dwRelayMSCnt;	
					return;
				}
				if(dwRelayMSCnt -FaultLockTime > 1000 )  //�޹���ʱ�䳬�����ڹ��ϼ��ʱ��
				{						
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
					CurRecoverFlag	 = FALSE;					//���غ�բ����������ָ���־		
					m_bIsHaveMakePTRpt = FALSE;					//������PT���߱������뱣���澯�����־					
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					m_bPDCheckA = FALSE;							//�����������书�ʷ����жϱ�־
					m_bPDCheckB = FALSE;
					m_bPDCheckC = FALSE;
					
					m_bR79PickUpRpt = FALSE;					
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct);				//�����ݿ���д���������
					ResetAllRelay(TRUE);
					return;					
				}
				else
					return;
			}
			// 2.2 ������ϱ�־δ����λ����ʾ����ѭ�����Ƽ̵������ڱ��ι��ϴ����д�δ����λ���ȴ�0.5����˳�
			else
			{
				if(!m_bFaultLKFlag)//�����ո���������¼��ʼʱ��
				{	
					FaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				if(dwRelayMSCnt -FaultLockTime>500 )	
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־						
					CurRecoverFlag	 = FALSE;					//���غ�բ����������ָ���־		
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					m_bIsHaveMakePTRpt = FALSE;					//������PT���߱������뱣���澯�����־
					m_bPDCheckA = FALSE;							//�����������书�ʷ����жϱ�־
					m_bPDCheckB = FALSE;	
					m_bPDCheckC = FALSE;	
					
					m_bR79PickUpRpt = FALSE;					
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct); 			
					ResetAllRelay(TRUE);
					return;					
				}
				else	//��ʱδ����ֱ�ӷ���
					return;
			}			
		}		
	}
}
void CRelayTask::Process52BF(void)
{
	if(ReadRelayWord(RW_52BF))
	{
		m_b52BFFLAG = TRUE;
	}
	if(m_b52BFFLAG)
	{
		if(!ReadRelayWord(RW_52BF))
		{
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&R27U);
			m_b52BFFLAG = FALSE;			
			m_bR79PickUpRpt = FALSE;					
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
	ResetOverValueRelay(&R50I0,resetFlag);
	ResetOverValueRelay(&R50I1,resetFlag);
	ResetOverValueRelay(&R50I2,resetFlag);
	ResetOverValueRelay(&R50I3,resetFlag);
	ResetOverValueRelay(&R50N1,resetFlag);
	ResetOverValueRelay(&R50N2,resetFlag);
	ResetOverValueRelay(&R50ISH,resetFlag);
}
void CRelayTask::PTCheck()
{	
	if(*CFG_pbyPTBR == CFG_STATUS_CLR) //PT����δͶ��
	{
		WriteRelayWord(RW_PIPT1,RELAY_OFF);
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_BUSDX_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT2,RELAY_OFF);		
		if(ReadRelayWord(RW_ARPT2))
		{			
			MakeAlarmRpt(R_LINEDX_RES);
			WriteRelayWord(RW_ARPT2,RELAY_OFF);
		}
		m_BusPTTimer = 0;
		m_LinePTTimer = 0;
		return;
	}
//ĸ��PT�����ж�
	if(((ULK.Mod<*RS_pnPT1_V)&&(Ip.Mod>*RS_pnPT1_C))||(NU2.Mod>m_U2Set))
	{
		if(ReadRelayWord(RW_PIPT1))
		{
			if(!ReadRelayWord(RW_ARPT1))
			{
				if(dwRelayMSCnt - m_BusPTTimer>*RS_pnPT1_D)
				{
					WriteRelayWord(RW_ARPT1,RELAY_ON);
					MakeAlarmRpt(R_BUSDX);
				}
			}
		}
		else
		{
			WriteRelayWord(RW_PIPT1,RELAY_ON);
			m_BusPTTimer = dwRelayMSCnt;
		}
	}
	//ֻ���ڵ�ѹ�ָ�����ʱ������
	if((ULK.Mod>=*RS_pnPT1_V)&&(NU2.Mod<=m_U2Set))
	{
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_BUSDX_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
		m_BusPTTimer = 0;
	}
//	else //ֻ���ڵ�ѹ�ָ�����ʱ������
//	{
//		WriteRelayWord(RW_ARPT1,RELAY_OFF);
//		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
//		m_BusPTTimer = 0;
//	}

//��·PT�����ж�
	TRelElecValPar UlMax,UlMin;
	BOOL	Condition1 = FALSE;
	BOOL	Condition2 = FALSE;
	BOOL	Condition3 = FALSE;
	UlMax = ElecMax(U1,U2,U3);
	UlMin = ElecMin(U1,U2,U3);
	if((UlMin.Mod<*RS_pnPT2_V2)&&((Up.Mod>*RS_pnPT2_V1)&&(Ip.Mod>*RS_pnPT2_C)&&(ReadRelayWord(RW_52A)))) //ĸ����ѹ�����ҿ��ش��ں�λ
			Condition1 = TRUE;
	if((UlMin.Mod<*RS_pnPT2_V2)&&(UlMax.Mod>*RS_pnPT2_V2))	//��·�������ѹ���Ĵ��ڶ�ֵ����С��С�ڶ�ֵ
		Condition2 = TRUE;
	if(((U1.Mod>*RS_pn25VLL)&&(U2.Mod>*RS_pn25VLL)&&(U3.Mod<*RS_pn25VLL))||    //��·���������ѹ�����ҽ���һ��С�ڼ�ͬ�ڵ�ѹ����
		((U1.Mod>*RS_pn25VLL)&&(U2.Mod<*RS_pn25VLL)&&(U3.Mod>*RS_pn25VLL))||   
		((U1.Mod<*RS_pn25VLL)&&(U2.Mod>*RS_pn25VLL)&&(U3.Mod>*RS_pn25VLL)))
		Condition3 = TRUE;

	if(Condition1||Condition2||Condition3)
	{
		if(ReadRelayWord(RW_PIPT2))
		{
			if(!ReadRelayWord(RW_ARPT2))
			{
				if(dwRelayMSCnt - m_LinePTTimer>*RS_pnPT2_D)
				{
					WriteRelayWord(RW_ARPT2,RELAY_ON);
					MakeAlarmRpt(R_LINEDX);
				}
			}
		}
		else
		{
			WriteRelayWord(RW_PIPT2,RELAY_ON);
			m_LinePTTimer = dwRelayMSCnt;
		}
	}
	else
	{		
		if(ReadRelayWord(RW_ARPT2))
		{			
			MakeAlarmRpt(R_LINEDX_RES);
			WriteRelayWord(RW_ARPT2,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT2,RELAY_OFF);
		m_LinePTTimer = 0;
	}
	
}

void CRelayTask::R59NAlarm() //��ѹ�澯
{
	static DWORD R59NTimer = 0;
	if(*CFG_pby59N_74 == CFG_STATUS_CLR)
	{
		WriteRelayWord(RW_PI59N,RELAY_OFF);
		WriteRelayWord(RW_TR59N,RELAY_OFF);
		R59NTimer = 0;
		return;
	}
	if(U0.Mod>*RS_pn59NU)
	{
		if(!ReadRelayWord(RW_TR59N))
		{
			if(!ReadRelayWord(RW_PI59N))
			{
				R59NTimer = dwRelayMSCnt;
				WriteRelayWord(RW_PI59N, RELAY_ON);
			}
			else
			{
				if((dwRelayMSCnt - R59NTimer)>*RS_pn59ND)
				{
					WriteRelayWord(RW_TR59N, RELAY_ON);
					MakeAlarmRpt(R_59NAR);
				}
			}
		}
	}
	else
	{
		WriteRelayWord(RW_PI59N,RELAY_OFF);
		WriteRelayWord(RW_TR59N,RELAY_OFF);
		R59NTimer = 0;
	}
}

void CRelayTask::Rel25Check(void) //��ͬ��
{
	CaluDF_DA_DV();  //����ͬ��ѹ��,�ǲƵ��

	if((*Rel25.p25RCFG == CFG_STATUS_CLR)&&(*Rel25.p79_25RCFG== CFG_STATUS_CLR)||(*Rel25.p25RSW == SW_STATUS_CUT)) //��ͬ��δͶ��
		ResetRel25();

	if(ReadRelayWord(Rel25.w25ActRW))  //��ͬ�ڶ�����
	{
		if(*Rel25.pSysTimer - Rel25.dwSynClosePulseTimer >= 1000)
			ResetRel25();
		return;
	}
	if(ReadRelayWord(Rel25.w79_25PI)||ReadRelayWord(Rel25.wMCL_25PI))
	{		
		if(ReadRelayWord(Rel25.w25LK)) //������
		{
			if(!Rel25.bHaveMakeLKRptFlag)
			{
				MakeAlarmRpt(Rel25.R25LKRptNum);
				Rel25.bHaveMakeLKRptFlag = TRUE;				
			}
			ResetRel25();
			return;
		}
		else
			Rel25.bHaveMakeLKRptFlag = FALSE;
	}
	else
	{			
		//Rel25.bHaveMakeLKRptFlag = FALSE;
        ResetRel25();
		return;
	}
	if(!Rel25.bManSynFlag) //��ͬ������
	{
		Rel25.bManSynFlag = TRUE;
		Rel25.bJudgeNetend = FALSE;
		Rel25.bSameNetFlag = FALSE;
		Rel25.bDiffNetFlag = FALSE;
		Rel25.bSigleNetFlag = FALSE;
		Rel25.bJudgeSameNetPIFlag = FALSE;
		Rel25.bJudgeDiffNetPIFlag = FALSE;
		Rel25.bJudgeSigleNetPIFlag = FALSE;
		*Rel25.pPA = 0;
        Rel25.dwSynCheckTime = *Rel25.pSysTimer;
		ReportMsg("Rel25 pick! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
	}
	
	if(ReadRelayWord(Rel25.w25INT)) //���ж�
	{
		if(!Rel25.bHaveMakeINTRptFlag)
		{
			MakeActRptWithoutStart(Rel25.R25INTRptNum);
			Rel25.bHaveMakeINTRptFlag = TRUE;
			ResetRel25();
			return;
		}
	}
	else
		Rel25.bHaveMakeINTRptFlag = FALSE;

	if(*Rel25.pSysTimer - Rel25.dwSynCheckTime > *Rel25.p25DTSet) //��ʱ�ѵ���ͬ��ʧ��
	{		
		MakeActRptWithoutStart(Rel25.R25FalRptNum);
		
		ReportMsg("Rel25 fail! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
		ResetRel25();
		return;
	}
	if(ReadRelayWord(Rel25.w79_25PI)) //������غ�բ�����ļ�ͬ�ڣ�����ѹ����ͬ�ڵ�ѹ��Χ֮�ڣ�ֱ�ӷ���
	{
		if(Rel25.bDVOutLimitFlag)
			return;
	}
	if(!Rel25.bJudgeNetend) //���ж���δ���
	{		
		JudgeGrid();  //���ж�
		if(!Rel25.bJudgeNetend)
			return;
	}
	
	if(Rel25.bSameNetFlag) //ͬ��ͬ���ж�
	{
		SameGridCheck();
		return;
	}
	if(Rel25.bDiffNetFlag) //����ͬ���ж�
	{
		DiffGridCheck();
		return;
	}
	if(Rel25.bSigleNetFlag) //����ͬ���ж�
		SingleGridCheck();
}
void CRelayTask::ResetRel25(void)
{	
	Rel25.bManSynFlag = FALSE;
	Rel25.bJudgeNetend = FALSE;
	Rel25.bSigleNetFlag = FALSE;
	Rel25.bSameNetFlag = FALSE;
	Rel25.bDiffNetFlag = FALSE;
	Rel25.bJudgeSameNetPIFlag = FALSE;
	Rel25.bJudgeDiffNetPIFlag = FALSE;
	Rel25.bJudgeSigleNetPIFlag = FALSE;

	Rel25.bHaveMakeLKRptFlag = FALSE;
	Rel25.bHaveMakeINTRptFlag = FALSE;
	ResetHoldRelayWord(Rel25.wMCL_25PI);
	ResetHoldRelayWord(Rel25.w79_25PI);
	WriteRelayWord(Rel25.w25ActRW, RELAY_OFF); 

}
void CRelayTask::JudgeGrid(void)	//�ж�ͬ���������ǵ���
{	
	if(ReadRelayWord(Rel25.w25LK))
	{
		ResetJudgeGrid();
		return;
	}
	//ĸ������·����ѹ��ֱ����Ϊͬ�ڳɹ�
	if((Rel25.pUsyn->Mod<*Rel25.p25_27Set)&&((U1.Mod<*Rel25.p25_27Set)&&(U2.Mod<*Rel25.p25_27Set)&&(U3.Mod<*Rel25.p25_27Set)))
	{
		WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
		Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
		MakeActRptWithoutStart(Rel25.R25SucRptNum);
		return;
	}
	if(Rel25.pUsyn->Mod<*Rel25.p25_27Set) //ĸ����ѹ�������м�ͬ�ڣ���ֹ��������·������ĸ��
	{
		ResetJudgeGrid();
		return;
	}
	if((U1.Mod<*Rel25.p25_27Set)&&(U2.Mod<*Rel25.p25_27Set)&&(U3.Mod<*Rel25.p25_27Set)) //��·����ѹ����Ϊ����
	{
		Rel25.bJudgeSameNetPIFlag = FALSE;
		Rel25.bJudgeDiffNetPIFlag = FALSE;
		if(!Rel25.bJudgeSigleNetPIFlag)
		{
			Rel25.bJudgeSigleNetPIFlag = TRUE;
			Rel25.dwInitSingleNetChkTimer = *Rel25.pSysTimer;
		}
		else
		{
			if(*Rel25.pSysTimer-Rel25.dwInitSingleNetChkTimer>1000)
			{				
				ReportMsg("Rel25: single grid!");
				Rel25.bSigleNetFlag = TRUE;
				Rel25.bSameNetFlag = FALSE;
				Rel25.bDiffNetFlag = FALSE;
				Rel25.bJudgeNetend = TRUE;
				Rel25.bJudgeSigleNetPIFlag = FALSE;
			}
		}
		return;
	}
	if((U1.Mod>*Rel25.p25_27Set)&&(U2.Mod>*Rel25.p25_27Set)&&(U3.Mod>*Rel25.p25_27Set)) //��·���������ѹ
	{
		if(*Rel25.pDF == 50000)
			ResetJudgeGrid();
		if(*Rel25.pDF>=50) //0.05HZ,Ϊ������Ƶ�ʷŴ���1000��
		{			
			Rel25.bJudgeSameNetPIFlag = FALSE;
			Rel25.bJudgeSigleNetPIFlag = FALSE;
			if(!Rel25.bJudgeDiffNetPIFlag)
			{
				Rel25.bJudgeDiffNetPIFlag = TRUE;
				Rel25.dwInitDiffleNetChkTimer = *Rel25.pSysTimer;
			}
			else
			{
				if(*Rel25.pSysTimer-Rel25.dwInitDiffleNetChkTimer > 100)
				{					
					ReportMsg("Rel25: diff grid!");
					Rel25.bSigleNetFlag = FALSE;
					Rel25.bSameNetFlag = FALSE;
					Rel25.bDiffNetFlag = TRUE;
					Rel25.bJudgeNetend = TRUE;
					Rel25.bJudgeDiffNetPIFlag = FALSE;
				}
			}
			return;
		}
		else //Ƶ��С��0.05Hz��Ϊͬ��
		{
			Rel25.bJudgeDiffNetPIFlag = FALSE;
			Rel25.bJudgeSigleNetPIFlag = FALSE;
			if(!Rel25.bJudgeSameNetPIFlag)
			{
				Rel25.bJudgeSameNetPIFlag = TRUE;
				Rel25.dwInitSameleNetChkTimer= *Rel25.pSysTimer;
			}
			else
			{
				if(*Rel25.pSysTimer-Rel25.dwInitSameleNetChkTimer > 1000)
				{
					ReportMsg("Rel25: same grid!");
					Rel25.bSigleNetFlag = FALSE;
					Rel25.bSameNetFlag = TRUE;
					Rel25.bDiffNetFlag = FALSE;
					Rel25.bJudgeNetend = TRUE;
					Rel25.bJudgeDiffNetPIFlag = FALSE;
				}
			}
			return;
		}		
	}
}
void CRelayTask::ResetJudgeGrid(void)//��λ�����ж�
{
	Rel25.bSigleNetFlag = FALSE;
	Rel25.bSameNetFlag = FALSE;
	Rel25.bDiffNetFlag = FALSE;
	Rel25.bJudgeNetend = FALSE;
	Rel25.bJudgeDiffNetPIFlag = FALSE;	
	Rel25.bJudgeSameNetPIFlag = FALSE;
	Rel25.bJudgeSigleNetPIFlag = FALSE;
}
//����ͬ�ڣ�
void CRelayTask::SingleGridCheck(void)
{
	WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
	Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
	MakeActRptWithoutStart(Rel25.R25_27RptNum); //����ѹ�պϱ���
}
//ͬ��ͬ���ж�
void CRelayTask::SameGridCheck(void)
{
	if (*Rel25.pDF>100) //ͬ��ͬ���ж��У�Ƶ����ֱ䶯�����½��е����ж�
	{
		ResetJudgeGrid();
		return;		
	}	
	if(Rel25.bDVOutLimitFlag == TRUE) //��ѹ���ں���Χ֮��
		return;
	if(*Rel25.pDV>*Rel25.p25VDSet)//ѹ����ڶ�ֵ
		return;
//	if(*Rel25.pDA>18000)
//		*Rel25.pDA = 36000-*Rel25.pDA;
//	if(*Rel25.pDA>*Rel25.p25PASet) //ͬ�ڹ��Ǵ��ڶ�ֵ
	if(m_DQ>*Rel25.p25PASet) //ͬ�ڹ��Ǵ��ڶ�ֵ
		return;
	if ((*Rel25.pDF)>50) //ͬ��ͬ�ڵ�Ƶ��Ҫ��
		return;
	
	WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
	Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
	MakeActRptWithoutStart(Rel25.R25SucRptNum);
	
	ReportMsg("Rel25 same grid succ! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
}
//����ͬ���ж�
void CRelayTask::DiffGridCheck(void)
{
	LONG preA;
	if (*Rel25.pDF<=20) //����ͬ���ж��У�Ƶ����ֱ䶯�����½��е����ж�
	{
		ResetJudgeGrid();
		return;		
	}
	if(Rel25.bDVOutLimitFlag == TRUE) //��ѹ���ں���Χ֮��
		return;
	if(*Rel25.pDF > *Rel25.p25FDSet) //Ƶ���������
		return;
	
	CaluPreAngle();
	if(*Rel25.pPA == 0)
		return;
	preA = *Rel25.pDA - *Rel25.pPA;
	preA = ToAbs(preA);
	if(preA>500)
		return;
	
	WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
	Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
	MakeActRptWithoutStart(Rel25.R25SucRptNum);
	
	ReportMsg("Rel25 diff grid succ! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
}
void CRelayTask::CaluPreAngle(void)
{
	if(*Rel25.pDF == 50000)
	{
		*Rel25.pPA = 0;
		return;
	}
	*Rel25.pPA = 36*(*Rel25.pDF)*(*Rel25.p52CLT)/1000;
}

void CRelayTask::JDXXCheck(void)
{	
	static DWORD RJDXXTimer = 0;
	if((*CFG_pbyJDXX == CFG_STATUS_SET)&&(*SW_pbyJDXX == CFG_STATUS_SET))
		WriteJDXXData();
	if(JDXXOnFlag == FALSE)
	{
		JDXXPickUpCHK();
		if(JDXXCheckFlag == TRUE)
		{
			OS_PostMessage(g_hDbaseTaskID, SM_JDXXPICK, 0,0,0,0 );	//RtosGetCurrentTaskId(), ������Ϣ�����ӵ�ѡ��
			RJDXXTimer = dwRelayMSCnt;
			JDXXOnFlag = TRUE;
		}
		else
			return;
	}
	if(!HaveJDXXResultFlag)
	{
		if((dwRelayMSCnt - RJDXXTimer) > 30000)
		{
			JDXXPowerDirCheck();
			if(JDXXForwardFlag)
				MakeAlarmRpt(R_64LINE);
			else
				MakeAlarmRpt(R_59NAR);
			JDXXOnFlag = FALSE;
			JDXXCheckFlag = FALSE;
			JDXXPickFlag = FALSE;
			HaveJDXXResultFlag = TRUE;
		}
	}
}
void CRelayTask::JDXXPickUpCHK(void)
{	
	static DWORD RJDXXPickTimer = 0;
	if((*CFG_pbyJDXX == CFG_STATUS_CLR)||(*SW_pbyJDXX== CFG_STATUS_CLR))
	{		
		HaveJDXXResultFlag = FALSE;
		JDXXPickFlag = FALSE;
		JDXXCheckFlag = FALSE;
		return;
	}
	if( U0.Mod > *RS_pn59NU)
	{
		if(HaveJDXXResultFlag == FALSE)
		{
			if(JDXXPickFlag == FALSE)
			{
				RJDXXPickTimer = dwRelayMSCnt;
				JDXXPickFlag = TRUE;
			}
			if((dwRelayMSCnt-RJDXXPickTimer) > 1000)
			{
				JDXXCheckFlag = TRUE;
			}
		}
	}
	else
	{		
		HaveJDXXResultFlag = FALSE;
		JDXXPickFlag = FALSE;
		JDXXCheckFlag = FALSE;
	}
}
void CRelayTask::JDXXPowerDirCheck()
{
	LONG temp1,temp2;
	LONG Angle;
	if(*CFG_pbyXHJD == CFG_STATUS_CLR)
	{	
		temp2=U0.Mod*I0.Mod/32;
	}
	else
	{
		temp2=U5.Mod*I5.Mod/32;
	}
	if(temp2 < 100)
	{
		JDXXForwardFlag = FALSE;
		return;
	}
	if(*CFG_pbyXHJD == CFG_STATUS_CLR)
	{	
		temp1=((U0.Imag*1000/U0.Mod)*(I0.Real*1000/I0.Mod)-(U0.Real*1000/U0.Mod)*(I0.Imag*1000/I0.Mod));  //(cosU*cosI+sinU*sinI)*cos90+(sinU*cosI-cosU*sinI)sin90
	}
	else
	{
		temp1=((U5.Imag*1000/U5.Mod)*(I5.Real*1000/I5.Mod)-(U5.Real*1000/U5.Mod)*(I5.Imag*1000/I5.Mod));
	}
	if(temp1 > 0)   
	{
		JDXXForwardFlag = TRUE;
	}	
	else 
	{
		JDXXForwardFlag = FALSE;
	}
}
void CRelayTask::OnJDXXResult(DWORD arg,DWORD result)
{
	if(JDXXOnFlag)
	{
		if(result == 1)
			MakeAlarmRpt(R_64LINE);
		else if(result == 2)
			MakeAlarmRpt(R_64BUS);
		else		
			MakeAlarmRpt(R_59NAR);
		JDXXOnFlag = FALSE;
		JDXXCheckFlag = FALSE;
		JDXXPickFlag = FALSE;		
		HaveJDXXResultFlag = TRUE;
	}
}
void CRelayTask::WriteJDXXData()
{
	if((*CF_pnU0 == 0)||(*CF_pnI0 == 0))
	{
		ReportMsg("The CF CF_pnU0 or CF_pnI0 is zero! CF_pnU0=%d,CF_pnI0=%d",*CF_pnU0,*CF_pnI0);
		return;
	}
	JDXXElecData.U0 	= 10000*(float)(U0.Mod)*(*AS_pnPTRatio)/(*CF_pnU0);
	JDXXElecData.U0_5	= 10000*(float)(U5.Mod)*(*AS_pnPTRatio)/(*CF_pnU0);
	JDXXElecData.I0 	= 1000*(float)(I0.Mod)*(*AS_pnZCTRatio)/(*CF_pnI0);
	JDXXElecData.I0_5	= 1000*(float)(I5.Mod)*(*AS_pnZCTRatio)/(*CF_pnI0);
	JDXXElecData.I0_A	= *RM_pnAI01;
	JDXXElecData.I0_5_A	= *RM_pnAI05;
	
}

