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
*           		�����㷨����     FOR  6534����AT�������װ��                                                               
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
*      yanzh             11/12/12   ��ʼ����                         
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

extern DWORD 	dwRelayMSCnt;		//�����ú��������
extern DWORD 	g_hDbaseTaskID;
extern DWORD 	dwRelayDataPtr;
extern BYTE 	byRelayWordValue[];
extern BYTE 	g_RptSerialNum;			//�������к� 
extern BOOL		RelayCheckErr;			//����Ԫ���Լ�����־
extern BOOL 	RelayTaskInitOK;		//������ʼ������
extern BOOL 	RelayCheckInitOK;		//����Ԫ���Լ��ʼ����־
extern TSysConfigTable	*G_pCfgTable;
extern DWORD dwADResumeCnt;	//AD���ָܻ����������쳣���������豣֤��������һ�������ܲ�����
extern BYTE *g_MaintSW;
BOOL 			RelayTaskInitOK = FALSE;
DWORD 			dwRelayTaskID;
DWORD			dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD			dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
TStructCheck 	StructCheck[CHECK_NUM];					//�ṹ��У������

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
	
	RelCFGCLR = CFG_STATUS_CLR;			//�����˳�
	RelCFGSET = CFG_STATUS_SET;			//����Ͷ��	
	m_bLastRecStatusFlag = FALSE;		//����¼�������������жϱ�־
	//��ȡ���ݿⱣ������ֵ����ָ�룬�Լӿ�����ٶ�
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	
	InitAllRelayMem();
	InitRealy();
	InitDCRecInfo();

	
	dwChanDataMask = ((0x01<<CHANNEL_6534_U1)|(0x01<<CHANNEL_6534_U2)|(0x01<<CHANNEL_6534_U3)|(0x01<<CHANNEL_6534_U4) \
					|(0x01<<CHANNEL_6534_U5)|(0x01<<CHANNEL_6534_U6)|(0x01<<CHANNEL_6534_U7)|(0x01<<CHANNEL_6534_U8) \
					|(0x01<<CHANNEL_6534_I1)|(0x01<<CHANNEL_6534_I2)|(0x01<<CHANNEL_6534_I3)|(0x01<<CHANNEL_6534_I4) \
					|(0x01<<CHANNEL_6534_I5)|(0x01<<CHANNEL_6534_I6)|(0x01<<CHANNEL_6534_I7)|(0x01<<CHANNEL_6534_I8));
	dwRecDataMask = ((0x01<<CHANNEL_6534_IC1)|(0x01<<CHANNEL_6534_IC2));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	//��ʼ����ȡ����������ʱ������
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);	
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);	
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);	
	
	
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2));
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*2));

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

	g_MaintSW = SW_pbyMAINT;



