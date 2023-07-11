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
#include "Relay.h"

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

extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;
extern DWORD dwRelayMSCnt;		//�����ú��������
extern TSysConfigTable	*G_pCfgTable;

extern BYTE g_RptSerialNum;	        //�������к� 
extern BYTE byRelayWordValue[];
extern BOOL RelayCheckInitOK;
extern BOOL RelayCheckErr;			//�����Լ����
extern DWORD dwADResumeCnt;
extern BYTE *g_MaintSW;

DWORD 		 dwRelayTaskID;
BOOL 		 RelayTaskInitOK = FALSE;
DWORD		 dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD		 dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
TStructCheck StructCheck[CHECK_NUM];					//����Ԫ��У������

TRunTimeRM	*pSysRM;		//��������ֵ����ָ��
TRunTimeBI  *pSysBI;		//ң��ʵʱ����ָ��

DWORD dwCaluInterval;	//�ȹ����ɼ����ۼӼ��
BOOL  g_bRel49Flag;		//�ȹ����ɽ����ۼӼ�����Ϣ��־
BOOL  g_bRel49TimeOut;  //�ȹ������ۼ���ʱ����־

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
	CRelayTask* pRelayTask = new CRelayTask(pdwApp);
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
				default:
					break;
			}
		}
	}
}	

