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
*           		保护算法代码     FOR  6582电铁自动投切装置                                                                  
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
*      yanzh             09/10/27   初始创建                         
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
extern BYTE 	RptSerialNum;			//报告序列号 
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
	
	//获取数据库保护测量值数据指针，以加快操作速度
	pSysRM = G_pCfgTable->DBaseConfig.pRM;
	pSysBI = G_pCfgTable->DBaseConfig.pBI;
	
	InitAllRelayMem();
	InitRealy();
	InitDCRecInfo();

	
	dwChanDataMask = ((0x01<<CHANNEL_6582_U1)|(0x01<<CHANNEL_6582_U2)|(0x01<<CHANNEL_6582_U3)|(0x01<<CHANNEL_6582_U4) \
					|(0x01<<CHANNEL_6582_U5)|(0x01<<CHANNEL_6582_U6)|(0x01<<CHANNEL_6582_U7)|(0x01<<CHANNEL_6582_U8) \
					|(0x01<<CHANNEL_6582_I1)|(0x01<<CHANNEL_6582_I2)|(0x01<<CHANNEL_6582_I3)|(0x01<<CHANNEL_6582_I4) \
					|(0x01<<CHANNEL_6582_I5)|(0x01<<CHANNEL_6582_I6)|(0x01<<CHANNEL_6582_I7)|(0x01<<CHANNEL_6582_I8));
//	dwRecDataMask = ((0x01<<CHANNEL_6582_IC1)|(0x01<<CHANNEL_6582_IC2));

	dwDCDataMask = ((0x01<<CHANNEL_DC_1)|(0x01<<CHANNEL_DC_2)|(0x01<<CHANNEL_DC_3)|(0x01<<CHANNEL_DC_4)| \
		(0x01<<CHANNEL_DC_5)|(0x01<<CHANNEL_DC_6));

	//初始化读取保护数据临时缓冲区
	dwSampBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_SAMPLE_LEN);	
