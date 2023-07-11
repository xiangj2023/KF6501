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
#include "hmiapp.h"


DWORD dwAITaskID;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD g_hLCDTaskID;
extern BOOL g_FaultOnFlag;
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
LONG teset[20];

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
	m_bHaveAdjustAngle=FALSE;
	m_lDACoef =0;
	
	m_AIDataMask = ( (0x01<<CHANNEL_6560_U1)|(0x01<<CHANNEL_6560_U2)|(0x01<<CHANNEL_6560_Ud1)|(0x01<<CHANNEL_6560_Ud2)|\
						(0x01<<CHANNEL_6560_Ud3)|(0x01<<CHANNEL_6560_IM)|(0x01<<CHANNEL_6560_IP)|(0x01<<CHANNEL_6560_I1)|\
						(0x01<<CHANNEL_6560_I2)|(0x01<<CHANNEL_6560_I3));
	m_CoefAdjustMask = ( (0x01<<CHANNEL_6560_U1)|(0x01<<CHANNEL_6560_U2)|(0x01<<CHANNEL_6560_Ud1)|(0x01<<CHANNEL_6560_Ud2)|\
						(0x01<<CHANNEL_6560_Ud3)|(0x01<<CHANNEL_6560_IM)|(0x01<<CHANNEL_6560_IP)|(0x01<<CHANNEL_6560_I1)|\
						(0x01<<CHANNEL_6560_I2)|(0x01<<CHANNEL_6560_I3));
	
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*10);
	m_pCoefAdjustBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*10);
	m_pComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*10);
	
	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*10);
	
	if(m_pComposeBuf == NULL)
	{
		LogError("CAIProc,m_pComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM);
	
	for(LONG i=0;i<18;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		UMArray[i]=0;
		U1MArray[i]=0;
		U2MArray[i]=0;
		Ud1MArray[i]=0;
		Ud2MArray[i]=0;
		Ud3MArray[i]=0;
		IMArray[i]=0;
		I1MArray[i]=0;
		I2MArray[i]=0;
		I3MArray[i]=0;
		PArray[i]=0;
		QArray[i]=0;
		COSArray[i]=0;
		IH1MArray[i]=0;
		IH2MArray[i]=0;
		IH3MArray[i]=0;
		IH5MArray[i]=0;
		IH7MArray[i]=0;
		IH9MArray[i]=0;
		IH11MArray[i]=0;
		
	}
	
	//-----------CT断线检测部分初始化-----------------
	InitCTI();
	CTCurSet = (*AS_pnIPCTRatio)*CTBRCHECKVALUE;
	CTTimeSet = 3000;
	//--------------------------------------------------
	//初始化系数校准标志
	m_bIsCoefAdjust = FALSE;
	m_dwWaitCnt = 0;
	m_dwAdjustCnt=0;
	dwAITaskID = GetTaskId();
	//设置AI定时器
	SetTimer(1,AI_TIMER_PERIOD);
}
void CAIProc::WriteMSComposeData(LONG *caludatabuf,LONG *originaldatabuf)
{	
	if(*AS_pnPM == BTMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			caludatabuf[i] = originaldatabuf[i];
		}
	}
	else if(*AS_pnPM == ATMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			caludatabuf[i] = originaldatabuf[i]-originaldatabuf[i+READ_POINT_NUM];
		}		
	}
	
}

/*********************************************************************************
*功能: AI定时消息响应
*输入: 定时器ID
*输出: 无
*返回: 无
**************************************************************************************/

