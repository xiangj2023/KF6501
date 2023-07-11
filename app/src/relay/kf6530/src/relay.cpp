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
*           6530变压器保护算法代码                                                                       
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
*      yanzh             2010/06/07    初始创建                         
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

extern BYTE byRelayWordValue[];
extern TSysConfigTable	*G_pCfgTable;
extern g_hDbaseTaskID;
extern dwRelayDataPtr;	//AD当前写指针
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
extern BYTE g_RptSerialNum;
extern BOOL RelayCheckErr;			//保护自检出错
extern BOOL RelayCheckInitOK;		//保护自检结构体已初始化结束标志
extern DWORD dwADResumeCnt;	//AD功能恢复计数，从异常到正常后需保证正常采样一个完整周波数据
extern BYTE *g_MaintSW;

BOOL RelayTaskInitOK = FALSE;
DWORD dwRelayTaskID;

DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
TStructCheck StructCheck[CHECK_NUM];					//结构体校验数组

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
/*
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}*/
	//获取数据库保护测量值数据指针，以加快操作速度
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	InitAllRelayMem();				//初始化保护内部变量
	InitRealy();					//初始化保护元件
	InitSet();
	InitDCRecInfo();
	

	RelCFGCLR = CFG_STATUS_CLR;		//保护退出
	RelCFGSET = CFG_STATUS_SET;		//保护投入
	
	g_RptSerialNum = 0;				//系统的报告序列号
	RptSerialNumOfAct =0;			//系统的保护动作报告序列号
	RptSerialNumOfStart=0;			//系统的带故障电量的启动报告序列号	
	FaultSINum = 0;					//采样间隔计数器	
	
	PickUpRW = RW_PICKUP;			//模值突变量启动继电器字
	FaultOnFlag = FALSE;			//进入故障处理标志
	HaveMakeStartRptFlag = FALSE;	//已作带故障电量启动报告标志
	m_bFaultLKFlag = FALSE;			//故障起始时刻锁定标志
	FaultCheckOutFlag = FALSE;		//检出故障标志
	m_bLastRecStatusFlag = FALSE;	//故障录波启动上升沿判断标志
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
	m_bHaveMakeTripFailRpt = FALSE;
	
	m_b52BFFlag = FALSE;
	
	PickCheck = 0;					//突变量启动判断连续点计数器
	for(LONG i=0;i<3;i++)			//突变量启动判断两次任务间连续点计数器
	{		
		PickCheckNext[i] = 0;	
		lPickUpValueArray[i]=0;
	}
	for(LONG i=0;i<6;i++)			//数字录波组字计算结果数组
	{
		DCCaluResult[i]=0;
	}
	for(LONG i=0;i<5;i++)
	{
		IDADrift[i]=0;
		IDBDrift[i]=0;
		IDCDrift[i]=0;
	}		
	//保护数据读取通道掩码
	dwChanDataMask = ( (0x01<<CHANNEL_6530_UA)|(0x01<<CHANNEL_6530_UB)|(0x01<<CHANNEL_6530_UC)|
						(0x01<<CHANNEL_6530_IHA)|(0x01<<CHANNEL_6530_IHB)|(0x01<<CHANNEL_6530_IHC)| \
						(0x01<<CHANNEL_6530_IL1)|(0x01<<CHANNEL_6530_IL2)|(0x01<<CHANNEL_6530_IL3)| \
						(0x01<<CHANNEL_6530_IL4));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	dwComposeDataMask = ((0x01<<CHANNEL_6530_IDA)|(0x01<<CHANNEL_6530_IRA)|(0x01<<CHANNEL_6530_IDB)| \
		(0x01<<CHANNEL_6530_IRB)|(0x01<<CHANNEL_6530_IDC)|(0x01<<CHANNEL_6530_IRC));
	//初始化读取保护数据临时缓冲区
	//保护数据缓冲区
	
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*10);	
	//合成数据回写缓冲区
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*6);
	//突变量启动历史数据缓冲区
	dwPickDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*3);
	//合成计算用数据缓冲区
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*6);
	//变压器低压侧电流数据缓冲区
	dwLIDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*3);	
		
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(DWORD)*READ_POINT_NUM*10);

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*6);


	ASSERT(dwPickDataBuf != NULL);
	if(dwPickDataBuf == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*3);
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(DWORD)*READ_POINT_NUM*6);

	ASSERT(dwLIDataBuf != NULL);
	if(dwLIDataBuf == NULL)
	{
		LogError("CRelayTask,dwLIDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwLIDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*3);
	//下面是初始化逻辑方程部分
	m_bIsLogicOk = TRUE;
	if (!InitLogicParseModule())
	{
		LogError("InitLogicParseModule",FILE_LINE,"Err!");
		m_bIsLogicOk = FALSE;
	}
	m_bIsLogicOk &= ResetLogicParseModule();
	
	//初始化开出自检为正常状态
	for(LONG i=0; i<BO_CHECK_BUF_LEN; i++)
	{
		dwBoInvalidStatus[i] = 0;
		dwBoBreakStatus[i] = 0;
	}
	//首先进行一遍开出失效自检检查
	for(LONG i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		BoInvalidCheck();

	pMatrix = NULL;
	
	g_MaintSW = SW_pbyMAINT;
	RelayTaskInitOK = TRUE;
	m_bLogicChanged = FALSE;
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

}
/************************************************************************************************
*功能: 初始化保护元件
*参数: 无
*返回: 无
*备注:
************************************************************************************************/

void  CRelayTask::InitRealy(void)
{
	 InitR87U_A();
	 InitR87U_B();
	 InitR87U_C();
	 InitR87R_A();
	 InitR87R_B();
	 InitR87R_C();
	 InitR87IAlarm();
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
//	MCF_GPIO_PODR_TIMER |= 0x02;		//高电平
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
	
	static WORD wInvalidCheckCnt;		//开出自检累加计数器
	if(DAE_GetRunningFlags(SS_EEPROM_DATA_ERR)||DAE_GetRunningFlags(SS_FILE_DIFFRELAYDATA_ERR))
	{
		RelayCheckErr = TRUE;
		return;
	}
	if(!m_bLogicChanged)
		BoBreakCheck();

	ReadSampleData(dwSampBuf,dwChanDataMask,0);	
	if(FALSE == WriteComposeBuf())
		return;	
	CaluUI(); 	
	CaluDrift();
	R87IAlarm(&R87Alarm);
	//首先进行开出自检,每次保护任务检一路开出
	//故障循环处理
	FalutProcess();								
	Process52BF();
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
BOOL CRelayTask::WriteComposeBuf()
{
	LONG tempA1,tempA2;
	LONG tempB1,tempB2;
	LONG tempC1,tempC2;

	if((*RS_pnTRTY>9)||(*RS_pnTRTY<0))
	{
		RelayCheckErr = TRUE;
		return FALSE;
	}

	if(*RS_pnLINUM == 1)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwLIDataBuf[i] = dwSampBuf[READ_POINT_NUM*6+i];
			dwLIDataBuf[READ_POINT_NUM+i] = 0;
			dwLIDataBuf[READ_POINT_NUM*2+i] = 0;
		}
	}
	if(*RS_pnLINUM == 2)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwLIDataBuf[i] = dwSampBuf[READ_POINT_NUM*6+i];
			dwLIDataBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*7+i];			
			dwLIDataBuf[READ_POINT_NUM*2+i] = 0;
		}
	}
	if(*RS_pnLINUM == 3)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwLIDataBuf[i] = dwSampBuf[READ_POINT_NUM*6+i];
			dwLIDataBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*7+i];
			dwLIDataBuf[READ_POINT_NUM*2+i] = dwSampBuf[READ_POINT_NUM*8+i];
		}
	}
	if(*RS_pnLINUM == 4)
	{
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			dwLIDataBuf[i] = dwSampBuf[READ_POINT_NUM*6+i]-dwSampBuf[READ_POINT_NUM*7+i];
			dwLIDataBuf[READ_POINT_NUM+i] = dwSampBuf[READ_POINT_NUM*8+i]-dwSampBuf[READ_POINT_NUM*9+i];;
			dwLIDataBuf[READ_POINT_NUM*2+i] = 0;
		}
	}
	if((*RS_pnTRTY==VV3_T)||(*RS_pnTRTY==VX_T))
		ptrMartix = MartixVV3[*RS_pnBph];
	else if(*RS_pnTRTY==VV1_T)
		ptrMartix = MartixVV1[*RS_pnBph];
	else
		ptrMartix = MatrixCoef[*RS_pnTRTY];

	

	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		// 三相VV、单相VV、VX、单相变以及高压侧CT角接时，直接取采样值为高压侧电流
		if((*RS_pnTRTY==VV3_T)||(*RS_pnTRTY==VV1_T)||(*RS_pnTRTY==VX_T)||(*RS_pnTRTY==SIGNP_T)||(*RS_pnCT1CON==CT_D))
		{
			tempA1 = dwSampBuf[READ_POINT_NUM*3+i];
			tempB1 = dwSampBuf[READ_POINT_NUM*4+i];
			tempC1 = dwSampBuf[READ_POINT_NUM*5+i];
		}
		else //其他变压器且为高压侧CT星接时需做运算
		{
			tempA1 = dwSampBuf[READ_POINT_NUM*3+i] - dwSampBuf[READ_POINT_NUM*4+i];
			tempB1 = dwSampBuf[READ_POINT_NUM*4+i] - dwSampBuf[READ_POINT_NUM*5+i];
			tempC1 = dwSampBuf[READ_POINT_NUM*5+i] - dwSampBuf[READ_POINT_NUM*3+i];
		}

		
		tempA2 = ((*RS_pnKba)*ptrMartix[0]*dwLIDataBuf[i]+ \
			(*RS_pnKbb)*ptrMartix[1]*dwLIDataBuf[READ_POINT_NUM+i]+(*RS_pnKbc)*ptrMartix[2]*dwLIDataBuf[READ_POINT_NUM*2+i])/100000;
		//A相差流采样点
		dwComposeBuf[i] = tempA1+tempA2; 
		//A相制动电流采样点 
		dwComposeBuf[READ_POINT_NUM+i]=(tempA1-tempA2)/2;		
		
		tempB2 = ((*RS_pnKba)*ptrMartix[3]*dwLIDataBuf[i]+ \
			(*RS_pnKbb)*ptrMartix[4]*dwLIDataBuf[READ_POINT_NUM+i]+(*RS_pnKbc)*ptrMartix[5]*dwLIDataBuf[READ_POINT_NUM*2+i])/100000; 
		//B相差流采样点
		dwComposeBuf[READ_POINT_NUM*2+i] = tempB1+tempB2;
		//B相制动电流采样点 	
		dwComposeBuf[READ_POINT_NUM*3+i] = (tempB1-tempB2)/2;			
		
		tempC2 = ((*RS_pnKba)*ptrMartix[6]*dwLIDataBuf[i]+ \
			(*RS_pnKbb)*ptrMartix[7]*dwLIDataBuf[READ_POINT_NUM+i]+(*RS_pnKbc)*ptrMartix[8]*dwLIDataBuf[READ_POINT_NUM*2+i])/100000; 
		//C相差流采样点 
		dwComposeBuf[READ_POINT_NUM*4+i] = tempC1+tempC2;	
		//C相制动电流采样点 	
		dwComposeBuf[READ_POINT_NUM*5+i] = (tempC1-tempC2)/2;
	}