//	dwRecBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2);	
//	dwComposeBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*2);	
	
	ASSERT(dwSampBuf != NULL);
	if(dwSampBuf == NULL)
	{
		LogError("CRelayTask,dwSampBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(dwSampBuf, 0, sizeof(sizeof(DWORD)*READ_SAMPLE_LEN));

//	ASSERT(dwRecBuf != NULL);
//	if(dwRecBuf == NULL)
//	{
//		LogError("CRelayTask,dwRecBuf",FILE_LINE,"malloc error.");
//		return;
//	}
//	memset(dwRecBuf, 0, sizeof(sizeof(DWORD)*RELAY_SAMP_INTERVAL*2));
	
//	ASSERT(dwComposeBuf != NULL);
//	if(dwComposeBuf == NULL)
//	{
//		LogError("CRelayTask,dwComposeBuf",FILE_LINE,"malloc error.");
//		return;
//	}
//	memset(dwComposeBuf, 0, sizeof(sizeof(DWORD)*READ_POINT_NUM*2));
	
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

	RelayTaskInitOK = TRUE;	
	m_bLogicChanged = FALSE;
	g_MaintSW = SW_pbyMAINT;
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
/*	 InitRelR50IC1();
	 InitRelR50IT1();
	 InitRelR50IF1();
	 InitRelR50IP1();
	 InitRelR50IC2();
	 InitRelR50IT2();
	 InitRelR50IF2();
	 InitRelR50IP2();*/
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
	
	//首先进行开出自检,每次保护任务检一路开出
	if(!m_bLogicChanged)
		BoBreakCheck();
	ReadSampleData(dwSampBuf,dwChanDataMask,0);
//	WriteComposeBuf(dwRecDataMask);
	CaluUI();          //根据采样数据，进算电量It,If,U1,U2	
//	FalutProcess();	//故障循环处理	
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
//void CRelayTask::WriteComposeBuf(DWORD dwMask)
//{
//	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，
//	{
//		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*9-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*10-RELAY_SAMP_INTERVAL+i];
//	}
//	for(LONG i=0;i<READ_POINT_NUM;i++)
//	{
//		dwComposeBuf[i]= dwSampBuf[READ_POINT_NUM*8+i] - dwSampBuf[READ_POINT_NUM*9+i];
//	}
//	
//	for(LONG i=0;i<RELAY_SAMP_INTERVAL;i++)    //最新回写点，每个合成通道点个数为SAMPLENUMOFRELTASK，
//	{
//		dwRecBuf[i] = dwSampBuf[READ_POINT_NUM*12-RELAY_SAMP_INTERVAL+i] - dwSampBuf[READ_POINT_NUM*13-RELAY_SAMP_INTERVAL+i];
//	}
//	for(LONG i=0;i<READ_POINT_NUM;i++)
//	{
//		dwComposeBuf[READ_POINT_NUM+i]= dwSampBuf[READ_POINT_NUM*11+i] - dwSampBuf[READ_POINT_NUM*12+i];
//	}

//	WriteComposeData(dwRecBuf, dwMask);
//}
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
//	CaluBaseFourier_S15(dwComposeBuf,&IC1);
//	CaluModValue(&IC1);
//	*RM_pnIC1= IC1.Mod;                                       
//	DB_LogWriteRM(RM_IC1, *RM_pnIC1);	
//	// 16. Ic2
//	CaluBaseFourier_S15(dwComposeBuf+READ_POINT_NUM,&IC2);
//	CaluModValue(&IC2);
//	*RM_pnIC2= IC2.Mod;                                       
//	DB_LogWriteRM(RM_IC2, *RM_pnIC2);

}
/****************************************************************************************
名  称: InitRelR50IC1
功  能: 初始化差流1动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::InitRelR50IC1()
//{
//	RelR50IC1.pEleSet			= RS_pn60I_AT1C;						//保护定值指针 
//	RelR50IC1.pTEleVal			= &IC1;						//电量指针 
//	RelR50IC1.ACTRelayWord		= RW_TR60I_AT1;					//动作继电器字 
//	RelR50IC1.StartRelayWord	= RW_PI60I_AT1;					//入段继电器字
//	RelR50IC1.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//}
 /****************************************************************************************
 名  称: InitRelR50IT1
 功  能: 初始化1ATT线过流动作元件
 参  数: 无
 返回值: 无
 *****************************************************************************************/
// void CRelayTask::InitRelR50IT1()
//{
//	RelR50IT1.pEleSet			= RS_pn50T_AT1C;						//保护定值指针 
//	RelR50IT1.pTEleVal			= &I1;						//电量指针 
//	RelR50IT1.ACTRelayWord		= RW_TR50T_AT1;					//动作继电器字 
//	RelR50IT1.StartRelayWord	= RW_PI50T_AT1;					//入段继电器字
//	RelR50IT1.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//} 
/****************************************************************************************
名  称: InitRelR50IF1
功  能: 初始化1ATF线过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::InitRelR50IF1()
//{
//	RelR50IF1.pEleSet			= RS_pn50F_AT1C;						//保护定值指针 
//	RelR50IF1.pTEleVal			= &I2;						//电量指针 
//	RelR50IF1.ACTRelayWord		= RW_TR50F_AT1;					//动作继电器字 
//	RelR50IF1.StartRelayWord	= RW_PI50F_AT1;					//入段继电器字
//	RelR50IF1.wExcepWordNum	= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//} 
/****************************************************************************************
名  称: InitRelR50IP1
功  能: 初始化1AT碰壳过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::InitRelR50IP1()
//{
//	RelR50IP1.pEleSet			= RS_pn50CS_AT1C;						//保护定值指针 
//	RelR50IP1.pTEleVal			= &I3;						//电量指针 
//	RelR50IP1.ACTRelayWord		= RW_TR50CS_AT1;					//动作继电器字 
//	RelR50IP1.StartRelayWord	= RW_PI50CS_AT1;					//入段继电器字
//	RelR50IP1.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//}
/****************************************************************************************
名  称: InitRelR50IC2
功  能: 初始化2AT差流过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::InitRelR50IC2()
//{
//	RelR50IC2.pEleSet			= RS_pn60I_AT2C;						//保护定值指针 
//	RelR50IC2.pTEleVal			= &IC2;						//电量指针 
//	RelR50IC2.ACTRelayWord		= RW_TR60I_AT2;					//动作继电器字 
//	RelR50IC2.StartRelayWord	= RW_PI60I_AT2;					//入段继电器字
//	RelR50IC2.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//}
/****************************************************************************************
名  称: InitRelR50IT2
功  能: 初始化2ATT线过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::InitRelR50IT2()
//{
//	RelR50IT2.pEleSet			= RS_pn50T_AT2C;						//保护定值指针 
//	RelR50IT2.pTEleVal			= &I4;						//电量指针 
//	RelR50IT2.ACTRelayWord		= RW_TR50T_AT2;					//动作继电器字 
//	RelR50IT2.StartRelayWord	= RW_PI50T_AT2;					//入段继电器字
//	RelR50IT2.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//}
/****************************************************************************************
名  称: InitRelR50IF2
功  能: 初始化2ATF线过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/

//void CRelayTask::InitRelR50IF2()
//{
//	RelR50IF2.pEleSet			= RS_pn50F_AT2C;						//保护定值指针 
//	RelR50IF2.pTEleVal			= &I5;						//电量指针 
//	RelR50IF2.ACTRelayWord		= RW_TR50F_AT2;					//动作继电器字 
//	RelR50IF2.StartRelayWord	= RW_PI50F_AT2;					//入段继电器字
//	RelR50IF2.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//}
/****************************************************************************************
名  称: InitRelR50IP2
功  能: 初始化2AT碰壳过流动作元件
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::InitRelR50IP2()
//{
//	RelR50IP2.pEleSet			= RS_pn50CS_AT2C;						//保护定值指针 
//	RelR50IP2.pTEleVal			= &I6;						//电量指针 
//	RelR50IP2.ACTRelayWord		= RW_TR50CS_AT2;					//动作继电器字 
//	RelR50IP2.StartRelayWord	= RW_PI50CS_AT2;					//入段继电器字
//	RelR50IP2.wExcepWordNum		= 0;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 

//}
void CRelayTask::InitStructCheck(void)
{
	WORD acc=0x55aa;
/*	StructCheck[0].pstruct = (WORD*)&RelR50IC1;
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
	StructCheck[7].bigsize = sizeof(RelR50IP2)/2 - RelR50IP2.wExcepWordNum; */
	StructCheck[0].pstruct = (WORD*)SW_BUF_ADDR;
	StructCheck[0].bigsize = (CF_BUF_ADDR+CF_BUF_SIZE-SW_BUF_ADDR)/2;
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

/****************************************************************************************
名  称: FalutProcess
功  能: 故障循环处理函数
参  数: 无
返回值: 无
*****************************************************************************************/
//void CRelayTask::FalutProcess(void)
//{ 	
//	SimpleOverValRelay(&RelR50IC1);
//	SimpleOverValRelay(&RelR50IT1);
//	SimpleOverValRelay(&RelR50IF1);
//	SimpleOverValRelay(&RelR50IP1);
//	SimpleOverValRelay(&RelR50IC2);
//	SimpleOverValRelay(&RelR50IT2);
//	SimpleOverValRelay(&RelR50IF2);
//	SimpleOverValRelay(&RelR50IP2);
//}
