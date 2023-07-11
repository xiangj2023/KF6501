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
#include "Relay.h"

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

extern DWORD g_hDbaseTaskID;
extern DWORD dwRelayDataPtr;
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
extern TSysConfigTable	*G_pCfgTable;

extern BYTE g_RptSerialNum;	        //报告序列号 
extern BYTE byRelayWordValue[];
extern BOOL RelayCheckInitOK;
extern BOOL RelayCheckErr;			//保护自检出错
extern DWORD dwADResumeCnt;
extern BYTE *g_MaintSW;

DWORD 		 dwRelayTaskID;
BOOL 		 RelayTaskInitOK = FALSE;
DWORD		 dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD		 dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
TStructCheck StructCheck[CHECK_NUM];					//保护元件校验数组

TRunTimeRM	*pSysRM;		//保护测量值数据指针
TRunTimeBI  *pSysBI;		//遥信实时数据指针

DWORD dwCaluInterval;	//热过负荷计算累加间隔
BOOL  g_bRel49Flag;		//热过负荷进行累加计算消息标志
BOOL  g_bRel49TimeOut;  //热过负荷累加延时到标志

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
	CRelayTask* pRelayTask = new CRelayTask(pdwApp);
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

CRelayTask::CRelayTask(DWORD *pdwApp):CApp(pdwApp),TimeSet(0)
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
	InitSet();
	InitDCRecInfo();   //初始化录波

    RelCFGCLR = CFG_STATUS_CLR;		//保护配置退出
	RelCFGSET = CFG_STATUS_SET;		//保护配置投入
	
	RptSerialNumOfAct =0;           //动作报告序列号，作动作报告用
	RptSerialNumOfStart=0;          //启动报告序列号，作带故障电量启动报告用
	FaultSINum = 0;                 //采样间隔数计数器
	
	b68IdH2Flag_A = FALSE;			//A相差流二次闭锁标志	
	b68IdH2Flag_B = FALSE;			//B相差流二次闭锁标志	
	b68IdH2Flag_C = FALSE;			//C相差流二次闭锁标志
	b68IdH5Flag_A = FALSE;	//A相差流5次闭锁标志	
	b68IdH5Flag_B = FALSE;	//B相差流5次闭锁标志	
	b68IdH5Flag_C = FALSE;	//C相差流5次闭锁标志			
	b68IrH2Flag_A = FALSE;	//A相制动电流2次闭锁标志	
	b68IrH2Flag_B = FALSE;	//B相制动电流2次闭锁标志	
	b68IrH2Flag_C = FALSE;	//C相制动电流2次闭锁标志
	b68IdHMulFlag_A = FALSE;
	b68IdHMulFlag_B = FALSE;
	b68IdHMulFlag_C = FALSE;


	HA_PDWithMemoryFlag = FALSE;
	HB_PDWithMemoryFlag = FALSE;
	HC_PDWithMemoryFlag = FALSE;
	LA_PDWithMemoryFlag = FALSE;
	LB_PDWithMemoryFlag = FALSE;
	LC_PDWithMemoryFlag = FALSE;

	HPTCheckTimer1 = 0;
	HPTCheckTimer2 = 0;
	HPTCheckTimer3 = 0;
	HPTCheckTimer4 = 0;	
	LPTCheckTimer1 = 0;
	LPTCheckTimer2 = 0;
	LPTCheckTimer3 = 0;
	LPTCheckTimer4 = 0;
	HPTMakeReportFlag = FALSE;
	LPTMakeReportFlag = FALSE;
	U2Set = 8*(*CF_pnUp)/100; //负序电压8V

    for(int i=0;i<3;i++)
	{
	  PickCheckNext[i] =0;
    }
	
	PickUpRW = RW_PICKUP;          //保护启动继电器字，不区分模值启动突变量启动
	TripFailFlag = FALSE;          //跳闸失败标志
	FaultOnFlag = FALSE;           //进入故障循环处理标志
	HaveMakeStartRptFlag = FALSE;  //作启动报告标志
	m_bFaultLKFlag = FALSE;        //突变量启动，锁定故障起始时刻标志	
	FaultCheckOutFlag = FALSE;     //检出故障标志
	m_b52BFFLAG = FALSE;
	m_bHaveMakeTripFailRpt = FALSE;
	
	m_b49IHaveMakeActRpt = FALSE;			
	m_b49IHaveMakeRSTRpt = FALSE;
	m_b49IHaveMakeWarnRpt = FALSE;
	
    dwChanDataMask = ((0x01<<CHANNEL_6533_UA)|(0x01<<CHANNEL_6533_UB)|(0x01<<CHANNEL_6533_UC)|\
						(0x01<<CHANNEL_6533_U0h)|(0x01<<CHANNEL_6533_Ua)|(0x01<<CHANNEL_6533_Ub)|\
						(0x01<<CHANNEL_6533_Uc)|(0x01<<CHANNEL_6533_U0l)|(0x01<<CHANNEL_6533_IA)|\
						(0x01<<CHANNEL_6533_IB)|(0x01<<CHANNEL_6533_IC)|(0x01<<CHANNEL_6533_Ia)|\
						(0x01<<CHANNEL_6533_Ib)|(0x01<<CHANNEL_6533_Ic)|(0x01<<CHANNEL_6533_I0)|\
						(0x01<<CHANNEL_6533_Ijx));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	dwComposeMask = ((0x01<<CHANNEL_6533_IDA)|(0x01<<CHANNEL_6533_IRA)|(0x01<<CHANNEL_6533_IDB)|\
					(0x01<<CHANNEL_6533_IRB)|(0x01<<CHANNEL_6533_IDC)|(0x01<<CHANNEL_6533_IRC));
	
	//初始化读取保护数据临时缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);	
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*11);
	dwComposeDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);
	dwLineUDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);		
	dwPDDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM);
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*6);
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
	memset(dwPickDataBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*11));
	
	ASSERT(dwComposeDataBuf != NULL);
	if(dwComposeDataBuf == NULL)
	{
		LogError("CRelayTask,dwComposeDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));
	
	ASSERT(dwLineUDataBuf != NULL);
	if(dwLineUDataBuf == NULL)
	{
		LogError("CRelayTask,dwLineUDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwLineUDataBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*6));
	
	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*6);
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

	RelayTaskInitOK = TRUE;
	m_bLogicChanged = FALSE;
	m_dwWaitLogicCounter = 0;
	g_MaintSW = SW_pbyMAINT;
	SetTimer(1,5);
	m_PTCheckTimer = 20*RELAY_SAMP_INTERVAL/POINT_PER_PERIOD;
}
/************************************************************************************************
*功能: 初始化与保护直接相关的定值
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void CRelayTask::InitSet(void)
{
	
	dwCaluInterval = Rel49IA.l49OT*2;
	if((*CFG_pby49I == CFG_STATUS_SET)&&(*SW_pby49I == SW_STATUS_SET))
		g_bRel49Flag = TRUE;
	else
		g_bRel49Flag = FALSE;
}

void  CRelayTask::InitRealy(void)
{
	 InitRel87U_A();		//A差动速断
	 InitRel87U_B();		//B差动速断
	 InitRel87U_C();		//C差动速断 	
	 InitRel87R_A();		//A比率差动
	 InitRel87R_B();		//B比率差动
	 InitRel87R_C();		//C比率差动
	 InitRel50I_H1();		//高压侧过流1段；
	 InitRel50I_H2();		//高压侧过流2段；
	 InitRel50I_H3();		//高压侧过流3段
	 InitRel50I_L1();		//低压侧过流1段
	 InitRel50I_L2();		//低压侧过流2段
	 InitRel50I_SH();		//母线充电保护
	 InitRel50I_N1();		//零序过流1段
	 InitRel50I_N2();		//零序过流2段
	 InitRel59U_NH();		//高压侧零序过压
	 InitRel59U_NL();		//低压侧零序过压
	 InitRel50OL1();		//过负荷1段
	 InitRel50OL2();		//过负荷2段
	 InitRel50OL3();		//过负荷3段
	 InitRel50IJX();		//间隙过流保护
	 InitRel27U();			//失压保护
	 InitRel49I();			//反时限热过负荷保护
	 InitR87IAlarm();		//差流越限告警
	 InitRel50OLA();		//过负荷告警 2014年8月27日 11:40:52 yanzh 增加
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
void CRelayTask::OnSampleEnd(DWORD arg)
{	
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}

	static WORD wInvalidCheckCnt;
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;

	
	//首先进行开出自检,每次保护任务检一路开出
	if(!m_bLogicChanged)
		BoBreakCheck();

	//初始化采样通道数据掩码
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	CaluUI();
	if(*CFG_pbyHAB == CFG_STATUS_CLR)
	{
		
		ComposeData();		          
		CaluMaxMin();
		CaluR87();
		Calu49I();
		Calu47U();
		R87IAlarm(&R87Alarm);
		PTCheck();	
	}
	else
	{
		if(IA.Mod>=IB.Mod)
			IHmax = IA;
		else
			IHmax = IB;
		if(UA.Mod>=UB.Mod)
			U_27U = UB;
		else
			U_27U = UA;
		InitFalutHAB();
	}
	
	FalutProcess();								//故障循环处理
	GeneralOverValueRelay(&Rel50OL_A);  //增加过负荷告警，2014年8月27日 13:46:27 yanzh
	GeneralUnderValRelay(&Rel27U);					//低电压保护子程序	
	Process52BF();
	
	if(g_bRel49TimeOut == TRUE)
	{
		ThermalOverLoad();
		g_bRel49TimeOut = FALSE;
	}
	//逻辑方程处理
	LogicProcess();
	
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

}
/************************************************************************************************
*功能: 向数字通道中写入录波数据
*参数: 无
*返回: 无
*备注:
************************************************************************************************/
void CRelayTask::WriteDCChannel()
{
	LONG i;
	RecordRelayWord(DCMacroArray,DCCaluResult);	
	for(i =0;i<RELAY_SAMP_INTERVAL;i++)
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
void CRelayTask::ComposeData(void)
{	
	LONG tempA1,tempA2;
	LONG tempB1,tempB2;
	LONG tempC1,tempC2;

	for(LONG j=0;j<READ_POINT_NUM;j++)
	{
		dwLineUDataBuf[j] = dwSampBuf[j]-dwSampBuf[READ_POINT_NUM+j]; //UAB=UA-UB
		dwLineUDataBuf[READ_POINT_NUM+j] = dwSampBuf[READ_POINT_NUM+j]-dwSampBuf[READ_POINT_NUM*2+j]; //UBC=UB-UC
		dwLineUDataBuf[READ_POINT_NUM*2+j] = dwSampBuf[READ_POINT_NUM*2+j]-dwSampBuf[j];//UCA=UC-UA

		dwLineUDataBuf[READ_POINT_NUM*3+j] = dwSampBuf[READ_POINT_NUM*4+j]-dwSampBuf[READ_POINT_NUM*5+j]; //Uab=Ua-Ub
		dwLineUDataBuf[READ_POINT_NUM*4+j] = dwSampBuf[READ_POINT_NUM*5+j]-dwSampBuf[READ_POINT_NUM*6+j]; //Ubc=Ub-Uc
		dwLineUDataBuf[READ_POINT_NUM*5+j] = dwSampBuf[READ_POINT_NUM*6+j]-dwSampBuf[READ_POINT_NUM*4+j]; //Uca=Uc-Ua		
	}
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		//高压侧
		if(*CFG_pbyHCT1CLOCK == CFG_STATUS_SET) // 高压侧1点校正投入
		{
			tempA1 = (dwSampBuf[READ_POINT_NUM*8+i]-dwSampBuf[READ_POINT_NUM*10+i])*100;//A-C
			tempB1 = (dwSampBuf[READ_POINT_NUM*9+i]-dwSampBuf[READ_POINT_NUM*8+i])*100;//B-A
			tempC1 = (dwSampBuf[READ_POINT_NUM*10+i]-dwSampBuf[READ_POINT_NUM*9+i])*100;//C-B
		}
		else if(*CFG_pbyHCT11CLOCK == CFG_STATUS_SET) //高压侧11点校正投入
		{
			tempA1 = (dwSampBuf[READ_POINT_NUM*8+i]-dwSampBuf[READ_POINT_NUM*9+i])*100;//A-B
			tempB1 = (dwSampBuf[READ_POINT_NUM*9+i]-dwSampBuf[READ_POINT_NUM*10+i])*100;//B-C
			tempC1 = (dwSampBuf[READ_POINT_NUM*10+i]-dwSampBuf[READ_POINT_NUM*8+i])*100;//C-A
		}
		else
		{
			tempA1 = (dwSampBuf[READ_POINT_NUM*8+i])*100;//A
			tempB1 = (dwSampBuf[READ_POINT_NUM*9+i])*100;//B
			tempC1 = (dwSampBuf[READ_POINT_NUM*10+i])*100;//C
		}
		//低压侧  11-a,13-b,15-c
		if(*CFG_pbyLCT1CLOCK == CFG_STATUS_SET) //低压侧1点校正投入
		{
			tempA2 = dwSampBuf[READ_POINT_NUM*11+i]-dwSampBuf[READ_POINT_NUM*15+i];//A-C
			tempB2 = dwSampBuf[READ_POINT_NUM*13+i]-dwSampBuf[READ_POINT_NUM*11+i];//B-A
			tempC2 = dwSampBuf[READ_POINT_NUM*15+i]-dwSampBuf[READ_POINT_NUM*13+i];//C-B
		}
		else if(*CFG_pbyLCT11CLOCK == CFG_STATUS_SET) //低压侧11点校正投入
		{
			tempA2 = dwSampBuf[READ_POINT_NUM*11+i]-dwSampBuf[READ_POINT_NUM*13+i];//A-B
			tempB2 = dwSampBuf[READ_POINT_NUM*13+i]-dwSampBuf[READ_POINT_NUM*15+i];//B-C
			tempC2 = dwSampBuf[READ_POINT_NUM*15+i]-dwSampBuf[READ_POINT_NUM*11+i];//C-A
		}
		else
		{
			tempA2 = dwSampBuf[READ_POINT_NUM*11+i];//A
			tempB2 = dwSampBuf[READ_POINT_NUM*13+i];//B
			tempC2 = dwSampBuf[READ_POINT_NUM*15+i];//C
		}
		dwComposeDataBuf[i] = (tempA1 +(*RS_pnkb)*tempA2)/100;
		dwComposeDataBuf[READ_POINT_NUM+i] = (tempA1 -(*RS_pnkb)*tempA2)/200;
		dwComposeDataBuf[READ_POINT_NUM*2+i] = (tempB1 +(*RS_pnkb)*tempB2)/100;
		dwComposeDataBuf[READ_POINT_NUM*3+i] = (tempB1 -(*RS_pnkb)*tempB2)/200;
		dwComposeDataBuf[READ_POINT_NUM*4+i] = (tempC1 +(*RS_pnkb)*tempC2)/100;
		dwComposeDataBuf[READ_POINT_NUM*5+i] = (tempC1 -(*RS_pnkb)*tempC2)/200;		
	}

	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{		
		dwRecBuf[i] = dwComposeDataBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwComposeDataBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*2+i] = dwComposeDataBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*3+i] = dwComposeDataBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*4+i] = dwComposeDataBuf[READ_POINT_NUM*5-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*5+i] = dwComposeDataBuf[READ_POINT_NUM*6-RELAY_SAMP_INTERVAL+i];
		WriteComposeData(dwRecBuf, dwComposeMask);
	}
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
			phase = PHASE_A;
			return ele1;
		}
		else
		{
			phase = PHASE_C;
			return ele3;
		}
	}
	else
	{
		if(ele2.Mod>=ele3.Mod)
		{
			phase = PHASE_B;
			return ele2;
		}
		else
		{
			phase = PHASE_C;
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
// 名称: 计算电量	                                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::CaluUI(void)
{
	LONG angle;
	//计算UA模值并写保护测量、置UA有压继电器字、无压继电器字
	CaluBaseFourier_S15(dwSampBuf,&UA);
	CaluModValue(&UA);
	CaluAngle(&UA,AngleTab);
	*RM_pnUHA= UA.Mod;
	*RM_pnAUHA = 0;
	DB_LogWriteRM(RM_UHA,*RM_pnUHA);
	DB_LogWriteRM(RM_AUHA,*RM_pnAUHA);	
	if(UA.Mod>=*AS_pn59UAV)		//置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59UHA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UHA, RELAY_OFF);
	}
	if(UA.Mod>=*AS_pn27UAV)
	{
		WriteRelayWord(RW_27UHA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UHA, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UB);
	CaluModValue(&UB);
	CaluAngle(&UB,AngleTab);
	*RM_pnUHB= UB.Mod;
	angle = UB.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAUHB = angle;
	else
		*RM_pnAUHB = angle+36000;
	DB_LogWriteRM(RM_UHB,*RM_pnUHB);
	DB_LogWriteRM(RM_AUHB,*RM_pnAUHB);	
	if(UB.Mod>=*AS_pn59UBV)		//置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59UHB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UHB, RELAY_OFF);
	}
	if(UB.Mod>=*AS_pn27UBV)
	{
		WriteRelayWord(RW_27UHB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UHB, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UC);
	CaluModValue(&UC);
	CaluAngle(&UC,AngleTab);
	*RM_pnUHC= UC.Mod;
	angle = UC.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAUHC = angle;
	else
		*RM_pnAUHC = angle+36000;
	DB_LogWriteRM(RM_UHC,*RM_pnUHC);
	DB_LogWriteRM(RM_AUHC,*RM_pnAUHC);	
	if(UC.Mod>=*AS_pn59UCV)		//置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59UHC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UHC, RELAY_OFF);
	}
	if(UC.Mod>=*AS_pn27UCV)
	{
		WriteRelayWord(RW_27UHC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UHC, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U0h);
	CaluModValue(&U0h);
	CaluAngle(&U0h,AngleTab);
	*RM_pnU0H= U0h.Mod;
	DB_LogWriteRM(RM_U0H,*RM_pnU0H);
	if(U0h.Mod>=*AS_pn59U0hV) 	//置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59U0H, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U0H, RELAY_OFF);
	}
	if(U0h.Mod>=*AS_pn27U0hV)
	{
		WriteRelayWord(RW_27U0H, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U0H, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&Ua);
	CaluModValue(&Ua);
	CaluAngle(&Ua,AngleTab);
	*RM_pnULa= Ua.Mod;
	angle = Ua.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAULa = angle;
	else
		*RM_pnAULa = angle+36000;
	DB_LogWriteRM(RM_ULA,*RM_pnULa);
	DB_LogWriteRM(RM_AULA,*RM_pnAULa);
	if(Ua.Mod>=*AS_pn59UaV)	//置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59ULA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59ULA, RELAY_OFF);
	}
	if(Ua.Mod>=*AS_pn27UaV)
	{
		WriteRelayWord(RW_27ULA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27ULA, RELAY_ON);
	}

	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&Ub);
	CaluModValue(&Ub);
	CaluAngle(&Ub,AngleTab);
	*RM_pnULb= Ub.Mod;
	angle = Ub.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAULb = angle;
	else
		*RM_pnAULb = angle+36000;
	DB_LogWriteRM(RM_ULB,*RM_pnULb);
	DB_LogWriteRM(RM_AULB,*RM_pnAULb);
	if(Ub.Mod>=*AS_pn59UbV) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59ULB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59ULB, RELAY_OFF);
	}
	if(Ub.Mod>=*AS_pn27UbV)
	{
		WriteRelayWord(RW_27ULB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27ULB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&Uc);
	CaluModValue(&Uc);
	CaluAngle(&Uc,AngleTab);
	*RM_pnULc= Uc.Mod;
	angle = Uc.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAULc = angle;
	else
		*RM_pnAULc = angle+36000;
	DB_LogWriteRM(RM_ULC,*RM_pnULc);
	DB_LogWriteRM(RM_AULC,*RM_pnAULc);
	if(Uc.Mod>=*AS_pn59UcV) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59ULC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59ULC, RELAY_OFF);
	}
	if(Uc.Mod>=*AS_pn27UcV)
	{
		WriteRelayWord(RW_27ULC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27ULC, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&U0l);
	CaluModValue(&U0l);
	*RM_pnU0L= U0l.Mod;
	DB_LogWriteRM(RM_U0L,*RM_pnU0L);
	if(U0l.Mod>=*AS_pn59U0lV) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_59U0L, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U0L, RELAY_OFF);
	}
	if(U0l.Mod>=*AS_pn27U0lV)
	{
		WriteRelayWord(RW_27U0L, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U0L, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IA);
	CaluModValue(&IA);
	CaluAngle(&IA,AngleTab);
	*RM_pnIHA= IA.Mod;
	angle = IA.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAIHA = angle;
	else
		*RM_pnAIHA = angle+36000;
	DB_LogWriteRM(RM_IHA,*RM_pnIHA);
	DB_LogWriteRM(RM_AIHA,*RM_pnAIHA);
	if(IA.Mod>=*AS_pn50IAC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50IHA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IHA, RELAY_OFF);
	}
	if(IA.Mod>=*AS_pn37IAC)
	{
		WriteRelayWord(RW_37IHA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IHA, RELAY_ON);
		
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&IB);
	CaluModValue(&IB);
	CaluAngle(&IB,AngleTab);
	*RM_pnIHB= IB.Mod;
	angle = IB.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAIHB = angle;
	else
		*RM_pnAIHB = angle+36000;
	DB_LogWriteRM(RM_IHB,*RM_pnIHB);
	DB_LogWriteRM(RM_AIHB,*RM_pnAIHB);
	if(IB.Mod>=*AS_pn50IBC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50IHB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IHB, RELAY_OFF);
	}
	if(IB.Mod>=*AS_pn37IBC)
	{
		WriteRelayWord(RW_37IHB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IHB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&IC);
	CaluModValue(&IC);
	CaluAngle(&IC,AngleTab);
	*RM_pnIHC= IC.Mod;
	angle = IC.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAIHC = angle;
	else
		*RM_pnAIHC = angle+36000;
	DB_LogWriteRM(RM_IHC,*RM_pnIHC);
	DB_LogWriteRM(RM_AIHC,*RM_pnAIHC);
	if(IC.Mod>=*AS_pn50ICC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50IHC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IHC, RELAY_OFF);
	}
	if(IC.Mod>=*AS_pn37ICC)
	{
		WriteRelayWord(RW_37IHC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IHC, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&Ia);
	CaluModValue(&Ia);
	CaluAngle(&Ia,AngleTab);
	*RM_pnILa= Ia.Mod;
	angle = Ia.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAILa = angle;
	else
		*RM_pnAILa = angle+36000;
	DB_LogWriteRM(RM_ILA,*RM_pnILa);
	DB_LogWriteRM(RM_AILA,*RM_pnAILa);
	if(Ia.Mod>=*AS_pn50IaC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50ILA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50ILA, RELAY_OFF);
	}
	if(Ia.Mod>=*AS_pn37IaC)
	{
		WriteRelayWord(RW_37ILA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37ILA, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&Ib);
	CaluModValue(&Ib);
	CaluAngle(&Ib,AngleTab);
	*RM_pnILb= Ib.Mod;
	angle = Ib.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAILb = angle;
	else
		*RM_pnAILb = angle+36000;
	DB_LogWriteRM(RM_ILB,*RM_pnILb);
	DB_LogWriteRM(RM_AILB,*RM_pnAILb);
	if(Ib.Mod>=*AS_pn50IbC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50ILB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50ILB, RELAY_OFF);
	}
	if(Ib.Mod>=*AS_pn37IbC)
	{
		WriteRelayWord(RW_37ILB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37ILB, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*15,&Ic);
	CaluModValue(&Ic);
	CaluAngle(&Ic,AngleTab);
	*RM_pnILc= Ic.Mod;
	angle = Ic.Angle - UA.Angle;
	if(angle>0)
		*RM_pnAILc = angle;
	else
		*RM_pnAILc = angle+36000;
	DB_LogWriteRM(RM_ILC,*RM_pnILc);
	DB_LogWriteRM(RM_AILC,*RM_pnAILc);
	if(Ic.Mod>=*AS_pn50IcC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50ILC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50ILC, RELAY_OFF);
	}
	if(Ic.Mod>=*AS_pn37IcC)
	{
		WriteRelayWord(RW_37ILC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37ILC, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&I0);
	CaluModValue(&I0);
	CaluAngle(&I0,AngleTab);
	*RM_pnI0= I0.Mod;
	DB_LogWriteRM(RM_I0,*RM_pnI0);
	if(I0.Mod>=*AS_pn50I0C) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50I0, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I0, RELAY_OFF);
	}
	if(I0.Mod>=*AS_pn37I0C)
	{
		WriteRelayWord(RW_37I0, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I0, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*14,&Ijx);
	CaluModValue(&Ijx);
	CaluAngle(&Ijx,AngleTab);
	*RM_pnIjx= Ijx.Mod;
	DB_LogWriteRM(RM_IJX,*RM_pnIjx);
	if(Ijx.Mod>=*AS_pn50IjxC) //置UA有压继电器字、无压继电器字
	{
		WriteRelayWord(RW_50IJX, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50IJX, RELAY_OFF);
	}
	if(Ijx.Mod>=*AS_pn37IjxC)
	{
		WriteRelayWord(RW_37IJX, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37IJX, RELAY_ON);
	}
		
}
void CRelayTask::CaluMaxMin(void)
{
	//计算线电压，用于功率方向的90°接线判断
	CaluBaseFourier_S15(dwLineUDataBuf,&UAB);
	CaluModValue(&UAB);	
	CaluAngle(&UAB,AngleTab);	
	*RM_pnUAB= UAB.Mod;
	DB_LogWriteRM(RM_UAB,*RM_pnUAB);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM,&UBC);
	CaluModValue(&UBC);	
	CaluAngle(&UBC,AngleTab);	
	*RM_pnUBC= UBC.Mod;
	DB_LogWriteRM(RM_UBC,*RM_pnUBC);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*2,&UCA);
	CaluModValue(&UCA);	
	CaluAngle(&UCA,AngleTab);
	*RM_pnUCA= UCA.Mod;
	DB_LogWriteRM(RM_UCA,*RM_pnUCA);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*3,&Uab);
	CaluModValue(&Uab);		
	CaluAngle(&Uab,AngleTab);
	*RM_pnULab= Uab.Mod;
	DB_LogWriteRM(RM_ULAB,*RM_pnULab);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*4,&Ubc);
	CaluModValue(&Ubc);	
	CaluAngle(&Ubc,AngleTab);
	*RM_pnULbc= Ubc.Mod;
	DB_LogWriteRM(RM_ULBC,*RM_pnULbc);
	
	CaluBaseFourier_S15(dwLineUDataBuf+READ_POINT_NUM*5,&Uca);
	CaluModValue(&Uca);
	CaluAngle(&Uca,AngleTab);
	*RM_pnULca= Uca.Mod;
	DB_LogWriteRM(RM_ULCA,*RM_pnULca);

	IHmax = ElecMax(IA,IB,IC,wHIPhase);
	ILmax = ElecMax(Ia,Ib,Ic,wLIPhase);
	UHmax = ElecMax(UA,UB,UC);
	UHmin = ElecMin(UAB,UBC,UCA);
	ULmax = ElecMax(Ua,Ub,Uc);
	ULmin = ElecMin(Uab,Ubc,Uca);
	if(UHmax.Mod>=ULmax.Mod)
		U_27U = UHmax;
	else
		U_27U = ULmax;

	if(wHIPhase == PHASE_A)
	{
		Rel50I_H1.PForwardRelayWord = RW_PFHA; 
		Rel50I_H1.PRewardRelayWord = RW_PRHA;			
		Rel50I_H2.PForwardRelayWord = RW_PFHA; 
		Rel50I_H2.PRewardRelayWord = RW_PRHA;
		Rel50I_H3.PForwardRelayWord = RW_PFHA; 
		Rel50I_H3.PRewardRelayWord = RW_PRHA;
		
//		HPFRW = RW_PFHA;
//		HPRRW = RW_PRHA;
	}
	else if(wHIPhase == PHASE_B)
	{		
		Rel50I_H1.PForwardRelayWord = RW_PFHB; 
		Rel50I_H1.PRewardRelayWord = RW_PRHB;			
		Rel50I_H2.PForwardRelayWord = RW_PFHB; 
		Rel50I_H2.PRewardRelayWord = RW_PRHB;
		Rel50I_H3.PForwardRelayWord = RW_PFHB; 
		Rel50I_H3.PRewardRelayWord = RW_PRHB;
//		HPFRW = RW_PFHB;
//		HPRRW = RW_PRHB;
	}
	else if(wHIPhase == PHASE_C)
	{		
		Rel50I_H1.PForwardRelayWord = RW_PFHC; 
		Rel50I_H1.PRewardRelayWord = RW_PRHC;			
		Rel50I_H2.PForwardRelayWord = RW_PFHC; 
		Rel50I_H2.PRewardRelayWord = RW_PRHC;
		Rel50I_H3.PForwardRelayWord = RW_PFHC; 
		Rel50I_H3.PRewardRelayWord = RW_PRHC;
//		HPFRW = RW_PFHC;
//		HPRRW = RW_PRHC;
	}
	
	if(wLIPhase == PHASE_A)
	{
		Rel50I_L1.PForwardRelayWord = RW_PFLA;
		Rel50I_L1.PRewardRelayWord= RW_PRLA;		
		Rel50I_L2.PForwardRelayWord = RW_PFLA;
		Rel50I_L2.PRewardRelayWord= RW_PRLA;
//		LPFRW = RW_PFLA;
//		LPRRW = RW_PRLA;
	}
	else if(wLIPhase == PHASE_B)
	{		
		Rel50I_L1.PForwardRelayWord = RW_PFLB;
		Rel50I_L1.PRewardRelayWord= RW_PRLB;		
		Rel50I_L2.PForwardRelayWord = RW_PFLB;
		Rel50I_L2.PRewardRelayWord= RW_PRLB;
//		LPFRW = RW_PFLB;
//		LPRRW = RW_PRLB;
	}
	else if(wLIPhase == PHASE_C)
	{		
		Rel50I_L1.PForwardRelayWord = RW_PFLC;
		Rel50I_L1.PRewardRelayWord= RW_PRLC;		
		Rel50I_L2.PForwardRelayWord = RW_PFLC;
		Rel50I_L2.PRewardRelayWord= RW_PRLC;
//		LPFRW = RW_PFLC;
//		LPRRW = RW_PRLC;
	}
}