CRelayTask::CRelayTask(DWORD *pdwApp):CApp(pdwApp),TimeSet(0)
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
	InitSet();
	InitDCRecInfo();   //��ʼ��¼��

    RelCFGCLR = CFG_STATUS_CLR;		//���������˳�
	RelCFGSET = CFG_STATUS_SET;		//��������Ͷ��
	
	RptSerialNumOfAct =0;           //�����������кţ�������������
	RptSerialNumOfStart=0;          //�����������кţ��������ϵ�������������
	FaultSINum = 0;                 //���������������
	
	b68IdH2Flag_A = FALSE;			//A��������α�����־	
	b68IdH2Flag_B = FALSE;			//B��������α�����־	
	b68IdH2Flag_C = FALSE;			//C��������α�����־
	b68IdH5Flag_A = FALSE;	//A�����5�α�����־	
	b68IdH5Flag_B = FALSE;	//B�����5�α�����־	
	b68IdH5Flag_C = FALSE;	//C�����5�α�����־			
	b68IrH2Flag_A = FALSE;	//A���ƶ�����2�α�����־	
	b68IrH2Flag_B = FALSE;	//B���ƶ�����2�α�����־	
	b68IrH2Flag_C = FALSE;	//C���ƶ�����2�α�����־
	b68IdHMulFlag_A = FALSE;
	b68IdHMulFlag_B = FALSE;
	b68IdHMulFlag_C = FALSE;


	HA_PDWithMemoryFlag = FALSE;
	HB_PDWithMemoryFlag = FALSE;
	HC_PDWithMemoryFlag = FALSE;
	LA_PDWithMemoryFlag = FALSE;
	LB_PDWithMemoryFlag = FALSE;
	LC_PDWithMemoryFlag = FALSE;

	HPTCheckTimer1 = 0;
	HPTCheckTimer2 = 0;
	HPTCheckTimer3 = 0;
	HPTCheckTimer4 = 0;	
	LPTCheckTimer1 = 0;
	LPTCheckTimer2 = 0;
	LPTCheckTimer3 = 0;
	LPTCheckTimer4 = 0;
	HPTMakeReportFlag = FALSE;
	LPTMakeReportFlag = FALSE;
	U2Set = 8*(*CF_pnUp)/100; //�����ѹ8V

    for(int i=0;i<3;i++)
	{
	  PickCheckNext[i] =0;
    }
	
	PickUpRW = RW_PICKUP;          //���������̵����֣�������ģֵ����ͻ��������
	TripFailFlag = FALSE;          //��բʧ�ܱ�־
	FaultOnFlag = FALSE;           //�������ѭ�������־
	HaveMakeStartRptFlag = FALSE;  //�����������־
	m_bFaultLKFlag = FALSE;        //ͻ��������������������ʼʱ�̱�־	
	FaultCheckOutFlag = FALSE;     //������ϱ�־
	m_b52BFFLAG = FALSE;
	m_bHaveMakeTripFailRpt = FALSE;
	
	m_b49IHaveMakeActRpt = FALSE;			
	m_b49IHaveMakeRSTRpt = FALSE;
	m_b49IHaveMakeWarnRpt = FALSE;
	
    dwChanDataMask = ((0x01<<CHANNEL_6533_UA)|(0x01<<CHANNEL_6533_UB)|(0x01<<CHANNEL_6533_UC)|\
						(0x01<<CHANNEL_6533_U0h)|(0x01<<CHANNEL_6533_Ua)|(0x01<<CHANNEL_6533_Ub)|\
						(0x01<<CHANNEL_6533_Uc)|(0x01<<CHANNEL_6533_U0l)|(0x01<<CHANNEL_6533_IA)|\
						(0x01<<CHANNEL_6533_IB)|(0x01<<CHANNEL_6533_IC)|(0x01<<CHANNEL_6533_Ia)|\
						(0x01<<CHANNEL_6533_Ib)|(0x01<<CHANNEL_6533_Ic)|(0x01<<CHANNEL_6533_I0)|\
						(0x01<<CHANNEL_6533_Ijx));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	dwComposeMask = ((0x01<<CHANNEL_6533_IDA)|(0x01<<CHANNEL_6533_IRA)|(0x01<<CHANNEL_6533_IDB)|\
					(0x01<<CHANNEL_6533_IRB)|(0x01<<CHANNEL_6533_IDC)|(0x01<<CHANNEL_6533_IRC));
	
	//��ʼ����ȡ����������ʱ������
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);	
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*11);
	dwComposeDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);
	dwLineUDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);		
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*6);
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
	memset(dwPickDataBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*11));
	
	ASSERT(dwComposeDataBuf != NULL);
	if(dwComposeDataBuf == NULL)
	{
		LogError("CRelayTask,dwComposeDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));
	
	ASSERT(dwLineUDataBuf != NULL);
	if(dwLineUDataBuf == NULL)
	{
		LogError("CRelayTask,dwLineUDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwLineUDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));
	
	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*6);
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

	RelayTaskInitOK = TRUE;
	m_bLogicChanged = FALSE;
	m_dwWaitLogicCounter = 0;
	g_MaintSW = SW_pbyMAINT;
	SetTimer(1,5);
	m_PTCheckTimer = 20*RELAY_SAMP_INTERVAL/POINT_PER_PERIOD;
}
/************************************************************************************************
*����: ��ʼ���뱣��ֱ����صĶ�ֵ
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitSet(void)
{
	
	dwCaluInterval = Rel49IA.l49OT*2;
	if((*CFG_pby49I == CFG_STATUS_SET)&&(*SW_pby49I == SW_STATUS_SET))
		g_bRel49Flag = TRUE;
	else
		g_bRel49Flag = FALSE;
}

void  CRelayTask::InitRealy(void)
{
	 InitRel87U_A();		//A��ٶ�
	 InitRel87U_B();		//B��ٶ�
	 InitRel87U_C();		//C��ٶ� 	
	 InitRel87R_A();		//A���ʲ
	 InitRel87R_B();		//B���ʲ
	 InitRel87R_C();		//C���ʲ
	 InitRel50I_H1();		//��ѹ�����1�Σ�
	 InitRel50I_H2();		//��ѹ�����2�Σ�
	 InitRel50I_H3();		//��ѹ�����3��
	 InitRel50I_L1();		//��ѹ�����1��
	 InitRel50I_L2();		//��ѹ�����2��
	 InitRel50I_SH();		//ĸ�߳�籣��
	 InitRel50I_N1();		//�������1��
	 InitRel50I_N2();		//�������2��
	 InitRel59U_NH();		//��ѹ�������ѹ
	 InitRel59U_NL();		//��ѹ�������ѹ
	 InitRel50OL1();		//������1��
	 InitRel50OL2();		//������2��
	 InitRel50OL3();		//������3��
	 InitRel50IJX();		//��϶��������
	 InitRel27U();			//ʧѹ����
	 InitRel49I();			//��ʱ���ȹ����ɱ���
	 InitR87IAlarm();		//����Խ�޸澯
	 InitRel50OLA();		//�����ɸ澯 2014��8��27�� 11:40:52 yanzh ����
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
void CRelayTask::OnSampleEnd(DWORD arg)
{	
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}

	static WORD wInvalidCheckCnt;
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;

	
	//���Ƚ��п����Լ�,ÿ�α��������һ·����
	if(!m_bLogicChanged)
		BoBreakCheck();

	//��ʼ������ͨ����������
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	CaluUI();
	if(*CFG_pbyHAB == CFG_STATUS_CLR)
	{
		
		ComposeData();		          
		CaluMaxMin();
		CaluR87();
		Calu49I();
		Calu47U();
		R87IAlarm(&R87Alarm);
		PTCheck();	
	}
	else
	{
		if(IA.Mod>=IB.Mod)
			IHmax = IA;
		else
			IHmax = IB;
		if(UA.Mod>=UB.Mod)
			U_27U = UB;
		else
			U_27U = UA;
		InitFalutHAB();
	}
	
	FalutProcess();								//����ѭ������
	GeneralOverValueRelay(&Rel50OL_A);  //���ӹ����ɸ澯��2014��8��27�� 13:46:27 yanzh
	GeneralUnderValRelay(&Rel27U);					//�͵�ѹ�����ӳ���	
	Process52BF();
	
	if(g_bRel49TimeOut == TRUE)
	{
		ThermalOverLoad();
		g_bRel49TimeOut = FALSE;
	}
	//�߼����̴���
	LogicProcess();
	
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
		
	//д����¼��
	WriteDCChannel();

}
/************************************************************************************************
*����: ������ͨ����д��¼������
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::WriteDCChannel()
{
	LONG i;
	RecordRelayWord(DCMacroArray,DCCaluResult);	
	for(i =0;i<RELAY_SAMP_INTERVAL;i++)
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
void CRelayTask::ComposeData(void)
{	
	LONG tempA1,tempA2;
	LONG tempB1,tempB2;
	LONG tempC1,tempC2;

	for(LONG j=0;j<READ_POINT_NUM;j++)
	{
		dwLineUDataBuf[j] = dwSampBuf[j]-dwSampBuf[READ_POINT_NUM+j]; //UAB=UA-UB
		dwLineUDataBuf[READ_POINT_NUM+j] = dwSampBuf[READ_POINT_NUM+j]-dwSampBuf[READ_POINT_NUM*2+j]; //UBC=UB-UC
		dwLineUDataBuf[READ_POINT_NUM*2+j] = dwSampBuf[READ_POINT_NUM*2+j]-dwSampBuf[j];//UCA=UC-UA

		dwLineUDataBuf[READ_POINT_NUM*3+j] = dwSampBuf[READ_POINT_NUM*4+j]-dwSampBuf[READ_POINT_NUM*5+j]; //Uab=Ua-Ub
		dwLineUDataBuf[READ_POINT_NUM*4+j] = dwSampBuf[READ_POINT_NUM*5+j]-dwSampBuf[READ_POINT_NUM*6+j]; //Ubc=Ub-Uc
		dwLineUDataBuf[READ_POINT_NUM*5+j] = dwSampBuf[READ_POINT_NUM*6+j]-dwSampBuf[READ_POINT_NUM*4+j]; //Uca=Uc-Ua		
	}
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		//��ѹ��
		if(*CFG_pbyHCT1CLOCK == CFG_STATUS_SET) // ��ѹ��1��У��Ͷ��
		{
			tempA1 = (dwSampBuf[READ_POINT_NUM*8+i]-dwSampBuf[READ_POINT_NUM*10+i])*100;//A-C
			tempB1 = (dwSampBuf[READ_POINT_NUM*9+i]-dwSampBuf[READ_POINT_NUM*8+i])*100;//B-A
			tempC1 = (dwSampBuf[READ_POINT_NUM*10+i]-dwSampBuf[READ_POINT_NUM*9+i])*100;//C-B
		}
		else if(*CFG_pbyHCT11CLOCK == CFG_STATUS_SET) //��ѹ��11��У��Ͷ��
		{
			tempA1 = (dwSampBuf[READ_POINT_NUM*8+i]-dwSampBuf[READ_POINT_NUM*9+i])*100;//A-B
			tempB1 = (dwSampBuf[READ_POINT_NUM*9+i]-dwSampBuf[READ_POINT_NUM*10+i])*100;//B-C
			tempC1 = (dwSampBuf[READ_POINT_NUM*10+i]-dwSampBuf[READ_POINT_NUM*8+i])*100;//C-A
		}
		else
		{
			tempA1 = (dwSampBuf[READ_POINT_NUM*8+i])*100;//A
			tempB1 = (dwSampBuf[READ_POINT_NUM*9+i])*100;//B
			tempC1 = (dwSampBuf[READ_POINT_NUM*10+i])*100;//C
		}
		//��ѹ��  11-a,13-b,15-c
		if(*CFG_pbyLCT1CLOCK == CFG_STATUS_SET) //��ѹ��1��У��Ͷ��
		{
			tempA2 = dwSampBuf[READ_POINT_NUM*11+i]-dwSampBuf[READ_POINT_NUM*15+i];//A-C
			tempB2 = dwSampBuf[READ_POINT_NUM*13+i]-dwSampBuf[READ_POINT_NUM*11+i];//B-A
			tempC2 = dwSampBuf[READ_POINT_NUM*15+i]-dwSampBuf[READ_POINT_NUM*13+i];//C-B
		}
		else if(*CFG_pbyLCT11CLOCK == CFG_STATUS_SET) //��ѹ��11��У��Ͷ��
		{
			tempA2 = dwSampBuf[READ_POINT_NUM*11+i]-dwSampBuf[READ_POINT_NUM*13+i];//A-B
			tempB2 = dwSampBuf[READ_POINT_NUM*13+i]-dwSampBuf[READ_POINT_NUM*15+i];//B-C
			tempC2 = dwSampBuf[READ_POINT_NUM*15+i]-dwSampBuf[READ_POINT_NUM*11+i];//C-A
		}
		else
		{
			tempA2 = dwSampBuf[READ_POINT_NUM*11+i];//A
			tempB2 = dwSampBuf[READ_POINT_NUM*13+i];//B
			tempC2 = dwSampBuf[READ_POINT_NUM*15+i];//C
		}
		dwComposeDataBuf[i] = (tempA1 +(*RS_pnkb)*tempA2)/100;
		dwComposeDataBuf[READ_POINT_NUM+i] = (tempA1 -(*RS_pnkb)*tempA2)/200;
		dwComposeDataBuf[READ_POINT_NUM*2+i] = (tempB1 +(*RS_pnkb)*tempB2)/100;
		dwComposeDataBuf[READ_POINT_NUM*3+i] = (tempB1 -(*RS_pnkb)*tempB2)/200;
		dwComposeDataBuf[READ_POINT_NUM*4+i] = (tempC1 +(*RS_pnkb)*tempC2)/100;
		dwComposeDataBuf[READ_POINT_NUM*5+i] = (tempC1 -(*RS_pnkb)*tempC2)/200;		
	}

	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{		
		dwRecBuf[i] = dwComposeDataBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwComposeDataBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*2+i] = dwComposeDataBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*3+i] = dwComposeDataBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*4+i] = dwComposeDataBuf[READ_POINT_NUM*5-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*5+i] = dwComposeDataBuf[READ_POINT_NUM*6-RELAY_SAMP_INTERVAL+i];
		WriteComposeData(dwRecBuf, dwComposeMask);
	}
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
			phase = PHASE_A;
			return ele1;
		}
		else
		{
			phase = PHASE_C;
			return ele3;
		}
	}
	else
	{
		if(ele2.Mod>=ele3.Mod)
		{
			phase = PHASE_B;
			return ele2;
		}
		else
		{
			phase = PHASE_C;
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
// ����: �������	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUI(void)
{
	LONG angle;
	//����UAģֵ��д������������UA��ѹ�̵����֡���ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf,&UA);
	CaluModValue(&UA);
	CaluAngle(&UA,AngleTab);
	*RM_pnUHA= UA.Mod;
	*RM_pnAUHA = 0;
	DB_LogWriteRM(RM_UHA,*RM_pnUHA);
	DB_LogWriteRM(RM_AUHA,*RM_pnAUHA);	
	if(UA.Mod>=*AS_pn59UAV)		//��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59UHA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UHA, RELAY_OFF);
	}
	if(UA.Mod>=*AS_pn27UAV)
	{
		WriteRelayWord(RW_27UHA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UHA, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UB);
	CaluModValue(&UB);
	CaluAngle(&UB,AngleTab);
	*RM_pnUHB= UB.Mod;
	angle = UB.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAUHB = angle;
	else
		*RM_pnAUHB = angle+36000;
	DB_LogWriteRM(RM_UHB,*RM_pnUHB);
	DB_LogWriteRM(RM_AUHB,*RM_pnAUHB);	
	if(UB.Mod>=*AS_pn59UBV)		//��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59UHB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UHB, RELAY_OFF);
	}
	if(UB.Mod>=*AS_pn27UBV)
	{
		WriteRelayWord(RW_27UHB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UHB, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UC);
	CaluModValue(&UC);
	CaluAngle(&UC,AngleTab);
	*RM_pnUHC= UC.Mod;
	angle = UC.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAUHC = angle;
	else
		*RM_pnAUHC = angle+36000;
	DB_LogWriteRM(RM_UHC,*RM_pnUHC);
	DB_LogWriteRM(RM_AUHC,*RM_pnAUHC);	
	if(UC.Mod>=*AS_pn59UCV)		//��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59UHC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UHC, RELAY_OFF);
	}
	if(UC.Mod>=*AS_pn27UCV)
	{
		WriteRelayWord(RW_27UHC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UHC, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0h);
	CaluModValue(&U0h);
	CaluAngle(&U0h,AngleTab);
	*RM_pnU0H= U0h.Mod;
	DB_LogWriteRM(RM_U0H,*RM_pnU0H);
	if(U0h.Mod>=*AS_pn59U0hV) 	//��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59U0H, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U0H, RELAY_OFF);
	}
	if(U0h.Mod>=*AS_pn27U0hV)
	{
		WriteRelayWord(RW_27U0H, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U0H, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&Ua);
	CaluModValue(&Ua);
	CaluAngle(&Ua,AngleTab);
	*RM_pnULa= Ua.Mod;
	angle = Ua.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAULa = angle;
	else
		*RM_pnAULa = angle+36000;
	DB_LogWriteRM(RM_ULA,*RM_pnULa);
	DB_LogWriteRM(RM_AULA,*RM_pnAULa);
	if(Ua.Mod>=*AS_pn59UaV)	//��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59ULA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59ULA, RELAY_OFF);
	}
	if(Ua.Mod>=*AS_pn27UaV)
	{
		WriteRelayWord(RW_27ULA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27ULA, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&Ub);
	CaluModValue(&Ub);
	CaluAngle(&Ub,AngleTab);
	*RM_pnULb= Ub.Mod;
	angle = Ub.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAULb = angle;
	else
		*RM_pnAULb = angle+36000;
	DB_LogWriteRM(RM_ULB,*RM_pnULb);
	DB_LogWriteRM(RM_AULB,*RM_pnAULb);
	if(Ub.Mod>=*AS_pn59UbV) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59ULB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59ULB, RELAY_OFF);
	}
	if(Ub.Mod>=*AS_pn27UbV)
	{
		WriteRelayWord(RW_27ULB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27ULB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&Uc);
	CaluModValue(&Uc);
	CaluAngle(&Uc,AngleTab);
	*RM_pnULc= Uc.Mod;
	angle = Uc.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAULc = angle;
	else
		*RM_pnAULc = angle+36000;
	DB_LogWriteRM(RM_ULC,*RM_pnULc);
	DB_LogWriteRM(RM_AULC,*RM_pnAULc);
	if(Uc.Mod>=*AS_pn59UcV) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59ULC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59ULC, RELAY_OFF);
	}
	if(Uc.Mod>=*AS_pn27UcV)
	{
		WriteRelayWord(RW_27ULC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27ULC, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&U0l);
	CaluModValue(&U0l);
	*RM_pnU0L= U0l.Mod;
	DB_LogWriteRM(RM_U0L,*RM_pnU0L);
	if(U0l.Mod>=*AS_pn59U0lV) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59U0L, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U0L, RELAY_OFF);
	}
	if(U0l.Mod>=*AS_pn27U0lV)
	{
		WriteRelayWord(RW_27U0L, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U0L, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IA);
	CaluModValue(&IA);
	CaluAngle(&IA,AngleTab);
	*RM_pnIHA= IA.Mod;
	angle = IA.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAIHA = angle;
	else
		*RM_pnAIHA = angle+36000;
	DB_LogWriteRM(RM_IHA,*RM_pnIHA);
	DB_LogWriteRM(RM_AIHA,*RM_pnAIHA);
	if(IA.Mod>=*AS_pn50IAC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50IHA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IHA, RELAY_OFF);
	}
	if(IA.Mod>=*AS_pn37IAC)
	{
		WriteRelayWord(RW_37IHA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IHA, RELAY_ON);
		
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&IB);
	CaluModValue(&IB);
	CaluAngle(&IB,AngleTab);
	*RM_pnIHB= IB.Mod;
	angle = IB.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAIHB = angle;
	else
		*RM_pnAIHB = angle+36000;
	DB_LogWriteRM(RM_IHB,*RM_pnIHB);
	DB_LogWriteRM(RM_AIHB,*RM_pnAIHB);
	if(IB.Mod>=*AS_pn50IBC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50IHB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IHB, RELAY_OFF);
	}
	if(IB.Mod>=*AS_pn37IBC)
	{
		WriteRelayWord(RW_37IHB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IHB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&IC);
	CaluModValue(&IC);
	CaluAngle(&IC,AngleTab);
	*RM_pnIHC= IC.Mod;
	angle = IC.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAIHC = angle;
	else
		*RM_pnAIHC = angle+36000;
	DB_LogWriteRM(RM_IHC,*RM_pnIHC);
	DB_LogWriteRM(RM_AIHC,*RM_pnAIHC);
	if(IC.Mod>=*AS_pn50ICC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50IHC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IHC, RELAY_OFF);
	}
	if(IC.Mod>=*AS_pn37ICC)
	{
		WriteRelayWord(RW_37IHC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IHC, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&Ia);
	CaluModValue(&Ia);
	CaluAngle(&Ia,AngleTab);
	*RM_pnILa= Ia.Mod;
	angle = Ia.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAILa = angle;
	else
		*RM_pnAILa = angle+36000;
	DB_LogWriteRM(RM_ILA,*RM_pnILa);
	DB_LogWriteRM(RM_AILA,*RM_pnAILa);
	if(Ia.Mod>=*AS_pn50IaC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50ILA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50ILA, RELAY_OFF);
	}
	if(Ia.Mod>=*AS_pn37IaC)
	{
		WriteRelayWord(RW_37ILA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37ILA, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&Ib);
	CaluModValue(&Ib);
	CaluAngle(&Ib,AngleTab);
	*RM_pnILb= Ib.Mod;
	angle = Ib.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAILb = angle;
	else
		*RM_pnAILb = angle+36000;
	DB_LogWriteRM(RM_ILB,*RM_pnILb);
	DB_LogWriteRM(RM_AILB,*RM_pnAILb);
	if(Ib.Mod>=*AS_pn50IbC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50ILB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50ILB, RELAY_OFF);
	}
	if(Ib.Mod>=*AS_pn37IbC)
	{
		WriteRelayWord(RW_37ILB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37ILB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*15,&Ic);
	CaluModValue(&Ic);
	CaluAngle(&Ic,AngleTab);
	*RM_pnILc= Ic.Mod;
	angle = Ic.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAILc = angle;
	else
		*RM_pnAILc = angle+36000;
	DB_LogWriteRM(RM_ILC,*RM_pnILc);
	DB_LogWriteRM(RM_AILC,*RM_pnAILc);
	if(Ic.Mod>=*AS_pn50IcC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50ILC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50ILC, RELAY_OFF);
	}
	if(Ic.Mod>=*AS_pn37IcC)
	{
		WriteRelayWord(RW_37ILC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37ILC, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&I0);
	CaluModValue(&I0);
	CaluAngle(&I0,AngleTab);
	*RM_pnI0= I0.Mod;
	DB_LogWriteRM(RM_I0,*RM_pnI0);
	if(I0.Mod>=*AS_pn50I0C) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50I0, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I0, RELAY_OFF);
	}
	if(I0.Mod>=*AS_pn37I0C)
	{
		WriteRelayWord(RW_37I0, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I0, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*14,&Ijx);
	CaluModValue(&Ijx);
	CaluAngle(&Ijx,AngleTab);
	*RM_pnIjx= Ijx.Mod;
	DB_LogWriteRM(RM_IJX,*RM_pnIjx);
	if(Ijx.Mod>=*AS_pn50IjxC) //��UA��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_50IJX, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IJX, RELAY_OFF);
	}
	if(Ijx.Mod>=*AS_pn37IjxC)
	{
		WriteRelayWord(RW_37IJX, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IJX, RELAY_ON);
	}
		
}
void CRelayTask::CaluMaxMin(void)
{
	//�����ߵ�ѹ�����ڹ��ʷ����90������ж�
	CaluBaseFourier_S15(dwLineUDataBuf,&UAB);
	CaluModValue(&UAB);	
	CaluAngle(&UAB,AngleTab);	
	*RM_pnUAB= UAB.Mod;
	DB_LogWriteRM(RM_UAB,*RM_pnUAB);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM,&UBC);
	CaluModValue(&UBC);	
	CaluAngle(&UBC,AngleTab);	
	*RM_pnUBC= UBC.Mod;
	DB_LogWriteRM(RM_UBC,*RM_pnUBC);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*2,&UCA);
	CaluModValue(&UCA);	
	CaluAngle(&UCA,AngleTab);
	*RM_pnUCA= UCA.Mod;
	DB_LogWriteRM(RM_UCA,*RM_pnUCA);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*3,&Uab);
	CaluModValue(&Uab);		
	CaluAngle(&Uab,AngleTab);
	*RM_pnULab= Uab.Mod;
	DB_LogWriteRM(RM_ULAB,*RM_pnULab);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*4,&Ubc);
	CaluModValue(&Ubc);	
	CaluAngle(&Ubc,AngleTab);
	*RM_pnULbc= Ubc.Mod;
	DB_LogWriteRM(RM_ULBC,*RM_pnULbc);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*5,&Uca);
	CaluModValue(&Uca);
	CaluAngle(&Uca,AngleTab);
	*RM_pnULca= Uca.Mod;
	DB_LogWriteRM(RM_ULCA,*RM_pnULca);

	IHmax = ElecMax(IA,IB,IC,wHIPhase);
	ILmax = ElecMax(Ia,Ib,Ic,wLIPhase);
	UHmax = ElecMax(UA,UB,UC);
	UHmin = ElecMin(UAB,UBC,UCA);
	ULmax = ElecMax(Ua,Ub,Uc);
	ULmin = ElecMin(Uab,Ubc,Uca);
	if(UHmax.Mod>=ULmax.Mod)
		U_27U = UHmax;
	else
		U_27U = ULmax;

	if(wHIPhase == PHASE_A)
	{
		Rel50I_H1.PForwardRelayWord = RW_PFHA; 
		Rel50I_H1.PRewardRelayWord = RW_PRHA;			
		Rel50I_H2.PForwardRelayWord = RW_PFHA; 
		Rel50I_H2.PRewardRelayWord = RW_PRHA;
		Rel50I_H3.PForwardRelayWord = RW_PFHA; 
		Rel50I_H3.PRewardRelayWord = RW_PRHA;
		
//		HPFRW = RW_PFHA;
//		HPRRW = RW_PRHA;
	}
	else if(wHIPhase == PHASE_B)
	{		
		Rel50I_H1.PForwardRelayWord = RW_PFHB; 
		Rel50I_H1.PRewardRelayWord = RW_PRHB;			
		Rel50I_H2.PForwardRelayWord = RW_PFHB; 
		Rel50I_H2.PRewardRelayWord = RW_PRHB;
		Rel50I_H3.PForwardRelayWord = RW_PFHB; 
		Rel50I_H3.PRewardRelayWord = RW_PRHB;
//		HPFRW = RW_PFHB;
//		HPRRW = RW_PRHB;
	}
	else if(wHIPhase == PHASE_C)
	{		
		Rel50I_H1.PForwardRelayWord = RW_PFHC; 
		Rel50I_H1.PRewardRelayWord = RW_PRHC;			
		Rel50I_H2.PForwardRelayWord = RW_PFHC; 
		Rel50I_H2.PRewardRelayWord = RW_PRHC;
		Rel50I_H3.PForwardRelayWord = RW_PFHC; 
		Rel50I_H3.PRewardRelayWord = RW_PRHC;
//		HPFRW = RW_PFHC;
//		HPRRW = RW_PRHC;
	}
	
	if(wLIPhase == PHASE_A)
	{
		Rel50I_L1.PForwardRelayWord = RW_PFLA;
		Rel50I_L1.PRewardRelayWord= RW_PRLA;		
		Rel50I_L2.PForwardRelayWord = RW_PFLA;
		Rel50I_L2.PRewardRelayWord= RW_PRLA;
//		LPFRW = RW_PFLA;
//		LPRRW = RW_PRLA;
	}
	else if(wLIPhase == PHASE_B)
	{		
		Rel50I_L1.PForwardRelayWord = RW_PFLB;
		Rel50I_L1.PRewardRelayWord= RW_PRLB;		
		Rel50I_L2.PForwardRelayWord = RW_PFLB;
		Rel50I_L2.PRewardRelayWord= RW_PRLB;
//		LPFRW = RW_PFLB;
//		LPRRW = RW_PRLB;
	}
	else if(wLIPhase == PHASE_C)
	{		
		Rel50I_L1.PForwardRelayWord = RW_PFLC;
		Rel50I_L1.PRewardRelayWord= RW_PRLC;		
		Rel50I_L2.PForwardRelayWord = RW_PFLC;
		Rel50I_L2.PRewardRelayWord= RW_PRLC;
//		LPFRW = RW_PFLC;
//		LPRRW = RW_PRLC;
	}
}

//�����������ƶ�����
void CRelayTask::CaluR87(void)
{
	CaluBaseFourier_S15(dwComposeDataBuf,&Ida);
	CaluModValue(&Ida);		
	*RM_pnIDA= Ida.Mod;
	DB_LogWriteRM(RM_IDA,*RM_pnIDA);
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM,&Ira);
	CaluModValue(&Ira);	
	*RM_pnIRA= Ira.Mod;
	DB_LogWriteRM(RM_IRA,*RM_pnIRA);

	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*2,&Idb);
	CaluModValue(&Idb); 
	*RM_pnIDB= Idb.Mod;
	DB_LogWriteRM(RM_IDB,*RM_pnIDB);
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*3,&Irb);
	CaluModValue(&Irb);
	*RM_pnIRB= Irb.Mod;
	DB_LogWriteRM(RM_IRB,*RM_pnIRB);
	
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*4,&Idc);
	CaluModValue(&Idc); 
	*RM_pnIDC= Idc.Mod;
	DB_LogWriteRM(RM_IDC,*RM_pnIDC);
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*5,&Irc);
	CaluModValue(&Irc);
	*RM_pnIRC= Irc.Mod;
	DB_LogWriteRM(RM_IRC,*RM_pnIRC);

	CaluSecondFourier(dwComposeDataBuf,&Ida2);
	CaluModValue(&Ida2);
	*RM_pnIDA2= Ida2.Mod;
	DB_LogWriteRM(RM_IDA2,*RM_pnIDA2);
	CaluThirdFourier(dwComposeDataBuf,&Ida3);
	CaluModValue(&Ida3);
	*RM_pnIDA3= Ida3.Mod;
	DB_LogWriteRM(RM_IDA3,*RM_pnIDA3);
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM,&Ira2);
	CaluModValue(&Ira2);	
	*RM_pnIRA2= Ira2.Mod;
	DB_LogWriteRM(RM_IRA2,*RM_pnIRA2);
	
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*2,&Idb2);
	CaluModValue(&Idb2);	
	*RM_pnIDB2= Idb2.Mod;
	DB_LogWriteRM(RM_IDB2,*RM_pnIDB2);
	CaluThirdFourier(dwComposeDataBuf+READ_POINT_NUM*2,&Idb3);
	CaluModValue(&Idb3);	
	*RM_pnIDB3= Idb3.Mod;
	DB_LogWriteRM(RM_IDB3,*RM_pnIDB3);
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*3,&Irb2);
	CaluModValue(&Irb2);
	*RM_pnIRB2= Irb2.Mod;
	DB_LogWriteRM(RM_IRB2,*RM_pnIRB2);

	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*4,&Idc2);
	CaluModValue(&Idc2);	
	*RM_pnIDC2= Idc2.Mod;
	DB_LogWriteRM(RM_IDC2,*RM_pnIDC2);
	CaluThirdFourier(dwComposeDataBuf+READ_POINT_NUM*4,&Idc3);
	CaluModValue(&Idc3);	
	*RM_pnIDC3= Idc3.Mod;
	DB_LogWriteRM(RM_IDC3,*RM_pnIDC3);
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*5,&Irc2);
	CaluModValue(&Irc2);	
	*RM_pnIRC2= Irc2.Mod;
	DB_LogWriteRM(RM_IRC2,*RM_pnIRC2);

	//δ����������г��ֵ������������ֵ�����г������  2013-08-29
	*RM_pnIDA5= 0;
	DB_LogWriteRM(RM_IDA5,*RM_pnIDA5);
	*RM_pnIDB5= 0;
	DB_LogWriteRM(RM_IDB5,*RM_pnIDB5);
	*RM_pnIDC5= 0;
	DB_LogWriteRM(RM_IDC5,*RM_pnIDC5);
	
}
void CRelayTask::Calu49I()
{
	if((*CFG_pby49I== CFG_STATUS_SET)&&(*SW_pby49I== SW_STATUS_SET))
	{
		Filter(dwSampBuf+READ_POINT_NUM*8);
		Filter(dwSampBuf+READ_POINT_NUM*9);
		Filter(dwSampBuf+READ_POINT_NUM*10);
		
		CaluMSMod(dwSampBuf+READ_POINT_NUM*8, dw49IA);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*9, dw49IB);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*10, dw49IC);
		
		Rel49IA.pCurrent = (dw49IA+Rel49IA.PreCurrent)/2;
		Rel49IA.PreCurrent = Rel49IA.pCurrent;
		Rel49IB.pCurrent = (dw49IB+Rel49IB.PreCurrent)/2;
		Rel49IB.PreCurrent = Rel49IB.pCurrent;
		Rel49IC.pCurrent = (dw49IC+Rel49IC.PreCurrent)/2;
		Rel49IC.PreCurrent = Rel49IC.pCurrent;
	}
}
TRelElecValPar CRelayTask::CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc) //���㸺���ѹ
{
	TRelElecValPar u2;
	u2.Real = ua->Real/3-ub->Real/6+866*ub->Imag/3000-uc->Real/6-866*uc->Imag/3000;
	u2.Imag = ua->Imag/3-ub->Imag/6-866*ub->Real/3000-uc->Imag/6+866*uc->Real/3000;
	CaluModValue(&u2);
	return u2;
}

void CRelayTask::Calu47U(void)
{
	U2h = CaluU2(&UA,&UB,&UC);
	U2l = CaluU2(&Ua,&Ub,&Uc);

	
	*RM_pnU2H= U2h.Mod;
	DB_LogWriteRM(RM_U2H,*RM_pnU2H);
	*RM_pnU2L= U2l.Mod;
	DB_LogWriteRM(RM_U2L,*RM_pnU2L);
	
	if((UHmin.Mod<*RS_pn47U_27U1)||(U2h.Mod>*RS_pn47U_59U2))
		WriteRelayWord(RW_47UH,RELAY_OFF);
	else
		WriteRelayWord(RW_47UH,RELAY_ON);
	
	if((ULmin.Mod<*RS_pn47U_27U1)||(U2l.Mod>*RS_pn47U_59U2))
		WriteRelayWord(RW_47UL,RELAY_OFF);
	else
		WriteRelayWord(RW_47UL,RELAY_ON);
		
}

//��ѹ������Ͷ��ʱ����ӦʧЧ�����Ĺ��ϵ�������
void CRelayTask::InitFalutHAB()		
{
	TRelElecValPar ZeroHAB;
	ZeroHAB.Real = 0;
	ZeroHAB.Imag = 0;
	ZeroHAB.Mod = 0;
	
	U2h=U2l=UAB=UBC=UCA=Uab=Ubc=Uca=ZeroHAB;		//��ѹ�ฺ���ѹ
	Ida=Idb=Idc=Ira=Irb=Irc=Ida2=Ida3=Idb2=Idb3=Idc2=Idc3=Ira2=Irb2=Irc2=ZeroHAB;		//A�����
	ILmax = ZeroHAB; //��ѹ����������
	Rel49IA.pCurrent = 0;
	Rel49IB.pCurrent = 0;
	Rel49IC.pCurrent = 0;
	if(ReadRelayWord(RW_47UH))
		WriteRelayWord(RW_47UH,RELAY_OFF);//�帴ѹ������־
	if(ReadRelayWord(RW_47UL))
		WriteRelayWord(RW_47UL,RELAY_OFF);

	*RM_pnUAB=0;
	DB_LogWriteRM(RM_UAB,*RM_pnUAB);
	*RM_pnUBC=0;
	DB_LogWriteRM(RM_UBC,*RM_pnUBC);
	*RM_pnUCA=0;
	DB_LogWriteRM(RM_UCA,*RM_pnUCA);
	*RM_pnULab=0;
	DB_LogWriteRM(RM_ULAB,*RM_pnULab);
	*RM_pnULbc=0;
	DB_LogWriteRM(RM_ULBC,*RM_pnULbc);
	*RM_pnULca=0;
	DB_LogWriteRM(RM_ULCA,*RM_pnULca);
	*RM_pnU2H= 0;
	DB_LogWriteRM(RM_U2H,*RM_pnU2H);
	*RM_pnU2L= 0;
	DB_LogWriteRM(RM_U2L,*RM_pnU2L);
	*RM_pnIDA= 0;
	DB_LogWriteRM(RM_IDA,*RM_pnIDA);
	*RM_pnIDB= 0;
	DB_LogWriteRM(RM_IDB,*RM_pnIDB);
	*RM_pnIDC= 0;
	DB_LogWriteRM(RM_IDC,*RM_pnIDC);
	*RM_pnIRA= 0;
	DB_LogWriteRM(RM_IRA,*RM_pnIRA);
	*RM_pnIRB= 0;
	DB_LogWriteRM(RM_IRB,*RM_pnIRB);
	*RM_pnIRC= 0;
	DB_LogWriteRM(RM_IRC,*RM_pnIRC);
	*RM_pnIDA2= 0;
	DB_LogWriteRM(RM_IDA2,*RM_pnIDA2);
	*RM_pnIDA3= 0;
	DB_LogWriteRM(RM_IDA3,*RM_pnIDA3);
	*RM_pnIDA5= 0;
	DB_LogWriteRM(RM_IDA5,*RM_pnIDA5);
	*RM_pnIDB2= 0;
	DB_LogWriteRM(RM_IDB2,*RM_pnIDB2);
	*RM_pnIDB3= 0;
	DB_LogWriteRM(RM_IDB3,*RM_pnIDB3);
	*RM_pnIDB5= 0;
	DB_LogWriteRM(RM_IDB5,*RM_pnIDB5);
	*RM_pnIDC2= 0;
	DB_LogWriteRM(RM_IDC2,*RM_pnIDC2);
	*RM_pnIDC3= 0;
	DB_LogWriteRM(RM_IDC3,*RM_pnIDC3);
	*RM_pnIDC5= 0;
	DB_LogWriteRM(RM_IDC5,*RM_pnIDC5);
	*RM_pnIRA2= 0;
	DB_LogWriteRM(RM_IRA2,*RM_pnIRA2);
	*RM_pnIRB2= 0;
	DB_LogWriteRM(RM_IRB2,*RM_pnIRB2);
	*RM_pnIRC2= 0;
	DB_LogWriteRM(RM_IRC2,*RM_pnIRC2);
}
//void CRelayTask::InitRepSerialNum()
//{
//	
//}
//A���ٶϱ���
void CRelayTask::InitRel87U_A()
{
	Rel87U_A.pElecSet				= RS_pn87UC;          			//��������������ֵ
	Rel87U_A.pTimeSet				= &TimeSet;				 		//��������ʱ�䶨ֵ
	Rel87U_A.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	Rel87U_A.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	Rel87U_A.pAccActTimeSet			= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel87U_A.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel87U_A.pRelCFG				= CFG_pby87U;					//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel87U_A.pRelSW					= SW_pby87U;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel87U_A.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_A.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_A.pRelPDCFG				= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_A.pRelVolLockCFG			= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_A.pRelFDCFG				= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel87U_A.pRelAccCFG				= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel87U_A.pRelFLCFG				= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel87U_A.pSysTimer				= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel87U_A.pRelElec 				= &Ida;							//��ǰ����ָ��		
	Rel87U_A.pRelHarm3				= NULL;							//��ǰ����г��ָ��	
	Rel87U_A.pRelHarm5				= NULL;							//��ǰ���г��ָ��	
	Rel87U_A.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	Rel87U_A.StartRelayWord			= RW_PI87UA;					//����������μ̵�����
	Rel87U_A.ActRelayWord			= RW_TR87UA;					//�������������̵�����
	Rel87U_A.AccRelayWord			= FALSE1;						//���������̵�����
	Rel87U_A.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	Rel87U_A.PForwardRelayWord		= FALSE1;						//����������̵�����
	Rel87U_A.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	Rel87U_A.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	Rel87U_A.LockRelayWord			= RW_LKPI87UA;					//��α����̵�����
	Rel87U_A.RSTRelayWord			= RW_LKRST87UA;				//���ر����̵�����
	Rel87U_A.LoopCtrlRelayWord		= RW_LPC87UA;					//����ѭ�����Ƽ̵�����
	
	Rel87U_A.wReportNum				= R_87U;						//���������������
	Rel87U_A.wAccReportNum			= 0xFFFF;						//���ٶ����������

	Rel87U_A.bMakeActRpt			= TRUE;
	Rel87U_A.dwK35					= NULL;
	Rel87U_A.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel87U_A.wPickUpReportNum		= RPT_NULL;						//��α������к�
	Rel87U_A.wResetReportNum		= RPT_NULL;						//���ر������к�
	//**********************************************************************	


//	Rel87U_A.dwRelTimer				= 0;			 			//��ʱ��
	Rel87U_A.byRelFCounter			= 0;						//���ϼ�����
	Rel87U_A.byRelRCounter			= 0;						//���ؼ�����	
	
	Rel87U_A.wExcepWordNum     		= 8;
}

//B���ٶϱ���
void CRelayTask::InitRel87U_B()
{
	Rel87U_B.pElecSet				= RS_pn87UC;          			//��������������ֵ
	Rel87U_B.pTimeSet				= &TimeSet;				 		//��������ʱ�䶨ֵ
	Rel87U_B.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	Rel87U_B.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	Rel87U_B.pAccActTimeSet			= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel87U_B.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel87U_B.pRelCFG				= CFG_pby87U;					//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel87U_B.pRelSW					= SW_pby87U;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel87U_B.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_B.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_B.pRelPDCFG				= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_B.pRelVolLockCFG			= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_B.pRelFDCFG				= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel87U_B.pRelAccCFG				= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel87U_B.pRelFLCFG				= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel87U_B.pSysTimer				= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel87U_B.pRelElec 				= &Idb;							//��ǰ����ָ��		
	Rel87U_B.pRelHarm3				= NULL;							//��ǰ����г��ָ��	
	Rel87U_B.pRelHarm5				= NULL;							//��ǰ���г��ָ��	
	Rel87U_B.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	Rel87U_B.StartRelayWord			= RW_PI87UB;					//����������μ̵�����
	Rel87U_B.ActRelayWord			= RW_TR87UB;					//�������������̵�����
	Rel87U_B.AccRelayWord			= FALSE1;						//���������̵�����
	Rel87U_B.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	Rel87U_B.PForwardRelayWord		= FALSE1;						//����������̵�����
	Rel87U_B.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	Rel87U_B.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	Rel87U_B.LockRelayWord			= RW_LKPI87UB;					//��α����̵�����
	Rel87U_B.RSTRelayWord			= RW_LKRST87UB;				//���ر����̵�����
	Rel87U_B.LoopCtrlRelayWord		= RW_LPC87UB;					//����ѭ�����Ƽ̵�����
	
	Rel87U_B.wReportNum				= R_87U;						//���������������
	Rel87U_B.wAccReportNum			= 0xFFFF;						//���ٶ����������

	Rel87U_B.bMakeActRpt			= TRUE;
	Rel87U_B.dwK35					= NULL;
	Rel87U_B.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel87U_B.wPickUpReportNum		= RPT_NULL;						//��α������к�
	Rel87U_B.wResetReportNum		= RPT_NULL;						//���ر������к�
	//**********************************************************************	


//	Rel87U_B.dwRelTimer				= 0;			 			//��ʱ��
	Rel87U_B.byRelFCounter			= 0;						//���ϼ�����
	Rel87U_B.byRelRCounter			= 0;						//���ؼ�����	
	
	Rel87U_B.wExcepWordNum     		= 8;
}
//C���ٶϱ���
void CRelayTask::InitRel87U_C()
{
	Rel87U_C.pElecSet				= RS_pn87UC;          			//��������������ֵ
	Rel87U_C.pTimeSet				= &TimeSet;				 		//��������ʱ�䶨ֵ
	Rel87U_C.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	Rel87U_C.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	Rel87U_C.pAccActTimeSet			= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel87U_C.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel87U_C.pRelCFG				= CFG_pby87U;					//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel87U_C.pRelSW					= SW_pby87U;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel87U_C.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_C.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_C.pRelPDCFG				= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_C.pRelVolLockCFG			= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel87U_C.pRelFDCFG				= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel87U_C.pRelAccCFG				= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel87U_C.pRelFLCFG				= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel87U_C.pSysTimer				= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel87U_C.pRelElec 				= &Idc;							//��ǰ����ָ��		
	Rel87U_C.pRelHarm3				= NULL;							//��ǰ����г��ָ��	
	Rel87U_C.pRelHarm5				= NULL;							//��ǰ���г��ָ��	
	Rel87U_C.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	Rel87U_C.StartRelayWord			= RW_PI87UC;					//����������μ̵�����
	Rel87U_C.ActRelayWord			= RW_TR87UC;					//�������������̵�����
	Rel87U_C.AccRelayWord			= FALSE1;						//���������̵�����
	Rel87U_C.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	Rel87U_C.PForwardRelayWord		= FALSE1;						//����������̵�����
	Rel87U_C.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	Rel87U_C.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	Rel87U_C.LockRelayWord			= RW_LKPI87UC;					//��α����̵�����
	Rel87U_C.RSTRelayWord			= RW_LKRST87UC;				//���ر����̵�����
	Rel87U_C.LoopCtrlRelayWord		= RW_LPC87UC;					//����ѭ�����Ƽ̵�����
	
	Rel87U_C.wReportNum				= R_87U;						//���������������
	Rel87U_C.wAccReportNum			= 0xFFFF;						//���ٶ����������

	Rel87U_C.bMakeActRpt			= TRUE;
	Rel87U_C.dwK35					= NULL;
	Rel87U_C.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel87U_C.wPickUpReportNum		= RPT_NULL;						//��α������к�
	Rel87U_C.wResetReportNum		= RPT_NULL;						//���ر������к�
	//**********************************************************************	


//	Rel87U_C.dwRelTimer				= 0;			 			//��ʱ��
	Rel87U_C.byRelFCounter			= 0;						//���ϼ�����
	Rel87U_C.byRelRCounter			= 0;						//���ؼ�����	
	
	Rel87U_C.wExcepWordNum     		= 8;
}
//A����ʲ����
void CRelayTask::InitRel87R_A()
{
	Rel87R_A.p87RISet			= RS_pn87RC; 	//�������ֵ
	Rel87R_A.pIRS1Set			= RS_pnIRS1; 	//�ƶ��յ�1��ֵ
	Rel87R_A.pIRS2Set			= RS_pnIRS2; 	//�ƶ��յ�2��ֵ 
	Rel87R_A.pSLP1Set			= RS_pnSLP1;		//�����ƶ�ϵ��1
	Rel87R_A.pSLP2Set			= RS_pnSLP2;		//�����ƶ�ϵ��2 
	Rel87R_A.pKrh2Set			= RS_pnkrh2;		//����г���ƶ�ϵ��
	Rel87R_A.pKrh5Set			= NULL;				//���г���ƶ�ϵ��
	Rel87R_A.pKravSet			= NULL;		//���ʲ�����ڷ�����Ȩϵ��
	Rel87R_A.pK2avSet			= NULL;		//����г�������ڷ�����Ȩϵ��

	Rel87R_A.p87RCFG			= CFG_pby87R;		//��������
	Rel87R_A.p87RSW				= SW_pby87R;		//��ѹ��
	Rel87R_A.p68IdH2MCFG		= CFG_pby68IDH2MAXLK;		//��������г��������������
	Rel87R_A.p68IdH2CCFG		= CFG_pby68IDH2MULLK;		//��������г���ۺ����������
	Rel87R_A.p68IdH2ICFG		= CFG_pby68IDH2SPLLK;		//��������г�������������
	Rel87R_A.p68IrH2MCFG		= CFG_pby68IRH2LK;		//�ƶ���������г����������
	Rel87R_A.p68IdH5CFG			= &RelCFGCLR;		//����5��г������
	Rel87R_A.p68IdHMulCFG		= CFG_pby68IDMULLK;

	Rel87R_A.pIdH2Flag			= &b68IdH2Flag_A; 	//����г��������־
	Rel87R_A.pIdH5Flag			= &b68IdH5Flag_A; 	//���г��������־
	Rel87R_A.pIrH2Flag			= &b68IrH2Flag_A; 	//�ƶ���������г��������־
	Rel87R_A.pIdHMulFlag		= &b68IdHMulFlag_A;
	

	Rel87R_A.p87RPI				= RS_pn87RC;		//���Ƽ����Ĳ�����ż���ֵ
	Rel87R_A.pId				= &Ida;			//��������
	Rel87R_A.pIr				= &Ira;			//�ƶ�����

	Rel87R_A.ACTRelayWord		= RW_TR87RA;					//�����̵����� 
	Rel87R_A.StartRelayWord		= RW_PI87RA; 				//��μ̵����� 
	Rel87R_A.LockRelayWord		= RW_LKPI87RA;					//��α����̵����� 
	Rel87R_A.RSTRelayWord		= RW_LKRST87RA;					//���ر����̵����� 
	Rel87R_A.LoopCtrlRelayWord	= RW_LPC87RA;				//����ѭ�����Ƽ̵����� 

	Rel87R_A.RptSerialNum		= &RptSerialNumOfAct;					//ϵͳ�������к� 
	Rel87R_A.pSysTimer			= &dwRelayMSCnt;; 					//ϵͳ��ʱ�� 
	Rel87R_A.wRelRptNum			= R_87R; 				//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel87R_A.wPickUpReportNum	= RPT_NULL;						//��α������к�
	Rel87R_A.wResetReportNum	= RPT_NULL;						//���ر������к�
	//**********************************************************************	

	Rel87R_A.wRelFCounter		= 0;					//���ϼ����� 
	Rel87R_A.wRelRCounter		= 0;					//���ؼ�����	
//	Rel87R_A.dwRelTimer			= 0; 					//������ʱ��		
	Rel87R_A.wExcepWordNum		= 7;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 
	Rel87R_A.Info				= 0;							//������Ϣ 
}
//B����ʲ����
void CRelayTask::InitRel87R_B()
{
	Rel87R_B.p87RISet			= RS_pn87RC; 	//�������ֵ
	Rel87R_B.pIRS1Set			= RS_pnIRS1; 	//�ƶ��յ�1��ֵ
	Rel87R_B.pIRS2Set			= RS_pnIRS2; 	//�ƶ��յ�2��ֵ 
	Rel87R_B.pSLP1Set			= RS_pnSLP1;		//�����ƶ�ϵ��1
	Rel87R_B.pSLP2Set			= RS_pnSLP2;		//�����ƶ�ϵ��2 
	Rel87R_B.pKrh2Set			= RS_pnkrh2;		//����г���ƶ�ϵ��
	Rel87R_B.pKrh5Set			= NULL;				//���г���ƶ�ϵ��
	Rel87R_B.pKravSet			= NULL;		//���ʲ�����ڷ�����Ȩϵ��
	Rel87R_B.pK2avSet			= NULL;		//����г�������ڷ�����Ȩϵ��

	Rel87R_B.p87RCFG			= CFG_pby87R;		//��������
	Rel87R_B.p87RSW				= SW_pby87R;		//��ѹ��
	Rel87R_B.p68IdH2MCFG		= CFG_pby68IDH2MAXLK;		//��������г��������������
	Rel87R_B.p68IdH2CCFG		= CFG_pby68IDH2MULLK;		//��������г���ۺ����������
	Rel87R_B.p68IdH2ICFG		= CFG_pby68IDH2SPLLK;		//��������г�������������
	Rel87R_B.p68IrH2MCFG		= CFG_pby68IRH2LK;		//�ƶ���������г����������
	Rel87R_B.p68IdH5CFG			= &RelCFGCLR;			//����5��г������
	Rel87R_B.p68IdHMulCFG		= CFG_pby68IDMULLK;

	Rel87R_B.pIdH2Flag			= &b68IdH2Flag_B; 	//����г��������־
	Rel87R_B.pIdH5Flag			= &b68IdH5Flag_B; 	//���г��������־
	Rel87R_B.pIrH2Flag			= &b68IrH2Flag_B; 	//�ƶ���������г��������־
	Rel87R_B.pIdHMulFlag		= &b68IdHMulFlag_B;
	

	Rel87R_B.p87RPI				= RS_pn87RC;		//���Ƽ����Ĳ�����ż���ֵ
	Rel87R_B.pId				= &Idb;			//��������
	Rel87R_B.pIr				= &Irb;			//�ƶ�����

	Rel87R_B.ACTRelayWord		= RW_TR87RB;					//�����̵����� 
	Rel87R_B.StartRelayWord		= RW_PI87RB; 				//��μ̵����� 
	Rel87R_B.LockRelayWord		= RW_LKPI87RB;					//��α����̵����� 
	Rel87R_B.RSTRelayWord		= RW_LKRST87RB;					//���ر����̵����� 
	Rel87R_B.LoopCtrlRelayWord	= RW_LPC87RB;				//����ѭ�����Ƽ̵����� 

	Rel87R_B.RptSerialNum		= &RptSerialNumOfAct;					//ϵͳ�������к� 
	Rel87R_B.pSysTimer			= &dwRelayMSCnt;; 					//ϵͳ��ʱ�� 
	Rel87R_B.wRelRptNum			= R_87R; 				//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel87R_B.wPickUpReportNum	= RPT_NULL;						//��α������к�
	Rel87R_B.wResetReportNum	= RPT_NULL;						//���ر������к�
	//**********************************************************************	

	Rel87R_B.wRelFCounter		= 0;					//���ϼ����� 
	Rel87R_B.wRelRCounter		= 0;					//���ؼ�����	
//	Rel87R_B.dwRelTimer			= 0; 					//������ʱ��		
	Rel87R_B.wExcepWordNum		= 7;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 
	Rel87R_B.Info				= 0;							//������Ϣ 
}
//C����ʲ����
void CRelayTask::InitRel87R_C()
{
	Rel87R_C.p87RISet			= RS_pn87RC; 	//�������ֵ
	Rel87R_C.pIRS1Set			= RS_pnIRS1; 	//�ƶ��յ�1��ֵ
	Rel87R_C.pIRS2Set			= RS_pnIRS2; 	//�ƶ��յ�2��ֵ 
	Rel87R_C.pSLP1Set			= RS_pnSLP1;		//�����ƶ�ϵ��1
	Rel87R_C.pSLP2Set			= RS_pnSLP2;		//�����ƶ�ϵ��2 
	Rel87R_C.pKrh2Set			= RS_pnkrh2;		//����г���ƶ�ϵ��
	Rel87R_C.pKrh5Set			= NULL;				//���г���ƶ�ϵ��
	Rel87R_C.pKravSet			= NULL;		//���ʲ�����ڷ�����Ȩϵ��
	Rel87R_C.pK2avSet			= NULL;		//����г�������ڷ�����Ȩϵ��

	Rel87R_C.p87RCFG			= CFG_pby87R;		//��������
	Rel87R_C.p87RSW				= SW_pby87R;		//��ѹ��
	Rel87R_C.p68IdH2MCFG		= CFG_pby68IDH2MAXLK;		//��������г��������������
	Rel87R_C.p68IdH2CCFG		= CFG_pby68IDH2MULLK;		//��������г���ۺ����������
	Rel87R_C.p68IdH2ICFG		= CFG_pby68IDH2SPLLK;		//��������г�������������
	Rel87R_C.p68IrH2MCFG		= CFG_pby68IRH2LK;		//�ƶ���������г����������
	Rel87R_C.p68IdH5CFG			= &RelCFGCLR;			//����5��г������
	Rel87R_C.p68IdHMulCFG		= CFG_pby68IDMULLK;

	Rel87R_C.pIdH2Flag			= &b68IdH2Flag_C; 	//����г��������־
	Rel87R_C.pIdH5Flag			= &b68IdH5Flag_C; 	//���г��������־
	Rel87R_C.pIrH2Flag			= &b68IrH2Flag_C; 	//�ƶ���������г��������־
	Rel87R_C.pIdHMulFlag		= &b68IdHMulFlag_C;
	

	Rel87R_C.p87RPI				= RS_pn87RC;		//���Ƽ����Ĳ�����ż���ֵ
	Rel87R_C.pId				= &Idc;			//��������
	Rel87R_C.pIr				= &Irc;			//�ƶ�����

	Rel87R_C.ACTRelayWord		= RW_TR87RC;					//�����̵����� 
	Rel87R_C.StartRelayWord		= RW_PI87RC; 				//��μ̵����� 
	Rel87R_C.LockRelayWord		= RW_LKPI87RC;					//��α����̵����� 
	Rel87R_C.RSTRelayWord		= RW_LKRST87RC;					//���ر����̵����� 
	Rel87R_C.LoopCtrlRelayWord	= RW_LPC87RC;				//����ѭ�����Ƽ̵����� 

	Rel87R_C.RptSerialNum		= &RptSerialNumOfAct;					//ϵͳ�������к� 
	Rel87R_C.pSysTimer			= &dwRelayMSCnt;; 					//ϵͳ��ʱ�� 
	Rel87R_C.wRelRptNum			= R_87R; 				//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel87R_C.wPickUpReportNum	= RPT_NULL;						//��α������к�
	Rel87R_C.wResetReportNum	= RPT_NULL;						//���ر������к�
	//**********************************************************************	

	Rel87R_C.wRelFCounter		= 0;					//���ϼ����� 
	Rel87R_C.wRelRCounter		= 0;					//���ؼ�����	
//	Rel87R_C.dwRelTimer			= 0; 					//������ʱ��		
	Rel87R_C.wExcepWordNum		= 7;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 
	Rel87R_C.Info				= 0;							//������Ϣ 
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�����1��                                                                 
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_H1()
{
	Rel50I_H1.pElecSet			= RS_pn50IH1C;          						//��������������ֵ
	Rel50I_H1.pTimeSet			= RS_pn50IH1D;				 			//��������ʱ�䶨ֵ
	Rel50I_H1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_H1.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_H1.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_H1.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_H1.pRelCFG			= CFG_pby50I_H1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_H1.pRelSW			= SW_pby50I_H;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_H1.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H1.pRelPDCFG			= CFG_pby50I_H1_PD;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H1.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H1.pRelFDCFG			= &RelCFGSET;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_H1.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_H1.pRel47UCFG		= CFG_pby50I_H1_47U;			//��ѹ��������
	Rel50I_H1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_H1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_H1.pRelElec 		    = &IHmax;									//��ǰ����ָ��		
	Rel50I_H1.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_H1.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_H1.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_H1.StartRelayWord	= RW_PI50IH1;						//����������μ̵�����
	Rel50I_H1.ActRelayWord		= RW_TR50IH1;							//�������������̵�����
	Rel50I_H1.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_H1.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_H1.PForwardRelayWord	= HPFRW;							//���ʷ���̵�����
	Rel50I_H1.PRewardRelayWord  = HPRRW;
	Rel50I_H1.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_H1.R47ULKRelayWord	= RW_47UH;
	Rel50I_H1.LockRelayWord		= RW_LKPI50IH1;						//��α����̵�����
	Rel50I_H1.RSTRelayWord		= RW_LKRST50IH1;							//���ر����̵�����
	Rel50I_H1.LoopCtrlRelayWord	= RW_LPC50IH1;						//����ѭ�����Ƽ̵�����
	Rel50I_H1.wReportNum		= R_50I_H1;							//���������������
	Rel50I_H1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_H1.dwK35				= NULL;
	Rel50I_H1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_H1.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_H1.wPickUpReportNum	= R_50I_H1_PI;						//��α������к�
	Rel50I_H1.wResetReportNum	= R_50I_H1_RES;						//���ر������к�
	//**********************************************************************	

	//Rel50I_H1.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_H1.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_H1.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_H1.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�����2��                                                                 
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_H2()
{
	Rel50I_H2.pElecSet			= RS_pn50IH2C;          						//��������������ֵ
	Rel50I_H2.pTimeSet			= RS_pn50IH2D;				 			//��������ʱ�䶨ֵ
	Rel50I_H2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_H2.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_H2.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_H2.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_H2.pRelCFG			= CFG_pby50I_H2;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_H2.pRelSW			= SW_pby50I_H;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_H2.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H2.pRelPDCFG			= CFG_pby50I_H2_PD;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H2.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H2.pRelFDCFG			= &RelCFGSET;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_H2.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_H2.pRel47UCFG		= CFG_pby50I_H2_47U;			//��ѹ��������
	Rel50I_H2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_H2.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_H2.pRelElec 		    = &IHmax;									//��ǰ����ָ��		
	Rel50I_H2.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_H2.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_H2.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_H2.StartRelayWord	= RW_PI50IH2;						//����������μ̵�����
	Rel50I_H2.ActRelayWord		= RW_TR50IH2;							//�������������̵�����
	Rel50I_H2.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_H2.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_H2.PForwardRelayWord	= HPFRW;							//���ʷ���̵�����
	Rel50I_H2.PRewardRelayWord  = HPRRW;
	Rel50I_H2.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_H2.R47ULKRelayWord	= RW_47UH;
	Rel50I_H2.LockRelayWord		= RW_LKPI50IH2;						//��α����̵�����
	Rel50I_H2.RSTRelayWord		= RW_LKRST50IH2;							//���ر����̵�����
	Rel50I_H2.LoopCtrlRelayWord	= RW_LPC50IH2;						//����ѭ�����Ƽ̵�����
	Rel50I_H2.wReportNum		= R_50I_H2;							//���������������
	Rel50I_H2.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_H2.dwK35				= NULL;
	Rel50I_H2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_H2.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_H2.wPickUpReportNum	= R_50I_H2_PI;						//��α������к�
	Rel50I_H2.wResetReportNum	= R_50I_H2_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_H2.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_H2.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_H2.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_H2.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�����3��                                                                 
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_H3()
{
	Rel50I_H3.pElecSet			= RS_pn50IH3C;          						//��������������ֵ
	Rel50I_H3.pTimeSet			= RS_pn50IH3D;				 			//��������ʱ�䶨ֵ
	Rel50I_H3.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_H3.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_H3.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_H3.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_H3.pRelCFG			= CFG_pby50I_H3;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_H3.pRelSW			= SW_pby50I_H;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_H3.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H3.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H3.pRelPDCFG			= CFG_pby50I_H3_PD;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H3.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_H3.pRelFDCFG			= &RelCFGSET;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_H3.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_H3.pRel47UCFG		= CFG_pby50I_H3_47U;			//��ѹ��������
	Rel50I_H3.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_H3.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_H3.pRelElec 		    = &IHmax;									//��ǰ����ָ��		
	Rel50I_H3.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_H3.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_H3.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_H3.StartRelayWord	= RW_PI50IH3;						//����������μ̵�����
	Rel50I_H3.ActRelayWord		= RW_TR50IH3;							//�������������̵�����
	Rel50I_H3.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_H3.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_H3.PForwardRelayWord	= HPFRW;							//���ʷ���̵�����
	Rel50I_H3.PRewardRelayWord  = HPRRW;
	Rel50I_H3.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_H3.R47ULKRelayWord	= RW_47UH;
	Rel50I_H3.LockRelayWord		= RW_LKPI50IH3;						//��α����̵�����
	Rel50I_H3.RSTRelayWord		= RW_LKRST50IH3;							//���ر����̵�����
	Rel50I_H3.LoopCtrlRelayWord	= RW_LPC50IH3;						//����ѭ�����Ƽ̵�����
	Rel50I_H3.wReportNum		= R_50I_H3;							//���������������
	Rel50I_H3.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_H3.dwK35				= NULL;
	Rel50I_H3.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_H3.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_H3.wPickUpReportNum	= R_50I_H3_PI;						//��α������к�
	Rel50I_H3.wResetReportNum	= R_50I_H3_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_H3.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_H3.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_H3.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_H3.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�����1��                                                                 
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_L1()
{
	Rel50I_L1.pElecSet			= RS_pn50IL1C;          						//��������������ֵ
	Rel50I_L1.pTimeSet			= RS_pn50IL1D;				 			//��������ʱ�䶨ֵ
	Rel50I_L1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_L1.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_L1.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_L1.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_L1.pRelCFG			= CFG_pby50I_L1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_L1.pRelSW			= SW_pby50I_L;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_L1.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L1.pRelPDCFG			= CFG_pby50I_L1_PD;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L1.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L1.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_L1.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_L1.pRel47UCFG		= CFG_pby50I_L1_47U;			//��ѹ��������
	Rel50I_L1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_L1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_L1.pRelElec 		    = &ILmax;									//��ǰ����ָ��		
	Rel50I_L1.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_L1.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_L1.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_L1.StartRelayWord	= RW_PI50IL1;						//����������μ̵�����
	Rel50I_L1.ActRelayWord		= RW_TR50IL1;							//�������������̵�����
	Rel50I_L1.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_L1.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_L1.PForwardRelayWord	= LPFRW;							//���ʷ���̵�����
	Rel50I_L1.PRewardRelayWord  = LPRRW;
	Rel50I_L1.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_L1.R47ULKRelayWord	= RW_47UL;
	Rel50I_L1.LockRelayWord		= RW_LKPI50IL1;						//��α����̵�����
	Rel50I_L1.RSTRelayWord		= RW_LKRST50IL1;							//���ر����̵�����
	Rel50I_L1.LoopCtrlRelayWord	= RW_LPC50IL1;						//����ѭ�����Ƽ̵�����
	Rel50I_L1.wReportNum		= R_50I_L1;							//���������������
	Rel50I_L1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_L1.dwK35				= NULL;
	Rel50I_L1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_L1.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_L1.wPickUpReportNum	= R_50I_L1_PI;						//��α������к�
	Rel50I_L1.wResetReportNum	= R_50I_L1_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_L1.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_L1.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_L1.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_L1.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�����2��                                                                 
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_L2()
{
	Rel50I_L2.pElecSet			= RS_pn50IL2C;          						//��������������ֵ
	Rel50I_L2.pTimeSet			= RS_pn50IL2D;				 			//��������ʱ�䶨ֵ
	Rel50I_L2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_L2.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_L2.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_L2.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_L2.pRelCFG			= CFG_pby50I_L2;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_L2.pRelSW			= SW_pby50I_L;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_L2.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L2.pRelPDCFG			= CFG_pby50I_L2_PD;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L2.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_L2.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_L2.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_L2.pRel47UCFG		= CFG_pby50I_L2_47U;			//��ѹ��������
	Rel50I_L2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_L2.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_L2.pRelElec 		    = &ILmax;									//��ǰ����ָ��		
	Rel50I_L2.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_L2.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_L2.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_L2.StartRelayWord	= RW_PI50IL2;						//����������μ̵�����
	Rel50I_L2.ActRelayWord		= RW_TR50IL2;							//�������������̵�����
	Rel50I_L2.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_L2.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_L2.PForwardRelayWord	= LPFRW;							//���ʷ���̵�����
	Rel50I_L2.PRewardRelayWord  = LPRRW;
	Rel50I_L2.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_L2.R47ULKRelayWord	= RW_47UL;
	Rel50I_L2.LockRelayWord		= RW_LKPI50IL2;						//��α����̵�����
	Rel50I_L2.RSTRelayWord		= RW_LKRST50IL2;							//���ر����̵�����
	Rel50I_L2.LoopCtrlRelayWord	= RW_LPC50IL2;						//����ѭ�����Ƽ̵�����
	Rel50I_L2.wReportNum		= R_50I_L2;							//���������������
	Rel50I_L2.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_L2.dwK35				= NULL;
	Rel50I_L2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_L2.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_L2.wPickUpReportNum	= R_50I_L2_PI;						//��α������к�
	Rel50I_L2.wResetReportNum	= R_50I_L2_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_L2.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_L2.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_L2.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_L2.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ��ĸ�߳�籣��                                                          
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_SH()
{
	Rel50I_SH.pElecSet			= RS_pn50ISHC;          						//��������������ֵ
	Rel50I_SH.pTimeSet			= RS_pn50ISHD;				 			//��������ʱ�䶨ֵ
	Rel50I_SH.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_SH.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_SH.pAccActTimeSet	= RS_pn50ISHD;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_SH.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_SH.pRelCFG			= CFG_pby50I_SH;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_SH.pRelSW			= SW_pby50I_SH;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_SH.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_SH.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_SH.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_SH.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_SH.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_SH.pRelAccCFG		= CFG_pby50I_SH;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_SH.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50I_SH.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_SH.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_SH.pRelElec 		    = &ILmax;									//��ǰ����ָ��		
	Rel50I_SH.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_SH.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_SH.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_SH.StartRelayWord	= RW_PI50ISH;						//����������μ̵�����
	Rel50I_SH.ActRelayWord		= RW_TR50ISH;							//�������������̵�����
	Rel50I_SH.AccRelayWord		= RW_TSH;									//���������̵�����
	Rel50I_SH.AccActRelayWord	= RW_TR50ISH;									//����ٶ����̵�����
	Rel50I_SH.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50I_SH.PRewardRelayWord  = FALSE1;
	Rel50I_SH.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_SH.R47ULKRelayWord	= FALSE1;
	Rel50I_SH.LockRelayWord		= RW_LKPI50ISH;						//��α����̵�����
	Rel50I_SH.RSTRelayWord		= RW_LKRST50ISH;							//���ر����̵�����
	Rel50I_SH.LoopCtrlRelayWord	= RW_LPC50ISH;						//����ѭ�����Ƽ̵�����
	Rel50I_SH.wReportNum		= R_50I_SH;							//���������������
	Rel50I_SH.wAccReportNum		= R_50I_SH;								//���ٶ����������

	Rel50I_SH.dwK35				= NULL;
	Rel50I_SH.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_SH.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_SH.wPickUpReportNum	= R_50I_SH_PI;						//��α������к�
	Rel50I_SH.wResetReportNum	= R_50I_SH_RES;						//���ر������к�	
	Rel50I_SH.wAccPickUpRptNum	= R_50I_SH_PI;						//��α������к�
	Rel50I_SH.wAccResetRptNum	= R_50I_SH_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_SH.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_SH.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_SH.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_SH.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���������1��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_N1()
{
	Rel50I_N1.pElecSet			= RS_pn50IN1C;          						//��������������ֵ
	Rel50I_N1.pTimeSet			= RS_pn50IN1D;				 			//��������ʱ�䶨ֵ
	Rel50I_N1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_N1.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_N1.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_N1.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_N1.pRelCFG			= CFG_pby50I_N1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_N1.pRelSW			= SW_pby50N;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_N1.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N1.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N1.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N1.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_N1.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_N1.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50I_N1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_N1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_N1.pRelElec 		    = &I0;									//��ǰ����ָ��		
	Rel50I_N1.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_N1.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_N1.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_N1.StartRelayWord	= RW_PI50N1;						//����������μ̵�����
	Rel50I_N1.ActRelayWord		= RW_TR50N1;							//�������������̵�����
	Rel50I_N1.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_N1.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_N1.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50I_N1.PRewardRelayWord  = FALSE1;
	Rel50I_N1.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_N1.R47ULKRelayWord	= FALSE1;
	Rel50I_N1.LockRelayWord		= RW_LKPI50N1;						//��α����̵�����
	Rel50I_N1.RSTRelayWord		= RW_LKRST50N1;							//���ر����̵�����
	Rel50I_N1.LoopCtrlRelayWord	= RW_LPC50N1;						//����ѭ�����Ƽ̵�����
	Rel50I_N1.wReportNum		= R_50I_N1;							//���������������
	Rel50I_N1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_N1.dwK35				= NULL;
	Rel50I_N1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_N1.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_N1.wPickUpReportNum	= R_50I_N1_PI;						//��α������к�
	Rel50I_N1.wResetReportNum	= R_50I_N1_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_N1.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_N1.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_N1.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_N1.wExcepWordNum     	= 8;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���������2��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_N2()
{
	Rel50I_N2.pElecSet			= RS_pn50IN2C;          						//��������������ֵ
	Rel50I_N2.pTimeSet			= RS_pn50IN2D;				 			//��������ʱ�䶨ֵ
	Rel50I_N2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_N2.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_N2.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_N2.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_N2.pRelCFG			= CFG_pby50I_N2;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_N2.pRelSW			= SW_pby50N;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_N2.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N2.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N2.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_N2.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_N2.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_N2.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50I_N2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_N2.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_N2.pRelElec 		    = &I0;									//��ǰ����ָ��		
	Rel50I_N2.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_N2.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_N2.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_N2.StartRelayWord	= RW_PI50N2;						//����������μ̵�����
	Rel50I_N2.ActRelayWord		= RW_TR50N2;							//�������������̵�����
	Rel50I_N2.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_N2.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_N2.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50I_N2.PRewardRelayWord  = FALSE1;
	Rel50I_N2.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_N2.R47ULKRelayWord	= FALSE1;
	Rel50I_N2.LockRelayWord		= RW_LKPI50N2;						//��α����̵�����
	Rel50I_N2.RSTRelayWord		= RW_LKRST50N2;							//���ر����̵�����
	Rel50I_N2.LoopCtrlRelayWord	= RW_LPC50N2;						//����ѭ�����Ƽ̵�����
	Rel50I_N2.wReportNum		= R_50I_N2;							//���������������
	Rel50I_N2.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_N2.dwK35				= NULL;
	Rel50I_N2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_N2.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_N2.wPickUpReportNum	= R_50I_N2_PI;						//��α������к�
	Rel50I_N2.wResetReportNum	= R_50I_N2_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_N2.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_N2.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_N2.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_N2.wExcepWordNum     = 8;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�������ѹ                                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel59U_NH()
{
	Rel59U_NH.pElecSet			= RS_pn59UNHV;          						//��������������ֵ
	Rel59U_NH.pTimeSet			= RS_pn59UNHD;				 			//��������ʱ�䶨ֵ
	Rel59U_NH.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel59U_NH.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel59U_NH.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel59U_NH.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel59U_NH.pRelCFG			= CFG_pby59U_NH;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel59U_NH.pRelSW			= SW_pby59N;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel59U_NH.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NH.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NH.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NH.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NH.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel59U_NH.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel59U_NH.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel59U_NH.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel59U_NH.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel59U_NH.pRelElec 		    = &U0h;									//��ǰ����ָ��		
	Rel59U_NH.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel59U_NH.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel59U_NH.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel59U_NH.StartRelayWord	= RW_PI59NH;						//����������μ̵�����
	Rel59U_NH.ActRelayWord		= RW_TR59NH;							//�������������̵�����
	Rel59U_NH.AccRelayWord		= FALSE1;									//���������̵�����
	Rel59U_NH.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel59U_NH.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel59U_NH.PRewardRelayWord  = FALSE1;
	Rel59U_NH.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel59U_NH.R47ULKRelayWord	= FALSE1;
	Rel59U_NH.LockRelayWord		= RW_LKPI59NH;						//��α����̵�����
	Rel59U_NH.RSTRelayWord		= RW_LKRST59NH;							//���ر����̵�����
	Rel59U_NH.LoopCtrlRelayWord	= RW_LPC59NH;						//����ѭ�����Ƽ̵�����
	Rel59U_NH.wReportNum		= R_59U_H;							//���������������
	Rel59U_NH.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel59U_NH.dwK35				= NULL;
	Rel59U_NH.RptSerialNum		= &RptSerialNumOfAct;						
    Rel59U_NH.bMakeActRpt       = TRUE;
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel59U_NH.wPickUpReportNum	= R_59U_H_PI;						//��α������к�
	Rel59U_NH.wResetReportNum	= R_59U_H_RES;						//���ر������к�
	//**********************************************************************	

//	Rel59U_NH.dwRelTimer		= 0;			 			//��ʱ��
	Rel59U_NH.byRelFCounter		= 0;						//���ϼ�����
	Rel59U_NH.byRelRCounter		= 0;						//���ؼ�����		
	Rel59U_NH.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����ѹ�������ѹ                                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel59U_NL()
{
	Rel59U_NL.pElecSet			= RS_pn59UNLV;          						//��������������ֵ
	Rel59U_NL.pTimeSet			= RS_pn59UNLD;				 			//��������ʱ�䶨ֵ
	Rel59U_NL.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel59U_NL.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel59U_NL.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel59U_NL.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel59U_NL.pRelCFG			= CFG_pby59U_NL;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel59U_NL.pRelSW			= SW_pby59N;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel59U_NL.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NL.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NL.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NL.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U_NL.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel59U_NL.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel59U_NL.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel59U_NL.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel59U_NL.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel59U_NL.pRelElec 		    = &U0l;									//��ǰ����ָ��		
	Rel59U_NL.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel59U_NL.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel59U_NL.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel59U_NL.StartRelayWord	= RW_PI59NL;						//����������μ̵�����
	Rel59U_NL.ActRelayWord		= RW_TR59NL;							//�������������̵�����
	Rel59U_NL.AccRelayWord		= FALSE1;									//���������̵�����
	Rel59U_NL.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel59U_NL.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel59U_NL.PRewardRelayWord  = FALSE1;
	Rel59U_NL.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel59U_NL.R47ULKRelayWord	= FALSE1;
	Rel59U_NL.LockRelayWord		= RW_LKPI59NL;						//��α����̵�����
	Rel59U_NL.RSTRelayWord		= RW_LKRST59NL;							//���ر����̵�����
	Rel59U_NL.LoopCtrlRelayWord	= RW_LPC59NL;						//����ѭ�����Ƽ̵�����
	Rel59U_NL.wReportNum		= R_59U_L;							//���������������
	Rel59U_NL.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel59U_NL.dwK35				= NULL;
	Rel59U_NL.RptSerialNum		= &RptSerialNumOfAct;						
    Rel59U_NL.bMakeActRpt       = FALSE;   //��װ���У���ѹ����������Ϊ�澯
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel59U_NL.wPickUpReportNum	= 0xFFFF;						//��α������к�
	Rel59U_NL.wResetReportNum	= 0xFFFF;						//���ر������к�
	//**********************************************************************	

//	Rel59U_NL.dwRelTimer		= 0;			 			//��ʱ��
	Rel59U_NL.byRelFCounter		= 0;						//���ϼ�����
	Rel59U_NL.byRelRCounter		= 0;						//���ؼ�����		
	Rel59U_NL.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ��������1��                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50OL1()
{
	Rel50OL_1.pElecSet			= RS_pn50OL1C;          						//��������������ֵ
	Rel50OL_1.pTimeSet			= RS_pn50OL1D;				 			//��������ʱ�䶨ֵ
	Rel50OL_1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50OL_1.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50OL_1.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50OL_1.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50OL_1.pRelCFG			= CFG_pby50OL_1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50OL_1.pRelSW			= SW_pby50OL;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50OL_1.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_1.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_1.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_1.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50OL_1.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50OL_1.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50OL_1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50OL_1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50OL_1.pRelElec 		    = &IHmax;									//��ǰ����ָ��		
	Rel50OL_1.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50OL_1.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50OL_1.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50OL_1.StartRelayWord	= RW_PI50OL1;						//����������μ̵�����
	Rel50OL_1.ActRelayWord		= RW_TR50OL1;							//�������������̵�����
	Rel50OL_1.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50OL_1.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50OL_1.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50OL_1.PRewardRelayWord  = FALSE1;
	Rel50OL_1.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50OL_1.R47ULKRelayWord	= FALSE1;
	Rel50OL_1.LockRelayWord		= RW_LKPI50OL1;						//��α����̵�����
	Rel50OL_1.RSTRelayWord		= RW_LKRST50OL1;							//���ر����̵�����
	Rel50OL_1.LoopCtrlRelayWord	= RW_LPC50OL1;						//����ѭ�����Ƽ̵�����
	Rel50OL_1.wReportNum		= R_50OL_1;							//���������������
	Rel50OL_1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50OL_1.dwK35				= NULL;
	Rel50OL_1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50OL_1.bMakeActRpt       = TRUE;   //��װ���У���ѹ����������Ϊ�澯
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50OL_1.wPickUpReportNum	= R_50OL_1_PI;						//��α������к�
	Rel50OL_1.wResetReportNum	= R_50OL_1_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50OL_1.dwRelTimer		= 0;			 			//��ʱ��
	Rel50OL_1.byRelFCounter		= 0;						//���ϼ�����
	Rel50OL_1.byRelRCounter		= 0;						//���ؼ�����		
	Rel50OL_1.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ��������2��                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50OL2()
{
	Rel50OL_2.pElecSet			= RS_pn50OL2C;          						//��������������ֵ
	Rel50OL_2.pTimeSet			= RS_pn50OL2D;				 			//��������ʱ�䶨ֵ
	Rel50OL_2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50OL_2.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50OL_2.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50OL_2.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50OL_2.pRelCFG			= CFG_pby50OL_2;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50OL_2.pRelSW			= SW_pby50OL;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50OL_2.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_2.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_2.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_2.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50OL_2.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50OL_2.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50OL_2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50OL_2.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50OL_2.pRelElec 		    = &IHmax;									//��ǰ����ָ��		
	Rel50OL_2.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50OL_2.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50OL_2.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50OL_2.StartRelayWord	= RW_PI50OL2;						//����������μ̵�����
	Rel50OL_2.ActRelayWord		= RW_TR50OL2;							//�������������̵�����
	Rel50OL_2.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50OL_2.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50OL_2.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50OL_2.PRewardRelayWord  = FALSE1;
	Rel50OL_2.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50OL_2.R47ULKRelayWord	= FALSE1;
	Rel50OL_2.LockRelayWord		= RW_LKPI50OL2;						//��α����̵�����
	Rel50OL_2.RSTRelayWord		= RW_LKRST50OL2;							//���ر����̵�����
	Rel50OL_2.LoopCtrlRelayWord	= RW_LPC50OL2;						//����ѭ�����Ƽ̵�����
	Rel50OL_2.wReportNum		= R_50OL_2;							//���������������
	Rel50OL_2.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50OL_2.dwK35				= NULL;
	Rel50OL_2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50OL_2.bMakeActRpt       = TRUE;   //��װ���У���ѹ����������Ϊ�澯
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50OL_2.wPickUpReportNum	= R_50OL_2_PI;						//��α������к�
	Rel50OL_2.wResetReportNum	= R_50OL_2_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50OL_2.dwRelTimer		= 0;			 			//��ʱ��
	Rel50OL_2.byRelFCounter		= 0;						//���ϼ�����
	Rel50OL_2.byRelRCounter		= 0;						//���ؼ�����		
	Rel50OL_2.wExcepWordNum     	= 8;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ��������3��                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50OL3()
{
	Rel50OL_3.pElecSet			= RS_pn50OL3C;          						//��������������ֵ
	Rel50OL_3.pTimeSet			= RS_pn50OL3D;				 			//��������ʱ�䶨ֵ
	Rel50OL_3.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50OL_3.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50OL_3.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50OL_3.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50OL_3.pRelCFG			= CFG_pby50OL_3;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50OL_3.pRelSW			= SW_pby50OL;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50OL_3.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_3.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_3.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_3.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50OL_3.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50OL_3.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50OL_3.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50OL_3.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50OL_3.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50OL_3.pRelElec 		    = &IHmax;									//��ǰ����ָ��		
	Rel50OL_3.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50OL_3.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50OL_3.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50OL_3.StartRelayWord	= RW_PI50OL3;						//����������μ̵�����
	Rel50OL_3.ActRelayWord		= RW_TR50OL3;							//�������������̵�����
	Rel50OL_3.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50OL_3.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50OL_3.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50OL_3.PRewardRelayWord  = FALSE1;
	Rel50OL_3.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50OL_3.R47ULKRelayWord	= FALSE1;
	Rel50OL_3.LockRelayWord		= RW_LKPI50OL3;						//��α����̵�����
	Rel50OL_3.RSTRelayWord		= RW_LKRST50OL3;							//���ر����̵�����
	Rel50OL_3.LoopCtrlRelayWord	= RW_LPC50OL3;						//����ѭ�����Ƽ̵�����
	Rel50OL_3.wReportNum		= R_50OL_3;							//���������������
	Rel50OL_3.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50OL_3.dwK35				= NULL;
	Rel50OL_3.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50OL_3.bMakeActRpt       = TRUE;   //��װ���У���ѹ����������Ϊ�澯
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50OL_3.wPickUpReportNum	= R_50OL_3_PI;						//��α������к�
	Rel50OL_3.wResetReportNum	= R_50OL_3_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50OL_3.dwRelTimer		= 0;			 			//��ʱ��
	Rel50OL_3.byRelFCounter		= 0;						//���ϼ�����
	Rel50OL_3.byRelRCounter		= 0;						//���ؼ�����		
	Rel50OL_3.wExcepWordNum     	= 8;
}
//�����ɸ澯��yanzh 2014��8��27�� 11:32:32����
void CRelayTask::InitRel50OLA(void)		//�����ɸ澯
{
		Rel50OL_A.pElecSet			= RS_pn50OLAC;									//��������������ֵ
		Rel50OL_A.pTimeSet			= RS_pn50OLAD;							//��������ʱ�䶨ֵ
		Rel50OL_A.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
		Rel50OL_A.pVolLockSet		= NULL; 							//����������ѹ������ֵ
		Rel50OL_A.pAccActTimeSet	= NULL; 								//������������ٶ���ʱ�䶨ֵ
		Rel50OL_A.pH2Coef			= NULL; 							//����г������ϵ����г����Ȩϵ���ķŴ���
		
		Rel50OL_A.pRelCFG			= CFG_pby50OL_A;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
		Rel50OL_A.pRelSW			= SW_pby50OL;							//����������ѹ��------1:Ͷ�룬2:�˳�
		Rel50OL_A.pRelHLockCFG		= &RelCFGCLR;						//г���������ܿ���λ--1:Ͷ�룬2:�˳�
		Rel50OL_A.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
		Rel50OL_A.pRelPDCFG 		= &RelCFGCLR;							//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
		Rel50OL_A.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
		Rel50OL_A.pRelFDCFG 		= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
		Rel50OL_A.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
		Rel50OL_A.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
		Rel50OL_A.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
		Rel50OL_A.pSysTimer 		= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
		
		Rel50OL_A.pRelElec			= &IHmax;									//��ǰ����ָ��		
		Rel50OL_A.pRelHarm3 		= NULL; 								//��ǰ����г��ָ��	
		Rel50OL_A.pRelHarm5 		= NULL; 								//��ǰ���г��ָ��	
		Rel50OL_A.pRelVolt			= NULL; 								//��ǰ���������ĵ�ѹ��ָ��
	
		Rel50OL_A.StartRelayWord	= RW_PI50OLA;						//����������μ̵�����
		Rel50OL_A.ActRelayWord		= RW_TR50OLA;							//�������������̵�����
		Rel50OL_A.AccRelayWord		= FALSE1;									//���������̵�����
		Rel50OL_A.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
		Rel50OL_A.PForwardRelayWord = FALSE1;							//���ʷ���̵�����
		Rel50OL_A.PRewardRelayWord	= FALSE1;
		Rel50OL_A.H2LockRelayWord	= FALSE1;							//����г�������̵�����
		Rel50OL_A.R47ULKRelayWord	= FALSE1;
		Rel50OL_A.LockRelayWord 	= RW_LKPI50OLA; 					//��α����̵�����
		Rel50OL_A.RSTRelayWord		= RW_LKRST50OLA;							//���ر����̵�����
		Rel50OL_A.LoopCtrlRelayWord = RW_LPC50OLA;						//����ѭ�����Ƽ̵�����
		Rel50OL_A.wReportNum		= R_50OL_A; 						//���������������
		Rel50OL_A.wAccReportNum 	= 0xFFFF;								//���ٶ����������
	
		Rel50OL_A.dwK35 			= NULL;
		Rel50OL_A.RptSerialNum		= &RptSerialNumOfAct;						
		Rel50OL_A.bMakeActRpt		= FALSE;   //������Ϊ�澯
		//**********************��η��ر��沿��*************	yanxs  13-03-18
		Rel50OL_A.wPickUpReportNum	= R_50OL_A_PI;						//��α������к�
		Rel50OL_A.wResetReportNum	= R_50OL_A_RES; 					//���ر������к�
		//**********************************************************************	
	
	//	Rel50OL_A.dwRelTimer		= 0;						//��ʱ��
		Rel50OL_A.byRelFCounter 	= 0;						//���ϼ�����
		Rel50OL_A.byRelRCounter 	= 0;						//���ؼ�����		
		Rel50OL_A.wExcepWordNum 		= 8;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����϶����                                               
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50IJX()
{
	Rel50I_JX.pElec1Set			= RS_pn50I_JXC;          						//��������������ֵ	
	Rel50I_JX.pElec2Set			= RS_pn59UNHV;          						//��������������ֵ
	Rel50I_JX.pTimeSet			= RS_pn50I_JXD;				 			//��������ʱ�䶨ֵ
	Rel50I_JX.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_JX.pVolLockSet		= NULL;				 	    		//����������ѹ������ֵ
	Rel50I_JX.pAccActTimeSet	= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I_JX.pH2Coef			= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_JX.pRelCFG			= CFG_pby50I_JX;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_JX.pRelSW			= SW_pby50I_JX;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelHLockCFG		= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelVolLockCFG	= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelFDCFG			= &RelCFGCLR;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I_JX.pRelAccCFG		= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I_JX.pRel47UCFG		= &RelCFGCLR;			//��ѹ��������
	Rel50I_JX.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_JX.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I_JX.pRelElec1 		= &Ijx;									//��ǰ����ָ��
	Rel50I_JX.pRelElec2 		= &U0h;
	Rel50I_JX.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	Rel50I_JX.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	Rel50I_JX.pRelVolt			= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_JX.StartRelayWord	= RW_PI50IJX;						//����������μ̵�����
	Rel50I_JX.ActRelayWord		= RW_TR50IJX;							//�������������̵�����
	Rel50I_JX.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I_JX.AccActRelayWord	= FALSE1;									//����ٶ����̵�����
	Rel50I_JX.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	Rel50I_JX.PRewardRelayWord  = FALSE1;
	Rel50I_JX.H2LockRelayWord	= FALSE1;							//����г�������̵�����
	Rel50I_JX.R47ULKRelayWord	= FALSE1;
	Rel50I_JX.LockRelayWord		= RW_LKPI50IJX;						//��α����̵�����
	Rel50I_JX.RSTRelayWord		= RW_LKRST50IJX;							//���ر����̵�����
	Rel50I_JX.LoopCtrlRelayWord	= RW_LPC50IJX;						//����ѭ�����Ƽ̵�����
	Rel50I_JX.wReportNum		= R_50I_JX;							//���������������
	Rel50I_JX.wAccReportNum		= 0xFFFF;								//���ٶ����������

	Rel50I_JX.dwK35				= NULL;
	Rel50I_JX.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_JX.bMakeActRpt       = TRUE;   //��װ���У���ѹ����������Ϊ�澯
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	Rel50I_JX.wPickUpReportNum	= R_50I_JX_PI;						//��α������к�
	Rel50I_JX.wResetReportNum	= R_50I_JX_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_JX.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_JX.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_JX.byRelRCounter		= 0;						//���ؼ�����		
	Rel50I_JX.wExcepWordNum     = 8;
}
/************************************************************************************************
*����: ��ʼ����ѹ����Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRel27U()
{
	Rel27U.pELeSet						= RS_pn27V;						//������ֵ 
	Rel27U.pTimeSet						= RS_pn27D;						//ʱ�䶨ֵ 
	Rel27U.pLockCurSet					= RS_pn27U_50I;					//����������ֵ 

	Rel27U.pRelCFG						= CFG_pby27U;					  	 //Ƿ���������� 
	Rel27U.pRelSW						= SW_pby27U;					  	 //Ƿ��������ѹ�� 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//Ƿ������Ͷ�澯
	Rel27U.pRelCurLockCFG				= CFG_pby27U_50I;
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ�� 

	Rel27U.pRelElec						= &U_27U;						//��ǰ�ĵ���ָ�� 
	Rel27U.pRelLockElec					= &IHmax;					//�������� 
	Rel27U.ActRelayWord					= RW_TR27U;					//Ƿ�����������̵����� 
	Rel27U.StartRelayWord				= RW_PI27U;					//Ƿ��������μ̵����� 
	Rel27U.AlarmRelayWord				= FALSE1;					//Ƿ�������澯�̵����� 
	Rel27U.LockRelayWord				= RW_LKPI27U;				//��α����̵����� 
	Rel27U.RSTRelayWord					= RW_LKRST27U;					//���Ϸ��ر����̵����� 
	Rel27U.LoopCtrlRelayWord			= FALSE1;				//����ѭ�����Ƽ̵����� 
	Rel27U.wR52BFRealyword				= RW_52BFA;
	Rel27U.wActRptNum					= R_27U;					//Ƿ����������������� 
	Rel27U.wAlarmRptNum					= 0xFFFF;					//Ƿ�������澯������� 
	Rel27U.wTRFailRptNum				= R_52BF;
	Rel27U.wPickUpRptNum				= R_PICKUP;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
	
	Rel27U.R27RptSerialNum				= 0;
//	Rel27U.dwRelTimer					= 0;						//Ƿ��������ʱ�� 	
	Rel27U.byRelFCounter				= 0;					//���ϼ����� 
	Rel27U.byRelRCounter				= 0;					//���ؼ����� 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
void CRelayTask::InitRel49I(void)
{
	
	Rel49IA.p49KSet					= RS_pn49K;
	Rel49IA.p49TSet					= RS_pn49T;
	Rel49IA.p49ARSet				= RS_pn49AR;
	Rel49IA.p49KTSet				= RS_pn49KT;
	Rel49IA.pRatedInSet				= RS_pnRAC;
	Rel49IA.pRel49CFG				= CFG_pby49I;
	Rel49IA.pRel49SW				= SW_pby49I;
	Rel49IA.pICoef					= CF_pnIp;
	Rel49IA.pIHACoef				= CF_pnIp;
	Rel49IA.pCurrent				= dw49IA;	
	Rel49IA.dwKIn					= 0;	
	Rel49IA.PreCurrent				= 0;			
	Rel49IA.l49OT					= 0;
	Rel49IA.l49KOT					= 0;
	Rel49IA.lExpT					= 0;
	Rel49IA.lExpKT					= 0;
	Rel49IA.R49WarnRW				= RW_AR49IA;
	Rel49IA.R49ActRW				= RW_TR49IA;
	Rel49IA.R49ActRptNum			= R_49I;
	Rel49IA.R49WarnRptNum			= R_49IAR;
	Rel49IA.R49RSTRptNum			= R_49I_RES;	
	Rel49IA.dw49Warn				= 0;
	Rel49IA.dw49Warn_099        	= 0;
	Rel49IA.dw49AThermaVal			= 0;
	Rel49IA.dw49AThermaPreVal		= 0;	
	Rel49IA.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
//	Rel49IA.dwRelTimer				= 0;							//Ƿ��������ʱ��	
	Rel49IA.wExcepWordNum			= 16;
	InitThermalOverLoad(&Rel49IA);

	Rel49IB.p49KSet					= RS_pn49K;
	Rel49IB.p49TSet					= RS_pn49T;
	Rel49IB.p49ARSet				= RS_pn49AR;
	Rel49IB.p49KTSet				= RS_pn49KT;
	Rel49IB.pRatedInSet				= RS_pnRAC;
	Rel49IB.pRel49CFG 				= CFG_pby49I;
	Rel49IB.pRel49SW				= SW_pby49I;
	Rel49IB.pICoef					= CF_pnIp;
	Rel49IB.pIHACoef				= CF_pnIp;
	Rel49IB.pCurrent				= dw49IB;
	Rel49IB.dwKIn 					= 0;	
	Rel49IB.PreCurrent				= 0;			
	Rel49IB.l49OT 					= 0;
	Rel49IB.l49KOT					= 0;
	Rel49IB.lExpT 					= 0;
	Rel49IB.lExpKT					= 0;
	Rel49IB.R49WarnRW 				= RW_AR49IB;
	Rel49IB.R49ActRW				= RW_TR49IB;
	Rel49IB.R49ActRptNum			= R_49I;
	Rel49IB.R49WarnRptNum 			= R_49IAR;
	Rel49IB.R49RSTRptNum			= R_49I_RES;
	Rel49IB.dw49Warn				= 0;
	Rel49IB.dw49Warn_099			= 0;
	Rel49IB.dw49AThermaVal			= 0;
	Rel49IB.dw49AThermaPreVal 		= 0;	
	Rel49IB.pSysTimer 				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
//	Rel49IB.dwRelTimer				= 0;								//Ƿ��������ʱ�� 
	Rel49IB.wExcepWordNum 			= 16;
	InitThermalOverLoad(&Rel49IB);

	Rel49IC.p49KSet					= RS_pn49K;
	Rel49IC.p49TSet					= RS_pn49T;
	Rel49IC.p49ARSet				= RS_pn49AR;
	Rel49IC.p49KTSet				= RS_pn49KT;
	Rel49IC.pRatedInSet				= RS_pnRAC;
	Rel49IC.pRel49CFG 				= CFG_pby49I;
	Rel49IC.pRel49SW				= SW_pby49I;
	Rel49IC.pICoef					= CF_pnIp;
	Rel49IC.pIHACoef				= CF_pnIp;
	Rel49IC.pCurrent				= dw49IC;		
	Rel49IC.dwKIn 					= 0;	
	Rel49IC.PreCurrent				= 0;			
	Rel49IC.l49OT 					= 0;
	Rel49IC.l49KOT					= 0;
	Rel49IC.lExpT 					= 0;
	Rel49IC.lExpKT					= 0;
	Rel49IC.R49WarnRW 				= RW_AR49IC;
	Rel49IC.R49ActRW				= RW_TR49IC;
	Rel49IC.R49ActRptNum			= R_49I;
	Rel49IC.R49WarnRptNum 			= R_49IAR;
	Rel49IC.R49RSTRptNum			= R_49I_RES;
	Rel49IC.dw49Warn				= 0;
	Rel49IC.dw49Warn_099			= 0;
	Rel49IC.dw49AThermaVal			= 0;
	Rel49IC.dw49AThermaPreVal 		= 0;	
	Rel49IC.pSysTimer 				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
//	Rel49IC.dwRelTimer				= 0;		
	Rel49IC.wExcepWordNum 			= 16;
	InitThermalOverLoad(&Rel49IC);
	
}
void CRelayTask::InitR87IAlarm()
{	
	
	R87Alarm.p87RSW				= SW_pby87R;		//��ѹ��
	R87Alarm.p87R_A_CFG			= CFG_pby87R;		//��������
	R87Alarm.p87R_B_CFG			= CFG_pby87R;		//��������
	R87Alarm.p87R_C_CFG			= CFG_pby87R;		//��������

	R87Alarm.p87RIASet			= RS_pn87RC; 	//A�������ֵ
	R87Alarm.p87RIBSet			= RS_pn87RC; 	//B�������ֵ
	R87Alarm.p87RICSet			= RS_pn87RC; 	//C�������ֵ
	R87Alarm.p87RIKSet			= RS_pnK87AR;		//����Խ�޸澯����

	R87Alarm.pIDA				= &Ida;			//����A
	R87Alarm.pIDB				= &Idb;			//����B
	R87Alarm.pIDC				= &Idc;			//����C

	R87Alarm.AlarmCnt			= R87IALARMTIME;		//����Խ�޸澯ʱ��
	R87Alarm.ARRW				= RW_AR87;			//����Խ�޸澯�̵�����
	R87Alarm.wRelRptNum			= R_87AR;		//����Խ�޸澯�������к�

	R87Alarm.ACnt				= 0;			//A�������
	R87Alarm.BCnt				= 0;			//B�������
	R87Alarm.CCnt				= 0;			//C�������
	R87Alarm.AAlarmFlag			= FALSE;		//A��澯��־
	R87Alarm.BAlarmFlag			= FALSE;		//B��澯��־
	R87Alarm.CAlarmFlag			= FALSE;		//C��澯��־
	R87Alarm.MakeRptFlag		= FALSE;	//���澯�����־
	R87Alarm.wExcepWordNum      = 16;
}

void CRelayTask::ResetIdHLock(void)
{	
	b68IdH2Flag_A = FALSE;	//A��������α�����־	
	b68IdH2Flag_B = FALSE;	//B��������α�����־	
	b68IdH2Flag_C = FALSE;	//C��������α�����־	
	WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
}

//��ʼ������Ԫ���ṹУ������
void CRelayTask::InitStructCheck(void) 	
{	
	WORD acc = 0x55aa;	
	StructCheck[0].pstruct = (WORD*)&Rel87U_A;
	StructCheck[0].bigsize = sizeof(Rel87U_A)/2 - Rel87U_A.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&Rel87U_B;
	StructCheck[1].bigsize = sizeof(Rel87U_B)/2 - Rel87U_B.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel87U_C;
	StructCheck[2].bigsize = sizeof(Rel87U_C)/2 - Rel87U_C.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel87R_A;
	StructCheck[3].bigsize = sizeof(Rel87R_A)/2 - Rel87R_A.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel87R_B;
	StructCheck[4].bigsize = sizeof(Rel87R_B)/2 - Rel87R_B.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel87R_C;
	StructCheck[5].bigsize = sizeof(Rel87R_C)/2 - Rel87R_C.wExcepWordNum;	
	StructCheck[6].pstruct = (WORD*)&Rel50I_H1;
	StructCheck[6].bigsize = sizeof(Rel50I_H1)/2 - Rel50I_H1.wExcepWordNum;		
	StructCheck[7].pstruct = (WORD*)&Rel50I_H2;
	StructCheck[7].bigsize = sizeof(Rel50I_H2)/2 - Rel50I_H2.wExcepWordNum;		
	StructCheck[8].pstruct = (WORD*)&Rel50I_H3;
	StructCheck[8].bigsize = sizeof(Rel50I_H3)/2 - Rel50I_H3.wExcepWordNum;	
	StructCheck[9].pstruct = (WORD*)&Rel50I_L1;
	StructCheck[9].bigsize = sizeof(Rel50I_L1)/2 - Rel50I_L1.wExcepWordNum;	
	StructCheck[10].pstruct = (WORD*)&Rel50I_L2;
	StructCheck[10].bigsize = sizeof(Rel50I_L2)/2 - Rel50I_L2.wExcepWordNum;	
	StructCheck[11].pstruct = (WORD*)&Rel50I_SH;
	StructCheck[11].bigsize = sizeof(Rel50I_SH)/2 - Rel50I_SH.wExcepWordNum;	
	StructCheck[12].pstruct = (WORD*)&Rel50I_N1;
	StructCheck[12].bigsize = sizeof(Rel50I_N1)/2 - Rel50I_N1.wExcepWordNum;	
	StructCheck[13].pstruct = (WORD*)&Rel50I_N2;
	StructCheck[13].bigsize = sizeof(Rel50I_N2)/2 - Rel50I_N2.wExcepWordNum;
	StructCheck[14].pstruct = (WORD*)&Rel59U_NH;
	StructCheck[14].bigsize = sizeof(Rel59U_NH)/2 - Rel59U_NH.wExcepWordNum;	
	StructCheck[15].pstruct = (WORD*)&Rel59U_NL;
	StructCheck[15].bigsize = sizeof(Rel59U_NL)/2 - Rel59U_NL.wExcepWordNum;	
	StructCheck[16].pstruct = (WORD*)&Rel50OL_1;
	StructCheck[16].bigsize = sizeof(Rel50OL_1)/2 - Rel50OL_1.wExcepWordNum;	
	StructCheck[17].pstruct = (WORD*)&Rel50OL_2;
	StructCheck[17].bigsize = sizeof(Rel50OL_2)/2 - Rel50OL_2.wExcepWordNum;	
	StructCheck[18].pstruct = (WORD*)&Rel50OL_3;
	StructCheck[18].bigsize = sizeof(Rel50OL_3)/2 - Rel50OL_3.wExcepWordNum;	
	StructCheck[19].pstruct = (WORD*)&Rel50I_JX;
	StructCheck[19].bigsize = sizeof(Rel50I_JX)/2 - Rel50I_JX.wExcepWordNum;	
	StructCheck[20].pstruct = (WORD*)&Rel27U;
	StructCheck[20].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;	
	StructCheck[21].pstruct = (WORD*)&Rel49IA;
	StructCheck[21].bigsize = sizeof(Rel49IA)/2 - Rel49IA.wExcepWordNum;	
	StructCheck[22].pstruct = (WORD*)&Rel49IB;
	StructCheck[22].bigsize = sizeof(Rel49IB)/2 - Rel49IB.wExcepWordNum;	
	StructCheck[23].pstruct = (WORD*)&Rel49IC;
	StructCheck[23].bigsize = sizeof(Rel49IC)/2 - Rel49IC.wExcepWordNum;		
	StructCheck[24].pstruct = (WORD*)&R87Alarm;
	StructCheck[24].bigsize = sizeof(R87Alarm)/2 - R87Alarm.wExcepWordNum;		
	StructCheck[25].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[25].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
void CRelayTask::R68IdH2M(void)	//����г����������
{
	if(*CFG_pby68IDH2MAXLK == CFG_STATUS_SET)
	{			
		if((Ida.Mod>*RS_pn87RC)&&(*Rel87R_A.p87RCFG == CFG_STATUS_SET))
		{
			if(Ida2.Mod>(*RS_pnkrh2*Ida.Mod/100))
			{
				b68IdH2Flag_A = TRUE;	//A��������α�����־	
				b68IdH2Flag_B = TRUE;	//B��������α�����־	
				b68IdH2Flag_C = TRUE;	//C��������α�����־	
				WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
				return;
			}
		}	
		if((Idb.Mod>*RS_pn87RC)&&(*Rel87R_B.p87RCFG == CFG_STATUS_SET))
		{
			if(Idb2.Mod>(*RS_pnkrh2*Idb.Mod/100))
			{
				b68IdH2Flag_A = TRUE;	//A��������α�����־	
				b68IdH2Flag_B = TRUE;	//B��������α�����־	
				b68IdH2Flag_C = TRUE;	//C��������α�����־	
				WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
				return;
			}
		}	
		if((Idc.Mod>*RS_pn87RC)&&(*Rel87R_B.p87RCFG == CFG_STATUS_SET))
		{
			if(Idc2.Mod>(*RS_pnkrh2*Idc.Mod/100))
			{
				b68IdH2Flag_A = TRUE;	//A��������α�����־	
				b68IdH2Flag_B = TRUE;	//B��������α�����־	
				b68IdH2Flag_C = TRUE;	//C��������α�����־	
				WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
				return;
			}
		}
	}
	
}
void CRelayTask::R68IdH2C(void)		//����г���ۺ������
{
	
	if(*CFG_pby68IDH2MULLK == CFG_STATUS_SET)
	{		
		LONG ID1,ID2;		
		ID1 = MAX(Ida.Mod,Idb.Mod,Idc.Mod);
		ID2 = MAX(Ida2.Mod,Idb2.Mod,Idc2.Mod);
		if(ID2>(*RS_pnkrh2*ID1/100))
		{
			b68IdH2Flag_A = TRUE;	//A��������α�����־	
			b68IdH2Flag_B = TRUE;	//B��������α�����־	
			b68IdH2Flag_C = TRUE;	//C��������α�����־	
			WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
			WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
			WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
			return;
		}
	}
	
}
void CRelayTask::R68IdH2I(void)		//����г���������
{
	if(*CFG_pby68IDH2SPLLK == CFG_STATUS_SET)
	{
		if((Ida.Mod>*RS_pn87RC)&&(Ida2.Mod>(*RS_pnkrh2*Ida.Mod/100)))
		{
			b68IdH2Flag_A = TRUE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
		}		
		else
		{
			b68IdH2Flag_A = FALSE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
		}
		
		if((Idb.Mod>*RS_pn87RC)&&(Idb2.Mod>(*RS_pnkrh2*Idb.Mod/100)))
		{
			b68IdH2Flag_B = TRUE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
		}			
		else
		{
			b68IdH2Flag_B = FALSE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
		}
		
		if((Idc.Mod>*RS_pn87RC)&&(Idc2.Mod>(*RS_pnkrh2*Idc.Mod/100)))
		{
			b68IdH2Flag_C = TRUE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
		}		
		else
		{
			b68IdH2Flag_C = FALSE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
		}
	}
}
void CRelayTask::R68IrH2M(void)		//�ƶ���������г������
{	
	if(*CFG_pby68IRH2LK != CFG_STATUS_SET)
	{		
		b68IrH2Flag_A = FALSE;
		b68IrH2Flag_B = FALSE;
		b68IrH2Flag_C = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF);
		WriteRelayWord(RW_LK68IRCH2,RELAY_OFF);
		return;
	}	
	if((Ida.Mod>*RS_pn87RC)&&(Ira2.Mod>(*RS_pnkrh2*Ira.Mod/100)))
	{
		b68IrH2Flag_A = TRUE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_ON);		
	}	
	else
	{
		b68IrH2Flag_A = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
	}
	
	if((Idb.Mod>*RS_pn87RC)&&(Irb2.Mod>(*RS_pnkrh2*Irb.Mod/100)))
	{
		b68IrH2Flag_B = TRUE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_ON);		
	}	
	else
	{
		b68IrH2Flag_B = FALSE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF); 	
	}
	
	if((Idc.Mod>*RS_pn87RC)&&(Irc2.Mod>(*RS_pnkrh2*Irc.Mod/100)))
	{
		b68IrH2Flag_C = TRUE;
		WriteRelayWord(RW_LK68IRCH2,RELAY_ON);		
	}
	else
	{
		b68IrH2Flag_C = FALSE;
		WriteRelayWord(RW_LK68IRCH2,RELAY_OFF); 	
	}
}
void CRelayTask::R68IdHI(void) 	//�ۺ�г������
{
	
	if(*CFG_pby68IDMULLK != CFG_STATUS_SET)
	{		
		b68IdHMulFlag_A = FALSE;
		b68IdHMulFlag_B = FALSE;
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDAH,RELAY_OFF);	
		WriteRelayWord(RW_LK68IDBH,RELAY_OFF);
		WriteRelayWord(RW_LK68IDCH,RELAY_OFF);
		return;
	}
	
	if((Ida.Mod>*RS_pn87RC)&&((Ida2.Mod+Ida3.Mod)>(*RS_pnkrh*Ida.Mod/100)))
	{
		b68IdHMulFlag_A = TRUE;
		WriteRelayWord(RW_LK68IDAH,RELAY_ON);		
	}	
	else
	{
		b68IdHMulFlag_A = FALSE;
		WriteRelayWord(RW_LK68IDAH,RELAY_OFF);		
	}

	
	if((Idb.Mod>*RS_pn87RC)&&((Idb2.Mod+Idb3.Mod)>(*RS_pnkrh*Idb.Mod/100)))
	{
		b68IdHMulFlag_B = TRUE;
		WriteRelayWord(RW_LK68IDBH,RELAY_ON);		
	}	
	else
	{
		b68IdHMulFlag_B = FALSE;
		WriteRelayWord(RW_LK68IDBH,RELAY_OFF);		
	}

	
	if((Idc.Mod>*RS_pn87RC)&&((Idc2.Mod+Idc3.Mod)>(*RS_pnkrh*Idc.Mod/100)))
	{
		b68IdHMulFlag_C = TRUE;
		WriteRelayWord(RW_LK68IDCH,RELAY_ON);		
	}	
	else
	{
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDCH,RELAY_OFF);		
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
	InitDCRecInfo();	
	InitStructCheck();
//	RelayCheck();
	UpDateCFG();
	UpDateSW();
}
void CRelayTask::OnLogicChanged(void)
{
	m_bLogicChanged = TRUE;
	m_dwWaitLogicCounter = dwRelayMSCnt;
	m_bIsLogicOk &= ResetLogicParseModule(TRUE);
}

void CRelayTask::ThermalOverLoad()
{		
	if((*CFG_pby49I != CFG_STATUS_SET)||(*SW_pby49I != SW_STATUS_SET))
		return;		
	RelThermalOverLoad(&Rel49IA);	//A�����ۼӼ���
	RelThermalOverLoad(&Rel49IB);	//B�����ۼӼ���
	RelThermalOverLoad(&Rel49IC);	//C�����ۼӼ���
	//�ȹ����ɶ�������Ҫ���࣬�����������ʱ��ֻ����A��ı���INF
	if(ReadRelayWord(Rel49IA.R49WarnRW)||ReadRelayWord(Rel49IB.R49WarnRW)||ReadRelayWord(Rel49IC.R49WarnRW))	//�澯�̵�������λ
	{
		if(m_b49IHaveMakeWarnRpt == FALSE)	//��δ���澯����
		{
			MakeAlarmRpt(Rel49IA.R49WarnRptNum);	//���澯����,�ȹ����ɸ澯
			m_b49IHaveMakeWarnRpt = TRUE;		//�������澯�����־
			return;
		}
	}
	if(ReadRelayWord(Rel49IA.R49ActRW)||ReadRelayWord(Rel49IB.R49ActRW)||ReadRelayWord(Rel49IC.R49ActRW))
	{
		if(m_b49IHaveMakeActRpt == FALSE)
		{			
			TAbsTime RptTime;
			WORD	 RptNo;
			ReadAbsTime(&RptTime);
			DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&RptTime,FALSE); //�����ݿ���д����������
			RptNo = g_RptSerialNum;
			MakeActRpt(Rel49IA.R49ActRptNum,RptNo);	
			DB_RelayEnd(RptNo); 			//�����ݿ���д�������
			m_b49IHaveMakeActRpt = TRUE;			
			m_b49IHaveMakeRSTRpt = FALSE;			
		}
		return;  //�����̵�������λ��ֱ���˳�
	}
	if((!ReadRelayWord(Rel49IA.R49WarnRW))&&(!ReadRelayWord(Rel49IB.R49WarnRW))&&(!ReadRelayWord(Rel49IC.R49WarnRW)))
	{		
		if((m_b49IHaveMakeRSTRpt == FALSE)&&(m_b49IHaveMakeWarnRpt == TRUE)) //�޸澯�̵�������λ��
		{
			MakeAlarmRpt(Rel49IA.R49RSTRptNum);
			m_b49IHaveMakeRSTRpt = TRUE;
			m_b49IHaveMakeActRpt = FALSE;
			m_b49IHaveMakeWarnRpt = FALSE;
		}
	}
}
//------------------------------------------------------------------------------------------ 
// ����: ģֵ�����б����							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	
	BYTE  pChannel[]={CHANNEL_6533_IA,CHANNEL_6533_IB,CHANNEL_6533_IC,CHANNEL_6533_Ia,CHANNEL_6533_Ib, \
					 CHANNEL_6533_Ic,CHANNEL_6533_I0,CHANNEL_6533_Ijx,CHANNEL_6533_IDA,CHANNEL_6533_IDB, \
					 CHANNEL_6533_IDC};
	BOOL  relayInputFlag = FALSE;
	if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))  //��·����բʧ�ܣ������б��������жϴ��� 
	{
		WriteRelayWord(PickUpRW, RELAY_OFF);
		return FALSE;
	}
	if((*Rel87U_A.pRelCFG == CFG_STATUS_SET) && (*Rel87U_A.pRelSW == SW_STATUS_SET))   //��ٶ�A
	{
		relayInputFlag = TRUE;
		if(Rel87U_A.pRelElec->Mod > *Rel87U_A.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*Rel87U_B.pRelCFG == CFG_STATUS_SET) && (*Rel87U_B.pRelSW == SW_STATUS_SET)) //��ٶ�B
	{		
		relayInputFlag = TRUE;
		if(Rel87U_B.pRelElec->Mod > *Rel87U_B.pElecSet)
		{			
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel87U_C.pRelCFG == CFG_STATUS_SET) && (*Rel87U_C.pRelSW == SW_STATUS_SET)) //��ٶ�C
	{		
		relayInputFlag = TRUE;
		if(Rel87U_C.pRelElec->Mod > *Rel87U_C.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	
	if((*Rel87R_A.p87RCFG== CFG_STATUS_SET) && (*Rel87R_A.p87RSW== SW_STATUS_SET)) //���ʲA
	{		
		relayInputFlag = TRUE;
		if(Rel87R_A.pId->Mod> *Rel87R_A.p87RISet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	
	if((*Rel87R_B.p87RCFG== CFG_STATUS_SET) && (*Rel87R_B.p87RSW== SW_STATUS_SET)) //���ʲA
	{		
		relayInputFlag = TRUE;
		if(Rel87R_B.pId->Mod> *Rel87R_B.p87RISet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	
	if((*Rel87R_C.p87RCFG== CFG_STATUS_SET) && (*Rel87R_C.p87RSW== SW_STATUS_SET)) //���ʲA
	{		
		relayInputFlag = TRUE;
		if(Rel87R_C.pId->Mod> *Rel87R_C.p87RISet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	if((*Rel50I_H1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_H1.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(Rel50I_H1.pRelElec->Mod > *Rel50I_H1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_H2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_H2.pRelSW == SW_STATUS_SET)) //�߲����2��
	{
		relayInputFlag = TRUE;
		if(Rel50I_H2.pRelElec->Mod > *Rel50I_H2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_H3.pRelCFG == CFG_STATUS_SET) && (*Rel50I_H3.pRelSW == SW_STATUS_SET)) //�߲����3��
	{
		relayInputFlag = TRUE;
		if(Rel50I_H3.pRelElec->Mod > *Rel50I_H3.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_L1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_L1.pRelSW == SW_STATUS_SET)) //��ѹ�����1��
	{
		relayInputFlag = TRUE;
		if(Rel50I_L1.pRelElec->Mod > *Rel50I_L1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_L2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_L2.pRelSW == SW_STATUS_SET)) //��ѹ�����2��
	{
		relayInputFlag = TRUE;
		if(Rel50I_L2.pRelElec->Mod > *Rel50I_L2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_SH.pRelCFG == CFG_STATUS_SET) && (*Rel50I_SH.pRelSW == SW_STATUS_SET)) //ĸ�䱣��
	{
		relayInputFlag = TRUE;
		if(Rel50I_SH.pRelElec->Mod > *Rel50I_SH.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_N1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_N1.pRelSW == SW_STATUS_SET)) //�������1��
	{
		relayInputFlag = TRUE;
		if(Rel50I_N1.pRelElec->Mod > *Rel50I_N1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_N2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_N2.pRelSW == SW_STATUS_SET)) //�������2��
	{
		relayInputFlag = TRUE;
		if(Rel50I_N2.pRelElec->Mod > *Rel50I_N2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel59U_NH.pRelCFG == CFG_STATUS_SET) && (*Rel59U_NH.pRelSW == SW_STATUS_SET)) //��ѹ�������ѹ��ѹ
	{
		relayInputFlag = TRUE;
		if(Rel59U_NH.pRelElec->Mod > *Rel59U_NH.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel59U_NL.pRelCFG == CFG_STATUS_SET) && (*Rel59U_NL.pRelSW == SW_STATUS_SET)) //��ѹ�������ѹ��ѹ
	{
		relayInputFlag = TRUE;
		if(Rel59U_NL.pRelElec->Mod > *Rel59U_NL.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50OL_1.pRelCFG == CFG_STATUS_SET) && (*Rel50OL_1.pRelSW == SW_STATUS_SET)) //������1��
	{
		relayInputFlag = TRUE;
		if(Rel50OL_1.pRelElec->Mod > *Rel50OL_1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50OL_2.pRelCFG == CFG_STATUS_SET) && (*Rel50OL_2.pRelSW == SW_STATUS_SET)) //������2��
	{
		relayInputFlag = TRUE;
		if(Rel50OL_2.pRelElec->Mod > *Rel50OL_2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50OL_3.pRelCFG == CFG_STATUS_SET) && (*Rel50OL_3.pRelSW == SW_STATUS_SET)) //������3��
	{
		relayInputFlag = TRUE;
		if(Rel50OL_3.pRelElec->Mod > *Rel50OL_3.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_JX.pRelCFG == CFG_STATUS_SET) && (*Rel50I_JX.pRelSW == SW_STATUS_SET)) //��϶��������
	{
		relayInputFlag = TRUE;
		if(Rel50I_JX.pRelElec1->Mod > *Rel50I_JX.pElec1Set)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}

	//ͻ������������,�б���Ͷ��
	if(relayInputFlag == TRUE)
	{
		 ReadPreSingleData(dwPickDataBuf,pChannel,11,RELAY_SAMP_INTERVAL);
		 for(LONG j=0;j<11;j++)
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

						WriteRelayWord(PickUpRW, RELAY_ON);
						ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
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
							
							WriteRelayWord(PickUpRW, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
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
	WORD lpcRelayWord = 0;
	if(FaultOnFlag ==FALSE)   						//��δ������ϴ���
	{		
		RelayPickUpCHK();					//ִ���������
		if(ReadRelayWord(PickUpRW))
		{			
			ResetAllRelay(TRUE);
			FaultSINum=RELAY_SAMP_INTERVAL;       //������������ʼ�����ϴ���������������
			FaultOnFlag=TRUE;
			return;
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
		if(HaveMakeStartRptFlag == FALSE)   //������ϵ���������������д����ϵ���
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}
		//���ʷ�����ж�
		if((*Rel50I_H1.pRelPDCFG == CFG_STATUS_SET)||(*Rel50I_H2.pRelPDCFG == CFG_STATUS_SET)||(*Rel50I_H3.pRelPDCFG == CFG_STATUS_SET))
		{
			if(wHIPhase == PHASE_A)
			{
				if(!HA_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTH))				//PT����ʱ�����ʷ����������ʧЧ
						HA_PDWithMemoryFlag = PowerDirCheck_90(&UBC,&IA,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFHA,RW_PRHA,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHA, RELAY_OFF);
						WriteRelayWord(RW_PRHA, RELAY_OFF);
					}
				}
			}
			else if(wHIPhase == PHASE_B)
			{
				if(!HB_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTH))				//PT����ʱ�����ʷ����������ʧЧ
						HB_PDWithMemoryFlag = PowerDirCheck_90(&UCA,&IB,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFHB,RW_PRHB,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHB, RELAY_OFF);
						WriteRelayWord(RW_PRHB, RELAY_OFF);
					}
				}
			}
			else if(wHIPhase == PHASE_C)
			{
				if(!HC_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTH))				//PT����ʱ�����ʷ����������ʧЧ
						HC_PDWithMemoryFlag = PowerDirCheck_90(&UAB,&IC,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFHC,RW_PRHC,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHC, RELAY_OFF);
						WriteRelayWord(RW_PRHC, RELAY_OFF);
					}
				}
			}
		}
		if((*Rel50I_L1.pRelPDCFG == CFG_STATUS_SET)||(*Rel50I_L2.pRelPDCFG == CFG_STATUS_SET))
		{
			if(wLIPhase == PHASE_A)
			{
				if(!LA_PDWithMemoryFlag)
					{
						if(!ReadRelayWord(RW_PTL))				//PT����ʱ�����ʷ����������ʧЧ
							LA_PDWithMemoryFlag = PowerDirCheck_90(&Ubc,&Ia,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFLA,RW_PRLA,0,FALSE);
						else
						{
							WriteRelayWord(RW_PFLA, RELAY_OFF);
							WriteRelayWord(RW_PRLA, RELAY_OFF);
						}
					}
			}
			else if(wLIPhase == PHASE_B)
			{
				if(!LB_PDWithMemoryFlag)
					{
						if(!ReadRelayWord(RW_PTL))				//PT����ʱ�����ʷ����������ʧЧ
							LB_PDWithMemoryFlag = PowerDirCheck_90(&Uca,&Ib,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFLB,RW_PRLB,0,FALSE);
						else
						{
							WriteRelayWord(RW_PFLB, RELAY_OFF);
							WriteRelayWord(RW_PRLB, RELAY_OFF);
						}
					}
			}
			else if(wLIPhase == PHASE_C)
			{
				if(!LC_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTL))				//PT����ʱ�����ʷ����������ʧЧ
						LC_PDWithMemoryFlag = PowerDirCheck_90(&Uab,&Ic,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFLC,RW_PRLC,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFLC, RELAY_OFF);
						WriteRelayWord(RW_PRLC, RELAY_OFF);
					}
				}
			}
		}
		ResetIdHLock();
		R68IdH2M();											//����г����������
		R68IdH2C();											//����г���ۺ������
		R68IdH2I();											//����г���������
		R68IrH2M();											//�ƶ���������г������
		R68IdHI();											//�ۺ�г������

		if(!ReadRelayWord(Rel87U_A.ActRelayWord)&&!ReadRelayWord(Rel87U_B.ActRelayWord)&&!ReadRelayWord(Rel87U_A.ActRelayWord))
		{
			if(!ReadRelayWord(Rel87R_B.ACTRelayWord)&&!ReadRelayWord(Rel87R_C.ACTRelayWord))
				Rel87RI(&Rel87R_A);									//���ʲA
			if(!ReadRelayWord(Rel87R_A.ACTRelayWord)&&!ReadRelayWord(Rel87R_C.ACTRelayWord))
				Rel87RI(&Rel87R_B);									//���ʲB
			if(!ReadRelayWord(Rel87R_B.ACTRelayWord)&&!ReadRelayWord(Rel87R_A.ACTRelayWord))
				Rel87RI(&Rel87R_C);									//���ʲC
		}
		if(!ReadRelayWord(Rel87R_A.ACTRelayWord)&&!ReadRelayWord(Rel87R_B.ACTRelayWord)&&!ReadRelayWord(Rel87R_C.ACTRelayWord))
		{
			if(!ReadRelayWord(Rel87U_B.ActRelayWord)&&!ReadRelayWord(Rel87U_C.ActRelayWord))
				GeneralOverValueRelay(&Rel87U_A);					//�ٶ�A
			if(!ReadRelayWord(Rel87U_C.ActRelayWord)&&!ReadRelayWord(Rel87U_A.ActRelayWord))
				GeneralOverValueRelay(&Rel87U_B);					//�ٶ�B
			if(!ReadRelayWord(Rel87U_B.ActRelayWord)&&!ReadRelayWord(Rel87U_A.ActRelayWord))
				GeneralOverValueRelay(&Rel87U_C);					//�ٶ�3
		}
		
		GeneralOverValueRelay(&Rel50I_H1);				    //��ѹ�����1��
		GeneralOverValueRelay(&Rel50I_H2);					//��ѹ�����2��
		GeneralOverValueRelay(&Rel50I_H3);					//��ѹ�����3��
		GeneralOverValueRelay(&Rel50I_L1);					//��ѹ�����1��
		GeneralOverValueRelay(&Rel50I_L2);					//��ѹ�����2��
		GeneralOverValueRelay(&Rel50I_SH);					//ĸ�䱣��
		GeneralOverValueRelay(&Rel50I_N1);					//����1��
		GeneralOverValueRelay(&Rel50I_N2);					//����2��
		GeneralOverValueRelay(&Rel59U_NH);					//��ѹ����ѹ
		GeneralOverValueRelay(&Rel59U_NL);					//��ѹ����ѹ
		GeneralOverValueRelay(&Rel50OL_1);					//������1��	
		GeneralOverValueRelay(&Rel50OL_2);					//������2��
		GeneralOverValueRelay(&Rel50OL_3);					//������3��
		GeneralOverValueRelay_3(&Rel50I_JX);					//��϶����


		if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))
		{
			if(!m_bHaveMakeTripFailRpt)
			{				
				MakeActRpt(R_52BF,RptSerialNumOfAct);   //����·����բʧ�ܱ���
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}
		//�б������������Ŀ�����
		if((ReadRelayWord(Rel87R_A.LoopCtrlRelayWord))||(ReadRelayWord(Rel87R_B.LoopCtrlRelayWord))||(ReadRelayWord(Rel87R_C.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel87U_A.LoopCtrlRelayWord))||(ReadRelayWord(Rel87U_B.LoopCtrlRelayWord))||(ReadRelayWord(Rel87U_C.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50I_H1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_H2.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_H3.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50I_L1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_L2.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_SH.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50I_N1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_N2.LoopCtrlRelayWord))||(ReadRelayWord(Rel59U_NH.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel59U_NL.LoopCtrlRelayWord))||(ReadRelayWord(Rel50OL_1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50OL_2.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50OL_3.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_JX.LoopCtrlRelayWord)))
		{
			
			if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))			//��բʧ�ܣ������б�־������������,���鸴��
			{
				WriteRelayWord(PickUpRW, RELAY_OFF);
				FaultCheckOutFlag = FALSE;
				FaultOnFlag = FALSE;
				HaveMakeStartRptFlag= FALSE;
				DB_RelayEnd(RptSerialNumOfAct); //�����ݿ���д���������				
				m_bHaveMakeTripFailRpt = FALSE;
				return;
			}
			else
			{
				FaultLockTime = dwRelayMSCnt;		//���ù���������ʱ�� 
				FaultCheckOutFlag = TRUE;			//�б���ѭ�����Ƽ̵�������λ���ü�����ϱ�־
				return;
			}
		}
		//�����б�������:1���Ѿ���������2��δ����������
		else				
		{
			if(FaultCheckOutFlag ==TRUE	)      // 1�����ϴ����Ѿ��������������ܹ�
			{
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime>500 )//���ϴ���������޹���ʱ�䳬��1�룬��λ���������̵����֣����־�����������棬���鸴��
				{	
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;
					DB_RelayEnd(RptSerialNumOfAct);	
					m_bHaveMakeTripFailRpt = FALSE;
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
				if(dwRelayMSCnt -FaultLockTime>1000 )//δ�����������޹���ʱ�䳬��0.5�룬��λ���������̵����֣����־�����������棬���鸴��
				{
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;					
					m_bFaultLKFlag = FALSE;
					DB_RelayEnd(RptSerialNumOfAct); 
					m_bHaveMakeTripFailRpt = FALSE;
					ResetAllRelay(TRUE);					
					return;					
				}
				else
					return;
			}			
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
	Reset87RI(&Rel87R_A,resetFlag);
	Reset87RI(&Rel87R_B,resetFlag);
	Reset87RI(&Rel87R_C,resetFlag);
	ResetOverValueRelay(&Rel87U_A,resetFlag);
	ResetOverValueRelay(&Rel87U_B,resetFlag);
	ResetOverValueRelay(&Rel87U_C,resetFlag);
	ResetOverValueRelay(&Rel50I_H1,resetFlag);
	ResetOverValueRelay(&Rel50I_H2,resetFlag);
	ResetOverValueRelay(&Rel50I_H3,resetFlag);
	ResetOverValueRelay(&Rel50I_L1,resetFlag);
	ResetOverValueRelay(&Rel50I_L2,resetFlag);
	ResetOverValueRelay(&Rel50I_SH,resetFlag);
	ResetOverValueRelay(&Rel50I_N1,resetFlag);
	ResetOverValueRelay(&Rel50I_N2,resetFlag);
	ResetOverValueRelay(&Rel59U_NH,resetFlag);
	ResetOverValueRelay(&Rel59U_NL,resetFlag);
	ResetOverValueRelay(&Rel50OL_1,resetFlag);
	ResetOverValueRelay(&Rel50OL_2,resetFlag);
	ResetOverValueRelay(&Rel50OL_3,resetFlag);
	ResetOverValueRelay_3(&Rel50I_JX,resetFlag);
	
}
void CRelayTask::Process52BF(void)
{
	if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))
	{
		m_b52BFFLAG = TRUE;
	}
	if(m_b52BFFLAG)
	{
		if((!ReadRelayWord(RW_52BFA))&&(!ReadRelayWord(RW_52BFB)))
		{
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&Rel27U);
			m_b52BFFLAG = FALSE;
		}
	}
}
void CRelayTask::PTCheck(void)
{
	if(*CFG_pbyPTCK == CFG_STATUS_CLR)
	{
		HPTCheckTimer1 = 0;
		HPTCheckTimer2 = 0;
		HPTCheckTimer3 = 0;
		HPTCheckTimer4 = 0;
		HPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTH))
		{
			MakeAlarmRpt(R_HPTAR_RES);
			WriteRelayWord(RW_PTH,RELAY_OFF);
		}
		
		LPTCheckTimer1 = 0;
		LPTCheckTimer2 = 0;
		LPTCheckTimer3 = 0;
		LPTCheckTimer4 = 0;
		LPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTL))
		{
			MakeAlarmRpt(R_LPTAR_RES);
			WriteRelayWord(RW_PTL,RELAY_OFF);
		}
		return;
	}
	//��ѹ��PT�����ж�
//	if(((UAB.Mod>=*RS_pn74VTV)&&(UBC.Mod>=*RS_pn74VTV)&&(UCA.Mod>=*RS_pn74VTV)&&(U2h.Mod<=U2Set))||(*CFG_pbyHAB == CFG_STATUS_SET))
	if((UAB.Mod>=*RS_pn74VTV)&&(UBC.Mod>=*RS_pn74VTV)&&(UCA.Mod>=*RS_pn74VTV)&&(U2h.Mod<=U2Set))

	{
		HPTCheckTimer1 = 0;
		HPTCheckTimer2 = 0;
		HPTCheckTimer3 = 0;
		HPTCheckTimer4 = 0;
		HPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTH))
		{
			MakeAlarmRpt(R_HPTAR_RES);
			WriteRelayWord(RW_PTH,RELAY_OFF);
		}
	}
	
	TRelElecValPar UL;
	UL = ElecMax(Uab,Ubc,Uca);
	if((UAB.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC)&&(UL.Mod>*RS_pn74VTV))
		HPTCheckTimer1++;
	else
		HPTCheckTimer1=0;
	
	if((UBC.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC)&&(UL.Mod>*RS_pn74VTV))
		HPTCheckTimer2++;
	else
		HPTCheckTimer2=0;
	
	if((UCA.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC)&&(UL.Mod>*RS_pn74VTV))
		HPTCheckTimer3++;
	else
		HPTCheckTimer3=0;
	
	if(U2h.Mod>U2Set)
		HPTCheckTimer4++;
	else
		HPTCheckTimer4=0;
		
	if(((HPTCheckTimer1*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer2*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer3*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer4*m_PTCheckTimer)>*RS_pn74VTD))    //yanxs 2012-12-03 ����PT����ʱ���ж�
	{
		if(!HPTMakeReportFlag)
		{
			MakeAlarmRpt(R_HPTAR);
			HPTMakeReportFlag = TRUE;
			WriteRelayWord(RW_PTH,RELAY_ON);
		}
	}
	//��ѹ��PT�����ж�
	
	if(((Uab.Mod>=*RS_pn74VTV)&&(Ubc.Mod>=*RS_pn74VTV)&&(Uca.Mod>=*RS_pn74VTV)&&(U2l.Mod<=U2Set))||(ReadRelayWord(RW_52B_A)))
	{
		LPTCheckTimer1 = 0;
		LPTCheckTimer2 = 0;
		LPTCheckTimer3 = 0;
		LPTCheckTimer4 = 0;
		LPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTL))
		{
			MakeAlarmRpt(R_LPTAR_RES);
			WriteRelayWord(RW_PTL,RELAY_OFF);
		}
	}
	TRelElecValPar UH;
	UH = ElecMax(UAB,UBC,UCA);
	if((UH.Mod>*RS_pn74VTV)&&(ReadRelayWord(RW_52A_A))&&(ReadRelayWord(RW_52A_B)))//��ѹ����ѹ�Ҹ�ѹ���·�����ں�λ
	{
		if(Uab.Mod<*RS_pn74VTV)
			LPTCheckTimer1++;
		else
			LPTCheckTimer1=0;
		
		if(Ubc.Mod<*RS_pn74VTV)
			LPTCheckTimer2++;
		else
			LPTCheckTimer2=0;
		
		if(Uca.Mod<*RS_pn74VTV)
			LPTCheckTimer3++;
		else
			LPTCheckTimer3=0;
		
		if(U2l.Mod>U2Set)
			LPTCheckTimer4++;
		else
			LPTCheckTimer4=0;
			
		if(((LPTCheckTimer1*m_PTCheckTimer)>*RS_pn74VTD) \
			||((LPTCheckTimer2*m_PTCheckTimer)>*RS_pn74VTD) \
			||((LPTCheckTimer3*m_PTCheckTimer)>*RS_pn74VTD) \
			||((LPTCheckTimer4*m_PTCheckTimer)>*RS_pn74VTD))          //yanxs 2012-12-03 ����PT����ʱ���ж�
		{
			if(!LPTMakeReportFlag)
			{
				MakeAlarmRpt(R_LPTAR);
				LPTMakeReportFlag = TRUE;
				WriteRelayWord(RW_PTL,RELAY_ON);
			}
		}
	}
	else
	{
		LPTCheckTimer1 = 0;
		LPTCheckTimer2 = 0;
		LPTCheckTimer3 = 0;
		LPTCheckTimer4 = 0;
		LPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTL))
		{
			MakeAlarmRpt(R_LPTAR_RES);
			WriteRelayWord(RW_PTL,RELAY_OFF);
		}
	}

	
}


