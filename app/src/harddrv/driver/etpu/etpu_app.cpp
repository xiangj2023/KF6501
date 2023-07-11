/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			etpu_app.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           ETPU功能模块                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      根据系统应用要求，利用ETPU的基本功能库，完成测频、测温、B格式解析、系统电源偏低监视等功能                         
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	EtpuInit			ETPU应用初始化
*      	EtpuFreqProc		频率测量处理
*      	EtpuPfoInt			系统12V电源电压偏低监视中断处理
*		EtpuTempProc		系统温度测量处理
*		EtpuIrigInt			B格式解析中断处理
*
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               09/06/9    初始创建                         
*                                                                                  
********************************************************************************************/
#include "etpu_app.h"
#include "led.h"
#include "etpu_cfg.h"
#include "daefunc.h"
#include "relay.h"
#include "relayfun.h"
#include "dbfunc.h"
extern int EtpuInit();

extern DWORD dwRelayTaskID;

int SysTempture;		//装置温度值,放大10倍
TIrigBInt IrigBInfor;
DWORD dwIrigIntCnt = 0;

TFreqData FreqData[MAX_FREQ_NUM];

LONG FreqBubbleData[FREQ_DATA_BUF];


DWORD g_dwFreqPWM = 1600000;
BYTE IrigQuality = 0;	//B格式信号质量
BYTE IrigBFlag;


/*********************************************************************************************
 *
 * 功能描述     ETPU功能初始化
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                初始化结果，0成功，其它失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
int EtpuInit()
{
	int rtn;

	memset((void*)FreqData, 0, sizeof(TFreqData)*MAX_FREQ_NUM);
	//根据类型指定的频率调整范围
	if(FREQ_TYPE == FREQ_TYPE_PRO)
	{
		FreqData[0].dwUpLimit = P_FREQ_UPPER_LIMIT;
		FreqData[0].dwLowLimit = P_FREQ_LOWER_LIMIT;
	}
	else
	{
		FreqData[0].dwUpLimit = M_FREQ_UPPER_LIMIT;
		FreqData[0].dwLowLimit = M_FREQ_LOWER_LIMIT;
	}
	
	//第二路频率固定为同期使用，范围固定
	FreqData[1].dwUpLimit = M_FREQ_UPPER_LIMIT;
	FreqData[1].dwLowLimit = M_FREQ_LOWER_LIMIT;
	
	
	//B格式信号解析中断加载
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_IRIG1, EtpuIrigInt, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_IRIG1);
	MCF_INTC1_ICRn(27+ETPU_IRIG1) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(6);

	
	//系统电源偏低监视中断加载
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_PFO, EtpuPfoInt, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_PFO);
	MCF_INTC1_ICRn(27+ETPU_PFO) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(7);


	//温度检测改用中断方式实现，采用GPIO方式，需要测试
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_TEMP, EtpuTempProc, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_TEMP);
	MCF_INTC1_ICRn(27+ETPU_TEMP) = MCF_INTC1_ICRn_IL(1) | MCF_INTC1_ICRn_IP(5);
/*
	//加载频率测量中断
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_FREQ1, EtpuFreq1Int, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_FREQ1);
	MCF_INTC1_ICRn(27+ETPU_FREQ1) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(5);

	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_FREQ2, EtpuFreq2Int, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_FREQ2);
	MCF_INTC1_ICRn(27+ETPU_FREQ2) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(4);
*/	
	//初始化ETPU
	rtn = (int)my_system_etpu_init();
	if(rtn)
		return rtn;

	//启动ETPU
	my_system_etpu_start();

	IrigBFlag = DB_WhichIRGB();
	
	return rtn;
}

