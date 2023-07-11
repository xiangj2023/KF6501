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
* 描述      KF6521线路保护测控装置                                                                       
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
*      yanzh             12/07/01    初始创建                         
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

extern DWORD dwADDataWritePtr;	//采样缓冲区数据读写指针

DWORD		dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD		dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
DWORD 		dwRelayTaskID;
BOOL 		RelayTaskInitOK = FALSE;
TStructCheck StructCheck[CHECK_NUM];
extern DWORD dwADResumeCnt;

TRunTimeRM	*pSysRM;		//保护测量值数据指针
TRunTimeBI  *pSysBI;		//遥信实时数据指针
BOOL g_FaultOnFlag;
TLineData	JDXXElecData;


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
				case SM_JDXXRESULT:
					pRelayTask->OnJDXXResult(0,msg.hParam);
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
	g_FaultOnFlag = FALSE;			//进入故障处理标志 用于CT断线检测
	HaveMakeStartRptFlag = FALSE;	//已作带故障电量启动报告标志
	m_bFaultLKFlag = FALSE;			//故障起始时刻锁定标志
	CurRecoverFlag =FALSE;   		//重合闸动作后电流恢复标志
	FaultCheckOutFlag = FALSE;		//检出故障标志
	m_bLastRecStatusFlag = FALSE;	//故障录波启动上升沿判断标志
	m_bPDCheckA = FALSE;				//已作带记忆功率方向检测标志
	m_bPDCheckB = FALSE;				//已作带记忆功率方向检测标志
	m_bPDCheckC = FALSE;				//已作带记忆功率方向检测标志
	m_b52BFFLAG = FALSE;
	m_bLogicChanged = FALSE;
	m_bR79PickUpRpt = FALSE;

	//---------------接地选线标志初始化---------------	
	JDXXOnFlag = FALSE;
	JDXXCheckFlag = FALSE;
	JDXXPickFlag = FALSE;
	JDXXForwardFlag = FALSE;
	HaveJDXXResultFlag = FALSE;
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
	dwChanDataMask = ( (0x01<<CHANNEL_6521_UA)|(0x01<<CHANNEL_6521_UB)|(0x01<<CHANNEL_6521_UC)|\
						(0x01<<CHANNEL_6521_U0)|(0x01<<CHANNEL_6521_U1)|(0x01<<CHANNEL_6521_U2)|\
						(0x01<<CHANNEL_6521_IA)|(0x01<<CHANNEL_6521_IB)|(0x01<<CHANNEL_6521_IC)|\
						(0x01<<CHANNEL_6521_I0)|(0x01<<CHANNEL_6521_I1)|(0x01<<CHANNEL_6521_U3));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	
	dwComposeMask = ((0x01<<CHANNEL_6521_UAB)|(0x01<<CHANNEL_6521_UBC)|(0x01<<CHANNEL_6521_UCA));
	
	m_dwFreqMask2 = CHANNEL_6521_U1;	
	//初始化读取保护数据临时缓冲区
	//保护数据缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*12);	
	//合成通道数据缓冲区
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);
	//突变量启动历史数据缓冲区
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*5);
	//带记忆功率方向判断数据缓冲区
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);

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
	
	ASSERT(dwPDDataBuf != NULL);
	if(dwPDDataBuf == NULL)
	{
		LogError("CRelayTask,dwPDDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPDDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM);
	
	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3));

//*************测频相关**********************************************************

	m_wFreqBuf = (SHORT*)OS_MemAlloc(sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	m_FreqCaluBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*FREQUENCE_SAMP_NUM);

	ASSERT(m_wFreqBuf != NULL);
	if(m_wFreqBuf == NULL)
	{
		LogError("CRelayTask,m_wFreqBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_wFreqBuf, 0, sizeof(SHORT)*FREQUENCE_SAMP_NUM);
	
	ASSERT(m_FreqCaluBuf != NULL);
	if(m_FreqCaluBuf == NULL)
	{
		LogError("CRelayTask,m_FreqCaluBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_FreqCaluBuf, 0, sizeof(DWORD)*FREQUENCE_SAMP_NUM);

//*******************************************************************************
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
	
	caluFlag = FALSE;
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

	m_U2Set = *CF_pnUp*8/100; //8V负序电压
	
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
	InitR50I2();
	InitR50I3();
	InitR50N1();
	InitR50N2();
	InitR50IOL();
	InitR50ISH();
	InitR27U();		
	InitRel79();	
	InitRel25();
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
//		LEDOnOff(ALARM_LED, TRUE);
		RelayCheckErr = TRUE;
		return;
	}

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
	GeneralOverValueRelay(&R50IOL);
	AutoRCAct(&Rel79);							//调用重合闸子程序
	R59NAlarm();
	Process52BF();
	
	
	LogicProcess();								//逻辑方程处理
	
	Rel25Check();		//检同期
	JDXXCheck();
	
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
	
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL] = dwComposeBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[i+RELAY_SAMP_INTERVAL*2] = dwComposeBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
	}
	WriteComposeData(dwRecBuf, dwComposeMask);

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
	Ip = ElecMax(Ia,Ib,Ic,m_Phase); //三相电流中的最大值，并标明相别
	ULK = ElecMin(Uab,Ubc,Uca);		//线电压的最小值，用于过流的低压闭锁
	Up = ElecMax(Uab,Ubc,Uca);		//线电压的最大值，用于低压保护

	//功率方向继电器字需根据最大电流的相别进行调整	
	if(m_Phase == RELAY_PHASE_A)
	{
		R50I1.PForwardRelayWord = RW_APF;
		R50I1.PRewardRelayWord	= RW_APR;		
		R50I2.PForwardRelayWord = RW_APF;
		R50I2.PRewardRelayWord	= RW_APR;
		R50I3.PForwardRelayWord = RW_APF;
		R50I3.PRewardRelayWord	= RW_APR;
	}
	else if(m_Phase == RELAY_PHASE_B)		
	{
		R50I1.PForwardRelayWord = RW_BPF;
		R50I1.PRewardRelayWord	= RW_BPR;		
		R50I2.PForwardRelayWord = RW_BPF;
		R50I2.PRewardRelayWord	= RW_BPR;
		R50I3.PForwardRelayWord = RW_BPF;
		R50I3.PRewardRelayWord	= RW_BPR;
	}	
	else if(m_Phase == RELAY_PHASE_C)		
	{
		R50I1.PForwardRelayWord = RW_CPF;
		R50I1.PRewardRelayWord	= RW_CPR;		
		R50I2.PForwardRelayWord = RW_CPF;
		R50I2.PRewardRelayWord	= RW_CPR;
		R50I3.PForwardRelayWord = RW_CPF;
		R50I3.PRewardRelayWord	= RW_CPR;
	}
}
void CRelayTask::CaluU2()
{
	NU2.Real = (Uab.Real*1000 + Ubc.Real*Factor2.Real -Ubc.Imag*Factor2.Imag + Uca.Real*Factor1.Real -Uca.Imag*Factor1.Imag)/5196;
	NU2.Imag = (Uab.Imag*1000 + Ubc.Imag*Factor2.Real +Ubc.Real*Factor2.Imag + Uca.Imag*Factor1.Real +Uca.Real*Factor1.Imag)/5196;
	CaluModValue(&NU2);
	*RM_pnU2= NU2.Mod;                                       
	DB_LogWriteRM(RM_U2, *RM_pnU2);

	//判断复压闭锁是否满足
	if((*R50I0.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P0_27)||(NU2.Mod>*RS_pn50P0_47)))
		WriteRelayWord(RW_LK50I0_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I0_47U,RELAY_ON);
	
	if((*R50I1.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P1_27)||(NU2.Mod>*RS_pn50P1_47)))
		WriteRelayWord(RW_LK50I1_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I1_47U,RELAY_ON);
	
	if((*R50I2.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P2_27)||(NU2.Mod>*RS_pn50P2_47)))
		WriteRelayWord(RW_LK50I2_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I2_47U,RELAY_ON);

	if((*R50I3.pRel47UCFG ==CFG_STATUS_SET)&&((ULK.Mod<*RS_pn50P3_27)||(NU2.Mod>*RS_pn50P3_47)))
		WriteRelayWord(RW_LK50I3_47U,RELAY_OFF);
	else
		WriteRelayWord(RW_LK50I3_47U,RELAY_ON);
	
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
	*RM_pnAU01=0;
	DB_LogWriteRM(RM_AU01, *RM_pnAU01);

	CaluAngle(&I0,AngleTab);	
	if(I0.Angle>=U0.Angle)
		*RM_pnAI01=I0.Angle - U0.Angle;
	else		
		*RM_pnAI01=36000 + I0.Angle - U0.Angle;
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
	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*3,&U5);
	CaluModValue(&U5);
	//计算结果进行补偿,1111为补偿系数
	U5.Mod = U5.Mod*1111/1000;
	U5.Imag = U5.Imag*1111/1000;
	U5.Real = U5.Real*1111/1000;
	*RM_pnU05= U5.Mod;										
	DB_LogWriteRM(RM_U05, *RM_pnU05); 	
	CaluAngle(&U5,AngleTab);	
	*RM_pnAU05=0;
	DB_LogWriteRM(RM_AU05, *RM_pnAU05);

	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*10,&I5);
	CaluModValue(&I5);	
	//计算结果进行补偿,1100为补偿系数
	I5.Mod = I5.Mod*1100/1000;
	I5.Imag = I5.Imag*1100/1000;
	I5.Real = I5.Real*1100/1000;
	*RM_pnI05= I5.Mod;										
	DB_LogWriteRM(RM_I05, *RM_pnI05); 	
	CaluAngle(&I5,AngleTab);	
	if(I5.Angle >= U5.Angle)
		*RM_pnAI05=I5.Angle - U5.Angle;
	else
		*RM_pnAI05=36000 + I5.Angle - U5.Angle;
	DB_LogWriteRM(RM_AI05, *RM_pnAI05);
}
void CRelayTask::CaluDF_DA_DV()
{
	LONG	Uh,Ul;
	if(*RS_pn25PH == SYN_PHASE_UA)
	{
		m_dwFreqMask1 = CHANNEL_6521_UA;
		Usyn = Ua;
	}
	else if(*RS_pn25PH == SYN_PHASE_UB)
	{
		m_dwFreqMask1 = CHANNEL_6521_UB;			
		Usyn = Ub;
	}
	else if(*RS_pn25PH == SYN_PHASE_UC)
	{
		m_dwFreqMask1 = CHANNEL_6521_UC;	
		Usyn = Uc;
	}
	else if(*RS_pn25PH == SYN_PHASE_UAB)
	{
		m_dwFreqMask1 = CHANNEL_6521_UAB;	
		Usyn = Uab;
	}
	else if(*RS_pn25PH == SYN_PHASE_UBC)
	{
		m_dwFreqMask1 = CHANNEL_6521_UBC;	
		Usyn = Ubc;
	}
	else if(*RS_pn25PH == SYN_PHASE_UCA)
	{
		m_dwFreqMask1 = CHANNEL_6521_UCA;
		Usyn = Uca;
	}
	else 
	{
		m_dwFreqMask1 = CHANNEL_6521_UA;
		Usyn = Ua;
	}
	if((Usyn.Mod>*RS_pn25VUL)||(Usyn.Mod<*RS_pn25VLL)||(U1.Mod>*RS_pn25VUL)||(U1.Mod<*RS_pn25VLL))
	{
		Rel25.bDVOutLimitFlag = TRUE;
	}
	else
	{
		Rel25.bDVOutLimitFlag = FALSE;
	}
	
	m_DU.Real = U1.Real - Usyn.Real;
	m_DU.Imag = U1.Imag - Usyn.Imag;
	CaluModValue(&m_DU);
	m_DV = m_DU.Mod;

	//两次保护任务计算一次频率，减轻CPU负荷
	if(!caluFlag)
	{
		CalcImmeFrequence(dwADDataWritePtr,Usyn.Mod,m_dwFreqMask1,m_wFreqBuf,m_FreqCaluBuf,&m_dwFreq1);		
		caluFlag = TRUE;
	}
	else
	{		
		CalcImmeFrequence(dwADDataWritePtr,U1.Mod,m_dwFreqMask2,m_wFreqBuf,m_FreqCaluBuf,&m_dwFreq2);		
		caluFlag = FALSE;
	}

	
	//ReportMsg("F1=%d     F2=%d     cnt0=%d,cnt1=%d,cnt2=%d  ,dwRelayMSCnt = %d",m_dwFreq1,m_dwFreq2,cnt0,cnt1,cnt2,dwRelayMSCnt);
	DB_WriteAI(M_F1,m_dwFreq1);	
	DB_WriteAI(M_F2,m_dwFreq2);
	
	m_DF = m_dwFreq2 - m_dwFreq1;
	
	if(m_DF<=0)//计算角差，关键是看二者的相对运动方向
	{
		m_DA = U1.Angle - Usyn.Angle;
		if(m_DA<0)
			m_DA = 36000+m_DA;
	}
	else
	{
		m_DA = U1.Angle - Usyn.Angle;
		if(m_DA>=0)
			m_DA = 36000-m_DA;
		else
			m_DA = (-1)*m_DA;
	}
	m_DF = ToAbs(m_DF);

	if(m_DA>18000)
	{
		m_DQ = 36000-*Rel25.pDA;		
	}
	else
	{
		m_DQ = m_DA;
	}


	//写入保护测量值
	*RM_pnF1= m_dwFreq1;                                       
	DB_LogWriteRM(RM_F1, *RM_pnF1);	
	*RM_pnF2= m_dwFreq2;                                       
	DB_LogWriteRM(RM_F2, *RM_pnF2);		
	*RM_pnDiffU= m_DV;                                       
	DB_LogWriteRM(RM_DIFFU, *RM_pnDiffU);	
	*RM_pnDiffA= m_DA;                                       
	DB_LogWriteRM(RM_DIFFA, *RM_pnDiffA);	
	*RM_pnDiffdq = m_DQ;
	DB_LogWriteRM(RM_DIFFDQ, *RM_pnDiffdq);
	
	//置检无压继电器字
	if((Ua.Mod<*RS_pn25_27)&&(Ub.Mod<*RS_pn25_27)&&(Uc.Mod<*RS_pn25_27))
		WriteRelayWord(RW_25_27UM,RELAY_ON);
	else
		WriteRelayWord(RW_25_27UM,RELAY_OFF);

	if((U1.Mod<*RS_pn25_27)&&(U2.Mod<*RS_pn25_27)&&(U3.Mod<*RS_pn25_27))
		WriteRelayWord(RW_25_27UL,RELAY_ON);
	else
		WriteRelayWord(RW_25_27UL,RELAY_OFF);
}

