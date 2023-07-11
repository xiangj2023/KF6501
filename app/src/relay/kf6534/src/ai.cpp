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
#include "etpu_app.h"
#include "etpu_cfg.h"
#include "hmiapp.h"

DWORD dwAITaskID;
extern DWORD g_hLCDTaskID;
extern g_dwFreqPWM;
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
	m_AIDataMask = ( (0x01<<CHANNEL_6534_U1)|(0x01<<CHANNEL_6534_U2)|(0x01<<CHANNEL_6534_U3)|(0x01<<CHANNEL_6534_U4) \
				|(0x01<<CHANNEL_6534_U5)|(0x01<<CHANNEL_6534_U6)|(0x01<<CHANNEL_6534_U7)|(0x01<<CHANNEL_6534_U8) \
				|(0x01<<CHANNEL_6534_I1)|(0x01<<CHANNEL_6534_I2)|(0x01<<CHANNEL_6534_I3)|(0x01<<CHANNEL_6534_I4) \
				|(0x01<<CHANNEL_6534_I5)|(0x01<<CHANNEL_6534_I6)|(0x01<<CHANNEL_6534_I7)|(0x01<<CHANNEL_6534_I8));
	
	m_dwFreqMask = 0x01;
	
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*16);
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*16);

	m_pCoefAdjustBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*16);
	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*16);	

//	m_wFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*POINT_PER_PERIOD*FREQBUFSIZE);
//	ASSERT(m_wFreqBuf != NULL);
//	if(m_wFreqBuf == NULL)
//	{
//		LogError("CAIProc,m_wFreqBuf",FILE_LINE,"malloc error.");
//		return;
//	}
//	memset(m_wFreqBuf, 0, sizeof(SHORT)*POINT_PER_PERIOD*FREQBUFSIZE);

