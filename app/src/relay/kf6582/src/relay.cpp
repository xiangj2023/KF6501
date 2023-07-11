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
*           		�����㷨����     FOR  6582�����Զ�Ͷ��װ��                                                                  
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

extern DWORD 	dwRelayMSCnt;		//�����ú��������
extern DWORD 	g_hDbaseTaskID;
extern DWORD 	dwRelayDataPtr;
extern BYTE 	byRelayWordValue[];
extern BYTE 	RptSerialNum;			//�������к� 
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
	
	//��ȡ���ݿⱣ������ֵ����ָ�룬�Լӿ�����ٶ�
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	
	InitAllRelayMem();
	InitRealy();
	InitDCRecInfo();

	
	dwChanDataMask = ((0x01<<CHANNEL_6582_U1)|(0x01<<CHANNEL_6582_U2)|(0x01<<CHANNEL_6582_U3)|(0x01<<CHANNEL_6582_U4) \
					|(0x01<<CHANNEL_6582_U5)|(0x01<<CHANNEL_6582_U6)|(0x01<<CHANNEL_6582_U7)|(0x01<<CHANNEL_6582_U8) \
					|(0x01<<CHANNEL_6582_I1)|(0x01<<CHANNEL_6582_I2)|(0x01<<CHANNEL_6582_I3)|(0x01<<CHANNEL_6582_I4) \
					|(0x01<<CHANNEL_6582_I5)|(0x01<<CHANNEL_6582_I6)|(0x01<<CHANNEL_6582_I7)|(0x01<<CHANNEL_6582_I8));
//	dwRecDataMask = ((0x01<<CHANNEL_6582_IC1)|(0x01<<CHANNEL_6582_IC2));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	//��ʼ����ȡ����������ʱ������
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);	
//	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);	
//	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);	
	
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

//	ASSERT(dwRecBuf != NULL);
//	if(dwRecBuf == NULL)
//	{
//		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
//		return;
//	}
//	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2));
	
//	ASSERT(dwComposeBuf != NULL);
//	if(dwComposeBuf == NULL)
//	{
//		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
//		return;
//	}
//	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*2));
	
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

	RelayTaskInitOK = TRUE;	
	m_bLogicChanged = FALSE;
	g_MaintSW = SW_pbyMAINT;
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
/*	 InitRelR50IC1();
	 InitRelR50IT1();
	 InitRelR50IF1();
	 InitRelR50IP1();
	 InitRelR50IC2();
	 InitRelR50IT2();
	 InitRelR50IF2();
	 InitRelR50IP2();*/
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
	
	//���Ƚ��п����Լ�,ÿ�α��������һ·����
	if(!m_bLogicChanged)
		BoBreakCheck();
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
//	WriteComposeBuf(dwRecDataMask);
	CaluUI();          //���ݲ������ݣ��������It,If,U1,U2	
//	FalutProcess();	//����ѭ������	
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
//void CRelayTask::WriteComposeBuf(DWORD dwMask)
//{
//	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��
//	{
//		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*9-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*10-RELAY_SAMP_INTERVAL+i];
//	}
//	for(LONG i=0;i<READ_POINT_NUM;i++)
//	{
//		dwComposeBuf[i]= dwSampBuf[READ_POINT_NUM*8+i] - dwSampBuf[READ_POINT_NUM*9+i];
//	}
//	
//	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��
//	{
//		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*12-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*13-RELAY_SAMP_INTERVAL+i];
//	}
//	for(LONG i=0;i<READ_POINT_NUM;i++)
//	{
//		dwComposeBuf[READ_POINT_NUM+i]= dwSampBuf[READ_POINT_NUM*11+i] - dwSampBuf[READ_POINT_NUM*12+i];
//	}

//	WriteComposeData(dwRecBuf, dwMask);
//}
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
//	CaluBaseFourier_S15(dwComposeBuf,&IC1);
//	CaluModValue(&IC1);
//	*RM_pnIC1= IC1.Mod;                                       
//	DB_LogWriteRM(RM_IC1, *RM_pnIC1);	
//	// 16. Ic2
//	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&IC2);
//	CaluModValue(&IC2);
//	*RM_pnIC2= IC2.Mod;                                       
//	DB_LogWriteRM(RM_IC2, *RM_pnIC2);

}
/****************************************************************************************
��  ��: InitRelR50IC1
��  ��: ��ʼ������1����Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::InitRelR50IC1()
//{
//	RelR50IC1.pEleSet			= RS_pn60I_AT1C;						//������ֵָ�� 
//	RelR50IC1.pTEleVal			= &IC1;						//����ָ�� 
//	RelR50IC1.ACTRelayWord		= RW_TR60I_AT1;					//�����̵����� 
//	RelR50IC1.StartRelayWord	= RW_PI60I_AT1;					//��μ̵�����
//	RelR50IC1.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//}
 /****************************************************************************************
 ��  ��: InitRelR50IT1
 ��  ��: ��ʼ��1ATT�߹�������Ԫ��
 ��  ��: ��
 ����ֵ: ��
 *****************************************************************************************/
