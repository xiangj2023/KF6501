/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			m5234timer.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           coldfire5234定时器驱动                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      5234 DMA定时器驱动接口，定时器0用作操作系统调度，定时器1用作辅助定时                         
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	DITimerInit			DI定时器初始化
*      	DITimerEnable		DI定时器允许
*      	DITimerDisable		DI定时器关闭
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/8/19    初始创建                         
*                                                                                  
********************************************************************************************/

#include "m5234timer.h"
#include "max11046.h"
#include "daedef.h"
#include "etpu_app.h"
#include "relay.h"

extern DWORD dwDIIntCnt;		//DI定时器中断计数
extern DWORD dwRelayTaskID;
extern DWORD dwCaluInterval;	//热过负荷计算累加间隔
#ifdef PRO_49I
	extern BOOL  g_bRel49Flag;		//热过负荷累加消息标志
	extern BOOL  g_bRel49TimeOut;
#endif
extern TSysConfigTable	*G_pCfgTable;

extern void DITimerInt();

DWORD dwRelayMSCnt;		//保护用毫秒计数器

/*********************************************************************************************
 *
 * 功能描述     DI定时器初始化
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
void DITimerInit()
{
	dwDIIntCnt = 0;//初始化DI定时中断计数

	//中断加载
//	OS_SetVector(MCF_INTC0_LVL_TMR1, DITimerInt, 0);
	
	DITimerDisable();
	DITimerEnable();

	OS_IntEnable(MCF_INTC0_LVL_TMR0);

	//设置中断优先级
	MCF_INTC0_ICRn(20) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(5);
}

/*********************************************************************************************
 *
 * 功能描述     DI定时器允许，进行分频计算，加载中断
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
void DITimerEnable()
{
	DWORD Prescale, ClkCnt;

	//禁止定时器
	MCF_TIMER_DTMR(DI_DMA_TIMER) = 0x00;

	//复位定时器计数值
	MCF_TIMER_DTCN(DI_DMA_TIMER) = 0x00;

	//计算分频值
	Prescale = (IN_SYS_CLOCK / DI_INT_TIMER_RATE) >> 16;
	ClkCnt = ((IN_SYS_CLOCK / (Prescale + 1))
		       / DI_INT_TIMER_RATE);

	//设置参考寄存器
	MCF_TIMER_DTRR(DI_DMA_TIMER) = ClkCnt;

	//启动定时器
	MCF_TIMER_DTMR(DI_DMA_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
			    	| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST \
			    	| (Prescale << 8));
	//清除DMA定时器中断屏蔽位
	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK20;
//	CLR_DMA_IMR_BIT(&DIIntTimer);
}

/*********************************************************************************************
 *
 * 功能描述     DI定时器关闭
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
void DITimerDisable()
{
	//置DMA定时器中断屏蔽位
	MCF_INTC0_IMRL |= MCF_INTC0_IMRL_INT_MASK20;

	MCF_TIMER_DTMR(DI_DMA_TIMER) = 0x00;
}

void RelayTimeInit()
{
	dwRelayMSCnt = 0;
	
//	MCF_GPIO_PDDR_TIMER = 0xff;

	//禁止定时器
	MCF_TIMER_DTMR(RELAY_MS_TIMER) = 0x00;
	
	//复位定时器计数值
	MCF_TIMER_DTCN(RELAY_MS_TIMER) = 0x00;
	
	//设置参考寄存器
	TimeSetDTRR(RELAY_MS_TIMER, RELAY_MS_RATE);
	
	//清除DMA定时器中断屏蔽位
//	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK21;
	
	//清除中断状态
	MCF_TIMER_DTER(RELAY_MS_TIMER) = MCF_TIMER_DTER_REF;

	//中断加载
	OS_SetVector(MCF_INTC0_LVL_TMR2, RelayTimeInt, 0);
	OS_IntEnable(MCF_INTC0_LVL_TMR2);

	//设置中断优先级
	MCF_INTC0_ICRn(21) = MCF_INTC0_ICRn_IL(2) | MCF_INTC0_ICRn_IP(5);

	//启动定时器,禁止产生中断
	MCF_TIMER_DTMR(RELAY_MS_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
					| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST);
}

//extern HANDLE			g_hWdogTaskID;

//保护定时中断
void RelayTimeInt(void * arg)
{
	static DWORD  counter;
	
	//注意必须清除中断状态
	MCF_TIMER_DTER(RELAY_MS_TIMER) = MCF_TIMER_DTER_REF;

	counter++;

	if( (counter&0x01) == 0 )
	{
		//保护用毫秒计数器累加
		dwRelayMSCnt++;

		//增加系统时间毫秒计数累加刷新
		if(G_pCfgTable != NULL)
		{
			G_pCfgTable->SystemStatus.stTime.Lo++;
			if(G_pCfgTable->SystemStatus.stTime.Lo == 0)
				G_pCfgTable->SystemStatus.stTime.Hi++;
		}
	}

	//每5毫秒检查一次AD中断是否正常
	if((counter%10) == 0)
	{
		CheckADStatus();
	}	

#ifdef PRO_49I
	if(g_bRel49Flag == TRUE)
	{
		if(dwCaluInterval != 0)
		{
			if((counter%dwCaluInterval) == 0)
				g_bRel49TimeOut = TRUE;
		}
		else
		{
			g_bRel49TimeOut = FALSE;
		}
	}
#endif
	//遥信采样中断处理暂时放在保护毫秒定时中断中实现
	DITimerInt();

 	// 每5毫秒发送一次镜像位信息
	//if((counter%40) == 0)
	//	OS_PostMessage(g_hWdogTaskID, SM_RELAYRTMB, 4, 0, 0, 0);
}

void ADTimeInit(void)
{

	//中断加载
	OS_SetVector(MCF_INTC0_LVL_TMR3, ADTimeInt, 0);

	//禁止定时器
	MCF_TIMER_DTMR(AD_START_TIMER) = 0x00;
	
	//复位定时器计数值
	MCF_TIMER_DTCN(AD_START_TIMER) = 0x00;
	
	//注意必须清除中断状态
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF;

	//设置参考寄存器
	TimeSetDTRR(AD_START_TIMER, AD_TIME_RATE);
	
	//启动定时器
	MCF_TIMER_DTMR(AD_START_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
					| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST);
	
	//清除DMA定时器中断屏蔽位
	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK22;
	
	OS_IntEnable(MCF_INTC0_LVL_TMR3);

	//设置中断优先级
	MCF_INTC0_ICRn(22) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(6);
}

//AD启动采样定时中断
void ADTimeInt(void * arg)
{
	//注意必须清除中断状态
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF;
	
	ADStartSamp();
}

//interval时间单位为微秒,根据定时间隔设置参考寄存器
void TimeSetDTRR(BYTE no, DWORD interval)
{
	DWORD dwReference;

	dwReference = (IN_SYS_CLOCK/1000000)*interval - 1;

	//设置参考寄存器
	MCF_TIMER_DTRR(no) = dwReference;
}


#ifdef _DBUG_IRIGB_

//定义B格式脉冲各种脉冲长度的计数限值
#define IRIG_8MS_H	(5000*8+100)
#define IRIG_8MS_L	(5000*8-100)
#define IRIG_2MS_H	(5000*2+100)
#define IRIG_2MS_L	(5000*2-100)
#define IRIG_5MS_H	(5000*5+100)
#define IRIG_5MS_L	(5000*5-100)

//定义B格式脉冲类型
#define IRIG_8MS		0x01
#define IRIG_5MS		0x02
#define IRIG_2MS		0x03
#define IRIG_INVALID	0x55

#define IRIGB_PULSE_H	0x55	//正脉冲标志
#define IRIGB_PULSE_L	0xaa	//负脉冲标志

BOOL bInResolve;		//是否开始进入B格式解析状态，即是否找到时钟头(连续2个8ms正脉冲)
DWORD dwCntPerInf;		//每个完整信息体内的脉冲计数累加值，用来估算捕捉的脉冲是否完整
BYTE bOldPulseSts;		//上次脉冲的状态(正或负)
BYTE bOldPulseType;		//上次脉冲类型(2、5、8毫秒)
BYTE bOldHPulseType;	//上次正脉冲类型
BOOL bIrigTimeValid;		//时间信息是否有效
TAbsTime IrigAbsTime;	//解析出的相对系统时间
TSysTime IrigSysTime;	//解析出的系统时间
DWORD dwOldDTCR;		//上次定时器捕捉寄存器值


//临时用AD启动定时器来测试B格式时钟解析过程
void IrigBTimeInit()
{
	//初始化参数
	bInResolve = FALSE;
	bIrigTimeValid = FALSE;
	dwOldDTCR = 0;
	bOldPulseType = IRIG_INVALID;
	
	//中断加载
	OS_SetVector(MCF_INTC0_LVL_TMR3, IrigBTimeInt, 0);

	//禁止定时器
	MCF_TIMER_DTMR(AD_START_TIMER) = 0x00;
	
	//复位定时器计数值
	MCF_TIMER_DTCN(AD_START_TIMER) = 0x00;
	
	//注意必须清除中断状态
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF|MCF_TIMER_DTER_CAP;

	//设置参考寄存器
//	TimeSetDTRR(AD_START_TIMER, AD_TIME_RATE);
	
	//启动定时器,捕捉输入脉冲任意边沿,15倍预分频,时钟频率为75000000/15=5MHz
	MCF_TIMER_DTMR(AD_START_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
			| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST|MCF_TIMER_DTMR_CE(3)|MCF_TIMER_DTMR_PS(15));
	
	//清除DMA定时器中断屏蔽位
	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK22;
	
	OS_IntEnable(MCF_INTC0_LVL_TMR3);

	//设置中断优先级
	MCF_INTC0_ICRn(22) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(6);
	
}

void IrigBTimeInt(void * arg)
{
	DWORD dwDTCR, dwPulseWide;
	BYTE bPulseType = IRIG_INVALID;
	//注意必须清除中断状态
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF;

	//读取当前捕获状态寄存器值
	dwDTCR = MCF_TIMER_DTCR(AD_START_TIMER);

	//计算本次脉冲宽度
	dwPulseWide = (dwDTCR > dwOldDTCR) ? (dwDTCR-dwOldDTCR) : (0xffffffff-dwOldDTCR+dwDTCR+1);

	//判断本次脉冲类型
	if( (dwPulseWide>IRIG_8MS_L) && (dwPulseWide<IRIG_8MS_H) )
		bPulseType = IRIG_8MS;

	if( (dwPulseWide>IRIG_5MS_L) && (dwPulseWide<IRIG_5MS_H) )
		bPulseType = IRIG_5MS;
	
	if( (dwPulseWide>IRIG_2MS_L) && (dwPulseWide<IRIG_2MS_H) )
		bPulseType = IRIG_2MS;

	
	
}

//读取当前B格式脉冲的电平状态
BOOL isIrigBPulsHigh()
{
	return TRUE;
}

#endif