//计算差动电流，制动电流
void CRelayTask::CaluR87(void)
{
	CaluBaseFourier_S15(dwComposeDataBuf,&Ida);
	CaluModValue(&Ida);		
	*RM_pnIDA= Ida.Mod;
	DB_LogWriteRM(RM_IDA,*RM_pnIDA);
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM,&Ira);
	CaluModValue(&Ira);	
	*RM_pnIRA= Ira.Mod;
	DB_LogWriteRM(RM_IRA,*RM_pnIRA);

	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*2,&Idb);
	CaluModValue(&Idb); 
	*RM_pnIDB= Idb.Mod;
	DB_LogWriteRM(RM_IDB,*RM_pnIDB);
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*3,&Irb);
	CaluModValue(&Irb);
	*RM_pnIRB= Irb.Mod;
	DB_LogWriteRM(RM_IRB,*RM_pnIRB);
	
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*4,&Idc);
	CaluModValue(&Idc); 
	*RM_pnIDC= Idc.Mod;
	DB_LogWriteRM(RM_IDC,*RM_pnIDC);
	CaluBaseFourier_S15(dwComposeDataBuf+READ_POINT_NUM*5,&Irc);
	CaluModValue(&Irc);
	*RM_pnIRC= Irc.Mod;
	DB_LogWriteRM(RM_IRC,*RM_pnIRC);

	CaluSecondFourier(dwComposeDataBuf,&Ida2);
	CaluModValue(&Ida2);
	*RM_pnIDA2= Ida2.Mod;
	DB_LogWriteRM(RM_IDA2,*RM_pnIDA2);
	CaluThirdFourier(dwComposeDataBuf,&Ida3);
	CaluModValue(&Ida3);
	*RM_pnIDA3= Ida3.Mod;
	DB_LogWriteRM(RM_IDA3,*RM_pnIDA3);
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM,&Ira2);
	CaluModValue(&Ira2);	
	*RM_pnIRA2= Ira2.Mod;
	DB_LogWriteRM(RM_IRA2,*RM_pnIRA2);
	
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*2,&Idb2);
	CaluModValue(&Idb2);	
	*RM_pnIDB2= Idb2.Mod;
	DB_LogWriteRM(RM_IDB2,*RM_pnIDB2);
	CaluThirdFourier(dwComposeDataBuf+READ_POINT_NUM*2,&Idb3);
	CaluModValue(&Idb3);	
	*RM_pnIDB3= Idb3.Mod;
	DB_LogWriteRM(RM_IDB3,*RM_pnIDB3);
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*3,&Irb2);
	CaluModValue(&Irb2);
	*RM_pnIRB2= Irb2.Mod;
	DB_LogWriteRM(RM_IRB2,*RM_pnIRB2);

	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*4,&Idc2);
	CaluModValue(&Idc2);	
	*RM_pnIDC2= Idc2.Mod;
	DB_LogWriteRM(RM_IDC2,*RM_pnIDC2);
	CaluThirdFourier(dwComposeDataBuf+READ_POINT_NUM*4,&Idc3);
	CaluModValue(&Idc3);	
	*RM_pnIDC3= Idc3.Mod;
	DB_LogWriteRM(RM_IDC3,*RM_pnIDC3);
	CaluSecondFourier(dwComposeDataBuf+READ_POINT_NUM*5,&Irc2);
	CaluModValue(&Irc2);	
	*RM_pnIRC2= Irc2.Mod;
	DB_LogWriteRM(RM_IRC2,*RM_pnIRC2);

	//未计算差流五次谐波值，将保护测量值中五次谐波置零  2013-08-29
	*RM_pnIDA5= 0;
	DB_LogWriteRM(RM_IDA5,*RM_pnIDA5);
	*RM_pnIDB5= 0;
	DB_LogWriteRM(RM_IDB5,*RM_pnIDB5);
	*RM_pnIDC5= 0;
	DB_LogWriteRM(RM_IDC5,*RM_pnIDC5);
	
}
void CRelayTask::Calu49I()
{
	if((*CFG_pby49I== CFG_STATUS_SET)&&(*SW_pby49I== SW_STATUS_SET))
	{
		Filter(dwSampBuf+READ_POINT_NUM*8);
		Filter(dwSampBuf+READ_POINT_NUM*9);
		Filter(dwSampBuf+READ_POINT_NUM*10);
		
		CaluMSMod(dwSampBuf+READ_POINT_NUM*8, dw49IA);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*9, dw49IB);
		CaluMSMod(dwSampBuf+READ_POINT_NUM*10, dw49IC);
		
		Rel49IA.pCurrent = (dw49IA+Rel49IA.PreCurrent)/2;
		Rel49IA.PreCurrent = Rel49IA.pCurrent;
		Rel49IB.pCurrent = (dw49IB+Rel49IB.PreCurrent)/2;
		Rel49IB.PreCurrent = Rel49IB.pCurrent;
		Rel49IC.pCurrent = (dw49IC+Rel49IC.PreCurrent)/2;
		Rel49IC.PreCurrent = Rel49IC.pCurrent;
	}
}
TRelElecValPar CRelayTask::CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc) //计算负序电压
{
	TRelElecValPar u2;
	u2.Real = ua->Real/3-ub->Real/6+866*ub->Imag/3000-uc->Real/6-866*uc->Imag/3000;
	u2.Imag = ua->Imag/3-ub->Imag/6-866*ub->Real/3000-uc->Imag/6+866*uc->Real/3000;
	CaluModValue(&u2);
	return u2;
}

void CRelayTask::Calu47U(void)
{
	U2h = CaluU2(&UA,&UB,&UC);
	U2l = CaluU2(&Ua,&Ub,&Uc);

	
	*RM_pnU2H= U2h.Mod;
	DB_LogWriteRM(RM_U2H,*RM_pnU2H);
	*RM_pnU2L= U2l.Mod;
	DB_LogWriteRM(RM_U2L,*RM_pnU2L);
	
	if((UHmin.Mod<*RS_pn47U_27U1)||(U2h.Mod>*RS_pn47U_59U2))
		WriteRelayWord(RW_47UH,RELAY_OFF);
	else
		WriteRelayWord(RW_47UH,RELAY_ON);
	
	if((ULmin.Mod<*RS_pn47U_27U1)||(U2l.Mod>*RS_pn47U_59U2))
		WriteRelayWord(RW_47UL,RELAY_OFF);
	else
		WriteRelayWord(RW_47UL,RELAY_ON);
		
}