// void CRelayTask::InitRelR50IT1()
//{
//	RelR50IT1.pEleSet			= RS_pn50T_AT1C;						//������ֵָ�� 
//	RelR50IT1.pTEleVal			= &I1;						//����ָ�� 
//	RelR50IT1.ACTRelayWord		= RW_TR50T_AT1;					//�����̵����� 
//	RelR50IT1.StartRelayWord	= RW_PI50T_AT1;					//��μ̵�����
//	RelR50IT1.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//} 
/****************************************************************************************
��  ��: InitRelR50IF1
��  ��: ��ʼ��1ATF�߹�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::InitRelR50IF1()
//{
//	RelR50IF1.pEleSet			= RS_pn50F_AT1C;						//������ֵָ�� 
//	RelR50IF1.pTEleVal			= &I2;						//����ָ�� 
//	RelR50IF1.ACTRelayWord		= RW_TR50F_AT1;					//�����̵����� 
//	RelR50IF1.StartRelayWord	= RW_PI50F_AT1;					//��μ̵�����
//	RelR50IF1.wExcepWordNum	= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//} 
/****************************************************************************************
��  ��: InitRelR50IP1
��  ��: ��ʼ��1AT���ǹ�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::InitRelR50IP1()
//{
//	RelR50IP1.pEleSet			= RS_pn50CS_AT1C;						//������ֵָ�� 
//	RelR50IP1.pTEleVal			= &I3;						//����ָ�� 
//	RelR50IP1.ACTRelayWord		= RW_TR50CS_AT1;					//�����̵����� 
//	RelR50IP1.StartRelayWord	= RW_PI50CS_AT1;					//��μ̵�����
//	RelR50IP1.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//}
/****************************************************************************************
��  ��: InitRelR50IC2
��  ��: ��ʼ��2AT������������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::InitRelR50IC2()
//{
//	RelR50IC2.pEleSet			= RS_pn60I_AT2C;						//������ֵָ�� 
//	RelR50IC2.pTEleVal			= &IC2;						//����ָ�� 
//	RelR50IC2.ACTRelayWord		= RW_TR60I_AT2;					//�����̵����� 
//	RelR50IC2.StartRelayWord	= RW_PI60I_AT2;					//��μ̵�����
//	RelR50IC2.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//}
/****************************************************************************************
��  ��: InitRelR50IT2
��  ��: ��ʼ��2ATT�߹�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::InitRelR50IT2()
//{
//	RelR50IT2.pEleSet			= RS_pn50T_AT2C;						//������ֵָ�� 
//	RelR50IT2.pTEleVal			= &I4;						//����ָ�� 
//	RelR50IT2.ACTRelayWord		= RW_TR50T_AT2;					//�����̵����� 
//	RelR50IT2.StartRelayWord	= RW_PI50T_AT2;					//��μ̵�����
//	RelR50IT2.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//}
/****************************************************************************************
��  ��: InitRelR50IF2
��  ��: ��ʼ��2ATF�߹�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/

//void CRelayTask::InitRelR50IF2()
//{
//	RelR50IF2.pEleSet			= RS_pn50F_AT2C;						//������ֵָ�� 
//	RelR50IF2.pTEleVal			= &I5;						//����ָ�� 
//	RelR50IF2.ACTRelayWord		= RW_TR50F_AT2;					//�����̵����� 
//	RelR50IF2.StartRelayWord	= RW_PI50F_AT2;					//��μ̵�����
//	RelR50IF2.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//}
/****************************************************************************************
��  ��: InitRelR50IP2
��  ��: ��ʼ��2AT���ǹ�������Ԫ��
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::InitRelR50IP2()
//{
//	RelR50IP2.pEleSet			= RS_pn50CS_AT2C;						//������ֵָ�� 
//	RelR50IP2.pTEleVal			= &I6;						//����ָ�� 
//	RelR50IP2.ACTRelayWord		= RW_TR50CS_AT2;					//�����̵����� 
//	RelR50IP2.StartRelayWord	= RW_PI50CS_AT2;					//��μ̵�����
//	RelR50IP2.wExcepWordNum		= 0;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 

//}
void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
/*	StructCheck[0].pstruct = (WORD*)&RelR50IC1;
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
	StructCheck[7].bigsize = sizeof(RelR50IP2)/2 - RelR50IP2.wExcepWordNum; */
	StructCheck[0].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[0].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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

/****************************************************************************************
��  ��: FalutProcess
��  ��: ����ѭ��������
��  ��: ��
����ֵ: ��
*****************************************************************************************/
//void CRelayTask::FalutProcess(void)
//{ 	
//	SimpleOverValRelay(&RelR50IC1);
//	SimpleOverValRelay(&RelR50IT1);
//	SimpleOverValRelay(&RelR50IF1);
//	SimpleOverValRelay(&RelR50IP1);
//	SimpleOverValRelay(&RelR50IC2);
//	SimpleOverValRelay(&RelR50IT2);
//	SimpleOverValRelay(&RelR50IF2);
//	SimpleOverValRelay(&RelR50IP2);
//}