/*

	if((*RS_pnTRTY==VV3_T)||(*RS_pnTRTY==VV1_T))
	{
		if(*RS_pnTRTY==VV3_T)
			pMatrix = MartixVV3;
		if(*RS_pnTRTY==VV1_T)
			pMatrix = MartixVV1;
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			tempA1 = dwSampBuf[READ_POINT_NUM*3+i];
			tempB1 = dwSampBuf[READ_POINT_NUM*4+i];
			tempC1 = dwSampBuf[READ_POINT_NUM*5+i];


			tempA2 = (*RS_pnKba)*(pMatrix[*RS_pnBph][0]*dwSampBuf[READ_POINT_NUM*6+i]+ \
				pMatrix[*RS_pnBph][1]*dwSampBuf[READ_POINT_NUM*7+i]+pMatrix[*RS_pnBph][2]*dwSampBuf[READ_POINT_NUM*8+i])/100000;
			//A相差流采样点
			dwComposeBuf[i] = tempA1+tempA2; 
			//A相制动电流采样点	
			dwComposeBuf[READ_POINT_NUM+i]=(tempA1-tempA2)/2;		
			
			tempB2 = (*RS_pnKbb)*(pMatrix[*RS_pnBph][3]*dwSampBuf[READ_POINT_NUM*6+i]+ \
				pMatrix[*RS_pnBph][4]*dwSampBuf[READ_POINT_NUM*7+i]+pMatrix[*RS_pnBph][5]*dwSampBuf[READ_POINT_NUM*8+i])/100000; 
			//B相差流采样点
			dwComposeBuf[READ_POINT_NUM*2+i] = tempB1+tempB2;
			//B相制动电流采样点		
			dwComposeBuf[READ_POINT_NUM*3+i] = (tempB1-tempB2)/2;			
			
			tempC2 = (*RS_pnKbc)*(pMatrix[*RS_pnBph][6]*dwSampBuf[READ_POINT_NUM*6+i]+ \
				pMatrix[*RS_pnBph][7]*dwSampBuf[READ_POINT_NUM*7+i]+pMatrix[*RS_pnBph][8]*dwSampBuf[READ_POINT_NUM*8+i])/100000; 
			//C相差流采样点 
			dwComposeBuf[READ_POINT_NUM*4+i] = tempC1+tempC2;	
			//C相制动电流采样点		
			dwComposeBuf[READ_POINT_NUM*5+i] = (tempC1-tempC2)/2;
			
		}
	}
	else if(*RS_pnTRTY==VX_T)
	{		
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{
			tempA1 = dwSampBuf[READ_POINT_NUM*3+i];
			tempB1 = dwSampBuf[READ_POINT_NUM*4+i];
			tempC1 = dwSampBuf[READ_POINT_NUM*5+i];

			tempA2 = (*RS_pnKba)*(MartixAT2[*RS_pnBph][0]*dwSampBuf[READ_POINT_NUM*6+i]-MartixAT2[*RS_pnBph][0]*dwSampBuf[READ_POINT_NUM*7+i] \
				+MartixAT2[*RS_pnBph][1]*dwSampBuf[READ_POINT_NUM*8+i]-MartixAT2[*RS_pnBph][1]*dwSampBuf[READ_POINT_NUM*9+i])/100000;
			dwComposeBuf[i] = tempA1+tempA2; 						//A相差流采样点
			dwComposeBuf[READ_POINT_NUM+i]=(tempA1-tempA2)/2;		//A相制动电流采样点		
			
			tempB2 = (*RS_pnKbb)*(MartixAT2[*RS_pnBph][2]*dwSampBuf[READ_POINT_NUM*6+i]-MartixAT2[*RS_pnBph][2]*dwSampBuf[READ_POINT_NUM*7+i] \
				+MartixAT2[*RS_pnBph][3]*dwSampBuf[READ_POINT_NUM*8+i]-MartixAT2[*RS_pnBph][3]*dwSampBuf[READ_POINT_NUM*9+i])/100000; 
			dwComposeBuf[READ_POINT_NUM*2+i] = tempB1+tempB2;		//B相差流采样点
			dwComposeBuf[READ_POINT_NUM*3+i] = (tempB1-tempB2)/2;	//B相制动电流采样点				
			
			tempC2 = (*RS_pnKbc)*(MartixAT2[*RS_pnBph][4]*dwSampBuf[READ_POINT_NUM*6+i]-MartixAT2[*RS_pnBph][4]*dwSampBuf[READ_POINT_NUM*7+i] \
				+MartixAT2[*RS_pnBph][5]*dwSampBuf[READ_POINT_NUM*8+i]-MartixAT2[*RS_pnBph][5]*dwSampBuf[READ_POINT_NUM*9+i])/100000; 
			dwComposeBuf[READ_POINT_NUM*4+i] = tempC1+tempC2;		//C相差流采样点 
			dwComposeBuf[READ_POINT_NUM*5+i] = (tempC1-tempC2)/2;	//C相制动电流采样点	
		}
	}
	else
	{	
		for(LONG i=0;i<READ_POINT_NUM;i++)
		{			
			if((*RS_pnCT1CON==CT_D)||(*RS_pnTRTY==SIGNP_T))  //高侧CT星接或者变压器接线形式为单相变、V/V变
			{			
				tempA1 = dwSampBuf[READ_POINT_NUM*3+i];
				tempB1 = dwSampBuf[READ_POINT_NUM*4+i];
				tempC1 = dwSampBuf[READ_POINT_NUM*5+i];
			}
			else
			{			
				tempA1 = dwSampBuf[READ_POINT_NUM*3+i] - dwSampBuf[READ_POINT_NUM*4+i];
				tempB1 = dwSampBuf[READ_POINT_NUM*4+i] - dwSampBuf[READ_POINT_NUM*5+i];
				tempC1 = dwSampBuf[READ_POINT_NUM*5+i] - dwSampBuf[READ_POINT_NUM*3+i];
			}

			
			tempA2 = (*RS_pnKba)*(MatrixCoef[*RS_pnTRTY][0]*dwSampBuf[READ_POINT_NUM*6+i]+ \
				MatrixCoef[*RS_pnTRTY][1]*dwSampBuf[READ_POINT_NUM*7+i]+MatrixCoef[*RS_pnTRTY][2]*dwSampBuf[READ_POINT_NUM*8+i])/100000;
			//A相差流采样点
			dwComposeBuf[i] = tempA1+tempA2; 
			//A相制动电流采样点	
			dwComposeBuf[READ_POINT_NUM+i]=(tempA1-tempA2)/2;		
		
			tempB2 = (*RS_pnKbb)*(MatrixCoef[*RS_pnTRTY][3]*dwSampBuf[READ_POINT_NUM*6+i]+ \
				MatrixCoef[*RS_pnTRTY][4]*dwSampBuf[READ_POINT_NUM*7+i]+MatrixCoef[*RS_pnTRTY][5]*dwSampBuf[READ_POINT_NUM*8+i])/100000; 
			//B相差流采样点
			dwComposeBuf[READ_POINT_NUM*2+i] = tempB1+tempB2;
			//B相制动电流采样点		
			dwComposeBuf[READ_POINT_NUM*3+i] = (tempB1-tempB2)/2;			
			
			tempC2 = (*RS_pnKbc)*(MatrixCoef[*RS_pnTRTY][6]*dwSampBuf[READ_POINT_NUM*6+i]+ \
				MatrixCoef[*RS_pnTRTY][7]*dwSampBuf[READ_POINT_NUM*7+i]+MatrixCoef[*RS_pnTRTY][8]*dwSampBuf[READ_POINT_NUM*8+i])/100000; 
			//C相差流采样点 
			dwComposeBuf[READ_POINT_NUM*4+i] = tempC1+tempC2;	
			//C相制动电流采样点		
			dwComposeBuf[READ_POINT_NUM*5+i] = (tempC1-tempC2)/2;
		}
	}*/
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
	{
		dwRecBuf[i] = dwComposeBuf[READ_POINT_NUM-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL+i] = dwComposeBuf[READ_POINT_NUM*2-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*2+i] = dwComposeBuf[READ_POINT_NUM*3-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*3+i] = dwComposeBuf[READ_POINT_NUM*4-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*4+i] = dwComposeBuf[READ_POINT_NUM*5-RELAY_SAMP_INTERVAL+i];
		dwRecBuf[RELAY_SAMP_INTERVAL*5+i] = dwComposeBuf[READ_POINT_NUM*6-RELAY_SAMP_INTERVAL+i];
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

void CRelayTask::CaluUI(void)
{	
	//UA
	CaluBaseFourier_S15(dwSampBuf,&UA);
	CaluModValue(&UA); 
	*RM_pnUA= UA.Mod; 									  
	DB_LogWriteRM(RM_UA, *RM_pnUA);
	if(UA.Mod>(*AS_pn59UA))
	{				
		WriteRelayWord(RW_59UA, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UA, RELAY_OFF);
	}
	if(UA.Mod>(*AS_pn27UA))
	{
		WriteRelayWord(RW_27UA, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UA, RELAY_ON);
	}
	
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&UB);
	CaluModValue(&UB); 
	*RM_pnUB= UB.Mod; 									  
	DB_LogWriteRM(RM_UB, *RM_pnUB);
	if(UB.Mod>(*AS_pn59UB))
	{				
		WriteRelayWord(RW_59UB, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UB, RELAY_OFF);
	}
	if(UB.Mod>(*AS_pn27UB))
	{
		WriteRelayWord(RW_27UB, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UB, RELAY_ON);
	}
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&UC);
	CaluModValue(&UC); 
	*RM_pnUC= UC.Mod; 									  
	DB_LogWriteRM(RM_UC, *RM_pnUC);
	if(UC.Mod>(*AS_pn59UC))
	{				
		WriteRelayWord(RW_59UC, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59UC, RELAY_OFF);
	}
	if(UC.Mod>(*AS_pn27UC))
	{
		WriteRelayWord(RW_27UC, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27UC, RELAY_ON);
	}




	LONG angle;
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&IHA);
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
	CaluAngle(&IHA,AngleTab);
	*RM_pnAIHA=0;
	DB_LogWriteRM(RM_AIHA, *RM_pnAIHA);
	//IHB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&IHB);
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
	CaluAngle(&IHB,AngleTab);
	angle = IHB.Angle-IHA.Angle;
	if(angle>0)
		*RM_pnAIHB = angle;
	else
		*RM_pnAIHB = angle+36000;
	DB_LogWriteRM(RM_AIHB, *RM_pnAIHB);
	//IHC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&IHC);
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
	CaluAngle(&IHC,AngleTab);
	angle = IHC.Angle-IHA.Angle;
	if(angle>0)
		*RM_pnAIHC = angle;
	else
		*RM_pnAIHC = angle+36000;
	DB_LogWriteRM(RM_AIHC, *RM_pnAIHC);
	//ILA
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&IL1);
	CaluModValue(&IL1); 
	*RM_pnIL1= IL1.Mod; 									  
	DB_LogWriteRM(RM_IL1, *RM_pnIL1);
	if(IL1.Mod>(*AS_pn50L1C))
	{				
		WriteRelayWord(RW_50L1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50L1, RELAY_OFF);
	}
	if(IL1.Mod>(*AS_pn37L1C))
	{
		WriteRelayWord(RW_37L1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37L1, RELAY_ON);
	}		
	CaluAngle(&IL1,AngleTab);
	angle = IL1.Angle-IHA.Angle;
	if(angle>0)
		*RM_pnAIL1 = angle;
	else
		*RM_pnAIL1 = angle+36000;
	DB_LogWriteRM(RM_AIL1, *RM_pnAIL1);
	//ILB
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&IL2);
	CaluModValue(&IL2); 
	*RM_pnIL2= IL2.Mod; 									  
	DB_LogWriteRM(RM_IL2, *RM_pnIL2);
	if(IL2.Mod>(*AS_pn50L2C))
	{				
		WriteRelayWord(RW_50L2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50L2, RELAY_OFF);
	}
	if(IL2.Mod>(*AS_pn37L2C))
	{
		WriteRelayWord(RW_37L2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37L2, RELAY_ON);
	}	
	CaluAngle(&IL2,AngleTab);
	angle = IL2.Angle-IHA.Angle;
	if(angle>0)
		*RM_pnAIL2 = angle;
	else
		*RM_pnAIL2 = angle+36000;
	DB_LogWriteRM(RM_AIL2, *RM_pnAIL2);
	//ILC
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&IL3);
	CaluModValue(&IL3); 
	*RM_pnIL3= IL3.Mod; 									  
	DB_LogWriteRM(RM_IL3, *RM_pnIL3);
	if(IL3.Mod>(*AS_pn50L3C))
	{				
		WriteRelayWord(RW_50L3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50L3, RELAY_OFF);
	}
	if(IL3.Mod>(*AS_pn37L3C))
	{
		WriteRelayWord(RW_37L3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37L3, RELAY_ON);
	}
	CaluAngle(&IL3,AngleTab);
	angle = IL3.Angle-IHA.Angle;
	if(angle>0)
		*RM_pnAIL3 = angle;
	else
		*RM_pnAIL3 = angle+36000;
	DB_LogWriteRM(RM_AIL3, *RM_pnAIL3);
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&IL4);
	CaluModValue(&IL4); 
	*RM_pnIL4= IL4.Mod; 									  
	DB_LogWriteRM(RM_IL4, *RM_pnIL4);
	if(IL4.Mod>(*AS_pn50L4C))
	{				
		WriteRelayWord(RW_50L4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50L4, RELAY_OFF);
	}
	if(IL4.Mod>(*AS_pn37L4C))
	{
		WriteRelayWord(RW_37L4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37L4, RELAY_ON);
	}
	CaluAngle(&IL4,AngleTab);
	angle = IL4.Angle-IHA.Angle;
	if(angle>0)
		*RM_pnAIL4 = angle;
	else
		*RM_pnAIL4 = angle+36000;
	DB_LogWriteRM(RM_AIL4, *RM_pnAIL4);
	//IDA
	CaluBaseFourier_S15(dwComposeBuf,&IDA);
	CaluModValue(&IDA);	
	*RM_pnIDA= IDA.Mod;                                       
	DB_LogWriteRM(RM_IDA, *RM_pnIDA);
	//IRA
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&IRA);
	CaluModValue(&IRA);	
	*RM_pnIRA= IRA.Mod;                                       
	DB_LogWriteRM(RM_IRA, *RM_pnIRA);
	//IDB
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*2,&IDB);
	CaluModValue(&IDB);	
	*RM_pnIDB= IDB.Mod;                                       
	DB_LogWriteRM(RM_IDB, *RM_pnIDB);
	//IRB
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*3,&IRB);
	CaluModValue(&IRB);	
	*RM_pnIRB= IRB.Mod;                                       
	DB_LogWriteRM(RM_IRB, *RM_pnIRB);
	//IDC
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*4,&IDC);
	CaluModValue(&IDC);	
	*RM_pnIDC= IDC.Mod;                                       
	DB_LogWriteRM(RM_IDC, *RM_pnIDC);
	//IRC
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM*5,&IRC);
	CaluModValue(&IRC);	
	*RM_pnIRC= IRC.Mod;                                       
	DB_LogWriteRM(RM_IRC, *RM_pnIRC);
	//IDA2
	CaluSecondFourier(dwComposeBuf,&IDA2);
	CaluModValue(&IDA2);
	IDA2.Mod = IDA2.Mod*1010/1000;
	*RM_pnIDA2= IDA2.Mod;                                       
	DB_LogWriteRM(RM_IDA2, *RM_pnIDA2);
	//IDA3
	CaluThirdFourier(dwComposeBuf,&IDA3);
	CaluModValue(&IDA3);	
	IDA3.Mod = IDA3.Mod*1031/1000;
	*RM_pnIDA3= IDA3.Mod;                                       
	DB_LogWriteRM(RM_IDA3, *RM_pnIDA3);
	//IDA5
	CaluFifthFourier(dwComposeBuf,&IDA5);
	CaluModValue(&IDA5);	
	IDA5.Mod = IDA5.Mod*1087/1000;
	*RM_pnIDA5= IDA5.Mod;                                       
	DB_LogWriteRM(RM_IDA5, *RM_pnIDA5);
	//IRA2
	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM,&IRA2);
	CaluModValue(&IRA2);	
	IRA2.Mod = IRA2.Mod*1010/1000;
	*RM_pnIRA2= IRA2.Mod;                                       
	DB_LogWriteRM(RM_IRA2, *RM_pnIRA2);
	//IDB2
	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*2,&IDB2);
	CaluModValue(&IDB2);	
	IDB2.Mod = IDB2.Mod*1010/1000;
	*RM_pnIDB2= IDB2.Mod;                                       
	DB_LogWriteRM(RM_IDB2, *RM_pnIDB2);
	//IDB3
	CaluThirdFourier(dwComposeBuf+READ_POINT_NUM*2,&IDB3);
	CaluModValue(&IDB3);	
	IDB3.Mod = IDB3.Mod*1031/1000;
	*RM_pnIDB3= IDB3.Mod;                                       
	DB_LogWriteRM(RM_IDB3, *RM_pnIDB3);
	//IDB5
	CaluFifthFourier(dwComposeBuf+READ_POINT_NUM*2,&IDB5);
	CaluModValue(&IDB5);	
	IDB5.Mod = IDB5.Mod*1087/1000;
	*RM_pnIDB5= IDB5.Mod;                                       
	DB_LogWriteRM(RM_IDB5, *RM_pnIDB5);
	//IRB2
	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*3,&IRB2);
	CaluModValue(&IRB2);	
	IRB2.Mod = IRB2.Mod*1010/1000;
	*RM_pnIRB2= IRB2.Mod;                                       
	DB_LogWriteRM(RM_IRB2, *RM_pnIRB2);
	//IDC2
	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*4,&IDC2);
	CaluModValue(&IDC2);	
	IDC2.Mod = IDC2.Mod*1010/1000;
	*RM_pnIDC2= IDC2.Mod;                                       
	DB_LogWriteRM(RM_IDC2, *RM_pnIDC2);
	//IDC3
	CaluThirdFourier(dwComposeBuf+READ_POINT_NUM*4,&IDC3);
	CaluModValue(&IDC3);
	IDC3.Mod = IDC3.Mod*1031/1000;
	*RM_pnIDC3= IDC3.Mod;                                       
	DB_LogWriteRM(RM_IDC3, *RM_pnIDC3);
	//IDC5
	CaluFifthFourier(dwComposeBuf+READ_POINT_NUM*4,&IDC5);
	CaluModValue(&IDC5);	
	IDC5.Mod = IDC5.Mod*1087/1000;
	*RM_pnIDC5= IDC5.Mod;                                       
	DB_LogWriteRM(RM_IDC5, *RM_pnIDC5);
	//IRC2
	CaluSecondFourier(dwComposeBuf+READ_POINT_NUM*5,&IRC2);
	CaluModValue(&IRC2);	
	IRC2.Mod = IRC2.Mod*1010/1000;
	*RM_pnIRC2= IRC2.Mod;                                       
	DB_LogWriteRM(RM_IRC2, *RM_pnIRC2);
}
void CRelayTask::ResetIdHLock()
{
	b68IdH2Flag_A = FALSE;	//A相差流二次闭锁标志	
	b68IdH2Flag_B = FALSE;	//B相差流二次闭锁标志	
	b68IdH2Flag_C = FALSE;	//C相差流二次闭锁标志	
	WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
	WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
}

