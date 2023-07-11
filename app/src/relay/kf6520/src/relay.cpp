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
#include "bo.h"
#include "daefunc.h"

extern DWORD dwRelayMSCnt;		//�����ú��������
extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;			//AD��ǰдָ��
extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern BYTE 	g_RptSerialNum;			//�������к� 
extern BOOL		RelayCheckErr;			//����Ԫ���Լ�����־
extern BOOL 	RelayCheckInitOK;		//����Ԫ���Լ��ʼ����־
extern BYTE		*g_MaintSW;

TLoadSupV * pTodayImax = (TLoadSupV *)DBASE_NVRAM_6520;
TLoadSupV * pTodayUmin = pTodayImax+1;
TLoadSupV * pYtdayImax = pTodayImax+2;
TLoadSupV * pYtdayUmin = pTodayImax+3;
TLoadSupV * pDaybyImax = pTodayImax+4;
TLoadSupV * pDaybyUmin = pTodayImax+5;
TSysTime  * pRecurrenceTime = (TSysTime*)(pDaybyUmin+1);  //���ݽ���ʱ��
DWORD	  * pDataValidity = (DWORD *)(pRecurrenceTime+1);//������Ч�Լ��ֵ
WORD	  * g_pHaveSwitchData = (WORD*)(pDataValidity+1);//���ݽ�����־

