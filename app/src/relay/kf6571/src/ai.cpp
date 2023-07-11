/*******************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
*******************************************************************************************/ 
                          
/*******************************************************************************************
*                                                                                  
* �ļ�����          
*			ai.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           		ң������                                                                      
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
*      yanzh             09/08/20    ��ʼ����                         
*                                                                                  
*******************************************************************************************/
#include "ai.h"
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
#include "etpu_app.h"
#include "etpu_cfg.h"
#include "hmiapp.h"

DWORD dwAITaskID;
extern DWORD g_hLCDTaskID;
extern DWORD g_dwFreqPWM;
extern TSysConfigTable	*G_pCfgTable;

BEGIN_MESSAGE_MAP(CAIProc,CApp)
	ON_SM_TIMEOUT(CAIProc)
	ON_SM_AUTOADJUST(CAIProc)
END_MESSAGE_MAP()

/*********************************************************************************************
 * ����: AI������ں���
 * ����: pdwApp	
 * ����: ��
 * ��ע: ��        
**********************************************************************************************/

void AITask(DWORD *pdwApp)
{
	CAIProc *pAITask = new CAIProc(pdwApp);
	pAITask->Init();
	pAITask->Run();
}	
/*********************************************************************************
*����: ң���ʼ���ӳ���
*����: ��
*����: ��
*��ע: ��
**************************************************************************************/

void CAIProc::Init(void)
{
	
	m_CoefAdjustDataMask =( (0x01<<CHANNEL_6571_U1)|(0x01<<CHANNEL_6571_U2)|(0x01<<CHANNEL_6571_U3)|(0x01<<CHANNEL_6571_U4) \
							|(0x01<<CHANNEL_6571_IT1)|(0x01<<CHANNEL_6571_IF1)|(0x01<<CHANNEL_6571_IT2)|(0x01<<CHANNEL_6571_IF2) \
							|(0x01<<CHANNEL_6571_IT3)|(0x01<<CHANNEL_6571_IF3)|(0x01<<CHANNEL_6571_IAT1)|(0x01<<CHANNEL_6571_IAT2));
	m_pCoefAdjustBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);
	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);

	for(LONG i=0;i<28;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		//ϵ��У������		
		AdjustU1[i] = 0;
		AdjustU2[i] = 0;
		AdjustU3[i] = 0;
		AdjustU4[i]  = 0;
		AdjustIT1[i] = 0;
		AdjustIF1[i] = 0;
		AdjustIT2[i] = 0;
		AdjustIF2[i] = 0;
		AdjustIT3[i] = 0;
		AdjustIF3[i] = 0;
		AdjustIAT1[i] = 0;
		AdjustIAT2[i] = 0;
	}
	//��ʼ��ϵ��У׼��־
	m_bIsCoefAdjust = FALSE;
	m_bIsSourceOK =FALSE;
	m_dwAdjustCnt=0;
	dwAITaskID = GetTaskId();
	//����AI��ʱ��
	SetTimer(1,AI_TIMER_PERIOD);
}
/*********************************************************************************
*����: AI��ʱ��Ϣ��Ӧ
*����: ��ʱ��ID
*���: ��
*����: ��
**************************************************************************************/

void CAIProc::OnTimeOut(DWORD id)
{
	static WORD freqcounter =0;
	//��ʼϵ��У׼
	if(m_bIsCoefAdjust == TRUE)
	{
		CoefAdjust();	
		return;
	}
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
}
void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//��������У����־

	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	dwMsgID = arg;
	//�����Զ�У׼��־
	m_bIsCoefAdjust = TRUE;	
	//��ʼ��ԴУ����־
	m_bIsSourceOK = FALSE;
	//У���ȴ���������0
	m_dwWaitCnt = 0;

	midU1 = 0;
	midU2 = 0;
	midU3 = 0;
	midU4 = 0;
	midIT1 = 0;
	midIF1 = 0;
	midIT2 = 0;
	midIF2 = 0;
	midIT3 = 0;
	midIF3 = 0;
	midIAT1 = 0;
	midIAT2 = 0;

	for(LONG i=0;i<12;i++)
		m_pAdjustData[i]= 0;

}
void CAIProc::CoefAdjustCalu(void)//����ϵ��У���еļ���
{	
	ReadSampleData(m_pCoefAdjustBuf,m_CoefAdjustDataMask,0);		//��ȡԭʼ��������	
	CaluBaseFourier_20(m_pCoefAdjustBuf,&U1);
	CaluModValue(&U1);	
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*2,&U3);
	CaluModValue(&U3);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*3,&U4);
	CaluModValue(&U4);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*4,&IT1);
	CaluModValue(&IT1);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*5,&IF1);
	CaluModValue(&IF1);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*6,&IT2);
	CaluModValue(&IT2);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*7,&IF2);
	CaluModValue(&IF2);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*8,&IT3);
	CaluModValue(&IT3);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*9,&IF3);
	CaluModValue(&IF3);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*10,&IAT1);
	CaluModValue(&IAT1);
	CaluBaseFourier_20(m_pCoefAdjustBuf+READ_POINT_NUM*11,&IAT2);
	CaluModValue(&IAT2);
}

