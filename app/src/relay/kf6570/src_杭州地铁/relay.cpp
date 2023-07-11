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
*           		�����㷨����     FOR  6570�����ۺϲ��װ��                                                             
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
*      yanzh             09/10/27   ��ʼ����                         
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

extern DWORD dwRelayMSCnt;		//�����ú��������
extern DWORD g_hDbaseTaskID;
extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern DWORD dwRelayDataPtr;	//AD��ǰдָ��
extern BYTE RptSerialNum;
extern BOOL RelayCheckErr;			//�����Լ����
extern BOOL RelayCheckInitOK;
extern DWORD dwADResumeCnt;
extern BYTE *g_MaintSW;

DWORD dwRelayTaskID;
BOOL RelayTaskInitOK = FALSE;
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
	
	dwFreqMask = (0x01<<CHANNEL_6570_UHA);
	dwFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*POINT_PER_PERIOD*2);
	ASSERT(dwFreqBuf != NULL);
	if(dwFreqBuf == NULL)
	{
		LogError("CRelayTask,dwFreqBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwFreqBuf, 0, sizeof(sizeof(SHORT)*POINT_PER_PERIOD*2));


	
	dwChanDataMask = ( (0x01<<CHANNEL_6570_UHA)|(0x01<<CHANNEL_6570_UHB)|(0x01<<CHANNEL_6570_UHC)|(0x01<<CHANNEL_6570_U0) \
					|(0x01<<CHANNEL_6570_ULA)|(0x01<<CHANNEL_6570_ULB)|(0x01<<CHANNEL_6570_ULC)|(0x01<<CHANNEL_6570_IHA) \
					|(0x01<<CHANNEL_6570_IHB)|(0x01<<CHANNEL_6570_IHC)|(0x01<<CHANNEL_6570_ILA) \
					|(0x01<<CHANNEL_6570_ILB)|(0x01<<CHANNEL_6570_ILC)|(0x01<<CHANNEL_6570_I1)|(0x01<<CHANNEL_6570_I2));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	//��ʼ����ȡ����������ʱ������
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

	
	//��ʼ���߼����̲���
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
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();
	RelayTaskInitOK = TRUE;
	m_bLastRecStatusFlag = FALSE;
	m_bLogicChanged = FALSE;
	SetTimer(1,20);
	g_MaintSW = SW_pbyMAINT;

}
/****************************************************************************************
��  ��: InitRealy
��  ��: ��ʼ��������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void  CRelayTask::InitRealy(void)
{
	
}
/****************************************************************************************
��  ��: OnTimeOut
��  ��: ʱ�䶨ʱ����Ӧ����
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::OnTimeOut(DWORD id)
{	
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
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
	
	//���Ƚ��п����Լ�,ÿ�α��������һ·����
	if(!m_bLogicChanged)
		BoBreakCheck();
		
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	CaluUI();          //���ݲ������ݣ��������It,If,U1,U2	

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
	
	WriteDCChannel();
/*
	static DWORD dwFreq = 5000;	
	static LONG freqcounter =0;
	TestFrequence(dwFreq,UHA.Mod,dwFreqMask,dwFreqBuf,m_dwFreq,m_dwTs);
	dwFreq = m_dwFreq;
	ReportMsg("freq= %d",m_dwFreq);
	g_dwFreqPWM = m_dwFreq*32;
	tesetbuf[freqcounter]= m_dwFreq;	
	freqcounter++;
	freqcounter %= 200;
*/

	
}

