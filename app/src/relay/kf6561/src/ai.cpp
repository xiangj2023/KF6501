/*******************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
*******************************************************************************************/ 
                          
/*******************************************************************************************
*                                                                                  
* 文件名称          
*			ai.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           		遥测任务                                                                      
*			                                                                
* 描述                                                                             
*                                                                                  
*                                
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	
*		
*		
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      yanzh             09/08/20    初始创建                         
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
#include "relay_defs.h"

DWORD dwAITaskID;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD g_hLCDTaskID;
extern DWORD g_dwFreqPWM;
extern DWORD dwADDataWritePtr;	//采样缓冲区数据读写指针
extern BOOL g_FaultOnFlag;
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
BEGIN_MESSAGE_MAP(CAIProc,CApp)
	ON_SM_TIMEOUT(CAIProc)
	ON_SM_AUTOADJUST(CAIProc)	
END_MESSAGE_MAP()

/*********************************************************************************************
 * 功能: AI任务入口函数
 * 输入: pdwApp	
 * 返回: 无
 * 备注: 无        
**********************************************************************************************/

void AITask(DWORD *pdwApp)
{
	CAIProc *pAITask = new CAIProc(pdwApp);
	pAITask->Init();
	pAITask->Run();
}	
/*********************************************************************************
*功能: 遥测初始化子程序
*输入: 无
*返回: 无
*备注: 无
**************************************************************************************/

