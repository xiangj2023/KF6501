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
*           		KF6560电容器保护算法代码                                                                       
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
#include "daefunc.h"

extern DWORD dwADResumeCnt;
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
extern g_hDbaseTaskID;
extern dwRelayDataPtr;	//AD当前写指针
extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern BYTE 	g_RptSerialNum;			//报告序列号 
extern BOOL		RelayCheckErr;			//保护元件自检出错标志
extern BOOL 	RelayCheckInitOK;		//保护元件自检初始化标志
extern BYTE 	*g_MaintSW;

BOOL	g_FaultOnFlag;				//进入故障循环处理标志 
BOOL 	RelayTaskInitOK;		//保护初始化正常
DWORD 	dwRelayTaskID;
DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
TStructCheck StructCheck[CHECK_NUM];
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

    //循环调用进行消息调度,直到收到任务退出消息
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
	
/*	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	
	//获取数据库保护测量值数据指针，以加快操作速度
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	
	InitAllRelayMem();
	InitRealy();

//	RelayCheck();

	g_RptSerialNum = 0;					//报告序列号，数据库组织用
	RptSerialNumOfAct =0;				//动作报告序列号，作动作报告用
	RptSerialNumOfStart=0;				//启动报告序列号，作带故障电量启动报告用
	FaultSINum = 0;					//采样间隔数计数器
	PickCheck =0;
	PickCheckNext =0;	
	lSampData1 =0;
	lSampData2 =0;
	lSampDate3 =0;
	lPickValue =0;
	m_lPickUpValue =0;
	PickUpRW = RW_PICKUP;			//保护启动继电器字，不区分模值启动突变量启动
	FaultOnFlag = FALSE;				//进入故障循环处理标志
	g_FaultOnFlag =FALSE;
	HaveMakeStartRptFlag = FALSE;		//作启动报告标志
	m_bFaultLKFlag = FALSE;				//突变量启动，锁定故障起始时刻标志
	FaultCheckOutFlag = FALSE;			//检出故障标志
	m_bHaveMakeTripFailRpt = FALSE;
	RelCFGCLR = CFG_STATUS_CLR;		//保护配置退出
	RelCFGSET = CFG_STATUS_SET;		//保护配置退出
//	m_bNextRelayStart = FALSE;		//本次故障处理中可能出现第二次动作标志


	
	dwChanDataMask = ((0x01<<CHANNEL_6560_U1)|(0x01<<CHANNEL_6560_U2)|(0x01<<CHANNEL_6560_Ud1)|(0x01<<CHANNEL_6560_Ud2)|\
						(0x01<<CHANNEL_6560_Ud3)|(0x01<<CHANNEL_6560_IP)|(0x01<<CHANNEL_6560_I1)|\
						(0x01<<CHANNEL_6560_I2)|(0x01<<CHANNEL_6560_I3));
	
	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	dwComposeMask = 0x01<<CHANNEL_6560_Up;
	
	
	//初始化读取保护数据临时缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*9);	
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	dwRECBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3));
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM));
	
	//下面是初始化逻辑方程部分
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();

	
	//初始化开出自检为正常状态
	int i;
	for(i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}

	//首先进行一遍开出失效自检检查
	for(i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();

	InitDCRecInfo();

	g_MaintSW = SW_pbyMAINT;
	m_bLogicChanged = FALSE;
	RelayTaskInitOK = TRUE;
	SetTimer(1,5);

}
//------------------------------------------------------------------------------------------ 
// 名称: OnTimeOut	 
// 功能: 定时器消息响应函数
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::OnTimeOut(DWORD id)
{	
	if(dwADResumeCnt ==0)
		return;
	else
	{
		LogicProcess();
	}

}

void CRelayTask::OnSampleEnd(DWORD arg)
{	
	g_FaultOnFlag = FaultOnFlag;
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
	//初始化采样通道数据掩码
	static WORD wInvalidCheckCnt;	
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
	
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	WriteCompseData();
	CaluUI();          //根据采样数据，进算电量It,If,U1,U2
	
	//首先进行开出自检
	if(!m_bLogicChanged)
		BoBreakCheck();
	
	if(FaultOnFlag == FALSE)
	{
		CheckPTBrok_1U(&RelPT);					//PT断线处理
	}
	
	FalutProcess();								//故障循环处理
	GeneralUnderValRelay(&Rel27U);				//低电压保护子程序
	Process52BF();
	LogicProcess();								//逻辑方程处理
	
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
	
	WriteDCChannel();							//写数字录波
//	MCF_GPIO_PODR_TIMER &= ~0x02;		//低电平
}
//------------------------------------------------------------------------------------------ 
// 名称: CaluUI	 
// 功能: 计算保护测量值
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::CaluUI(void)
{	
	//计算UP模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwComposeBuf,&UP);
	CaluModValue(&UP);
	*RM_pnUp = UP.Mod;
	DB_LogWriteRM(RM_UP, *RM_pnUp);
	if(UP.Mod > *AS_pn59UP)
	{
		WriteRelayWord(RW_59UP, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UP, RELAY_OFF);
	}
	if(UP.Mod <*AS_pn27UP)
	{
		WriteRelayWord(RW_27UP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UP, RELAY_OFF);
	}	

	//计算U1模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf,&U1);
	CaluModValue(&U1);	
	*RM_pnU1 = U1.Mod;
	DB_LogWriteRM(RM_U1, *RM_pnU1);	
	if(U1.Mod > *AS_pn59U1)
	{
		WriteRelayWord(RW_59U1, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(U1.Mod <*AS_pn27U1)
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}	

	//计算U2模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	*RM_pnU2 = U2.Mod;
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
	if(U2.Mod > *AS_pn59U2)
	{
		WriteRelayWord(RW_59U2, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(U2.Mod <*AS_pn27U2)
	{
		WriteRelayWord(RW_27U2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}	
	//计算Ud1模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&Ud1);
	CaluModValue(&Ud1);
	*RM_pnUd1 = Ud1.Mod;
	DB_LogWriteRM(RM_UD1, *RM_pnUd1);	
	if(Ud1.Mod > *AS_pn59Ud1)
	{
		WriteRelayWord(RW_59UD1, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UD1, RELAY_OFF);
	}
	if(Ud1.Mod <*AS_pn27Ud1)
	{
		WriteRelayWord(RW_27UD1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UD1, RELAY_OFF);
	}	
	//计算Ud2模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&Ud2);
	CaluModValue(&Ud2);
	*RM_pnUd2 = Ud2.Mod;
	DB_LogWriteRM(RM_UD2, *RM_pnUd2);	
	if(Ud2.Mod > *AS_pn59Ud2)
	{
		WriteRelayWord(RW_59UD2, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UD2, RELAY_OFF);
	}
	if(Ud2.Mod <*AS_pn27Ud2)
	{
		WriteRelayWord(RW_27UD2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UD2, RELAY_OFF);
	}	
	//计算Ud2模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&Ud3);
	CaluModValue(&Ud3);
	*RM_pnUd3 = Ud3.Mod;
	DB_LogWriteRM(RM_UD3, *RM_pnUd3);	
	if(Ud3.Mod > *AS_pn59Ud3)
	{
		WriteRelayWord(RW_59UD3, RELAY_ON);		
	}
	else
	{
		WriteRelayWord(RW_59UD3, RELAY_OFF);
	}
	if(Ud3.Mod <*AS_pn27Ud3)
	{
		WriteRelayWord(RW_27UD3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_27UD3, RELAY_OFF);
	}	

	//计算IP模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&IP);
	CaluModValue(&IP);
	*RM_pnIp = IP.Mod;
	DB_LogWriteRM(RM_IP, *RM_pnIp);
	if(IP.Mod>*AS_pn50IP)
	{
		WriteRelayWord(RW_50IP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IP, RELAY_OFF);
	}
	if(IP.Mod>*AS_pn37IP)
	{
		WriteRelayWord(RW_37IP, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IP, RELAY_ON);
	}
	
	//计算I1模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&I1);
	CaluModValue(&I1);
	*RM_pnI1 = I1.Mod;
	DB_LogWriteRM(RM_I1, *RM_pnI1);	
	if(I1.Mod>*AS_pn50I1)
	{
		WriteRelayWord(RW_50I1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1, RELAY_OFF);
	}
	if(I1.Mod>*AS_pn37I1)
	{
		WriteRelayWord(RW_37I1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1, RELAY_ON);
	}
	//计算I2模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&I2);
	CaluModValue(&I2);
	*RM_pnI2 = I2.Mod;
	DB_LogWriteRM(RM_I2, *RM_pnI2);	
	if(I2.Mod>*AS_pn50I2)
	{
		WriteRelayWord(RW_50I2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I2, RELAY_OFF);
	}
	if(I2.Mod>*AS_pn37I2)
	{
		WriteRelayWord(RW_37I2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I2, RELAY_ON);
	}
	//计算I3模值并置其有压无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&I3);
	CaluModValue(&I3);
	*RM_pnI3 = I3.Mod;
	DB_LogWriteRM(RM_I3, *RM_pnI3);
	if(I3.Mod>*AS_pn50I3)
	{
		WriteRelayWord(RW_50I3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I3, RELAY_OFF);
	}
	if(I3.Mod>*AS_pn37I3)
	{
		WriteRelayWord(RW_37I3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I3, RELAY_ON);
	}
	
	CaluSecondFourier(dwSampBuf+READ_POINT_NUM*5,&IP2);
	CaluModValue(&IP2);
	IP2.Mod = IP2.Mod*CurHarmCal[0]/1000;	

	CaluThirdFourier(dwSampBuf+READ_POINT_NUM*5,&IP3);
	CaluModValue(&IP3);
	IP3.Mod = IP3.Mod*CurHarmCal[1]/1000;
	*RM_pnIh3 = IP3.Mod;
	DB_LogWriteRM(RM_IH3, *RM_pnIh3);

	CaluFifthFourier(dwSampBuf+READ_POINT_NUM*5,&IP5);
	CaluModValue(&IP5);
	IP5.Mod = IP5.Mod*CurHarmCal[2]/1000;
	*RM_pnIh5 = IP5.Mod;
	DB_LogWriteRM(RM_IH5, *RM_pnIh5);

	CaluSeventhFourier(dwSampBuf+READ_POINT_NUM*5,&IP7);
	CaluModValue(&IP7);
	IP7.Mod = IP7.Mod*CurHarmCal[3]/1000;
	*RM_pnIh7 = IP7.Mod;
	DB_LogWriteRM(RM_IH7, *RM_pnIh7);
	
	m_IPH = IP3.Mod*IP3.Mod+IP5.Mod*IP5.Mod*25/9+IP7.Mod*IP7.Mod*49/9;
	IPH.Mod= Evolution64(m_IPH);
}


/************************************************************************************************
*功能: 初始化与保护直接相关的定值
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitSet(void)
{
	if(*AS_pnPM == BTMODE)
	{
		WriteRelayWord(RW_BTMODE,RELAY_ON);
		WriteRelayWord(RW_ATMODE,RELAY_OFF);
	}
	
	else if(*AS_pnPM == ATMODE)
	{
		WriteRelayWord(RW_BTMODE,RELAY_OFF);
		WriteRelayWord(RW_ATMODE,RELAY_ON);
	}
}

void  CRelayTask::InitRealy(void)
{
	InitRel50I0();
	InitRel50I1();
	InitRel59U();
	InitRel50IH();
	InitRel60I1();
	InitRel60I2();
	InitRel50ST();
	InitRel60Ud1();
	InitRel60Ud2();
	InitRel60Ud3();
	InitRel27U();
	InitRelPT();
	InitStructCheck();
	
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
void CRelayTask::WriteCompseData(void)
{
	if(*AS_pnPM == BTMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i];
		}
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			dwRECBuf[i]= dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		}
	}
	else if(*AS_pnPM == ATMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwComposeBuf[i] = dwSampBuf[i]-dwSampBuf[i+READ_POINT_NUM];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			dwRECBuf[i]= dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i]-dwSampBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		}
	}
	WriteComposeData(dwRECBuf, dwComposeMask);
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
// 名称: InitRel50I0	 
// 功能: 初始化速断保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I0(void)
{
	Rel50I0.pElecSet			= RS_pn50_0C;          						//过量保护电量定值
	Rel50I0.pTimeSet			= RS_pn50_0D;				 			//过量保护时间定值
	Rel50I0.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel50I0.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel50I0.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel50I0.pH2Coef				= NULL;

	Rel50I0.pRelCFG				= CFG_pby50_0;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I0.pRelSW				= SW_pby50_0;         					//过量保护软压板------1:投入，2:退出
	Rel50I0.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I0.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel50I0.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel50I0.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel50I0.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel50I0.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I0.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel50I0.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I0.pRelElec 			= &IP;									//当前电量指针		
	Rel50I0.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel50I0.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel50I0.pRelVolt			= &UP;									//当前用来闭锁的电压的指针

	Rel50I0.pRel47UCFG		= &RelCFGCLR;;
	Rel50I0.R47ULKRelayWord   = FALSE1;

	Rel50I0.StartRelayWord		= RW_PI50_0;						//过量保护入段继电器字
	Rel50I0.ActRelayWord		= RW_TR50_0;							//过量保护动作继电器字
	Rel50I0.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel50I0.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel50I0.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel50I0.PRewardRelayWord    = FALSE1;
	Rel50I0.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel50I0.LockRelayWord		= RW_LKPI50_0;						//入段闭锁继电器字
	Rel50I0.RSTRelayWord		= RW_LKRST50_0;							//返回闭锁继电器字
	Rel50I0.LoopCtrlRelayWord	= RW_LPC50_0;						//保护循环控制继电器字
	Rel50I0.wReportNum			= R_R50I0;							//过量保护报告序号
	Rel50I0.wAccReportNum		= 0xFFFF;								//加速动作报告序号
	Rel50I0.RptSerialNum		= &RptSerialNumOfAct;			

	Rel50I0.dwK35				= NULL;
	Rel50I0.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel50I0.wPickUpReportNum	= R_R50I0_PI;						//入段报告序列号
	Rel50I0.wResetReportNum		= R_R50I0_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50I0.dwRelTimer			= 0;			 			//定时器
	Rel50I0.byRelFCounter		= 0;						//故障计数器
	Rel50I0.byRelRCounter		= 0;						//返回计数器	
	
	Rel50I0.wExcepWordNum        =8;
	Rel50I0.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel50I1	 
// 功能: 初始化过流保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I1(void)
{		
	Rel50I1.pElecSet			= RS_pn50_1C;          					//过量保护电量定值
	Rel50I1.pTimeSet			= RS_pn50_1D;				 			//过量保护时间定值
	Rel50I1.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel50I1.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel50I1.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel50I1.pH2Coef				= NULL;

	Rel50I1.pRelCFG				= CFG_pby50_1;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I1.pRelSW				= SW_pby50_1;         					//过量保护软压板------1:投入，2:退出
	Rel50I1.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I1.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel50I1.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel50I1.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel50I1.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel50I1.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I1.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel50I1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I1.pRelElec 			= &IP;									//当前电量指针		
	Rel50I1.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel50I1.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel50I1.pRelVolt			= &UP;									//当前用来闭锁的电压的指针

	Rel50I1.pRel47UCFG		= &RelCFGCLR;;
	Rel50I1.R47ULKRelayWord   = FALSE1;

	Rel50I1.StartRelayWord		= RW_PI50_1;						//过量保护入段继电器字
	Rel50I1.ActRelayWord		= RW_TR50_1;							//过量保护动作继电器字
	Rel50I1.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel50I1.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel50I1.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel50I1.PRewardRelayWord    = FALSE1;
	Rel50I1.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel50I1.LockRelayWord		= RW_LKPI50_1;						//入段闭锁继电器字
	Rel50I1.RSTRelayWord		= RW_LKRST50_1;							//返回闭锁继电器字
	Rel50I1.LoopCtrlRelayWord	= RW_LPC50_1;						//保护循环控制继电器字
	Rel50I1.wReportNum			= R_R50I1;							//过量保护报告序号
	Rel50I1.wAccReportNum		= 0xFFFF;								//加速动作报告序号
	Rel50I1.RptSerialNum		= &RptSerialNumOfAct;			
	Rel50I1.dwK35				= NULL;
	
	Rel50I1.bMakeActRpt			= TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel50I1.wPickUpReportNum	= R_R50I1_PI;						//入段报告序列号
	Rel50I1.wResetReportNum		= R_R50I1_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50I1.dwRelTimer			= 0;			 			//定时器
	Rel50I1.byRelFCounter		= 0;						//故障计数器
	Rel50I1.byRelRCounter		= 0;						//返回计数器	
	Rel50I1.wExcepWordNum       = 8;
	Rel50I1.wInfo				= 0;

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel59U	 
// 功能: 初始化过压保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel59U()
{	
	Rel59U.pElecSet				= RS_pn59V;          					//过量保护电量定值
	Rel59U.pTimeSet				= RS_pn59D;				 			//过量保护时间定值
	Rel59U.pHarmKhr1Set			= NULL;									//过量保护谐波加权系数
	Rel59U.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel59U.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel59U.pH2Coef				= NULL;

	Rel59U.pRelCFG				= CFG_pby59;							//过量保护功能控制字--1:投入，2:退出 
	Rel59U.pRelSW				= SW_pby59;         						//过量保护软压板------1:投入，2:退出
	Rel59U.pRelHLockCFG			= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel59U.pRelHSuprCFG			= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel59U.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel59U.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel59U.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel59U.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel59U.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel59U.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel59U.pRelElec 			= &UP;									//当前电量指针		
	Rel59U.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel59U.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel59U.pRelVolt				= NULL;									//当前用来闭锁的电压的指针


	Rel59U.pRel47UCFG		= &RelCFGCLR;;
	Rel59U.R47ULKRelayWord   = FALSE1;

	Rel59U.StartRelayWord		= RW_PI59;						//过量保护入段继电器字
	Rel59U.ActRelayWord			= RW_TR59;							//过量保护动作继电器字
	Rel59U.AccRelayWord			= FALSE1;								//后加速允许继电器字
	Rel59U.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel59U.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel59U.PRewardRelayWord     = FALSE1;
	Rel59U.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel59U.LockRelayWord		= RW_LKPI59;						//入段闭锁继电器字
	Rel59U.RSTRelayWord			= RW_LKRST59;							//返回闭锁继电器字
	Rel59U.LoopCtrlRelayWord	= RW_LPC59;						//保护循环控制继电器字
	Rel59U.wReportNum			= R_R59U;								//过量保护报告序号
	Rel59U.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel59U.dwK35				= NULL;
	
	Rel59U.bMakeActRpt			= TRUE;
	Rel59U.RptSerialNum			= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel59U.wPickUpReportNum		= R_R59U_PI;						//入段报告序列号
	Rel59U.wResetReportNum		= R_R59U_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel59U.dwRelTimer			= 0;			 			//定时器
	Rel59U.byRelFCounter		= 0;						//故障计数器
	Rel59U.byRelRCounter		= 0;						//返回计数器		
	Rel59U.wExcepWordNum        = 8;
	Rel59U.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel50IH	 
// 功能: 初始化谐波过流保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50IH(void)
{
	Rel50IH.pElecSet			= RS_pn50_HC;          					//过量保护电量定值
	Rel50IH.pTimeSet			= RS_pn50_HD;				 			//过量保护时间定值
	Rel50IH.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel50IH.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel50IH.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel50IH.pH2Coef				= NULL;

	Rel50IH.pRelCFG				= CFG_pby50_H;							//过量保护功能控制字--1:投入，2:退出 
	Rel50IH.pRelSW				= SW_pby50_H;         					//过量保护软压板------1:投入，2:退出
	Rel50IH.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel50IH.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel50IH.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel50IH.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel50IH.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel50IH.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50IH.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel50IH.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50IH.pRelElec 			= &IPH;									//当前电量指针		
	Rel50IH.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel50IH.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel50IH.pRelVolt			= &UP;									//当前用来闭锁的电压的指针

	Rel50IH.pRel47UCFG		= &RelCFGCLR;;
	Rel50IH.R47ULKRelayWord   = FALSE1;

	Rel50IH.StartRelayWord		= RW_PI50H;						//过量保护入段继电器字
	Rel50IH.ActRelayWord		= RW_TR50H;							//过量保护动作继电器字
	Rel50IH.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel50IH.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel50IH.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel50IH.PRewardRelayWord    = FALSE1;
	Rel50IH.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel50IH.LockRelayWord		= RW_LKPI50H;						//入段闭锁继电器字
	Rel50IH.RSTRelayWord		= RW_LKRST50H;							//返回闭锁继电器字
	Rel50IH.LoopCtrlRelayWord	= RW_LPC50H;						//保护循环控制继电器字
	Rel50IH.wReportNum			= R_R50IH;								//过量保护报告序号
	Rel50IH.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50IH.dwK35				= NULL;
	Rel50IH.bMakeActRpt			= TRUE;
	Rel50IH.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel50IH.wPickUpReportNum	= R_R50IH_PI;						//入段报告序列号
	Rel50IH.wResetReportNum		= R_R50IH_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50IH.dwRelTimer			= 0;			 			//定时器
	Rel50IH.byRelFCounter		= 0;						//故障计数器
	Rel50IH.byRelRCounter		= 0;						//返回计数器		
	Rel50IH.wExcepWordNum		= 8;
	Rel50IH.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel60I1	 
// 功能: 初始化差流1保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60I1(void)
{
	Rel60I1.pElecSet			= RS_pn60I1C;          					//过量保护电量定值
	Rel60I1.pTimeSet			= RS_pn60I1D;				 			//过量保护时间定值
	Rel60I1.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel60I1.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel60I1.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel60I1.pH2Coef				= NULL;

	Rel60I1.pRelCFG				= CFG_pby60I1;							//过量保护功能控制字--1:投入，2:退出 
	Rel60I1.pRelSW				= SW_pby60I;         						//过量保护软压板------1:投入，2:退出
	Rel60I1.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel60I1.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel60I1.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel60I1.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel60I1.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel60I1.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel60I1.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel60I1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel60I1.pRelElec 			= &I1;									//当前电量指针		
	Rel60I1.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel60I1.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel60I1.pRelVolt			= &UP;									//当前用来闭锁的电压的指针

	Rel60I1.pRel47UCFG		= &RelCFGCLR;;
	Rel60I1.R47ULKRelayWord   = FALSE1;

	Rel60I1.StartRelayWord		= RW_PI60I1;						//过量保护入段继电器字
	Rel60I1.ActRelayWord		= RW_TR60I1;							//过量保护动作继电器字
	Rel60I1.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel60I1.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel60I1.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel60I1.PRewardRelayWord    = FALSE1;
	Rel60I1.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel60I1.LockRelayWord		= RW_LKPI60I1;						//入段闭锁继电器字
	Rel60I1.RSTRelayWord		= RW_LKRST60I1;							//返回闭锁继电器字
	Rel60I1.LoopCtrlRelayWord	= RW_LPC60I1;						//保护循环控制继电器字
	Rel60I1.wReportNum			= R_R60I1;								//过量保护报告序号
	Rel60I1.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel60I1.dwK35				= NULL;
	
	Rel60I1.bMakeActRpt			= TRUE;
	Rel60I1.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel60I1.wPickUpReportNum	= R_R60I1_PI;						//入段报告序列号
	Rel60I1.wResetReportNum		= R_R60I1_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel60I1.dwRelTimer			= 0;			 			//定时器
	Rel60I1.byRelFCounter		= 0;						//故障计数器
	Rel60I1.byRelRCounter		= 0;						//返回计数器		
	Rel60I1.wExcepWordNum		= 8;
	Rel60I1.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel60I2	 
// 功能: 初始化差流2保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60I2(void)
{
	Rel60I2.pElecSet			= RS_pn60I2C;          					//过量保护电量定值
	Rel60I2.pTimeSet			= RS_pn60I2D;				 			//过量保护时间定值
	Rel60I2.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel60I2.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel60I2.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel60I2.pH2Coef				= NULL;

	Rel60I2.pRelCFG				= CFG_pby60I2;							//过量保护功能控制字--1:投入，2:退出 
	Rel60I2.pRelSW				= SW_pby60I;         						//过量保护软压板------1:投入，2:退出
	Rel60I2.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel60I2.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel60I2.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel60I2.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel60I2.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel60I2.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel60I2.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel60I2.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel60I2.pRelElec 			= &I2;									//当前电量指针		
	Rel60I2.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel60I2.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel60I2.pRelVolt			= &UP;									//当前用来闭锁的电压的指针

	Rel60I2.pRel47UCFG		= &RelCFGCLR;;
	Rel60I2.R47ULKRelayWord   = FALSE1;

	Rel60I2.StartRelayWord		= RW_PI60I2;						//过量保护入段继电器字
	Rel60I2.ActRelayWord		= RW_TR60I2;							//过量保护动作继电器字
	Rel60I2.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel60I2.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel60I2.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel60I2.PRewardRelayWord    = FALSE1;
	Rel60I2.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel60I2.LockRelayWord		= RW_LKPI60I2;						//入段闭锁继电器字
	Rel60I2.RSTRelayWord		= RW_LKRST60I2;							//返回闭锁继电器字
	Rel60I2.LoopCtrlRelayWord	= RW_LPC60I2;						//保护循环控制继电器字
	Rel60I2.wReportNum			= R_R60I2;								//过量保护报告序号
	Rel60I2.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel60I2.dwK35				= NULL;
	
	Rel60I2.bMakeActRpt			= TRUE;
	Rel60I2.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel60I2.wPickUpReportNum	= R_R60I2_PI;						//入段报告序列号
	Rel60I2.wResetReportNum		= R_R60I2_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel60I2.dwRelTimer			= 0;			 			//定时器
	Rel60I2.byRelFCounter		= 0;						//故障计数器
	Rel60I2.byRelRCounter		= 0;						//返回计数器		
	Rel60I2.wExcepWordNum		= 8;
	Rel60I2.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel50ST	 
// 功能: 初始化支路过流保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50ST(void)
{
	Rel50ST.pElecSet			= RS_pn50STC;          					//过量保护电量定值
	Rel50ST.pTimeSet			= RS_pn50STD;				 			//过量保护时间定值
	Rel50ST.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel50ST.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel50ST.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel50ST.pH2Coef				= NULL;

	Rel50ST.pRelCFG				= CFG_pby50ST;							//过量保护功能控制字--1:投入，2:退出 
	Rel50ST.pRelSW				= SW_pby50ST;         						//过量保护软压板------1:投入，2:退出
	Rel50ST.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel50ST.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel50ST.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel50ST.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel50ST.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel50ST.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50ST.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel50ST.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50ST.pRelElec 			= &I3;									//当前电量指针		
	Rel50ST.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel50ST.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel50ST.pRelVolt			= &UP;									//当前用来闭锁的电压的指针

	Rel50ST.pRel47UCFG		= &RelCFGCLR;;
	Rel50ST.R47ULKRelayWord   = FALSE1;

	Rel50ST.StartRelayWord		= RW_PI50ST;						//过量保护入段继电器字
	Rel50ST.ActRelayWord		= RW_TR50ST;							//过量保护动作继电器字
	Rel50ST.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel50ST.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel50ST.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel50ST.PRewardRelayWord	= FALSE1;
	Rel50ST.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel50ST.LockRelayWord		= RW_LKPI50ST;						//入段闭锁继电器字
	Rel50ST.RSTRelayWord		= RW_LKRST50ST;							//返回闭锁继电器字
	Rel50ST.LoopCtrlRelayWord	= RW_LPC50ST;						//保护循环控制继电器字
	Rel50ST.wReportNum			= R_R50ST;								//过量保护报告序号
	Rel50ST.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50ST.dwK35				= NULL;
	Rel50ST.bMakeActRpt			= TRUE;
	Rel50ST.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel50ST.wPickUpReportNum	= R_R50ST_PI;						//入段报告序列号
	Rel50ST.wResetReportNum		= R_R50ST_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel50ST.dwRelTimer			= 0;			 			//定时器
	Rel50ST.byRelFCounter		= 0;						//故障计数器
	Rel50ST.byRelRCounter		= 0;						//返回计数器		
	Rel50ST.wExcepWordNum		= 8;
	Rel50ST.wInfo				 =0;
}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel60U1	 
// 功能: 初始化差压1保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60Ud1(void)
{
	Rel60Ud1.pElecSet			= RS_pn60U1V;          					//过量保护电量定值
	Rel60Ud1.pTimeSet			= RS_pn60U1D;				 			//过量保护时间定值
	Rel60Ud1.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel60Ud1.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel60Ud1.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel60Ud1.pH2Coef				= NULL;

	Rel60Ud1.pRelCFG				= CFG_pby60U1;							//过量保护功能控制字--1:投入，2:退出 
	Rel60Ud1.pRelSW				= SW_pby60U;         						//过量保护软压板------1:投入，2:退出
	Rel60Ud1.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel60Ud1.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel60Ud1.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel60Ud1.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel60Ud1.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel60Ud1.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel60Ud1.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel60Ud1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel60Ud1.pRelElec 			= &Ud1;									//当前电量指针		
	Rel60Ud1.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel60Ud1.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel60Ud1.pRelVolt			= NULL;									//当前用来闭锁的电压的指针
	Rel60Ud1.pRel47UCFG		= &RelCFGCLR;;
	Rel60Ud1.R47ULKRelayWord   = FALSE1;


	Rel60Ud1.StartRelayWord		= RW_PI60U1;						//过量保护入段继电器字
	Rel60Ud1.ActRelayWord		= RW_TR60U1;							//过量保护动作继电器字
	Rel60Ud1.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel60Ud1.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel60Ud1.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel60Ud1.PRewardRelayWord	= FALSE1;
	Rel60Ud1.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel60Ud1.LockRelayWord		= RW_LKPI60U1;						//入段闭锁继电器字
	Rel60Ud1.RSTRelayWord		= RW_LKRST60U1;							//返回闭锁继电器字
	Rel60Ud1.LoopCtrlRelayWord	= RW_LPC60U1;						//保护循环控制继电器字
	Rel60Ud1.wReportNum			= R_R60U1;								//过量保护报告序号
	Rel60Ud1.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel60Ud1.dwK35				= NULL;
	
	Rel60Ud1.bMakeActRpt			= TRUE;
	Rel60Ud1.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel60Ud1.wPickUpReportNum	= R_R60U1_PI;						//入段报告序列号
	Rel60Ud1.wResetReportNum	= R_R60U1_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel60Ud1.dwRelTimer			= 0;			 			//定时器
	Rel60Ud1.byRelFCounter		= 0;						//故障计数器
	Rel60Ud1.byRelRCounter		= 0;						//返回计数器		
	Rel60Ud1.wExcepWordNum		= 8;
	Rel60Ud1.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel60U2	 
// 功能: 初始化差压2保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel60Ud2(void)
{
	Rel60Ud2.pElecSet			= RS_pn60U2V;          					//过量保护电量定值
	Rel60Ud2.pTimeSet			= RS_pn60U2D;				 			//过量保护时间定值
	Rel60Ud2.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel60Ud2.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel60Ud2.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel60Ud2.pH2Coef				= NULL;

	Rel60Ud2.pRelCFG				= CFG_pby60U2;							//过量保护功能控制字--1:投入，2:退出 
	Rel60Ud2.pRelSW				= SW_pby60U;         						//过量保护软压板------1:投入，2:退出
	Rel60Ud2.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel60Ud2.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel60Ud2.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel60Ud2.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel60Ud2.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel60Ud2.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel60Ud2.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel60Ud2.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel60Ud2.pRelElec 			= &Ud2;									//当前电量指针		
	Rel60Ud2.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel60Ud2.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel60Ud2.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel60Ud2.StartRelayWord		= RW_PI60U2;						//过量保护入段继电器字
	Rel60Ud2.ActRelayWord		= RW_TR60U2;							//过量保护动作继电器字
	Rel60Ud2.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel60Ud2.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel60Ud2.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel60Ud2.PRewardRelayWord	= FALSE1;
	Rel60Ud2.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel60Ud2.LockRelayWord		= RW_LKPI60U2;						//入段闭锁继电器字
	Rel60Ud2.RSTRelayWord		= RW_LKRST60U2;							//返回闭锁继电器字
	Rel60Ud2.LoopCtrlRelayWord	= RW_LPC60U2;						//保护循环控制继电器字
	Rel60Ud2.wReportNum			= R_R60U2;								//过量保护报告序号
	Rel60Ud2.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel60Ud2.dwK35				= NULL;

	
	Rel60Ud2.pRel47UCFG		= &RelCFGCLR;;
	Rel60Ud2.R47ULKRelayWord   = FALSE1;
	Rel60Ud2.bMakeActRpt			= TRUE;
	Rel60Ud2.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel60Ud2.wPickUpReportNum	= R_R60U2_PI;						//入段报告序列号
	Rel60Ud2.wResetReportNum	= R_R60U2_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel60Ud2.dwRelTimer			= 0;			 			//定时器
	Rel60Ud2.byRelFCounter		= 0;						//故障计数器
	Rel60Ud2.byRelRCounter		= 0;						//返回计数器		
	Rel60Ud2.wExcepWordNum		= 8;
	Rel60Ud2.wInfo				 =0;

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel60U3	 
// 功能: 初始化差压3保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel60Ud3(void)
{
	Rel60Ud3.pElecSet			= RS_pn60U3V;          					//过量保护电量定值
	Rel60Ud3.pTimeSet			= RS_pn60U3D;				 			//过量保护时间定值
	Rel60Ud3.pHarmKhr1Set		= NULL;									//过量保护谐波加权系数
	Rel60Ud3.pVolLockSet			= NULL;				 	    				//过量保护电压闭锁定值
	Rel60Ud3.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel60Ud3.pH2Coef				= NULL;

	Rel60Ud3.pRelCFG				= CFG_pby60U3;							//过量保护功能控制字--1:投入，2:退出 
	Rel60Ud3.pRelSW				= SW_pby60U;         						//过量保护软压板------1:投入，2:退出
	Rel60Ud3.pRelHLockCFG		= &RelCFGCLR;			    					//谐波闭锁功能控制位--1:投入，2:退出
	Rel60Ud3.pRelHSuprCFG		= &RelCFGCLR; 							//谐波抑制功能控制位--1:投入，2:退出
	Rel60Ud3.pRelPDCFG			= &RelCFGCLR;		    						//功率方向功能控制位--1:投入，2:退出
	Rel60Ud3.pRelVolLockCFG		= &RelCFGCLR;								//低压闭锁功能控制位--1:投入，2:退出
	Rel60Ud3.pRelFDCFG			= &RelCFGCLR;								//功率方向功能控制位--1:正方向，2:反方向
	Rel60Ud3.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel60Ud3.pRelFLCFG			= &RelCFGCLR;								//故障测距投入
	Rel60Ud3.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel60Ud3.pRelElec 			= &Ud3;									//当前电量指针		
	Rel60Ud3.pRelHarm3			= &IP3;									//当前三次谐波指针	
	Rel60Ud3.pRelHarm5			= &IP5;									//当前五次谐波指针	
	Rel60Ud3.pRelVolt			= NULL;									//当前用来闭锁的电压的指针
	Rel60Ud3.pRel47UCFG		= &RelCFGCLR;;
	Rel60Ud3.R47ULKRelayWord   = FALSE1;

	Rel60Ud3.StartRelayWord		= RW_PI60U3;						//过量保护入段继电器字
	Rel60Ud3.ActRelayWord		= RW_TR60U3;							//过量保护动作继电器字
	Rel60Ud3.AccRelayWord		= FALSE1;								//后加速允许继电器字
	Rel60Ud3.AccActRelayWord		= FALSE1;								//后加速动作继电器字
	Rel60Ud3.PForwardRelayWord	= FALSE1;								//功率方向继电器字
	Rel60Ud3.PRewardRelayWord	= FALSE1;
	Rel60Ud3.H2LockRelayWord		= FALSE1;								//二次谐波闭锁继电器字
	Rel60Ud3.LockRelayWord		= RW_LKPI60U3;						//入段闭锁继电器字
	Rel60Ud3.RSTRelayWord		= RW_LKRST60U3;							//返回闭锁继电器字
	Rel60Ud3.LoopCtrlRelayWord	= RW_LPC60U3;						//保护循环控制继电器字
	Rel60Ud3.wReportNum			= R_R60U3;								//过量保护报告序号
	Rel60Ud3.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel60Ud3.dwK35				= NULL;
	
	Rel60Ud3.bMakeActRpt			= TRUE;
	Rel60Ud3.RptSerialNum		= &RptSerialNumOfAct;			
	//**********************入段返回报告部分*************	yanxs  13-03-21
	Rel60Ud3.wPickUpReportNum	= R_R60U3_PI;						//入段报告序列号
	Rel60Ud3.wResetReportNum	= R_R60U3_RES;						//返回报告序列号
	//**********************************************************************	
//	Rel60Ud3.dwRelTimer			= 0;			 			//定时器
	Rel60Ud3.byRelFCounter		= 0;						//故障计数器
	Rel60Ud3.byRelRCounter		= 0;						//返回计数器		
	Rel60Ud3.wExcepWordNum		= 8;
	Rel60Ud3.wInfo				= 0;

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRel27U	 
// 功能: 初始化低压保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel27U()
{
	Rel27U.pELeSet					= RS_pn27V;						//电量定值 
	Rel27U.pTimeSet					= RS_pn27D;						//时间定值 
	Rel27U.pLockCurSet				= RS_pn27_LK50;					//闭锁电流定值 

	Rel27U.pRelCFG					= CFG_pby27;					  	 //欠量保护配置 
	Rel27U.pRelSW					= SW_pby27;					  	 //欠量保护软压板 
	Rel27U.pRelAlarmCFG				= &RelCFGCLR;							//欠量保护投跳闸 
	Rel27U.pRelCurLockCFG			= &RelCFGSET;
	
	Rel27U.pSysTimer				= &dwRelayMSCnt;						//系统定时器指针 

	Rel27U.pRelElec					= &UP;						//当前的电量指针 
	Rel27U.pRelLockElec				= &IP;						//闭锁电流 
	Rel27U.ActRelayWord				= RW_TR27;					//欠量保护动作继电器字 
	Rel27U.StartRelayWord			= RW_PI27;					//欠量保护入段继电器字 
	Rel27U.AlarmRelayWord			= FALSE1;					//欠量保护告警继电器字 
	Rel27U.LockRelayWord			= RW_LKPI27;				//入段闭锁继电器字 
	Rel27U.RSTRelayWord				= RW_LKRST27;					//故障返回闭锁继电器字 
	Rel27U.LoopCtrlRelayWord		= RW_LPC27;				//故障循环控制继电器字 
	Rel27U.wR52BFRealyword			= RW_52BF;
	Rel27U.wActRptNum				= R_R27U;					//欠量保护动作报告序号 
	Rel27U.wAlarmRptNum				= 0xFFFF;					//欠量保护告警报告序号 
	Rel27U.wTRFailRptNum			= R_TRFAIL;					//断路器跳闸失败报告序号
	Rel27U.wPickUpRptNum			= R_ACTPICK;					//保护启动报告序号
	Rel27U.RptSerialNum				= &g_RptSerialNum;				//保护报告序列号
	
	Rel27U.R27RptSerialNum			= 0;
//	Rel27U.dwRelTimer				= 0;						//欠量保护定时器 	
	Rel27U.byRelFCounter			= 0;					//故障计数器 
	Rel27U.byRelRCounter			= 0;					//返回计数器 
	Rel27U.bHaveMake52BFRptFlag		= FALSE;
	Rel27U.wExcepWordNum			= 14;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
	Rel27U.wInfo				 	= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: InitRelPT	 
// 功能: 初始化PT断线保护元件 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRelPT()
{	
	RelPT.pRelPTBrokCFG 		= CFG_pby74PT;						//PT断线配置

	RelPT.pR74PTU1Set			= RS_pn74PTV;						//PT断线检测电压定值
	RelPT.pR74PTU2Set			= NULL;
	RelPT.pR74PTISet			= RS_pn74PTC;						//pt断线检测电流定值
	RelPT.pR74PTTSet			= RS_pn74PTD;						//pt断线检测时间定值
	
	RelPT.pRelCur				= &IP;							//判断用电流
	RelPT.pRelU1				= &U1;							//判断用第一路电压	
	RelPT.pRelU2				= NULL;

	RelPT.StartRelayWord		= RW_PI74PT;						//PT1断线入段继电器字
	RelPT.ActRelayWord			= RW_AR74PT;						//PT1断线动作继电器字
	RelPT.dwSysTimer			= &dwRelayMSCnt;						//系统定时器
	RelPT.wRelRptNum			= R_PTBR;						//PT断线动作报告号1
	RelPT.wRelResRptNum			= R_PTBR_RES;					//PT断线返回报告号1
	 
//	RelPT.dwRelTimer			= 0;							//PT1断线定时器	
	RelPT.wExcepWordNum			= 5;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为6
}
void CRelayTask::InitStructCheck(void)
{	
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&Rel50I0;
	StructCheck[0].bigsize = sizeof(Rel50I0)/2-Rel50I0.wExcepWordNum;
	StructCheck[1].pstruct = (WORD*)&Rel50I1;
	StructCheck[1].bigsize = sizeof(Rel50I1)/2-Rel50I1.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel59U;
	StructCheck[2].bigsize = sizeof(Rel59U)/2-Rel59U.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel50IH;
	StructCheck[3].bigsize = sizeof(Rel50IH)/2-Rel50IH.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel60I1;
	StructCheck[4].bigsize = sizeof(Rel60I1)/2-Rel60I1.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel60I2;
	StructCheck[5].bigsize = sizeof(Rel60I2)/2-Rel60I2.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel50ST;
	StructCheck[6].bigsize = sizeof(Rel50ST)/2-Rel50ST.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel60Ud1;
	StructCheck[7].bigsize = sizeof(Rel60Ud1)/2-Rel60Ud1.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&Rel60Ud2;
	StructCheck[8].bigsize = sizeof(Rel60Ud2)/2-Rel60Ud2.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&Rel60Ud3;
	StructCheck[9].bigsize = sizeof(Rel60Ud3)/2-Rel60Ud3.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&Rel27U;
	StructCheck[10].bigsize = sizeof(Rel27U)/2-Rel27U.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)&RelPT;
	StructCheck[11].bigsize = sizeof(RelPT)/2-RelPT.wExcepWordNum;
	StructCheck[12].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[12].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;	
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
{
	//重新初始化保护数据
	::InitAllRelayMem();
	InitRealy();	
	InitSet();
	InitDCRecInfo();
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
	BYTE  pChannel[]={CHANNEL_6560_IP};
	if(ReadRelayWord(RW_52BF))  //断路器跳闸失败，不进行保护启动判断处理 
	{
		WriteRelayWord(PickUpRW, RELAY_OFF);
		return FALSE;
	}
	if((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(Rel50I0.pRelElec->Mod > *Rel50I0.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*Rel50I1.pRelCFG == CFG_STATUS_SET)&& (*Rel50I1.pRelSW == SW_STATUS_SET)) //过流投入 
	{
		if(Rel50I1.pRelElec->Mod > *Rel50I1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50IH.pRelCFG == CFG_STATUS_SET) && (*Rel50IH.pRelSW == SW_STATUS_SET)) //谐波过流投入 
	{
		if(Rel50IH.pRelElec->Mod > *Rel50IH.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60I1.pRelCFG == CFG_STATUS_SET)&& (*Rel60I1.pRelSW == SW_STATUS_SET)) //差流1段投入
	{
		if(Rel60I1.pRelElec->Mod > *Rel60I1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60I2.pRelCFG == CFG_STATUS_SET) && (*Rel60I2.pRelSW == SW_STATUS_SET)) //差流2段投入
	{
		if(Rel60I2.pRelElec->Mod > *Rel60I2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50ST.pRelCFG == CFG_STATUS_SET) && (*Rel50ST.pRelSW == SW_STATUS_SET)) //支路过流投入
	{
		if(Rel50ST.pRelElec->Mod > *Rel50ST.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60Ud1.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud1.pRelSW == SW_STATUS_SET)) //差压1投入
	{
		if(Rel60Ud1.pRelElec->Mod > *Rel60Ud1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60Ud2.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud2.pRelSW == SW_STATUS_SET)) //差压2投入
	{
		if(Rel60Ud2.pRelElec->Mod > *Rel60Ud2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel60Ud3.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud3.pRelSW == SW_STATUS_SET)) //差压3投入
	{
		if(Rel60Ud3.pRelElec->Mod > *Rel60Ud3.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel59U.pRelCFG == CFG_STATUS_SET) && (*Rel59U.pRelSW == SW_STATUS_SET)) //过压保护投入
	{
		if(Rel59U.pRelElec->Mod > *Rel59U.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}

//突变量启动判断部分
	//至少有一个保护投入
	if(((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET)) || \
		((*Rel50I1.pRelCFG == CFG_STATUS_SET) && (*Rel50I1.pRelSW == SW_STATUS_SET)) || \
		((*Rel50IH.pRelCFG == CFG_STATUS_SET) && (*Rel50IH.pRelSW == SW_STATUS_SET)) || \
		((*Rel60I1.pRelCFG == CFG_STATUS_SET) && (*Rel60I1.pRelSW == SW_STATUS_SET)) || \
		((*Rel60I2.pRelCFG == CFG_STATUS_SET) && (*Rel60I2.pRelSW == SW_STATUS_SET))  || \
		((*Rel60Ud1.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud1.pRelSW == SW_STATUS_SET)) || \
		((*Rel60Ud2.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud2.pRelSW == SW_STATUS_SET)) || \
		((*Rel60Ud3.pRelCFG == CFG_STATUS_SET) && (*Rel60Ud3.pRelSW == SW_STATUS_SET)) || \
		((*Rel50ST.pRelCFG == CFG_STATUS_SET) && (*Rel50ST.pRelSW == SW_STATUS_SET)) ||	\
		((*Rel59U.pRelCFG == CFG_STATUS_SET) && (*Rel59U.pRelSW == SW_STATUS_SET)))
	
	{
		PickCheck = 0;
		ReadPreSingleData(dwPickDataBuf,pChannel,1,RELAY_SAMP_INTERVAL);
		//算法1，根据宏RELAY_SAMP_INTERVAL确定的通用算法
	//将3个周波的采样点分别取至相应的数组,并计算突变量,且进行判断
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i];
			lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i];
			lSampDate3 = dwPickDataBuf[i];
			lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));

			
	//		if(lPickValue>*RS_pnPIC)
				
			//采用浮动门槛值判断，ΔI>0.25*ΔI'+ C,其中ΔI'为上个一个邻近的点的突变量值，C为突变量启动定值
			if(lPickValue*4>*RS_pnPIC*4+m_lPickUpValue)
			{
				PickCheck++;
				if(PickCheck ==3)
				{					
					QWORD PickTime; 					
					ReadAbsTime(&PickUpTime);
					PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
					PickUpTime.Lo= (DWORD)PickTime;
					PickUpTime.Hi= (WORD)(PickTime>>32);	

					WriteRelayWord(PickUpRW, RELAY_ON);
					ReadAbsTime(&PickUpTime);
					DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
					RptSerialNumOfAct = g_RptSerialNum;
					DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
					RptSerialNumOfStart = g_RptSerialNum;
					PickCheck =0;
					PickCheckNext =0;
					return TRUE;	
				}
				if(i<2)
				{
					PickCheckNext++;
					if(PickCheckNext ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi<<32+PickUpTime.Lo-i*625/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	

						WriteRelayWord(PickUpRW, RELAY_ON);
						ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
						RptSerialNumOfStart = g_RptSerialNum;
						PickCheck =0;
						PickCheckNext =0;
						return TRUE;	
					}
				}
				if((i>=2)&&((RELAY_SAMP_INTERVAL-i) >= 3))
					PickCheckNext =0;
				
				if((RELAY_SAMP_INTERVAL-i) < 3)
					PickCheckNext++;
			}
			else
			{
				PickCheck =0;
				PickCheckNext=0;
			}
			m_lPickUpValue = lPickValue;
		}	
	}
	
	WriteRelayWord(PickUpRW, RELAY_OFF);
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
	//LONG checktime=0;
	if(FaultOnFlag ==FALSE)   						//尚未进入故障处理
	{		
		RelayPickUpCHK();					//执行启动检测
		if(ReadRelayWord(PickUpRW))
		{
			ResetAllRelay(TRUE);
			FaultSINum = RELAY_SAMP_INTERVAL;       //保护启动，初始化故障处理采样间隔计数器
			FaultOnFlag = TRUE;
			WriteRelayWord(RelPT.StartRelayWord,RELAY_OFF);
//			return;
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//故障处理采样间隔计数器累加
	if(FaultSINum<POINT_PER_PERIOD)					//故障处理采样间隔计数器小于一周波采样点数
	{
		return;
	}
	else
	{
		if(HaveMakeStartRptFlag == FALSE)
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}
		
		GeneralOverValueRelay(&Rel50I0);				//调用速断子程序
		GeneralOverValueRelay(&Rel50I1);				//调用过流保护子程序
		GeneralOverValueRelay(&Rel60I1);				//调用差流1保护子程序
		GeneralOverValueRelay(&Rel60I2);				//调用差流2保护子程序
		GeneralOverValueRelay(&Rel50ST);				//调用支路过流保护子程序
		GeneralOverValueRelay(&Rel50IH);				//调用谐波过流保护子程序
	 	GeneralOverValueRelay(&Rel60Ud1);				//调用差压1保护子程序
		GeneralOverValueRelay(&Rel60Ud2);				//调用差压2保护子程序
		GeneralOverValueRelay(&Rel60Ud3);				//调用差压3保护子程序
		GeneralOverValueRelay(&Rel59U);				//调用过压保护子程序
	

		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveMakeTripFailRpt)
			{
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   //作断路器跳闸失败报告
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}
		if(ReadRelayWord(Rel50I0.LoopCtrlRelayWord)||ReadRelayWord(Rel50I1.LoopCtrlRelayWord)||ReadRelayWord(Rel60I1.LoopCtrlRelayWord)  		//有保护循环控制继电器字被置位
			||ReadRelayWord(Rel60I2.LoopCtrlRelayWord)||ReadRelayWord(Rel50ST.LoopCtrlRelayWord)||ReadRelayWord(Rel60Ud1.LoopCtrlRelayWord) 
			||ReadRelayWord(Rel60Ud2.LoopCtrlRelayWord)||ReadRelayWord(Rel60Ud3.LoopCtrlRelayWord)||ReadRelayWord(Rel59U.LoopCtrlRelayWord)
			||ReadRelayWord(Rel50IH.LoopCtrlRelayWord))
		{
			
			if(ReadRelayWord(RW_52BF))			//跳闸失败，清所有标志，作结束报告
			{
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   //作断路器跳闸失败报告
				WriteRelayWord(PickUpRW, RELAY_OFF);
				FaultCheckOutFlag = FALSE;
				FaultOnFlag = FALSE;
				HaveMakeStartRptFlag= FALSE;
				
				m_bHaveMakeTripFailRpt = FALSE;
				DB_RelayEnd(RptSerialNumOfAct); //向数据库中写入结束报告
				return;
			}
			else
			{
				FaultLockTime = dwRelayMSCnt;		//重置故障锁定定时器 
				FaultCheckOutFlag = TRUE;			//有保护循环控制继电器字置位，置检出故障标志
				return;
			}
		}
		else				//无保护循环控制继电器置位: 1、故障处理已经结束；2、未发生过故障
		{
			if(FaultCheckOutFlag ==TRUE	)      // 1、故障处理已经结束
			{			
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime>500 )      //故障处理结束，无故障时间超过1秒，复位保护启动继电器字，清标志，作结束报告
				{	
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;
					
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct);	
					ResetAllRelay(TRUE);
					return;					
				}
				else
					return;
			}
			else							//未发生过故障
			{
				if(m_bFaultLKFlag == FALSE)     //故障刚启动，记录起始故障时刻
				{	
					FaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				
				if(dwRelayMSCnt -FaultLockTime>1000 )	//未发生过故障无故障时间超过0.5秒，复位保护启动继电器字，清标志，作结束报告
				{
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;
					
					m_bHaveMakeTripFailRpt = FALSE;
					DB_RelayEnd(RptSerialNumOfAct); 
					m_bFaultLKFlag = FALSE;
					ResetAllRelay(TRUE);
					return;					
				}
				else
					return;
			}			
		}		
	}
}
void CRelayTask::Process52BF()
{
	if(ReadRelayWord(RW_52BF))
	{
		m_b52BFFlag = TRUE;
	}
	if(m_b52BFFlag)
	{
		if(!ReadRelayWord(RW_52BF))
		{
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&Rel27U);
			m_b52BFFlag = FALSE;
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
/*	if(ReadRelayWord(RW_52BF))			//跳闸失败继电器字置位
	{
		TripFailFlag=TRUE;					//置跳闸失败标志
		return;
	}
	else									//无跳闸失败继电器字置位，有跳闸失败标志，整组复归，即读取了跳闸失败继电器字的下降沿
	{
		if(TripFailFlag)
		{
*/			
			ResetOverValueRelay(&Rel50I0,resetFlag);
			ResetOverValueRelay(&Rel50I1,resetFlag);
			ResetOverValueRelay(&Rel60I1,resetFlag);
			ResetOverValueRelay(&Rel60I2,resetFlag);
			ResetOverValueRelay(&Rel50ST,resetFlag);
			ResetOverValueRelay(&Rel60Ud1,resetFlag);
			ResetOverValueRelay(&Rel60Ud2,resetFlag);
			ResetOverValueRelay(&Rel60Ud3,resetFlag);
			ResetOverValueRelay(&Rel59U,resetFlag);
			ResetOverValueRelay(&Rel50IH,resetFlag);
//			TripFailFlag=FALSE;
//		}		
//	}	
}