void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&R50I1;
	StructCheck[0].bigsize = sizeof(R50I1)/2 - R50I1.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&R50I2;
	StructCheck[1].bigsize = sizeof(R50I2)/2 - R50I2.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&R50I3;
	StructCheck[2].bigsize = sizeof(R50I3)/2 - R50I3.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&R50N1;
	StructCheck[3].bigsize = sizeof(R50N1)/2 - R50N1.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&R50N2;
	StructCheck[4].bigsize = sizeof(R50N2)/2 - R50N2.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&R27U;
	StructCheck[5].bigsize = sizeof(R27U)/2 - R27U.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel79;
	StructCheck[6].bigsize = sizeof(Rel79)/2 - Rel79.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel25;
	StructCheck[7].bigsize = sizeof(Rel25)/2 - Rel25.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&R50I0;
	StructCheck[8].bigsize = sizeof(R50I0)/2 - R50I0.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&R50IOL;
	StructCheck[9].bigsize = sizeof(R50IOL)/2 - R50IOL.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&R50ISH;
	StructCheck[10].bigsize = sizeof(R50ISH)/2 - R50ISH.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[11].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
// 名称: 初始化电流速断保护元件                                                                    
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I0()
{
	R50I0.pElecSet				= RS_pn50P0C;          						//过量保护电量定值 
	R50I0.pTimeSet				= RS_pn50P0D;				 				//过量保护时间定值 
	R50I0.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50I0.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50I0.pAccActTimeSet		= NULL;						//过量保护后加速动作时间定值 
	R50I0.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50I0.pRelCFG				= CFG_pby50I0;							//过量保护功能控制字--1:投入，2:退出  
	R50I0.pRelSW				= SW_pby50I0;         					//过量保护软压板------1:投入，2:退出  
	R50I0.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50I0.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50I0.pRelPDCFG				= CFG_pby50I0_67;		    				//功率方向功能控制位--1:投入，2:退出 
	R50I0.pRelFDCFG				= CFG_pby50I0_PD;					//
	R50I0.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50I0.pRelAccCFG			= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R50I0.pRel47UCFG			= CFG_pby50I0_47;					//复压闭锁功能
	R50I0.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	
	R50I0.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50I0.pRelElec 				= &Ip;						//当前电量指针 		
	R50I0.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50I0.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50I0.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50I0.StartRelayWord		= RW_PI50I0;							//过量保护入段继电器字 
	R50I0.ActRelayWord			= RW_TR50I0;							//过量保护动作继电器字 
	R50I0.AccRelayWord			= FALSE1;							//后加速允许继电器字,是一个时间继电器字，延时到后自动复归 
	R50I0.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R50I0.PForwardRelayWord		= RW_APF;						//功率正方向继电器字 
	R50I0.PRewardRelayWord  	= RW_APR;
	R50I0.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	R50I0.R47ULKRelayWord		= RW_LK50I0_47U;
	R50I0.LockRelayWord			= RW_LKPI50I0;							//入段闭锁继电器字 
	R50I0.RSTRelayWord			= RW_LKRST50I0;							//返回闭锁继电器字 
	R50I0.LoopCtrlRelayWord		= RW_LPC50I0;						//保护循环控制继电器字 
	
	R50I0.wReportNum			= R_50I0;								//过量保护报告序号 
	R50I0.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R50I0.dwK35					= NULL;
	R50I0.RptSerialNum			= &RptSerialNumOfAct;
	R50I0.bMakeActRpt			= TRUE;
	R50I0.wPickUpReportNum		= R_50I0_PI;						//入段报告序列号
	R50I0.wResetReportNum		= R_50I0_RES;						//返回报告序列号
	R50I0.wAccPickUpRptNum		= 0xFFFF;							//加速入段报告序列号
	R50I0.wAccResetRptNum		= 0xFFFF;							//加速返回报告序列号
	//**********************************************************************	
//	R50I0.dwRelTimer			= 0;			 				//定时器 
	R50I0.byRelFCounter			= 0;						//故障计数器 
	R50I0.byRelRCounter			= 0;						//返回计数器 	
	R50I0.wExcepWordNum			= 8;
	R50I0.wInfo 				= 0;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流I段保护元件	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I1()
{
	R50I1.pElecSet				= RS_pn50P1C;          						//过量保护电量定值 
	R50I1.pTimeSet				= RS_pn50P1D;				 				//过量保护时间定值 
	R50I1.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50I1.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50I1.pAccActTimeSet		= RS_pn94D;						//过量保护后加速动作时间定值 
	R50I1.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50I1.pRelCFG				= CFG_pby50I1;							//过量保护功能控制字--1:投入，2:退出  
	R50I1.pRelSW				= SW_pby50I1;         					//过量保护软压板------1:投入，2:退出  
	R50I1.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50I1.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50I1.pRelPDCFG				= CFG_pby50I1_67;		    				//功率方向功能控制位--1:投入，2:退出 
	R50I1.pRelFDCFG				= CFG_pby50I1_PD;					//
	R50I1.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50I1.pRelAccCFG			= CFG_pby50I1_94;							//有无后加速功能------1:有，  2:无 
	R50I1.pRel47UCFG			= CFG_pby50I1_47;					//复压闭锁功能
	R50I1.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	
	R50I1.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50I1.pRelElec 				= &Ip;						//当前电量指针 		
	R50I1.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50I1.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50I1.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50I1.StartRelayWord		= RW_PI50I1;							//过量保护入段继电器字 
	R50I1.ActRelayWord			= RW_TR50I1;							//过量保护动作继电器字 
	R50I1.AccRelayWord			= RW_TSOTF;							//后加速允许继电器字,是一个时间继电器字，延时到后自动复归 
	R50I1.AccActRelayWord		= RW_TR50I1_94;						//后加速动作继电器字 
	R50I1.PForwardRelayWord		= RW_APF;						//功率正方向继电器字 
	R50I1.PRewardRelayWord  	= RW_APR;
	R50I1.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	R50I1.R47ULKRelayWord		= RW_LK50I1_47U;
	R50I1.LockRelayWord			= RW_LKPI50I1;							//入段闭锁继电器字 
	R50I1.RSTRelayWord			= RW_LKRST50I1;							//返回闭锁继电器字 
	R50I1.LoopCtrlRelayWord		= RW_LPC50I1;						//保护循环控制继电器字 
	
	R50I1.wReportNum			= R_50I1;								//过量保护报告序号 
	R50I1.wAccReportNum			= R_50I1_94;							//加速动作报告序号 
	
	R50I1.dwK35					= NULL;
	R50I1.RptSerialNum			= &RptSerialNumOfAct;
	R50I1.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50I1.wPickUpReportNum		= R_50I1_PI;						//入段报告序列号
	R50I1.wResetReportNum		= R_50I1_RES;						//返回报告序列号
	R50I1.wAccPickUpRptNum		= R_50I1_94_PI;						//加速入段报告序列号
	R50I1.wAccResetRptNum		= R_50I1_94_RES;					//加速返回报告序列号
	//**********************************************************************	
//	R50I1.dwRelTimer			= 0;			 				//定时器 
	R50I1.byRelFCounter			= 0;						//故障计数器 
	R50I1.byRelRCounter			= 0;						//返回计数器 	
	R50I1.wExcepWordNum			= 8;
	R50I1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流II段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitR50I2()
{	
	R50I2.pElecSet				= RS_pn50P2C;          						//过量保护电量定值 
	R50I2.pTimeSet				= RS_pn50P2D;				 				//过量保护时间定值 
	R50I2.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50I2.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50I2.pAccActTimeSet		= RS_pn94D;						//过量保护后加速动作时间定值 
	R50I2.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50I2.pRelCFG				= CFG_pby50I2;							//过量保护功能控制字--1:投入，2:退出  
	R50I2.pRelSW				= SW_pby50I2;         					//过量保护软压板------1:投入，2:退出  
	R50I2.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50I2.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50I2.pRelPDCFG				= CFG_pby50I2_67;		    				//功率方向功能控制位--1:投入，2:退出 
	R50I2.pRelFDCFG				= CFG_pby50I2_PD;
	R50I2.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50I2.pRelAccCFG			= CFG_pby50I2_94;							//有无后加速功能------1:有，  2:无 
	R50I2.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R50I2.pRel47UCFG			= CFG_pby50I2_47;
	R50I2.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50I2.pRelElec 				= &Ip;						//当前电量指针 		
	R50I2.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50I2.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50I2.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50I2.StartRelayWord		= RW_PI50I2;							//过量保护入段继电器字 
	R50I2.ActRelayWord			= RW_TR50I2;							//过量保护动作继电器字 
	R50I2.AccRelayWord			= RW_TSOTF;							//后加速允许继电器字 
	R50I2.AccActRelayWord		= RW_TR50I2_94;						//后加速动作继电器字 
	R50I2.PForwardRelayWord		= RW_APF;						//功率正方向继电器字 
	R50I2.PRewardRelayWord  	= RW_APR;					//功率反方向继电器字 
	R50I2.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R50I2.R47ULKRelayWord		= RW_LK50I2_47U;
	R50I2.LockRelayWord			= RW_LKPI50I2;							//入段闭锁继电器字 
	R50I2.RSTRelayWord			= RW_LKRST50I2;							//返回闭锁继电器字 
	R50I2.LoopCtrlRelayWord		= RW_LPC50I2;						//保护循环控制继电器字 
	
	R50I2.wReportNum			= R_50I2;								//过量保护报告序号 
	R50I2.wAccReportNum			= R_50I2_94;							//加速动作报告序号 
	
	R50I2.dwK35					= NULL;
	R50I2.RptSerialNum			= &RptSerialNumOfAct;
	R50I2.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50I2.wPickUpReportNum		= R_50I2_PI;						//入段报告序列号
	R50I2.wResetReportNum		= R_50I2_RES;						//返回报告序列号
	R50I2.wAccPickUpRptNum		= R_50I2_94_PI;						//加速入段报告序列号
	R50I2.wAccResetRptNum		= R_50I2_94_RES;					//加速返回报告序列号
	//**********************************************************************	

//	R50I2.dwRelTimer			= 0;			 				//定时器 
	R50I2.byRelFCounter			= 0;						//故障计数器 
	R50I2.byRelRCounter			= 0;						//返回计数器 
	R50I2.wExcepWordNum			= 8;
	R50I2.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流III段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50I3()
{	
	R50I3.pElecSet				= RS_pn50P3C;          						//过量保护电量定值 
	R50I3.pTimeSet				= RS_pn50P3D;				 				//过量保护时间定值 
	R50I3.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50I3.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50I3.pAccActTimeSet		= RS_pn94D;						//过量保护后加速动作时间定值 
	R50I3.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50I3.pRelCFG				= CFG_pby50I3;							//过量保护功能控制字--1:投入，2:退出  
	R50I3.pRelSW				= SW_pby50I3;         					//过量保护软压板------1:投入，2:退出  
	R50I3.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50I3.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50I3.pRelPDCFG				= CFG_pby50I3_67;		    				//功率方向功能控制位--1:投入，2:退出 
	R50I3.pRelFDCFG				= CFG_pby50I3_PD;
	R50I3.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50I3.pRelAccCFG			= CFG_pby50I3_94;							//有无后加速功能------1:有，  2:无 
	R50I3.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R50I3.pRel47UCFG			= CFG_pby50I3_47;
	R50I3.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50I3.pRelElec 				= &Ip;						//当前电量指针 		
	R50I3.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50I3.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50I3.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50I3.StartRelayWord		= RW_PI50I3;							//过量保护入段继电器字 
	R50I3.ActRelayWord			= RW_TR50I3;							//过量保护动作继电器字 
	R50I3.AccRelayWord			= RW_TSOTF;							//后加速允许继电器字 
	R50I3.AccActRelayWord		= RW_TR50I3_94;						//后加速动作继电器字 
	R50I3.PForwardRelayWord		= RW_APF;						//功率正方向继电器字 
	R50I3.PRewardRelayWord  	= RW_APR;					//功率反方向继电器字 
	R50I3.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R50I3.R47ULKRelayWord		= RW_LK50I3_47U;
	R50I3.LockRelayWord			= RW_LKPI50I3;							//入段闭锁继电器字 
	R50I3.RSTRelayWord			= RW_LKRST50I3;							//返回闭锁继电器字 
	R50I3.LoopCtrlRelayWord		= RW_LPC50I3;						//保护循环控制继电器字 
	
	R50I3.wReportNum			= R_50I3;								//过量保护报告序号 
	R50I3.wAccReportNum			= R_50I3_94;							//加速动作报告序号 
	
	R50I3.dwK35					= NULL;
	R50I3.RptSerialNum			= &RptSerialNumOfAct;
	R50I3.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50I3.wPickUpReportNum		= R_50I3_PI;						//入段报告序列号
	R50I3.wResetReportNum		= R_50I3_RES;						//返回报告序列号
	R50I3.wAccPickUpRptNum		= R_50I3_94_PI;						//加速入段报告序列号
	R50I3.wAccResetRptNum		= R_50I3_94_RES;					//加速返回报告序列号
	//**********************************************************************	

//	R50I3.dwRelTimer			= 0;			 				//定时器 
	R50I3.byRelFCounter			= 0;						//故障计数器 
	R50I3.byRelRCounter			= 0;						//返回计数器 
	R50I3.wExcepWordNum			= 8;
	R50I3.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流III段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50IOL()
{	
	R50IOL.pElecSet				= RS_pn50OLC;          						//过量保护电量定值 
	R50IOL.pTimeSet				= RS_pn50OLD;				 				//过量保护时间定值 
	R50IOL.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50IOL.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50IOL.pAccActTimeSet		= NULL;						//过量保护后加速动作时间定值 
	R50IOL.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50IOL.pRelCFG				= CFG_pby50IOL;							//过量保护功能控制字--1:投入，2:退出  
	R50IOL.pRelSW				= SW_pby50IOL;         					//过量保护软压板------1:投入，2:退出  
	R50IOL.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50IOL.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50IOL.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R50IOL.pRelFDCFG			= &RelCFGCLR;
	R50IOL.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50IOL.pRelAccCFG			= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R50IOL.pRelFLCFG			= &RelCFGCLR;				//故障测距投入 
	R50IOL.pRel47UCFG			= &RelCFGCLR;
	R50IOL.pSysTimer			= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50IOL.pRelElec 				= &Ip;						//当前电量指针 		
	R50IOL.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50IOL.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50IOL.pRelVolt					= NULL;						//当前用来闭锁的电压的指针 

	R50IOL.StartRelayWord		= RW_PI50IOL;							//过量保护入段继电器字 
	R50IOL.ActRelayWord			= RW_TR50IOL;							//过量保护动作继电器字 
	R50IOL.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R50IOL.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R50IOL.PForwardRelayWord	= FALSE1;						//功率正方向继电器字 
	R50IOL.PRewardRelayWord  	= FALSE1;					//功率反方向继电器字 
	R50IOL.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R50IOL.R47ULKRelayWord		= FALSE1;
	R50IOL.LockRelayWord		= RW_LKPI50IOL;							//入段闭锁继电器字 
	R50IOL.RSTRelayWord			= RW_LKRST50IOL;							//返回闭锁继电器字 
	R50IOL.LoopCtrlRelayWord	= RW_LPC50IOL;						//保护循环控制继电器字 
	
	R50IOL.wReportNum			= R_50IOL;								//过量保护报告序号 
	R50IOL.wAccReportNum		= 0xFFFF;							//加速动作报告序号 
	
	R50IOL.dwK35				= NULL;
	R50IOL.RptSerialNum			= &RptSerialNumOfAct;
	R50IOL.bMakeActRpt			= FALSE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50IOL.wPickUpReportNum		= 0xFFFF;						//入段报告序列号
	R50IOL.wResetReportNum		= 0xFFFF;						//返回报告序列号
	R50IOL.wAccPickUpRptNum		= 0xFFFF;						//加速入段报告序列号
	R50IOL.wAccResetRptNum		= 0xFFFF;						//加速返回报告序列号
	//**********************************************************************	

//	R50IOL.dwRelTimer			= 0;			 				//定时器 
	R50IOL.byRelFCounter			= 0;						//故障计数器 
	R50IOL.byRelRCounter			= 0;						//返回计数器 
	R50IOL.wExcepWordNum			= 8;
	R50IOL.wInfo 				= 0;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化零序过流保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50N1()
{	
	R50N1.pElecSet				= RS_pn50N1C;          						//过量保护电量定值 
	R50N1.pTimeSet				= RS_pn50N1D;				 				//过量保护时间定值 
	R50N1.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50N1.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50N1.pAccActTimeSet		= NULL;						//过量保护后加速动作时间定值 
	R50N1.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50N1.pRelCFG				= CFG_pby50N1;							//过量保护功能控制字--1:投入，2:退出  
	R50N1.pRelSW				= SW_pby50N1;         					//过量保护软压板------1:投入，2:退出  
	R50N1.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50N1.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50N1.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R50N1.pRelFDCFG				= &RelCFGCLR;
	R50N1.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50N1.pRelAccCFG			= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R50N1.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R50N1.pRel47UCFG			= &RelCFGCLR;
	R50N1.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50N1.pRelElec 				= &I0;						//当前电量指针 		
	R50N1.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50N1.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50N1.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50N1.StartRelayWord		= RW_PI50N1;							//过量保护入段继电器字 
	R50N1.ActRelayWord			= RW_TR50N1;							//过量保护动作继电器字 
	R50N1.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R50N1.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R50N1.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R50N1.PRewardRelayWord  	= FALSE1;					//功率反方向继电器字 
	R50N1.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R50N1.R47ULKRelayWord		= FALSE1;
	R50N1.LockRelayWord			= RW_LKPI50N1;							//入段闭锁继电器字 
	R50N1.RSTRelayWord			= RW_LKRST50N1;							//返回闭锁继电器字 
	R50N1.LoopCtrlRelayWord		= RW_LPC50N1;						//保护循环控制继电器字 
	
	R50N1.wReportNum			= R_50N1;								//过量保护报告序号 
	R50N1.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R50N1.dwK35					= NULL;
	R50N1.RptSerialNum			= &RptSerialNumOfAct;
	R50N1.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50N1.wPickUpReportNum		= R_50N1_PI;						//入段报告序列号
	R50N1.wResetReportNum		= R_50N1_RES;						//返回报告序列号
	R50N1.wAccPickUpRptNum		= 0xFFFF;							//加速入段报告序列号
	R50N1.wAccResetRptNum		= 0xFFFF;							//加速返回报告序列号
	//**********************************************************************	

//	R50N1.dwRelTimer			= 0;			 				//定时器 
	R50N1.byRelFCounter			= 0;						//故障计数器 
	R50N1.byRelRCounter			= 0;						//返回计数器 
	R50N1.wExcepWordNum			= 8;
	R50N1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化零序过流保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitR50N2()
{	
	R50N2.pElecSet				= RS_pn50N2C;          						//过量保护电量定值 
	R50N2.pTimeSet				= RS_pn50N2D;				 				//过量保护时间定值 
	R50N2.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数 
	R50N2.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	R50N2.pAccActTimeSet		= NULL;						//过量保护后加速动作时间定值 
	R50N2.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50N2.pRelCFG				= CFG_pby50N2;							//过量保护功能控制字--1:投入，2:退出  
	R50N2.pRelSW				= SW_pby50N2;         					//过量保护软压板------1:投入，2:退出  
	R50N2.pRelHLockCFG			= &RelCFGCLR;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	R50N2.pRelHSuprCFG			= &RelCFGCLR; 						//谐波抑制功能控制位--1:投入，2:退出 
	R50N2.pRelPDCFG				= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	R50N2.pRelFDCFG				= &RelCFGCLR;
	R50N2.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50N2.pRelAccCFG			= &RelCFGCLR;							//有无后加速功能------1:有，  2:无 
	R50N2.pRelFLCFG				= &RelCFGCLR;				//故障测距投入 
	R50N2.pRel47UCFG			= &RelCFGCLR;
	R50N2.pSysTimer				= &dwRelayMSCnt;								//系统定时器指针 	
	
	R50N2.pRelElec 				= &I0;						//当前电量指针 		
	R50N2.pRelHarm3				= NULL;						//当前三次谐波指针 	
	R50N2.pRelHarm5				= NULL;						//当前五次谐波指针 	
	R50N2.pRelVolt				= NULL;						//当前用来闭锁的电压的指针 

	R50N2.StartRelayWord		= RW_PI50N2;							//过量保护入段继电器字 
	R50N2.ActRelayWord			= RW_TR50N2;							//过量保护动作继电器字 
	R50N2.AccRelayWord			= FALSE1;							//后加速允许继电器字 
	R50N2.AccActRelayWord		= FALSE1;						//后加速动作继电器字 
	R50N2.PForwardRelayWord		= FALSE1;						//功率正方向继电器字 
	R50N2.PRewardRelayWord  	= FALSE1;					//功率反方向继电器字 
	R50N2.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	
	R50N2.R47ULKRelayWord		= FALSE1;
	R50N2.LockRelayWord			= RW_LKPI50N2;							//入段闭锁继电器字 
	R50N2.RSTRelayWord			= RW_LKRST50N2;							//返回闭锁继电器字 
	R50N2.LoopCtrlRelayWord		= RW_LPC50N2;						//保护循环控制继电器字 
	
	R50N2.wReportNum			= R_50N2;								//过量保护报告序号 
	R50N2.wAccReportNum			= 0xFFFF;							//加速动作报告序号 
	
	R50N2.dwK35					= NULL;
	R50N2.RptSerialNum			= &RptSerialNumOfAct;
	R50N2.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	R50N2.wPickUpReportNum		= R_50N2_PI;						//入段报告序列号
	R50N2.wResetReportNum		= R_50N2_RES;						//返回报告序列号
	R50N2.wAccPickUpRptNum		= 0xFFFF;							//加速入段报告序列号
	R50N2.wAccResetRptNum		= 0xFFFF;							//加速返回报告序列号
	//**********************************************************************	

//	R50N2.dwRelTimer			= 0;			 				//定时器 
	R50N2.byRelFCounter			= 0;						//故障计数器 
	R50N2.byRelRCounter			= 0;						//返回计数器 
	R50N2.wExcepWordNum			= 8;
	R50N2.wInfo 				= 0;
}

/********************************************************************************
*功能: 初始化母充保护元件
*参数: 无
*返回: 无
*备注:
**********************************************************************************/
void CRelayTask::InitR50ISH()
{
	R50ISH.pElecSet				= RS_pn50ISHC;								//过量保护电量定值 
	R50ISH.pTimeSet				= RS_pn50ISHD;								//过量保护时间定值 
	R50ISH.pHarmKhr1Set			= NULL; 						//过量保护谐波加权系数 
	R50ISH.pVolLockSet			= NULL; 						//过量保护电压闭锁定值 
	R50ISH.pAccActTimeSet		= RS_pn50ISHD; 					//过量保护后加速动作时间定值 
	R50ISH.pH2Coef				= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	R50ISH.pRelCFG				= CFG_pby50I_SH;							//过量保护功能控制字--1:投入，2:退出  
	R50ISH.pRelSW				= SW_pby50ISH;							//过量保护软压板------1:投入，2:退出  
	R50ISH.pRelHLockCFG			= &RelCFGCLR;							//谐波闭锁功能控制位--1:投入，2:退出 
	R50ISH.pRelHSuprCFG			= &RelCFGCLR;						//谐波抑制功能控制位--1:投入，2:退出 
	R50ISH.pRelPDCFG 			= &RelCFGCLR;							//功率方向功能控制位--1:投入，2:退出 
	R50ISH.pRelFDCFG 			= &RelCFGCLR;					//
	R50ISH.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	R50ISH.pRelAccCFG			= CFG_pby50I_SH;							//有无后加速功能------1:有，  2:无 
	R50ISH.pRel47UCFG			= &RelCFGCLR;					//复压闭锁功能
	R50ISH.pRelFLCFG 			= &RelCFGCLR;				//故障测距投入 
	R50ISH.pSysTimer 			= &dwRelayMSCnt;								//系统定时器指针	
	
	R50ISH.pRelElec				= &Ip;						//当前电量指针		
	R50ISH.pRelHarm3 			= NULL; 					//当前三次谐波指针	
	R50ISH.pRelHarm5 			= NULL; 					//当前五次谐波指针	
	R50ISH.pRelVolt				= NULL; 					//当前用来闭锁的电压的指针 

	R50ISH.StartRelayWord		= RW_PI50ISH;							//过量保护入段继电器字 
	R50ISH.ActRelayWord			= RW_TR50ISH;							//过量保护动作继电器字 
	R50ISH.AccRelayWord			= RW_TSH;							//后加速允许继电器字,是一个时间继电器字，延时到后自动复归 
	R50ISH.AccActRelayWord		= RW_TR50ISH;						//后加速动作继电器字 
	R50ISH.PForwardRelayWord 	= FALSE1;						//功率正方向继电器字 
	R50ISH.PRewardRelayWord		= FALSE1;
	R50ISH.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字 
	R50ISH.R47ULKRelayWord		= FALSE1;
	R50ISH.LockRelayWord 		= RW_LKPI50ISH;							//入段闭锁继电器字 
	R50ISH.RSTRelayWord			= RW_LKRST50ISH; 						//返回闭锁继电器字 
	R50ISH.LoopCtrlRelayWord 	= RW_LPC50ISH;						//保护循环控制继电器字 
	
	R50ISH.wReportNum			= R_50ISH;								//过量保护报告序号 
	R50ISH.wAccReportNum 		= R_50ISH;							//加速动作报告序号 
	
	R50ISH.dwK35 				= NULL;
	R50ISH.RptSerialNum			= &RptSerialNumOfAct;
	R50ISH.bMakeActRpt			= TRUE;
	R50ISH.wPickUpReportNum		= R_50ISH_PI;						//入段报告序列号
	R50ISH.wResetReportNum		= R_50ISH_RES;						//返回报告序列号
	R50ISH.wAccPickUpRptNum		= R_50ISH_PI;							//加速入段报告序列号
	R50ISH.wAccResetRptNum		= R_50ISH_RES;							//加速返回报告序列号
	//**********************************************************************	
//	R50ISH.dwRelTimer			= 0;							//定时器 
	R50ISH.byRelFCounter 		= 0;						//故障计数器 
	R50ISH.byRelRCounter 		= 0;						//返回计数器	
	R50ISH.wExcepWordNum 		= 8;
	R50ISH.wInfo 				= 0;
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
	R27U.pLockCurSet					= RS_pn27_LK37;					//闭锁电流定值 

	R27U.pRelCFG						= CFG_pby27U;					  	 //欠量保护配置 
	R27U.pRelSW							= SW_pby27;					  	 //欠量保护软压板 
	R27U.pRelAlarmCFG					= &RelCFGCLR;						//欠量保护投跳闸 
	R27U.pRelCurLockCFG					= &RelCFGSET;
	R27U.pSysTimer						= &dwRelayMSCnt;						//系统定时器指针 

	R27U.pRelElec						= &Up;						//当前的电量指针 
	R27U.pRelLockElec					= &Ip;					//闭锁电流 
	R27U.ActRelayWord					= RW_TR27U;					//欠量保护动作继电器字 
	R27U.StartRelayWord					= RW_PI27U;					//欠量保护入段继电器字 
	R27U.AlarmRelayWord					= FALSE1;					//欠量保护告警继电器字 
	R27U.LockRelayWord					= RW_LKPI27;				//入段闭锁继电器字 
	R27U.RSTRelayWord					= RW_LKRST27;					//故障返回闭锁继电器字 
	R27U.LoopCtrlRelayWord				= RW_LPC27U;				//故障循环控制继电器字 
	R27U.wR52BFRealyword				= RW_52BF;					//断路器跳闸失败继电器字
	R27U.wActRptNum						= R_27TR;					//欠量保护动作报告序号 
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
*功能: 初始化重合闸保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRel79()
{
	Rel79.R79NumSet						= RS_pn79SH;						//投入的重合闸次数 
	Rel79.R79T1Set						= RS_pn79D1;							//第一次重合闸动作时间 
	Rel79.R79T2Set						= RS_pn79D2;							//第二次重合闸动作时间 
	Rel79.R79RSTSet						= RS_pn79CT;							//重合闸充放电时间 
	Rel79.R79SOTFTSet					= RS_pnSOTFD;						//合于故障检测延时，即为后加速投入时间 
	Rel79.R79PISTSet					= AS_pnTCB;						//重合闸启动监视延时，即为断路器动作延时 
	Rel79.R79CLSTSet					= RS_pn79CLSD;						//重合闸动作监视延时 

	
	Rel79.R79SCFG						= CFG_pby79UC;	//偷跳重合闸配置
	Rel79.R79SW							= SW_pby79;
	Rel79.R79RW							= RW_CFG79;	//	把配置放在重合闸闭锁继电器字里

	Rel79.R79CYSRW						= RW_79CYS;					//重合闸周期状态继电器字--自保持继电器字 
	Rel79.R79RSSRW						= RW_79RSS;					//重合闸复归状态继电器字--自保持继电器字 
	Rel79.R79LOSRW						= RW_79LOS;					//重合闸闭锁状态继电器字--自保持继电器字 

	Rel79.R79SH0RW						= RW_79SH0;					//重合闸动作状态继电器字一--输入继电器字 		
	Rel79.R79SH1RW						= RW_79SH1;					//重合闸动作状态继电器字二--输入继电器字 

	Rel79.R79PIRW						= RW_79PI;					//重合闸启动逻辑继电器字--自保持继电器字 
	Rel79.R79PISRW						= RW_79PIS;					//重合闸启动监视逻辑继电器字--中间继电器字 
	Rel79.R79TORW						= RW_79TO;					//重合闸延时到逻辑继电器字--输入继电器字 
	Rel79.R79CLSRW						= RW_79CLS;					//重合闸动作监视逻辑继电器字--中间继电器字 
	Rel79.R79CLRW						= RW_79CL;					//重合闸动作逻辑继电器字--自保持继电器字 
	Rel79.R79CLCRW						= RW_79CLC;					//重合闸动作命令撤销逻辑继电器字--中间继电器字 
	Rel79.R79LOCKRW						= RW_79LK;					//重合闸闭锁逻辑继电器字--中间继电器字 
	Rel79.R79SOTFRW						= RW_TSOTF;					//重合闸合于故障逻辑继电器字--输入继电器字 
	Rel79.R79TRCHECK					= RW_79TRCK;				//重合闸启动条件判断继电器字

	Rel79.R7952A						= RW_52A;						//重合闸断路器合位继电器字 
	Rel79.R7952B						= RW_52B;						//重合闸断路器分位继电器字 
	Rel79.dwSysTimer					= &dwRelayMSCnt;;				//系统定时器指针 
	Rel79.wRpt79_1						= R_R79_1;					//一次重合闸动作报告号 
	Rel79.wRpt79_2						= R_R79_2;					//二次重合闸动作报告号 
	Rel79.wRpt79Fail					= R_FA_79;				//重合闸失败报告
	Rel79.wRpt79LK_1					= R_R79LK_1;				//重合闸被闭锁报告序号
	Rel79.wRpt79LK_2					= R_R79LK_2;				//重合闸被闭锁报告序号
	Rel79.wRpt79_1_SUCC					= R_R79SUCC_1;			//一次重合闸成功报告  by yanzh 2013年6月18日 17:31:57
	Rel79.wRpt79_1_FAIL					= R_R79FAIL_1;			//一次重合闸失败报告
	Rel79.wRpt79_2_SUCC					= R_R79SUCC_2;			//二次重合闸成功报告
	Rel79.wRpt79_2_FAIL					= R_R79FAIL_2;			//二次重合闸失败报告
	Rel79.wRpt79PI_1					= R_R79PI_1;				//重合闸启动报告	
	Rel79.wRpt79PI_2					= R_R79PI_2;				//重合闸启动报告
	Rel79.wRpt79NPI_1					= R_R79NPI_1;				//重合闸未启动报告
	Rel79.wRpt79NPI_2					= R_R79NPI_2;				//重合闸未启动报告	
	Rel79.wPickUpRptNum					= R_ACTPICK;

	Rel79.RptSerialNum					= &RptSerialNumOfAct;
	Rel79.SysRptSerialNum				= &g_RptSerialNum;
	Rel79.pFaultOnFlag					= &FaultOnFlag;	
	Rel79.pR79PIRptFlag 				= &m_bR79PickUpRpt;

	
	Rel79.R79RSFLAG						= FALSE;					//重合闸充放电延时启动标志 
	Rel79.R79PISFLAG					= FALSE;					//重合闸启动监视延时启动标志 
	Rel79.R79TFLAG						= FALSE;					//重合闸动作延时启动标志 
	Rel79.R79CLSFLAG					= FALSE;					//重合闸动作监视延时启动标志 
	Rel79.R79LKFlag						= FALSE;					//重合闸被闭锁报告标志	
	Rel79.R79CLFlag						= FALSE;					//重合闸动作标志
//	Rel79.R79Timer						= 0;					//重合闸动作延时定时器 
	Rel79.R79ShotCnt					= 0;					//重合闸已经动作次数 	
	Rel79.wExcepWordNum					= 16;				//本结构对象计算CRC校验时需排除在外的字节数，本结构为12 

}
void CRelayTask::InitRel25(void)
{
	Rel25.p25_27Set                     = RS_pn25_27;  //合闸检无压
	Rel25.p25DTSet                      = RS_pn25DT;   //同期检测时间
	Rel25.p25VULSet                     = RS_pn25VUL;  //检同期电压上限
	Rel25.p25VLLSet                     = RS_pn25VLL;  //检同期电压下限
	Rel25.p25VDSet                      = RS_pn25VD;   //同期允许压差
	Rel25.p25FDSet                      = RS_pn25FD;   //同期允许频差
	Rel25.p25PASet                      = RS_pn25PA;   //同期功角
	Rel25.p25PHSet                      = RS_pn25PH;   //同期相别
	Rel25.p52CLT						= RS_pn52CLT;
	Rel25.p25RCFG                       = CFG_pby25 ;  //手合检同期配置
	Rel25.p79_25RCFG					= CFG_pby79_25;
	Rel25.p25RSW                        = SW_pby25;    //手合检同期软压板

	Rel25.pUsyn							= &Usyn;
	Rel25.pU1							= &U1;
	Rel25.pDA							= &m_DA;		    //角差
	Rel25.pDV							= &m_DV; 	//压差
	Rel25.pDF							= &m_DF;	//频差
	Rel25.pPA							= &m_FA;         //导前角

    Rel25.w52A                          = RW_52A;       //合位继电器字
    Rel25.w25LK                         = RW_25LK;     //检同期闭锁继电器字
    Rel25.w79_25PI                      = RW_79_25PI;      //检同期启动继电器字
    Rel25.wMCL_25PI                     = RW_MCL_25PI;      //检同期启动继电器字
    Rel25.w25ActRW                      = RW_25CL;      //检同期动作继电器字
    Rel25.w25INT						= RW_25INT;

	Rel25.R25SucRptNum					= R_SU_25;	//检同期成功报告序号
	Rel25.R25FalRptNum					= R_FA_25;	//检同期失败报告序号
	Rel25.R25LKRptNum					= R_25LK;	//检同期被闭锁报告序号
	Rel25.R25_27RptNum					= R_WL_CL; //无压空合报告序号
	Rel25.R25INTRptNum					= R_25BK;
	Rel25.RptSerialNum					= &RptSerialNumOfAct;					//系统报告序列号 
	Rel25.pSysTimer						= &dwRelayMSCnt;						//系统定时器 
	
	Rel25.bManSynFlag                    = FALSE;        //手合检同期标志
	Rel25.bJudgeNetend                   = FALSE;        //网判断完成标志
	Rel25.bSameNetFlag                   = FALSE;        //同网标志
	Rel25.bDiffNetFlag                   = FALSE;        //异网标志
	Rel25.bSigleNetFlag                  = FALSE;        //单网标志
	Rel25.bJudgeSameNetPIFlag            = FALSE;        //同网判断延时入段标志
	Rel25.bJudgeDiffNetPIFlag            = FALSE;        //异网判断延时入段标志
	Rel25.bJudgeSigleNetPIFlag           = FALSE;        //单网判断延时入段标志
	Rel25.bBusPTBreakFlag               = FALSE;         //母线PT断线标志
	Rel25.bHaveMakeLKRptFlag			= FALSE;     		//检同期被闭锁报告标志
	Rel25.bHaveMakeINTRptFlag			= FALSE;       		//检同期被中断报告标志
	Rel25.bDVOutLimitFlag				= FALSE;

//	Rel25.dwInitSingleNetChkTimer        = 0;            //单网判断时间定时器
//	Rel25.dwInitSameleNetChkTimer        = 0;            //同网判断时间定时器
//	Rel25.dwInitDiffleNetChkTimer        = 0;            //异网判断时间定时器
//	Rel25.dwSynCheckTime                 = 0;            //检同期时间定时器
//	Rel25.dwSynClosePulseTimer           = 0;            //检同期合闸脉冲继电器字
	Rel25.wExcepWordNum                 = 36;
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
	BYTE  pChannel[]={CHANNEL_6521_IA,CHANNEL_6521_IB,CHANNEL_6521_IC,CHANNEL_6521_I0,CHANNEL_6521_I1};
	if(ReadRelayWord(RW_52BF))  //断路器跳闸失败，不进行保护启动判断处理 
	{
		WriteRelayWord(RW_PICKUP, RELAY_OFF);
		return FALSE;
	}
	
	if((*R50I0.pRelCFG == CFG_STATUS_SET) && (*R50I0.pRelSW == SW_STATUS_SET))   //电流速断保护
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
	if(((*R50I1.pRelCFG == CFG_STATUS_SET)||(*R50I1.pRelAccCFG == CFG_STATUS_SET)) && (*R50I1.pRelSW == SW_STATUS_SET))   //过流1段保护投入 
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
	if(((*R50I2.pRelCFG == CFG_STATUS_SET)||(*R50I2.pRelAccCFG == CFG_STATUS_SET)) && (*R50I2.pRelSW == SW_STATUS_SET))   //过流2段保护投入 
	{		
		haveRelayInput = TRUE;
		if(R50I2.pRelElec->Mod > *R50I2.pElecSet)
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
	if(((*R50I3.pRelCFG == CFG_STATUS_SET)||(*R50I3.pRelAccCFG == CFG_STATUS_SET))&& (*R50I3.pRelSW == SW_STATUS_SET))   //过流3段保护投入 
	{
		haveRelayInput = TRUE;
		if(R50I3.pRelElec->Mod > *R50I3.pElecSet)
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
	if((*R50N1.pRelCFG == CFG_STATUS_SET) && (*R50N1.pRelSW == SW_STATUS_SET))   //零流1段保护投入 
	{
		haveRelayInput = TRUE;
		if(R50N1.pRelElec->Mod > *R50N1.pElecSet)
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
	if((*R50N2.pRelCFG == CFG_STATUS_SET) && (*R50N2.pRelSW == SW_STATUS_SET))   //零流1段保护投入 
	{
		haveRelayInput = TRUE;
		if(R50N2.pRelElec->Mod > *R50N2.pElecSet)
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
	if((*R50ISH.pRelCFG == CFG_STATUS_SET) && (*R50ISH.pRelSW == SW_STATUS_SET))   //母线充电保护投入 
	{
		haveRelayInput = TRUE;
		if(R50ISH.pRelElec->Mod > *R50ISH.pElecSet)
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

		if((*R50I0.pRelPDCFG == CFG_STATUS_SET)||(*R50I1.pRelPDCFG == CFG_STATUS_SET)||(*R50I2.pRelPDCFG == CFG_STATUS_SET)||(*R50I3.pRelPDCFG == CFG_STATUS_SET))
		{
			if(m_Phase == RELAY_PHASE_A)
			{
				if(!m_bPDCheckA)
				{
					if(!ReadRelayWord(RW_ARPT1))				//PT断线情况下，功率方向闭锁功能失效
						m_bPDCheckA = PowerDirCheck_90(&Ubc,&Ia,dwPDDataBuf,*RS_pn67ANG,*CF_pnUp,RW_APF,RW_APR,CHANNEL_6521_UBC,TRUE);
					else
					{						
						WriteRelayWord(RW_APF, RELAY_OFF);
						WriteRelayWord(RW_APR, RELAY_OFF);
					}
				}
			}
			else if(m_Phase == RELAY_PHASE_B)
			{
				if(!m_bPDCheckB)					
				{
					if(!ReadRelayWord(RW_ARPT1))				//PT断线情况下，功率方向闭锁功能失效
						m_bPDCheckB = PowerDirCheck_90(&Uca,&Ib,dwPDDataBuf,*RS_pn67ANG,*CF_pnUp,RW_BPF,RW_BPR,CHANNEL_6521_UCA,TRUE);
					else
					{						
						WriteRelayWord(RW_BPF, RELAY_OFF);
						WriteRelayWord(RW_BPR, RELAY_OFF);
					}
				}
			}
			else if(m_Phase == RELAY_PHASE_C)
			{
				if(!m_bPDCheckC)
				{
					if(!ReadRelayWord(RW_ARPT1))				//PT断线情况下，功率方向闭锁功能失效
						m_bPDCheckC = PowerDirCheck_90(&Uab,&Ic,dwPDDataBuf,*RS_pn67ANG,*CF_pnUp,RW_CPF,RW_CPR,CHANNEL_6521_UAB,TRUE);
					else
					{						
						WriteRelayWord(RW_CPF, RELAY_OFF);
						WriteRelayWord(RW_CPR, RELAY_OFF);
					}
				}
			}			
		}
		GeneralOverValueRelay(&R50I0);				//调用速断子程序
		GeneralOverValueRelay(&R50I1);				//调用速断子程序
		GeneralOverValueRelay(&R50I2);				//调用过流I段保护子程序
		GeneralOverValueRelay(&R50I3);				//调用过流III段保护子程序
		GeneralOverValueRelay(&R50N1);				//调用过流III段保护子程序
		GeneralOverValueRelay(&R50N2);
		GeneralOverValueRelay(&R50ISH);				//母线充电保护子程序
		
		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveMakeTripFailRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//作断路器跳闸失败报告
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}

		if(Rel79.R79TFLAG)						//重合闸已经进入延时状态,等待重合闸动作
			return;
		//重合闸动作继电器字置位，等待电流恢复，若电流恢复，且再一次发生故障，该继电器字被复位
		//若电流未恢复或者恢复后未曾发生动作，则时间到后，该继电器字也被复位
		//若在该时间内电流恢复，重置FaultSINum计数器
		if(ReadRelayWord(Rel79.R79CLRW))  			
		{
			if(CurRecoverFlag)				 ///重合闸动作后电流已经恢复
				return;
			else
			{
				if((!ReadRelayWord(RW_37IA))||(!ReadRelayWord(RW_37IB))||(!ReadRelayWord(RW_37IC)))	//检查有流继电器字是否被置位
				{
					FaultSINum = 0;		//重新初始化故障处理采样间隔计数器
					CurRecoverFlag = TRUE;    				//置电流恢复标志
					return;
				}				
				return;
			}
		}
		// 1。有保护循环控制继电器字被置位，等待保护动作或者退出
		if(ReadRelayWord(R50I0.LoopCtrlRelayWord)||ReadRelayWord(R50I1.LoopCtrlRelayWord)||ReadRelayWord(R50I2.LoopCtrlRelayWord)|| \
			ReadRelayWord(R50I3.LoopCtrlRelayWord)||ReadRelayWord(R50N1.LoopCtrlRelayWord)||ReadRelayWord(R50N2.LoopCtrlRelayWord)|| \
			ReadRelayWord(R50ISH.LoopCtrlRelayWord))
		{			
			// 1.1 保护已经动作，且跳闸失败
			if(ReadRelayWord(RW_52BF))
			{
				WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
				FaultCheckOutFlag = FALSE;					//清检出故障标志
				FaultOnFlag = FALSE;						//清故障循环处理标志
				HaveMakeStartRptFlag= FALSE;				//清已作带故障电量启动报告标志
				m_bIsHaveMakePTRpt = FALSE;					//清已作PT断线闭锁距离保护告警报告标志				
				m_bFaultLKFlag = FALSE; 					//清有启动无动作时启动时刻锁定标志	
				m_bPDCheckA = FALSE;							//清已作带记忆功率方向判断标志
				m_bPDCheckB = FALSE;
				m_bPDCheckC = FALSE;
				CurRecoverFlag = FALSE;						//清重合闸动作后电流恢复标志
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
				if(dwRelayMSCnt -FaultLockTime > 1000 )  //无故障时间超过合于故障检测时间
				{						
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
					CurRecoverFlag	 = FALSE;					//清重合闸动作后电流恢复标志		
					m_bIsHaveMakePTRpt = FALSE;					//清已作PT断线闭锁距离保护告警报告标志					
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					m_bPDCheckA = FALSE;							//清已作带记忆功率方向判断标志
					m_bPDCheckB = FALSE;
					m_bPDCheckC = FALSE;
					
					m_bR79PickUpRpt = FALSE;					
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
				if(dwRelayMSCnt -FaultLockTime>500 )	
				{
					WriteRelayWord(RW_PICKUP, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志						
					CurRecoverFlag	 = FALSE;					//清重合闸动作后电流恢复标志		
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					m_bIsHaveMakePTRpt = FALSE;					//清已作PT断线闭锁距离保护告警报告标志
					m_bPDCheckA = FALSE;							//清已作带记忆功率方向判断标志
					m_bPDCheckB = FALSE;	
					m_bPDCheckC = FALSE;	
					
					m_bR79PickUpRpt = FALSE;					
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
			m_bR79PickUpRpt = FALSE;					
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
	ResetOverValueRelay(&R50I2,resetFlag);
	ResetOverValueRelay(&R50I3,resetFlag);
	ResetOverValueRelay(&R50N1,resetFlag);
	ResetOverValueRelay(&R50N2,resetFlag);
	ResetOverValueRelay(&R50ISH,resetFlag);
}
void CRelayTask::PTCheck()
{	
	if(*CFG_pbyPTBR == CFG_STATUS_CLR) //PT断线未投入
	{
		WriteRelayWord(RW_PIPT1,RELAY_OFF);
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_BUSDX_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT2,RELAY_OFF);		
		if(ReadRelayWord(RW_ARPT2))
		{			
			MakeAlarmRpt(R_LINEDX_RES);
			WriteRelayWord(RW_ARPT2,RELAY_OFF);
		}
		m_BusPTTimer = 0;
		m_LinePTTimer = 0;
		return;
	}
//母线PT断线判断
	if(((ULK.Mod<*RS_pnPT1_V)&&(Ip.Mod>*RS_pnPT1_C))||(NU2.Mod>m_U2Set))
	{
		if(ReadRelayWord(RW_PIPT1))
		{
			if(!ReadRelayWord(RW_ARPT1))
			{
				if(dwRelayMSCnt - m_BusPTTimer>*RS_pnPT1_D)
				{
					WriteRelayWord(RW_ARPT1,RELAY_ON);
					MakeAlarmRpt(R_BUSDX);
				}
			}
		}
		else
		{
			WriteRelayWord(RW_PIPT1,RELAY_ON);
			m_BusPTTimer = dwRelayMSCnt;
		}
	}
	//只有在电压恢复正常时方复归
	if((ULK.Mod>=*RS_pnPT1_V)&&(NU2.Mod<=m_U2Set))
	{
		if(ReadRelayWord(RW_ARPT1))
		{
			MakeAlarmRpt(R_BUSDX_RES);
			WriteRelayWord(RW_ARPT1,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
		m_BusPTTimer = 0;
	}
//	else //只有在电压恢复正常时方复归
//	{
//		WriteRelayWord(RW_ARPT1,RELAY_OFF);
//		WriteRelayWord(RW_PIPT1,RELAY_OFF);		
//		m_BusPTTimer = 0;
//	}

//线路PT断线判断
	TRelElecValPar UlMax,UlMin;
	BOOL	Condition1 = FALSE;
	BOOL	Condition2 = FALSE;
	BOOL	Condition3 = FALSE;
	UlMax = ElecMax(U1,U2,U3);
	UlMin = ElecMin(U1,U2,U3);
	if((UlMin.Mod<*RS_pnPT2_V2)&&((Up.Mod>*RS_pnPT2_V1)&&(Ip.Mod>*RS_pnPT2_C)&&(ReadRelayWord(RW_52A)))) //母线有压有流且开关处于合位
			Condition1 = TRUE;
	if((UlMin.Mod<*RS_pnPT2_V2)&&(UlMax.Mod>*RS_pnPT2_V2))	//线路侧三相电压最大的大于定值，最小的小于定值
		Condition2 = TRUE;
	if(((U1.Mod>*RS_pn25VLL)&&(U2.Mod>*RS_pn25VLL)&&(U3.Mod<*RS_pn25VLL))||    //线路侧的三个电压中有且仅有一个小于检同期电压下限
		((U1.Mod>*RS_pn25VLL)&&(U2.Mod<*RS_pn25VLL)&&(U3.Mod>*RS_pn25VLL))||   
		((U1.Mod<*RS_pn25VLL)&&(U2.Mod>*RS_pn25VLL)&&(U3.Mod>*RS_pn25VLL)))
		Condition3 = TRUE;

	if(Condition1||Condition2||Condition3)
	{
		if(ReadRelayWord(RW_PIPT2))
		{
			if(!ReadRelayWord(RW_ARPT2))
			{
				if(dwRelayMSCnt - m_LinePTTimer>*RS_pnPT2_D)
				{
					WriteRelayWord(RW_ARPT2,RELAY_ON);
					MakeAlarmRpt(R_LINEDX);
				}
			}
		}
		else
		{
			WriteRelayWord(RW_PIPT2,RELAY_ON);
			m_LinePTTimer = dwRelayMSCnt;
		}
	}
	else
	{		
		if(ReadRelayWord(RW_ARPT2))
		{			
			MakeAlarmRpt(R_LINEDX_RES);
			WriteRelayWord(RW_ARPT2,RELAY_OFF);
		}
		WriteRelayWord(RW_PIPT2,RELAY_OFF);
		m_LinePTTimer = 0;
	}
	
}

void CRelayTask::R59NAlarm() //零压告警
{
	static DWORD R59NTimer = 0;
	if(*CFG_pby59N_74 == CFG_STATUS_CLR)
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
					MakeAlarmRpt(R_59NAR);
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

void CRelayTask::Rel25Check(void) //检同期
{
	CaluDF_DA_DV();  //计算同期压差,角差，频差

	if((*Rel25.p25RCFG == CFG_STATUS_CLR)&&(*Rel25.p79_25RCFG== CFG_STATUS_CLR)||(*Rel25.p25RSW == SW_STATUS_CUT)) //检同期未投入
		ResetRel25();

	if(ReadRelayWord(Rel25.w25ActRW))  //检同期动作过
	{
		if(*Rel25.pSysTimer - Rel25.dwSynClosePulseTimer >= 1000)
			ResetRel25();
		return;
	}
	if(ReadRelayWord(Rel25.w79_25PI)||ReadRelayWord(Rel25.wMCL_25PI))
	{		
		if(ReadRelayWord(Rel25.w25LK)) //被闭锁
		{
			if(!Rel25.bHaveMakeLKRptFlag)
			{
				MakeAlarmRpt(Rel25.R25LKRptNum);
				Rel25.bHaveMakeLKRptFlag = TRUE;				
			}
			ResetRel25();
			return;
		}
		else
			Rel25.bHaveMakeLKRptFlag = FALSE;
	}
	else
	{			
		//Rel25.bHaveMakeLKRptFlag = FALSE;
        ResetRel25();
		return;
	}
	if(!Rel25.bManSynFlag) //检同期启动
	{
		Rel25.bManSynFlag = TRUE;
		Rel25.bJudgeNetend = FALSE;
		Rel25.bSameNetFlag = FALSE;
		Rel25.bDiffNetFlag = FALSE;
		Rel25.bSigleNetFlag = FALSE;
		Rel25.bJudgeSameNetPIFlag = FALSE;
		Rel25.bJudgeDiffNetPIFlag = FALSE;
		Rel25.bJudgeSigleNetPIFlag = FALSE;
		*Rel25.pPA = 0;
        Rel25.dwSynCheckTime = *Rel25.pSysTimer;
		ReportMsg("Rel25 pick! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
	}
	
	if(ReadRelayWord(Rel25.w25INT)) //被中断
	{
		if(!Rel25.bHaveMakeINTRptFlag)
		{
			MakeActRptWithoutStart(Rel25.R25INTRptNum);
			Rel25.bHaveMakeINTRptFlag = TRUE;
			ResetRel25();
			return;
		}
	}
	else
		Rel25.bHaveMakeINTRptFlag = FALSE;

	if(*Rel25.pSysTimer - Rel25.dwSynCheckTime > *Rel25.p25DTSet) //延时已到，同期失败
	{		
		MakeActRptWithoutStart(Rel25.R25FalRptNum);
		
		ReportMsg("Rel25 fail! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
		ResetRel25();
		return;
	}
	if(ReadRelayWord(Rel25.w79_25PI)) //如果是重合闸启动的检同期，若电压不在同期电压范围之内，直接返回
	{
		if(Rel25.bDVOutLimitFlag)
			return;
	}
	if(!Rel25.bJudgeNetend) //网判断尚未完成
	{		
		JudgeGrid();  //网判断
		if(!Rel25.bJudgeNetend)
			return;
	}
	
	if(Rel25.bSameNetFlag) //同网同期判断
	{
		SameGridCheck();
		return;
	}
	if(Rel25.bDiffNetFlag) //异网同期判断
	{
		DiffGridCheck();
		return;
	}
	if(Rel25.bSigleNetFlag) //单网同期判断
		SingleGridCheck();
}
void CRelayTask::ResetRel25(void)
{	
	Rel25.bManSynFlag = FALSE;
	Rel25.bJudgeNetend = FALSE;
	Rel25.bSigleNetFlag = FALSE;
	Rel25.bSameNetFlag = FALSE;
	Rel25.bDiffNetFlag = FALSE;
	Rel25.bJudgeSameNetPIFlag = FALSE;
	Rel25.bJudgeDiffNetPIFlag = FALSE;
	Rel25.bJudgeSigleNetPIFlag = FALSE;

	Rel25.bHaveMakeLKRptFlag = FALSE;
	Rel25.bHaveMakeINTRptFlag = FALSE;
	ResetHoldRelayWord(Rel25.wMCL_25PI);
	ResetHoldRelayWord(Rel25.w79_25PI);
	WriteRelayWord(Rel25.w25ActRW, RELAY_OFF); 

}
void CRelayTask::JudgeGrid(void)	//判断同网异网还是单网
{	
	if(ReadRelayWord(Rel25.w25LK))
	{
		ResetJudgeGrid();
		return;
	}
	//母线与线路均无压，直接判为同期成功
	if((Rel25.pUsyn->Mod<*Rel25.p25_27Set)&&((U1.Mod<*Rel25.p25_27Set)&&(U2.Mod<*Rel25.p25_27Set)&&(U3.Mod<*Rel25.p25_27Set)))
	{
		WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
		Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
		MakeActRptWithoutStart(Rel25.R25SucRptNum);
		return;
	}
	if(Rel25.pUsyn->Mod<*Rel25.p25_27Set) //母线无压，不进行检同期，防止电流从线路反送至母线
	{
		ResetJudgeGrid();
		return;
	}
	if((U1.Mod<*Rel25.p25_27Set)&&(U2.Mod<*Rel25.p25_27Set)&&(U3.Mod<*Rel25.p25_27Set)) //线路侧无压，判为单网
	{
		Rel25.bJudgeSameNetPIFlag = FALSE;
		Rel25.bJudgeDiffNetPIFlag = FALSE;
		if(!Rel25.bJudgeSigleNetPIFlag)
		{
			Rel25.bJudgeSigleNetPIFlag = TRUE;
			Rel25.dwInitSingleNetChkTimer = *Rel25.pSysTimer;
		}
		else
		{
			if(*Rel25.pSysTimer-Rel25.dwInitSingleNetChkTimer>1000)
			{				
				ReportMsg("Rel25: single grid!");
				Rel25.bSigleNetFlag = TRUE;
				Rel25.bSameNetFlag = FALSE;
				Rel25.bDiffNetFlag = FALSE;
				Rel25.bJudgeNetend = TRUE;
				Rel25.bJudgeSigleNetPIFlag = FALSE;
			}
		}
		return;
	}
	if((U1.Mod>*Rel25.p25_27Set)&&(U2.Mod>*Rel25.p25_27Set)&&(U3.Mod>*Rel25.p25_27Set)) //线路侧三相均有压
	{
		if(*Rel25.pDF == 50000)
			ResetJudgeGrid();
		if(*Rel25.pDF>=50) //0.05HZ,为异网，频率放大了1000倍
		{			
			Rel25.bJudgeSameNetPIFlag = FALSE;
			Rel25.bJudgeSigleNetPIFlag = FALSE;
			if(!Rel25.bJudgeDiffNetPIFlag)
			{
				Rel25.bJudgeDiffNetPIFlag = TRUE;
				Rel25.dwInitDiffleNetChkTimer = *Rel25.pSysTimer;
			}
			else
			{
				if(*Rel25.pSysTimer-Rel25.dwInitDiffleNetChkTimer > 100)
				{					
					ReportMsg("Rel25: diff grid!");
					Rel25.bSigleNetFlag = FALSE;
					Rel25.bSameNetFlag = FALSE;
					Rel25.bDiffNetFlag = TRUE;
					Rel25.bJudgeNetend = TRUE;
					Rel25.bJudgeDiffNetPIFlag = FALSE;
				}
			}
			return;
		}
		else //频差小于0.05Hz，为同网
		{
			Rel25.bJudgeDiffNetPIFlag = FALSE;
			Rel25.bJudgeSigleNetPIFlag = FALSE;
			if(!Rel25.bJudgeSameNetPIFlag)
			{
				Rel25.bJudgeSameNetPIFlag = TRUE;
				Rel25.dwInitSameleNetChkTimer= *Rel25.pSysTimer;
			}
			else
			{
				if(*Rel25.pSysTimer-Rel25.dwInitSameleNetChkTimer > 1000)
				{
					ReportMsg("Rel25: same grid!");
					Rel25.bSigleNetFlag = FALSE;
					Rel25.bSameNetFlag = TRUE;
					Rel25.bDiffNetFlag = FALSE;
					Rel25.bJudgeNetend = TRUE;
					Rel25.bJudgeDiffNetPIFlag = FALSE;
				}
			}
			return;
		}		
	}
}
void CRelayTask::ResetJudgeGrid(void)//复位电网判断
{
	Rel25.bSigleNetFlag = FALSE;
	Rel25.bSameNetFlag = FALSE;
	Rel25.bDiffNetFlag = FALSE;
	Rel25.bJudgeNetend = FALSE;
	Rel25.bJudgeDiffNetPIFlag = FALSE;	
	Rel25.bJudgeSameNetPIFlag = FALSE;
	Rel25.bJudgeSigleNetPIFlag = FALSE;
}
//单网同期，
void CRelayTask::SingleGridCheck(void)
{
	WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
	Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
	MakeActRptWithoutStart(Rel25.R25_27RptNum); //做无压空合报告
}
//同网同期判断
void CRelayTask::SameGridCheck(void)
{
	if (*Rel25.pDF>100) //同网同期判断中，频差出现变动，重新进行电网判断
	{
		ResetJudgeGrid();
		return;		
	}	
	if(Rel25.bDVOutLimitFlag == TRUE) //电压不在合理范围之内
		return;
	if(*Rel25.pDV>*Rel25.p25VDSet)//压差大于定值
		return;
//	if(*Rel25.pDA>18000)
//		*Rel25.pDA = 36000-*Rel25.pDA;
//	if(*Rel25.pDA>*Rel25.p25PASet) //同期功角大于定值
	if(m_DQ>*Rel25.p25PASet) //同期功角大于定值
		return;
	if ((*Rel25.pDF)>50) //同网同期的频差要求
		return;
	
	WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
	Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
	MakeActRptWithoutStart(Rel25.R25SucRptNum);
	
	ReportMsg("Rel25 same grid succ! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
}
//异网同期判断
void CRelayTask::DiffGridCheck(void)
{
	LONG preA;
	if (*Rel25.pDF<=20) //异网同期判断中，频差出现变动，重新进行电网判断
	{
		ResetJudgeGrid();
		return;		
	}
	if(Rel25.bDVOutLimitFlag == TRUE) //电压不在合理范围之内
		return;
	if(*Rel25.pDF > *Rel25.p25FDSet) //频差不满足条件
		return;
	
	CaluPreAngle();
	if(*Rel25.pPA == 0)
		return;
	preA = *Rel25.pDA - *Rel25.pPA;
	preA = ToAbs(preA);
	if(preA>500)
		return;
	
	WriteRelayWord(Rel25.w25ActRW, RELAY_ON);
	Rel25.dwSynClosePulseTimer = *Rel25.pSysTimer;
	MakeActRptWithoutStart(Rel25.R25SucRptNum);
	
	ReportMsg("Rel25 diff grid succ! DA=%d,DF=%d,DV=%d",*Rel25.pDA,*Rel25.pDF,*Rel25.pDV);
}
void CRelayTask::CaluPreAngle(void)
{
	if(*Rel25.pDF == 50000)
	{
		*Rel25.pPA = 0;
		return;
	}
	*Rel25.pPA = 36*(*Rel25.pDF)*(*Rel25.p52CLT)/1000;
}

void CRelayTask::JDXXCheck(void)
{	
	static DWORD RJDXXTimer = 0;
	if((*CFG_pbyJDXX == CFG_STATUS_SET)&&(*SW_pbyJDXX == CFG_STATUS_SET))
		WriteJDXXData();
	if(JDXXOnFlag == FALSE)
	{
		JDXXPickUpCHK();
		if(JDXXCheckFlag == TRUE)
		{
			OS_PostMessage(g_hDbaseTaskID, SM_JDXXPICK, 0,0,0,0 );	//RtosGetCurrentTaskId(), 发送消息启动接地选线
			RJDXXTimer = dwRelayMSCnt;
			JDXXOnFlag = TRUE;
		}
		else
			return;
	}
	if(!HaveJDXXResultFlag)
	{
		if((dwRelayMSCnt - RJDXXTimer) > 30000)
		{
			JDXXPowerDirCheck();
			if(JDXXForwardFlag)
				MakeAlarmRpt(R_64LINE);
			else
				MakeAlarmRpt(R_59NAR);
			JDXXOnFlag = FALSE;
			JDXXCheckFlag = FALSE;
			JDXXPickFlag = FALSE;
			HaveJDXXResultFlag = TRUE;
		}
	}
}
void CRelayTask::JDXXPickUpCHK(void)
{	
	static DWORD RJDXXPickTimer = 0;
	if((*CFG_pbyJDXX == CFG_STATUS_CLR)||(*SW_pbyJDXX== CFG_STATUS_CLR))
	{		
		HaveJDXXResultFlag = FALSE;
		JDXXPickFlag = FALSE;
		JDXXCheckFlag = FALSE;
		return;
	}
	if( U0.Mod > *RS_pn59NU)
	{
		if(HaveJDXXResultFlag == FALSE)
		{
			if(JDXXPickFlag == FALSE)
			{
				RJDXXPickTimer = dwRelayMSCnt;
				JDXXPickFlag = TRUE;
			}
			if((dwRelayMSCnt-RJDXXPickTimer) > 1000)
			{
				JDXXCheckFlag = TRUE;
			}
		}
	}
	else
	{		
		HaveJDXXResultFlag = FALSE;
		JDXXPickFlag = FALSE;
		JDXXCheckFlag = FALSE;
	}
}
void CRelayTask::JDXXPowerDirCheck()
{
	LONG temp1,temp2;
	LONG Angle;
	if(*CFG_pbyXHJD == CFG_STATUS_CLR)
	{	
		temp2=U0.Mod*I0.Mod/32;
	}
	else
	{
		temp2=U5.Mod*I5.Mod/32;
	}
	if(temp2 < 100)
	{
		JDXXForwardFlag = FALSE;
		return;
	}
	if(*CFG_pbyXHJD == CFG_STATUS_CLR)
	{	
		temp1=((U0.Imag*1000/U0.Mod)*(I0.Real*1000/I0.Mod)-(U0.Real*1000/U0.Mod)*(I0.Imag*1000/I0.Mod));  //(cosU*cosI+sinU*sinI)*cos90+(sinU*cosI-cosU*sinI)sin90
	}
	else
	{
		temp1=((U5.Imag*1000/U5.Mod)*(I5.Real*1000/I5.Mod)-(U5.Real*1000/U5.Mod)*(I5.Imag*1000/I5.Mod));
	}
	if(temp1 > 0)   
	{
		JDXXForwardFlag = TRUE;
	}	
	else 
	{
		JDXXForwardFlag = FALSE;
	}
}
void CRelayTask::OnJDXXResult(DWORD arg,DWORD result)
{
	if(JDXXOnFlag)
	{
		if(result == 1)
			MakeAlarmRpt(R_64LINE);
		else if(result == 2)
			MakeAlarmRpt(R_64BUS);
		else		
			MakeAlarmRpt(R_59NAR);
		JDXXOnFlag = FALSE;
		JDXXCheckFlag = FALSE;
		JDXXPickFlag = FALSE;		
		HaveJDXXResultFlag = TRUE;
	}
}
void CRelayTask::WriteJDXXData()
{
	if((*CF_pnU0 == 0)||(*CF_pnI0 == 0))
	{
		ReportMsg("The CF CF_pnU0 or CF_pnI0 is zero! CF_pnU0=%d,CF_pnI0=%d",*CF_pnU0,*CF_pnI0);
		return;
	}
	JDXXElecData.U0 	= 10000*(float)(U0.Mod)*(*AS_pnPTRatio)/(*CF_pnU0);
	JDXXElecData.U0_5	= 10000*(float)(U5.Mod)*(*AS_pnPTRatio)/(*CF_pnU0);
	JDXXElecData.I0 	= 1000*(float)(I0.Mod)*(*AS_pnZCTRatio)/(*CF_pnI0);
	JDXXElecData.I0_5	= 1000*(float)(I5.Mod)*(*AS_pnZCTRatio)/(*CF_pnI0);
	JDXXElecData.I0_A	= *RM_pnAI01;
	JDXXElecData.I0_5_A	= *RM_pnAI05;
	
}

