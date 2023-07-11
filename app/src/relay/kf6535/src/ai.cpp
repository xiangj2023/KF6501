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


#define MAX_FREQUENCE  5050
#define MIN_FREQUENCE  4950
#define MIN_TESTU      30



DWORD dwAITaskID;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD g_hLCDTaskID;

extern DWORD dwRelayDataPtr;	//AD当前写指针,测频


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
	m_CoefAdjustMask = ( (0x01<<CHANNEL_6535_UA)|(0x01<<CHANNEL_6535_UB)|(0x01<<CHANNEL_6535_UC)|(0x01<<CHANNEL_6535_U0)|\
						(0x01<<CHANNEL_6535_IHA)|(0x01<<CHANNEL_6535_IHB)|(0x01<<CHANNEL_6535_IHC)|(0x01<<CHANNEL_6535_IMA)|\
						(0x01<<CHANNEL_6535_I0)|(0x01<<CHANNEL_6535_IMB)|(0x01<<CHANNEL_6535_IMC)|(0x01<<CHANNEL_6535_Ijx)|\
						(0x01<<CHANNEL_6535_ILA)|(0x01<<CHANNEL_6535_ILB)|(0x01<<CHANNEL_6535_ILC));
	
	m_pCoefAdjustBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*15);
	m_pDataBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*15);
	
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);

	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);

	for(LONG i=0;i<45;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{		
		AdjustUA[i] = 0;
		AdjustUB[i] = 0;
		AdjustUC[i] = 0;
		AdjustU0[i] = 0;
		AdjustIHA[i] = 0;
		AdjustIHB[i] = 0;
		AdjustIHC[i] = 0;
		AdjustIMA[i] = 0;
		AdjustIMB[i] = 0;
		AdjustIMC[i] = 0;
		AdjustILA[i] = 0;
		AdjustILB[i] = 0;
		AdjustILC[i] = 0;
		AdjustI0[i] = 0;
		AdjustIJX[i] = 0;
		AdjustUaM[i] = 0;
		AdjustUbM[i] = 0;
		AdjustUcM[i] = 0;
		AdjustU0M[i] = 0;
		AdjustIHAM[i] = 0;
		AdjustIHBM[i] = 0;
		AdjustIHCM[i] = 0;
		AdjustIMAM[i] = 0;
		AdjustIMBM[i] = 0;
		AdjustIMCM[i] = 0;
		AdjustILAM[i] = 0;
		AdjustILBM[i] = 0;
		AdjustILCM[i] = 0;
		AdjustI0M[i] = 0;
		AdjustIJXM[i] = 0;
		m_pBubbleData[i]=0;
	}
	//初始化系数校准标志
	m_bIsCoefAdjust = FALSE;
	m_dwWaitCnt = 0;
	m_dwAdjustCnt=0;
	dwAITaskID = GetTaskId();
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
	//开始系数校准
	if(m_bIsCoefAdjust == TRUE)
	{
		CoefAdjust();	
		return;
	}	
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);

	
	ReadSampleData(m_pDataBuf,m_CoefAdjustMask,0);		//读取原始采样数据		
	for(LONG i=0;i<15;i++)
		Filter(m_pDataBuf+READ_POINT_NUM*i);

	CaluMSMod(m_pDataBuf,Ua);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,Ub);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,Uc);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,U0);
	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,IHA);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,IHB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,IHC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,IMA);	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,I0);	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,IMB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*10,IMC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*11,IJX);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*12,ILA);	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*13,ILB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*14,ILC);

	WriteAIData(M_UA,Ua,*CF_pnUAD,m_pBubbleData,UaArray,TRUE);
	WriteAIData(M_UB,Ub,*CF_pnUBD,m_pBubbleData,UbArray,TRUE);
	WriteAIData(M_UC,Uc,*CF_pnUCD,m_pBubbleData,UcArray,TRUE);
	WriteAIData(M_U0,U0,*CF_pnU0D,m_pBubbleData,U0Array,TRUE);	
	WriteAIData(M_IHAM,IHA,*CF_pnIHAD,m_pBubbleData,IHAArray,TRUE);
	WriteAIData(M_IHBM,IHB,*CF_pnIHBD,m_pBubbleData,IHBArray,TRUE);
	WriteAIData(M_IHCM,IHC,*CF_pnIHCD,m_pBubbleData,IHCArray,TRUE);	
	WriteAIData(M_IMAM,IMA,*CF_pnIMAD,m_pBubbleData,IMAArray,TRUE);
	WriteAIData(M_IMBM,IMB,*CF_pnIMBD,m_pBubbleData,IMBArray,TRUE);
	WriteAIData(M_IMCM,IMC,*CF_pnIMCD,m_pBubbleData,IMCArray,TRUE);
	WriteAIData(M_ILAM,ILA,*CF_pnILAD,m_pBubbleData,ILAArray,TRUE);
	WriteAIData(M_ILBM,ILB,*CF_pnILBD,m_pBubbleData,ILBArray,TRUE);
	WriteAIData(M_ILCM,ILC,*CF_pnILCD,m_pBubbleData,ILCArray,TRUE);
	WriteAIData(M_I0M,I0,*CF_pnI0D,m_pBubbleData,I0Array,TRUE);
	WriteAIData(M_IJXM,IJX,*CF_pnIjxD,m_pBubbleData,IJXArray,TRUE);
	
}