/************************************************************************************************
*����: ϵ��У��
*����: �˲������ݻ�����
*����: ��
*��ע:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	// ϵ��У��100�ν���
	if(m_dwAdjustCnt>99)
	{
		LONG Value[17];
		
		Value[0]	= m_pAdjustData[0]/(m_dwSourceU*100);     			//UHA
		Value[1]	= m_pAdjustData[1]/(m_dwSourceU*100);					//UHB
		Value[2]	= m_pAdjustData[2]/(m_dwSourceU*100);					//UHC
		Value[3]	= m_pAdjustData[3]/(m_dwSourceU*100);					//U0
		Value[4]	= m_pAdjustData[4]/(100*m_dwSourceI);					//IHA
		Value[5]	= m_pAdjustData[5]/(100*m_dwSourceI);				//IHB
		Value[6]	= m_pAdjustData[6]/(100*m_dwSourceI);					//IHC
		Value[7]	= m_pAdjustData[7]/(100*m_dwSourceI);				//ILA
		Value[8]	= m_pAdjustData[8]/(100*m_dwSourceI);				//ILB
		Value[9]	= m_pAdjustData[9]/(100*m_dwSourceI);			    //ILC
		Value[10]	= m_pAdjustData[10]/(100*m_dwSourceI);				//I1
		Value[11]	= m_pAdjustData[11]/(100*m_dwSourceI);				//I2
		Value[12]	= Value[0]*IMPDMULTIPLE/(Value[4]);;				//PA
		Value[13]	= 1000;
		Value[14]	= 100;
		Value[15]	= 10;
		Value[16]	= 1;
		
		DB_SetCFValue(CF_VALUE_ALL, Value);				//д��ϵ��ֵ
		DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);	//д��ϵ��ȷ��
		CoefAdjustEnd(SUCCESS);
		return;	
	}
// ϵ��У���еĵ�������
	CoefAdjustCalu();
	
// ���ȿ�ʼԴ�ж�,��У���ǲ�
	if(m_bIsSourceOK == FALSE)
	{
		if((U1.Mod>(*CF_pnU1*m_dwSourceU*11/10))||(U1.Mod<(*CF_pnU1*m_dwSourceU*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		
		if((U2.Mod>(*CF_pnU2*m_dwSourceU*11/10))||(U2.Mod<(*CF_pnU2*m_dwSourceU*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}

		if((U3.Mod>(*CF_pnU3*m_dwSourceU*11/10))||(U3.Mod<(*CF_pnU3*m_dwSourceU*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}

		if((U4.Mod>(*CF_pnU4*m_dwSourceU*11/10))||(U4.Mod<(*CF_pnU4*m_dwSourceU*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		
		if((IT1.Mod>(*CF_pnIT1*m_dwSourceI*11/10))||(IT1.Mod<(*CF_pnIT1*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IF1.Mod>(*CF_pnIF1*m_dwSourceI*11/10))||(IF1.Mod<(*CF_pnIF1*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IT2.Mod>(*CF_pnIT2*m_dwSourceI*11/10))||(IT2.Mod<(*CF_pnIT2*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IF2.Mod>(*CF_pnIF2*m_dwSourceI*11/10))||(IF2.Mod<(*CF_pnIF2*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IT3.Mod>(*CF_pnIT3*m_dwSourceI*11/10))||(IT3.Mod<(*CF_pnIT3*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IF3.Mod>(*CF_pnIF3*m_dwSourceI*11/10))||(IF3.Mod<(*CF_pnIF3*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IAT1.Mod>(*CF_pnIAT1*m_dwSourceI*11/10))||(IAT1.Mod<(*CF_pnIAT1*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if((IAT2.Mod>(*CF_pnIAT2*m_dwSourceI*11/10))||(IAT2.Mod<(*CF_pnIAT2*m_dwSourceI*9/10)))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		for(int i=0;i<12;i++)
		{
			if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))				
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		m_bIsSourceOK = TRUE;		
		return;
	}

	midU1 = CoefAdjustSort(U1.Mod,AdjustU1);
	midU2 = CoefAdjustSort(U2.Mod,AdjustU2);
	midU3 = CoefAdjustSort(U3.Mod,AdjustU3);
	midU4 = CoefAdjustSort(U4.Mod,AdjustU4);
	midIT1 = CoefAdjustSort(IT1.Mod,AdjustIT1);
	midIF1 = CoefAdjustSort(IF1.Mod,AdjustIF1);
	midIT2 = CoefAdjustSort(IT2.Mod,AdjustIT2);
	midIF2 = CoefAdjustSort(IF2.Mod,AdjustIF2);
	midIT3 = CoefAdjustSort(IT3.Mod,AdjustIT3);
	midIF3 = CoefAdjustSort(IF3.Mod,AdjustIF3);
	midIAT1 = CoefAdjustSort(IAT1.Mod,AdjustIAT1);
	midIAT2 = CoefAdjustSort(IAT2.Mod,AdjustIAT2);
	

	if(m_dwWaitCnt < (2000/AI_TIMER_PERIOD)) //ϵ��У����ԴУ����ȷ��ȴ������ӣ���֤������������
	{
		m_dwWaitCnt++;
		return;
	}

	m_pAdjustData[0]  += midU1;
	m_pAdjustData[1]  += midU2;
	m_pAdjustData[2]  += midU3;
	m_pAdjustData[3]  += midU4;
	m_pAdjustData[4]  += midIT1;
	m_pAdjustData[5]  += midIF1;
	m_pAdjustData[6]  += midIT2;
	m_pAdjustData[7]  += midIF2;
	m_pAdjustData[8]  += midIT3;
	m_pAdjustData[9]  += midIF3;
	m_pAdjustData[10] += midIAT1;
	m_pAdjustData[11] += midIAT2;
	m_dwAdjustCnt++;
}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//���³�ʼ���ۼ��������飬��������һ��ϵ��У��
	for(LONG i=0;i<28;i++)     
		m_pAdjustData[i]=0;

	//ϵ��У�������������������
	m_dwAdjustCnt =0;
	//ϵ��У����ʼ��ȴ���������������
	m_dwWaitCnt   =0;
	//��ϵ��У�������־
	m_bIsCoefAdjust = FALSE;
	//��ԴУ����־������־
	m_bIsSourceOK = FALSE;
	
	//��Һ����������Ϣ������У�����
	OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, FULLVALUE, 0, 0);

}


LONG CAIProc::CoefAdjustSort(LONG Val,LONG* OriginalData)
{	
	for(LONG i=0;i<NUMOFYCVALUE-1;i++)
		OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];

	OriginalData[0] = Val;
	
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{	
		m_pBubbleData[i]=OriginalData[i];
	}
	BubbleSort(m_pBubbleData,NUMOFYCVALUE);	

	return m_pBubbleData[NUMOFYCVALUE/2];
}

//void CAIProc::RefreshCommRW()
//{	
//	WORD sernum,cannum,ethnum;	
//	sernum = DB_GetSerialNum();
//	cannum = DB_GetCANNum();
//	ethnum = DB_GetEthLinkNum();
//	TSerialPara tSerial;
//	TCanBusPara tCan;
//	TEthPortPara tEth;
//	BYTE commuStatus;
//	if(ethnum > 0)
//	{
//		for(WORD i=0;i<ethnum;i++)
//		{			
//			DB_GetETHPara(i,&tEth);
//			WriteCommRW(i,tEth.wPortID,RW_ET11ERR,SS_ETHA_COMM_ERR,commuStatus);
//		}
//	}
//	else
//	{
//		for(WORD i=0;i<10;i++)
//			WriteRelayWord(RW_ET11ERR+i,RELAY_ON);
//	}
//	if(sernum > 0)
//	{
//		for(WORD i=0;i<sernum;i++)
//		{
//			DB_GetSerialPara(i, &tSerial);		
//			WriteCommRW(i,tSerial.wPortID,RW_COM1ERR,SS_SER_COMM_ERR,commuStatus);
//		}
//	}
//	else
//		WriteRelayWord(RW_COM1ERR,RELAY_ON);
//	if(cannum > 0)
//	{
//		for(WORD i=0;i<cannum;i++)
//		{
//			DB_GetCANPara(i, &tCan);
//			WriteCommRW(i,tCan.wPortID,RW_CAN1ERR,SS_CAN_COMM_ERR,commuStatus);
//		}
//	}
//	else
//	{
//		for(WORD i=0;i<2;i++)
//			WriteRelayWord(RW_CAN1ERR+i,RELAY_ON);
//	}
//}

//void CAIProc::WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus)
//{	
//	if(DAE_GetCommStatus(wPortID,commuStatus)!=DB_ERR_OK)	
//		WriteRelayWord(RelayWord+i,RELAY_ON);
//	else
//	{
//		if((DAE_GetRunningFlags(RunningFlag+i))||(commuStatus == COMM_STATUS_BREAK))
//			WriteRelayWord(RelayWord+i,RELAY_ON);
//		else
//			WriteRelayWord(RelayWord+i,RELAY_OFF);
//	}
//}






































