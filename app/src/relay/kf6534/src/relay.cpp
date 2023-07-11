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
*           		保护算法代码     FOR  6534电铁AT保护测控装置                                                               
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
*      yanzh             11/12/12   初始创建                         
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

extern DWORD 	dwRelayMSCnt;		//保护用毫秒计数器
extern DWORD 	g_hDbaseTaskID;
extern DWORD 	dwRelayDataPtr;
extern BYTE 	byRelayWordValue[];
extern BYTE 	g_RptSerialNum;			//报告序列号 
extern BOOL		RelayCheckErr;			//保护元件自检出错标志
extern BOOL 	RelayTaskInitOK;		//保护初始化正常
extern BOOL 	RelayCheckInitOK;		//保护元件自检初始化标志
extern TSysConfigTable	*G_pCfgTable;
extern DWORD dwADResumeCnt;	//AD功能恢复计数，从异常到正常后需保证正常采样一个完整周波数据
extern BYTE *g_MaintSW;
BOOL 			RelayTaskInitOK = FALSE;
DWORD 			dwRelayTaskID;
DWORD			dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
DWORD			dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
TStructCheck 	StructCheck[CHECK_NUM];					//结构体校验数组

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
/****************************************************************************************
名  称: Init
功  能: 保护任务初始化
参  数: 无
返回值: 无
*****************************************************************************************/

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
	
	RelCFGCLR = CFG_STATUS_CLR;			//保护退出
	RelCFGSET = CFG_STATUS_SET;			//保护投入	
	m_bLastRecStatusFlag = FALSE;		//故障录波启动上升沿判断标志
	//获取数据库保护测量值数据指针，以加快操作速度
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	
	InitAllRelayMem();
	InitRealy();
	InitDCRecInfo();

	
	dwChanDataMask = ((0x01<<CHANNEL_6534_U1)|(0x01<<CHANNEL_6534_U2)|(0x01<<CHANNEL_6534_U3)|(0x01<<CHANNEL_6534_U4) \
					|(0x01<<CHANNEL_6534_U5)|(0x01<<CHANNEL_6534_U6)|(0x01<<CHANNEL_6534_U7)|(0x01<<CHANNEL_6534_U8) \
					|(0x01<<CHANNEL_6534_I1)|(0x01<<CHANNEL_6534_I2)|(0x01<<CHANNEL_6534_I3)|(0x01<<CHANNEL_6534_I4) \
					|(0x01<<CHANNEL_6534_I5)|(0x01<<CHANNEL_6534_I6)|(0x01<<CHANNEL_6534_I7)|(0x01<<CHANNEL_6534_I8));
	dwRecDataMask = ((0x01<<CHANNEL_6534_IC1)|(0x01<<CHANNEL_6534_IC2));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	//初始化读取保护数据临时缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);	
	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);	
	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);	
	
	
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

	ASSERT(dwRecBuf != NULL);
	if(dwRecBuf == NULL)
	{
		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2));
	
	ASSERT(dwComposeBuf != NULL);
	if(dwComposeBuf == NULL)
	{
		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*2));

	//初始化逻辑方程部分
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

	g_MaintSW = SW_pbyMAINT;



//***************AT1相关变量区*******************************************************	
	//AT1突变量启动历史数据缓冲区
	dwPickDataBuf_AT1 = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);
	ASSERT(dwPickDataBuf_AT1 != NULL);
	if(dwPickDataBuf_AT1 == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf_AT1",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf_AT1, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);

	RptSerialNumOfAct_AT1 = 0;	 //动作报告序列号
	RptSerialNumOfStart_AT1 = 0; //启动报告序列号		
	PickCheck_AT1 = 0;			 //突变量启动计数器
	for(LONG m=0;m<4;m++)
	{
		m_lPickUpValue_AT1[m] = 0;	 //相邻点突变量启动值
		m_wPickCheckNext_AT1[m] = 0; //两次保护任务中连续突变量大于定值的计数器 
	}
	m_wPickCheck_AT1 = 0;		//本次判断中连续突变量大于定值的计数器
	lSampData1_AT1 = 0; 		//突变量值1
	lSampData2_AT1 = 0; 		//突变量值2
	lSampDate3_AT1 = 0; 		//突变量值3
	lPickValue_AT1 = 0; 		//本次突变量值
	FaultSINum_AT1 = 0; 				//故障处理采样间隔计数器		
	FaultLockTime_AT1 = 0;				//故障锁定定时器		
	FaultOnFlag_AT1 = FALSE;					//进入故障循环处理标志		
	HaveMakeStartRptFlag_AT1 = FALSE;		//作启动报告标志 
	FaultCheckOutFlag_AT1 = FALSE;			//故障循环处理检出故障标志 
	m_bFaultLKFlag_AT1 = FALSE; 			//故障起始时刻时间锁定标志，用于突变量启动	
	m_b52BFFlag_AT1 = FALSE;
	m_bHaveRelayAct_AT1 = FALSE;			//本次保护启动判断中保护曾动作过标志
	m_bHaveMake52BFRpt_AT1 = FALSE;			//断路器失败报告已作出

//***************AT2相关变量区*******************************************************	
	//AT2突变量启动历史数据缓冲区
	dwPickDataBuf_AT2 = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);
	ASSERT(dwPickDataBuf_AT2 != NULL);
	if(dwPickDataBuf_AT2 == NULL)
	{
		LogError("CRelayTask,dwPickDataBuf_AT2",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwPickDataBuf_AT2, 0, sizeof(DWORD)*RELAY_SAMP_INTERVAL*3*4);

	RptSerialNumOfAct_AT2 = 0;	 //动作报告序列号
	RptSerialNumOfStart_AT2 = 0; //启动报告序列号		
	PickCheck_AT2 = 0;			 //突变量启动计数器
	for(LONG m=0;m<4;m++)
	{
		m_lPickUpValue_AT2[m] = 0;	 //相邻点突变量启动值
		m_wPickCheckNext_AT2[m] = 0; //两次保护任务中连续突变量大于定值的计数器 
	}
	m_wPickCheck_AT2 = 0;		//本次判断中连续突变量大于定值的计数器
	lSampData1_AT2 = 0; 		//突变量值1
	lSampData2_AT2 = 0; 		//突变量值2
	lSampDate3_AT2 = 0; 		//突变量值3
	lPickValue_AT2 = 0; 		//本次突变量值
	FaultSINum_AT2 = 0; 				//故障处理采样间隔计数器		
	FaultLockTime_AT2 = 0;				//故障锁定定时器		
	FaultOnFlag_AT2 = FALSE;					//进入故障循环处理标志		
	HaveMakeStartRptFlag_AT2 = FALSE;		//作启动报告标志 
	FaultCheckOutFlag_AT2 = FALSE;			//故障循环处理检出故障标志 
	m_bFaultLKFlag_AT2 = FALSE; 			//故障起始时刻时间锁定标志，用于突变量启动	
	m_b52BFFlag_AT2 = FALSE;
	
	m_bHaveMake52BFRpt_AT2 = FALSE;
	m_bHaveRelayAct_AT2 =FALSE;

	
	m_bLogicChanged = FALSE;
	RelayTaskInitOK = TRUE; 
	SetTimer(1,5);

}
/****************************************************************************************
名  称: InitRealy
功  能: 初始化各保护元件
参  数: 无
返回值: 无
*****************************************************************************************/
void  CRelayTask::InitRealy(void)
{
	 InitRelR50IC1();
	 InitRelR50IT1();
	 InitRelR50IF1();
	 InitRelR50IP1();
	 InitRelR50IC2();
	 InitRelR50IT2();
	 InitRelR50IF2();
	 InitRelR50IP2();
	 InitStructCheck();
}
/****************************************************************************************
名  称: OnTimeOut
功  能: 时间定时器响应函数
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::OnTimeOut(DWORD id)
{
	if(dwADResumeCnt ==0)
		return;
	else
	{
		LogicProcess();
	}

}
/****************************************************************************************
名  称: OnSampleEnd
功  能: 中断响应函数，保护程序主函数
参  数: 
返回值: 无
*****************************************************************************************/
void CRelayTask::OnSampleEnd(DWORD arg)
{	
//	MCF_GPIO_PODR_TIMER |= 0x02;		//高电平
	static WORD wInvalidCheckCnt;		//开出自检累加计数器	
	DWORD dwDataPtr;
	dwDataPtr = dwRelayDataPtr;
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
	
	//首先进行开出自检,每次保护任务检一路开出
	if(!m_bLogicChanged)
		BoBreakCheck();
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
	WriteComposeBuf(dwRecDataMask);
	CaluUI();          //根据采样数据，进算电量It,If,U1,U2	
	FalutProcess_AT1();	//故障循环处理	
	FalutProcess_AT2();	//故障循环处理
	Process52BF_AT1();
	Process52BF_AT2();
	
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

	WriteDCChannel();
//	MCF_GPIO_PODR_TIMER &= ~0x02;		//低电平
}

