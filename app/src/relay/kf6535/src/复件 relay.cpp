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
*           6530��ѹ�������㷨����                                                                       
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
*      yanzh             2010/06/07    ��ʼ����                         
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

extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern g_hDbaseTaskID;
extern dwRelayDataPtr;	//AD��ǰдָ��
extern DWORD dwRelayMSCnt;		//�����ú��������
extern BYTE g_RptSerialNum;
extern BOOL RelayCheckErr;			//�����Լ����
extern BOOL RelayCheckInitOK;		//�����Լ�ṹ���ѳ�ʼ��������־
extern DWORD dwADResumeCnt;	//AD���ָܻ����������쳣���������豣֤��������һ�������ܲ�����
extern BYTE *g_MaintSW;

BOOL RelayTaskInitOK = FALSE;
DWORD dwRelayTaskID;

DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
TStructCheck StructCheck[CHECK_NUM];					//�ṹ��У������

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

	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}
	//��ȡ���ݿⱣ������ֵ����ָ�룬�Լӿ�����ٶ�
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	InitAllRelayMem();				//��ʼ�������ڲ�����
	InitRealy();					//��ʼ������Ԫ��
	InitSet();
	InitDCRecInfo();
	

	RelCFGCLR = CFG_STATUS_CLR;		//�����˳�
	RelCFGSET = CFG_STATUS_SET;		//����Ͷ��
	
	g_RptSerialNum = 0;				//ϵͳ�ı������к�
	RptSerialNumOfAct =0;			//ϵͳ�ı��������������к�
	RptSerialNumOfStart=0;			//ϵͳ�Ĵ����ϵ����������������к�	
	FaultSINum = 0;					//�������������	
	
	PickUpRW = RW_PICKUP;			//ģֵͻ���������̵�����
	FaultOnFlag = FALSE;			//������ϴ����־
	HaveMakeStartRptFlag = FALSE;	//���������ϵ������������־
	m_bFaultLKFlag = FALSE;			//������ʼʱ��������־
	FaultCheckOutFlag = FALSE;		//������ϱ�־
	m_bLastRecStatusFlag = FALSE;	//����¼�������������жϱ�־	
	m_bHaveTripFailedRpt = FALSE;	//������·����բʧ�ܱ����־
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

	HPTCheckTimer1 = 0;
	HPTCheckTimer2 = 0;
	HPTCheckTimer3 = 0;
	HPTCheckTimer4 = 0;	
	HPTMakeReportFlag = FALSE;
	U2Set = 8*(*CF_pnUp)/100; //�����ѹ8V
	
	m_b52BFFlag = FALSE;
	
	PickCheck = 0;					//ͻ���������ж������������
	for(LONG i=0;i<3;i++)			//ͻ���������ж���������������������
	{		
		PickCheckNext[i] = 0;	
		lPickUpValueArray[i]=0;
	}
	for(LONG i=0;i<6;i++)			//����¼�����ּ���������
	{
		DCCaluResult[i]=0;
	}
	for(LONG i=0;i<5;i++)
	{
		IDADrift[i]=0;
		IDBDrift[i]=0;
		IDCDrift[i]=0;
	}		
	//�������ݶ�ȡͨ������
	dwChanDataMask = ( (0x01<<CHANNEL_6535_UA)|(0x01<<CHANNEL_6535_UB)|(0x01<<CHANNEL_6535_UC)|
						(0x01<<CHANNEL_6535_IHA)|(0x01<<CHANNEL_6535_IHB)|(0x01<<CHANNEL_6535_IHC)| \
						(0x01<<CHANNEL_6535_IMA)|(0x01<<CHANNEL_6535_IMB)|(0x01<<CHANNEL_6535_IMC)| \
						(0x01<<CHANNEL_6535_ILA)|(0x01<<CHANNEL_6535_ILB)|(0x01<<CHANNEL_6535_ILC));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	dwComposeDataMask = ((0x01<<CHANNEL_6535_IDA)|(0x01<<CHANNEL_6535_IDB)|(0x01<<CHANNEL_6535_IDC)| \
						(0x01<<CHANNEL_6535_IRA_1)|(0x01<<CHANNEL_6535_IRA_2)|(0x01<<CHANNEL_6535_IRA_3)| \
						(0x01<<CHANNEL_6535_IRB_1)|(0x01<<CHANNEL_6535_IRB_2)|(0x01<<CHANNEL_6535_IRB_3)| \
						(0x01<<CHANNEL_6535_IRC_1)|(0x01<<CHANNEL_6535_IRC_2)|(0x01<<CHANNEL_6535_IRC_3));
	//��ʼ����ȡ����������ʱ������
	//�������ݻ�����
	
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);	
	//�ϳ����ݻ�д������
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*12);
	//ͻ����������ʷ���ݻ�����
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*12);
	//�ϳɼ��������ݻ�����
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);
	//�ߵ�ѹ���������ݻ�����
	dwLineUDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	//���ʷ���������ݻ�����	
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(DWORD)*READ_POINT_NUM*12);

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*12);


	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*12);
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*12);
	
	ASSERT(dwLineUDataBuf != NULL);
	if(dwLineUDataBuf == NULL)
	{
		LogError("CRelayTask,dwLineUDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwLineUDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*3);
	
	//�����ǳ�ʼ���߼����̲���
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

	g_MaintSW = SW_pbyMAINT;
	RelayTaskInitOK = TRUE;
	m_bLogicChanged = FALSE;
	m_PTCheckTimer = 20*RELAY_SAMP_INTERVAL/POINT_PER_PERIOD;
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
	//--------------��ʼ���ϳ�ͨ��ϵ����������-----------------
	if((*RS_pnWCTHCON>2)||(*RS_pnWCTHCON<0)||(*RS_pnWCTMCON>2)||(*RS_pnWCTMCON<0) \
		||(*RS_pnWCTLCON>2)||(RS_pnWCTLCON<0)||(*RS_pnWHCTC>12)||(*RS_pnWHCTC<0) \
		||(*RS_pnWMCTC>12)||(*RS_pnWMCTC<0)||(*RS_pnWLCTC>12)||(*RS_pnWLCTC<0))
	{
		for(WORD i=0;i<9;i++)
		{
			CoefCorH[i] = 0;
			CoefCorM[i] = 0;
			CoefCorL[i] = 0;
		}
		RelayCheckErr = TRUE;
		return;
	}
	for(WORD i=0;i<9;i++)
	{
		CoefCorH[i] = Martix6535[*RS_pnWHCTC][i];
		//*********************ѡȡ��У����ʽ������/CT���ӷ�ʽ����
		if(*CFG_pbyMAB == CFG_STATUS_SET)
			CoefCorM[i] = Martix6535_AB[*RS_pnMBph][i]*WCT6535[*RS_pnWCTHCON][1]/1000;
		else
			CoefCorM[i] = Martix6535[*RS_pnWMCTC][i]*WCT6535[*RS_pnWCTHCON][*RS_pnWCTMCON]/1000;
		
		if(*CFG_pbyLAB == CFG_STATUS_SET)
			CoefCorL[i] = Martix6535_AB[*RS_pnLBph][i]*WCT6535[*RS_pnWCTHCON][1]/1000;
		else
			CoefCorL[i] = Martix6535[*RS_pnWLCTC][i]*WCT6535[*RS_pnWCTHCON][*RS_pnWCTLCON]/1000;
	}
	//*********************�е�ѹ���ٳ�ƽ��ϵ��
	CoefCorM[0]=(*RS_pnKma)*CoefCorM[0]/100;
	CoefCorM[1]=(*RS_pnKma)*CoefCorM[1]/100;
	CoefCorM[2]=(*RS_pnKma)*CoefCorM[2]/100;
	CoefCorM[3]=(*RS_pnKmb)*CoefCorM[3]/100;
	CoefCorM[4]=(*RS_pnKmb)*CoefCorM[4]/100;
	CoefCorM[5]=(*RS_pnKmb)*CoefCorM[5]/100;
	CoefCorM[6]=(*RS_pnKmc)*CoefCorM[6]/100;
	CoefCorM[7]=(*RS_pnKmc)*CoefCorM[7]/100;
	CoefCorM[8]=(*RS_pnKmc)*CoefCorM[8]/100;
	
	CoefCorL[0]=(*RS_pnKla)*CoefCorL[0]/100;
	CoefCorL[1]=(*RS_pnKla)*CoefCorL[1]/100;
	CoefCorL[2]=(*RS_pnKla)*CoefCorL[2]/100;
	CoefCorL[3]=(*RS_pnKlb)*CoefCorL[3]/100;
	CoefCorL[4]=(*RS_pnKlb)*CoefCorL[4]/100;
	CoefCorL[5]=(*RS_pnKlb)*CoefCorL[5]/100;
	CoefCorL[6]=(*RS_pnKlc)*CoefCorL[6]/100;
	CoefCorL[7]=(*RS_pnKlc)*CoefCorL[7]/100;
	CoefCorL[8]=(*RS_pnKlc)*CoefCorL[8]/100;
}
/************************************************************************************************
*����: ��ʼ������Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	InitR87U_A();
	InitR87U_B();
	InitR87U_C();
	InitR87R_A();
	InitR87R_B();
	InitR87R_C();
	InitR50IH1();
	InitR50IH2();
	InitR50IM1();
	InitR50IM2();
	InitR50IL1();
	InitR50IL2();
	InitR50OL1();
	InitR50OL2();
	InitR50OL3();
	InitR27U();
	InitR87IAlarm();
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
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
	
	static WORD wInvalidCheckCnt;		//�����Լ��ۼӼ�����
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}

//	if(!m_bLogicChanged)
//		BoBreakCheck();

	ReadSampleData(dwSampBuf,dwChanDataMask,0);	
	if(FALSE == WriteComposeBuf())
		return;	
	CaluUI(); 
	CaluMaxMin();
	CaluR87();
	CaluDrift();
	R87IAlarm(&R87Alarm);
	//���Ƚ��п����Լ�,ÿ�α��������һ·����
	//����ѭ������
	FalutProcess();		
	GeneralUnderValRelay(&Rel27U);					//�͵�ѹ�����ӳ���	
	Process52BF();
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
BOOL CRelayTask::WriteComposeBuf()
{
	LONG tempA1,tempA2,tempA3;
	LONG tempB1,tempB2,tempB3;
	LONG tempC1,tempC2,tempC3;

	for(LONG j=0;j<READ_POINT_NUM;j++)
	{
		dwLineUDataBuf[j] = dwSampBuf[j]-dwSampBuf[READ_POINT_NUM+j]; //UAB=UA-UB
		dwLineUDataBuf[READ_POINT_NUM+j] = dwSampBuf[READ_POINT_NUM+j]-dwSampBuf[READ_POINT_NUM*2+j]; //UBC=UB-UC
		dwLineUDataBuf[READ_POINT_NUM*2+j] = dwSampBuf[READ_POINT_NUM*2+j]-dwSampBuf[j];//UCA=UC-UA
	}
	
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		tempA1 = (CoefCorH[0]*dwSampBuf[READ_POINT_NUM*3+i]+ \
			CoefCorH[1]*dwSampBuf[READ_POINT_NUM*4+i]+CoefCorH[2]*dwSampBuf[READ_POINT_NUM*5+i])/1000;
		tempB1 = (CoefCorH[3]*dwSampBuf[READ_POINT_NUM*3+i]+ \
			CoefCorH[4]*dwSampBuf[READ_POINT_NUM*4+i]+CoefCorH[5]*dwSampBuf[READ_POINT_NUM*5+i])/1000;
		tempC1 = (CoefCorH[6]*dwSampBuf[READ_POINT_NUM*3+i]+ \
			CoefCorH[7]*dwSampBuf[READ_POINT_NUM*4+i]+CoefCorH[8]*dwSampBuf[READ_POINT_NUM*5+i])/1000;

		tempA2 = (CoefCorM[0]*dwSampBuf[READ_POINT_NUM*6+i]+ \
			CoefCorM[1]*dwSampBuf[READ_POINT_NUM*7+i]+CoefCorM[2]*dwSampBuf[READ_POINT_NUM*8+i])/1000;
		tempB2 = (CoefCorM[3]*dwSampBuf[READ_POINT_NUM*6+i]+ \
			CoefCorM[4]*dwSampBuf[READ_POINT_NUM*7+i]+CoefCorM[5]*dwSampBuf[READ_POINT_NUM*8+i])/1000;
		tempC2 = (CoefCorM[6]*dwSampBuf[READ_POINT_NUM*6+i]+ \
			CoefCorM[7]*dwSampBuf[READ_POINT_NUM*7+i]+CoefCorM[8]*dwSampBuf[READ_POINT_NUM*8+i])/1000;

		tempA3 = (CoefCorL[0]*dwSampBuf[READ_POINT_NUM*9+i]+ \
			CoefCorL[1]*dwSampBuf[READ_POINT_NUM*10+i]+CoefCorL[2]*dwSampBuf[READ_POINT_NUM*11+i])/1000;
		tempB3 = (CoefCorL[3]*dwSampBuf[READ_POINT_NUM*9+i]+ \
			CoefCorL[4]*dwSampBuf[READ_POINT_NUM*10+i]+CoefCorL[5]*dwSampBuf[READ_POINT_NUM*11+i])/1000;
		tempC3 = (CoefCorL[6]*dwSampBuf[READ_POINT_NUM*9+i]+ \
			CoefCorL[7]*dwSampBuf[READ_POINT_NUM*10+i]+CoefCorL[8]*dwSampBuf[READ_POINT_NUM*11+i])/1000;
		
		dwComposeBuf[i] 				  =  tempA1+tempA2+tempA3; 
		dwComposeBuf[READ_POINT_NUM+i]	  =  tempB1+tempB2+tempB3;		
		dwComposeBuf[READ_POINT_NUM*2+i]  =  tempC1+tempC2+tempC3;		
		dwComposeBuf[READ_POINT_NUM*3+i]  = (tempA1-tempA2-tempA3)/2;		
		dwComposeBuf[READ_POINT_NUM*4+i]  = (tempA2-tempA1-tempA3)/2;		
		dwComposeBuf[READ_POINT_NUM*5+i]  = (tempA3-tempA2-tempA1)/2;		
		dwComposeBuf[READ_POINT_NUM*6+i]  = (tempB1-tempB2-tempB3)/2;		
		dwComposeBuf[READ_POINT_NUM*7+i]  = (tempB2-tempB1-tempB3)/2;		
		dwComposeBuf[READ_POINT_NUM*8+i]  = (tempB3-tempB2-tempB1)/2;		
		dwComposeBuf[READ_POINT_NUM*9+i]  = (tempC1-tempC2-tempC3)/2;		
		dwComposeBuf[READ_POINT_NUM*10+i] = (tempC2-tempC1-tempC3)/2;		
		dwComposeBuf[READ_POINT_NUM*11+i] = (tempC2-tempC2+tempC1)/2;		
	}
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwComposeBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*2+i] = dwComposeBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*3+i] = dwComposeBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*4+i] = dwComposeBuf[READ_POINT_NUM*5-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*5+i] = dwComposeBuf[READ_POINT_NUM*6-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*6+i] = dwComposeBuf[READ_POINT_NUM*7-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*7+i] = dwComposeBuf[READ_POINT_NUM*8-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*8+i] = dwComposeBuf[READ_POINT_NUM*9-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*9+i] = dwComposeBuf[READ_POINT_NUM*10-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*10+i] = dwComposeBuf[READ_POINT_NUM*11-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*11+i] = dwComposeBuf[READ_POINT_NUM*12-RELAY_SAMP_INTERVAL+i];
	}
	if(WriteComposeData(dwRecBuf, dwComposeDataMask))
	{
		ReportMsg("Error:Write ComposeData failure!");
		RelayCheckErr = TRUE;
		return FALSE;
	}
	return TRUE;
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
			return ele1;
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
// ����: ���㸺���ѹ	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
TRelElecValPar CRelayTask::CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc) //���㸺���ѹ
{
	TRelElecValPar u2;
	u2.Real = ua->Real/3-ub->Real/6+866*ub->Imag/3000-uc->Real/6-866*uc->Imag/3000;
	u2.Imag = ua->Imag/3-ub->Imag/6-866*ub->Real/3000-uc->Imag/6+866*uc->Real/3000;
	CaluModValue(&u2);
	return u2;
}
//------------------------------------------------------------------------------------------ 
// ����: �������	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUI(void)
{	
	//UA
	CaluBaseFourier_S15(dwSampBuf,&UA);
	CaluModValue(&UA); 
	*RM_pnUA= UA.Mod; 									  
	DB_LogWriteRM(RM_UA, *RM_pnUA);
	if(UA.Mod>(*AS_pn59UA))
		WriteRelayWord(RW_59UA, RELAY_ON);
	else
		WriteRelayWord(RW_59UA, RELAY_OFF);
	if(UA.Mod>(*AS_pn27UA))
		WriteRelayWord(RW_27UA, RELAY_OFF);
	else
		WriteRelayWord(RW_27UA, RELAY_ON);
	//UB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UB);
	CaluModValue(&UB); 
	*RM_pnUB= UB.Mod; 									  
	DB_LogWriteRM(RM_UB, *RM_pnUB);
	if(UB.Mod>(*AS_pn59UB))
		WriteRelayWord(RW_59UB, RELAY_ON);
	else
		WriteRelayWord(RW_59UB, RELAY_OFF);
	if(UB.Mod>(*AS_pn27UB))
		WriteRelayWord(RW_27UB, RELAY_OFF);
	else
		WriteRelayWord(RW_27UB, RELAY_ON);
	//UC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UC);
	CaluModValue(&UC); 
	*RM_pnUC= UC.Mod; 									  
	DB_LogWriteRM(RM_UC, *RM_pnUC);
	if(UC.Mod>(*AS_pn59UC))
		WriteRelayWord(RW_59UC, RELAY_ON);
	else
		WriteRelayWord(RW_59UC, RELAY_OFF);
	if(UC.Mod>(*AS_pn27UC))
		WriteRelayWord(RW_27UC, RELAY_OFF);
	else
		WriteRelayWord(RW_27UC, RELAY_ON);
	//IHA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&IHA);
	CaluModValue(&IHA); 
	*RM_pnIHA= IHA.Mod; 									  
	DB_LogWriteRM(RM_IHA, *RM_pnIHA);
	if(IHA.Mod>(*AS_pn50HAC))
		WriteRelayWord(RW_50HA, RELAY_ON);
	else
		WriteRelayWord(RW_50HA, RELAY_OFF);
	if(IHA.Mod>(*AS_pn37HAC))
		WriteRelayWord(RW_37HA, RELAY_OFF);
	else
		WriteRelayWord(RW_37HA, RELAY_ON);
	//IHB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&IHB);
	CaluModValue(&IHB); 
	*RM_pnIHB= IHB.Mod; 									  
	DB_LogWriteRM(RM_IHB, *RM_pnIHB);
	if(IHB.Mod>(*AS_pn50HBC))
		WriteRelayWord(RW_50HB, RELAY_ON);
	else
		WriteRelayWord(RW_50HB, RELAY_OFF);
	if(IHB.Mod>(*AS_pn37HBC))
		WriteRelayWord(RW_37HB, RELAY_OFF);
	else
		WriteRelayWord(RW_37HB, RELAY_ON);
	//IHC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&IHC);
	CaluModValue(&IHC); 
	*RM_pnIHC= IHC.Mod; 									  
	DB_LogWriteRM(RM_IHC, *RM_pnIHC);
	if(IHC.Mod>(*AS_pn50HCC))
		WriteRelayWord(RW_50HC, RELAY_ON);
	else
		WriteRelayWord(RW_50HC, RELAY_OFF);
	if(IHC.Mod>(*AS_pn37HCC))
		WriteRelayWord(RW_37HC, RELAY_OFF);
	else
		WriteRelayWord(RW_37HC, RELAY_ON);
	//IMA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&IMA);
	CaluModValue(&IMA); 
	*RM_pnIMA= IMA.Mod; 									  
	DB_LogWriteRM(RM_IMA, *RM_pnIMA);
	if(IMA.Mod>(*AS_pn50MAC))
		WriteRelayWord(RW_50MA, RELAY_ON);
	else
		WriteRelayWord(RW_50MA, RELAY_OFF);
	if(IMA.Mod>(*AS_pn37MAC))
		WriteRelayWord(RW_37MA, RELAY_OFF);
	else
		WriteRelayWord(RW_37MA, RELAY_ON);
	//IMB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&IMB);
	CaluModValue(&IMB); 
	*RM_pnIMB= IMB.Mod; 									  
	DB_LogWriteRM(RM_IMB, *RM_pnIMB);
	if(IMB.Mod>(*AS_pn50MBC))
		WriteRelayWord(RW_50MB, RELAY_ON);
	else
		WriteRelayWord(RW_50MB, RELAY_OFF);
	if(IMB.Mod>(*AS_pn37MBC))
		WriteRelayWord(RW_37MB, RELAY_OFF);
	else
		WriteRelayWord(RW_37MB, RELAY_ON);
	//IMC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IMC);
	CaluModValue(&IMC); 
	*RM_pnIMC= IMC.Mod; 									  
	DB_LogWriteRM(RM_IMC, *RM_pnIMC);
	if(IMC.Mod>(*AS_pn50MCC))
		WriteRelayWord(RW_50MC, RELAY_ON);
	else
		WriteRelayWord(RW_50MC, RELAY_OFF);
	if(IMC.Mod>(*AS_pn37MCC))
		WriteRelayWord(RW_37MC, RELAY_OFF);
	else
		WriteRelayWord(RW_37MC, RELAY_ON);
	//ILA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&ILA);
	CaluModValue(&ILA); 
	*RM_pnILA= ILA.Mod; 									  
	DB_LogWriteRM(RM_ILA, *RM_pnILA);
	if(ILA.Mod>(*AS_pn50LAC))
		WriteRelayWord(RW_50LA, RELAY_ON);
	else
		WriteRelayWord(RW_50LA, RELAY_OFF);
	if(ILA.Mod>(*AS_pn37LAC))
		WriteRelayWord(RW_37LA, RELAY_OFF);
	else
		WriteRelayWord(RW_37LA, RELAY_ON);
	//ILB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&ILB);
	CaluModValue(&ILB); 
	*RM_pnILB= ILB.Mod; 									  
	DB_LogWriteRM(RM_ILB, *RM_pnILB);
	if(ILB.Mod>(*AS_pn50LBC))
		WriteRelayWord(RW_50LB, RELAY_ON);
	else
		WriteRelayWord(RW_50LB, RELAY_OFF);
	if(ILB.Mod>(*AS_pn37LBC))
		WriteRelayWord(RW_37LB, RELAY_OFF);
	else
		WriteRelayWord(RW_37LB, RELAY_ON);
	//ILC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&ILC);
	CaluModValue(&ILC); 
	*RM_pnILC= ILC.Mod; 									  
	DB_LogWriteRM(RM_ILC, *RM_pnILC);
	if(ILC.Mod>(*AS_pn50LCC))
		WriteRelayWord(RW_50LC, RELAY_ON);
	else
		WriteRelayWord(RW_50LC, RELAY_OFF);
	if(ILC.Mod>(*AS_pn37LCC))
		WriteRelayWord(RW_37LC, RELAY_OFF);
	else
		WriteRelayWord(RW_37LC, RELAY_ON);
	//�Ƕȼ���   ��IHAΪ��׼
	CaluAngle(&IHA,AngleTab);
	CaluAngle(&IHB,AngleTab);
	CaluAngle(&IHC,AngleTab);
	CaluAngle(&IMA,AngleTab);
	CaluAngle(&IMB,AngleTab);
	CaluAngle(&IMC,AngleTab);
	CaluAngle(&ILA,AngleTab);
	CaluAngle(&ILB,AngleTab);
	CaluAngle(&ILC,AngleTab);
	CaluAngle(&UA,AngleTab);
	CaluAngle(&UB,AngleTab);
	CaluAngle(&UC,AngleTab);
	*RM_pnAIHA=0;	
	*RM_pnAIHB=IHB.Angle - IHA.Angle;
	*RM_pnAIHC=IHC.Angle - IHA.Angle;
	*RM_pnAIMA=IMA.Angle - IHA.Angle;
	*RM_pnAIMB=IMB.Angle - IHA.Angle;
	*RM_pnAIMC=IMC.Angle - IHA.Angle;
	*RM_pnAILA=ILA.Angle - IHA.Angle;
	*RM_pnAILB=ILB.Angle - IHA.Angle;
	*RM_pnAILC=ILC.Angle - IHA.Angle;
	*RM_pnAUA=UA.Angle - IHA.Angle;
	*RM_pnAUB=UB.Angle - IHA.Angle;
	*RM_pnAUC=UC.Angle - IHA.Angle;
	DB_LogWriteRM(RM_AIHA, *RM_pnAIHA);
	DB_LogWriteRM(RM_AIHB, *RM_pnAIHB);
	DB_LogWriteRM(RM_AIHC, *RM_pnAIHC);
	DB_LogWriteRM(RM_AIMA, *RM_pnAIMA);
	DB_LogWriteRM(RM_AIMB, *RM_pnAIMB);
	DB_LogWriteRM(RM_AIMC, *RM_pnAIMC);
	DB_LogWriteRM(RM_AILA, *RM_pnAILA);
	DB_LogWriteRM(RM_AILB, *RM_pnAILB);
	DB_LogWriteRM(RM_AILC, *RM_pnAILC);
	DB_LogWriteRM(RM_AUA, *RM_pnAUA);
	DB_LogWriteRM(RM_AUB, *RM_pnAUB);
	DB_LogWriteRM(RM_AUC, *RM_pnAUC);
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
	
	U2H = CaluU2(&UA, &UB, &UC);  
	*RM_pnU2H = U2H.Mod;
	DB_LogWriteRM(RM_U2H,*RM_pnU2H);
	
	UHmax_ph = ElecMax(UA,UB,UC);
	UHmin_li = ElecMin(UAB,UBC,UCA);
	
	if((UHmin_li.Mod<*RS_pn47U_27U1)||(U2H.Mod>*RS_pn47U_59U2))
		WriteRelayWord(RW_47UH,RELAY_OFF);
	else
		WriteRelayWord(RW_47UH,RELAY_ON);
	
	IHmax = ElecMax(IHA,IHB,IHC,wHIPhase);
	IMmax = ElecMax(IMA,IMB,IMC);
	ILmax = ElecMax(ILA,ILB,ILC);
	
	if(wHIPhase == PHASE_A)
	{
		R50IH1.PForwardRelayWord = RW_PFHA; 
		R50IH1.PRewardRelayWord = RW_PRHA;			
		R50IH2.PForwardRelayWord = RW_PFHA; 
		R50IH2.PRewardRelayWord = RW_PRHA;
	}
	else if(wHIPhase == PHASE_B)
	{		
		R50IH1.PForwardRelayWord = RW_PFHB; 
		R50IH1.PRewardRelayWord = RW_PRHB;			
		R50IH2.PForwardRelayWord = RW_PFHB; 
		R50IH2.PRewardRelayWord = RW_PRHB;
	}
	else if(wHIPhase == PHASE_C)
	{		
		R50IH1.PForwardRelayWord = RW_PFHC; 
		R50IH1.PRewardRelayWord = RW_PRHC;			
		R50IH2.PForwardRelayWord = RW_PFHC; 
		R50IH2.PRewardRelayWord = RW_PRHC;
	}
}
void CRelayTask::CaluR87(void)
{
	CaluBaseFourier_S15(dwComposeBuf,&IDA);
	CaluModValue(&IDA);		
	*RM_pnIDA= IDA.Mod;
	DB_LogWriteRM(RM_IDA,*RM_pnIDA);
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&IDB);
	CaluModValue(&IDB);		
	*RM_pnIDB= IDB.Mod;
	DB_LogWriteRM(RM_IDB,*RM_pnIDB);
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*2,&IDC);
	CaluModValue(&IDC);		
	*RM_pnIDC= IDC.Mod;
	DB_LogWriteRM(RM_IDC,*RM_pnIDC);
	//------------��������2��3��5��г��---------------
	CaluSecondFourier(dwComposeBuf,&IDA2);
	CaluModValue(&IDA2);
	*RM_pnIDA2= IDA2.Mod;
	DB_LogWriteRM(RM_IDA2,*RM_pnIDA2);
	CaluThirdFourier(dwComposeBuf,&IDA3);
	CaluModValue(&IDA3);
	*RM_pnIDA3= IDA3.Mod;
	DB_LogWriteRM(RM_IDA3,*RM_pnIDA3);
	CaluFifthFourier(dwComposeBuf,&IDA5);
	CaluModValue(&IDA5);
	*RM_pnIDA5= IDA5.Mod;
	DB_LogWriteRM(RM_IDA5,*RM_pnIDA5);

	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM,&IDB2);
	CaluModValue(&IDB2);
	*RM_pnIDB2= IDB2.Mod;
	DB_LogWriteRM(RM_IDB2,*RM_pnIDB2);
	CaluThirdFourier(dwComposeBuf+READ_POINT_NUM,&IDB3);
	CaluModValue(&IDB3);
	*RM_pnIDB3= IDB3.Mod;
	DB_LogWriteRM(RM_IDB3,*RM_pnIDB3);
	CaluFifthFourier(dwComposeBuf+READ_POINT_NUM,&IDB5);
	CaluModValue(&IDB5);
	*RM_pnIDB5= IDB5.Mod;
	DB_LogWriteRM(RM_IDB5,*RM_pnIDB5);

	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*2,&IDC2);
	CaluModValue(&IDC2);
	*RM_pnIDC2= IDC2.Mod;
	DB_LogWriteRM(RM_IDC2,*RM_pnIDC2);
	CaluThirdFourier(dwComposeBuf+READ_POINT_NUM*2,&IDC3);
	CaluModValue(&IDC3);
	*RM_pnIDC3= IDC3.Mod;
	DB_LogWriteRM(RM_IDC3,*RM_pnIDC3);
	CaluFifthFourier(dwComposeBuf+READ_POINT_NUM*2,&IDC5);
	CaluModValue(&IDC5);
	*RM_pnIDC5= IDC5.Mod;
	DB_LogWriteRM(RM_IDC5,*RM_pnIDC5);

	
	//------------�ֱ����������������µ��ƶ�����----------
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*3,&IRA_1);
	CaluModValue(&IRA_1);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*4,&IRA_2);
	CaluModValue(&IRA_2);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*5,&IRA_3);
	CaluModValue(&IRA_3);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*6,&IRB_1);
	CaluModValue(&IRB_1);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*7,&IRB_2);
	CaluModValue(&IRB_2);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*8,&IRB_3);
	CaluModValue(&IRB_3);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*9,&IRC_1);
	CaluModValue(&IRC_1);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*10,&IRC_2);
	CaluModValue(&IRC_2);		
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*11,&IRC_3);
	CaluModValue(&IRC_3);

	IRA = ElecMax(IRA_1,IRA_2,IRA_3,wIRAPhase);
	IRB = ElecMax(IRB_1,IRB_2,IRB_3,wIRBPhase);
	IRC = ElecMax(IRC_1,IRC_2,IRC_3,wIRCPhase);	
	*RM_pnIRA= IRA.Mod;
	DB_LogWriteRM(RM_IRA,*RM_pnIRA);
	*RM_pnIRB= IRB.Mod;
	DB_LogWriteRM(RM_IRB,*RM_pnIRB);
	*RM_pnIRC= IRC.Mod;
	DB_LogWriteRM(RM_IRC,*RM_pnIRC);
	if(wIRAPhase == PHASE_A)
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*3,&IRA2);
	}
	else if(wIRAPhase == PHASE_B)		
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*4,&IRA2);
	}	
	else if(wIRAPhase == PHASE_C)		
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*5,&IRA2);
	}
	CaluModValue(&IRA2);
	*RM_pnIRA2= IRA2.Mod;
	DB_LogWriteRM(RM_IRA2,*RM_pnIRA2);
	
	if(wIRBPhase == PHASE_A)
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*6,&IRB2);
	}
	else if(wIRBPhase == PHASE_B)		
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*7,&IRB2);
	}	
	else if(wIRBPhase == PHASE_C)		
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*8,&IRB2);
	}
	CaluModValue(&IRB2);
	*RM_pnIRB2= IRB2.Mod;
	DB_LogWriteRM(RM_IRB2,*RM_pnIRB2);
	
	if(wIRCPhase == PHASE_A)
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*9,&IRC2);
	}
	else if(wIRCPhase == PHASE_B)		
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*10,&IRC2);
	}	
	else if(wIRCPhase == PHASE_C)		
	{
		CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*11,&IRC2);
	}
	CaluModValue(&IRC2);
	*RM_pnIRC2= IRC2.Mod;
	DB_LogWriteRM(RM_IRC2,*RM_pnIRC2);
}
void CRelayTask::ResetIdHLock()
{
	b68IdH2Flag_A = FALSE;	//A��������α�����־	
	b68IdH2Flag_B = FALSE;	//B��������α�����־	
	b68IdH2Flag_C = FALSE;	//C��������α�����־	
	WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
}
/***************************************************************************************
����:		���������������Ư
����:		��
����:		��
****************************************************************************************/
void CRelayTask::CaluDrift(void)
{
	if(FaultOnFlag == TRUE)//�Ѿ�������ϴ����������������Ưֵ
		return;
	LONG D1=0;
	LONG D2=0;
	LONG D3=0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		D1 += dwComposeBuf[i+2];
		D2 += dwComposeBuf[READ_POINT_NUM*+i+2];
		D3 += dwComposeBuf[READ_POINT_NUM*2+i+2];
	}
	for(LONG i=0;i<4;i++)
	{
		IDADrift[4-i]=IDADrift[3-i];
		IDBDrift[4-i]=IDBDrift[3-i];
		IDBDrift[4-i]=IDBDrift[3-i];
	}
	IDADrift[0]=D1/POINT_PER_PERIOD;
	IDBDrift[0]=D2/POINT_PER_PERIOD;
	IDCDrift[0]=D3/POINT_PER_PERIOD;
}
//------------------------------------------------------------------------------------------ 
// ����: �����ڷ�������                                                              
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::R68IdAped()
{
	if(*CFG_pbyDCRES_87R == CFG_STATUS_SET)
	{
		LONG tempA=0;
		LONG tempB=0;
		LONG tempC=0;
		for(LONG i=0;i<POINT_PER_PERIOD;i++)
		{
			tempA += dwComposeBuf[i+2];
			tempB += dwComposeBuf[READ_POINT_NUM+i+2];
			tempC += dwComposeBuf[READ_POINT_NUM*2+i+2];
		}
		tempA = tempA*1414/(POINT_PER_PERIOD*1000) - IDADrift[4];   
		tempB = tempB*1414/(POINT_PER_PERIOD*1000) - IDBDrift[4];
		tempC = tempC*1414/(POINT_PER_PERIOD*1000) - IDCDrift[4];
		tempA = ToAbs(tempA);
		tempB = ToAbs(tempB);
		tempC = ToAbs(tempC);
		//����A�ż�ֵУ��
		if((IDA.Mod ==0)||(tempA>15*IDA.Mod/10))//���Ƿ�ĸΪ0�����
		{
			A_87RPI = *RS_pn87RIA;
			IDAH2 = IDA2.Mod;
		}
		else		
		{
			A_87RPI = *RS_pn87RIA*(100+(*RS_pnKrav*tempA/IDA.Mod))/100; 
			IDAH2 = IDA2.Mod*(100+(*RS_pnK2av*tempA/IDA.Mod))/100;
		}
		
		//����B�ż�ֵУ��
		if((IDB.Mod ==0)||(tempB>15*IDB.Mod/10))
		{
			B_87RPI = *RS_pn87RIB;
			IDBH2 = IDB2.Mod;
		}
		else
		{
			B_87RPI = *RS_pn87RIB*(100+*RS_pnKrav*tempB/IDB.Mod)/100;
			IDBH2 = IDB2.Mod*(100+(*RS_pnK2av*tempB/IDB.Mod))/100;
		}
		//����C�ż�ֵУ��
		if((IDC.Mod ==0)||(tempC>15*IDC.Mod/10))
		{
			C_87RPI = *RS_pn87RIC;
			IDCH2 = IDC2.Mod;
		}
		else
		{
			C_87RPI = *RS_pn87RIC*(100+*RS_pnKrav*tempC/IDC.Mod)/100;		
			IDCH2 = IDC2.Mod*(100+(*RS_pnK2av*tempC/IDC.Mod))/100;
		}
	}
	else
	{
		A_87RPI = *RS_pn87RIA;		
		B_87RPI = *RS_pn87RIB;
		C_87RPI = *RS_pn87RIC;
		IDAH2 = IDA2.Mod;
		IDBH2 = IDB2.Mod;
		IDCH2 = IDC2.Mod;
	}
}