/****************************************************************************************
��  ��: CaluUI
��  ��: �����ͨ���ĵ�����ѹģֵ
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::CaluUI(void)
{	
	// 1.UHA
	CaluBaseFourier_S15(dwSampBuf,&UHA);
	CaluModValue(&UHA);
	if(UHA.Mod >= *RS_pn59HA)
	{
		WriteRelayWord(RW_59HA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59HA, RELAY_OFF);
	}
	if(UHA.Mod >= *RS_pn27HA)
	{
		WriteRelayWord(RW_27HA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27HA, RELAY_ON);
	}
	// 2.UHB	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UHB);
	CaluModValue(&UHB);
	if(UHB.Mod >= *RS_pn59HB)
	{
		WriteRelayWord(RW_59HB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59HB, RELAY_OFF);
	}
	if(UHB.Mod >= *RS_pn27HB)
	{
		WriteRelayWord(RW_27HB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27HB, RELAY_ON);
	}
	// 3.UHC	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UHC);
	CaluModValue(&UHC);
	if(UHC.Mod >= *RS_pn59HC)
	{
		WriteRelayWord(RW_59HC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59HC, RELAY_OFF);
	}
	if(UHC.Mod >= *RS_pn27HC)
	{
		WriteRelayWord(RW_27HC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27HC, RELAY_ON);
	}
	// 3.U0	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0);
	CaluModValue(&U0);
	if(U0.Mod >= *RS_pn59N)
	{
		WriteRelayWord(RW_59N, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59N, RELAY_OFF);
	}
	if(U0.Mod >= *RS_pn27N)
	{
		WriteRelayWord(RW_27N, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27N, RELAY_ON);
	}
	// 3.ULA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&ULA);
	CaluModValue(&ULA);
	if(ULA.Mod >= *RS_pn59LA)
	{
		WriteRelayWord(RW_59LA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LA, RELAY_OFF);
	}
	if(ULA.Mod >= *RS_pn27LA)
	{
		WriteRelayWord(RW_27LA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LA, RELAY_ON);
	}
	// 3.ULB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&ULB);
	CaluModValue(&ULB);
	if(ULB.Mod >= *RS_pn59LB)
	{
		WriteRelayWord(RW_59LB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LB, RELAY_OFF);
	}
	if(ULB.Mod >= *RS_pn27LB)
	{
		WriteRelayWord(RW_27LB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LB, RELAY_ON);
	}
	// 3.ULB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&ULC);
	CaluModValue(&ULC);
	if(ULC.Mod >= *RS_pn59LC)
	{
		WriteRelayWord(RW_59LC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LC, RELAY_OFF);
	}
	if(ULC.Mod >= *RS_pn27LC)
	{
		WriteRelayWord(RW_27LC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LC, RELAY_ON);
	}	
	// 3.IHA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&IHA);
	CaluModValue(&IHA);
	if(IHA.Mod >= *RS_pn50HA)
	{
		WriteRelayWord(RW_50HA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HA, RELAY_OFF);
	}
	if(IHA.Mod >= *RS_pn37HA)
	{
		WriteRelayWord(RW_37HA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HA, RELAY_ON);
	}
	// 3.IHA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IHB);
	CaluModValue(&IHB);
	if(IHB.Mod >= *RS_pn50HB)
	{
		WriteRelayWord(RW_50HB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HB, RELAY_OFF);
	}
	if(IHB.Mod >= *RS_pn37HB)
	{
		WriteRelayWord(RW_37HB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HB, RELAY_ON);
	}

	// 3.IHA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&IHC);
	CaluModValue(&IHC);
	if(IHC.Mod >= *RS_pn50HC)
	{
		WriteRelayWord(RW_50HC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HC, RELAY_OFF);
	}
	if(IHC.Mod >= *RS_pn37HC)
	{
		WriteRelayWord(RW_37HC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HC, RELAY_ON);
	}
	// 3.ILA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&ILA);
	CaluModValue(&ILA);
	if(ILA.Mod >= *RS_pn50LA)
	{
		WriteRelayWord(RW_50LA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LA, RELAY_OFF);
	}
	if(ILA.Mod >= *RS_pn37LA)
	{
		WriteRelayWord(RW_37LA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LA, RELAY_ON);
	}
	// 3.ILB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&ILB);
	CaluModValue(&ILB);
	if(ILB.Mod >= *RS_pn50LB)
	{
		WriteRelayWord(RW_50LB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LB, RELAY_OFF);
	}
	if(ILB.Mod >= *RS_pn37LB)
	{
		WriteRelayWord(RW_37LB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LB, RELAY_ON);
	}
	// 3.ILC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&ILC);
	CaluModValue(&ILC);
	if(ILC.Mod >= *RS_pn50LC)
	{
		WriteRelayWord(RW_50LC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LC, RELAY_OFF);
	}
	if(ILC.Mod >= *RS_pn37LC)
	{
		WriteRelayWord(RW_37LC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LC, RELAY_ON);
	}
	// 3.ILC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&I1);
	CaluModValue(&I1);
	if(I1.Mod >= *RS_pn50I1)
	{
		WriteRelayWord(RW_50I1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1, RELAY_OFF);
	}
	if(I1.Mod >= *RS_pn37I1)
	{
		WriteRelayWord(RW_37I1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1, RELAY_ON);
	}
	// 3.ILC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*14,&I2);
	CaluModValue(&I2);
	if(I2.Mod >= *RS_pn50I2)
	{
		WriteRelayWord(RW_50I2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I2, RELAY_OFF);
	}
	if(I2.Mod >= *RS_pn37I2)
	{
		WriteRelayWord(RW_37I2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I2, RELAY_ON);
	}
	CaluCustomAI();
}
void CRelayTask::CaluCustomAI(void) //2013��5��28�� 16:28:56�����Զ���ң�⺯���������ص�ѹ��yanzh
{
	WORD gear=0,gear1=0,gear2=0,gear4=0,gear8=0,gear10=0;
	if(ReadRelayWord(RW_M1))
		gear1 = 1;
	if(ReadRelayWord(RW_M2))
		gear2 = 2;	
	if(ReadRelayWord(RW_M3))
		gear4 = 4;	
	if(ReadRelayWord(RW_M4))
		gear8 = 8;	
	if(ReadRelayWord(RW_M5))
		gear10 = 10;

	gear = gear1+gear2+gear4+gear8+gear10;
	DB_WriteAI(M_CUSTOM1,gear);
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
	::InitAllRelayMem();
	InitRealy();
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
		DCChannelVal[i+RELAY_SAMP_INTERVAL*2] = DCCaluResult[2];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*3] = DCCaluResult[3];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*4] = DCCaluResult[4];
		DCChannelVal[i+RELAY_SAMP_INTERVAL*5] = DCCaluResult[5];
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

