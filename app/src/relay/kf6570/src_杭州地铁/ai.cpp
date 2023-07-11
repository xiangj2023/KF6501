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
#include "relay_defs.h"

DWORD dwAITaskID;
extern DWORD g_hLCDTaskID;
extern g_dwFreqPWM;
extern TSysConfigTable	*G_pCfgTable;

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
	m_AIDataMask = ( (0x01<<CHANNEL_6570_UHA)|(0x01<<CHANNEL_6570_UHB)|(0x01<<CHANNEL_6570_UHC)|(0x01<<CHANNEL_6570_U0) \
					|(0x01<<CHANNEL_6570_ULA)|(0x01<<CHANNEL_6570_ULB)|(0x01<<CHANNEL_6570_ULC)|(0x01<<CHANNEL_6570_IHA) \
					|(0x01<<CHANNEL_6570_IHB)|(0x01<<CHANNEL_6570_IHC)|(0x01<<CHANNEL_6570_ILA) \
					|(0x01<<CHANNEL_6570_ILB)|(0x01<<CHANNEL_6570_ILC)|(0x01<<CHANNEL_6570_I1)|(0x01<<CHANNEL_6570_I2));
	
	m_CoefAdjustDataMask = ( (0x01<<CHANNEL_6570_UHA)|(0x01<<CHANNEL_6570_UHB)|(0x01<<CHANNEL_6570_UHC)|(0x01<<CHANNEL_6570_U0) \
					|(0x01<<CHANNEL_6570_ULA)|(0x01<<CHANNEL_6570_ULB)|(0x01<<CHANNEL_6570_ULC)|(0x01<<CHANNEL_6570_IHA) \
					|(0x01<<CHANNEL_6570_IHB)|(0x01<<CHANNEL_6570_IHC)|(0x01<<CHANNEL_6570_ILA) \
					|(0x01<<CHANNEL_6570_ILB)|(0x01<<CHANNEL_6570_ILC)|(0x01<<CHANNEL_6570_I1)|(0x01<<CHANNEL_6570_I2));

	m_dwFreqMask = 0x01;
	
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*15);
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);

	m_pCoefAdjustBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*15);
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*15);
	
	m_pComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	ASSERT(m_pComposeBuf != NULL);
	if(m_pComposeBuf == NULL)
	{
		LogError("CAIProc,m_pComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*3);


	m_wFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*POINT_PER_PERIOD*FREQBUFSIZE);
	ASSERT(m_wFreqBuf != NULL);
	if(m_wFreqBuf == NULL)
	{
		LogError("CAIProc,m_wFreqBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_wFreqBuf, 0, sizeof(SHORT)*POINT_PER_PERIOD*FREQBUFSIZE);

	m_FreqCaluBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*POINT_PER_PERIOD*FREQBUFSIZE);	
	ASSERT(m_FreqCaluBuf != NULL);
	if(m_FreqCaluBuf == NULL)
	{
		LogError("CAIProc,m_FreqCaluBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_FreqCaluBuf, 0, sizeof(DWORD)*POINT_PER_PERIOD*FREQBUFSIZE);

	for(LONG i=0;i<28;i++)
		m_pAdjustData[i]=0;

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		ArrayUHA[i]=0;
		ArrayUHB[i]=0;
		ArrayUHC[i]=0;
		ArrayU0[i]=0;
		ArrayULA[i]=0;
		ArrayULB[i]=0;
		ArrayULC[i]=0;
		ArrayIHA[i]=0;
		ArrayIHB[i]=0;
		ArrayIHC[i]=0;
		ArrayILA[i]=0;
		ArrayILB[i]=0;
		ArrayILC[i]=0;
		ArrayI1[i]=0;
		ArrayI2[i]=0;
		ArrayPA[i]=0;
		ArrayQA[i]=0;
		ArrayCOSA[i]=0;
		ArrayPB[i]=0;
		ArrayQB[i]=0;
		ArrayCOSB[i]=0;
		ArrayPC[i]=0;
		ArrayQC[i]=0;
		ArrayCOSC[i]=0;
		ArrayP[i]=0;
		ArrayQ[i]=0;
		ArrayCOS[i]=0;
		ArrayUHAB[i]=0;
		ArrayUHBC[i]=0;
		ArrayUHCA[i]=0;
		ArrayUHA1[i]=0;
		ArrayUHA2[i]=0;
		ArrayUHA3[i]=0;
		ArrayUHA5[i]=0;
		ArrayUHA7[i]=0;
		ArrayUHA9[i]=0;
		ArrayUHA11[i]=0;
		ArrayUHB1[i]=0;
		ArrayUHB2[i]=0;
		ArrayUHB3[i]=0;
		ArrayUHB5[i]=0;
		ArrayUHB7[i]=0;
		ArrayUHB9[i]=0;
		ArrayUHB11[i]=0;
		ArrayUHC1[i]=0;
		ArrayUHC2[i]=0;
		ArrayUHC3[i]=0;
		ArrayUHC5[i]=0;
		ArrayUHC7[i]=0;
		ArrayUHC9[i]=0;
		ArrayUHC11[i]=0;
		ArrayIHA1[i]=0;
		ArrayIHA2[i]=0;
		ArrayIHA3[i]=0;
		ArrayIHA5[i]=0;
		ArrayIHA7[i]=0;
		ArrayIHA9[i]=0;
		ArrayIHA11[i]=0;
		ArrayIHB1[i]=0;
		ArrayIHB2[i]=0;
		ArrayIHB3[i]=0;
		ArrayIHB5[i]=0;
		ArrayIHB7[i]=0;
		ArrayIHB9[i]=0;
		ArrayIHB11[i]=0;
		ArrayIHC1[i]=0;
		ArrayIHC2[i]=0;
		ArrayIHC3[i]=0;
		ArrayIHC5[i]=0;
		ArrayIHC7[i]=0;
		ArrayIHC9[i]=0;
		ArrayIHC11[i]=0;
		//系数校正部分		
		ArrayAdjUHA[i] = 0;
		ArrayAdjUHB[i] = 0;
		ArrayAdjUHC[i] = 0;
		ArrayAdjU0[i]  = 0;
		ArrayAdjULA[i] = 0;
		ArrayAdjULB[i] = 0;
		ArrayAdjULC[i] = 0;
		ArrayAdjIHA[i] = 0;
		ArrayAdjIHB[i] = 0;
		ArrayAdjIHC[i] = 0;
		ArrayAdjILA[i] = 0;
		ArrayAdjILB[i] = 0;
		ArrayAdjILC[i] = 0;
		ArrayAdjI1[i]  = 0;
		ArrayAdjI2[i]  = 0;
		ArrayAdjPA[i]  = 0;
		ArrayAdjQA[i]  = 0;
		ArrayAdjPB[i]  = 0;
		ArrayAdjQB[i]  = 0;
		ArrayAdjPC[i]  = 0;
		ArrayAdjQC[i]  = 0;		
	}
	//初始化系数校准标志
	m_bIsCoefAdjust = FALSE;
	m_bIsSourceOK =FALSE;
	m_dwAdjustCnt=0;
	m_FreqFlag = FALSE;
	m_dwPreFreq = 50000;
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
	LONG angle;
	static WORD freqcounter =0;
	//开始系数校准
//	LEDOnOff(18, TRUE);
	if(m_bIsCoefAdjust == TRUE)
	{
		CoefAdjust();	
		return;
	}
	
	//读取原始采样数据
	ReadSampleData(m_pDataBuf,m_AIDataMask,0);
	//各模拟量通道滤波
	for(LONG i=0;i<15;i++)
	{
		Filter(m_pDataBuf+READ_POINT_NUM*i);
	}
	//合成线电压计算缓冲区
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		m_pComposeBuf[i] = m_pDataBuf[i]-m_pDataBuf[i+READ_POINT_NUM];
		m_pComposeBuf[i+READ_POINT_NUM] = m_pDataBuf[i+READ_POINT_NUM]-m_pDataBuf[i+READ_POINT_NUM*2];
		m_pComposeBuf[i+READ_POINT_NUM*2] = m_pDataBuf[i+READ_POINT_NUM*2]-m_pDataBuf[i];
		
	}	
	CaluMSMod(m_pDataBuf,m_UHA);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,m_UHB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,m_UHC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,m_U0);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,m_ULA);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,m_ULB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,m_ULC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,m_IHA);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,m_IHB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,m_IHC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*10,m_ILA);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*11,m_ILB);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*12,m_ILC);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*13,m_I1);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*14,m_I2);
	CaluMSMod(m_pComposeBuf,m_UHAB);
	CaluMSMod(m_pComposeBuf+READ_POINT_NUM,m_UHBC);
	CaluMSMod(m_pComposeBuf+READ_POINT_NUM*2,m_UHCA);