void CAIProc::Init(void)
{		
	dwAITaskID = GetTaskId();		
	m_AIDataMask = ( (0x01<<CHANNEL_6561_UA)|(0x01<<CHANNEL_6561_UB)|(0x01<<CHANNEL_6561_UC)|\
						(0x01<<CHANNEL_6561_U0)|(0x01<<CHANNEL_6561_U1)|(0x01<<CHANNEL_6561_U2)|\
						(0x01<<CHANNEL_6561_U3)|(0x01<<CHANNEL_6561_IAM)|(0x01<<CHANNEL_6561_IBM)|\
						(0x01<<CHANNEL_6561_ICM)|(0x01<<CHANNEL_6561_I0)|(0x01<<CHANNEL_6561_I1)|\
						(0x01<<CHANNEL_6561_IA)|(0x01<<CHANNEL_6561_IB)|(0x01<<CHANNEL_6561_IC));  //yanxs  2013-03-22 CT断线检查增加保护电流

	m_CoefAdjustMask = ( (0x01<<CHANNEL_6561_UA)|(0x01<<CHANNEL_6561_UB)|(0x01<<CHANNEL_6561_UC)|\
						(0x01<<CHANNEL_6561_U0)|(0x01<<CHANNEL_6561_U1)|(0x01<<CHANNEL_6561_U2)|\
						(0x01<<CHANNEL_6561_U3)|(0x01<<CHANNEL_6561_IAM)|(0x01<<CHANNEL_6561_IBM)|\
						(0x01<<CHANNEL_6561_ICM)|(0x01<<CHANNEL_6561_I0)|(0x01<<CHANNEL_6561_I1)| \
						(0x01<<CHANNEL_6561_IA)|(0x01<<CHANNEL_6561_IB)|(0x01<<CHANNEL_6561_IC));
	
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*15);
	m_pComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	m_pCoefAdjustBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*15);

	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);
	
	ASSERT(m_pComposeBuf != NULL);
	if(m_pComposeBuf == NULL)
	{
		LogError("CAIProc,m_pComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*3);

	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);


//测频相关***********************************************************************

	m_dwFreqMask = (0x01<<CHANNEL_6561_UA);

	m_wFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*FREQUENCE_SAMP_NUM);	
	ASSERT(m_wFreqBuf != NULL);
	if(m_wFreqBuf == NULL)
	{
		LogError("CAIProc,m_wFreqBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_wFreqBuf, 0, sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	
	m_FreqCaluBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*FREQUENCE_SAMP_NUM);	
	ASSERT(m_FreqCaluBuf != NULL);
	if(m_FreqCaluBuf == NULL)
	{
		LogError("CAIProc,m_FreqCaluBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_FreqCaluBuf, 0, sizeof(DWORD)*FREQUENCE_SAMP_NUM);
//测频相关***********************************************************************



	for(LONG i=0;i<39;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		m_pBubbleData[i]=0;
		
		UaArray[i]=0;
		UbArray[i]=0;
		UcArray[i]=0;
		U0Array[i]=0;
		U1Array[i]=0;
		U2Array[i]=0;
		U3Array[i]=0;
		UabArray[i]=0;
		UbcArray[i]=0;
		UcaArray[i]=0;
		IaArray[i]=0;
		IbArray[i]=0;
		IcArray[i]=0;
		I0Array[i]=0;
		I1Array[i]=0;
		
		AdjUaArray[i]=0;
		AdjUbArray[i]=0;
		AdjUcArray[i]=0;
		AdjU0Array[i]=0;
		AdjU1Array[i]=0;
		AdjU2Array[i]=0;
		AdjU3Array[i]=0;
		AdjIAArray[i]=0;
		AdjIBArray[i]=0;
		AdjICArray[i]=0;
		AdjI0Array[i]=0;
		AdjI1Array[i]=0;


	}
	//初始化系数校准标志
	
	m_bIsSourceOK =FALSE;		//角差已经校正标志
	AdjAA =0;					//角差值
	AdjBA =0;
	AdjCA =0;
	m_bIsCoefAdjust = FALSE;		//进入系数校正标志
	m_dwWaitCnt = 0;				//开始校正后的等待次数
	m_dwAdjustCnt=0;				//校正计算次数


	//CT断线初始化
	InitCTACheck();
	InitCTBCheck();
	InitCTCCheck();
	CTCurSet = (*AS_pnPCTRatio)*CTBRCHECKVALUE;
	CTTimeSet = 3000;


	
	// 1度电的数字量，1w功率系数*1000*1小时的累加次数
	// 本装置电度的最小刻度为0.01度，该功率系数是100W的系数值，所以在以后的累加中累加量直接与以下值进行比较。
	m_OneKilowattP = (QWORD)*CF_pnC2*1000*3600*1000/(AI_TIMER_PERIOD*100);
	m_OneKilowattQ = (QWORD)*CF_pnC2*1000*3600*1000/(AI_TIMER_PERIOD*100);
	m_AccKilowattP = 0;
	m_AccKilowattQ = 0;
	//设置AI定时器
	SetTimer(1,AI_TIMER_PERIOD);
}
/*********************************************************************************
*功能: AI定时消息响应
*输入: 定时器ID
*输出: 无
*返回: 无
**************************************************************************************/

void CAIProc::OnTimeOut(DWORD id)
{
	STATUS rc;
	if(m_bIsCoefAdjust == TRUE)
	{
		CoefAdjust();	
		return;
	}
//读取原始采样数据
	rc = ReadSampleData(m_pDataBuf,m_AIDataMask,0);		
	if(rc == ERR_FAILURE)
		return;
	
//合成Up/Ip通道，并滤波
	WriteMSComposeBuf(m_pComposeBuf,m_pDataBuf);			
	for(LONG i=0;i<15;i++)
		Filter(m_pDataBuf+READ_POINT_NUM*i);
	for(LONG i=0;i<3;i++)
		Filter(m_pComposeBuf+READ_POINT_NUM*i);
	
//测量值计算
	CaluMSMod(m_pDataBuf,Ua);						
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,Ub);		
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,Uc);						
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,U0);			
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,U1);		
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,U2);		
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,U3);		
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,IA);			
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,IB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,IC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*13,I0);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*14,I1);
	CaluMSMod(m_pComposeBuf,Uab);
	CaluMSMod(m_pComposeBuf+READ_POINT_NUM,Ubc);
	CaluMSMod(m_pComposeBuf+READ_POINT_NUM*2,Uca);
	//-------------------------------CT断线检查部分增加  上面计算I0,I1部分也相应作了修改--------
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*10,Ia_p);			
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*11,Ib_p);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*12,Ic_p);
	*RM_pnIam = IA;
	DB_LogWriteRM(RM_IAM, *RM_pnIam);
	*RM_pnIbm = IB;
	DB_LogWriteRM(RM_IBM, *RM_pnIbm);
	*RM_pnIcm = IC;
	DB_LogWriteRM(RM_ICM, *RM_pnIcm);
	CaluMaxMin();
	if((g_FaultOnFlag == TRUE)||ReadRelayWord(RW_52BF))
	{
		if(!ReadRelayWord(RW_ARCTBR_A))
		{
			ResetCTCheck(&RelCTA);
		}
		if(!ReadRelayWord(RW_ARCTBR_B))
		{
			ResetCTCheck(&RelCTB);
		}
		if(!ReadRelayWord(RW_ARCTBR_C))
		{
			ResetCTCheck(&RelCTC);
		}
	}
	else
	{
		CTCheck(&RelCTA);
		CTCheck(&RelCTB);
		CTCheck(&RelCTC);
	}

	if(*AS_pnPM == PHASEU) //接入的是相电压，用三表法测功率
	{
		CaluMSPQ(m_pDataBuf,m_pDataBuf+READ_POINT_NUM*7,*CF_pnAA,Pa,Qa);
		CaluMSPQ(m_pDataBuf+READ_POINT_NUM,m_pDataBuf+READ_POINT_NUM*8,*CF_pnBA,Pb,Qb);
		CaluMSPQ(m_pDataBuf+READ_POINT_NUM*2,m_pDataBuf+READ_POINT_NUM*9,*CF_pnCA,Pc,Qc);
	}
	else  //接入的是线电压，使用二表法测功率 p=Uab*Ia+Ucb*Ic
	{
		CaluMSPQ(m_pDataBuf,m_pDataBuf+READ_POINT_NUM*7,*CF_pnAA,Pa,Qa);
		CaluMSPQ(m_pDataBuf+READ_POINT_NUM,m_pDataBuf+READ_POINT_NUM*9,*CF_pnCA,Pc,Qc);
		Pb = 0;
		Qb = 0;
		Pc = (-1)*Pc;  //使用位操作
		Qc = (-1)*Qc;
		
	}
	//计算三相总加功率
	if(((*CF_pnPA)==0)||((*CF_pnPB)==0)||((*CF_pnPC)==0)||((*CF_pnQA)==0)||((*CF_pnQB)==0)||((*CF_pnQC)==0))
	{
		P = 0;
		Q = 0;
		cos = 0;
	}
	else
	{
	   QWORD SS;
	   P = (LLONG)Pa*10000/(*CF_pnPA)+(LLONG)Pb*10000/(*CF_pnPB)+(LLONG)Pc*10000/(*CF_pnPC);
	   Q = (LLONG)Qa*10000/(*CF_pnQA)+(LLONG)Qb*10000/(*CF_pnQB)+(LLONG)Qc*10000/(*CF_pnQC);
	   SS = P*P+Q*Q;
	   S = Evolution64(SS);
	   if(S<=20)
			cos = 0;
	   else
			cos = 10000*P/(LONG)S;
	   if((ToAbs(cos))>9990)
			Q = 0;
	   if((ToAbs(cos))<10)
			P =0;
	   cos = cos/10;
	}