/***************************************************************************************
功能:		计算三相差流的零漂
输入:		无
返回:		无
****************************************************************************************/

void CRelayTask::CaluDrift(void)
{
	if(FaultOnFlag == TRUE)//已经进入故障处理，不计算差流的零漂值
		return;

	LONG D1=0;
	LONG D2=0;
	LONG D3=0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		D1 += dwComposeBuf[i+2];
		D2 += dwComposeBuf[READ_POINT_NUM*2+i+2];
		D3 += dwComposeBuf[READ_POINT_NUM*4+i+2];
	}
	for(LONG i=0;i<4;i++)
	{
		IDADrift[4-i]=IDADrift[3-i];
		IDBDrift[4-i]=IDBDrift[3-i];
		IDBDrift[4-i]=IDBDrift[3-i];
	}
	IDADrift[0]=D1/POINT_PER_PERIOD;
	IDBDrift[0]=D2/POINT_PER_PERIOD;
	IDCDrift[0]=D3/POINT_PER_PERIOD;

}
//------------------------------------------------------------------------------------------ 
// 名称: 非周期分量抑制                                                              
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//------------------------------------------------------------------------------------------ 
void CRelayTask::R68IdAped()
{
	LONG tempA=0;
	LONG tempB=0;
	LONG tempC=0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		tempA += dwComposeBuf[i+2];
		tempB += dwComposeBuf[READ_POINT_NUM*2+i+2];
		tempC += dwComposeBuf[READ_POINT_NUM*4+i+2];
	}
	tempA = tempA*1414/(POINT_PER_PERIOD*1000) - IDADrift[4];   
	tempB = tempB*1414/(POINT_PER_PERIOD*1000) - IDBDrift[4];
	tempC = tempC*1414/(POINT_PER_PERIOD*1000) - IDCDrift[4];
	tempA = ToAbs(tempA);
	tempB = ToAbs(tempB);
	tempC = ToAbs(tempC);
	//差流A门槛值校正
	if((IDA.Mod ==0)||(tempA>15*IDA.Mod/10))//考虑分母为0的情况
	{
		A_87RPI = *RS_pn87RIA;
	}
	else		
	{
		A_87RPI = *RS_pn87RIA*(100+(*RS_pnKrav*tempA/IDA.Mod))/100; 
		IDAH2 = IDA2.Mod*(100+(*RS_pnK2av*tempA/IDA.Mod))/100;
	}
	
	//差流B门槛值校正
	if((IDB.Mod ==0)||(tempB>15*IDB.Mod/10))
	{
		B_87RPI = *RS_pn87RIB;
	}
	else
	{
		B_87RPI = *RS_pn87RIB*(100+*RS_pnKrav*tempB/IDB.Mod)/100;
		IDBH2 = IDB2.Mod*(100+(*RS_pnK2av*tempB/IDB.Mod))/100;
	}
	//差流C门槛值校正
	if((IDC.Mod ==0)||(tempC>15*IDC.Mod/10))
	{
		C_87RPI = *RS_pn87RIC;
	}
	else
	{
		C_87RPI = *RS_pn87RIC*(100+*RS_pnKrav*tempC/IDC.Mod)/100;		
		IDCH2 = IDC2.Mod*(100+(*RS_pnK2av*tempC/IDC.Mod))/100;
	}
			
}