//计算基波/谐波/以及角度，以UA的角度为基准，故UA要首先计算
	CaluBaseFourier_S15(m_pDataBuf,&m_UHA1);	
	CaluModValue(&m_UHA1);
	CaluAngle(&m_UHA1,AngleTab);
	*RM_pnAUHA = 0;
	DB_LogWriteRM(RM_AUHA,*RM_pnAUHA);	
	CaluSecondFourier(m_pDataBuf,&m_UHA2);
	CaluModValue(&m_UHA2);
	m_UHA2.Mod = m_UHA2.Mod*VolHarmCal_6570_U[0]/1000;	
	CaluThirdFourier(m_pDataBuf,&m_UHA3);
	CaluModValue(&m_UHA3);	
	m_UHA3.Mod = m_UHA3.Mod*VolHarmCal_6570_U[1]/1000;
	CaluFifthFourier(m_pDataBuf,&m_UHA5);
	CaluModValue(&m_UHA5);	
	m_UHA5.Mod = m_UHA5.Mod*VolHarmCal_6570_U[2]/1000;
	CaluSeventhFourier(m_pDataBuf,&m_UHA7);
	CaluModValue(&m_UHA7);	
	m_UHA7.Mod = m_UHA7.Mod*VolHarmCal_6570_U[3]/1000;
	CaluNinthFourier(m_pDataBuf,&m_UHA9);
	CaluModValue(&m_UHA9);
	m_UHA9.Mod = m_UHA9.Mod*VolHarmCal_6570_U[4]/1000;
	CaluEleventhFourier(m_pDataBuf,&m_UHA11);
	CaluModValue(&m_UHA11);
	m_UHA11.Mod = m_UHA11.Mod*VolHarmCal_6570_U[5]/1000;
	
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM,&m_UHB1);	
	CaluModValue(&m_UHB1);
	CaluAngle(&m_UHB1,AngleTab);
	angle = m_UHB1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAUHB = angle;
	else
		*RM_pnAUHB = angle+36000;
	DB_LogWriteRM(RM_AUHB,*RM_pnAUHB);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM,&m_UHB2);
	CaluModValue(&m_UHB2);
	m_UHB2.Mod = m_UHB2.Mod*VolHarmCal_6570_U[0]/1000;	
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM,&m_UHB3);
	CaluModValue(&m_UHB3);
	m_UHB3.Mod = m_UHB3.Mod*VolHarmCal_6570_U[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM,&m_UHB5);
	CaluModValue(&m_UHB5);
	m_UHB5.Mod = m_UHB5.Mod*VolHarmCal_6570_U[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM,&m_UHB7);
	CaluModValue(&m_UHB7);
	m_UHB7.Mod = m_UHB7.Mod*VolHarmCal_6570_U[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM,&m_UHB9);
	CaluModValue(&m_UHB9);
	m_UHB9.Mod = m_UHB9.Mod*VolHarmCal_6570_U[4]/1000;
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM,&m_UHB11);
	CaluModValue(&m_UHB11);
	m_UHB11.Mod = m_UHB11.Mod*VolHarmCal_6570_U[5]/1000;
	
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*2,&m_UHC1);
	CaluModValue(&m_UHC1);	
	CaluAngle(&m_UHC1,AngleTab);
	angle = m_UHC1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAUHC = angle;
	else
		*RM_pnAUHC = angle+36000;
	DB_LogWriteRM(RM_AUHC,*RM_pnAUHC);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*2,&m_UHC2);
	CaluModValue(&m_UHC2);
	m_UHC2.Mod = m_UHC2.Mod*VolHarmCal_6570_U[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*2,&m_UHC3);
	CaluModValue(&m_UHC3);
	m_UHC3.Mod = m_UHC3.Mod*VolHarmCal_6570_U[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*2,&m_UHC5);
	CaluModValue(&m_UHC5);
	m_UHC5.Mod = m_UHC5.Mod*VolHarmCal_6570_U[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*2,&m_UHC7);
	CaluModValue(&m_UHC7);
	m_UHC7.Mod = m_UHC7.Mod*VolHarmCal_6570_U[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*2,&m_UHC9);
	CaluModValue(&m_UHC9);
	m_UHC9.Mod = m_UHC9.Mod*VolHarmCal_6570_U[4]/1000;
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*2,&m_UHC11);
	CaluModValue(&m_UHC11);
	m_UHC11.Mod = m_UHC11.Mod*VolHarmCal_6570_U[5]/1000;
	
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*7,&m_IHA1);
	CaluModValue(&m_IHA1);	
	CaluAngle(&m_IHA1,AngleTab);
	angle = m_IHA1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAIHA = angle;
	else
		*RM_pnAIHA = angle+36000;
	DB_LogWriteRM(RM_AIHA,*RM_pnAIHA);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*7,&m_IHA2);
	CaluModValue(&m_IHA2);
	m_IHA2.Mod = m_IHA2.Mod*CurHarmCal_6570_I[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*7,&m_IHA3);
	CaluModValue(&m_IHA3);
	m_IHA3.Mod = m_IHA3.Mod*CurHarmCal_6570_I[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*7,&m_IHA5);
	CaluModValue(&m_IHA5);
	m_IHA5.Mod = m_IHA5.Mod*CurHarmCal_6570_I[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*7,&m_IHA7);
	CaluModValue(&m_IHA7);
	m_IHA7.Mod = m_IHA7.Mod*CurHarmCal_6570_I[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*7,&m_IHA9);
	CaluModValue(&m_IHA9);
	m_IHA9.Mod = m_IHA9.Mod*CurHarmCal_6570_I[4]/1000;
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*7,&m_IHA11);
	CaluModValue(&m_IHA11);
	m_IHA11.Mod = m_IHA11.Mod*CurHarmCal_6570_I[5]/1000;

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*8,&m_IHB1);
	CaluModValue(&m_IHB1);	
	CaluAngle(&m_IHB1,AngleTab);
	angle = m_IHB1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAIHB = angle;
	else
		*RM_pnAIHB = angle+36000;
	DB_LogWriteRM(RM_AIHB,*RM_pnAIHB);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*8,&m_IHB2);
	CaluModValue(&m_IHB2);
	m_IHB2.Mod = m_IHB2.Mod*CurHarmCal_6570_I[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*8,&m_IHB3);
	CaluModValue(&m_IHB3);
	m_IHB3.Mod = m_IHB3.Mod*CurHarmCal_6570_I[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*8,&m_IHB5);
	CaluModValue(&m_IHB5);
	m_IHB5.Mod = m_IHB5.Mod*CurHarmCal_6570_I[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*8,&m_IHB7);
	CaluModValue(&m_IHB7);
	m_IHB7.Mod = m_IHB7.Mod*CurHarmCal_6570_I[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*8,&m_IHB9);
	CaluModValue(&m_IHB9);
	m_IHB9.Mod = m_IHB9.Mod*CurHarmCal_6570_I[4]/1000;
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*8,&m_IHB11);
	CaluModValue(&m_IHB11);
	m_IHB11.Mod = m_IHB11.Mod*CurHarmCal_6570_I[5]/1000;
	
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*9,&m_IHC1);
	CaluModValue(&m_IHC1);	
	CaluAngle(&m_IHC1,AngleTab);
	angle = m_IHC1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAIHC = angle;
	else
		*RM_pnAIHC = angle+36000;
	DB_LogWriteRM(RM_AIHC,*RM_pnAIHC);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*9,&m_IHC2);
	CaluModValue(&m_IHC2);
	m_IHC2.Mod = m_IHC2.Mod*CurHarmCal_6570_I[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*9,&m_IHC3);
	CaluModValue(&m_IHC3);	
	m_IHC3.Mod = m_IHC3.Mod*CurHarmCal_6570_I[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*9,&m_IHC5);
	CaluModValue(&m_IHC5);
	m_IHC5.Mod = m_IHC5.Mod*CurHarmCal_6570_I[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*9,&m_IHC7);
	CaluModValue(&m_IHC7);
	m_IHC7.Mod = m_IHC7.Mod*CurHarmCal_6570_I[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*9,&m_IHC9);
	CaluModValue(&m_IHC9);
	m_IHC9.Mod = m_IHC9.Mod*CurHarmCal_6570_I[4]/1000;	
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*9,&m_IHC11);
	CaluModValue(&m_IHC11);
	m_IHC11.Mod = m_IHC11.Mod*CurHarmCal_6570_I[5]/1000;

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*10,&m_ILA1);
	CaluModValue(&m_ILA1);	
	CaluAngle(&m_ILA1,AngleTab);
	angle = m_ILA1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAILA = angle;
	else
		*RM_pnAILA = angle+36000;
	DB_LogWriteRM(RM_AILA,*RM_pnAILA);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*10,&m_ILA2);
	CaluModValue(&m_ILA2);
	m_ILA2.Mod = m_ILA2.Mod*CurHarmCal_6570_I[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*10,&m_ILA3);
	CaluModValue(&m_ILA3);
	m_ILA3.Mod = m_ILA3.Mod*CurHarmCal_6570_I[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*10,&m_ILA5);
	CaluModValue(&m_ILA5);
	m_ILA5.Mod = m_ILA5.Mod*CurHarmCal_6570_I[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*10,&m_ILA7);
	CaluModValue(&m_ILA7);
	m_ILA7.Mod = m_ILA7.Mod*CurHarmCal_6570_I[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*10,&m_ILA9);
	CaluModValue(&m_ILA9);
	m_ILA9.Mod = m_ILA9.Mod*CurHarmCal_6570_I[4]/1000;
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*10,&m_ILA11);
	CaluModValue(&m_ILA11);
	m_ILA11.Mod = m_ILA11.Mod*CurHarmCal_6570_I[5]/1000;


	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*11,&m_ILB1);
	CaluModValue(&m_ILB1);	
	CaluAngle(&m_ILB1,AngleTab);
	angle = m_ILB1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAILB = angle;
	else
		*RM_pnAILB = angle+36000;
	DB_LogWriteRM(RM_AILB,*RM_pnAILB);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*11,&m_ILB2);
	CaluModValue(&m_ILB2);
	m_ILB2.Mod = m_ILB2.Mod*CurHarmCal_6570_I[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*11,&m_ILB3);
	CaluModValue(&m_ILB3);
	m_ILB3.Mod = m_ILB3.Mod*CurHarmCal_6570_I[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*11,&m_ILB5);
	CaluModValue(&m_ILB5);
	m_ILB5.Mod = m_ILB5.Mod*CurHarmCal_6570_I[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*11,&m_ILB7);
	CaluModValue(&m_ILB7);
	m_ILB7.Mod = m_ILB7.Mod*CurHarmCal_6570_I[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*11,&m_ILB9);
	CaluModValue(&m_ILB9);
	m_ILB9.Mod = m_ILB9.Mod*CurHarmCal_6570_I[4]/1000;
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*11,&m_ILB11);
	CaluModValue(&m_ILB11);
	m_ILB11.Mod = m_ILB11.Mod*CurHarmCal_6570_I[5]/1000;

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*12,&m_ILC1);
	CaluModValue(&m_ILC1);	
	CaluAngle(&m_ILC1,AngleTab);
	angle = m_ILC1.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAILC = angle;
	else
		*RM_pnAILC = angle+36000;
	DB_LogWriteRM(RM_AILC,*RM_pnAILC);
	CaluSecondFourier(m_pDataBuf+READ_POINT_NUM*12,&m_ILC2);
	CaluModValue(&m_ILC2);
	m_ILC2.Mod = m_ILC2.Mod*CurHarmCal_6570_I[0]/1000;
	CaluThirdFourier(m_pDataBuf+READ_POINT_NUM*12,&m_ILC3);
	CaluModValue(&m_ILC3);	
	m_ILC3.Mod = m_ILC3.Mod*CurHarmCal_6570_I[1]/1000;
	CaluFifthFourier(m_pDataBuf+READ_POINT_NUM*12,&m_ILC5);
	CaluModValue(&m_ILC5);
	m_ILC5.Mod = m_ILC5.Mod*CurHarmCal_6570_I[2]/1000;
	CaluSeventhFourier(m_pDataBuf+READ_POINT_NUM*12,&m_ILC7);
	CaluModValue(&m_ILC7);
	m_ILC7.Mod = m_ILC7.Mod*CurHarmCal_6570_I[3]/1000;
	CaluNinthFourier(m_pDataBuf+READ_POINT_NUM*12,&m_ILC9);
	CaluModValue(&m_ILC9);
	m_ILC9.Mod = m_ILC9.Mod*CurHarmCal_6570_I[4]/1000;	
	CaluEleventhFourier(m_pDataBuf+READ_POINT_NUM*12,&m_ILC11);
	CaluModValue(&m_ILC11);
	m_ILC11.Mod = m_ILC11.Mod*CurHarmCal_6570_I[5]/1000;