//向数据库中写入测量值	
/*
	Ua = AiBubbleSort(Ua,UaArray);
	if(Ua<=*CF_pnUaDRIFT)
		Ua = 0;
	DB_WriteAI(M_UA,Ua);
	
	Ub = AiBubbleSort(Ub,UbArray);
	if(Ub<=*CF_pnUbDRIFT)
		Ub = 0;
	DB_WriteAI(M_UB,Ub);

	Uc = AiBubbleSort(Uc,UcArray);
	if(Uc<=*CF_pnUcDRIFT)
		Uc = 0;
	DB_WriteAI(M_UC,Uc);

	U0 = AiBubbleSort(U0,U0Array);
	if(U0<=*CF_pnU0DRIFT)
		U0 = 0;
	DB_WriteAI(M_U0,U0);

	U1 = AiBubbleSort(U1,U1Array);
	if(U1<=*CF_pnUl1DRIFT)
		U1 = 0;
	DB_WriteAI(M_U1,U1);
	
	U2 = AiBubbleSort(U2,U2Array);
	if(U2<=*CF_pnUl2DRIFT)
		U2 = 0;
	DB_WriteAI(M_U2,U2);

	U3 = AiBubbleSort(U3,U3Array);
	if(U3<=*CF_pnUl3DRIFT)
		U3 = 0;
	DB_WriteAI(M_U3,U3);

	IA= AiBubbleSort(IA,IaArray);
	if(IA<=*CF_pnIaDRIFT)
		IA = 0;
	DB_WriteAI(M_IA,IA);

	IB= AiBubbleSort(IB,IbArray);
	if(IB<=*CF_pnIbDRIFT)
		IB = 0;
	DB_WriteAI(M_IB,IB);
	
	IC= AiBubbleSort(IC,IcArray);
	if(IC<=*CF_pnIcDRIFT)
		IC = 0;
	DB_WriteAI(M_IC,IC);
*/
	WriteAIData(M_UA,Ua,*CF_pnUaDRIFT,m_pBubbleData,UaArray,TRUE);
	WriteAIData(M_UB,Ub,*CF_pnUbDRIFT,m_pBubbleData,UbArray,TRUE);
	WriteAIData(M_UC,Uc,*CF_pnUcDRIFT,m_pBubbleData,UcArray,TRUE);
	WriteAIData(M_U0,U0,*CF_pnU0DRIFT,m_pBubbleData,U0Array,TRUE);
	WriteAIData(M_U1,U1,*CF_pnUl1DRIFT,m_pBubbleData,U1Array,TRUE);
	WriteAIData(M_U2,U2,*CF_pnUl2DRIFT,m_pBubbleData,U2Array,TRUE);	
	WriteAIData(M_U3,U3,*CF_pnUl3DRIFT,m_pBubbleData,U3Array,TRUE);
	WriteAIData(M_IA,IA,*CF_pnIaDRIFT,m_pBubbleData,IaArray,TRUE);
	WriteAIData(M_IB,IB,*CF_pnIbDRIFT,m_pBubbleData,IbArray,TRUE);
	WriteAIData(M_IC,IC,*CF_pnIcDRIFT,m_pBubbleData,IcArray,TRUE);
	WriteAIData(M_I0,I0,*CF_pnI0DRIFT,m_pBubbleData,I0Array,TRUE);
	WriteAIData(M_I1,I1,*CF_pnI1DRIFT,m_pBubbleData,I1Array,TRUE);
	
	WriteAIData(M_UAB,Uab,*CF_pnUaDRIFT,m_pBubbleData,UabArray,TRUE);
	WriteAIData(M_UBC,Ubc,*CF_pnUbDRIFT,m_pBubbleData,UbcArray,TRUE);
	WriteAIData(M_UCA,Uca,*CF_pnUcDRIFT,m_pBubbleData,UcaArray,TRUE);

	WriteAIData(M_P,P,m_pBubbleData,PArray);
	WriteAIData(M_Q,Q,m_pBubbleData,QArray);
	WriteAIData(M_COS,cos,m_pBubbleData,cosArray);