void CAIProc::OnTimeOut(DWORD id)
{	
	//开始系数校正
	if(m_bIsCoefAdjust == TRUE)
	{
		CoefAdjust();	
		return;
	}
	//读取原始采样数据
	ReadSampleData(m_pDataBuf,m_AIDataMask,0);		
	WriteMSComposeData(m_pComposeBuf,m_pDataBuf);
	//滤波
	for(LONG i=0;i<10;i++)
	{
		Filter(m_pDataBuf+READ_POINT_NUM*i);
	}
	Filter(m_pComposeBuf);

	
	//均方根法求测量值的模值
	CaluMSMod(m_pComposeBuf,m_Um);
	CaluMSMod(m_pDataBuf,m_U1m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,m_U2m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,m_Ud1m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,m_Ud2m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,m_Ud3m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,m_Im);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,m_I1m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,m_I2m);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,m_I3m);
	//************************************CT断线检查部分*************************
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,m_Ip);
	*RM_pnIm = m_Im;
	DB_LogWriteRM(RM_IM, *RM_pnIm);
	CaluCTMaxMin();
	if((g_FaultOnFlag == TRUE)||(ReadRelayWord(RW_52BF)))
	{	
		if(!ReadRelayWord(RW_ARCTBR))
			ResetCTCheck(&RelCTI);
	}
	else
	{
		CTCheck(&RelCTI);
	}

	//傅里叶算法求谐波分量,使用保护电流通道计算谐波
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*6,&m_Ih1m);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*6,&m_Ih2m);
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*6,&m_Ih3m);
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*6,&m_Ih5m);
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*6,&m_Ih7m);
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*6,&m_Ih9m);
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*6,&m_Ih11m);

	CaluModValue(&m_Ih1m);
	CaluModValue(&m_Ih2m);
	CaluModValue(&m_Ih3m);
	CaluModValue(&m_Ih5m);
	CaluModValue(&m_Ih7m);
	CaluModValue(&m_Ih9m);
	CaluModValue(&m_Ih11m);

	//谐波校正
	m_Ih2m.Mod = m_Ih2m.Mod*CurHarmCal[0]/1000;
	m_Ih3m.Mod = m_Ih3m.Mod*CurHarmCal[1]/1000;
	m_Ih5m.Mod = m_Ih5m.Mod*CurHarmCal[2]/1000;
	m_Ih7m.Mod = m_Ih7m.Mod*CurHarmCal[3]/1000;
	m_Ih9m.Mod = m_Ih9m.Mod*CurHarmCal[4]/1000;
	m_Ih11m.Mod = m_Ih11m.Mod*CurHarmCal[5]/1000;	
	//功率计算
	CaluMSPQ(m_pComposeBuf,m_pDataBuf+READ_POINT_NUM*5,*CF_pnA,m_lMSP,m_lMSQ,m_lMSCOS);		//计算有功、无功、功率因数

	//写入遥测值
	m_Um = AiBubbleSort(m_Um,UMArray);
	if(m_Um<=*CF_pnDRIFTU)		
		m_Um = 0;
	DB_WriteAI(M_UM,m_Um);

	m_U1m = AiBubbleSort(m_U1m,U1MArray);
	if(m_U1m<=*CF_pnDRIFTU1)
		m_U1m = 0;
	DB_WriteAI(M_U1M,m_U1m);

	m_U2m = AiBubbleSort(m_U2m,U2MArray);
	if(m_U2m<=*CF_pnDRIFTU2)
		m_U2m = 0;
	DB_WriteAI(M_U2M,m_U2m);

	m_Ud1m = AiBubbleSort(m_Ud1m,Ud1MArray);
	if(m_Ud1m<=*CF_pnDRIFTUD1)
		m_Ud1m = 0;
	DB_WriteAI(M_UD1M,m_Ud1m);

	m_Ud2m = AiBubbleSort(m_Ud2m,Ud2MArray);
	if(m_Ud2m<=*CF_pnDRIFTUD2)
		m_Ud2m = 0;
	DB_WriteAI(M_UD2M,m_Ud2m);	

	m_Ud3m = AiBubbleSort(m_Ud3m,Ud3MArray);
	if(m_Ud3m<=*CF_pnDRIFTUD3)
		m_Ud3m = 0;
	DB_WriteAI(M_UD3M,m_Ud3m);

	m_Im = AiBubbleSort(m_Im,IMArray);
	if(m_Im<=*CF_pnDRIFTIM)
		m_Im = 0;
	DB_WriteAI(M_IM,m_Im);

	m_I1m = AiBubbleSort(m_I1m,I1MArray);
	if(m_I1m<=*CF_pnDRIFTI1)
		m_I1m = 0;
	DB_WriteAI(M_I1M,m_I1m);

	m_I2m = AiBubbleSort(m_I2m,I2MArray);
	if(m_I2m<=*CF_pnDRIFTI2)
		m_I2m = 0;
	DB_WriteAI(M_I2M,m_I2m);
	
	m_I3m = AiBubbleSort(m_I3m,I3MArray);
	if(m_I3m<=*CF_pnDRIFTI3)
		m_I3m = 0;
	DB_WriteAI(M_I3M,m_I3m);
	
	WriteAIData(M_P,m_lMSP,m_pBubbleData,PArray);
	WriteAIData(M_Q,m_lMSQ,m_pBubbleData,QArray);
	WriteAIData(M_COS,m_lMSCOS,m_pBubbleData,COSArray);
	WriteAIData(M_IH1,m_Ih1m.Mod,m_pBubbleData,IH1MArray);
	WriteAIData(M_IH2,m_Ih2m.Mod,m_pBubbleData,IH2MArray);
	WriteAIData(M_IH3,m_Ih3m.Mod,m_pBubbleData,IH3MArray);
	WriteAIData(M_IH5,m_Ih5m.Mod,m_pBubbleData,IH5MArray);
	WriteAIData(M_IH7,m_Ih7m.Mod,m_pBubbleData,IH7MArray);
	WriteAIData(M_IH9,m_Ih9m.Mod,m_pBubbleData,IH9MArray);
	WriteAIData(M_IH11,m_Ih11m.Mod,m_pBubbleData,IH11MArray);
	
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
	
}