//计算低压侧电流电压的角度
	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*4,&m_sULA);
	CaluModValue(&m_sULA);	
	CaluAngle(&m_sULA,AngleTab);
	angle = m_sULA.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAULA = angle;
	else
		*RM_pnAULA = angle+36000;
	DB_LogWriteRM(RM_AULA,*RM_pnAULA);

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*5,&m_sULB);
	CaluModValue(&m_sULB);	
	CaluAngle(&m_sULB,AngleTab);
	angle = m_sULB.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAULB = angle;
	else
		*RM_pnAULB = angle+36000;
	DB_LogWriteRM(RM_AULB,*RM_pnAULB);

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*6,&m_sULC);
	CaluModValue(&m_sULC);	
	CaluAngle(&m_sULC,AngleTab);
	angle = m_sULC.Angle-m_UHA1.Angle;
	if(angle>0)
		*RM_pnAULC = angle;
	else
		*RM_pnAULC = angle+36000;
	DB_LogWriteRM(RM_AULC,*RM_pnAULC);

/*	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*10,&m_sILA);
	CaluModValue(&m_sILA);	
	CaluAngle(&m_sILA,AngleTab);
	*RM_pnAILA = m_sILA.Angle-m_IHA1.Angle;
	DB_LogWriteRM(RM_AILA,*RM_pnAILA);

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*11,&m_sILB);
	CaluModValue(&m_sILB);	
	CaluAngle(&m_sILB,AngleTab);
	*RM_pnAILB = m_sILB.Angle-m_IHA1.Angle;
	DB_LogWriteRM(RM_AILB,*RM_pnAILB);

	CaluBaseFourier_S15(m_pDataBuf+READ_POINT_NUM*12,&m_sILC);
	CaluModValue(&m_sILC);	
	CaluAngle(&m_sILC,AngleTab);
	*RM_pnAILC = m_sILC.Angle-m_IHA1.Angle;
	DB_LogWriteRM(RM_AILC,*RM_pnAILC);
*/
//计算三相功率和总加功率
	CaluMSPQ(m_pDataBuf,m_pDataBuf+READ_POINT_NUM*7,m_PA,m_QA,m_COSA,*CF_pnAA);
	CaluMSPQ(m_pDataBuf+READ_POINT_NUM,m_pDataBuf+READ_POINT_NUM*8,m_PB,m_QB,m_COSB,*CF_pnAB);
	CaluMSPQ(m_pDataBuf+READ_POINT_NUM*2,m_pDataBuf+READ_POINT_NUM*9,m_PC,m_QC,m_COSC,*CF_pnAC);
	//计算三相总加功率
	if(((*CF_pnPA)==0)||((*CF_pnPB)==0)||((*CF_pnPC)==0))
	{
		m_P = 0;
		m_Q = 0;
		m_COS = 0;
	}
	else
	{
		m_P = (LLONG)m_PA*10000/(*CF_pnPA)+(LLONG)m_PB*10000/(*CF_pnPB)+(LLONG)m_PC*10000/(*CF_pnPC);
		m_Q = (LLONG)m_QA*10000/(*CF_pnQA)+(LLONG)m_QB*10000/(*CF_pnQB)+(LLONG)m_QC*10000/(*CF_pnQC);

		QWORD ts;
		LONG ms;
		ts = (LLONG)m_P*m_P+(LLONG)m_Q*m_Q;
		ms = Evolution64(ts);
		if(ms<20)
			m_COS = 0;
		else
		{
			m_COS = (LLONG)m_P*10000/ms;
		}
		if((ToAbs(m_COS))>9990)
			m_Q =0;
		if((ToAbs(m_COS))<10)
			m_P =0;
		m_COS = m_COS/10;
	}