//积分电度计算
	if(DB_IsCaculateIntegral())
	{
		if(P>=0)
			m_AccKilowattP += P;
		if(Q>=0)			
			m_AccKilowattQ += Q;

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
	CaluUIAngle();
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
//装置启动后2秒开始测频
	static WORD freqcounter =0;
	if((freqcounter>10))
	{
		m_FreqFlag = TRUE;
	}		
	freqcounter++;	
	
	if(m_FreqFlag)
	{
		BOOL rc;
		rc = CalcImmeFrequence(dwADDataWritePtr,Ua,m_dwFreqMask,m_wFreqBuf,m_FreqCaluBuf,&m_dwFreq);
/*		if(rc)
		{
			BubbleSort(m_dwFreq,NUM_FREQ_PER_AITASK);
			m_dwPreFreq = m_dwFreq[NUM_FREQ_PER_AITASK/2]/10;
			
			//频率不在合理频率变化范围之内,取50Hz
			if(m_dwPreFreq>MAX_FREQUENCE||m_dwPreFreq<MIN_FREQUENCE) 
			{
				m_dwPreFreq = 50000;
//				ReportMsg("Error: Freq out limit,using default!");
			}
			g_dwFreqPWM = m_dwPreFreq*POINT_PER_PERIOD;
//			ReportMsg("freq = %d",m_dwPreFreq);
			ADStartFreqUpdate(g_dwFreqPWM);
		}
		else
		{
			m_dwPreFreq = 0;
			ADStartFreqUpdate(50000*POINT_PER_PERIOD);
		}		*/
		m_dwPreFreq = m_dwFreq;
		DB_WriteAI(M_F1,m_dwFreq);
	}
	
//	LEDOnOff(18, FALSE);
}
void CAIProc::CaluUIAngle(void)
{
	LONG angle;
	CaluBaseFourier_S15(m_pDataBuf,&UAM);	
	CaluModValue(&UAM);
	CaluAngle(&UAM,AngleTab);
	
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*7,&IAM);	
	CaluModValue(&IAM);
	CaluAngle(&IAM,AngleTab);
	angle = IAM.Angle - UAM.Angle;
	if(angle>0)
		*RM_pnAIma = angle;
	else
		*RM_pnAIma = angle+36000;
	DB_LogWriteRM(RM_AIMA, *RM_pnAIma);

	
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*8,&IBM);	
	CaluModValue(&IBM);
	CaluAngle(&IBM,AngleTab);	
	angle = IBM.Angle - UAM.Angle;
	if(angle>0)
		*RM_pnAImb = angle;
	else
		*RM_pnAImb = angle+36000;
	DB_LogWriteRM(RM_AIMB, *RM_pnAImb);


	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*9,&ICM);	
	CaluModValue(&ICM);
	CaluAngle(&ICM,AngleTab);	
	angle = ICM.Angle - UAM.Angle;
	if(angle>0)
		*RM_pnAImc = angle;
	else
		*RM_pnAImc = angle+36000;
	DB_LogWriteRM(RM_AIMC, *RM_pnAImc);
}
void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//设置自动校准标志

	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDriftAdjust = TRUE;
		UaDrift = 0;
		UbDrift = 0;
		UcDrift = 0;
		U0Drift = 0;
		U1Drift = 0;
		U2Drift = 0;
		U3Drift = 0;
		IADrift = 0;
		IBDrift = 0;
		ICDrift = 0;
		I0Drift = 0;
		I1Drift = 0;
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
	
	MidAdjUa = 0;
	MidAdjUb = 0;
	MidAdjUc = 0;
	MidAdjU0 = 0;
	MidAdjU1 = 0;
	MidAdjU2 = 0;
	MidAdjU3 = 0;
	MidAdjIA = 0;
	MidAdjIB = 0;
	MidAdjIC = 0;
	MidAdjI0 = 0;
	MidAdjI1 = 0;
	
	MidAdjUap = 0;
	MidAdjUbp = 0;
	MidAdjUcp = 0;
	MidAdjU0p = 0;
	MidAdjU1p = 0;
	MidAdjU2p = 0;
	MidAdjU3p = 0;
	MidAdjIap = 0;
	MidAdjIbp = 0;
	MidAdjIcp = 0;
	MidAdjI0p = 0;
	MidAdjI1p = 0;

	
	for(LONG i=0;i<39;i++) 
	{
		m_pAdjustData[i] =0;
	}
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		AdjUaArray[i] = 0;
		AdjUbArray[i] = 0;
		AdjUcArray[i] = 0;
		AdjU0Array[i] = 0;
		AdjU1Array[i] = 0;
		AdjU2Array[i] = 0;
		AdjU3Array[i] = 0;
		AdjIAArray[i] = 0;
		AdjIBArray[i] = 0;
		AdjICArray[i] = 0;
		AdjI0Array[i] = 0;
		AdjI1Array[i] = 0;		
		UapArray[i] = 0;
		UbpArray[i] = 0;
		UcpArray[i] = 0;
		U0pArray[i] = 0;
		U1pArray[i] = 0;
		U2pArray[i] = 0;
		U3pArray[i] = 0;
		IapArray[i] = 0;
		IbpArray[i] = 0;
		IcpArray[i] = 0;
		I0pArray[i] = 0;
		I1pArray[i] = 0;
	}
}
/**************************************************************************************************
* 功能: 计算遥测量有功，无功，功率因数
* 参数: 滤波后的数据缓冲区
* 返回: 无
* 备注:
***************************************************************************************************/