//高压侧两相投入时，将应失效保护的故障电量置零
void CRelayTask::InitFalutHAB()		
{
	TRelElecValPar ZeroHAB;
	ZeroHAB.Real = 0;
	ZeroHAB.Imag = 0;
	ZeroHAB.Mod = 0;
	
	U2h=U2l=UAB=UBC=UCA=Uab=Ubc=Uca=ZeroHAB;		//高压侧负序电压
	Ida=Idb=Idc=Ira=Irb=Irc=Ida2=Ida3=Idb2=Idb3=Idc2=Idc3=Ira2=Irb2=Irc2=ZeroHAB;		//A相差流
	ILmax = ZeroHAB; //低压侧最大相电流
	Rel49IA.pCurrent = 0;
	Rel49IB.pCurrent = 0;
	Rel49IC.pCurrent = 0;
	if(ReadRelayWord(RW_47UH))
		WriteRelayWord(RW_47UH,RELAY_OFF);//清复压闭锁标志
	if(ReadRelayWord(RW_47UL))
		WriteRelayWord(RW_47UL,RELAY_OFF);

	*RM_pnUAB=0;
	DB_LogWriteRM(RM_UAB,*RM_pnUAB);
	*RM_pnUBC=0;
	DB_LogWriteRM(RM_UBC,*RM_pnUBC);
	*RM_pnUCA=0;
	DB_LogWriteRM(RM_UCA,*RM_pnUCA);
	*RM_pnULab=0;
	DB_LogWriteRM(RM_ULAB,*RM_pnULab);
	*RM_pnULbc=0;
	DB_LogWriteRM(RM_ULBC,*RM_pnULbc);
	*RM_pnULca=0;
	DB_LogWriteRM(RM_ULCA,*RM_pnULca);
	*RM_pnU2H= 0;
	DB_LogWriteRM(RM_U2H,*RM_pnU2H);
	*RM_pnU2L= 0;
	DB_LogWriteRM(RM_U2L,*RM_pnU2L);
	*RM_pnIDA= 0;
	DB_LogWriteRM(RM_IDA,*RM_pnIDA);
	*RM_pnIDB= 0;
	DB_LogWriteRM(RM_IDB,*RM_pnIDB);
	*RM_pnIDC= 0;
	DB_LogWriteRM(RM_IDC,*RM_pnIDC);
	*RM_pnIRA= 0;
	DB_LogWriteRM(RM_IRA,*RM_pnIRA);
	*RM_pnIRB= 0;
	DB_LogWriteRM(RM_IRB,*RM_pnIRB);
	*RM_pnIRC= 0;
	DB_LogWriteRM(RM_IRC,*RM_pnIRC);
	*RM_pnIDA2= 0;
	DB_LogWriteRM(RM_IDA2,*RM_pnIDA2);
	*RM_pnIDA3= 0;
	DB_LogWriteRM(RM_IDA3,*RM_pnIDA3);
	*RM_pnIDA5= 0;
	DB_LogWriteRM(RM_IDA5,*RM_pnIDA5);
	*RM_pnIDB2= 0;
	DB_LogWriteRM(RM_IDB2,*RM_pnIDB2);
	*RM_pnIDB3= 0;
	DB_LogWriteRM(RM_IDB3,*RM_pnIDB3);
	*RM_pnIDB5= 0;
	DB_LogWriteRM(RM_IDB5,*RM_pnIDB5);
	*RM_pnIDC2= 0;
	DB_LogWriteRM(RM_IDC2,*RM_pnIDC2);
	*RM_pnIDC3= 0;
	DB_LogWriteRM(RM_IDC3,*RM_pnIDC3);
	*RM_pnIDC5= 0;
	DB_LogWriteRM(RM_IDC5,*RM_pnIDC5);
	*RM_pnIRA2= 0;
	DB_LogWriteRM(RM_IRA2,*RM_pnIRA2);
	*RM_pnIRB2= 0;
	DB_LogWriteRM(RM_IRB2,*RM_pnIRB2);
	*RM_pnIRC2= 0;
	DB_LogWriteRM(RM_IRC2,*RM_pnIRC2);
}
//void CRelayTask::InitRepSerialNum()
//{
//	
//}
//A相差动速断保护
void CRelayTask::InitRel87U_A()
{
	Rel87U_A.pElecSet				= RS_pn87UC;          			//过量保护电量定值
	Rel87U_A.pTimeSet				= &TimeSet;				 		//过量保护时间定值
	Rel87U_A.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数
	Rel87U_A.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	Rel87U_A.pAccActTimeSet			= NULL;							//过量保护后加速动作时间定值
	Rel87U_A.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel87U_A.pRelCFG				= CFG_pby87U;					//过量保护功能控制字--1:投入，2:退出 
	Rel87U_A.pRelSW					= SW_pby87U;         			//过量保护软压板------1:投入，2:退出
	Rel87U_A.pRelHLockCFG			= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel87U_A.pRelHSuprCFG			= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel87U_A.pRelPDCFG				= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel87U_A.pRelVolLockCFG			= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel87U_A.pRelFDCFG				= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel87U_A.pRelAccCFG				= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel87U_A.pRelFLCFG				= &RelCFGCLR;					//故障测距投入
	Rel87U_A.pSysTimer				= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel87U_A.pRelElec 				= &Ida;							//当前电量指针		
	Rel87U_A.pRelHarm3				= NULL;							//当前三次谐波指针	
	Rel87U_A.pRelHarm5				= NULL;							//当前五次谐波指针	
	Rel87U_A.pRelVolt				= NULL;							//当前用来闭锁的电压的指针

	Rel87U_A.StartRelayWord			= RW_PI87UA;					//过量保护入段继电器字
	Rel87U_A.ActRelayWord			= RW_TR87UA;					//过量保护动作继电器字
	Rel87U_A.AccRelayWord			= FALSE1;						//后加速允许继电器字
	Rel87U_A.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	Rel87U_A.PForwardRelayWord		= FALSE1;						//功率正方向继电器字
	Rel87U_A.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	Rel87U_A.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	Rel87U_A.LockRelayWord			= RW_LKPI87UA;					//入段闭锁继电器字
	Rel87U_A.RSTRelayWord			= RW_LKRST87UA;				//返回闭锁继电器字
	Rel87U_A.LoopCtrlRelayWord		= RW_LPC87UA;					//保护循环控制继电器字
	
	Rel87U_A.wReportNum				= R_87U;						//过量保护报告序号
	Rel87U_A.wAccReportNum			= 0xFFFF;						//加速动作报告序号

	Rel87U_A.bMakeActRpt			= TRUE;
	Rel87U_A.dwK35					= NULL;
	Rel87U_A.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel87U_A.wPickUpReportNum		= RPT_NULL;						//入段报告序列号
	Rel87U_A.wResetReportNum		= RPT_NULL;						//返回报告序列号
	//**********************************************************************	


//	Rel87U_A.dwRelTimer				= 0;			 			//定时器
	Rel87U_A.byRelFCounter			= 0;						//故障计数器
	Rel87U_A.byRelRCounter			= 0;						//返回计数器	
	
	Rel87U_A.wExcepWordNum     		= 8;
}