//	m_FreqCaluBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*POINT_PER_PERIOD*FREQBUFSIZE);	
//	ASSERT(m_FreqCaluBuf != NULL);
//	if(m_FreqCaluBuf == NULL)
//	{
//		LogError("CAIProc,m_FreqCaluBuf",FILE_LINE,"malloc error.");
//		return;
//	}
//	memset(m_FreqCaluBuf, 0, sizeof(DWORD)*POINT_PER_PERIOD*FREQBUFSIZE);

	
	for(LONG i=0;i<32;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		UM1Array[i]=0;
		UM2Array[i]=0;
		UM3Array[i]=0;
		UM4Array[i]=0;
		UM5Array[i]=0;
		UM6Array[i]=0;
		UM7Array[i]=0;
		UM8Array[i]=0;
		IM1Array[i]=0;
		IM2Array[i]=0;
		IM3Array[i]=0;
		IM4Array[i]=0;
		IM5Array[i]=0;
		IM6Array[i]=0;
		IM7Array[i]=0;
		IM8Array[i]=0;
	}
	//初始化系数校准标志
	m_bIsCoefAdjust = FALSE;
	m_bIsSourceOK =FALSE;
	m_dwAdjustCnt=0;
	dwAITaskID = GetTaskId();

	
	m_FreqFlag = FALSE;
	m_dwPreFreq = 50000;
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
	ReadSampleData(m_pDataBuf,m_AIDataMask,0);
	for(LONG i=0;i<16;i++)
	{
		Filter(m_pDataBuf+READ_POINT_NUM*i);
	}
	CaluMSMod(m_pDataBuf,m_U1);							//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,m_U2);			//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,m_U3);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,m_U4);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,m_U5);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,m_U6);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,m_U7);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,m_U8);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,m_I1);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,m_I2);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*10,m_I3);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*11,m_I4);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*12,m_I5);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*13,m_I6);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*14,m_I7);		//测量电压U1
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*15,m_I8);		//测量电压U1


	
	m_U1 = AiBubbleSort(m_U1,UM1Array);
	if(m_U1<=*CF_pnU1D)		
		m_U1 = 0;
	DB_WriteAI(M_UM1,m_U1);

	m_U2 = AiBubbleSort(m_U2,UM2Array);
	if(m_U2<=*CF_pnU2D)		
		m_U2 = 0;
	DB_WriteAI(M_UM2,m_U2);
	
	m_U3 = AiBubbleSort(m_U3,UM3Array);
	if(m_U3<=*CF_pnU3D)		
		m_U3 = 0;
	DB_WriteAI(M_UM3,m_U3);

	m_U4 = AiBubbleSort(m_U4,UM4Array);
	if(m_U4<=*CF_pnU4D)		
		m_U4 = 0;
	DB_WriteAI(M_UM4,m_U4);

	m_U5 = AiBubbleSort(m_U5,UM5Array);
	if(m_U5<=*CF_pnU5D)		
		m_U5 = 0;
	DB_WriteAI(M_UM5,m_U5);

	m_U6 = AiBubbleSort(m_U6,UM6Array);
	if(m_U6<=*CF_pnU6D)		
		m_U6 = 0;
	DB_WriteAI(M_UM6,m_U6);

	m_U7 = AiBubbleSort(m_U7,UM7Array);
	if(m_U7<=*CF_pnU7D)		
		m_U7 = 0;
	DB_WriteAI(M_UM7,m_U7);

	m_U8 = AiBubbleSort(m_U8,UM8Array);
	if(m_U8<=*CF_pnU8D)		
		m_U8 = 0;
	DB_WriteAI(M_UM8,m_U8);

	m_I1 = AiBubbleSort(m_I1,IM1Array);
	if(m_I1<=*CF_pnI1D)		
		m_I1 = 0;
	DB_WriteAI(M_IM1,m_I1);

	m_I2 = AiBubbleSort(m_I2,IM2Array);
	if(m_I2<=*CF_pnI2D)		
		m_I2 = 0;
	DB_WriteAI(M_IM2,m_I2);

	m_I3 = AiBubbleSort(m_I3,IM3Array);
	if(m_I3<=*CF_pnI3D)		
		m_I3 = 0;
	DB_WriteAI(M_IM3,m_I3);

	m_I4 = AiBubbleSort(m_I4,IM4Array);
	if(m_I4<=*CF_pnI4D)		
		m_I4 = 0;
	DB_WriteAI(M_IM4,m_I4);

	m_I5 = AiBubbleSort(m_I5,IM5Array);
	if(m_I5<=*CF_pnI5D)		
		m_I5 = 0;
	DB_WriteAI(M_IM5,m_I5);

	m_I6 = AiBubbleSort(m_I6,IM6Array);
	if(m_I6<=*CF_pnI6D)		
		m_I6 = 0;
	DB_WriteAI(M_IM6,m_I6);

	m_I7 = AiBubbleSort(m_I7,IM7Array);
	if(m_I7<=*CF_pnI7D)		
		m_I7 = 0;
	DB_WriteAI(M_IM7,m_I7);

	m_I8 = AiBubbleSort(m_I8,IM8Array);
	if(m_I8<=*CF_pnI8D)		
		m_I8 = 0;
	DB_WriteAI(M_IM8,m_I8);

//装置启动后2秒开始测频
//	static WORD freqcounter =0;