void CAIProc::WriteMSComposeBuf(LONG *ComposeBuf,const LONG*DataBuf)
{	
	if(*AS_pnPM == PHASEU) //接入的是相电压
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			ComposeBuf[i]=DataBuf[i]-DataBuf[i+READ_POINT_NUM];
			ComposeBuf[i+READ_POINT_NUM]=DataBuf[i+READ_POINT_NUM]-DataBuf[i+READ_POINT_NUM*2];
			ComposeBuf[i+READ_POINT_NUM*2]=DataBuf[i+READ_POINT_NUM*2]-DataBuf[i];
		}
	}
	else
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			ComposeBuf[i]=DataBuf[i];
			ComposeBuf[i+READ_POINT_NUM]=DataBuf[i+READ_POINT_NUM];
			ComposeBuf[i+READ_POINT_NUM*2]=DataBuf[i+READ_POINT_NUM*2];
		}
	}
}
/**************************************************************************************************
* 功能: 计算遥测量有功，无功，功率因数
* 参数: 滤波后的数据缓冲区
* 返回: 无
* 备注:
***************************************************************************************************/
void CAIProc::CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ)
{
	LLONG   p 	=0;
	LLONG   q	=0;
	QWORD 	s	=0;
	DWORD 	s2	=0;
	LONG    tp 	= 0;
	LONG    tq 	= 0;
	//功率计算
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		p += UBuf[1+i]*IBuf[1+i];
		q += IBuf[1+i]*(UBuf[i]-UBuf[2+i]);
	}
	tp = p/3200;
	tq = q/SINWTS;
	//功率校正
	MSP = tp + tq*DACoef/10000;     
	MSQ = tq - tp*DACoef/10000;  
	//功率因数计算
}
/************************************************************************************************
*功能: 处理系数校正中的计算
*参数: 
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjustCalu(void)
{
	
	ReadSampleData(m_pCoefAdjustBuf,m_CoefAdjustMask,0);		//读取原始采样数据	

	CaluBaseFourier_S15(m_pCoefAdjustBuf,&Uap);
	CaluModValue(&Uap);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&Ubp);
	CaluModValue(&Ubp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&Ucp);
	CaluModValue(&Ucp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&U0p);
	CaluModValue(&U0p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&U1p);
	CaluModValue(&U1p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&U2p);
	CaluModValue(&U2p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&U3p);
	CaluModValue(&U3p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*10,&Iap);
	CaluModValue(&Iap);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*11,&Ibp);
	CaluModValue(&Ibp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*12,&Icp);
	CaluModValue(&Icp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*13,&I0p);
	CaluModValue(&I0p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*14,&I1p);
	CaluModValue(&I1p);


	Filter(m_pCoefAdjustBuf);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*2);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*3);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*4);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*5);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*6);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*7);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*8);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*9);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*13);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*14);

	CaluMSMod(m_pCoefAdjustBuf,AdjUa);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,AdjUb);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,AdjUc);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,AdjU0);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,AdjU1);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,AdjU2);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,AdjU3);	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,AdjIA);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,AdjIB);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,AdjIC);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*13,AdjI0);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*14,AdjI1);

}
/************************************************************************************************
*功能: 系数校正
*参数: 滤波后数据缓冲区
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	//系数整定之前需将装置整定为相电压模式
	if(*AS_pnPM != PHASEU)
	{
		CoefAdjustEnd(FAILURE);
		return;
	}
	if(m_dwAdjustCnt>99)
	{
		if(m_bIsDriftAdjust == TRUE)
		{			
			UaDrift = m_pAdjustData[27]; 
			UbDrift = m_pAdjustData[28]; 
			UcDrift = m_pAdjustData[29]; 
			U0Drift = m_pAdjustData[30]; 
			U1Drift = m_pAdjustData[31]; 
			U2Drift = m_pAdjustData[32]; 
			U3Drift = m_pAdjustData[33]; 
			IADrift = m_pAdjustData[34]; 
			IBDrift = m_pAdjustData[35]; 
			ICDrift = m_pAdjustData[36]; 
			I0Drift = m_pAdjustData[37]; 
			I1Drift = m_pAdjustData[38]; 
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{
			//注意U3/U4的PT可能不安装，直接等于U1的系数
			LONG Value[42];			
			Value[0]	= (m_pAdjustData[0]+m_pAdjustData[1]+m_pAdjustData[2]+m_pAdjustData[4]+m_pAdjustData[5]+m_pAdjustData[6])/(m_dwSourceU*6); //Up    			//Ip
			Value[1]	= m_pAdjustData[3]/m_dwSourceU;						//U0
			Value[2]	= (m_pAdjustData[7]+m_pAdjustData[8]+m_pAdjustData[9])/(m_dwSourceI*30);//Ip
			Value[3]	= m_pAdjustData[10]/10;								//I0	   yanxs  20140904
			Value[4]	= m_pAdjustData[11]/10;								//I1
			Value[5]	= m_pAdjustData[12]/m_dwSourceU;					//Uam
			Value[6]	= m_pAdjustData[13]/m_dwSourceU;					//Ubm
			Value[7]	= m_pAdjustData[14]/m_dwSourceU;					//Ucm
			Value[8]	= m_pAdjustData[15]/m_dwSourceU;					//U0m
			Value[9]	= m_pAdjustData[16]/m_dwSourceU;					//U1m
			Value[10]	= m_pAdjustData[17]/m_dwSourceU;					//U2m
			Value[11]	= m_pAdjustData[18]/m_dwSourceU;					//U3m			
			Value[12]	= m_pAdjustData[19]/(m_dwSourceI*10);				//IAm，
			Value[13]	= m_pAdjustData[20]/(m_dwSourceI*10);				//IBm，
			Value[14]	= m_pAdjustData[21]/(m_dwSourceI*10);				//ICm
			Value[15]	= m_pAdjustData[22]/10;								//I0m		
			Value[16]	= m_pAdjustData[23]/10;								//I1m
			Value[17]	= m_pAdjustData[24]/(m_dwSourceU*m_dwSourceI);		//pa，保存100W的系数
			Value[18] 	= Value[17];										//qa，保存100W的系数
			Value[19] 	= m_pAdjustData[25]/(m_dwSourceU*m_dwSourceI);		//pb
			Value[20] 	= Value[19];										//qb
			Value[21] 	= m_pAdjustData[26]/(m_dwSourceU*m_dwSourceI);		//pc
			Value[22] 	= Value[21];;										//qc
			Value[23] 	= AdjAA;											//角差A
			Value[24] 	= AdjBA;											//角差B
			Value[25] 	= AdjCA;											//角差C
			Value[26]	= 1000;
			Value[27]	= 100;
			Value[28]	= 10;
			Value[29]	= 1;
			Value[30]	= IADrift;
			Value[31]	= IBDrift;	
			Value[32]	= ICDrift;
			Value[33]	= I0Drift;	
			Value[34]	= I1Drift;
			Value[35]	= UaDrift;
			Value[36]	= UbDrift;
			Value[37]	= UcDrift;
			Value[38]	= U0Drift;
			Value[39]	= U1Drift;
			Value[40]	= U2Drift;
			Value[41]	= U3Drift;
			
			
				
			DB_SetCFValue(CF_VALUE_ALL, Value);					//写入系数值
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);		//写入系数确认
			CoefAdjustEnd(SUCCESS);
			return;
		}
	
	}
	
	CoefAdjustCalu();

	if(!m_bIsSourceOK)  //首先校正角差及电压电流源检测
	{
		if(m_bIsDriftAdjust) //死区值校正源判断
		{	
			if((Uap.Mod>(*CF_pnUp/200))||(Ubp.Mod>(*CF_pnUp/200))||(Ucp.Mod>(*CF_pnUp/200))||(U0p.Mod>(*CF_pnUp/200))||\
				(U1p.Mod>(*CF_pnUp/200))||(U2p.Mod>(*CF_pnUp/200))||(U3p.Mod>(*CF_pnUp/200)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((Iap.Mod>(*CF_pnIp/5))||(Ibp.Mod>(*CF_pnIp/5))||(Icp.Mod>(*CF_pnIp/5))||(I0p.Mod>(*CF_pnIp/5))||(I1p.Mod>(*CF_pnIp/5)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((AdjIA>(*CF_pnIAM/5))||(AdjIB>(*CF_pnIBM/5))||(AdjIC>(*CF_pnICM/5)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		else	//系数值校正源判断
		{
			LONG upLimit,downLimit;
			upLimit = *CF_pnUp*m_dwSourceU*11/1000;
			downLimit = *CF_pnUp*m_dwSourceU*9/1000;			
			if((Uap.Mod>upLimit)||(Uap.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((Ubp.Mod>upLimit)||(Ubp.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((Ucp.Mod>upLimit)||(Ucp.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((U0p.Mod>upLimit)||(U0p.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((U1p.Mod>upLimit)||(U1p.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((U2p.Mod>upLimit)||(U2p.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((U3p.Mod>upLimit)||(U3p.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			upLimit = *CF_pnIp*m_dwSourceI*11/100;
			downLimit = *CF_pnIp*m_dwSourceI*9/100;
			if((Iap.Mod>upLimit)||(Iap.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((Ibp.Mod>upLimit)||(Ibp.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((Icp.Mod>upLimit)||(Icp.Mod<downLimit))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
//			if((I0p.Mod>*CF_pnI0*m_dwSourceI*11/100)||(I0p.Mod<*CF_pnI0*m_dwSourceI*9/100))
//			{
//				CoefAdjustEnd(FAILURE);
//				return;
//			}
//			if((I1p.Mod>*CF_pnI1*m_dwSourceI*11/100)||(I1p.Mod<*CF_pnI1*m_dwSourceI*9/100))
			if((I1p.Mod>*CF_pnI1*11/100)||(I1p.Mod<*CF_pnI1*9/100))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((AdjIA>(*CF_pnIAM*m_dwSourceI*11/100))||(AdjIA<(*CF_pnIAM*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((AdjIB>(*CF_pnIBM*m_dwSourceI*11/100))||(AdjIB<(*CF_pnIBM*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((AdjIC>(*CF_pnICM*m_dwSourceI*11/100))||(AdjIC<(*CF_pnICM*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			for(int i=1;i<15;i++)
			{
				if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))					
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			
			CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*7,0,AdjPa,AdjQa); 	
			CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM,m_pCoefAdjustBuf+READ_POINT_NUM*8,0,AdjPb,AdjQb);
			CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_pCoefAdjustBuf+READ_POINT_NUM*9,0,AdjPc,AdjQc);
			
			if((AdjQa>*CF_pnQA/3)||(AdjQb>*CF_pnQB/3)||(AdjQc>*CF_pnQC/3))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(AdjQa == 0)
				AdjAA = 0;
			else
				AdjAA = AdjQa*10000/((LONG)Evolution64((LLONG)AdjPa*AdjPa+(LLONG)AdjQa*AdjQa)); 	
			
			if(AdjQb == 0)
				AdjBA = 0;
			else
				AdjBA = AdjQb*10000/((LONG)Evolution64((LLONG)AdjPb*AdjPb+(LLONG)AdjQb*AdjQb)); 		
			
			if(AdjQc == 0)
				AdjCA = 0;
			else
				AdjCA = AdjQc*10000/((LONG)Evolution64((LLONG)AdjPc*AdjPc+(LLONG)AdjQc*AdjQc)); 		
		}	
		m_bIsSourceOK = TRUE;
		return;
	}
	CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*7,AdjAA,AdjPa,AdjQa); 	
	CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM,m_pCoefAdjustBuf+READ_POINT_NUM*8,AdjBA,AdjPb,AdjQb);
	CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_pCoefAdjustBuf+READ_POINT_NUM*9,AdjCA,AdjPc,AdjQc);

	if(m_bIsDriftAdjust) //取得最大零漂值
	{
		if(AdjUa>MidAdjUa)
			MidAdjUa = AdjUa;
		if(AdjUb>MidAdjUb)
			MidAdjUb = AdjUb;
		if(AdjUc>MidAdjUc)
			MidAdjUc = AdjUc;
		if(AdjU0>MidAdjU0)
			MidAdjU0 = AdjU0;
		if(AdjU1>MidAdjU1)
			MidAdjU1 = AdjU1;
		if(AdjU2>MidAdjU2)
			MidAdjU2 = AdjU2;
		if(AdjU3>MidAdjU3)
			MidAdjU3 = AdjU3;
		if(AdjIA>MidAdjIA)
			MidAdjIA = AdjIA;
		if(AdjIB>MidAdjIB)
			MidAdjIB = AdjIB;
		if(AdjIC>MidAdjIC)
			MidAdjIC = AdjIC;
		if(AdjI0>MidAdjI0)
			MidAdjI0 = AdjI0;
		if(AdjI1>MidAdjI1)
			MidAdjI1 = AdjI1;
	}
	else
	{
		MidAdjUa = AiBubbleSort(AdjUa,AdjUaArray);
		MidAdjUb = AiBubbleSort(AdjUb,AdjUbArray);
		MidAdjUc = AiBubbleSort(AdjUc,AdjUcArray);
		MidAdjU0 = AiBubbleSort(AdjU0,AdjU0Array);
		MidAdjU1 = AiBubbleSort(AdjUa,AdjU1Array);
		MidAdjU2 = AiBubbleSort(AdjUa,AdjU2Array);
		MidAdjU3 = AiBubbleSort(AdjUa,AdjU3Array);		
		MidAdjIA = AiBubbleSort(AdjIA,AdjIAArray);
		MidAdjIB = AiBubbleSort(AdjIB,AdjIBArray);
		MidAdjIC = AiBubbleSort(AdjIC,AdjICArray);
		MidAdjI0 = AiBubbleSort(AdjI0,AdjI0Array);
		MidAdjI1 = AiBubbleSort(AdjI1,AdjI1Array);
		
		MidAdjUap = AiBubbleSort(Uap.Mod,UapArray);
		MidAdjUbp = AiBubbleSort(Ubp.Mod,UbpArray);
		MidAdjUcp = AiBubbleSort(Ucp.Mod,UcpArray);
		MidAdjU0p = AiBubbleSort(U0p.Mod,U0pArray);
		MidAdjU1p = AiBubbleSort(U1p.Mod,U1pArray);
		MidAdjU2p = AiBubbleSort(U2p.Mod,U2pArray);
		MidAdjU3p = AiBubbleSort(U3p.Mod,U3pArray);
		MidAdjIap = AiBubbleSort(Iap.Mod,IapArray);
		MidAdjIbp = AiBubbleSort(Ibp.Mod,IbpArray);
		MidAdjIcp = AiBubbleSort(Icp.Mod,IcpArray);
		MidAdjI0p = AiBubbleSort(I0p.Mod,I0pArray);
		MidAdjI1p = AiBubbleSort(I1p.Mod,I1pArray);
	}
	
	if(m_dwWaitCnt < (2000/AI_TIMER_PERIOD)) //系数校正，源校验正确后等待两秒钟
	{
		m_dwWaitCnt++;
		return;
	}
	if(m_bIsDriftAdjust)
	{
		
		m_pAdjustData[27]  = MidAdjUa;
		m_pAdjustData[28]  = MidAdjUb;
		m_pAdjustData[29]  = MidAdjUc;
		m_pAdjustData[30]  = MidAdjU0;
		m_pAdjustData[31]  = MidAdjU1;
		m_pAdjustData[32]  = MidAdjU2;
		m_pAdjustData[33]  = MidAdjU3;
		m_pAdjustData[34]  = MidAdjIA;
		m_pAdjustData[35]  = MidAdjIB;
		m_pAdjustData[36]  = MidAdjIC;
		m_pAdjustData[37]  = MidAdjI0;
		m_pAdjustData[38]  = MidAdjI1;
	}
	else
	{
		m_pAdjustData[0] += MidAdjUap;
		m_pAdjustData[1] += MidAdjUbp;
		m_pAdjustData[2] += MidAdjUcp;
		m_pAdjustData[3] += MidAdjU0p;
		m_pAdjustData[4] += MidAdjU1p;
		m_pAdjustData[5] += MidAdjU2p;
		m_pAdjustData[6] += MidAdjU3p;
		m_pAdjustData[7] += MidAdjIap;
		m_pAdjustData[8] += MidAdjIbp;
		m_pAdjustData[9] += MidAdjIcp;
		m_pAdjustData[10] += MidAdjI0p;
		m_pAdjustData[11] += MidAdjI1p;

		m_pAdjustData[12] += MidAdjUa;
		m_pAdjustData[13] += MidAdjUb;
		m_pAdjustData[14] += MidAdjUc;
		m_pAdjustData[15] += MidAdjU0;
		m_pAdjustData[16] += MidAdjU1;
		m_pAdjustData[17] += MidAdjU2;
		m_pAdjustData[18] += MidAdjU3;
		m_pAdjustData[19] += MidAdjIA;
		m_pAdjustData[20] += MidAdjIB;
		m_pAdjustData[21] += MidAdjIC;
		m_pAdjustData[22] += MidAdjI0;
		m_pAdjustData[23] += MidAdjI1;
		m_pAdjustData[24] += AdjPa;
		m_pAdjustData[25] += AdjPb;
		m_pAdjustData[26] += AdjPc;
	}
	
	m_dwAdjustCnt++;
}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<39;i++) 
	{
		m_pAdjustData[i] =0;
	}
	
	//角差校正标志清零
	m_bIsSourceOK = FALSE;

	//系数校正计算次数计数器清零
	m_dwAdjustCnt=0;

	//清系数校正任务标志
	m_bIsCoefAdjust = FALSE;

	//系数校正等待时间清零
	m_dwWaitCnt = 0;
	
	//向液晶任务发送消息，传递校正结果
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
void CAIProc::InitCTACheck()
{
	RelCTA.pRelCTBrokCFG = CFG_pbyCTBR;				//CT断线配置 
	RelCTA.pRelCurSet = &CTCurSet;					//电流门槛值 
	RelCTA.pRelTimeSet = &CTTimeSet;				//告警时间 
	RelCTA.dwSysTimer = &dwRelayMSCnt;				//系统定时器 

	RelCTA.pRelCurMax = &IaMax;						//判断用大电流 
	RelCTA.pRelCurMin = &IaMin;						//判断用小电流 

	RelCTA.StartRelayWord = RW_PICTBR_A;			//CT断线入段继电器字 
	RelCTA.ActRelayWord = RW_ARCTBR_A;				//CT断线动作继电器字 	
	RelCTA.wRelRptNum = R_CTBR_A;					//CT断线动作报告号
	RelCTA.wRelResRptNum = R_CTBR_A_RES;			//CT断线动作报告号
	
	RelCTA.byRelFCounter = 0;						//故障计数器 
	RelCTA.byRelRCounter = 0;						//返回计数器 
	RelCTA.dwRelTimer = 0;							//CT断线定时器 	
	RelCTA.wExcepWordNum = 6;						//TRelCTBrokPar CT断线 本结构对象计算CRC校验时需排除在外的字节数，本结构为6，大小为18 
}
void CAIProc::InitCTBCheck()
{
	RelCTB.pRelCTBrokCFG = CFG_pbyCTBR;				//CT断线配置 
	RelCTB.pRelCurSet = &CTCurSet;					//电流门槛值 
	RelCTB.pRelTimeSet = &CTTimeSet;				//告警时间 
	RelCTB.dwSysTimer = &dwRelayMSCnt;				//系统定时器 

	RelCTB.pRelCurMax = &IbMax;						//判断用大电流 
	RelCTB.pRelCurMin = &IbMin;						//判断用小电流 

	RelCTB.StartRelayWord = RW_PICTBR_B;			//CT断线入段继电器字 
	RelCTB.ActRelayWord = RW_ARCTBR_B;				//CT断线动作继电器字 	
	RelCTB.wRelRptNum = R_CTBR_B;					//CT断线动作报告号
	RelCTB.wRelResRptNum = R_CTBR_B_RES;			//CT断线动作报告号
	
	RelCTB.byRelFCounter = 0;						//故障计数器 
	RelCTB.byRelRCounter = 0;						//返回计数器 
	RelCTB.dwRelTimer = 0;							//CT断线定时器 	
	RelCTB.wExcepWordNum = 6;						//TRelCTBrokPar CT断线 本结构对象计算CRC校验时需排除在外的字节数，本结构为6，大小为18 
}
void CAIProc::InitCTCCheck()
{
	RelCTC.pRelCTBrokCFG = CFG_pbyCTBR;				//CT断线配置 
	RelCTC.pRelCurSet = &CTCurSet;					//电流门槛值 
	RelCTC.pRelTimeSet = &CTTimeSet;				//告警时间 
	RelCTC.dwSysTimer = &dwRelayMSCnt;				//系统定时器 

	RelCTC.pRelCurMax = &IcMax;						//判断用大电流 
	RelCTC.pRelCurMin = &IcMin;						//判断用小电流 

	RelCTC.StartRelayWord = RW_PICTBR_C;			//CT断线入段继电器字 
	RelCTC.ActRelayWord = RW_ARCTBR_C;				//CT断线动作继电器字 	
	RelCTC.wRelRptNum = R_CTBR_C;					//CT断线动作报告号
	RelCTC.wRelResRptNum = R_CTBR_C_RES;			//CT断线动作报告号
	
	RelCTC.byRelFCounter = 0;						//故障计数器 
	RelCTC.byRelRCounter = 0;						//返回计数器 
	RelCTC.dwRelTimer = 0;							//CT断线定时器 	
	RelCTC.wExcepWordNum = 6;						//TRelCTBrokPar CT断线 本结构对象计算CRC校验时需排除在外的字节数，本结构为6，大小为18 
}

void CAIProc::CaluMaxMin()
{
	LONG tempIA,tempIB,tempIC;	
	CTCurSet = (*AS_pnPCTRatio)*CTBRCHECKVALUE;
	IaMax = (*AS_pnPCTRatio)*Ia_p*CTBRKPM;
	IbMax = (*AS_pnPCTRatio)*Ib_p*CTBRKPM;
	IcMax = (*AS_pnPCTRatio)*Ic_p*CTBRKPM;
	IaMin = (*AS_pnMCTRatio)*IA;
	IbMin = (*AS_pnMCTRatio)*IB;
	IcMin = (*AS_pnMCTRatio)*IC;
	if( IaMax < IaMin)
	{
		tempIA = IaMax;
		IaMax = IaMin;
		IaMin = tempIA;
	}
	
	if( IbMax < IbMin)
	{
		tempIB = IbMax;
		IbMax = IbMin;
		IbMin = tempIB;
	}
	
	if( IcMax < IcMin)
	{
		tempIC = IcMax;
		IcMax = IcMin;
		IcMin = tempIC;
	}
}