/*********************************************************************************************
 *
 * 功能描述     第一路频率测量中断处理，本通道用于控制AD采样的频率，当频率值越限时，按照上次的
 *				有效值控制AD采样，当连续0.5秒钟没有本中断，认为频率为0，按照50Hz控制采样,此时需要
 *				保证频率显示为0.
 *
 * 参数说明      - arg	: 输入 	
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void EtpuFreq1Int(void *arg)
{
	EtpuFreqProc(0);
	//根据输入量频率值调整AD采样频率
//	if(EtpuFreqProc(0) == TRUE)
//		ADStartFreqUpdate(FreqData[0].dwFreq);
//	ADStartFreqUpdate(g_dwFreqPWM);
//	ADStartFreqUpdate(g_dwFreqPWM);

}

/*********************************************************************************************
 *
 * 功能描述     第二路频率测量中断处理，本通道用于同期控制，当连续1秒钟没有本中断，认为频率为0，
 *				频率显示为0.
 *
 * 参数说明      - arg	: 输入 	
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void EtpuFreq2Int(void *arg)
{
	EtpuFreqProc(1);
}

/*********************************************************************************************
 *
 * 功能描述     ETPU测频处理函数，通过GPIO输入功能实现，中断方式捕获任意边沿。计算出两个通向
 *				边沿之间的时间，然后算出频率值
 *
 * 参数说明      - channel	: 输入 	ETPU通道序号
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL EtpuFreqProc(BYTE channel)
{
	TFreqData *pFreq;
	DWORD value;
	DWORD counter;

	MCF_ETPU_ECnSCR(ETPU_FREQ1+channel) = 0xC0C00000;	//清除中断
	pFreq = &FreqData[channel];

	pFreq->dwIntCnt++;
	if(pFreq->dwIntCnt&0x01)
		return FALSE;
	 
	//根据当前时钟计数，计算周期时间
	counter = MCF_ETPU_ETB1R;	//读取基准时钟的当前时钟计数，24位
	value = (0x1000000+counter-pFreq->dwOldCounter)%0x1000000;
/*
	static LONG ttcounter = 0;
	TestBuf[ttcounter] = value;
	ttcounter++;
	if(ttcounter==200)
		ttcounter = 0;

*/
	
	pFreq->dwOldCounter = counter;
	
	//检查测量值是否合法,不合法本次测量无效并丢弃
	if( (value>pFreq->dwUpLimit) || (value<pFreq->dwLowLimit) )
	{
		pFreq->dwErrCnt++;
		return FALSE;
	}

	//缓存历史脉冲计数，进行简单滤波处理

//	value = GetSortData(value,FREQ_DATA_BUF,(LONG*)pFreq->dwPulseLen,FreqBubbleData);
//	pFreq->dwPulseLen[2] = pFreq->dwPulseLen[1];
//	pFreq->dwPulseLen[1] = pFreq->dwPulseLen[0];
//	pFreq->dwPulseLen[0] = value;

//	value = GetMidFreqValue(channel);
	if(value == 0)
		return FALSE;
	
	//计算脉冲宽度，单位微秒,公式:x/(37.5/4)即8x/75
	pFreq->dwPeriod = (value<<3)/75;

	//计算启动AD采样的PWM的频率
	pFreq->dwFreq = (300000000*10)/value;
	return TRUE;

}


DWORD GetMidFreqValue(BYTE channel)
{
	DWORD max = 0;
	DWORD min = 0x1000000;
	DWORD middle = 0;
	DWORD value;
	
	for(int i=0; i<FREQ_DATA_BUF; i++)
	{
		value = FreqData[channel].dwPulseLen[i];
		middle += value;
		if(max<value)
			max = value;
		if(min>value)
			min = value;
	}

	middle -= (max+min);
	return (middle/(FREQ_DATA_BUF-2));
}

