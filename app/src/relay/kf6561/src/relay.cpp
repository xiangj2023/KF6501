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
* 描述      KF6561电容器保护测控装置                                                                       
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
*      yanzh             12/07/08    初始创建                         
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
#include "bo.h"
#include "daefunc.h"

extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;			//AD当前写指针
extern TSysConfigTable	*G_pCfgTable;
extern BYTE 	g_RptSerialNum;			//报告序列号 
extern BOOL		RelayCheckErr;			//保护元件自检出错标志
extern BOOL 	RelayCheckInitOK;		//保护元件自检初始化标志
extern BYTE		*g_MaintSW;

BOOL g_FaultOnFlag;			////进入故障处理标志 CT断线检测用
DWORD		dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD		dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
DWORD 		dwRelayTaskID;
BOOL 		RelayTaskInitOK = FALSE;
TStructCheck StructCheck[CHECK_NUM];
extern DWORD dwADResumeCnt;

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
	DWORD status = ERR_MESSAGE_OK;
	OS_TaskDesp *pTask;

	OS_Sleep(1500);
	CRelayTask *pRelayTask = new CRelayTask(pdwApp);
	pRelayTask->Init();
	
	pTask = OS_GetTaskTab(dwRelayTaskID);

    //循环调用进行消息调度,直到收到任务退出消息
    while(status != ERR_MESSAGE_QUIT)
    {
		//清除任务运行计数
		if(pTask != NULL)
			pTask->Counter = 0;

		status = OS_GetMessage(dwRelayTaskID, &msg);
		if(status == ERR_MESSAGE_OK)
		{
			switch(LOWORD(msg.lParam))
			{
				case SM_TIMEOUT:
					pRelayTask->OnTimeOut(0);
					break;
				case SM_RELAYDATA:	//保护数据修改
					pRelayTask->OnRelayChange(0, 0);
					break;
				case SM_RELAY_INT:
					pRelayTask->OnSampleEnd(0,0);
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


	
/*	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	//获取数据库保护测量值数据指针，以加快操作速度
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	//初始化保护内部变量
	InitAllRelayMem();	
	//初始化保护元件
	InitRealy();
	//初始化数字录波参数	
	for(LONG i=0;i<97;i++)
	{
		DCMacroArray[i]=0;
	}
	InitDCRecInfo();	
	InitSet();
	
	RelCFGCLR = CFG_STATUS_CLR;			//保护退出
	RelCFGSET = CFG_STATUS_SET;			//保护投入
	
	g_RptSerialNum = 0;				//系统的报告序列号
	RptSerialNumOfAct =0;			//系统的保护动作报告序列号
	RptSerialNumOfStart=0;			//系统的带故障电量的启动报告序列号	
	FaultSINum = 0;					//采样间隔计数器	
	
	TripFailFlag = FALSE;			//跳闸失败标志
	FaultOnFlag = FALSE;			//进入故障处理标志
	g_FaultOnFlag = FALSE;			//进入故障处理标志 CT断线检测用
	HaveMakeStartRptFlag = FALSE;	//已作带故障电量启动报告标志
	m_bFaultLKFlag = FALSE;			//故障起始时刻锁定标志
	FaultCheckOutFlag = FALSE;		//检出故障标志
	m_bLastRecStatusFlag = FALSE;	//故障录波启动上升沿判断标志
	m_b52BFFLAG = FALSE;
	m_bLogicChanged = FALSE;
	m_bHaveMakeTripFailRpt = FALSE;
	PickCheck = 0;					//突变量启动判断连续点计数器
	for(WORD i=0;i<5;i++)
		PickCheckNext[i] = 0;				//突变量启动判断两次任务间连续点计数器	
	lSampData1 = 0;
	lSampData2 = 0;
	lSampDate3 = 0;
	lPickValue = 0;
	m_lPickUpValue = 0;
	
	//数字录波继电器字值合成字数组初始化
	for(LONG i=0;i<6;i++)
	{
		DCCaluResult[i]=0;
	}
	//保护数据读取通道掩码
	dwChanDataMask = ( (0x01<<CHANNEL_6561_UA)|(0x01<<CHANNEL_6561_UB)|(0x01<<CHANNEL_6561_UC)|\
						(0x01<<CHANNEL_6561_U0)|(0x01<<CHANNEL_6561_U1)|(0x01<<CHANNEL_6561_U2)|\
						(0x01<<CHANNEL_6561_IA)|(0x01<<CHANNEL_6561_IB)|(0x01<<CHANNEL_6561_IC)|\
						(0x01<<CHANNEL_6561_I0)|(0x01<<CHANNEL_6561_I1)|(0x01<<CHANNEL_6561_U3));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	
	
	//初始化读取保护数据临时缓冲区
	//保护数据缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);	
	//合成通道数据缓冲区
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	//突变量启动历史数据缓冲区
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*5);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*12));


	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*2);

	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	
	//下面是初始化逻辑方程部分
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	
	g_MaintSW = SW_pbyMAINT;

	//初始化开出自检为正常状态
	
	for(LONG i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();

	RelayTaskInitOK = TRUE;
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
	Factor1.Real = -1000/2;
	Factor1.Imag = 1732/2;
	Factor1.Mod = 1000;
	
	Factor2.Real = -1000/2;
	Factor2.Imag = -1732/2;
	Factor2.Mod = 1000;

	m_U2Set = *CF_pnUp*8; //8V负序电压
	
}
/************************************************************************************************
*功能: 初始化保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	InitR50I0();
	InitR50I1();
	InitR60I();
	InitR60U();
	InitR59U();
	InitR27U();		
	InitStructCheck();
	
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

//保护采样结束消息处理
void CRelayTask::OnSampleEnd(DWORD ptr, DWORD arg)
{	
	static WORD wInvalidCheckCnt;
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
	g_FaultOnFlag = FaultOnFlag;
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
		{
			RelayCheckErr = TRUE;
			return;
		}
//	if(RelayCheckErr)
//	{
//	//	ReportMsg("Relay Check Serious Error,and Relay Task Locked!");
//		LedAllOn();
//		return ;
//	}
//	MCF_GPIO_PODR_TIMER |= 0x02;		//高电平

	//首先进行开出自检
	if(!m_bLogicChanged)
		BoBreakCheck();

	//读取采样数据
	ReadSampleData(dwSampBuf,dwChanDataMask,0);	
	//计算电量It/If/U1/U2
	CaluUI(); 
	
	//合成Up,Ip通道并回写
	WriteComposeBuf();
	//计算Ip/Up以及谐波分量
	CaluUpIp();
	//计算负序电压，并处理复压闭锁继电器字
	CaluU2();
	//计算各电量的角度
	CaluUIAngle();

	//进入故障处理之后不进行PT断线判断
	if(!FaultOnFlag)
	{
		PTCheck();							
	}
	FalutProcess();								//故障循环处理
	GeneralUnderValRelay(&R27U);				//低电压保护子程序
	R59NAlarm();
	Process52BF();
	LogicProcess();								//逻辑方程处理
	//判断是否进行录波
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
	
	//写数字录波数据
	WriteDCChannel();
	

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
//	MCF_GPIO_PODR_TIMER &= ~0x02;		//低电平
}

//------------------------------------------------------------------------------------------ 
// 名称: 写合成采样数据缓冲区	                                                           							          
// 输入:  pBuf1,原始物理采样点数据缓冲区；pBuf2,合成采样点数据缓冲区；dwMask,通道掩码                                        
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::WriteComposeBuf()
{	
	if(*AS_pnPM == PHASEU) //外部接入的是相电压
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i]-dwSampBuf[i+READ_POINT_NUM];
			dwComposeBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM]-dwSampBuf[i+READ_POINT_NUM*2];
			dwComposeBuf[i+READ_POINT_NUM*2] = dwSampBuf[i+READ_POINT_NUM*2]-dwSampBuf[i];		
		}		
		CaluBaseFourier_S15(dwComposeBuf,&Uab);
		CaluModValue(&Uab);
		CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&Ubc);
		CaluModValue(&Ubc);
		CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*2,&Uca);
		CaluModValue(&Uca);
	}
	else //外部接入的是线电压
	{		
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i];
			dwComposeBuf[i+READ_POINT_NUM] = dwSampBuf[i+READ_POINT_NUM];
			dwComposeBuf[i+READ_POINT_NUM*2] = dwSampBuf[i+READ_POINT_NUM*2];		
		}		
		Uab = Ua;
		Ubc = Ub;
		Uca = Uc;
		
	}
	*RM_pnUab= Uab.Mod;                                       
	DB_LogWriteRM(RM_UAB, *RM_pnUab);	
	*RM_pnUbc= Ubc.Mod;                                       
	DB_LogWriteRM(RM_UBC, *RM_pnUbc);	
	*RM_pnUca= Uca.Mod;                                       
	DB_LogWriteRM(RM_UCA, *RM_pnUca);	

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
// 名称: 计算电量	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::CaluUI(void)
{		
	//计算U1模值并写保护测量、置U1有压继电器字、无压继电器字

	CaluBaseFourier_S15(dwSampBuf,&Ua);
	CaluModValue(&Ua);
	*RM_pnUa= Ua.Mod;                                       
	DB_LogWriteRM(RM_UA, *RM_pnUa);	
	if(Ua.Mod>=*AS_pn59UA)
	{
		WriteRelayWord(RW_59UA,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UA,RELAY_OFF);
	}
	if(Ua.Mod>=*AS_pn27UA)
	{
		WriteRelayWord(RW_27UA,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UA,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&Ub);
	CaluModValue(&Ub);
	*RM_pnUb= Ub.Mod;                                       
	DB_LogWriteRM(RM_UB, *RM_pnUb);	
	if(Ub.Mod>=*AS_pn59UB)
	{
		WriteRelayWord(RW_59UB,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UB,RELAY_OFF);
	}
	if(Ub.Mod>=*AS_pn27UB)
	{
		WriteRelayWord(RW_27UB,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UB,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&Uc);
	CaluModValue(&Uc);
	*RM_pnUc= Uc.Mod;                                       
	DB_LogWriteRM(RM_UC, *RM_pnUc);	
	if(Uc.Mod>=*AS_pn59UC)
	{
		WriteRelayWord(RW_59UC,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UC,RELAY_OFF);
	}
	if(Uc.Mod>=*AS_pn27UC)
	{
		WriteRelayWord(RW_27UC,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UC,RELAY_ON);
	}


	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0);
	CaluModValue(&U0);
	*RM_pnU0= U0.Mod;                                       
	DB_LogWriteRM(RM_U0, *RM_pnU0);	
	if(U0.Mod>=*AS_pn59U0)
	{
		WriteRelayWord(RW_59U0,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U0,RELAY_OFF);
	}
	if(U0.Mod>=*AS_pn27U0)
	{
		WriteRelayWord(RW_27U0,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U0,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&U1);
	CaluModValue(&U1);
	*RM_pnUL1= U1.Mod;                                       
	DB_LogWriteRM(RM_UL1, *RM_pnUL1);	
	if(U1.Mod>=*AS_pn59U1)
	{
		WriteRelayWord(RW_59U1,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1,RELAY_OFF);
	}
	if(U1.Mod>=*AS_pn27U1)
	{
		WriteRelayWord(RW_27U1,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&U2);
	CaluModValue(&U2);
	*RM_pnUL2= U2.Mod;                                       
	DB_LogWriteRM(RM_UL2, *RM_pnUL2);	
	if(U2.Mod>=*AS_pn59U2)
	{
		WriteRelayWord(RW_59U2,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U2,RELAY_OFF);
	}
	if(U2.Mod>=*AS_pn27U2)
	{
		WriteRelayWord(RW_27U2,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U2,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&U3);
	CaluModValue(&U3);
	*RM_pnUL3= U3.Mod;                                       
	DB_LogWriteRM(RM_UL3, *RM_pnUL3);	
	if(U3.Mod>=*AS_pn59U3)
	{
		WriteRelayWord(RW_59U3,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3,RELAY_OFF);
	}
	if(U3.Mod>=*AS_pn27U3)
	{
		WriteRelayWord(RW_27U3,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&Ia);
	CaluModValue(&Ia);
	*RM_pnIa= Ia.Mod;                                       
	DB_LogWriteRM(RM_IA, *RM_pnIa);	
	if(Ia.Mod>=*AS_pn50IA)
	{
		WriteRelayWord(RW_50IA,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IA,RELAY_OFF);
	}
	if(Ia.Mod>=*AS_pn37IA)
	{
		WriteRelayWord(RW_37IA,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IA,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&Ib);
	CaluModValue(&Ib);
	*RM_pnIb= Ib.Mod;                                       
	DB_LogWriteRM(RM_IB, *RM_pnIb);	
	if(Ib.Mod>=*AS_pn50IB)
	{
		WriteRelayWord(RW_50IB,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IB,RELAY_OFF);
	}
	if(Ib.Mod>=*AS_pn37IB)
	{
		WriteRelayWord(RW_37IB,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IB,RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&Ic);
	CaluModValue(&Ic);
	*RM_pnIc= Ic.Mod;                                       
	DB_LogWriteRM(RM_IC, *RM_pnIc);	
	if(Ic.Mod>=*AS_pn50IC)
	{
		WriteRelayWord(RW_50IC,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IC,RELAY_OFF);
	}
	if(Ic.Mod>=*AS_pn37IC)
	{
		WriteRelayWord(RW_37IC,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IC,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&I0);
	CaluModValue(&I0);
	*RM_pnI0= I0.Mod;                                       
	DB_LogWriteRM(RM_I0, *RM_pnI0);	
	if(I0.Mod>=*AS_pn50I0)
	{
		WriteRelayWord(RW_50I0,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I0,RELAY_OFF);
	}
	if(I0.Mod>=*AS_pn37I0)
	{
		WriteRelayWord(RW_37I0,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I0,RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&I1);
	CaluModValue(&I1);
	*RM_pnI1= I1.Mod;                                       
	DB_LogWriteRM(RM_I1, *RM_pnI1);	
	if(I1.Mod>=*AS_pn50I1)
	{
		WriteRelayWord(RW_50I1,RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1,RELAY_OFF);
	}
	if(I1.Mod>=*AS_pn37I1)
	{
		WriteRelayWord(RW_37I1,RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1,RELAY_ON);
	}
}
//------------------------------------------------------------------------------------------ 
// 名称: 计算合成电量	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUpIp(void)
{
	Ipmax = ElecMax(Ia,Ib,Ic); //三相电流中的最大值，并标明相别
	Ulmin = ElecMin(Uab,Ubc,Uca);		//线电压的最小值，
	Ulmax = ElecMax(Uab,Ubc,Uca);		//线电压的最大值，用于低压保护
}
void CRelayTask::CaluU2()
{
	NU2.Real = (Uab.Real*1000 + Ubc.Real*Factor2.Real -Ubc.Imag*Factor2.Imag + Uca.Real*Factor1.Real -Uca.Imag*Factor1.Imag)/5196;
	NU2.Imag = (Uab.Imag*1000 + Ubc.Imag*Factor2.Real +Ubc.Real*Factor2.Imag + Uca.Imag*Factor1.Real +Uca.Real*Factor1.Imag)/5196;
	CaluModValue(&NU2);
	*RM_pnU2= NU2.Mod;                                       
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
}
void CRelayTask::CaluUIAngle()
{
	LONG angle;
	CaluAngle(&Ua,AngleTab);	
	*RM_pnAUa=0;
	DB_LogWriteRM(RM_AUA, *RM_pnAUa);
	
	CaluAngle(&Ub,AngleTab);
	angle = Ub.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUb = angle;
	else
		*RM_pnAUb = angle+36000;
	DB_LogWriteRM(RM_AUB, *RM_pnAUb);

	CaluAngle(&Uc,AngleTab);	
	angle = Uc.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUc = angle;
	else
		*RM_pnAUc = angle+36000;
	DB_LogWriteRM(RM_AUC, *RM_pnAUc);

	CaluAngle(&Uab,AngleTab);	
	angle = Uab.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUab = angle;
	else
		*RM_pnAUab = angle+36000;
	DB_LogWriteRM(RM_AUAB, *RM_pnAUab);
	
	CaluAngle(&Ubc,AngleTab);	
	angle = Ubc.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUbc = angle;
	else
		*RM_pnAUbc = angle+36000;
	DB_LogWriteRM(RM_AUBC, *RM_pnAUbc);
	
	CaluAngle(&Uca,AngleTab);	
	angle = Uca.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUca = angle;
	else
		*RM_pnAUca = angle+36000;
	DB_LogWriteRM(RM_AUCA, *RM_pnAUca);

	CaluAngle(&U1,AngleTab);	
	angle = U1.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUl1 = angle;
	else
		*RM_pnAUl1 = angle+36000;
	DB_LogWriteRM(RM_AUL1, *RM_pnAUl1);
	
	CaluAngle(&U2,AngleTab);	
	angle = U2.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUl2 = angle;
	else
		*RM_pnAUl2 = angle+36000;
	DB_LogWriteRM(RM_AUL2, *RM_pnAUl2);
	
	CaluAngle(&U3,AngleTab);	
	angle = U3.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAUl3 = angle;
	else
		*RM_pnAUl3 = angle+36000;
	DB_LogWriteRM(RM_AUL3, *RM_pnAUl3);

	CaluAngle(&Ia,AngleTab);	
	angle = Ia.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIpa = angle;
	else
		*RM_pnAIpa = angle+36000;
	DB_LogWriteRM(RM_AIPA, *RM_pnAIpa);

	CaluAngle(&Ib,AngleTab);	
	angle = Ib.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIpb = angle;
	else
		*RM_pnAIpb = angle+36000;
	DB_LogWriteRM(RM_AIPB, *RM_pnAIpb);

	CaluAngle(&Ic,AngleTab);	
	angle = Ic.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIpc = angle;
	else
		*RM_pnAIpc = angle+36000;
	DB_LogWriteRM(RM_AIPC, *RM_pnAIpc);

	CaluAngle(&U0,AngleTab);	
	angle = U0.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAU01 = angle;
	else
		*RM_pnAU01 = angle+36000;
	DB_LogWriteRM(RM_AU01, *RM_pnAU01);

	CaluAngle(&I0,AngleTab);	
	angle = I0.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAI01 = angle;
	else
		*RM_pnAI01 = angle+36000;
	DB_LogWriteRM(RM_AI01, *RM_pnAI01);
	
	CaluAngle(&I1,AngleTab);	
	angle = I1.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAIP1 = angle;
	else
		*RM_pnAIP1 = angle+36000;
	DB_LogWriteRM(RM_AIP1, *RM_pnAIP1);

	CaluU5I5();

}
void CRelayTask::CaluU5I5(void)
{
	LONG angle;
	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*4,&U5);
	CaluModValue(&U5);
	*RM_pnU05= U5.Mod;										
	DB_LogWriteRM(RM_U05, *RM_pnU05); 	
	CaluAngle(&U5,AngleTab);	
	angle = U5.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAU05 = angle;
	else
		*RM_pnAU05 = angle+36000;
	DB_LogWriteRM(RM_AU05, *RM_pnAU05);

	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*11,&I5);
	CaluModValue(&I5);
	*RM_pnI05= I5.Mod;										
	DB_LogWriteRM(RM_I05, *RM_pnI05); 	
	CaluAngle(&I5,AngleTab);	
	angle = I5.Angle - Ua.Angle;
	if(angle>0)
		*RM_pnAI05 = angle;
	else
		*RM_pnAI05 = angle+36000;
	DB_LogWriteRM(RM_AI05, *RM_pnAI05);
}

void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&R50I0;
	StructCheck[0].bigsize = sizeof(R50I0)/2 - R50I0.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&R50I1;
	StructCheck[1].bigsize = sizeof(R50I1)/2 - R50I1.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&R60I;
	StructCheck[2].bigsize = sizeof(R60I)/2 - R60I.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&R60U;
	StructCheck[3].bigsize = sizeof(R60U)/2 - R60U.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&R59U;
	StructCheck[4].bigsize = sizeof(R59U)/2 - R59U.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&R27U;
	StructCheck[5].bigsize = sizeof(R27U)/2 - R27U.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[6].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
	for(LONG j=0;j<CHECK_NUM;j++)
	{
		for(LONG i=0;i<StructCheck[j].bigsize;i++)
		{
			acc += StructCheck[j].pstruct[i];
		}
		StructCheck[j].acc = acc;
		acc=0x55aa;
	}

	RelayCheckInitOK = TRUE;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流I段保护元件	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I0()
{
	R50I0.pElecSet				= RS_pn50I0C;          						//过量保护电量定值 
	R50I0.pTimeSet				= RS_pn50I0D;				 				//过量保护时间定值 
	R50I0.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50I0.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50I0.pAccActTimeSet		= NULL;						//过量保护后加速动作时间定值 
	R50I0.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50I0.pRelCFG				= CFG_pby50I0;							//过量保护功能控制字--1:投入，2:退出  
	R50I0.pRelSW				= SW_pby50I0;         					//过量保护软压板------1:投入，2:退出  
	R50I0.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50I0.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50I0.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R50I0.pRelFDCFG				= &RelCFGCLR;					//
	R50I0.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50I0.pRelAccCFG			= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R50I0.pRel47UCFG			= &RelCFGCLR;					//复压闭锁功能
	R50I0.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	
	R50I0.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50I0.pRelElec 				= &Ipmax;						//当前电量指针 		
	R50I0.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50I0.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50I0.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50I0.StartRelayWord		= RW_PI50I0;							//过量保护入段继电器字 
	R50I0.ActRelayWord			= RW_TR50I0;							//过量保护动作继电器字 
	R50I0.AccRelayWord			= FALSE1;							//后加速允许继电器字,是一个时间继电器字，延时到后自动复归 
	R50I0.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R50I0.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R50I0.PRewardRelayWord  	= FALSE1;
	R50I0.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	R50I0.R47ULKRelayWord		= FALSE1;
	R50I0.LockRelayWord			= RW_LKPI50I0;							//入段闭锁继电器字 
	R50I0.RSTRelayWord			= RW_LKRST50I0;							//返回闭锁继电器字 
	R50I0.LoopCtrlRelayWord		= RW_LPC50I0;						//保护循环控制继电器字 
	
	R50I0.wReportNum			= R_R50I0;								//过量保护报告序号 
	R50I0.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R50I0.dwK35					= NULL;
	R50I0.RptSerialNum			= &RptSerialNumOfAct;
	R50I0.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50I0.wPickUpReportNum		= R_R50I0_PI;						//入段报告序列号
	R50I0.wResetReportNum		= R_R50I0_RES;						//返回报告序列号
	//**********************************************************************	


//	R50I0.dwRelTimer			= 0;			 				//定时器 
	R50I0.byRelFCounter			= 0;						//故障计数器 
	R50I0.byRelRCounter			= 0;						//返回计数器 	
	R50I0.wExcepWordNum			= 8;
	R50I0.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流II段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitR50I1()
{	
	R50I1.pElecSet				= RS_pn50I1C;          						//过量保护电量定值 
	R50I1.pTimeSet				= RS_pn50I1D;				 				//过量保护时间定值 
	R50I1.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50I1.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50I1.pAccActTimeSet		= NULL;						//过量保护后加速动作时间定值 
	R50I1.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50I1.pRelCFG				= CFG_pby50I1;							//过量保护功能控制字--1:投入，2:退出  
	R50I1.pRelSW				= SW_pby50I1;         					//过量保护软压板------1:投入，2:退出  
	R50I1.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50I1.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50I1.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R50I1.pRelFDCFG				= &RelCFGCLR;
	R50I1.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50I1.pRelAccCFG			= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R50I1.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R50I1.pRel47UCFG			= &RelCFGCLR;
	R50I1.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50I1.pRelElec 				= &Ipmax;						//当前电量指针 		
	R50I1.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50I1.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50I1.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50I1.StartRelayWord		= RW_PI50I1;							//过量保护入段继电器字 
	R50I1.ActRelayWord			= RW_TR50I1;							//过量保护动作继电器字 
	R50I1.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R50I1.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R50I1.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R50I1.PRewardRelayWord  	= FALSE1;					//功率反方向继电器字 
	R50I1.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R50I1.R47ULKRelayWord		= FALSE1;
	R50I1.LockRelayWord			= RW_LKPI50I1;							//入段闭锁继电器字 
	R50I1.RSTRelayWord			= RW_LKRST50I1;							//返回闭锁继电器字 
	R50I1.LoopCtrlRelayWord		= RW_LPC50I1;						//保护循环控制继电器字 
	
	R50I1.wReportNum			= R_R50I1;								//过量保护报告序号 
	R50I1.wAccReportNum			= 0xFFFF;;							//加速动作报告序号 
	
	R50I1.dwK35					= NULL;
	R50I1.RptSerialNum			= &RptSerialNumOfAct;
	R50I1.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50I1.wPickUpReportNum		= R_R50I1_PI;						//入段报告序列号
	R50I1.wResetReportNum		= R_R50I1_RES;						//返回报告序列号
	//**********************************************************************	

//	R50I1.dwRelTimer			= 0;			 				//定时器 
	R50I1.byRelFCounter			= 0;						//故障计数器 
	R50I1.byRelRCounter			= 0;						//返回计数器 
	R50I1.wExcepWordNum			= 8;
	R50I1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流III段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR60I()
{	
	R60I.pElecSet				= RS_pn60IC;          						//过量保护电量定值 
	R60I.pTimeSet				= RS_pn60ID;				 				//过量保护时间定值 
	R60I.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R60I.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R60I.pAccActTimeSet			= NULL;						//过量保护后加速动作时间定值 
	R60I.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R60I.pRelCFG				= CFG_pby60I;							//过量保护功能控制字--1:投入，2:退出  
	R60I.pRelSW					= SW_pby60I;         					//过量保护软压板------1:投入，2:退出  
	R60I.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R60I.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R60I.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R60I.pRelFDCFG				= &RelCFGCLR;
	R60I.pRelVolLockCFG			= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R60I.pRelAccCFG				= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R60I.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R60I.pRel47UCFG				= &RelCFGCLR;
	R60I.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R60I.pRelElec 				= &I1;						//当前电量指针 		
	R60I.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R60I.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R60I.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R60I.StartRelayWord			= RW_PI60I;							//过量保护入段继电器字 
	R60I.ActRelayWord			= RW_TR60I;							//过量保护动作继电器字 
	R60I.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R60I.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R60I.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R60I.PRewardRelayWord  		= FALSE1;					//功率反方向继电器字 
	R60I.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R60I.R47ULKRelayWord		= FALSE1;
	R60I.LockRelayWord			= RW_LKPI60I;							//入段闭锁继电器字 
	R60I.RSTRelayWord			= RW_LKRST60I;							//返回闭锁继电器字 
	R60I.LoopCtrlRelayWord		= RW_LPC60I;						//保护循环控制继电器字 
	
	R60I.wReportNum				= R_R60I;								//过量保护报告序号 
	R60I.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R60I.dwK35					= NULL;
	R60I.RptSerialNum			= &RptSerialNumOfAct;
	R60I.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R60I.wPickUpReportNum		= R_R60I_PI;						//入段报告序列号
	R60I.wResetReportNum		= R_R60I_RES;						//返回报告序列号
	//**********************************************************************	

//	R60I.dwRelTimer				= 0;			 				//定时器 
	R60I.byRelFCounter			= 0;						//故障计数器 
	R60I.byRelRCounter			= 0;						//返回计数器 
	R60I.wExcepWordNum			= 8;
	R60I.wInfo 					= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化零序过流保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR60U()
{	
	R60U.pElecSet				= RS_pn60UV;          						//过量保护电量定值 
	R60U.pTimeSet				= RS_pn60UD;				 				//过量保护时间定值 
	R60U.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R60U.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R60U.pAccActTimeSet			= NULL;						//过量保护后加速动作时间定值 
	R60U.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R60U.pRelCFG				= CFG_pby60U;							//过量保护功能控制字--1:投入，2:退出  
	R60U.pRelSW					= SW_pby60U;         					//过量保护软压板------1:投入，2:退出  
	R60U.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R60U.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R60U.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R60U.pRelFDCFG				= &RelCFGCLR;
	R60U.pRelVolLockCFG			= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R60U.pRelAccCFG				= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R60U.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R60U.pRel47UCFG				= &RelCFGCLR;
	R60U.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R60U.pRelElec 				= &U1;						//当前电量指针 		
	R60U.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R60U.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R60U.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R60U.StartRelayWord			= RW_PI60U;							//过量保护入段继电器字 
	R60U.ActRelayWord			= RW_TR60U;							//过量保护动作继电器字 
	R60U.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R60U.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R60U.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R60U.PRewardRelayWord  		= FALSE1;					//功率反方向继电器字 
	R60U.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R60U.R47ULKRelayWord		= FALSE1;
	R60U.LockRelayWord			= RW_LKPI60U;							//入段闭锁继电器字 
	R60U.RSTRelayWord			= RW_LKRST60U;							//返回闭锁继电器字 
	R60U.LoopCtrlRelayWord		= RW_LPC60U;						//保护循环控制继电器字 
	
	R60U.wReportNum				= R_R60U;								//过量保护报告序号 
	R60U.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R60U.dwK35					= NULL;
	R60U.RptSerialNum			= &RptSerialNumOfAct;
	R60U.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R60U.wPickUpReportNum		= R_R60U_PI;						//入段报告序列号
	R60U.wResetReportNum		= R_R60U_RES;						//返回报告序列号
	//**********************************************************************	

//	R60U.dwRelTimer				= 0;			 				//定时器 
	R60U.byRelFCounter			= 0;						//故障计数器 
	R60U.byRelRCounter			= 0;						//返回计数器 
	R60U.wExcepWordNum			= 8;
	R60U.wInfo 					= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化零序过流保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR59U()
{	
	
	R59U.pElecSet				= RS_pn59U;          						//过量保护电量定值 
	R59U.pTimeSet				= RS_pn59D;				 				//过量保护时间定值 
	R59U.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R59U.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R59U.pAccActTimeSet			= NULL;						//过量保护后加速动作时间定值 
	R59U.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R59U.pRelCFG				= CFG_pby59U;							//过量保护功能控制字--1:投入，2:退出  
	R59U.pRelSW					= SW_pby59U;         					//过量保护软压板------1:投入，2:退出  
	R59U.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R59U.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R59U.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R59U.pRelFDCFG				= &RelCFGCLR;
	R59U.pRelVolLockCFG			= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R59U.pRelAccCFG				= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R59U.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R59U.pRel47UCFG				= &RelCFGCLR;
	R59U.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R59U.pRelElec 				= &Ulmax;						//当前电量指针 		
	R59U.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R59U.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R59U.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R59U.StartRelayWord			= RW_PI59U;							//过量保护入段继电器字 
	R59U.ActRelayWord			= RW_TR59U;							//过量保护动作继电器字 
	R59U.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R59U.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R59U.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R59U.PRewardRelayWord  		= FALSE1;					//功率反方向继电器字 
	R59U.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R59U.R47ULKRelayWord		= FALSE1;
	R59U.LockRelayWord			= RW_LKPI59U;							//入段闭锁继电器字 
	R59U.RSTRelayWord			= RW_LKRST59U;							//返回闭锁继电器字 
	R59U.LoopCtrlRelayWord		= RW_LPC59U;						//保护循环控制继电器字 
	
	R59U.wReportNum				= R_R59U;								//过量保护报告序号 
	R59U.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R59U.dwK35					= NULL;
	R59U.RptSerialNum			= &RptSerialNumOfAct;
	R59U.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R59U.wPickUpReportNum		= R_R59U_PI;						//入段报告序列号
	R59U.wResetReportNum		= R_R59U_RES;						//返回报告序列号
	//**********************************************************************	

//	R59U.dwRelTimer				= 0;			 				//定时器 
	R59U.byRelFCounter			= 0;						//故障计数器 
	R59U.byRelRCounter			= 0;						//返回计数器 
	R59U.wExcepWordNum			= 8;
	R59U.wInfo 					= 0;
}

/************************************************************************************************
*功能: 初始化低压保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitR27U()
{
	R27U.pELeSet						= RS_pn27U;						//电量定值 
	R27U.pTimeSet						= RS_pn27D;						//时间定值 
	R27U.pLockCurSet					= RS_pn27ILO;					//闭锁电流定值 

	R27U.pRelCFG						= CFG_pby27U;					  	 //欠量保护配置 
	R27U.pRelSW							= SW_pby27U;					  	 //欠量保护软压板 
	R27U.pRelAlarmCFG					= &RelCFGCLR;						//欠量保护投跳闸 
	R27U.pRelCurLockCFG					= &RelCFGSET;
	R27U.pSysTimer						= &dwRelayMSCnt;						//系统定时器指针 

	R27U.pRelElec						= &Ulmax;						//当前的电量指针 
	R27U.pRelLockElec					= &Ipmax;					//闭锁电流 
	R27U.ActRelayWord					= RW_TR27U;					//欠量保护动作继电器字 
	R27U.StartRelayWord					= RW_PI27U;					//欠量保护入段继电器字 
	R27U.AlarmRelayWord					= FALSE1;					//欠量保护告警继电器字 
	R27U.LockRelayWord					= RW_LKPI27;				//入段闭锁继电器字 
	R27U.RSTRelayWord					= RW_LKRST27;					//故障返回闭锁继电器字 
	R27U.LoopCtrlRelayWord				= RW_LPC27U;				//故障循环控制继电器字 
	R27U.wR52BFRealyword				= RW_52BF;					//断路器跳闸失败继电器字
	R27U.wActRptNum						= R_R27U;					//欠量保护动作报告序号 
	R27U.wAlarmRptNum					= 0xFFFF;					//欠量保护告警报告序号 
	R27U.wTRFailRptNum					= R_TRFAIL;
	R27U.wPickUpRptNum					= R_ACTPICK;
	R27U.RptSerialNum					= &g_RptSerialNum;
	
	R27U.R27RptSerialNum				= 0;
//	R27U.dwRelTimer						= 0;						//欠量保护定时器 	
	R27U.byRelFCounter					= 0;					//故障计数器 
	R27U.byRelRCounter					= 0;					//返回计数器 
	R27U.bHaveMake52BFRptFlag			= FALSE;
	R27U.wExcepWordNum					= 14;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
/************************************************************************************************
*功能: 取得模值最大的向量
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

TRelElecValPar CRelayTask::ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3)
{
	if(ele1.Mod>=ele2.Mod)
	{
		if(ele1.Mod>=ele3.Mod)
		{
			
			return ele1;
		}
		else
			return ele3;		
	}
	else
	{
		if(ele2.Mod>=ele3.Mod)
			return ele2;
		else
			return ele3;
	}
}
/************************************************************************************************
*功能: 取得模值最大的向量，并标明相别
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

TRelElecValPar CRelayTask::ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase)
{
	if(ele1.Mod>=ele2.Mod)
	{
		if(ele1.Mod>=ele3.Mod)
		{
			phase = RELAY_PHASE_A;
			return ele1;
		}
		else
		{
			phase = RELAY_PHASE_C;
			return ele3;
		}
	}
	else
	{
		if(ele2.Mod>=ele3.Mod)
		{
			phase = RELAY_PHASE_B;
			return ele2;
		}
		else
		{
			phase = RELAY_PHASE_C;
			return ele3;
		}
	}
}
/************************************************************************************************
*功能: 取得模值最小的向量
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
TRelElecValPar CRelayTask::ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3)
{
	if(ele1.Mod<=ele2.Mod)
	{
		if(ele1.Mod<=ele3.Mod)
			return ele1;
		else
			return ele3;
	}
	else
	{
		if(ele2.Mod<=ele3.Mod)
			return ele2;
		else
			return ele3;
	}
}
//------------------------------------------------------------------------------------------ 
// 名称: OnRelayChange	 
// 功能: 相应定值修改消息函数 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::OnRelayChange(DWORD dwDataType,DWORD dwIndex)
{
	//重新初始化保护数据
	::InitAllRelayMem();
	InitRealy();
	InitSet();	
	InitStructCheck();
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
	BOOL haveRelayInput = FALSE;
	BYTE  pChannel[]={CHANNEL_6561_IA,CHANNEL_6561_IB,CHANNEL_6561_IC,CHANNEL_6561_I0,CHANNEL_6561_I1};
	if(ReadRelayWord(RW_52BF))  //断路器跳闸失败，不进行保护启动判断处理 
	{
		WriteRelayWord(RW_PICKUP, RELAY_OFF);
		return FALSE;
	}
	if((*R50I0.pRelCFG == CFG_STATUS_SET) && (*R50I0.pRelSW == SW_STATUS_SET))   //过流1段保护投入 
	{
		haveRelayInput = TRUE;
		if(R50I0.pRelElec->Mod > *R50I0.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R50I1.pRelCFG == CFG_STATUS_SET) && (*R50I1.pRelSW == SW_STATUS_SET))   //过流2段保护投入 
	{		
		haveRelayInput = TRUE;
		if(R50I1.pRelElec->Mod > *R50I1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R60I.pRelCFG == CFG_STATUS_SET) && (*R60I.pRelSW == SW_STATUS_SET))   //过流3段保护投入 
	{
		haveRelayInput = TRUE;
		if(R60I.pRelElec->Mod > *R60I.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R60U.pRelCFG == CFG_STATUS_SET) && (*R60U.pRelSW == SW_STATUS_SET))   //零流1段保护投入 
	{
		haveRelayInput = TRUE;
		if(R60U.pRelElec->Mod > *R60U.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*R59U.pRelCFG == CFG_STATUS_SET) && (*R59U.pRelSW == SW_STATUS_SET))   //零流1段保护投入 
	{
		haveRelayInput = TRUE;
		if(R59U.pRelElec->Mod > *R59U.pElecSet)
		{
			WriteRelayWord(RW_PICKUP, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}

	//突变量启动部分
	//突变量启动部分,有保护投入
	if(haveRelayInput)
	{
		 ReadPreSingleData(dwPickDataBuf,pChannel,5,RELAY_SAMP_INTERVAL);
		 for(LONG j=0;j<5;j++)
		 {	
			//将3个周波的采样点分别取至相应的数组,并计算突变量,且进行判断
			PickCheck = 0;	
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickDataBuf[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));
				
				if(lPickValue>*RS_pnPIC)
				{
					PickCheck++;
					if(PickCheck==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	

						WriteRelayWord(RW_PICKUP, RELAY_ON);
						ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
						RptSerialNumOfStart = g_RptSerialNum;
						PickCheck =0;
						PickCheckNext[j] =0;
						return TRUE;	
					}
					if(i<2)
					{
						PickCheckNext[j]++;
						if(PickCheckNext[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime);
							PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);	
							
							WriteRelayWord(RW_PICKUP, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
							RptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
							RptSerialNumOfStart = g_RptSerialNum;
							PickCheck =0;
							PickCheckNext[j] =0;
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						PickCheckNext[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						PickCheckNext[j]++;
				}
				else
				{
					PickCheck =0;
					PickCheckNext[j]=0;
				}
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
	if(!FaultOnFlag)   						//尚未进入故障处理
	{		
		RelayPickUpCHK();					//执行启动检测
		if(ReadRelayWord(RW_PICKUP))
		{
			FaultSINum = 0;       //保护启动，初始化故障处理采样间隔计数器
			FaultOnFlag=TRUE;
			ResetAllRelay(TRUE);
//			return;
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//故障处理采样间隔计数器累加
	if(FaultSINum<READ_POINT_NUM)					//故障处理采样间隔计数器小于一周波采样点数
	{
		return;
	}	
	else
	{
		if(!HaveMakeStartRptFlag)   //向带故障电量的启动报告中写入故障电量
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}

		
		GeneralOverValueRelay(&R50I0);				//调用速断子程序
		GeneralOverValueRelay(&R50I1);				//调用过流I段保护子程序
		GeneralOverValueRelay(&R60I);				//调用过流III段保护子程序
		GeneralOverValueRelay(&R60U);				//调用过流III段保护子程序
		GeneralOverValueRelay(&R59U);
		
		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveMakeTripFailRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//作断路器跳闸失败报告
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}

		// 1。有保护循环控制继电器字被置位，等待保护动作或者退出
		if(ReadRelayWord(R50I0.LoopCtrlRelayWord)||ReadRelayWord(R50I1.LoopCtrlRelayWord)||ReadRelayWord(R60I.LoopCtrlRelayWord)
			||ReadRelayWord(R60U.LoopCtrlRelayWord)||ReadRelayWord(R59U.LoopCtrlRelayWord))
		{			
			// 1.1 保护已经动作，且跳闸失败
			if(ReadRelayWord(RW_52BF))
			{
				WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
				FaultCheckOutFlag = FALSE;					//清检出故障标志
				FaultOnFlag = FALSE;						//清故障循环处理标志
				HaveMakeStartRptFlag= FALSE;				//清已作带故障电量启动报告标志
				m_bFaultLKFlag = FALSE; 					//清有启动无动作时启动时刻锁定标志	
				m_bHaveMakeTripFailRpt = FALSE;
				DB_RelayEnd(RptSerialNumOfAct); 			//向数据库中写入结束报告	
				return;
			}
			// 1.2 等待保护动作，刷新故障锁定定时器，置检出故障标志
			else
			{
				FaultLockTime = dwRelayMSCnt;		
				FaultCheckOutFlag = TRUE;
				return;
			}
		}
		// 2。无保护循环控制继电器字被置位，此时保护已动作结束或没有满足任何保护动作条件
		else				
		{
			// 2.1 检出故障标志被置位，表示保护循环控制继电器字曾经被置位--不一定动作过--等待RS_pnSOTFT时间后退出
			if(FaultCheckOutFlag)     
			{
				if(ReadRelayWord(RW_T52BF)) //断路器跳闸失败判断正在延时
				{
					FaultLockTime = dwRelayMSCnt;	
					return;
				}
				if((dwRelayMSCnt -FaultLockTime)>500)  //无故障时间超过合于故障检测时间
				{						
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct);				//向数据库中写入结束报告
					ResetAllRelay(TRUE);
					return;					
				}
				else
					return;
			}
			// 2.2 检出故障标志未被置位，表示保护循环控制继电器字在本次故障处理中从未被置位，等待0.5秒后退出
			else
			{
				if(!m_bFaultLKFlag)//保护刚刚启动，记录起始时刻
				{	
					FaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				if(dwRelayMSCnt -FaultLockTime>1000 )	
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志						
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct); 			
					ResetAllRelay(TRUE);
					return;					
				}
				else	//延时未到，直接返回
					return;
			}			
		}		
	}
}
void CRelayTask::Process52BF(void)
{
	if(ReadRelayWord(RW_52BF))
	{
		m_b52BFFLAG = TRUE;
	}
	if(m_b52BFFLAG)
	{
		if(!ReadRelayWord(RW_52BF))
		{
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&R27U);
			m_b52BFFLAG = FALSE;
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
	ResetOverValueRelay(&R50I0,resetFlag);
	ResetOverValueRelay(&R50I1,resetFlag);
	ResetOverValueRelay(&R60U,resetFlag);
	ResetOverValueRelay(&R60I,resetFlag);
	ResetOverValueRelay(&R59U,resetFlag);
}
void CRelayTask::PTCheck()
{	
	if(*CFG_pbyPTBR == CFG_STATUS_CLR) //PT断线未投入
	{
		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_R74PT_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		m_BusPTTimer = 0;
		return;
	}
//母线PT断线判断
	if(((Ulmin.Mod<*RS_pn74PTU)&&(Ipmax.Mod>*RS_pn74PTI))||(NU2.Mod>*RS_pn74PTU2))
	{
		if(ReadRelayWord(RW_PIPT1))
		{
			if(!ReadRelayWord(RW_ARPT1))
			{
				if(dwRelayMSCnt - m_BusPTTimer>*RS_pn74PTD)
				{
					WriteRelayWord(RW_ARPT1,RELAY_ON);
					MakeAlarmRpt(R_R74PT);
				}
			}
		}
		else
		{
			WriteRelayWord(RW_PIPT1,RELAY_ON);
			m_BusPTTimer = dwRelayMSCnt;
		}
	}
	else //只有在电压恢复正常时方复归
	{
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_R74PT_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
		m_BusPTTimer = 0;
	}
}

void CRelayTask::R59NAlarm() //零压告警
{
	static DWORD R59NTimer = 0;
	if(*CFG_pby59N == CFG_STATUS_CLR)
	{
		WriteRelayWord(RW_PI59N,RELAY_OFF);
		WriteRelayWord(RW_TR59N,RELAY_OFF);
		R59NTimer = 0;
		return;
	}
	if(U0.Mod>*RS_pn59NU)
	{
		if(!ReadRelayWord(RW_TR59N))
		{
			if(!ReadRelayWord(RW_PI59N))
			{
				R59NTimer = dwRelayMSCnt;
				WriteRelayWord(RW_PI59N, RELAY_ON);
			}
			else
			{
				if((dwRelayMSCnt - R59NTimer)>*RS_pn59ND)
				{
					WriteRelayWord(RW_TR59N, RELAY_ON);
					MakeAlarmRpt(R_R59NU);
				}
			}
		}
	}
	else
	{
		WriteRelayWord(RW_PI59N,RELAY_OFF);
		WriteRelayWord(RW_TR59N,RELAY_OFF);
		R59NTimer = 0;
	}
}