//***************AT1��ر�����*******************************************************	
	//AT1ͻ����������ʷ���ݻ�����
	dwPickDataBuf_AT1 = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);
	ASSERT(dwPickDataBuf_AT1 != NULL);
	if(dwPickDataBuf_AT1 == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf_AT1",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf_AT1, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);

	RptSerialNumOfAct_AT1 = 0;	 //�����������к�
	RptSerialNumOfStart_AT1 = 0; //�����������к�		
	PickCheck_AT1 = 0;			 //ͻ��������������
	for(LONG m=0;m<4;m++)
	{
		m_lPickUpValue_AT1[m] = 0;	 //���ڵ�ͻ��������ֵ
		m_wPickCheckNext_AT1[m] = 0; //���α�������������ͻ�������ڶ�ֵ�ļ����� 
	}
	m_wPickCheck_AT1 = 0;		//�����ж�������ͻ�������ڶ�ֵ�ļ�����
	lSampData1_AT1 = 0; 		//ͻ����ֵ1
	lSampData2_AT1 = 0; 		//ͻ����ֵ2
	lSampDate3_AT1 = 0; 		//ͻ����ֵ3
	lPickValue_AT1 = 0; 		//����ͻ����ֵ
	FaultSINum_AT1 = 0; 				//���ϴ���������������		
	FaultLockTime_AT1 = 0;				//����������ʱ��		
	FaultOnFlag_AT1 = FALSE;					//�������ѭ�������־		
	HaveMakeStartRptFlag_AT1 = FALSE;		//�����������־ 
	FaultCheckOutFlag_AT1 = FALSE;			//����ѭ�����������ϱ�־ 
	m_bFaultLKFlag_AT1 = FALSE; 			//������ʼʱ��ʱ��������־������ͻ��������	
	m_b52BFFlag_AT1 = FALSE;
	m_bHaveRelayAct_AT1 = FALSE;			//���α��������ж��б�������������־
	m_bHaveMake52BFRpt_AT1 = FALSE;			//��·��ʧ�ܱ���������

//***************AT2��ر�����*******************************************************	
	//AT2ͻ����������ʷ���ݻ�����
	dwPickDataBuf_AT2 = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);
	ASSERT(dwPickDataBuf_AT2 != NULL);
	if(dwPickDataBuf_AT2 == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf_AT2",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf_AT2, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);

	RptSerialNumOfAct_AT2 = 0;	 //�����������к�
	RptSerialNumOfStart_AT2 = 0; //�����������к�		
	PickCheck_AT2 = 0;			 //ͻ��������������
	for(LONG m=0;m<4;m++)
	{
		m_lPickUpValue_AT2[m] = 0;	 //���ڵ�ͻ��������ֵ
		m_wPickCheckNext_AT2[m] = 0; //���α�������������ͻ�������ڶ�ֵ�ļ����� 
	}
	m_wPickCheck_AT2 = 0;		//�����ж�������ͻ�������ڶ�ֵ�ļ�����
	lSampData1_AT2 = 0; 		//ͻ����ֵ1
	lSampData2_AT2 = 0; 		//ͻ����ֵ2
	lSampDate3_AT2 = 0; 		//ͻ����ֵ3
	lPickValue_AT2 = 0; 		//����ͻ����ֵ
	FaultSINum_AT2 = 0; 				//���ϴ���������������		
	FaultLockTime_AT2 = 0;				//����������ʱ��		
	FaultOnFlag_AT2 = FALSE;					//�������ѭ�������־		
	HaveMakeStartRptFlag_AT2 = FALSE;		//�����������־ 
	FaultCheckOutFlag_AT2 = FALSE;			//����ѭ�����������ϱ�־ 
	m_bFaultLKFlag_AT2 = FALSE; 			//������ʼʱ��ʱ��������־������ͻ��������	
	m_b52BFFlag_AT2 = FALSE;
	
	m_bHaveMake52BFRpt_AT2 = FALSE;
	m_bHaveRelayAct_AT2 =FALSE;

	
	m_bLogicChanged = FALSE;
	RelayTaskInitOK = TRUE; 
	SetTimer(1,5);

}
/****************************************************************************************
��  ��: InitRealy
��  ��: ��ʼ��������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void  CRelayTask::InitRealy(void)
{
	 InitRelR50IC1();
	 InitRelR50IT1();
	 InitRelR50IF1();
	 InitRelR50IP1();
	 InitRelR50IC2();
	 InitRelR50IT2();
	 InitRelR50IF2();
	 InitRelR50IP2();
	 InitStructCheck();
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
//	MCF_GPIO_PODR_TIMER |= 0x02;		//�ߵ�ƽ
	static WORD wInvalidCheckCnt;		//�����Լ��ۼӼ�����	
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
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
	
	//���Ƚ��п����Լ�,ÿ�α��������һ·����
	if(!m_bLogicChanged)
		BoBreakCheck();
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	WriteComposeBuf(dwRecDataMask);
	CaluUI();          //���ݲ������ݣ��������It,If,U1,U2	
	FalutProcess_AT1();	//����ѭ������	
	FalutProcess_AT2();	//����ѭ������
	Process52BF_AT1();
	Process52BF_AT2();
	
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
//	MCF_GPIO_PODR_TIMER &= ~0x02;		//�͵�ƽ
}

/****************************************************************************************
��  ��: WriteComposeBuf
��  ��: �ϳ�ͨ�����ݻ�д
��  ��: dwMask -- �ϳ�ͨ����������
����ֵ: ��
*****************************************************************************************/
void CRelayTask::WriteComposeBuf(DWORD dwMask)
{
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��
	{
		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*9-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*10-RELAY_SAMP_INTERVAL+i];
	}
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		dwComposeBuf[i]= dwSampBuf[READ_POINT_NUM*8+i] - dwSampBuf[READ_POINT_NUM*9+i];
	}
	
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��
	{
		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*12-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*13-RELAY_SAMP_INTERVAL+i];
	}
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		dwComposeBuf[READ_POINT_NUM+i]= dwSampBuf[READ_POINT_NUM*11+i] - dwSampBuf[READ_POINT_NUM*12+i];
	}

	WriteComposeData(dwRecBuf, dwMask);
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
	CaluBaseFourier_S15(dwSampBuf,&U1);
	CaluModValue(&U1);
	*RM_pnUp1= U1.Mod;                                       
	DB_LogWriteRM(RM_UP1, *RM_pnUp1);	
	if(U1.Mod>=*RS_pn59U1)	
	{	
		WriteRelayWord(RW_59U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(U1.Mod>=*RS_pn27U1)
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}
	// 2.U2
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	*RM_pnUp2= U2.Mod;                                       
	DB_LogWriteRM(RM_UP2, *RM_pnUp2);	
	if(U2.Mod>=*RS_pn59U2)	
	{	
		WriteRelayWord(RW_59U2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(U2.Mod>=*RS_pn27U2)
	{
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U2, RELAY_ON);
	}
	// 3.U3
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&U3);
	CaluModValue(&U3);
	*RM_pnUp3= U3.Mod;                                       
	DB_LogWriteRM(RM_UP3, *RM_pnUp3);	
	if(U3.Mod>=*RS_pn59U3)	
	{	
		WriteRelayWord(RW_59U3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3, RELAY_OFF);
	}
	if(U3.Mod>=*RS_pn27U3)
	{
		WriteRelayWord(RW_27U3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3, RELAY_ON);
	}
	// 4.U4
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U4);
	CaluModValue(&U4);
	*RM_pnUp4= U4.Mod;                                       
	DB_LogWriteRM(RM_UP4, *RM_pnUp4);	
	if(U4.Mod>=*RS_pn59U4)	
	{	
		WriteRelayWord(RW_59U4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U4, RELAY_OFF);
	}
	if(U4.Mod>=*RS_pn27U4)
	{
		WriteRelayWord(RW_27U4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U4, RELAY_ON);
	}
	// 5.U5
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&U5);
	CaluModValue(&U5);
	*RM_pnUp5= U5.Mod;                                       
	DB_LogWriteRM(RM_UP5, *RM_pnUp5);	
	if(U5.Mod>=*RS_pn59U5)	
	{	
		WriteRelayWord(RW_59U5, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U5, RELAY_OFF);
	}
	if(U5.Mod>=*RS_pn27U5)
	{
		WriteRelayWord(RW_27U5, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U5, RELAY_ON);
	}
	// 6.U6
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&U6);
	CaluModValue(&U6);
	*RM_pnUp6= U6.Mod;                                       
	DB_LogWriteRM(RM_UP6, *RM_pnUp6);	
	if(U6.Mod>=*RS_pn59U6)	
	{	
		WriteRelayWord(RW_59U6, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U6, RELAY_OFF);
	}
	if(U6.Mod>=*RS_pn27U6)
	{
		WriteRelayWord(RW_27U6, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U6, RELAY_ON);
	}
	// 7.U7
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&U7);
	CaluModValue(&U7);
	*RM_pnUp7= U7.Mod;                                       
	DB_LogWriteRM(RM_UP7, *RM_pnUp7);	
	if(U7.Mod>=*RS_pn59U7)	
	{	
		WriteRelayWord(RW_59U7, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U7, RELAY_OFF);
	}
	if(U7.Mod>=*RS_pn27U7)
	{
		WriteRelayWord(RW_27U7, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U7, RELAY_ON);
	}
	// 8.U8
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&U8);
	CaluModValue(&U8);
	*RM_pnUp8= U8.Mod;                                       
	DB_LogWriteRM(RM_UP8, *RM_pnUp8);	
	if(U8.Mod>=*RS_pn59U8)	
	{	
		WriteRelayWord(RW_59U8, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U8, RELAY_OFF);
	}
	if(U8.Mod>=*RS_pn27U8)
	{
		WriteRelayWord(RW_27U8, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U8, RELAY_ON);
	}

	// 9. I1
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&I1);
	CaluModValue(&I1);
	*RM_pnIp1= I1.Mod;                                       
	DB_LogWriteRM(RM_IP1, *RM_pnIp1);	
	if(I1.Mod>=*RS_pn50I1)	
	{	
		WriteRelayWord(RW_50I1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1, RELAY_OFF);
	}
	if(I1.Mod>=*RS_pn37I1)
	{
		WriteRelayWord(RW_37I1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1, RELAY_ON);
	}
	// 10. I2
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&I2);
	CaluModValue(&I2);
	*RM_pnIp2= I2.Mod;                                       
	DB_LogWriteRM(RM_IP2, *RM_pnIp2);	
	if(I2.Mod>=*RS_pn50I2)	
	{	
		WriteRelayWord(RW_50I2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I2, RELAY_OFF);
	}
	if(I2.Mod>=*RS_pn37I2)
	{
		WriteRelayWord(RW_37I2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I2, RELAY_ON);
	}
	// 11. I3
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&I3);
	CaluModValue(&I3);
	*RM_pnIp3= I3.Mod;                                       
	DB_LogWriteRM(RM_IP3, *RM_pnIp3);	
	if(I3.Mod>=*RS_pn50I3)	
	{	
		WriteRelayWord(RW_50I3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I3, RELAY_OFF);
	}
	if(I3.Mod>=*RS_pn37I3)
	{
		WriteRelayWord(RW_37I3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I3, RELAY_ON);
	}
	// 12. I4
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&I4);
	CaluModValue(&I4);
	*RM_pnIp4= I4.Mod;                                       
	DB_LogWriteRM(RM_IP4, *RM_pnIp4);	
	if(I4.Mod>=*RS_pn50I4)	
	{	
		WriteRelayWord(RW_50I4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I4, RELAY_OFF);
	}
	if(I4.Mod>=*RS_pn37I4)
	{
		WriteRelayWord(RW_37I4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I4, RELAY_ON);
	}
	// 13. I5
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&I5);
	CaluModValue(&I5);
	*RM_pnIp5= I5.Mod;                                       
	DB_LogWriteRM(RM_IP5, *RM_pnIp5);	
	if(I5.Mod>=*RS_pn50I5)	
	{	
		WriteRelayWord(RW_50I5, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I5, RELAY_OFF);
	}
	if(I5.Mod>=*RS_pn37I5)
	{
		WriteRelayWord(RW_37I5, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I5, RELAY_ON);
	}
	// 14.I6
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&I6);
	CaluModValue(&I6);
	*RM_pnIp6= I6.Mod;                                       
	DB_LogWriteRM(RM_IP6, *RM_pnIp6);	
	if(I6.Mod>=*RS_pn50I6)	
	{	
		WriteRelayWord(RW_50I6, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I6, RELAY_OFF);
	}
	if(I6.Mod>=*RS_pn37I6)
	{
		WriteRelayWord(RW_37I6, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I6, RELAY_ON);
	}
	// 15.I7
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*14,&I7);
	CaluModValue(&I7);
	*RM_pnIp7= I7.Mod;                                       
	DB_LogWriteRM(RM_IP7, *RM_pnIp7);	
	if(I7.Mod>=*RS_pn50I7)	
	{	
		WriteRelayWord(RW_50I7, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I7, RELAY_OFF);
	}
	if(I7.Mod>=*RS_pn37I7)
	{
		WriteRelayWord(RW_37I7, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I7, RELAY_ON);
	}
	// 16.I8
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*15,&I8);
	CaluModValue(&I8);
	*RM_pnIp8= I8.Mod;                                       
	DB_LogWriteRM(RM_IP8, *RM_pnIp8);	
	if(I8.Mod>=*RS_pn50I8)	
	{	
		WriteRelayWord(RW_50I8, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I8, RELAY_OFF);
	}
	if(I8.Mod>=*RS_pn37I8)
	{
		WriteRelayWord(RW_37I8, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I8, RELAY_ON);
	}
	// 15. Ic1
	CaluBaseFourier_S15(dwComposeBuf,&IC1);
	CaluModValue(&IC1);
	*RM_pnIC1= IC1.Mod;                                       
	DB_LogWriteRM(RM_IC1, *RM_pnIC1);	
	// 16. Ic2
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&IC2);
	CaluModValue(&IC2);
	*RM_pnIC2= IC2.Mod;                                       
	DB_LogWriteRM(RM_IC2, *RM_pnIC2);

}
/****************************************************************************************
��  ��: InitRelR50IC1
��  ��: ��ʼ������1����Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitRelR50IC1()
{
	
	RelR50IC1.pElecSet			= RS_pn60I_AT1C;          			//��������������ֵ
	RelR50IC1.pTimeSet			= RS_pn60I_AT1T;				 	//��������ʱ�䶨ֵ
	RelR50IC1.pHarmKhr1Set		= NULL;							//��������г����Ȩϵ��
	RelR50IC1.pVolLockSet		= NULL;				 	//����������ѹ������ֵ
	RelR50IC1.pAccActTimeSet	= NULL;							//������������ٶ���ʱ�䶨ֵ
	RelR50IC1.pH2Coef			= NULL;							//����г������ϵ����г����Ȩϵ���ķŴ���
	
	RelR50IC1.pRelCFG			= CFG_pby60I_AT1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IC1.pRelSW			= SW_pby60I_AT1;         			//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IC1.pRelHLockCFG		= &RelCFGCLR;			    	//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC1.pRelHSuprCFG		= &RelCFGCLR; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC1.pRelPDCFG			= &RelCFGCLR;		    		//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC1.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC1.pRelFDCFG			= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IC1.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IC1.pRelFLCFG			= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IC1.pSysTimer			= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	
	
	RelR50IC1.pRelElec 			= &IC1;							//��ǰ����ָ��		
	RelR50IC1.pRelHarm3			= NULL;							//��ǰ����г��ָ��	
	RelR50IC1.pRelHarm5			= NULL;							//��ǰ���г��ָ��	
	RelR50IC1.pRelVolt			= NULL;							//��ǰ���������ĵ�ѹ��ָ��
	RelR50IC1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IC1.R47ULKRelayWord   = FALSE1;

	RelR50IC1.StartRelayWord	= RW_PI60I_AT1;					//����������μ̵�����
	RelR50IC1.ActRelayWord		= RW_TR60I_AT1;					//�������������̵�����
	RelR50IC1.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IC1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IC1.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IC1.PRewardRelayWord  = FALSE1;						//���ʷ�����̵�����
	RelR50IC1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IC1.LockRelayWord		= RW_LKPI60I_AT1;					//��α����̵�����
	RelR50IC1.RSTRelayWord		= RW_LKRST60I_AT1;				//���ر����̵�����
	RelR50IC1.LoopCtrlRelayWord = RW_LPC60I_AT1;					//����ѭ�����Ƽ̵�����
	
	RelR50IC1.wReportNum		= R_60I_1AT;						//���������������
	RelR50IC1.wAccReportNum		= 0xFFFF;						//���ٶ����������

	RelR50IC1.bMakeActRpt		= TRUE;
	RelR50IC1.dwK35				= NULL;
	RelR50IC1.RptSerialNum		= &RptSerialNumOfAct_AT1;						
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IC1.wPickUpReportNum	= R_60I_1AT_PI;						//��α������к�
	RelR50IC1.wResetReportNum	= R_60I_1AT_RES;						//���ر������к�
	//**********************************************************************	


//	RelR50IC1.dwRelTimer		= 0;			 			//��ʱ��
	RelR50IC1.byRelFCounter		= 0;						//���ϼ�����
	RelR50IC1.byRelRCounter		= 0;						//���ؼ�����	
	RelR50IC1.wExcepWordNum     = 8;

}
 /****************************************************************************************
 ��  ��: InitRelR50IT1
 ��  ��: ��ʼ��1ATT�߹�������Ԫ��
 ��  ��: ��
 ����ֵ: ��
 *****************************************************************************************/
 void CRelayTask::InitRelR50IT1()
{
	RelR50IT1.pElecSet			= RS_pn50T_AT1C;					//��������������ֵ
	RelR50IT1.pTimeSet			= RS_pn50T_AT1T;					//��������ʱ�䶨ֵ
	RelR50IT1.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IT1.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IT1.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IT1.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IT1.pRelCFG			= CFG_pby50T_AT1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IT1.pRelSW			= SW_pby50T_AT1;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IT1.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT1.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT1.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT1.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT1.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IT1.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IT1.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IT1.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IT1.pRelElec			= &I1; 						//��ǰ����ָ��		
	RelR50IT1.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IT1.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IT1.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IT1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IT1.R47ULKRelayWord   = FALSE1;

	RelR50IT1.StartRelayWord	= RW_PI50T_AT1; 				//����������μ̵�����
	RelR50IT1.ActRelayWord		= RW_TR50T_AT1; 				//�������������̵�����
	RelR50IT1.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IT1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IT1.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IT1.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IT1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IT1.LockRelayWord 	= RW_LKPI50T_AT1;					//��α����̵�����
	RelR50IT1.RSTRelayWord		= RW_LKRST50T_AT1;				//���ر����̵�����
	RelR50IT1.LoopCtrlRelayWord = RW_LPC50T_AT1;					//����ѭ�����Ƽ̵�����

	RelR50IT1.wReportNum		= R_50T_1AT;						//���������������
	RelR50IT1.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IT1.bMakeActRpt		= TRUE;
	RelR50IT1.dwK35 			= NULL;
	RelR50IT1.RptSerialNum		= &RptSerialNumOfAct_AT1;						
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IT1.wPickUpReportNum	= R_50T_1AT_PI;						//��α������к�
	RelR50IT1.wResetReportNum	= R_50T_1AT_RES;						//���ر������к�
	//**********************************************************************	

//	RelR50IT1.dwRelTimer		= 0;						//��ʱ��
	RelR50IT1.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IT1.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IT1.wExcepWordNum 	= 8;

} 
/****************************************************************************************
��  ��: InitRelR50IF1
��  ��: ��ʼ��1ATF�߹�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitRelR50IF1()
{

	RelR50IF1.pElecSet			= RS_pn50F_AT1C;					//��������������ֵ
	RelR50IF1.pTimeSet			= RS_pn50F_AT1T;					//��������ʱ�䶨ֵ
	RelR50IF1.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IF1.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IF1.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IF1.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IF1.pRelCFG			= CFG_pby50F_AT1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IF1.pRelSW			= SW_pby50F_AT1;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IF1.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF1.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF1.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF1.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF1.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IF1.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IF1.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IF1.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IF1.pRelElec			= &I2;						//��ǰ����ָ��		
	RelR50IF1.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IF1.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IF1.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IF1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IF1.R47ULKRelayWord   = FALSE1;

	RelR50IF1.StartRelayWord	= RW_PI50F_AT1; 				//����������μ̵�����
	RelR50IF1.ActRelayWord		= RW_TR50F_AT1; 				//�������������̵�����
	RelR50IF1.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IF1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IF1.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IF1.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IF1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IF1.LockRelayWord 	= RW_LKPI50F_AT1;					//��α����̵�����
	RelR50IF1.RSTRelayWord		= RW_LKRST50F_AT1;				//���ر����̵�����
	RelR50IF1.LoopCtrlRelayWord = RW_LPC50F_AT1;					//����ѭ�����Ƽ̵�����

	RelR50IF1.wReportNum		= R_50F_1AT;						//���������������
	RelR50IF1.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IF1.bMakeActRpt		= TRUE;
	RelR50IF1.dwK35 			= NULL;
	RelR50IF1.RptSerialNum		= &RptSerialNumOfAct_AT1;		
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IF1.wPickUpReportNum	= R_50F_1AT_PI;						//��α������к�
	RelR50IF1.wResetReportNum	= R_50F_1AT_RES;						//���ر������к�
	//**********************************************************************	
//	RelR50IF1.dwRelTimer		= 0;						//��ʱ��
	RelR50IF1.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IF1.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IF1.wExcepWordNum 	= 8;

} 
/****************************************************************************************
��  ��: InitRelR50IP1
��  ��: ��ʼ��1AT���ǹ�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitRelR50IP1()
{
	RelR50IP1.pElecSet			= RS_pn50CS_AT1C;					//��������������ֵ
	RelR50IP1.pTimeSet			= RS_pn50CS_AT1T;					//��������ʱ�䶨ֵ
	RelR50IP1.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IP1.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IP1.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IP1.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IP1.pRelCFG			= CFG_pby50CS_AT1;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IP1.pRelSW			= SW_pby50CS_AT1;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IP1.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP1.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP1.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP1.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP1.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IP1.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IP1.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IP1.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IP1.pRelElec			= &I3;						//��ǰ����ָ��		
	RelR50IP1.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IP1.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IP1.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IP1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IP1.R47ULKRelayWord   = FALSE1;

	RelR50IP1.StartRelayWord	= RW_PI50CS_AT1; 				//����������μ̵�����
	RelR50IP1.ActRelayWord		= RW_TR50CS_AT1; 				//�������������̵�����
	RelR50IP1.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IP1.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IP1.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IP1.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IP1.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IP1.LockRelayWord 	= RW_LKPI50CS_AT1;					//��α����̵�����
	RelR50IP1.RSTRelayWord		= RW_LKRST50CS_AT1;				//���ر����̵�����
	RelR50IP1.LoopCtrlRelayWord = RW_LPC50CS_AT1;					//����ѭ�����Ƽ̵�����

	RelR50IP1.wReportNum		= R_50CS_1AT;						//���������������
	RelR50IP1.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IP1.bMakeActRpt		= TRUE;
	RelR50IP1.dwK35 			= NULL;
	RelR50IP1.RptSerialNum		= &RptSerialNumOfAct_AT1;		
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IP1.wPickUpReportNum	= R_50CS_1AT_PI;						//��α������к�
	RelR50IP1.wResetReportNum	= R_50CS_1AT_RES;						//���ر������к�
	//**********************************************************************	
//	RelR50IP1.dwRelTimer		= 0;						//��ʱ��
	RelR50IP1.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IP1.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IP1.wExcepWordNum 	= 8;

}
/****************************************************************************************
��  ��: InitRelR50IC2
��  ��: ��ʼ��2AT������������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitRelR50IC2()
{
	RelR50IC2.pElecSet			= RS_pn60I_AT2C;					//��������������ֵ
	RelR50IC2.pTimeSet			= RS_pn60I_AT2T;					//��������ʱ�䶨ֵ
	RelR50IC2.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IC2.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IC2.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IC2.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IC2.pRelCFG			= CFG_pby60I_AT2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IC2.pRelSW			= SW_pby60I_AT2;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IC2.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC2.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC2.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC2.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IC2.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IC2.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IC2.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IC2.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IC2.pRelElec			= &IC2;						//��ǰ����ָ��		
	RelR50IC2.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IC2.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IC2.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IC2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IC2.R47ULKRelayWord   = FALSE1;

	RelR50IC2.StartRelayWord	= RW_PI60I_AT2; 				//����������μ̵�����
	RelR50IC2.ActRelayWord		= RW_TR60I_AT2; 				//�������������̵�����
	RelR50IC2.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IC2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IC2.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IC2.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IC2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IC2.LockRelayWord 	= RW_LKPI60I_AT2;					//��α����̵�����
	RelR50IC2.RSTRelayWord		= RW_LKRST60I_AT2;				//���ر����̵�����
	RelR50IC2.LoopCtrlRelayWord = RW_LPC60I_AT2;					//����ѭ�����Ƽ̵�����

	RelR50IC2.wReportNum		= R_60I_2AT;						//���������������
	RelR50IC2.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IC2.bMakeActRpt		= TRUE;
	RelR50IC2.dwK35 			= NULL;
	RelR50IC2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IC2.wPickUpReportNum	= R_60I_2AT_PI;						//��α������к�
	RelR50IC2.wResetReportNum	= R_60I_2AT_RES;						//���ر������к�
	//**********************************************************************	
//	RelR50IC2.dwRelTimer		= 0;						//��ʱ��
	RelR50IC2.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IC2.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IC2.wExcepWordNum 	= 8;

}
/****************************************************************************************
��  ��: InitRelR50IT2
��  ��: ��ʼ��2ATT�߹�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitRelR50IT2()
{

	RelR50IT2.pElecSet			= RS_pn50T_AT2C;					//��������������ֵ
	RelR50IT2.pTimeSet			= RS_pn50T_AT2T;					//��������ʱ�䶨ֵ
	RelR50IT2.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IT2.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IT2.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IT2.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IT2.pRelCFG			= CFG_pby50T_AT2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IT2.pRelSW			= SW_pby50T_AT2;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IT2.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT2.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT2.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT2.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IT2.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IT2.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IT2.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IT2.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IT2.pRelElec			= &I4; 					//��ǰ����ָ��		
	RelR50IT2.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IT2.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IT2.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IT2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IT2.R47ULKRelayWord   = FALSE1;

	RelR50IT2.StartRelayWord	= RW_PI50T_AT2; 				//����������μ̵�����
	RelR50IT2.ActRelayWord		= RW_TR50T_AT2; 				//�������������̵�����
	RelR50IT2.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IT2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IT2.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IT2.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IT2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IT2.LockRelayWord 	= RW_LKPI50T_AT2;					//��α����̵�����
	RelR50IT2.RSTRelayWord		= RW_LKRST50T_AT2;				//���ر����̵�����
	RelR50IT2.LoopCtrlRelayWord = RW_LPC50T_AT2;					//����ѭ�����Ƽ̵�����

	RelR50IT2.wReportNum		= R_50T_2AT;						//���������������
	RelR50IT2.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IT2.bMakeActRpt		= TRUE;
	RelR50IT2.dwK35 			= NULL;
	RelR50IT2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IT2.wPickUpReportNum	= R_50T_2AT_PI;						//��α������к�
	RelR50IT2.wResetReportNum	= R_50T_2AT_RES;						//���ر������к�
	//**********************************************************************	
//	RelR50IT2.dwRelTimer		= 0;						//��ʱ��
	RelR50IT2.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IT2.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IT2.wExcepWordNum 	= 8;

}
/****************************************************************************************
��  ��: InitRelR50IF2
��  ��: ��ʼ��2ATF�߹�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/

void CRelayTask::InitRelR50IF2()
{

	RelR50IF2.pElecSet			= RS_pn50F_AT2C;					//��������������ֵ
	RelR50IF2.pTimeSet			= RS_pn50F_AT2T;					//��������ʱ�䶨ֵ
	RelR50IF2.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IF2.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IF2.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IF2.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IF2.pRelCFG			= CFG_pby50F_AT2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IF2.pRelSW			= SW_pby50F_AT2;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IF2.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF2.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF2.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF2.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IF2.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IF2.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IF2.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IF2.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IF2.pRelElec			= &I5;					//��ǰ����ָ��		
	RelR50IF2.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IF2.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IF2.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IF2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IF2.R47ULKRelayWord   = FALSE1;

	RelR50IF2.StartRelayWord	= RW_PI50F_AT2; 				//����������μ̵�����
	RelR50IF2.ActRelayWord		= RW_TR50F_AT2; 				//�������������̵�����
	RelR50IF2.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IF2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IF2.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IF2.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IF2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IF2.LockRelayWord 	= RW_LKPI50F_AT2;					//��α����̵�����
	RelR50IF2.RSTRelayWord		= RW_LKRST50F_AT2;				//���ر����̵�����
	RelR50IF2.LoopCtrlRelayWord = RW_LPC50F_AT2;					//����ѭ�����Ƽ̵�����

	RelR50IF2.wReportNum		= R_50F_2AT;						//���������������
	RelR50IF2.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IF2.bMakeActRpt		= TRUE;
	RelR50IF2.dwK35 			= NULL;
	RelR50IF2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IF2.wPickUpReportNum	= R_50F_2AT_PI;						//��α������к�
	RelR50IF2.wResetReportNum	= R_50F_2AT_RES;						//���ر������к�
	//**********************************************************************	
//	RelR50IF2.dwRelTimer		= 0;						//��ʱ��
	RelR50IF2.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IF2.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IF2.wExcepWordNum 	= 8;

}
/****************************************************************************************
��  ��: InitRelR50IP2
��  ��: ��ʼ��2AT���ǹ�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CRelayTask::InitRelR50IP2()
{
	
	RelR50IP2.pElecSet			= RS_pn50CS_AT2C;					//��������������ֵ
	RelR50IP2.pTimeSet			= RS_pn50CS_AT2T;					//��������ʱ�䶨ֵ
	RelR50IP2.pHarmKhr1Set		= NULL; 						//��������г����Ȩϵ��
	RelR50IP2.pVolLockSet		= NULL; 				//����������ѹ������ֵ
	RelR50IP2.pAccActTimeSet	= NULL; 						//������������ٶ���ʱ�䶨ֵ
	RelR50IP2.pH2Coef			= NULL; 						//����г������ϵ����г����Ȩϵ���ķŴ���

	RelR50IP2.pRelCFG			= CFG_pby50CS_AT2;				//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	RelR50IP2.pRelSW			= SW_pby50CS_AT2;					//����������ѹ��------1:Ͷ�룬2:�˳�
	RelR50IP2.pRelHLockCFG		= &RelCFGCLR;					//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP2.pRelHSuprCFG		= &RelCFGCLR;					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP2.pRelPDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP2.pRelVolLockCFG	= &RelCFGCLR;				//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	RelR50IP2.pRelFDCFG 		= &RelCFGCLR;					//���ʷ����ܿ���λ--1:������2:������
	RelR50IP2.pRelAccCFG		= &RelCFGCLR;					//���޺���ٹ���------1:�У�  2:��
	RelR50IP2.pRelFLCFG 		= &RelCFGCLR;					//���ϲ��Ͷ��
	RelR50IP2.pSysTimer 		= &dwRelayMSCnt;				//ϵͳ��ʱ��ָ��	

	RelR50IP2.pRelElec			= &I6;					//��ǰ����ָ��		
	RelR50IP2.pRelHarm3 		= NULL; 						//��ǰ����г��ָ��	
	RelR50IP2.pRelHarm5 		= NULL; 						//��ǰ���г��ָ��	
	RelR50IP2.pRelVolt			= NULL; 						//��ǰ���������ĵ�ѹ��ָ��
	RelR50IP2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IP2.R47ULKRelayWord   = FALSE1;

	RelR50IP2.StartRelayWord	= RW_PI50CS_AT2; 				//����������μ̵�����
	RelR50IP2.ActRelayWord		= RW_TR50CS_AT2; 				//�������������̵�����
	RelR50IP2.AccRelayWord		= FALSE1;						//���������̵�����
	RelR50IP2.AccActRelayWord	= FALSE1;						//����ٶ����̵�����
	RelR50IP2.PForwardRelayWord = FALSE1;						//����������̵�����
	RelR50IP2.PRewardRelayWord	= FALSE1;						//���ʷ�����̵�����
	RelR50IP2.H2LockRelayWord	= FALSE1;						//����г�������̵�����
	RelR50IP2.LockRelayWord 	= RW_LKPI50CS_AT2;					//��α����̵�����
	RelR50IP2.RSTRelayWord		= RW_LKRST50CS_AT2;				//���ر����̵�����
	RelR50IP2.LoopCtrlRelayWord = RW_LPC50CS_AT2;					//����ѭ�����Ƽ̵�����

	RelR50IP2.wReportNum		= R_50CS_2AT;						//���������������
	RelR50IP2.wAccReportNum 	= 0xFFFF;						//���ٶ����������

	RelR50IP2.bMakeActRpt		= TRUE;
	RelR50IP2.dwK35 			= NULL;
	RelR50IP2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************��η��ر��沿��*************	yanxs  2013-03-22
	RelR50IP2.wPickUpReportNum	= R_50CS_2AT_PI;						//��α������к�
	RelR50IP2.wResetReportNum	= R_50CS_2AT_RES;						//���ر������к�
	//**********************************************************************	
//	RelR50IP2.dwRelTimer		= 0;						//��ʱ��
	RelR50IP2.byRelFCounter 	= 0;						//���ϼ�����
	RelR50IP2.byRelRCounter 	= 0;						//���ؼ�����	
	RelR50IP2.wExcepWordNum 	= 8;
}
void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&RelR50IC1;
	StructCheck[0].bigsize = sizeof(RelR50IC1)/2 - RelR50IC1.wExcepWordNum;		
	StructCheck[1].pstruct = (WORD*)&RelR50IT1;
	StructCheck[1].bigsize = sizeof(RelR50IT1)/2 - RelR50IT1.wExcepWordNum;	
	StructCheck[2].pstruct = (WORD*)&RelR50IF1;
	StructCheck[2].bigsize = sizeof(RelR50IF1)/2 - RelR50IF1.wExcepWordNum; 
	StructCheck[3].pstruct = (WORD*)&RelR50IP1;
	StructCheck[3].bigsize = sizeof(RelR50IP1)/2 - RelR50IP1.wExcepWordNum; 
	StructCheck[4].pstruct = (WORD*)&RelR50IC2;
	StructCheck[4].bigsize = sizeof(RelR50IC2)/2 - RelR50IC2.wExcepWordNum; 
	StructCheck[5].pstruct = (WORD*)&RelR50IT2;
	StructCheck[5].bigsize = sizeof(RelR50IT2)/2 - RelR50IT2.wExcepWordNum; 
	StructCheck[6].pstruct = (WORD*)&RelR50IF2;
	StructCheck[6].bigsize = sizeof(RelR50IF2)/2 - RelR50IF2.wExcepWordNum; 
	StructCheck[7].pstruct = (WORD*)&RelR50IP2;
	StructCheck[7].bigsize = sizeof(RelR50IP2)/2 - RelR50IP2.wExcepWordNum; 
	StructCheck[8].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[8].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
//------------------------------------------------------------------------------------------ 
// ����: ģֵ�����б����							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK_AT1(void)
{
	BYTE  pChannel[]={CHANNEL_6534_I1,CHANNEL_6534_I2,CHANNEL_6534_I3,CHANNEL_6534_IC1};
		
	if(ReadRelayWord(RW_52BF_AT1))  //��·����բʧ�ܣ������б��������жϴ��� 
	{
		WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
		return FALSE;
	}
	if((*RelR50IC1.pRelCFG == CFG_STATUS_SET) && (*RelR50IC1.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
			if(RelR50IC1.pRelElec->Mod > *RelR50IC1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
	
	if((*RelR50IT1.pRelCFG == CFG_STATUS_SET) && (*RelR50IT1.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IT1.pRelElec->Mod > *RelR50IT1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IF1.pRelCFG == CFG_STATUS_SET) && (*RelR50IF1.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IF1.pRelElec->Mod > *RelR50IF1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IP1.pRelCFG == CFG_STATUS_SET) && (*RelR50IP1.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IP1.pRelElec->Mod > *RelR50IP1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
//ͻ���������жϲ���
	//������һ������Ͷ��
	if(((*RelR50IC1.pRelCFG == CFG_STATUS_SET) && (*RelR50IC1.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IT1.pRelCFG == CFG_STATUS_SET) && (*RelR50IT1.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IF1.pRelCFG == CFG_STATUS_SET) && (*RelR50IF1.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IP1.pRelCFG == CFG_STATUS_SET) && (*RelR50IP1.pRelSW == SW_STATUS_SET)))
	
	{
		
		BYTE  pChannel[]={CHANNEL_6534_I1,CHANNEL_6534_I2,CHANNEL_6534_I3,CHANNEL_6534_IC1};
		ReadPreSingleData(dwPickDataBuf_AT1,pChannel,4,RELAY_SAMP_INTERVAL);		
		//�㷨1�����ݺ�RELAY_SAMP_INTERVALȷ����ͨ���㷨
		//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
		//ע��:RELAY_SAMP_INTERVAL��ֵ����С��3
		for(LONG j=0;j<4;j++)
		{
			PickCheck_AT1 = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1_AT1 = dwPickDataBuf_AT1[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2_AT1 = dwPickDataBuf_AT1[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3_AT1 = dwPickDataBuf_AT1[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue_AT1 = MULTIPLES*ToAbs(((lSampData1_AT1-lSampData2_AT1)-(lSampData2_AT1-lSampDate3_AT1)));
				
				//���ø����ż�ֵ�жϣ���I>0.25*��I'+ C,���Ц�I'Ϊ�ϸ�һ���ڽ��ĵ��ͻ����ֵ��CΪͻ����������ֵ
				if(lPickValue_AT1*4>*RS_pnPIC*4+m_lPickUpValue_AT1[j])
				{
					m_wPickCheck_AT1++;
					if(m_wPickCheck_AT1 ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime_AT1);
						PickTime = PickUpTime_AT1.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime_AT1.Lo-(i*625+500)/1000;
						PickUpTime_AT1.Lo= (DWORD)PickTime;
						PickUpTime_AT1.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //�����ݿ���д����������
						RptSerialNumOfAct_AT1 = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //�����ݿ���д����������
						RptSerialNumOfStart_AT1 = g_RptSerialNum;
						m_wPickCheck_AT1 =0;
						for(LONG m=0;m<4;m++)
						{
							m_wPickCheckNext_AT1[m] =0;
							m_lPickUpValue_AT1[m]=0; 
						}
						
						return TRUE;	
					}
					if(i<2)
					{
						m_wPickCheckNext_AT1[j]++;
						if(m_wPickCheckNext_AT1[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime_AT1);
							PickTime = PickUpTime_AT1.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime_AT1.Lo-(i*625+500)/1000;
							PickUpTime_AT1.Lo= (DWORD)PickTime;
							PickUpTime_AT1.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
							//ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //�����ݿ���д����������
							RptSerialNumOfAct_AT1 = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //�����ݿ���д����������
							RptSerialNumOfStart_AT1 = g_RptSerialNum;
							m_wPickCheck_AT1 =0;
							for(LONG m=0;m<4;m++)
							{
								m_wPickCheckNext_AT1[m] =0;
								m_lPickUpValue_AT1[m]=0;
							}
							
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						m_wPickCheckNext_AT1[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						m_wPickCheckNext_AT1[j]++;
				}
				else
				{
					m_wPickCheck_AT1 =0;
					m_wPickCheckNext_AT1[j]=0;
				}
				m_lPickUpValue_AT1[j] = lPickValue_AT1;
			}
		}
	}
	
	WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
	return FALSE;
}
//------------------------------------------------------------------------------------------ 
// ����: ģֵ�����б����							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK_AT2(void)
{
	BYTE  pChannel[]={CHANNEL_6534_I4,CHANNEL_6534_I5,CHANNEL_6534_I6,CHANNEL_6534_IC2};
	
	if(ReadRelayWord(RW_52BF_AT2))  //��·����բʧ�ܣ������б��������жϴ��� 
	{
		WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
		return FALSE;
	}
	if((*RelR50IC2.pRelCFG == CFG_STATUS_SET) && (*RelR50IC2.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IC2.pRelElec->Mod > *RelR50IC2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
	
	if((*RelR50IT2.pRelCFG == CFG_STATUS_SET) && (*RelR50IT2.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IT2.pRelElec->Mod > *RelR50IT2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IF2.pRelCFG == CFG_STATUS_SET) && (*RelR50IF2.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IF2.pRelElec->Mod > *RelR50IF2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IP2.pRelCFG == CFG_STATUS_SET) && (*RelR50IP2.pRelSW == SW_STATUS_SET))   //�ٶϱ���Ͷ�� 
	{
		if(RelR50IP2.pRelElec->Mod > *RelR50IP2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //�����ݿ���д����������
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //�����ݿ���д����������
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
//ͻ���������жϲ���
	//������һ������Ͷ��
	if(((*RelR50IC2.pRelCFG == CFG_STATUS_SET) && (*RelR50IC2.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IT2.pRelCFG == CFG_STATUS_SET) && (*RelR50IT2.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IF2.pRelCFG == CFG_STATUS_SET) && (*RelR50IF2.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IP2.pRelCFG == CFG_STATUS_SET) && (*RelR50IP2.pRelSW == SW_STATUS_SET)))
	
	{
		
		BYTE  pChannel[]={CHANNEL_6534_I4,CHANNEL_6534_I5,CHANNEL_6534_I6,CHANNEL_6534_IC2};
		ReadPreSingleData(dwPickDataBuf_AT2,pChannel,4,RELAY_SAMP_INTERVAL);		
		//�㷨1�����ݺ�RELAY_SAMP_INTERVALȷ����ͨ���㷨
		//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
		//ע��:RELAY_SAMP_INTERVAL��ֵ����С��3
		for(LONG j=0;j<4;j++)
		{
			PickCheck_AT2 = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1_AT2 = dwPickDataBuf_AT2[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2_AT2 = dwPickDataBuf_AT2[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3_AT2 = dwPickDataBuf_AT2[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue_AT2 = MULTIPLES*ToAbs(((lSampData1_AT2-lSampData2_AT2)-(lSampData2_AT2-lSampDate3_AT2)));
				
				//���ø����ż�ֵ�жϣ���I>0.25*��I'+ C,���Ц�I'Ϊ�ϸ�һ���ڽ��ĵ��ͻ����ֵ��CΪͻ����������ֵ
				if(lPickValue_AT2*4>*RS_pnPIC*4+m_lPickUpValue_AT2[j])
				{
					m_wPickCheck_AT2++;
					if(m_wPickCheck_AT2 ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime_AT2);
						PickTime = PickUpTime_AT2.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime_AT2.Lo-(i*625+500)/1000;
						PickUpTime_AT2.Lo= (DWORD)PickTime;
						PickUpTime_AT2.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //�����ݿ���д����������
						RptSerialNumOfAct_AT2 = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //�����ݿ���д����������
						RptSerialNumOfStart_AT2 = g_RptSerialNum;
						m_wPickCheck_AT2 =0;
						for(LONG m=0;m<4;m++)
						{
							m_wPickCheckNext_AT2[m] =0;
							m_lPickUpValue_AT2[m]=0; 
						}
						
						return TRUE;	
					}
					if(i<2)
					{
						m_wPickCheckNext_AT2[j]++;
						if(m_wPickCheckNext_AT2[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime_AT2);
							PickTime = PickUpTime_AT2.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime_AT2.Lo-(i*625+500)/1000;
							PickUpTime_AT2.Lo= (DWORD)PickTime;
							PickUpTime_AT2.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
							//ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //�����ݿ���д����������
							RptSerialNumOfAct_AT2 = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //�����ݿ���д����������
							RptSerialNumOfStart_AT2 = g_RptSerialNum;
							m_wPickCheck_AT2 =0;
							for(LONG m=0;m<4;m++)
							{
								m_wPickCheckNext_AT2[m] =0;
								m_lPickUpValue_AT2[m]=0;
							}
							
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						m_wPickCheckNext_AT2[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						m_wPickCheckNext_AT2[j]++;
				}
				else
				{
					m_wPickCheck_AT2 =0;
					m_wPickCheckNext_AT2[j]=0;
				}
				m_lPickUpValue_AT2[j] = lPickValue_AT2;
			}
		}
	}
	
	WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
	return FALSE;
}

//------------------------------------------------------------------------------------------ 
// ����: ����ѭ�����Ƴ���							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
void CRelayTask::FalutProcess_AT1(void)
{ 	
	if(FaultOnFlag_AT1 ==FALSE)   						//��δ������ϴ���
	{		
		RelayPickUpCHK_AT1();					//ִ���������
		if(ReadRelayWord(RW_PICKUP_AT1))
		{
			ResetAllRelay_AT1(TRUE);
			FaultSINum_AT1 = RELAY_SAMP_INTERVAL;       //������������ʼ�����ϴ���������������
			FaultOnFlag_AT1 = TRUE;
		}
		else
			return;		
	}
	FaultSINum_AT1 += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(FaultSINum_AT1<POINT_PER_PERIOD)					//���ϴ���������������С��һ�ܲ���������
	{
		return;
	}
	else
	{
		if(HaveMakeStartRptFlag_AT1 == FALSE)
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart_AT1);
			HaveMakeStartRptFlag_AT1 = TRUE;
//			ReportMsg("relay start report!!!!!");
		}
		
		GeneralOverValueRelay(&RelR50IC1);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&RelR50IT1);				//���ù��������ӳ���
		GeneralOverValueRelay(&RelR50IF1);				//���ò���1�����ӳ���
		GeneralOverValueRelay(&RelR50IP1);				//���ò���2�����ӳ���

		if(ReadRelayWord(RW_52BF_AT1))
		{
			if(!m_bHaveMake52BFRpt_AT1)
			{
				MakeActRpt(R_52BF_AT1,RptSerialNumOfAct_AT1);	//����·����բʧ�ܱ���
				m_bHaveMake52BFRpt_AT1 = TRUE;
			}
		}

		if(ReadRelayWord(RelR50IC1.LoopCtrlRelayWord)||ReadRelayWord(RelR50IT1.LoopCtrlRelayWord)||ReadRelayWord(RelR50IF1.LoopCtrlRelayWord)  		//�б���ѭ�����Ƽ̵����ֱ���λ
			||ReadRelayWord(RelR50IP1.LoopCtrlRelayWord))
		{	
			if(ReadRelayWord(RW_TR1))
				m_bHaveRelayAct_AT1 = TRUE;
			if(ReadRelayWord(RW_52BF_AT1))			//��բʧ�ܣ������б�־������������
			{
				m_bHaveMake52BFRpt_AT1 = FALSE;
				WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
				FaultCheckOutFlag_AT1 = FALSE;
				FaultOnFlag_AT1 = FALSE;
				HaveMakeStartRptFlag_AT1= FALSE;
				DB_RelayEnd(RptSerialNumOfAct_AT1); //�����ݿ���д���������				
				m_bFaultLKFlag_AT1 = FALSE;
				return;
			}
			else
			{
				FaultLockTime_AT1 = dwRelayMSCnt;		//���ù���������ʱ�� 
				FaultCheckOutFlag_AT1 = TRUE;			//�б���ѭ�����Ƽ̵�������λ���ü�����ϱ�־
				return;
			}
		}
		else				//�ޱ���ѭ�����Ƽ̵�����λ: 1�����ϴ����Ѿ�������2��δ����������
		{
			if(FaultCheckOutFlag_AT1 ==TRUE	)      // 1�����ϴ����Ѿ�����
			{	
				if(ReadRelayWord(RW_T52BF_AT1))
				{
					FaultLockTime_AT1 = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime_AT1>1000 )      //���ϴ���������޹���ʱ�䳬��1�룬��λ���������̵����֣����־������������
				{	
					WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
					FaultCheckOutFlag_AT1 = FALSE;
					FaultOnFlag_AT1 = FALSE;
					HaveMakeStartRptFlag_AT1 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT1);						
					m_bFaultLKFlag_AT1 = FALSE;
					ResetAllRelay_AT1(TRUE);					
					m_bHaveMake52BFRpt_AT1 = FALSE;
					return;					
				}
				else
					return;
			}
			else							//δ����������
			{
				if(m_bFaultLKFlag_AT1 == FALSE)     //���ϸ���������¼��ʼ����ʱ��
				{	
					FaultLockTime_AT1 = dwRelayMSCnt;	
					m_bFaultLKFlag_AT1 = TRUE;					
				}
				
				if(dwRelayMSCnt -FaultLockTime_AT1>500 )	//δ�����������޹���ʱ�䳬��0.5�룬��λ���������̵����֣����־������������
				{
					WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
					FaultCheckOutFlag_AT1 = FALSE;
					FaultOnFlag_AT1 = FALSE;
					HaveMakeStartRptFlag_AT1 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT1); 
					m_bFaultLKFlag_AT1 = FALSE;
					ResetAllRelay_AT1(TRUE);					
					m_bHaveMake52BFRpt_AT1 = FALSE;
					return;					
				}
				else
					return;
			}			
		}		
	}
}
//------------------------------------------------------------------------------------------ 
// ����: ����ѭ�����Ƴ���							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
void CRelayTask::FalutProcess_AT2(void)
{ 	
	if(FaultOnFlag_AT2 ==FALSE)   						//��δ������ϴ���
	{		
		RelayPickUpCHK_AT2();					//ִ���������
		if(ReadRelayWord(RW_PICKUP_AT2))
		{
			ResetAllRelay_AT2(TRUE);
			FaultSINum_AT2 = RELAY_SAMP_INTERVAL;       //������������ʼ�����ϴ���������������
			FaultOnFlag_AT2 = TRUE;
		}
		else
			return;		
	}
	FaultSINum_AT2 += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(FaultSINum_AT2<POINT_PER_PERIOD)					//���ϴ���������������С��һ�ܲ���������
	{
		return;
	}
	else
	{
		if(HaveMakeStartRptFlag_AT2 == FALSE)
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart_AT2);
			HaveMakeStartRptFlag_AT2 = TRUE;
		}
		
		GeneralOverValueRelay(&RelR50IC2);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&RelR50IT2);				//���ù��������ӳ���
		GeneralOverValueRelay(&RelR50IF2);				//���ò���1�����ӳ���
		GeneralOverValueRelay(&RelR50IP2);				//���ò���2�����ӳ���

		if(ReadRelayWord(RW_52BF_AT2))
		{
			if(!m_bHaveMake52BFRpt_AT2)
			{
				MakeActRpt(R_52BF_AT2,RptSerialNumOfAct_AT2);   //����·����բʧ�ܱ���
				m_bHaveMake52BFRpt_AT2 = TRUE;
			}
		}

		if(ReadRelayWord(RelR50IC2.LoopCtrlRelayWord)||ReadRelayWord(RelR50IT2.LoopCtrlRelayWord)||ReadRelayWord(RelR50IF2.LoopCtrlRelayWord)  		//�б���ѭ�����Ƽ̵����ֱ���λ
			||ReadRelayWord(RelR50IP2.LoopCtrlRelayWord))
		{			
			if(ReadRelayWord(RW_TR2))
				m_bHaveRelayAct_AT2 = TRUE;
			if(ReadRelayWord(RW_52BF_AT2))			//��բʧ�ܣ������б�־������������
			{
				m_bHaveMake52BFRpt_AT2 = FALSE;
				WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
				FaultCheckOutFlag_AT2 = FALSE;
				FaultOnFlag_AT2 = FALSE;
				HaveMakeStartRptFlag_AT2= FALSE;
				DB_RelayEnd(RptSerialNumOfAct_AT2); //�����ݿ���д���������				
				m_bFaultLKFlag_AT2 = FALSE;
				return;
			}
			else
			{
				FaultLockTime_AT2 = dwRelayMSCnt;		//���ù���������ʱ�� 
				FaultCheckOutFlag_AT2 = TRUE;			//�б���ѭ�����Ƽ̵�������λ���ü�����ϱ�־
				return;
			}
		}
		else				//�ޱ���ѭ�����Ƽ̵�����λ: 1�����ϴ����Ѿ�������2��δ����������
		{
			if(FaultCheckOutFlag_AT2 ==TRUE	)      // 1�����ϴ����Ѿ�����
			{				
//				if((m_bHaveRelayAct_AT2 == TRUE)&&(m_bHaveMake52BFRpt_AT2 ==FALSE))
//				{					
//					MakeActRpt(R_52BF_AT2,RptSerialNumOfAct_AT2);	//����·����բʧ�ܱ���
//					m_bHaveMake52BFRpt_AT2 = TRUE;
//				}
				if(ReadRelayWord(RW_T52BF_AT2))
				{
					FaultLockTime_AT2 = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime_AT2>1000 )      //���ϴ���������޹���ʱ�䳬��1�룬��λ���������̵����֣����־������������
				{	
					WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
					FaultCheckOutFlag_AT2 = FALSE;
					FaultOnFlag_AT2 = FALSE;
					HaveMakeStartRptFlag_AT2 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT2);						
					m_bFaultLKFlag_AT2 = FALSE;
					ResetAllRelay_AT2(TRUE);					
					m_bHaveMake52BFRpt_AT2 = FALSE;
					return;					
				}
				else
					return;
			}
			else							//δ����������
			{
				if(m_bFaultLKFlag_AT2 == FALSE)     //���ϸ���������¼��ʼ����ʱ��
				{	
					FaultLockTime_AT2 = dwRelayMSCnt;	
					m_bFaultLKFlag_AT2 = TRUE;					
				}
				
				if(dwRelayMSCnt -FaultLockTime_AT2>500 )	//δ�����������޹���ʱ�䳬��0.5�룬��λ���������̵����֣����־������������
				{
					WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
					FaultCheckOutFlag_AT2 = FALSE;
					FaultOnFlag_AT2 = FALSE;
					HaveMakeStartRptFlag_AT2 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT2); 
					m_bFaultLKFlag_AT2 = FALSE;
					ResetAllRelay_AT2(TRUE);					
					m_bHaveMake52BFRpt_AT2 = FALSE;
					return;					
				}
				else
					return;
			}			
		}		
	}
}


void CRelayTask::ResetAllRelay_AT1(BOOL resetFlag)
{	
	m_bHaveMake52BFRpt_AT1 = FALSE;
	m_bHaveRelayAct_AT1 =FALSE;
	ResetOverValueRelay(&RelR50IC1,resetFlag);
	ResetOverValueRelay(&RelR50IT1,resetFlag);
	ResetOverValueRelay(&RelR50IF1,resetFlag);
	ResetOverValueRelay(&RelR50IP1,resetFlag);
	
}
void CRelayTask::ResetAllRelay_AT2(BOOL resetFlag)
{				
	m_bHaveMake52BFRpt_AT2 = FALSE;
	m_bHaveRelayAct_AT2 =FALSE;
	ResetOverValueRelay(&RelR50IC2,resetFlag);
	ResetOverValueRelay(&RelR50IT2,resetFlag);
	ResetOverValueRelay(&RelR50IF2,resetFlag);
	ResetOverValueRelay(&RelR50IP2,resetFlag);
	
}

void CRelayTask::Process52BF_AT1()
{
	if(ReadRelayWord(RW_52BF_AT1))
	{
		m_b52BFFlag_AT1 = TRUE;
	}
	if(m_b52BFFlag_AT1)
	{
		if(!ReadRelayWord(RW_52BF_AT1))
		{
			ResetAllRelay_AT1(TRUE);
			m_b52BFFlag_AT1 = FALSE;
		}
	}
}
void CRelayTask::Process52BF_AT2()
{
	if(ReadRelayWord(RW_52BF_AT2))
	{
		m_b52BFFlag_AT2 = TRUE;
	}
	if(m_b52BFFlag_AT2)
	{
		if(!ReadRelayWord(RW_52BF_AT2))
		{
			ResetAllRelay_AT2(TRUE);
			m_b52BFFlag_AT2 = FALSE;
		}
	}
}