//	DB_WriteAI(M_P,m_P);
//	DB_WriteAI(M_Q,m_Q);
//	DB_WriteAI(M_COS,m_COS);

//写入遥测值,对于物理通道，判断是否大于死区值	
	m_UHAB = AiBubbleSort(m_UHAB,ArrayUHAB);
	if(m_UHAB<=*CF_pnUHAD)		
		m_UHAB = 0;
	DB_WriteAI(M_UHAB,m_UHAB);

	m_UHBC = AiBubbleSort(m_UHBC,ArrayUHBC);
	if(m_UHBC<=*CF_pnUHBD)		
		m_UHBC = 0;
	DB_WriteAI(M_UHBC,m_UHBC);

	m_UHCA = AiBubbleSort(m_UHCA,ArrayUHCA);
	if(m_UHCA<=*CF_pnUHCD)		
		m_UHCA = 0;
	DB_WriteAI(M_UHCA,m_UHCA);
	
	m_UHA = AiBubbleSort(m_UHA,ArrayUHA);
	if(m_UHA<=*CF_pnUHAD)		
		m_UHA = 0;
	DB_WriteAI(M_UHA,m_UHA);

	m_UHB = AiBubbleSort(m_UHB,ArrayUHB);
	if(m_UHB<=*CF_pnUHBD)		
		m_UHB = 0;
	DB_WriteAI(M_UHB,m_UHB);

	m_UHC = AiBubbleSort(m_UHC,ArrayUHC);
	if(m_UHC<=*CF_pnUHCD)		
		m_UHC = 0;
	DB_WriteAI(M_UHC,m_UHC);

	m_U0 = AiBubbleSort(m_U0,ArrayU0);
	if(m_U0<=*CF_pnU0D)		
		m_U0 = 0;
	DB_WriteAI(M_U0,m_U0);

	m_ULA = AiBubbleSort(m_ULA,ArrayULA);
	if(m_ULA<=*CF_pnULAD)		
		m_ULA = 0;
	DB_WriteAI(M_ULA,m_ULA);

	m_ULB = AiBubbleSort(m_ULB,ArrayULB);
	if(m_ULB<=*CF_pnULBD)		
		m_ULB = 0;
	DB_WriteAI(M_ULB,m_ULB);

	m_ULC = AiBubbleSort(m_ULC,ArrayULC);
	if(m_ULC<=*CF_pnULCD)		
		m_ULC = 0;
	DB_WriteAI(M_ULC,m_ULC);

	m_IHA = AiBubbleSort(m_IHA,ArrayIHA);
	if(m_IHA<=*CF_pnIHAD)		
		m_IHA = 0;
	DB_WriteAI(M_IHA,m_IHA);
	
	m_IHB = AiBubbleSort(m_IHB,ArrayIHB);
	if(m_IHB<=*CF_pnIHBD)		
		m_IHB = 0;
	DB_WriteAI(M_IHB,m_IHB);

	m_IHC = AiBubbleSort(m_IHC,ArrayIHC);
	if(m_IHC<=*CF_pnIHCD)		
		m_IHC = 0;
	DB_WriteAI(M_IHC,m_IHC);

	m_ILA = AiBubbleSort(m_ILA,ArrayILA);
	if(m_ILA<=*CF_pnILAD)		
		m_ILA = 0;
	DB_WriteAI(M_ILA,m_ILA);

	m_ILB = AiBubbleSort(m_ILB,ArrayILB);
	if(m_ILB<=*CF_pnILBD)		
		m_ILB = 0;
	DB_WriteAI(M_ILB,m_ILB);

	m_ILC = AiBubbleSort(m_ILC,ArrayILC);
	if(m_ILC<=*CF_pnILCD)		
		m_ILC = 0;
	DB_WriteAI(M_ILC,m_ILC);

	m_I1 = AiBubbleSort(m_I1,ArrayI1);
	if(m_I1<=*CF_pnI1D)		
		m_I1 = 0;
	DB_WriteAI(M_I1,m_I1);

	m_I2 = AiBubbleSort(m_I2,ArrayI2);
	if(m_I2<=*CF_pnI2D)		
		m_I2 = 0;
	DB_WriteAI(M_I2,m_I2);


	WriteAIData(M_PA,m_PA,m_pBubbleData,ArrayPA);
	WriteAIData(M_QA,m_QA,m_pBubbleData,ArrayQA);
	WriteAIData(M_COSA,m_COSA,m_pBubbleData,ArrayCOSA);
	WriteAIData(M_PB,m_PB,m_pBubbleData,ArrayPB);
	WriteAIData(M_QB,m_QB,m_pBubbleData,ArrayQB);
	WriteAIData(M_COSB,m_COSB,m_pBubbleData,ArrayCOSB);
	WriteAIData(M_PC,m_PC,m_pBubbleData,ArrayPC);
	WriteAIData(M_QC,m_QC,m_pBubbleData,ArrayQC);
	WriteAIData(M_COSC,m_COSC,m_pBubbleData,ArrayCOSC);
	WriteAIData(M_P,m_P,m_pBubbleData,ArrayP);
	WriteAIData(M_Q,m_Q,m_pBubbleData,ArrayQ);
	WriteAIData(M_COS,m_COS,m_pBubbleData,ArrayCOS);

	WriteAIData(M_UHA1,m_UHA1.Mod,m_pBubbleData,ArrayUHA1);
	WriteAIData(M_UHA2,m_UHA2.Mod,m_pBubbleData,ArrayUHA2);
	WriteAIData(M_UHA3,m_UHA3.Mod,m_pBubbleData,ArrayUHA3);
	WriteAIData(M_UHA5,m_UHA5.Mod,m_pBubbleData,ArrayUHA5);
	WriteAIData(M_UHA7,m_UHA7.Mod,m_pBubbleData,ArrayUHA7);
	WriteAIData(M_UHA9,m_UHA9.Mod,m_pBubbleData,ArrayUHA9);
	WriteAIData(M_UHA11,m_UHA11.Mod,m_pBubbleData,ArrayUHA11);
	WriteAIData(M_UHB1,m_UHB1.Mod,m_pBubbleData,ArrayUHB1);
	WriteAIData(M_UHB2,m_UHB2.Mod,m_pBubbleData,ArrayUHB2);
	WriteAIData(M_UHB3,m_UHB3.Mod,m_pBubbleData,ArrayUHB3);
	WriteAIData(M_UHB5,m_UHB5.Mod,m_pBubbleData,ArrayUHB5);
	WriteAIData(M_UHB7,m_UHB7.Mod,m_pBubbleData,ArrayUHB7);
	WriteAIData(M_UHB9,m_UHB9.Mod,m_pBubbleData,ArrayUHB9);
	WriteAIData(M_UHB11,m_UHB11.Mod,m_pBubbleData,ArrayUHB11);
	WriteAIData(M_UHC1,m_UHC1.Mod,m_pBubbleData,ArrayUHC1);
	WriteAIData(M_UHC2,m_UHC2.Mod,m_pBubbleData,ArrayUHC2);
	WriteAIData(M_UHC3,m_UHC3.Mod,m_pBubbleData,ArrayUHC3);
	WriteAIData(M_UHC5,m_UHC5.Mod,m_pBubbleData,ArrayUHC5);
	WriteAIData(M_UHC7,m_UHC7.Mod,m_pBubbleData,ArrayUHC7);
	WriteAIData(M_UHC9,m_UHC9.Mod,m_pBubbleData,ArrayUHC9);
	WriteAIData(M_UHC11,m_UHC11.Mod,m_pBubbleData,ArrayUHC11);	
	WriteAIData(M_IHA1,m_IHA1.Mod,m_pBubbleData,ArrayIHA1);
	WriteAIData(M_IHA2,m_IHA2.Mod,m_pBubbleData,ArrayIHA2);
	WriteAIData(M_IHA3,m_IHA3.Mod,m_pBubbleData,ArrayIHA3);
	WriteAIData(M_IHA5,m_IHA5.Mod,m_pBubbleData,ArrayIHA5);
	WriteAIData(M_IHA7,m_IHA7.Mod,m_pBubbleData,ArrayIHA7);
	WriteAIData(M_IHA9,m_IHA9.Mod,m_pBubbleData,ArrayIHA9);
	WriteAIData(M_IHA11,m_IHA11.Mod,m_pBubbleData,ArrayIHA11);
	WriteAIData(M_IHB1,m_IHB1.Mod,m_pBubbleData,ArrayIHB1);
	WriteAIData(M_IHB2,m_IHB2.Mod,m_pBubbleData,ArrayIHB2);
	WriteAIData(M_IHB3,m_IHB3.Mod,m_pBubbleData,ArrayIHB3);
	WriteAIData(M_IHB5,m_IHB5.Mod,m_pBubbleData,ArrayIHB5);
	WriteAIData(M_IHB7,m_IHB7.Mod,m_pBubbleData,ArrayIHB7);
	WriteAIData(M_IHB9,m_IHB9.Mod,m_pBubbleData,ArrayIHB9);
	WriteAIData(M_IHB11,m_IHB11.Mod,m_pBubbleData,ArrayIHB11);
	WriteAIData(M_IHC1,m_IHC1.Mod,m_pBubbleData,ArrayIHC1);
	WriteAIData(M_IHC2,m_IHC2.Mod,m_pBubbleData,ArrayIHC2);
	WriteAIData(M_IHC3,m_IHC3.Mod,m_pBubbleData,ArrayIHC3);
	WriteAIData(M_IHC5,m_IHC5.Mod,m_pBubbleData,ArrayIHC5);
	WriteAIData(M_IHC7,m_IHC7.Mod,m_pBubbleData,ArrayIHC7);
	WriteAIData(M_IHC9,m_IHC9.Mod,m_pBubbleData,ArrayIHC9);
	WriteAIData(M_IHC11,m_IHC11.Mod,m_pBubbleData,ArrayIHC11);

	WriteAIData(M_IA1,m_ILA1.Mod,m_pBubbleData,ArrayILA1);
	WriteAIData(M_IA2,m_ILA2.Mod,m_pBubbleData,ArrayILA2);
	WriteAIData(M_IA3,m_ILA3.Mod,m_pBubbleData,ArrayILA3);
	WriteAIData(M_IA5,m_ILA5.Mod,m_pBubbleData,ArrayILA5);
	WriteAIData(M_IA7,m_ILA7.Mod,m_pBubbleData,ArrayILA7);
	WriteAIData(M_IA9,m_ILA9.Mod,m_pBubbleData,ArrayILA9);
	WriteAIData(M_IA11,m_ILA11.Mod,m_pBubbleData,ArrayILA11);
	WriteAIData(M_IB1,m_ILB1.Mod,m_pBubbleData,ArrayILB1);
	WriteAIData(M_IB2,m_ILB2.Mod,m_pBubbleData,ArrayILB2);
	WriteAIData(M_IB3,m_ILB3.Mod,m_pBubbleData,ArrayILB3);
	WriteAIData(M_IB5,m_ILB5.Mod,m_pBubbleData,ArrayILB5);
	WriteAIData(M_IB7,m_ILB7.Mod,m_pBubbleData,ArrayILB7);
	WriteAIData(M_IB9,m_ILB9.Mod,m_pBubbleData,ArrayILB9);
	WriteAIData(M_IB11,m_ILB11.Mod,m_pBubbleData,ArrayILB11);
	WriteAIData(M_IC1,m_ILC1.Mod,m_pBubbleData,ArrayILC1);
	WriteAIData(M_IC2,m_ILC2.Mod,m_pBubbleData,ArrayILC2);
	WriteAIData(M_IC3,m_ILC3.Mod,m_pBubbleData,ArrayILC3);
	WriteAIData(M_IC5,m_ILC5.Mod,m_pBubbleData,ArrayILC5);
	WriteAIData(M_IC7,m_ILC7.Mod,m_pBubbleData,ArrayILC7);
	WriteAIData(M_IC9,m_ILC9.Mod,m_pBubbleData,ArrayILC9);
	WriteAIData(M_IC11,m_ILC11.Mod,m_pBubbleData,ArrayILC11);