void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//设置自动校准标志

	
	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDriftAdjust = TRUE;	
		
		m_lDriftU  = 0;
		m_lDriftU1 = 0;
		m_lDriftU2 = 0;
		m_lDriftUd1 = 0;
		m_lDriftUd2 = 0;
		m_lDriftUd3 = 0;
		m_lDriftIm = 0;
		m_lDriftI1 = 0;
		m_lDriftI2 = 0;
		m_lDriftI3 = 0;
	}
	else
		m_bIsDriftAdjust = FALSE;

	dwMsgID = arg;
	
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;	
	m_bIsCoefAdjust = TRUE;
	m_bHaveAdjustAngle = FALSE;
/*	m_lDriftU  = 0;
	m_lDriftU1 = 0;
	m_lDriftU2 = 0;
	m_lDriftUd1 = 0;
	m_lDriftUd2 = 0;
	m_lDriftUd3 = 0;
	m_lDriftIm = 0;
	m_lDriftI1 = 0;
	m_lDriftI2 = 0;
	m_lDriftI3 = 0;
*/
	
	m_lMidUp = 0;
	m_lMidU1p = 0;
	m_lMidU2p = 0;
	m_lMidUd1p = 0;
	m_lMidUd2p = 0;
	m_lMidUd3p = 0;
	m_lMidIp = 0;
	m_lMidI1p = 0;
	m_lMidI2p = 0;
	m_lMidI3p = 0;
	m_lMidUm = 0;
	m_lMidU1m = 0;
	m_lMidU2m = 0;
	m_lMidUd1m = 0;
	m_lMidUd2m = 0;
	m_lMidUd3m = 0;
	m_lMidIm = 0;
	m_lMidI1m = 0;
	m_lMidI2m = 0;
	m_lMidI3m = 0;
	m_lMidP = 0;
	m_lMidQ = 0;
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		AArrayUp[i] =0;
		AArrayU1p[i] =0;
		AArrayU2p[i] =0;
		AArrayUd1p[i] =0;
		AArrayUd2p[i] =0;
		AArrayUd3p[i] =0;
		AArrayIp[i] =0;
		AArrayI1p[i] =0;
		AArrayI2p[i] =0; 
		AArrayI3p[i] =0;
		AArrayUm[i] =0;
		AArrayU1m[i] =0;
		AArrayU2m[i] =0;
		AArrayUd1m[i] =0;
		AArrayUd2m[i] =0;
		AArrayUd3m[i] =0;
		AArrayIm[i] =0;
		AArrayI1m[i] =0;
		AArrayI2m[i] =0;
		AArrayI3m[i] =0;
		AArrayP[i] =0;
		AArrayQ[i] =0;
	}
	
	for(LONG i=0;i<22;i++)     
		m_pAdjustData[i]=0;
}
/*
void CAIProc::OnAutoAdjustStep2(DWORD arg,DWORD Uval,DWORD Ival)
{
	//设置自动校准标志
	m_bIsCoefAdjust = TRUE;
	m_bIsDriftAdjust = FALSE;
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	
	m_lMidUp = 0;
	m_lMidU1p = 0;
	m_lMidU2p = 0;
	m_lMidUd1p = 0;
	m_lMidUd2p = 0;
	m_lMidUd3p = 0;
	m_lMidIp = 0;
	m_lMidI1p = 0;
	m_lMidI2p = 0;
	m_lMidI3p = 0;
	m_lMidUm = 0;
	m_lMidU1m = 0;
	m_lMidU2m = 0;
	m_lMidUd1m = 0;
	m_lMidUd2m = 0;
	m_lMidUd3m = 0;
	m_lMidIm = 0;
	m_lMidI1m = 0;
	m_lMidI2m = 0;
	m_lMidI3m = 0;
	m_lMidP = 0;
	m_lMidQ = 0;
	for(LONG i=0;i<22;i++)     
		m_pAdjustData[i]=0;
}
*/
/**************************************************************************************************
* 功能: 计算遥测量有功，无功，功率因数
* 参数: 滤波后的数据缓冲区
* 返回: 无
* 备注:
***************************************************************************************************/
void CAIProc::CaluMSPQ(LONG *UDataBuf,LONG *IDataBuf,LONG DACoef, LONG &MSP,LONG &MSQ,LONG &MSCOS,BOOL coef)
{
	QWORD s		=0;
	DWORD s2	=0;
	LLONG    tp 	= 0;
	LLONG    tq 	= 0;
	//功率计算
	for(LONG i=0;i<32;i++)
	{
		tp += UDataBuf[1+i]*IDataBuf[1+i];
		tq += IDataBuf[1+i]*(UDataBuf[i]-UDataBuf[2+i]);
	}
	tp = tp/3200;
	tq = tq/SINWTS;// 32*100*2*1000*SIN(wTs)即6400*SIN(11.25°)
	//功率校正,角差放大了10000倍
	MSP = tp + tq*DACoef/10000;     
	MSQ = tq - tp*DACoef/10000;  
	//功率因数计算
	s=(LLONG)MSP*MSP+(LLONG)MSQ*MSQ;
	s2= Evolution64(s);
	if(s2<=2)
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
void CAIProc::CoefAdjustCalu(void)
{
	
	ReadSampleData(m_pCoefAdjustBuf,m_CoefAdjustMask,0);		//读取原始采样数据	
	WriteMSComposeData(m_pComposeBuf,m_pCoefAdjustBuf);	
	
	CaluBaseFourier_S15(m_pComposeBuf,&m_AdjustUp);	
	CaluModValue(&m_AdjustUp);	
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf,&m_AdjustU1p);
	CaluModValue(&m_AdjustU1p);
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&m_AdjustU2p);
	CaluModValue(&m_AdjustU2p);	
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&m_AdjustUd1p);
	CaluModValue(&m_AdjustUd1p);
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&m_AdjustUd2p);
	CaluModValue(&m_AdjustUd2p);

	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&m_AdjustUd3p);
	CaluModValue(&m_AdjustUd3p);
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&m_AdjustIp);
	CaluModValue(&m_AdjustIp);
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&m_AdjustI1p);
	CaluModValue(&m_AdjustI1p);
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&m_AdjustI2p);
	CaluModValue(&m_AdjustI2p);
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&m_AdjustI3p);
	CaluModValue(&m_AdjustI3p);

	Filter(m_pComposeBuf);  //UM
	Filter(m_pCoefAdjustBuf);//U1
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM);//U2
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*2);//UD1
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*3);//UD2
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*4);//UD3	
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*5);//IM
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*7);//ID1
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*8);//ID2
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*9);//ID3

	
	CaluMSMod(m_pComposeBuf,m_AdjustUm);
	CaluMSMod(m_pCoefAdjustBuf,m_AdjustU1m);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,m_AdjustU2m);	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_AdjustUd1m);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,m_AdjustUd2m);	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,m_AdjustUd3m);
	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,m_AdjustIm);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,m_AdjustI1m);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,m_AdjustI2m);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,m_AdjustI3m);	
}
/************************************************************************************************
*功能: 系数校正
*参数: 滤波后数据缓冲区
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	
	if(*AS_pnPM != BTMODE)
	{
		CoefAdjustEnd(FAILURE);
		return;
	}
	if(m_dwAdjustCnt>99)
	{
		if(m_bIsDriftAdjust == TRUE)
		{			
			m_lDriftU	= m_pAdjustData[10];
			m_lDriftU1	= m_pAdjustData[11];
			m_lDriftU2	= m_pAdjustData[12];
			m_lDriftUd1	= m_pAdjustData[13];
			m_lDriftUd2	= m_pAdjustData[14];			
			m_lDriftUd3	= m_pAdjustData[15];
			m_lDriftIm  = m_pAdjustData[16];
			m_lDriftI1	= m_pAdjustData[17];
			m_lDriftI2	= m_pAdjustData[18];
			m_lDriftI3	= m_pAdjustData[19];
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{
			LONG Value[37];			
			Value[0]	= m_pAdjustData[0]/m_dwSourceU;     			//UP
			Value[1]	= m_pAdjustData[1]/m_dwSourceU;					//U1P
			Value[2]	= m_pAdjustData[2]/m_dwSourceU;					//U2P
			Value[3]	= m_pAdjustData[3]/m_dwSourceU;					//U1P
			Value[4]	= m_pAdjustData[4]/m_dwSourceU;					//U2P
			Value[5]	= m_pAdjustData[5]/m_dwSourceU;					//U3P
			Value[6]	= m_pAdjustData[6]/(m_dwSourceI*10);					//IP
			Value[7]	= m_pAdjustData[7]/(m_dwSourceI*10);					//I1P
			Value[8]	= m_pAdjustData[8]/(m_dwSourceI*10);					//I2P
			Value[9]	= m_pAdjustData[9]/(m_dwSourceI*10);					//I3P
			Value[10]	= m_pAdjustData[10]/m_dwSourceU;					//UM
			Value[11]	= m_pAdjustData[11]/m_dwSourceU;					//U1M
			Value[12]	= m_pAdjustData[12]/m_dwSourceU;					//U2M			
			Value[13]	= m_pAdjustData[13]/m_dwSourceU;					//U1M
			Value[14]	= m_pAdjustData[14]/m_dwSourceU;					//U2M
			Value[15]	= m_pAdjustData[15]/m_dwSourceU;					//U3M
			Value[16]	= m_pAdjustData[16]/(m_dwSourceI*10);					//IM
			Value[17]	= m_pAdjustData[17]/(m_dwSourceI*10);					//I1M
			Value[18] 	= m_pAdjustData[18]/(m_dwSourceI*10);					//I2M
			Value[19] 	= m_pAdjustData[19]/(m_dwSourceI*10);					//I3M
			Value[20] 	= m_pAdjustData[20]/(m_dwSourceI*m_dwSourceU);					//P
			Value[21] 	= Value[20];								//Q
			Value[22] 	= m_lDACoef;								//A
			Value[23] 	= 1000;									//
			Value[24] 	= 100;									//
			Value[25]	= 10;									//
			Value[26]	= 1;
			Value[27]	= m_lDriftU;
			Value[28]   = m_lDriftU1;
			Value[29]   = m_lDriftU2;
			Value[30]   = m_lDriftUd1;
			Value[31]   = m_lDriftUd2;
			Value[32]   = m_lDriftUd3;
			Value[33]   = m_lDriftIm;
			Value[34]   = m_lDriftI1; 
			Value[35]   = m_lDriftI2;
			Value[36]   = m_lDriftI3;				
			
			DB_SetCFValue(CF_VALUE_ALL, Value);				//写入系数值
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);	//写入系数确认
			CoefAdjustEnd(SUCCESS);
			return;
		}	
	}
	
	CoefAdjustCalu();

	if(!m_bHaveAdjustAngle)  //首先校正角差,以及源判断
	{
		if(m_bIsDriftAdjust == TRUE)
		{
			if(m_AdjustU1p.Mod > (*CF_pnU1P/200)) // 与小值0.5V作比较
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if(m_AdjustU2p.Mod > (*CF_pnU2P/200))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustUd1p.Mod > (*CF_pnUd1P/200))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if(m_AdjustUd2p.Mod > (*CF_pnUd2P/200))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustUd3p.Mod > (*CF_pnUd3P/200))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}				
			if(m_AdjustIm > (*CF_pnIM/5))  // 与小值0.2A作比较
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if(m_AdjustIp.Mod > (*CF_pnIP/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if(m_AdjustI1p.Mod > (*CF_pnI1P/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustI2p.Mod > (*CF_pnI2P/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjustI3p.Mod > (*CF_pnI3P/5))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		else
		{			
			if((m_AdjustU1p.Mod<(*CF_pnU1P*m_dwSourceU*9/1000))||(m_AdjustU1p.Mod>(*CF_pnU1P*m_dwSourceU*11/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustU2p.Mod<(*CF_pnU2P*m_dwSourceU*9/1000))||(m_AdjustU2p.Mod>(*CF_pnU2P*m_dwSourceU*11/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustUd1p.Mod<(*CF_pnUd1P*m_dwSourceU*9/1000))||(m_AdjustUd1p.Mod>(*CF_pnUd1P*m_dwSourceU*11/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustUd2p.Mod<(*CF_pnUd2P*m_dwSourceU*9/1000))||(m_AdjustUd2p.Mod>(*CF_pnUd2P*m_dwSourceU*11/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustUd3p.Mod<(*CF_pnUd3P*m_dwSourceU*9/1000))||(m_AdjustUd3p.Mod>(*CF_pnUd3P*m_dwSourceU*11/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIm<(*CF_pnIM*m_dwSourceI*9/100))||(m_AdjustIm>(*CF_pnIM*m_dwSourceI*11/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIp.Mod<(*CF_pnIP*m_dwSourceI*9/100))||(m_AdjustIp.Mod>(*CF_pnIP*m_dwSourceI*11/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustI1p.Mod<(*CF_pnI1P*m_dwSourceI*9/100))||(m_AdjustI1p.Mod>(*CF_pnI1P*m_dwSourceI*11/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustI2p.Mod<(*CF_pnI2P*m_dwSourceI*9/100))||(m_AdjustI2p.Mod>(*CF_pnI2P*m_dwSourceI*11/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustI3p.Mod<(*CF_pnI3P*m_dwSourceI*9/100))||(m_AdjustI3p.Mod>(*CF_pnI3P*m_dwSourceI*11/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}		
			for(int i=1;i<10;i++)
			{
				if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
		}
		
		CaluMSPQ(m_pComposeBuf,m_pCoefAdjustBuf+READ_POINT_NUM*5,0,m_AdjustP,m_AdjustQ,m_lMSCOS,FALSE);		//计算有功、无功、功率因数
		if(m_AdjustQ>*CF_pnQ/3)
		{
			CoefAdjustEnd(FAILURE);
			return;
		}
		if(m_AdjustQ == 0)
			m_lDACoef = 0;
		else
			m_lDACoef = m_AdjustQ*10000/((LONG)Evolution64((LLONG)m_AdjustP*m_AdjustP+(LLONG)m_AdjustQ*m_AdjustQ));
		
		m_bHaveAdjustAngle = TRUE;
		m_dwAdjustCnt=0;

		return;
	}

	CaluMSPQ(m_pComposeBuf,m_pCoefAdjustBuf+READ_POINT_NUM*5,m_lDACoef,m_AdjustP,m_AdjustQ,m_lMSCOS);		//计算有功、无功、功率因数

	//压入数组中，排序并取出中间值
	if(m_bIsDriftAdjust) //死区值校正时，取最大零漂值
	{
		if(m_AdjustUm>m_lMidUm)
			m_lMidUm = m_AdjustUm;
		if(m_AdjustU1m>m_lMidU1m)
			m_lMidU1m = m_AdjustU1m;
		if(m_AdjustU2m>m_lMidU2m)
			m_lMidU2m = m_AdjustU2m;
		if(m_AdjustUd1m>m_lMidUd1m)
			m_lMidUd1m = m_AdjustUd1m;
		if(m_AdjustUd2m>m_lMidUd2m)
			m_lMidUd2m = m_AdjustUd2m;
		if(m_AdjustUd3m>m_lMidUd3m)
			m_lMidUd3m = m_AdjustUd3m;
		if(m_AdjustIm>m_lMidIm)
			m_lMidIm = m_AdjustIm;
		if(m_AdjustI1m>m_lMidI1m)
			m_lMidI1m = m_AdjustI1m;
		if(m_AdjustI2m>m_lMidI2m)
			m_lMidI2m = m_AdjustI2m;
		if(m_AdjustI3m>m_lMidI3m)
			m_lMidI3m = m_AdjustI3m;		
			
	}
	else //系数校正值，采用冒泡排序，并取中间值
	{		
		m_lMidUp  = AiBubbleSort(m_AdjustUp.Mod,AArrayUp);
		m_lMidU1p = AiBubbleSort(m_AdjustU1p.Mod,AArrayU1p);
		m_lMidU2p = AiBubbleSort(m_AdjustU2p.Mod,AArrayU2p);
		m_lMidUd1p = AiBubbleSort(m_AdjustUd1p.Mod,AArrayUd1p);
		m_lMidUd2p = AiBubbleSort(m_AdjustUd2p.Mod,AArrayUd2p);
		m_lMidUd3p = AiBubbleSort(m_AdjustUd3p.Mod,AArrayUd3p);
		m_lMidIp  = AiBubbleSort(m_AdjustIp.Mod,AArrayIp);
		m_lMidI1p = AiBubbleSort(m_AdjustI1p.Mod,AArrayI1p);
		m_lMidI2p = AiBubbleSort(m_AdjustI2p.Mod,AArrayI2p);
		m_lMidI3p = AiBubbleSort(m_AdjustI3p.Mod,AArrayI3p);

		m_lMidUm  = AiBubbleSort(m_AdjustUm,AArrayUm);
		m_lMidU1m = AiBubbleSort(m_AdjustU1m,AArrayU1m);
		m_lMidU2m = AiBubbleSort(m_AdjustU2m,AArrayU2m);
		m_lMidUd1m = AiBubbleSort(m_AdjustUd1m,AArrayUd1m);
		m_lMidUd2m = AiBubbleSort(m_AdjustUd2m,AArrayUd2m);
		m_lMidUd3m = AiBubbleSort(m_AdjustUd3m,AArrayUd3m);
		m_lMidIm  = AiBubbleSort(m_AdjustIm,AArrayIm);
		m_lMidI1m = AiBubbleSort(m_AdjustI1m,AArrayI1m);
		m_lMidI2m = AiBubbleSort(m_AdjustI2m,AArrayI2m);
		m_lMidI3m = AiBubbleSort(m_AdjustI3m,AArrayI3m);
		
		m_lMidP   = AiBubbleSort(m_AdjustP,AArrayP);
		m_lMidQ   = AiBubbleSort(m_AdjustQ,AArrayQ);
	}
	
	
	if(m_dwWaitCnt<2000/AI_TIMER_PERIOD) //系数校正，检测源正确后等待两秒钟
	{
		m_dwWaitCnt++;
		return;
	}

	if(m_bIsDriftAdjust)
	{
		m_pAdjustData[10] = m_lMidUm;
		m_pAdjustData[11] = m_lMidU1m;
		m_pAdjustData[12] = m_lMidU2m;
		m_pAdjustData[13] = m_lMidUd1m;
		m_pAdjustData[14] = m_lMidUd2m;
		m_pAdjustData[15] = m_lMidUd3m;
		m_pAdjustData[16] = m_lMidIm;
		m_pAdjustData[17] = m_lMidI1m;
		m_pAdjustData[18] = m_lMidI2m;
		m_pAdjustData[19] = m_lMidI3m;	
		m_pAdjustData[20] = m_lMidP;
		m_pAdjustData[21] = m_lMidQ;	
	}
	else
	{		
		m_pAdjustData[0] += m_lMidUp;
		m_pAdjustData[1] += m_lMidU1p;
		m_pAdjustData[2] += m_lMidU2p;
		m_pAdjustData[3] += m_lMidUd1p;
		m_pAdjustData[4] += m_lMidUd2p;
		m_pAdjustData[5] += m_lMidUd3p;
		m_pAdjustData[6] += m_lMidIp;
		m_pAdjustData[7] += m_lMidI1p;
		m_pAdjustData[8] += m_lMidI2p;
		m_pAdjustData[9] += m_lMidI3p;	

		m_pAdjustData[10] += m_lMidUm;
		m_pAdjustData[11] += m_lMidU1m;
		m_pAdjustData[12] += m_lMidU2m;
		m_pAdjustData[13] += m_lMidUd1m;
		m_pAdjustData[14] += m_lMidUd2m;
		m_pAdjustData[15] += m_lMidUd3m;
		m_pAdjustData[16] += m_lMidIm;
		m_pAdjustData[17] += m_lMidI1m;
		m_pAdjustData[18] += m_lMidI2m;
		m_pAdjustData[19] += m_lMidI3m;	
		m_pAdjustData[20] += m_lMidP;
		m_pAdjustData[21] += m_lMidQ;	
	}
	m_dwAdjustCnt++;


}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<22;i++)     
		m_pAdjustData[i]=0;
	
	//角差校正标志清零
	m_bHaveAdjustAngle = FALSE;

	//系数校正计算次数计数器清零
	m_dwAdjustCnt=0;

	//清系数校正任务标志
	m_bIsCoefAdjust = FALSE;

	//清系数校正等待计数器
	m_dwWaitCnt =0;
	
	//向液晶任务发送消息，传递校正结果
	if(m_bIsDriftAdjust == TRUE)
		OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, ZERODRIFT, 0, 0);
	else
		OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, FULLVALUE, 0, 0);

}

LONG CAIProc::AiBubbleSort(LONG Val,LONG OriginalData[NUMOFYCVALUE])	
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

void CAIProc::InitCTI()
{
	RelCTI.pRelCTBrokCFG = CFG_pbyCTBR;				//CT断线配置 
	RelCTI.pRelCurSet = &CTCurSet;					//电流门槛值 
	RelCTI.pRelTimeSet = &CTTimeSet;				//告警时间 
	RelCTI.dwSysTimer = &dwRelayMSCnt;				//系统定时器 

	RelCTI.pRelCurMax = &m_IMax;						//判断用大电流 
	RelCTI.pRelCurMin = &m_IMin;						//判断用小电流 

	RelCTI.StartRelayWord = RW_PICTBR;			//CT断线入段继电器字 
	RelCTI.ActRelayWord = RW_ARCTBR;				//CT断线动作继电器字 	
	RelCTI.wRelRptNum = R_CTBR;					//CT断线动作报告号
	RelCTI.wRelResRptNum = R_CTBR_RES;			//CT断线动作报告号
	
	RelCTI.byRelFCounter = 0;						//故障计数器 
	RelCTI.byRelRCounter = 0;						//返回计数器 
	RelCTI.dwRelTimer = 0;							//CT断线定时器 	
	RelCTI.wExcepWordNum = 6;						//TRelCTBrokPar CT断线 本结构对象计算CRC校验时需排除在外的字节数，本结构为6，大小为18 
}

void CAIProc::CaluCTMaxMin(void)
{	
	LONG tempI;
	CTCurSet = *AS_pnIPCTRatio*CTBRCHECKVALUE;
	m_IMax = *AS_pnIPCTRatio*m_Ip*CTBRKPM;
	m_IMin = *AS_pnIMCTRatio*m_Im;
	if(m_IMax < m_IMin)
	{	
		tempI = m_IMax;		
		m_IMax = m_IMin;
		m_IMin = tempI;
	}
}