//	if((freqcounter>10))
//	{
//		m_FreqFlag = TRUE;
//	}
//		
//	freqcounter++;	
//	if(m_FreqFlag)
//	{
//		BOOL rc;
//		rc = TestFrequence(m_dwPreFreq,m_U1,m_dwFreqMask,m_wFreqBuf,m_FreqCaluBuf,m_dwFreq);
//		if(rc)
//		{
//			BubbleSort(m_dwFreq,NUM_FREQ_PER_AITASK);
//			m_dwPreFreq = m_dwFreq[NUM_FREQ_PER_AITASK/2]/10;
//			
//			//频率不在合理频率变化范围之内,取50Hz
//			if(m_dwPreFreq>MAX_FREQUENCE||m_dwPreFreq<MIN_FREQUENCE) 
//			{
//				m_dwPreFreq = 50000;
//			}
//			g_dwFreqPWM = m_dwPreFreq*POINT_PER_PERIOD;
//			ADStartFreqUpdate(g_dwFreqPWM);
//		}
//		else
//		{
//			m_dwPreFreq = 0;
//			ADStartFreqUpdate(50000*POINT_PER_PERIOD);
//		}		
//	}
	//LEDOnOff(18, FALSE);
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);

}

void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//设置自动校准标志	
	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDriftAdjust = TRUE;		
		m_lDriftU1 = 0;
		m_lDriftU2 = 0;
		m_lDriftU3 = 0;
		m_lDriftU4 = 0;
		m_lDriftU5 = 0;
		m_lDriftU6 = 0;
		m_lDriftU7 = 0;
		m_lDriftU8 = 0;
		m_lDriftI1 = 0;
		m_lDriftI2 = 0;
		m_lDriftI3 = 0;
		m_lDriftI4 = 0;
		m_lDriftI5 = 0;
		m_lDriftI6 = 0;
		m_lDriftI7 = 0;
		m_lDriftI8 = 0;
	}
	else
		m_bIsDriftAdjust = FALSE;		
	
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	dwMsgID = arg;
	
	m_bIsCoefAdjust = TRUE;	
	m_bIsSourceOK = FALSE;
	m_dwWaitCnt = 0;
	
	m_lMidU1m = 0;
	m_lMidU2m = 0;
	m_lMidU3m = 0;
	m_lMidU4m = 0;
	m_lMidU5m = 0;
	m_lMidU6m = 0;
	m_lMidU7m = 0;
	m_lMidU8m = 0;
	m_lMidI1m = 0;
	m_lMidI2m = 0;
	m_lMidI3m = 0;
	m_lMidI4m = 0;
	m_lMidI5m = 0;
	m_lMidI6m = 0;
	m_lMidI7m = 0;
	m_lMidI8m = 0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		AArrayU1p[i] = 0;
		AArrayU2p[i] = 0;
		AArrayU3p[i] = 0;
		AArrayU4p[i] = 0;
		AArrayU5p[i] = 0;
		AArrayU6p[i] = 0;
		AArrayU7p[i] = 0;
		AArrayU8p[i] = 0;
		AArrayI1p[i] = 0;
		AArrayI2p[i] = 0;
		AArrayI3p[i] = 0;
		AArrayI4p[i] = 0;
		AArrayI5p[i] = 0;
		AArrayI6p[i] = 0;
		AArrayI7p[i] = 0;
		AArrayI8p[i] = 0;
		
		AArrayU1m[i] = 0;
		AArrayU2m[i] = 0;
		AArrayU3m[i] = 0;
		AArrayU4m[i] = 0;
		AArrayU5m[i] = 0;
		AArrayU6m[i] = 0;
		AArrayU7m[i] = 0;
		AArrayU8m[i] = 0;
		AArrayI1m[i] = 0;
		AArrayI2m[i] = 0;
		AArrayI3m[i] = 0;
		AArrayI4m[i] = 0;
		AArrayI5m[i] = 0;
		AArrayI6m[i] = 0;
		AArrayI7m[i] = 0;
		AArrayI8m[i] = 0;
		
		
	}
}
/*
void CAIProc::OnAutoAdjustStep2(DWORD arg,DWORD Uval,DWORD Ival)
{	
	m_bIsCoefAdjust = TRUE;	
	m_bIsDriftAdjust = FALSE;
	m_bIsSourceOK = FALSE;
	m_dwWaitCnt = 0;
	
	m_lMidU1m = 0;
	m_lMidU2m = 0;
	m_lMidU3m = 0;
	m_lMidU4m = 0;
	m_lMidU5m = 0;
	m_lMidU6m = 0;
	m_lMidU7m = 0;
	m_lMidU8m = 0;
	m_lMidI1m = 0;
	m_lMidI2m = 0;
	m_lMidI3m = 0;
	m_lMidI4m = 0;
	m_lMidI5m = 0;
	m_lMidI6m = 0;
	m_lMidI7m = 0;
	m_lMidI8m = 0;
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
}
*/
void CAIProc::CoefAdjustCalu(void)//处理系数校正中的计算
{	
	// 1.读取原始采样数据	
	ReadSampleData(m_pCoefAdjustBuf,m_AIDataMask,0);		
	// 2.计算保护测量值
	CaluBaseFourier_S15(m_pCoefAdjustBuf,&m_AdjustU1p);
	CaluModValue(&m_AdjustU1p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&m_AdjustU2p);
	CaluModValue(&m_AdjustU2p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&m_AdjustU3p);
	CaluModValue(&m_AdjustU3p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&m_AdjustU4p);
	CaluModValue(&m_AdjustU4p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&m_AdjustU5p);
	CaluModValue(&m_AdjustU5p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&m_AdjustU6p);
	CaluModValue(&m_AdjustU6p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&m_AdjustU7p);
	CaluModValue(&m_AdjustU7p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&m_AdjustU8p);
	CaluModValue(&m_AdjustU8p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&m_AdjustI1p);
	CaluModValue(&m_AdjustI1p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&m_AdjustI2p);
	CaluModValue(&m_AdjustI2p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*10,&m_AdjustI3p);
	CaluModValue(&m_AdjustI3p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*11,&m_AdjustI4p);
	CaluModValue(&m_AdjustI4p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*12,&m_AdjustI5p);
	CaluModValue(&m_AdjustI5p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*13,&m_AdjustI6p);
	CaluModValue(&m_AdjustI6p);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*14,&m_AdjustI7p);
	CaluModValue(&m_AdjustI7p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*15,&m_AdjustI8p);
	CaluModValue(&m_AdjustI8p);
	// 3.计算遥测值
	for(LONG i=0;i<16;i++)
	{
		Filter(m_pCoefAdjustBuf+READ_POINT_NUM*i);
	}	
	CaluMSMod(m_pCoefAdjustBuf,m_AdjustU1m);						//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,m_AdjustU2m);			//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_AdjustU3m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,m_AdjustU4m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,m_AdjustU5m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,m_AdjustU6m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,m_AdjustU7m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,m_AdjustU8m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,m_AdjustI1m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,m_AdjustI2m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*10,m_AdjustI3m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*11,m_AdjustI4m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*12,m_AdjustI5m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*13,m_AdjustI6m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*14,m_AdjustI7m);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*15,m_AdjustI8m);		//测量电压U1
}