//装置启动后2秒开始测频
	if((freqcounter>10))
	{
		m_FreqFlag = TRUE;
	}
		
	freqcounter++;	
	if(m_FreqFlag)
	{
		BOOL rc;
		rc = TestFrequence(m_dwPreFreq,m_UHA,m_dwFreqMask,m_wFreqBuf,m_FreqCaluBuf,m_dwFreq,true);
		if(rc)
		{
			BubbleSort(m_dwFreq,NUM_FREQ_PER_AITASK);
			m_dwPreFreq = m_dwFreq[NUM_FREQ_PER_AITASK/2]/10;
			
			//频率不在合理频率变化范围之内,取50Hz*/
			if(m_dwPreFreq>MAX_FREQUENCE||m_dwPreFreq<MIN_FREQUENCE) 
			{				
	//			ReportMsg("freq out limit! force");
				m_dwPreFreq = 50000;
			}
			g_dwFreqPWM = m_dwPreFreq*POINT_PER_PERIOD; 
	//		ReportMsg("freq = %d",m_dwPreFreq);
		//	ADStartFreqUpdate(g_dwFreqPWM);
			AD1StartFreqUpdate(g_dwFreqPWM);
		}
		else
		{
			m_dwPreFreq = 0;
			AD1StartFreqUpdate(50000*POINT_PER_PERIOD);
		}		
		DB_WriteAI(M_F,m_dwPreFreq);
	}
//	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
	//LEDOnOff(18, FALSE);
}
/*********************************************************************************
*功能: 功率计算
*参数: UBuf,电压采样值；IBuf,电流采样值；P/Q/Cos/dA,有功/无功/功率因数/角差
*输出: 无
*返回: 无
**************************************************************************************/

void CAIProc::CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG &P,LONG &Q,LONG &Cos,LONG dA,BOOL coef)
{
	LLONG	tp=0;
	LLONG	tq=0;
	QWORD	ts=0;
	DWORD	S =0;

	for(LONG i=0;i<32;i++)
	{
		tp += UBuf[i+1]*IBuf[i+1]; 
		tq += (UBuf[i]-UBuf[2+i])*IBuf[i+1];
	}
	tp = tp/3200;          // 1W的数字量太大，除32后再除100
	tq = tq/SINWTS;   // SINWTS = 32*100*1000*2*sin(wTs)

	P = tp + tq*dA/10000;    //dA角差系数，拟放大10000倍
	Q = tq - tp*dA/10000;	//dA角差系数，拟放大10000倍

	ts = (LLONG)P*P+(LLONG)Q*Q;
	S  = Evolution64(ts);
	if(S<=20)
		Cos =0;
	else
	{
		Cos = (LLONG)P*10000/(LONG)S;
	}
	if(coef)
	{
		if(ToAbs(Cos)>9990)
			Q=0;
		if(ToAbs(Cos)<10)
			P=0;
	}
	Cos = Cos/10;
}




