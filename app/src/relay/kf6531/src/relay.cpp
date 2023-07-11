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
#include "scdefs.h"
#include "daefunc.h"
extern BYTE   byRelayWordValue[];
extern HANDLE g_hDbaseTaskID;
extern DWORD  dwRelayDataPtr;	//AD��ǰдָ��
extern DWORD  dwRelayMSCnt;		//�����ú��������
extern BYTE   g_RptSerialNum;
extern BOOL   RelayCheckErr;			//�����Լ����
extern BOOL   RelayCheckInitOK;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD dwADResumeCnt;
extern BYTE *g_MaintSW;


DWORD dwRelayTaskID;

BOOL  RelayTaskInitOK = FALSE;
DWORD dwCaluInterval;	//�ȹ����ɼ����ۼӼ��
BOOL  g_bRel49Flag;		//�ȹ����ɽ����ۼӼ�����Ϣ��־
BOOL  g_bRel49TimeOut;  //�ȹ������ۼ���ʱ����־

DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
TStructCheck StructCheck[CHECK_NUM];			//�����ṹ��У������

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
/*	
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	//��ȡ���ݿⱣ������ֵ����ָ�룬�Լӿ�����ٶ�
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;

	InitAllRelayMem();			//��ʼ�������ڲ�����
	InitRealy();				//��ʼ������Ԫ��
	InitSet();
	InitDCRecInfo();
	InitStructCheck();
	

	RelCFGCLR = CFG_STATUS_CLR;			//�����˳�
	RelCFGSET = CFG_STATUS_SET;			//����Ͷ��
	
	g_RptSerialNum = 0;					//ϵͳ�ı������к�
	m_wRptSerialNumOfAct =0;			//ϵͳ�ı��������������к�
	m_wRptSerialNumOfStart=0;			//ϵͳ�Ĵ����ϵ����������������к�	
	m_dwFaultSINum = 0;					//�������������	
	
	m_bTripFailFlag = FALSE;			//��բʧ�ܱ�־
	m_bFaultOnFlag = FALSE;				//������ϴ����־
	m_bHaveMakeStartRptFlag = FALSE;	//���������ϵ������������־
	m_bFaultLKFlag = FALSE;				//������ʼʱ��������־
	m_bFaultCheckOutFlag = FALSE;		//������ϱ�־
	m_bLastRecStatusFlag = FALSE;		//����¼�������������жϱ�־

	m_b52BFLAFlag = FALSE;
	m_b52BFLBFlag = FALSE;
	m_b52BFHFlag = FALSE;
	m_bHaveMake52BF_H_RptFlag = FALSE;	//������ѹ���·����բʧ�ܱ���		
	m_bHaveMake52BF_LA_RptFlag = FALSE; 	
	m_bHaveMake52BF_LB_RptFlag = FALSE; 	
	m_b52BFRecover_H_FLAG = FALSE;		//��ѹ���·����բʧ�ָܻ���־
	m_b52BFRecover_LA_FLAG = FALSE;
	m_b52BFRecover_LB_FLAG = FALSE;

	
	m_wPickCheck = 0;					//ͻ���������ж������������
	for(LONG i=0;i<7;i++)
	{		
		m_wPickCheckNext[i] = 0;				//ͻ���������ж���������������������
		m_lPickUpValueArray[i] = 0;			//ͻ������ֵ������������
	}
	for(LONG i=0;i<6;i++)
	{
		DCCaluResult[i]=0;
	}
	//�������ݶ�ȡͨ������
	dwChanDataMask = ( (0x01<<CHANNEL_6531_UHA)|(0x01<<CHANNEL_6531_UHB)|(0x01<<CHANNEL_6531_UHC)| \
						(0x01<<CHANNEL_6531_U0)|(0x01<<CHANNEL_6531_ULA)|(0x01<<CHANNEL_6531_ULB)| \
						(0x01<<CHANNEL_6531_IHA)|(0x01<<CHANNEL_6531_IHB)|(0x01<<CHANNEL_6531_IHC)| \
						(0x01<<CHANNEL_6531_I0)|(0x01<<CHANNEL_6531_I1)|(0x01<<CHANNEL_6531_I2)|\
						(0x01<<CHANNEL_6531_I3)|(0x01<<CHANNEL_6531_I4));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	dwComposeDataMask = ((0x01<<CHANNEL_6531_ILA)|(0x01<<CHANNEL_6531_ILB)|(0x01<<CHANNEL_6531_Ijx));

	
	//��ʼ����ȡ����������ʱ������
	//�������ݻ�����
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*14);	
	//�ϳ����ݻ�д������
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	//ͻ����������ʷ���ݻ�����
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*7);
	//�ߵ�ѹ���������ݻ�����
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*7);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(DWORD)*READ_POINT_NUM*14);

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);


	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*7);
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*7);
	//�����ǳ�ʼ���߼����̲���
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	//��ʼ�������Լ�Ϊ����״̬
	for(int i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	//���Ƚ���һ�鿪��ʧЧ�Լ���
	for(int j=0; j<OUTPUT_RELAY_WORD_NUM; j++)
		BoInvalidCheck();
	
	RelayTaskInitOK = TRUE;
	m_bLogicChanged = FALSE;
	m_dwWaitLogicCounter = 0;
	g_MaintSW = SW_pbyMAINT;
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
	dwCaluInterval = Rel49I_IA.l49OT*2;
	if((*CFG_pby49 == CFG_STATUS_SET)&&(*SW_pby49 == SW_STATUS_SET))
		g_bRel49Flag = TRUE;
	else
		g_bRel49Flag = FALSE;

}
/************************************************************************************************
*����: ��ʼ������Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	 InitRel50I_IA();
	 InitRel50I_IB();
	 InitRel50I_IC();
	 InitRel50I_Ia1();
	 InitRel50I_Ia2();	 
	 InitRel50I_Ib1();
	 InitRel50I_Ib2();
	 InitRel50I_JX();
	 InitRel49IHA();
	 InitRel49IHB();
   	 InitRel49IHC();
	 InitRel50IA1();
	 InitRel50IA2();
	 InitRel50IB1();
	 InitRel50IB2();
	 InitRel50IC1();
	 InitRel50IC2();
	 InitRel50IN();
	 InitRel59UN();
	 InitRel27U();
	 InitRel74PT();
	
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
void CRelayTask::ThermalOverLoad()
{		
	if((*CFG_pby49 != CFG_STATUS_SET)||(*SW_pby49 != SW_STATUS_SET))
		return;		
	RelThermalOverLoad(&Rel49I_IA);	//A�����ۼӼ���
	RelThermalOverLoad(&Rel49I_IB);	//B�����ۼӼ���
	RelThermalOverLoad(&Rel49I_IC);	//C�����ۼӼ���
	//�ȹ����ɶ�������Ҫ���࣬�����������ʱ��ֻ����A��ı���INF
	if(ReadRelayWord(Rel49I_IA.R49WarnRW)||ReadRelayWord(Rel49I_IB.R49WarnRW)||ReadRelayWord(Rel49I_IC.R49WarnRW))	//�澯�̵�������λ
	{
		if(m_bHaveMakeWarnRpt == FALSE)	//��δ���澯����
		{
			MakeAlarmRpt(Rel49I_IA.R49WarnRptNum);	//���澯����,�ȹ����ɸ澯
			m_bHaveMakeWarnRpt = TRUE;		//�������澯�����־
			return;
		}
	}
	if(ReadRelayWord(Rel49I_IA.R49ActRW)||ReadRelayWord(Rel49I_IB.R49ActRW)||ReadRelayWord(Rel49I_IC.R49ActRW))
	{
		if(m_bHaveMakeActRpt == FALSE)
		{			
			TAbsTime RptTime;
			WORD	 RptNo;
			ReadAbsTime(&RptTime);
			DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&RptTime,FALSE); //�����ݿ���д����������
			RptNo = g_RptSerialNum;
			MakeActRpt(Rel49I_IA.R49ActRptNum,RptNo);	
			DB_RelayEnd(RptNo); 			//�����ݿ���д�������
			m_bHaveMakeActRpt = TRUE;			
			m_bHaveMakeRSTRpt = FALSE;			
		}
		return;  //�����̵�������λ��ֱ���˳�
	}
	if((!ReadRelayWord(Rel49I_IA.R49WarnRW))&&(!ReadRelayWord(Rel49I_IB.R49WarnRW))&&(!ReadRelayWord(Rel49I_IC.R49WarnRW)))
	{		
		if((m_bHaveMakeRSTRpt == FALSE)&&(m_bHaveMakeWarnRpt == TRUE)) //�޸澯�̵�������λ��
		{
			MakeAlarmRpt(Rel49I_IA.R49RSTRptNum);
			m_bHaveMakeRSTRpt = TRUE;
			m_bHaveMakeActRpt = FALSE;
			m_bHaveMakeWarnRpt = FALSE;
		}
	}
}
//��������������Ϣ����
void CRelayTask::OnSampleEnd(DWORD arg)
{	
//	MCF_GPIO_PODR_TIMER |= 0x02;		//�ߵ�ƽ

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
	static WORD wInvalidCheckCnt;	
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;

	if(!m_bLogicChanged)
		BoBreakCheck();

	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	if(FALSE == WriteComposeBuf())
		return;
	CaluUpIp();
	CaluUI(); 	
	//���Ƚ��п����Լ�
	
	if(m_bFaultOnFlag == FALSE)
	{
		PTCheck();							
	}
	
	FalutProcess();								//����ѭ������
	GeneralOverValueRelay(&Rel50IA1);
	GeneralOverValueRelay(&Rel50IB1);
	GeneralOverValueRelay(&Rel50IC1);	
	GeneralUnderValRelay(&Rel27U);				//�͵�ѹ�����ӳ���
	if(g_bRel49TimeOut == TRUE)
	{
		ThermalOverLoad();
		g_bRel49TimeOut = FALSE;
	}
	LogicProcess();
	Process52BF();
	
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

	//MCF_GPIO_PODR_TIMER &= ~0x02;		//�͵�ƽ
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
// ����: ͨ���ϳ�	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::WriteComposeBuf()
{
	//�ϳɼ����ߵ�ѹ���õĲ�������
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{	
		//Uab
		dwComposeBuf[i] = dwSampBuf[i]-dwSampBuf[READ_POINT_NUM+i];
		//Ubc
		dwComposeBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM+i]-dwSampBuf[READ_POINT_NUM*2+i];
		//Uca
		dwComposeBuf[READ_POINT_NUM*2+i] = dwSampBuf[READ_POINT_NUM*2+i]-dwSampBuf[i];
		//Ulab
		dwComposeBuf[READ_POINT_NUM*3+i] = dwSampBuf[READ_POINT_NUM*4+i]-dwSampBuf[READ_POINT_NUM*5+i];
		if(*AS_pnLCN == LCUR_2)
		{
			dwComposeBuf[READ_POINT_NUM*4+i] = dwSampBuf[READ_POINT_NUM*10+i]; //Ia
			dwComposeBuf[READ_POINT_NUM*5+i] = dwSampBuf[READ_POINT_NUM*12+i]; //Ib
			dwComposeBuf[READ_POINT_NUM*6+i] = dwSampBuf[READ_POINT_NUM*13+i]; //Ijx
		}
		else if(*AS_pnLCN == LCUR_4)
		{
			dwComposeBuf[READ_POINT_NUM*4+i] = dwSampBuf[READ_POINT_NUM*10+i]-dwSampBuf[READ_POINT_NUM*11+i]; //Ia
			dwComposeBuf[READ_POINT_NUM*5+i] = dwSampBuf[READ_POINT_NUM*12+i]-dwSampBuf[READ_POINT_NUM*13+i];  //Ib
			dwComposeBuf[READ_POINT_NUM*6+i] = 0;//VXʱ�޼�϶��������
		}
		else
		{
			dwComposeBuf[READ_POINT_NUM*4+i] = 0;
			dwComposeBuf[READ_POINT_NUM*5+i] = 0;
			dwComposeBuf[READ_POINT_NUM*6+i] = 0;
		}
	}
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM*5-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwComposeBuf[READ_POINT_NUM*6-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*2+i] = dwComposeBuf[READ_POINT_NUM*7-RELAY_SAMP_INTERVAL+i];
	}
	if(WriteComposeData(dwRecBuf, dwComposeDataMask))
	{
		ReportMsg("Error:Write ComposeData failure!");
		RelayCheckErr = TRUE;
		return FALSE;
	}
	return TRUE;

}
//------------------------------------------------------------------------------------------ 
// ����: �������	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUpIp(void)
{
	//�����ߵ�ѹ
	CaluBaseFourier_S15(dwComposeBuf,&LineUHAB);
	CaluModValue(&LineUHAB);	
	*RM_pnUHAB= LineUHAB.Mod;                                       
	DB_LogWriteRM(RM_UHAB, *RM_pnUHAB);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&LineUHBC);
	CaluModValue(&LineUHBC);	
	*RM_pnUHBC= LineUHBC.Mod;                                       
	DB_LogWriteRM(RM_UHBC, *RM_pnUHBC);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*2,&LineUHCA);
	CaluModValue(&LineUHCA);	
	*RM_pnUHCA= LineUHCA.Mod;                                       
	DB_LogWriteRM(RM_UHCA, *RM_pnUHCA);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*3,&LineULAB);
	CaluModValue(&LineULAB);	
	*RM_pnULAB= LineULAB.Mod;                                       
	DB_LogWriteRM(RM_ULAB, *RM_pnULAB);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*4,&ILA);
	CaluModValue(&ILA);	
	*RM_pnILA= ILA.Mod;                                       
	DB_LogWriteRM(RM_ILA, *RM_pnILA);	
	if(ILA.Mod>(*AS_pn50LAC))
	{				  
		WriteRelayWord(RW_50LA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LA, RELAY_OFF);
	}
	if(ILA.Mod>(*AS_pn37LAC))
	{
		WriteRelayWord(RW_37LA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LA, RELAY_ON);
	}
	//����ILB
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*5,&ILB);
	CaluModValue(&ILB);	
	*RM_pnILB= ILB.Mod;                                       
	DB_LogWriteRM(RM_ILB, *RM_pnILB);	
	if(ILB.Mod>(*AS_pn50LBC))
	{				  
		WriteRelayWord(RW_50LB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LB, RELAY_OFF);
	}
	if(ILB.Mod>(*AS_pn37LBC))
	{
		WriteRelayWord(RW_37LB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*6,&IJX);
	CaluModValue(&IJX);	
	*RM_pnIJX= IJX.Mod;                                       
	DB_LogWriteRM(RM_IJX, *RM_pnIJX);	
	if(IJX.Mod>(*AS_pn50JXC))
	{				  
		WriteRelayWord(RW_50JX, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50JX, RELAY_OFF);
	}
	if(IJX.Mod>(*AS_pn37JXC))
	{
		WriteRelayWord(RW_37JX, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37JX, RELAY_ON);
	}

	//�����ȹ��������õı����������������ۼӼ���---����Чֵ
	if((*CFG_pby49 == CFG_STATUS_SET)&&(*SW_pby49 == SW_STATUS_SET))
	{
		Filter(dwSampBuf+READ_POINT_NUM*6);
		Filter(dwSampBuf+READ_POINT_NUM*7);
		Filter(dwSampBuf+READ_POINT_NUM*8);
		
		CaluMSMod(dwSampBuf+READ_POINT_NUM*6, IAP);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*7, IBP);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*8, ICP);
		
		Rel49I_IA.pCurrent = (IAP+Rel49I_IA.PreCurrent)/2;
		Rel49I_IA.PreCurrent = Rel49I_IA.pCurrent;
		Rel49I_IB.pCurrent = (IBP+Rel49I_IB.PreCurrent)/2;
		Rel49I_IB.PreCurrent = Rel49I_IB.pCurrent;
		Rel49I_IC.pCurrent = (ICP+Rel49I_IC.PreCurrent)/2;
		Rel49I_IC.PreCurrent = Rel49I_IC.pCurrent;
	}

	//���3���ߵ�ѹ�����ֵ�Լ���Сֵ,�������ֵ���ڸ߲�PT���ߵ��жϣ���Сֵ���ڵͲ�PT���ߵ��ж�
	PhaseUHMin = MinElecABC(UHA,UHB,UHC);//��ѹ��PT�����жϸ�Ϊ���ѹ�жϣ�yanzh 2013��1��23��14:29:49
	LineUHMax = MaxElecABC(LineUHAB,LineUHBC,LineUHCA);
	//����ʧѹ�����õĵ�ѹ�Ǹ�ѹ���������ѹ����ѹ��������ѹ��5����ѹ�е����ֵ
	PhaseUHMax = MaxElecABC(UHA,UHB,UHC);
	U_R27U = MaxElecABC(PhaseUHMax,ULA,ULB);
	
	//�ó��߲�ʧѹ�������������õĵ���  �߲�����������ֵ
	I_R27U = MaxElecABC(IHA,IHB,IHC);
	//��ѹ��������ѹ�еĴ�ֵ���ڸ�ѹ��PT�����ж�
	//��ѹ��������ѹ�����ǵ��ߵ�ѹ������ѹ�е���Сֵ���ڸ�ѹ����������ĵ�ѹ����
	if(ULA.Mod>=ULB.Mod)
	{	
		PhaseULMin = ULB;
		PhaseULMax = ULA;
	}
	else
	{
		PhaseULMin = ULA;
		PhaseULMax = ULB;
	}

	if(PhaseULMin.Mod <= LineULAB.Mod*2)
	{
		U_R50IH = PhaseULMin;
	}
	else
	{
		U_R50IH.Mod = LineULAB.Mod*2;   //��ѹ���ߵ�ѹ��2�붨ֵ���бȽ�
	}

	if(ULA.Mod>=LineULAB.Mod*2)
		ULmin1.Mod =  LineULAB.Mod*2;
	else
		ULmin1 = ULA;

	if(ULB.Mod>=LineULAB.Mod*2)
		ULmin2.Mod =  LineULAB.Mod*2;
	else
		ULmin2 = ULB;
}
//------------------------------------------------------------------------------------------ 
// ����: �������	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::CaluUI(void)
{	
	 //����UHA
	CaluBaseFourier_S15(dwSampBuf,&UHA);
	CaluModValue(&UHA);	
	*RM_pnUHA= UHA.Mod;                                       
	DB_LogWriteRM(RM_UHA, *RM_pnUHA);
	if(UHA.Mod>(*AS_pn59HAV))
	{				
		WriteRelayWord(RW_59HA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59HA, RELAY_OFF);
	}
	if(UHA.Mod>(*AS_pn27HAV))
	{
		WriteRelayWord(RW_27HA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27HA, RELAY_ON);
	}
	  //����UHB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UHB);
	CaluModValue(&UHB);
	*RM_pnUHB= UHB.Mod;									   
	DB_LogWriteRM(RM_UHB, *RM_pnUHB);	
	if(UHB.Mod>(*AS_pn59HBV))
	{				 
		 WriteRelayWord(RW_59HB, RELAY_ON);
	}
	else
	{
		 WriteRelayWord(RW_59HB, RELAY_OFF);
	}
	if(UHB.Mod>(*AS_pn27HBV))
	{
		 WriteRelayWord(RW_27HB, RELAY_OFF);
	}
	else
	{
		 WriteRelayWord(RW_27HB, RELAY_ON);
	}
	  //����UHC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UHC);
	CaluModValue(&UHC);
	*RM_pnUHC= UHC.Mod;									   
	DB_LogWriteRM(RM_UHC, *RM_pnUHC);	
	if(UHC.Mod>(*AS_pn59HCV))
	{				 
		 WriteRelayWord(RW_59HC, RELAY_ON);
	}
	else
	{
		 WriteRelayWord(RW_59HC, RELAY_OFF);
	}
	if(UHC.Mod>(*AS_pn27HCV))
	{
		 WriteRelayWord(RW_27HC, RELAY_OFF);
	}
	else
	{
		 WriteRelayWord(RW_27HC, RELAY_ON);
	}

	//����U0
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0);
	CaluModValue(&U0);
	*RM_pnU0= U0.Mod;									
	DB_LogWriteRM(RM_U0, *RM_pnU0);
	if(U0.Mod>(*AS_pn59NV))
	{				  
		WriteRelayWord(RW_59N, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59N, RELAY_OFF);
	}
	if(U0.Mod>(*AS_pn27NV))
	{
		WriteRelayWord(RW_27N, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27N, RELAY_ON);
	}
	//����ULA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&ULA);
	CaluModValue(&ULA);
	*RM_pnULA= ULA.Mod;									
	DB_LogWriteRM(RM_ULA, *RM_pnULA);	
	if(ULA.Mod>(*AS_pn59LAV))
	{				  
		WriteRelayWord(RW_59LA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LA, RELAY_OFF);
	}
	if(ULA.Mod>(*AS_pn27LAV))
	{
		WriteRelayWord(RW_27LA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LA, RELAY_ON);
	}
	//����ULB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&ULB);
	CaluModValue(&ULB);
	*RM_pnULB= ULB.Mod;									
	DB_LogWriteRM(RM_ULB, *RM_pnULB);	
	if(ULB.Mod>(*AS_pn59LBV))
	{				  
		WriteRelayWord(RW_59LB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LB, RELAY_OFF);
	}
	if(ULB.Mod>(*AS_pn27LBV))
	{
		WriteRelayWord(RW_27LB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LB, RELAY_ON);
	}
	//����IHA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&IHA);
	CaluModValue(&IHA);
	*RM_pnIHA= IHA.Mod;									
	DB_LogWriteRM(RM_IHA, *RM_pnIHA);	
	if(IHA.Mod>(*AS_pn50HAC))
	{				  
		WriteRelayWord(RW_50HA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HA, RELAY_OFF);
	}
	if(IHA.Mod>(*AS_pn37HAC))
	{
		WriteRelayWord(RW_37HA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HA, RELAY_ON);
	}
	//����IHB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&IHB);
	CaluModValue(&IHB);
	*RM_pnIHB= IHB.Mod;									
	DB_LogWriteRM(RM_IHB, *RM_pnIHB);
	if(IHB.Mod>(*AS_pn50HBC))
	{				  
		WriteRelayWord(RW_50HB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HB, RELAY_OFF);
	}
	if(IHB.Mod>(*AS_pn37HBC))
	{
		WriteRelayWord(RW_37HB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HB, RELAY_ON);
	}
	//����IHC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IHC);
	CaluModValue(&IHC);
	*RM_pnIHC= IHC.Mod;									
	DB_LogWriteRM(RM_IHC, *RM_pnIHC);
	if(IHC.Mod>(*AS_pn50HCC))
	{				  
		WriteRelayWord(RW_50HC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HC, RELAY_OFF);
	}
	if(IHC.Mod>(*AS_pn37HCC))
	{
		WriteRelayWord(RW_37HC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HC, RELAY_ON);
	}
	//����I0
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&I0);
	CaluModValue(&I0);
	*RM_pnI0= I0.Mod;									
	DB_LogWriteRM(RM_I0, *RM_pnI0);
	if(I0.Mod>(*AS_pn50NC))
	{				  
		WriteRelayWord(RW_50N, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50N, RELAY_OFF);
	}
	if(I0.Mod>(*AS_pn37NC))
	{
		WriteRelayWord(RW_37N, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37N, RELAY_ON);
	}
	//����IL1
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&IL1);
	CaluModValue(&IL1);
	*RM_pnIL1= IL1.Mod;									
	DB_LogWriteRM(RM_IL1, *RM_pnIL1);
	//����IL2
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&IL2);
	CaluModValue(&IL2);
	*RM_pnIL2= IL2.Mod;									
	DB_LogWriteRM(RM_IL2, *RM_pnIL2);
	//����IL3
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&IL3);
	CaluModValue(&IL3);
	*RM_pnIL3= IL3.Mod;									
	DB_LogWriteRM(RM_IL3, *RM_pnIL3);	
	//����IL4
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&IL4);
	CaluModValue(&IL4);
	*RM_pnIL4= IL4.Mod;									
	DB_LogWriteRM(RM_IL4, *RM_pnIL4);	
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���߲�A������������Ԫ��                                                                   
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_IA()
{
	Rel50I_IA.pElecSet			= RS_pn50HAC;          			//��������������ֵ
	Rel50I_IA.pTimeSet			= RS_pn50HAD;				 	//��������ʱ�䶨ֵ
	Rel50I_IA.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_IA.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_IA.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_IA.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_IA.pRelCFG			= CFG_pby50HA;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_IA.pRelSW			= SW_pby50H;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_IA.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IA.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IA.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IA.pRelVolLockCFG	= CFG_pby50H_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IA.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_IA.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_IA.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_IA.pRel47UCFG		= &RelCFGCLR;
	Rel50I_IA.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_IA.pRelElec 			= &IHA;							//��ǰ����ָ��		
	Rel50I_IA.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50I_IA.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50I_IA.pRelVolt			= &U_R50IH;							//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_IA.StartRelayWord	= RW_PI50HA;					//����������μ̵�����
	Rel50I_IA.ActRelayWord		= RW_TR50HA;					//�������������̵�����
	Rel50I_IA.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_IA.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_IA.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50I_IA.PRewardRelayWord  = FALSE1;						//���ʷ�����̵�����
	Rel50I_IA.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_IA.R47ULKRelayWord   = FALSE1;
	Rel50I_IA.LockRelayWord		= RW_LKPI50HA;					//��α����̵�����
	Rel50I_IA.RSTRelayWord		= RW_LKRST50HA;				//���ر����̵�����
	Rel50I_IA.LoopCtrlRelayWord	= RW_LPC50HA;					//����ѭ�����Ƽ̵�����
	
	Rel50I_IA.wReportNum		= R_R50IHA;						//���������������
	Rel50I_IA.wAccReportNum		= 0xFFFF;						//���ٶ����������
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_IA.wPickUpReportNum	= R_R50IHA_PI;						//��α������к�
	Rel50I_IA.wResetReportNum	= R_R50IHA_RES;						//���ر������к�
	//**********************************************************************	

	Rel50I_IA.bMakeActRpt		= TRUE;
	Rel50I_IA.dwK35				= NULL;
	Rel50I_IA.RptSerialNum		= &m_wRptSerialNumOfAct;		
//	Rel50I_IA.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_IA.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_IA.byRelRCounter		= 0;						//���ؼ�����	
	Rel50I_IA.wExcepWordNum     = 8;

}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���߲�B��������Ԫ��                                                                    
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_IB()
{
	Rel50I_IB.pElecSet			= RS_pn50HBC;          			//��������������ֵ
	Rel50I_IB.pTimeSet			= RS_pn50HBD;				 	//��������ʱ�䶨ֵ
	Rel50I_IB.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_IB.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_IB.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_IB.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_IB.pRelCFG			= CFG_pby50HB;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_IB.pRelSW			= SW_pby50H;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_IB.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IB.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IB.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IB.pRelVolLockCFG	= CFG_pby50H_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IB.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_IB.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_IB.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	
	Rel50I_IB.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_IB.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_IB.pRelElec 			= &IHB;							//��ǰ����ָ��		
	Rel50I_IB.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50I_IB.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50I_IB.pRelVolt			= &U_R50IH;							//��ǰ���������ĵ�ѹ��ָ��

	Rel50I_IB.StartRelayWord	= RW_PI50HB;					//����������μ̵�����
	Rel50I_IB.ActRelayWord		= RW_TR50HB;					//�������������̵�����
	Rel50I_IB.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_IB.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_IB.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50I_IB.PRewardRelayWord  = FALSE1;						//���ʷ�����̵�����
	Rel50I_IB.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	
	Rel50I_IB.R47ULKRelayWord   = FALSE1;
	Rel50I_IB.LockRelayWord		= RW_LKPI50HB;					//��α����̵�����
	Rel50I_IB.RSTRelayWord		= RW_LKRST50HB;				//���ر����̵�����
	Rel50I_IB.LoopCtrlRelayWord	= RW_LPC50HB;					//����ѭ�����Ƽ̵�����
	
	Rel50I_IB.wReportNum		= R_R50IHB;						//���������������
	Rel50I_IB.wAccReportNum		= 0xFFFF;						//���ٶ����������

	Rel50I_IB.bMakeActRpt		= TRUE;
	Rel50I_IB.dwK35				= NULL;
	Rel50I_IB.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_IB.wPickUpReportNum	= R_R50IHB_PI;						//��α������к�
	Rel50I_IB.wResetReportNum	= R_R50IHB_RES;						//���ر������к�
	//**********************************************************************	


//	Rel50I_IB.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_IB.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_IB.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50I_IB.wExcepWordNum     = 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���߲�C��������Ԫ��                                              
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_IC()
{	
	Rel50I_IC.pElecSet			= RS_pn50HCC;          			//��������������ֵ
	Rel50I_IC.pTimeSet			= RS_pn50HCD;				 	//��������ʱ�䶨ֵ
	Rel50I_IC.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_IC.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_IC.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_IC.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_IC.pRelCFG			= CFG_pby50HC;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_IC.pRelSW			= SW_pby50H;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_IC.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IC.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IC.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IC.pRelVolLockCFG	= CFG_pby50H_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_IC.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_IC.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_IC.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_IC.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_IC.pRelElec 			= &IHC;							//��ǰ����ָ��		
	Rel50I_IC.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50I_IC.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50I_IC.pRelVolt			= &U_R50IH;							//��ǰ���������ĵ�ѹ��ָ��


	Rel50I_IC.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_IC.R47ULKRelayWord   = FALSE1;

	Rel50I_IC.StartRelayWord	= RW_PI50HC;					//����������μ̵�����
	Rel50I_IC.ActRelayWord		= RW_TR50HC;					//�������������̵�����
	Rel50I_IC.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_IC.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_IC.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50I_IC.PRewardRelayWord  = FALSE1;						//���ʷ�����̵�����
	Rel50I_IC.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_IC.LockRelayWord		= RW_LKPI50HC;					//��α����̵�����
	Rel50I_IC.RSTRelayWord		= RW_LKRST50HC;				//���ر����̵�����
	Rel50I_IC.LoopCtrlRelayWord	= RW_LPC50HC;					//����ѭ�����Ƽ̵�����
	
	Rel50I_IC.wReportNum		= R_R50IHC;						//���������������
	Rel50I_IC.wAccReportNum		= 0xFFFF;						//���ٶ����������

	Rel50I_IC.bMakeActRpt		= TRUE;
	Rel50I_IC.dwK35				= NULL;
	Rel50I_IC.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_IC.wPickUpReportNum	= R_R50IHC_PI;						//��α������к�
	Rel50I_IC.wResetReportNum	= R_R50IHC_RES;						//���ر������к�
	//**********************************************************************	


//	Rel50I_IC.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_IC.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_IC.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50I_IC.wExcepWordNum     = 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���Ͳ�A����I�α���Ԫ��                                        
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ia1()
{	
	Rel50I_Ia1.pElecSet			= RS_pn50LA1C;          			//��������������ֵ
	Rel50I_Ia1.pTimeSet			= RS_pn50LA1D;				 	//��������ʱ�䶨ֵ
	Rel50I_Ia1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_Ia1.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_Ia1.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_Ia1.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_Ia1.pRelCFG			= CFG_pby50LA1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_Ia1.pRelSW			= SW_pby50L;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_Ia1.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia1.pRelPDCFG		= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia1.pRelVolLockCFG	= CFG_pby50LA_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia1.pRelFDCFG		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_Ia1.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_Ia1.pRelFLCFG		= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_Ia1.pSysTimer		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_Ia1.pRelElec 		= &ILA;							//��ǰ����ָ��		
	Rel50I_Ia1.pRelHarm3		= NULL;							//��ǰ����г��ָ��	
	Rel50I_Ia1.pRelHarm5		= NULL;							//��ǰ���г��ָ��	
	Rel50I_Ia1.pRelVolt			= &ULmin1;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50I_Ia1.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ia1.R47ULKRelayWord   = FALSE1;

	Rel50I_Ia1.StartRelayWord	= RW_PI50LA1;					//����������μ̵�����
	Rel50I_Ia1.ActRelayWord		= RW_TR50LA1;					//�������������̵�����
	Rel50I_Ia1.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_Ia1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_Ia1.PForwardRelayWord= FALSE1;						//����������̵�����
	Rel50I_Ia1.PRewardRelayWord = FALSE1;						//���ʷ�����̵�����
	Rel50I_Ia1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_Ia1.LockRelayWord	= RW_LKPI50LA1;					//��α����̵�����
	Rel50I_Ia1.RSTRelayWord		= RW_LKRST50LA1;				//���ر����̵�����
	Rel50I_Ia1.LoopCtrlRelayWord= RW_LPC50LA1;					//����ѭ�����Ƽ̵�����
	
	Rel50I_Ia1.wReportNum		= R_R50ILA1;						//���������������
	Rel50I_Ia1.wAccReportNum	= 0xFFFF;						//���ٶ����������

	Rel50I_Ia1.bMakeActRpt		= TRUE;
	Rel50I_Ia1.dwK35			= NULL;
	Rel50I_Ia1.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_Ia1.wPickUpReportNum	= R_R50ILA1_PI;						//��α������к�
	Rel50I_Ia1.wResetReportNum	= R_R50ILA1_RES;						//���ر������к�
	//**********************************************************************	


//	Rel50I_Ia1.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_Ia1.byRelFCounter	= 0;						//���ϼ�����
	Rel50I_Ia1.byRelRCounter	= 0;						//���ؼ�����	
	Rel50I_Ia1.wExcepWordNum    = 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���Ͳ�A����II�α���Ԫ��                                        
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ia2()
{	
	Rel50I_Ia2.pElecSet			= RS_pn50LA2C;          			//��������������ֵ
	Rel50I_Ia2.pTimeSet			= RS_pn50LA2D;				 	//��������ʱ�䶨ֵ
	Rel50I_Ia2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_Ia2.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_Ia2.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_Ia2.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_Ia2.pRelCFG			= CFG_pby50LA2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_Ia2.pRelSW			= SW_pby50L;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_Ia2.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia2.pRelPDCFG		= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia2.pRelVolLockCFG	= CFG_pby50LA_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ia2.pRelFDCFG		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_Ia2.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_Ia2.pRelFLCFG		= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_Ia2.pSysTimer		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_Ia2.pRelElec 		= &ILA;							//��ǰ����ָ��		
	Rel50I_Ia2.pRelHarm3		= NULL;							//��ǰ����г��ָ��	
	Rel50I_Ia2.pRelHarm5		= NULL;							//��ǰ���г��ָ��	
	Rel50I_Ia2.pRelVolt			= &ULmin1;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50I_Ia2.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ia2.R47ULKRelayWord   = FALSE1;

	Rel50I_Ia2.StartRelayWord	= RW_PI50LA2;					//����������μ̵�����
	Rel50I_Ia2.ActRelayWord		= RW_TR50LA2;					//�������������̵�����
	Rel50I_Ia2.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_Ia2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_Ia2.PForwardRelayWord= FALSE1;						//����������̵�����
	Rel50I_Ia2.PRewardRelayWord = FALSE1;						//���ʷ�����̵�����
	Rel50I_Ia2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_Ia2.LockRelayWord	= RW_LKPI50LA2;					//��α����̵�����
	Rel50I_Ia2.RSTRelayWord		= RW_LKRST50LA2;				//���ر����̵�����
	Rel50I_Ia2.LoopCtrlRelayWord= RW_LPC50LA2;					//����ѭ�����Ƽ̵�����
	
	Rel50I_Ia2.wReportNum		= R_R50ILA2;						//���������������
	Rel50I_Ia2.wAccReportNum	= 0xFFFF;						//���ٶ����������

	Rel50I_Ia2.bMakeActRpt		= TRUE;
	Rel50I_Ia2.dwK35			= NULL;
	Rel50I_Ia2.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_Ia2.wPickUpReportNum	= R_R50ILA2_PI;						//��α������к�
	Rel50I_Ia2.wResetReportNum	= R_R50ILA2_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_Ia2.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_Ia2.byRelFCounter	= 0;						//���ϼ�����
	Rel50I_Ia2.byRelRCounter	= 0;						//���ؼ�����	
	Rel50I_Ia2.wExcepWordNum    = 8;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���Ͳ�B����I�α���Ԫ��                                              
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ib1()
{	
	Rel50I_Ib1.pElecSet			= RS_pn50LB1C;          			//��������������ֵ
	Rel50I_Ib1.pTimeSet			= RS_pn50LB1D;				 	//��������ʱ�䶨ֵ
	Rel50I_Ib1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_Ib1.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_Ib1.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_Ib1.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_Ib1.pRelCFG			= CFG_pby50LB1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_Ib1.pRelSW			= SW_pby50L;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_Ib1.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib1.pRelPDCFG		= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib1.pRelVolLockCFG	= CFG_pby50LB_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib1.pRelFDCFG		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_Ib1.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_Ib1.pRelFLCFG		= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_Ib1.pSysTimer		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_Ib1.pRelElec 		= &ILB;							//��ǰ����ָ��		
	Rel50I_Ib1.pRelHarm3		= NULL;							//��ǰ����г��ָ��	
	Rel50I_Ib1.pRelHarm5		= NULL;							//��ǰ���г��ָ��	
	Rel50I_Ib1.pRelVolt			= &ULmin2;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50I_Ib1.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ib1.R47ULKRelayWord   = FALSE1;

	Rel50I_Ib1.StartRelayWord	= RW_PI50LB1;					//����������μ̵�����
	Rel50I_Ib1.ActRelayWord		= RW_TR50LB1;					//�������������̵�����
	Rel50I_Ib1.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_Ib1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_Ib1.PForwardRelayWord= FALSE1;						//����������̵�����
	Rel50I_Ib1.PRewardRelayWord = FALSE1;						//���ʷ�����̵�����
	Rel50I_Ib1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_Ib1.LockRelayWord	= RW_LKPI50LB1;					//��α����̵�����
	Rel50I_Ib1.RSTRelayWord		= RW_LKRST50LB1;				//���ر����̵�����
	Rel50I_Ib1.LoopCtrlRelayWord= RW_LPC50LB1;					//����ѭ�����Ƽ̵�����
	
	Rel50I_Ib1.wReportNum		= R_R50ILB1;						//���������������
	Rel50I_Ib1.wAccReportNum	= 0xFFFF;						//���ٶ����������

	Rel50I_Ib1.bMakeActRpt		= TRUE;
	Rel50I_Ib1.dwK35			= NULL;
	Rel50I_Ib1.RptSerialNum		= &m_wRptSerialNumOfAct;				
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_Ib1.wPickUpReportNum	= R_R50ILB1_PI;						//��α������к�
	Rel50I_Ib1.wResetReportNum	= R_R50ILB1_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_Ib1.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_Ib1.byRelFCounter	= 0;						//���ϼ�����
	Rel50I_Ib1.byRelRCounter	= 0;						//���ؼ�����		
	Rel50I_Ib1.wExcepWordNum     = 8;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ���Ͳ�B����II�α���Ԫ��                                              
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ib2()
{	
	Rel50I_Ib2.pElecSet			= RS_pn50LB2C;          			//��������������ֵ
	Rel50I_Ib2.pTimeSet			= RS_pn50LB2D;				 	//��������ʱ�䶨ֵ
	Rel50I_Ib2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_Ib2.pVolLockSet		= RS_pn27LU;				 	//����������ѹ������ֵ
	Rel50I_Ib2.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_Ib2.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_Ib2.pRelCFG			= CFG_pby50LB2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_Ib2.pRelSW			= SW_pby50L;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_Ib2.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib2.pRelPDCFG		= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib2.pRelVolLockCFG	= CFG_pby50LB_27;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_Ib2.pRelFDCFG		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_Ib2.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_Ib2.pRelFLCFG		= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_Ib2.pSysTimer		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_Ib2.pRelElec 		= &ILB;							//��ǰ����ָ��		
	Rel50I_Ib2.pRelHarm3		= NULL;							//��ǰ����г��ָ��	
	Rel50I_Ib2.pRelHarm5		= NULL;							//��ǰ���г��ָ��	
	Rel50I_Ib2.pRelVolt			= &ULmin2;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50I_Ib2.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ib2.R47ULKRelayWord   = FALSE1;

	Rel50I_Ib2.StartRelayWord	= RW_PI50LB2;					//����������μ̵�����
	Rel50I_Ib2.ActRelayWord		= RW_TR50LB2;					//�������������̵�����
	Rel50I_Ib2.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_Ib2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_Ib2.PForwardRelayWord= FALSE1;						//����������̵�����
	Rel50I_Ib2.PRewardRelayWord = FALSE1;						//���ʷ�����̵�����
	Rel50I_Ib2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_Ib2.LockRelayWord	= RW_LKPI50LB2;					//��α����̵�����
	Rel50I_Ib2.RSTRelayWord		= RW_LKRST50LB2;				//���ر����̵�����
	Rel50I_Ib2.LoopCtrlRelayWord= RW_LPC50LB2;					//����ѭ�����Ƽ̵�����
	
	Rel50I_Ib2.wReportNum		= R_R50ILB2;						//���������������
	Rel50I_Ib2.wAccReportNum	= 0xFFFF;						//���ٶ����������

	Rel50I_Ib2.bMakeActRpt		= TRUE;
	Rel50I_Ib2.dwK35			= NULL;
	Rel50I_Ib2.RptSerialNum		= &m_wRptSerialNumOfAct;				
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_Ib2.wPickUpReportNum	= R_R50ILB2_PI;						//��α������к�
	Rel50I_Ib2.wResetReportNum	= R_R50ILB2_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_Ib2.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_Ib2.byRelFCounter	= 0;						//���ϼ�����
	Rel50I_Ib2.byRelRCounter	= 0;						//���ؼ�����		
	Rel50I_Ib2.wExcepWordNum    = 8;
}

//------------------------------------------------------------------------------------------ 
// ����: ��ʼ����϶��������Ԫ��                                          
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_JX()
{	
	Rel50I_JX.pElec1Set			= RS_pn50DCGC;          			//��������������ֵ	
	Rel50I_JX.pElec2Set			= RS_pn59NV;          			//��������������ֵ
	Rel50I_JX.pTimeSet			= RS_pn50DCGD;				 	//��������ʱ�䶨ֵ
	Rel50I_JX.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I_JX.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50I_JX.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I_JX.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I_JX.pRelCFG			= CFG_pby50DCG;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I_JX.pRelSW			= SW_pby50JX;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I_JX.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I_JX.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I_JX.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I_JX.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I_JX.pRelElec1 		= &IJX;							//��ǰ����ָ��	
	Rel50I_JX.pRelElec2 		= &U0;							//��ǰ����ָ��	
	Rel50I_JX.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50I_JX.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50I_JX.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50I_JX.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_JX.R47ULKRelayWord   = FALSE1;

	Rel50I_JX.StartRelayWord	= RW_PI50DCG;					//����������μ̵�����
	Rel50I_JX.ActRelayWord		= RW_TR50DCG;					//�������������̵�����
	Rel50I_JX.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I_JX.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50I_JX.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50I_JX.PRewardRelayWord  = FALSE1;						//���ʷ�����̵�����
	Rel50I_JX.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50I_JX.LockRelayWord		= RW_LKPI50DCG;					//��α����̵�����
	Rel50I_JX.RSTRelayWord		= RW_LKRST50DCG;				//���ر����̵�����
	Rel50I_JX.LoopCtrlRelayWord	= RW_LPC50DCG;					//����ѭ�����Ƽ̵�����
	
	Rel50I_JX.wReportNum		= R_R50IJX;						//���������������
	Rel50I_JX.wAccReportNum		= 0xFFFF;						//���ٶ����������

	Rel50I_JX.bMakeActRpt		= TRUE;
	Rel50I_JX.dwK35				= NULL;
	Rel50I_JX.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I_JX.wPickUpReportNum	= R_R50IJX_PI;						//��α������к�
	Rel50I_JX.wResetReportNum	= R_R50IJX_RES;						//���ر������к�
	//**********************************************************************	

//	Rel50I_JX.dwRelTimer		= 0;			 			//��ʱ��
	Rel50I_JX.byRelFCounter		= 0;						//���ϼ�����
	Rel50I_JX.byRelRCounter		= 0;						//���ؼ�����
	
	Rel50I_JX.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ��A������1�澯����Ԫ��                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IA1()
{	
	Rel50IA1.pElecSet			= RS_pn50OLA1C;          			//��������������ֵ
	Rel50IA1.pTimeSet			= RS_pn50OLA1D;				 	//��������ʱ�䶨ֵ
	Rel50IA1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50IA1.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50IA1.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50IA1.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50IA1.pRelCFG			= CFG_pby50OLA1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IA1.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IA1.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA1.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA1.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50IA1.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50IA1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50IA1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50IA1.pRelElec 			= &IHA;							//��ǰ����ָ��		
	Rel50IA1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50IA1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50IA1.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50IA1.pRel47UCFG		= &RelCFGCLR;;
	Rel50IA1.R47ULKRelayWord   = FALSE1;

	Rel50IA1.StartRelayWord		= RW_PI50OLA1;					//����������μ̵�����
	Rel50IA1.ActRelayWord		= RW_TR50OLA1;					//�������������̵�����
	Rel50IA1.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50IA1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50IA1.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50IA1.PRewardRelayWord   = FALSE1;						//���ʷ�����̵�����
	Rel50IA1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50IA1.LockRelayWord		= RW_LKPI50OLA1;					//��α����̵�����
	Rel50IA1.RSTRelayWord		= RW_LKRST50OLA1;				//���ر����̵�����
	Rel50IA1.LoopCtrlRelayWord	= RW_LPC50OLA1;					//����ѭ�����Ƽ̵�����
	
	Rel50IA1.wReportNum			= R_R50IA1;						//���������������
	Rel50IA1.wAccReportNum		= 0xFFFF;						//���ٶ����������

	Rel50IA1.bMakeActRpt		= FALSE;
	Rel50IA1.dwK35				= NULL;
	Rel50IA1.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50IA1.wPickUpReportNum	= 0xFFFF;						//��α������к�
	Rel50IA1.wResetReportNum	= 0xFFFF;						//���ر������к�
	//**********************************************************************	
//	Rel50IA1.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IA1.byRelFCounter		= 0;						//���ϼ�����
	Rel50IA1.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50IA1.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ��B������1�澯����Ԫ��                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IB1()
{	
	Rel50IB1.pElecSet			= RS_pn50OLB1C;          			//��������������ֵ
	Rel50IB1.pTimeSet			= RS_pn50OLB1D;				 	//��������ʱ�䶨ֵ
	Rel50IB1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50IB1.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50IB1.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50IB1.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50IB1.pRelCFG			= CFG_pby50OLB1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IB1.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IB1.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB1.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB1.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50IB1.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50IB1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50IB1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50IB1.pRelElec 			= &IHB;							//��ǰ����ָ��		
	Rel50IB1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50IB1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50IB1.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50IB1.pRel47UCFG		= &RelCFGCLR;;
	Rel50IB1.R47ULKRelayWord   = FALSE1;

	Rel50IB1.StartRelayWord		= RW_PI50OLB1;					//����������μ̵�����
	Rel50IB1.ActRelayWord		= RW_TR50OLB1;					//�������������̵�����
	Rel50IB1.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50IB1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50IB1.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50IB1.PRewardRelayWord   = FALSE1;						//���ʷ�����̵�����
	Rel50IB1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50IB1.LockRelayWord		= RW_LKPI50OLB1;					//��α����̵�����
	Rel50IB1.RSTRelayWord		= RW_LKRST50OLB1;				//���ر����̵�����
	Rel50IB1.LoopCtrlRelayWord	= RW_LPC50OLB1;					//����ѭ�����Ƽ̵�����
	
	Rel50IB1.wReportNum			= R_R50IB1;						//���������������
	Rel50IB1.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel50IB1.bMakeActRpt		= FALSE;

	Rel50IB1.dwK35				= NULL;
	Rel50IB1.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50IB1.wPickUpReportNum	= 0xFFFF;						//��α������к�
	Rel50IB1.wResetReportNum	= 0xFFFF;						//���ر������к�
	//**********************************************************************	
//	Rel50IB1.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IB1.byRelFCounter		= 0;						//���ϼ�����
	Rel50IB1.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50IB1.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ��C������1�澯����Ԫ��                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitRel50IC1()
{	
	Rel50IC1.pElecSet			= RS_pn50OLC1C;          			//��������������ֵ
	Rel50IC1.pTimeSet			= RS_pn50OLC1D;				 	//��������ʱ�䶨ֵ
	Rel50IC1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50IC1.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50IC1.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50IC1.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50IC1.pRelCFG			= CFG_pby50OLC1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IC1.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IC1.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC1.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC1.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50IC1.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50IC1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50IC1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50IC1.pRelElec 			= &IHC;							//��ǰ����ָ��		
	Rel50IC1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50IC1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50IC1.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50IC1.pRel47UCFG		= &RelCFGCLR;;
	Rel50IC1.R47ULKRelayWord   = FALSE1;

	Rel50IC1.StartRelayWord		= RW_PI50OLC1;					//����������μ̵�����
	Rel50IC1.ActRelayWord		= RW_TR50OLC1;					//�������������̵�����
	Rel50IC1.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50IC1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50IC1.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50IC1.PRewardRelayWord   = FALSE1;						//���ʷ�����̵�����
	Rel50IC1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50IC1.LockRelayWord		= RW_LKPI50OLC1;					//��α����̵�����
	Rel50IC1.RSTRelayWord		= RW_LKRST50OLC1;				//���ر����̵�����
	Rel50IC1.LoopCtrlRelayWord	= RW_LPC50OLC1;					//����ѭ�����Ƽ̵�����
	
	Rel50IC1.wReportNum			= R_R50IC1;						//���������������
	Rel50IC1.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel50IC1.bMakeActRpt		= FALSE;

	Rel50IC1.dwK35				= NULL;
	Rel50IC1.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50IC1.wPickUpReportNum	= 0xFFFF;						//��α������к�
	Rel50IC1.wResetReportNum	= 0xFFFF;						//���ر������к�
	//**********************************************************************	
//	Rel50IC1.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IC1.byRelFCounter		= 0;						//���ϼ�����
	Rel50IC1.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50IC1.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ��A������2��������Ԫ��                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IA2()
{	
	Rel50IA2.pElecSet			= RS_pn50OLA2C;          			//��������������ֵ
	Rel50IA2.pTimeSet			= RS_pn50OLA2D;				 	//��������ʱ�䶨ֵ
	Rel50IA2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50IA2.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50IA2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50IA2.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50IA2.pRelCFG			= CFG_pby59OLA2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IA2.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IA2.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA2.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IA2.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50IA2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50IA2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50IA2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50IA2.pRelElec 			= &IHA;							//��ǰ����ָ��		
	Rel50IA2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50IA2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50IA2.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50IA2.pRel47UCFG		= &RelCFGCLR;;
	Rel50IA2.R47ULKRelayWord   = FALSE1;

	Rel50IA2.StartRelayWord		= RW_PI50OLA2;					//����������μ̵�����
	Rel50IA2.ActRelayWord		= RW_TR50OLA2;					//�������������̵�����
	Rel50IA2.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50IA2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50IA2.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50IA2.PRewardRelayWord   = FALSE1;						//���ʷ�����̵�����
	Rel50IA2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50IA2.LockRelayWord		= RW_LKPI50OLA2;					//��α����̵�����
	Rel50IA2.RSTRelayWord		= RW_LKRST50OLA2;				//���ر����̵�����
	Rel50IA2.LoopCtrlRelayWord	= RW_LPC50OLA2;					//����ѭ�����Ƽ̵�����
	
	Rel50IA2.wReportNum			= R_R50IA2;						//���������������
	Rel50IA2.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel50IA2.bMakeActRpt		= TRUE;
	Rel50IA2.dwK35				= NULL;
	Rel50IA2.RptSerialNum		= &m_wRptSerialNumOfAct;
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50IA2.wPickUpReportNum	= R_R50IA2_PI;						//��α������к�
	Rel50IA2.wResetReportNum	= R_R50IA2_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50IA2.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IA2.byRelFCounter		= 0;						//���ϼ�����
	Rel50IA2.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50IA2.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ��B������2��������Ԫ��                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IB2()
{	
	Rel50IB2.pElecSet			= RS_pn50OLB2C;          			//��������������ֵ
	Rel50IB2.pTimeSet			= RS_pn50OLB2D;				 	//��������ʱ�䶨ֵ
	Rel50IB2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50IB2.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50IB2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50IB2.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50IB2.pRelCFG			= CFG_pby50OLB2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IB2.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IB2.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB2.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IB2.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50IB2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50IB2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50IB2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50IB2.pRelElec 			= &IHB;							//��ǰ����ָ��		
	Rel50IB2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50IB2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50IB2.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50IB2.pRel47UCFG		= &RelCFGCLR;;
	Rel50IB2.R47ULKRelayWord   = FALSE1;

	Rel50IB2.StartRelayWord		= RW_PI50OLB2;					//����������μ̵�����
	Rel50IB2.ActRelayWord		= RW_TR50OLB2;					//�������������̵�����
	Rel50IB2.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50IB2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50IB2.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50IB2.PRewardRelayWord   = FALSE1;						//���ʷ�����̵�����
	Rel50IB2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50IB2.LockRelayWord		= RW_LKPI50OLB2;					//��α����̵�����
	Rel50IB2.RSTRelayWord		= RW_LKRST50OLB2;				//���ر����̵�����
	Rel50IB2.LoopCtrlRelayWord	= RW_LPC50OLB2;					//����ѭ�����Ƽ̵�����
	
	Rel50IB2.wReportNum			= R_R50IB2;						//���������������
	Rel50IB2.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel50IB2.bMakeActRpt		= TRUE;
	Rel50IB2.dwK35				= NULL;
	Rel50IB2.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50IB2.wPickUpReportNum	= R_R50IB2_PI;						//��α������к�
	Rel50IB2.wResetReportNum	= R_R50IB2_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50IB2.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IB2.byRelFCounter		= 0;						//���ϼ�����
	Rel50IB2.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50IB2.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ��C������2��������Ԫ��                                       
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitRel50IC2()
{	
	Rel50IC2.pElecSet			= RS_pn50OLC2C;          			//��������������ֵ
	Rel50IC2.pTimeSet			= RS_pn50OLC2D;				 	//��������ʱ�䶨ֵ
	Rel50IC2.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50IC2.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	Rel50IC2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50IC2.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50IC2.pRelCFG			= CFG_pby50OLC2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50IC2.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50IC2.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC2.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC2.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50IC2.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50IC2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50IC2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50IC2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50IC2.pRelElec 			= &IHC;							//��ǰ����ָ��		
	Rel50IC2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50IC2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50IC2.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50IC2.pRel47UCFG		= &RelCFGCLR;;
	Rel50IC2.R47ULKRelayWord   = FALSE1;

	Rel50IC2.StartRelayWord		= RW_PI50OLC2;					//����������μ̵�����
	Rel50IC2.ActRelayWord		= RW_TR50OLC2;					//�������������̵�����
	Rel50IC2.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50IC2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	Rel50IC2.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50IC2.PRewardRelayWord   = FALSE1;						//���ʷ�����̵�����
	Rel50IC2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	Rel50IC2.LockRelayWord		= RW_LKPI50OLC2;					//��α����̵�����
	Rel50IC2.RSTRelayWord		= RW_LKRST50OLC2;				//���ر����̵�����
	Rel50IC2.LoopCtrlRelayWord	= RW_LPC50OLC2;					//����ѭ�����Ƽ̵�����
	
	Rel50IC2.wReportNum			= R_R50IC2;						//���������������
	Rel50IC2.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel50IC2.bMakeActRpt		= TRUE;
	Rel50IC2.dwK35				= NULL;
	Rel50IC2.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50IC2.wPickUpReportNum	= R_R50IC2_PI;						//��α������к�
	Rel50IC2.wResetReportNum	= R_R50IC2_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50IC2.dwRelTimer			= 0;			 			//��ʱ��
	Rel50IC2.byRelFCounter		= 0;						//���ϼ�����
	Rel50IC2.byRelRCounter		= 0;						//���ؼ�����	
	
	Rel50IC2.wExcepWordNum     = 8;
}
//******************************************************************************************
// ����: ��ʼ�������������Ԫ��                               
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitRel50IN()
{	
	Rel50I0.pElecSet			= RS_pn50NC;          			//��������������ֵ
	Rel50I0.pTimeSet			= RS_pn50ND;				 	//��������ʱ�䶨ֵ
	Rel50I0.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel50I0.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	Rel50I0.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel50I0.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I0.pRelCFG				= CFG_pby50N;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I0.pRelSW				= SW_pby50N;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I0.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel50I0.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel50I0.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel50I0.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel50I0.pRelElec 			= &I0;							//��ǰ����ָ��		
	Rel50I0.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel50I0.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel50I0.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel50I0.pRel47UCFG		= &RelCFGCLR;;
	Rel50I0.R47ULKRelayWord   = FALSE1;

	Rel50I0.StartRelayWord		= RW_PI50N;					//����������μ̵�����
	Rel50I0.ActRelayWord		= RW_TR50N;					//�������������̵�����
	Rel50I0.AccRelayWord		= FALSE1;						//���������̵�����
	Rel50I0.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	Rel50I0.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel50I0.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	Rel50I0.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	Rel50I0.LockRelayWord		= RW_LKPI50N;					//��α����̵�����
	Rel50I0.RSTRelayWord		= RW_LKRST50N;					//���ر����̵�����
	Rel50I0.LoopCtrlRelayWord	= RW_LPC50N;					//����ѭ�����Ƽ̵�����
	
	Rel50I0.wReportNum			= R_R50IN;						//���������������
	Rel50I0.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel50I0.bMakeActRpt			= TRUE;
	Rel50I0.dwK35				= NULL;
	Rel50I0.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel50I0.wPickUpReportNum	= R_R50IN_PI;						//��α������к�
	Rel50I0.wResetReportNum		= R_R50IN_RES;						//���ر������к�
	//**********************************************************************	
//	Rel50I0.dwRelTimer			= 0;			 			//��ʱ��
	Rel50I0.byRelFCounter		= 0;						//���ϼ�����
	Rel50I0.byRelRCounter		= 0;						//���ؼ�����
	
	Rel50I0.wExcepWordNum     = 8;
}
/*****************************************************************************************
*����: ��ʼ�������ѹ����Ԫ��
*����: ��
*����: ��
*��ע:
******************************************************************************************/
void CRelayTask::InitRel59UN()
{	
	Rel59U0.pElecSet			= RS_pn59NV;          			//��������������ֵ
	Rel59U0.pTimeSet			= RS_pn59ND;				 	//��������ʱ�䶨ֵ
	Rel59U0.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	Rel59U0.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	Rel59U0.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	Rel59U0.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel59U0.pRelCFG				= CFG_pby59N;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel59U0.pRelSW				= SW_pby59N;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel59U0.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U0.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U0.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U0.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel59U0.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	Rel59U0.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	Rel59U0.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	Rel59U0.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	Rel59U0.pRelElec 			= &U0;							//��ǰ����ָ��		
	Rel59U0.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	Rel59U0.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	Rel59U0.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	Rel59U0.pRel47UCFG		= &RelCFGCLR;;
	Rel59U0.R47ULKRelayWord   = FALSE1;

	Rel59U0.StartRelayWord		= RW_PI59N;					//����������μ̵�����
	Rel59U0.ActRelayWord		= RW_TR59N;					//�������������̵�����
	Rel59U0.AccRelayWord		= FALSE1;						//���������̵�����
	Rel59U0.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	Rel59U0.PForwardRelayWord	= FALSE1;						//����������̵�����
	Rel59U0.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	Rel59U0.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	Rel59U0.LockRelayWord		= RW_LKPI59N;					//��α����̵�����
	Rel59U0.RSTRelayWord		= RW_LKRST59N;					//���ر����̵�����
	Rel59U0.LoopCtrlRelayWord	= RW_LPC59N;					//����ѭ�����Ƽ̵�����
	
	Rel59U0.wReportNum			= R_R59UN;						//���������������
	Rel59U0.wAccReportNum		= 0xFFFF;						//���ٶ����������
	Rel59U0.bMakeActRpt			= TRUE;
	Rel59U0.dwK35				= NULL;
	Rel59U0.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************��η��ر��沿��*********************	yanxs  13-03-21
	Rel59U0.wPickUpReportNum	= R_R59UN_PI;						//��α������к�
	Rel59U0.wResetReportNum		= R_R59UN_RES;						//���ر������к�
	//**********************************************************************	
//	Rel59U0.dwRelTimer			= 0;			 			//��ʱ��
	Rel59U0.byRelFCounter		= 0;						//���ϼ�����
	Rel59U0.byRelRCounter		= 0;						//���ؼ�����
	
	Rel59U0.wExcepWordNum     = 8;
}
/************************************************************************************************
*����: ��ʼ����ѹ����Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRel27U()
{
	Rel27U.pELeSet						= RS_pn27HV;						//������ֵ 
	Rel27U.pTimeSet						= RS_pn27HD;						//ʱ�䶨ֵ 
	Rel27U.pLockCurSet					= RS_pn27H_LK50;					//����������ֵ 

	Rel27U.pRelCFG						= CFG_pby27;					  	 //Ƿ���������� 
	Rel27U.pRelSW						= SW_pby27;					  	 //Ƿ��������ѹ�� 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//Ƿ������Ͷ��բ 
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��
	Rel27U.pRelCurLockCFG				= &RelCFGSET;

	Rel27U.pRelElec						= &U_R27U;						//��ǰ�ĵ���ָ�� 
	Rel27U.pRelLockElec					= &I_R27U;					//�������� 
	Rel27U.ActRelayWord					= RW_TR27;					//Ƿ�����������̵����� 
	Rel27U.StartRelayWord				= RW_PI27;					//Ƿ��������μ̵����� 
	Rel27U.AlarmRelayWord				= FALSE1;					//Ƿ�������澯�̵����� 
	Rel27U.LockRelayWord				= RW_LKPI27;				//��α����̵����� 
	Rel27U.RSTRelayWord					= RW_LKRST27;					//���Ϸ��ر����̵����� 
	Rel27U.LoopCtrlRelayWord			= FALSE1;				//����ѭ�����Ƽ̵����� 
	Rel27U.wR52BFRealyword				= RW_52BFH;
	Rel27U.wActRptNum					= R_R27U;					//Ƿ����������������� 
	Rel27U.wAlarmRptNum					= 0xFFFF;					//Ƿ�������澯������� 
	Rel27U.wTRFailRptNum				= R_TR_H_FAIL;
	Rel27U.wPickUpRptNum				= R_ACTPICK;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
	
	Rel27U.R27RptSerialNum				= 0;
//	Rel27U.dwRelTimer					= 0;						//Ƿ��������ʱ�� 	
	Rel27U.byRelFCounter				= 0;					//���ϼ����� 
	Rel27U.byRelRCounter				= 0;					//���ؼ����� 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
/************************************************************************************************
*����: ��ʼ���߲�A��ʱ�޹����ɲ����ṹ
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitRel49IHA(void)
{	
	Rel49I_IA.p49KSet				= RS_pn49K;
	Rel49I_IA.p49TSet				= RS_pn49T;
	Rel49I_IA.p49ARSet				= RS_pn49AR;
	Rel49I_IA.p49KTSet				= RS_pn49KT;
	Rel49I_IA.pRatedInSet			= RS_pnTIN;
	Rel49I_IA.pRel49CFG				= CFG_pby49;
	Rel49I_IA.pRel49SW				= SW_pby49;
	Rel49I_IA.pICoef				= CF_pnIHA;
	Rel49I_IA.pIHACoef				= CF_pnIHA;
	Rel49I_IA.pCurrent				= IAP;
	
	Rel49I_IA.dwKIn					= 0;	
	Rel49I_IA.PreCurrent			= 0;			
	Rel49I_IA.l49OT					= 0;
	Rel49I_IA.l49KOT				= 0;
	Rel49I_IA.lExpT					= 0;
	Rel49I_IA.lExpKT				= 0;

	Rel49I_IA.R49WarnRW				= RW_AR49A;
	Rel49I_IA.R49ActRW				= RW_TR49A;
	Rel49I_IA.R49ActRptNum			= R_R49ACT;
	Rel49I_IA.R49WarnRptNum			= R_R49AR;
	Rel49I_IA.R49RSTRptNum			= R_R49RST;
	
	Rel49I_IA.dw49Warn				= 0;
	Rel49I_IA.dw49Warn_099          = 0;
	Rel49I_IA.dw49AThermaVal		= 0;
	Rel49I_IA.dw49AThermaPreVal		= 0;	
	Rel49I_IA.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
//	Rel49I_IA.dwRelTimer			= 0;								//Ƿ��������ʱ�� 
	
	Rel49I_IA.wExcepWordNum			= 16;
	InitThermalOverLoad(&Rel49I_IA);
}

/************************************************************************************************
*����: ��ʼ���߲�A��ʱ�޹����ɲ����ṹ
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitRel49IHB(void)
{	
	Rel49I_IB.p49KSet				= RS_pn49K;
	Rel49I_IB.p49TSet				= RS_pn49T;
	Rel49I_IB.p49ARSet				= RS_pn49AR;
	Rel49I_IB.p49KTSet				= RS_pn49KT;
	Rel49I_IB.pRatedInSet			= RS_pnTIN;
	Rel49I_IB.pRel49CFG				= CFG_pby49;
	Rel49I_IB.pRel49SW				= SW_pby49;
	Rel49I_IB.pICoef				= CF_pnIHB;
	Rel49I_IB.pIHACoef				= CF_pnIHA;
	Rel49I_IB.pCurrent				= IBP;
	
	
	Rel49I_IB.dwKIn					= 0;	
	Rel49I_IB.PreCurrent			= 0;			
	Rel49I_IB.l49OT					= 0;
	Rel49I_IB.l49KOT				= 0;
	Rel49I_IB.lExpT					= 0;
	Rel49I_IB.lExpKT				= 0;
	
	Rel49I_IB.R49WarnRW				= RW_AR49B;
	Rel49I_IB.R49ActRW				= RW_TR49B;
	Rel49I_IB.R49ActRptNum			= R_R49ACT;
	Rel49I_IB.R49WarnRptNum			= R_R49AR;
	Rel49I_IB.R49RSTRptNum			= R_R49RST;
	
	Rel49I_IB.dw49Warn				= 0;
	Rel49I_IB.dw49Warn_099          = 0;
	Rel49I_IB.dw49AThermaVal		= 0;
	Rel49I_IB.dw49AThermaPreVal		= 0;	
	Rel49I_IB.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
//	Rel49I_IB.dwRelTimer			= 0;								//Ƿ��������ʱ�� 
	Rel49I_IB.wExcepWordNum 		= 16;

	InitThermalOverLoad(&Rel49I_IB);
}
/************************************************************************************************
*����: ��ʼ���߲�A��ʱ�޹����ɲ����ṹ
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitRel49IHC(void)
{	
	Rel49I_IC.p49KSet				= RS_pn49K;
	Rel49I_IC.p49TSet				= RS_pn49T;
	Rel49I_IC.p49ARSet				= RS_pn49AR;
	Rel49I_IC.p49KTSet				= RS_pn49KT;
	Rel49I_IC.pRatedInSet			= RS_pnTIN;
	Rel49I_IC.pRel49CFG				= CFG_pby49;
	Rel49I_IC.pRel49SW				= SW_pby49;
	Rel49I_IC.pICoef				= CF_pnIHC;
	Rel49I_IC.pIHACoef				= CF_pnIHA;
	Rel49I_IC.pCurrent				= ICP;
		
	Rel49I_IC.dwKIn					= 0;	
	Rel49I_IC.PreCurrent			= 0;			
	Rel49I_IC.l49OT					= 0;
	Rel49I_IC.l49KOT				= 0;
	Rel49I_IC.lExpT					= 0;
	Rel49I_IC.lExpKT				= 0;
	
	Rel49I_IC.R49WarnRW				= RW_AR49C;
	Rel49I_IC.R49ActRW				= RW_TR49C;
	Rel49I_IC.R49ActRptNum			= R_R49ACT;
	Rel49I_IC.R49WarnRptNum			= R_R49AR;
	Rel49I_IC.R49RSTRptNum			= R_R49RST;
	
	Rel49I_IC.dw49Warn				= 0;
	Rel49I_IC.dw49Warn_099          = 0;
	Rel49I_IC.dw49AThermaVal		= 0;
	Rel49I_IC.dw49AThermaPreVal		= 0;	
	Rel49I_IC.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
//	Rel49I_IC.dwRelTimer			= 0;		
	Rel49I_IC.wExcepWordNum 		= 16;

	InitThermalOverLoad(&Rel49I_IC);
}
/************************************************************************************************
*����: ��ʼ��PT���߲����ṹ
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitRel74PT(void)
{
//--------------�߲�PT���߲����ṹ---------------------
	Rel74PTH.pRelPTBrokCFG = CFG_pby74PT;						//PT�������� 
	Rel74PTH.pR74PTU1Set = RS_pn74HPTV;						//PT���߼���ѹ��ֵ
	Rel74PTH.pR74PTU2Set = RS_pn74LPTV;						//PT���߼���ѹ��ֵ
	Rel74PTH.pR74PTISet = RS_pn74PTHC;						//pt���߼�������ֵ	
	Rel74PTH.pR74PTTSet = RS_pn74PTD;						//pt���߼��ʱ�䶨ֵ
	
	Rel74PTH.pRelCur = &I_R27U;							//�ж��õ��� 
	Rel74PTH.pRelU1	= &PhaseUHMin;							//�ж��õ�һ·��ѹ 	
	Rel74PTH.pRelU2 = &PhaseULMax;	
	

	Rel74PTH.StartRelayWord = RW_PI74PTH;						//PT������μ̵����� 
	Rel74PTH.ActRelayWord	= RW_AR74PTH;						//PT���߶����̵����� 
	Rel74PTH.LKPIRelayWord	= RW_LKPIPTH;
	Rel74PTH.dwSysTimer		= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel74PTH.wRelRptNum		= R_RHPTBR;						//PT���߶��������
	Rel74PTH.wRelResRptNum	= R_RHPTBRRST;					//PT���߷��ر����
//	Rel74PTH.dwRelTimer		= 0;							//PT���߶�ʱ�� 	
	Rel74PTH.wExcepWordNum  = 5;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6 

//--------------�Ͳ�A��PT���߲����ṹ---------------------
	Rel74PTLA.pRelPTBrokCFG = CFG_pby74PT;						//PT�������� 
	Rel74PTLA.pR74PTU1Set = RS_pn74LPTV;						//PT���߼���ѹ��ֵ
	Rel74PTLA.pR74PTU2Set = RS_pn74HPTV;						//PT���߼���ѹ��ֵ
	Rel74PTLA.pR74PTISet = RS_pn74PTLC;						//pt���߼�������ֵ	
	Rel74PTLA.pR74PTTSet = RS_pn74PTD;						//pt���߼��ʱ�䶨ֵ

	Rel74PTLA.pRelCur = &ILA;							//�ж��õ��� 
	Rel74PTLA.pRelU1 = &ULA;							//�ж��õ�һ·��ѹ	
	Rel74PTLA.pRelU2 = &LineUHMax;	

	Rel74PTLA.StartRelayWord = RW_PI74PTLA;						//PT������μ̵����� 
	Rel74PTLA.ActRelayWord	= RW_AR74PTLA;						//PT���߶����̵�����	
	Rel74PTLA.LKPIRelayWord = RW_LKPIPTLA;
	Rel74PTLA.dwSysTimer 	= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel74PTLA.wRelRptNum 	= R_RLAPTBR; 					//PT���߶��������
	Rel74PTLA.wRelResRptNum	= R_RLAPTBRRST;					//PT���߷��ر����
//	Rel74PTLA.dwRelTimer 	= 0;							//PT���߶�ʱ��	
	Rel74PTLA.wExcepWordNum	= 5;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6 

//--------------�Ͳ�B��PT���߲����ṹ---------------------
	Rel74PTLB.pRelPTBrokCFG = CFG_pby74PT; 					//PT�������� 
	Rel74PTLB.pR74PTU1Set = RS_pn74LPTV; 					//PT���߼���ѹ��ֵ
	Rel74PTLB.pR74PTU2Set = RS_pn74HPTV; 					//PT���߼���ѹ��ֵ
	Rel74PTLB.pR74PTISet = RS_pn74PTLC; 					//pt���߼�������ֵ	
	Rel74PTLB.pR74PTTSet = RS_pn74PTD;						//pt���߼��ʱ�䶨ֵ

	Rel74PTLB.pRelCur = &ILB;							//�ж��õ��� 
	Rel74PTLB.pRelU1 = &ULB; 						//�ж��õ�һ·��ѹ	
	Rel74PTLB.pRelU2 = &LineUHMax;	

	Rel74PTLB.StartRelayWord = RW_PI74PTLB; 					//PT������μ̵����� 
	Rel74PTLB.ActRelayWord	= RW_AR74PTLB; 					//PT���߶����̵�����	
	Rel74PTLB.LKPIRelayWord = RW_LKPIPTLB;
	Rel74PTLB.dwSysTimer	= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel74PTLB.wRelRptNum	= R_RLBPTBR;					//PT���߶��������
	Rel74PTLB.wRelResRptNum	= R_RLBPTBRRST;					//PT���߷��ر����
//	Rel74PTLB.dwRelTimer	= 0;							//PT���߶�ʱ��	
	Rel74PTLB.wExcepWordNum = 5;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6 
}

void CRelayTask::InitStructCheck()
{
	WORD acc = 0x55aa;
	
	StructCheck[0].pstruct = (WORD*)&Rel50I_IA;
	StructCheck[0].bigsize = sizeof(Rel50I_IA)/2 - Rel50I_IA.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&Rel50I_IB;
	StructCheck[1].bigsize = sizeof(Rel50I_IB)/2 - Rel50I_IB.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel50I_IC;
	StructCheck[2].bigsize = sizeof(Rel50I_IC)/2 - Rel50I_IC.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel50I_Ia1;
	StructCheck[3].bigsize = sizeof(Rel50I_Ia1)/2 - Rel50I_Ia1.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel50I_Ia2;
	StructCheck[4].bigsize = sizeof(Rel50I_Ia2)/2 - Rel50I_Ia2.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel50I_Ib1;
	StructCheck[5].bigsize = sizeof(Rel50I_Ib1)/2 - Rel50I_Ib1.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel50I_Ib2;
	StructCheck[6].bigsize = sizeof(Rel50I_Ib2)/2 - Rel50I_Ib2.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel50I_JX;
	StructCheck[7].bigsize = sizeof(Rel50I_JX)/2 - Rel50I_JX.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&Rel49I_IA;
	StructCheck[8].bigsize = sizeof(Rel49I_IA)/2 - Rel49I_IA.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&Rel49I_IB;
	StructCheck[9].bigsize = sizeof(Rel49I_IB)/2 - Rel49I_IB.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&Rel49I_IC;
	StructCheck[10].bigsize = sizeof(Rel49I_IC)/2 - Rel49I_IC.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)&Rel50IA1;
	StructCheck[11].bigsize = sizeof(Rel50IA1)/2 - Rel50IA1.wExcepWordNum;
	StructCheck[12].pstruct = (WORD*)&Rel50IA2;
	StructCheck[12].bigsize = sizeof(Rel50IA2)/2 - Rel50IA2.wExcepWordNum;
	StructCheck[13].pstruct = (WORD*)&Rel50IB1;
	StructCheck[13].bigsize = sizeof(Rel50IB1)/2 - Rel50IB1.wExcepWordNum;
	StructCheck[14].pstruct = (WORD*)&Rel50IB2;
	StructCheck[14].bigsize = sizeof(Rel50IB2)/2 - Rel50IB2.wExcepWordNum;
	StructCheck[15].pstruct = (WORD*)&Rel50IC1;
	StructCheck[15].bigsize = sizeof(Rel50IC1)/2 - Rel50IC1.wExcepWordNum;
	StructCheck[16].pstruct = (WORD*)&Rel50IC2;
	StructCheck[16].bigsize = sizeof(Rel50IC2)/2 - Rel50IC2.wExcepWordNum;
	StructCheck[17].pstruct = (WORD*)&Rel50I0;
	StructCheck[17].bigsize = sizeof(Rel50I0)/2 - Rel50I0.wExcepWordNum;
	StructCheck[18].pstruct = (WORD*)&Rel59U0;
	StructCheck[18].bigsize = sizeof(Rel59U0)/2 - Rel59U0.wExcepWordNum;
	StructCheck[19].pstruct = (WORD*)&Rel27U;
	StructCheck[19].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;
	StructCheck[20].pstruct = (WORD*)&Rel74PTH;
	StructCheck[20].bigsize = sizeof(Rel74PTH)/2 - Rel74PTH.wExcepWordNum;
	StructCheck[21].pstruct = (WORD*)&Rel74PTLA;
	StructCheck[21].bigsize = sizeof(Rel74PTLA)/2 - Rel74PTLA.wExcepWordNum;
	StructCheck[22].pstruct = (WORD*)&Rel74PTLB;
	StructCheck[22].bigsize = sizeof(Rel74PTLB)/2 - Rel74PTLB.wExcepWordNum;
	StructCheck[23].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[23].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
{//	ReportMsg("Relay Task Recieved SETTING change message! Type=%d Index=%d",dwDataType,dwIndex);
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

//------------------------------------------------------------------------------------------ 
// ����: ģֵ�����б����							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	if(ReadRelayWord(RW_52BFH))
		return FALSE;
	
	if((*Rel50I_IA.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IA.pRelSW == SW_STATUS_SET))   //�߲�A����
	{
		if(Rel50I_IA.pRelElec->Mod > *Rel50I_IA.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}	
	if((*Rel50I_IB.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IB.pRelSW == SW_STATUS_SET))   //�߲�B����
	{
		if(Rel50I_IB.pRelElec->Mod > *Rel50I_IB.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	
	if((*Rel50I_IC.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IC.pRelSW == SW_STATUS_SET))   //�߲�C����
	{
		if(Rel50I_IC.pRelElec->Mod > *Rel50I_IC.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	
	if((*Rel50I_JX.pRelCFG == CFG_STATUS_SET) && (*Rel50I_JX.pRelSW == SW_STATUS_SET))   //��϶����
	{
		if(Rel50I_JX.pRelElec1->Mod > *Rel50I_JX.pElec1Set)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50IA2.pRelCFG == CFG_STATUS_SET) && (*Rel50IA2.pRelSW == SW_STATUS_SET))   //�߲�A������
	{
		if(Rel50IA2.pRelElec->Mod > *Rel50IA2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50IB2.pRelCFG == CFG_STATUS_SET) && (*Rel50IB2.pRelSW == SW_STATUS_SET))   //�߲�B������
	{
		if(Rel50IB2.pRelElec->Mod > *Rel50IB2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50IC2.pRelCFG == CFG_STATUS_SET) && (*Rel50IC2.pRelSW == SW_STATUS_SET))   //�߲�C������
	{
		if(Rel50IC2.pRelElec->Mod > *Rel50IC2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET))   //����
	{
		if(Rel50I0.pRelElec->Mod > *Rel50I0.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel59U0.pRelCFG == CFG_STATUS_SET) && (*Rel59U0.pRelSW == SW_STATUS_SET))   //��ѹ
	{
		if(Rel59U0.pRelElec->Mod > *Rel59U0.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if(!ReadRelayWord(RW_52BFLA))
	{
		if((*Rel50I_Ia1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ia1.pRelSW == SW_STATUS_SET))   //�Ͳ�a����
		{
			if(Rel50I_Ia1.pRelElec->Mod > *Rel50I_Ia1.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
		
		if((*Rel50I_Ia2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ia2.pRelSW == SW_STATUS_SET))   //�Ͳ�a����
		{
			if(Rel50I_Ia2.pRelElec->Mod > *Rel50I_Ia2.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
	}
	
	if(!ReadRelayWord(RW_52BFLB))
	{
		if((*Rel50I_Ib1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ib1.pRelSW == SW_STATUS_SET))   //�Ͳ�b����
		{
			if(Rel50I_Ib1.pRelElec->Mod > *Rel50I_Ib1.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
		
		if((*Rel50I_Ib2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ib2.pRelSW == SW_STATUS_SET))   //�Ͳ�b����
		{
			if(Rel50I_Ib2.pRelElec->Mod > *Rel50I_Ib2.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
	}
	
	if(((*Rel50I_IA.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IA.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_IB.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_IB.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_IC.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_IC.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_Ia1.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ia1.pRelSW == SW_STATUS_SET))||
		((*Rel50I_Ib1.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ib1.pRelSW == SW_STATUS_SET))||
		((*Rel50I_Ia2.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ia2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_Ib2.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ib2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_JX.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_JX.pRelSW == SW_STATUS_SET))|| \
		((*Rel50IA2.pRelCFG == CFG_STATUS_SET)&& (*Rel50IA2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50IB2.pRelCFG == CFG_STATUS_SET)&& (*Rel50IB2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50IC2.pRelCFG == CFG_STATUS_SET)&& (*Rel50IC2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I0.pRelCFG == CFG_STATUS_SET)&& (*Rel50I0.pRelSW == SW_STATUS_SET))|| \
		((*Rel59U0.pRelCFG == CFG_STATUS_SET)&& (*Rel59U0.pRelSW == SW_STATUS_SET)))
	
	{
		BYTE  pChannel[]={CHANNEL_6531_IHA,CHANNEL_6531_IHB,CHANNEL_6531_IHC,CHANNEL_6531_I0,CHANNEL_6531_Ijx,CHANNEL_6531_ILA,CHANNEL_6531_ILB};
		ReadPreSingleData(dwPickDataBuf,pChannel,7,RELAY_SAMP_INTERVAL);		
		//�㷨1�����ݺ�RELAY_SAMP_INTERVALȷ����ͨ���㷨
		//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
		//ע��:RELAY_SAMP_INTERVAL��ֵ����С��3
		for(LONG j=0;j<7;j++)
		{
			m_wPickCheck = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickDataBuf[i+j*3*RELAY_SAMP_INTERVAL];
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
						PickTime += PickUpTime.Lo-(i*625+500)/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_PICKUP, RELAY_ON);
						//ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
						m_wRptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
						m_wRptSerialNumOfStart = g_RptSerialNum;
						m_wPickCheck =0;
						for(LONG m=0;m<7;m++)
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
							PickTime += PickUpTime.Lo-(i*625+500)/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_PICKUP, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
							m_wRptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
							m_wRptSerialNumOfStart = g_RptSerialNum;
							m_wPickCheck =0;
							for(LONG m=0;m<7;m++)
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
	if(m_bFaultOnFlag ==FALSE)   						//��δ������ϴ���
	{		
		RelayPickUpCHK();					//ִ���������
		if(ReadRelayWord(RW_PICKUP))
		{
			ResetAllRelay(TRUE);
			m_dwFaultSINum=RELAY_SAMP_INTERVAL;       //������������ʼ�����ϴ���������������
			m_bFaultOnFlag=TRUE;
			return;
		}
		else
			return;		
	}
	m_dwFaultSINum += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(m_dwFaultSINum<READ_POINT_NUM)					//���ϴ���������������С��һ�ܲ���������
	{
		return;
	}	
	else
	{
		if(m_bHaveMakeStartRptFlag == FALSE)   //������ϵ���������������д����ϵ���
		{
			MakeStartRpt(R_PICKUP,m_wRptSerialNumOfStart);
			m_bHaveMakeStartRptFlag = TRUE;
		}
				
		GeneralOverValueRelay(&Rel50I_IA);				//�����ٶ��ӳ���A
		GeneralOverValueRelay(&Rel50I_IB);				//�����ٶ��ӳ���B
		GeneralOverValueRelay(&Rel50I_IC);				//�����ٶ��ӳ���C
		if(!ReadRelayWord(RW_52BFLA))
		{
			GeneralOverValueRelay(&Rel50I_Ia1);				//�����ٶ��ӳ���		
			GeneralOverValueRelay(&Rel50I_Ia2);				//�����ٶ��ӳ���
		}
		if(!ReadRelayWord(RW_52BFLB))
		{
			GeneralOverValueRelay(&Rel50I_Ib1);				//�����ٶ��ӳ���		
			GeneralOverValueRelay(&Rel50I_Ib2);				//�����ٶ��ӳ���
		}
		GeneralOverValueRelay_3(&Rel50I_JX);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel50IA2);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel50IB2);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel50IC2);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel50I0);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel59U0);				//�����ٶ��ӳ���

		if(ReadRelayWord(RW_52BFH))
		{
			if((!m_bHaveMake52BF_H_RptFlag)&&(!m_b52BFRecover_H_FLAG))
			{
				m_b52BFRecover_H_FLAG = TRUE;
				MakeActRpt(R_TR_H_FAIL,m_wRptSerialNumOfAct);		//����·����բʧ�ܱ���
				m_bHaveMake52BF_H_RptFlag = TRUE;
			}
		}
		if(ReadRelayWord(RW_52BFLA))
		{
			if((!m_bHaveMake52BF_LA_RptFlag)&&(!m_b52BFRecover_LA_FLAG))
			{
				m_b52BFRecover_LA_FLAG = TRUE;
				MakeActRpt(R_TR_LA_FAIL,m_wRptSerialNumOfAct);		//����·����բʧ�ܱ���
				m_bHaveMake52BF_LA_RptFlag = TRUE;
			}
		}
		if(ReadRelayWord(RW_52BFLB))
		{
			if((!m_bHaveMake52BF_LB_RptFlag)&&(!m_b52BFRecover_LB_FLAG))
			{
				m_b52BFRecover_LB_FLAG = TRUE;
				MakeActRpt(R_TR_LB_FAIL,m_wRptSerialNumOfAct);		//����·����բʧ�ܱ���
				m_bHaveMake52BF_LB_RptFlag = TRUE;
			}
		}
		
		// 1���б���ѭ�����Ƽ̵����ֱ���λ���ȴ��������������˳�
		if(ReadRelayWord(Rel50I_IA.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IB.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IC.LoopCtrlRelayWord) \
			||((!ReadRelayWord(RW_52BFLA))&&(ReadRelayWord(Rel50I_Ia1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ia2.LoopCtrlRelayWord)))
			||((!ReadRelayWord(RW_52BFLB))&&(ReadRelayWord(Rel50I_Ib1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ib2.LoopCtrlRelayWord))) \
			||ReadRelayWord(Rel50I_JX.LoopCtrlRelayWord)||ReadRelayWord(Rel50IA2.LoopCtrlRelayWord)||ReadRelayWord(Rel59U0.LoopCtrlRelayWord)  \
			||ReadRelayWord(Rel50IB2.LoopCtrlRelayWord)||ReadRelayWord(Rel50IC2.LoopCtrlRelayWord)||ReadRelayWord(Rel50I0.LoopCtrlRelayWord)) \
		{
			if(ReadRelayWord(RW_52BFH))
			{				
				WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
				m_bFaultCheckOutFlag = FALSE;					//�������ϱ�־
				m_bFaultOnFlag = FALSE;						//�����ѭ�������־
				m_bHaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
				m_bFaultLKFlag = FALSE; 					//���������޶���ʱ����ʱ��������־	
				m_bHaveMake52BF_H_RptFlag = FALSE;
				m_bHaveMake52BF_LA_RptFlag = FALSE;
				m_bHaveMake52BF_LB_RptFlag = FALSE;
				DB_RelayEnd(m_wRptSerialNumOfAct); 			//�����ݿ���д���������
				return; 				
			}	
			//�Ͳ��·����բʧ��
			if(ReadRelayWord(RW_52BFLA)||ReadRelayWord(RW_52BFLB))  
			{
				//�и�ѹ��ı������
				if(ReadRelayWord(Rel50I_IA.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IB.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IC.LoopCtrlRelayWord) \
					||ReadRelayWord(Rel50I_JX.LoopCtrlRelayWord)||ReadRelayWord(Rel50IA2.LoopCtrlRelayWord)||ReadRelayWord(Rel59U0.LoopCtrlRelayWord)  \
					||ReadRelayWord(Rel50IB2.LoopCtrlRelayWord)||ReadRelayWord(Rel50IC2.LoopCtrlRelayWord)||ReadRelayWord(Rel50I0.LoopCtrlRelayWord) \
					||((!ReadRelayWord(RW_52BFLA))&&(ReadRelayWord(Rel50I_Ia1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ia2.LoopCtrlRelayWord))) \
					||((!ReadRelayWord(RW_52BFLB))&&(ReadRelayWord(Rel50I_Ib1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ib2.LoopCtrlRelayWord))))
				{					
					m_dwFaultLockTime = dwRelayMSCnt;		
					m_bFaultCheckOutFlag = TRUE;
					return;
				}
				else
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					m_bFaultCheckOutFlag = FALSE;					//�������ϱ�־
					m_bFaultOnFlag = FALSE; 					//�����ѭ�������־
					m_bHaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
					m_bFaultLKFlag = FALSE; 					//���������޶���ʱ����ʱ��������־						
					m_bHaveMake52BF_H_RptFlag = FALSE;
					m_bHaveMake52BF_LA_RptFlag = FALSE;
					m_bHaveMake52BF_LB_RptFlag = FALSE;
					DB_RelayEnd(m_wRptSerialNumOfAct);			//�����ݿ���д���������
					return; 				
				}				
			}
			m_dwFaultLockTime = dwRelayMSCnt;		
			m_bFaultCheckOutFlag = TRUE;
			return;
		}
		// 2���ޱ���ѭ�����Ƽ̵����ֱ���λ����ʱ�����Ѷ���������û�������κα�����������
		else				
		{
			// 2.1 ������ϱ�־����λ����ʾ����ѭ�����Ƽ̵�������������λ--��һ��������--�ȴ�RS_pnSOTFTʱ����˳�
			if(m_bFaultCheckOutFlag == TRUE)     
			{
				if((ReadRelayWord(RW_T52BFH))||(ReadRelayWord(RW_T52BFLA))||(ReadRelayWord(RW_T52BFLB)))
				{
					m_dwFaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -m_dwFaultLockTime > 1000 )  //�޹���ʱ�䳬�����ڹ��ϼ��ʱ��
				{						
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					m_bFaultCheckOutFlag = FALSE;					//�������ϱ�־
					m_bFaultOnFlag = FALSE;						//�����ѭ�������־
					m_bHaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					DB_RelayEnd(m_wRptSerialNumOfAct);				//�����ݿ���д���������
					ResetAllRelay(TRUE);
					m_bHaveMake52BF_H_RptFlag = FALSE;
					m_bHaveMake52BF_LA_RptFlag = FALSE;
					m_bHaveMake52BF_LB_RptFlag = FALSE;
					return;					
				}
				else
					return;
			}
			// 2.2 ������ϱ�־δ����λ����ʾ����ѭ�����Ƽ̵������ڱ��ι��ϴ����д�δ����λ���ȴ�0.5����˳�
			else
			{
				
				if(m_bFaultLKFlag == FALSE)//�����ո���������¼��ʼʱ��
				{	
					m_dwFaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				if(dwRelayMSCnt -m_dwFaultLockTime>500 )	
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//�屣�������̵�����
					m_bFaultCheckOutFlag = FALSE;					//�������ϱ�־
					m_bFaultOnFlag = FALSE;						//�����ѭ�������־
					m_bHaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־						
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					DB_RelayEnd(m_wRptSerialNumOfAct); 
					ResetAllRelay(TRUE);
					m_bHaveMake52BF_H_RptFlag = FALSE;
					m_bHaveMake52BF_LA_RptFlag = FALSE;
					m_bHaveMake52BF_LB_RptFlag = FALSE;
					return;					
				}
				else	//��ʱδ����ֱ�ӷ���
					return;
			}			
		}		
	}
}
//------------------------------------------------------------------------------------------
// ����: ��·����բʧ�ܸ��鴦��						                              
// ����: ��                                                                                
// ���:                                                         
// ����: ��                                                                                
//------------------------------------------------------------------------------------------
void CRelayTask::Process52BF()
{
	if(ReadRelayWord(RW_52BFH))
	{
		m_b52BFHFlag = TRUE;
	}
	if(m_b52BFHFlag)
	{
		if(!ReadRelayWord(RW_52BFH))
		{			
			m_b52BFRecover_H_FLAG = FALSE;
			m_b52BFHFlag = FALSE;
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&Rel27U);			
		}
	}

	if(ReadRelayWord(RW_52BFLA))
	{
		m_b52BFLAFlag = TRUE;
	}
	if(m_b52BFLAFlag)
	{
		if(!ReadRelayWord(RW_52BFLA))
		{			
			m_b52BFRecover_LA_FLAG = FALSE;
			m_b52BFLAFlag = FALSE;
			ResetOverValueRelay(&Rel50I_Ia1,TRUE);
			ResetOverValueRelay(&Rel50I_Ia2,TRUE);
		}
	}

	if(ReadRelayWord(RW_52BFLB))
	{
		m_b52BFLBFlag = TRUE;
	}
	if(m_b52BFLBFlag)
	{
		if(!ReadRelayWord(RW_52BFLB))
		{
			m_b52BFRecover_LB_FLAG = FALSE;
			m_b52BFLBFlag = FALSE;
			ResetOverValueRelay(&Rel50I_Ib1,TRUE);
			ResetOverValueRelay(&Rel50I_Ib2,TRUE);
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
	ResetOverValueRelay(&Rel50I_IA,resetFlag);
	ResetOverValueRelay(&Rel50I_IB,resetFlag);
	ResetOverValueRelay(&Rel50I_IC,resetFlag);
	if(!ReadRelayWord(RW_52BFLA))
	{
		ResetOverValueRelay(&Rel50I_Ia1,resetFlag);
		ResetOverValueRelay(&Rel50I_Ia2,resetFlag);
	}
	if(!ReadRelayWord(RW_52BFLB))
	{
		ResetOverValueRelay(&Rel50I_Ib1,resetFlag);
		ResetOverValueRelay(&Rel50I_Ib2,resetFlag);
	}
	ResetOverValueRelay_3(&Rel50I_JX,resetFlag);
	ResetOverValueRelay(&Rel50IA2,resetFlag);
	ResetOverValueRelay(&Rel50IB2,resetFlag);
	ResetOverValueRelay(&Rel50IC2,resetFlag);
	ResetOverValueRelay(&Rel50I0,resetFlag);
	ResetOverValueRelay(&Rel59U0,resetFlag);
}
void CRelayTask::PTCheck()
{	
	CheckPTBrok_2U(&Rel74PTH);
	CheckPTBrok_2U(&Rel74PTLA);
	CheckPTBrok_2U(&Rel74PTLB);
}
TRelElecValPar& CRelayTask::MaxElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C)
{
	if(A.Mod>=B.Mod)
	{
		if(A.Mod>=C.Mod)
			return A;
		else
			return C;
	}
	else
	{
		if(B.Mod>=C.Mod)
			return B;
		else
			return C;
	}
}
TRelElecValPar& CRelayTask::MinElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C)
{
	if(A.Mod>=B.Mod)
	{
		if(B.Mod>=C.Mod)
			return C;
		else
			return B;
	}
	else
	{
		if(A.Mod>=C.Mod)
			return C;
		else
			return A;
	}
}