/***************************************************************************************
����:		���������г����������
����:		��
����:		��
****************************************************************************************/
void CRelayTask::R68IdH2M()
{
	if(*CFG_pby68IdH2M_87R == CFG_STATUS_SET)
	{			
		if((IDA.Mod>A_87RPI)&&(*R87R_A.p87RCFG == CFG_STATUS_SET))
		{
			if(IDA2.Mod>(*RS_pnKrh2*IDA.Mod/100))
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
		if((IDB.Mod>B_87RPI)&&(*R87R_B.p87RCFG == CFG_STATUS_SET))
		{
			if(IDB2.Mod>(*RS_pnKrh2*IDB.Mod/100))
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
		if((IDC.Mod>C_87RPI)&&(*R87R_C.p87RCFG == CFG_STATUS_SET))
		{
			if(IDC2.Mod>(*RS_pnKrh2*IDC.Mod/100))
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
/***************************************************************************************
����:		���������г���ۺ������
����:		��
����:		��
****************************************************************************************/

void CRelayTask::R68IdH2C(void)		
{
	if(*CFG_pby68IdH2C_87R == CFG_STATUS_SET)
	{		
		LONG ID1,ID2;		
		ID1 = MaxABC(IDA.Mod,IDB.Mod,IDC.Mod);
		ID2 = MaxABC(IDA2.Mod,IDB2.Mod,IDC2.Mod);
		if(ID2>(*RS_pnKrh2*ID1/100))
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
/***************************************************************************************
����:		���������г���������
����:		��
����:		��
****************************************************************************************/

void CRelayTask::R68IdH2I()  
{
	if(*CFG_pby68IdH2I_87R == CFG_STATUS_SET)
	{
		if((IDA.Mod>A_87RPI)&&(IDAH2>(*RS_pnKrh2*IDA.Mod/100)))
		{
			b68IdH2Flag_A = TRUE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
		}			
		else
		{
			b68IdH2Flag_A = FALSE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
		}
		
		if((IDB.Mod>B_87RPI)&&(IDBH2>(*RS_pnKrh2*IDB.Mod/100)))
		{
			b68IdH2Flag_B = TRUE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
		}			
		else
		{
			b68IdH2Flag_B = FALSE;	//A��������α�����־					
			WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
		}
		
		if((IDC.Mod>C_87RPI)&&(IDCH2>(*RS_pnKrh2*IDC.Mod/100)))
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
/***************************************************************************************
����:		�ƶ���������г������
����:		��
����:		��
****************************************************************************************/
void CRelayTask::R68IrH2M()
{	
	if(*CFG_pby68IrH2I_87R != CFG_STATUS_SET)
	{		
		b68IrH2Flag_A = FALSE;
		b68IrH2Flag_B = FALSE;
		b68IrH2Flag_C = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF);
		WriteRelayWord(RW_LK68IRCH2,RELAY_OFF);
		return;
	}
	
	if((IDA.Mod>A_87RPI)&&(IRA2.Mod>(*RS_pnKrh2*IRA.Mod/100)))
	{
		b68IrH2Flag_A = TRUE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_ON);		
	}		
	else
	{
		b68IrH2Flag_A = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
	}
	
	if((IDB.Mod>B_87RPI)&&(IRB2.Mod>(*RS_pnKrh2*IRB.Mod/100)))
	{
		b68IrH2Flag_B = TRUE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_ON);		
	}		
	else
	{
		b68IrH2Flag_B = FALSE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF); 	
	}
	
	if((IDC.Mod>C_87RPI)&&(IRC2.Mod>(*RS_pnKrh2*IRC.Mod/100)))
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
/***************************************************************************************
����:		��������г������
����:		��
����:		��
****************************************************************************************/
void CRelayTask::R68IdH5()
{	
	if(*CFG_pby68IdH5I_87R != CFG_STATUS_SET)
	{	
		b68IdH5Flag_A = FALSE;
		b68IdH5Flag_B = FALSE;
		b68IdH5Flag_C = FALSE;
		WriteRelayWord(RW_LK68IDAH5,RELAY_OFF); 	
		WriteRelayWord(RW_LK68IDBH5,RELAY_OFF);
		WriteRelayWord(RW_LK68IDCH5,RELAY_OFF);
		return;
	}
	
	if((IDA.Mod>A_87RPI)&&(IDA5.Mod>*RS_pnKrh5*IDA.Mod/100))
	{
		b68IdH5Flag_A= TRUE;
		WriteRelayWord(RW_LK68IDAH5,RELAY_ON);		
	}		
	else
	{
		b68IdH5Flag_A= FALSE;
		WriteRelayWord(RW_LK68IDAH5,RELAY_OFF); 	
	}

	
	if((IDB.Mod>B_87RPI)&&(IDB5.Mod>*RS_pnKrh5*IDB.Mod/100))
	{
		b68IdH5Flag_B= TRUE;
		WriteRelayWord(RW_LK68IDBH5,RELAY_ON);		
	}		
	else
	{
		b68IdH5Flag_B= FALSE;
		WriteRelayWord(RW_LK68IDBH5,RELAY_OFF); 	
	}
	
	if((IDC.Mod>C_87RPI)&&(IDC5.Mod>*RS_pnKrh5*IDC.Mod/100))
	{
		b68IdH5Flag_C= TRUE;
		WriteRelayWord(RW_LK68IDCH5,RELAY_ON);		
	}
	else
	{
		b68IdH5Flag_C= FALSE;
		WriteRelayWord(RW_LK68IDCH5,RELAY_OFF); 	
	}
}
//******************************************************************************************
// ����: ������ۺ�г������             
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::R68IdHI(void) 	//�ۺ�г������
{	
	if(*CFG_pby68IdHMUL_87R != CFG_STATUS_SET)
	{		
		b68IdHMulFlag_A = FALSE;
		b68IdHMulFlag_B = FALSE;
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDAMUL,RELAY_OFF);	
		WriteRelayWord(RW_LK68IDBMUL,RELAY_OFF);
		WriteRelayWord(RW_LK68IDCMUL,RELAY_OFF);
		return;
	}
	
	if((IDA.Mod>*RS_pn87RIA)&&((IDA2.Mod+IDA3.Mod)>(*RS_pnKrhm*IDA.Mod/100)))
	{
		b68IdHMulFlag_A = TRUE;
		WriteRelayWord(RW_LK68IDAMUL,RELAY_ON);		
	}
	else
	{
		b68IdHMulFlag_A = FALSE;
		WriteRelayWord(RW_LK68IDAMUL,RELAY_OFF);		
	}
	
	if((IDB.Mod>*RS_pn87RIB)&&((IDB2.Mod+IDB3.Mod)>(*RS_pnKrhm*IDB.Mod/100)))
	{
		b68IdHMulFlag_B = TRUE;
		WriteRelayWord(RW_LK68IDBMUL,RELAY_ON);		
	}		
	else
	{
		b68IdHMulFlag_B = FALSE;
		WriteRelayWord(RW_LK68IDBMUL,RELAY_OFF);		
	}
	
	if((IDC.Mod>*RS_pn87RIC)&&((IDC2.Mod+IDC3.Mod)>(*RS_pnKrhm*IDC.Mod/100)))
	{
		b68IdHMulFlag_C = TRUE;
		WriteRelayWord(RW_LK68IDCMUL,RELAY_ON);		
	}
	else
	{
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDCMUL,RELAY_OFF);		
	}
}


//******************************************************************************************
// ����: ��ʼ��A���ٶϱ���Ԫ��               
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87U_A()
{	
	R87U_A.pElecSet				= RS_pn87UIA;          			//��������������ֵ
	R87U_A.pTimeSet				= &TimeSet;				 		//��������ʱ�䶨ֵ
	R87U_A.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R87U_A.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	R87U_A.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R87U_A.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R87U_A.pRelCFG				= CFG_pby87U_A;					//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R87U_A.pRelSW				= SW_pby87U;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R87U_A.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_A.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_A.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_A.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_A.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R87U_A.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R87U_A.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R87U_A.pRel47UCFG			= &RelCFGCLR;
	R87U_A.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R87U_A.pRelElec 			= &IDA;							//��ǰ����ָ��		
	R87U_A.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R87U_A.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R87U_A.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R87U_A.StartRelayWord		= RW_PI87U_A;					//����������μ̵�����
	R87U_A.ActRelayWord			= RW_TR87U_A;					//�������������̵�����
	R87U_A.AccRelayWord			= FALSE1;						//���������̵�����
	R87U_A.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R87U_A.PForwardRelayWord	= FALSE1;						//����������̵�����
	R87U_A.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	R87U_A.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R87U_A.R47ULKRelayWord		= FALSE1;
	R87U_A.LockRelayWord		= FALSE1;//RW_LKPI87U_A;					//��α����̵�����
	R87U_A.RSTRelayWord			= TRUE1;//RW_LKRST87U_A;				//���ر����̵�����
	R87U_A.LoopCtrlRelayWord	= RW_LPC87U_A;					//����ѭ�����Ƽ̵�����
	
	R87U_A.wReportNum			= R_R87UA;						//���������������
	R87U_A.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R87U_A.bMakeActRpt			= TRUE;
	R87U_A.dwK35				= NULL;
	R87U_A.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************��η��ر��沿��*************	yanxs  13-03-20
	R87U_A.wPickUpReportNum		= 0xFFFF;				//��α������к�
	R87U_A.wResetReportNum		= 0xFFFF;				//���ر������к�
	R87U_A.wAccPickUpRptNum		= 0xFFFF;				//��α������к�
	R87U_A.wAccResetRptNum		= 0xFFFF;				//���ر������к�
	//**********************************************************************	

	R87U_A.dwRelTimer			= 0;			 			//��ʱ��
	R87U_A.byRelFCounter		= 0;						//���ϼ�����
	R87U_A.byRelRCounter		= 0;						//���ؼ�����	
	
	R87U_A.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ��B���ٶϱ���Ԫ��               
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87U_B()
{	
	R87U_B.pElecSet				= RS_pn87UIB;          			//��������������ֵ
	R87U_B.pTimeSet				= &TimeSet;				 		//��������ʱ�䶨ֵ
	R87U_B.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R87U_B.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	R87U_B.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R87U_B.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R87U_B.pRelCFG				= CFG_pby87U_B;					//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R87U_B.pRelSW				= SW_pby87U;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R87U_B.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_B.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_B.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_B.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_B.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R87U_B.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R87U_B.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R87U_B.pRel47UCFG			= &RelCFGCLR;
	R87U_B.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R87U_B.pRelElec 			= &IDB;							//��ǰ����ָ��		
	R87U_B.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R87U_B.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R87U_B.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R87U_B.StartRelayWord		= RW_PI87U_B;					//����������μ̵�����
	R87U_B.ActRelayWord			= RW_TR87U_B;					//�������������̵�����
	R87U_B.AccRelayWord			= FALSE1;						//���������̵�����
	R87U_B.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R87U_B.PForwardRelayWord	= FALSE1;						//����������̵�����
	R87U_B.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	R87U_B.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	
	R87U_B.R47ULKRelayWord		= FALSE1;
	R87U_B.LockRelayWord		= FALSE1;//RW_LKPI87U_B;					//��α����̵�����
	R87U_B.RSTRelayWord			= TRUE1;//RW_LKRST87U_B;				//���ر����̵�����
	R87U_B.LoopCtrlRelayWord	= RW_LPC87U_B;					//����ѭ�����Ƽ̵�����
	
	R87U_B.wReportNum			= R_R87UB;						//���������������
	R87U_B.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R87U_B.bMakeActRpt			= TRUE;
	R87U_B.dwK35				= NULL;
	R87U_B.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************��η��ر��沿��*************	yanxs  13-03-20
	R87U_B.wPickUpReportNum		= 0xFFFF;				//��α������к�
	R87U_B.wResetReportNum		= 0xFFFF;				//���ر������к�
	R87U_B.wAccPickUpRptNum		= 0xFFFF;				//��α������к�
	R87U_B.wAccResetRptNum		= 0xFFFF;				//���ر������к�
	//**********************************************************************	


	R87U_B.dwRelTimer			= 0;			 			//��ʱ��
	R87U_B.byRelFCounter		= 0;						//���ϼ�����
	R87U_B.byRelRCounter		= 0;						//���ؼ�����	
	
	R87U_B.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ��C���ٶϱ���Ԫ��               
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87U_C()
{	
	R87U_C.pElecSet				= RS_pn87UIC;          			//��������������ֵ
	R87U_C.pTimeSet				= &TimeSet;				 		//��������ʱ�䶨ֵ
	R87U_C.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R87U_C.pVolLockSet			= NULL;				 			//����������ѹ������ֵ
	R87U_C.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R87U_C.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R87U_C.pRelCFG				= CFG_pby87U_C;					//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R87U_C.pRelSW				= SW_pby87U;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R87U_C.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_C.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_C.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_C.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R87U_C.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R87U_C.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R87U_C.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	
	R87U_C.pRel47UCFG			= &RelCFGCLR;
	R87U_C.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R87U_C.pRelElec 			= &IDC;							//��ǰ����ָ��		
	R87U_C.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R87U_C.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R87U_C.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R87U_C.StartRelayWord		= RW_PI87U_C;					//����������μ̵�����
	R87U_C.ActRelayWord			= RW_TR87U_C;					//�������������̵�����
	R87U_C.AccRelayWord			= FALSE1;						//���������̵�����
	R87U_C.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R87U_C.PForwardRelayWord	= FALSE1;						//����������̵�����
	R87U_C.PRewardRelayWord   	= FALSE1;						//���ʷ�����̵�����
	R87U_C.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	
	R87U_C.R47ULKRelayWord		= FALSE1;
	R87U_C.LockRelayWord		= FALSE1;//RW_LKPI87U_C;					//��α����̵�����
	R87U_C.RSTRelayWord			= TRUE1;//RW_LKRST87U_C;				//���ر����̵�����
	R87U_C.LoopCtrlRelayWord	= RW_LPC87U_C;					//����ѭ�����Ƽ̵�����
	
	R87U_C.wReportNum			= R_R87UC;						//���������������
	R87U_C.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R87U_C.bMakeActRpt			= TRUE;
	R87U_C.dwK35				= NULL;
	R87U_C.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************��η��ر��沿��*************	yanxs  13-03-20
	R87U_C.wPickUpReportNum		= 0xFFFF;				//��α������к�
	R87U_C.wResetReportNum		= 0xFFFF;				//���ر������к�
	R87U_C.wAccPickUpRptNum		= 0xFFFF;				//��α������к�
	R87U_C.wAccResetRptNum		= 0xFFFF;				//���ر������к�
	//**********************************************************************	


	R87U_C.dwRelTimer			= 0;			 			//��ʱ��
	R87U_C.byRelFCounter		= 0;						//���ϼ�����
	R87U_C.byRelRCounter		= 0;						//���ؼ�����	
	
	R87U_C.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ��A����ʲ����Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87R_A()
{
	R87R_A.p87RISet		= RS_pn87RIA; 	//�������ֵ
	R87R_A.pIRS1Set		= RS_pnIRS1A; 	//�ƶ��յ�1��ֵ
	R87R_A.pIRS2Set		= RS_pnIRS2A; 	//�ƶ��յ�2��ֵ 
	R87R_A.pSLP1Set			= RS_pnSLP1;		//�����ƶ�ϵ��1
	R87R_A.pSLP2Set			= RS_pnSLP2;		//�����ƶ�ϵ��2 
	R87R_A.pKrh2Set			= RS_pnKrh2;		//����г���ƶ�ϵ��
	R87R_A.pKrh5Set			= RS_pnKrh5;		//���г���ƶ�ϵ��
	R87R_A.pKravSet			= RS_pnKrav;		//���ʲ�����ڷ�����Ȩϵ��
	R87R_A.pK2avSet			= RS_pnK2av;		//����г�������ڷ�����Ȩϵ��

	R87R_A.p87RCFG			= CFG_pby87R_A;		//��������
	R87R_A.p87RSW			= SW_pby87R;		//��ѹ��
	R87R_A.p68IdH2MCFG		= CFG_pby68IdH2M_87R;		//��������г��������������
	R87R_A.p68IdH2CCFG		= CFG_pby68IdH2C_87R;		//��������г���ۺ����������
	R87R_A.p68IdH2ICFG		= CFG_pby68IdH2I_87R;		//��������г�������������
	R87R_A.p68IrH2MCFG		= CFG_pby68IrH2I_87R;		//�ƶ���������г����������
	R87R_A.p68IdH5CFG		= CFG_pby68IdH5I_87R;		//����5��г������
	R87R_A.p68IdHMulCFG		= CFG_pby68IdHMUL_87R;		//CFG_pby68IdHMUL_87R;

	R87R_A.pIdH2Flag		= &b68IdH2Flag_A; 	//����г��������־
	R87R_A.pIdH5Flag		= &b68IdH5Flag_A; 	//���г��������־
	R87R_A.pIrH2Flag		= &b68IrH2Flag_A; 	//�ƶ���������г��������־
	R87R_A.pIdHMulFlag		= &b68IdHMulFlag_A;

	R87R_A.p87RPI			= &A_87RPI;		//���Ƽ����Ĳ�����ż���ֵ
	R87R_A.pId				= &IDA;			//��������
	R87R_A.pIr				= &IRA;			//�ƶ�����

	R87R_A.ACTRelayWord		= RW_TR87R_A;					//�����̵����� 
	R87R_A.StartRelayWord	= RW_PI87R_A; 				//��μ̵����� 
	R87R_A.LockRelayWord	= FALSE1;//RW_LKPI87R_A;					//��α����̵����� 
	R87R_A.RSTRelayWord		= TRUE1;//RW_LKRST87R_A;					//���ر����̵����� 
	R87R_A.LoopCtrlRelayWord= RW_LPC87R_A;				//����ѭ�����Ƽ̵����� 

	R87R_A.RptSerialNum		= &RptSerialNumOfAct;					//ϵͳ�������к� 
	R87R_A.pSysTimer		= &dwRelayMSCnt;; 					//ϵͳ��ʱ�� 
	R87R_A.wRelRptNum		= R_R87RA; 				//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-20
	R87R_A.wPickUpReportNum		= 0xFFFF;				//��α������к�
	R87R_A.wResetReportNum		= 0xFFFF;				//���ر������к�
	//**********************************************************************	

	R87R_A.wRelFCounter		= 0;					//���ϼ����� 
	R87R_A.wRelRCounter		= 0;					//���ؼ�����	
	R87R_A.dwRelTimer		= 0; 					//������ʱ��		
	R87R_A.wExcepWordNum	= 7;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 
	R87R_A.Info				= 0;							//������Ϣ 
}
//******************************************************************************************
// ����: ��ʼ��B����ʲ����Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87R_B()
{
	R87R_B.p87RISet		= RS_pn87RIB; 	//�������ֵ
	R87R_B.pIRS1Set		= RS_pnIRS1B; 	//�ƶ��յ�1��ֵ
	R87R_B.pIRS2Set		= RS_pnIRS2B; 	//�ƶ��յ�2��ֵ 
	R87R_B.pSLP1Set			= RS_pnSLP1;		//�����ƶ�ϵ��1
	R87R_B.pSLP2Set			= RS_pnSLP2;		//�����ƶ�ϵ��2 
	R87R_B.pKrh2Set			= RS_pnKrh2;		//����г���ƶ�ϵ��
	R87R_B.pKrh5Set			= RS_pnKrh5;		//���г���ƶ�ϵ��
	R87R_B.pKravSet			= RS_pnKrav;		//���ʲ�����ڷ�����Ȩϵ��
	R87R_B.pK2avSet			= RS_pnK2av;		//����г�������ڷ�����Ȩϵ��

	R87R_B.p87RCFG			= CFG_pby87R_B;		//��������
	R87R_B.p87RSW			= SW_pby87R;		//��ѹ��
	R87R_B.p68IdH2MCFG		= CFG_pby68IdH2M_87R;		//��������г��������������
	R87R_B.p68IdH2CCFG		= CFG_pby68IdH2C_87R;		//��������г���ۺ����������
	R87R_B.p68IdH2ICFG		= CFG_pby68IdH2I_87R;		//��������г�������������
	R87R_B.p68IrH2MCFG		= CFG_pby68IrH2I_87R;		//�ƶ���������г����������
	R87R_B.p68IdH5CFG		= CFG_pby68IdH5I_87R;		//����5��г������
	R87R_B.p68IdHMulCFG		= CFG_pby68IdHMUL_87R;		//�����ۺ�г������

	R87R_B.pIdH2Flag		= &b68IdH2Flag_B; 	//����г��������־
	R87R_B.pIdH5Flag		= &b68IdH5Flag_B; 	//���г��������־
	R87R_B.pIrH2Flag		= &b68IrH2Flag_B; 	//�ƶ���������г��������־
	R87R_B.pIdHMulFlag		= &b68IdHMulFlag_B;

	R87R_B.p87RPI			= &B_87RPI;		//���Ƽ����Ĳ�����ż���ֵ
	R87R_B.pId				= &IDB;			//��������
	R87R_B.pIr				= &IRB;			//�ƶ�����

	R87R_B.ACTRelayWord		= RW_TR87R_B;					//�����̵����� 
	R87R_B.StartRelayWord	= RW_PI87R_B; 				//��μ̵����� 
	R87R_B.LockRelayWord	= FALSE1;//RW_LKPI87R_B;					//��α����̵����� 
	R87R_B.RSTRelayWord		= TRUE1;//RW_LKRST87R_B;					//���ر����̵����� 
	R87R_B.LoopCtrlRelayWord= RW_LPC87R_B;				//����ѭ�����Ƽ̵����� 

	R87R_B.RptSerialNum		= &RptSerialNumOfAct;					//ϵͳ�������к� 
	R87R_B.pSysTimer		= &dwRelayMSCnt;; 					//ϵͳ��ʱ�� 
	R87R_B.wRelRptNum		= R_R87RB; 				//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-20
	R87R_B.wPickUpReportNum		= 0xFFFF;				//��α������к�
	R87R_B.wResetReportNum		= 0xFFFF;				//���ر������к�
	//**********************************************************************	

	R87R_B.wRelFCounter		= 0;					//���ϼ����� 
	R87R_B.wRelRCounter		= 0;					//���ؼ�����	
	R87R_B.dwRelTimer		= 0; 					//������ʱ��		
	R87R_B.wExcepWordNum	= 7;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 
	R87R_B.Info				= 0;							//������Ϣ 
}
//******************************************************************************************
// ����: ��ʼ��C����ʲ����Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87R_C()
{
	R87R_C.p87RISet		= RS_pn87RIC; 	//�������ֵ
	R87R_C.pIRS1Set		= RS_pnIRS1C; 	//�ƶ��յ�1��ֵ
	R87R_C.pIRS2Set		= RS_pnIRS2C; 	//�ƶ��յ�2��ֵ 
	R87R_C.pSLP1Set			= RS_pnSLP1;		//�����ƶ�ϵ��1
	R87R_C.pSLP2Set			= RS_pnSLP2;		//�����ƶ�ϵ��2 
	R87R_C.pKrh2Set			= RS_pnKrh2;		//����г���ƶ�ϵ��
	R87R_C.pKrh5Set			= RS_pnKrh5;		//���г���ƶ�ϵ��
	R87R_C.pKravSet			= RS_pnKrav;		//���ʲ�����ڷ�����Ȩϵ��
	R87R_C.pK2avSet			= RS_pnK2av;		//����г�������ڷ�����Ȩϵ��

	R87R_C.p87RCFG			= CFG_pby87R_C;		//��������
	R87R_C.p87RSW			= SW_pby87R;		//��ѹ��
	R87R_C.p68IdH2MCFG		= CFG_pby68IdH2M_87R;		//��������г��������������
	R87R_C.p68IdH2CCFG		= CFG_pby68IdH2C_87R;		//��������г���ۺ����������
	R87R_C.p68IdH2ICFG		= CFG_pby68IdH2I_87R;		//��������г�������������
	R87R_C.p68IrH2MCFG		= CFG_pby68IrH2I_87R;		//�ƶ���������г����������
	R87R_C.p68IdH5CFG		= CFG_pby68IdH5I_87R;		//����5��г������
	R87R_C.p68IdHMulCFG		= CFG_pby68IdHMUL_87R;		//�����ۺ�г������

	R87R_C.pIdH2Flag		= &b68IdH2Flag_C; 	//����г��������־
	R87R_C.pIdH5Flag		= &b68IdH5Flag_C; 	//���г��������־
	R87R_C.pIrH2Flag		= &b68IrH2Flag_C; 	//�ƶ���������г��������־
	R87R_C.pIdHMulFlag		= &b68IdHMulFlag_C;

	R87R_C.p87RPI			= &C_87RPI;		//���Ƽ����Ĳ�����ż���ֵ
	R87R_C.pId				= &IDC;			//��������
	R87R_C.pIr				= &IRC;			//�ƶ�����

	R87R_C.ACTRelayWord		= RW_TR87R_C;					//�����̵����� 
	R87R_C.StartRelayWord	= RW_PI87R_C; 				//��μ̵����� 
	R87R_C.LockRelayWord	= FALSE1;//RW_LKPI87R_C;					//��α����̵����� 
	R87R_C.RSTRelayWord		= TRUE1;//RW_LKRST87R_C;					//���ر����̵����� 
	R87R_C.LoopCtrlRelayWord= RW_LPC87R_C;				//����ѭ�����Ƽ̵����� 

	R87R_C.RptSerialNum		= &RptSerialNumOfAct;					//ϵͳ�������к� 
	R87R_C.pSysTimer		= &dwRelayMSCnt;; 					//ϵͳ��ʱ�� 
	R87R_C.wRelRptNum		= R_R87RC; 				//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-20
	R87R_C.wPickUpReportNum		= 0xFFFF;				//��α������к�
	R87R_C.wResetReportNum		= 0xFFFF;				//���ر������к�
	//**********************************************************************	

	R87R_C.wRelFCounter		= 0;					//���ϼ����� 
	R87R_C.wRelRCounter		= 0;					//���ؼ�����	
	R87R_C.dwRelTimer		= 0; 					//������ʱ��		
	R87R_C.wExcepWordNum	= 7;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 
	R87R_C.Info				= 0;							//������Ϣ 
}
//******************************************************************************************
// ����: ��ʼ����ѹ�����1�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50IH1()
{
	R50IH1.pElecSet				= RS_pn50IH1C;          						//��������������ֵ
	R50IH1.pTimeSet				= RS_pn50IH1D;				 			//��������ʱ�䶨ֵ
	R50IH1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50IH1.pVolLockSet			= NULL;				 	    		//����������ѹ������ֵ
	R50IH1.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	R50IH1.pH2Coef				= NULL;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50IH1.pRelCFG				= CFG_pby50I_H1;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50IH1.pRelSW				= SW_pby50I_H;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	R50IH1.pRelHLockCFG			= &RelCFGCLR;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH1.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH1.pRelPDCFG			= CFG_pby50I_H1_PD;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH1.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH1.pRelFDCFG			= &RelCFGSET;						//���ʷ����ܿ���λ--1:������2:������
	R50IH1.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	R50IH1.pRel47UCFG			= CFG_pby50I_H1_47U;			//��ѹ��������
	R50IH1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50IH1.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	R50IH1.pRelElec 			= &IHmax;									//��ǰ����ָ��		
	R50IH1.pRelHarm3			= NULL;									//��ǰ����г��ָ��	
	R50IH1.pRelHarm5			= NULL;									//��ǰ���г��ָ��	
	R50IH1.pRelVolt				= NULL;									//��ǰ���������ĵ�ѹ��ָ��

	R50IH1.StartRelayWord		= RW_PI50IH1;						//����������μ̵�����
	R50IH1.ActRelayWord			= RW_TR50IH1;							//�������������̵�����
	R50IH1.AccRelayWord			= FALSE1;									//���������̵�����
	R50IH1.AccActRelayWord		= FALSE1;									//����ٶ����̵�����
	R50IH1.PForwardRelayWord	= FALSE1;							//���ʷ���̵�����
	R50IH1.PRewardRelayWord  	= FALSE1;
	R50IH1.H2LockRelayWord		= FALSE1;							//����г�������̵�����
	R50IH1.R47ULKRelayWord		= RW_47UH;
	R50IH1.LockRelayWord		= RW_LKPI50IH1;						//��α����̵�����
	R50IH1.RSTRelayWord			= RW_LKRST50IH1;							//���ر����̵�����
	R50IH1.LoopCtrlRelayWord	= RW_LPC50IH1;						//����ѭ�����Ƽ̵�����
	R50IH1.wReportNum			= R_R50IH1;							//���������������
	R50IH1.wAccReportNum		= 0xFFFF;								//���ٶ����������

	R50IH1.dwK35				= NULL;
	R50IH1.RptSerialNum			= &RptSerialNumOfAct;						
    R50IH1.bMakeActRpt       	= TRUE;

	R50IH1.wPickUpReportNum		= R_R50IH1_PI;						//��α������к�
	R50IH1.wResetReportNum		= R_R50IH1_RES;						//���ر������к�
	R50IH1.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50IH1.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50IH1.dwRelTimer			= 0;			 			//��ʱ��
	R50IH1.byRelFCounter		= 0;						//���ϼ�����
	R50IH1.byRelRCounter		= 0;						//���ؼ�����		
	R50IH1.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ����ѹ�����2�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50IH2()
{
	R50IH2.pElecSet				= RS_pn50IH2C;					//��������������ֵ
	R50IH2.pTimeSet				= RS_pn50IH2D;				 	//��������ʱ�䶨ֵ
	R50IH2.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50IH2.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50IH2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50IH2.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50IH2.pRelCFG				= CFG_pby50I_H2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50IH2.pRelSW				= SW_pby50I_H;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50IH2.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH2.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH2.pRelPDCFG			= CFG_pby50I_H2_PD;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IH2.pRelFDCFG			= &RelCFGSET;					//���ʷ����ܿ���λ--1:������2:������
	R50IH2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50IH2.pRel47UCFG			= CFG_pby50I_H2_47U;			//��ѹ��������
	R50IH2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50IH2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50IH2.pRelElec 			= &IHmax;						//��ǰ����ָ��		
	R50IH2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50IH2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50IH2.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50IH2.StartRelayWord		= RW_PI50IH2;					//����������μ̵�����
	R50IH2.ActRelayWord			= RW_TR50IH2;					//�������������̵�����
	R50IH2.AccRelayWord			= FALSE1;						//���������̵�����
	R50IH2.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50IH2.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50IH2.PRewardRelayWord  	= FALSE1;
	R50IH2.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50IH2.R47ULKRelayWord		= RW_47UH;
	R50IH2.LockRelayWord		= RW_LKPI50IH2;					//��α����̵�����
	R50IH2.RSTRelayWord			= RW_LKRST50IH2;				//���ر����̵�����
	R50IH2.LoopCtrlRelayWord	= RW_LPC50IH2;					//����ѭ�����Ƽ̵�����
	R50IH2.wReportNum			= R_R50IH2;						//���������������
	R50IH2.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50IH2.dwK35				= NULL;
	R50IH2.RptSerialNum			= &RptSerialNumOfAct;						
    R50IH2.bMakeActRpt       	= TRUE;

	R50IH2.wPickUpReportNum		= R_R50IH2_PI;					//��α������к�
	R50IH2.wResetReportNum		= R_R50IH2_RES;					//���ر������к�
	R50IH2.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50IH2.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50IH2.dwRelTimer			= 0;			 				//��ʱ��
	R50IH2.byRelFCounter		= 0;							//���ϼ�����
	R50IH2.byRelRCounter		= 0;							//���ؼ�����		
	R50IH2.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ����ѹ�����1�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50IM1()
{
	R50IM1.pElecSet				= RS_pn50IM1C;					//��������������ֵ
	R50IM1.pTimeSet				= RS_pn50IM1D;				 	//��������ʱ�䶨ֵ
	R50IM1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50IM1.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50IM1.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50IM1.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50IM1.pRelCFG				= CFG_pby50I_M1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50IM1.pRelSW				= SW_pby50I_M;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50IM1.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM1.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM1.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM1.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50IM1.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50IM1.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50IM1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50IM1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50IM1.pRelElec 			= &IMmax;						//��ǰ����ָ��		
	R50IM1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50IM1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50IM1.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50IM1.StartRelayWord		= RW_PI50IM1;					//����������μ̵�����
	R50IM1.ActRelayWord			= RW_TR50IM1;					//�������������̵�����
	R50IM1.AccRelayWord			= FALSE1;						//���������̵�����
	R50IM1.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50IM1.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50IM1.PRewardRelayWord  	= FALSE1;
	R50IM1.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50IM1.R47ULKRelayWord		= FALSE1;
	R50IM1.LockRelayWord		= RW_LKPI50IM1;					//��α����̵�����
	R50IM1.RSTRelayWord			= RW_LKRST50IM1;				//���ر����̵�����
	R50IM1.LoopCtrlRelayWord	= RW_LPC50IM1;					//����ѭ�����Ƽ̵�����
	R50IM1.wReportNum			= R_R50IM1;						//���������������
	R50IM1.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50IM1.dwK35				= NULL;
	R50IM1.RptSerialNum			= &RptSerialNumOfAct;						
    R50IM1.bMakeActRpt       	= TRUE;

	R50IM1.wPickUpReportNum		= R_R50IM1_PI;					//��α������к�
	R50IM1.wResetReportNum		= R_R50IM1_RES;					//���ر������к�
	R50IM1.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50IM1.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50IM1.dwRelTimer			= 0;			 				//��ʱ��
	R50IM1.byRelFCounter		= 0;							//���ϼ�����
	R50IM1.byRelRCounter		= 0;							//���ؼ�����		
	R50IM1.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ����ѹ�����2�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50IM2()
{
	R50IM2.pElecSet				= RS_pn50IM2C;					//��������������ֵ
	R50IM2.pTimeSet				= RS_pn50IM2D;				 	//��������ʱ�䶨ֵ
	R50IM2.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50IM2.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50IM2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50IM2.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50IM2.pRelCFG				= CFG_pby50I_M2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50IM2.pRelSW				= SW_pby50I_M;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50IM2.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM2.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM2.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IM2.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50IM2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50IM2.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50IM2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50IM2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50IM2.pRelElec 			= &IMmax;						//��ǰ����ָ��		
	R50IM2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50IM2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50IM2.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50IM2.StartRelayWord		= RW_PI50IM2;					//����������μ̵�����
	R50IM2.ActRelayWord			= RW_TR50IM2;					//�������������̵�����
	R50IM2.AccRelayWord			= FALSE1;						//���������̵�����
	R50IM2.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50IM2.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50IM2.PRewardRelayWord  	= FALSE1;
	R50IM2.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50IM2.R47ULKRelayWord		= FALSE1;
	R50IM2.LockRelayWord		= RW_LKPI50IM2;					//��α����̵�����
	R50IM2.RSTRelayWord			= RW_LKRST50IM2;				//���ر����̵�����
	R50IM2.LoopCtrlRelayWord	= RW_LPC50IM2;					//����ѭ�����Ƽ̵�����
	R50IM2.wReportNum			= R_R50IM2;						//���������������
	R50IM2.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50IM2.dwK35				= NULL;
	R50IM2.RptSerialNum			= &RptSerialNumOfAct;						
    R50IM2.bMakeActRpt       	= TRUE;

	R50IM2.wPickUpReportNum		= R_R50IM2_PI;					//��α������к�
	R50IM2.wResetReportNum		= R_R50IM2_RES;					//���ر������к�
	R50IM2.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50IM2.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50IM2.dwRelTimer			= 0;			 				//��ʱ��
	R50IM2.byRelFCounter		= 0;							//���ϼ�����
	R50IM2.byRelRCounter		= 0;							//���ؼ�����		
	R50IM2.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ����ѹ�����1�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50IL1()
{
	R50IL1.pElecSet				= RS_pn50IL1C;					//��������������ֵ
	R50IL1.pTimeSet				= RS_pn50IL1D;				 	//��������ʱ�䶨ֵ
	R50IL1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50IL1.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50IL1.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50IL1.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50IL1.pRelCFG				= CFG_pby50I_L1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50IL1.pRelSW				= SW_pby50I_L;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50IL1.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL1.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL1.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL1.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50IL1.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50IL1.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50IL1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50IL1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50IL1.pRelElec 			= &ILmax;						//��ǰ����ָ��		
	R50IL1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50IL1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50IL1.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50IL1.StartRelayWord		= RW_PI50IL1;					//����������μ̵�����
	R50IL1.ActRelayWord			= RW_TR50IL1;					//�������������̵�����
	R50IL1.AccRelayWord			= FALSE1;						//���������̵�����
	R50IL1.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50IL1.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50IL1.PRewardRelayWord  	= FALSE1;
	R50IL1.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50IL1.R47ULKRelayWord		= FALSE1;
	R50IL1.LockRelayWord		= RW_LKPI50IL1;					//��α����̵�����
	R50IL1.RSTRelayWord			= RW_LKRST50IL1;				//���ر����̵�����
	R50IL1.LoopCtrlRelayWord	= RW_LPC50IL1;					//����ѭ�����Ƽ̵�����
	R50IL1.wReportNum			= R_R50IL1;						//���������������
	R50IL1.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50IL1.dwK35				= NULL;
	R50IL1.RptSerialNum			= &RptSerialNumOfAct;						
    R50IL1.bMakeActRpt       	= TRUE;

	R50IL1.wPickUpReportNum		= R_R50IL1_PI;					//��α������к�
	R50IL1.wResetReportNum		= R_R50IL1_RES;					//���ر������к�
	R50IL1.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50IL1.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50IL1.dwRelTimer			= 0;			 				//��ʱ��
	R50IL1.byRelFCounter		= 0;							//���ϼ�����
	R50IL1.byRelRCounter		= 0;							//���ؼ�����		
	R50IL1.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ����ѹ�����2�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50IL2()
{
	R50IL2.pElecSet				= RS_pn50IL2C;					//��������������ֵ
	R50IL2.pTimeSet				= RS_pn50IL2D;				 	//��������ʱ�䶨ֵ
	R50IL2.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50IL2.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50IL2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50IL2.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50IL2.pRelCFG				= CFG_pby50I_L2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50IL2.pRelSW				= SW_pby50I_L;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50IL2.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL2.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL2.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50IL2.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50IL2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50IL2.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50IL2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50IL2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50IL2.pRelElec 			= &ILmax;						//��ǰ����ָ��		
	R50IL2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50IL2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50IL2.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50IL2.StartRelayWord		= RW_PI50IL2;					//����������μ̵�����
	R50IL2.ActRelayWord			= RW_TR50IL2;					//�������������̵�����
	R50IL2.AccRelayWord			= FALSE1;						//���������̵�����
	R50IL2.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50IL2.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50IL2.PRewardRelayWord  	= FALSE1;
	R50IL2.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50IL2.R47ULKRelayWord		= FALSE1;
	R50IL2.LockRelayWord		= RW_LKPI50IL2;					//��α����̵�����
	R50IL2.RSTRelayWord			= RW_LKRST50IL2;				//���ر����̵�����
	R50IL2.LoopCtrlRelayWord	= RW_LPC50IL2;					//����ѭ�����Ƽ̵�����
	R50IL2.wReportNum			= R_R50IL2;						//���������������
	R50IL2.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50IL2.dwK35				= NULL;
	R50IL2.RptSerialNum			= &RptSerialNumOfAct;						
    R50IL2.bMakeActRpt       	= TRUE;

	R50IL2.wPickUpReportNum		= R_R50IL2_PI;					//��α������к�
	R50IL2.wResetReportNum		= R_R50IL2_RES;					//���ر������к�
	R50IL2.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50IL2.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50IL2.dwRelTimer			= 0;			 				//��ʱ��
	R50IL2.byRelFCounter		= 0;							//���ϼ�����
	R50IL2.byRelRCounter		= 0;							//���ؼ�����		
	R50IL2.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ��������1�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50OL1()
{
	R50OL1.pElecSet				= RS_pn50OL1C;					//��������������ֵ
	R50OL1.pTimeSet				= RS_pn50OL1D;				 	//��������ʱ�䶨ֵ
	R50OL1.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50OL1.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50OL1.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50OL1.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50OL1.pRelCFG				= CFG_pby50OL_1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50OL1.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50OL1.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL1.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL1.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL1.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50OL1.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50OL1.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50OL1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50OL1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50OL1.pRelElec 			= &IHmax;						//��ǰ����ָ��		
	R50OL1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50OL1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50OL1.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50OL1.StartRelayWord		= RW_PI50OL1;					//����������μ̵�����
	R50OL1.ActRelayWord			= RW_TR50OL1;					//�������������̵�����
	R50OL1.AccRelayWord			= FALSE1;						//���������̵�����
	R50OL1.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50OL1.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50OL1.PRewardRelayWord  	= FALSE1;
	R50OL1.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50OL1.R47ULKRelayWord		= FALSE1;
	R50OL1.LockRelayWord		= RW_LKPI50OL1;					//��α����̵�����
	R50OL1.RSTRelayWord			= RW_LKRST50OL1;				//���ر����̵�����
	R50OL1.LoopCtrlRelayWord	= RW_LPC50OL1;					//����ѭ�����Ƽ̵�����
	R50OL1.wReportNum			= R_R50OL1;						//���������������
	R50OL1.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50OL1.dwK35				= NULL;
	R50OL1.RptSerialNum			= &RptSerialNumOfAct;						
    R50OL1.bMakeActRpt       	= TRUE;

	R50OL1.wPickUpReportNum		= R_R50OL1_PI;					//��α������к�
	R50OL1.wResetReportNum		= R_R50OL1_RES;					//���ر������к�
	R50OL1.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50OL1.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50OL1.dwRelTimer			= 0;			 				//��ʱ��
	R50OL1.byRelFCounter		= 0;							//���ϼ�����
	R50OL1.byRelRCounter		= 0;							//���ؼ�����		
	R50OL1.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ��������2�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50OL2()
{
	R50OL2.pElecSet				= RS_pn50OL2C;					//��������������ֵ
	R50OL2.pTimeSet				= RS_pn50OL2D;				 	//��������ʱ�䶨ֵ
	R50OL2.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50OL2.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50OL2.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50OL2.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50OL2.pRelCFG				= CFG_pby50OL_2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50OL2.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50OL2.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL2.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL2.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL2.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL2.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50OL2.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50OL2.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50OL2.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50OL2.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50OL2.pRelElec 			= &IHmax;						//��ǰ����ָ��		
	R50OL2.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50OL2.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50OL2.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50OL2.StartRelayWord		= RW_PI50OL2;					//����������μ̵�����
	R50OL2.ActRelayWord			= RW_TR50OL2;					//�������������̵�����
	R50OL2.AccRelayWord			= FALSE1;						//���������̵�����
	R50OL2.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50OL2.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50OL2.PRewardRelayWord  	= FALSE1;
	R50OL2.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50OL2.R47ULKRelayWord		= FALSE1;
	R50OL2.LockRelayWord		= RW_LKPI50OL2;					//��α����̵�����
	R50OL2.RSTRelayWord			= RW_LKRST50OL2;				//���ر����̵�����
	R50OL2.LoopCtrlRelayWord	= RW_LPC50OL2;					//����ѭ�����Ƽ̵�����
	R50OL2.wReportNum			= R_R50OL2;						//���������������
	R50OL2.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50OL2.dwK35				= NULL;
	R50OL2.RptSerialNum			= &RptSerialNumOfAct;						
    R50OL2.bMakeActRpt       	= TRUE;

	R50OL2.wPickUpReportNum		= R_R50OL2_PI;					//��α������к�
	R50OL2.wResetReportNum		= R_R50OL2_RES;					//���ر������к�
	R50OL2.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50OL2.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50OL2.dwRelTimer			= 0;			 				//��ʱ��
	R50OL2.byRelFCounter		= 0;							//���ϼ�����
	R50OL2.byRelRCounter		= 0;							//���ؼ�����		
	R50OL2.wExcepWordNum     	= 8;
}
//******************************************************************************************
// ����: ��ʼ��������3�α���Ԫ��      
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitR50OL3()
{
	R50OL3.pElecSet				= RS_pn50OL3C;					//��������������ֵ
	R50OL3.pTimeSet				= RS_pn50OL3D;				 	//��������ʱ�䶨ֵ
	R50OL3.pHarmKhr1Set			= NULL;							//��������г����Ȩϵ��
	R50OL3.pVolLockSet			= NULL;							//����������ѹ������ֵ
	R50OL3.pAccActTimeSet		= NULL;							//������������ٶ���ʱ�䶨ֵ
	R50OL3.pH2Coef				= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	R50OL3.pRelCFG				= CFG_pby50OL_3;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	R50OL3.pRelSW				= SW_pby50OL;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	R50OL3.pRelHLockCFG			= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL3.pRelHSuprCFG			= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL3.pRelPDCFG			= &RelCFGCLR;		    	//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL3.pRelVolLockCFG		= &RelCFGCLR;					//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	R50OL3.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	R50OL3.pRelAccCFG			= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	R50OL3.pRel47UCFG			= &RelCFGCLR;			//��ѹ��������
	R50OL3.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	R50OL3.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	R50OL3.pRelElec 			= &IHmax;						//��ǰ����ָ��		
	R50OL3.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	R50OL3.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	R50OL3.pRelVolt				= NULL;							//��ǰ���������ĵ�ѹ��ָ��

	R50OL3.StartRelayWord		= RW_PI50OL3;					//����������μ̵�����
	R50OL3.ActRelayWord			= RW_TR50OL3;					//�������������̵�����
	R50OL3.AccRelayWord			= FALSE1;						//���������̵�����
	R50OL3.AccActRelayWord		= FALSE1;						//����ٶ����̵�����
	R50OL3.PForwardRelayWord	= FALSE1;						//���ʷ���̵�����
	R50OL3.PRewardRelayWord  	= FALSE1;
	R50OL3.H2LockRelayWord		= FALSE1;						//����г�������̵�����
	R50OL3.R47ULKRelayWord		= FALSE1;
	R50OL3.LockRelayWord		= RW_LKPI50OL3;					//��α����̵�����
	R50OL3.RSTRelayWord			= RW_LKRST50OL3;				//���ر����̵�����
	R50OL3.LoopCtrlRelayWord	= RW_LPC50OL3;					//����ѭ�����Ƽ̵�����
	R50OL3.wReportNum			= R_R50OL3;						//���������������
	R50OL3.wAccReportNum		= 0xFFFF;						//���ٶ����������

	R50OL3.dwK35				= NULL;
	R50OL3.RptSerialNum			= &RptSerialNumOfAct;						
    R50OL3.bMakeActRpt       	= TRUE;

	R50OL3.wPickUpReportNum		= R_R50OL3_PI;					//��α������к�
	R50OL3.wResetReportNum		= R_R50OL3_RES;					//���ر������к�
	R50OL3.wAccPickUpRptNum		= 0xFFFF;						//��α������к�
	R50OL3.wAccResetRptNum		= 0xFFFF;						//���ر������к�

	R50OL3.dwRelTimer			= 0;			 				//��ʱ��
	R50OL3.byRelFCounter		= 0;							//���ϼ�����
	R50OL3.byRelRCounter		= 0;							//���ؼ�����		
	R50OL3.wExcepWordNum     	= 8;
}
/************************************************************************************************
*����: ��ʼ����ѹ����Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitR27U()
{
	Rel27U.pELeSet						= RS_pn27UV;						//������ֵ 
	Rel27U.pTimeSet						= RS_pn27UD;						//ʱ�䶨ֵ 
	Rel27U.pLockCurSet					= RS_pn27U_50I;					//����������ֵ 

	Rel27U.pRelCFG						= CFG_pby27U;					  	 //Ƿ���������� 
	Rel27U.pRelSW						= SW_pby27U;					  	 //Ƿ��������ѹ�� 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//Ƿ������Ͷ�澯
	Rel27U.pRelCurLockCFG				= CFG_pby27U_50I;
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ�� 

	Rel27U.pRelElec						= &UHmax_ph;						//��ǰ�ĵ���ָ�� 
	Rel27U.pRelLockElec					= &IHmax;					//�������� 
	Rel27U.ActRelayWord					= RW_TR27U;					//Ƿ�����������̵����� 
	Rel27U.StartRelayWord				= RW_PI27U;					//Ƿ��������μ̵����� 
	Rel27U.AlarmRelayWord				= FALSE1;					//Ƿ�������澯�̵����� 
	Rel27U.LockRelayWord				= RW_LKPI27U;				//��α����̵����� 
	Rel27U.RSTRelayWord					= RW_LKRST27U;					//���Ϸ��ر����̵����� 
	Rel27U.LoopCtrlRelayWord			= FALSE1;				//����ѭ�����Ƽ̵����� 
	Rel27U.wR52BFRealyword				= RW_52BF_ALL;
	Rel27U.wActRptNum					= R_R27U;					//Ƿ����������������� 
	Rel27U.wAlarmRptNum					= 0xFFFF;					//Ƿ�������澯������� 
	Rel27U.wTRFailRptNum				= R_52BF;
	Rel27U.wPickUpRptNum				= R_PICKUP;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
	
	Rel27U.R27RptSerialNum				= 0;
	Rel27U.dwRelTimer					= 0;						//Ƿ��������ʱ�� 	
	Rel27U.byRelFCounter				= 0;					//���ϼ����� 
	Rel27U.byRelRCounter				= 0;					//���ؼ����� 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
void CRelayTask::InitR87IAlarm()
{	
	
	R87Alarm.p87RSW				= SW_pby87R;		//��ѹ��
	R87Alarm.p87R_A_CFG			= CFG_pby87R_A;		//��������
	R87Alarm.p87R_B_CFG			= CFG_pby87R_B;		//��������
	R87Alarm.p87R_C_CFG			= CFG_pby87R_C;		//��������

	R87Alarm.p87RIASet			= RS_pn87RIA; 	//A�������ֵ
	R87Alarm.p87RIBSet			= RS_pn87RIB; 	//B�������ֵ
	R87Alarm.p87RICSet			= RS_pn87RIC; 	//C�������ֵ
	R87Alarm.p87RIKSet			= RS_pnK87AR;		//����Խ�޸澯����

	R87Alarm.pIDA				= &IDA;			//����A
	R87Alarm.pIDB				= &IDB;			//����B
	R87Alarm.pIDC				= &IDC;			//����C

	R87Alarm.AlarmCnt			= R87IALARMTIME;		//����Խ�޸澯ʱ��
	R87Alarm.ARRW				= RW_AR87;			//����Խ�޸澯�̵�����
	R87Alarm.wRelRptNum			= R_AR87R;		//����Խ�޸澯�������к�

	R87Alarm.ACnt				= 0;			//A�������
	R87Alarm.BCnt				= 0;			//B�������
	R87Alarm.CCnt				= 0;			//C�������
	R87Alarm.AAlarmFlag			= FALSE;		//A��澯��־
	R87Alarm.BAlarmFlag			= FALSE;		//B��澯��־
	R87Alarm.CAlarmFlag			= FALSE;		//C��澯��־
	R87Alarm.MakeRptFlag		= FALSE;	//���澯�����־
	R87Alarm.wExcepWordNum      = 16;
}

//******************************************************************************************
// ����: ��ʼ�������ṹ���Լ���Ϣ
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitStructCheck()
{
	WORD acc = 0x55aa;
	
	StructCheck[0].pstruct = (WORD*)&R87U_A;
	StructCheck[0].bigsize = sizeof(R87U_A)/2 - R87U_A.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&R87U_B;
	StructCheck[1].bigsize = sizeof(R87U_B)/2 - R87U_B.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&R87U_C;
	StructCheck[2].bigsize = sizeof(R87U_C)/2 - R87U_C.wExcepWordNum;	
	StructCheck[3].pstruct = (WORD*)&R87R_A;
	StructCheck[3].bigsize = sizeof(R87R_A)/2 - R87R_A.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&R87R_B;
	StructCheck[4].bigsize = sizeof(R87R_B)/2 - R87R_B.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&R87R_C;
	StructCheck[5].bigsize = sizeof(R87R_C)/2 - R87R_C.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&R50IH1;
	StructCheck[6].bigsize = sizeof(R50IH1)/2 - R50IH1.wExcepWordNum;	
	StructCheck[7].pstruct = (WORD*)&R50IH2;
	StructCheck[7].bigsize = sizeof(R50IH2)/2 - R50IH2.wExcepWordNum;	
	StructCheck[8].pstruct = (WORD*)&R50IM1;
	StructCheck[8].bigsize = sizeof(R50IM1)/2 - R50IM1.wExcepWordNum;	
	StructCheck[9].pstruct = (WORD*)&R50IM2;
	StructCheck[9].bigsize = sizeof(R50IM2)/2 - R50IM2.wExcepWordNum;	
	StructCheck[10].pstruct = (WORD*)&R50IL1;
	StructCheck[10].bigsize = sizeof(R50IL1)/2 - R50IL1.wExcepWordNum;	
	StructCheck[11].pstruct = (WORD*)&R50IL2;
	StructCheck[11].bigsize = sizeof(R50IL2)/2 - R50IL2.wExcepWordNum;	
	StructCheck[12].pstruct = (WORD*)&R50OL1;
	StructCheck[12].bigsize = sizeof(R50OL1)/2 - R50OL1.wExcepWordNum;	
	StructCheck[13].pstruct = (WORD*)&R50OL2;
	StructCheck[13].bigsize = sizeof(R50OL2)/2 - R50OL2.wExcepWordNum;	
	StructCheck[14].pstruct = (WORD*)&R50OL3;
	StructCheck[14].bigsize = sizeof(R50OL3)/2 - R50OL3.wExcepWordNum;	
	StructCheck[15].pstruct = (WORD*)&Rel27U;
	StructCheck[15].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;	
	StructCheck[16].pstruct = (WORD*)&R87Alarm;
	StructCheck[16].bigsize = sizeof(R87Alarm)/2 - R87Alarm.wExcepWordNum;	
	StructCheck[17].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[17].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;

	for(LONG j=0;j<18;j++)
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
	if((ReadRelayWord(RW_52BFH))||(ReadRelayWord(RW_52BFM))||(ReadRelayWord(RW_52BFL)))
		return FALSE;
	BOOL  relayInputFlag = FALSE;
	if((*R87U_A.pRelCFG == CFG_STATUS_SET) && (*R87U_A.pRelSW == SW_STATUS_SET))   //A���ٶ�
	{
		relayInputFlag = TRUE;
		if(R87U_A.pRelElec->Mod > *R87U_A.pElecSet)
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
	if((*R87U_B.pRelCFG == CFG_STATUS_SET) && (*R87U_B.pRelSW == SW_STATUS_SET))   //B���ٶ�
	{
		relayInputFlag = TRUE;
		if(R87U_B.pRelElec->Mod > *R87U_B.pElecSet)
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
	if((*R87U_C.pRelCFG == CFG_STATUS_SET) && (*R87U_C.pRelSW == SW_STATUS_SET))   //C���ٶ�
	{
		relayInputFlag = TRUE;
		if(R87U_C.pRelElec->Mod > *R87U_C.pElecSet)
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

	if((*R87R_A.p87RCFG == CFG_STATUS_SET) && (*R87R_A.p87RSW == SW_STATUS_SET))   //A����ʲ
	{
		relayInputFlag = TRUE;
		if(R87R_A.pId->Mod > *R87R_A.p87RISet)
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
	if((*R87R_B.p87RCFG == CFG_STATUS_SET) && (*R87R_B.p87RSW == SW_STATUS_SET))   //B����ʲ
	{
		relayInputFlag = TRUE;
		if(R87R_B.pId->Mod > *R87R_B.p87RISet)
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
	if((*R87R_C.p87RCFG == CFG_STATUS_SET) && (*R87R_C.p87RSW == SW_STATUS_SET))   //C����ʲ
	{
		relayInputFlag = TRUE;
		if(R87R_C.pId->Mod > *R87R_C.p87RISet)
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
	if((*R50IH1.pRelCFG == CFG_STATUS_SET) && (*R50IH1.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50IH1.pRelElec->Mod > *R50IH1.pElecSet)
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
	if((*R50IH2.pRelCFG == CFG_STATUS_SET) && (*R50IH2.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50IH2.pRelElec->Mod > *R50IH2.pElecSet)
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
	if((*R50IM1.pRelCFG == CFG_STATUS_SET) && (*R50IM1.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50IM1.pRelElec->Mod > *R50IM1.pElecSet)
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
	if((*R50IM2.pRelCFG == CFG_STATUS_SET) && (*R50IM2.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50IM2.pRelElec->Mod > *R50IM2.pElecSet)
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
	if((*R50IL1.pRelCFG == CFG_STATUS_SET) && (*R50IL1.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50IL1.pRelElec->Mod > *R50IL1.pElecSet)
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
	if((*R50IL2.pRelCFG == CFG_STATUS_SET) && (*R50IL2.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50IL2.pRelElec->Mod > *R50IL2.pElecSet)
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
	if((*R50OL1.pRelCFG == CFG_STATUS_SET) && (*R50OL1.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50OL1.pRelElec->Mod > *R50OL1.pElecSet)
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
	if((*R50OL2.pRelCFG == CFG_STATUS_SET) && (*R50OL2.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50OL2.pRelElec->Mod > *R50OL2.pElecSet)
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
	if((*R50OL3.pRelCFG == CFG_STATUS_SET) && (*R50OL3.pRelSW == SW_STATUS_SET)) //�߲����1��
	{
		relayInputFlag = TRUE;
		if(R50OL3.pRelElec->Mod > *R50OL3.pElecSet)
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
	
	BYTE  pChannel[]={CHANNEL_6535_IHA,CHANNEL_6535_IHB,CHANNEL_6535_IHC,CHANNEL_6535_IMA, \
						CHANNEL_6535_IMB,CHANNEL_6535_IMC,CHANNEL_6535_ILA,CHANNEL_6535_ILB, \
						CHANNEL_6535_ILC,CHANNEL_6535_IDA,CHANNEL_6535_IDB,CHANNEL_6535_IDC};		
	if(relayInputFlag == TRUE)
	{
		STATUS rc;
		rc = ReadPreSingleData(dwPickDataBuf,pChannel,12,RELAY_SAMP_INTERVAL);

		if(rc != ERR_SUCCESS)
		{
			ReportMsg("Serious Error: Read sample data failure!!!");
			return FALSE;
		}
		
		for(LONG j=0;j<12;j++)//������ͻ��������ͨ����
		{
			PickCheck = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{			
				lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickDataBuf[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue = MULTIPLES*ToAbs((ToAbs((lSampData1-lSampData2))-ToAbs((lSampData2-lSampDate3))));
//				lPickValue = MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));

				//���ø����ż�ֵ�жϣ���I>0.25*��I'+ C,���Ц�I'Ϊ�ϸ�һ���ڽ��ĵ��ͻ����ֵ��CΪͻ����������ֵ
				if(lPickValue*4>*RS_pnPIC*4+lPickUpValueArray[j])
				//if(lPickValue>*RS_pnPIC)
				{
					PickCheck++;
					if(PickCheck==3)
					{   //������������ʱ������
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime.Lo-(i*625+500)/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(PickUpRW, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
						RptSerialNumOfStart = g_RptSerialNum;
						PickCheck =0;
						for(LONG m=0;m<3;m++)
						{
							PickCheckNext[m] =0;
							lPickUpValueArray[m]=0;
						}
						return TRUE;	
					}
					if(i<2)
					{
						PickCheckNext[j]++;
						if(PickCheckNext[j]==3)
						{							
							QWORD PickTime;
							ReadAbsTime(&PickUpTime);
							PickTime = PickUpTime.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime.Lo-(i*625+500)/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);
							
							WriteRelayWord(PickUpRW, RELAY_ON);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //�����ݿ���д����������
							RptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //�����ݿ���д����������
							RptSerialNumOfStart = g_RptSerialNum;
							PickCheck =0;
							for(LONG m=0;m<3;m++)
							{
								PickCheckNext[m] =0;
								lPickUpValueArray[m]=0;
							}
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
				lPickUpValueArray[j] = lPickValue;				
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
	if(FaultOnFlag ==FALSE)   						//��δ������ϴ���
	{		
		RelayPickUpCHK();					//ִ���������
		if(ReadRelayWord(RW_PICKUP))
		{			
			ResetAllRelay(TRUE);
			FaultSINum=RELAY_SAMP_INTERVAL;       //������������ʼ�����ϴ���������������
			FaultOnFlag=TRUE;
//			return;							//������ʽʵ����ٱ�������ʱ����
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(FaultSINum<POINT_PER_PERIOD-3)					//���ϴ���������������С��һ�ܲ���������
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
		
		if((*R50IH1.pRelPDCFG == CFG_STATUS_SET)||(*R50IH2.pRelPDCFG == CFG_STATUS_SET))
		{
			if(wHIPhase == PHASE_A)
			{
				if(!HA_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_74VT))
						HA_PDWithMemoryFlag = PowerDirCheck_90(&UBC,&IHA,dwPDDataBuf,*RS_pnA_50IH,*CF_pnUp,RW_PFHA,RW_PRHA,0,FALSE);
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
					if(!ReadRelayWord(RW_74VT))
						HB_PDWithMemoryFlag = PowerDirCheck_90(&UCA,&IHB,dwPDDataBuf,*RS_pnA_50IH,*CF_pnUp,RW_PFHB,RW_PRHB,0,FALSE);
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
					if(!ReadRelayWord(RW_74VT))
						HC_PDWithMemoryFlag = PowerDirCheck_90(&UAB,&IHC,dwPDDataBuf,*RS_pnA_50IH,*CF_pnUp,RW_PFHC,RW_PRHC,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHC, RELAY_OFF);
						WriteRelayWord(RW_PRHC, RELAY_OFF);
					}
				}
			}
		}
		ResetIdHLock();
		R68IdAped();
		R68IdH2M();
		R68IdH2C();
		R68IdH2I();
		R68IrH2M();
		R68IdH5();	
		R68IdHI();
		GeneralOverValueRelay(&R87U_A);				//��ٶ�A
		GeneralOverValueRelay(&R87U_B);				//��ٶ�B
		GeneralOverValueRelay(&R87U_C);				//��ٶ�C
		Rel87RI(&R87R_A);				//���ʲA
		Rel87RI(&R87R_B);				//���ʲB
		Rel87RI(&R87R_C);				//���ʲC		
		GeneralOverValueRelay(&R50IH1);				    //��ѹ�����1��
		GeneralOverValueRelay(&R50IH2);					//��ѹ�����2��
		GeneralOverValueRelay(&R50IM1);					//��ѹ�����1��
		GeneralOverValueRelay(&R50IM2);					//��ѹ�����2��
		GeneralOverValueRelay(&R50IL1);					//��ѹ�����1��
		GeneralOverValueRelay(&R50IL2);					//��ѹ�����2��
		GeneralOverValueRelay(&R50OL1);					//������1��
		GeneralOverValueRelay(&R50OL2);					//������2��
		GeneralOverValueRelay(&R50OL3);					//������3��
		
		if(ReadRelayWord(RW_52BFH)||ReadRelayWord(RW_52BFM)||ReadRelayWord(RW_52BFL))
		{
			if(!m_bHaveTripFailedRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//����·����բʧ�ܱ���
				m_bHaveTripFailedRpt = TRUE;
			}
		}
		// 1���б���ѭ�����Ƽ̵����ֱ���λ���ȴ��������������˳�
		if(ReadRelayWord(R87U_A.LoopCtrlRelayWord)||ReadRelayWord(R87U_B.LoopCtrlRelayWord)||ReadRelayWord(R87U_C.LoopCtrlRelayWord) \
		   ||ReadRelayWord(R87R_A.LoopCtrlRelayWord)||ReadRelayWord(R87R_B.LoopCtrlRelayWord)||ReadRelayWord(R87R_C.LoopCtrlRelayWord) \
		   ||ReadRelayWord(R50IH1.LoopCtrlRelayWord)||ReadRelayWord(R50IH2.LoopCtrlRelayWord)||ReadRelayWord(R50IM1.LoopCtrlRelayWord) \
		   ||ReadRelayWord(R50IM2.LoopCtrlRelayWord)||ReadRelayWord(R50IL1.LoopCtrlRelayWord)||ReadRelayWord(R50IL2.LoopCtrlRelayWord) \
		   ||ReadRelayWord(R50OL1.LoopCtrlRelayWord)||ReadRelayWord(R50OL2.LoopCtrlRelayWord)||ReadRelayWord(R50OL3.LoopCtrlRelayWord))
		{
			if(ReadRelayWord(RW_52BFH)||ReadRelayWord(RW_52BFM)||ReadRelayWord(RW_52BFL))
			{				
//				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//����·����բʧ�ܱ���
				WriteRelayWord(PickUpRW, RELAY_OFF);		//�屣�������̵�����
				FaultCheckOutFlag = FALSE;					//�������ϱ�־
				FaultOnFlag = FALSE;						//�����ѭ�������־
				HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
				m_bFaultLKFlag = FALSE; 					//���������޶���ʱ����ʱ��������־				
				DB_RelayEnd(RptSerialNumOfAct); 			//�����ݿ���д���������
				m_bHaveTripFailedRpt = FALSE;	//������·����բʧ�ܱ����־
				return; 				
			}		
			FaultLockTime = dwRelayMSCnt;		
			FaultCheckOutFlag = TRUE;
			return;
		}
		// 2���ޱ���ѭ�����Ƽ̵����ֱ���λ����ʱ�����Ѷ���������û�������κα�����������
		else				
		{
			// 2.1 ������ϱ�־����λ����ʾ����ѭ�����Ƽ̵�������������λ--��һ��������--�ȴ�RS_pnSOTFTʱ����˳�
			if(FaultCheckOutFlag == TRUE)     
			{
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime > 500 )  //�޹���ʱ�䳬�����ڹ��ϼ��ʱ��
				{						
					WriteRelayWord(PickUpRW, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					DB_RelayEnd(RptSerialNumOfAct);				//�����ݿ���д���������
					ResetAllRelay(TRUE);					
					m_bHaveTripFailedRpt = FALSE;				//������·����բʧ�ܱ����־
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
					FaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				if(dwRelayMSCnt -FaultLockTime>500 )	
				{
					WriteRelayWord(PickUpRW, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־						
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					DB_RelayEnd(RptSerialNumOfAct); 
					ResetAllRelay(TRUE);
					m_bHaveTripFailedRpt = FALSE;				//������·����բʧ�ܱ����־
					return;					
				}
				else	//��ʱδ����ֱ�ӷ���
					return;
			}			
		}		
	}
}
void CRelayTask::Process52BF()
{
	if((ReadRelayWord(RW_52BFH))||(ReadRelayWord(RW_52BFM))||(ReadRelayWord(RW_52BFL)))
	{
		m_b52BFFlag = TRUE;
	}
	if(m_b52BFFlag)
	{
		if((!ReadRelayWord(RW_52BFH))&&(!ReadRelayWord(RW_52BFM))&&(!ReadRelayWord(RW_52BFL)))
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
	ResetOverValueRelay(&R87U_A,resetFlag);
	ResetOverValueRelay(&R87U_B,resetFlag);
	ResetOverValueRelay(&R87U_C,resetFlag);
	Reset87RI(&R87R_A,resetFlag);
	Reset87RI(&R87R_B,resetFlag);
	Reset87RI(&R87R_C,resetFlag);
	ResetOverValueRelay(&R50IH1,resetFlag);
	ResetOverValueRelay(&R50IH2,resetFlag);
	ResetOverValueRelay(&R50IM1,resetFlag);
	ResetOverValueRelay(&R50IM2,resetFlag);
	ResetOverValueRelay(&R50IL1,resetFlag);
	ResetOverValueRelay(&R50IL2,resetFlag);
	ResetOverValueRelay(&R50OL1,resetFlag);
	ResetOverValueRelay(&R50OL2,resetFlag);
	ResetOverValueRelay(&R50OL3,resetFlag);
}

void CRelayTask::PTCheck()
{
	if(*CFG_pby74VT == CFG_STATUS_CLR)
	{
		HPTCheckTimer1 = 0;
		HPTCheckTimer2 = 0;
		HPTCheckTimer3 = 0;
		HPTCheckTimer4 = 0;
		HPTMakeReportFlag = FALSE;
		WriteRelayWord(RW_74VT,RELAY_OFF);	
		
		return;
	}
	//��ѹ��PT�����ж�
	if((UHmin_li.Mod>=*RS_pn74VTV)&&(U2H.Mod<=U2Set))
	{
		HPTCheckTimer1 = 0;
		HPTCheckTimer2 = 0;
		HPTCheckTimer3 = 0;
		HPTCheckTimer4 = 0;
		HPTMakeReportFlag = FALSE;
		WriteRelayWord(RW_74VT,RELAY_OFF);			
	}
	if((UAB.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC))
		HPTCheckTimer1++;
	else
		HPTCheckTimer1=0;
	
	if((UBC.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC))
		HPTCheckTimer2++;
	else
		HPTCheckTimer2=0;
	
	if((UCA.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC))
		HPTCheckTimer3++;
	else
		HPTCheckTimer3=0;
	
	if(U2H.Mod>U2Set)
		HPTCheckTimer4++;
	else
		HPTCheckTimer4=0;
		
	if(((HPTCheckTimer1*m_PTCheckTimer)>*RS_pn74VTD)||((HPTCheckTimer2*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer3*m_PTCheckTimer)>*RS_pn74VTD)||((HPTCheckTimer4*m_PTCheckTimer)>*RS_pn74VTD))    
	{
		if(!HPTMakeReportFlag)
		{
			MakeAlarmRpt(R_R74VT);
			HPTMakeReportFlag = TRUE;
			WriteRelayWord(RW_74VT,RELAY_ON);
		}
	}
}
