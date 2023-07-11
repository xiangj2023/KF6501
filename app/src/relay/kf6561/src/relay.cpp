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
* ����      KF6561�������������װ��                                                                       
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
*      yanzh             12/07/08    ��ʼ����                         
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

BOOL g_FaultOnFlag;			////������ϴ����־ CT���߼����
DWORD		dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD		dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
DWORD 		dwRelayTaskID;
BOOL 		RelayTaskInitOK = FALSE;
TStructCheck StructCheck[CHECK_NUM];
extern DWORD dwADResumeCnt;

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
	g_FaultOnFlag = FALSE;			//������ϴ����־ CT���߼����
	HaveMakeStartRptFlag = FALSE;	//���������ϵ������������־
	m_bFaultLKFlag = FALSE;			//������ʼʱ��������־
	FaultCheckOutFlag = FALSE;		//������ϱ�־
	m_bLastRecStatusFlag = FALSE;	//����¼�������������жϱ�־
	m_b52BFFLAG = FALSE;
	m_bLogicChanged = FALSE;
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
	dwChanDataMask = ( (0x01<<CHANNEL_6561_UA)|(0x01<<CHANNEL_6561_UB)|(0x01<<CHANNEL_6561_UC)|\
						(0x01<<CHANNEL_6561_U0)|(0x01<<CHANNEL_6561_U1)|(0x01<<CHANNEL_6561_U2)|\
						(0x01<<CHANNEL_6561_IA)|(0x01<<CHANNEL_6561_IB)|(0x01<<CHANNEL_6561_IC)|\
						(0x01<<CHANNEL_6561_I0)|(0x01<<CHANNEL_6561_I1)|(0x01<<CHANNEL_6561_U3));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	
	
	//��ʼ����ȡ����������ʱ������
	//�������ݻ�����
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);	
	//�ϳ�ͨ�����ݻ�����
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	//ͻ����������ʷ���ݻ�����
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*5);
		
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

	m_U2Set = *CF_pnUp*8; //8V�����ѹ
	
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
	InitR60I();
	InitR60U();
	InitR59U();
	InitR27U();		
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
			RelayCheckErr = TRUE;
			return;
		}
//	if(RelayCheckErr)
//	{
//	//	ReportMsg("Relay Check Serious Error,and Relay Task Locked!");
//		LedAllOn();
//		return ;
//	}
//	MCF_GPIO_PODR_TIMER |= 0x02;		//�ߵ�ƽ

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
	R59NAlarm();
	Process52BF();
	LogicProcess();								//�߼����̴���
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
//	MCF_GPIO_PODR_TIMER &= ~0x02;		//�͵�ƽ
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
	Ipmax = ElecMax(Ia,Ib,Ic); //��������е����ֵ�����������
	Ulmin = ElecMin(Uab,Ubc,Uca);		//�ߵ�ѹ����Сֵ��
	Ulmax = ElecMax(Uab,Ubc,Uca);		//�ߵ�ѹ�����ֵ�����ڵ�ѹ����
}
void CRelayTask::CaluU2()
{
	NU2.Real = (Uab.Real*1000 + Ubc.Real*Factor2.Real -Ubc.Imag*Factor2.Imag + Uca.Real*Factor1.Real -Uca.Imag*Factor1.Imag)/5196;
	NU2.Imag = (Uab.Imag*1000 + Ubc.Imag*Factor2.Real +Ubc.Real*Factor2.Imag + Uca.Imag*Factor1.Real +Uca.Real*Factor1.Imag)/5196;
	CaluModValue(&NU2);
	*RM_pnU2= NU2.Mod;                                       
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
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
	angle = U0.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAU01 = angle;
	else
		*RM_pnAU01 = angle+36000;
	DB_LogWriteRM(RM_AU01, *RM_pnAU01);

	CaluAngle(&I0,AngleTab);	
	angle = I0.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAI01 = angle;
	else
		*RM_pnAI01 = angle+36000;
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
	LONG angle;
	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*4,&U5);
	CaluModValue(&U5);
	*RM_pnU05= U5.Mod;										
	DB_LogWriteRM(RM_U05, *RM_pnU05); 	
	CaluAngle(&U5,AngleTab);	
	angle = U5.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAU05 = angle;
	else
		*RM_pnAU05 = angle+36000;
	DB_LogWriteRM(RM_AU05, *RM_pnAU05);

	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*11,&I5);
	CaluModValue(&I5);
	*RM_pnI05= I5.Mod;										
	DB_LogWriteRM(RM_I05, *RM_pnI05); 	
	CaluAngle(&I5,AngleTab);	
	angle = I5.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAI05 = angle;
	else
		*RM_pnAI05 = angle+36000;
	DB_LogWriteRM(RM_AI05, *RM_pnAI05);
}