/****************************************************************************************
名  称: WriteComposeBuf
功  能: 合成通道数据回写
参  数: dwMask -- 合成通道数据掩码
返回值: 无
*****************************************************************************************/
void CRelayTask::WriteComposeBuf(DWORD dwMask)
{
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，
	{
		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*9-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*10-RELAY_SAMP_INTERVAL+i];
	}
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		dwComposeBuf[i]= dwSampBuf[READ_POINT_NUM*8+i] - dwSampBuf[READ_POINT_NUM*9+i];
	}
	
	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，
	{
		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*12-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*13-RELAY_SAMP_INTERVAL+i];
	}
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		dwComposeBuf[READ_POINT_NUM+i]= dwSampBuf[READ_POINT_NUM*11+i] - dwSampBuf[READ_POINT_NUM*12+i];
	}

	WriteComposeData(dwRecBuf, dwMask);
}
/****************************************************************************************
名  称: CaluUI
功  能: 计算各通道的电流电压模值
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::CaluUI(void)
{	
	// 1.U1
	CaluBaseFourier_S15(dwSampBuf,&U1);
	CaluModValue(&U1);
	*RM_pnUp1= U1.Mod;                                       
	DB_LogWriteRM(RM_UP1, *RM_pnUp1);	
	if(U1.Mod>=*RS_pn59U1)	
	{	
		WriteRelayWord(RW_59U1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U1, RELAY_OFF);
	}
	if(U1.Mod>=*RS_pn27U1)
	{
		WriteRelayWord(RW_27U1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U1, RELAY_ON);
	}
	// 2.U2
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM,&U2);
	CaluModValue(&U2);
	*RM_pnUp2= U2.Mod;                                       
	DB_LogWriteRM(RM_UP2, *RM_pnUp2);	
	if(U2.Mod>=*RS_pn59U2)	
	{	
		WriteRelayWord(RW_59U2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U2, RELAY_OFF);
	}
	if(U2.Mod>=*RS_pn27U2)
	{
		WriteRelayWord(RW_27U2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U2, RELAY_ON);
	}
	// 3.U3
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*2,&U3);
	CaluModValue(&U3);
	*RM_pnUp3= U3.Mod;                                       
	DB_LogWriteRM(RM_UP3, *RM_pnUp3);	
	if(U3.Mod>=*RS_pn59U3)	
	{	
		WriteRelayWord(RW_59U3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U3, RELAY_OFF);
	}
	if(U3.Mod>=*RS_pn27U3)
	{
		WriteRelayWord(RW_27U3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U3, RELAY_ON);
	}
	// 4.U4
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*3,&U4);
	CaluModValue(&U4);
	*RM_pnUp4= U4.Mod;                                       
	DB_LogWriteRM(RM_UP4, *RM_pnUp4);	
	if(U4.Mod>=*RS_pn59U4)	
	{	
		WriteRelayWord(RW_59U4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U4, RELAY_OFF);
	}
	if(U4.Mod>=*RS_pn27U4)
	{
		WriteRelayWord(RW_27U4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U4, RELAY_ON);
	}
	// 5.U5
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*4,&U5);
	CaluModValue(&U5);
	*RM_pnUp5= U5.Mod;                                       
	DB_LogWriteRM(RM_UP5, *RM_pnUp5);	
	if(U5.Mod>=*RS_pn59U5)	
	{	
		WriteRelayWord(RW_59U5, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U5, RELAY_OFF);
	}
	if(U5.Mod>=*RS_pn27U5)
	{
		WriteRelayWord(RW_27U5, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U5, RELAY_ON);
	}
	// 6.U6
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*5,&U6);
	CaluModValue(&U6);
	*RM_pnUp6= U6.Mod;                                       
	DB_LogWriteRM(RM_UP6, *RM_pnUp6);	
	if(U6.Mod>=*RS_pn59U6)	
	{	
		WriteRelayWord(RW_59U6, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U6, RELAY_OFF);
	}
	if(U6.Mod>=*RS_pn27U6)
	{
		WriteRelayWord(RW_27U6, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U6, RELAY_ON);
	}
	// 7.U7
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*6,&U7);
	CaluModValue(&U7);
	*RM_pnUp7= U7.Mod;                                       
	DB_LogWriteRM(RM_UP7, *RM_pnUp7);	
	if(U7.Mod>=*RS_pn59U7)	
	{	
		WriteRelayWord(RW_59U7, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U7, RELAY_OFF);
	}
	if(U7.Mod>=*RS_pn27U7)
	{
		WriteRelayWord(RW_27U7, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U7, RELAY_ON);
	}
	// 8.U8
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*7,&U8);
	CaluModValue(&U8);
	*RM_pnUp8= U8.Mod;                                       
	DB_LogWriteRM(RM_UP8, *RM_pnUp8);	
	if(U8.Mod>=*RS_pn59U8)	
	{	
		WriteRelayWord(RW_59U8, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_59U8, RELAY_OFF);
	}
	if(U8.Mod>=*RS_pn27U8)
	{
		WriteRelayWord(RW_27U8, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_27U8, RELAY_ON);
	}

	// 9. I1
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*8,&I1);
	CaluModValue(&I1);
	*RM_pnIp1= I1.Mod;                                       
	DB_LogWriteRM(RM_IP1, *RM_pnIp1);	
	if(I1.Mod>=*RS_pn50I1)	
	{	
		WriteRelayWord(RW_50I1, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I1, RELAY_OFF);
	}
	if(I1.Mod>=*RS_pn37I1)
	{
		WriteRelayWord(RW_37I1, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I1, RELAY_ON);
	}
	// 10. I2
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*9,&I2);
	CaluModValue(&I2);
	*RM_pnIp2= I2.Mod;                                       
	DB_LogWriteRM(RM_IP2, *RM_pnIp2);	
	if(I2.Mod>=*RS_pn50I2)	
	{	
		WriteRelayWord(RW_50I2, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I2, RELAY_OFF);
	}
	if(I2.Mod>=*RS_pn37I2)
	{
		WriteRelayWord(RW_37I2, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I2, RELAY_ON);
	}
	// 11. I3
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*10,&I3);
	CaluModValue(&I3);
	*RM_pnIp3= I3.Mod;                                       
	DB_LogWriteRM(RM_IP3, *RM_pnIp3);	
	if(I3.Mod>=*RS_pn50I3)	
	{	
		WriteRelayWord(RW_50I3, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I3, RELAY_OFF);
	}
	if(I3.Mod>=*RS_pn37I3)
	{
		WriteRelayWord(RW_37I3, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I3, RELAY_ON);
	}
	// 12. I4
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*11,&I4);
	CaluModValue(&I4);
	*RM_pnIp4= I4.Mod;                                       
	DB_LogWriteRM(RM_IP4, *RM_pnIp4);	
	if(I4.Mod>=*RS_pn50I4)	
	{	
		WriteRelayWord(RW_50I4, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I4, RELAY_OFF);
	}
	if(I4.Mod>=*RS_pn37I4)
	{
		WriteRelayWord(RW_37I4, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I4, RELAY_ON);
	}
	// 13. I5
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*12,&I5);
	CaluModValue(&I5);
	*RM_pnIp5= I5.Mod;                                       
	DB_LogWriteRM(RM_IP5, *RM_pnIp5);	
	if(I5.Mod>=*RS_pn50I5)	
	{	
		WriteRelayWord(RW_50I5, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I5, RELAY_OFF);
	}
	if(I5.Mod>=*RS_pn37I5)
	{
		WriteRelayWord(RW_37I5, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I5, RELAY_ON);
	}
	// 14.I6
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*13,&I6);
	CaluModValue(&I6);
	*RM_pnIp6= I6.Mod;                                       
	DB_LogWriteRM(RM_IP6, *RM_pnIp6);	
	if(I6.Mod>=*RS_pn50I6)	
	{	
		WriteRelayWord(RW_50I6, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I6, RELAY_OFF);
	}
	if(I6.Mod>=*RS_pn37I6)
	{
		WriteRelayWord(RW_37I6, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I6, RELAY_ON);
	}
	// 15.I7
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*14,&I7);
	CaluModValue(&I7);
	*RM_pnIp7= I7.Mod;                                       
	DB_LogWriteRM(RM_IP7, *RM_pnIp7);	
	if(I7.Mod>=*RS_pn50I7)	
	{	
		WriteRelayWord(RW_50I7, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I7, RELAY_OFF);
	}
	if(I7.Mod>=*RS_pn37I7)
	{
		WriteRelayWord(RW_37I7, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I7, RELAY_ON);
	}
	// 16.I8
	CaluBaseFourier_S15(dwSampBuf+READ_POINT_NUM*15,&I8);
	CaluModValue(&I8);
	*RM_pnIp8= I8.Mod;                                       
	DB_LogWriteRM(RM_IP8, *RM_pnIp8);	
	if(I8.Mod>=*RS_pn50I8)	
	{	
		WriteRelayWord(RW_50I8, RELAY_ON);
	}
	else
	{
		WriteRelayWord(RW_50I8, RELAY_OFF);
	}
	if(I8.Mod>=*RS_pn37I8)
	{
		WriteRelayWord(RW_37I8, RELAY_OFF);
	}
	else
	{
		WriteRelayWord(RW_37I8, RELAY_ON);
	}
	// 15. Ic1
	CaluBaseFourier_S15(dwComposeBuf,&IC1);
	CaluModValue(&IC1);
	*RM_pnIC1= IC1.Mod;                                       
	DB_LogWriteRM(RM_IC1, *RM_pnIC1);	
	// 16. Ic2
	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&IC2);
	CaluModValue(&IC2);
	*RM_pnIC2= IC2.Mod;                                       
	DB_LogWriteRM(RM_IC2, *RM_pnIC2);

}
/****************************************************************************************
名  称: InitRelR50IC1
功  能: 初始化差流1动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::InitRelR50IC1()
{
	
	RelR50IC1.pElecSet			= RS_pn60I_AT1C;          			//过量保护电量定值
	RelR50IC1.pTimeSet			= RS_pn60I_AT1T;				 	//过量保护时间定值
	RelR50IC1.pHarmKhr1Set		= NULL;							//过量保护谐波加权系数
	RelR50IC1.pVolLockSet		= NULL;				 	//过量保护电压闭锁定值
	RelR50IC1.pAccActTimeSet	= NULL;							//过量保护后加速动作时间定值
	RelR50IC1.pH2Coef			= NULL;							//二次谐波闭锁系数，谐波加权系数的放大倍数
	
	RelR50IC1.pRelCFG			= CFG_pby60I_AT1;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IC1.pRelSW			= SW_pby60I_AT1;         			//过量保护软压板------1:投入，2:退出
	RelR50IC1.pRelHLockCFG		= &RelCFGCLR;			    	//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IC1.pRelHSuprCFG		= &RelCFGCLR; 					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IC1.pRelPDCFG			= &RelCFGCLR;		    		//功率方向功能控制位--1:投入，2:退出
	RelR50IC1.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IC1.pRelFDCFG			= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IC1.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IC1.pRelFLCFG			= &RelCFGCLR;					//故障测距投入
	RelR50IC1.pSysTimer			= &dwRelayMSCnt;				//系统定时器指针	
	
	RelR50IC1.pRelElec 			= &IC1;							//当前电量指针		
	RelR50IC1.pRelHarm3			= NULL;							//当前三次谐波指针	
	RelR50IC1.pRelHarm5			= NULL;							//当前五次谐波指针	
	RelR50IC1.pRelVolt			= NULL;							//当前用来闭锁的电压的指针
	RelR50IC1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IC1.R47ULKRelayWord   = FALSE1;

	RelR50IC1.StartRelayWord	= RW_PI60I_AT1;					//过量保护入段继电器字
	RelR50IC1.ActRelayWord		= RW_TR60I_AT1;					//过量保护动作继电器字
	RelR50IC1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IC1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IC1.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IC1.PRewardRelayWord  = FALSE1;						//功率反方向继电器字
	RelR50IC1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IC1.LockRelayWord		= RW_LKPI60I_AT1;					//入段闭锁继电器字
	RelR50IC1.RSTRelayWord		= RW_LKRST60I_AT1;				//返回闭锁继电器字
	RelR50IC1.LoopCtrlRelayWord = RW_LPC60I_AT1;					//保护循环控制继电器字
	
	RelR50IC1.wReportNum		= R_60I_1AT;						//过量保护报告序号
	RelR50IC1.wAccReportNum		= 0xFFFF;						//加速动作报告序号

	RelR50IC1.bMakeActRpt		= TRUE;
	RelR50IC1.dwK35				= NULL;
	RelR50IC1.RptSerialNum		= &RptSerialNumOfAct_AT1;						
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IC1.wPickUpReportNum	= R_60I_1AT_PI;						//入段报告序列号
	RelR50IC1.wResetReportNum	= R_60I_1AT_RES;						//返回报告序列号
	//**********************************************************************	


//	RelR50IC1.dwRelTimer		= 0;			 			//定时器
	RelR50IC1.byRelFCounter		= 0;						//故障计数器
	RelR50IC1.byRelRCounter		= 0;						//返回计数器	
	RelR50IC1.wExcepWordNum     = 8;

}
 /****************************************************************************************
 名  称: InitRelR50IT1
 功  能: 初始化1ATT线过流动作元件
 参  数: 无
 返回值: 无
 *****************************************************************************************/
 void CRelayTask::InitRelR50IT1()
{
	RelR50IT1.pElecSet			= RS_pn50T_AT1C;					//过量保护电量定值
	RelR50IT1.pTimeSet			= RS_pn50T_AT1T;					//过量保护时间定值
	RelR50IT1.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IT1.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IT1.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IT1.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IT1.pRelCFG			= CFG_pby50T_AT1;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IT1.pRelSW			= SW_pby50T_AT1;					//过量保护软压板------1:投入，2:退出
	RelR50IT1.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IT1.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IT1.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IT1.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IT1.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IT1.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IT1.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IT1.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IT1.pRelElec			= &I1; 						//当前电量指针		
	RelR50IT1.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IT1.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IT1.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IT1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IT1.R47ULKRelayWord   = FALSE1;

	RelR50IT1.StartRelayWord	= RW_PI50T_AT1; 				//过量保护入段继电器字
	RelR50IT1.ActRelayWord		= RW_TR50T_AT1; 				//过量保护动作继电器字
	RelR50IT1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IT1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IT1.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IT1.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IT1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IT1.LockRelayWord 	= RW_LKPI50T_AT1;					//入段闭锁继电器字
	RelR50IT1.RSTRelayWord		= RW_LKRST50T_AT1;				//返回闭锁继电器字
	RelR50IT1.LoopCtrlRelayWord = RW_LPC50T_AT1;					//保护循环控制继电器字

	RelR50IT1.wReportNum		= R_50T_1AT;						//过量保护报告序号
	RelR50IT1.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IT1.bMakeActRpt		= TRUE;
	RelR50IT1.dwK35 			= NULL;
	RelR50IT1.RptSerialNum		= &RptSerialNumOfAct_AT1;						
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IT1.wPickUpReportNum	= R_50T_1AT_PI;						//入段报告序列号
	RelR50IT1.wResetReportNum	= R_50T_1AT_RES;						//返回报告序列号
	//**********************************************************************	

//	RelR50IT1.dwRelTimer		= 0;						//定时器
	RelR50IT1.byRelFCounter 	= 0;						//故障计数器
	RelR50IT1.byRelRCounter 	= 0;						//返回计数器	
	RelR50IT1.wExcepWordNum 	= 8;

} 
/****************************************************************************************
名  称: InitRelR50IF1
功  能: 初始化1ATF线过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::InitRelR50IF1()
{

	RelR50IF1.pElecSet			= RS_pn50F_AT1C;					//过量保护电量定值
	RelR50IF1.pTimeSet			= RS_pn50F_AT1T;					//过量保护时间定值
	RelR50IF1.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IF1.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IF1.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IF1.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IF1.pRelCFG			= CFG_pby50F_AT1;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IF1.pRelSW			= SW_pby50F_AT1;					//过量保护软压板------1:投入，2:退出
	RelR50IF1.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IF1.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IF1.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IF1.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IF1.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IF1.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IF1.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IF1.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IF1.pRelElec			= &I2;						//当前电量指针		
	RelR50IF1.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IF1.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IF1.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IF1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IF1.R47ULKRelayWord   = FALSE1;

	RelR50IF1.StartRelayWord	= RW_PI50F_AT1; 				//过量保护入段继电器字
	RelR50IF1.ActRelayWord		= RW_TR50F_AT1; 				//过量保护动作继电器字
	RelR50IF1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IF1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IF1.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IF1.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IF1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IF1.LockRelayWord 	= RW_LKPI50F_AT1;					//入段闭锁继电器字
	RelR50IF1.RSTRelayWord		= RW_LKRST50F_AT1;				//返回闭锁继电器字
	RelR50IF1.LoopCtrlRelayWord = RW_LPC50F_AT1;					//保护循环控制继电器字

	RelR50IF1.wReportNum		= R_50F_1AT;						//过量保护报告序号
	RelR50IF1.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IF1.bMakeActRpt		= TRUE;
	RelR50IF1.dwK35 			= NULL;
	RelR50IF1.RptSerialNum		= &RptSerialNumOfAct_AT1;		
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IF1.wPickUpReportNum	= R_50F_1AT_PI;						//入段报告序列号
	RelR50IF1.wResetReportNum	= R_50F_1AT_RES;						//返回报告序列号
	//**********************************************************************	
//	RelR50IF1.dwRelTimer		= 0;						//定时器
	RelR50IF1.byRelFCounter 	= 0;						//故障计数器
	RelR50IF1.byRelRCounter 	= 0;						//返回计数器	
	RelR50IF1.wExcepWordNum 	= 8;

} 
/****************************************************************************************
名  称: InitRelR50IP1
功  能: 初始化1AT碰壳过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::InitRelR50IP1()
{
	RelR50IP1.pElecSet			= RS_pn50CS_AT1C;					//过量保护电量定值
	RelR50IP1.pTimeSet			= RS_pn50CS_AT1T;					//过量保护时间定值
	RelR50IP1.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IP1.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IP1.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IP1.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IP1.pRelCFG			= CFG_pby50CS_AT1;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IP1.pRelSW			= SW_pby50CS_AT1;					//过量保护软压板------1:投入，2:退出
	RelR50IP1.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IP1.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IP1.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IP1.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IP1.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IP1.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IP1.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IP1.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IP1.pRelElec			= &I3;						//当前电量指针		
	RelR50IP1.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IP1.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IP1.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IP1.pRel47UCFG		= &RelCFGCLR;;
	RelR50IP1.R47ULKRelayWord   = FALSE1;

	RelR50IP1.StartRelayWord	= RW_PI50CS_AT1; 				//过量保护入段继电器字
	RelR50IP1.ActRelayWord		= RW_TR50CS_AT1; 				//过量保护动作继电器字
	RelR50IP1.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IP1.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IP1.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IP1.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IP1.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IP1.LockRelayWord 	= RW_LKPI50CS_AT1;					//入段闭锁继电器字
	RelR50IP1.RSTRelayWord		= RW_LKRST50CS_AT1;				//返回闭锁继电器字
	RelR50IP1.LoopCtrlRelayWord = RW_LPC50CS_AT1;					//保护循环控制继电器字

	RelR50IP1.wReportNum		= R_50CS_1AT;						//过量保护报告序号
	RelR50IP1.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IP1.bMakeActRpt		= TRUE;
	RelR50IP1.dwK35 			= NULL;
	RelR50IP1.RptSerialNum		= &RptSerialNumOfAct_AT1;		
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IP1.wPickUpReportNum	= R_50CS_1AT_PI;						//入段报告序列号
	RelR50IP1.wResetReportNum	= R_50CS_1AT_RES;						//返回报告序列号
	//**********************************************************************	
//	RelR50IP1.dwRelTimer		= 0;						//定时器
	RelR50IP1.byRelFCounter 	= 0;						//故障计数器
	RelR50IP1.byRelRCounter 	= 0;						//返回计数器	
	RelR50IP1.wExcepWordNum 	= 8;

}
/****************************************************************************************
名  称: InitRelR50IC2
功  能: 初始化2AT差流过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::InitRelR50IC2()
{
	RelR50IC2.pElecSet			= RS_pn60I_AT2C;					//过量保护电量定值
	RelR50IC2.pTimeSet			= RS_pn60I_AT2T;					//过量保护时间定值
	RelR50IC2.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IC2.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IC2.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IC2.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IC2.pRelCFG			= CFG_pby60I_AT2;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IC2.pRelSW			= SW_pby60I_AT2;					//过量保护软压板------1:投入，2:退出
	RelR50IC2.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IC2.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IC2.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IC2.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IC2.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IC2.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IC2.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IC2.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IC2.pRelElec			= &IC2;						//当前电量指针		
	RelR50IC2.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IC2.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IC2.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IC2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IC2.R47ULKRelayWord   = FALSE1;

	RelR50IC2.StartRelayWord	= RW_PI60I_AT2; 				//过量保护入段继电器字
	RelR50IC2.ActRelayWord		= RW_TR60I_AT2; 				//过量保护动作继电器字
	RelR50IC2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IC2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IC2.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IC2.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IC2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IC2.LockRelayWord 	= RW_LKPI60I_AT2;					//入段闭锁继电器字
	RelR50IC2.RSTRelayWord		= RW_LKRST60I_AT2;				//返回闭锁继电器字
	RelR50IC2.LoopCtrlRelayWord = RW_LPC60I_AT2;					//保护循环控制继电器字

	RelR50IC2.wReportNum		= R_60I_2AT;						//过量保护报告序号
	RelR50IC2.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IC2.bMakeActRpt		= TRUE;
	RelR50IC2.dwK35 			= NULL;
	RelR50IC2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IC2.wPickUpReportNum	= R_60I_2AT_PI;						//入段报告序列号
	RelR50IC2.wResetReportNum	= R_60I_2AT_RES;						//返回报告序列号
	//**********************************************************************	
//	RelR50IC2.dwRelTimer		= 0;						//定时器
	RelR50IC2.byRelFCounter 	= 0;						//故障计数器
	RelR50IC2.byRelRCounter 	= 0;						//返回计数器	
	RelR50IC2.wExcepWordNum 	= 8;

}
/****************************************************************************************
名  称: InitRelR50IT2
功  能: 初始化2ATT线过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::InitRelR50IT2()
{

	RelR50IT2.pElecSet			= RS_pn50T_AT2C;					//过量保护电量定值
	RelR50IT2.pTimeSet			= RS_pn50T_AT2T;					//过量保护时间定值
	RelR50IT2.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IT2.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IT2.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IT2.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IT2.pRelCFG			= CFG_pby50T_AT2;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IT2.pRelSW			= SW_pby50T_AT2;					//过量保护软压板------1:投入，2:退出
	RelR50IT2.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IT2.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IT2.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IT2.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IT2.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IT2.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IT2.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IT2.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IT2.pRelElec			= &I4; 					//当前电量指针		
	RelR50IT2.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IT2.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IT2.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IT2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IT2.R47ULKRelayWord   = FALSE1;

	RelR50IT2.StartRelayWord	= RW_PI50T_AT2; 				//过量保护入段继电器字
	RelR50IT2.ActRelayWord		= RW_TR50T_AT2; 				//过量保护动作继电器字
	RelR50IT2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IT2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IT2.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IT2.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IT2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IT2.LockRelayWord 	= RW_LKPI50T_AT2;					//入段闭锁继电器字
	RelR50IT2.RSTRelayWord		= RW_LKRST50T_AT2;				//返回闭锁继电器字
	RelR50IT2.LoopCtrlRelayWord = RW_LPC50T_AT2;					//保护循环控制继电器字

	RelR50IT2.wReportNum		= R_50T_2AT;						//过量保护报告序号
	RelR50IT2.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IT2.bMakeActRpt		= TRUE;
	RelR50IT2.dwK35 			= NULL;
	RelR50IT2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IT2.wPickUpReportNum	= R_50T_2AT_PI;						//入段报告序列号
	RelR50IT2.wResetReportNum	= R_50T_2AT_RES;						//返回报告序列号
	//**********************************************************************	
//	RelR50IT2.dwRelTimer		= 0;						//定时器
	RelR50IT2.byRelFCounter 	= 0;						//故障计数器
	RelR50IT2.byRelRCounter 	= 0;						//返回计数器	
	RelR50IT2.wExcepWordNum 	= 8;

}
/****************************************************************************************
名  称: InitRelR50IF2
功  能: 初始化2ATF线过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/

void CRelayTask::InitRelR50IF2()
{

	RelR50IF2.pElecSet			= RS_pn50F_AT2C;					//过量保护电量定值
	RelR50IF2.pTimeSet			= RS_pn50F_AT2T;					//过量保护时间定值
	RelR50IF2.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IF2.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IF2.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IF2.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IF2.pRelCFG			= CFG_pby50F_AT2;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IF2.pRelSW			= SW_pby50F_AT2;					//过量保护软压板------1:投入，2:退出
	RelR50IF2.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IF2.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IF2.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IF2.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IF2.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IF2.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IF2.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IF2.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IF2.pRelElec			= &I5;					//当前电量指针		
	RelR50IF2.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IF2.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IF2.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IF2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IF2.R47ULKRelayWord   = FALSE1;

	RelR50IF2.StartRelayWord	= RW_PI50F_AT2; 				//过量保护入段继电器字
	RelR50IF2.ActRelayWord		= RW_TR50F_AT2; 				//过量保护动作继电器字
	RelR50IF2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IF2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IF2.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IF2.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IF2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IF2.LockRelayWord 	= RW_LKPI50F_AT2;					//入段闭锁继电器字
	RelR50IF2.RSTRelayWord		= RW_LKRST50F_AT2;				//返回闭锁继电器字
	RelR50IF2.LoopCtrlRelayWord = RW_LPC50F_AT2;					//保护循环控制继电器字

	RelR50IF2.wReportNum		= R_50F_2AT;						//过量保护报告序号
	RelR50IF2.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IF2.bMakeActRpt		= TRUE;
	RelR50IF2.dwK35 			= NULL;
	RelR50IF2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IF2.wPickUpReportNum	= R_50F_2AT_PI;						//入段报告序列号
	RelR50IF2.wResetReportNum	= R_50F_2AT_RES;						//返回报告序列号
	//**********************************************************************	
//	RelR50IF2.dwRelTimer		= 0;						//定时器
	RelR50IF2.byRelFCounter 	= 0;						//故障计数器
	RelR50IF2.byRelRCounter 	= 0;						//返回计数器	
	RelR50IF2.wExcepWordNum 	= 8;

}
/****************************************************************************************
名  称: InitRelR50IP2
功  能: 初始化2AT碰壳过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::InitRelR50IP2()
{
	
	RelR50IP2.pElecSet			= RS_pn50CS_AT2C;					//过量保护电量定值
	RelR50IP2.pTimeSet			= RS_pn50CS_AT2T;					//过量保护时间定值
	RelR50IP2.pHarmKhr1Set		= NULL; 						//过量保护谐波加权系数
	RelR50IP2.pVolLockSet		= NULL; 				//过量保护电压闭锁定值
	RelR50IP2.pAccActTimeSet	= NULL; 						//过量保护后加速动作时间定值
	RelR50IP2.pH2Coef			= NULL; 						//二次谐波闭锁系数，谐波加权系数的放大倍数

	RelR50IP2.pRelCFG			= CFG_pby50CS_AT2;				//过量保护功能控制字--1:投入，2:退出 
	RelR50IP2.pRelSW			= SW_pby50CS_AT2;					//过量保护软压板------1:投入，2:退出
	RelR50IP2.pRelHLockCFG		= &RelCFGCLR;					//谐波闭锁功能控制位--1:投入，2:退出
	RelR50IP2.pRelHSuprCFG		= &RelCFGCLR;					//谐波抑制功能控制位--1:投入，2:退出
	RelR50IP2.pRelPDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:投入，2:退出
	RelR50IP2.pRelVolLockCFG	= &RelCFGCLR;				//低压闭锁功能控制位--1:投入，2:退出
	RelR50IP2.pRelFDCFG 		= &RelCFGCLR;					//功率方向功能控制位--1:正方向，2:反方向
	RelR50IP2.pRelAccCFG		= &RelCFGCLR;					//有无后加速功能------1:有，  2:无
	RelR50IP2.pRelFLCFG 		= &RelCFGCLR;					//故障测距投入
	RelR50IP2.pSysTimer 		= &dwRelayMSCnt;				//系统定时器指针	

	RelR50IP2.pRelElec			= &I6;					//当前电量指针		
	RelR50IP2.pRelHarm3 		= NULL; 						//当前三次谐波指针	
	RelR50IP2.pRelHarm5 		= NULL; 						//当前五次谐波指针	
	RelR50IP2.pRelVolt			= NULL; 						//当前用来闭锁的电压的指针
	RelR50IP2.pRel47UCFG		= &RelCFGCLR;;
	RelR50IP2.R47ULKRelayWord   = FALSE1;

	RelR50IP2.StartRelayWord	= RW_PI50CS_AT2; 				//过量保护入段继电器字
	RelR50IP2.ActRelayWord		= RW_TR50CS_AT2; 				//过量保护动作继电器字
	RelR50IP2.AccRelayWord		= FALSE1;						//后加速允许继电器字
	RelR50IP2.AccActRelayWord	= FALSE1;						//后加速动作继电器字
	RelR50IP2.PForwardRelayWord = FALSE1;						//功率正方向继电器字
	RelR50IP2.PRewardRelayWord	= FALSE1;						//功率反方向继电器字
	RelR50IP2.H2LockRelayWord	= FALSE1;						//二次谐波闭锁继电器字
	RelR50IP2.LockRelayWord 	= RW_LKPI50CS_AT2;					//入段闭锁继电器字
	RelR50IP2.RSTRelayWord		= RW_LKRST50CS_AT2;				//返回闭锁继电器字
	RelR50IP2.LoopCtrlRelayWord = RW_LPC50CS_AT2;					//保护循环控制继电器字

	RelR50IP2.wReportNum		= R_50CS_2AT;						//过量保护报告序号
	RelR50IP2.wAccReportNum 	= 0xFFFF;						//加速动作报告序号

	RelR50IP2.bMakeActRpt		= TRUE;
	RelR50IP2.dwK35 			= NULL;
	RelR50IP2.RptSerialNum		= &RptSerialNumOfAct_AT2;		
	//**********************入段返回报告部分*************	yanxs  2013-03-22
	RelR50IP2.wPickUpReportNum	= R_50CS_2AT_PI;						//入段报告序列号
	RelR50IP2.wResetReportNum	= R_50CS_2AT_RES;						//返回报告序列号
	//**********************************************************************	
//	RelR50IP2.dwRelTimer		= 0;						//定时器
	RelR50IP2.byRelFCounter 	= 0;						//故障计数器
	RelR50IP2.byRelRCounter 	= 0;						//返回计数器	
	RelR50IP2.wExcepWordNum 	= 8;
}
void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
	StructCheck[0].pstruct = (WORD*)&RelR50IC1;
	StructCheck[0].bigsize = sizeof(RelR50IC1)/2 - RelR50IC1.wExcepWordNum;		
	StructCheck[1].pstruct = (WORD*)&RelR50IT1;
	StructCheck[1].bigsize = sizeof(RelR50IT1)/2 - RelR50IT1.wExcepWordNum;	
	StructCheck[2].pstruct = (WORD*)&RelR50IF1;
	StructCheck[2].bigsize = sizeof(RelR50IF1)/2 - RelR50IF1.wExcepWordNum; 
	StructCheck[3].pstruct = (WORD*)&RelR50IP1;
	StructCheck[3].bigsize = sizeof(RelR50IP1)/2 - RelR50IP1.wExcepWordNum; 
	StructCheck[4].pstruct = (WORD*)&RelR50IC2;
	StructCheck[4].bigsize = sizeof(RelR50IC2)/2 - RelR50IC2.wExcepWordNum; 
	StructCheck[5].pstruct = (WORD*)&RelR50IT2;
	StructCheck[5].bigsize = sizeof(RelR50IT2)/2 - RelR50IT2.wExcepWordNum; 
	StructCheck[6].pstruct = (WORD*)&RelR50IF2;
	StructCheck[6].bigsize = sizeof(RelR50IF2)/2 - RelR50IF2.wExcepWordNum; 
	StructCheck[7].pstruct = (WORD*)&RelR50IP2;
	StructCheck[7].bigsize = sizeof(RelR50IP2)/2 - RelR50IP2.wExcepWordNum; 
	StructCheck[8].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[8].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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

/****************************************************************************************
名  称: OnRelayChange
功  能: 保护参数修改后的响应函数，重新初始化各参数以及保护元件
参  数: 无
返回值: 无
*****************************************************************************************/
void CRelayTask::OnRelayChange(DWORD dwDataType,DWORD dwIndex)
{
	::InitAllRelayMem();
	InitRealy();
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

/************************************************************************************************
功能: 向数字通道中写入录波数据
参数: 无
返回: 无
备注:
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
功能: 初始化数字通道录波相关信息
参数: 无
返回: 无
备注:
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
// 功能: 模值启动判别程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK_AT1(void)
{
	BYTE  pChannel[]={CHANNEL_6534_I1,CHANNEL_6534_I2,CHANNEL_6534_I3,CHANNEL_6534_IC1};
		
	if(ReadRelayWord(RW_52BF_AT1))  //断路器跳闸失败，不进行保护启动判断处理 
	{
		WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
		return FALSE;
	}
	if((*RelR50IC1.pRelCFG == CFG_STATUS_SET) && (*RelR50IC1.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
			if(RelR50IC1.pRelElec->Mod > *RelR50IC1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
	
	if((*RelR50IT1.pRelCFG == CFG_STATUS_SET) && (*RelR50IT1.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IT1.pRelElec->Mod > *RelR50IT1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IF1.pRelCFG == CFG_STATUS_SET) && (*RelR50IF1.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IF1.pRelElec->Mod > *RelR50IF1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IP1.pRelCFG == CFG_STATUS_SET) && (*RelR50IP1.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IP1.pRelElec->Mod > *RelR50IP1.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT1);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT1 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT1 = g_RptSerialNum;
			return TRUE;
		}
	}
//突变量启动判断部分
	//至少有一个保护投入
	if(((*RelR50IC1.pRelCFG == CFG_STATUS_SET) && (*RelR50IC1.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IT1.pRelCFG == CFG_STATUS_SET) && (*RelR50IT1.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IF1.pRelCFG == CFG_STATUS_SET) && (*RelR50IF1.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IP1.pRelCFG == CFG_STATUS_SET) && (*RelR50IP1.pRelSW == SW_STATUS_SET)))
	
	{
		
		BYTE  pChannel[]={CHANNEL_6534_I1,CHANNEL_6534_I2,CHANNEL_6534_I3,CHANNEL_6534_IC1};
		ReadPreSingleData(dwPickDataBuf_AT1,pChannel,4,RELAY_SAMP_INTERVAL);		
		//算法1，根据宏RELAY_SAMP_INTERVAL确定的通用算法
		//将3个周波的采样点分别取至相应的数组,并计算突变量,且进行判断
		//注意:RELAY_SAMP_INTERVAL的值不能小于3
		for(LONG j=0;j<4;j++)
		{
			PickCheck_AT1 = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1_AT1 = dwPickDataBuf_AT1[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2_AT1 = dwPickDataBuf_AT1[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3_AT1 = dwPickDataBuf_AT1[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue_AT1 = MULTIPLES*ToAbs(((lSampData1_AT1-lSampData2_AT1)-(lSampData2_AT1-lSampDate3_AT1)));
				
				//采用浮动门槛值判断，ΔI>0.25*ΔI'+ C,其中ΔI'为上个一个邻近的点的突变量值，C为突变量启动定值
				if(lPickValue_AT1*4>*RS_pnPIC*4+m_lPickUpValue_AT1[j])
				{
					m_wPickCheck_AT1++;
					if(m_wPickCheck_AT1 ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime_AT1);
						PickTime = PickUpTime_AT1.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime_AT1.Lo-(i*625+500)/1000;
						PickUpTime_AT1.Lo= (DWORD)PickTime;
						PickUpTime_AT1.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //向数据库中写入启动报告
						RptSerialNumOfAct_AT1 = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //向数据库中写入启动报告
						RptSerialNumOfStart_AT1 = g_RptSerialNum;
						m_wPickCheck_AT1 =0;
						for(LONG m=0;m<4;m++)
						{
							m_wPickCheckNext_AT1[m] =0;
							m_lPickUpValue_AT1[m]=0; 
						}
						
						return TRUE;	
					}
					if(i<2)
					{
						m_wPickCheckNext_AT1[j]++;
						if(m_wPickCheckNext_AT1[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime_AT1);
							PickTime = PickUpTime_AT1.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime_AT1.Lo-(i*625+500)/1000;
							PickUpTime_AT1.Lo= (DWORD)PickTime;
							PickUpTime_AT1.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_PICKUP_AT1, RELAY_ON);
							//ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT1,FALSE); //向数据库中写入启动报告
							RptSerialNumOfAct_AT1 = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT1,TRUE); //向数据库中写入启动报告
							RptSerialNumOfStart_AT1 = g_RptSerialNum;
							m_wPickCheck_AT1 =0;
							for(LONG m=0;m<4;m++)
							{
								m_wPickCheckNext_AT1[m] =0;
								m_lPickUpValue_AT1[m]=0;
							}
							
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						m_wPickCheckNext_AT1[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						m_wPickCheckNext_AT1[j]++;
				}
				else
				{
					m_wPickCheck_AT1 =0;
					m_wPickCheckNext_AT1[j]=0;
				}
				m_lPickUpValue_AT1[j] = lPickValue_AT1;
			}
		}
	}
	
	WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
	return FALSE;
}
//------------------------------------------------------------------------------------------ 
// 功能: 模值启动判别程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
BOOL CRelayTask::RelayPickUpCHK_AT2(void)
{
	BYTE  pChannel[]={CHANNEL_6534_I4,CHANNEL_6534_I5,CHANNEL_6534_I6,CHANNEL_6534_IC2};
	
	if(ReadRelayWord(RW_52BF_AT2))  //断路器跳闸失败，不进行保护启动判断处理 
	{
		WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
		return FALSE;
	}
	if((*RelR50IC2.pRelCFG == CFG_STATUS_SET) && (*RelR50IC2.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IC2.pRelElec->Mod > *RelR50IC2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
	
	if((*RelR50IT2.pRelCFG == CFG_STATUS_SET) && (*RelR50IT2.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IT2.pRelElec->Mod > *RelR50IT2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IF2.pRelCFG == CFG_STATUS_SET) && (*RelR50IF2.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IF2.pRelElec->Mod > *RelR50IF2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
	if((*RelR50IP2.pRelCFG == CFG_STATUS_SET) && (*RelR50IP2.pRelSW == SW_STATUS_SET))   //速断保护投入 
	{
		if(RelR50IP2.pRelElec->Mod > *RelR50IP2.pElecSet)
		{
			WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
			ReadAbsTime(&PickUpTime_AT2);
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //向数据库中写入启动报告
			RptSerialNumOfAct_AT2 = g_RptSerialNum;
			DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //向数据库中写入启动报告
			RptSerialNumOfStart_AT2 = g_RptSerialNum;
			return TRUE;
		}
	}
//突变量启动判断部分
	//至少有一个保护投入
	if(((*RelR50IC2.pRelCFG == CFG_STATUS_SET) && (*RelR50IC2.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IT2.pRelCFG == CFG_STATUS_SET) && (*RelR50IT2.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IF2.pRelCFG == CFG_STATUS_SET) && (*RelR50IF2.pRelSW == SW_STATUS_SET)) || \
		((*RelR50IP2.pRelCFG == CFG_STATUS_SET) && (*RelR50IP2.pRelSW == SW_STATUS_SET)))
	
	{
		
		BYTE  pChannel[]={CHANNEL_6534_I4,CHANNEL_6534_I5,CHANNEL_6534_I6,CHANNEL_6534_IC2};
		ReadPreSingleData(dwPickDataBuf_AT2,pChannel,4,RELAY_SAMP_INTERVAL);		
		//算法1，根据宏RELAY_SAMP_INTERVAL确定的通用算法
		//将3个周波的采样点分别取至相应的数组,并计算突变量,且进行判断
		//注意:RELAY_SAMP_INTERVAL的值不能小于3
		for(LONG j=0;j<4;j++)
		{
			PickCheck_AT2 = 0;
			for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)
			{
				lSampData1_AT2 = dwPickDataBuf_AT2[2*RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampData2_AT2 = dwPickDataBuf_AT2[RELAY_SAMP_INTERVAL+i+j*3*RELAY_SAMP_INTERVAL];
				lSampDate3_AT2 = dwPickDataBuf_AT2[i+j*3*RELAY_SAMP_INTERVAL];
				lPickValue_AT2 = MULTIPLES*ToAbs(((lSampData1_AT2-lSampData2_AT2)-(lSampData2_AT2-lSampDate3_AT2)));
				
				//采用浮动门槛值判断，ΔI>0.25*ΔI'+ C,其中ΔI'为上个一个邻近的点的突变量值，C为突变量启动定值
				if(lPickValue_AT2*4>*RS_pnPIC*4+m_lPickUpValue_AT2[j])
				{
					m_wPickCheck_AT2++;
					if(m_wPickCheck_AT2 ==3)
					{						
						QWORD PickTime;						
						ReadAbsTime(&PickUpTime_AT2);
						PickTime = PickUpTime_AT2.Hi;
						PickTime <<= 32;
						PickTime += PickUpTime_AT2.Lo-(i*625+500)/1000;
						PickUpTime_AT2.Lo= (DWORD)PickTime;
						PickUpTime_AT2.Hi= (WORD)(PickTime>>32);	
						
						WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //向数据库中写入启动报告
						RptSerialNumOfAct_AT2 = g_RptSerialNum;
						DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //向数据库中写入启动报告
						RptSerialNumOfStart_AT2 = g_RptSerialNum;
						m_wPickCheck_AT2 =0;
						for(LONG m=0;m<4;m++)
						{
							m_wPickCheckNext_AT2[m] =0;
							m_lPickUpValue_AT2[m]=0; 
						}
						
						return TRUE;	
					}
					if(i<2)
					{
						m_wPickCheckNext_AT2[j]++;
						if(m_wPickCheckNext_AT2[j] ==3)
						{							
							QWORD PickTime; 					
							ReadAbsTime(&PickUpTime_AT2);
							PickTime = PickUpTime_AT2.Hi;
							PickTime <<= 32;
							PickTime += PickUpTime_AT2.Lo-(i*625+500)/1000;
							PickUpTime_AT2.Lo= (DWORD)PickTime;
							PickUpTime_AT2.Hi= (WORD)(PickTime>>32);	

							WriteRelayWord(RW_PICKUP_AT2, RELAY_ON);
							//ReadAbsTime(&PickUpTime);
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_ACTPICK].wInf,&PickUpTime_AT2,FALSE); //向数据库中写入启动报告
							RptSerialNumOfAct_AT2 = g_RptSerialNum;
							DB_RelayStart(++g_RptSerialNum,ReportInf[R_PICKUP].wInf,&PickUpTime_AT2,TRUE); //向数据库中写入启动报告
							RptSerialNumOfStart_AT2 = g_RptSerialNum;
							m_wPickCheck_AT2 =0;
							for(LONG m=0;m<4;m++)
							{
								m_wPickCheckNext_AT2[m] =0;
								m_lPickUpValue_AT2[m]=0;
							}
							
							return TRUE;	
						}
					}
					if((i>=2) && ((RELAY_SAMP_INTERVAL-i) >= 3))
						m_wPickCheckNext_AT2[j] =0;
					
					if((RELAY_SAMP_INTERVAL-i) < 3)
						m_wPickCheckNext_AT2[j]++;
				}
				else
				{
					m_wPickCheck_AT2 =0;
					m_wPickCheckNext_AT2[j]=0;
				}
				m_lPickUpValue_AT2[j] = lPickValue_AT2;
			}
		}
	}
	
	WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
	return FALSE;
}

//------------------------------------------------------------------------------------------ 
// 功能: 保护循环控制程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
void CRelayTask::FalutProcess_AT1(void)
{ 	
	if(FaultOnFlag_AT1 ==FALSE)   						//尚未进入故障处理
	{		
		RelayPickUpCHK_AT1();					//执行启动检测
		if(ReadRelayWord(RW_PICKUP_AT1))
		{
			ResetAllRelay_AT1(TRUE);
			FaultSINum_AT1 = RELAY_SAMP_INTERVAL;       //保护启动，初始化故障处理采样间隔计数器
			FaultOnFlag_AT1 = TRUE;
		}
		else
			return;		
	}
	FaultSINum_AT1 += RELAY_SAMP_INTERVAL;				//故障处理采样间隔计数器累加
	if(FaultSINum_AT1<POINT_PER_PERIOD)					//故障处理采样间隔计数器小于一周波采样点数
	{
		return;
	}
	else
	{
		if(HaveMakeStartRptFlag_AT1 == FALSE)
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart_AT1);
			HaveMakeStartRptFlag_AT1 = TRUE;
//			ReportMsg("relay start report!!!!!");
		}
		
		GeneralOverValueRelay(&RelR50IC1);				//调用速断子程序
		GeneralOverValueRelay(&RelR50IT1);				//调用过流保护子程序
		GeneralOverValueRelay(&RelR50IF1);				//调用差流1保护子程序
		GeneralOverValueRelay(&RelR50IP1);				//调用差流2保护子程序

		if(ReadRelayWord(RW_52BF_AT1))
		{
			if(!m_bHaveMake52BFRpt_AT1)
			{
				MakeActRpt(R_52BF_AT1,RptSerialNumOfAct_AT1);	//作断路器跳闸失败报告
				m_bHaveMake52BFRpt_AT1 = TRUE;
			}
		}

		if(ReadRelayWord(RelR50IC1.LoopCtrlRelayWord)||ReadRelayWord(RelR50IT1.LoopCtrlRelayWord)||ReadRelayWord(RelR50IF1.LoopCtrlRelayWord)  		//有保护循环控制继电器字被置位
			||ReadRelayWord(RelR50IP1.LoopCtrlRelayWord))
		{	
			if(ReadRelayWord(RW_TR1))
				m_bHaveRelayAct_AT1 = TRUE;
			if(ReadRelayWord(RW_52BF_AT1))			//跳闸失败，清所有标志，作结束报告
			{
				m_bHaveMake52BFRpt_AT1 = FALSE;
				WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
				FaultCheckOutFlag_AT1 = FALSE;
				FaultOnFlag_AT1 = FALSE;
				HaveMakeStartRptFlag_AT1= FALSE;
				DB_RelayEnd(RptSerialNumOfAct_AT1); //向数据库中写入结束报告				
				m_bFaultLKFlag_AT1 = FALSE;
				return;
			}
			else
			{
				FaultLockTime_AT1 = dwRelayMSCnt;		//重置故障锁定定时器 
				FaultCheckOutFlag_AT1 = TRUE;			//有保护循环控制继电器字置位，置检出故障标志
				return;
			}
		}
		else				//无保护循环控制继电器置位: 1、故障处理已经结束；2、未发生过故障
		{
			if(FaultCheckOutFlag_AT1 ==TRUE	)      // 1、故障处理已经结束
			{	
				if(ReadRelayWord(RW_T52BF_AT1))
				{
					FaultLockTime_AT1 = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime_AT1>1000 )      //故障处理结束，无故障时间超过1秒，复位保护启动继电器字，清标志，作结束报告
				{	
					WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
					FaultCheckOutFlag_AT1 = FALSE;
					FaultOnFlag_AT1 = FALSE;
					HaveMakeStartRptFlag_AT1 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT1);						
					m_bFaultLKFlag_AT1 = FALSE;
					ResetAllRelay_AT1(TRUE);					
					m_bHaveMake52BFRpt_AT1 = FALSE;
					return;					
				}
				else
					return;
			}
			else							//未发生过故障
			{
				if(m_bFaultLKFlag_AT1 == FALSE)     //故障刚启动，记录起始故障时刻
				{	
					FaultLockTime_AT1 = dwRelayMSCnt;	
					m_bFaultLKFlag_AT1 = TRUE;					
				}
				
				if(dwRelayMSCnt -FaultLockTime_AT1>500 )	//未发生过故障无故障时间超过0.5秒，复位保护启动继电器字，清标志，作结束报告
				{
					WriteRelayWord(RW_PICKUP_AT1, RELAY_OFF);
					FaultCheckOutFlag_AT1 = FALSE;
					FaultOnFlag_AT1 = FALSE;
					HaveMakeStartRptFlag_AT1 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT1); 
					m_bFaultLKFlag_AT1 = FALSE;
					ResetAllRelay_AT1(TRUE);					
					m_bHaveMake52BFRpt_AT1 = FALSE;
					return;					
				}
				else
					return;
			}			
		}		
	}
}
//------------------------------------------------------------------------------------------ 
// 功能: 保护循环控制程序							                                 
// 输入: 无                                                                                  
// 输出:                                                              
// 返回: 无                                                                                   
//------------------------------------------------------------------------------------------ 
void CRelayTask::FalutProcess_AT2(void)
{ 	
	if(FaultOnFlag_AT2 ==FALSE)   						//尚未进入故障处理
	{		
		RelayPickUpCHK_AT2();					//执行启动检测
		if(ReadRelayWord(RW_PICKUP_AT2))
		{
			ResetAllRelay_AT2(TRUE);
			FaultSINum_AT2 = RELAY_SAMP_INTERVAL;       //保护启动，初始化故障处理采样间隔计数器
			FaultOnFlag_AT2 = TRUE;
		}
		else
			return;		
	}
	FaultSINum_AT2 += RELAY_SAMP_INTERVAL;				//故障处理采样间隔计数器累加
	if(FaultSINum_AT2<POINT_PER_PERIOD)					//故障处理采样间隔计数器小于一周波采样点数
	{
		return;
	}
	else
	{
		if(HaveMakeStartRptFlag_AT2 == FALSE)
		{
			MakeStartRpt(R_PICKUP,RptSerialNumOfStart_AT2);
			HaveMakeStartRptFlag_AT2 = TRUE;
		}
		
		GeneralOverValueRelay(&RelR50IC2);				//调用速断子程序
		GeneralOverValueRelay(&RelR50IT2);				//调用过流保护子程序
		GeneralOverValueRelay(&RelR50IF2);				//调用差流1保护子程序
		GeneralOverValueRelay(&RelR50IP2);				//调用差流2保护子程序

		if(ReadRelayWord(RW_52BF_AT2))
		{
			if(!m_bHaveMake52BFRpt_AT2)
			{
				MakeActRpt(R_52BF_AT2,RptSerialNumOfAct_AT2);   //作断路器跳闸失败报告
				m_bHaveMake52BFRpt_AT2 = TRUE;
			}
		}

		if(ReadRelayWord(RelR50IC2.LoopCtrlRelayWord)||ReadRelayWord(RelR50IT2.LoopCtrlRelayWord)||ReadRelayWord(RelR50IF2.LoopCtrlRelayWord)  		//有保护循环控制继电器字被置位
			||ReadRelayWord(RelR50IP2.LoopCtrlRelayWord))
		{			
			if(ReadRelayWord(RW_TR2))
				m_bHaveRelayAct_AT2 = TRUE;
			if(ReadRelayWord(RW_52BF_AT2))			//跳闸失败，清所有标志，作结束报告
			{
				m_bHaveMake52BFRpt_AT2 = FALSE;
				WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
				FaultCheckOutFlag_AT2 = FALSE;
				FaultOnFlag_AT2 = FALSE;
				HaveMakeStartRptFlag_AT2= FALSE;
				DB_RelayEnd(RptSerialNumOfAct_AT2); //向数据库中写入结束报告				
				m_bFaultLKFlag_AT2 = FALSE;
				return;
			}
			else
			{
				FaultLockTime_AT2 = dwRelayMSCnt;		//重置故障锁定定时器 
				FaultCheckOutFlag_AT2 = TRUE;			//有保护循环控制继电器字置位，置检出故障标志
				return;
			}
		}
		else				//无保护循环控制继电器置位: 1、故障处理已经结束；2、未发生过故障
		{
			if(FaultCheckOutFlag_AT2 ==TRUE	)      // 1、故障处理已经结束
			{				
//				if((m_bHaveRelayAct_AT2 == TRUE)&&(m_bHaveMake52BFRpt_AT2 ==FALSE))
//				{					
//					MakeActRpt(R_52BF_AT2,RptSerialNumOfAct_AT2);	//作断路器跳闸失败报告
//					m_bHaveMake52BFRpt_AT2 = TRUE;
//				}
				if(ReadRelayWord(RW_T52BF_AT2))
				{
					FaultLockTime_AT2 = dwRelayMSCnt;
					return;
				}
				if(dwRelayMSCnt -FaultLockTime_AT2>1000 )      //故障处理结束，无故障时间超过1秒，复位保护启动继电器字，清标志，作结束报告
				{	
					WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
					FaultCheckOutFlag_AT2 = FALSE;
					FaultOnFlag_AT2 = FALSE;
					HaveMakeStartRptFlag_AT2 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT2);						
					m_bFaultLKFlag_AT2 = FALSE;
					ResetAllRelay_AT2(TRUE);					
					m_bHaveMake52BFRpt_AT2 = FALSE;
					return;					
				}
				else
					return;
			}
			else							//未发生过故障
			{
				if(m_bFaultLKFlag_AT2 == FALSE)     //故障刚启动，记录起始故障时刻
				{	
					FaultLockTime_AT2 = dwRelayMSCnt;	
					m_bFaultLKFlag_AT2 = TRUE;					
				}
				
				if(dwRelayMSCnt -FaultLockTime_AT2>500 )	//未发生过故障无故障时间超过0.5秒，复位保护启动继电器字，清标志，作结束报告
				{
					WriteRelayWord(RW_PICKUP_AT2, RELAY_OFF);
					FaultCheckOutFlag_AT2 = FALSE;
					FaultOnFlag_AT2 = FALSE;
					HaveMakeStartRptFlag_AT2 = FALSE;
					DB_RelayEnd(RptSerialNumOfAct_AT2); 
					m_bFaultLKFlag_AT2 = FALSE;
					ResetAllRelay_AT2(TRUE);					
					m_bHaveMake52BFRpt_AT2 = FALSE;
					return;					
				}
				else
					return;
			}			
		}		
	}
}


void CRelayTask::ResetAllRelay_AT1(BOOL resetFlag)
{	
	m_bHaveMake52BFRpt_AT1 = FALSE;
	m_bHaveRelayAct_AT1 =FALSE;
	ResetOverValueRelay(&RelR50IC1,resetFlag);
	ResetOverValueRelay(&RelR50IT1,resetFlag);
	ResetOverValueRelay(&RelR50IF1,resetFlag);
	ResetOverValueRelay(&RelR50IP1,resetFlag);
	
}
void CRelayTask::ResetAllRelay_AT2(BOOL resetFlag)
{				
	m_bHaveMake52BFRpt_AT2 = FALSE;
	m_bHaveRelayAct_AT2 =FALSE;
	ResetOverValueRelay(&RelR50IC2,resetFlag);
	ResetOverValueRelay(&RelR50IT2,resetFlag);
	ResetOverValueRelay(&RelR50IF2,resetFlag);
	ResetOverValueRelay(&RelR50IP2,resetFlag);
	
}

void CRelayTask::Process52BF_AT1()
{
	if(ReadRelayWord(RW_52BF_AT1))
	{
		m_b52BFFlag_AT1 = TRUE;
	}
	if(m_b52BFFlag_AT1)
	{
		if(!ReadRelayWord(RW_52BF_AT1))
		{
			ResetAllRelay_AT1(TRUE);
			m_b52BFFlag_AT1 = FALSE;
		}
	}
}
void CRelayTask::Process52BF_AT2()
{
	if(ReadRelayWord(RW_52BF_AT2))
	{
		m_b52BFFlag_AT2 = TRUE;
	}
	if(m_b52BFFlag_AT2)
	{
		if(!ReadRelayWord(RW_52BF_AT2))
		{
			ResetAllRelay_AT2(TRUE);
			m_b52BFFlag_AT2 = FALSE;
		}
	}
}

