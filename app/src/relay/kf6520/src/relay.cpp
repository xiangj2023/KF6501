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
#include "bo.h"
#include "daefunc.h"

extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;			//AD当前写指针
extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern BYTE 	g_RptSerialNum;			//报告序列号 
extern BOOL		RelayCheckErr;			//保护元件自检出错标志
extern BOOL 	RelayCheckInitOK;		//保护元件自检初始化标志
extern BYTE		*g_MaintSW;

TLoadSupV * pTodayImax = (TLoadSupV *)DBASE_NVRAM_6520;
TLoadSupV * pTodayUmin = pTodayImax+1;
TLoadSupV * pYtdayImax = pTodayImax+2;
TLoadSupV * pYtdayUmin = pTodayImax+3;
TLoadSupV * pDaybyImax = pTodayImax+4;
TLoadSupV * pDaybyUmin = pTodayImax+5;
TSysTime  * pRecurrenceTime = (TSysTime*)(pDaybyUmin+1);  //数据交换时间
DWORD	  * pDataValidity = (DWORD *)(pRecurrenceTime+1);//数据有效性检查值
WORD	  * g_pHaveSwitchData = (WORD*)(pDataValidity+1);//数据交换标志

DWORD		dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD		dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
DWORD 		dwRelayTaskID;
BOOL 		RelayTaskInitOK = FALSE;
TStructCheck StructCheck[CHECK_NUM];
extern DWORD dwADResumeCnt;
BOOL				g_FaultOnFlag;				//进入故障循环处理标志 

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
	//初始化保护报告序列号
	InitRepSerialNum();	
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
//	RptSerialNumOfPick = 0;			//系统入段报告序列号    yanxs  13-02-05
//	RptFlagOfPickUp = FALSE ;			//系统入段报告已经启动标志
	RptSerialNumOfAct =0;			//系统的保护动作报告序列号
	RptSerialNumOfStart=0;			//系统的带故障电量的启动报告序列号	
	FaultSINum = 0;					//采样间隔计数器	
	
	PickUpRW = RW_PICKUP;			//模值突变量启动继电器字
	TripFailFlag = FALSE;			//跳闸失败标志
	FaultOnFlag = FALSE;			//进入故障处理标志
	g_FaultOnFlag = FALSE;			//进入故障处理标志
	HaveMakeStartRptFlag = FALSE;	//已作带故障电量启动报告标志
	m_bFaultLKFlag = FALSE;			//故障起始时刻锁定标志
	CurRecoverFlag =FALSE;   		//重合闸动作后电流恢复标志
	FaultCheckOutFlag = FALSE;		//检出故障标志
	m_bIsHaveMakePTRpt = FALSE;		//已作PT断线闭锁距离保护报告标志
	m_bLastRecStatusFlag = FALSE;	//故障录波启动上升沿判断标志
	m_bOneOhmPTLK= FALSE;			//电阻电抗小于1欧姆标志
	m_bPDCheck = FALSE;				//已作带记忆功率方向检测标志
	m_lOtherRelayTrip = FALSE;	
	m_bHaveRelayAct	= FALSE;		//本次故障处理中保护已经动作标志
	m_b52BFFLAG = FALSE;
	m_bLogicChanged = FALSE;
	m_bR79PickUpRpt = FALSE;
	m_bHaveTripFailedRpt = FALSE;
	
	K35 = 0;						//谐波抑制K35
	PickCheck = 0;					//突变量启动判断连续点计数器
	PickCheckNext = 0;				//突变量启动判断两次任务间连续点计数器	
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
	dwChanDataMask = ( (0x01<<CHANNEL_6520_It)|(0x01<<CHANNEL_6520_If)|(0x01<<CHANNEL_6520_U1)|\
						(0x01<<CHANNEL_6520_U2)|(0x01<<CHANNEL_6520_U3)|(0x01<<CHANNEL_6520_U4));
	//增量保护数据读取通道掩码
	dwInCreDataMask=(0x01<<CHANNEL_6520_IP);
	//回写合成数据用通道掩码
	dwComposeDataMask = ((0x01<<CHANNEL_6520_IP)|(0x01<<CHANNEL_6520_UP));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));
	
	//初始化读取保护数据临时缓冲区
	//保护数据缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);	
	//合成数据回写缓冲区
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);
	//增量保护计算数据缓冲区
	dwIncreCurBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);	
	//合成通道数据缓冲区
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);
	//突变量启动历史数据缓冲区
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3);
	//带记忆功率方向判断数据缓冲区
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);

	ASSERT(dwIncreCurBuf != NULL);
	if(dwIncreCurBuf == NULL)
	{
		LogError("CRelayTask,dwIncreCurBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwIncreCurBuf, 0, sizeof(DWORD)*READ_POINT_NUM);

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

	//下面是初始化逻辑方程部分
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	
	*RM_pnTL = 0;
	*RM_pnFL = 0;
	g_MaintSW = SW_pbyMAINT;

	//初始化开出自检为正常状态
	
	for(LONG i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();

	InitLoadMonitor();

	RelayTaskInitOK = TRUE;
	SetTimer(1,5);

//将阻抗值初始化为默认值，防止直供方式下，F线阻抗值显示有问题
	Impd.Real = INVALIDRX*(*CF_pnZ);
	Impd.Imag = INVALIDRX*(*CF_pnZ);
	ImpdT.Real = INVALIDRX*(*CF_pnZ);
	ImpdT.Imag = INVALIDRX*(*CF_pnZ);	
	ImpdF.Real = INVALIDRX*(*CF_pnZ);
	ImpdF.Imag = INVALIDRX*(*CF_pnZ);	

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
		WriteRelayWord(RW_BTMODE, RELAY_ON);
		WriteRelayWord(RW_AT1MODE, RELAY_OFF);
		WriteRelayWord(RW_AT2MODE, RELAY_OFF);
	}
	else if(*AS_pnPM == AT1MODE)
	{
		WriteRelayWord(RW_BTMODE, RELAY_OFF);
		WriteRelayWord(RW_AT1MODE, RELAY_ON);
		WriteRelayWord(RW_AT2MODE, RELAY_OFF);
	}
	
	else if(*AS_pnPM == AT2MODE)
	{
		WriteRelayWord(RW_BTMODE, RELAY_OFF);
		WriteRelayWord(RW_AT1MODE, RELAY_OFF);
		WriteRelayWord(RW_AT2MODE, RELAY_ON);
	}
}
/************************************************************************************************
*功能: 初始化保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	InitRel50I0();
	InitRel50I1();
	InitRel50I2();
	InitRel50I3();
	InitRel211();
	InitRel212();
	InitRel213();
	InitRel214();
	InitRel27U();
    InitRel51I();
	InitRel21FL_F();	
	InitRel21FL_T();
	InitRelPT1();
	InitRelPT2();
	InitRel79();
	InitRel50DI();
	InitStructCheck();
	
}
void CRelayTask::OnTimeOut(DWORD id)
{		
//	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);
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

	//合成Up,Ip通道并回写
	WriteComposeBuf();
	//计算电量It/If/U1/U2
	CaluUI(); 
	//计算Ip/Up以及谐波分量
	CaluUpIp();	
	CaluUIAngle();
	//计算阻抗
	CaluZ();

	//谐波抑制的K35计算K35=1+k(I3+I5)/I1,放大100倍
	if(!ReadRelayWord(RW_37IP))
	{
		//距离谐波抑制、速断谐波抑制、过流谐波抑制有投入的情况下计算K35
		if((*CFG_pby21_HRES== CFG_STATUS_SET) || (*CFG_pby50_0_HRES == CFG_STATUS_SET) || (*CFG_pby50_HRES== CFG_STATUS_SET)) 
		{
			//如果EEPROM乱了，RW_37IP可能至不上，此时要保证IP大于0
			if((Ip.Mod*100<(I3.Mod+I5.Mod))||(Ip.Mod<2)) 
				K35 = 10000;
			else				
				K35 = *CF_pnC2 + (*RS_pnFACTOR1)*(I3.Mod+I5.Mod)/Ip.Mod;
		}
	}
	else
		K35 = *CF_pnC2;

	//进入故障处理之后不进行PT断线判断
	if(FaultOnFlag == FALSE)
	{
		PTCheck();	
	}
	FalutProcess();								//故障循环处理
	GeneralUnderValRelay(&Rel27U);				//低电压保护子程序
	AutoRCAct(&Rel79);							//调用重合闸子程序
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
	//负荷监测
	LoadMonitor();
	
	//处理保护故障处理中防止数据窗不满的情况下保护误动作
//	if(m_bNextRelayStart)
//	{
//		if(!ReadRelayWord(RW_37IP))  //有电流
//		{
//			FaultSINum = 0;
//			m_bNextRelayStart = FALSE;
//		//	FaultCheckOutFlag = FALSE;
//		}
//	}

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
// 名称: 负荷监测                                                           							          
// 输入: 无                                       
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::LoadMonitor()
{	
	//负荷监测	
	static WORD SwitchTimes=0;
	LONG CurrentH;
	LONG VoltageH;
//	LONG AngleCurr;
	LONG AngleVolt;
	TSysTime SysTime;
	TAbsTime AbsTime;
	CurrentH = *AS_pnPCTRatio*Ip.Mod;
	VoltageH = *AS_pnPT1Ratio*Up.Mod;
//	AngleCurr = 0;
	AngleVolt = Up.Angle - Ip.Angle;
	ReadSystemTime(&SysTime);
	ReadAbsTime(&AbsTime);
	if(SysTime.Hour == (BYTE)*AS_pnLMT)
	{
		if(*g_pHaveSwitchData == FALSE)
		{
			*pDaybyImax = *pYtdayImax;
			*pDaybyUmin = *pYtdayUmin;
			*pYtdayImax = *pTodayImax;
			*pYtdayUmin = *pTodayUmin;
			if(!ReadRelayWord(RW_37IP)&&(CurrentH < *AS_pnLMCMAX)) //数据交换时将符负荷监测记录为交换时刻实际电压电流值
			{
				pTodayImax->Current = CurrentH;			
				pTodayImax->Voltage = VoltageH;
//				pTodayImax->AngleCurr = AngleCurr;			
				pTodayImax->AngleVolt = AngleVolt;
			}
			else			//数据交换时将符负荷监测记录为0
			{
				pTodayImax->Current = 0;			
				pTodayImax->Voltage = 0;
//				pTodayImax->AngleCurr = 0;			
				pTodayImax->AngleVolt = 0;
			}
			pTodayImax->Time	= AbsTime;
			if(!ReadRelayWord(RW_37IP)&&(VoltageH > (*AS_pnLMVMIN)*10)) //数据交换时将符负荷监测记录为交换时刻实际电压电流值
			{
				pTodayUmin->Current = CurrentH;		
				pTodayUmin->Voltage = VoltageH;
//				pTodayUmin->AngleCurr = AngleCurr;			
				pTodayUmin->AngleVolt = AngleVolt;
			}
			else  //数据交换时将符负荷监测记录为0
			{
				pTodayUmin->Current = 0;		
				pTodayUmin->Voltage = 0;
//				pTodayUmin->AngleCurr = 0;			
				pTodayUmin->AngleVolt = 0;
			}
			pTodayUmin->Time	= AbsTime;
//			pTodayImax->Current = 0;//数据交换之后几天的电流值置为0,电压置为200V(考虑到客专AT模式)
//			pTodayUmin->Voltage = *AS_pnPT1Ratio*(*CF_pnUP)*2;
			*pRecurrenceTime = SysTime;
			*g_pHaveSwitchData = TRUE;
			ReportMsg("load monitor: switch data!%d-%d,%d:%d:%d %d",SysTime.Month,SysTime.Day,SysTime.Hour,SysTime.Minute,SysTime.Second,SysTime.MSecond);
			return;
		}
	}
	else
	{
		*g_pHaveSwitchData = FALSE;
	}		
	if(!ReadRelayWord(RW_37IP))
	{
		if((CurrentH > pTodayImax->Current)&&(CurrentH < *AS_pnLMCMAX))
		{
			pTodayImax->Current = CurrentH;
			pTodayImax->Voltage = VoltageH;
//			pTodayImax->AngleCurr = AngleCurr;			
			pTodayImax->AngleVolt = AngleVolt;
			pTodayImax->Time	= AbsTime;
		}
		if(VoltageH > (*AS_pnLMVMIN)*10)
		{
			if(pTodayUmin->Voltage == 0) //第一次初始化时
			{
				pTodayUmin->Current = CurrentH;		
				pTodayUmin->Voltage = VoltageH;
//				pTodayUmin->AngleCurr = AngleCurr;			
				pTodayUmin->AngleVolt = AngleVolt;
				pTodayUmin->Time	= AbsTime;
//				pTodayUmin->Voltage = *AS_pnPT1Ratio*(*CF_pnUP)*2;
			}
//			if((VoltageH < pTodayUmin->Voltage)&&(VoltageH > (*AS_pnLMVMIN)*10))
			if(VoltageH < pTodayUmin->Voltage)
			{
				pTodayUmin->Current = CurrentH;		
				pTodayUmin->Voltage = VoltageH;
//				pTodayUmin->AngleCurr = AngleCurr;			
				pTodayUmin->AngleVolt = AngleVolt;
				pTodayUmin->Time	= AbsTime;
			}
		}
	}
	//WriteRelayWord(RW_37IP, RELAY_ON);
}
//------------------------------------------------------------------------------------------ 
// 名称: 写合成采样数据缓冲区	                                                           							          
// 输入:  pBuf1,原始物理采样点数据缓冲区；pBuf2,合成采样点数据缓冲区；dwMask,通道掩码                                        
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::WriteComposeBuf()
{	
	
	if(*AS_pnPM == BTMODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++) 
		{
			dwComposeBuf[i]=dwSampBuf[i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)   //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，通道为Up,16
		{
			dwRecBuf[i] = dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		}

		for(LONG i=0;i<READ_POINT_NUM;i++)	 
		{
			dwComposeBuf[READ_POINT_NUM+i]=dwSampBuf[READ_POINT_NUM*2+i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)   //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，通道为Ip,17
		{
			dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwSampBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		}

	}
	else if(*AS_pnPM == AT1MODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++) 
		{
			dwComposeBuf[i]=dwSampBuf[i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)   //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，通道为Up,16
		{
			dwRecBuf[i] = dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		}

		for(LONG i=0;i<READ_POINT_NUM;i++)	 
		{
			dwComposeBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*2+i]-dwSampBuf[READ_POINT_NUM*3+i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)  //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，通道为Ip,17
		{
			dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwSampBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		}

	}
	else if(*AS_pnPM == AT2MODE)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)     //由U1、U2通道采样点数据合成Up通道采样点数据 
		{
			dwComposeBuf[i]=dwSampBuf[i]-dwSampBuf[READ_POINT_NUM+i];
		}		
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，通道为Up,16
		{
			dwRecBuf[i] = dwSampBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		}

		for(LONG i=0;i<READ_POINT_NUM;i++)	 
		{
			dwComposeBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*2+i]-dwSampBuf[READ_POINT_NUM*3+i];
		}
		
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)  //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，通道为Ip,17
		{
			dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwSampBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		}

	}

	WriteComposeData(dwRecBuf, dwComposeDataMask);
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
	CaluBaseFourier_S15(dwSampBuf,&U1);
	CaluModValue(&U1);
	*RM_pnU1= U1.Mod;                                       
	DB_LogWriteRM(RM_U1, *RM_pnU1);	
	if(U1.Mod>=*AS_pn59U1V)	
	{	
		WriteRelayWord(RW_59U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(U1.Mod>=*AS_pn27U1V)
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}

	//计算U2模值并写保护测量、置U2有压继电器字、无压继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	*RM_pnU2= U2.Mod;                                       
	DB_LogWriteRM(RM_U2, *RM_pnU2);	
	if(U2.Mod>=*AS_pn59U2V)		
	{	
		WriteRelayWord(RW_59U2, RELAY_ON);
	}
	else	
	{	
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(U2.Mod>=*AS_pn27U2V)
	{	
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}
	else
	{	
		WriteRelayWord(RW_27U2, RELAY_ON);
	}
	 //计算It模值并写保护测量、置It有流继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&It);
	CaluModValue(&It);
	*RM_pnIt= It.Mod;                                       
	DB_LogWriteRM(RM_IT, *RM_pnIt);
	if(It.Mod>(*AS_pn50ITC))
	{				
		WriteRelayWord(RW_50IT, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IT, RELAY_OFF);
	}
	if(It.Mod>(*AS_pn37ITC))
	{
		WriteRelayWord(RW_37IT, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IT, RELAY_ON);
	}	
	 //计算If模值并写保护测量、置If有流继电器字
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&If);
	CaluModValue(&If);
	*RM_pnIf= If.Mod;                                       
	DB_LogWriteRM(RM_IF, *RM_pnIf);
	if(If.Mod>*AS_pn50IFC)
	{				
		WriteRelayWord(RW_50IF, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IF, RELAY_OFF);
	}
	if(If.Mod>(*AS_pn37IFC))
	{
		WriteRelayWord(RW_37IF, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IF, RELAY_ON);
	}	
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&U3);
	CaluModValue(&U3);
	*RM_pnU3= U3.Mod;                                       
	DB_LogWriteRM(RM_U3, *RM_pnU3);
	if(U3.Mod>*AS_pn59U3V)
	{				
		WriteRelayWord(RW_59U3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3, RELAY_OFF);
	}
	if(U3.Mod>(*AS_pn27U3V))
	{
		WriteRelayWord(RW_27U3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3, RELAY_ON);
	}	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&U4);
	CaluModValue(&U4);
	*RM_pnU4= U4.Mod;                                       
	DB_LogWriteRM(RM_U4, *RM_pnU4);
	if(U4.Mod>*AS_pn59U4V)
	{				
		WriteRelayWord(RW_59U4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U4, RELAY_OFF);
	}
	if(U4.Mod>(*AS_pn27U4V))
	{
		WriteRelayWord(RW_27U4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U4, RELAY_ON);
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
	//客专AT供电方式，UP=Ut-Uf
	if(*AS_pnPM == AT2MODE)
	{
		CaluBaseFourier_S15(dwComposeBuf,&Up);
		CaluModValue(&Up);
	}
	else
	{
		Up=U1;
	}	
	*RM_pnUp= Up.Mod;                                       
	DB_LogWriteRM(RM_UP, *RM_pnUp);	
	if(Up.Mod >= *AS_pn59UPV)		//置Up有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59UP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UP, RELAY_OFF);
	}
	if(Up.Mod >= *AS_pn27UPV)
	{
		WriteRelayWord(RW_27UP, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UP, RELAY_ON);
	}
	
	//AT供电方式下IP=It-If
	if(*AS_pnPM == BTMODE)
 	{
		Ip=It;
 	}
	else
	{		
		CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&Ip);  
		CaluModValue(&Ip);	 	
	}
	*RM_pnIp= Ip.Mod;                                       
	DB_LogWriteRM(RM_IP, *RM_pnIp);	
	if(Ip.Mod>*AS_pn50IPC)		
	{
		WriteRelayWord(RW_50IP, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IP, RELAY_OFF);
	}
	if(Ip.Mod>*AS_pn37IPC)
	{
		WriteRelayWord(RW_37IP, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IP, RELAY_ON);
	}	

	//计算I2模值并写保护测量
	if(*AS_pnPM == BTMODE)
	 	CaluSecondFourier(dwSampBuf+READ_POINT_NUM*2,&I2);		
	else
	 	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM,&I2);	 		 
	CaluModValue(&I2);
	I2.Mod = I2.Mod*CurHarmCal[0]/1000;
	*RM_pnI2= I2.Mod;                                       
	DB_LogWriteRM(RM_I2, *RM_pnI2);	

	//计算I3模值并写保护测量	
	if(*AS_pnPM == BTMODE)	 
	 	CaluThirdFourier(dwSampBuf+READ_POINT_NUM*2,&I3);
	else
	 	CaluThirdFourier(dwComposeBuf+READ_POINT_NUM,&I3);	 
	CaluModValue(&I3);
	I3.Mod = I3.Mod*CurHarmCal[1]/1000;
	*RM_pnI3= I3.Mod;                                       
	DB_LogWriteRM(RM_I3, *RM_pnI3);

	//计算I5模值并写保护测量
	if(*AS_pnPM == BTMODE)	 
		CaluFifthFourier(dwSampBuf+READ_POINT_NUM*2,&I5);		
	else	 	
	 	CaluFifthFourier(dwComposeBuf+READ_POINT_NUM,&I5);	 
	CaluModValue(&I5);
	I5.Mod = I5.Mod*CurHarmCal[2]/1000;
	*RM_pnI5= I5.Mod;                                       
	DB_LogWriteRM(RM_I5, *RM_pnI5);

	if(*AS_pnPM == AT2MODE) //客专供电方式
	{
		if(U1.Mod>=U2.Mod)
		{
			if(U2.Mod>=Up.Mod)
				ULock = Up;
			else
				ULock = U2;
		}
		else
		{
			if(U1.Mod>=Up.Mod)
				ULock = Up;
			else
				ULock = U1;
		}
	}
	else
		ULock = Up;
	
}
void CRelayTask::CaluZ(void)
{
	//BT供电方式下Zp=Zt
	if (*AS_pnPM == BTMODE)
	{
		//计算T线阻抗	
		CaluRX(&U1,&It,&ImpdT,RW_37IP,CF_pnZ);	//计算T线阻抗
		CaluImpdAngle(&ImpdT,AngleTab);
		*RM_pnTR= ImpdT.Real;  
		*RM_pnTX= ImpdT.Imag;
		*RM_pnTZA= ImpdT.Angle;
		DB_LogWriteRM(RM_TR, *RM_pnTR); 
		DB_LogWriteRM(RM_TX, *RM_pnTX); 
		DB_LogWriteRM(RM_TZA, *RM_pnTZA);
		
		Impd=ImpdT;		
		*RM_pnR= Impd.Real;  
		*RM_pnX= Impd.Imag;
		*RM_pnZA = Impd.Angle;
		DB_LogWriteRM(RM_R, *RM_pnR);	
		DB_LogWriteRM(RM_X, *RM_pnX);
		DB_LogWriteRM(RM_ZA, *RM_pnZA);			
		*M_pnR = Impd.Real;		
		*M_pnX = Impd.Imag;
		DB_WriteAI(M_R, *M_pnR);
		DB_WriteAI(M_X, *M_pnX);
	}	
	else    
	{
		if(*AS_pnPM == AT2MODE)			//AT2供电方式下计算T,F线阻抗
		{			
			CaluRX(&U1,&It,&ImpdT,RW_37IP,CF_pnZ);	//计算T线阻抗
			CaluImpdAngle(&ImpdT,AngleTab);
			*RM_pnTR= ImpdT.Real;  
			*RM_pnTX= ImpdT.Imag;
			*RM_pnTZA= ImpdT.Angle;
			DB_LogWriteRM(RM_TR, *RM_pnTR); 
			DB_LogWriteRM(RM_TX, *RM_pnTX); 
			DB_LogWriteRM(RM_TZA, *RM_pnTZA);

			CaluRX(&U2,&If,&ImpdF,RW_37IP,CF_pnZ);	//计算F线阻抗
			CaluImpdAngle(&ImpdF,AngleTab);
			*RM_pnFR= ImpdF.Real;  
			*RM_pnFX= ImpdF.Imag;
			*RM_pnFZA = ImpdF.Angle;
			DB_LogWriteRM(RM_FR, *RM_pnFR);	
			DB_LogWriteRM(RM_FX, *RM_pnFX);	
			DB_LogWriteRM(RM_FZA, *RM_pnFZA);	
		}

		CaluRX(&Up,&Ip,&Impd,RW_37IP,CF_pnZ);
		CaluImpdAngle(&Impd,AngleTab);
		*RM_pnR= Impd.Real;  
		*RM_pnX= Impd.Imag;
		*RM_pnZA = Impd.Angle;
		DB_LogWriteRM(RM_R, *RM_pnR);	
		DB_LogWriteRM(RM_X, *RM_pnX);	
		DB_LogWriteRM(RM_ZA, *RM_pnZA);	
		*M_pnR = Impd.Real;		
		*M_pnX = Impd.Imag;
		DB_WriteAI(M_R, *M_pnR);
		DB_WriteAI(M_X, *M_pnX);			
	}
}
void CRelayTask::CaluUIAngle(void)
{	
	LONG angle;
	CaluAngle(&Ip,AngleTab);
	*RM_pnAIP=0;
	DB_LogWriteRM(RM_AIP, *RM_pnAIP);
	CaluAngle(&U1,AngleTab);
	angle = U1.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAU1 = angle+36000;
	else
		*RM_pnAU1 = angle;		
	DB_LogWriteRM(RM_AU1, *RM_pnAU1);
	
	CaluAngle(&U2,AngleTab);	
	angle = U2.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAU2 = angle+36000;
	else
		*RM_pnAU2 = angle;	
	DB_LogWriteRM(RM_AU2, *RM_pnAU2);	
	
	CaluAngle(&Up,AngleTab);	
	angle = Up.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAUP = angle+36000;
	else
		*RM_pnAUP = angle;	
	DB_LogWriteRM(RM_AUP, *RM_pnAUP);	
	
	CaluAngle(&It,AngleTab);	
	angle = It.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAIT = angle+36000;
	else
		*RM_pnAIT = angle;	
	DB_LogWriteRM(RM_AIT, *RM_pnAIT);	
	
	CaluAngle(&If,AngleTab);	
	angle = If.Angle-Ip.Angle;
	if(angle<0)
		*RM_pnAIF = angle+36000;
	else
		*RM_pnAIF = angle;	
	DB_LogWriteRM(RM_AIF, *RM_pnAIF);	
//	CaluAngle(&m_sItm,AngleTab);
//	*RM_pnAITM=m_sItm.Angle-Ip.Angle;
//	DB_LogWriteRM(RM_AITM, *RM_pnAITM); 
//	CaluAngle(&m_sIfm,AngleTab);
//	*RM_pnAIFM=m_sIfm.Angle-Ip.Angle;
//	DB_LogWriteRM(RM_AIFM, *RM_pnAIFM);	
}

void CRelayTask::InitLoadMonitor()
{
	
	//NVRam数据的有效性检查，用于负荷监测数据
	if(*pDataValidity != 0x55AA33CC)
	{
		memset(pTodayImax, 0, sizeof(TLoadSupV)*6+sizeof(TSysTime)+6);
		*pDataValidity = 0x55AA33CC;
		ReportMsg("Warning!,NvRam was modified!");
		return;
	}
	TAbsTime tabstime,tabstime1,tabstime2;
	TSysTime tsystime,tsystime1;
	QWORD abstime1,abstime2,abstime3,abstime4;
	ReadAbsTime(&tabstime);
	ReadSystemTime(&tsystime);
	if(tsystime.Year<2000||tsystime.Year>2050) //时间不正常
	{
	//	ReportMsg("Error!System time is out limit! Load Monitor disabler!");
		return;
	}
	
	if(pRecurrenceTime->Year<2000)
	{
	//	ReportMsg("Error!Recurrenc time is out limit! Load Monitor disabler!");
		return;
	}
	if(pRecurrenceTime->Day == tsystime.Day) //今天已经进行过数据交换
		return;
	//第一种情况:上电时刻与上一次数据交换时刻之间超过了24小时，马上进行交换。	
	abstime1 = tabstime.Hi;
	abstime1 = (abstime1<<32)+tabstime.Lo;
	ToAbsTime(pRecurrenceTime,&tabstime2);
	abstime4 = tabstime2.Hi;
	abstime4 = (abstime4<<32)+tabstime2.Lo;
	if(abstime1 - abstime4 >=0x5265c00)
	{
		*pDaybyImax = *pYtdayImax;
		*pDaybyUmin = *pYtdayUmin;
		*pYtdayImax = *pTodayImax;
		*pYtdayUmin = *pTodayUmin;		
		pTodayImax->Current = 0;
		pTodayImax->Voltage = 0;
//		pTodayImax->AngleCurr = 0;
		pTodayImax->AngleVolt = 0;
		pTodayImax->Time	= tabstime;
		pTodayUmin->Current = 0;
		pTodayUmin->Voltage = 0;
//		pTodayUmin->AngleCurr = 0;
		pTodayUmin->AngleVolt = 0;
		pTodayUmin->Time	= tabstime;
		*pRecurrenceTime = tsystime;
		//在初始化阶段已经交换数据，正常运行时不需要再交换一次
		if(tsystime.Hour == *AS_pnLMT) 
			*g_pHaveSwitchData = TRUE;

		return;
	}
	//第二种情况:举例以8点整交换数据，上电时刻的时标与昨天8点0分0秒0毫秒时刻的时间差若大于24小时，进行交换
	//小于则不进行交换。防止出现连续停电的情况
	//得到昨天当前时刻的绝对时标,并将其小时值置为数据交换时间的小时数
	abstime3 = abstime1-0x5265c00;//0x5265c00为24小时的毫秒数	
	tabstime1.Lo= abstime3&0xffffffff;
	tabstime1.Hi= abstime3>>32;	
	AbsTimeTo(&tabstime1,&tsystime1);
	tsystime1.Hour = *AS_pnLMT;
	tsystime1.Minute = 0;
	tsystime1.Second = 0;
	tsystime1.MSecond = 0;
	ToAbsTime(&tsystime1, &tabstime1);
	abstime2 = tabstime1.Hi;
	abstime2 = (abstime2<<32)+tabstime1.Lo;	
	//当前上电时刻与昨天数据交换的理论时刻的时间差大于1天，立即进行数据交换。
	if((abstime1 - abstime2) >= 0x5265c00)
	{
		*pDaybyImax = *pYtdayImax;
		*pDaybyUmin = *pYtdayUmin;
		*pYtdayImax = *pTodayImax;
		*pYtdayUmin = *pTodayUmin;		
		pTodayImax->Current = 0;
		pTodayImax->Voltage = 0;
//		pTodayImax->AngleCurr = 0;
		pTodayImax->AngleVolt = 0;
		pTodayImax->Time	= tabstime;
		pTodayUmin->Current = 0;
		pTodayUmin->Voltage = 0;
//		pTodayUmin->AngleCurr = 0;
		pTodayUmin->AngleVolt = 0;
		pTodayUmin->Time	= tabstime;
		*pRecurrenceTime = tsystime;
		//在初始化阶段已经交换数据，正常运行时不需要再交换一次
		if(tsystime.Hour == *AS_pnLMT) 
			*g_pHaveSwitchData = TRUE;
	}	
}

void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&Rel50I0;
	StructCheck[0].bigsize = sizeof(Rel50I0)/2 - Rel50I0.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&Rel50I1;
	StructCheck[1].bigsize = sizeof(Rel50I1)/2 - Rel50I1.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel50I2;
	StructCheck[2].bigsize = sizeof(Rel50I2)/2 - Rel50I2.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel50I3;
	StructCheck[3].bigsize = sizeof(Rel50I3)/2 - Rel50I3.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel211;
	StructCheck[4].bigsize = sizeof(Rel211)/2 - Rel211.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel212;
	StructCheck[5].bigsize = sizeof(Rel212)/2 - Rel212.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&Rel213;
	StructCheck[6].bigsize = sizeof(Rel213)/2 - Rel213.wExcepWordNum;
	StructCheck[7].pstruct = (WORD*)&Rel214;
	StructCheck[7].bigsize = sizeof(Rel214)/2 - Rel214.wExcepWordNum;
	StructCheck[8].pstruct = (WORD*)&Rel27U;
	StructCheck[8].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;
	StructCheck[9].pstruct = (WORD*)&Rel51I;
	StructCheck[9].bigsize = sizeof(Rel51I)/2 - Rel51I.wExcepWordNum;
	StructCheck[10].pstruct = (WORD*)&Rel21FL_T;
	StructCheck[10].bigsize = sizeof(Rel21FL_T)/2 - Rel21FL_T.wExcepWordNum;
	StructCheck[11].pstruct = (WORD*)&Rel21FL_F;
	StructCheck[11].bigsize = sizeof(Rel21FL_F)/2 - Rel21FL_F.wExcepWordNum;
	StructCheck[12].pstruct = (WORD*)&RelPT1;
	StructCheck[12].bigsize = sizeof(RelPT1)/2 - RelPT1.wExcepWordNum;
	StructCheck[13].pstruct = (WORD*)&RelPT2;
	StructCheck[13].bigsize = sizeof(RelPT2)/2 - RelPT2.wExcepWordNum;
	StructCheck[14].pstruct = (WORD*)&Rel79;
	StructCheck[14].bigsize = sizeof(Rel79)/2 - Rel79.wExcepWordNum;
	StructCheck[15].pstruct = (WORD*)&Rel50DI;
	StructCheck[15].bigsize = sizeof(Rel50DI)/2 - Rel50DI.wExcepWordNum;
	StructCheck[16].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[16].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
// 名称: 初始化速断保护元件	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I0()
{
	Rel50I0.pElecSet			= RS_pn50_0C;          						//过量保护电量定值
	Rel50I0.pTimeSet			= RS_pn50_0D;				 			//过量保护时间定值
	Rel50I0.pHarmKhr1Set		= RS_pnFACTOR1;							//过量保护谐波加权系数
	Rel50I0.pVolLockSet			= RS_pn50_0_27V;				 	    		//过量保护电压闭锁定值
	Rel50I0.pAccActTimeSet		= NULL;									//过量保护后加速动作时间定值
	Rel50I0.pH2Coef				= CF_pnC2;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I0.pRelCFG				= CFG_pby50_0;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I0.pRelSW				= SW_pby50_0;         					//过量保护软压板------1:投入，2:退出
	Rel50I0.pRelHLockCFG		= CFG_pby50_0_HLK;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I0.pRelHSuprCFG		= CFG_pby50_0_HRES; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I0.pRelPDCFG			= CFG_pby50_0_67;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I0.pRelVolLockCFG		= CFG_pby50_0_27;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I0.pRelFDCFG			= CFG_pby50_0_D;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I0.pRelAccCFG			= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I0.pRelFLCFG			= CFG_pby21_FL;					//故障测距投入
	Rel50I0.pRel47UCFG			= &RelCFGCLR;					
	Rel50I0.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I0.pRelElec 			= &Ip;									//当前电量指针		
	Rel50I0.pRelHarm3			= &I3;									//当前三次谐波指针	
	Rel50I0.pRelHarm5			= &I5;									//当前五次谐波指针	
	Rel50I0.pRelVolt			= &ULock;									//当前用来闭锁的电压的指针

	Rel50I0.StartRelayWord		= RW_PI50_0;						//过量保护入段继电器字
	Rel50I0.ActRelayWord		= RW_TR50_0;							//过量保护动作继电器字
	Rel50I0.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I0.AccActRelayWord		= FALSE1;									//后加速动作继电器字
	Rel50I0.PForwardRelayWord	= RW_32F;							//功率方向继电器字
	Rel50I0.PRewardRelayWord    = RW_32R;
	Rel50I0.H2LockRelayWord		= RW_LKH2;							//二次谐波闭锁继电器字
	
	Rel50I0.R47ULKRelayWord		= FALSE1;
	Rel50I0.LockRelayWord		= RW_LKPI50_0;						//入段闭锁继电器字
	Rel50I0.RSTRelayWord		= RW_LKRST50_0;							//返回闭锁继电器字
	Rel50I0.LoopCtrlRelayWord	= RW_LPC50_0;						//保护循环控制继电器字
	
	
	Rel50I0.wReportNum			= wRep50I0;							//过量保护报告序号
	Rel50I0.wAccReportNum		= 0xFFFF;							//加速动作报告序号

	Rel50I0.dwK35				= &K35;
	Rel50I0.RptSerialNum		= &RptSerialNumOfAct;				//动作报告号
	Rel50I0.bMakeActRpt			= TRUE;
	//**********************入段报告部分*******************   yanxs  13-02-05
	Rel50I0.wPickUpReportNum	= wRep50I0Pick; 					//入段报告inf序号
	Rel50I0.wResetReportNum 	= wRep50I0RES;						//返回报告inf序号
	Rel50I0.wAccPickUpRptNum	= 0xFFFF; 							//加速入段报告inf序号  yanxs  2013-06-20
	Rel50I0.wAccResetRptNum 	= 0xFFFF;							//加速返回报告inf序号
	//**********************************************************************
	//Rel50I0.dwRelTimer			= 0;			 			//定时器
	Rel50I0.byRelFCounter		= 0;						//故障计数器
	Rel50I0.byRelRCounter		= 0;						//返回计数器	
	Rel50I0.wExcepWordNum		= 8;
	Rel50I0.wInfo 				= 0;

}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流I段保护元件	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I1()
{
	Rel50I1.pElecSet			= RS_pn50_1C;          						//过量保护电量定值 
	Rel50I1.pTimeSet			= RS_pn50_1D;				 				//过量保护时间定值 
	Rel50I1.pHarmKhr1Set		= RS_pnFACTOR1;							//过量保护谐波加权系数 
	Rel50I1.pVolLockSet			= RS_pn50_1_27V;				 	    	//过量保护电压闭锁定值 
	Rel50I1.pAccActTimeSet		= RS_pn94D;						//过量保护后加速动作时间定值 
	Rel50I1.pH2Coef				= CF_pnC2;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	Rel50I1.pRelCFG				= CFG_pby50_1;							//过量保护功能控制字--1:投入，2:退出  
	Rel50I1.pRelSW				= SW_pby50_123;         					//过量保护软压板------1:投入，2:退出  
	Rel50I1.pRelHLockCFG		= CFG_pby50_HLK;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	Rel50I1.pRelHSuprCFG		= CFG_pby50_HRES; 						//谐波抑制功能控制位--1:投入，2:退出 
	Rel50I1.pRelPDCFG			= CFG_pby50_1_67;		    				//功率方向功能控制位--1:投入，2:退出 
	Rel50I1.pRelFDCFG			= CFG_pby50_1_D;					//
	Rel50I1.pRelVolLockCFG		= CFG_pby50_1_27;						//低压闭锁功能控制位--1:投入，2:退出  
	Rel50I1.pRelAccCFG			= CFG_pby50_1_94;							//有无后加速功能------1:有，  2:无 
	Rel50I1.pRel47UCFG			= &RelCFGCLR;					//故障测距投入

	Rel50I1.pRelFLCFG			= CFG_pby21_FL;				//故障测距投入 
	Rel50I1.pSysTimer			= &dwRelayMSCnt;								//系统定时器指针 	
	
	Rel50I1.pRelElec 			= &Ip;						//当前电量指针 		
	Rel50I1.pRelHarm3			= &I3;						//当前三次谐波指针 	
	Rel50I1.pRelHarm5			= &I5;						//当前五次谐波指针 	
	Rel50I1.pRelVolt			= &ULock;						//当前用来闭锁的电压的指针 

	Rel50I1.StartRelayWord		= RW_PI50_1;							//过量保护入段继电器字 
	Rel50I1.ActRelayWord		= RW_TR50_1;							//过量保护动作继电器字 
	Rel50I1.AccRelayWord		= RW_TSOTF;							//后加速允许继电器字,是一个时间继电器字，延时到后自动复归 
	Rel50I1.AccActRelayWord		= RW_TR50_1_94;						//后加速动作继电器字 
	Rel50I1.PForwardRelayWord	= RW_32F;						//功率正方向继电器字 
	Rel50I1.PRewardRelayWord  	= RW_32R;
	Rel50I1.H2LockRelayWord		= RW_LKH2;						//二次谐波闭锁继电器字 
	Rel50I1.R47ULKRelayWord		= FALSE1;
	Rel50I1.LockRelayWord		= RW_LKPI50_1;							//入段闭锁继电器字 
	Rel50I1.RSTRelayWord		= RW_LKRST50_1;							//返回闭锁继电器字 
	Rel50I1.LoopCtrlRelayWord	= RW_LPC50_1;						//保护循环控制继电器字 
	
	Rel50I1.wReportNum			= wRep50I1;								//过量保护报告序号 
	Rel50I1.wAccReportNum		= wRep50I1Acc;							//加速动作报告序号
	
	
	Rel50I1.dwK35				= &K35;
	Rel50I1.RptSerialNum		= &RptSerialNumOfAct;
	Rel50I1.bMakeActRpt			= TRUE;


	//**********************入段报告部分*******************   yanxs  13-02-05
	Rel50I1.wPickUpReportNum	= wRep50I1Pick; 					//入段报告序号
	Rel50I1.wResetReportNum 	= wRep50I1RES;						//返回报告序号	
	Rel50I1.wAccPickUpRptNum	= wRep50I1AccPick; 					//加速入段报告inf序号  yanxs  2013-06-20
	Rel50I1.wAccResetRptNum 	= wRep50I1AccRES;					//加速返回报告inf序号
	//**********************************************************************
//	Rel50I1.dwRelTimer			= 0;			 				//定时器 
	Rel50I1.byRelFCounter		= 0;						//故障计数器 
	Rel50I1.byRelRCounter		= 0;						//返回计数器 	
	Rel50I1.wExcepWordNum		= 8;
	Rel50I1.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流II段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I2()
{	
	Rel50I2.pElecSet			= RS_pn50_2C;          						//过量保护电量定值 
	Rel50I2.pTimeSet			= RS_pn50_2D;				 				//过量保护时间定值 
	Rel50I2.pHarmKhr1Set		= RS_pnFACTOR1;							//过量保护谐波加权系数 
	Rel50I2.pVolLockSet			= RS_pn50_2_27V;				 	    	//过量保护电压闭锁定值 
	Rel50I2.pAccActTimeSet		= RS_pn94D;						//过量保护后加速动作时间定值 
	Rel50I2.pH2Coef				= CF_pnC2;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	Rel50I2.pRelCFG				= CFG_pby50_2;							//过量保护功能控制字--1:投入，2:退出  
	Rel50I2.pRelSW				= SW_pby50_123;         					//过量保护软压板------1:投入，2:退出  
	Rel50I2.pRelHLockCFG		= CFG_pby50_HLK;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	Rel50I2.pRelHSuprCFG		= CFG_pby50_HRES; 						//谐波抑制功能控制位--1:投入，2:退出 
	Rel50I2.pRelPDCFG			= CFG_pby50_2_67;		    				//功率方向功能控制位--1:投入，2:退出 
	Rel50I2.pRelFDCFG			= CFG_pby50_2_D;
	Rel50I2.pRelVolLockCFG		= CFG_pby50_2_27;						//低压闭锁功能控制位--1:投入，2:退出  
	Rel50I2.pRelAccCFG			= CFG_pby50_2_94;							//有无后加速功能------1:有，  2:无 
	Rel50I2.pRelFLCFG			= CFG_pby21_FL;				//故障测距投入 
	Rel50I2.pRel47UCFG			= &RelCFGCLR;
	Rel50I2.pSysTimer			= &dwRelayMSCnt;								//系统定时器指针 	
	
	Rel50I2.pRelElec 			= &Ip;						//当前电量指针 		
	Rel50I2.pRelHarm3			= &I3;						//当前三次谐波指针 	
	Rel50I2.pRelHarm5			= &I5;						//当前五次谐波指针 	
	Rel50I2.pRelVolt			= &ULock;						//当前用来闭锁的电压的指针 

	Rel50I2.StartRelayWord		= RW_PI50_2;							//过量保护入段继电器字 
	Rel50I2.ActRelayWord		= RW_TR50_2;							//过量保护动作继电器字 
	Rel50I2.AccRelayWord		= RW_TSOTF;							//后加速允许继电器字 
	Rel50I2.AccActRelayWord		= RW_TR50_2_94;						//后加速动作继电器字 
	Rel50I2.PForwardRelayWord	= RW_32F;						//功率正方向继电器字 
	Rel50I2.PRewardRelayWord  	= RW_32R;					//功率反方向继电器字 
	Rel50I2.H2LockRelayWord		= RW_LKH2;						//二次谐波闭锁继电器字 
	
	Rel50I2.R47ULKRelayWord		= FALSE1;
	Rel50I2.LockRelayWord		= RW_LKPI50_2;							//入段闭锁继电器字 
	Rel50I2.RSTRelayWord		= RW_LKRST50_2;							//返回闭锁继电器字 
	Rel50I2.LoopCtrlRelayWord	= RW_LPC50_2;						//保护循环控制继电器字 
	
	Rel50I2.wReportNum			= wRep50I2;								//过量保护报告序号 
	Rel50I2.wAccReportNum		= wRep50I2Acc;							//加速动作报告序号 
	
	Rel50I2.dwK35				= &K35;
	Rel50I2.RptSerialNum		= &RptSerialNumOfAct;
	Rel50I2.bMakeActRpt			= TRUE;

	//**********************入段报告部分*******************   yanxs  13-02-05
	Rel50I2.wPickUpReportNum	= wRep50I2Pick; 					//入段报告序号
	Rel50I2.wResetReportNum 	= wRep50I2RES;						//返回报告序号	
	Rel50I2.wAccPickUpRptNum	= wRep50I2AccPick; 					//加速入段报告inf序号  yanxs  2013-06-20
	Rel50I2.wAccResetRptNum 	= wRep50I2AccRES;					//加速返回报告inf序号
	//**********************************************************************
	//Rel50I2.dwRelTimer			= 0;			 				//定时器 
	Rel50I2.byRelFCounter		= 0;						//故障计数器 
	Rel50I2.byRelRCounter		= 0;						//返回计数器 
	Rel50I2.wExcepWordNum		= 8;
	Rel50I2.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过流III段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel50I3()
{	
	Rel50I3.pElecSet			= RS_pn50_3C;          						//过量保护电量定值 
	Rel50I3.pTimeSet			= RS_pn50_3D;				 				//过量保护时间定值 
	Rel50I3.pHarmKhr1Set		= RS_pnFACTOR1;							//过量保护谐波加权系数 
	Rel50I3.pVolLockSet			= NULL;				 	    	//过量保护电压闭锁定值 
	Rel50I3.pAccActTimeSet		= RS_pn94D;						//过量保护后加速动作时间定值 
	Rel50I3.pH2Coef				= CF_pnC2;							//二次谐波闭锁系数，谐波加权系数的放大倍数

	Rel50I3.pRelCFG				= CFG_pby50_3;							//过量保护功能控制字--1:投入，2:退出  
	Rel50I3.pRelSW				= SW_pby50_123;         					//过量保护软压板------1:投入，2:退出  
	Rel50I3.pRelHLockCFG		= CFG_pby50_HLK;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	Rel50I3.pRelHSuprCFG		= CFG_pby50_HRES; 						//谐波抑制功能控制位--1:投入，2:退出 
	Rel50I3.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出 
	Rel50I3.pRelFDCFG			= &RelCFGCLR;						//过流III段投入正方向控制位 
	Rel50I3.pRelVolLockCFG		= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出  
	Rel50I3.pRelAccCFG			= CFG_pby50_3_94;							//有无后加速功能------1:有，  2:无 
	Rel50I3.pRelFLCFG			= CFG_pby21_FL;				//故障测距投入 
	Rel50I3.pRel47UCFG			= &RelCFGCLR;
	Rel50I3.pSysTimer			= &dwRelayMSCnt;								//系统定时器指针 	
	
	Rel50I3.pRelElec 			= &Ip;						//当前电量指针 		
	Rel50I3.pRelHarm3			= &I3;						//当前三次谐波指针 	
	Rel50I3.pRelHarm5			= &I5;						//当前五次谐波指针 	
	Rel50I3.pRelVolt			= &ULock;						//当前用来闭锁的电压的指针 

	Rel50I3.StartRelayWord		= RW_PI50_3;							//过量保护入段继电器字 
	Rel50I3.ActRelayWord		= RW_TR50_3;							//过量保护动作继电器字 
	Rel50I3.AccRelayWord		= RW_TSOTF;							//后加速允许继电器字 
	Rel50I3.AccActRelayWord		= RW_TR50_3_94;						//后加速动作继电器字 
	Rel50I3.PForwardRelayWord	= RW_32F;						//功率正方向继电器字 
	Rel50I3.PRewardRelayWord  	= RW_32R;						//功率反方向继电器字 
	Rel50I3.H2LockRelayWord		= RW_LKH2;						//二次谐波闭锁继电器字 
	Rel50I3.R47ULKRelayWord		= FALSE1;
	Rel50I3.LockRelayWord		= RW_LKPI50_3;							//入段闭锁继电器字 
	Rel50I3.RSTRelayWord		= RW_LKRST50_3;							//返回闭锁继电器字 
	Rel50I3.LoopCtrlRelayWord	= RW_LPC50_3;						//保护循环控制继电器字 
	
	Rel50I3.wReportNum			= wRep50I3;								//过量保护报告序号 
	Rel50I3.wAccReportNum		= wRep50I3Acc;							//加速动作报告序号 
	Rel50I3.dwK35				= &K35;
	Rel50I3.RptSerialNum		= &RptSerialNumOfAct;
	Rel50I3.bMakeActRpt			= TRUE;

	//**********************入段报告部分*******************   yanxs  13-02-05
	Rel50I3.wPickUpReportNum	= wRep50I3Pick; 					//入段报告序号
	Rel50I3.wResetReportNum 	= wRep50I3RES;						//返回报告序号	
	Rel50I3.wAccPickUpRptNum	= wRep50I3AccPick; 					//加速入段报告inf序号  yanxs  2013-06-20
	Rel50I3.wAccResetRptNum 	= wRep50I3AccRES;					//加速返回报告inf序号
	//**********************************************************************
//	Rel50I3.dwRelTimer			= 0;			 				//定时器 
	Rel50I3.byRelFCounter		= 0;						//故障计数器 
	Rel50I3.byRelRCounter		= 0;						//返回计数器 
	Rel50I3.wExcepWordNum		= 8;
	Rel50I3.wInfo 				= 0;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化距离I段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel211()
{
	Rel211.pRel21RSet					= RS_pn21_1R;					//距离保护电阻定值 
	Rel211.pRel21XSet					= RS_pn21_1X;					//距离保护电抗定值 
	Rel211.pRel21TSet    				= RS_pn21_1D;					//距离保护时间定值 
	Rel211.pRel21Khr1Set				= RS_pnFACTOR1;					//距离保护谐波抑制系数 
	Rel211.pRel21LineAngleSet			= RS_pnZANGLE;					//线路阻抗角 
	Rel211.pRel21AccTSet				= NULL;							//距离后加速动作时间 
	Rel211.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT断线闭锁电流定值 
	Rel211.pH2Coef						= CF_pnC2;						//二次谐波闭锁系数，谐波加权系数的放大倍数
	Rel211.pRel21ZOffset				= RS_pn21ZOFFSET;					//平行四边形阻抗偏移值 
	Rel211.pRel21AOffset				= RS_pn21AOFFSET;				//阻抗四边形角度偏移
	
	Rel211.pRel21CFG					= CFG_pby21_1;						//距离保护配置 
	Rel211.pRel21SW						= SW_pby21;						//距离保护软压板 
	Rel211.pRel21DirCFG					= CFG_pby21_1_D;					//距离保护方向 
	Rel211.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//平行四边形 
	Rel211.pRel21HLockCFG				= CFG_pby21_HLK;					//距离保护谐波闭锁 
	Rel211.pRel21HSuprCFG				= CFG_pby21_HRES;					//距离保护谐波抑制 
	Rel211.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT断线检查 
	Rel211.pRel21ACCCFG					= &RelCFGCLR;							//后加速功能控制 
	Rel211.pRelFLCFG					= CFG_pby21_FL;				//故障测距投入 
	Rel211.pSysTimer					= &dwRelayMSCnt;					//系统定时器指针 
	Rel211.pTangTabAddr					= TangAngleTab;						//正切表首地址 

	Rel211.pRel21Cur					= &Ip;							//电流指针 
	Rel211.pRel21Vol					= &Up;							//电压指针 
	Rel211.pRel21Impd					= &Impd;						//当前的阻抗 
	Rel211.pRel21H3						= &I3;							//三次谐波指针 
	Rel211.pRel21H5						= &I5;							//五次谐波指针 

	Rel211.pnZCoef						= CF_pnZ;						//阻抗系数

	Rel211.StartRelayWord				= RW_PI21_1;						//距离保护入段继电器字 
	Rel211.ACTRelayWord					= RW_TR21_1;						//距离保护动作继电器字 
	Rel211.ACCRelayWord					= FALSE1;						//后加速允许继电器字 
	Rel211.ACCActRelayWord				= FALSE1;					//后加速动作继电器字 
	Rel211.PTBreakRelayWord				= RW_ARVT1;					//PT断线继电器字 
	Rel211.H2LockRelayWord				= RW_LKH2;					//二次谐波闭锁继电器字 
	Rel211.LockRelayWord				= RW_LKPI21_1;						//入段闭锁继电器字 
	Rel211.LoopCtrlRelayWord			= RW_LPC21_1;					//保护循环控制继电器字 
	Rel211.R37IRelayWord				= RW_37IP;						//无流继电器字

	Rel211.wRel21RptNum					= wRep211;				     	//距离保护报告序号 
	Rel211.wRel21AccRptNum				= 0xFFFF;					//距离保护后加速报告序号 
	Rel211.wRel21PTLockRptNum			= R_PTLK;					//PT断线闭锁距离保护报告序号 
	
	Rel211.dwK35						= &K35;
	Rel211.RptSerialNum					= &RptSerialNumOfAct;	
	//**********************入段报告部分*******************   yanxs  13-03-07
	Rel211.wPickUpReportNum				= wRep211Pick; 					//入段报告序号
	Rel211.wResetReportNum 				= wRep211RES;					//返回报告序号	
	Rel211.wAccPickUpRptNum				= 0xFFFF; 						//加速入段报告序号  2013-06-20  yanxs
	Rel211.wAccResetRptNum 				= 0xFFFF;						//加速返回报告序号	
	//**********************************************************************
	
//	Rel211.dwRel21Timer					= 0;						//定时器 	
	Rel211.byRel21FCounter				= 0;						//距离保护故障计数器 
	Rel211.byRel21RCounter				= 0;						//距离保护返回计数器 
	Rel211.wExcepWordNum				= 8;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 

}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化距离II段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel212()
{	
	Rel212.pRel21RSet					= RS_pn21_2R;					//距离保护电阻定值 
	Rel212.pRel21XSet					= RS_pn21_2X;					//距离保护电抗定值 
	Rel212.pRel21TSet    				= RS_pn21_2D;					//距离保护时间定值 
	Rel212.pRel21Khr1Set				= RS_pnFACTOR1;					//距离保护谐波抑制系数 
	Rel212.pRel21LineAngleSet			= RS_pnZANGLE;					//线路阻抗角 
	Rel212.pRel21AccTSet				= RS_pn94D;					//距离后加速动作时间 
	Rel212.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT断线闭锁电流定值 
	Rel212.pH2Coef						= CF_pnC2;						//二次谐波闭锁系数，谐波加权系数的放大倍数
	Rel212.pRel21ZOffset				= RS_pn21ZOFFSET;					//平行四边形阻抗偏移值 
	Rel212.pRel21AOffset				= RS_pn21AOFFSET;				//阻抗四边形角度偏移

	Rel212.pRel21CFG					= CFG_pby21_2;						//距离保护配置 
	Rel212.pRel21SW						= SW_pby21;						//距离保护软压板 
	Rel212.pRel21DirCFG					= CFG_pby21_2_D;					//距离保护方向 
	Rel212.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//平行四边形 
	Rel212.pRel21HLockCFG				= CFG_pby21_HLK;					//距离保护谐波闭锁 
	Rel212.pRel21HSuprCFG				= CFG_pby21_HRES;					//距离保护谐波抑制 
	Rel212.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT断线检查 
	Rel212.pRel21ACCCFG					= CFG_pby21_2_94;							//后加速功能控制 
	Rel212.pRelFLCFG					= CFG_pby21_FL;				//故障测距投入 
	Rel212.pSysTimer					= &dwRelayMSCnt;					//系统定时器指针 
	Rel212.pTangTabAddr					= TangAngleTab;						//正切表首地址 

	Rel212.pRel21Cur					= &Ip;							//电流指针 
	Rel212.pRel21Vol					= &Up;							//电压指针 
	Rel212.pRel21Impd					= &Impd;						//当前的阻抗 
	Rel212.pRel21H3						= &I3;							//三次谐波指针 
	Rel212.pRel21H5						= &I5;							//五次谐波指针 

	Rel212.pnZCoef						= CF_pnZ;						//阻抗系数

	Rel212.StartRelayWord				= RW_PI21_2;						//距离保护入段继电器字 
	Rel212.ACTRelayWord					= RW_TR21_2;						//距离保护动作继电器字 
	Rel212.ACCRelayWord					= RW_TSOTF;						//后加速允许继电器字 
	Rel212.ACCActRelayWord				= RW_TR21_2_94;					//后加速动作继电器字 
	Rel212.PTBreakRelayWord				= RW_ARVT1;					//PT断线继电器字 
	Rel212.H2LockRelayWord				= RW_LKH2;					//二次谐波闭锁继电器字 
	Rel212.LockRelayWord				= RW_LKPI21_2;						//入段闭锁继电器字 
	Rel212.LoopCtrlRelayWord			= RW_LPC21_2;					//保护循环控制继电器字 
	Rel212.R37IRelayWord				= RW_37IP;
	
	Rel212.wRel21RptNum					= wRep212;				     	//距离保护报告序号 
	Rel212.wRel21AccRptNum				= wRepR212Acc;					//距离保护后加速报告序号 
	Rel212.wRel21PTLockRptNum			= R_PTLK;					//PT断线闭锁距离保护报告序号 
	
	Rel212.dwK35						= &K35;
	Rel212.RptSerialNum					= &RptSerialNumOfAct;
	//**********************入段报告部分*******************   yanxs  13-03-07
	Rel212.wPickUpReportNum				= wRep212Pick; 					//入段报告序号
	Rel212.wResetReportNum 				= wRep212RES;					//返回报告序号	
	Rel212.wAccPickUpRptNum				= wRepR212AccPick; 				//加速入段报告序号  2013-06-20  yanxs
	Rel212.wAccResetRptNum 				= wRepR212AccRES;				//加速返回报告序号	
	//**********************************************************************

//	Rel212.dwRel21Timer					= 0;						//定时器 	
	Rel212.byRel21FCounter				= 0;						//距离保护故障计数器 
	Rel212.byRel21RCounter				= 0;					//距离保护返回计数器 
	Rel212.wExcepWordNum				= 8;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化距离III段保护元件                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 

void CRelayTask::InitRel213()
{
	Rel213.pRel21RSet					= RS_pn21_3R;					//距离保护电阻定值 
	Rel213.pRel21XSet					= RS_pn21_3X;					//距离保护电抗定值 
	Rel213.pRel21TSet    				= RS_pn21_3D;					//距离保护时间定值 
	Rel213.pRel21Khr1Set				= RS_pnFACTOR1;					//距离保护谐波抑制系数 
	Rel213.pRel21LineAngleSet			= RS_pnZANGLE;					//线路阻抗角 
	Rel213.pRel21AccTSet				= RS_pn94D;					//距离后加速动作时间 
	Rel213.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT断线闭锁电流定值 
	Rel213.pH2Coef						= CF_pnC2;						//二次谐波闭锁系数，谐波加权系数的放大倍数
	Rel213.pRel21ZOffset				= RS_pn21ZOFFSET;					//平行四边形阻抗偏移值 
	Rel213.pRel21AOffset				= RS_pn21AOFFSET;				//阻抗四边形角度偏移

	Rel213.pRel21CFG					= CFG_pby21_3;						//距离保护配置 
	Rel213.pRel21SW						= SW_pby21;						//距离保护软压板 
	Rel213.pRel21DirCFG					= CFG_pby21_3_D;					//距离保护方向 
	Rel213.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//平行四边形 
	Rel213.pRel21HLockCFG				= CFG_pby21_HLK;					//距离保护谐波闭锁 
	Rel213.pRel21HSuprCFG				= CFG_pby21_HRES;					//距离保护谐波抑制 
	Rel213.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT断线检查 
	Rel213.pRel21ACCCFG					= CFG_pby21_3_94;					//后加速功能控制 
	Rel213.pRelFLCFG					= CFG_pby21_FL;				//故障测距投入 
	Rel213.pSysTimer					= &dwRelayMSCnt;					//系统定时器指针 
	Rel213.pTangTabAddr					= TangAngleTab;						//正切表首地址 

	Rel213.pRel21Cur					= &Ip;							//电流指针 
	Rel213.pRel21Vol					= &Up;							//电压指针 
	Rel213.pRel21Impd					= &Impd;						//当前的阻抗 
	Rel213.pRel21H3						= &I3;							//三次谐波指针 
	Rel213.pRel21H5						= &I5;							//五次谐波指针 

	Rel213.pnZCoef						= CF_pnZ;						//阻抗系数

	Rel213.StartRelayWord				= RW_PI21_3;						//距离保护入段继电器字 
	Rel213.ACTRelayWord					= RW_TR21_3;						//距离保护动作继电器字 
	Rel213.ACCRelayWord					= RW_TSOTF;						//后加速允许继电器字 
	Rel213.ACCActRelayWord				= RW_TR21_3_94;					//后加速动作继电器字 
	Rel213.PTBreakRelayWord				= RW_ARVT1;					//PT断线继电器字 
	Rel213.H2LockRelayWord				= RW_LKH2;					//二次谐波闭锁继电器字 
	Rel213.LockRelayWord				= RW_LKPI21_3;						//入段闭锁继电器字 
	Rel213.LoopCtrlRelayWord			= RW_LPC21_3;					//保护循环控制继电器字 
	Rel213.R37IRelayWord				= RW_37IP;
	
	Rel213.wRel21RptNum					= wRep213;				     	//距离保护报告序号 
	Rel213.wRel21AccRptNum				= wRepR213Acc;					//距离保护后加速报告序号 
	Rel213.wRel21PTLockRptNum			= R_PTLK;					//PT断线闭锁距离保护报告序号 
	Rel213.dwK35						= &K35;
	Rel213.RptSerialNum					= &RptSerialNumOfAct;
	//**********************入段报告部分*******************   yanxs  13-03-07
	Rel213.wPickUpReportNum				= wRep213Pick; 					//入段报告序号
	Rel213.wResetReportNum 				= wRep213RES;					//返回报告序号	
	Rel213.wAccPickUpRptNum				= wRepR213AccPick; 				//加速入段报告序号  2013-06-20  yanxs
	Rel213.wAccResetRptNum 				= wRepR213AccRES;				//加速返回报告序号	
	//**********************************************************************

//	Rel213.dwRel21Timer					= 0;						//定时器 	
	Rel213.byRel21FCounter				= 0;						//距离保护故障计数器 
	Rel213.byRel21RCounter				= 0;					//距离保护返回计数器 
	Rel213.wExcepWordNum				= 8;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
/************************************************************************************************
*功能: 初始化距离IV段保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
void CRelayTask::InitRel214()
{
	Rel214.pRel21RSet					= RS_pn21_4R;					//距离保护电阻定值 
	Rel214.pRel21XSet					= RS_pn21_4X;					//距离保护电抗定值 
	Rel214.pRel21TSet    				= RS_pn21_4D;					//距离保护时间定值 
	Rel214.pRel21Khr1Set				= RS_pnFACTOR1;					//距离保护谐波抑制系数 
	Rel214.pRel21LineAngleSet			= RS_pnZANGLE;					//线路阻抗角 
	Rel214.pRel21AccTSet				= RS_pn94D;					//距离后加速动作时间 
	Rel214.pRel21PTBreakCurSet			= RS_pn74VTBLC;				//PT断线闭锁电流定值 
	Rel214.pH2Coef						= CF_pnC2;						//二次谐波闭锁系数，谐波加权系数的放大倍数
	Rel214.pRel21ZOffset				= RS_pn21ZOFFSET;					//平行四边形阻抗偏移值 
	Rel214.pRel21AOffset				= RS_pn21AOFFSET;				//阻抗四边形角度偏移

	Rel214.pRel21CFG					= CFG_pby21_4;						//距离保护配置 
	Rel214.pRel21SW						= SW_pby21;						//距离保护软压板 
	Rel214.pRel21DirCFG					= CFG_pby21_4_D;					//距离保护方向 
	Rel214.pRel21QuadTypeCFG			= CFG_pbyParallelC;					//平行四边形 
	Rel214.pRel21HLockCFG				= CFG_pby21_HLK;					//距离保护谐波闭锁 
	Rel214.pRel21HSuprCFG				= CFG_pby21_HRES;					//距离保护谐波抑制 
	Rel214.pRel21PTBreakCFG				= CFG_pbyPT1BR;				//PT断线检查 
	Rel214.pRel21ACCCFG					= CFG_pby21_4_94;					//后加速功能控制 
	Rel214.pRelFLCFG					= CFG_pby21_FL;				//故障测距投入 
	Rel214.pSysTimer					= &dwRelayMSCnt;					//系统定时器指针 
	Rel214.pTangTabAddr					= TangAngleTab;						//正切表首地址 

	Rel214.pRel21Cur					= &Ip;							//电流指针 
	Rel214.pRel21Vol					= &Up;							//电压指针 
	Rel214.pRel21Impd					= &Impd;						//当前的阻抗 
	Rel214.pRel21H3						= &I3;							//三次谐波指针 
	Rel214.pRel21H5						= &I5;							//五次谐波指针 

	Rel214.pnZCoef						= CF_pnZ;						//阻抗系数

	Rel214.StartRelayWord				= RW_PI21_4;						//距离保护入段继电器字 
	Rel214.ACTRelayWord					= RW_TR21_4;						//距离保护动作继电器字 
	Rel214.ACCRelayWord					= RW_TSOTF;						//后加速允许继电器字 
	Rel214.ACCActRelayWord				= RW_TR21_4_94;					//后加速动作继电器字 
	Rel214.PTBreakRelayWord				= RW_ARVT1;					//PT断线继电器字 
	Rel214.H2LockRelayWord				= RW_LKH2;						//二次谐波闭锁继电器字 
	Rel214.LockRelayWord				= RW_LKPI21_4;					//入段闭锁继电器字 
	Rel214.LoopCtrlRelayWord			= RW_LPC21_4;					//保护循环控制继电器字 
	Rel214.R37IRelayWord				= RW_37IP;
	
	Rel214.wRel21RptNum					= wRep214;				     			//距离保护报告序号 
	Rel214.wRel21AccRptNum				= wRepR214Acc;						//距离保护后加速报告序号 
	Rel214.wRel21PTLockRptNum			= R_PTLK;						//PT断线闭锁距离保护报告序号 
	
	Rel214.dwK35						= &K35;
	Rel214.RptSerialNum					= &RptSerialNumOfAct;
	//**********************入段报告部分*******************   yanxs  13-03-07
	Rel214.wPickUpReportNum				= wRep214Pick; 					//入段报告序号
	Rel214.wResetReportNum 				= wRep214RES;					//返回报告序号	
	Rel214.wAccPickUpRptNum				= wRepR214AccPick; 				//加速入段报告序号  2013-06-20  yanxs
	Rel214.wAccResetRptNum 				= wRepR214AccRES;				//加速返回报告序号	
	//**********************************************************************

//	Rel214.dwRel21Timer					= 0;						//定时器 	
	Rel214.byRel21FCounter				= 0;						//距离保护故障计数器 
	Rel214.byRel21RCounter				= 0;					//距离保护返回计数器 
	Rel214.wExcepWordNum				= 8;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
/************************************************************************************************
*功能: 初始化低压保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRel27U()
{
	Rel27U.pELeSet						= RS_pn27V;						//电量定值 
	Rel27U.pTimeSet						= RS_pn27D;						//时间定值 
	Rel27U.pLockCurSet					= RS_pn27_37C;					//闭锁电流定值 

	Rel27U.pRelCFG						= CFG_pby27;					  	 //欠量保护配置 
	Rel27U.pRelSW						= SW_pby27;					  	 //欠量保护软压板 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//欠量保护投跳闸 
	Rel27U.pRelCurLockCFG				= &RelCFGSET;
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//系统定时器指针 

	Rel27U.pRelElec						= &Up;						//当前的电量指针 
	Rel27U.pRelLockElec					= &Ip;					//闭锁电流 
	Rel27U.ActRelayWord					= RW_TR27;					//欠量保护动作继电器字 
	Rel27U.StartRelayWord				= RW_PI27;					//欠量保护入段继电器字 
	Rel27U.AlarmRelayWord				= RW_AR27;					//欠量保护告警继电器字 
	Rel27U.LockRelayWord				= RW_LKPI27;				//入段闭锁继电器字 
	Rel27U.RSTRelayWord					= RW_LKRST27;					//故障返回闭锁继电器字 
	Rel27U.LoopCtrlRelayWord			= RW_LPC27;				//故障循环控制继电器字 
	Rel27U.wR52BFRealyword				= RW_52BF;					//断路器跳闸失败继电器字
	Rel27U.wActRptNum					= wRep27U;					//欠量保护动作报告序号 
	Rel27U.wAlarmRptNum					= R_R27ALARM;					//欠量保护告警报告序号 
	Rel27U.wTRFailRptNum				= R_TRFAIL;
	Rel27U.wPickUpRptNum				= R_ACTPICK;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
//	Rel27U.wResetReportNum				= R_R27U_RES;
//	Rel27U.wPickUpReportNum				= wRep27UPick;
	Rel27U.R27RptSerialNum				= 0;
//	Rel27U.dwRelTimer					= 0;						//欠量保护定时器 	
	Rel27U.byRelFCounter				= 0;					//故障计数器 
	Rel27U.byRelRCounter				= 0;					//返回计数器 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
/************************************************************************************************
*功能: 初始化反时限保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRel51I()
{
	Rel51I.pRelCurSet					= RS_pn51C;                   			 //反时限保护电流定值 
	Rel51I.pRelTimeSet					= RS_pn51TD;					//反时限保护时间定值 
	Rel51I.pRelTypeSet					= RS_pn51TYPE;					//反时限保护曲线类型 

	Rel51I.pRelCFG						= CFG_pby51;						//反时限保护配置 
	Rel51I.pRelSW						= SW_pby50_123;						//反时限保护软压板,用的是过流保护压板 
	Rel51I.pSysTimer					= &dwRelayMSCnt;						//系统定时器 
	Rel51I.pCoefTabA					= InverseCurCoefA;					//一般反时限系数表地址 
	Rel51I.pCoefTabB					= InverseCurCoefB;						//极反时限系数表地址 
	Rel51I.pCoefTabC					= InverseCurCoefC;						//甚反时限系数表地址 

	Rel51I.pRelElec						= &Ip;						//当前的电量指针 
	Rel51I.dwCaluCurSet					= *RS_pn51C*11/10;					//计算后的反时限电流定值 

	Rel51I.ACTRelayWord					= RW_TR51;					//反时限动作继电器字 
	Rel51I.StartRelayWord				= RW_PI51;					//反时限入段继电器字 
	Rel51I.LockRelayWord				= RW_LKPI51;					//入段闭锁继电器字 
	Rel51I.RSTRelayWord					= RW_LKRST51;					//返回闭锁继电器字 
	Rel51I.LoopCtrlRelayWord			= RW_LPC51;				//故障循环控制继电器字 
	Rel51I.wRelRptNum					= wRep51I;					//反时限保护报告序号 
	Rel51I.RptSerialNum					= &RptSerialNumOfAct;
	//**********************入段报告部分*******************   yanxs  13-03-08
	Rel51I.wPickUpReportNum				= wRep51IPick; 					//入段报告inf序号
	Rel51I.wResetReportNum 				= wRep51IRES;						//返回报告inf序号
//	Rel51I.RptPickupSerialNum			= &RptSerialNumOfPick;				//入段报告号
//	Rel51I.RptPickupFlag				= &RptFlagOfPickUp;					//入段报告启动标志
	//**********************************************************************


//	Rel51I.dwRelTimer					= 0;						//反时限保护定时器 	
	Rel51I.byRelRCounter				= 0;						//故障计数器 
	Rel51I.byRelRCounter				= 0;						//返回计数器 
	Rel51I.CurCounter					= 0;						//电流累加量 
	Rel51I.LimitCouter					= 0;						//极限动作时间判断用的累加次数 
	Rel51I.MaxCouter					= MAXINVERSETIME*50*POINT_PER_PERIOD/RELAY_SAMP_INTERVAL;	
	Rel51I.CurItem						= 0;

	if (*(RS_pn51TYPE) == 1)								//电流累加量计算后的定值 (28000*50*SAMPNUM*Is*Is*Ts)  
	{
		Rel51I.CurCounterSet = (LLONG)INVERSECOEFA*POINT_PER_PERIOD*(*RS_pn51TD) \
			*(*RS_pn51C)*(*RS_pn51C)/(20*RELAY_SAMP_INTERVAL);
	}
	else if (*(RS_pn51TYPE) == 2)
	{
		Rel51I.CurCounterSet = (LLONG)INVERSECOEFB*POINT_PER_PERIOD*(*RS_pn51TD) \
			*(*RS_pn51C)*(*RS_pn51C)/(20*RELAY_SAMP_INTERVAL);
	}
	else if (*(RS_pn51TYPE) == 3)
	{
		Rel51I.CurCounterSet = (LLONG)INVERSECOEFC*POINT_PER_PERIOD*(*RS_pn51TD) \
			*(*RS_pn51C)*(*RS_pn51C)/(20*RELAY_SAMP_INTERVAL);
	}					
	Rel51I.wExcepWordNum	=16;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为34 

}
/************************************************************************************************
*功能: 初始化F线故障测距元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRel21FL_F()
{
	Rel21FL_F.pRel21FLCFG				= CFG_pby21_FL;						//故障测距配置字 
	Rel21FL_F.pRel21FLNSet				= RS_pnF21FLN;                             	       //测距分段数定值 
	Rel21FL_F.pRel21FL1LSet				= RS_pnF21FLN1L; 						//第1段距离定值 
	Rel21FL_F.pRel21FL1XSet				= RS_pnF21FLN1X;						//第1段单位电抗定值 
	Rel21FL_F.pRel21FL2LSet				= RS_pnF21FLN2L; 						//第2段距离定值 
	Rel21FL_F.pRel21FL2XSet				= RS_pnF21FLN2X;  					//第2段单位电抗定值 
	Rel21FL_F.pRel21FL3LSet				= RS_pnF21FLN3L ;  						//第3段距离定值 
	Rel21FL_F.pRel21FL3XSet				= RS_pnF21FLN3X; 						//第3段单位电抗定值 
	Rel21FL_F.pRel21FL4LSet				= RS_pnF21FLN4L ;  						//第4段距离定值 
	Rel21FL_F.pRel21FL4XSet				= RS_pnF21FLN4X; 						//第4段单位电抗定值 
	Rel21FL_F.pRel21FL5LSet				= RS_pnF21FLN5L;  						//第5段距离定值 
	Rel21FL_F.pRel21FL5XSet				= RS_pnF21FLN5X; 						//第5段单位电抗定值 
	Rel21FL_F.pRel21FL6LSet				= RS_pnF21FLN6L; 						//第6段距离定值 
	Rel21FL_F.pRel21FL6XSet				= RS_pnF21FLN6X; 						//第6段单位电抗定值 
	Rel21FL_F.pRel21FL7LSet				= RS_pnF21FLN7L;						//第7段距离定值 
	Rel21FL_F.pRel21FL7XSet				= RS_pnF21FLN7X; 						//第7段单位电抗定值 
	Rel21FL_F.pRel21FL8LSet				= RS_pnF21FLN8L; 						//第8段距离定值 
	Rel21FL_F.pRel21FL8XSet				= RS_pnF21FLN8X ;						//第8段单位电抗定值 
	Rel21FL_F.pRel21FL9LSet				= RS_pnF21FLN9L; 						//第9段距离定值 
	Rel21FL_F.pRel21FL9XSet				= RS_pnF21FLN9X;						//第9段单位电抗定值 
	Rel21FL_F.pRel21FL10LSet			= RS_pnF21FLN10L ; 					//第10段距离定值 
	Rel21FL_F.pRel21FL10XSet			= RS_pnF21FLN10X;					//第10段距离定值 
	Rel21FL_F.pRel21FLS0				= RS_pnS0;
	Rel21FL_F.pRel21FLSD				= RS_pnSD;
	Rel21FL_F.pRel21FLLc				= RS_pnLc;

    Rel21FL_F.pFaultValueL				= RM_pnFL;						//故障电量L的指针
	Rel21FL_F.pnLCoef					= CF_pnC2;
	Rel21FL_F.FaultValueNum				= RM_FL;
	Rel21FL_F.pSS						 = RM_pnSS;
	Rel21FL_F.SSNO						 = RM_SS;

	
	
	Rel21FL_F.pRel21FLImpd				= &ImpdF;						//当前阻抗的指针 
	Rel21FL_F.wExcepWordNum				= 0;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为0 
	
	CalcFaultLocateSetTab(&Rel21FL_F);  								//F线故障测距定指表计算
}
/************************************************************************************************
*功能: 初始化T线故障测距元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRel21FL_T()
{
	Rel21FL_T.pRel21FLCFG				= CFG_pby21_FL;						//故障测距配置字 
	Rel21FL_T.pRel21FLNSet				= RS_pnT21FLN;                             	       //测距分段数定值 
	Rel21FL_T.pRel21FL1LSet				= RS_pnT21FLN1L; 						//第1段距离定值 
	Rel21FL_T.pRel21FL1XSet				= RS_pnT21FLN1X;						//第1段单位电抗定值 
	Rel21FL_T.pRel21FL2LSet				= RS_pnT21FLN2L; 						//第2段距离定值 
	Rel21FL_T.pRel21FL2XSet				= RS_pnT21FLN2X;  					//第2段单位电抗定值 
	Rel21FL_T.pRel21FL3LSet				= RS_pnT21FLN3L ;  						//第3段距离定值 
	Rel21FL_T.pRel21FL3XSet				= RS_pnT21FLN3X; 						//第3段单位电抗定值 
	Rel21FL_T.pRel21FL4LSet				= RS_pnT21FLN4L ;  						//第4段距离定值 
	Rel21FL_T.pRel21FL4XSet				= RS_pnT21FLN4X; 						//第4段单位电抗定值 
	Rel21FL_T.pRel21FL5LSet				= RS_pnT21FLN5L;  						//第5段距离定值 
	Rel21FL_T.pRel21FL5XSet				= RS_pnT21FLN5X; 						//第5段单位电抗定值 
	Rel21FL_T.pRel21FL6LSet				= RS_pnT21FLN6L; 						//第6段距离定值 
	Rel21FL_T.pRel21FL6XSet				= RS_pnT21FLN6X; 						//第6段单位电抗定值 
	Rel21FL_T.pRel21FL7LSet				= RS_pnT21FLN7L;						//第7段距离定值 
	Rel21FL_T.pRel21FL7XSet				= RS_pnT21FLN7X; 						//第7段单位电抗定值 
	Rel21FL_T.pRel21FL8LSet				= RS_pnT21FLN8L; 						//第8段距离定值 
	Rel21FL_T.pRel21FL8XSet				= RS_pnT21FLN8X ;						//第8段单位电抗定值 
	Rel21FL_T.pRel21FL9LSet				= RS_pnT21FLN9L; 						//第9段距离定值 
	Rel21FL_T.pRel21FL9XSet				= RS_pnT21FLN9X;						//第9段单位电抗定值 
	Rel21FL_T.pRel21FL10LSet			= RS_pnT21FLN10L ; 					//第10段距离定值 
	Rel21FL_T.pRel21FL10XSet			= RS_pnT21FLN10X;					//第10段距离定值 	
	Rel21FL_T.pRel21FLS0				= RS_pnS0;
	Rel21FL_T.pRel21FLSD				= RS_pnSD;
	Rel21FL_T.pRel21FLLc				= RS_pnLc;
	
    Rel21FL_T.pFaultValueL				= RM_pnTL;						//故障电量L的指针 
	Rel21FL_T.pnLCoef					= CF_pnC2;                      //距离的放大倍数
	Rel21FL_T.FaultValueNum				= RM_TL;
	
	Rel21FL_T.pSS						 = RM_pnSS;
	Rel21FL_T.SSNO						 = RM_SS;
	Rel21FL_T.pRel21FLImpd				= &ImpdT;						//当前阻抗的指针 	
	Rel21FL_T.wExcepWordNum				= 0;						//本结构对象计算CRC校验时需排除在外的字节数，本结构为0 
	
	CalcFaultLocateSetTab(&Rel21FL_T);//T线故障测距定指表计算
}
/************************************************************************************************
*功能: 初始化PT1断线检测元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRelPT1()
{	
	RelPT1.pRelPTBrokCFG 		= CFG_pbyPT1BR;						//PT断线配置

	RelPT1.pR74PTU1Set			= RS_pn74VTV;								//PT断线检测电压定值	
	RelPT2.pR74PTU2Set			= NULL;
	RelPT1.pR74PTISet			= RS_pn74VTC;								//pt断线检测电流定值
	RelPT1.pR74PTTSet			= RS_pn74VTD;								//pt断线检测时间定值
	
	RelPT1.pRelCur				= &Ip;										//判断用电流
	RelPT1.pRelU1				= &U1;										//判断用第一路电压	
	RelPT2.pRelU2				= NULL;

	RelPT1.StartRelayWord		= RW_PIVT1;							//PT1断线入段继电器字
	RelPT1.ActRelayWord			= RW_ARVT1;							//PT1断线动作继电器字	
	RelPT1.dwSysTimer			= &dwRelayMSCnt;							//系统定时器
	RelPT1.wRelRptNum			= R_PT1;									//PT断线动作报告号1
	RelPT1.wRelResRptNum		= R_PT1_RES;								//PT断线返回报告号1

	RelPT1.wFCounter			= 0;
	//RelPT1.dwRelTimer			= 0;											//PT1断线定时器
	RelPT1.wExcepWordNum		= 5;											//本结构对象计算CRC校验时需排除在外的字节数，本结构为6
}
/************************************************************************************************
*功能: 初始化PT2断线检测元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitRelPT2()
{	
	RelPT2.pRelPTBrokCFG 		= CFG_pbyPT2BR;						//PT断线配置

	RelPT2.pR74PTU1Set			= RS_pn74VTV;								//PT断线检测电压定值
	RelPT2.pR74PTU2Set			= NULL;
	RelPT2.pR74PTISet			= RS_pn74VTC;								//pt断线检测电流定值
	RelPT2.pR74PTTSet			= RS_pn74VTD;								//pt断线检测时间定值
	
	RelPT2.pRelCur				= &Ip;										//判断用电流
	RelPT2.pRelU1				= &U2;		//判断用第一路电压
	RelPT2.pRelU2				= NULL;

	RelPT2.StartRelayWord		= RW_PIVT2;							//PT1断线入段继电器字
	RelPT2.ActRelayWord			= RW_ARVT2;							//PT1断线动作继电器字	
	RelPT2.dwSysTimer			= &dwRelayMSCnt;							//系统定时器
	RelPT2.wRelRptNum			= R_PT2;									//PT断线动作报告号1
	RelPT2.wRelResRptNum		= R_PT2_RES;								//PT断线返回报告号2

	RelPT2.wFCounter			= 0;
	//RelPT2.dwRelTimer			= 0;											//PT1断线定时器
	RelPT2.wExcepWordNum		= 5;											//本结构对象计算CRC校验时需排除在外的字节数，本结构为6
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
	Rel79.R79RSTSet						= RS_pn79RD;							//重合闸充放电时间 
	Rel79.R79SOTFTSet					= RS_pnSOTFD;						//合于故障检测延时，即为后加速投入时间 
	Rel79.R79PISTSet					= AS_pnTCB;						//重合闸启动监视延时，即为断路器动作延时 
	Rel79.R79CLSTSet					= RS_pn79CLSD;						//重合闸动作监视延时 

	
	Rel79.R79SCFG						= CFG_pby79UC;	//偷跳重合闸配置
	Rel79.R79SW							= SW_pby79;
	Rel79.R79RW							= RW_F79;	//	把配置放在重合闸闭锁继电器字里

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
	Rel79.wRpt79Fail					= R_R79FAIL;				//重合闸失败报告
	Rel79.wRpt79LK_1					= R_R79LK_1;					//重合闸被闭锁报告序号
	
	Rel79.wRpt79LK_2					= R_R79LK_2;					//重合闸被闭锁报告序号
	Rel79.wPickUpRptNum					= R_ACTPICK;
	
	Rel79.wRpt79_1_SUCC					= R_R79_1_SUCC;			//一次重合闸成功报告  by yanzh 2013年6月18日 17:31:57
	Rel79.wRpt79_1_FAIL					= R_R79_1_FAIL;			//一次重合闸失败报告
	Rel79.wRpt79_2_SUCC					= R_R79_2_SUCC;			//二次重合闸成功报告
	Rel79.wRpt79_2_FAIL					= R_R79_2_FAIL;			//二次重合闸失败报告
	Rel79.wRpt79PI_1					= R_R79PI_1;				//重合闸启动报告	
	Rel79.wRpt79PI_2					= R_R79PI_2;				//重合闸启动报告
	Rel79.wRpt79NPI_1						= R_R79NPI_1;				//重合闸未启动报告
	Rel79.wRpt79NPI_2						= R_R79NPI_2;				//重合闸未启动报告
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
	//Rel79.R79Timer						= 0;					//重合闸动作延时定时器 
	Rel79.R79ShotCnt					= 0;					//重合闸已经动作次数 	
	Rel79.wExcepWordNum					= 16;				//本结构对象计算CRC校验时需排除在外的字节数，本结构为12 

}
/************************************************************************************************
*功能: 初始化增量保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void  CRelayTask::InitRel50DI()
{
	Rel50DI.pRelCurSet				= RS_pn50DC;                   			 //增量保护电流定值 
	Rel50DI.pRelTimeSet				= RS_pn50DD;					//增量保护时间定值 
	Rel50DI.pRelLockCurSet			= RS_pn50D_37C;					//增量保护小电流闭锁定值 
	Rel50DI.pRelKh1					= RS_pnH2_PCT;						//二次谐波闭锁系数 
	Rel50DI.pRelKhr					= RS_pnFACTOR2;						//增量保护谐波加权系数 

	Rel50DI.pH2Coef					= CF_pnC2;
	Rel50DI.pRelCFG					= CFG_pby50D;						//增量保护配置 
	Rel50DI.pRelSW					= SW_pby50D;						//增量保护软压板 


	Rel50DI.pRelElec1				= &Ip;						//当前基波电流指针 
	Rel50DI.pRelElec2				= &I2;						//当前二次谐波电流指针 
	Rel50DI.pRelElec3				= &I3;						//当前三次谐波电流指针 
	Rel50DI.pRelElec5				= &I5;						//当前五次谐波电流指针 
	Rel50DI.pFaultSINum				= &FaultSINum;					//故障处理采样间隔计数器指针 
	Rel50DI.ACTRelayWord			= RW_TR50D;					//增量保护动作继电器字 
	Rel50DI.StartRelayWord			= RW_PI50D;					//增量保护入段继电器字 
	Rel50DI.LockRelayWord			= RW_LKPI50D;					//入段闭锁继电器字 
	Rel50DI.RSTRelayWord			= RW_LKRST50D;					//返回闭锁继电器字 
	Rel50DI.LoopCtrlRelayWord		= RW_LPC50D;				//故障循环控制继电器字 	
	Rel50DI.OtherRelTrip			= &m_lOtherRelayTrip;
	Rel50DI.R79ActRelayWord			= RW_79CL;
	Rel50DI.H2LockRelayWord			= RW_LKH2;
	Rel50DI.RptSerialNum			= &RptSerialNumOfAct;
	Rel50DI.pSysTimer				= &dwRelayMSCnt;						//系统定时器 
	Rel50DI.wRelRptNum				= wRep50DI;
	//**********************入段报告部分*******************   yanxs  13-03-08
	Rel50DI.wPickUpReportNum		= wRep50DIPick; 					//入段报告inf序号
	Rel50DI.wResetReportNum 		= wRep50DIRES;						//返回报告inf序号
//	Rel50DI.RptPickupSerialNum		= &RptSerialNumOfPick;				//入段报告号
//	Rel50DI.RptPickupFlag			= &RptFlagOfPickUp;					//入段报告启动标志
	//**********************************************************************
	
	Rel50DI.dwPreI1Mod				= 0;						//故障前基波模值 
	Rel50DI.dwPreI3Mod				= 0;						//故障前三次谐波模值 
	Rel50DI.dwPreI5Mod				= 0;						//故障前五次谐波模值 
	Rel50DI.PreCurHaveCalcFlag		= FALSE;				//故障前电量已经计算标志 
	Rel50DI.wRelFCounter			= 0;
	Rel50DI.wRelRCounter			= 0;
	Rel50DI.wExcepWordNum			= 14;
//	Rel50DI.dwRelTimer				= 0;						//欠量保护定时器 	

}
//------------------------------------------------------------------------------------------ 
// 名称: InitRepSerialNum	 
// 功能: 初始化保护动作报告序列号，供电方式的不同，同一保护动作的故障电量也不同 
// 输入:                                       
// 输出:                                                            
// 返回: 无                                                                                                                                                              
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRepSerialNum(void)
{
	if(*AS_pnPM == BTMODE)  //直供模式
	{
		wRep211 	= R_R211_D;
		wRep212 	= R_R212_D;
		wRep213		= R_R213_D;
		wRep214		= R_R214_D;
		wRep50I0 	= R_R50I0_D ;
		wRep50I1 	= R_R50I1_D;
		wRep50I2 	= R_R50I2_D;
		wRep50I3 	= R_R50I3_D;
		wRep51I 	= R_R51I_D;
		wRep50DI 	= R_R50DI_D;
		wRepR212Acc = R_R212_94_D;
		wRepR213Acc = R_R213_94_D;
		wRepR214Acc = R_R214_94_D;
		wRep50I1Acc	= R_R50I1_94_D;
		wRep50I2Acc	= R_R50I2_94_D;
		wRep50I3Acc	= R_R50I3_94_D;
		wRep27U		= R_R27U_D;
//************************入段报告部分*************************   yanxs  13-02-05
		wRep211Pick  = R_R211_D_PI;
		wRep212Pick  = R_R212_D_PI;
		wRep213Pick  = R_R213_D_PI;
		wRep214Pick  = R_R214_D_PI;
		wRep50I0Pick = R_R50I0_D_PI;
		wRep50I1Pick = R_R50I1_D_PI;
		wRep50I2Pick = R_R50I2_D_PI;
		wRep50I3Pick = R_R50I3_D_PI;
		wRep51IPick  = R_R51I_D_PI;
		wRep50DIPick = R_R50DI_D_PI;
		wRepR212AccPick = R_R212_94_D_PI;
		wRepR213AccPick = R_R213_94_D_PI;
		wRepR214AccPick = R_R214_94_D_PI;
		wRep50I1AccPick	= R_R50I1_94_D_PI;
		wRep50I2AccPick	= R_R50I2_94_D_PI;
		wRep50I3AccPick	= R_R50I3_94_D_PI;
//**************************保护返回报告***********************   yanxs  13-06-20
		wRep211RES 	= R_R211_D_RES;
		wRep212RES 	= R_R212_D_RES;
		wRep213RES 	= R_R213_D_RES;
		wRep214RES 	= R_R214_D_RES;
		wRep50I0RES	= R_R50I0_D_RES ;
		wRep50I1RES	= R_R50I1_D_RES;
		wRep50I2RES	= R_R50I2_D_RES;
		wRep50I3RES	= R_R50I3_D_RES;
		wRep51IRES	= R_R51I_D_RES;
		wRep50DIRES	= R_R50DI_D_RES;
		wRepR212AccRES = R_R212_94_D_RES;
		wRepR213AccRES = R_R213_94_D_RES;
		wRepR214AccRES = R_R214_94_D_RES;
		wRep50I1AccRES = R_R50I1_94_D_RES;
		wRep50I2AccRES = R_R50I2_94_D_RES;
		wRep50I3AccRES = R_R50I3_94_D_RES;
	}
	else if(*AS_pnPM == AT1MODE) //AT1供电方式
	{		
		wRep211 	= R_R211_S;
		wRep212 	= R_R212_S;
		wRep213 	= R_R213_S;
		wRep214 	= R_R214_S;
		wRep50I0	= R_R50I0_S ;
		wRep50I1	= R_R50I1_S;
		wRep50I2	= R_R50I2_S;
		wRep50I3	= R_R50I3_S;
		wRep51I 	= R_R51I_S;
		wRep50DI	= R_R50DI_S;
		wRepR212Acc = R_R212_94_S;
		wRepR213Acc = R_R213_94_S;
		wRepR214Acc = R_R214_94_S;
		wRep50I1Acc = R_R50I1_94_S;
		wRep50I2Acc = R_R50I2_94_S;
		wRep50I3Acc = R_R50I3_94_S;
		wRep27U 	= R_R27U_S;
		wRep211Pick  = R_R211_S_PI;
		wRep212Pick  = R_R212_S_PI;
		wRep213Pick  = R_R213_S_PI;
		wRep214Pick  = R_R214_S_PI;
		wRep50I0Pick = R_R50I0_S_PI;
		wRep50I1Pick = R_R50I1_S_PI;
		wRep50I2Pick = R_R50I2_S_PI;
		wRep50I3Pick = R_R50I3_S_PI;
		wRep51IPick  = R_R51I_S_PI;
		wRep50DIPick = R_R50DI_S_PI;
		wRepR212AccPick = R_R212_94_S_PI;
		wRepR213AccPick = R_R213_94_S_PI;
		wRepR214AccPick = R_R214_94_S_PI;
		wRep50I1AccPick = R_R50I1_94_S_PI;
		wRep50I2AccPick = R_R50I2_94_S_PI;
		wRep50I3AccPick = R_R50I3_94_S_PI;
		wRep211RES	= R_R211_S_RES;
		wRep212RES	= R_R212_S_RES;
		wRep213RES	= R_R213_S_RES;
		wRep214RES	= R_R214_S_RES;
		wRep50I0RES = R_R50I0_S_RES ;
		wRep50I1RES = R_R50I1_S_RES;
		wRep50I2RES = R_R50I2_S_RES;
		wRep50I3RES = R_R50I3_S_RES;
		wRep51IRES	= R_R51I_S_RES;
		wRep50DIRES = R_R50DI_S_RES;
		wRepR212AccRES = R_R212_94_S_RES;
		wRepR213AccRES = R_R213_94_S_RES;
		wRepR214AccRES = R_R214_94_S_RES;
		wRep50I1AccRES = R_R50I1_94_S_RES;
		wRep50I2AccRES = R_R50I2_94_S_RES;
		wRep50I3AccRES = R_R50I3_94_S_RES;
	}
	else  //AT供电方式，包括普通AT和客专AT两种模式
	{		
		wRep211 	= R_R211;
		wRep212 	= R_R212;
		wRep213		= R_R213;
		wRep214		= R_R214;
		wRep50I0	= R_R50I0;
		wRep50I1 	= R_R50I1;
		wRep50I2 	= R_R50I2;
		wRep50I3 	= R_R50I3;
		wRep51I 	= R_R51I;
		wRep50DI 	= R_R50DI;
		wRepR212Acc = R_R212_94;
		wRepR213Acc = R_R213_94;
		wRepR214Acc = R_R214_94;
		wRep50I1Acc	= R_R50I1_94;
		wRep50I2Acc	= R_R50I2_94;
		wRep50I3Acc	= R_R50I3_94;
		wRep27U		= R_R27TR;
//**************************入段报告部分************  yanxs  13-02-05
		wRep211Pick  = R_R211_PI;
		wRep212Pick  = R_R212_PI;
		wRep213Pick  = R_R213_PI;
		wRep214Pick  = R_R214_PI;
		wRep50I0Pick = R_R50I0_PI;
		wRep50I1Pick = R_R50I1_PI;
		wRep50I2Pick = R_R50I2_PI;
		wRep50I3Pick = R_R50I3_PI;
		wRep51IPick  = R_R51I_PI;
		wRep50DIPick = R_R50DI_PI;
		wRepR212AccPick = R_R212_94_PI;
		wRepR213AccPick = R_R213_94_PI;
		wRepR214AccPick = R_R214_94_PI;
		wRep50I1AccPick	= R_R50I1_94_PI;
		wRep50I2AccPick	= R_R50I2_94_PI;
		wRep50I3AccPick	= R_R50I3_94_PI;
		wRep211RES	= R_R211_RES;
		wRep212RES	= R_R212_RES;
		wRep213RES	= R_R213_RES;
		wRep214RES	= R_R214_RES;
		wRep50I0RES = R_R50I0_RES;
		wRep50I1RES = R_R50I1_RES;
		wRep50I2RES = R_R50I2_RES;
		wRep50I3RES = R_R50I3_RES;
		wRep51IRES	= R_R51I_RES;
		wRep50DIRES = R_R50DI_RES;
		wRepR212AccRES = R_R212_94_RES;
		wRepR213AccRES = R_R213_94_RES;
		wRepR214AccRES = R_R214_94_RES;
		wRep50I1AccRES = R_R50I1_94_RES;
		wRep50I2AccRES = R_R50I2_94_RES;
		wRep50I3AccRES = R_R50I3_94_RES;
//		wRep27UPick  = R_R27U_PICK;
//****************************************************
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
	InitRepSerialNum();
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

/*------------------------------------------------------------------------------------------
名称: FaultLocate	
功能：故障测距定值计算                                                             
参数: 故障测距定值表                                        
返回:                                                         
备注: 无                                                                                                                                                             
------------------------------------------------------------------------------------------ */
void CRelayTask::CalcFaultLocateSetTab(TRelFaultLocatePar *Relay)
{
	if ((*Relay->pRel21FLCFG) == CFG_STATUS_CLR)
	{
		return;				//未投入，退出 
	}
	//第一段定值计算 
	Relay->SetTab[0]  = 0;
	Relay->SetTab[1]  = *(Relay->pRel21FL1XSet);
	Relay->SetTab[2]  = (*(Relay->pRel21FL1LSet)) * (*(Relay->pRel21FL1XSet));
	if (*(Relay->pRel21FLNSet) < 2)
		return;
	//第二段定值计算 
	Relay->SetTab[3]  = Relay->SetTab[0] + (*(Relay->pRel21FL1LSet));
	Relay->SetTab[4]  = *(Relay->pRel21FL2XSet);
	Relay->SetTab[5]  = Relay->SetTab[2] + (*(Relay->pRel21FL2XSet)*(*(Relay->pRel21FL2LSet)));
	if (*(Relay->pRel21FLNSet) < 3)
		return;
	//第三段定值计算 
	Relay->SetTab[6]  = Relay->SetTab[3] + (*(Relay->pRel21FL2LSet));
	Relay->SetTab[7]  = *(Relay->pRel21FL3XSet);
	Relay->SetTab[8]  = Relay->SetTab[5] + (*(Relay->pRel21FL3XSet)*(*(Relay->pRel21FL3LSet)));
	if (*(Relay->pRel21FLNSet) < 4)
		return;
	//第四段定值计算 
	Relay->SetTab[9]  = Relay->SetTab[6] + (*(Relay->pRel21FL3LSet));
	Relay->SetTab[10] = *(Relay->pRel21FL4XSet);
	Relay->SetTab[11] = Relay->SetTab[8] + (*(Relay->pRel21FL4XSet)*(*(Relay->pRel21FL4LSet)));
	if (*(Relay->pRel21FLNSet) < 5)
		return;
	//第五段定值计算 
	Relay->SetTab[12] = Relay->SetTab[9] + (*(Relay->pRel21FL4LSet));
	Relay->SetTab[13] = *(Relay->pRel21FL5XSet);
	Relay->SetTab[14] = Relay->SetTab[11] + (*(Relay->pRel21FL5XSet)*(*(Relay->pRel21FL5LSet)));
	if (*(Relay->pRel21FLNSet) < 6)
		return;
	//第六段定值计算 
	Relay->SetTab[15] = Relay->SetTab[12] + (*(Relay->pRel21FL5LSet));
	Relay->SetTab[16] = *(Relay->pRel21FL6XSet);
	Relay->SetTab[17] = Relay->SetTab[14] + (*(Relay->pRel21FL6XSet)*(*(Relay->pRel21FL6LSet)));
	if (*(Relay->pRel21FLNSet) < 7)
		return;
	//第七段定值计算 
	Relay->SetTab[18] = Relay->SetTab[15] + (*(Relay->pRel21FL6LSet));
	Relay->SetTab[19] = *(Relay->pRel21FL7XSet);
	Relay->SetTab[20] = Relay->SetTab[17] + (*(Relay->pRel21FL7XSet)*(*(Relay->pRel21FL7LSet)));
	if (*(Relay->pRel21FLNSet) < 8)
		return;
	//第八段定值计算 
	Relay->SetTab[21] = Relay->SetTab[18] + (*(Relay->pRel21FL7LSet));
	Relay->SetTab[22] = *(Relay->pRel21FL8XSet);
	Relay->SetTab[23] = Relay->SetTab[20] + (*(Relay->pRel21FL8XSet)*(*(Relay->pRel21FL8LSet)));
	if (*(Relay->pRel21FLNSet) < 9)
		return;
	//第九段定值计算 
	Relay->SetTab[24] = Relay->SetTab[21] + (*(Relay->pRel21FL8LSet));
	Relay->SetTab[25] = *(Relay->pRel21FL9XSet);
	Relay->SetTab[26] = Relay->SetTab[23] + (*(Relay->pRel21FL9XSet)*(*(Relay->pRel21FL9LSet)));
	if (*(Relay->pRel21FLNSet) < 10)
		return;
	//第十段定值计算 
	Relay->SetTab[27] = Relay->SetTab[24] + (*(Relay->pRel21FL9LSet));
	Relay->SetTab[28] = *(Relay->pRel21FL10XSet);
	Relay->SetTab[29] = Relay->SetTab[26] + (*(Relay->pRel21FL10XSet)*(*(Relay->pRel21FL10LSet)));
	if (*(Relay->pRel21FLNSet) > 10)
		return;
}
/*------------------------------------------------------------------------------------------
名称: FaultLocate	
功能：电抗法故障测距                                                             
参数:                                         
返回:                                                         
备注: 无                                                                                                                                                             
------------------------------------------------------------------------------------------ */

void CRelayTask::FaultLocate(TRelFaultLocatePar *Relay)
{
	WORD num = 0;
	DWORD React = ToAbs(Relay->pRel21FLImpd->Imag)*(*Relay->pnLCoef);

	if ((*Relay->pRel21FLCFG) == CFG_STATUS_CLR)
	{
		*Relay->pFaultValueL= 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	if ((*(Relay->pRel21FLNSet)>MAXFLNUM) || (*(Relay->pRel21FLNSet)<=0))
	{
		*Relay->pFaultValueL= 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	for (;num<(*(Relay->pRel21FLNSet));num++)     //判断故障点落在哪个段中 
	{
		if (React<=Relay->SetTab[num*3+2])
			break;		
	}
	if ((num == (*(Relay->pRel21FLNSet))||(Relay->SetTab[num*3+1] == 0)))  //电抗超过最大值，或者单位阻抗为0 
	{
		*(Relay->pFaultValueL) = 0;
		DB_LogWriteRM(Relay->FaultValueNum, 0 );
		return;
	}
	if (num == 0)  //故障点在第一段内 	
	{
		*(Relay->pFaultValueL) = React/Relay->SetTab[1];
		DB_LogWriteRM(Relay->FaultValueNum, *(Relay->pFaultValueL) );
		return;
	}
	
	*(Relay->pFaultValueL) = Relay->SetTab[num*3] + (React - Relay->SetTab[num*3-1])/Relay->SetTab[num*3+1];
	
	DB_LogWriteRM(Relay->FaultValueNum, *(Relay->pFaultValueL) );
	if(*Relay->pRel21FLSD == FOREWORD)
		*Relay->pSS = (*Relay->pRel21FLS0) + (*Relay->pFaultValueL)-(*Relay->pRel21FLLc);
	else
		*Relay->pSS = (*Relay->pRel21FLS0) - (*Relay->pFaultValueL)+(*Relay->pRel21FLLc);

	
	DB_LogWriteRM(Relay->SSNO, *Relay->pSS);

}

//------------------------------------------------------------------------------------------ 
// 功能: 模值启动判别程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	BYTE  pChannel[]={CHANNEL_6520_IP};
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
	if(((*Rel50I1.pRelCFG == CFG_STATUS_SET) ||(*Rel50I1.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I1.pRelSW == SW_STATUS_SET)) //过流1段投入 
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
	if(((*Rel50I2.pRelCFG == CFG_STATUS_SET) ||(*Rel50I2.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I2.pRelSW == SW_STATUS_SET)) //过流2段投入 
	{
		if(Rel50I2.pRelElec->Mod > *Rel50I2.pElecSet)
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
	if(((*Rel50I3.pRelCFG == CFG_STATUS_SET)||(*Rel50I3.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I3.pRelSW == SW_STATUS_SET)) //过流3段投入 
	{
		if(Rel50I3.pRelElec->Mod > *Rel50I3.pElecSet)
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
	if((*Rel51I.pRelCFG == CFG_STATUS_SET) && (*Rel51I.pRelSW == SW_STATUS_SET)) //反时限保护投入 
	{
		if(Rel51I.pRelElec->Mod > *Rel51I.pRelCurSet*11/10)
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
	//突变量启动部分
	if(((*Rel50I0.pRelCFG == CFG_STATUS_SET) && (*Rel50I0.pRelSW == SW_STATUS_SET)) || \
		(((*Rel50I1.pRelCFG == CFG_STATUS_SET) ||(*Rel50I1.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I1.pRelSW == SW_STATUS_SET)) || \
		(((*Rel50I2.pRelCFG == CFG_STATUS_SET) ||(*Rel50I2.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I2.pRelSW == SW_STATUS_SET)) || \
		(((*Rel50I3.pRelCFG == CFG_STATUS_SET) ||(*Rel50I3.pRelAccCFG == CFG_STATUS_SET))&& (*Rel50I3.pRelSW == SW_STATUS_SET)) || \
		((*Rel51I.pRelCFG == CFG_STATUS_SET) && (*Rel51I.pRelSW == SW_STATUS_SET))  || \
		((*Rel211.pRel21CFG == CFG_STATUS_SET) && (*Rel211.pRel21SW == SW_STATUS_SET)) || \
		((*Rel212.pRel21CFG == CFG_STATUS_SET) && (*Rel212.pRel21SW == SW_STATUS_SET)) || \
		((*Rel213.pRel21CFG == CFG_STATUS_SET) && (*Rel213.pRel21SW == SW_STATUS_SET)) || \
		((*Rel214.pRel21CFG == CFG_STATUS_SET) && (*Rel214.pRel21SW == SW_STATUS_SET)) ||
		((*Rel50DI.pRelCFG == CFG_STATUS_SET)&&(*Rel50DI.pRelSW== SW_STATUS_SET)))
	
	{
		PickCheck = 0;
		ReadPreSingleData(dwPickDataBuf,pChannel,1,RELAY_SAMP_INTERVAL);

		//算法1，根据宏RELAY_SAMP_INTERVAL确定的通用算法
		//将3个周波的采样点分别取至相应的数组,并计算突变量,且进行判断
		//注意:RELAY_SAMP_INTERVAL的值不能小于3
		for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
		{
			lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i];
			lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i];
			lSampDate3 = dwPickDataBuf[i];
			lPickValue=MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));
			
//			if(lPickValue>*RS_pnPIC)				
			if(lPickValue*4>*RS_pnPIC*4+m_lPickUpValue)
			{
				PickCheck++;
				if(PickCheck ==3)
				{					
					QWORD PickTime; 					
					ReadAbsTime(&PickUpTime);
					PickTime = PickUpTime.Hi;
					PickTime <<= 32;
					PickTime += PickUpTime.Lo-(i*625+500)/1000;
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
						PickTime = PickUpTime.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime.Lo-(i*625+500)/1000;
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
				if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
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
	if(!FaultOnFlag)   						//尚未进入故障处理
	{		
		RelayPickUpCHK();					//执行启动检测
		if(ReadRelayWord(PickUpRW))
		{
			FaultSINum = 0;       //保护启动，初始化故障处理采样间隔计数器
			FaultOnFlag=TRUE;
			ResetAllRelay(TRUE);
			//进入故障处理，不进行PT断线判断
			if(!ReadRelayWord(RelPT1.ActRelayWord))
				WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);
			if(!ReadRelayWord(RelPT2.ActRelayWord))
				WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);	
		}
		else
			return;		
	}
	FaultSINum += RELAY_SAMP_INTERVAL;				//故障处理采样间隔计数器累加
	if(FaultSINum<READ_POINT_NUM)					//故障处理采样间隔计数器小于一周波采样点数
	{
		//增量保护的增量电流计算
		CalcInCreCurrent(&Rel50DI,dwIncreCurBuf,dwInCreDataMask);
		return;
	}	
	else
	{
		if(!HaveMakeStartRptFlag)   //向带故障电量的启动报告中写入故障电量
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}

		//调用故障测距程序，计算故障点距离
		if(*AS_pnPM == BTMODE)
			FaultLocate(&Rel21FL_T);
		if(*AS_pnPM == AT2MODE)
		{
			FaultLocate(&Rel21FL_T);
			FaultLocate(&Rel21FL_F);			
		}
		
		if((!ReadRelayWord(RW_37IP))&&(I2.Mod*(*CF_pnC2)>(*RS_pnH2_PCT)*Ip.Mod))
		{				
			WriteRelayWord(RW_LKH2, RELAY_ON);	//置二次谐波闭锁继电器字	
		}
		else
		{
			WriteRelayWord(RW_LKH2, RELAY_OFF);
		}
		
		if(ReadRelayWord(RW_SL21))
		{
			if(ReadRelayWord(RW_F21_1)||ReadRelayWord(RW_F21_2)||ReadRelayWord(RW_F21_3)||ReadRelayWord(RW_F21_4))
			{
				if((!ReadRelayWord(RW_F74VT1)&&((*AS_pnPM == AT1MODE)||(*AS_pnPM == BTMODE))) \
					||(!ReadRelayWord(RW_F74VT1)&&!ReadRelayWord(RW_F74VT2)&&(*AS_pnPM == AT2MODE)))  //PT断线未投入
				{
					GeneralRelay21(&Rel211);						//调用距离I段保护子程序
					GeneralRelay21(&Rel212);						//调用距离II段保护子程序		
					GeneralRelay21(&Rel213);						//调用距离III段保护子程序
					GeneralRelay21(&Rel214);						//调用距离IV段保护子程序
				}
				else
				{	
					if(Ip.Mod>*RS_pn74VTC)
					{							
						DWORD tempzt,tempzf;
						if (*AS_pnPM == AT2MODE)
						{
							tempzt = ToAbs(ImpdT.Imag) + ToAbs(ImpdT.Real);    // |R|+|X|<1欧姆
							tempzf = ToAbs(ImpdF.Imag) + ToAbs(ImpdF.Real);    // |R|+|X|<1欧姆
						}
						else
						{
							tempzt = ToAbs(Impd.Imag) + ToAbs(Impd.Real);   // |R|+|X|<1欧姆
							tempzf = tempzt;
						}
						
						if((Ip.Mod<*RS_pn74VTBLC)&&(tempzt < (*CF_pnZ)||tempzf<(*CF_pnZ)))
							m_bOneOhmPTLK=TRUE;
						else
							m_bOneOhmPTLK=FALSE;
					}
					else
					{
						m_bOneOhmPTLK=FALSE;
					}
					if(m_bOneOhmPTLK)//已判出的PT断线作为距离保护的入段闭锁继电器字进行闭锁，该条件不做PT断线闭锁距离保护报告
					{
						if(!m_bIsHaveMakePTRpt)	//作PT短线闭锁距离保护报告
						{
							MakeAlarmRpt(R_PTLK);
							m_bIsHaveMakePTRpt= TRUE;							
						}						
					}
					else
					{
						GeneralRelay21(&Rel211);						//调用距离I段保护子程序
						GeneralRelay21(&Rel212);						//调用距离II段保护子程序		
						GeneralRelay21(&Rel213);						//调用距离III段保护子程序
						GeneralRelay21(&Rel214);						//调用距离IV段保护子程序
					}
				}
			}	
			else
			{
				if(ReadRelayWord(RW_F21_2_94)||ReadRelayWord(RW_F21_3_94)||ReadRelayWord(RW_F21_4_94))
				{						
					GeneralRelay21(&Rel212);						//调用距离II段保护子程序		
					GeneralRelay21(&Rel213);						//调用距离III段保护子程序
					GeneralRelay21(&Rel214);						//调用距离IV段保护子程序
				}
			}
		}

		
		if((*Rel50I0.pRelPDCFG ==CFG_STATUS_SET)||(*Rel50I1.pRelPDCFG ==CFG_STATUS_SET)||(*Rel50I2.pRelPDCFG ==CFG_STATUS_SET)     //功率方向判断
			||(*Rel50I3.pRelPDCFG ==CFG_STATUS_SET))
		{
			if(!m_bPDCheck)   //未发生带记忆功率方向判断
			{
				if((!ReadRelayWord(RW_ARVT1))&&(!ReadRelayWord(RW_ARVT2)))		//2013-04-27   PT断线情况下，功率方向闭锁功能失效
				{
					m_bPDCheck = PowerDirCheck(&Up,&Ip,dwPDDataBuf,*RS_pnZANGLE,*CF_pnUP,RW_32F,RW_32R,CHANNEL_6520_UP,TRUE);
				}
				else
				{				
					WriteRelayWord(RW_32R, RELAY_OFF);
					WriteRelayWord(RW_32F, RELAY_OFF);
				}
			}
		}
		
		GeneralOverValueRelay(&Rel50I0);				//调用速断子程序
		GeneralOverValueRelay(&Rel50I1);				//调用过流I段保护子程序
		GeneralOverValueRelay(&Rel50I2);				//调用过流III段保护子程序
		GeneralOverValueRelay(&Rel50I3);				//调用过流III段保护子程序
		InverseCurRelay(&Rel51I);						//调用反时限保护子程序
		
		if(ReadRelayWord(RW_TR50_0)||ReadRelayWord(RW_TR50_1)||ReadRelayWord(RW_TR50_1_94)||ReadRelayWord(RW_TR50_2)||
			ReadRelayWord(RW_TR50_2_94)||ReadRelayWord(RW_TR50_3)||ReadRelayWord(RW_TR50_3_94)||ReadRelayWord(RW_TR21_1)||
			ReadRelayWord(RW_TR21_2)||ReadRelayWord(RW_TR21_2_94)||ReadRelayWord(RW_TR21_3)||ReadRelayWord(RW_TR21_3_94)||
			ReadRelayWord(RW_TR21_4)||ReadRelayWord(RW_TR21_4_94)||ReadRelayWord(RW_TR51)||ReadRelayWord(RW_TR27))
		{
			m_lOtherRelayTrip = TRUE;  //除了增量保护之外，其他保护动作标志，若有动作，增量保护返回。
		}		
		IncreCurrentRelay(&Rel50DI);					//调用电流增量保护子程序
		

		if(ReadRelayWord(RW_52BF))
		{
			if(!m_bHaveTripFailedRpt)
			{				
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//作断路器跳闸失败报告
				m_bHaveTripFailedRpt = TRUE;
			}
		}

		if((Rel79.R79TFLAG)||(Rel79.R79PISFLAG))					//重合闸已经进入延时状态,等待重合闸动作
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
				if(!ReadRelayWord(RW_37IP))   				//检查有流继电器字是否被置位
				{
					m_lOtherRelayTrip = FALSE;  //除了增量保护之外，其他保护动作标志，若有动作，增量保护返回。
					FaultSINum = 0;		//重新初始化故障处理采样间隔计数器
					CurRecoverFlag = TRUE;    				//置电流恢复标志
					return;
				}				
				return;
			}
		}
		
		// 1。有保护循环控制继电器字被置位，等待保护动作或者退出
		if(ReadRelayWord(Rel211.LoopCtrlRelayWord)||ReadRelayWord(Rel212.LoopCtrlRelayWord)||ReadRelayWord(Rel213.LoopCtrlRelayWord)
			||ReadRelayWord(Rel214.LoopCtrlRelayWord)||ReadRelayWord(Rel50I0.LoopCtrlRelayWord)||ReadRelayWord(Rel50I1.LoopCtrlRelayWord) 
			||ReadRelayWord(Rel50I2.LoopCtrlRelayWord)||ReadRelayWord(Rel50I3.LoopCtrlRelayWord)||ReadRelayWord(Rel51I.LoopCtrlRelayWord)
			||ReadRelayWord(Rel50DI.LoopCtrlRelayWord))
		{
			// 1.1 保护已经动作，且跳闸失败
			if(ReadRelayWord(RW_52BF))
			{
				WriteRelayWord(PickUpRW, RELAY_OFF);		//清保护启动继电器字
				FaultCheckOutFlag = FALSE;					//清检出故障标志
				FaultOnFlag = FALSE;						//清故障循环处理标志
				HaveMakeStartRptFlag= FALSE;				//清已作带故障电量启动报告标志
				m_bIsHaveMakePTRpt = FALSE;					//清已作PT断线闭锁距离保护告警报告标志				
				m_bFaultLKFlag = FALSE; 					//清有启动无动作时启动时刻锁定标志	
				m_bPDCheck = FALSE;							//清已作带记忆功率方向判断标志
				CurRecoverFlag = FALSE;						//清重合闸动作后电流恢复标志
				DB_RelayEnd(RptSerialNumOfAct); 			//向数据库中写入结束报告	
				m_bHaveRelayAct = FALSE;
				m_lOtherRelayTrip = FALSE;
				m_bHaveTripFailedRpt = FALSE;
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
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime > 1000 )  //无故障时间超过合于故障检测时间
				{					
					WriteRelayWord(PickUpRW, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
					CurRecoverFlag	 = FALSE;					//清重合闸动作后电流恢复标志		
					m_bIsHaveMakePTRpt = FALSE;					//清已作PT断线闭锁距离保护告警报告标志					
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					m_bPDCheck = FALSE;							//清已作带记忆功率方向判断标志
					m_bHaveRelayAct = FALSE;
					m_lOtherRelayTrip = FALSE;					
					m_bR79PickUpRpt = FALSE;					
					DB_RelayEnd(RptSerialNumOfAct);				//向数据库中写入结束报告
					ResetAllRelay(TRUE);
					m_bHaveTripFailedRpt = FALSE;

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
					WriteRelayWord(PickUpRW, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志						
					CurRecoverFlag	 = FALSE;					//清重合闸动作后电流恢复标志		
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					m_bIsHaveMakePTRpt = FALSE;					//清已作PT断线闭锁距离保护告警报告标志
					m_bPDCheck = FALSE;							//清已作带记忆功率方向判断标志
					DB_RelayEnd(RptSerialNumOfAct); 			
					m_bHaveRelayAct = FALSE;
					m_lOtherRelayTrip = FALSE;					
					m_bR79PickUpRpt = FALSE;
					ResetAllRelay(TRUE);	
					m_bHaveTripFailedRpt = FALSE;
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
			ResetUnderValRelay(&Rel27U);
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
/*	if(ReadRelayWord(RW_52BF))			//跳闸失败继电器字置位
	{
		TripFailFlag=TRUE;					//置跳闸失败标志
		return;
	}
	else									//无跳闸失败继电器字置位，有跳闸失败标志，整组复归，即读取了跳闸失败继电器字的下降沿
	{
		if(TripFailFlag)
		{
*/			ResetRelay21(&Rel211,resetFlag);
			ResetRelay21(&Rel212,resetFlag);
			ResetRelay21(&Rel213,resetFlag);
			ResetRelay21(&Rel214,resetFlag);
			
			ResetOverValueRelay(&Rel50I0,resetFlag);
			ResetOverValueRelay(&Rel50I1,resetFlag);
			ResetOverValueRelay(&Rel50I2,resetFlag);
			ResetOverValueRelay(&Rel50I3,resetFlag);
			ResetUnderValRelay(&Rel27U,resetFlag);
			ResetInverseCurRel(&Rel51I,resetFlag);
			ResetR50DI(&Rel50DI,resetFlag);
//			if (RptFlagOfPickUp == TRUE)
//			{
//				DB_RelayEnd(RptSerialNumOfPick); 			//向数据库中写入结束报告
//				RptFlagOfPickUp = FALSE;
//			}
//			TripFailFlag=FALSE;
//		}		
//	}	
}
void CRelayTask::PTCheck()
{	
	if(*RelPT1.pRelPTBrokCFG == CFG_STATUS_SET)
	{
		if(RelPT1.pRelU1->Mod < *RelPT1.pR74PTU1Set)  //U1小于定值
		{
			if(RelPT1.pRelCur->Mod > *RelPT1.pR74PTISet)   //有流，继续判断
			{				
				if(!ReadRelayWord(RelPT1.ActRelayWord))		//尚未动作
				{
					if(!ReadRelayWord(RelPT1.StartRelayWord))		//尚未入段
					{
						RelPT1.wFCounter++;
						if(RelPT1.wFCounter>2)
						{
							RelPT1.dwRelTimer = *RelPT1.dwSysTimer;
							WriteRelayWord(RelPT1.StartRelayWord, RELAY_ON);
						}
					}
					else
					{
						if((*RelPT1.dwSysTimer - RelPT1.dwRelTimer) > *RelPT1.pR74PTTSet)
						{
							WriteRelayWord(RelPT1.ActRelayWord, RELAY_ON);
							MakeAlarmRpt(RelPT1.wRelRptNum);
						}
					}
				}
			}
			else
			{				
				RelPT1.dwRelTimer  =0;
				WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);				
			}
		}
		else   //PT1断线恢复
		{
			RelPT1.dwRelTimer = 0;
			WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);
			if(ReadRelayWord(RelPT1.ActRelayWord))		//已经动作	
			{
				MakeAlarmRpt(RelPT1.wRelResRptNum);
				WriteRelayWord(RelPT1.ActRelayWord, RELAY_OFF);
			}
		}
	}
	else
	{
		RelPT1.dwRelTimer  =0;
		if(ReadRelayWord(RelPT1.ActRelayWord))		//已经动作	
		{
			MakeAlarmRpt(RelPT1.wRelResRptNum);
			WriteRelayWord(RelPT1.ActRelayWord, RELAY_OFF);
		}
		WriteRelayWord(RelPT1.StartRelayWord, RELAY_OFF);
	}

	
	if((*RelPT2.pRelPTBrokCFG == CFG_STATUS_SET))
	{
		if(RelPT2.pRelU1->Mod < *RelPT2.pR74PTU1Set)  //U2小于定值
		{
			if(RelPT2.pRelCur->Mod > *RelPT2.pR74PTISet)   //有流，继续判断
			{				
				if(!ReadRelayWord(RelPT2.ActRelayWord))		//尚未动作
				{
					if(!ReadRelayWord(RelPT2.StartRelayWord))		//尚未入段
					{
						RelPT2.wFCounter++;
						if(RelPT2.wFCounter > 2)
						{
							RelPT2.dwRelTimer = *RelPT2.dwSysTimer;
							WriteRelayWord(RelPT2.StartRelayWord, RELAY_ON);
						}
					}
					else 
					{
						if((*RelPT2.dwSysTimer - RelPT2.dwRelTimer) > *RelPT2.pR74PTTSet)
						{
							WriteRelayWord(RelPT2.ActRelayWord, RELAY_ON);
							MakeAlarmRpt(RelPT2.wRelRptNum);
						}
					}
				}		
			}
			else
			{
				RelPT2.dwRelTimer  =0;
				WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);
			}
		}
		else 		//PT2断线恢复
		{
			RelPT2.dwRelTimer = 0;
			WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);
			if(ReadRelayWord(RelPT2.ActRelayWord))		//已经动作
			{
				MakeAlarmRpt(RelPT2.wRelResRptNum);
				WriteRelayWord(RelPT2.ActRelayWord, RELAY_OFF);		
			}
		}
	}
	else			//PT2断线恢复
	{
		RelPT2.dwRelTimer  =0;		
		if(ReadRelayWord(RelPT2.ActRelayWord))		//已经动作
		{
			MakeAlarmRpt(RelPT2.wRelResRptNum);
			WriteRelayWord(RelPT2.ActRelayWord, RELAY_OFF);		
		}
		WriteRelayWord(RelPT2.StartRelayWord, RELAY_OFF);
	}

}


