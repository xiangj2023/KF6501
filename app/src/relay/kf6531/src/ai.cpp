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
	m_CoefAdjustMask = ( (0x01<<CHANNEL_6531_UHA)|(0x01<<CHANNEL_6531_UHB)|(0x01<<CHANNEL_6531_UHC)|\
						(0x01<<CHANNEL_6531_U0)|(0x01<<CHANNEL_6531_ULA)|(0x01<<CHANNEL_6531_ULB)|\
						(0x01<<CHANNEL_6531_IHA)|(0x01<<CHANNEL_6531_IHB)|(0x01<<CHANNEL_6531_IHC)|
						(0x01<<CHANNEL_6531_I0)|(0x01<<CHANNEL_6531_I1)|(0x01<<CHANNEL_6531_I2)|
						(0x01<<CHANNEL_6531_I3)|(0x01<<CHANNEL_6531_I4));
	
	m_pCoefAdjustBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*14);
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*14);
	m_pComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*14);

	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*14);
	
	ASSERT(m_pComposeBuf != NULL);
	if(m_pComposeBuf == NULL)
	{
		LogError("CAIProc,m_pComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*3);


	for(LONG i=0;i<42;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		AdjustUHA[i] = 0;
		AdjustUHB[i] = 0;
		AdjustUHC[i] = 0;
		AdjustU0[i]  = 0;
		AdjustULA[i] = 0;
		AdjustULB[i] = 0;
		AdjustIHA[i] = 0;
		AdjustIHB[i] = 0;
		AdjustIHC[i] = 0;
		AdjustI0[i]  = 0;
		AdjustIL1[i] = 0;
		AdjustIL2[i] = 0;
		AdjustIL3[i] = 0;
		AdjustIL4[i] = 0;
		m_pBubbleData[i]=0;

		
		AdjustUHAM[i] = 0;
		AdjustUHBM[i] = 0;
		AdjustUHCM[i] = 0;
		AdjustU0M[i]  = 0;
		AdjustULAM[i] = 0;
		AdjustULBM[i] = 0;
		AdjustIHAM[i] = 0;
		AdjustIHBM[i] = 0;
		AdjustIHCM[i] = 0;
		AdjustI0M[i]  = 0;
		AdjustIL1M[i] = 0;
		AdjustIL2M[i] = 0;
		AdjustIL3M[i] = 0;
		AdjustIL4M[i] = 0;
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

	for(LONG i=0;i<14;i++)
		Filter(m_pDataBuf+READ_POINT_NUM*i);

	CaluMSMod(m_pDataBuf,Uha);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,Uhb);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,Uhc);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,U0);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,Ula);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,Ulb);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,Iha);	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,Ihb);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,Ihc);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,I0);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*10,I1);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*11,I2);	
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*12,I3);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*13,I4);

	WriteAIData(M_UHA,Uha,*CF_pnUHAD,m_pBubbleData,ArrayUHAM,TRUE);
	WriteAIData(M_UHB,Uhb,*CF_pnUHBD,m_pBubbleData,ArrayUHBM,TRUE);
	WriteAIData(M_UHC,Uhc,*CF_pnUHCD,m_pBubbleData,ArrayUHCM,TRUE);
	WriteAIData(M_U0,U0,*CF_pnU0D,m_pBubbleData,ArrayU0M,TRUE);
	WriteAIData(M_ULA,Ula,*CF_pnULAD,m_pBubbleData,ArrayULAM,TRUE);
	WriteAIData(M_ULB,Ulb,*CF_pnULBD,m_pBubbleData,ArrayULBM,TRUE);
	WriteAIData(M_IHA,Iha,*CF_pnIHAD,m_pBubbleData,ArrayIHAM,TRUE);
	WriteAIData(M_IHB,Ihb,*CF_pnIHBD,m_pBubbleData,ArrayIHBM,TRUE);
	WriteAIData(M_IHC,Ihc,*CF_pnIHCD,m_pBubbleData,ArrayIHCM,TRUE);
	WriteAIData(M_I0,I0,*CF_pnI0D,m_pBubbleData,ArrayI0M,TRUE);
	WriteAIData(M_IL1,I1,*CF_pnIL1D,m_pBubbleData,ArrayIL1M,TRUE);
	WriteAIData(M_IL2,I2,*CF_pnIL2D,m_pBubbleData,ArrayIL2M,TRUE);	
	WriteAIData(M_IL3,I3,*CF_pnIL3D,m_pBubbleData,ArrayIL3M,TRUE);
	WriteAIData(M_IL4,I4,*CF_pnIL4D,m_pBubbleData,ArrayIL4M,TRUE);

	if(*AS_pnLCN == LCUR_2)
	{		
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{	
			m_pComposeBuf[i] = m_pDataBuf[READ_POINT_NUM*10+i];
			m_pComposeBuf[READ_POINT_NUM+i] = m_pDataBuf[READ_POINT_NUM*12+i];
			m_pComposeBuf[READ_POINT_NUM*2+i] = m_pDataBuf[READ_POINT_NUM*13+i];
		}
	}
	else if(*AS_pnLCN == LCUR_4)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{	
			m_pComposeBuf[i] = m_pDataBuf[READ_POINT_NUM*10+i]- m_pDataBuf[READ_POINT_NUM*11+i];
			m_pComposeBuf[READ_POINT_NUM+i] = m_pDataBuf[READ_POINT_NUM*12+i] - m_pDataBuf[READ_POINT_NUM*13+i];
			m_pComposeBuf[READ_POINT_NUM*2+i] = 0;
		}
	}
	else
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{	
			m_pComposeBuf[i] = 0;
			m_pComposeBuf[READ_POINT_NUM+i] = 0;
			m_pComposeBuf[READ_POINT_NUM*2+i] = 0;
		}
	}

	for(LONG i=0;i<3;i++)
		Filter(m_pComposeBuf+READ_POINT_NUM*i);
	CaluMSMod(m_pComposeBuf,Ila);
	CaluMSMod(m_pComposeBuf+READ_POINT_NUM,Ilb);
	CaluMSMod(m_pComposeBuf+READ_POINT_NUM*2,Ijx);

	WriteAIData(M_ILA,Ila,m_pBubbleData,ArrayILAM);	
	WriteAIData(M_ILB,Ilb,m_pBubbleData,ArrayILBM);	
	WriteAIData(M_IJX,Ijx,m_pBubbleData,ArrayIJXM);	
}

