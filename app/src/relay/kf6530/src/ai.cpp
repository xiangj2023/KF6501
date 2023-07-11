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
	m_CoefAdjustMask = ( (0x01<<CHANNEL_6530_UA)|(0x01<<CHANNEL_6530_UB)|(0x01<<CHANNEL_6530_UC)|
						(0x01<<CHANNEL_6530_IHA)|(0x01<<CHANNEL_6530_IHB)|(0x01<<CHANNEL_6530_IHC)|\
						(0x01<<CHANNEL_6530_IL1)|(0x01<<CHANNEL_6530_IL2)|(0x01<<CHANNEL_6530_IL3)|\
						(0x01<<CHANNEL_6530_IL4));
	
	m_pCoefAdjustBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*10);
	m_pDataBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*10);
	
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
	//测频
	
//	m_dwFpre = 5000;                    //初始化前次采样频率为50Hz
//	m_dwFnow = 0;
//	m_dwTs = 0;
//	
//	dwTestFDataMask = (0x01<<CHANNEL_6530_IHA);
//	dwTestFBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*POINT_PER_PERIOD*2);	//初始化测频缓冲区			
//	ASSERT(dwTestFBuf!= NULL);
//	if(dwTestFBuf == NULL)
//	{
//		LogError("CRelayTask,dwTestFBuf",FILE_LINE,"malloc error.");
//		return;
//	}

//

	for(LONG i=0;i<30;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{		
		AdjustUA[i] = 0;
		AdjustUB[i] = 0;
		AdjustUC[i] = 0;
		AdjustIHA[i] = 0;
		AdjustIHB[i] = 0;
		AdjustIHC[i] = 0;
		AdjustIL1[i] = 0;
		AdjustIL2[i] = 0;
		AdjustIL3[i] = 0;
		AdjustIL4[i] = 0;
		AdjustUaM[i] = 0;
		AdjustUbM[i] = 0;
		AdjustUcM[i] = 0;
		AdjustIAM[i] = 0;
		AdjustIBM[i] = 0;
		AdjustICM[i] = 0;
		AdjustI1M[i] = 0;
		AdjustI2M[i] = 0;
		AdjustI3M[i] = 0;
		AdjustI4M[i] = 0;
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
	Filter(m_pDataBuf);
	Filter(m_pDataBuf+READ_POINT_NUM);
	Filter(m_pDataBuf+READ_POINT_NUM*2);
	Filter(m_pDataBuf+READ_POINT_NUM*3);
	Filter(m_pDataBuf+READ_POINT_NUM*4);
	Filter(m_pDataBuf+READ_POINT_NUM*5);
	Filter(m_pDataBuf+READ_POINT_NUM*6);
	Filter(m_pDataBuf+READ_POINT_NUM*7);
	Filter(m_pDataBuf+READ_POINT_NUM*8);
	Filter(m_pDataBuf+READ_POINT_NUM*9);

	CaluMSMod(m_pDataBuf,Ua);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,Ub);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,Uc);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,IA);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,IB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,IC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,I1);	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,I2);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,I3);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,I4);

	WriteAIData(M_UA,Ua,*CF_pnUAD,m_pBubbleData,UaArray,TRUE);
	WriteAIData(M_UB,Ub,*CF_pnUBD,m_pBubbleData,UbArray,TRUE);
	WriteAIData(M_UC,Uc,*CF_pnUCD,m_pBubbleData,UcArray,TRUE);
	WriteAIData(M_IHAM,IA,*CF_pnIAD,m_pBubbleData,IAArray,TRUE);
	WriteAIData(M_IHBM,IB,*CF_pnIBD,m_pBubbleData,IBArray,TRUE);
	WriteAIData(M_IHCM,IC,*CF_pnICD,m_pBubbleData,ICArray,TRUE);	
	WriteAIData(M_IL1M,I1,*CF_pnI1D,m_pBubbleData,I1Array,TRUE);
	WriteAIData(M_IL2M,I2,*CF_pnI2D,m_pBubbleData,I2Array,TRUE);
	WriteAIData(M_IL3M,I3,*CF_pnI3D,m_pBubbleData,I3Array,TRUE);
	WriteAIData(M_IL4M,I4,*CF_pnI4D,m_pBubbleData,I4Array,TRUE);
	
}