/*********************************************************************************************
 *
 * 功能描述     定时频率测量通道监视，当没有频率信号超过0.5秒时，频率显示为0，且对于第一路频率
 *				通道，如果没有频率信号，按照50Hz控制AD采样频率
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *               无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void FreqChanMonitor()
{
	static DWORD dwOldFreqIntCnt[MAX_FREQ_NUM];

	//通过检查各频率通道的中断计数是否变化，来判断是否有频率信号，没有则将频率值清零
	for(int i=0; i<MAX_FREQ_NUM; i++)
	{
		if(dwOldFreqIntCnt[i] == FreqData[i].dwIntCnt)
		{
			FreqData[i].dwFreq = 0;
			FreqData[i].dwPeriod = 0;
			FreqData[i].dwOldCounter = 0;
		}
		dwOldFreqIntCnt[i] = FreqData[i].dwIntCnt;
	}

	//如果第一路频率值为0，则按照50Hz调整
//	if(FreqData[0].dwFreq == 0)
//		ADStartFreqUpdate(1600*10);
}
/*********************************************************************************************
 *
 * 功能描述     根据通道号取测量到的频率值，放大100倍
 *				
 *
 * 参数说明      - channel	: 输入 	ETPU通道序号
 *				 
 *
 * 返回代码
 *               频率值
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD GetFreqValue(BYTE channel)
{
	if(channel>1)
		return 0;
	if(FreqData[channel].dwPeriod == 0)
		return 0;
	else
		return (100000000/FreqData[channel].dwPeriod);
}

/*********************************************************************************************
 *
 * 功能描述     ETPU系统电源偏低监视中断函数，采用GPIO的输入捕捉方式，监视下降沿,任意沿中断
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void EtpuPfoInt(void *arg)
{
	MCF_ETPU_ECnSCR(ETPU_PFO) = 0xC0C00000;	//清除中断

	//如果当前状态为低，说明电源电压偏低发生
	if(~(MCF_ETPU_ECnSCR(ETPU_PFO)&MCF_ETPU_ECnSCR_IPS))	
	{
		//暂时通过消息通知保护任务进行相应处理.
		//OS_PostMessage(dwRelayTaskID, (WORD)SM_POWERDOWN, 0,0,0,0);
		DAE_SetRunningFlags(SS_12V_ERR);
	}
	else//电源正常
	{
		DAE_ClearRunningFlags(SS_12V_ERR);
	}
}

/*********************************************************************************************
 *
 * 功能描述     ETPU测温处理函数，采用GPIO的输入捕捉方式，捕捉任意边沿，由于温度芯片输出为标准
 *				方波，因此实际上按照半周期测量进行计算。
 *				温度芯片MAX6576，计算公式为:时间(us)/640 - 273.15
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void EtpuTempProc(void *arg)
{
	static DWORD dwOldTempCnt;
	DWORD dwCurCnt;
	int value;
	MCF_ETPU_ECnSCR(ETPU_TEMP) = 0xC0C00000; //清除中断

	dwCurCnt = MCF_ETPU_ETB1R;		//读取基准时钟的当前时钟计数，24位

	//根据与上次计数差值，计算脉冲计数个数
	if(dwCurCnt>dwOldTempCnt)
		value = dwCurCnt-dwOldTempCnt;
	else
		value = 0x1000000 + dwCurCnt - dwOldTempCnt;
	dwOldTempCnt = dwCurCnt;

	//计算温度值，放大10倍,注意不管采用哪种边沿捕捉实际上都捕捉了任意边沿，因此测量的是半个周期的时间长度
	value /= 300;
	value -= 2732;
	if( (value>-1000) && (value<1000))
		SysTempture = value;
}

/*********************************************************************************************
 *
 * 功能描述     B格式时钟解析中断处理，采用GPIO的输入功能，捕捉任意边沿并产生中断。
 *				只处理上升沿计算负脉冲的长度，并判断脉冲类型，找到整秒的开始后，解析时间信息
 *				并在捕获到新的P0的上升沿时，将上一秒解析出的时间信息加1秒后写入系统时间.
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void EtpuIrigInt(void * arg)
{
	BYTE type = IRIG_B_ERR;
	DWORD value, dwCurCnt;
	static DWORD dwOldCnt;

	MCF_ETPU_ECnSCR(ETPU_IRIG1) = 0xC0C00000;	//清除中断

	//读取捕获到的负脉冲类型

	dwCurCnt = MCF_ETPU_ETB1R;		//读取基准时钟的当前时钟计数，24位
	
	//计算时钟计数差
	if(dwCurCnt>dwOldCnt)
		value = dwCurCnt - dwOldCnt;
	else
		value = 0x1000000 + dwCurCnt - dwOldCnt;
	dwOldCnt = dwCurCnt;

	//根据需要，只处理负脉冲，正脉冲跳过
	if(MCF_ETPU_ECnSCR(ETPU_IRIG1)&MCF_ETPU_ECnSCR_IPS)
	{
		//根据脉冲计数，判断脉冲类型
		if((value>PULSE_2MS_L)&&(value<PULSE_2MS_H))
			type = IRIG_B_2MS;
		else if((value>PULSE_5MS_L)&&(value<PULSE_5MS_H))
			type = IRIG_B_5MS;
		else if((value>PULSE_8MS_L)&&(value<PULSE_8MS_H))
			type = IRIG_B_8MS;
		
		
		switch(type)
		{
			case IRIG_B_ERR:
				memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
				break;
			case IRIG_B_8MS:	// 0
			case IRIG_B_5MS:	// 1
				//如果没有进入解析状态，结束
				if(IrigBInfor.isInResolve == FALSE)
					break;
				
				//最后一个信息字的最后一个信息位进行时间格式转化和加1秒处理
				//解析位信息
				if(type == IRIG_B_5MS)
					IrigBInfor.bIrigWordInf[IrigBInfor.bCurPulseWordNum] |= \
										(0x01<<IrigBInfor.bCurPulseBitNum);
				IrigBInfor.bCurPulseBitNum++;
				
				if( ( IrigBInfor.bCurPulseWordNum == (IRIG_B_WORD_NUM-1) ) && \
					( IrigBInfor.bCurPulseBitNum == (IRIG_B_BIT_NUM) ) )
				{	
					//将解析出的上一秒时间转换为相对时间
					IrigBToAbsTime();

					//用上一秒解析的时间+1秒作为新时间更新系统时间
					value = IrigBInfor.OldAbsTime.Lo;
					IrigBInfor.OldAbsTime.Lo += 1000;
					if(IrigBInfor.OldAbsTime.Lo<value)
						IrigBInfor.OldAbsTime.Hi++;
					IrigBInfor.isAbsTimeOk = TRUE;
				}
				
				if(IrigBInfor.bCurPulseBitNum>IRIG_B_BIT_NUM)
				{
					//初始化相关状态及标志
					memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
				}
				dwIrigIntCnt++;
				break;
			case IRIG_B_2MS:	//标示位
				//检查是否为P0,通过检查时钟信息是否有效，即上个脉冲为最后信息字的最后信息位
				//这样处理可以尽快响应整秒的开始位置
				if(IrigBInfor.isAbsTimeOk == TRUE)
				{
					//更新系统时间
					WriteAbsTime(&IrigBInfor.OldAbsTime);
					
					//初始化相关状态及标志
					memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
//					IrigBInfor.bOldPulseType = type;
					IrigBInfor.isInResolve = FALSE;
					break;
				}
				if(IrigBInfor.bOldPulseType == IRIG_B_2MS)	//是Pr信息
				{
					IrigBInfor.isInResolve = TRUE;	//找到信息开始
					IrigBInfor.bCurPulseBitNum = 0;
					IrigBInfor.bCurPulseWordNum = 0;
				}
				//P0~P9
				else if((IrigBInfor.bOldPulseType == IRIG_B_8MS)|| \
						(IrigBInfor.bOldPulseType == IRIG_B_5MS) )
				{
					if(IrigBInfor.isInResolve == TRUE)
					{
						//进入下一个信息字的开始
						IrigBInfor.bCurPulseBitNum = 0;
						IrigBInfor.bCurPulseWordNum++;
						if(IrigBInfor.bCurPulseWordNum>IRIG_B_WORD_NUM)
						{
							//初始化相关状态及标志
							memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
						}
					}
				}
				else if(IrigBInfor.bOldPulseType == IRIG_B_ERR)
				{
					memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
					IrigBInfor.isInResolve = FALSE;
				}
				dwIrigIntCnt++;
				break;
		}
		
		IrigBInfor.bOldPulseType = type;
	}
}


/*********************************************************************************************
 *
 * 功能描述     将解析出的B格式时钟信息转换为相对于1970年的毫秒数
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void IrigBToAbsTime()
{
	int t, year, day, hour, minute, second;
	DWORD i = 0;
	static int OldYear;
	BYTE PlanetNum;

	//为了满足KF6500系统B格式标准和KF3500系统B格式标准，需要根据配置进行两种数据的分包解析
	//对于3500系统，第5、6、7个字节内容有差异
	//计算年数字

	if(IrigBFlag)	//KF3500
	{
		year = 2000 + (IrigBInfor.bIrigWordInf[IRIG_35_YEAR]&0x0f) + \
				((IrigBInfor.bIrigWordInf[IRIG_35_YEAR]>>5)&0x0f)*10;

		//卫星数
		PlanetNum = (IrigBInfor.bIrigWordInf[IRIG_35_PLANET]&0x0f) + \
				((IrigBInfor.bIrigWordInf[IRIG_35_PLANET]>>5)&0x0f)*10;

		//根据卫星数设置时间信息质量
		if(PlanetNum>3)
			IrigQuality = 0;
		else if(PlanetNum>1)
		{
			IrigQuality = 3;
		}
		else
		{
			IrigQuality = 10;
		}
	}
	else	//KF6500
	{
		year = 2000 + (IrigBInfor.bIrigWordInf[IRIG_B_YEAR]&0x0f) + \
				((IrigBInfor.bIrigWordInf[IRIG_B_YEAR]>>5)&0x0f)*10;
		
		//同时刷新B格式信号质量
		IrigQuality = (IrigBInfor.bIrigWordInf[IRIG_B_QT]>>1)&0x0f;
	}
	
	//计算日数字
	day = (IrigBInfor.bIrigWordInf[IRIG_B_DAY]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_DAY]>>5)&0x0f)*10+\
			(IrigBInfor.bIrigWordInf[IRIG_B_H_DAY]&0x03)*100;

	//计算小时数字
	hour = (IrigBInfor.bIrigWordInf[IRIG_B_HOUR]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_HOUR]>>5)&0x07)*10;
	
	//计算分钟数字
	minute = (IrigBInfor.bIrigWordInf[IRIG_B_MIN]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_MIN]>>5)&0x07)*10;

	//计算秒数字
	second = (IrigBInfor.bIrigWordInf[IRIG_B_SEC]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_SEC]>>5)&0x07)*10;
	
    for(t=1970;t<year;t++)
    {
        if((t & 0x03)==0)
            i+=LeapYearSecond;
        else
            i+=NoneLeapYearSecond;
    }

	i += (day-1)*86400;
	i += hour*3600;
	i += minute*60;
	i += second;

	//将秒时间转换为毫秒时间
	SecondToMSecond(i, 0, &IrigBInfor.OldAbsTime);

}


 /*********************************************************************************************
  *
  * 功能描述	 检查B格式信号是否正常
  *
  * 参数说明	  无
  * 			  
  *
  * 返回代码
  * 			   无
  *
  * 其它说明：			
  *
 **********************************************************************************************/
 void isIrigOk()
 {
 	static DWORD dwPreCnt;

	//通过检查一段时间内B格式信号脉冲个数是否变化判断是否正常
	if(dwIrigIntCnt == dwPreCnt)	//计数无变化，说明信号异常	
	{
		DAE_SetRunningFlags(SS_TIME_B);
	}
	else	//正常
	{
		DAE_ClearRunningFlags(SS_TIME_B);
	}
	dwPreCnt = dwIrigIntCnt;
 }