/***************************************************************************************
功能:		差动电流二次谐波最大相闭锁
输入:		无
返回:		无
****************************************************************************************/
void CRelayTask::R68IdH2M()
{
	if(*CFG_pby68IdH2M_87R == CFG_STATUS_SET)
	{			
		if((IDA.Mod>A_87RPI)&&(*R87R_A.p87RCFG == CFG_STATUS_SET))
		{
			if(IDA2.Mod>(*RS_pnKrh2*IDA.Mod/100))
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
		if((IDB.Mod>B_87RPI)&&(*R87R_B.p87RCFG == CFG_STATUS_SET))
		{
			if(IDB2.Mod>(*RS_pnKrh2*IDB.Mod/100))
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
		if((IDC.Mod>C_87RPI)&&(*R87R_C.p87RCFG == CFG_STATUS_SET))
		{
			if(IDC2.Mod>(*RS_pnKrh2*IDC.Mod/100))
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
/***************************************************************************************
功能:		差动电流二次谐波综合相闭锁
输入:		无
返回:		无
****************************************************************************************/

void CRelayTask::R68IdH2C(void)		
{
	if(*CFG_pby68IdH2C_87R == CFG_STATUS_SET)
	{		
		LONG ID1,ID2;		
		ID1 = MaxABC(IDA.Mod,IDB.Mod,IDC.Mod);
		ID2 = MaxABC(IDA2.Mod,IDB2.Mod,IDC2.Mod);
		if(ID2>(*RS_pnKrh2*ID1/100))
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
/***************************************************************************************
功能:		差动电流二次谐波分相闭锁
输入:		无
返回:		无
****************************************************************************************/

void CRelayTask::R68IdH2I()  
{
	if(*CFG_pby68IdH2I_87R == CFG_STATUS_SET)
	{
		if((IDA.Mod>A_87RPI)&&(IDAH2>(*RS_pnKrh2*IDA.Mod/100)))
		{
			b68IdH2Flag_A = TRUE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDAH2,RELAY_ON);
		}			
		else
		{
			b68IdH2Flag_A = FALSE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
		}
		
		if((IDB.Mod>B_87RPI)&&(IDBH2>(*RS_pnKrh2*IDB.Mod/100)))
		{
			b68IdH2Flag_B = TRUE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDBH2,RELAY_ON);
		}			
		else
		{
			b68IdH2Flag_B = FALSE;	//A相差流二次闭锁标志					
			WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
		}

		
		if((IDC.Mod>C_87RPI)&&(IDCH2>(*RS_pnKrh2*IDC.Mod/100)))
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
/***************************************************************************************
功能:		制动电流二次谐波闭锁
输入:		无
返回:		无
****************************************************************************************/
void CRelayTask::R68IrH2M()
{	
	if(*CFG_pby68IrH2I_87R != CFG_STATUS_SET)
	{		
		b68IrH2Flag_A = FALSE;
		b68IrH2Flag_B = FALSE;
		b68IrH2Flag_C = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF);
		WriteRelayWord(RW_LK68IRCH2,RELAY_OFF);
		return;
	}
	
	if((IDA.Mod>A_87RPI)&&(IRA2.Mod>(*RS_pnKrh2*IRA.Mod/100)))
	{
		b68IrH2Flag_A = TRUE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_ON);		
	}		
	else
	{
		b68IrH2Flag_A = FALSE;
		WriteRelayWord(RW_LK68IRAH2,RELAY_OFF); 	
	}
	
	if((IDB.Mod>B_87RPI)&&(IRB2.Mod>(*RS_pnKrh2*IRB.Mod/100)))
	{
		b68IrH2Flag_B = TRUE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_ON);		
	}		
	else
	{
		b68IrH2Flag_B = FALSE;
		WriteRelayWord(RW_LK68IRBH2,RELAY_OFF); 	
	}
	
	if((IDC.Mod>C_87RPI)&&(IRC2.Mod>(*RS_pnKrh2*IRC.Mod/100)))
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
/***************************************************************************************
功能:		差动电流五次谐波闭锁
输入:		无
返回:		无
****************************************************************************************/
void CRelayTask::R68IdH5()
{	
	if(*CFG_pby68IdH5I_87R != CFG_STATUS_SET)
	{	
		b68IdH5Flag_A = FALSE;
		b68IdH5Flag_B = FALSE;
		b68IdH5Flag_C = FALSE;
		WriteRelayWord(RW_LK68IDAH5,RELAY_OFF); 	
		WriteRelayWord(RW_LK68IDBH5,RELAY_OFF);
		WriteRelayWord(RW_LK68IDCH5,RELAY_OFF);
		return;
	}
	
	if((IDA.Mod>A_87RPI)&&(IDA5.Mod>*RS_pnKrh5*IDA.Mod/100))
	{
		b68IdH5Flag_A= TRUE;
		WriteRelayWord(RW_LK68IDAH5,RELAY_ON);		
	}		
	else
	{
		b68IdH5Flag_A= FALSE;
		WriteRelayWord(RW_LK68IDAH5,RELAY_OFF); 	
	}

	
	if((IDB.Mod>B_87RPI)&&(IDB5.Mod>*RS_pnKrh5*IDB.Mod/100))
	{
		b68IdH5Flag_B= TRUE;
		WriteRelayWord(RW_LK68IDBH5,RELAY_ON);		
	}		
	else
	{
		b68IdH5Flag_B= FALSE;
		WriteRelayWord(RW_LK68IDBH5,RELAY_OFF); 	
	}
	
	if((IDC.Mod>C_87RPI)&&(IDC5.Mod>*RS_pnKrh5*IDC.Mod/100))
	{
		b68IdH5Flag_C= TRUE;
		WriteRelayWord(RW_LK68IDCH5,RELAY_ON);		
	}
	else
	{
		b68IdH5Flag_C= FALSE;
		WriteRelayWord(RW_LK68IDCH5,RELAY_OFF); 	
	}
}
//******************************************************************************************
// 名称: 差动电流综合谐波闭锁             
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::R68IdHI(void) 	//综合谐波闭锁
{
	
	if(*CFG_pby68IdHMUL_87R != CFG_STATUS_SET)
	{		
		b68IdHMulFlag_A = FALSE;
		b68IdHMulFlag_B = FALSE;
		b68IdHMulFlag_C = FALSE;
		WriteRelayWord(RW_LK68IDAH,RELAY_OFF);	
		WriteRelayWord(RW_LK68IDBH,RELAY_OFF);
		WriteRelayWord(RW_LK68IDCH,RELAY_OFF);
		return;
	}
	
	if((IDA.Mod>*RS_pn87RIA)&&((IDA2.Mod+IDA3.Mod)>(*RS_pnKrh*IDA.Mod/100)))
	{
		b68IdHMulFlag_A = TRUE;
		WriteRelayWord(RW_LK68IDAH,RELAY_ON);		
	}
	else
	{
		b68IdHMulFlag_A = FALSE;
		WriteRelayWord(RW_LK68IDAH,RELAY_OFF);		
	}
	
	if((IDB.Mod>*RS_pn87RIB)&&((IDB2.Mod+IDB3.Mod)>(*RS_pnKrh*IDB.Mod/100)))
	{
		b68IdHMulFlag_B = TRUE;
		WriteRelayWord(RW_LK68IDBH,RELAY_ON);		
	}		
	else
	{
		b68IdHMulFlag_B = FALSE;
		WriteRelayWord(RW_LK68IDBH,RELAY_OFF);		
	}
	
	if((IDC.Mod>*RS_pn87RIC)&&((IDC2.Mod+IDC3.Mod)>(*RS_pnKrh*IDC.Mod/100)))
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


//******************************************************************************************
// 名称: 初始化A相差动速断保护元件               
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87U_A()
{	
	R87U_A.pElecSet				= RS_pn87UIA;          			//过量保护电量定值
	R87U_A.pTimeSet				= &TimeSet;				 		//过量保护时间定值
	R87U_A.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数
	R87U_A.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	R87U_A.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	R87U_A.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	R87U_A.pRelCFG				= CFG_pby87U_A;					//过量保护功能控制字--1:投入，2:退出 
	R87U_A.pRelSW				= SW_pby87U;         			//过量保护软压板------1:投入，2:退出
	R87U_A.pRelHLockCFG			= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	R87U_A.pRelHSuprCFG			= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	R87U_A.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	R87U_A.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	R87U_A.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	R87U_A.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	R87U_A.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	R87U_A.pRel47UCFG			= &RelCFGCLR;
	R87U_A.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	R87U_A.pRelElec 			= &IDA;							//当前电量指针		
	R87U_A.pRelHarm3			= NULL;							//当前三次谐波指针	
	R87U_A.pRelHarm5			= NULL;							//当前五次谐波指针	
	R87U_A.pRelVolt				= NULL;							//当前用来闭锁的电压的指针

	R87U_A.StartRelayWord		= RW_PI87U_A;					//过量保护入段继电器字
	R87U_A.ActRelayWord			= RW_TR87U_A;					//过量保护动作继电器字
	R87U_A.AccRelayWord			= FALSE1;						//后加速允许继电器字
	R87U_A.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	R87U_A.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	R87U_A.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	R87U_A.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	R87U_A.R47ULKRelayWord		= FALSE1;
	R87U_A.LockRelayWord		= FALSE1;//RW_LKPI87U_A;					//入段闭锁继电器字
	R87U_A.RSTRelayWord			= TRUE1;//RW_LKRST87U_A;				//返回闭锁继电器字
	R87U_A.LoopCtrlRelayWord	= RW_LPC87U_A;					//保护循环控制继电器字
	
	R87U_A.wReportNum			= R_R87UA;						//过量保护报告序号
	R87U_A.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	R87U_A.bMakeActRpt			= TRUE;
	R87U_A.dwK35				= NULL;
	R87U_A.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************入段返回报告部分*************	yanxs  13-03-20
	R87U_A.wPickUpReportNum		= RPT_NULL;				//入段报告序列号
	R87U_A.wResetReportNum		= RPT_NULL;				//返回报告序列号
	//**********************************************************************	

//	R87U_A.dwRelTimer			= 0;			 			//定时器
	R87U_A.byRelFCounter		= 0;						//故障计数器
	R87U_A.byRelRCounter		= 0;						//返回计数器	
	
	R87U_A.wExcepWordNum     	= 8;
}
//******************************************************************************************
// 名称: 初始化B相差动速断保护元件               
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87U_B()
{	
	R87U_B.pElecSet				= RS_pn87UIB;          			//过量保护电量定值
	R87U_B.pTimeSet				= &TimeSet;				 		//过量保护时间定值
	R87U_B.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数
	R87U_B.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	R87U_B.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	R87U_B.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	R87U_B.pRelCFG				= CFG_pby87U_B;					//过量保护功能控制字--1:投入，2:退出 
	R87U_B.pRelSW				= SW_pby87U;         			//过量保护软压板------1:投入，2:退出
	R87U_B.pRelHLockCFG			= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	R87U_B.pRelHSuprCFG			= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	R87U_B.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	R87U_B.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	R87U_B.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	R87U_B.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	R87U_B.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	R87U_B.pRel47UCFG			= &RelCFGCLR;
	R87U_B.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	R87U_B.pRelElec 			= &IDB;							//当前电量指针		
	R87U_B.pRelHarm3			= NULL;							//当前三次谐波指针	
	R87U_B.pRelHarm5			= NULL;							//当前五次谐波指针	
	R87U_B.pRelVolt				= NULL;							//当前用来闭锁的电压的指针

	R87U_B.StartRelayWord		= RW_PI87U_B;					//过量保护入段继电器字
	R87U_B.ActRelayWord			= RW_TR87U_B;					//过量保护动作继电器字
	R87U_B.AccRelayWord			= FALSE1;						//后加速允许继电器字
	R87U_B.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	R87U_B.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	R87U_B.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	R87U_B.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	
	R87U_B.R47ULKRelayWord		= FALSE1;
	R87U_B.LockRelayWord		= FALSE1;//RW_LKPI87U_B;					//入段闭锁继电器字
	R87U_B.RSTRelayWord			= TRUE1;//RW_LKRST87U_B;				//返回闭锁继电器字
	R87U_B.LoopCtrlRelayWord	= RW_LPC87U_B;					//保护循环控制继电器字
	
	R87U_B.wReportNum			= R_R87UB;						//过量保护报告序号
	R87U_B.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	R87U_B.bMakeActRpt			= TRUE;
	R87U_B.dwK35				= NULL;
	R87U_B.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************入段返回报告部分*************	yanxs  13-03-20
	R87U_B.wPickUpReportNum		= RPT_NULL;				//入段报告序列号
	R87U_B.wResetReportNum		= RPT_NULL;				//返回报告序列号
	//**********************************************************************	


//	R87U_B.dwRelTimer			= 0;			 			//定时器
	R87U_B.byRelFCounter		= 0;						//故障计数器
	R87U_B.byRelRCounter		= 0;						//返回计数器	
	
	R87U_B.wExcepWordNum     	= 8;
}
//******************************************************************************************
// 名称: 初始化C相差动速断保护元件               
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87U_C()
{	
	R87U_C.pElecSet				= RS_pn87UIC;          			//过量保护电量定值
	R87U_C.pTimeSet				= &TimeSet;				 		//过量保护时间定值
	R87U_C.pHarmKhr1Set			= NULL;							//过量保护谐波加权系数
	R87U_C.pVolLockSet			= NULL;				 			//过量保护电压闭锁定值
	R87U_C.pAccActTimeSet		= NULL;							//过量保护后加速动作时间定值
	R87U_C.pH2Coef				= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	R87U_C.pRelCFG				= CFG_pby87U_C;					//过量保护功能控制字--1:投入，2:退出 
	R87U_C.pRelSW				= SW_pby87U;         			//过量保护软压板------1:投入，2:退出
	R87U_C.pRelHLockCFG			= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	R87U_C.pRelHSuprCFG			= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	R87U_C.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	R87U_C.pRelVolLockCFG		= &RelCFGCLR;					//低压闭锁功能控制位--1:投入，2:退出
	R87U_C.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	R87U_C.pRelAccCFG			= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	R87U_C.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	
	R87U_C.pRel47UCFG			= &RelCFGCLR;
	R87U_C.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	R87U_C.pRelElec 			= &IDC;							//当前电量指针		
	R87U_C.pRelHarm3			= NULL;							//当前三次谐波指针	
	R87U_C.pRelHarm5			= NULL;							//当前五次谐波指针	
	R87U_C.pRelVolt				= NULL;							//当前用来闭锁的电压的指针

	R87U_C.StartRelayWord		= RW_PI87U_C;					//过量保护入段继电器字
	R87U_C.ActRelayWord			= RW_TR87U_C;					//过量保护动作继电器字
	R87U_C.AccRelayWord			= FALSE1;						//后加速允许继电器字
	R87U_C.AccActRelayWord		= FALSE1;						//后加速动作继电器字
	R87U_C.PForwardRelayWord	= FALSE1;						//功率正方向继电器字
	R87U_C.PRewardRelayWord   	= FALSE1;						//功率反方向继电器字
	R87U_C.H2LockRelayWord		= FALSE1;						//二次谐波闭锁继电器字
	
	R87U_C.R47ULKRelayWord		= FALSE1;
	R87U_C.LockRelayWord		= FALSE1;//RW_LKPI87U_C;					//入段闭锁继电器字
	R87U_C.RSTRelayWord			= TRUE1;//RW_LKRST87U_C;				//返回闭锁继电器字
	R87U_C.LoopCtrlRelayWord	= RW_LPC87U_C;					//保护循环控制继电器字
	
	R87U_C.wReportNum			= R_R87UC;						//过量保护报告序号
	R87U_C.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	R87U_C.bMakeActRpt			= TRUE;
	R87U_C.dwK35				= NULL;
	R87U_C.RptSerialNum			= &RptSerialNumOfAct;						
	//**********************入段返回报告部分*************	yanxs  13-03-20
	R87U_C.wPickUpReportNum		= RPT_NULL;				//入段报告序列号
	R87U_C.wResetReportNum		= RPT_NULL;				//返回报告序列号
	//**********************************************************************	


//	R87U_C.dwRelTimer			= 0;			 			//定时器
	R87U_C.byRelFCounter		= 0;						//故障计数器
	R87U_C.byRelRCounter		= 0;						//返回计数器	
	
	R87U_C.wExcepWordNum     	= 8;
}
//******************************************************************************************
// 名称: 初始化A相比率差动保护元件      
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87R_A()
{
	R87R_A.p87RISet		= RS_pn87RIA; 	//差动电流定值
	R87R_A.pIRS1Set		= RS_pnIRS1A; 	//制动拐点1定值
	R87R_A.pIRS2Set		= RS_pnIRS2A; 	//制动拐点2定值 
	R87R_A.pSLP1Set			= RS_pnSLP1;		//比率制动系数1
	R87R_A.pSLP2Set			= RS_pnSLP2;		//比率制动系数2 
	R87R_A.pKrh2Set			= RS_pnKrh2;		//二次谐波制动系数
	R87R_A.pKrh5Set			= RS_pnKrh5;		//五次谐波制动系数
	R87R_A.pKravSet			= RS_pnKrav;		//比率差动非周期分量加权系数
	R87R_A.pK2avSet			= RS_pnK2av;		//二次谐波非周期分量加权系数

	R87R_A.p87RCFG			= CFG_pby87R_A;		//保护配置
	R87R_A.p87RSW			= SW_pby87R;		//软压板
	R87R_A.p68IdH2MCFG		= CFG_pby68IdH2M_87R;		//差流二次谐波最大相闭锁配置
	R87R_A.p68IdH2CCFG		= CFG_pby68IdH2C_87R;		//差流二次谐波综合相闭锁配置
	R87R_A.p68IdH2ICFG		= CFG_pby68IdH2I_87R;		//差流二次谐波分相闭锁配置
	R87R_A.p68IrH2MCFG		= CFG_pby68IrH2I_87R;		//制动电流二次谐波闭锁配置
	R87R_A.p68IdH5CFG		= CFG_pby68IdH5I_87R;		//差流5次谐波闭锁
	R87R_A.p68IdHMulCFG		= CFG_pby68IdHMUL_87R;

	R87R_A.pIdH2Flag		= &b68IdH2Flag_A; 	//二次谐波闭锁标志
	R87R_A.pIdH5Flag		= &b68IdH5Flag_A; 	//五次谐波闭锁标志
	R87R_A.pIrH2Flag		= &b68IrH2Flag_A; 	//制动电流二次谐波闭锁标志
	R87R_A.pIdHMulFlag		= &b68IdHMulFlag_A;

	R87R_A.p87RPI			= &A_87RPI;		//抑制计算后的差动电流门槛定值
	R87R_A.pId				= &IDA;			//差流电流
	R87R_A.pIr				= &IRA;			//制动电流

	R87R_A.ACTRelayWord		= RW_TR87R_A;					//动作继电器字 
	R87R_A.StartRelayWord	= RW_PI87R_A; 				//入段继电器字 
	R87R_A.LockRelayWord	= FALSE1;//RW_LKPI87R_A;					//入段闭锁继电器字 
	R87R_A.RSTRelayWord		= TRUE1;//RW_LKRST87R_A;					//返回闭锁继电器字 
	R87R_A.LoopCtrlRelayWord= RW_LPC87R_A;				//故障循环控制继电器字 

	R87R_A.RptSerialNum		= &RptSerialNumOfAct;					//系统报告序列号 
	R87R_A.pSysTimer		= &dwRelayMSCnt;; 					//系统定时器 
	R87R_A.wRelRptNum		= R_R87RA; 				//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-20
	R87R_A.wPickUpReportNum		= RPT_NULL;				//入段报告序列号
	R87R_A.wResetReportNum		= RPT_NULL;				//返回报告序列号
	//**********************************************************************	

	R87R_A.wRelFCounter		= 0;					//故障计数器 
	R87R_A.wRelRCounter		= 0;					//返回计数器	
//	R87R_A.dwRelTimer		= 0; 					//保护定时器		
	R87R_A.wExcepWordNum	= 7;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为14 
	R87R_A.Info				= 0;							//备用信息 
}
//******************************************************************************************
// 名称: 初始化B相比率差动保护元件      
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87R_B()
{
	R87R_B.p87RISet		= RS_pn87RIB; 	//差动电流定值
	R87R_B.pIRS1Set		= RS_pnIRS1B; 	//制动拐点1定值
	R87R_B.pIRS2Set		= RS_pnIRS2B; 	//制动拐点2定值 
	R87R_B.pSLP1Set			= RS_pnSLP1;		//比率制动系数1
	R87R_B.pSLP2Set			= RS_pnSLP2;		//比率制动系数2 
	R87R_B.pKrh2Set			= RS_pnKrh2;		//二次谐波制动系数
	R87R_B.pKrh5Set			= RS_pnKrh5;		//五次谐波制动系数
	R87R_B.pKravSet			= RS_pnKrav;		//比率差动非周期分量加权系数
	R87R_B.pK2avSet			= RS_pnK2av;		//二次谐波非周期分量加权系数

	R87R_B.p87RCFG			= CFG_pby87R_B;		//保护配置
	R87R_B.p87RSW			= SW_pby87R;		//软压板
	R87R_B.p68IdH2MCFG		= CFG_pby68IdH2M_87R;		//差流二次谐波最大相闭锁配置
	R87R_B.p68IdH2CCFG		= CFG_pby68IdH2C_87R;		//差流二次谐波综合相闭锁配置
	R87R_B.p68IdH2ICFG		= CFG_pby68IdH2I_87R;		//差流二次谐波分相闭锁配置
	R87R_B.p68IrH2MCFG		= CFG_pby68IrH2I_87R;		//制动电流二次谐波闭锁配置
	R87R_B.p68IdH5CFG		= CFG_pby68IdH5I_87R;		//差流5次谐波闭锁
	R87R_B.p68IdHMulCFG		= CFG_pby68IdHMUL_87R;

	R87R_B.pIdH2Flag		= &b68IdH2Flag_B; 	//二次谐波闭锁标志
	R87R_B.pIdH5Flag		= &b68IdH5Flag_B; 	//五次谐波闭锁标志
	R87R_B.pIrH2Flag		= &b68IrH2Flag_B; 	//制动电流二次谐波闭锁标志
	R87R_B.pIdHMulFlag		= &b68IdHMulFlag_B;

	R87R_B.p87RPI			= &B_87RPI;		//抑制计算后的差动电流门槛定值
	R87R_B.pId				= &IDB;			//差流电流
	R87R_B.pIr				= &IRB;			//制动电流

	R87R_B.ACTRelayWord		= RW_TR87R_B;					//动作继电器字 
	R87R_B.StartRelayWord	= RW_PI87R_B; 				//入段继电器字 
	R87R_B.LockRelayWord	= FALSE1;//RW_LKPI87R_B;					//入段闭锁继电器字 
	R87R_B.RSTRelayWord		= TRUE1;//RW_LKRST87R_B;					//返回闭锁继电器字 
	R87R_B.LoopCtrlRelayWord= RW_LPC87R_B;				//故障循环控制继电器字 

	R87R_B.RptSerialNum		= &RptSerialNumOfAct;					//系统报告序列号 
	R87R_B.pSysTimer		= &dwRelayMSCnt;; 					//系统定时器 
	R87R_B.wRelRptNum		= R_R87RB; 				//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-20
	R87R_B.wPickUpReportNum		= RPT_NULL;				//入段报告序列号
	R87R_B.wResetReportNum		= RPT_NULL;				//返回报告序列号
	//**********************************************************************	

	R87R_B.wRelFCounter		= 0;					//故障计数器 
	R87R_B.wRelRCounter		= 0;					//返回计数器	
//	R87R_B.dwRelTimer		= 0; 					//保护定时器		
	R87R_B.wExcepWordNum	= 7;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为14 
	R87R_B.Info				= 0;							//备用信息 
}
//******************************************************************************************
// 名称: 初始化C相比率差动保护元件      
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************

void CRelayTask::InitR87R_C()
{
	R87R_C.p87RISet		= RS_pn87RIC; 	//差动电流定值
	R87R_C.pIRS1Set		= RS_pnIRS1C; 	//制动拐点1定值
	R87R_C.pIRS2Set		= RS_pnIRS2C; 	//制动拐点2定值 
	R87R_C.pSLP1Set			= RS_pnSLP1;		//比率制动系数1
	R87R_C.pSLP2Set			= RS_pnSLP2;		//比率制动系数2 
	R87R_C.pKrh2Set			= RS_pnKrh2;		//二次谐波制动系数
	R87R_C.pKrh5Set			= RS_pnKrh5;		//五次谐波制动系数
	R87R_C.pKravSet			= RS_pnKrav;		//比率差动非周期分量加权系数
	R87R_C.pK2avSet			= RS_pnK2av;		//二次谐波非周期分量加权系数

	R87R_C.p87RCFG			= CFG_pby87R_C;		//保护配置
	R87R_C.p87RSW			= SW_pby87R;		//软压板
	R87R_C.p68IdH2MCFG		= CFG_pby68IdH2M_87R;		//差流二次谐波最大相闭锁配置
	R87R_C.p68IdH2CCFG		= CFG_pby68IdH2C_87R;		//差流二次谐波综合相闭锁配置
	R87R_C.p68IdH2ICFG		= CFG_pby68IdH2I_87R;		//差流二次谐波分相闭锁配置
	R87R_C.p68IrH2MCFG		= CFG_pby68IrH2I_87R;		//制动电流二次谐波闭锁配置
	R87R_C.p68IdH5CFG		= CFG_pby68IdH5I_87R;		//差流5次谐波闭锁
	R87R_C.p68IdHMulCFG		= CFG_pby68IdHMUL_87R;

	R87R_C.pIdH2Flag		= &b68IdH2Flag_C; 	//二次谐波闭锁标志
	R87R_C.pIdH5Flag		= &b68IdH5Flag_C; 	//五次谐波闭锁标志
	R87R_C.pIrH2Flag		= &b68IrH2Flag_C; 	//制动电流二次谐波闭锁标志
	R87R_C.pIdHMulFlag		= &b68IdHMulFlag_C;

	R87R_C.p87RPI			= &C_87RPI;		//抑制计算后的差动电流门槛定值
	R87R_C.pId				= &IDC;			//差流电流
	R87R_C.pIr				= &IRC;			//制动电流

	R87R_C.ACTRelayWord		= RW_TR87R_C;					//动作继电器字 
	R87R_C.StartRelayWord	= RW_PI87R_C; 				//入段继电器字 
	R87R_C.LockRelayWord	= FALSE1;//RW_LKPI87R_C;					//入段闭锁继电器字 
	R87R_C.RSTRelayWord		= TRUE1;//RW_LKRST87R_C;					//返回闭锁继电器字 
	R87R_C.LoopCtrlRelayWord= RW_LPC87R_C;				//故障循环控制继电器字 

	R87R_C.RptSerialNum		= &RptSerialNumOfAct;					//系统报告序列号 
	R87R_C.pSysTimer		= &dwRelayMSCnt;; 					//系统定时器 
	R87R_C.wRelRptNum		= R_R87RC; 				//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-20
	R87R_C.wPickUpReportNum		= RPT_NULL;				//入段报告序列号
	R87R_C.wResetReportNum		= RPT_NULL;				//返回报告序列号
	//**********************************************************************	

	R87R_C.wRelFCounter		= 0;					//故障计数器 
	R87R_C.wRelRCounter		= 0;					//返回计数器	
//	R87R_C.dwRelTimer		= 0; 					//保护定时器		
	R87R_C.wExcepWordNum	= 7;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为14 
	R87R_C.Info				= 0;							//备用信息 
}
void CRelayTask::InitR87IAlarm()
{	
	
	R87Alarm.p87RSW				= SW_pby87R;		//软压板
	R87Alarm.p87R_A_CFG			= CFG_pby87R_A;		//保护配置
	R87Alarm.p87R_B_CFG			= CFG_pby87R_B;		//保护配置
	R87Alarm.p87R_C_CFG			= CFG_pby87R_C;		//保护配置

	R87Alarm.p87RIASet			= RS_pn87RIA; 	//A差动电流定值
	R87Alarm.p87RIBSet			= RS_pn87RIB; 	//B差动电流定值
	R87Alarm.p87RICSet			= RS_pn87RIC; 	//C差动电流定值
	R87Alarm.p87RIKSet			= RS_pnK87AR;		//差流越限告警比率

	R87Alarm.pIDA				= &IDA;			//差流A
	R87Alarm.pIDB				= &IDB;			//差流B
	R87Alarm.pIDC				= &IDC;			//差流C

	R87Alarm.AlarmCnt			= R87IALARMTIME;		//差流越限告警时间
	R87Alarm.ARRW				= RW_AR87;			//差流越限告警继电器字
	R87Alarm.wRelRptNum			= R_AR87R;		//差流越限告警报告序列号

	R87Alarm.ACnt				= 0;			//A相计数器
	R87Alarm.BCnt				= 0;			//B相计数器
	R87Alarm.CCnt				= 0;			//C相计数器
	R87Alarm.AAlarmFlag			= FALSE;		//A相告警标志
	R87Alarm.BAlarmFlag			= FALSE;		//B相告警标志
	R87Alarm.CAlarmFlag			= FALSE;		//C相告警标志
	R87Alarm.MakeRptFlag		= FALSE;	//作告警报告标志
	R87Alarm.wExcepWordNum      = 16;
}

//******************************************************************************************
// 名称: 初始化保护结构体自检信息
// 输入:                                          
// 输出: 无                                                          
// 返回: 无                                                                                                                                                               
//******************************************************************************************
void CRelayTask::InitStructCheck()
{
	WORD acc = 0x55aa;
	
	StructCheck[0].pstruct = (WORD*)&R87U_A;
	StructCheck[0].bigsize = sizeof(R87U_A)/2 - R87U_A.wExcepWordNum;	
	StructCheck[1].pstruct = (WORD*)&R87U_B;
	StructCheck[1].bigsize = sizeof(R87U_B)/2 - R87U_B.wExcepWordNum;
	StructCheck[2].pstruct = (WORD*)&R87U_C;
	StructCheck[2].bigsize = sizeof(R87U_C)/2 - R87U_C.wExcepWordNum;	
	StructCheck[3].pstruct = (WORD*)&R87R_A;
	StructCheck[3].bigsize = sizeof(R87R_A)/2 - R87R_A.wExcepWordNum;
	StructCheck[4].pstruct = (WORD*)&R87R_B;
	StructCheck[4].bigsize = sizeof(R87R_B)/2 - R87R_B.wExcepWordNum;
	StructCheck[5].pstruct = (WORD*)&R87R_C;
	StructCheck[5].bigsize = sizeof(R87R_C)/2 - R87R_C.wExcepWordNum;
	StructCheck[6].pstruct = (WORD*)&R87Alarm;
	StructCheck[6].bigsize = sizeof(R87Alarm)/2 - R87Alarm.wExcepWordNum;	
	StructCheck[7].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[7].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;

	for(LONG j=0;j<8;j++)
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

	if((*R87U_A.pRelCFG == CFG_STATUS_SET) && (*R87U_A.pRelSW == SW_STATUS_SET))   //A相差动速断
	{
		if(R87U_A.pRelElec->Mod > *R87U_A.pElecSet)
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
	if((*R87U_B.pRelCFG == CFG_STATUS_SET) && (*R87U_B.pRelSW == SW_STATUS_SET))   //B相差动速断
	{
		if(R87U_B.pRelElec->Mod > *R87U_B.pElecSet)
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
	if((*R87U_C.pRelCFG == CFG_STATUS_SET) && (*R87U_C.pRelSW == SW_STATUS_SET))   //C相差动速断
	{
		if(R87U_C.pRelElec->Mod > *R87U_C.pElecSet)
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

	if((*R87R_A.p87RCFG == CFG_STATUS_SET) && (*R87R_A.p87RSW == SW_STATUS_SET))   //A相比率差动
	{
		if(R87R_A.pId->Mod > *R87R_A.p87RISet)
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
	if((*R87R_B.p87RCFG == CFG_STATUS_SET) && (*R87R_B.p87RSW == SW_STATUS_SET))   //B相比率差动
	{
		if(R87R_B.pId->Mod > *R87R_B.p87RISet)
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
	if((*R87R_C.p87RCFG == CFG_STATUS_SET) && (*R87R_C.p87RSW == SW_STATUS_SET))   //C相比率差动
	{
		if(R87R_C.pId->Mod > *R87R_C.p87RISet)
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

	if(((*R87R_A.p87RCFG == CFG_STATUS_SET) && (*R87R_A.p87RSW == SW_STATUS_SET)) || \
		((*R87R_B.p87RCFG == CFG_STATUS_SET) && (*R87R_B.p87RSW == SW_STATUS_SET)) || \
		((*R87R_C.p87RCFG == CFG_STATUS_SET) && (*R87R_C.p87RSW == SW_STATUS_SET)) || \
		((*R87U_A.pRelCFG == CFG_STATUS_SET)&& (*R87U_A.pRelSW == SW_STATUS_SET)) || \
		((*R87U_B.pRelCFG == CFG_STATUS_SET)&& (*R87U_B.pRelSW == SW_STATUS_SET)) || \
		((*R87U_C.pRelCFG == CFG_STATUS_SET)&& (*R87U_C.pRelSW == SW_STATUS_SET)))
	
	{
		STATUS rc;
		BYTE  pChannel[]={CHANNEL_6530_IDA,CHANNEL_6530_IDB,CHANNEL_6530_IDC};		
		rc = ReadPreSingleData(dwPickDataBuf,pChannel,3,RELAY_SAMP_INTERVAL);

		if(rc != ERR_SUCCESS)
		{
			ReportMsg("Serious Error: Read sample data failure!!!");
			return;
		}
		
		for(LONG j=0;j<3;j++)//根据需突变启动的通道数
		{
			PickCheck = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{			
				lSampData1 = dwPickDataBuf[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2 = dwPickDataBuf[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3 = dwPickDataBuf[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue = MULTIPLES*ToAbs(((lSampData1-lSampData2)-(lSampData2-lSampDate3)));

				//采用浮动门槛值判断，ΔI>0.25*ΔI'+ C,其中ΔI'为上个一个邻近的点的突变量值，C为突变量启动定值
				if(lPickValue*4>*RS_pnPIC*4+lPickUpValueArray[j])
				//if(lPickValue>*RS_pnPIC)
				{
					PickCheck++;
					if(PickCheck==3)
					{   //修正保护启动时间的误差
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime);
						PickTime = PickUpTime.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime.Lo-(i*625+500)/1000;
						PickUpTime.Lo= (DWORD)PickTime;
						PickUpTime.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(PickUpRW, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
						RptSerialNumOfAct = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
						RptSerialNumOfStart = g_RptSerialNum;
						PickCheck =0;
						for(LONG m=0;m<3;m++)
						{
							PickCheckNext[m] =0;
							lPickUpValueArray[m]=0;
						}
						return TRUE;	
					}
					if(i<2)
					{
						PickCheckNext[j]++;
						if(PickCheckNext[j]==3)
						{							
							QWORD PickTime;
							ReadAbsTime(&PickUpTime);
							PickTime = PickUpTime.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime.Lo-(i*625+500)/1000;
							PickUpTime.Lo= (DWORD)PickTime;
							PickUpTime.Hi= (WORD)(PickTime>>32);
							
							WriteRelayWord(PickUpRW, RELAY_ON);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime,FALSE); //向数据库中写入启动报告
							RptSerialNumOfAct = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime,TRUE); //向数据库中写入启动报告
							RptSerialNumOfStart = g_RptSerialNum;
							PickCheck =0;
							for(LONG m=0;m<3;m++)
							{
								PickCheckNext[m] =0;
								lPickUpValueArray[m]=0;
							}
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
				lPickUpValueArray[j] = lPickValue;				
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
	if(FaultOnFlag ==FALSE)   						//尚未进入故障处理
	{		
		RelayPickUpCHK();					//执行启动检测
		if(ReadRelayWord(RW_PICKUP))
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
	if(FaultSINum<POINT_PER_PERIOD-3)					//故障处理采样间隔计数器小于一周波采样点数
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
		if((*CFG_pby68IdH2M_87R==CFG_STATUS_CLR)&&(*CFG_pby68IdH2C_87R==CFG_STATUS_CLR)&&(*CFG_pby68IdH2I_87R==CFG_STATUS_CLR))
		{
			b68IdH2Flag_A = FALSE;	//A相差流二次闭锁标志	
			b68IdH2Flag_B = FALSE;	//B相差流二次闭锁标志	
			b68IdH2Flag_C = FALSE;	//C相差流二次闭锁标志	
			WriteRelayWord(RW_LK68IDAH2,RELAY_OFF);
			WriteRelayWord(RW_LK68IDBH2,RELAY_OFF);
			WriteRelayWord(RW_LK68IDCH2,RELAY_OFF);
		}
		
		ResetIdHLock();
		R68IdAped();
		R68IdH2M();
		R68IdH2C();
		R68IdH2I();
		R68IrH2M();
		R68IdH5();	
		R68IdHI();
		GeneralOverValueRelay(&R87U_A);				//差动速断A
		GeneralOverValueRelay(&R87U_B);				//差动速断B
		GeneralOverValueRelay(&R87U_C);				//差动速断C
		Rel87RI(&R87R_A);				//比率差动A
		Rel87RI(&R87R_B);				//比率差动B
		Rel87RI(&R87R_C);				//比率差动C

		if(ReadRelayWord(RW_52BFH))
		{
			if(!m_bHaveMakeTripFailRpt)
			{
				MakeActRpt(R_TRFAIL,RptSerialNumOfAct);   	//作断路器跳闸失败报告
				m_bHaveMakeTripFailRpt = TRUE;
			}
		}
		
		// 1。有保护循环控制继电器字被置位，等待保护动作或者退出
		if(ReadRelayWord(R87U_A.LoopCtrlRelayWord)||ReadRelayWord(R87U_B.LoopCtrlRelayWord)||ReadRelayWord(R87U_C.LoopCtrlRelayWord)
			||ReadRelayWord(R87R_A.LoopCtrlRelayWord)||ReadRelayWord(R87R_B.LoopCtrlRelayWord)||ReadRelayWord(R87R_C.LoopCtrlRelayWord))
		{
			if(ReadRelayWord(RW_52BFH))
			{				
				WriteRelayWord(PickUpRW, RELAY_OFF);		//清保护启动继电器字
				FaultCheckOutFlag = FALSE;					//清检出故障标志
				FaultOnFlag = FALSE;						//清故障循环处理标志
				HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志
				m_bFaultLKFlag = FALSE; 					//清有启动无动作时启动时刻锁定标志	
				DB_RelayEnd(RptSerialNumOfAct); 			//向数据库中写入结束报告
				m_bHaveMakeTripFailRpt = FALSE;
				return; 				
			}		
			FaultLockTime = dwRelayMSCnt;		
			FaultCheckOutFlag = TRUE;
			return;
		}
		// 2。无保护循环控制继电器字被置位，此时保护已动作结束或没有满足任何保护动作条件
		else				
		{
			// 2.1 检出故障标志被置位，表示保护循环控制继电器字曾经被置位--不一定动作过--等待RS_pnSOTFT时间后退出
			if(FaultCheckOutFlag == TRUE)     
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
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					DB_RelayEnd(RptSerialNumOfAct);				//向数据库中写入结束报告					
					m_bHaveMakeTripFailRpt = FALSE;
					ResetAllRelay(TRUE);
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
					FaultLockTime = dwRelayMSCnt;	
					m_bFaultLKFlag = TRUE;					
				}
				if(dwRelayMSCnt -FaultLockTime>500 )	
				{
					WriteRelayWord(PickUpRW, RELAY_OFF);		//清保护启动继电器字
					FaultCheckOutFlag = FALSE;					//清检出故障标志
					FaultOnFlag = FALSE;						//清故障循环处理标志
					HaveMakeStartRptFlag = FALSE;				//清已作带故障电量启动报告标志						
					m_bFaultLKFlag = FALSE;						//清有启动无动作时启动时刻锁定标志	
					DB_RelayEnd(RptSerialNumOfAct); 
					ResetAllRelay(TRUE);					
					m_bHaveMakeTripFailRpt = FALSE;
					return;					
				}
				else	//延时未到，直接返回
					return;
			}			
		}		
	}
}
void CRelayTask::Process52BF()
{
	if(ReadRelayWord(RW_52BFH))
	{
		m_b52BFFlag = TRUE;
	}
	if(m_b52BFFlag)
	{
		if(!ReadRelayWord(RW_52BFH))
		{
			ResetAllRelay(TRUE);
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
	ResetOverValueRelay(&R87U_A,resetFlag);
	ResetOverValueRelay(&R87U_B,resetFlag);
	ResetOverValueRelay(&R87U_C,resetFlag);
	Reset87RI(&R87R_A,resetFlag);
	Reset87RI(&R87R_B,resetFlag);
	Reset87RI(&R87R_C,resetFlag);
}