void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	if((Uval==0)&&(Ival ==0))
	{
		m_bIsDriftAdjust = TRUE;
		UaDrift = 0;
		UbDrift = 0;
		UcDrift = 0;
		U0Drift = 0;
		IHADrift = 0;
		IHBDrift = 0;
		IHCDrift = 0;
		IMADrift = 0;
		IMBDrift = 0;
		IMCDrift = 0;
		ILADrift = 0;
		ILBDrift = 0;
		ILCDrift = 0;
		I0Drift = 0;
		IJXDrift = 0;
	}
	else
		m_bIsDriftAdjust = FALSE;

	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	
	dwMsgID = arg;
	m_bIsCoefAdjust = TRUE;
	m_bIsHaveAdjustSource = FALSE;
	m_dwWaitCnt =0;
	m_dwAdjustCnt = 0;
	MidUA = 0;
	MidUB = 0;
	MidUC = 0;
	MidU0 = 0;
	MidIHA =0;
	MidIHB =0;
	MidIHC =0;
	MidIMA =0;
	MidIMB =0;
	MidIMC =0;
	MidILA =0;
	MidILB =0;
	MidILC =0;	
	MidI0 =0;	
	MidIJX =0;	
	MidUaM =0;
	MidUbM =0;
	MidUcM =0;
	MidU0M =0;
	MidIHAM =0;
	MidIHBM =0;
	MidIHCM =0;
	MidIMAM =0;
	MidIMBM =0;
	MidIMCM =0;
	MidILAM =0;
	MidILBM =0;
	MidILCM =0;
	MidI0M =0;
	MidIJXM =0;
	 for(LONG i=0;i<45;i++)
	 	m_pAdjustData[i]=0;
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

	CaluBaseFourier_S15(m_pCoefAdjustBuf,&m_AdjustUA);
	CaluModValue(&m_AdjustUA);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&m_AdjustUB);
	CaluModValue(&m_AdjustUB);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&m_AdjustUC);
	CaluModValue(&m_AdjustUC);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&m_AdjustIHA);
	CaluModValue(&m_AdjustIHA);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&m_AdjustIHB);
	CaluModValue(&m_AdjustIHB);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&m_AdjustIHC);
	CaluModValue(&m_AdjustIHC);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&m_AdjustIMA);
	CaluModValue(&m_AdjustIMA);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&m_AdjustIMB);
	CaluModValue(&m_AdjustIMB);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*10,&m_AdjustIMC);
	CaluModValue(&m_AdjustIMC);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*12,&m_AdjustILA);
	CaluModValue(&m_AdjustILA);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*13,&m_AdjustILB);
	CaluModValue(&m_AdjustILB);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*14,&m_AdjustILC);
	CaluModValue(&m_AdjustILC);	

	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&m_AdjustU0);
	CaluModValue(&m_AdjustU0);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&m_AdjustI0);
	CaluModValue(&m_AdjustI0);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*11,&m_AdjustIJX);
	CaluModValue(&m_AdjustIJX);	

	for(LONG i=0;i<15;i++)
		Filter(m_pCoefAdjustBuf+READ_POINT_NUM*i);

	CaluMSMod(m_pCoefAdjustBuf,UaM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,UbM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,UcM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,IHAM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,IHBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,IHCM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,IMAM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,IMBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*10,IMCM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*12,ILAM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*13,ILBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*14,ILCM);
	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,U0M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,I0M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*11,IJXM);
}
/************************************************************************************************
*功能: 系数校正
*参数: 滤波后数据缓冲区
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	if(m_dwAdjustCnt>99)
	{
		if(m_bIsDriftAdjust)
		{				
			UaDrift = m_pAdjustData[30]; 
			UbDrift = m_pAdjustData[31]; 
			UcDrift = m_pAdjustData[32]; 
			U0Drift = m_pAdjustData[33]; 
			IHADrift = m_pAdjustData[34]; 
			IHBDrift = m_pAdjustData[35]; 
			IHCDrift = m_pAdjustData[36]; 
			IMADrift = m_pAdjustData[37]; 
			IMBDrift = m_pAdjustData[38]; 
			IMCDrift = m_pAdjustData[39]; 
			ILADrift = m_pAdjustData[40]; 
			ILBDrift = m_pAdjustData[41]; 
			ILCDrift = m_pAdjustData[42]; 
			I0Drift  = m_pAdjustData[43]; 
			IJXDrift = m_pAdjustData[44]; 
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{			
			LONG Value[39];				
			Value[0] = (m_pAdjustData[0]+m_pAdjustData[1]+m_pAdjustData[2])/(3*m_dwSourceU);//UP
			Value[1] = m_pAdjustData[3]/(m_dwSourceU);	//U0
			Value[2] = (m_pAdjustData[4]+m_pAdjustData[5]+m_pAdjustData[6] \
						+m_pAdjustData[7]+m_pAdjustData[8]+m_pAdjustData[9] \
						+m_pAdjustData[10]+m_pAdjustData[11]+m_pAdjustData[12])/(m_dwSourceI*9*10);	//IP
			Value[3] = m_pAdjustData[13]/(m_dwSourceI*10);	//I0
			Value[4] = m_pAdjustData[14]/(m_dwSourceI*10);	//IJX
			Value[5] = m_pAdjustData[15]/(m_dwSourceU);	//UAM
			Value[6] = m_pAdjustData[16]/(m_dwSourceU);	//UBM
			Value[7] = m_pAdjustData[17]/(m_dwSourceU);	//UCM
			Value[8] = m_pAdjustData[18]/(m_dwSourceU);	//U0M
			Value[9] = m_pAdjustData[19]/(m_dwSourceI*10);	//IHAM
			Value[10] = m_pAdjustData[20]/(m_dwSourceI*10);	//IHBM
			Value[11] = m_pAdjustData[21]/(m_dwSourceI*10);	//IHCM
			Value[12] = m_pAdjustData[22]/(m_dwSourceI*10);	//IMAM
			Value[13] = m_pAdjustData[23]/(m_dwSourceI*10);	//IMBM
			Value[14] = m_pAdjustData[24]/(m_dwSourceI*10);	//IMCM
			Value[15] = m_pAdjustData[25]/(m_dwSourceI*10);	//ILAM
			Value[16] = m_pAdjustData[26]/(m_dwSourceI*10);	//ILBM
			Value[17] = m_pAdjustData[27]/(m_dwSourceI*10);	//ILCM
			Value[18] = m_pAdjustData[28]/(m_dwSourceI*10);	//I0M
			Value[19] = m_pAdjustData[29]/(m_dwSourceI*10);	//IJXM
			Value[20] = UaDrift;
			Value[21] = UbDrift;
			Value[22] = UcDrift;
			Value[23] = U0Drift;
			Value[24] = IHADrift;
			Value[25] = IHBDrift;
			Value[26] = IHCDrift;
			Value[27] = IMADrift;
			Value[28] = IMBDrift;
			Value[29] = IMCDrift;
			Value[30] = ILADrift;
			Value[31] = ILBDrift;
			Value[32] = ILCDrift;
			Value[33] = I0Drift;
			Value[34] = IJXDrift;
			Value[35] = 1000;	
			Value[36] = 100;   
			Value[37] = 10;
			Value[38] = 1;
					
			DB_SetCFValue(CF_VALUE_ALL, Value);					//写入系数值
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);		//写入系数确认
			CoefAdjustEnd(SUCCESS);
			return;
		}	
	}
	
	CoefAdjustCalu();

	if(!m_bIsHaveAdjustSource)  //首先校正角差及电压电流源检测
	{	
		if(m_bIsDriftAdjust)
		{
			if((m_AdjustUA.Mod>(*CF_pnUp/200))||(m_AdjustUB.Mod>(*CF_pnUp/200))||(m_AdjustUC.Mod>(*CF_pnUp/200))||(m_AdjustU0.Mod>(*CF_pnU0/200)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHA.Mod>(*CF_pnIp/50))||(m_AdjustIHB.Mod>(*CF_pnIp/50))||(m_AdjustIHC.Mod>(*CF_pnIp/50)) \
				||(m_AdjustIMA.Mod>(*CF_pnIp/50))||(m_AdjustIMB.Mod>(*CF_pnIp/50))||(m_AdjustIMC.Mod>(*CF_pnIp/50))\
				||(m_AdjustILA.Mod>(*CF_pnIp/50))||(m_AdjustILB.Mod>(*CF_pnIp/50))||(m_AdjustILC.Mod>(*CF_pnIp/50))\
				||(m_AdjustI0.Mod>(*CF_pnI0/50))||(m_AdjustIJX.Mod>(*CF_pnIjx/50)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		else
		{
			if((m_AdjustUA.Mod>(*CF_pnUp*m_dwSourceU*11/1000))||(m_AdjustUA.Mod<(*CF_pnUp*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustUB.Mod>(*CF_pnUp*m_dwSourceU*11/1000))||(m_AdjustUB.Mod<(*CF_pnUp*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}

			if((m_AdjustUC.Mod>(*CF_pnUp*m_dwSourceU*11/1000))||(m_AdjustUC.Mod<(*CF_pnUp*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustU0.Mod>(*CF_pnU0*m_dwSourceU*11/1000))||(m_AdjustU0.Mod<(*CF_pnU0*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHA.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustIHA.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHB.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustIHB.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHC.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustIHC.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if((m_AdjustIMA.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustIMA.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIMB.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustIMB.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIMC.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustIMC.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustILA.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustILA.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustILB.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustILB.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustILC.Mod>(*CF_pnIp*m_dwSourceI*11/100))||(m_AdjustILC.Mod<(*CF_pnIp*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustI0.Mod>(*CF_pnI0*m_dwSourceI*11/100))||(m_AdjustI0.Mod<(*CF_pnI0*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIJX.Mod>(*CF_pnIjx*m_dwSourceI*11/100))||(m_AdjustIJX.Mod<(*CF_pnIjx*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		m_bIsHaveAdjustSource = TRUE;
		return;
	}

	if(m_bIsDriftAdjust) //死区值校正
	{
		if(UaM>MidUaM)
			MidUaM = UaM;		
		if(UbM>MidUbM)
			MidUbM = UbM;
		if(UcM>MidUcM)
			MidUcM = UcM;
		if(U0M>MidU0M)
			MidU0M = U0M;
		if(IHAM>MidIHAM)
			MidIHAM = IHAM;
		if(IHBM>MidIHBM)
			MidIHBM = IHBM;
		if(IHCM>MidIHCM)
			MidIHCM = IHCM;
		if(IMAM>MidIMAM)
			MidIMAM = IMAM;
		if(IMBM>MidIMBM)
			MidIMBM = IMBM;
		if(IMCM>MidIMCM)
			MidIMCM = IMCM;
		if(ILAM>MidILAM)
			MidILAM = ILAM;
		if(ILBM>MidILBM)
			MidILBM = ILBM;
		if(ILCM>MidILCM)
			MidILCM = ILCM;
		if(I0M>MidI0M)
			MidI0M = I0M;
		if(IJXM>MidIJXM)
			MidIJXM = IJXM;

	}
	else
	{
		MidUA  = CoefAdjustSort(m_AdjustUA.Mod,AdjustUA);	
		MidUB  = CoefAdjustSort(m_AdjustUB.Mod,AdjustUB);
		MidUC  = CoefAdjustSort(m_AdjustUC.Mod,AdjustUC);		
		MidU0  = CoefAdjustSort(m_AdjustU0.Mod,AdjustU0);		
		MidIHA = CoefAdjustSort(m_AdjustIHA.Mod,AdjustIHA);
		MidIHB = CoefAdjustSort(m_AdjustIHB.Mod,AdjustIHB);
		MidIHC = CoefAdjustSort(m_AdjustIHC.Mod,AdjustIHC);
		MidIMA = CoefAdjustSort(m_AdjustIMA.Mod,AdjustIMA);
		MidIMB = CoefAdjustSort(m_AdjustIMB.Mod,AdjustIMB);
		MidIMC = CoefAdjustSort(m_AdjustIMC.Mod,AdjustIMC);
		MidILA = CoefAdjustSort(m_AdjustILA.Mod,AdjustILA);
		MidILB = CoefAdjustSort(m_AdjustILB.Mod,AdjustILB);
		MidILC = CoefAdjustSort(m_AdjustILC.Mod,AdjustILC);
		MidI0 = CoefAdjustSort(m_AdjustI0.Mod,AdjustI0);
		MidIJX = CoefAdjustSort(m_AdjustIJX.Mod,AdjustIJX);

		MidUaM = CoefAdjustSort(UaM,AdjustUaM);
		MidUbM = CoefAdjustSort(UbM,AdjustUbM);
		MidUcM = CoefAdjustSort(UcM,AdjustUcM);
		MidU0M = CoefAdjustSort(U0M,AdjustU0M);
		MidIHAM = CoefAdjustSort(IHAM,AdjustIHAM);
		MidIHBM = CoefAdjustSort(IHBM,AdjustIHBM);
		MidIHCM = CoefAdjustSort(IHCM,AdjustIHCM);		
		MidIMAM = CoefAdjustSort(IMAM,AdjustIMAM);
		MidIMBM = CoefAdjustSort(IMBM,AdjustIMBM);
		MidIMCM = CoefAdjustSort(IMCM,AdjustIMCM);		
		MidILAM = CoefAdjustSort(ILAM,AdjustILAM);
		MidILBM = CoefAdjustSort(ILBM,AdjustILBM);
		MidILCM = CoefAdjustSort(ILCM,AdjustILCM);		
		MidI0M = CoefAdjustSort(I0M,AdjustI0M);		
		MidIJXM = CoefAdjustSort(IJXM,AdjustIJXM);		
		
	}
	
	if(m_dwWaitCnt < (2000/AI_TIMER_PERIOD)) //系数校正，源校验正确后等待两秒钟，保证排序数组填满
	{
		m_dwWaitCnt++;
		return;
	}
	
	if(m_bIsDriftAdjust) //死区值校正
	{		
		m_pAdjustData[30] = MidUaM;
		m_pAdjustData[31] = MidUbM;
		m_pAdjustData[32] = MidUcM;
		m_pAdjustData[33] = MidU0M;
		m_pAdjustData[34] = MidIHAM;
		m_pAdjustData[35] = MidIHBM;
		m_pAdjustData[36] = MidIHCM;
		m_pAdjustData[37] = MidIMAM;
		m_pAdjustData[38] = MidIMBM;
		m_pAdjustData[39] = MidIMCM;
		m_pAdjustData[40] = MidILAM;
		m_pAdjustData[41] = MidILBM;
		m_pAdjustData[42] = MidILCM;
		m_pAdjustData[43] = MidI0M;
		m_pAdjustData[44] = MidIJXM;
	}
	else
	{
		m_pAdjustData[0] += MidUA;
		m_pAdjustData[1] += MidUB;
		m_pAdjustData[2] += MidUC;		
		m_pAdjustData[3] += MidU0;		
		m_pAdjustData[4] += MidIHA;
		m_pAdjustData[5] += MidIHB;
		m_pAdjustData[6] += MidIHC;
		m_pAdjustData[7] += MidIMA;
		m_pAdjustData[8] += MidIMB;
		m_pAdjustData[9] += MidIMC;
		m_pAdjustData[10] += MidILA;
		m_pAdjustData[11] += MidILB;
		m_pAdjustData[12] += MidILC;
		m_pAdjustData[13] += MidI0;
		m_pAdjustData[14] += MidIJX;
		m_pAdjustData[15] += MidUaM;
		m_pAdjustData[16] += MidUbM;
		m_pAdjustData[17] += MidUcM;		
		m_pAdjustData[18] += MidU0M;		
		m_pAdjustData[19] += MidIHAM;
		m_pAdjustData[20] += MidIHBM;
		m_pAdjustData[21] += MidIHCM;
		m_pAdjustData[22] += MidIMAM;
		m_pAdjustData[23] += MidIMBM;
		m_pAdjustData[24] += MidIMCM;
		m_pAdjustData[25] += MidILAM;
		m_pAdjustData[26] += MidILBM;
		m_pAdjustData[27] += MidILCM;
		m_pAdjustData[28] += MidI0M;
		m_pAdjustData[29] += MidIJXM;
	}

	m_dwAdjustCnt++;
	

}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<45;i++) 
	{
		m_pAdjustData[i] =0;
	}
	
	//源校正标志
	m_bIsHaveAdjustSource = FALSE;

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