void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//设置死区校正标志
	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDriftAdjust = TRUE;			
		m_DriftUHA = 0;
		m_DriftUHB = 0;
		m_DriftUHC = 0;
		m_DriftU0  = 0;
		m_DriftULA = 0;
		m_DriftULB = 0;
		m_DriftULC = 0;
		m_DriftIHA = 0;
		m_DriftIHB = 0;
		m_DriftIHC = 0;
		m_DriftILA = 0;
		m_DriftILB = 0;
		m_DriftILC = 0;
		m_DriftI1  = 0;
		m_DriftI2  = 0; 
	}
	else
		m_bIsDriftAdjust = FALSE;

	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	dwMsgID = arg;
	//设置自动校准标志
	m_bIsCoefAdjust = TRUE;	
	//初始化源校正标志
	m_bIsSourceOK = FALSE;
	//校正等待计数器清0
	m_dwWaitCnt = 0;

	m_tDA = 0;
	m_tDB = 0;
	m_tDC = 0;
	
	//各通道死区值清0
	m_MidUHA = 0;
	m_MidUHB = 0;
	m_MidUHC = 0;
	m_MidU0  = 0;
	m_MidULA = 0;
	m_MidULB = 0;
	m_MidULC = 0;
	m_MidIHA = 0;	
	m_MidIHB = 0;
	m_MidIHC = 0;
	m_MidILA = 0;
	m_MidILB = 0;
	m_MidILC = 0;
	m_MidI1  = 0;
	m_MidI2  = 0;
	m_MidUHA1 = 0;
	m_MidUHB1 = 0;
	m_MidUHC1 = 0;
	m_MidIHA1 = 0;
	m_MidIHB1 = 0;
	m_MidIHC1 = 0;
	//待排序数组清零
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		ArrayAdjUHA[i] = 0;
		ArrayAdjUHB[i] = 0;
		ArrayAdjUHC[i] = 0;
		ArrayAdjU0[i]  = 0;
		ArrayAdjULA[i] = 0;
		ArrayAdjULB[i] = 0;
		ArrayAdjULC[i] = 0;
		ArrayAdjIHA[i] = 0;
		ArrayAdjIHB[i] = 0;
		ArrayAdjIHC[i] = 0;
		ArrayAdjILA[i] = 0;
		ArrayAdjILB[i] = 0;
		ArrayAdjILC[i] = 0;
		ArrayAdjI1[i]  = 0;
		ArrayAdjI2[i]  = 0;
		ArrayAdjPA[i]  = 0;
		ArrayAdjQA[i]  = 0;
		ArrayAdjPB[i]  = 0;
		ArrayAdjQB[i]  = 0;
		ArrayAdjPC[i]  = 0;
		ArrayAdjQC[i]  = 0;		
	}
}
/*
void CAIProc::OnAutoAdjustStep2(DWORD arg,DWORD Uval,DWORD Ival)
{	
	//开始系数校正标志置位
	m_bIsCoefAdjust = TRUE;
	//清死区值校正标志
	m_bIsDriftAdjust = FALSE;
	//清源校正标志
	m_bIsSourceOK = FALSE;
	//校正等待计数器清零
	m_dwWaitCnt = 0;
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	
	m_MidUHA = 0;
	m_MidUHB = 0;
	m_MidUHC = 0;
	m_MidU0  = 0;
	m_MidULA = 0;
	m_MidULB = 0;
	m_MidULC = 0;
	m_MidIHA = 0;	
	m_MidIHB = 0;
	m_MidIHC = 0;
	m_MidILA = 0;
	m_MidILB = 0;
	m_MidILC = 0;
	m_MidI1  = 0;
	m_MidI2  = 0;
	m_MidUHA1 = 0;
	m_MidUHB1 = 0;
	m_MidUHC1 = 0;
	m_MidIHA1 = 0;
	m_MidIHB1 = 0;
	m_MidIHC1 = 0;
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		ArrayAdjUHA[i] = 0;
		ArrayAdjUHB[i] = 0;
		ArrayAdjUHC[i] = 0;
		ArrayAdjU0[i]  = 0;
		ArrayAdjULA[i] = 0;
		ArrayAdjULB[i] = 0;
		ArrayAdjULC[i] = 0;
		ArrayAdjIHA[i] = 0;
		ArrayAdjIHB[i] = 0;
		ArrayAdjIHC[i] = 0;
		ArrayAdjILA[i] = 0;
		ArrayAdjILB[i] = 0;
		ArrayAdjILC[i] = 0;
		ArrayAdjI1[i]  = 0;
		ArrayAdjI2[i]  = 0;
		ArrayAdjPA[i]  = 0;
		ArrayAdjQA[i]  = 0;
		ArrayAdjPB[i]  = 0;
		ArrayAdjQB[i]  = 0;
		ArrayAdjPC[i]  = 0;
		ArrayAdjQC[i]  = 0;		
	}
}
*/
void CAIProc::CoefAdjustCalu(void)//处理系数校正中的计算
{	
	ReadSampleData(m_pCoefAdjustBuf,m_CoefAdjustDataMask,0);		//读取原始采样数据	
	for(LONG i=0;i<16;i++)
	{
		Filter(m_pCoefAdjustBuf+READ_POINT_NUM*i);
	}
	
	CaluMSMod(m_pCoefAdjustBuf,m_AdjUHA);						//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,m_AdjUHB);			//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_AdjUHC);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,m_AdjU0);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,m_AdjULA);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,m_AdjULB);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,m_AdjULC);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,m_AdjIHA);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,m_AdjIHB);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,m_AdjIHC);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*10,m_AdjILA);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*11,m_AdjILB);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*12,m_AdjILC);		//测量电压U1	
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*13,m_AdjI1);		//测量电压U1
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*14,m_AdjI2); 		//测量电压U1

	//非死区值校正时，计算各通道的谐波基波
	if(!m_bIsDriftAdjust)
	{
		CaluBaseFourier_S15(m_pCoefAdjustBuf,&m_AdjUHA1);	
		CaluModValue(&m_AdjUHA1);
		CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&m_AdjUHB1);
		CaluModValue(&m_AdjUHB1);
		CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&m_AdjUHC1);
		CaluModValue(&m_AdjUHC1);
		CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&m_AdjIHA1);	
		CaluModValue(&m_AdjIHA1);
		CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&m_AdjIHB1);	
		CaluModValue(&m_AdjIHB1);
		CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&m_AdjIHC1);	
		CaluModValue(&m_AdjIHC1);
		
	}
	
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
		if(m_bIsDriftAdjust)
		{
			
			m_DriftUHA = m_pAdjustData[0];
			m_DriftUHB = m_pAdjustData[1];
			m_DriftUHC = m_pAdjustData[2];
			m_DriftU0  = m_pAdjustData[3];
			m_DriftULA = m_pAdjustData[4];
			m_DriftULB = m_pAdjustData[5];
			m_DriftULC = m_pAdjustData[6];
			m_DriftIHA = m_pAdjustData[7];
			m_DriftIHB = m_pAdjustData[8];
			m_DriftIHC = m_pAdjustData[9];
			m_DriftILA = m_pAdjustData[10];
			m_DriftILB = m_pAdjustData[11];
			m_DriftILC = m_pAdjustData[12];	
			m_DriftI1  = m_pAdjustData[13];	
			m_DriftI2  = m_pAdjustData[14];	
			
			CoefAdjustEnd(SUCCESS);
			return;
		}
		else
		{			
			LONG Value[52];
			
			Value[0]	= m_pAdjustData[0]/m_dwSourceU;     			//UHA
			Value[1]	= m_pAdjustData[1]/m_dwSourceU;					//UHB
			Value[2]	= m_pAdjustData[2]/m_dwSourceU;					//UHC
			Value[3]	= m_pAdjustData[3]/m_dwSourceU;					//U0
			Value[4]	= m_pAdjustData[4]/m_dwSourceU;					//ULA
			Value[5]	= m_pAdjustData[5]/m_dwSourceU;					//ULB
			Value[6]	= m_pAdjustData[6]/m_dwSourceU;					//ULC
			Value[7]	= m_pAdjustData[7]/(10*m_dwSourceI);					//IHA
			Value[8]	= m_pAdjustData[8]/(10*m_dwSourceI);				//IHB
			Value[9]	= m_pAdjustData[9]/(10*m_dwSourceI);					//IHC
			Value[10]	= m_pAdjustData[10]/(10*m_dwSourceI);				//ILA
			Value[11]	= m_pAdjustData[11]/(10*m_dwSourceI);				//ILB
			Value[12]	= m_pAdjustData[12]/(10*m_dwSourceI);			    //ILC
			Value[13]	= m_pAdjustData[13]/(10*m_dwSourceI);				//I1
			Value[14]	= m_pAdjustData[14]/(10*m_dwSourceI);				//I2
			Value[15]	= m_pAdjustData[15]/(m_dwSourceU*m_dwSourceI);				//PA
			Value[16]	= m_pAdjustData[16]/(m_dwSourceU*m_dwSourceI);						//PB
			Value[17]	= m_pAdjustData[17]/(m_dwSourceU*m_dwSourceI);						//PC
			Value[18]	= Value[15];				//U3m
			Value[19]	= Value[16];				//U4m
			Value[20]	= Value[17];				//U5m
			Value[21]	= m_tDA;				//U7m
			Value[22]	= m_tDB;				//U8m
			Value[23]	= m_tDC;				//I1m
			Value[24]	= m_DriftUHA;				//I2m
			Value[25]	= m_DriftUHB;				//I3m
			Value[26]	= m_DriftUHC;				//I4m
			Value[27]	= m_DriftU0;				//I5m
			Value[28]	= m_DriftULA;				//I6m
			Value[29]	= m_DriftULB;
			Value[30]	= m_DriftULC;
			Value[31]	= m_DriftIHA;
			Value[32]	= m_DriftIHB;	
			Value[33]   = m_DriftIHC;
			Value[34]	= m_DriftILA;
			Value[35]	= m_DriftILB;
			Value[36]	= m_DriftILC;
			Value[37]	= m_DriftI1;
			Value[38]	= m_DriftI2;
			Value[39]	= 1000;
			Value[40]	= 100;
			Value[41]	= 10;
			Value[42]	= 1;
			Value[43]	= m_pAdjustData[21]/m_dwSourceU;
			Value[44]	= m_pAdjustData[22]/m_dwSourceU;
			Value[45]	= m_pAdjustData[23]/m_dwSourceU;			
			Value[46]	= m_pAdjustData[24]/(10*m_dwSourceI);
			Value[47]	= m_pAdjustData[25]/(10*m_dwSourceI);
			Value[48]	= m_pAdjustData[26]/(10*m_dwSourceI);
			Value[49]   = (Value[0]+Value[1])/2;
			Value[50]   = (Value[1]+Value[2])/2;
			Value[51]   = (Value[0]+Value[2])/2;
			
			DB_SetCFValue(CF_VALUE_ALL, Value);				//写入系数值
			DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);	//写入系数确认
			CoefAdjustEnd(SUCCESS);
			return;
		}
	
	}