void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&R50I0;
	StructCheck[0].bigsize = sizeof(R50I0)/2 - R50I0.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&R50I1;
	StructCheck[1].bigsize = sizeof(R50I1)/2 - R50I1.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&R60I;
	StructCheck[2].bigsize = sizeof(R60I)/2 - R60I.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&R60U;
	StructCheck[3].bigsize = sizeof(R60U)/2 - R60U.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&R59U;
	StructCheck[4].bigsize = sizeof(R59U)/2 - R59U.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&R27U;
	StructCheck[5].bigsize = sizeof(R27U)/2 - R27U.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[6].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
// ����: ��ʼ������I�α���Ԫ��	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I0()
{
	R50I0.pElecSet				= RS_pn50I0C;          						//��������������ֵ 
	R50I0.pTimeSet				= RS_pn50I0D;				 				//��������ʱ�䶨ֵ 
	R50I0.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50I0.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50I0.pAccActTimeSet		= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R50I0.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50I0.pRelCFG				= CFG_pby50I0;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50I0.pRelSW				= SW_pby50I0;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50I0.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I0.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I0.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I0.pRelFDCFG				= &RelCFGCLR;					//
	R50I0.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50I0.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R50I0.pRel47UCFG			= &RelCFGCLR;					//��ѹ��������
	R50I0.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	
	R50I0.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50I0.pRelElec 				= &Ipmax;						//��ǰ����ָ�� 		
	R50I0.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50I0.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50I0.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50I0.StartRelayWord		= RW_PI50I0;							//����������μ̵����� 
	R50I0.ActRelayWord			= RW_TR50I0;							//�������������̵����� 
	R50I0.AccRelayWord			= FALSE1;							//���������̵�����,��һ��ʱ��̵����֣���ʱ�����Զ����� 
	R50I0.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R50I0.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R50I0.PRewardRelayWord  	= FALSE1;
	R50I0.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	R50I0.R47ULKRelayWord		= FALSE1;
	R50I0.LockRelayWord			= RW_LKPI50I0;							//��α����̵����� 
	R50I0.RSTRelayWord			= RW_LKRST50I0;							//���ر����̵����� 
	R50I0.LoopCtrlRelayWord		= RW_LPC50I0;						//����ѭ�����Ƽ̵����� 
	
	R50I0.wReportNum			= R_R50I0;								//��������������� 
	R50I0.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R50I0.dwK35					= NULL;
	R50I0.RptSerialNum			= &RptSerialNumOfAct;
	R50I0.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50I0.wPickUpReportNum		= R_R50I0_PI;						//��α������к�
	R50I0.wResetReportNum		= R_R50I0_RES;						//���ر������к�
	//**********************************************************************	


//	R50I0.dwRelTimer			= 0;			 				//��ʱ�� 
	R50I0.byRelFCounter			= 0;						//���ϼ����� 
	R50I0.byRelRCounter			= 0;						//���ؼ����� 	
	R50I0.wExcepWordNum			= 8;
	R50I0.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������II�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitR50I1()
{	
	R50I1.pElecSet				= RS_pn50I1C;          						//��������������ֵ 
	R50I1.pTimeSet				= RS_pn50I1D;				 				//��������ʱ�䶨ֵ 
	R50I1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R50I1.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R50I1.pAccActTimeSet		= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R50I1.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R50I1.pRelCFG				= CFG_pby50I1;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R50I1.pRelSW				= SW_pby50I1;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R50I1.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I1.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I1.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R50I1.pRelFDCFG				= &RelCFGCLR;
	R50I1.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R50I1.pRelAccCFG			= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R50I1.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R50I1.pRel47UCFG			= &RelCFGCLR;
	R50I1.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R50I1.pRelElec 				= &Ipmax;						//��ǰ����ָ�� 		
	R50I1.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R50I1.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R50I1.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R50I1.StartRelayWord		= RW_PI50I1;							//����������μ̵����� 
	R50I1.ActRelayWord			= RW_TR50I1;							//�������������̵����� 
	R50I1.AccRelayWord			= FALSE1;							//���������̵����� 
	R50I1.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R50I1.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R50I1.PRewardRelayWord  	= FALSE1;					//���ʷ�����̵����� 
	R50I1.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R50I1.R47ULKRelayWord		= FALSE1;
	R50I1.LockRelayWord			= RW_LKPI50I1;							//��α����̵����� 
	R50I1.RSTRelayWord			= RW_LKRST50I1;							//���ر����̵����� 
	R50I1.LoopCtrlRelayWord		= RW_LPC50I1;						//����ѭ�����Ƽ̵����� 
	
	R50I1.wReportNum			= R_R50I1;								//��������������� 
	R50I1.wAccReportNum			= 0xFFFF;;							//���ٶ���������� 
	
	R50I1.dwK35					= NULL;
	R50I1.RptSerialNum			= &RptSerialNumOfAct;
	R50I1.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R50I1.wPickUpReportNum		= R_R50I1_PI;						//��α������к�
	R50I1.wResetReportNum		= R_R50I1_RES;						//���ر������к�
	//**********************************************************************	

//	R50I1.dwRelTimer			= 0;			 				//��ʱ�� 
	R50I1.byRelFCounter			= 0;						//���ϼ����� 
	R50I1.byRelRCounter			= 0;						//���ؼ����� 
	R50I1.wExcepWordNum			= 8;
	R50I1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������III�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR60I()
{	
	R60I.pElecSet				= RS_pn60IC;          						//��������������ֵ 
	R60I.pTimeSet				= RS_pn60ID;				 				//��������ʱ�䶨ֵ 
	R60I.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R60I.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R60I.pAccActTimeSet			= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R60I.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R60I.pRelCFG				= CFG_pby60I;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R60I.pRelSW					= SW_pby60I;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R60I.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R60I.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R60I.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R60I.pRelFDCFG				= &RelCFGCLR;
	R60I.pRelVolLockCFG			= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R60I.pRelAccCFG				= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R60I.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R60I.pRel47UCFG				= &RelCFGCLR;
	R60I.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R60I.pRelElec 				= &I1;						//��ǰ����ָ�� 		
	R60I.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R60I.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R60I.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R60I.StartRelayWord			= RW_PI60I;							//����������μ̵����� 
	R60I.ActRelayWord			= RW_TR60I;							//�������������̵����� 
	R60I.AccRelayWord			= FALSE1;							//���������̵����� 
	R60I.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R60I.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R60I.PRewardRelayWord  		= FALSE1;					//���ʷ�����̵����� 
	R60I.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R60I.R47ULKRelayWord		= FALSE1;
	R60I.LockRelayWord			= RW_LKPI60I;							//��α����̵����� 
	R60I.RSTRelayWord			= RW_LKRST60I;							//���ر����̵����� 
	R60I.LoopCtrlRelayWord		= RW_LPC60I;						//����ѭ�����Ƽ̵����� 
	
	R60I.wReportNum				= R_R60I;								//��������������� 
	R60I.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R60I.dwK35					= NULL;
	R60I.RptSerialNum			= &RptSerialNumOfAct;
	R60I.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R60I.wPickUpReportNum		= R_R60I_PI;						//��α������к�
	R60I.wResetReportNum		= R_R60I_RES;						//���ر������к�
	//**********************************************************************	

//	R60I.dwRelTimer				= 0;			 				//��ʱ�� 
	R60I.byRelFCounter			= 0;						//���ϼ����� 
	R60I.byRelRCounter			= 0;						//���ؼ����� 
	R60I.wExcepWordNum			= 8;
	R60I.wInfo 					= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ�������������Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR60U()
{	
	R60U.pElecSet				= RS_pn60UV;          						//��������������ֵ 
	R60U.pTimeSet				= RS_pn60UD;				 				//��������ʱ�䶨ֵ 
	R60U.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R60U.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R60U.pAccActTimeSet			= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R60U.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R60U.pRelCFG				= CFG_pby60U;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R60U.pRelSW					= SW_pby60U;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R60U.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R60U.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R60U.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R60U.pRelFDCFG				= &RelCFGCLR;
	R60U.pRelVolLockCFG			= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R60U.pRelAccCFG				= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R60U.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R60U.pRel47UCFG				= &RelCFGCLR;
	R60U.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R60U.pRelElec 				= &U1;						//��ǰ����ָ�� 		
	R60U.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R60U.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R60U.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R60U.StartRelayWord			= RW_PI60U;							//����������μ̵����� 
	R60U.ActRelayWord			= RW_TR60U;							//�������������̵����� 
	R60U.AccRelayWord			= FALSE1;							//���������̵����� 
	R60U.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R60U.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R60U.PRewardRelayWord  		= FALSE1;					//���ʷ�����̵����� 
	R60U.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R60U.R47ULKRelayWord		= FALSE1;
	R60U.LockRelayWord			= RW_LKPI60U;							//��α����̵����� 
	R60U.RSTRelayWord			= RW_LKRST60U;							//���ر����̵����� 
	R60U.LoopCtrlRelayWord		= RW_LPC60U;						//����ѭ�����Ƽ̵����� 
	
	R60U.wReportNum				= R_R60U;								//��������������� 
	R60U.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R60U.dwK35					= NULL;
	R60U.RptSerialNum			= &RptSerialNumOfAct;
	R60U.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R60U.wPickUpReportNum		= R_R60U_PI;						//��α������к�
	R60U.wResetReportNum		= R_R60U_RES;						//���ر������к�
	//**********************************************************************	

//	R60U.dwRelTimer				= 0;			 				//��ʱ�� 
	R60U.byRelFCounter			= 0;						//���ϼ����� 
	R60U.byRelRCounter			= 0;						//���ؼ����� 
	R60U.wExcepWordNum			= 8;
	R60U.wInfo 					= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ�������������Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR59U()
{	
	
	R59U.pElecSet				= RS_pn59U;          						//��������������ֵ 
	R59U.pTimeSet				= RS_pn59D;				 				//��������ʱ�䶨ֵ 
	R59U.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ�� 
	R59U.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	R59U.pAccActTimeSet			= NULL;						//������������ٶ���ʱ�䶨ֵ 
	R59U.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���

	R59U.pRelCFG				= CFG_pby59U;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	R59U.pRelSW					= SW_pby59U;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	R59U.pRelHLockCFG			= &RelCFGCLR;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	R59U.pRelHSuprCFG			= &RelCFGCLR; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	R59U.pRelPDCFG				= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	R59U.pRelFDCFG				= &RelCFGCLR;
	R59U.pRelVolLockCFG			= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	R59U.pRelAccCFG				= &RelCFGCLR;							//���޺���ٹ���------1:�У�  2:�� 
	R59U.pRelFLCFG				= &RelCFGCLR;				//���ϲ��Ͷ�� 
	R59U.pRel47UCFG				= &RelCFGCLR;
	R59U.pSysTimer				= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	R59U.pRelElec 				= &Ulmax;						//��ǰ����ָ�� 		
	R59U.pRelHarm3				= NULL;						//��ǰ����г��ָ�� 	
	R59U.pRelHarm5				= NULL;						//��ǰ���г��ָ�� 	
	R59U.pRelVolt				= NULL;						//��ǰ���������ĵ�ѹ��ָ�� 

	R59U.StartRelayWord			= RW_PI59U;							//����������μ̵����� 
	R59U.ActRelayWord			= RW_TR59U;							//�������������̵����� 
	R59U.AccRelayWord			= FALSE1;							//���������̵����� 
	R59U.AccActRelayWord		= FALSE1;						//����ٶ����̵����� 
	R59U.PForwardRelayWord		= FALSE1;						//����������̵����� 
	R59U.PRewardRelayWord  		= FALSE1;					//���ʷ�����̵����� 
	R59U.H2LockRelayWord		= FALSE1;						//����г�������̵����� 
	
	R59U.R47ULKRelayWord		= FALSE1;
	R59U.LockRelayWord			= RW_LKPI59U;							//��α����̵����� 
	R59U.RSTRelayWord			= RW_LKRST59U;							//���ر����̵����� 
	R59U.LoopCtrlRelayWord		= RW_LPC59U;						//����ѭ�����Ƽ̵����� 
	
	R59U.wReportNum				= R_R59U;								//��������������� 
	R59U.wAccReportNum			= 0xFFFF;							//���ٶ���������� 
	
	R59U.dwK35					= NULL;
	R59U.RptSerialNum			= &RptSerialNumOfAct;
	R59U.bMakeActRpt			= TRUE;
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	R59U.wPickUpReportNum		= R_R59U_PI;						//��α������к�
	R59U.wResetReportNum		= R_R59U_RES;						//���ر������к�
	//**********************************************************************	

//	R59U.dwRelTimer				= 0;			 				//��ʱ�� 
	R59U.byRelFCounter			= 0;						//���ϼ����� 
	R59U.byRelRCounter			= 0;						//���ؼ����� 
	R59U.wExcepWordNum			= 8;
	R59U.wInfo 					= 0;
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
	R27U.pLockCurSet					= RS_pn27ILO;					//����������ֵ 

	R27U.pRelCFG						= CFG_pby27U;					  	 //Ƿ���������� 
	R27U.pRelSW							= SW_pby27U;					  	 //Ƿ��������ѹ�� 
	R27U.pRelAlarmCFG					= &RelCFGCLR;						//Ƿ������Ͷ��բ 
	R27U.pRelCurLockCFG					= &RelCFGSET;
	R27U.pSysTimer						= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ�� 

	R27U.pRelElec						= &Ulmax;						//��ǰ�ĵ���ָ�� 
	R27U.pRelLockElec					= &Ipmax;					//�������� 
	R27U.ActRelayWord					= RW_TR27U;					//Ƿ�����������̵����� 
	R27U.StartRelayWord					= RW_PI27U;					//Ƿ��������μ̵����� 
	R27U.AlarmRelayWord					= FALSE1;					//Ƿ�������澯�̵����� 
	R27U.LockRelayWord					= RW_LKPI27;				//��α����̵����� 
	R27U.RSTRelayWord					= RW_LKRST27;					//���Ϸ��ر����̵����� 
	R27U.LoopCtrlRelayWord				= RW_LPC27U;				//����ѭ�����Ƽ̵����� 
	R27U.wR52BFRealyword				= RW_52BF;					//��·����բʧ�̵ܼ�����
	R27U.wActRptNum						= R_R27U;					//Ƿ����������������� 
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
	BYTE  pChannel[]={CHANNEL_6561_IA,CHANNEL_6561_IB,CHANNEL_6561_IC,CHANNEL_6561_I0,CHANNEL_6561_I1};
	if(ReadRelayWord(RW_52BF))  //��·����բʧ�ܣ������б��������жϴ��� 
	{
		WriteRelayWord(RW_PICKUP, RELAY_OFF);
		return FALSE;
	}
	if((*R50I0.pRelCFG == CFG_STATUS_SET) && (*R50I0.pRelSW == SW_STATUS_SET))   //����1�α���Ͷ�� 
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
	if((*R50I1.pRelCFG == CFG_STATUS_SET) && (*R50I1.pRelSW == SW_STATUS_SET))   //����2�α���Ͷ�� 
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
	if((*R60I.pRelCFG == CFG_STATUS_SET) && (*R60I.pRelSW == SW_STATUS_SET))   //����3�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R60I.pRelElec->Mod > *R60I.pElecSet)
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
	if((*R60U.pRelCFG == CFG_STATUS_SET) && (*R60U.pRelSW == SW_STATUS_SET))   //����1�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R60U.pRelElec->Mod > *R60U.pElecSet)
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
	if((*R59U.pRelCFG == CFG_STATUS_SET) && (*R59U.pRelSW == SW_STATUS_SET))   //����1�α���Ͷ�� 
	{
		haveRelayInput = TRUE;
		if(R59U.pRelElec->Mod > *R59U.pElecSet)
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
//			return;
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

		
		GeneralOverValueRelay(&R50I0);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&R50I1);				//���ù���I�α����ӳ���
		GeneralOverValueRelay(&R60I);				//���ù���III�α����ӳ���
		GeneralOverValueRelay(&R60U);				//���ù���III�α����ӳ���
		GeneralOverValueRelay(&R59U);
		
		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveMakeTripFailRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//����·����բʧ�ܱ���
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}

		// 1���б���ѭ�����Ƽ̵����ֱ���λ���ȴ��������������˳�
		if(ReadRelayWord(R50I0.LoopCtrlRelayWord)||ReadRelayWord(R50I1.LoopCtrlRelayWord)||ReadRelayWord(R60I.LoopCtrlRelayWord)
			||ReadRelayWord(R60U.LoopCtrlRelayWord)||ReadRelayWord(R59U.LoopCtrlRelayWord))
		{			
			// 1.1 �����Ѿ�����������բʧ��
			if(ReadRelayWord(RW_52BF))
			{
				WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
				FaultCheckOutFlag = FALSE;					//�������ϱ�־
				FaultOnFlag = FALSE;						//�����ѭ�������־
				HaveMakeStartRptFlag= FALSE;				//�����������ϵ������������־
				m_bFaultLKFlag = FALSE; 					//���������޶���ʱ����ʱ��������־	
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
				if((dwRelayMSCnt -FaultLockTime)>500)  //�޹���ʱ�䳬�����ڹ��ϼ��ʱ��
				{						
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
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
				if(dwRelayMSCnt -FaultLockTime>1000 )	
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־						
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־
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
	ResetOverValueRelay(&R60U,resetFlag);
	ResetOverValueRelay(&R60I,resetFlag);
	ResetOverValueRelay(&R59U,resetFlag);
}
void CRelayTask::PTCheck()
{	
	if(*CFG_pbyPTBR == CFG_STATUS_CLR) //PT����δͶ��
	{
		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_R74PT_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		m_BusPTTimer = 0;
		return;
	}
//ĸ��PT�����ж�
	if(((Ulmin.Mod<*RS_pn74PTU)&&(Ipmax.Mod>*RS_pn74PTI))||(NU2.Mod>*RS_pn74PTU2))
	{
		if(ReadRelayWord(RW_PIPT1))
		{
			if(!ReadRelayWord(RW_ARPT1))
			{
				if(dwRelayMSCnt - m_BusPTTimer>*RS_pn74PTD)
				{
					WriteRelayWord(RW_ARPT1,RELAY_ON);
					MakeAlarmRpt(R_R74PT);
				}
			}
		}
		else
		{
			WriteRelayWord(RW_PIPT1,RELAY_ON);
			m_BusPTTimer = dwRelayMSCnt;
		}
	}
	else //ֻ���ڵ�ѹ�ָ�����ʱ������
	{
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_R74PT_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
		m_BusPTTimer = 0;
	}
}

void CRelayTask::R59NAlarm() //��ѹ�澯
{
	static DWORD R59NTimer = 0;
	if(*CFG_pby59N == CFG_STATUS_CLR)
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
					MakeAlarmRpt(R_R59NU);
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