void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	if((Uval==0)&&(Ival ==0))
	{
		m_bIsDriftAdjust = TRUE;
		UaDrift = 0;
		UbDrift = 0;
		UcDrift = 0;
		IADrift = 0;
		IBDrift = 0;
		ICDrift = 0;
		I1Drift = 0;
		I2Drift = 0;
		I3Drift = 0;
		I4Drift = 0;
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
	MidIHA =0;
	MidIHB =0;
	MidIHC =0;
	MidIL1 =0;
	MidIL2 =0;
	MidIL3 =0;	
	MidIL4 =0;	
	MidUaM =0;
	MidUbM =0;
	MidUcM =0;
	MidIAM =0;
	MidIBM =0;
	MidICM =0;
	MidI1M =0;
	MidI2M =0;
	MidI3M =0;
	MidI4M =0;
	 for(LONG i=0;i<30;i++)
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
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&m_AdjustIHA);
	CaluModValue(&m_AdjustIHA);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&m_AdjustIHB);
	CaluModValue(&m_AdjustIHB);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&m_AdjustIHC);
	CaluModValue(&m_AdjustIHC);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&m_AdjustIL1);
	CaluModValue(&m_AdjustIL1);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&m_AdjustIL2);
	CaluModValue(&m_AdjustIL2);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&m_AdjustIL3);
	CaluModValue(&m_AdjustIL3);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&m_AdjustIL4);
	CaluModValue(&m_AdjustIL4);	

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

	CaluMSMod(m_pCoefAdjustBuf,UaM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,UbM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,UcM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,IAM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,IBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,ICM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,I1M);	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,I2M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,I3M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,I4M);
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
			UaDrift = m_pAdjustData[20]; 
			UbDrift = m_pAdjustData[21]; 
			UcDrift = m_pAdjustData[22]; 
			IADrift = m_pAdjustData[23]; 
			IBDrift = m_pAdjustData[24]; 
			ICDrift = m_pAdjustData[25]; 
			I1Drift = m_pAdjustData[26]; 
			I2Drift = m_pAdjustData[27]; 
			I3Drift = m_pAdjustData[28]; 
			I4Drift = m_pAdjustData[29]; 
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{			
			LONG Value[38];				
			Value[0] = m_pAdjustData[0]/(m_dwSourceI*10);	//IHA
			Value[1] = m_pAdjustData[1]/(m_dwSourceI*10);	//IHB
			Value[2] = m_pAdjustData[2]/(m_dwSourceI*10);	//IHC
			Value[3] = m_pAdjustData[3]/(m_dwSourceI*10);	//IL1
			Value[4] = m_pAdjustData[4]/(m_dwSourceI*10);	//IL2
			Value[5] = m_pAdjustData[5]/(m_dwSourceI*10);	//IL3
			Value[6] = m_pAdjustData[6]/(m_dwSourceI*10);	//IL4
			Value[7] = Value[0];	//IDA
			Value[8] = Value[1];
			Value[9] = Value[2];
			Value[10] = Value[0];
			Value[11] = Value[1];
			Value[12] = Value[2];
			Value[13] = (m_pAdjustData[7]+m_pAdjustData[8]+m_pAdjustData[9])/(3*m_dwSourceU);
			Value[14] = 1000;
			Value[15] = 100;
			Value[16] = 10;
			Value[17] = 1;	
			Value[18] = m_pAdjustData[10]/m_dwSourceU;   
			Value[19] = m_pAdjustData[11]/m_dwSourceU;
			Value[20] = m_pAdjustData[12]/m_dwSourceU;
			Value[21] = m_pAdjustData[13]/(m_dwSourceI*10);
			Value[22] = m_pAdjustData[14]/(m_dwSourceI*10);
			Value[23] = m_pAdjustData[15]/(m_dwSourceI*10);
			Value[24] = m_pAdjustData[16]/(m_dwSourceI*10);
			Value[25] = m_pAdjustData[17]/(m_dwSourceI*10);
			Value[26] = m_pAdjustData[18]/(m_dwSourceI*10);
			Value[27] = m_pAdjustData[19]/(m_dwSourceI*10);
			Value[28] = UaDrift;
			Value[29] = UbDrift;
			Value[30] = UcDrift;
			Value[31] = IADrift;
			Value[32] = IBDrift;
			Value[33] = ICDrift;
			Value[34] = I1Drift;
			Value[35] = I2Drift;
			Value[36] = I3Drift;
			Value[37] = I4Drift;
					
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
			if((m_AdjustUA.Mod>(*CF_pnU/200))||(m_AdjustUB.Mod>(*CF_pnU/200))||(m_AdjustUC.Mod>(*CF_pnU/200)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHA.Mod>(*CF_pnIA/50))||(m_AdjustIHB.Mod>(*CF_pnIB/50))||(m_AdjustIHC.Mod>(*CF_pnIC/50)) \
				||(m_AdjustIL1.Mod>(*CF_pnI1/50))||(m_AdjustIL2.Mod>(*CF_pnI2/50))||(m_AdjustIL3.Mod>(*CF_pnI3/50)) \
				||(m_AdjustIL4.Mod>(*CF_pnI4/50)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		else
		{
			if((m_AdjustUA.Mod>(*CF_pnU*m_dwSourceU*11/1000))||(m_AdjustUA.Mod<(*CF_pnU*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			
			if((m_AdjustUB.Mod>(*CF_pnU*m_dwSourceU*11/1000))||(m_AdjustUB.Mod<(*CF_pnU*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}

			if((m_AdjustUC.Mod>(*CF_pnU*m_dwSourceU*11/1000))||(m_AdjustUC.Mod<(*CF_pnU*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHA.Mod>(*CF_pnIA*m_dwSourceI*11/100))||(m_AdjustIHA.Mod<(*CF_pnIA*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHB.Mod>(*CF_pnIB*m_dwSourceI*11/100))||(m_AdjustIHB.Mod<(*CF_pnIB*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHC.Mod>(*CF_pnIC*m_dwSourceI*11/100))||(m_AdjustIHC.Mod<(*CF_pnIC*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if((m_AdjustIL1.Mod>(*CF_pnI1*m_dwSourceI*11/100))||(m_AdjustIL1.Mod<(*CF_pnI1*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL2.Mod>(*CF_pnI2*m_dwSourceI*11/100))||(m_AdjustIL2.Mod<(*CF_pnI2*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL3.Mod>(*CF_pnI3*m_dwSourceI*11/100))||(m_AdjustIL3.Mod<(*CF_pnI3*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL4.Mod>(*CF_pnI4*m_dwSourceI*11/100))||(m_AdjustIL4.Mod<(*CF_pnI4*m_dwSourceI*9/100)))
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
		if(IAM>MidIAM)
			MidIAM = IAM;
		if(IBM>MidIBM)
			MidIBM = IBM;
		if(ICM>MidICM)
			MidICM = ICM;
		if(I1M>MidI1M)
			MidI1M = I1M;
		if(I2M>MidI2M)
			MidI2M = I2M;
		if(I3M>MidI3M)
			MidI3M = I3M;
		if(I4M>MidI4M)
			MidI4M = I4M;

	}
	else
	{
		MidUA  = CoefAdjustSort(m_AdjustUA.Mod,AdjustUA);	
		MidUB  = CoefAdjustSort(m_AdjustUB.Mod,AdjustUB);
		MidUC  = CoefAdjustSort(m_AdjustUC.Mod,AdjustUC);		
		MidIHA = CoefAdjustSort(m_AdjustIHA.Mod,AdjustIHA);
		MidIHB = CoefAdjustSort(m_AdjustIHB.Mod,AdjustIHB);
		MidIHC = CoefAdjustSort(m_AdjustIHC.Mod,AdjustIHC);
		MidIL1 = CoefAdjustSort(m_AdjustIL1.Mod,AdjustIL1);
		MidIL2 = CoefAdjustSort(m_AdjustIL2.Mod,AdjustIL2);
		MidIL3 = CoefAdjustSort(m_AdjustIL3.Mod,AdjustIL3);
		MidIL4 = CoefAdjustSort(m_AdjustIL4.Mod,AdjustIL4);

		MidUaM = CoefAdjustSort(UaM,AdjustUaM);
		MidUbM = CoefAdjustSort(UbM,AdjustUbM);
		MidUcM = CoefAdjustSort(UcM,AdjustUcM);
		MidIAM = CoefAdjustSort(IAM,AdjustIAM);
		MidIBM = CoefAdjustSort(IBM,AdjustIBM);
		MidICM = CoefAdjustSort(ICM,AdjustICM);		
		MidI1M = CoefAdjustSort(I1M,AdjustI1M);
		MidI2M = CoefAdjustSort(I2M,AdjustI2M);
		MidI3M = CoefAdjustSort(I3M,AdjustI3M);
		MidI4M = CoefAdjustSort(I4M,AdjustI4M);		
	}
	
	if(m_dwWaitCnt < (2000/AI_TIMER_PERIOD)) //系数校正，源校验正确后等待两秒钟，保证排序数组填满
	{
		m_dwWaitCnt++;
		return;
	}
	
	if(m_bIsDriftAdjust) //死区值校正
	{		
		m_pAdjustData[20] = MidUaM;
		m_pAdjustData[21] = MidUbM;
		m_pAdjustData[22] = MidUcM;
		m_pAdjustData[23] = MidIAM;
		m_pAdjustData[24] = MidIBM;
		m_pAdjustData[25] = MidICM;
		m_pAdjustData[26] = MidI1M;
		m_pAdjustData[27] = MidI2M;
		m_pAdjustData[28] = MidI3M;
		m_pAdjustData[29] = MidI4M;
	}
	else
	{
		m_pAdjustData[0] += MidIHA;
		m_pAdjustData[1] += MidIHB;
		m_pAdjustData[2] += MidIHC;
		m_pAdjustData[3] += MidIL1;
		m_pAdjustData[4] += MidIL2;
		m_pAdjustData[5] += MidIL3;
		m_pAdjustData[6] += MidIL4;
		m_pAdjustData[7] += MidUA;
		m_pAdjustData[8] += MidUB;
		m_pAdjustData[9] += MidUC;		
		m_pAdjustData[10] += MidUaM;
		m_pAdjustData[11] += MidUbM;
		m_pAdjustData[12] += MidUcM;
		m_pAdjustData[13] += MidIAM;
		m_pAdjustData[14] += MidIBM;
		m_pAdjustData[15] += MidICM;
		m_pAdjustData[16] += MidI1M;
		m_pAdjustData[17] += MidI2M;
		m_pAdjustData[18] += MidI3M;
		m_pAdjustData[19] += MidI4M;
	}

	m_dwAdjustCnt++;
	

}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<27;i++) 
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
//void CAIProc::RefreshCommRW()
//{
//	for(WORD i=0;i<5;i++)
//	{
//		if(DAE_GetRunningFlags(SS_ETHA_COMM_ERR+i))
//			WriteRelayWord(RW_ET11ERR+i,RELAY_ON);
//		else
//			WriteRelayWord(RW_ET11ERR+i,RELAY_OFF);
//		
//		if(DAE_GetRunningFlags(SS_ETHB_COMM_ERR+i))
//			WriteRelayWord(RW_ET21ERR+i,RELAY_ON);
//		else
//			WriteRelayWord(RW_ET21ERR+i,RELAY_OFF);
//	}
//	
//	if(DAE_GetRunningFlags(SS_SER_COMM_ERR))
//		WriteRelayWord(RW_COM1ERR,RELAY_ON);
//	else
//		WriteRelayWord(RW_COM1ERR,RELAY_OFF);

//	
//	if(DAE_GetRunningFlags(SS_CAN_COMM_ERR))
//		WriteRelayWord(RW_CAN1ERR,RELAY_ON);
//	else
//		WriteRelayWord(RW_CAN1ERR,RELAY_OFF);

//	if(DAE_GetRunningFlags(SS_CAN_COMM_ERR+1))
//		WriteRelayWord(RW_CAN2ERR,RELAY_ON);
//	else
//		WriteRelayWord(RW_CAN2ERR,RELAY_OFF);
//	
//}