/*
BYTE GetIrigPulseType()
{
	static DWORD dwOldCnt;
	DWORD dwCurCnt, value;

	dwCurCnt = MCF_ETPU_ETB1R;		//读取基准时钟的当前时钟计数，24位
	
	//计算时钟计数差
	if(dwCurCnt>dwOldCnt)
		value = dwCurCnt - dwOldCnt;
	else
		value = 0x1000000 + dwCurCnt - dwOldCnt;
	dwOldCnt = dwCurCnt;

	//根据脉冲计数，判断脉冲类型
	if((value>PULSE_2MS_L)&&(value<PULSE_2MS_H))
		return IRIG_B_2MS;
	
	if((value>PULSE_5MS_L)&&(value<PULSE_5MS_H))
		return IRIG_B_5MS;
	
	if((value>PULSE_8MS_L)&&(value<PULSE_8MS_H))
		return IRIG_B_8MS;
	
	return IRIG_B_ERR;
}
*/

/*
void EtpuPWMInt(void * arg)
{
	iPwmIntCnt++;
	dwICCounter[3] = (DWORD)eTPU->CHAN[ETPU_IC].SCR.R;
	dwICCounter[4] = (DWORD)eTPU->CHAN[ETPU_PWM].SCR.R;

	// Erase interrupt flag 
	eTPU->CISR_A.R |= (0x01<<ETPU_PWM);

	if(iPwmIntCnt%2)
		outportb(LED1_CS_BASE,0xff);
	else
		outportb(LED1_CS_BASE,0x00);
}

void EtpuICInt(void * arg)
{
	iICIntCnt++;
	
	eTPU->CISR_A.R |= (0x01<<ETPU_IC);
	dwICCounter[0] = (DWORD)fs_etpu_ic_read_trans_count(ETPU_IC);
	dwICCounter[1] = (DWORD)fs_etpu_ic_read_final_time(ETPU_IC);
	dwICCounter[2] = (DWORD)fs_etpu_ic_read_last_time(ETPU_IC);
	
	if(iICIntCnt%2)
		outportb(LED2_CS_BASE,0xff);
	else
		outportb(LED2_CS_BASE,0x00);
	
}

void ReadEtpuICValue()
{
	dwICCounter[0] = (DWORD)fs_etpu_ic_read_trans_count(ETPU_TEMP);
	dwICCounter[1] = (DWORD)fs_etpu_ic_read_final_time(ETPU_TEMP);
	dwICCounter[2] = (DWORD)fs_etpu_ic_read_last_time(ETPU_TEMP);
	printf("etpu trans=%d final=%x last=%d.\n", dwICCounter[0],dwICCounter[1],dwICCounter[2]);
}
*/
/*********************************************************************************************
 *
 * 功能描述 	检查B格式信号是否正常
 *
 * 参数说明 	 无
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/