//B相差动速断保护
void CRelayTask::InitRel87U_B()
{
	Rel87U_B.pElecSet				= RS_pn87UC;          			//过量保护电量定值
	Rel87U_B.pTimeSet				= &TimeSet;				 		//过量保护时间定值
	Rel87U_B.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数
	Rel87U_B.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	Rel87U_B.pAccActTimeSet			= NULL;							//过量保护后加速动作时间定值
	Rel87U_B.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel87U_B.pRelCFG				= CFG_pby87U;					//过量保护功能控制字--1:投入，2:退出 
	Rel87U_B.pRelSW					= SW_pby87U;         			//过量保护软压板------1:投入，2:退出
	Rel87U_B.pRelHLockCFG			= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel87U_B.pRelHSuprCFG			= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel87U_B.pRelPDCFG				= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel87U_B.pRelVolLockCFG			= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel87U_B.pRelFDCFG				= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel87U_B.pRelAccCFG				= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel87U_B.pRelFLCFG				= &RelCFGCLR;					//故障测距投入
	Rel87U_B.pSysTimer				= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel87U_B.pRelElec 				= &Idb;							//当前电量指针		
	Rel87U_B.pRelHarm3				= NULL;							//当前三次谐波指针	
	Rel87U_B.pRelHarm5				= NULL;							//当前五次谐波指针	
	Rel87U_B.pRelVolt				= NULL;							//当前用来闭锁的电压的指针

	Rel87U_B.StartRelayWord			= RW_PI87UB;					//过量保护入段继电器字
	Rel87U_B.ActRelayWord			= RW_TR87UB;					//过量保护动作继电器字
	Rel87U_B.AccRelayWord			= FALSE1;						//后加速允许继电器字
	Rel87U_B.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	Rel87U_B.PForwardRelayWord		= FALSE1;						//功率正方向继电器字
	Rel87U_B.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	Rel87U_B.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	Rel87U_B.LockRelayWord			= RW_LKPI87UB;					//入段闭锁继电器字
	Rel87U_B.RSTRelayWord			= RW_LKRST87UB;				//返回闭锁继电器字
	Rel87U_B.LoopCtrlRelayWord		= RW_LPC87UB;					//保护循环控制继电器字
	
	Rel87U_B.wReportNum				= R_87U;						//过量保护报告序号
	Rel87U_B.wAccReportNum			= 0xFFFF;						//加速动作报告序号

	Rel87U_B.bMakeActRpt			= TRUE;
	Rel87U_B.dwK35					= NULL;
	Rel87U_B.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel87U_B.wPickUpReportNum		= RPT_NULL;						//入段报告序列号
	Rel87U_B.wResetReportNum		= RPT_NULL;						//返回报告序列号
	//**********************************************************************	


//	Rel87U_B.dwRelTimer				= 0;			 			//定时器
	Rel87U_B.byRelFCounter			= 0;						//故障计数器
	Rel87U_B.byRelRCounter			= 0;						//返回计数器	
	
	Rel87U_B.wExcepWordNum     		= 8;
}
//C相差动速断保护
void CRelayTask::InitRel87U_C()
{
	Rel87U_C.pElecSet				= RS_pn87UC;          			//过量保护电量定值
	Rel87U_C.pTimeSet				= &TimeSet;				 		//过量保护时间定值
	Rel87U_C.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数
	Rel87U_C.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	Rel87U_C.pAccActTimeSet			= NULL;							//过量保护后加速动作时间定值
	Rel87U_C.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel87U_C.pRelCFG				= CFG_pby87U;					//过量保护功能控制字--1:投入，2:退出 
	Rel87U_C.pRelSW					= SW_pby87U;         			//过量保护软压板------1:投入，2:退出
	Rel87U_C.pRelHLockCFG			= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	Rel87U_C.pRelHSuprCFG			= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel87U_C.pRelPDCFG				= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	Rel87U_C.pRelVolLockCFG			= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	Rel87U_C.pRelFDCFG				= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	Rel87U_C.pRelAccCFG				= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	Rel87U_C.pRelFLCFG				= &RelCFGCLR;					//故障测距投入
	Rel87U_C.pSysTimer				= &dwRelayMSCnt;				//系统定时器指针	
	
	Rel87U_C.pRelElec 				= &Idc;							//当前电量指针		
	Rel87U_C.pRelHarm3				= NULL;							//当前三次谐波指针	
	Rel87U_C.pRelHarm5				= NULL;							//当前五次谐波指针	
	Rel87U_C.pRelVolt				= NULL;							//当前用来闭锁的电压的指针

	Rel87U_C.StartRelayWord			= RW_PI87UC;					//过量保护入段继电器字
	Rel87U_C.ActRelayWord			= RW_TR87UC;					//过量保护动作继电器字
	Rel87U_C.AccRelayWord			= FALSE1;						//后加速允许继电器字
	Rel87U_C.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	Rel87U_C.PForwardRelayWord		= FALSE1;						//功率正方向继电器字
	Rel87U_C.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	Rel87U_C.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	Rel87U_C.LockRelayWord			= RW_LKPI87UC;					//入段闭锁继电器字
	Rel87U_C.RSTRelayWord			= RW_LKRST87UC;				//返回闭锁继电器字
	Rel87U_C.LoopCtrlRelayWord		= RW_LPC87UC;					//保护循环控制继电器字
	
	Rel87U_C.wReportNum				= R_87U;						//过量保护报告序号
	Rel87U_C.wAccReportNum			= 0xFFFF;						//加速动作报告序号

	Rel87U_C.bMakeActRpt			= TRUE;
	Rel87U_C.dwK35					= NULL;
	Rel87U_C.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel87U_C.wPickUpReportNum		= RPT_NULL;						//入段报告序列号
	Rel87U_C.wResetReportNum		= RPT_NULL;						//返回报告序列号
	//**********************************************************************	


//	Rel87U_C.dwRelTimer				= 0;			 			//定时器
	Rel87U_C.byRelFCounter			= 0;						//故障计数器
	Rel87U_C.byRelRCounter			= 0;						//返回计数器	
	
	Rel87U_C.wExcepWordNum     		= 8;
}
//A相比率差动保护
void CRelayTask::InitRel87R_A()
{
	Rel87R_A.p87RISet			= RS_pn87RC; 	//差动电流定值
	Rel87R_A.pIRS1Set			= RS_pnIRS1; 	//制动拐点1定值
	Rel87R_A.pIRS2Set			= RS_pnIRS2; 	//制动拐点2定值 
	Rel87R_A.pSLP1Set			= RS_pnSLP1;		//比率制动系数1
	Rel87R_A.pSLP2Set			= RS_pnSLP2;		//比率制动系数2 
	Rel87R_A.pKrh2Set			= RS_pnkrh2;		//二次谐波制动系数
	Rel87R_A.pKrh5Set			= NULL;				//五次谐波制动系数
	Rel87R_A.pKravSet			= NULL;		//比率差动非周期分量加权系数
	Rel87R_A.pK2avSet			= NULL;		//二次谐波非周期分量加权系数

	Rel87R_A.p87RCFG			= CFG_pby87R;		//保护配置
	Rel87R_A.p87RSW				= SW_pby87R;		//软压板
	Rel87R_A.p68IdH2MCFG		= CFG_pby68IDH2MAXLK;		//差流二次谐波最大相闭锁配置
	Rel87R_A.p68IdH2CCFG		= CFG_pby68IDH2MULLK;		//差流二次谐波综合相闭锁配置
	Rel87R_A.p68IdH2ICFG		= CFG_pby68IDH2SPLLK;		//差流二次谐波分相闭锁配置
	Rel87R_A.p68IrH2MCFG		= CFG_pby68IRH2LK;		//制动电流二次谐波闭锁配置
	Rel87R_A.p68IdH5CFG			= &RelCFGCLR;		//差流5次谐波闭锁
	Rel87R_A.p68IdHMulCFG		= CFG_pby68IDMULLK;

	Rel87R_A.pIdH2Flag			= &b68IdH2Flag_A; 	//二次谐波闭锁标志
	Rel87R_A.pIdH5Flag			= &b68IdH5Flag_A; 	//五次谐波闭锁标志
	Rel87R_A.pIrH2Flag			= &b68IrH2Flag_A; 	//制动电流二次谐波闭锁标志
	Rel87R_A.pIdHMulFlag		= &b68IdHMulFlag_A;
	

	Rel87R_A.p87RPI				= RS_pn87RC;		//抑制计算后的差动电流门槛定值
	Rel87R_A.pId				= &Ida;			//差流电流
	Rel87R_A.pIr				= &Ira;			//制动电流

	Rel87R_A.ACTRelayWord		= RW_TR87RA;					//动作继电器字 
	Rel87R_A.StartRelayWord		= RW_PI87RA; 				//入段继电器字 
	Rel87R_A.LockRelayWord		= RW_LKPI87RA;					//入段闭锁继电器字 
	Rel87R_A.RSTRelayWord		= RW_LKRST87RA;					//返回闭锁继电器字 
	Rel87R_A.LoopCtrlRelayWord	= RW_LPC87RA;				//故障循环控制继电器字 

	Rel87R_A.RptSerialNum		= &RptSerialNumOfAct;					//系统报告序列号 
	Rel87R_A.pSysTimer			= &dwRelayMSCnt;; 					//系统定时器 
	Rel87R_A.wRelRptNum			= R_87R; 				//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel87R_A.wPickUpReportNum	= RPT_NULL;						//入段报告序列号
	Rel87R_A.wResetReportNum	= RPT_NULL;						//返回报告序列号
	//**********************************************************************	

	Rel87R_A.wRelFCounter		= 0;					//故障计数器 
	Rel87R_A.wRelRCounter		= 0;					//返回计数器	
//	Rel87R_A.dwRelTimer			= 0; 					//保护定时器		
	Rel87R_A.wExcepWordNum		= 7;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为14 
	Rel87R_A.Info				= 0;							//备用信息 
}
//B相比率差动保护
void CRelayTask::InitRel87R_B()
{
	Rel87R_B.p87RISet			= RS_pn87RC; 	//差动电流定值
	Rel87R_B.pIRS1Set			= RS_pnIRS1; 	//制动拐点1定值
	Rel87R_B.pIRS2Set			= RS_pnIRS2; 	//制动拐点2定值 
	Rel87R_B.pSLP1Set			= RS_pnSLP1;		//比率制动系数1
	Rel87R_B.pSLP2Set			= RS_pnSLP2;		//比率制动系数2 
	Rel87R_B.pKrh2Set			= RS_pnkrh2;		//二次谐波制动系数
	Rel87R_B.pKrh5Set			= NULL;				//五次谐波制动系数
	Rel87R_B.pKravSet			= NULL;		//比率差动非周期分量加权系数
	Rel87R_B.pK2avSet			= NULL;		//二次谐波非周期分量加权系数

	Rel87R_B.p87RCFG			= CFG_pby87R;		//保护配置
	Rel87R_B.p87RSW				= SW_pby87R;		//软压板
	Rel87R_B.p68IdH2MCFG		= CFG_pby68IDH2MAXLK;		//差流二次谐波最大相闭锁配置
	Rel87R_B.p68IdH2CCFG		= CFG_pby68IDH2MULLK;		//差流二次谐波综合相闭锁配置
	Rel87R_B.p68IdH2ICFG		= CFG_pby68IDH2SPLLK;		//差流二次谐波分相闭锁配置
	Rel87R_B.p68IrH2MCFG		= CFG_pby68IRH2LK;		//制动电流二次谐波闭锁配置
	Rel87R_B.p68IdH5CFG			= &RelCFGCLR;			//差流5次谐波闭锁
	Rel87R_B.p68IdHMulCFG		= CFG_pby68IDMULLK;

	Rel87R_B.pIdH2Flag			= &b68IdH2Flag_B; 	//二次谐波闭锁标志
	Rel87R_B.pIdH5Flag			= &b68IdH5Flag_B; 	//五次谐波闭锁标志
	Rel87R_B.pIrH2Flag			= &b68IrH2Flag_B; 	//制动电流二次谐波闭锁标志
	Rel87R_B.pIdHMulFlag		= &b68IdHMulFlag_B;
	

	Rel87R_B.p87RPI				= RS_pn87RC;		//抑制计算后的差动电流门槛定值
	Rel87R_B.pId				= &Idb;			//差流电流
	Rel87R_B.pIr				= &Irb;			//制动电流

	Rel87R_B.ACTRelayWord		= RW_TR87RB;					//动作继电器字 
	Rel87R_B.StartRelayWord		= RW_PI87RB; 				//入段继电器字 
	Rel87R_B.LockRelayWord		= RW_LKPI87RB;					//入段闭锁继电器字 
	Rel87R_B.RSTRelayWord		= RW_LKRST87RB;					//返回闭锁继电器字 
	Rel87R_B.LoopCtrlRelayWord	= RW_LPC87RB;				//故障循环控制继电器字 

	Rel87R_B.RptSerialNum		= &RptSerialNumOfAct;					//系统报告序列号 
	Rel87R_B.pSysTimer			= &dwRelayMSCnt;; 					//系统定时器 
	Rel87R_B.wRelRptNum			= R_87R; 				//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel87R_B.wPickUpReportNum	= RPT_NULL;						//入段报告序列号
	Rel87R_B.wResetReportNum	= RPT_NULL;						//返回报告序列号
	//**********************************************************************	

	Rel87R_B.wRelFCounter		= 0;					//故障计数器 
	Rel87R_B.wRelRCounter		= 0;					//返回计数器	
//	Rel87R_B.dwRelTimer			= 0; 					//保护定时器		
	Rel87R_B.wExcepWordNum		= 7;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为14 
	Rel87R_B.Info				= 0;							//备用信息 
}
//C相比率差动保护
void CRelayTask::InitRel87R_C()
{
	Rel87R_C.p87RISet			= RS_pn87RC; 	//差动电流定值
	Rel87R_C.pIRS1Set			= RS_pnIRS1; 	//制动拐点1定值
	Rel87R_C.pIRS2Set			= RS_pnIRS2; 	//制动拐点2定值 
	Rel87R_C.pSLP1Set			= RS_pnSLP1;		//比率制动系数1
	Rel87R_C.pSLP2Set			= RS_pnSLP2;		//比率制动系数2 
	Rel87R_C.pKrh2Set			= RS_pnkrh2;		//二次谐波制动系数
	Rel87R_C.pKrh5Set			= NULL;				//五次谐波制动系数
	Rel87R_C.pKravSet			= NULL;		//比率差动非周期分量加权系数
	Rel87R_C.pK2avSet			= NULL;		//二次谐波非周期分量加权系数

	Rel87R_C.p87RCFG			= CFG_pby87R;		//保护配置
	Rel87R_C.p87RSW				= SW_pby87R;		//软压板
	Rel87R_C.p68IdH2MCFG		= CFG_pby68IDH2MAXLK;		//差流二次谐波最大相闭锁配置
	Rel87R_C.p68IdH2CCFG		= CFG_pby68IDH2MULLK;		//差流二次谐波综合相闭锁配置
	Rel87R_C.p68IdH2ICFG		= CFG_pby68IDH2SPLLK;		//差流二次谐波分相闭锁配置
	Rel87R_C.p68IrH2MCFG		= CFG_pby68IRH2LK;		//制动电流二次谐波闭锁配置
	Rel87R_C.p68IdH5CFG			= &RelCFGCLR;			//差流5次谐波闭锁
	Rel87R_C.p68IdHMulCFG		= CFG_pby68IDMULLK;

	Rel87R_C.pIdH2Flag			= &b68IdH2Flag_C; 	//二次谐波闭锁标志
	Rel87R_C.pIdH5Flag			= &b68IdH5Flag_C; 	//五次谐波闭锁标志
	Rel87R_C.pIrH2Flag			= &b68IrH2Flag_C; 	//制动电流二次谐波闭锁标志
	Rel87R_C.pIdHMulFlag		= &b68IdHMulFlag_C;
	

	Rel87R_C.p87RPI				= RS_pn87RC;		//抑制计算后的差动电流门槛定值
	Rel87R_C.pId				= &Idc;			//差流电流
	Rel87R_C.pIr				= &Irc;			//制动电流

	Rel87R_C.ACTRelayWord		= RW_TR87RC;					//动作继电器字 
	Rel87R_C.StartRelayWord		= RW_PI87RC; 				//入段继电器字 
	Rel87R_C.LockRelayWord		= RW_LKPI87RC;					//入段闭锁继电器字 
	Rel87R_C.RSTRelayWord		= RW_LKRST87RC;					//返回闭锁继电器字 
	Rel87R_C.LoopCtrlRelayWord	= RW_LPC87RC;				//故障循环控制继电器字 

	Rel87R_C.RptSerialNum		= &RptSerialNumOfAct;					//系统报告序列号 
	Rel87R_C.pSysTimer			= &dwRelayMSCnt;; 					//系统定时器 
	Rel87R_C.wRelRptNum			= R_87R; 				//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel87R_C.wPickUpReportNum	= RPT_NULL;						//入段报告序列号
	Rel87R_C.wResetReportNum	= RPT_NULL;						//返回报告序列号
	//**********************************************************************	

	Rel87R_C.wRelFCounter		= 0;					//故障计数器 
	Rel87R_C.wRelRCounter		= 0;					//返回计数器	
//	Rel87R_C.dwRelTimer			= 0; 					//保护定时器		
	Rel87R_C.wExcepWordNum		= 7;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为14 
	Rel87R_C.Info				= 0;							//备用信息 
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化高压侧过流1段                                                                 
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_H1()
{
	Rel50I_H1.pElecSet			= RS_pn50IH1C;          						//过量保护电量定值
	Rel50I_H1.pTimeSet			= RS_pn50IH1D;				 			//过量保护时间定值
	Rel50I_H1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_H1.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_H1.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_H1.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_H1.pRelCFG			= CFG_pby50I_H1;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_H1.pRelSW			= SW_pby50I_H;         					//过量保护软压板------1:投入，2:退出
	Rel50I_H1.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_H1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_H1.pRelPDCFG			= CFG_pby50I_H1_PD;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_H1.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_H1.pRelFDCFG			= &RelCFGSET;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_H1.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_H1.pRel47UCFG		= CFG_pby50I_H1_47U;			//复压闭锁功能
	Rel50I_H1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_H1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_H1.pRelElec 		    = &IHmax;									//当前电量指针		
	Rel50I_H1.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_H1.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_H1.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_H1.StartRelayWord	= RW_PI50IH1;						//过量保护入段继电器字
	Rel50I_H1.ActRelayWord		= RW_TR50IH1;							//过量保护动作继电器字
	Rel50I_H1.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_H1.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_H1.PForwardRelayWord	= HPFRW;							//功率方向继电器字
	Rel50I_H1.PRewardRelayWord  = HPRRW;
	Rel50I_H1.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_H1.R47ULKRelayWord	= RW_47UH;
	Rel50I_H1.LockRelayWord		= RW_LKPI50IH1;						//入段闭锁继电器字
	Rel50I_H1.RSTRelayWord		= RW_LKRST50IH1;							//返回闭锁继电器字
	Rel50I_H1.LoopCtrlRelayWord	= RW_LPC50IH1;						//保护循环控制继电器字
	Rel50I_H1.wReportNum		= R_50I_H1;							//过量保护报告序号
	Rel50I_H1.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_H1.dwK35				= NULL;
	Rel50I_H1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_H1.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_H1.wPickUpReportNum	= R_50I_H1_PI;						//入段报告序列号
	Rel50I_H1.wResetReportNum	= R_50I_H1_RES;						//返回报告序列号
	//**********************************************************************	

	//Rel50I_H1.dwRelTimer		= 0;			 			//定时器
	Rel50I_H1.byRelFCounter		= 0;						//故障计数器
	Rel50I_H1.byRelRCounter		= 0;						//返回计数器		
	Rel50I_H1.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化高压侧过流2段                                                                 
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_H2()
{
	Rel50I_H2.pElecSet			= RS_pn50IH2C;          						//过量保护电量定值
	Rel50I_H2.pTimeSet			= RS_pn50IH2D;				 			//过量保护时间定值
	Rel50I_H2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_H2.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_H2.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_H2.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_H2.pRelCFG			= CFG_pby50I_H2;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_H2.pRelSW			= SW_pby50I_H;         					//过量保护软压板------1:投入，2:退出
	Rel50I_H2.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_H2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_H2.pRelPDCFG			= CFG_pby50I_H2_PD;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_H2.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_H2.pRelFDCFG			= &RelCFGSET;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_H2.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_H2.pRel47UCFG		= CFG_pby50I_H2_47U;			//复压闭锁功能
	Rel50I_H2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_H2.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_H2.pRelElec 		    = &IHmax;									//当前电量指针		
	Rel50I_H2.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_H2.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_H2.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_H2.StartRelayWord	= RW_PI50IH2;						//过量保护入段继电器字
	Rel50I_H2.ActRelayWord		= RW_TR50IH2;							//过量保护动作继电器字
	Rel50I_H2.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_H2.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_H2.PForwardRelayWord	= HPFRW;							//功率方向继电器字
	Rel50I_H2.PRewardRelayWord  = HPRRW;
	Rel50I_H2.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_H2.R47ULKRelayWord	= RW_47UH;
	Rel50I_H2.LockRelayWord		= RW_LKPI50IH2;						//入段闭锁继电器字
	Rel50I_H2.RSTRelayWord		= RW_LKRST50IH2;							//返回闭锁继电器字
	Rel50I_H2.LoopCtrlRelayWord	= RW_LPC50IH2;						//保护循环控制继电器字
	Rel50I_H2.wReportNum		= R_50I_H2;							//过量保护报告序号
	Rel50I_H2.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_H2.dwK35				= NULL;
	Rel50I_H2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_H2.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_H2.wPickUpReportNum	= R_50I_H2_PI;						//入段报告序列号
	Rel50I_H2.wResetReportNum	= R_50I_H2_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_H2.dwRelTimer		= 0;			 			//定时器
	Rel50I_H2.byRelFCounter		= 0;						//故障计数器
	Rel50I_H2.byRelRCounter		= 0;						//返回计数器		
	Rel50I_H2.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化高压侧过流3段                                                                 
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_H3()
{
	Rel50I_H3.pElecSet			= RS_pn50IH3C;          						//过量保护电量定值
	Rel50I_H3.pTimeSet			= RS_pn50IH3D;				 			//过量保护时间定值
	Rel50I_H3.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_H3.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_H3.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_H3.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_H3.pRelCFG			= CFG_pby50I_H3;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_H3.pRelSW			= SW_pby50I_H;         					//过量保护软压板------1:投入，2:退出
	Rel50I_H3.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_H3.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_H3.pRelPDCFG			= CFG_pby50I_H3_PD;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_H3.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_H3.pRelFDCFG			= &RelCFGSET;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_H3.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_H3.pRel47UCFG		= CFG_pby50I_H3_47U;			//复压闭锁功能
	Rel50I_H3.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_H3.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_H3.pRelElec 		    = &IHmax;									//当前电量指针		
	Rel50I_H3.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_H3.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_H3.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_H3.StartRelayWord	= RW_PI50IH3;						//过量保护入段继电器字
	Rel50I_H3.ActRelayWord		= RW_TR50IH3;							//过量保护动作继电器字
	Rel50I_H3.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_H3.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_H3.PForwardRelayWord	= HPFRW;							//功率方向继电器字
	Rel50I_H3.PRewardRelayWord  = HPRRW;
	Rel50I_H3.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_H3.R47ULKRelayWord	= RW_47UH;
	Rel50I_H3.LockRelayWord		= RW_LKPI50IH3;						//入段闭锁继电器字
	Rel50I_H3.RSTRelayWord		= RW_LKRST50IH3;							//返回闭锁继电器字
	Rel50I_H3.LoopCtrlRelayWord	= RW_LPC50IH3;						//保护循环控制继电器字
	Rel50I_H3.wReportNum		= R_50I_H3;							//过量保护报告序号
	Rel50I_H3.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_H3.dwK35				= NULL;
	Rel50I_H3.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_H3.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_H3.wPickUpReportNum	= R_50I_H3_PI;						//入段报告序列号
	Rel50I_H3.wResetReportNum	= R_50I_H3_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_H3.dwRelTimer		= 0;			 			//定时器
	Rel50I_H3.byRelFCounter		= 0;						//故障计数器
	Rel50I_H3.byRelRCounter		= 0;						//返回计数器		
	Rel50I_H3.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化低压侧过流1段                                                                 
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_L1()
{
	Rel50I_L1.pElecSet			= RS_pn50IL1C;          						//过量保护电量定值
	Rel50I_L1.pTimeSet			= RS_pn50IL1D;				 			//过量保护时间定值
	Rel50I_L1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_L1.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_L1.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_L1.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_L1.pRelCFG			= CFG_pby50I_L1;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_L1.pRelSW			= SW_pby50I_L;         					//过量保护软压板------1:投入，2:退出
	Rel50I_L1.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_L1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_L1.pRelPDCFG			= CFG_pby50I_L1_PD;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_L1.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_L1.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_L1.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_L1.pRel47UCFG		= CFG_pby50I_L1_47U;			//复压闭锁功能
	Rel50I_L1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_L1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_L1.pRelElec 		    = &ILmax;									//当前电量指针		
	Rel50I_L1.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_L1.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_L1.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_L1.StartRelayWord	= RW_PI50IL1;						//过量保护入段继电器字
	Rel50I_L1.ActRelayWord		= RW_TR50IL1;							//过量保护动作继电器字
	Rel50I_L1.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_L1.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_L1.PForwardRelayWord	= LPFRW;							//功率方向继电器字
	Rel50I_L1.PRewardRelayWord  = LPRRW;
	Rel50I_L1.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_L1.R47ULKRelayWord	= RW_47UL;
	Rel50I_L1.LockRelayWord		= RW_LKPI50IL1;						//入段闭锁继电器字
	Rel50I_L1.RSTRelayWord		= RW_LKRST50IL1;							//返回闭锁继电器字
	Rel50I_L1.LoopCtrlRelayWord	= RW_LPC50IL1;						//保护循环控制继电器字
	Rel50I_L1.wReportNum		= R_50I_L1;							//过量保护报告序号
	Rel50I_L1.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_L1.dwK35				= NULL;
	Rel50I_L1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_L1.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_L1.wPickUpReportNum	= R_50I_L1_PI;						//入段报告序列号
	Rel50I_L1.wResetReportNum	= R_50I_L1_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_L1.dwRelTimer		= 0;			 			//定时器
	Rel50I_L1.byRelFCounter		= 0;						//故障计数器
	Rel50I_L1.byRelRCounter		= 0;						//返回计数器		
	Rel50I_L1.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化低压侧过流2段                                                                 
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_L2()
{
	Rel50I_L2.pElecSet			= RS_pn50IL2C;          						//过量保护电量定值
	Rel50I_L2.pTimeSet			= RS_pn50IL2D;				 			//过量保护时间定值
	Rel50I_L2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_L2.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_L2.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_L2.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_L2.pRelCFG			= CFG_pby50I_L2;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_L2.pRelSW			= SW_pby50I_L;         					//过量保护软压板------1:投入，2:退出
	Rel50I_L2.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_L2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_L2.pRelPDCFG			= CFG_pby50I_L2_PD;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_L2.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_L2.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_L2.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_L2.pRel47UCFG		= CFG_pby50I_L2_47U;			//复压闭锁功能
	Rel50I_L2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_L2.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_L2.pRelElec 		    = &ILmax;									//当前电量指针		
	Rel50I_L2.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_L2.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_L2.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_L2.StartRelayWord	= RW_PI50IL2;						//过量保护入段继电器字
	Rel50I_L2.ActRelayWord		= RW_TR50IL2;							//过量保护动作继电器字
	Rel50I_L2.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_L2.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_L2.PForwardRelayWord	= LPFRW;							//功率方向继电器字
	Rel50I_L2.PRewardRelayWord  = LPRRW;
	Rel50I_L2.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_L2.R47ULKRelayWord	= RW_47UL;
	Rel50I_L2.LockRelayWord		= RW_LKPI50IL2;						//入段闭锁继电器字
	Rel50I_L2.RSTRelayWord		= RW_LKRST50IL2;							//返回闭锁继电器字
	Rel50I_L2.LoopCtrlRelayWord	= RW_LPC50IL2;						//保护循环控制继电器字
	Rel50I_L2.wReportNum		= R_50I_L2;							//过量保护报告序号
	Rel50I_L2.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_L2.dwK35				= NULL;
	Rel50I_L2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_L2.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_L2.wPickUpReportNum	= R_50I_L2_PI;						//入段报告序列号
	Rel50I_L2.wResetReportNum	= R_50I_L2_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_L2.dwRelTimer		= 0;			 			//定时器
	Rel50I_L2.byRelFCounter		= 0;						//故障计数器
	Rel50I_L2.byRelRCounter		= 0;						//返回计数器		
	Rel50I_L2.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化母线充电保护                                                          
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_SH()
{
	Rel50I_SH.pElecSet			= RS_pn50ISHC;          						//过量保护电量定值
	Rel50I_SH.pTimeSet			= RS_pn50ISHD;				 			//过量保护时间定值
	Rel50I_SH.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_SH.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_SH.pAccActTimeSet	= RS_pn50ISHD;									//过量保护后加速动作时间定值
	Rel50I_SH.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_SH.pRelCFG			= CFG_pby50I_SH;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_SH.pRelSW			= SW_pby50I_SH;         					//过量保护软压板------1:投入，2:退出
	Rel50I_SH.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_SH.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_SH.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_SH.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_SH.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_SH.pRelAccCFG		= CFG_pby50I_SH;								//有无后加速功能------1:有，  2:无
	Rel50I_SH.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50I_SH.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_SH.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_SH.pRelElec 		    = &ILmax;									//当前电量指针		
	Rel50I_SH.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_SH.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_SH.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_SH.StartRelayWord	= RW_PI50ISH;						//过量保护入段继电器字
	Rel50I_SH.ActRelayWord		= RW_TR50ISH;							//过量保护动作继电器字
	Rel50I_SH.AccRelayWord		= RW_TSH;									//后加速允许继电器字
	Rel50I_SH.AccActRelayWord	= RW_TR50ISH;									//后加速动作继电器字
	Rel50I_SH.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50I_SH.PRewardRelayWord  = FALSE1;
	Rel50I_SH.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_SH.R47ULKRelayWord	= FALSE1;
	Rel50I_SH.LockRelayWord		= RW_LKPI50ISH;						//入段闭锁继电器字
	Rel50I_SH.RSTRelayWord		= RW_LKRST50ISH;							//返回闭锁继电器字
	Rel50I_SH.LoopCtrlRelayWord	= RW_LPC50ISH;						//保护循环控制继电器字
	Rel50I_SH.wReportNum		= R_50I_SH;							//过量保护报告序号
	Rel50I_SH.wAccReportNum		= R_50I_SH;								//加速动作报告序号

	Rel50I_SH.dwK35				= NULL;
	Rel50I_SH.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_SH.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_SH.wPickUpReportNum	= R_50I_SH_PI;						//入段报告序列号
	Rel50I_SH.wResetReportNum	= R_50I_SH_RES;						//返回报告序列号	
	Rel50I_SH.wAccPickUpRptNum	= R_50I_SH_PI;						//入段报告序列号
	Rel50I_SH.wAccResetRptNum	= R_50I_SH_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_SH.dwRelTimer		= 0;			 			//定时器
	Rel50I_SH.byRelFCounter		= 0;						//故障计数器
	Rel50I_SH.byRelRCounter		= 0;						//返回计数器		
	Rel50I_SH.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化零序过流1段                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_N1()
{
	Rel50I_N1.pElecSet			= RS_pn50IN1C;          						//过量保护电量定值
	Rel50I_N1.pTimeSet			= RS_pn50IN1D;				 			//过量保护时间定值
	Rel50I_N1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_N1.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_N1.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_N1.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_N1.pRelCFG			= CFG_pby50I_N1;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_N1.pRelSW			= SW_pby50N;         					//过量保护软压板------1:投入，2:退出
	Rel50I_N1.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_N1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_N1.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_N1.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_N1.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_N1.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_N1.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50I_N1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_N1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_N1.pRelElec 		    = &I0;									//当前电量指针		
	Rel50I_N1.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_N1.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_N1.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_N1.StartRelayWord	= RW_PI50N1;						//过量保护入段继电器字
	Rel50I_N1.ActRelayWord		= RW_TR50N1;							//过量保护动作继电器字
	Rel50I_N1.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_N1.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_N1.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50I_N1.PRewardRelayWord  = FALSE1;
	Rel50I_N1.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_N1.R47ULKRelayWord	= FALSE1;
	Rel50I_N1.LockRelayWord		= RW_LKPI50N1;						//入段闭锁继电器字
	Rel50I_N1.RSTRelayWord		= RW_LKRST50N1;							//返回闭锁继电器字
	Rel50I_N1.LoopCtrlRelayWord	= RW_LPC50N1;						//保护循环控制继电器字
	Rel50I_N1.wReportNum		= R_50I_N1;							//过量保护报告序号
	Rel50I_N1.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_N1.dwK35				= NULL;
	Rel50I_N1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_N1.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_N1.wPickUpReportNum	= R_50I_N1_PI;						//入段报告序列号
	Rel50I_N1.wResetReportNum	= R_50I_N1_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_N1.dwRelTimer		= 0;			 			//定时器
	Rel50I_N1.byRelFCounter		= 0;						//故障计数器
	Rel50I_N1.byRelRCounter		= 0;						//返回计数器		
	Rel50I_N1.wExcepWordNum     	= 8;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化零序过流2段                                                         
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50I_N2()
{
	Rel50I_N2.pElecSet			= RS_pn50IN2C;          						//过量保护电量定值
	Rel50I_N2.pTimeSet			= RS_pn50IN2D;				 			//过量保护时间定值
	Rel50I_N2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_N2.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_N2.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_N2.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_N2.pRelCFG			= CFG_pby50I_N2;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_N2.pRelSW			= SW_pby50N;         					//过量保护软压板------1:投入，2:退出
	Rel50I_N2.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_N2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_N2.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_N2.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_N2.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_N2.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_N2.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50I_N2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_N2.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_N2.pRelElec 		    = &I0;									//当前电量指针		
	Rel50I_N2.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_N2.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_N2.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_N2.StartRelayWord	= RW_PI50N2;						//过量保护入段继电器字
	Rel50I_N2.ActRelayWord		= RW_TR50N2;							//过量保护动作继电器字
	Rel50I_N2.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_N2.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_N2.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50I_N2.PRewardRelayWord  = FALSE1;
	Rel50I_N2.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_N2.R47ULKRelayWord	= FALSE1;
	Rel50I_N2.LockRelayWord		= RW_LKPI50N2;						//入段闭锁继电器字
	Rel50I_N2.RSTRelayWord		= RW_LKRST50N2;							//返回闭锁继电器字
	Rel50I_N2.LoopCtrlRelayWord	= RW_LPC50N2;						//保护循环控制继电器字
	Rel50I_N2.wReportNum		= R_50I_N2;							//过量保护报告序号
	Rel50I_N2.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_N2.dwK35				= NULL;
	Rel50I_N2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_N2.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_N2.wPickUpReportNum	= R_50I_N2_PI;						//入段报告序列号
	Rel50I_N2.wResetReportNum	= R_50I_N2_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_N2.dwRelTimer		= 0;			 			//定时器
	Rel50I_N2.byRelFCounter		= 0;						//故障计数器
	Rel50I_N2.byRelRCounter		= 0;						//返回计数器		
	Rel50I_N2.wExcepWordNum     = 8;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化高压侧零序过压                                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel59U_NH()
{
	Rel59U_NH.pElecSet			= RS_pn59UNHV;          						//过量保护电量定值
	Rel59U_NH.pTimeSet			= RS_pn59UNHD;				 			//过量保护时间定值
	Rel59U_NH.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel59U_NH.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel59U_NH.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel59U_NH.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel59U_NH.pRelCFG			= CFG_pby59U_NH;							//过量保护功能控制字--1:投入，2:退出 
	Rel59U_NH.pRelSW			= SW_pby59N;         					//过量保护软压板------1:投入，2:退出
	Rel59U_NH.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel59U_NH.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel59U_NH.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel59U_NH.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel59U_NH.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel59U_NH.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel59U_NH.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel59U_NH.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel59U_NH.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel59U_NH.pRelElec 		    = &U0h;									//当前电量指针		
	Rel59U_NH.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel59U_NH.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel59U_NH.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel59U_NH.StartRelayWord	= RW_PI59NH;						//过量保护入段继电器字
	Rel59U_NH.ActRelayWord		= RW_TR59NH;							//过量保护动作继电器字
	Rel59U_NH.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel59U_NH.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel59U_NH.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel59U_NH.PRewardRelayWord  = FALSE1;
	Rel59U_NH.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel59U_NH.R47ULKRelayWord	= FALSE1;
	Rel59U_NH.LockRelayWord		= RW_LKPI59NH;						//入段闭锁继电器字
	Rel59U_NH.RSTRelayWord		= RW_LKRST59NH;							//返回闭锁继电器字
	Rel59U_NH.LoopCtrlRelayWord	= RW_LPC59NH;						//保护循环控制继电器字
	Rel59U_NH.wReportNum		= R_59U_H;							//过量保护报告序号
	Rel59U_NH.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel59U_NH.dwK35				= NULL;
	Rel59U_NH.RptSerialNum		= &RptSerialNumOfAct;						
    Rel59U_NH.bMakeActRpt       = TRUE;
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel59U_NH.wPickUpReportNum	= R_59U_H_PI;						//入段报告序列号
	Rel59U_NH.wResetReportNum	= R_59U_H_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel59U_NH.dwRelTimer		= 0;			 			//定时器
	Rel59U_NH.byRelFCounter		= 0;						//故障计数器
	Rel59U_NH.byRelRCounter		= 0;						//返回计数器		
	Rel59U_NH.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化低压侧零序过压                                                       
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel59U_NL()
{
	Rel59U_NL.pElecSet			= RS_pn59UNLV;          						//过量保护电量定值
	Rel59U_NL.pTimeSet			= RS_pn59UNLD;				 			//过量保护时间定值
	Rel59U_NL.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel59U_NL.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel59U_NL.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel59U_NL.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel59U_NL.pRelCFG			= CFG_pby59U_NL;							//过量保护功能控制字--1:投入，2:退出 
	Rel59U_NL.pRelSW			= SW_pby59N;         					//过量保护软压板------1:投入，2:退出
	Rel59U_NL.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel59U_NL.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel59U_NL.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel59U_NL.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel59U_NL.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel59U_NL.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel59U_NL.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel59U_NL.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel59U_NL.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel59U_NL.pRelElec 		    = &U0l;									//当前电量指针		
	Rel59U_NL.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel59U_NL.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel59U_NL.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel59U_NL.StartRelayWord	= RW_PI59NL;						//过量保护入段继电器字
	Rel59U_NL.ActRelayWord		= RW_TR59NL;							//过量保护动作继电器字
	Rel59U_NL.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel59U_NL.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel59U_NL.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel59U_NL.PRewardRelayWord  = FALSE1;
	Rel59U_NL.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel59U_NL.R47ULKRelayWord	= FALSE1;
	Rel59U_NL.LockRelayWord		= RW_LKPI59NL;						//入段闭锁继电器字
	Rel59U_NL.RSTRelayWord		= RW_LKRST59NL;							//返回闭锁继电器字
	Rel59U_NL.LoopCtrlRelayWord	= RW_LPC59NL;						//保护循环控制继电器字
	Rel59U_NL.wReportNum		= R_59U_L;							//过量保护报告序号
	Rel59U_NL.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel59U_NL.dwK35				= NULL;
	Rel59U_NL.RptSerialNum		= &RptSerialNumOfAct;						
    Rel59U_NL.bMakeActRpt       = FALSE;   //本装置中，低压侧零序配置为告警
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel59U_NL.wPickUpReportNum	= 0xFFFF;						//入段报告序列号
	Rel59U_NL.wResetReportNum	= 0xFFFF;						//返回报告序列号
	//**********************************************************************	

//	Rel59U_NL.dwRelTimer		= 0;			 			//定时器
	Rel59U_NL.byRelFCounter		= 0;						//故障计数器
	Rel59U_NL.byRelRCounter		= 0;						//返回计数器		
	Rel59U_NL.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过负荷1段                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50OL1()
{
	Rel50OL_1.pElecSet			= RS_pn50OL1C;          						//过量保护电量定值
	Rel50OL_1.pTimeSet			= RS_pn50OL1D;				 			//过量保护时间定值
	Rel50OL_1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50OL_1.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50OL_1.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50OL_1.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50OL_1.pRelCFG			= CFG_pby50OL_1;							//过量保护功能控制字--1:投入，2:退出 
	Rel50OL_1.pRelSW			= SW_pby50OL;         					//过量保护软压板------1:投入，2:退出
	Rel50OL_1.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50OL_1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50OL_1.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50OL_1.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50OL_1.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50OL_1.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50OL_1.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50OL_1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50OL_1.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50OL_1.pRelElec 		    = &IHmax;									//当前电量指针		
	Rel50OL_1.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50OL_1.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50OL_1.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50OL_1.StartRelayWord	= RW_PI50OL1;						//过量保护入段继电器字
	Rel50OL_1.ActRelayWord		= RW_TR50OL1;							//过量保护动作继电器字
	Rel50OL_1.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50OL_1.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50OL_1.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50OL_1.PRewardRelayWord  = FALSE1;
	Rel50OL_1.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50OL_1.R47ULKRelayWord	= FALSE1;
	Rel50OL_1.LockRelayWord		= RW_LKPI50OL1;						//入段闭锁继电器字
	Rel50OL_1.RSTRelayWord		= RW_LKRST50OL1;							//返回闭锁继电器字
	Rel50OL_1.LoopCtrlRelayWord	= RW_LPC50OL1;						//保护循环控制继电器字
	Rel50OL_1.wReportNum		= R_50OL_1;							//过量保护报告序号
	Rel50OL_1.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50OL_1.dwK35				= NULL;
	Rel50OL_1.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50OL_1.bMakeActRpt       = TRUE;   //本装置中，低压侧零序配置为告警
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50OL_1.wPickUpReportNum	= R_50OL_1_PI;						//入段报告序列号
	Rel50OL_1.wResetReportNum	= R_50OL_1_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50OL_1.dwRelTimer		= 0;			 			//定时器
	Rel50OL_1.byRelFCounter		= 0;						//故障计数器
	Rel50OL_1.byRelRCounter		= 0;						//返回计数器		
	Rel50OL_1.wExcepWordNum     	= 8;
}
//------------------------------------------------------------------------------------------ 
// 名称: 初始化过负荷2段                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50OL2()
{
	Rel50OL_2.pElecSet			= RS_pn50OL2C;          						//过量保护电量定值
	Rel50OL_2.pTimeSet			= RS_pn50OL2D;				 			//过量保护时间定值
	Rel50OL_2.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50OL_2.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50OL_2.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50OL_2.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50OL_2.pRelCFG			= CFG_pby50OL_2;							//过量保护功能控制字--1:投入，2:退出 
	Rel50OL_2.pRelSW			= SW_pby50OL;         					//过量保护软压板------1:投入，2:退出
	Rel50OL_2.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50OL_2.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50OL_2.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50OL_2.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50OL_2.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50OL_2.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50OL_2.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50OL_2.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50OL_2.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50OL_2.pRelElec 		    = &IHmax;									//当前电量指针		
	Rel50OL_2.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50OL_2.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50OL_2.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50OL_2.StartRelayWord	= RW_PI50OL2;						//过量保护入段继电器字
	Rel50OL_2.ActRelayWord		= RW_TR50OL2;							//过量保护动作继电器字
	Rel50OL_2.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50OL_2.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50OL_2.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50OL_2.PRewardRelayWord  = FALSE1;
	Rel50OL_2.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50OL_2.R47ULKRelayWord	= FALSE1;
	Rel50OL_2.LockRelayWord		= RW_LKPI50OL2;						//入段闭锁继电器字
	Rel50OL_2.RSTRelayWord		= RW_LKRST50OL2;							//返回闭锁继电器字
	Rel50OL_2.LoopCtrlRelayWord	= RW_LPC50OL2;						//保护循环控制继电器字
	Rel50OL_2.wReportNum		= R_50OL_2;							//过量保护报告序号
	Rel50OL_2.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50OL_2.dwK35				= NULL;
	Rel50OL_2.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50OL_2.bMakeActRpt       = TRUE;   //本装置中，低压侧零序配置为告警
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50OL_2.wPickUpReportNum	= R_50OL_2_PI;						//入段报告序列号
	Rel50OL_2.wResetReportNum	= R_50OL_2_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50OL_2.dwRelTimer		= 0;			 			//定时器
	Rel50OL_2.byRelFCounter		= 0;						//故障计数器
	Rel50OL_2.byRelRCounter		= 0;						//返回计数器		
	Rel50OL_2.wExcepWordNum     	= 8;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化过负荷3段                                                     
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50OL3()
{
	Rel50OL_3.pElecSet			= RS_pn50OL3C;          						//过量保护电量定值
	Rel50OL_3.pTimeSet			= RS_pn50OL3D;				 			//过量保护时间定值
	Rel50OL_3.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50OL_3.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50OL_3.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50OL_3.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50OL_3.pRelCFG			= CFG_pby50OL_3;							//过量保护功能控制字--1:投入，2:退出 
	Rel50OL_3.pRelSW			= SW_pby50OL;         					//过量保护软压板------1:投入，2:退出
	Rel50OL_3.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50OL_3.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50OL_3.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50OL_3.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50OL_3.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50OL_3.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50OL_3.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50OL_3.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50OL_3.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50OL_3.pRelElec 		    = &IHmax;									//当前电量指针		
	Rel50OL_3.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50OL_3.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50OL_3.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50OL_3.StartRelayWord	= RW_PI50OL3;						//过量保护入段继电器字
	Rel50OL_3.ActRelayWord		= RW_TR50OL3;							//过量保护动作继电器字
	Rel50OL_3.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50OL_3.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50OL_3.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50OL_3.PRewardRelayWord  = FALSE1;
	Rel50OL_3.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50OL_3.R47ULKRelayWord	= FALSE1;
	Rel50OL_3.LockRelayWord		= RW_LKPI50OL3;						//入段闭锁继电器字
	Rel50OL_3.RSTRelayWord		= RW_LKRST50OL3;							//返回闭锁继电器字
	Rel50OL_3.LoopCtrlRelayWord	= RW_LPC50OL3;						//保护循环控制继电器字
	Rel50OL_3.wReportNum		= R_50OL_3;							//过量保护报告序号
	Rel50OL_3.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50OL_3.dwK35				= NULL;
	Rel50OL_3.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50OL_3.bMakeActRpt       = TRUE;   //本装置中，低压侧零序配置为告警
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50OL_3.wPickUpReportNum	= R_50OL_3_PI;						//入段报告序列号
	Rel50OL_3.wResetReportNum	= R_50OL_3_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50OL_3.dwRelTimer		= 0;			 			//定时器
	Rel50OL_3.byRelFCounter		= 0;						//故障计数器
	Rel50OL_3.byRelRCounter		= 0;						//返回计数器		
	Rel50OL_3.wExcepWordNum     	= 8;
}
//过负荷告警，yanzh 2014年8月27日 11:32:32增加
void CRelayTask::InitRel50OLA(void)		//过负荷告警
{
		Rel50OL_A.pElecSet			= RS_pn50OLAC;									//过量保护电量定值
		Rel50OL_A.pTimeSet			= RS_pn50OLAD;							//过量保护时间定值
		Rel50OL_A.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
		Rel50OL_A.pVolLockSet		= NULL; 							//过量保护电压闭锁定值
		Rel50OL_A.pAccActTimeSet	= NULL; 								//过量保护后加速动作时间定值
		Rel50OL_A.pH2Coef			= NULL; 							//二次谐波闭锁系数，谐波加权系数的放大倍数
		
		Rel50OL_A.pRelCFG			= CFG_pby50OL_A;							//过量保护功能控制字--1:投入，2:退出 
		Rel50OL_A.pRelSW			= SW_pby50OL;							//过量保护软压板------1:投入，2:退出
		Rel50OL_A.pRelHLockCFG		= &RelCFGCLR;						//谐波闭锁功能控制位--1:投入，2:退出
		Rel50OL_A.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
		Rel50OL_A.pRelPDCFG 		= &RelCFGCLR;							//功率方向功能控制位--1:投入，2:退出
		Rel50OL_A.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
		Rel50OL_A.pRelFDCFG 		= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
		Rel50OL_A.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
		Rel50OL_A.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
		Rel50OL_A.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
		Rel50OL_A.pSysTimer 		= &dwRelayMSCnt;						//系统定时器指针	
		
		Rel50OL_A.pRelElec			= &IHmax;									//当前电量指针		
		Rel50OL_A.pRelHarm3 		= NULL; 								//当前三次谐波指针	
		Rel50OL_A.pRelHarm5 		= NULL; 								//当前五次谐波指针	
		Rel50OL_A.pRelVolt			= NULL; 								//当前用来闭锁的电压的指针
	
		Rel50OL_A.StartRelayWord	= RW_PI50OLA;						//过量保护入段继电器字
		Rel50OL_A.ActRelayWord		= RW_TR50OLA;							//过量保护动作继电器字
		Rel50OL_A.AccRelayWord		= FALSE1;									//后加速允许继电器字
		Rel50OL_A.AccActRelayWord	= FALSE1;									//后加速动作继电器字
		Rel50OL_A.PForwardRelayWord = FALSE1;							//功率方向继电器字
		Rel50OL_A.PRewardRelayWord	= FALSE1;
		Rel50OL_A.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
		Rel50OL_A.R47ULKRelayWord	= FALSE1;
		Rel50OL_A.LockRelayWord 	= RW_LKPI50OLA; 					//入段闭锁继电器字
		Rel50OL_A.RSTRelayWord		= RW_LKRST50OLA;							//返回闭锁继电器字
		Rel50OL_A.LoopCtrlRelayWord = RW_LPC50OLA;						//保护循环控制继电器字
		Rel50OL_A.wReportNum		= R_50OL_A; 						//过量保护报告序号
		Rel50OL_A.wAccReportNum 	= 0xFFFF;								//加速动作报告序号
	
		Rel50OL_A.dwK35 			= NULL;
		Rel50OL_A.RptSerialNum		= &RptSerialNumOfAct;						
		Rel50OL_A.bMakeActRpt		= FALSE;   //本段设为告警
		//**********************入段返回报告部分*************	yanxs  13-03-18
		Rel50OL_A.wPickUpReportNum	= R_50OL_A_PI;						//入段报告序列号
		Rel50OL_A.wResetReportNum	= R_50OL_A_RES; 					//返回报告序列号
		//**********************************************************************	
	
	//	Rel50OL_A.dwRelTimer		= 0;						//定时器
		Rel50OL_A.byRelFCounter 	= 0;						//故障计数器
		Rel50OL_A.byRelRCounter 	= 0;						//返回计数器		
		Rel50OL_A.wExcepWordNum 		= 8;
}

//------------------------------------------------------------------------------------------ 
// 名称: 初始化间隙过流                                               
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::InitRel50IJX()
{
	Rel50I_JX.pElec1Set			= RS_pn50I_JXC;          						//过量保护电量定值	
	Rel50I_JX.pElec2Set			= RS_pn59UNHV;          						//过量保护电量定值
	Rel50I_JX.pTimeSet			= RS_pn50I_JXD;				 			//过量保护时间定值
	Rel50I_JX.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	Rel50I_JX.pVolLockSet		= NULL;				 	    		//过量保护电压闭锁定值
	Rel50I_JX.pAccActTimeSet	= NULL;									//过量保护后加速动作时间定值
	Rel50I_JX.pH2Coef			= NULL;								//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	Rel50I_JX.pRelCFG			= CFG_pby50I_JX;							//过量保护功能控制字--1:投入，2:退出 
	Rel50I_JX.pRelSW			= SW_pby50I_JX;         					//过量保护软压板------1:投入，2:退出
	Rel50I_JX.pRelHLockCFG		= &RelCFGCLR;			    		//谐波闭锁功能控制位--1:投入，2:退出
	Rel50I_JX.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	Rel50I_JX.pRelPDCFG			= &RelCFGCLR;		    				//功率方向功能控制位--1:投入，2:退出
	Rel50I_JX.pRelVolLockCFG	= &RelCFGCLR;						//低压闭锁功能控制位--1:投入，2:退出
	Rel50I_JX.pRelFDCFG			= &RelCFGCLR;						//功率方向功能控制位--1:正方向，2:反方向
	Rel50I_JX.pRelAccCFG		= &RelCFGCLR;								//有无后加速功能------1:有，  2:无
	Rel50I_JX.pRel47UCFG		= &RelCFGCLR;			//复压闭锁功能
	Rel50I_JX.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	Rel50I_JX.pSysTimer			= &dwRelayMSCnt;						//系统定时器指针	
	
	Rel50I_JX.pRelElec1 		= &Ijx;									//当前电量指针
	Rel50I_JX.pRelElec2 		= &U0h;
	Rel50I_JX.pRelHarm3			= NULL;									//当前三次谐波指针	
	Rel50I_JX.pRelHarm5			= NULL;									//当前五次谐波指针	
	Rel50I_JX.pRelVolt			= NULL;									//当前用来闭锁的电压的指针

	Rel50I_JX.StartRelayWord	= RW_PI50IJX;						//过量保护入段继电器字
	Rel50I_JX.ActRelayWord		= RW_TR50IJX;							//过量保护动作继电器字
	Rel50I_JX.AccRelayWord		= FALSE1;									//后加速允许继电器字
	Rel50I_JX.AccActRelayWord	= FALSE1;									//后加速动作继电器字
	Rel50I_JX.PForwardRelayWord	= FALSE1;							//功率方向继电器字
	Rel50I_JX.PRewardRelayWord  = FALSE1;
	Rel50I_JX.H2LockRelayWord	= FALSE1;							//二次谐波闭锁继电器字
	Rel50I_JX.R47ULKRelayWord	= FALSE1;
	Rel50I_JX.LockRelayWord		= RW_LKPI50IJX;						//入段闭锁继电器字
	Rel50I_JX.RSTRelayWord		= RW_LKRST50IJX;							//返回闭锁继电器字
	Rel50I_JX.LoopCtrlRelayWord	= RW_LPC50IJX;						//保护循环控制继电器字
	Rel50I_JX.wReportNum		= R_50I_JX;							//过量保护报告序号
	Rel50I_JX.wAccReportNum		= 0xFFFF;								//加速动作报告序号

	Rel50I_JX.dwK35				= NULL;
	Rel50I_JX.RptSerialNum		= &RptSerialNumOfAct;						
    Rel50I_JX.bMakeActRpt       = TRUE;   //本装置中，低压侧零序配置为告警
	//**********************入段返回报告部分*************	yanxs  13-03-18
	Rel50I_JX.wPickUpReportNum	= R_50I_JX_PI;						//入段报告序列号
	Rel50I_JX.wResetReportNum	= R_50I_JX_RES;						//返回报告序列号
	//**********************************************************************	

//	Rel50I_JX.dwRelTimer		= 0;			 			//定时器
	Rel50I_JX.byRelFCounter		= 0;						//故障计数器
	Rel50I_JX.byRelRCounter		= 0;						//返回计数器		
	Rel50I_JX.wExcepWordNum     = 8;
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
	Rel27U.pLockCurSet					= RS_pn27U_50I;					//闭锁电流定值 

	Rel27U.pRelCFG						= CFG_pby27U;					  	 //欠量保护配置 
	Rel27U.pRelSW						= SW_pby27U;					  	 //欠量保护软压板 
	Rel27U.pRelAlarmCFG					= &RelCFGCLR;						//欠量保护投告警
	Rel27U.pRelCurLockCFG				= CFG_pby27U_50I;
	Rel27U.pSysTimer					= &dwRelayMSCnt;						//系统定时器指针 

	Rel27U.pRelElec						= &U_27U;						//当前的电量指针 
	Rel27U.pRelLockElec					= &IHmax;					//闭锁电流 
	Rel27U.ActRelayWord					= RW_TR27U;					//欠量保护动作继电器字 
	Rel27U.StartRelayWord				= RW_PI27U;					//欠量保护入段继电器字 
	Rel27U.AlarmRelayWord				= FALSE1;					//欠量保护告警继电器字 
	Rel27U.LockRelayWord				= RW_LKPI27U;				//入段闭锁继电器字 
	Rel27U.RSTRelayWord					= RW_LKRST27U;					//故障返回闭锁继电器字 
	Rel27U.LoopCtrlRelayWord			= FALSE1;				//故障循环控制继电器字 
	Rel27U.wR52BFRealyword				= RW_52BFA;
	Rel27U.wActRptNum					= R_27U;					//欠量保护动作报告序号 
	Rel27U.wAlarmRptNum					= 0xFFFF;					//欠量保护告警报告序号 
	Rel27U.wTRFailRptNum				= R_52BF;
	Rel27U.wPickUpRptNum				= R_PICKUP;
	Rel27U.RptSerialNum					= &g_RptSerialNum;
	
	Rel27U.R27RptSerialNum				= 0;
//	Rel27U.dwRelTimer					= 0;						//欠量保护定时器 	
	Rel27U.byRelFCounter				= 0;					//故障计数器 
	Rel27U.byRelRCounter				= 0;					//返回计数器 
	Rel27U.bHaveMake52BFRptFlag			= FALSE;
	Rel27U.wExcepWordNum				= 14;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为10 
}
void CRelayTask::InitRel49I(void)
{
	
	Rel49IA.p49KSet					= RS_pn49K;
	Rel49IA.p49TSet					= RS_pn49T;
	Rel49IA.p49ARSet				= RS_pn49AR;
	Rel49IA.p49KTSet				= RS_pn49KT;
	Rel49IA.pRatedInSet				= RS_pnRAC;
	Rel49IA.pRel49CFG				= CFG_pby49I;
	Rel49IA.pRel49SW				= SW_pby49I;
	Rel49IA.pICoef					= CF_pnIp;
	Rel49IA.pIHACoef				= CF_pnIp;
	Rel49IA.pCurrent				= dw49IA;	
	Rel49IA.dwKIn					= 0;	
	Rel49IA.PreCurrent				= 0;			
	Rel49IA.l49OT					= 0;
	Rel49IA.l49KOT					= 0;
	Rel49IA.lExpT					= 0;
	Rel49IA.lExpKT					= 0;
	Rel49IA.R49WarnRW				= RW_AR49IA;
	Rel49IA.R49ActRW				= RW_TR49IA;
	Rel49IA.R49ActRptNum			= R_49I;
	Rel49IA.R49WarnRptNum			= R_49IAR;
	Rel49IA.R49RSTRptNum			= R_49I_RES;	
	Rel49IA.dw49Warn				= 0;
	Rel49IA.dw49Warn_099        	= 0;
	Rel49IA.dw49AThermaVal			= 0;
	Rel49IA.dw49AThermaPreVal		= 0;	
	Rel49IA.pSysTimer				= &dwRelayMSCnt;						//系统定时器 
//	Rel49IA.dwRelTimer				= 0;							//欠量保护定时器	
	Rel49IA.wExcepWordNum			= 16;
	InitThermalOverLoad(&Rel49IA);

	Rel49IB.p49KSet					= RS_pn49K;
	Rel49IB.p49TSet					= RS_pn49T;
	Rel49IB.p49ARSet				= RS_pn49AR;
	Rel49IB.p49KTSet				= RS_pn49KT;
	Rel49IB.pRatedInSet				= RS_pnRAC;
	Rel49IB.pRel49CFG 				= CFG_pby49I;
	Rel49IB.pRel49SW				= SW_pby49I;
	Rel49IB.pICoef					= CF_pnIp;
	Rel49IB.pIHACoef				= CF_pnIp;
	Rel49IB.pCurrent				= dw49IB;
	Rel49IB.dwKIn 					= 0;	
	Rel49IB.PreCurrent				= 0;			
	Rel49IB.l49OT 					= 0;
	Rel49IB.l49KOT					= 0;
	Rel49IB.lExpT 					= 0;
	Rel49IB.lExpKT					= 0;
	Rel49IB.R49WarnRW 				= RW_AR49IB;
	Rel49IB.R49ActRW				= RW_TR49IB;
	Rel49IB.R49ActRptNum			= R_49I;
	Rel49IB.R49WarnRptNum 			= R_49IAR;
	Rel49IB.R49RSTRptNum			= R_49I_RES;
	Rel49IB.dw49Warn				= 0;
	Rel49IB.dw49Warn_099			= 0;
	Rel49IB.dw49AThermaVal			= 0;
	Rel49IB.dw49AThermaPreVal 		= 0;	
	Rel49IB.pSysTimer 				= &dwRelayMSCnt;						//系统定时器 
//	Rel49IB.dwRelTimer				= 0;								//欠量保护定时器 
	Rel49IB.wExcepWordNum 			= 16;
	InitThermalOverLoad(&Rel49IB);

	Rel49IC.p49KSet					= RS_pn49K;
	Rel49IC.p49TSet					= RS_pn49T;
	Rel49IC.p49ARSet				= RS_pn49AR;
	Rel49IC.p49KTSet				= RS_pn49KT;
	Rel49IC.pRatedInSet				= RS_pnRAC;
	Rel49IC.pRel49CFG 				= CFG_pby49I;
	Rel49IC.pRel49SW				= SW_pby49I;
	Rel49IC.pICoef					= CF_pnIp;
	Rel49IC.pIHACoef				= CF_pnIp;
	Rel49IC.pCurrent				= dw49IC;		
	Rel49IC.dwKIn 					= 0;	
	Rel49IC.PreCurrent				= 0;			
	Rel49IC.l49OT 					= 0;
	Rel49IC.l49KOT					= 0;
	Rel49IC.lExpT 					= 0;
	Rel49IC.lExpKT					= 0;
	Rel49IC.R49WarnRW 				= RW_AR49IC;
	Rel49IC.R49ActRW				= RW_TR49IC;
	Rel49IC.R49ActRptNum			= R_49I;
	Rel49IC.R49WarnRptNum 			= R_49IAR;
	Rel49IC.R49RSTRptNum			= R_49I_RES;
	Rel49IC.dw49Warn				= 0;
	Rel49IC.dw49Warn_099			= 0;
	Rel49IC.dw49AThermaVal			= 0;
	Rel49IC.dw49AThermaPreVal 		= 0;	
	Rel49IC.pSysTimer 				= &dwRelayMSCnt;						//系统定时器 
//	Rel49IC.dwRelTimer				= 0;		
	Rel49IC.wExcepWordNum 			= 16;
	InitThermalOverLoad(&Rel49IC);
	
}
void CRelayTask::InitR87IAlarm()
{	
	
	R87Alarm.p87RSW				= SW_pby87R;		//软压板
	R87Alarm.p87R_A_CFG			= CFG_pby87R;		//保护配置
	R87Alarm.p87R_B_CFG			= CFG_pby87R;		//保护配置
	R87Alarm.p87R_C_CFG			= CFG_pby87R;		//保护配置

	R87Alarm.p87RIASet			= RS_pn87RC; 	//A差动电流定值
	R87Alarm.p87RIBSet			= RS_pn87RC; 	//B差动电流定值
	R87Alarm.p87RICSet			= RS_pn87RC; 	//C差动电流定值
	R87Alarm.p87RIKSet			= RS_pnK87AR;		//差流越限告警比率

	R87Alarm.pIDA				= &Ida;			//差流A
	R87Alarm.pIDB				= &Idb;			//差流B
	R87Alarm.pIDC				= &Idc;			//差流C

	R87Alarm.AlarmCnt			= R87IALARMTIME;		//差流越限告警时间
	R87Alarm.ARRW				= RW_AR87;			//差流越限告警继电器字
	R87Alarm.wRelRptNum			= R_87AR;		//差流越限告警报告序列号

	R87Alarm.ACnt				= 0;			//A相计数器
	R87Alarm.BCnt				= 0;			//B相计数器
	R87Alarm.CCnt				= 0;			//C相计数器
	R87Alarm.AAlarmFlag			= FALSE;		//A相告警标志
	R87Alarm.BAlarmFlag			= FALSE;		//B相告警标志
	R87Alarm.CAlarmFlag			= FALSE;		//C相告警标志
	R87Alarm.MakeRptFlag		= FALSE;	//作告警报告标志
	R87Alarm.wExcepWordNum      = 16;
}

void CRelayTask::ResetIdHLock(void)
{	
	b68IdH2Flag_A = FALSE;	//A相差流二次闭锁标志	
	b68IdH2Flag_B = FALSE;	//B相差流二次闭锁标志	
	b68IdH2Flag_C = FALSE;	//C相差流二次闭锁标志	
	WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
}

//初始化保护元件结构校验数组
void CRelayTask::InitStructCheck(void) 	
{	
	WORD acc = 0x55aa;	
	StructCheck[0].pstruct = (WORD*)&Rel87U_A;
	StructCheck[0].bigsize = sizeof(Rel87U_A)/2 - Rel87U_A.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&Rel87U_B;
	StructCheck[1].bigsize = sizeof(Rel87U_B)/2 - Rel87U_B.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&Rel87U_C;
	StructCheck[2].bigsize = sizeof(Rel87U_C)/2 - Rel87U_C.wExcepWordNum;
	StructCheck[3].pstruct = (WORD*)&Rel87R_A;
	StructCheck[3].bigsize = sizeof(Rel87R_A)/2 - Rel87R_A.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&Rel87R_B;
	StructCheck[4].bigsize = sizeof(Rel87R_B)/2 - Rel87R_B.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&Rel87R_C;
	StructCheck[5].bigsize = sizeof(Rel87R_C)/2 - Rel87R_C.wExcepWordNum;	
	StructCheck[6].pstruct = (WORD*)&Rel50I_H1;
	StructCheck[6].bigsize = sizeof(Rel50I_H1)/2 - Rel50I_H1.wExcepWordNum;		
	StructCheck[7].pstruct = (WORD*)&Rel50I_H2;
	StructCheck[7].bigsize = sizeof(Rel50I_H2)/2 - Rel50I_H2.wExcepWordNum;		
	StructCheck[8].pstruct = (WORD*)&Rel50I_H3;
	StructCheck[8].bigsize = sizeof(Rel50I_H3)/2 - Rel50I_H3.wExcepWordNum;	
	StructCheck[9].pstruct = (WORD*)&Rel50I_L1;
	StructCheck[9].bigsize = sizeof(Rel50I_L1)/2 - Rel50I_L1.wExcepWordNum;	
	StructCheck[10].pstruct = (WORD*)&Rel50I_L2;
	StructCheck[10].bigsize = sizeof(Rel50I_L2)/2 - Rel50I_L2.wExcepWordNum;	
	StructCheck[11].pstruct = (WORD*)&Rel50I_SH;
	StructCheck[11].bigsize = sizeof(Rel50I_SH)/2 - Rel50I_SH.wExcepWordNum;	
	StructCheck[12].pstruct = (WORD*)&Rel50I_N1;
	StructCheck[12].bigsize = sizeof(Rel50I_N1)/2 - Rel50I_N1.wExcepWordNum;	
	StructCheck[13].pstruct = (WORD*)&Rel50I_N2;
	StructCheck[13].bigsize = sizeof(Rel50I_N2)/2 - Rel50I_N2.wExcepWordNum;
	StructCheck[14].pstruct = (WORD*)&Rel59U_NH;
	StructCheck[14].bigsize = sizeof(Rel59U_NH)/2 - Rel59U_NH.wExcepWordNum;	
	StructCheck[15].pstruct = (WORD*)&Rel59U_NL;
	StructCheck[15].bigsize = sizeof(Rel59U_NL)/2 - Rel59U_NL.wExcepWordNum;	
	StructCheck[16].pstruct = (WORD*)&Rel50OL_1;
	StructCheck[16].bigsize = sizeof(Rel50OL_1)/2 - Rel50OL_1.wExcepWordNum;	
	StructCheck[17].pstruct = (WORD*)&Rel50OL_2;
	StructCheck[17].bigsize = sizeof(Rel50OL_2)/2 - Rel50OL_2.wExcepWordNum;	
	StructCheck[18].pstruct = (WORD*)&Rel50OL_3;
	StructCheck[18].bigsize = sizeof(Rel50OL_3)/2 - Rel50OL_3.wExcepWordNum;	
	StructCheck[19].pstruct = (WORD*)&Rel50I_JX;
	StructCheck[19].bigsize = sizeof(Rel50I_JX)/2 - Rel50I_JX.wExcepWordNum;	
	StructCheck[20].pstruct = (WORD*)&Rel27U;
	StructCheck[20].bigsize = sizeof(Rel27U)/2 - Rel27U.wExcepWordNum;	
	StructCheck[21].pstruct = (WORD*)&Rel49IA;
	StructCheck[21].bigsize = sizeof(Rel49IA)/2 - Rel49IA.wExcepWordNum;	
	StructCheck[22].pstruct = (WORD*)&Rel49IB;
	StructCheck[22].bigsize = sizeof(Rel49IB)/2 - Rel49IB.wExcepWordNum;	
	StructCheck[23].pstruct = (WORD*)&Rel49IC;
	StructCheck[23].bigsize = sizeof(Rel49IC)/2 - Rel49IC.wExcepWordNum;		
	StructCheck[24].pstruct = (WORD*)&R87Alarm;
	StructCheck[24].bigsize = sizeof(R87Alarm)/2 - R87Alarm.wExcepWordNum;		
	StructCheck[25].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[25].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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
void CRelayTask::R68IdH2M(void)	//二次谐波最大相闭锁
{
	if(*CFG_pby68IDH2MAXLK == CFG_STATUS_SET)
	{			
		if((Ida.Mod>*RS_pn87RC)&&(*Rel87R_A.p87RCFG == CFG_STATUS_SET))
		{
			if(Ida2.Mod>(*RS_pnkrh2*Ida.Mod/100))
			{
				b68IdH2Flag_A = TRUE;	//A相差流二次闭锁标志	
				b68IdH2Flag_B = TRUE;	//B相差流二次闭锁标志	
				b68IdH2Flag_C = TRUE;	//C相差流二次闭锁标志	
				WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
				return;
			}
		}	
		if((Idb.Mod>*RS_pn87RC)&&(*Rel87R_B.p87RCFG == CFG_STATUS_SET))
		{
			if(Idb2.Mod>(*RS_pnkrh2*Idb.Mod/100))
			{
				b68IdH2Flag_A = TRUE;	//A相差流二次闭锁标志	
				b68IdH2Flag_B = TRUE;	//B相差流二次闭锁标志	
				b68IdH2Flag_C = TRUE;	//C相差流二次闭锁标志	
				WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
				return;
			}
		}	
		if((Idc.Mod>*RS_pn87RC)&&(*Rel87R_B.p87RCFG == CFG_STATUS_SET))
		{
			if(Idc2.Mod>(*RS_pnkrh2*Idc.Mod/100))
			{
				b68IdH2Flag_A = TRUE;	//A相差流二次闭锁标志	
				b68IdH2Flag_B = TRUE;	//B相差流二次闭锁标志	
				b68IdH2Flag_C = TRUE;	//C相差流二次闭锁标志	
				WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
				WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
				return;
			}
		}
	}
	
}
void CRelayTask::R68IdH2C(void)		//二次谐波综合相闭锁
{
	
	if(*CFG_pby68IDH2MULLK == CFG_STATUS_SET)
	{		
		LONG ID1,ID2;		
		ID1 = MAX(Ida.Mod,Idb.Mod,Idc.Mod);
		ID2 = MAX(Ida2.Mod,Idb2.Mod,Idc2.Mod);
		if(ID2>(*RS_pnkrh2*ID1/100))
		{
			b68IdH2Flag_A = TRUE;	//A相差流二次闭锁标志	
			b68IdH2Flag_B = TRUE;	//B相差流二次闭锁标志	
			b68IdH2Flag_C = TRUE;	//C相差流二次闭锁标志	
			WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
			WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
			WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
			return;
		}
	}
	
}
void CRelayTask::R68IdH2I(void)		//二次谐波分相闭锁
{
	if(*CFG_pby68IDH2SPLLK == CFG_STATUS_SET)
	{
		if((Ida.Mod>*RS_pn87RC)&&(Ida2.Mod>(*RS_pnkrh2*Ida.Mod/100)))
		{
			b68IdH2Flag_A = TRUE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
		}		
		else
		{
			b68IdH2Flag_A = FALSE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
		}
		
		if((Idb.Mod>*RS_pn87RC)&&(Idb2.Mod>(*RS_pnkrh2*Idb.Mod/100)))
		{
			b68IdH2Flag_B = TRUE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
		}			
		else
		{
			b68IdH2Flag_B = FALSE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
		}
		
		if((Idc.Mod>*RS_pn87RC)&&(Idc2.Mod>(*RS_pnkrh2*Idc.Mod/100)))
		{
			b68IdH2Flag_C = TRUE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDCH2,RELAY_ON);
		}		
		else
		{
			b68IdH2Flag_C = FALSE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
		}
	}
}
void CRelayTask::R68IrH2M(void)		//制动电流二次谐波抑制
{	
	if(*CFG_pby68IRH2LK != CFG_STATUS_SET)
	{		
		b68IrH2Flag_A = FALSE;
		b68IrH2Flag_B = FALSE;
		b68IrH2Flag_C = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF);
		WriteRelayWord(RW_LK68IRCH2,RELAY_OFF);
		return;
	}	
	if((Ida.Mod>*RS_pn87RC)&&(Ira2.Mod>(*RS_pnkrh2*Ira.Mod/100)))
	{
		b68IrH2Flag_A = TRUE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_ON);		
	}	
	else
	{
		b68IrH2Flag_A = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
	}
	
	if((Idb.Mod>*RS_pn87RC)&&(Irb2.Mod>(*RS_pnkrh2*Irb.Mod/100)))
	{
		b68IrH2Flag_B = TRUE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_ON);		
	}	
	else
	{
		b68IrH2Flag_B = FALSE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF); 	
	}
	
	if((Idc.Mod>*RS_pn87RC)&&(Irc2.Mod>(*RS_pnkrh2*Irc.Mod/100)))
	{
		b68IrH2Flag_C = TRUE;
		WriteRelayWord(RW_LK68IRCH2,RELAY_ON);		
	}
	else
	{
		b68IrH2Flag_C = FALSE;
		WriteRelayWord(RW_LK68IRCH2,RELAY_OFF); 	
	}
}
void CRelayTask::R68IdHI(void) 	//综合谐波闭锁
{
	
	if(*CFG_pby68IDMULLK != CFG_STATUS_SET)
	{		
		b68IdHMulFlag_A = FALSE;
		b68IdHMulFlag_B = FALSE;
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDAH,RELAY_OFF);	
		WriteRelayWord(RW_LK68IDBH,RELAY_OFF);
		WriteRelayWord(RW_LK68IDCH,RELAY_OFF);
		return;
	}
	
	if((Ida.Mod>*RS_pn87RC)&&((Ida2.Mod+Ida3.Mod)>(*RS_pnkrh*Ida.Mod/100)))
	{
		b68IdHMulFlag_A = TRUE;
		WriteRelayWord(RW_LK68IDAH,RELAY_ON);		
	}	
	else
	{
		b68IdHMulFlag_A = FALSE;
		WriteRelayWord(RW_LK68IDAH,RELAY_OFF);		
	}

	
	if((Idb.Mod>*RS_pn87RC)&&((Idb2.Mod+Idb3.Mod)>(*RS_pnkrh*Idb.Mod/100)))
	{
		b68IdHMulFlag_B = TRUE;
		WriteRelayWord(RW_LK68IDBH,RELAY_ON);		
	}	
	else
	{
		b68IdHMulFlag_B = FALSE;
		WriteRelayWord(RW_LK68IDBH,RELAY_OFF);		
	}

	
	if((Idc.Mod>*RS_pn87RC)&&((Idc2.Mod+Idc3.Mod)>(*RS_pnkrh*Idc.Mod/100)))
	{
		b68IdHMulFlag_C = TRUE;
		WriteRelayWord(RW_LK68IDCH,RELAY_ON);		
	}	
	else
	{
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDCH,RELAY_OFF);		
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

void CRelayTask::ThermalOverLoad()
{		
	if((*CFG_pby49I != CFG_STATUS_SET)||(*SW_pby49I != SW_STATUS_SET))
		return;		
	RelThermalOverLoad(&Rel49IA);	//A相热累加计算
	RelThermalOverLoad(&Rel49IB);	//B相热累加计算
	RelThermalOverLoad(&Rel49IC);	//C相热累加计算
	//热过负荷动作不需要分相，故在作报告的时候只采用A相的报告INF
	if(ReadRelayWord(Rel49IA.R49WarnRW)||ReadRelayWord(Rel49IB.R49WarnRW)||ReadRelayWord(Rel49IC.R49WarnRW))	//告警继电器字置位
	{
		if(m_b49IHaveMakeWarnRpt == FALSE)	//尚未做告警报告
		{
			MakeAlarmRpt(Rel49IA.R49WarnRptNum);	//作告警报告,热过负荷告警
			m_b49IHaveMakeWarnRpt = TRUE;		//置已作告警报告标志
			return;
		}
	}
	if(ReadRelayWord(Rel49IA.R49ActRW)||ReadRelayWord(Rel49IB.R49ActRW)||ReadRelayWord(Rel49IC.R49ActRW))
	{
		if(m_b49IHaveMakeActRpt == FALSE)
		{			
			TAbsTime RptTime;
			WORD	 RptNo;
			ReadAbsTime(&RptTime);
			DB_RelayStart(++g_RptSerialNum,RELAY_INF_NOUSED,&RptTime,FALSE); //向数据库中写入启动报告
			RptNo = g_RptSerialNum;
			MakeActRpt(Rel49IA.R49ActRptNum,RptNo);	
			DB_RelayEnd(RptNo); 			//向数据库中写入结束报
			m_b49IHaveMakeActRpt = TRUE;			
			m_b49IHaveMakeRSTRpt = FALSE;			
		}
		return;  //动作继电器字置位，直接退出
	}
	if((!ReadRelayWord(Rel49IA.R49WarnRW))&&(!ReadRelayWord(Rel49IB.R49WarnRW))&&(!ReadRelayWord(Rel49IC.R49WarnRW)))
	{		
		if((m_b49IHaveMakeRSTRpt == FALSE)&&(m_b49IHaveMakeWarnRpt == TRUE)) //无告警继电器字置位，
		{
			MakeAlarmRpt(Rel49IA.R49RSTRptNum);
			m_b49IHaveMakeRSTRpt = TRUE;
			m_b49IHaveMakeActRpt = FALSE;
			m_b49IHaveMakeWarnRpt = FALSE;
		}
	}
}
//------------------------------------------------------------------------------------------ 
// 功能: 模值启动判别程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK(void)
{
	
	BYTE  pChannel[]={CHANNEL_6533_IA,CHANNEL_6533_IB,CHANNEL_6533_IC,CHANNEL_6533_Ia,CHANNEL_6533_Ib, \
					 CHANNEL_6533_Ic,CHANNEL_6533_I0,CHANNEL_6533_Ijx,CHANNEL_6533_IDA,CHANNEL_6533_IDB, \
					 CHANNEL_6533_IDC};
	BOOL  relayInputFlag = FALSE;
	if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))  //断路器跳闸失败，不进行保护启动判断处理 
	{
		WriteRelayWord(PickUpRW, RELAY_OFF);
		return FALSE;
	}
	if((*Rel87U_A.pRelCFG == CFG_STATUS_SET) && (*Rel87U_A.pRelSW == SW_STATUS_SET))   //差动速断A
	{
		relayInputFlag = TRUE;
		if(Rel87U_A.pRelElec->Mod > *Rel87U_A.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*Rel87U_B.pRelCFG == CFG_STATUS_SET) && (*Rel87U_B.pRelSW == SW_STATUS_SET)) //差动速断B
	{		
		relayInputFlag = TRUE;
		if(Rel87U_B.pRelElec->Mod > *Rel87U_B.pElecSet)
		{			
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel87U_C.pRelCFG == CFG_STATUS_SET) && (*Rel87U_C.pRelSW == SW_STATUS_SET)) //差动速断C
	{		
		relayInputFlag = TRUE;
		if(Rel87U_C.pRelElec->Mod > *Rel87U_C.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	
	if((*Rel87R_A.p87RCFG== CFG_STATUS_SET) && (*Rel87R_A.p87RSW== SW_STATUS_SET)) //比率差动A
	{		
		relayInputFlag = TRUE;
		if(Rel87R_A.pId->Mod> *Rel87R_A.p87RISet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	
	if((*Rel87R_B.p87RCFG== CFG_STATUS_SET) && (*Rel87R_B.p87RSW== SW_STATUS_SET)) //比率差动A
	{		
		relayInputFlag = TRUE;
		if(Rel87R_B.pId->Mod> *Rel87R_B.p87RISet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	
	if((*Rel87R_C.p87RCFG== CFG_STATUS_SET) && (*Rel87R_C.p87RSW== SW_STATUS_SET)) //比率差动A
	{		
		relayInputFlag = TRUE;
		if(Rel87R_C.pId->Mod> *Rel87R_C.p87RISet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}	
	if((*Rel50I_H1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_H1.pRelSW == SW_STATUS_SET)) //高侧过流1段
	{
		relayInputFlag = TRUE;
		if(Rel50I_H1.pRelElec->Mod > *Rel50I_H1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_H2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_H2.pRelSW == SW_STATUS_SET)) //高侧过流2段
	{
		relayInputFlag = TRUE;
		if(Rel50I_H2.pRelElec->Mod > *Rel50I_H2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_H3.pRelCFG == CFG_STATUS_SET) && (*Rel50I_H3.pRelSW == SW_STATUS_SET)) //高侧过流3段
	{
		relayInputFlag = TRUE;
		if(Rel50I_H3.pRelElec->Mod > *Rel50I_H3.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_L1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_L1.pRelSW == SW_STATUS_SET)) //低压侧过流1段
	{
		relayInputFlag = TRUE;
		if(Rel50I_L1.pRelElec->Mod > *Rel50I_L1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_L2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_L2.pRelSW == SW_STATUS_SET)) //低压侧过流2段
	{
		relayInputFlag = TRUE;
		if(Rel50I_L2.pRelElec->Mod > *Rel50I_L2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_SH.pRelCFG == CFG_STATUS_SET) && (*Rel50I_SH.pRelSW == SW_STATUS_SET)) //母充保护
	{
		relayInputFlag = TRUE;
		if(Rel50I_SH.pRelElec->Mod > *Rel50I_SH.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_N1.pRelCFG == CFG_STATUS_SET) && (*Rel50I_N1.pRelSW == SW_STATUS_SET)) //零序过流1段
	{
		relayInputFlag = TRUE;
		if(Rel50I_N1.pRelElec->Mod > *Rel50I_N1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_N2.pRelCFG == CFG_STATUS_SET) && (*Rel50I_N2.pRelSW == SW_STATUS_SET)) //零序过流2段
	{
		relayInputFlag = TRUE;
		if(Rel50I_N2.pRelElec->Mod > *Rel50I_N2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel59U_NH.pRelCFG == CFG_STATUS_SET) && (*Rel59U_NH.pRelSW == SW_STATUS_SET)) //高压侧零序电压过压
	{
		relayInputFlag = TRUE;
		if(Rel59U_NH.pRelElec->Mod > *Rel59U_NH.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel59U_NL.pRelCFG == CFG_STATUS_SET) && (*Rel59U_NL.pRelSW == SW_STATUS_SET)) //高压侧零序电压过压
	{
		relayInputFlag = TRUE;
		if(Rel59U_NL.pRelElec->Mod > *Rel59U_NL.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50OL_1.pRelCFG == CFG_STATUS_SET) && (*Rel50OL_1.pRelSW == SW_STATUS_SET)) //过负荷1段
	{
		relayInputFlag = TRUE;
		if(Rel50OL_1.pRelElec->Mod > *Rel50OL_1.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50OL_2.pRelCFG == CFG_STATUS_SET) && (*Rel50OL_2.pRelSW == SW_STATUS_SET)) //过负荷2段
	{
		relayInputFlag = TRUE;
		if(Rel50OL_2.pRelElec->Mod > *Rel50OL_2.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50OL_3.pRelCFG == CFG_STATUS_SET) && (*Rel50OL_3.pRelSW == SW_STATUS_SET)) //过负荷3段
	{
		relayInputFlag = TRUE;
		if(Rel50OL_3.pRelElec->Mod > *Rel50OL_3.pElecSet)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}
	if((*Rel50I_JX.pRelCFG == CFG_STATUS_SET) && (*Rel50I_JX.pRelSW == SW_STATUS_SET)) //间隙过流保护
	{
		relayInputFlag = TRUE;
		if(Rel50I_JX.pRelElec1->Mod > *Rel50I_JX.pElec1Set)
		{
			WriteRelayWord(PickUpRW, RELAY_ON);
			ReadAbsTime(&PickUpTime);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfAct = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart = g_RptSerialNum;
			return TRUE;			
		}
	}

	//突变量启动部分,有保护投入
	if(relayInputFlag == TRUE)
	{
		 ReadPreSingleData(dwPickDataBuf,pChannel,11,RELAY_SAMP_INTERVAL);
		 for(LONG j=0;j<11;j++)
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

						WriteRelayWord(PickUpRW, RELAY_ON);
						ReadAbsTime(&PickUpTime);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
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
							
							WriteRelayWord(PickUpRW, RELAY_ON);
							ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_TRPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
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
	WORD lpcRelayWord = 0;
	if(FaultOnFlag ==FALSE)   						//尚未进入故障处理
	{		
		RelayPickUpCHK();					//执行启动检测
		if(ReadRelayWord(PickUpRW))
		{			
			ResetAllRelay(TRUE);
			FaultSINum=RELAY_SAMP_INTERVAL;       //保护启动，初始化故障处理采样间隔计数器
			FaultOnFlag=TRUE;
			return;
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
		if(HaveMakeStartRptFlag == FALSE)   //向带故障电量的启动报告中写入故障电量
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart);
			HaveMakeStartRptFlag = TRUE;
		}
		//功率方向的判断
		if((*Rel50I_H1.pRelPDCFG == CFG_STATUS_SET)||(*Rel50I_H2.pRelPDCFG == CFG_STATUS_SET)||(*Rel50I_H3.pRelPDCFG == CFG_STATUS_SET))
		{
			if(wHIPhase == PHASE_A)
			{
				if(!HA_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTH))				//PT断线时，功率方向闭锁功能失效
						HA_PDWithMemoryFlag = PowerDirCheck_90(&UBC,&IA,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFHA,RW_PRHA,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHA, RELAY_OFF);
						WriteRelayWord(RW_PRHA, RELAY_OFF);
					}
				}
			}
			else if(wHIPhase == PHASE_B)
			{
				if(!HB_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTH))				//PT断线时，功率方向闭锁功能失效
						HB_PDWithMemoryFlag = PowerDirCheck_90(&UCA,&IB,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFHB,RW_PRHB,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHB, RELAY_OFF);
						WriteRelayWord(RW_PRHB, RELAY_OFF);
					}
				}
			}
			else if(wHIPhase == PHASE_C)
			{
				if(!HC_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTH))				//PT断线时，功率方向闭锁功能失效
						HC_PDWithMemoryFlag = PowerDirCheck_90(&UAB,&IC,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFHC,RW_PRHC,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFHC, RELAY_OFF);
						WriteRelayWord(RW_PRHC, RELAY_OFF);
					}
				}
			}
		}
		if((*Rel50I_L1.pRelPDCFG == CFG_STATUS_SET)||(*Rel50I_L2.pRelPDCFG == CFG_STATUS_SET))
		{
			if(wLIPhase == PHASE_A)
			{
				if(!LA_PDWithMemoryFlag)
					{
						if(!ReadRelayWord(RW_PTL))				//PT断线时，功率方向闭锁功能失效
							LA_PDWithMemoryFlag = PowerDirCheck_90(&Ubc,&Ia,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFLA,RW_PRLA,0,FALSE);
						else
						{
							WriteRelayWord(RW_PFLA, RELAY_OFF);
							WriteRelayWord(RW_PRLA, RELAY_OFF);
						}
					}
			}
			else if(wLIPhase == PHASE_B)
			{
				if(!LB_PDWithMemoryFlag)
					{
						if(!ReadRelayWord(RW_PTL))				//PT断线时，功率方向闭锁功能失效
							LB_PDWithMemoryFlag = PowerDirCheck_90(&Uca,&Ib,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFLB,RW_PRLB,0,FALSE);
						else
						{
							WriteRelayWord(RW_PFLB, RELAY_OFF);
							WriteRelayWord(RW_PRLB, RELAY_OFF);
						}
					}
			}
			else if(wLIPhase == PHASE_C)
			{
				if(!LC_PDWithMemoryFlag)
				{
					if(!ReadRelayWord(RW_PTL))				//PT断线时，功率方向闭锁功能失效
						LC_PDWithMemoryFlag = PowerDirCheck_90(&Uab,&Ic,dwPDDataBuf,*RS_pnA_50I,*CF_pnUp,RW_PFLC,RW_PRLC,0,FALSE);
					else
					{
						WriteRelayWord(RW_PFLC, RELAY_OFF);
						WriteRelayWord(RW_PRLC, RELAY_OFF);
					}
				}
			}
		}
		ResetIdHLock();
		R68IdH2M();											//二次谐波最大相闭锁
		R68IdH2C();											//二次谐波综合相闭锁
		R68IdH2I();											//二次谐波分相闭锁
		R68IrH2M();											//制动电流二次谐波抑制
		R68IdHI();											//综合谐波抑制

		if(!ReadRelayWord(Rel87U_A.ActRelayWord)&&!ReadRelayWord(Rel87U_B.ActRelayWord)&&!ReadRelayWord(Rel87U_A.ActRelayWord))
		{
			if(!ReadRelayWord(Rel87R_B.ACTRelayWord)&&!ReadRelayWord(Rel87R_C.ACTRelayWord))
				Rel87RI(&Rel87R_A);									//比率差动A
			if(!ReadRelayWord(Rel87R_A.ACTRelayWord)&&!ReadRelayWord(Rel87R_C.ACTRelayWord))
				Rel87RI(&Rel87R_B);									//比率差动B
			if(!ReadRelayWord(Rel87R_B.ACTRelayWord)&&!ReadRelayWord(Rel87R_A.ACTRelayWord))
				Rel87RI(&Rel87R_C);									//比率差动C
		}
		if(!ReadRelayWord(Rel87R_A.ACTRelayWord)&&!ReadRelayWord(Rel87R_B.ACTRelayWord)&&!ReadRelayWord(Rel87R_C.ACTRelayWord))
		{
			if(!ReadRelayWord(Rel87U_B.ActRelayWord)&&!ReadRelayWord(Rel87U_C.ActRelayWord))
				GeneralOverValueRelay(&Rel87U_A);					//速断A
			if(!ReadRelayWord(Rel87U_C.ActRelayWord)&&!ReadRelayWord(Rel87U_A.ActRelayWord))
				GeneralOverValueRelay(&Rel87U_B);					//速断B
			if(!ReadRelayWord(Rel87U_B.ActRelayWord)&&!ReadRelayWord(Rel87U_A.ActRelayWord))
				GeneralOverValueRelay(&Rel87U_C);					//速断3
		}
		
		GeneralOverValueRelay(&Rel50I_H1);				    //高压侧过流1段
		GeneralOverValueRelay(&Rel50I_H2);					//高压侧过流2段
		GeneralOverValueRelay(&Rel50I_H3);					//高压侧过流3段
		GeneralOverValueRelay(&Rel50I_L1);					//低压侧过流1段
		GeneralOverValueRelay(&Rel50I_L2);					//低压侧过流2段
		GeneralOverValueRelay(&Rel50I_SH);					//母充保护
		GeneralOverValueRelay(&Rel50I_N1);					//零流1段
		GeneralOverValueRelay(&Rel50I_N2);					//零流2段
		GeneralOverValueRelay(&Rel59U_NH);					//高压侧零压
		GeneralOverValueRelay(&Rel59U_NL);					//低压侧零压
		GeneralOverValueRelay(&Rel50OL_1);					//过负荷1段	
		GeneralOverValueRelay(&Rel50OL_2);					//过负荷2段
		GeneralOverValueRelay(&Rel50OL_3);					//过负荷3段
		GeneralOverValueRelay_3(&Rel50I_JX);					//间隙过流


		if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))
		{
			if(!m_bHaveMakeTripFailRpt)
			{				
				MakeActRpt(R_52BF,RptSerialNumOfAct);   //作断路器跳闸失败报告
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}
		//有保护发生动作的可能性
		if((ReadRelayWord(Rel87R_A.LoopCtrlRelayWord))||(ReadRelayWord(Rel87R_B.LoopCtrlRelayWord))||(ReadRelayWord(Rel87R_C.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel87U_A.LoopCtrlRelayWord))||(ReadRelayWord(Rel87U_B.LoopCtrlRelayWord))||(ReadRelayWord(Rel87U_C.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50I_H1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_H2.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_H3.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50I_L1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_L2.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_SH.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50I_N1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_N2.LoopCtrlRelayWord))||(ReadRelayWord(Rel59U_NH.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel59U_NL.LoopCtrlRelayWord))||(ReadRelayWord(Rel50OL_1.LoopCtrlRelayWord))||(ReadRelayWord(Rel50OL_2.LoopCtrlRelayWord)) \
			||(ReadRelayWord(Rel50OL_3.LoopCtrlRelayWord))||(ReadRelayWord(Rel50I_JX.LoopCtrlRelayWord)))
		{
			
			if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))			//跳闸失败，清所有标志，作结束报告,整组复归
			{
				WriteRelayWord(PickUpRW, RELAY_OFF);
				FaultCheckOutFlag = FALSE;
				FaultOnFlag = FALSE;
				HaveMakeStartRptFlag= FALSE;
				DB_RelayEnd(RptSerialNumOfAct); //向数据库中写入结束报告				
				m_bHaveMakeTripFailRpt = FALSE;
				return;
			}
			else
			{
				FaultLockTime = dwRelayMSCnt;		//重置故障锁定定时器 
				FaultCheckOutFlag = TRUE;			//有保护循环控制继电器字置位，置检出故障标志
				return;
			}
		}
		//不会有保护动作:1、已经动作过；2、未发生过故障
		else				
		{
			if(FaultCheckOutFlag ==TRUE	)      // 1、故障处理已经结束或曾经可能过
			{
				if(ReadRelayWord(RW_T52BF))
				{
					FaultLockTime = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime>500 )//故障处理结束，无故障时间超过1秒，复位保护启动继电器字，清标志，作结束报告，整组复归
				{	
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;
					DB_RelayEnd(RptSerialNumOfAct);	
					m_bHaveMakeTripFailRpt = FALSE;
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
				if(dwRelayMSCnt -FaultLockTime>1000 )//未发生过故障无故障时间超过0.5秒，复位保护启动继电器字，清标志，作结束报告，整组复归
				{
					WriteRelayWord(PickUpRW, RELAY_OFF);
					FaultCheckOutFlag = FALSE;
					FaultOnFlag = FALSE;
					HaveMakeStartRptFlag = FALSE;					
					m_bFaultLKFlag = FALSE;
					DB_RelayEnd(RptSerialNumOfAct); 
					m_bHaveMakeTripFailRpt = FALSE;
					ResetAllRelay(TRUE);					
					return;					
				}
				else
					return;
			}			
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
	Reset87RI(&Rel87R_A,resetFlag);
	Reset87RI(&Rel87R_B,resetFlag);
	Reset87RI(&Rel87R_C,resetFlag);
	ResetOverValueRelay(&Rel87U_A,resetFlag);
	ResetOverValueRelay(&Rel87U_B,resetFlag);
	ResetOverValueRelay(&Rel87U_C,resetFlag);
	ResetOverValueRelay(&Rel50I_H1,resetFlag);
	ResetOverValueRelay(&Rel50I_H2,resetFlag);
	ResetOverValueRelay(&Rel50I_H3,resetFlag);
	ResetOverValueRelay(&Rel50I_L1,resetFlag);
	ResetOverValueRelay(&Rel50I_L2,resetFlag);
	ResetOverValueRelay(&Rel50I_SH,resetFlag);
	ResetOverValueRelay(&Rel50I_N1,resetFlag);
	ResetOverValueRelay(&Rel50I_N2,resetFlag);
	ResetOverValueRelay(&Rel59U_NH,resetFlag);
	ResetOverValueRelay(&Rel59U_NL,resetFlag);
	ResetOverValueRelay(&Rel50OL_1,resetFlag);
	ResetOverValueRelay(&Rel50OL_2,resetFlag);
	ResetOverValueRelay(&Rel50OL_3,resetFlag);
	ResetOverValueRelay_3(&Rel50I_JX,resetFlag);
	
}
void CRelayTask::Process52BF(void)
{
	if(ReadRelayWord(RW_52BFA)||ReadRelayWord(RW_52BFB))
	{
		m_b52BFFLAG = TRUE;
	}
	if(m_b52BFFLAG)
	{
		if((!ReadRelayWord(RW_52BFA))&&(!ReadRelayWord(RW_52BFB)))
		{
			ResetAllRelay(TRUE);
			ResetUnderValRelay(&Rel27U);
			m_b52BFFLAG = FALSE;
		}
	}
}
void CRelayTask::PTCheck(void)
{
	if(*CFG_pbyPTCK == CFG_STATUS_CLR)
	{
		HPTCheckTimer1 = 0;
		HPTCheckTimer2 = 0;
		HPTCheckTimer3 = 0;
		HPTCheckTimer4 = 0;
		HPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTH))
		{
			MakeAlarmRpt(R_HPTAR_RES);
			WriteRelayWord(RW_PTH,RELAY_OFF);
		}
		
		LPTCheckTimer1 = 0;
		LPTCheckTimer2 = 0;
		LPTCheckTimer3 = 0;
		LPTCheckTimer4 = 0;
		LPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTL))
		{
			MakeAlarmRpt(R_LPTAR_RES);
			WriteRelayWord(RW_PTL,RELAY_OFF);
		}
		return;
	}
	//高压侧PT断线判断
//	if(((UAB.Mod>=*RS_pn74VTV)&&(UBC.Mod>=*RS_pn74VTV)&&(UCA.Mod>=*RS_pn74VTV)&&(U2h.Mod<=U2Set))||(*CFG_pbyHAB == CFG_STATUS_SET))
	if((UAB.Mod>=*RS_pn74VTV)&&(UBC.Mod>=*RS_pn74VTV)&&(UCA.Mod>=*RS_pn74VTV)&&(U2h.Mod<=U2Set))

	{
		HPTCheckTimer1 = 0;
		HPTCheckTimer2 = 0;
		HPTCheckTimer3 = 0;
		HPTCheckTimer4 = 0;
		HPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTH))
		{
			MakeAlarmRpt(R_HPTAR_RES);
			WriteRelayWord(RW_PTH,RELAY_OFF);
		}
	}
	
	TRelElecValPar UL;
	UL = ElecMax(Uab,Ubc,Uca);
	if((UAB.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC)&&(UL.Mod>*RS_pn74VTV))
		HPTCheckTimer1++;
	else
		HPTCheckTimer1=0;
	
	if((UBC.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC)&&(UL.Mod>*RS_pn74VTV))
		HPTCheckTimer2++;
	else
		HPTCheckTimer2=0;
	
	if((UCA.Mod<*RS_pn74VTV)&&(IHmax.Mod>*RS_pn74VTC)&&(UL.Mod>*RS_pn74VTV))
		HPTCheckTimer3++;
	else
		HPTCheckTimer3=0;
	
	if(U2h.Mod>U2Set)
		HPTCheckTimer4++;
	else
		HPTCheckTimer4=0;
		
	if(((HPTCheckTimer1*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer2*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer3*m_PTCheckTimer)>*RS_pn74VTD) \
		||((HPTCheckTimer4*m_PTCheckTimer)>*RS_pn74VTD))    //yanxs 2012-12-03 更改PT断线时间判断
	{
		if(!HPTMakeReportFlag)
		{
			MakeAlarmRpt(R_HPTAR);
			HPTMakeReportFlag = TRUE;
			WriteRelayWord(RW_PTH,RELAY_ON);
		}
	}
	//低压侧PT断线判断
	
	if(((Uab.Mod>=*RS_pn74VTV)&&(Ubc.Mod>=*RS_pn74VTV)&&(Uca.Mod>=*RS_pn74VTV)&&(U2l.Mod<=U2Set))||(ReadRelayWord(RW_52B_A)))
	{
		LPTCheckTimer1 = 0;
		LPTCheckTimer2 = 0;
		LPTCheckTimer3 = 0;
		LPTCheckTimer4 = 0;
		LPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTL))
		{
			MakeAlarmRpt(R_LPTAR_RES);
			WriteRelayWord(RW_PTL,RELAY_OFF);
		}
	}
	TRelElecValPar UH;
	UH = ElecMax(UAB,UBC,UCA);
	if((UH.Mod>*RS_pn74VTV)&&(ReadRelayWord(RW_52A_A))&&(ReadRelayWord(RW_52A_B)))//高压侧有压且高压侧断路器处于合位
	{
		if(Uab.Mod<*RS_pn74VTV)
			LPTCheckTimer1++;
		else
			LPTCheckTimer1=0;
		
		if(Ubc.Mod<*RS_pn74VTV)
			LPTCheckTimer2++;
		else
			LPTCheckTimer2=0;
		
		if(Uca.Mod<*RS_pn74VTV)
			LPTCheckTimer3++;
		else
			LPTCheckTimer3=0;
		
		if(U2l.Mod>U2Set)
			LPTCheckTimer4++;
		else
			LPTCheckTimer4=0;
			
		if(((LPTCheckTimer1*m_PTCheckTimer)>*RS_pn74VTD) \
			||((LPTCheckTimer2*m_PTCheckTimer)>*RS_pn74VTD) \
			||((LPTCheckTimer3*m_PTCheckTimer)>*RS_pn74VTD) \
			||((LPTCheckTimer4*m_PTCheckTimer)>*RS_pn74VTD))          //yanxs 2012-12-03 更改PT断线时间判断
		{
			if(!LPTMakeReportFlag)
			{
				MakeAlarmRpt(R_LPTAR);
				LPTMakeReportFlag = TRUE;
				WriteRelayWord(RW_PTL,RELAY_ON);
			}
		}
	}
	else
	{
		LPTCheckTimer1 = 0;
		LPTCheckTimer2 = 0;
		LPTCheckTimer3 = 0;
		LPTCheckTimer4 = 0;
		LPTMakeReportFlag = FALSE;
		if(ReadRelayWord(RW_PTL))
		{
			MakeAlarmRpt(R_LPTAR_RES);
			WriteRelayWord(RW_PTL,RELAY_OFF);
		}
	}

	
}