void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{	
	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDriftAdjust = TRUE;
		UHADrift	= 0;
		UHBDrift	= 0;
		UHCDrift	= 0;
		U0Drift		= 0;
		ULADrift	= 0;
		ULBDrift	= 0;
		IHADrift	= 0;
		IHBDrift	= 0;
		IHCDrift	= 0;
		I0Drift		= 0;
		IL1Drift	= 0;
		IL2Drift	= 0;
		IL3Drift	= 0;
		IL4Drift	= 0;
	}
	else
		m_bIsDriftAdjust = FALSE;
	
	m_bIsCoefAdjust = TRUE;
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	dwMsgID = arg;
	
	MidUHA = 0;
	MidUHB = 0;
	MidUHC = 0;
	MidU0 = 0;
	MidULA = 0;
	MidULB = 0;
	MidIHA = 0;
	MidIHB = 0;
	MidIHC = 0;
	MidI0 = 0;
	MidIL1= 0;
	MidIL2 = 0;
	MidIL3= 0;
	MidIL4 = 0;
	
	MidUHA = 0;
	MidUHB = 0;
	MidUHC = 0;
	MidU0 = 0;
	MidULA = 0;
	MidULB = 0;
	MidIHA = 0;
	MidIHB = 0;
	MidIHC = 0;
	MidI0 = 0;
	MidIL1 = 0;
	MidIL2 = 0;
	MidIL3 = 0;
	MidIL4 = 0;
	for(LONG i=0;i<42;i++)
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
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf,&m_AdjustUHA);
	CaluModValue(&m_AdjustUHA);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&m_AdjustUHB);
	CaluModValue(&m_AdjustUHB);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&m_AdjustUHC);
	CaluModValue(&m_AdjustUHC);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&m_AdjustU0);
	CaluModValue(&m_AdjustU0);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&m_AdjustULA);
	CaluModValue(&m_AdjustULA);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&m_AdjustULB);
	CaluModValue(&m_AdjustULB);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&m_AdjustIHA);
	CaluModValue(&m_AdjustIHA);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&m_AdjustIHB);
	CaluModValue(&m_AdjustIHB);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&m_AdjustIHC);
	CaluModValue(&m_AdjustIHC);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&m_AdjustI0);
	CaluModValue(&m_AdjustI0);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*10,&m_AdjustIL1);
	CaluModValue(&m_AdjustIL1);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*11,&m_AdjustIL2);
	CaluModValue(&m_AdjustIL2);		
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*12,&m_AdjustIL3);
	CaluModValue(&m_AdjustIL3);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*13,&m_AdjustIL4);
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
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*10);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*11);	
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*12);
	Filter(m_pCoefAdjustBuf+READ_POINT_NUM*13);

	CaluMSMod(m_pCoefAdjustBuf,m_adjUHAM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,m_adjUHBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_adjUHCM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,m_adjU0M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,m_adjULAM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,m_adjULBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,m_adjIHAM);	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,m_adjIHBM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,m_adjIHCM);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,m_adjI0M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*10,m_adjIL1M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*11,m_adjIL2M);	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*12,m_adjIL3M);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*13,m_adjIL4M);

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
		if(m_bIsDriftAdjust == TRUE)
		{			
			UHADrift = m_pAdjustData[28]; 
			UHBDrift = m_pAdjustData[29]; 
			UHCDrift = m_pAdjustData[30]; 
			U0Drift  = m_pAdjustData[31]; 
			ULADrift = m_pAdjustData[32]; 
			ULBDrift = m_pAdjustData[33]; 
			IHADrift = m_pAdjustData[34]; 
			IHBDrift = m_pAdjustData[35]; 
			IHCDrift = m_pAdjustData[36]; 
			I0Drift  = m_pAdjustData[37]; 
			IL1Drift = m_pAdjustData[38]; 
			IL2Drift = m_pAdjustData[39]; 
			IL3Drift = m_pAdjustData[40];
			IL4Drift = m_pAdjustData[41];
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{
			LONG Value[48];			
			
			Value[0] = (m_pAdjustData[0]+m_pAdjustData[1])/(2*m_dwSourceU);  //UHAB
			Value[1] = (m_pAdjustData[1]+m_pAdjustData[2])/(2*m_dwSourceU);	//UHBC
			Value[2] = (m_pAdjustData[0]+m_pAdjustData[2])/(2*m_dwSourceU);	//UHCA
			Value[3] = m_pAdjustData[0]/m_dwSourceU;	//UHA
			Value[4] = m_pAdjustData[1]/m_dwSourceU;	//UHB
			Value[5] = m_pAdjustData[2]/m_dwSourceU;	//UHC
			Value[6] = m_pAdjustData[3]/m_dwSourceU;	//U0
			Value[7] = m_pAdjustData[4]/m_dwSourceU;	//ULA
			Value[8] = m_pAdjustData[5]/m_dwSourceU;	//ULB
			Value[9] = m_pAdjustData[6]/(m_dwSourceI*10);	//IHA
			Value[10] = m_pAdjustData[7]/(m_dwSourceI*10);	//IHB
			Value[11] = m_pAdjustData[8]/(m_dwSourceI*10);	//IHC
			Value[12] = m_pAdjustData[9]/(m_dwSourceI*10);	//IH0
			Value[13] = m_pAdjustData[10]/(m_dwSourceI*10);	//IL1
			Value[14] = m_pAdjustData[11]/(m_dwSourceI*10);	//IL2
			Value[15] = m_pAdjustData[12]/(m_dwSourceI*10);	//IL3
			Value[16] = m_pAdjustData[13]/(m_dwSourceI*10);	//IL4;
			Value[17] = 1000;
			Value[18] = 100;
			Value[19] = 10;		
			Value[20] = 1; 
			Value[21] = m_pAdjustData[14]/m_dwSourceU; //UHAM
			Value[22] = m_pAdjustData[15]/m_dwSourceU; //UHBM
			Value[23] = m_pAdjustData[16]/m_dwSourceU; //UHCM
			Value[24] = m_pAdjustData[17]/m_dwSourceU; //UH0M
			Value[25] = m_pAdjustData[18]/m_dwSourceU; //ULAM
			Value[26] = m_pAdjustData[19]/m_dwSourceU; //ULBM
			Value[27] = m_pAdjustData[20]/(m_dwSourceI*10); //IHAM
			Value[28] = m_pAdjustData[21]/(m_dwSourceI*10); //IHBM
			Value[29] = m_pAdjustData[22]/(m_dwSourceI*10); //IHCM
			Value[30] = m_pAdjustData[23]/(m_dwSourceI*10); //I0M
			Value[31] = m_pAdjustData[24]/(m_dwSourceI*10); //IL1M
			Value[32] = m_pAdjustData[25]/(m_dwSourceI*10); //IL2M
			Value[33] = m_pAdjustData[26]/(m_dwSourceI*10); //IL3M
			Value[34] = m_pAdjustData[27]/(m_dwSourceI*10); //IL4M
			Value[35] = UHADrift;
			Value[36] = UHBDrift;
			Value[37] = UHCDrift;
			Value[38] = U0Drift;
			Value[39] = ULADrift;
			Value[40] = ULBDrift;
			Value[41] = IHADrift;
			Value[42] = IHBDrift;
			Value[43] = IHCDrift;
			Value[44] = I0Drift;
			Value[45] = IL1Drift;
			Value[46] = IL2Drift;
			Value[47] = IL3Drift;
			Value[48] = IL4Drift;
				
			DB_SetCFValue(CF_VALUE_ALL, Value);					//写入系数值
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);		//写入系数确认
			CoefAdjustEnd(SUCCESS);
			return;
		}
	
	}
	
	CoefAdjustCalu();

	if(!m_bIsHaveAdjustSource)  //首先校正角差及电压电流源检测
	{
		if(m_bIsDriftAdjust)//是进行的死区值校正
		{
			if((m_AdjustUHA.Mod>(*CF_pnUHA/200))||(m_AdjustUHB.Mod>(*CF_pnUHB/200))||(m_AdjustUHC.Mod>(*CF_pnUHC/200))||(m_AdjustU0.Mod>(*CF_pnU0/200))||\
				(m_AdjustULA.Mod>(*CF_pnULA/200))||(m_AdjustULA.Mod>(*CF_pnULB/200)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}			
			if((m_AdjustIHA.Mod>(*CF_pnIHA/50))||(m_AdjustIHB.Mod>(*CF_pnIHB/50))||(m_AdjustIHB.Mod>(*CF_pnIHC/50))||(m_AdjustI0.Mod>(*CF_pnI0/50))||\
				(m_AdjustIL1.Mod>(*CF_pnIL1/50))||(m_AdjustIL2.Mod>(*CF_pnIL2/50))||(m_AdjustIL3.Mod>(*CF_pnIL3/50))||(m_AdjustIL4.Mod>(*CF_pnIL4/50)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
		}
		else
		{		
			if((m_AdjustUHA.Mod>(*CF_pnUHA*m_dwSourceU*11/1000))||(m_AdjustUHA.Mod<(*CF_pnUHA*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}		
			if((m_AdjustUHB.Mod>(*CF_pnUHB*m_dwSourceU*11/1000))||(m_AdjustUHB.Mod<(*CF_pnUHB*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustUHC.Mod>(*CF_pnUHC*m_dwSourceU*11/1000))||(m_AdjustUHC.Mod<(*CF_pnUHC*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}	
			if((m_AdjustU0.Mod>(*CF_pnU0*m_dwSourceU*11/1000))||(m_AdjustU0.Mod<(*CF_pnU0*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustULA.Mod>(*CF_pnULA*m_dwSourceU*11/1000))||(m_AdjustULA.Mod<(*CF_pnULA*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustULB.Mod>(*CF_pnULB*m_dwSourceU*11/1000))||(m_AdjustULB.Mod<(*CF_pnULB*m_dwSourceU*9/1000)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}		
			if((m_AdjustIHA.Mod>(*CF_pnIHA*m_dwSourceI*11/100))||(m_AdjustIHA.Mod<(*CF_pnIHA*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHB.Mod>(*CF_pnIHB*m_dwSourceI*11/100))||(m_AdjustIHB.Mod<(*CF_pnIHB*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIHC.Mod>(*CF_pnIHC*m_dwSourceI*11/100))||(m_AdjustIHC.Mod<(*CF_pnIHC*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustI0.Mod>(*CF_pnI0*m_dwSourceI*11/100))||(m_AdjustI0.Mod<(*CF_pnI0*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL1.Mod>(*CF_pnIL1*m_dwSourceI*11/100))||(m_AdjustIL1.Mod<(*CF_pnIL1*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL2.Mod>(*CF_pnIL2*m_dwSourceI*11/100))||(m_AdjustIL2.Mod<(*CF_pnIL2*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL3.Mod>(*CF_pnIL3*m_dwSourceI*11/100))||(m_AdjustIL3.Mod<(*CF_pnIL3*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if((m_AdjustIL4.Mod>(*CF_pnIL4*m_dwSourceI*11/100))||(m_AdjustIL4.Mod<(*CF_pnIL4*m_dwSourceI*9/100)))
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			for(int i=1;i<14;i++)
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
	if(m_bIsDriftAdjust) //死区值校正，取得最大零漂值
	{
		if(m_adjUHAM>MidUHAM)
			MidUHAM = m_adjUHAM;
		if(m_adjUHBM>MidUHBM)
			MidUHBM = m_adjUHBM;
		if(m_adjUHCM>MidUHCM)
			MidUHCM = m_adjUHCM;
		if(m_adjU0M>MidU0M)
			MidU0M = m_adjU0M;
		if(m_adjULAM>MidULAM)
			MidULAM = m_adjULAM;
		if(m_adjULBM>MidULBM)
			MidULBM = m_adjULBM;		
		if(m_adjIHAM>MidIHAM)
			MidIHAM = m_adjIHAM;		
		if(m_adjIHBM>MidIHBM)
			MidIHBM = m_adjIHBM;
		if(m_adjIHCM>MidIHCM)
			MidIHCM = m_adjIHCM;
		if(m_adjI0M>MidI0M)
			MidI0M = m_adjI0M;
		if(m_adjIL1M>MidIL1M)
			MidIL1M = m_adjIL1M;
		if(m_adjIL2M>MidIL2M)
			MidIL2M = m_adjIL2M;		
		if(m_adjIL3M>MidIL3M)
			MidIL3M = m_adjIL3M;
		if(m_adjIL4M>MidIL4M)
			MidIL4M = m_adjIL4M;
	}
	else
	{
		MidUHA = CoefAdjustSort(m_AdjustUHA.Mod,AdjustUHA);
		MidUHB = CoefAdjustSort(m_AdjustUHB.Mod,AdjustUHB);
		MidUHC = CoefAdjustSort(m_AdjustUHC.Mod,AdjustUHC);
		MidU0  = CoefAdjustSort(m_AdjustU0.Mod,AdjustU0);
		MidULA = CoefAdjustSort(m_AdjustULA.Mod,AdjustULA);
		MidULB = CoefAdjustSort(m_AdjustULB.Mod,AdjustULB);
		MidIHA = CoefAdjustSort(m_AdjustIHA.Mod,AdjustIHA);
		MidIHB = CoefAdjustSort(m_AdjustIHB.Mod,AdjustIHB);
		MidIHC = CoefAdjustSort(m_AdjustIHC.Mod,AdjustIHC);
		MidI0  = CoefAdjustSort(m_AdjustI0.Mod,AdjustI0);
		MidIL1 = CoefAdjustSort(m_AdjustIL1.Mod,AdjustIL1);
		MidIL2 = CoefAdjustSort(m_AdjustIL2.Mod,AdjustIL2);
		MidIL3 = CoefAdjustSort(m_AdjustIL3.Mod,AdjustIL3);
		MidIL4 = CoefAdjustSort(m_AdjustIL4.Mod,AdjustIL4);

		MidUHAM = CoefAdjustSort(m_adjUHAM,AdjustUHAM);
		MidUHBM = CoefAdjustSort(m_adjUHBM,AdjustUHBM);
		MidUHCM = CoefAdjustSort(m_adjUHCM,AdjustUHCM);
		MidU0M  = CoefAdjustSort(m_adjU0M,AdjustU0M);
		MidULAM = CoefAdjustSort(m_adjULAM,AdjustULAM);
		MidULBM = CoefAdjustSort(m_adjULBM,AdjustULBM);	
		MidIHAM = CoefAdjustSort(m_adjIHAM,AdjustIHAM);
		MidIHBM = CoefAdjustSort(m_adjIHBM,AdjustIHBM);
		MidIHCM = CoefAdjustSort(m_adjIHCM,AdjustIHCM);
		MidI0M  = CoefAdjustSort(m_adjI0M,AdjustI0M);
		MidIL1M = CoefAdjustSort(m_adjIL1M,AdjustIL1M);
		MidIL2M = CoefAdjustSort(m_adjIL2M,AdjustIL2M);
		MidIL3M = CoefAdjustSort(m_adjIL3M,AdjustIL3M);
		MidIL4M = CoefAdjustSort(m_adjIL4M,AdjustIL4M);
	}
		
	if(m_dwWaitCnt < (2000/AI_TIMER_PERIOD)) //系数校正，源校验正确后等待两秒钟，保证排序数组填满
	{
		m_dwWaitCnt++;
		return;
	}
	if(m_bIsDriftAdjust) //死区值校正，取得最大零漂值
	{
		m_pAdjustData[28] = MidUHAM;
		m_pAdjustData[29] = MidUHBM;
		m_pAdjustData[30] = MidUHCM;
		m_pAdjustData[31] = MidU0M;
		m_pAdjustData[32] = MidULAM;
		m_pAdjustData[33] = MidULBM;
		m_pAdjustData[34] = MidIHAM;
		m_pAdjustData[35] = MidIHBM;
		m_pAdjustData[36] = MidIHCM;
		m_pAdjustData[37] = MidI0M;
		m_pAdjustData[38] = MidIL1M;
		m_pAdjustData[39] = MidIL2M;
		m_pAdjustData[40] = MidIL3M;
		m_pAdjustData[41] = MidIL4M;
	}
	else
	{
		m_pAdjustData[0] += MidUHA;
		m_pAdjustData[1] += MidUHB;
		m_pAdjustData[2] += MidUHC;
		m_pAdjustData[3] += MidU0;
		m_pAdjustData[4] += MidULA;
		m_pAdjustData[5] += MidULB;
		m_pAdjustData[6] += MidIHA;
		m_pAdjustData[7] += MidIHB;
		m_pAdjustData[8] += MidIHC;
		m_pAdjustData[9] += MidI0;
		m_pAdjustData[10] += MidIL1;
		m_pAdjustData[11] += MidIL2;
		m_pAdjustData[12] += MidIL3;
		m_pAdjustData[13] += MidIL4;
		
		m_pAdjustData[14] += MidUHAM;
		m_pAdjustData[15] += MidUHBM;
		m_pAdjustData[16] += MidUHCM;
		m_pAdjustData[17] += MidU0M;
		m_pAdjustData[18] += MidULAM;
		m_pAdjustData[19] += MidULBM;
		m_pAdjustData[20] += MidIHAM;
		m_pAdjustData[21] += MidIHBM;
		m_pAdjustData[22] += MidIHCM;
		m_pAdjustData[23] += MidI0M;
		m_pAdjustData[24] += MidIL1M;
		m_pAdjustData[25] += MidIL2M;
		m_pAdjustData[26] += MidIL3M;
		m_pAdjustData[27] += MidIL4M;
	}	
	m_dwAdjustCnt++;
	

}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<42;i++) 
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