// 系数校正中的电量计算
	CoefAdjustCalu();
	
// 首先开始源判断,并校正角差
	if(m_bIsSourceOK == FALSE)
	{
		LONG ElecVal[]={m_AdjUHA,m_AdjUHB,m_AdjUHC,m_AdjU0,m_AdjULA,m_AdjULB,m_AdjULC,m_AdjIHA,m_AdjIHB,m_AdjIHC,
						m_AdjILA,m_AdjILB,m_AdjILC,m_AdjI1,m_AdjI2};
		
		LONG ElecCoef[]={*CF_pnUHA,*CF_pnUHB,*CF_pnUHC,*CF_pnU0,*CF_pnULA,*CF_pnULB
			,*CF_pnULC,*CF_pnIHA,*CF_pnIHB,*CF_pnIHC,*CF_pnILA,*CF_pnILB
			,*CF_pnILC,*CF_pnI1,*CF_pnI2};
		
		if(m_bIsDriftAdjust == TRUE)
		{	
			for(LONG i=0;i<7;i++)
			{
				if(ElecVal[i] > ElecCoef[i]/200) // 与小值0.5V作比较
				{
					CoefAdjustEnd(FAILURE);
					return;
				}	
			}			
			for(LONG i=0;i<8;i++)
			{
				if(ElecVal[7+i] > ElecCoef[7+i]/5) // 与小值0.2A作比较
				{
					CoefAdjustEnd(FAILURE);
					return;
				}	
			}			
		}
		else
		{
			//首次源校正
			for(LONG i=0;i<7;i++)
			{				
				if((ElecVal[i]<(ElecCoef[i]*m_dwSourceU*9/1000))||(ElecVal[i]>(ElecCoef[i]*m_dwSourceU*11/1000)))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			for(LONG i=0;i<8;i++)
			{				
				if((ElecVal[7+i]<(ElecCoef[7+i]*m_dwSourceI*9/100))||(ElecVal[7+i]>(ElecCoef[7+i]*m_dwSourceI*11/100)))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}	
			for(int i=1;i<15;i++)
			{
				if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			//源校正通过，计算角差
			CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*7,m_AdjPA,m_AdjQA,m_COSA,0,FALSE);
			CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM,m_pCoefAdjustBuf+READ_POINT_NUM*8,m_AdjPB,m_AdjQB,m_COSB,0,FALSE);
			CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_pCoefAdjustBuf+READ_POINT_NUM*9,m_AdjPC,m_AdjQC,m_COSC,0,FALSE);
			
			if((m_AdjQA>*CF_pnQA/3)||(m_AdjQB>*CF_pnQB/3)||(m_AdjQC>*CF_pnQC/3))  //无功大于2W，电流源或者电压源的角度有问题
			{
				CoefAdjustEnd(FAILURE);
				return;
			}
			if(m_AdjQA == 0)
				m_tDA = 0;
			else
				m_tDA = m_AdjQA*10000/((LONG)Evolution64((LLONG)m_AdjQA*m_AdjQA+(LLONG)m_AdjPA*m_AdjPA));

			if(m_AdjQB == 0)
				m_tDB = 0;
			else
				m_tDB = m_AdjQB*10000/((LONG)Evolution64((LLONG)m_AdjQB*m_AdjQB+(LLONG)m_AdjPB*m_AdjPB));
			
			if(m_AdjQC == 0)
				m_tDC = 0;
			else
				m_tDC = m_AdjQC*10000/((LONG)Evolution64((LLONG)m_AdjQC*m_AdjQC+(LLONG)m_AdjPC*m_AdjPC));

		}
		m_bIsSourceOK = TRUE;		
		return;
	}
	CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*7,m_AdjPA,m_AdjQA,m_COSA,m_tDA);
	CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM,m_pCoefAdjustBuf+READ_POINT_NUM*8,m_AdjPB,m_AdjQB,m_COSB,m_tDB);
	CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_pCoefAdjustBuf+READ_POINT_NUM*9,m_AdjPC,m_AdjQC,m_COSC,m_tDC);