DWORD		dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
DWORD		dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
DWORD 		dwRelayTaskID;
BOOL 		RelayTaskInitOK = FALSE;
TStructCheck StructCheck[CHECK_NUM];
extern DWORD dwADResumeCnt;
BOOL				g_FaultOnFlag;				//�������ѭ�������־ 

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
	//��ʼ�������������к�
	InitRepSerialNum();	
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
//	RptSerialNumOfPick = 0;			//ϵͳ��α������к�    yanxs  13-02-05
//	RptFlagOfPickUp = FALSE ;			//ϵͳ��α����Ѿ�������־
	RptSerialNumOfAct =0;			//ϵͳ�ı��������������к�
	RptSerialNumOfStart=0;			//ϵͳ�Ĵ����ϵ����������������к�	
	FaultSINum = 0;					//�������������	
	
	PickUpRW = RW_PICKUP;			//ģֵͻ���������̵�����
	TripFailFlag = FALSE;			//��բʧ�ܱ�־
	FaultOnFlag = FALSE;			//������ϴ����־
	g_FaultOnFlag = FALSE;			//������ϴ����־
	HaveMakeStartRptFlag = FALSE;	//���������ϵ������������־
	m_bFaultLKFlag = FALSE;			//������ʼʱ��������־
	CurRecoverFlag =FALSE;   		//�غ�բ����������ָ���־
	FaultCheckOutFlag = FALSE;		//������ϱ�־
	m_bIsHaveMakePTRpt = FALSE;		//����PT���߱������뱣�������־
	m_bLastRecStatusFlag = FALSE;	//����¼�������������жϱ�־
	m_bOneOhmPTLK= FALSE;			//����翹С��1ŷķ��־
	m_bPDCheck = FALSE;				//���������书�ʷ������־
	m_lOtherRelayTrip = FALSE;	
	m_bHaveRelayAct	= FALSE;		//���ι��ϴ����б����Ѿ�������־
	m_b52BFFLAG = FALSE;
	m_bLogicChanged = FALSE;
	m_bR79PickUpRpt = FALSE;
	m_bHaveTripFailedRpt = FALSE;
	
	K35 = 0;						//г������K35
	PickCheck = 0;					//ͻ���������ж������������
	PickCheckNext = 0;				//ͻ���������ж���������������������	
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
	dwChanDataMask = ( (0x01<<CHANNEL_6520_It)|(0x01<<CHANNEL_6520_If)|(0x01<<CHANNEL_6520_U1)|\
						(0x01<<CHANNEL_6520_U2)|(0x01<<CHANNEL_6520_U3)|(0x01<<CHANNEL_6520_U4));
	//�����������ݶ�ȡͨ������
	dwInCreDataMask=(0x01<<CHANNEL_6520_IP);
	//��д�ϳ�������ͨ������
	dwComposeDataMask = ((0x01<<CHANNEL_6520_IP)|(0x01<<CHANNEL_6520_UP));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	
	//��ʼ����ȡ����������ʱ������
	//�������ݻ�����
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);	
	//�ϳ����ݻ�д������
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);
	//���������������ݻ�����
	dwIncreCurBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);	
	//�ϳ�ͨ�����ݻ�����
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);
	//ͻ����������ʷ���ݻ�����
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	//�����书�ʷ����ж����ݻ�����
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);

	ASSERT(dwIncreCurBuf != NULL);
	if(dwIncreCurBuf == NULL)
	{
		LogError("CRelayTask,dwIncreCurBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwIncreCurBuf, 0, sizeof(DWORD)*READ_POINT_NUM);

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

	//�����ǳ�ʼ���߼����̲���
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	
	*RM_pnTL = 0;
	*RM_pnFL = 0;
	g_MaintSW = SW_pbyMAINT;

	//��ʼ�������Լ�Ϊ����״̬
	
	for(LONG i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();

	InitLoadMonitor();

	RelayTaskInitOK = TRUE;
	SetTimer(1,5);

//���迹ֵ��ʼ��ΪĬ��ֵ����ֱֹ����ʽ�£�F���迹ֵ��ʾ������
	Impd.Real = INVALIDRX*(*CF_pnZ);
	Impd.Imag = INVALIDRX*(*CF_pnZ);
	ImpdT.Real = INVALIDRX*(*CF_pnZ);
	ImpdT.Imag = INVALIDRX*(*CF_pnZ);	
	ImpdF.Real = INVALIDRX*(*CF_pnZ);
	ImpdF.Imag = INVALIDRX*(*CF_pnZ);	

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
		WriteRelayWord(RW_BTMODE, RELAY_ON);
		WriteRelayWord(RW_AT1MODE, RELAY_OFF);
		WriteRelayWord(RW_AT2MODE, RELAY_OFF);
	}
	else if(*AS_pnPM == AT1MODE)
	{
		WriteRelayWord(RW_BTMODE, RELAY_OFF);
		WriteRelayWord(RW_AT1MODE, RELAY_ON);
		WriteRelayWord(RW_AT2MODE, RELAY_OFF);
	}
	
	else if(*AS_pnPM == AT2MODE)
	{
		WriteRelayWord(RW_BTMODE, RELAY_OFF);
		WriteRelayWord(RW_AT1MODE, RELAY_OFF);
		WriteRelayWord(RW_AT2MODE, RELAY_ON);
	}
}
/************************************************************************************************
*����: ��ʼ������Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	InitRel50I0();
	InitRel50I1();
	InitRel50I2();
	InitRel50I3();
	InitRel211();
	InitRel212();
	InitRel213();
	InitRel214();
	InitRel27U();
    InitRel51I();
	InitRel21FL_F();	
	InitRel21FL_T();
	InitRelPT1();
	InitRelPT2();
	InitRel79();
	InitRel50DI();
	InitStructCheck();
	
}
void CRelayTask::OnTimeOut(DWORD id)
{		
//	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
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

	//�ϳ�Up,Ipͨ������д
	WriteComposeBuf();
	//�������It/If/U1/U2
	CaluUI(); 
	//����Ip/Up�Լ�г������
	CaluUpIp();	
	CaluUIAngle();
	//�����迹
	CaluZ();

	//г�����Ƶ�K35����K35=1+k(I3+I5)/I1,�Ŵ�100��
	if(!ReadRelayWord(RW_37IP))
	{
		//����г�����ơ��ٶ�г�����ơ�����г��������Ͷ�������¼���K35
		if((*CFG_pby21_HRES== CFG_STATUS_SET) || (*CFG_pby50_0_HRES == CFG_STATUS_SET) || (*CFG_pby50_HRES== CFG_STATUS_SET)) 
		{
			//���EEPROM���ˣ�RW_37IP���������ϣ���ʱҪ��֤IP����0
			if((Ip.Mod*100<(I3.Mod+I5.Mod))||(Ip.Mod<2)) 
				K35 = 10000;
			else				
				K35 = *CF_pnC2 + (*RS_pnFACTOR1)*(I3.Mod+I5.Mod)/Ip.Mod;
		}
	}
	else
		K35 = *CF_pnC2;

	//������ϴ���֮�󲻽���PT�����ж�
	if(FaultOnFlag == FALSE)
	{
		PTCheck();	
	}
	FalutProcess();								//����ѭ������
	GeneralUnderValRelay(&Rel27U);				//�͵�ѹ�����ӳ���
	AutoRCAct(&Rel79);							//�����غ�բ�ӳ���
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
	//���ɼ��
	LoadMonitor();
	
	//���������ϴ����з�ֹ���ݴ�����������±�������
//	if(m_bNextRelayStart)
//	{
//		if(!ReadRelayWord(RW_37IP))  //�е���
//		{
//			FaultSINum = 0;
//			m_bNextRelayStart = FALSE;
//		//	FaultCheckOutFlag = FALSE;
//		}
//	}

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
// ����: ���ɼ��                                                           							          
// ����: ��                                       
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::LoadMonitor()
{	
	//���ɼ��	
	static WORD SwitchTimes=0;
	LONG CurrentH;
	LONG VoltageH;
//	LONG AngleCurr;
	LONG AngleVolt;
	TSysTime SysTime;
	TAbsTime AbsTime;
	CurrentH = *AS_pnPCTRatio*Ip.Mod;
	VoltageH = *AS_pnPT1Ratio*Up.Mod;
//	AngleCurr = 0;
	AngleVolt = Up.Angle - Ip.Angle;
	ReadSystemTime(&SysTime);
	ReadAbsTime(&AbsTime);
	if(SysTime.Hour == (BYTE)*AS_pnLMT)
	{
		if(*g_pHaveSwitchData == FALSE)
		{
			*pDaybyImax = *pYtdayImax;
			*pDaybyUmin = *pYtdayUmin;
			*pYtdayImax = *pTodayImax;
			*pYtdayUmin = *pTodayUmin;
			if(!ReadRelayWord(RW_37IP)&&(CurrentH < *AS_pnLMCMAX)) //���ݽ���ʱ�������ɼ���¼Ϊ����ʱ��ʵ�ʵ�ѹ����ֵ
			{
				pTodayImax->Current = CurrentH;			
				pTodayImax->Voltage = VoltageH;
//				pTodayImax->AngleCurr = AngleCurr;			
				pTodayImax->AngleVolt = AngleVolt;
			}
			else			//���ݽ���ʱ�������ɼ���¼Ϊ0
			{
				pTodayImax->Current = 0;			
				pTodayImax->Voltage = 0;
//				pTodayImax->AngleCurr = 0;			
				pTodayImax->AngleVolt = 0;
			}
			pTodayImax->Time	= AbsTime;
			if(!ReadRelayWord(RW_37IP)&&(VoltageH > (*AS_pnLMVMIN)*10)) //���ݽ���ʱ�������ɼ���¼Ϊ����ʱ��ʵ�ʵ�ѹ����ֵ
			{
				pTodayUmin->Current = CurrentH;		
				pTodayUmin->Voltage = VoltageH;
//				pTodayUmin->AngleCurr = AngleCurr;			
				pTodayUmin->AngleVolt = AngleVolt;
			}
			else  //���ݽ���ʱ�������ɼ���¼Ϊ0
			{
				pTodayUmin->Current = 0;		
				pTodayUmin->Voltage = 0;
//				pTodayUmin->AngleCurr = 0;			
				pTodayUmin->AngleVolt = 0;
			}
			pTodayUmin->Time	= AbsTime;
//			pTodayImax->Current = 0;//���ݽ���֮����ĵ���ֵ��Ϊ0,��ѹ��Ϊ200V(���ǵ���רATģʽ)
//			pTodayUmin->Voltage = *AS_pnPT1Ratio*(*CF_pnUP)*2;
			*pRecurrenceTime = SysTime;
			*g_pHaveSwitchData = TRUE;
			ReportMsg("load monitor: switch data!%d-%d,%d:%d:%d %d",SysTime.Month,SysTime.Day,SysTime.Hour,SysTime.Minute,SysTime.Second,SysTime.MSecond);
			return;
		}
	}
	else
	{
		*g_pHaveSwitchData = FALSE;
	}		
	if(!ReadRelayWord(RW_37IP))
	{
		if((CurrentH > pTodayImax->Current)&&(CurrentH < *AS_pnLMCMAX))
		{
			pTodayImax->Current = CurrentH;
			pTodayImax->Voltage = VoltageH;
//			pTodayImax->AngleCurr = AngleCurr;			
			pTodayImax->AngleVolt = AngleVolt;
			pTodayImax->Time	= AbsTime;
		}
		if(VoltageH > (*AS_pnLMVMIN)*10)
		{
			if(pTodayUmin->Voltage == 0) //��һ�γ�ʼ��ʱ
			{
				pTodayUmin->Current = CurrentH;		
				pTodayUmin->Voltage = VoltageH;
//				pTodayUmin->AngleCurr = AngleCurr;			
				pTodayUmin->AngleVolt = AngleVolt;
				pTodayUmin->Time	= AbsTime;
//				pTodayUmin->Voltage = *AS_pnPT1Ratio*(*CF_pnUP)*2;
			}
//			if((VoltageH < pTodayUmin->Voltage)&&(VoltageH > (*AS_pnLMVMIN)*10))
			if(VoltageH < pTodayUmin->Voltage)
			{
				pTodayUmin->Current = CurrentH;		
				pTodayUmin->Voltage = VoltageH;
//				pTodayUmin->AngleCurr = AngleCurr;			
				pTodayUmin->AngleVolt = AngleVolt;
				pTodayUmin->Time	= AbsTime;
			}
		}
	}
	//WriteRelayWord(RW_37IP, RELAY_ON);
}
//------------------------------------------------------------------------------------------ 
// ����: д�ϳɲ������ݻ�����	                                                           							          
// ����:  pBuf1,ԭʼ������������ݻ�������pBuf2,�ϳɲ��������ݻ�������dwMask,ͨ������                                        
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::WriteComposeBuf()
{	
	
	if(*AS_pnPM == BTMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++) 
		{
			dwComposeBuf[i]=dwSampBuf[i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)   //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��ͨ��ΪUp,16
		{
			dwRecBuf[i] = dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		}

		for(LONG i=0;i<READ_POINT_NUM;i++)	 
		{
			dwComposeBuf[READ_POINT_NUM+i]=dwSampBuf[READ_POINT_NUM*2+i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)   //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��ͨ��ΪIp,17
		{
			dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwSampBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		}

	}
	else if(*AS_pnPM == AT1MODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++) 
		{
			dwComposeBuf[i]=dwSampBuf[i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)   //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��ͨ��ΪUp,16
		{
			dwRecBuf[i] = dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		}

		for(LONG i=0;i<READ_POINT_NUM;i++)	 
		{
			dwComposeBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*2+i]-dwSampBuf[READ_POINT_NUM*3+i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)  //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��ͨ��ΪIp,17
		{
			dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwSampBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		}

	}
	else if(*AS_pnPM == AT2MODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)     //��U1��U2ͨ�����������ݺϳ�Upͨ������������ 
		{
			dwComposeBuf[i]=dwSampBuf[i]-dwSampBuf[READ_POINT_NUM+i];
		}		
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��ͨ��ΪUp,16
		{
			dwRecBuf[i] = dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		}

		for(LONG i=0;i<READ_POINT_NUM;i++)	 
		{
			dwComposeBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*2+i]-dwSampBuf[READ_POINT_NUM*3+i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)  //���»�д�㣬ÿ���ϳ�ͨ�������ΪSAMPLENUMOFRELTASK��ͨ��ΪIp,17
		{
			dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwSampBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		}

	}

	WriteComposeData(dwRecBuf, dwComposeDataMask);
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
	CaluBaseFourier_S15(dwSampBuf,&U1);
	CaluModValue(&U1);
	*RM_pnU1= U1.Mod;                                       
	DB_LogWriteRM(RM_U1, *RM_pnU1);	
	if(U1.Mod>=*AS_pn59U1V)	
	{	
		WriteRelayWord(RW_59U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(U1.Mod>=*AS_pn27U1V)
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}

	//����U2ģֵ��д������������U2��ѹ�̵����֡���ѹ�̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	*RM_pnU2= U2.Mod;                                       
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
	if(U2.Mod>=*AS_pn59U2V)		
	{	
		WriteRelayWord(RW_59U2, RELAY_ON);
	}
	else	
	{	
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(U2.Mod>=*AS_pn27U2V)
	{	
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}
	else
	{	
		WriteRelayWord(RW_27U2, RELAY_ON);
	}
	 //����Itģֵ��д������������It�����̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&It);
	CaluModValue(&It);
	*RM_pnIt= It.Mod;                                       
	DB_LogWriteRM(RM_IT, *RM_pnIt);
	if(It.Mod>(*AS_pn50ITC))
	{				
		WriteRelayWord(RW_50IT, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IT, RELAY_OFF);
	}
	if(It.Mod>(*AS_pn37ITC))
	{
		WriteRelayWord(RW_37IT, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IT, RELAY_ON);
	}	
	 //����Ifģֵ��д������������If�����̵�����
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&If);
	CaluModValue(&If);
	*RM_pnIf= If.Mod;                                       
	DB_LogWriteRM(RM_IF, *RM_pnIf);
	if(If.Mod>*AS_pn50IFC)
	{				
		WriteRelayWord(RW_50IF, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IF, RELAY_OFF);
	}
	if(If.Mod>(*AS_pn37IFC))
	{
		WriteRelayWord(RW_37IF, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IF, RELAY_ON);
	}	
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&U3);
	CaluModValue(&U3);
	*RM_pnU3= U3.Mod;                                       
	DB_LogWriteRM(RM_U3, *RM_pnU3);
	if(U3.Mod>*AS_pn59U3V)
	{				
		WriteRelayWord(RW_59U3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3, RELAY_OFF);
	}
	if(U3.Mod>(*AS_pn27U3V))
	{
		WriteRelayWord(RW_27U3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3, RELAY_ON);
	}	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&U4);
	CaluModValue(&U4);
	*RM_pnU4= U4.Mod;                                       
	DB_LogWriteRM(RM_U4, *RM_pnU4);
	if(U4.Mod>*AS_pn59U4V)
	{				
		WriteRelayWord(RW_59U4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U4, RELAY_OFF);
	}
	if(U4.Mod>(*AS_pn27U4V))
	{
		WriteRelayWord(RW_27U4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U4, RELAY_ON);
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
	//��רAT���緽ʽ��UP=Ut-Uf
	if(*AS_pnPM == AT2MODE)
	{
		CaluBaseFourier_S15(dwComposeBuf,&Up);
		CaluModValue(&Up);
	}
	else
	{
		Up=U1;
	}	
	*RM_pnUp= Up.Mod;                                       
	DB_LogWriteRM(RM_UP, *RM_pnUp);	
	if(Up.Mod >= *AS_pn59UPV)		//��Up��ѹ�̵����֡���ѹ�̵�����
	{
		WriteRelayWord(RW_59UP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UP, RELAY_OFF);
	}
	if(Up.Mod >= *AS_pn27UPV)
	{
		WriteRelayWord(RW_27UP, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UP, RELAY_ON);
	}
	
	//AT���緽ʽ��IP=It-If
	if(*AS_pnPM == BTMODE)
 	{
		Ip=It;
 	}
	else
	{		
		CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&Ip);  
		CaluModValue(&Ip);	 	
	}
	*RM_pnIp= Ip.Mod;                                       
	DB_LogWriteRM(RM_IP, *RM_pnIp);	
	if(Ip.Mod>*AS_pn50IPC)		
	{
		WriteRelayWord(RW_50IP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IP, RELAY_OFF);
	}
	if(Ip.Mod>*AS_pn37IPC)
	{
		WriteRelayWord(RW_37IP, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IP, RELAY_ON);
	}	

	//����I2ģֵ��д��������
	if(*AS_pnPM == BTMODE)
	 	CaluSecondFourier(dwSampBuf+READ_POINT_NUM*2,&I2);		
	else
	 	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM,&I2);	 		 
	CaluModValue(&I2);
	I2.Mod = I2.Mod*CurHarmCal[0]/1000;
	*RM_pnI2= I2.Mod;                                       
	DB_LogWriteRM(RM_I2, *RM_pnI2);	

	//����I3ģֵ��д��������	
	if(*AS_pnPM == BTMODE)	 
	 	CaluThirdFourier(dwSampBuf+READ_POINT_NUM*2,&I3);
	else
	 	CaluThirdFourier(dwComposeBuf+READ_POINT_NUM,&I3);	 
	CaluModValue(&I3);
	I3.Mod = I3.Mod*CurHarmCal[1]/1000;
	*RM_pnI3= I3.Mod;                                       
	DB_LogWriteRM(RM_I3, *RM_pnI3);

	//����I5ģֵ��д��������
	if(*AS_pnPM == BTMODE)	 
		CaluFifthFourier(dwSampBuf+READ_POINT_NUM*2,&I5);		
	else	 	
	 	CaluFifthFourier(dwComposeBuf+READ_POINT_NUM,&I5);	 
	CaluModValue(&I5);
	I5.Mod = I5.Mod*CurHarmCal[2]/1000;
	*RM_pnI5= I5.Mod;                                       
	DB_LogWriteRM(RM_I5, *RM_pnI5);

	if(*AS_pnPM == AT2MODE) //��ר���緽ʽ
	{
		if(U1.Mod>=U2.Mod)
		{
			if(U2.Mod>=Up.Mod)
				ULock = Up;
			else
				ULock = U2;
		}
		else
		{
			if(U1.Mod>=Up.Mod)
				ULock = Up;
			else
				ULock = U1;
		}
	}
	else
		ULock = Up;
	
}
void CRelayTask::CaluZ(void)
{
	//BT���緽ʽ��Zp=Zt
	if (*AS_pnPM == BTMODE)
	{
		//����T���迹	
		CaluRX(&U1,&It,&ImpdT,RW_37IP,CF_pnZ);	//����T���迹
		CaluImpdAngle(&ImpdT,AngleTab);
		*RM_pnTR= ImpdT.Real;  
		*RM_pnTX= ImpdT.Imag;
		*RM_pnTZA= ImpdT.Angle;
		DB_LogWriteRM(RM_TR, *RM_pnTR); 
		DB_LogWriteRM(RM_TX, *RM_pnTX); 
		DB_LogWriteRM(RM_TZA, *RM_pnTZA);
		
		Impd=ImpdT;		
		*RM_pnR= Impd.Real;  
		*RM_pnX= Impd.Imag;
		*RM_pnZA = Impd.Angle;
		DB_LogWriteRM(RM_R, *RM_pnR);	
		DB_LogWriteRM(RM_X, *RM_pnX);
		DB_LogWriteRM(RM_ZA, *RM_pnZA);			
		*M_pnR = Impd.Real;		
		*M_pnX = Impd.Imag;
		DB_WriteAI(M_R, *M_pnR);
		DB_WriteAI(M_X, *M_pnX);
	}	
	else    
	{
		if(*AS_pnPM == AT2MODE)			//AT2���緽ʽ�¼���T,F���迹
		{			
			CaluRX(&U1,&It,&ImpdT,RW_37IP,CF_pnZ);	//����T���迹
			CaluImpdAngle(&ImpdT,AngleTab);
			*RM_pnTR= ImpdT.Real;  
			*RM_pnTX= ImpdT.Imag;
			*RM_pnTZA= ImpdT.Angle;
			DB_LogWriteRM(RM_TR, *RM_pnTR); 
			DB_LogWriteRM(RM_TX, *RM_pnTX); 
			DB_LogWriteRM(RM_TZA, *RM_pnTZA);

			CaluRX(&U2,&If,&ImpdF,RW_37IP,CF_pnZ);	//����F���迹
			CaluImpdAngle(&ImpdF,AngleTab);
			*RM_pnFR= ImpdF.Real;  
			*RM_pnFX= ImpdF.Imag;
			*RM_pnFZA = ImpdF.Angle;
			DB_LogWriteRM(RM_FR, *RM_pnFR);	
			DB_LogWriteRM(RM_FX, *RM_pnFX);	
			DB_LogWriteRM(RM_FZA, *RM_pnFZA);	
		}

		CaluRX(&Up,&Ip,&Impd,RW_37IP,CF_pnZ);
		CaluImpdAngle(&Impd,AngleTab);
		*RM_pnR= Impd.Real;  
		*RM_pnX= Impd.Imag;
		*RM_pnZA = Impd.Angle;
		DB_LogWriteRM(RM_R, *RM_pnR);	
		DB_LogWriteRM(RM_X, *RM_pnX);	
		DB_LogWriteRM(RM_ZA, *RM_pnZA);	
		*M_pnR = Impd.Real;		
		*M_pnX = Impd.Imag;
		DB_WriteAI(M_R, *M_pnR);
		DB_WriteAI(M_X, *M_pnX);			
	}
}
void CRelayTask::CaluUIAngle(void)
{	
	LONG angle;
	CaluAngle(&Ip,AngleTab);
	*RM_pnAIP=0;
	DB_LogWriteRM(RM_AIP, *RM_pnAIP);
	CaluAngle(&U1,AngleTab);
	angle = U1.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAU1 = angle+36000;
	else
		*RM_pnAU1 = angle;		
	DB_LogWriteRM(RM_AU1, *RM_pnAU1);
	
	CaluAngle(&U2,AngleTab);	
	angle = U2.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAU2 = angle+36000;
	else
		*RM_pnAU2 = angle;	
	DB_LogWriteRM(RM_AU2, *RM_pnAU2);	
	
	CaluAngle(&Up,AngleTab);	
	angle = Up.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAUP = angle+36000;
	else
		*RM_pnAUP = angle;	
	DB_LogWriteRM(RM_AUP, *RM_pnAUP);	
	
	CaluAngle(&It,AngleTab);	
	angle = It.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAIT = angle+36000;
	else
		*RM_pnAIT = angle;	
	DB_LogWriteRM(RM_AIT, *RM_pnAIT);	
	
	CaluAngle(&If,AngleTab);	
	angle = If.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAIF = angle+36000;
	else
		*RM_pnAIF = angle;	
	DB_LogWriteRM(RM_AIF, *RM_pnAIF);	
//	CaluAngle(&m_sItm,AngleTab);
//	*RM_pnAITM=m_sItm.Angle-Ip.Angle;
//	DB_LogWriteRM(RM_AITM, *RM_pnAITM); 
//	CaluAngle(&m_sIfm,AngleTab);
//	*RM_pnAIFM=m_sIfm.Angle-Ip.Angle;
//	DB_LogWriteRM(RM_AIFM, *RM_pnAIFM);	
}

void CRelayTask::InitLoadMonitor()
{
	
	//NVRam���ݵ���Ч�Լ�飬���ڸ��ɼ������
	if(*pDataValidity != 0x55AA33CC)
	{
		memset(pTodayImax, 0, sizeof(TLoadSupV)*6+sizeof(TSysTime)+6);
		*pDataValidity = 0x55AA33CC;
		ReportMsg("Warning!,NvRam was modified!");
		return;
	}
	TAbsTime tabstime,tabstime1,tabstime2;
	TSysTime tsystime,tsystime1;
	QWORD abstime1,abstime2,abstime3,abstime4;
	ReadAbsTime(&tabstime);
	ReadSystemTime(&tsystime);
	if(tsystime.Year<2000||tsystime.Year>2050) //ʱ�䲻����
	{
	//	ReportMsg("Error!System time is out limit! Load Monitor disabler!");
		return;
	}
	
	if(pRecurrenceTime->Year<2000)
	{
	//	ReportMsg("Error!Recurrenc time is out limit! Load Monitor disabler!");
		return;
	}
	if(pRecurrenceTime->Day == tsystime.Day) //�����Ѿ����й����ݽ���
		return;
	//��һ�����:�ϵ�ʱ������һ�����ݽ���ʱ��֮�䳬����24Сʱ�����Ͻ��н�����	
	abstime1 = tabstime.Hi;
	abstime1 = (abstime1<<32)+tabstime.Lo;
	ToAbsTime(pRecurrenceTime,&tabstime2);
	abstime4 = tabstime2.Hi;
	abstime4 = (abstime4<<32)+tabstime2.Lo;
	if(abstime1 - abstime4 >=0x5265c00)
	{
		*pDaybyImax = *pYtdayImax;
		*pDaybyUmin = *pYtdayUmin;
		*pYtdayImax = *pTodayImax;
		*pYtdayUmin = *pTodayUmin;		
		pTodayImax->Current = 0;
		pTodayImax->Voltage = 0;
//		pTodayImax->AngleCurr = 0;
		pTodayImax->AngleVolt = 0;
		pTodayImax->Time	= tabstime;
		pTodayUmin->Current = 0;
		pTodayUmin->Voltage = 0;
//		pTodayUmin->AngleCurr = 0;
		pTodayUmin->AngleVolt = 0;
		pTodayUmin->Time	= tabstime;
		*pRecurrenceTime = tsystime;
		//�ڳ�ʼ���׶��Ѿ��������ݣ���������ʱ����Ҫ�ٽ���һ��
		if(tsystime.Hour == *AS_pnLMT) 
			*g_pHaveSwitchData = TRUE;

		return;
	}
	//�ڶ������:������8�����������ݣ��ϵ�ʱ�̵�ʱ��������8��0��0��0����ʱ�̵�ʱ���������24Сʱ�����н���
	//С���򲻽��н�������ֹ��������ͣ������
	//�õ����쵱ǰʱ�̵ľ���ʱ��,������Сʱֵ��Ϊ���ݽ���ʱ���Сʱ��
	abstime3 = abstime1-0x5265c00;//0x5265c00Ϊ24Сʱ�ĺ�����	
	tabstime1.Lo= abstime3&0xffffffff;
	tabstime1.Hi= abstime3>>32;	
	AbsTimeTo(&tabstime1,&tsystime1);
	tsystime1.Hour = *AS_pnLMT;
	tsystime1.Minute = 0;
	tsystime1.Second = 0;
	tsystime1.MSecond = 0;
	ToAbsTime(&tsystime1, &tabstime1);
	abstime2 = tabstime1.Hi;
	abstime2 = (abstime2<<32)+tabstime1.Lo;	
	//��ǰ�ϵ�ʱ�����������ݽ���������ʱ�̵�ʱ������1�죬�����������ݽ�����
	if((abstime1 - abstime2) >= 0x5265c00)
	{
		*pDaybyImax = *pYtdayImax;
		*pDaybyUmin = *pYtdayUmin;
		*pYtdayImax = *pTodayImax;
		*pYtdayUmin = *pTodayUmin;		
		pTodayImax->Current = 0;
		pTodayImax->Voltage = 0;
//		pTodayImax->AngleCurr = 0;
		pTodayImax->AngleVolt = 0;
		pTodayImax->Time	= tabstime;
		pTodayUmin->Current = 0;
		pTodayUmin->Voltage = 0;
//		pTodayUmin->AngleCurr = 0;
		pTodayUmin->AngleVolt = 0;
		pTodayUmin->Time	= tabstime;
		*pRecurrenceTime = tsystime;
		//�ڳ�ʼ���׶��Ѿ��������ݣ���������ʱ����Ҫ�ٽ���һ��
		if(tsystime.Hour == *AS_pnLMT) 
			*g_pHaveSwitchData = TRUE;
	}	
}

void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&Rel50I0;
	StructCheck[0].bigsize = sizeof(Rel50I0)/2 - Rel50I0.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&Rel50I1;
	StructCheck[1].bigsize = sizeof(Rel50I1)/2 - Rel50I1.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel50I2;
	StructCheck[2].bigsize = sizeof(Rel50I2)/2 - Rel50I2.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel50I3;
	StructCheck[3].bigsize = sizeof(Rel50I3)/2 - Rel50I3.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel211;
	StructCheck[4].bigsize = sizeof(Rel211)/2 - Rel211.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel212;
	StructCheck[5].bigsize = sizeof(Rel212)/2 - Rel212.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel213;
	StructCheck[6].bigsize = sizeof(Rel213)/2 - Rel213.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel214;
	StructCheck[7].bigsize = sizeof(Rel214)/2 - Rel214.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&Rel27U;
	StructCheck[8].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&Rel51I;
	StructCheck[9].bigsize = sizeof(Rel51I)/2 - Rel51I.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&Rel21FL_T;
	StructCheck[10].bigsize = sizeof(Rel21FL_T)/2 - Rel21FL_T.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)&Rel21FL_F;
	StructCheck[11].bigsize = sizeof(Rel21FL_F)/2 - Rel21FL_F.wExcepWordNum;
	StructCheck[12].pstruct = (WORD*)&RelPT1;
	StructCheck[12].bigsize = sizeof(RelPT1)/2 - RelPT1.wExcepWordNum;
	StructCheck[13].pstruct = (WORD*)&RelPT2;
	StructCheck[13].bigsize = sizeof(RelPT2)/2 - RelPT2.wExcepWordNum;
	StructCheck[14].pstruct = (WORD*)&Rel79;
	StructCheck[14].bigsize = sizeof(Rel79)/2 - Rel79.wExcepWordNum;
	StructCheck[15].pstruct = (WORD*)&Rel50DI;
	StructCheck[15].bigsize = sizeof(Rel50DI)/2 - Rel50DI.wExcepWordNum;
	StructCheck[16].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[16].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
// ����: ��ʼ���ٶϱ���Ԫ��	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I0()
{
	Rel50I0.pElecSet			= RS_pn50_0C;          						//��������������ֵ
	Rel50I0.pTimeSet			= RS_pn50_0D;				 			//��������ʱ�䶨ֵ
	Rel50I0.pHarmKhr1Set		= RS_pnFACTOR1;							//��������г����Ȩϵ��
	Rel50I0.pVolLockSet			= RS_pn50_0_27V;				 	    		//����������ѹ������ֵ
	Rel50I0.pAccActTimeSet		= NULL;									//������������ٶ���ʱ�䶨ֵ
	Rel50I0.pH2Coef				= CF_pnC2;								//����г������ϵ����г����Ȩϵ���ķŴ���
	
	Rel50I0.pRelCFG				= CFG_pby50_0;							//�����������ܿ�����--1:Ͷ�룬2:�˳� 
	Rel50I0.pRelSW				= SW_pby50_0;         					//����������ѹ��------1:Ͷ�룬2:�˳�
	Rel50I0.pRelHLockCFG		= CFG_pby50_0_HLK;			    		//г���������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelHSuprCFG		= CFG_pby50_0_HRES; 					//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelPDCFG			= CFG_pby50_0_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelVolLockCFG		= CFG_pby50_0_27;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�
	Rel50I0.pRelFDCFG			= CFG_pby50_0_D;						//���ʷ����ܿ���λ--1:������2:������
	Rel50I0.pRelAccCFG			= &RelCFGCLR;								//���޺���ٹ���------1:�У�  2:��
	Rel50I0.pRelFLCFG			= CFG_pby21_FL;					//���ϲ��Ͷ��
	Rel50I0.pRel47UCFG			= &RelCFGCLR;					
	Rel50I0.pSysTimer			= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ��	
	
	Rel50I0.pRelElec 			= &Ip;									//��ǰ����ָ��		
	Rel50I0.pRelHarm3			= &I3;									//��ǰ����г��ָ��	
	Rel50I0.pRelHarm5			= &I5;									//��ǰ���г��ָ��	
	Rel50I0.pRelVolt			= &ULock;									//��ǰ���������ĵ�ѹ��ָ��

	Rel50I0.StartRelayWord		= RW_PI50_0;						//����������μ̵�����
	Rel50I0.ActRelayWord		= RW_TR50_0;							//�������������̵�����
	Rel50I0.AccRelayWord		= FALSE1;									//���������̵�����
	Rel50I0.AccActRelayWord		= FALSE1;									//����ٶ����̵�����
	Rel50I0.PForwardRelayWord	= RW_32F;							//���ʷ���̵�����
	Rel50I0.PRewardRelayWord    = RW_32R;
	Rel50I0.H2LockRelayWord		= RW_LKH2;							//����г�������̵�����
	
	Rel50I0.R47ULKRelayWord		= FALSE1;
	Rel50I0.LockRelayWord		= RW_LKPI50_0;						//��α����̵�����
	Rel50I0.RSTRelayWord		= RW_LKRST50_0;							//���ر����̵�����
	Rel50I0.LoopCtrlRelayWord	= RW_LPC50_0;						//����ѭ�����Ƽ̵�����
	
	
	Rel50I0.wReportNum			= wRep50I0;							//���������������
	Rel50I0.wAccReportNum		= 0xFFFF;							//���ٶ����������

	Rel50I0.dwK35				= &K35;
	Rel50I0.RptSerialNum		= &RptSerialNumOfAct;				//���������
	Rel50I0.bMakeActRpt			= TRUE;
	//**********************��α��沿��*******************   yanxs  13-02-05
	Rel50I0.wPickUpReportNum	= wRep50I0Pick; 					//��α���inf���
	Rel50I0.wResetReportNum 	= wRep50I0RES;						//���ر���inf���
	Rel50I0.wAccPickUpRptNum	= 0xFFFF; 							//������α���inf���  yanxs  2013-06-20
	Rel50I0.wAccResetRptNum 	= 0xFFFF;							//���ٷ��ر���inf���
	//**********************************************************************
	//Rel50I0.dwRelTimer			= 0;			 			//��ʱ��
	Rel50I0.byRelFCounter		= 0;						//���ϼ�����
	Rel50I0.byRelRCounter		= 0;						//���ؼ�����	
	Rel50I0.wExcepWordNum		= 8;
	Rel50I0.wInfo 				= 0;

}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������I�α���Ԫ��	                                                                     
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I1()
{
	Rel50I1.pElecSet			= RS_pn50_1C;          						//��������������ֵ 
	Rel50I1.pTimeSet			= RS_pn50_1D;				 				//��������ʱ�䶨ֵ 
	Rel50I1.pHarmKhr1Set		= RS_pnFACTOR1;							//��������г����Ȩϵ�� 
	Rel50I1.pVolLockSet			= RS_pn50_1_27V;				 	    	//����������ѹ������ֵ 
	Rel50I1.pAccActTimeSet		= RS_pn94D;						//������������ٶ���ʱ�䶨ֵ 
	Rel50I1.pH2Coef				= CF_pnC2;							//����г������ϵ����г����Ȩϵ���ķŴ���

	Rel50I1.pRelCFG				= CFG_pby50_1;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	Rel50I1.pRelSW				= SW_pby50_123;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	Rel50I1.pRelHLockCFG		= CFG_pby50_HLK;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I1.pRelHSuprCFG		= CFG_pby50_HRES; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I1.pRelPDCFG			= CFG_pby50_1_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I1.pRelFDCFG			= CFG_pby50_1_D;					//
	Rel50I1.pRelVolLockCFG		= CFG_pby50_1_27;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	Rel50I1.pRelAccCFG			= CFG_pby50_1_94;							//���޺���ٹ���------1:�У�  2:�� 
	Rel50I1.pRel47UCFG			= &RelCFGCLR;					//���ϲ��Ͷ��

	Rel50I1.pRelFLCFG			= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel50I1.pSysTimer			= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	Rel50I1.pRelElec 			= &Ip;						//��ǰ����ָ�� 		
	Rel50I1.pRelHarm3			= &I3;						//��ǰ����г��ָ�� 	
	Rel50I1.pRelHarm5			= &I5;						//��ǰ���г��ָ�� 	
	Rel50I1.pRelVolt			= &ULock;						//��ǰ���������ĵ�ѹ��ָ�� 

	Rel50I1.StartRelayWord		= RW_PI50_1;							//����������μ̵����� 
	Rel50I1.ActRelayWord		= RW_TR50_1;							//�������������̵����� 
	Rel50I1.AccRelayWord		= RW_TSOTF;							//���������̵�����,��һ��ʱ��̵����֣���ʱ�����Զ����� 
	Rel50I1.AccActRelayWord		= RW_TR50_1_94;						//����ٶ����̵����� 
	Rel50I1.PForwardRelayWord	= RW_32F;						//����������̵����� 
	Rel50I1.PRewardRelayWord  	= RW_32R;
	Rel50I1.H2LockRelayWord		= RW_LKH2;						//����г�������̵����� 
	Rel50I1.R47ULKRelayWord		= FALSE1;
	Rel50I1.LockRelayWord		= RW_LKPI50_1;							//��α����̵����� 
	Rel50I1.RSTRelayWord		= RW_LKRST50_1;							//���ر����̵����� 
	Rel50I1.LoopCtrlRelayWord	= RW_LPC50_1;						//����ѭ�����Ƽ̵����� 
	
	Rel50I1.wReportNum			= wRep50I1;								//��������������� 
	Rel50I1.wAccReportNum		= wRep50I1Acc;							//���ٶ����������
	
	
	Rel50I1.dwK35				= &K35;
	Rel50I1.RptSerialNum		= &RptSerialNumOfAct;
	Rel50I1.bMakeActRpt			= TRUE;


	//**********************��α��沿��*******************   yanxs  13-02-05
	Rel50I1.wPickUpReportNum	= wRep50I1Pick; 					//��α������
	Rel50I1.wResetReportNum 	= wRep50I1RES;						//���ر������	
	Rel50I1.wAccPickUpRptNum	= wRep50I1AccPick; 					//������α���inf���  yanxs  2013-06-20
	Rel50I1.wAccResetRptNum 	= wRep50I1AccRES;					//���ٷ��ر���inf���
	//**********************************************************************
//	Rel50I1.dwRelTimer			= 0;			 				//��ʱ�� 
	Rel50I1.byRelFCounter		= 0;						//���ϼ����� 
	Rel50I1.byRelRCounter		= 0;						//���ؼ����� 	
	Rel50I1.wExcepWordNum		= 8;
	Rel50I1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������II�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I2()
{	
	Rel50I2.pElecSet			= RS_pn50_2C;          						//��������������ֵ 
	Rel50I2.pTimeSet			= RS_pn50_2D;				 				//��������ʱ�䶨ֵ 
	Rel50I2.pHarmKhr1Set		= RS_pnFACTOR1;							//��������г����Ȩϵ�� 
	Rel50I2.pVolLockSet			= RS_pn50_2_27V;				 	    	//����������ѹ������ֵ 
	Rel50I2.pAccActTimeSet		= RS_pn94D;						//������������ٶ���ʱ�䶨ֵ 
	Rel50I2.pH2Coef				= CF_pnC2;							//����г������ϵ����г����Ȩϵ���ķŴ���

	Rel50I2.pRelCFG				= CFG_pby50_2;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	Rel50I2.pRelSW				= SW_pby50_123;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	Rel50I2.pRelHLockCFG		= CFG_pby50_HLK;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I2.pRelHSuprCFG		= CFG_pby50_HRES; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I2.pRelPDCFG			= CFG_pby50_2_67;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I2.pRelFDCFG			= CFG_pby50_2_D;
	Rel50I2.pRelVolLockCFG		= CFG_pby50_2_27;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	Rel50I2.pRelAccCFG			= CFG_pby50_2_94;							//���޺���ٹ���------1:�У�  2:�� 
	Rel50I2.pRelFLCFG			= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel50I2.pRel47UCFG			= &RelCFGCLR;
	Rel50I2.pSysTimer			= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	Rel50I2.pRelElec 			= &Ip;						//��ǰ����ָ�� 		
	Rel50I2.pRelHarm3			= &I3;						//��ǰ����г��ָ�� 	
	Rel50I2.pRelHarm5			= &I5;						//��ǰ���г��ָ�� 	
	Rel50I2.pRelVolt			= &ULock;						//��ǰ���������ĵ�ѹ��ָ�� 

	Rel50I2.StartRelayWord		= RW_PI50_2;							//����������μ̵����� 
	Rel50I2.ActRelayWord		= RW_TR50_2;							//�������������̵����� 
	Rel50I2.AccRelayWord		= RW_TSOTF;							//���������̵����� 
	Rel50I2.AccActRelayWord		= RW_TR50_2_94;						//����ٶ����̵����� 
	Rel50I2.PForwardRelayWord	= RW_32F;						//����������̵����� 
	Rel50I2.PRewardRelayWord  	= RW_32R;					//���ʷ�����̵����� 
	Rel50I2.H2LockRelayWord		= RW_LKH2;						//����г�������̵����� 
	
	Rel50I2.R47ULKRelayWord		= FALSE1;
	Rel50I2.LockRelayWord		= RW_LKPI50_2;							//��α����̵����� 
	Rel50I2.RSTRelayWord		= RW_LKRST50_2;							//���ر����̵����� 
	Rel50I2.LoopCtrlRelayWord	= RW_LPC50_2;						//����ѭ�����Ƽ̵����� 
	
	Rel50I2.wReportNum			= wRep50I2;								//��������������� 
	Rel50I2.wAccReportNum		= wRep50I2Acc;							//���ٶ���������� 
	
	Rel50I2.dwK35				= &K35;
	Rel50I2.RptSerialNum		= &RptSerialNumOfAct;
	Rel50I2.bMakeActRpt			= TRUE;

	//**********************��α��沿��*******************   yanxs  13-02-05
	Rel50I2.wPickUpReportNum	= wRep50I2Pick; 					//��α������
	Rel50I2.wResetReportNum 	= wRep50I2RES;						//���ر������	
	Rel50I2.wAccPickUpRptNum	= wRep50I2AccPick; 					//������α���inf���  yanxs  2013-06-20
	Rel50I2.wAccResetRptNum 	= wRep50I2AccRES;					//���ٷ��ر���inf���
	//**********************************************************************
	//Rel50I2.dwRelTimer			= 0;			 				//��ʱ�� 
	Rel50I2.byRelFCounter		= 0;						//���ϼ����� 
	Rel50I2.byRelRCounter		= 0;						//���ؼ����� 
	Rel50I2.wExcepWordNum		= 8;
	Rel50I2.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������III�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I3()
{	
	Rel50I3.pElecSet			= RS_pn50_3C;          						//��������������ֵ 
	Rel50I3.pTimeSet			= RS_pn50_3D;				 				//��������ʱ�䶨ֵ 
	Rel50I3.pHarmKhr1Set		= RS_pnFACTOR1;							//��������г����Ȩϵ�� 
	Rel50I3.pVolLockSet			= NULL;				 	    	//����������ѹ������ֵ 
	Rel50I3.pAccActTimeSet		= RS_pn94D;						//������������ٶ���ʱ�䶨ֵ 
	Rel50I3.pH2Coef				= CF_pnC2;							//����г������ϵ����г����Ȩϵ���ķŴ���

	Rel50I3.pRelCFG				= CFG_pby50_3;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	Rel50I3.pRelSW				= SW_pby50_123;         					//����������ѹ��------1:Ͷ�룬2:�˳�  
	Rel50I3.pRelHLockCFG		= CFG_pby50_HLK;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I3.pRelHSuprCFG		= CFG_pby50_HRES; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I3.pRelPDCFG			= &RelCFGCLR;		    				//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	Rel50I3.pRelFDCFG			= &RelCFGCLR;						//����III��Ͷ�����������λ 
	Rel50I3.pRelVolLockCFG		= &RelCFGCLR;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	Rel50I3.pRelAccCFG			= CFG_pby50_3_94;							//���޺���ٹ���------1:�У�  2:�� 
	Rel50I3.pRelFLCFG			= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel50I3.pRel47UCFG			= &RelCFGCLR;
	Rel50I3.pSysTimer			= &dwRelayMSCnt;								//ϵͳ��ʱ��ָ�� 	
	
	Rel50I3.pRelElec 			= &Ip;						//��ǰ����ָ�� 		
	Rel50I3.pRelHarm3			= &I3;						//��ǰ����г��ָ�� 	
	Rel50I3.pRelHarm5			= &I5;						//��ǰ���г��ָ�� 	
	Rel50I3.pRelVolt			= &ULock;						//��ǰ���������ĵ�ѹ��ָ�� 

	Rel50I3.StartRelayWord		= RW_PI50_3;							//����������μ̵����� 
	Rel50I3.ActRelayWord		= RW_TR50_3;							//�������������̵����� 
	Rel50I3.AccRelayWord		= RW_TSOTF;							//���������̵����� 
	Rel50I3.AccActRelayWord		= RW_TR50_3_94;						//����ٶ����̵����� 
	Rel50I3.PForwardRelayWord	= RW_32F;						//����������̵����� 
	Rel50I3.PRewardRelayWord  	= RW_32R;						//���ʷ�����̵����� 
	Rel50I3.H2LockRelayWord		= RW_LKH2;						//����г�������̵����� 
	Rel50I3.R47ULKRelayWord		= FALSE1;
	Rel50I3.LockRelayWord		= RW_LKPI50_3;							//��α����̵����� 
	Rel50I3.RSTRelayWord		= RW_LKRST50_3;							//���ر����̵����� 
	Rel50I3.LoopCtrlRelayWord	= RW_LPC50_3;						//����ѭ�����Ƽ̵����� 
	
	Rel50I3.wReportNum			= wRep50I3;								//��������������� 
	Rel50I3.wAccReportNum		= wRep50I3Acc;							//���ٶ���������� 
	Rel50I3.dwK35				= &K35;
	Rel50I3.RptSerialNum		= &RptSerialNumOfAct;
	Rel50I3.bMakeActRpt			= TRUE;

	//**********************��α��沿��*******************   yanxs  13-02-05
	Rel50I3.wPickUpReportNum	= wRep50I3Pick; 					//��α������
	Rel50I3.wResetReportNum 	= wRep50I3RES;						//���ر������	
	Rel50I3.wAccPickUpRptNum	= wRep50I3AccPick; 					//������α���inf���  yanxs  2013-06-20
	Rel50I3.wAccResetRptNum 	= wRep50I3AccRES;					//���ٷ��ر���inf���
	//**********************************************************************
//	Rel50I3.dwRelTimer			= 0;			 				//��ʱ�� 
	Rel50I3.byRelFCounter		= 0;						//���ϼ����� 
	Rel50I3.byRelRCounter		= 0;						//���ؼ����� 
	Rel50I3.wExcepWordNum		= 8;
	Rel50I3.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������I�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel211()
{
	Rel211.pRel21RSet					= RS_pn21_1R;					//���뱣�����趨ֵ 
	Rel211.pRel21XSet					= RS_pn21_1X;					//���뱣���翹��ֵ 
	Rel211.pRel21TSet    				= RS_pn21_1D;					//���뱣��ʱ�䶨ֵ 
	Rel211.pRel21Khr1Set				= RS_pnFACTOR1;					//���뱣��г������ϵ�� 
	Rel211.pRel21LineAngleSet			= RS_pnZANGLE;					//��·�迹�� 
	Rel211.pRel21AccTSet				= NULL;							//�������ٶ���ʱ�� 
	Rel211.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT���߱���������ֵ 
	Rel211.pH2Coef						= CF_pnC2;						//����г������ϵ����г����Ȩϵ���ķŴ���
	Rel211.pRel21ZOffset				= RS_pn21ZOFFSET;					//ƽ���ı����迹ƫ��ֵ 
	Rel211.pRel21AOffset				= RS_pn21AOFFSET;				//�迹�ı��νǶ�ƫ��
	
	Rel211.pRel21CFG					= CFG_pby21_1;						//���뱣������ 
	Rel211.pRel21SW						= SW_pby21;						//���뱣����ѹ�� 
	Rel211.pRel21DirCFG					= CFG_pby21_1_D;					//���뱣������ 
	Rel211.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//ƽ���ı��� 
	Rel211.pRel21HLockCFG				= CFG_pby21_HLK;					//���뱣��г������ 
	Rel211.pRel21HSuprCFG				= CFG_pby21_HRES;					//���뱣��г������ 
	Rel211.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT���߼�� 
	Rel211.pRel21ACCCFG					= &RelCFGCLR;							//����ٹ��ܿ��� 
	Rel211.pRelFLCFG					= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel211.pSysTimer					= &dwRelayMSCnt;					//ϵͳ��ʱ��ָ�� 
	Rel211.pTangTabAddr					= TangAngleTab;						//���б��׵�ַ 

	Rel211.pRel21Cur					= &Ip;							//����ָ�� 
	Rel211.pRel21Vol					= &Up;							//��ѹָ�� 
	Rel211.pRel21Impd					= &Impd;						//��ǰ���迹 
	Rel211.pRel21H3						= &I3;							//����г��ָ�� 
	Rel211.pRel21H5						= &I5;							//���г��ָ�� 

	Rel211.pnZCoef						= CF_pnZ;						//�迹ϵ��

	Rel211.StartRelayWord				= RW_PI21_1;						//���뱣����μ̵����� 
	Rel211.ACTRelayWord					= RW_TR21_1;						//���뱣�������̵����� 
	Rel211.ACCRelayWord					= FALSE1;						//���������̵����� 
	Rel211.ACCActRelayWord				= FALSE1;					//����ٶ����̵����� 
	Rel211.PTBreakRelayWord				= RW_ARVT1;					//PT���߼̵����� 
	Rel211.H2LockRelayWord				= RW_LKH2;					//����г�������̵����� 
	Rel211.LockRelayWord				= RW_LKPI21_1;						//��α����̵����� 
	Rel211.LoopCtrlRelayWord			= RW_LPC21_1;					//����ѭ�����Ƽ̵����� 
	Rel211.R37IRelayWord				= RW_37IP;						//�����̵�����

	Rel211.wRel21RptNum					= wRep211;				     	//���뱣��������� 
	Rel211.wRel21AccRptNum				= 0xFFFF;					//���뱣������ٱ������ 
	Rel211.wRel21PTLockRptNum			= R_PTLK;					//PT���߱������뱣��������� 
	
	Rel211.dwK35						= &K35;
	Rel211.RptSerialNum					= &RptSerialNumOfAct;	
	//**********************��α��沿��*******************   yanxs  13-03-07
	Rel211.wPickUpReportNum				= wRep211Pick; 					//��α������
	Rel211.wResetReportNum 				= wRep211RES;					//���ر������	
	Rel211.wAccPickUpRptNum				= 0xFFFF; 						//������α������  2013-06-20  yanxs
	Rel211.wAccResetRptNum 				= 0xFFFF;						//���ٷ��ر������	
	//**********************************************************************
	
//	Rel211.dwRel21Timer					= 0;						//��ʱ�� 	
	Rel211.byRel21FCounter				= 0;						//���뱣�����ϼ����� 
	Rel211.byRel21RCounter				= 0;						//���뱣�����ؼ����� 
	Rel211.wExcepWordNum				= 8;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 

}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������II�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel212()
{	
	Rel212.pRel21RSet					= RS_pn21_2R;					//���뱣�����趨ֵ 
	Rel212.pRel21XSet					= RS_pn21_2X;					//���뱣���翹��ֵ 
	Rel212.pRel21TSet    				= RS_pn21_2D;					//���뱣��ʱ�䶨ֵ 
	Rel212.pRel21Khr1Set				= RS_pnFACTOR1;					//���뱣��г������ϵ�� 
	Rel212.pRel21LineAngleSet			= RS_pnZANGLE;					//��·�迹�� 
	Rel212.pRel21AccTSet				= RS_pn94D;					//�������ٶ���ʱ�� 
	Rel212.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT���߱���������ֵ 
	Rel212.pH2Coef						= CF_pnC2;						//����г������ϵ����г����Ȩϵ���ķŴ���
	Rel212.pRel21ZOffset				= RS_pn21ZOFFSET;					//ƽ���ı����迹ƫ��ֵ 
	Rel212.pRel21AOffset				= RS_pn21AOFFSET;				//�迹�ı��νǶ�ƫ��

	Rel212.pRel21CFG					= CFG_pby21_2;						//���뱣������ 
	Rel212.pRel21SW						= SW_pby21;						//���뱣����ѹ�� 
	Rel212.pRel21DirCFG					= CFG_pby21_2_D;					//���뱣������ 
	Rel212.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//ƽ���ı��� 
	Rel212.pRel21HLockCFG				= CFG_pby21_HLK;					//���뱣��г������ 
	Rel212.pRel21HSuprCFG				= CFG_pby21_HRES;					//���뱣��г������ 
	Rel212.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT���߼�� 
	Rel212.pRel21ACCCFG					= CFG_pby21_2_94;							//����ٹ��ܿ��� 
	Rel212.pRelFLCFG					= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel212.pSysTimer					= &dwRelayMSCnt;					//ϵͳ��ʱ��ָ�� 
	Rel212.pTangTabAddr					= TangAngleTab;						//���б��׵�ַ 

	Rel212.pRel21Cur					= &Ip;							//����ָ�� 
	Rel212.pRel21Vol					= &Up;							//��ѹָ�� 
	Rel212.pRel21Impd					= &Impd;						//��ǰ���迹 
	Rel212.pRel21H3						= &I3;							//����г��ָ�� 
	Rel212.pRel21H5						= &I5;							//���г��ָ�� 

	Rel212.pnZCoef						= CF_pnZ;						//�迹ϵ��

	Rel212.StartRelayWord				= RW_PI21_2;						//���뱣����μ̵����� 
	Rel212.ACTRelayWord					= RW_TR21_2;						//���뱣�������̵����� 
	Rel212.ACCRelayWord					= RW_TSOTF;						//���������̵����� 
	Rel212.ACCActRelayWord				= RW_TR21_2_94;					//����ٶ����̵����� 
	Rel212.PTBreakRelayWord				= RW_ARVT1;					//PT���߼̵����� 
	Rel212.H2LockRelayWord				= RW_LKH2;					//����г�������̵����� 
	Rel212.LockRelayWord				= RW_LKPI21_2;						//��α����̵����� 
	Rel212.LoopCtrlRelayWord			= RW_LPC21_2;					//����ѭ�����Ƽ̵����� 
	Rel212.R37IRelayWord				= RW_37IP;
	
	Rel212.wRel21RptNum					= wRep212;				     	//���뱣��������� 
	Rel212.wRel21AccRptNum				= wRepR212Acc;					//���뱣������ٱ������ 
	Rel212.wRel21PTLockRptNum			= R_PTLK;					//PT���߱������뱣��������� 
	
	Rel212.dwK35						= &K35;
	Rel212.RptSerialNum					= &RptSerialNumOfAct;
	//**********************��α��沿��*******************   yanxs  13-03-07
	Rel212.wPickUpReportNum				= wRep212Pick; 					//��α������
	Rel212.wResetReportNum 				= wRep212RES;					//���ر������	
	Rel212.wAccPickUpRptNum				= wRepR212AccPick; 				//������α������  2013-06-20  yanxs
	Rel212.wAccResetRptNum 				= wRepR212AccRES;				//���ٷ��ر������	
	//**********************************************************************

//	Rel212.dwRel21Timer					= 0;						//��ʱ�� 	
	Rel212.byRel21FCounter				= 0;						//���뱣�����ϼ����� 
	Rel212.byRel21RCounter				= 0;					//���뱣�����ؼ����� 
	Rel212.wExcepWordNum				= 8;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
//------------------------------------------------------------------------------------------ 
// ����: ��ʼ������III�α���Ԫ��                                                         
// ����:                                          
// ���: ��                                                          
// ����: ��                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel213()
{
	Rel213.pRel21RSet					= RS_pn21_3R;					//���뱣�����趨ֵ 
	Rel213.pRel21XSet					= RS_pn21_3X;					//���뱣���翹��ֵ 
	Rel213.pRel21TSet    				= RS_pn21_3D;					//���뱣��ʱ�䶨ֵ 
	Rel213.pRel21Khr1Set				= RS_pnFACTOR1;					//���뱣��г������ϵ�� 
	Rel213.pRel21LineAngleSet			= RS_pnZANGLE;					//��·�迹�� 
	Rel213.pRel21AccTSet				= RS_pn94D;					//�������ٶ���ʱ�� 
	Rel213.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT���߱���������ֵ 
	Rel213.pH2Coef						= CF_pnC2;						//����г������ϵ����г����Ȩϵ���ķŴ���
	Rel213.pRel21ZOffset				= RS_pn21ZOFFSET;					//ƽ���ı����迹ƫ��ֵ 
	Rel213.pRel21AOffset				= RS_pn21AOFFSET;				//�迹�ı��νǶ�ƫ��

	Rel213.pRel21CFG					= CFG_pby21_3;						//���뱣������ 
	Rel213.pRel21SW						= SW_pby21;						//���뱣����ѹ�� 
	Rel213.pRel21DirCFG					= CFG_pby21_3_D;					//���뱣������ 
	Rel213.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//ƽ���ı��� 
	Rel213.pRel21HLockCFG				= CFG_pby21_HLK;					//���뱣��г������ 
	Rel213.pRel21HSuprCFG				= CFG_pby21_HRES;					//���뱣��г������ 
	Rel213.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT���߼�� 
	Rel213.pRel21ACCCFG					= CFG_pby21_3_94;					//����ٹ��ܿ��� 
	Rel213.pRelFLCFG					= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel213.pSysTimer					= &dwRelayMSCnt;					//ϵͳ��ʱ��ָ�� 
	Rel213.pTangTabAddr					= TangAngleTab;						//���б��׵�ַ 

	Rel213.pRel21Cur					= &Ip;							//����ָ�� 
	Rel213.pRel21Vol					= &Up;							//��ѹָ�� 
	Rel213.pRel21Impd					= &Impd;						//��ǰ���迹 
	Rel213.pRel21H3						= &I3;							//����г��ָ�� 
	Rel213.pRel21H5						= &I5;							//���г��ָ�� 

	Rel213.pnZCoef						= CF_pnZ;						//�迹ϵ��

	Rel213.StartRelayWord				= RW_PI21_3;						//���뱣����μ̵����� 
	Rel213.ACTRelayWord					= RW_TR21_3;						//���뱣�������̵����� 
	Rel213.ACCRelayWord					= RW_TSOTF;						//���������̵����� 
	Rel213.ACCActRelayWord				= RW_TR21_3_94;					//����ٶ����̵����� 
	Rel213.PTBreakRelayWord				= RW_ARVT1;					//PT���߼̵����� 
	Rel213.H2LockRelayWord				= RW_LKH2;					//����г�������̵����� 
	Rel213.LockRelayWord				= RW_LKPI21_3;						//��α����̵����� 
	Rel213.LoopCtrlRelayWord			= RW_LPC21_3;					//����ѭ�����Ƽ̵����� 
	Rel213.R37IRelayWord				= RW_37IP;
	
	Rel213.wRel21RptNum					= wRep213;				     	//���뱣��������� 
	Rel213.wRel21AccRptNum				= wRepR213Acc;					//���뱣������ٱ������ 
	Rel213.wRel21PTLockRptNum			= R_PTLK;					//PT���߱������뱣��������� 
	Rel213.dwK35						= &K35;
	Rel213.RptSerialNum					= &RptSerialNumOfAct;
	//**********************��α��沿��*******************   yanxs  13-03-07
	Rel213.wPickUpReportNum				= wRep213Pick; 					//��α������
	Rel213.wResetReportNum 				= wRep213RES;					//���ر������	
	Rel213.wAccPickUpRptNum				= wRepR213AccPick; 				//������α������  2013-06-20  yanxs
	Rel213.wAccResetRptNum 				= wRepR213AccRES;				//���ٷ��ر������	
	//**********************************************************************

//	Rel213.dwRel21Timer					= 0;						//��ʱ�� 	
	Rel213.byRel21FCounter				= 0;						//���뱣�����ϼ����� 
	Rel213.byRel21RCounter				= 0;					//���뱣�����ؼ����� 
	Rel213.wExcepWordNum				= 8;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
/************************************************************************************************
*����: ��ʼ������IV�α���Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/
void CRelayTask::InitRel214()
{
	Rel214.pRel21RSet					= RS_pn21_4R;					//���뱣�����趨ֵ 
	Rel214.pRel21XSet					= RS_pn21_4X;					//���뱣���翹��ֵ 
	Rel214.pRel21TSet    				= RS_pn21_4D;					//���뱣��ʱ�䶨ֵ 
	Rel214.pRel21Khr1Set				= RS_pnFACTOR1;					//���뱣��г������ϵ�� 
	Rel214.pRel21LineAngleSet			= RS_pnZANGLE;					//��·�迹�� 
	Rel214.pRel21AccTSet				= RS_pn94D;					//�������ٶ���ʱ�� 
	Rel214.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT���߱���������ֵ 
	Rel214.pH2Coef						= CF_pnC2;						//����г������ϵ����г����Ȩϵ���ķŴ���
	Rel214.pRel21ZOffset				= RS_pn21ZOFFSET;					//ƽ���ı����迹ƫ��ֵ 
	Rel214.pRel21AOffset				= RS_pn21AOFFSET;				//�迹�ı��νǶ�ƫ��

	Rel214.pRel21CFG					= CFG_pby21_4;						//���뱣������ 
	Rel214.pRel21SW						= SW_pby21;						//���뱣����ѹ�� 
	Rel214.pRel21DirCFG					= CFG_pby21_4_D;					//���뱣������ 
	Rel214.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//ƽ���ı��� 
	Rel214.pRel21HLockCFG				= CFG_pby21_HLK;					//���뱣��г������ 
	Rel214.pRel21HSuprCFG				= CFG_pby21_HRES;					//���뱣��г������ 
	Rel214.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT���߼�� 
	Rel214.pRel21ACCCFG					= CFG_pby21_4_94;					//����ٹ��ܿ��� 
	Rel214.pRelFLCFG					= CFG_pby21_FL;				//���ϲ��Ͷ�� 
	Rel214.pSysTimer					= &dwRelayMSCnt;					//ϵͳ��ʱ��ָ�� 
	Rel214.pTangTabAddr					= TangAngleTab;						//���б��׵�ַ 

	Rel214.pRel21Cur					= &Ip;							//����ָ�� 
	Rel214.pRel21Vol					= &Up;							//��ѹָ�� 
	Rel214.pRel21Impd					= &Impd;						//��ǰ���迹 
	Rel214.pRel21H3						= &I3;							//����г��ָ�� 
	Rel214.pRel21H5						= &I5;							//���г��ָ�� 

	Rel214.pnZCoef						= CF_pnZ;						//�迹ϵ��

	Rel214.StartRelayWord				= RW_PI21_4;						//���뱣����μ̵����� 
	Rel214.ACTRelayWord					= RW_TR21_4;						//���뱣�������̵����� 
	Rel214.ACCRelayWord					= RW_TSOTF;						//���������̵����� 
	Rel214.ACCActRelayWord				= RW_TR21_4_94;					//����ٶ����̵����� 
	Rel214.PTBreakRelayWord				= RW_ARVT1;					//PT���߼̵����� 
	Rel214.H2LockRelayWord				= RW_LKH2;						//����г�������̵����� 
	Rel214.LockRelayWord				= RW_LKPI21_4;					//��α����̵����� 
	Rel214.LoopCtrlRelayWord			= RW_LPC21_4;					//����ѭ�����Ƽ̵����� 
	Rel214.R37IRelayWord				= RW_37IP;
	
	Rel214.wRel21RptNum					= wRep214;				     			//���뱣��������� 
	Rel214.wRel21AccRptNum				= wRepR214Acc;						//���뱣������ٱ������ 
	Rel214.wRel21PTLockRptNum			= R_PTLK;						//PT���߱������뱣��������� 
	
	Rel214.dwK35						= &K35;
	Rel214.RptSerialNum					= &RptSerialNumOfAct;
	//**********************��α��沿��*******************   yanxs  13-03-07
	Rel214.wPickUpReportNum				= wRep214Pick; 					//��α������
	Rel214.wResetReportNum 				= wRep214RES;					//���ر������	
	Rel214.wAccPickUpRptNum				= wRepR214AccPick; 				//������α������  2013-06-20  yanxs
	Rel214.wAccResetRptNum 				= wRepR214AccRES;				//���ٷ��ر������	
	//**********************************************************************

//	Rel214.dwRel21Timer					= 0;						//��ʱ�� 	
	Rel214.byRel21FCounter				= 0;						//���뱣�����ϼ����� 
	Rel214.byRel21RCounter				= 0;					//���뱣�����ؼ����� 
	Rel214.wExcepWordNum				= 8;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
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
	Rel27U.pLockCurSet					= RS_pn27_37C;					//����������ֵ 

	Rel27U.pRelCFG						= CFG_pby27;					  	 //Ƿ���������� 
	Rel27U.pRelSW						= SW_pby27;					  	 //Ƿ��������ѹ�� 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//Ƿ������Ͷ��բ 
	Rel27U.pRelCurLockCFG				= &RelCFGSET;
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//ϵͳ��ʱ��ָ�� 

	Rel27U.pRelElec						= &Up;						//��ǰ�ĵ���ָ�� 
	Rel27U.pRelLockElec					= &Ip;					//�������� 
	Rel27U.ActRelayWord					= RW_TR27;					//Ƿ�����������̵����� 
	Rel27U.StartRelayWord				= RW_PI27;					//Ƿ��������μ̵����� 
	Rel27U.AlarmRelayWord				= RW_AR27;					//Ƿ�������澯�̵����� 
	Rel27U.LockRelayWord				= RW_LKPI27;				//��α����̵����� 
	Rel27U.RSTRelayWord					= RW_LKRST27;					//���Ϸ��ر����̵����� 
	Rel27U.LoopCtrlRelayWord			= RW_LPC27;				//����ѭ�����Ƽ̵����� 
	Rel27U.wR52BFRealyword				= RW_52BF;					//��·����բʧ�̵ܼ�����
	Rel27U.wActRptNum					= wRep27U;					//Ƿ����������������� 
	Rel27U.wAlarmRptNum					= R_R27ALARM;					//Ƿ�������澯������� 
	Rel27U.wTRFailRptNum				= R_TRFAIL;
	Rel27U.wPickUpRptNum				= R_ACTPICK;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
//	Rel27U.wResetReportNum				= R_R27U_RES;
//	Rel27U.wPickUpReportNum				= wRep27UPick;
	Rel27U.R27RptSerialNum				= 0;
//	Rel27U.dwRelTimer					= 0;						//Ƿ��������ʱ�� 	
	Rel27U.byRelFCounter				= 0;					//���ϼ����� 
	Rel27U.byRelRCounter				= 0;					//���ؼ����� 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ10 
}
/************************************************************************************************
*����: ��ʼ����ʱ�ޱ���Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRel51I()
{
	Rel51I.pRelCurSet					= RS_pn51C;                   			 //��ʱ�ޱ���������ֵ 
	Rel51I.pRelTimeSet					= RS_pn51TD;					//��ʱ�ޱ���ʱ�䶨ֵ 
	Rel51I.pRelTypeSet					= RS_pn51TYPE;					//��ʱ�ޱ����������� 

	Rel51I.pRelCFG						= CFG_pby51;						//��ʱ�ޱ������� 
	Rel51I.pRelSW						= SW_pby50_123;						//��ʱ�ޱ�����ѹ��,�õ��ǹ�������ѹ�� 
	Rel51I.pSysTimer					= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel51I.pCoefTabA					= InverseCurCoefA;					//һ�㷴ʱ��ϵ�����ַ 
	Rel51I.pCoefTabB					= InverseCurCoefB;						//����ʱ��ϵ�����ַ 
	Rel51I.pCoefTabC					= InverseCurCoefC;						//����ʱ��ϵ�����ַ 

	Rel51I.pRelElec						= &Ip;						//��ǰ�ĵ���ָ�� 
	Rel51I.dwCaluCurSet					= *RS_pn51C*11/10;					//�����ķ�ʱ�޵�����ֵ 

	Rel51I.ACTRelayWord					= RW_TR51;					//��ʱ�޶����̵����� 
	Rel51I.StartRelayWord				= RW_PI51;					//��ʱ����μ̵����� 
	Rel51I.LockRelayWord				= RW_LKPI51;					//��α����̵����� 
	Rel51I.RSTRelayWord					= RW_LKRST51;					//���ر����̵����� 
	Rel51I.LoopCtrlRelayWord			= RW_LPC51;				//����ѭ�����Ƽ̵����� 
	Rel51I.wRelRptNum					= wRep51I;					//��ʱ�ޱ���������� 
	Rel51I.RptSerialNum					= &RptSerialNumOfAct;
	//**********************��α��沿��*******************   yanxs  13-03-08
	Rel51I.wPickUpReportNum				= wRep51IPick; 					//��α���inf���
	Rel51I.wResetReportNum 				= wRep51IRES;						//���ر���inf���
//	Rel51I.RptPickupSerialNum			= &RptSerialNumOfPick;				//��α����
//	Rel51I.RptPickupFlag				= &RptFlagOfPickUp;					//��α���������־
	//**********************************************************************


//	Rel51I.dwRelTimer					= 0;						//��ʱ�ޱ�����ʱ�� 	
	Rel51I.byRelRCounter				= 0;						//���ϼ����� 
	Rel51I.byRelRCounter				= 0;						//���ؼ����� 
	Rel51I.CurCounter					= 0;						//�����ۼ��� 
	Rel51I.LimitCouter					= 0;						//���޶���ʱ���ж��õ��ۼӴ��� 
	Rel51I.MaxCouter					= MAXINVERSETIME*50*POINT_PER_PERIOD/RELAY_SAMP_INTERVAL;	
	Rel51I.CurItem						= 0;

	if (*(RS_pn51TYPE) == 1)								//�����ۼ��������Ķ�ֵ (28000*50*SAMPNUM*Is*Is*Ts)  
	{
		Rel51I.CurCounterSet = (LLONG)INVERSECOEFA*POINT_PER_PERIOD*(*RS_pn51TD) \
			*(*RS_pn51C)*(*RS_pn51C)/(20*RELAY_SAMP_INTERVAL);
	}
	else if (*(RS_pn51TYPE) == 2)
	{
		Rel51I.CurCounterSet = (LLONG)INVERSECOEFB*POINT_PER_PERIOD*(*RS_pn51TD) \
			*(*RS_pn51C)*(*RS_pn51C)/(20*RELAY_SAMP_INTERVAL);
	}
	else if (*(RS_pn51TYPE) == 3)
	{
		Rel51I.CurCounterSet = (LLONG)INVERSECOEFC*POINT_PER_PERIOD*(*RS_pn51TD) \
			*(*RS_pn51C)*(*RS_pn51C)/(20*RELAY_SAMP_INTERVAL);
	}					
	Rel51I.wExcepWordNum	=16;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ34 

}
/************************************************************************************************
*����: ��ʼ��F�߹��ϲ��Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRel21FL_F()
{
	Rel21FL_F.pRel21FLCFG				= CFG_pby21_FL;						//���ϲ�������� 
	Rel21FL_F.pRel21FLNSet				= RS_pnF21FLN;                             	       //���ֶ�����ֵ 
	Rel21FL_F.pRel21FL1LSet				= RS_pnF21FLN1L; 						//��1�ξ��붨ֵ 
	Rel21FL_F.pRel21FL1XSet				= RS_pnF21FLN1X;						//��1�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL2LSet				= RS_pnF21FLN2L; 						//��2�ξ��붨ֵ 
	Rel21FL_F.pRel21FL2XSet				= RS_pnF21FLN2X;  					//��2�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL3LSet				= RS_pnF21FLN3L ;  						//��3�ξ��붨ֵ 
	Rel21FL_F.pRel21FL3XSet				= RS_pnF21FLN3X; 						//��3�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL4LSet				= RS_pnF21FLN4L ;  						//��4�ξ��붨ֵ 
	Rel21FL_F.pRel21FL4XSet				= RS_pnF21FLN4X; 						//��4�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL5LSet				= RS_pnF21FLN5L;  						//��5�ξ��붨ֵ 
	Rel21FL_F.pRel21FL5XSet				= RS_pnF21FLN5X; 						//��5�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL6LSet				= RS_pnF21FLN6L; 						//��6�ξ��붨ֵ 
	Rel21FL_F.pRel21FL6XSet				= RS_pnF21FLN6X; 						//��6�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL7LSet				= RS_pnF21FLN7L;						//��7�ξ��붨ֵ 
	Rel21FL_F.pRel21FL7XSet				= RS_pnF21FLN7X; 						//��7�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL8LSet				= RS_pnF21FLN8L; 						//��8�ξ��붨ֵ 
	Rel21FL_F.pRel21FL8XSet				= RS_pnF21FLN8X ;						//��8�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL9LSet				= RS_pnF21FLN9L; 						//��9�ξ��붨ֵ 
	Rel21FL_F.pRel21FL9XSet				= RS_pnF21FLN9X;						//��9�ε�λ�翹��ֵ 
	Rel21FL_F.pRel21FL10LSet			= RS_pnF21FLN10L ; 					//��10�ξ��붨ֵ 
	Rel21FL_F.pRel21FL10XSet			= RS_pnF21FLN10X;					//��10�ξ��붨ֵ 
	Rel21FL_F.pRel21FLS0				= RS_pnS0;
	Rel21FL_F.pRel21FLSD				= RS_pnSD;
	Rel21FL_F.pRel21FLLc				= RS_pnLc;

    Rel21FL_F.pFaultValueL				= RM_pnFL;						//���ϵ���L��ָ��
	Rel21FL_F.pnLCoef					= CF_pnC2;
	Rel21FL_F.FaultValueNum				= RM_FL;
	Rel21FL_F.pSS						 = RM_pnSS;
	Rel21FL_F.SSNO						 = RM_SS;

	
	
	Rel21FL_F.pRel21FLImpd				= &ImpdF;						//��ǰ�迹��ָ�� 
	Rel21FL_F.wExcepWordNum				= 0;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ0 
	
	CalcFaultLocateSetTab(&Rel21FL_F);  								//F�߹��ϲ�ඨָ�����
}
/************************************************************************************************
*����: ��ʼ��T�߹��ϲ��Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRel21FL_T()
{
	Rel21FL_T.pRel21FLCFG				= CFG_pby21_FL;						//���ϲ�������� 
	Rel21FL_T.pRel21FLNSet				= RS_pnT21FLN;                             	       //���ֶ�����ֵ 
	Rel21FL_T.pRel21FL1LSet				= RS_pnT21FLN1L; 						//��1�ξ��붨ֵ 
	Rel21FL_T.pRel21FL1XSet				= RS_pnT21FLN1X;						//��1�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL2LSet				= RS_pnT21FLN2L; 						//��2�ξ��붨ֵ 
	Rel21FL_T.pRel21FL2XSet				= RS_pnT21FLN2X;  					//��2�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL3LSet				= RS_pnT21FLN3L ;  						//��3�ξ��붨ֵ 
	Rel21FL_T.pRel21FL3XSet				= RS_pnT21FLN3X; 						//��3�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL4LSet				= RS_pnT21FLN4L ;  						//��4�ξ��붨ֵ 
	Rel21FL_T.pRel21FL4XSet				= RS_pnT21FLN4X; 						//��4�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL5LSet				= RS_pnT21FLN5L;  						//��5�ξ��붨ֵ 
	Rel21FL_T.pRel21FL5XSet				= RS_pnT21FLN5X; 						//��5�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL6LSet				= RS_pnT21FLN6L; 						//��6�ξ��붨ֵ 
	Rel21FL_T.pRel21FL6XSet				= RS_pnT21FLN6X; 						//��6�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL7LSet				= RS_pnT21FLN7L;						//��7�ξ��붨ֵ 
	Rel21FL_T.pRel21FL7XSet				= RS_pnT21FLN7X; 						//��7�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL8LSet				= RS_pnT21FLN8L; 						//��8�ξ��붨ֵ 
	Rel21FL_T.pRel21FL8XSet				= RS_pnT21FLN8X ;						//��8�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL9LSet				= RS_pnT21FLN9L; 						//��9�ξ��붨ֵ 
	Rel21FL_T.pRel21FL9XSet				= RS_pnT21FLN9X;						//��9�ε�λ�翹��ֵ 
	Rel21FL_T.pRel21FL10LSet			= RS_pnT21FLN10L ; 					//��10�ξ��붨ֵ 
	Rel21FL_T.pRel21FL10XSet			= RS_pnT21FLN10X;					//��10�ξ��붨ֵ 	
	Rel21FL_T.pRel21FLS0				= RS_pnS0;
	Rel21FL_T.pRel21FLSD				= RS_pnSD;
	Rel21FL_T.pRel21FLLc				= RS_pnLc;
	
    Rel21FL_T.pFaultValueL				= RM_pnTL;						//���ϵ���L��ָ�� 
	Rel21FL_T.pnLCoef					= CF_pnC2;                      //����ķŴ���
	Rel21FL_T.FaultValueNum				= RM_TL;
	
	Rel21FL_T.pSS						 = RM_pnSS;
	Rel21FL_T.SSNO						 = RM_SS;
	Rel21FL_T.pRel21FLImpd				= &ImpdT;						//��ǰ�迹��ָ�� 	
	Rel21FL_T.wExcepWordNum				= 0;						//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ0 
	
	CalcFaultLocateSetTab(&Rel21FL_T);//T�߹��ϲ�ඨָ�����
}
/************************************************************************************************
*����: ��ʼ��PT1���߼��Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRelPT1()
{	
	RelPT1.pRelPTBrokCFG 		= CFG_pbyPT1BR;						//PT��������

	RelPT1.pR74PTU1Set			= RS_pn74VTV;								//PT���߼���ѹ��ֵ	
	RelPT2.pR74PTU2Set			= NULL;
	RelPT1.pR74PTISet			= RS_pn74VTC;								//pt���߼�������ֵ
	RelPT1.pR74PTTSet			= RS_pn74VTD;								//pt���߼��ʱ�䶨ֵ
	
	RelPT1.pRelCur				= &Ip;										//�ж��õ���
	RelPT1.pRelU1				= &U1;										//�ж��õ�һ·��ѹ	
	RelPT2.pRelU2				= NULL;

	RelPT1.StartRelayWord		= RW_PIVT1;							//PT1������μ̵�����
	RelPT1.ActRelayWord			= RW_ARVT1;							//PT1���߶����̵�����	
	RelPT1.dwSysTimer			= &dwRelayMSCnt;							//ϵͳ��ʱ��
	RelPT1.wRelRptNum			= R_PT1;									//PT���߶��������1
	RelPT1.wRelResRptNum		= R_PT1_RES;								//PT���߷��ر����1

	RelPT1.wFCounter			= 0;
	//RelPT1.dwRelTimer			= 0;											//PT1���߶�ʱ��
	RelPT1.wExcepWordNum		= 5;											//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6
}
/************************************************************************************************
*����: ��ʼ��PT2���߼��Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void CRelayTask::InitRelPT2()
{	
	RelPT2.pRelPTBrokCFG 		= CFG_pbyPT2BR;						//PT��������

	RelPT2.pR74PTU1Set			= RS_pn74VTV;								//PT���߼���ѹ��ֵ
	RelPT2.pR74PTU2Set			= NULL;
	RelPT2.pR74PTISet			= RS_pn74VTC;								//pt���߼�������ֵ
	RelPT2.pR74PTTSet			= RS_pn74VTD;								//pt���߼��ʱ�䶨ֵ
	
	RelPT2.pRelCur				= &Ip;										//�ж��õ���
	RelPT2.pRelU1				= &U2;		//�ж��õ�һ·��ѹ
	RelPT2.pRelU2				= NULL;

	RelPT2.StartRelayWord		= RW_PIVT2;							//PT1������μ̵�����
	RelPT2.ActRelayWord			= RW_ARVT2;							//PT1���߶����̵�����	
	RelPT2.dwSysTimer			= &dwRelayMSCnt;							//ϵͳ��ʱ��
	RelPT2.wRelRptNum			= R_PT2;									//PT���߶��������1
	RelPT2.wRelResRptNum		= R_PT2_RES;								//PT���߷��ر����2

	RelPT2.wFCounter			= 0;
	//RelPT2.dwRelTimer			= 0;											//PT1���߶�ʱ��
	RelPT2.wExcepWordNum		= 5;											//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6
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
	Rel79.R79RSTSet						= RS_pn79RD;							//�غ�բ��ŵ�ʱ�� 
	Rel79.R79SOTFTSet					= RS_pnSOTFD;						//���ڹ��ϼ����ʱ����Ϊ�����Ͷ��ʱ�� 
	Rel79.R79PISTSet					= AS_pnTCB;						//�غ�բ����������ʱ����Ϊ��·��������ʱ 
	Rel79.R79CLSTSet					= RS_pn79CLSD;						//�غ�բ����������ʱ 

	
	Rel79.R79SCFG						= CFG_pby79UC;	//͵���غ�բ����
	Rel79.R79SW							= SW_pby79;
	Rel79.R79RW							= RW_F79;	//	�����÷����غ�բ�����̵�������

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
	Rel79.wRpt79Fail					= R_R79FAIL;				//�غ�բʧ�ܱ���
	Rel79.wRpt79LK_1					= R_R79LK_1;					//�غ�բ�������������
	
	Rel79.wRpt79LK_2					= R_R79LK_2;					//�غ�բ�������������
	Rel79.wPickUpRptNum					= R_ACTPICK;
	
	Rel79.wRpt79_1_SUCC					= R_R79_1_SUCC;			//һ���غ�բ�ɹ�����  by yanzh 2013��6��18�� 17:31:57
	Rel79.wRpt79_1_FAIL					= R_R79_1_FAIL;			//һ���غ�բʧ�ܱ���
	Rel79.wRpt79_2_SUCC					= R_R79_2_SUCC;			//�����غ�բ�ɹ�����
	Rel79.wRpt79_2_FAIL					= R_R79_2_FAIL;			//�����غ�բʧ�ܱ���
	Rel79.wRpt79PI_1					= R_R79PI_1;				//�غ�բ��������	
	Rel79.wRpt79PI_2					= R_R79PI_2;				//�غ�բ��������
	Rel79.wRpt79NPI_1						= R_R79NPI_1;				//�غ�բδ��������
	Rel79.wRpt79NPI_2						= R_R79NPI_2;				//�غ�բδ��������
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
	//Rel79.R79Timer						= 0;					//�غ�բ������ʱ��ʱ�� 
	Rel79.R79ShotCnt					= 0;					//�غ�բ�Ѿ��������� 	
	Rel79.wExcepWordNum					= 16;				//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ12 

}
/************************************************************************************************
*����: ��ʼ����������Ԫ��
*����: ��
*����: ��
*��ע:
************************************************************************************************/

void  CRelayTask::InitRel50DI()
{
	Rel50DI.pRelCurSet				= RS_pn50DC;                   			 //��������������ֵ 
	Rel50DI.pRelTimeSet				= RS_pn50DD;					//��������ʱ�䶨ֵ 
	Rel50DI.pRelLockCurSet			= RS_pn50D_37C;					//��������С����������ֵ 
	Rel50DI.pRelKh1					= RS_pnH2_PCT;						//����г������ϵ�� 
	Rel50DI.pRelKhr					= RS_pnFACTOR2;						//��������г����Ȩϵ�� 

	Rel50DI.pH2Coef					= CF_pnC2;
	Rel50DI.pRelCFG					= CFG_pby50D;						//������������ 
	Rel50DI.pRelSW					= SW_pby50D;						//����������ѹ�� 


	Rel50DI.pRelElec1				= &Ip;						//��ǰ��������ָ�� 
	Rel50DI.pRelElec2				= &I2;						//��ǰ����г������ָ�� 
	Rel50DI.pRelElec3				= &I3;						//��ǰ����г������ָ�� 
	Rel50DI.pRelElec5				= &I5;						//��ǰ���г������ָ�� 
	Rel50DI.pFaultSINum				= &FaultSINum;					//���ϴ���������������ָ�� 
	Rel50DI.ACTRelayWord			= RW_TR50D;					//�������������̵����� 
	Rel50DI.StartRelayWord			= RW_PI50D;					//����������μ̵����� 
	Rel50DI.LockRelayWord			= RW_LKPI50D;					//��α����̵����� 
	Rel50DI.RSTRelayWord			= RW_LKRST50D;					//���ر����̵����� 
	Rel50DI.LoopCtrlRelayWord		= RW_LPC50D;				//����ѭ�����Ƽ̵����� 	
	Rel50DI.OtherRelTrip			= &m_lOtherRelayTrip;
	Rel50DI.R79ActRelayWord			= RW_79CL;
	Rel50DI.H2LockRelayWord			= RW_LKH2;
	Rel50DI.RptSerialNum			= &RptSerialNumOfAct;
	Rel50DI.pSysTimer				= &dwRelayMSCnt;						//ϵͳ��ʱ�� 
	Rel50DI.wRelRptNum				= wRep50DI;
	//**********************��α��沿��*******************   yanxs  13-03-08
	Rel50DI.wPickUpReportNum		= wRep50DIPick; 					//��α���inf���
	Rel50DI.wResetReportNum 		= wRep50DIRES;						//���ر���inf���
//	Rel50DI.RptPickupSerialNum		= &RptSerialNumOfPick;				//��α����
//	Rel50DI.RptPickupFlag			= &RptFlagOfPickUp;					//��α���������־
	//**********************************************************************
	
	Rel50DI.dwPreI1Mod				= 0;						//����ǰ����ģֵ 
	Rel50DI.dwPreI3Mod				= 0;						//����ǰ����г��ģֵ 
	Rel50DI.dwPreI5Mod				= 0;						//����ǰ���г��ģֵ 
	Rel50DI.PreCurHaveCalcFlag		= FALSE;				//����ǰ�����Ѿ������־ 
	Rel50DI.wRelFCounter			= 0;
	Rel50DI.wRelRCounter			= 0;
	Rel50DI.wExcepWordNum			= 14;
//	Rel50DI.dwRelTimer				= 0;						//Ƿ��������ʱ�� 	

}
//------------------------------------------------------------------------------------------ 
// ����: InitRepSerialNum	 
// ����: ��ʼ�����������������кţ����緽ʽ�Ĳ�ͬ��ͬһ���������Ĺ��ϵ���Ҳ��ͬ 
// ����:                                       
// ���:                                                            
// ����: ��                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRepSerialNum(void)
{
	if(*AS_pnPM == BTMODE)  //ֱ��ģʽ
	{
		wRep211 	= R_R211_D;
		wRep212 	= R_R212_D;
		wRep213		= R_R213_D;
		wRep214		= R_R214_D;
		wRep50I0 	= R_R50I0_D ;
		wRep50I1 	= R_R50I1_D;
		wRep50I2 	= R_R50I2_D;
		wRep50I3 	= R_R50I3_D;
		wRep51I 	= R_R51I_D;
		wRep50DI 	= R_R50DI_D;
		wRepR212Acc = R_R212_94_D;
		wRepR213Acc = R_R213_94_D;
		wRepR214Acc = R_R214_94_D;
		wRep50I1Acc	= R_R50I1_94_D;
		wRep50I2Acc	= R_R50I2_94_D;
		wRep50I3Acc	= R_R50I3_94_D;
		wRep27U		= R_R27U_D;
//************************��α��沿��*************************   yanxs  13-02-05
		wRep211Pick  = R_R211_D_PI;
		wRep212Pick  = R_R212_D_PI;
		wRep213Pick  = R_R213_D_PI;
		wRep214Pick  = R_R214_D_PI;
		wRep50I0Pick = R_R50I0_D_PI;
		wRep50I1Pick = R_R50I1_D_PI;
		wRep50I2Pick = R_R50I2_D_PI;
		wRep50I3Pick = R_R50I3_D_PI;
		wRep51IPick  = R_R51I_D_PI;
		wRep50DIPick = R_R50DI_D_PI;
		wRepR212AccPick = R_R212_94_D_PI;
		wRepR213AccPick = R_R213_94_D_PI;
		wRepR214AccPick = R_R214_94_D_PI;
		wRep50I1AccPick	= R_R50I1_94_D_PI;
		wRep50I2AccPick	= R_R50I2_94_D_PI;
		wRep50I3AccPick	= R_R50I3_94_D_PI;
//**************************�������ر���***********************   yanxs  13-06-20
		wRep211RES 	= R_R211_D_RES;
		wRep212RES 	= R_R212_D_RES;
		wRep213RES 	= R_R213_D_RES;
		wRep214RES 	= R_R214_D_RES;
		wRep50I0RES	= R_R50I0_D_RES ;
		wRep50I1RES	= R_R50I1_D_RES;
		wRep50I2RES	= R_R50I2_D_RES;
		wRep50I3RES	= R_R50I3_D_RES;
		wRep51IRES	= R_R51I_D_RES;
		wRep50DIRES	= R_R50DI_D_RES;
		wRepR212AccRES = R_R212_94_D_RES;
		wRepR213AccRES = R_R213_94_D_RES;
		wRepR214AccRES = R_R214_94_D_RES;
		wRep50I1AccRES = R_R50I1_94_D_RES;
		wRep50I2AccRES = R_R50I2_94_D_RES;
		wRep50I3AccRES = R_R50I3_94_D_RES;
	}
	else if(*AS_pnPM == AT1MODE) //AT1���緽ʽ
	{		
		wRep211 	= R_R211_S;
		wRep212 	= R_R212_S;
		wRep213 	= R_R213_S;
		wRep214 	= R_R214_S;
		wRep50I0	= R_R50I0_S ;
		wRep50I1	= R_R50I1_S;
		wRep50I2	= R_R50I2_S;
		wRep50I3	= R_R50I3_S;
		wRep51I 	= R_R51I_S;
		wRep50DI	= R_R50DI_S;
		wRepR212Acc = R_R212_94_S;
		wRepR213Acc = R_R213_94_S;
		wRepR214Acc = R_R214_94_S;
		wRep50I1Acc = R_R50I1_94_S;
		wRep50I2Acc = R_R50I2_94_S;
		wRep50I3Acc = R_R50I3_94_S;
		wRep27U 	= R_R27U_S;
		wRep211Pick  = R_R211_S_PI;
		wRep212Pick  = R_R212_S_PI;
		wRep213Pick  = R_R213_S_PI;
		wRep214Pick  = R_R214_S_PI;
		wRep50I0Pick = R_R50I0_S_PI;
		wRep50I1Pick = R_R50I1_S_PI;
		wRep50I2Pick = R_R50I2_S_PI;
		wRep50I3Pick = R_R50I3_S_PI;
		wRep51IPick  = R_R51I_S_PI;
		wRep50DIPick = R_R50DI_S_PI;
		wRepR212AccPick = R_R212_94_S_PI;
		wRepR213AccPick = R_R213_94_S_PI;
		wRepR214AccPick = R_R214_94_S_PI;
		wRep50I1AccPick = R_R50I1_94_S_PI;
		wRep50I2AccPick = R_R50I2_94_S_PI;
		wRep50I3AccPick = R_R50I3_94_S_PI;
		wRep211RES	= R_R211_S_RES;
		wRep212RES	= R_R212_S_RES;
		wRep213RES	= R_R213_S_RES;
		wRep214RES	= R_R214_S_RES;
		wRep50I0RES = R_R50I0_S_RES ;
		wRep50I1RES = R_R50I1_S_RES;
		wRep50I2RES = R_R50I2_S_RES;
		wRep50I3RES = R_R50I3_S_RES;
		wRep51IRES	= R_R51I_S_RES;
		wRep50DIRES = R_R50DI_S_RES;
		wRepR212AccRES = R_R212_94_S_RES;
		wRepR213AccRES = R_R213_94_S_RES;
		wRepR214AccRES = R_R214_94_S_RES;
		wRep50I1AccRES = R_R50I1_94_S_RES;
		wRep50I2AccRES = R_R50I2_94_S_RES;
		wRep50I3AccRES = R_R50I3_94_S_RES;
	}
	else  //AT���緽ʽ��������ͨAT�Ϳ�רAT����ģʽ
	{		
		wRep211 	= R_R211;
		wRep212 	= R_R212;
		wRep213		= R_R213;
		wRep214		= R_R214;
		wRep50I0	= R_R50I0;
		wRep50I1 	= R_R50I1;
		wRep50I2 	= R_R50I2;
		wRep50I3 	= R_R50I3;
		wRep51I 	= R_R51I;
		wRep50DI 	= R_R50DI;
		wRepR212Acc = R_R212_94;
		wRepR213Acc = R_R213_94;
		wRepR214Acc = R_R214_94;
		wRep50I1Acc	= R_R50I1_94;
		wRep50I2Acc	= R_R50I2_94;
		wRep50I3Acc	= R_R50I3_94;
		wRep27U		= R_R27TR;
//**************************��α��沿��************  yanxs  13-02-05
		wRep211Pick  = R_R211_PI;
		wRep212Pick  = R_R212_PI;
		wRep213Pick  = R_R213_PI;
		wRep214Pick  = R_R214_PI;
		wRep50I0Pick = R_R50I0_PI;
		wRep50I1Pick = R_R50I1_PI;
		wRep50I2Pick = R_R50I2_PI;
		wRep50I3Pick = R_R50I3_PI;
		wRep51IPick  = R_R51I_PI;
		wRep50DIPick = R_R50DI_PI;
		wRepR212AccPick = R_R212_94_PI;
		wRepR213AccPick = R_R213_94_PI;
		wRepR214AccPick = R_R214_94_PI;
		wRep50I1AccPick	= R_R50I1_94_PI;
		wRep50I2AccPick	= R_R50I2_94_PI;
		wRep50I3AccPick	= R_R50I3_94_PI;
		wRep211RES	= R_R211_RES;
		wRep212RES	= R_R212_RES;
		wRep213RES	= R_R213_RES;
		wRep214RES	= R_R214_RES;
		wRep50I0RES = R_R50I0_RES;
		wRep50I1RES = R_R50I1_RES;
		wRep50I2RES = R_R50I2_RES;
		wRep50I3RES = R_R50I3_RES;
		wRep51IRES	= R_R51I_RES;
		wRep50DIRES = R_R50DI_RES;
		wRepR212AccRES = R_R212_94_RES;
		wRepR213AccRES = R_R213_94_RES;
		wRepR214AccRES = R_R214_94_RES;
		wRep50I1AccRES = R_R50I1_94_RES;
		wRep50I2AccRES = R_R50I2_94_RES;
		wRep50I3AccRES = R_R50I3_94_RES;
//		wRep27UPick  = R_R27U_PICK;
//****************************************************
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
	InitRepSerialNum();
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

/*------------------------------------------------------------------------------------------
����: FaultLocate	
���ܣ����ϲ�ඨֵ����                                                             
����: ���ϲ�ඨֵ��                                        
����:                                                         
��ע: ��                                                                                                                                                             
------------------------------------------------------------------------------------------ */
void CRelayTask::CalcFaultLocateSetTab(TRelFaultLocatePar *Relay)
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
/*------------------------------------------------------------------------------------------
����: FaultLocate	
���ܣ��翹�����ϲ��                                                             
����:                                         
����:                                                         
��ע: ��                                                                                                                                                             
------------------------------------------------------------------------------------------ */

void CRelayTask::FaultLocate(TRelFaultLocatePar *Relay)
{
	WORD num = 0;
	DWORD React = ToAbs(Relay->pRel21FLImpd->Imag)*(*Relay->pnLCoef);

	if ((*Relay->pRel21FLCFG) == CFG_STATUS_CLR)
	{
		*Relay->pFaultValueL= 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	if ((*(Relay->pRel21FLNSet)>MAXFLNUM) || (*(Relay->pRel21FLNSet)<=0))
	{
		*Relay->pFaultValueL= 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	for (;num<(*(Relay->pRel21FLNSet));num++)     //�жϹ��ϵ������ĸ����� 
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
	if(*Relay->pRel21FLSD == FOREWORD)
		*Relay->pSS = (*Relay->pRel21FLS0) + (*Relay->pFaultValueL)-(*Relay->pRel21FLLc);
	else
		*Relay->pSS = (*Relay->pRel21FLS0) - (*Relay->pFaultValueL)+(*Relay->pRel21FLLc);

	
	DB_LogWriteRM(Relay->SSNO, *Relay->pSS);

}

//------------------------------------------------------------------------------------------ 
// ����: ģֵ�����б����							                                 
// ����: ��                                                                                  
// ���:                                                              
// ����: ��                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	BYTE  pChannel[]={CHANNEL_6520_IP};
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
	if(((*Rel50I1.pRelCFG == CFG_STATUS_SET) ||(*Rel50I1.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I1.pRelSW == SW_STATUS_SET)) //����1��Ͷ�� 
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
	if(((*Rel50I2.pRelCFG == CFG_STATUS_SET) ||(*Rel50I2.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I2.pRelSW == SW_STATUS_SET)) //����2��Ͷ�� 
	{
		if(Rel50I2.pRelElec->Mod > *Rel50I2.pElecSet)
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
	if(((*Rel50I3.pRelCFG == CFG_STATUS_SET)||(*Rel50I3.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I3.pRelSW == SW_STATUS_SET)) //����3��Ͷ�� 
	{
		if(Rel50I3.pRelElec->Mod > *Rel50I3.pElecSet)
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
	if((*Rel51I.pRelCFG == CFG_STATUS_SET) && (*Rel51I.pRelSW == SW_STATUS_SET)) //��ʱ�ޱ���Ͷ�� 
	{
		if(Rel51I.pRelElec->Mod > *Rel51I.pRelCurSet*11/10)
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
	//ͻ������������
	if(((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET)) || \
		(((*Rel50I1.pRelCFG == CFG_STATUS_SET) ||(*Rel50I1.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I1.pRelSW == SW_STATUS_SET)) || \
		(((*Rel50I2.pRelCFG == CFG_STATUS_SET) ||(*Rel50I2.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I2.pRelSW == SW_STATUS_SET)) || \
		(((*Rel50I3.pRelCFG == CFG_STATUS_SET) ||(*Rel50I3.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I3.pRelSW == SW_STATUS_SET)) || \
		((*Rel51I.pRelCFG == CFG_STATUS_SET) && (*Rel51I.pRelSW == SW_STATUS_SET))  || \
		((*Rel211.pRel21CFG == CFG_STATUS_SET) && (*Rel211.pRel21SW == SW_STATUS_SET)) || \
		((*Rel212.pRel21CFG == CFG_STATUS_SET) && (*Rel212.pRel21SW == SW_STATUS_SET)) || \
		((*Rel213.pRel21CFG == CFG_STATUS_SET) && (*Rel213.pRel21SW == SW_STATUS_SET)) || \
		((*Rel214.pRel21CFG == CFG_STATUS_SET) && (*Rel214.pRel21SW == SW_STATUS_SET)) ||
		((*Rel50DI.pRelCFG == CFG_STATUS_SET)&&(*Rel50DI.pRelSW== SW_STATUS_SET)))
	
	{
		PickCheck = 0;
		ReadPreSingleData(dwPickDataBuf,pChannel,1,RELAY_SAMP_INTERVAL);

		//�㷨1�����ݺ�RELAY_SAMP_INTERVALȷ����ͨ���㷨
		//��3���ܲ��Ĳ�����ֱ�ȡ����Ӧ������,������ͻ����,�ҽ����ж�
		//ע��:RELAY_SAMP_INTERVAL��ֵ����С��3
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i];
			lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i];
			lSampDate3 = dwPickDataBuf[i];
			lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));
			
//			if(lPickValue>*RS_pnPIC)				
			if(lPickValue*4>*RS_pnPIC*4+m_lPickUpValue)
			{
				PickCheck++;
				if(PickCheck ==3)
				{					
					QWORD PickTime; 					
					ReadAbsTime(&PickUpTime);
					PickTime = PickUpTime.Hi;
					PickTime <<= 32;
					PickTime += PickUpTime.Lo-(i*625+500)/1000;
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
						PickTime = PickUpTime.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime.Lo-(i*625+500)/1000;
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
				if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
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
	if(!FaultOnFlag)   						//��δ������ϴ���
	{		
		RelayPickUpCHK();					//ִ���������
		if(ReadRelayWord(PickUpRW))
		{
			FaultSINum = 0;       //������������ʼ�����ϴ���������������
			FaultOnFlag=TRUE;
			ResetAllRelay(TRUE);
			//������ϴ���������PT�����ж�
			if(!ReadRelayWord(RelPT1.ActRelayWord))
				WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);
			if(!ReadRelayWord(RelPT2.ActRelayWord))
				WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);	
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//���ϴ����������������ۼ�
	if(FaultSINum<READ_POINT_NUM)					//���ϴ���������������С��һ�ܲ���������
	{
		//����������������������
		CalcInCreCurrent(&Rel50DI,dwIncreCurBuf,dwInCreDataMask);
		return;
	}	
	else
	{
		if(!HaveMakeStartRptFlag)   //������ϵ���������������д����ϵ���
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}

		//���ù��ϲ����򣬼�����ϵ����
		if(*AS_pnPM == BTMODE)
			FaultLocate(&Rel21FL_T);
		if(*AS_pnPM == AT2MODE)
		{
			FaultLocate(&Rel21FL_T);
			FaultLocate(&Rel21FL_F);			
		}
		
		if((!ReadRelayWord(RW_37IP))&&(I2.Mod*(*CF_pnC2)>(*RS_pnH2_PCT)*Ip.Mod))
		{				
			WriteRelayWord(RW_LKH2, RELAY_ON);	//�ö���г�������̵�����	
		}
		else
		{
			WriteRelayWord(RW_LKH2, RELAY_OFF);
		}
		
		if(ReadRelayWord(RW_SL21))
		{
			if(ReadRelayWord(RW_F21_1)||ReadRelayWord(RW_F21_2)||ReadRelayWord(RW_F21_3)||ReadRelayWord(RW_F21_4))
			{
				if((!ReadRelayWord(RW_F74VT1)&&((*AS_pnPM == AT1MODE)||(*AS_pnPM == BTMODE))) \
					||(!ReadRelayWord(RW_F74VT1)&&!ReadRelayWord(RW_F74VT2)&&(*AS_pnPM == AT2MODE)))  //PT����δͶ��
				{
					GeneralRelay21(&Rel211);						//���þ���I�α����ӳ���
					GeneralRelay21(&Rel212);						//���þ���II�α����ӳ���		
					GeneralRelay21(&Rel213);						//���þ���III�α����ӳ���
					GeneralRelay21(&Rel214);						//���þ���IV�α����ӳ���
				}
				else
				{	
					if(Ip.Mod>*RS_pn74VTC)
					{							
						DWORD tempzt,tempzf;
						if (*AS_pnPM == AT2MODE)
						{
							tempzt = ToAbs(ImpdT.Imag) + ToAbs(ImpdT.Real);    // |R|+|X|<1ŷķ
							tempzf = ToAbs(ImpdF.Imag) + ToAbs(ImpdF.Real);    // |R|+|X|<1ŷķ
						}
						else
						{
							tempzt = ToAbs(Impd.Imag) + ToAbs(Impd.Real);   // |R|+|X|<1ŷķ
							tempzf = tempzt;
						}
						
						if((Ip.Mod<*RS_pn74VTBLC)&&(tempzt < (*CF_pnZ)||tempzf<(*CF_pnZ)))
							m_bOneOhmPTLK=TRUE;
						else
							m_bOneOhmPTLK=FALSE;
					}
					else
					{
						m_bOneOhmPTLK=FALSE;
					}
					if(m_bOneOhmPTLK)//���г���PT������Ϊ���뱣������α����̵����ֽ��б���������������PT���߱������뱣������
					{
						if(!m_bIsHaveMakePTRpt)	//��PT���߱������뱣������
						{
							MakeAlarmRpt(R_PTLK);
							m_bIsHaveMakePTRpt= TRUE;							
						}						
					}
					else
					{
						GeneralRelay21(&Rel211);						//���þ���I�α����ӳ���
						GeneralRelay21(&Rel212);						//���þ���II�α����ӳ���		
						GeneralRelay21(&Rel213);						//���þ���III�α����ӳ���
						GeneralRelay21(&Rel214);						//���þ���IV�α����ӳ���
					}
				}
			}	
			else
			{
				if(ReadRelayWord(RW_F21_2_94)||ReadRelayWord(RW_F21_3_94)||ReadRelayWord(RW_F21_4_94))
				{						
					GeneralRelay21(&Rel212);						//���þ���II�α����ӳ���		
					GeneralRelay21(&Rel213);						//���þ���III�α����ӳ���
					GeneralRelay21(&Rel214);						//���þ���IV�α����ӳ���
				}
			}
		}

		
		if((*Rel50I0.pRelPDCFG ==CFG_STATUS_SET)||(*Rel50I1.pRelPDCFG ==CFG_STATUS_SET)||(*Rel50I2.pRelPDCFG ==CFG_STATUS_SET)     //���ʷ����ж�
			||(*Rel50I3.pRelPDCFG ==CFG_STATUS_SET))
		{
			if(!m_bPDCheck)   //δ���������书�ʷ����ж�
			{
				if((!ReadRelayWord(RW_ARVT1))&&(!ReadRelayWord(RW_ARVT2)))		//2013-04-27   PT��������£����ʷ����������ʧЧ
				{
					m_bPDCheck = PowerDirCheck(&Up,&Ip,dwPDDataBuf,*RS_pnZANGLE,*CF_pnUP,RW_32F,RW_32R,CHANNEL_6520_UP,TRUE);
				}
				else
				{				
					WriteRelayWord(RW_32R, RELAY_OFF);
					WriteRelayWord(RW_32F, RELAY_OFF);
				}
			}
		}
		
		GeneralOverValueRelay(&Rel50I0);				//�����ٶ��ӳ���
		GeneralOverValueRelay(&Rel50I1);				//���ù���I�α����ӳ���
		GeneralOverValueRelay(&Rel50I2);				//���ù���III�α����ӳ���
		GeneralOverValueRelay(&Rel50I3);				//���ù���III�α����ӳ���
		InverseCurRelay(&Rel51I);						//���÷�ʱ�ޱ����ӳ���
		
		if(ReadRelayWord(RW_TR50_0)||ReadRelayWord(RW_TR50_1)||ReadRelayWord(RW_TR50_1_94)||ReadRelayWord(RW_TR50_2)||
			ReadRelayWord(RW_TR50_2_94)||ReadRelayWord(RW_TR50_3)||ReadRelayWord(RW_TR50_3_94)||ReadRelayWord(RW_TR21_1)||
			ReadRelayWord(RW_TR21_2)||ReadRelayWord(RW_TR21_2_94)||ReadRelayWord(RW_TR21_3)||ReadRelayWord(RW_TR21_3_94)||
			ReadRelayWord(RW_TR21_4)||ReadRelayWord(RW_TR21_4_94)||ReadRelayWord(RW_TR51)||ReadRelayWord(RW_TR27))
		{
			m_lOtherRelayTrip = TRUE;  //������������֮�⣬��������������־�����ж����������������ء�
		}		
		IncreCurrentRelay(&Rel50DI);					//���õ������������ӳ���
		

		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveTripFailedRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//����·����բʧ�ܱ���
				m_bHaveTripFailedRpt = TRUE;
			}
		}

		if((Rel79.R79TFLAG)||(Rel79.R79PISFLAG))					//�غ�բ�Ѿ�������ʱ״̬,�ȴ��غ�բ����
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
				if(!ReadRelayWord(RW_37IP))   				//��������̵������Ƿ���λ
				{
					m_lOtherRelayTrip = FALSE;  //������������֮�⣬��������������־�����ж����������������ء�
					FaultSINum = 0;		//���³�ʼ�����ϴ���������������
					CurRecoverFlag = TRUE;    				//�õ����ָ���־
					return;
				}				
				return;
			}
		}
		
		// 1���б���ѭ�����Ƽ̵����ֱ���λ���ȴ��������������˳�
		if(ReadRelayWord(Rel211.LoopCtrlRelayWord)||ReadRelayWord(Rel212.LoopCtrlRelayWord)||ReadRelayWord(Rel213.LoopCtrlRelayWord)
			||ReadRelayWord(Rel214.LoopCtrlRelayWord)||ReadRelayWord(Rel50I0.LoopCtrlRelayWord)||ReadRelayWord(Rel50I1.LoopCtrlRelayWord) 
			||ReadRelayWord(Rel50I2.LoopCtrlRelayWord)||ReadRelayWord(Rel50I3.LoopCtrlRelayWord)||ReadRelayWord(Rel51I.LoopCtrlRelayWord)
			||ReadRelayWord(Rel50DI.LoopCtrlRelayWord))
		{
			// 1.1 �����Ѿ�����������բʧ��
			if(ReadRelayWord(RW_52BF))
			{
				WriteRelayWord(PickUpRW, RELAY_OFF);		//�屣�������̵�����
				FaultCheckOutFlag = FALSE;					//�������ϱ�־
				FaultOnFlag = FALSE;						//�����ѭ�������־
				HaveMakeStartRptFlag= FALSE;				//�����������ϵ������������־
				m_bIsHaveMakePTRpt = FALSE;					//������PT���߱������뱣���澯�����־				
				m_bFaultLKFlag = FALSE; 					//���������޶���ʱ����ʱ��������־	
				m_bPDCheck = FALSE;							//�����������书�ʷ����жϱ�־
				CurRecoverFlag = FALSE;						//���غ�բ����������ָ���־
				DB_RelayEnd(RptSerialNumOfAct); 			//�����ݿ���д���������	
				m_bHaveRelayAct = FALSE;
				m_lOtherRelayTrip = FALSE;
				m_bHaveTripFailedRpt = FALSE;
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
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime > 1000 )  //�޹���ʱ�䳬�����ڹ��ϼ��ʱ��
				{					
					WriteRelayWord(PickUpRW, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־
					CurRecoverFlag	 = FALSE;					//���غ�բ����������ָ���־		
					m_bIsHaveMakePTRpt = FALSE;					//������PT���߱������뱣���澯�����־					
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					m_bPDCheck = FALSE;							//�����������书�ʷ����жϱ�־
					m_bHaveRelayAct = FALSE;
					m_lOtherRelayTrip = FALSE;					
					m_bR79PickUpRpt = FALSE;					
					DB_RelayEnd(RptSerialNumOfAct);				//�����ݿ���д���������
					ResetAllRelay(TRUE);
					m_bHaveTripFailedRpt = FALSE;

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
					WriteRelayWord(PickUpRW, RELAY_OFF);		//�屣�������̵�����
					FaultCheckOutFlag = FALSE;					//�������ϱ�־
					FaultOnFlag = FALSE;						//�����ѭ�������־
					HaveMakeStartRptFlag = FALSE;				//�����������ϵ������������־						
					CurRecoverFlag	 = FALSE;					//���غ�բ����������ָ���־		
					m_bFaultLKFlag = FALSE;						//���������޶���ʱ����ʱ��������־	
					m_bIsHaveMakePTRpt = FALSE;					//������PT���߱������뱣���澯�����־
					m_bPDCheck = FALSE;							//�����������书�ʷ����жϱ�־
					DB_RelayEnd(RptSerialNumOfAct); 			
					m_bHaveRelayAct = FALSE;
					m_lOtherRelayTrip = FALSE;					
					m_bR79PickUpRpt = FALSE;
					ResetAllRelay(TRUE);	
					m_bHaveTripFailedRpt = FALSE;
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
			ResetUnderValRelay(&Rel27U);
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
/*	if(ReadRelayWord(RW_52BF))			//��բʧ�̵ܼ�������λ
	{
		TripFailFlag=TRUE;					//����բʧ�ܱ�־
		return;
	}
	else									//����բʧ�̵ܼ�������λ������բʧ�ܱ�־�����鸴�飬����ȡ����բʧ�̵ܼ����ֵ��½���
	{
		if(TripFailFlag)
		{
*/			ResetRelay21(&Rel211,resetFlag);
			ResetRelay21(&Rel212,resetFlag);
			ResetRelay21(&Rel213,resetFlag);
			ResetRelay21(&Rel214,resetFlag);
			
			ResetOverValueRelay(&Rel50I0,resetFlag);
			ResetOverValueRelay(&Rel50I1,resetFlag);
			ResetOverValueRelay(&Rel50I2,resetFlag);
			ResetOverValueRelay(&Rel50I3,resetFlag);
			ResetUnderValRelay(&Rel27U,resetFlag);
			ResetInverseCurRel(&Rel51I,resetFlag);
			ResetR50DI(&Rel50DI,resetFlag);
//			if (RptFlagOfPickUp == TRUE)
//			{
//				DB_RelayEnd(RptSerialNumOfPick); 			//�����ݿ���д���������
//				RptFlagOfPickUp = FALSE;
//			}
//			TripFailFlag=FALSE;
//		}		
//	}	
}
void CRelayTask::PTCheck()
{	
	if(*RelPT1.pRelPTBrokCFG == CFG_STATUS_SET)
	{
		if(RelPT1.pRelU1->Mod < *RelPT1.pR74PTU1Set)  //U1С�ڶ�ֵ
		{
			if(RelPT1.pRelCur->Mod > *RelPT1.pR74PTISet)   //�����������ж�
			{				
				if(!ReadRelayWord(RelPT1.ActRelayWord))		//��δ����
				{
					if(!ReadRelayWord(RelPT1.StartRelayWord))		//��δ���
					{
						RelPT1.wFCounter++;
						if(RelPT1.wFCounter>2)
						{
							RelPT1.dwRelTimer = *RelPT1.dwSysTimer;
							WriteRelayWord(RelPT1.StartRelayWord, RELAY_ON);
						}
					}
					else
					{
						if((*RelPT1.dwSysTimer - RelPT1.dwRelTimer) > *RelPT1.pR74PTTSet)
						{
							WriteRelayWord(RelPT1.ActRelayWord, RELAY_ON);
							MakeAlarmRpt(RelPT1.wRelRptNum);
						}
					}
				}
			}
			else
			{				
				RelPT1.dwRelTimer  =0;
				WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);				
			}
		}
		else   //PT1���߻ָ�
		{
			RelPT1.dwRelTimer = 0;
			WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);
			if(ReadRelayWord(RelPT1.ActRelayWord))		//�Ѿ�����	
			{
				MakeAlarmRpt(RelPT1.wRelResRptNum);
				WriteRelayWord(RelPT1.ActRelayWord, RELAY_OFF);
			}
		}
	}
	else
	{
		RelPT1.dwRelTimer  =0;
		if(ReadRelayWord(RelPT1.ActRelayWord))		//�Ѿ�����	
		{
			MakeAlarmRpt(RelPT1.wRelResRptNum);
			WriteRelayWord(RelPT1.ActRelayWord, RELAY_OFF);
		}
		WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);
	}

	
	if((*RelPT2.pRelPTBrokCFG == CFG_STATUS_SET))
	{
		if(RelPT2.pRelU1->Mod < *RelPT2.pR74PTU1Set)  //U2С�ڶ�ֵ
		{
			if(RelPT2.pRelCur->Mod > *RelPT2.pR74PTISet)   //�����������ж�
			{				
				if(!ReadRelayWord(RelPT2.ActRelayWord))		//��δ����
				{
					if(!ReadRelayWord(RelPT2.StartRelayWord))		//��δ���
					{
						RelPT2.wFCounter++;
						if(RelPT2.wFCounter > 2)
						{
							RelPT2.dwRelTimer = *RelPT2.dwSysTimer;
							WriteRelayWord(RelPT2.StartRelayWord, RELAY_ON);
						}
					}
					else 
					{
						if((*RelPT2.dwSysTimer - RelPT2.dwRelTimer) > *RelPT2.pR74PTTSet)
						{
							WriteRelayWord(RelPT2.ActRelayWord, RELAY_ON);
							MakeAlarmRpt(RelPT2.wRelRptNum);
						}
					}
				}		
			}
			else
			{
				RelPT2.dwRelTimer  =0;
				WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);
			}
		}
		else 		//PT2���߻ָ�
		{
			RelPT2.dwRelTimer = 0;
			WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);
			if(ReadRelayWord(RelPT2.ActRelayWord))		//�Ѿ�����
			{
				MakeAlarmRpt(RelPT2.wRelResRptNum);
				WriteRelayWord(RelPT2.ActRelayWord, RELAY_OFF);		
			}
		}
	}
	else			//PT2���߻ָ�
	{
		RelPT2.dwRelTimer  =0;		
		if(ReadRelayWord(RelPT2.ActRelayWord))		//�Ѿ�����
		{
			MakeAlarmRpt(RelPT2.wRelResRptNum);
			WriteRelayWord(RelPT2.ActRelayWord, RELAY_OFF);		
		}
		WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);
	}

}


