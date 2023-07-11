/*******************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
*******************************************************************************************/ 
                          
/*******************************************************************************************
*                                                                                  
* 文件名称          
*			relay.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           保护算法代码                                                                       
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
*      yanzh             08/12/19    初始创建                         
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
#include "scdefs.h"
#include "daefunc.h"
extern BYTE   byRelayWordValue[];
extern HANDLE g_hDbaseTaskID;
extern DWORD  dwRelayDataPtr;	//AD当前写指针
extern DWORD  dwRelayMSCnt;		//保护用毫秒计数器
extern BYTE   g_RptSerialNum;
extern BOOL   RelayCheckErr;			//保护自检出错
extern BOOL   RelayCheckInitOK;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD dwADResumeCnt;
extern BYTE *g_MaintSW;


DWORD dwRelayTaskID;

BOOL  RelayTaskInitOK = FALSE;
DWORD dwCaluInterval;	//热过负荷计算累加间隔
BOOL  g_bRel49Flag;		//热过负荷进行累加计算消息标志
BOOL  g_bRel49TimeOut;  //热过负荷累加延时到标志

DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
TStructCheck StructCheck[CHECK_NUM];			//保护结构体校验数组

TRunTimeRM	*pSysRM;		//保护测量值数据指针
TRunTimeBI  *pSysBI;		//遥信实时数据指针

BEGIN_MESSAGE_MAP(CRelayTask,CApp)
	ON_SM_SAMPLEEND(CRelayTask)
	ON_SM_RELAYDATA(CRelayTask)
	ON_SM_TIMEOUT(CRelayTask)
END_MESSAGE_MAP()

void RelayTask(DWORD *pdwApp)
{
	TMessage msg;
	DWORD status;
	OS_TaskDesp *pTask;

	OS_Sleep(1500);
	CRelayTask *pRelayTask = new CRelayTask(pdwApp);
	pRelayTask->Init();

	pTask = OS_GetTaskTab(dwRelayTaskID);

    while(status != ERR_MESSAGE_QUIT)
    {
		status = OS_GetMessage(dwRelayTaskID, &msg);
		if(status == ERR_MESSAGE_OK)
		{
			//清除任务运行计数
			if(pTask != NULL)
				pTask->Counter = 0;

			switch(LOWORD(msg.lParam))
			{
				case SM_TIMEOUT:
					pRelayTask->OnTimeOut(0);
					break;
				case SM_RELAYDATA:	//保护数据修改
					pRelayTask->OnRelayChange(0, 0);
					break;
				case SM_RELAY_INT:
					pRelayTask->OnSampleEnd(0);
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
	
	ADCInit();			//初始化ADC，加载采样中断
	RelayTimeInit();	//初始化保护用毫秒定时器
	#if(HARD_WARE_VER<=HW_VERSION_11)
		ADTimeInit();		//初始化采样
	#endif
/*	
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	//获取数据库保护测量值数据指针，以加快操作速度
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;

	InitAllRelayMem();			//初始化保护内部变量
	InitRealy();				//初始化保护元件
	InitSet();
	InitDCRecInfo();
	InitStructCheck();
	

	RelCFGCLR = CFG_STATUS_CLR;			//保护退出
	RelCFGSET = CFG_STATUS_SET;			//保护投入
	
	g_RptSerialNum = 0;					//系统的报告序列号
	m_wRptSerialNumOfAct =0;			//系统的保护动作报告序列号
	m_wRptSerialNumOfStart=0;			//系统的带故障电量的启动报告序列号	
	m_dwFaultSINum = 0;					//采样间隔计数器	
	
	m_bTripFailFlag = FALSE;			//跳闸失败标志
	m_bFaultOnFlag = FALSE;				//进入故障处理标志
	m_bHaveMakeStartRptFlag = FALSE;	//已作带故障电量启动报告标志
	m_bFaultLKFlag = FALSE;				//故障起始时刻锁定标志
	m_bFaultCheckOutFlag = FALSE;		//检出故障标志
	m_bLastRecStatusFlag = FALSE;		//故障录波启动上升沿判断标志

	m_b52BFLAFlag = FALSE;
	m_b52BFLBFlag = FALSE;
	m_b52BFHFlag = FALSE;
	m_bHaveMake52BF_H_RptFlag = FALSE;	//已作高压侧断路器跳闸失败报告		
	m_bHaveMake52BF_LA_RptFlag = FALSE; 	
	m_bHaveMake52BF_LB_RptFlag = FALSE; 	
	m_b52BFRecover_H_FLAG = FALSE;		//高压侧断路器跳闸失败恢复标志
	m_b52BFRecover_LA_FLAG = FALSE;
	m_b52BFRecover_LB_FLAG = FALSE;

	
	m_wPickCheck = 0;					//突变量启动判断连续点计数器
	for(LONG i=0;i<7;i++)
	{		
		m_wPickCheckNext[i] = 0;				//突变量启动判断两次任务间连续点计数器
		m_lPickUpValueArray[i] = 0;			//突变量定值浮动调整数组
	}
	for(LONG i=0;i<6;i++)
	{
		DCCaluResult[i]=0;
	}
	//保护数据读取通道掩码
	dwChanDataMask = ( (0x01<<CHANNEL_6531_UHA)|(0x01<<CHANNEL_6531_UHB)|(0x01<<CHANNEL_6531_UHC)| \
						(0x01<<CHANNEL_6531_U0)|(0x01<<CHANNEL_6531_ULA)|(0x01<<CHANNEL_6531_ULB)| \
						(0x01<<CHANNEL_6531_IHA)|(0x01<<CHANNEL_6531_IHB)|(0x01<<CHANNEL_6531_IHC)| \
						(0x01<<CHANNEL_6531_I0)|(0x01<<CHANNEL_6531_I1)|(0x01<<CHANNEL_6531_I2)|\
						(0x01<<CHANNEL_6531_I3)|(0x01<<CHANNEL_6531_I4));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	dwComposeDataMask = ((0x01<<CHANNEL_6531_ILA)|(0x01<<CHANNEL_6531_ILB)|(0x01<<CHANNEL_6531_Ijx));

	
	//初始化读取保护数据临时缓冲区
	//保护数据缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*14);	
	//合成数据回写缓冲区
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	//突变量启动历史数据缓冲区
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*7);
	//线电压计算用数据缓冲区
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*7);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(DWORD)*READ_POINT_NUM*14);

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);


	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*7);
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*7);
	//下面是初始化逻辑方程部分
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	//初始化开出自检为正常状态
	for(int i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	//首先进行一遍开出失效自检检查
	for(int j=0; j<OUTPUT_RELAY_WORD_NUM; j++)
		BoInvalidCheck();
	
	RelayTaskInitOK = TRUE;
	m_bLogicChanged = FALSE;
	m_dwWaitLogicCounter = 0;
	g_MaintSW = SW_pbyMAINT;
	SetTimer(1,5);

}
/************************************************************************************************
*功能: 初始化与保护直接相关的定值
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitSet(void)
{
	dwCaluInterval = Rel49I_IA.l49OT*2;
	if((*CFG_pby49 == CFG_STATUS_SET)&&(*SW_pby49 == SW_STATUS_SET))
		g_bRel49Flag = TRUE;
	else
		g_bRel49Flag = FALSE;

}
/************************************************************************************************
*功能: 初始化保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	 InitRel50I_IA();
	 InitRel50I_IB();
	 InitRel50I_IC();
	 InitRel50I_Ia1();
	 InitRel50I_Ia2();	 
	 InitRel50I_Ib1();
	 InitRel50I_Ib2();
	 InitRel50I_JX();
	 InitRel49IHA();
	 InitRel49IHB();
   	 InitRel49IHC();
	 InitRel50IA1();
	 InitRel50IA2();
	 InitRel50IB1();
	 InitRel50IB2();
	 InitRel50IC1();
	 InitRel50IC2();
	 InitRel50IN();
	 InitRel59UN();
	 InitRel27U();
	 InitRel74PT();
	
}
void CRelayTask::OnTimeOut(DWORD id)
{
	
	if(dwADResumeCnt ==0)
		return;
	else
	{
		LogicProcess();
	}
}
void CRelayTask::ThermalOverLoad()
{		
	if((*CFG_pby49 != CFG_STATUS_SET)||(*SW_pby49 != SW_STATUS_SET))
		return;		
	RelThermalOverLoad(&Rel49I_IA);	//A相热累加计算
	RelThermalOverLoad(&Rel49I_IB);	//B相热累加计算
	RelThermalOverLoad(&Rel49I_IC);	//C相热累加计算
	//热过负荷动作不需要分相，故在作报告的时候只采用A相的报告INF
	if(ReadRelayWord(Rel49I_IA.R49WarnRW)||ReadRelayWord(Rel49I_IB.R49WarnRW)||ReadRelayWord(Rel49I_IC.R49WarnRW))	//告警继电器字置位
	{
		if(m_bHaveMakeWarnRpt == FALSE)	//尚未做告警报告
		{
			MakeAlarmRpt(Rel49I_IA.R49WarnRptNum);	//作告警报告,热过负荷告警
			m_bHaveMakeWarnRpt = TRUE;		//置已作告警报告标志
			return;
		}
	}
	if(ReadRelayWord(Rel49I_IA.R49ActRW)||ReadRelayWord(Rel49I_IB.R49ActRW)||ReadRelayWord(Rel49I_IC.R49ActRW))
	{
		if(m_bHaveMakeActRpt == FALSE)
		{			
			TAbsTime RptTime;
			WORD	 RptNo;
			ReadAbsTime(&RptTime);
			DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&RptTime,FALSE); //向数据库中写入启动报告
			RptNo = g_RptSerialNum;
			MakeActRpt(Rel49I_IA.R49ActRptNum,RptNo);	
			DB_RelayEnd(RptNo); 			//向数据库中写入结束报
			m_bHaveMakeActRpt = TRUE;			
			m_bHaveMakeRSTRpt = FALSE;			
		}
		return;  //动作继电器字置位，直接退出
	}
	if((!ReadRelayWord(Rel49I_IA.R49WarnRW))&&(!ReadRelayWord(Rel49I_IB.R49WarnRW))&&(!ReadRelayWord(Rel49I_IC.R49WarnRW)))
	{		
		if((m_bHaveMakeRSTRpt == FALSE)&&(m_bHaveMakeWarnRpt == TRUE)) //无告警继电器字置位，
		{
			MakeAlarmRpt(Rel49I_IA.R49RSTRptNum);
			m_bHaveMakeRSTRpt = TRUE;
			m_bHaveMakeActRpt = FALSE;
			m_bHaveMakeWarnRpt = FALSE;
		}
	}
}
//保护采样结束消息处理
void CRelayTask::OnSampleEnd(DWORD arg)
{	
//	MCF_GPIO_PODR_TIMER |= 0x02;		//高电平

	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}
//	if(RelayCheckErr)
//	{
//		//ReportMsg("Relay Check Serious Error,and Relay Task Locked!");
//		LedAllOn();
//		return ;
//	}
	static WORD wInvalidCheckCnt;	
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;

	if(!m_bLogicChanged)
		BoBreakCheck();

	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	if(FALSE == WriteComposeBuf())
		return;
	CaluUpIp();
	CaluUI(); 	
	//首先进行开出自检
	
	if(m_bFaultOnFlag == FALSE)
	{
		PTCheck();							
	}
	
	FalutProcess();								//故障循环处理
	GeneralOverValueRelay(&Rel50IA1);
	GeneralOverValueRelay(&Rel50IB1);
	GeneralOverValueRelay(&Rel50IC1);	
	GeneralUnderValRelay(&Rel27U);				//低电压保护子程序
	if(g_bRel49TimeOut == TRUE)
	{
		ThermalOverLoad();
		g_bRel49TimeOut = FALSE;
	}
	LogicProcess();
	Process52BF();
	
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
		//失效自检,200次保护处理进行一路开出检查
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
	
	//写数字录波
	WriteDCChannel();

	//MCF_GPIO_PODR_TIMER &= ~0x02;		//低电平
}
/************************************************************************************************
*功能: 向数字通道中写入录波数据
*参数: 无
*返回: 无
*备注:
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
*功能: 初始化数字通道录波相关信息
*参数: 无
*返回: 无
*备注:
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
// 名称: 通道合成	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::WriteComposeBuf()
{
	//合成计算线电压所用的采样数据
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{	
		//Uab
		dwComposeBuf[i] = dwSampBuf[i]-dwSampBuf[READ_POINT_NUM+i];
		//Ubc
		dwComposeBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM+i]-dwSampBuf[READ_POINT_NUM*2+i];
		//Uca
		dwComposeBuf[READ_POINT_NUM*2+i] = dwSampBuf[READ_POINT_NUM*2+i]-dwSampBuf[i];
		//Ulab
		dwComposeBuf[READ_POINT_NUM*3+i] = dwSampBuf[READ_POINT_NUM*4+i]-dwSampBuf[READ_POINT_NUM*5+i];
		if(*AS_pnLCN == LCUR_2)
		{
			dwComposeBuf[READ_POINT_NUM*4+i] = dwSampBuf[READ_POINT_NUM*10+i]; //Ia
			dwComposeBuf[READ_POINT_NUM*5+i] = dwSampBuf[READ_POINT_NUM*12+i]; //Ib
			dwComposeBuf[READ_POINT_NUM*6+i] = dwSampBuf[READ_POINT_NUM*13+i]; //Ijx
		}
		else if(*AS_pnLCN == LCUR_4)
		{
			dwComposeBuf[READ_POINT_NUM*4+i] = dwSampBuf[READ_POINT_NUM*10+i]-dwSampBuf[READ_POINT_NUM*11+i]; //Ia
			dwComposeBuf[READ_POINT_NUM*5+i] = dwSampBuf[READ_POINT_NUM*12+i]-dwSampBuf[READ_POINT_NUM*13+i];  //Ib
			dwComposeBuf[READ_POINT_NUM*6+i] = 0;//VX时无间隙过流保护
		}
		else
		{
			dwComposeBuf[READ_POINT_NUM*4+i] = 0;
			dwComposeBuf[READ_POINT_NUM*5+i] = 0;
			dwComposeBuf[READ_POINT_NUM*6+i] = 0;
		}
	}
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM*5-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwComposeBuf[READ_POINT_NUM*6-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*2+i] = dwComposeBuf[READ_POINT_NUM*7-RELAY_SAMP_INTERVAL+i];
	}
	if(WriteComposeData(dwRecBuf, dwComposeDataMask))
	{
		ReportMsg("Error:Write ComposeData failure!");
		RelayCheckErr = TRUE;
		return FALSE;
	}
	return TRUE;

}
//------------------------------------------------------------------------------------------ 
// 名称: 计算电量	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUpIp(void)
{
	//计算线电压
	CaluBaseFourier_S15(dwComposeBuf,&LineUHAB);
	CaluModValue(&LineUHAB);	
	*RM_pnUHAB= LineUHAB.Mod;                                       
	DB_LogWriteRM(RM_UHAB, *RM_pnUHAB);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&LineUHBC);
	CaluModValue(&LineUHBC);	
	*RM_pnUHBC= LineUHBC.Mod;                                       
	DB_LogWriteRM(RM_UHBC, *RM_pnUHBC);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*2,&LineUHCA);
	CaluModValue(&LineUHCA);	
	*RM_pnUHCA= LineUHCA.Mod;                                       
	DB_LogWriteRM(RM_UHCA, *RM_pnUHCA);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*3,&LineULAB);
	CaluModValue(&LineULAB);	
	*RM_pnULAB= LineULAB.Mod;                                       
	DB_LogWriteRM(RM_ULAB, *RM_pnULAB);
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*4,&ILA);
	CaluModValue(&ILA);	
	*RM_pnILA= ILA.Mod;                                       
	DB_LogWriteRM(RM_ILA, *RM_pnILA);	
	if(ILA.Mod>(*AS_pn50LAC))
	{				  
		WriteRelayWord(RW_50LA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LA, RELAY_OFF);
	}
	if(ILA.Mod>(*AS_pn37LAC))
	{
		WriteRelayWord(RW_37LA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LA, RELAY_ON);
	}
	//计算ILB
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*5,&ILB);
	CaluModValue(&ILB);	
	*RM_pnILB= ILB.Mod;                                       
	DB_LogWriteRM(RM_ILB, *RM_pnILB);	
	if(ILB.Mod>(*AS_pn50LBC))
	{				  
		WriteRelayWord(RW_50LB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50LB, RELAY_OFF);
	}
	if(ILB.Mod>(*AS_pn37LBC))
	{
		WriteRelayWord(RW_37LB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37LB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*6,&IJX);
	CaluModValue(&IJX);	
	*RM_pnIJX= IJX.Mod;                                       
	DB_LogWriteRM(RM_IJX, *RM_pnIJX);	
	if(IJX.Mod>(*AS_pn50JXC))
	{				  
		WriteRelayWord(RW_50JX, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50JX, RELAY_OFF);
	}
	if(IJX.Mod>(*AS_pn37JXC))
	{
		WriteRelayWord(RW_37JX, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37JX, RELAY_ON);
	}

	//计算热过负荷所用的保护电流，并进行累加计算---真有效值
	if((*CFG_pby49 == CFG_STATUS_SET)&&(*SW_pby49 == SW_STATUS_SET))
	{
		Filter(dwSampBuf+READ_POINT_NUM*6);
		Filter(dwSampBuf+READ_POINT_NUM*7);
		Filter(dwSampBuf+READ_POINT_NUM*8);
		
		CaluMSMod(dwSampBuf+READ_POINT_NUM*6, IAP);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*7, IBP);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*8, ICP);
		
		Rel49I_IA.pCurrent = (IAP+Rel49I_IA.PreCurrent)/2;
		Rel49I_IA.PreCurrent = Rel49I_IA.pCurrent;
		Rel49I_IB.pCurrent = (IBP+Rel49I_IB.PreCurrent)/2;
		Rel49I_IB.PreCurrent = Rel49I_IB.pCurrent;
		Rel49I_IC.pCurrent = (ICP+Rel49I_IC.PreCurrent)/2;
		Rel49I_IC.PreCurrent = Rel49I_IC.pCurrent;
	}

	//求出3个线电压的最大值以及最小值,其中最大值用于高侧PT断线的判断，最小值用于低侧PT断线的判断
	PhaseUHMin = MinElecABC(UHA,UHB,UHC);//高压侧PT断线判断改为相电压判断，yanzh 2013年1月23日14:29:49
	LineUHMax = MaxElecABC(LineUHAB,LineUHBC,LineUHCA);
	//三相失压保护用的电压是高压侧三个相电压，低压侧两个电压共5个电压中的最大值
	PhaseUHMax = MaxElecABC(UHA,UHB,UHC);
	U_R27U = MaxElecABC(PhaseUHMax,ULA,ULB);
	
	//得出高侧失压保护电流闭锁用的电流  高侧三相电流最大值
	I_R27U = MaxElecABC(IHA,IHB,IHC);
	//低压侧两个电压中的大值用于高压侧PT断线判断
	//低压侧两个电压和他们的线电压三个电压中的最小值用于高压侧过流保护的低压闭锁
	if(ULA.Mod>=ULB.Mod)
	{	
		PhaseULMin = ULB;
		PhaseULMax = ULA;
	}
	else
	{
		PhaseULMin = ULA;
		PhaseULMax = ULB;
	}

	if(PhaseULMin.Mod <= LineULAB.Mod*2)
	{
		U_R50IH = PhaseULMin;
	}
	else
	{
		U_R50IH.Mod = LineULAB.Mod*2;   //低压侧线电压乘2与定值进行比较
	}

	if(ULA.Mod>=LineULAB.Mod*2)
		ULmin1.Mod =  LineULAB.Mod*2;
	else
		ULmin1 = ULA;

	if(ULB.Mod>=LineULAB.Mod*2)
		ULmin2.Mod =  LineULAB.Mod*2;
	else
		ULmin2 = ULB;
}
//------------------------------------------------------------------------------------------ 
// 名称: 计算电量	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::CaluUI(void)
{	
	 //计算UHA
	CaluBaseFourier_S15(dwSampBuf,&UHA);
	CaluModValue(&UHA);	
	*RM_pnUHA= UHA.Mod;                                       
	DB_LogWriteRM(RM_UHA, *RM_pnUHA);
	if(UHA.Mod>(*AS_pn59HAV))
	{				
		WriteRelayWord(RW_59HA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59HA, RELAY_OFF);
	}
	if(UHA.Mod>(*AS_pn27HAV))
	{
		WriteRelayWord(RW_27HA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27HA, RELAY_ON);
	}
	  //计算UHB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UHB);
	CaluModValue(&UHB);
	*RM_pnUHB= UHB.Mod;									   
	DB_LogWriteRM(RM_UHB, *RM_pnUHB);	
	if(UHB.Mod>(*AS_pn59HBV))
	{				 
		 WriteRelayWord(RW_59HB, RELAY_ON);
	}
	else
	{
		 WriteRelayWord(RW_59HB, RELAY_OFF);
	}
	if(UHB.Mod>(*AS_pn27HBV))
	{
		 WriteRelayWord(RW_27HB, RELAY_OFF);
	}
	else
	{
		 WriteRelayWord(RW_27HB, RELAY_ON);
	}
	  //计算UHC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UHC);
	CaluModValue(&UHC);
	*RM_pnUHC= UHC.Mod;									   
	DB_LogWriteRM(RM_UHC, *RM_pnUHC);	
	if(UHC.Mod>(*AS_pn59HCV))
	{				 
		 WriteRelayWord(RW_59HC, RELAY_ON);
	}
	else
	{
		 WriteRelayWord(RW_59HC, RELAY_OFF);
	}
	if(UHC.Mod>(*AS_pn27HCV))
	{
		 WriteRelayWord(RW_27HC, RELAY_OFF);
	}
	else
	{
		 WriteRelayWord(RW_27HC, RELAY_ON);
	}

	//计算U0
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0);
	CaluModValue(&U0);
	*RM_pnU0= U0.Mod;									
	DB_LogWriteRM(RM_U0, *RM_pnU0);
	if(U0.Mod>(*AS_pn59NV))
	{				  
		WriteRelayWord(RW_59N, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59N, RELAY_OFF);
	}
	if(U0.Mod>(*AS_pn27NV))
	{
		WriteRelayWord(RW_27N, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27N, RELAY_ON);
	}
	//计算ULA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&ULA);
	CaluModValue(&ULA);
	*RM_pnULA= ULA.Mod;									
	DB_LogWriteRM(RM_ULA, *RM_pnULA);	
	if(ULA.Mod>(*AS_pn59LAV))
	{				  
		WriteRelayWord(RW_59LA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LA, RELAY_OFF);
	}
	if(ULA.Mod>(*AS_pn27LAV))
	{
		WriteRelayWord(RW_27LA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LA, RELAY_ON);
	}
	//计算ULB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&ULB);
	CaluModValue(&ULB);
	*RM_pnULB= ULB.Mod;									
	DB_LogWriteRM(RM_ULB, *RM_pnULB);	
	if(ULB.Mod>(*AS_pn59LBV))
	{				  
		WriteRelayWord(RW_59LB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59LB, RELAY_OFF);
	}
	if(ULB.Mod>(*AS_pn27LBV))
	{
		WriteRelayWord(RW_27LB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27LB, RELAY_ON);
	}
	//计算IHA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&IHA);
	CaluModValue(&IHA);
	*RM_pnIHA= IHA.Mod;									
	DB_LogWriteRM(RM_IHA, *RM_pnIHA);	
	if(IHA.Mod>(*AS_pn50HAC))
	{				  
		WriteRelayWord(RW_50HA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HA, RELAY_OFF);
	}
	if(IHA.Mod>(*AS_pn37HAC))
	{
		WriteRelayWord(RW_37HA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HA, RELAY_ON);
	}
	//计算IHB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&IHB);
	CaluModValue(&IHB);
	*RM_pnIHB= IHB.Mod;									
	DB_LogWriteRM(RM_IHB, *RM_pnIHB);
	if(IHB.Mod>(*AS_pn50HBC))
	{				  
		WriteRelayWord(RW_50HB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HB, RELAY_OFF);
	}
	if(IHB.Mod>(*AS_pn37HBC))
	{
		WriteRelayWord(RW_37HB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HB, RELAY_ON);
	}
	//计算IHC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IHC);
	CaluModValue(&IHC);
	*RM_pnIHC= IHC.Mod;									
	DB_LogWriteRM(RM_IHC, *RM_pnIHC);
	if(IHC.Mod>(*AS_pn50HCC))
	{				  
		WriteRelayWord(RW_50HC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50HC, RELAY_OFF);
	}
	if(IHC.Mod>(*AS_pn37HCC))
	{
		WriteRelayWord(RW_37HC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37HC, RELAY_ON);
	}
	//计算I0
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&I0);
	CaluModValue(&I0);
	*RM_pnI0= I0.Mod;									
	DB_LogWriteRM(RM_I0, *RM_pnI0);
	if(I0.Mod>(*AS_pn50NC))
	{				  
		WriteRelayWord(RW_50N, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50N, RELAY_OFF);
	}
	if(I0.Mod>(*AS_pn37NC))
	{
		WriteRelayWord(RW_37N, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37N, RELAY_ON);
	}
	//计算IL1
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&IL1);
	CaluModValue(&IL1);
	*RM_pnIL1= IL1.Mod;									
	DB_LogWriteRM(RM_IL1, *RM_pnIL1);
	//计算IL2
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&IL2);
	CaluModValue(&IL2);
	*RM_pnIL2= IL2.Mod;									
	DB_LogWriteRM(RM_IL2, *RM_pnIL2);
	//计算IL3
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&IL3);
	CaluModValue(&IL3);
	*RM_pnIL3= IL3.Mod;									
	DB_LogWriteRM(RM_IL3, *RM_pnIL3);	
	//计算IL4
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&IL4);
	CaluModValue(&IL4);
	*RM_pnIL4= IL4.Mod;									
	DB_LogWriteRM(RM_IL4, *RM_pnIL4);	
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化高侧A过流保护动作元件                                                                   
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_IA()
{
	Rel50I_IA.pElecSet			= RS_pn50HAC;          			//过量保护电量定值
	Rel50I_IA.pTimeSet			= RS_pn50HAD;				 	//过量保护时间定值
	Rel50I_IA.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_IA.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_IA.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_IA.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_IA.pRelCFG			= CFG_pby50HA;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_IA.pRelSW			= SW_pby50H;         			//过量保护软压板------1:投入，2:退出
	Rel50I_IA.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_IA.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_IA.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_IA.pRelVolLockCFG	= CFG_pby50H_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_IA.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_IA.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_IA.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_IA.pRel47UCFG		= &RelCFGCLR;
	Rel50I_IA.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_IA.pRelElec 			= &IHA;							//当前电量指针		
	Rel50I_IA.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50I_IA.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50I_IA.pRelVolt			= &U_R50IH;							//当前用来闭锁的电压的指针

	Rel50I_IA.StartRelayWord	= RW_PI50HA;					//过量保护入段继电器字
	Rel50I_IA.ActRelayWord		= RW_TR50HA;					//过量保护动作继电器字
	Rel50I_IA.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_IA.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_IA.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50I_IA.PRewardRelayWord  = FALSE1;						//功率反方向继电器字
	Rel50I_IA.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_IA.R47ULKRelayWord   = FALSE1;
	Rel50I_IA.LockRelayWord		= RW_LKPI50HA;					//入段闭锁继电器字
	Rel50I_IA.RSTRelayWord		= RW_LKRST50HA;				//返回闭锁继电器字
	Rel50I_IA.LoopCtrlRelayWord	= RW_LPC50HA;					//保护循环控制继电器字
	
	Rel50I_IA.wReportNum		= R_R50IHA;						//过量保护报告序号
	Rel50I_IA.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_IA.wPickUpReportNum	= R_R50IHA_PI;						//入段报告序列号
	Rel50I_IA.wResetReportNum	= R_R50IHA_RES;						//返回报告序列号
	//**********************************************************************	

	Rel50I_IA.bMakeActRpt		= TRUE;
	Rel50I_IA.dwK35				= NULL;
	Rel50I_IA.RptSerialNum		= &m_wRptSerialNumOfAct;		
//	Rel50I_IA.dwRelTimer		= 0;			 			//定时器
	Rel50I_IA.byRelFCounter		= 0;						//故障计数器
	Rel50I_IA.byRelRCounter		= 0;						//返回计数器	
	Rel50I_IA.wExcepWordNum     = 8;

}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化高侧B过流保护元件                                                                    
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_IB()
{
	Rel50I_IB.pElecSet			= RS_pn50HBC;          			//过量保护电量定值
	Rel50I_IB.pTimeSet			= RS_pn50HBD;				 	//过量保护时间定值
	Rel50I_IB.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_IB.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_IB.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_IB.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_IB.pRelCFG			= CFG_pby50HB;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_IB.pRelSW			= SW_pby50H;         			//过量保护软压板------1:投入，2:退出
	Rel50I_IB.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_IB.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_IB.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_IB.pRelVolLockCFG	= CFG_pby50H_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_IB.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_IB.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_IB.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	
	Rel50I_IB.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_IB.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_IB.pRelElec 			= &IHB;							//当前电量指针		
	Rel50I_IB.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50I_IB.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50I_IB.pRelVolt			= &U_R50IH;							//当前用来闭锁的电压的指针

	Rel50I_IB.StartRelayWord	= RW_PI50HB;					//过量保护入段继电器字
	Rel50I_IB.ActRelayWord		= RW_TR50HB;					//过量保护动作继电器字
	Rel50I_IB.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_IB.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_IB.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50I_IB.PRewardRelayWord  = FALSE1;						//功率反方向继电器字
	Rel50I_IB.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	
	Rel50I_IB.R47ULKRelayWord   = FALSE1;
	Rel50I_IB.LockRelayWord		= RW_LKPI50HB;					//入段闭锁继电器字
	Rel50I_IB.RSTRelayWord		= RW_LKRST50HB;				//返回闭锁继电器字
	Rel50I_IB.LoopCtrlRelayWord	= RW_LPC50HB;					//保护循环控制继电器字
	
	Rel50I_IB.wReportNum		= R_R50IHB;						//过量保护报告序号
	Rel50I_IB.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	Rel50I_IB.bMakeActRpt		= TRUE;
	Rel50I_IB.dwK35				= NULL;
	Rel50I_IB.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_IB.wPickUpReportNum	= R_R50IHB_PI;						//入段报告序列号
	Rel50I_IB.wResetReportNum	= R_R50IHB_RES;						//返回报告序列号
	//**********************************************************************	


//	Rel50I_IB.dwRelTimer		= 0;			 			//定时器
	Rel50I_IB.byRelFCounter		= 0;						//故障计数器
	Rel50I_IB.byRelRCounter		= 0;						//返回计数器	
	
	Rel50I_IB.wExcepWordNum     = 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化高侧C过流保护元件                                              
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_IC()
{	
	Rel50I_IC.pElecSet			= RS_pn50HCC;          			//过量保护电量定值
	Rel50I_IC.pTimeSet			= RS_pn50HCD;				 	//过量保护时间定值
	Rel50I_IC.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_IC.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_IC.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_IC.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_IC.pRelCFG			= CFG_pby50HC;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_IC.pRelSW			= SW_pby50H;         			//过量保护软压板------1:投入，2:退出
	Rel50I_IC.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_IC.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_IC.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_IC.pRelVolLockCFG	= CFG_pby50H_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_IC.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_IC.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_IC.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_IC.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_IC.pRelElec 			= &IHC;							//当前电量指针		
	Rel50I_IC.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50I_IC.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50I_IC.pRelVolt			= &U_R50IH;							//当前用来闭锁的电压的指针


	Rel50I_IC.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_IC.R47ULKRelayWord   = FALSE1;

	Rel50I_IC.StartRelayWord	= RW_PI50HC;					//过量保护入段继电器字
	Rel50I_IC.ActRelayWord		= RW_TR50HC;					//过量保护动作继电器字
	Rel50I_IC.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_IC.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_IC.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50I_IC.PRewardRelayWord  = FALSE1;						//功率反方向继电器字
	Rel50I_IC.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_IC.LockRelayWord		= RW_LKPI50HC;					//入段闭锁继电器字
	Rel50I_IC.RSTRelayWord		= RW_LKRST50HC;				//返回闭锁继电器字
	Rel50I_IC.LoopCtrlRelayWord	= RW_LPC50HC;					//保护循环控制继电器字
	
	Rel50I_IC.wReportNum		= R_R50IHC;						//过量保护报告序号
	Rel50I_IC.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	Rel50I_IC.bMakeActRpt		= TRUE;
	Rel50I_IC.dwK35				= NULL;
	Rel50I_IC.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_IC.wPickUpReportNum	= R_R50IHC_PI;						//入段报告序列号
	Rel50I_IC.wResetReportNum	= R_R50IHC_RES;						//返回报告序列号
	//**********************************************************************	


//	Rel50I_IC.dwRelTimer		= 0;			 			//定时器
	Rel50I_IC.byRelFCounter		= 0;						//故障计数器
	Rel50I_IC.byRelRCounter		= 0;						//返回计数器	
	
	Rel50I_IC.wExcepWordNum     = 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化低侧A过流I段保护元件                                        
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ia1()
{	
	Rel50I_Ia1.pElecSet			= RS_pn50LA1C;          			//过量保护电量定值
	Rel50I_Ia1.pTimeSet			= RS_pn50LA1D;				 	//过量保护时间定值
	Rel50I_Ia1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_Ia1.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_Ia1.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_Ia1.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_Ia1.pRelCFG			= CFG_pby50LA1;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_Ia1.pRelSW			= SW_pby50L;         			//过量保护软压板------1:投入，2:退出
	Rel50I_Ia1.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_Ia1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_Ia1.pRelPDCFG		= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_Ia1.pRelVolLockCFG	= CFG_pby50LA_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_Ia1.pRelFDCFG		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_Ia1.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_Ia1.pRelFLCFG		= &RelCFGCLR;					//故障测距投入
	Rel50I_Ia1.pSysTimer		= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_Ia1.pRelElec 		= &ILA;							//当前电量指针		
	Rel50I_Ia1.pRelHarm3		= NULL;							//当前三次谐波指针	
	Rel50I_Ia1.pRelHarm5		= NULL;							//当前五次谐波指针	
	Rel50I_Ia1.pRelVolt			= &ULmin1;							//当前用来闭锁的电压的指针
	Rel50I_Ia1.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ia1.R47ULKRelayWord   = FALSE1;

	Rel50I_Ia1.StartRelayWord	= RW_PI50LA1;					//过量保护入段继电器字
	Rel50I_Ia1.ActRelayWord		= RW_TR50LA1;					//过量保护动作继电器字
	Rel50I_Ia1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_Ia1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_Ia1.PForwardRelayWord= FALSE1;						//功率正方向继电器字
	Rel50I_Ia1.PRewardRelayWord = FALSE1;						//功率反方向继电器字
	Rel50I_Ia1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_Ia1.LockRelayWord	= RW_LKPI50LA1;					//入段闭锁继电器字
	Rel50I_Ia1.RSTRelayWord		= RW_LKRST50LA1;				//返回闭锁继电器字
	Rel50I_Ia1.LoopCtrlRelayWord= RW_LPC50LA1;					//保护循环控制继电器字
	
	Rel50I_Ia1.wReportNum		= R_R50ILA1;						//过量保护报告序号
	Rel50I_Ia1.wAccReportNum	= 0xFFFF;						//加速动作报告序号

	Rel50I_Ia1.bMakeActRpt		= TRUE;
	Rel50I_Ia1.dwK35			= NULL;
	Rel50I_Ia1.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_Ia1.wPickUpReportNum	= R_R50ILA1_PI;						//入段报告序列号
	Rel50I_Ia1.wResetReportNum	= R_R50ILA1_RES;						//返回报告序列号
	//**********************************************************************	


//	Rel50I_Ia1.dwRelTimer		= 0;			 			//定时器
	Rel50I_Ia1.byRelFCounter	= 0;						//故障计数器
	Rel50I_Ia1.byRelRCounter	= 0;						//返回计数器	
	Rel50I_Ia1.wExcepWordNum    = 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化低侧A过流II段保护元件                                        
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ia2()
{	
	Rel50I_Ia2.pElecSet			= RS_pn50LA2C;          			//过量保护电量定值
	Rel50I_Ia2.pTimeSet			= RS_pn50LA2D;				 	//过量保护时间定值
	Rel50I_Ia2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_Ia2.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_Ia2.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_Ia2.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_Ia2.pRelCFG			= CFG_pby50LA2;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_Ia2.pRelSW			= SW_pby50L;         			//过量保护软压板------1:投入，2:退出
	Rel50I_Ia2.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_Ia2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_Ia2.pRelPDCFG		= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_Ia2.pRelVolLockCFG	= CFG_pby50LA_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_Ia2.pRelFDCFG		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_Ia2.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_Ia2.pRelFLCFG		= &RelCFGCLR;					//故障测距投入
	Rel50I_Ia2.pSysTimer		= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_Ia2.pRelElec 		= &ILA;							//当前电量指针		
	Rel50I_Ia2.pRelHarm3		= NULL;							//当前三次谐波指针	
	Rel50I_Ia2.pRelHarm5		= NULL;							//当前五次谐波指针	
	Rel50I_Ia2.pRelVolt			= &ULmin1;							//当前用来闭锁的电压的指针
	Rel50I_Ia2.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ia2.R47ULKRelayWord   = FALSE1;

	Rel50I_Ia2.StartRelayWord	= RW_PI50LA2;					//过量保护入段继电器字
	Rel50I_Ia2.ActRelayWord		= RW_TR50LA2;					//过量保护动作继电器字
	Rel50I_Ia2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_Ia2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_Ia2.PForwardRelayWord= FALSE1;						//功率正方向继电器字
	Rel50I_Ia2.PRewardRelayWord = FALSE1;						//功率反方向继电器字
	Rel50I_Ia2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_Ia2.LockRelayWord	= RW_LKPI50LA2;					//入段闭锁继电器字
	Rel50I_Ia2.RSTRelayWord		= RW_LKRST50LA2;				//返回闭锁继电器字
	Rel50I_Ia2.LoopCtrlRelayWord= RW_LPC50LA2;					//保护循环控制继电器字
	
	Rel50I_Ia2.wReportNum		= R_R50ILA2;						//过量保护报告序号
	Rel50I_Ia2.wAccReportNum	= 0xFFFF;						//加速动作报告序号

	Rel50I_Ia2.bMakeActRpt		= TRUE;
	Rel50I_Ia2.dwK35			= NULL;
	Rel50I_Ia2.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_Ia2.wPickUpReportNum	= R_R50ILA2_PI;						//入段报告序列号
	Rel50I_Ia2.wResetReportNum	= R_R50ILA2_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_Ia2.dwRelTimer		= 0;			 			//定时器
	Rel50I_Ia2.byRelFCounter	= 0;						//故障计数器
	Rel50I_Ia2.byRelRCounter	= 0;						//返回计数器	
	Rel50I_Ia2.wExcepWordNum    = 8;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化低侧B过流I段保护元件                                              
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ib1()
{	
	Rel50I_Ib1.pElecSet			= RS_pn50LB1C;          			//过量保护电量定值
	Rel50I_Ib1.pTimeSet			= RS_pn50LB1D;				 	//过量保护时间定值
	Rel50I_Ib1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_Ib1.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_Ib1.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_Ib1.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_Ib1.pRelCFG			= CFG_pby50LB1;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_Ib1.pRelSW			= SW_pby50L;         			//过量保护软压板------1:投入，2:退出
	Rel50I_Ib1.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_Ib1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_Ib1.pRelPDCFG		= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_Ib1.pRelVolLockCFG	= CFG_pby50LB_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_Ib1.pRelFDCFG		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_Ib1.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_Ib1.pRelFLCFG		= &RelCFGCLR;					//故障测距投入
	Rel50I_Ib1.pSysTimer		= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_Ib1.pRelElec 		= &ILB;							//当前电量指针		
	Rel50I_Ib1.pRelHarm3		= NULL;							//当前三次谐波指针	
	Rel50I_Ib1.pRelHarm5		= NULL;							//当前五次谐波指针	
	Rel50I_Ib1.pRelVolt			= &ULmin2;							//当前用来闭锁的电压的指针
	Rel50I_Ib1.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ib1.R47ULKRelayWord   = FALSE1;

	Rel50I_Ib1.StartRelayWord	= RW_PI50LB1;					//过量保护入段继电器字
	Rel50I_Ib1.ActRelayWord		= RW_TR50LB1;					//过量保护动作继电器字
	Rel50I_Ib1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_Ib1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_Ib1.PForwardRelayWord= FALSE1;						//功率正方向继电器字
	Rel50I_Ib1.PRewardRelayWord = FALSE1;						//功率反方向继电器字
	Rel50I_Ib1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_Ib1.LockRelayWord	= RW_LKPI50LB1;					//入段闭锁继电器字
	Rel50I_Ib1.RSTRelayWord		= RW_LKRST50LB1;				//返回闭锁继电器字
	Rel50I_Ib1.LoopCtrlRelayWord= RW_LPC50LB1;					//保护循环控制继电器字
	
	Rel50I_Ib1.wReportNum		= R_R50ILB1;						//过量保护报告序号
	Rel50I_Ib1.wAccReportNum	= 0xFFFF;						//加速动作报告序号

	Rel50I_Ib1.bMakeActRpt		= TRUE;
	Rel50I_Ib1.dwK35			= NULL;
	Rel50I_Ib1.RptSerialNum		= &m_wRptSerialNumOfAct;				
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_Ib1.wPickUpReportNum	= R_R50ILB1_PI;						//入段报告序列号
	Rel50I_Ib1.wResetReportNum	= R_R50ILB1_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_Ib1.dwRelTimer		= 0;			 			//定时器
	Rel50I_Ib1.byRelFCounter	= 0;						//故障计数器
	Rel50I_Ib1.byRelRCounter	= 0;						//返回计数器		
	Rel50I_Ib1.wExcepWordNum     = 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化低侧B过流II段保护元件                                              
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_Ib2()
{	
	Rel50I_Ib2.pElecSet			= RS_pn50LB2C;          			//过量保护电量定值
	Rel50I_Ib2.pTimeSet			= RS_pn50LB2D;				 	//过量保护时间定值
	Rel50I_Ib2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_Ib2.pVolLockSet		= RS_pn27LU;				 	//过量保护电压闭锁定值
	Rel50I_Ib2.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_Ib2.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_Ib2.pRelCFG			= CFG_pby50LB2;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_Ib2.pRelSW			= SW_pby50L;         			//过量保护软压板------1:投入，2:退出
	Rel50I_Ib2.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_Ib2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_Ib2.pRelPDCFG		= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_Ib2.pRelVolLockCFG	= CFG_pby50LB_27;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_Ib2.pRelFDCFG		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_Ib2.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_Ib2.pRelFLCFG		= &RelCFGCLR;					//故障测距投入
	Rel50I_Ib2.pSysTimer		= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_Ib2.pRelElec 		= &ILB;							//当前电量指针		
	Rel50I_Ib2.pRelHarm3		= NULL;							//当前三次谐波指针	
	Rel50I_Ib2.pRelHarm5		= NULL;							//当前五次谐波指针	
	Rel50I_Ib2.pRelVolt			= &ULmin2;							//当前用来闭锁的电压的指针
	Rel50I_Ib2.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_Ib2.R47ULKRelayWord   = FALSE1;

	Rel50I_Ib2.StartRelayWord	= RW_PI50LB2;					//过量保护入段继电器字
	Rel50I_Ib2.ActRelayWord		= RW_TR50LB2;					//过量保护动作继电器字
	Rel50I_Ib2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_Ib2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_Ib2.PForwardRelayWord= FALSE1;						//功率正方向继电器字
	Rel50I_Ib2.PRewardRelayWord = FALSE1;						//功率反方向继电器字
	Rel50I_Ib2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_Ib2.LockRelayWord	= RW_LKPI50LB2;					//入段闭锁继电器字
	Rel50I_Ib2.RSTRelayWord		= RW_LKRST50LB2;				//返回闭锁继电器字
	Rel50I_Ib2.LoopCtrlRelayWord= RW_LPC50LB2;					//保护循环控制继电器字
	
	Rel50I_Ib2.wReportNum		= R_R50ILB2;						//过量保护报告序号
	Rel50I_Ib2.wAccReportNum	= 0xFFFF;						//加速动作报告序号

	Rel50I_Ib2.bMakeActRpt		= TRUE;
	Rel50I_Ib2.dwK35			= NULL;
	Rel50I_Ib2.RptSerialNum		= &m_wRptSerialNumOfAct;				
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_Ib2.wPickUpReportNum	= R_R50ILB2_PI;						//入段报告序列号
	Rel50I_Ib2.wResetReportNum	= R_R50ILB2_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_Ib2.dwRelTimer		= 0;			 			//定时器
	Rel50I_Ib2.byRelFCounter	= 0;						//故障计数器
	Rel50I_Ib2.byRelRCounter	= 0;						//返回计数器		
	Rel50I_Ib2.wExcepWordNum    = 8;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化间隙过流保护元件                                          
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I_JX()
{	
	Rel50I_JX.pElec1Set			= RS_pn50DCGC;          			//过量保护电量定值	
	Rel50I_JX.pElec2Set			= RS_pn59NV;          			//过量保护电量定值
	Rel50I_JX.pTimeSet			= RS_pn50DCGD;				 	//过量保护时间定值
	Rel50I_JX.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_JX.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50I_JX.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	Rel50I_JX.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_JX.pRelCFG			= CFG_pby50DCG;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I_JX.pRelSW			= SW_pby50JX;         			//过量保护软压板------1:投入，2:退出
	Rel50I_JX.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_JX.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_JX.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I_JX.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_JX.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_JX.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I_JX.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_JX.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I_JX.pRelElec1 		= &IJX;							//当前电量指针	
	Rel50I_JX.pRelElec2 		= &U0;							//当前电量指针	
	Rel50I_JX.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50I_JX.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50I_JX.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50I_JX.pRel47UCFG		= &RelCFGCLR;;
	Rel50I_JX.R47ULKRelayWord   = FALSE1;

	Rel50I_JX.StartRelayWord	= RW_PI50DCG;					//过量保护入段继电器字
	Rel50I_JX.ActRelayWord		= RW_TR50DCG;					//过量保护动作继电器字
	Rel50I_JX.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I_JX.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50I_JX.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50I_JX.PRewardRelayWord  = FALSE1;						//功率反方向继电器字
	Rel50I_JX.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50I_JX.LockRelayWord		= RW_LKPI50DCG;					//入段闭锁继电器字
	Rel50I_JX.RSTRelayWord		= RW_LKRST50DCG;				//返回闭锁继电器字
	Rel50I_JX.LoopCtrlRelayWord	= RW_LPC50DCG;					//保护循环控制继电器字
	
	Rel50I_JX.wReportNum		= R_R50IJX;						//过量保护报告序号
	Rel50I_JX.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	Rel50I_JX.bMakeActRpt		= TRUE;
	Rel50I_JX.dwK35				= NULL;
	Rel50I_JX.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I_JX.wPickUpReportNum	= R_R50IJX_PI;						//入段报告序列号
	Rel50I_JX.wResetReportNum	= R_R50IJX_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_JX.dwRelTimer		= 0;			 			//定时器
	Rel50I_JX.byRelFCounter		= 0;						//故障计数器
	Rel50I_JX.byRelRCounter		= 0;						//返回计数器
	
	Rel50I_JX.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化A过负荷1告警保护元件                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IA1()
{	
	Rel50IA1.pElecSet			= RS_pn50OLA1C;          			//过量保护电量定值
	Rel50IA1.pTimeSet			= RS_pn50OLA1D;				 	//过量保护时间定值
	Rel50IA1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50IA1.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50IA1.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50IA1.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50IA1.pRelCFG			= CFG_pby50OLA1;				//过量保护功能控制字--1:投入，2:退出 
	Rel50IA1.pRelSW				= SW_pby50OL;         			//过量保护软压板------1:投入，2:退出
	Rel50IA1.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IA1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50IA1.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50IA1.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50IA1.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50IA1.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50IA1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50IA1.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50IA1.pRelElec 			= &IHA;							//当前电量指针		
	Rel50IA1.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50IA1.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50IA1.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50IA1.pRel47UCFG		= &RelCFGCLR;;
	Rel50IA1.R47ULKRelayWord   = FALSE1;

	Rel50IA1.StartRelayWord		= RW_PI50OLA1;					//过量保护入段继电器字
	Rel50IA1.ActRelayWord		= RW_TR50OLA1;					//过量保护动作继电器字
	Rel50IA1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50IA1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50IA1.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50IA1.PRewardRelayWord   = FALSE1;						//功率反方向继电器字
	Rel50IA1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50IA1.LockRelayWord		= RW_LKPI50OLA1;					//入段闭锁继电器字
	Rel50IA1.RSTRelayWord		= RW_LKRST50OLA1;				//返回闭锁继电器字
	Rel50IA1.LoopCtrlRelayWord	= RW_LPC50OLA1;					//保护循环控制继电器字
	
	Rel50IA1.wReportNum			= R_R50IA1;						//过量保护报告序号
	Rel50IA1.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	Rel50IA1.bMakeActRpt		= FALSE;
	Rel50IA1.dwK35				= NULL;
	Rel50IA1.RptSerialNum		= &m_wRptSerialNumOfAct;						
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50IA1.wPickUpReportNum	= 0xFFFF;						//入段报告序列号
	Rel50IA1.wResetReportNum	= 0xFFFF;						//返回报告序列号
	//**********************************************************************	
//	Rel50IA1.dwRelTimer			= 0;			 			//定时器
	Rel50IA1.byRelFCounter		= 0;						//故障计数器
	Rel50IA1.byRelRCounter		= 0;						//返回计数器	
	
	Rel50IA1.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化B过负荷1告警保护元件                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IB1()
{	
	Rel50IB1.pElecSet			= RS_pn50OLB1C;          			//过量保护电量定值
	Rel50IB1.pTimeSet			= RS_pn50OLB1D;				 	//过量保护时间定值
	Rel50IB1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50IB1.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50IB1.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50IB1.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50IB1.pRelCFG			= CFG_pby50OLB1;				//过量保护功能控制字--1:投入，2:退出 
	Rel50IB1.pRelSW				= SW_pby50OL;         			//过量保护软压板------1:投入，2:退出
	Rel50IB1.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IB1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50IB1.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50IB1.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50IB1.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50IB1.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50IB1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50IB1.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50IB1.pRelElec 			= &IHB;							//当前电量指针		
	Rel50IB1.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50IB1.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50IB1.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50IB1.pRel47UCFG		= &RelCFGCLR;;
	Rel50IB1.R47ULKRelayWord   = FALSE1;

	Rel50IB1.StartRelayWord		= RW_PI50OLB1;					//过量保护入段继电器字
	Rel50IB1.ActRelayWord		= RW_TR50OLB1;					//过量保护动作继电器字
	Rel50IB1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50IB1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50IB1.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50IB1.PRewardRelayWord   = FALSE1;						//功率反方向继电器字
	Rel50IB1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50IB1.LockRelayWord		= RW_LKPI50OLB1;					//入段闭锁继电器字
	Rel50IB1.RSTRelayWord		= RW_LKRST50OLB1;				//返回闭锁继电器字
	Rel50IB1.LoopCtrlRelayWord	= RW_LPC50OLB1;					//保护循环控制继电器字
	
	Rel50IB1.wReportNum			= R_R50IB1;						//过量保护报告序号
	Rel50IB1.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel50IB1.bMakeActRpt		= FALSE;

	Rel50IB1.dwK35				= NULL;
	Rel50IB1.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50IB1.wPickUpReportNum	= 0xFFFF;						//入段报告序列号
	Rel50IB1.wResetReportNum	= 0xFFFF;						//返回报告序列号
	//**********************************************************************	
//	Rel50IB1.dwRelTimer			= 0;			 			//定时器
	Rel50IB1.byRelFCounter		= 0;						//故障计数器
	Rel50IB1.byRelRCounter		= 0;						//返回计数器	
	
	Rel50IB1.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化C过负荷1告警保护元件                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitRel50IC1()
{	
	Rel50IC1.pElecSet			= RS_pn50OLC1C;          			//过量保护电量定值
	Rel50IC1.pTimeSet			= RS_pn50OLC1D;				 	//过量保护时间定值
	Rel50IC1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50IC1.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50IC1.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50IC1.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50IC1.pRelCFG			= CFG_pby50OLC1;				//过量保护功能控制字--1:投入，2:退出 
	Rel50IC1.pRelSW				= SW_pby50OL;         			//过量保护软压板------1:投入，2:退出
	Rel50IC1.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IC1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50IC1.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50IC1.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50IC1.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50IC1.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50IC1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50IC1.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50IC1.pRelElec 			= &IHC;							//当前电量指针		
	Rel50IC1.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50IC1.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50IC1.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50IC1.pRel47UCFG		= &RelCFGCLR;;
	Rel50IC1.R47ULKRelayWord   = FALSE1;

	Rel50IC1.StartRelayWord		= RW_PI50OLC1;					//过量保护入段继电器字
	Rel50IC1.ActRelayWord		= RW_TR50OLC1;					//过量保护动作继电器字
	Rel50IC1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50IC1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50IC1.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50IC1.PRewardRelayWord   = FALSE1;						//功率反方向继电器字
	Rel50IC1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50IC1.LockRelayWord		= RW_LKPI50OLC1;					//入段闭锁继电器字
	Rel50IC1.RSTRelayWord		= RW_LKRST50OLC1;				//返回闭锁继电器字
	Rel50IC1.LoopCtrlRelayWord	= RW_LPC50OLC1;					//保护循环控制继电器字
	
	Rel50IC1.wReportNum			= R_R50IC1;						//过量保护报告序号
	Rel50IC1.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel50IC1.bMakeActRpt		= FALSE;

	Rel50IC1.dwK35				= NULL;
	Rel50IC1.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50IC1.wPickUpReportNum	= 0xFFFF;						//入段报告序列号
	Rel50IC1.wResetReportNum	= 0xFFFF;						//返回报告序列号
	//**********************************************************************	
//	Rel50IC1.dwRelTimer			= 0;			 			//定时器
	Rel50IC1.byRelFCounter		= 0;						//故障计数器
	Rel50IC1.byRelRCounter		= 0;						//返回计数器	
	
	Rel50IC1.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化A过负荷2动作保护元件                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IA2()
{	
	Rel50IA2.pElecSet			= RS_pn50OLA2C;          			//过量保护电量定值
	Rel50IA2.pTimeSet			= RS_pn50OLA2D;				 	//过量保护时间定值
	Rel50IA2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50IA2.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50IA2.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50IA2.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50IA2.pRelCFG			= CFG_pby59OLA2;				//过量保护功能控制字--1:投入，2:退出 
	Rel50IA2.pRelSW				= SW_pby50OL;         			//过量保护软压板------1:投入，2:退出
	Rel50IA2.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IA2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50IA2.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50IA2.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50IA2.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50IA2.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50IA2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50IA2.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50IA2.pRelElec 			= &IHA;							//当前电量指针		
	Rel50IA2.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50IA2.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50IA2.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50IA2.pRel47UCFG		= &RelCFGCLR;;
	Rel50IA2.R47ULKRelayWord   = FALSE1;

	Rel50IA2.StartRelayWord		= RW_PI50OLA2;					//过量保护入段继电器字
	Rel50IA2.ActRelayWord		= RW_TR50OLA2;					//过量保护动作继电器字
	Rel50IA2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50IA2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50IA2.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50IA2.PRewardRelayWord   = FALSE1;						//功率反方向继电器字
	Rel50IA2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50IA2.LockRelayWord		= RW_LKPI50OLA2;					//入段闭锁继电器字
	Rel50IA2.RSTRelayWord		= RW_LKRST50OLA2;				//返回闭锁继电器字
	Rel50IA2.LoopCtrlRelayWord	= RW_LPC50OLA2;					//保护循环控制继电器字
	
	Rel50IA2.wReportNum			= R_R50IA2;						//过量保护报告序号
	Rel50IA2.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel50IA2.bMakeActRpt		= TRUE;
	Rel50IA2.dwK35				= NULL;
	Rel50IA2.RptSerialNum		= &m_wRptSerialNumOfAct;
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50IA2.wPickUpReportNum	= R_R50IA2_PI;						//入段报告序列号
	Rel50IA2.wResetReportNum	= R_R50IA2_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50IA2.dwRelTimer			= 0;			 			//定时器
	Rel50IA2.byRelFCounter		= 0;						//故障计数器
	Rel50IA2.byRelRCounter		= 0;						//返回计数器	
	
	Rel50IA2.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化B过负荷2动作保护元件                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitRel50IB2()
{	
	Rel50IB2.pElecSet			= RS_pn50OLB2C;          			//过量保护电量定值
	Rel50IB2.pTimeSet			= RS_pn50OLB2D;				 	//过量保护时间定值
	Rel50IB2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50IB2.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50IB2.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50IB2.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50IB2.pRelCFG			= CFG_pby50OLB2;				//过量保护功能控制字--1:投入，2:退出 
	Rel50IB2.pRelSW				= SW_pby50OL;         			//过量保护软压板------1:投入，2:退出
	Rel50IB2.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IB2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50IB2.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50IB2.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50IB2.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50IB2.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50IB2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50IB2.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50IB2.pRelElec 			= &IHB;							//当前电量指针		
	Rel50IB2.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50IB2.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50IB2.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50IB2.pRel47UCFG		= &RelCFGCLR;;
	Rel50IB2.R47ULKRelayWord   = FALSE1;

	Rel50IB2.StartRelayWord		= RW_PI50OLB2;					//过量保护入段继电器字
	Rel50IB2.ActRelayWord		= RW_TR50OLB2;					//过量保护动作继电器字
	Rel50IB2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50IB2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50IB2.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50IB2.PRewardRelayWord   = FALSE1;						//功率反方向继电器字
	Rel50IB2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50IB2.LockRelayWord		= RW_LKPI50OLB2;					//入段闭锁继电器字
	Rel50IB2.RSTRelayWord		= RW_LKRST50OLB2;				//返回闭锁继电器字
	Rel50IB2.LoopCtrlRelayWord	= RW_LPC50OLB2;					//保护循环控制继电器字
	
	Rel50IB2.wReportNum			= R_R50IB2;						//过量保护报告序号
	Rel50IB2.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel50IB2.bMakeActRpt		= TRUE;
	Rel50IB2.dwK35				= NULL;
	Rel50IB2.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50IB2.wPickUpReportNum	= R_R50IB2_PI;						//入段报告序列号
	Rel50IB2.wResetReportNum	= R_R50IB2_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50IB2.dwRelTimer			= 0;			 			//定时器
	Rel50IB2.byRelFCounter		= 0;						//故障计数器
	Rel50IB2.byRelRCounter		= 0;						//返回计数器	
	
	Rel50IB2.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化C过负荷2动作保护元件                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitRel50IC2()
{	
	Rel50IC2.pElecSet			= RS_pn50OLC2C;          			//过量保护电量定值
	Rel50IC2.pTimeSet			= RS_pn50OLC2D;				 	//过量保护时间定值
	Rel50IC2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50IC2.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	Rel50IC2.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50IC2.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50IC2.pRelCFG			= CFG_pby50OLC2;				//过量保护功能控制字--1:投入，2:退出 
	Rel50IC2.pRelSW				= SW_pby50OL;         			//过量保护软压板------1:投入，2:退出
	Rel50IC2.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IC2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50IC2.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50IC2.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50IC2.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50IC2.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50IC2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50IC2.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50IC2.pRelElec 			= &IHC;							//当前电量指针		
	Rel50IC2.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50IC2.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50IC2.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50IC2.pRel47UCFG		= &RelCFGCLR;;
	Rel50IC2.R47ULKRelayWord   = FALSE1;

	Rel50IC2.StartRelayWord		= RW_PI50OLC2;					//过量保护入段继电器字
	Rel50IC2.ActRelayWord		= RW_TR50OLC2;					//过量保护动作继电器字
	Rel50IC2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50IC2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	Rel50IC2.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50IC2.PRewardRelayWord   = FALSE1;						//功率反方向继电器字
	Rel50IC2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	Rel50IC2.LockRelayWord		= RW_LKPI50OLC2;					//入段闭锁继电器字
	Rel50IC2.RSTRelayWord		= RW_LKRST50OLC2;				//返回闭锁继电器字
	Rel50IC2.LoopCtrlRelayWord	= RW_LPC50OLC2;					//保护循环控制继电器字
	
	Rel50IC2.wReportNum			= R_R50IC2;						//过量保护报告序号
	Rel50IC2.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel50IC2.bMakeActRpt		= TRUE;
	Rel50IC2.dwK35				= NULL;
	Rel50IC2.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50IC2.wPickUpReportNum	= R_R50IC2_PI;						//入段报告序列号
	Rel50IC2.wResetReportNum	= R_R50IC2_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50IC2.dwRelTimer			= 0;			 			//定时器
	Rel50IC2.byRelFCounter		= 0;						//故障计数器
	Rel50IC2.byRelRCounter		= 0;						//返回计数器	
	
	Rel50IC2.wExcepWordNum     = 8;
}
//******************************************************************************************
// 名称: 初始化零序过流保护元件                               
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitRel50IN()
{	
	Rel50I0.pElecSet			= RS_pn50NC;          			//过量保护电量定值
	Rel50I0.pTimeSet			= RS_pn50ND;				 	//过量保护时间定值
	Rel50I0.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I0.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	Rel50I0.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel50I0.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I0.pRelCFG				= CFG_pby50N;				//过量保护功能控制字--1:投入，2:退出 
	Rel50I0.pRelSW				= SW_pby50N;         			//过量保护软压板------1:投入，2:退出
	Rel50I0.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I0.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I0.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel50I0.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel50I0.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel50I0.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel50I0.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I0.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel50I0.pRelElec 			= &I0;							//当前电量指针		
	Rel50I0.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel50I0.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel50I0.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel50I0.pRel47UCFG		= &RelCFGCLR;;
	Rel50I0.R47ULKRelayWord   = FALSE1;

	Rel50I0.StartRelayWord		= RW_PI50N;					//过量保护入段继电器字
	Rel50I0.ActRelayWord		= RW_TR50N;					//过量保护动作继电器字
	Rel50I0.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel50I0.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	Rel50I0.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel50I0.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	Rel50I0.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	Rel50I0.LockRelayWord		= RW_LKPI50N;					//入段闭锁继电器字
	Rel50I0.RSTRelayWord		= RW_LKRST50N;					//返回闭锁继电器字
	Rel50I0.LoopCtrlRelayWord	= RW_LPC50N;					//保护循环控制继电器字
	
	Rel50I0.wReportNum			= R_R50IN;						//过量保护报告序号
	Rel50I0.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel50I0.bMakeActRpt			= TRUE;
	Rel50I0.dwK35				= NULL;
	Rel50I0.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel50I0.wPickUpReportNum	= R_R50IN_PI;						//入段报告序列号
	Rel50I0.wResetReportNum		= R_R50IN_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50I0.dwRelTimer			= 0;			 			//定时器
	Rel50I0.byRelFCounter		= 0;						//故障计数器
	Rel50I0.byRelRCounter		= 0;						//返回计数器
	
	Rel50I0.wExcepWordNum     = 8;
}
/*****************************************************************************************
*功能: 初始化零序过压保护元件
*参数: 无
*返回: 无
*备注:
******************************************************************************************/
void CRelayTask::InitRel59UN()
{	
	Rel59U0.pElecSet			= RS_pn59NV;          			//过量保护电量定值
	Rel59U0.pTimeSet			= RS_pn59ND;				 	//过量保护时间定值
	Rel59U0.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel59U0.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	Rel59U0.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	Rel59U0.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel59U0.pRelCFG				= CFG_pby59N;				//过量保护功能控制字--1:投入，2:退出 
	Rel59U0.pRelSW				= SW_pby59N;         			//过量保护软压板------1:投入，2:退出
	Rel59U0.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel59U0.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel59U0.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel59U0.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel59U0.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel59U0.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel59U0.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel59U0.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel59U0.pRelElec 			= &U0;							//当前电量指针		
	Rel59U0.pRelHarm3			= NULL;							//当前三次谐波指针	
	Rel59U0.pRelHarm5			= NULL;							//当前五次谐波指针	
	Rel59U0.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	Rel59U0.pRel47UCFG		= &RelCFGCLR;;
	Rel59U0.R47ULKRelayWord   = FALSE1;

	Rel59U0.StartRelayWord		= RW_PI59N;					//过量保护入段继电器字
	Rel59U0.ActRelayWord		= RW_TR59N;					//过量保护动作继电器字
	Rel59U0.AccRelayWord		= FALSE1;						//后加速允许继电器字
	Rel59U0.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	Rel59U0.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	Rel59U0.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	Rel59U0.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	Rel59U0.LockRelayWord		= RW_LKPI59N;					//入段闭锁继电器字
	Rel59U0.RSTRelayWord		= RW_LKRST59N;					//返回闭锁继电器字
	Rel59U0.LoopCtrlRelayWord	= RW_LPC59N;					//保护循环控制继电器字
	
	Rel59U0.wReportNum			= R_R59UN;						//过量保护报告序号
	Rel59U0.wAccReportNum		= 0xFFFF;						//加速动作报告序号
	Rel59U0.bMakeActRpt			= TRUE;
	Rel59U0.dwK35				= NULL;
	Rel59U0.RptSerialNum		= &m_wRptSerialNumOfAct;	
	//**********************入段返回报告部分*********************	yanxs  13-03-21
	Rel59U0.wPickUpReportNum	= R_R59UN_PI;						//入段报告序列号
	Rel59U0.wResetReportNum		= R_R59UN_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel59U0.dwRelTimer			= 0;			 			//定时器
	Rel59U0.byRelFCounter		= 0;						//故障计数器
	Rel59U0.byRelRCounter		= 0;						//返回计数器
	
	Rel59U0.wExcepWordNum     = 8;
}
/************************************************************************************************
*功能: 初始化低压保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRel27U()
{
	Rel27U.pELeSet						= RS_pn27HV;						//电量定值 
	Rel27U.pTimeSet						= RS_pn27HD;						//时间定值 
	Rel27U.pLockCurSet					= RS_pn27H_LK50;					//闭锁电流定值 

	Rel27U.pRelCFG						= CFG_pby27;					  	 //欠量保护配置 
	Rel27U.pRelSW						= SW_pby27;					  	 //欠量保护软压板 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//欠量保护投跳闸 
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//系统定时器指针
	Rel27U.pRelCurLockCFG				= &RelCFGSET;

	Rel27U.pRelElec						= &U_R27U;						//当前的电量指针 
	Rel27U.pRelLockElec					= &I_R27U;					//闭锁电流 
	Rel27U.ActRelayWord					= RW_TR27;					//欠量保护动作继电器字 
	Rel27U.StartRelayWord				= RW_PI27;					//欠量保护入段继电器字 
	Rel27U.AlarmRelayWord				= FALSE1;					//欠量保护告警继电器字 
	Rel27U.LockRelayWord				= RW_LKPI27;				//入段闭锁继电器字 
	Rel27U.RSTRelayWord					= RW_LKRST27;					//故障返回闭锁继电器字 
	Rel27U.LoopCtrlRelayWord			= FALSE1;				//故障循环控制继电器字 
	Rel27U.wR52BFRealyword				= RW_52BFH;
	Rel27U.wActRptNum					= R_R27U;					//欠量保护动作报告序号 
	Rel27U.wAlarmRptNum					= 0xFFFF;					//欠量保护告警报告序号 
	Rel27U.wTRFailRptNum				= R_TR_H_FAIL;
	Rel27U.wPickUpRptNum				= R_ACTPICK;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
	
	Rel27U.R27RptSerialNum				= 0;
//	Rel27U.dwRelTimer					= 0;						//欠量保护定时器 	
	Rel27U.byRelFCounter				= 0;					//故障计数器 
	Rel27U.byRelRCounter				= 0;					//返回计数器 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
/************************************************************************************************
*功能: 初始化高侧A反时限过负荷参数结构
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
void CRelayTask::InitRel49IHA(void)
{	
	Rel49I_IA.p49KSet				= RS_pn49K;
	Rel49I_IA.p49TSet				= RS_pn49T;
	Rel49I_IA.p49ARSet				= RS_pn49AR;
	Rel49I_IA.p49KTSet				= RS_pn49KT;
	Rel49I_IA.pRatedInSet			= RS_pnTIN;
	Rel49I_IA.pRel49CFG				= CFG_pby49;
	Rel49I_IA.pRel49SW				= SW_pby49;
	Rel49I_IA.pICoef				= CF_pnIHA;
	Rel49I_IA.pIHACoef				= CF_pnIHA;
	Rel49I_IA.pCurrent				= IAP;
	
	Rel49I_IA.dwKIn					= 0;	
	Rel49I_IA.PreCurrent			= 0;			
	Rel49I_IA.l49OT					= 0;
	Rel49I_IA.l49KOT				= 0;
	Rel49I_IA.lExpT					= 0;
	Rel49I_IA.lExpKT				= 0;

	Rel49I_IA.R49WarnRW				= RW_AR49A;
	Rel49I_IA.R49ActRW				= RW_TR49A;
	Rel49I_IA.R49ActRptNum			= R_R49ACT;
	Rel49I_IA.R49WarnRptNum			= R_R49AR;
	Rel49I_IA.R49RSTRptNum			= R_R49RST;
	
	Rel49I_IA.dw49Warn				= 0;
	Rel49I_IA.dw49Warn_099          = 0;
	Rel49I_IA.dw49AThermaVal		= 0;
	Rel49I_IA.dw49AThermaPreVal		= 0;	
	Rel49I_IA.pSysTimer				= &dwRelayMSCnt;						//系统定时器 
//	Rel49I_IA.dwRelTimer			= 0;								//欠量保护定时器 
	
	Rel49I_IA.wExcepWordNum			= 16;
	InitThermalOverLoad(&Rel49I_IA);
}

/************************************************************************************************
*功能: 初始化高侧A反时限过负荷参数结构
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
void CRelayTask::InitRel49IHB(void)
{	
	Rel49I_IB.p49KSet				= RS_pn49K;
	Rel49I_IB.p49TSet				= RS_pn49T;
	Rel49I_IB.p49ARSet				= RS_pn49AR;
	Rel49I_IB.p49KTSet				= RS_pn49KT;
	Rel49I_IB.pRatedInSet			= RS_pnTIN;
	Rel49I_IB.pRel49CFG				= CFG_pby49;
	Rel49I_IB.pRel49SW				= SW_pby49;
	Rel49I_IB.pICoef				= CF_pnIHB;
	Rel49I_IB.pIHACoef				= CF_pnIHA;
	Rel49I_IB.pCurrent				= IBP;
	
	
	Rel49I_IB.dwKIn					= 0;	
	Rel49I_IB.PreCurrent			= 0;			
	Rel49I_IB.l49OT					= 0;
	Rel49I_IB.l49KOT				= 0;
	Rel49I_IB.lExpT					= 0;
	Rel49I_IB.lExpKT				= 0;
	
	Rel49I_IB.R49WarnRW				= RW_AR49B;
	Rel49I_IB.R49ActRW				= RW_TR49B;
	Rel49I_IB.R49ActRptNum			= R_R49ACT;
	Rel49I_IB.R49WarnRptNum			= R_R49AR;
	Rel49I_IB.R49RSTRptNum			= R_R49RST;
	
	Rel49I_IB.dw49Warn				= 0;
	Rel49I_IB.dw49Warn_099          = 0;
	Rel49I_IB.dw49AThermaVal		= 0;
	Rel49I_IB.dw49AThermaPreVal		= 0;	
	Rel49I_IB.pSysTimer				= &dwRelayMSCnt;						//系统定时器 
//	Rel49I_IB.dwRelTimer			= 0;								//欠量保护定时器 
	Rel49I_IB.wExcepWordNum 		= 16;

	InitThermalOverLoad(&Rel49I_IB);
}
/************************************************************************************************
*功能: 初始化高侧A反时限过负荷参数结构
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
void CRelayTask::InitRel49IHC(void)
{	
	Rel49I_IC.p49KSet				= RS_pn49K;
	Rel49I_IC.p49TSet				= RS_pn49T;
	Rel49I_IC.p49ARSet				= RS_pn49AR;
	Rel49I_IC.p49KTSet				= RS_pn49KT;
	Rel49I_IC.pRatedInSet			= RS_pnTIN;
	Rel49I_IC.pRel49CFG				= CFG_pby49;
	Rel49I_IC.pRel49SW				= SW_pby49;
	Rel49I_IC.pICoef				= CF_pnIHC;
	Rel49I_IC.pIHACoef				= CF_pnIHA;
	Rel49I_IC.pCurrent				= ICP;
		
	Rel49I_IC.dwKIn					= 0;	
	Rel49I_IC.PreCurrent			= 0;			
	Rel49I_IC.l49OT					= 0;
	Rel49I_IC.l49KOT				= 0;
	Rel49I_IC.lExpT					= 0;
	Rel49I_IC.lExpKT				= 0;
	
	Rel49I_IC.R49WarnRW				= RW_AR49C;
	Rel49I_IC.R49ActRW				= RW_TR49C;
	Rel49I_IC.R49ActRptNum			= R_R49ACT;
	Rel49I_IC.R49WarnRptNum			= R_R49AR;
	Rel49I_IC.R49RSTRptNum			= R_R49RST;
	
	Rel49I_IC.dw49Warn				= 0;
	Rel49I_IC.dw49Warn_099          = 0;
	Rel49I_IC.dw49AThermaVal		= 0;
	Rel49I_IC.dw49AThermaPreVal		= 0;	
	Rel49I_IC.pSysTimer				= &dwRelayMSCnt;						//系统定时器 
//	Rel49I_IC.dwRelTimer			= 0;		
	Rel49I_IC.wExcepWordNum 		= 16;

	InitThermalOverLoad(&Rel49I_IC);
}
/************************************************************************************************
*功能: 初始化PT断线参数结构
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
void CRelayTask::InitRel74PT(void)
{
//--------------高侧PT断线参数结构---------------------
	Rel74PTH.pRelPTBrokCFG = CFG_pby74PT;						//PT断线配置 
	Rel74PTH.pR74PTU1Set = RS_pn74HPTV;						//PT断线检测电压定值
	Rel74PTH.pR74PTU2Set = RS_pn74LPTV;						//PT断线检测电压定值
	Rel74PTH.pR74PTISet = RS_pn74PTHC;						//pt断线检测电流定值	
	Rel74PTH.pR74PTTSet = RS_pn74PTD;						//pt断线检测时间定值
	
	Rel74PTH.pRelCur = &I_R27U;							//判断用电流 
	Rel74PTH.pRelU1	= &PhaseUHMin;							//判断用第一路电压 	
	Rel74PTH.pRelU2 = &PhaseULMax;	
	

	Rel74PTH.StartRelayWord = RW_PI74PTH;						//PT断线入段继电器字 
	Rel74PTH.ActRelayWord	= RW_AR74PTH;						//PT断线动作继电器字 
	Rel74PTH.LKPIRelayWord	= RW_LKPIPTH;
	Rel74PTH.dwSysTimer		= &dwRelayMSCnt;						//系统定时器 
	Rel74PTH.wRelRptNum		= R_RHPTBR;						//PT断线动作报告号
	Rel74PTH.wRelResRptNum	= R_RHPTBRRST;					//PT断线返回报告号
//	Rel74PTH.dwRelTimer		= 0;							//PT断线定时器 	
	Rel74PTH.wExcepWordNum  = 5;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为6 

//--------------低侧A相PT断线参数结构---------------------
	Rel74PTLA.pRelPTBrokCFG = CFG_pby74PT;						//PT断线配置 
	Rel74PTLA.pR74PTU1Set = RS_pn74LPTV;						//PT断线检测电压定值
	Rel74PTLA.pR74PTU2Set = RS_pn74HPTV;						//PT断线检测电压定值
	Rel74PTLA.pR74PTISet = RS_pn74PTLC;						//pt断线检测电流定值	
	Rel74PTLA.pR74PTTSet = RS_pn74PTD;						//pt断线检测时间定值

	Rel74PTLA.pRelCur = &ILA;							//判断用电流 
	Rel74PTLA.pRelU1 = &ULA;							//判断用第一路电压	
	Rel74PTLA.pRelU2 = &LineUHMax;	

	Rel74PTLA.StartRelayWord = RW_PI74PTLA;						//PT断线入段继电器字 
	Rel74PTLA.ActRelayWord	= RW_AR74PTLA;						//PT断线动作继电器字	
	Rel74PTLA.LKPIRelayWord = RW_LKPIPTLA;
	Rel74PTLA.dwSysTimer 	= &dwRelayMSCnt;						//系统定时器 
	Rel74PTLA.wRelRptNum 	= R_RLAPTBR; 					//PT断线动作报告号
	Rel74PTLA.wRelResRptNum	= R_RLAPTBRRST;					//PT断线返回报告号
//	Rel74PTLA.dwRelTimer 	= 0;							//PT断线定时器	
	Rel74PTLA.wExcepWordNum	= 5;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为6 

//--------------低侧B相PT断线参数结构---------------------
	Rel74PTLB.pRelPTBrokCFG = CFG_pby74PT; 					//PT断线配置 
	Rel74PTLB.pR74PTU1Set = RS_pn74LPTV; 					//PT断线检测电压定值
	Rel74PTLB.pR74PTU2Set = RS_pn74HPTV; 					//PT断线检测电压定值
	Rel74PTLB.pR74PTISet = RS_pn74PTLC; 					//pt断线检测电流定值	
	Rel74PTLB.pR74PTTSet = RS_pn74PTD;						//pt断线检测时间定值

	Rel74PTLB.pRelCur = &ILB;							//判断用电流 
	Rel74PTLB.pRelU1 = &ULB; 						//判断用第一路电压	
	Rel74PTLB.pRelU2 = &LineUHMax;	

	Rel74PTLB.StartRelayWord = RW_PI74PTLB; 					//PT断线入段继电器字 
	Rel74PTLB.ActRelayWord	= RW_AR74PTLB; 					//PT断线动作继电器字	
	Rel74PTLB.LKPIRelayWord = RW_LKPIPTLB;
	Rel74PTLB.dwSysTimer	= &dwRelayMSCnt;						//系统定时器 
	Rel74PTLB.wRelRptNum	= R_RLBPTBR;					//PT断线动作报告号
	Rel74PTLB.wRelResRptNum	= R_RLBPTBRRST;					//PT断线返回报告号
//	Rel74PTLB.dwRelTimer	= 0;							//PT断线定时器	
	Rel74PTLB.wExcepWordNum = 5;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为6 
}

void CRelayTask::InitStructCheck()
{
	WORD acc = 0x55aa;
	
	StructCheck[0].pstruct = (WORD*)&Rel50I_IA;
	StructCheck[0].bigsize = sizeof(Rel50I_IA)/2 - Rel50I_IA.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&Rel50I_IB;
	StructCheck[1].bigsize = sizeof(Rel50I_IB)/2 - Rel50I_IB.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel50I_IC;
	StructCheck[2].bigsize = sizeof(Rel50I_IC)/2 - Rel50I_IC.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel50I_Ia1;
	StructCheck[3].bigsize = sizeof(Rel50I_Ia1)/2 - Rel50I_Ia1.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel50I_Ia2;
	StructCheck[4].bigsize = sizeof(Rel50I_Ia2)/2 - Rel50I_Ia2.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel50I_Ib1;
	StructCheck[5].bigsize = sizeof(Rel50I_Ib1)/2 - Rel50I_Ib1.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel50I_Ib2;
	StructCheck[6].bigsize = sizeof(Rel50I_Ib2)/2 - Rel50I_Ib2.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel50I_JX;
	StructCheck[7].bigsize = sizeof(Rel50I_JX)/2 - Rel50I_JX.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&Rel49I_IA;
	StructCheck[8].bigsize = sizeof(Rel49I_IA)/2 - Rel49I_IA.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&Rel49I_IB;
	StructCheck[9].bigsize = sizeof(Rel49I_IB)/2 - Rel49I_IB.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&Rel49I_IC;
	StructCheck[10].bigsize = sizeof(Rel49I_IC)/2 - Rel49I_IC.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)&Rel50IA1;
	StructCheck[11].bigsize = sizeof(Rel50IA1)/2 - Rel50IA1.wExcepWordNum;
	StructCheck[12].pstruct = (WORD*)&Rel50IA2;
	StructCheck[12].bigsize = sizeof(Rel50IA2)/2 - Rel50IA2.wExcepWordNum;
	StructCheck[13].pstruct = (WORD*)&Rel50IB1;
	StructCheck[13].bigsize = sizeof(Rel50IB1)/2 - Rel50IB1.wExcepWordNum;
	StructCheck[14].pstruct = (WORD*)&Rel50IB2;
	StructCheck[14].bigsize = sizeof(Rel50IB2)/2 - Rel50IB2.wExcepWordNum;
	StructCheck[15].pstruct = (WORD*)&Rel50IC1;
	StructCheck[15].bigsize = sizeof(Rel50IC1)/2 - Rel50IC1.wExcepWordNum;
	StructCheck[16].pstruct = (WORD*)&Rel50IC2;
	StructCheck[16].bigsize = sizeof(Rel50IC2)/2 - Rel50IC2.wExcepWordNum;
	StructCheck[17].pstruct = (WORD*)&Rel50I0;
	StructCheck[17].bigsize = sizeof(Rel50I0)/2 - Rel50I0.wExcepWordNum;
	StructCheck[18].pstruct = (WORD*)&Rel59U0;
	StructCheck[18].bigsize = sizeof(Rel59U0)/2 - Rel59U0.wExcepWordNum;
	StructCheck[19].pstruct = (WORD*)&Rel27U;
	StructCheck[19].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;
	StructCheck[20].pstruct = (WORD*)&Rel74PTH;
	StructCheck[20].bigsize = sizeof(Rel74PTH)/2 - Rel74PTH.wExcepWordNum;
	StructCheck[21].pstruct = (WORD*)&Rel74PTLA;
	StructCheck[21].bigsize = sizeof(Rel74PTLA)/2 - Rel74PTLA.wExcepWordNum;
	StructCheck[22].pstruct = (WORD*)&Rel74PTLB;
	StructCheck[22].bigsize = sizeof(Rel74PTLB)/2 - Rel74PTLB.wExcepWordNum;
	StructCheck[23].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[23].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
	for(LONG j=0;j<CHECK_NUM;j++)
	{		
		for(LONG i=0;i<StructCheck[j].bigsize;i++)
		{
			acc += StructCheck[j].pstruct[i];
		}
		StructCheck[j].acc = acc;
		acc = 0x55aa;
	}
	RelayCheckInitOK = TRUE;
}

//------------------------------------------------------------------------------------------ 
// 名称: OnRelayChange	 
// 功能: 相应定值修改消息函数 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::OnRelayChange(DWORD dwDataType,DWORD dwIndex)
{//	ReportMsg("Relay Task Recieved SETTING change message! Type=%d Index=%d",dwDataType,dwIndex);
	//重新初始化保护数据
	::InitAllRelayMem();
	InitRealy();
	InitSet();	
	InitDCRecInfo();
	InitStructCheck();
//	RelayCheck();
	UpDateCFG();
	UpDateSW();
}
void CRelayTask::OnLogicChanged(void)
{
	m_bLogicChanged = TRUE;
	m_dwWaitLogicCounter = dwRelayMSCnt;

	m_bIsLogicOk &= ResetLogicParseModule(TRUE);
}

//------------------------------------------------------------------------------------------ 
// 功能: 模值启动判别程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	if(ReadRelayWord(RW_52BFH))
		return FALSE;
	
	if((*Rel50I_IA.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IA.pRelSW == SW_STATUS_SET))   //高侧A过流
	{
		if(Rel50I_IA.pRelElec->Mod > *Rel50I_IA.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}	
	if((*Rel50I_IB.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IB.pRelSW == SW_STATUS_SET))   //高侧B过流
	{
		if(Rel50I_IB.pRelElec->Mod > *Rel50I_IB.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	
	if((*Rel50I_IC.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IC.pRelSW == SW_STATUS_SET))   //高侧C过流
	{
		if(Rel50I_IC.pRelElec->Mod > *Rel50I_IC.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	
	if((*Rel50I_JX.pRelCFG == CFG_STATUS_SET) && (*Rel50I_JX.pRelSW == SW_STATUS_SET))   //间隙过流
	{
		if(Rel50I_JX.pRelElec1->Mod > *Rel50I_JX.pElec1Set)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50IA2.pRelCFG == CFG_STATUS_SET) && (*Rel50IA2.pRelSW == SW_STATUS_SET))   //高侧A过负荷
	{
		if(Rel50IA2.pRelElec->Mod > *Rel50IA2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50IB2.pRelCFG == CFG_STATUS_SET) && (*Rel50IB2.pRelSW == SW_STATUS_SET))   //高侧B过负荷
	{
		if(Rel50IB2.pRelElec->Mod > *Rel50IB2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50IC2.pRelCFG == CFG_STATUS_SET) && (*Rel50IC2.pRelSW == SW_STATUS_SET))   //高侧C过负荷
	{
		if(Rel50IC2.pRelElec->Mod > *Rel50IC2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET))   //零流
	{
		if(Rel50I0.pRelElec->Mod > *Rel50I0.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if((*Rel59U0.pRelCFG == CFG_STATUS_SET) && (*Rel59U0.pRelSW == SW_STATUS_SET))   //零压
	{
		if(Rel59U0.pRelElec->Mod > *Rel59U0.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			m_wRptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			m_wRptSerialNumOfStart = g_RptSerialNum;
			
			return TRUE;
		}
	}
	if(!ReadRelayWord(RW_52BFLA))
	{
		if((*Rel50I_Ia1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ia1.pRelSW == SW_STATUS_SET))   //低侧a过流
		{
			if(Rel50I_Ia1.pRelElec->Mod > *Rel50I_Ia1.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
		
		if((*Rel50I_Ia2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ia2.pRelSW == SW_STATUS_SET))   //低侧a过流
		{
			if(Rel50I_Ia2.pRelElec->Mod > *Rel50I_Ia2.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
	}
	
	if(!ReadRelayWord(RW_52BFLB))
	{
		if((*Rel50I_Ib1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ib1.pRelSW == SW_STATUS_SET))   //低侧b过流
		{
			if(Rel50I_Ib1.pRelElec->Mod > *Rel50I_Ib1.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
		
		if((*Rel50I_Ib2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_Ib2.pRelSW == SW_STATUS_SET))   //低侧b过流
		{
			if(Rel50I_Ib2.pRelElec->Mod > *Rel50I_Ib2.pElecSet)
			{
				WriteRelayWord(RW_PICKUP, RELAY_ON);
				ReadAbsTime(&PickUpTime);
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
				m_wRptSerialNumOfAct = g_RptSerialNum;
				DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
				m_wRptSerialNumOfStart = g_RptSerialNum;
				
				return TRUE;
			}
		}
	}
	
	if(((*Rel50I_IA.pRelCFG == CFG_STATUS_SET) && (*Rel50I_IA.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_IB.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_IB.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_IC.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_IC.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_Ia1.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ia1.pRelSW == SW_STATUS_SET))||
		((*Rel50I_Ib1.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ib1.pRelSW == SW_STATUS_SET))||
		((*Rel50I_Ia2.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ia2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_Ib2.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_Ib2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I_JX.pRelCFG == CFG_STATUS_SET)&& (*Rel50I_JX.pRelSW == SW_STATUS_SET))|| \
		((*Rel50IA2.pRelCFG == CFG_STATUS_SET)&& (*Rel50IA2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50IB2.pRelCFG == CFG_STATUS_SET)&& (*Rel50IB2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50IC2.pRelCFG == CFG_STATUS_SET)&& (*Rel50IC2.pRelSW == SW_STATUS_SET))|| \
		((*Rel50I0.pRelCFG == CFG_STATUS_SET)&& (*Rel50I0.pRelSW == SW_STATUS_SET))|| \
		((*Rel59U0.pRelCFG == CFG_STATUS_SET)&& (*Rel59U0.pRelSW == SW_STATUS_SET)))
	
	{
		BYTE  pChannel[]={CHANNEL_6531_IHA,CHANNEL_6531_IHB,CHANNEL_6531_IHC,CHANNEL_6531_I0,CHANNEL_6531_Ijx,CHANNEL_6531_ILA,CHANNEL_6531_ILB};
		ReadPreSingleData(dwPickDataBuf,pChannel,7,RELAY_SAMP_INTERVAL);		
		//算法1，根据宏RELAY_SAMP_INTERVAL确定的通用算法
		//将3个周波的采样点分别取至相应的数组,并计算突变量,且进行判断
		//注意:RELAY_SAMP_INTERVAL的值不能小于3
		for(LONG j=0;j<7;j++)
		{
			m_wPickCheck = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickDataBuf[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));
				
				//采用浮动门槛值判断，ΔI>0.25*ΔI'+ C,其中ΔI'为上个一个邻近的点的突变量值，C为突变量启动定值
				if(lPickValue*4>*RS_pnPIC*4+m_lPickUpValueArray[j])
				{
					m_wPickCheck++;
					if(m_wPickCheck ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime.Lo-(i*625+500)/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_PICKUP, RELAY_ON);
						//ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
						m_wRptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
						m_wRptSerialNumOfStart = g_RptSerialNum;
						m_wPickCheck =0;
						for(LONG m=0;m<7;m++)
						{
							m_wPickCheckNext[m] =0;
							m_lPickUpValueArray[m]=0;
						}
						
						return TRUE;	
					}
					if(i<2)
					{
						m_wPickCheckNext[j]++;
						if(m_wPickCheckNext[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime);
							PickTime = PickUpTime.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime.Lo-(i*625+500)/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_PICKUP, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
							m_wRptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
							m_wRptSerialNumOfStart = g_RptSerialNum;
							m_wPickCheck =0;
							for(LONG m=0;m<7;m++)
							{
								m_wPickCheckNext[m] =0;
								m_lPickUpValueArray[m]=0;
							}
							
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						m_wPickCheckNext[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						m_wPickCheckNext[j]++;
				}
				else
				{
					m_wPickCheck =0;
					m_wPickCheckNext[j]=0;
				}
				m_lPickUpValueArray[j] = lPickValue;
			}
		}
	}

	WriteRelayWord(RW_PICKUP, RELAY_OFF);
	
	return FALSE;
}
//------------------------------------------------------------------------------------------ 
// 功能: 保护循环控制程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
void CRelayTask::FalutProcess(void)
{ 	
	if(m_bFaultOnFlag ==FALSE)   						//尚未进入故障处理
	{		
		RelayPickUpCHK();					//执行启动检测
		if(ReadRelayWord(RW_PICKUP))
		{
			ResetAllRelay(TRUE);
			m_dwFaultSINum=RELAY_SAMP_INTERVAL;       //保护启动，初始化故障处理采样间隔计数器
			m_bFaultOnFlag=TRUE;
			return;
		}
		else
			return;		
	}
	m_dwFaultSINum += RELAY_SAMP_INTERVAL;				//故障处理采样间隔计数器累加
	if(m_dwFaultSINum<READ_POINT_NUM)					//故障处理采样间隔计数器小于一周波采样点数
	{
		return;
	}	
	else
	{
		if(m_bHaveMakeStartRptFlag == FALSE)   //向带故障电量的启动报告中写入故障电量
		{
			MakeStartRpt(R_PICKUP,m_wRptSerialNumOfStart);
			m_bHaveMakeStartRptFlag = TRUE;
		}
				
		GeneralOverValueRelay(&Rel50I_IA);				//调用速断子程序A
		GeneralOverValueRelay(&Rel50I_IB);				//调用速断子程序B
		GeneralOverValueRelay(&Rel50I_IC);				//调用速断子程序C
		if(!ReadRelayWord(RW_52BFLA))
		{
			GeneralOverValueRelay(&Rel50I_Ia1);				//调用速断子程序		
			GeneralOverValueRelay(&Rel50I_Ia2);				//调用速断子程序
		}
		if(!ReadRelayWord(RW_52BFLB))
		{
			GeneralOverValueRelay(&Rel50I_Ib1);				//调用速断子程序		
			GeneralOverValueRelay(&Rel50I_Ib2);				//调用速断子程序
		}
		GeneralOverValueRelay_3(&Rel50I_JX);				//调用速断子程序
		GeneralOverValueRelay(&Rel50IA2);				//调用速断子程序
		GeneralOverValueRelay(&Rel50IB2);				//调用速断子程序
		GeneralOverValueRelay(&Rel50IC2);				//调用速断子程序
		GeneralOverValueRelay(&Rel50I0);				//调用速断子程序
		GeneralOverValueRelay(&Rel59U0);				//调用速断子程序

		if(ReadRelayWord(RW_52BFH))
		{
			if((!m_bHaveMake52BF_H_RptFlag)&&(!m_b52BFRecover_H_FLAG))
			{
				m_b52BFRecover_H_FLAG = TRUE;
				MakeActRpt(R_TR_H_FAIL,m_wRptSerialNumOfAct);		//作断路器跳闸失败报告
				m_bHaveMake52BF_H_RptFlag = TRUE;
			}
		}
		if(ReadRelayWord(RW_52BFLA))
		{
			if((!m_bHaveMake52BF_LA_RptFlag)&&(!m_b52BFRecover_LA_FLAG))
			{
				m_b52BFRecover_LA_FLAG = TRUE;
				MakeActRpt(R_TR_LA_FAIL,m_wRptSerialNumOfAct);		//作断路器跳闸失败报告
				m_bHaveMake52BF_LA_RptFlag = TRUE;
			}
		}
		if(ReadRelayWord(RW_52BFLB))
		{
			if((!m_bHaveMake52BF_LB_RptFlag)&&(!m_b52BFRecover_LB_FLAG))
			{
				m_b52BFRecover_LB_FLAG = TRUE;
				MakeActRpt(R_TR_LB_FAIL,m_wRptSerialNumOfAct);		//作断路器跳闸失败报告
				m_bHaveMake52BF_LB_RptFlag = TRUE;
			}
		}
		
		// 1。有保护循环控制继电器字被置位，等待保护动作或者退出
		if(ReadRelayWord(Rel50I_IA.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IB.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IC.LoopCtrlRelayWord) \
			||((!ReadRelayWord(RW_52BFLA))&&(ReadRelayWord(Rel50I_Ia1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ia2.LoopCtrlRelayWord)))
			||((!ReadRelayWord(RW_52BFLB))&&(ReadRelayWord(Rel50I_Ib1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ib2.LoopCtrlRelayWord))) \
			||ReadRelayWord(Rel50I_JX.LoopCtrlRelayWord)||ReadRelayWord(Rel50IA2.LoopCtrlRelayWord)||ReadRelayWord(Rel59U0.LoopCtrlRelayWord)  \
			||ReadRelayWord(Rel50IB2.LoopCtrlRelayWord)||ReadRelayWord(Rel50IC2.LoopCtrlRelayWord)||ReadRelayWord(Rel50I0.LoopCtrlRelayWord)) \
		{
			if(ReadRelayWord(RW_52BFH))
			{				
				WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
				m_bFaultCheckOutFlag = FALSE;					//清检出故障标志
				m_bFaultOnFlag = FALSE;						//清故障循环处理标志
				m_bHaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
				m_bFaultLKFlag = FALSE; 					//清有启动无动作时启动时刻锁定标志	
				m_bHaveMake52BF_H_RptFlag = FALSE;
				m_bHaveMake52BF_LA_RptFlag = FALSE;
				m_bHaveMake52BF_LB_RptFlag = FALSE;
				DB_RelayEnd(m_wRptSerialNumOfAct); 			//向数据库中写入结束报告
				return; 				
			}	
			//低侧断路器跳闸失败
			if(ReadRelayWord(RW_52BFLA)||ReadRelayWord(RW_52BFLB))  
			{
				//有高压侧的保护入段
				if(ReadRelayWord(Rel50I_IA.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IB.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_IC.LoopCtrlRelayWord) \
					||ReadRelayWord(Rel50I_JX.LoopCtrlRelayWord)||ReadRelayWord(Rel50IA2.LoopCtrlRelayWord)||ReadRelayWord(Rel59U0.LoopCtrlRelayWord)  \
					||ReadRelayWord(Rel50IB2.LoopCtrlRelayWord)||ReadRelayWord(Rel50IC2.LoopCtrlRelayWord)||ReadRelayWord(Rel50I0.LoopCtrlRelayWord) \
					||((!ReadRelayWord(RW_52BFLA))&&(ReadRelayWord(Rel50I_Ia1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ia2.LoopCtrlRelayWord))) \
					||((!ReadRelayWord(RW_52BFLB))&&(ReadRelayWord(Rel50I_Ib1.LoopCtrlRelayWord)||ReadRelayWord(Rel50I_Ib2.LoopCtrlRelayWord))))
				{					
					m_dwFaultLockTime = dwRelayMSCnt;		
					m_bFaultCheckOutFlag = TRUE;
					return;
				}
				else
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					m_bFaultCheckOutFlag = FALSE;					//清检出故障标志
					m_bFaultOnFlag = FALSE; 					//清故障循环处理标志
					m_bHaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
					m_bFaultLKFlag = FALSE; 					//清有启动无动作时启动时刻锁定标志						
					m_bHaveMake52BF_H_RptFlag = FALSE;
					m_bHaveMake52BF_LA_RptFlag = FALSE;
					m_bHaveMake52BF_LB_RptFlag = FALSE;
					DB_RelayEnd(m_wRptSerialNumOfAct);			//向数据库中写入结束报告
					return; 				
				}				
			}
			m_dwFaultLockTime = dwRelayMSCnt;		
			m_bFaultCheckOutFlag = TRUE;
			return;
		}
		// 2。无保护循环控制继电器字被置位，此时保护已动作结束或没有满足任何保护动作条件
		else				
		{
			// 2.1 检出故障标志被置位，表示保护循环控制继电器字曾经被置位--不一定动作过--等待RS_pnSOTFT时间后退出
			if(m_bFaultCheckOutFlag == TRUE)     
			{
				if((ReadRelayWord(RW_T52BFH))||(ReadRelayWord(RW_T52BFLA))||(ReadRelayWord(RW_T52BFLB)))
				{
					m_dwFaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -m_dwFaultLockTime > 1000 )  //无故障时间超过合于故障检测时间
				{						
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					m_bFaultCheckOutFlag = FALSE;					//清检出故障标志
					m_bFaultOnFlag = FALSE;						//清故障循环处理标志
					m_bHaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					DB_RelayEnd(m_wRptSerialNumOfAct);				//向数据库中写入结束报告
					ResetAllRelay(TRUE);
					m_bHaveMake52BF_H_RptFlag = FALSE;
					m_bHaveMake52BF_LA_RptFlag = FALSE;
					m_bHaveMake52BF_LB_RptFlag = FALSE;
					return;					
				}
				else
					return;
			}
			// 2.2 检出故障标志未被置位，表示保护循环控制继电器字在本次故障处理中从未被置位，等待0.5秒后退出
			else
			{
				
				if(m_bFaultLKFlag == FALSE)//保护刚刚启动，记录起始时刻
				{	
					m_dwFaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				if(dwRelayMSCnt -m_dwFaultLockTime>500 )	
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					m_bFaultCheckOutFlag = FALSE;					//清检出故障标志
					m_bFaultOnFlag = FALSE;						//清故障循环处理标志
					m_bHaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志						
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					DB_RelayEnd(m_wRptSerialNumOfAct); 
					ResetAllRelay(TRUE);
					m_bHaveMake52BF_H_RptFlag = FALSE;
					m_bHaveMake52BF_LA_RptFlag = FALSE;
					m_bHaveMake52BF_LB_RptFlag = FALSE;
					return;					
				}
				else	//延时未到，直接返回
					return;
			}			
		}		
	}
}
//------------------------------------------------------------------------------------------
// 功能: 断路器跳闸失败复归处理						                              
// 输入: 无                                                                                
// 输出:                                                         
// 返回: 无                                                                                
//------------------------------------------------------------------------------------------
void CRelayTask::Process52BF()
{
	if(ReadRelayWord(RW_52BFH))
	{
		m_b52BFHFlag = TRUE;
	}
	if(m_b52BFHFlag)
	{
		if(!ReadRelayWord(RW_52BFH))
		{			
			m_b52BFRecover_H_FLAG = FALSE;
			m_b52BFHFlag = FALSE;
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&Rel27U);			
		}
	}

	if(ReadRelayWord(RW_52BFLA))
	{
		m_b52BFLAFlag = TRUE;
	}
	if(m_b52BFLAFlag)
	{
		if(!ReadRelayWord(RW_52BFLA))
		{			
			m_b52BFRecover_LA_FLAG = FALSE;
			m_b52BFLAFlag = FALSE;
			ResetOverValueRelay(&Rel50I_Ia1,TRUE);
			ResetOverValueRelay(&Rel50I_Ia2,TRUE);
		}
	}

	if(ReadRelayWord(RW_52BFLB))
	{
		m_b52BFLBFlag = TRUE;
	}
	if(m_b52BFLBFlag)
	{
		if(!ReadRelayWord(RW_52BFLB))
		{
			m_b52BFRecover_LB_FLAG = FALSE;
			m_b52BFLBFlag = FALSE;
			ResetOverValueRelay(&Rel50I_Ib1,TRUE);
			ResetOverValueRelay(&Rel50I_Ib2,TRUE);
		}
	}
	
}

//------------------------------------------------------------------------------------------
//功能: 保护元件整组复归函数							                              
// 输入: 无                                                                                
// 输出:                                                         
// 返回: 无                                                                                
//------------------------------------------------------------------------------------------

void CRelayTask::ResetAllRelay(BOOL resetFlag)
{			
	ResetOverValueRelay(&Rel50I_IA,resetFlag);
	ResetOverValueRelay(&Rel50I_IB,resetFlag);
	ResetOverValueRelay(&Rel50I_IC,resetFlag);
	if(!ReadRelayWord(RW_52BFLA))
	{
		ResetOverValueRelay(&Rel50I_Ia1,resetFlag);
		ResetOverValueRelay(&Rel50I_Ia2,resetFlag);
	}
	if(!ReadRelayWord(RW_52BFLB))
	{
		ResetOverValueRelay(&Rel50I_Ib1,resetFlag);
		ResetOverValueRelay(&Rel50I_Ib2,resetFlag);
	}
	ResetOverValueRelay_3(&Rel50I_JX,resetFlag);
	ResetOverValueRelay(&Rel50IA2,resetFlag);
	ResetOverValueRelay(&Rel50IB2,resetFlag);
	ResetOverValueRelay(&Rel50IC2,resetFlag);
	ResetOverValueRelay(&Rel50I0,resetFlag);
	ResetOverValueRelay(&Rel59U0,resetFlag);
}
void CRelayTask::PTCheck()
{	
	CheckPTBrok_2U(&Rel74PTH);
	CheckPTBrok_2U(&Rel74PTLA);
	CheckPTBrok_2U(&Rel74PTLB);
}
TRelElecValPar& CRelayTask::MaxElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C)
{
	if(A.Mod>=B.Mod)
	{
		if(A.Mod>=C.Mod)
			return A;
		else
			return C;
	}
	else
	{
		if(B.Mod>=C.Mod)
			return B;
		else
			return C;
	}
}
TRelElecValPar& CRelayTask::MinElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C)
{
	if(A.Mod>=B.Mod)
	{
		if(B.Mod>=C.Mod)
			return C;
		else
			return B;
	}
	else
	{
		if(A.Mod>=C.Mod)
			return C;
		else
			return A;
	}
}