//死区值校正，取得每个通道的最大零漂值
	if(m_bIsDriftAdjust)
	{
		if(m_AdjUHA>m_MidUHA)
			m_MidUHA = m_AdjUHA;
		if(m_AdjUHB>m_MidUHB)
			m_MidUHB = m_AdjUHB;
		if(m_AdjUHC>m_MidUHC)
			m_MidUHC = m_AdjUHC;
		if(m_AdjU0>m_MidU0)
			m_MidU0  = m_AdjU0;
		if(m_AdjULA>m_MidULA)
			m_MidULA = m_AdjULA;
		if(m_AdjULB>m_MidULB)
			m_MidULB = m_AdjULB;
		if(m_AdjULC>m_MidULC)
			m_MidULC = m_AdjULC;
		if(m_AdjIHA>m_MidIHA)
			m_MidIHA = m_AdjIHA;
		if(m_AdjIHB>m_MidIHB)
			m_MidIHB = m_AdjIHB;
		if(m_AdjIHC>m_MidIHC)
			m_MidIHC = m_AdjIHC;
		if(m_AdjILA>m_MidILA)
			m_MidILA = m_AdjILA;
		if(m_AdjILB>m_MidILB)
			m_MidILB = m_AdjILB;
		if(m_AdjILC>m_MidILC)
			m_MidILC = m_AdjILC;
		if(m_AdjI1>m_MidI1)
			m_MidI1  = m_AdjI1;
		if(m_AdjI2>m_MidI2)
			m_MidI2  = m_AdjI2;
	}
//系数排序计算，取得中间值
	else
	{
		m_MidUHA = AiBubbleSort(m_AdjUHA,ArrayAdjUHA);
		m_MidUHB = AiBubbleSort(m_AdjUHB,ArrayAdjUHB);
		m_MidUHC = AiBubbleSort(m_AdjUHC,ArrayAdjUHC);
		m_MidU0  = AiBubbleSort(m_AdjU0,ArrayAdjU0);
		m_MidULA = AiBubbleSort(m_AdjULA,ArrayAdjULA);
		m_MidULB = AiBubbleSort(m_AdjULB,ArrayAdjULB);
		m_MidULC = AiBubbleSort(m_AdjULC,ArrayAdjULC);
		m_MidIHA = AiBubbleSort(m_AdjIHA,ArrayAdjIHA);	
		m_MidIHB = AiBubbleSort(m_AdjIHB,ArrayAdjIHB);
		m_MidIHC = AiBubbleSort(m_AdjIHC,ArrayAdjIHC);
		m_MidILA = AiBubbleSort(m_AdjILA,ArrayAdjILA);
		m_MidILB = AiBubbleSort(m_AdjILB,ArrayAdjILB);
		m_MidILC = AiBubbleSort(m_AdjILC,ArrayAdjILC);
		m_MidI1  = AiBubbleSort(m_AdjI1,ArrayAdjI1);
		m_MidI2  = AiBubbleSort(m_AdjI2,ArrayAdjI2);
		m_MidUHA1 = AiBubbleSort(m_AdjUHA1.Mod,ArrayAdjUHA1);
		m_MidUHB1 = AiBubbleSort(m_AdjUHB1.Mod,ArrayAdjUHB1);
		m_MidUHC1 = AiBubbleSort(m_AdjUHC1.Mod,ArrayAdjUHC1);
		m_MidIHA1 = AiBubbleSort(m_AdjIHA1.Mod,ArrayAdjIHA1);
		m_MidIHB1 = AiBubbleSort(m_AdjIHB1.Mod,ArrayAdjIHB1);
		m_MidIHC1 = AiBubbleSort(m_AdjIHC1.Mod,ArrayAdjIHC1);
	}

	
	// 系数校正之初，等待两秒钟
	if(m_dwWaitCnt <2000/AI_TIMER_PERIOD)		
	{
		m_dwWaitCnt++;
		return;
	}
	if(m_bIsDriftAdjust)
	{
		m_pAdjustData[0]  = m_MidUHA;
		m_pAdjustData[1]  = m_MidUHB;
		m_pAdjustData[2]  = m_MidUHC;
		m_pAdjustData[3]  = m_MidU0;
		m_pAdjustData[4]  = m_MidULA;
		m_pAdjustData[5]  = m_MidULB;
		m_pAdjustData[6]  = m_MidULC;
		m_pAdjustData[7]  = m_MidIHA;
		m_pAdjustData[8]  = m_MidIHB;
		m_pAdjustData[9]  = m_MidIHC;
		m_pAdjustData[10] = m_MidILA;
		m_pAdjustData[11] = m_MidILB;
		m_pAdjustData[12] = m_MidILC;
		m_pAdjustData[13] = m_MidI1;
		m_pAdjustData[14] = m_MidI2;
	}
	else
	{
		m_pAdjustData[0]  += m_MidUHA;
		m_pAdjustData[1]  += m_MidUHB;
		m_pAdjustData[2]  += m_MidUHC;
		m_pAdjustData[3]  += m_MidU0;
		m_pAdjustData[4]  += m_MidULA;
		m_pAdjustData[5]  += m_MidULB;
		m_pAdjustData[6]  += m_MidULC;
		m_pAdjustData[7]  += m_MidIHA;
		m_pAdjustData[8]  += m_MidIHB;
		m_pAdjustData[9]  += m_MidIHC;
		m_pAdjustData[10] += m_MidILA;
		m_pAdjustData[11] += m_MidILB;
		m_pAdjustData[12] += m_MidILC;
		m_pAdjustData[13] += m_MidI1;
		m_pAdjustData[14] += m_MidI2;
		m_pAdjustData[15] += m_AdjPA;
		m_pAdjustData[16] += m_AdjPB;
		m_pAdjustData[17] += m_AdjPC;
		m_pAdjustData[18] += m_AdjQA;
		m_pAdjustData[19] += m_AdjQB;
		m_pAdjustData[20] += m_AdjQC;	
		m_pAdjustData[21] += m_MidUHA1;
		m_pAdjustData[22] += m_MidUHB1;
		m_pAdjustData[23] += m_MidUHC1;
		m_pAdjustData[24] += m_MidIHA1;
		m_pAdjustData[25] += m_MidIHB1;
		m_pAdjustData[26] += m_MidIHC1;
	}
//	ReportMsg("%d",m_MidUHA);
	m_dwAdjustCnt++;

}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<28;i++)     
		m_pAdjustData[i]=0;

	//系数校正计算次数计数器清零
	m_dwAdjustCnt =0;
	//系数校正开始后等待次数计数器清零
	m_dwWaitCnt   =0;
	//清系数校正任务标志
	m_bIsCoefAdjust = FALSE;
	//清源校正标志结束标志
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
/*void CAIProc::RefreshCommRW()
{	
	WORD sernum,cannum,ethnum;	
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();
	TSerialPara tSerial;
	TCanBusPara tCan;
	TEthPortPara tEth;
	BYTE commuStatus;
	for(WORD i=0;i<ethnum;i++)
	{			
		DB_GetETHPara(i,&tEth);
		WriteCommRW(i,tEth.wPortID,RW_ET11ERR,SS_ETHA_COMM_ERR,commuStatus);
	}
	for(WORD i=0;i<sernum;i++)
	{
		DB_GetSerialPara(i, &tSerial);		
		WriteCommRW(i,tSerial.wPortID,RW_COM1ERR,SS_SER_COMM_ERR,commuStatus);
	}
	for(WORD i=0;i<cannum;i++)
	{
		DB_GetCANPara(i, &tCan);
		WriteCommRW(i,tCan.wPortID,RW_CAN1ERR,SS_CAN_COMM_ERR,commuStatus);
	}
	
}

void CAIProc::WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus)
{	
	if(DAE_GetCommStatus(wPortID,commuStatus)!=DB_ERR_OK)	
		WriteRelayWord(RelayWord+i,RELAY_ON);
	else
	{
		if((DAE_GetRunningFlags(RunningFlag+i))||(commuStatus == COMM_STATUS_BREAK))
			WriteRelayWord(RelayWord+i,RELAY_ON);
		else
			WriteRelayWord(RelayWord+i,RELAY_OFF);
	}
}*/






