/************************************************************************************************
*功能: 系数校正
*参数: 滤波后数据缓冲区
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	// 系数校正100次结束
	if(m_dwAdjustCnt>99)
	{
		if(m_bIsDriftAdjust == TRUE)
		{
			m_lDriftU1 = m_pAdjustData[16];
			m_lDriftU2 = m_pAdjustData[17];
			m_lDriftU3 = m_pAdjustData[18];
			m_lDriftU4 = m_pAdjustData[19];
			m_lDriftU5 = m_pAdjustData[20];
			m_lDriftU6 = m_pAdjustData[21];
			m_lDriftU7 = m_pAdjustData[22];
			m_lDriftU8 = m_pAdjustData[23];
			m_lDriftI1 = m_pAdjustData[24];
			m_lDriftI2 = m_pAdjustData[25];
			m_lDriftI3 = m_pAdjustData[26];
			m_lDriftI4 = m_pAdjustData[27];
			m_lDriftI5 = m_pAdjustData[28];
			m_lDriftI6 = m_pAdjustData[29];		
			m_lDriftI7 = m_pAdjustData[30];
			m_lDriftI8 = m_pAdjustData[31];		
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{			
			LONG Value[54];
			
			Value[0]	= m_pAdjustData[0]/m_dwSourceU;     			//U1p
			Value[1]	= m_pAdjustData[1]/m_dwSourceU;					//U2p
			Value[2]	= m_pAdjustData[2]/m_dwSourceU;					//U3p
			Value[3]	= m_pAdjustData[3]/m_dwSourceU;					//U4p
			Value[4]	= m_pAdjustData[4]/m_dwSourceU;					//U5p
			Value[5]	= m_pAdjustData[5]/m_dwSourceU;					//U6p
			Value[6]	= m_pAdjustData[6]/m_dwSourceU;					//U7p
			Value[7]	= m_pAdjustData[7]/m_dwSourceU;					//U8p
			Value[8]	= m_pAdjustData[8]/(10*m_dwSourceI);			//I1p
			Value[9]	= m_pAdjustData[9]/(10*m_dwSourceI);			//I2p
			Value[10]	= m_pAdjustData[10]/(10*m_dwSourceI);			//I3p
			Value[11]	= m_pAdjustData[11]/(10*m_dwSourceI);			//I4p
			Value[12]	= m_pAdjustData[12]/(10*m_dwSourceI);			//I5p
			Value[13]	= m_pAdjustData[13]/(10*m_dwSourceI);			//I6p
			Value[14]	= m_pAdjustData[14]/(10*m_dwSourceI);			//I7p
			Value[15]	= m_pAdjustData[15]/(10*m_dwSourceI);			//I8p			
			Value[16]	= (Value[8]+Value[9])/2;						//Ic1
			Value[17]	= (Value[11]+Value[12])/2;						//Ic2			
			Value[18]	= m_pAdjustData[16]/m_dwSourceU;				//U1m
			Value[19]	= m_pAdjustData[17]/m_dwSourceU;				//U2m
			Value[20]	= m_pAdjustData[18]/m_dwSourceU;				//U3m
			Value[21]	= m_pAdjustData[19]/m_dwSourceU;				//U4m
			Value[22]	= m_pAdjustData[20]/m_dwSourceU;				//U5m
			Value[23]	= m_pAdjustData[21]/m_dwSourceU;				//U6m
			Value[24]	= m_pAdjustData[22]/m_dwSourceU;				//U7m
			Value[25]	= m_pAdjustData[23]/m_dwSourceU;				//U8m
			Value[26]	= m_pAdjustData[24]/(10*m_dwSourceI);			//I1m
			Value[27]	= m_pAdjustData[25]/(10*m_dwSourceI);			//I2m
			Value[28]	= m_pAdjustData[26]/(10*m_dwSourceI);			//I3m
			Value[29]	= m_pAdjustData[27]/(10*m_dwSourceI);			//I4m
			Value[30]	= m_pAdjustData[28]/(10*m_dwSourceI);			//I5m
			Value[31]	= m_pAdjustData[29]/(10*m_dwSourceI);			//I6m
			Value[32]	= m_pAdjustData[30]/(10*m_dwSourceI);			//I7m
			Value[33]	= m_pAdjustData[31]/(10*m_dwSourceI);			//I8m			
			Value[34]	= 1000;
			Value[35]	= 100;
			Value[36]	= 10;
			Value[37]	= 1;	
			Value[38]   = m_lDriftU1;
			Value[39]	= m_lDriftU2;
			Value[40]	= m_lDriftU3;
			Value[41]	= m_lDriftU4;
			Value[42]	= m_lDriftU5;
			Value[43]	= m_lDriftU6;
			Value[44]	= m_lDriftU7;
			Value[45]	= m_lDriftU8;
			Value[46]	= m_lDriftI1;
			Value[47]	= m_lDriftI2;
			Value[48]	= m_lDriftI3;
			Value[49]	= m_lDriftI4;
			Value[50]	= m_lDriftI5;
			Value[51]	= m_lDriftI6;
			Value[52]	= m_lDriftI7;
			Value[53]	= m_lDriftI8;
			
			DB_SetCFValue(CF_VALUE_ALL, Value);				//写入系数值
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);	//写入系数确认
			CoefAdjustEnd(SUCCESS);
			return;
		}
	
	}
	// 系数校正中的电量计算
	CoefAdjustCalu();
	
	// 系数校正等待两秒钟后，首先开始源判断
	if(m_bIsSourceOK == FALSE)
	{
		LONG ElecVal[]={m_AdjustU1p.Mod,m_AdjustU2p.Mod,m_AdjustU3p.Mod,m_AdjustU4p.Mod,m_AdjustU5p.Mod,
			m_AdjustU6p.Mod,m_AdjustU7p.Mod,m_AdjustU8p.Mod,m_AdjustI1p.Mod,m_AdjustI2p.Mod,m_AdjustI3p.Mod,
			m_AdjustI4p.Mod,m_AdjustI5p.Mod,m_AdjustI6p.Mod,m_AdjustI7p.Mod,m_AdjustI8p.Mod};
		
		LONG ElecCoef[]={*CF_pnUP1,*CF_pnUP2,*CF_pnUP3,*CF_pnUP4,*CF_pnUP5,*CF_pnUP6
			,*CF_pnUP7,*CF_pnUP8,*CF_pnIP1,*CF_pnIP2,*CF_pnIP3,*CF_pnIP4,*CF_pnIP5
			,*CF_pnIP6,*CF_pnIP7,*CF_pnIP8};
		
		if(m_bIsDriftAdjust)
		{	
			for(LONG i=0;i<8;i++)
			{
				if(ElecVal[i] > ElecCoef[i]/200) // 与小值0.5V作比较
				{
					CoefAdjustEnd(FAILURE);
					return;
				}	
			}			
			for(LONG i=0;i<8;i++)
			{
				if(ElecVal[8+i] > ElecCoef[8+i]/5) // 与小值0.2A作比较
				{
					CoefAdjustEnd(FAILURE);
					return;
				}	
			}			
		}
		else
		{
			for(LONG i=0;i<8;i++)
			{				
				if((ElecVal[i]<(ElecCoef[i]*m_dwSourceU*9/1000))||(ElecVal[i]>(ElecCoef[i]*m_dwSourceU*11/1000)))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			for(LONG i=0;i<8;i++)
			{				
				if((ElecVal[8+i]<(ElecCoef[8+i]*m_dwSourceI*9/100))||(ElecVal[8+i]>(ElecCoef[8+i]*m_dwSourceI*11/100)))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			for(int i=1;i<16;i++)
			{
				if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
		}
		m_bIsSourceOK = TRUE;		
		return;
	}
//死区值校正时，取最大零漂值
	if(m_bIsDriftAdjust)
	{
		if(m_AdjustU1m>m_lMidU1m)
			m_lMidU1m = m_AdjustU1m;		
		if(m_AdjustU2m>m_lMidU2m)
			m_lMidU2m = m_AdjustU2m;		
		if(m_AdjustU3m>m_lMidU3m)
			m_lMidU3m = m_AdjustU3m;		
		if(m_AdjustU4m>m_lMidU4m)
			m_lMidU4m = m_AdjustU4m;		
		if(m_AdjustU5m>m_lMidU5m)
			m_lMidU5m = m_AdjustU5m;		
		if(m_AdjustU6m>m_lMidU6m)
			m_lMidU6m = m_AdjustU6m;		
		if(m_AdjustU7m>m_lMidU7m)
			m_lMidU7m = m_AdjustU7m;		
		if(m_AdjustU8m>m_lMidU8m)
			m_lMidU8m = m_AdjustU8m;		
		if(m_AdjustI1m>m_lMidI1m)
			m_lMidI1m = m_AdjustI1m;		
		if(m_AdjustI2m>m_lMidI2m)
			m_lMidI2m = m_AdjustI2m;		
		if(m_AdjustI3m>m_lMidI3m)
			m_lMidI3m = m_AdjustI3m;		
		if(m_AdjustI4m>m_lMidI4m)
			m_lMidI4m = m_AdjustI4m;		
		if(m_AdjustI5m>m_lMidI5m)
			m_lMidI5m = m_AdjustI5m;		
		if(m_AdjustI6m>m_lMidI6m)
			m_lMidI6m = m_AdjustI6m;		
		if(m_AdjustI7m>m_lMidI7m)
			m_lMidI7m = m_AdjustI7m;		
		if(m_AdjustI8m>m_lMidI8m)
			m_lMidI8m = m_AdjustI8m;
	}
//系数校正，取排序后的中间值
	else
	{
		m_lMidU1p = AiBubbleSort(m_AdjustU1p.Mod,AArrayU1p);
		m_lMidU2p = AiBubbleSort(m_AdjustU2p.Mod,AArrayU2p);
		m_lMidU3p = AiBubbleSort(m_AdjustU3p.Mod,AArrayU3p);
		m_lMidU4p = AiBubbleSort(m_AdjustU4p.Mod,AArrayU4p);
		m_lMidU5p = AiBubbleSort(m_AdjustU5p.Mod,AArrayU5p);
		m_lMidU6p = AiBubbleSort(m_AdjustU6p.Mod,AArrayU6p);
		m_lMidU7p = AiBubbleSort(m_AdjustU7p.Mod,AArrayU7p);
		m_lMidU8p = AiBubbleSort(m_AdjustU8p.Mod,AArrayU8p);	
		m_lMidI1p = AiBubbleSort(m_AdjustI1p.Mod,AArrayI1p);
		m_lMidI2p = AiBubbleSort(m_AdjustI2p.Mod,AArrayI2p);
		m_lMidI3p = AiBubbleSort(m_AdjustI3p.Mod,AArrayI3p);
		m_lMidI4p = AiBubbleSort(m_AdjustI4p.Mod,AArrayI4p);
		m_lMidI5p = AiBubbleSort(m_AdjustI5p.Mod,AArrayI5p);
		m_lMidI6p = AiBubbleSort(m_AdjustI6p.Mod,AArrayI6p);
		m_lMidI7p = AiBubbleSort(m_AdjustI7p.Mod,AArrayI7p);
		m_lMidI8p = AiBubbleSort(m_AdjustI8p.Mod,AArrayI8p);
		
		m_lMidU1m = AiBubbleSort(m_AdjustU1m,AArrayU1m);
		m_lMidU2m = AiBubbleSort(m_AdjustU2m,AArrayU2m);
		m_lMidU3m = AiBubbleSort(m_AdjustU3m,AArrayU3m);
		m_lMidU4m = AiBubbleSort(m_AdjustU4m,AArrayU4m);
		m_lMidU5m = AiBubbleSort(m_AdjustU5m,AArrayU5m);
		m_lMidU6m = AiBubbleSort(m_AdjustU6m,AArrayU6m);
		m_lMidU7m = AiBubbleSort(m_AdjustU7m,AArrayU7m);
		m_lMidU8m = AiBubbleSort(m_AdjustU8m,AArrayU8m);
		m_lMidI1m = AiBubbleSort(m_AdjustI1m,AArrayI1m);
		m_lMidI2m = AiBubbleSort(m_AdjustI2m,AArrayI2m);
		m_lMidI3m = AiBubbleSort(m_AdjustI3m,AArrayI3m);
		m_lMidI4m = AiBubbleSort(m_AdjustI4m,AArrayI4m);
		m_lMidI5m = AiBubbleSort(m_AdjustI5m,AArrayI5m);
		m_lMidI6m = AiBubbleSort(m_AdjustI6m,AArrayI6m);
		m_lMidI7m = AiBubbleSort(m_AdjustI7m,AArrayI7m);
		m_lMidI8m = AiBubbleSort(m_AdjustI8m,AArrayI8m);
	}
	
	// 系数校正之初，等待两秒钟
	if(m_dwWaitCnt <2000/AI_TIMER_PERIOD)		
	{
		m_dwWaitCnt++;
		return;
	}
	if(m_bIsDriftAdjust)
	{
		m_pAdjustData[16] = m_lMidU1m;
		m_pAdjustData[17] = m_lMidU2m;
		m_pAdjustData[18] = m_lMidU3m;
		m_pAdjustData[19] = m_lMidU4m;
		m_pAdjustData[20] = m_lMidU5m;
		m_pAdjustData[21] = m_lMidU6m;
		m_pAdjustData[22] = m_lMidU7m;
		m_pAdjustData[23] = m_lMidU8m;	
		m_pAdjustData[24] = m_lMidI1m;	
		m_pAdjustData[25] = m_lMidI2m;	
		m_pAdjustData[26] = m_lMidI3m;	
		m_pAdjustData[27] = m_lMidI4m;	
		m_pAdjustData[28] = m_lMidI5m;	
		m_pAdjustData[29] = m_lMidI6m;	
		m_pAdjustData[30] = m_lMidI7m;	
		m_pAdjustData[31] = m_lMidI8m;		
	}
	else
	{
		m_pAdjustData[0]  += m_lMidU1p;
		m_pAdjustData[1]  += m_lMidU2p;
		m_pAdjustData[2]  += m_lMidU3p;
		m_pAdjustData[3]  += m_lMidU4p;
		m_pAdjustData[4]  += m_lMidU5p;
		m_pAdjustData[5]  += m_lMidU6p;
		m_pAdjustData[6]  += m_lMidU7p;
		m_pAdjustData[7]  += m_lMidU8p;
		m_pAdjustData[8]  += m_lMidI1p;
		m_pAdjustData[9]  += m_lMidI2p;
		m_pAdjustData[10] += m_lMidI3p;
		m_pAdjustData[11] += m_lMidI4p;
		m_pAdjustData[12] += m_lMidI5p;
		m_pAdjustData[13] += m_lMidI6p;
		m_pAdjustData[14] += m_lMidI7p;
		m_pAdjustData[15] += m_lMidI8p;
		
		m_pAdjustData[16] += m_lMidU1m;
		m_pAdjustData[17] += m_lMidU2m;
		m_pAdjustData[18] += m_lMidU3m;
		m_pAdjustData[19] += m_lMidU4m;
		m_pAdjustData[20] += m_lMidU5m;
		m_pAdjustData[21] += m_lMidU6m;
		m_pAdjustData[22] += m_lMidU7m;
		m_pAdjustData[23] += m_lMidU8m;	
		m_pAdjustData[24] += m_lMidI1m;	
		m_pAdjustData[25] += m_lMidI2m;	
		m_pAdjustData[26] += m_lMidI3m;	
		m_pAdjustData[27] += m_lMidI4m;	
		m_pAdjustData[28] += m_lMidI5m;	
		m_pAdjustData[29] += m_lMidI6m;	
		m_pAdjustData[30] += m_lMidI7m;	
		m_pAdjustData[31] += m_lMidI8m;		
	}
	
	m_dwAdjustCnt++;

}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<32;i++)     
		m_pAdjustData[i]=0;

	//系数校正计算次数计数器清零
	m_dwAdjustCnt =0;
	m_dwWaitCnt   =0;
	//清系数校正任务标志
	m_bIsCoefAdjust = FALSE;
	m_bIsSourceOK = FALSE;
	
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







































