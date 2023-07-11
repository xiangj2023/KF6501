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
#include "Etpu_app.h"
#include "etpu_cfg.h"
#include "hmiapp.h"

extern DWORD dwADDataWritePtr;	//�������������ݶ�дָ��

DWORD dwAITaskID;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD g_hLCDTaskID;
extern DWORD g_dwFreqPWM;
extern DWORD dwRelayMSCnt;		//�����ú��������
extern BOOL	 g_FaultOnFlag;		//�������ѭ�������־ 
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
	OS_Sleep(2000);
	dwAITaskID = GetTaskId();		
	m_AIDataMask = ((0x01<<CHANNEL_6520_U3)|(0x01<<CHANNEL_6520_U4));
	m_CoefAdjustMask = ( (0x01<<CHANNEL_6520_Itm)|(0x01<<CHANNEL_6520_Ifm)|(0x01<<CHANNEL_6520_U1)|\
						(0x01<<CHANNEL_6520_U2)|(0x01<<CHANNEL_6520_It)|(0x01<<CHANNEL_6520_If)|\
						(0x01<<CHANNEL_6520_U3)|(0x01<<CHANNEL_6520_U4));
	//	m_RTMask = ((0x01<<CHANNEL_6520_Itm)|(0x01<<ChANNEL_6520_Ifm)|(0x01<<CHANNEL_6520_U1)|\
	//						(0x01<<CHANNEL_6520_U2));
	//	m_pRTBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*4);
	m_RTMask = ((0x01<<CHANNEL_6520_Itm)|(0x01<<CHANNEL_6520_Ifm)|(0x01<<CHANNEL_6520_U1)|\
						(0x01<<CHANNEL_6520_U2)|(0x01<<CHANNEL_6520_It)|(0x01<<CHANNEL_6520_If));  //yanxs  ���ӱ�������It��If����  ����CT���߼��
	m_pRTBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);
	m_pComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);
	m_pCoefAdjustBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*8);
	//	m_pComposeAdjustBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);

	ASSERT(m_pRTBuf != NULL);
	if(m_pRTBuf == NULL)
	{
		LogError("CAIProc,m_pRTBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pRTBuf, 0, sizeof(DWORD)*READ_POINT_NUM*6);
	
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*2);
	
	ASSERT(m_pComposeBuf != NULL);
	if(m_pComposeBuf == NULL)
	{
		LogError("CAIProc,m_pComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*2);

	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*8);

	//	ASSERT(m_pComposeAdjustBuf != NULL);
	//	if(m_pComposeAdjustBuf == NULL)
	//	{
	//		LogError("CAIProc,m_pComposeAdjustBuf",FILE_LINE,"malloc error.");
	//		return;
	//	}
	//	memset(m_pComposeAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*2);
	
	//*******************��Ƶ�û�����**********************************
	m_bCaluFreqFlag = FALSE;
	m_dwFreqMask = CHANNEL_6520_U1;	
	m_wFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	m_FreqCaluBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*FREQUENCE_SAMP_NUM);
	
	ASSERT(m_wFreqBuf != NULL);
	if(m_wFreqBuf == NULL)
	{
		LogError("CAIProc,m_wFreqBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_wFreqBuf, 0, sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	
	ASSERT(m_FreqCaluBuf != NULL);
	if(m_FreqCaluBuf == NULL)
	{
		LogError("CAIProc,m_FreqCaluBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_FreqCaluBuf, 0, sizeof(DWORD)*FREQUENCE_SAMP_NUM);

	for(LONG i=0;i<15;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		UMArray[i]=0;
		U1Array[i]=0;
		U2Array[i]=0;
		IMArray[i]=0;
		ITArray[i]=0;
		IFArray[i]=0;
		PArray[i]=0;
		QArray[i]=0;
		COSArray[i]=0;
		I1Array[i]=0;
		I2Array[i]=0;
		I3Array[i]=0;
		I5Array[i]=0;
		I7Array[i]=0;
		I9Array[i]=0;
		I11Array[i]=0;
		RArray[i]=0;
		XArray[i]=0;
		m_pBubbleData[i]=0;

		AdjustIp[i]=0;
		AdjustIt[i]=0;
		AdjustIf[i]=0;
		AdjustUp[i]=0;
		AdjustU1[i]=0;
		AdjustU2[i]=0;
		AdjustIm[i]=0;
		AdjustItm[i]=0;
		AdjustIfm[i]=0;
		AdjustUm[i]=0;
		AdjustU1m[i]=0;
		AdjustU2m[i]=0;
		AdjustP[i]=0;
	}
	//��ʼ��ϵ��У׼��־	
	m_bIsSourceOK =FALSE;			//�ǲ��Ѿ�У����־
	m_lDACoef =0;					//�ǲ�ֵ
	m_bIsCoefAdjust = FALSE;		//����ϵ��У����־
	m_dwWaitCnt = 0;				//��ʼУ����ĵȴ�����
	m_dwAdjustCnt=0;				//У���������

	// 1�ȵ����������1w����ϵ��*1000*1Сʱ���ۼӴ���
	// ��װ�õ�ȵ���С�̶�Ϊ0.01�ȣ��ù���ϵ����100W��ϵ��ֵ���������Ժ���ۼ����ۼ���ֱ��������ֵ���бȽϡ�
	m_OneKilowattP = (QWORD)*CF_pnP*10*3600*1000/(AI_TIMER_PERIOD*100);
	m_OneKilowattQ = (QWORD)*CF_pnQ*10*3600*1000/(AI_TIMER_PERIOD*100);
	m_AccKilowattP = 0;
	m_AccKilowattQ = 0;

	m_U1Last = 0;
	m_U2Last = 0;
	m_ItLast = 0;
	m_IfLast = 0;
	m_U1m = 0;
	m_U2m = 0;
	m_Itm = 0;
	m_Ifm = 0;
	
	m_U1WritePtr = 0;
	m_U2WritePtr = 0;
	m_ItWritePtr = 0;
	m_IfWritePtr = 0;
	
	//-----------CT���߼�ⲿ�ֳ�ʼ��-----------------
	InitCTIt();
	InitCTIf();
	CTCurSet = (*AS_pnPCTRatio)*CTBRCHECKVALUE;
	CTTimeSet = 3000;
	//--------------------------------------------------
	//����AI��ʱ��
	SetTimer(1,AI_TIMER_PERIOD_ONE);
	wTaskScheduleCnt = 0;	
}
/*********************************************************************************
*����: AI��ʱ��Ϣ��Ӧ
*����: ��ʱ��ID
*���: ��
*����: ��
**************************************************************************************/

void CAIProc::OnTimeOut(DWORD id)
{	
	STATUS rc;
	static WORD coefAdjustCnt = 0;
	if(m_bIsCoefAdjust == TRUE)
	{
		coefAdjustCnt++;
		if(coefAdjustCnt == 10)
		{
			CoefAdjust();	
			coefAdjustCnt = 0;
		}
		return;
	}	
	rc = ReadSampleData(m_pRTBuf,m_RTMask,0); 	
	if(rc == ERR_FAILURE)
		return;
	wTaskScheduleCnt++;

	Filter(m_pRTBuf);
	Filter(m_pRTBuf+READ_POINT_NUM);
	Filter(m_pRTBuf+READ_POINT_NUM*2);
	Filter(m_pRTBuf+READ_POINT_NUM*3);
	
	CaluMSMod(m_pRTBuf,m_U1m);						//������ѹU1
	CaluMSMod(m_pRTBuf+READ_POINT_NUM,m_U2m); 		//������ѹU2
	CaluMSMod(m_pRTBuf+READ_POINT_NUM*2,m_Itm);		//��������It
	CaluMSMod(m_pRTBuf+READ_POINT_NUM*3,m_Ifm);		//��������If	
//	//-----------------CT���߼������-----------------
	Filter(m_pRTBuf+READ_POINT_NUM*4);			//CT���߼������
	Filter(m_pRTBuf+READ_POINT_NUM*5);
	CaluMSMod(m_pRTBuf+READ_POINT_NUM*4,m_It_p);		//��������It  CT���߼������
	CaluMSMod(m_pRTBuf+READ_POINT_NUM*5,m_If_p);		//��������If  CT���߼������
	*RM_pnItm= m_Itm;                                       
	DB_LogWriteRM(RM_ITM, *RM_pnItm);	
	*RM_pnIfm= m_Ifm;                                       
	DB_LogWriteRM(RM_IFM, *RM_pnIfm);	
	CaluCTMaxMin();
	if((g_FaultOnFlag == TRUE)||(ReadRelayWord(RW_52BF)))
	{	
		
		if(!ReadRelayWord(RW_ARCT1))
		{
			ResetCTCheck(&RelCTIt);
		}
		if(!ReadRelayWord(RW_ARCT2))
		{
			ResetCTCheck(&RelCTIf);
		}
	}
	else
	{
		CTCheck(&RelCTIt);
		if(*AS_pnPM != BTMODE)
			CTCheck(&RelCTIf);
	}
	
	//--------------------------------------
	if(*CFG_pbyLOADREC== CFG_STATUS_SET) //�仯ң�⹦��Ͷ��
	{
		ReadAbsTime(&m_AbsTime);
		LONG	temp =0;
		temp = m_U1m-m_U1Last;
		if(ToAbs(temp)>*AS_pnV_THRD)
		{
			m_EventAI.nNo = M_U1;
			m_EventAI.lValue = m_U1m;
			m_EventAI.stTime = m_AbsTime;
			DB_WriteAIE(&m_EventAI);
		}		
		temp = m_U2m-m_U2Last;
		if(ToAbs(temp)>*AS_pnV_THRD)
		{
			m_EventAI.nNo = M_U2;
			m_EventAI.lValue = m_U2m;
			m_EventAI.stTime = m_AbsTime;
			DB_WriteAIE(&m_EventAI);
		}
		temp = m_Itm-m_ItLast;
		if(ToAbs(temp)>*AS_pnC_THRD)
		{
			m_EventAI.nNo = M_IT;
			m_EventAI.lValue = m_Itm;
			m_EventAI.stTime = m_AbsTime;
			DB_WriteAIE(&m_EventAI);
		}
		temp = m_Ifm-m_IfLast;
		if(ToAbs(temp)>*AS_pnC_THRD)
		{
			m_EventAI.nNo = M_IF;
			m_EventAI.lValue = m_Ifm;
			m_EventAI.stTime = m_AbsTime;
			DB_WriteAIE(&m_EventAI);
		}
	}
	m_U1Last = m_U1m;	
	m_U2Last = m_U2m;	
	m_ItLast = m_Itm;	
	m_IfLast = m_Ifm;

	if(m_bCaluFreqFlag)
	{
		//װ��������2�뿪ʼ��Ƶ
		static WORD freqcounter =0;
		if((freqcounter>10))
		{
			m_FreqFlag = TRUE;
		}		
		freqcounter++;		
		if(m_FreqFlag)
		{		
			CalcImmeFrequence(dwADDataWritePtr,m_U1m,m_dwFreqMask,m_wFreqBuf,m_FreqCaluBuf,&m_dwPreFreq);		
			DB_WriteAI(M_F,m_dwPreFreq);
		}
		m_bCaluFreqFlag = FALSE;
	}

	if(wTaskScheduleCnt == AI_TIMER_PERIOD_NUM)
	{
		RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
		//��ȡԭʼ��������
		rc = ReadSampleData(m_pDataBuf,m_AIDataMask,0);		
		if(rc == ERR_FAILURE)
			return;
		
		//�ϳ�Up/Ipͨ�������˲�
		WriteMSComposeBuf();	
		//8������ͨ�������˲�
		for(LONG i=0;i<2;i++)
			Filter(m_pDataBuf+READ_POINT_NUM*i);
		
		//���������Ƕȵļ���
		CaluBaseFourier_S15(m_pRTBuf+READ_POINT_NUM*2,&m_sItm);
		CaluBaseFourier_S15(m_pRTBuf+READ_POINT_NUM*3,&m_sIfm);
		CaluBaseFourier_S15(m_pComposeBuf+READ_POINT_NUM,&m_sIp);	
		CaluModValue(&m_sItm);
		CaluModValue(&m_sIfm);
		CaluModValue(&m_sIp);	
		CaluAngle(&m_sIp,AngleTab);

		LONG angle;
		CaluAngle(&m_sItm,AngleTab);
		angle = m_sItm.Angle-m_sIp.Angle;
		if(angle>0)
			*RM_pnAITM = angle;
		else
			*RM_pnAITM = angle+36000;
		DB_LogWriteRM(RM_AITM, *RM_pnAITM); 
		
		CaluAngle(&m_sIfm,AngleTab);
		angle = m_sIfm.Angle-m_sIp.Angle;
		if(angle>0)
			*RM_pnAIFM = angle;
		else
			*RM_pnAIFM = angle+36000;
		DB_LogWriteRM(RM_AIFM, *RM_pnAIFM);	

		
		//U3,U4����ֵ����
		CaluMSMod(m_pDataBuf,m_U3m);		//��������It
		CaluMSMod(m_pDataBuf+READ_POINT_NUM,m_U4m);		//��������If	
		CaluMSPQ(m_pRTBuf,m_pRTBuf+READ_POINT_NUM*2,*CF_pnA,m_lMSP,m_lMSQ,m_lMSCOS);		//�����й����޹�����������

	//���ֵ�ȼ���
		if(DB_IsCaculateIntegral())
		{
			if(m_lMSP>=0)
				m_AccKilowattP += m_lMSP;
			if(m_lMSQ>=0)			
				m_AccKilowattQ += m_lMSQ;

			if(m_AccKilowattP>m_OneKilowattP)
			{
				DB_WriteCI(0,1);
				m_AccKilowattP -= m_OneKilowattP;
				
			}		
			if(m_AccKilowattQ>m_OneKilowattQ)
			{
				DB_WriteCI(1,1);
				m_AccKilowattQ -= m_OneKilowattQ;
			}
		}
	//г������ֵ���㣬���ñ�������ͨ��
		CaluBaseFourier_S15(m_pComposeBuf+READ_POINT_NUM,&m_sI1);
		CaluSecondFourier(m_pComposeBuf+READ_POINT_NUM,&m_sI2);
		CaluThirdFourier(m_pComposeBuf+READ_POINT_NUM,&m_sI3);
		CaluFifthFourier(m_pComposeBuf+READ_POINT_NUM,&m_sI5);
		CaluSeventhFourier(m_pComposeBuf+READ_POINT_NUM,&m_sI7);
		CaluNinthFourier(m_pComposeBuf+READ_POINT_NUM,&m_sI9);
		CaluEleventhFourier(m_pComposeBuf+READ_POINT_NUM,&m_sI11);	
		CaluModValue(&m_sI1);
		CaluModValue(&m_sI2);
		CaluModValue(&m_sI3);
		CaluModValue(&m_sI5);
		CaluModValue(&m_sI7);
		CaluModValue(&m_sI9);
		CaluModValue(&m_sI11);
		m_sI2.Mod = m_sI2.Mod*CurHarmCal[0]/1000; 
		m_sI3.Mod = m_sI3.Mod*CurHarmCal[1]/1000; 
		m_sI5.Mod = m_sI5.Mod*CurHarmCal[2]/1000; 
		m_sI7.Mod = m_sI7.Mod*CurHarmCal[3]/1000; 
		m_sI9.Mod = m_sI9.Mod*CurHarmCal[4]/1000; 
		m_sI11.Mod = m_sI11.Mod*CurHarmCal[5]/1000; 

		//�����ݿ���д�����ֵ
		m_U1m = AiBubbleSort(m_U1m,U1Array);
		if(m_U1m<=*CF_pnU1DRIFT)		
			m_U1m = 0;
		DB_WriteAI(M_U1,m_U1m);
		
		m_U2m = AiBubbleSort(m_U2m,U2Array);
		if(m_U2m<=*CF_pnU2DRIFT)		
			m_U2m = 0;
		DB_WriteAI(M_U2,m_U2m);

		m_U3m = AiBubbleSort(m_U3m,U3Array);
		if(m_U3m<=*CF_pnU3DRIFT)		
			m_U3m = 0;
		DB_WriteAI(M_U3,m_U3m);

		m_U4m = AiBubbleSort(m_U4m,U4Array);
		if(m_U4m<=*CF_pnU4DRIFT)		
			m_U4m = 0;
		DB_WriteAI(M_U4,m_U4m);

		m_Itm = AiBubbleSort(m_Itm,ITArray);
		if(m_Itm<=*CF_pnITDRIFT)		
			m_Itm = 0;
		DB_WriteAI(M_IT,m_Itm);

		m_Ifm = AiBubbleSort(m_Ifm,IFArray);
		if(m_Ifm<=*CF_pnIFDRIFT)		
			m_Ifm = 0;
		DB_WriteAI(M_IF,m_Ifm);
		
		WriteAIData(M_P,m_lMSP,m_pBubbleData,PArray);
		WriteAIData(M_Q,m_lMSQ,m_pBubbleData,QArray);
		WriteAIData(M_COS,m_lMSCOS,m_pBubbleData,COSArray);
		WriteAIData(M_I1,m_sI1.Mod,m_pBubbleData,I1Array);
		WriteAIData(M_I2,m_sI2.Mod,m_pBubbleData,I2Array);
		WriteAIData(M_I3,m_sI3.Mod,m_pBubbleData,I3Array);
		WriteAIData(M_I5,m_sI5.Mod,m_pBubbleData,I5Array);
		WriteAIData(M_I7,m_sI7.Mod,m_pBubbleData,I7Array);
		WriteAIData(M_I9,m_sI9.Mod,m_pBubbleData,I9Array);
		WriteAIData(M_I11,m_sI11.Mod,m_pBubbleData,I11Array);

		m_bCaluFreqFlag = TRUE;
		wTaskScheduleCnt = 0;
	}
}
void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//�����Զ�У׼��־

	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDriftAdjust = TRUE;
		ItmDrift = 0;
		IfmDrift = 0;
		U1mDrift = 0;
		U2mDrift = 0;
	}
	else
		m_bIsDriftAdjust = FALSE;
		
	
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	dwMsgID = arg;
	m_bIsCoefAdjust = TRUE;
	m_bIsSourceOK = FALSE;
	m_dwWaitCnt =0;
	m_dwAdjustCnt = 0;	
	
	MedianIm = 0;
	MedianItm = 0;
	MedianIfm = 0;
	MedianUm = 0;
	MedianU1m = 0;
	MedianU2m = 0;		
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		AdjustIp[i]  = 0;
		AdjustIt[i]	 = 0;
		AdjustIf[i]	 = 0;
		AdjustUp[i]	 = 0;
		AdjustU1[i]	 = 0;
		AdjustU2[i]  = 0;
		AdjustIm[i]  = 0;
		AdjustItm[i] = 0;
		AdjustIfm[i] = 0;
		AdjustUm[i]  = 0;
		AdjustU1m[i] = 0;
		AdjustU2m[i] = 0;
		AdjustP[i] 	 = 0;
	}
}
/**************************************************************************************************
* ����: ����ң�����й����޹�����������
* ����: �˲�������ݻ�����
* ����: ��
* ��ע:
***************************************************************************************************/
void CAIProc::WriteMSComposeBuf()
{	

	if(*AS_pnPM == BTMODE)				
	{
		for(LONG i=0;i<READ_POINT_NUM;i++) 
		{
			m_pComposeBuf[i]=m_pRTBuf[i];		 //��ѹͨ��	UP
			m_pComposeBuf[READ_POINT_NUM+i]=m_pRTBuf[READ_POINT_NUM*4+i];//IP
		}
	}
	else if(*AS_pnPM == AT1MODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++) 		//��ѹͨ��
		{
			m_pComposeBuf[i]=m_pRTBuf[i];			
			m_pComposeBuf[READ_POINT_NUM+i]=m_pRTBuf[READ_POINT_NUM*4+i]-m_pRTBuf[READ_POINT_NUM*5+i];
		}
	}
	else if(*AS_pnPM == AT2MODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)     //��U1��U2ͨ�����������ݺϳ�Upͨ������������ 
		{
			m_pComposeBuf[i]=m_pRTBuf[i]-m_pRTBuf[READ_POINT_NUM+i];			
			m_pComposeBuf[READ_POINT_NUM+i]=m_pRTBuf[READ_POINT_NUM*4+i]-m_pRTBuf[READ_POINT_NUM*5+i];
		}
	}
}
/**************************************************************************************************
* ����: ����ң�����й����޹�����������
* ����: �˲�������ݻ�����
* ����: ��
* ��ע:
***************************************************************************************************/
void CAIProc::CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ,LONG &MSCOS,BOOL coef)
{
	LLONG   p 	=0;
	LLONG   q	=0;
	QWORD 	s	=0;
	DWORD 	s2	=0;
	LONG    tp 	= 0;
	LONG    tq 	= 0;
	//���ʼ���
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		p += UBuf[1+i]*IBuf[1+i];
		q += IBuf[1+i]*(UBuf[i]-UBuf[2+i]);
	}
	tp = p/3200;
	tq = q/SINWTS;
	//����У��
	MSP = tp + tq*DACoef/10000;     
	MSQ = tq - tp*DACoef/10000;  
	//������������
	s=(LLONG)MSP*MSP+(LLONG)MSQ*MSQ;
	s2= Evolution64(s);
	if(s2<=20)
		MSCOS=0;
	else
	{
		MSCOS = (LLONG)MSP*10000/(LONG)s2;
	}
	if(coef)
	{
		if(ToAbs(MSCOS)>9990)
			MSQ=0;
		if(ToAbs(MSCOS)<10)
			MSP=0;
	}
	MSCOS = MSCOS/10;
}
/************************************************************************************************
*����: ����ϵ��У���еļ���
*����: 
*����: ��
*��ע:
*************************************************************************************************/
void CAIProc::CoefAdjustCalu(void)
{
	
	ReadSampleData(m_pCoefAdjustBuf,m_CoefAdjustMask,0);		//��ȡԭʼ��������	
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf,&m_AdjustU1);
	CaluModValue(&m_AdjustU1);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&m_AdjustU2);
	CaluModValue(&m_AdjustU2);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&m_AdjustIt);
	CaluModValue(&m_AdjustIt);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&m_AdjustIf);
	CaluModValue(&m_AdjustIf);
 
	Filter(m_pCoefAdjustBuf);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*2);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*3);
	
	CaluMSMod(m_pCoefAdjustBuf,m_AdjustU1m);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,m_AdjustU2m);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_AdjustItm);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,m_AdjustIfm);
}
/************************************************************************************************
*����: ϵ��У��
*����: �˲������ݻ�����
*����: ��
*��ע:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	//ϵ������֮ǰ�轫װ������ΪBT���緽ʽ
	if(*AS_pnPM != BTMODE)
	{
		CoefAdjustEnd(FAILURE);
		return;
	}
	if(m_dwAdjustCnt>99)
	{
		if(m_bIsDriftAdjust == TRUE)
		{					
			ItmDrift	= m_pAdjustData[10]; 				//Itm
			IfmDrift	= m_pAdjustData[11]; 				//Ifm
			U1mDrift 	= m_pAdjustData[12];				//U1m������100V��ϵ��
			U2mDrift	= m_pAdjustData[13];				//U2m������100V��ϵ��
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{
			//ע��U3/U4��PT���ܲ���װ��ֱ�ӵ���U1��ϵ��
			LONG Value[24];			
			
			Value[0]	= (m_pAdjustData[0]+m_pAdjustData[1])/(m_dwSourceI*20);  //Ip
			Value[1]	= (m_pAdjustData[2]+m_pAdjustData[3])/(m_dwSourceU*2);	 //Up
			Value[2]	= Value[1];												 //U3
			Value[3]	= Value[1];												 //U4
			Value[4]	= m_pAdjustData[4]/(m_dwSourceI*10);					 //ITM
			Value[5]	= m_pAdjustData[5]/(m_dwSourceI*10);				     //IFM
			Value[6]	= m_pAdjustData[6]/m_dwSourceU;							 //U1M
			Value[7]	= m_pAdjustData[7]/m_dwSourceU;							 //U2M
			Value[8]	= Value[7];												 //U3M
			Value[9]	= Value[7];												 //U4M
			Value[10]	= m_pAdjustData[8]/(m_dwSourceI*m_dwSourceU);			 //P
			Value[11]	= Value[10];											 //Q
			Value[12]	= m_lDACoef;											 //A
			Value[13]	= Value[1]*IMPDMULTIPLE/(10*Value[0]);				     //Z
			Value[14]	= 1000;					
			Value[15]	= 100;								
			Value[16]	= 10;					
			Value[17]	= 1;								
			Value[18] 	= ItmDrift;								
			Value[19] 	= IfmDrift;		
			Value[20] 	= U1mDrift;		
			Value[21] 	= U2mDrift;	
			Value[22] 	= U1mDrift;										
			Value[23] 	= U1mDrift;										
				
			DB_SetCFValue(CF_VALUE_ALL, Value);					//д��ϵ��ֵ
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);		//д��ϵ��ȷ��
			CoefAdjustEnd(SUCCESS);
			return;
		}
	
	}
	
	CoefAdjustCalu();

	if(!m_bIsSourceOK)  //����У���ǲ��ѹ����Դ���
	{
		if(m_bIsDriftAdjust) //����ֵУ��Դ�ж�
		{
			if(m_AdjustU1.Mod>(*CF_pnUP/200))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustU2.Mod>(*CF_pnUP/200))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustIt.Mod>(*CF_pnIP/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustIf.Mod>(*CF_pnIP/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustItm>(*CF_pnITM/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustIfm>(*CF_pnIFM/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		else	//ϵ��ֵУ��Դ�ж�
		{
			if((m_AdjustU1.Mod>(*CF_pnUP*m_dwSourceU*11/1000))||(m_AdjustU1.Mod<(*CF_pnUP*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustU2.Mod>(*CF_pnUP*m_dwSourceU*11/1000))||(m_AdjustU2.Mod<(*CF_pnUP*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustIt.Mod>(*CF_pnIP*m_dwSourceI*11/100))||(m_AdjustIt.Mod<(*CF_pnIP*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustIf.Mod>(*CF_pnIP*m_dwSourceI*11/100))||(m_AdjustIf.Mod<(*CF_pnIP*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustItm>(*CF_pnITM*m_dwSourceI*11/100))||(m_AdjustItm<(*CF_pnITM*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if((m_AdjustIfm>(*CF_pnIFM*m_dwSourceI*11/100))||(m_AdjustIfm<(*CF_pnIFM*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			for(int i=1;i<6;i++)
			{
				if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}			
			}			
			CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*2,0,m_AdjustP,m_AdjustQ,m_AdjustCos,FALSE);
			if(m_AdjustQ>*CF_pnQ/3) 		
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if(m_AdjustQ == 0)
				m_lDACoef = 0;
			else
				m_lDACoef = m_AdjustQ*10000/((LONG)Evolution64((LLONG)m_AdjustP*m_AdjustP+(LLONG)m_AdjustQ*m_AdjustQ)); 		
		}
		m_bIsSourceOK = TRUE;
		return;
	}

	CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*2,m_lDACoef,m_AdjustP,m_AdjustQ,m_AdjustCos);

	if(m_bIsDriftAdjust)
	{
		if(m_AdjustItm>MedianItm)
			MedianItm = m_AdjustItm;
		if(m_AdjustIfm>MedianIfm)
			MedianIfm = m_AdjustIfm;
		if(m_AdjustU1m>MedianU1m)
			MedianU1m = m_AdjustU1m;
		if(m_AdjustU2m>MedianU2m)
			MedianU2m = m_AdjustU2m;		
	}
	else
	{
		MedianIt = AiBubbleSort(m_AdjustIt.Mod,AdjustIt); 
		MedianIf = AiBubbleSort(m_AdjustIf.Mod,AdjustIf);
		MedianU1 = AiBubbleSort(m_AdjustU1.Mod,AdjustU1);
		MedianU2 = AiBubbleSort(m_AdjustU2.Mod,AdjustU2);
		
		MedianItm = AiBubbleSort(m_AdjustItm,AdjustItm);
		MedianIfm = AiBubbleSort(m_AdjustIfm,AdjustIfm);
		MedianU1m = AiBubbleSort(m_AdjustU1m,AdjustU1m);
		MedianU2m = AiBubbleSort(m_AdjustU2m,AdjustU2m);
		MedianP   = AiBubbleSort(m_AdjustP,AdjustP);
	}
	
	if(m_dwWaitCnt < (2000/AI_TIMER_PERIOD)) //ϵ��У����ԴУ����ȷ��ȴ�������
	{
		m_dwWaitCnt++;
		return;
	}
	if(m_bIsDriftAdjust)
	{
		
		m_pAdjustData[10]  = MedianItm;
		m_pAdjustData[11]  = MedianIfm;
		m_pAdjustData[12]  = MedianU1m;
		m_pAdjustData[13]  = MedianU2m;
	}
	else
	{
		m_pAdjustData[0] += MedianIt;
		m_pAdjustData[1] += MedianIf;
		m_pAdjustData[2] += MedianU1;
		m_pAdjustData[3] += MedianU2;
		m_pAdjustData[4] += MedianItm;
		m_pAdjustData[5] += MedianIfm;
		m_pAdjustData[6] += MedianU1m;
		m_pAdjustData[7] += MedianU2m;
		m_pAdjustData[8] += MedianP;
		m_pAdjustData[9] += m_AdjustQ;	
	}
//	ReportMsg("%d",MedianU1);
	m_dwAdjustCnt++;
	

}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//���³�ʼ���ۼ��������飬��������һ��ϵ��У��
	for(LONG i=0;i<15;i++) 
	{
		m_pAdjustData[i] =0;
	}
	
	//�ǲ�У����־����
	m_bIsSourceOK = FALSE;

	//ϵ��У�������������������
	m_dwAdjustCnt=0;

	//��ϵ��У�������־
	m_bIsCoefAdjust = FALSE;

	//ϵ��У���ȴ�ʱ������
	m_dwWaitCnt = 0;
	
	//��Һ����������Ϣ������У�����
	if(m_bIsDriftAdjust == TRUE)
		OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, ZERODRIFT, 0, 0);
	else
		OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, FULLVALUE, 0, 0);
	
}

LONG CAIProc::AiBubbleSort(LONG Val,LONG* OriginalData)
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

void CAIProc::CaluCTMaxMin(void)
{	
	LONG tempIt,tempIf;
	CTCurSet = (*AS_pnPCTRatio)*CTBRCHECKVALUE;
	m_ItMax = (*AS_pnPCTRatio)*m_It_p*CTBRKPM;
	m_ItMin = (*AS_pnMCTRatio)*m_Itm;
	m_IfMax = (*AS_pnPCTRatio)*m_If_p*CTBRKPM;
	m_IfMin = (*AS_pnMCTRatio)*m_Ifm;
	
	if(m_ItMax < m_ItMin)
	{		
		tempIt = m_ItMax;
		m_ItMax = m_ItMin;
		m_ItMin = tempIt;
	}
	if(m_IfMax < m_IfMin)
	{		
		tempIf = m_IfMax;
		m_IfMax = m_IfMin;
		m_IfMin = tempIf;
	}
}

void CAIProc::InitCTIt()
{
	RelCTIt.pRelCTBrokCFG = CFG_pbyCTBR;				//CT�������� 
	RelCTIt.pRelCurSet = &CTCurSet;					//�����ż�ֵ 
	RelCTIt.pRelTimeSet = &CTTimeSet;				//�澯ʱ�� 
	RelCTIt.dwSysTimer = &dwRelayMSCnt;				//ϵͳ��ʱ�� 

	RelCTIt.pRelCurMax = &m_ItMax;						//�ж��ô���� 
	RelCTIt.pRelCurMin = &m_ItMin;						//�ж���С���� 

	RelCTIt.StartRelayWord = RW_PICT1;			//CT������μ̵����� 
	RelCTIt.ActRelayWord = RW_ARCT1;				//CT���߶����̵����� 	
	RelCTIt.wRelRptNum = R_CT1;					//CT���߶��������
	RelCTIt.wRelResRptNum = R_CT1_RES;			//CT���߶��������
	
	RelCTIt.byRelFCounter = 0;						//���ϼ����� 
	RelCTIt.byRelRCounter = 0;						//���ؼ����� 
	RelCTIt.dwRelTimer = 0;							//CT���߶�ʱ�� 	
	RelCTIt.wExcepWordNum = 6;						//TRelCTBrokPar CT���� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6����СΪ18 
}
void CAIProc::InitCTIf()
{
	RelCTIf.pRelCTBrokCFG = CFG_pbyCTBR;				//CT�������� 
	RelCTIf.pRelCurSet = &CTCurSet;					//�����ż�ֵ 
	RelCTIf.pRelTimeSet = &CTTimeSet;				//�澯ʱ�� 
	RelCTIf.dwSysTimer = &dwRelayMSCnt;				//ϵͳ��ʱ�� 

	RelCTIf.pRelCurMax = &m_IfMax;						//�ж��ô���� 
	RelCTIf.pRelCurMin = &m_IfMin;						//�ж���С���� 

	RelCTIf.StartRelayWord = RW_PICT2;			//CT������μ̵����� 
	RelCTIf.ActRelayWord = RW_ARCT2;				//CT���߶����̵����� 	
	RelCTIf.wRelRptNum = R_CT2;					//CT���߶��������
	RelCTIf.wRelResRptNum = R_CT2_RES;			//CT���߶��������
	
	RelCTIf.byRelFCounter = 0;						//���ϼ����� 
	RelCTIf.byRelRCounter = 0;						//���ؼ����� 
	RelCTIf.dwRelTimer = 0;							//CT���߶�ʱ�� 	
	RelCTIf.wExcepWordNum = 6;						//TRelCTBrokPar CT���� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6����СΪ18 
}


















